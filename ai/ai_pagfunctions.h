#pragma once

#include "../ai.h"

void AI_EvalFunc_PAG_SetGeneratorFreq_C(struct sGameObjectMain* a1, int a2, float a3);
void AI_EvalFunc_PAG_SetFlag_C(struct sGameObjectMain* a1, int a2, int a3);
void AI_EvalFunc_PAG_SetColor_C(struct sGameObjectMain* a1, int color);
void AI_EvalFunc_PAG_Setf_C(struct sGameObjectMain* a1, int a2, float a3);
void AI_EvalFunc_PAG_SetMatrixRef_C(struct sGameObjectMain* a1, struct sGameObjectMain* a2);
void AI_EvalFunc_PAG_SetObject_C(struct sGameObjectMain* a1, struct sGameObjectMain* a2);
void AI_EvalFunc_PAG_ShiftParticles_C(struct sGameObjectMain* a1, Vec3* a2);
float AI_EvalFunc_PAG_Getf_C(struct sGameObjectMain* gao, int floatIdx);

tAINode* AI_EvalFunc_PAG_AddGenerator(tAINode* a1);
tAINode* AI_EvalFunc_PAG_DelGenerator(tAINode* a1);
tAINode* AI_EvalFunc_PAG_SetGeneratorFreq(tAINode* a1);
tAINode* AI_EvalFunc_PAG_SetFlag(tAINode* a1);
tAINode* AI_EvalFunc_PAG_GetFlag(tAINode* a1);
tAINode* AI_EvalFunc_PAG_Setf(tAINode* a1);
tAINode* AI_EvalFunc_PAG_Getf(tAINode* a1);
tAINode* AI_EvalFunc_PAG_SetColor(tAINode* a1);
tAINode* AI_EvalFunc_PAG_SetObject(tAINode* a1);
tAINode* AI_EvalFunc_PAG_SetMatrixRef(tAINode* a1);
tAINode* AI_EvalFunc_PAG_ShiftParticles(tAINode* a1);
