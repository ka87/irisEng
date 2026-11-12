#pragma once

#include "../common.h"
#include "../ai.h"

void TIME_Wait_C(float a1);
BOOL AI_EvalFunc_TIMEElapsed_C(float a1, float a2);
void AI_EvalFunc_TIMEFactorSet_C(float a1);
void AI_EvalFunc_TIMELock_C();
void AI_EvalFunc_TIMEUnlock_C();
float AI_EvalFunc_TIMEGet_C();

tAINode* AI_EvalFunc_TIMEWait(tAINode* node);
tAINode* AI_EvalFunc_TIMEGet(tAINode* node);
tAINode* AI_EvalFunc_TIMEElapsed(tAINode* node);
tAINode* AI_EvalFunc_TIMEGetDt(tAINode* node);
tAINode* AI_EvalFunc_TIMEFactorSet(tAINode* node);
tAINode* AI_EvalFunc_TIMELock(tAINode* node);
tAINode* AI_EvalFunc_TIMEUnlock(tAINode* node);

