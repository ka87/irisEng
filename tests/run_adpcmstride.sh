#!/bin/sh
# Where a stream starts when it is played from an offset.
#
#   ./tests/run_adpcmstride.sh
#
# A direct check: the differential never plays from a non-zero offset,
# so it cannot see this arithmetic at all.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests -DWINSOUND_FROZEN"
OUT=tests/build
mkdir -p "$OUT"

$CC $CFLAGS -c tests/test_adpcmstride.c  -o "$OUT/stride.drv.o"
$CC $CFLAGS -c platform/sndbackend.c     -o "$OUT/stride.sut.o"
$CC $CFLAGS -c platform/adpcm.c          -o "$OUT/stride.adpcm.o"
$CC $CFLAGS -c basic_array.c             -o "$OUT/stride.bas.o"
$CC $CFLAGS -c math.c                    -o "$OUT/stride.math.o"
$CC $CFLAGS -c tests/sndbackend_mocks.c  -o "$OUT/stride.mocks.o"
$CC $CFLAGS -DWSND_SIDE_NEW=1 -c tests/sndbackend_side.c -o "$OUT/stride.side.o"
# The 32-bit stand-in for an address, which sndbackend.c keeps its three
# key/value lists in. Nothing here drives those lists; it is linked
# because the file refers to it.
$CC $CFLAGS -c handle.c                  -o "$OUT/stride.handle.o"
$CC -no-pie "$OUT/stride.drv.o" "$OUT/stride.sut.o" "$OUT/stride.adpcm.o" \
    "$OUT/stride.bas.o" "$OUT/stride.math.o" "$OUT/stride.mocks.o" "$OUT/stride.side.o" \
    "$OUT/stride.handle.o" -o "$OUT/adpcmstride" -lm
"$OUT/adpcmstride"
