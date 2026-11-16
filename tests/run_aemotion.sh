#!/bin/sh
# The unit test for aemotion.c.
#
#   ./tests/run_aemotion.sh
#
# The motion smooth's decision and its four samples. Two of its
# properties cannot be seen in a picture at all:
#
#   - the threshold is not a round number and the comparison is strict,
#     so the movement equal to it must not smear and the one a hair
#     above it must. Either side of that the picture is the same;
#   - the distance that decides is measured in three dimensions and the
#     smear uses two, so a camera moving straight down its own line of
#     sight crosses the threshold and then smears by nothing. A test
#     that moved the camera diagonally would never notice.
#
# What is not here is that last frame's point must be replaced whether
# or not the smear ran. This module stores nothing, so that rule is the
# caller's and is asserted in the two renderer harnesses instead.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -O0 -g -Wall -Wextra"
OUT=tests/build
mkdir -p "$OUT"

echo "=== the motion smooth ==="
$CC $CFLAGS -fsanitize=address,undefined -c tests/test_aemotion.c \
    -o "$OUT/amo.drv.o"
$CC $CFLAGS -fsanitize=address,undefined -c aemotion.c -o "$OUT/amo.o"
$CC -fsanitize=address,undefined "$OUT/amo.drv.o" "$OUT/amo.o" -lm \
    -o "$OUT/amo"
"$OUT/amo"
