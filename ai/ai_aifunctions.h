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
void AI_EvalFunc_AICBDel_C(struct sGameObjectMain* a1, struct sGameObjectMain* a2, int a3, struct sAIFunction* a4);
int AI_EvalFunc_AIPriorityGet_C(struct sGameObjectMain* a1);
void AI_EvalFunc_AIMainActorSet_C(struct sGameObjectMain* a1, int a2);
void AI_EvalFunc_AIMainActorReset_C(int a1);
struct sAIFunction* AI_EvalFunc_AITrackGet_C(int a1);
void AI_EvalFunc_AITrackCurStop_C();
void AI_EvalFunc_AIPrioritySet_C(struct sGameObjectMain* a1, char a2);
void AI_EvalFunc_AIReinitVars_C(struct sGameObjectMain* gao);

tAINode* AI_EvalFunc_CastVector(tAINode* a1);
tAINode* AI_EvalFunc_AIExecute(tAINode* a1);
tAINode* AI_EvalFunc_AITrackChange(tAINode* a1);
tAINode* AI_EvalFunc_AITrackStop(tAINode* a1);
tAINode* AI_EvalFunc_AITrackCurChange(tAINode* a1);
tAINode* AI_EvalFunc_AITrackCurStop(tAINode* a1);
tAINode* AI_EvalFunc_AITrackCurChangeNow(tAINode* a1);
tAINode* AI_EvalFunc_AITrackGet(tAINode* a1);
tAINode* AI_EvalFunc_AITrackCurGet(tAINode* a1);
tAINode* AI_EvalFunc_AIIsModel(tAINode* a1);
tAINode* AI_EvalFunc_AIPriorityGet(tAINode* a1);
tAINode* AI_EvalFunc_AIPrioritySet(tAINode* a1);
tAINode* AI_EvalFunc_AIMainActorSet(tAINode* a1);
tAINode* AI_EvalFunc_AIMainActorGet(tAINode* a1);
tAINode* AI_EvalFunc_AICBAdd(tAINode* a1);
tAINode* AI_EvalFunc_AICBDel(tAINode* a1);
tAINode* AI_EvalFunc_AITrackCurGetNum(tAINode* a1);
tAINode* AI_EvalFunc_AIMainActorReset(tAINode* a1);
tAINode* AI_EvalFunc_AIReinitUniv(tAINode* a1);
tAINode* AI_EvalFunc_AIReinitVars(tAINode* a1);
tAINode* AI_EvalFunc_AIGetCurSystem(tAINode* a1);
