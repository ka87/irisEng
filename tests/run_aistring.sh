#!/bin/sh
# A script's string constants, across a four-byte node field.
#
#   ./tests/run_aistring.sh
#
# AI_ResolveAllFctRef turns each string-constant node's offset into a
# reference the interpreter can use, and a node's val is an int32_t
# because that is what one node occupies in a world file. It used to
# put the constants block's address there, truncated, while the read
# side -- AI_GetStringFromStack -- had already been converted to
# resolve a handle out of that same word. The two halves disagreed,
# and every script that formats a string goes through them.
#
# This is the writer checked against that reader, with the real handle
# layer linked rather than the identity: what the resolve leaves has to
# resolve back to the string, and must not be the address with its top
# half cut off. Freeing the model has to give the handles back.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
BASE="-std=gnu99 -fcommon -fms-extensions -O0 -g -Itests/stub -Itests"
# The engine sources are compiled quiet, as everywhere here. This
# driver and its mocks are not: -w silently defeats
# -Werror=implicit-function-declaration whatever the order, and an
# implicitly declared allocator returns int, which truncates every
# pointer it hands back. That is the fault this file exists for, and it
# cost two sittings of this driver's own setup before the include was
# put in.
ENGINE_CFLAGS="$BASE -w"
DRV_CFLAGS="$BASE -Werror=implicit-function-declaration"
OUT=tests/build
mkdir -p "$OUT"

$CC $ENGINE_CFLAGS -c ai_common.c            -o "$OUT/aistring.sut.o"
$CC $ENGINE_CFLAGS -c handle.c               -o "$OUT/aistring.hdl.o"
$CC $DRV_CFLAGS -c tests/aistring_mocks.c    -o "$OUT/aistring.mocks.o"
$CC $DRV_CFLAGS -c tests/test_aistring.c     -o "$OUT/aistring.drv.o"
$CC "$OUT/aistring.sut.o" "$OUT/aistring.hdl.o" "$OUT/aistring.mocks.o" \
    "$OUT/aistring.drv.o" -o "$OUT/aistring" -lm

"$OUT/aistring"
