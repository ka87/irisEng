#pragma once
#include "../ai.h"
#include "../animation.h"

void AI_EvalFunc_ANIShapeSelect_C(struct sGameObjectMain* gao, uint8_t aiCanal, uint8_t shapeIdx);
void AI_EvalFunc_ANICloneSet_C(struct sGameObjectMain* gao, struct sGameObjectMain* source, int clone);
struct sGameObjectMain* AI_EvalFunc_ANICanalGaoGet_C(struct sGameObjectMain* gao, uint8_t aiCanal);
struct sGameObjectMain* AI_EvalFunc_ANICanalGaoGet_C_CURRENT(uint8_t aiCanal);
unsigned int AI_EvalFunc_ANICurrentFrameGet_C(struct sGameObjectMain* gao, int animIdx);
void AI_EvalFunc_ANIBlendFrameMaxSet_C(struct sGameObjectMain* gao, int frameMax);
BOOL AI_EvalFunc_ANIIsInBlend_C_CURRENT();
int AI_EvalFunc_ANIFrequencySet_C(struct sGameObjectMain* gao, int animIdx, int freq);
int AI_EvalFunc_ANIFrequencyGet_C(struct sGameObjectMain* gao, int animIdx);
struct sGameObjectMain* AI_EvalFunc_ANIShapeObjectGet_C(struct sGameObjectMain* gao, char aiCanal, uint8_t shapeIdx);
int AI_EvalFunc_ANIShapeGet_C(struct sGameObjectMain* gao, char aiCanal);
int AI_EvalFunc_ANIIsBoneAnimed_C(struct sGameObjectMain* gao, uint8_t aiCanal);
void AI_EvalFunc_ANICurrentFrameSet_C(struct sGameObjectMain* gao, int animIdx, int frame);
unsigned int AI_EvalFunc_ANINbFrameGet_C(struct sGameObjectMain* gao, int animIdx);
void AI_EvalFunc_ANIPelvisOffsetSet_C(struct sGameObjectMain* gao, float offset);

tAINode* AI_EvalFunc_ANIFrequencyGet(tAINode* node);
tAINode* AI_EvalFunc_ANIFrequencySet(tAINode* node);
tAINode* AI_EvalFunc_ANICurrentFrameGet(tAINode* node);
tAINode* AI_EvalFunc_ANICurrentFrameSet(tAINode* node);
tAINode* AI_EvalFunc_ANINbFrameGet(tAINode* node);
tAINode* AI_EvalFunc_ANIAnimGet(tAINode* node);
tAINode* AI_EvalFunc_ANIFlagGet(tAINode* node);
tAINode* AI_EvalFunc_ANIFlagSet(tAINode* node);
tAINode* AI_EvalFunc_ANIFlagTest(tAINode* node);
tAINode* AI_EvalFunc_ANIModeGet(tAINode* node);
tAINode* AI_EvalFunc_ANIModeSet(tAINode* node);
tAINode* AI_EvalFunc_ANIShapeSelect(tAINode* node);
tAINode* AI_EvalFunc_ANICanalGaoGet(tAINode* node);
tAINode* AI_EvalFunc_ANIIsInBlend(tAINode* node);
tAINode* AI_EvalFunc_ANISkeletonChange(tAINode* node);
tAINode* AI_EvalFunc_ANISetEnd(tAINode* node);
tAINode* AI_EvalFunc_ANISkeletonRestore(tAINode* node);
tAINode* AI_EvalFunc_ANICloneSet(tAINode* node);
tAINode* AI_EvalFunc_ANIShapeGet(tAINode* node);
tAINode* AI_EvalFunc_ANIShapeObjectGet(tAINode* node);
tAINode* AI_EvalFunc_ANIBitFieldLODSet(tAINode* node);
tAINode* AI_EvalFunc_ANILODSet(tAINode* node);
tAINode* AI_EvalFunc_ANIIsBoneAnimed(tAINode* node);
tAINode* AI_EvalFunc_ANIBlendFrameMaxSet(tAINode* node);
tAINode* AI_EvalFunc_ANIPelvisOffsetSet(tAINode* node);