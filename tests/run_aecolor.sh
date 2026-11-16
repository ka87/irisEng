#!/bin/sh
# The unit test for aecolor.c.
#
#   ./tests/run_aecolor.sh
#
# The three after-effects that are a colour transform and nothing
# else: brightness, contrast and grayscale. Each comes out of the
# other renderer's texture stages as a gain, a bias and -- for the
# grayscale -- three weights, so the numbers are derived rather than
# chosen, and a derived number is worth deriving the other way round.
#
# That is what this does. The grayscale is checked against the
# two-stage form it was collapsed from; the contrast against the one
# formula both of its halves turn out to be; the brightness at the
# places its two halves meet. It also pins the clamp between the
# steps, which is what stops the three being composed into one
# unclamped expression -- they would come out brighter at the ends.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -O0 -g -Wall -Wextra"
OUT=tests/build
mkdir -p "$OUT"

echo "=== after-effect colour ==="
$CC $CFLAGS -fsanitize=address,undefined -c tests/test_aecolor.c \
    -o "$OUT/aec.drv.o"
$CC $CFLAGS -fsanitize=address,undefined -c aecolor.c -o "$OUT/aec.o"
$CC -fsanitize=address,undefined "$OUT/aec.drv.o" "$OUT/aec.o" -lm \
    -o "$OUT/aec"
"$OUT/aec"
