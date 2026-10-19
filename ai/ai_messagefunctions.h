#pragma once

#include "../common.h"
#include "../gameobject.h"

tMessage gst_Lifo;

struct sMessage* AI_EvalFunc_MSGGet_C_CURRENT();
void AI_EvalFunc_MSGRegOwner_C(struct sGameObjectMain* a1);
int AI_EvalFunc_MSGGetCount_C(struct sGameObjectMain* a1);
void AI_EvalFunc_MSGSend_C(struct sGameObjectMain* a1, struct sMessage* Src);
struct sMessage* AI_EvalFunc_MSGGetByID_C(struct sGameObjectMain* a1, int a2);
void AI_EvalFunc_MSGClear_C(struct sGameObjectMain* a1);
struct sMessage* AI_EvalFunc_MSGRead_C(struct sGameObjectMain* a1, int a2);
struct sMessage* AI_EvalFunc_MSGGet_C(struct sGameObjectMain* a1);

tAINode* AI_EvalFunc_MSGSend(tAINode* arg0);
tAINode* AI_EvalFunc_MSGGet(tAINode* arg0);
tAINode* AI_EvalFunc_MSGClear(tAINode* a1);
tAINode* AI_EvalFunc_MSGOn(tAINode* a1);
tAINode* AI_EvalFunc_MSGSendEx(tAINode* a1);
tAINode* AI_EvalFunc_MSGGetByID(tAINode* arg0);
tAINode* AI_EvalFunc_MSGSendAll(tAINode* a1);
tAINode* AI_EvalFunc_MSGTestInt1(tAINode* a1);
tAINode* AI_EvalFunc_MSGRead(tAINode* arg0);
tAINode* AI_EvalFunc_MSGReadByID(tAINode* arg0);
tAINode* AI_EvalFunc_MSGGetCount(tAINode* a1);
tAINode* AI_EvalFunc_MSGSendAllOwner(tAINode* a1);
tAINode* AI_EvalFunc_MSGRegOwner(tAINode* a1);