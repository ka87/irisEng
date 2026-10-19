#include "ai_compiledscripts_inc.h"

//SFX_Sally_Shield_SS_C_Apparition
int fct_SFX_Sally_Shield_SS_C_Apparition_SFX_Sally_Shield_SS_C_Disparition;
int SFX_Sally_Shield_SS_C_Apparition__PJ_Sally_gao;

//SFX_Sally_Shield_SS_C_init
int fct_SFX_Sally_Shield_SS_C_init_SFX_Sally_Shield_SS_BDisplay;
int fct_SFX_Sally_Shield_SS_C_init_SFX_Sally_Shield_SS_ADisplay;
int fct_SFX_Sally_Shield_SS_C_init_SFX_Sally_Shield_SS_C_Wait;

//SFX_Sally_Shield_SS_C_Wait
int fct_SFX_Sally_Shield_SS_C_Wait_SFX_Sally_Shield_SS_C_Apparition;

//SFX_Sally_Shield_SS_C_Disparition
int fct_SFX_Sally_Shield_SS_C_Disparition_SFX_Sally_Shield_SS_C_Apparition;
int SFX_Sally_Shield_SS_C_Disparition_GST_Global_gao;
int SFX_Sally_Shield_SS_C_Disparition_GST_Global_gao_f_game_speed;
int fct_SFX_Sally_Shield_SS_C_Disparition_SFX_Sally_Shield_SS_C_Wait;

int SFX_Sally_Shield_SS_C_Apparition()
{
    float a2; // [esp+4h] [ebp-A8h]
    float v2; // [esp+8h] [ebp-A4h]
    float v3; // [esp+Ch] [ebp-A0h]
    float v4; // [esp+10h] [ebp-9Ch]
    tScriptStruct_SFXSallyShield* v5; // [esp+14h] [ebp-98h]
    float v6; // [esp+1Ch] [ebp-90h]
    int v7; // [esp+28h] [ebp-84h]
    tMessage* Src; // [esp+3Ch] [ebp-70h]
    void* v9; // [esp+48h] [ebp-64h]
    tMessage v10; // [esp+64h] [ebp-48h] BYREF
    int v11; // [esp+98h] [ebp-14h]
    tScriptStruct_SFXSallyShield* v12; // [esp+9Ch] [ebp-10h]
    float v13; // [esp+A0h] [ebp-Ch]
    BOOL v14; // [esp+A4h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_SFXSallyShield*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v12 = v5;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_Sally_Shield_SS_C_Apparition_SFX_Sally_Shield_SS_C_Disparition = (int)AI_ul_RealResolveOneRefFunction(0x26002181u);
        SFX_Sally_Shield_SS_C_Apparition__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
    }
    v12->i_after = 0;
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(1))
    {
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 3);
        v11 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_Sally_Shield_SS_C_Apparition_SFX_Sally_Shield_SS_C_Disparition);
        if (v11 != 1000)
            return v11;
    }
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x10, 0);
    v12->f_coef_opacite = v12->f_coef_opacite - 4.0f * TIM_gf_dt;
    if (v12->f_coef_opacite > 0.0)
        v4 = v12->f_coef_opacite;
    else
        v4 = 0.0;
    v12->f_coef_opacite = v4;
    v12->f_coef_noiseB = v12->f_coef_noiseB - 290.0f * TIM_gf_dt;
    if (v12->f_coef_noiseB > 0.0)
        v3 = v12->f_coef_noiseB;
    else
        v3 = 0.0;
    v12->f_coef_noiseB = v3;
    v14 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(8);
    if (v14 && AI_EvalFunc_TIMEElapsed_C(v12->f_time_noise, v12->f_delay_noise))
    {
        v6 = 20.0f * TIM_gf_dt;
        if (v6 < 0.0)
            v6 = 0.0;
        if (v6 > 1.0)
            v6 = 1.0;
        v13 = 1.0f * v6 + v12->f_coef_noiseL * (1.0f - v6);
        v12->f_coef_noiseL = v13;
        if (v12->f_coef_noiseL >= 0.98000002)
        {
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 8);
            v12->f_time_noise = TIM_gf_MainClock - TIM_gf_LockTime;
            v12->f_delay_noise = AI_EvalFunc_MATHRandFloat_C(0.050000001f, 0.15000001f);
        }
    }
    else if (v12->f_coef_noiseL != 0.0)
    {
        v12->f_coef_noiseL = v12->f_coef_noiseL - 8.0f * TIM_gf_dt;
        if (v12->f_coef_noiseL > 0.0)
            v2 = v12->f_coef_noiseL;
        else
            v2 = 0.0;
        v12->f_coef_noiseL = v2;
    }
    a2 = v12->f_coef_noiseB + v12->f_coef_noiseL;
    AI_EvalFunc_SPG_SetNoise_C(AI_gpst_CurrentGameObject, a2);
    if (AI_EvalFunc_MSGGetCount_C(AI_gpst_CurrentGameObject))
    {
        Src = (tMessage*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v10, Src, sizeof(v10));
        while (v10.msgID)
        {
            if (v10.msgID == 37)
            {
                AI_EvalFunc_OBJ_CapaSet_C_CURRENT(8, 0);
                AI_EvalFunc_SNDRequestPlayRandomFreq_C(
                    (tGameObjectMain*)SFX_Sally_Shield_SS_C_Apparition__PJ_Sally_gao,
                    57,
                    -1.0,
                    0.89999998f,
                    1.1f);
                if ((v10.Int1 & 7u) > 5)
                {
                    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x8000, 3);
                    v7 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_Sally_Shield_SS_C_Apparition_SFX_Sally_Shield_SS_C_Disparition);
                    if (v7 != 1000)
                        return v7;
                }
            }
            v9 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
            memcpy(&v10, v9, sizeof(v10));
        }
    }
    return 1000;
}

int SFX_Sally_Shield_SS_C_init()
{
    int result; // eax
    tGameObjectMain* v1; // [esp+8h] [ebp-1Ch]
    tGameObjectMain* v2; // [esp+10h] [ebp-14h]
    int v3; // [esp+18h] [ebp-Ch]

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_Sally_Shield_SS_C_init_SFX_Sally_Shield_SS_BDisplay = (int)AI_ul_RealResolveOneRefFunction(0x2600217Bu);
        fct_SFX_Sally_Shield_SS_C_init_SFX_Sally_Shield_SS_ADisplay = (int)AI_ul_RealResolveOneRefFunction(0x2600217Cu);
        fct_SFX_Sally_Shield_SS_C_init_SFX_Sally_Shield_SS_C_Wait = (int)AI_ul_RealResolveOneRefFunction(0x26002183u);
    }
    v2 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v2 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v2,
            0,
            (tAIFunction*)fct_SFX_Sally_Shield_SS_C_init_SFX_Sally_Shield_SS_BDisplay);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            0,
            (tAIFunction*)fct_SFX_Sally_Shield_SS_C_init_SFX_Sally_Shield_SS_BDisplay);
    }
    v1 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v1 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v1,
            1,
            (tAIFunction*)fct_SFX_Sally_Shield_SS_C_init_SFX_Sally_Shield_SS_ADisplay);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            1,
            (tAIFunction*)fct_SFX_Sally_Shield_SS_C_init_SFX_Sally_Shield_SS_ADisplay);
    }
    v3 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_Sally_Shield_SS_C_init_SFX_Sally_Shield_SS_C_Wait);
    if (v3 == 1000)
        result = 1000;
    else
        result = v3;
    return result;
}

int SFX_Sally_Shield_SS_BDisplay()
{
    tScriptStruct_SFXSallyShield* v1; // [esp+0h] [ebp-Ch]
    int a1; // [esp+4h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXSallyShield*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    AI_EvalFunc_LightSetRLIFactor_C(1.0);
    a1 = AI_EvalFunc_COLORBlend_C(0, 0xFFFFFFFF, v1->f_coef_opacite);
    AI_EvalFunc_LightSetRLIDestColor_C(a1);
    return 1000;
}

int SFX_Sally_Shield_SS_C_Wait()
{
    int result; // eax
    int v1; // [esp+8h] [ebp-8h]

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_Sally_Shield_SS_C_Wait_SFX_Sally_Shield_SS_C_Apparition = (int)AI_ul_RealResolveOneRefFunction(0x2600217Du);
    }
    if (!AI_EvalFunc_OBJ_CapaTest_C_CURRENT(2)
        || (AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, -1),
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0),
            v1 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_Sally_Shield_SS_C_Wait_SFX_Sally_Shield_SS_C_Apparition),
            v1 == 1000))
    {
        result = 1000;
    }
    else
    {
        result = v1;
    }
    return result;
}

int SFX_Sally_Shield_SS_C_Disparition()
{
    float v1; // [esp+4h] [ebp-34h]
    tScriptStruct_SFXSallyShield* v2; // [esp+8h] [ebp-30h]
    float v3; // [esp+10h] [ebp-28h]
    int v4; // [esp+14h] [ebp-24h]
    float v5; // [esp+18h] [ebp-20h]
    int v6; // [esp+2Ch] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SFXSallyShield*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_Sally_Shield_SS_C_Disparition_SFX_Sally_Shield_SS_C_Apparition = (int)AI_ul_RealResolveOneRefFunction(0x2600217Du);
        SFX_Sally_Shield_SS_C_Disparition_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &SFX_Sally_Shield_SS_C_Disparition_GST_Global_gao_f_game_speed,
            (void*)SFX_Sally_Shield_SS_C_Disparition_GST_Global_gao,
            3548);
        fct_SFX_Sally_Shield_SS_C_Disparition_SFX_Sally_Shield_SS_C_Wait = (int)AI_ul_RealResolveOneRefFunction(0x26002183u);
    }
    v2->i_after = 0;
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(2))
    {
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 2);
        AI_EvalFunc_MSGClear_C(AI_gpst_CurrentGameObject);
        v6 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_Sally_Shield_SS_C_Disparition_SFX_Sally_Shield_SS_C_Apparition);
        if (v6 != 1000)
            return v6;
    }
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x10);
    AI_EvalFunc_OBJ_CapaTest_C_CURRENT(4);
    v3 = 100.0f / *(float*)SFX_Sally_Shield_SS_C_Disparition_GST_Global_gao_f_game_speed;
    v5 = 1.5f / *(float*)SFX_Sally_Shield_SS_C_Disparition_GST_Global_gao_f_game_speed;
    v2->f_coef_opacite = v5 * TIM_gf_dt + v2->f_coef_opacite;
    if (v2->f_coef_opacite >= 1.0)
    {
        v2->f_coef_noiseB = 100.0;
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        v4 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_Sally_Shield_SS_C_Disparition_SFX_Sally_Shield_SS_C_Wait);
        if (v4 != 1000)
            return v4;
    }
    if (v2->f_coef_opacite < 1.0)
        v1 = v2->f_coef_opacite;
    else
        v1 = 1.0;
    v2->f_coef_opacite = v1;
    v2->f_coef_noiseB = v3 * TIM_gf_dt + v2->f_coef_noiseB;
    AI_EvalFunc_SPG_SetNoise_C(AI_gpst_CurrentGameObject, v2->f_coef_noiseB);
    return 1000;
}

int SFX_Sally_Shield_SS_ADisplay()
{
    tScriptStruct_SFXEclaire* v1; // [esp+4h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXEclaire*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    AI_EvalFunc_LightSetRLIFactor_C(v1->old_factor);
    AI_EvalFunc_LightSetRLIDestColor_C(v1->old_color);
    return 1000;
}