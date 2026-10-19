#include "ai_compiledscripts_inc.h"

//JeTeParle_jtp_revol
int JeTeParle_jtp_revol_GST_Board_panel_gao;

//JeTeParle_jtp_mamago_bricole
int JeTeParle_jtp_mamago_bricole_univVars;
int JeTeParle_jtp_mamago_bricole_univVars_Flag_speciaux_sauve;
int JeTeParle_jtp_mamago_bricole_GST_Board_panel_gao;

//JeTeParle_jtp_palet
int JeTeParle_jtp_palet_univVars;
int JeTeParle_jtp_palet_univVars_Flag_speciaux_sauve;
int JeTeParle_jtp_palet_GST_Board_panel_gao;

//JeTeParle_jtp_init
int JeTeParle_jtp_init_univVars;
int JeTeParle_jtp_init_univVars_Flag_speciaux_sauve;
int JeTeParle_jtp_init_INV_manager_gao;
int JeTeParle_jtp_init_INV_manager_gao_i_Animals_Nb;
int JeTeParle_jtp_init_INV_manager_gao_ao_Animals;
int JeTeParle_jtp_init_INV_manager_gao_ai_AnimalsData;
int fct_JeTeParle_jtp_init_JeTeParle_jtp_rottete;
int fct_JeTeParle_jtp_init_JeTeParle_jtp_mamago_presente_objets;
int fct_JeTeParle_jtp_init_JeTeParle_jtp_mamago_bricole_AB;
int fct_JeTeParle_jtp_init_JeTeParle_jtp_mamago_bricole;
int fct_JeTeParle_jtp_init_JeTeParle_jtp_palet_posperle;
int fct_JeTeParle_jtp_init_JeTeParle_jtp_palet;
int fct_JeTeParle_jtp_init_JeTeParle_jtp_revol_AB;
int fct_JeTeParle_jtp_init_JeTeParle_jtp_revol;
int fct_JeTeParle_jtp_init_JeTeParle_jtp_reflex;

//JeTeParle__jtp_check
int JeTeParle__jtp_check__PJ_Sally_gao;
int JeTeParle__jtp_check_GST_Board_panel_gao;
int JeTeParle__jtp_check__PJ_Sally_gao_current_pm_mode;
int JeTeParle__jtp_check__PJ_Sally_gao_current_pm_priority;
int JeTeParle__jtp_check_GST_Board_panel_gao_i_sally_priority;
int JeTeParle__jtp_check_GST_Board_panel_gao_o_sally_contextuel;
int JeTeParle__jtp_check_GST_Board_panel_gao_i_sally_action;
int JeTeParle__jtp_check_GST_Board_panel_gao_v_sally_offset;
int JeTeParle__jtp_check_GST_Board_panel_gao_i_force_interface;
int JeTeParle__jtp_check_univVars;
int JeTeParle__jtp_check_univVars_Flag_speciaux_sauve;
int JeTeParle__jtp_check_TXT_Sally_gao;
int JeTeParle__jtp_check_GST_Board_panel_gao_ai_dialog_consequences;
int JeTeParle__jtp_check_GST_Board_panel_gao_o_dialogue_actor;
int JeTeParle__jtp_check_GST_Board_panel_gao_at_dialog_questions;
int JeTeParle__jtp_check_GST_Board_panel_gao_at_dialog_reponses;
int JeTeParle__jtp_check_GST_Board_panel_gao_ai_dialog_grisage;

//JeTeParle_jtp_rottete
int JeTeParle_jtp_rottete__PJ_Sally_gao;
int fct_JeTeParle_jtp_rottete_JeTeParle_jtp_exec_regarde;

//JeTeParle_jtp_mamago_presente_objets
int JeTeParle_jtp_mamago_presente_objets_GST_Board_panel_gao;

//fct_JeTeParle_jtp_mamago_bricole_AB
int fct_JeTeParle_jtp_mamago_bricole_AB_JeTeParle_jtp_rottete;
int JeTeParle_jtp_mamago_bricole_AB__PJ_Sally_gao;

//JeTeParle_jtp_palet_posperle
int JeTeParle_jtp_palet_posperle_univVars;
int JeTeParle_jtp_palet_posperle_univVars_Flag_speciaux_sauve;

//Dialogue_dialogue_check
int Dialogue_dialogue_check__PJ_Sally_gao;
int Dialogue_dialogue_check_GST_Board_panel_gao;
int Dialogue_dialogue_check__PJ_Sally_gao_current_pm_mode;
int Dialogue_dialogue_check__PJ_Sally_gao_current_pm_priority;
int Dialogue_dialogue_check_GST_Board_panel_gao_i_sally_priority;
int Dialogue_dialogue_check_GST_Board_panel_gao_o_sally_contextuel;
int Dialogue_dialogue_check_GST_Board_panel_gao_i_sally_action;
int Dialogue_dialogue_check_GST_Board_panel_gao_v_sally_offset;
int Dialogue_dialogue_check_GST_Board_panel_gao_i_force_interface;
int Dialogue_dialogue_check_univVars;
int Dialogue_dialogue_check_univVars_Flag_speciaux_sauve;
int Dialogue_dialogue_check_TXT_Sally_gao;
int Dialogue_dialogue_check_GST_Board_panel_gao_ai_dialog_consequences;
int Dialogue_dialogue_check_GST_Board_panel_gao_o_dialogue_actor;
int Dialogue_dialogue_check_GST_Board_panel_gao_at_dialog_questions;
int Dialogue_dialogue_check_GST_Board_panel_gao_at_dialog_reponses;
int Dialogue_dialogue_check_GST_Board_panel_gao_ai_dialog_grisage;

int JeTeParle_jtp_revol()
{
    int result; // eax
    tScriptStruct_ActorJeTeParle* v1; // [esp+8h] [ebp-20h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ActorJeTeParle*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        JeTeParle_jtp_revol_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
    }
    AI_EvalFunc_ANIPelvisOffsetSet_C(AI_gpst_CurrentGameObject, v1->f_offset_pivot);
    if (AI_EvalFunc_OBJ_CustomBitsTest_C((tGameObjectMain*)JeTeParle_jtp_revol_GST_Board_panel_gao, 0x800))
    {
        AI_EvalFunc_ACTActionSet_C_CURRENT(0);
        v1->f_time_revol = TIM_gf_MainClock - TIM_gf_LockTime;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        if (AI_EvalFunc_TIMEElapsed_C(v1->f_time_revol, v1->f_rand_revol))
        {
            v1->f_rand_revol = AI_EvalFunc_MATHRandFloat_C(2.0, 4.0);
            v1->f_time_revol = TIM_gf_MainClock - TIM_gf_LockTime;
            if (AI_EvalFunc_ACTActionGet_C_CURRENT())
            {
                AI_EvalFunc_ACTActionSet_C_CURRENT(0);
            }
            else if (v1->i_act_revol)
            {
                v1->i_act_revol = 0;
                AI_EvalFunc_ACTActionSet_C_CURRENT(2);
            }
            else
            {
                v1->i_act_revol = 1;
                AI_EvalFunc_ACTActionSet_C_CURRENT(1);
            }
        }
        result = 1000;
    }
    return result;
}

int JeTeParle_jtp_mamago_bricole()
{
    float v1; // [esp+8h] [ebp-34h]
    tScriptStruct_ActorJeTeParle* v2; // [esp+Ch] [ebp-30h]
    float v3; // [esp+10h] [ebp-2Ch]
    float v4; // [esp+18h] [ebp-24h]
    float v5; // [esp+20h] [ebp-1Ch]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorJeTeParle*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        JeTeParle_jtp_mamago_bricole_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &JeTeParle_jtp_mamago_bricole_univVars_Flag_speciaux_sauve,
            (void*)JeTeParle_jtp_mamago_bricole_univVars,
            4488);
        JeTeParle_jtp_mamago_bricole_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
    }
    if (AI_EvalFunc_ARRBitTest_C((int*)(JeTeParle_jtp_mamago_bricole_univVars_Flag_speciaux_sauve + 4), 2189))
    {
        AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 2u, 1);
        v4 = 2.0;
        if (AI_EvalFunc_OBJ_CustomBitsTest_C((tGameObjectMain*)JeTeParle_jtp_mamago_bricole_GST_Board_panel_gao, 0x800))
        {
            v2->f_angle_target = 0.94999999f;
            AI_EvalFunc_ACTActionSet_C_CURRENT(4);
            v4 = 5.0;
        }
        else
        {
            v2->f_angle_target = 0.0;
            AI_EvalFunc_ACTActionSet_C_CURRENT(3);
        }
        if (v2->f_angle_target != v2->f_angle)
        {
            v3 = v4 * TIM_gf_dt;
            if (v3 < 0.0)
                v3 = 0.0;
            if (v3 > 1.0)
                v3 = 1.0;
            v5 = v2->f_angle_target * v3 + v2->f_angle * (1.0f - v3);
            v2->f_angle = v5;
            v1 = (float)fabs(v2->f_angle_target - v2->f_angle);
            if (v1 < 0.0049999999)
                v2->f_angle = v2->f_angle_target;
        }
    }
    else
    {
        AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 1u, 2);
        if (!v2->i_rot_mamago)
        {
            AI_EvalFunc_OBJRotateGlobalZ_C(AI_gpst_CurrentGameObject, 2.2f);
            v2->i_rot_mamago = 1;
        }
        if (AI_EvalFunc_OBJ_CustomBitsTest_C((tGameObjectMain*)JeTeParle_jtp_mamago_bricole_GST_Board_panel_gao, 0x800))
            AI_EvalFunc_ACTActionSet_C_CURRENT(9);
        else
            AI_EvalFunc_ACTActionSet_C_CURRENT(8);
    }
    return 1000;
}

int JeTeParle_jtp_palet()
{
    int result; // eax

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        JeTeParle_jtp_palet_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&JeTeParle_jtp_palet_univVars_Flag_speciaux_sauve, (void*)JeTeParle_jtp_palet_univVars, 4488);
        JeTeParle_jtp_palet_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
    }
    if (AI_EvalFunc_ARRBitTest_C((int*)(JeTeParle_jtp_palet_univVars_Flag_speciaux_sauve + 4), 2834))
    {
        if (AI_EvalFunc_ARRBitTest_C((int*)(JeTeParle_jtp_palet_univVars_Flag_speciaux_sauve + 4), 2841))
            AI_EvalFunc_ACTActionSet_C_CURRENT(3);
        else
            AI_EvalFunc_ACTActionSet_C_CURRENT(0);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(1))
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        if (AI_EvalFunc_OBJ_CustomBitsTest_C((tGameObjectMain*)JeTeParle_jtp_palet_GST_Board_panel_gao, 2048))
            AI_EvalFunc_ACTActionSet_C_CURRENT(2);
        else
            AI_EvalFunc_ACTActionSet_C_CURRENT(0);
        result = 1000;
    }
    return result;
}

int JeTeParle_jtp_reflex()
{
    int result; // eax
    tScriptStruct_ActorJeTeParle* v1; // [esp+0h] [ebp-54h]
    tGameObjectMain* v2; // [esp+4h] [ebp-50h]
    tGameObjectMain* v3; // [esp+8h] [ebp-4Ch]
    tGameObjectMain* v4; // [esp+Ch] [ebp-48h]
    tGameObjectMain* v5; // [esp+10h] [ebp-44h]
    tMessage v6; // [esp+18h] [ebp-3Ch] BYREF
    void* Src; // [esp+4Ch] [ebp-8h]
    tScriptStruct_ActorJeTeParle* v8; // [esp+50h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ActorJeTeParle*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v8 = v1;
    v5 = (tGameObjectMain*)v1->o_papa;
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
    if (!v4)
        goto LABEL_17;
    v3 = (tGameObjectMain*)v8->o_papa;
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        v2 = v3;
    }
    else
    {
        v2 = 0;
    }
    if ((v2->flags2 & 1) != 0)
    {
    LABEL_17:
        Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v6, Src, sizeof(v6));
        if (v6.msgID == 10 && v6.f2 < 0.0 || v6.msgID == 20)
            memcpy(&v8->m_dialogue, &v6, sizeof(v8->m_dialogue));
        result = 1000;
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int JeTeParle_jtp_exec_regarde()
{
    int v0; // eax
    tGameObjectMain* v1; // eax
    tScriptStruct_ActorJeTeParle* v3; // [esp+0h] [ebp-118h]
    tGameObjectMain* v4; // [esp+10h] [ebp-108h]
    tGameObjectMain* v5; // [esp+20h] [ebp-F8h]
    tGameObjectMain* v6; // [esp+24h] [ebp-F4h]
    tGameObjectMain* v7; // [esp+28h] [ebp-F0h]
    tGameObjectMain* v8; // [esp+2Ch] [ebp-ECh]
    tGameObjectMain* v9; // [esp+30h] [ebp-E8h]
    tGameObjectMain* v10; // [esp+40h] [ebp-D8h]
    tGameObjectMain* v11; // [esp+50h] [ebp-C8h]
    tGameObjectMain* v12; // [esp+54h] [ebp-C4h]
    tGameObjectMain* v13; // [esp+58h] [ebp-C0h]
    tGameObjectMain* v14; // [esp+60h] [ebp-B8h]
    tGameObjectMain* v15; // [esp+64h] [ebp-B4h]
    tGameObjectMain* v16; // [esp+68h] [ebp-B0h]
    tGameObjectMain* v17; // [esp+70h] [ebp-A8h]
    Vec3 a3; // [esp+74h] [ebp-A4h] BYREF
    Vec3 v19; // [esp+84h] [ebp-94h] BYREF
    Vec3 a1; // [esp+90h] [ebp-88h] BYREF
    Vec3 v21; // [esp+9Ch] [ebp-7Ch] BYREF
    Vec3 v22; // [esp+A8h] [ebp-70h] BYREF
    float v23; // [esp+B4h] [ebp-64h]
    int v24; // [esp+B8h] [ebp-60h]
    unsigned int v25; // [esp+BCh] [ebp-5Ch]
    Vec3 v26; // [esp+C0h] [ebp-58h] BYREF
    Vec3 v27; // [esp+CCh] [ebp-4Ch] BYREF
    Vec3 v28; // [esp+D8h] [ebp-40h] BYREF
    tGameObjectMain* v29; // [esp+E4h] [ebp-34h]
    int v30; // [esp+E8h] [ebp-30h]
    float v31; // [esp+ECh] [ebp-2Ch]
    tScriptStruct_ActorJeTeParle* v32; // [esp+F0h] [ebp-28h]
    float v33; // [esp+F4h] [ebp-24h]
    unsigned int v34; // [esp+F8h] [ebp-20h]
    Vec3 v35; // [esp+108h] [ebp-10h] BYREF
    float v36; // [esp+114h] [ebp-4h]

    v21.x = 1.0;
    v21.y = 0.0;
    v21.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_ActorJeTeParle*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v32 = v3;
    v33 = 0.0;
    v34 = 0;
    v25 = 0;
    v17 = (tGameObjectMain*)v3->des_o_acteur_suividesyeux;
    if (v17)
    {
        if ((unsigned int)v17 <= 2)
            v17 = AI_MainActor[(uint32_t)v17 - 1];
        v0 = AI_EvalFunc_OBJ_HasBeenKilled_C(v17);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_HasBeenKilled_C(0);
    }
    v24 = v0;
    if (v0)
        v32->des_o_acteur_suividesyeux = 0;
    v33 = v32->des_f_RT_vitesse_de_rotation;
    if (v32->des_f_RT_vitesse_de_rotation < 1.0)
    {
        v31 = TIM_gf_dt;
        v33 = TIM_gf_dt * 20.0f * v33;
    }
    v16 = (tGameObjectMain*)v32->des_o_acteur_suividesyeux;
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
    if (v15 && (v30 = AI_EvalFunc_ACTCustomBitTest_C(AI_gpst_CurrentGameObject, 4u)) != 0)
    {
        if ((v32->des_i_RT_flags & 0x10000) != 0)
        {
            v34 = v32->des_i_RT_flags & 0xFFFEFFFF;
            v14 = (tGameObjectMain*)v32->des_o_acteur_suividesyeux;
            if (v14)
            {
                if ((unsigned int)v14 <= 2)
                    v14 = AI_MainActor[(uint32_t)v14 - 1];
                v1 = AI_EvalFunc_ANICanalGaoGet_C(v14, 0);
            }
            else
            {
                v1 = AI_EvalFunc_ANICanalGaoGet_C(0, 0);
            }
            v25 = (unsigned int)v1;
            v13 = v1;
            if (v1)
            {
                if ((unsigned int)v1 <= 2)
                    v13 = AI_MainActor[(uint32_t)v1 - 1];
                v12 = v13;
            }
            else
            {
                v12 = 0;
            }
            if (v12)
            {
                if (v32->des_v_hauteur_cible.z <= 10000.0)
                {
                    v10 = (tGameObjectMain*)v25;
                    if (v25)
                    {
                        if (v25 <= 2)
                            v10 = AI_MainActor[v25 - 1];
                        AI_EvalFunc_OBJGetPos_C(v10, &v26);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v26);
                    }
                    v35.x = 0.0;
                    v35.y = 0.0;
                    v35.z = 0.25;
                    MATH_VecAddVec(&a1, &v26, &v35);
                    v21 = a1;
                    v29 = AI_gpst_CurrentGameObject;
                    v9 = (tGameObjectMain*)v32->des_o_acteur_suividesyeux;
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
                    v7 = v29;
                    if (v29)
                    {
                        if ((unsigned int)v29 <= 2)
                            v7 = AI_MainActor[(uint32_t)v29 - 1];
                        v6 = v7;
                    }
                    else
                    {
                        v6 = 0;
                    }
                    if (v8 != v6)
                    {
                        v32->des_v_hauteur_cible.x = 0.0;
                        v32->des_v_hauteur_cible.y = 0.0;
                        v32->des_v_hauteur_cible.z = 0.0;
                    }
                }
                else
                {
                    v32->des_v_hauteur_cible.z = v32->des_v_hauteur_cible.z - 10000.0f;
                    v11 = (tGameObjectMain*)v32->des_o_acteur_suividesyeux;
                    if (v11)
                    {
                        if ((unsigned int)v11 <= 2)
                            v11 = AI_MainActor[(uint32_t)v11 - 1];
                        AI_EvalFunc_OBJGetPos_C(v11, &a3);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &a3);
                    }
                    v21 = a3;
                }
            }
            else
            {
                v5 = (tGameObjectMain*)v32->des_o_acteur_suividesyeux;
                if (v5)
                {
                    if ((unsigned int)v5 <= 2)
                        v5 = AI_MainActor[(uint32_t)v5 - 1];
                    AI_EvalFunc_OBJGetPos_C(v5, &v22);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v22);
                }
                v21 = v22;
            }
        }
        else
        {
            v34 = v32->des_i_RT_flags;
            v4 = (tGameObjectMain*)v32->des_o_acteur_suividesyeux;
            if (v4)
            {
                if ((unsigned int)v4 <= 2)
                    v4 = AI_MainActor[(uint32_t)v4 - 1];
                AI_EvalFunc_OBJGetPos_C(v4, &v28);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v28);
            }
            v21 = v28;
        }
        MATH_VecAddVec(&v19, &v21, &v32->des_v_hauteur_cible);
        if (v33 <= 1.0)
        {
            if (v33 >= 0.0)
                v36 = v33;
            else
                v36 = 0.0;
        }
        else
        {
            v36 = 1.0;
        }
        AI_EvalFunc_OBJEyeFollow_C(AI_gpst_CurrentGameObject, &v19, v36, v34 + v32->des_i_type_acteur);
    }
    else
    {
        v34 = v32->des_i_RT_flags & 0xFFFEFFFF;
        v27.x = 0.0;
        v27.y = 0.0;
        v27.z = 0.0;
        if (v33 <= 1.0)
        {
            if (v33 >= 0.0)
                v23 = v33;
            else
                v23 = 0.0;
        }
        else
        {
            v23 = 1.0;
        }
        AI_EvalFunc_OBJEyeFollow_C(AI_gpst_CurrentGameObject, &v27, v23, v34 + v32->des_i_type_acteur);
    }
    return 1000;
}

int JeTeParle_jtp_init()
{
    tGameObjectMain* v1; // eax
    double v2; // st7
    float X; // [esp+0h] [ebp-E8h]
    tScriptStruct_ActorJeTeParle* v4; // [esp+4h] [ebp-E4h]
    tGameObjectMain* v5; // [esp+Ch] [ebp-DCh]
    tGameObjectMain* v6; // [esp+14h] [ebp-D4h]
    tGameObjectMain* v7; // [esp+1Ch] [ebp-CCh]
    tGameObjectMain* v8; // [esp+24h] [ebp-C4h]
    tGameObjectMain* v9; // [esp+2Ch] [ebp-BCh]
    tGameObjectMain* v10; // [esp+34h] [ebp-B4h]
    tGameObjectMain* v11; // [esp+3Ch] [ebp-ACh]
    tGameObjectMain* v12; // [esp+44h] [ebp-A4h]
    tGameObjectMain* v13; // [esp+48h] [ebp-A0h]
    tGameObjectMain* v14; // [esp+50h] [ebp-98h]
    tGameObjectMain* v15; // [esp+58h] [ebp-90h]
    tGameObjectMain* v16; // [esp+5Ch] [ebp-8Ch]
    tGameObjectMain* v17; // [esp+60h] [ebp-88h]
    tGameObjectMain* v18; // [esp+68h] [ebp-80h]
    tGameObjectMain* v19; // [esp+70h] [ebp-78h]
    tGameObjectMain* v20; // [esp+74h] [ebp-74h]
    tGameObjectMain* v21; // [esp+78h] [ebp-70h]
    Vec3 v22; // [esp+90h] [ebp-58h] BYREF
    tGameObjectMain* v23; // [esp+9Ch] [ebp-4Ch]
    BOOL v24; // [esp+A4h] [ebp-44h]
    tGameObjectMain* v25; // [esp+ACh] [ebp-3Ch]
    tGameObjectMain* v26; // [esp+B0h] [ebp-38h]
    Vec3 v27; // [esp+B4h] [ebp-34h] BYREF
    int v28; // [esp+C0h] [ebp-28h]
    int v29; // [esp+C4h] [ebp-24h]
    tScriptStruct_ActorJeTeParle* v30; // [esp+CCh] [ebp-1Ch]
    int i; // [esp+D0h] [ebp-18h]
    Vec3 a3; // [esp+D4h] [ebp-14h] BYREF
    tGameObjectMain* v33; // [esp+E4h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_ActorJeTeParle*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v30 = v4;
    v29 = 0;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        JeTeParle_jtp_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&JeTeParle_jtp_init_univVars_Flag_speciaux_sauve, (void*)JeTeParle_jtp_init_univVars, 4488);
        JeTeParle_jtp_init_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &JeTeParle_jtp_init_INV_manager_gao_i_Animals_Nb,
            (void*)JeTeParle_jtp_init_INV_manager_gao,
            23888);
        AI2C_ResolveBuffer(
            &JeTeParle_jtp_init_INV_manager_gao_ao_Animals,
            (void*)JeTeParle_jtp_init_INV_manager_gao,
            23368);
        AI2C_ResolveBuffer(
            &JeTeParle_jtp_init_INV_manager_gao_ai_AnimalsData,
            (void*)JeTeParle_jtp_init_INV_manager_gao,
            23628);
        fct_JeTeParle_jtp_init_JeTeParle_jtp_rottete = (int)AI_ul_RealResolveOneRefFunction(0x26003094u);
        fct_JeTeParle_jtp_init_JeTeParle_jtp_mamago_presente_objets = (int)AI_ul_RealResolveOneRefFunction(0x2600308Du);
        fct_JeTeParle_jtp_init_JeTeParle_jtp_mamago_bricole_AB = (int)AI_ul_RealResolveOneRefFunction(0x2600308Fu);
        fct_JeTeParle_jtp_init_JeTeParle_jtp_mamago_bricole = (int)AI_ul_RealResolveOneRefFunction(0x26003089u);
        fct_JeTeParle_jtp_init_JeTeParle_jtp_palet_posperle = (int)AI_ul_RealResolveOneRefFunction(0x490289B8u);
        fct_JeTeParle_jtp_init_JeTeParle_jtp_palet = (int)AI_ul_RealResolveOneRefFunction(0x3D0034BDu);
        fct_JeTeParle_jtp_init_JeTeParle_jtp_revol_AB = (int)AI_ul_RealResolveOneRefFunction(0x3D0052CEu);
        fct_JeTeParle_jtp_init_JeTeParle_jtp_revol = (int)AI_ul_RealResolveOneRefFunction(0x3D0052CCu);
        fct_JeTeParle_jtp_init_JeTeParle_jtp_reflex = (int)AI_ul_RealResolveOneRefFunction(0x9E0035A9);
    }
    if (v30->des_i_sf_jesuis_la)
    {
        if (v30->des_i_sf_jesuis_la <= 0)
        {
            v29 = 0;
        }
        else if (v30->des_i_sf_jesuis_la < 10000)
        {
            v29 = AI_EvalFunc_ARRBitTest_C(
                (int*)(JeTeParle_jtp_init_univVars_Flag_speciaux_sauve + 4),
                v30->des_i_sf_jesuis_la);
        }
        else
        {
            v24 = AI_EvalFunc_ARRBitTest_C(
                (int*)(JeTeParle_jtp_init_univVars_Flag_speciaux_sauve + 4),
                v30->des_i_sf_jesuis_la - 10000);
            v29 = 1 - v24;
        }
        if (!v29)
        {
            v28 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v28 != 1000)
                return v28;
        }
    }
    v21 = (tGameObjectMain*)v30->des_o_jesuisla_desactive;
    if (v21)
    {
        if ((unsigned int)v21 <= 2)
            v21 = AI_MainActor[(uint32_t)v21 - 1];
        v20 = v21;
    }
    else
    {
        v20 = 0;
    }
    if (v20)
    {
        v19 = (tGameObjectMain*)v30->des_o_jesuisla_desactive;
        if (v19)
        {
            if ((unsigned int)v19 <= 2)
                v19 = AI_MainActor[(uint32_t)v19 - 1];
            AI_EvalFunc_OBJ_FlagInactiveSet_C(v19, 1);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
        }
        v18 = (tGameObjectMain*)v30->des_o_jesuisla_desactive;
        if (v18)
        {
            if ((unsigned int)v18 <= 2)
                v18 = AI_MainActor[(uint32_t)v18 - 1];
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(v18, 1);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
        }
    }
    v17 = (tGameObjectMain*)v30->des_o_jesuisla_active;
    if (v17)
    {
        if ((unsigned int)v17 <= 2)
            v17 = AI_MainActor[(uint32_t)v17 - 1];
        v16 = v17;
    }
    else
    {
        v16 = 0;
    }
    if (v16)
    {
        v15 = (tGameObjectMain*)v30->des_o_jesuisla_active;
        if (v15)
        {
            if ((unsigned int)v15 <= 2)
                v15 = AI_MainActor[(uint32_t)v15 - 1];
            AI_EvalFunc_OBJ_FlagInactiveSet_C(v15, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
        }
        v14 = (tGameObjectMain*)v30->des_o_jesuisla_active;
        if (v14)
        {
            if ((unsigned int)v14 <= 2)
                v14 = AI_MainActor[(uint32_t)v14 - 1];
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(v14, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
        }
    }
    if (v30->des_i_animaux != -1)
    {
        AI_EvalFunc_OBJ_InfoPhotoParamSet_C(
            AI_gpst_CurrentGameObject,
            4,
            v30->des_i_animaux,
            0,
            0,
            0.15000001f,
            5.0,
            0.30000001f,
            0.5);
        if (v30->des_i_etage != -1 && *(int*)JeTeParle_jtp_init_INV_manager_gao_i_Animals_Nb < 64)
        {
            *(uint32_t*)(JeTeParle_jtp_init_INV_manager_gao_ao_Animals
                + 4 * *(uint32_t*)JeTeParle_jtp_init_INV_manager_gao_i_Animals_Nb
                + 4) = (uint32_t)AI_gpst_CurrentGameObject;
            *(uint32_t*)(JeTeParle_jtp_init_INV_manager_gao_ai_AnimalsData
                + 4 * (*(uint32_t*)JeTeParle_jtp_init_INV_manager_gao_i_Animals_Nb)++
                + 4) = v30->des_i_animaux + (v30->des_i_etage << 8);
        }
        v13 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v13 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            v12 = v13;
        }
        else
        {
            v12 = 0;
        }
        v12->extended->fields->designFlags = 64;
    }
    for (i = 0; i < 10 && v30->des_ai_sf_pos[i]; ++i)
    {
        if (v30->des_ai_sf_pos[i] <= 0)
            v29 = 0;
        else
            v29 = AI_EvalFunc_ARRBitTest_C(
                (int*)(JeTeParle_jtp_init_univVars_Flag_speciaux_sauve + 4),
                v30->des_ai_sf_pos[i]);
        if (v29)
        {
            v11 = (tGameObjectMain*)v30->des_ao_sf_pos[i];
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v11 = AI_MainActor[(uint32_t)v11 - 1];
                AI_EvalFunc_OBJGetPos_C(v11, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            AI_EvalFunc_OBJSetPos_C_CURRENT(&a3);
            v10 = (tGameObjectMain*)v30->des_ao_sf_pos[i];
            if (v10)
            {
                if ((unsigned int)v10 <= 2)
                    v10 = AI_MainActor[(uint32_t)v10 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v10, &v22);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v22);
            }
            v27.x = 0.0;
            v27.y = 0.0;
            v27.z = 1.0;
            AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v22, &v27);
        }
    }
    v1 = AI_EvalFunc_OBJ_HierarchyGet_C(AI_gpst_CurrentGameObject);
    v30->o_papa = (int)v1;
    switch (v30->i_compotement_specifique)
    {
    case 1:
        v23 = AI_gpst_CurrentGameObject;
        v9 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v9 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v9,
                3,
                (tAIFunction*)fct_JeTeParle_jtp_init_JeTeParle_jtp_rottete);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                3,
                (tAIFunction*)fct_JeTeParle_jtp_init_JeTeParle_jtp_rottete);
        }
        AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_JeTeParle_jtp_init_JeTeParle_jtp_mamago_presente_objets);
        break;
    case 2:
        v25 = AI_gpst_CurrentGameObject;
        v8 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v8 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v8,
                3,
                (tAIFunction*)fct_JeTeParle_jtp_init_JeTeParle_jtp_mamago_bricole_AB);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                3,
                (tAIFunction*)fct_JeTeParle_jtp_init_JeTeParle_jtp_mamago_bricole_AB);
        }
        AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_JeTeParle_jtp_init_JeTeParle_jtp_mamago_bricole);
        break;
    case 3:
        v7 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v7 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v7,
                3,
                (tAIFunction*)fct_JeTeParle_jtp_init_JeTeParle_jtp_palet_posperle);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                3,
                (tAIFunction*)fct_JeTeParle_jtp_init_JeTeParle_jtp_palet_posperle);
        }
        AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_JeTeParle_jtp_init_JeTeParle_jtp_palet);
        break;
    case 4:
        v33 = AI_gpst_CurrentGameObject;
        v6 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v6 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v6,
                3,
                (tAIFunction*)fct_JeTeParle_jtp_init_JeTeParle_jtp_revol_AB);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                3,
                (tAIFunction*)fct_JeTeParle_jtp_init_JeTeParle_jtp_revol_AB);
        }
        AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_JeTeParle_jtp_init_JeTeParle_jtp_revol);
        break;
    default:
        v26 = AI_gpst_CurrentGameObject;
        v5 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v5 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v5,
                3,
                (tAIFunction*)fct_JeTeParle_jtp_init_JeTeParle_jtp_rottete);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                3,
                (tAIFunction*)fct_JeTeParle_jtp_init_JeTeParle_jtp_rottete);
        }
        break;
    }
    X = v30->des_f_angle_talk * 0.017453f;
    v2 = cosf(X);
    v30->des_f_angle_talk = (float)v2;
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_JeTeParle_jtp_init_JeTeParle_jtp_reflex);
    return 1000;
}

int JeTeParle__jtp_check()
{
    double v1; // st7
    uint32_t* v2; // eax
    int* v3; // eax
    uint32_t* v4; // eax
    int* v5; // eax
    int* v6; // eax
    char* v7; // eax
    char* v8; // eax
    char* v9; // eax
    char* v10; // eax
    int v11; // [esp-4h] [ebp-1A8h]
    int v12; // [esp+0h] [ebp-1A4h]
    const void* v13; // [esp+0h] [ebp-1A4h]
    const void* v14; // [esp+0h] [ebp-1A4h]
    char* v15; // [esp+0h] [ebp-1A4h]
    char* v16; // [esp+0h] [ebp-1A4h]
    int v17; // [esp+4h] [ebp-1A0h]
    float v18; // [esp+Ch] [ebp-198h]
    tScriptStruct_ActorJeTeParle* v19; // [esp+10h] [ebp-194h]
    int* v20; // [esp+14h] [ebp-190h]
    int* v21; // [esp+1Ch] [ebp-188h]
    int* v22; // [esp+24h] [ebp-180h]
    int* a1; // [esp+2Ch] [ebp-178h]
    int* v24; // [esp+34h] [ebp-170h]
    double v25; // [esp+3Ch] [ebp-168h]
    double v26; // [esp+44h] [ebp-160h]
    double v27; // [esp+54h] [ebp-150h]
    tGameObjectMain* v28; // [esp+70h] [ebp-134h]
    tGameObjectMain* v29; // [esp+74h] [ebp-130h]
    tGameObjectMain* v30; // [esp+78h] [ebp-12Ch]
    tGameObjectMain* v31; // [esp+7Ch] [ebp-128h]
    tGameObjectMain* v32; // [esp+80h] [ebp-124h]
    Vec3 a3; // [esp+90h] [ebp-114h] BYREF
    Vec3 a2; // [esp+9Ch] [ebp-108h] BYREF
    float v35; // [esp+ACh] [ebp-F8h]
    float v36; // [esp+B4h] [ebp-F0h]
    int v37; // [esp+B8h] [ebp-ECh]
    float v38; // [esp+BCh] [ebp-E8h]
    Vec3 dest; // [esp+D0h] [ebp-D4h] BYREF
    Vec3 v40; // [esp+DCh] [ebp-C8h] BYREF
    BOOL v41; // [esp+E8h] [ebp-BCh]
    BOOL v42; // [esp+F0h] [ebp-B4h]
    int v43; // [esp+F4h] [ebp-B0h]
    int v44; // [esp+F8h] [ebp-ACh]
    int v45; // [esp+FCh] [ebp-A8h]
    int v46; // [esp+100h] [ebp-A4h]
    Vec3 v47; // [esp+104h] [ebp-A0h] BYREF
    tMessage Src; // [esp+110h] [ebp-94h] BYREF
    Vec3 v50; // [esp+14Ch] [ebp-58h]
    BOOL v51; // [esp+158h] [ebp-4Ch]
    int i; // [esp+15Ch] [ebp-48h]
    int a4; // [esp+160h] [ebp-44h]
    int num; // [esp+164h] [ebp-40h]
    float v55; // [esp+16Ch] [ebp-38h]
    tScriptStruct_ActorJeTeParle* v56; // [esp+170h] [ebp-34h]
    BOOL v57; // [esp+174h] [ebp-30h]
    Vec3 v58; // [esp+178h] [ebp-2Ch] BYREF
    Vec3 v59; // [esp+184h] [ebp-20h] BYREF
    int v60; // [esp+19Ch] [ebp-8h]

    v50.x = 1.0;
    v50.y = 0.0;
    v50.z = 0.0;
    v40.x = 1.0;
    v40.y = 0.0;
    v40.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v19 = (tScriptStruct_ActorJeTeParle*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v19 = 0;
    v56 = v19;
    v51 = 0;
    i = 0;
    v60 = 0;
    v42 = 0;
    v43 = 0;
    v38 = 0.0;
    v44 = 0;
    a4 = 0;
    v45 = 0;
    num = 0;
    v57 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        JeTeParle__jtp_check__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        JeTeParle__jtp_check_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &JeTeParle__jtp_check__PJ_Sally_gao_current_pm_mode,
            (void*)JeTeParle__jtp_check__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(
            &JeTeParle__jtp_check__PJ_Sally_gao_current_pm_priority,
            (void*)JeTeParle__jtp_check__PJ_Sally_gao,
            576);
        AI2C_ResolveBuffer(
            &JeTeParle__jtp_check_GST_Board_panel_gao_i_sally_priority,
            (void*)JeTeParle__jtp_check_GST_Board_panel_gao,
            396);
        AI2C_ResolveBuffer(
            &JeTeParle__jtp_check_GST_Board_panel_gao_o_sally_contextuel,
            (void*)JeTeParle__jtp_check_GST_Board_panel_gao,
            388);
        AI2C_ResolveBuffer(
            &JeTeParle__jtp_check_GST_Board_panel_gao_i_sally_action,
            (void*)JeTeParle__jtp_check_GST_Board_panel_gao,
            412);
        AI2C_ResolveBuffer(
            &JeTeParle__jtp_check_GST_Board_panel_gao_v_sally_offset,
            (void*)JeTeParle__jtp_check_GST_Board_panel_gao,
            400);
        AI2C_ResolveBuffer(
            &JeTeParle__jtp_check_GST_Board_panel_gao_i_force_interface,
            (void*)JeTeParle__jtp_check_GST_Board_panel_gao,
            3292);
        JeTeParle__jtp_check_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&JeTeParle__jtp_check_univVars_Flag_speciaux_sauve, (void*)JeTeParle__jtp_check_univVars, 4488);
        JeTeParle__jtp_check_TXT_Sally_gao = AI_ul_RealResolveOneRefObject(0x7200064Eu);
        AI2C_ResolveBuffer(
            &JeTeParle__jtp_check_GST_Board_panel_gao_ai_dialog_consequences,
            (void*)JeTeParle__jtp_check_GST_Board_panel_gao,
            1432);
        AI2C_ResolveBuffer(
            &JeTeParle__jtp_check_GST_Board_panel_gao_o_dialogue_actor,
            (void*)JeTeParle__jtp_check_GST_Board_panel_gao,
            1100);
        AI2C_ResolveBuffer(
            &JeTeParle__jtp_check_GST_Board_panel_gao_at_dialog_questions,
            (void*)JeTeParle__jtp_check_GST_Board_panel_gao,
            1104);
        AI2C_ResolveBuffer(
            &JeTeParle__jtp_check_GST_Board_panel_gao_at_dialog_reponses,
            (void*)JeTeParle__jtp_check_GST_Board_panel_gao,
            1268);
        AI2C_ResolveBuffer(
            &JeTeParle__jtp_check_GST_Board_panel_gao_ai_dialog_grisage,
            (void*)JeTeParle__jtp_check_GST_Board_panel_gao,
            1516);
    }
    v32 = (tGameObjectMain*)v56->o_papa;
    if (v32)
    {
        if ((unsigned int)v32 <= 2)
            v32 = AI_MainActor[(uint32_t)v32 - 1];
        v31 = v32;
    }
    else
    {
        v31 = 0;
    }
    if (v31)
    {
        v30 = (tGameObjectMain*)v56->o_papa;
        if (v30)
        {
            if ((unsigned int)v30 <= 2)
                v30 = AI_MainActor[(uint32_t)v30 - 1];
            v29 = v30;
        }
        else
        {
            v29 = 0;
        }
        v37 = (unsigned __int16)v29->flags2;
        if ((v37 & 1) == 0)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
    }
    v56->i_talk_propose = 0;
    v28 = (tGameObjectMain*)JeTeParle__jtp_check__PJ_Sally_gao;
    if (JeTeParle__jtp_check__PJ_Sally_gao)
    {
        if ((unsigned int)JeTeParle__jtp_check__PJ_Sally_gao <= 2)
            v28 = AI_MainActor[JeTeParle__jtp_check__PJ_Sally_gao - 1];
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v28);
    }
    else
    {
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v55 = (float)v1;
    if (v56->des_f_distance_talk * v56->des_f_distance_talk > v55
        && !AI_EvalFunc_OBJ_CustomBitsTest_C((tGameObjectMain*)JeTeParle__jtp_check_GST_Board_panel_gao, 2048))
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)JeTeParle__jtp_check__PJ_Sally_gao, &a3);
        MATH_SubstractVec(&dest, &a2, &a3);
        v40 = dest;
        AI_EvalFunc_MATHVecSetHorzNormalize_C(&v40);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v59);
        v35 = v40.z * v59.z + v40.y * v59.y + v59.x * v40.x;
        if (v56->des_f_angle_talk <= (double)v35)
        {
            AI_EvalFunc_MATHVecNormalize_C(&v40, &v47);
            v50 = v47;
            AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)JeTeParle__jtp_check__PJ_Sally_gao, &v58);
            v36 = v50.z * v58.z + v50.y * v58.y + v58.x * v50.x;
            v38 = v36 * 5.0f;
            v27 = v38 + -10.0 + MATH_gd_magicDoubleForRound;
            v60 = (*(uint32_t *)&v27);
            v46 = (unsigned __int16)*(uint32_t*)(JeTeParle__jtp_check__PJ_Sally_gao + 4);
            if ((v46 & 1) != 0)
                v57 = !*(uint32_t*)JeTeParle__jtp_check__PJ_Sally_gao_current_pm_mode
                || *(int*)JeTeParle__jtp_check__PJ_Sally_gao_current_pm_priority < 20;
            else
                v57 = 0;
            if (v57)
            {
                v18 = (float)v60;
                if (v18 > (double)*(float*)JeTeParle__jtp_check_GST_Board_panel_gao_i_sally_priority)
                {
                    *(uint32_t*)JeTeParle__jtp_check_GST_Board_panel_gao_o_sally_contextuel = (uint32_t)AI_gpst_CurrentGameObject;
                    *(float*)JeTeParle__jtp_check_GST_Board_panel_gao_i_sally_priority = (float)v60;
                    *(uint32_t*)JeTeParle__jtp_check_GST_Board_panel_gao_i_sally_action = 5;
                    *(uint32_t*)JeTeParle__jtp_check_GST_Board_panel_gao_v_sally_offset = 0;
                    *(uint32_t*)(JeTeParle__jtp_check_GST_Board_panel_gao_v_sally_offset + 4) = 0;
                    *(uint32_t*)(JeTeParle__jtp_check_GST_Board_panel_gao_v_sally_offset + 8) = 1065353216;
                    *(uint32_t*)JeTeParle__jtp_check_GST_Board_panel_gao_i_force_interface = 1;
                }
            }
            v56->i_talk_propose = v57;
        }
    }
    v43 = 0;
    v44 = 0;
    if (v56->m_dialogue.msgID == 10 && (v41 = AI_EvalFunc_TIMEElapsed_C(v56->f_time_fin_talk, 0.5)))
    {
        v51 = AI_EvalFunc_ARRBitTest_C((int*)(JeTeParle__jtp_check_univVars_Flag_speciaux_sauve + 4), 2083);
        if (v56->i_interdit_pendant_mili_malad && v51)
        {
            Src.msgID = 30;
            Src.Int1 = 60;
            Src.Int2 = 0;
            Src.f2 = 1.0;
            Src.v1.x = 0.0;
            Src.v1.y = 0.0;
            Src.v1.z = 0.0;
            Src.v2.x = 0.0;
            Src.v2.y = 0.0;
            Src.v2.z = 0.0;
            Src.gao = 0;
            Src.gao2 = AI_gpst_CurrentGameObject;
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)JeTeParle__jtp_check_TXT_Sally_gao, &Src);
            v56->m_dialogue.msgID = 0;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v43 = 1;
        if (v56->Valide_Effet_1_Question)
            AI_EvalFunc_OBJ_CapaSet_C((tGameObjectMain*)JeTeParle__jtp_check_GST_Board_panel_gao, 128, 0);
        else
            AI_EvalFunc_OBJ_CapaSet_C((tGameObjectMain*)JeTeParle__jtp_check_GST_Board_panel_gao, 0, 128);
    }
    else if (v56->m_dialogue.msgID == 20)
    {
        v44 = 1;
    }
    if (!v44)
        goto LABEL_73;
    v26 = v56->m_dialogue.f2 - 1.0 + MATH_gd_magicDoubleForRound;
    a4 = (*(uint32_t *)&v26);
    v25 = v56->m_dialogue.f1 + MATH_gd_magicDoubleForRound;
    v45 = (*(uint8_t*)&v25);
    if ((*(uint8_t*)&v25) == 255)
    {
        v56->f_time_fin_talk = TIM_gf_MainClock - TIM_gf_LockTime;
        v43 = 1;
        goto LABEL_73;
    }
    if (v45 != 254 && v45 != 253)
    {
        a4 = v45 - 1;
        for (i = 0; ; ++i)
        {
            if (i >= 10)
            {
                v43 = 1;
                goto LABEL_73;
            }
            if (i >= 3)
            {
                if (*(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4) == 255)
                {
                LABEL_59:
                    AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)JeTeParle__jtp_check_GST_Board_panel_gao, 1024, 0);
                    continue;
                }
                if (*(int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4) <= 10000)
                {
                    if (*(int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4) > 0)
                    {
                        v21 = (int*)(JeTeParle__jtp_check_univVars_Flag_speciaux_sauve + 4);
                        v5 = (int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4);
                        AI_EvalFunc_ARRBitSet_C(v21, *v5);
                    }
                }
                else if (*(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4) - 10000 > 0)
                {
                    v22 = (int*)(JeTeParle__jtp_check_univVars_Flag_speciaux_sauve + 4);
                    v4 = (uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4);
                    AI_EvalFunc_ARRBitClear_C(v22, *v4 - 10000);
                }
            }
            else
            {
                if (*(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_Dimension1, i, a4) == 255)
                    goto LABEL_59;
                if (*(int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_Dimension1, i, a4) <= 10000)
                {
                    if (*(int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_Dimension1, i, a4) > 0)
                    {
                        a1 = (int*)(JeTeParle__jtp_check_univVars_Flag_speciaux_sauve + 4);
                        v3 = (int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_Dimension1, i, a4);
                        AI_EvalFunc_ARRBitSet_C(a1, *v3);
                    }
                }
                else if (*(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_Dimension1, i, a4) - 10000 > 0)
                {
                    v24 = (int*)(JeTeParle__jtp_check_univVars_Flag_speciaux_sauve + 4);
                    v2 = (uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_Dimension1, i, a4);
                    AI_EvalFunc_ARRBitClear_C(v24, *v2 - 10000);
                }
            }
        }
    }
    v56->f_time_fin_talk = TIM_gf_MainClock - TIM_gf_LockTime;
    for (i = 0; i < 5; ++i)
        *(uint32_t*)(JeTeParle__jtp_check_GST_Board_panel_gao_ai_dialog_consequences + 4 * i + 4) = 0;
LABEL_73:
    if (v43)
    {
        *(uint32_t*)JeTeParle__jtp_check_GST_Board_panel_gao_o_dialogue_actor = (uint32_t)AI_gpst_CurrentGameObject;
        num = 0;
        for (i = 0; i < 20; ++i)
        {
            v20 = (int*)(JeTeParle__jtp_check_univVars_Flag_speciaux_sauve + 4);
            v17 = *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_conditions_Dimension1, 3, i);
            v12 = *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_conditions_Dimension1, 2, i);
            v11 = *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_conditions_Dimension1, 1, i);
            v6 = (int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_conditions_Dimension1, 0, i);
            v42 = AI_EvalFunc_ARRBit4Test_C(v20, *v6, v11, v12, v17);
            if (v42)
            {
                if (v56->ai_grisage[i])
                {
                    v13 = (const void*)AI_EvalVarArray2_C(10, (char*)&v56->__at_questionsReponses_Dimension1, 0, i);
                    v7 = AI_EvalVarArray_C(10, (char*)JeTeParle__jtp_check_GST_Board_panel_gao_at_dialog_questions, num);
                    memcpy(v7, v13, 8u);
                    v14 = (const void*)AI_EvalVarArray2_C(10, (char*)&v56->__at_questionsReponses_Dimension1, 1, i);
                    v8 = AI_EvalVarArray_C(10, (char*)JeTeParle__jtp_check_GST_Board_panel_gao_at_dialog_reponses, num);
                    memcpy(v8, v14, 8u);
                    *(uint32_t*)(JeTeParle__jtp_check_GST_Board_panel_gao_ai_dialog_grisage + 4 * num + 4) = v56->ai_grisage[i];
                    *(uint32_t*)(JeTeParle__jtp_check_GST_Board_panel_gao_ai_dialog_consequences + 4 * num++ + 4) = i + 1;
                }
            }
        }
        v15 = AI_EvalVarArray_C(10, (char*)&v56->__at_question_reponse_OK_Dimension1, 0);
        v9 = AI_EvalVarArray_C(10, (char*)JeTeParle__jtp_check_GST_Board_panel_gao_at_dialog_questions, num);
        memcpy(v9, v15, 8u);
        v16 = AI_EvalVarArray_C(10, (char*)&v56->__at_question_reponse_OK_Dimension1, 1);
        v10 = AI_EvalVarArray_C(10, (char*)JeTeParle__jtp_check_GST_Board_panel_gao_at_dialog_reponses, num);
        memcpy(v10, v16, 8u);
        *(uint32_t*)(JeTeParle__jtp_check_GST_Board_panel_gao_ai_dialog_grisage + 4 * num + 4) = -1;
        *(uint32_t*)(JeTeParle__jtp_check_GST_Board_panel_gao_ai_dialog_consequences + 4 * num + 4) = 253;
        for (i = num + 1; i < 20; ++i)
        {
            *(uint32_t*)(JeTeParle__jtp_check_GST_Board_panel_gao_ai_dialog_grisage + 4 * i + 4) = 0;
            *(uint32_t*)(JeTeParle__jtp_check_GST_Board_panel_gao_ai_dialog_consequences + 4 * i + 4) = 0;
        }
    }
    v56->m_dialogue.msgID = 0;
    return 1000;
}

int JeTeParle_jtp_rottete()
{
    double v0; // st7
    int result; // eax
    float v2; // [esp+0h] [ebp-124h]
    tScriptStruct_ActorJeTeParle* v3; // [esp+8h] [ebp-11Ch]
    Vec3* v4; // [esp+Ch] [ebp-118h]
    Vec3* v5; // [esp+10h] [ebp-114h]
    Vec3* v6; // [esp+30h] [ebp-F4h]
    Vec3* v7; // [esp+34h] [ebp-F0h]
    tGameObjectMain* v8; // [esp+54h] [ebp-D0h]
    tGameObjectMain* v9; // [esp+58h] [ebp-CCh]
    tGameObjectMain* v10; // [esp+60h] [ebp-C4h]
    Vec3 a4; // [esp+64h] [ebp-C0h] BYREF
    float v13; // [esp+74h] [ebp-B0h]
    float v14; // [esp+78h] [ebp-ACh]
    float v15; // [esp+7Ch] [ebp-A8h]
    Vec3 a2; // [esp+80h] [ebp-A4h] BYREF
    int v18; // [esp+9Ch] [ebp-88h]
    Vec3 v19; // [esp+B4h] [ebp-70h] BYREF
    float v20; // [esp+C0h] [ebp-64h]
    float a1; // [esp+C4h] [ebp-60h]
    float a3; // [esp+C8h] [ebp-5Ch]
    BOOL v23; // [esp+E4h] [ebp-40h]
    float v24; // [esp+E8h] [ebp-3Ch]
    tScriptStruct_ActorJeTeParle* v26; // [esp+F0h] [ebp-34h]
    Vec3 v27; // [esp+F4h] [ebp-30h] BYREF
    Vec3 v28; // [esp+10Ch] [ebp-18h] BYREF
    Vec3 v29; // [esp+118h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_ActorJeTeParle*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v26 = v3;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        JeTeParle_jtp_rottete__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        fct_JeTeParle_jtp_rottete_JeTeParle_jtp_exec_regarde = (int)AI_ul_RealResolveOneRefFunction(0x26000785u);
    }
    if (!AI_EvalFunc_ACTHasKit_C(AI_gpst_CurrentGameObject))
        goto LABEL_48;
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(1))
    {
        v26->des_i_RT_flags = 0x80010000;
        v23 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(2);
        v26->des_f_RT_vitesse_de_rotation = v23 ? 1.0f : 0.2f;
        v26->des_o_acteur_suividesyeux = v26->o_regard_capa;
    }
    else if (v26->i_regarde_si_talk && v26->i_talk_propose)
    {
        v26->des_i_RT_flags = 0x80010000;
        v26->des_f_RT_vitesse_de_rotation = v26->f_vitesse_je_regarde_Sally;
        v26->des_o_acteur_suividesyeux = JeTeParle_jtp_rottete__PJ_Sally_gao;
    }
    else
    {
        if (v26->f_dist_regard == 0.0f)
            goto LABEL_23;
        v10 = (tGameObjectMain*)JeTeParle_jtp_rottete__PJ_Sally_gao;
        if (JeTeParle_jtp_rottete__PJ_Sally_gao)
        {
            if ((unsigned int)JeTeParle_jtp_rottete__PJ_Sally_gao <= 2)
                v10 = AI_MainActor[JeTeParle_jtp_rottete__PJ_Sally_gao - 1];
            v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v10);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
        }
        v13 = (float)v0;
        if (v26->f_dist_regard * v26->f_dist_regard <= v13)
        {
        LABEL_23:
            v9 = (tGameObjectMain*)v26->o_perso_a_regarder;
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
                v26->des_i_RT_flags = 0x80010000;
                v26->des_f_RT_vitesse_de_rotation = v26->f_vitesse_je_regarde_Sally;
                v26->des_o_acteur_suividesyeux = v26->o_perso_a_regarder;
            }
            else if (AI_EvalFunc_ACTCustomBitTest_C(AI_gpst_CurrentGameObject, 8u))
            {
                v19.x = 0.0;
                v19.y = 0.0;
                v19.z = 0.0;
                if (AI2C_DistOfTwoPointsIsZero(&v26->macro_regard_aleatoire_v_curr, &v19))
                {
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v26->macro_regard_aleatoire_v_curr);
                    v6 = &v26->macro_regard_aleatoire_v_targ;
                    v7 = &v26->macro_regard_aleatoire_v_curr;
                    v26->macro_regard_aleatoire_v_targ.x = v26->macro_regard_aleatoire_v_curr.x;
                    v6->y = v7->y;
                    v6->z = v7->z;
                    v26->macro_regard_aleatoire_i_cpt = 4;
                }
                v15 = v26->macro_regard_aleatoire_v_targ.z * v26->macro_regard_aleatoire_v_curr.z
                    + v26->macro_regard_aleatoire_v_targ.y * v26->macro_regard_aleatoire_v_curr.y
                    + v26->macro_regard_aleatoire_v_curr.x * v26->macro_regard_aleatoire_v_targ.x;
                if (v15 > 0.99900001)
                {
                    if (v26->macro_regard_aleatoire_i_cpt > 0)
                    {
                        --v26->macro_regard_aleatoire_i_cpt;
                        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v27);
                        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&a2);
                        a1 = AI_EvalFunc_MATHRandFloat_C(-1.0, 1.0);
                        v14 = AI_EvalFunc_MTHFloatSign_C(a1);
                        v29.x = a2.x * v14;
                        v29.y = a2.y * v14;
                        v29.z = a2.z * v14;
                        a3 = AI_EvalFunc_MATHRandFloat_C(0.0, 0.80000001f);
                        AI_EvalFunc_MATHVecBlend_C(&v27, &v29, a3, &v26->macro_regard_aleatoire_v_targ);
                        v26->macro_regard_aleatoire_v_targ.z = AI_EvalFunc_MATHRandFloat_C(-0.5, 0.80000001f)
                            + v26->macro_regard_aleatoire_v_targ.z;
                        MATH_NormalizeEqualVector(&v26->macro_regard_aleatoire_v_targ);
                    }
                    else
                    {
                        v26->macro_regard_aleatoire_i_cpt = 4;
                        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v26->macro_regard_aleatoire_v_targ);
                    }
                    AI_EvalFunc_MATHVecBlend_C(&v26->macro_regard_aleatoire_v_curr, &v26->macro_regard_aleatoire_v_targ, 0.5, &a4);
                    AI_EvalFunc_MATHVecNormalize_C(&a4, &v26->macro_regard_aleatoire_v_inte);
                }
                v24 = v26->macro_regard_aleatoire_v_inte.z * v26->macro_regard_aleatoire_v_curr.z
                    + v26->macro_regard_aleatoire_v_inte.y * v26->macro_regard_aleatoire_v_curr.y
                    + v26->macro_regard_aleatoire_v_curr.x * v26->macro_regard_aleatoire_v_inte.x;
                v20 = TIM_gf_dt;
                v2 = v24 * TIM_gf_dt * 1.25f;
                AI_EvalFunc_MATHVecBlend_C(&v26->macro_regard_aleatoire_v_curr, &v26->macro_regard_aleatoire_v_targ, v2, &v28);
                AI_EvalFunc_MATHVecNormalize_C(&v28, &v26->macro_regard_aleatoire_v_curr);
                v26->des_o_acteur_suividesyeux = (int)AI_gpst_CurrentGameObject;
                v26->des_i_RT_flags = -2147418112;
                v4 = &v26->des_v_hauteur_cible;
                v5 = &v26->macro_regard_aleatoire_v_curr;
                v26->des_v_hauteur_cible.x = v26->macro_regard_aleatoire_v_curr.x;
                v4->y = v5->y;
                v4->z = v5->z;
            }
            else
            {
                v26->des_o_acteur_suividesyeux = 0;
            }
        }
        else
        {
            v26->des_i_RT_flags = 0x80010000;
            v26->des_f_RT_vitesse_de_rotation = v26->f_vitesse_je_regarde_Sally;
            v26->des_o_acteur_suividesyeux = JeTeParle_jtp_rottete__PJ_Sally_gao;
        }
    }
    if (v26->i_ne_tourne_que_la_tete)
        v26->des_i_RT_flags |= 0x10000000u;
    if (v26->i_custom_limit)
        v26->des_i_RT_flags |= 0x8000000u;
    if (v26->i_regarder_dans_le_dos)
        v26->des_i_RT_flags &= ~0x80000000;
    v18 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_JeTeParle_jtp_rottete_JeTeParle_jtp_exec_regarde);
    if (v18 == 1000)
        LABEL_48:
    result = 1000;
    else
        result = v18;
    return result;
}

int JeTeParle_jtp_mamago_presente_objets()
{
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        JeTeParle_jtp_mamago_presente_objets_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
    }
    if (AI_EvalFunc_OBJ_CustomBitsTest_C(
        (tGameObjectMain*)JeTeParle_jtp_mamago_presente_objets_GST_Board_panel_gao,
        0x800))
    {
        AI_EvalFunc_ACTActionSet_C_CURRENT(7);
    }
    else
    {
        AI_EvalFunc_ACTActionSet_C_CURRENT(6);
    }
    AI_EvalFunc_OBJ_EyeFollowParamSet_C(1.0471981f, -1.0471981f, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0);
    return 1000;
}

int JeTeParle_jtp_mamago_bricole_AB()
{
    float v1; // [esp+0h] [ebp-58h]
    tScriptStruct_ActorJeTeParle* v2; // [esp+4h] [ebp-54h]
    tGameObjectMain* v3; // [esp+8h] [ebp-50h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-4Ch]
    Vec3 v5; // [esp+10h] [ebp-48h] BYREF
    Vec3 a3; // [esp+1Ch] [ebp-3Ch] BYREF
    tGameObjectMain* v7; // [esp+28h] [ebp-30h]
    float v8; // [esp+2Ch] [ebp-2Ch]
    int v9; // [esp+30h] [ebp-28h]
    Vec3 dest; // [esp+34h] [ebp-24h] BYREF
    Vec3 a2; // [esp+40h] [ebp-18h] BYREF
    tScriptStruct_ActorJeTeParle* v12; // [esp+50h] [ebp-8h]
    float a1; // [esp+54h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorJeTeParle*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v12 = v2;
    v7 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_JeTeParle_jtp_mamago_bricole_AB_JeTeParle_jtp_rottete = (int)AI_ul_RealResolveOneRefFunction(0x26003094u);
        JeTeParle_jtp_mamago_bricole_AB__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
    }
    v9 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_JeTeParle_jtp_mamago_bricole_AB_JeTeParle_jtp_rottete);
    if (v9 != 1000)
        return v9;
    v7 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0);
    if (v12->f_angle != 0.0f)
    {
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&a2);
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)JeTeParle_jtp_mamago_bricole_AB__PJ_Sally_gao, &a3);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v5);
        MATH_SubstractVec(&dest, &a3, &v5);
        a1 = dest.z * a2.z + dest.y * a2.y + a2.x * dest.x;
        v8 = AI_EvalFunc_MTHFloatSign_C(a1);
        v4 = v7;
        if (v7)
        {
            if ((unsigned int)v7 <= 2)
                v4 = AI_MainActor[(uint32_t)v7 - 1];
            v3 = v4;
        }
        else
        {
            v3 = 0;
        }
        v1 = v12->f_angle * v8;
        AI_EvalFunc_OBJRotateLocalZ_c(v3, v1);
    }
    return 1000;
};

int JeTeParle_jtp_palet_posperle()
{
    tScriptStruct_ActorJeTeParle* v1; // [esp+0h] [ebp-D4h]
    tGameObjectMain* v2; // [esp+8h] [ebp-CCh]
    tGameObjectMain* v3; // [esp+10h] [ebp-C4h]
    tGameObjectMain* v4; // [esp+18h] [ebp-BCh]
    tGameObjectMain* v5; // [esp+28h] [ebp-ACh]
    tGameObjectMain* v6; // [esp+30h] [ebp-A4h]
    tGameObjectMain* v7; // [esp+38h] [ebp-9Ch]
    tGameObjectMain* v8; // [esp+40h] [ebp-94h]
    tGameObjectMain* v9; // [esp+50h] [ebp-84h]
    tGameObjectMain* v10; // [esp+54h] [ebp-80h]
    tGameObjectMain* v11; // [esp+58h] [ebp-7Ch]
    tGameObjectMain* v12; // [esp+5Ch] [ebp-78h]
    Vec3 a3; // [esp+60h] [ebp-74h] BYREF
    Vec3 a1; // [esp+70h] [ebp-64h] BYREF
    Vec3 v15; // [esp+80h] [ebp-54h] BYREF
    Vec3 v16; // [esp+8Ch] [ebp-48h] BYREF
    Vec3 v17; // [esp+98h] [ebp-3Ch] BYREF
    tGameObjectMain* v18; // [esp+A4h] [ebp-30h]
    int v19; // [esp+ACh] [ebp-28h]
    tScriptStruct_ActorJeTeParle* v20; // [esp+B4h] [ebp-20h]
    BOOL v21; // [esp+B8h] [ebp-1Ch]
    Vec3 v22; // [esp+BCh] [ebp-18h] BYREF
    float v23; // [esp+C8h] [ebp-Ch]
    float v24; // [esp+CCh] [ebp-8h]
    float v25; // [esp+D0h] [ebp-4h]

    v15.x = 1.0;
    v15.y = 0.0;
    v15.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ActorJeTeParle*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v20 = v1;
    v19 = 0;
    v18 = 0;
    v21 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        JeTeParle_jtp_palet_posperle_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &JeTeParle_jtp_palet_posperle_univVars_Flag_speciaux_sauve,
            (void*)JeTeParle_jtp_palet_posperle_univVars,
            4488);
    }
    v12 = (tGameObjectMain*)v20->des_o_jesuisla_desactive;
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
        v10 = (tGameObjectMain*)v20->des_o_jesuisla_active;
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
            v19 = 0;
            v21 = AI_EvalFunc_ARRBitTest_C((int*)(JeTeParle_jtp_palet_posperle_univVars_Flag_speciaux_sauve + 4), 2838);
            if (v21)
            {
                v21 = AI_EvalFunc_ARRBitTest_C((int*)(JeTeParle_jtp_palet_posperle_univVars_Flag_speciaux_sauve + 4), 2839);
                if (v21)
                {
                    v21 = AI_EvalFunc_ARRBitTest_C((int*)(JeTeParle_jtp_palet_posperle_univVars_Flag_speciaux_sauve + 4), 2840);
                    if (!v21)
                        v19 = 1;
                }
            }
            else
            {
                v19 = 1;
            }
            v8 = (tGameObjectMain*)v20->des_o_jesuisla_desactive;
            if (v8)
            {
                if ((unsigned int)v8 <= 2)
                    v8 = AI_MainActor[(uint32_t)v8 - 1];
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(v8, 1 - v19);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1 - v19);
            }
            v7 = (tGameObjectMain*)v20->des_o_jesuisla_active;
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(v7, 1 - v19);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1 - v19);
            }
            if (v19)
            {
                v18 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0);
                v6 = v18;
                if (v18)
                {
                    if ((unsigned int)v18 <= 2)
                        v6 = AI_MainActor[(uint32_t)v18 - 1];
                    AI_EvalFunc_OBJGetPos_C(v6, &a3);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &a3);
                }
                v5 = v18;
                if (v18)
                {
                    if ((unsigned int)v18 <= 2)
                        v5 = AI_MainActor[(uint32_t)v18 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v5, &v16);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v16);
                }
                v22.x = v16.x * 0.5f;
                v22.y = v16.y * 0.5f;
                v22.z = v16.z * 0.5f;
                MATH_VecAddVec(&a1, &a3, &v22);
                v15 = a1;
                v4 = v18;
                if (v18)
                {
                    if ((unsigned int)v18 <= 2)
                        v4 = AI_MainActor[(uint32_t)v18 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v4, &v17);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v17);
                }
                v23 = v17.x * 0.050000001f;
                v24 = v17.y * 0.050000001f;
                v25 = v17.z * 0.050000001f;
                v15.x = v15.x + v23;
                v15.y = v15.y + v24;
                v15.z = v15.z + v25;
                v3 = (tGameObjectMain*)v20->des_o_jesuisla_desactive;
                if (v3)
                {
                    if ((unsigned int)v3 <= 2)
                        v3 = AI_MainActor[(uint32_t)v3 - 1];
                    AI_EvalFunc_OBJSetPos_C(v3, &v15);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &v15);
                }
                v2 = (tGameObjectMain*)v20->des_o_jesuisla_active;
                if (v2)
                {
                    if ((unsigned int)v2 <= 2)
                        v2 = AI_MainActor[(uint32_t)v2 - 1];
                    AI_EvalFunc_OBJSetPos_C(v2, &v15);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &v15);
                }
            }
        }
    }
    return 1000;
};

int JeTeParle_jtp_revol_AB()
{
    tScriptStruct_ActorJeTeParle* v1; // [esp+0h] [ebp-2Ch]
    tGameObjectMain* v2; // [esp+8h] [ebp-24h]
    tGameObjectMain* v3; // [esp+10h] [ebp-1Ch]
    Vec3 a3; // [esp+1Ch] [ebp-10h] BYREF
    tScriptStruct_ActorJeTeParle* v5; // [esp+28h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ActorJeTeParle*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v5 = v1;
    v3 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(v1->i_shape_main_revol);
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        AI_EvalFunc_OBJGetPos_C(v3, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    v2 = (tGameObjectMain*)v5->des_o_panneau;
    if (v2)
    {
        if ((unsigned int)v2 <= 2)
            v2 = AI_MainActor[(uint32_t)v2 - 1];
        AI_EvalFunc_OBJSetPos_C(v2, &a3);
    }
    else
    {
        AI_EvalFunc_OBJSetPos_C(0, &a3);
    }
    return 1000;
};

int Dialogue_dialogue_check()
{
    double v1; // st7
    uint32_t* v2; // eax
    int* v3; // eax
    uint32_t* v4; // eax
    int* v5; // eax
    int* v6; // eax
    char* v7; // eax
    char* v8; // eax
    char* v9; // eax
    char* v10; // eax
    int v11; // [esp-4h] [ebp-1A8h]
    int v12; // [esp+0h] [ebp-1A4h]
    const void* v13; // [esp+0h] [ebp-1A4h]
    const void* v14; // [esp+0h] [ebp-1A4h]
    char* v15; // [esp+0h] [ebp-1A4h]
    char* v16; // [esp+0h] [ebp-1A4h]
    int v17; // [esp+4h] [ebp-1A0h]
    float v18; // [esp+Ch] [ebp-198h]
    tScriptStruct_ActorJeTeParle* v19; // [esp+10h] [ebp-194h]
    int* v20; // [esp+14h] [ebp-190h]
    int* v21; // [esp+1Ch] [ebp-188h]
    int* v22; // [esp+24h] [ebp-180h]
    int* a1; // [esp+2Ch] [ebp-178h]
    int* v24; // [esp+34h] [ebp-170h]
    double v25; // [esp+3Ch] [ebp-168h]
    double v26; // [esp+44h] [ebp-160h]
    double v27; // [esp+54h] [ebp-150h]
    tGameObjectMain* v28; // [esp+70h] [ebp-134h]
    tGameObjectMain* v29; // [esp+74h] [ebp-130h]
    tGameObjectMain* v30; // [esp+78h] [ebp-12Ch]
    tGameObjectMain* v31; // [esp+7Ch] [ebp-128h]
    tGameObjectMain* v32; // [esp+80h] [ebp-124h]
    Vec3 a3; // [esp+90h] [ebp-114h] BYREF
    Vec3 a2; // [esp+9Ch] [ebp-108h] BYREF
    float v35; // [esp+ACh] [ebp-F8h]
    float v36; // [esp+B4h] [ebp-F0h]
    int v37; // [esp+B8h] [ebp-ECh]
    float v38; // [esp+BCh] [ebp-E8h]
    Vec3 dest; // [esp+D0h] [ebp-D4h] BYREF
    Vec3 v40; // [esp+DCh] [ebp-C8h] BYREF
    BOOL v41; // [esp+E8h] [ebp-BCh]
    BOOL v42; // [esp+F0h] [ebp-B4h]
    int v43; // [esp+F4h] [ebp-B0h]
    int v44; // [esp+F8h] [ebp-ACh]
    int v45; // [esp+FCh] [ebp-A8h]
    int v46; // [esp+100h] [ebp-A4h]
    Vec3 v47; // [esp+104h] [ebp-A0h] BYREF
    tMessage Src; // [esp+110h] [ebp-94h] BYREF
    Vec3 v50; // [esp+14Ch] [ebp-58h]
    BOOL v51; // [esp+158h] [ebp-4Ch]
    int i; // [esp+15Ch] [ebp-48h]
    int a4; // [esp+160h] [ebp-44h]
    int num; // [esp+164h] [ebp-40h]
    float v55; // [esp+16Ch] [ebp-38h]
    tScriptStruct_ActorJeTeParle* v56; // [esp+170h] [ebp-34h]
    BOOL v57; // [esp+174h] [ebp-30h]
    Vec3 v58; // [esp+178h] [ebp-2Ch] BYREF
    Vec3 v59; // [esp+184h] [ebp-20h] BYREF
    int v60; // [esp+19Ch] [ebp-8h]

    v50.x = 1.0;
    v50.y = 0.0;
    v50.z = 0.0;
    v40.x = 1.0;
    v40.y = 0.0;
    v40.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v19 = (tScriptStruct_ActorJeTeParle*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v19 = 0;
    v56 = v19;
    v51 = 0;
    i = 0;
    v60 = 0;
    v42 = 0;
    v43 = 0;
    v38 = 0.0;
    v44 = 0;
    a4 = 0;
    v45 = 0;
    num = 0;
    v57 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Dialogue_dialogue_check__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Dialogue_dialogue_check_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &Dialogue_dialogue_check__PJ_Sally_gao_current_pm_mode,
            (void*)Dialogue_dialogue_check__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(
            &Dialogue_dialogue_check__PJ_Sally_gao_current_pm_priority,
            (void*)Dialogue_dialogue_check__PJ_Sally_gao,
            576);
        AI2C_ResolveBuffer(
            &Dialogue_dialogue_check_GST_Board_panel_gao_i_sally_priority,
            (void*)Dialogue_dialogue_check_GST_Board_panel_gao,
            396);
        AI2C_ResolveBuffer(
            &Dialogue_dialogue_check_GST_Board_panel_gao_o_sally_contextuel,
            (void*)Dialogue_dialogue_check_GST_Board_panel_gao,
            388);
        AI2C_ResolveBuffer(
            &Dialogue_dialogue_check_GST_Board_panel_gao_i_sally_action,
            (void*)Dialogue_dialogue_check_GST_Board_panel_gao,
            412);
        AI2C_ResolveBuffer(
            &Dialogue_dialogue_check_GST_Board_panel_gao_v_sally_offset,
            (void*)Dialogue_dialogue_check_GST_Board_panel_gao,
            400);
        AI2C_ResolveBuffer(
            &Dialogue_dialogue_check_GST_Board_panel_gao_i_force_interface,
            (void*)Dialogue_dialogue_check_GST_Board_panel_gao,
            3292);
        Dialogue_dialogue_check_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Dialogue_dialogue_check_univVars_Flag_speciaux_sauve,
            (void*)Dialogue_dialogue_check_univVars,
            4488);
        Dialogue_dialogue_check_TXT_Sally_gao = AI_ul_RealResolveOneRefObject(0x7200064Eu);
        AI2C_ResolveBuffer(
            &Dialogue_dialogue_check_GST_Board_panel_gao_ai_dialog_consequences,
            (void*)Dialogue_dialogue_check_GST_Board_panel_gao,
            1432);
        AI2C_ResolveBuffer(
            &Dialogue_dialogue_check_GST_Board_panel_gao_o_dialogue_actor,
            (void*)Dialogue_dialogue_check_GST_Board_panel_gao,
            1100);
        AI2C_ResolveBuffer(
            &Dialogue_dialogue_check_GST_Board_panel_gao_at_dialog_questions,
            (void*)Dialogue_dialogue_check_GST_Board_panel_gao,
            1104);
        AI2C_ResolveBuffer(
            &Dialogue_dialogue_check_GST_Board_panel_gao_at_dialog_reponses,
            (void*)Dialogue_dialogue_check_GST_Board_panel_gao,
            1268);
        AI2C_ResolveBuffer(
            &Dialogue_dialogue_check_GST_Board_panel_gao_ai_dialog_grisage,
            (void*)Dialogue_dialogue_check_GST_Board_panel_gao,
            1516);
    }
    v32 = (tGameObjectMain*)v56->o_papa;
    if (v32)
    {
        if ((unsigned int)v32 <= 2)
            v32 = AI_MainActor[(uint32_t)v32 - 1];
        v31 = v32;
    }
    else
    {
        v31 = 0;
    }
    if (v31)
    {
        v30 = (tGameObjectMain*)v56->o_papa;
        if (v30)
        {
            if ((unsigned int)v30 <= 2)
                v30 = AI_MainActor[(uint32_t)v30 - 1];
            v29 = v30;
        }
        else
        {
            v29 = 0;
        }
        v37 = (unsigned __int16)v29->flags2;
        if ((v37 & 1) == 0)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
    }
    v56->i_talk_propose = 0;
    v28 = (tGameObjectMain*)Dialogue_dialogue_check__PJ_Sally_gao;
    if (Dialogue_dialogue_check__PJ_Sally_gao)
    {
        if ((unsigned int)Dialogue_dialogue_check__PJ_Sally_gao <= 2)
            v28 = AI_MainActor[Dialogue_dialogue_check__PJ_Sally_gao - 1];
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v28);
    }
    else
    {
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v55 = (float)v1;
    if (v56->des_f_distance_talk * v56->des_f_distance_talk > v55
        && !AI_EvalFunc_OBJ_CustomBitsTest_C((tGameObjectMain*)Dialogue_dialogue_check_GST_Board_panel_gao, 2048))
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Dialogue_dialogue_check__PJ_Sally_gao, &a3);
        MATH_SubstractVec(&dest, &a2, &a3);
        v40 = dest;
        AI_EvalFunc_MATHVecSetHorzNormalize_C(&v40);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v59);
        v35 = v40.z * v59.z + v40.y * v59.y + v59.x * v40.x;
        if (v56->des_f_angle_talk <= (double)v35)
        {
            AI_EvalFunc_MATHVecNormalize_C(&v40, &v47);
            v50 = v47;
            AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Dialogue_dialogue_check__PJ_Sally_gao, &v58);
            v36 = v50.z * v58.z + v50.y * v58.y + v58.x * v50.x;
            v38 = v36 * 5.0f;
            v27 = v38 + -10.0 + MATH_gd_magicDoubleForRound;
            v60 = (*(uint32_t *)&v27);
            v46 = (unsigned __int16)*(uint32_t*)(Dialogue_dialogue_check__PJ_Sally_gao + 4);
            if ((v46 & 1) != 0)
                v57 = !*(uint32_t*)Dialogue_dialogue_check__PJ_Sally_gao_current_pm_mode
                || *(int*)Dialogue_dialogue_check__PJ_Sally_gao_current_pm_priority < 20;
            else
                v57 = 0;
            if (v57)
            {
                v18 = (float)v60;
                if (v18 > (double)*(float*)Dialogue_dialogue_check_GST_Board_panel_gao_i_sally_priority)
                {
                    *(uint32_t*)Dialogue_dialogue_check_GST_Board_panel_gao_o_sally_contextuel = (uint32_t)AI_gpst_CurrentGameObject;
                    *(float*)Dialogue_dialogue_check_GST_Board_panel_gao_i_sally_priority = (float)v60;
                    *(uint32_t*)Dialogue_dialogue_check_GST_Board_panel_gao_i_sally_action = 5;
                    *(uint32_t*)Dialogue_dialogue_check_GST_Board_panel_gao_v_sally_offset = 0;
                    *(uint32_t*)(Dialogue_dialogue_check_GST_Board_panel_gao_v_sally_offset + 4) = 0;
                    *(uint32_t*)(Dialogue_dialogue_check_GST_Board_panel_gao_v_sally_offset + 8) = 1065353216;
                    *(uint32_t*)Dialogue_dialogue_check_GST_Board_panel_gao_i_force_interface = 1;
                }
            }
            v56->i_talk_propose = v57;
        }
    }
    v43 = 0;
    v44 = 0;
    if (v56->m_dialogue.msgID == 10 && (v41 = AI_EvalFunc_TIMEElapsed_C(v56->f_time_fin_talk, 0.5)))
    {
        v51 = AI_EvalFunc_ARRBitTest_C((int*)(Dialogue_dialogue_check_univVars_Flag_speciaux_sauve + 4), 2083);
        if (v56->i_interdit_pendant_mili_malad && v51)
        {
            Src.msgID = 30;
            Src.Int1 = 60;
            Src.Int2 = 0;
            Src.f2 = 1.0;
            Src.v1.x = 0.0;
            Src.v1.y = 0.0;
            Src.v1.z = 0.0;
            Src.v2.x = 0.0;
            Src.v2.y = 0.0;
            Src.v2.z = 0.0;
            Src.gao = 0;
            Src.gao2 = AI_gpst_CurrentGameObject;
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)Dialogue_dialogue_check_TXT_Sally_gao, &Src);
            v56->m_dialogue.msgID = 0;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v43 = 1;
        if (v56->Valide_Effet_1_Question)
            AI_EvalFunc_OBJ_CapaSet_C((tGameObjectMain*)Dialogue_dialogue_check_GST_Board_panel_gao, 128, 0);
        else
            AI_EvalFunc_OBJ_CapaSet_C((tGameObjectMain*)Dialogue_dialogue_check_GST_Board_panel_gao, 0, 128);
    }
    else if (v56->m_dialogue.msgID == 20)
    {
        v44 = 1;
    }
    if (!v44)
        goto LABEL_73;
    v26 = v56->m_dialogue.f2 - 1.0 + MATH_gd_magicDoubleForRound;
    a4 = (*(uint32_t*)&v26);
    v25 = v56->m_dialogue.f1 + MATH_gd_magicDoubleForRound;
    v45 = (*(uint8_t*)&v25);
    if ((*(uint8_t*)&v25) == 255)
    {
        v56->f_time_fin_talk = TIM_gf_MainClock - TIM_gf_LockTime;
        v43 = 1;
        goto LABEL_73;
    }
    if (v45 != 254 && v45 != 253)
    {
        a4 = v45 - 1;
        for (i = 0; ; ++i)
        {
            if (i >= 10)
            {
                v43 = 1;
                goto LABEL_73;
            }
            if (i >= 3)
            {
                if (*(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4) == 255)
                {
                LABEL_59:
                    AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)Dialogue_dialogue_check_GST_Board_panel_gao, 1024, 0);
                    continue;
                }
                if (*(int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4) <= 10000)
                {
                    if (*(int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4) > 0)
                    {
                        v21 = (int*)(Dialogue_dialogue_check_univVars_Flag_speciaux_sauve + 4);
                        v5 = (int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4);
                        AI_EvalFunc_ARRBitSet_C(v21, *v5);
                    }
                }
                else if (*(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4) - 10000 > 0)
                {
                    v22 = (int*)(Dialogue_dialogue_check_univVars_Flag_speciaux_sauve + 4);
                    v4 = (uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4);
                    AI_EvalFunc_ARRBitClear_C(v22, *v4 - 10000);
                }
            }
            else
            {
                if (*(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_Dimension1, i, a4) == 255)
                    goto LABEL_59;
                if (*(int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_Dimension1, i, a4) <= 10000)
                {
                    if (*(int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_Dimension1, i, a4) > 0)
                    {
                        a1 = (int*)(Dialogue_dialogue_check_univVars_Flag_speciaux_sauve + 4);
                        v3 = (int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_Dimension1, i, a4);
                        AI_EvalFunc_ARRBitSet_C(a1, *v3);
                    }
                }
                else if (*(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_Dimension1, i, a4) - 10000 > 0)
                {
                    v24 = (int*)(Dialogue_dialogue_check_univVars_Flag_speciaux_sauve + 4);
                    v2 = (uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_Dimension1, i, a4);
                    AI_EvalFunc_ARRBitClear_C(v24, *v2 - 10000);
                }
            }
        }
    }
    v56->f_time_fin_talk = TIM_gf_MainClock - TIM_gf_LockTime;
    for (i = 0; i < 5; ++i)
        *(uint32_t*)(Dialogue_dialogue_check_GST_Board_panel_gao_ai_dialog_consequences + 4 * i + 4) = 0;
LABEL_73:
    if (v43)
    {
        *(uint32_t*)Dialogue_dialogue_check_GST_Board_panel_gao_o_dialogue_actor = (uint32_t)AI_gpst_CurrentGameObject;
        num = 0;
        for (i = 0; i < 20; ++i)
        {
            v20 = (int*)(Dialogue_dialogue_check_univVars_Flag_speciaux_sauve + 4);
            v17 = *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_conditions_Dimension1, 3, i);
            v12 = *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_conditions_Dimension1, 2, i);
            v11 = *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_conditions_Dimension1, 1, i);
            v6 = (int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_conditions_Dimension1, 0, i);
            v42 = AI_EvalFunc_ARRBit4Test_C(v20, *v6, v11, v12, v17);
            if (v42)
            {
                if (v56->ai_grisage[i])
                {
                    v13 = (const void*)AI_EvalVarArray2_C(10, (char*)&v56->__at_questionsReponses_Dimension1, 0, i);
                    v7 = AI_EvalVarArray_C(10, (char*)Dialogue_dialogue_check_GST_Board_panel_gao_at_dialog_questions, num);
                    memcpy(v7, v13, 8u);
                    v14 = (const void*)AI_EvalVarArray2_C(10, (char*)&v56->__at_questionsReponses_Dimension1, 1, i);
                    v8 = AI_EvalVarArray_C(10, (char*)Dialogue_dialogue_check_GST_Board_panel_gao_at_dialog_reponses, num);
                    memcpy(v8, v14, 8u);
                    *(uint32_t*)(Dialogue_dialogue_check_GST_Board_panel_gao_ai_dialog_grisage + 4 * num + 4) = v56->ai_grisage[i];
                    *(uint32_t*)(Dialogue_dialogue_check_GST_Board_panel_gao_ai_dialog_consequences + 4 * num++ + 4) = i + 1;
                }
            }
        }
        v15 = AI_EvalVarArray_C(10, (char*)&v56->__at_question_reponse_OK_Dimension1, 0);
        v9 = AI_EvalVarArray_C(10, (char*)Dialogue_dialogue_check_GST_Board_panel_gao_at_dialog_questions, num);
        memcpy(v9, v15, 8u);
        v16 = AI_EvalVarArray_C(10, (char*)&v56->__at_question_reponse_OK_Dimension1, 1);
        v10 = AI_EvalVarArray_C(10, (char*)Dialogue_dialogue_check_GST_Board_panel_gao_at_dialog_reponses, num);
        memcpy(v10, v16, 8u);
        *(uint32_t*)(Dialogue_dialogue_check_GST_Board_panel_gao_ai_dialog_grisage + 4 * num + 4) = -1;
        *(uint32_t*)(Dialogue_dialogue_check_GST_Board_panel_gao_ai_dialog_consequences + 4 * num + 4) = 253;
        for (i = num + 1; i < 20; ++i)
        {
            *(uint32_t*)(Dialogue_dialogue_check_GST_Board_panel_gao_ai_dialog_grisage + 4 * i + 4) = 0;
            *(uint32_t*)(Dialogue_dialogue_check_GST_Board_panel_gao_ai_dialog_consequences + 4 * i + 4) = 0;
        }
    }
    v56->m_dialogue.msgID = 0;
    return 1000;
}