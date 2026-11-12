#pragma once
#include "../ai.h"

int AI_EvalFunc_ACTCustomBitTest_C(struct sGameObjectMain* gao, uint8_t mask);
void AI_EvalFunc_ACTActionSet_C(struct sGameObjectMain* gao, int actionId);
int AI_EvalFunc_ACTActionGet_C(struct sGameObjectMain* gao);
void AI_EvalFunc_ACTActionSet_C_CURRENT(int actionId);
int AI_EvalFunc_ACTDesignFlagsTest_C(struct sGameObjectMain* gao, uint16_t mask);
int AI_EvalFunc_ACTCustomBitGet_C(struct sGameObjectMain* gao);
int AI_EvalFunc_ACTActionItemGet_C(struct sGameObjectMain* gao);
BOOL AI_EvalFunc_ACTActionFinished_C_CURRENT();
BOOL AI_EvalFunc_ACTActionIsTransition_C(struct sGameObjectMain* gao);
int AI_EvalFunc_ACTActionGetTransition_C(struct sGameObjectMain* gao);
int AI_EvalFunc_ACTActionGet_C_CURRENT();
int AI_EvalFunc_ACTFrequencyGet_C(struct sGameObjectMain* gao, int actionId, int itemId);
int AI_EvalFunc_ACTDesignFlagsGet_C(struct sGameObjectMain* gao);
BOOL AI_EvalFunc_ACTActionFinished_C(struct sGameObjectMain* gao);
BOOL AI_EvalFunc_ACTHasKit_C(struct sGameObjectMain* gao);

tAINode* AI_EvalFunc_ACTActionGet(tAINode* node);
tAINode* AI_EvalFunc_ACTActionSet(tAINode* node);
tAINode* AI_EvalFunc_ACTCustomBitGet(tAINode* node);
tAINode* AI_EvalFunc_ACTCustomBitTest(tAINode* node);
tAINode* AI_EvalFunc_ACTActionFinished(tAINode* node);
tAINode* AI_EvalFunc_ACTActionIsTransition(tAINode* node);
tAINode* AI_EvalFunc_ACTActionItemGet(tAINode* node);
tAINode* AI_EvalFunc_ACTActionGetTransition(tAINode* node);
tAINode* AI_EvalFunc_ACTKitMerge(tAINode* node);
tAINode* AI_EvalFunc_ACTKitMergeOver(tAINode* node);
tAINode* AI_EvalFunc_ACTKitRestore(tAINode* node);
tAINode* AI_EvalFunc_ACTActionFrameSet(tAINode* node);
tAINode* AI_EvalFunc_ACTActionFrequencySet(tAINode* node);
tAINode* AI_EvalFunc_ACTDesignFlagsGet(tAINode* node);
tAINode* AI_EvalFunc_ACTDesignFlagsTest(tAINode* node);
tAINode* AI_EvalFunc_ACTHasKit(tAINode* node);
tAINode* AI_EvalFunc_ACTFrequencyGet(tAINode* node);
