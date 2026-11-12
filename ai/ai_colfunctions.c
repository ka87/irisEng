#include "../time.h"
#include "ai_colfunctions.h"

int i_Raster_741;
int i_Raster_774;
int i_Raster_758;

int AI_EvalFunc_COLBVGetNbOverlaps_C(tGameObjectMain* gao)
{
    tWorld* v2;

    if ((gao->flags2 & 0x80) == 0)
    {
        v2 = WOR_gpst_CurrentWorld;
        if (WOR_gpst_CurrentWorld)
        {
            gao->flags2 |= 0x80u;
            INT_SnP_AddObject(gao, v2->snp);
            INT_SnP_InsertionSort(v2->snp, 0);
        }
    }
    return gao->extended->coll->detectionList->overlapNum;
}

tAINode* AI_EvalFunc_COLBVGetNbOverlaps(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLBVGetNbOverlaps_C(v3);
    AI_PutIntToStack(v1);
    return node + 1;
}

struct sGameObjectMain* AI_EvalFunc_COLBVGetFirstOverlap_C(struct sGameObjectMain* gao, int flagMustHave, int flagMustNothave, int whatToTest)
{
    tWorld* v5;
    tGameObjectMain** v6;
    tGameObjectCollDetectionList* v7;
    unsigned int v8;

    if ((gao->flags2 & 0x80) == 0)
    {
        v5 = WOR_gpst_CurrentWorld;
        if (WOR_gpst_CurrentWorld)
        {
            gao->flags2 |= 0x80u;
            INT_SnP_AddObject(gao, v5->snp);
            INT_SnP_InsertionSort(v5->snp, 0);
        }
    }
    v7 = gao->extended->coll->detectionList;
    v6 = v7->detectionListGaos;
    v8 = (unsigned int)&v7->detectionListGaos[v7->overlapNum];
    while ((unsigned int)v6 < v8)
    {
        if (OBJ_b_test_flag(*v6, flagMustHave, flagMustNothave, whatToTest))
            return *v6;
        ++v6;
    }
    return NULL;
}

tAINode* AI_EvalFunc_COLBVGetFirstOverlap(tAINode* node)
{
    int v2[3];
    tGameObjectMain* v3; 
    int flagMustHave;
    int flagMustNothave;
    tGameObjectMain* v6;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v6 = v3;
    v2[0] = (int)AI_EvalFunc_COLBVGetFirstOverlap_C(v3, flagMustHave, flagMustNothave, whatToTest);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return node + 1;
}

int AI_EvalFunc_COLBVGetAllOverlaps_C(struct sGameObjectMain* gao, struct sGameObjectMain** a2, int flagMustHave, int flagMustNothave, int whatToTest)
{
    tWorld* v6;
    tGameObjectMain** Src;
    tGameObjectCollDetectionList* v8;
    tGameObjectMain** v9;
    int v10;

    if ((gao->flags2 & 0x80) == 0)
    {
        v6 = WOR_gpst_CurrentWorld;
        if (WOR_gpst_CurrentWorld)
        {
            gao->flags2 |= 0x80u;
            INT_SnP_AddObject(gao, v6->snp);
            INT_SnP_InsertionSort(v6->snp, 0);
        }
    }
    v8 = gao->extended->coll->detectionList;
    Src = v8->detectionListGaos;
    v9 = &v8->detectionListGaos[v8->overlapNum];
    v10 = 0;
    while (Src < v9)
    {
        if (OBJ_b_test_flag(*Src, flagMustHave, flagMustNothave, whatToTest))
            memcpy(&a2[v10++], Src, sizeof(tGameObjectMain*));
        ++Src;
    }
    a2[v10] = NULL;
    return v10;
}

tAINode* AI_EvalFunc_COLBVGetAllOverlaps(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    char dest[52];
    int flagMustHave; 
    int flagMustNothave;
    tVarStackElem a2;
    tGameObjectMain* v8;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v8 = v3;
    v1 = AI_EvalFunc_COLBVGetAllOverlaps_C(v3, (tGameObjectMain**)a2.ptr, flagMustHave, flagMustNothave, whatToTest);
    AI_PutIntToStack(v1);
    return node + 1;
}

BOOL AI_EvalFunc_COLBVObjectObject_C(struct sGameObjectMain* gao, struct sGameObjectMain* a2)
{
    BOOL result;

    if (a2)
        result = COL_b_BVOverlap(gao, a2, FALSE, FALSE, TRUE);
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_COLBVObjectObject(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    tGameObjectMain* a2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLBVObjectObject_C(v3, a2);
    AI_PutIntToStack(v1);
    return node + 1;
}

tGameObjectMain* AI_EvalFunc_COLRayBVDist_C(tGameObjectMain* gao, Vec3* a2, Vec3* a3, float a4, int a5, int a6, int whatToTest)
{
    return COL_RayBV(&WOR_gpst_CurrentWorld->activatedObjectsTable4PF, a2, a3, a4, a5, a6, whatToTest, 1);
}

tAINode* AI_EvalFunc_COLRayBVDist(tAINode* arg0)
{
    int v2[3];
    tGameObjectMain* v3;
    int a5;
    int a6;
    float a4;
    Vec3 a2;
    Vec3 a1;
    tGameObjectMain* v9;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    a6 = AI_GetIntFromStack();
    a5 = AI_GetIntFromStack();
    a4 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&a1);
    AI_GetVecFromStack(&a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v9 = v3;
    v2[0] = (int)AI_EvalFunc_COLRayBVDist_C(v3, &a2, &a1, a4, a5, a6, whatToTest);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return arg0 + 1;
}

tGameObjectMain* AI_EvalFunc_COLRayBVVector_C(tGameObjectMain* gao, Vec3* a2, Vec3* a3, int a4, int a5, int a6)
{
    float a4a;
    Vec3 a3a;
    tWorld* v9; 
    float v10;

    v9 = WOR_gpst_CurrentWorld;
    a4a = MATH_VecLen(a3);
    v10 = (float)(1.0 / a4a);
    a3a.x = v10 * a3->x;
    a3a.y = a3->y * v10;
    a3a.z = a3->z * v10;
    return COL_RayBV(&v9->activatedObjectsTable4PF, a2, &a3a, a4a, a4, a5, a6, 1);
}

tAINode* AI_EvalFunc_COLRayBVVector(tAINode* arg0)
{
    int v2[3];
    tGameObjectMain* v3;
    int a4;
    int a5;
    Vec3 a2;
    Vec3 a1;
    tGameObjectMain* v8;
    int a6;

    a6 = AI_GetIntFromStack();
    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    AI_GetVecFromStack(&a1);
    AI_GetVecFromStack(&a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v8 = v3;
    v2[0] = (int)AI_EvalFunc_COLRayBVVector_C(v3, &a2, &a1, a4, a5, a6);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return arg0 + 1;
}

int AI_EvalFunc_COLCollideType_C(struct sGameObjectMain* gao, int a3)
{
    return COL_Report_CollideType(gao, WOR_gpst_CurrentWorld, a3);
}

tAINode* AI_EvalFunc_COLCollideType(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    int a3;

    a3 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLCollideType_C(v3, a3);
    AI_PutIntToStack(v1);
    return node + 1;
}

Vec3* AI_EvalFunc_COLNormalGet_C(struct sGameObjectMain* gao, int a2, Vec3* a3)
{
    Vec3* result;
    BOOL v4;
    Vec3 a3a;
    tWorld* a2a;

    a2a = WOR_gpst_CurrentWorld;
    COL_Report_GetNormal(gao, WOR_gpst_CurrentWorld, &a3a, a2);
    if (fabs(a3a.x) != 0.0) // todo inline!!!
    {
        v4 = 0;
    }
    else if (fabs(a3a.y) != 0.0)
    {
        v4 = 0;
    }
    else
    {
        v4 = fabs(a3a.z)== 0.0;
    }
    if (v4)
    {
        *a3 = MATH_gst_NulVector;
        result = a3;
    }
    else
    {
        *a3 = a3a;
        result = &a3a;
    }
    return result;
}

tAINode* AI_EvalFunc_COLNormalGet(tAINode* arg0)
{
    tGameObjectMain* v2;
    int a2;
    Vec3 a1;
    tGameObjectMain* v5;

    a2 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v2;
    AI_EvalFunc_COLNormalGet_C(v2, a2, &a1);
    AI_PutVecToStack(&a1);
    return arg0 + 1;
}

struct sGameObjectMain* AI_EvalFunc_COLRayObjectDist_C(struct sGameObjectMain* gao, Vec3* a2, Vec3* a3, float a4, int a5, int a6, int a7, int a8)
{
    tGameObjectMain* result;
    unsigned int a14;

    a14 = a8 & 0x80000000;
    switch (a8 & 0x7FFFFFFF)
    {
    case 0:
        result = COL_Visual_RayCast(WOR_gpst_CurrentWorld, gao, a2, a3, a4, a5, a6, a7, 0, 1);
        break;
    case 1:
        result = COL_ColMap_RayCast(WOR_gpst_CurrentWorld, gao, a2, a3, a4, a5, a6, a7, 0, 1, 1, 1, 0, a14);
        break;
    case 2:
        result = COL_ColMap_RayCast(WOR_gpst_CurrentWorld, gao, a2, a3, a4, a5, a6, a7, 0, 1, 0, 1, 0, a14);
        break;
    case 3:
        result = COL_ColMap_RayCast(WOR_gpst_CurrentWorld, gao, a2, a3, a4, a5, a6, a7, 0, 1, 1, 0, 0, a14);
        break;
    case 4:
        result = COL_ColMap_RayCast(WOR_gpst_CurrentWorld, gao, a2, a3, a4, a5, a6, a7, 0, 1, 1, 1, 1, a14);
        break;
    case 5:
        result = COL_ColMap_RayCast(WOR_gpst_CurrentWorld, gao, a2, a3, a4, a5, a6, a7, 0, 1, 0, 1, 1, a14);
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

tAINode* AI_EvalFunc_COLRayObjectDist(tAINode* arg0)
{
    int v2[4];
    tGameObjectMain* v3;
    Vec3 a1;
    int a5;
    Vec3 a2; 
    int a6;
    int a8;
    float a4;
    tGameObjectMain* v10;
    int a7;

    a8 = AI_GetIntFromStack();
    a7 = AI_GetIntFromStack();
    a6 = AI_GetIntFromStack();
    a5 = AI_GetIntFromStack();
    a4 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&a1);
    AI_GetVecFromStack(&a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v10 = v3;
    if (!i_Raster_741)
    {
        PRO_RasterAdd(&PRO_RayObjectDist, "Trame Loop", "Game Engine", "Ray Object Dist", 0, 0.0f);
        i_Raster_741 = 1;
    }
    if (!PRO_RayObjectDist.insideCounter)
        PRO_RayObjectDist.startTick = TIM_GetTicksAsInt32();
    ++PRO_RayObjectDist.insideCounter;
    v2[0] = (int)AI_EvalFunc_COLRayObjectDist_C(v10, &a2, &a1, a4, a5, a6, a7, a8);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    PRO_RecordTimeDecRef(&PRO_RayObjectDist);
    return arg0 + 1;
}

struct sGameObjectMain* AI_EvalFunc_COLRayObjectVector_C(struct sGameObjectMain* gao, Vec3* a2, Vec3* a3, int a4, int a5, int a6, int a7)
{
    tGameObjectMain* result;
    Vec3 a4a;
    int a14; 
    float a5a;
    tWorld* v11;
    float v12;

    v11 = WOR_gpst_CurrentWorld;
    a5a = MATH_VecLen(a3);
    v12 = (float)(1.0 / a5a);
    a4a.x = v12 * a3->x;
    a4a.y = a3->y * v12;
    a4a.z = a3->z * v12;
    a14 = a7 & 0x80000000;
    switch (a7 & 0x7FFFFFFF)
    {
    case 0:
        result = COL_Visual_RayCast(v11, gao, a2, &a4a, a5a, a4, a5, a6, 0, 1);
        break;
    case 1:
        result = COL_ColMap_RayCast(v11, gao, a2, &a4a, a5a, a4, a5, a6, 0, 1, 1, 1, 0, a14);
        break;
    case 2:
        result = COL_ColMap_RayCast(v11, gao, a2, &a4a, a5a, a4, a5, a6, 0, 1, 0, 1, 0, a14);
        break;
    case 3:
        result = COL_ColMap_RayCast(v11, gao, a2, &a4a, a5a, a4, a5, a6, 0, 1, 1, 0, 0, a14);
        break;
    case 4:
        result = COL_ColMap_RayCast(v11, gao, a2, &a4a, a5a, a4, a5, a6, 0, 1, 1, 1, 1, a14);
        break;
    case 5:
        result = COL_ColMap_RayCast(v11, gao, a2, &a4a, a5a, a4, a5, a6, 0, 1, 0, 1, 1, a14);
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

tAINode* AI_EvalFunc_COLRayObjectVector(tAINode* arg0)
{
    int v2[4];
    tGameObjectMain* v3;
    Vec3 a1;
    int a4;
    Vec3 a2;
    int a5;
    int a7;
    tGameObjectMain* v9;
    int a6;

    a7 = AI_GetIntFromStack();
    a6 = AI_GetIntFromStack();
    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    AI_GetVecFromStack(&a1);
    AI_GetVecFromStack(&a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v9 = v3;
    if (!i_Raster_774)
    {
        PRO_RasterAdd(&PRO_RayObjectVector, "Trame Loop", "Game Engine", "Ray Object Vector", 0, 0.0f);
        i_Raster_774 = 1;
    }
    if (!PRO_RayObjectVector.insideCounter)
        PRO_RayObjectVector.startTick = TIM_GetTicksAsInt32();
    ++PRO_RayObjectVector.insideCounter;
    v2[0] = (int)AI_EvalFunc_COLRayObjectVector_C(v9, &a2, &a1, a4, a5, a6, a7);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    PRO_RecordTimeDecRef(&PRO_RayObjectVector);
    return arg0 + 1;
}

Vec3* AI_EvalFunc_COLRayPosGet_C(struct sGameObjectMain* gao, Vec3* a2)
{
    *a2 = WOR_gpst_CurrentWorld->colRayHitPos;
    return a2;
}

tAINode* AI_EvalFunc_COLRayPosGet(tAINode* arg0)
{
    tGameObjectMain* v2;
    Vec3 a1;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_EvalFunc_COLRayPosGet_C(v2, &a1);
    AI_PutVecToStack(&a1);
    return arg0 + 1;
}

Vec3* AI_EvalFunc_COLRayNormalGet_C(struct sGameObjectMain* gao, Vec3* a2)
{
    *a2 = WOR_gpst_CurrentWorld->rayCastNormalVec;
    return a2;
}

tAINode* AI_EvalFunc_COLRayNormalGet(tAINode* arg0)
{
    tGameObjectMain* v2;
    Vec3 a1;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_EvalFunc_COLRayNormalGet_C(v2, &a1);
    AI_PutVecToStack(&a1);
    return arg0 + 1;
}

struct sGameObjectMain* AI_EvalFunc_COLGaoGet_C(struct sGameObjectMain* gao, int a2)
{
    return COL_Report_GetCOBGameObject(gao, WOR_gpst_CurrentWorld, a2);
}

tAINode* AI_EvalFunc_COLGaoGet(tAINode* node)
{
    int v2[3];
    tGameObjectMain* v3;
    int a3;
    tGameObjectMain* v5;

    a3 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v3;
    v2[0] = (int)AI_EvalFunc_COLGaoGet_C(v3, a3);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return node + 1;
}

int AI_EvalFunc_COLGMFlagsGet_C(struct sGameObjectMain* gao, int a3)
{
    return COL_GameMaterial_GetFlags(gao, WOR_gpst_CurrentWorld, a3);
}

tAINode* AI_EvalFunc_COLGMFlagsGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    int a3;

    a3 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLGMFlagsGet_C(v3, a3);
    AI_PutIntToStack(v1);
    return node + 1;
}

float AI_EvalFunc_COLGMSlideGet_C(struct sGameObjectMain* gao, int a3)
{
    return COL_GameMaterial_GetSlide(gao, WOR_gpst_CurrentWorld, a3);
}

tAINode* AI_EvalFunc_COLGMSlideGet(tAINode* node)
{
    float v2;
    tGameObjectMain* v3;
    int a3;

    a3 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_COLGMSlideGet_C(v3, a3);
    AI_PutFloatToStack(v2);
    return node + 1;
}

float AI_EvalFunc_COLGMReboundGet_C(tGameObjectMain* gao, int a3)
{
    return COL_GameMaterial_GetRebound(gao, WOR_gpst_CurrentWorld, a3);
}

tAINode* AI_EvalFunc_COLGMReboundGet(tAINode* node)
{
    float v2;
    tGameObjectMain* v3;
    int a3;

    a3 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_COLGMReboundGet_C(v3, a3);
    AI_PutFloatToStack(v2);
    return node + 1;
}

int AI_EvalFunc_COLGMSoundGet_C(struct sGameObjectMain* gao, int a3)
{
    return COL_GameMaterial_GetSound(gao, WOR_gpst_CurrentWorld, a3);
}

tAINode* AI_EvalFunc_COLGMSoundGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    int a3;

    a3 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLGMSoundGet_C(v3, a3);
    AI_PutIntToStack(v1);
    return node + 1;
}

void AI_EvalFunc_COLGlobalWallCosAngleSet_C(float a1)
{
    COL_gst_GlobalVars.wallCosAngle = a1;
}

tAINode* AI_EvalFunc_COLGlobalWallCosAngleSet(tAINode* arg0)
{
    float a1;

    a1 = AI_GetFloatFromStack();
    AI_EvalFunc_COLGlobalWallCosAngleSet_C(a1);
    return arg0 + 1;
}

void AI_EvalFunc_COLComputeJumpPoint_C(struct sGameObjectMain* gao, float a2, int a3, int a4, Vec3* a5)
{
    COL_ComputeJumpPoint(gao, a5, a2, a3, a4);
}

tAINode* AI_EvalFunc_COLComputeJumpPoint(tAINode* arg0)
{
    tGameObjectMain* v2;
    float a2;
    int a3;
    Vec3 a1;
    int a4;
    tGameObjectMain* v7; 

    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    a2 = AI_GetFloatFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v7 = v2;
    AI_EvalFunc_COLComputeJumpPoint_C(v2, a2, a3, a4, &a1);
    AI_PutVecToStack(&a1);
    return arg0 + 1;
}

void AI_EvalFunc_COLCrossableSet_C(struct sGameObjectMain* gao, int16_t a2, int16_t a3)
{
    tColInstance* v3;

    if (gao && gao->extended && gao->extended->coll)
    {
        if (gao->extended->coll->colInstance)
        {
            v3 = gao->extended->coll->colInstance;
            v3->crossable = a2 | ~a3 & v3->crossable;
        }
    }
}

tAINode* AI_EvalFunc_COLCrossableSet(tAINode* node)
{
    tGameObjectMain* v2;
    int16_t a2;
    int16_t a3;

    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLCrossableSet_C(v2, a2, a3);
    return node + 1;
}

int AI_EvalFunc_COLZDMObjectListGet_C(tGameObjectMain* gao, tGameObjectMain** a2, Vec3* a3, int a5)
{
    return COL_Report_GetZDMGameObjectList(gao, WOR_gpst_CurrentWorld, a2, a3, a5);
}

tAINode* AI_EvalFunc_COLZDMObjectListGet(tAINode* arg0)
{
    int v1;
    tGameObjectMain* v3;
    Vec3 a1;
    char dest[52];
    int a5; 
    tVarStackElem a2;
    tGameObjectMain* v8; 

    a5 = AI_GetIntFromStack();
    AI_GetVecFromStack(&a1);
    AI_GetVarFromStack(dest, &a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v8 = v3;
    v1 = AI_EvalFunc_COLZDMObjectListGet_C(v3, (tGameObjectMain**)a2.ptr, &a1, a5);
    AI_PutIntToStack(v1);
    return arg0 + 1;
}

tAINode* AI_EvalFunc_COLRayCompute(tAINode* node)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    int a6;
    int a7;
    char dest[52];
    unsigned int* a10;
    int a9;
    Vec3 a4;
    Vec3 a5;
    tWorld* v11;
    tGameObjectMain* a3;
    int a8;
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    a10 = (unsigned int*)a2.ptr;
    a9 = AI_GetIntFromStack();
    a8 = AI_GetIntFromStack();
    a7 = AI_GetIntFromStack();
    a6 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = v2;
    v11 = WOR_gpst_CurrentWorld;
    AI_GetVecFromStack(&a5);
    a1a = a5;
    AI_GetVecFromStack(&a4);
    COL_RayCompute(&a1a, v11, a3, &a4, &a5, a6, a7, a8, a9, a10);
    AI_PutVecToStack(&a1a);
    return node + 1;
}

struct sGameObjectMain* AI_EvalFunc_COLBestWallGaoGet_C(struct sGameObjectMain* gao, Vec3* a2, int* a5)
{
    return COL_Report_GetBestWallGOInDirection(gao, WOR_gpst_CurrentWorld, a2, 0.0, a5);
}

tAINode* AI_EvalFunc_COLBestWallGaoGet(tAINode* arg0)
{
    int v2[3];
    tGameObjectMain* v3;
    char dest[52];
    Vec3 a1;
    tGameObjectMain* v6;
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    AI_GetVecFromStack(&a1);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v6 = v3;
    v2[0] = (int)AI_EvalFunc_COLBestWallGaoGet_C(v3, &a1, (int*)a2.ptr);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return arg0 + 1;
}

int AI_EvalFunc_COLPriorityGet_C(struct sGameObjectMain* gao)
{
    int result;

    if (gao && gao->extended && gao->extended->coll && gao->extended->coll->colInstance)
        result = (char)gao->extended->coll->colInstance->priority;
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_COLPriorityGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLPriorityGet_C(v3);
    AI_PutIntToStack(v1);
    return node + 1;
}

void AI_EvalFunc_COLPrioritySet_C(struct sGameObjectMain* gao, char a2)
{
    if (gao && gao->extended && gao->extended->coll)
    {
        if (gao->extended->coll->colInstance)
            gao->extended->coll->colInstance->priority = a2;
    }
}

tAINode* AI_EvalFunc_COLPrioritySet(tAINode* node)
{
    char v1; 
    tGameObjectMain* v3;

    v1 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLPrioritySet_C(v3, v1);
    return node + 1;
}

struct sGameObjectMain* AI_EvalFunc_COLCrossObjectGet_C(struct sGameObjectMain* gao, int a2)
{
    return WOR_gpst_CurrentWorld->collCrossObjects[a2];
}

tAINode* AI_EvalFunc_COLCrossObjectGet(tAINode* node)
{
    int a1a[3];
    tGameObjectMain* v3;
    int v4;
    tGameObjectMain* v5;

    v4 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v3;
    a1a[0] = (int)AI_EvalFunc_COLCrossObjectGet_C(v3, v4);
    AI_PutObjectToStack(a1a, AI_gaw_EnumLink[40]);
    return node + 1;
}

int AI_EvalFunc_COLCrossObjectNumber_C()
{
    return WOR_gpst_CurrentWorld->colCrossObjectNumber;
}

tAINode* AI_EvalFunc_COLCrossObjectNumber(tAINode* node)
{
    int v1;

    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLCrossObjectNumber_C();
    AI_PutIntToStack(v1);
    return node + 1;
}

void AI_EvalFunc_COLPointGet_C(struct sGameObjectMain* gao, int a2, Vec3* a3)
{
    Vec3* v3;

    v3 = COL_Report_GetCollidedPoint(gao, WOR_gpst_CurrentWorld, a2);
    if (v3)
        *a3 = *v3;
    else
        *a3 = MATH_gst_NulVector;
}

tAINode* AI_EvalFunc_COLPointGet(tAINode* arg0)
{
    tGameObjectMain* v2;
    Vec3 a3;
    int a2;
    tGameObjectMain* a1;

    a2 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1 = v2;
    AI_EvalFunc_COLPointGet_C(v2, a2, &a3);
    AI_PutVecToStack(&a3);
    return arg0 + 1;
}

void AI_EvalFunc_COLGMatPointGet_C(tGameObjectMain* gao, int a2, int* a3, Vec3* a4)
{
    Vec3* v4;

    v4 = COL_Report_GMat_GetCollidedPoint(gao, WOR_gpst_CurrentWorld, a2, a3);
    if (v4)
        *a4 = *v4;
    else
        *a4 = MATH_gst_NulVector;
}

tAINode* AI_EvalFunc_COLGMatPointGet(tAINode* arg0)
{
    tGameObjectMain* v2;
    Vec3 a4;
    int a2;
    char dest[52];
    tGameObjectMain* a1;
    tVarStackElem v7;

    AI_GetVarFromStack(dest, &v7);
    a2 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1 = v2;
    AI_EvalFunc_COLGMatPointGet_C(v2, a2, (int *)v7.ptr, &a4);
    AI_PutVecToStack(&a4);
    return arg0 + 1;
}

int AI_EvalFunc_COLGetAllReportsNormal_C(tGameObjectMain* gao, Vec3* a2, int a4)
{
    return COL_Report_GetAllReportsNormal(gao, WOR_gpst_CurrentWorld, a2, a4);
}

tAINode* AI_EvalFunc_COLGetAllReportsNormal(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    char dest[52];
    int a4; 
    tVarStackElem a2;
    tGameObjectMain* a1a;

    a4 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1a = v3;
    v1 = AI_EvalFunc_COLGetAllReportsNormal_C(v3, (Vec3*)a2.ptr, a4);
    AI_PutIntToStack(v1);
    return node + 1;
}

int AI_EvalFunc_COLGetAllVisibleObjects_C(struct sGameObjectMain *a1, struct sGameObjectMain** a5, int flagMustHave, int flagMustNothave, int whatToTest)
{
    return COL_VisibleObjectListCreate(WOR_gpst_CurrentWorld, flagMustHave, flagMustNothave, whatToTest, a5);
}

tAINode* AI_EvalFunc_COLGetAllVisibleObjects(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    char dest[52];
    int flagMustHave;
    int flagMustNothave;
    tVarStackElem a2;
    tGameObjectMain* a1a;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1a = v3;
    v1 = AI_EvalFunc_COLGetAllVisibleObjects_C(v3, (tGameObjectMain**)a2.ptr, flagMustHave, flagMustNothave, whatToTest);
    AI_PutIntToStack(v1);
    return node + 1;
}

struct sGameObjectMain* AI_EvalFunc_COLBestWallAngleGaoGet_C(struct sGameObjectMain* gao, Vec3* a2, float a3, int* a5)
{
    return COL_Report_GetBestWallGOInDirection(gao, WOR_gpst_CurrentWorld, a2, a3, a5);
}

tAINode* AI_EvalFunc_COLBestWallAngleGaoGet(tAINode* arg0)
{
    int v2[3];
    tGameObjectMain* v3;
    char dest[52];
    Vec3 a1;
    float a3;
    tGameObjectMain* v7;
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    a3 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&a1);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v7 = v3;
    v2[0] = (int)AI_EvalFunc_COLBestWallAngleGaoGet_C(v3, &a1, a3, (int*)a2.ptr);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return arg0 + 1;
}

int AI_EvalFunc_COLGetReportsNumber_C(struct sGameObjectMain* gao, int a3)
{
    return COL_Report_GetNumberOfReports(gao, WOR_gpst_CurrentWorld, a3);
}

tAINode* AI_EvalFunc_COLGetReportsNumber(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    int a3;

    a3 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLGetReportsNumber_C(v3, a3);
    AI_PutIntToStack(v1);
    return node + 1;
}

void AI_EvalFunc_COLGetResultingNormal_C(tGameObjectMain* gao, int a2, Vec3* a4)
{
    COL_Report_ComputeResultingNormal(gao, WOR_gpst_CurrentWorld, a2, a4);
}

tAINode* AI_EvalFunc_COLGetResultingNormal(tAINode* arg0)
{
    tGameObjectMain* v2;
    int a2;
    Vec3 a1;
    tGameObjectMain* v5;

    a2 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v2;
    AI_EvalFunc_COLGetResultingNormal_C(v2, a2, &a1);
    AI_PutVecToStack(&a1);
    return arg0 + 1;
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

tGameObjectMain* AI_EvalFunc_COLSpecialBestWallGaoGet_C(struct sGameObjectMain* gao, Vec3* a2, int* a5)
{
    return COL_Report_GetBestWallGOInDirectionSpecial(gao, WOR_gpst_CurrentWorld, a2, 0.0, a5);
}

tAINode* AI_EvalFunc_COLSpecialBestWallGaoGet(tAINode* arg0)
{
    int v2[3];
    tGameObjectMain* v3;
    char dest[52];
    Vec3 a1;
    tGameObjectMain* v6;
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    AI_GetVecFromStack(&a1);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v6 = v3;
    v2[0] = (int)AI_EvalFunc_COLSpecialBestWallGaoGet_C(v3, &a1, (int*)a2.ptr);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return arg0 + 1;
}

int AI_EvalFunc_COLGMIDGet_C(struct sGameObjectMain* gao, int a3)
{
    int result; 
    tWorld* a2;

    a2 = WOR_gpst_CurrentWorld;
    if ((uint16_t)COL_GameMaterial_GetID(gao, WOR_gpst_CurrentWorld, a3) == 0xFFFF)
        result = -1;
    else
        result = (uint16_t)COL_GameMaterial_GetID(gao, a2, a3);
    return result;
}

tAINode* AI_EvalFunc_COLGMIDGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    int a3;

    a3 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLGMIDGet_C(v3, a3);
    AI_PutIntToStack(v1);
    return node + 1;
}

void AI_EvalFunc_COL_SpecificCrossableSet_C(uint16_t a1)
{
    COL_guw_UserCrossable = a1;
}

tAINode* AI_EvalFunc_COL_SpecificCrossableSet(tAINode* node)
{
    uint16_t a1a;

    a1a = AI_GetIntFromStack();
    AI_EvalFunc_COL_SpecificCrossableSet_C(a1a);
    return node + 1;
}

int AI_EvalFunc_COLGetReportsNumberAndRanks_C(struct sGameObjectMain* gao, int* a5, int a3, int a4)
{
    return COL_Report_GetNumberOfReportsAndRanks(gao, WOR_gpst_CurrentWorld, a4, a3, a5);
}

tAINode* AI_EvalFunc_COLGetReportsNumberAndRanks(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    char dest[52];
    int a3;
    int a4;
    tVarStackElem a2;
    tGameObjectMain* a1a;

    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1a = v3;
    v1 = AI_EvalFunc_COLGetReportsNumberAndRanks_C(v3, (int*)a2.ptr, a3, a4);
    AI_PutIntToStack(v1);
    return node + 1;
}

int AI_EvalFunc_COLObjectCollide_C(struct sGameObjectMain* gao, struct sGameObjectMain* a2)
{
    return COL_Report_b_ObjectsAreColliding(gao, a2, WOR_gpst_CurrentWorld);
}

tAINode* AI_EvalFunc_COLObjectCollide(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v1 = AI_EvalFunc_COLObjectCollide_C(v3, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    AI_PutIntToStack(v1);
    return node + 1;
}



struct sGameObjectMain* AI_EvalFunc_COLRaySingleObjectDist_C(struct sGameObjectMain* gao, Vec3* rayV1, Vec3* rayV2, float rayF1, int flagMustHave, int flagMustNothave, int whatToTest, int a8)
{
    tGameObjectMain* result;

    switch (a8 & 0x7FFFFFFF)
    {
    case 0:
        result = COL_Visual_RayCast_OneObject(
            gao,
            rayV1,
            rayV2,
            rayF1,
            flagMustHave,
            flagMustNothave,
            whatToTest,
            0,
            1,
            0,
            0);
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        result = 0;
        break;
    case 0xA:
        result = COL_Visual_RayCast_OneObject(
            gao,
            rayV1,
            rayV2,
            rayF1,
            flagMustHave,
            flagMustNothave,
            whatToTest,
            0,
            1,
            1,
            1);
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

tAINode* AI_EvalFunc_COLRaySingleObjectDist(tAINode* arg0)
{
    int v2[18];
    Vec3 rayV2;
    int flagMustHave;
    Vec3 rayV1;
    int flagMustNothave; 
    int a8; 
    float rayF1; 
    tGameObjectMain* a1;
    int whatToTest; 

    a8 = AI_GetIntFromStack();
    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    rayF1 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&rayV2);
    AI_GetVecFromStack(&rayV1);
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a1 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    if (!i_Raster_758)
    {
        PRO_RasterAdd(&PRO_RayObjectDist, "Trame Loop", "Game Engine", "Ray Object Dist", 0, 0.0f);
        i_Raster_758 = 1;
    }
    if (!PRO_RayObjectDist.insideCounter)
        PRO_RayObjectDist.startTick = TIM_GetTicksAsInt32();
    ++PRO_RayObjectDist.insideCounter;
    v2[0] = (int)AI_EvalFunc_COLRaySingleObjectDist_C(
        a1,
        &rayV1,
        &rayV2,
        rayF1,
        flagMustHave,
        flagMustNothave,
        whatToTest,
        a8);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    PRO_RecordTimeDecRef(&PRO_RayObjectDist);
    return arg0 + 1;
}

int AI_EvalFunc_COL_IndexReportGet_C(tGameObjectMain* gao, int a2, int a4)
{
    return COL_Report_ul_BestReportGet(gao, WOR_gpst_CurrentWorld, a2, a4);
}

tAINode* AI_EvalFunc_COL_IndexReportGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3; 
    int a2;
    int a4;

    a4 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COL_IndexReportGet_C(v3, a2, a4);
    AI_PutIntToStack(v1);
    return node + 1;
}

void AI_EvalFunc_COL_RayMinDistSet_C(float a1)
{
    COL_gb_UseMinDistance = 1;
    COL_gf_MinDistance = a1;
}

tAINode* AI_EvalFunc_COL_RayMinDistSet(tAINode* node)
{
    float a1a;

    a1a = AI_GetFloatFromStack();
    AI_EvalFunc_COL_RayMinDistSet_C(a1a);
    return node + 1;
}

BOOL AI_EvalFunc_COLHasGMat_C(struct sGameObjectMain* gao, int a3)
{
    return COL_Report_HasGMat(gao, WOR_gpst_CurrentWorld, a3);
}

tAINode* AI_EvalFunc_COLHasGMat(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    int a3;

    a3 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLHasGMat_C(v3, a3);
    AI_PutIntToStack(v1);
    return node + 1;
}

int AI_EvalFunc_COLCrossableGet_C(tGameObjectMain* gao)
{
    int result;

    if (gao && gao->extended && gao->extended->coll && gao->extended->coll->colInstance)
        result = (uint16_t)gao->extended->coll->colInstance->crossable;
    else
        result = -1;
    return result;
}

tAINode* AI_EvalFunc_COLCrossableGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLCrossableGet_C(v3);
    AI_PutIntToStack(v1);
    return node + 1;
}

int AI_EvalFunc_COLDesignGet_C(tGameObjectMain* gao, int a3)
{
    return COL_GetDesign(gao, WOR_gpst_CurrentWorld, a3);
}

tAINode* AI_EvalFunc_COLDesignGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    int a3;

    a3 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLDesignGet_C(v3, a3);
    AI_PutIntToStack(v1);
    return node + 1;
}

int AI_EvalFunc_COLZDEZDEListGet_C(struct sGameObjectMain* gao, struct sGameObjectMain** a7, char a3, char a4, int flagMustHave, int flagMustNothave, int whatToTest)
{
    return COL_ListCreate(gao, flagMustHave, flagMustNothave, whatToTest, a3, WOR_gpst_CurrentWorld, a7, a4);
}

tAINode* AI_EvalFunc_COLZDEZDEListGet(tAINode* node)
{
    int v1; 
    tGameObjectMain* v3; 
    char dest[52];
    int a4;
    int flagMustHave;
    int flagMustNothave; 
    int a3;
    tVarStackElem a2;
    tGameObjectMain* a1a;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1a = v3;
    v1 = AI_EvalFunc_COLZDEZDEListGet_C(v3, (tGameObjectMain**)a2.ptr, a3, a4, flagMustHave, flagMustNothave, whatToTest);
    AI_PutIntToStack(v1);
    return node + 1;
}

int AI_EvalFunc_COLZDEBVListGet_C(tGameObjectMain* gao, tGameObjectMain** a2, char a3, int a4, int a5, int a6)
{
    return COL_ListCreate(gao, a4, a5, a6, a3, WOR_gpst_CurrentWorld, a2, 255);
}

tAINode* AI_EvalFunc_COLZDEBVListGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    char dest[52];
    int v5;
    int v6;
    int v7;
    tVarStackElem a2;
    tWorld* v9;
    tGameObjectMain* v10;
    int v11;

    v11 = AI_GetIntFromStack();
    v6 = AI_GetIntFromStack();
    v5 = AI_GetIntFromStack();
    v7 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v10 = v3;
    v9 = WOR_gpst_CurrentWorld;
    v1 = AI_EvalFunc_COLZDEBVListGet_C(v3, (tGameObjectMain**)a2.ptr, v7, v5, v6, v11);
    AI_PutIntToStack(v1);
    return node + 1;
}

BOOL AI_EvalFunc_COLZDEZDECollide_C(struct sGameObjectMain* gao, struct sGameObjectMain* a2, uint8_t a3, uint8_t a4)
{
    return COL_ZDE_ZDECollide(gao, a3, a4, a2);
}

tAINode* AI_EvalFunc_COLZDEZDECollide(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    tGameObjectMain* a2;
    uint8_t a3;
    uint8_t a4;

    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLZDEZDECollide_C(v3, a2, a3, a4);
    AI_PutIntToStack(v1);
    return node + 1;
}

BOOL AI_EvalFunc_COLZDEBVCollide_C(tGameObjectMain* gao, tGameObjectMain* a2, uint8_t a3)
{
    return COL_ZDE_BVCollide(gao, a3, a2, 0);
}

tAINode* AI_EvalFunc_COLZDEBVCollide(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    tGameObjectMain* a2;
    uint8_t a3;

    a3 = AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLZDEBVCollide_C(v3, a2, a3);
    AI_PutIntToStack(v1);
    return node + 1;
}

int AI_EvalFunc_COLZDEPivotCollide_C(struct sGameObjectMain* gao, struct sGameObjectMain* a2, uint8_t a3)
{
    return COL_ZDE_PivotCollide(gao, a3, a2, 0);
}

tAINode* AI_EvalFunc_COLZDEPivotCollide(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    tGameObjectMain* a2;
    uint8_t a3;

    a3 = AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLZDEPivotCollide_C(v3, a2, a3);
    AI_PutIntToStack(v1);
    return node + 1;
}

int AI_EvalFunc_COLZDEPivotListGet_C(struct sGameObjectMain* gao, struct sGameObjectMain** a7, char a3, int flagMustHave, int flagMustNothave, int whatToTest)
{
    return COL_ListCreate(gao, flagMustHave, flagMustNothave, whatToTest, a3, WOR_gpst_CurrentWorld, a7, 0xFB);
}

tAINode* AI_EvalFunc_COLZDEPivotListGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    char dest[52];
    int flagMustHave; 
    int flagMustNothave;
    int a3;
    tVarStackElem a2;
    tWorld* v9; 
    tGameObjectMain* v10; 
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v10 = v3;
    v9 = WOR_gpst_CurrentWorld;
    v1 = AI_EvalFunc_COLZDEPivotListGet_C(v3, (tGameObjectMain**)a2.ptr, a3, flagMustHave, flagMustNothave, whatToTest);
    AI_PutIntToStack(v1);
    return node + 1;
}

int AI_EvalFunc_COLBVPivotListGet_C(struct sGameObjectMain* gao, struct sGameObjectMain** a2, int a3, int a4, int a5)
{
    return COL_ListCreate(gao, a3, a4, a5, 0, WOR_gpst_CurrentWorld, a2, 253);
}

tAINode* AI_EvalFunc_COLBVPivotListGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    char dest[52];
    int v5;
    int v6;
    tVarStackElem a2;
    tWorld* v8;
    tGameObjectMain* v9;
    int v10;

    v10 = AI_GetIntFromStack();
    v6 = AI_GetIntFromStack();
    v5 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v9 = v3;
    v8 = WOR_gpst_CurrentWorld;
    v1 = AI_EvalFunc_COLBVPivotListGet_C(v3, (tGameObjectMain**)a2.ptr, v5, v6, v10);
    AI_PutIntToStack(v1);
    return node + 1;
}

int AI_EvalFunc_COLBVZDEListGet_C(tGameObjectMain* gao, tGameObjectMain** a2, char a3, int a4, int a5, int a6)
{
    return COL_ListCreate(gao, a4, a5, a6, a3, WOR_gpst_CurrentWorld, a2, 254);
}

tAINode* AI_EvalFunc_COLBVZDEListGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    char dest[52];
    int v5;
    int v6;
    int v7;
    tVarStackElem a2;
    tWorld* v9;
    tGameObjectMain* v10;
    int v11;

    v11 = AI_GetIntFromStack();
    v6 = AI_GetIntFromStack();
    v5 = AI_GetIntFromStack();
    v7 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v10 = v3;
    v9 = WOR_gpst_CurrentWorld;
    v1 = AI_EvalFunc_COLBVZDEListGet_C(v3, (tGameObjectMain**)a2.ptr, v7, v5, v6, v11);
    AI_PutIntToStack(v1);
    return node + 1;
}

int AI_EvalFunc_COLPivotZDEListGet_C(tGameObjectMain* gao, tGameObjectMain** a7, char a3, int flagMustHave, int flagMustNothave, int whatToTest)
{
    return COL_ListCreate(gao, flagMustHave, flagMustNothave, whatToTest, a3, WOR_gpst_CurrentWorld, a7, 0xFA);
}

tAINode* AI_EvalFunc_COLPivotZDEListGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    char dest[52];
    int flagMustHave; 
    int flagMustNothave;
    int a3; 
    tVarStackElem a2;
    tWorld* v9;
    tGameObjectMain* a1a;
    int whatToTest; 

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1a = v3;
    v9 = WOR_gpst_CurrentWorld;
    v1 = AI_EvalFunc_COLPivotZDEListGet_C(v3, (tGameObjectMain**)a2.ptr, a3, flagMustHave, flagMustNothave, whatToTest);
    AI_PutIntToStack(v1);
    return node + 1;
}

int AI_EvalFunc_COLPivotBVListGet_C(struct sGameObjectMain* gao, struct sGameObjectMain** a2, int a3, int a4, int a5)
{
    return COL_ListCreate(gao, a3, a4, a5, 0, WOR_gpst_CurrentWorld, a2, 252);
}

tAINode* AI_EvalFunc_COLPivotBVListGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    char dest[52];
    int v5;
    int v6;
    tVarStackElem a2;
    tWorld* v8;
    tGameObjectMain* v9;
    int v10;

    v10 = AI_GetIntFromStack();
    v6 = AI_GetIntFromStack();
    v5 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v9 = v3;
    v8 = WOR_gpst_CurrentWorld;
    v1 = AI_EvalFunc_COLPivotBVListGet_C(v3, (tGameObjectMain**)a2.ptr, v5, v6, v10);
    AI_PutIntToStack(v1);
    return node + 1;
}

int AI_EvalFunc_COLBVPivotCollide_C(struct sGameObjectMain* gao1, struct sGameObjectMain* gao2)
{
    return COL_BV_PivotCollide(gao1, gao2, FALSE);
}

tAINode* AI_EvalFunc_COLBVPivotCollide(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    tGameObjectMain* a2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLBVPivotCollide_C(v3, a2);
    AI_PutIntToStack(v1);
    return node + 1;
}

int AI_EvalFunc_COLPivotBVCollide_C(struct sGameObjectMain* gao1, struct sGameObjectMain* gao2)
{
    return COL_BV_PivotCollide(gao1, gao2, TRUE);
}

tAINode* AI_EvalFunc_COLPivotBVCollide(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    tGameObjectMain* a2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLPivotBVCollide_C(v3, a2);
    AI_PutIntToStack(v1);
    return node + 1;
}

int AI_EvalFunc_COLZDEBVCenterCollide_C(tGameObjectMain* gao, tGameObjectMain* a2, uint8_t a3)
{
    return COL_ZDE_BVCenterCollide(gao, a3, a2, 0);
}

tAINode* AI_EvalFunc_COLZDEBVCenterCollide(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    tGameObjectMain* a2;
    uint8_t a3;

    a3 = AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLZDEBVCenterCollide_C(v3, a2, a3);
    AI_PutIntToStack(v1);
    return node + 1;
}

int AI_EvalFunc_COLZDEBVCenterListGet_C(tGameObjectMain* gao, tGameObjectMain** a7, char a3, int flagMustHave, int flagMustNothave, int whatToTest)
{
    return COL_ListCreate(gao, flagMustHave, flagMustNothave, whatToTest, a3, WOR_gpst_CurrentWorld, a7, 0xEF);
}

tAINode* AI_EvalFunc_COLZDEBVCenterListGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    char dest[52];
    int flagMustHave; 
    int flagMustNothave;
    int a3; 
    tVarStackElem a2; 
    tWorld* v9;
    tGameObjectMain* a1a;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1a = v3;
    v9 = WOR_gpst_CurrentWorld;
    v1 = AI_EvalFunc_COLZDEBVCenterListGet_C(
        v3,
        (tGameObjectMain**)a2.ptr,
        a3,
        flagMustHave,
        flagMustNothave,
        whatToTest);
    AI_PutIntToStack(v1);
    return node + 1;
}

int AI_EvalFunc_COLBVCenterZDEListGet_C(tGameObjectMain* gao, tGameObjectMain** a7, char id, int flagMustHave, int flagMustNothave, int whatToTest)
{
    return COL_ListCreate(gao, flagMustHave, flagMustNothave, whatToTest, id, WOR_gpst_CurrentWorld, a7, 0xEE);
}

tAINode* AI_EvalFunc_COLBVCenterZDEListGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    char dest[52];
    int flagMustHave;
    int flagMustNothave;
    int id;
    tVarStackElem a2;
    tWorld* v9;
    tGameObjectMain* gao;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    id = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    gao = v3;
    v9 = WOR_gpst_CurrentWorld;
    v1 = AI_EvalFunc_COLBVCenterZDEListGet_C(
        v3,
        (tGameObjectMain**)a2.ptr,
        id,
        flagMustHave,
        flagMustNothave,
        whatToTest);
    AI_PutIntToStack(v1);
    return node + 1;
}

void AI_EvalFunc_COLStartMatrixSet_C(struct sGameObjectMain* a2, Vec3* a3)
{
    Vec3 v3;
    Mtx4x4 v4;

    if (a2 && a2->extended && a2->extended->coll && a2->extended->coll->colInstance)
    {
        if ((a2->flags & 0x400000) != 0)
        {
            MATH_InvertMatrix(&v4, a2->basic->mtx->refGao2->matrices);
            MATH_TransformVertex(&v3, &v4, a3);
            a2->extended->coll->colInstance->mtx->Pos = v3;
        }
        else
        {
            a2->extended->coll->colInstance->mtx->Pos = *a3;
        }
        COL_ul_SnP_RefreshType = 0;
    }
}

tAINode* AI_EvalFunc_COLStartMatrixSet(tAINode* node)
{
    tGameObjectMain* v2;
    Vec3 a3;
    tGameObjectMain* a2;

    AI_GetVecFromStack(&a3);
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = v2;
    AI_EvalFunc_COLStartMatrixSet_C(v2, &a3);
    return node + 1;
}

tGameObjectMain* AI_EvalFunc_COLFirstObjectInRangeGet_C(tGameObjectMain* gao, Vec3* a2, float a3, int a4, int a5, int a6)
{
    return COL_GetFirstObjectInRange(WOR_gpst_CurrentWorld, a2, a3, a4, a5, a6);
}

tAINode* AI_EvalFunc_COLFirstObjectInRangeGet(tAINode* arg0)
{
    int v2[3];
    tGameObjectMain* v3;
    int a4;
    int a5; 
    float a3;
    Vec3 a2; 
    tGameObjectMain* a1;
    int a6;

    a6 = AI_GetIntFromStack();
    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    a3 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1 = v3;
    v2[0] = (int)AI_EvalFunc_COLFirstObjectInRangeGet_C(v3, &a2, a3, a4, a5, a6);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return arg0 + 1;
}

tAINode* AI_EvalFunc_COLTriangleCollision(tAINode* arg0)
{
    int v1;
    tGameObjectMain* v3;
    Vec3 a1;
    int flagMustHave;
    int flagMustNothave;
    Vec3* a4;
    char dest[52];
    Vec3 a3; 
    Vec3 a2; 
    tWorld* a5;
    tGameObjectMain* v12;
    int whatToTest;
    tVarStackElem v14;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v12 = v3;
    a5 = WOR_gpst_CurrentWorld;
    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &v14);
    a4 = (Vec3*)v14.ptr;
    AI_GetVecFromStack(&a3);
    AI_GetVecFromStack(&a2);
    AI_GetVecFromStack(&a1);
    v1 = COL_TriangleCollision(&a1, &a2, &a3, a4, a5, flagMustHave, flagMustNothave, whatToTest);
    AI_PutIntToStack(v1);
    return arg0 + 1;
}

void AI_EvalFunc_COLSwapToSpecific_C(struct sGameObjectMain* gao, char a2)
{
    COL_SwapToSpecific(gao, a2);
}

tAINode* AI_EvalFunc_COLSwapToSpecific(tAINode* node)
{
    tGameObjectMain* v2;
    char a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_COLSwapToSpecific_C(v2, a2);
    return node + 1;
}

int AI_EvalFunc_COLDistanceListGet_C(tGameObjectMain* gao, tGameObjectMain** a5, Vec3* a6, float a7, int flagMustHave, int flagMustNothave, int whatToTest, char a8)
{
    return COL_ListCreateDistance(flagMustHave, flagMustNothave, whatToTest, WOR_gpst_CurrentWorld, a5, a6, a7, a8);
}

tAINode* AI_EvalFunc_COLDistanceListGet(tAINode* arg0)
{
    int v1;
    tGameObjectMain* v3;
    char dest[52];
    int flagMustHave;
    int flagMustNothave;
    int a8;
    float a7;
    Vec3 a1;
    tVarStackElem a2;
    tWorld* v11;
    tGameObjectMain* v12;
    int whatToTest;

    a8 = AI_GetIntFromStack();
    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    a7 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&a1);
    AI_GetVarFromStack(dest, &a2);
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v12 = v3;
    v11 = WOR_gpst_CurrentWorld;
    v1 = AI_EvalFunc_COLDistanceListGet_C(
        v3,
        (tGameObjectMain**)a2.ptr,
        &a1,
        a7,
        flagMustHave,
        flagMustNothave,
        whatToTest,
        a8);
    AI_PutIntToStack(v1);
    return arg0 + 1;
}

void AI_EvalFunc_COLSpecificPosSet_C(struct sGameObjectMain* gao, char a2, Vec3* a3)
{
    COL_ZDx_SpecificPosSet(gao, a2, a3);
}

tAINode* AI_EvalFunc_COLSpecificPosSet(tAINode* arg0)
{
    tGameObjectMain* v2;
    Vec3 a3;
    int a2; 
    tGameObjectMain* a1;

    AI_GetVecFromStack(&a3);
    a2 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1 = v2;
    AI_EvalFunc_COLSpecificPosSet_C(v2, a2, &a3);
    return arg0 + 1;
}

void AI_EvalFunc_COLSpecificSizeSet_C(struct sGameObjectMain* gao, uint8_t a2, Vec3* a3)
{
    COL_ZDx_SpecificSizeSet(gao, a2, a3);
}

tAINode* AI_EvalFunc_COLSpecificSizeSet(tAINode* arg0)
{
    tGameObjectMain* v2;
    uint8_t a2;
    Vec3 a1;
    tGameObjectMain* v5;

    AI_GetVecFromStack(&a1);
    a2 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v2;
    AI_EvalFunc_COLSpecificSizeSet_C(v2, a2, &a1);
    return arg0 + 1;
}

int AI_EvalFunc_COLSetDesignGet_C(tGameObjectMain* gao, unsigned int a2)
{
    tColInstance* v3;

    if ((gao->flags & 0x600) == 0 || a2 > 15)
        return 0;
    v3 = gao->extended->coll->colInstance;
    return v3->selectedInstElemPtrs[v3->colSet->zoneIDs[(uint8_t)a2]]->design;
}

tAINode* AI_EvalFunc_COLSetDesignGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    int a2;

    a2 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLSetDesignGet_C(v3, a2);
    AI_PutIntToStack(v1);
    return node + 1;
}

float AI_EvalFunc_COLSizeGet_C(struct sGameObjectMain* gao, uint8_t a2)
{
    return COL_ZDx_SizeGet(gao, a2);
}

tAINode* AI_EvalFunc_COLSizeGet(tAINode* node)
{
    float v2;
    tGameObjectMain* v3;
    uint8_t a2;

    a2 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_COLSizeGet_C(v3, a2);
    AI_PutFloatToStack(v2);
    return node + 1;
}

int AI_EvalFunc_COLTypeGet_C(tGameObjectMain* gao, uint8_t a2)
{
    int result;
    tColInstance* v3;
    uint8_t v4;

    if ((gao->flags & 0x600) == 0 || a2 > 0xFu)
        return 0;
    v3 = gao->extended->coll->colInstance;
    v4 = v3->colSet->zoneIDs[a2];
    if (v4 <= 0xFu)
        result = v3->selectedInstElemPtrs[v4]->elemType;
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_COLTypeGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    uint8_t a2;

    a2 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLTypeGet_C(v3, a2);
    AI_PutIntToStack(v1);
    return node + 1;
}

void AI_EvalFunc_COLBoxMaxGet_C(tGameObjectMain* gao, unsigned int a2, Vec3* a3)
{
    tColInstance* v4;
    uint8_t v5;
    tColInstanceElem* v6;

    if ((gao->flags & 0x600) == 0 || a2 > 0xF)
        *a3 = MATH_gst_NulVector;
    v4 = gao->extended->coll->colInstance;
    v5 = v4->colSet->zoneIDs[(uint8_t)a2];
    if (v5 > 0xFu)
        *a3 = MATH_gst_NulVector;
    v6 = v4->selectedInstElemPtrs[v5];
    if (v6->elemType == 1)
    {
        *a3 = ((tColBox*)(v6->elemShapePtr))->pos1;
    }
    else
    {
        *a3 = MATH_gst_NulVector;
    }
}

tAINode* AI_EvalFunc_COLBoxMaxGet(tAINode* arg0)
{
    tGameObjectMain* v2;
    Vec3 a1;
    unsigned int a2;
    tGameObjectMain* v5;

    a2 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v2;
    AI_EvalFunc_COLBoxMaxGet_C(v2, a2, &a1);
    AI_PutVecToStack(&a1);
    return arg0 + 1;
}

void AI_EvalFunc_COLBoxMinGet_C(tGameObjectMain* gao, unsigned int a2, Vec3* a3)
{
    tColInstance* v3;
    uint8_t v4;
    tColInstanceElem* v5;

    if ((gao->flags & 0x600) == 0 || a2 > 0xF)
        *a3 = MATH_gst_NulVector;
    v3 = gao->extended->coll->colInstance;
    v4 = v3->colSet->zoneIDs[(uint8_t)a2];
    if (v4 > 0xFu)
        *a3 = MATH_gst_NulVector;
    v5 = v3->selectedInstElemPtrs[v4];
    if (v5->elemType == 1)
        *a3 = ((tColBox*)(v5->elemShapePtr))->pos2;
    else
        *a3 = MATH_gst_NulVector;
}

tAINode* AI_EvalFunc_COLBoxMinGet(tAINode* arg0)
{
    tGameObjectMain* v2;
    Vec3 a1;
    unsigned int a2;
    tGameObjectMain* v5;

    a2 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v2;
    AI_EvalFunc_COLBoxMinGet_C(v2, a2, &a1);
    AI_PutVecToStack(&a1);
    return arg0 + 1;
}

void AI_EvalFunc_COLPosGet_C(struct sGameObjectMain* gao, uint8_t a2, Vec3* a3)
{
    a3->x = 0.0;
    a3->y = 0.0;
    a3->z = 0.0;
    COL_ZDx_PosGet(gao, a2, a3);
}

tAINode* AI_EvalFunc_COLPosGet(tAINode* arg0)
{
    tGameObjectMain* v2;
    Vec3 a1;
    int a2;
    tGameObjectMain* v5;

    a2 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v2;
    AI_EvalFunc_COLPosGet_C(v2, a2, &a1);
    AI_PutVecToStack(&a1);
    return arg0 + 1;
}

void AI_EvalFunc_COLFlagXSet_C(tGameObjectMain* gao, uint8_t a2)
{
    tColInstanceElem* v2;
    tColInstance* v3;

    if ((gao->flags & 0x600) != 0)
    {
        v3 = gao->extended->coll->colInstance;
        v2 = v3->selectedInstElemPtrs[v3->colSet->zoneIDs[a2]];
        v2->colFlags |= 0x10u;
    }
}

tAINode* AI_EvalFunc_COLFlagXSet(tAINode* node)
{
    uint8_t v1;
    tGameObjectMain* v3;

    v1 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLFlagXSet_C(v3, v1);
    return node + 1;
}

void AI_EvalFunc_COLFlagXReset_C(tGameObjectMain* gao, uint8_t a2)
{
    tColInstanceElem* v2;
    tColInstance* v3;

    if ((gao->flags & 0x600) != 0)
    {
        v3 = gao->extended->coll->colInstance;
        v2 = v3->selectedInstElemPtrs[v3->colSet->zoneIDs[a2]];
        v2->colFlags &= ~0x10u;
    }
}

tAINode* AI_EvalFunc_COLFlagXReset(tAINode* node)
{
    uint8_t v1;
    tGameObjectMain* v3;

    v1 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLFlagXReset_C(v3, v1);
    return node + 1;
}

BOOL AI_EvalFunc_COLHasZone_C(struct sGameObjectMain* gao, int a2)
{
    BOOL result;
    tColInstance* v3;

    if (!gao || !gao->extended || !gao->extended->coll)
        return 0;
    v3 = gao->extended->coll->colInstance;
    if (v3 && v3->colSet)
        result = v3->colSet->zoneIDs[a2] < (int)v3->zoneNum;
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_COLHasZone(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    int a2;

    a2 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLHasZone_C(v3, a2);
    AI_PutIntToStack(v1);
    return node + 1;
}

void AI_EvalFunc_COLGeoColMapPosSet_C(struct sGameObjectMain* gao, uint8_t a2, Vec3* a3)
{
    COL_GeoColMap_PosSet(gao, a2, a3);
}

tAINode* AI_EvalFunc_COLGeoColMapPosSet(tAINode* arg0)
{
    tGameObjectMain* v2;
    uint8_t v3;
    Vec3 a1;
    tGameObjectMain* v5;

    AI_GetVecFromStack(&a1);
    v3 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v2;
    AI_EvalFunc_COLGeoColMapPosSet_C(v2, v3, &a1);
    return arg0 + 1;
}

void AI_EvalFunc_COLFlagSet_C(struct sGameObjectMain* gao, uint8_t a2, char a3, char a4)
{
    tColInstance* v4;
    tColInstanceElem* v5; 

    if ((gao->flags & 0x600) != 0)
    {
        v4 = gao->extended->coll->colInstance;
        v5 = v4->selectedInstElemPtrs[v4->colSet->zoneIDs[a2]];
        v5->colFlags |= a3;
        v5->colFlags &= ~a4;
    }
}

tAINode* AI_EvalFunc_COLFlagSet(tAINode* node)
{
    tGameObjectMain* v2;
    char a4;
    uint8_t a2;
    char a3;

    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLFlagSet_C(v2, a2, a3, a4);
    return node + 1;
}

void AI_EvalFunc_COLBackFaceSet_C(tGameObjectMain* gao, Vec3* a2)
{
    if (gao && gao->extended && gao->extended->coll)
    {
        if (gao->extended->coll->colInstance)
        {
            gao->extended->coll->colInstance->backface = 1;
            gao->extended->coll->colInstance->backFaceVec = *a2;
        }
    }
}

tAINode* AI_EvalFunc_COLBackFaceSet(tAINode* node)
{
    tGameObjectMain* v2;
    Vec3 a2;

    AI_GetVecFromStack(&a2);
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLBackFaceSet_C(v2, &a2);
    return node + 1;
}

int AI_EvalFunc_COLGMatReportGet_C(struct sGameObjectMain* gao, int a3)
{
    return COL_Report_ul_GMatReportGet(gao, WOR_gpst_CurrentWorld, a3);
}

tAINode* AI_EvalFunc_COLGMatReportGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    int a3;

    a3 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLGMatReportGet_C(v3, a3);
    AI_PutIntToStack(v1);
    return node + 1;
}

float AI_EvalFunc_BVSizeGet_C(struct sGameObjectMain* arg0)
{
    tGameObjectBV* v1;
    float a1;
    float a2;
    float a3;
    float v6;

    if (!OBJ_BV_IsAABBox(arg0->boundingVolume))
        return arg0->boundingVolume->BVMax.x;
    v1 = arg0->boundingVolume;
    a3 = v1->BVMax.z - v1->BVMin.z;
    a2 = v1->BVMax.y - v1->BVMin.y;
    a1 = v1->BVMax.x - v1->BVMin.x;
    v6 = float_get_max_of_3vals_sub_48BBB0(a1, a2, a3);
    return v6 * 0.5f;
}

tAINode* AI_EvalFunc_BVSizeGet(tAINode* node)
{
    float v2;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_BVSizeGet_C(v3);
    AI_PutFloatToStack(v2);
    return node + 1;
}

void AI_EvalFunc_BVRandomPosGet_C(struct sGameObjectMain* gao, int a2, Vec3* a3)
{
    Mtx4x4* v3;
    tGameObjectBV* v4;
    float v5;
    float v6;
    float v7;
    float v8;
    float v9;
    float v10;
    float v11;
    float v12;
    float v13;
    float v14;
    float v15;
    float v16;
    float v17;
    float v18;
    float v19;
    float v20;
    float v21;
    float v22;
    Vec3 a1a;
    Vec3 v24;
    Vec3 v25;

    if (OBJ_BV_IsAABBox(gao->boundingVolume))
    {
        MATH_VecAddVec(&a1a, &gao->boundingVolume->BVMax, &gao->matrices->Pos);
        MATH_VecAddVec(&v24, &gao->boundingVolume->BVMin, &gao->matrices->Pos);
        v20 = a1a.x;
        v19 = v24.x;
        v10 = (float)rand();
        v21 = (float)(v10 / 32767.0);
        a3->x = (v20 - v19) * v21 + v19;
        v17 = a1a.y;
        v16 = v24.y;
        v9 = (float)rand();
        v18 = (float)(v9 / 32767.0);
        a3->y = (v17 - v16) * v18 + v16;
        if (a2)
        {
            v8 = (float)((a1a.z - v24.z) * 0.5 + v24.z);
        }
        else
        {
            v14 = a1a.z;
            v13 = v24.z;
            v7 = (float)rand();
            v15 = (float)(v7 / 32767.0);
            v8 = (v14 - v13) * v15 + v13;
        }
        a3->z = v8;
    }
    else
    {
        v4 = gao->boundingVolume;
        v3 = OBJ_pst_GetAbsoluteMatrix(gao);
        MATH_TransformVertexNoScale(&v25, v3, &v4->BVMin);
        v22 = gao->boundingVolume->BVMax.x;
        a1a.x = v25.x + v22;
        a1a.y = v25.y + v22;
        a1a.z = v25.z + v22;
        v24.x = v25.x - v22;
        v24.y = v25.y - v22;
        v24.z = v25.z - v22;
        v6 = (float)rand();
        v12 = (float)(v6 / 32767.0);
        a3->x = (a1a.x - v24.x) * v12 + v24.x;
        v5 = (float)rand();
        v11 = (float)(v5 / 32767.0);
        a3->y = (a1a.y - v24.y) * v11 + v24.y;
        if (!a2) // TODO FIXME: ugxanaz van mint fentebb, csak itt azért nem látszik mert a végérõl hiányzik egy értékadás, így igazából a Z síkra nem ad értéket. Kidebuggolni!!! IDAban megnézve
            rand();
    }
}

tAINode* AI_EvalFunc_BVRandomPosGet(tAINode* node)
{
    tGameObjectMain* v2;
    int a2;
    Vec3 a3;

    a2 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_BVRandomPosGet_C(v2, a2, &a3);
    AI_PutVecToStack(&a3);
    return node + 1;
}


void AI_EvalFunc_BVMinGet_C(struct sGameObjectMain* gao, Vec3* a2)
{
    if (OBJ_BV_IsAABBox(gao->boundingVolume))
        *a2 = gao->boundingVolume->BVMinOriginal;
    else
        *a2 = MATH_gst_NulVector;
}

tAINode* AI_EvalFunc_BVMinGet(tAINode* arg0)
{
    tGameObjectMain* v2;
    Vec3 a2;
    tGameObjectMain* a1;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1 = v2;
    AI_EvalFunc_BVMinGet_C(v2, &a2);
    AI_PutVecToStack(&a2);
    return arg0 + 1;
}

void AI_EvalFunc_BVMinSet_C(struct sGameObjectMain* gao, Vec3* a2)
{
    if (OBJ_BV_IsAABBox(gao->boundingVolume))
    {
        gao->boundingVolume->BVMinOriginal = *a2;
        gao->boundingVolume->BVMin = *a2;
    }
}

tAINode* AI_EvalFunc_BVMinSet(tAINode* arg0)
{
    tGameObjectMain* v2;
    Vec3 a2;
    tGameObjectMain* a1;

    AI_GetVecFromStack(&a2);
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1 = v2;
    v2->flags2 |= 0x2000000u;
    AI_EvalFunc_BVMinSet_C(a1, &a2);
    return arg0 + 1;
}

void AI_EvalFunc_BVMaxGet_C(struct sGameObjectMain* gao, Vec3* a2)
{
    if (OBJ_BV_IsAABBox(gao->boundingVolume))
        *a2 = gao->boundingVolume->BVMaxOriginal;
    else
        *a2 = MATH_gst_NulVector;
}

tAINode* AI_EvalFunc_BVMaxGet(tAINode* arg0)
{
    tGameObjectMain* v2;
    Vec3 a2;
    tGameObjectMain* a1;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1 = v2;
    AI_EvalFunc_BVMaxGet_C(v2, &a2);
    AI_PutVecToStack(&a2);
    return arg0 + 1;
}

void AI_EvalFunc_BVMaxSet_C(struct sGameObjectMain* gao, Vec3* a2)
{
    if (OBJ_BV_IsAABBox(gao->boundingVolume))
    {
        gao->boundingVolume->BVMaxOriginal = *a2;
        gao->boundingVolume->BVMax = *a2;
    }
}

tAINode* AI_EvalFunc_BVMaxSet(tAINode* arg0)
{
    tGameObjectMain* v2;
    Vec3 a2;
    tGameObjectMain* a1;

    AI_GetVecFromStack(&a2);
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1 = v2;
    v2->flags2 |= 0x2000000u;
    AI_EvalFunc_BVMaxSet_C(a1, &a2);
    return arg0 + 1;
}

float AI_EvalFunc_BVSizeGetMin_C(tGameObjectMain* gao)
{
    tGameObjectBV* v1;
    float a1a;
    float a2;
    float a3;
    float v6;

    if (!OBJ_BV_IsAABBox(gao->boundingVolume))
        return gao->boundingVolume->BVMax.x;
    v1 = gao->boundingVolume;
    a3 = v1->BVMax.z - v1->BVMin.z;
    a2 = v1->BVMax.y - v1->BVMin.y;
    a1a = v1->BVMax.x - v1->BVMin.x;
    v6 = float_get_min_of_3vals_sub_48BB80(a1a, a2, a3);
    return v6 * 0.5f;
}

tAINode* AI_EvalFunc_BVSizeGetMin(tAINode* node)
{
    float v2;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_BVSizeGetMin_C(v3);
    AI_PutFloatToStack(v2);
    return node + 1;
}

void AI_EvalFunc_BVRecompute_C(tGameObjectMain* C)
{
    OBJ_ComputeBV(C, 0);
}

tAINode* AI_EvalFunc_BVRecompute(tAINode* node)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_BVRecompute_C(v2);
    return node + 1;
}

void AI_EvalFunc_COLSetActivationSet_C(struct sGameObjectMain* gao, int16_t a2, int16_t a3)
{
    tColInstance* v3;

    if (gao && gao->extended && gao->extended->coll)
    {
        if (gao->extended->coll->colInstance)
        {
            v3 = gao->extended->coll->colInstance;
            v3->activationFlags = a2 | ~a3 & v3->activationFlags;
        }
    }
}

tAINode* AI_EvalFunc_COLSetActivationSet(tAINode* node)
{
    tGameObjectMain* v2;
    int16_t a3;
    int16_t a2;

    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLSetActivationSet_C(v2, a2, a3);
    return node + 1;
}

int AI_EvalFunc_COLSetActivationGet_C(struct sGameObjectMain* gao)
{
    int result;

    if ((gao->flags & 0x600) != 0)
        result = gao->extended->coll->colInstance->activationFlags;
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_COLSetActivationGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLSetActivationGet_C(v3);
    AI_PutIntToStack(v1);
    return node + 1;
}

void AI_EvalFunc_COLMapActivationSet_C(struct sGameObjectMain* gao, uint8_t a2, char a3)
{
    tColMap* v3;

    if (gao && gao->extended && gao->extended->coll && gao->extended->coll->colMap && (gao->flags & 0x100) != 0)
    {
        v3 = gao->extended->coll->colMap;
        v3->activation = a2 | ~a3 & v3->activation;
    }
}

tAINode* AI_EvalFunc_COLMapActivationSet(tAINode* node)
{
    tGameObjectMain* v2;
    char a3;
    uint8_t a2;

    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLMapActivationSet_C(v2, a2, a3);
    return node + 1;
}

int AI_EvalFunc_COLMapActivationGet_C(struct sGameObjectMain* gao)
{
    int result;

    if ((gao->flags & 0x100) != 0)
        result = gao->extended->coll->colMap->activation;
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_COLMapActivationGet(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_COLMapActivationGet_C(v3);
    AI_PutIntToStack(v1);
    return node + 1;
}

void AI_EvalFunc_COLLODSet_C(struct sGameObjectMain* gao, char a2, char a3)
{
    if ((gao->flags & 0x600) != 0)
    {
        gao->extended->coll->colInstance->minLod = a3;
        gao->extended->coll->colInstance->maxLod = a2;
    }
}

tAINode* AI_EvalFunc_COLLODSet(tAINode* node)
{
    tGameObjectMain* v2;
    char a2;
    char a3;

    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLLODSet_C(v2, a2, a3);
    return node + 1;
}

void AI_EvalFunc_COLWallPush_C(tGameObjectMain* gao, int a2)
{
    if ((gao->flags & 0x600) != 0)
        gao->extended->coll->colInstance->wallPush = a2 != 0;
}

tAINode* AI_EvalFunc_COLWallPush(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;

    v1 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_COLWallPush_C(v3, v1);
    return node + 1;
}