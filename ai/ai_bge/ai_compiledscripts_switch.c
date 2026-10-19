#include "ai_compiledscripts_inc.h"

//Switch_s1_init
int fct_Switch_s1_init_Switch_s1_exec_magnet_gene;
int Switch_s1_init_GST_Global_gao;
int Switch_s1_init_GST_Global_gao_o_targets;
int Switch_s1_init_GST_Global_gao_i_targets_nb;
int Switch_s1_init_GST_Global_gao_v_targets_offset;
int fct_Switch_s1_init_Switch_s1_set_action;
int fct_Switch_s1_init_Switch_s1_wait;

//Switch_s1_wait
int Switch_s1_wait_GST_Fight_Manager_gao;
int Switch_s1_wait_GST_Fight_Manager_gao_i_Evil_NbActor;
int Switch_s1_wait__PJ_Sally_gao;
int Switch_s1_wait__PJ_Sally_gao_i_fight_on;
int fct_Switch_s1_wait_Switch_s1_exec_send_info;
int Switch_s1_wait_univVars;
int Switch_s1_wait_univVars_Perso_ID_actor;
int Switch_s1_wait_univVars_objet_inventaire;
int Switch_s1_wait_univVars_Flag_speciaux_sauve;
int fct_Switch_s1_wait_TXT_Sally_gao;
int fct_Switch_s1_wait_Switch_s1_exec_on_off;
int Switch_s1_wait_univVars_Perso_ID_Companion;
int fct_Switch_s1_wait_Switch_s1_exec_get_message;

//Switch_s1_exec_set_action
int Switch_s1_exec_set_action_GST_Fight_Manager_gao;
int Switch_s1_exec_set_action_GST_Fight_Manager_gao_i_Evil_NbActor;
int Switch_s1_exec_set_action_GST_Board_panel_gao;
int Switch_s1_exec_set_action_univVars;
int Switch_s1_exec_set_action_univVars_Perso_ID_actor;
int Switch_s1_exec_set_action_univVars_Perso_ID_Companion;
int Switch_s1_exec_set_action_univVars_objet_inventaire;
int Switch_s1_exec_set_action__PJ_Sally_gao;
int Switch_s1_exec_set_action__PJ_Sally_gao_current_pm_mode;
int Switch_s1_exec_set_action__PJ_Sally_gao_current_pm_priority;
int Switch_s1_exec_set_action_GST_Board_panel_gao_i_force_interface;

//Switch_s1_exec_set_constante
int Switch_s1_exec_set_constante_univVars;
int Switch_s1_exec_set_constante_univVars_Flag_speciaux_sauve;

//Switch_s1_exec_on_off
int Switch_s1_exec_on_off_univVars;
int Switch_s1_exec_on_off_univVars_Flag_speciaux_sauve;

//Switch_s1_exec_magnet_gene
int Switch_s1_exec_magnet_gene_DUP_Magnet_de_base_gao;

//Switch_s1_set_action
int Switch_s1_set_action_GST_Board_panel_gao;
int Switch_s1_set_action_univVars;
int Switch_s1_set_action_univVars_Flag_speciaux_sauve;
int Switch_s1_set_action__PJ_Sally_gao;
int Switch_s1_set_action_GST_Fight_Manager_gao;
int Switch_s1_set_action_GST_Fight_Manager_gao_i_Evil_NbActor;
int fct_Switch_s1_set_action_Switch_s1_exec_set_action_current_pm_mode;
int fct_Switch_s1_set_action_Switch_s1_exec_set_action_current_pm_priority;
int Switch_s1_set_action_univVars_objet_inventaire;
int fct_Switch_s1_set_action_GST_Board_panel_gao_i_force_interface;
int Switch_s1_set_action_univVars_Perso_ID_equipement;
int Switch_s1_set_action_univVars_Perso_ID_Companion;
int fct_Switch_s1_set_action_Switch_s1_exec_set_action;

//Switch_s1_exec_get_message
int Switch_s1_exec_get_message_univVars;
int Switch_s1_exec_get_message_univVars_Perso_ID_Companion;
int Switch_s1_exec_get_message_univVars_Perso_ID_actor;
int Switch_s1_exec_get_message_univVars_Flag_speciaux_sauve;
int Switch_s1_exec_get_message_GST_Global_gao;
int Switch_s1_exec_get_message_GST_Global_gao_o_actor_texte;
int Switch_s1_exec_get_message_univVars_objet_inventaire;

//Switch_s1_exec_send_info
int Switch_s1_exec_send_info_univVars;
int Switch_s1_exec_send_info_univVars_Flag_speciaux_sauve;
int Switch_s1_exec_send_info__PJ_Sally_gao;
int Switch_s1_exec_send_info_TXT_Sally_gao;
int Switch_s1_exec_send_info_univVars_Perso_ID_actor;

int Switch_s1_init()
{
    int v1; // eax
    int v2; // eax
    float v3; // [esp+0h] [ebp-D0h]
    float v4; // [esp+4h] [ebp-CCh]
    float v5; // [esp+8h] [ebp-C8h]
    tScriptStruct_Switch* v6; // [esp+Ch] [ebp-C4h]
    tGameObjectMain* v7; // [esp+20h] [ebp-B0h]
    tGameObjectMain* v8; // [esp+24h] [ebp-ACh]
    tGameObjectMain* v9; // [esp+38h] [ebp-98h]
    tGameObjectMain* v10; // [esp+3Ch] [ebp-94h]
    float v11; // [esp+40h] [ebp-90h]
    Vec3 a1; // [esp+44h] [ebp-8Ch] BYREF
    int v13; // [esp+50h] [ebp-80h]
    Vec3 a3; // [esp+54h] [ebp-7Ch] BYREF
    int v15; // [esp+64h] [ebp-6Ch]
    int v17; // [esp+78h] [ebp-58h]
    int v18; // [esp+80h] [ebp-50h]
    int v19; // [esp+84h] [ebp-4Ch]
    int v20; // [esp+88h] [ebp-48h]
    unsigned int v21; // [esp+8Ch] [ebp-44h]
    Vec3 v22; // [esp+94h] [ebp-3Ch] BYREF
    int v23; // [esp+A0h] [ebp-30h]
    int v24; // [esp+A4h] [ebp-2Ch]
    Vec3 a2; // [esp+A8h] [ebp-28h] BYREF
    tScriptStruct_Switch* v26; // [esp+B4h] [ebp-1Ch]
    tGameObjectMain* v27; // [esp+B8h] [ebp-18h]
    tGameObjectMain* v28; // [esp+BCh] [ebp-14h]

    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_Switch*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v26 = v6;
    v19 = 0;
    v20 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Switch_s1_init_Switch_s1_exec_magnet_gene = (int)AI_ul_RealResolveOneRefFunction(0x9E001B3C);
        Switch_s1_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(&Switch_s1_init_GST_Global_gao_o_targets, (void*)Switch_s1_init_GST_Global_gao, 964);
        AI2C_ResolveBuffer(&Switch_s1_init_GST_Global_gao_i_targets_nb, (void*)Switch_s1_init_GST_Global_gao, 2572);
        AI2C_ResolveBuffer(&Switch_s1_init_GST_Global_gao_v_targets_offset, (void*)Switch_s1_init_GST_Global_gao, 1368);
        fct_Switch_s1_init_Switch_s1_set_action = (int)AI_ul_RealResolveOneRefFunction(0x9E001B40);
        fct_Switch_s1_init_Switch_s1_wait = (int)AI_ul_RealResolveOneRefFunction(0x9E001B3E);
    }
    if (v26->des_i_vala_allowed)
    {
        if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x4000))
            AI_EvalFunc_GRIDCurrentSet_C(1);
        else
            AI_EvalFunc_GRIDCurrentSet_C(0);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v22);
        v11 = v26->des_f_offset_vala;
        a3.x = v22.x * v11;
        a3.y = v22.y * v11;
        a3.z = v22.z * v11;
        MATH_VecAddVec(&a1, &a2, &a3);
        v19 = AI_EvalFunc_GRIDGetCapa_C(AI_gpst_CurrentGameObject, &a1);
        if ((v19 & 0x20) != 0)
            v27 = AI_gpst_CurrentGameObject;
    }
    if (v26->des_generer_un_magnet)
    {
        v15 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Switch_s1_init_Switch_s1_exec_magnet_gene);
        if (v15 != 1000)
            return v15;
    }
    if (v26->des_enterprise_target)
    {
        v23 = AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 8, 0);
        v28 = AI_gpst_CurrentGameObject;
        v10 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v10 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            v9 = v10;
        }
        else
        {
            v9 = 0;
        }
        v20 = AI_EvalFunc_ARRObjSearch_C(
            (tGameObjectMain**)(Switch_s1_init_GST_Global_gao_o_targets + 4),
            *(uint32_t*)Switch_s1_init_GST_Global_gao_i_targets_nb,
            v9);
        if (v20 == -1)
        {
            v20 = AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)(Switch_s1_init_GST_Global_gao_o_targets + 4), 100, 0);
            if (v20 != -1)
            {
                ++* (uint32_t*)Switch_s1_init_GST_Global_gao_i_targets_nb;
                *(uint32_t*)(Switch_s1_init_GST_Global_gao_o_targets + 4 * v20 + 4) = (uint32_t)AI_gpst_CurrentGameObject;
                *(uint32_t*)AI_EvalVarArray_C3((char*)Switch_s1_init_GST_Global_gao_v_targets_offset, v20) = 0;
                *((uint32_t*)AI_EvalVarArray_C3((char*)Switch_s1_init_GST_Global_gao_v_targets_offset, v20) + 1) = 0;
                *((uint32_t*)AI_EvalVarArray_C3((char*)Switch_s1_init_GST_Global_gao_v_targets_offset, v20) + 2) = 0;
            }
        }
    }
    v26->i_actif = 0;
    v26->f_time_actif = 0.0;
    v18 = AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 2, 0);
    AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(0, -1);
    v8 = (tGameObjectMain*)v26->des_o_target;
    if (v8)
    {
        if ((unsigned int)v8 <= 2)
            v8 = AI_MainActor[(uint32_t)v8 - 1];
        v7 = v8;
    }
    else
    {
        v7 = 0;
    }
    if (!v7)
        v26->des_o_target = (int)AI_gpst_CurrentGameObject;
    if (v26->des_i_action_type_peyj == 255)
        v26->ai_action[0] = 0;
    else
        v26->ai_action[0] = 2829;
    if (v26->des_i_action_type_mili == 255)
        v26->ai_action[1] = 0;
    else
        v26->ai_action[1] = 2829;
    v1 = AI_EvalFunc_MATGetConstant_C(AI_gpst_CurrentGameObject, 1u);
    v26->c_base_constante = v1;
    v13 = AI_EvalFunc_COLORGetRed_C(v26->c_base_constante);
    v17 = AI_EvalFunc_COLORGetGreen_C(v26->c_base_constante);
    v24 = AI_EvalFunc_COLORGetBlue_C(v26->c_base_constante);
    v21 = AI_EvalFunc_COLORGetAlpha_C(v26->c_base_constante);
    v5 = (float)v24;
    v4 = (float)v17;
    v3 = (float)v13;
    v2 = AI_EvalFunc_COLORSetRGBA_C(
        (unsigned int)(v3 * 0.1),
        (unsigned int)(v4 * 0.1),
        (unsigned int)(v5 * 0.1),
        v21);
    v26->c_off_constante = v2;
    v26->f_time_flash = -1.0e14f;
    if (v26->i_prio_action)
        v26->i_prio_action = 2;
    else
        v26->i_prio_action = 5;
    AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_Switch_s1_init_Switch_s1_set_action);
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Switch_s1_init_Switch_s1_wait);
    return 1000;
}

int Switch_s1_wait()
{
    BOOL v1; // eax
    float v2; // [esp+8h] [ebp-21Ch]
    tScriptStruct_Switch* v3; // [esp+10h] [ebp-214h]
    tGameObjectMain* v4; // [esp+18h] [ebp-20Ch]
    tAI* v5; // [esp+1Ch] [ebp-208h]
    tGameObjectMain* v6; // [esp+20h] [ebp-204h]
    tGameObjectMain* v7; // [esp+24h] [ebp-200h]
    float v8; // [esp+38h] [ebp-1ECh]
    tAI* v9; // [esp+3Ch] [ebp-1E8h]
    tGameObjectMain* v10; // [esp+40h] [ebp-1E4h]
    tGameObjectMain* v11; // [esp+44h] [ebp-1E0h]
    tGameObjectMain* v12; // [esp+48h] [ebp-1DCh]
    tGameObjectMain* v13; // [esp+4Ch] [ebp-1D8h]
    tGameObjectMain* v14; // [esp+50h] [ebp-1D4h]
    tGameObjectMain* v15; // [esp+54h] [ebp-1D0h]
    tGameObjectMain* v16; // [esp+6Ch] [ebp-1B8h]
    tGameObjectMain* v17; // [esp+70h] [ebp-1B4h]
    tGameObjectMain* v18; // [esp+74h] [ebp-1B0h]
    tGameObjectMain* v19; // [esp+7Ch] [ebp-1A8h]
    tGameObjectMain* v20; // [esp+80h] [ebp-1A4h]
    tGameObjectMain* v21; // [esp+84h] [ebp-1A0h]
    tGameObjectMain* v22; // [esp+88h] [ebp-19Ch]
    tGameObjectMain* v23; // [esp+94h] [ebp-190h]
    tGameObjectMain* v24; // [esp+98h] [ebp-18Ch]
    tGameObjectMain* v25; // [esp+9Ch] [ebp-188h]
    tGameObjectMain* v26; // [esp+A0h] [ebp-184h]
    tGameObjectMain* v27; // [esp+A8h] [ebp-17Ch]
    tGameObjectMain* v28; // [esp+ACh] [ebp-178h]
    tGameObjectMain* v29; // [esp+B4h] [ebp-170h]
    tGameObjectMain* v30; // [esp+BCh] [ebp-168h]
    Vec3 v31; // [esp+C0h] [ebp-164h] BYREF
    tGameObjectMain* v32; // [esp+DCh] [ebp-148h]
    BOOL v33; // [esp+E8h] [ebp-13Ch]
    Vec3 v34; // [esp+ECh] [ebp-138h] BYREF
    float a2; // [esp+FCh] [ebp-128h]
    int v36; // [esp+114h] [ebp-110h]
    int v37; // [esp+118h] [ebp-10Ch]
    tMessage v38; // [esp+11Ch] [ebp-108h] BYREF
    unsigned int v39; // [esp+154h] [ebp-D0h]
    int v41; // [esp+198h] [ebp-8Ch]
    int v43; // [esp+1A4h] [ebp-80h]
    tAI* v45; // [esp+1ACh] [ebp-78h]
    int v47; // [esp+1B4h] [ebp-70h]
    Vec3 a3; // [esp+1C4h] [ebp-60h] BYREF
    int v49; // [esp+1DCh] [ebp-48h]
    BOOL v50; // [esp+1E0h] [ebp-44h]
    int v51; // [esp+1E4h] [ebp-40h]
    int v52; // [esp+1E8h] [ebp-3Ch]
    void* Src; // [esp+1F0h] [ebp-34h]
    tScriptStruct_Switch* v54; // [esp+1F4h] [ebp-30h]
    int v56; // [esp+1FCh] [ebp-28h]
    int v57; // [esp+200h] [ebp-24h]
    int v58; // [esp+204h] [ebp-20h]
    BOOL v59; // [esp+20Ch] [ebp-18h]
    Vec3 v60; // [esp+210h] [ebp-14h] BYREF
    int v61; // [esp+21Ch] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_Switch*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v54 = v3;
    v32 = 0;
    v50 = 0;
    v51 = 0;
    v45 = 0;
    v39 = 0;
    v41 = 0;
    v52 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Switch_s1_wait_GST_Fight_Manager_gao = AI_ul_RealResolveOneRefObject(0x9E002FFA);
        AI2C_ResolveBuffer(
            &Switch_s1_wait_GST_Fight_Manager_gao_i_Evil_NbActor,
            (void*)Switch_s1_wait_GST_Fight_Manager_gao,
            2624);
        Switch_s1_wait__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(&Switch_s1_wait__PJ_Sally_gao_i_fight_on, (void*)Switch_s1_wait__PJ_Sally_gao, 1216);
        fct_Switch_s1_wait_Switch_s1_exec_send_info = (int)AI_ul_RealResolveOneRefFunction(0x720006D9u);
        Switch_s1_wait_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&Switch_s1_wait_univVars_Perso_ID_actor, (void*)Switch_s1_wait_univVars, 988);
        AI2C_ResolveBuffer(&Switch_s1_wait_univVars_objet_inventaire, (void*)Switch_s1_wait_univVars, 1480);
        AI2C_ResolveBuffer(&Switch_s1_wait_univVars_Flag_speciaux_sauve, (void*)Switch_s1_wait_univVars, 4488);
        fct_Switch_s1_wait_TXT_Sally_gao = AI_ul_RealResolveOneRefObject(0x7200064Eu);
        fct_Switch_s1_wait_Switch_s1_exec_on_off = (int)AI_ul_RealResolveOneRefFunction(0x9E001D18);
        AI2C_ResolveBuffer(&Switch_s1_wait_univVars_Perso_ID_Companion, (void*)Switch_s1_wait_univVars, 1040);
        fct_Switch_s1_wait_Switch_s1_exec_get_message = (int)AI_ul_RealResolveOneRefFunction(0x9E002E17);
    }
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 1);
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 4);
    v32 = *AI_MainActor;
    if (v54->des_i_interdire_en_fight && *(uint32_t*)Switch_s1_wait_GST_Fight_Manager_gao_i_Evil_NbActor)
    {
        AI_EvalFunc_MSGClear_C(AI_gpst_CurrentGameObject);
        goto LABEL_131;
    }
    Src = AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v54->m_msg, Src, sizeof(v54->m_msg));
    if (v54->m_msg.msgID == 10 && !AI_EvalFunc_OBJ_CapaTest_C_CURRENT(32))
    {
        v54->m_msg.msgID = 7;
        v54->m_msg.gao = AI_gpst_CurrentGameObject;
        v54->m_msg.gao2 = 0;
        v54->m_msg.Int1 = 1;
        v54->m_msg.Int2 = 0;
        v54->m_msg.f2 = 0.0;
        v54->m_msg.f1 = 0.0;
        v54->m_msg.v1.x = 0.0;
        v54->m_msg.v1.y = 0.0;
        v54->m_msg.v1.z = 0.0;
        v54->m_msg.v2.x = 0.0;
        v54->m_msg.v2.y = 0.0;
        v54->m_msg.v2.z = 0.0;
        v30 = v32;
        if (v32)
        {
            if ((unsigned int)v32 <= 2)
                v30 = AI_MainActor[(uint32_t)v32 - 1];
            AI_EvalFunc_MSGSend_C(v30, &v54->m_msg);
            goto LABEL_131;
        }
        goto LABEL_12;
    }
    if (v54->m_msg.msgID == 9 || v54->m_msg.msgID == 10 && (v59 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(32)))
    {
        v54->m_msg.msgID = 64;
        v54->m_msg.Int1 = v54->des_i_carte_d_access;
        v54->m_msg.gao = AI_gpst_CurrentGameObject;
        v29 = v32;
        if (v32)
        {
            if ((unsigned int)v32 <= 2)
                v29 = AI_MainActor[(uint32_t)v32 - 1];
            AI_EvalFunc_MSGSend_C(v29, &v54->m_msg);
            goto LABEL_131;
        }
    LABEL_12:
        AI_EvalFunc_MSGSend_C(0, &v54->m_msg);
        goto LABEL_131;
    }
    if (v54->m_msg.msgID == 37 && (v54->m_msg.Int1 & 7) != 0)
    {
        if (AI_EvalFunc_MATHFloatNullEps_C(v54->f_time_actif))
        {
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(4, 0);
            if (v54->des_i_carte_d_access)
            {
                AI_EvalFunc_OBJ_CapaSet_C_CURRENT(1, 0);
                v54->i_index_message = 26;
                v28 = v54->m_msg.gao;
                if (v28)
                {
                    if ((unsigned int)v28 <= 2)
                        v28 = AI_MainActor[(uint32_t)v28 - 1];
                    v27 = v28;
                }
                else
                {
                    v27 = 0;
                }
                if (v27 != (tGameObjectMain*)Switch_s1_wait__PJ_Sally_gao
                    || (v54->m_msg.Int1 & 0x2000000) != 0
                    || *(uint32_t*)Switch_s1_wait__PJ_Sally_gao_i_fight_on)
                {
                    v26 = *(tGameObjectMain**)(Switch_s1_wait_univVars_Perso_ID_actor + 4);
                    if (v26)
                    {
                        if ((unsigned int)v26 <= 2)
                            v26 = AI_MainActor[(uint32_t)v26 - 1];
                        v25 = v26;
                    }
                    else
                    {
                        v25 = 0;
                    }
                    if (!v25)
                        goto LABEL_55;
                    v24 = v54->m_msg.gao;
                    if (v24)
                    {
                        if ((unsigned int)v24 <= 2)
                            v24 = AI_MainActor[(uint32_t)v24 - 1];
                        v23 = v24;
                    }
                    else
                    {
                        v23 = 0;
                    }
                    if (v23 == *(tGameObjectMain**)(Switch_s1_wait_univVars_Perso_ID_actor + 4))
                    {
                        if (!*(uint32_t*)AI_EvalVarArray2_C(
                            1,
                            (char*)Switch_s1_wait_univVars_objet_inventaire,
                            v54->des_i_carte_d_access,
                            0))
                        {
                            v36 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Switch_s1_wait_Switch_s1_exec_send_info);
                            if (v36 != 1000)
                                return v36;
                            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                            return 1000;
                        }
                    }
                    else
                    {
                    LABEL_55:
                        v22 = *(tGameObjectMain**)(Switch_s1_wait_univVars_Perso_ID_actor + 8);
                        if (v22)
                        {
                            if ((unsigned int)v22 <= 2)
                                v22 = AI_MainActor[(uint32_t)v22 - 1];
                            v21 = v22;
                        }
                        else
                        {
                            v21 = 0;
                        }
                        if (!v21)
                            goto LABEL_71;
                        v20 = v54->m_msg.gao;
                        if (v20)
                        {
                            if ((unsigned int)v20 <= 2)
                                v20 = AI_MainActor[(uint32_t)v20 - 1];
                            v19 = v20;
                        }
                        else
                        {
                            v19 = 0;
                        }
                        if (v19 != *(tGameObjectMain**)(Switch_s1_wait_univVars_Perso_ID_actor + 8))
                        {
                        LABEL_71:
                            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                            return 1000;
                        }
                        if (!*(uint32_t*)AI_EvalVarArray2_C(
                            1,
                            (char*)Switch_s1_wait_univVars_objet_inventaire,
                            v54->des_i_carte_d_access,
                            1))
                        {
                            v43 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Switch_s1_wait_Switch_s1_exec_send_info);
                            if (v43 != 1000)
                                return v43;
                            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                            return 1000;
                        }
                    }
                }
                else if (v54->m_msg.f2 >= 0.0)
                {
                    v56 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Switch_s1_wait_Switch_s1_exec_send_info);
                    if (v56 != 1000)
                        return v56;
                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                    return 1000;
                }
            }
            v18 = (tGameObjectMain*)v54->des_o_actor_who_allow_it;
            if (v18)
            {
                if ((unsigned int)v18 <= 2)
                    v18 = AI_MainActor[(uint32_t)v18 - 1];
                v17 = v18;
            }
            else
            {
                v17 = 0;
            }
            if (v17)
            {
                v16 = (tGameObjectMain*)v54->des_o_actor_who_allow_it;
                if (v16)
                {
                    if ((unsigned int)v16 <= 2)
                        v16 = AI_MainActor[(uint32_t)v16 - 1];
                    v1 = AI_EvalFunc_OBJ_CapaTest_C(v16, 256);
                }
                else
                {
                    v1 = AI_EvalFunc_OBJ_CapaTest_C(0, 256);
                }
                v33 = v1;
                if (!v1)
                {
                    v54->i_index_message = 25;
                    v61 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Switch_s1_wait_Switch_s1_exec_send_info);
                    if (v61 != 1000)
                        return v61;
                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                    return 1000;
                }
            }
            if (v54->i_interdire_si_comp_KO)
            {
                v50 = AI_EvalFunc_ARRBitTest_C((int*)(Switch_s1_wait_univVars_Flag_speciaux_sauve + 4), 930);
                if (v50)
                {
                    v38.msgID = 30;
                    v38.Int1 = 58;
                    v38.Int2 = 0;
                    v38.f2 = 1.5;
                    v38.v1.x = 0.0;
                    v38.v1.y = 0.0;
                    v38.v1.z = 0.0;
                    v38.v2.x = 0.0;
                    v38.v2.y = 0.0;
                    v38.v2.z = 0.0;
                    v38.gao = 0;
                    v38.gao2 = 0;
                    AI_EvalFunc_MSGSend_C((tGameObjectMain*)fct_Switch_s1_wait_TXT_Sally_gao, &v38);
                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                    return 1000;
                }
                v50 = AI_EvalFunc_ARRBitTest_C((int*)(Switch_s1_wait_univVars_Flag_speciaux_sauve + 4), 931);
                if (v50)
                {
                    v38.msgID = 30;
                    v38.Int1 = 59;
                    v38.Int2 = 0;
                    v38.f2 = 1.5;
                    v38.v1.x = 0.0;
                    v38.v1.y = 0.0;
                    v38.v1.z = 0.0;
                    v38.v2.x = 0.0;
                    v38.v2.y = 0.0;
                    v38.v2.z = 0.0;
                    v38.gao = 0;
                    v38.gao2 = 0;
                    AI_EvalFunc_MSGSend_C((tGameObjectMain*)fct_Switch_s1_wait_TXT_Sally_gao, &v38);
                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                    return 1000;
                }
            }
            if (v54->des_i_SF_mecanisme_en_fonctio)
            {
                if (v54->des_i_SF_mecanisme_en_fonctio >= 10000)
                {
                    v51 = v54->des_i_SF_mecanisme_en_fonctio - 10000;
                    v50 = v51 > 0 && AI_EvalFunc_ARRBitTest_C((int*)(Switch_s1_wait_univVars_Flag_speciaux_sauve + 4), v51);
                    v50 = !v50;
                }
                else
                {
                    v50 = v54->des_i_SF_mecanisme_en_fonctio > 0
                        && AI_EvalFunc_ARRBitTest_C(
                            (int*)(Switch_s1_wait_univVars_Flag_speciaux_sauve + 4),
                            v54->des_i_SF_mecanisme_en_fonctio);
                }
                if (!v50)
                {
                    v54->i_index_message = 25;
                    v47 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Switch_s1_wait_Switch_s1_exec_send_info);
                    if (v47 != 1000)
                        return v47;
                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                    return 1000;
                }
            }
            if (v54->TXT_X2Inter_QdCompagnonAbsent)
            {
                v15 = v54->m_msg.gao;
                if (v15)
                {
                    if ((unsigned int)v15 <= 2)
                        v15 = AI_MainActor[(uint32_t)v15 - 1];
                    v14 = v15;
                }
                else
                {
                    v14 = 0;
                }
                if (v14 == (tGameObjectMain*)Switch_s1_wait__PJ_Sally_gao)
                {
                    v45 = (tAI*)AI_MainActor[1];
                    v13 = (tGameObjectMain*)v45;
                    if (v45)
                    {
                        if ((unsigned int)v45 <= 2)
                            v13 = AI_MainActor[(uint32_t)v45 - 1];
                        v12 = v13;
                    }
                    else
                    {
                        v12 = 0;
                    }
                    if (!v12)
                        goto LABEL_183;
                    if (v45 == WOR_gst_Universe.univAIInstance)
                    {
                        v9 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v11 = (tGameObjectMain*)v45;
                        if (v45)
                        {
                            if ((unsigned int)v45 <= 2)
                                v11 = AI_MainActor[(uint32_t)v45 - 1];
                            v10 = v11;
                        }
                        else
                        {
                            v10 = 0;
                        }
                        v9 = v10->extended->AIInstance;
                    }
                    if ((*((uint32_t*)v9->instanceVarDataCurrent + 409) & 2) != 0)
                    {
                    LABEL_183:
                        v54->i_index_message = v54->TXT_X2Inter_QdCompagnonAbsent;
                        v37 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Switch_s1_wait_Switch_s1_exec_send_info);
                        if (v37 != 1000)
                            return v37;
                    }
                }
            }
            v54->i_actif = 1;
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 1);
            v49 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(0, -1.0);
            if (v54->des_f_time_on != 0.0)
                v54->f_time_actif = TIM_gf_MainClock - TIM_gf_LockTime;
        }
    }
LABEL_131:
    v57 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Switch_s1_wait_Switch_s1_exec_on_off);
    if (v57 != 1000)
        return v57;
    if (v54->des_sound_mode)
    {
        if (!AI_EvalFunc_TIMEElapsed_C(v54->f_time_actif, v54->des_f_time_on))
        {
            if (AI_EvalFunc_TIMEElapsed_C(v54->f_time_sound, v54->f_sound_delay))
            {
                v54->f_time_sound = TIM_gf_MainClock - TIM_gf_LockTime;
                if (v54->f_time_actif + v54->des_f_time_on <= v54->f_time_sound + 3.0)
                {
                    v54->f_sound_delay = v54->des_sound_frequence_countdown / 2.0f;
                    v54->i_sound_pulse = 3;
                }
                else
                {
                    v54->f_sound_delay = v54->des_sound_frequence_countdown;
                    v54->i_sound_pulse = 2;
                }
                if (AI_EvalFunc_TIMEElapsed_C(v54->f_time_actif, v54->des_delay_sans_son))
                {
                    v52 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, v54->i_sound_pulse, 128);
                    AI_EvalFunc_SNDUltraPlaySound_C(AI_gpst_CurrentGameObject, v52);
                }
            }
        }
    }
    if (v54->des_i_interdire_en_fight && *(uint32_t*)Switch_s1_wait_GST_Fight_Manager_gao_i_Evil_NbActor)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    if (v54->des_i_vala_allowed)
    {
        v41 = *(uint32_t*)Switch_s1_wait_univVars_Perso_ID_Companion;
        if (v41 == -1 || v41 > 1)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v54->des_i_action_type = v54->ai_action[v41];
        v54->f_vala_prio = (float)(v41 + 150);
        v54->des_i_acteur_choix = v41 << 24;
        memcpy(&v54->m_vala, &v54->m_msg, sizeof(v54->m_vala));
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v34);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v60);
        v8 = v54->des_f_offset_vala;
        a3.x = v60.x * v8;
        a3.y = v60.y * v8;
        a3.z = v60.z * v8;
        MATH_VecAddVec(&v31, &v34, &a3);
        v54->v_vala_pos = v31;
        if (v54->des_i_SF_acces_a_l_inter)
        {
            if (v54->des_i_SF_acces_a_l_inter >= 10000)
            {
                if (v54->des_i_SF_acces_a_l_inter - 10000 <= 0)
                    v50 = 0;
                else
                    v50 = AI_EvalFunc_ARRBitTest_C(
                        (int*)(Switch_s1_wait_univVars_Flag_speciaux_sauve + 4),
                        v54->des_i_SF_acces_a_l_inter - 10000);
                v50 = !v50;
            }
            else if (v54->des_i_SF_acces_a_l_inter <= 0)
            {
                v50 = 0;
            }
            else
            {
                v50 = AI_EvalFunc_ARRBitTest_C(
                    (int*)(Switch_s1_wait_univVars_Flag_speciaux_sauve + 4),
                    v54->des_i_SF_acces_a_l_inter);
            }
        }
        else
        {
            v50 = 1;
        }
        if (v50)
        {
            v58 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Switch_s1_wait_Switch_s1_exec_get_message);
            if (v58 != 1000)
                return v58;
        }
        else if (v54->m_vala.msgID == 38 && v54->m_vala.f2 == 100.0)
        {
            v45 = (tAI*)AI_MainActor[1];
            if (v45 == WOR_gst_Universe.univAIInstance)
            {
                v5 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v7 = (tGameObjectMain*)v45;
                if (v45)
                {
                    if ((unsigned int)v45 <= 2)
                        v7 = AI_MainActor[(uint32_t)v45 - 1];
                    v6 = v7;
                }
                else
                {
                    v6 = 0;
                }
                v5 = v6->extended->AIInstance;
            }
            v39 = *((uint32_t*)v5->instanceVarDataCurrent + 7);
            v38.msgID = 30;
            v38.Int1 = 42;
            v38.Int2 = 0;
            v38.f2 = 1.5;
            v38.v1.x = 0.0;
            v38.v1.y = 0.0;
            v38.v1.z = 0.0;
            v38.v2.x = 0.0;
            v38.v2.y = 0.0;
            v38.v2.z = 0.0;
            v38.gao = 0;
            v38.gao2 = 0;
            v4 = (tGameObjectMain*)v39;
            if (v39)
            {
                if (v39 <= 2)
                    v4 = AI_MainActor[v39 - 1];
                AI_EvalFunc_MSGSend_C(v4, &v38);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &v38);
            }
        }
    }
    AI_EvalFunc_OBJ_ExtraFlagsSet_C(AI_gpst_CurrentGameObject, 8, 0);
    if (2.0 < 1600.0)
        v2 = 1600.0;
    else
        v2 = 2.0;
    a2 = v2;
    AI_EvalFunc_OBJ_AILodSet_C(AI_gpst_CurrentGameObject, v2);
    return 1000;
}

int Switch_s1_exec_reset_constante()
{
    tScriptStruct_Switch* v1; // [esp+0h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Switch*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (v1->c_base_constante)
        AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v1->c_base_constante, 1);
    return 1000;
}

int Switch_s1_exec_set_action()
{
    double v1; // st7
    double v2; // st7
    BOOL v3; // eax
    double v4; // st7
    double v5; // st7
    BOOL v6; // eax
    float v7; // [esp+8h] [ebp-378h]
    float v8; // [esp+Ch] [ebp-374h]
    float v9; // [esp+18h] [ebp-368h]
    float v10; // [esp+1Ch] [ebp-364h]
    float v11; // [esp+28h] [ebp-358h]
    float v12; // [esp+2Ch] [ebp-354h]
    tScriptStruct_Switch* v13; // [esp+30h] [ebp-350h]
    tGameObjectMain* v14; // [esp+40h] [ebp-340h]
    tGameObjectMain* v15; // [esp+44h] [ebp-33Ch]
    tGameObjectMain* v16; // [esp+48h] [ebp-338h]
    tAI* v17; // [esp+4Ch] [ebp-334h]
    tGameObjectMain* v18; // [esp+50h] [ebp-330h]
    tGameObjectMain* v19; // [esp+54h] [ebp-32Ch]
    tAI* v20; // [esp+58h] [ebp-328h]
    tGameObjectMain* v21; // [esp+5Ch] [ebp-324h]
    tGameObjectMain* v22; // [esp+60h] [ebp-320h]
    tAI* v23; // [esp+64h] [ebp-31Ch]
    tGameObjectMain* v24; // [esp+68h] [ebp-318h]
    tGameObjectMain* v25; // [esp+6Ch] [ebp-314h]
    tAI* v26; // [esp+70h] [ebp-310h]
    tGameObjectMain* v27; // [esp+74h] [ebp-30Ch]
    tGameObjectMain* v28; // [esp+78h] [ebp-308h]
    tAI* v29; // [esp+7Ch] [ebp-304h]
    tGameObjectMain* v30; // [esp+80h] [ebp-300h]
    tGameObjectMain* v31; // [esp+84h] [ebp-2FCh]
    tAI* v32; // [esp+88h] [ebp-2F8h]
    tGameObjectMain* v33; // [esp+8Ch] [ebp-2F4h]
    tGameObjectMain* v34; // [esp+90h] [ebp-2F0h]
    tAI* v35; // [esp+94h] [ebp-2ECh]
    tGameObjectMain* v36; // [esp+98h] [ebp-2E8h]
    tGameObjectMain* v37; // [esp+9Ch] [ebp-2E4h]
    tAI* v38; // [esp+A0h] [ebp-2E0h]
    tGameObjectMain* v39; // [esp+A4h] [ebp-2DCh]
    tGameObjectMain* v40; // [esp+A8h] [ebp-2D8h]
    tAI* v41; // [esp+ACh] [ebp-2D4h]
    tGameObjectMain* v42; // [esp+B0h] [ebp-2D0h]
    tGameObjectMain* v43; // [esp+B4h] [ebp-2CCh]
    tAI* v44; // [esp+B8h] [ebp-2C8h]
    tGameObjectMain* v45; // [esp+BCh] [ebp-2C4h]
    tGameObjectMain* v46; // [esp+C0h] [ebp-2C0h]
    tAI* v47; // [esp+C4h] [ebp-2BCh]
    tGameObjectMain* v48; // [esp+C8h] [ebp-2B8h]
    tGameObjectMain* v49; // [esp+CCh] [ebp-2B4h]
    tAI* v50; // [esp+D0h] [ebp-2B0h]
    tGameObjectMain* v51; // [esp+D4h] [ebp-2ACh]
    tGameObjectMain* v52; // [esp+D8h] [ebp-2A8h]
    tAI* v53; // [esp+DCh] [ebp-2A4h]
    tGameObjectMain* v54; // [esp+E0h] [ebp-2A0h]
    tGameObjectMain* v55; // [esp+E4h] [ebp-29Ch]
    tAI* v56; // [esp+E8h] [ebp-298h]
    tGameObjectMain* v57; // [esp+ECh] [ebp-294h]
    tGameObjectMain* v58; // [esp+F0h] [ebp-290h]
    tAI* v59; // [esp+F4h] [ebp-28Ch]
    tGameObjectMain* v60; // [esp+F8h] [ebp-288h]
    tGameObjectMain* v61; // [esp+FCh] [ebp-284h]
    tAI* v62; // [esp+100h] [ebp-280h]
    tGameObjectMain* v63; // [esp+104h] [ebp-27Ch]
    tGameObjectMain* v64; // [esp+108h] [ebp-278h]
    tGameObjectMain* v65; // [esp+110h] [ebp-270h]
    tGameObjectMain* v66; // [esp+118h] [ebp-268h]
    tGameObjectMain* v67; // [esp+120h] [ebp-260h]
    tGameObjectMain* v68; // [esp+128h] [ebp-258h]
    tGameObjectMain* v69; // [esp+12Ch] [ebp-254h]
    tGameObjectMain* v70; // [esp+130h] [ebp-250h]
    tGameObjectMain* v71; // [esp+134h] [ebp-24Ch]
    tGameObjectMain* v72; // [esp+138h] [ebp-248h]
    tAI* v73; // [esp+13Ch] [ebp-244h]
    tGameObjectMain* v74; // [esp+140h] [ebp-240h]
    tGameObjectMain* v75; // [esp+144h] [ebp-23Ch]
    tAI* v76; // [esp+148h] [ebp-238h]
    tGameObjectMain* v77; // [esp+14Ch] [ebp-234h]
    tGameObjectMain* v78; // [esp+150h] [ebp-230h]
    tAI* v79; // [esp+154h] [ebp-22Ch]
    tGameObjectMain* v80; // [esp+158h] [ebp-228h]
    tGameObjectMain* v81; // [esp+15Ch] [ebp-224h]
    tAI* v82; // [esp+160h] [ebp-220h]
    tGameObjectMain* v83; // [esp+164h] [ebp-21Ch]
    tGameObjectMain* v84; // [esp+168h] [ebp-218h]
    tAI* v85; // [esp+16Ch] [ebp-214h]
    tGameObjectMain* v86; // [esp+170h] [ebp-210h]
    tGameObjectMain* v87; // [esp+174h] [ebp-20Ch]
    tAI* v88; // [esp+178h] [ebp-208h]
    tGameObjectMain* v89; // [esp+17Ch] [ebp-204h]
    tGameObjectMain* v90; // [esp+180h] [ebp-200h]
    tAI* v91; // [esp+184h] [ebp-1FCh]
    tGameObjectMain* v92; // [esp+188h] [ebp-1F8h]
    tGameObjectMain* v93; // [esp+18Ch] [ebp-1F4h]
    tAI* v94; // [esp+190h] [ebp-1F0h]
    tGameObjectMain* v95; // [esp+194h] [ebp-1ECh]
    tGameObjectMain* v96; // [esp+198h] [ebp-1E8h]
    tAI* v97; // [esp+19Ch] [ebp-1E4h]
    tGameObjectMain* v98; // [esp+1A0h] [ebp-1E0h]
    tGameObjectMain* v99; // [esp+1A4h] [ebp-1DCh]
    double v100; // [esp+1A8h] [ebp-1D8h]
    tAI* v101; // [esp+1B0h] [ebp-1D0h]
    tGameObjectMain* v102; // [esp+1B4h] [ebp-1CCh]
    tGameObjectMain* v103; // [esp+1B8h] [ebp-1C8h]
    tAI* v104; // [esp+1BCh] [ebp-1C4h]
    tGameObjectMain* v105; // [esp+1C0h] [ebp-1C0h]
    tGameObjectMain* v106; // [esp+1C4h] [ebp-1BCh]
    tGameObjectMain* v107; // [esp+1CCh] [ebp-1B4h]
    tGameObjectMain* v108; // [esp+1D4h] [ebp-1ACh]
    tGameObjectMain* v109; // [esp+1DCh] [ebp-1A4h]
    Vec3* v110; // [esp+1E0h] [ebp-1A0h]
    Vec3* v111; // [esp+1E4h] [ebp-19Ch]
    tAI* v112; // [esp+1F8h] [ebp-188h]
    tGameObjectMain* v113; // [esp+1FCh] [ebp-184h]
    tGameObjectMain* v114; // [esp+200h] [ebp-180h]
    tAI* v115; // [esp+204h] [ebp-17Ch]
    tGameObjectMain* v116; // [esp+208h] [ebp-178h]
    tGameObjectMain* v117; // [esp+20Ch] [ebp-174h]
    tAI* v118; // [esp+210h] [ebp-170h]
    tGameObjectMain* v119; // [esp+214h] [ebp-16Ch]
    tGameObjectMain* v120; // [esp+218h] [ebp-168h]
    tAI* v121; // [esp+21Ch] [ebp-164h]
    tGameObjectMain* v122; // [esp+220h] [ebp-160h]
    tGameObjectMain* v123; // [esp+224h] [ebp-15Ch]
    tAI* v124; // [esp+228h] [ebp-158h]
    tGameObjectMain* v125; // [esp+22Ch] [ebp-154h]
    tGameObjectMain* v126; // [esp+230h] [ebp-150h]
    tGameObjectMain* v127; // [esp+238h] [ebp-148h]
    tGameObjectMain* v128; // [esp+240h] [ebp-140h]
    tGameObjectMain* v129; // [esp+250h] [ebp-130h]
    tAI* v130; // [esp+254h] [ebp-12Ch]
    tGameObjectMain* v131; // [esp+258h] [ebp-128h]
    tGameObjectMain* v132; // [esp+25Ch] [ebp-124h]
    tGameObjectMain* v133; // [esp+260h] [ebp-120h]
    tGameObjectMain* v134; // [esp+264h] [ebp-11Ch]
    double v135; // [esp+268h] [ebp-118h]
    tGameObjectMain* v136; // [esp+270h] [ebp-110h]
    tGameObjectMain* v137; // [esp+274h] [ebp-10Ch]
    float X; // [esp+278h] [ebp-108h]
    float v139; // [esp+278h] [ebp-108h]
    float v140; // [esp+278h] [ebp-108h]
    float v141; // [esp+27Ch] [ebp-104h]
    Vec3 v142; // [esp+284h] [ebp-FCh] BYREF
    Vec3 v143; // [esp+290h] [ebp-F0h] BYREF
    tGameObjectMain* v144; // [esp+29Ch] [ebp-E4h]
    int v145; // [esp+2A0h] [ebp-E0h]
    int v146; // [esp+2A4h] [ebp-DCh]
    int v147; // [esp+2A8h] [ebp-D8h]
    int v148; // [esp+2ACh] [ebp-D4h]
    Vec3 v149; // [esp+2B0h] [ebp-D0h]
    Vec3 v150; // [esp+2BCh] [ebp-C4h] BYREF
    float v151; // [esp+2C8h] [ebp-B8h]
    int v152; // [esp+2CCh] [ebp-B4h]
    Vec3 v153; // [esp+2D0h] [ebp-B0h] BYREF
    Vec3 v154; // [esp+2DCh] [ebp-A4h]
    Vec3 v155; // [esp+2E8h] [ebp-98h] BYREF
    BOOL v156; // [esp+2F4h] [ebp-8Ch]
    tAI* v157; // [esp+2F8h] [ebp-88h]
    float v158; // [esp+2FCh] [ebp-84h]
    float v159; // [esp+300h] [ebp-80h]
    float v160; // [esp+304h] [ebp-7Ch]
    Vec3 v161; // [esp+308h] [ebp-78h] BYREF
    int v162; // [esp+31Ch] [ebp-64h]
    Vec3 dest; // [esp+320h] [ebp-60h] BYREF
    float v164; // [esp+32Ch] [ebp-54h]
    int v165; // [esp+330h] [ebp-50h]
    Vec3 v166; // [esp+334h] [ebp-4Ch] BYREF
    int v167; // [esp+340h] [ebp-40h]
    tAI* v168; // [esp+344h] [ebp-3Ch]
    Vec3 a3; // [esp+348h] [ebp-38h] BYREF
    tScriptStruct_Switch* v170; // [esp+354h] [ebp-2Ch]
    float v171; // [esp+358h] [ebp-28h]
    Vec3 v172; // [esp+35Ch] [ebp-24h] BYREF
    BOOL v173; // [esp+368h] [ebp-18h]
    Vec3 v174; // [esp+370h] [ebp-10h] BYREF
    float v175; // [esp+37Ch] [ebp-4h]

    v149.x = 1.0;
    v149.y = 0.0;
    v149.z = 0.0;
    v154.x = 1.0;
    v154.y = 0.0;
    v154.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v13 = (tScriptStruct_Switch*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v13 = 0;
    v170 = v13;
    v157 = 0;
    v162 = 0;
    v145 = 0;
    v168 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Switch_s1_exec_set_action_GST_Fight_Manager_gao = AI_ul_RealResolveOneRefObject(0x9E002FFA);
        AI2C_ResolveBuffer(
            &Switch_s1_exec_set_action_GST_Fight_Manager_gao_i_Evil_NbActor,
            (void*)Switch_s1_exec_set_action_GST_Fight_Manager_gao,
            2624);
        Switch_s1_exec_set_action_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        Switch_s1_exec_set_action_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Switch_s1_exec_set_action_univVars_Perso_ID_actor,
            (void*)Switch_s1_exec_set_action_univVars,
            988);
        AI2C_ResolveBuffer(
            &Switch_s1_exec_set_action_univVars_Perso_ID_Companion,
            (void*)Switch_s1_exec_set_action_univVars,
            1040);
        AI2C_ResolveBuffer(
            &Switch_s1_exec_set_action_univVars_objet_inventaire,
            (void*)Switch_s1_exec_set_action_univVars,
            1480);
        Switch_s1_exec_set_action__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(
            &Switch_s1_exec_set_action__PJ_Sally_gao_current_pm_mode,
            (void*)Switch_s1_exec_set_action__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(
            &Switch_s1_exec_set_action__PJ_Sally_gao_current_pm_priority,
            (void*)Switch_s1_exec_set_action__PJ_Sally_gao,
            576);
        AI2C_ResolveBuffer(
            &Switch_s1_exec_set_action_GST_Board_panel_gao_i_force_interface,
            (void*)Switch_s1_exec_set_action_GST_Board_panel_gao,
            3292);
    }
    if (v170->Interdiction_pendant_fight && *(uint32_t*)Switch_s1_exec_set_action_GST_Fight_Manager_gao_i_Evil_NbActor)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    if (v170->i_trigger_on == 2)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    v145 = v170->des_o_target;
    if (v170->des_f_agit_distmax < 0.5)
        v170->des_f_agit_distmax = 0.5;
    X = v170->des_f_vala_angle / 180.0f * 3.141593f;
    v168 = (tAI*)Switch_s1_exec_set_action_GST_Board_panel_gao;
    v137 = (tGameObjectMain*)v170->o_compagnon_qui_agit;
    if (v137)
    {
        if ((unsigned int)v137 <= 2)
            v137 = AI_MainActor[(uint32_t)v137 - 1];
        v136 = v137;
    }
    else
    {
        v136 = 0;
    }
    if (v136)
    {
        v157 = (tAI*)v170->o_compagnon_qui_agit;
    }
    else
    {
        v135 = v170->f_vala_prio - 150.0 + MATH_gd_magicDoubleForRound;
        v162 = (*(uint32_t *)&v135);
        v157 = *(tAI**)(Switch_s1_exec_set_action_univVars_Perso_ID_actor + 4 * (*(uint32_t *)&v135) + 4);
    }
    v134 = (tGameObjectMain*)v157;
    if (v157)
    {
        if ((unsigned int)v157 <= 2)
            v134 = AI_MainActor[(uint32_t)v157 - 1];
        v133 = v134;
    }
    else
    {
        v133 = 0;
    }
    if (!v133)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    if (v157 == WOR_gst_Universe.univAIInstance)
    {
        v130 = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        v132 = (tGameObjectMain*)v157;
        if (v157)
        {
            if ((unsigned int)v157 <= 2)
                v132 = AI_MainActor[(uint32_t)v157 - 1];
            v131 = v132;
        }
        else
        {
            v131 = 0;
        }
        v130 = v131->extended->AIInstance;
    }
    if (((tScriptStruct_Companion*)(v130->instanceVarDataCurrent))->co_current_mode != 11 )
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v150);
        v129 = (tGameObjectMain*)v157;
        if (v157)
        {
            if ((unsigned int)v157 <= 2)
                v129 = AI_MainActor[(uint32_t)v157 - 1];
            AI_EvalFunc_OBJGetPos_C(v129, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        MATH_SubstractVec(&dest, &v150, &a3);
        v149 = dest;
        v128 = (tGameObjectMain*)v157;
        if (v157)
        {
            if ((unsigned int)v157 <= 2)
                v128 = AI_MainActor[(uint32_t)v157 - 1];
            AI_EvalFunc_OBJGetPos_C(v128, &v143);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v143);
        }
        v127 = (tGameObjectMain*)v170->o_vala_visee;
        if (v127)
        {
            if ((unsigned int)v127 <= 2)
                v127 = AI_MainActor[(uint32_t)v127 - 1];
            v1 = AI_EvalFunc_OBJIsInCone_C(v127, &v143, -1.0, v170->des_f_agit_distmax, 1);
        }
        else
        {
            v1 = AI_EvalFunc_OBJIsInCone_C(0, &v143, -1.0, v170->des_f_agit_distmax, 1);
        }
        v141 = (float)v1;
        if (v141 == 0.0 || (v12 = (float)fabs(v149.z), v159 = v12, v12 >= 1.5))
        {
            v144 = AI_MainActor[1];
            v72 = (tGameObjectMain*)v157;
            if (v157)
            {
                if ((unsigned int)v157 <= 2)
                    v72 = AI_MainActor[(uint32_t)v157 - 1];
                v71 = v72;
            }
            else
            {
                v71 = 0;
            }
            v70 = v144;
            if (v144)
            {
                if ((unsigned int)v144 <= 2)
                    v70 = AI_MainActor[(uint32_t)v144 - 1];
                v69 = v70;
            }
            else
            {
                v69 = 0;
            }
            if (v71 == v69)
            {
                if (v170->des_f_distmax_sally == 0.0)
                    goto LABEL_412;
                v68 = (tGameObjectMain*)Switch_s1_exec_set_action__PJ_Sally_gao;
                if (Switch_s1_exec_set_action__PJ_Sally_gao)
                {
                    if ((unsigned int)Switch_s1_exec_set_action__PJ_Sally_gao <= 2)
                        v68 = AI_MainActor[Switch_s1_exec_set_action__PJ_Sally_gao - 1];
                    v4 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v68);
                }
                else
                {
                    v4 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
                }
                v175 = (float)v4;
                if (v170->des_f_distmax_sally * v170->des_f_distmax_sally > v175)
                {
                LABEL_412:
                    v67 = (tGameObjectMain*)v157;
                    if (v157)
                    {
                        if ((unsigned int)v157 <= 2)
                            v67 = AI_MainActor[(uint32_t)v157 - 1];
                        AI_EvalFunc_OBJGetPos_C(v67, &v161);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v161);
                    }
                    v151 = cosf(X);
                    v66 = (tGameObjectMain*)v170->o_vala_visee;
                    if (v66)
                    {
                        if ((unsigned int)v66 <= 2)
                            v66 = AI_MainActor[(uint32_t)v66 - 1];
                        v5 = AI_EvalFunc_OBJIsInCone_C(v66, &v161, v151, v170->des_f_vala_distmax, 1);
                    }
                    else
                    {
                        v5 = AI_EvalFunc_OBJIsInCone_C(0, &v161, v151, v170->des_f_vala_distmax, 1);
                    }
                    v158 = (float)v5;
                    if (v158 != 0.0)
                    {
                        v10 = (float)fabs(v149.z);
                        if (v10 < 1.5)
                        {
                            if (v170->i_vala_action_sent)
                            {
                                v65 = (tGameObjectMain*)v157;
                                if (v157)
                                {
                                    if ((unsigned int)v157 <= 2)
                                        v65 = AI_MainActor[(uint32_t)v157 - 1];
                                    v6 = AI_EvalFunc_OBJ_CapaTest_C(v65, 0x80);
                                }
                                else
                                {
                                    v6 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x80);
                                }
                                v156 = v6;
                                if (v6)
                                {
                                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                                    return 1000;
                                }
                            }
                            if (v168 == WOR_gst_Universe.univAIInstance)
                            {
                                v62 = WOR_gst_Universe.univAIInstance;
                            }
                            else
                            {
                                v64 = (tGameObjectMain*)v168;
                                if (v168)
                                {
                                    if ((unsigned int)v168 <= 2)
                                        v64 = AI_MainActor[(uint32_t)v168 - 1];
                                    v63 = v64;
                                }
                                else
                                {
                                    v63 = 0;
                                }
                                v62 = v63->extended->AIInstance;
                            }
                            if (v168 == WOR_gst_Universe.univAIInstance)
                            {
                                v59 = WOR_gst_Universe.univAIInstance;
                            }
                            else
                            {
                                v61 = (tGameObjectMain*)v168;
                                if (v168)
                                {
                                    if ((unsigned int)v168 <= 2)
                                        v61 = AI_MainActor[(uint32_t)v168 - 1];
                                    v60 = v61;
                                }
                                else
                                {
                                    v60 = 0;
                                }
                                v59 = v60->extended->AIInstance;
                            }
                            v9 = v170->des_f_vala_offset_cible;
                            *(float*)AI_EvalVarArray_C2(
                                (char*)&(((tScriptStruct_GSTBoardPanel*)(v59->instanceVarDataCurrent))->__af_targetsoffsets_Dimension1),
                                    ((tScriptStruct_GSTBoardPanel*)(v62->instanceVarDataCurrent))->i_current_nb_of_target) = v9;
                            if (v168 == WOR_gst_Universe.univAIInstance)
                            {
                                v56 = WOR_gst_Universe.univAIInstance;
                            }
                            else
                            {
                                v58 = (tGameObjectMain*)v168;
                                if (v168)
                                {
                                    if ((unsigned int)v168 <= 2)
                                        v58 = AI_MainActor[(uint32_t)v168 - 1];
                                    v57 = v58;
                                }
                                else
                                {
                                    v57 = 0;
                                }
                                v56 = v57->extended->AIInstance;
                            }
                            if (v168 == WOR_gst_Universe.univAIInstance)
                            {
                                v53 = WOR_gst_Universe.univAIInstance;
                            }
                            else
                            {
                                v55 = (tGameObjectMain*)v168;
                                if (v168)
                                {
                                    if ((unsigned int)v168 <= 2)
                                        v55 = AI_MainActor[(uint32_t)v168 - 1];
                                    v54 = v55;
                                }
                                else
                                {
                                    v54 = 0;
                                }
                                v53 = v54->extended->AIInstance;
                            }
                            ((tScriptStruct_GSTBoardPanel*)(v53->instanceVarDataCurrent))->ao_targets[((tScriptStruct_GSTBoardPanel*)(v56->instanceVarDataCurrent))->i_current_nb_of_target] = v145;
                            if (v168 == WOR_gst_Universe.univAIInstance)
                            {
                                v50 = WOR_gst_Universe.univAIInstance;
                            }
                            else
                            {
                                v52 = (tGameObjectMain*)v168;
                                if (v168)
                                {
                                    if ((unsigned int)v168 <= 2)
                                        v52 = AI_MainActor[(uint32_t)v168 - 1];
                                    v51 = v52;
                                }
                                else
                                {
                                    v51 = 0;
                                }
                                v50 = v51->extended->AIInstance;
                            }
                            ++((tScriptStruct_GSTBoardPanel*)(v50->instanceVarDataCurrent))->i_current_nb_of_target;
                            v167 = (unsigned __int16)*(uint32_t*)(Switch_s1_exec_set_action__PJ_Sally_gao + 4);
                            if ((v167 & 1) != 0)
                            {
                                if (*(uint32_t*)Switch_s1_exec_set_action__PJ_Sally_gao_current_pm_mode
                                    && *(int*)Switch_s1_exec_set_action__PJ_Sally_gao_current_pm_priority >= 95)
                                {
                                    v140 = 0.0;
                                }
                                else
                                {
                                    v140 = 1.0;
                                }
                            }
                            else
                            {
                                v140 = 0.0;
                            }
                            if (v140 != 0.0)
                            {
                                if (v168 == WOR_gst_Universe.univAIInstance)
                                {
                                    v47 = WOR_gst_Universe.univAIInstance;
                                }
                                else
                                {
                                    v49 = (tGameObjectMain*)v168;
                                    if (v168)
                                    {
                                        if ((unsigned int)v168 <= 2)
                                            v49 = AI_MainActor[(uint32_t)v168 - 1];
                                        v48 = v49;
                                    }
                                    else
                                    {
                                        v48 = 0;
                                    }
                                    v47 = v48->extended->AIInstance;
                                }
                                if (v168 == WOR_gst_Universe.univAIInstance)
                                {
                                    v44 = WOR_gst_Universe.univAIInstance;
                                }
                                else
                                {
                                    v46 = (tGameObjectMain*)v168;
                                    if (v168)
                                    {
                                        if ((unsigned int)v168 <= 2)
                                            v46 = AI_MainActor[(uint32_t)v168 - 1];
                                        v45 = v46;
                                    }
                                    else
                                    {
                                        v45 = 0;
                                    }
                                    v44 = v45->extended->AIInstance;
                                }
                                ((tScriptStruct_GSTBoardPanel*)(v44->instanceVarDataCurrent))->ao_listofobjects[((tScriptStruct_GSTBoardPanel*)(v47->instanceVarDataCurrent))->i_current_nb_of_object] = (int)AI_gpst_CurrentGameObject;
                                if (v168 == WOR_gst_Universe.univAIInstance)
                                {
                                    v41 = WOR_gst_Universe.univAIInstance;
                                }
                                else
                                {
                                    v43 = (tGameObjectMain*)v168;
                                    if (v168)
                                    {
                                        if ((unsigned int)v168 <= 2)
                                            v43 = AI_MainActor[(uint32_t)v168 - 1];
                                        v42 = v43;
                                    }
                                    else
                                    {
                                        v42 = 0;
                                    }
                                    v41 = v42->extended->AIInstance;
                                }
                                if (v168 == WOR_gst_Universe.univAIInstance)
                                {
                                    v38 = WOR_gst_Universe.univAIInstance;
                                }
                                else
                                {
                                    v40 = (tGameObjectMain*)v168;
                                    if (v168)
                                    {
                                        if ((unsigned int)v168 <= 2)
                                            v40 = AI_MainActor[(uint32_t)v168 - 1];
                                        v39 = v40;
                                    }
                                    else
                                    {
                                        v39 = 0;
                                    }
                                    v38 = v39->extended->AIInstance;
                                }
                                ((tScriptStruct_GSTBoardPanel*)(v38->instanceVarDataCurrent))->ai_listofpriority[((tScriptStruct_GSTBoardPanel*)(v41->instanceVarDataCurrent))->i_current_nb_of_object] = 100;
                                if (v168 == WOR_gst_Universe.univAIInstance)
                                {
                                    v35 = WOR_gst_Universe.univAIInstance;
                                }
                                else
                                {
                                    v37 = (tGameObjectMain*)v168;
                                    if (v168)
                                    {
                                        if ((unsigned int)v168 <= 2)
                                            v37 = AI_MainActor[(uint32_t)v168 - 1];
                                        v36 = v37;
                                    }
                                    else
                                    {
                                        v36 = 0;
                                    }
                                    v35 = v36->extended->AIInstance;
                                }
                                if (v168 == WOR_gst_Universe.univAIInstance)
                                {
                                    v32 = WOR_gst_Universe.univAIInstance;
                                }
                                else
                                {
                                    v34 = (tGameObjectMain*)v168;
                                    if (v168)
                                    {
                                        if ((unsigned int)v168 <= 2)
                                            v34 = AI_MainActor[(uint32_t)v168 - 1];
                                        v33 = v34;
                                    }
                                    else
                                    {
                                        v33 = 0;
                                    }
                                    v32 = v33->extended->AIInstance;
                                }
                                ((tScriptStruct_GSTBoardPanel*)(v32->instanceVarDataCurrent))->ai_action[((tScriptStruct_GSTBoardPanel*)(v35->instanceVarDataCurrent))->i_current_nb_of_object] = v170->des_i_acteur_choix + ((v170->des_i_action_type & 0xFF00) >> 8);
                                if (v168 == WOR_gst_Universe.univAIInstance)
                                {
                                    v29 = WOR_gst_Universe.univAIInstance;
                                }
                                else
                                {
                                    v31 = (tGameObjectMain*)v168;
                                    if (v168)
                                    {
                                        if ((unsigned int)v168 <= 2)
                                            v31 = AI_MainActor[(uint32_t)v168 - 1];
                                        v30 = v31;
                                    }
                                    else
                                    {
                                        v30 = 0;
                                    }
                                    v29 = v30->extended->AIInstance;
                                }
                                if (v168 == WOR_gst_Universe.univAIInstance)
                                {
                                    v26 = WOR_gst_Universe.univAIInstance;
                                }
                                else
                                {
                                    v28 = (tGameObjectMain*)v168;
                                    if (v168)
                                    {
                                        if ((unsigned int)v168 <= 2)
                                            v28 = AI_MainActor[(uint32_t)v168 - 1];
                                        v27 = v28;
                                    }
                                    else
                                    {
                                        v27 = 0;
                                    }
                                    v26 = v27->extended->AIInstance;
                                }
                                ((tScriptStruct_GSTBoardPanel*)(v26->instanceVarDataCurrent))->ai_listofbutton[((tScriptStruct_GSTBoardPanel*)(v29->instanceVarDataCurrent))->i_current_nb_of_object] = 0;
                                if (v168 == WOR_gst_Universe.univAIInstance)
                                {
                                    v23 = WOR_gst_Universe.univAIInstance;
                                }
                                else
                                {
                                    v25 = (tGameObjectMain*)v168;
                                    if (v168)
                                    {
                                        if ((unsigned int)v168 <= 2)
                                            v25 = AI_MainActor[(uint32_t)v168 - 1];
                                        v24 = v25;
                                    }
                                    else
                                    {
                                        v24 = 0;
                                    }
                                    v23 = v24->extended->AIInstance;
                                }
                                if (v168 == WOR_gst_Universe.univAIInstance)
                                {
                                    v20 = WOR_gst_Universe.univAIInstance;
                                }
                                else
                                {
                                    v22 = (tGameObjectMain*)v168;
                                    if (v168)
                                    {
                                        if ((unsigned int)v168 <= 2)
                                            v22 = AI_MainActor[(uint32_t)v168 - 1];
                                        v21 = v22;
                                    }
                                    else
                                    {
                                        v21 = 0;
                                    }
                                    v20 = v21->extended->AIInstance;
                                }
                                ((tScriptStruct_GSTBoardPanel*)(v20->instanceVarDataCurrent))->ao_listoftargets[((tScriptStruct_GSTBoardPanel*)(v23->instanceVarDataCurrent))->i_current_nb_of_object] = v145;
                                if (v168 == WOR_gst_Universe.univAIInstance)
                                {
                                    v17 = WOR_gst_Universe.univAIInstance;
                                }
                                else
                                {
                                    v19 = (tGameObjectMain*)v168;
                                    if (v168)
                                    {
                                        if ((unsigned int)v168 <= 2)
                                            v19 = AI_MainActor[(uint32_t)v168 - 1];
                                        v18 = v19;
                                    }
                                    else
                                    {
                                        v18 = 0;
                                    }
                                    v17 = v18->extended->AIInstance;
                                }
                                ++((tScriptStruct_GSTBoardPanel*)(v17->instanceVarDataCurrent))->i_current_nb_of_object;
                                *(uint32_t*)Switch_s1_exec_set_action_GST_Board_panel_gao_i_force_interface = 1;
                            }
                        }
                    }
                }
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v174);
                v165 = AI_EvalFunc_GRIDGetCapa_C(AI_gpst_CurrentGameObject, &v174);
                if ((v165 & 0xC0) != 0)
                {
                    v16 = (tGameObjectMain*)v170->o_compagnon_qui_agit;
                    if (v16)
                    {
                        if ((unsigned int)v16 <= 2)
                            v16 = AI_MainActor[(uint32_t)v16 - 1];
                        v15 = v16;
                    }
                    else
                    {
                        v15 = 0;
                    }
                    if (v15)
                    {
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&v155);
                        AI_EvalFunc_GRIDPosGet_C(AI_gpst_CurrentGameObject, &v155, &v153);
                        v14 = (tGameObjectMain*)v170->o_compagnon_qui_agit;
                        if (v14)
                        {
                            if ((unsigned int)v14 <= 2)
                                v14 = AI_MainActor[(uint32_t)v14 - 1];
                            AI_EvalFunc_OBJGetPos_C(v14, &v166);
                        }
                        else
                        {
                            AI_EvalFunc_OBJGetPos_C(0, &v166);
                        }
                        AI_EvalFunc_GRIDPosGet_C(AI_gpst_CurrentGameObject, &v166, &v142);
                        MATH_SubstractVec(&v172, &v153, &v142);
                        v154 = v172;
                        v8 = (float)fabs(v172.x);
                        v160 = v8;
                        if (v8 > 1.5 || (v7 = (float)fabs(v154.y), v171 = v7, v7 > 1.5))
                            v170->o_compagnon_qui_agit = 0;
                    }
                }
            }
        }
        else
        {
            if (v168 == WOR_gst_Universe.univAIInstance)
            {
                v124 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v126 = (tGameObjectMain*)v168;
                if (v168)
                {
                    if ((unsigned int)v168 <= 2)
                        v126 = AI_MainActor[(uint32_t)v168 - 1];
                    v125 = v126;
                }
                else
                {
                    v125 = 0;
                }
                v124 = v125->extended->AIInstance;
            }
            if (v168 == WOR_gst_Universe.univAIInstance)
            {
                v121 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v123 = (tGameObjectMain*)v168;
                if (v168)
                {
                    if ((unsigned int)v168 <= 2)
                        v123 = AI_MainActor[(uint32_t)v168 - 1];
                    v122 = v123;
                }
                else
                {
                    v122 = 0;
                }
                v121 = v122->extended->AIInstance;
            }
            v11 = v170->des_f_vala_offset_cible;
            *(float*)AI_EvalVarArray_C2(
                (char*)&(((tScriptStruct_GSTBoardPanel*)(v121->instanceVarDataCurrent))->__af_targetsoffsets_Dimension1),
                ((tScriptStruct_GSTBoardPanel*)(v124->instanceVarDataCurrent))->i_current_nb_of_target) = v11;
            if (v168 == WOR_gst_Universe.univAIInstance)
            {
                v118 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v120 = (tGameObjectMain*)v168;
                if (v168)
                {
                    if ((unsigned int)v168 <= 2)
                        v120 = AI_MainActor[(uint32_t)v168 - 1];
                    v119 = v120;
                }
                else
                {
                    v119 = 0;
                }
                v118 = v119->extended->AIInstance;
            }
            if (v168 == WOR_gst_Universe.univAIInstance)
            {
                v115 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v117 = (tGameObjectMain*)v168;
                if (v168)
                {
                    if ((unsigned int)v168 <= 2)
                        v117 = AI_MainActor[(uint32_t)v168 - 1];
                    v116 = v117;
                }
                else
                {
                    v116 = 0;
                }
                v115 = v116->extended->AIInstance;
            }
            ((tScriptStruct_GSTBoardPanel*)(v115->instanceVarDataCurrent))->ao_targets[((tScriptStruct_GSTBoardPanel*)(v118->instanceVarDataCurrent))->i_current_nb_of_target] = v145;
            if (v168 == WOR_gst_Universe.univAIInstance)
            {
                v112 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v114 = (tGameObjectMain*)v168;
                if (v168)
                {
                    if ((unsigned int)v168 <= 2)
                        v114 = AI_MainActor[(uint32_t)v168 - 1];
                    v113 = v114;
                }
                else
                {
                    v113 = 0;
                }
                v112 = v113->extended->AIInstance;
            }
            ++* ((uint32_t*)v112->instanceVarDataCurrent + 106);
            if (v170->des_f_vala_distmax != 0.0 && v170->i_vala_fini && v170->des_i_auto
                || v170->des_f_vala_distmax == 0.0 && v170->des_i_auto)
            {
                v170->m_vala.msgID = 6;
                v170->m_vala.Int1 = 5;
                v170->m_vala.Int2 = (unsigned __int8)v170->des_i_action_type;
                v170->m_vala.f2 = v170->des_f_vala_angle_belier;
                v170->m_vala.gao = 0;
                v170->m_vala.gao2 = AI_gpst_CurrentGameObject;
                v170->m_vala.f1 = v170->des_f_agit_distmax;
                v146 = 0;
                v147 = 0;
                v148 = 0;
                v149.x = 0.0;
                v149.y = 0.0;
                v149.z = 0.0;
                if (v170->m_vala.Int2 == 13
                    && v170->des_i_carte_d_access
                    && *(uint32_t*)AI_EvalVarArray2_C(
                        1,
                        (char*)Switch_s1_exec_set_action_univVars_objet_inventaire,
                        v170->des_i_carte_d_access,
                        *(uint32_t*)Switch_s1_exec_set_action_univVars_Perso_ID_Companion))
                {
                    v149.x = (float)v170->des_i_carte_d_access;
                }
                v149.y = v170->f_delai_attente_apres_action;
                v170->m_vala.v1 = v149;
                v110 = &v170->m_vala.v2;
                v111 = &v170->v_coup_cul_offset_sol;
                v170->m_vala.v2.x = v170->v_coup_cul_offset_sol.x;
                v110->y = v111->y;
                v110->z = v111->z;
                v109 = (tGameObjectMain*)v157;
                if (v157)
                {
                    if ((unsigned int)v157 <= 2)
                        v109 = AI_MainActor[(uint32_t)v157 - 1];
                    AI_EvalFunc_MSGSend_C(v109, &v170->m_vala);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &v170->m_vala);
                }
                v170->i_vala_fini = 0;
                v170->i_vala_action_sent = 1;
                AI_EvalFunc_OBJ_CapaSet_C_CURRENT(8, 0);
                v170->o_compagnon_qui_agit = 0;
            }
            else
            {
                if (v170->des_f_distmax_sally == 0.0f)
                    goto LABEL_413;
                v108 = (tGameObjectMain*)Switch_s1_exec_set_action__PJ_Sally_gao;
                if (Switch_s1_exec_set_action__PJ_Sally_gao)
                {
                    if ((unsigned int)Switch_s1_exec_set_action__PJ_Sally_gao <= 2)
                        v108 = AI_MainActor[Switch_s1_exec_set_action__PJ_Sally_gao - 1];
                    v2 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v108);
                }
                else
                {
                    v2 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
                }
                v164 = (float)v2;
                if (v170->des_f_distmax_sally * v170->des_f_distmax_sally > v164)
                {
                LABEL_413:
                    if (v170->i_vala_action_sent)
                    {
                        v107 = (tGameObjectMain*)v157;
                        if (v157)
                        {
                            if ((unsigned int)v157 <= 2)
                                v107 = AI_MainActor[(uint32_t)v157 - 1];
                            v3 = AI_EvalFunc_OBJ_CapaTest_C(v107, 128);
                        }
                        else
                        {
                            v3 = AI_EvalFunc_OBJ_CapaTest_C(0, 128);
                        }
                        v173 = v3;
                        if (v3)
                        {
                            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                            return 1000;
                        }
                    }
                    v152 = (unsigned __int16)*(uint32_t*)(Switch_s1_exec_set_action__PJ_Sally_gao + 4);
                    if ((v152 & 1) != 0)
                    {
                        if (*(uint32_t*)Switch_s1_exec_set_action__PJ_Sally_gao_current_pm_mode
                            && *(int*)Switch_s1_exec_set_action__PJ_Sally_gao_current_pm_priority >= 95)
                        {
                            v139 = 0.0;
                        }
                        else
                        {
                            v139 = 1.0;
                        }
                    }
                    else
                    {
                        v139 = 0.0;
                    }
                    if (v139 != 0.0)
                    {
                        if (v168 == WOR_gst_Universe.univAIInstance)
                        {
                            v104 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v106 = (tGameObjectMain*)v168;
                            if (v168)
                            {
                                if ((unsigned int)v168 <= 2)
                                    v106 = AI_MainActor[(uint32_t)v168 - 1];
                                v105 = v106;
                            }
                            else
                            {
                                v105 = 0;
                            }
                            v104 = v105->extended->AIInstance;
                        }
                        if (v168 == WOR_gst_Universe.univAIInstance)
                        {
                            v101 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v103 = (tGameObjectMain*)v168;
                            if (v168)
                            {
                                if ((unsigned int)v168 <= 2)
                                    v103 = AI_MainActor[(uint32_t)v168 - 1];
                                v102 = v103;
                            }
                            else
                            {
                                v102 = 0;
                            }
                            v101 = v102->extended->AIInstance;
                        }
                        ((tScriptStruct_GSTBoardPanel*)(v101->instanceVarDataCurrent))->ao_listofobjects[((tScriptStruct_GSTBoardPanel*)(v104->instanceVarDataCurrent))->i_current_nb_of_object] = (int)AI_gpst_CurrentGameObject;
                        if (v168 == WOR_gst_Universe.univAIInstance)
                        {
                            v97 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v99 = (tGameObjectMain*)v168;
                            if (v168)
                            {
                                if ((unsigned int)v168 <= 2)
                                    v99 = AI_MainActor[(uint32_t)v168 - 1];
                                v98 = v99;
                            }
                            else
                            {
                                v98 = 0;
                            }
                            v97 = v98->extended->AIInstance;
                        }
                        if (v168 == WOR_gst_Universe.univAIInstance)
                        {
                            v94 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v96 = (tGameObjectMain*)v168;
                            if (v168)
                            {
                                if ((unsigned int)v168 <= 2)
                                    v96 = AI_MainActor[(uint32_t)v168 - 1];
                                v95 = v96;
                            }
                            else
                            {
                                v95 = 0;
                            }
                            v94 = v95->extended->AIInstance;
                        }
                        v100 = v170->f_vala_prio + MATH_gd_magicDoubleForRound;
                        ((tScriptStruct_GSTBoardPanel*)(v94->instanceVarDataCurrent))->ai_listofpriority[((tScriptStruct_GSTBoardPanel*)(v97->instanceVarDataCurrent))->i_current_nb_of_object] = (*(uint32_t*)&v100);
                        if (v168 == WOR_gst_Universe.univAIInstance)
                        {
                            v91 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v93 = (tGameObjectMain*)v168;
                            if (v168)
                            {
                                if ((unsigned int)v168 <= 2)
                                    v93 = AI_MainActor[(uint32_t)v168 - 1];
                                v92 = v93;
                            }
                            else
                            {
                                v92 = 0;
                            }
                            v91 = v92->extended->AIInstance;
                        }
                        if (v168 == WOR_gst_Universe.univAIInstance)
                        {
                            v88 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v90 = (tGameObjectMain*)v168;
                            if (v168)
                            {
                                if ((unsigned int)v168 <= 2)
                                    v90 = AI_MainActor[(uint32_t)v168 - 1];
                                v89 = v90;
                            }
                            else
                            {
                                v89 = 0;
                            }
                            v88 = v89->extended->AIInstance;
                        }
                        ((tScriptStruct_GSTBoardPanel*)(v88->instanceVarDataCurrent))->ai_action[((tScriptStruct_GSTBoardPanel*)(v91->instanceVarDataCurrent))->i_current_nb_of_object] = v170->des_i_acteur_choix + ((v170->des_i_action_type & 0xFF00) >> 8);
                        if (v168 == WOR_gst_Universe.univAIInstance)
                        {
                            v85 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v87 = (tGameObjectMain*)v168;
                            if (v168)
                            {
                                if ((unsigned int)v168 <= 2)
                                    v87 = AI_MainActor[(uint32_t)v168 - 1];
                                v86 = v87;
                            }
                            else
                            {
                                v86 = 0;
                            }
                            v85 = v86->extended->AIInstance;
                        }
                        if (v168 == WOR_gst_Universe.univAIInstance)
                        {
                            v82 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v84 = (tGameObjectMain*)v168;
                            if (v168)
                            {
                                if ((unsigned int)v168 <= 2)
                                    v84 = AI_MainActor[(uint32_t)v168 - 1];
                                v83 = v84;
                            }
                            else
                            {
                                v83 = 0;
                            }
                            v82 = v83->extended->AIInstance;
                        }
                        ((tScriptStruct_GSTBoardPanel*)(v82->instanceVarDataCurrent))->ai_listofbutton[((tScriptStruct_GSTBoardPanel*)(v85->instanceVarDataCurrent))->i_current_nb_of_object] = 0;
                        if (v168 == WOR_gst_Universe.univAIInstance)
                        {
                            v79 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v81 = (tGameObjectMain*)v168;
                            if (v168)
                            {
                                if ((unsigned int)v168 <= 2)
                                    v81 = AI_MainActor[(uint32_t)v168 - 1];
                                v80 = v81;
                            }
                            else
                            {
                                v80 = 0;
                            }
                            v79 = v80->extended->AIInstance;
                        }
                        if (v168 == WOR_gst_Universe.univAIInstance)
                        {
                            v76 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v78 = (tGameObjectMain*)v168;
                            if (v168)
                            {
                                if ((unsigned int)v168 <= 2)
                                    v78 = AI_MainActor[(uint32_t)v168 - 1];
                                v77 = v78;
                            }
                            else
                            {
                                v77 = 0;
                            }
                            v76 = v77->extended->AIInstance;
                        }
                        ((tScriptStruct_GSTBoardPanel*)(v76->instanceVarDataCurrent))->ao_listoftargets[((tScriptStruct_GSTBoardPanel*)(v79->instanceVarDataCurrent))->i_current_nb_of_object] = v145;

                        if (v168 == WOR_gst_Universe.univAIInstance)
                        {
                            v73 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v75 = (tGameObjectMain*)v168;
                            if (v168)
                            {
                                if ((unsigned int)v168 <= 2)
                                    v75 = AI_MainActor[(uint32_t)v168 - 1];
                                v74 = v75;
                            }
                            else
                            {
                                v74 = 0;
                            }
                            v73 = v74->extended->AIInstance;
                        }
                        ++((tScriptStruct_GSTBoardPanel*)(v73->instanceVarDataCurrent))->i_current_nb_of_object;
                        *(uint32_t*)Switch_s1_exec_set_action_GST_Board_panel_gao_i_force_interface = 1;
                    }
                }
            }
        }
    }
    return 1000;
}

int Switch_s1_exec_set_constante()
{
    BOOL v1; // eax
    double v2; // st7
    float v3; // [esp+0h] [ebp-98h]
    float X; // [esp+4h] [ebp-94h]
    float Xa; // [esp+4h] [ebp-94h]
    float Xb; // [esp+4h] [ebp-94h]
    float Xc; // [esp+4h] [ebp-94h]
    float v8; // [esp+8h] [ebp-90h]
    float v9; // [esp+Ch] [ebp-8Ch]
    float v10; // [esp+14h] [ebp-84h]
    float v11; // [esp+18h] [ebp-80h]
    float v12; // [esp+20h] [ebp-78h]
    tScriptStruct_Switch* v13; // [esp+24h] [ebp-74h]
    tGameObjectMain* v14; // [esp+2Ch] [ebp-6Ch]
    tGameObjectMain* v15; // [esp+30h] [ebp-68h]
    tGameObjectMain* v16; // [esp+34h] [ebp-64h]
    float v17; // [esp+40h] [ebp-58h]
    float v18; // [esp+40h] [ebp-58h]
    float v19; // [esp+40h] [ebp-58h]
    float v20; // [esp+40h] [ebp-58h]
    float v21; // [esp+50h] [ebp-48h]
    int v22; // [esp+54h] [ebp-44h]
    float v23; // [esp+58h] [ebp-40h]
    int v24; // [esp+64h] [ebp-34h]
    float v25; // [esp+68h] [ebp-30h]
    int v26; // [esp+6Ch] [ebp-2Ch]
    float v27; // [esp+70h] [ebp-28h]
    float v28; // [esp+78h] [ebp-20h]
    int a2; // [esp+7Ch] [ebp-1Ch]
    BOOL v30; // [esp+80h] [ebp-18h]
    BOOL v31; // [esp+80h] [ebp-18h]
    int v32; // [esp+8Ch] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v13 = (tScriptStruct_Switch*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v13 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Switch_s1_exec_set_constante_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Switch_s1_exec_set_constante_univVars_Flag_speciaux_sauve,
            (void*)Switch_s1_exec_set_constante_univVars,
            4488);
    }
    if (!v13->c_base_constante)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    v30 = 1;
    if (v13->des_i_SF_mecanisme_en_fonctio)
    {
        if (v13->des_i_SF_mecanisme_en_fonctio >= 10000)
        {
            a2 = v13->des_i_SF_mecanisme_en_fonctio - 10000;
            if (a2 <= 0)
                v31 = 0;
            else
                v31 = AI_EvalFunc_ARRBitTest_C((int*)(Switch_s1_exec_set_constante_univVars_Flag_speciaux_sauve + 4), a2);
            v30 = !v31;
        }
        else if (v13->des_i_SF_mecanisme_en_fonctio <= 0)
        {
            v30 = 0;
        }
        else
        {
            v30 = AI_EvalFunc_ARRBitTest_C(
                (int*)(Switch_s1_exec_set_constante_univVars_Flag_speciaux_sauve + 4),
                v13->des_i_SF_mecanisme_en_fonctio);
        }
    }
    if (v13->i_valide_courant_aussi && !v30)
        goto LABEL_31;
    v16 = (tGameObjectMain*)v13->des_o_actor_who_allow_it;
    if (v16)
    {
        if ((unsigned int)v16 <= 2)
            v16 = AI_MainActor[(uint32_t)v16 - 1];
        v15 = v16;
    }
    else
    {
        v15 = 0;
    }
    if (v15)
    {
        v14 = (tGameObjectMain*)v13->des_o_actor_who_allow_it;
        if (v14)
        {
            if ((unsigned int)v14 <= 2)
                v14 = AI_MainActor[(uint32_t)v14 - 1];
            v1 = AI_EvalFunc_OBJ_CapaTest_C(v14, 0x100);
        }
        else
        {
            v1 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x100);
        }
        if (!v1)
            goto LABEL_31;
    }
    v12 = (float)v13->des_i_flash_nb;
    if (v13->des_f_flash_time * v12 >= v13->des_f_time_on)
    {
        v10 = (float)v13->des_i_flash_nb;
        v28 = v13->des_f_flash_time * v10;
        v26 = 0;
    }
    else
    {
        v11 = (float)(v13->des_i_flash_nb - 1);
        v28 = v13->des_f_flash_time * v11 + v13->des_f_flash_time * 3.0f / 2.0f;
        v26 = 1;
    }
    if (!AI_EvalFunc_TIMEElapsed_C(v13->f_time_flash, v28))
    {
        if (v26)
        {
            v23 = TIM_gf_MainClock - TIM_gf_LockTime;
            v9 = (float)(v13->des_i_flash_nb - 1);
            v17 = (v23 - v13->f_time_flash) / v28 * (v9 * 6.283185f + 4.712389f);
            v2 = sinf(v17);
        }
        else
        {
            v25 = TIM_gf_MainClock - TIM_gf_LockTime;
            v18 = (v25 - v13->f_time_flash) / v13->des_f_flash_time;
            v8 = (float)v13->i_flag_bip;
            if (v18 > (double)v8)
            {
                ++v13->i_flag_bip;
                AI_EvalFunc_SNDRequestPlay_C_CURRENT(4, -1.0);
            }
            v19 = v18 * 6.283185f;
            v2 = sinf(v19);
        }
        v20 = (float)v2;
        if (v20 <= 0.0)
        {
            Xc = -v20;
            v22 = AI_EvalFunc_COLORBlend_C(v13->c_base_constante, v13->c_off_constante, Xc);
            AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v22, 1);
        }
        else
        {
            v24 = AI_EvalFunc_COLORBlend_C(v13->c_base_constante, 0xFFFFFF, v20);
            AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v24, 1);
        }
        return 1000;
    }
    if (v13->i_flag_bip)
    {
        v13->i_flag_bip = 0;
        AI_EvalFunc_SNDRequestPlay_C_CURRENT(4, -1.0);
    }
    if (!v26)
    {
    LABEL_42:
        AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v13->c_base_constante, 1);
        return 1000;
    }
    if (AI_EvalFunc_TIMEElapsed_C(v13->f_time_flash, v13->des_f_time_on))
    {
        X = v13->des_f_flash_time / 4.0f;
        v3 = v13->f_time_flash + v13->des_f_time_on;
        if (!AI_EvalFunc_TIMEElapsed_C(v3, X))
        {
            v21 = TIM_gf_MainClock - TIM_gf_LockTime;
            Xa = (v21 - (v13->f_time_flash + v13->des_f_time_on)) / (v13->des_f_flash_time / 4.0f) * 1.570796f + 4.712389f;
            v27 = sinf(Xa);
            Xb = -v27;
            v32 = AI_EvalFunc_COLORBlend_C(v13->c_base_constante, v13->c_off_constante, Xb);
            AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v32, 1);
            return 1000;
        }
        goto LABEL_42;
    }
LABEL_31:
    AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v13->c_off_constante, 1);
    return 1000;
}

int Switch_s1_exec_on_off()
{
    BOOL v0; // eax
    float v2; // [esp+4h] [ebp-CCh]
    float v3; // [esp+8h] [ebp-C8h]
    tScriptStruct_Switch* v4; // [esp+Ch] [ebp-C4h]
    tGameObjectMain* v5; // [esp+1Ch] [ebp-B4h]
    tGameObjectMain* v6; // [esp+20h] [ebp-B0h]
    tGameObjectMain* v7; // [esp+24h] [ebp-ACh]
    tGameObjectMain* v8; // [esp+2Ch] [ebp-A4h]
    tGameObjectMain* v9; // [esp+30h] [ebp-A0h]
    tGameObjectMain* v10; // [esp+34h] [ebp-9Ch]
    tGameObjectMain* v11; // [esp+3Ch] [ebp-94h]
    tGameObjectMain* v12; // [esp+40h] [ebp-90h]
    tGameObjectMain* v13; // [esp+44h] [ebp-8Ch]
    tGameObjectMain* v14; // [esp+4Ch] [ebp-84h]
    tGameObjectMain* v15; // [esp+50h] [ebp-80h]
    tGameObjectMain* v16; // [esp+54h] [ebp-7Ch]
    tGameObjectMain* v17; // [esp+5Ch] [ebp-74h]
    tGameObjectMain* v18; // [esp+60h] [ebp-70h]
    tGameObjectMain* v19; // [esp+64h] [ebp-6Ch]
    tGameObjectMain* v20; // [esp+6Ch] [ebp-64h]
    tGameObjectMain* v21; // [esp+70h] [ebp-60h]
    tGameObjectMain* v22; // [esp+74h] [ebp-5Ch]
    tMessage Src; // [esp+90h] [ebp-40h] BYREF
    tScriptStruct_Switch* v25; // [esp+C8h] [ebp-8h]
    BOOL v26; // [esp+CCh] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_Switch*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v25 = v4;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Switch_s1_exec_on_off_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Switch_s1_exec_on_off_univVars_Flag_speciaux_sauve,
            (void*)Switch_s1_exec_on_off_univVars,
            4488);
    }
    if (v25->i_actif)
    {
        v22 = (tGameObjectMain*)v25->des_o_actor_who_allow_it;
        if (v22)
        {
            if ((unsigned int)v22 <= 2)
                v22 = AI_MainActor[(uint32_t)v22 - 1];
            v21 = v22;
        }
        else
        {
            v21 = 0;
        }
        if (v21)
        {
            v20 = (tGameObjectMain*)v25->des_o_actor_who_allow_it;
            if (v20)
            {
                if ((unsigned int)v20 <= 2)
                    v20 = AI_MainActor[(uint32_t)v20 - 1];
                v0 = AI_EvalFunc_OBJ_CapaTest_C(v20, 0x100);
            }
            else
            {
                v0 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x100);
            }
            if (!v0)
            {
                v25->i_actif = 0;
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                return 1000;
            }
        }
        v3 = (float)v25->des_i_flash_nb;
        v2 = v25->des_f_flash_time * v3;
        if (AI_EvalFunc_TIMEElapsed_C(v25->f_time_flash, v2))
            v25->f_time_flash = TIM_gf_MainClock - TIM_gf_LockTime;
        if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
        {
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 256);
            v19 = (tGameObjectMain*)v25->o_acteur_capa;
            if (v19)
            {
                if ((unsigned int)v19 <= 2)
                    v19 = AI_MainActor[(uint32_t)v19 - 1];
                v18 = v19;
            }
            else
            {
                v18 = 0;
            }
            if (v18)
            {
                v17 = (tGameObjectMain*)v25->o_acteur_capa;
                if (v17)
                {
                    if ((unsigned int)v17 <= 2)
                        v17 = AI_MainActor[(uint32_t)v17 - 1];
                    AI_EvalFunc_OBJ_CapaSet_C(v17, 0, v25->i_capa_a_changer);
                }
                else
                {
                    AI_EvalFunc_OBJ_CapaSet_C(0, 0, v25->i_capa_a_changer);
                }
            }
        }
        else
        {
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(256, 0);
            v16 = (tGameObjectMain*)v25->o_acteur_capa;
            if (v16)
            {
                if ((unsigned int)v16 <= 2)
                    v16 = AI_MainActor[(uint32_t)v16 - 1];
                v15 = v16;
            }
            else
            {
                v15 = 0;
            }
            if (v15)
            {
                v14 = (tGameObjectMain*)v25->o_acteur_capa;
                if (v14)
                {
                    if ((unsigned int)v14 <= 2)
                        v14 = AI_MainActor[(uint32_t)v14 - 1];
                    AI_EvalFunc_OBJ_CapaSet_C(v14, v25->i_capa_a_changer, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_CapaSet_C(0, v25->i_capa_a_changer, 0);
                }
            }
        }
        v13 = (tGameObjectMain*)v25->des_o_ascenseur;
        if (v13)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            v12 = v13;
        }
        else
        {
            v12 = 0;
        }
        if (v12)
        {
            Src.msgID = 51;
            Src.Int1 = v25->des_i_etage;
            v11 = (tGameObjectMain*)v25->des_o_ascenseur;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v11 = AI_MainActor[(uint32_t)v11 - 1];
                AI_EvalFunc_MSGSend_C(v11, &Src);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &Src);
            }
        }
        v25->i_actif = 0;
    }
    if (v25->des_f_time_on != 0.0
        && v25->f_time_actif != 0.0
        && AI_EvalFunc_TIMEElapsed_C(v25->f_time_actif, v25->des_f_time_on))
    {
        if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
        {
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 256);
            v10 = (tGameObjectMain*)v25->o_acteur_capa;
            if (v10)
            {
                if ((unsigned int)v10 <= 2)
                    v10 = AI_MainActor[(uint32_t)v10 - 1];
                v9 = v10;
            }
            else
            {
                v9 = 0;
            }
            if (v9)
            {
                v8 = (tGameObjectMain*)v25->o_acteur_capa;
                if (v8)
                {
                    if ((unsigned int)v8 <= 2)
                        v8 = AI_MainActor[(uint32_t)v8 - 1];
                    AI_EvalFunc_OBJ_CapaSet_C(v8, 0, v25->i_capa_a_changer);
                }
                else
                {
                    AI_EvalFunc_OBJ_CapaSet_C(0, 0, v25->i_capa_a_changer);
                }
            }
        }
        else
        {
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x100, 0);
            v7 = (tGameObjectMain*)v25->o_acteur_capa;
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                v6 = v7;
            }
            else
            {
                v6 = 0;
            }
            if (v6)
            {
                v5 = (tGameObjectMain*)v25->o_acteur_capa;
                if (v5)
                {
                    if ((unsigned int)v5 <= 2)
                        v5 = AI_MainActor[(uint32_t)v5 - 1];
                    AI_EvalFunc_OBJ_CapaSet_C(v5, v25->i_capa_a_changer, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_CapaSet_C(0, v25->i_capa_a_changer, 0);
                }
            }
        }
        v25->f_time_actif = 0.0;
    }
    v26 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100);
    if ((!v26 || v25->des_i_on) && (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100) || !v25->des_i_on))
    {
        if (v25->des_SF_on_off > 0)
            AI_EvalFunc_ARRBitClear_C((int*)(Switch_s1_exec_on_off_univVars_Flag_speciaux_sauve + 4), v25->des_SF_on_off);
    }
    else if (v25->des_SF_on_off > 0)
    {
        AI_EvalFunc_ARRBitSet_C((int*)(Switch_s1_exec_on_off_univVars_Flag_speciaux_sauve + 4), v25->des_SF_on_off);
    }
    return 1000;
}


int Switch_s1_exec_magnet_gene()
{
    tGameObjectMain* v0; // eax
    tGameObjectMain* v1; // eax
    float v3; // [esp+0h] [ebp-220h]
    float v4; // [esp+4h] [ebp-21Ch]
    float v5; // [esp+8h] [ebp-218h]
    float v6; // [esp+Ch] [ebp-214h]
    float v7; // [esp+10h] [ebp-210h]
    float v8; // [esp+14h] [ebp-20Ch]
    tScriptStruct_Switch* v9; // [esp+18h] [ebp-208h]
    tGameObjectMain* v10; // [esp+20h] [ebp-200h]
    tGameObjectMain* v11; // [esp+28h] [ebp-1F8h]
    tGameObjectMain* v12; // [esp+40h] [ebp-1E0h]
    tGameObjectMain* a3; // [esp+44h] [ebp-1DCh]
    tGameObjectMain* v14; // [esp+48h] [ebp-1D8h]
    tGameObjectMain* v15; // [esp+50h] [ebp-1D0h]
    Vec3 v16; // [esp+7Ch] [ebp-1A4h] BYREF
    float v17; // [esp+88h] [ebp-198h]
    float v18; // [esp+8Ch] [ebp-194h]
    float v19; // [esp+90h] [ebp-190h]
    Vec3 v20; // [esp+94h] [ebp-18Ch] BYREF
    Vec3 v21; // [esp+A0h] [ebp-180h] BYREF
    tGameObjectMain* v22; // [esp+B0h] [ebp-170h]
    Vec3 v23; // [esp+B4h] [ebp-16Ch] BYREF
    Vec3 v24; // [esp+C0h] [ebp-160h]
    Vec3 v25; // [esp+CCh] [ebp-154h] BYREF
    Vec3 v26; // [esp+D8h] [ebp-148h] BYREF
    Vec3 v27; // [esp+E4h] [ebp-13Ch] BYREF
    Vec3 pos; // [esp+F0h] [ebp-130h] BYREF
    Vec3 v29; // [esp+FCh] [ebp-124h] BYREF
    Vec3 v30; // [esp+108h] [ebp-118h] BYREF
    Vec3 dest; // [esp+114h] [ebp-10Ch] BYREF
    Vec3 v32; // [esp+120h] [ebp-100h] BYREF
    float v33; // [esp+12Ch] [ebp-F4h]
    float v34; // [esp+130h] [ebp-F0h]
    float v35; // [esp+134h] [ebp-ECh]
    Vec3 v36; // [esp+138h] [ebp-E8h]
    Vec3 v37; // [esp+144h] [ebp-DCh] BYREF
    Vec3 v38; // [esp+154h] [ebp-CCh] BYREF
    Vec3 v39; // [esp+160h] [ebp-C0h] BYREF
    Vec3 v40; // [esp+16Ch] [ebp-B4h] BYREF
    Vec3 v41; // [esp+178h] [ebp-A8h]
    Vec3 v42; // [esp+188h] [ebp-98h] BYREF
    Vec3 v43; // [esp+194h] [ebp-8Ch] BYREF
    tGameObjectMain* v44; // [esp+1A0h] [ebp-80h]
    Vec3 v45; // [esp+1A4h] [ebp-7Ch]
    Vec3 a2; // [esp+1B0h] [ebp-70h] BYREF
    Vec3 v47; // [esp+1BCh] [ebp-64h] BYREF
    Vec3 v48; // [esp+1C8h] [ebp-58h]
    tScriptStruct_Switch* v49; // [esp+1D4h] [ebp-4Ch]
    Vec3 v50; // [esp+1DCh] [ebp-44h]
    Vec3 v51; // [esp+1E8h] [ebp-38h] BYREF
    Vec3 v52; // [esp+1F4h] [ebp-2Ch] BYREF
    Vec3 v53; // [esp+204h] [ebp-1Ch]
    Vec3 v54; // [esp+210h] [ebp-10h]

    v24.x = 1.0;
    v24.y = 0.0;
    v24.z = 0.0;
    v39.x = 1.0;
    v39.y = 0.0;
    v39.z = 0.0;
    a2.x = 1.0;
    a2.y = 0.0;
    a2.z = 0.0;
    pos.x = 1.0;
    pos.y = 0.0;
    pos.z = 0.0;
    v20.x = 1.0;
    v20.y = 0.0;
    v20.z = 0.0;
    v36.x = 1.0;
    v36.y = 0.0;
    v36.z = 0.0;
    v37.x = 1.0;
    v37.y = 0.0;
    v37.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v9 = (tScriptStruct_Switch*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v9 = 0;
    v49 = v9;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Switch_s1_exec_magnet_gene_DUP_Magnet_de_base_gao = AI_ul_RealResolveOneRefObject(0x3500923Du);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v25);
    v20 = v25;
    v48.x = 0.0;
    v48.y = 0.1f;
    v48.z = 0.0;
    v39.x = 0.0;
    v39.y = 0.1f;
    v39.z = 0.0;
    AI_EvalFunc_MATHVecLocalToGlobal_C(AI_gpst_CurrentGameObject, &v39, &v43);
    MATH_VecAddVec(&v16, &v20, &v43);
    pos = v16;
    v0 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)Switch_s1_exec_magnet_gene_DUP_Magnet_de_base_gao, &pos);
    v49->o_magnet = (int)v0;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v32);
    MATH_VecInvert(&v51, &v32);
    v15 = (tGameObjectMain*)v49->o_magnet;
    if (v15)
    {
        if ((unsigned int)v15 <= 2)
            v15 = AI_MainActor[(uint32_t)v15 - 1];
        AI_EvalFunc_OBJ_SightSet_C(v15, &v51);
    }
    else
    {
        AI_EvalFunc_OBJ_SightSet_C(0, &v51);
    }
    v22 = AI_gpst_CurrentGameObject;
    v14 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v14 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        a3 = v14;
    }
    else
    {
        a3 = 0;
    }
    v12 = (tGameObjectMain*)v49->o_magnet;
    if (v12)
    {
        if ((unsigned int)v12 <= 2)
            v12 = AI_MainActor[(uint32_t)v12 - 1];
        v1 = AI_EvalFunc_OBJ_HierarchySet_C(v12, a3);
    }
    else
    {
        v1 = AI_EvalFunc_OBJ_HierarchySet_C(0, a3);
    }
    v44 = v1;
    v50.x = 0.0;
    v50.y = -1.0;
    v50.z = 0.0;
    v26.x = 0.0 * 0.5;
    v26.y = -1.0 * 0.5;
    v26.z = v26.x;
    v41.x = 1.0;
    v41.y = 0.0;
    v41.z = 0.0;
    v21.x = 1.0 * 0.5;
    v21.y = v26.x;
    v21.z = v26.x;
    v54.x = 0.0;
    v54.y = 0.0;
    v54.z = 1.0;
    v40.x = 0.0 * 1.5;
    v40.y = v40.x;
    v40.z = 1.0 * 1.5;
    MATH_VecAddVec(&v23, &v21, &v40);
    MATH_SubstractVec(&dest, &v26, &v23);
    v24 = dest;
    v30.x = 0.0 * 0.5;
    v30.y = 1.0 * 0.5;
    v30.z = v30.x;
    v45.x = 1.0;
    v45.y = 0.0;
    v45.z = 0.0;
    v38.x = v30.y;
    v38.y = v30.x;
    v38.z = v30.x;
    v53.x = 0.0;
    v53.y = 0.0;
    v53.z = 1.0;
    v42.x = v30.x;
    v42.y = v30.x;
    v42.z = v30.y;
    MATH_VecAddVec(&v27, &v38, &v42);
    MATH_VecAddVec(&v29, &v30, &v27);
    v36 = v29;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v47);
    v17 = v47.x * 0.44999999f;
    v18 = v47.y * 0.44999999f;
    v19 = v47.z * 0.44999999f;
    v24.x = v24.x + v17;
    v24.y = v24.y + v18;
    v24.z = v24.z + v19;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v52);
    v33 = v52.x * 0.44999999f;
    v34 = v52.y * 0.44999999f;
    v35 = v52.z * 0.44999999f;
    v36.x = v36.x + v33;
    v36.y = v36.y + v34;
    v36.z = v36.z + v35;
    if (v24.x > (double)v36.x)
        v8 = v36.x;
    else
        v8 = v24.x;
    v37.x = v8;
    if (v24.y > (double)v36.y)
        v7 = v36.y;
    else
        v7 = v24.y;
    v37.y = v7;
    if (v24.z > (double)v36.z)
        v6 = v36.z;
    else
        v6 = v24.z;
    v37.z = v6;
    if (v24.x < (double)v36.x)
        v5 = v36.x;
    else
        v5 = v24.x;
    a2.x = v5;
    if (v24.y < (double)v36.y)
        v4 = v36.y;
    else
        v4 = v24.y;
    a2.y = v4;
    if (v24.z < (double)v36.z)
        v3 = v36.z;
    else
        v3 = v24.z;
    a2.z = v3;
    v11 = (tGameObjectMain*)v49->o_magnet;
    if (v11)
    {
        if ((unsigned int)v11 <= 2)
            v11 = AI_MainActor[(uint32_t)v11 - 1];
        AI_EvalFunc_BVMinSet_C(v11, &v37);
    }
    else
    {
        AI_EvalFunc_BVMinSet_C(0, &v37);
    }
    v10 = (tGameObjectMain*)v49->o_magnet;
    if (v10)
    {
        if ((unsigned int)v10 <= 2)
            v10 = AI_MainActor[(uint32_t)v10 - 1];
        AI_EvalFunc_BVMaxSet_C(v10, &a2);
    }
    else
    {
        AI_EvalFunc_BVMaxSet_C(0, &a2);
    }
    return 1000;
}

int Switch_s1_set_action()
{
    char* v0; // eax
    char* v1; // eax
    int result; // eax
    float v3; // [esp+4h] [ebp-200h]
    float v4; // [esp+8h] [ebp-1FCh]
    double X; // [esp+Ch] [ebp-1F8h]
    tScriptStruct_Switch* v6; // [esp+18h] [ebp-1ECh]
    tAI* v7; // [esp+1Ch] [ebp-1E8h]
    tGameObjectMain* v8; // [esp+20h] [ebp-1E4h]
    tGameObjectMain* v9; // [esp+24h] [ebp-1E0h]
    tAI* v10; // [esp+28h] [ebp-1DCh]
    tGameObjectMain* v11; // [esp+2Ch] [ebp-1D8h]
    tGameObjectMain* v12; // [esp+30h] [ebp-1D4h]
    tAI* v13; // [esp+34h] [ebp-1D0h]
    tGameObjectMain* v14; // [esp+38h] [ebp-1CCh]
    tGameObjectMain* v15; // [esp+3Ch] [ebp-1C8h]
    tAI* v16; // [esp+40h] [ebp-1C4h]
    tGameObjectMain* v17; // [esp+44h] [ebp-1C0h]
    tGameObjectMain* v18; // [esp+48h] [ebp-1BCh]
    tAI* v19; // [esp+4Ch] [ebp-1B8h]
    tGameObjectMain* v20; // [esp+50h] [ebp-1B4h]
    tGameObjectMain* v21; // [esp+54h] [ebp-1B0h]
    tAI* v22; // [esp+58h] [ebp-1ACh]
    tGameObjectMain* v23; // [esp+5Ch] [ebp-1A8h]
    tGameObjectMain* v24; // [esp+60h] [ebp-1A4h]
    tAI* v25; // [esp+64h] [ebp-1A0h]
    tGameObjectMain* v26; // [esp+68h] [ebp-19Ch]
    tGameObjectMain* v27; // [esp+6Ch] [ebp-198h]
    tAI* v28; // [esp+70h] [ebp-194h]
    tGameObjectMain* v29; // [esp+74h] [ebp-190h]
    tGameObjectMain* v30; // [esp+78h] [ebp-18Ch]
    tAI* v31; // [esp+7Ch] [ebp-188h]
    tGameObjectMain* v32; // [esp+80h] [ebp-184h]
    tGameObjectMain* v33; // [esp+84h] [ebp-180h]
    tAI* v34; // [esp+88h] [ebp-17Ch]
    tGameObjectMain* v35; // [esp+8Ch] [ebp-178h]
    tGameObjectMain* v36; // [esp+90h] [ebp-174h]
    tAI* v37; // [esp+94h] [ebp-170h]
    tGameObjectMain* v38; // [esp+98h] [ebp-16Ch]
    tGameObjectMain* v39; // [esp+9Ch] [ebp-168h]
    tAI* v40; // [esp+A0h] [ebp-164h]
    tGameObjectMain* v41; // [esp+A4h] [ebp-160h]
    tGameObjectMain* v42; // [esp+A8h] [ebp-15Ch]
    tAI* v43; // [esp+ACh] [ebp-158h]
    tGameObjectMain* v44; // [esp+B0h] [ebp-154h]
    tGameObjectMain* v45; // [esp+B4h] [ebp-150h]
    tAI* v46; // [esp+B8h] [ebp-14Ch]
    tGameObjectMain* v47; // [esp+BCh] [ebp-148h]
    tGameObjectMain* v48; // [esp+C0h] [ebp-144h]
    tAI* v49; // [esp+C4h] [ebp-140h]
    tGameObjectMain* v50; // [esp+C8h] [ebp-13Ch]
    tGameObjectMain* v51; // [esp+CCh] [ebp-138h]
    tAI* v52; // [esp+D0h] [ebp-134h]
    tGameObjectMain* v53; // [esp+D4h] [ebp-130h]
    tGameObjectMain* v54; // [esp+D8h] [ebp-12Ch]
    tAI* v55; // [esp+DCh] [ebp-128h]
    tGameObjectMain* v56; // [esp+E0h] [ebp-124h]
    tGameObjectMain* v57; // [esp+E4h] [ebp-120h]
    tAI* v58; // [esp+E8h] [ebp-11Ch]
    tGameObjectMain* v59; // [esp+ECh] [ebp-118h]
    tGameObjectMain* v60; // [esp+F0h] [ebp-114h]
    tAI* v61; // [esp+F4h] [ebp-110h]
    tGameObjectMain* v62; // [esp+F8h] [ebp-10Ch]
    tGameObjectMain* v63; // [esp+FCh] [ebp-108h]
    tGameObjectMain* v64; // [esp+110h] [ebp-F4h]
    tGameObjectMain* v65; // [esp+114h] [ebp-F0h]
    tGameObjectMain* v66; // [esp+118h] [ebp-ECh]
    tGameObjectMain* v67; // [esp+120h] [ebp-E4h]
    tGameObjectMain* v68; // [esp+124h] [ebp-E0h]
    tGameObjectMain* v69; // [esp+128h] [ebp-DCh]
    double v70; // [esp+12Ch] [ebp-D8h]
    int v71; // [esp+150h] [ebp-B4h]
    Vec3 v72; // [esp+154h] [ebp-B0h] BYREF
    Vec3 dest; // [esp+164h] [ebp-A0h] BYREF
    int v74; // [esp+170h] [ebp-94h]
    float v76; // [esp+178h] [ebp-8Ch]
    float v77; // [esp+17Ch] [ebp-88h]
    int v78; // [esp+180h] [ebp-84h]
    tGameObjectMain* v79; // [esp+184h] [ebp-80h]
    Vec3 v80; // [esp+190h] [ebp-74h] BYREF
    float v81; // [esp+1B0h] [ebp-54h]
    Vec3 a2; // [esp+1C0h] [ebp-44h] BYREF
    int v83; // [esp+1D0h] [ebp-34h]
    tAI* v84; // [esp+1D8h] [ebp-2Ch]
    tScriptStruct_Switch* v85; // [esp+1E0h] [ebp-24h]
    float v86; // [esp+1E4h] [ebp-20h]
    Vec3 a3; // [esp+1E8h] [ebp-1Ch] BYREF
    int v89; // [esp+1F8h] [ebp-Ch]
    float v90; // [esp+200h] [ebp-4h]

    v80.x = 1.0;
    v80.y = 0.0;
    v80.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_Switch*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v85 = v6;
    v89 = 0;
    v77 = 0.0;
    v83 = 0;
    v86 = 0.0;
    v79 = 0;
    v76 = 0.0;
    v84 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Switch_s1_set_action_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        Switch_s1_set_action_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&Switch_s1_set_action_univVars_Flag_speciaux_sauve, (void*)Switch_s1_set_action_univVars, 4488);
        Switch_s1_set_action__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Switch_s1_set_action_GST_Fight_Manager_gao = AI_ul_RealResolveOneRefObject(0x9E002FFA);
        AI2C_ResolveBuffer(
            &Switch_s1_set_action_GST_Fight_Manager_gao_i_Evil_NbActor,
            (void*)Switch_s1_set_action_GST_Fight_Manager_gao,
            2624);
        AI2C_ResolveBuffer(
            &fct_Switch_s1_set_action_Switch_s1_exec_set_action_current_pm_mode,
            (void*)Switch_s1_set_action__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(
            &fct_Switch_s1_set_action_Switch_s1_exec_set_action_current_pm_priority,
            (void*)Switch_s1_set_action__PJ_Sally_gao,
            576);
        AI2C_ResolveBuffer(&Switch_s1_set_action_univVars_objet_inventaire, (void*)Switch_s1_set_action_univVars, 1480);
        AI2C_ResolveBuffer(
            &fct_Switch_s1_set_action_GST_Board_panel_gao_i_force_interface,
            (void*)Switch_s1_set_action_GST_Board_panel_gao,
            3292);
        AI2C_ResolveBuffer(&Switch_s1_set_action_univVars_Perso_ID_equipement, (void*)Switch_s1_set_action_univVars, 40);
        AI2C_ResolveBuffer(&Switch_s1_set_action_univVars_Perso_ID_Companion, (void*)Switch_s1_set_action_univVars, 1040);
        fct_Switch_s1_set_action_Switch_s1_exec_set_action = (int)AI_ul_RealResolveOneRefFunction(0x9E002DFC);
    }
    v84 = (tAI*)Switch_s1_set_action_GST_Board_panel_gao;
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x10))
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x20, 0x10);
    else
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x20);
    if (v85->des_SF_on_off)
    {
        if (v85->des_SF_on_off <= 0)
            v89 = 0;
        else
            v89 = AI_EvalFunc_ARRBitTest_C((int*)(Switch_s1_set_action_univVars_Flag_speciaux_sauve + 4), v85->des_SF_on_off);
    }
    else
    {
        v89 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100);
    }
    if (v85->des_i_on)
    {
        X = (double)(v89 - 1);
        v4 = (float)fabs(X);
        v70 = v4 + MATH_gd_magicDoubleForRound;
        v89 = (*(uint32_t *)&v70);
    }
    if (v89)
    {
        if (!AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
        {
            v85->f_time_flash = TIM_gf_MainClock - TIM_gf_LockTime;
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x100, 0);
        }
        v69 = (tGameObjectMain*)v85->o_acteur_capa;
        if (v69)
        {
            if ((unsigned int)v69 <= 2)
                v69 = AI_MainActor[(uint32_t)v69 - 1];
            v68 = v69;
        }
        else
        {
            v68 = 0;
        }
        if (v68)
        {
            v67 = (tGameObjectMain*)v85->o_acteur_capa;
            if (v67)
            {
                if ((unsigned int)v67 <= 2)
                    v67 = AI_MainActor[(uint32_t)v67 - 1];
                AI_EvalFunc_OBJ_CapaSet_C(v67, v85->i_capa_a_changer, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_CapaSet_C(0, v85->i_capa_a_changer, 0);
            }
        }
    }
    else
    {
        if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
        {
            v85->f_time_flash = TIM_gf_MainClock - TIM_gf_LockTime;
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
        }
        v66 = (tGameObjectMain*)v85->o_acteur_capa;
        if (v66)
        {
            if ((unsigned int)v66 <= 2)
                v66 = AI_MainActor[(uint32_t)v66 - 1];
            v65 = v66;
        }
        else
        {
            v65 = 0;
        }
        if (v65)
        {
            v64 = (tGameObjectMain*)v85->o_acteur_capa;
            if (v64)
            {
                if ((unsigned int)v64 <= 2)
                    v64 = AI_MainActor[(uint32_t)v64 - 1];
                AI_EvalFunc_OBJ_CapaSet_C(v64, 0, v85->i_capa_a_changer);
            }
            else
            {
                AI_EvalFunc_OBJ_CapaSet_C(0, 0, v85->i_capa_a_changer);
            }
        }
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Switch_s1_set_action__PJ_Sally_gao, &a3);
    MATH_SubstractVec(&dest, &a2, &a3);
    v80.x = dest.x;
    v80.y = dest.y;
    v77 = dest.z;
    v80.z = 0.0;
    v71 = 0;
    v76 = MATH_VecLen(&v80);
    if (v85->des_i_interdire_en_fight && *(uint32_t*)Switch_s1_set_action_GST_Fight_Manager_gao_i_Evil_NbActor)
        goto LABEL_236;
    if (v76 >= 1.2
        || (AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v72), v90 = v72.z * v80.z + v72.y * v80.y + v80.x * v72.x, v90 >= 0.0)
        || (v81 = AI_EvalFunc_DYNSpeedGet_C((tGameObjectMain*)Switch_s1_set_action__PJ_Sally_gao), v81 >= 1.5)
        || v77 <= 0.2
        || v77 >= 2.0)
    {
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 16);
        v85->i_change_item = 0;
    }
    else
    {
        v74 = (unsigned __int16)*(uint32_t*)(Switch_s1_set_action__PJ_Sally_gao + 4);
        if ((v74 & 1) != 0)
        {
            if (*(uint32_t*)fct_Switch_s1_set_action_Switch_s1_exec_set_action_current_pm_mode
                && *(int*)fct_Switch_s1_set_action_Switch_s1_exec_set_action_current_pm_priority >= 20)
            {
                v86 = 0.0;
            }
            else
            {
                v86 = 1.0;
            }
        }
        else
        {
            v86 = 0.0;
        }
        if (v86 != 0.0)
            v85->o_compagnon_qui_agit = 0;
        if (v85->des_i_carte_d_access
            && *(uint32_t*)AI_EvalVarArray2_C(
                1,
                (char*)Switch_s1_set_action_univVars_objet_inventaire,
                v85->des_i_carte_d_access,
                8))
        {
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x10, 0);
            if (v86 != 0.0)
            {
                if (v84 == WOR_gst_Universe.univAIInstance)
                {
                    v61 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v63 = (tGameObjectMain*)v84;
                    if (v84)
                    {
                        if ((unsigned int)v84 <= 2)
                            v63 = AI_MainActor[(uint32_t)v84 - 1];
                        v62 = v63;
                    }
                    else
                    {
                        v62 = 0;
                    }
                    v61 = v62->extended->AIInstance;
                }
                v3 = (float)v85->i_prio_action;
                if (((tScriptStruct_GSTBoardPanel*)(v61->instanceVarDataCurrent))->i_sally_priority < (double)v3)
                {
                    if (v84 == WOR_gst_Universe.univAIInstance)
                    {
                        v58 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v60 = (tGameObjectMain*)v84;
                        if (v84)
                        {
                            if ((unsigned int)v84 <= 2)
                                v60 = AI_MainActor[(uint32_t)v84 - 1];
                            v59 = v60;
                        }
                        else
                        {
                            v59 = 0;
                        }
                        v58 = v59->extended->AIInstance;
                    }
                    ((tScriptStruct_GSTBoardPanel*)(v58->instanceVarDataCurrent))->o_sally_contextuel = (int)AI_gpst_CurrentGameObject;
                    if (v84 == WOR_gst_Universe.univAIInstance)
                    {
                        v55 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v57 = (tGameObjectMain*)v84;
                        if (v84)
                        {
                            if ((unsigned int)v84 <= 2)
                                v57 = AI_MainActor[(uint32_t)v84 - 1];
                            v56 = v57;
                        }
                        else
                        {
                            v56 = 0;
                        }
                        v55 = v56->extended->AIInstance;
                    }
                    ((tScriptStruct_GSTBoardPanel*)(v55->instanceVarDataCurrent))->i_sally_priority = (float)v85->i_prio_action;
                    if (v84 == WOR_gst_Universe.univAIInstance)
                    {
                        v52 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v54 = (tGameObjectMain*)v84;
                        if (v84)
                        {
                            if ((unsigned int)v84 <= 2)
                                v54 = AI_MainActor[(uint32_t)v84 - 1];
                            v53 = v54;
                        }
                        else
                        {
                            v53 = 0;
                        }
                        v52 = v53->extended->AIInstance;
                    }
                    ((tScriptStruct_GSTBoardPanel*)(v52->instanceVarDataCurrent))->i_sally_action = 4;
                    if (v84 == WOR_gst_Universe.univAIInstance)
                    {
                        v49 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v51 = (tGameObjectMain*)v84;
                        if (v84)
                        {
                            if ((unsigned int)v84 <= 2)
                                v51 = AI_MainActor[(uint32_t)v84 - 1];
                            v50 = v51;
                        }
                        else
                        {
                            v50 = 0;
                        }
                        v49 = v50->extended->AIInstance;
                    }
                    ((tScriptStruct_GSTBoardPanel*)(v49->instanceVarDataCurrent))->v_sally_offset.x = 0.0;
                    if (v84 == WOR_gst_Universe.univAIInstance)
                    {
                        v46 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v48 = (tGameObjectMain*)v84;
                        if (v84)
                        {
                            if ((unsigned int)v84 <= 2)
                                v48 = AI_MainActor[(uint32_t)v84 - 1];
                            v47 = v48;
                        }
                        else
                        {
                            v47 = 0;
                        }
                        v46 = v47->extended->AIInstance;
                    }
                    ((tScriptStruct_GSTBoardPanel*)(v46->instanceVarDataCurrent))->v_sally_offset.y = 0.0;
                    if (v84 == WOR_gst_Universe.univAIInstance)
                    {
                        v43 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v45 = (tGameObjectMain*)v84;
                        if (v84)
                        {
                            if ((unsigned int)v84 <= 2)
                                v45 = AI_MainActor[(uint32_t)v84 - 1];
                            v44 = v45;
                        }
                        else
                        {
                            v44 = 0;
                        }
                        v43 = v44->extended->AIInstance;
                    }
                    ((tScriptStruct_GSTBoardPanel*)(v43->instanceVarDataCurrent))->v_sally_offset.z = v85->offset_z_regard;
                    *(uint32_t*)fct_Switch_s1_set_action_GST_Board_panel_gao_i_force_interface = 1;
                }
            }
            if (*(uint32_t*)AI_EvalVarArray2_C(
                1,
                (char*)Switch_s1_set_action_univVars_objet_inventaire,
                v85->des_i_carte_d_access,
                8)
                && !v85->i_change_item)
            {
                v0 = AI_EvalVarArray2_C(1, (char*)Switch_s1_set_action_univVars_Perso_ID_equipement, 0, 8);
                *(uint32_t*)v0 = v85->des_i_carte_d_access;
                v85->i_change_item = 1;
            }
            v1 = AI_EvalVarArray2_C(1, (char*)Switch_s1_set_action_univVars_Perso_ID_equipement, 0, 8);
            if (*(uint32_t*)v1 == v85->des_i_carte_d_access && v86 != 0.0)
            {
                if (v84 == WOR_gst_Universe.univAIInstance)
                {
                    v40 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v42 = (tGameObjectMain*)v84;
                    if (v84)
                    {
                        if ((unsigned int)v84 <= 2)
                            v42 = AI_MainActor[(uint32_t)v84 - 1];
                        v41 = v42;
                    }
                    else
                    {
                        v41 = 0;
                    }
                    v40 = v41->extended->AIInstance;
                }
                if (v85->i_prio_action > ((tScriptStruct_GSTBoardPanel*)(v40->instanceVarDataCurrent))->i_use_prio)
                {
                    if (v84 == WOR_gst_Universe.univAIInstance)
                    {
                        v37 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v39 = (tGameObjectMain*)v84;
                        if (v84)
                        {
                            if ((unsigned int)v84 <= 2)
                                v39 = AI_MainActor[(uint32_t)v84 - 1];
                            v38 = v39;
                        }
                        else
                        {
                            v38 = 0;
                        }
                        v37 = v38->extended->AIInstance;
                    }
                    ((tScriptStruct_GSTBoardPanel*)(v37->instanceVarDataCurrent))->o_use_object = (int)AI_gpst_CurrentGameObject;
                    if (v84 == WOR_gst_Universe.univAIInstance)
                    {
                        v34 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v36 = (tGameObjectMain*)v84;
                        if (v84)
                        {
                            if ((unsigned int)v84 <= 2)
                                v36 = AI_MainActor[(uint32_t)v84 - 1];
                            v35 = v36;
                        }
                        else
                        {
                            v35 = 0;
                        }
                        v34 = v35->extended->AIInstance;
                    }
                    ((tScriptStruct_GSTBoardPanel*)(v34->instanceVarDataCurrent))->i_use_prio = v85->i_prio_action;
                    if (v84 == WOR_gst_Universe.univAIInstance)
                    {
                        v31 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v33 = (tGameObjectMain*)v84;
                        if (v84)
                        {
                            if ((unsigned int)v84 <= 2)
                                v33 = AI_MainActor[(uint32_t)v84 - 1];
                            v32 = v33;
                        }
                        else
                        {
                            v32 = 0;
                        }
                        v31 = v32->extended->AIInstance;
                    }
                    ((tScriptStruct_GSTBoardPanel*)(v31->instanceVarDataCurrent))->i_use_action = 4;
                    if (v84 == WOR_gst_Universe.univAIInstance)
                    {
                        v28 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v30 = (tGameObjectMain*)v84;
                        if (v84)
                        {
                            if ((unsigned int)v84 <= 2)
                                v30 = AI_MainActor[(uint32_t)v84 - 1];
                            v29 = v30;
                        }
                        else
                        {
                            v29 = 0;
                        }
                        v28 = v29->extended->AIInstance;
                    }
                    ((tScriptStruct_GSTBoardPanel*)(v28->instanceVarDataCurrent))->f_use_offset = v85->offset_z_regard;
                }
            }
        }
        else
        {
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 16);
            if (v86 != 0.0)
            {
                if (v84 == WOR_gst_Universe.univAIInstance)
                {
                    v25 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v27 = (tGameObjectMain*)v84;
                    if (v84)
                    {
                        if ((unsigned int)v84 <= 2)
                            v27 = AI_MainActor[(uint32_t)v84 - 1];
                        v26 = v27;
                    }
                    else
                    {
                        v26 = 0;
                    }
                    v25 = v26->extended->AIInstance;
                }
                if (((tScriptStruct_GSTBoardPanel*)(v25->instanceVarDataCurrent))->i_sally_priority < 5.0)
                {
                    if (v84 == WOR_gst_Universe.univAIInstance)
                    {
                        v22 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v24 = (tGameObjectMain*)v84;
                        if (v84)
                        {
                            if ((unsigned int)v84 <= 2)
                                v24 = AI_MainActor[(uint32_t)v84 - 1];
                            v23 = v24;
                        }
                        else
                        {
                            v23 = 0;
                        }
                        v22 = v23->extended->AIInstance;
                    }
                    ((tScriptStruct_GSTBoardPanel*)(v22->instanceVarDataCurrent))->o_sally_contextuel = (int)AI_gpst_CurrentGameObject;
                    if (v84 == WOR_gst_Universe.univAIInstance)
                    {
                        v19 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v21 = (tGameObjectMain*)v84;
                        if (v84)
                        {
                            if ((unsigned int)v84 <= 2)
                                v21 = AI_MainActor[(uint32_t)v84 - 1];
                            v20 = v21;
                        }
                        else
                        {
                            v20 = 0;
                        }
                        v19 = v20->extended->AIInstance;
                    }
                    ((tScriptStruct_GSTBoardPanel*)(v19->instanceVarDataCurrent))->i_sally_priority = 5.0;
                    if (v84 == WOR_gst_Universe.univAIInstance)
                    {
                        v16 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v18 = (tGameObjectMain*)v84;
                        if (v84)
                        {
                            if ((unsigned int)v84 <= 2)
                                v18 = AI_MainActor[(uint32_t)v84 - 1];
                            v17 = v18;
                        }
                        else
                        {
                            v17 = 0;
                        }
                        v16 = v17->extended->AIInstance;
                    }
                    ((tScriptStruct_GSTBoardPanel*)(v16->instanceVarDataCurrent))->i_sally_action = 11;
                    if (v84 == WOR_gst_Universe.univAIInstance)
                    {
                        v13 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v15 = (tGameObjectMain*)v84;
                        if (v84)
                        {
                            if ((unsigned int)v84 <= 2)
                                v15 = AI_MainActor[(uint32_t)v84 - 1];
                            v14 = v15;
                        }
                        else
                        {
                            v14 = 0;
                        }
                        v13 = v14->extended->AIInstance;
                    }
                    ((tScriptStruct_GSTBoardPanel*)(v13->instanceVarDataCurrent))->v_sally_offset.x = 0.0;
                    if (v84 == WOR_gst_Universe.univAIInstance)
                    {
                        v10 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v12 = (tGameObjectMain*)v84;
                        if (v84)
                        {
                            if ((unsigned int)v84 <= 2)
                                v12 = AI_MainActor[(uint32_t)v84 - 1];
                            v11 = v12;
                        }
                        else
                        {
                            v11 = 0;
                        }
                        v10 = v11->extended->AIInstance;
                    }
                    ((tScriptStruct_GSTBoardPanel*)(v10->instanceVarDataCurrent))->v_sally_offset.y = 0.0;
                    if (v84 == WOR_gst_Universe.univAIInstance)
                    {
                        v7 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v9 = (tGameObjectMain*)v84;
                        if (v84)
                        {
                            if ((unsigned int)v84 <= 2)
                                v9 = AI_MainActor[(uint32_t)v84 - 1];
                            v8 = v9;
                        }
                        else
                        {
                            v8 = 0;
                        }
                        v7 = v8->extended->AIInstance;
                    }
                    ((tScriptStruct_GSTBoardPanel*)(v7->instanceVarDataCurrent))->v_sally_offset.z = v85->offset_z_regard;
                    *(uint32_t*)fct_Switch_s1_set_action_GST_Board_panel_gao_i_force_interface = 1;
                }
            }
        }
        v71 = 1;
    }
    if (!v85->des_i_vala_allowed || v71)
        goto LABEL_236;
    v79 = AI_gpst_CurrentGameObject;
    v83 = *(uint32_t*)Switch_s1_set_action_univVars_Perso_ID_Companion;
    if (v83 == -1 || v83 > 1)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    if (!v85->ai_action[v83])
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    v85->des_i_action_type = 2829;
    v85->f_vala_prio = (float)(v83 + 150);
    v85->des_i_acteur_choix = v83 << 24;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v85->v_vala_pos);
    v78 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Switch_s1_set_action_Switch_s1_exec_set_action);
    if (v78 == 1000)
        LABEL_236:
    result = 1000;
    else
        result = v78;
    return result;
}

int Switch_s1_exec_get_message()
{
    tScriptStruct_Switch* v1; // [esp+0h] [ebp-1A4h]
    tGameObjectMain* v2; // [esp+8h] [ebp-19Ch]
    tGameObjectMain* v3; // [esp+10h] [ebp-194h]
    Vec3* v4; // [esp+14h] [ebp-190h]
    Vec3* v5; // [esp+18h] [ebp-18Ch]
    Vec3* v6; // [esp+1Ch] [ebp-188h]
    tGameObjectMain* v7; // [esp+38h] [ebp-16Ch]
    tGameObjectMain* v8; // [esp+40h] [ebp-164h]
    tGameObjectMain* v9; // [esp+50h] [ebp-154h]
    tGameObjectMain* v10; // [esp+60h] [ebp-144h]
    tGameObjectMain* v11; // [esp+68h] [ebp-13Ch]
    float v12; // [esp+88h] [ebp-11Ch]
    float v13; // [esp+8Ch] [ebp-118h]
    int v14; // [esp+98h] [ebp-10Ch]
    BOOL v15; // [esp+A0h] [ebp-104h]
    tMessage Src; // [esp+C8h] [ebp-DCh] BYREF
    tGameObjectMain* v17; // [esp+FCh] [ebp-A8h]
    int v18; // [esp+100h] [ebp-A4h]
    float v19; // [esp+104h] [ebp-A0h]
    int v20; // [esp+108h] [ebp-9Ch]
    int v21; // [esp+10Ch] [ebp-98h]
    BOOL v22; // [esp+124h] [ebp-80h]
    int v23; // [esp+138h] [ebp-6Ch]
    int a4; // [esp+144h] [ebp-60h]
    tGameObjectMain* v25; // [esp+148h] [ebp-5Ch]
    tScriptStruct_Switch* v26; // [esp+150h] [ebp-54h]
    int v27; // [esp+160h] [ebp-44h]
    tMessage v28; // [esp+170h] [ebp-34h] BYREF

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Switch*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v26 = v1;
    v17 = 0;
    v18 = 0;
    v22 = 0;
    v25 = 0;
    v23 = 0;
    a4 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Switch_s1_exec_get_message_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Switch_s1_exec_get_message_univVars_Perso_ID_Companion,
            (void*)Switch_s1_exec_get_message_univVars,
            1040);
        AI2C_ResolveBuffer(
            &Switch_s1_exec_get_message_univVars_Perso_ID_actor,
            (void*)Switch_s1_exec_get_message_univVars,
            988);
        AI2C_ResolveBuffer(
            &Switch_s1_exec_get_message_univVars_Flag_speciaux_sauve,
            (void*)Switch_s1_exec_get_message_univVars,
            4488);
        Switch_s1_exec_get_message_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Switch_s1_exec_get_message_GST_Global_gao_o_actor_texte,
            (void*)Switch_s1_exec_get_message_GST_Global_gao,
            3616);
        AI2C_ResolveBuffer(
            &Switch_s1_exec_get_message_univVars_objet_inventaire,
            (void*)Switch_s1_exec_get_message_univVars,
            1480);
    }
    v23 = 0;
    v18 = 0;
    v14 = 0;
    a4 = -1;
    v25 = 0;
    v17 = AI_MainActor[1];
    if (v26->m_vala.msgID != 38)
        goto LABEL_32;
    v23 = 1;
    a4 = *(uint32_t*)Switch_s1_exec_get_message_univVars_Perso_ID_Companion;
    if (a4 == -1)
    {
        v23 = 0;
        goto LABEL_32;
    }
    v25 = *(tGameObjectMain**)(Switch_s1_exec_get_message_univVars_Perso_ID_actor + 4 * a4 + 4);
    if (v26->m_vala.f2 == 100.0)
    {
        v18 = 0;
        v14 = 0;
    }
    else if (v26->f_vala_prio == v26->m_vala.f2)
    {
        v18 = 1;
    }
    else
    {
        v18 = 2;
    }
    if ((unsigned __int8)v26->ai_action[a4] == 20)
    {
        v15 = AI_EvalFunc_ARRBitTest_C((int*)(Switch_s1_exec_get_message_univVars_Flag_speciaux_sauve + 4), 2199);
        v22 = AI_EvalFunc_ARRBitTest_C((int*)(Switch_s1_exec_get_message_univVars_Flag_speciaux_sauve + 4), 2864);
        if (!v15 && v22)
        {
            v17 = *(tGameObjectMain**)(Switch_s1_exec_get_message_GST_Global_gao_o_actor_texte + 4 * a4 + 4);
            Src.msgID = 30;
            Src.Int1 = 35;
            Src.Int2 = 10000;
            Src.f2 = 1.5;
            Src.v1.x = 0.0;
            Src.v1.y = 0.0;
            Src.v1.z = 0.0;
            Src.v2.x = 0.0;
            Src.v2.y = 0.0;
            Src.v2.z = 0.0;
            Src.gao = 0;
            Src.gao2 = v25;
            v11 = v17;
            if (v17)
            {
                if ((unsigned int)v17 <= 2)
                    v11 = AI_MainActor[(uint32_t)v17 - 1];
                AI_EvalFunc_MSGSend_C(v11, &Src);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &Src);
            }
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
    LABEL_32:
        if (v26->i_trigger_on)
        {
            v27 = AI_EvalFunc_OBJ_CustomBitsGet_C(AI_gpst_CurrentGameObject);
            v14 = v27 >> 8;
            AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(0, -1);
            if (v27 >> 8)
            {
                v23 = 1;
                a4 = ((v27 >> 8) & 7) - 1;
                v18 = ((v27 >> 8) & 8) >> 3;
                if ((v27 & 0x700) != 0)
                    v25 = *(tGameObjectMain**)(Switch_s1_exec_get_message_univVars_Perso_ID_actor + 4 * a4 + 4);
                else
                    v23 = 0;
                if (v26->i_trigger_on == 2 && (v27 & 0x1000) != 0)
                    v26->i_trigger_on = 1;
            }
        }
        if (!v23)
            return 1000;
        if (v18)
        {
            if (v18 == 1)
            {
                v26->m_vala.msgID = 6;
                v26->m_vala.gao2 = AI_gpst_CurrentGameObject;
                v26->m_vala.Int1 = 5;
                v26->m_vala.Int2 = (unsigned __int8)v26->ai_action[a4];
                v12 = 0.0;
                if (v26->m_vala.Int2 == 13
                    && v26->des_i_carte_d_access
                    && *(uint32_t*)AI_EvalVarArray2_C(
                        1,
                        (char*)Switch_s1_exec_get_message_univVars_objet_inventaire,
                        v26->des_i_carte_d_access,
                        a4))
                {
                    v19 = (float)v26->des_i_carte_d_access;
                    v20 = 0;
                    v21 = 0;
                    v12 = v19;
                }
                v13 = v26->f_delai_attente_apres_action;
                v6 = &v26->m_vala.v1;
                v26->m_vala.v1.x = v12;
                v6->y = v13;
                v6->z = 0.0;
                v4 = &v26->m_vala.v2;
                v5 = &v26->v_coup_cul_offset_sol;
                v26->m_vala.v2.x = v26->v_coup_cul_offset_sol.x;
                v4->y = v5->y;
                v4->z = v5->z;
                v26->m_vala.f2 = v26->des_f_vala_angle_belier;
                v26->m_vala.f1 = v26->des_f_agit_distmax;
                v3 = v25;
                if (v25)
                {
                    if ((unsigned int)v25 <= 2)
                        v3 = AI_MainActor[(uint32_t)v25 - 1];
                    AI_EvalFunc_MSGSend_C(v3, &v26->m_vala);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &v26->m_vala);
                }
                v26->i_vala_action_sent = 1;
                v26->f_start_action = TIM_gf_MainClock - TIM_gf_LockTime;
                AI_EvalFunc_OBJ_CapaSet_C_CURRENT(8, 0);
            }
            else
            {
                v26->m_vala.msgID = 6;
                v26->m_vala.Int1 = 0;
                v2 = v25;
                if (v25)
                {
                    if ((unsigned int)v25 <= 2)
                        v2 = AI_MainActor[(uint32_t)v25 - 1];
                    AI_EvalFunc_MSGSend_C(v2, &v26->m_vala);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &v26->m_vala);
                }
                v26->i_vala_action_sent = 1;
                v26->o_compagnon_qui_agit = 0;
            }
            return 1000;
        }
        if (v14)
        {
            Src.msgID = 6;
            Src.Int1 = 1;
            Src.Int2 = 0;
            Src.f2 = 8.0;
            Src.f1 = 0.0;
            Src.v1 = v26->v_vala_pos;
            Src.v2.x = 0.0;
            Src.v2.y = 0.0;
            Src.v2.z = 0.0;
            Src.gao = AI_gpst_CurrentGameObject;
            Src.gao2 = AI_gpst_CurrentGameObject;
            v9 = v25;
            if (v25)
            {
                if ((unsigned int)v25 <= 2)
                    v9 = AI_MainActor[(uint32_t)v25 - 1];
                AI_EvalFunc_MSGSend_C(v9, &Src);
                goto LABEL_52;
            }
        }
        else
        {
            Src.msgID = 6;
            Src.Int1 = 1;
            Src.Int2 = 3;
            Src.f2 = 8.0;
            Src.f1 = 0.0;
            Src.v1 = v26->v_vala_pos;
            Src.v2.x = 0.0;
            Src.v2.y = 0.0;
            Src.v2.z = 0.0;
            Src.gao = AI_gpst_CurrentGameObject;
            Src.gao2 = AI_gpst_CurrentGameObject;
            v8 = v25;
            if (v25)
            {
                if ((unsigned int)v25 <= 2)
                    v8 = AI_MainActor[(uint32_t)v25 - 1];
                AI_EvalFunc_MSGSend_C(v8, &Src);
                goto LABEL_52;
            }
        }
        AI_EvalFunc_MSGSend_C(0, &Src);
    LABEL_52:
        v26->i_vala_action_sent = 0;
        v26->i_vala_fini = 1;
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(2, 0);
        v26->o_compagnon_qui_agit = (int)v25;
        v28.msgID = 92;
        v28.Int1 = (unsigned __int8)v26->ai_action[a4];
        v28.Int2 = v26->des_i_carte_d_access;
        v7 = v25;
        if (v25)
        {
            if ((unsigned int)v25 <= 2)
                v7 = AI_MainActor[(uint32_t)v25 - 1];
            AI_EvalFunc_MSGSend_C(v7, &v28);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &v28);
        }
        return 1000;
    }
    if ((unsigned __int8)v26->ai_action[a4] != 13
        || !v26->des_i_carte_d_access
        || *(uint32_t*)AI_EvalVarArray2_C(
            1,
            (char*)Switch_s1_exec_get_message_univVars_objet_inventaire,
            v26->des_i_carte_d_access,
            a4))
    {
        goto LABEL_32;
    }
    v17 = *(tGameObjectMain**)(Switch_s1_exec_get_message_GST_Global_gao_o_actor_texte + 4 * a4 + 4);
    Src.msgID = 30;
    Src.Int1 = 26;
    Src.Int2 = 10000;
    Src.f2 = 1.5;
    Src.v1.x = 0.0;
    Src.v1.y = 0.0;
    Src.v1.z = 0.0;
    Src.v2.x = 0.0;
    Src.v2.y = 0.0;
    Src.v2.z = 0.0;
    Src.gao = 0;
    Src.gao2 = v25;
    v10 = v17;
    if (v17)
    {
        if ((unsigned int)v17 <= 2)
            v10 = AI_MainActor[(uint32_t)v17 - 1];
        AI_EvalFunc_MSGSend_C(v10, &Src);
    }
    else
    {
        AI_EvalFunc_MSGSend_C(0, &Src);
    }
    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
    return 1000;
}

int Switch_s1_exec_send_info()
{
    int result; // eax
    tScriptStruct_Switch* v1; // [esp+8h] [ebp-E0h]
    tGameObjectMain* v2; // [esp+10h] [ebp-D8h]
    tAI* v3; // [esp+14h] [ebp-D4h]
    tGameObjectMain* v4; // [esp+18h] [ebp-D0h]
    tGameObjectMain* v5; // [esp+1Ch] [ebp-CCh]
    tGameObjectMain* v6; // [esp+24h] [ebp-C4h]
    tGameObjectMain* v7; // [esp+28h] [ebp-C0h]
    tGameObjectMain* v8; // [esp+2Ch] [ebp-BCh]
    tGameObjectMain* v9; // [esp+30h] [ebp-B8h]
    tGameObjectMain* v10; // [esp+3Ch] [ebp-ACh]
    tGameObjectMain* v11; // [esp+40h] [ebp-A8h]
    tGameObjectMain* v12; // [esp+44h] [ebp-A4h]
    tGameObjectMain* v13; // [esp+48h] [ebp-A0h]
    tGameObjectMain* v14; // [esp+50h] [ebp-98h]
    tGameObjectMain* v15; // [esp+54h] [ebp-94h]
    tMessage Src; // [esp+90h] [ebp-58h] BYREF
    int v17; // [esp+C8h] [ebp-20h]
    tScriptStruct_Switch* v19; // [esp+D0h] [ebp-18h]
    tAI* v20; // [esp+D4h] [ebp-14h]
    unsigned int v21; // [esp+E4h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Switch*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v19 = v1;
    v17 = 0;
    v21 = 0;
    v20 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Switch_s1_exec_send_info_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Switch_s1_exec_send_info_univVars_Flag_speciaux_sauve,
            (void*)Switch_s1_exec_send_info_univVars,
            4488);
        Switch_s1_exec_send_info__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Switch_s1_exec_send_info_TXT_Sally_gao = AI_ul_RealResolveOneRefObject(0x7200064Eu);
        AI2C_ResolveBuffer(
            &Switch_s1_exec_send_info_univVars_Perso_ID_actor,
            (void*)Switch_s1_exec_send_info_univVars,
            988);
    }
    AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
    if (AI_EvalFunc_TIMEElapsed_C(v19->f_time_last_info, 4.0))
    {
        v19->f_time_last_info = TIM_gf_MainClock - TIM_gf_LockTime;
        if (v19->des_i_SF_allow_special_text)
        {
            if (v19->des_i_SF_allow_special_text <= 0)
            {
                v17 = 0;
            }
            else if (v19->des_i_SF_allow_special_text < 10000)
            {
                v17 = AI_EvalFunc_ARRBitTest_C(
                    (int*)(Switch_s1_exec_send_info_univVars_Flag_speciaux_sauve + 4),
                    v19->des_i_SF_allow_special_text);
            }
            else
            {
                v17 = 1
                    - AI_EvalFunc_ARRBitTest_C(
                        (int*)(Switch_s1_exec_send_info_univVars_Flag_speciaux_sauve + 4),
                        v19->des_i_SF_allow_special_text - 10000);
            }
        }
        else
        {
            v17 = 1;
        }
        v15 = v19->m_msg.gao;
        if (v15)
        {
            if ((unsigned int)v15 <= 2)
                v15 = AI_MainActor[(uint32_t)v15 - 1];
            v14 = v15;
        }
        else
        {
            v14 = 0;
        }
        if (v14 == (tGameObjectMain*)Switch_s1_exec_send_info__PJ_Sally_gao)
        {
            if (v17)
            {
                if (v19->des_i_sally_text_index >= 0)
                    v19->i_index_message = v19->des_i_sally_text_index;
            }
            Src.msgID = 30;
            Src.Int1 = v19->i_index_message;
            Src.Int2 = 0;
            Src.f2 = 4.0;
            Src.v1.x = 0.0;
            Src.v1.y = 0.0;
            Src.v1.z = 0.0;
            Src.v2.x = 0.0;
            Src.v2.y = 0.0;
            Src.v2.z = 0.0;
            Src.gao = 0;
            Src.gao2 = 0;
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)Switch_s1_exec_send_info_TXT_Sally_gao, &Src);
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            v13 = *(tGameObjectMain**)(Switch_s1_exec_send_info_univVars_Perso_ID_actor + 4);
            if (v13)
            {
                if ((unsigned int)v13 <= 2)
                    v13 = AI_MainActor[(uint32_t)v13 - 1];
                v12 = v13;
            }
            else
            {
                v12 = 0;
            }
            if (!v12)
                goto LABEL_40;
            v11 = v19->m_msg.gao;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v11 = AI_MainActor[(uint32_t)v11 - 1];
                v10 = v11;
            }
            else
            {
                v10 = 0;
            }
            if (v10 == *(tGameObjectMain**)(Switch_s1_exec_send_info_univVars_Perso_ID_actor + 4))
            {
                if (v17 && v19->des_i_peyj_text_index >= 0)
                    v19->i_index_message = v19->des_i_peyj_text_index;
            }
            else
            {
            LABEL_40:
                v9 = *(tGameObjectMain**)(Switch_s1_exec_send_info_univVars_Perso_ID_actor + 8);
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
                    goto LABEL_55;
                v7 = v19->m_msg.gao;
                if (v7)
                {
                    if ((unsigned int)v7 <= 2)
                        v7 = AI_MainActor[(uint32_t)v7 - 1];
                    v6 = v7;
                }
                else
                {
                    v6 = 0;
                }
                if (v6 == *(tGameObjectMain**)(Switch_s1_exec_send_info_univVars_Perso_ID_actor + 8))
                {
                    if (v17 && v19->des_i_militaire_text_index >= 0)
                        v19->i_index_message = v19->des_i_militaire_text_index;
                }
                else
                {
                LABEL_55:
                    v19->i_index_message = -1;
                }
            }
            if (v19->i_index_message != -1)
            {
                v20 = (tAI*)v19->m_msg.gao;
                if (v20 == WOR_gst_Universe.univAIInstance)
                {
                    v3 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v5 = (tGameObjectMain*)v20;
                    if (v20)
                    {
                        if ((unsigned int)v20 <= 2)
                            v5 = AI_MainActor[(uint32_t)v20 - 1];
                        v4 = v5;
                    }
                    else
                    {
                        v4 = 0;
                    }
                    v3 = v4->extended->AIInstance;
                }
                v21 = ((tScriptStruct_Switch*)(v3->instanceVarDataCurrent))->o_acteur_capa;
                Src.msgID = 30;
                Src.Int1 = v19->i_index_message;
                Src.Int2 = 0;
                Src.f2 = 4.0;
                Src.v1.x = 0.0;
                Src.v1.y = 0.0;
                Src.v1.z = 0.0;
                Src.v2.x = 0.0;
                Src.v2.y = 0.0;
                Src.v2.z = 0.0;
                Src.gao = 0;
                Src.gao2 = 0;
                v2 = (tGameObjectMain*)v21;
                if (v21)
                {
                    if (v21 <= 2)
                        v2 = AI_MainActor[v21 - 1];
                    AI_EvalFunc_MSGSend_C(v2, &Src);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &Src);
                }
            }
            result = 1000;
        }
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}