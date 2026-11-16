#!/bin/sh
# The unit test for perfbars.c.
#
#   ./tests/run_perfbars.sh
#
# What `tweak perf`'s display decides, with no renderer in it. The
# cases that earn their place are the mode-3 gate (which draws only
# while the frame rate is above its floor, so it shows nothing exactly
# when the frame is slow), the fitting count (which answers the index
# the walk stopped at, and so answers more rows than the page draws
# when everything fits), the selection sort (because every raster that
# has not run averages nought and a library sort would shuffle them),
# and the bar's segments, which shrink by a quarter each.
set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -O0 -g -Wall -Wextra"
OUT=tests/build
mkdir -p "$OUT"

echo "=== the performance display's arithmetic ==="
$CC $CFLAGS -fsanitize=address,undefined -Itests/stub \
    -c tests/test_perfbars.c -o "$OUT/pfb.drv.o"
$CC -fsanitize=address,undefined "$OUT/pfb.drv.o" -lm -o "$OUT/pfb"
"$OUT/pfb"
