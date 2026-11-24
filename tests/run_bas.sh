#!/bin/sh
# The key/value array, against a model of what it should hold.
#
#   ./tests/run_bas.sh
#
# A direct check, and the first of any kind for basic_array.c. Seven
# runners link that file, so it is exercised a great deal and was
# checked not at all: a mutation that made its delete shift one element
# too few survived every one of them.
#
# It is worth more than its size suggests. Every store the 64-bit work
# converted goes through this file, and the keys it compares are the
# handles that work put there.
#
# The allocator is the C library's, so this does not depend on the
# engine's own.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Werror=implicit-function-declaration -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

$CC $CFLAGS -c basic_array.c       -o "$OUT/bas.sut.o"
$CC $CFLAGS -c tests/bas_mocks.c   -o "$OUT/bas.mocks.o"
$CC $CFLAGS -c tests/test_bas.c    -o "$OUT/bas.drv.o"
$CC "$OUT/bas.sut.o" "$OUT/bas.mocks.o" "$OUT/bas.drv.o" -o "$OUT/bas" -lm

"$OUT/bas"

# ./tests/run_bas.sh --asan builds the same driver under
# AddressSanitizer, which is the only thing that can see the over-read
# this file has in the middle-shift branch of its two deletes: the
# element it reads past the end is written to a slot that --actElemNum
# drops on the next line, so no behavioural check can see it, and it is
# out of bounds only when the array is exactly full.
#
# It is not part of the ordinary run because the defect is older than
# this work and what to do about it has not been decided; the mode is
# here so that the demonstration is one command rather than a
# reconstruction.
if [ "$1" = "--asan" ]; then
    echo "--- the same driver under AddressSanitizer ---"
    for f in basic_array.c tests/bas_mocks.c tests/test_bas.c; do
        $CC $CFLAGS -fsanitize=address -c "$f" \
            -o "$OUT/bas.asan.$(basename "$f" .c).o"
    done
    $CC -fsanitize=address "$OUT/bas.asan.basic_array.o" \
        "$OUT/bas.asan.bas_mocks.o" "$OUT/bas.asan.test_bas.o" \
        -o "$OUT/bas_asan" -lm
    "$OUT/bas_asan" || echo "(the over-read above is the known one)"
fi
