#include "../ai.h"
#include "ai_mathfunctions.h"

Mtx4x4 st_M1_459;
Mtx4x4 st_M2_460;

float AI_gf_Epsilon = 0.000099999997f;

int AI_EvalFunc_MATHBitSet_C(int a1, char a2)
{
    return (1 << a2) | a1;
}

tAINode* AI_EvalFunc_MATHBitSet(tAINode* a1)
{
    int v1;
    char v3;
    int v4;

    v3 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_MATHBitSet_C(v4, v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_MATHBitChange_C(int a1, char a2)
{
    int v3;

    if (((1 << a2) & a1) != 0)
        v3 = ~(1 << a2) & a1;
    else
        v3 = (1 << a2) | a1;
    return v3;
}

tAINode* AI_EvalFunc_MATHBitChange(tAINode* a1)
{
    int v1;
    char v3;
    int v4;

    v3 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_MATHBitChange_C(v4, v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_MATHBitReset_C(int a1, char a2)
{
    return ~(1 << a2) & a1;
}

tAINode* AI_EvalFunc_MATHBitReset(tAINode* a1)
{
    int v1;
    char v3;
    int v4;

    v3 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_MATHBitReset_C(v4, v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

BOOL AI_EvalFunc_MATHBitTest_C(int a1, char a2)
{
    return ((1 << a2) & a1) != 0;
}

tAINode* AI_EvalFunc_MATHBitTest(tAINode* a1)
{
    int v1;
    char v3;
    int v4;

    v3 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_MATHBitTest_C(v4, v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

float AI_EvalFunc_MATHSinus_C(float f) {
    return sinf(f);
}

tAINode* AI_EvalFunc_MATHSinus(tAINode* a1)
{
    float v2;
    float X;

    X = AI_GetFloatFromStack();
    v2 = AI_EvalFunc_MATHSinus_C(X);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

float AI_EvalFunc_MATHCosinus_C(float f) {
    return cosf(f);
}

tAINode* AI_EvalFunc_MATHCosinus(tAINode* a1)
{
    float v2;
    float X;

    X = AI_GetFloatFromStack();
    v2 = AI_EvalFunc_MATHCosinus_C(X);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

float AI_EvalFunc_MATHRandFloat_C(float a1, float a2)
{
    float v3;
    float v4;

    if (a1 == a2)
        return a1;
    v3 = (float)rand();
    v4 = (float)(v3 / 32767.0);
    return (float)((a2 - a1) * v4 + a1);
}

tAINode* AI_EvalFunc_MATHRandFloat(tAINode* a1)
{
    float v2;
    float v3;
    float v4;

    v3 = AI_GetFloatFromStack();
    v4 = AI_GetFloatFromStack();
    v2 = AI_EvalFunc_MATHRandFloat_C(v4, v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_MATHRandInt_C(int a1, int a2)
{
    int v3;
    int v4;

    if (a1 == a2)
        return a1;
    v4 = a1 + (a2 - a1) * RND_JADErand() / 0x7FFF;
    if (v4 == a2)
        v3 = v4 - 1;
    else
        v3 = v4;
    return v3;
}

tAINode* AI_EvalFunc_MATHRandInt(tAINode* a1)
{
    int v1;
    int v3;
    int v4;

    v4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_MATHRandInt_C(v3, v4);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

float AI_EvalFunc_MATHAbsFloat_C(float f) {
    return (float)fabs(f);
}

tAINode* AI_EvalFunc_MATHAbsFloat(tAINode* a1)
{
    float v2;
    float v3;

    v3 = AI_GetFloatFromStack();
    v2 = AI_EvalFunc_MATHAbsFloat_C(v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_MATHAbsInt(tAINode* a1)
{
    int v2;

    v2 = AI_GetIntFromStack();
    if (v2 < 0)
        AI_PutIntToStack(-v2);
    else
        AI_PutIntToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_MATHVecDistance(tAINode* a1)
{
    float v2;
    Vec3 dest;
    Vec3* a3;
    Vec3* a2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    MATH_SubstractVec(&dest, a2, (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    v2 = MATH_VecLen(&dest);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_MATHVecNorm(tAINode* a1)
{
    float v2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v2 = MATH_VecLen((Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

void AI_EvalFunc_MATHVecNormalize_C(Vec3* src, Vec3* dest)
{    
    if (MATH_b_NulVector(src))
    {
        dest->x = 1.0f;
        dest->y = 0.0f;
        dest->z = 0.0f;
    }
    else
    {
        MATH_NormalizeVector(dest, src);
    }
}

tAINode* AI_EvalFunc_MATHVecNormalize(tAINode* a1)
{
    Vec3 a1a;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_MATHVecNormalize_C((Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

void AI_EvalFunc_MATHVecSetNormVector_C(Vec3* a1, float a2)
{
    MATH_NormalizeVector(a1, a1);
    a1->x = a2 * a1->x;
    a1->y = a2 * a1->y;
    a1->z = a2 * a1->z;
}

tAINode* AI_EvalFunc_MATHVecSetNormVector(tAINode* a1)
{
    float v2;
    char v3[52];
    tVarStackElem a2;

    v2 = AI_GetFloatFromStack();
    AI_GetVarFromStack(v3, &a2);
    AI_EvalFunc_MATHVecSetNormVector_C((Vec3*)a2.ptr, v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_MATHVecSetNormalize(tAINode* a1)
{
    char dest[52];
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    MATH_NormalizeEqualVector((Vec3*)a2.ptr);
    return a1 + 1;
}

void  AI_EvalFunc_MATHVecSetHorzNormalize_C(Vec3* v)
{
    v->z = 0.0f;

    if (MATH_b_NulVector(v))
    {
        v->x = 1.0f;
        v->y = 0.0f;
        v->z = 0.0f;
    }
    else
    {
        MATH_NormalizeEqualVector(v);
    }
}

tAINode* AI_EvalFunc_MATHVecSetHorzNormalize(tAINode* a1)
{
    char dest[52];
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    AI_EvalFunc_MATHVecSetHorzNormalize_C((Vec3*)a2.ptr);
    return a1 + 1;
}

float AI_EvalFunc_MATHVecDotProduct_C(Vec3* v1, Vec3* v2) {
    return MATH_DotProduct(v1, v2);
}

tAINode* AI_EvalFunc_MATHVecDotProduct(tAINode* a1)
{
    Vec3* v1, *v2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v1 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v2 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];

    AI_PutFloatToStack(AI_EvalFunc_MATHVecDotProduct_C(v1, v2));
    return a1 + 1;
}

void AI_EvalFunc_MATHVecCrossProduct_C(Vec3* a1, Vec3* a2, Vec3* a3)
{
    MATH_CrossProduct(a3, a1, a2);
}

tAINode* AI_EvalFunc_MATHVecCrossProduct(tAINode* a1)
{
    Vec3* a2;
    Vec3 a1a;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_MATHVecCrossProduct_C((Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a2, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

tAINode* AI_EvalFunc_MATHDistPointLine(tAINode* a1)
{
    float v2;
    Vec3 v1;
    Vec3 v4;
    Vec3 a1a;

    AI_GetVecFromStack(&a1a);
    AI_GetVecFromStack(&v4);
    AI_GetVecFromStack(&v1);
    v2 = MATH_f_PointLineDistance(&v1, &v4, &a1a);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

void AI_EvalFunc_MATHProjPointLine_C(Vec3* a1, Vec3* a2, Vec3* a3, Vec3* a4)
{
    MATH_PointLineProjection(a4, a1, a2, a3);
}

tAINode* AI_EvalFunc_MATHProjPointLine(tAINode* a1)
{
    Vec3 v2;
    Vec3 a2;
    Vec3 a1a;
    Vec3 a3;

    AI_GetVecFromStack(&a3);
    AI_GetVecFromStack(&a2);
    AI_GetVecFromStack(&v2);
    AI_EvalFunc_MATHProjPointLine_C(&v2, &a2, &a3, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

void AI_EvalFunc_MATHProjPointSegment_C(Vec3* a1, Vec3* a2, Vec3* a3, Vec3* a4)
{
    MATH_PointSegmentProjection(a4, a1, a2, a3);
}

tAINode* AI_EvalFunc_MATHProjPointSegment(tAINode* a1)
{
    Vec3 v2;
    Vec3 a2;
    Vec3 a1a;
    Vec3 a3;

    AI_GetVecFromStack(&a3);
    AI_GetVecFromStack(&a2);
    AI_GetVecFromStack(&v2);
    AI_EvalFunc_MATHProjPointSegment_C(&v2, &a2, &a3, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

void AI_EvalFunc_MATHProjPointParabol_C(Vec3* a1, Vec3* a2, Vec3* a3, Vec3* a4, float a7, Vec3* a6)
{
    Vec3 a2a;

    MATH_PointParabolProjection3D(a6, &a2a, a1, a2, a3, a4, a7);
}

tAINode* AI_EvalFunc_MATHProjPointParabol(tAINode* a1)
{
    Vec3 a2;
    float a7;
    Vec3 a1a;
    Vec3 a4;
    Vec3 v6;
    Vec3 a3;

    a7 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&a4);
    AI_GetVecFromStack(&a3);
    AI_GetVecFromStack(&a2);
    AI_GetVecFromStack(&v6);
    AI_EvalFunc_MATHProjPointParabol_C(&v6, &a2, &a3, &a4, a7, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

void AI_EvalFunc_MATHMatrixBlend_C(Vec3* a2, Vec3* a3, Vec3* a4, Vec3* a5, Vec3* a6, Vec3* a7, float amount, char a9)
{

    Mtx4x4 v9;

    if (amount < 0.0)
        amount = 0.0;
    if (amount > 1.0)
        amount = 1.0;
    if (a9)
    {
        MATH_CrossProduct(&st_M1_459.v1, a5, a4);
        MATH_CrossProduct(&st_M2_460.v1, a7, a6);
        st_M2_460.v3.x = a7->x; // TODO: atnezni megegyszer
        st_M2_460.v3.y = a7->y;
        st_M2_460.v3.z = a7->z;

        st_M1_459.v3.x = a5->x;
        st_M1_459.v3.y = a5->y;
        st_M1_459.v3.z = a5->z;

        st_M2_460.v2.x = -a6->x;
        st_M2_460.v2.y = -a6->y;
        st_M2_460.v2.z = -a6->z;

        st_M1_459.v2.x = -a4->x;
        st_M1_459.v2.y = -a4->y;
        st_M1_459.v2.z = -a4->z;
    }
    MATH_MatrixBlend(&v9, &st_M1_459, &st_M2_460, amount, a9);
    a2->x = -v9.v2.x;
    a2->y = -v9.v2.y;
    a2->z = -v9.v2.z;
    *a3 = v9.v3;
}

tAINode* AI_EvalFunc_MATHMatrixBlend(tAINode* a1)
{
    float a8;
    char v3[52];
    tVarStackElem a2;
    Vec3 a4;
    char dest[52];
    Vec3 a1a;
    Vec3 a6;
    Vec3 a5;
    tVarStackElem v10;

    a8 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&a1a);
    AI_GetVecFromStack(&a6);
    AI_GetVecFromStack(&a5);
    AI_GetVecFromStack(&a4);
    AI_GetVarFromStack(dest, &a2);
    AI_GetVarFromStack(v3, &v10);
    AI_EvalFunc_MATHMatrixBlend_C((Vec3*)v10.ptr, (Vec3*)a2.ptr, &a4, &a5, &a6, &a1a, a8, 1);
    return a1 + 1;
}

void AI_EvalFunc_MATHMatrixVecLocalToGlobal_C(Vec3* a2, Vec3* a3, Vec3* a4, Vec3* a5)
{
    Mtx4x4 v5;

    MATH_CrossProduct(&v5.v1, a5, a4);
    v5.v3 = *a5;
    v5.v2.x = -a4->x;
    v5.v2.y = -a4->y;
    v5.v2.z = -a4->z;
    MATH_TransformVectorNoScale(a2, &v5, a3);
}

tAINode* AI_EvalFunc_MATHMatrixVecLocalToGlobal(tAINode* a1)
{
    Vec3 a4;
    Vec3 a5;
    Vec3 a1a;
    Vec3 a3;

    AI_GetVecFromStack(&a5);
    AI_GetVecFromStack(&a4);
    AI_GetVecFromStack(&a3);
    AI_EvalFunc_MATHMatrixVecLocalToGlobal_C(&a1a, &a3, &a4, &a5);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

float AI_EvalFunc_MATHFloatLimit_C(float num, float min, float max) {

    if (num <= max)
    {
        if (num >= min)
            return num;
        else
            return min;
    }
    else
    {
        return max;
    }
}

tAINode* AI_EvalFunc_MATHFloatLimit(tAINode* aiNode)
{
    float num, min, max;

    max = AI_GetFloatFromStack();
    min = AI_GetFloatFromStack();
    num = AI_GetFloatFromStack();

    AI_PutFloatToStack(AI_EvalFunc_MATHFloatLimit_C(num, min, max));

    return aiNode + 1;
}

float AI_EvalFunc_MATHFloatMin_C(float num1, float num2) {
    if (num1 > num2)
        return num2;
    else
        return num1;
}

tAINode* AI_EvalFunc_MATHFloatMin(tAINode* aiNode)
{
    float num1, num2;

    num1 = AI_GetFloatFromStack();
    num2 = AI_GetFloatFromStack();
    
    AI_PutFloatToStack(AI_EvalFunc_MATHFloatMin_C(num1, num2));

    return aiNode + 1;
}

float AI_EvalFunc_MATHFloatMax_C(float num1, float num2) {
    if (num2 < num1)
        return num1;
    else
        return num2;
}

tAINode* AI_EvalFunc_MATHFloatMax(tAINode* aiNode)
{
    float num1, num2;

    num1 = AI_GetFloatFromStack();
    num2 = AI_GetFloatFromStack();

    AI_PutFloatToStack(AI_EvalFunc_MATHFloatMax_C(num1, num2));

    return aiNode + 1;
}

BOOL AI_EvalFunc_MATHFloatNullEps_C(float a1)
{
    double v2;

    v2 = fabs(a1);
    return v2 < (double)AI_gf_Epsilon;
}

tAINode* AI_EvalFunc_MATHFloatNullEps(tAINode* a1)
{
    int v1;
    float v3;

    v3 = AI_GetFloatFromStack();
    v1 = AI_EvalFunc_MATHFloatNullEps_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_MATHEpsilonSet_C(float a1)
{
    AI_gf_Epsilon = a1;
}

tAINode* AI_EvalFunc_MATHEpsilonSet(tAINode* a1)
{
    float a1a;

    a1a = AI_GetFloatFromStack();
    AI_EvalFunc_MATHEpsilonSet_C(a1a);
    return a1 + 1;
}

float AI_EvalFunc_MATHEpsilonGet_C()
{
    return AI_gf_Epsilon;
}

tAINode* AI_EvalFunc_MATHEpsilonGet(tAINode* a1)
{
    float v2;

    v2 = AI_EvalFunc_MATHEpsilonGet_C();
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

void AI_EvalFunc_MATHVecBlend_C(Vec3* a1, Vec3* a2, float a3, Vec3* a4)
{
    MATH_BlendVector(a4, a1, a2, a3);
}

tAINode* AI_EvalFunc_MATHVecBlend(tAINode* a1)
{
    Vec3* a2;
    float a3;
    Vec3 a1a;

    a3 = AI_GetFloatFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_MATHVecBlend_C((Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a2, a3, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

void AI_EvalFunc_MATHVecRotate_C(Vec3* a1, Vec3* a2, float a3, Vec3* a4)
{
    float v4;
    float v5;
    Vec3 a3a;
    float v7;
    Vec3* v8;
    Vec3* v9;
    Vec3 src2;
    Vec3 dest;
    float v12;
    Vec3 a2a; 
    float v14;
    float v15;
    float v16;
    float v17;

    v8 = &src2;
    v9 = a1;
    src2 = *a1;
    v12 = src2.z * a2->z + src2.y * a2->y + src2.x * a2->x;
    v14 = a2->z * a2->z + a2->y * a2->y + a2->x * a2->x;
    v12 = v12 / v14;
    v15 = v12 * a2->x;
    v16 = a2->y * v12;
    v17 = a2->z * v12;
    src2.x = src2.x - v15;
    src2.y = src2.y - v16;
    src2.z = src2.z - v17;
    v5 = (float)cos(a3);
    v7 = v5;
    a2a.x = src2.x * v5;
    a2a.y = src2.y * v5;
    a2a.z = src2.z * v5;
    MATH_NormalizeEqualVector(a2);
    MATH_CrossProduct(&dest, a2, &src2);
    v4 = (float)sin(a3);
    a3a.x = dest.x * v4;
    a3a.y = dest.y * v4;
    a3a.z = dest.z * v4;
    MATH_VecAddVec(&a2a, &a2a, &a3a);
    a2a.x = a2a.x + v15;
    a2a.y = a2a.y + v16;
    a2a.z = a2a.z + v17;
    *a4 = a2a;
}

tAINode* AI_EvalFunc_MATHVecRotate(tAINode* a1)
{
    Vec3 v2;
    Vec3 a2;
    Vec3 a1a;
    float a3;

    a3 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&a2);
    AI_GetVecFromStack(&v2);
    AI_EvalFunc_MATHVecRotate_C(&v2, &a2, a3, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

tAINode* AI_EvalFunc_MATHVecSetNull(tAINode* a1)
{
    Vec3* v2;
    char dest[52];
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    v2 = (Vec3*)a2.ptr;
    v2->x = 0.0f;
    v2->y = 0.0f;
    v2->z = 0.0f;
    return a1 + 1;
}

float AI_EvalFunc_MATHFloatRound_C(float a1, float a2) // TODO: ellenorizni ibztos jo-e
{
    float v3;
    double v4;

    v4 = 1.0 / a2 * a1 + MATH_gd_magicDoubleForRound;
    v3 = (float)*(int32_t *)&v4;
    return v3 * a2;
}

tAINode* AI_EvalFunc_MATHFloatRound(tAINode* a1)  // TODO: ellenorizni ibztos jo-e + AI_EvalFunc_MATHFloatRound_C inline
{
    float v2;
    float v3;
    double v4;
    float v5;
    float v6;

    v5 = AI_GetFloatFromStack();
    v6 = AI_GetFloatFromStack();
    v4 = 1.0 / v5 * v6 + MATH_gd_magicDoubleForRound;
    v3 = (float)v4;
    v2 = v3 * v5;
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_MATHVecRound(tAINode* a1)
{
    char dest[52];
    float precision;
    tVarStackElem a2;

    precision = AI_GetFloatFromStack();
    AI_GetVarFromStack(dest, &a2);
    MATH_RoundVectorWithPrecision((Vec3*)a2.ptr, precision);
    return a1 + 1;
}

tAINode* AI_EvalFunc_MATHVecCos(tAINode* a1)
{
    float v2;
    Vec3* vec2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    vec2 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v2 = MATH_f_VecCos((Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], vec2);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_MATHVecSin(tAINode* a1)
{
    float v2;
    Vec3* v3;
    Vec3* v4;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v3 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v4 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v2 = MATH_f_VecSin((Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], v4, v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_MATHVecAngle(tAINode* a1)
{
    float v2;
    Vec3* a3;
    Vec3* a2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v2 = MATH_f_VecAngle((Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a2, a3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_MATHVecAbsAngle(tAINode* a1)
{
    float v2;
    float v3;
    Vec3* vec2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    vec2 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v3 = MATH_f_VecCos((Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], vec2);// MATH_f_VecAbsAngle inline
    if (v3 < 1.0)
    {
        if (v3 > -1.0)
        {
            v2 = (float)acos(v3);
            AI_PutFloatToStack(v2);
        }
        else
        {
            AI_PutFloatToStack(3.1415927f);
        }
    }
    else
    {
        AI_PutFloatToStack(0.0);
    }
    return a1 + 1;
}

void AI_EvalFunc_MATHVecBlendRotate_C(Vec3* a2, Vec3* a3, float a4, Vec3* a5)
{
    float v5;
    BOOL v6; 
    float v7;
    float v9;
    Mtx4x4 v11;
    float v12;
    Vec3* v13;
    Vec3* v14;
    BOOL v15; 
    Vec3* v19;
    Vec3* v20;
    Vec3 v21; 
    Vec3* v22;
    Vec3* v23;
    BOOL v24; 
    Vec3 v28; 
    Vec3* v29;
    Vec3* v30;
    BOOL v31; 
    Vec3* v35;
    Vec3* v36;
    BOOL v37; 
    char v41; 

    if (a4 < 0.0)
        a4 = 0.0;
    if (a4 > 1.0)
        a4 = 1.0;
    v41 = 0;
   
    v37 = MATH_b_NulVectorWithEpsilon(a2, 0.0000001f);
    if (v37)
    {
        v41 = 1;
        v36 = a5;
        v35 = a3;
        a5->x = a3->x;
        v36->y = v35->y;
        v36->z = v35->z;
    }
   
    v31 = MATH_b_NulVectorWithEpsilon(a3, 0.0000001f);
    if (v31)
    {
        v41 = 1;
        v30 = a5;
        v29 = a2;
        a5->x = a2->x;
        v30->y = v29->y;
        v30->z = v29->z;
    }
    if (!v41)
    {
        MATH_CrossProduct(&v28, a2, a3);        
        v24 = MATH_b_NulVectorWithEpsilon(&v28, 0.00001f); // 0.0000099999997 (ps2)
        if (v24)
        {
            if (a3->z * a2->z + a3->y * a2->y + a2->x * a3->x <= 0.0f)
            {
                v20 = &v21;
                v19 = &MATH_gst_BaseVectorI;
                v21 = MATH_gst_BaseVectorI;
                MATH_CrossProduct(&v28, a2, &v21);                
                v15 = MATH_b_NulVectorWithEpsilon(&v28, 0.001f);
                if (v15)
                {
                    v14 = &v21;
                    v13 = &MATH_gst_BaseVectorJ;
                    v21 = MATH_gst_BaseVectorJ;
                    MATH_CrossProduct(&v28, a2, &v21);
                }
            }
            else
            {
                v23 = a5;
                v22 = a2;
                a5->x = a2->x;
                v23->y = v22->y;
                v23->z = v22->z;
                v41 = 1;
            }
        }
        if (!v41)
        {
            MATH_NormalizeEqualVector(&v28);
            v12 = MATH_f_VecAngle(a2, a3, &v28) * a4;
            MATH_MakeRotationMatrix_AxisAngle(&v11, &v28, v12, 0, 1);
            MATH_TransformVector(a5, &v11, a2);
        }
    }
    v9 = (float)sqrt(MATH_VecLenSquared(a2));
    v7 = (float)sqrt(MATH_VecLenSquared(a3));
    v6 = MATH_b_NulVectorWithEpsilon(a5, 0.0000001f);
    if (v6)
    {
        a5->x = 0.0;
        a5->y = 0.0;
        a5->z = 0.0;
    }
    else
    {
        v5 = MATH_LinearInterpolateFloats(v9, v7, a4);
        MATH_NormalizeVector(a5, a5);
        a5->x = v5 * a5->x;
        a5->y = v5 * a5->y;
        a5->z = v5 * a5->z;
    }
}

tAINode* AI_EvalFunc_MATHVecBlendRotate(tAINode* a1)
{
    float a4;
    Vec3* a3;
    Vec3 a5;

    a4 = AI_GetFloatFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_MATHVecBlendRotate_C((Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a3, a4, &a5);
    AI_PutVecToStack(&a5);
    return a1 + 1;
}

float AI_EvalFunc_MATHFloatBlend_C(float f1, float f2, float factor) {

    if (factor < 0.0f)
        factor = 0.0f;
    if (factor > 1.0f)
        factor = 1.0f;
    return f2 * factor + (1.0f - factor) * f1;
}

tAINode* AI_EvalFunc_MATHFloatBlend(tAINode* a1)
{
    float f2;
    float factor;
    float f1;

    factor = AI_GetFloatFromStack();
    f2 = AI_GetFloatFromStack();
    f1 = AI_GetFloatFromStack();

    AI_PutFloatToStack(AI_EvalFunc_MATHFloatBlend_C(f1, f2, factor));
    return a1 + 1;
}

float AI_EvalFunc_MATHRandBino_C(float a1, float a2, float a3, float a4)
{
    float v5;
    float v6;
    float v7;
    int v8;
    float v9;
    int v10;
    float a1a;

    a1a = (a1 - a4) / a2;
    v8 = (int)((a3 - a4) / a2);
    v10 = 0;
    v9 = 0.0;
    while (v10 < v8)
    {
        v6 = (float)rand();
        v7 = (float)(v6 / 32767.0);
        v5 = (float)v8;
        if ((v5 - 0.0) * v7 + 0.0 < a1a)
            v9 = v9 + a2;
        ++v10;
    }
    return v9 + a4;
}

tAINode* AI_EvalFunc_MATHRandBino(tAINode* a1)
{
    float v2;
    float a2;
    float a1a;
    float a4;
    float a3;

    a4 = AI_GetFloatFromStack();
    a3 = AI_GetFloatFromStack();
    a2 = AI_GetFloatFromStack();
    a1a = AI_GetFloatFromStack();
    v2 = AI_EvalFunc_MATHRandBino_C(a1a, a2, a3, a4);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

void AI_EvalFunc_MATHVecInCone_C(Vec3* a1, Vec3* a2, float a3, int a4, Vec3* a5)
{
    float v5;
    float v6;
    float v7;
    Vec3 v8; 
    float v12;
    Vec3 src2;
    float v14;
    Vec3 a2a; 
    float v16;
    Vec3 dest;

    v16 = 1.0;
    if ((a4 & 1) == 0)
        MATH_NormalizeEqualVector(a2);
    v14 = a1->z * a2->z + a1->y * a2->y + a2->x * a1->x;
    if ((a4 & 2) == 0)
    {
        v16 = MATH_VecLen(a1);
        v14 = v14 / v16;
    }
    v7 = (float)cos(a3);
    if (v14 <= (double)v7)
    {
        v6 = (float)fabs(v14 - -1.0);
        if (v6 < 0.0000099999997)
        {
            if (a1->y == 0.0)
            {
                src2.x = 0.0;
                src2.y = 1.0;
            }
            else
            {
                src2.x = 1.0;
                src2.y = 0.0;
            }
            src2.z = 0.0;
            MATH_CrossProduct(&a2a, a2, &src2);
        }
        else
        {
            MATH_CrossProduct(&a2a, a2, a1);
        }
        MATH_CrossProduct(&dest, &a2a, a2);
        MATH_NormalizeEqualVector(&dest);
        v5 = (float)sin(a3);
        dest.x = dest.x * v5;
        dest.y = dest.y * v5;
        dest.z = dest.z * v5;
        v8.x = v7 * a2->x;
        v8.y = a2->y * v7;
        v8.z = a2->z * v7;
        MATH_VecAddVec(&dest, &dest, &v8);
        if ((a4 & 6) == 0)
        {
            dest.x = dest.x * v16;
            dest.y = dest.y * v16;
            dest.z = dest.z * v16;
        }
        *a5 = dest;
    }
    else
    {
        if ((a4 & 6) == 4)
        {
            v12 = (float)(1.0 / v16);
            a1->x = v12 * a1->x;
            a1->y = v12 * a1->y;
            a1->z = v12 * a1->z;
        }
        a5->x = a1->x;
        a5->y = a1->y;
        a5->z = a1->z;
    }
}

tAINode* AI_EvalFunc_MATHVecInCone(tAINode* a1)
{
    int a4;
    Vec3* a2;
    float a3;
    Vec3 a5;

    a4 = AI_GetIntFromStack();
    a3 = AI_GetFloatFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_MATHVecInCone_C((Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a2, a3, a4, &a5);
    AI_PutVecToStack(&a5);
    return a1 + 1;
}

void AI_EvalFunc_MATHVecGlobalToLocal_C(struct sGameObjectMain* a1, Vec3* a2, Vec3* a3)
{
    Mtx4x4* v3;

    v3 = OBJ_pst_GetAbsoluteMatrix(a1);
    MATH_VectorGlobalToLocal(a3, v3, a2);
}

tAINode* AI_EvalFunc_MATHVecGlobalToLocal(tAINode* a1)
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
    AI_EvalFunc_MATHVecGlobalToLocal_C(v5, &a2, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

void AI_EvalFunc_MATHVecLocalToGlobal_C(struct sGameObjectMain* a1, Vec3* a2, Vec3* a3)
{
    Mtx4x4* v3;

    v3 = OBJ_pst_GetAbsoluteMatrix(a1);
    MATH_TransformVectorNoScale(a3, v3, a2);
}

tAINode* AI_EvalFunc_MATHVecLocalToGlobal(tAINode* a1)
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
    AI_EvalFunc_MATHVecLocalToGlobal_C(v5, &a2, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

BOOL AI_EvalFunc_MTHVecNull_C(Vec3* a1)
{
    if (fabs(a1->x) != 0.0)
        return 0;
    if (fabs(a1->y) != 0.0)
        return 0;
    return fabs(a1->z) == 0.0;
}

tAINode* AI_EvalFunc_MTHVecNull(tAINode* a1)
{
    BOOL v1;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v1 = AI_EvalFunc_MTHVecNull_C((Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

BOOL AI_EvalFunc_MTHVecNullEpsilon_C(Vec3* a1)
{
    if (fabs(a1->x) > AI_gf_Epsilon)
        return 0;
    if (fabs(a1->y) <= AI_gf_Epsilon)
        return fabs(a1->z) <= AI_gf_Epsilon;
    return 0;
}

tAINode* AI_EvalFunc_MTHVecNullEpsilon(tAINode* a1)
{
    BOOL v1;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v1 = AI_EvalFunc_MTHVecNullEpsilon_C((Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

BOOL AI_EvalFunc_MTHVecNullToler_C(Vec3* a1, float a2)
{
    if (fabs(a1->x) > a2)
        return 0;
    if (fabs(a1->y) <= a2)
        return fabs(a1->z) <= a2;
    return 0;
}

tAINode* AI_EvalFunc_MTHVecNullToler(tAINode* a1)
{
    BOOL v1;
    float a2;

    a2 = AI_GetFloatFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v1 = AI_EvalFunc_MTHVecNullToler_C((Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a2);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

BOOL AI_EvalFunc_MTHFloatNullToler_C(float a1, float a2)
{
    float v3;

    v3 = (float)fabs(a1);
    return v3 < (double)a2;
}

tAINode* AI_EvalFunc_MTHFloatNullToler(tAINode* a1)
{
    int v1;
    float v3;
    float v4;

    v3 = AI_GetFloatFromStack();
    v4 = AI_GetFloatFromStack();
    v1 = AI_EvalFunc_MTHFloatNullToler_C(v4, v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

float AI_EvalFunc_MTHFloatSign_C(float a1)
{
    double result;
    uint32_t tmp;

    if (a1 == 0.0)
        result = 1.0;
    else {
        tmp = (*(uint32_t*)&a1) & 0x80000000 | 0x3F800000;
        result = *(float*)&tmp;
    }
    return (float)result;
}

tAINode* AI_EvalFunc_MTHFloatSign(tAINode* a1)
{
    float v2;
    float a1a;

    a1a = AI_GetFloatFromStack();
    v2 = AI_EvalFunc_MTHFloatSign_C(a1a);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_MTHIntSign_C(int a1)
{
    if (a1 > 0)
        return 1;
    if (a1 < 0)
        a1 = -1;
    return a1;
}

tAINode* AI_EvalFunc_MTHIntSign(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_MTHIntSign_C(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_MTHFloatSqrt(tAINode* a1)
{
    float X;
    float Xa;

    X = AI_GetFloatFromStack();
    Xa = sqrtf(X);
    AI_PutFloatToStack(Xa);
    return a1 + 1;
}

tAINode* AI_EvalFunc_MTHLn(tAINode* a1)
{
    float v2;
    float v3;

    v3 = AI_GetFloatFromStack();
    v2 = (float)log(v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_MTHLog10(tAINode* a1)
{
    float v2;
    float v3;

    v3 = AI_GetFloatFromStack();
    v2 = (float)log10(v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_MTHModulo_C(int a1, int a2)
{
    int result;

    if (a2)
        result = a1 % a2;
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_MTHModulo(tAINode* a1)
{
    int v1;
    int v3;
    int v4;

    v3 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_MTHModulo_C(v4, v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}