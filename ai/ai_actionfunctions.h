#pragma once
#include "../ai.h"

int AI_EvalFunc_ACTCustomBitTest_C(struct sGameObjectMain* a1, uint8_t a2);
void AI_EvalFunc_ACTActionSet_C(struct sGameObjectMain* a1, int actionId);
int AI_EvalFunc_ACTActionGet_C(struct sGameObjectMain* a1);
void AI_EvalFunc_ACTActionSet_C_CURRENT(int a1);
int AI_EvalFunc_ACTDesignFlagsTest_C(struct sGameObjectMain* a1, uint16_t a2);
int AI_EvalFunc_ACTCustomBitGet_C(struct sGameObjectMain* a1);
int AI_EvalFunc_ACTActionItemGet_C(struct sGameObjectMain* a1);
BOOL AI_EvalFunc_ACTActionFinished_C_CURRENT();
BOOL AI_EvalFunc_ACTActionIsTransition_C(struct sGameObjectMain* a1);
int AI_EvalFunc_ACTActionGetTransition_C(struct sGameObjectMain* a1);
int AI_EvalFunc_ACTActionGet_C_CURRENT();
int AI_EvalFunc_ACTFrequencyGet_C(struct sGameObjectMain* a1, int a2, int a3);
int AI_EvalFunc_ACTDesignFlagsGet_C(struct sGameObjectMain* a1);
BOOL AI_EvalFunc_ACTActionFinished_C(struct sGameObjectMain* a1);
BOOL AI_EvalFunc_ACTHasKit_C(struct sGameObjectMain* a1);

tAINode* AI_EvalFunc_ACTActionGet(tAINode* a1);
tAINode* AI_EvalFunc_ACTActionSet(tAINode* a1);
tAINode* AI_EvalFunc_ACTCustomBitGet(tAINode* a1);
tAINode* AI_EvalFunc_ACTCustomBitTest(tAINode* a1);
tAINode* AI_EvalFunc_ACTActionFinished(tAINode* a1);
tAINode* AI_EvalFunc_ACTActionIsTransition(tAINode* a1);
tAINode* AI_EvalFunc_ACTActionItemGet(tAINode* a1);
tAINode* AI_EvalFunc_ACTActionGetTransition(tAINode* a1);
tAINode* AI_EvalFunc_ACTKitMerge(tAINode* a1);
tAINode* AI_EvalFunc_ACTKitMergeOver(tAINode* a1);
tAINode* AI_EvalFunc_ACTKitRestore(tAINode* a1);
tAINode* AI_EvalFunc_ACTActionFrameSet(tAINode* a1);
tAINode* AI_EvalFunc_ACTActionFrequencySet(tAINode* a1);
tAINode* AI_EvalFunc_ACTDesignFlagsGet(tAINode* a1);
tAINode* AI_EvalFunc_ACTDesignFlagsTest(tAINode* a1);
tAINode* AI_EvalFunc_ACTHasKit(tAINode* a1);
tAINode* AI_EvalFunc_ACTFrequencyGet(tAINode* a1);
