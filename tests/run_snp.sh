#!/bin/sh
# Differential test for snp.c, as a standalone runner.
#
#   ./tests/run_snp.sh
#
# It is separate from run.sh only because other files were being
# rewritten in parallel and run.sh is where that work would collide; the
# body below is the same shape as run.sh's run_* functions and is meant
# to be folded in.
#
# snp.c sits at the repo root, so the frozen copy includes its headers
# through "../". LOA_ul_FileTypeSize_38 is renamed as well as the
# functions, because loading.h declares it without extern and the two
# copies would otherwise share one tentative definition and the running
# totals would not be independent.
#
# The three engine calls snp.c makes are redirected into the harness so
# the sequence can be compared. MATH_TransformVertexNoScale is only
# wrapped: the real math.c is linked and the wrapper delegates to it, so
# the arithmetic is the engine's own.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

snp_syms="INT_SnP_pstCreateAxisTable INT_SnP_Alloc INT_SnP_DesAlloc INT_SnP_Reinit \
INT_SnP_GetFlag INT_SnP_SetFlag INT_SnP_FullGetFlag INT_SnP_FullSetFlag \
INT_SnP_ResetAllReferenceFlags INT_SnP_GetFirstFreeIndex INT_SnP_ObjectMustBeRemoved \
INT_SnP_AddNode INT_SnP_AddBox INT_SnP_AddSphere INT_SnP_AttachObject \
INT_SnP_AddObject INT_SnP_Refresh INT_SnP_AddOverlap INT_SnP_RemoveOverlap \
INT_SnP_UpdateAllDetectionLists INT_RegisterObjectForRemoving \
INT_SnP_UpdateAxisTable INT_SnP_DetachObject INT_SnP_UnknownInversionSortInner \
INT_SnP_ComputeInversion INT_SnP_InsertionSort \
LOA_ul_FileTypeSize_38"

MOCKDEFS="-DOBJ_pst_GetAbsoluteMatrix=SNPMOCK_GetAbsoluteMatrix \
-DOBJ_BV_IsAABBox=SNPMOCK_BV_IsAABBox \
-DTAB_TableElemIsInvalid=SNPMOCK_TableElemIsInvalid \
-DMATH_TransformVertexNoScale=SNPMOCK_TransformVertexNoScale"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

echo "=== snp ==="
# shellcheck disable=SC2086
$CC $CFLAGS $MOCKDEFS -c snp.c -o "$OUT/snp.new.o" $(defs NEW_ $snp_syms)
# shellcheck disable=SC2086
$CC $CFLAGS $MOCKDEFS -c tests/baseline_snp.c -o "$OUT/snp.base.o" $(defs BASE_ $snp_syms)
$CC $CFLAGS -c math.c -o "$OUT/snp.math.o"
$CC $CFLAGS -c tests/snp_mocks.c -o "$OUT/snp.mocks.o"
$CC $CFLAGS -c tests/test_snp.c -o "$OUT/snp.drv.o"
$CC "$OUT/snp.new.o" "$OUT/snp.base.o" "$OUT/snp.math.o" \
    "$OUT/snp.mocks.o" "$OUT/snp.drv.o" -o "$OUT/snp" -lm

"$OUT/snp"
