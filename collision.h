#pragma once
#include "common.h"

typedef struct sColCylinder
{
    Vec3 center;
    float height;
    float radius;
}  tColCylinder;


typedef struct sColSphere
{
    Vec3 center;
    float radius;
} tColSphere;

typedef struct sColBox
{
    Vec3 pos1;
    Vec3 pos2;
} tColBox;


typedef struct sColInstanceElem
{
    byte colFlags;
    byte elemType;
    byte unk1;
    byte design;
    void *elemShapePtr;
} tColInstanceElem;

typedef struct sColInstSet
{
    byte instElemNum;
    byte unk1;
    int16_t instanceNum;
    struct sColInstanceElem* colInstElems;
    byte* zoneIDs;
    int unk2;
} tColInstSet;

typedef struct sColInstance
{
    struct sColInstSet* colSet;
    byte zoneNum;
    byte sharedZoneNum;
    byte instElemNum;
    byte priority;
    struct sColInstanceElem** selectedInstElemPtrs;
    struct sColInstanceElem* colInstElems;
    uint16_t activationFlags;
    uint16_t instElemBitmask;
    int16_t crossable;
    int16_t flag;
    byte minLod;
    byte maxLod;
    byte unk1;
    byte backface;
    int wallPush;
    Mtx4x4* mtx;
    Vec3 backFaceVec;
} tColInstance;

typedef struct sCobGameMaterialElems
{
    int flags;
    float slide;
    float rebound;
    byte sound;
    byte b2; // read but unused
    int16_t id;
} tCobGameMaterialElems;

typedef struct sCobGameMaterial
{
    int num;
    int* ptrInts;
    struct sCobGameMaterialElems* elems;
    int gameMaterialUsageCnt;
} tCobGameMaterial;

typedef struct sColCollisionObject
{
    byte objectType;
    byte flags;
    int16_t usageCount;
    struct sCobGameMaterial* gameMaterial;
    void *objectPtrByType;
} tColCollisionObject;

typedef struct sColMap
{
    byte colObjectID;
    uint8_t activation;
    byte b3; // read but unused
    byte b4; // read but unused
    struct sColCollisionObject** collisionObjectPtr;
} tColMap;

typedef struct sCobMeshTriangleElem
{
    uint16_t v1;
    uint16_t v2;
    uint16_t v3;
    uint16_t proximity1;
    uint16_t proximity2;
    uint16_t proximity3;
} tCobMeshTriangleElem;

typedef struct sCobMeshChunkElem
{
    uint16_t elemNum;
    byte designVal;
    int16_t flags;
    byte _pad[2];
    int gameMaterialElemID;
    struct sCobMeshTriangleElem* triangleElems;
} tCobMeshChunkElem;

typedef struct sCobOK3AABBoxElemChunkElem
{
    uint16_t triangleChunkID;
    uint16_t num;
    uint16_t* triangleChunkElemIDs;
} tCobOK3AABBoxElemChunkElem;

typedef struct sCobOK3AABBoxElem
{
    Vec3 vec1;
    Vec3 vec2;
    uint32_t num;
    struct sCobOK3AABBoxElemChunkElem* chunkElems;
} tCobOK3AABBoxElem;

typedef struct sCobOK3
{
    uint32_t num;
    tCobOK3AABBoxElem* aaBoxes;
    int* faceTagInts;
} tCobOK3;

typedef struct sCobMeshObjectMesh
{
    unsigned int vertexCount;
    unsigned int faceCount;
    unsigned int meshChunkElemNum;
    Vec3* vertices;
    Vec3* normals;
    struct sCobMeshChunkElem* meshChunkElems;
    struct sCobOK3* ok3;
} tCobMeshObjectMesh;

typedef struct sCobNormalObjectGeneric
{
    void* enclosurePtr;
    int gameMaterialElemID;
} tCobNormalObjectGeneric;

/*
Flags: 
- 0x1 triangle based collision
- 0x2 we are colliding with segment between vertex 1 and 2 
- 0x4 we are colliding with segment between vertex 1 and 3
- 0x8 we are colliding with segment between vertex 2 and 3
- 0x10 dynamic hit collision
- 0x20 normal collision
- 0x40 ground type collision
- 0x80 wall type collision
- 0x800 slippery edge collision (valid with every flag above)
- 0x10000 we are colliding with a corner of collision geometry
- 0x20000 slippery edge 2. collision (slipperier than 0x800; valid with every flag above)
*/
typedef struct sRecordElem
{
    struct sGameObjectMain* collisionInstanceGameObject;
    struct sGameObjectMain* collisionMapGameObject;
    Vec3 collidedPoint;
    Vec3 recalVec;
    Vec3 normalVec;
    struct sCobGameMaterialElems* gameMaterial;
    int flags;
    int triangleChunkElemID;
    int16_t triangleChunkID;
    byte designVal;
} tRecordElem;

typedef struct sColGlobal
{
    int num;
    struct sWorld* colWorld;
    struct sGameObjectMain* colInstanceGao;
    struct sColInstanceElem* collInstanceElem;
    struct sColInstance* colInstance;
    struct sColInstanceElem** colInstanceElemPtrs;
    Mtx4x4* colInstMatrix;
    Mtx4x4* colInstGaoMatrices;
    Vec3 accelVecInZDMZDR; // speedVec
    Vec3 centerPos;
    Vec3 centerPosPrevious;
    Vec3 dynCenterPos;
    float dynRadius;
    float radius;
    Vec3 moveVec;
    Vec3 moveVecUnit;
    struct sGameObjectMain* colMapGao;
    byte gap1[12];
    struct sColMap* colMap;
    struct sColCollisionObject* collisionObjectPtr;
    struct sCobMeshChunkElem* cobMeshChunkElem;
    struct sCobMeshTriangleElem* cobMeshTriangleElem;
    Mtx4x4* colMapGaoMatrices;
    byte gap2[12];
    Mtx4x4 colMapGaoInvMatrix;
    byte gap3[8];
    Vec3 colMapEdgeNormalVec;
    float wallCosAngle;
    float maxStepSize;
    Vec3 vec6InZDMZDRGravityNeg; //invUnitG
    Vec3* ptrSetToVec6InZDMZDRGravityNeg;
    int recordCountBackup;
    struct sGameObjectMain* gaosNeededToBeRecomputed[50];
    byte gaosNeededToBeRecomputedNum;
    byte gap4[3];
    int firstRecordToRecompute;
    int recomputeNeeded;
    byte gap5[12];
    int COL_gb_SpecialCamera;
    int COL_gb_AfterEngine;
    byte COL_uc_RecomputingLoop;
    byte gap6[3];
} tColGlobal;

typedef struct sColRayGlobal
{
    struct sWorld* worldSetInColMapRayCast;
    struct sGameObjectMain* gao; // closest gao
    struct sGameObjectMain* colMapGao;
    Vec3* sourcePoint;
    Vec3* direction;
    byte gap[4];
    float distance;
    int skipRayCast;
    int skipCrossable;
    int skipCamera;
    int skipFlagX;
    int useUserCrossable;
    int skipVecsTransformationToGaoSpace;
    int* ignoreColMapRayCastResult; // just visual
} tColRayGlobal;


int COL_OK3_Realloc;
tColGlobal COL_gst_GlobalVars;
int COL_gb_UseMinDistance;
float COL_gf_MinDistance;
tColRayGlobal COL_gst_Ray;
uint16_t COL_guw_UserCrossable;
int COL_gb_LastLoop;
int ColMapPoints;

extern int COL_ul_SnP_RefreshType;

void COL_AllocDetectionList(struct sGameObjectMain* a1);
void COL_FreeDetectionList(struct sGameObjectCollMain* a1);
BOOL COL_BV_PivotCollide(struct sGameObjectMain* gao1, struct sGameObjectMain* gao2, BOOL reverse);
void COL_Free(struct sGameObjectMain* a1);
int COL_AllocColMap(struct sGameObjectMain* a1);
void COL_FreeColMap(struct sGameObjectCollMain* a1);
void COL_FreeInstance(struct sGameObjectCollMain* a1);
void COL_DuplicateInstance(struct sGameObjectMain* a1, struct sGameObjectMain* a2);
void COL_DuplicateColMap(struct sGameObjectMain* a1, struct sGameObjectMain* a2);
void COL_ResolveRefs(struct sGameObjectMain* a1);
void COL_Reinit(struct sGameObjectMain* a1);
void COL_MainCall(struct sWorld* a1);
void COL_OneCall(struct sGameObjectMain* a1, int a2);
BOOL COL_b_BVIn(struct sGameObjectMain* a1, struct sGameObjectMain* a2);
BOOL COL_b_BVOverlap(struct sGameObjectMain* gao1, struct sGameObjectMain* gao2, BOOL OBBoxOfFirstGao, BOOL OBBoxOfSecondGao, BOOL useSnP);
void COL_InitModule();
struct sColInstance* COL_ul_CallBackLoadInstance(uint32_t Offset);
struct sColMap* COL_ul_CallBackLoadColMap(uint32_t Offset);
struct sGameObjectMain* COL_RayBV(struct tableDescStruct* a1, Vec3* a2, Vec3* a3, float a4, int a5, int a6, int whatToTest, int a8);
struct sGameObjectMain* COL_Visual_RayCast(struct sWorld* a1, struct sGameObjectMain* a2, Vec3* a3, Vec3* a4, float a5, int flagMustHave, int flagMustNothave, int whatToTest, int* a9, int a10);
struct sGameObjectMain* COL_ColMap_RayCast(struct sWorld* a1, struct sGameObjectMain* a2, Vec3* a3, Vec3* a4, float a5, int a6, int a7, int a8, int* a9, int a10, int a11, int a12, int a13, int a14);
int COL_Report_CollideType(struct sGameObjectMain* a1, struct sWorld* a2, int a3);
void COL_Report_GetNormal(struct sGameObjectMain* a1, struct sWorld* a2, Vec3* a3, int a4);
struct sGameObjectMain* COL_Report_GetCOBGameObject(struct sGameObjectMain* a1, struct sWorld* a2, int a3);
int COL_GameMaterial_GetFlags(struct sGameObjectMain* a1, struct sWorld* a2, int a3);
float COL_GameMaterial_GetSlide(struct sGameObjectMain* a1, struct sWorld* a2, int a3);
float COL_GameMaterial_GetRebound(struct sGameObjectMain* a1, struct sWorld* a2, int a3);
byte COL_GameMaterial_GetSound(struct sGameObjectMain* a1, struct sWorld* a2, int a3);
void COL_ComputeJumpPoint(struct sGameObjectMain* a1, Vec3* a2, float a3, int a4, int a5);
int COL_Report_GetZDMGameObjectList(struct sGameObjectMain* a1, struct sWorld* a2, struct sGameObjectMain** a3, Vec3* a4, int a5);
void COL_RayCompute(Vec3* a1, struct sWorld* a2, struct sGameObjectMain* a3, Vec3* a4, Vec3* a5, int a6, int a7, int a8, int a9, unsigned int* a10);
struct sGameObjectMain* COL_Report_GetBestWallGOInDirection(struct sGameObjectMain* a1, struct sWorld* a2, Vec3* a3, float a4, int* a5);
Vec3* COL_Report_GetCollidedPoint(struct sGameObjectMain* a1, struct sWorld* a2, int a3);
Vec3* COL_Report_GMat_GetCollidedPoint(struct sGameObjectMain* a1, struct sWorld* a2, int a3, int* a4);
int COL_Report_GetAllReportsNormal(struct sGameObjectMain* a1, struct sWorld* a2, Vec3* a3, int a4);
int COL_VisibleObjectListCreate(struct sWorld* a1, int flagMustHave, int flagMustNothave, int whatToTest, struct sGameObjectMain** a5);
int COL_Report_GetNumberOfReports(struct sGameObjectMain* a1, struct sWorld* a2, int a3);
void COL_Report_ComputeResultingNormal(struct sGameObjectMain* a1, struct sWorld* a2, int a3, Vec3* a4);
struct sGameObjectMain* COL_Report_GetBestWallGOInDirectionSpecial(struct sGameObjectMain* a1, struct sWorld* a2, Vec3* a3, float a4, int* a5);
int16_t COL_GameMaterial_GetID(struct sGameObjectMain* a1, struct sWorld* a2, uint16_t a3);
int COL_Report_GetNumberOfReportsAndRanks(struct sGameObjectMain* a1, struct sWorld* a2, int a3, unsigned int a4, int* a5);
int COL_Report_b_ObjectsAreColliding(struct sGameObjectMain* a1, struct sGameObjectMain* a2, struct sWorld* a3);
struct sGameObjectMain* COL_Visual_RayCast_OneObject(struct sGameObjectMain* a1, Vec3* rayV1, Vec3* rayV2, float rayF1, int flagMustHave, int flagMustNothave, int whatToTest, int* type7, int type1, int a10, int type6);
int COL_Report_ul_BestReportGet(struct sGameObjectMain* a1, struct sWorld* a2, int a3, int a4);
BOOL COL_Report_HasGMat(struct sGameObjectMain* a1, struct sWorld* a2, int a3);
byte COL_GetDesign(struct sGameObjectMain* a1, struct sWorld* a2, int32_t a3);
BOOL COL_ZDE_ZDECollide(struct sGameObjectMain* a1, uint8_t a2, uint8_t a3, struct sGameObjectMain* a4);
BOOL COL_ZDE_BVCollide(struct sGameObjectMain* a1, uint8_t a2, struct sGameObjectMain* a3, int a4);
int COL_ZDE_PivotCollide(struct sGameObjectMain* a1, uint8_t id, struct sGameObjectMain* a3, int direction);
int COL_ZDE_BVCenterCollide(struct sGameObjectMain* a1, uint8_t a2, struct sGameObjectMain* a3, int a4);
struct sGameObjectMain* COL_GetFirstObjectInRange(struct sWorld* a1, Vec3* a2, float a3, int a4, int a5, int a6);
int COL_ListCreate(struct sGameObjectMain* a1, int flagMustHave, int flagMustNothave, int whatToTest, char id, struct sWorld* a6, struct sGameObjectMain** a7, char a3);
int COL_TriangleCollision(Vec3* a1, Vec3* src, Vec3* a3, Vec3* a4, struct sWorld* a5, int flagMustHave, int flagMustNothave, int whatToTest);
void COL_SwapToSpecific(struct sGameObjectMain* a1, uint8_t a2);
int COL_ListCreateDistance(int flagMustHave, int flagMustNothave, int whatToTest, struct sWorld* a4, struct sGameObjectMain** a5, Vec3* a6, float a7, char a8);
void COL_ZDx_SpecificPosSet(struct sGameObjectMain* a1, uint8_t a2, Vec3* a3);
void COL_ZDx_SpecificSizeSet(struct sGameObjectMain* a1, uint8_t a2, Vec3* a3);
float COL_ZDx_SizeGet(struct sGameObjectMain* a1, uint8_t a2);
void COL_ZDx_PosGet(struct sGameObjectMain* a1, uint8_t a2, Vec3* a3);
void COL_GeoColMap_PosSet(struct sGameObjectMain* a1, uint8_t a2, Vec3* a3);
int COL_Report_ul_GMatReportGet(struct sGameObjectMain* a1, struct sWorld* a2, int a3);
float col_sqrlen_sub_409560(Vec3* a1, Vec3* a2);

int INT_PointInTriangle(Vec3* a1, Vec3* a2, Vec3* a3, Vec3* a4, Vec3* a5, byte* a6);
BOOL INT_SphereAABBox(Vec3* a1, float a2, Vec3* a3, Vec3* a4);

void COL_OK3_Build(void* aa, int type, int a3);

void REC_MainCall(struct sWorld* a1, int a2);

