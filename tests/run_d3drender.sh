#!/bin/sh
# The differential test for platform/d3drender.c.
#
#   ./tests/run_d3drender.sh
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
# The renderer asks the path module where a screenshot goes and
# where the settings application is. Only SDL's headers are wanted:
# the entry points it uses are defined by the mocks
# (tests/sdlpath.h), so a case can point the install anywhere.
SDL3_INC="$PWD/thirdparty/SDL3/include"
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -I"$SDL3_INC" -Itests/stub -Itests"
# both sides see the real SDK's window-message numbers: the stub's are
# sequential, which inverts their order and makes most of the two
# message handlers unreachable.  See tests/d3drender_msg_sdk.h.
CFLAGS="$CFLAGS -include tests/d3drender_msg_sdk.h"
OUT=tests/build
mkdir -p "$OUT"

# zlib and libpng, which the screenshot writer is built on. Built
# once into an archive rather than per harness.
./tests/build_png.sh

syms=$(python3 - <<'EOF'
import re
src = open('platform/d3drender.c', encoding='latin-1').read()
names = []
for m in re.finditer(r'^[A-Za-z_][\w \*]*?[\w\*]\s+\**(\w+)\s*\(', src, re.M):
    names.append(m.group(1))
# The three UV projections came out of platform/d3drender.c into
# materialuv.c, which is compiled below as a second translation unit of
# this side. They are named here because the frozen copy has two of
# them by name -- unlike projection.c and geovertex.c, whose contents
# it only has inline -- so both sides need the rename to keep them
# apart at the link.
names += ['MAT_UV_ComputeAndSetTransform', 'MAT_UV_Compute_Chrome',
          'MAT_UV_Compute_PLANAR_GIZMO_NOSTORE']
# PC_SetUVMatrix went the same way and for the same reason: it is the
# matrix those three end up writing, there is nothing of a renderer in
# it, and its callers are all in materialuv.c. The frozen copy still
# has it, so both sides need the rename.
names += ['PC_SetUVMatrix']
names += ['d3dProjectionMtx', 'd3dTransformMatrix', 'd3dTransformMatrix2',
          'engineFogTableModeToD3D', 'worldPtrIfMultiStream',
          'D3D_gi_FlatLayerBox', 'D3D_gi_AspectTrace',
          'D3D_gf_CutsceneBand']
seen = []
for n in names:
    if n not in seen and n not in ('if', 'for', 'while', 'switch', 'return', 'sizeof'):
        seen.append(n)
print(' '.join(seen))
EOF
)

# What only the frozen copy has by name.
#
# rasterNames is the engine's own list of what it measures, and it had
# been left in the renderer's file long after its declaration moved to
# profiler.h. It is in profiler.c now, which this harness already
# links, so the rewrite uses that one and only the frozen copy's
# definition needs a name of its own -- the same arrangement as the UV
# projections above, from the other end.
# PC_EngineCheatFinal is the engine's debug keys and is in
# debugkeys.c now, which this harness does not build. The frozen copy
# still has it and nothing compares it, so only that copy's definition
# needs a name of its own -- without which it would collide the day
# anything here does link the new file.
# The flat layer's box went the same way as rasterNames: its four
# answers and the flag that turns them off are arithmetic over the
# engine's own numbers, they name no renderer, and three other files
# were asking this one for them. They are in flatlayer.c now, which
# this harness links -- so the rewrite's side of the comparison is
# that module and only the frozen copy's definitions need names of
# their own.
baseonly="rasterNames PC_EngineCheatFinal \
D3D_FlatLayerWidth D3D_FlatLayerOffsetX \
D3D_FlatLayerToLayoutX D3D_FlatLayerFromLayoutX D3D_gi_FlatLayerBox"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

echo "=== d3drender ==="
$CC $CFLAGS -c platform/d3drender.c -o "$OUT/d3drender.new.o" $(defs NEW_ $syms)
# The material's texture-coordinate projections, which used to be in
# platform/d3drender.c and are shared now: the three of them that told the
# device as well as the engine. Compiled as a second translation unit
# of this side rather than linked plain, because the frozen copy has
# two of them by name and the two must not collide.
$CC $CFLAGS -c materialuv.c -o "$OUT/d3drender.uv.o" $(defs NEW_ $syms)
# the frozen copy still spells one function the way the decompiler named
# it; map it onto the same per-side symbol the rewrite's new name takes
# Only the frozen copy sees a D3DX header: the engine stopped
# including one when the four matrix calls and the two texture calls
# were reimplemented, and the baseline still refers to all of them.
# Forcing it in here rather than for both sides is what keeps that
# independence honest -- if the live file still needed D3DX, it would
# not compile.
$CC $CFLAGS -include tests/stub/d3dx9math.h \
    -include tests/d3drender_baseline_compat.h \
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
    -DpcINO_dinput_mouse_and_keyb_acquire=pcINO_mouse_and_keyb_acquire \
    -DpcINO_dinput_mouse_and_keyb_unacquire=pcINO_mouse_and_keyb_unacquire \
    -c tests/baseline_d3drender.c -o "$OUT/d3drender.base.o" \
    $(defs BASE_ $syms) $(defs BASE_ $baseonly)
$CC $CFLAGS -c math.c -o "$OUT/d3drender.math.o"
# The real matrix module, linked rather than mocked: it is pure
# arithmetic with nothing to stand in for. platform/d3dtex.c is not
# linked -- see the stand-ins in tests/d3drender_mocks.c for why.
$CC $CFLAGS -c platform/d3dmath.c -o "$OUT/d3drender.wgmath.o"
# The camera fitting and the projections came out of platform/d3drender.c into
# projection.c, which is arithmetic with nothing to stand in for, so it
# is linked. The frozen copy still has its own inline copy, which is
# what makes this comparison worth running.
$CC $CFLAGS -c projection.c -o "$OUT/d3drender.proj.o"
# The vertex building came out of platform/d3drender.c into geovertex.c, and is
# linked for the same reason: array walking and colour packing with
# nothing to stand in for. The frozen copy still has its own inline
# copy of both.
$CC $CFLAGS -c geovertex.c -o "$OUT/d3drender.gvx.o"
# The sprite quad building came out of platform/d3drender.c into geosprite.c,
# and is linked for the same reason: walking the engine's arrays with
# nothing to stand in for. The frozen copy still has its own inline
# copy.
$CC $CFLAGS -c geosprite.c -o "$OUT/d3drender.gsp.o"
# rasterstate.c states the per-draw flags word's meaning once, for
# both renderers. Unlike projection.c and geovertex.c it is not a move:
# this renderer still reads the bits itself, and the driver's
# rasterStateCase compares the two readings over every combination of
# them. Linked for that check, which is why it is pure.
$CC $CFLAGS -c rasterstate.c -o "$OUT/d3drender.rst.o"
# watermesh.c states the water surface's index strip, vertex stream and
# reflection remap once, for both renderers. It is a move out of
# platform/d3drender.c, so the frozen copy still has all three inline and the
# water cases below compare the two -- including the bytes each side
# writes into the locked index and vertex buffers, which is what proves
# the shared copy rather than merely exercising it.
$CC $CFLAGS -c watermesh.c -o "$OUT/d3drender.wms.o"
# perfbars.c holds what `tweak perf`'s display decides -- the frame
# rate, the smoothing, the order, which rows are shown, a row's colour
# and a bar's segments -- for both renderers. A move out of
# platform/d3drender.c, so the frozen copy still has all of it inline and the
# perf cases below compare the two: the quads this renderer draws and
# the text it writes are what says the shared copy reproduces it.
$CC $CFLAGS -c perfbars.c -o "$OUT/d3drender.pfb.o"
# pngsave.c is the screenshot's file, written with the vendored libpng
# and shared by both renderers.
#
# Linked here because this renderer's screenshot calls it -- and for
# nothing more than that. An earlier version of this comment claimed
# the cases below compared the two renderers' files byte for byte.
# They do not and never did: the only screenshot case here runs with
# no device, so neither writer is reached. What covers the writer is
# tests/run_pngsave.sh, which reads a file back with libpng's own
# reader.
$CC $CFLAGS -Ithirdparty/zlib -Ithirdparty/libpng -c pngsave.c -o "$OUT/d3drender.pngsave.o"
# waterreflect.c is the reflection pass's own share: the mirrored
# transform, the size threshold and the clip plane. Also a move out
# of platform/d3drender.c, so the frozen copy still has all three inline.
$CC $CFLAGS -c waterreflect.c -o "$OUT/d3drender.wrf.o"
# watersmooth.c holds the surface smoothing's rectangles, steps and
# taps for both renderers; tests/run_d3daftereffects.sh is where it is
# compared against the other renderer.
$CC $CFLAGS -c watersmooth.c -o "$OUT/d3drender.wsm.o"
# depthblur.c comes with it: the smoothing's weights are the same
# chain the depth blur's taps are weighed by, so it asks that file
# for them rather than carrying a second copy.
$CC $CFLAGS -c depthblur.c -o "$OUT/d3drender.dpb.o"
$CC $CFLAGS -c fog.c -o "$OUT/d3drender.fog.o"
$CC $CFLAGS -c profiler.c -o "$OUT/d3drender.profiler.o"
$CC $CFLAGS -c tests/d3drender_mocks.c -o "$OUT/d3drender.mocks.o"
$CC $CFLAGS -c datapath.c -o "$OUT/d3drender.datapath.o"
$CC $CFLAGS -c flatlayer.c -o "$OUT/d3drender.flatlayer.o"
$CC $CFLAGS -c tests/test_d3drender.c -o "$OUT/d3drender.drv.o"
$CC "$OUT/d3drender.new.o" "$OUT/d3drender.uv.o" "$OUT/d3drender.base.o" "$OUT/d3drender.math.o" \
    "$OUT/d3drender.wgmath.o" "$OUT/d3drender.proj.o" "$OUT/d3drender.gvx.o" \
    "$OUT/d3drender.rst.o" "$OUT/d3drender.fog.o" "$OUT/d3drender.gsp.o" "$OUT/d3drender.profiler.o" \
    "$OUT/d3drender.wms.o" "$OUT/d3drender.pfb.o" "$OUT/d3drender.pngsave.o" "$OUT/libpng.a" \
    "$OUT/d3drender.wrf.o" "$OUT/d3drender.wsm.o" "$OUT/d3drender.dpb.o" \
    "$OUT/d3drender.mocks.o" "$OUT/d3drender.datapath.o" \
    "$OUT/d3drender.flatlayer.o" \
    "$OUT/d3drender.drv.o" \
    -o "$OUT/d3drender" -lm
"$OUT/d3drender"
