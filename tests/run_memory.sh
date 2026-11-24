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
#            build puts it and where the 32-bit words the allocator
#            keeps its base, its bump pointer and its free-block
#            links in are wide enough. This has to pass, now and
#            after any widening -- it is the characterisation of what
#            the allocator does today
#
#   native   the arena wherever the C library puts it, which on a
#            64-bit host is above 4 GB. The allocator cannot survive
#            that yet, so this mode is run for the report and not for
#            the verdict -- but if it ever passes, this script fails
#            on purpose, because that means the widening landed and
#            the mode should be required from then on
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
if "$OUT/memory_native" > "$OUT/memory_native.log" 2>&1; then
    nativeRc=0
else
    nativeRc=$?
fi
sed 's/^/  /' "$OUT/memory_native.log"

if [ "$nativeRc" -eq 0 ]; then
    case $(printf '%d' $(getconf LONG_BIT 2>/dev/null || echo 32)) in
    32)
        echo "  pointers are 32 bits here, so both modes are the same build"
        ;;
    *)
        echo "  FAIL this mode passed, which it could not before the"
        echo "       allocator stopped keeping addresses in 32-bit words."
        echo "       That work has evidently landed: make this mode"
        echo "       required instead of reported, and delete this branch"
        echo "       along with tests/memory_lowarena.c if the low mode is"
        echo "       no longer telling you anything."
        exit 1
        ;;
    esac
else
    echo "  exit $nativeRc -- expected on this build, and not a failure here."
    echo "  The allocator keeps its arena base, its bump pointer and a free"
    echo "  block's links in 32-bit words, so with the arena above 4 GB the"
    echo "  first write to a block header goes to a truncated address. The"
    echo "  low mode above is what covers the logic until that is widened."
fi
