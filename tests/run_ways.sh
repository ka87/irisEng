#!/bin/sh
# The differential test for ways.c.
#
#   ./tests/run_ways.sh
#
# Standalone on purpose: tests/run.sh is where several passes would
# collide, so this script is the one to run until it is folded in.
# The frozen copy and the rewrite go into one binary with BASE_/NEW_
# prefixes, the real basic_array.c is linked rather than mocked so the
# seen set is the engine's own, and everything else -- the allocator,
# the loader's readers and the big-file reader -- is a logging mock.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

syms="WAY_FreeStruct WAY_ReinitLinks WAY_RemoveAllNetworks WAY_ul_NetWorkCallback \
WAY_ul_AllNetWorksCallback WAY_InitModule WAY_CloseModule WAY_pst_SearchLinkList \
WAY_pst_SearchOneLinkInList WAY_ComputePoundNetworksRec WAY_ComputePoundOneNetworkForObj \
WAY_ComputePoundOneNetwork WAY_ComputePoundNetworks WAY_ul_AllLinkListsCallback \
WAY_gst_Seen gpst_CurrentWorld WAY_gi_ShortParams"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

echo "=== ways ==="
$CC $CFLAGS -c ways.c -o "$OUT/ways.new.o" $(defs NEW_ $syms)
$CC $CFLAGS -c tests/baseline_ways.c -o "$OUT/ways.base.o" $(defs BASE_ $syms)
$CC $CFLAGS -c basic_array.c -o "$OUT/ways.bas.o"
$CC $CFLAGS -c tests/ways_mocks.c -o "$OUT/ways.mocks.o"
$CC $CFLAGS -c tests/test_ways.c -o "$OUT/ways.drv.o"
$CC "$OUT/ways.new.o" "$OUT/ways.base.o" "$OUT/ways.bas.o" \
    "$OUT/ways.mocks.o" "$OUT/ways.drv.o" -o "$OUT/ways" -lm
"$OUT/ways"
