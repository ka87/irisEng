#!/bin/sh
# What the clock counts a frame as, against how long it really took.
#
#   ./tests/run_timepace.sh
#
# time.c had no harness at all before this one. It wants very little:
# the clock reaches outside itself only for the millisecond timer it
# measures the processor against and for the sleep the pacing uses, and
# the four of those are defined by the driver.
#
# The cases are real time, not a driven clock -- the thing being
# checked is a relation between the world's clock and the wall's, and a
# clock the test had injected could not tell them apart. So this one
# takes about a second and a half where the others take none, most of
# it the quarter second the clock spends measuring the processor at
# startup.
#
# Nothing else is linked: the clock's only use of the maths module is
# f2iMagic, which is inline, and the drift measurement that uses it is
# not on the path any of this exercises.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

echo "=== frame pacing ==="
$CC $CFLAGS -c time.c -o "$OUT/tpc.time.o"
$CC $CFLAGS -c tests/test_timepace.c -o "$OUT/tpc.drv.o"
$CC "$OUT/tpc.time.o" "$OUT/tpc.drv.o" -o "$OUT/tpc" -lm
"$OUT/tpc"
