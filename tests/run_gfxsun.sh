#!/bin/sh
# The unit test for the engine's half of gfx/gfxsun.c.
#
#   ./tests/run_gfxsun.sh
#
# gfxsun.c had no harness of any kind: tests/baseline_gfxsun.c is a
# frozen copy that nothing has ever driven, and it was frozen before
# the renderer split, so it cannot be compared across that change.
# This is a direct check of the half that stayed behind.
#
# The real math.c is linked rather than mocked. The projection is
# mocked, because where the ring of positions lands on screen is what
# the clipping has to be driven with, and so is the renderer, so a
# case can say "no answer this frame" without hardware to be slow.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

echo "=== sun, engine side ==="
$CC $CFLAGS -c tests/test_gfxsun.c -o "$OUT/gsun.drv.o"
$CC $CFLAGS -c math.c -o "$OUT/gsun.math.o"
$CC "$OUT/gsun.drv.o" "$OUT/gsun.math.o" -o "$OUT/gsun" -lm
"$OUT/gsun"
