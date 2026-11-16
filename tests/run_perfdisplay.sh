#!/bin/sh
# The unit test for win/perfdisplay.c.
#
#   ./tests/run_perfdisplay.sh
#
# The page `tweak perf` puts over the frame, drawn once for both
# renderers through the overlay's three interface entries. The driver
# includes perfdisplay.c so the quad batch and the running averages,
# which are file-local, can be read back after a draw.
#
# The case that matters most is not a drawing one: the profiler's
# per-frame sums are a frame's worth only because this display clears
# them once it has shown them. Nothing else in the engine does, so a
# renderer that drew the page without that would show bars which only
# ever grew -- and the clear has to happen even on a frame that drew
# nothing, which is the state mode 3 is in below its floor.
set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
# The engine's headers declare structs inside parameter lists, which
# -Wall has a lot to say about; the stubs stand in for windows.h.
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

echo "=== the performance display ==="
$CC $CFLAGS -fsanitize=address,undefined\
    -c tests/test_perfdisplay.c -o "$OUT/pfd.drv.o"
$CC $CFLAGS -fsanitize=address,undefined -c perfbars.c -o "$OUT/pfd.pfb.o"
$CC -fsanitize=address,undefined "$OUT/pfd.drv.o" "$OUT/pfd.pfb.o" -lm \
    -o "$OUT/pfd"
"$OUT/pfd"
