#pragma once

#include "../ai.h"

void AI_EvalFunc_PAG_SetGeneratorFreq_C(struct sGameObjectMain* gao, int a2, float a3);
void AI_EvalFunc_PAG_SetFlag_C(struct sGameObjectMain* gao, int a2, int a3);
void AI_EvalFunc_PAG_SetColor_C(struct sGameObjectMain* gao, int color);
void AI_EvalFunc_PAG_Setf_C(struct sGameObjectMain* gao, int a2, float a3);
void AI_EvalFunc_PAG_SetMatrixRef_C(struct sGameObjectMain* gao, struct sGameObjectMain* a2);
void AI_EvalFunc_PAG_SetObject_C(struct sGameObjectMain* gao, struct sGameObjectMain* a2);
void AI_EvalFunc_PAG_ShiftParticles_C(struct sGameObjectMain* gao, Vec3* a2);
float AI_EvalFunc_PAG_Getf_C(struct sGameObjectMain* gao, int floatIdx);

tAINode* AI_EvalFunc_PAG_AddGenerator(tAINode* node);
tAINode* AI_EvalFunc_PAG_DelGenerator(tAINode* node);
tAINode* AI_EvalFunc_PAG_SetGeneratorFreq(tAINode* node);
tAINode* AI_EvalFunc_PAG_SetFlag(tAINode* node);
tAINode* AI_EvalFunc_PAG_GetFlag(tAINode* node);
tAINode* AI_EvalFunc_PAG_Setf(tAINode* node);
tAINode* AI_EvalFunc_PAG_Getf(tAINode* node);
tAINode* AI_EvalFunc_PAG_SetColor(tAINode* node);
tAINode* AI_EvalFunc_PAG_SetObject(tAINode* node);
tAINode* AI_EvalFunc_PAG_SetMatrixRef(tAINode* node);
tAINode* AI_EvalFunc_PAG_ShiftParticles(tAINode* node);
