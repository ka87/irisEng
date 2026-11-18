#!/bin/sh
# The unit test for platform/glshader.c.
#
#   ./tests/run_glshader.sh
#
# New code with no earlier copy, so a direct check.
#
# Every GL call in the shader loader goes through a function pointer in
# glcore.h, and that is what makes this test possible with no driver
# anywhere: the pointers are aimed at mocks, so a case can say "this
# stage will not compile" or "the link fails" and then check what the
# loader cleaned up -- which is the part a real machine hides until
# something runs out of objects.
#
# The shader files are real, written into tests/build and read back
# through datapath.c, whose idea of where the executable is gets
# pointed at that directory.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
# datapath.c asks SDL where the executable is. Only the headers
# are wanted here: the entry points it uses are defined by the
# harness (tests/sdlpath.h), so a case can point the install at a
# fixture directory and there is no library to link.
SDL3_INC="$PWD/thirdparty/SDL3/include"
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -I"$SDL3_INC" -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

echo "=== GL shaders ==="
$CC $CFLAGS -fsanitize=address -c tests/test_glshader.c -o "$OUT/glsh.drv.o"
$CC $CFLAGS -fsanitize=address -c platform/glshader.c -o "$OUT/glsh.o"
$CC $CFLAGS -fsanitize=address -c platform/glload.c -o "$OUT/glsh.load.o"
$CC $CFLAGS -fsanitize=address -c datapath.c -o "$OUT/glsh.datapath.o"
$CC -fsanitize=address "$OUT/glsh.drv.o" "$OUT/glsh.o" "$OUT/glsh.load.o" \
    "$OUT/glsh.datapath.o" -o "$OUT/glsh"
"$OUT/glsh"
