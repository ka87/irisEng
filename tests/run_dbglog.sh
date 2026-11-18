#!/bin/sh
# The unit test for the log's timestamp, banner and closing line.
#
#   ./tests/run_dbglog.sh
#
# dbg.c is compiled by including it, the way the renderer harnesses
# include their renderer: the engine has headers of its own called
# time.h, math.h, string.h and memory.h, so putting the project's
# directory on the include path makes the C library's headers resolve
# to the engine's. Including the source relatively keeps the path out
# of it, which is also what the real build does.
#
# The log goes into the player's own directory, which the harness
# points at one of its own under tests/build -- through the same path
# module the engine uses, answered by tests/sdlpath.h.
set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
# The engine's headers declare structs inside parameter lists, so the
# build is noisy by nature; the errors are what matter here.
# Only SDL's headers are wanted: the entry points the path module
# uses are defined by the harness, so there is no library to link.
SDL3_INC="$PWD/thirdparty/SDL3/include"
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -Wall"
OUT=tests/build
mkdir -p "$OUT/dbglog.run"

echo "=== the log's stamp and banner ==="
$CC $CFLAGS -fsanitize=address,undefined -I"$SDL3_INC" -Itests/stub \
    -Itests -c tests/test_dbglog.c -o "$OUT/dbglog.o"
$CC $CFLAGS -fsanitize=address,undefined -I"$SDL3_INC" \
    -c datapath.c -o "$OUT/dbglog.datapath.o"
$CC -fsanitize=address,undefined "$OUT/dbglog.o" "$OUT/dbglog.datapath.o" \
    -lm -o "$OUT/dbglog"
DBGLOG_DIR="$PWD/$OUT/dbglog.run" "$OUT/dbglog"
