#!/bin/sh
# The differential test for ok3.c.
#
#   ./tests/run_ok3.sh
#
# Standalone on purpose: tests/run.sh is where several passes would
# collide, so this script is the one to run until it is folded in.
# The frozen copy and the rewrite in one binary with BASE_/NEW_
# prefixes, the real math.c linked rather than mocked, and the
# allocator plus INT_PointInTriangle replaced by logging mocks.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

ok3_syms="Face_Plane bevel_2D bevel_3D check_point check_line \
COL_OK3_GetNumberOfFacesInBox COL_pst_OK3_GetTriangle COL_OK3_OptimizeBoxBV \
COL_OK3_TriangleTouchBox COL_OK3_Optimize COL_OK3_SubdivideBox COL_OK3_Build"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

echo "=== ok3 ==="
$CC $CFLAGS -c ok3.c -o "$OUT/ok3.new.o" $(defs NEW_ $ok3_syms)
$CC $CFLAGS -c tests/baseline_ok3.c -o "$OUT/ok3.base.o" $(defs BASE_ $ok3_syms)
$CC $CFLAGS -c math.c -o "$OUT/ok3.math.o"
$CC $CFLAGS -c tests/ok3_mocks.c -o "$OUT/ok3.mocks.o"
$CC $CFLAGS -c tests/test_ok3.c -o "$OUT/ok3.drv.o"
$CC "$OUT/ok3.new.o" "$OUT/ok3.base.o" "$OUT/ok3.math.o" "$OUT/ok3.mocks.o" \
    "$OUT/ok3.drv.o" -o "$OUT/ok3" -lm
"$OUT/ok3"
