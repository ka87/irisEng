#pragma once

#include "../common.h"

void AI_EvalFunc_AITrackCurChange_C(struct sAIFunction* a1);
int AI_EvalFunc_AITrackCurChangeNow_C(struct sAIFunction* a1);
int AI_EvalFunc_AIGetCurSystem_C();
void AI_EvalFunc_AITrackStop_C(int a1);
int AI_EvalFunc_AIExecute_C(struct sAIFunction* funcPtr);
int AI_EvalFunc_AITrackChange_C(int a1, struct sAIFunction* a2);
int AI_EvalFunc_AITrackCurGetNum_C();
struct sAIFunction* AI_EvalFunc_AITrackCurGet_C();
void AI_EvalFunc_AICBAdd_C(struct sGameObjectMain* gao, struct sGameObjectMain* a2, int id, struct sAIFunction* ptr);
void AI_EvalFunc_AICBDel_C(struct sGameObjectMain* gao, struct sGameObjectMain* a2, int a3, struct sAIFunction* a4);
int AI_EvalFunc_AIPriorityGet_C(struct sGameObjectMain* gao);
void AI_EvalFunc_AIMainActorSet_C(struct sGameObjectMain* gao, int a2);
void AI_EvalFunc_AIMainActorReset_C(int a1);
struct sAIFunction* AI_EvalFunc_AITrackGet_C(int a1);
void AI_EvalFunc_AITrackCurStop_C();
void AI_EvalFunc_AIPrioritySet_C(struct sGameObjectMain* gao, char a2);
void AI_EvalFunc_AIReinitVars_C(struct sGameObjectMain* gao);

tAINode* AI_EvalFunc_CastVector(tAINode* node);
tAINode* AI_EvalFunc_AIExecute(tAINode* node);
tAINode* AI_EvalFunc_AITrackChange(tAINode* node);
tAINode* AI_EvalFunc_AITrackStop(tAINode* node);
tAINode* AI_EvalFunc_AITrackCurChange(tAINode* node);
tAINode* AI_EvalFunc_AITrackCurStop(tAINode* node);
tAINode* AI_EvalFunc_AITrackCurChangeNow(tAINode* node);
tAINode* AI_EvalFunc_AITrackGet(tAINode* node);
tAINode* AI_EvalFunc_AITrackCurGet(tAINode* node);
tAINode* AI_EvalFunc_AIIsModel(tAINode* node);
tAINode* AI_EvalFunc_AIPriorityGet(tAINode* node);
tAINode* AI_EvalFunc_AIPrioritySet(tAINode* node);
tAINode* AI_EvalFunc_AIMainActorSet(tAINode* node);
tAINode* AI_EvalFunc_AIMainActorGet(tAINode* node);
tAINode* AI_EvalFunc_AICBAdd(tAINode* node);
tAINode* AI_EvalFunc_AICBDel(tAINode* node);
tAINode* AI_EvalFunc_AITrackCurGetNum(tAINode* node);
tAINode* AI_EvalFunc_AIMainActorReset(tAINode* node);
tAINode* AI_EvalFunc_AIReinitUniv(tAINode* node);
tAINode* AI_EvalFunc_AIReinitVars(tAINode* node);
tAINode* AI_EvalFunc_AIGetCurSystem(tAINode* node);
