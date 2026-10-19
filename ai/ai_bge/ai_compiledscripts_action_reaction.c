#include "ai_compiledscripts_inc.h"

//action_reaction_ar_init
int action_reaction_ar_init__PJ_Sally_gao;
int action_reaction_ar_init_univVars;
int action_reaction_ar_init_univVars_Flag_speciaux_sauve;
int fct_action_reaction_ar_init_action_reaction_ar_action;

//action_reaction_ar_check
int fct_action_reaction_ar_check_action_reaction_ar_exec_send_info;

//action_reaction_ar_exec_send_info
int action_reaction_ar_exec_send_info_TXT_Sally_gao;

//action_reaction_ar_action
int action_reaction_ar_action_GST_Fight_Manager_gao;
int action_reaction_ar_action_GST_Fight_Manager_gao_i_Evil_NbActor;
int action_reaction_ar_action_GST_Board_panel_gao;
int action_reaction_ar_action__PJ_Sally_gao;
int action_reaction_ar_action__PJ_Sally_gao_current_pm_mode;
int action_reaction_ar_action__PJ_Sally_gao_current_pm_priority;
int action_reaction_ar_action_GST_Board_panel_gao_i_force_interface;
int action_reaction_ar_action_univVars;
int action_reaction_ar_action_univVars_Flag_speciaux_sauve;

int action_reaction_ar_init()
{
    tScriptStruct_ActionReaction* v1; // [esp+4h] [ebp-20h]
    int a2a; // [esp+10h] [ebp-14h]
    int a2; // [esp+10h] [ebp-14h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ActionReaction*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        action_reaction_ar_init__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        action_reaction_ar_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &action_reaction_ar_init_univVars_Flag_speciaux_sauve,
            (void*)action_reaction_ar_init_univVars,
            4488);
        fct_action_reaction_ar_init_action_reaction_ar_action = (int)AI_ul_RealResolveOneRefFunction(0x9E000653);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)5)
    {
        a2 = 3 * (v1->des_i_nb_kick_for_break - 1);
        AI_EvalFunc_TRASetEnd_C(AI_gpst_CurrentGameObject, a2);
        AI_EvalFunc_TRASetEnd_C(AI_gpst_CurrentGameObject, a2 + 1);
        AI_EvalFunc_TRASetEnd_C(AI_gpst_CurrentGameObject, a2 + 2);
    LABEL_17:
        AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    v1->o_activateur = action_reaction_ar_init__PJ_Sally_gao;
    v1->des_i_distance_action = v1->des_i_distance_action * v1->des_i_distance_action;
    if (v1->des_f_haut_regard == 0.0)
        v1->des_f_haut_regard = (float)v1->des_f_offsetZ_regard_sally;
    if (v1->des_SF_dois_je_m_activer)
    {
        if (v1->des_SF_dois_je_m_activer > 0
            && AI_EvalFunc_ARRBitTest_C(
                (int*)(action_reaction_ar_init_univVars_Flag_speciaux_sauve + 4),
                v1->des_SF_dois_je_m_activer))
        {
            if (AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 0x2000000))
            {
                a2a = 3 * (v1->des_i_nb_kick_for_break - 1);
                AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, a2a);
                AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, a2a + 1);
                AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, a2a + 2);
                *AI_gppst_CurrentJumpNode = (tAINode*)5;
                TIME_Wait_C(0.1f);
                return 0;
            }
            goto LABEL_17;
        }
    }
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_action_reaction_ar_init_action_reaction_ar_action);
    return 1000;
}

int action_reaction_ar_check()
{
    tScriptStruct_ActionReaction* v2; // [esp+4h] [ebp-A8h]
    //tGameObjectMain* v19; // [esp+54h] [ebp-58h]
    int v20; // [esp+80h] [ebp-2Ch]
    int v21; // [esp+84h] [ebp-28h]
    int v22; // [esp+88h] [ebp-24h]
    int v24; // [esp+94h] [ebp-18h]
    void* Src; // [esp+98h] [ebp-14h]
    int a2; // [esp+A0h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActionReaction*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_action_reaction_ar_check_action_reaction_ar_exec_send_info = (int)AI_ul_RealResolveOneRefFunction(0x72000A9Au);
    }
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(1))
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1;
    }
    Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v2->m_msg, Src, sizeof(v2->m_msg));
    if (v2->m_msg.msgID == 10)
    {
        v24 = 0;
        v20 = v2->des_i_action;
        if (v20 == 22)
        {
            v20 = 21;
            v24 = 1;
        }
        v2->m_msg.gao = AI_gpst_CurrentGameObject;
        v2->m_msg.gao2 = 0;
        v2->m_msg.msgID = v20;
        v2->m_msg.Int1 = v24;
        v2->m_msg.Int2 = 0;
        v2->m_msg.f2 = 0.0;
        v2->m_msg.f1 = 0.0;
        v2->m_msg.v1.x = 0.0;
        v2->m_msg.v1.y = 0.0;
        v2->m_msg.v1.z = 0.0;
        v2->m_msg.v2.x = 0.0;
        v2->m_msg.v2.y = 0.0;
        v2->m_msg.v2.z = 0.0;
        AI_EvalFunc_MSGSend_C(AI2C_CheckActorGao((tGameObjectMain*)v2->o_activateur), &v2->m_msg);
        return 1000;
    }
    if (v2->m_msg.msgID != 37)
    {
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 4);
        return 1000;
    }
    if (v2->i_insensible_a_la_SA)
    {
        if (AI2C_CheckActorGao(v2->m_msg.gao2) == AI_gpst_CurrentGameObject)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
    }

    if (AI2C_CheckActorGao((tGameObjectMain*)v2->m_msg.gao) != AI2C_CheckActorGao((tGameObjectMain*)v2->o_activateur))
    {
        if (!v2->des_transfer_all_paf)
            return 1000;

        if (!AI2C_CheckActorGao((tGameObjectMain*)v2->des_o_transfert_PafUniv))
            return 1000;

        AI_EvalFunc_MSGSend_C(AI2C_CheckActorGao((tGameObjectMain*)v2->des_o_transfert_PafUniv), &v2->m_msg);
        return 1000; 
    }

    if (AI2C_CheckActorGao((tGameObjectMain*)v2->des_o_transfert_PafUniv))
    {
        AI_EvalFunc_MSGSend_C(AI2C_CheckActorGao((tGameObjectMain*)v2->des_o_transfert_PafUniv), &v2->m_msg);        
    }

    v21 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(AI_EvalFunc_MATHRandInt_C(0, 2), -1.0);
    
    if (v2->des_f_coef_volume != 0.0f)
    {
        SND_SetVol(v21, v2->des_f_coef_volume * SND_f_GetVol(v21));
    }
    
    AI_EvalFunc_SNDRequestPlay_C_CURRENT(2, -1.0);
    
    if (AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 0x2000000))
    {
        AI_EvalFunc_TRAStopAll_C(AI_gpst_CurrentGameObject);
        if (v2->des_i_evt_cycle)
        {
            a2 = 0;
            AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, 0);
        }
        else
        {
            a2 = 3 * v2->i_nb_kick;
            AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, a2);
        }
        AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, a2 + 1);
        AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, a2 + 2);
    }
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(4, 0);
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(2, 0);
    if (v2->des_i_nb_kick_for_break != -1)
        ++v2->i_nb_kick;

    if (!AI2C_CheckActorGao((tGameObjectMain*)v2->des_o_transfert_PafUniv) && (v2->des_i_unbreakable || v2->des_mode_info))
    {
        v22 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_action_reaction_ar_check_action_reaction_ar_exec_send_info);
        if (v22 != 1000)
            return v22;
    }
    return 1000;
}

int action_reaction_ar_exec_send_info()
{
    tScriptStruct_ActionReaction* v1; // [esp+0h] [ebp-5Ch]
    tMessage Src; // [esp+8h] [ebp-54h] BYREF
    int v4; // [esp+58h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ActionReaction*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v4 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        action_reaction_ar_exec_send_info_TXT_Sally_gao = AI_ul_RealResolveOneRefObject(0x7200064Eu);
    }
    if (v1->des_i_sally_text_index < 0)
        v4 = AI_EvalFunc_MATHRandInt_C(0, 3) + 20;
    else
        v4 = v1->des_i_sally_text_index;
    Src.msgID = 30;
    Src.Int1 = v4;
    Src.Int2 = 0;
    Src.f2 = v1->des_f_duree_du_text;
    Src.v1.x = 0.0;
    Src.v1.y = 0.0;
    Src.v1.z = 0.0;
    Src.v2.x = 0.0;
    Src.v2.y = 0.0;
    Src.v2.z = 0.0;
    Src.gao = 0;
    Src.gao2 = 0;
    AI_EvalFunc_MSGSend_C((tGameObjectMain*)action_reaction_ar_exec_send_info_TXT_Sally_gao, &Src);
    return 1000;
}

int action_reaction_ar_action()
{
    float v3; // [esp+0h] [ebp-ACh]
    tScriptStruct_ActionReaction* v4; // [esp+4h] [ebp-A8h]
    float v29; // [esp+88h] [ebp-24h]
    BOOL v30; // [esp+90h] [ebp-1Ch]
    int v31; // [esp+94h] [ebp-18h]
    int v32; // [esp+9Ch] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_ActionReaction*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        action_reaction_ar_action_GST_Fight_Manager_gao = AI_ul_RealResolveOneRefObject(0x9E002FFA);
        AI2C_ResolveBuffer(
            &action_reaction_ar_action_GST_Fight_Manager_gao_i_Evil_NbActor,
            (void*)action_reaction_ar_action_GST_Fight_Manager_gao,
            2624);
        action_reaction_ar_action_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        action_reaction_ar_action__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(
            &action_reaction_ar_action__PJ_Sally_gao_current_pm_mode,
            (void*)action_reaction_ar_action__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(
            &action_reaction_ar_action__PJ_Sally_gao_current_pm_priority,
            (void*)action_reaction_ar_action__PJ_Sally_gao,
            576);
        AI2C_ResolveBuffer(
            &action_reaction_ar_action_GST_Board_panel_gao_i_force_interface,
            (void*)action_reaction_ar_action_GST_Board_panel_gao,
            3292);
        action_reaction_ar_action_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &action_reaction_ar_action_univVars_Flag_speciaux_sauve,
            (void*)action_reaction_ar_action_univVars,
            4488);
    }
    if (v4->des_i_distance_action == 0.0f)
        goto LABEL_20;

    v29 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, AI2C_CheckActorGao((tGameObjectMain*)v4->o_activateur));
    if (v4->des_i_distance_action <= (double)v29)
    {
        if (v4->des_i_distance_action != 0.0f)
            goto LABEL_22;
    LABEL_20:
        if (AI_EvalFunc_COLZDEZDECollide_C(AI_gpst_CurrentGameObject, AI2C_CheckActorGao((tGameObjectMain*)v4->o_activateur), 3u, 1u))
            v31 = 1;
        else
            LABEL_22:
        v31 = 0;
    }
    else
    {
        v31 = 1;
    }
    if (v31)
    {
        v32 = v4->des_i_action == 3 ? 1812 : 3092;
        if (!v4->des_i_interdire_en_fight || !*(uint32_t*)action_reaction_ar_action_GST_Fight_Manager_gao_i_Evil_NbActor)
        {
            if ((*(uint32_t*)(action_reaction_ar_action__PJ_Sally_gao + 4) & 1) != 0)
                v30 = *(uint32_t*)action_reaction_ar_action__PJ_Sally_gao_current_pm_mode == v32 >> 8
                || *(uint32_t*)action_reaction_ar_action__PJ_Sally_gao_current_pm_priority < (int)(unsigned __int8)v32;
            else
                v30 = 0;
            if (v30)
            {
                v3 = (float)v4->des_i_priority_jump;
                if (((tScriptStruct_GSTBoardPanel*)(AI2C_GetAIModelFromGao((tGameObjectMain*)action_reaction_ar_action_GST_Board_panel_gao)->instanceVarDataCurrent))->i_sally_priority < (double)v3)
                {
                    ((tScriptStruct_GSTBoardPanel*)(AI2C_GetAIModelFromGao((tGameObjectMain*)action_reaction_ar_action_GST_Board_panel_gao)->instanceVarDataCurrent))->o_sally_contextuel = (int)AI_gpst_CurrentGameObject;                    
                    ((tScriptStruct_GSTBoardPanel*)(AI2C_GetAIModelFromGao((tGameObjectMain*)action_reaction_ar_action_GST_Board_panel_gao)->instanceVarDataCurrent))->i_sally_priority = (float)v4->des_i_priority_jump;
                    ((tScriptStruct_GSTBoardPanel*)(AI2C_GetAIModelFromGao((tGameObjectMain*)action_reaction_ar_action_GST_Board_panel_gao)->instanceVarDataCurrent))->i_sally_action = 0;
                    ((tScriptStruct_GSTBoardPanel*)(AI2C_GetAIModelFromGao((tGameObjectMain*)action_reaction_ar_action_GST_Board_panel_gao)->instanceVarDataCurrent))->v_sally_offset.x = 0.0;
                    ((tScriptStruct_GSTBoardPanel*)(AI2C_GetAIModelFromGao((tGameObjectMain*)action_reaction_ar_action_GST_Board_panel_gao)->instanceVarDataCurrent))->v_sally_offset.y = 0.0;
                    ((tScriptStruct_GSTBoardPanel*)(AI2C_GetAIModelFromGao((tGameObjectMain*)action_reaction_ar_action_GST_Board_panel_gao)->instanceVarDataCurrent))->v_sally_offset.y = v4->des_f_haut_regard;
                    *(uint32_t*)action_reaction_ar_action_GST_Board_panel_gao_i_force_interface = 1;
                }
            }
        }
    }
    if (v4->i_nb_kick == v4->des_i_nb_kick_for_break)
    {
        if (v4->des_SF_je_suis_active)
        {
            if (v4->des_SF_je_suis_active > 0)
                AI_EvalFunc_ARRBitSet_C((int*)(action_reaction_ar_action_univVars_Flag_speciaux_sauve + 4), v4->des_SF_je_suis_active);
        }
        else
        {
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x100, 0);
        }
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, 10);

        if (v4->des_SF_dois_je_m_activer > 0)
            AI_EvalFunc_ARRBitSet_C((int*)(action_reaction_ar_action_univVars_Flag_speciaux_sauve + 4), v4->des_SF_dois_je_m_activer);

        AI_EvalFunc_OBJ_FlagsIdentityGet_C(AI_gpst_CurrentGameObject);
    }
    return 1000;
}