#include "ai_compiledscripts_inc.h"

//SND_LoopVolFreqCapa_lvfc_watch
int SND_LoopVolFreqCapa_lvfc_watch_univVars;
int SND_LoopVolFreqCapa_lvfc_watch_univVars_Flag_speciaux_sauve;

int SND_LoopVolFreqCapa_lvfc_watch()
{
    unsigned int v0;
    unsigned int v1;
    float v3; // [esp+4h] [ebp-64h]
    float v4; // [esp+4h] [ebp-64h]
    float v5; // [esp+Ch] [ebp-5Ch]
    int v6; // [esp+10h] [ebp-58h]
    tScriptStruct_SNDLoopVolFreqCapa* v7; // [esp+14h] [ebp-54h]
    float v8; // [esp+30h] [ebp-38h]
    float v9; // [esp+34h] [ebp-34h]
    float v10; // [esp+44h] [ebp-24h]
    int v11; // [esp+50h] [ebp-18h]
    int a2; // [esp+5Ch] [ebp-Ch]
    int a2a; // [esp+5Ch] [ebp-Ch]
    int a2b; // [esp+5Ch] [ebp-Ch]
    float v15; // [esp+64h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_SNDLoopVolFreqCapa*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SND_LoopVolFreqCapa_lvfc_watch_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &SND_LoopVolFreqCapa_lvfc_watch_univVars_Flag_speciaux_sauve,
            (void*)SND_LoopVolFreqCapa_lvfc_watch_univVars,
            4488);
    }
    if (v7->SF_ON_OFF <= 0)
    {
        v11 = 0;
    }
    else if (v7->SF_ON_OFF < 10000)
    {
        v11 = AI_EvalFunc_ARRBitTest_C(
            (int*)(SND_LoopVolFreqCapa_lvfc_watch_univVars_Flag_speciaux_sauve + 4),
            v7->SF_ON_OFF);
    }
    else
    {
        v11 = 1
            - AI_EvalFunc_ARRBitTest_C(
                (int*)(SND_LoopVolFreqCapa_lvfc_watch_univVars_Flag_speciaux_sauve + 4),
                v7->SF_ON_OFF - 10000);
    }
    if (v11)
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x100, 0);
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
    {
        if (v7->i_snd_id[0] != -1)
        {
            for (a2 = 0; a2 < v7->i_nb_son_simul; ++a2)
            {
                SND_Release(v7->i_snd_id[a2]);
                v7->i_snd_id[a2] = -1;
            }
        }
    }
    else
    {
        if (v7->i_snd_id[0] == -1)
        {
            for (a2a = 0; a2a < v7->i_nb_son_simul; ++a2a)
            {
                v7->i_snd_id[a2a] = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, a2a, 0x18000000);
                AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v7->i_snd_id[a2a]);
                v6 = SND_l_GetFreq(v7->i_snd_id[a2a]);
                *(float*)AI_EvalVarArray_C2((char*)&v7->__f_snd_freq_Dimension1, a2a) = (float)v6;
                v5 = SND_f_GetVol(v7->i_snd_id[a2a]);
                *(float*)AI_EvalVarArray_C2((char*)&v7->__f_snd_vol_Dimension1, a2a) = v5;
                v15 = AI_EvalFunc_MATHRandFloat_C(v7->f_vol_min, v7->f_vol_max);
                v3 = *(float*)AI_EvalVarArray_C2((char*)&v7->__f_snd_vol_Dimension1, a2a) * v15;
                SND_SetVol(v7->i_snd_id[a2a], v3);
                v9 = AI_EvalFunc_MATHRandFloat_C(v7->f_freq_min, v7->f_freq_max);
                v0 = (unsigned int)(*(float*)AI_EvalVarArray_C2((char*)&v7->__f_snd_freq_Dimension1, a2a) * v9);
                SND_SetFreq(v7->i_snd_id[a2a], v0);
                v7->tf_time_change = TIM_gf_MainClock - TIM_gf_LockTime;
            }
        }
        if (AI_EvalFunc_TIMEElapsed_C(v7->tf_time_change, 3.0))
        {
            v7->tf_time_change = TIM_gf_MainClock - TIM_gf_LockTime;
            for (a2b = 0; a2b < v7->i_nb_son_simul; ++a2b)
            {
                v10 = AI_EvalFunc_MATHRandFloat_C(v7->f_vol_min, v7->f_vol_max);
                v4 = *(float*)AI_EvalVarArray_C2((char*)&v7->__f_snd_vol_Dimension1, a2b) * v10;
                SND_SetVol(v7->i_snd_id[a2b], v4);
                v8 = AI_EvalFunc_MATHRandFloat_C(v7->f_freq_min, v7->f_freq_max);
                v1 = (unsigned int)(*(float*)AI_EvalVarArray_C2((char*)&v7->__f_snd_freq_Dimension1, a2b) * v8);
                SND_SetFreq(v7->i_snd_id[a2b], v1);
            }
        }
    }
    if (v11)
        AI_EvalFunc_OBJ_FlagInactiveSet_C(AI_gpst_CurrentGameObject, 1);
    return 1000;
}