#pragma once
#include "common.h"

struct sGameObjectCollMain;
struct tableDescStruct;

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
    uint8_t colFlags;
    uint8_t elemType;
    uint8_t unk1;
    uint8_t design;
    void *elemShapePtr;
} tColInstanceElem;

typedef struct sColInstSet
{
    uint8_t instElemNum;
    uint8_t unk1;
    int16_t instanceNum;
    struct sColInstanceElem* colInstElems;
    uint8_t* zoneIDs;
    int unk2;
} tColInstSet;

typedef struct sColInstance
{
    struct sColInstSet* colSet;
    uint8_t zoneNum;
    uint8_t sharedZoneNum;
    uint8_t instElemNum;
    uint8_t priority;
    struct sColInstanceElem** selectedInstElemPtrs;
    struct sColInstanceElem* colInstElems;
    uint16_t activationFlags;
    uint16_t instElemBitmask;
    int16_t crossable;
    int16_t flag;
    uint8_t minLod;
    uint8_t maxLod;
    uint8_t unk1;
    uint8_t backface;
    int wallPush;
    Mtx4x4* mtx;
    Vec3 backFaceVec;
} tColInstance;

typedef struct sCobGameMaterialElems
{
    int flags;
    float slide;
    float rebound;
    uint8_t sound;
    uint8_t b2; // read but unused
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
    uint8_t objectType;
    uint8_t flags;
    int16_t usageCount;
    struct sCobGameMaterial* gameMaterial;
    void *objectPtrByType;
} tColCollisionObject;

typedef struct sColMap
{
    uint8_t colObjectID;
    uint8_t activation;
    uint8_t b3; // read but unused
    uint8_t b4; // read but unused
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
    uint8_t designVal;
    int16_t flags;
    uint8_t _pad[2];
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
    uint8_t designVal;
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
    uint8_t gap1[12];
    struct sColMap* colMap;
    struct sColCollisionObject* collisionObjectPtr;
    struct sCobMeshChunkElem* cobMeshChunkElem;
    struct sCobMeshTriangleElem* cobMeshTriangleElem;
    Mtx4x4* colMapGaoMatrices;
    uint8_t gap2[12];
    Mtx4x4 colMapGaoInvMatrix;
    uint8_t gap3[8];
    Vec3 colMapEdgeNormalVec;
    float wallCosAngle;
    float maxStepSize;
    Vec3 vec6InZDMZDRGravityNeg; //invUnitG
    Vec3* ptrSetToVec6InZDMZDRGravityNeg;
    int recordCountBackup;
    struct sGameObjectMain* gaosNeededToBeRecomputed[50];
    uint8_t gaosNeededToBeRecomputedNum;
    uint8_t gap4[3];
    int firstRecordToRecompute;
    int recomputeNeeded;
    uint8_t gap5[12];
    int COL_gb_SpecialCamera;
    int COL_gb_AfterEngine;
    uint8_t COL_uc_RecomputingLoop;
    uint8_t gap6[3];
} tColGlobal;

typedef struct sColRayGlobal
{
    struct sWorld* worldSetInColMapRayCast;
    struct sGameObjectMain* gao; // closest gao
    struct sGameObjectMain* colMapGao;
    Vec3* sourcePoint;
    Vec3* direction;
    uint8_t gap[4];
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

void COL_AllocDetectionList(struct sGameObjectMain* gao);
void COL_FreeDetectionList(struct sGameObjectCollMain* coll);
BOOL COL_BV_PivotCollide(struct sGameObjectMain* gao1, struct sGameObjectMain* gao2, BOOL reverse);
void COL_Free(struct sGameObjectMain* gao);
int COL_AllocColMap(struct sGameObjectMain* gao);
void COL_FreeColMap(struct sGameObjectCollMain* coll);
void COL_FreeInstance(struct sGameObjectCollMain* coll);
void COL_DuplicateInstance(struct sGameObjectMain* dest, struct sGameObjectMain* src);
void COL_DuplicateColMap(struct sGameObjectMain* dest, struct sGameObjectMain* src);
void COL_ResolveRefs(struct sGameObjectMain* gao);
void COL_Reinit(struct sGameObjectMain* gao);
void COL_MainCall(struct sWorld* world);
void COL_OneCall(struct sGameObjectMain* gao, int firstRecord);
BOOL COL_b_BVIn(struct sGameObjectMain* outerGao, struct sGameObjectMain* innerGao);
BOOL COL_b_BVOverlap(struct sGameObjectMain* gao1, struct sGameObjectMain* gao2, BOOL OBBoxOfFirstGao, BOOL OBBoxOfSecondGao, BOOL useSnP);
void COL_InitModule();
struct sColInstance* COL_ul_CallBackLoadInstance(uint32_t Offset);
struct sColMap* COL_ul_CallBackLoadColMap(uint32_t Offset);
struct sGameObjectMain* COL_RayBV(struct tableDescStruct* table, Vec3* rayOrigin, Vec3* rayDir, float rayDistance, int flagMustHave, int flagMustNothave, int whatToTest, int skipCrossObjects);
struct sGameObjectMain* COL_Visual_RayCast(struct sWorld* world, struct sGameObjectMain* skipGao, Vec3* source, Vec3* direction, float maxDist, int flagMustHave, int flagMustNothave, int whatToTest, int* ignoreResult, int skipRayCast);
struct sGameObjectMain* COL_ColMap_RayCast(struct sWorld* world, struct sGameObjectMain* excludeGao, Vec3* rayOrigin, Vec3* rayDir, float rayDistance, int flagMustHave, int flagMustNothave, int whatToTest, int* outClearPath, int skipRayCast, int skipCrossable, int skipCamera, int skipFlagX, int useUserCrossable);
int COL_Report_CollideType(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter);
void COL_Report_GetNormal(struct sGameObjectMain* gao, struct sWorld* world, Vec3* outNormal, int reportFilter);
struct sGameObjectMain* COL_Report_GetCOBGameObject(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter);
int COL_GameMaterial_GetFlags(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter);
float COL_GameMaterial_GetSlide(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter);
float COL_GameMaterial_GetRebound(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter);
uint8_t COL_GameMaterial_GetSound(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter);
void COL_ComputeJumpPoint(struct sGameObjectMain* gao, Vec3* outPoint, float margin, int down, int useLastRayHit);
int COL_Report_GetZDMGameObjectList(struct sGameObjectMain* gao, struct sWorld* world, struct sGameObjectMain** outList, Vec3* direction, int reportFilter);
void COL_RayCompute(Vec3* offsetOut, struct sWorld* world, struct sGameObjectMain* gao, Vec3* source, Vec3* direction, int flagMustHave, int flagMustNothave, int whatToTest, int skipRayCast, unsigned int* resultFlags);
struct sGameObjectMain* COL_Report_GetBestWallGOInDirection(struct sGameObjectMain* gao, struct sWorld* world, Vec3* direction, float minCos, int* outRank);
Vec3* COL_Report_GetCollidedPoint(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter);
Vec3* COL_Report_GMat_GetCollidedPoint(struct sGameObjectMain* gao, struct sWorld* world, int gameMaterialMask, int* outRank);
int COL_Report_GetAllReportsNormal(struct sGameObjectMain* gao, struct sWorld* world, Vec3* outNormals, int reportFilter);
int COL_VisibleObjectListCreate(struct sWorld* world, int flagMustHave, int flagMustNothave, int whatToTest, struct sGameObjectMain** outList);
int COL_Report_GetNumberOfReports(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter);
void COL_Report_ComputeResultingNormal(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter, Vec3* outNormal);
struct sGameObjectMain* COL_Report_GetBestWallGOInDirectionSpecial(struct sGameObjectMain* gao, struct sWorld* world, Vec3* direction, float minCos, int* outRank);
int16_t COL_GameMaterial_GetID(struct sGameObjectMain* gao, struct sWorld* world, uint16_t reportFilter);
int COL_Report_GetNumberOfReportsAndRanks(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter, unsigned int maxRanks, int* outRanks);
int COL_Report_b_ObjectsAreColliding(struct sGameObjectMain* gao1, struct sGameObjectMain* gao2, struct sWorld* world);
struct sGameObjectMain* COL_Visual_RayCast_OneObject(struct sGameObjectMain* gao, Vec3* rayOrigin, Vec3* rayDir, float rayDistance, int flagMustHave, int flagMustNothave, int whatToTest, int* outClearPath, int skipRayCast, int skipBVTest, int skipTransform);
int COL_Report_ul_BestReportGet(struct sGameObjectMain* gao, struct sWorld* world, int flagsWanted, int flagsRejected);
BOOL COL_Report_HasGMat(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter);
uint8_t COL_GetDesign(struct sGameObjectMain* gao, struct sWorld* world, int32_t reportFilter);
BOOL COL_ZDE_ZDECollide(struct sGameObjectMain* gao1, uint8_t id1, uint8_t id2, struct sGameObjectMain* gao2);
BOOL COL_ZDE_BVCollide(struct sGameObjectMain* gao1, uint8_t id, struct sGameObjectMain* gao2, int direction);
int COL_ZDE_PivotCollide(struct sGameObjectMain* gao1, uint8_t id, struct sGameObjectMain* gao2, int direction);
int COL_ZDE_BVCenterCollide(struct sGameObjectMain* gao1, uint8_t id, struct sGameObjectMain* gao2, int direction);
struct sGameObjectMain* COL_GetFirstObjectInRange(struct sWorld* world, Vec3* pos, float range, int flagMustHave, int flagMustNothave, int whatToTest);
int COL_ListCreate(struct sGameObjectMain* gao, int flagMustHave, int flagMustNothave, int whatToTest, char id, struct sWorld* world, struct sGameObjectMain** outList, char mode);
int COL_TriangleCollision(Vec3* corner1, Vec3* corner2, Vec3* corner3, Vec3* outPoint, struct sWorld* world, int flagMustHave, int flagMustNothave, int whatToTest);
void COL_SwapToSpecific(struct sGameObjectMain* gao, uint8_t id);
int COL_ListCreateDistance(int flagMustHave, int flagMustNothave, int whatToTest, struct sWorld* world, struct sGameObjectMain** outList, Vec3* pos, float range, char maxCount);
void COL_ZDx_SpecificPosSet(struct sGameObjectMain* gao, uint8_t id, Vec3* pos);
void COL_ZDx_SpecificSizeSet(struct sGameObjectMain* gao, uint8_t id, Vec3* size);
float COL_ZDx_SizeGet(struct sGameObjectMain* gao, uint8_t id);
void COL_ZDx_PosGet(struct sGameObjectMain* gao, uint8_t id, Vec3* outPos);
void COL_GeoColMap_PosSet(struct sGameObjectMain* gao, uint8_t cobIndex, Vec3* pos);
int COL_Report_ul_GMatReportGet(struct sGameObjectMain* gao, struct sWorld* world, int gameMaterialMask);
float col_sqrlen_sub_409560(Vec3* a1, Vec3* a2);

int INT_PointInTriangle(Vec3* point, Vec3* p1, Vec3* p2, Vec3* p3, Vec3* normal, uint8_t* edgeOut);
BOOL INT_SphereAABBox(Vec3* center, float radius, Vec3* boxMin, Vec3* boxMax);

void COL_OK3_Build(void* aa, int type, int a3);

void REC_MainCall(struct sWorld* world, int firstRecord);

