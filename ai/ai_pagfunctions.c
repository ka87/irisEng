#include "../particlegenerator.h"
#include "../graphicsobjects/particle.h"
#include "ai_pagfunctions.h"

int AI_EvalFunc_PAG_AddGenerator_C(tGameObjectMain* gao, tGameObjectMain* a2, float a3)
{
    int result;
    tPAGStruct* v4;

    v4 = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (v4)
        result = PAG_i_AddGenerator(v4, a2, a3);
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_PAG_AddGenerator(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    float a3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetFloatFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v1 = AI_EvalFunc_PAG_AddGenerator_C(v3, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a3);
    AI_PutIntToStack(v1);
    return node + 1;
}

void AI_EvalFunc_PAG_DelGenerator_C(tGameObjectMain* gao, int a2)
{
    tPAGStruct* v2;

    v2 = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (v2)
        PAG_DelGenerator(v2, a2);
}

tAINode* AI_EvalFunc_PAG_DelGenerator(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_PAG_DelGenerator_C(v3, v1);
    return node + 1;
}

void AI_EvalFunc_PAG_SetGeneratorFreq_C(struct sGameObjectMain* gao, int a2, float a3)
{
    tPAGStruct* v3;

    v3 = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (v3)
        PAG_SetGeneratorFrequency(v3, a2, a3);
}

tAINode* AI_EvalFunc_PAG_SetGeneratorFreq(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    float a3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetFloatFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_PAG_SetGeneratorFreq_C(v3, v1, a3);
    return node + 1;
}

void AI_EvalFunc_PAG_SetFlag_C(struct sGameObjectMain* gao, int a2, int a3)
{
    tPAGStruct* v3;

    v3 = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (v3)
    {
        v3->flags |= a2;
        v3->flags &= ~a3;
    }
}

tAINode* AI_EvalFunc_PAG_SetFlag(tAINode* node)
{
    int v1;
    int v3;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_PAG_SetFlag_C(v4, v1, v3);
    return node + 1;
}

uint32_t AI_EvalFunc_PAG_GetFlag_C(tGameObjectMain* gao)
{
    int result;
    tPAGStruct* v2;

    v2 = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (v2)
        result = v2->flags;
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_PAG_GetFlag(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_PAG_GetFlag_C(v3);
    AI_PutIntToStack(v1);
    return node + 1;
}

void AI_EvalFunc_PAG_Setf_C(struct sGameObjectMain* gao, int a2, float a3)
{
    tPAGStruct* v3;

    if (a2 >= 0 && a2 < 35)
    {
        v3 = PAG_pst_GetGameObjectParticleGenerator(gao);
        if (v3)
            *((float*)&v3->ffb1 + a2) = a3;
    }
}

tAINode* AI_EvalFunc_PAG_Setf(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;
    float a3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetFloatFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_PAG_Setf_C(v3, v1, a3);
    return node + 1;
}

float AI_EvalFunc_PAG_Getf_C(struct sGameObjectMain* gao, int floatIdx)
{
    float result;
    tPAGStruct* v3;

    if (floatIdx < 0 || floatIdx >= 35)
        return 0.0f;
    v3 = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (v3)
        result = *((float*)&v3->ffb1 + floatIdx);
    else
        result = 0.0f;
    return result;
}

tAINode* AI_EvalFunc_PAG_Getf(tAINode* node)
{
    int v1;
    float v3;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    v3 = AI_EvalFunc_PAG_Getf_C(v4, v1);
    AI_PutFloatToStack(v3);
    return node + 1;
}

void AI_EvalFunc_PAG_SetColor_C(struct sGameObjectMain* gao, int color)
{
    tPAGStruct* v2;

    v2 = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (v2)
        v2->color = color;
}

tAINode* AI_EvalFunc_PAG_SetColor(tAINode* node)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_PAG_SetColor_C(v3, v1);
    return node + 1;
}

void AI_EvalFunc_PAG_SetObject_C(struct sGameObjectMain* gao, struct sGameObjectMain* a2)
{
    tPAGStruct* v2;

    v2 = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (v2)
        v2->gao1 = a2;
}

tAINode* AI_EvalFunc_PAG_SetObject(tAINode* node)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_PAG_SetObject_C(v2, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    return node + 1;
}

void AI_EvalFunc_PAG_SetMatrixRef_C(struct sGameObjectMain* gao, struct sGameObjectMain* a2)
{
    tPAGStruct* v2;

    v2 = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (v2)
        v2->gaoForMatrixRef = a2;
}

tAINode* AI_EvalFunc_PAG_SetMatrixRef(tAINode* node)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_PAG_SetMatrixRef_C(v2, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    return node + 1;
}

void AI_EvalFunc_PAG_ShiftParticles_C(struct sGameObjectMain* gao, Vec3* a2)
{
    Vec3* v2;
    tPAGStruct* v3;
    int i;

    v3 = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (v3)
    {
        for (i = 0; i < v3->triangleNum2; ++i)
        {
            v2 = &v3->pagElems[i].particlePos;
            v2->x = v2->x + a2->x;
            v2->y = v2->y + a2->y;
            v2->z = v2->z + a2->z;
        }
    }
}

tAINode* AI_EvalFunc_PAG_ShiftParticles(tAINode* node)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_PAG_ShiftParticles_C(v2, (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    return node + 1;
}