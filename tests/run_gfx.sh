#!/bin/sh
# The differential test for gfx.c.
#
#   ./tests/run_gfx.sh
#
# Standalone on purpose: tests/run.sh is where several passes would
# collide, so this script is the one to run until it is folded in.
# Same shape as run.sh's own recipes -- the frozen copy and the
# rewrite in one binary with BASE_/NEW_ prefixes, every engine entry
# point gfx.c reaches replaced by a logging mock.
#
# -no-pie is not cosmetic: the frozen copy casts the vertex-colour
# pointer through (unsigned int), which is lossless on the engine's
# 32-bit target and lossless here only while the arena lives at an
# address below 2^32.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

gfx_syms="GFX_NeedGeomEx GFX_NeedGeomExElem GFX_End GFX_SetCurrent GFX_FlagSet \
GFX_SetMaterial GFX_NeedGeomNbElemSprites GFX_NeedGeomElemNbSprites GFX_i_Request \
GFX_NeedGeom GFX_SetQuadIndices GFX_SetQuadUVs GFX_RenderGeom GFX_pst_FindById \
GFX_SetRenderState GFX_Attached GFX_FreeOne GFX_Del GFX_Test GFX_i_SetDeath \
GFX_Render GFX_Tetris_Dummy_Create GFX_i_Add GFX_i_AddSorted GFX_pst_Getv \
GFX_f_Getf GFX_i_Geti GFX_Seti GFX_Setf GFX_Setv GFX_CreateExplosion GFX_Free \
GFX_sguw_Id ga_Paramf_First ga_Paramf_Number ga_Parami_First ga_Parami_Number \
ga_Paramv_First ga_Paramv_Number gaa_Paramf_Offset gaa_Parami_Offset gaa_Paramv_Offset"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

echo "=== gfx ==="
$CC $CFLAGS -c gfx.c -o "$OUT/gfx.new.o" $(defs NEW_ $gfx_syms)
$CC $CFLAGS -c tests/baseline_gfx.c -o "$OUT/gfx.base.o" $(defs BASE_ $gfx_syms)
$CC $CFLAGS -c tests/gfxcore_mocks.c -o "$OUT/gfx.mocks.o"
$CC $CFLAGS -c tests/test_gfx.c -o "$OUT/gfx.drv.o"
$CC -no-pie "$OUT/gfx.new.o" "$OUT/gfx.base.o" "$OUT/gfx.mocks.o" "$OUT/gfx.drv.o" \
    -o "$OUT/gfx" -lm
"$OUT/gfx"
