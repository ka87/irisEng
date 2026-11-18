#!/bin/sh
# The unit test for platform/d3dtex.c.
#
#   ./tests/run_d3dtex.sh
#
# New code replacing two D3DX calls, and a direct check rather than a
# differential one on purpose: the D3DX mocks in the renderer's own
# harness only logged that they had been called and moved no pixels,
# so there was never anything there for a real implementation to be
# compared against. What matters about a box filter is the pixels, and
# the textures and surfaces here have real buffers behind them.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

echo "=== renderer textures ==="
$CC $CFLAGS -fsanitize=address -c tests/test_d3dtex.c -o "$OUT/wgtex.drv.o"
$CC $CFLAGS -fsanitize=address -c platform/d3dtex.c -o "$OUT/wgtex.o"
$CC -fsanitize=address "$OUT/wgtex.drv.o" "$OUT/wgtex.o" -o "$OUT/wgtex"
"$OUT/wgtex"
