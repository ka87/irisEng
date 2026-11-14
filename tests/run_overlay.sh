#!/bin/sh
# The unit test for win/overlay.c.
#
#   ./tests/run_overlay.sh
#
# The overlay is new code, so there is no frozen copy to compare it
# against: the driver includes overlay.c directly, which is what lets
# it read the quad batch and the text queue back after a draw, and
# stands in for the three D3D entry points and the debug font itself.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

echo "=== overlay ==="
$CC $CFLAGS -c tests/test_overlay.c -o "$OUT/overlay.drv.o"
$CC "$OUT/overlay.drv.o" -o "$OUT/overlay" -lm
"$OUT/overlay"
