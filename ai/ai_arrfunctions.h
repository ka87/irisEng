#pragma once

#include "../ai.h"
#include "../gameobject.h"

int* ARR_gpi_Global;

BOOL AI_EvalFunc_ARRBitTest_C(int* bits, int bit);
void AI_EvalFunc_ARRBitClear_C(int* bits, int bit);
void AI_EvalFunc_ARRBitSet_C(int* bits, int bit);
void AI_EvalFunc_ARRGlobalIntSet_C(int* bits);
int AI_EvalFunc_ARRObjSearch_C(struct sGameObjectMain** objs, int num, struct sGameObjectMain* gao);
int AI_EvalFunc_ARRObjBestSight_C(struct sGameObjectMain** objs, int num, Vec3* from, Vec3* direction, float falloff);
int AI_EvalFunc_ARRObjBestSightBVCenter_C(struct sGameObjectMain** objs, int num, Vec3* from, Vec3* direction, float falloff);
int AI_EvalFunc_ARRObjBestSightInfoPhoto_C(struct sGameObjectMain** objs, int num, Vec3* from, Vec3* direction, float toleranceDeg);
int AI_EvalFunc_ARRObjNearestOfPoint_C(struct sGameObjectMain** objs, int num, Vec3* pos);
void* AI_EvalFunc_ARRObjCopy_C(void* dst, void* src, int num);
BOOL AI_EvalFunc_ARRBit4Test_C(int* bits, int bit1, int bit2, int bit3, int bit4);
void AI_EvalFunc_ARRBitChange_C(int* bits, int bit);
int AI_EvalFunc_ARRSortByFlags_C(struct sGameObjectMain** objs, int num, int flagMustHave, int flagMustNothave, int whatToTest);
BOOL AI_EvalFunc_ARRGlobalIntFlagTest_C(int bit);
void AI_EvalFunc_ARRGlobalIntFlagSet_C(int bit);
void AI_EvalFunc_ARRGlobalIntFlagClear_C(int bit);
BOOL AI_EvalFunc_ARRBitOr_C(int* bits, int* bitIdx, int* expected, int num, int skip);
BOOL AI_EvalFunc_ARRBitAnd_C(int* bits, int* bitIdx, int* expected, int num, int skip);

tAINode* AI_EvalFunc_ARRObjNearestOfPoint(tAINode* node);
tAINode* AI_EvalFunc_ARRObjCopy(tAINode* node);
tAINode* AI_EvalFunc_ARRObjCopyIdFlags(tAINode* node);
tAINode* AI_EvalFunc_ARRObjCopyCB(tAINode* node);
tAINode* AI_EvalFunc_ARRObjBestSight(tAINode* node);
tAINode* AI_EvalFunc_ARRObjSearch(tAINode* node);
tAINode* AI_EvalFunc_ARRBitSet(tAINode* node);
tAINode* AI_EvalFunc_ARRBitClear(tAINode* node);
tAINode* AI_EvalFunc_ARRBitSetRange(tAINode* node);
tAINode* AI_EvalFunc_ARRBitClearRange(tAINode* node);
tAINode* AI_EvalFunc_ARRBitTest(tAINode* node);
tAINode* AI_EvalFunc_ARRBitChange(tAINode* node);
tAINode* AI_EvalFunc_ARRBitAnd(tAINode* node);
tAINode* AI_EvalFunc_ARRBitOr(tAINode* node);
tAINode* AI_EvalFunc_ARRBit4Test(tAINode* node);
tAINode* AI_EvalFunc_ARRObjBestSightBVCenter(tAINode* node);
tAINode* AI_EvalFunc_ARRGlobalIntSet(tAINode* node);
tAINode* AI_EvalFunc_ARRGlobalIntFlagSet(tAINode* node);
tAINode* AI_EvalFunc_ARRGlobalIntFlagClear(tAINode* node);
tAINode* AI_EvalFunc_ARRGlobalIntFlagTest(tAINode* node);
tAINode* AI_EvalFunc_ARRSortByFlags(tAINode* node);
tAINode* AI_EvalFunc_ARRObjBestSightInfoPhoto(tAINode* node);