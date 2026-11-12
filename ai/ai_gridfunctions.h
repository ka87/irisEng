#pragma once
#include "../common.h"

int i_Raster_339;

int AI_EvalFunc_GRIDGetCapa_C(tGameObjectMain* gao, Vec3* a2);
void AI_EvalFunc_GRIDSetCenter_C(struct sGameObjectMain* gao, Vec3* a2);
int AI_EvalFunc_GRIDModeSet_C(int a1);
int AI_EvalFunc_GRIDCurrentSet_C(int a1);
int AI_EvalFunc_GRIDTagVec_C(struct sGameObjectMain* gao, Vec3* pos, int a3, int a4, Vec3* a5);
int AI_EvalFunc_GRIDTagDyn_C(struct sGameObjectMain* gao, Vec3* a2, Vec3* a3, float a4, int a5);
int AI_EvalFunc_GRIDTagBox_C(struct sGameObjectMain* gao, Vec3* pos, Vec3* a3, int a4);
int AI_EvalFunc_GRIDTag_C(struct sGameObjectMain* gao, Vec3* a2, int a3, int a4);
BOOL AI_EvalFunc_GRIDIsIn_C(struct sGameObjectMain* gao, Vec3* a2);
Vec3* AI_EvalFunc_GRIDPosGet_C(struct sGameObjectMain* gao, Vec3* a2, Vec3* a3);
int AI_EvalFunc_GRIDDestSet_C(int a1);
void AI_EvalFunc_GRIDCptDest_C(struct sGameObjectMain* gao, Vec3* a2, float a3, Vec3* dest);
int AI_EvalFunc_GRIDShortWay_C(struct sGameObjectMain* gao, Vec3* a2, Vec3* a3, Vec3* a4, char a5, int a6, float a7, int a10, int a11);
int AI_EvalFunc_GRIDSmoothWay_C(struct sGameObjectMain* gao, Vec3* a2, int a4);
void AI_EvalFunc_GRIDSetPosAll_C(int a1, int a2, Vec3* a3);

tAINode* AI_EvalFunc_GRIDSetCenter(tAINode* node);
tAINode* AI_EvalFunc_GRIDSetCapa(tAINode* node);
tAINode* AI_EvalFunc_GRIDGetCapa(tAINode* node);
tAINode* AI_EvalFunc_GRIDIsIn(tAINode* node);
tAINode* AI_EvalFunc_GRIDShortWay(tAINode* node);
tAINode* AI_EvalFunc_GRIDPosGet(tAINode* node);
tAINode* AI_EvalFunc_GRIDAddCapa(tAINode* node);
tAINode* AI_EvalFunc_GRIDTag(tAINode* node);
tAINode* AI_EvalFunc_GRIDSmoothWay(tAINode* node);
tAINode* AI_EvalFunc_GRIDTagVec(tAINode* arg0);
tAINode* AI_EvalFunc_GRIDCurrentSet(tAINode* node);
tAINode* AI_EvalFunc_GRIDDestSet(tAINode* node);
tAINode* AI_EvalFunc_GRIDModeSet(tAINode* node);
tAINode* AI_EvalFunc_GRIDCptDest(tAINode* arg0);
tAINode* AI_EvalFunc_GRIDTagDyn(tAINode* arg0);
tAINode* AI_EvalFunc_GRIDSurround(tAINode* node);
tAINode* AI_EvalFunc_GRIDTagBox(tAINode* node);
tAINode* AI_EvalFunc_GRIDSetPosAll(tAINode* node);
tAINode* AI_EvalFunc_GRIDSetWall(tAINode* node);