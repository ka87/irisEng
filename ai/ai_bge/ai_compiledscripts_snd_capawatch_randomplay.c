#include "ai_compiledscripts_inc.h"

int SND_CapaWatch_RandomPlay_cwrp_watch()
{
    BOOL v0; // eax
    int result; // eax
    BOOL v2; // eax
    int v3; // eax
    tScriptStruct_SNDCapaWatchRandomPlay* v4; // [esp+8h] [ebp-3Ch]
    tGameObjectMain* v5; // [esp+10h] [ebp-34h]
    tGameObjectMain* v6; // [esp+18h] [ebp-2Ch]
    float v7; // [esp+18h] [ebp-2Ch]
    tGameObjectMain* v8; // [esp+1Ch] [ebp-28h]
    tGameObjectMain* v9; // [esp+20h] [ebp-24h]
    float v10; // [esp+24h] [ebp-20h]
    float v11; // [esp+30h] [ebp-14h]
    int a1; // [esp+40h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_SNDCapaWatchRandomPlay*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v9 = (tGameObjectMain*)v4->o_cine_actor;
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
    if (!v8)
        goto LABEL_17;
    v6 = (tGameObjectMain*)v4->o_cine_actor;
    if (v6)
    {
        if ((unsigned int)v6 <= 2)
            v6 = AI_MainActor[(uint32_t)v6 - 1];
        v0 = AI_EvalFunc_OBJ_CapaTest_C(v6, 0x100);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x100);
    }
    if (v0)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
    LABEL_17:
        v5 = (tGameObjectMain*)v4->o_watch_actor;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            v2 = AI_EvalFunc_OBJ_CapaTest_C(v5, v4->i_watch_capa);
        }
        else
        {
            v2 = AI_EvalFunc_OBJ_CapaTest_C(0, v4->i_watch_capa);
        }
        if (v2)
        {
            v3 = AI_EvalFunc_MATHRandInt_C(0, v4->i_snd_nb_smd);
            a1 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(
                AI_gpst_CurrentGameObject,
                v3,
                -1.0,
                v4->f_snd_min_freq,
                v4->f_snd_max_freq);
            v10 = SND_f_GetVol(a1);
            v11 = AI_EvalFunc_MATHRandFloat_C(v4->f_snd_min_vol, v4->f_snd_max_vol);
            v7 = v10 * v11;
            SND_SetVol(a1, v7);
        }
        result = 1000;
    }
    return result;
}