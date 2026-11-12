#!/bin/sh
# Differential tests: the pre-refactor copy of a file against the
# rewritten one, in a single binary, over identical state.
#
#   ./tests/run.sh            run everything
#   ./tests/run.sh snake      run the tests whose name matches
#
# Needs nothing but gcc. tests/stub holds just enough of windows.h,
# d3d9.h and dsound.h for the real engine headers to parse; it is not
# the real toolchain, so a batch should still be built under MSVC.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

# Every public name in a file under test, renamed so the two copies can
# live in one binary.
snake_syms="GAO_ModifierSNAKE_Apply GAO_ModifierSNAKE_UpdatePointers \
GAO_ModifierSNAKE_Create GAO_ModifierSNAKE_Destroy GAO_ModifierSNAKE_Load \
GAO_ModifierSNAKE_Reinit GAO_ModifierSNAKE_Unapply"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

fails=0
run_one() {
    name=$1; live=$2; base=$3; incdir=$4; syms=$5
    [ -n "$FILTER" ] && case "$name" in *"$FILTER"*) ;; *) return 0 ;; esac

    printf '=== %s\n' "$name"
    # shellcheck disable=SC2086
    $CC $CFLAGS -c "$live" -o "$OUT/$name.new.o" $(defs NEW_ $syms)
    # shellcheck disable=SC2086
    $CC $CFLAGS -I"$incdir" -c "$base" -o "$OUT/$name.base.o" $(defs BASE_ $syms)
    $CC $CFLAGS -c tests/engine_mocks.c -o "$OUT/$name.mocks.o"
    $CC $CFLAGS -c "tests/test_$name.c" -o "$OUT/$name.drv.o"
    $CC "$OUT/$name.new.o" "$OUT/$name.base.o" "$OUT/$name.mocks.o" \
        "$OUT/$name.drv.o" -o "$OUT/$name" -lm
    "$OUT/$name" || fails=$((fails + 1))
    echo
}

FILTER=$1

run_one snakemodifier modifiers/snakemodifier.c \
        tests/baseline_snakemodifier.c modifiers "$snake_syms"

if [ "$fails" -ne 0 ]; then
    echo "$fails test binary/binaries reported differences"
    exit 1
fi
echo "no differences"
