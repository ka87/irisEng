#!/bin/sh
# The debugging allocator, memdebug.inc.
#
#   ./tests/run_memdebug.sh
#
# This is what the engine is swapped onto when something is writing
# over something else: -DIRIS_MEM_DEBUG renames the pool out of the
# way and this file takes its names, giving every engine block its own
# malloc. A write past a block then lands in the C library's guard
# rather than in the next engine object, and a block handed back twice
# is named rather than put on a free chain it is already on.
#
# It had no coverage of any kind, and carried three faults of the
# shape it exists to find -- a header size, an aligned descriptor and
# the slack in front of it, all of them figures that were right when a
# pointer was four bytes. None of them can be seen on a 32-bit build,
# so nothing here would have caught them either until this runner
# existed.
#
# With AddressSanitizer where the toolchain has it, and without where
# it does not: the separation is most of the value and does not need
# it. The file compiles either way now, which is what lets MSVC build
# it at all.
set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Werror=implicit-function-declaration -DIRIS_MEM_DEBUG -Itests/stub -Itests -Ithirdparty/SDL3/include"
OUT=tests/build
mkdir -p "$OUT"

# Only where the host has it. The file is built both ways on purpose:
# the sanitizer must not be what makes it correct.
if printf 'int main(void){return 0;}\n' > "$OUT/asan_probe.c" \
   && $CC -fsanitize=address "$OUT/asan_probe.c" -o "$OUT/asan_probe" 2>/dev/null
then
    ASAN="-fsanitize=address"
    echo "--- with AddressSanitizer ---"
else
    ASAN=""
    echo "--- without AddressSanitizer, which this host does not have ---"
fi

for san in "$ASAN" ""; do
    [ -z "$ASAN" ] && [ -z "$san" ] && [ -n "$ran_plain" ] && continue
    tag=plain
    [ -n "$san" ] && tag=asan
    $CC $CFLAGS $san -c memory.c             -o "$OUT/memdbg.$tag.o"
    $CC $CFLAGS $san -c tests/test_memdebug.c -o "$OUT/memdbg.$tag.drv.o"
    $CC $CFLAGS $san -c tests/memory_mocks.c  -o "$OUT/memdbg.$tag.mocks.o"
    $CC $san "$OUT/memdbg.$tag.o" "$OUT/memdbg.$tag.mocks.o" "$OUT/memdbg.$tag.drv.o" \
        -o "$OUT/memdbg.$tag" -lm
    [ -n "$san" ] && echo "--- with AddressSanitizer ---" || echo "--- without it ---"
    ASAN_OPTIONS=detect_leaks=0 "$OUT/memdbg.$tag"
    ran_plain=1
done
