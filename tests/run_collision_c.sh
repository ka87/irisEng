#!/bin/sh
# Differential test for collision.c functions 98-158 (the last third).
#
#   ./tests/run_collision_c.sh
#
# Standalone on purpose: tests/run.sh is left alone while the three
# ranges of collision.c are being rewritten in parallel, and this gets
# folded into it after the merge.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

# Every function and file-scope object collision.c defines. The engine
# globals it only declares stay unprefixed, so the two copies share
# them exactly as they do in the engine.
collision_syms="COL_A_BCS_ComputeZDxInfo COL_Actor_GameObject COL_AllocColMap \
COL_AllocColSet COL_AllocDetectionList COL_AllocInstance COL_BV_PivotCollide \
COL_Box_Triangle COL_ColMap_AddCob COL_ColMap_AddGeometric COL_ColMap_AddSphere \
COL_ColMap_Box_RayCast COL_ColMap_Cylinder_RayCast COL_ColMap_OK3_RayCast \
COL_ColMap_RayCast COL_ColMap_Sphere_RayCast COL_ColMap_Triangles_RayCast \
COL_Compute COL_ComputeJumpPoint COL_ComputeMove COL_ComputeProximity \
COL_ComputeRejection COL_ComputeReport COL_ComputeReports COL_DuplicateColMap \
COL_DuplicateInstance COL_DynamicHierarchisation COL_FindEdgeInFollowDirection \
COL_Free COL_FreeColMap COL_FreeColSet COL_FreeDetectionList COL_FreeGameMaterial \
COL_FreeInstance COL_FreeMathematicalCob COL_FreeTrianglesIndexedCob COL_GMat_Skip \
COL_GameMaterial_GetFlags COL_GameMaterial_GetID COL_GameMaterial_GetRebound \
COL_GameMaterial_GetSlide COL_GameMaterial_GetSound COL_GeoColMap_PosSet \
COL_GetDesign COL_GetFirstObjectInRange \
COL_GetPointConnectedTriangleWithMuchHorizontalEdge COL_GetZConnected \
COL_InitGlobalVars COL_InitModule COL_ListCreate COL_ListCreateDistance \
COL_MainCall COL_NeedRecomputation COL_OneCall COL_PreComputeReports \
COL_RayAABBox COL_RayBV COL_RayCompute COL_RaySphereBV COL_Reinit \
COL_ReportCollision COL_Report_CollideType COL_Report_ComputeResultingNormal \
COL_Report_GMat_GetCollidedPoint COL_Report_GetAllReportsNormal \
COL_Report_GetBestWallGOInDirection COL_Report_GetBestWallGOInDirectionSpecial \
COL_Report_GetCOBGameObject COL_Report_GetCollidedPoint COL_Report_GetNormal \
COL_Report_GetNumberOfReports COL_Report_GetNumberOfReportsAndRanks \
COL_Report_GetZDMGameObjectList COL_Report_HasGMat COL_Report_b_ObjectsAreColliding \
COL_Report_ul_BestReportGet COL_Report_ul_GMatReportGet COL_ResolveRefs \
COL_SetFaceTag COL_Sphere_Triangle COL_SwapToSpecific \
COL_TestAndCollideTriangleIfNeeded COL_TriangleCollision COL_UpdateBV \
COL_UpdateCobFromVisualRealTime COL_UpdateDynamicHierarchyLink COL_UpdateOldMatrix \
COL_UpdateRealTimeColMaps COL_VisibleObjectListCreate COL_Visual_OK3_RayCast \
COL_Visual_RayCast COL_Visual_RayCast_OneObject COL_Visual_Triangles_RayCast \
COL_ZDE_BVCenterCollide COL_ZDE_BVCollide COL_ZDE_PivotCollide COL_ZDE_ZDECollide \
COL_ZDM_Box COL_ZDM_Cylinder COL_ZDM_Sphere COL_ZDM_ZDR COL_ZDx_PosGet \
COL_ZDx_SizeGet COL_ZDx_SpecificPosSet COL_ZDx_SpecificSizeSet COL_Zone_Transform \
COL_b_BVIn COL_b_BVOverlap COL_b_IsARealTopologicEdge COL_b_MoveReports \
COL_b_SingleBVIn COL_b_SingleBVOverlap COL_b_SlowActorVsFastActor COL_b_TestFaceTag \
COL_f_SqrLen COL_get_activation_flag_for_id__max_16_sub_4132C0 COL_pst_GMat_Get \
COL_test_flag_sub_436970 COL_ul_CallBackLoadCob COL_ul_CallBackLoadColMap \
COL_ul_CallBackLoadColSet COL_ul_CallBackLoadGameMaterial COL_ul_CallBackLoadInstance \
COL_ul_SnP_RefreshType INT_DynamicHit INT_FullAABBoxTriangle INT_FullRayAABBox \
INT_FullRaySphere INT_FullRayTriangle INT_FullSegmentSphere INT_FullSegmentTriangle \
INT_FullSphereCylinder INT_FullSphereOBBox INT_FullSphereSphere INT_FullSphereTriangle \
INT_FullTriangleTriangle INT_HighLevel_OBBoxOBBox INT_LowLevel_OBBoxOBBox \
INT_PointInTriangle INT_PointLineClosestPoint INT_PointSphere \
INT_PointTriangleClosestPointGet INT_SphereAABBox INT_SphereOBBox INT_SphereSphere \
INT_b_FullRayCylinder INT_b_InXYRange INT_b_InXZRange INT_b_InYZRange \
INT_b_IntersectEdgeWithBoxWithHit INT_ul_GetPositionPointWithBox \
INT_ul_GetPositionPointWithEdgeBox REC_Apply REC_ApplyDyn REC_ApplyDynObj \
REC_CalcImpulse REC_CalcNormalForImpulse REC_MainCall REC_OrderReports \
REC_PhysicalCall bvin_sqrlen_sub_412FD0 gcjpUnkFloatflt_968524"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

# shellcheck disable=SC2086
$CC $CFLAGS -c collision.c -o "$OUT/collision_c.new.o" $(defs NEW_ $collision_syms)
# shellcheck disable=SC2086
$CC $CFLAGS -Itests -c tests/baseline_collision.c -o "$OUT/collision_c.base.o" $(defs BASE_ $collision_syms)
$CC $CFLAGS -c math.c -o "$OUT/collision_c.math.o"
$CC $CFLAGS -c tests/collision_c_mocks.c -o "$OUT/collision_c.mocks.o"
$CC $CFLAGS -c tests/math_mocks.c -o "$OUT/collision_c.mathmocks.o"
$CC $CFLAGS -c tests/test_collision_c.c -o "$OUT/collision_c.drv.o"
$CC "$OUT/collision_c.new.o" "$OUT/collision_c.base.o" "$OUT/collision_c.math.o" \
    "$OUT/collision_c.mocks.o" "$OUT/collision_c.mathmocks.o" "$OUT/collision_c.drv.o" \
    -o "$OUT/collision_c" -lm

"$OUT/collision_c"
