#pragma once

#include "../common.h"
#include "../gameobject.h"

tMessage gst_Lifo;

struct sMessage* AI_EvalFunc_MSGGet_C_CURRENT();
void AI_EvalFunc_MSGRegOwner_C(struct sGameObjectMain* gao);
int AI_EvalFunc_MSGGetCount_C(struct sGameObjectMain* gao);
void AI_EvalFunc_MSGSend_C(struct sGameObjectMain* gao, struct sMessage* Src);
struct sMessage* AI_EvalFunc_MSGGetByID_C(struct sGameObjectMain* gao, int a2);
void AI_EvalFunc_MSGClear_C(struct sGameObjectMain* gao);
struct sMessage* AI_EvalFunc_MSGRead_C(struct sGameObjectMain* gao, int a2);
struct sMessage* AI_EvalFunc_MSGGet_C(struct sGameObjectMain* gao);

tAINode* AI_EvalFunc_MSGSend(tAINode* arg0);
tAINode* AI_EvalFunc_MSGGet(tAINode* arg0);
tAINode* AI_EvalFunc_MSGClear(tAINode* node);
tAINode* AI_EvalFunc_MSGOn(tAINode* node);
tAINode* AI_EvalFunc_MSGSendEx(tAINode* node);
tAINode* AI_EvalFunc_MSGGetByID(tAINode* arg0);
tAINode* AI_EvalFunc_MSGSendAll(tAINode* node);
tAINode* AI_EvalFunc_MSGTestInt1(tAINode* node);
tAINode* AI_EvalFunc_MSGRead(tAINode* arg0);
tAINode* AI_EvalFunc_MSGReadByID(tAINode* arg0);
tAINode* AI_EvalFunc_MSGGetCount(tAINode* node);
tAINode* AI_EvalFunc_MSGSendAllOwner(tAINode* node);
tAINode* AI_EvalFunc_MSGRegOwner(tAINode* node);