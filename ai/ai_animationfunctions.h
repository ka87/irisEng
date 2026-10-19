#pragma once
#include "../ai.h"
#include "../animation.h"

void AI_EvalFunc_ANIShapeSelect_C(struct sGameObjectMain* a1, unsigned __int8 a2, unsigned __int8 a3);
void AI_EvalFunc_ANICloneSet_C(struct sGameObjectMain* a1, struct sGameObjectMain* a2, int a3);
struct sGameObjectMain* AI_EvalFunc_ANICanalGaoGet_C(struct sGameObjectMain* a1, unsigned __int8 a2);
struct sGameObjectMain* AI_EvalFunc_ANICanalGaoGet_C_CURRENT(unsigned __int8 a1);
unsigned int AI_EvalFunc_ANICurrentFrameGet_C(struct sGameObjectMain* a1, int a2);
void AI_EvalFunc_ANIBlendFrameMaxSet_C(struct sGameObjectMain* a1, int a2);
BOOL AI_EvalFunc_ANIIsInBlend_C_CURRENT();
int AI_EvalFunc_ANIFrequencySet_C(struct sGameObjectMain* a1, int a2, int a3);
int AI_EvalFunc_ANIFrequencyGet_C(struct sGameObjectMain* a1, int a2);
struct sGameObjectMain* AI_EvalFunc_ANIShapeObjectGet_C(struct sGameObjectMain* a1, char a2, unsigned __int8 a3);
int AI_EvalFunc_ANIShapeGet_C(struct sGameObjectMain* a1, char a2);
int AI_EvalFunc_ANIIsBoneAnimed_C(struct sGameObjectMain* a1, unsigned __int8 a2);
void AI_EvalFunc_ANICurrentFrameSet_C(struct sGameObjectMain* a1, int a2, int a3);
unsigned int AI_EvalFunc_ANINbFrameGet_C(struct sGameObjectMain* a1, int a2);
void AI_EvalFunc_ANIPelvisOffsetSet_C(struct sGameObjectMain* a1, float a2);

tAINode* AI_EvalFunc_ANIFrequencyGet(tAINode* a1);
tAINode* AI_EvalFunc_ANIFrequencySet(tAINode* a1);
tAINode* AI_EvalFunc_ANICurrentFrameGet(tAINode* a1);
tAINode* AI_EvalFunc_ANICurrentFrameSet(tAINode* a1);
tAINode* AI_EvalFunc_ANINbFrameGet(tAINode* a1);
tAINode* AI_EvalFunc_ANIAnimGet(tAINode* a1);
tAINode* AI_EvalFunc_ANIFlagGet(tAINode* a1);
tAINode* AI_EvalFunc_ANIFlagSet(tAINode* a1);
tAINode* AI_EvalFunc_ANIFlagTest(tAINode* a1);
tAINode* AI_EvalFunc_ANIModeGet(tAINode* a1);
tAINode* AI_EvalFunc_ANIModeSet(tAINode* a1);
tAINode* AI_EvalFunc_ANIShapeSelect(tAINode* a1);
tAINode* AI_EvalFunc_ANICanalGaoGet(tAINode* a1);
tAINode* AI_EvalFunc_ANIIsInBlend(tAINode* a1);
tAINode* AI_EvalFunc_ANISkeletonChange(tAINode* a1);
tAINode* AI_EvalFunc_ANISetEnd(tAINode* a1);
tAINode* AI_EvalFunc_ANISkeletonRestore(tAINode* a1);
tAINode* AI_EvalFunc_ANICloneSet(tAINode* a1);
tAINode* AI_EvalFunc_ANIShapeGet(tAINode* a1);
tAINode* AI_EvalFunc_ANIShapeObjectGet(tAINode* arg0);
tAINode* AI_EvalFunc_ANIBitFieldLODSet(tAINode* a1);
tAINode* AI_EvalFunc_ANILODSet(tAINode* a1);
tAINode* AI_EvalFunc_ANIIsBoneAnimed(tAINode* a1);
tAINode* AI_EvalFunc_ANIBlendFrameMaxSet(tAINode* a1);
tAINode* AI_EvalFunc_ANIPelvisOffsetSet(tAINode* a1);