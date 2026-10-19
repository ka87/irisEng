#pragma once
#include "../common.h"

void AI_EvalFunc_WORRefreshVisible_C(int arg0);
int AI_EvalFunc_WORLoaded_C();
void AI_EvalFunc_WORPreLoadTex_C();
int AI_EvalFunc_WORReinit_C();
void AI_EvalFunc_WORColorAmbiantSet_C(int a1, int a2);
void AI_EvalFunc_WORColorAmbiant2Set_C(int a1, int a2);
int AI_EvalFunc_WORGetKey_C();
int AI_EvalFunc_WORColorAmbiantGet_C(int a1);
int AI_EvalFunc_WORColorAmbiant2Get_C(int a1);
int AI_EvalFunc_WORCurIndexGet_C();
void AI_EvalFunc_WORPause_C(int a1, int a2);
void AI_EvalFunc_WORUnPause_C(int a1, int a2);
void AI_EvalFunc_WORRegisterES_C(int a1, struct sGameObjectMain* a2);
void AI_EvalFunc_WORRefreshVisibleOfActor_C();
int AI_EvalFunc_WORPlayAbsTime_C(float a1, float a2);

tAINode* AI_EvalFunc_WORUniverseAdd(tAINode* a1);
tAINode* AI_EvalFunc_WORActive(tAINode* a1);
tAINode* AI_EvalFunc_WORDesactive(tAINode* a1);
tAINode* AI_EvalFunc_WORGetObject(tAINode* a1);
tAINode* AI_EvalFunc_WORUniverseDestroy(tAINode* a1);
tAINode* AI_EvalFunc_WORLoadList(tAINode* a1);
tAINode* AI_EvalFunc_WORRefreshVisible(tAINode* a1);
tAINode* AI_EvalFunc_WORReinit(tAINode* param1);
tAINode* AI_EvalFunc_WORPause(tAINode* a1);
tAINode* AI_EvalFunc_WORUnPause(tAINode* a1);
tAINode* AI_EvalFunc_WORCurIndexGet(tAINode* a1);
tAINode* AI_EvalFunc_WORColorAmbiantSet(tAINode* a1);
tAINode* AI_EvalFunc_WORColorAmbiantGet(tAINode* a1);
tAINode* AI_EvalFunc_WORColorBackSet(tAINode* a1);
tAINode* AI_EvalFunc_WORColorBackGet(tAINode* a1);
tAINode* AI_EvalFunc_WORLoaded(tAINode* a1);
tAINode* AI_EvalFunc_WORRefreshVisibleOfActor(tAINode* a1);
tAINode* AI_EvalFunc_WORPlayAbsTime(tAINode* param1);
tAINode* AI_EvalFunc_WORIsPlayAbs(tAINode* a1);
tAINode* AI_EvalFunc_WORColorAmbiant2Set(tAINode* a1);
tAINode* AI_EvalFunc_WORColorAmbiant2Get(tAINode* a1);
tAINode* AI_EvalFunc_WORRegisterES(tAINode* a1);
tAINode* AI_EvalFunc_WORGetKey(tAINode* a1);
tAINode* AI_EvalFunc_WORCheckSetOrder(tAINode* a1);