#!/bin/sh
# The unit test for platform/d3dmath.c.
#
#   ./tests/run_d3dmath.sh
#
# New code replacing four D3DX calls, so this is a direct check and
# not a differential one. The reference inverses and the reference
# product in the driver were computed outside it in exact rational
# arithmetic; each inverse is also checked by multiplying it back and
# asking for the identity, which depends on neither the reference nor
# the formula under test.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

echo "=== renderer matrices ==="
$CC $CFLAGS -c tests/test_d3dmath.c -o "$OUT/wgmath.drv.o"
$CC $CFLAGS -c platform/d3dmath.c -o "$OUT/wgmath.o"
$CC "$OUT/wgmath.drv.o" "$OUT/wgmath.o" -o "$OUT/wgmath" -lm
"$OUT/wgmath"
