#include "ai_compiledscripts_inc.h"

//Grillon_Cigales_gc_init
int fct_Grillon_Cigales_gc_init_Grillon_Cigales_gc_play;

//Grillon_Cigales_gc_play
int Grillon_Cigales_gc_play_univVars;
int Grillon_Cigales_gc_play_univVars_Planetes_CurHour;

int Grillon_Cigales_gc_init()
{
    tScriptStruct_GrillonCigales* v1; // [esp+0h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GrillonCigales*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Grillon_Cigales_gc_init_Grillon_Cigales_gc_play = (int)AI_ul_RealResolveOneRefFunction(0x9E005B48);
    }
    v1->f_time_crt = TIM_gf_MainClock - TIM_gf_LockTime;
    v1->f_time_total = 0.0;
    v1->i_SND = -1;
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Grillon_Cigales_gc_init_Grillon_Cigales_gc_play);
    return 1000;
}

int Grillon_Cigales_gc_play()
{
    float* v1; // eax
    float* v2; // eax
    float v3; // [esp+4h] [ebp-38h]
    float v4; // [esp+4h] [ebp-38h]
    float v5; // [esp+8h] [ebp-34h]
    float v6; // [esp+Ch] [ebp-30h]
    tScriptStruct_GrillonCigales* v7; // [esp+14h] [ebp-28h]
    int a2; // [esp+34h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_GrillonCigales*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Grillon_Cigales_gc_play_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Grillon_Cigales_gc_play_univVars_Planetes_CurHour,
            (void*)Grillon_Cigales_gc_play_univVars,
            6116);
    }
    switch ((unsigned int)*AI_gppst_CurrentJumpNode)
    {
    case 1u:
        SND_StopRq(v7->i_SND);
        *AI_gppst_CurrentJumpNode = (tAINode*)2;
        TIME_Wait_C(v7->f_time_pause_1);
        return 0;
    case 2u:
        AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v7->i_SND);
        *AI_gppst_CurrentJumpNode = (tAINode*)3;
        TIME_Wait_C(v7->f_time_boucle_2);
        return 0;
    case 3u:
        SND_StopRq(v7->i_SND);
        SND_Release(v7->i_SND);
        v7->i_SND = -1;
        *AI_gppst_CurrentJumpNode = (tAINode*)4;
        TIME_Wait_C(v7->f_time_pause_2);
        return 0;
    case 4u:
        return 1000;
    default:
        if (!AI_EvalFunc_TIMEElapsed_C(v7->f_time_crt, v7->f_time_total) && v7->i_SND != -1)
        {
            AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v7->i_SND);
            *AI_gppst_CurrentJumpNode = (tAINode*)1;
            TIME_Wait_C(v7->f_time_boucle_1);
            return 0;
        }
        v7->i_ind_snd = -1;
        a2 = 0;
        break;
    }
    while (a2 < 4)
    {
        if (*(float*)AI_EvalVarArray_C2((char*)v7, a2) <= (double)*(float*)Grillon_Cigales_gc_play_univVars_Planetes_CurHour
            && *(float*)AI_EvalVarArray_C2((char*)&v7->__af_heure_fin_Dimension1, a2) > (double)*(float*)Grillon_Cigales_gc_play_univVars_Planetes_CurHour)
        {
            v7->i_ind_snd = a2;
            break;
        }
        ++a2;
    }
    if (v7->i_ind_snd != -1)
    {
        v6 = (float)(int)AI_EvalFunc_MATHRandInt_C(v7->ai_nb_min[v7->i_ind_snd], v7->ai_nb_max[v7->i_ind_snd] + 1);
        v7->f_time_boucle_1 = *(float*)AI_EvalVarArray_C2((char*)&v7->__af_duree_Dimension1, v7->i_ind_snd) * v6;
        v3 = *(float*)AI_EvalVarArray_C2((char*)&v7->__af_pause_max_Dimension1, v7->i_ind_snd);
        v1 = (float*)AI_EvalVarArray_C2((char*)&v7->__af_pause_min_Dimension1, v7->i_ind_snd);
        v7->f_time_pause_1 = AI_EvalFunc_MATHRandFloat_C(*v1, v3);
        v5 = (float)(int)AI_EvalFunc_MATHRandInt_C(v7->ai_nb_bis_min[v7->i_ind_snd], v7->ai_nb_bis_max[v7->i_ind_snd] + 1);
        v7->f_time_boucle_2 = *(float*)AI_EvalVarArray_C2((char*)&v7->__af_duree_Dimension1, v7->i_ind_snd) * v5;
        v4 = *(float*)AI_EvalVarArray_C2((char*)&v7->__af_pause_bis_max_Dimension1, v7->i_ind_snd);
        v2 = (float*)AI_EvalVarArray_C2((char*)&v7->__af_pause_bis_min_Dimension1, v7->i_ind_snd);
        v7->f_time_pause_2 = AI_EvalFunc_MATHRandFloat_C(*v2, v4);
        v7->f_time_total = v7->f_time_boucle_1 + v7->f_time_pause_1 + v7->f_time_boucle_2 + v7->f_time_pause_2;
        v7->f_time_crt = TIM_gf_MainClock - TIM_gf_LockTime;
        v7->i_SND = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, v7->i_ind_snd, 402653184);
    }
    return 1000;
}