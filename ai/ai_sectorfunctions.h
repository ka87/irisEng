#pragma once
#include "../ai.h"

void AI_EvalFunc_GRPSectInvisibleSet_C(struct sGameObjectMain* a1, int a2, int a3);
void AI_EvalFunc_GRPSectSetIsIn_C(struct sGameObjectMain* a1);
int AI_EvalFunc_GRPSectRegister_C(struct sGameObjectMain* a1);

tAINode* AI_EvalFunc_GRPSectInvisibleSet(tAINode* a1);
tAINode* AI_EvalFunc_GRPSectInactiveSet(tAINode* a1);
tAINode* AI_EvalFunc_GRPSectRegister(tAINode* a1);
tAINode* AI_EvalFunc_GRPCullDoor(tAINode* a1);
tAINode* AI_EvalFunc_GRPSectSetIsIn(tAINode* a1);
tAINode* AI_EvalFunc_GRPFillArray(tAINode* a1);