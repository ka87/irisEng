#pragma once
#include "../common.h"

int i_Raster_339;

int AI_EvalFunc_GRIDGetCapa_C(tGameObjectMain* gao, Vec3* pos);
void AI_EvalFunc_GRIDSetCenter_C(struct sGameObjectMain* gao, Vec3* pos);
int AI_EvalFunc_GRIDModeSet_C(int mode);
int AI_EvalFunc_GRIDCurrentSet_C(int which);
int AI_EvalFunc_GRIDTagVec_C(struct sGameObjectMain* gao, Vec3* pos, int shape, int capability, Vec3* dir);
int AI_EvalFunc_GRIDTagDyn_C(struct sGameObjectMain* gao, Vec3* pos, Vec3* cells, float distLimit, char capability);
int AI_EvalFunc_GRIDTagBox_C(struct sGameObjectMain* gao, Vec3* pos, Vec3* to, int capability);
int AI_EvalFunc_GRIDTag_C(struct sGameObjectMain* gao, Vec3* pos, int shape, int capability);
BOOL AI_EvalFunc_GRIDIsIn_C(struct sGameObjectMain* gao, Vec3* pos);
Vec3* AI_EvalFunc_GRIDPosGet_C(struct sGameObjectMain* gao, Vec3* pos, Vec3* cellCentre);
int AI_EvalFunc_GRIDDestSet_C(int which);
void AI_EvalFunc_GRIDCptDest_C(struct sGameObjectMain* gao, Vec3* target, float step, Vec3* dest);
int AI_EvalFunc_GRIDShortWay_C(struct sGameObjectMain* gao, Vec3* pathOut, Vec3* start, Vec3* target, char lowByte, int nearLimit, float maxDist, int maxCells, int maxPoints);
int AI_EvalFunc_GRIDSmoothWay_C(struct sGameObjectMain* gao, Vec3* path, int num);
void AI_EvalFunc_GRIDSetPosAll_C(struct sGameObjectMain* gao, int which, Vec3* origin);

tAINode* AI_EvalFunc_GRIDSetCenter(tAINode* node);
tAINode* AI_EvalFunc_GRIDSetCapa(tAINode* node);
tAINode* AI_EvalFunc_GRIDGetCapa(tAINode* node);
tAINode* AI_EvalFunc_GRIDIsIn(tAINode* node);
tAINode* AI_EvalFunc_GRIDShortWay(tAINode* node);
tAINode* AI_EvalFunc_GRIDPosGet(tAINode* node);
tAINode* AI_EvalFunc_GRIDAddCapa(tAINode* node);
tAINode* AI_EvalFunc_GRIDTag(tAINode* node);
tAINode* AI_EvalFunc_GRIDSmoothWay(tAINode* node);
tAINode* AI_EvalFunc_GRIDTagVec(tAINode* node);
tAINode* AI_EvalFunc_GRIDCurrentSet(tAINode* node);
tAINode* AI_EvalFunc_GRIDDestSet(tAINode* node);
tAINode* AI_EvalFunc_GRIDModeSet(tAINode* node);
tAINode* AI_EvalFunc_GRIDCptDest(tAINode* node);
tAINode* AI_EvalFunc_GRIDTagDyn(tAINode* node);
tAINode* AI_EvalFunc_GRIDSurround(tAINode* node);
tAINode* AI_EvalFunc_GRIDTagBox(tAINode* node);
tAINode* AI_EvalFunc_GRIDSetPosAll(tAINode* node);
tAINode* AI_EvalFunc_GRIDSetWall(tAINode* node);