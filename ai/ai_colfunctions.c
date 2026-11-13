#include "../time.h"
#include "ai_colfunctions.h"

// Each ray builtin registers its profiler raster once, on first use.
static int i_RasterRayObjectDist;
static int i_RasterRayObjectVector;
static int i_RasterRaySingleObjectDist;

// A bounding-volume overlap query only answers once the object is in
// the sweep-and-prune list, so the three that ask put it there first.
static void AI_l_EnsureInSnP(tGameObjectMain* gao)
{
    tWorld* world;

    if ((gao->flags2 & 0x80) != 0)
        return;

    world = WOR_gpst_CurrentWorld;
    if (!world)
        return;

    gao->flags2 |= 0x80u;
    INT_SnP_AddObject(gao, world->snp);
    INT_SnP_InsertionSort(world->snp, 0);
}

// Opens a profiler raster for one of the ray builtins, registering it
// the first time through.
static void AI_l_RayProfilerEnter(profilerRasterStructElem* raster, int* registered, char* name)
{
    if (!*registered)
    {
        PRO_RasterAdd(raster, "Trame Loop", "Game Engine", name, 0, 0.0f);
        *registered = 1;
    }
    if (!raster->insideCounter)
        raster->startTick = TIM_GetTicksAsInt32();
    ++raster->insideCounter;
}

// The collision instance a script is addressing, or NULL. Many of these
// builtins spell the guard out; the ones that test the object's 0x600
// flags instead use AI_l_ColInstanceIfFlagged below.
static tColInstance* AI_l_GetColInstance(struct sGameObjectMain* gao)
{
    if (!gao || !gao->extended || !gao->extended->coll)
        return NULL;
    return gao->extended->coll->colInstance;
}

// The same, for the builtins that reach the instance through the
// object's collision-set flags rather than a null chain.
static tColInstance* AI_l_ColInstanceIfFlagged(struct sGameObjectMain* gao)
{
    if ((gao->flags & 0x600) == 0)
        return NULL;
    return gao->extended->coll->colInstance;
}

// A zone number is an index into the set's zone table, which gives the
// index of the instance element that zone currently selects.
static tColInstanceElem* AI_l_ZoneElem(tColInstance* colInstance, uint8_t zone)
{
    return colInstance->selectedInstElemPtrs[colInstance->colSet->zoneIDs[zone]];
}

// A script that gives a ray as a vector rather than a direction and a
// length hands over both: the direction normalised, and the length
// returned.
static float AI_l_RaySplitVector(Vec3* dir, Vec3* ray)
{
    float length;
    float inverse;

    length = MATH_VecLen(ray);
    inverse = (float)(1.0 / length);
    MATH_VecMultScalar(dir, ray, inverse);
    return length;
}

// rand() mapped onto 0..1, exactly as the original computes it.
static float AI_l_Rand01(void)
{
    float raw;

    raw = (float)rand();
    return (float)(raw / 32767.0);
}

// A uniform pick between two bounds, which the random-position builtin
// does once per axis.
static float AI_l_RandBetween(float min, float max)
{
    return (max - min) * AI_l_Rand01() + min;
}

/* The ray-cast kind, which the two general ray builtins share. Its low
   31 bits select the cast and which parts of the collision map take
   part; its top bit is a flag of its own, passed through to the
   collision-map cast unchanged. */
static tGameObjectMain* AI_l_RayCastByKind(tWorld* world, tGameObjectMain* gao,
                                           Vec3* rayV1, Vec3* rayV2, float rayF1,
                                           int flagMustHave, int flagMustNothave,
                                           int whatToTest, int castKind)
{
    unsigned int topFlag;

    topFlag = castKind & 0x80000000;
    switch (castKind & 0x7FFFFFFF)
    {
    case 0:
        return COL_Visual_RayCast(world, gao, rayV1, rayV2, rayF1,
                                  flagMustHave, flagMustNothave, whatToTest, 0, 1);
    case 1:
        return COL_ColMap_RayCast(world, gao, rayV1, rayV2, rayF1,
                                  flagMustHave, flagMustNothave, whatToTest,
                                  0, 1, 1, 1, 0, topFlag);
    case 2:
        return COL_ColMap_RayCast(world, gao, rayV1, rayV2, rayF1,
                                  flagMustHave, flagMustNothave, whatToTest,
                                  0, 1, 0, 1, 0, topFlag);
    case 3:
        return COL_ColMap_RayCast(world, gao, rayV1, rayV2, rayF1,
                                  flagMustHave, flagMustNothave, whatToTest,
                                  0, 1, 1, 0, 0, topFlag);
    case 4:
        return COL_ColMap_RayCast(world, gao, rayV1, rayV2, rayF1,
                                  flagMustHave, flagMustNothave, whatToTest,
                                  0, 1, 1, 1, 1, topFlag);
    case 5:
        return COL_ColMap_RayCast(world, gao, rayV1, rayV2, rayF1,
                                  flagMustHave, flagMustNothave, whatToTest,
                                  0, 1, 0, 1, 1, topFlag);
    default:
        return NULL;
    }
}

int AI_EvalFunc_COLBVGetNbOverlaps_C(tGameObjectMain* gao)
{
    AI_l_EnsureInSnP(gao);
    return gao->extended->coll->detectionList->overlapNum;
}

tAINode* AI_EvalFunc_COLBVGetNbOverlaps(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLBVGetNbOverlaps_C(gaoUltra));
    return node + 1;
}

struct sGameObjectMain* AI_EvalFunc_COLBVGetFirstOverlap_C(struct sGameObjectMain* gao, int flagMustHave, int flagMustNothave, int whatToTest)
{
    tGameObjectCollDetectionList* overlaps;
    tGameObjectMain** entry;
    tGameObjectMain** end;

    AI_l_EnsureInSnP(gao);

    // the original compared these two cursors as unsigned ints, which
    // is the same thing on a 32-bit target
    overlaps = gao->extended->coll->detectionList;
    end = &overlaps->detectionListGaos[overlaps->overlapNum];
    for (entry = overlaps->detectionListGaos; entry < end; ++entry)
    {
        if (OBJ_b_test_flag(*entry, flagMustHave, flagMustNothave, whatToTest))
            return *entry;
    }
    return NULL;
}

tAINode* AI_EvalFunc_COLBVGetFirstOverlap(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* overlap;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    overlap = AI_EvalFunc_COLBVGetFirstOverlap_C(gaoUltra, flagMustHave, flagMustNothave, whatToTest);
    AI_PutObjectToStack(&overlap, AI_gaw_EnumLink[40]);
    return node + 1;
}

// The list the script gets is terminated with a null entry as well as
// counted.
int AI_EvalFunc_COLBVGetAllOverlaps_C(struct sGameObjectMain* gao, struct sGameObjectMain** outList, int flagMustHave, int flagMustNothave, int whatToTest)
{
    tGameObjectCollDetectionList* overlaps;
    tGameObjectMain** entry;
    tGameObjectMain** end;
    int kept;

    AI_l_EnsureInSnP(gao);

    overlaps = gao->extended->coll->detectionList;
    end = &overlaps->detectionListGaos[overlaps->overlapNum];
    kept = 0;
    for (entry = overlaps->detectionListGaos; entry < end; ++entry)
    {
        if (OBJ_b_test_flag(*entry, flagMustHave, flagMustNothave, whatToTest))
            outList[kept++] = *entry;
    }
    outList[kept] = NULL;
    return kept;
}

tAINode* AI_EvalFunc_COLBVGetAllOverlaps(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLBVGetAllOverlaps_C(gaoUltra, (tGameObjectMain**)var.ptr,
                                                       flagMustHave, flagMustNothave, whatToTest));
    return node + 1;
}

BOOL AI_EvalFunc_COLBVObjectObject_C(struct sGameObjectMain* gao, struct sGameObjectMain* other)
{
    if (!other)
        return FALSE;
    return COL_b_BVOverlap(gao, other, FALSE, FALSE, TRUE);
}

tAINode* AI_EvalFunc_COLBVObjectObject(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* other;

    other = (tGameObjectMain*)AI_GetObjectFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLBVObjectObject_C(gaoUltra, other));
    return node + 1;
}

// gao is unused: the cast is against the world's activated objects.
tGameObjectMain* AI_EvalFunc_COLRayBVDist_C(tGameObjectMain* gao, Vec3* rayV1, Vec3* rayV2, float rayF1, int flagMustHave, int flagMustNothave, int whatToTest)
{
    return COL_RayBV(&WOR_gpst_CurrentWorld->activatedObjectsTable4PF,
                     rayV1, rayV2, rayF1, flagMustHave, flagMustNothave, whatToTest, 1);
}

tAINode* AI_EvalFunc_COLRayBVDist(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* hit;
    Vec3 rayV1;
    Vec3 rayV2;
    float rayF1;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    rayF1 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&rayV2);
    AI_GetVecFromStack(&rayV1);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    hit = AI_EvalFunc_COLRayBVDist_C(gaoUltra, &rayV1, &rayV2, rayF1,
                                     flagMustHave, flagMustNothave, whatToTest);
    AI_PutObjectToStack(&hit, AI_gaw_EnumLink[40]);
    return node + 1;
}

tGameObjectMain* AI_EvalFunc_COLRayBVVector_C(tGameObjectMain* gao, Vec3* rayV1, Vec3* ray, int flagMustHave, int flagMustNothave, int whatToTest)
{
    tWorld* world;
    Vec3 rayV2;
    float rayF1;

    world = WOR_gpst_CurrentWorld;
    rayF1 = AI_l_RaySplitVector(&rayV2, ray);
    return COL_RayBV(&world->activatedObjectsTable4PF,
                     rayV1, &rayV2, rayF1, flagMustHave, flagMustNothave, whatToTest, 1);
}

tAINode* AI_EvalFunc_COLRayBVVector(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* hit;
    Vec3 rayV1;
    Vec3 ray;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    AI_GetVecFromStack(&ray);
    AI_GetVecFromStack(&rayV1);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    hit = AI_EvalFunc_COLRayBVVector_C(gaoUltra, &rayV1, &ray,
                                       flagMustHave, flagMustNothave, whatToTest);
    AI_PutObjectToStack(&hit, AI_gaw_EnumLink[40]);
    return node + 1;
}

int AI_EvalFunc_COLCollideType_C(struct sGameObjectMain* gao, int reportFilter)
{
    return COL_Report_CollideType(gao, WOR_gpst_CurrentWorld, reportFilter);
}

tAINode* AI_EvalFunc_COLCollideType(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int reportFilter;

    reportFilter = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLCollideType_C(gaoUltra, reportFilter));
    return node + 1;
}

// Note the return: the second arm hands back the address of a local,
// which no caller reads. Preserved as written.
Vec3* AI_EvalFunc_COLNormalGet_C(struct sGameObjectMain* gao, int reportFilter, Vec3* normal)
{
    Vec3 reported;

    COL_Report_GetNormal(gao, WOR_gpst_CurrentWorld, &reported, reportFilter);
    if (MATH_b_NulVector(&reported))
    {
        *normal = MATH_gst_NulVector;
        return normal;
    }
    *normal = reported;
    return &reported;
}

tAINode* AI_EvalFunc_COLNormalGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 normal;
    int reportFilter;

    reportFilter = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLNormalGet_C(gaoUltra, reportFilter, &normal);
    AI_PutVecToStack(&normal);
    return node + 1;
}

struct sGameObjectMain* AI_EvalFunc_COLRayObjectDist_C(struct sGameObjectMain* gao, Vec3* rayV1, Vec3* rayV2, float rayF1, int flagMustHave, int flagMustNothave, int whatToTest, int castKind)
{
    return AI_l_RayCastByKind(WOR_gpst_CurrentWorld, gao, rayV1, rayV2, rayF1,
                              flagMustHave, flagMustNothave, whatToTest, castKind);
}

tAINode* AI_EvalFunc_COLRayObjectDist(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* hit;
    Vec3 rayV1;
    Vec3 rayV2;
    float rayF1;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;
    int castKind;

    castKind = AI_GetIntFromStack();
    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    rayF1 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&rayV2);
    AI_GetVecFromStack(&rayV1);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    AI_l_RayProfilerEnter(&PRO_RayObjectDist, &i_RasterRayObjectDist, "Ray Object Dist");
    hit = AI_EvalFunc_COLRayObjectDist_C(gaoUltra, &rayV1, &rayV2, rayF1,
                                         flagMustHave, flagMustNothave, whatToTest, castKind);
    AI_PutObjectToStack(&hit, AI_gaw_EnumLink[40]);
    PRO_RecordTimeDecRef(&PRO_RayObjectDist);
    return node + 1;
}

struct sGameObjectMain* AI_EvalFunc_COLRayObjectVector_C(struct sGameObjectMain* gao, Vec3* rayV1, Vec3* ray, int flagMustHave, int flagMustNothave, int whatToTest, int castKind)
{
    tWorld* world;
    Vec3 rayV2;
    float rayF1;

    world = WOR_gpst_CurrentWorld;
    rayF1 = AI_l_RaySplitVector(&rayV2, ray);
    return AI_l_RayCastByKind(world, gao, rayV1, &rayV2, rayF1,
                              flagMustHave, flagMustNothave, whatToTest, castKind);
}

tAINode* AI_EvalFunc_COLRayObjectVector(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* hit;
    Vec3 rayV1;
    Vec3 ray;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;
    int castKind;

    castKind = AI_GetIntFromStack();
    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    AI_GetVecFromStack(&ray);
    AI_GetVecFromStack(&rayV1);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    AI_l_RayProfilerEnter(&PRO_RayObjectVector, &i_RasterRayObjectVector, "Ray Object Vector");
    hit = AI_EvalFunc_COLRayObjectVector_C(gaoUltra, &rayV1, &ray,
                                           flagMustHave, flagMustNothave, whatToTest, castKind);
    AI_PutObjectToStack(&hit, AI_gaw_EnumLink[40]);
    PRO_RecordTimeDecRef(&PRO_RayObjectVector);
    return node + 1;
}

Vec3* AI_EvalFunc_COLRayPosGet_C(struct sGameObjectMain* gao, Vec3* pos)
{
    *pos = WOR_gpst_CurrentWorld->colRayHitPos;
    return pos;
}

tAINode* AI_EvalFunc_COLRayPosGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pos;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLRayPosGet_C(gaoUltra, &pos);
    AI_PutVecToStack(&pos);
    return node + 1;
}

Vec3* AI_EvalFunc_COLRayNormalGet_C(struct sGameObjectMain* gao, Vec3* normal)
{
    *normal = WOR_gpst_CurrentWorld->rayCastNormalVec;
    return normal;
}

tAINode* AI_EvalFunc_COLRayNormalGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 normal;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLRayNormalGet_C(gaoUltra, &normal);
    AI_PutVecToStack(&normal);
    return node + 1;
}

struct sGameObjectMain* AI_EvalFunc_COLGaoGet_C(struct sGameObjectMain* gao, int reportFilter)
{
    return COL_Report_GetCOBGameObject(gao, WOR_gpst_CurrentWorld, reportFilter);
}

tAINode* AI_EvalFunc_COLGaoGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* collided;
    int reportFilter;

    reportFilter = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    collided = AI_EvalFunc_COLGaoGet_C(gaoUltra, reportFilter);
    AI_PutObjectToStack(&collided, AI_gaw_EnumLink[40]);
    return node + 1;
}

int AI_EvalFunc_COLGMFlagsGet_C(struct sGameObjectMain* gao, int reportFilter)
{
    return COL_GameMaterial_GetFlags(gao, WOR_gpst_CurrentWorld, reportFilter);
}

tAINode* AI_EvalFunc_COLGMFlagsGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int reportFilter;

    reportFilter = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLGMFlagsGet_C(gaoUltra, reportFilter));
    return node + 1;
}

float AI_EvalFunc_COLGMSlideGet_C(struct sGameObjectMain* gao, int reportFilter)
{
    return COL_GameMaterial_GetSlide(gao, WOR_gpst_CurrentWorld, reportFilter);
}

tAINode* AI_EvalFunc_COLGMSlideGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int reportFilter;

    reportFilter = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_COLGMSlideGet_C(gaoUltra, reportFilter));
    return node + 1;
}

float AI_EvalFunc_COLGMReboundGet_C(tGameObjectMain* gao, int reportFilter)
{
    return COL_GameMaterial_GetRebound(gao, WOR_gpst_CurrentWorld, reportFilter);
}

tAINode* AI_EvalFunc_COLGMReboundGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int reportFilter;

    reportFilter = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_COLGMReboundGet_C(gaoUltra, reportFilter));
    return node + 1;
}

int AI_EvalFunc_COLGMSoundGet_C(struct sGameObjectMain* gao, int reportFilter)
{
    return COL_GameMaterial_GetSound(gao, WOR_gpst_CurrentWorld, reportFilter);
}

tAINode* AI_EvalFunc_COLGMSoundGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int reportFilter;

    reportFilter = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLGMSoundGet_C(gaoUltra, reportFilter));
    return node + 1;
}

void AI_EvalFunc_COLGlobalWallCosAngleSet_C(float cosAngle)
{
    COL_gst_GlobalVars.wallCosAngle = cosAngle;
}

tAINode* AI_EvalFunc_COLGlobalWallCosAngleSet(tAINode* node)
{
    float cosAngle;

    cosAngle = AI_GetFloatFromStack();
    AI_EvalFunc_COLGlobalWallCosAngleSet_C(cosAngle);
    return node + 1;
}

void AI_EvalFunc_COLComputeJumpPoint_C(struct sGameObjectMain* gao, float margin, int down, int useLastRayHit, Vec3* point)
{
    COL_ComputeJumpPoint(gao, point, margin, down, useLastRayHit);
}

tAINode* AI_EvalFunc_COLComputeJumpPoint(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 point;
    float margin;
    int down;
    int useLastRayHit;

    useLastRayHit = AI_GetIntFromStack();
    down = AI_GetIntFromStack();
    margin = AI_GetFloatFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLComputeJumpPoint_C(gaoUltra, margin, down, useLastRayHit, &point);
    AI_PutVecToStack(&point);
    return node + 1;
}

void AI_EvalFunc_COLCrossableSet_C(struct sGameObjectMain* gao, int16_t toSet, int16_t mask)
{
    tColInstance* colInstance;

    colInstance = AI_l_GetColInstance(gao);
    if (colInstance)
        colInstance->crossable = toSet | (~mask & colInstance->crossable);
}

tAINode* AI_EvalFunc_COLCrossableSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int16_t toSet;
    int16_t mask;

    mask = AI_GetIntFromStack();
    toSet = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLCrossableSet_C(gaoUltra, toSet, mask);
    return node + 1;
}

int AI_EvalFunc_COLZDMObjectListGet_C(tGameObjectMain* gao, tGameObjectMain** outList, Vec3* pos, int reportFilter)
{
    return COL_Report_GetZDMGameObjectList(gao, WOR_gpst_CurrentWorld, outList, pos, reportFilter);
}

tAINode* AI_EvalFunc_COLZDMObjectListGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    Vec3 pos;
    int reportFilter;

    reportFilter = AI_GetIntFromStack();
    AI_GetVecFromStack(&pos);
    AI_GetVarFromStack(converted, &var);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLZDMObjectListGet_C(gaoUltra, (tGameObjectMain**)var.ptr,
                                                       &pos, reportFilter));
    return node + 1;
}

// The first vector popped is both input and output: it goes in as the
// ray's start and comes back as the computed point.
tAINode* AI_EvalFunc_COLRayCompute(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tWorld* world;
    char converted[52];
    tVarStackElem var;
    unsigned int* out;
    Vec3 point;
    Vec3 rayV1;
    Vec3 rayV2;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;
    int skipRayCast;

    AI_GetVarFromStack(converted, &var);
    out = (unsigned int*)var.ptr;
    skipRayCast = AI_GetIntFromStack();
    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    world = WOR_gpst_CurrentWorld;
    AI_GetVecFromStack(&rayV1);
    point = rayV1;
    AI_GetVecFromStack(&rayV2);

    COL_RayCompute(&point, world, gaoUltra, &rayV2, &rayV1,
                   flagMustHave, flagMustNothave, whatToTest, skipRayCast, out);
    AI_PutVecToStack(&point);
    return node + 1;
}

struct sGameObjectMain* AI_EvalFunc_COLBestWallGaoGet_C(struct sGameObjectMain* gao, Vec3* direction, int* outRank)
{
    return COL_Report_GetBestWallGOInDirection(gao, WOR_gpst_CurrentWorld, direction, 0.0, outRank);
}

tAINode* AI_EvalFunc_COLBestWallGaoGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* wall;
    char converted[52];
    tVarStackElem var;
    Vec3 direction;

    AI_GetVarFromStack(converted, &var);
    AI_GetVecFromStack(&direction);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    wall = AI_EvalFunc_COLBestWallGaoGet_C(gaoUltra, &direction, (int*)var.ptr);
    AI_PutObjectToStack(&wall, AI_gaw_EnumLink[40]);
    return node + 1;
}

int AI_EvalFunc_COLPriorityGet_C(struct sGameObjectMain* gao)
{
    tColInstance* colInstance;

    colInstance = AI_l_GetColInstance(gao);
    if (!colInstance)
        return 0;
    return (char)colInstance->priority;
}

tAINode* AI_EvalFunc_COLPriorityGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLPriorityGet_C(gaoUltra));
    return node + 1;
}

void AI_EvalFunc_COLPrioritySet_C(struct sGameObjectMain* gao, char priority)
{
    tColInstance* colInstance;

    colInstance = AI_l_GetColInstance(gao);
    if (colInstance)
        colInstance->priority = priority;
}

tAINode* AI_EvalFunc_COLPrioritySet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char priority;

    priority = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLPrioritySet_C(gaoUltra, priority);
    return node + 1;
}

struct sGameObjectMain* AI_EvalFunc_COLCrossObjectGet_C(struct sGameObjectMain* gao, int index)
{
    return WOR_gpst_CurrentWorld->collCrossObjects[index];
}

tAINode* AI_EvalFunc_COLCrossObjectGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* crossed;
    int index;

    index = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    crossed = AI_EvalFunc_COLCrossObjectGet_C(gaoUltra, index);
    AI_PutObjectToStack(&crossed, AI_gaw_EnumLink[40]);
    return node + 1;
}

int AI_EvalFunc_COLCrossObjectNumber_C()
{
    return WOR_gpst_CurrentWorld->colCrossObjectNumber;
}

tAINode* AI_EvalFunc_COLCrossObjectNumber(tAINode* node)
{
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLCrossObjectNumber_C());
    return node + 1;
}

void AI_EvalFunc_COLPointGet_C(struct sGameObjectMain* gao, int reportFilter, Vec3* point)
{
    Vec3* reported;

    reported = COL_Report_GetCollidedPoint(gao, WOR_gpst_CurrentWorld, reportFilter);
    if (reported)
        *point = *reported;
    else
        *point = MATH_gst_NulVector;
}

tAINode* AI_EvalFunc_COLPointGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 point;
    int reportFilter;

    reportFilter = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLPointGet_C(gaoUltra, reportFilter, &point);
    AI_PutVecToStack(&point);
    return node + 1;
}

void AI_EvalFunc_COLGMatPointGet_C(tGameObjectMain* gao, int reportFilter, int* outGMat, Vec3* point)
{
    Vec3* reported;

    reported = COL_Report_GMat_GetCollidedPoint(gao, WOR_gpst_CurrentWorld, reportFilter, outGMat);
    if (reported)
        *point = *reported;
    else
        *point = MATH_gst_NulVector;
}

tAINode* AI_EvalFunc_COLGMatPointGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    Vec3 point;
    int reportFilter;

    AI_GetVarFromStack(converted, &var);
    reportFilter = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLGMatPointGet_C(gaoUltra, reportFilter, (int*)var.ptr, &point);
    AI_PutVecToStack(&point);
    return node + 1;
}

int AI_EvalFunc_COLGetAllReportsNormal_C(tGameObjectMain* gao, Vec3* outNormals, int reportFilter)
{
    return COL_Report_GetAllReportsNormal(gao, WOR_gpst_CurrentWorld, outNormals, reportFilter);
}

tAINode* AI_EvalFunc_COLGetAllReportsNormal(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    int reportFilter;

    reportFilter = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLGetAllReportsNormal_C(gaoUltra, (Vec3*)var.ptr, reportFilter));
    return node + 1;
}

// gao is unused: the list is built over the whole world.
int AI_EvalFunc_COLGetAllVisibleObjects_C(struct sGameObjectMain* gao, struct sGameObjectMain** outList, int flagMustHave, int flagMustNothave, int whatToTest)
{
    return COL_VisibleObjectListCreate(WOR_gpst_CurrentWorld, flagMustHave, flagMustNothave,
                                       whatToTest, outList);
}

tAINode* AI_EvalFunc_COLGetAllVisibleObjects(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLGetAllVisibleObjects_C(gaoUltra, (tGameObjectMain**)var.ptr,
                                                           flagMustHave, flagMustNothave, whatToTest));
    return node + 1;
}

struct sGameObjectMain* AI_EvalFunc_COLBestWallAngleGaoGet_C(struct sGameObjectMain* gao, Vec3* direction, float cosAngle, int* outRank)
{
    return COL_Report_GetBestWallGOInDirection(gao, WOR_gpst_CurrentWorld, direction, cosAngle, outRank);
}

tAINode* AI_EvalFunc_COLBestWallAngleGaoGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* wall;
    char converted[52];
    tVarStackElem var;
    Vec3 direction;
    float cosAngle;

    AI_GetVarFromStack(converted, &var);
    cosAngle = AI_GetFloatFromStack();
    AI_GetVecFromStack(&direction);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    wall = AI_EvalFunc_COLBestWallAngleGaoGet_C(gaoUltra, &direction, cosAngle, (int*)var.ptr);
    AI_PutObjectToStack(&wall, AI_gaw_EnumLink[40]);
    return node + 1;
}

int AI_EvalFunc_COLGetReportsNumber_C(struct sGameObjectMain* gao, int reportFilter)
{
    return COL_Report_GetNumberOfReports(gao, WOR_gpst_CurrentWorld, reportFilter);
}

tAINode* AI_EvalFunc_COLGetReportsNumber(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int reportFilter;

    reportFilter = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLGetReportsNumber_C(gaoUltra, reportFilter));
    return node + 1;
}

void AI_EvalFunc_COLGetResultingNormal_C(tGameObjectMain* gao, int reportFilter, Vec3* normal)
{
    COL_Report_ComputeResultingNormal(gao, WOR_gpst_CurrentWorld, reportFilter, normal);
}

tAINode* AI_EvalFunc_COLGetResultingNormal(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 normal;
    int reportFilter;

    reportFilter = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLGetResultingNormal_C(gaoUltra, reportFilter, &normal);
    AI_PutVecToStack(&normal);
    return node + 1;
}

void AI_EvalFunc_COLForceSnPRefresh_C()
{
    COL_ul_SnP_RefreshType = 0;
}

tAINode* AI_EvalFunc_COLForceSnPRefresh(tAINode* node)
{
    AI_EvalFunc_COLForceSnPRefresh_C();
    return node + 1;
}

tGameObjectMain* AI_EvalFunc_COLSpecialBestWallGaoGet_C(struct sGameObjectMain* gao, Vec3* direction, int* outRank)
{
    return COL_Report_GetBestWallGOInDirectionSpecial(gao, WOR_gpst_CurrentWorld, direction, 0.0, outRank);
}

tAINode* AI_EvalFunc_COLSpecialBestWallGaoGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* wall;
    char converted[52];
    tVarStackElem var;
    Vec3 direction;

    AI_GetVarFromStack(converted, &var);
    AI_GetVecFromStack(&direction);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    wall = AI_EvalFunc_COLSpecialBestWallGaoGet_C(gaoUltra, &direction, (int*)var.ptr);
    AI_PutObjectToStack(&wall, AI_gaw_EnumLink[40]);
    return node + 1;
}

// The lookup runs twice, as the original has it, once for the test and
// once for the answer.
int AI_EvalFunc_COLGMIDGet_C(struct sGameObjectMain* gao, int reportFilter)
{
    tWorld* world;

    world = WOR_gpst_CurrentWorld;
    if ((uint16_t)COL_GameMaterial_GetID(gao, world, reportFilter) == 0xFFFF)
        return -1;
    return (uint16_t)COL_GameMaterial_GetID(gao, world, reportFilter);
}

tAINode* AI_EvalFunc_COLGMIDGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int reportFilter;

    reportFilter = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLGMIDGet_C(gaoUltra, reportFilter));
    return node + 1;
}

void AI_EvalFunc_COL_SpecificCrossableSet_C(uint16_t crossable)
{
    COL_guw_UserCrossable = crossable;
}

tAINode* AI_EvalFunc_COL_SpecificCrossableSet(tAINode* node)
{
    uint16_t crossable;

    crossable = AI_GetIntFromStack();
    AI_EvalFunc_COL_SpecificCrossableSet_C(crossable);
    return node + 1;
}

int AI_EvalFunc_COLGetReportsNumberAndRanks_C(struct sGameObjectMain* gao, int* outRanks, int maxRanks, int reportFilter)
{
    return COL_Report_GetNumberOfReportsAndRanks(gao, WOR_gpst_CurrentWorld, reportFilter,
                                                 maxRanks, outRanks);
}

tAINode* AI_EvalFunc_COLGetReportsNumberAndRanks(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    int maxRanks;
    int reportFilter;

    reportFilter = AI_GetIntFromStack();
    maxRanks = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLGetReportsNumberAndRanks_C(gaoUltra, (int*)var.ptr,
                                                               maxRanks, reportFilter));
    return node + 1;
}

int AI_EvalFunc_COLObjectCollide_C(struct sGameObjectMain* gao, struct sGameObjectMain* other)
{
    return COL_Report_b_ObjectsAreColliding(gao, other, WOR_gpst_CurrentWorld);
}

tAINode* AI_EvalFunc_COLObjectCollide(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLObjectCollide_C(gaoUltra,
                     (tGameObjectMain*)AI_GetObjectFromStack()));
    return node + 1;
}

/* One object rather than the world. Only kinds 0 and 0xA do anything:
   the collision-map kinds 1 to 5 report nothing here. The top bit of
   the kind is not consulted, unlike the two general ray builtins. */
struct sGameObjectMain* AI_EvalFunc_COLRaySingleObjectDist_C(struct sGameObjectMain* gao, Vec3* rayV1, Vec3* rayV2, float rayF1, int flagMustHave, int flagMustNothave, int whatToTest, int castKind)
{
    switch (castKind & 0x7FFFFFFF)
    {
    case 0:
        return COL_Visual_RayCast_OneObject(gao, rayV1, rayV2, rayF1,
                                            flagMustHave, flagMustNothave, whatToTest,
                                            0, 1, 0, 0);
    case 0xA:
        return COL_Visual_RayCast_OneObject(gao, rayV1, rayV2, rayF1,
                                            flagMustHave, flagMustNothave, whatToTest,
                                            0, 1, 1, 1);
    default:
        return NULL;
    }
}

tAINode* AI_EvalFunc_COLRaySingleObjectDist(tAINode* node)
{
    tGameObjectMain* target;
    tGameObjectMain* hit;
    Vec3 rayV1;
    Vec3 rayV2;
    float rayF1;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;
    int castKind;

    castKind = AI_GetIntFromStack();
    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    rayF1 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&rayV2);
    AI_GetVecFromStack(&rayV1);
    target = (tGameObjectMain*)AI_GetObjectFromStack();

    AI_l_RayProfilerEnter(&PRO_RayObjectDist, &i_RasterRaySingleObjectDist, "Ray Object Dist");
    hit = AI_EvalFunc_COLRaySingleObjectDist_C(target, &rayV1, &rayV2, rayF1,
                                               flagMustHave, flagMustNothave, whatToTest, castKind);
    AI_PutObjectToStack(&hit, AI_gaw_EnumLink[40]);
    PRO_RecordTimeDecRef(&PRO_RayObjectDist);
    return node + 1;
}

int AI_EvalFunc_COL_IndexReportGet_C(tGameObjectMain* gao, int reportFlagsRequired, int reportFlagsForbidden)
{
    return COL_Report_ul_BestReportGet(gao, WOR_gpst_CurrentWorld,
                                       reportFlagsRequired, reportFlagsForbidden);
}

tAINode* AI_EvalFunc_COL_IndexReportGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int reportFlagsRequired;
    int reportFlagsForbidden;

    reportFlagsForbidden = AI_GetIntFromStack();
    reportFlagsRequired = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COL_IndexReportGet_C(gaoUltra, reportFlagsRequired,
                                                      reportFlagsForbidden));
    return node + 1;
}

void AI_EvalFunc_COL_RayMinDistSet_C(float minDistance)
{
    COL_gb_UseMinDistance = 1;
    COL_gf_MinDistance = minDistance;
}

tAINode* AI_EvalFunc_COL_RayMinDistSet(tAINode* node)
{
    float minDistance;

    minDistance = AI_GetFloatFromStack();
    AI_EvalFunc_COL_RayMinDistSet_C(minDistance);
    return node + 1;
}

BOOL AI_EvalFunc_COLHasGMat_C(struct sGameObjectMain* gao, int reportFilter)
{
    return COL_Report_HasGMat(gao, WOR_gpst_CurrentWorld, reportFilter);
}

tAINode* AI_EvalFunc_COLHasGMat(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int reportFilter;

    reportFilter = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLHasGMat_C(gaoUltra, reportFilter));
    return node + 1;
}

int AI_EvalFunc_COLCrossableGet_C(tGameObjectMain* gao)
{
    tColInstance* colInstance;

    colInstance = AI_l_GetColInstance(gao);
    if (!colInstance)
        return -1;
    return (uint16_t)colInstance->crossable;
}

tAINode* AI_EvalFunc_COLCrossableGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLCrossableGet_C(gaoUltra));
    return node + 1;
}

int AI_EvalFunc_COLDesignGet_C(tGameObjectMain* gao, int reportFilter)
{
    return COL_GetDesign(gao, WOR_gpst_CurrentWorld, reportFilter);
}

tAINode* AI_EvalFunc_COLDesignGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int reportFilter;

    reportFilter = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLDesignGet_C(gaoUltra, reportFilter));
    return node + 1;
}

/* The eight pairing builtins below differ only in the test mode they
   fix. COL_ListCreate reads that last argument either as a sentinel
   selecting the test -- 0xFF ZDE against bounding volume, 0xFE the
   reverse, 0xFD bounding volume against pivot, 0xFC the reverse, 0xFB
   ZDE against pivot, 0xFA the reverse, 0xEF ZDE against bounding-volume
   centre, 0xEE the reverse -- or, for any other value, as the second
   object's zone number for a ZDE-against-ZDE test. COLZDEZDEListGet is
   the one that takes that route, which is why it is the only one whose
   mode comes from the script. */

int AI_EvalFunc_COLZDEZDEListGet_C(struct sGameObjectMain* gao, struct sGameObjectMain** outList, char zone, char otherZone, int flagMustHave, int flagMustNothave, int whatToTest)
{
    return COL_ListCreate(gao, flagMustHave, flagMustNothave, whatToTest, zone,
                          WOR_gpst_CurrentWorld, outList, otherZone);
}

tAINode* AI_EvalFunc_COLZDEZDEListGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    int zone;
    int otherZone;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    otherZone = AI_GetIntFromStack();
    zone = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLZDEZDEListGet_C(gaoUltra, (tGameObjectMain**)var.ptr,
                                                    zone, otherZone,
                                                    flagMustHave, flagMustNothave, whatToTest));
    return node + 1;
}

int AI_EvalFunc_COLZDEBVListGet_C(tGameObjectMain* gao, tGameObjectMain** outList, char zone, int flagMustHave, int flagMustNothave, int whatToTest)
{
    return COL_ListCreate(gao, flagMustHave, flagMustNothave, whatToTest, zone,
                          WOR_gpst_CurrentWorld, outList, 255);
}

tAINode* AI_EvalFunc_COLZDEBVListGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    int zone;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    zone = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLZDEBVListGet_C(gaoUltra, (tGameObjectMain**)var.ptr, zone,
                                                   flagMustHave, flagMustNothave, whatToTest));
    return node + 1;
}

BOOL AI_EvalFunc_COLZDEZDECollide_C(struct sGameObjectMain* gao, struct sGameObjectMain* other, uint8_t zone, uint8_t otherZone)
{
    return COL_ZDE_ZDECollide(gao, zone, otherZone, other);
}

tAINode* AI_EvalFunc_COLZDEZDECollide(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* other;
    uint8_t zone;
    uint8_t otherZone;

    otherZone = AI_GetIntFromStack();
    zone = AI_GetIntFromStack();
    other = (tGameObjectMain*)AI_GetObjectFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLZDEZDECollide_C(gaoUltra, other, zone, otherZone));
    return node + 1;
}

BOOL AI_EvalFunc_COLZDEBVCollide_C(tGameObjectMain* gao, tGameObjectMain* other, uint8_t zone)
{
    return COL_ZDE_BVCollide(gao, zone, other, 0);
}

tAINode* AI_EvalFunc_COLZDEBVCollide(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* other;
    uint8_t zone;

    zone = AI_GetIntFromStack();
    other = (tGameObjectMain*)AI_GetObjectFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLZDEBVCollide_C(gaoUltra, other, zone));
    return node + 1;
}

int AI_EvalFunc_COLZDEPivotCollide_C(struct sGameObjectMain* gao, struct sGameObjectMain* other, uint8_t zone)
{
    return COL_ZDE_PivotCollide(gao, zone, other, 0);
}

tAINode* AI_EvalFunc_COLZDEPivotCollide(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* other;
    uint8_t zone;

    zone = AI_GetIntFromStack();
    other = (tGameObjectMain*)AI_GetObjectFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLZDEPivotCollide_C(gaoUltra, other, zone));
    return node + 1;
}

int AI_EvalFunc_COLZDEPivotListGet_C(struct sGameObjectMain* gao, struct sGameObjectMain** outList, char zone, int flagMustHave, int flagMustNothave, int whatToTest)
{
    return COL_ListCreate(gao, flagMustHave, flagMustNothave, whatToTest, zone,
                          WOR_gpst_CurrentWorld, outList, 0xFB);
}

tAINode* AI_EvalFunc_COLZDEPivotListGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    int zone;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    zone = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLZDEPivotListGet_C(gaoUltra, (tGameObjectMain**)var.ptr, zone,
                                                      flagMustHave, flagMustNothave, whatToTest));
    return node + 1;
}

// No zone: a bounding volume against a pivot does not need one.
int AI_EvalFunc_COLBVPivotListGet_C(struct sGameObjectMain* gao, struct sGameObjectMain** outList, int flagMustHave, int flagMustNothave, int whatToTest)
{
    return COL_ListCreate(gao, flagMustHave, flagMustNothave, whatToTest, 0,
                          WOR_gpst_CurrentWorld, outList, 253);
}

tAINode* AI_EvalFunc_COLBVPivotListGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLBVPivotListGet_C(gaoUltra, (tGameObjectMain**)var.ptr,
                                                     flagMustHave, flagMustNothave, whatToTest));
    return node + 1;
}

int AI_EvalFunc_COLBVZDEListGet_C(tGameObjectMain* gao, tGameObjectMain** outList, char zone, int flagMustHave, int flagMustNothave, int whatToTest)
{
    return COL_ListCreate(gao, flagMustHave, flagMustNothave, whatToTest, zone,
                          WOR_gpst_CurrentWorld, outList, 254);
}

tAINode* AI_EvalFunc_COLBVZDEListGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    int zone;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    zone = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLBVZDEListGet_C(gaoUltra, (tGameObjectMain**)var.ptr, zone,
                                                   flagMustHave, flagMustNothave, whatToTest));
    return node + 1;
}

int AI_EvalFunc_COLPivotZDEListGet_C(tGameObjectMain* gao, tGameObjectMain** outList, char zone, int flagMustHave, int flagMustNothave, int whatToTest)
{
    return COL_ListCreate(gao, flagMustHave, flagMustNothave, whatToTest, zone,
                          WOR_gpst_CurrentWorld, outList, 0xFA);
}

tAINode* AI_EvalFunc_COLPivotZDEListGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    int zone;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    zone = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLPivotZDEListGet_C(gaoUltra, (tGameObjectMain**)var.ptr, zone,
                                                      flagMustHave, flagMustNothave, whatToTest));
    return node + 1;
}

int AI_EvalFunc_COLPivotBVListGet_C(struct sGameObjectMain* gao, struct sGameObjectMain** outList, int flagMustHave, int flagMustNothave, int whatToTest)
{
    return COL_ListCreate(gao, flagMustHave, flagMustNothave, whatToTest, 0,
                          WOR_gpst_CurrentWorld, outList, 252);
}

tAINode* AI_EvalFunc_COLPivotBVListGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLPivotBVListGet_C(gaoUltra, (tGameObjectMain**)var.ptr,
                                                     flagMustHave, flagMustNothave, whatToTest));
    return node + 1;
}

int AI_EvalFunc_COLBVPivotCollide_C(struct sGameObjectMain* gao, struct sGameObjectMain* other)
{
    return COL_BV_PivotCollide(gao, other, FALSE);
}

tAINode* AI_EvalFunc_COLBVPivotCollide(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* other;

    other = (tGameObjectMain*)AI_GetObjectFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLBVPivotCollide_C(gaoUltra, other));
    return node + 1;
}

// The same test with the two objects the other way round.
int AI_EvalFunc_COLPivotBVCollide_C(struct sGameObjectMain* gao, struct sGameObjectMain* other)
{
    return COL_BV_PivotCollide(gao, other, TRUE);
}

tAINode* AI_EvalFunc_COLPivotBVCollide(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* other;

    other = (tGameObjectMain*)AI_GetObjectFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLPivotBVCollide_C(gaoUltra, other));
    return node + 1;
}

int AI_EvalFunc_COLZDEBVCenterCollide_C(tGameObjectMain* gao, tGameObjectMain* other, uint8_t zone)
{
    return COL_ZDE_BVCenterCollide(gao, zone, other, 0);
}

tAINode* AI_EvalFunc_COLZDEBVCenterCollide(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* other;
    uint8_t zone;

    zone = AI_GetIntFromStack();
    other = (tGameObjectMain*)AI_GetObjectFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLZDEBVCenterCollide_C(gaoUltra, other, zone));
    return node + 1;
}

int AI_EvalFunc_COLZDEBVCenterListGet_C(tGameObjectMain* gao, tGameObjectMain** outList, char zone, int flagMustHave, int flagMustNothave, int whatToTest)
{
    return COL_ListCreate(gao, flagMustHave, flagMustNothave, whatToTest, zone,
                          WOR_gpst_CurrentWorld, outList, 0xEF);
}

tAINode* AI_EvalFunc_COLZDEBVCenterListGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    int zone;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    zone = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLZDEBVCenterListGet_C(gaoUltra, (tGameObjectMain**)var.ptr, zone,
                                                         flagMustHave, flagMustNothave, whatToTest));
    return node + 1;
}

int AI_EvalFunc_COLBVCenterZDEListGet_C(tGameObjectMain* gao, tGameObjectMain** outList, char zone, int flagMustHave, int flagMustNothave, int whatToTest)
{
    return COL_ListCreate(gao, flagMustHave, flagMustNothave, whatToTest, zone,
                          WOR_gpst_CurrentWorld, outList, 0xEE);
}

tAINode* AI_EvalFunc_COLBVCenterZDEListGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    int zone;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    zone = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLBVCenterZDEListGet_C(gaoUltra, (tGameObjectMain**)var.ptr, zone,
                                                         flagMustHave, flagMustNothave, whatToTest));
    return node + 1;
}

// An object carried by another keeps its collision matrix in the
// carrier's space, so the position is brought back through the
// reference object's inverse first.
void AI_EvalFunc_COLStartMatrixSet_C(struct sGameObjectMain* gao, Vec3* pos)
{
    tColInstance* colInstance;
    Mtx4x4 inverse;
    Vec3 local;

    colInstance = AI_l_GetColInstance(gao);
    if (!colInstance)
        return;

    if ((gao->flags & 0x400000) != 0)
    {
        MATH_InvertMatrix(&inverse, gao->basic->mtx->refGao2->matrices);
        MATH_TransformVertex(&local, &inverse, pos);
        colInstance->mtx->Pos = local;
    }
    else
    {
        colInstance->mtx->Pos = *pos;
    }
    COL_ul_SnP_RefreshType = 0;
}

tAINode* AI_EvalFunc_COLStartMatrixSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pos;

    AI_GetVecFromStack(&pos);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLStartMatrixSet_C(gaoUltra, &pos);
    return node + 1;
}

// gao is unused: the search is over the whole world.
tGameObjectMain* AI_EvalFunc_COLFirstObjectInRangeGet_C(tGameObjectMain* gao, Vec3* pos, float range, int flagMustHave, int flagMustNothave, int whatToTest)
{
    return COL_GetFirstObjectInRange(WOR_gpst_CurrentWorld, pos, range,
                                     flagMustHave, flagMustNothave, whatToTest);
}

tAINode* AI_EvalFunc_COLFirstObjectInRangeGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* found;
    Vec3 pos;
    float range;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    range = AI_GetFloatFromStack();
    AI_GetVecFromStack(&pos);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    found = AI_EvalFunc_COLFirstObjectInRangeGet_C(gaoUltra, &pos, range,
                                                   flagMustHave, flagMustNothave, whatToTest);
    AI_PutObjectToStack(&found, AI_gaw_EnumLink[40]);
    return node + 1;
}

tAINode* AI_EvalFunc_COLTriangleCollision(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tWorld* world;
    char converted[52];
    tVarStackElem var;
    Vec3 corner1;
    Vec3 corner2;
    Vec3 corner3;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    world = WOR_gpst_CurrentWorld;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    AI_GetVecFromStack(&corner3);
    AI_GetVecFromStack(&corner2);
    AI_GetVecFromStack(&corner1);
    AI_PutIntToStack(COL_TriangleCollision(&corner1, &corner2, &corner3, (Vec3*)var.ptr, world,
                                           flagMustHave, flagMustNothave, whatToTest));
    return node + 1;
}

void AI_EvalFunc_COLSwapToSpecific_C(struct sGameObjectMain* gao, char zone)
{
    COL_SwapToSpecific(gao, zone);
}

tAINode* AI_EvalFunc_COLSwapToSpecific(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char zone;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    zone = AI_GetIntFromStack();
    AI_EvalFunc_COLSwapToSpecific_C(gaoUltra, zone);
    return node + 1;
}

// gao is unused: the list is built over the whole world, nearest first,
// and maxCount caps it.
int AI_EvalFunc_COLDistanceListGet_C(tGameObjectMain* gao, tGameObjectMain** outList, Vec3* pos, float range, int flagMustHave, int flagMustNothave, int whatToTest, char maxCount)
{
    return COL_ListCreateDistance(flagMustHave, flagMustNothave, whatToTest,
                                  WOR_gpst_CurrentWorld, outList, pos, range, maxCount);
}

tAINode* AI_EvalFunc_COLDistanceListGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    Vec3 pos;
    float range;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;
    int maxCount;

    maxCount = AI_GetIntFromStack();
    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    range = AI_GetFloatFromStack();
    AI_GetVecFromStack(&pos);
    AI_GetVarFromStack(converted, &var);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLDistanceListGet_C(gaoUltra, (tGameObjectMain**)var.ptr,
                                                      &pos, range, flagMustHave, flagMustNothave,
                                                      whatToTest, maxCount));
    return node + 1;
}

void AI_EvalFunc_COLSpecificPosSet_C(struct sGameObjectMain* gao, char zone, Vec3* pos)
{
    COL_ZDx_SpecificPosSet(gao, zone, pos);
}

tAINode* AI_EvalFunc_COLSpecificPosSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pos;
    int zone;

    AI_GetVecFromStack(&pos);
    zone = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLSpecificPosSet_C(gaoUltra, zone, &pos);
    return node + 1;
}

void AI_EvalFunc_COLSpecificSizeSet_C(struct sGameObjectMain* gao, uint8_t zone, Vec3* size)
{
    COL_ZDx_SpecificSizeSet(gao, zone, size);
}

tAINode* AI_EvalFunc_COLSpecificSizeSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 size;
    uint8_t zone;

    AI_GetVecFromStack(&size);
    zone = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLSpecificSizeSet_C(gaoUltra, zone, &size);
    return node + 1;
}

int AI_EvalFunc_COLSetDesignGet_C(tGameObjectMain* gao, unsigned int zone)
{
    tColInstance* colInstance;

    colInstance = AI_l_ColInstanceIfFlagged(gao);
    if (!colInstance || zone > 15)
        return 0;
    return AI_l_ZoneElem(colInstance, (uint8_t)zone)->design;
}

tAINode* AI_EvalFunc_COLSetDesignGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int zone;

    zone = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLSetDesignGet_C(gaoUltra, zone));
    return node + 1;
}

float AI_EvalFunc_COLSizeGet_C(struct sGameObjectMain* gao, uint8_t zone)
{
    return COL_ZDx_SizeGet(gao, zone);
}

tAINode* AI_EvalFunc_COLSizeGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    uint8_t zone;

    zone = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_COLSizeGet_C(gaoUltra, zone));
    return node + 1;
}

int AI_EvalFunc_COLTypeGet_C(tGameObjectMain* gao, uint8_t zone)
{
    tColInstance* colInstance;
    uint8_t elemIdx;

    colInstance = AI_l_ColInstanceIfFlagged(gao);
    if (!colInstance || zone > 0xFu)
        return 0;

    elemIdx = colInstance->colSet->zoneIDs[zone];
    if (elemIdx > 0xFu)
        return 0;
    return colInstance->selectedInstElemPtrs[elemIdx]->elemType;
}

tAINode* AI_EvalFunc_COLTypeGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    uint8_t zone;

    zone = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLTypeGet_C(gaoUltra, zone));
    return node + 1;
}

/* The two box getters write the null vector on each failing guard and
   then carry on, so an object without a collision set is dereferenced
   anyway rather than returning. Preserved as written; the missing
   returns are the original's. */
void AI_EvalFunc_COLBoxMaxGet_C(tGameObjectMain* gao, unsigned int zone, Vec3* corner)
{
    tColInstance* colInstance;
    tColInstanceElem* elem;
    uint8_t elemIdx;

    if ((gao->flags & 0x600) == 0 || zone > 0xF)
        *corner = MATH_gst_NulVector;

    colInstance = gao->extended->coll->colInstance;
    elemIdx = colInstance->colSet->zoneIDs[(uint8_t)zone];
    if (elemIdx > 0xFu)
        *corner = MATH_gst_NulVector;

    elem = colInstance->selectedInstElemPtrs[elemIdx];
    if (elem->elemType == 1)
        *corner = ((tColBox*)(elem->elemShapePtr))->pos1;
    else
        *corner = MATH_gst_NulVector;
}

tAINode* AI_EvalFunc_COLBoxMaxGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 corner;
    unsigned int zone;

    zone = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLBoxMaxGet_C(gaoUltra, zone, &corner);
    AI_PutVecToStack(&corner);
    return node + 1;
}

void AI_EvalFunc_COLBoxMinGet_C(tGameObjectMain* gao, unsigned int zone, Vec3* corner)
{
    tColInstance* colInstance;
    tColInstanceElem* elem;
    uint8_t elemIdx;

    if ((gao->flags & 0x600) == 0 || zone > 0xF)
        *corner = MATH_gst_NulVector;

    colInstance = gao->extended->coll->colInstance;
    elemIdx = colInstance->colSet->zoneIDs[(uint8_t)zone];
    if (elemIdx > 0xFu)
        *corner = MATH_gst_NulVector;

    elem = colInstance->selectedInstElemPtrs[elemIdx];
    if (elem->elemType == 1)
        *corner = ((tColBox*)(elem->elemShapePtr))->pos2;
    else
        *corner = MATH_gst_NulVector;
}

tAINode* AI_EvalFunc_COLBoxMinGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 corner;
    unsigned int zone;

    zone = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLBoxMinGet_C(gaoUltra, zone, &corner);
    AI_PutVecToStack(&corner);
    return node + 1;
}

void AI_EvalFunc_COLPosGet_C(struct sGameObjectMain* gao, uint8_t zone, Vec3* pos)
{
    pos->x = 0.0;
    pos->y = 0.0;
    pos->z = 0.0;
    COL_ZDx_PosGet(gao, zone, pos);
}

tAINode* AI_EvalFunc_COLPosGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pos;
    int zone;

    zone = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLPosGet_C(gaoUltra, zone, &pos);
    AI_PutVecToStack(&pos);
    return node + 1;
}

void AI_EvalFunc_COLFlagXSet_C(tGameObjectMain* gao, uint8_t zone)
{
    tColInstance* colInstance;

    colInstance = AI_l_ColInstanceIfFlagged(gao);
    if (colInstance)
        AI_l_ZoneElem(colInstance, zone)->colFlags |= 0x10u;
}

tAINode* AI_EvalFunc_COLFlagXSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    uint8_t zone;

    zone = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLFlagXSet_C(gaoUltra, zone);
    return node + 1;
}

void AI_EvalFunc_COLFlagXReset_C(tGameObjectMain* gao, uint8_t zone)
{
    tColInstance* colInstance;

    colInstance = AI_l_ColInstanceIfFlagged(gao);
    if (colInstance)
        AI_l_ZoneElem(colInstance, zone)->colFlags &= ~0x10u;
}

tAINode* AI_EvalFunc_COLFlagXReset(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    uint8_t zone;

    zone = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLFlagXReset_C(gaoUltra, zone);
    return node + 1;
}

BOOL AI_EvalFunc_COLHasZone_C(struct sGameObjectMain* gao, int zone)
{
    tColInstance* colInstance;

    colInstance = AI_l_GetColInstance(gao);
    if (!colInstance || !colInstance->colSet)
        return FALSE;
    return colInstance->colSet->zoneIDs[zone] < (int)colInstance->zoneNum;
}

tAINode* AI_EvalFunc_COLHasZone(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int zone;

    zone = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLHasZone_C(gaoUltra, zone));
    return node + 1;
}

void AI_EvalFunc_COLGeoColMapPosSet_C(struct sGameObjectMain* gao, uint8_t zone, Vec3* pos)
{
    COL_GeoColMap_PosSet(gao, zone, pos);
}

tAINode* AI_EvalFunc_COLGeoColMapPosSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pos;
    uint8_t zone;

    AI_GetVecFromStack(&pos);
    zone = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLGeoColMapPosSet_C(gaoUltra, zone, &pos);
    return node + 1;
}

void AI_EvalFunc_COLFlagSet_C(struct sGameObjectMain* gao, uint8_t zone, char toSet, char toClear)
{
    tColInstance* colInstance;
    tColInstanceElem* elem;

    colInstance = AI_l_ColInstanceIfFlagged(gao);
    if (!colInstance)
        return;

    elem = AI_l_ZoneElem(colInstance, zone);
    elem->colFlags |= toSet;
    elem->colFlags &= ~toClear;
}

tAINode* AI_EvalFunc_COLFlagSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    uint8_t zone;
    char toSet;
    char toClear;

    toClear = AI_GetIntFromStack();
    toSet = AI_GetIntFromStack();
    zone = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLFlagSet_C(gaoUltra, zone, toSet, toClear);
    return node + 1;
}

void AI_EvalFunc_COLBackFaceSet_C(tGameObjectMain* gao, Vec3* direction)
{
    tColInstance* colInstance;

    colInstance = AI_l_GetColInstance(gao);
    if (colInstance)
    {
        colInstance->backface = 1;
        colInstance->backFaceVec = *direction;
    }
}

tAINode* AI_EvalFunc_COLBackFaceSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 direction;

    AI_GetVecFromStack(&direction);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLBackFaceSet_C(gaoUltra, &direction);
    return node + 1;
}

int AI_EvalFunc_COLGMatReportGet_C(struct sGameObjectMain* gao, int reportFilter)
{
    return COL_Report_ul_GMatReportGet(gao, WOR_gpst_CurrentWorld, reportFilter);
}

tAINode* AI_EvalFunc_COLGMatReportGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int reportFilter;

    reportFilter = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLGMatReportGet_C(gaoUltra, reportFilter));
    return node + 1;
}

/* A volume that is not an axis-aligned box carries a single radius in
   BVMax.x, which is how both size getters and the random-position
   builtin below read it. */
float AI_EvalFunc_BVSizeGet_C(struct sGameObjectMain* gao)
{
    tGameObjectBV* bv;
    float extentX;
    float extentY;
    float extentZ;

    if (!OBJ_BV_IsAABBox(gao->boundingVolume))
        return gao->boundingVolume->BVMax.x;

    bv = gao->boundingVolume;
    extentZ = bv->BVMax.z - bv->BVMin.z;
    extentY = bv->BVMax.y - bv->BVMin.y;
    extentX = bv->BVMax.x - bv->BVMin.x;
    return MATH_f_MaxOf3(extentX, extentY, extentZ) * 0.5f;
}

tAINode* AI_EvalFunc_BVSizeGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_BVSizeGet_C(gaoUltra));
    return node + 1;
}

void AI_EvalFunc_BVRandomPosGet_C(struct sGameObjectMain* gao, int centreOnZ, Vec3* pos)
{
    tGameObjectBV* bv;
    Mtx4x4* absolute;
    Vec3 boxMax;
    Vec3 boxMin;
    Vec3 centre;
    float radius;

    if (OBJ_BV_IsAABBox(gao->boundingVolume))
    {
        MATH_VecAddVec(&boxMax, &gao->boundingVolume->BVMax, &gao->matrices->Pos);
        MATH_VecAddVec(&boxMin, &gao->boundingVolume->BVMin, &gao->matrices->Pos);
        pos->x = AI_l_RandBetween(boxMin.x, boxMax.x);
        pos->y = AI_l_RandBetween(boxMin.y, boxMax.y);
        if (centreOnZ)
            pos->z = (float)((boxMax.z - boxMin.z) * 0.5 + boxMin.z);
        else
            pos->z = AI_l_RandBetween(boxMin.z, boxMax.z);
    }
    else
    {
        bv = gao->boundingVolume;
        absolute = OBJ_pst_GetAbsoluteMatrix(gao);
        MATH_TransformVertexNoScale(&centre, absolute, &bv->BVMin);
        radius = gao->boundingVolume->BVMax.x;
        boxMax.x = centre.x + radius;
        boxMax.y = centre.y + radius;
        boxMax.z = centre.z + radius;
        boxMin.x = centre.x - radius;
        boxMin.y = centre.y - radius;
        boxMin.z = centre.z - radius;
        pos->x = AI_l_RandBetween(boxMin.x, boxMax.x);
        pos->y = AI_l_RandBetween(boxMin.y, boxMax.y);
        if (!centreOnZ) // TODO FIXME: ugxanaz van mint fentebb, csak itt azért nem látszik mert a végérõl hiányzik egy értékadás, így igazából a Z síkra nem ad értéket. Kidebuggolni!!! IDAban megnézve
            rand();
    }
}

tAINode* AI_EvalFunc_BVRandomPosGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pos;
    int centreOnZ;

    centreOnZ = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_BVRandomPosGet_C(gaoUltra, centreOnZ, &pos);
    AI_PutVecToStack(&pos);
    return node + 1;
}

void AI_EvalFunc_BVMinGet_C(struct sGameObjectMain* gao, Vec3* corner)
{
    if (OBJ_BV_IsAABBox(gao->boundingVolume))
        *corner = gao->boundingVolume->BVMinOriginal;
    else
        *corner = MATH_gst_NulVector;
}

tAINode* AI_EvalFunc_BVMinGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 corner;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_BVMinGet_C(gaoUltra, &corner);
    AI_PutVecToStack(&corner);
    return node + 1;
}

void AI_EvalFunc_BVMinSet_C(struct sGameObjectMain* gao, Vec3* corner)
{
    if (OBJ_BV_IsAABBox(gao->boundingVolume))
    {
        gao->boundingVolume->BVMinOriginal = *corner;
        gao->boundingVolume->BVMin = *corner;
    }
}

// The flag is raised whether or not the volume is a box.
tAINode* AI_EvalFunc_BVMinSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 corner;

    AI_GetVecFromStack(&corner);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    gaoUltra->flags2 |= 0x2000000u;
    AI_EvalFunc_BVMinSet_C(gaoUltra, &corner);
    return node + 1;
}

void AI_EvalFunc_BVMaxGet_C(struct sGameObjectMain* gao, Vec3* corner)
{
    if (OBJ_BV_IsAABBox(gao->boundingVolume))
        *corner = gao->boundingVolume->BVMaxOriginal;
    else
        *corner = MATH_gst_NulVector;
}

tAINode* AI_EvalFunc_BVMaxGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 corner;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_BVMaxGet_C(gaoUltra, &corner);
    AI_PutVecToStack(&corner);
    return node + 1;
}

void AI_EvalFunc_BVMaxSet_C(struct sGameObjectMain* gao, Vec3* corner)
{
    if (OBJ_BV_IsAABBox(gao->boundingVolume))
    {
        gao->boundingVolume->BVMaxOriginal = *corner;
        gao->boundingVolume->BVMax = *corner;
    }
}

tAINode* AI_EvalFunc_BVMaxSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 corner;

    AI_GetVecFromStack(&corner);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    gaoUltra->flags2 |= 0x2000000u;
    AI_EvalFunc_BVMaxSet_C(gaoUltra, &corner);
    return node + 1;
}

float AI_EvalFunc_BVSizeGetMin_C(tGameObjectMain* gao)
{
    tGameObjectBV* bv;
    float extentX;
    float extentY;
    float extentZ;

    if (!OBJ_BV_IsAABBox(gao->boundingVolume))
        return gao->boundingVolume->BVMax.x;

    bv = gao->boundingVolume;
    extentZ = bv->BVMax.z - bv->BVMin.z;
    extentY = bv->BVMax.y - bv->BVMin.y;
    extentX = bv->BVMax.x - bv->BVMin.x;
    return MATH_f_MinOf3(extentX, extentY, extentZ) * 0.5f;
}

tAINode* AI_EvalFunc_BVSizeGetMin(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_BVSizeGetMin_C(gaoUltra));
    return node + 1;
}

void AI_EvalFunc_BVRecompute_C(tGameObjectMain* gao)
{
    OBJ_ComputeBV(gao, 0);
}

tAINode* AI_EvalFunc_BVRecompute(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_BVRecompute_C(gaoUltra);
    return node + 1;
}

/* Both activation setters take the same pair: every bit of flags is
   set, and the bits named by mask that flags does not carry are
   cleared. */
void AI_EvalFunc_COLSetActivationSet_C(struct sGameObjectMain* gao, int16_t flags, int16_t mask)
{
    tColInstance* colInstance;

    colInstance = AI_l_GetColInstance(gao);
    if (colInstance)
        colInstance->activationFlags = flags | (~mask & colInstance->activationFlags);
}

tAINode* AI_EvalFunc_COLSetActivationSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int16_t flags;
    int16_t mask;

    mask = AI_GetIntFromStack();
    flags = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLSetActivationSet_C(gaoUltra, flags, mask);
    return node + 1;
}

int AI_EvalFunc_COLSetActivationGet_C(struct sGameObjectMain* gao)
{
    if ((gao->flags & 0x600) == 0)
        return 0;
    return gao->extended->coll->colInstance->activationFlags;
}

tAINode* AI_EvalFunc_COLSetActivationGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLSetActivationGet_C(gaoUltra));
    return node + 1;
}

void AI_EvalFunc_COLMapActivationSet_C(struct sGameObjectMain* gao, uint8_t flags, char mask)
{
    tColMap* colMap;

    if (!gao || !gao->extended || !gao->extended->coll ||
        !gao->extended->coll->colMap || (gao->flags & 0x100) == 0)
        return;

    colMap = gao->extended->coll->colMap;
    colMap->activation = flags | (~mask & colMap->activation);
}

tAINode* AI_EvalFunc_COLMapActivationSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    uint8_t flags;
    char mask;

    mask = AI_GetIntFromStack();
    flags = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLMapActivationSet_C(gaoUltra, flags, mask);
    return node + 1;
}

int AI_EvalFunc_COLMapActivationGet_C(struct sGameObjectMain* gao)
{
    if ((gao->flags & 0x100) == 0)
        return 0;
    return gao->extended->coll->colMap->activation;
}

tAINode* AI_EvalFunc_COLMapActivationGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_COLMapActivationGet_C(gaoUltra));
    return node + 1;
}

// The script passes the maximum first and the minimum second.
void AI_EvalFunc_COLLODSet_C(struct sGameObjectMain* gao, char maxLod, char minLod)
{
    if ((gao->flags & 0x600) != 0)
    {
        gao->extended->coll->colInstance->minLod = minLod;
        gao->extended->coll->colInstance->maxLod = maxLod;
    }
}

tAINode* AI_EvalFunc_COLLODSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char maxLod;
    char minLod;

    minLod = AI_GetIntFromStack();
    maxLod = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLLODSet_C(gaoUltra, maxLod, minLod);
    return node + 1;
}

void AI_EvalFunc_COLWallPush_C(tGameObjectMain* gao, int on)
{
    if ((gao->flags & 0x600) != 0)
        gao->extended->coll->colInstance->wallPush = on != 0;
}

tAINode* AI_EvalFunc_COLWallPush(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int on;

    on = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLWallPush_C(gaoUltra, on);
    return node + 1;
}
