#!/bin/sh
# Degenerate vectors through the normalizing math in math.c.
#
#   ./tests/run_degenvec.sh
#
# A direct check rather than a differential one. The math differential
# compares math.c against a frozen copy that predates the zero-length
# guards and never draws a zero-length vector, so it cannot see them
# either way; this asserts the behaviour outright, in both directions.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Werror=implicit-function-declaration -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

$CC $CFLAGS -c tests/test_degenvec.c -o "$OUT/degenvec.drv.o"
$CC $CFLAGS -c math.c -o "$OUT/degenvec.math.o"
$CC $CFLAGS -c tests/math_mocks.c -o "$OUT/degenvec.mocks.o"
$CC "$OUT/degenvec.drv.o" "$OUT/degenvec.math.o" "$OUT/degenvec.mocks.o" \
    -o "$OUT/degenvec" -lm
"$OUT/degenvec"
