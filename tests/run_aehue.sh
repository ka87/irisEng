#!/bin/sh
# The unit test for aftereffects/aehue.c.
#
#   ./tests/run_aehue.sh
#
# The hue sweep: one wave, three phases a third of a cycle apart, and
# three bytes packed into a colour. None of it shows in a picture -- a
# wrong wave is a tint of a slightly different colour and a wrong phase
# offset is a tint of the wrong one, both as convincing as the right
# answer -- so the values are pinned here, taken from the other
# renderer's own expressions.
#
# The case that earns the file is the overflow one. Above a depth of
# one the channels pass their bytes and do not all overflow the same
# way: red's extra is swallowed and green's and blue's carry into the
# channel above. That is a property of the packing rather than of the
# arithmetic, so a renderer computing three clamped channels would pass
# everything else and draw the wrong picture.
#
# Built with the undefined-behaviour sanitizer, because turning the
# wave into a whole number is a float-to-integer conversion and the
# wave can come out a hair above one.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -O0 -g -Wall -Wextra"
OUT=tests/build
mkdir -p "$OUT"

echo "=== the hue sweep ==="
$CC $CFLAGS -fsanitize=address,undefined -c tests/test_aehue.c \
    -o "$OUT/ahu.drv.o"
$CC $CFLAGS -fsanitize=address,undefined -c aftereffects/aehue.c -o "$OUT/ahu.o"
$CC -fsanitize=address,undefined "$OUT/ahu.drv.o" "$OUT/ahu.o" -lm \
    -o "$OUT/ahu"
"$OUT/ahu"
