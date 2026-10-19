#include "ai_compiledscripts_inc.h"

//Random_Track_Player_rtp_init
int fct_Random_Track_Player_rtp_init_Random_Track_Player_trp_play;

//Random_Track_Player_trp_play
int Random_Track_Player_trp_play_univVars;
int Random_Track_Player_trp_play_univVars_Planetes_CurHour;

int Random_Track_Player_rtp_init()
{
    tScriptStruct_RandomTrackPlayer* v1; // [esp+0h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_RandomTrackPlayer*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Random_Track_Player_rtp_init_Random_Track_Player_trp_play = (int)AI_ul_RealResolveOneRefFunction(0x9E005B23);
    }
    AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 0x2000000);
    v1->i_track_crt = 0;
    v1->i_track_last = 0;
    v1->f_time_fin = TIM_gf_MainClock - TIM_gf_LockTime;
    v1->f_time_pause = 0.0;
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Random_Track_Player_rtp_init_Random_Track_Player_trp_play);
    return 1000;
}

int Random_Track_Player_trp_play()
{
    tScriptStruct_RandomTrackPlayer* v1; // [esp+8h] [ebp-20h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_RandomTrackPlayer*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Random_Track_Player_trp_play_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Random_Track_Player_trp_play_univVars_Planetes_CurHour,
            (void*)Random_Track_Player_trp_play_univVars,
            6116);
    }
    if (AI_EvalFunc_TRAIsStop_C(AI_gpst_CurrentGameObject, v1->i_track_crt))
    {
        if (v1->f_heure_deb <= (double)*(float*)Random_Track_Player_trp_play_univVars_Planetes_CurHour
            && v1->f_heure_fin > (double)*(float*)Random_Track_Player_trp_play_univVars_Planetes_CurHour
            && AI_EvalFunc_TIMEElapsed_C(v1->f_time_fin, v1->f_time_pause))
        {
            v1->f_time_pause = AI_EvalFunc_MATHRandFloat_C(v1->f_temps_pause_min, v1->f_temps_pause_Max);
            v1->i_track_crt = AI_EvalFunc_MATHRandInt_C(0, v1->i_nb_track);
            if (v1->i_track_crt == v1->i_track_last)
                v1->i_track_crt = AI_EvalFunc_MTHModulo_C(v1->i_track_crt + 1, v1->i_nb_track);
            v1->i_track_last = v1->i_track_crt;
            AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, v1->i_track_crt);
        }
    }
    else
    {
        v1->f_time_fin = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    return 1000;
}