#!/bin/sh
# The unit test for platform/wingdivideo.c, the Direct3D side of the
# fullscreen video display.
#
#   ./tests/run_wingdivideo.sh
#
# New code with no earlier copy, so this is a direct check and not a
# differential one. The device is mocked and records what it was asked
# for, in order, so the checks are on the sequence as much as on the
# result -- which is what the failure paths need, since what they have
# to get right is giving back what they had already made.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

echo "=== fullscreen video display ==="
$CC $CFLAGS -c platform/wingdivideo.c -o "$OUT/wgv.o"
$CC $CFLAGS -c tests/test_wingdivideo.c -o "$OUT/wgv.drv.o"
$CC "$OUT/wgv.o" "$OUT/wgv.drv.o" -o "$OUT/wgv"
"$OUT/wgv"
