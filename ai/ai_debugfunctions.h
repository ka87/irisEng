#pragma once
#include "../ai.h"
#include "../common.h"

void AI_EvalFunc_DbgTraceInt_C();
void AI_EvalFunc_DbgLastRayGet_Dummy_C(struct sGameObjectMain* gao, Vec3* ray);

tAINode* AI_EvalFunc_DbgTraceInt(tAINode* node);
tAINode* AI_EvalFunc_DbgTraceFloat(tAINode* node);
tAINode* AI_EvalFunc_DbgTraceVector(tAINode* arg0);
tAINode* AI_EvalFunc_DbgTraceString(tAINode* node);
tAINode* AI_EvalFunc_DbgBreakPoint(tAINode* node);
tAINode* AI_EvalFunc_DbgTraceChar(tAINode* node);
tAINode* AI_EvalFunc_DbgTraceObject(tAINode* node);
tAINode* AI_EvalFunc_DbgRenderVector(tAINode* arg0);
tAINode* AI_EvalFunc_DbgEraseAllVectors(tAINode* node);
tAINode* AI_EvalFunc_DbgLastRayGet(tAINode* arg0);
tAINode* AI_EvalFunc_DbgClearLog(tAINode* node);
tAINode* AI_EvalFunc_DbgStartRaster(tAINode* node);
tAINode* AI_EvalFunc_DbgStopRaster(tAINode* node);
tAINode* AI_EvalFunc_DbgRenderCircle(tAINode* arg0);
tAINode* AI_EvalFunc_DbgVarsCurToInit(tAINode* node);
tAINode* AI_EvalFunc_DbgClearLog(tAINode* node);;