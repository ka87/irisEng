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
$CC $CFLAGS -fsanitize=address -c win/wingdimath.c -o "$OUT/wgo.wgmath.o"
$CC $CFLAGS -fsanitize=address -c math.c -o "$OUT/wgo.math.o"
$CC -fsanitize=address "$OUT/wgo.drv.o" "$OUT/wgo.load.o" "$OUT/wgo.shader.o" \
    "$OUT/wgo.datapath.o" "$OUT/wgo.proj.o" "$OUT/wgo.gvx.o" \
    "$OUT/wgo.wgmath.o" "$OUT/wgo.math.o" -o "$OUT/wgo" -lm
"$OUT/wgo"
