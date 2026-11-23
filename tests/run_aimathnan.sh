#!/bin/sh
# What the clamping AI maths functions do with a non-number.
#
#   ./tests/run_aimathnan.sh
#
# A direct check rather than a differential one: ai_mathfunctions.c has
# no frozen copy to compare against, and the behaviour being pinned is
# not visible in ordinary arithmetic at all.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Werror=implicit-function-declaration -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

$CC $CFLAGS -c tests/test_aimathnan.c -o "$OUT/aimathnan.drv.o"
$CC $CFLAGS -c ai/ai_mathfunctions.c -o "$OUT/aimathnan.sut.o"
$CC $CFLAGS -c math.c -o "$OUT/aimathnan.math.o"
$CC $CFLAGS -c tests/math_mocks.c -o "$OUT/aimathnan.mocks.o"
$CC "$OUT/aimathnan.drv.o" "$OUT/aimathnan.sut.o" "$OUT/aimathnan.math.o" \
    "$OUT/aimathnan.mocks.o" -o "$OUT/aimathnan" -lm
"$OUT/aimathnan"
