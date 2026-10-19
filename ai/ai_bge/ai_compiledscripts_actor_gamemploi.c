#include "ai_compiledscripts_inc.h"

//Gam_emploi_du_temps_edt_init
int Gam_emploi_du_temps_edt_init_INV_manager_gao;
int Gam_emploi_du_temps_edt_init_INV_manager_gao_i_Animals_Nb;
int Gam_emploi_du_temps_edt_init_INV_manager_gao_ao_Animals;
int Gam_emploi_du_temps_edt_init_INV_manager_gao_ai_AnimalsData;
int Gam_emploi_du_temps_edt_init_univVars;
int Gam_emploi_du_temps_edt_init_univVars_Flag_speciaux_sauve;
int fct_Gam_emploi_du_temps_edt_init_Gam_emploi_du_temps_edt_tag_off;
int fct_Gam_emploi_du_temps_edt_init_Gam_emploi_du_temps_edt_reflex;
int fct_Gam_emploi_du_temps_edt_init_Gam_emploi_du_temps_edt_tag_on;
int fct_Gam_emploi_du_temps_edt_init_Gam_emploi_du_temps_edt_after_blend;
int Gam_emploi_du_temps_edt_init_univVars_Planetes_CurHour;

//Gam_emploi_du_temps_edt_reflex
int Gam_emploi_du_temps_edt_reflex_univVars_Planetes_CurHour;
int Gam_emploi_du_temps_edt_reflex_univVars;
int fct_Gam_emploi_du_temps_edt_reflex_Gam_emploi_du_temps_edt_exec_next_edt;
int fct_Gam_emploi_du_temps_edt_reflex_Gam_emploi_du_temps_edt_exec_cherche_dest;
int fct_Gam_emploi_du_temps_edt_reflex_Gam_emploi_du_temps_edt_dialogue_check;
int Gam_emploi_du_temps_edt_reflex__PJ_Sally_gao;
int fct_Gam_emploi_du_temps_edt_reflex_Gam_emploi_du_temps_edt_tag_on;

//Gam_emploi_du_temps_edt_exec_track_occupation
int fct_Gam_emploi_du_temps_edt_exec_track_occupation_Gam_emploi_du_temps_edt_occupation_dodo;
int fct_Gam_emploi_du_temps_edt_exec_track_occupation_Gam_emploi_du_temps_edt_occupation_jump;
int fct_Gam_emploi_du_temps_edt_exec_track_occupation_Gam_emploi_du_temps_edt_occupation_loup_init;
int fct_Gam_emploi_du_temps_edt_exec_track_occupation_Gam_emploi_du_temps_edt_occupation_regarde;
int fct_Gam_emploi_du_temps_edt_exec_track_occupation_Gam_emploi_du_temps_edt_occupation_mange;

//Gam_emploi_du_temps_edt_occupation_loup_init
int fct_Gam_emploi_du_temps_edt_occupation_loup_init_Gam_emploi_du_temps_edt_occupation_loup_nargue;
int fct_Gam_emploi_du_temps_edt_occupation_loup_init_Gam_emploi_du_temps_edt_occupation_loup_attentejoueur;

//Gam_emploi_du_temps_edt_occupation_loup_attentejoueur
int fct_Gam_emploi_du_temps_edt_occupation_loup_attentejoueur_Gam_emploi_du_temps_edt_exec_get_free_player;
int fct_Gam_emploi_du_temps_edt_occupation_loup_attentejoueur_Gam_emploi_du_temps_edt_occupation_loup_poursuite;

//Gam_emploi_du_temps_edt_occupation_loup_poursuite
int fct_Gam_emploi_du_temps_edt_occupation_loup_poursuite_Gam_emploi_du_temps_edt_occupation_loup_attentejoueur;
int fct_Gam_emploi_du_temps_edt_occupation_loup_poursuite_Gam_emploi_du_temps_edt_occupation_loup_nargue;
int fct_Gam_emploi_du_temps_edt_occupation_loup_poursuite_Gam_emploi_du_temps_edt_exec_recherche_chemin;

//Gam_emploi_du_temps_edt_exec_cherche_dest
int fct_Gam_emploi_du_temps_edt_exec_cherche_dest_Gam_emploi_du_temps_edt_exec_track_occupation;
int fct_Gam_emploi_du_temps_edt_exec_cherche_dest_Gam_emploi_du_temps_edt_deplacement_direct;
int fct_Gam_emploi_du_temps_edt_exec_cherche_dest_Gam_emploi_du_temps_edt_deplacement;

//Gam_emploi_du_temps_edt_deplacement
int fct_Gam_emploi_du_temps_edt_deplacement_Gam_emploi_du_temps_edt_exec_recherche_chemin;

//Gam_emploi_du_temps_edt_deplacement_direct
int fct_Gam_emploi_du_temps_edt_deplacement_direct_Gam_emploi_du_temps_edt_exec_track_occupation;

//Gam_emploi_du_temps_edt_after_blend
int Gam_emploi_du_temps_edt_after_blend__PJ_Sally_gao;;
int fct_Gam_emploi_du_temps_edt_after_blend_Gam_emploi_du_temps_edt_exec_suivi_regard;

//Gam_emploi_du_temps_edt_occupation_loup_nargue
int fct_Gam_emploi_du_temps_edt_occupation_loup_nargue_Gam_emploi_du_temps_edt_occupation_loup_poursuite;
int fct_Gam_emploi_du_temps_edt_occupation_loup_nargue_Gam_emploi_du_temps_edt_occupation_loup_safe;
int fct_Gam_emploi_du_temps_edt_occupation_loup_nargue_Gam_emploi_du_temps_edt_exec_recherche_chemin;

//Gam_emploi_du_temps_edt_dialogue_check
int Gam_emploi_du_temps_edt_dialogue_check__PJ_Sally_gao;
int Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao;
int Gam_emploi_du_temps_edt_dialogue_check__PJ_Sally_gao_current_pm_mode;
int Gam_emploi_du_temps_edt_dialogue_check__PJ_Sally_gao_current_pm_priority;
int Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_i_sally_priority;
int Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_o_sally_contextuel;
int Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_i_sally_action;
int Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_v_sally_offset;
int Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_i_force_interface;
int Gam_emploi_du_temps_edt_dialogue_check_univVars;
int Gam_emploi_du_temps_edt_dialogue_check_univVars_Flag_speciaux_sauve;
int Gam_emploi_du_temps_edt_dialogue_check_TXT_Sally_gao;
int Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_ai_dialog_consequences;
int Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_o_dialogue_actor;
int Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_at_dialog_questions;
int Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_at_dialog_reponses;
int Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_ai_dialog_grisage;

//Gam_emploi_du_temps_edt_occupation_loup_safe
int fct_Gam_emploi_du_temps_edt_occupation_loup_safe_Gam_emploi_du_temps_edt_occupation_loup_poursuite;
int fct_Gam_emploi_du_temps_edt_occupation_loup_safe_Gam_emploi_du_temps_edt_occupation_loup_nargue;

int Gam_emploi_du_temps_edt_init()
{
    int result; // eax
    BOOL v1; // eax
    BOOL v2; // eax
    double v3; // st7
    char* v4; // eax
    float X; // [esp+0h] [ebp-154h]
    tScriptStruct_ActorGamEmploi* v6; // [esp+8h] [ebp-14Ch]
    tGameObjectMain* v7; // [esp+10h] [ebp-144h]
    tGameObjectMain* v8; // [esp+18h] [ebp-13Ch]
    tGameObjectMain* v9; // [esp+1Ch] [ebp-138h]
    tGameObjectMain* v10; // [esp+20h] [ebp-134h]
    double v11; // [esp+24h] [ebp-130h]
    tGameObjectMain* v12; // [esp+30h] [ebp-124h]
    tGameObjectMain* v13; // [esp+48h] [ebp-10Ch]
    tGameObjectMain* v14; // [esp+50h] [ebp-104h]
    tGameObjectMain* v15; // [esp+58h] [ebp-FCh]
    tGameObjectMain* v16; // [esp+6Ch] [ebp-E8h]
    tGameObjectMain* v17; // [esp+70h] [ebp-E4h]
    tGameObjectMain* v18; // [esp+74h] [ebp-E0h]
    tGameObjectMain* v19; // [esp+78h] [ebp-DCh]
    float v20; // [esp+84h] [ebp-D0h]
    Vec3 a3; // [esp+94h] [ebp-C0h] BYREF
    Vec3 v22; // [esp+A0h] [ebp-B4h] BYREF
    int v23; // [esp+ACh] [ebp-A8h]
    tGameObjectMain* v24; // [esp+B0h] [ebp-A4h]
    Vec3 v25; // [esp+B4h] [ebp-A0h] BYREF
    int v26; // [esp+C0h] [ebp-94h]
    tGameObjectMain* v28; // [esp+C8h] [ebp-8Ch]
    Vec3 a1; // [esp+CCh] [ebp-88h] BYREF
    Vec3 v30; // [esp+D8h] [ebp-7Ch] BYREF
    BOOL v31; // [esp+E4h] [ebp-70h]
    int i; // [esp+F0h] [ebp-64h]
    int v33; // [esp+F4h] [ebp-60h]
    BOOL v34; // [esp+F8h] [ebp-5Ch]
    BOOL v35; // [esp+FCh] [ebp-58h]
    Vec3 v36; // [esp+100h] [ebp-54h] BYREF
    Vec3 v37; // [esp+10Ch] [ebp-48h] BYREF
    tGameObjectMain* v38; // [esp+120h] [ebp-34h]
    tGameObjectMain* v39; // [esp+124h] [ebp-30h]
    Vec3 v40; // [esp+128h] [ebp-2Ch] BYREF
    tScriptStruct_ActorGamEmploi* v41; // [esp+134h] [ebp-20h]
    Vec3 v42; // [esp+138h] [ebp-1Ch] BYREF
    tGameObjectMain* v43; // [esp+144h] [ebp-10h]
    BOOL v44; // [esp+14Ch] [ebp-8h]
    int v45; // [esp+150h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v41 = v6;
    i = 0;
    v33 = 0;
    
    if (v6->o_manager != 0) {
        tGameObjectMain* man = (tGameObjectMain*)v6->o_manager;
        DBG_OutputDebugString("o_manager: %p %x\n", v6->o_manager, man->flags);
    }

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Gam_emploi_du_temps_edt_init_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &Gam_emploi_du_temps_edt_init_INV_manager_gao_i_Animals_Nb,
            (void*)Gam_emploi_du_temps_edt_init_INV_manager_gao,
            23888);
        AI2C_ResolveBuffer(
            &Gam_emploi_du_temps_edt_init_INV_manager_gao_ao_Animals,
            (void*)Gam_emploi_du_temps_edt_init_INV_manager_gao,
            23368);
        AI2C_ResolveBuffer(
            &Gam_emploi_du_temps_edt_init_INV_manager_gao_ai_AnimalsData,
            (void*)Gam_emploi_du_temps_edt_init_INV_manager_gao,
            23628);
        Gam_emploi_du_temps_edt_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Gam_emploi_du_temps_edt_init_univVars_Flag_speciaux_sauve,
            (void*)Gam_emploi_du_temps_edt_init_univVars,
            4488);
        fct_Gam_emploi_du_temps_edt_init_Gam_emploi_du_temps_edt_tag_off = (int)AI_ul_RealResolveOneRefFunction(0x7100F773u);
        fct_Gam_emploi_du_temps_edt_init_Gam_emploi_du_temps_edt_reflex = (int)AI_ul_RealResolveOneRefFunction(0x7100F76Eu);
        fct_Gam_emploi_du_temps_edt_init_Gam_emploi_du_temps_edt_tag_on = (int)AI_ul_RealResolveOneRefFunction(0x7100F774u);
        fct_Gam_emploi_du_temps_edt_init_Gam_emploi_du_temps_edt_after_blend = (int)AI_ul_RealResolveOneRefFunction(0x2600058Fu);
        AI2C_ResolveBuffer(
            &Gam_emploi_du_temps_edt_init_univVars_Planetes_CurHour,
            (void*)Gam_emploi_du_temps_edt_init_univVars,
            6116);
    }
    switch ((unsigned int)*AI_gppst_CurrentJumpNode)
    {
    case 0u:
        *AI_gppst_CurrentJumpNode = (tAINode*)1;
        TIME_Wait_C(0.000001f);
        return 0;
    case 1u:
        *AI_gppst_CurrentJumpNode = (tAINode*)2;
        TIME_Wait_C(0.000001f);
        return 0;
    case 2u:
        *AI_gppst_CurrentJumpNode = (tAINode*)3;
        TIME_Wait_C(0.000001f);
        return 0;
    case 3u:
        if (v41->des_i_animaux != -1)
        {
            AI_EvalFunc_OBJ_InfoPhotoParamSet_C(
                AI_gpst_CurrentGameObject,
                4,
                v41->des_i_animaux,
                0,
                0,
                0.15000001f,
                1.0,
                0.89999998f,
                0.5);
            v24 = AI_gpst_CurrentGameObject;
            v19 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v19 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v18 = v19;
            }
            else
            {
                v18 = 0;
            }
            v18->extended->fields->designFlags = 64;
            if (v41->des_i_etage != -1)
            {
                v33 = 0;
                if (*(int*)Gam_emploi_du_temps_edt_init_INV_manager_gao_i_Animals_Nb < 64)
                {
                    for (i = 0; i < *(int32_t*)Gam_emploi_du_temps_edt_init_INV_manager_gao_i_Animals_Nb; ++i)
                    {
                        v39 = AI_gpst_CurrentGameObject;
                        v17 = AI_gpst_CurrentGameObject;
                        if (AI_gpst_CurrentGameObject)
                        {
                            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                                v17 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                            v16 = v17;
                        }
                        else
                        {
                            v16 = 0;
                        }
                        if (*(tGameObjectMain**)(Gam_emploi_du_temps_edt_init_INV_manager_gao_ao_Animals + 4 * i + 4) == v16)
                            break;
                    }
                    if (i == *(uint32_t*)Gam_emploi_du_temps_edt_init_INV_manager_gao_i_Animals_Nb)
                    {
                        *(uint32_t*)(Gam_emploi_du_temps_edt_init_INV_manager_gao_ao_Animals
                            + 4 * *(uint32_t*)Gam_emploi_du_temps_edt_init_INV_manager_gao_i_Animals_Nb
                            + 4) = (uint32_t)AI_gpst_CurrentGameObject;
                        *(uint32_t*)(Gam_emploi_du_temps_edt_init_INV_manager_gao_ai_AnimalsData
                            + 4 * (*(uint32_t*)Gam_emploi_du_temps_edt_init_INV_manager_gao_i_Animals_Nb)++
                            + 4) = v41->des_i_animaux + (v41->des_i_etage << 8);
                    }
                }
            }
        }
        if (!v41->i_SF_snap_on_wp)
            goto LABEL_75;
        if (v41->i_SF_snap_on_wp <= 0)
            v41->i_flag = 0;
        else
            v41->i_flag = v41->i_SF_snap_on_wp < 10000 ? AI_EvalFunc_ARRBitTest_C(
                (int*)(Gam_emploi_du_temps_edt_init_univVars_Flag_speciaux_sauve
                    + 4),
                v41->i_SF_snap_on_wp) : 1
            - AI_EvalFunc_ARRBitTest_C(
                (int*)(Gam_emploi_du_temps_edt_init_univVars_Flag_speciaux_sauve
                    + 4),
                v41->i_SF_snap_on_wp - 10000);
        if (v41->i_SF_snap_on_wp1 <= 0)
        {
            v41->i_flag1 = 0;
        }
        else if (v41->i_SF_snap_on_wp1 < 10000)
        {
            v41->i_flag1 = AI_EvalFunc_ARRBitTest_C(
                (int*)(Gam_emploi_du_temps_edt_init_univVars_Flag_speciaux_sauve + 4),
                v41->i_SF_snap_on_wp1);
        }
        else
        {
            v31 = AI_EvalFunc_ARRBitTest_C(
                (int*)(Gam_emploi_du_temps_edt_init_univVars_Flag_speciaux_sauve + 4),
                v41->i_SF_snap_on_wp1 - 10000);
            v41->i_flag1 = 1 - v31;
        }
        if (v41->i_flag && (v41->i_flag1 || !v41->i_SF_snap_on_wp1))
            goto LABEL_75;
        v15 = (tGameObjectMain*)v41->o_snap_on_wp;
        if (v15)
        {
            if ((unsigned int)v15 <= 2)
                v15 = AI_MainActor[(uint32_t)v15 - 1];
            AI_EvalFunc_OBJGetPos_C(v15, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        AI_EvalFunc_OBJSetPos_C_CURRENT(&a3);
        v14 = (tGameObjectMain*)v41->o_snap_on_wp;
        if (v14)
        {
            if ((unsigned int)v14 <= 2)
                v14 = AI_MainActor[(uint32_t)v14 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v14, &v37);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &v37);
        }
        v13 = (tGameObjectMain*)v41->o_snap_on_wp;
        if (v13)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            AI_EvalFunc_OBJ_BankingGet_C(v13, &v42);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGet_C(0, &v42);
        }
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v37, &v42);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v22);
        AI_EvalFunc_COLStartMatrixSet_C(AI_gpst_CurrentGameObject, &v22);
        AI_EvalFunc_ACTActionSet_C_CURRENT(v41->i_snap_action);
        goto LABEL_61;
    case 24u:
    LABEL_61:
        if (v41->i_flag && (v41->i_flag1 || !v41->i_SF_snap_on_wp1))
        {
        LABEL_75:
            v23 = AI_EvalFunc_ACTActionGet_C_CURRENT();
            if (v23 == 5)
            {
            LABEL_76:
                if (!AI_EvalFunc_ACTActionFinished_C_CURRENT())
                {
                    *AI_gppst_CurrentJumpNode = (tAINode*)26;
                    return 0;
                }
            }
            if (*(float*)AI_EvalVarArray_C2((char*)&v41->__f_occupation_hour_Dimension1, 0) == 0.0)
            {
                AI_EvalFunc_AITrackCurStop_C();
                v38 = AI_gpst_CurrentGameObject;
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                return 1;
            }
            X = v41->des_f_angle_talk * 0.017453f;
            v3 = cosf(X);
            v41->des_f_angle_talk = (float)v3;
            AI_EvalFunc_AITrackChange_C(0, (tAIFunction*)fct_Gam_emploi_du_temps_edt_init_Gam_emploi_du_temps_edt_tag_off);
            AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_Gam_emploi_du_temps_edt_init_Gam_emploi_du_temps_edt_reflex);
            AI_EvalFunc_AITrackChange_C(4, (tAIFunction*)fct_Gam_emploi_du_temps_edt_init_Gam_emploi_du_temps_edt_tag_on);
            v28 = AI_gpst_CurrentGameObject;
            v12 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v12 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v12,
                    3,
                    (tAIFunction*)fct_Gam_emploi_du_temps_edt_init_Gam_emploi_du_temps_edt_after_blend);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    3,
                    (tAIFunction*)fct_Gam_emploi_du_temps_edt_init_Gam_emploi_du_temps_edt_after_blend);
            }
            v41->tag_change_type = 1;
            v34 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
            v45 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 4100, 0);
            v36.x = 0.0;
            v36.y = 0.0;
            v36.z = -20.0;
            AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v36);
            v25.x = 0.0;
            v25.y = 0.0;
            v25.z = 0.0;
            v26 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v25);
            AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(0, 0xFF00);
            AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x8040, 0);
            v41->f_time_changetarget = TIM_gf_MainClock - TIM_gf_LockTime;
            v41->f_time_changetarget_delay = AI_EvalFunc_MATHRandFloat_C(1.5, 3.0);
            v11 = v41->f_speed_coef + MATH_gd_magicDoubleForRound;
            v41->i_fuite_frequency = (*(uint32_t *)&v11);
            AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, v41->f_scale_coef);
            v41->i_zoom = 1;
            v41->f_way_blendrotate = 0.125;
            v20 = *(float*)Gam_emploi_du_temps_edt_init_univVars_Planetes_CurHour;
            v41->i_occupation_indice = 0;
            while (*(float*)AI_EvalVarArray_C2((char*)&v41->__f_occupation_hour_Dimension1, v41->i_occupation_indice) < (double)v20)
            {
                if (++v41->i_occupation_indice == 30)
                {
                    AI_EvalFunc_AITrackCurStop_C();
                    AI_EvalFunc_AITrackStop_C(0);
                    AI_EvalFunc_AITrackStop_C(1);
                    AI_EvalFunc_AITrackStop_C(4);
                    v43 = AI_gpst_CurrentGameObject;
                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                    return 1;
                }
            }
            v4 = AI_EvalVarArray_C2((char*)&v41->__f_occupation_hour_Dimension1, v41->i_occupation_indice);
            v41->f_edt_hour = *(float*)v4;
            v41->i_occupation_courante = v41->i_occupation_type[v41->i_occupation_indice];
            v41->i_occupation_courante_old = v41->i_occupation_courante;
            v41->o_nearest = v41->o_occupation_pos[v41->i_occupation_indice];
            v41->o_nearest_old = v41->o_nearest;
            v41->i_occupation_cherche_nouvelle = 1;
            v41->f_old_curent_hour = v20;
            v10 = (tGameObjectMain*)v41->o_nearest;
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
                if (!v41->i_SF_snap_on_wp && !v41->i_test_map_reset)
                {
                    v8 = (tGameObjectMain*)v41->o_nearest;
                    if (v8)
                    {
                        if ((unsigned int)v8 <= 2)
                            v8 = AI_MainActor[(uint32_t)v8 - 1];
                        AI_EvalFunc_OBJGetPos_C(v8, &a1);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &a1);
                    }
                    AI_EvalFunc_OBJSetPos_C_CURRENT(&a1);
                    v7 = (tGameObjectMain*)v41->o_nearest;
                    if (v7)
                    {
                        if ((unsigned int)v7 <= 2)
                            v7 = AI_MainActor[(uint32_t)v7 - 1];
                        AI_EvalFunc_OBJ_SightGet_C(v7, &v40);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_SightGet_C(0, &v40);
                    }
                    AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v40);
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v30);
                    AI_EvalFunc_COLStartMatrixSet_C(AI_gpst_CurrentGameObject, &v30);
                }
            }
            else
            {
                v41->o_nearest = (int)AI_gpst_CurrentGameObject;
            }
            result = 1000;
        }
        else
        {
            if (v41->i_SF_snap_on_wp <= 0)
            {
                v41->i_flag = 0;
            }
            else if (v41->i_SF_snap_on_wp < 10000)
            {
                v1 = AI_EvalFunc_ARRBitTest_C(
                    (int*)(Gam_emploi_du_temps_edt_init_univVars_Flag_speciaux_sauve + 4),
                    v41->i_SF_snap_on_wp);
                v41->i_flag = v1;
            }
            else
            {
                v35 = AI_EvalFunc_ARRBitTest_C(
                    (int*)(Gam_emploi_du_temps_edt_init_univVars_Flag_speciaux_sauve + 4),
                    v41->i_SF_snap_on_wp - 10000);
                v41->i_flag = 1 - v35;
            }
            if (v41->i_SF_snap_on_wp1 <= 0)
            {
                v41->i_flag1 = 0;
            }
            else if (v41->i_SF_snap_on_wp1 < 10000)
            {
                v2 = AI_EvalFunc_ARRBitTest_C(
                    (int*)(Gam_emploi_du_temps_edt_init_univVars_Flag_speciaux_sauve + 4),
                    v41->i_SF_snap_on_wp1);
                v41->i_flag1 = v2;
            }
            else
            {
                v44 = AI_EvalFunc_ARRBitTest_C(
                    (int*)(Gam_emploi_du_temps_edt_init_univVars_Flag_speciaux_sauve + 4),
                    v41->i_SF_snap_on_wp1 - 10000);
                v41->i_flag1 = 1 - v44;
            }
            *AI_gppst_CurrentJumpNode = (tAINode*)24;
            result = 0;
        }
        return result;
    case 26u:
        goto LABEL_76;
    default:
        *AI_gppst_CurrentJumpNode = 0;
        TIME_Wait_C(0.000001f);
        return 0;
    }
}

int Gam_emploi_du_temps_edt_reflex()
{
    char* v1; // eax
    char* v2; // eax
    float a4; // [esp+0h] [ebp-1ECh]
    float v4; // [esp+Ch] [ebp-1E0h]
    tScriptStruct_ActorGamEmploi* v5; // [esp+18h] [ebp-1D4h]
    double v6; // [esp+1Ch] [ebp-1D0h]
    tGameObjectMain* v7; // [esp+24h] [ebp-1C8h]
    tGameObjectMain* v8; // [esp+28h] [ebp-1C4h]
    tGameObjectMain* v9; // [esp+2Ch] [ebp-1C0h]
    tGameObjectMain* v10; // [esp+30h] [ebp-1BCh]
    tAI* v11; // [esp+34h] [ebp-1B8h]
    tAI* v12; // [esp+38h] [ebp-1B4h]
    tGameObjectMain* v13; // [esp+3Ch] [ebp-1B0h]
    tGameObjectMain* v14; // [esp+40h] [ebp-1ACh]
    tAI* v15; // [esp+44h] [ebp-1A8h]
    tAI* v16; // [esp+48h] [ebp-1A4h]
    tGameObjectMain* v17; // [esp+4Ch] [ebp-1A0h]
    tGameObjectMain* v18; // [esp+50h] [ebp-19Ch]
    tAI* v19; // [esp+54h] [ebp-198h]
    tAI* v20; // [esp+58h] [ebp-194h]
    tGameObjectMain* v21; // [esp+5Ch] [ebp-190h]
    tGameObjectMain* v22; // [esp+60h] [ebp-18Ch]
    tAI* v23; // [esp+64h] [ebp-188h]
    tAI* v24; // [esp+68h] [ebp-184h]
    tGameObjectMain* v25; // [esp+6Ch] [ebp-180h]
    tGameObjectMain* v26; // [esp+70h] [ebp-17Ch]
    tAI* v27; // [esp+74h] [ebp-178h]
    tAI* v28; // [esp+78h] [ebp-174h]
    tGameObjectMain* v29; // [esp+7Ch] [ebp-170h]
    tGameObjectMain* v30; // [esp+80h] [ebp-16Ch]
    tAI* v31; // [esp+88h] [ebp-164h]
    tAI* v32; // [esp+8Ch] [ebp-160h]
    tGameObjectMain* v33; // [esp+90h] [ebp-15Ch]
    tGameObjectMain* v34; // [esp+94h] [ebp-158h]
    tAI* v35; // [esp+98h] [ebp-154h]
    tAI* v36; // [esp+9Ch] [ebp-150h]
    tGameObjectMain* v37; // [esp+A0h] [ebp-14Ch]
    tGameObjectMain* v38; // [esp+A4h] [ebp-148h]
    tGameObjectMain* v39; // [esp+A8h] [ebp-144h]
    tGameObjectMain* v40; // [esp+ACh] [ebp-140h]
    tAI* v41; // [esp+B0h] [ebp-13Ch]
    tAI* v42; // [esp+B4h] [ebp-138h]
    tGameObjectMain* v43; // [esp+B8h] [ebp-134h]
    tGameObjectMain* v44; // [esp+BCh] [ebp-130h]
    tGameObjectMain* v45; // [esp+C0h] [ebp-12Ch]
    tGameObjectMain* v46; // [esp+C4h] [ebp-128h]
    tAI* v47; // [esp+C8h] [ebp-124h]
    tAI* v48; // [esp+CCh] [ebp-120h]
    tGameObjectMain* v49; // [esp+D0h] [ebp-11Ch]
    tGameObjectMain* v50; // [esp+D4h] [ebp-118h]
    Vec3 a5; // [esp+E0h] [ebp-10Ch] BYREF
    tMessage v52; // [esp+ECh] [ebp-100h] BYREF
    Vec3 a1; // [esp+124h] [ebp-C8h] BYREF
    int v54; // [esp+130h] [ebp-BCh]
    Vec3 v55; // [esp+134h] [ebp-B8h] BYREF
    Vec3 a3; // [esp+148h] [ebp-A4h] BYREF
    Vec3 dest; // [esp+154h] [ebp-98h] BYREF
    void* v59; // [esp+160h] [ebp-8Ch]
    int v60; // [esp+164h] [ebp-88h]
    int v61; // [esp+168h] [ebp-84h]
    int v62; // [esp+16Ch] [ebp-80h]
    Vec3 a2; // [esp+170h] [ebp-7Ch] BYREF
    Vec3 v64; // [esp+17Ch] [ebp-70h] BYREF
    void* Src; // [esp+188h] [ebp-64h]
    Vec3 v66; // [esp+18Ch] [ebp-60h] BYREF
    tGameObjectMain* v67; // [esp+19Ch] [ebp-50h]
    float v68; // [esp+1A0h] [ebp-4Ch]
    int v69; // [esp+1A4h] [ebp-48h]
    float v71; // [esp+1ACh] [ebp-40h]
    tScriptStruct_ActorGamEmploi* v73; // [esp+1B4h] [ebp-38h]
    int i; // [esp+1B8h] [ebp-34h]
    Vec3 v75; // [esp+1BCh] [ebp-30h] BYREF
    int v77; // [esp+1CCh] [ebp-20h]
    Vec3 v78; // [esp+1D0h] [ebp-1Ch] BYREF
    Vec3 v79; // [esp+1DCh] [ebp-10h]
    int v80; // [esp+1E8h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v73 = v5;
    v69 = 0;
    v80 = 0;
    v68 = 0.0;
    i = 0;

    // TODO FIXME: figure out o_manager & friends !!!!!!

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Gam_emploi_du_temps_edt_reflex_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Gam_emploi_du_temps_edt_reflex_univVars_Planetes_CurHour,
            (void*)Gam_emploi_du_temps_edt_reflex_univVars,
            6116);
        fct_Gam_emploi_du_temps_edt_reflex_Gam_emploi_du_temps_edt_exec_next_edt = (int)AI_ul_RealResolveOneRefFunction(0x7100F76Du);
        fct_Gam_emploi_du_temps_edt_reflex_Gam_emploi_du_temps_edt_exec_cherche_dest = (int)AI_ul_RealResolveOneRefFunction(0x7100F795u);
        fct_Gam_emploi_du_temps_edt_reflex_Gam_emploi_du_temps_edt_dialogue_check = (int)AI_ul_RealResolveOneRefFunction(0x260005F0u);
        Gam_emploi_du_temps_edt_reflex__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        fct_Gam_emploi_du_temps_edt_reflex_Gam_emploi_du_temps_edt_tag_on = (int)AI_ul_RealResolveOneRefFunction(0x7100F774u);
    }
    v73->speed = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
    if (AI_EvalFunc_OBJ_CustomBitsTest_C(AI_gpst_CurrentGameObject, 0x100))
    {
        v48 = (tAI*)v73->o_manager;
        if (v48 == WOR_gst_Universe.univAIInstance)
        {
            v47 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v50 = (tGameObjectMain*)v73->o_manager;
            if (v48)
            {
                if ((unsigned int)v48 <= 2)
                    v50 = AI_MainActor[(uint32_t)v48 - 1];
                v49 = v50;
            }
            else
            {
                v49 = 0;
            }
            v47 = v49->extended->AIInstance;
        }
        v73->o_le_loup = *((uint32_t*)v47->instanceVarDataCurrent + 2);
        v46 = (tGameObjectMain*)v73->o_le_loup;
        if (v46)
        {
            if ((unsigned int)v46 <= 2)
                v46 = AI_MainActor[(uint32_t)v46 - 1];
            v45 = v46;
        }
        else
        {
            v45 = 0;
        }
        if (!v45)
        {
            v42 = (tAI*)v73->o_manager;
            if (v42 == WOR_gst_Universe.univAIInstance)
            {
                v41 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v44 = (tGameObjectMain*)v73->o_manager;
                if (v42)
                {
                    if ((unsigned int)v42 <= 2)
                        v44 = AI_MainActor[(uint32_t)v42 - 1];
                    v43 = v44;
                }
                else
                {
                    v43 = 0;
                }
                v41 = v43->extended->AIInstance;
            }
            *((uint32_t*)v41->instanceVarDataCurrent + 2) = (uint32_t)AI_gpst_CurrentGameObject;
            v73->o_le_loup = (int)AI_gpst_CurrentGameObject;
            v40 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v40 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v39 = v40;
            }
            else
            {
                v39 = 0;
            }
            v36 = (tAI*)v73->o_manager;
            if (v36 == WOR_gst_Universe.univAIInstance)
            {
                v35 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v38 = (tGameObjectMain*)v73->o_manager;
                if (v36)
                {
                    if ((unsigned int)v36 <= 2)
                        v38 = AI_MainActor[(uint32_t)v36 - 1];
                    v37 = v38;
                }
                else
                {
                    v37 = 0;
                }
                v35 = v37->extended->AIInstance;
            }
            v32 = (tAI*)v73->o_manager;
            if (v32 == WOR_gst_Universe.univAIInstance)
            {
                v31 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v34 = (tGameObjectMain*)v73->o_manager;
                if (v32)
                {
                    if ((unsigned int)v32 <= 2)
                        v34 = AI_MainActor[(uint32_t)v32 - 1];
                    v33 = v34;
                }
                else
                {
                    v33 = 0;
                }
                v31 = v33->extended->AIInstance;
            }
            v62 = AI_EvalFunc_ARRObjSearch_C(
                (tGameObjectMain**)v31->instanceVarDataCurrent + 4,
                *((uint32_t*)v35->instanceVarDataCurrent + 24),
                v39);
            i = v62;
            v28 = (tAI*)v73->o_manager;
            if (v28 == WOR_gst_Universe.univAIInstance)
            {
                v27 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v30 = (tGameObjectMain*)v73->o_manager;
                if (v28)
                {
                    if ((unsigned int)v28 <= 2)
                        v30 = AI_MainActor[(uint32_t)v28 - 1];
                    v29 = v30;
                }
                else
                {
                    v29 = 0;
                }
                v27 = v29->extended->AIInstance;
            }
            --* ((uint32_t*)v27->instanceVarDataCurrent + 24);
            v24 = (tAI*)v73->o_manager;
            if (v24 == WOR_gst_Universe.univAIInstance)
            {
                v23 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v26 = (tGameObjectMain*)v73->o_manager;
                if (v24)
                {
                    if ((unsigned int)v24 <= 2)
                        v26 = AI_MainActor[(uint32_t)v24 - 1];
                    v25 = v26;
                }
                else
                {
                    v25 = 0;
                }
                v23 = v25->extended->AIInstance;
            }
            if (i != *((uint32_t*)v23->instanceVarDataCurrent + 24))
            {
                v20 = (tAI*)v73->o_manager;
                if (v20 == WOR_gst_Universe.univAIInstance)
                {
                    v19 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v22 = (tGameObjectMain*)v73->o_manager;
                    if (v20)
                    {
                        if ((unsigned int)v20 <= 2)
                            v22 = AI_MainActor[(uint32_t)v20 - 1];
                        v21 = v22;
                    }
                    else
                    {
                        v21 = 0;
                    }
                    v19 = v21->extended->AIInstance;
                }
                v69 = *((uint32_t*)v19->instanceVarDataCurrent + 24);
                v16 = (tAI*)v73->o_manager;
                if (v16 == WOR_gst_Universe.univAIInstance)
                {
                    v15 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v18 = (tGameObjectMain*)v73->o_manager;
                    if (v16)
                    {
                        if ((unsigned int)v16 <= 2)
                            v18 = AI_MainActor[(uint32_t)v16 - 1];
                        v17 = v18;
                    }
                    else
                    {
                        v17 = 0;
                    }
                    v15 = v17->extended->AIInstance;
                }
                v12 = (tAI*)v73->o_manager;
                if (v12 == WOR_gst_Universe.univAIInstance)
                {
                    v11 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v14 = (tGameObjectMain*)v73->o_manager;
                    if (v12)
                    {
                        if ((unsigned int)v12 <= 2)
                            v14 = AI_MainActor[(uint32_t)v12 - 1];
                        v13 = v14;
                    }
                    else
                    {
                        v13 = 0;
                    }
                    v11 = v13->extended->AIInstance;
                }
                *((uint32_t*)v11->instanceVarDataCurrent + i + 4) = *((uint32_t*)v15->instanceVarDataCurrent + v69 + 4);
            }
        }
    }
    if (!v73->i_dialogue_on)
    {
        v68 = *(float*)Gam_emploi_du_temps_edt_reflex_univVars_Planetes_CurHour;
        if (v73->f_edt_hour <= (double)v68 || v73->f_old_curent_hour > (double)v68 || v73->i_force_next_edt)
        {
            v73->i_force_next_edt = 0;
            v77 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_reflex_Gam_emploi_du_temps_edt_exec_next_edt);
            if (v77 != 1000)
                return v77;
        }
        v73->f_old_curent_hour = v68;
        if (v73->i_occupation_cherche_nouvelle)
        {
            v54 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_reflex_Gam_emploi_du_temps_edt_exec_cherche_dest);
            if (v54 != 1000)
                return v54;
            v73->i_way_force_recompute = 1;
        }
        v67 = AI_gpst_CurrentGameObject;
        v10 = (tGameObjectMain*)v73->o_le_loup;
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
        v8 = v67;
        if (v67)
        {
            if ((unsigned int)v67 <= 2)
                v8 = AI_MainActor[(uint32_t)v67 - 1];
            v7 = v8;
        }
        else
        {
            v7 = 0;
        }
        if (v9 == v7)
        {
            if (AI_EvalFunc_OBJ_CustomBitsTest_C(AI_gpst_CurrentGameObject, 256))
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v55);
                v64.x = 0.0;
                v64.y = 0.0;
                v64.z = 0.30000001f;
                MATH_VecAddVec(&a1, &v55, &v64);
                v79.x = 0.0;
                v79.y = 0.0;
                v79.z = 1.0;
            }
        }
    }
    Src = AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v52, Src, sizeof(v52));
    while (v52.msgID)
    {
        if (v52.msgID == 10)
        {
            if (v52.f2 < 0.0)
            {
                v73->i_dialogue_on = 1;
                memcpy(&v73->m_dialogue, &v52, sizeof(v73->m_dialogue));
                if (AI_EvalFunc_ACTCustomBitTest_C(AI_gpst_CurrentGameObject, 0x20u))
                    AI_EvalFunc_ACTActionSet_C_CURRENT(0);
            }
        }
        else if (v52.msgID == 20)
        {
            v4 = (float)(unsigned __int8)(unsigned __int64)v52.f1;
            v6 = v4 + MATH_gd_magicDoubleForRound;
            v80 = (*(uint32_t *)&v6);
            if ((*(uint32_t*)&v6) == 254 || v80 == 253)
            {
                v73->i_dialogue_on = 0;
                v68 = *(float*)Gam_emploi_du_temps_edt_reflex_univVars_Planetes_CurHour;
                for (i = 0; ; ++i)
                {
                    v1 = AI_EvalVarArray_C2((char*)&v73->__f_occupation_hour_Dimension1, i);
                    if (*(float*)v1 >= (double)v68)
                        break;
                }
                if (i != v73->i_occupation_indice)
                {
                    v73->i_occupation_indice = i;
                    v2 = AI_EvalVarArray_C2((char*)&v73->__f_occupation_hour_Dimension1, v73->i_occupation_indice);
                    v73->f_edt_hour = *(float*)v2;
                    v73->i_occupation_courante = v73->i_occupation_type[v73->i_occupation_indice];
                    v73->i_occupation_courante_old = v73->i_occupation_courante;
                    v73->o_nearest = v73->o_occupation_pos[v73->i_occupation_indice];
                    v73->o_nearest_old = v73->o_nearest;
                    v73->i_occupation_cherche_nouvelle = 1;
                    v73->f_old_curent_hour = v68;
                }
            }
            else
            {
                v73->i_dialogue_on = 1;
            }
            memcpy(&v73->m_dialogue, &v52, sizeof(v73->m_dialogue));
        }
        v59 = AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v52, v59, sizeof(v52));
    }
    if (AI_EvalFunc_ACTCustomBitTest_C(AI_gpst_CurrentGameObject, 0x80u))
    {
        if (v73->I_Speak)
        {
            v60 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_reflex_Gam_emploi_du_temps_edt_dialogue_check);
            if (v60 != 1000)
                return v60;
        }
    }
    if (!v73->i_dialogue_on)
        return 1000;
    if (!AI_EvalFunc_ACTActionGet_C_CURRENT())
    {
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Gam_emploi_du_temps_edt_reflex__PJ_Sally_gao, &v78);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v66);
        MATH_SubstractVec(&dest, &v78, &v66);
        AI_EvalFunc_MATHVecNormalize_C(&dest, &a3);
        v71 = TIM_gf_dt;
        a4 = 10.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
        v75.x = 0.0;
        v75.y = 0.0;
        v75.z = 1.0;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a5, &v75);
    }
    v61 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_reflex_Gam_emploi_du_temps_edt_tag_on);
    if (v61 != 1000)
        return v61;
    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
    return 1;
}

int Gam_emploi_du_temps_edt_tag_off()
{
    char* v0; // eax
    float a4; // [esp+0h] [ebp-30h]
    int v3; // [esp+4h] [ebp-2Ch]
    tScriptStruct_ActorGamEmploi* v4; // [esp+8h] [ebp-28h]
    BOOL v5; // [esp+2Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    if (v4->tag_etage_force == -1 && AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 64))
    {
        if ((AI_EvalFunc_COLGMFlagsGet_C(AI_gpst_CurrentGameObject, 0x40) & 0x800000) != 0)
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x4000, 0);
        else
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x4000);
    }
    if (v4->tag_flag)
    {
        v4->tag_flag = 0;
        AI_EvalFunc_GRIDModeSet_C(v4->tag_mode);
        AI_EvalFunc_GRIDCurrentSet_C(v4->tag_etage);
        if (v4->tag_type != -1)
        {
            if ((v4->tag_type & 0xFF000000) != 0)
            {
                AI_EvalFunc_GRIDTagVec_C(
                    AI_gpst_CurrentGameObject,
                    &v4->tag_vectorpos,
                    v4->tag_type & 0xFFFFFF,
                    -v4->tag_value,
                    &v4->tag_vectorsight);
            }
            else if ((v4->tag_type & 0xFF0000) != 0)
            {
                v3 = -v4->tag_value;
                a4 = v4->tag_size;
                v0 = AI_EvalVarArray_C3((char*)&v4->__tag_2x2dyn_pos_Dimension1, 0);
                AI_EvalFunc_GRIDTagDyn_C(AI_gpst_CurrentGameObject, &v4->tag_vectorpos, (Vec3*)v0, a4, v3);
            }
            else if ((v4->tag_type & 0xFF00) != 0)
            {
                AI_EvalFunc_GRIDTagBox_C(AI_gpst_CurrentGameObject, &v4->tag_free_pos1, &v4->tag_free_pos2, -v4->tag_value);
            }
            else
            {
                AI_EvalFunc_GRIDTag_C(AI_gpst_CurrentGameObject, &v4->tag_vectorpos, v4->tag_type, -v4->tag_value);
            }
        }
        AI_EvalFunc_GRIDModeSet_C(0);
    }
    v5 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x4000);
    AI_EvalFunc_GRIDCurrentSet_C(v5);
    return 1000;
}

int Gam_emploi_du_temps_edt_tag_on()
{
    char* v0; // eax
    float a4; // [esp+0h] [ebp-60h]
    int v3; // [esp+4h] [ebp-5Ch]
    tScriptStruct_ActorGamEmploi* v4; // [esp+8h] [ebp-58h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    if (!v4->tag_flag)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v4->tag_vectorpos);
        if (AI_EvalFunc_GRIDIsIn_C(AI_gpst_CurrentGameObject, &v4->tag_vectorpos))
        {
            v4->tag_size = v4->tag_change_size;
            v4->tag_mode = v4->tag_change_mode;
            v4->tag_type = v4->tag_change_type;
            v4->tag_value = v4->tag_change_value;
            AI_EvalFunc_GRIDModeSet_C(v4->tag_mode);
            if (v4->tag_etage_force == -1)
                v4->tag_etage = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x4000);
            else
                v4->tag_etage = v4->tag_etage_force;
            AI_EvalFunc_GRIDCurrentSet_C(v4->tag_etage);
            if (v4->tag_type != -1)
            {
                if ((v4->tag_type & 0xFF000000) != 0)
                {
                    if (v4->tag_type >= 0)
                        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v4->tag_vectorsight);
                    else
                        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v4->tag_vectorsight);
                    v4->tag_flag = AI_EvalFunc_GRIDTagVec_C(
                        AI_gpst_CurrentGameObject,
                        &v4->tag_vectorpos,
                        v4->tag_type & 0xFFFFFF,
                        v4->tag_value,
                        &v4->tag_vectorsight);
                }
                else if ((v4->tag_type & 0xFF0000) != 0)
                {
                    v3 = v4->tag_value;
                    a4 = v4->tag_size;
                    v0 = AI_EvalVarArray_C3((char*)&v4->__tag_2x2dyn_pos_Dimension1, 0);
                    v4->tag_flag = AI_EvalFunc_GRIDTagDyn_C(AI_gpst_CurrentGameObject, &v4->tag_vectorpos, (Vec3*)v0, a4, v3);
                }
                else if ((v4->tag_type & 0xFF00) != 0)
                {
                    v4->tag_free_pos1 = v4->tag_change_free_pos1;
                    v4->tag_free_pos2 = v4->tag_change_free_pos2;
                    v4->tag_flag = AI_EvalFunc_GRIDTagBox_C(
                        AI_gpst_CurrentGameObject,
                        &v4->tag_free_pos1,
                        &v4->tag_free_pos2,
                        v4->tag_value);
                }
                else
                {
                    v4->tag_flag = AI_EvalFunc_GRIDTag_C(
                        AI_gpst_CurrentGameObject,
                        &v4->tag_vectorpos,
                        v4->tag_type,
                        v4->tag_value);
                }
            }
            AI_EvalFunc_GRIDModeSet_C(0);
        }
    }
    return 1000;
}

int Gam_emploi_du_temps_edt_exec_track_occupation()
{
    int v0; // eax
    tScriptStruct_ActorGamEmploi* v2; // [esp+0h] [ebp-14h]
    tGameObjectMain* v3; // [esp+8h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Gam_emploi_du_temps_edt_exec_track_occupation_Gam_emploi_du_temps_edt_occupation_dodo = (int)AI_ul_RealResolveOneRefFunction(0x7100F7C0u);
        fct_Gam_emploi_du_temps_edt_exec_track_occupation_Gam_emploi_du_temps_edt_occupation_jump = (int)AI_ul_RealResolveOneRefFunction(0x7100F7C1u);
        fct_Gam_emploi_du_temps_edt_exec_track_occupation_Gam_emploi_du_temps_edt_occupation_loup_init = (int)AI_ul_RealResolveOneRefFunction(0x7100F7C2u);
        fct_Gam_emploi_du_temps_edt_exec_track_occupation_Gam_emploi_du_temps_edt_occupation_regarde = (int)AI_ul_RealResolveOneRefFunction(0x26000353u);
        fct_Gam_emploi_du_temps_edt_exec_track_occupation_Gam_emploi_du_temps_edt_occupation_mange = (int)AI_ul_RealResolveOneRefFunction(0x2600048Bu);
    }
    v2->o_old_capa_wp = v2->o_occupation_dest;
    v3 = (tGameObjectMain*)v2->o_old_capa_wp;
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        v0 = AI_EvalFunc_OBJ_CapaGet_C(v3);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_CapaGet_C(0);
    }
    v2->i_old_capa = v0;
    if (v2->i_occupation_courante)
    {
        switch (v2->i_occupation_courante)
        {
        case 1:
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_exec_track_occupation_Gam_emploi_du_temps_edt_occupation_jump);
            break;
        case 2:
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_exec_track_occupation_Gam_emploi_du_temps_edt_occupation_loup_init);
            break;
        case 3:
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_exec_track_occupation_Gam_emploi_du_temps_edt_occupation_regarde);
            break;
        case 4:
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_exec_track_occupation_Gam_emploi_du_temps_edt_occupation_mange);
            break;
        }
    }
    else
    {
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_exec_track_occupation_Gam_emploi_du_temps_edt_occupation_dodo);
    }
    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
    return 4;
}

int Gam_emploi_du_temps_edt_occupation_loup_init()
{
    tScriptStruct_ActorGamEmploi* v1; // [esp+4h] [ebp-B4h]
    tAI* v2; // [esp+8h] [ebp-B0h]
    tAI* v3; // [esp+Ch] [ebp-ACh]
    tGameObjectMain* v4; // [esp+10h] [ebp-A8h]
    tGameObjectMain* v5; // [esp+14h] [ebp-A4h]
    tAI* v6; // [esp+18h] [ebp-A0h]
    tAI* v7; // [esp+1Ch] [ebp-9Ch]
    tGameObjectMain* v8; // [esp+20h] [ebp-98h]
    tGameObjectMain* v9; // [esp+24h] [ebp-94h]
    tAI* v10; // [esp+28h] [ebp-90h]
    tAI* v11; // [esp+2Ch] [ebp-8Ch]
    tGameObjectMain* v12; // [esp+30h] [ebp-88h]
    tGameObjectMain* v13; // [esp+34h] [ebp-84h]
    tAI* v14; // [esp+38h] [ebp-80h]
    tAI* v15; // [esp+3Ch] [ebp-7Ch]
    tGameObjectMain* v16; // [esp+40h] [ebp-78h]
    tGameObjectMain* v17; // [esp+44h] [ebp-74h]
    tGameObjectMain* v18; // [esp+48h] [ebp-70h]
    tGameObjectMain* v19; // [esp+4Ch] [ebp-6Ch]
    tAI* v20; // [esp+50h] [ebp-68h]
    tAI* v21; // [esp+54h] [ebp-64h]
    tGameObjectMain* v22; // [esp+58h] [ebp-60h]
    tGameObjectMain* v23; // [esp+5Ch] [ebp-5Ch]
    tAI* v24; // [esp+60h] [ebp-58h]
    tAI* v25; // [esp+64h] [ebp-54h]
    tGameObjectMain* v26; // [esp+68h] [ebp-50h]
    tGameObjectMain* v27; // [esp+6Ch] [ebp-4Ch]
    tAI* v28; // [esp+7Ch] [ebp-3Ch]
    tAI* v29; // [esp+80h] [ebp-38h]
    tGameObjectMain* v30; // [esp+84h] [ebp-34h]
    tGameObjectMain* v31; // [esp+88h] [ebp-30h]
    tGameObjectMain* v32; // [esp+98h] [ebp-20h]
    tGameObjectMain* v33; // [esp+9Ch] [ebp-1Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Gam_emploi_du_temps_edt_occupation_loup_init_Gam_emploi_du_temps_edt_occupation_loup_nargue = (int)AI_ul_RealResolveOneRefFunction(0x2600004Du);
        fct_Gam_emploi_du_temps_edt_occupation_loup_init_Gam_emploi_du_temps_edt_occupation_loup_attentejoueur = (int)AI_ul_RealResolveOneRefFunction(0x26000049u);
    }
    v1->i_old_capa = 0;
    AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(0x100, 0);
    v33 = (tGameObjectMain*)v1->o_occupation_dest;
    if (v33)
    {
        if ((unsigned int)v33 <= 2)
            v33 = AI_MainActor[(uint32_t)v33 - 1];
        v32 = v33;
    }
    else
    {
        v32 = 0;
    }
    v1->o_manager = v32->extended->fields->designGO1;
    v29 = (tAI*)v1->o_manager;
    if (v29 == WOR_gst_Universe.univAIInstance)
    {
        v28 = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        v31 = (tGameObjectMain*)v1->o_manager;
        if (v29)
        {
            if ((unsigned int)v29 <= 2)
                v31 = AI_MainActor[(uint32_t)v29 - 1];
            v30 = v31;
        }
        else
        {
            v30 = 0;
        }
        v28 = v30->extended->AIInstance;
    }
    v1->i_safe_nbr = AI_EvalFunc_WAYNetFill_C(*(tWayNetwork**)v28->instanceVarDataCurrent, 0, (tGameObjectMain**)v1->o_safe_list);
    v25 = (tAI*)v1->o_manager;
    if (v25 == WOR_gst_Universe.univAIInstance)
    {
        v24 = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        v27 = (tGameObjectMain*)v1->o_manager;
        if (v25)
        {
            if ((unsigned int)v25 <= 2)
                v27 = AI_MainActor[(uint32_t)v25 - 1];
            v26 = v27;
        }
        else
        {
            v26 = 0;
        }
        v24 = v26->extended->AIInstance;
    }
    v1->i_attente_nbr = AI_EvalFunc_WAYNetFill_C(*((tWayNetwork**)v24->instanceVarDataCurrent + 1), 0, (tGameObjectMain**)v1->o_attente_list);
    v21 = (tAI*)v1->o_manager;
    if (v21 == WOR_gst_Universe.univAIInstance)
    {
        v20 = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        v23 = (tGameObjectMain*)v1->o_manager;
        if (v21)
        {
            if ((unsigned int)v21 <= 2)
                v23 = AI_MainActor[(uint32_t)v21 - 1];
            v22 = v23;
        }
        else
        {
            v22 = 0;
        }
        v20 = v22->extended->AIInstance;
    }
    v1->o_le_loup = *((uint32_t*)v20->instanceVarDataCurrent + 2);
    v19 = (tGameObjectMain*)v1->o_le_loup;
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
        v15 = (tAI*)v1->o_manager;
        if (v15 == WOR_gst_Universe.univAIInstance)
        {
            v14 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v17 = (tGameObjectMain*)v1->o_manager;
            if (v15)
            {
                if ((unsigned int)v15 <= 2)
                    v17 = AI_MainActor[(uint32_t)v15 - 1];
                v16 = v17;
            }
            else
            {
                v16 = 0;
            }
            v14 = v16->extended->AIInstance;
        }
        v11 = (tAI*)v1->o_manager;
        if (v11 == WOR_gst_Universe.univAIInstance)
        {
            v10 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v13 = (tGameObjectMain*)v1->o_manager;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v13 = AI_MainActor[(uint32_t)v11 - 1];
                v12 = v13;
            }
            else
            {
                v12 = 0;
            }
            v10 = v12->extended->AIInstance;
        }
        *((uint32_t*)v10->instanceVarDataCurrent + *((uint32_t*)v14->instanceVarDataCurrent + 24) + 4) = (uint32_t)AI_gpst_CurrentGameObject;
        v7 = (tAI*)v1->o_manager;
        if (v7 == WOR_gst_Universe.univAIInstance)
        {
            v6 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v9 = (tGameObjectMain*)v1->o_manager;
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v9 = AI_MainActor[(uint32_t)v7 - 1];
                v8 = v9;
            }
            else
            {
                v8 = 0;
            }
            v6 = v8->extended->AIInstance;
        }
        ++* ((uint32_t*)v6->instanceVarDataCurrent + 24);
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_occupation_loup_init_Gam_emploi_du_temps_edt_occupation_loup_nargue);
    }
    else
    {
        v3 = (tAI*)v1->o_manager;
        if (v3 == WOR_gst_Universe.univAIInstance)
        {
            v2 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v5 = (tGameObjectMain*)v1->o_manager;
            if (v3)
            {
                if ((unsigned int)v3 <= 2)
                    v5 = AI_MainActor[(uint32_t)v3 - 1];
                v4 = v5;
            }
            else
            {
                v4 = 0;
            }
            v2 = v4->extended->AIInstance;
        }
        *((uint32_t*)v2->instanceVarDataCurrent + 2) = (uint32_t)AI_gpst_CurrentGameObject;
        v1->o_le_loup = (int)AI_gpst_CurrentGameObject;
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_occupation_loup_init_Gam_emploi_du_temps_edt_occupation_loup_attentejoueur);
    }
    return 1000;
}

int Gam_emploi_du_temps_edt_occupation_loup_attentejoueur()
{
    double v1; // st7
    float a4; // [esp+0h] [ebp-B0h]
    tScriptStruct_ActorGamEmploi* v3; // [esp+8h] [ebp-A8h]
    tGameObjectMain* v4; // [esp+10h] [ebp-A0h]
    tGameObjectMain* v5; // [esp+18h] [ebp-98h]
    tGameObjectMain* v6; // [esp+20h] [ebp-90h]
    tGameObjectMain* v7; // [esp+24h] [ebp-8Ch]
    tAI* v8; // [esp+28h] [ebp-88h]
    tAI* v9; // [esp+2Ch] [ebp-84h]
    tGameObjectMain* v10; // [esp+30h] [ebp-80h]
    tGameObjectMain* v11; // [esp+34h] [ebp-7Ch]
    int v12; // [esp+38h] [ebp-78h]
    Vec3 a1; // [esp+3Ch] [ebp-74h] BYREF
    unsigned int v14; // [esp+48h] [ebp-68h]
    Vec3 v15; // [esp+4Ch] [ebp-64h] BYREF
    Vec3 v16; // [esp+58h] [ebp-58h] BYREF
    float v17; // [esp+64h] [ebp-4Ch]
    int v18; // [esp+68h] [ebp-48h]
    float v19; // [esp+6Ch] [ebp-44h]
    Vec3 a5; // [esp+70h] [ebp-40h] BYREF
    Vec3 v21; // [esp+7Ch] [ebp-34h] BYREF
    int v22; // [esp+88h] [ebp-28h]
    int v23; // [esp+8Ch] [ebp-24h]
    tScriptStruct_ActorGamEmploi* v24; // [esp+94h] [ebp-1Ch]
    Vec3 a3; // [esp+98h] [ebp-18h] BYREF
    Vec3 a2; // [esp+A4h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v24 = v3;
    v14 = 0;
    v23 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Gam_emploi_du_temps_edt_occupation_loup_attentejoueur_Gam_emploi_du_temps_edt_exec_get_free_player = (int)AI_ul_RealResolveOneRefFunction(0x26000058u);
        fct_Gam_emploi_du_temps_edt_occupation_loup_attentejoueur_Gam_emploi_du_temps_edt_occupation_loup_poursuite = (int)AI_ul_RealResolveOneRefFunction(0x2600004Au);
    }
    v23 = 0;
    v9 = (tAI*)v24->o_manager;
    if (v9 == WOR_gst_Universe.univAIInstance)
    {
        v8 = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        v11 = (tGameObjectMain*)v24->o_manager;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v11 = AI_MainActor[(uint32_t)v9 - 1];
            v10 = v11;
        }
        else
        {
            v10 = 0;
        }
        v8 = v10->extended->AIInstance;
    }
    if (*((uint32_t*)v8->instanceVarDataCurrent + 24))
    {
        v18 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_occupation_loup_attentejoueur_Gam_emploi_du_temps_edt_exec_get_free_player);
        if (v18 != 1000)
            return v18;
        v7 = (tGameObjectMain*)v24->o_perso_a_attraper;
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
            v24->f_time_changetarget = TIM_gf_MainClock - TIM_gf_LockTime;
            v24->f_time_loup_init = v24->f_time_changetarget;
            v22 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_occupation_loup_attentejoueur_Gam_emploi_du_temps_edt_occupation_loup_poursuite);
            if (v22 != 1000)
                return v22;
        }
        else
        {
            v23 = 1;
        }
    }
    else
    {
        v23 = 1;
    }
    if (v23)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
        v12 = AI_EvalFunc_ARRObjNearestOfPoint_C((tGameObjectMain**)v24->o_attente_list, v24->i_attente_nbr, &a1);
        v14 = v24->o_attente_list[v12];
        v5 = (tGameObjectMain*)v14;
        if (v14)
        {
            if (v14 <= 2)
                v5 = AI_MainActor[v14 - 1];
            v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v5);
        }
        else
        {
            v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
        }
        v19 = (float)v1;
        if (v19 >= 1.0)
        {
            AI_EvalFunc_ACTActionSet_C_CURRENT(29);
            AI_EvalFunc_ANIFrequencySet_C(AI_gpst_CurrentGameObject, 0, v24->i_fuite_frequency);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
            v4 = (tGameObjectMain*)v14;
            if (v14)
            {
                if (v14 <= 2)
                    v4 = AI_MainActor[v14 - 1];
                AI_EvalFunc_OBJGetPos_C(v4, &v16);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v16);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v21);
            MATH_SubstractVec(&a3, &v16, &v21);
            v17 = TIM_gf_dt;
            a4 = 7.0f * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
            v15.x = 0.0;
            v15.y = 0.0;
            v15.z = 1.0;
            AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a5, &v15);
        }
        else
        {
            AI_EvalFunc_ACTActionSet_C_CURRENT(20);
        }
    }
    return 1000;
}

int Gam_emploi_du_temps_edt_exec_recherche_chemin()
{
    float a4; // [esp+0h] [ebp-B4Ch]
    float a4a; // [esp+0h] [ebp-B4Ch]
    float a4b; // [esp+0h] [ebp-B4Ch]
    float a7; // [esp+8h] [ebp-B44h]
    float v5; // [esp+Ch] [ebp-B40h]
    tScriptStruct_ActorGamEmploi* v6; // [esp+10h] [ebp-B3Ch]
    Vec3* v7; // [esp+54h] [ebp-AF8h]
    Vec3* v8; // [esp+5Ch] [ebp-AF0h]
    Vec3* v9; // [esp+64h] [ebp-AE8h]
    BOOL v10; // [esp+9Ch] [ebp-AB0h]
    Vec3 v11; // [esp+A0h] [ebp-AACh] BYREF
    Vec3 v12; // [esp+ACh] [ebp-AA0h] BYREF
    Vec3 v13; // [esp+B8h] [ebp-A94h] BYREF
    Vec3 v14[200]; // [esp+C4h] [ebp-A88h] BYREF
    int v16; // [esp+A28h] [ebp-124h]
    Vec3 a2; // [esp+A30h] [ebp-11Ch] BYREF
    BOOL v19; // [esp+A40h] [ebp-10Ch]
    BOOL v20; // [esp+A50h] [ebp-FCh]
    Vec3 v21; // [esp+A58h] [ebp-F4h] BYREF
    Vec3 a1; // [esp+A68h] [ebp-E4h] BYREF
    Vec3 v24[2]; // [esp+A74h] [ebp-D8h] BYREF
    float v25; // [esp+A8Ch] [ebp-C0h]
    Vec3 v26[2]; // [esp+A90h] [ebp-BCh] BYREF
    Vec3 dest; // [esp+AACh] [ebp-A0h] BYREF
    float v28; // [esp+AB8h] [ebp-94h]
    float v29; // [esp+ABCh] [ebp-90h]
    int v30; // [esp+AC0h] [ebp-8Ch]
    int v31; // [esp+AC4h] [ebp-88h]
    Vec3 v32; // [esp+AC8h] [ebp-84h] BYREF
    Vec3 v33; // [esp+AE4h] [ebp-68h] BYREF
    Vec3 v34; // [esp+AF0h] [ebp-5Ch] BYREF
    Vec3 v35; // [esp+AFCh] [ebp-50h] BYREF
    BOOL v36; // [esp+B08h] [ebp-44h]
    tScriptStruct_ActorGamEmploi* v37; // [esp+B0Ch] [ebp-40h]
    float v38; // [esp+B10h] [ebp-3Ch]
    float v39; // [esp+B14h] [ebp-38h]
    Vec3 v40; // [esp+B18h] [ebp-34h] BYREF
    Vec3 v41; // [esp+B24h] [ebp-28h] BYREF
    Vec3 a3; // [esp+B40h] [ebp-Ch] BYREF

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    v34.x = 1.0;
    v34.y = 0.0;
    v34.z = 0.0;
    v13.x = 1.0;
    v13.y = 0.0;
    v13.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v37 = v6;
    v30 = 0;
    v19 = 0;
    v38 = 0.0;
    v31 = 0;
    v16 = 0;
    v25 = 0.0;
    if (v6->f_way_len_big == 0.0)
        v25 = 20.0;
    else
        v25 = v37->f_way_len_big;
    v37->f_way_len_big = 0.0;
    if (v37->f_way_test_born == 0.0)
        v38 = 4.0;
    else
        v38 = v37->f_way_test_born;
    v37->f_way_test_born = 0.0;
    v37->v_way_destpos.z = v37->v_way_currentpos.z;
    v20 = AI_EvalFunc_GRIDIsIn_C(AI_gpst_CurrentGameObject, &v37->v_way_currentpos);
    if (v20 && (v36 = AI_EvalFunc_GRIDIsIn_C(AI_gpst_CurrentGameObject, &v37->v_way_destpos)))
    {
        v19 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x4000);
        AI_EvalFunc_GRIDCurrentSet_C(v19);
        AI_EvalFunc_GRIDDestSet_C(v37->i_way_etage_dest);
        v10 = (!v37->i_way_etage_dest || !v19) && (v37->i_way_etage_dest || v19);
        v16 = AI_EvalFunc_GRIDGetCapa_C(AI_gpst_CurrentGameObject, &v37->v_way_case_next);
        if (!v10 && (v16 & 0xC0) != 0 || v10 && (v16 & 0xC0) != 0 && (v16 & 4) == 0)
            v37->i_way_force_recompute = 1;
        AI_EvalFunc_GRIDPosGet_C(AI_gpst_CurrentGameObject, &v37->v_way_currentpos, &v12);
        if (AI2C_DistOfTwoPointsIsZero(&v37->v_way_old_case, &v12) && !v37->i_way_force_recompute)
        {
            MATH_SubstractVec(&v11, &v37->v_way_case_dest, &v37->v_way_currentpos);
            v13.x = v11.x;
            v13.y = v11.y;
            v13.z = 0.0;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(v24);
            AI_EvalFunc_MATHVecNormalize_C(&v13, &v33);
            v28 = TIM_gf_dt;
            a4a = v37->f_way_blendrotate * (TIM_gf_dt * 30.0f);
            AI_EvalFunc_MATHVecBlendRotate_C(v24, &v33, a4a, &v37->v_way_sight);
            AI_EvalFunc_GRIDPosGet_C(AI_gpst_CurrentGameObject, &v37->v_way_currentpos, &v32);
            if (AI2C_DistOfTwoPointsIsZero(&v37->v_way_case_dest, &v32))
            {
                v37->i_way_force_recompute = 1;
            }
            else if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x80))
            {
                v37->i_way_force_recompute = 1;
                AI_EvalFunc_COLNormalGet_C(AI_gpst_CurrentGameObject, 0x80, &v21);
                a1.x = v21.x;
                a1.y = v21.y;
                a1.z = 0.0;
                AI_EvalFunc_MATHVecSetNormVector_C(&a1, 1.0);
            }
        }
        else
        {
            if (!v37->i_way_force_recompute)
                v37->f_way_time_newcell = TIM_gf_MainClock - TIM_gf_LockTime;
            if (v37->i_way_search_around_destpos)
            {
                AI_EvalFunc_GRIDCurrentSet_C(v37->i_way_etage_dest);
                v5 = (float)v37->i_way_algo_limitdestpos;
                AI_EvalFunc_GRIDCptDest_C(AI_gpst_CurrentGameObject, &v37->v_way_destpos, v5, &v41);
                v34 = v41;
                AI_EvalFunc_GRIDCurrentSet_C(v19);
                if (AI2C_VecEqNull(&v34))
                {
                    v37->i_way_destpos_error = 1;
                    v37->i_way_moving = 0;
                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                    return 1000;
                }
                v37->i_way_destpos_error = 0;
            }
            else
            {
                v34 = v37->v_way_destpos;
            }
            AI_EvalFunc_GRIDPosGet_C(AI_gpst_CurrentGameObject, &v37->v_way_currentpos, &v37->v_way_old_case);
            a7 = (float)v37->i_way_algo_limitdestpos;
            v30 = AI_EvalFunc_GRIDShortWay_C(
                AI_gpst_CurrentGameObject,
                v14,
                &v37->v_way_currentpos,
                &v34,
                0,
                (unsigned int)v38,
                a7,
                (unsigned int)v25,
                99);
            if (v30 < 2)
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v37->v_way_sight);
                v37->i_way_force_recompute = 1;
                v37->i_way_moving = 0;
            }
            else
            {
                v31 = AI_EvalFunc_GRIDSmoothWay_C(AI_gpst_CurrentGameObject, v14, v30);
                v37->i_way_force_recompute = 0;
                v9 = &v14[v30 - 2];
                v37->v_way_case_next = *v9;
                if (v31 >= v30 - 2)
                {
                    v7 = &v14[v30 - 2];
                    v37->v_way_case_dest = *v7;
                }
                else
                {
                    v8 = &v14[v31];
                    v37->v_way_case_dest = *v8;
                }
                MATH_SubstractVec(&dest, &v37->v_way_case_dest, &v37->v_way_currentpos);
                v13.x = dest.x;
                v13.y = dest.y;
                v13.z = 0.0;
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
                AI_EvalFunc_MATHVecNormalize_C(&v13, &a3);
                v29 = TIM_gf_dt;
                a4 = v37->f_way_blendrotate * (TIM_gf_dt * 30.0f);
                AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &v37->v_way_sight);
            }
        }
    }
    else
    {
        MATH_SubstractVec(&v35, &v37->v_way_destpos, &v37->v_way_currentpos);
        v13.x = v35.x;
        v13.y = v35.y;
        v13.z = 0.0;
        if (AI_EvalFunc_MTHVecNullToler_C(&v13, 0.5))
        {
            v37->i_way_force_recompute = 1;
            v37->i_way_moving = 0;
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v40);
            AI_EvalFunc_MATHVecNormalize_C(&v13, v26);
            v39 = TIM_gf_dt;
            a4b = v37->f_way_blendrotate * (TIM_gf_dt * 30.0f);
            AI_EvalFunc_MATHVecBlendRotate_C(&v40, v26, a4b, &v37->v_way_sight);
        }
    }
    return 1000;
}

int Gam_emploi_du_temps_edt_occupation_jump()
{
    tScriptStruct_ActorGamEmploi* v1; // [esp+0h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v1->i_old_capa = 0;
    AI_EvalFunc_ACTActionSet_C_CURRENT(10);
    return 1000;
}

int Gam_emploi_du_temps_edt_occupation_loup_poursuite()
{
    int result; // eax
    float a4; // [esp+0h] [ebp-370h]
    float a4a; // [esp+0h] [ebp-370h]
    float v3; // [esp+Ch] [ebp-364h]
    float v4; // [esp+10h] [ebp-360h]
    tScriptStruct_ActorGamEmploi* v5; // [esp+20h] [ebp-350h]
    tGameObjectMain* v6; // [esp+28h] [ebp-348h]
    float v7; // [esp+2Ch] [ebp-344h]
    Vec3 v8; // [esp+34h] [ebp-33Ch] BYREF
    Vec3* v9; // [esp+40h] [ebp-330h]
    Vec3* v10; // [esp+44h] [ebp-32Ch]
    tGameObjectMain* v11; // [esp+48h] [ebp-328h]
    tGameObjectMain* v12; // [esp+4Ch] [ebp-324h]
    Vec3* v13; // [esp+50h] [ebp-320h]
    Vec3* v14; // [esp+54h] [ebp-31Ch]
    tGameObjectMain* v15; // [esp+58h] [ebp-318h]
    tGameObjectMain* v16; // [esp+5Ch] [ebp-314h]
    tGameObjectMain* v17; // [esp+60h] [ebp-310h]
    tGameObjectMain* v18; // [esp+64h] [ebp-30Ch]
    tGameObjectMain* v19; // [esp+68h] [ebp-308h]
    tGameObjectMain* v20; // [esp+6Ch] [ebp-304h]
    tAI* v21; // [esp+70h] [ebp-300h]
    tAI* v22; // [esp+74h] [ebp-2FCh]
    tGameObjectMain* v23; // [esp+78h] [ebp-2F8h]
    tGameObjectMain* v24; // [esp+7Ch] [ebp-2F4h]
    tAI* v25; // [esp+80h] [ebp-2F0h]
    tAI* v26; // [esp+84h] [ebp-2ECh]
    tGameObjectMain* v27; // [esp+88h] [ebp-2E8h]
    tGameObjectMain* v28; // [esp+8Ch] [ebp-2E4h]
    tAI* v29; // [esp+90h] [ebp-2E0h]
    tAI* v30; // [esp+94h] [ebp-2DCh]
    tGameObjectMain* v31; // [esp+98h] [ebp-2D8h]
    tGameObjectMain* v32; // [esp+9Ch] [ebp-2D4h]
    tAI* v33; // [esp+A0h] [ebp-2D0h]
    tAI* v34; // [esp+A4h] [ebp-2CCh]
    tGameObjectMain* v35; // [esp+A8h] [ebp-2C8h]
    tGameObjectMain* v36; // [esp+ACh] [ebp-2C4h]
    tAI* v37; // [esp+B0h] [ebp-2C0h]
    tAI* v38; // [esp+B4h] [ebp-2BCh]
    tGameObjectMain* v39; // [esp+B8h] [ebp-2B8h]
    tGameObjectMain* v40; // [esp+BCh] [ebp-2B4h]
    tAI* v41; // [esp+C0h] [ebp-2B0h]
    tAI* v42; // [esp+C4h] [ebp-2ACh]
    tGameObjectMain* v43; // [esp+C8h] [ebp-2A8h]
    tGameObjectMain* v44; // [esp+CCh] [ebp-2A4h]
    tAI* v45; // [esp+D0h] [ebp-2A0h]
    tAI* v46; // [esp+D4h] [ebp-29Ch]
    tGameObjectMain* v47; // [esp+D8h] [ebp-298h]
    tGameObjectMain* v48; // [esp+DCh] [ebp-294h]
    tAI* v49; // [esp+E0h] [ebp-290h]
    tAI* v50; // [esp+E4h] [ebp-28Ch]
    tGameObjectMain* v51; // [esp+E8h] [ebp-288h]
    tGameObjectMain* v52; // [esp+ECh] [ebp-284h]
    int v53; // [esp+F0h] [ebp-280h]
    tAI* v54; // [esp+F4h] [ebp-27Ch]
    tAI* v55; // [esp+F8h] [ebp-278h]
    tGameObjectMain* v56; // [esp+FCh] [ebp-274h]
    tGameObjectMain* v57; // [esp+100h] [ebp-270h]
    tAI* v58; // [esp+104h] [ebp-26Ch]
    tAI* v59; // [esp+108h] [ebp-268h]
    tGameObjectMain* v60; // [esp+10Ch] [ebp-264h]
    tGameObjectMain* v61; // [esp+110h] [ebp-260h]
    tGameObjectMain* v62; // [esp+114h] [ebp-25Ch]
    tGameObjectMain* v63; // [esp+118h] [ebp-258h]
    tGameObjectMain* a1; // [esp+11Ch] [ebp-254h]
    tGameObjectMain* v65; // [esp+120h] [ebp-250h]
    tAI* v66; // [esp+124h] [ebp-24Ch]
    tAI* v67; // [esp+128h] [ebp-248h]
    tGameObjectMain* v68; // [esp+12Ch] [ebp-244h]
    tGameObjectMain* v69; // [esp+130h] [ebp-240h]
    tGameObjectMain* v70; // [esp+134h] [ebp-23Ch]
    tGameObjectMain* v71; // [esp+138h] [ebp-238h]
    tGameObjectMain* v72; // [esp+13Ch] [ebp-234h]
    tGameObjectMain* v73; // [esp+140h] [ebp-230h]
    tGameObjectMain* v74; // [esp+144h] [ebp-22Ch]
    tGameObjectMain* v75; // [esp+148h] [ebp-228h]
    tAI* v76; // [esp+14Ch] [ebp-224h]
    tAI* v77; // [esp+150h] [ebp-220h]
    tGameObjectMain* v78; // [esp+154h] [ebp-21Ch]
    tGameObjectMain* v79; // [esp+158h] [ebp-218h]
    int v80; // [esp+15Ch] [ebp-214h]
    tAI* v81; // [esp+160h] [ebp-210h]
    tAI* v82; // [esp+164h] [ebp-20Ch]
    tGameObjectMain* v83; // [esp+168h] [ebp-208h]
    tGameObjectMain* v84; // [esp+16Ch] [ebp-204h]
    int v85; // [esp+170h] [ebp-200h]
    tAI* v86; // [esp+174h] [ebp-1FCh]
    tAI* v87; // [esp+178h] [ebp-1F8h]
    tGameObjectMain* v88; // [esp+17Ch] [ebp-1F4h]
    tGameObjectMain* v89; // [esp+180h] [ebp-1F0h]
    tAI* v90; // [esp+184h] [ebp-1ECh]
    tAI* v91; // [esp+188h] [ebp-1E8h]
    tGameObjectMain* v92; // [esp+18Ch] [ebp-1E4h]
    tGameObjectMain* v93; // [esp+190h] [ebp-1E0h]
    tGameObjectMain* v94; // [esp+194h] [ebp-1DCh]
    tGameObjectMain* v95; // [esp+198h] [ebp-1D8h]
    tGameObjectMain* v96; // [esp+19Ch] [ebp-1D4h]
    tGameObjectMain* v97; // [esp+1A0h] [ebp-1D0h]
    tGameObjectMain* v98; // [esp+1A4h] [ebp-1CCh]
    tGameObjectMain* v99; // [esp+1A8h] [ebp-1C8h]
    tGameObjectMain* v100; // [esp+1ACh] [ebp-1C4h]
    tGameObjectMain* v101; // [esp+1B0h] [ebp-1C0h]
    Vec3* v102; // [esp+1B4h] [ebp-1BCh]
    Vec3* v103; // [esp+1B8h] [ebp-1B8h]
    Vec3 v104; // [esp+1BCh] [ebp-1B4h] BYREF
    float v105; // [esp+1C8h] [ebp-1A8h]
    Vec3 v106; // [esp+1D0h] [ebp-1A0h] BYREF
    Vec3 v108; // [esp+1F0h] [ebp-180h] BYREF
    Vec3 v109; // [esp+1FCh] [ebp-174h] BYREF
    float v110; // [esp+208h] [ebp-168h]
    Vec3 v111; // [esp+20Ch] [ebp-164h] BYREF
    Vec3 v112; // [esp+218h] [ebp-158h] BYREF
    Vec3 v113; // [esp+224h] [ebp-14Ch] BYREF
    Vec3 v114; // [esp+230h] [ebp-140h] BYREF
    Vec3 v115; // [esp+23Ch] [ebp-134h] BYREF
    Vec3 dest; // [esp+248h] [ebp-128h] BYREF
    Vec3 v117; // [esp+254h] [ebp-11Ch] BYREF
    Vec3 v118; // [esp+26Ch] [ebp-104h] BYREF
    Vec3 a3; // [esp+278h] [ebp-F8h] BYREF
    float v120; // [esp+284h] [ebp-ECh]
    float v121; // [esp+28Ch] [ebp-E4h]
    Vec3 v122; // [esp+29Ch] [ebp-D4h] BYREF
    Vec3 v123; // [esp+2A8h] [ebp-C8h] BYREF
    int a5; // [esp+2C0h] [ebp-B0h] BYREF
    int v126; // [esp+2C4h] [ebp-ACh]
    Vec3 v127; // [esp+2C8h] [ebp-A8h] BYREF
    Vec3 v128; // [esp+2D4h] [ebp-9Ch] BYREF
    tGameObjectMain* v129; // [esp+2E0h] [ebp-90h]
    Vec3 v131; // [esp+2E8h] [ebp-88h] BYREF
    Vec3 v132; // [esp+2F4h] [ebp-7Ch] BYREF
    int v133; // [esp+300h] [ebp-70h]
    float v134; // [esp+308h] [ebp-68h]
    Vec3 v135; // [esp+30Ch] [ebp-64h] BYREF
    int v136; // [esp+318h] [ebp-58h]
    Vec3 a2; // [esp+320h] [ebp-50h] BYREF
    tScriptStruct_ActorGamEmploi* v139; // [esp+32Ch] [ebp-44h]
    int v140; // [esp+330h] [ebp-40h]
    Vec3 v141; // [esp+334h] [ebp-3Ch] BYREF
    int v142; // [esp+340h] [ebp-30h]
    int v143; // [esp+344h] [ebp-2Ch]
    int v144; // [esp+348h] [ebp-28h]
    BOOL v145; // [esp+350h] [ebp-20h]
    Vec3 v146; // [esp+358h] [ebp-18h] BYREF
    float v147; // [esp+364h] [ebp-Ch]
    int v148; // [esp+36Ch] [ebp-4h]

    v122.x = 1.0;
    v122.y = 0.0;
    v122.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v139 = v5;
    v133 = 0;
    v136 = 0;
    a5 = 0;
    v147 = 0.0;
    v129 = 0;
    v126 = 0;
    v140 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Gam_emploi_du_temps_edt_occupation_loup_poursuite_Gam_emploi_du_temps_edt_occupation_loup_attentejoueur = (int)AI_ul_RealResolveOneRefFunction(0x26000049u);
        fct_Gam_emploi_du_temps_edt_occupation_loup_poursuite_Gam_emploi_du_temps_edt_occupation_loup_nargue = (int)AI_ul_RealResolveOneRefFunction(0x2600004Du);
        fct_Gam_emploi_du_temps_edt_occupation_loup_poursuite_Gam_emploi_du_temps_edt_exec_recherche_chemin = (int)AI_ul_RealResolveOneRefFunction(0x7100F791u);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)8)
        goto LABEL_188;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v111);
    v102 = &v122;
    v103 = &v111;
    v122 = v111;
    v136 = 0;
    v101 = (tGameObjectMain*)v139->o_perso_a_attraper;
    if (v101)
    {
        if ((unsigned int)v101 <= 2)
            v101 = AI_MainActor[(uint32_t)v101 - 1];
        v100 = v101;
    }
    else
    {
        v100 = 0;
    }
    if (v100)
    {
        v99 = (tGameObjectMain*)v139->o_perso_a_attraper;
        if (v99)
        {
            if ((unsigned int)v99 <= 2)
                v99 = AI_MainActor[(uint32_t)v99 - 1];
            v98 = v99;
        }
        else
        {
            v98 = 0;
        }
        if (!AI_EvalFunc_OBJ_CustomBitsTest_C(v98, 0x100))
            goto LABEL_25;
        v97 = (tGameObjectMain*)v139->o_perso_a_attraper;
        if (v97)
        {
            if ((unsigned int)v97 <= 2)
                v97 = AI_MainActor[(uint32_t)v97 - 1];
            v96 = v97;
        }
        else
        {
            v96 = 0;
        }
        if (AI_EvalFunc_OBJ_CustomBitsTest_C(v96, 0x200))
            LABEL_25:
        v139->o_perso_a_attraper = 0;
    }
    if (AI_EvalFunc_TIMEElapsed_C(v139->f_time_changetarget, v139->f_time_changetarget_delay))
    {
        v139->f_time_changetarget = TIM_gf_MainClock - TIM_gf_LockTime;
        v139->f_time_changetarget_delay = AI_EvalFunc_MATHRandFloat_C(0.40000001f, 1.0);
        v139->o_perso_a_attraper = 0;
    }
    v95 = (tGameObjectMain*)v139->o_perso_a_attraper;
    if (v95)
    {
        if ((unsigned int)v95 <= 2)
            v95 = AI_MainActor[(uint32_t)v95 - 1];
        v94 = v95;
    }
    else
    {
        v94 = 0;
    }
    if (!v94)
    {
        v91 = (tAI*)v139->o_manager;
        if (v91 == WOR_gst_Universe.univAIInstance)
        {
            v90 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v93 = (tGameObjectMain*)v91;
            if (v91)
            {
                if ((unsigned int)v93 <= 2)
                    v93 = AI_MainActor[(uint32_t)v93 - 1];
                v92 = v93;
            }
            else
            {
                v92 = 0;
            }
            v90 = v92->extended->AIInstance;
        }
        v126 = *((uint32_t*)v90->instanceVarDataCurrent + 24);
        if (v126)
        {
            v87 = (tAI*)v139->o_manager;
            if (v87 == WOR_gst_Universe.univAIInstance)
            {
                v86 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v89 = (tGameObjectMain*)v87;
                if (v87)
                {
                    if ((unsigned int)v89 <= 2)
                        v89 = AI_MainActor[(uint32_t)v89 - 1];
                    v88 = v89;
                }
                else
                {
                    v88 = 0;
                }
                v86 = v88->extended->AIInstance;
            }
            v85 = 0;
            v126 = AI_EvalFunc_ARRSortByFlags_C((tGameObjectMain**)v86->instanceVarDataCurrent + 4, v126, -1, 512, 5);
            if (v126)
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v108);
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v128);
                v82 = (tAI*)v139->o_manager;
                if (v82 == WOR_gst_Universe.univAIInstance)
                {
                    v81 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v84 = (tGameObjectMain*)v82;
                    if (v82)
                    {
                        if ((unsigned int)v84 <= 2)
                            v84 = AI_MainActor[(uint32_t)v84 - 1];
                        v83 = v84;
                    }
                    else
                    {
                        v83 = 0;
                    }
                    v81 = v83->extended->AIInstance;
                }
                v80 = 0;
                v140 = AI_EvalFunc_ARRObjBestSight_C((tGameObjectMain**)v81->instanceVarDataCurrent + 4, v126, &v108, &v128, 10.0);
                v77 = (tAI*)v139->o_manager;
                if (v77 == WOR_gst_Universe.univAIInstance)
                {
                    v76 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v79 = (tGameObjectMain*)v77;
                    if (v77)
                    {
                        if ((unsigned int)v79 <= 2)
                            v79 = AI_MainActor[(uint32_t)v79 - 1];
                        v78 = v79;
                    }
                    else
                    {
                        v78 = 0;
                    }
                    v76 = v78->extended->AIInstance;
                }
                v139->o_perso_a_attraper = *((uint32_t*)v76->instanceVarDataCurrent + v140 + 4);
            }
        }
        v139->i_way_force_recompute = 1;
        v75 = (tGameObjectMain*)v139->o_perso_a_attraper;
        if (v75)
        {
            if ((unsigned int)v75 <= 2)
                v75 = AI_MainActor[(uint32_t)v75 - 1];
            v74 = v75;
        }
        else
        {
            v74 = 0;
        }
        if (!v74)
        {
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_occupation_loup_poursuite_Gam_emploi_du_temps_edt_occupation_loup_attentejoueur);
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
    }
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v112);
    v129 = AI_EvalFunc_COLBestWallGaoGet_C(AI_gpst_CurrentGameObject, &v112, &a5);
    v73 = v129;
    if (v129)
    {
        if ((unsigned int)v73 <= 2)
            v73 = AI_MainActor[(uint32_t)v73 - 1];
        v72 = v73;
    }
    else
    {
        v72 = 0;
    }
    v71 = (tGameObjectMain*)v139->o_perso_a_attraper;
    if (v71)
    {
        if ((unsigned int)v71 <= 2)
            v71 = AI_MainActor[(uint32_t)v71 - 1];
        v70 = v71;
    }
    else
    {
        v70 = 0;
    }
    if (v72 == v70)
    {
        v139->i_way_force_recompute = 1;
        v67 = (tAI*)v139->o_manager;
        if (v67 == WOR_gst_Universe.univAIInstance)
        {
            v66 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v69 = (tGameObjectMain*)v67;
            if (v67)
            {
                if ((unsigned int)v69 <= 2)
                    v69 = AI_MainActor[(uint32_t)v69 - 1];
                v68 = v69;
            }
            else
            {
                v68 = 0;
            }
            v66 = v68->extended->AIInstance;
        }
        *((uint32_t*)v66->instanceVarDataCurrent + 2) = v139->o_perso_a_attraper;
        v139->o_le_loup = v139->o_perso_a_attraper;
        v65 = (tGameObjectMain*)v139->o_perso_a_attraper;
        if (v65)
        {
            if ((unsigned int)v65 <= 2)
                v65 = AI_MainActor[(uint32_t)v65 - 1];
            a1 = v65;
        }
        else
        {
            a1 = 0;
        }
        AI_EvalFunc_OBJ_CustomBitsSet_C(a1, 1024, 0);
        v63 = (tGameObjectMain*)v139->o_le_loup;
        if (v63)
        {
            if ((unsigned int)v63 <= 2)
                v63 = AI_MainActor[(uint32_t)v63 - 1];
            v62 = v63;
        }
        else
        {
            v62 = 0;
        }
        v59 = (tAI*)v139->o_manager;
        if (v59 == WOR_gst_Universe.univAIInstance)
        {
            v58 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v61 = (tGameObjectMain*)v59;
            if (v59)
            {
                if ((unsigned int)v61 <= 2)
                    v61 = AI_MainActor[(uint32_t)v61 - 1];
                v60 = v61;
            }
            else
            {
                v60 = 0;
            }
            v58 = v60->extended->AIInstance;
        }
        v55 = (tAI*)v139->o_manager;
        if (v55 == WOR_gst_Universe.univAIInstance)
        {
            v54 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v57 = (tGameObjectMain*)v55;
            if (v55)
            {
                if ((unsigned int)v57 <= 2)
                    v57 = AI_MainActor[(uint32_t)v57 - 1];
                v56 = v57;
            }
            else
            {
                v56 = 0;
            }
            v54 = v56->extended->AIInstance;
        }
        v53 = 0;
        v140 = AI_EvalFunc_ARRObjSearch_C(
            (tGameObjectMain**)v54->instanceVarDataCurrent + 4,
            *((uint32_t*)v58->instanceVarDataCurrent + 24),
            v62);
        v50 = (tAI*)v139->o_manager;
        if (v50 == WOR_gst_Universe.univAIInstance)
        {
            v49 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v52 = (tGameObjectMain*)v50;
            if (v50)
            {
                if ((unsigned int)v52 <= 2)
                    v52 = AI_MainActor[(uint32_t)v52 - 1];
                v51 = v52;
            }
            else
            {
                v51 = 0;
            }
            v49 = v51->extended->AIInstance;
        }
        --* ((uint32_t*)v49->instanceVarDataCurrent + 24);
        v46 = (tAI*)v139->o_manager;
        if (v46 == WOR_gst_Universe.univAIInstance)
        {
            v45 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v48 = (tGameObjectMain*)v46;
            if (v46)
            {
                if ((unsigned int)v48 <= 2)
                    v48 = AI_MainActor[(uint32_t)v48 - 1];
                v47 = v48;
            }
            else
            {
                v47 = 0;
            }
            v45 = v47->extended->AIInstance;
        }
        if (v140 != *((uint32_t*)v45->instanceVarDataCurrent + 24))
        {
            v42 = (tAI*)v139->o_manager;
            if (v42 == WOR_gst_Universe.univAIInstance)
            {
                v41 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v44 = (tGameObjectMain*)v42;
                if (v42)
                {
                    if ((unsigned int)v44 <= 2)
                        v44 = AI_MainActor[(uint32_t)v44 - 1];
                    v43 = v44;
                }
                else
                {
                    v43 = 0;
                }
                v41 = v43->extended->AIInstance;
            }
            v133 = *((uint32_t*)v41->instanceVarDataCurrent + 24);
            v38 = (tAI*)v139->o_manager;
            if (v38 == WOR_gst_Universe.univAIInstance)
            {
                v37 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v40 = (tGameObjectMain*)v38;
                if (v38)
                {
                    if ((unsigned int)v40 <= 2)
                        v40 = AI_MainActor[(uint32_t)v40 - 1];
                    v39 = v40;
                }
                else
                {
                    v39 = 0;
                }
                v37 = v39->extended->AIInstance;
            }
            v34 = (tAI*)v139->o_manager;
            if (v34 == WOR_gst_Universe.univAIInstance)
            {
                v33 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v36 = (tGameObjectMain*)v34;
                if (v34)
                {
                    if ((unsigned int)v36 <= 2)
                        v36 = AI_MainActor[(uint32_t)v36 - 1];
                    v35 = v36;
                }
                else
                {
                    v35 = 0;
                }
                v33 = v35->extended->AIInstance;
            }
            *((uint32_t*)v33->instanceVarDataCurrent + v140 + 4) = *((uint32_t*)v37->instanceVarDataCurrent + v133 + 4);
        }
        v30 = (tAI*)v139->o_manager;
        if (v30 == WOR_gst_Universe.univAIInstance)
        {
            v29 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v32 = (tGameObjectMain*)v30;
            if (v30)
            {
                if ((unsigned int)v32 <= 2)
                    v32 = AI_MainActor[(uint32_t)v32 - 1];
                v31 = v32;
            }
            else
            {
                v31 = 0;
            }
            v29 = v31->extended->AIInstance;
        }
        v26 = (tAI*)v139->o_manager;
        if (v26 == WOR_gst_Universe.univAIInstance)
        {
            v25 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v28 = (tGameObjectMain*)v26;
            if (v26)
            {
                if ((unsigned int)v28 <= 2)
                    v28 = AI_MainActor[(uint32_t)v28 - 1];
                v27 = v28;
            }
            else
            {
                v27 = 0;
            }
            v25 = v27->extended->AIInstance;
        }
        *((uint32_t*)v25->instanceVarDataCurrent + *((uint32_t*)v29->instanceVarDataCurrent + 24) + 4) = (uint32_t)AI_gpst_CurrentGameObject;
        v22 = (tAI*)v139->o_manager;
        if (v22 == WOR_gst_Universe.univAIInstance)
        {
            v21 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v24 = (tGameObjectMain*)v22;
            if (v22)
            {
                if ((unsigned int)v24 <= 2)
                    v24 = AI_MainActor[(uint32_t)v24 - 1];
                v23 = v24;
            }
            else
            {
                v23 = 0;
            }
            v21 = v23->extended->AIInstance;
        }
        ++* ((uint32_t*)v21->instanceVarDataCurrent + 24);
        AI_EvalFunc_ACTActionSet_C_CURRENT(28);
    LABEL_188:
        if (AI_EvalFunc_ACTActionFinished_C_CURRENT())
        {
            v139->i_way_force_recompute = 1;
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_occupation_loup_poursuite_Gam_emploi_du_temps_edt_occupation_loup_nargue);
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v113);
            v20 = (tGameObjectMain*)v139->o_perso_a_attraper;
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
            AI_EvalFunc_OBJGetPos_C(v19, &v118);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v104);
            MATH_SubstractVec(&dest, &v118, &v104);
            v134 = TIM_gf_dt;
            a4 = 5.0f * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlend_C(&v113, &dest, a4, &v123);
            v146.x = 0.0;
            v146.y = 0.0;
            v146.z = 1.0;
            AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v123, &v146);
            *AI_gppst_CurrentJumpNode = (tAINode*)8;
            result = 0;
        }
        return result;
    }
    v18 = (tGameObjectMain*)v139->o_perso_a_attraper;
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
        v16 = (tGameObjectMain*)v139->o_perso_a_attraper;
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
        AI_EvalFunc_OBJGetPos_C(v15, &v127);
        MATH_SubstractVec(&v115, &v127, &v122);
        AI_EvalFunc_MATHVecNormalize_C(&v115, &v114);
        v131.x = v114.x * 0.75f;
        v131.y = v114.y * 0.75f;
        v131.z = v114.z * 0.75f;
        v13 = &a3;
        v14 = &v131;
        a3 = v131;
        v12 = (tGameObjectMain*)v139->o_perso_a_attraper;
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
        AI_EvalFunc_OBJGetPos_C(v11, &v106);
        MATH_SubstractVec(&v141, &v106, &a3);
        v9 = &v139->v_way_destpos;
        v10 = &v141;
        v139->v_way_destpos.x = v141.x;
        v9->y = v10->y;
        v9->z = v10->z;
        MATH_SubstractVec(&v8, &v139->v_way_destpos, &v122);
        v121 = MATH_VecLen(&v8);
        if (v121 >= 1.0)
        {
            v142 = 0;
            v143 = 0;
            v144 = 1065353216;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v139->v_way_currentpos);
            v139->i_way_etage_dest = 0;
            v139->i_way_moving = 1;
            v139->f_way_blendrotate = 0.2f;
            v148 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_occupation_loup_poursuite_Gam_emploi_du_temps_edt_exec_recherche_chemin);
            if (v148 != 1000)
                return v148;
            if (!v139->i_way_moving)
                v136 = 1;
        }
        else
        {
            v139->i_way_moving = 1;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
            v120 = TIM_gf_dt;
            a4a = v139->f_way_blendrotate * (TIM_gf_dt * 30.0f);
            AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4a, &v139->v_way_sight);
        }
        if (v139->i_way_moving)
        {
            AI_EvalFunc_ACTActionSet_C_CURRENT(21);
            v139->f_time_keep_anim = TIM_gf_MainClock - TIM_gf_LockTime;
            v105 = TIM_gf_MainClock - TIM_gf_LockTime;
            v7 = v105 - v139->f_time_loup_init;
            if (v7 > 5.0)
                v4 = 5.0;
            else
                v4 = v105 - v139->f_time_loup_init;
            v110 = v4;
            v147 = v4 * 32.0f / 5.0f;
            v3 = (float)v139->i_fuite_frequency;
            AI_EvalFunc_ANIFrequencySet_C(AI_gpst_CurrentGameObject, 0, (unsigned int)(v3 - (32.0f - v147)));
            v135.x = 0.0;
            v135.y = 0.0;
            v135.z = 1.0;
            AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v139->v_way_sight, &v135);
        }
        else if (!v136 || (v145 = AI_EvalFunc_TIMEElapsed_C(v139->f_time_keep_anim, 0.1f)))
        {
            AI_EvalFunc_ACTActionSet_C_CURRENT(0);
            v139->f_time_keep_anim = TIM_gf_MainClock - TIM_gf_LockTime;
        }
    }
    v6 = (tGameObjectMain*)v139->o_perso_a_attraper;
    if (v6)
    {
        if ((unsigned int)v6 <= 2)
            v6 = AI_MainActor[(uint32_t)v6 - 1];
        AI_EvalFunc_OBJGetPos_C(v6, &v117);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v117);
    }
    v132.x = 0.0;
    v132.y = 0.0;
    v132.z = 0.30000001f;
    MATH_VecAddVec(&v109, &v117, &v132);
    return 1000;
}

int Gam_emploi_du_temps_edt_exec_next_edt()
{
    tScriptStruct_ActorGamEmploi* v1; // [esp+8h] [ebp-E4h]
    tAI* v2; // [esp+Ch] [ebp-E0h]
    tAI* v3; // [esp+10h] [ebp-DCh]
    tGameObjectMain* v4; // [esp+14h] [ebp-D8h]
    tGameObjectMain* v5; // [esp+18h] [ebp-D4h]
    tAI* v6; // [esp+1Ch] [ebp-D0h]
    tAI* v7; // [esp+20h] [ebp-CCh]
    tGameObjectMain* v8; // [esp+24h] [ebp-C8h]
    tGameObjectMain* v9; // [esp+28h] [ebp-C4h]
    tAI* v10; // [esp+2Ch] [ebp-C0h]
    tAI* v11; // [esp+30h] [ebp-BCh]
    tGameObjectMain* v12; // [esp+34h] [ebp-B8h]
    tGameObjectMain* v13; // [esp+38h] [ebp-B4h]
    tAI* v14; // [esp+3Ch] [ebp-B0h]
    tAI* v15; // [esp+40h] [ebp-ACh]
    tGameObjectMain* v16; // [esp+44h] [ebp-A8h]
    tGameObjectMain* v17; // [esp+48h] [ebp-A4h]
    tAI* v18; // [esp+4Ch] [ebp-A0h]
    tAI* v19; // [esp+50h] [ebp-9Ch]
    tGameObjectMain* v20; // [esp+54h] [ebp-98h]
    tGameObjectMain* v21; // [esp+58h] [ebp-94h]
    tAI* v22; // [esp+60h] [ebp-8Ch]
    tAI* v23; // [esp+64h] [ebp-88h]
    tGameObjectMain* v24; // [esp+68h] [ebp-84h]
    tGameObjectMain* v25; // [esp+6Ch] [ebp-80h]
    tAI* v26; // [esp+70h] [ebp-7Ch]
    tAI* v27; // [esp+74h] [ebp-78h]
    tGameObjectMain* v28; // [esp+78h] [ebp-74h]
    tGameObjectMain* v29; // [esp+7Ch] [ebp-70h]
    tGameObjectMain* a3; // [esp+80h] [ebp-6Ch]
    tGameObjectMain* v31; // [esp+84h] [ebp-68h]
    tAI* v32; // [esp+88h] [ebp-64h]
    tAI* v33; // [esp+8Ch] [ebp-60h]
    tGameObjectMain* v34; // [esp+90h] [ebp-5Ch]
    tGameObjectMain* v35; // [esp+94h] [ebp-58h]
    tGameObjectMain* v36; // [esp+98h] [ebp-54h]
    tGameObjectMain* v37; // [esp+9Ch] [ebp-50h]
    tGameObjectMain* v38; // [esp+A0h] [ebp-4Ch]
    tGameObjectMain* v39; // [esp+A4h] [ebp-48h]
    tGameObjectMain* v40; // [esp+ACh] [ebp-40h]
    tGameObjectMain* v41; // [esp+B0h] [ebp-3Ch]
    tGameObjectMain* v42; // [esp+B4h] [ebp-38h]
    tGameObjectMain* v43; // [esp+B8h] [ebp-34h]
    tGameObjectMain* v44; // [esp+BCh] [ebp-30h]
    tGameObjectMain* v45; // [esp+C0h] [ebp-2Ch]
    tGameObjectMain* v46; // [esp+C4h] [ebp-28h]
    int v47; // [esp+E8h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (++v1->i_occupation_indice == 30
        || *(float*)AI_EvalVarArray_C2((char*)&v1->__f_occupation_hour_Dimension1, v1->i_occupation_indice) == 0.0)
    {
        v1->i_occupation_indice = 0;
    }
    v1->f_edt_hour = *(float*)AI_EvalVarArray_C2((char*)&v1->__f_occupation_hour_Dimension1, v1->i_occupation_indice);
    v1->i_occupation_courante = v1->i_occupation_type[v1->i_occupation_indice];
    v1->o_nearest = v1->o_occupation_pos[v1->i_occupation_indice];
    v46 = (tGameObjectMain*)v1->o_nearest;
    if (v46)
    {
        if ((unsigned int)v46 <= 2)
            v46 = AI_MainActor[(uint32_t)v46 - 1];
        v45 = v46;
    }
    else
    {
        v45 = 0;
    }
    if (!v45)
        v1->o_nearest = (int)AI_gpst_CurrentGameObject;
    v44 = (tGameObjectMain*)v1->o_nearest;
    if (v44)
    {
        if ((unsigned int)v44 <= 2)
            v44 = AI_MainActor[(uint32_t)v44 - 1];
        v43 = v44;
    }
    else
    {
        v43 = 0;
    }
    v42 = (tGameObjectMain*)v1->o_nearest_old;
    if (v42)
    {
        if ((unsigned int)v42 <= 2)
            v42 = AI_MainActor[(uint32_t)v42 - 1];
        v41 = v42;
    }
    else
    {
        v41 = 0;
    }
    if (v43 != v41 || v1->i_occupation_courante != v1->i_occupation_courante_old)
        v1->i_occupation_cherche_nouvelle = 1;
    v1->o_nearest_old = v1->o_nearest;
    v1->i_occupation_courante_old = v1->i_occupation_courante;
    if (AI_EvalFunc_OBJ_CustomBitsTest_C(AI_gpst_CurrentGameObject, 256))
    {
        if (AI_EvalFunc_OBJ_CustomBitsTest_C(AI_gpst_CurrentGameObject, 512))
        {
            v40 = (tGameObjectMain*)v1->o_safe;
            if (v40)
            {
                if ((unsigned int)v40 <= 2)
                    v40 = AI_MainActor[(uint32_t)v40 - 1];
                AI_EvalFunc_OBJ_CapaSet_C(v40, 0, 0x8000);
            }
            else
            {
                AI_EvalFunc_OBJ_CapaSet_C(0, 0, 0x8000);
            }
        }
        AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(0, 0x700);
        v39 = (tGameObjectMain*)v1->o_le_loup;
        if (v39)
        {
            if ((unsigned int)v39 <= 2)
                v39 = AI_MainActor[(uint32_t)v39 - 1];
            v38 = v39;
        }
        else
        {
            v38 = 0;
        }
        v37 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v37 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            v36 = v37;
        }
        else
        {
            v36 = 0;
        }
        if (v38 == v36)
        {
            v33 = (tAI*)v1->o_manager;
            if (v33 == WOR_gst_Universe.univAIInstance)
            {
                v32 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v35 = (tGameObjectMain*)v1->o_manager;
                if (v33)
                {
                    if ((unsigned int)v33 <= 2)
                        v35 = AI_MainActor[(uint32_t)v33 - 1];
                    v34 = v35;
                }
                else
                {
                    v34 = 0;
                }
                v32 = v34->extended->AIInstance;
            }
            *((uint32_t*)v32->instanceVarDataCurrent + 2) = 0;
        }
        else
        {
            v31 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v31 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                a3 = v31;
            }
            else
            {
                a3 = 0;
            }
            v27 = (tAI*)v1->o_manager;
            if (v27 == WOR_gst_Universe.univAIInstance)
            {
                v26 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v29 = (tGameObjectMain*)v1->o_manager;
                if (v27)
                {
                    if ((unsigned int)v27 <= 2)
                        v29 = AI_MainActor[(uint32_t)v27 - 1];
                    v28 = v29;
                }
                else
                {
                    v28 = 0;
                }
                v26 = v28->extended->AIInstance;
            }
            v23 = (tAI*)v1->o_manager;
            if (v23 == WOR_gst_Universe.univAIInstance)
            {
                v22 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v25 = (tGameObjectMain*)v1->o_manager;
                if (v23)
                {
                    if ((unsigned int)v23 <= 2)
                        v25 = AI_MainActor[(uint32_t)v23 - 1];
                    v24 = v25;
                }
                else
                {
                    v24 = 0;
                }
                v22 = v24->extended->AIInstance;
            }
            v47 = AI_EvalFunc_ARRObjSearch_C(
                (tGameObjectMain**)v22->instanceVarDataCurrent + 4,
                *((uint32_t*)v26->instanceVarDataCurrent + 24),
                a3);
            v19 = (tAI*)v1->o_manager;
            if (v19 == WOR_gst_Universe.univAIInstance)
            {
                v18 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v21 = (tGameObjectMain*)v1->o_manager;
                if (v19)
                {
                    if ((unsigned int)v19 <= 2)
                        v21 = AI_MainActor[(uint32_t)v19 - 1];
                    v20 = v21;
                }
                else
                {
                    v20 = 0;
                }
                v18 = v20->extended->AIInstance;
            }
            --* ((uint32_t*)v18->instanceVarDataCurrent + 24);
            v15 = (tAI*)v1->o_manager;
            if (v15 == WOR_gst_Universe.univAIInstance)
            {
                v14 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v17 = (tGameObjectMain*)v1->o_manager;
                if (v15)
                {
                    if ((unsigned int)v15 <= 2)
                        v17 = AI_MainActor[(uint32_t)v15 - 1];
                    v16 = v17;
                }
                else
                {
                    v16 = 0;
                }
                v14 = v16->extended->AIInstance;
            }
            if (v47 != *((uint32_t*)v14->instanceVarDataCurrent + 24))
            {
                v11 = (tAI*)v1->o_manager;
                if (v11 == WOR_gst_Universe.univAIInstance)
                {
                    v10 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v13 = (tGameObjectMain*)v1->o_manager;
                    if (v11)
                    {
                        if ((unsigned int)v11 <= 2)
                            v13 = AI_MainActor[(uint32_t)v11 - 1];
                        v12 = v13;
                    }
                    else
                    {
                        v12 = 0;
                    }
                    v10 = v12->extended->AIInstance;
                }
                v7 = (tAI*)v1->o_manager;
                if (v7 == WOR_gst_Universe.univAIInstance)
                {
                    v6 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v9 = (tGameObjectMain*)v1->o_manager;
                    if (v7)
                    {
                        if ((unsigned int)v7 <= 2)
                            v9 = AI_MainActor[(uint32_t)v7 - 1];
                        v8 = v9;
                    }
                    else
                    {
                        v8 = 0;
                    }
                    v6 = v8->extended->AIInstance;
                }
                v3 = (tAI*)v1->o_manager;
                if (v3 == WOR_gst_Universe.univAIInstance)
                {
                    v2 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v5 = (tGameObjectMain*)v1->o_manager;
                    if (v3)
                    {
                        if ((unsigned int)v3 <= 2)
                            v5 = AI_MainActor[(uint32_t)v3 - 1];
                        v4 = v5;
                    }
                    else
                    {
                        v4 = 0;
                    }
                    v2 = v4->extended->AIInstance;
                }
                *((uint32_t*)v2->instanceVarDataCurrent + v47 + 4) = *((uint32_t*)v6->instanceVarDataCurrent
                    + *((uint32_t*)v10->instanceVarDataCurrent + 24)
                    + 4);
            }
        }
    }
    return 1000;
}

int Gam_emploi_du_temps_edt_exec_cherche_dest()
{
    tGameObjectMain* v0; // eax
    BOOL v1; // eax
    BOOL v2; // eax
    int result; // eax
    tScriptStruct_ActorGamEmploi* v4; // [esp+0h] [ebp-8Ch]
    tGameObjectMain* v5; // [esp+10h] [ebp-7Ch]
    tGameObjectMain* v6; // [esp+18h] [ebp-74h]
    tGameObjectMain* v7; // [esp+20h] [ebp-6Ch]
    tGameObjectMain* a4; // [esp+28h] [ebp-64h]
    tGameObjectMain* v9; // [esp+2Ch] [ebp-60h]
    tGameObjectMain* a5; // [esp+30h] [ebp-5Ch]
    tGameObjectMain* v11; // [esp+34h] [ebp-58h]
    tGameObjectMain* v12; // [esp+38h] [ebp-54h]
    tGameObjectMain* v13; // [esp+3Ch] [ebp-50h]
    tGameObjectMain* v14; // [esp+40h] [ebp-4Ch]
    tGameObjectMain* v15; // [esp+44h] [ebp-48h]
    tGameObjectMain* v16; // [esp+4Ch] [ebp-40h]
    Vec3 dest; // [esp+58h] [ebp-34h] BYREF
    Vec3 a3; // [esp+68h] [ebp-24h] BYREF
    tScriptStruct_ActorGamEmploi* v19; // [esp+7Ch] [ebp-10h]
    Vec3 v20; // [esp+80h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v19 = v4;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Gam_emploi_du_temps_edt_exec_cherche_dest_Gam_emploi_du_temps_edt_exec_track_occupation = (int)AI_ul_RealResolveOneRefFunction(0x7100F7BBu);
        fct_Gam_emploi_du_temps_edt_exec_cherche_dest_Gam_emploi_du_temps_edt_deplacement_direct = (int)AI_ul_RealResolveOneRefFunction(0x7100F7B9u);
        fct_Gam_emploi_du_temps_edt_exec_cherche_dest_Gam_emploi_du_temps_edt_deplacement = (int)AI_ul_RealResolveOneRefFunction(0x7100F794u);
    }
    v16 = (tGameObjectMain*)v19->o_nearest;
    if (v16)
    {
        if ((unsigned int)v16 <= 2)
            v16 = AI_MainActor[(uint32_t)v16 - 1];
        v0 = AI_EvalFunc_WAYNearestWPOfObj_C(
            v16,
            (tWayNetwork*)v19->n_occupation_network,
            1 << v19->i_occupation_courante,
            0,
            3);
    }
    else
    {
        v0 = AI_EvalFunc_WAYNearestWPOfObj_C(
            0,
            (tWayNetwork*)v19->n_occupation_network,
            1 << v19->i_occupation_courante,
            0,
            3);
    }
    v19->o_occupation_dest = (int)v0;
    v15 = (tGameObjectMain*)v19->o_occupation_dest;
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
    if (!v14)
        return 1000;
    v19->i_occupation_cherche_nouvelle = 0;
    v13 = (tGameObjectMain*)v19->o_occupation_wp_intermediaire;
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
        v19->o_occupation_source = v19->o_occupation_wp_intermediaire;
    else
        v19->o_occupation_source = (int)AI_EvalFunc_WAYNearestWPOfObj_C(
            AI_gpst_CurrentGameObject,
            (tWayNetwork*)v19->n_occupation_network,
            -1,
            0,
            0);
    v11 = (tGameObjectMain*)v19->o_occupation_dest;
    if (v11)
    {
        if ((unsigned int)v11 <= 2)
            v11 = AI_MainActor[(uint32_t)v11 - 1];
        a5 = v11;
    }
    else
    {
        a5 = 0;
    }
    v9 = (tGameObjectMain*)v19->o_occupation_source;
    if (v9)
    {
        if ((unsigned int)v9 <= 2)
            v9 = AI_MainActor[(uint32_t)v9 - 1];
        a4 = v9;
    }
    else
    {
        a4 = 0;
    }
    v19->i_occupation_nbr_de_wp = AI_EvalFunc_WAYShortWay_C(
        AI_gpst_CurrentGameObject,
        (tWayNetwork*)v19->n_occupation_network,
        (tGameObjectMain**)v19->o_occupation_network_list,
        a4,
        a5);
    v7 = (tGameObjectMain*)*(&v19->__o_occupation_network_list_Dimension1 + v19->i_occupation_nbr_de_wp);
    if (v7)
    {
        if ((unsigned int)v7 <= 2)
            v7 = AI_MainActor[(uint32_t)v7 - 1];
        v1 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, v7);
    }
    else
    {
        v1 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, 0);
    }
    if (!v1)
    {
    LABEL_56:
        AI_EvalFunc_AITrackChange_C(
            2,
            (tAIFunction*)fct_Gam_emploi_du_temps_edt_exec_cherche_dest_Gam_emploi_du_temps_edt_deplacement);
        return 1000;
    }
    if (--v19->i_occupation_nbr_de_wp)
    {
        v19->o_occupation_source = *(&v19->__o_occupation_network_list_Dimension1 + v19->i_occupation_nbr_de_wp);
        goto LABEL_56;
    }
    v6 = (tGameObjectMain*)v19->o_occupation_dest;
    if (v6)
    {
        if ((unsigned int)v6 <= 2)
            v6 = AI_MainActor[(uint32_t)v6 - 1];
        v2 = AI_EvalFunc_OBJ_CustomBitsTest_C(v6, 256);
    }
    else
    {
        v2 = AI_EvalFunc_OBJ_CustomBitsTest_C(0, 256);
    }
    if (v2)
    {
        v5 = (tGameObjectMain*)v19->o_occupation_dest;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            AI_EvalFunc_OBJGetPos_C(v5, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v20);
        MATH_SubstractVec(&dest, &a3, &v20);
        v19->v_memo_axe = dest;
        AI_EvalFunc_AITrackChange_C(
            2,
            (tAIFunction*)fct_Gam_emploi_du_temps_edt_exec_cherche_dest_Gam_emploi_du_temps_edt_deplacement_direct);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        AI_EvalFunc_AITrackChange_C(
            2,
            (tAIFunction*)fct_Gam_emploi_du_temps_edt_exec_cherche_dest_Gam_emploi_du_temps_edt_exec_track_occupation);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int Gam_emploi_du_temps_edt_deplacement()
{
    BOOL v1; // eax
    double v2; // st7
    double v3; // st7
    int v4; // eax
    BOOL v5; // eax
    tScriptStruct_ActorGamEmploi* v6; // [esp+8h] [ebp-ACh]
    tGameObjectMain* v7; // [esp+10h] [ebp-A4h]
    tGameObjectMain* v8; // [esp+18h] [ebp-9Ch]
    tGameObjectMain* v9; // [esp+20h] [ebp-94h]
    tGameObjectMain* v10; // [esp+28h] [ebp-8Ch]
    tGameObjectMain* v11; // [esp+2Ch] [ebp-88h]
    tGameObjectMain* v12; // [esp+30h] [ebp-84h]
    tGameObjectMain* v13; // [esp+38h] [ebp-7Ch]
    tGameObjectMain* v14; // [esp+40h] [ebp-74h]
    tGameObjectMain* v15; // [esp+44h] [ebp-70h]
    tGameObjectMain* v16; // [esp+4Ch] [ebp-68h]
    tGameObjectMain* v17; // [esp+54h] [ebp-60h]
    int v18; // [esp+7Ch] [ebp-38h]
    Vec3 a3; // [esp+80h] [ebp-34h] BYREF
    int v20; // [esp+8Ch] [ebp-28h]
    float v21; // [esp+9Ch] [ebp-18h]
    tScriptStruct_ActorGamEmploi* v23; // [esp+A4h] [ebp-10h]
    float v24; // [esp+A8h] [ebp-Ch]
    BOOL v25; // [esp+B0h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v23 = v6;
    v20 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Gam_emploi_du_temps_edt_deplacement_Gam_emploi_du_temps_edt_exec_recherche_chemin = (int)AI_ul_RealResolveOneRefFunction(0x7100F791u);
    }
    if (AI_EvalFunc_ACTCustomBitTest_C(AI_gpst_CurrentGameObject, 0x40u))
    {
        if (!AI_EvalFunc_ACTActionFinished_C_CURRENT())
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
    }
    v20 = 0;
    if (!v23->i_occupation_cherche_nouvelle)
    {
        v23->o_occupation_wp_intermediaire = *(&v23->__o_occupation_network_list_Dimension1 + v23->i_occupation_nbr_de_wp);
        v17 = (tGameObjectMain*)v23->o_occupation_wp_intermediaire;
        if (v17)
        {
            if ((unsigned int)v17 <= 2)
                v17 = AI_MainActor[(uint32_t)v17 - 1];
            AI_EvalFunc_OBJGetPos_C(v17, &v23->v_way_destpos);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v23->v_way_destpos);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v23->v_way_currentpos);
        v16 = (tGameObjectMain*)v23->o_occupation_wp_intermediaire;
        if (v16)
        {
            if ((unsigned int)v16 <= 2)
                v16 = AI_MainActor[(uint32_t)v16 - 1];
            v1 = AI_EvalFunc_OBJ_CapaTest_C(v16, 0x4000);
        }
        else
        {
            v1 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x4000);
        }
        v23->i_way_etage_dest = v1;
        v23->i_way_moving = 1;
        v23->f_way_blendrotate = 0.125;
        v18 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_deplacement_Gam_emploi_du_temps_edt_exec_recherche_chemin);
        if (v18 != 1000)
            return v18;
        if (!v23->i_way_moving)
            v20 = 1;
    }
    v15 = (tGameObjectMain*)v23->o_occupation_dest;
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
    if (v14 && v23->i_way_moving)
    {
        v23->f_time_keep_anim = TIM_gf_MainClock - TIM_gf_LockTime;
        v13 = (tGameObjectMain*)v23->o_occupation_network_list[0];
        if (v13)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            v2 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v13);
        }
        else
        {
            v2 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
        }
        v21 = (float)v2;
        if (v21 < 16.0)
            goto LABEL_53;
        v12 = (tGameObjectMain*)v23->o_old_capa_wp;
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
        if (!v11)
            goto LABEL_54;
        v10 = (tGameObjectMain*)v23->o_old_capa_wp;
        if (v10)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            v3 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v10);
        }
        else
        {
            v3 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
        }
        v24 = (float)v3;
        if (v24 < 9.0)
            LABEL_53:
        AI_EvalFunc_ACTActionSet_C_CURRENT(1);
        else
            LABEL_54:
        AI_EvalFunc_ACTActionSet_C_CURRENT(v23->i_action_trot);
        AI_EvalFunc_ANIFrequencySet_C(AI_gpst_CurrentGameObject, 0, v23->i_fuite_frequency);
        if (!AI_EvalFunc_ACTCustomBitTest_C(AI_gpst_CurrentGameObject, 1u))
        {
            if (v23->i_old_capa)
            {
                v9 = (tGameObjectMain*)v23->o_old_capa_wp;
                if (v9)
                {
                    if ((unsigned int)v9 <= 2)
                        v9 = AI_MainActor[(uint32_t)v9 - 1];
                    AI_EvalFunc_OBJ_CapaSet_C(v9, v23->i_old_capa, ~v23->i_old_capa); // TODO FIXME NEM BIZTOS H JO
                }
                else
                {
                    AI_EvalFunc_OBJ_CapaSet_C(0, v23->i_old_capa, ~v23->i_old_capa);
                }
                v23->i_old_capa = 0;
            }
            a3.x = 0.0;
            a3.y = 0.0;
            a3.z = 1.0;
            AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v23->v_way_sight, &a3);
            v8 = (tGameObjectMain*)v23->o_occupation_dest;
            if (v8)
            {
                if ((unsigned int)v8 <= 2)
                    v8 = AI_MainActor[(uint32_t)v8 - 1];
                v4 = AI_EvalFunc_OBJ_CapaGet_C(v8);
            }
            else
            {
                v4 = AI_EvalFunc_OBJ_CapaGet_C(0);
            }
            if (!v4)
                v23->i_occupation_cherche_nouvelle = 1;
            v7 = (tGameObjectMain*)v23->o_occupation_wp_intermediaire;
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                v5 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, v7);
            }
            else
            {
                v5 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, 0);
            }
            v25 = v5;
            if (v5)
                v23->i_occupation_cherche_nouvelle = 1;
        }
    }
    else
    {
        if (!v20 || AI_EvalFunc_TIMEElapsed_C(v23->f_time_keep_anim, 0.1f))
            AI_EvalFunc_ACTActionSet_C_CURRENT(0);
        v23->i_way_force_recompute = 1;
        v23->i_occupation_cherche_nouvelle = 1;
    }
    return 1000;
}

int Gam_emploi_du_temps_edt_deplacement_direct()
{
    double v0; // st7
    float a4; // [esp+0h] [ebp-A8h]
    tScriptStruct_ActorGamEmploi* v3; // [esp+8h] [ebp-A0h]
    tGameObjectMain* v4; // [esp+10h] [ebp-98h]
    tGameObjectMain* v5; // [esp+18h] [ebp-90h]
    tGameObjectMain* v6; // [esp+24h] [ebp-84h]
    float v7; // [esp+28h] [ebp-80h]
    float v8; // [esp+2Ch] [ebp-7Ch]
    Vec3 v9; // [esp+30h] [ebp-78h] BYREF
    Vec3 v10; // [esp+3Ch] [ebp-6Ch] BYREF
    Vec3 v11; // [esp+48h] [ebp-60h] BYREF
    float v12; // [esp+54h] [ebp-54h]
    int v13; // [esp+58h] [ebp-50h]
    Vec3 a5; // [esp+5Ch] [ebp-4Ch] BYREF
    Vec3 dest; // [esp+68h] [ebp-40h] BYREF
    Vec3 v16; // [esp+74h] [ebp-34h] BYREF
    Vec3 a1; // [esp+80h] [ebp-28h] BYREF
    tScriptStruct_ActorGamEmploi* v18; // [esp+8Ch] [ebp-1Ch]
    Vec3 a3; // [esp+90h] [ebp-18h] BYREF
    Vec3 a2; // [esp+9Ch] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v18 = v3;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Gam_emploi_du_temps_edt_deplacement_direct_Gam_emploi_du_temps_edt_exec_track_occupation = (int)AI_ul_RealResolveOneRefFunction(0x7100F7BBu);
    }
    AI_EvalFunc_ACTActionSet_C_CURRENT(1);
    AI_EvalFunc_ANIFrequencySet_C(AI_gpst_CurrentGameObject, 0, v18->i_fuite_frequency);
    v6 = (tGameObjectMain*)v18->o_occupation_dest;
    if (v6)
    {
        if ((unsigned int)v6 <= 2)
            v6 = AI_MainActor[(uint32_t)v6 - 1];
        AI_EvalFunc_OBJGetPos_C(v6, &v11);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v11);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
    MATH_SubstractVec(&dest, &v11, &a1);
    v8 = dest.z * v18->v_memo_axe.z + dest.y * v18->v_memo_axe.y + dest.x * v18->v_memo_axe.x;
    if (v8 <= 0.0)
        goto LABEL_28;
    v5 = (tGameObjectMain*)v18->o_occupation_dest;
    if (v5)
    {
        if ((unsigned int)v5 <= 2)
            v5 = AI_MainActor[(uint32_t)v5 - 1];
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v5);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v7 = (float)v0;
    if (v7 <= 0.0099999998)
    {
    LABEL_28:
        v13 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_deplacement_direct_Gam_emploi_du_temps_edt_exec_track_occupation);
        if (v13 != 1000)
            return v13;
    }
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
    v4 = (tGameObjectMain*)v18->o_occupation_dest;
    if (v4)
    {
        if ((unsigned int)v4 <= 2)
            v4 = AI_MainActor[(uint32_t)v4 - 1];
        AI_EvalFunc_OBJGetPos_C(v4, &v10);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v10);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v16);
    MATH_SubstractVec(&a3, &v10, &v16);
    v12 = TIM_gf_dt;
    a4 = v18->f_way_blendrotate * (TIM_gf_dt * 30.0f);
    AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
    v9.x = 0.0;
    v9.y = 0.0;
    v9.z = 1.0;
    AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a5, &v9);
    return 1000;
}

int Gam_emploi_du_temps_edt_occupation_regarde()
{
    float a4; // [esp+0h] [ebp-70h]
    tScriptStruct_ActorGamEmploi* v2; // [esp+8h] [ebp-68h]
    tGameObjectMain* v3; // [esp+10h] [ebp-60h]
    tGameObjectMain* v4; // [esp+18h] [ebp-58h]
    Vec3 a3; // [esp+1Ch] [ebp-54h] BYREF
    Vec3 a2; // [esp+28h] [ebp-48h] BYREF
    Vec3 v7; // [esp+34h] [ebp-3Ch] BYREF
    Vec3 v8; // [esp+40h] [ebp-30h] BYREF
    float v9; // [esp+4Ch] [ebp-24h]
    int v10; // [esp+50h] [ebp-20h]
    Vec3 v11; // [esp+54h] [ebp-1Ch] BYREF
    tScriptStruct_ActorGamEmploi* v12; // [esp+60h] [ebp-10h]
    Vec3 a5; // [esp+64h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v12 = v2;
    v4 = (tGameObjectMain*)v2->o_old_capa_wp;
    if (v4)
    {
        if ((unsigned int)v4 <= 2)
            v4 = AI_MainActor[(uint32_t)v4 - 1];
        AI_EvalFunc_OBJ_CapaSet_C(v4, 0, -1);
    }
    else
    {
        AI_EvalFunc_OBJ_CapaSet_C(0, 0, -1);
    }
    v8.x = 0.0;
    v8.y = 0.0;
    v8.z = 0.0;
    AI_EvalFunc_DYNTractionSet_C_CURRENT(&v8);
    v11.x = 0.0;
    v11.y = 0.0;
    v11.z = 0.0;
    v10 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v11);
    AI_EvalFunc_ACTActionSet_C_CURRENT(13);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
    v3 = (tGameObjectMain*)v12->o_old_capa_wp;
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        AI_EvalFunc_OBJ_SightGet_C(v3, &a3);
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C(0, &a3);
    }
    v9 = TIM_gf_dt;
    a4 = 3.0f * TIM_gf_dt;
    AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
    v7.x = 0.0;
    v7.y = 0.0;
    v7.z = 1.0;
    AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a5, &v7);
    return 1000;
}

int Gam_emploi_du_temps_edt_occupation_mange()
{
    int result; // eax
    float a4; // [esp+0h] [ebp-78h]
    tScriptStruct_ActorGamEmploi* v2; // [esp+Ch] [ebp-6Ch]
    tGameObjectMain* v3; // [esp+14h] [ebp-64h]
    tGameObjectMain* v4; // [esp+1Ch] [ebp-5Ch]
    Vec3 a2; // [esp+20h] [ebp-58h] BYREF
    Vec3 a5; // [esp+30h] [ebp-48h] BYREF
    Vec3 v8; // [esp+3Ch] [ebp-3Ch] BYREF
    Vec3 a3; // [esp+48h] [ebp-30h] BYREF
    Vec3 v10; // [esp+54h] [ebp-24h] BYREF
    int v11; // [esp+60h] [ebp-18h]
    Vec3 v12; // [esp+64h] [ebp-14h] BYREF
    tScriptStruct_ActorGamEmploi* v13; // [esp+70h] [ebp-8h]
    float v14; // [esp+74h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v13 = v2;
    if (*AI_gppst_CurrentJumpNode != (tAINode*)1)
    {
        v4 = (tGameObjectMain*)v13->o_old_capa_wp;
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            AI_EvalFunc_OBJ_CapaSet_C(v4, 0, -1);
        }
        else
        {
            AI_EvalFunc_OBJ_CapaSet_C(0, 0, -1);
        }
        v8.x = 0.0;
        v8.y = 0.0;
        v8.z = 0.0;
        AI_EvalFunc_DYNTractionSet_C_CURRENT(&v8);
        v12.x = 0.0;
        v12.y = 0.0;
        v12.z = 0.0;
        v11 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v12);
        AI_EvalFunc_ACTActionSet_C_CURRENT(17);
    }
    if (AI_EvalFunc_ACTActionFinished_C_CURRENT())
    {
        v13->i_force_next_edt = 1;
        result = 1000;
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
        v3 = (tGameObjectMain*)v13->o_old_capa_wp;
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v3, &a3);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &a3);
        }
        v14 = TIM_gf_dt;
        a4 = 3.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
        v10.x = 0.0;
        v10.y = 0.0;
        v10.z = 1.0;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a5, &v10);
        *AI_gppst_CurrentJumpNode = (tAINode*)1;
        result = 0;
    }
    return result;
}

int Gam_emploi_du_temps_edt_after_blend()
{
    int result; // eax
    double v1; // st7
    tGameObjectMain* v2; // eax
    tScriptStruct_ActorGamEmploi* v3; // [esp+0h] [ebp-68h]
    tGameObjectMain* v4; // [esp+8h] [ebp-60h]
    Vec3 dest; // [esp+Ch] [ebp-5Ch] BYREF
    Vec3 v6; // [esp+18h] [ebp-50h] BYREF
    float v7; // [esp+28h] [ebp-40h]
    Vec3 v8; // [esp+2Ch] [ebp-3Ch] BYREF
    Vec3 a3[2]; // [esp+38h] [ebp-30h] BYREF
    Vec3 a1; // [esp+50h] [ebp-18h] BYREF
    tScriptStruct_ActorGamEmploi* v11; // [esp+5Ch] [ebp-Ch]
    int v12; // [esp+60h] [ebp-8h]
    float v13; // [esp+64h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v11 = v3;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Gam_emploi_du_temps_edt_after_blend__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        fct_Gam_emploi_du_temps_edt_after_blend_Gam_emploi_du_temps_edt_exec_suivi_regard = (int)AI_ul_RealResolveOneRefFunction(0x26000590u);
    }
    if (v11->i_zoom)
    {
        v11->i_zoom = 0;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        v4 = (tGameObjectMain*)Gam_emploi_du_temps_edt_after_blend__PJ_Sally_gao;
        if (Gam_emploi_du_temps_edt_after_blend__PJ_Sally_gao)
        {
            if ((unsigned int)Gam_emploi_du_temps_edt_after_blend__PJ_Sally_gao <= 2)
                v4 = AI_MainActor[Gam_emploi_du_temps_edt_after_blend__PJ_Sally_gao - 1];
            v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v4);
        }
        else
        {
            v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
        }
        v7 = (float)v1;
        if (v7 >= 36.0f
            || (AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a1),
                AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Gam_emploi_du_temps_edt_after_blend__PJ_Sally_gao, a3),
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v6),
                MATH_SubstractVec(&dest, a3, &v6),
                AI_EvalFunc_MATHVecNormalize_C(&dest, &v8),
                v13 = v8.z * a1.z + v8.y * a1.y + a1.x * v8.x,
                v13 <= 0.173648f))
        {
            v11->des_o_acteur_suividesyeux = 0;
        }
        else
        {
            v2 = AI_EvalFunc_ANICanalGaoGet_C((tGameObjectMain*)Gam_emploi_du_temps_edt_after_blend__PJ_Sally_gao, 0);
            v11->des_o_acteur_suividesyeux = (int)v2;
        }
        v11->des_f_RT_vitesse_de_rotation = 0.15000001f;
        v11->des_v_hauteur_cible.x = 0.0;
        v11->des_v_hauteur_cible.y = 0.0;
        v11->des_v_hauteur_cible.z = 0.0;
        v11->des_i_RT_flags = 0x80000000;
        v12 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_after_blend_Gam_emploi_du_temps_edt_exec_suivi_regard);
        if (v12 == 1000)
            result = 1000;
        else
            result = v12;
    }
    return result;
}

int Gam_emploi_du_temps_edt_occupation_loup_nargue()
{
    float a4; // [esp+0h] [ebp-208h]
    tScriptStruct_ActorGamEmploi* v2; // [esp+Ch] [ebp-1FCh]
    tGameObjectMain* v3; // [esp+1Ch] [ebp-1ECh]
    tGameObjectMain* v4; // [esp+24h] [ebp-1E4h]
    Vec3 v5; // [esp+2Ch] [ebp-1DCh] BYREF
    tGameObjectMain* v6; // [esp+38h] [ebp-1D0h]
    tGameObjectMain* v7; // [esp+3Ch] [ebp-1CCh]
    int v8; // [esp+40h] [ebp-1C8h]
    tGameObjectMain* v9; // [esp+44h] [ebp-1C4h]
    tGameObjectMain* v10; // [esp+48h] [ebp-1C0h]
    tGameObjectMain* v11; // [esp+4Ch] [ebp-1BCh]
    tGameObjectMain* v12; // [esp+50h] [ebp-1B8h]
    tGameObjectMain* v13; // [esp+54h] [ebp-1B4h]
    tGameObjectMain* v14; // [esp+58h] [ebp-1B0h]
    int v15; // [esp+5Ch] [ebp-1ACh]
    Vec3* v16; // [esp+64h] [ebp-1A4h]
    Vec3* v17; // [esp+68h] [ebp-1A0h]
    tGameObjectMain* v18; // [esp+6Ch] [ebp-19Ch]
    tGameObjectMain* v19; // [esp+70h] [ebp-198h]
    Vec3* v20; // [esp+74h] [ebp-194h]
    Vec3* v21; // [esp+78h] [ebp-190h]
    tGameObjectMain* a1; // [esp+7Ch] [ebp-18Ch]
    tGameObjectMain* v23; // [esp+80h] [ebp-188h]
    tGameObjectMain* v24; // [esp+84h] [ebp-184h]
    tGameObjectMain* v25; // [esp+88h] [ebp-180h]
    tGameObjectMain* v26; // [esp+8Ch] [ebp-17Ch]
    tGameObjectMain* v27; // [esp+90h] [ebp-178h]
    float v28; // [esp+94h] [ebp-174h]
    int v29; // [esp+98h] [ebp-170h]
    float v30; // [esp+ACh] [ebp-15Ch]
    int v31; // [esp+B0h] [ebp-158h]
    Vec3 v32; // [esp+B8h] [ebp-150h] BYREF
    int v33; // [esp+C4h] [ebp-144h]
    tGameObjectMain* v34[21]; // [esp+C8h] [ebp-140h] BYREF
    Vec3 v35; // [esp+11Ch] [ebp-ECh] BYREF
    int v37; // [esp+138h] [ebp-D0h]
    Vec3 v38; // [esp+148h] [ebp-C0h] BYREF
    int v39; // [esp+160h] [ebp-A8h]
    Vec3 v40; // [esp+164h] [ebp-A4h] BYREF
    Vec3 v41; // [esp+174h] [ebp-94h] BYREF
    Vec3 a2; // [esp+184h] [ebp-84h] BYREF
    int v43; // [esp+194h] [ebp-74h]
    Vec3 dest; // [esp+198h] [ebp-70h] BYREF
    float v45; // [esp+1A4h] [ebp-64h]
    tGameObjectMain* v46; // [esp+1A8h] [ebp-60h]
    int v47; // [esp+1B8h] [ebp-50h]
    int v48; // [esp+1BCh] [ebp-4Ch]
    int i; // [esp+1C0h] [ebp-48h]
    Vec3 v51; // [esp+1C8h] [ebp-40h] BYREF
    tScriptStruct_ActorGamEmploi* v52; // [esp+1D4h] [ebp-34h]
    int v53; // [esp+1D8h] [ebp-30h]
    Vec3 v54; // [esp+1DCh] [ebp-2Ch] BYREF
    BOOL v55; // [esp+1E8h] [ebp-20h]
    Vec3 a3; // [esp+1F0h] [ebp-18h] BYREF
    Vec3 v57; // [esp+1FCh] [ebp-Ch] BYREF

    v35.x = 1.0;
    v35.y = 0.0;
    v35.z = 0.0;
    v38.x = 1.0;
    v38.y = 0.0;
    v38.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v52 = v2;
    v33 = 0;
    v39 = 0;
    v30 = 0.0;
    v48 = 0;
    v31 = 0;
    v53 = 0;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Gam_emploi_du_temps_edt_occupation_loup_nargue_Gam_emploi_du_temps_edt_occupation_loup_poursuite = (int)AI_ul_RealResolveOneRefFunction(0x2600004Au);
        fct_Gam_emploi_du_temps_edt_occupation_loup_nargue_Gam_emploi_du_temps_edt_occupation_loup_safe = (int)AI_ul_RealResolveOneRefFunction(0x26000056u);
        fct_Gam_emploi_du_temps_edt_occupation_loup_nargue_Gam_emploi_du_temps_edt_exec_recherche_chemin = (int)AI_ul_RealResolveOneRefFunction(0x7100F791u);
    }
    if (*AI_gppst_CurrentJumpNode != (tAINode*)1)
    {
        if (!AI_EvalFunc_OBJ_CustomBitsTest_C(AI_gpst_CurrentGameObject, 0x400))
            goto LABEL_11;
        AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(0, 0x400);
        AI_EvalFunc_ACTActionSet_C_CURRENT(25);
    }
    if (!AI_EvalFunc_ACTActionFinished_C_CURRENT())
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)1;
        return 0;
    }
LABEL_11:
    v31 = 0;
    v46 = AI_gpst_CurrentGameObject;
    v27 = (tGameObjectMain*)v52->o_le_loup;
    if (v27)
    {
        if ((unsigned int)v27 <= 2)
            v27 = AI_MainActor[(uint32_t)v27 - 1];
        v26 = v27;
    }
    else
    {
        v26 = 0;
    }
    v25 = v46;
    if (v46)
    {
        if ((unsigned int)v25 <= 2)
            v25 = AI_MainActor[(uint32_t)v25 - 1];
        v24 = v25;
    }
    else
    {
        v24 = 0;
    }
    if (v26 == v24)
    {
        v52->o_perso_a_attraper = 0;
        v52->f_time_changetarget = TIM_gf_MainClock - TIM_gf_LockTime;
        v52->f_time_loup_init = v52->f_time_changetarget;
        v52->i_way_force_recompute = 1;
        v29 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_occupation_loup_nargue_Gam_emploi_du_temps_edt_occupation_loup_poursuite);
        if (v29 != 1000)
            return v29;
    }
    v23 = (tGameObjectMain*)v52->o_le_loup;
    if (v23)
    {
        if ((unsigned int)v23 <= 2)
            v23 = AI_MainActor[(uint32_t)v23 - 1];
        a1 = v23;
    }
    else
    {
        a1 = 0;
    }
    AI_EvalFunc_DYNSpeedGetVector_C(a1, &v41);
    v20 = &v35;
    v21 = &v41;
    v35 = v41;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v54);
    v19 = (tGameObjectMain*)v52->o_le_loup;
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
    AI_EvalFunc_OBJGetPos_C(v18, &v32);
    MATH_SubstractVec(&dest, &v54, &v32);
    v16 = &v38;
    v17 = &dest;
    v38 = dest;
    v30 = MATH_VecLen(&v38);
    if ((v52->f_distance_trop_proche <= (double)v30 || (v37 = AI_EvalFunc_ACTActionGet_C_CURRENT(), v37 == 24))
        && ((v43 = AI_EvalFunc_ACTActionGet_C_CURRENT(), v43 != 24) || v52->f_distance_trop_proche + 1.0 <= v30))
    {
        if (v52->f_distance_trop_loin >= (double)v30)
        {
            v52->i_way_moving = 0;
            v52->i_way_force_recompute = 1;
            v53 = 23;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
            a3.x = -v38.x;
            a3.y = -v38.y;
            a3.z = -v38.z;
            v45 = TIM_gf_dt;
            a4 = 10.0f * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &v52->v_way_sight);
        }
        else
        {
            v3 = (tGameObjectMain*)v52->o_le_loup;
            if (v3)
            {
                if ((unsigned int)v3 <= 2)
                    v3 = AI_MainActor[(uint32_t)v3 - 1];
                AI_EvalFunc_OBJGetPos_C(v3, &v52->v_way_destpos);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v52->v_way_destpos);
            }
            v52->i_way_moving = 1;
            v52->f_way_blendrotate = 0.15000001f;
            v53 = 26;
        }
    }
    else
    {
        v15 = 0;
        v33 = AI_EvalFunc_ARRSortByFlags_C((tGameObjectMain**)v52->o_safe_list, v52->i_safe_nbr, -1, 0x8000, 3);
        if (v33)
        {
            v39 = v33;
            v14 = (tGameObjectMain*)v52->o_safe_backup;
            if (v14)
            {
                if ((unsigned int)v14 <= 2)
                    v14 = AI_MainActor[(uint32_t)v14 - 1];
                v13 = v14;
            }
            else
            {
                v13 = 0;
            }
            if (v13)
            {
                v39 = 0;
                for (i = 0; i < v33; ++i)
                {
                    v12 = (tGameObjectMain*)v52->o_safe_list[i];
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
                    v10 = (tGameObjectMain*)v52->o_safe_backup;
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
                    if (v11 != v9)
                        v34[v39++] = (tGameObjectMain*)v52->o_safe_list[i];
                }
            }
            else
            {
                v8 = 0;
                AI_EvalFunc_ARRObjCopy_C(v34, v52->o_safe_list, v33);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v57);
            v48 = AI_EvalFunc_ARRObjBestSight_C(v34, v39, &v57, &v38, 0.0);
            v52->o_safe = (int)v34[v48];
            v7 = (tGameObjectMain*)v52->o_safe;
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
            AI_EvalFunc_OBJGetPos_C(v6, &v52->v_way_destpos);
            v52->i_way_moving = 1;
            v52->f_way_blendrotate = 0.2f;
            v53 = 24;
            AI_EvalFunc_ANIFrequencySet_C(AI_gpst_CurrentGameObject, 0, v52->i_fuite_frequency);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v40);
            MATH_SubstractVec(&v5, &v40, &v52->v_way_destpos);
            v28 = MATH_VecLen(&v5);
            if (v28 < 0.80000001)
            {
                AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(512, 0);
                v4 = (tGameObjectMain*)v52->o_safe;
                if (v4)
                {
                    if ((unsigned int)v4 <= 2)
                        v4 = AI_MainActor[(uint32_t)v4 - 1];
                    AI_EvalFunc_OBJ_CapaSet_C(v4, 0x8000, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_CapaSet_C(0, 0x8000, 0);
                }
                v52->f_time_safe = TIM_gf_MainClock - TIM_gf_LockTime;
                AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_occupation_loup_nargue_Gam_emploi_du_temps_edt_occupation_loup_safe);
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                return 1000;
            }
        }
    }
    if (v52->i_way_moving)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v52->v_way_currentpos);
        v52->i_way_etage_dest = 0;
        v47 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_occupation_loup_nargue_Gam_emploi_du_temps_edt_exec_recherche_chemin);
        if (v47 != 1000)
            return v47;
        if (!v52->i_way_moving)
        {
            v31 = 1;
            v52->i_way_force_recompute = 1;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v52->v_way_sight);
            v53 = 23;
        }
    }
    if (v31 && !(v55 = AI_EvalFunc_TIMEElapsed_C(v52->f_time_keep_anim, 0.1f)))
    {
        v52->f_time_keep_anim = v52->f_time_keep_anim;
    }
    else
    {
        AI_EvalFunc_ACTActionSet_C_CURRENT(v53);
        v52->f_time_keep_anim = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    AI_EvalFunc_ANIFrequencySet_C(AI_gpst_CurrentGameObject, 0, v52->i_fuite_frequency);
    v51.x = 0.0;
    v51.y = 0.0;
    v51.z = 1.0;
    AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v52->v_way_sight, &v51);
    return 1000;
}

int Gam_emploi_du_temps_edt_exec_get_free_player()
{
    BOOL v0; // eax
    tScriptStruct_ActorGamEmploi* v2; // [esp+0h] [ebp-3Ch]
    tGameObjectMain* v3; // [esp+8h] [ebp-34h]
    tAI* v4; // [esp+Ch] [ebp-30h]
    tAI* v5; // [esp+10h] [ebp-2Ch]
    tGameObjectMain* v6; // [esp+14h] [ebp-28h]
    tGameObjectMain* v7; // [esp+18h] [ebp-24h]
    tAI* v8; // [esp+1Ch] [ebp-20h]
    tAI* v9; // [esp+20h] [ebp-1Ch]
    tGameObjectMain* v10; // [esp+24h] [ebp-18h]
    tGameObjectMain* v11; // [esp+28h] [ebp-14h]
    int i; // [esp+30h] [ebp-Ch]
    int v13; // [esp+38h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v9 = (tAI*)v2->o_manager;
    if (v9 == WOR_gst_Universe.univAIInstance)
    {
        v8 = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        v11 = (tGameObjectMain*)v2->o_manager;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v11 = AI_MainActor[(uint32_t)v9 - 1];
            v10 = v11;
        }
        else
        {
            v10 = 0;
        }
        v8 = v10->extended->AIInstance;
    }
    v13 = *((uint32_t*)v8->instanceVarDataCurrent + 24);
    for (i = 0; i < v13; ++i)
    {
        v5 = (tAI*)v2->o_manager;
        if (v5 == WOR_gst_Universe.univAIInstance)
        {
            v4 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v7 = (tGameObjectMain*)v2->o_manager;
            if (v5)
            {
                if ((unsigned int)v5 <= 2)
                    v7 = AI_MainActor[(uint32_t)v5 - 1];
                v6 = v7;
            }
            else
            {
                v6 = 0;
            }
            v4 = v6->extended->AIInstance;
        }
        v2->o_perso_a_attraper = *((uint32_t*)v4->instanceVarDataCurrent + i + 4);
        v3 = (tGameObjectMain*)v2->o_perso_a_attraper;
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            v0 = AI_EvalFunc_OBJ_CustomBitsTest_C(v3, 0x200);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_CustomBitsTest_C(0, 0x200);
        }
        if (!v0)
            break;
        v2->o_perso_a_attraper = 0;
    }
    return 1000;
}

int Gam_emploi_du_temps_edt_dialogue_check()
{
    double v1; // st7
    char* v2; // eax
    int* v3; // eax
    char* v4; // eax
    int* v5; // eax
    int* v6; // eax
    char* v7; // eax
    char* v8; // eax
    char* v9; // eax
    char* v10; // eax
    int v11; // [esp-4h] [ebp-1A8h]
    int v12; // [esp+0h] [ebp-1A4h]
    char* v13; // [esp+0h] [ebp-1A4h]
    char* v14; // [esp+0h] [ebp-1A4h]
    char* v15; // [esp+0h] [ebp-1A4h]
    char* v16; // [esp+0h] [ebp-1A4h]
    int v17; // [esp+4h] [ebp-1A0h]
    float v18; // [esp+Ch] [ebp-198h]
    tScriptStruct_ActorGamEmploi* v19; // [esp+10h] [ebp-194h]
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
    tScriptStruct_ActorGamEmploi* v56; // [esp+170h] [ebp-34h]
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
        v19 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
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
        Gam_emploi_du_temps_edt_dialogue_check__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &Gam_emploi_du_temps_edt_dialogue_check__PJ_Sally_gao_current_pm_mode,
            (void*)Gam_emploi_du_temps_edt_dialogue_check__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(
            &Gam_emploi_du_temps_edt_dialogue_check__PJ_Sally_gao_current_pm_priority,
            (void*)Gam_emploi_du_temps_edt_dialogue_check__PJ_Sally_gao,
            576);
        AI2C_ResolveBuffer(
            &Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_i_sally_priority,
            (void*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao,
            396);
        AI2C_ResolveBuffer(
            &Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_o_sally_contextuel,
            (void*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao,
            388);
        AI2C_ResolveBuffer(
            &Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_i_sally_action,
            (void*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao,
            412);
        AI2C_ResolveBuffer(
            &Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_v_sally_offset,
            (void*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao,
            400);
        AI2C_ResolveBuffer(
            &Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_i_force_interface,
            (void*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao,
            3292);
        Gam_emploi_du_temps_edt_dialogue_check_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Gam_emploi_du_temps_edt_dialogue_check_univVars_Flag_speciaux_sauve,
            (void*)Gam_emploi_du_temps_edt_dialogue_check_univVars,
            4488);
        Gam_emploi_du_temps_edt_dialogue_check_TXT_Sally_gao = AI_ul_RealResolveOneRefObject(0x7200064Eu);
        AI2C_ResolveBuffer(
            &Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_ai_dialog_consequences,
            (void*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao,
            1432);
        AI2C_ResolveBuffer(
            &Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_o_dialogue_actor,
            (void*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao,
            1100);
        AI2C_ResolveBuffer(
            &Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_at_dialog_questions,
            (void*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao,
            1104);
        AI2C_ResolveBuffer(
            &Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_at_dialog_reponses,
            (void*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao,
            1268);
        AI2C_ResolveBuffer(
            &Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_ai_dialog_grisage,
            (void*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao,
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
    v28 = (tGameObjectMain*)Gam_emploi_du_temps_edt_dialogue_check__PJ_Sally_gao;
    if (Gam_emploi_du_temps_edt_dialogue_check__PJ_Sally_gao)
    {
        if ((unsigned int)Gam_emploi_du_temps_edt_dialogue_check__PJ_Sally_gao <= 2)
            v28 = AI_MainActor[Gam_emploi_du_temps_edt_dialogue_check__PJ_Sally_gao - 1];
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v28);
    }
    else
    {
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v55 = (float)v1;
    if (v56->des_f_distance_talk * v56->des_f_distance_talk > v55
        && !AI_EvalFunc_OBJ_CustomBitsTest_C(
            (tGameObjectMain*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao,
            2048))
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Gam_emploi_du_temps_edt_dialogue_check__PJ_Sally_gao, &a3);
        MATH_SubstractVec(&dest, &a2, &a3);
        v40 = dest;
        AI_EvalFunc_MATHVecSetHorzNormalize_C(&v40);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v59);
        v35 = v40.z * v59.z + v40.y * v59.y + v59.x * v40.x;
        if (v56->des_f_angle_talk <= (double)v35)
        {
            AI_EvalFunc_MATHVecNormalize_C(&v40, &v47);
            v50 = v47;
            AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Gam_emploi_du_temps_edt_dialogue_check__PJ_Sally_gao, &v58);
            v36 = v50.z * v58.z + v50.y * v58.y + v58.x * v50.x;
            v38 = v36 * 5.0f;
            v27 = v38 + -10.0 + MATH_gd_magicDoubleForRound;
            v60 = (*(uint32_t *)&v27);
            v46 = (unsigned __int16)*(uint32_t*)(Gam_emploi_du_temps_edt_dialogue_check__PJ_Sally_gao + 4);
            if ((v46 & 1) != 0)
                v57 = !*(uint32_t*)Gam_emploi_du_temps_edt_dialogue_check__PJ_Sally_gao_current_pm_mode
                || *(int*)Gam_emploi_du_temps_edt_dialogue_check__PJ_Sally_gao_current_pm_priority < 20;
            else
                v57 = 0;
            if (v57)
            {
                v18 = (float)v60;
                if (v18 > (double)*(float*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_i_sally_priority)
                {
                    *(uint32_t*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_o_sally_contextuel = (uint32_t)AI_gpst_CurrentGameObject;
                    *(float*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_i_sally_priority = (float)v60;
                    *(uint32_t*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_i_sally_action = 5;
                    *(uint32_t*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_v_sally_offset = 0;
                    *(uint32_t*)(Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_v_sally_offset + 4) = 0;
                    *(uint32_t*)(Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_v_sally_offset + 8) = 0x3F800000;
                    *(uint32_t*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_i_force_interface = 1;
                }
            }
            v56->i_talk_propose = v57;
        }
    }
    v43 = 0;
    v44 = 0;
    if (v56->m_dialogue.msgID == 10 && (v41 = AI_EvalFunc_TIMEElapsed_C(v56->f_time_fin_talk, 0.5)))
    {
        v51 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Gam_emploi_du_temps_edt_dialogue_check_univVars_Flag_speciaux_sauve + 4),
            2083);
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
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)Gam_emploi_du_temps_edt_dialogue_check_TXT_Sally_gao, &Src);
            v56->m_dialogue.msgID = 0;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v43 = 1;
        if (v56->Valide_Effet_1_Question)
            AI_EvalFunc_OBJ_CapaSet_C((tGameObjectMain*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao, 0x80, 0);
        else
            AI_EvalFunc_OBJ_CapaSet_C((tGameObjectMain*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao, 0, 0x80);
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
    v45 = (*(uint8_t *)&v25); // TODO IFXME ?????????
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
                    AI_EvalFunc_OBJ_CustomBitsSet_C(
                        (tGameObjectMain*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao,
                        1024,
                        0);
                    continue;
                }
                if (*(int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4) <= 10000)
                {
                    if (*(int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4) > 0)
                    {
                        v21 = (int*)(Gam_emploi_du_temps_edt_dialogue_check_univVars_Flag_speciaux_sauve + 4);
                        v5 = (int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4);
                        AI_EvalFunc_ARRBitSet_C(v21, *v5);
                    }
                }
                else if (*(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4) - 10000 > 0)
                {
                    v22 = (int*)(Gam_emploi_du_temps_edt_dialogue_check_univVars_Flag_speciaux_sauve + 4);
                    v4 = AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4);
                    AI_EvalFunc_ARRBitClear_C(v22, *(uint32_t*)v4 - 10000);
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
                        a1 = (int*)(Gam_emploi_du_temps_edt_dialogue_check_univVars_Flag_speciaux_sauve + 4);
                        v3 = (int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_Dimension1, i, a4);
                        AI_EvalFunc_ARRBitSet_C(a1, *v3);
                    }
                }
                else if (*(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_Dimension1, i, a4) - 10000 > 0)
                {
                    v24 = (int*)(Gam_emploi_du_temps_edt_dialogue_check_univVars_Flag_speciaux_sauve + 4);
                    v2 = AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_Dimension1, i, a4);
                    AI_EvalFunc_ARRBitClear_C(v24, *(uint32_t*)v2 - 10000);
                }
            }
        }
    }
    v56->f_time_fin_talk = TIM_gf_MainClock - TIM_gf_LockTime;
    for (i = 0; i < 5; ++i)
        *(uint32_t*)(Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_ai_dialog_consequences + 4 * i + 4) = 0;
LABEL_73:
    if (v43)
    {
        *(uint32_t*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_o_dialogue_actor = (uint32_t)AI_gpst_CurrentGameObject;
        num = 0;
        for (i = 0; i < 20; ++i)
        {
            v20 = (int*)(Gam_emploi_du_temps_edt_dialogue_check_univVars_Flag_speciaux_sauve + 4);
            v17 = *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_conditions_Dimension1, 3, i);
            v12 = *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_conditions_Dimension1, 2, i);
            v11 = *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_conditions_Dimension1, 1, i);
            v6 = (int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_conditions_Dimension1, 0, i);
            v42 = AI_EvalFunc_ARRBit4Test_C(v20, *v6, v11, v12, v17);
            if (v42)
            {
                if (v56->ai_grisage[i])
                {
                    v13 = AI_EvalVarArray2_C(10, (char*)&v56->__at_questionsReponses_Dimension1, 0, i);
                    v7 = AI_EvalVarArray_C(
                        10,
                        (char*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_at_dialog_questions,
                        num);
                    memcpy(v7, v13, 8u);
                    v14 = AI_EvalVarArray2_C(10, (char*)&v56->__at_questionsReponses_Dimension1, 1, i);
                    v8 = AI_EvalVarArray_C(
                        10,
                        (char*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_at_dialog_reponses,
                        num);
                    memcpy(v8, v14, 8u);
                    *(uint32_t*)(Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_ai_dialog_grisage + 4 * num + 4) = v56->ai_grisage[i];
                    *(uint32_t*)(Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_ai_dialog_consequences + 4 * num++ + 4) = i + 1;
                }
            }
        }
        v15 = AI_EvalVarArray_C(10, (char*)&v56->__at_question_reponse_OK_Dimension1, 0);
        v9 = AI_EvalVarArray_C(
            10,
            (char*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_at_dialog_questions,
            num);
        memcpy(v9, v15, 8u);
        v16 = AI_EvalVarArray_C(10, (char*)&v56->__at_question_reponse_OK_Dimension1, 1);
        v10 = AI_EvalVarArray_C(
            10,
            (char*)Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_at_dialog_reponses,
            num);
        memcpy(v10, v16, 8u);
        *(uint32_t*)(Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_ai_dialog_grisage + 4 * num + 4) = -1;
        *(uint32_t*)(Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_ai_dialog_consequences + 4 * num + 4) = 253;
        for (i = num + 1; i < 20; ++i)
        {
            *(uint32_t*)(Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_ai_dialog_grisage + 4 * i + 4) = 0;
            *(uint32_t*)(Gam_emploi_du_temps_edt_dialogue_check_GST_Board_panel_gao_ai_dialog_consequences + 4 * i + 4) = 0;
        }
    }
    v56->m_dialogue.msgID = 0;
    return 1000;
}

int Gam_emploi_du_temps_edt_action_attente()
{
    AI_EvalFunc_ACTActionSet_C_CURRENT(0);
    return 1000;
}

int Gam_emploi_du_temps_edt_occupation_loup_safe()
{
    float a4; // [esp+0h] [ebp-CCh]
    tScriptStruct_ActorGamEmploi* v2; // [esp+8h] [ebp-C4h]
    tGameObjectMain* v3; // [esp+10h] [ebp-BCh]
    tGameObjectMain* v4; // [esp+18h] [ebp-B4h]
    Vec3 dest; // [esp+20h] [ebp-ACh] BYREF
    tGameObjectMain* v6; // [esp+2Ch] [ebp-A0h]
    tGameObjectMain* v7; // [esp+30h] [ebp-9Ch]
    tGameObjectMain* a1; // [esp+34h] [ebp-98h]
    tGameObjectMain* v9; // [esp+38h] [ebp-94h]
    tGameObjectMain* v10; // [esp+3Ch] [ebp-90h]
    tGameObjectMain* v11; // [esp+40h] [ebp-8Ch]
    tGameObjectMain* v12; // [esp+44h] [ebp-88h]
    tGameObjectMain* v13; // [esp+48h] [ebp-84h]
    Vec3 v14; // [esp+4Ch] [ebp-80h] BYREF
    Vec3 v15; // [esp+58h] [ebp-74h] BYREF
    float v16; // [esp+64h] [ebp-68h]
    int v17; // [esp+68h] [ebp-64h]
    Vec3 v18; // [esp+6Ch] [ebp-60h] BYREF
    tGameObjectMain* v19; // [esp+78h] [ebp-54h]
    float v20; // [esp+7Ch] [ebp-50h]
    Vec3 v21; // [esp+80h] [ebp-4Ch] BYREF
    Vec3 a3; // [esp+8Ch] [ebp-40h] BYREF
    Vec3 a2; // [esp+98h] [ebp-34h] BYREF
    int v24; // [esp+A4h] [ebp-28h]
    tScriptStruct_ActorGamEmploi* v25; // [esp+ACh] [ebp-20h]
    Vec3 v26; // [esp+B0h] [ebp-1Ch] BYREF
    BOOL v27; // [esp+BCh] [ebp-10h]
    Vec3 a5; // [esp+C0h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v25 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Gam_emploi_du_temps_edt_occupation_loup_safe_Gam_emploi_du_temps_edt_occupation_loup_poursuite = (int)AI_ul_RealResolveOneRefFunction(0x2600004Au);
        fct_Gam_emploi_du_temps_edt_occupation_loup_safe_Gam_emploi_du_temps_edt_occupation_loup_nargue = (int)AI_ul_RealResolveOneRefFunction(0x2600004Du);
    }
    v19 = AI_gpst_CurrentGameObject;
    v13 = (tGameObjectMain*)v25->o_le_loup;
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
    v11 = v19;
    if (v19)
    {
        if ((unsigned int)v11 <= 2)
            v11 = AI_MainActor[(uint32_t)v11 - 1];
        v10 = v11;
    }
    else
    {
        v10 = 0;
    }
    if (v12 == v10)
    {
        v9 = (tGameObjectMain*)v25->o_safe;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            a1 = v9;
        }
        else
        {
            a1 = 0;
        }
        AI_EvalFunc_OBJ_CapaSet_C(a1, 0, 0x8000);
        AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(0, 0x200);
        v25->f_time_changetarget = TIM_gf_MainClock - TIM_gf_LockTime;
        v25->f_time_loup_init = v25->f_time_changetarget;
        v25->o_perso_a_attraper = 0;
        v24 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_occupation_loup_safe_Gam_emploi_du_temps_edt_occupation_loup_poursuite);
        if (v24 != 1000)
            return v24;
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v15);
    v7 = (tGameObjectMain*)v25->o_le_loup;
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
    AI_EvalFunc_OBJGetPos_C(v6, &v14);
    MATH_SubstractVec(&dest, &v15, &v14);
    v20 = MATH_VecLen(&dest);
    if (v25->f_distance_trop_proche - 0.5 < v20 && (v27 = AI_EvalFunc_TIMEElapsed_C(v25->f_time_safe, v25->f_duree_safe)))
    {
        v4 = (tGameObjectMain*)v25->o_safe;
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            AI_EvalFunc_OBJ_CapaSet_C(v4, 0, 0x8000);
        }
        else
        {
            AI_EvalFunc_OBJ_CapaSet_C(0, 0, 0x8000);
        }
        AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(0, 0x200);
        v25->o_safe_backup = v25->o_safe;
        v17 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Gam_emploi_du_temps_edt_occupation_loup_safe_Gam_emploi_du_temps_edt_occupation_loup_nargue);
        if (v17 != 1000)
            return v17;
    }
    else
    {
        AI_EvalFunc_ACTActionSet_C_CURRENT(23);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
        v3 = (tGameObjectMain*)v25->o_le_loup;
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            AI_EvalFunc_OBJGetPos_C(v3, &v26);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v26);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v18);
        MATH_SubstractVec(&a3, &v26, &v18);
        v16 = TIM_gf_dt;
        a4 = TIM_gf_dt * 7.0f;
        AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
        v21.x = 0.0;
        v21.y = 0.0;
        v21.z = 1.0;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a5, &v21);
    }
    return 1000;
}

int Gam_emploi_du_temps_edt_exec_suivi_regard()
{
    int v0; // eax
    tGameObjectMain* v1; // eax
    tScriptStruct_ActorGamEmploi* v3; // [esp+0h] [ebp-118h]
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
    tScriptStruct_ActorGamEmploi* v32; // [esp+F0h] [ebp-28h]
    float v33; // [esp+F4h] [ebp-24h]
    unsigned int v34; // [esp+F8h] [ebp-20h]
    Vec3 v35; // [esp+108h] [ebp-10h] BYREF
    float v36; // [esp+114h] [ebp-4h]

    v21.x = 1.0;
    v21.y = 0.0;
    v21.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_ActorGamEmploi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
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
            v34 = v32->des_i_RT_flags & ~0x10000u;
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
        v34 = v32->des_i_RT_flags & ~0x10000u;
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