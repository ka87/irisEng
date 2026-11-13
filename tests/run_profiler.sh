#!/bin/sh
# The differential test for profiler.c.
#
#   ./tests/run_profiler.sh
#
# Standalone on purpose: tests/run.sh is where several passes would
# collide, so this script is the one to run until it is folded in.
#
# The frozen copy and the rewrite go into one binary. Both the
# functions and the file-scope globals the two copies each define are
# prefixed BASE_/NEW_; the raster structures are tentative definitions
# in profiler.h and so are shared, which is why each case runs one side
# over a cleared world before the other.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

syms="PRO_RasterAdd PRO_InitModule profiler_dbg_init ENG_ForceStartRasters \
PRO_RecordTimeDecRef PRO_PerfCounterStartBlock PRO_PerfCounterEndBlock \
PRO_QPC_RecordTimeDecRef PRO_ClearOneRaster PRO_ClearAllRasters PRO_InitRasters \
PRO_DisplayTextsOnRasterWindow PRO_ProfilerDisplayHandler PRO_ProfilerWindowHandler \
PRO_rasterWindowIsOn PRO_rasterDetailPage PRO_FirstRasterPtr \
pointsToUnkRaster_dword_98F944 PRO_CurrRaster PRO_rasterHWND gfDBGRasterSetTo60"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

echo "=== profiler ==="
$CC $CFLAGS -c profiler.c -o "$OUT/profiler.new.o" $(defs NEW_ $syms)
# the frozen copy still spells one function without its final letter;
# map it onto the same per-side symbol the corrected name takes
$CC $CFLAGS -DPRO_ProfilerWindowHander=BASE_PRO_ProfilerWindowHandler \
    -c tests/baseline_profiler.c -o "$OUT/profiler.base.o" $(defs BASE_ $syms)
$CC $CFLAGS -c tests/profiler_mocks.c -o "$OUT/profiler.mocks.o"
$CC $CFLAGS -c tests/test_profiler.c -o "$OUT/profiler.drv.o"
$CC "$OUT/profiler.new.o" "$OUT/profiler.base.o" "$OUT/profiler.mocks.o" \
    "$OUT/profiler.drv.o" -o "$OUT/profiler" -lm
"$OUT/profiler"
