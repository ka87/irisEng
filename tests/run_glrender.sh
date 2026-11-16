#!/bin/sh
# What the shipped shaders actually compute.
#
#   ./tests/run_glrender.sh
#
# Unlike every other test of the OpenGL renderer, this one does not
# mock OpenGL. It brings up a real 4.5 core context with no window --
# Mesa's software renderer answers for it -- aims win/glcore.h's
# function pointers at that, draws through win/wingl.c's own entry
# points and reads the pixels back. So it covers the renderer and the
# shaders together: a mock harness can say which call was made, and
# only this can say what came out.
#
# It is allowed to be unavailable. A machine with no libEGL, no
# software renderer or no 4.5 core profile prints why and passes --
# which is the whole point of keeping it separate from
# tests/run_wingloverlay.sh, whose cases run anywhere.
#
# No AddressSanitizer here: it would be instrumenting the graphics
# driver rather than the code under test, and llvmpipe allocates and
# frees in ways that are its business.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

$CC $CFLAGS -c tests/test_glrender.c -o "$OUT/glr.drv.o"
$CC $CFLAGS -c tests/eglheadless.c -o "$OUT/glr.egl.o"
$CC $CFLAGS -c win/glload.c -o "$OUT/glr.load.o"
$CC $CFLAGS -c win/glshader.c -o "$OUT/glr.shader.o"
$CC $CFLAGS -c datapath.c -o "$OUT/glr.datapath.o"
# The same real arithmetic the other GL harness links: the camera
# fitting, the vertex building, the flags decode and the matrix
# helpers, with math.c for MATH_MakeOGLMatrix. gdi.c would pull the
# whole engine in, so the two display settings it holds are defined by
# the driver.
$CC $CFLAGS -c projection.c -o "$OUT/glr.proj.o"
$CC $CFLAGS -c geovertex.c -o "$OUT/glr.gvx.o"
$CC $CFLAGS -c rasterstate.c -o "$OUT/glr.rst.o"
$CC $CFLAGS -c depthblur.c -o "$OUT/glr.dpb.o"
$CC $CFLAGS -c fog.c -o "$OUT/glr.fog.o"
$CC $CFLAGS -c cinebands.c -o "$OUT/glr.cbd.o"
$CC $CFLAGS -c aecolor.c -o "$OUT/glr.aec.o"
$CC $CFLAGS -c aeframe.c -o "$OUT/glr.aef.o"
# watermesh.c builds the water surface for both renderers. Pure, so
# linked rather than mocked; tests/run_wingdi.sh is where it is
# compared against the other renderer byte for byte.
$CC $CFLAGS -c watermesh.c -o "$OUT/glr.wms.o"
# waterreflect.c is the reflection pass's own share: the mirrored
# transform, the size threshold and the clip plane. Also a move out
# of win/wingdi.c, so the frozen copy still has all three inline.
$CC $CFLAGS -c waterreflect.c -o "$OUT/glr.wrf.o"
# watersmooth.c holds the surface smoothing's rectangles, steps and
# taps for both renderers; tests/run_wingdi_ae.sh is where it is
# compared against the other renderer.
$CC $CFLAGS -c watersmooth.c -o "$OUT/glr.wsm.o"
# aefade.c holds the fade's timer -- the step, the truncation and the
# finish -- for both renderers; tests/run_wingdi_ae.sh is where it is
# compared against the other renderer.
$CC $CFLAGS -c aefade.c -o "$OUT/glr.afd.o"
# aehue.c holds the hue sweep's wave and the packing of its three
# channels into one colour; tests/run_wingdi_ae.sh is where it is
# compared against the other renderer.
$CC $CFLAGS -c aehue.c -o "$OUT/glr.ahu.o"
# aemotion.c holds the motion smooth's camera-movement test and its
# four taps; tests/run_wingdi_ae.sh is where both are compared against
# the other renderer.
$CC $CFLAGS -c aemotion.c -o "$OUT/glr.amo.o"
$CC $CFLAGS -c geosprite.c -o "$OUT/glr.gsp.o"
$CC $CFLAGS -c win/wingdimath.c -o "$OUT/glr.wgmath.o"
$CC $CFLAGS -c math.c -o "$OUT/glr.math.o"
# -ldl for the EGL lookup: the library is opened by name because there
# are no EGL headers to link against.
$CC "$OUT/glr.drv.o" "$OUT/glr.egl.o" "$OUT/glr.load.o" \
    "$OUT/glr.shader.o" "$OUT/glr.datapath.o" "$OUT/glr.proj.o" \
    "$OUT/glr.gvx.o" "$OUT/glr.rst.o" "$OUT/glr.dpb.o" "$OUT/glr.fog.o" "$OUT/glr.cbd.o" "$OUT/glr.aec.o" "$OUT/glr.aef.o" "$OUT/glr.wms.o" "$OUT/glr.wrf.o" "$OUT/glr.wsm.o" "$OUT/glr.afd.o" "$OUT/glr.ahu.o" "$OUT/glr.amo.o" "$OUT/glr.gsp.o" \
    "$OUT/glr.wgmath.o" \
    "$OUT/glr.math.o" -o "$OUT/glr" -lm -ldl
# Software rendering, so the answer does not depend on whose card the
# machine has.
LIBGL_ALWAYS_SOFTWARE=1 "$OUT/glr"
