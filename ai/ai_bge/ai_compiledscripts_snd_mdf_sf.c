#include "ai_compiledscripts_inc.h"

//Snd_mdf_SF_snd_mdf_play
int Snd_mdf_SF_snd_mdf_play_univVars;
int Snd_mdf_SF_snd_mdf_play_univVars_Flag_speciaux_sauve;
int fct_Snd_mdf_SF_snd_mdf_play_Snd_mdf_SF_snd_mdf_stop;

//Snd_mdf_SF_snd_mdf_init
int Snd_mdf_SF_snd_mdf_init_univVars;
int Snd_mdf_SF_snd_mdf_init_univVars_Flag_speciaux_sauve;
int fct_Snd_mdf_SF_snd_mdf_init_Snd_mdf_SF_snd_mdf_play;
int fct_Snd_mdf_SF_snd_mdf_init_Snd_mdf_SF_snd_mdf_stop;

//Snd_mdf_SF_snd_mdf_stop
int Snd_mdf_SF_snd_mdf_stop_univVars;
int Snd_mdf_SF_snd_mdf_stop_univVars_Flag_speciaux_sauve;
int fct_Snd_mdf_SF_snd_mdf_stop_Snd_mdf_SF_snd_mdf_play;

int Snd_mdf_SF_snd_mdf_play()
{
    tScriptStruct_SNDMDFSF* v1; // [esp+0h] [ebp-14h]
    int v2; // [esp+10h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SNDMDFSF*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Snd_mdf_SF_snd_mdf_play_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Snd_mdf_SF_snd_mdf_play_univVars_Flag_speciaux_sauve,
            (void*)Snd_mdf_SF_snd_mdf_play_univVars,
            4488);
        fct_Snd_mdf_SF_snd_mdf_play_Snd_mdf_SF_snd_mdf_stop = (int)AI_ul_RealResolveOneRefFunction(0x26000233u);
    }
    if (v1->SF_a_tester <= 0)
        v2 = 0;
    else
        v2 = AI_EvalFunc_ARRBitTest_C((int*)(Snd_mdf_SF_snd_mdf_play_univVars_Flag_speciaux_sauve + 4), v1->SF_a_tester);
    if (v2 != v1->i_son_qd_SF_a_vrai)
    {
        AI_EvalFunc_SNDStopModifier_C(AI_gpst_CurrentGameObject, -1);
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Snd_mdf_SF_snd_mdf_play_Snd_mdf_SF_snd_mdf_stop);
    }
    return 1000;
}

int Snd_mdf_SF_snd_mdf_init()
{
    tScriptStruct_SNDMDFSF* v1; // [esp+0h] [ebp-24h]
    int v2; // [esp+14h] [ebp-10h]
    int v3; // [esp+20h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SNDMDFSF*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Snd_mdf_SF_snd_mdf_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Snd_mdf_SF_snd_mdf_init_univVars_Flag_speciaux_sauve,
            (void*)Snd_mdf_SF_snd_mdf_init_univVars,
            4488);
        fct_Snd_mdf_SF_snd_mdf_init_Snd_mdf_SF_snd_mdf_play = (int)AI_ul_RealResolveOneRefFunction(0x26000232u);
        fct_Snd_mdf_SF_snd_mdf_init_Snd_mdf_SF_snd_mdf_stop = (int)AI_ul_RealResolveOneRefFunction(0x26000233u);
    }
    if (v1->SF_destruction)
    {
        if (v1->SF_destruction <= 0)
            v3 = 0;
        else
            v3 = v1->SF_destruction < 10000 ? AI_EvalFunc_ARRBitTest_C(
                (int*)(Snd_mdf_SF_snd_mdf_init_univVars_Flag_speciaux_sauve + 4),
                v1->SF_destruction) : 1
            - AI_EvalFunc_ARRBitTest_C(
                (int*)(Snd_mdf_SF_snd_mdf_init_univVars_Flag_speciaux_sauve
                    + 4),
                v1->SF_destruction - 10000);
        if (v3)
        {
            v2 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v2 != 1000)
                return v2;
        }
    }
    if ((AI_EvalFunc_SNDGetStatusModifier_C(AI_gpst_CurrentGameObject, -1) & 1) != 0)
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Snd_mdf_SF_snd_mdf_init_Snd_mdf_SF_snd_mdf_play);
    else
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Snd_mdf_SF_snd_mdf_init_Snd_mdf_SF_snd_mdf_stop);
    return 1000;
}

int Snd_mdf_SF_snd_mdf_stop()
{
    tScriptStruct_SNDMDFSF* v1; // [esp+0h] [ebp-14h]
    int v2; // [esp+10h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SNDMDFSF*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Snd_mdf_SF_snd_mdf_stop_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Snd_mdf_SF_snd_mdf_stop_univVars_Flag_speciaux_sauve,
            (void*)Snd_mdf_SF_snd_mdf_stop_univVars,
            4488);
        fct_Snd_mdf_SF_snd_mdf_stop_Snd_mdf_SF_snd_mdf_play = (int)AI_ul_RealResolveOneRefFunction(0x26000232u);
    }
    if (v1->SF_a_tester <= 0)
        v2 = 0;
    else
        v2 = AI_EvalFunc_ARRBitTest_C((int*)(Snd_mdf_SF_snd_mdf_stop_univVars_Flag_speciaux_sauve + 4), v1->SF_a_tester);
    if (v2 == v1->i_son_qd_SF_a_vrai)
    {
        AI_EvalFunc_SNDPlayModifier_C(AI_gpst_CurrentGameObject, -1);
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Snd_mdf_SF_snd_mdf_stop_Snd_mdf_SF_snd_mdf_play);
    }
    return 1000;
}