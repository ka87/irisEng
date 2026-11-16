#!/bin/sh
# The differential test for win/wingdi.c.
#
#   ./tests/run_wingdi.sh
#
# tests/run.sh calls this script, so there is no second copy of the
# symbol list; running it on its own is the quick way to work on it.
#
# The frozen copy and the rewrite go into one binary with BASE_/NEW_
# prefixes on every function the file defines and on the eleven globals
# it defines strongly -- everything else it declares is a tentative
# definition and is deliberately shared, so the two sides are compared
# over the same renderer state.  The Direct3D device is a logging mock
# vtable; the real math.c and profiler.c are linked rather than mocked,
# with MATH_InitModule called before the first case.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
# both sides see the real SDK's window-message numbers: the stub's are
# sequential, which inverts their order and makes most of the two
# message handlers unreachable.  See tests/wingdi_msg_sdk.h.
CFLAGS="$CFLAGS -include tests/wingdi_msg_sdk.h"
OUT=tests/build
mkdir -p "$OUT"

syms=$(python3 - <<'EOF'
import re
src = open('win/wingdi.c', encoding='latin-1').read()
names = []
for m in re.finditer(r'^[A-Za-z_][\w \*]*?[\w\*]\s+\**(\w+)\s*\(', src, re.M):
    names.append(m.group(1))
names += ['d3dProjectionMtx', 'd3dTransformMatrix', 'd3dTransformMatrix2',
          'engineFogTableModeToD3D', 'rasterNames', 'worldPtrIfMultiStream',
          'D3D_gi_FlatLayerBox', 'D3D_gi_AspectTrace',
          'D3D_gf_CutsceneBand']
seen = []
for n in names:
    if n not in seen and n not in ('if', 'for', 'while', 'switch', 'return', 'sizeof'):
        seen.append(n)
print(' '.join(seen))
EOF
)

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

echo "=== wingdi ==="
$CC $CFLAGS -c win/wingdi.c -o "$OUT/wingdi.new.o" $(defs NEW_ $syms)
# the frozen copy still spells one function the way the decompiler named
# it; map it onto the same per-side symbol the rewrite's new name takes
# Only the frozen copy sees a D3DX header: the engine stopped
# including one when the four matrix calls and the two texture calls
# were reimplemented, and the baseline still refers to all of them.
# Forcing it in here rather than for both sides is what keeps that
# independence honest -- if the live file still needed D3DX, it would
# not compile.
$CC $CFLAGS -include tests/stub/d3dx9math.h \
    -include tests/wingdi_baseline_compat.h \
    -DMAT_ComputeUV_sub_4521A0=BASE_MAT_UV_ComputeAndSetTransform \
    -DD3D_OutputDebugString=GDI_OutputDebugString \
    -Dpd3drp=rendererState \
    -Dd3dvb2=vertexBuffer2 -Dd3dvb=vertexBuffer \
    -Dd3dUVVBNum=uvBufferNum -Dd3dUVVB=uvBuffers \
    -Dd3dIndexBuf=indexBuffer \
    -DD3D_gi_Widescreen=GDI_gi_Widescreen \
    -DD3D_gf_RefYoverX=GDI_gf_RefYoverX \
    -DtD3dRequestSnapshot=tGdi_SnapshotRequest \
    -DsD3dRequestSnapshot=sGdi_SnapshotRequest \
    -c tests/baseline_wingdi.c -o "$OUT/wingdi.base.o" $(defs BASE_ $syms)
$CC $CFLAGS -c math.c -o "$OUT/wingdi.math.o"
# The real matrix module, linked rather than mocked: it is pure
# arithmetic with nothing to stand in for. win/wingditex.c is not
# linked -- see the stand-ins in tests/wingdi_mocks.c for why.
$CC $CFLAGS -c win/wingdimath.c -o "$OUT/wingdi.wgmath.o"
# The camera fitting and the projections came out of win/wingdi.c into
# projection.c, which is arithmetic with nothing to stand in for, so it
# is linked. The frozen copy still has its own inline copy, which is
# what makes this comparison worth running.
$CC $CFLAGS -c projection.c -o "$OUT/wingdi.proj.o"
# The vertex building came out of win/wingdi.c into geovertex.c, and is
# linked for the same reason: array walking and colour packing with
# nothing to stand in for. The frozen copy still has its own inline
# copy of both.
$CC $CFLAGS -c geovertex.c -o "$OUT/wingdi.gvx.o"
# The sprite quad building came out of win/wingdi.c into geosprite.c,
# and is linked for the same reason: walking the engine's arrays with
# nothing to stand in for. The frozen copy still has its own inline
# copy.
$CC $CFLAGS -c geosprite.c -o "$OUT/wingdi.gsp.o"
# rasterstate.c states the per-draw flags word's meaning once, for
# both renderers. Unlike projection.c and geovertex.c it is not a move:
# this renderer still reads the bits itself, and the driver's
# rasterStateCase compares the two readings over every combination of
# them. Linked for that check, which is why it is pure.
$CC $CFLAGS -c rasterstate.c -o "$OUT/wingdi.rst.o"
# watermesh.c states the water surface's index strip, vertex stream and
# reflection remap once, for both renderers. It is a move out of
# win/wingdi.c, so the frozen copy still has all three inline and the
# water cases below compare the two -- including the bytes each side
# writes into the locked index and vertex buffers, which is what proves
# the shared copy rather than merely exercising it.
$CC $CFLAGS -c watermesh.c -o "$OUT/wingdi.wms.o"
# waterreflect.c is the reflection pass's own share: the mirrored
# transform, the size threshold and the clip plane. Also a move out
# of win/wingdi.c, so the frozen copy still has all three inline.
$CC $CFLAGS -c waterreflect.c -o "$OUT/wingdi.wrf.o"
# watersmooth.c holds the surface smoothing's rectangles, steps and
# taps for both renderers; tests/run_wingdi_ae.sh is where it is
# compared against the other renderer.
$CC $CFLAGS -c watersmooth.c -o "$OUT/wingdi.wsm.o"
# depthblur.c comes with it: the smoothing's weights are the same
# chain the depth blur's taps are weighed by, so it asks that file
# for them rather than carrying a second copy.
$CC $CFLAGS -c depthblur.c -o "$OUT/wingdi.dpb.o"
$CC $CFLAGS -c fog.c -o "$OUT/wingdi.fog.o"
$CC $CFLAGS -c profiler.c -o "$OUT/wingdi.profiler.o"
$CC $CFLAGS -c tests/wingdi_mocks.c -o "$OUT/wingdi.mocks.o"
$CC $CFLAGS -c tests/test_wingdi.c -o "$OUT/wingdi.drv.o"
$CC "$OUT/wingdi.new.o" "$OUT/wingdi.base.o" "$OUT/wingdi.math.o" \
    "$OUT/wingdi.wgmath.o" "$OUT/wingdi.proj.o" "$OUT/wingdi.gvx.o" \
    "$OUT/wingdi.rst.o" "$OUT/wingdi.fog.o" "$OUT/wingdi.gsp.o" "$OUT/wingdi.profiler.o" \
    "$OUT/wingdi.wms.o" "$OUT/wingdi.wrf.o" "$OUT/wingdi.wsm.o" "$OUT/wingdi.dpb.o" \
    "$OUT/wingdi.mocks.o" \
    "$OUT/wingdi.drv.o" \
    -o "$OUT/wingdi" -lm
"$OUT/wingdi"
