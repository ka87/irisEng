#!/bin/sh
# The unit test for pngwrite.c.
#
#   ./tests/run_pngwrite.sh
#
# The writer was moved out of win/wingdi.c whole and the Direct3D
# differential proves the move: the frozen copy still has it inline and
# the two write the same file byte for byte. What that cannot see is the
# parameter the move added -- bottomUp, for a frame OpenGL hands back
# with its bottom row first -- so this reads the file back and decodes
# its rows. No decoder is needed: the pixels are stored uncompressed.
set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -O0 -g -Wall -Wextra"
OUT=tests/build
mkdir -p "$OUT"

echo "=== the screenshot's file format ==="
$CC $CFLAGS -fsanitize=address,undefined -c tests/test_pngwrite.c \
    -o "$OUT/png.drv.o"
$CC -fsanitize=address,undefined "$OUT/png.drv.o" -lm -o "$OUT/png"
"$OUT/png"
