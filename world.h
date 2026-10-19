#pragma once
#include "gameobject.h"
#include "view.h"
#include "snp.h"
#include "grid.h"
#include "gfx.h"
#include "common.h"

typedef struct sEOTStructs
{
    tableDescStruct animatedObjectsTablePF;
    tableDescStruct dynamicObjectsTablePF;
    tableDescStruct AICallbacksTablePF;
    tableDescStruct collMapObjectsTablePF;
    tableDescStruct collInstanceZDMGameObjectsPF;
    tableDescStruct collInstanceZDEGameObjectsPF;
    tableDescStruct eventGameObjectsTablePF;
    tableDescStruct objectsWithHiearchyTablePF;
} tEOTStructs;

typedef struct sDelObj
{
    struct sGameObjectMain* gao;
    int* flash;
} tDelObj;


typedef struct sWorld
{
    int gaoTableMaxElemNum;
    int viewNum;
    int visibilityCallNeeded;
    int tablesManagerAndEOTNeeded;
    int SnPRefreshNeeded;
    byte gap0[4];
    tableDescStruct objectTablePF;
    tableDescStruct worldGROTable;
    tableDescStruct worldMaterialTable;
    tSnp* snp;
    struct sViewElem* viewPtr;
    struct sViewElem* currViewElem;
    tableDescStruct visibleGameObjectsPF;
    tableDescStruct activatedObjectsTable4PF;
    tableDescStruct lightObjectsTablePF;
    tableDescStruct objectsWithBVMaskTablePF;
    struct sEOTStructs eot;
    int ambientColor1;
    int clearColor;
    struct sWayAllNetworks* wayPtr;
    char worldName[60];
    struct sWorldGridStruct* grid1Ptr;
    struct sWorldGridStruct* grid2Ptr;
    int ambientColor2;
    byte gap3[4];
    Mtx4x4 unkMatrix;
    int recordCount;
    struct sGameObjectMain* collCrossObjects[16];
    int colCrossObjectNumber;
    struct sRecordElem records[1000];
    struct sGameObjectMain* rayCastGao;
    Vec3 colRayHitPos;
    Vec3 rayCastNormalVec;
    byte _pad;
    byte rayCastCobMeshChunkElemsDesignVal;
    __int16 rayCastTriangleChunkID;
    int rayCastTriangleChunkElemID;
    int rayCastTriangleChunkLastElemID;
    struct sCobGameMaterialElems* rayCastCobGmat;
    int aiCallCount;
    int dynCallCount;
    int hierarchyCallCount;
    struct sGfxElem* gfxCurrentList;
    struct sGfxElem* gfxCurrentListSortedAfterAE;
    int deletedObjNum;
    struct sDelObj deletedObjs[400];
    int distanceToHide;
    int worldKey;
    byte gap[12];
} tWorld;

typedef struct sUniverse
{
    struct sAI* univAIInstance;
    tableDescStruct univWorldTablePF;
    byte unknownReloadFlag;
    byte _pad[3];
} tUniverse;

typedef struct sBVMasks
{
    struct sGameObjectMain* gao;
    Vec3 planeLeft;
    Vec3 planeUp;
    Vec3 planeRight;
    Vec3 planeDown;
    Vec3 pool[4];
    float is11;
    float is12;
    float is13;
    int is14;
    float is15;
    int flags;
} tBVMasks;

int WOR_gi_NumKilledGO;
struct sGameObjectMain* WOR_AllKilledGO[200];
tWorld* WOR_gpst_CurrentWorld;
tWorld* WOR_gpst_WorldToLoadIn;
tUniverse WOR_gst_Universe;
BOOL WOR_gb_HasAddedMaterial;
BOOL WOR_gb_FirstTrameAfterLoad;

int BVMask_gi_Num;
struct sBVMasks BVMask_ga_Masks[200];

int WOR_gi_IsLoading;

extern int WOR_gb_RealAllocWorld;
extern int WOR_gb_FinalWorld;
extern int WOR_gul_WorldText;
extern int WOR_gul_WorldKey; // unused?

void WOR_ActivateObjectIfFlagsAllow(struct sGameObjectMain* a1, struct sWorld* a2);
void WOR_World_DetachObject(struct sWorld* a1, struct sGameObjectMain* a2);
void WOR_World_AttachGameObject(struct sWorld* a1, struct sGameObjectMain* a2);
void WOR_World_JustAfterLoadObject(struct sWorld* a1, struct sGameObjectMain* a2, int resolveAI, int attach);
void WOR_DelObj(struct sWorld* a1, struct sGameObjectMain* a2);
int WOR_i_GetAllChildrenOf(struct sWorld* a1, struct sGameObjectMain* a2, struct sGameObjectMain** ret);
void WOR_AddDelObj(struct sWorld* a1, struct sGameObjectMain* a2, int* a3);
struct sWorld* WOR_pst_Universe_AddWorld(int worldKey, int unused1, int resolveAI);
int WOR_l_Universe_GetWorldIndex(struct sWorld* a1);
void WOR_World_ActivateObjects(struct sWorld* a1);
void WOR_World_MakeObjectsVisible(struct sWorld* a1);
void WOR_ResetKillGO();
void WOR_World_Attached(tWorld* a1);
void WOR_Render(struct sWorld* world, struct sGdi_DisplayData* dxdd);
void WOR_Universe_Open(int a1);
void WOR_InitModule();
void WOR_CloseModule();
void WOR_Universe_Close();
void WOR_Universe_DeleteWorld(struct sWorld* a1);
struct sWorld* WOR_pst_Universe_GetActiveWorld(int a1);
void WOR_World_ResolveAIRef(struct sWorld* a1);
struct sWorld* WOR_pst_Universe_AddWorldList(int a1, int worldKey, int unused1);

void EOT_UpdateIdentityFlagsOfTable(tableDescStruct* a1);
void EOT_RemoveObjectFromTableIfBadFlag(tableDescStruct* a1, int a2);
void EOT_SetOfEOT_Close(tEOTStructs* a1);
void EOT_SetOfEOT_Build(tEOTStructs* a1, tableDescStruct* a2);
void EOT_SetOfEOT_OneBuild(tableDescStruct* a1, int a2, tableDescStruct* a3);



