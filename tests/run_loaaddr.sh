#!/bin/sh
# The address store in loading.c, the reference resolver that reads it
# in ai_common.c, and the two places where an object's four bytes and
# its address meet on the evaluator's stack.
#
#   ./tests/run_loaaddr.sh
#
# A direct check, and the first of any kind for either: there is no
# differential for loading.c or ai_common.c and no frozen copy of
# either.
#
# Two kinds of check in it. The store's behaviour and the resolver's
# rules about which keys it looks up and which it hands straight back
# hold whether or not the store keeps handles, so they are what says
# the handles did not change anything. The last one does not: a second
# reference to an already-loaded key reads what the store holds and
# hands it back as an address, so before the store held handles it came
# back with its top half missing.
#
# The real loading.c, basic_array.c and ai_common.c are built here; the
# allocator is the C library's, so this does not depend on the engine's
# own, and the big-file key lookup is the driver's so that it can say
# what resolves to what.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Werror=implicit-function-declaration -Itests/stub -Itests -Ithirdparty/SDL3/include"
OUT=tests/build
mkdir -p "$OUT"

$CC $CFLAGS -c loading.c                -o "$OUT/loaaddr.loading.o"
$CC $CFLAGS -c basic_array.c            -o "$OUT/loaaddr.bas.o"
$CC $CFLAGS -c ai_common.c              -o "$OUT/loaaddr.aicommon.o"
$CC $CFLAGS -c handle.c                 -o "$OUT/loaaddr.handle.o"
$CC $CFLAGS -c tests/loaaddr_mocks.c    -o "$OUT/loaaddr.mocks.o"
$CC $CFLAGS -c ai/ai_types.c            -o "$OUT/loaaddr.aitypes.o"
$CC $CFLAGS -c tests/test_loaaddr.c     -o "$OUT/loaaddr.drv.o"
$CC $CFLAGS -c tests/test_aistack.c     -o "$OUT/aistack.drv.o"

common="$OUT/loaaddr.loading.o $OUT/loaaddr.bas.o $OUT/loaaddr.aicommon.o \
$OUT/loaaddr.aitypes.o $OUT/loaaddr.handle.o $OUT/loaaddr.mocks.o"

# shellcheck disable=SC2086
$CC $common "$OUT/loaaddr.drv.o" -o "$OUT/loaaddr" -lm
"$OUT/loaaddr"

# shellcheck disable=SC2086
$CC $common "$OUT/aistack.drv.o" -o "$OUT/aistack" -lm
"$OUT/aistack"
