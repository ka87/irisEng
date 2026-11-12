#pragma once
#include "../common.h"

void AI_EvalFunc_VIEWSnapshot_DoIt();
struct sGameObjectMain* AI_EvalFunc_VIEWAssignObject_C(struct sGameObjectMain* gao, int viewIdx);
void AI_EvalFunc_VIEWFocaleSet_C(int a1, float a2);
void AI_EvalFunc_VIEWForceNoFog_C(int a1);
void AI_EvalFunc_VIEW3dWorldTo2d_C(int a1, Vec3* a2, Vec3* a3);
void AI_EvalFunc_VIEW2dTo3d_C(int a1, Vec3* a2, Vec3* a3);
struct sGameObjectMain* AI_EvalFunc_VIEWGetObject_C(int a1);
int AI_EvalFunc_VIEWAnalyser_C(struct sGameObjectMain* gao, int a2);
void AI_EvalFunc_VIEWSnapshot_C(int slotID);
float AI_EvalFunc_VIEWFocaleGet_C(int a1);
BOOL AI_EvalFunc_VIEWCulled_C(struct sGameObjectMain* gao, int a2);

tAINode* AI_EvalFunc_VIEWAssignObject(tAINode* node);
tAINode* AI_EvalFunc_VIEWCreate(tAINode* node);
tAINode* AI_EvalFunc_VIEWDestroy(tAINode* node);
tAINode* AI_EvalFunc_VIEWGetObject(tAINode* node);
tAINode* AI_EvalFunc_VIEWFocaleSet(tAINode* node);
tAINode* AI_EvalFunc_VIEWFocaleGet(tAINode* node);
tAINode* AI_EvalFunc_VIEWViewportPosSet(tAINode* node);
tAINode* AI_EvalFunc_VIEWViewportPosGet(tAINode* node);
tAINode* AI_EvalFunc_VIEWViewportSizeSet(tAINode* node);
tAINode* AI_EvalFunc_VIEWViewportSizeGet(tAINode* node);
tAINode* AI_EvalFunc_VIEW3dWorldTo2d(tAINode* node);
tAINode* AI_EvalFunc_VIEWWidth(tAINode* node);
tAINode* AI_EvalFunc_VIEWHeight(tAINode* aiNode);
tAINode* AI_EvalFunc_VIEWSnapshot(tAINode* node);
tAINode* AI_EvalFunc_VIEWAnalyser(tAINode* node);
tAINode* AI_EvalFunc_VIEWCulled(tAINode* node);
tAINode* AI_EvalFunc_VIEW2dTo3d(tAINode* node);
tAINode* AI_EvalFunc_VIEWDrawMaskSet(tAINode* node);
tAINode* AI_EvalFunc_VIEWForceNoFog(tAINode* node);
