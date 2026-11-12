#pragma once
#include "../ai.h"

int NET_b_Client;
int NET_b_Init;
int NET_b_Server;

tAINode* AI_EvalFunc_NETCommand(tAINode* node);
tAINode* AI_EvalFunc_NETCommandStr(tAINode* node);
tAINode* AI_EvalFunc_NETServerGetName(tAINode* node);
tAINode* AI_EvalFunv_NETPushi(tAINode* a1);
tAINode* AI_EvalFunv_NETPushf(tAINode* a1);
tAINode* AI_EvalFunv_NETPopf(tAINode* a1);
tAINode* AI_EvalFunv_NETPopv(tAINode* arg0);
tAINode* AI_EvalFunc_NETServerCreate(tAINode* node);