#pragma once

#include "../ai.h"

void AI_EvalFunc_LightSetFlag_C(struct sGameObjectMain* a2, char a3, int a4);
void AI_EvalFunc_LightSetGO_C(struct sGameObjectMain* gao, struct sGameObjectMain* a2);
void AI_EvalFunc_LightSetColor_C(struct sGameObjectMain* gao, int a2);
void AI_EvalFunc_LightSetAttenuationFactor_C(struct sGameObjectMain* gao, float a2);
void AI_EvalFunc_LightSetRLIFactor_C(float a1);
void AI_EvalFunc_LightSetRLIDestColor_C(int a1);
float AI_EvalFunc_LightGetRLIFactor_C();
float AI_EvalFunc_LightGetRLIFactor_C();
int AI_EvalFunc_LightGetRLIDestColor_C();
void AI_EvalFunc_LightSetNearFar_C(struct sGameObjectMain* gao, float a2, float a3);
BOOL AI_EvalFunc_LightRLIFactorUsed_C(int a1);
BOOL AI_EvalFunc_LightGetFlag_C(struct sGameObjectMain* gao, char a2);
int AI_EvalFunc_LightGetColor_C(struct sGameObjectMain* gao);

tAINode* AI_EvalFunc_LightGetColor(tAINode* node);
tAINode* AI_EvalFunc_LightSetColor(tAINode* node);
tAINode* AI_EvalFunc_LightSetNearFar(tAINode* node);
tAINode* AI_EvalFunc_LightGetNear(tAINode* node);
tAINode* AI_EvalFunc_LightGetFar(tAINode* node);
tAINode* AI_EvalFunc_LightSetGO(tAINode* node);
tAINode* AI_EvalFunc_LightSetFlag(tAINode* node);
tAINode* AI_EvalFunc_LightGetFlag(tAINode* node);
tAINode* AI_EvalFunc_LightGetRLIFactor(tAINode* node);
tAINode* AI_EvalFunc_LightSetRLIFactor(tAINode* node);
tAINode* AI_EvalFunc_LightGetRLIDestColor(tAINode* node);
tAINode* AI_EvalFunc_LightSetRLIDestColor(tAINode* node);
tAINode* AI_EvalFunc_LightRLIFactorUsed(tAINode* node);
tAINode* AI_EvalFunc_LightFogState(tAINode* node);
tAINode* AI_EvalFunc_LightGetFog(tAINode* node);
tAINode* AI_EvalFunc_LightGetAttenuationFactor(tAINode* node);
tAINode* AI_EvalFunc_LightSetAttenuationFactor(tAINode* node);