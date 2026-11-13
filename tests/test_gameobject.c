/* Differential test for gameobject.c: the frozen pre-rewrite copy
   against the rewritten one, over identical state.

   Both copies run over one scene buffer at one address, rebuilt
   identically before each side, with the mock allocator rewound between
   them, so every pointer either copy writes into the scene can be
   compared directly rather than being normalised. A case is equivalent
   when the scene bytes, the used prefix of the mock arena, the return
   values and the ordered sequence of mocked engine calls all match.

   What it is for. The rewrite replaced all 42 gotos -- thirteen of them
   in OBJ_ChangeIdentityFlags, whose 27 "v3 = a3" statements turned out
   to be dead, and seven in OBJ_EyeFollow, whose blend ramp and torso
   target were a maze of shared tails. It also restored 21 inlined
   copies of MATH_CopyMatrix, two of MATH_SetIdentityMatrix, four of
   MATH_NormalizeEqualVector and one of MATH_GetSqrt, folded the
   dword-plus-remainder memcpy in OBJ_GameObject_Duplicate back into one
   call, and collapsed two identical accumulator sets in
   OBJ_Frame_CullingSphere.
   Those are the changes a reading cannot settle.

   What it does not cover, and why: OBJ_ul_GameObjectCallback and
   OBJ_ul_World_LoadGolCallback need the big-file reader and the
   loader's address table, so a test would have had to build a .gao byte
   stream and a working LOA_MakeFileRef resolver -- it would be
   measuring the fixture. The mocks on those paths abort instead of
   returning, so a rewrite that reached one fails the test rather than
   passing on a stand-in. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stddef.h>

#include "../common.h"
#include "../math.h"
#include "../tables.h"
#include "../gameobject.h"
#include "../groups.h"
#include "../modifiers.h"
#include "../dynamics.h"
#include "../collision.h"
#include "../events.h"
#include "../sound.h"
#include "../ways.h"
#include "../world.h"
#include "../cameraengine.h"
#include "../view.h"
#include "../gdi.h"
#include "../graphicsobjects/geo.h"
#include "../graphicsobjects/staticlod.h"
#include "../graphicsobjects/light.h"
#include "../graphicsobjects/gro_common.h"
#include "../time.h"
#include "../loading.h"
#include "gameobject_mocks.h"

#define DECL2(ret, name, args) ret BASE_##name args; ret NEW_##name args;

DECL2(void, OBJ_UpdateStatusFlagRTL, (tGameObjectMain*))
DECL2(int, OBJ_b_HasBeenProcess, (tGameObjectMain*, char, int))
DECL2(void, OBJ_ResetProcess, (tGameObjectMain*, char, char))
DECL2(void, OBJ_SetProcess, (tGameObjectMain*, char, char))
DECL2(BOOL, OBJ_BV_IsSphere, (tGameObjectBV*))
DECL2(BOOL, OBJ_BV_IsAABBox, (tGameObjectBV*))
DECL2(void, OBJ_BV_SetSphereFlag, (tGameObjectBV*))
DECL2(BOOL, OBJ_HasNoBV, (tGameObjectMain*))
DECL2(void, OBJ_BV_InitAABBox, (tGameObjectBV*, Mtx4x4*, Vec3*))
DECL2(void, OBJ_BV_InitSphereBV, (tGameObjectBV*, Vec3*, float, Vec3*))
DECL2(void, OBJ_BV_AddPointToSphereBV, (tGameObjectBV*, Vec3*, Vec3*))
DECL2(void, OBJ_BV_AddPointToAABBox, (tGameObjectBV*, Mtx4x4*, Vec3*, int))
DECL2(void, OBJ_ComputeCameraBV, (tGameObjectMain*, int))
DECL2(void, OBJ_ComputeWaypointBV, (tGameObjectMain*, int))
DECL2(void, OBJ_ComputeLightBV, (tGameObjectMain*))
DECL2(void, OBJ_ComputeAABBoxFromOBBox, (tGameObjectMain*))
DECL2(void, OBJ_DuplicateBV, (tGameObjectMain*, tGameObjectMain*))
DECL2(void, OBJ_BV_ComputeCenter, (tGameObjectMain*, Vec3*))
DECL2(void, OBJ_BV_CalcBVForTwoOBJs, (tGameObjectMain*, tGameObjectMain*))
DECL2(void, OBJ_MergeBVs, (tGameObjectMain*, tGameObjectMain*))
DECL2(int, GEO_MRM_GetRealNumberOfPoints, (tGEOGRO*))
DECL2(void, OBJ_ComputeGeometricObjectBV, (tGameObjectMain*, int))
DECL2(void, OBJ_ComputeStaticLODObjectBV, (tGameObjectMain*, int))
DECL2(void, OBJ_ComputeAnimBV, (tGameObjectMain*))
DECL2(void, OBJ_ComputeBV, (tGameObjectMain*, int))
DECL2(tGaoMatrix*, OBJ_pst_GetLocalMatrix, (tGameObjectMain*))
DECL2(void, OBJ_ComputeGlobalWhenHie, (tGameObjectMain*))
DECL2(void, OBJ_ComputeRecursivScale, (tGameObjectMain*, Vec3*))
DECL2(void, OBJ_SetAbsoluteMatrix, (tGameObjectMain*, Mtx4x4*))
DECL2(void, OBJ_MakeAbsoluteMatrix, (tGameObjectMain*, Mtx4x4*))
DECL2(void, OBJ_SightGet, (tGameObjectMain*, Vec3*))
DECL2(void, OBJ_SightSet, (tGameObjectMain*, Vec3*))
DECL2(void, OBJ_BankingSet, (tGameObjectMain*, Vec3*))
DECL2(void, OBJ_SightGeneralSet, (tGameObjectMain*, Vec3*, Vec3*))
DECL2(void, OBJ_BankingGeneralSet, (tGameObjectMain*, Vec3*, Vec3*))
DECL2(void, OBJ_AddInGroup, (tGroup*, tGameObjectMain*))
DECL2(void, OBJ_GameObject_CreateExtendedIfNot, (tGameObjectMain*))
DECL2(void, OBJ_FreeGroupGrl, (tableDescStruct*))
DECL2(void, OBJ_FreeGroup, (tGroup*))
DECL2(void, OBJ_SightInitGet, (tGameObjectMain*, Vec3*))
DECL2(void, OBJ_BankingGet, (tGameObjectMain*, Vec3*))
DECL2(void, OBJ_HorizonGet, (tGameObjectMain*, Vec3*))
DECL2(void, OBJ_SetLocalPosition, (tGameObjectMain*, Vec3*))
DECL2(void, OBJ_SetAbsolutePosition, (tGameObjectMain*, Vec3*))
DECL2(void, OBJ_SetInitialAbsolutePosition, (tGameObjectMain*, Vec3*))
DECL2(void, OBJ_SetInitialAbsoluteMatrix, (tGameObjectMain*, Mtx4x4*))
DECL2(void, OBJ_ComputeGlobalWithLocal, (tGameObjectMain*, Mtx4x4*, int))
DECL2(void, OBJ_ComputeLocalWhenHie, (tGameObjectMain*))
DECL2(void, OBJMY_ComputeLocalWhenHie, (tGameObjectMain*))
DECL2(Vec3*, OBJ_pst_GetPos, (tGameObjectMain*))
DECL2(tDyn*, OBJ_pst_GetDynaStruct, (tGameObjectMain*))
DECL2(int, OBJ_b_test_flag, (tGameObjectMain*, int, int, int))
DECL2(void, OBJ_putInGroup, (tGameObjectMain*, tGroup*))
DECL2(tGameObjectMain*, OBJ_pst_GroupGetByRank, (tGroup*, int))
DECL2(BOOL, OBJ_b_IsChild, (tGameObjectMain*))
DECL2(tGameObjectMain*, OBJ_GameObject_Create, (int))
DECL2(void, OBJ_ReinitMatrixStorageByFlags, (tGameObjectMain*, Mtx4x4*))
DECL2(void, OBJ_RestoreInitialPos, (tGameObjectMain*))
DECL2(void, OBJ_Reinit, (tGameObjectMain*))
DECL2(void, OBJ_CheckFlagsAndActivateObject, (tGameObjectMain*, int))
DECL2(void, OBJ_GameObject_Remove, (tGameObjectMain*, BOOL))
DECL2(Mtx4x4*, OBJ_pst_GetFlashMatrix, (tGameObjectMain*))
DECL2(void, OBJ_ChangeIdentityFlags, (tGameObjectMain*, unsigned int, int))
DECL2(void, OBJ_GameObject_AllocateGizmo, (tGameObjectMain*, int, int))
DECL2(void, OBJ_ResolveAIRef, (tGameObjectMain*))
DECL2(tGameObjectMain*, OBJ_GameObject_Duplicate, (tWorld*, tGameObjectMain*, int, int, Vec3*))
DECL2(void, OBJ_Gizmo_UpdateGizmoPtr, (tGameObjectMain*))
DECL2(void, OBJ_ScaleSet, (tGameObjectMain*, Vec3*))
DECL2(void, OBJ_ZoomSet, (tGameObjectMain*, float))
DECL2(tGameObjectMain*, get_reference_point_gao_2_from_gao_sub_40BEA0, (tGameObjectMain*))
DECL2(void, OBJ_EyeFollow, (tGameObjectMain*, Vec3*, float, int, int, int, int, int, int, int))
DECL2(void, OBJ_FreezeModifier_C, (tGameObjectMain*, int, int))
DECL2(BOOL, OBJ_ModifierType_C, (tGameObjectMain*, int))
DECL2(tGEOGRO*, OBJ_p_GetCurrentGeo, (tGameObjectMain*))
DECL2(int, OBJ_Frame_CullingSphere, (Vec3*, float, tDDCamStruct*, int, int, float*, float*))
DECL2(int, OBJ_Frame_CullingAABBox, (Vec3*, Vec3*, tDDCamStruct*, int, int, float*, float*))
DECL2(void, OBJ_UpdateCullingVars, (tDDCamStruct*))
DECL2(int, OBJ_CullingAABBox, (Vec3**, uint8_t*))
DECL2(BOOL, OBJ_CullingSphere, (Vec3*, float))
DECL2(BOOL, OBJ_CullingObject, (tGameObjectMain*))
DECL2(BOOL, OBJ_CullingPointInside, (Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, char))
DECL2(void, OBJ_HierarchyMainCall, (tWorld*))
DECL2(tGroGeneric*, OBJ_p_GetGro, (tGameObjectMain*))
DECL2(tGEOMAT*, OBJ_p_GetGrm, (tGameObjectMain*))
DECL2(char, OBJ_uc_CalcLODVis, (tGameObjectMain*, tViewElem*))

extern tGameObjectMain* GMOCK_canal[16];
extern tGEOGRO* GMOCK_lodPick;

/* ------------------------------------------------------------ scene */

#define NGAO 6

typedef struct
{
    tGameObjectMain gao[NGAO];
    tGameObjectBasic basic[NGAO];
    tGaoMatrix gaomtx[NGAO];
    tGameObjectBV bv[NGAO];
    tGameObjectExtended ext[NGAO];
    tGameObjectCollMain coll[NGAO];
    tGameObjectDesignFields fields[NGAO];
    tGameObjectVisualObject visual[NGAO];
    tGameObjectActorObject actor[NGAO];
    tGameObjectMultiPartData mp[NGAO];
    Mtx4x4 matrices[NGAO][3];

    tGroup group[2];
    tableDescStruct table[2];
    tableElemStructPF elems[2][NGAO];
    tableElemStructPF helems[NGAO];

    tGroIntf intf[12];
    tGroGeneric gro[12];
    tGEOGRO geo[2];
    tGEOSTATICLOD lod;
    Vec3 verts[12];
    tLIGHT light;

    tColMap colmap;
    tColCollisionObject colobj[2];
    tColCollisionObject* colobjp[2];
    tCobMeshObjectMesh mesh;
    Vec3 meshverts[5];

    tMultiPartGameObjectLinks links[4];
    Mtx4x4 partmtx[4];

    tDDCamStruct cam;
    tViewElem view;
    tGdi_DisplayData display;
    tAnim anim[2];
    tEventData eve;
    tActionKit* kitBuf;
    unsigned char kitStore[256];
    unsigned char actionStore[256];
    tModifier mdf;
    tWorld world;

    Vec3 scratch[8];
    Mtx4x4 mscratch[4];
    float fscratch[8];
} Scene;

static Scene g_scene;
static unsigned int g_rng;

static float frand(void)
{
    g_rng = g_rng * 1103515245u + 12345u;
    return (float)((int)((g_rng >> 9) & 0xFFFF) - 32768) / 8192.0f;
}

static int irand(int n)
{
    g_rng = g_rng * 1103515245u + 12345u;
    return (int)((g_rng >> 13) % (unsigned)n);
}

/* The graphics-object interface the fixture hands out: three entry
   points that log and do nothing, so a call that moved shows up in the
   ordered comparison. */
static void gro_AddRef(tGroGeneric* g, int d) { GMOCK_Log("gro.AddRef %d", d); (void)g; }
static void gro_Destroy(tGroGeneric* g) { GMOCK_Log("gro.Destroy"); (void)g; }
static void gro_Reinit(tGroGeneric* g) { GMOCK_Log("gro.Reinit"); (void)g; }

static void mtx_random(Mtx4x4* m)
{
    m->v1.x = frand(); m->v1.y = frand(); m->v1.z = frand(); m->_m14 = 0.0f;
    m->v2.x = frand(); m->v2.y = frand(); m->v2.z = frand(); m->_m24 = 0.0f;
    m->v3.x = frand(); m->v3.y = frand(); m->v3.z = frand(); m->_m34 = 0.0f;
    m->Pos.x = frand(); m->Pos.y = frand(); m->Pos.z = frand(); m->w = 1.0f;
    m->mtxflag = (unsigned)irand(16);
    m->unk1 = 0; m->unk2 = 0; m->unk3 = 0;
}

/* Builds the whole scene deterministically from one seed. Called once
   before each side, so both sides see byte-identical state. */
static void scene_build(unsigned int seed)
{
    Scene* s = &g_scene;
    int i;
    int j;

    memset(s, 0, sizeof(*s));
    g_rng = seed * 2654435761u + 1u;

    for (i = 0; i < 12; ++i)
    {
        s->intf[i].id = i + 1;
        s->intf[i].AddRef = gro_AddRef;
        s->intf[i].Destroy = gro_Destroy;
        s->intf[i].Reinit = gro_Reinit;
        s->gro[i].intf = &s->intf[i];
        s->gro[i].usageCount = 1;
    }
    for (i = 0; i < 12; ++i)
    {
        s->verts[i].x = frand();
        s->verts[i].y = frand();
        s->verts[i].z = frand();
    }
    for (i = 0; i < 5; ++i)
    {
        s->meshverts[i].x = frand();
        s->meshverts[i].y = frand();
        s->meshverts[i].z = frand();
    }

    s->geo[0].intf = &s->intf[0];        /* id 1, a plain geometry */
    s->geo[0].vertexNum = 8;
    s->geo[0].vertices = s->verts;
    s->geo[0].mrm = 0;
    s->geo[1].intf = &s->intf[0];
    s->geo[1].vertexNum = 4;
    s->geo[1].vertices = s->verts + 4;
    s->geo[1].mrm = 0;
    s->lod.intf = &s->intf[7];           /* id 8, static LOD */
    s->lod.lodNum = 2;
    s->lod.currDistance = 1;
    s->lod.geo[0] = &s->geo[1];
    s->lod.geo[1] = &s->geo[0];
    GMOCK_lodPick = &s->geo[0];

    s->light.intf = &s->intf[1];
    s->light.flags = (int)(seed & 7);
    s->light.ffar = 3.5f + (float)(seed % 5);

    s->mesh.vertexCount = 5;
    s->mesh.vertices = s->meshverts;
    s->colobj[0].objectType = 5;
    s->colobj[0].objectPtrByType = &s->mesh;
    s->colobj[1].objectType = 3;
    s->colobj[1].objectPtrByType = 0;
    s->colobjp[0] = &s->colobj[0];
    s->colobjp[1] = &s->colobj[1];
    s->colmap.colObjectID = 2;
    s->colmap.collisionObjectPtr = s->colobjp;

    for (i = 0; i < NGAO; ++i)
    {
        tGameObjectMain* g = &s->gao[i];

        for (j = 0; j < 3; ++j)
            mtx_random(&s->matrices[i][j]);
        g->matrices = s->matrices[i];
        g->boundingVolume = &s->bv[i];
        g->basic = &s->basic[i];
        g->extended = &s->ext[i];
        g->flags = 0x1000 | 0x2000 | 0x4000 | 0x400000 | 0x10000 | 0x80000 | 0x40;
        g->flags2 = (int)(0x1000000u * (unsigned)(i & 3));
        g->process = (char)(i * 37);
        g->designFlags = (uint8_t)(i * 51 + 3);
        g->u3 = 16;
        g->distance = (uint8_t)(i * 29);
        g->LODDist = (uint8_t)(i * 13);
        g->snpIndex = (uint16_t)i;

        s->basic[i].mtx = &s->gaomtx[i];
        s->basic[i].actorObjectOrVisualObject = &s->visual[i];
        s->basic[i].multiPartData = &s->mp[i];
        s->basic[i].dyn = 0;
        mtx_random(&s->gaomtx[i].mtx);
        s->gaomtx[i].refGao = 0;
        s->gaomtx[i].refGao2 = 0;

        s->bv[i].BVMin.x = frand(); s->bv[i].BVMin.y = frand(); s->bv[i].BVMin.z = frand();
        s->bv[i].BVMax.x = 1.0f + fabsf(frand());
        s->bv[i].BVMax.y = 1.0f + fabsf(frand());
        s->bv[i].BVMax.z = 1.0f + fabsf(frand());
        s->bv[i].BVMinLocal.x = -1.0f - fabsf(frand());
        s->bv[i].BVMinLocal.y = -1.0f - fabsf(frand());
        s->bv[i].BVMinLocal.z = -1.0f - fabsf(frand());
        s->bv[i].BVMaxLocal.x = 1.0f + fabsf(frand());
        s->bv[i].BVMaxLocal.y = 1.0f + fabsf(frand());
        s->bv[i].BVMaxLocal.z = 1.0f + fabsf(frand());

        s->ext[i].coll = &s->coll[i];
        s->ext[i].fields = &s->fields[i];
        s->ext[i].capa = (uint16_t)(i * 4369 + 1);
        s->ext[i].capaBackup = (uint16_t)(i * 7 + 2);
        s->ext[i].AIPriority = (uint8_t)(100 + i);
        s->ext[i].extraFlag = (int16_t)(i * 11);
        s->ext[i].light = 0;
        s->ext[i].mdf = 0;
        s->ext[i].grp = &s->group[0];
        s->coll[i].colMap = &s->colmap;
        s->fields[i].designFlags = i * 0x1111 + 5;
        s->fields[i].designDesFlags = i * 0x2222 + 9;

        s->visual[i].GROGEOOrSomethingElse = (tGroGeneric*)&s->geo[0];
        s->visual[i].GROMAT = &s->gro[3];
        s->visual[i].objDrawMask = 0x12340000 + i;

        s->mp[i].elemNum = 0;
        s->mp[i].multipartGaoObjectPartsOrMatrices = 0;
    }

    /* a three-deep hierarchy, gao[2] -> gao[1] -> gao[0] -> gao[5],
       rooted at an object that is not itself a child. Every object the
       eye-follow tracks needs a parent: the engine dereferences the one
       it gets back without a null test. */
    s->gaomtx[2].refGao2 = &s->gao[1];
    s->gaomtx[2].refGao = &s->gao[1];
    s->gaomtx[1].refGao2 = &s->gao[0];
    s->gaomtx[1].refGao = &s->gao[0];
    s->gaomtx[0].refGao2 = &s->gao[5];
    s->gaomtx[0].refGao = &s->gao[5];
    s->gao[5].flags &= ~0x400000;

    /* the actor side: gao[3] is an animated object with a skeleton of
       gao[0..2] and an action kit */
    s->gao[3].flags = (s->gao[3].flags & ~0x4000) | 2;
    s->basic[3].actorObjectOrVisualObject = &s->actor[3];
    s->actor[3].skeletonGroup = &s->group[0];
    s->actor[3].group = &s->group[0];
    s->actor[3].actionItem = 1;
    s->actor[3].flagsEgBlendIw2 = (uint8_t)(seed & 1);
    s->actor[3].unkb = (uint8_t)(seed % 17);
    s->actor[3].animPtr[0] = &s->anim[0];
    s->actor[3].animPtr[1] = &s->anim[1];
    s->anim[0].ptr = &s->eve;
    s->anim[1].actionForTransition = 0;
    s->anim[1].actionElemForTransition = (uint8_t)(seed & 1);

    for (i = 0; i < 2; ++i)
    {
        s->table[i].firstElemPtr = s->elems[i];
        s->table[i].nextElemPtr = &s->elems[i][3];
        s->table[i].lastElemPtr = &s->elems[i][NGAO];
        s->table[i].actElemNum = 3;
        for (j = 0; j < NGAO; ++j)
        {
            s->elems[i][j].objectPtr = &s->gao[j];
            s->elems[i][j].objectFlags = s->gao[j].flags;
        }
        s->group[i].tablePF = &s->table[i];
        s->group[i].groupElemNum = 3;
    }

    /* the action kit, laid out the way the engine does: the kit header
       is followed by its action pointers, and each action header by its
       elements */
    {
        tActionKit* kit = (tActionKit*)s->kitStore;
        tAction** actions = (tAction**)(s->kitStore + sizeof(tActionKit));
        tAction* act = (tAction*)s->actionStore;
        tActionElem* elems = (tActionElem*)(s->actionStore + sizeof(tAction));

        kit->elemNum = 2;
        kit->usageCount = 1;
        kit->flags = 0;
        actions[0] = act;
        actions[1] = act;
        act->actionElemNum = 3;
        act->actionElemIDForTransition = 0;
        act->usageCount = 1;
        for (i = 0; i < 3; ++i)
        {
            elems[i].customBits = (uint8_t)((seed >> i) & 4);
            elems[i].designFlags = (int16_t)((seed & 2) ? -1 : (int)(seed & 0x1000));
            elems[i].eventLists = (i == 1) ? (tGameObjectEventsList*)&s->eve : 0;
        }
        s->actor[3].actionKit = kit;
        s->actor[3].actionPtr = act;
        s->kitBuf = kit;
    }

    /* the camera and the view */
    mtx_random(&s->cam.objectMtx);
    s->cam.objectMtx.Pos.x = -1.0f;
    s->cam.objectMtx.Pos.y = -2.0f;
    s->cam.objectMtx.Pos.z = -8.0f;
    mtx_random(&s->cam.cameraMtx);
    s->cam.centerX = 320.0f;
    s->cam.centerY = 240.0f;
    s->cam.factorX = 300.0f;
    s->cam.factorY = 300.0f;
    s->cam.isoFactor = 1.5f;
    s->cam.isoZoom = 2.5f;
    s->cam.flags = (int)(seed & 1);
    s->cam.planeUp.x = frand(); s->cam.planeUp.y = 1.0f; s->cam.planeUp.z = frand();
    s->cam.planeDown.x = frand(); s->cam.planeDown.y = -1.0f; s->cam.planeDown.z = frand();
    s->cam.planeLeft.x = -1.0f; s->cam.planeLeft.y = frand(); s->cam.planeLeft.z = frand();
    s->cam.planeRight.x = 1.0f; s->cam.planeRight.y = frand(); s->cam.planeRight.z = frand();

    mtx_random(&s->view.cameraMtx);
    s->view.displayData = &s->display;
    s->display.ct.cam.factorX = 300.0f;
    s->display.NormalizedWidthOrHeightMultBy255 = 255.0f / 640.0f;

    /* the world's hierarchy table, children before parents: in
       parent-first order the pass never has to climb, so the step back
       over the cursor is never taken */
    for (j = 0; j < NGAO; ++j)
    {
        s->helems[j].objectPtr = &s->gao[NGAO - 1 - j];
        s->helems[j].objectFlags = s->gao[NGAO - 1 - j].flags;
    }
    s->world.eot.objectsWithHiearchyTablePF.firstElemPtr = s->helems;
    s->world.eot.objectsWithHiearchyTablePF.nextElemPtr = &s->helems[NGAO];
    s->world.hierarchyCallCount = (int)(seed & 1);
    s->world.dynCallCount = (int)((seed >> 1) & 1);
    s->world.snp = 0;

    for (i = 0; i < 8; ++i)
    {
        s->scratch[i].x = frand();
        s->scratch[i].y = frand();
        s->scratch[i].z = frand();
    }
    for (i = 0; i < 4; ++i)
        mtx_random(&s->mscratch[i]);

    WOR_gpst_CurrentWorld = &s->world;
    TIM_gf_dt = 0.0166666f;
    OBJ_gf_EYEFOLLOW_HeadZMax = 1000.0f;
    OBJ_gf_EYEFOLLOW_HeadZMin = 1000.0f;
    OBJ_gf_EYEFOLLOW_HeadXMax = 1000.0f;
    OBJ_gf_EYEFOLLOW_HeadXMin = 1000.0f;
    OBJ_gf_EYEFOLLOW_TorsoZMax = 1000.0f;
    OBJ_gf_EYEFOLLOW_TorsoZMin = 1000.0f;
    OBJ_gf_EYEFOLLOW_HeadXOffset = 1000.0f;
    GMOCK_canal[0] = &s->gao[0];
    GMOCK_canal[1] = &s->gao[1];
    GMOCK_canal[12] = &s->gao[2];
    LOA_ul_FileTypeSize_37 = 0;
    LOA_ul_FileTypeSize_29 = 0;
    LOA_ul_FileTypeSize_27 = 0;
    LOA_ul_FileTypeSize_34 = 0;
}

/* --------------------------------------------------- the comparison */

#define PLANE_FLOAT_DIFFS 14

static int g_cases;
static int g_bad;
static unsigned long g_fLo;      /* a scene window compared as floats */
static unsigned long g_fHi;
static int g_fDiffs;             /* how many of those floats differed */
static double g_fWorstRel;
static double g_fWorstAbs;
static Scene g_snap[2];
static long g_ret[2];
static double g_fret[2];

static void side_begin(unsigned int seed)
{
    GMOCK_Reset();
    scene_build(seed);
}

/* MATH_MulMatrixMatrix writes a matrix's 4x4 and its mtxflag and
   leaves unk1..unk3 alone, and four sites here copy the whole 80 bytes
   of a stack local it has just filled -- so three words of whatever was
   on the stack travel into the matrix. Both copies do it; only the
   garbage differs, because their frames differ. Zeroing those words
   before the snapshot keeps every byte that means something compared. */
static void scene_scrub(void)
{
    Mtx4x4* m[] = {
        &g_scene.matrices[0][0], &g_scene.gaomtx[0].mtx, &g_scene.partmtx[0],
        &g_scene.mscratch[0], &g_scene.cam.objectMtx, &g_scene.cam.cameraMtx,
        &g_scene.cam.projectionMtx, &g_scene.view.cameraMtx,
        &g_scene.view.prevViewPointMtx
    };
    int n[] = { NGAO * 3, 0, 4, 4, 1, 1, 1, 1, 1 };
    int i;
    int j;

    for (i = 0; i < (int)(sizeof(n) / sizeof(n[0])); ++i)
        for (j = 0; j < n[i]; ++j)
            m[i][j].unk1 = m[i][j].unk2 = m[i][j].unk3 = 0;
    for (j = 0; j < NGAO; ++j)
        g_scene.gaomtx[j].mtx.unk1 = g_scene.gaomtx[j].mtx.unk2 =
            g_scene.gaomtx[j].mtx.unk3 = 0;
}

static void side_end(int side)
{
    scene_scrub();
    memcpy(&g_snap[side], &g_scene, sizeof(Scene));
    GMOCK_SnapshotLog(side);
    GMOCK_SnapshotHeap(side);
}

static void check(const char* what)
{
    int bad = 0;
    unsigned long i;
    const unsigned char* a = (const unsigned char*)&g_snap[0];
    const unsigned char* b = (const unsigned char*)&g_snap[1];

    ++g_cases;
    for (i = 0; i < sizeof(Scene); ++i)
    {
        if (i >= g_fLo && i < g_fHi)
            continue;
        if (a[i] != b[i])
        {
            printf("  %s: scene byte %lu differs (%02x vs %02x)\n", what, i, a[i], b[i]);
            bad = 1;
            break;
        }
    }
    for (i = g_fLo; i + 4 <= g_fHi; i += 4)
    {
        float fa;
        float fb;
        double rel;
        double abs_;

        memcpy(&fa, a + i, 4);
        memcpy(&fb, b + i, 4);
        if (memcmp(&fa, &fb, 4) == 0)
            continue;
        abs_ = fabs((double)fa - (double)fb);
        rel = fabs((double)fb) > 1e-30 ? abs_ / fabs((double)fb) : abs_;
        ++g_fDiffs;
        if (rel > g_fWorstRel)
            g_fWorstRel = rel;
        if (abs_ > g_fWorstAbs)
            g_fWorstAbs = abs_;
        if (rel > 1e-5 && abs_ > 1e-6)
        {
            printf("  %s: float at %lu differs beyond tolerance: %.9g vs %.9g\n",
                   what, i, (double)fa, (double)fb);
            bad = 1;
        }
    }
    if (g_ret[0] != g_ret[1])
    {
        printf("  %s: return %ld vs %ld\n", what, g_ret[0], g_ret[1]);
        bad = 1;
    }
    if (memcmp(&g_fret[0], &g_fret[1], sizeof(double)))
    {
        printf("  %s: float return %.17g vs %.17g\n", what, g_fret[0], g_fret[1]);
        bad = 1;
    }
    bad |= GMOCK_CompareLogs(what);
    bad |= GMOCK_CompareHeaps(what);
    if (bad)
        ++g_bad;
}

#define CASE(sd, what, body)                   \
    do {                                         \
        int SIDE;                                \
        unsigned int SEED_ = (unsigned int)(sd); \
        g_ret[0] = g_ret[1] = 0;                 \
        g_fret[0] = g_fret[1] = 0;               \
        for (SIDE = 0; SIDE < 2; ++SIDE) {       \
            side_begin(SEED_);                   \
            { body }                             \
            side_end(SIDE);                      \
        }                                        \
        check(what);                             \
    } while (0)

#define PICK(f) (SIDE ? NEW_##f : BASE_##f)

/* ------------------------------------------------------------ cases */

static void t_flags(void)
{
    static const int f1[] = { 0, 2, 0x4000, 0x4002, 0x20, 4, 0x26, 0x4020 };
    static const int f2[] = { 0, 8, 0x40000, 0x80000, 0xC0000, 0x40008, 0x80008 };
    unsigned int seed = 1;
    int i, j;

    for (i = 0; i < (int)(sizeof(f1) / sizeof(f1[0])); ++i)
        for (j = 0; j < (int)(sizeof(f2) / sizeof(f2[0])); ++j)
            CASE(seed++, "UpdateStatusFlagRTL", {
                g_scene.gao[0].flags = f1[i];
                g_scene.gao[0].flags2 = f2[j];
                PICK(OBJ_UpdateStatusFlagRTL)(&g_scene.gao[0]);
            });

    for (i = 0; i < 4; ++i)
        for (j = 0; j < 8; ++j)
            CASE(seed++, "process", {
                char cc = (char)i;
                char mk = (char)(1 << j);
                g_ret[SIDE] = PICK(OBJ_b_HasBeenProcess)(&g_scene.gao[0], cc, mk);
                PICK(OBJ_ResetProcess)(&g_scene.gao[1], cc, mk);
                PICK(OBJ_SetProcess)(&g_scene.gao[2], cc, mk);
            });
}

static void t_test_flag(void)
{
    static const int have[] = { -1, 0, 1, 5, 0x1111, 0x4369, 0xFF, 0x2222 };
    static const int nothave[] = { 0, 2, 0x10, 0x1111, 0xFF };
    unsigned int seed = 100;
    int w, i, j;

    for (w = 0; w <= 16; ++w)
        for (i = 0; i < (int)(sizeof(have) / sizeof(have[0])); ++i)
            for (j = 0; j < (int)(sizeof(nothave) / sizeof(nothave[0])); ++j)
                CASE(seed++, "b_test_flag", {
                    int k;
                    long acc = 0;
                    for (k = 0; k < NGAO; ++k)
                        acc = acc * 3 + PICK(OBJ_b_test_flag)(&g_scene.gao[k], have[i], nothave[j], w);
                    /* and again with the design/capa fields zeroed, which
                       is the only way the "== -1" arms fire */
                    for (k = 0; k < NGAO; ++k)
                    {
                        g_scene.gao[k].designFlags = 0;
                        g_scene.ext[k].capa = 0;
                        g_scene.fields[k].designFlags = 0;
                        g_scene.fields[k].designDesFlags = 0;
                        acc = acc * 3 + PICK(OBJ_b_test_flag)(&g_scene.gao[k], have[i], nothave[j], w);
                    }
                    g_ret[SIDE] = acc;
                });
}

static void t_bv_basics(void)
{
    unsigned int seed = 1000;
    int i;

    for (i = 0; i < 40; ++i)
        CASE(seed++, "bv basics", {
            long acc = 0;
            int k;
            if (i & 1)
                PICK(OBJ_BV_SetSphereFlag)(&g_scene.bv[0]);
            if (i & 2)
            {
                g_scene.bv[1].BVMax.x = 0.0f;
                g_scene.bv[1].BVMax.y = 0.0f;
                g_scene.bv[1].BVMax.z = 0.0f;
            }
            if (i & 4)
                PICK(OBJ_BV_SetSphereFlag)(&g_scene.bv[1]);
            if (i & 8)
                g_scene.bv[2].BVMax.x = 0.0f;
            for (k = 0; k < NGAO; ++k)
            {
                acc = acc * 5 + PICK(OBJ_BV_IsSphere)(&g_scene.bv[k]);
                acc = acc * 5 + PICK(OBJ_BV_IsAABBox)(&g_scene.bv[k]);
                acc = acc * 5 + PICK(OBJ_HasNoBV)(&g_scene.gao[k]);
            }
            g_ret[SIDE] = acc;
        });

    for (i = 0; i < 60; ++i)
        CASE(seed++, "bv build", {
            Vec3* sc = (i & 1) ? &g_scene.scratch[2] : 0;
            PICK(OBJ_BV_InitAABBox)(&g_scene.bv[0], &g_scene.mscratch[0], &g_scene.scratch[0]);
            PICK(OBJ_BV_AddPointToAABBox)(&g_scene.bv[0], &g_scene.mscratch[1], &g_scene.scratch[1], i & 2);
            PICK(OBJ_BV_AddPointToAABBox)(&g_scene.bv[0], &g_scene.mscratch[1], &g_scene.scratch[3], (i & 4) >> 2);
            PICK(OBJ_BV_InitSphereBV)(&g_scene.bv[1], &g_scene.scratch[0], (float)(i % 3), sc);
            PICK(OBJ_BV_AddPointToSphereBV)(&g_scene.bv[1], &g_scene.scratch[1], sc);
            PICK(OBJ_BV_AddPointToSphereBV)(&g_scene.bv[1], &g_scene.scratch[3], sc);
            PICK(OBJ_BV_AddPointToSphereBV)(&g_scene.bv[1], &g_scene.scratch[4], sc);
        });

    for (i = 0; i < 40; ++i)
        CASE(seed++, "bv compute", {
            g_scene.gao[0].flags = (i & 1) ? (g_scene.gao[0].flags | 0x80000)
                                           : (g_scene.gao[0].flags & ~0x80000);
            PICK(OBJ_ComputeCameraBV)(&g_scene.gao[0], i & 2);
            PICK(OBJ_ComputeWaypointBV)(&g_scene.gao[1], i & 2);
            g_scene.ext[2].light = (tGroGeneric*)&g_scene.light;
            g_scene.light.flags = i & 7;
            PICK(OBJ_ComputeLightBV)(&g_scene.gao[2]);
            PICK(OBJ_ComputeAABBoxFromOBBox)(&g_scene.gao[3]);
            PICK(OBJ_DuplicateBV)(&g_scene.gao[3], &g_scene.gao[4]);
            PICK(OBJ_BV_ComputeCenter)(&g_scene.gao[4], &g_scene.scratch[0]);
            PICK(OBJ_BV_CalcBVForTwoOBJs)(&g_scene.gao[4], &g_scene.gao[5]);
            PICK(OBJ_MergeBVs)(&g_scene.gao[5], &g_scene.gao[3]);
        });

    /* the one pair of values for which the merge's six clamps can tell
       `>=` from `>`: a negative zero reaching a positive zero. For every
       other pair the two spellings store the same bits, because the
       clamp assigns the value it just compared equal to. */
    for (i = 0; i < 8; ++i)
        CASE(seed++, "bv merge signed zero", {
            int k;
            for (k = 0; k < NGAO; ++k)
            {
                g_scene.matrices[k][0].Pos.x = (k & 1) ? -0.0f : 0.0f;
                g_scene.matrices[k][0].Pos.y = (k & 1) ? -0.0f : 0.0f;
                g_scene.matrices[k][0].Pos.z = (k & 1) ? -0.0f : 0.0f;
                g_scene.bv[k].BVMin.x = (k & 1) ? -0.0f : 0.0f;
                g_scene.bv[k].BVMin.y = (k & 1) ? -0.0f : 0.0f;
                g_scene.bv[k].BVMin.z = (k & 1) ? -0.0f : 0.0f;
                g_scene.bv[k].BVMax.x = (k & 1) ? 0.0f : -0.0f;
                g_scene.bv[k].BVMax.y = (k & 1) ? 0.0f : -0.0f;
                g_scene.bv[k].BVMax.z = (k & 1) ? 0.0f : -0.0f;
            }
            PICK(OBJ_MergeBVs)(&g_scene.gao[i % 3], &g_scene.gao[(i + 1) % 3]);
            PICK(OBJ_BV_CalcBVForTwoOBJs)(&g_scene.gao[(i + 2) % 3], &g_scene.gao[i % 3]);
        });

    /* the merge path with equal and with inverted bounds, which the
       random fixture never produces */
    for (i = 0; i < 16; ++i)
        CASE(seed++, "bv merge corners", {
            g_scene.bv[0] = g_scene.bv[1];
            g_scene.matrices[0][0].Pos = g_scene.matrices[1][0].Pos;
            if (i & 1)
                PICK(OBJ_BV_SetSphereFlag)(&g_scene.bv[0]);
            if (i & 2)
                PICK(OBJ_BV_SetSphereFlag)(&g_scene.bv[1]);
            if (i & 4)
            {
                g_scene.bv[1].BVMin = g_scene.bv[1].BVMax;
            }
            PICK(OBJ_MergeBVs)(&g_scene.gao[0], &g_scene.gao[1]);
            PICK(OBJ_MergeBVs)(&g_scene.gao[1], &g_scene.gao[0]);
        });
}

static void t_bv_dispatch(void)
{
    unsigned int seed = 2000;
    int i;

    for (i = 0; i < 48; ++i)
        CASE(seed++, "geometric bv", {
            int asSphere = i & 1;
            g_scene.gao[0].flags = 0x1000 | 0x2000 | 0x4000 | 0x80000;
            if (i & 2)
                g_scene.gao[0].flags |= 0x100;
            if (i & 4)
                g_scene.matrices[0][0].mtxflag |= 8;
            else
                g_scene.matrices[0][0].mtxflag &= ~8u;
            if (i & 8)
                g_scene.geo[0].vertices = 0;
            if (i & 16)
                g_scene.geo[0].vertexNum = 1;
            g_ret[SIDE] = PICK(GEO_MRM_GetRealNumberOfPoints)(&g_scene.geo[0]);
            PICK(OBJ_ComputeGeometricObjectBV)(&g_scene.gao[0], asSphere);
        });

    for (i = 0; i < 12; ++i)
        CASE(seed++, "staticlod bv", {
            g_scene.visual[0].GROGEOOrSomethingElse = (tGroGeneric*)&g_scene.lod;
            if (i & 1)
                g_scene.lod.geo[0] = 0;
            PICK(OBJ_ComputeStaticLODObjectBV)(&g_scene.gao[0], i & 2);
        });

    for (i = 0; i < 16; ++i)
        CASE(seed++, "anim bv", {
            g_scene.gao[3].flags = (i & 1) ? (g_scene.gao[3].flags | 2)
                                           : (g_scene.gao[3].flags & ~2);
            if (i & 2)
                g_scene.actor[3].skeletonGroup = 0;
            if (i & 4)
                g_scene.basic[3].actorObjectOrVisualObject = 0;
            if (i & 8)
                g_scene.gao[1].flags2 |= 0x2000000;
            if (i & 16)
                g_scene.gao[2].flags &= ~0x80000;
            PICK(OBJ_ComputeAnimBV)(&g_scene.gao[3]);
        });

    for (i = 0; i < 64; ++i)
        CASE(seed++, "ComputeBV", {
            int idx = i % NGAO;
            tGameObjectMain* g = &g_scene.gao[idx];
            g->flags = 0x1000 | 0x2000;
            if (i & 8) g->flags |= 0x4000;
            if (i & 16) g->flags |= 0x80;
            if (i & 2) g_scene.ext[idx].light = (tGroGeneric*)&g_scene.light;
            g->flags2 = (i & 4) ? 0x2000000 : 0;
            if (i & 32)
            {
                /* the animated arm: the object has to carry an actor,
                   not a visual, for OBJ_ComputeAnimBV to walk it */
                g->flags |= 2;
                g_scene.basic[idx].actorObjectOrVisualObject = &g_scene.actor[idx];
                g_scene.actor[idx].skeletonGroup = (i & 8) ? &g_scene.group[0] : 0;
            }
            else
            {
                g_scene.basic[idx].actorObjectOrVisualObject = &g_scene.visual[idx];
                g_scene.visual[idx].GROGEOOrSomethingElse =
                    (i % 4 == 0) ? (tGroGeneric*)&g_scene.geo[0]   /* id 1, geometry */
                                 : (i % 4 == 1) ? &g_scene.gro[5]  /* id 6, camera */
                                 : (i % 4 == 2) ? (tGroGeneric*)&g_scene.lod  /* id 8 */
                                                : &g_scene.gro[2]; /* id 3, no arm */
                if (i % 16 == 15)
                    g_scene.visual[idx].GROGEOOrSomethingElse = 0;
                if (i % 16 == 7)
                    g_scene.gro[5].intf = 0;
            }
            PICK(OBJ_ComputeBV)(g, i & 1);
        });
}

static void t_matrix(void)
{
    unsigned int seed = 3000;
    int i;

    for (i = 0; i < 64; ++i)
        CASE(seed++, "matrix", {
            int idx = i % NGAO;
            tGameObjectMain* g = &g_scene.gao[idx];
            if (i & 1) g->flags |= 0x10000; else g->flags &= ~0x10000;
            if (i & 2) g->flags |= 0x4000000; else g->flags &= ~0x4000000;
            if (i & 4) g->flags |= 0x400000; else g->flags &= ~0x400000;
            PICK(OBJ_SetAbsoluteMatrix)(g, &g_scene.mscratch[0]);
            PICK(OBJ_MakeAbsoluteMatrix)(g, &g_scene.mscratch[1]);
            PICK(OBJ_SightGet)(g, &g_scene.scratch[0]);
            PICK(OBJ_SightInitGet)(g, &g_scene.scratch[1]);
            PICK(OBJ_BankingGet)(g, &g_scene.scratch[2]);
            PICK(OBJ_HorizonGet)(g, &g_scene.scratch[3]);
            PICK(OBJ_SetLocalPosition)(g, &g_scene.scratch[4]);
            PICK(OBJ_SetAbsolutePosition)(g, &g_scene.scratch[5]);
            PICK(OBJ_SetInitialAbsolutePosition)(g, &g_scene.scratch[6]);
            PICK(OBJ_SetInitialAbsoluteMatrix)(g, &g_scene.mscratch[2]);
            g_ret[SIDE] = (long)(PICK(OBJ_pst_GetPos)(g) - (Vec3*)&g_scene);
            g_ret[SIDE] = g_ret[SIDE] * 7 + (PICK(OBJ_pst_GetFlashMatrix)(g) ? 1 : 0);
            g_ret[SIDE] = g_ret[SIDE] * 7 + (PICK(OBJ_pst_GetLocalMatrix)(g) ? 1 : 0);
            g_ret[SIDE] = g_ret[SIDE] * 7 + PICK(OBJ_b_IsChild)(g);
            g_ret[SIDE] = g_ret[SIDE] * 7 + (PICK(OBJ_pst_GetDynaStruct)(g) ? 1 : 0);
        });

    for (i = 0; i < 48; ++i)
        CASE(seed++, "hierarchy", {
            int idx = i % 3;
            PICK(OBJ_ComputeGlobalWhenHie)(&g_scene.gao[idx]);
            PICK(OBJ_ComputeGlobalWithLocal)(&g_scene.gao[2], &g_scene.mscratch[0], i & 1);
            PICK(OBJ_ComputeGlobalWithLocal)(&g_scene.gao[0], &g_scene.mscratch[1], i & 2);
            PICK(OBJ_ComputeLocalWhenHie)(&g_scene.gao[2]);
            PICK(OBJMY_ComputeLocalWhenHie)(&g_scene.gao[1]);
            PICK(OBJ_ComputeRecursivScale)(&g_scene.gao[2], &g_scene.scratch[0]);
            PICK(OBJ_ComputeRecursivScale)(&g_scene.gao[0], &g_scene.scratch[1]);
            PICK(OBJ_ScaleSet)(&g_scene.gao[(i + 1) % 3], &g_scene.scratch[2]);
            PICK(OBJ_ZoomSet)(&g_scene.gao[(i + 2) % 3], 0.5f + (float)(i % 4));
            g_ret[SIDE] = (long)(PICK(get_reference_point_gao_2_from_gao_sub_40BEA0)(&g_scene.gao[idx])
                                 ? 1 : 0);
        });

    for (i = 0; i < 32; ++i)
        CASE(seed++, "ReinitMatrixStorage", {
            tGameObjectMain* g = &g_scene.gao[i % NGAO];
            if (i & 1) g->flags |= 0x10000; else g->flags &= ~0x10000;
            if (i & 2) g->flags |= 0x4000000; else g->flags &= ~0x4000000;
            PICK(OBJ_ReinitMatrixStorageByFlags)(g, &g_scene.mscratch[0]);
            PICK(OBJ_RestoreInitialPos)(&g_scene.gao[2]);
        });
}

static void t_groups(void)
{
    unsigned int seed = 4000;
    int i;

    for (i = 0; i < 24; ++i)
        CASE(seed++, "groups", {
            long acc = 0;
            int k;
            PICK(OBJ_putInGroup)(&g_scene.gao[i % NGAO], (i & 1) ? &g_scene.group[0] : 0);
            if (i & 2)
                g_scene.ext[i % NGAO].grp = 0;
            for (k = -1; k < 6; ++k)
                acc = acc * 3 + (PICK(OBJ_pst_GroupGetByRank)((i & 4) ? 0 : &g_scene.group[0], k) ? 1 : 0);
            acc = acc * 3 + (PICK(OBJ_p_GetGro)(&g_scene.gao[i % NGAO]) ? 1 : 0);
            acc = acc * 3 + (PICK(OBJ_p_GetGrm)(&g_scene.gao[i % NGAO]) ? 1 : 0);
            acc = acc * 3 + (PICK(OBJ_p_GetCurrentGeo)(&g_scene.gao[i % NGAO]) ? 1 : 0);
            g_ret[SIDE] = acc;
        });

    for (i = 0; i < 12; ++i)
        CASE(seed++, "GetCurrentGeo", {
            g_scene.visual[0].GROGEOOrSomethingElse =
                (i % 3 == 0) ? (tGroGeneric*)&g_scene.geo[0]
                             : (i % 3 == 1) ? (tGroGeneric*)&g_scene.lod
                                            : 0;
            GMOCK_lodPick = (i & 4) ? &g_scene.geo[1] : 0;
            if (i & 8)
                g_scene.gao[0].flags &= ~0x4000;
            g_ret[SIDE] = PICK(OBJ_p_GetCurrentGeo)(&g_scene.gao[0]) ? 1 : 0;
        });

    for (i = 0; i < 8; ++i)
        CASE(seed++, "modifier", {
            g_scene.ext[0].mdf = (i & 1) ? &g_scene.mdf : 0;
            g_scene.mdf.modifierFlags = (unsigned)(i * 0x1000000);
            PICK(OBJ_FreezeModifier_C)(&g_scene.gao[0], 7, i & 2);
            g_ret[SIDE] = PICK(OBJ_ModifierType_C)(&g_scene.gao[0], (i & 4) ? 7 : 3);
        });
}

/* The seven one- and two-line entry points the larger cases never
   reach, so that the only functions the test does not enter are the two
   loader callbacks. */
static void t_small(void)
{
    unsigned int seed = 4500;
    int i;

    for (i = 0; i < 16; ++i)
        CASE(seed++, "small", {
            tGameObjectMain* g = &g_scene.gao[i % NGAO];
            PICK(OBJ_SightSet)(g, &g_scene.scratch[0]);
            PICK(OBJ_BankingSet)(g, &g_scene.scratch[1]);
            PICK(OBJ_SightGeneralSet)(g, &g_scene.scratch[2], &g_scene.scratch[3]);
            PICK(OBJ_BankingGeneralSet)(g, &g_scene.scratch[4], &g_scene.scratch[5]);
            PICK(OBJ_AddInGroup)(&g_scene.group[i & 1], g);
            if (i & 2)
                g->flags &= ~0x2000;
            PICK(OBJ_GameObject_CreateExtendedIfNot)(g);
            PICK(OBJ_FreeGroupGrl)((i & 4) ? &g_scene.table[1] : 0);
            PICK(OBJ_FreeGroup)(&g_scene.group[1]);
        });
}

static void t_culling(void)
{
    unsigned int seed = 5000;
    int i;

    g_fLo = offsetof(Scene, scratch);
    g_fHi = offsetof(Scene, fscratch) + sizeof(g_scene.fscratch);
    for (i = 0; i < 32; ++i)
        CASE(seed++, "UpdateCullingVars", {
            g_scene.cam.flags = i & 1;
            PICK(OBJ_UpdateCullingVars)(&g_scene.cam);
            /* the globals the pass writes are shared between the two
               copies, so copy them into the scene to be compared */
            g_scene.scratch[0] = g_stOverlayed_GlobalNormPlaneUp;
            g_scene.scratch[1] = g_stOverlayed_GlobalNormPlaneDown;
            g_scene.scratch[2] = g_stOverlayed_GlobalNormPlaneLeft;
            g_scene.scratch[3] = g_stOverlayed_GlobalNormPlaneRight;
            g_scene.scratch[4] = g_stGlobalCameraPos;
            g_scene.scratch[5] = g_stGlobalCameraSight;
            g_scene.fscratch[0] = g_fDistance;
        });

    g_fLo = 0;
    g_fHi = 0;
    for (i = 0; i < 64; ++i)
        CASE(seed++, "culling", {
            Vec3* box[2];
            uint8_t fl = 0;
            long acc;
            PICK(OBJ_UpdateCullingVars)(&g_scene.cam);
            box[0] = &g_scene.scratch[0];
            box[1] = &g_scene.scratch[1];
            box[0]->x = frand() * (float)(i % 5);
            box[0]->y = frand() * (float)(i % 5);
            box[0]->z = frand() * (float)(i % 5);
            box[1]->x = box[0]->x - 1.0f;
            box[1]->y = box[0]->y - 1.0f;
            box[1]->z = box[0]->z - 1.0f;
            acc = PICK(OBJ_CullingAABBox)(box, (i & 1) ? &fl : 0);
            acc = acc * 7 + fl;
            acc = acc * 7 + PICK(OBJ_CullingSphere)(&g_scene.scratch[2], 0.25f * (float)(i % 7));
            acc = acc * 7 + PICK(OBJ_CullingPointInside)(&g_scene.scratch[3],
                                                         &g_stGlobalNormPlaneLeft,
                                                         &g_stGlobalNormPlaneUp,
                                                         &g_stGlobalNormPlaneRight,
                                                         &g_stGlobalNormPlaneDown,
                                                         (char)(i & 15));
            if (i & 2)
                PICK(OBJ_BV_SetSphereFlag)(&g_scene.bv[0]);
            if (i & 4)
            {
                g_scene.bv[0].BVMax.x = 0.0f;
                g_scene.bv[0].BVMax.y = 0.0f;
                g_scene.bv[0].BVMax.z = 0.0f;
            }
            /* gao[3] is the animated one and carries a real actor, so the
               anim arm stores its flag inside a tGameObjectActorObject
               rather than 176 bytes past a tGameObjectVisualObject */
            acc = acc * 7 + PICK(OBJ_CullingObject)((i & 8) ? &g_scene.gao[3]
                                                            : &g_scene.gao[0]);
            /* OBJ_CullingObject passes the address of an uninitialised
               byte to OBJ_CullingAABBox, which writes it only when the
               box is inside, and then stores it whatever happened. The
               byte is not comparable between two stack frames; the
               result the callers use is. */
            g_scene.visual[0].culledFlagFromCullingObject = 0;
            g_scene.actor[3].culledFlagFromCullingObject = 0;
            g_ret[SIDE] = acc;
        });

    for (i = 0; i < 96; ++i)
        CASE(seed++, "frame culling", {
            float a = 0.0f;
            float b = 0.0f;
            long acc;
            g_scene.scratch[0].x = frand() * (float)(1 + i % 9);
            g_scene.scratch[0].y = frand() * (float)(1 + i % 9);
            g_scene.scratch[0].z = (float)(i % 9) - 3.0f;
            g_scene.scratch[1].x = g_scene.scratch[0].x - 1.0f - (float)(i % 3);
            g_scene.scratch[1].y = g_scene.scratch[0].y - 1.0f - (float)(i % 3);
            g_scene.scratch[1].z = g_scene.scratch[0].z - 1.0f - (float)(i % 4);
            acc = PICK(OBJ_Frame_CullingSphere)(&g_scene.scratch[0], 0.5f + (float)(i % 6),
                                                &g_scene.cam, 640, 480,
                                                (i & 1) ? &a : 0, (i & 2) ? &b : 0);
            g_scene.fscratch[0] = a;
            g_scene.fscratch[1] = b;
            a = 0.0f;
            b = 0.0f;
            acc = acc * 7 + PICK(OBJ_Frame_CullingAABBox)(&g_scene.scratch[1], &g_scene.scratch[0],
                                                          &g_scene.cam, 640, 480,
                                                          (i & 1) ? &a : 0, (i & 2) ? &b : 0);
            g_scene.fscratch[2] = a;
            g_scene.fscratch[3] = b;
            g_ret[SIDE] = acc;
        });

    /* boxes wide enough and near enough that their corners land
       outside the viewport, which is the only way the clamped and the
       unclamped extents the two ratios divide can differ */
    for (i = 0; i < 128; ++i)
        CASE(seed++, "frame culling wide", {
            float a = 0.0f;
            float b = 0.0f;
            long acc;
            float sz = 0.5f + (float)(i % 11) * 2.0f;
            g_scene.cam.factorX = 200.0f + (float)(i % 5) * 400.0f;
            g_scene.cam.factorY = g_scene.cam.factorX;
            g_scene.scratch[0].x = (float)((i % 7) - 3) * sz;
            g_scene.scratch[0].y = (float)((i % 5) - 2) * sz;
            g_scene.scratch[0].z = (float)(i % 13) - 7.5f;
            g_scene.scratch[1].x = g_scene.scratch[0].x - sz;
            g_scene.scratch[1].y = g_scene.scratch[0].y - sz * 1.5f;
            g_scene.scratch[1].z = g_scene.scratch[0].z - sz * 0.5f;
            acc = PICK(OBJ_Frame_CullingAABBox)(&g_scene.scratch[1], &g_scene.scratch[0],
                                                &g_scene.cam, 640, 480, &a, &b);
            g_scene.fscratch[0] = a;
            g_scene.fscratch[1] = b;
            a = 0.0f;
            b = 0.0f;
            acc = acc * 7 + PICK(OBJ_Frame_CullingSphere)(&g_scene.scratch[0], sz,
                                                          &g_scene.cam, 640, 480, &a, &b);
            g_scene.fscratch[2] = a;
            g_scene.fscratch[3] = b;
            g_ret[SIDE] = acc;
        });

    for (i = 0; i < 32; ++i)
        CASE(seed++, "CalcLODVis", {
            if (i & 1)
                PICK(OBJ_BV_SetSphereFlag)(&g_scene.bv[0]);
            if (i & 2)
                g_scene.view.displayData = 0;
            g_scene.view.cameraMtx.Pos.z = (float)(i % 8) * 2.0f;
            g_ret[SIDE] = PICK(OBJ_uc_CalcLODVis)(&g_scene.gao[0], &g_scene.view);
        });

    /* Both loops below are driven from a seed base of their own, above
       everything this function uses, so that adding them does not shift
       the random scenes of the cases before them. */
    seed = 5900;

    /* The corner scan's stop index does not reach the output: once it has
       found one corner, the measuring loop reads all eight regardless.
       What the scan decides is only whether a match exists at all, so
       only a box whose SOLE on-screen corner is the last one can see a
       change in the loop bound. The random boxes above never produce
       that -- every match they find is at corner 0, 2, 4 or 6.

       The camera sits at (-1,-2,-8) and the projection shears depth into
       x and y, so in camera-relative terms this box spans X -3..5,
       Y 2..14, W 10..40 at a focal length of 2000. Corner 7 projects to
       the exact centre; every other corner fails the inside test and
       both straddle arms, which needs each of them either outside the
       rect on both axes, or outside on one and on the latch's
       non-triggering side of the other. */
    for (i = 0; i < 4; ++i)
        CASE(seed++, "frame culling last corner", {
            float a = 0.0f;
            float b = 0.0f;
            g_scene.cam.factorX = 2000.0f;
            g_scene.cam.factorY = 2000.0f;
            g_scene.scratch[1].x = -4.0f;
            g_scene.scratch[1].y = 0.0f;
            g_scene.scratch[1].z = 2.0f;
            g_scene.scratch[0].x = 4.0f;
            g_scene.scratch[0].y = 12.0f;
            g_scene.scratch[0].z = 32.0f;
            g_ret[SIDE] = PICK(OBJ_Frame_CullingAABBox)(&g_scene.scratch[1], &g_scene.scratch[0],
                                                        &g_scene.cam, 640, 480,
                                                        (i & 1) ? &a : 0, (i & 2) ? &b : 0);
            g_scene.fscratch[0] = a;
            g_scene.fscratch[1] = b;
        });

    /* lod only reaches its 255 clamp for a large volume seen from very
       close. The loop above hit that by luck of the seed, which means a
       case added anywhere before it took the coverage away again; this
       drives it on purpose. */
    for (i = 0; i < 4; ++i)
        CASE(seed++, "CalcLODVis clamp", {
            g_scene.bv[0].BVMin.x = -500.0f;
            g_scene.bv[0].BVMin.y = -500.0f;
            g_scene.bv[0].BVMin.z = -500.0f;
            g_scene.bv[0].BVMax.x = 500.0f;
            g_scene.bv[0].BVMax.y = 500.0f;
            g_scene.bv[0].BVMax.z = 500.0f;
            g_scene.view.cameraMtx.Pos.x = 0.5f + (float)i;
            g_scene.view.cameraMtx.Pos.y = 0.5f;
            g_scene.view.cameraMtx.Pos.z = 0.5f;
            g_ret[SIDE] = PICK(OBJ_uc_CalcLODVis)(&g_scene.gao[0], &g_scene.view);
        });
}

static void t_hierarchy_call(void)
{
    unsigned int seed = 6000;
    int i;

    for (i = 0; i < 48; ++i)
        CASE(seed++, "HierarchyMainCall", {
            if (i & 32)
            {
                /* the other order, parents first */
                int k;
                for (k = 0; k < NGAO; ++k)
                    g_scene.helems[k].objectPtr = &g_scene.gao[k];
            }
            if (i & 1)
                g_scene.helems[2].objectPtr = 0;
            if (i & 2)
                g_scene.helems[4].objectPtr = (void*)-1;
            if (i & 4)
                g_scene.gaomtx[3].refGao2 = &g_scene.gao[2];
            if (i & 8)
            {
                /* 0x400000 and a null local matrix never occur together
                   in the engine, so the flag goes with the pointer */
                g_scene.basic[5].mtx = 0;
                g_scene.gao[5].flags &= ~0x400000;
            }
            if (i & 16)
            {
                g_scene.gao[4].basic = 0;
                g_scene.gao[4].flags &= ~0x400000;
            }
            g_scene.world.hierarchyCallCount = i & 1;
            PICK(OBJ_HierarchyMainCall)(&g_scene.world);
        });
}

static void t_eyefollow(void)
{
    unsigned int seed = 7000;
    int i;

    for (i = 0; i < 192; ++i)
        CASE(seed++, "EyeFollow", {
            Vec3 tgt;
            tgt = g_scene.scratch[0];
            if (i & 1)
            {
                tgt.x = 0.0f;
                tgt.y = 0.0f;
                tgt.z = 0.0f;
            }
            if (i & 0x100)
            {
                OBJ_gf_EYEFOLLOW_HeadZMax = 1.2f;
                OBJ_gf_EYEFOLLOW_HeadXMin = -0.3f;
                OBJ_gf_EYEFOLLOW_TorsoZMin = -0.9f;
                OBJ_gf_EYEFOLLOW_HeadXOffset = 0.15f;
            }
            if (i & 0x200)
                g_scene.gao[3].flags &= ~2;
            if (i & 0x400)
                g_scene.actor[3].actionKit = 0;
            PICK(OBJ_EyeFollow)(&g_scene.gao[3], &tgt, 0.25f + 0.25f * (float)(i % 4),
                                i % 5,                 /* preset, incl. the default arm */
                                (i >> 1) & 1,          /* clampToSight */
                                (i >> 2) & 1,          /* unlimited */
                                (i >> 3) & 1,          /* torsoFollowsHead */
                                (i >> 4) & 1,          /* torsoFollowsOwn */
                                (i >> 5) & 1,          /* useGlobalLimits */
                                (i >> 6) & 1);         /* useThirdBone */
        });
}

static void t_lifecycle(void)
{
    static const int createFlags[] = {
        0, 2, 4, 0x1000, 0x1002, 0x1004, 0x1000 | 0x4000, 0x10000, 0x4000000,
        0x10000 | 0x4000000, 0x1000 | 0x400000, 0x1000 | 0x200000,
        0x1000 | 0x1000000 | 0x200000, 0x2000, 0x2000 | 0x700, 0x2000 | 0x20,
        0x2000 | 0x8000, 0x80000, 0x1000 | 0x2000 | 0x4000 | 0x80000 | 0x400000,
    };
    unsigned int seed = 8000;
    int i;

    for (i = 0; i < (int)(sizeof(createFlags) / sizeof(createFlags[0])); ++i)
        CASE(seed++, "GameObject_Create", {
            tGameObjectMain* g = PICK(OBJ_GameObject_Create)(createFlags[i]);
            g_ret[SIDE] = (long)((unsigned char*)g - GMOCK_HeapBase());
            g_ret[SIDE] = g_ret[SIDE] * 31 + LOA_ul_FileTypeSize_37 + LOA_ul_FileTypeSize_29;
        });

    for (i = 0; i < (int)(sizeof(createFlags) / sizeof(createFlags[0])); ++i)
        CASE(seed++, "Create+Remove", {
            tGameObjectMain* g = PICK(OBJ_GameObject_Create)(createFlags[i] | 0x1000);
            PICK(OBJ_GameObject_Remove)(g, (BOOL)(i & 1));
            g_ret[SIDE] = (long)GMOCK_HeapUsed();
        });

    for (i = 0; i < 64; ++i)
        CASE(seed++, "ChangeIdentityFlags", {
            /* 0x1000 and 0x2000 go in because the engine promotes them
                from the other bits only on the way in, never on an object
                it is handed */
            tGameObjectMain* g = PICK(OBJ_GameObject_Create)(createFlags[i % 19] | 0x1000 | 0x2000);
            unsigned int nf = (unsigned)createFlags[(i + 7) % 19] | 0x1000u | 0x2000u;
            if (i & 1) nf |= 0x40;
            if (i & 2) nf |= 0x8000;
            if (i & 4) nf |= 0x40000;
            if (i & 8) nf |= 0x2000000;
            /* dropping 0x200000 while 0x1000000 is still set frees the
               multi-part block and then dereferences it, in both copies */
            if (g->flags & 0x1000000) nf |= 0x200000u;
            PICK(OBJ_ChangeIdentityFlags)(g, nf, (i & 16) ? -1 : (int)g->flags);
            g_ret[SIDE] = (long)g->flags * 31 + (long)g->flags2;
        });

    for (i = 0; i < 16; ++i)
        CASE(seed++, "AllocateGizmo", {
            tGameObjectMain* g = PICK(OBJ_GameObject_Create)(0x1000 | 0x2000 | 0x80000);
            PICK(OBJ_GameObject_AllocateGizmo)(g, 1 + (i % 4), i & 1);
            PICK(OBJ_Gizmo_UpdateGizmoPtr)(g);
            g_ret[SIDE] = (long)GMOCK_HeapUsed();
        });

    for (i = 0; i < 24; ++i)
        CASE(seed++, "Reinit", {
            tGameObjectMain* g = &g_scene.gao[i % NGAO];
            if (i & 1) g->flags |= 1; else g->flags &= ~1;
            if (i & 2) g->flags &= ~0x4000;
            /* dropping the basic block means dropping every flag that
               implies it; the engine never has one without the other */
            if (i & 4) { g->basic = 0; g->flags &= ~(0x1000 | 0x4000 | 0x400000 | 0x200000 | 2 | 4); }
            PICK(OBJ_Reinit)(g);
            PICK(OBJ_CheckFlagsAndActivateObject)(&g_scene.gao[(i + 1) % NGAO], i * 0x111111);
        });

    for (i = 0; i < 24; ++i)
        CASE(seed++, "ResolveAIRef", {
            tGameObjectMain* g = &g_scene.gao[3];
            if (i & 1) g->flags |= 0x20; else g->flags &= ~0x20;
            if (i & 2) g->flags |= 0x2000000; else g->flags &= ~0x2000000;
            if (i & 4) g_scene.actor[3].actionKit = 0;
            if (i & 8) g_scene.anim[0].ptr = 0;
            if (i & 16) g_scene.actor[3].animPtr[0] = (tAnim*)-1;
            g_scene.ext[3].AIInstance = (i & 32) ? (tAI*)&g_scene.eve : 0;
            PICK(OBJ_ResolveAIRef)(g);
        });

    for (i = 0; i < 24; ++i)
        CASE(seed++, "Duplicate", {
            tGameObjectMain* src = PICK(OBJ_GameObject_Create)(createFlags[i % 19] | 0x1000 | 0x2000);
            tGameObjectMain* dup;
            src->flags2 = i * 0x111111;
            /* Create leaves the source's matrices identical to each
               other, which makes a copy taken from the wrong one of them
               invisible. Give each slot a mark of its own -- in v1, not
               in Pos, which Duplicate overwrites when it is handed a
               position. */
            src->matrices[0].v1.x = 11.0f + (float)i;
            if (src->flags & 0x10000)
            {
                src->matrices[1].v1.x = 22.0f + (float)i;
                if (src->flags & 0x4000000)
                    src->matrices[2].v1.x = 33.0f + (float)i;
            }
            else if (src->flags & 0x4000000)
            {
                src->matrices[1].v1.x = 22.0f + (float)i;
            }
            dup = PICK(OBJ_GameObject_Duplicate)(&g_scene.world, src, i & 1, i & 2,
                                                 (i & 4) ? &g_scene.scratch[0] : 0);
            g_ret[SIDE] = (long)dup->flags * 31 + (long)dup->flags2;
        });
}

int main(void)
{
    MATH_InitModule();

    if (getenv("GO_OFFSETS"))
    {
#define O(m) printf("%-28s %6lu %6lu\n", #m, (unsigned long)offsetof(Scene, m), (unsigned long)sizeof(((Scene*)0)->m))
        O(gao); O(basic); O(gaomtx); O(bv); O(ext); O(coll); O(fields);
        O(visual); O(actor); O(mp); O(matrices); O(group); O(table);
        O(elems); O(intf); O(gro); O(geo); O(lod); O(verts); O(light);
        O(colmap); O(colobj); O(colobjp); O(mesh); O(meshverts);
        O(links); O(partmtx); O(cam); O(view); O(display); O(anim);
        O(eve); O(kitBuf); O(kitStore); O(actionStore); O(mdf); O(world);
        O(scratch); O(mscratch); O(fscratch);
#undef O
        return 0;
    }

    t_flags();
    t_test_flag();
    t_bv_basics();
    t_bv_dispatch();
    t_matrix();
    t_groups();
    t_small();
    t_culling();
    t_hierarchy_call();
    t_eyefollow();
    t_lifecycle();

    printf("gameobject: %d cases, %d differing\n", g_cases, g_bad);
    printf("  normalised plane floats: %d differing, worst %.3g relative, %.3g absolute\n",
           g_fDiffs, g_fWorstRel, g_fWorstAbs);
    /* The four restored MATH_NormalizeEqualVector calls sum the squares
       x+y+z where the inlined copies summed z+y+x, which moves the last
       bit of a plane normal. The count is pinned, not just reported: a
       tolerance that swallowed a real narrowing would otherwise pass. */
    if (g_fDiffs != PLANE_FLOAT_DIFFS)
    {
        printf("  plane float difference count moved: %d, expected %d\n",
               g_fDiffs, PLANE_FLOAT_DIFFS);
        ++g_bad;
    }
    return g_bad ? 1 : 0;
}
