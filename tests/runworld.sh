#!/bin/sh
# Differential test for world.c, as a standalone runner.
#
#   ./tests/runworld.sh
#
# It is separate from run.sh only because three other files were being
# rewritten in parallel and run.sh is where that work would collide; the
# body below is the same shape as run.sh's run_* functions and is meant
# to be folded in.
#
# world.c sits at the repo root, so the frozen copy includes its headers
# through "../" for the same reason animation.c's and strings.c's do:
# -I. would put the engine's own strings.h in front of the system one.
# The file's globals are renamed as well as its functions -- world.h
# declares them without extern, so both copies would otherwise share one
# tentative definition and the two runs would not be independent.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

world_syms="WOR_ActivateObjectIfFlagsAllow WOR_World_DetachObject WOR_ResetKillGO \
WOR_DelObj WOR_AddDelObj WOR_MakeObjectVisibleIfFlagsAllow WOR_World_ActivateObjects \
WOR_World_AddLoadedObject WOR_pst_World_Create EOT_SetOfEOT_Init WOR_World_Init \
WOR_Universe_Init WOR_InitModule WOR_World_Close WOR_World_Destroy \
WOR_Universe_DeleteWorld WOR_l_Universe_GetWorldIndex WOR_pst_Universe_GetActiveWorld \
WOR_i_GetAllChildrenOf WOR_GetLastChildObj EOT_RemoveObjectFromTableIfBadFlag \
EOT_UpdateIdentityFlagsOfTable EOT_SetOfEOT_Close EOT_SetOfEOT_Clear \
EOT_AISortWithPriority EOT_SetOfEOT_Build EOT_SetOfEOT_OneBuild WOR_World_ResolveAIRef \
WOR_World_AttachGameObject WOR_World_JustAfterLoadObject WOR_World_CheckFather \
WOR_World_CheckGroup WOR_ul_GameObjectGroupCallback WOR_ul_World_LoadCallback \
WOR_pst_World_Load WOR_BVMaskComputeInside WOR_BVMaskPreCompute WOR_BVMaskPreComputeAll \
ENG_InitOneWorld WOR_pst_Universe_AddWorld WOR_World_MakeObjectsVisible WOR_SetCam \
WOR_Render_All_GO WOR_Universe_Close WOR_Universe_Open WOR_World_Attached WOR_Render \
WOR_Universe_Destroy WOR_CloseModule WOR_pst_Universe_MergeWorld \
WOR_pst_Universe_AddWorldList \
_floatIsNegative WOR_l_SignOf WOR_l_InvertMaskPlanes WOR_l_ResetRasterStats \
zero_out_param1_values_statistics_sub_43EBF0 \
WOR_gi_NumKilledGO WOR_AllKilledGO WOR_gpst_CurrentWorld WOR_gpst_WorldToLoadIn \
WOR_gst_Universe WOR_gb_HasAddedMaterial WOR_gb_FirstTrameAfterLoad \
WOR_gf_LoadStartTime BVMask_gi_Num BVMask_ga_Masks WOR_gi_IsLoading \
WOR_gb_RealAllocWorld WOR_gb_FinalWorld WOR_gul_WorldText WOR_gul_WorldKey"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

# shellcheck disable=SC2086
$CC $CFLAGS -c world.c -o "$OUT/world.new.o" $(defs NEW_ $world_syms)
# shellcheck disable=SC2086
$CC $CFLAGS -c tests/baseline_world.c -o "$OUT/world.base.o" $(defs BASE_ $world_syms)
$CC $CFLAGS -c tables.c -o "$OUT/world.tables.o"
$CC $CFLAGS -c tests/world_mocks.c -o "$OUT/world.mocks.o"
$CC $CFLAGS -c tests/test_world.c -o "$OUT/world.drv.o"
$CC "$OUT/world.new.o" "$OUT/world.base.o" "$OUT/world.tables.o" \
    "$OUT/world.mocks.o" "$OUT/world.drv.o" -o "$OUT/world" -lm

"$OUT/world"
