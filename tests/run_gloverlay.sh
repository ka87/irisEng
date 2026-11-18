#!/bin/sh
# The unit test for the OpenGL renderer's 2D pass.
#
#   ./tests/run_gloverlay.sh
#
# Every GL call in platform/glrender.c goes through a function pointer, so this
# drives the whole pass with no driver anywhere: the pointers are aimed
# at mocks that record what was asked for, and a case can then check
# which program was used, what was uploaded, how the vertex layouts
# were described, and what state the pass put back.
#
# The shaders are the real ones from data/shaders, read through
# datapath.c with its idea of the executable pointed at the repository
# root -- so a uniform renamed in a shader file and not in glrender.c is a
# failing test rather than a black overlay on somebody's machine.
#
# platform/glrender.c is included by the driver rather than linked, because the
# pass's state is static to it.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

echo "=== GL overlay pass ==="
$CC $CFLAGS -fsanitize=address -c tests/test_gloverlay.c -o "$OUT/wgo.drv.o"
$CC $CFLAGS -fsanitize=address -c platform/glload.c -o "$OUT/wgo.load.o"
$CC $CFLAGS -fsanitize=address -c platform/glshader.c -o "$OUT/wgo.shader.o"
$CC $CFLAGS -fsanitize=address -c datapath.c -o "$OUT/wgo.datapath.o"
# The real camera fitting, vertex building and matrix helpers, which
# are arithmetic with nothing to stand in for. math.c comes with them
# for MATH_MakeOGLMatrix, and gdi.c holds the two display settings the
# projection depends on -- but gdi.c would pull in the whole engine,
# so those two are defined by the driver instead.
$CC $CFLAGS -fsanitize=address -c projection.c -o "$OUT/wgo.proj.o"
$CC $CFLAGS -fsanitize=address -c geovertex.c -o "$OUT/wgo.gvx.o"
# rasterstate.c reads the material flags the renderer acts on. Pure,
# so linked rather than mocked; tests/run_d3drender.sh is where that
# reading is checked against the other renderer.
$CC $CFLAGS -fsanitize=address -c rasterstate.c -o "$OUT/wgo.rst.o"
$CC $CFLAGS -fsanitize=address -c depthblur.c -o "$OUT/wgo.dpb.o"
$CC $CFLAGS -fsanitize=address -c fog.c -o "$OUT/wgo.fog.o"
$CC $CFLAGS -fsanitize=address -c cinebands.c -o "$OUT/wgo.cbd.o"
$CC $CFLAGS -fsanitize=address -c aftereffects/aecolor.c -o "$OUT/wgo.aec.o"
$CC $CFLAGS -fsanitize=address -c aftereffects/aeframe.c -o "$OUT/wgo.aef.o"
# watermesh.c builds the water surface for both renderers. Pure, so
# linked rather than mocked; tests/run_d3drender.sh is where it is
# compared against the other renderer byte for byte.
$CC $CFLAGS -fsanitize=address -c watermesh.c -o "$OUT/wgo.wms.o"
# waterreflect.c is the reflection pass's own share: the mirrored
# transform, the size threshold and the clip plane. Also a move out
# of platform/d3drender.c, so the frozen copy still has all three inline.
$CC $CFLAGS -fsanitize=address -c waterreflect.c -o "$OUT/wgo.wrf.o"
# watersmooth.c holds the surface smoothing's rectangles, steps and
# taps for both renderers; tests/run_d3daftereffects.sh is where it is
# compared against the other renderer.
$CC $CFLAGS -fsanitize=address -c watersmooth.c -o "$OUT/wgo.wsm.o"
# aftereffects/aefade.c holds the fade's timer -- the step, the truncation and the
# finish -- for both renderers; tests/run_d3daftereffects.sh is where it is
# compared against the other renderer.
$CC $CFLAGS -fsanitize=address -c aftereffects/aefade.c -o "$OUT/wgo.afd.o"
# aftereffects/aehue.c holds the hue sweep's wave and the packing of its three
# channels into one colour; tests/run_d3daftereffects.sh is where it is
# compared against the other renderer.
$CC $CFLAGS -fsanitize=address -c aftereffects/aehue.c -o "$OUT/wgo.ahu.o"
# aftereffects/aemotion.c holds the motion smooth's camera-movement test and its
# four taps; tests/run_d3daftereffects.sh is where both are compared against
# the other renderer.
$CC $CFLAGS -fsanitize=address -c aftereffects/aemotion.c -o "$OUT/wgo.amo.o"
# aftereffects/aezoom2.c holds the second zoom smooth: the stage split, the walk of
# eight angles it decides, the weight, and the per-sample transform --
# which is written as a rotation and is not one. Both renderers take
# all of that from here.
$CC $CFLAGS -fsanitize=address -c aftereffects/aezoom2.c -o "$OUT/wgo.az2.o"
$CC $CFLAGS -fsanitize=address -c geosprite.c -o "$OUT/wgo.gsp.o"
$CC $CFLAGS -fsanitize=address -c platform/d3dmath.c -o "$OUT/wgo.wgmath.o"
$CC $CFLAGS -fsanitize=address -c math.c -o "$OUT/wgo.math.o"
$CC -fsanitize=address "$OUT/wgo.drv.o" "$OUT/wgo.load.o" "$OUT/wgo.shader.o" \
    "$OUT/wgo.datapath.o" "$OUT/wgo.proj.o" "$OUT/wgo.gvx.o" \
    "$OUT/wgo.rst.o" "$OUT/wgo.dpb.o" "$OUT/wgo.fog.o" "$OUT/wgo.cbd.o" "$OUT/wgo.aec.o" "$OUT/wgo.aef.o" "$OUT/wgo.wms.o" "$OUT/wgo.wrf.o" "$OUT/wgo.wsm.o" "$OUT/wgo.afd.o" "$OUT/wgo.ahu.o" "$OUT/wgo.amo.o" "$OUT/wgo.az2.o" "$OUT/wgo.gsp.o" \
    "$OUT/wgo.wgmath.o" "$OUT/wgo.math.o" -o "$OUT/wgo" -lm
"$OUT/wgo"
