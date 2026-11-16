#!/bin/sh
# The unit test for the OpenGL renderer's 2D pass.
#
#   ./tests/run_wingloverlay.sh
#
# Every GL call in win/wingl.c goes through a function pointer, so this
# drives the whole pass with no driver anywhere: the pointers are aimed
# at mocks that record what was asked for, and a case can then check
# which program was used, what was uploaded, how the vertex layouts
# were described, and what state the pass put back.
#
# The shaders are the real ones from data/shaders, read through
# datapath.c with its idea of the executable pointed at the repository
# root -- so a uniform renamed in a shader file and not in wingl.c is a
# failing test rather than a black overlay on somebody's machine.
#
# win/wingl.c is included by the driver rather than linked, because the
# pass's state is static to it.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

echo "=== GL overlay pass ==="
$CC $CFLAGS -fsanitize=address -c tests/test_wingloverlay.c -o "$OUT/wgo.drv.o"
$CC $CFLAGS -fsanitize=address -c win/glload.c -o "$OUT/wgo.load.o"
$CC $CFLAGS -fsanitize=address -c win/glshader.c -o "$OUT/wgo.shader.o"
$CC $CFLAGS -fsanitize=address -c datapath.c -o "$OUT/wgo.datapath.o"
# The real camera fitting, vertex building and matrix helpers, which
# are arithmetic with nothing to stand in for. math.c comes with them
# for MATH_MakeOGLMatrix, and gdi.c holds the two display settings the
# projection depends on -- but gdi.c would pull in the whole engine,
# so those two are defined by the driver instead.
$CC $CFLAGS -fsanitize=address -c projection.c -o "$OUT/wgo.proj.o"
$CC $CFLAGS -fsanitize=address -c geovertex.c -o "$OUT/wgo.gvx.o"
# rasterstate.c reads the material flags the renderer acts on. Pure,
# so linked rather than mocked; tests/run_wingdi.sh is where that
# reading is checked against the other renderer.
$CC $CFLAGS -fsanitize=address -c rasterstate.c -o "$OUT/wgo.rst.o"
$CC $CFLAGS -fsanitize=address -c depthblur.c -o "$OUT/wgo.dpb.o"
$CC $CFLAGS -fsanitize=address -c fog.c -o "$OUT/wgo.fog.o"
$CC $CFLAGS -fsanitize=address -c cinebands.c -o "$OUT/wgo.cbd.o"
$CC $CFLAGS -fsanitize=address -c aecolor.c -o "$OUT/wgo.aec.o"
$CC $CFLAGS -fsanitize=address -c aeframe.c -o "$OUT/wgo.aef.o"
# watermesh.c builds the water surface for both renderers. Pure, so
# linked rather than mocked; tests/run_wingdi.sh is where it is
# compared against the other renderer byte for byte.
$CC $CFLAGS -fsanitize=address -c watermesh.c -o "$OUT/wgo.wms.o"
# waterreflect.c is the reflection pass's own share: the mirrored
# transform, the size threshold and the clip plane. Also a move out
# of win/wingdi.c, so the frozen copy still has all three inline.
$CC $CFLAGS -fsanitize=address -c waterreflect.c -o "$OUT/wgo.wrf.o"
# watersmooth.c holds the surface smoothing's rectangles, steps and
# taps for both renderers; tests/run_wingdi_ae.sh is where it is
# compared against the other renderer.
$CC $CFLAGS -fsanitize=address -c watersmooth.c -o "$OUT/wgo.wsm.o"
# aefade.c holds the fade's timer -- the step, the truncation and the
# finish -- for both renderers; tests/run_wingdi_ae.sh is where it is
# compared against the other renderer.
$CC $CFLAGS -fsanitize=address -c aefade.c -o "$OUT/wgo.afd.o"
# aehue.c holds the hue sweep's wave and the packing of its three
# channels into one colour; tests/run_wingdi_ae.sh is where it is
# compared against the other renderer.
$CC $CFLAGS -fsanitize=address -c aehue.c -o "$OUT/wgo.ahu.o"
$CC $CFLAGS -fsanitize=address -c geosprite.c -o "$OUT/wgo.gsp.o"
$CC $CFLAGS -fsanitize=address -c win/wingdimath.c -o "$OUT/wgo.wgmath.o"
$CC $CFLAGS -fsanitize=address -c math.c -o "$OUT/wgo.math.o"
$CC -fsanitize=address "$OUT/wgo.drv.o" "$OUT/wgo.load.o" "$OUT/wgo.shader.o" \
    "$OUT/wgo.datapath.o" "$OUT/wgo.proj.o" "$OUT/wgo.gvx.o" \
    "$OUT/wgo.rst.o" "$OUT/wgo.dpb.o" "$OUT/wgo.fog.o" "$OUT/wgo.cbd.o" "$OUT/wgo.aec.o" "$OUT/wgo.aef.o" "$OUT/wgo.wms.o" "$OUT/wgo.wrf.o" "$OUT/wgo.wsm.o" "$OUT/wgo.afd.o" "$OUT/wgo.ahu.o" "$OUT/wgo.gsp.o" \
    "$OUT/wgo.wgmath.o" "$OUT/wgo.math.o" -o "$OUT/wgo" -lm
"$OUT/wgo"
