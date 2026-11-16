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
# The log goes to debug.log in the working directory, so the test runs
# in a directory of its own under tests/build.
set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
# The engine's headers declare structs inside parameter lists, so the
# build is noisy by nature; the errors are what matter here.
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -Wall"
OUT=tests/build
mkdir -p "$OUT/dbglog.run"

echo "=== the log's stamp and banner ==="
$CC $CFLAGS -fsanitize=address,undefined -Itests/stub \
    -c tests/test_dbglog.c -o "$OUT/dbglog.o"
$CC -fsanitize=address,undefined "$OUT/dbglog.o" -lm -o "$OUT/dbglog"
DBGLOG_DIR="$PWD/$OUT/dbglog.run" "$OUT/dbglog"
