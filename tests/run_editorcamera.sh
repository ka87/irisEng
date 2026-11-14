#!/bin/sh
# The unit test for editorcamera.c.
#
#   ./tests/run_editorcamera.sh
#
# New code with no earlier copy, so this is a direct check and not a
# differential one. The driver includes editorcamera.c, which is what
# lets it read back which view the camera took and where the camera
# is, and it stands in for the engine around it.
#
# The rotation helpers in the driver are real rather than stubs: a
# stub that did nothing would leave every check about which axis the
# camera moves along passing on code that moved along the wrong one.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests -Ithirdparty/ffmpeg/include"
OUT=tests/build
mkdir -p "$OUT"

echo "=== editor camera ==="
$CC $CFLAGS -c tests/test_editorcamera.c -o "$OUT/ecam.drv.o"
$CC "$OUT/ecam.drv.o" -o "$OUT/ecam" -lm
"$OUT/ecam"
