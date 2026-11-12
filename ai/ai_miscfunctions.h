#pragma once

#include "../ai.h"

void AI_EvalFunc_GRPInvisibleSet_C(struct sGameObjectMain* gao, int a2);
void AI_EvalFunc_GRPInactiveSet_C(struct sGameObjectMain* gao, int a2);
void AI_EvalFunc_WTR_Enable_C(int a1);
void AI_EvalFunc_WTR_SetL_C(int a1, int a2);
void AI_EvalFunc_WTR_SetF_C(int a1, float a2);
void AI_EvalFunc_WTR_GetDif_C(Vec3* a1, Vec3* a2);
void AI_EvalFunc_SPG_SetNoise_C(struct sGameObjectMain* gao, float a2);
void AI_EvalFunc_SDW_Activate_C(struct sGameObjectMain* gao, int a2);
void AI_EvalFunc_SNAKE_SetInertia_C(struct sGameObjectMain* gao, float a2);
void AI_EvalFunc_Gsp_AE_MASTER_C(int a1, int a2, int a3, float a4);
void AI_EvalFunc_CIN_CutsceneMode_C(int a1);

tAINode* AI_EvalFunc_GRPInvisibleSet(tAINode* node);
tAINode* AI_EvalFunc_GRPInactiveSet(tAINode* node);
tAINode* AI_EvalFunc_Gsp_AE_MASTER(tAINode* node);
int PC_AE_MASTER_GET_ONOFF_sub_4A37D0(int a1);
tAINode* AI_EvalFunc_Gsp_AE_MASTER_GETONOFF(tAINode* node);
tAINode* AI_EvalFunc_Gsp_AE_MASTER_GETP1(tAINode* node);
tAINode* AI_EvalFunc_Gsp_AE_MASTEREVT(tAINode* node);
tAINode* AI_EvalFunc_CIN_OnOff(tAINode* node);
tAINode* AI_EvalFunc_CIN_CutsceneMode(tAINode* node);
tAINode* AI_EvalFunc_DBR_GetN(tAINode* node);
tAINode* AI_EvalFunc_ConsoleSpecificOption(tAINode* node);
tAINode* AI_EvalFunc_XMEN_OnOff(tAINode* node);
tAINode* AI_EvalFunc_XMEN_SetSize(tAINode* node);
tAINode* AI_EvalFunc_XMEC_OnOff(tAINode* node);
tAINode* AI_EvalFunc_SNAKE_SetInertia(tAINode* node);
tAINode* AI_EvalFunc_SPG_SetNoise(tAINode* node);
tAINode* AI_EvalFunc_SDW_Activate(tAINode* node);
tAINode* AI_EvalFunc_WTR_GetDif(tAINode* node);
tAINode* AI_EvalFunc_WTR_GetNrm(tAINode* arg0);
tAINode* AI_EvalFunc_WTR_Enable(tAINode* node);
tAINode* AI_EvalFunc_WTR_SetF(tAINode* node);
tAINode* AI_EvalFunc_WTR_SetL(tAINode* node);
tAINode* AI_EvalFunc_pop_one_elem_put_zero_sub_4B4280(tAINode* node);
tAINode* AI_EvalFunc_WTR_put_zero_no_pop_sub_4B42C0(tAINode* node);
int AI_EvalFunc_BGE_PC_Price_Check_C(int a1, int a2);
int AI_EvalFunc_BGE_PC_Samsue_Check_C();
int AI_EvalFunc_BGE_PC_Radio_Check_C();
int AI_EvalFunc_BGE_PC_Robot_Check_C();
tAINode* AI_EvalFunc_BGE_PC_Price_Compute(tAINode* node);
tAINode* AI_EvalFunc_BGE_PC_Price_Check(tAINode* node);
tAINode* AI_EvalFunc_BGE_PC_Samsue_Compute(tAINode* node);
tAINode* AI_EvalFunc_BGE_PC_Samsue_Check(tAINode* node);
tAINode* AI_EvalFunc_BGE_PC_Radio_Compute(tAINode* node);
tAINode* AI_EvalFunc_BGE_PC_Radio_Check(tAINode* node);
tAINode* AI_EvalFunc_BGE_PC_Robot_Compute(tAINode* node);
tAINode* AI_EvalFunc_BGE_PC_Robot_Check(tAINode* node);
