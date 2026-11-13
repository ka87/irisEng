/* Differential test for world.c: the frozen pre-rewrite copy against the
   rewritten one, over identical state.

   Both copies run over one scene buffer at one address with the mock
   allocator rewound between them, so every pointer either copy writes
   into the scene can be compared directly rather than being normalised.
   A case is equivalent when the scene bytes, the used prefix of the mock
   heap, the per-side engine globals, the return values and the ordered
   sequence of mocked engine calls all match.

   What it is for. The rewrite replaced nine gotos, folded the bounding
   volume mask's eight-times unrolled corner test into a loop, folded two
   of its three switch-on-index pointer picks into tables, restored nine
   inlined copies of TAB_AddElemToTablePF and one of
   TAB_PTable_AddElemToTable, and rewrote the AI priority sort's cycle
   placement. Those are the changes a reading cannot settle, and the mask
   arithmetic in particular is a pile of mixed float/double/long double
   narrowings that a tidy-up could silently move.

   What it does not cover, and why: WOR_pst_World_Load, the two loader
   callbacks, WOR_World_Init/Close/Destroy, WOR_Render, WOR_Render_All_GO,
   WOR_SetCam and WOR_pst_Universe_AddWorldList need the big-file reader,
   the loader's address table and a Direct3D display device. Standing
   those up means standing up the mocks rather than the code. The mocks
   for everything on those paths abort instead of returning, so a rewrite
   that reached one of them fails the test rather than passing on a
   stand-in. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../common.h"
#include "../math.h"
#include "../tables.h"
#include "../gameobject.h"
#include "../groups.h"
#include "../modifiers.h"
#include "../modifiers/xmenmodifier.h"
#include "../graphicsobjects/gro_common.h"
#include "../graphicsobjects/geo.h"
#include "../graphicsobjects/geomaterial.h"
#include "../graphicsobjects/light.h"
#include "../cameraengine.h"
#include "../profiler.h"
#include "../engine.h"
#include "../events.h"
#include "../world.h"
#include "world_mocks.h"

/* ------------------------------------------- the two copies' exports */

#define DECL2(ret, name, args) ret BASE_##name args; ret NEW_##name args;

DECL2(void, WOR_ActivateObjectIfFlagsAllow, (tGameObjectMain*, tWorld*))
DECL2(void, WOR_MakeObjectVisibleIfFlagsAllow, (tGameObjectMain*, tWorld*))
DECL2(void, WOR_World_DetachObject, (tWorld*, tGameObjectMain*))
DECL2(void, WOR_World_ActivateObjects, (tWorld*))
DECL2(void, WOR_World_MakeObjectsVisible, (tWorld*))
DECL2(void, WOR_World_AddLoadedObject, (tWorld*, tGameObjectMain*, int))
DECL2(void, WOR_DelObj, (tWorld*, tGameObjectMain*))
DECL2(void, WOR_AddDelObj, (tWorld*, tGameObjectMain*, int*))
DECL2(void, WOR_ResetKillGO, (void))
DECL2(int, WOR_i_GetAllChildrenOf, (tWorld*, tGameObjectMain*, tGameObjectMain**))
DECL2(tGameObjectMain*, WOR_GetLastChildObj, (tGameObjectMain*))
DECL2(int, WOR_l_Universe_GetWorldIndex, (tWorld*))
DECL2(tWorld*, WOR_pst_Universe_GetActiveWorld, (int))
DECL2(void, EOT_SetOfEOT_Init, (tEOTStructs*))
DECL2(void, EOT_SetOfEOT_Close, (tEOTStructs*))
DECL2(void, EOT_SetOfEOT_Clear, (tEOTStructs*))
DECL2(void, EOT_SetOfEOT_Build, (tEOTStructs*, tableDescStruct*))
DECL2(void, EOT_SetOfEOT_OneBuild, (tableDescStruct*, int, tableDescStruct*))
DECL2(void, EOT_AISortWithPriority, (tableDescStruct*))
DECL2(void, EOT_RemoveObjectFromTableIfBadFlag, (tableDescStruct*, int))
DECL2(void, EOT_UpdateIdentityFlagsOfTable, (tableDescStruct*))
DECL2(void, WOR_World_CheckFather, (tGameObjectMain*, tWorld*))
DECL2(void, WOR_World_CheckGroup, (tGameObjectMain*, tWorld*))
DECL2(int, WOR_BVMaskComputeInside, (tGameObjectMain*))
DECL2(int, WOR_BVMaskPreCompute, (tGameObjectMain*, tDDCamStruct*, int))
DECL2(void, WOR_BVMaskPreComputeAll, (tWorld*, tDDContainerStruct*))
DECL2(void, WOR_World_AttachGameObject, (tWorld*, tGameObjectMain*))
DECL2(void, WOR_World_JustAfterLoadObject, (tWorld*, tGameObjectMain*, int, int))
DECL2(void, WOR_World_ResolveAIRef, (tWorld*))

extern int BASE_BVMask_gi_Num;
extern int NEW_BVMask_gi_Num;
extern tBVMasks BASE_BVMask_ga_Masks[200];
extern tBVMasks NEW_BVMask_ga_Masks[200];
extern tWorld* BASE_WOR_gpst_CurrentWorld;
extern tWorld* NEW_WOR_gpst_CurrentWorld;
extern tUniverse BASE_WOR_gst_Universe;
extern tUniverse NEW_WOR_gst_Universe;
extern BOOL BASE_WOR_gb_HasAddedMaterial;
extern BOOL NEW_WOR_gb_HasAddedMaterial;
extern int BASE_WOR_gi_NumKilledGO;
extern int NEW_WOR_gi_NumKilledGO;

#define CALL(fn, ...) (side == 0 ? BASE_##fn(__VA_ARGS__) : NEW_##fn(__VA_ARGS__))

static int* BvNum(int side) { return side == 0 ? &BASE_BVMask_gi_Num : &NEW_BVMask_gi_Num; }
static tBVMasks* Masks(int side) { return side == 0 ? BASE_BVMask_ga_Masks : NEW_BVMask_ga_Masks; }
static tUniverse* Univ(int side) { return side == 0 ? &BASE_WOR_gst_Universe : &NEW_WOR_gst_Universe; }

static void SetCurrentWorld(int side, tWorld* w)
{
    if (side == 0)
        BASE_WOR_gpst_CurrentWorld = w;
    else
        NEW_WOR_gpst_CurrentWorld = w;
}

static void SetHasAddedMaterial(int side, BOOL v)
{
    if (side == 0)
        BASE_WOR_gb_HasAddedMaterial = v;
    else
        NEW_WOR_gb_HasAddedMaterial = v;
}

/* ---------------------------------------------------------- the scene */

#define NG 10            /* game objects */
#define NW 2             /* worlds */
#define NSUB 3           /* sub-materials per multi-material */
#define NMASK 6          /* masks the inside test is driven against */

typedef struct
{
    tWorld world[NW];
    tGameObjectMain gao[NG];
    tGameObjectBasic basic[NG];
    tGameObjectExtended ext[NG];
    tGameObjectBV bv[NG];
    Mtx4x4 mtx[NG];
    tGaoMatrix gaoMtx[NG];
    tGameObjectDesignFields fields[NG];
    tGameObjectVisualObject visual[NG];
    tGameObjectActorObject actor[NG];
    tAnim anim[NG];
    tEventData eve[NG];
    tGameObjectEventsList evelist[NG];
    tGEOGRO gro[NG];
    tGEOMAT mat[NG];
    tGEOMAT sub[NG][NSUB];
    tGEOMAT* subPtr[NG][NSUB];
    tLIGHT light[NG];
    tModifier mdf[NG];
    tXmenMdfStruct xmen[NG];
    tGEOMAT xmenMat[NG];
    tGroIntf intfGeo;
    tGroIntf intfMat;
    tGroIntf intfSub;
    tGroIntf intfSub5;
    tGroIntf intfLight;
    tGroIntf intfLod;
    tModifierInterface mdfIntf;
    tGroup group;
    tableDescStruct groupTable;
    int flash[NG];
    int rli[NG][8];
    tDDContainerStruct dd;
    tEOTStructs eot;
    tableDescStruct srcTable;
    tableDescStruct dstTable;
    tGameObjectMain* children[NG + 4];
} Scene;

static Scene scene;

/* the AddRef the fake graphics-object interfaces carry; both copies call
   it through the same pointer, so the log compares directly */
static void MOCK_AddRef(tGroGeneric* o, int n)
{
    WMOCK_Logf("AddRef id=%d use=%d n=%d", o->intf ? o->intf->id : -1, o->usageCount, n);
    o->usageCount += n;
}

/* ----------------------------------------------------------- the PRNG */

static unsigned int rngState;

static void Seed(unsigned int s) { rngState = s * 2654435761u + 12345u; }

static unsigned int Next(void)
{
    rngState = rngState * 1103515245u + 12345u;
    return rngState >> 8;
}

static float NextF(float lo, float hi)
{
    return lo + (hi - lo) * ((float)(Next() & 0xFFFF) / 65535.0f);
}

/* ------------------------------------------------------- scene builder */

static void BuildScene(unsigned int seed)
{
    int i, j;

    memset(&scene, 0, sizeof(scene));
    memset(&gst_TablesManagerList, 0, sizeof(gst_TablesManagerList));
    gb_TablesManagerListInitialized = 0;
    Seed(seed);

    scene.intfGeo.id = 1;
    scene.intfGeo.AddRef = MOCK_AddRef;
    scene.intfMat.id = 4;
    scene.intfMat.AddRef = MOCK_AddRef;
    scene.intfSub.id = 3;
    scene.intfSub.AddRef = MOCK_AddRef;
    scene.intfSub5.id = 5;
    scene.intfSub5.AddRef = MOCK_AddRef;
    scene.intfLight.id = 6;
    scene.intfLight.AddRef = MOCK_AddRef;
    scene.intfLod.id = 8;
    scene.intfLod.AddRef = MOCK_AddRef;
    scene.mdfIntf.mdfId = 9;

    for (i = 0; i < NG; ++i)
    {
        scene.gao[i].flags = (int)(Next() & 0x7FFFFFFF);
        scene.gao[i].flags2 = (int)(Next() & 0x7FFFFFFF);
        scene.gao[i].matrices = &scene.mtx[i];
        scene.gao[i].boundingVolume = &scene.bv[i];
        scene.gao[i].basic = &scene.basic[i];
        scene.gao[i].extended = &scene.ext[i];
        scene.gao[i].distance = (uint8_t)(Next() & 0xFF);
        scene.gao[i].process = (char)(Next() & 0xFF);
        scene.gao[i].if_this_not0_wont_render_gao = (uint8_t)(Next() & 1);

        scene.ext[i].AIPriority = (uint8_t)(Next() % 5);
        scene.ext[i].fields = &scene.fields[i];
        scene.ext[i].light = (struct sGroGeneric*)&scene.light[i];
        scene.ext[i].grp = &scene.group;
        scene.ext[i].mdf = &scene.mdf[i];
        scene.ext[i].renderMinReqDistSq = NextF(-1.0f, 50.0f);

        scene.fields[i].designNet1 = (int)(Next() & 0xFF);
        scene.fields[i].designNet2 = (int)(Next() & 0xFF);
        scene.fields[i].designGO1 = (int)(Next() & 0xFF);
        scene.fields[i].designGO2 = (int)(Next() & 0xFF);

        scene.basic[i].mtx = &scene.gaoMtx[i];
        scene.basic[i].actorObjectOrVisualObject = &scene.visual[i];
        scene.basic[i].extraMaterial = &scene.mat[i];
        scene.gaoMtx[i].refGao2 = (i > 0) ? &scene.gao[i - 1] : NULL;

        scene.visual[i].GROGEOOrSomethingElse = (struct sGroGeneric*)&scene.gro[i];
        scene.visual[i].GROMAT = (struct sGroGeneric*)&scene.mat[i];
        scene.visual[i].RLIVertexColors = scene.rli[i];
        scene.rli[i][0] = (int)(Next() % 6);

        scene.gro[i].intf = &scene.intfGeo;
        scene.gro[i].vertexNum = Next() % 6;
        scene.mat[i].intf = &scene.intfMat;
        scene.mat[i].usageCount = (int)(Next() & 1);
        scene.mat[i].multiMaterialNumIfType4ThenAmbiantColor = NSUB;
        scene.mat[i].multiMaterialPtrIfType4ThenDiffuseColor = scene.subPtr[i];
        for (j = 0; j < NSUB; ++j)
        {
            scene.subPtr[i][j] = &scene.sub[i][j];
            switch ((i + j) % 3)
            {
            case 0: scene.sub[i][j].intf = &scene.intfGeo; break;
            case 1: scene.sub[i][j].intf = &scene.intfSub; break;
            default: scene.sub[i][j].intf = &scene.intfSub5; break;
            }
            Next();
        }
        scene.light[i].intf = &scene.intfLight;
        scene.light[i].flags = (int)(Next() & 0x4000000F) | 8;
        scene.light[i].color = (int)Next();
        scene.light[i].fnear = NextF(0.0f, 10.0f);
        scene.light[i].ffar = NextF(10.0f, 90.0f);

        scene.mdf[i].modifierIntf = &scene.mdfIntf;
        scene.mdf[i].mdfPrivateDataPtr = &scene.xmen[i];
        scene.mdf[i].nextElem = NULL;
        scene.xmen[i].geomat = ((Next() & 1) != 0) ? &scene.xmenMat[i] : NULL;
        scene.xmenMat[i].intf = &scene.intfSub;

        scene.actor[i].actionKit = NULL;
        scene.actor[i].animPtr[0] = &scene.anim[i];
        scene.actor[i].flagsEgBlendIw2 = (uint8_t)(Next() & 1);
        scene.anim[i].ptr = &scene.eve[i];
        scene.eve[i].eventListStruct = &scene.evelist[i];
        scene.evelist[i].usageCount = (int)(Next() % 7);

        scene.bv[i].BVMin.x = NextF(-20.0f, 0.0f);
        scene.bv[i].BVMin.y = NextF(-20.0f, 0.0f);
        scene.bv[i].BVMin.z = NextF(-20.0f, 0.0f);
        scene.bv[i].BVMax.x = NextF(0.0f, 20.0f);
        scene.bv[i].BVMax.y = NextF(0.0f, 20.0f);
        scene.bv[i].BVMax.z = NextF(0.0f, 20.0f);

        /* every fourth box ties two of its three extents exactly, which is
           the only input that separates >= from > in the flattest-axis pick */
        if ((i % 4) == 1)
        {
            scene.bv[i].BVMin.y = scene.bv[i].BVMin.x;
            scene.bv[i].BVMax.y = scene.bv[i].BVMax.x;
        }
        else if ((i % 4) == 2)
        {
            scene.bv[i].BVMin.z = scene.bv[i].BVMin.x;
            scene.bv[i].BVMax.z = scene.bv[i].BVMax.x;
        }
        else if ((i % 4) == 3)
        {
            scene.bv[i].BVMin.z = scene.bv[i].BVMin.y;
            scene.bv[i].BVMax.z = scene.bv[i].BVMax.y;
        }

        scene.mtx[i].Pos.x = NextF(-30.0f, 30.0f);
        scene.mtx[i].Pos.y = NextF(-30.0f, 30.0f);
        scene.mtx[i].Pos.z = NextF(-30.0f, 30.0f);

        scene.flash[i] = (int)(Next() & 0xFFFF) | 1;
    }

    scene.dd.cam.objectMtx.Pos.x = NextF(-40.0f, 40.0f);
    scene.dd.cam.objectMtx.Pos.y = NextF(-40.0f, 40.0f);
    scene.dd.cam.objectMtx.Pos.z = NextF(-40.0f, 40.0f);
    scene.dd.cam.objectMtx.v3.x = NextF(-1.0f, 1.0f);
    scene.dd.cam.objectMtx.v3.y = NextF(-1.0f, 1.0f);
    scene.dd.cam.objectMtx.v3.z = NextF(-1.0f, 1.0f);

    g_stGlobalNormPlaneLeft.x = 1.0f;  g_stGlobalNormPlaneLeft.y = 0.0f;  g_stGlobalNormPlaneLeft.z = 0.0f;
    g_stGlobalNormPlaneUp.x = 0.0f;    g_stGlobalNormPlaneUp.y = 1.0f;    g_stGlobalNormPlaneUp.z = 0.0f;
    g_stGlobalNormPlaneRight.x = -1.0f;g_stGlobalNormPlaneRight.y = 0.0f; g_stGlobalNormPlaneRight.z = 0.0f;
    g_stGlobalNormPlaneDown.x = 0.0f;  g_stGlobalNormPlaneDown.y = -1.0f; g_stGlobalNormPlaneDown.z = 0.0f;

    for (i = 0; i < NW; ++i)
    {
        TAB_PFtable_Init(&scene.world[i].objectTablePF, 8, 1.0f);
        TAB_PFtable_Init(&scene.world[i].activatedObjectsTable4PF, 8, 1.0f);
        TAB_PFtable_Init(&scene.world[i].visibleGameObjectsPF, 8, 1.0f);
        TAB_PFtable_Init(&scene.world[i].lightObjectsTablePF, 8, 1.0f);
        TAB_PFtable_Init(&scene.world[i].objectsWithBVMaskTablePF, 8, 1.0f);
        TAB_Ptable_Init(&scene.world[i].worldGROTable, 2, 1.0f);
        TAB_Ptable_Init(&scene.world[i].worldMaterialTable, 2, 1.0f);
        scene.world[i].dynCallCount = 3 + i;
        scene.world[i].aiCallCount = 5 + i;
        scene.world[i].hierarchyCallCount = 7 + i;
        scene.world[i].distanceToHide = (int)(Next() % 4) - 1;
        scene.world[i].deletedObjNum = 0;
    }
    TAB_PFtable_Init(&scene.groupTable, 8, 1.0f);
    scene.group.tablePF = &scene.groupTable;
    scene.group.groupElemNum = 1;
}

/* fill a PF table with n of the scene's objects, every third one a hole */
static void FillTable(tableDescStruct* t, int n, int holes)
{
    int i;

    for (i = 0; i < n; ++i)
    {
        if (holes && (i % 3) == 2)
            TAB_AddElemToTablePF(t, (void*)1, 0);
        else
            TAB_AddElemToTablePF(t, &scene.gao[i % NG], scene.gao[i % NG].flags);
    }
}

/* ------------------------------------------------------- the capture */

#define CAP_LOG 4000
#define CAP_HEAP (512u * 1024u)

typedef struct
{
    Scene scene;
    unsigned int heapUsed;
    unsigned char heap[CAP_HEAP];
    int bvNum;
    tBVMasks masks[NMASK + 2];
    tWorld* current;
    BOOL addedMaterial;
    int numKilled;
    int eotInside;
    int eotSum;
    int visInside;
    int visSum;
    int ret[8];
    int retNum;
    int logNum;
    int logOverflow;
    char log[CAP_LOG][WMOCK_LOG_LINE];
} Capture;

static Capture capA;
static Capture capB;

static void CaptureNow(int side, Capture* c)
{
    int i;

    c->scene = scene;
    c->heapUsed = WMOCK_HeapUsed();
    if (c->heapUsed > CAP_HEAP)
    {
        fprintf(stderr, "test_world: capture heap too small (%u)\n", c->heapUsed);
        exit(2);
    }
    memcpy(c->heap, WMOCK_HeapBase(), c->heapUsed);
    c->bvNum = *BvNum(side);
    memcpy(c->masks, Masks(side), sizeof(c->masks));
    c->current = side == 0 ? BASE_WOR_gpst_CurrentWorld : NEW_WOR_gpst_CurrentWorld;
    c->addedMaterial = side == 0 ? BASE_WOR_gb_HasAddedMaterial : NEW_WOR_gb_HasAddedMaterial;
    c->numKilled = side == 0 ? BASE_WOR_gi_NumKilledGO : NEW_WOR_gi_NumKilledGO;
    c->eotInside = PRO_EOTCreation.insideCounter;
    c->eotSum = PRO_EOTCreation.sumTime;
    c->visInside = PRO_Visibility.insideCounter;
    c->visSum = PRO_Visibility.sumTime;
    c->logNum = WMOCK_LogNum();
    c->logOverflow = WMOCK_LogOverflowed();
    if (c->logNum > CAP_LOG)
    {
        fprintf(stderr, "test_world: capture log too small (%d)\n", c->logNum);
        exit(2);
    }
    for (i = 0; i < c->logNum; ++i)
        memcpy(c->log[i], WMOCK_LogLine(i), WMOCK_LOG_LINE);
}

/* ------------------------------------------------------------- cases */

enum
{
    K_BVPRE, K_BVINSIDE, K_BVALL,
    K_EOTSORT, K_EOTBUILD, K_EOTONE, K_EOTFLAGS,
    K_ACTIVATE, K_VISIBLE, K_ADDLOADED, K_DETACH,
    K_DELOBJ, K_CHILDREN, K_UNIVERSE,
    K_CHECKFATHER, K_CHECKGROUP,
    K_ATTACH, K_JUSTAFTER, K_RESOLVEAI,
    K_NUM
};

static const char* KindName(int k)
{
    static const char* names[K_NUM] = {
        "BVPreCompute", "BVComputeInside", "BVPreComputeAll",
        "EOT_AISortWithPriority", "EOT_SetOfEOT_Build", "EOT_SetOfEOT_OneBuild",
        "EOT flag tables", "ActivateObjects", "MakeObjectsVisible",
        "AddLoadedObject", "DetachObject", "DelObj/AddDelObj",
        "GetAllChildrenOf", "Universe lookups",
        "CheckFather", "CheckGroup",
        "AttachGameObject", "JustAfterLoadObject", "ResolveAIRef"
    };
    return names[k];
}

typedef struct
{
    int kind;
    unsigned int seed;
    int a;
    int b;
} Case;

static void Run(int side, const Case* c, Capture* cap)
{
    tWorld* w;
    tWorld* w2;
    int i;
    int n;

    WMOCK_HeapReset();
    WMOCK_LogReset();
    WMOCK_ClockReset();
    memset(&PRO_EOTCreation, 0, sizeof(PRO_EOTCreation));
    memset(&PRO_Visibility, 0, sizeof(PRO_Visibility));
    memset(Masks(side), 0, 200 * sizeof(tBVMasks));
    *BvNum(side) = 0;
    SetCurrentWorld(side, NULL);
    SetHasAddedMaterial(side, FALSE);
    ENG_gf_SpecialTime = (c->seed & 1) ? 0.0f : 2.5f;
    ENG_gb_ActiveSectorization = (int)(c->seed & 2) >> 1;
    EVE_gb_PlayOnlyAnimTracks = 0;
    WMOCK_cullSeed = (int)(c->seed & 3);

    BuildScene(c->seed);
    w = &scene.world[0];
    w2 = &scene.world[1];
    SetCurrentWorld(side, w);
    cap->retNum = 0;

    switch (c->kind)
    {
    case K_BVPRE:
        *BvNum(side) = c->a % 4;
        cap->ret[cap->retNum++] = CALL(WOR_BVMaskPreCompute, &scene.gao[c->b % NG], &scene.dd.cam, c->a & 1);
        break;

    case K_BVINSIDE:
        for (i = 0; i < NMASK; ++i)
        {
            *BvNum(side) = i;
            CALL(WOR_BVMaskPreCompute, &scene.gao[i % NG], &scene.dd.cam, (c->a >> i) & 1);
        }
        *BvNum(side) = c->a % (NMASK + 1);
        cap->ret[cap->retNum++] = CALL(WOR_BVMaskComputeInside, &scene.gao[c->b % NG]);
        /* and again with the probe itself in the table, which stops the walk */
        Masks(side)[c->a % (NMASK + 1) > 0 ? (c->a % (NMASK + 1)) - 1 : 0].gao = &scene.gao[c->b % NG];
        cap->ret[cap->retNum++] = CALL(WOR_BVMaskComputeInside, &scene.gao[c->b % NG]);
        break;

    case K_BVALL:
        n = 2 + (c->a % 5);
        for (i = 0; i < n; ++i)
            TAB_AddElemToTablePF(&w->objectsWithBVMaskTablePF, &scene.gao[i % NG], scene.gao[i % NG].flags);
        if (c->b & 1)
            TAB_AddElemToTablePF(&w->objectsWithBVMaskTablePF, (void*)1, 0);
        CALL(WOR_BVMaskPreComputeAll, w, &scene.dd);
        break;

    case K_EOTSORT:
        n = 1 + (c->a % 8);
        for (i = 0; i < n; ++i)
            scene.ext[i % NG].AIPriority = (uint8_t)((c->seed >> (i & 7)) % 4);
        TAB_PFtable_Init(&scene.srcTable, 16, 1.0f);
        for (i = 0; i < n; ++i)
            TAB_AddElemToTablePF(&scene.srcTable, &scene.gao[i % NG],
                                 (int)((c->b & (1 << i)) ? 0x40000000 : 0));
        if (c->b & 0x100)
            TAB_AddElemToTablePF(&scene.srcTable, (void*)1, 0);
        CALL(EOT_AISortWithPriority, &scene.srcTable);
        break;

    case K_EOTBUILD:
        TAB_PFtable_Init(&scene.srcTable, 16, 1.0f);
        FillTable(&scene.srcTable, 1 + (c->a % 9), c->b & 1);
        CALL(EOT_SetOfEOT_Init, &scene.eot);
        CALL(EOT_SetOfEOT_Build, &scene.eot, &scene.srcTable);
        if (c->b & 2)
            CALL(EOT_SetOfEOT_Clear, &scene.eot);
        CALL(EOT_SetOfEOT_Close, &scene.eot);
        break;

    case K_EOTONE:
        TAB_PFtable_Init(&scene.srcTable, 16, 1.0f);
        TAB_PFtable_Init(&scene.dstTable, 16, 1.0f);
        FillTable(&scene.srcTable, 1 + (c->a % 9), c->b & 1);
        CALL(EOT_SetOfEOT_OneBuild, &scene.dstTable, 1 << (c->a % 26), &scene.srcTable);
        break;

    case K_EOTFLAGS:
        TAB_PFtable_Init(&scene.srcTable, 16, 1.0f);
        FillTable(&scene.srcTable, 1 + (c->a % 9), c->b & 1);
        CALL(EOT_UpdateIdentityFlagsOfTable, &scene.srcTable);
        for (i = 0; i < NG; ++i)
            scene.gao[i].flags ^= (int)(1 << (c->a % 30));
        CALL(EOT_RemoveObjectFromTableIfBadFlag, &scene.srcTable, 1 << (c->a % 30));
        break;

    case K_ACTIVATE:
        FillTable(&w->objectTablePF, 1 + (c->a % 7), c->b & 1);
        CALL(WOR_World_ActivateObjects, w);
        CALL(WOR_ActivateObjectIfFlagsAllow, &scene.gao[c->b % NG], w);
        break;

    case K_VISIBLE:
        FillTable(&w->objectTablePF, 1 + (c->a % 7), c->b & 1);
        CALL(WOR_World_MakeObjectsVisible, w);
        CALL(WOR_MakeObjectVisibleIfFlagsAllow, &scene.gao[c->b % NG], w);
        break;

    case K_ADDLOADED:
        FillTable(&w->objectTablePF, c->a % 5, 0);
        for (i = 0; i < 1 + (c->b % 4); ++i)
            CALL(WOR_World_AddLoadedObject, w, &scene.gao[(c->a + i) % NG], (int)Next());
        break;

    case K_DETACH:
        FillTable(&w->objectTablePF, 1 + (c->a % 7), c->b & 1);
        FillTable(&w->activatedObjectsTable4PF, 1 + (c->a % 5), 0);
        FillTable(&w->visibleGameObjectsPF, 1 + (c->a % 4), 0);
        FillTable(&w->lightObjectsTablePF, 2, 0);
        CALL(EOT_SetOfEOT_Init, &scene.eot);
        w->eot = scene.eot;
        CALL(WOR_World_DetachObject, w, &scene.gao[c->b % NG]);
        break;

    case K_DELOBJ:
        CALL(WOR_ResetKillGO);
        for (i = 0; i < 1 + (c->a % 6); ++i)
            CALL(WOR_AddDelObj, w, &scene.gao[i % NG], &scene.flash[i % NG]);
        CALL(WOR_DelObj, w, &scene.gao[c->b % NG]);
        for (i = 0; i < 1 + (c->b % 3); ++i)
            CALL(WOR_AddDelObj, w, &scene.gao[(i + 2) % NG], &scene.flash[(i + 2) % NG]);
        CALL(WOR_DelObj, w, &scene.gao[(c->b + 1) % NG]);
        break;

    case K_CHILDREN:
        /* every object has at most one child here: WOR_i_GetAllChildrenOf
           writes one slot per child with no bound, and WOR_GetLastChildObj
           gives it a single pointer to write into */
        for (i = 0; i < NG; ++i)
        {
            scene.gao[i].flags = (scene.gao[i].flags & ~0x400000) |
                                 ((c->a & (1 << (i & 7))) ? 0x400000 : 0);
            scene.gao[i].flags2 |= 1;
            scene.gao[i].flags2 &= ~0x400000;
        }
        FillTable(&w->objectTablePF, NG, c->b & 1);
        SetCurrentWorld(side, w);
        cap->ret[cap->retNum++] = CALL(WOR_i_GetAllChildrenOf, w, &scene.gao[c->b % NG], scene.children);
        cap->ret[cap->retNum++] = CALL(WOR_i_GetAllChildrenOf, w, NULL, scene.children);
        cap->ret[cap->retNum++] = (int)(intptr_t)CALL(WOR_GetLastChildObj, &scene.gao[c->b % NG]);
        break;

    case K_UNIVERSE:
        TAB_PFtable_Init(&Univ(side)->univWorldTablePF, 8, 1.0f);
        for (i = 0; i < 1 + (c->a % 4); ++i)
        {
            if ((c->b & (1 << i)) != 0)
                TAB_AddElemToTablePF(&Univ(side)->univWorldTablePF, (void*)1, 0);
            else
                TAB_AddElemToTablePF(&Univ(side)->univWorldTablePF,
                                     &scene.world[i % NW], (int)((c->a >> i) & 1));
        }
        cap->ret[cap->retNum++] = CALL(WOR_l_Universe_GetWorldIndex, w);
        cap->ret[cap->retNum++] = CALL(WOR_l_Universe_GetWorldIndex, w2);
        cap->ret[cap->retNum++] = (int)(intptr_t)CALL(WOR_pst_Universe_GetActiveWorld, c->a % 3);
        break;

    case K_CHECKFATHER:
        FillTable(&w->objectTablePF, 1 + (c->a % 6), c->b & 1);
        for (i = 0; i < NG; ++i)
        {
            scene.gao[i].flags |= 0x400000;
            if ((c->b & (1 << (i & 7))) != 0)
                scene.basic[i].mtx = NULL;
        }
        CALL(WOR_World_CheckFather, &scene.gao[c->a % NG], w);
        break;

    case K_CHECKGROUP:
        FillTable(&w->objectTablePF, 1 + (c->a % 4), 0);
        FillTable(&scene.groupTable, 1 + (c->a % 6), c->b & 1);
        for (i = 0; i < NG; ++i)
            scene.gao[i].flags |= 0x800000;
        CALL(WOR_World_CheckGroup, &scene.gao[c->a % NG], w);
        break;

    case K_ATTACH:
        for (i = 0; i < NG; ++i)
        {
            /* 0x4000 and 2 pick which way the one union field is read, so
               exactly one of them is set: the engine's own note at that
               line says the same thing */
            scene.gao[i].flags = (int)(c->seed * (i + 1)) & 0x00C00010;
            if (c->b & 2)
                scene.gao[i].flags |= 2;
            else
                scene.gao[i].flags |= 0x4000;
            if (c->b & 4)
                scene.actor[i].actionKit = (struct sActionKit*)&scene.actor[i];
            scene.basic[i].actorObjectOrVisualObject =
                (scene.gao[i].flags & 2) ? (void*)&scene.actor[i] : (void*)&scene.visual[i];
            if (c->b & 8)
                scene.visual[i].GROGEOOrSomethingElse = NULL;
            if (c->b & 16)
                scene.visual[i].GROMAT = NULL;
            if (c->b & 32)
                scene.ext[i].grp = NULL;
            if (c->b & 1)
                scene.gao[i].flags |= 0x800000;
            if ((c->b & 64) && i + 1 < NG)
                scene.mdf[i].nextElem = &scene.mdf[i + 1];
        }
        CALL(WOR_World_AttachGameObject, w, &scene.gao[c->a % NG]);
        break;

    case K_JUSTAFTER:
        for (i = 0; i < NG; ++i)
        {
            scene.gao[i].flags = (int)(c->seed * (i + 3)) & 0x08002002;
            scene.gao[i].flags |= ((c->b >> 4) & 7) << 8;
            scene.basic[i].actorObjectOrVisualObject =
                (scene.gao[i].flags & 2) ? (void*)&scene.actor[i] : (void*)&scene.visual[i];
            if (c->b & 1)
                scene.actor[i].animPtr[0] = NULL;
            if (c->b & 2)
                scene.ext[i].mdf = NULL;
        }
        CALL(WOR_World_JustAfterLoadObject, w, &scene.gao[c->a % NG],
             (c->b >> 2) & 1, (c->b >> 3) & 1);
        break;

    case K_RESOLVEAI:
        FillTable(&w->objectTablePF, 1 + (c->a % 7), c->b & 1);
        for (i = 0; i < NG; ++i)
        {
            if ((c->b & (1 << (i & 7))) != 0)
                scene.gao[i].flags |= 0x40;
            else
                scene.gao[i].flags &= ~0x40;
            if (c->a & 1)
                scene.fields[i].designNet1 = 0;
        }
        CALL(WOR_World_ResolveAIRef, w);
        break;
    }

    CaptureNow(side, cap);
}

/* ------------------------------------------------------- the compare */

static int fails;
static int casesRun;
static int logLinesCompared;
static long long bytesCompared;

static void Report(const Case* c, const char* what)
{
    if (fails < 12)
        printf("  DIFF %-24s seed=%u a=%d b=%d : %s\n",
               KindName(c->kind), c->seed, c->a, c->b, what);
    ++fails;
}

static void Compare(const Case* c)
{
    int i;
    int bad = 0;

    bytesCompared += (long long)sizeof(Scene);
    if (memcmp(&capA.scene, &capB.scene, sizeof(Scene)) != 0)
    {
        const unsigned char* a = (const unsigned char*)&capA.scene;
        const unsigned char* b = (const unsigned char*)&capB.scene;
        unsigned int off = 0;
        while (off < sizeof(Scene) && a[off] == b[off])
            ++off;
        {
            char msg[96];
            snprintf(msg, sizeof(msg), "scene byte %u of %u differs (%02x vs %02x)",
                     off, (unsigned int)sizeof(Scene), a[off], b[off]);
            Report(c, msg);
        }
        bad = 1;
    }
    if (capA.heapUsed != capB.heapUsed)
    {
        Report(c, "heap used differs");
        bad = 1;
    }
    else if (memcmp(capA.heap, capB.heap, capA.heapUsed) != 0)
    {
        Report(c, "heap bytes differ");
        bad = 1;
    }
    bytesCompared += capA.heapUsed;
    if (capA.bvNum != capB.bvNum)
    {
        Report(c, "BVMask_gi_Num differs");
        bad = 1;
    }
    if (memcmp(capA.masks, capB.masks, sizeof(capA.masks)) != 0)
    {
        Report(c, "BVMask_ga_Masks differ");
        bad = 1;
    }
    bytesCompared += (long long)sizeof(capA.masks);
    if (capA.current != capB.current || capA.addedMaterial != capB.addedMaterial ||
        capA.numKilled != capB.numKilled)
    {
        Report(c, "world globals differ");
        bad = 1;
    }
    if (capA.eotInside != capB.eotInside || capA.eotSum != capB.eotSum ||
        capA.visInside != capB.visInside || capA.visSum != capB.visSum)
    {
        Report(c, "profiler rasters differ");
        bad = 1;
    }
    if (capA.retNum != capB.retNum)
    {
        Report(c, "return count differs");
        bad = 1;
    }
    else
    {
        for (i = 0; i < capA.retNum; ++i)
        {
            if (capA.ret[i] != capB.ret[i])
            {
                char msg[64];
                snprintf(msg, sizeof(msg), "return %d: %d vs %d", i, capA.ret[i], capB.ret[i]);
                Report(c, msg);
                bad = 1;
                break;
            }
        }
    }
    if (capA.logOverflow || capB.logOverflow)
    {
        Report(c, "call log overflowed");
        bad = 1;
    }
    if (capA.logNum != capB.logNum)
    {
        char msg[64];
        snprintf(msg, sizeof(msg), "call count %d vs %d", capA.logNum, capB.logNum);
        Report(c, msg);
        bad = 1;
    }
    else
    {
        logLinesCompared += capA.logNum;
        for (i = 0; i < capA.logNum; ++i)
        {
            if (strcmp(capA.log[i], capB.log[i]) != 0)
            {
                char msg[100];
                snprintf(msg, sizeof(msg), "call %d: '%s' vs '%s'", i, capA.log[i], capB.log[i]);
                Report(c, msg);
                bad = 1;
                break;
            }
        }
    }
    (void)bad;
    ++casesRun;
}

static void RunCase(int kind, unsigned int seed, int a, int b)
{
    Case c;


    c.kind = kind;
    c.seed = seed;
    c.a = a;
    c.b = b;
    Run(0, &c, &capA);
    Run(1, &c, &capB);
    Compare(&c);
}

int main(void)
{
    unsigned int seed;
    int a, b;
    int perKind[K_NUM];
    int k;

    for (k = 0; k < K_NUM; ++k)
        perKind[k] = 0;

    printf("=== world\n");
    fflush(stdout);

    for (seed = 1; seed <= 60; ++seed)
    {
        for (a = 0; a < 8; ++a)
        {
            RunCase(K_BVPRE, seed, a, (int)(seed % NG));
            perKind[K_BVPRE]++;
        }
        for (a = 0; a < 6; ++a)
        {
            RunCase(K_BVINSIDE, seed, a, a + (int)seed);
            perKind[K_BVINSIDE]++;
        }
        for (a = 0; a < 4; ++a)
        {
            RunCase(K_BVALL, seed, a, a);
            perKind[K_BVALL]++;
        }
    }

    for (seed = 1; seed <= 40; ++seed)
    {
        for (a = 0; a < 8; ++a)
        {
            for (b = 0; b < 4; ++b)
            {
                RunCase(K_EOTSORT, seed, a, b * 37 + a);
                perKind[K_EOTSORT]++;
            }
            RunCase(K_EOTBUILD, seed, a, a & 3);
            perKind[K_EOTBUILD]++;
            RunCase(K_EOTONE, seed, a, a & 1);
            perKind[K_EOTONE]++;
            RunCase(K_EOTFLAGS, seed, a, a & 1);
            perKind[K_EOTFLAGS]++;
            RunCase(K_ACTIVATE, seed, a, a);
            perKind[K_ACTIVATE]++;
            RunCase(K_VISIBLE, seed, a, a);
            perKind[K_VISIBLE]++;
            RunCase(K_ADDLOADED, seed, a, a);
            perKind[K_ADDLOADED]++;
            RunCase(K_DETACH, seed, a, a);
            perKind[K_DETACH]++;
            RunCase(K_DELOBJ, seed, a, a);
            perKind[K_DELOBJ]++;
            RunCase(K_CHILDREN, seed, a, a);
            perKind[K_CHILDREN]++;
            RunCase(K_UNIVERSE, seed, a, a);
            perKind[K_UNIVERSE]++;
            RunCase(K_CHECKFATHER, seed, a, a * 11);
            perKind[K_CHECKFATHER]++;
            RunCase(K_CHECKGROUP, seed, a, a);
            perKind[K_CHECKGROUP]++;
            RunCase(K_RESOLVEAI, seed, a, a * 7);
            perKind[K_RESOLVEAI]++;
        }
        for (b = 0; b < 128; ++b)
        {
            RunCase(K_ATTACH, seed, (int)(seed % NG), b);
            perKind[K_ATTACH]++;
        }
        for (b = 0; b < 128; ++b)
        {
            RunCase(K_JUSTAFTER, seed, (int)(seed % NG), b);
            perKind[K_JUSTAFTER]++;
        }
    }

    for (k = 0; k < K_NUM; ++k)
        printf("  %-24s %6d cases\n", KindName(k), perKind[k]);
    printf("  %d cases, %d engine calls compared, %lld state bytes compared\n",
           casesRun, logLinesCompared, bytesCompared);
    if (fails)
    {
        printf("  %d case(s) differ\n", fails);
        return 1;
    }
    printf("  no differences\n");
    return 0;
}
