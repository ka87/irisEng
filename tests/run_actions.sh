#!/bin/sh
# Differential test for actions.c, as a standalone runner.
#
#   ./tests/run_actions.sh
#
# It is separate from run.sh only because other files were being
# rewritten in parallel and run.sh is where that work would collide; the
# body below is the same shape as run.sh's run_* functions and is meant
# to be folded in.
#
# actions.c sits at the repo root, so the frozen copy includes its
# headers through "../": -I. would put the engine's own time.h in front
# of the system one.
#
# The real basic_array.c and math.c are linked rather than mocked, so
# the transition search and the matrix copy are the engine's own. The
# loader, the big-file reader and the animation calls are the harness's.
#
# -no-pie, because a transition is stored through BAS as an int and read
# back as a pointer; that round trips only while the data sits below
# 2GB, which is where the engine's own 32-bit build puts it.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -no-pie -fno-pie -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

act_syms="ACT_FreeAction ACT_ActionRestoreInit ACT_FreeActionKit \
ACT_pst_FindTransition ACT_SetAction ACT_SetUsageCounts ACT_ul_ActionCallback \
ACT_ul_ActionKitCallback ACT_UseAction ACT_ActionSaveInit ACTKitMerge \
ACTKitMergeNoOver ACTKitMergeOver"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

echo "=== actions ==="
# shellcheck disable=SC2086
$CC $CFLAGS -c actions.c -o "$OUT/actions.new.o" $(defs NEW_ $act_syms)
# shellcheck disable=SC2086
$CC $CFLAGS -c tests/baseline_actions.c -o "$OUT/actions.base.o" $(defs BASE_ $act_syms)
$CC $CFLAGS -c math.c -o "$OUT/actions.math.o"
$CC $CFLAGS -c basic_array.c -o "$OUT/actions.bas.o"
$CC $CFLAGS -c tests/actions_mocks.c -o "$OUT/actions.mocks.o"
$CC $CFLAGS -c tests/test_actions.c -o "$OUT/actions.drv.o"
$CC -no-pie "$OUT/actions.new.o" "$OUT/actions.base.o" "$OUT/actions.math.o" \
    "$OUT/actions.bas.o" "$OUT/actions.mocks.o" "$OUT/actions.drv.o" \
    -o "$OUT/actions" -lm

"$OUT/actions"
