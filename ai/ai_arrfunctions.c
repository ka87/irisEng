#include "ai_arrfunctions.h"
#include "ai_gameobjectfunctions.h"

int AI_EvalFunc_ARRObjNearestOfPoint_C(struct sGameObjectMain** a1, int arg4, Vec3* a2)
{
    float v4;
    Vec3 dest;
    float v6;
    int v7;
    Vec3* a3;
    tGameObjectMain* v9;
    int v10;

    v7 = 0;
    v10 = arg4 - 1;
    v6 = FLT_MAX;
    while (v10 >= 0)
    {
        v9 = a1[v10];
        if (v9 && v9 != (tGameObjectMain*)-1)
        {
            a3 = &v9->matrices->Pos;
            MATH_SubstractVec(&dest, a2, a3);
            v4 = MATH_VecLen(&dest);
            if (v4 < (double)v6)
            {
                v6 = v4;
                v7 = v10;
            }
        }
        --v10;
    }
    return v7;
}

tAINode* AI_EvalFunc_ARRObjNearestOfPoint(tAINode* a1)
{
    int v1;
    int arg4;
    char dest[52];
    tVarStackElem a2;
    Vec3* v6;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v6 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    arg4 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v1 = AI_EvalFunc_ARRObjNearestOfPoint_C((tGameObjectMain**)a2.ptr, arg4, v6);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void* AI_EvalFunc_ARRObjCopy_C(void* a1, void* Src, int a3)
{
    return memcpy(a1, Src, 4 * a3);
}

tAINode* AI_EvalFunc_ARRObjCopy(tAINode* a1)
{
    tVarStackElem v2;
    int v3;
    char v4[52];
    tVarStackElem a2;
    char dest[52];

    v3 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    AI_GetVarFromStack(v4, &v2);
    AI_EvalFunc_ARRObjCopy_C(v2.ptr, a2.ptr, v3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_ARRObjCopyIdFlags(tAINode* a1)
{
    tVarStackElem v2;
    int v3;
    int v4;
    int v5;
    char v6[52];
    int i;
    tVarStackElem a2;
    int v9;
    char dest[52];
    tGameObjectMain* v11;

    v3 = AI_GetIntFromStack();
    v5 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    AI_GetVarFromStack(v6, &v2);
    v9 = 0;
    for (i = 0; i < v4; ++i)
    {
        v11 = *(tGameObjectMain**)&a2.ptr[4 * i];
        if (v11 && (v3 & v11->flags) != 0 && (v5 & v11->flags) == 0)
            *(tGameObjectMain**)&a2.ptr[4 * v9++] = v11;
    }
    AI_PutIntToStack(v9);
    return a1 + 1;
}

tAINode* AI_EvalFunc_ARRObjCopyCB(tAINode* a1)
{
    tVarStackElem v2;
    int v3;
    int v4;
    int v5;
    char v6[52];
    int i;
    tVarStackElem a2;
    int v9;
    char dest[52];
    tGameObjectMain* v11;

    v3 = AI_GetIntFromStack();
    v5 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    AI_GetVarFromStack(v6, &v2);
    v9 = 0;
    for (i = 0; i < v4; ++i)
    {
        v11 = *(tGameObjectMain**)&a2.ptr[4 * i];
        if (v11 && (v3 & 0xFF00 & v11->flags2 & 0xFF00) != 0 && (v5 & 0xFF00 & v11->flags2 & 0xFF00) == 0)
            *(tGameObjectMain**)&a2.ptr[4 * v9++] = v11;
    }
    AI_PutIntToStack(v9);
    return a1 + 1;
}

int AI_EvalFunc_ARRObjBestSight_C(struct sGameObjectMain** a1, int a2, Vec3* a3, Vec3* a4, float a5)
{
    BOOL v6;
    float v7;
    float v8 = 0.0f;
    float v9;
    Vec3 dest;
    int v11;
    Vec3 a1a;
    tGameObjectMain* v13;
    int v14;

    MATH_NormalizeVector(&a1a, a4);
    v14 = a2 - 1;
    v11 = v14;
    v9 = -FLT_MAX;
    while (v14 >= 0)
    {
        v13 = a1[v14];
        if (v13 && v13 != (tGameObjectMain*)-1)
        {
            MATH_SubstractVec(&dest, &v13->matrices->Pos, a3);
            if (fabs(dest.x) != 0.0)
                v6 = 0;
            else
                v6 = fabs(dest.y) == 0.0 && fabs(dest.z) == 0.0;
            if (!v6)
            {
                if (a5 != 0.0)
                    v8 = MATH_VecLen(&dest);
                MATH_NormalizeVector(&dest, &dest);
                v7 = a1a.z * dest.z + a1a.y * dest.y + dest.x * a1a.x;
                if (a5 != 0.0)
                    v7 = v7 - v8 / a5;
                if (v7 > (double)v9)
                {
                    v9 = v7;
                    v11 = v14;
                }
            }
        }
        --v14;
    }
    return v11;
}

tAINode* AI_EvalFunc_ARRObjBestSight(tAINode* arg0)
{
    int v1;
    char dest[52];
    Vec3 a3;
    int v5;
    float a5;
    tVarStackElem a2;
    Vec3 a1;

    a5 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&a1);
    AI_GetVecFromStack(&a3);
    v5 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v1 = AI_EvalFunc_ARRObjBestSight_C((tGameObjectMain**)a2.ptr, v5, &a3, &a1, a5);
    AI_PutIntToStack(v1);
    return arg0 + 1;
}

int AI_EvalFunc_ARRObjSearch_C(struct sGameObjectMain** a1, int a2, struct sGameObjectMain* a3)
{
    int i;

    for (i = 0; i < a2; ++i)
    {
        if (a1[i] == a3)
            return i;
    }
    return -1;
}

tAINode* AI_EvalFunc_ARRObjSearch(tAINode* a1)
{
    int v1;
    int v3;
    tVarStackElem a2;
    char dest[52];
    tGameObjectMain* v6;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v6 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v3 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v1 = AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)a2.ptr, v3, v6);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_ARRBitSet_C(int* a1, int a2)
{
    a1[(int)(a2 & 0xFFFFFFE0) >> 5] |= 1 << (a2 & 0x1F);
}

tAINode* AI_EvalFunc_ARRBitSet(tAINode* a1)
{
    int v2;
    tVarStackElem a2;
    char dest[52];

    v2 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    AI_EvalFunc_ARRBitSet_C((int*)a2.ptr, v2);
    return a1 + 1;
}

void AI_EvalFunc_ARRBitClear_C(int* a1, int a2)
{
    a1[(int)(a2 & 0xFFFFFFE0) >> 5] &= ~(1 << (a2 & 0x1F));
}

tAINode* AI_EvalFunc_ARRBitClear(tAINode* a1)
{
    int v2;
    tVarStackElem a2;
    char dest[52];

    v2 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    AI_EvalFunc_ARRBitClear_C((int*)a2.ptr, v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_ARRBitSetRange(tAINode* a1)
{
    int v2;
    int v3;
    tVarStackElem a2;
    int* v5;
    char dest[52];
    int* v7;

    v3 = AI_GetIntFromStack();
    v2 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v5 = (int*)&a2.ptr[4 * ((int)(v2 & 0xFFFFFFE0) >> 5)];
    v7 = (int*)&a2.ptr[4 * ((int)(v3 & 0xFFFFFFE0) >> 5)];
    if (v5 == v7)
    {
        *v5 |= (2 << (v3 & 0x1F)) - (1 << (v2 & 0x1F));
    }
    else
    {
        *v5++ |= -(1 << (v2 & 0x1F));
        *v7 |= (2 << (v3 & 0x1F)) - 1;
        while (v5 != v7)
            *v5++ = -1;
    }
    return a1 + 1;
}

tAINode* AI_EvalFunc_ARRBitClearRange(tAINode* a1)
{
    int v2;
    int v3;
    tVarStackElem a2;
    int* v5;
    char dest[52];
    int* v7;

    v3 = AI_GetIntFromStack();
    v2 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v5 = (int*)&a2.ptr[4 * ((int)(v2 & 0xFFFFFFE0) >> 5)];
    v7 = (int*)&a2.ptr[4 * ((int)(v3 & 0xFFFFFFE0) >> 5)];
    if (v5 == v7)
    {
        *v5 &= ~((2 << (v3 & 0x1F)) - (1 << (v2 & 0x1F)));
    }
    else
    {
        *v5++ &= (1 << (v2 & 0x1F)) - 1;
        *v7 &= ~((2 << (v3 & 0x1F)) - 1);
        while (v5 != v7)
            *v5++ = 0;
    }
    return a1 + 1;
}

BOOL AI_EvalFunc_ARRBitTest_C(int* a1, int a2)
{
    return ((1 << (a2 & 0x1F)) & a1[(int)(a2 & 0xFFFFFFE0) >> 5]) != 0;
}

tAINode* AI_EvalFunc_ARRBitTest(tAINode* a1)
{
    int v1;
    tVarStackElem a2;
    char dest[52];
    int v5;

    v5 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v1 = AI_EvalFunc_ARRBitTest_C((int*)a2.ptr, v5);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_ARRBitChange_C(int* a1, int a2)
{
    int* v2;

    v2 = &a1[(int)(a2 & 0xFFFFFFE0) >> 5];
    if (((1 << (a2 & 0x1F)) & *v2) != 0)
        *v2 &= ~(1 << (a2 & 0x1F));
    else
        *v2 |= 1 << (a2 & 0x1F);
}

tAINode* AI_EvalFunc_ARRBitChange(tAINode* a1)
{
    int v2; 
    tVarStackElem a2;
    char dest[52];

    v2 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    AI_EvalFunc_ARRBitChange_C((int*)a2.ptr, v2);
    return a1 + 1;
}

BOOL AI_EvalFunc_ARRBitAnd_C(int* a1, int* a2, int* a3, int a4, int a5)
{
    int i;
    int v7;

    v7 = 0;
    for (i = 0; i < a4; ++i)
    {
        if (a2[i] != a5)
        {
            v7 = 1;
            if (((1 << (a2[i] & 0x1F)) & a1[(int)(a2[i] & 0xFFFFFFE0) >> 5]) != 0)
            {
                if (!a3[i])
                    return 0;
            }
            else if (a3[i])
            {
                return 0;
            }
        }
    }
    return v7 != 0;
}

tAINode* AI_EvalFunc_ARRBitAnd(tAINode* a1)
{
    int v1;
    char dest[52];
    int a4; 
    tVarStackElem a2;
    tVarStackElem v6;
    char v7[52];
    int a5;
    tVarStackElem v9;
    char v10[52]; 

    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    AI_GetVarFromStack(v10, &v9);
    AI_GetVarFromStack(v7, &v6);
    v1 = AI_EvalFunc_ARRBitAnd_C((int*)v6.ptr, (int*)v9.ptr, (int*)a2.ptr, a4, a5);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

BOOL AI_EvalFunc_ARRBitOr_C(int* a1, int* a2, int* a3, int a4, int a5)
{
    int i;

    for (i = 0; i < a4; ++i)
    {
        if (a2[i] != a5)
        {
            if (((1 << (a2[i] & 0x1F)) & a1[(int)(a2[i] & 0xFFFFFFE0) >> 5]) != 0)
            {
                if (a3[i])
                    return 1;
            }
            else if (!a3[i])
            {
                return 1;
            }
        }
    }
    return 0;
}

tAINode* AI_EvalFunc_ARRBitOr(tAINode* a1)
{
    int v1;
    char dest[52];
    int a4;
    tVarStackElem a2;
    tVarStackElem v6;
    char v7[52]; 
    int a5; 
    tVarStackElem v9;
    char v10[52]; 

    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    AI_GetVarFromStack(v10, &v9);
    AI_GetVarFromStack(v7, &v6);
    v1 = AI_EvalFunc_ARRBitOr_C((int*)v6.ptr, (int*)v9.ptr, (int*)a2.ptr, a4, a5);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

BOOL AI_EvalFunc_ARRBit4Test_C(int* a1, int a2, int a3, int a4, int a5)
{
    int v6;
    int v7[4];
    int v8;
    int i; 

    v7[0] = a2;
    v7[1] = a3;
    v7[2] = a4;
    v7[3] = a5;
    for (i = 0; i < 4; ++i)
    {
        if (v7[i])
        {
            v6 = v7[i];
            if (v6 > 10000)
                v6 -= 10000;
            v8 = (1 << (v6 & 0x1F)) & a1[(int)(v6 & 0xFFFFFFE0) >> 5];
            if (v8)
            {
                if (v7[i] > 10000)
                    return 0;
            }
            if (!v8 && v7[i] < 10000)
                return 0;
        }
    }
    return 1;
}

tAINode* AI_EvalFunc_ARRBit4Test(tAINode* a1)
{
    int v1;
    tVarStackElem a2;
    char dest[52];
    int v5;
    int a3;
    int a4;
    int a5;

    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    v5 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v1 = AI_EvalFunc_ARRBit4Test_C((int*)a2.ptr, v5, a3, a4, a5);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_ARRObjBestSightBVCenter_C(tGameObjectMain** a1, int a2, Vec3* a3, Vec3* a4, float a5)
{
    float v6;
    float v7 = 0.0f;
    float v8;
    Vec3 dest;
    Vec3 a2a;
    int v11; 
    tGameObjectMain* v12;
    int v13;

    MATH_NormalizeVector(a4, a4);
    v13 = a2 - 1;
    v11 = v13;
    v8 = -FLT_MAX;
    while (v13 >= 0)
    {
        v12 = a1[v13];
        if (v12 && v12 != (tGameObjectMain*)-1)
        {
            OBJ_BV_ComputeCenter(v12, &a2a);
            MATH_SubstractVec(&dest, &a2a, a3);
            if (a5 != 0.0)
                v7 = MATH_VecLen(&dest);
            MATH_NormalizeVector(&dest, &dest);
            v6 = a4->z * dest.z + a4->y * dest.y + dest.x * a4->x;
            if (a5 != 0.0)
                v6 = v6 - v7 / a5;
            if (v6 > (double)v8)
            {
                v8 = v6;
                v11 = v13;
            }
        }
        --v13;
    }
    return v11;
}

tAINode* AI_EvalFunc_ARRObjBestSightBVCenter(tAINode* arg0)
{
    int v1;
    char dest[52];
    Vec3 a3;
    int v5;
    float a5;
    tVarStackElem a2;
    Vec3 a1;

    a5 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&a1);
    AI_GetVecFromStack(&a3);
    v5 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v1 = AI_EvalFunc_ARRObjBestSightBVCenter_C((tGameObjectMain**)a2.ptr, v5, &a3, &a1, a5);
    AI_PutIntToStack(v1);
    return arg0 + 1;
}

void AI_EvalFunc_ARRGlobalIntSet_C(int* a1)
{
    ARR_gpi_Global = a1;
}

tAINode* AI_EvalFunc_ARRGlobalIntSet(tAINode* a1)
{
    tVarStackElem a2;
    char dest[52];

    AI_GetVarFromStack(dest, &a2);
    ARR_gpi_Global = (int*)a2.ptr;
    return a1 + 1;
}

void AI_EvalFunc_ARRGlobalIntFlagSet_C(int a2)
{
    AI_EvalFunc_ARRBitSet_C(ARR_gpi_Global, a2);
}

tAINode* AI_EvalFunc_ARRGlobalIntFlagSet(tAINode* a1)
{
    int v2;

    v2 = AI_GetIntFromStack();
    AI_EvalFunc_ARRGlobalIntFlagSet_C(v2);
    return a1 + 1;
}

void AI_EvalFunc_ARRGlobalIntFlagClear_C(int a2)
{
    AI_EvalFunc_ARRBitClear_C(ARR_gpi_Global, a2);
}

tAINode* AI_EvalFunc_ARRGlobalIntFlagClear(tAINode* a1)
{
    int v2;

    v2 = AI_GetIntFromStack();
    AI_EvalFunc_ARRGlobalIntFlagClear_C(v2);
    return a1 + 1;
}

BOOL AI_EvalFunc_ARRGlobalIntFlagTest_C(int a1)
{
    return AI_EvalFunc_ARRBitTest_C(ARR_gpi_Global, a1);
}

tAINode* AI_EvalFunc_ARRGlobalIntFlagTest(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_ARRGlobalIntFlagTest_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_ARRSortByFlags_C(struct sGameObjectMain** a1, int a2, int flagMustHave, int flagMustNothave, int whatToTest)
{
    tGameObjectMain* v6;
    int v8;
    tGameObjectMain* gao;

    v8 = 0;
    while (v8 < a2)
    {
        gao = a1[v8];
        if (gao)
        {
            if (OBJ_b_test_flag(gao, flagMustHave, flagMustNothave, whatToTest))
            {
                ++v8;
            }
            else
            {
                --a2;
                v6 = a1[v8];
                a1[v8] = a1[a2];
                a1[a2] = v6;
            }
        }
    }
    return a2;
}

tAINode* AI_EvalFunc_ARRSortByFlags(tAINode* a1)
{
    int v1;
    int v3;
    int flagMustHave;
    int flagMustNothave;
    tVarStackElem a2;
    char dest[52];
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v1 = AI_EvalFunc_ARRSortByFlags_C((tGameObjectMain**)a2.ptr, v3, flagMustHave, flagMustNothave, whatToTest);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_ARRObjBestSightInfoPhoto_C(struct sGameObjectMain** arg0, int arg4, Vec3* a3, Vec3* a1, float a5)
{
    double v5;
    float v7;
    float v8;
    float v9;
    float v10;
    Vec3 a2; 
    float v12;
    Vec3 v13; 
    int v14; 
    float v15;
    int v16; 
    float v17;
    tGameObjectMain* v18;
    int v19;
    int i;

    v14 = arg4;
    MATH_NormalizeVector(a1, a1);
    v19 = arg4 - 1;
    v16 = v19;
    v12 = -FLT_MAX;
    while (v19 >= 0)
    {
        v18 = arg0[v19];
        if (v18 && v18 != (tGameObjectMain*)-1)
        {
            AI_EvalFunc_OBJ_InfoPhotoPivotGet_C(v18, &v13);
            MATH_SubstractVec(&a2, &v13, a3);
            MATH_NormalizeVector(&a2, &a2);
            v17 = a1->z * a2.z + a1->y * a2.y + a2.x * a1->x;
            if (v17 > (double)v12)
                v12 = v17;
        }
        --v19;
    }
    v8 = (float)acos(v12);
    v15 = v8;
    if (a5 == 0.0)
        v5 = v15 + 0.087266468;
    else
        v5 = a5 * 3.1415927 / 180.0 + v15;
    v15 = (float)v5;
    v7 = (float)cos(v15);
    v9 = FLT_MAX;
    for (i = v14 - 1; i >= 0; --i)
    {
        v18 = arg0[i];
        if (v18)
        {
            if (v18 != (tGameObjectMain*)-1)
            {
                AI_EvalFunc_OBJ_InfoPhotoPivotGet_C(v18, &v13);
                MATH_SubstractVec(&a2, &v13, a3);
                v10 = MATH_VecLen(&a2);
                MATH_NormalizeVector(&a2, &a2);
                v17 = a1->z * a2.z + a1->y * a2.y + a2.x * a1->x;
                if (v17 > (double)v7 && v10 < (double)v9)
                {
                    v9 = v10;
                    v16 = i;
                }
            }
        }
    }
    return v16;
}

tAINode* AI_EvalFunc_ARRObjBestSightInfoPhoto(tAINode* arg0)
{
    int v1;
    char dest[52];
    Vec3 a3;
    int arg4;
    float a5;
    tVarStackElem a2;
    Vec3 a1; 

    a5 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&a1);
    AI_GetVecFromStack(&a3);
    arg4 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    v1 = AI_EvalFunc_ARRObjBestSightInfoPhoto_C((tGameObjectMain**)a2.ptr, arg4, &a3, &a1, a5);
    AI_PutIntToStack(v1);
    return arg0 + 1;
}