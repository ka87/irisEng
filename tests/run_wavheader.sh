#!/bin/sh
# Reading a sound file's header, for each of the three shapes.
#
#   ./tests/run_wavheader.sh
#
# A direct check: the differential harness skips this function on
# purpose, so nothing else covers the three shapes or the place the
# data tag sits in each.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

# The 32-bit stand-in for an address: the engine files below
# register and release through it, and it has no engine
# dependencies of its own.
$CC $CFLAGS -c handle.c -o "$OUT/wavheader.handle.o"

$CC $CFLAGS -c tests/test_wavheader.c -o "$OUT/wavhdr.drv.o"
$CC $CFLAGS -c sound.c                -o "$OUT/wavhdr.sut.o"
$CC $CFLAGS -c tests/sound_mocks.c    -o "$OUT/wavhdr.mocks.o"
$CC $CFLAGS -c basic_array.c          -o "$OUT/wavhdr.bas.o"
$CC $CFLAGS -c math.c                 -o "$OUT/wavhdr.math.o"
$CC -no-pie "$OUT/wavhdr.drv.o" "$OUT/wavhdr.sut.o" "$OUT/wavhdr.mocks.o" \
    "$OUT/wavhdr.bas.o" "$OUT/wavhdr.math.o" -o "$OUT/wavheader" "$OUT/wavheader.handle.o" -lm
"$OUT/wavheader"
