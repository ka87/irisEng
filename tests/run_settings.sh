#!/bin/sh
# The unit test for settings.c.
#
#   ./tests/run_settings.sh
#
# New code with no earlier copy, so this is a direct check and not a
# differential one. The driver includes settings.c so it can point
# the reader at a file it writes itself, and it reads the shipped
# engine.ini as one of its cases -- so a typo in that file fails
# here rather than in game.
#
# It runs from the repository root, because that case names
# engine.ini relative to it.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

echo "=== settings ==="
$CC $CFLAGS -c tests/test_settings.c -o "$OUT/settings.drv.o"
$CC "$OUT/settings.drv.o" -o "$OUT/settings"
"$OUT/settings"
