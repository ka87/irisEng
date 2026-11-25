#!/bin/sh
# A geometry loaded from a buffer, and the markers in its pointer slots.
#
#   ./tests/run_geo.sh
#   ./tests/run_geo.sh --asan
#
# graphicsobjects/geo.c had no test and no runner built it. Its loader
# parks three values from the file in slots declared as pointers -- the
# vertex colour count, an MRM magic number and a per-chunk UV flag --
# and the two allocators read each one back and replace it with the
# storage they size from it.
#
# The MRM one was compared against the literal 0xc0de0008 as a pointer.
# The value arrives as a signed int and the literal is unsigned, so on a
# build with eight-byte pointers the two are sign and zero extended
# respectively and never match: the second per-vertex array is not
# allocated, the loader skips the block of the file that fills it, and
# everything after that is read from the wrong offset.
#
# That is why the byte count is the assertion that matters here, and why
# the MRM check is a pair of loads differing by exactly that block.
#
# --asan is kept for the allocation sizes, not for a known fault: the
# allocators here are the C library's, so a size the loader gets wrong
# is visible rather than absorbed by the engine's arena.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
# No -Werror=implicit-function-declaration: -w disables it. The loader
# this driver calls is in no header, and calling it implicitly would
# truncate the geometry it hands back. tests/implicitcheck.sh is what
# watches for that, and it sweeps this directory.
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

$CC $CFLAGS -c graphicsobjects/geo.c -o "$OUT/geo.sut.o"
$CC $CFLAGS -c tests/geo_mocks.c     -o "$OUT/geo.mocks.o"
$CC $CFLAGS -c tests/test_geo.c      -o "$OUT/geo.drv.o"
$CC "$OUT/geo.sut.o" "$OUT/geo.mocks.o" "$OUT/geo.drv.o" -o "$OUT/geo" -lm

"$OUT/geo"

if [ "$1" = "--asan" ]; then
    echo "--- the same driver under AddressSanitizer ---"
    for f in graphicsobjects/geo.c tests/geo_mocks.c tests/test_geo.c; do
        $CC $CFLAGS -fsanitize=address -c "$f" \
            -o "$OUT/geo.asan.$(basename "$f" .c).o"
    done
    $CC -fsanitize=address "$OUT/geo.asan.geo.o" "$OUT/geo.asan.geo_mocks.o" \
        "$OUT/geo.asan.test_geo.o" -o "$OUT/geo_asan" -lm
    "$OUT/geo_asan"
fi
