#!/bin/sh
# The 32-bit stand-in for an address, in handle.c.
#
#   ./tests/run_handles.sh
#
# A direct check rather than a differential one: there is no earlier
# copy of this file to compare against, and the behaviour that matters
# is what it promises rather than what it used to do.
#
# Four parts, because the file has to be right on two builds:
#
#   1. the translation itself, at the host word size, like the rest of
#      the harness -- so on a 64-bit host this is the build the layer
#      exists for and not a model of it
#   2. the path a 32-bit build takes, which is the header's three
#      entry points reduced to the casts the engine already writes.
#      Picked by forcing the switch rather than by compiling -m32, so
#      that it runs on any host
#   3. that handle.c contributes nothing whatever on that path: no
#      code, no data, no symbols of its own. That is what keeps a
#      32-bit binary byte-identical, which the frozen baselines depend
#      on, and it is checkable anywhere for the same reason as 2
#   4. and the same compile under a real 32-bit compiler where one is
#      to hand, which is the only part that needs the i386 headers

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Werror=implicit-function-declaration -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

$CC $CFLAGS -c tests/test_handles.c -o "$OUT/handles.drv.o"
$CC $CFLAGS -c handle.c -o "$OUT/handles.o"
$CC "$OUT/handles.drv.o" "$OUT/handles.o" -o "$OUT/handles" -lm
"$OUT/handles"

# The no-translation path. handle.c is not linked in on purpose: on
# that path there is nothing in it to link, and if this ever needs the
# object file then the header has stopped being self-contained.
$CC $CFLAGS -c tests/test_handles32.c -o "$OUT/handles32.drv.o"
$CC "$OUT/handles32.drv.o" -o "$OUT/handles32" -lm
"$OUT/handles32"

# What handle.c adds to a build that does not translate: nothing.
#
# "No symbols at all" would be the wrong thing to ask for. The engine's
# headers declare globals without extern, so -fcommon gives every
# translation unit that includes them a tentative definition apiece,
# and handle.c gets about eighteen it never mentions. What belongs to
# handle.c itself is everything that is not one of those, plus whatever
# lands in the three sections that carry code and data.
check_contributes_nothing()
{
    arch=$1
    obj=$2
    bad=$(nm "$obj" | grep -v ' C ' | grep -v '^ *$' || true)
    if [ -n "$bad" ]; then
        echo "  FAIL handle.c defines something on a $arch build:"
        echo "$bad" | sed 's/^/    /'
        exit 1
    fi
    for section in .text .data .bss; do
        bytes=$(objdump -h "$obj" \
                | awk -v s="$section" '$2 == s { print $3; exit }')
        if [ -z "$bytes" ]; then
            continue                       # absent is as empty as empty
        fi
        if [ "$(printf '%d' "0x$bytes")" -ne 0 ]; then
            echo "  FAIL handle.c has $bytes bytes of $section on a $arch build"
            exit 1
        fi
    done
    echo "  $arch: no code, no data, no symbols of its own"
}

echo "=== what handle.c adds where pointers already fit ==="
$CC $CFLAGS -DHDL_TRANSLATES=0 -c handle.c -o "$OUT/handles.flat.o"
check_contributes_nothing "this host, switch forced" "$OUT/handles.flat.o"

# And under a real 32-bit compiler, which settles it without the
# switch being forced at all. Compiling is enough -- no link, so no
# 32-bit libc is wanted -- but the engine's headers still need the
# 32-bit C library headers to parse, so this is skipped where they are
# absent rather than failed.
if printf '#include <stdio.h>\nint main(void){return 0;}\n' \
   | $CC -m32 -fsyntax-only -xc - 2>/dev/null; then
    $CC -m32 $CFLAGS -c handle.c -o "$OUT/handles.m32.o"
    check_contributes_nothing "-m32" "$OUT/handles.m32.o"
else
    echo "  SKIP -m32: no 32-bit C library headers here, so the engine's"
    echo "       own headers will not parse. Part 3 above covers the same"
    echo "       property on this host."
fi
