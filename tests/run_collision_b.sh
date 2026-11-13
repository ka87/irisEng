#!/bin/sh
# Differential test for collision.c functions 64-97 (the middle third).
#
#   ./tests/run_collision_b.sh
#
# Standalone on purpose: the three ranges of collision.c are being
# rewritten in parallel and tests/run.sh is shared, so this is folded
# into run.sh after the three are merged.
#
# The whole frozen collision.c and the whole live one go into one
# binary; every function and initialised global either defines is
# renamed so the two can coexist. The common symbols the engine's
# headers declare are left shared on purpose -- they are the world both
# copies read, and the driver rebuilds its fixture before each side.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

# Every function and initialised global collision.c defines, taken from
# the object file so the list cannot drift: nm, types T (text) and D
# (initialised data).
$CC $CFLAGS -c collision.c -o "$OUT/collision_b.probe.o"
SYMS=$(nm -g --defined-only "$OUT/collision_b.probe.o" \
       | awk '$2 == "T" || $2 == "D" { print $3 }' | sort)

defs() {
    prefix=$1
    shift
    for s in $SYMS; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

# shellcheck disable=SC2046
$CC $CFLAGS -c collision.c -o "$OUT/collision_b.new.o" $(defs NEW_)
# shellcheck disable=SC2046
$CC $CFLAGS -iquote . -c tests/baseline_collision.c -o "$OUT/collision_b.base.o" $(defs BASE_)
$CC $CFLAGS -c math.c -o "$OUT/collision_b.math.o"
$CC $CFLAGS -c tests/collision_b_mocks.c -o "$OUT/collision_b.mocks.o"
$CC $CFLAGS -c tests/test_collision_b.c -o "$OUT/collision_b.drv.o"
$CC "$OUT/collision_b.new.o" "$OUT/collision_b.base.o" "$OUT/collision_b.math.o" \
    "$OUT/collision_b.mocks.o" "$OUT/collision_b.drv.o" -o "$OUT/collision_b" -lm

"$OUT/collision_b"
