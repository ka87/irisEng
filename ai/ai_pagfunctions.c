#include "../particlegenerator.h"
#include "../graphicsobjects/particle.h"
#include "ai_pagfunctions.h"

int AI_EvalFunc_PAG_AddGenerator_C(tGameObjectMain* gao, tGameObjectMain* emitter, float frequency)
{
    tPAGStruct* pag;

    pag = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (!pag)
        return 0;
    return PAG_i_AddGenerator(pag, emitter, frequency);
}

tAINode* AI_EvalFunc_PAG_AddGenerator(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* emitter;
    float frequency;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    frequency = AI_GetFloatFromStack();
    emitter = (tGameObjectMain*)AI_GetObjectFromStack();
    AI_PutIntToStack(AI_EvalFunc_PAG_AddGenerator_C(gaoUltra, emitter, frequency));
    return node + 1;
}

void AI_EvalFunc_PAG_DelGenerator_C(tGameObjectMain* gao, int generatorID)
{
    tPAGStruct* pag;

    pag = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (pag)
        PAG_DelGenerator(pag, generatorID);
}

tAINode* AI_EvalFunc_PAG_DelGenerator(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int generatorID;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    generatorID = AI_GetIntFromStack();
    AI_EvalFunc_PAG_DelGenerator_C(gaoUltra, generatorID);
    return node + 1;
}

void AI_EvalFunc_PAG_SetGeneratorFreq_C(struct sGameObjectMain* gao, int generatorID, float frequency)
{
    tPAGStruct* pag;

    pag = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (pag)
        PAG_SetGeneratorFrequency(pag, generatorID, frequency);
}

tAINode* AI_EvalFunc_PAG_SetGeneratorFreq(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int generatorID;
    float frequency;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    frequency = AI_GetFloatFromStack();
    generatorID = AI_GetIntFromStack();
    AI_EvalFunc_PAG_SetGeneratorFreq_C(gaoUltra, generatorID, frequency);
    return node + 1;
}

void AI_EvalFunc_PAG_SetFlag_C(struct sGameObjectMain* gao, int toSet, int toClear)
{
    tPAGStruct* pag;

    pag = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (pag)
    {
        pag->flags |= toSet;
        pag->flags &= ~toClear;
    }
}

tAINode* AI_EvalFunc_PAG_SetFlag(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int toSet;
    int toClear;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    toClear = AI_GetIntFromStack();
    toSet = AI_GetIntFromStack();
    AI_EvalFunc_PAG_SetFlag_C(gaoUltra, toSet, toClear);
    return node + 1;
}

uint32_t AI_EvalFunc_PAG_GetFlag_C(tGameObjectMain* gao)
{
    tPAGStruct* pag;

    pag = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (!pag)
        return 0;
    return pag->flags;
}

tAINode* AI_EvalFunc_PAG_GetFlag(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    AI_PutIntToStack(AI_EvalFunc_PAG_GetFlag_C(gaoUltra));
    return node + 1;
}

void AI_EvalFunc_PAG_Setf_C(struct sGameObjectMain* gao, int floatIdx, float value)
{
    tPAGStruct* pag;

    if (floatIdx < 0 || floatIdx >= 35)
        return;

    pag = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (pag)
        *((float*)&pag->ffb1 + floatIdx) = value;
}

tAINode* AI_EvalFunc_PAG_Setf(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int floatIdx;
    float value;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    value = AI_GetFloatFromStack();
    floatIdx = AI_GetIntFromStack();
    AI_EvalFunc_PAG_Setf_C(gaoUltra, floatIdx, value);
    return node + 1;
}

float AI_EvalFunc_PAG_Getf_C(struct sGameObjectMain* gao, int floatIdx)
{
    tPAGStruct* pag;

    if (floatIdx < 0 || floatIdx >= 35)
        return 0.0f;

    pag = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (!pag)
        return 0.0f;
    return *((float*)&pag->ffb1 + floatIdx);
}

tAINode* AI_EvalFunc_PAG_Getf(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int floatIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    floatIdx = AI_GetIntFromStack();
    AI_PutFloatToStack(AI_EvalFunc_PAG_Getf_C(gaoUltra, floatIdx));
    return node + 1;
}

void AI_EvalFunc_PAG_SetColor_C(struct sGameObjectMain* gao, int color)
{
    tPAGStruct* pag;

    pag = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (pag)
        pag->color = color;
}

tAINode* AI_EvalFunc_PAG_SetColor(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int color;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    color = AI_GetIntFromStack();
    AI_EvalFunc_PAG_SetColor_C(gaoUltra, color);
    return node + 1;
}

void AI_EvalFunc_PAG_SetObject_C(struct sGameObjectMain* gao, struct sGameObjectMain* emitter)
{
    tPAGStruct* pag;

    pag = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (pag)
        pag->gao1 = emitter;
}

tAINode* AI_EvalFunc_PAG_SetObject(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    AI_EvalFunc_PAG_SetObject_C(gaoUltra, (tGameObjectMain*)AI_GetObjectFromStack());
    return node + 1;
}

void AI_EvalFunc_PAG_SetMatrixRef_C(struct sGameObjectMain* gao, struct sGameObjectMain* reference)
{
    tPAGStruct* pag;

    pag = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (pag)
        pag->gaoForMatrixRef = reference;
}

tAINode* AI_EvalFunc_PAG_SetMatrixRef(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    AI_EvalFunc_PAG_SetMatrixRef_C(gaoUltra, (tGameObjectMain*)AI_GetObjectFromStack());
    return node + 1;
}

void AI_EvalFunc_PAG_ShiftParticles_C(struct sGameObjectMain* gao, Vec3* shift)
{
    tPAGStruct* pag;
    Vec3* pos;
    int i;

    pag = PAG_pst_GetGameObjectParticleGenerator(gao);
    if (!pag)
        return;

    for (i = 0; i < pag->triangleNum2; ++i)
    {
        pos = &pag->pagElems[i].particlePos;
        pos->x = pos->x + shift->x;
        pos->y = pos->y + shift->y;
        pos->z = pos->z + shift->z;
    }
}

tAINode* AI_EvalFunc_PAG_ShiftParticles(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    // the vector is read where the stack keeps it rather than copied out
    AI_EvalFunc_PAG_ShiftParticles_C(gaoUltra, (Vec3*)AI_PopStackSlot());
    return node + 1;
}
