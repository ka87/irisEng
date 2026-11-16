#!/bin/sh
# The unit test for cinebands.c.
#
#   ./tests/run_cinebands.sh
#
# The cutscene letterbox's one decision. Small, but it is the gate that
# matters: two halves, and dropping either is invisible until a
# cutscene runs. It is also the number the other renderer's
# after-effects have to agree with, and they do not share the code --
# so this is where that number is written down.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -O0 -g -Wall -Wextra"
OUT=tests/build
mkdir -p "$OUT"

echo "=== cutscene bands ==="
$CC $CFLAGS -fsanitize=address,undefined -c tests/test_cinebands.c \
    -o "$OUT/cbd.drv.o"
$CC $CFLAGS -fsanitize=address,undefined -c cinebands.c -o "$OUT/cbd.o"
$CC -fsanitize=address,undefined "$OUT/cbd.drv.o" "$OUT/cbd.o" -o "$OUT/cbd"
"$OUT/cbd"
