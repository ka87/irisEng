#!/bin/sh
# Each script type's copier against the width its table declares.
#
#   ./tests/run_aitypes.sh
#
# AI_gast_Types gives every type a dataTypeSize and a copier. The size
# is what the user stack advances by and what an array of that type is
# strided by; the copier is what moves a value. A copier that writes
# more than the declared size runs into the next stack push or the next
# array element, and nothing in the engine compares the two -- one is a
# number in a table, the other a function.
#
# This hands each copier a padded destination and measures what it
# actually writes.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
BASE="-std=gnu99 -fcommon -fms-extensions -O0 -g -Itests/stub -Itests"
# The engine sources quiet, as everywhere here; the driver and its mocks
# with the implicit-declaration diagnostic on, because -w defeats it and
# an implicitly declared function that returns a pointer truncates it.
ENGINE_CFLAGS="$BASE -w"
DRV_CFLAGS="$BASE -Werror=implicit-function-declaration"
OUT=tests/build
mkdir -p "$OUT"

$CC $ENGINE_CFLAGS -c ai/ai_types.c        -o "$OUT/aitypes.sut.o"
$CC $ENGINE_CFLAGS -c handle.c             -o "$OUT/aitypes.hdl.o"
$CC $DRV_CFLAGS -c tests/aitypes_mocks.c   -o "$OUT/aitypes.mocks.o"
$CC $DRV_CFLAGS -c tests/test_aitypes.c    -o "$OUT/aitypes.drv.o"
$CC "$OUT/aitypes.sut.o" "$OUT/aitypes.hdl.o" "$OUT/aitypes.mocks.o" \
    "$OUT/aitypes.drv.o" -o "$OUT/aitypes" -lm

"$OUT/aitypes"
