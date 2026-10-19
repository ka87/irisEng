#include "../math.h"
#include "../grid.h"
#include "../gameobject.h"
#include "../time.h"
#include "../profiler.h"
#include "ai_gridfunctions.h"

int AI_EvalFunc_GRIDGetCapa_C(tGameObjectMain* a1, Vec3* a2)
{
    tWorldGridStruct* v3;

    if (GRID_gi_Current)
        v3 = WOR_gpst_CurrentWorld->grid2Ptr;
    else
        v3 = WOR_gpst_CurrentWorld->grid1Ptr;
    return GRI_i_GetCapa(v3, a2);
}

int AI_EvalFunc_GRIDSetWall_C(int a1)
{
    int result;

    result = a1;
    GRID_gi_TestWall = a1;
    return result;
};

tAINode* AI_EvalFunc_GRIDSetWall(tAINode* a1)
{
    int v1;

    v1 = AI_GetIntFromStack();
    AI_EvalFunc_GRIDSetWall_C(v1);
    return a1 + 1;
}

int AI_EvalFunc_GRIDModeSet_C(int a1)
{
    int result;

    result = a1;
    GRID_gi_Mode = a1;
    return result;
}

tAINode* AI_EvalFunc_GRIDModeSet(tAINode* a1)
{
    int v1;

    v1 = AI_GetIntFromStack();
    AI_EvalFunc_GRIDModeSet_C(v1);
    return a1 + 1;
}

int AI_EvalFunc_GRIDCurrentSet_C(int a1)
{
    int result;

    result = a1;
    GRID_gi_Current = a1;
    return result;
}

tAINode* AI_EvalFunc_GRIDCurrentSet(tAINode* a1)
{
    int v1;

    v1 = AI_GetIntFromStack();
    AI_EvalFunc_GRIDCurrentSet_C(v1);
    return a1 + 1;
}

int AI_EvalFunc_GRIDDestSet_C(int a1)
{
    int result;

    result = a1;
    GRID_gi_Dest = a1;
    return result;
}

tAINode* AI_EvalFunc_GRIDDestSet(tAINode* a1)
{
    GRID_gi_Dest = AI_GetIntFromStack();
    return a1 + 1;
}

void AI_EvalFunc_GRIDSetCapa_C(int a1, Vec3* a2, char a3)
{
    tWorldGridStruct* v3;

    if (GRID_gi_Current)
        v3 = WOR_gpst_CurrentWorld->grid2Ptr;
    else
        v3 = WOR_gpst_CurrentWorld->grid1Ptr;
    GRI_SetCapa(v3, a2, a3);
}

tAINode* AI_EvalFunc_GRIDSetCapa(tAINode* a1)
{
    tGameObjectMain* v2;
    char a3;
    Vec3 a1a;
    tGameObjectMain* v5;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v2;
    a3 = AI_GetIntFromStack();
    AI_GetVecFromStack(&a1a);
    AI_EvalFunc_GRIDSetCapa_C((int)v5, &a1a, a3);
    return a1 + 1;
}

void AI_EvalFunc_GRIDSetPosAll_C(int a1, int a2, Vec3* a3)
{
    tWorldGridStruct* v3;
    int v4;

    v4 = GRID_gi_Current;
    GRID_gi_Current = a2;
    if (a2)
        v3 = WOR_gpst_CurrentWorld->grid2Ptr;
    else
        v3 = WOR_gpst_CurrentWorld->grid1Ptr;
    v3->startX = a3->x;
    v3->startY = a3->y;
    GRID_gi_Current = v4;
}

tAINode* AI_EvalFunc_GRIDSetPosAll(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a3;
    int a2;
    int a1a;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1a = (int)v2;
    AI_GetVecFromStack(&a3);
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_GRIDSetPosAll_C(a1a, a2, &a3);
    return a1 + 1;
}

int AI_EvalFunc_GRIDAddCapa_C(int a1, Vec3* a2, char a3)
{
    tWorldGridStruct* v4;

    if (GRID_gi_Current)
        v4 = WOR_gpst_CurrentWorld->grid2Ptr;
    else
        v4 = WOR_gpst_CurrentWorld->grid1Ptr;
    return GRI_AddCapa(v4, a2, a3);
}

tAINode* AI_EvalFunc_GRIDAddCapa(tAINode* a1)
{
    tGameObjectMain* v2; 
    Vec3 a1a;
    int a3;
    tGameObjectMain* v5;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v2;
    a3 = AI_GetIntFromStack();
    AI_GetVecFromStack(&a1a);
    AI_EvalFunc_GRIDAddCapa_C((int)v5, &a1a, a3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_GRIDGetCapa(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    Vec3 a1a;
    tGameObjectMain* v5;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v3;
    AI_GetVecFromStack(&a1a);
    v1 = AI_EvalFunc_GRIDGetCapa_C(v5, &a1a);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_GRIDShortWay_C(struct sGameObjectMain* a1, Vec3* a2, Vec3* a3, Vec3* a4, char a5, int a6, float a7, int a10, int a11)
{
    int v9;
    tWorldGridStruct* v11;
    tWorldGridStruct* v12;
    int v13;

    if (GRID_gi_Current)
        v12 = WOR_gpst_CurrentWorld->grid2Ptr;
    else
        v12 = WOR_gpst_CurrentWorld->grid1Ptr;
    if (GRID_gi_Dest)
        v11 = WOR_gpst_CurrentWorld->grid2Ptr;
    else
        v11 = WOR_gpst_CurrentWorld->grid1Ptr;
    if (!i_Raster_339)
    {
        PRO_RasterAdd(&PRO_ShortWay, "Trame Loop", "Game Engine", "GRID Short Way", 0, 0.0);
        i_Raster_339 = 1;
    }
    if (!PRO_ShortWay.insideCounter)
        PRO_ShortWay.startTick = TIM_GetTicksAsInt32();
    ++PRO_ShortWay.insideCounter;
    v9 = (a10 & 0xffffff00) | (a5 & 0xff); // v9 = a10; LOBYTE(v9) = a5;
    v13 = GRID_ShortestPath(a1, v12, v11, a3, a4, a2, v9, a6, a7, a10, a11);
    PRO_RecordTimeDecRef(&PRO_ShortWay);
    GRID_gi_Dest = 0;
    return v13;
}

tAINode* AI_EvalFunc_GRIDShortWay(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    char a5;
    char dest[52];
    int a11;
    int a6;
    Vec3 a1a;
    Vec3 a3;
    int a10;
    tVarStackElem a2;
    float a7;
    tGameObjectMain* v13;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v13 = v3;
    a11 = AI_GetIntFromStack();
    a10 = AI_GetIntFromStack();
    a7 = AI_GetFloatFromStack();
    a6 = AI_GetIntFromStack();
    a5 = AI_GetIntFromStack();
    AI_GetVecFromStack(&a1a);
    AI_GetVecFromStack(&a3);
    AI_GetVarFromStack(dest, &a2);
    v1 = AI_EvalFunc_GRIDShortWay_C(v13, (Vec3*)a2.ptr, &a3, &a1a, a5, a6, a7, a10, a11);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

Vec3* AI_EvalFunc_GRIDPosGet_C(struct sGameObjectMain* a1, Vec3* a2, Vec3* a3)
{
    Vec3* result;
    float v4;
    float v5;
    tWorldGridStruct* v6;
    int a3a;
    int a4;

    if (GRID_gi_Current)
        v6 = WOR_gpst_CurrentWorld->grid2Ptr;
    else
        v6 = WOR_gpst_CurrentWorld->grid1Ptr;
    *a3 = *a2;
    GRI_3Dto2D(v6, a3, &a3a, &a4);
    v5 = (float)a3a;
    a3->x = (float)(v5 + 0.5 + v6->startX);
    v4 = (float)a4;
    result = a3;
    a3->y = (float)(v4 + 0.5 + v6->startY);
    a3->z = 0.0;
    return result;
}

tAINode* AI_EvalFunc_GRIDPosGet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    Vec3 a2;
    tGameObjectMain* v5;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v2;
    AI_GetVecFromStack(&a2);
    AI_EvalFunc_GRIDPosGet_C(v5, &a2, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

void AI_EvalFunc_GRIDSetCenter_C(struct sGameObjectMain* a1, Vec3* a2)
{
    tWorld* v2;
    int a3;
    int a4;

    v2 = WOR_gpst_CurrentWorld;
    if (WOR_gpst_CurrentWorld->grid1Ptr)
    {
        GRI_3Dto2D(WOR_gpst_CurrentWorld->grid1Ptr, a2, &a3, &a4);
        GRID_SetRealCenter(v2->grid1Ptr, a3, a4);
    }
    if (v2->grid2Ptr)
    {
        GRI_3Dto2D(v2->grid2Ptr, a2, &a3, &a4);
        GRID_SetRealCenter(v2->grid2Ptr, a3, a4);
    }
}

tAINode* AI_EvalFunc_GRIDSetCenter(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_GetVecFromStack(&a1a);
    AI_EvalFunc_GRIDSetCenter_C(v4, &a1a);
    return a1 + 1;
}

BOOL AI_EvalFunc_GRIDIsIn_C(struct sGameObjectMain* a1, Vec3* a2)
{
    BOOL result;
    int v3;
    int v4;

    if (GRID_gi_Current)
        result = GRID_3Dto2D(WOR_gpst_CurrentWorld->grid2Ptr, a2, &v3, &v4) != 0;
    else
        result = GRID_3Dto2D(WOR_gpst_CurrentWorld->grid1Ptr, a2, &v3, &v4) != 0;
    return result;
}

tAINode* AI_EvalFunc_GRIDIsIn(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    Vec3 a1a;
    tGameObjectMain* v5;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v3;
    AI_GetVecFromStack(&a1a);
    v1 = AI_EvalFunc_GRIDIsIn_C(v5, &a1a);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_GRIDTag_C(struct sGameObjectMain* a1, Vec3* a2, int a3, int a4)
{
    tWorldGridStruct* v5;

    if (GRID_gi_Current)
        v5 = WOR_gpst_CurrentWorld->grid2Ptr;
    else
        v5 = WOR_gpst_CurrentWorld->grid1Ptr;
    return GRI_Tag(v5, a2, a3, a4);
}

tAINode* AI_EvalFunc_GRIDTag(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int v4;
    int v5;
    Vec3 a1a;
    tGameObjectMain* v7;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v7 = v3;
    v4 = AI_GetIntFromStack();
    v5 = AI_GetIntFromStack();
    AI_GetVecFromStack(&a1a);
    v1 = AI_EvalFunc_GRIDTag_C(v7, &a1a, v5, v4);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_GRIDTagBox_C(struct sGameObjectMain* a1, Vec3* pos, Vec3* a3, int a4)
{
    tWorldGridStruct* v5;

    if (GRID_gi_Current)
        v5 = WOR_gpst_CurrentWorld->grid2Ptr;
    else
        v5 = WOR_gpst_CurrentWorld->grid1Ptr;
    return GRI_TagBox(v5, pos, a3, a4);
}

tAINode* AI_EvalFunc_GRIDTagBox(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    Vec3 pos;
    int a4;
    Vec3 a1a;
    tGameObjectMain* v7;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v7 = v3;
    a4 = AI_GetIntFromStack();
    AI_GetVecFromStack(&a1a);
    AI_GetVecFromStack(&pos);
    v1 = AI_EvalFunc_GRIDTagBox_C(v7, &pos, &a1a, a4);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_GRIDSmoothWay_C(struct sGameObjectMain* a1, Vec3* a2, int a4)
{
    tWorldGridStruct* v4;
    tWorldGridStruct* v5;

    if (GRID_gi_Current)
        v5 = WOR_gpst_CurrentWorld->grid2Ptr;
    else
        v5 = WOR_gpst_CurrentWorld->grid1Ptr;
    if (GRID_gi_Dest)
        v4 = WOR_gpst_CurrentWorld->grid2Ptr;
    else
        v4 = WOR_gpst_CurrentWorld->grid1Ptr;
    return GRI_SmoothWay(v4, v5, a2, a4);
}

tAINode* AI_EvalFunc_GRIDSmoothWay(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    char dest[52];
    int a4;
    tVarStackElem a2;
    tGameObjectMain* v7;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v7 = v3;
    a4 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v1 = AI_EvalFunc_GRIDSmoothWay_C(v7, (Vec3*)a2.ptr, a4);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_GRIDTagVec_C(struct sGameObjectMain* a1, Vec3* pos, int a3, int a4, Vec3* a5)
{
    tWorldGridStruct* v6;

    if (GRID_gi_Current)
        v6 = WOR_gpst_CurrentWorld->grid2Ptr;
    else
        v6 = WOR_gpst_CurrentWorld->grid1Ptr;
    return GRI_TagVec(v6, pos, a3, a4, a5);
}

tAINode* AI_EvalFunc_GRIDTagVec(tAINode* arg0)
{
    int v1;
    tGameObjectMain* v3;
    Vec3 a1;
    int a4;
    int a3;
    Vec3 pos;
    tGameObjectMain* v8;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v8 = v3;
    AI_GetVecFromStack(&a1);
    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    AI_GetVecFromStack(&pos);
    v1 = AI_EvalFunc_GRIDTagVec_C(v8, &pos, a3, a4, &a1);
    AI_PutIntToStack(v1);
    return arg0 + 1;
}

void AI_EvalFunc_GRIDCptDest_C(struct sGameObjectMain* a1, Vec3* a2, float a3, Vec3* dest)
{
    tWorldGridStruct* v4;
    float v5;

    if (GRID_gi_Current)
        v4 = WOR_gpst_CurrentWorld->grid2Ptr;
    else
        v4 = WOR_gpst_CurrentWorld->grid1Ptr;
    MATH_SubstractVec(dest, a2, &a1->matrices->Pos);
    v5 = MATH_VecLen(dest);
    if (v5 != 0.0)
    {
        if (v5 < (double)a3)
            a3 = v5;
        if (a3 == 0.0)
            a3 = 1.0;
        MATH_NormalizeVector(dest, dest);
        dest->x = a3 * dest->x;
        dest->y = a3 * dest->y;
        dest->z = a3 * dest->z;
        MATH_VecAddVec(dest, dest, &a1->matrices->Pos);
        if (!GRI_i_ComputeDest(v4, dest))
        {
            dest->z = 0.0;
            dest->y = 0.0;
            dest->x = 0.0;
        }
    }
}

tAINode* AI_EvalFunc_GRIDCptDest(tAINode* arg0)
{
    tGameObjectMain* v2;
    Vec3 a1;
    float a3;
    Vec3 a2;
    tGameObjectMain* v6;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v6 = v2;
    a3 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&a2);
    AI_EvalFunc_GRIDCptDest_C(v6, &a2, a3, &a1);
    AI_PutVecToStack(&a1);
    return arg0 + 1;
}

int AI_EvalFunc_GRIDTagDyn_C(struct sGameObjectMain* a1, Vec3* a2, Vec3* a3, float a4, int a5)
{
    tWorldGridStruct* v6;

    if (GRID_gi_Current)
        v6 = WOR_gpst_CurrentWorld->grid2Ptr;
    else
        v6 = WOR_gpst_CurrentWorld->grid1Ptr;
    return GRI_TagDyn(v6, a2, a4, a3, a5);
}

tAINode* AI_EvalFunc_GRIDTagDyn(tAINode* arg0)
{
    int v1;
    tGameObjectMain* v3;
    char dest[52]; 
    int a5;
    float a4;
    Vec3 a1;
    tVarStackElem a2;
    tGameObjectMain* v9;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v9 = v3;
    a5 = AI_GetIntFromStack();
    a4 = AI_GetFloatFromStack();
    AI_GetVarFromStack(dest, &a2);
    AI_GetVecFromStack(&a1);
    v1 = AI_EvalFunc_GRIDTagDyn_C(v9, &a1, (Vec3*)a2.ptr, a4, a5);
    AI_PutIntToStack(v1);
    return arg0 + 1;
}

BOOL AI_EvalFunc_GRIDSurround_C(tGameObjectMain* a1)
{
    tWorldGridStruct* v2;
    Vec3 a2;
    tWorld* v4;

    v4 = WOR_gpst_CurrentWorld;
    if (GRID_gi_Current)
        v2 = v4->grid2Ptr;
    else
        v2 = v4->grid1Ptr;
    OBJ_SightGet(a1, &a2);
    return GRI_Surround(v2, &a1->matrices->Pos, &a2);
}

tAINode* AI_EvalFunc_GRIDSurround(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_GRIDSurround_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}
