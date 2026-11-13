#!/bin/sh
# The differential test for materialanddraw.c.
#
#   ./tests/run_matdraw.sh
#
# Standalone on purpose: tests/run.sh is where several passes would
# collide, so this script is the one to run until it is folded in.
#
# The frozen copy and the rewrite go into one binary with BASE_/NEW_
# prefixes. The real math.c is linked rather than mocked -- the driver
# calls MATH_InitModule before the first case, so the inverse table the
# two z-sort walks index is the engine's own -- and so is the real
# profiler.c, which is what the restored start/end blocks are measured
# against.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

syms="MAT_IsMaterialTransparent MAT_SetRenderState MAT_DrawIndexedTriangle_ST \
MAT_IsObjectTransparent MAT_MaterialsHaveAlphaBlendingType3And4 \
MAT_IsLastElemInMultiTexture MAT_DrawIndexedTriangle_MT MAT_DrawIndexedTriangle \
GAO_XMN_Render_TransparentTriangles GEO_Render_TransparentTriangles \
MAT_DrawIndexedSprites_MT MAT_DrawIndexedSprites MAT_GetValidityMASK \
MAT_VUISIdentity MAT_UV_COMPUTE_GetMAtrix MAT_SetScale MAT_SetSpeed MAT_SetPos \
MAT_SetRotation MAT_GetSpeed MAT_GetScale MAT_GetRotation"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

echo "=== matdraw ==="
$CC $CFLAGS -c materialanddraw.c -o "$OUT/matdraw.new.o" $(defs NEW_ $syms)
$CC $CFLAGS -c tests/baseline_matdraw.c -o "$OUT/matdraw.base.o" $(defs BASE_ $syms)
$CC $CFLAGS -c math.c -o "$OUT/matdraw.math.o"
$CC $CFLAGS -c profiler.c -o "$OUT/matdraw.profiler.o"
$CC $CFLAGS -c tests/matdraw_mocks.c -o "$OUT/matdraw.mocks.o"
$CC $CFLAGS -c tests/test_matdraw.c -o "$OUT/matdraw.drv.o"
$CC "$OUT/matdraw.new.o" "$OUT/matdraw.base.o" "$OUT/matdraw.math.o" \
    "$OUT/matdraw.profiler.o" "$OUT/matdraw.mocks.o" "$OUT/matdraw.drv.o" \
    -o "$OUT/matdraw" -lm
"$OUT/matdraw"
