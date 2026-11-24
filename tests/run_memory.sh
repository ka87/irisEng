#!/bin/sh
# The engine's own allocator, memory.c.
#
#   ./tests/run_memory.sh
#
# A direct check, and the first of any kind this file has had. It is
# also the one place in the harness where the same binary is built
# twice over the same sources, because the allocator's behaviour
# depends on where its arena lands:
#
#   low      the arena forced below 4 GB, which is where a 32-bit
#            build puts it
#
#   native   the arena wherever the C library puts it, which on a
#            64-bit host is above 4 GB and is what the allocator now
#            has to cope with
#
# Both are required. The low mode was written when the allocator could
# not survive the native one, to characterise what it did so that the
# widening could be shown to preserve it; it is kept because the two
# modes put the arena in different places, and an allocator that works
# at either is the one a 32-bit build needs.
#
# Only SDL's semaphore and five functions on paths the driver never
# takes are mocked: the allocator asks nothing else of the engine.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Werror=implicit-function-declaration -Itests/stub -Itests -Ithirdparty/SDL3/include"
OUT=tests/build
mkdir -p "$OUT"

$CC $CFLAGS -c memory.c                   -o "$OUT/mem.o"
$CC $CFLAGS -c tests/test_memory.c        -o "$OUT/mem.drv.o"
$CC $CFLAGS -c tests/memory_mocks.c       -o "$OUT/mem.mocks.o"
$CC $CFLAGS -c tests/memory_lowarena.c    -o "$OUT/mem.lowarena.o"

$CC "$OUT/mem.o" "$OUT/mem.mocks.o" "$OUT/mem.drv.o" "$OUT/mem.lowarena.o" \
    -Wl,--wrap=malloc -o "$OUT/memory_low" -lm
$CC "$OUT/mem.o" "$OUT/mem.mocks.o" "$OUT/mem.drv.o" \
    -o "$OUT/memory_native" -lm

echo "--- arena below 4 GB, as a 32-bit build has it ---"
"$OUT/memory_low"

echo "--- arena where this host puts it ---"
"$OUT/memory_native"
