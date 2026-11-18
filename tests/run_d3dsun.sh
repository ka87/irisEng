#!/bin/sh
# The unit test for platform/d3dsun.c, the Direct3D side of the sun's
# occlusion test.
#
#   ./tests/run_d3dsun.sh
#
# New code with no earlier copy, so this is a direct check and not a
# differential one. The driver includes d3dsun.c so it can read the
# ring back, and mocks a device that can be told, per surface, whether
# the hardware has finished with it -- which is what the three
# surfaces in the ring exist to cope with.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

echo "=== sun occlusion sampler ==="
$CC $CFLAGS -c tests/test_d3dsun.c -o "$OUT/wgs.drv.o"
$CC "$OUT/wgs.drv.o" -o "$OUT/wgs"
"$OUT/wgs"
