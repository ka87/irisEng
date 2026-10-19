#include "ai_dynfunctions.h"
#include "../gameobject.h"

tAINode* AI_EvalFunc_DYNSpeedReachInTime(tAINode* a1)
{
    int v1;
    tGameObjectBasic* v3;
    tGameObjectMain* v4;
    Vec3 a3;
    tDyn* a1a;
    float a2;
    float a5;
    Vec3 a4;
    tGameObjectMain* v10;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v10 = v4;
    a5 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&a4);
    v3 = v10->basic;
    if (v3)
        a1a = v3->dyn;
    else
        a1a = 0;
    if (a1a)
    {
        v1 = DYN_b_SpeedReachInTime(a1a, &a2, &a3, &a4, a5);
        AI_PutIntToStack(v1);
    }
    else
    {
        AI_PutIntToStack(0);
    }
    return a1 + 1;
}

int AI_EvalFunc_DYNSpeedSetVector_C(struct sGameObjectMain* a1, Vec3* a2)
{
    tGameObjectBasic* v3;

    v3 = a1->basic;
    if (v3)
        DYN_SetSpeedVector(v3->dyn, a2);
    else
        DYN_SetSpeedVector(0, a2);
    return 1;
}

tAINode* AI_EvalFunc_DYNSpeedSetVector(tAINode* a1)
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
    v1 = AI_EvalFunc_DYNSpeedSetVector_C(v5, &a1a);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_DYNSpeedGetVector_C(struct sGameObjectMain* a1, Vec3* a2)
{
    tGameObjectBasic* v2;
    tDyn* a1a;

    v2 = a1->basic;
    if (v2)
        a1a = v2->dyn;
    else
        a1a = 0;
    if (a1a)
        DYN_SpeedGetVector(a1a, a2);
    else
        *a2 = MATH_gst_NulVector;
}

void AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(Vec3* a2)
{
    tGameObjectBasic* v1;
    tDyn* a1;

    v1 = AI_gpst_CurrentGameObject->basic;
    if (v1)
        a1 = v1->dyn;
    else
        a1 = 0;
    if (a1)
        DYN_SpeedGetVector(a1, a2);
    else
        *a2 = MATH_gst_NulVector;
}

tAINode* AI_EvalFunc_DYNSpeedGetVector(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a2;
    tGameObjectMain* a1a;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1a = v2;
    AI_EvalFunc_DYNSpeedGetVector_C(v2, &a2);
    AI_PutVecToStack(&a2);
    return a1 + 1;
}

int AI_EvalFunc_DYNMassSet_C(tGameObjectMain* a1, float a2)
{
    tGameObjectBasic* v3;
    tDyn* v4;

    v3 = a1->basic;
    if (v3)
        v4 = v3->dyn;
    else
        v4 = 0;
    if (!v4)
        return 0;
    v4->mass = (float)(1.0 / a2);
    return 1;
}

tAINode* AI_EvalFunc_DYNMassSet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    float a2;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    v1 = AI_EvalFunc_DYNMassSet_C(v3, a2);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_DYNMaxStepSizeSet(tAINode* a1)
{
    tGameObjectBasic* v2;
    tGameObjectMain* v3;
    float v4;
    tDyn* v5;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = AI_GetFloatFromStack();
    v2 = v3->basic;
    if (v2)
        v5 = v2->dyn;
    else
        v5 = 0;
    if (v5)
        v5->maxStepSize = v4;
    return a1 + 1;
}

float AI_EvalFunc_DYNMassGet_C(tGameObjectMain* a1)
{
    double result;
    tGameObjectBasic* v2;
    tDyn* v3;

    v2 = a1->basic;
    if (v2)
        v3 = v2->dyn;
    else
        v3 = 0;
    if (v3)
        result = 1.0 / v3->mass;
    else
        result = 0.0;
    return (float)result;
}

tAINode* AI_EvalFunc_DYNMassGet(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_DYNMassGet_C(v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

BOOL AI_EvalFunc_DYNOn_C(struct sGameObjectMain* a1)
{
    tGameObjectBasic* v2;
    int v3;
    int v4;
    tDyn* v5;
    tWorld* v6;

    if (a1)
        v4 = a1->flags;
    else
        v4 = 0;
    OBJ_ChangeIdentityFlags(a1, v4 | 4, -1);
    v6 = WOR_gpst_CurrentWorld;
    if (AI_C_Callback == 1)
    {
        v3 = a1->flags;
        ((tableElemStructPF*)(WOR_gpst_CurrentWorld->eot.dynamicObjectsTablePF.nextElemPtr))->objectPtr = a1;
        ((tableElemStructPF*)(v6->eot.dynamicObjectsTablePF.nextElemPtr))->objectFlags = v3;
        ++v6->eot.dynamicObjectsTablePF.actElemNum;
        ++((tableElemStructPF*)(v6->eot.dynamicObjectsTablePF.nextElemPtr));
    }
    else
    {
        ENG_gb_MustRebuildDyn = 1;
    }
    v2 = a1->basic;
    if (v2)
        v5 = v2->dyn;
    else
        v5 = 0;
    return v5 != 0;
}

tAINode* AI_EvalFunc_DYNOn(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_DYNOn_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

BOOL AI_EvalFunc_DYNOff_C(struct sGameObjectMain* a1)
{
    tGameObjectBasic* v2;
    int v3;
    tDyn* v4;

    if (a1)
        v3 = a1->flags;
    else
        v3 = 0;
    OBJ_ChangeIdentityFlags(a1, v3 & ~4u, -1);
    if (AI_C_Callback == 1)
        TAB_PFtable_RemoveElemWithPointer(&WOR_gpst_CurrentWorld->eot.dynamicObjectsTablePF, a1);
    else
        ENG_gb_MustRebuildDyn = 1;
    v2 = a1->basic;
    if (v2)
        v4 = v2->dyn;
    else
        v4 = 0;
    return v4 == 0;
}

tAINode* AI_EvalFunc_DYNOff(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_DYNOff_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_DYNFlagsSet_C(struct sGameObjectMain* a1, int a2, int a3)
{
    tGameObjectBasic* v4;
    tGameObjectBasic* v5;
    tDyn* v7;
    int v8;

    if (!a1)
        return -1;
    v5 = a1->basic;
    if (v5)
        v7 = v5->dyn;
    else
        v7 = 0;
    if (!v7)
    {
        OBJ_ChangeIdentityFlags(a1, a1->flags | 4, -1);
        v4 = a1->basic;
        if (v4)
            v7 = v4->dyn;
        else
            v7 = 0;
    }
    v8 = a2 | ~a3 & v7->flags;
    DYN_ChangeDynFlags(v7, v8, v7->flags);
    return v8;
}

tAINode* AI_EvalFunc_DYNFlagsSet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int a3;
    int a2;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_DYNFlagsSet_C(v3, a2, a3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_DYNFlagsGet_C(struct sGameObjectMain* a1)
{
    int result;
    tGameObjectBasic* v2;
    int* v3;

    if (!a1)
        return 0;
    v2 = a1->basic;
    if (v2)
        v3 = &v2->dyn->flags;
    else
        v3 = 0;
    if (v3)
        result = *v3;
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_DYNFlagsGet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_DYNFlagsGet_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

Vec3* AI_EvalFunc_DYNGravitySet_C(struct sGameObjectMain* a1, Vec3* a2)
{
    tGameObjectBasic* v3;
    tDyn* v4;

    v3 = a1->basic;
    if (v3)
        v4 = v3->dyn;
    else
        v4 = 0;
    v4->forces->gravityVector = *a2;
    //v4->forces->gravityVector.z = 0;
    return a2;
}

tAINode* AI_EvalFunc_DYNGravitySet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_GetVecFromStack(&a1a);
    AI_EvalFunc_DYNGravitySet_C(v4, &a1a);
    return a1 + 1;
}

void AI_EvalFunc_DYNTractionSet_C(struct sGameObjectMain* a1, Vec3* a2)
{
    tGameObjectBasic* v2;
    tDyn* v3;

    v2 = a1->basic;
    if (v2)
        v3 = v2->dyn;
    else
        v3 = 0;
    if (v3)
    {
        if (v3->forces)
            v3->forces->tractionVector = *a2;
    }
}

void AI_EvalFunc_DYNTractionSet_C_CURRENT(Vec3* a1)
{
    tGameObjectBasic* v1;
    tDyn* v2;

    v1 = AI_gpst_CurrentGameObject->basic;
    if (v1)
        v2 = v1->dyn;
    else
        v2 = 0;
    if (v2)
    {
        if (v2->forces)
            v2->forces->tractionVector = *a1;
    }
}

tAINode* AI_EvalFunc_DYNTractionSet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a2;
    tGameObjectMain* a1a;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1a = v2;
    AI_GetVecFromStack(&a2);
    AI_EvalFunc_DYNTractionSet_C(a1a, &a2);
    return a1 + 1;
}

Vec3* AI_EvalFunc_DYNStreamSet_C(struct sGameObjectMain* a1, Vec3* a2)
{
    tGameObjectBasic* v3;
    tDyn* v4;

    v3 = a1->basic;
    if (v3)
        v4 = v3->dyn;
    else
        v4 = 0;
    v4->forces->streamVec = *a2;
    return a2;
}

tAINode* AI_EvalFunc_DYNStreamSet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_GetVecFromStack(&a1a);
    AI_EvalFunc_DYNStreamSet_C(v4, &a1a);
    return a1 + 1;
}

int AI_EvalFunc_DYNFrictionSet_C(struct sGameObjectMain* a1, float a2)
{
    tGameObjectBasic* v3;
    tDyn* v4;

    v3 = a1->basic;
    if (v3)
        v4 = v3->dyn;
    else
        v4 = 0;
    v4->forces->frictionVector.x = a2;
    return 1;
}

tAINode* AI_EvalFunc_DYNFrictionSet(tAINode* a1)
{
    int v1;
    float a2;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    v1 = AI_EvalFunc_DYNFrictionSet_C(v4, a2);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

float AI_EvalFunc_DYNFrictionGet_C(struct sGameObjectMain* a1)
{
    tGameObjectBasic* v2;
    tDyn* v3;

    if (!a1)
        return 0.0;
    v2 = a1->basic;
    if (v2)
        v3 = v2->dyn;
    else
        v3 = 0;
    if (!v3)
        return 0.0;
    if ((v3->flags & 4) != 0)
        return v3->forces->frictionVector.x;
    return 0.0;
}

tAINode* AI_EvalFunc_DYNFrictionGet(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_DYNFrictionGet_C(v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_DYNFrictionVectorSet_C(struct sGameObjectMain* a1, Vec3* a2)
{
    tGameObjectBasic* v3;
    tDyn* v4;

    v3 = a1->basic;
    if (v3)
        v4 = v3->dyn;
    else
        v4 = 0;
    if ((v4->flags & 4) == 0)
        return 0;
    v4->forces->frictionVector = *a2;
    return 1;
}

tAINode* AI_EvalFunc_DYNFrictionVectorSet(tAINode* a1)
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
    v1 = AI_EvalFunc_DYNFrictionVectorSet_C(v5, &a1a);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

Vec3* AI_EvalFunc_DYNFrictionVectorGet_C(struct sGameObjectMain* a1, Vec3* a2)
{
    tDynForces* v2;
    tGameObjectBasic* v4;
    tDyn* v5;

    v4 = a1->basic;
    if (v4)
        v5 = v4->dyn;
    else
        v5 = 0;
    v2 = v5->forces;
    *a2 = v2->frictionVector;
    return &v2->frictionVector;
}

tAINode* AI_EvalFunc_DYNFrictionVectorGet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_EvalFunc_DYNFrictionVectorGet_C(v2, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

struct sDynForces* AI_EvalFunc_DYNGravityVectorGet_C(struct sGameObjectMain* a1, Vec3* a2)
{
    tDynForces* v3;
    tGameObjectBasic* v4;
    tDyn* v5;

    v4 = a1->basic;
    if (v4)
        v5 = v4->dyn;
    else
        v5 = 0;
    v3 = v5->forces;
    *a2 = v3->gravityVector;
    return v3;
}

tAINode* AI_EvalFunc_DYNGravityVectorGet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_EvalFunc_DYNGravityVectorGet_C(v2, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

Vec3* AI_EvalFunc_DYNStreamVectorGet_C(struct sGameObjectMain* a1, Vec3* a2)
{
    tDynForces* v2;
    tGameObjectBasic* v4;
    tDyn* v5;

    v4 = a1->basic;
    if (v4)
        v5 = v4->dyn;
    else
        v5 = 0;
    v2 = v5->forces;
    *a2 = v2->streamVec;
    return &v2->streamVec;
}

tAINode* AI_EvalFunc_DYNStreamVectorGet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_EvalFunc_DYNStreamVectorGet_C(v2, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

Vec3* AI_EvalFunc_DYNTractionVectorGet_C(struct sGameObjectMain* a1, Vec3* a2)
{
    tDynForces* v2;
    tGameObjectBasic* v4;
    tDyn* v5;

    v4 = a1->basic;
    if (v4)
        v5 = v4->dyn;
    else
        v5 = 0;
    v2 = v5->forces;
    *a2 = v2->tractionVector;
    return &v2->tractionVector;
}

tAINode* AI_EvalFunc_DYNTractionVectorGet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_EvalFunc_DYNTractionVectorGet_C(v2, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

tAINode* AI_EvalFunc_DYNFrictionSetForMaxSpeed_(tAINode* a1)
{
    float v2;
    tGameObjectBasic* v3;
    tGameObjectMain* v4;
    float a2;
    tDyn* a1a;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    if (v4 && ((v3 = v4->basic) == 0 ? (a1a = 0) : (a1a = v3->dyn), a1a && (a1a->flags & 4) != 0))
    {
        v2 = DYN_f_FindFrictionForMaxSpeed(a1a, a2);
        a1a->forces->frictionVector.x = v2;
        AI_PutIntToStack(1);
    }
    else
    {
        AI_PutIntToStack(0);
    }
    return a1 + 1;
}

float AI_EvalFunc_DYNSpeedGet_C(struct sGameObjectMain* a1)
{
    Vec3 a2;
    tGameObjectBasic* v3;
    tDyn* a1a;

    v3 = a1->basic;
    if (v3)
        a1a = v3->dyn;
    else
        a1a = 0;
    if (!a1a)
        return 0.0;
    DYN_SpeedGetVector(a1a, &a2);
    return MATH_VecLen(&a2);
}

tAINode* AI_EvalFunc_DYNSpeedGet(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_DYNSpeedGet_C(v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

void AI_EvalFunc_DYNSpeedSet_C(struct sGameObjectMain* a1, float a2)
{
    tGameObjectBasic* v2;

    v2 = a1->basic;
    if (v2)
        DYN_ApplyHorizontalSpeed(v2->dyn, a2);
    else
        DYN_ApplyHorizontalSpeed(0, a2);
}

tAINode* AI_EvalFunc_DYNSpeedSet(tAINode* a1)
{
    tGameObjectMain* v2;
    float a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_DYNSpeedSet_C(v2, a2);
    return a1 + 1;
}

void AI_EvalFunc_DYNElasticSet_C(tGameObjectMain* a1, float a2)
{
    tGameObjectBasic* v2;
    tDyn* v3;

    v2 = a1->basic;
    if (v2)
        v3 = v2->dyn;
    else
        v3 = 0;
    v3->solid->elastic = a2;
}

tAINode* AI_EvalFunc_DYNElasticSet(tAINode* a1)
{
    tGameObjectMain* v2;
    float a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_DYNElasticSet_C(v2, a2);
    return a1 + 1;
}

float AI_EvalFunc_DYNSpeedLimitGet_C(struct sGameObjectMain* a1)
{
    tGameObjectBasic* v2;
    tDyn* v3;

    v2 = a1->basic;
    if (v2)
        v3 = v2->dyn;
    else
        v3 = 0;
    return v3->speedLimit;
}

tAINode* AI_EvalFunc_DYNSpeedLimitGet(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_DYNSpeedLimitGet_C(v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_DYNSpeedLimitHorizGet(tAINode* a1)
{
    tGameObjectBasic* v2;
    tGameObjectMain* v3;
    tDyn* v4;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = v3->basic;
    if (v2)
        v4 = v2->dyn;
    else
        v4 = 0;
    AI_PutFloatToStack(v4->speedLimit);
    return a1 + 1;
}

tAINode* AI_EvalFunc_DYNSpeedLimitVertGet(tAINode* a1)
{
    tGameObjectBasic* v2;
    tGameObjectMain* v3;
    tDyn* v4;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = v3->basic;
    if (v2)
        v4 = v2->dyn;
    else
        v4 = 0;
    AI_PutFloatToStack(v4->speedLimitVertical);
    return a1 + 1;
}

struct sDyn* AI_EvalFunc_DYNSpeedLimitSet_C(struct sGameObjectMain* a1, float a2)
{
    tDyn* result;
    tGameObjectBasic* v3;
    tDyn* v4;

    v3 = a1->basic;
    if (v3)
        v4 = v3->dyn;
    else
        v4 = 0;
    v4->speedLimit = a2;
    result = v4;
    v4->speedLimitVertical = a2;
    return result;
}

tAINode* AI_EvalFunc_DYNSpeedLimitSet(tAINode* a1)
{
    tGameObjectMain* v2;
    float a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_DYNSpeedLimitSet_C(v2, a2);
    return a1 + 1;
}

void AI_EvalFunc_DYNSpeedLimitHorizSet_C(tGameObjectMain* a1, float a2)
{
    tGameObjectBasic* v2;
    tDyn* v3;

    v2 = a1->basic;
    if (v2)
        v3 = v2->dyn;
    else
        v3 = 0;
    v3->speedLimit = a2;
}

tAINode* AI_EvalFunc_DYNSpeedLimitHorizSet(tAINode* a1)
{
    tGameObjectMain* v2;
    float a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_DYNSpeedLimitHorizSet_C(v2, a2);
    return a1 + 1;
}

void AI_EvalFunc_DYNSpeedLimitVertSet_C(tGameObjectMain* a1, float a2)
{
    tGameObjectBasic* v2;
    tDyn* v3;

    v2 = a1->basic;
    if (v2)
        v3 = v2->dyn;
    else
        v3 = 0;
    v3->speedLimitVertical = a2;
}

tAINode* AI_EvalFunc_DYNSpeedLimitVertSet(tAINode* a1)
{
    tGameObjectMain* v2;
    float a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_DYNSpeedLimitVertSet_C(v2, a2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_DYNAutoOrientInertiaSet(tAINode* a1)
{
    tGameObjectBasic* v2;
    tGameObjectMain* v3;
    tDyn* v4;
    int16_t v5;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = AI_GetIntFromStack();
    if (v3)
    {
        v2 = v3->basic;
        if (v2)
            v4 = v2->dyn;
        else
            v4 = 0;
        if (v4)
        {
            v4->flags &= ~0xC00u;
            v4->flags += v5 & 0xC00;
        }
    }
    return a1 + 1;
}

tAINode* AI_EvalFunc_DYNAutoOrientInertiaGet(tAINode* a1)
{
    tGameObjectBasic* v2;
    tGameObjectMain* v3;
    tDyn* v4;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    if (v3 && ((v2 = v3->basic) == 0 ? (v4 = 0) : (v4 = v2->dyn), v4))
        AI_PutIntToStack(v4->flags & 0xC00);
    else
        AI_PutIntToStack(0);
    return a1 + 1;
}

void AI_EvalFunc_DYNConstraintObjectToSegment_C(tGameObjectMain* a1, Vec3* a2, Vec3* a4)
{
    tGameObjectBasic* v3;
    tDyn* v4;

    v3 = a1->basic;
    if (v3)
        v4 = v3->dyn;
    else
        v4 = 0;
    DYN_ConstraintInit(v4->constraint, 1, a2, a4, &MATH_gst_NulVector, 0.0);
}

tAINode* AI_EvalFunc_DYNConstraintObjectToSegment(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a2;
    Vec3 a1a;
    tGameObjectMain* v5;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v2;
    AI_GetVecFromStack(&a1a);
    AI_GetVecFromStack(&a2);
    AI_EvalFunc_DYNConstraintObjectToSegment_C(v5, &a2, &a1a);
    return a1 + 1;
}

void AI_EvalFunc_DYNConstraintObjectToParabol_C(tGameObjectMain* a1, Vec3* a2, Vec3* a3, Vec3* a4, float a5)
{
    tGameObjectBasic* v5;
    tDyn* v6;

    v5 = a1->basic;
    if (v5)
        v6 = v5->dyn;
    else
        v6 = 0;
    DYN_ConstraintInit(v6->constraint, 2, a2, a3, a4, a5);
}

tAINode* AI_EvalFunc_DYNConstraintObjectToParabol(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a2;
    float a5;
    Vec3 a1a;
    Vec3 a3;
    tGameObjectMain* v7;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v7 = v2;
    a5 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&a1a);
    AI_GetVecFromStack(&a3);
    AI_GetVecFromStack(&a2);
    AI_EvalFunc_DYNConstraintObjectToParabol_C(v7, &a2, &a3, &a1a, a5);
    return a1 + 1;
}

void AI_EvalFunc_DYNImpulsion_C(struct sGameObjectMain* a2, Vec3* a3, float a4)
{
    Vec3* v4;
    Vec3 v5;
    Vec3 v6;
    Mtx4x4 v7;
    Vec3 v8;
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
    Vec3 v20;
    Vec3 v21;
    Vec3* v22;
    Vec3 v23;
    Vec3 v24;
    Vec3* v25;
    float v26;
    Vec3* v27;
    Vec3 v28;
    Vec3* v29;
    Vec3 v30;
    tDynForces* v31;
    tDyn* v32;
    tGameObjectBasic* v33;

    v33 = a2->basic;
    if (v33)
        v32 = v33->dyn;
    else
        v32 = 0;
    MATH_VecSet(&v32->forces->tractionVector, &MATH_gst_NulVector);
    MATH_VecSet(&v32->forces->frictionVector, &MATH_gst_NulVector);
    v32->speedLimit = 3.402822e38f;
    v32->speedLimitVertical = 3.402822e38f;
    v31 = v32->forces;
    MATH_NormalizeVector(&v30, &v31->gravityVector);
    v29 = &v30;
    v27 = &v28;
    v28.x = -v30.x;
    v28.y = -v30.y;
    v28.z = -v30.z;
    v26 = MATH_VecLen(&v31->gravityVector);
    v25 = v32->vptr;
    MATH_SubstractVec(&v24, a3, v25);
    if (a4 < 0.0)
        a4 = 0.0;
    v22 = &v23;
    v23.x = v28.x * a4;
    v23.y = v28.y * a4;
    v23.z = v28.z * a4;
    if (v24.z * v28.z + v24.y * v28.y + v28.x * v24.x <= 0.0)
    {
        v23.x = v23.x + v25->x;
        v23.y = v23.y + v25->y;
        v4 = v25;
    }
    else
    {
        v23.x = v23.x + a3->x;
        v23.y = v23.y + a3->y;
        v4 = a3;
    }
    v23.z = v23.z + v4->z;
    MATH_SubstractVec(&v21, a3, &v23);
    MATH_SubstractVec(&v20, v25, &v23);
    v19 = v30.z * v20.z + v30.y * v20.y + v20.x * v30.x;
    v18 = v30.z * v21.z + v30.y * v21.y + v21.x * v30.x;
    v17 = (float)sqrt(v19);
    v16 = v17;
    v15 = (float)sqrt(v18);
    v14 = v15;
    v13 = (float)sqrt(2.0 / v26);
    v12 = v13 * (v17 + v15);
    v11 = (float)sqrt(2.0 * v26 * v19);
    v10 = v11;
    v9 = (float)(1.0 / v12);
    if (MATH_b_EqVectorWithEpsilon(&v28, &MATH_gst_BaseVectorK, 0.000099999997f))
    {
        v8.z = v10;
        v8.x = (a3->x - v25->x) * v9;
        v8.y = (a3->y - v25->y) * v9;
    }
    else
    {
        MATH_MakeRotationMatrix_UsingBanking(&v7, &v24, &v28, 1);
        MATH_VectorGlobalToLocal(&v6, &v7, &v24);
        v5.z = v10;
        v5.x = v6.x * v9;
        v5.y = v6.y * v9;
        MATH_TransformVectorNoScale(&v8, &v7, &v5);
    }
    DYN_SetSpeedVector(v32, &v8);
}

tAINode* AI_EvalFunc_DYNImpulsion(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a3;
    float a4;
    tGameObjectMain* a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = v2;
    a4 = AI_GetFloatFromStack();
    AI_GetVecFromStack(&a3);
    AI_EvalFunc_DYNImpulsion_C(a2, &a3, a4);
    return a1 + 1;
}

tAINode* AI_EvalFunc_DYNTorqueVectorSet(tAINode* a1)
{
    Vec3* v2;
    tGameObjectBasic* v3;
    tGameObjectMain* v4;
    tDynSolid* v5;
    Vec3 a1a;
    tDyn* v7;
    tGameObjectMain* v8;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v8 = v4;
    AI_GetVecFromStack(&a1a);
    v3 = v8->basic;
    if (v3)
        v7 = v3->dyn;
    else
        v7 = 0;
    v5 = v7->solid;
    if (v5)
        v2 = &v5->torqueVector;
    else
        v2 = 0;
    *v2 = a1a;
    return a1 + 1;
}

tAINode* AI_EvalFunc_DYNTorqueVectorGet(tAINode* a1)
{
    tGameObjectBasic* v2;
    tGameObjectMain* v3;
    tDynSolid* v4;
    tDyn* v5;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = v3->basic;
    if (v2)
        v5 = v2->dyn;
    else
        v5 = 0;
    v4 = v5->solid;
    if (v4)
        AI_PutVecToStack(&v4->torqueVector);
    else
        AI_PutVecToStack(0);
    return a1 + 1;
}

tAINode* AI_EvalFunc_DYNRotSpeedVectorSet(tAINode* a1)
{
    Vec3* v2;
    tGameObjectBasic* v3;
    tGameObjectMain* v4;
    tDyn* v5;
    Vec3 a1a;
    tGameObjectMain* v7;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v7 = v4;
    AI_GetVecFromStack(&a1a);
    v3 = v7->basic;
    if (v3)
        v5 = v3->dyn;
    else
        v5 = 0;
    if (v5->solid)
        v2 = &v5->solid->rotSpeedVector;
    else
        v2 = 0;
    *v2 = a1a;
    return a1 + 1;
}

tAINode* AI_EvalFunc_DYNRotSpeedVectorGet(tAINode* a1)
{
    tGameObjectBasic* v2;
    tGameObjectMain* v3;
    Vec3* v4;
    tDyn* v5;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = v3->basic;
    if (v2)
        v5 = v2->dyn;
    else
        v5 = 0;
    v4 = &v5->solid->rotSpeedVector;
    if (v4)
        AI_PutVecToStack(v4);
    else
        AI_PutVecToStack(0);
    return a1 + 1;
}

tAINode* AI_EvalFunc_DYNRotSpeedLimitGet(tAINode* a1)
{
    tGameObjectBasic* v2;
    tGameObjectMain* v3;
    tDynSolid* v4;
    tDyn* v5;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = v3->basic;
    if (v2)
        v5 = v2->dyn;
    else
        v5 = 0;
    v4 = v5->solid;
    if (v4)
        AI_PutFloatToStack(v4->rotSpeedLimit);
    else
        AI_PutFloatToStack(0.0);
    return a1 + 1;
}

tAINode* AI_EvalFunc_DYNRotSpeedLimitSet(tAINode* a1)
{
    tGameObjectBasic* v2;
    tGameObjectMain* v3;
    tDynSolid* v4;
    tDyn* v5;
    float v6;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v6 = AI_GetFloatFromStack();
    v2 = v3->basic;
    if (v2)
        v5 = v2->dyn;
    else
        v5 = 0;
    v4 = v5->solid;
    if (v4)
        v4->rotSpeedLimit = v6;
    return a1 + 1;
}

BOOL AI_EvalFunc_DYNSlideSet_C(tGameObjectMain* a1, float a2)
{
    BOOL result;
    tGameObjectBasic* v3;
    tDyn* v4;
    tDynCol* v5;

    v3 = a1->basic;
    if (v3)
        v4 = v3->dyn;
    else
        v4 = 0;
    v5 = v4->collision;
    result = v5 != 0;
    if (v5)
        v5->slide = a2;
    return result;
}

tAINode* AI_EvalFunc_DYNSlideSet(tAINode* a1)
{
    float a2;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_DYNSlideSet_C(v3, a2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_DYNSlideGet(tAINode* a1)
{
    tGameObjectBasic* v2;
    tGameObjectMain* v3;
    tDyn* v4;
    tDynCol* v5;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = v3->basic;
    if (v2)
        v4 = v2->dyn;
    else
        v4 = 0;
    v5 = v4->collision;
    if (v5)
        AI_PutFloatToStack(v5->slide);
    else
        AI_PutFloatToStack(1.0);
    return a1 + 1;
}

BOOL AI_EvalFunc_DYNReboundSet_C(struct sGameObjectMain* a1, float a2)
{
    BOOL result;
    tGameObjectBasic* v3;
    tDyn* v4;
    tDynCol* v5;

    v3 = a1->basic;
    if (v3)
        v4 = v3->dyn;
    else
        v4 = 0;
    v5 = v4->collision;
    result = v5 != 0;
    if (v5)
        v5->rebound = a2;
    return result;
}

tAINode* AI_EvalFunc_DYNReboundSet(tAINode* a1)
{
    tGameObjectMain* v2;
    float a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_DYNReboundSet_C(v2, a2);
    return a1 + 1;
}

struct sDynCol* AI_EvalFunc_DYNApplyRectFactorSet_C(struct sGameObjectMain* a1, float a2)
{
    tDynCol* result;
    tGameObjectBasic* v3;
    tDyn* v4;

    v3 = a1->basic;
    if (v3)
        v4 = v3->dyn;
    else
        v4 = 0;
    result = v4->collision;
    result->rectFactor = a2;
    return result;
}

tAINode* AI_EvalFunc_DYNApplyRectFactorSet(tAINode* a1)
{
    tGameObjectMain* v2;
    float a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_DYNApplyRectFactorSet_C(v2, a2);
    return a1 + 1;
}

float AI_EvalFunc_DYNReboundGet_C(tGameObjectMain* a1)
{
    float v2;
    tGameObjectBasic* v3;
    tDyn* v4;
    float* v5;

    v3 = a1->basic;
    if (v3)
        v4 = v3->dyn;
    else
        v4 = 0;
    v5 = &v4->collision->rebound;
    if (v5)
        v2 = *v5;
    else
        v2 = 0.0;
    return v2;
}

tAINode* AI_EvalFunc_DYNReboundGet(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_DYNReboundGet_C(v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_DYNReboundHorizCosAngleSet(tAINode* a1)
{
    tGameObjectBasic* v2;
    tGameObjectMain* v3;
    tDyn* v4;
    tDynCol* v5;
    float v6;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v6 = AI_GetFloatFromStack();
    v2 = v3->basic;
    if (v2)
        v4 = v2->dyn;
    else
        v4 = 0;
    v5 = v4->collision;
    if (v5)
        v5->reboundHorizCosAngle = v6;
    return a1 + 1;
}

tAINode* AI_EvalFunc_DYNReboundHorizCosAngleGet(tAINode* a1)
{
    tGameObjectBasic* v2;
    tGameObjectMain* v3;
    tDyn* v4;
    tDynCol* v5;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = v3->basic;
    if (v2)
        v4 = v2->dyn;
    else
        v4 = 0;
    v5 = v4->collision;
    if (v5)
        AI_PutFloatToStack(v5->reboundHorizCosAngle);
    else
        AI_PutFloatToStack(0.0);
    return a1 + 1;
}

BOOL AI_EvalFunc_DYNSlideHorizCosAngleSet_C(struct sGameObjectMain* a1, float a2)
{
    BOOL result;
    tGameObjectBasic* v3;
    tDyn* v4;
    tDynCol* v5;

    v3 = a1->basic;
    if (v3)
        v4 = v3->dyn;
    else
        v4 = 0;
    v5 = v4->collision;
    result = v5 != 0;
    if (v5)
        v5->slideHorizCosAngle = a2;
    return result;
}

tAINode* AI_EvalFunc_DYNSlideHorizCosAngleSet(tAINode* a1)
{
    tGameObjectMain* v2;
    float a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_DYNSlideHorizCosAngleSet_C(v2, a2);
    return a1 + 1;
}

float AI_EvalFunc_DYNSlideHorizCosAngleGet_C(struct sGameObjectMain* a1)
{
    float v2;
    tGameObjectBasic* v3;
    tDyn* v4;
    tDynCol* v5;

    v3 = a1->basic;
    if (v3)
        v4 = v3->dyn;
    else
        v4 = 0;
    v5 = v4->collision;
    if (v5)
        v2 = v5->slideHorizCosAngle;
    else
        v2 = 0.0;
    return v2;
}

tAINode* AI_EvalFunc_DYNSlideHorizCosAngleGet(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_DYNSlideHorizCosAngleGet_C(v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_DYNSpeedVectorHorizSet(tAINode* a1)
{
    tGameObjectBasic* v2;
    tGameObjectMain* v3;
    Vec3 a1a;
    tDyn* v5;
    tGameObjectMain* v6;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v6 = v3;
    v2 = v3->basic;
    if (v2)
        v5 = v2->dyn;
    else
        v5 = 0;
    AI_GetVecFromStack(&a1a);
    DYN_SetSpeedVector_WithoutChangingGravity(v5, &a1a);
    return a1 + 1;
}

Vec3* AI_EvalFunc_DYNMaxPosSet_C(struct sGameObjectMain* a1, Vec3* a2)
{
    tGameObjectBasic* v3;
    tDyn* v4;

    v3 = a1->basic;
    if (v3)
        v4 = v3->dyn;
    else
        v4 = 0;
    v4->maxPos = *a2;
    return &v4->maxPos;
}

tAINode* AI_EvalFunc_DYNMaxPosSet(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_GetVecFromStack(&a1a);
    AI_EvalFunc_DYNMaxPosSet_C(v4, &a1a);
    return a1 + 1;
}

Vec3* AI_EvalFunc_DYNConstraintGetTangent_C(tGameObjectMain* a1, Vec3* a2)
{
    tDynConstraint* v2;
    tGameObjectBasic* v4;
    tDyn* v5;

    v4 = a1->basic;
    if (v4)
        v5 = v4->dyn;
    else
        v5 = 0;
    v2 = v5->constraint;
    *a2 = v2->tangentVec;
    return &v2->tangentVec;
}

tAINode* AI_EvalFunc_DYNConstraintGetTangent(tAINode* a1)
{
    tGameObjectMain* v2;
    Vec3 a1a;
    tGameObjectMain* v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v2;
    AI_EvalFunc_DYNConstraintGetTangent_C(v2, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

void AI_EvalFunc_DYNAlignSight_C(tGameObjectMain* a1)
{
    tGameObjectBasic* v1;
    Vec3 a2;
    tDyn* a1a;
    float v4;

    v1 = a1->basic;
    if (v1)
        a1a = v1->dyn;
    else
        a1a = 0;
    DYN_SpeedGetVector(a1a, &a2);
    v4 = MATH_VecLen(&a2);
    OBJ_SightGet(a1, &a2);
    MATH_NormalizeVector(&a2, &a2);
    a2.x = a2.x * v4;
    a2.y = a2.y * v4;
    a2.z = a2.z * v4;
    DYN_SetSpeedVector(a1a, &a2);
}

tAINode* AI_EvalFunc_DYNAlignSight(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_DYNAlignSight_C(v2);
    return a1 + 1;
}