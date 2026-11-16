#!/bin/sh
# The unit test for aeframe.c.
#
#   ./tests/run_aeframe.sh
#
# The three after-effects that build a frame out of several samples of
# one: the motion blur, the remanence and the zoom smooth. Each comes
# down to a schedule and a set of weights, and neither shows in the
# picture -- a wrong weight is a slightly different smear and a wrong
# schedule is a smear of the wrong thing.
#
# The schedules are held to the same three properties as the depth
# blur's: no pass reads what it writes, each reads what the one before
# wrote, and the result is where the sum expects it. A wrong ping-pong
# produces a blurred picture either way.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -O0 -g -Wall -Wextra"
OUT=tests/build
mkdir -p "$OUT"

echo "=== after-effect frame sums ==="
$CC $CFLAGS -fsanitize=address,undefined -c tests/test_aeframe.c \
    -o "$OUT/aef.drv.o"
$CC $CFLAGS -fsanitize=address,undefined -c aeframe.c -o "$OUT/aef.o"
$CC -fsanitize=address,undefined "$OUT/aef.drv.o" "$OUT/aef.o" -lm \
    -o "$OUT/aec"
"$OUT/aec"
