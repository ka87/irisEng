#!/bin/sh
# A material loaded from a buffer, and its multi-texture chain.
#
#   ./tests/run_geomat.sh
#   ./tests/run_geomat.sh --asan
#
# graphicsobjects/geomaterial.c had no test and no runner built it. The
# multi-texture loader built its chain through the address of the
# material's texture slot, which was an int: four of a pointer's eight
# bytes went into it and four into the field above, and the validator
# then followed the sign-extended low half. This is the direct check for
# that path.
#
# The plain mode above is what sees that: against the int slot it does
# not reach an assertion, because the validator follows the truncated
# head inside the loader.
#
# --asan builds the same driver under AddressSanitizer and sees a
# different fault, which nothing behavioural can: a type 4 material is
# the first five fields of tGEOMAT and was allocated to a literal 20
# bytes, which the five fields outgrow by eight once a pointer is eight.
# The allocator rounds up, so the plain mode passes with it present.
#
# A third fault here is seen by neither mode on its own. The eight-byte
# store into the four-byte slot ran into validityFlag, which is inside
# the material's own allocation -- so ASan has nothing to say about it
# -- and the validator assigns that field before anything reads it, so
# no assertion can see it either. It only ever showed up as the
# truncated head, which is what the plain mode catches.
#
# The allocator here is the C library's, so neither depends on the
# engine's arena.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
# No -Werror=implicit-function-declaration here: -w disables it, so the
# copy of it the other runners carry has never fired. The loaders this
# driver calls are in no header, and calling one implicitly truncates
# the material it hands back -- which is the same fault this runner is
# about, arriving through the driver instead. tests/implicitcheck.sh is
# what actually watches for that, and it sweeps this directory too.
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

$CC $CFLAGS -c graphicsobjects/geomaterial.c -o "$OUT/geomat.sut.o"
$CC $CFLAGS -c tests/geomat_mocks.c          -o "$OUT/geomat.mocks.o"
$CC $CFLAGS -c tests/test_geomat.c           -o "$OUT/geomat.drv.o"
$CC "$OUT/geomat.sut.o" "$OUT/geomat.mocks.o" "$OUT/geomat.drv.o" \
    -o "$OUT/geomat" -lm

"$OUT/geomat"

if [ "$1" = "--asan" ]; then
    echo "--- the same driver under AddressSanitizer ---"
    for f in graphicsobjects/geomaterial.c tests/geomat_mocks.c tests/test_geomat.c; do
        $CC $CFLAGS -fsanitize=address -c "$f" \
            -o "$OUT/geomat.asan.$(basename "$f" .c).o"
    done
    $CC -fsanitize=address "$OUT/geomat.asan.geomaterial.o" \
        "$OUT/geomat.asan.geomat_mocks.o" "$OUT/geomat.asan.test_geomat.o" \
        -o "$OUT/geomat_asan" -lm
    "$OUT/geomat_asan"
fi
