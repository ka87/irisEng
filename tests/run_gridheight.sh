#!/bin/sh
# Which of grid.c's functions decide a position's height.
#
#   ./tests/run_gridheight.sh
#
# grid.c had no test. The rewrite from decompiler output into source
# factored three cell-to-world conversions through one helper that set
# z to nought, which is right for the two that write path points and
# wrong for the one that adjusts a destination it was handed -- and
# that is what left the companions standing still.
#
# A differential against the copy the rewrite came from is not
# available: that copy reads locals the decompiler only assigns later
# in the file, so it faults outside the original binary's register
# state. This pins the property instead.
#
# grid.c is linked on its own with the twelve things it reaches for
# defined by the driver, so the test needs no other engine module.
set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

# The 32-bit stand-in for an address: the engine files below
# register and release through it, and it has no engine
# dependencies of its own.
$CC $CFLAGS -c handle.c -o "$OUT/gridheight.handle.o"

echo "=== grid heights ==="
$CC $CFLAGS -fsanitize=address -c grid.c -o "$OUT/ghe.grid.o"
$CC $CFLAGS -fsanitize=address -c tests/test_gridheight.c -o "$OUT/ghe.drv.o"
$CC -fsanitize=address "$OUT/ghe.grid.o" "$OUT/ghe.drv.o" -o "$OUT/ghe" "$OUT/gridheight.handle.o" -lm
"$OUT/ghe"
