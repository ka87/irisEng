#include "ai_compiledscripts_inc.h"

//SFX_RedFlash_RF_comportement
int SFX_RedFlash_RF_comportement_GST_Global_gao;
int SFX_RedFlash_RF_comportement_GST_Global_gao_i_fade_priority;
int SFX_RedFlash_RF_comportement_GST_Global_gao_f_fade_start_time;
int SFX_RedFlash_RF_comportement_GST_Global_gao_f_fade_delay;
int SFX_RedFlash_RF_comportement_univVars;
int SFX_RedFlash_RF_comportement_univVars_i_fade_fx;
int SFX_RedFlash_RF_comportement_INV_MatFade_gao;

//SFX_RedFlash_RF_init
int fct_SFX_RedFlash_RF_init_SFX_RedFlash_RF_comportement;

int SFX_RedFlash_RF_comportement()
{
    BOOL v0; // eax
    tScriptStruct_SFXRedFlash* v2; // [esp+8h] [ebp-74h]
    tGameObjectMain* v3; // [esp+10h] [ebp-6Ch]
    tGameObjectMain* v4; // [esp+14h] [ebp-68h]
    tGameObjectMain* v5; // [esp+18h] [ebp-64h]
    tGameObjectMain* v6; // [esp+20h] [ebp-5Ch]
    tGameObjectMain* v7; // [esp+28h] [ebp-54h]
    tGameObjectMain* v8; // [esp+2Ch] [ebp-50h]
    tGameObjectMain* v9; // [esp+30h] [ebp-4Ch]
    tGameObjectMain* v10; // [esp+38h] [ebp-44h]
    tGameObjectMain* v11; // [esp+3Ch] [ebp-40h]
    tGameObjectMain* v12; // [esp+40h] [ebp-3Ch]
    tGameObjectMain* v13; // [esp+48h] [ebp-34h]
    int v14; // [esp+70h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SFXRedFlash*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SFX_RedFlash_RF_comportement_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &SFX_RedFlash_RF_comportement_GST_Global_gao_i_fade_priority,
            (void*)SFX_RedFlash_RF_comportement_GST_Global_gao,
            3268);
        AI2C_ResolveBuffer(
            &SFX_RedFlash_RF_comportement_GST_Global_gao_f_fade_start_time,
            (void*)SFX_RedFlash_RF_comportement_GST_Global_gao,
            3260);
        AI2C_ResolveBuffer(
            &SFX_RedFlash_RF_comportement_GST_Global_gao_f_fade_delay,
            (void*)SFX_RedFlash_RF_comportement_GST_Global_gao,
            3264);
        SFX_RedFlash_RF_comportement_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &SFX_RedFlash_RF_comportement_univVars_i_fade_fx,
            (void*)SFX_RedFlash_RF_comportement_univVars,
            20400);
        SFX_RedFlash_RF_comportement_INV_MatFade_gao = AI_ul_RealResolveOneRefObject(0x5E006934u);
    }
    v13 = (tGameObjectMain*)v2->des_o_object;
    if (v2->des_o_object)
    {
        if ((unsigned int)v13 <= 2)
            v13 = AI_MainActor[(uint32_t)v13 - 1];
        v0 = AI_EvalFunc_OBJ_CapaTest_C(v13, v2->des_i_capa);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_CapaTest_C(0, v2->des_i_capa);
    }
    if (v0)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v2->f_time_alarme, 3.0))
        {
            if (v2->i_mdf_play)
            {
                v2->i_mdf_play = 0;
                AI_EvalFunc_SNDStopModifier_C(AI_gpst_CurrentGameObject, -1);
                v12 = (tGameObjectMain*)v2->o_alarme_distante;
                if (v12)
                {
                    if ((unsigned int)v12 <= 2)
                        v12 = AI_MainActor[(uint32_t)v12 - 1];
                    v11 = v12;
                }
                else
                {
                    v11 = 0;
                }
                if (v11)
                {
                    v10 = (tGameObjectMain*)v2->o_alarme_distante;
                    if (v10)
                    {
                        if ((unsigned int)v10 <= 2)
                            v10 = AI_MainActor[(uint32_t)v10 - 1];
                        AI_EvalFunc_SNDStopModifier_C(v10, 0);
                    }
                    else
                    {
                        AI_EvalFunc_SNDStopModifier_C(0, 0);
                    }
                }
            }
            if (AI_EvalFunc_TIMEElapsed_C(v2->f_time_alarme, 13.0))
                v2->f_time_alarme = TIM_gf_MainClock - TIM_gf_LockTime;
        }
        else if (!v2->i_mdf_play)
        {
            v2->i_mdf_play = 1;
            AI_EvalFunc_SNDPlayModifier_C(AI_gpst_CurrentGameObject, -1);
            v9 = (tGameObjectMain*)v2->o_alarme_distante;
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                v8 = v9;
            }
            else
            {
                v8 = 0;
            }
            if (v8)
            {
                v7 = (tGameObjectMain*)v2->o_alarme_distante;
                if (v7)
                {
                    if ((unsigned int)v7 <= 2)
                        v7 = AI_MainActor[(uint32_t)v7 - 1];
                    AI_EvalFunc_SNDPlayModifier_C(v7, 0);
                }
                else
                {
                    AI_EvalFunc_SNDPlayModifier_C(0, 0);
                }
            }
        }
        if (AI_EvalFunc_TIMEElapsed_C(v2->f_time_start_fade, 1.0))
        {
            if (*(int*)SFX_RedFlash_RF_comportement_GST_Global_gao_i_fade_priority <= 100
                || AI_EvalFunc_TIMEElapsed_C(
                    *(float*)SFX_RedFlash_RF_comportement_GST_Global_gao_f_fade_start_time,
                    *(float*)SFX_RedFlash_RF_comportement_GST_Global_gao_f_fade_delay))
            {
                *(float*)SFX_RedFlash_RF_comportement_GST_Global_gao_f_fade_start_time = TIM_gf_MainClock - TIM_gf_LockTime;
                *(uint32_t*)SFX_RedFlash_RF_comportement_GST_Global_gao_f_fade_delay = 0x3F028F5C;
                *(uint32_t*)SFX_RedFlash_RF_comportement_GST_Global_gao_i_fade_priority = 100;
                AI_EvalFunc_GFXDel_C_CURRENT(10);
                v14 = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
                *(uint32_t*)SFX_RedFlash_RF_comportement_univVars_i_fade_fx = v14;
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v14, 0, 1);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v14, 2, 1);
                v6 = (tGameObjectMain*)SFX_RedFlash_RF_comportement_INV_MatFade_gao;
                if (SFX_RedFlash_RF_comportement_INV_MatFade_gao)
                {
                    if ((unsigned int)SFX_RedFlash_RF_comportement_INV_MatFade_gao <= 2)
                        v6 = AI_MainActor[SFX_RedFlash_RF_comportement_INV_MatFade_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v14, v6, 9);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v14, 0, 9);
                }
                GFXSetCurGlobal(v14);
                AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0);
                AI_EvalFunc_GFXSeti_C_CURGLOB(11101, 0x400000FF);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 0.20999999f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 0.1f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.2f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
            }
            else
            {
                v14 = -1;
            }
            if (v14 != -1)
                v2->f_time_start_fade = TIM_gf_MainClock - TIM_gf_LockTime;
        }
    }
    else
    {
        if (v2->i_mdf_play)
        {
            v2->i_mdf_play = 0;
            AI_EvalFunc_SNDStopModifier_C(AI_gpst_CurrentGameObject, -1);
            v5 = (tGameObjectMain*)v2->o_alarme_distante;
            if (v5)
            {
                if ((unsigned int)v5 <= 2)
                    v5 = AI_MainActor[(uint32_t)v5 - 1];
                v4 = v5;
            }
            else
            {
                v4 = 0;
            }
            if (v4)
            {
                v3 = (tGameObjectMain*)v2->o_alarme_distante;
                if (v3)
                {
                    if ((unsigned int)v3 <= 2)
                        v3 = AI_MainActor[(uint32_t)v3 - 1];
                    AI_EvalFunc_SNDStopModifier_C(v3, 0);
                }
                else
                {
                    AI_EvalFunc_SNDStopModifier_C(0, 0);
                }
            }
        }
        v2->f_time_alarme = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    return 1000;
}

int SFX_RedFlash_RF_init()
{
    int result; // eax
    int v1; // [esp+4h] [ebp-8h]

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_RedFlash_RF_init_SFX_RedFlash_RF_comportement = (int)AI_ul_RealResolveOneRefFunction(0x72000800u);
    }
    v1 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_RedFlash_RF_init_SFX_RedFlash_RF_comportement);
    if (v1 == 1000)
        result = 1000;
    else
        result = v1;
    return result;
}