#!/bin/sh
# The unit test for renderers.c.
#
#   ./tests/run_renderers.sh
#
# New code with no earlier copy, so this is a direct check and not a
# differential one. The driver includes renderers.c and drives the
# picker through tables of its own, so a case can say "this renderer
# cannot run" without there being a machine that cannot run it.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

echo "=== renderer picker ==="
$CC $CFLAGS -c tests/test_renderers.c -o "$OUT/renderers.drv.o"
$CC "$OUT/renderers.drv.o" -o "$OUT/renderers"
"$OUT/renderers"
