#!/bin/sh
# The unit test for pngsave.c, the screenshot's file.
#
#   ./tests/run_pngsave.sh
#
# A round trip: a picture of known colours is written and read back
# with libpng's own reader. What it proves is the two things the writer
# does beyond handing the rows over -- the BGRA channel order and the
# row order -- both of which are libpng transformations rather than
# code here, so getting one wrong produces a plausible file rather than
# a broken one.
#
# It is also the check that the file is readable at all, which the
# hand-rolled writer this replaced never had: its own test could only
# read the pixels back out of its own format.
set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -O0 -g -Wall"
OUT=tests/build
mkdir -p "$OUT"

./tests/build_png.sh

echo "=== the screenshot's file ==="
$CC $CFLAGS -fsanitize=address,undefined -Ithirdparty/zlib \
    -Ithirdparty/libpng -c tests/test_pngsave.c -o "$OUT/pngsave.drv.o"
$CC -fsanitize=address,undefined "$OUT/pngsave.drv.o" "$OUT/libpng.a" -lm \
    -o "$OUT/pngsave"
"$OUT/pngsave"
