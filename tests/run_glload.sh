#!/bin/sh
# The unit test for platform/glload.c.
#
#   ./tests/run_glload.sh
#
# New code with no earlier copy, so a direct check rather than a
# differential one.
#
# The loader takes its two ways of resolving a name as arguments,
# which is why this test can exist at all: a driver's
# wglGetProcAddress is not something a test can arrange, but what
# matters about the loader is -- which lookup answers, what a refusal
# comes back as, which name is reported when one is missing, and what
# counts as a good enough version string.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

echo "=== GL loader ==="
$CC $CFLAGS -fsanitize=address -c tests/test_glload.c -o "$OUT/glload.drv.o"
$CC $CFLAGS -fsanitize=address -c platform/glload.c -o "$OUT/glload.o"
$CC -fsanitize=address "$OUT/glload.drv.o" "$OUT/glload.o" -o "$OUT/glload"
"$OUT/glload"
