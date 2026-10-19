#pragma once

#include "../ai.h"
#include "../gameobject.h"

int* ARR_gpi_Global;

BOOL AI_EvalFunc_ARRBitTest_C(int* a1, int a2);
void AI_EvalFunc_ARRBitClear_C(int* a1, int a2);
void AI_EvalFunc_ARRBitSet_C(int* a1, int a2);
void AI_EvalFunc_ARRGlobalIntSet_C(int* a1);
int AI_EvalFunc_ARRObjSearch_C(struct sGameObjectMain** a1, int a2, struct sGameObjectMain* a3);
int AI_EvalFunc_ARRObjBestSight_C(struct sGameObjectMain** a1, int a2, Vec3* a3, Vec3* a4, float a5);
int AI_EvalFunc_ARRObjBestSightInfoPhoto_C(struct sGameObjectMain** arg0, int arg4, Vec3* a3, Vec3* a1, float a5);
int AI_EvalFunc_ARRObjNearestOfPoint_C(struct sGameObjectMain** a1, int arg4, Vec3* a2);
void* AI_EvalFunc_ARRObjCopy_C(void* a1, void* Src, int a3);
BOOL AI_EvalFunc_ARRBit4Test_C(int* a1, int a2, int a3, int a4, int a5);
void AI_EvalFunc_ARRBitChange_C(int* a1, int a2);
int AI_EvalFunc_ARRSortByFlags_C(struct sGameObjectMain** a1, int a2, int flagMustHave, int flagMustNothave, int whatToTest);
BOOL AI_EvalFunc_ARRGlobalIntFlagTest_C(int a1);
void AI_EvalFunc_ARRGlobalIntFlagSet_C(int a2);
void AI_EvalFunc_ARRGlobalIntFlagClear_C(int a2);
BOOL AI_EvalFunc_ARRBitOr_C(int* a1, int* a2, int* a3, int a4, int a5);
BOOL AI_EvalFunc_ARRBitAnd_C(int* a1, int* a2, int* a3, int a4, int a5);

tAINode* AI_EvalFunc_ARRObjNearestOfPoint(tAINode* a1);
tAINode* AI_EvalFunc_ARRObjCopy(tAINode* a1);
tAINode* AI_EvalFunc_ARRObjCopyIdFlags(tAINode* a1);
tAINode* AI_EvalFunc_ARRObjCopyCB(tAINode* a1);
tAINode* AI_EvalFunc_ARRObjBestSight(tAINode* arg0);
tAINode* AI_EvalFunc_ARRObjSearch(tAINode* a1);
tAINode* AI_EvalFunc_ARRBitSet(tAINode* a1);
tAINode* AI_EvalFunc_ARRBitClear(tAINode* a1);
tAINode* AI_EvalFunc_ARRBitSetRange(tAINode* a1);
tAINode* AI_EvalFunc_ARRBitClearRange(tAINode* a1);
tAINode* AI_EvalFunc_ARRBitTest(tAINode* a1);
tAINode* AI_EvalFunc_ARRBitChange(tAINode* a1);
tAINode* AI_EvalFunc_ARRBitAnd(tAINode* a1);
tAINode* AI_EvalFunc_ARRBitOr(tAINode* a1);
tAINode* AI_EvalFunc_ARRBit4Test(tAINode* a1);
tAINode* AI_EvalFunc_ARRObjBestSightBVCenter(tAINode* arg0);
tAINode* AI_EvalFunc_ARRGlobalIntSet(tAINode* a1);
tAINode* AI_EvalFunc_ARRGlobalIntFlagSet(tAINode* a1);
tAINode* AI_EvalFunc_ARRGlobalIntFlagClear(tAINode* a1);
tAINode* AI_EvalFunc_ARRGlobalIntFlagTest(tAINode* a1);
tAINode* AI_EvalFunc_ARRSortByFlags(tAINode* a1);
tAINode* AI_EvalFunc_ARRObjBestSightInfoPhoto(tAINode* arg0);