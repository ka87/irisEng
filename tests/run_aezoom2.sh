#!/bin/sh
# The unit test for aftereffects/aezoom2.c.
#
#   ./tests/run_aezoom2.sh
#
# The second zoom smooth: eight samples of the frame, each turned about
# the middle of the picture, summed.
#
# The case this file exists for is the transform one. The effect's
# transform is written as a rotation and is not one -- its second row
# reads the first row's answer -- so its determinant is cos squared and
# every sample is turned *and shrunk*. A tidied rotation would pass
# every other check here and be a different effect with the wrong name,
# so the determinant is asserted against the cosine directly.
#
# The other thing pinned here is the stage split, which decides the
# angles and not just the drawing: the two splits sample the same eight
# angles at some turns and differ by a step of a float at the fourth
# sample at about two turns in three. That is measured over forty
# thousand turns rather than argued, because a first reading assumed
# they always agreed and a turn of one is one of the cases where they
# do.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -O0 -g -Wall -Wextra"
OUT=tests/build
mkdir -p "$OUT"

echo "=== the second zoom smooth ==="
$CC $CFLAGS -fsanitize=address,undefined -c tests/test_aezoom2.c \
    -o "$OUT/az2.drv.o"
$CC $CFLAGS -fsanitize=address,undefined -c aftereffects/aezoom2.c -o "$OUT/az2.o"
$CC -fsanitize=address,undefined "$OUT/az2.drv.o" "$OUT/az2.o" -lm \
    -o "$OUT/az2"
"$OUT/az2"
