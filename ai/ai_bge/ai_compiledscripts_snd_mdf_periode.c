#include "ai_compiledscripts_inc.h"

//SND_MDF_periode_snd_init
int fct_SND_MDF_periode_snd_init_SND_MDF_periode_snd_mp;

//SND_MDF_periode_snd_mp
int SND_MDF_periode_snd_mp_univVars;
int SND_MDF_periode_snd_mp_univVars_Planetes_CurHour;
int SND_MDF_periode_snd_mp_PJ_Sally_gao;

int SND_MDF_periode_snd_init()
{
    tScriptStruct_SNDMDFPeriode* v1; // [esp+0h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SNDMDFPeriode*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SND_MDF_periode_snd_init_SND_MDF_periode_snd_mp = (int)AI_ul_RealResolveOneRefFunction(0xBD00017B);
    }
    v1->f_far = v1->f_far * v1->f_far;
    AI_EvalFunc_AITrackChange_C(2, (tAIFunction*)fct_SND_MDF_periode_snd_init_SND_MDF_periode_snd_mp);
    return 1000;
}

int SND_MDF_periode_snd_mp()
{
    double v0; // st7
    tScriptStruct_SNDMDFPeriode* v2; // [esp+0h] [ebp-18h]
    tGameObjectMain* v3; // [esp+8h] [ebp-10h]
    float v4; // [esp+Ch] [ebp-Ch]
    int v5; // [esp+14h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SNDMDFPeriode*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SND_MDF_periode_snd_mp_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&SND_MDF_periode_snd_mp_univVars_Planetes_CurHour, (void*)SND_MDF_periode_snd_mp_univVars, 6116);
        SND_MDF_periode_snd_mp_PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
    }
    if (*(float*)SND_MDF_periode_snd_mp_univVars_Planetes_CurHour <= (double)v2->f_heure_debjour
        || v2->f_heure_finjour <= (double)*(float*)SND_MDF_periode_snd_mp_univVars_Planetes_CurHour)
    {
        v5 = v2->i_inveser_nuitjour;
    }
    else
    {
        v5 = 1 - v2->i_inveser_nuitjour;
    }
    if (v5)
    {
        v3 = (tGameObjectMain*)SND_MDF_periode_snd_mp_PJ_Sally_gao;
        if (SND_MDF_periode_snd_mp_PJ_Sally_gao)
        {
            if ((unsigned int)SND_MDF_periode_snd_mp_PJ_Sally_gao <= 2)
                v3 = AI_MainActor[SND_MDF_periode_snd_mp_PJ_Sally_gao - 1];
            v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v3);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
        }
        v4 = (float)v0;
        if (v2->f_far <= (double)v4)
        {
            if (v2->i_playing)
                AI_EvalFunc_SNDStopModifier_C(AI_gpst_CurrentGameObject, 0);
            v2->i_playing = 0;
        }
        else
        {
            if (!v2->i_playing)
                AI_EvalFunc_SNDPlayModifier_C(AI_gpst_CurrentGameObject, 0);
            v2->i_playing = 1;
        }
    }
    else if (v2->i_playing)
    {
        AI_EvalFunc_SNDStopModifier_C(AI_gpst_CurrentGameObject, 0);
        v2->i_playing = 0;
    }
    return 1000;
}