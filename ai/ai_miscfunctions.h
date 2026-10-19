#pragma once

#include "../ai.h"

void AI_EvalFunc_GRPInvisibleSet_C(struct sGameObjectMain* a1, int a2);
void AI_EvalFunc_GRPInactiveSet_C(struct sGameObjectMain* a1, int a2);
void AI_EvalFunc_WTR_Enable_C(int a1);
void AI_EvalFunc_WTR_SetL_C(int a1, int a2);
void AI_EvalFunc_WTR_SetF_C(int a1, float a2);
void AI_EvalFunc_WTR_GetDif_C(Vec3* a1, Vec3* a2);
void AI_EvalFunc_SPG_SetNoise_C(struct sGameObjectMain* a1, float a2);
void AI_EvalFunc_SDW_Activate_C(struct sGameObjectMain* a1, int a2);
void AI_EvalFunc_SNAKE_SetInertia_C(struct sGameObjectMain* a1, float a2);
void AI_EvalFunc_Gsp_AE_MASTER_C(int a1, int a2, int a3, float a4);

tAINode* AI_EvalFunc_GRPInvisibleSet(tAINode* a1);
tAINode* AI_EvalFunc_GRPInactiveSet(tAINode* a1);
tAINode* AI_EvalFunc_Gsp_AE_MASTER(tAINode* a1);
int PC_AE_MASTER_GET_ONOFF_sub_4A37D0(int a1);
tAINode* AI_EvalFunc_Gsp_AE_MASTER_GETONOFF(tAINode* a1);
tAINode* AI_EvalFunc_Gsp_AE_MASTER_GETP1(tAINode* a1);
tAINode* AI_EvalFunc_Gsp_AE_MASTEREVT(tAINode* a1);
tAINode* AI_EvalFunc_CIN_OnOff(tAINode* a1);
tAINode* AI_EvalFunc_DBR_GetN(tAINode* a1);
tAINode* AI_EvalFunc_ConsoleSpecificOption(tAINode* a1);
tAINode* AI_EvalFunc_XMEN_OnOff(tAINode* a1);
tAINode* AI_EvalFunc_XMEN_SetSize(tAINode* a1);
tAINode* AI_EvalFunc_XMEC_OnOff(tAINode* a1);
tAINode* AI_EvalFunc_SNAKE_SetInertia(tAINode* a1);
tAINode* AI_EvalFunc_SPG_SetNoise(tAINode* a1);
tAINode* AI_EvalFunc_SDW_Activate(tAINode* a1);
tAINode* AI_EvalFunc_WTR_GetDif(tAINode* a1);
tAINode* AI_EvalFunc_WTR_GetNrm(tAINode* arg0);
tAINode* AI_EvalFunc_WTR_Enable(tAINode* a1);
tAINode* AI_EvalFunc_WTR_SetF(tAINode* a1);
tAINode* AI_EvalFunc_WTR_SetL(tAINode* a1);
tAINode* AI_EvalFunc_pop_one_elem_put_zero_sub_4B4280(tAINode* a1);
tAINode* AI_EvalFunc_WTR_put_zero_no_pop_sub_4B42C0(tAINode* a1);
