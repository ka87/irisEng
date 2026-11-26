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
#
# And HDL_TRANSLATES=0 for the same reason, which is what lets this
# differential keep comparing raw bytes. actions.c now puts a handle in
# the four-byte value beside a transition's key, and the frozen copy
# puts a truncated address there, so the two disagree at that word by
# construction -- 92 of the 864 cases, measured. Forcing the identity
# form of the translation layer makes the new copy store the same
# truncated address the frozen one does, which under -no-pie is
# lossless, so the comparison stays byte for byte and keeps all of its
# teeth about what this test is actually for: the rewrite.
#
# What it does NOT then exercise is the translating path through those
# three sites. That is tests/test_actrefs.c, run below, which drives the
# same store, search and free with translation on -- the one thing this
# differential cannot see.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -no-pie -fno-pie -Itests/stub -Itests"
# IRIS_FROZEN_SIZES: the frozen copy sizes an action's block at twenty
# bytes an element, which is what tActionElem measures where pointers
# are four, and it cannot be changed. This build pins every such figure in the rewrite to
# the same figures so the two agree on what the allocator was asked
# for; the mock's 512-byte guard covers the elements being wider than
# that here. Whether the real size is enough is checked below instead,
# which is the half a differential against a 32-bit copy cannot do.
DIFFCFLAGS="$CFLAGS -DHDL_TRANSLATES=0 -DIRIS_FROZEN_SIZES"
# The translating build: no -no-pie and nothing forced, so the layer
# is whatever this host's pointer width asks for.
CFLAGS_T="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

# The 32-bit stand-in for an address: the engine files below
# register and release through it, and it has no engine
# dependencies of its own.
$CC $DIFFCFLAGS -c handle.c -o "$OUT/actions.handle.o"

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
$CC $DIFFCFLAGS -c actions.c -o "$OUT/actions.new.o" $(defs NEW_ $act_syms)
# shellcheck disable=SC2086
$CC $DIFFCFLAGS -c tests/baseline_actions.c -o "$OUT/actions.base.o" $(defs BASE_ $act_syms)
$CC $DIFFCFLAGS -c math.c -o "$OUT/actions.math.o"
$CC $DIFFCFLAGS -c basic_array.c -o "$OUT/actions.bas.o"
$CC $DIFFCFLAGS -c tests/actions_mocks.c -o "$OUT/actions.mocks.o"
$CC $DIFFCFLAGS -c tests/test_actions.c -o "$OUT/actions.drv.o"
$CC -no-pie "$OUT/actions.new.o" "$OUT/actions.base.o" "$OUT/actions.math.o" \
    "$OUT/actions.bas.o" "$OUT/actions.mocks.o" "$OUT/actions.drv.o" \
    -o "$OUT/actions" "$OUT/actions.handle.o" -lm

"$OUT/actions"

# And the same three sites with the translation layer really on, which
# is the part the differential above gives up to keep its byte
# comparison. One copy of actions.c, its own names, no -no-pie: a
# handle here is a number that is nothing like the address it stands
# for, and the first check in the driver is that it is.
$CC $CFLAGS_T -c handle.c              -o "$OUT/actrefs.handle.o"
$CC $CFLAGS_T -c actions.c             -o "$OUT/actrefs.actions.o"
$CC $CFLAGS_T -c math.c                -o "$OUT/actrefs.math.o"
$CC $CFLAGS_T -c basic_array.c         -o "$OUT/actrefs.bas.o"
$CC $CFLAGS_T -c tests/actions_mocks.c -o "$OUT/actrefs.mocks.o"
$CC $CFLAGS_T -c tests/test_actrefs.c  -o "$OUT/actrefs.drv.o"
$CC "$OUT/actrefs.actions.o" "$OUT/actrefs.math.o" "$OUT/actrefs.bas.o" \
    "$OUT/actrefs.mocks.o" "$OUT/actrefs.drv.o" "$OUT/actrefs.handle.o" \
    -o "$OUT/actrefs" -lm

"$OUT/actrefs"
