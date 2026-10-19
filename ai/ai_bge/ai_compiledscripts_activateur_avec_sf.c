#include "ai_compiledscripts_inc.h"

//Activateur_avec_SF_Acs_init
int fct_Activateur_avec_SF_Acs_init_Activateur_avec_SF_Acs_loop;

int Activateur_avec_SF_Acs_init()
{
    int result; // eax
    tScriptStruct_ActivateurAvecSF* v1; // [esp+0h] [ebp-38h]
    int v6; // [esp+28h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ActivateurAvecSF*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Activateur_avec_SF_Acs_init_Activateur_avec_SF_Acs_loop = (int)AI_ul_RealResolveOneRefFunction(0x3500A7A4u);
    }
    
    if (!AI2C_CheckActorGao((tGameObjectMain*)v1->des_o_acteur))
        v1->des_o_acteur = (int)AI_gpst_CurrentGameObject;

    v1->f_temps = TIM_gf_MainClock - TIM_gf_LockTime;
    if (AI_EvalFunc_ARRGlobalIntFlagTest_C(v1->des_i_SpFlag) == v1->des_i_SpFlag_etat)
    {
        if (v1->des_i_control_flags >= 0)
        {
            AI_EvalFunc_OBJ_FlagsControlSet_C(AI2C_CheckActorGao((tGameObjectMain*)v1->des_o_acteur), 0, v1->des_i_control_flags);            
        }
        else
        {
            AI_EvalFunc_OBJ_FlagsControlSet_C(AI2C_CheckActorGao((tGameObjectMain*)v1->des_o_acteur), -v1->des_i_control_flags, 0);            
        }
        AI_EvalFunc_OBJ_FlagInactiveSet_C(AI_gpst_CurrentGameObject, 1);
    }
    v6 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Activateur_avec_SF_Acs_init_Activateur_avec_SF_Acs_loop);
    if (v6 == 1000)
        result = 1000;
    else
        result = v6;
    return result;
}

int Activateur_avec_SF_Acs_loop()
{
    int result; // eax
    tScriptStruct_ActivateurAvecSF* v1; // [esp+8h] [ebp-2Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ActivateurAvecSF*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (AI_EvalFunc_TIMEElapsed_C(v1->f_temps, v1->des_f_frequence_test))
    {
        v1->f_temps = TIM_gf_MainClock - TIM_gf_LockTime;
        if (AI_EvalFunc_ARRGlobalIntFlagTest_C(v1->des_i_SpFlag) == v1->des_i_SpFlag_etat)
        {
            if (v1->des_i_control_flags >= 0)
            {
                AI_EvalFunc_OBJ_FlagsControlSet_C(AI2C_CheckActorGao((tGameObjectMain*)v1->des_o_acteur), 0, v1->des_i_control_flags);                
            }
            else
            {
                AI_EvalFunc_OBJ_FlagsControlSet_C(AI2C_CheckActorGao((tGameObjectMain*)v1->des_o_acteur), -v1->des_i_control_flags, 0);             
            }
            AI_EvalFunc_OBJ_FlagInactiveSet_C(AI_gpst_CurrentGameObject, 1);
        }
        result = 1000;
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}