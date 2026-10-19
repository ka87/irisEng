#pragma once
#include "../ai.h"
#include "../common.h"

void AI_EvalFunc_DbgTraceInt_C();
void AI_EvalFunc_DbgLastRayGet_Dummy_C(struct sGameObjectMain* a1, Vec3* a2);

tAINode* AI_EvalFunc_DbgTraceInt(tAINode* a1);
tAINode* AI_EvalFunc_DbgTraceFloat(tAINode* a1);
tAINode* AI_EvalFunc_DbgTraceVector(tAINode* arg0);
tAINode* AI_EvalFunc_DbgTraceString(tAINode* a1);
tAINode* AI_EvalFunc_DbgBreakPoint(tAINode* a1);
tAINode* AI_EvalFunc_DbgTraceChar(tAINode* a1);
tAINode* AI_EvalFunc_DbgTraceObject(tAINode* a1);
tAINode* AI_EvalFunc_DbgRenderVector(tAINode* arg0);
tAINode* AI_EvalFunc_DbgEraseAllVectors(tAINode* a1);
tAINode* AI_EvalFunc_DbgLastRayGet(tAINode* arg0);
tAINode* AI_EvalFunc_DbgClearLog(tAINode* a1);
tAINode* AI_EvalFunc_DbgStartRaster(tAINode* a1);
tAINode* AI_EvalFunc_DbgStopRaster(tAINode* a1);
tAINode* AI_EvalFunc_DbgRenderCircle(tAINode* arg0);
tAINode* AI_EvalFunc_DbgVarsCurToInit(tAINode* a1);
tAINode* AI_EvalFunc_DbgClearLog(tAINode* a1);;