#pragma once
#include "../ai.h"

void AI_EvalFunc_GRPSectInvisibleSet_C(struct sGameObjectMain* gao, int a2, int a3);
void AI_EvalFunc_GRPSectSetIsIn_C(struct sGameObjectMain* gao);
int AI_EvalFunc_GRPSectRegister_C(struct sGameObjectMain* gao);

tAINode* AI_EvalFunc_GRPSectInvisibleSet(tAINode* node);
tAINode* AI_EvalFunc_GRPSectInactiveSet(tAINode* node);
tAINode* AI_EvalFunc_GRPSectRegister(tAINode* node);
tAINode* AI_EvalFunc_GRPCullDoor(tAINode* node);
tAINode* AI_EvalFunc_GRPSectSetIsIn(tAINode* node);
tAINode* AI_EvalFunc_GRPFillArray(tAINode* node);