#!/bin/sh
# The differential test for collision.c functions 1..63 (range A).
#
#   ./tests/run_collision_a.sh
#
# Standalone on purpose: tests/run.sh is folded in after the three
# collision ranges are merged, so this script is the one to run until
# then. Same shape as run.sh's own recipes -- the frozen copy and the
# rewrite in one binary with BASE_/NEW_ prefixes, the real math.c
# linked rather than mocked, and everything else stubbed to abort.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

# Every function collision.c defines, renamed so the two copies can
# live in one binary. The two the pass named carry their old spelling
# on the baseline side, so both ends up as BASE_<new name>.
col_syms="COL_AllocDetectionList COL_FreeDetectionList COL_BV_PivotCollide \
COL_FreeColSet COL_FreeInstance COL_FreeGameMaterial COL_FreeTrianglesIndexedCob \
COL_FreeMathematicalCob COL_FreeColMap COL_Free COL_AllocColMap INT_PointInTriangle \
COL_ComputeProximity COL_ColMap_AddSphere COL_pst_GMat_Get COL_GMat_Skip \
INT_PointSphere INT_SphereSphere INT_FullSphereSphere INT_FullSegmentSphere \
INT_FullSegmentTriangle INT_FullSphereTriangle COL_ReportCollision \
COL_NeedRecomputation COL_b_IsARealTopologicEdge COL_ComputeReport COL_AllocInstance \
COL_DuplicateInstance COL_DuplicateColMap COL_ColMap_AddGeometric COL_ColMap_AddCob \
COL_ResolveRefs COL_Reinit INT_FullRayTriangle INT_FullRayAABBox INT_FullRaySphere \
INT_b_FullRayCylinder COL_ColMap_Triangles_RayCast COL_ColMap_OK3_RayCast \
COL_Visual_OK3_RayCast COL_Visual_Triangles_RayCast COL_ColMap_Box_RayCast \
COL_ColMap_Sphere_RayCast COL_ColMap_Cylinder_RayCast COL_RaySphereBV COL_RayAABBox \
COL_Visual_RayCast COL_RayCompute COL_UpdateOldMatrix COL_InitGlobalVars \
COL_A_BCS_ComputeZDxInfo COL_SetFaceTag COL_b_TestFaceTag INT_PointLineClosestPoint \
INT_PointTriangleClosestPointGet INT_DynamicHit INT_ul_GetPositionPointWithBox \
INT_ul_GetPositionPointWithEdgeBox INT_b_InYZRange INT_b_InXZRange INT_b_InXYRange \
INT_b_IntersectEdgeWithBoxWithHit INT_FullAABBoxTriangle COL_Box_Triangle \
COL_Sphere_Triangle COL_TestAndCollideTriangleIfNeeded INT_SphereAABBox INT_SphereOBBox \
INT_FullSphereOBBox INT_FullSphereCylinder COL_ZDM_Box COL_ZDM_Sphere COL_ZDM_Cylinder \
COL_ComputeRejection COL_ZDM_ZDR COL_Actor_GameObject COL_UpdateDynamicHierarchyLink \
COL_b_SlowActorVsFastActor COL_PreComputeReports COL_ComputeReports COL_OneCall \
COL_ComputeMove COL_DynamicHierarchisation COL_b_MoveReports COL_UpdateBV \
COL_UpdateCobFromVisualRealTime COL_UpdateRealTimeColMaps COL_Compute COL_MainCall \
REC_OrderReports REC_PhysicalCall REC_ApplyDynObj REC_ApplyDyn REC_CalcNormalForImpulse \
REC_CalcImpulse REC_Apply REC_MainCall bvin_sqrlen_sub_412FD0 COL_b_SingleBVIn \
COL_b_BVIn INT_LowLevel_OBBoxOBBox INT_HighLevel_OBBoxOBBox COL_b_SingleBVOverlap \
COL_b_BVOverlap COL_InitModule COL_AllocColSet COL_ul_CallBackLoadColSet \
COL_ul_CallBackLoadInstance COL_ul_CallBackLoadGameMaterial COL_ul_CallBackLoadCob \
COL_ul_CallBackLoadColMap COL_RayBV COL_Report_CollideType COL_Report_GetNormal \
COL_ColMap_RayCast COL_Report_GetCOBGameObject COL_GameMaterial_GetFlags \
COL_GameMaterial_GetSlide COL_GameMaterial_GetRebound COL_GameMaterial_GetSound \
COL_FindEdgeInFollowDirection COL_GetZConnected \
COL_GetPointConnectedTriangleWithMuchHorizontalEdge COL_ComputeJumpPoint \
COL_Report_GetZDMGameObjectList COL_Report_GetBestWallGOInDirection \
COL_Report_GetCollidedPoint COL_Report_GMat_GetCollidedPoint \
COL_Report_GetAllReportsNormal COL_VisibleObjectListCreate COL_Report_GetNumberOfReports \
COL_Report_ComputeResultingNormal COL_Report_GetBestWallGOInDirectionSpecial \
COL_GameMaterial_GetID COL_Report_GetNumberOfReportsAndRanks \
COL_Report_b_ObjectsAreColliding COL_Visual_RayCast_OneObject COL_Report_ul_BestReportGet \
COL_Report_HasGMat COL_GetDesign COL_ZDE_PivotCollide COL_Zone_Transform \
COL_ZDE_BVCollide COL_ZDE_BVCenterCollide COL_ZDE_ZDECollide COL_ListCreate \
COL_GetFirstObjectInRange INT_FullTriangleTriangle COL_TriangleCollision \
COL_SwapToSpecific COL_ListCreateDistance COL_ZDx_SpecificPosSet COL_ZDx_SpecificSizeSet \
COL_ZDx_SizeGet COL_ZDx_PosGet COL_GeoColMap_PosSet COL_Report_ul_GMatReportGet \
col_sqrlen_sub_409560 COL_ul_SnP_RefreshType gcjpUnkFloatflt_968524"

# Named by this pass; the frozen copy still has the address spellings.
renamed_new="COL_w_TestInstanceFlag COL_i_ZoneActivationGet"
renamed_old="COL_test_flag_sub_436970 COL_get_activation_flag_for_id__max_16_sub_4132C0"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

echo "=== collision range A ==="
$CC $CFLAGS -c collision.c -o "$OUT/colA.new.o" \
    $(defs NEW_ $col_syms) \
    -DCOL_w_TestInstanceFlag=NEW_COL_w_TestInstanceFlag \
    -DCOL_i_ZoneActivationGet=NEW_COL_i_ZoneActivationGet
$CC $CFLAGS -c tests/baseline_collision.c -o "$OUT/colA.base.o" \
    $(defs BASE_ $col_syms) \
    -DCOL_test_flag_sub_436970=BASE_COL_w_TestInstanceFlag \
    -DCOL_get_activation_flag_for_id__max_16_sub_4132C0=BASE_COL_i_ZoneActivationGet
$CC $CFLAGS -c math.c -o "$OUT/colA.math.o"
$CC $CFLAGS -c tests/collision_a_mocks.c -o "$OUT/colA.mocks.o"
$CC $CFLAGS -c tests/test_collision_a.c -o "$OUT/colA.drv.o"
$CC "$OUT/colA.new.o" "$OUT/colA.base.o" "$OUT/colA.math.o" \
    "$OUT/colA.mocks.o" "$OUT/colA.drv.o" -o "$OUT/collision_a" -lm
"$OUT/collision_a"
