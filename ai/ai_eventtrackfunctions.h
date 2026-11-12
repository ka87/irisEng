#pragma once
#include "../ai.h"

BOOL AI_EvalFunc_TRAIsStop_C(struct sGameObjectMain* gao, int trackIdx);
void AI_EvalFunc_TRAPlay_C(struct sGameObjectMain* gao, int trackIdx);
int AI_EvalFunc_TRAIsStopAll_C(struct sGameObjectMain* gao);
void AI_EvalFunc_TRAPlayAll_C(struct sGameObjectMain* gao);
void AI_EvalFunc_TRASetEnd_C(struct sGameObjectMain* gao, int trackIdx);
void AI_EvalFunc_TRAStopAll_C(struct sGameObjectMain* gao);
void AI_EvalFunc_TRAReinitAll_C(struct sGameObjectMain* gao);

tAINode* AI_EvalFunc_TRAPlayAll(tAINode* node);
tAINode* AI_EvalFunc_TRAStopAll(tAINode* node);
tAINode* AI_EvalFunc_TRAPauseAll(tAINode* node);
tAINode* AI_EvalFunc_TRAUnPauseAll(tAINode* node);
tAINode* AI_EvalFunc_TRAPlay(tAINode* node);
tAINode* AI_EvalFunc_TRAStop(tAINode* node);
tAINode* AI_EvalFunc_TRAPause(tAINode* node);
tAINode* AI_EvalFunc_TRAUnPause(tAINode* node);
tAINode* AI_EvalFunc_TRAReinitAll(tAINode* node);
tAINode* AI_EvalFunc_TRAReinit(tAINode* node);
tAINode* AI_EvalFunc_TRAIsStop(tAINode* node);
tAINode* AI_EvalFunc_TRAIsStopAll(tAINode* node);
tAINode* AI_EvalFunc_TRASetEnd(tAINode* node);
tAINode* AI_EvalFunc_TRASetTime(tAINode* node);
tAINode* AI_EvalFunc_TRAGetTotalTime(tAINode* node);
tAINode* AI_EvalFunc_TRAGetCurrentTime(tAINode* node);
tAINode* AI_EvalFunc_TRASetTimeToLabel(tAINode* node);
