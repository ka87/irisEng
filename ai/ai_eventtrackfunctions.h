#pragma once
#include "../ai.h"

BOOL AI_EvalFunc_TRAIsStop_C(struct sGameObjectMain* a1, int a2);
void AI_EvalFunc_TRAPlay_C(struct sGameObjectMain* a1, int a2);
int AI_EvalFunc_TRAIsStopAll_C(struct sGameObjectMain* a1);
void AI_EvalFunc_TRAPlayAll_C(struct sGameObjectMain* a1);
void AI_EvalFunc_TRASetEnd_C(struct sGameObjectMain* a1, int a2);
void AI_EvalFunc_TRAStopAll_C(struct sGameObjectMain* a1);
void AI_EvalFunc_TRAReinitAll_C(struct sGameObjectMain* a1);

tAINode* AI_EvalFunc_TRAPlayAll(tAINode* a1);
tAINode* AI_EvalFunc_TRAStopAll(tAINode* a1);
tAINode* AI_EvalFunc_TRAPauseAll(tAINode* a1);
tAINode* AI_EvalFunc_TRAUnPauseAll(tAINode* a1);
tAINode* AI_EvalFunc_TRAPlay(tAINode* a1);
tAINode* AI_EvalFunc_TRAStop(tAINode* a1);
tAINode* AI_EvalFunc_TRAPause(tAINode* a1);
tAINode* AI_EvalFunc_TRAUnPause(tAINode* a1);
tAINode* AI_EvalFunc_TRAReinitAll(tAINode* a1);
tAINode* AI_EvalFunc_TRAReinit(tAINode* a1);
tAINode* AI_EvalFunc_TRAIsStop(tAINode* a1);
tAINode* AI_EvalFunc_TRAIsStopAll(tAINode* a1);
tAINode* AI_EvalFunc_TRASetEnd(tAINode* a1);
tAINode* AI_EvalFunc_TRASetTime(tAINode* a1);
tAINode* AI_EvalFunc_TRAGetTotalTime(tAINode* a1);
tAINode* AI_EvalFunc_TRAGetCurrentTime(tAINode* a1);
tAINode* AI_EvalFunc_TRASetTimeToLabel(tAINode* a1);
