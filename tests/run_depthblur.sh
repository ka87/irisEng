#!/bin/sh
# The unit test for depthblur.c.
#
#   ./tests/run_depthblur.sh
#
# The extraction is checked by tests/run_wingdi_ae.sh, which runs the
# frozen pre-rewrite after-effects beside the rewrite and compares the
# whole ramp, the five distances, the mode and the transform with every
# mode driven on purpose. That is the stronger check and this is not a
# second copy of it.
#
# What this adds is the property the ramp rests on: that the distances
# always come out ordered, which is what keeps its divisions inside the
# table. The frozen copy divides without checking and so does the
# extraction, so "it has never gone out of range" was the whole
# argument -- worth testing rather than believing, with a second
# renderer about to build a texture from the same table.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -O0 -g -Wall -Wextra -Wno-unused-parameter"
OUT=tests/build
mkdir -p "$OUT"

echo "=== depth blur ==="
$CC $CFLAGS -fsanitize=address,undefined -c tests/test_depthblur.c \
    -o "$OUT/dpb.drv.o"
$CC $CFLAGS -fsanitize=address,undefined -c depthblur.c -o "$OUT/dpb.o"
$CC -fsanitize=address,undefined "$OUT/dpb.drv.o" "$OUT/dpb.o" \
    -o "$OUT/dpb" -lm
"$OUT/dpb"
