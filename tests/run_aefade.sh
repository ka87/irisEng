#!/bin/sh
# The unit test for aftereffects/aefade.c.
#
#   ./tests/run_aefade.sh
#
# The fade's timer: the step, the truncation that makes a duration come
# to something other than what it asked for, and the finish that clears
# the effect's own bit.
#
# Built with the undefined-behaviour sanitizer on, and that is the
# point of having this as well as the differential. The step is turned
# into a whole number by a conversion that the other renderer leaves
# out of range in two cases, and the answer a real machine gives there
# decides whether a fade finishes at once or never -- so this side
# guards the conversion, and a case that reached it anyway would fail
# here rather than quietly working on one compiler.
#
# It also covers the two things the differential cannot see: the blend
# share, which only the other renderer's hardware computes, and the
# range guard, which nothing reachable can drive.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -O0 -g -Wall -Wextra"
OUT=tests/build
mkdir -p "$OUT"

echo "=== the fade's timer ==="
$CC $CFLAGS -fsanitize=address,undefined -c tests/test_aefade.c \
    -o "$OUT/afd.drv.o"
$CC $CFLAGS -fsanitize=address,undefined -c aftereffects/aefade.c -o "$OUT/afd.o"
$CC -fsanitize=address,undefined "$OUT/afd.drv.o" "$OUT/afd.o" -lm \
    -o "$OUT/afd"
"$OUT/afd"
