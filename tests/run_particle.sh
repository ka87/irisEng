#!/bin/sh
# The differential test for particlegenerator.c.
#
#   ./tests/run_particle.sh
#
# Standalone on purpose: tests/run.sh is where several passes would
# collide, so this script is the one to run until it is folded in.
# Same shape as run.sh's own recipes -- the frozen copy and the
# rewrite in one binary with BASE_/NEW_ prefixes, the real math.c
# linked rather than mocked, and the engine around them stubbed.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

syms="PAG_Update PAG_Generate PAG_RenderCommon PAGRenderingEnabled PAG_l_ScaleToRandomSpeed"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

echo "=== particlegenerator ==="
$CC $CFLAGS -c particlegenerator.c -o "$OUT/pag.new.o" $(defs NEW_ $syms)
$CC $CFLAGS -c tests/baseline_particlegenerator.c -o "$OUT/pag.base.o" $(defs BASE_ $syms)
$CC $CFLAGS -c math.c -o "$OUT/pag.math.o"
$CC $CFLAGS -c tests/particle_mocks.c -o "$OUT/pag.mocks.o"
$CC $CFLAGS -c tests/test_particle.c -o "$OUT/pag.drv.o"
$CC "$OUT/pag.new.o" "$OUT/pag.base.o" "$OUT/pag.math.o" \
    "$OUT/pag.mocks.o" "$OUT/pag.drv.o" -o "$OUT/particle" -lm
"$OUT/particle"
