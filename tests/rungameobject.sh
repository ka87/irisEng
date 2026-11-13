#!/bin/sh
# Differential test for gameobject.c, as a standalone runner.
#
#   ./tests/rungameobject.sh
#
# It is separate from run.sh only because other files are being
# rewritten in parallel and run.sh is where that work would collide; the
# body below is the same shape as run.sh's run_* functions and is meant
# to be folded in.
#
# gameobject.c sits at the repo root, so the frozen copy includes its
# headers through "../" for the same reason world.c's and animation.c's
# do: -I. would put the engine's own strings.h in front of the system
# one. The file defines exactly one non-common global, AI_MainActor, so
# that is renamed along with the functions; everything else the file
# touches is a tentative definition the two copies share, and the driver
# snapshots those into the scene where a case needs them compared.
#
# math.c is linked rather than mocked, so the matrix and vector helpers
# are the engine's own -- and MATH_InitModule() is called before the
# first case, because MATH_gl_TableRacine is what OBJ_uc_CalcLODVis
# reads and an uninitialised table would make every LOD come out the
# same on both sides.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

gameobject_syms="$(grep -oP '^[A-Za-z_][\w \*]*?\s\*?\K[A-Za-z_]\w*(?=\s*\()' gameobject.c | sort -u) AI_MainActor"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

# shellcheck disable=SC2086
$CC $CFLAGS -c gameobject.c -o "$OUT/gameobject.new.o" $(defs NEW_ $gameobject_syms)
# shellcheck disable=SC2086
$CC $CFLAGS -c tests/baseline_gameobject.c -o "$OUT/gameobject.base.o" $(defs BASE_ $gameobject_syms)
$CC $CFLAGS -c math.c -o "$OUT/gameobject.math.o"
$CC $CFLAGS -c tests/gameobject_mocks.c -o "$OUT/gameobject.mocks.o"
$CC $CFLAGS -c tests/test_gameobject.c -o "$OUT/gameobject.drv.o"
$CC "$OUT/gameobject.new.o" "$OUT/gameobject.base.o" "$OUT/gameobject.math.o" \
    "$OUT/gameobject.mocks.o" "$OUT/gameobject.drv.o" -o "$OUT/gameobject" -lm

"$OUT/gameobject"
