#include "ai_compiledscripts_inc.h"

//Waf_Appareil_Photo_gam_init
int Waf_Appareil_Photo_gam_init_univVars;
int Waf_Appareil_Photo_gam_init_univVars_Flag_speciaux_sauve;
int fct_Waf_Appareil_Photo_gam_init_Waf_Appareil_Photo_gam_tag_off;
int fct_Waf_Appareil_Photo_gam_init_Waf_Appareil_Photo_gam_reflex;
int fct_Waf_Appareil_Photo_gam_init_Waf_Appareil_Photo_gam_tag_on;
int fct_Waf_Appareil_Photo_gam_init_Waf_Appareil_Photo_gam_after_rec;
int Waf_Appareil_Photo_gam_init_INV_manager_gao;
int Waf_Appareil_Photo_gam_init_INV_manager_gao_i_Animals_Nb;
int Waf_Appareil_Photo_gam_init_INV_manager_gao_ao_Animals;
int Waf_Appareil_Photo_gam_init_INV_manager_gao_ai_AnimalsData;
int Waf_Appareil_Photo_gam_init__PJ_Sally_gao;
int fct_Waf_Appareil_Photo_gam_init_Waf_Appareil_Photo_gam_comportement_deplacements;
int fct_Waf_Appareil_Photo_gam_init_Waf_Appareil_Photo_gam_comportement_attente_SF;

//Waf_Appareil_Photo_gam_reflex
int Waf_Appareil_Photo_gam_reflex_univVars;
int Waf_Appareil_Photo_gam_reflex_univVars_Flag_speciaux_sauve;
int Waf_Appareil_Photo_gam_reflex__PJ_Sally_gao;
int Waf_Appareil_Photo_gam_reflex__PJ_Sally_gao_current_pm_mode;
int Waf_Appareil_Photo_gam_reflex__PJ_Sally_gao_current_pm_priority;
int Waf_Appareil_Photo_gam_reflex_GST_Board_panel_gao;
int Waf_Appareil_Photo_gam_reflex_GST_Board_panel_gao_i_saut_prio;
int Waf_Appareil_Photo_gam_reflex_GST_Board_panel_gao_o_saut_object;
int Waf_Appareil_Photo_gam_reflex_GST_Board_panel_gao_i_saut_action;
int Waf_Appareil_Photo_gam_reflex_GST_Board_panel_gao_Boutonces_JumpCurAction;
int Waf_Appareil_Photo_gam_reflex_GST_AffichageTexte_gao;
int Waf_Appareil_Photo_gam_reflex_GST_AffichageTexte_gao_i_encart;
int Waf_Appareil_Photo_gam_reflex_GST_AffichageTexte_gao_message_a_traiter;
int fct_Waf_Appareil_Photo_gam_reflex_Waf_Appareil_Photo_gam_comportement_attrape;

//Waf_Appareil_Photo_gam_comportement_deplacements
int Waf_Appareil_Photo_gam_comportement_deplacements__PJ_Sally_gao;
int Waf_Appareil_Photo_gam_comportement_deplacements_GST_AffichageTexte_gao;
int Waf_Appareil_Photo_gam_comportement_deplacements_GST_AffichageTexte_gao_i_affiche;
int Waf_Appareil_Photo_gam_comportement_deplacements_TXT_Sally_gao;
int Waf_Appareil_Photo_gam_comportement_deplacements__PJ_Sally_gao_current_pm_mode;
int Waf_Appareil_Photo_gam_comportement_deplacements__PJ_Sally_gao_i_joy_action_kit;
int fct_Waf_Appareil_Photo_gam_comportement_deplacements_Waf_Appareil_Photo_gam_exec_recherche_chemin;

//Waf_Appareil_Photo_gam_comportement_attrape
int Waf_Appareil_Photo_gam_comportement_attrape__PJ_Sally_gao;
int Waf_Appareil_Photo_gam_comportement_attrape_GST_Global_gao;
int Waf_Appareil_Photo_gam_comportement_attrape_GST_Global_gao_o_joy_acteur;
int Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao;
int Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_i_dynplan;
int Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_f_timedynplan;
int Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_o_viseedynplan;
int Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_f_zdynplan;
int Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_f_distdynplan;
int Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_i_contrechampdynplan;
int Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_v_sightdynplan;
int Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_v_offsetviseedynplan;
int Waf_Appareil_Photo_gam_comportement_attrape_univVars;
int Waf_Appareil_Photo_gam_comportement_attrape_univVars_Flag_speciaux_sauve;

//Waf_Appareil_Photo_gam_comportement_attente_SF
int Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao;
int Waf_Appareil_Photo_gam_comportement_attente_SF_univVars;
int Waf_Appareil_Photo_gam_comportement_attente_SF_univVars_Flag_speciaux_sauve;
int Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao_current_pm_mode;
int Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao_current_pm_priority;
int Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao;
int Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao_i_sally_priority;
int Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao_o_sally_contextuel;
int Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao_i_sally_action;
int Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao_v_sally_offset;
int Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao_i_force_interface;
int fct_Waf_Appareil_Photo_gam_comportement_attente_SF_Waf_Appareil_Photo_gam_comportement_deplacements;

//Waf_Appareil_Photo_gam_after_rec
int Waf_Appareil_Photo_gam_after_rec_univVars;
int Waf_Appareil_Photo_gam_after_rec_univVars_Flag_speciaux_sauve;
int Waf_Appareil_Photo_gam_after_rec__PJ_Sally_gao;

//Waf_Appareil_Photo__gam_comportement_coince
int fct_Waf_Appareil_Photo__gam_comportement_coince_Waf_Appareil_Photo_gam_comportement_deplacements;

int Waf_Appareil_Photo_gam_init()
{
    unsigned int v0; // eax
    tScriptStruct_ActorWafwaf* v2; // [esp+4h] [ebp-CCh]
    tGameObjectMain* v3; // [esp+10h] [ebp-C0h]
    tGameObjectMain* v4; // [esp+1Ch] [ebp-B4h]
    tGameObjectMain* v5; // [esp+20h] [ebp-B0h]
    tGameObjectMain* v6; // [esp+24h] [ebp-ACh]
    tGameObjectMain* v7; // [esp+2Ch] [ebp-A4h]
    tGameObjectMain* v8; // [esp+34h] [ebp-9Ch]
    tGameObjectMain* v9; // [esp+38h] [ebp-98h]
    tGameObjectMain* v10; // [esp+3Ch] [ebp-94h]
    tGameObjectMain* v11; // [esp+44h] [ebp-8Ch]
    tGameObjectMain* v12; // [esp+48h] [ebp-88h]
    tGameObjectMain* v13; // [esp+4Ch] [ebp-84h]
    tGameObjectMain* v14; // [esp+50h] [ebp-80h]
    tGameObjectMain* v15; // [esp+54h] [ebp-7Ch]
    tGameObjectMain* v16; // [esp+58h] [ebp-78h]
    tGameObjectMain* v17; // [esp+5Ch] [ebp-74h]
    tGameObjectMain* v18; // [esp+60h] [ebp-70h]
    int v19; // [esp+68h] [ebp-68h]
    tGameObjectMain* v20; // [esp+6Ch] [ebp-64h]
    Vec3 v21; // [esp+78h] [ebp-58h] BYREF
    tGameObjectMain* v22; // [esp+8Ch] [ebp-44h]
    Vec3 a3; // [esp+90h] [ebp-40h] BYREF
    int i; // [esp+9Ch] [ebp-34h]
    int v25; // [esp+A0h] [ebp-30h]
    int v26; // [esp+A4h] [ebp-2Ch]
    int v27; // [esp+A8h] [ebp-28h]
    unsigned int v28; // [esp+ACh] [ebp-24h]
    tScriptStruct_ActorWafwaf* v29; // [esp+B4h] [ebp-1Ch]
    BOOL v30; // [esp+B8h] [ebp-18h]
    Vec3 v31; // [esp+BCh] [ebp-14h] BYREF

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorWafwaf*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v29 = v2;
    i = 0;
    v25 = 0;
    v30 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Waf_Appareil_Photo_gam_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_init_univVars_Flag_speciaux_sauve,
            (void*)Waf_Appareil_Photo_gam_init_univVars,
            4488);
        fct_Waf_Appareil_Photo_gam_init_Waf_Appareil_Photo_gam_tag_off = (int)AI_ul_RealResolveOneRefFunction(0x2600015Bu);
        fct_Waf_Appareil_Photo_gam_init_Waf_Appareil_Photo_gam_reflex = (int)AI_ul_RealResolveOneRefFunction(0x26000159u);
        fct_Waf_Appareil_Photo_gam_init_Waf_Appareil_Photo_gam_tag_on = (int)AI_ul_RealResolveOneRefFunction(0x2600015Au);
        fct_Waf_Appareil_Photo_gam_init_Waf_Appareil_Photo_gam_after_rec = (int)AI_ul_RealResolveOneRefFunction(0x260005D3u);
        Waf_Appareil_Photo_gam_init_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_init_INV_manager_gao_i_Animals_Nb,
            (void*)Waf_Appareil_Photo_gam_init_INV_manager_gao,
            23888);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_init_INV_manager_gao_ao_Animals,
            (void*)Waf_Appareil_Photo_gam_init_INV_manager_gao,
            23368);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_init_INV_manager_gao_ai_AnimalsData,
            (void*)Waf_Appareil_Photo_gam_init_INV_manager_gao,
            23628);
        Waf_Appareil_Photo_gam_init__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        fct_Waf_Appareil_Photo_gam_init_Waf_Appareil_Photo_gam_comportement_deplacements = (int)AI_ul_RealResolveOneRefFunction(0x26000156u);
        fct_Waf_Appareil_Photo_gam_init_Waf_Appareil_Photo_gam_comportement_attente_SF = (int)AI_ul_RealResolveOneRefFunction(0x26000225u);
    }
    v29->SF_app_photo = 2730;
    AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x40, 0);
    v20 = AI_gpst_CurrentGameObject;
    v18 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v18 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        v17 = v18;
    }
    else
    {
        v17 = 0;
    }
    v17->extended->fields->designInt3 = 0x4000001;
    v16 = v20;
    if (v20)
    {
        if ((unsigned int)v20 <= 2)
            v16 = AI_MainActor[(uint32_t)v20 - 1];
        v15 = v16;
    }
    else
    {
        v15 = 0;
    }
    v15->extended->fields->designFlags |= 0x40u;
    v14 = v20;
    if (v20)
    {
        if ((unsigned int)v20 <= 2)
            v14 = AI_MainActor[(uint32_t)v20 - 1];
        v13 = v14;
    }
    else
    {
        v13 = 0;
    }
    v13->extended->fields->designFloat1 = 0.0;
    v12 = v20;
    if (v20)
    {
        if ((unsigned int)v20 <= 2)
            v12 = AI_MainActor[(uint32_t)v20 - 1];
        v11 = v12;
    }
    else
    {
        v11 = 0;
    }
    v11->extended->fields->designFloat2 = 1.0;
    if (v29->SF_app_photo <= 0)
        v30 = 0;
    else
        v30 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Waf_Appareil_Photo_gam_init_univVars_Flag_speciaux_sauve + 4),
            v29->SF_app_photo);
    if (v30)
    {
        v10 = (tGameObjectMain*)v29->o_perso_a_activer_si_jai_lapp;
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
            v8 = (tGameObjectMain*)v29->o_perso_a_activer_si_jai_lapp;
            if (v8)
            {
                if ((unsigned int)v8 <= 2)
                    v8 = AI_MainActor[(uint32_t)v8 - 1];
                v0 = AI_EvalFunc_OBJ_FlagsControlSet_C(v8, 0, 0x600000);
            }
            else
            {
                v0 = AI_EvalFunc_OBJ_FlagsControlSet_C(0, 0, 0x600000);
            }
            v28 = v0;
        }
        v19 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v19 != 1000)
            return v19;
    }
    AI_EvalFunc_AITrackChange_C(0, (tAIFunction*)fct_Waf_Appareil_Photo_gam_init_Waf_Appareil_Photo_gam_tag_off);
    AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_Waf_Appareil_Photo_gam_init_Waf_Appareil_Photo_gam_reflex);
    AI_EvalFunc_AITrackChange_C(4, (tAIFunction*)fct_Waf_Appareil_Photo_gam_init_Waf_Appareil_Photo_gam_tag_on);
    v7 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v7 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v7,
            2,
            (tAIFunction*)fct_Waf_Appareil_Photo_gam_init_Waf_Appareil_Photo_gam_after_rec);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            2,
            (tAIFunction*)fct_Waf_Appareil_Photo_gam_init_Waf_Appareil_Photo_gam_after_rec);
    }
    v25 = 0;
    if (*(int*)Waf_Appareil_Photo_gam_init_INV_manager_gao_i_Animals_Nb < 64)
    {
        for (i = 0; i < *(int32_t*)Waf_Appareil_Photo_gam_init_INV_manager_gao_i_Animals_Nb; ++i)
        {
            v22 = AI_gpst_CurrentGameObject;
            v6 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v6 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v5 = v6;
            }
            else
            {
                v5 = 0;
            }
            if (*(tGameObjectMain**)(Waf_Appareil_Photo_gam_init_INV_manager_gao_ao_Animals + 4 * i + 4) == v5)
                break;
        }
        if (i == *(uint32_t*)Waf_Appareil_Photo_gam_init_INV_manager_gao_i_Animals_Nb)
        {
            *(uint32_t*)(Waf_Appareil_Photo_gam_init_INV_manager_gao_ao_Animals
                + 4 * *(uint32_t*)Waf_Appareil_Photo_gam_init_INV_manager_gao_i_Animals_Nb
                + 4) = (uint32_t)AI_gpst_CurrentGameObject;
            *(uint32_t*)(Waf_Appareil_Photo_gam_init_INV_manager_gao_ai_AnimalsData
                + 4 * (*(uint32_t*)Waf_Appareil_Photo_gam_init_INV_manager_gao_i_Animals_Nb)++
                + 4) = 25601;
        }
    }
    v4 = (tGameObjectMain*)v29->o_app_photo;
    if (v4)
    {
        if ((unsigned int)v4 <= 2)
            v4 = AI_MainActor[(uint32_t)v4 - 1];
        AI_EvalFunc_OBJ_FlagInactiveSet_C(v4, 1);
    }
    else
    {
        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
    }
    v29->tag_change_type = 2;
    AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x8000, 0);
    AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
    v27 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x801004, 0);
    v31.x = 0.0;
    v31.y = 0.0;
    v31.z = -20.0;
    AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v31);
    v21.x = 0.0;
    v21.y = 0.0;
    v21.z = 0.0;
    v26 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v21);
    AI_EvalFunc_COLCrossableSet_C(AI_gpst_CurrentGameObject, 0, 0x80);
    AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 1u, -2);
    v29->o_bones_app_photo = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0xAu);
    v29->f_dist_propose_attrape = v29->f_dist_propose_attrape * v29->f_dist_propose_attrape;
    v29->f_distance_fight = v29->f_distance_fight * v29->f_distance_fight;
    v29->o_le_loup = Waf_Appareil_Photo_gam_init__PJ_Sally_gao;
    if (v29->SF_joue <= 0)
        v30 = 0;
    else
        v30 = AI_EvalFunc_ARRBitTest_C((int*)(Waf_Appareil_Photo_gam_init_univVars_Flag_speciaux_sauve + 4), v29->SF_joue);
    if (v30)
    {
        v3 = (tGameObjectMain*)v29->o_point_attente;
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
        AI_EvalFunc_OBJSetPos_C_CURRENT(&a3);
        v29->i_joujou = 1;
        v29->i_r2_pressed = 0;
        v29->f_time_message_r2 = TIM_gf_MainClock - TIM_gf_LockTime;
        if (v29->SF_attrape > 0)
            AI_EvalFunc_ARRBitClear_C((int*)(Waf_Appareil_Photo_gam_init_univVars_Flag_speciaux_sauve + 4), v29->SF_attrape);
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Waf_Appareil_Photo_gam_init_Waf_Appareil_Photo_gam_comportement_deplacements);
    }
    else
    {
        AI_EvalFunc_ACTActionSet_C_CURRENT(7);
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Waf_Appareil_Photo_gam_init_Waf_Appareil_Photo_gam_comportement_attente_SF);
    }
    return 1000;
}

int Waf_Appareil_Photo_gam_reflex()
{
    int v0; // eax
    tScriptStruct_ActorWafwaf* v2; // [esp+0h] [ebp-70h]
    tGameObjectMain* v3; // [esp+Ch] [ebp-64h]
    void* Src; // [esp+14h] [ebp-5Ch]
    tMessage v5; // [esp+20h] [ebp-50h] BYREF
    int v6; // [esp+58h] [ebp-18h]
    BOOL v7; // [esp+5Ch] [ebp-14h]
    int v8; // [esp+60h] [ebp-10h]
    tScriptStruct_ActorWafwaf* v9; // [esp+64h] [ebp-Ch]
    BOOL v10; // [esp+68h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorWafwaf*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v9 = v2;
    v7 = 0;
    v10 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Waf_Appareil_Photo_gam_reflex_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_reflex_univVars_Flag_speciaux_sauve,
            (void*)Waf_Appareil_Photo_gam_reflex_univVars,
            4488);
        Waf_Appareil_Photo_gam_reflex__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_reflex__PJ_Sally_gao_current_pm_mode,
            (void*)Waf_Appareil_Photo_gam_reflex__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_reflex__PJ_Sally_gao_current_pm_priority,
            (void*)Waf_Appareil_Photo_gam_reflex__PJ_Sally_gao,
            576);
        Waf_Appareil_Photo_gam_reflex_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_reflex_GST_Board_panel_gao_i_saut_prio,
            (void*)Waf_Appareil_Photo_gam_reflex_GST_Board_panel_gao,
            440);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_reflex_GST_Board_panel_gao_o_saut_object,
            (void*)Waf_Appareil_Photo_gam_reflex_GST_Board_panel_gao,
            432);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_reflex_GST_Board_panel_gao_i_saut_action,
            (void*)Waf_Appareil_Photo_gam_reflex_GST_Board_panel_gao,
            444);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_reflex_GST_Board_panel_gao_Boutonces_JumpCurAction,
            (void*)Waf_Appareil_Photo_gam_reflex_GST_Board_panel_gao,
            2012);
        Waf_Appareil_Photo_gam_reflex_GST_AffichageTexte_gao = AI_ul_RealResolveOneRefObject(0x7100E7ACu);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_reflex_GST_AffichageTexte_gao_i_encart,
            (void*)Waf_Appareil_Photo_gam_reflex_GST_AffichageTexte_gao,
            76);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_reflex_GST_AffichageTexte_gao_message_a_traiter,
            (void*)Waf_Appareil_Photo_gam_reflex_GST_AffichageTexte_gao,
            0);
        fct_Waf_Appareil_Photo_gam_reflex_Waf_Appareil_Photo_gam_comportement_attrape = (int)AI_ul_RealResolveOneRefFunction(0x2600028Du);
    }
    if (v9->i_joujou)
    {
        v10 = v9->SF_attrape > 0
            && AI_EvalFunc_ARRBitTest_C(
                (int*)(Waf_Appareil_Photo_gam_reflex_univVars_Flag_speciaux_sauve + 4),
                v9->SF_attrape);
        if (!v10)
        {
            v3 = (tGameObjectMain*)v9->o_center;
            if (v3)
            {
                if ((unsigned int)v3 <= 2)
                    v3 = AI_MainActor[(uint32_t)v3 - 1];
                v0 = AI_EvalFunc_COLPivotBVCollide_C((tGameObjectMain*)Waf_Appareil_Photo_gam_reflex__PJ_Sally_gao, v3);
            }
            else
            {
                v0 = AI_EvalFunc_COLPivotBVCollide_C((tGameObjectMain*)Waf_Appareil_Photo_gam_reflex__PJ_Sally_gao, 0);
            }
            v8 = v0;
            if (v0)
            {
                v6 = (unsigned __int16)*(uint32_t*)(Waf_Appareil_Photo_gam_reflex__PJ_Sally_gao + 4);
                if ((v6 & 1) != 0)
                    v7 = !*(uint32_t*)Waf_Appareil_Photo_gam_reflex__PJ_Sally_gao_current_pm_mode
                    || *(int*)Waf_Appareil_Photo_gam_reflex__PJ_Sally_gao_current_pm_priority < 20;
                else
                    v7 = 0;
                if (v7 && *(int*)Waf_Appareil_Photo_gam_reflex_GST_Board_panel_gao_i_saut_prio < 27)
                {
                    *(uint32_t*)Waf_Appareil_Photo_gam_reflex_GST_Board_panel_gao_o_saut_object = (uint32_t)AI_gpst_CurrentGameObject;
                    *(uint32_t*)Waf_Appareil_Photo_gam_reflex_GST_Board_panel_gao_i_saut_prio = 27;
                    *(uint32_t*)Waf_Appareil_Photo_gam_reflex_GST_Board_panel_gao_i_saut_action = 0;
                    *(uint32_t*)Waf_Appareil_Photo_gam_reflex_GST_Board_panel_gao_Boutonces_JumpCurAction = 9;
                }
            }
            Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
            memcpy(&v5, Src, sizeof(v5));
            if (v5.msgID == 93)
            {
                v5.msgID = 75;
                v5.gao = AI_gpst_CurrentGameObject;
                v5.Int1 = 0;
                v5.Int2 = 0;
                AI_EvalFunc_MSGSend_C((tGameObjectMain*)Waf_Appareil_Photo_gam_reflex__PJ_Sally_gao, &v5);
            }
            else if (v5.msgID == 75 && v5.Int2)
            {
                if (*(uint32_t*)Waf_Appareil_Photo_gam_reflex_GST_AffichageTexte_gao_i_encart)
                {
                    *(uint32_t*)(Waf_Appareil_Photo_gam_reflex_GST_AffichageTexte_gao_message_a_traiter + 48) = 1;
                    *(uint32_t*)(Waf_Appareil_Photo_gam_reflex_GST_AffichageTexte_gao_message_a_traiter + 40) = -1;
                }
                v9->f_time_attrape = TIM_gf_MainClock - TIM_gf_LockTime;
                if (v9->SF_attrape > 0)
                    AI_EvalFunc_ARRBitSet_C(
                        (int*)(Waf_Appareil_Photo_gam_reflex_univVars_Flag_speciaux_sauve + 4),
                        v9->SF_attrape);
                AI_EvalFunc_ACTActionSet_C_CURRENT(8);
                v9->i_flag = 0;
                v9->i_flag_sally_vers_moi = 0;
                AI_EvalFunc_AITrackChange_C(
                    2,
                    (tAIFunction*)fct_Waf_Appareil_Photo_gam_reflex_Waf_Appareil_Photo_gam_comportement_attrape);
            }
        }
    }
    return 1000;
}

int Waf_Appareil_Photo_gam_comportement_deplacements()
{
    BOOL v0; // eax
    int v1; // eax
    double v2; // st7
    double v3; // st7
    double v4; // st7
    float a4; // [esp+0h] [ebp-3D8h]
    tScriptStruct_ActorWafwaf* v7; // [esp+8h] [ebp-3D0h]
    tGameObjectMain* v8; // [esp+18h] [ebp-3C0h]
    tGameObjectMain* v9; // [esp+20h] [ebp-3B8h]
    tGameObjectMain* v10; // [esp+38h] [ebp-3A0h]
    float v11; // [esp+4Ch] [ebp-38Ch]
    tGameObjectMain* v12; // [esp+64h] [ebp-374h]
    tGameObjectMain* v13; // [esp+7Ch] [ebp-35Ch]
    tGameObjectMain* v14; // [esp+84h] [ebp-354h]
    tGameObjectMain* v15; // [esp+8Ch] [ebp-34Ch]
    tGameObjectMain* v16; // [esp+94h] [ebp-344h]
    tGameObjectMain* v17; // [esp+9Ch] [ebp-33Ch]
    tGameObjectMain* v18; // [esp+A4h] [ebp-334h]
    tGameObjectMain* v19; // [esp+ACh] [ebp-32Ch]
    tGameObjectMain* v20; // [esp+B4h] [ebp-324h]
    tGameObjectMain* v21; // [esp+C8h] [ebp-310h]
    Vec3 v22[2]; // [esp+CCh] [ebp-30Ch] BYREF
    float v23; // [esp+E4h] [ebp-2F4h]
    float v24; // [esp+E8h] [ebp-2F0h]
    float v26; // [esp+F0h] [ebp-2E8h]
    Vec3 v27[2]; // [esp+F4h] [ebp-2E4h] BYREF
    Vec3 v28; // [esp+10Ch] [ebp-2CCh] BYREF
    Vec3 dest; // [esp+11Ch] [ebp-2BCh] BYREF
    Vec3 v30; // [esp+128h] [ebp-2B0h] BYREF
    Vec3 v31; // [esp+134h] [ebp-2A4h] BYREF
    Vec3 v32; // [esp+14Ch] [ebp-28Ch] BYREF
    Vec3 v33; // [esp+158h] [ebp-280h] BYREF
    Vec3 v34; // [esp+164h] [ebp-274h] BYREF
    Vec3 v35; // [esp+170h] [ebp-268h] BYREF
    Vec3 v36[2]; // [esp+17Ch] [ebp-25Ch] BYREF
    float v37; // [esp+194h] [ebp-244h]
    int v38; // [esp+198h] [ebp-240h]
    Vec3 v39; // [esp+1A0h] [ebp-238h]
    Vec3 v40; // [esp+1ACh] [ebp-22Ch] BYREF
    Vec3 v41; // [esp+1B8h] [ebp-220h] BYREF
    Vec3 v42; // [esp+1C4h] [ebp-214h] BYREF
    tMessage Src; // [esp+1D0h] [ebp-208h] BYREF
    float v44; // [esp+204h] [ebp-1D4h]
    int v45; // [esp+208h] [ebp-1D0h]
    Vec3 v46; // [esp+20Ch] [ebp-1CCh] BYREF
    Vec3 a1; // [esp+218h] [ebp-1C0h] BYREF
    float v48; // [esp+224h] [ebp-1B4h]
    Vec3 v49; // [esp+228h] [ebp-1B0h] BYREF
    Vec3 v50; // [esp+234h] [ebp-1A4h] BYREF
    Vec3 v51; // [esp+240h] [ebp-198h]
    Vec3 v52; // [esp+24Ch] [ebp-18Ch] BYREF
    float v53; // [esp+258h] [ebp-180h]
    float v54; // [esp+25Ch] [ebp-17Ch]
    Vec3 v55; // [esp+260h] [ebp-178h]
    Vec3 v56; // [esp+26Ch] [ebp-16Ch] BYREF
    Vec3 v57; // [esp+278h] [ebp-160h] BYREF
    Vec3 v58; // [esp+284h] [ebp-154h]
    Vec3 v59; // [esp+290h] [ebp-148h] BYREF
    Vec3 v60; // [esp+2A0h] [ebp-138h] BYREF
    Vec3 v61[2]; // [esp+2ACh] [ebp-12Ch] BYREF
    Vec3 v62; // [esp+2C4h] [ebp-114h] BYREF
    float v63; // [esp+2D4h] [ebp-104h]
    Vec3 a3; // [esp+2DCh] [ebp-FCh] BYREF
    float v65; // [esp+2E8h] [ebp-F0h]
    float v66; // [esp+2ECh] [ebp-ECh]
    Vec3 v68; // [esp+2F4h] [ebp-E4h] BYREF
    Vec3 v69; // [esp+300h] [ebp-D8h] BYREF
    float v70; // [esp+30Ch] [ebp-CCh]
    int v71; // [esp+310h] [ebp-C8h]
    Vec3 v72; // [esp+320h] [ebp-B8h] BYREF
    Vec3 v73; // [esp+32Ch] [ebp-ACh] BYREF
    Vec3 v74; // [esp+338h] [ebp-A0h] BYREF
    Vec3 v75; // [esp+344h] [ebp-94h] BYREF
    Vec3 v76; // [esp+350h] [ebp-88h] BYREF
    tScriptStruct_ActorWafwaf* v77; // [esp+35Ch] [ebp-7Ch]
    int v78; // [esp+360h] [ebp-78h]
    float v79; // [esp+364h] [ebp-74h]
    Vec3 v80; // [esp+368h] [ebp-70h] BYREF
    Vec3 v81; // [esp+374h] [ebp-64h] BYREF
    Vec3 v82; // [esp+380h] [ebp-58h] BYREF
    int v84; // [esp+390h] [ebp-48h]
    float v85; // [esp+394h] [ebp-44h]
    BOOL v86; // [esp+398h] [ebp-40h]
    Vec3 v87; // [esp+3A8h] [ebp-30h] BYREF
    Vec3 v88; // [esp+3B4h] [ebp-24h] BYREF
    Vec3 v89; // [esp+3CCh] [ebp-Ch] BYREF

    v39.x = 1.0;
    v39.y = 0.0;
    v39.z = 0.0;
    v55.x = 1.0;
    v55.y = 0.0;
    v55.z = 0.0;
    v40.x = 1.0;
    v40.y = 0.0;
    v40.z = 0.0;
    v56.x = 1.0;
    v56.y = 0.0;
    v56.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_ActorWafwaf*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    v77 = v7;
    v44 = 0.0;
    v38 = 0;
    v24 = 0.0;
    v78 = 0;
    v66 = 0.0;
    v85 = 0.0;
    v79 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Waf_Appareil_Photo_gam_comportement_deplacements__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Waf_Appareil_Photo_gam_comportement_deplacements_GST_AffichageTexte_gao = AI_ul_RealResolveOneRefObject(0x7100E7ACu);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_deplacements_GST_AffichageTexte_gao_i_affiche,
            (void*)Waf_Appareil_Photo_gam_comportement_deplacements_GST_AffichageTexte_gao,
            204);
        Waf_Appareil_Photo_gam_comportement_deplacements_TXT_Sally_gao = AI_ul_RealResolveOneRefObject(0x7200064Eu);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_deplacements__PJ_Sally_gao_current_pm_mode,
            (void*)Waf_Appareil_Photo_gam_comportement_deplacements__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_deplacements__PJ_Sally_gao_i_joy_action_kit,
            (void*)Waf_Appareil_Photo_gam_comportement_deplacements__PJ_Sally_gao,
            52);
        fct_Waf_Appareil_Photo_gam_comportement_deplacements_Waf_Appareil_Photo_gam_exec_recherche_chemin = (int)AI_ul_RealResolveOneRefFunction(0x26000163u);
    }
    v45 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x800000, 0);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v74);
    v21 = (tGameObjectMain*)v77->o_le_loup;
    if (v21)
    {
        if ((unsigned int)v21 <= 2)
            v21 = AI_MainActor[(uint32_t)v21 - 1];
        AI_EvalFunc_OBJGetPos_C(v21, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    MATH_SubstractVec(&dest, &v74, &a3);
    a1 = dest;
    v79 = MATH_VecLen(&a1);
    v38 = AI_EvalFunc_ACTActionGet_C_CURRENT();
    v20 = (tGameObjectMain*)v77->o_center;
    if (v20)
    {
        if ((unsigned int)v20 <= 2)
            v20 = AI_MainActor[(uint32_t)v20 - 1];
        v0 = AI_EvalFunc_COLBVObjectObject_C(
            (tGameObjectMain*)Waf_Appareil_Photo_gam_comportement_deplacements__PJ_Sally_gao,
            v20);
    }
    else
    {
        v0 = AI_EvalFunc_COLBVObjectObject_C(
            (tGameObjectMain*)Waf_Appareil_Photo_gam_comportement_deplacements__PJ_Sally_gao,
            0);
    }
    v86 = v0;
    if (v0)
    {
        v66 = AI_EvalFunc_DYNSpeedGet_C((tGameObjectMain*)Waf_Appareil_Photo_gam_comportement_deplacements__PJ_Sally_gao);
        if (v77->i_r2_out || v66 < 0.2)
        {
            v65 = TIM_gf_MainClock - TIM_gf_LockTime;
            v77->f_time_message_r2 = v65 - 8.0f;
            v77->i_r2_out = 0;
        }
        if (!*(uint32_t*)Waf_Appareil_Photo_gam_comportement_deplacements_GST_AffichageTexte_gao_i_affiche)
        {
            if (AI_EvalFunc_TIMEElapsed_C(v77->f_time_message_r2, 10.0))
            {
                if (v66 > 0.80000001)
                {
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v41);
                    v63 = a1.z * v41.z + a1.y * v41.y + v41.x * a1.x;
                    if (v63 > 0.0)
                    {
                        Src.msgID = 30;
                        Src.Int1 = 72;
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
                        AI_EvalFunc_MSGSend_C(
                            (tGameObjectMain*)Waf_Appareil_Photo_gam_comportement_deplacements_TXT_Sally_gao,
                            &Src);
                        v77->f_time_message_r2 = TIM_gf_MainClock - TIM_gf_LockTime;
                    }
                }
            }
        }
    }
    else
    {
        v77->i_r2_out = 1;
    }
    v19 = (tGameObjectMain*)v77->o_center;
    if (v19)
    {
        if ((unsigned int)v19 <= 2)
            v19 = AI_MainActor[(uint32_t)v19 - 1];
        AI_EvalFunc_OBJGetPos_C(v19, &v42);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v42);
    }
    v52.x = 0.0;
    v52.y = 0.0;
    v52.z = 0.62f;
    MATH_VecAddVec(v27, &v42, &v52);
    v51.x = 0.0;
    v51.y = 0.0;
    v51.z = 1.0;
    v85 = 6.0;
    v18 = (tGameObjectMain*)v77->o_le_loup;
    if (v18)
    {
        if ((unsigned int)v18 <= 2)
            v18 = AI_MainActor[(uint32_t)v18 - 1];
        v1 = AI_EvalFunc_COLMapActivationGet_C(v18);
    }
    else
    {
        v1 = AI_EvalFunc_COLMapActivationGet_C(0);
    }
    v71 = v1;
    if ((v1 & 2) != 0)
    {
        v17 = (tGameObjectMain*)v77->o_le_loup;
        if (v17)
        {
            if ((unsigned int)v17 <= 2)
                v17 = AI_MainActor[(uint32_t)v17 - 1];
            v2 = AI_EvalFunc_DYNSpeedGet_C(v17);
        }
        else
        {
            v2 = AI_EvalFunc_DYNSpeedGet_C(0);
        }
        v44 = (float)v2;
        if (v44 >= 1.5)
        {
            if (v44 < 2.3)
                v85 = 3.0;
        }
        else
        {
            v85 = 2.0;
        }
    }
    if (v79 <= 18.0)
    {
        if ((*(uint32_t*)Waf_Appareil_Photo_gam_comportement_deplacements__PJ_Sally_gao_current_pm_mode == 8
            || (v79 >= (double)v85 || v38 == 2) && (v38 != 2 || v79 >= 7.5))
            && (*(uint32_t*)Waf_Appareil_Photo_gam_comportement_deplacements__PJ_Sally_gao_current_pm_mode != 8
                || *(int*)Waf_Appareil_Photo_gam_comportement_deplacements__PJ_Sally_gao_i_joy_action_kit >= 5
                && *(int*)Waf_Appareil_Photo_gam_comportement_deplacements__PJ_Sally_gao_i_joy_action_kit <= 6))
        {
            v9 = (tGameObjectMain*)v77->o_center;
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                v4 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v9);
            }
            else
            {
                v4 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
            }
            v23 = (float)v4;
            if (v23 >= 64.0 || (v79 <= 8.5 || v38 == 1) && (v38 != 1 || v79 <= 6.5))
            {
                v77->i_way_moving = 0;
                v77->i_way_force_recompute = 1;
                v78 = 0;
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v32);
                v62.x = -a1.x;
                v62.y = -a1.y;
                v62.z = -a1.z;
                v26 = TIM_gf_dt;
                a4 = 7.0f * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&v32, &v62, a4, &v77->v_way_sight);
            }
            else
            {
                v8 = (tGameObjectMain*)v77->o_le_loup;
                if (v8)
                {
                    if ((unsigned int)v8 <= 2)
                        v8 = AI_MainActor[(uint32_t)v8 - 1];
                    AI_EvalFunc_OBJGetPos_C(v8, &v77->v_way_destpos);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v77->v_way_destpos);
                }
                v77->i_way_moving = 1;
                v77->f_way_blendrotate = 0.2f;
                v78 = 1;
            }
        }
        else
        {
            if (AI_EvalFunc_TIMEElapsed_C(v77->f_time_longe_mur, 3.0))
            {
                v24 = v77->f_mon_rayon_daction;
                v15 = (tGameObjectMain*)v77->o_center;
                if (v15)
                {
                    if ((unsigned int)v15 <= 2)
                        v15 = AI_MainActor[(uint32_t)v15 - 1];
                    v3 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v15);
                }
                else
                {
                    v3 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
                }
                v48 = (float)v3;
                if (v24 * v24 >= v48)
                {
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v33);
                    MATH_VecAddVec(&v50, &v33, &a1);
                    v77->v_way_destpos = v50;
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v46);
                    v14 = (tGameObjectMain*)v77->o_center;
                    if (v14)
                    {
                        if ((unsigned int)v14 <= 2)
                            v14 = AI_MainActor[(uint32_t)v14 - 1];
                        AI_EvalFunc_OBJGetPos_C(v14, &v69);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v69);
                    }
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v30);
                    v81.x = 0.0;
                    v81.y = 0.0;
                    v81.z = -1.0;
                    MATH_VecAddVec(&v57, &v30, &v81);
                    MATH_SubstractVec(&v76, &v69, &v57);
                    v13 = (tGameObjectMain*)v77->o_center;
                    if (v13)
                    {
                        if ((unsigned int)v13 <= 2)
                            v13 = AI_MainActor[(uint32_t)v13 - 1];
                        AI_EvalFunc_OBJGetPos_C(v13, &v59);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v59);
                    }
                    AI_EvalFunc_OBJGetPos_C_CURRENT(v61);
                    MATH_SubstractVec(&v80, &v59, v61);
                    AI_EvalFunc_MATHVecNormalize_C(&v80, &v34);
                    v56 = v34;
                    v88.x = 0.0;
                    v88.y = 0.0;
                    v88.z = 1.0;
                    AI_EvalFunc_MATHVecCrossProduct_C(&v56, &v88, &v68);
                    v39 = v68;
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v28);
                    v12 = (tGameObjectMain*)v77->o_center;
                    if (v12)
                    {
                        if ((unsigned int)v12 <= 2)
                            v12 = AI_MainActor[(uint32_t)v12 - 1];
                        AI_EvalFunc_OBJGetPos_C(v12, &v35);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v35);
                    }
                    MATH_SubstractVec(&v75, &v28, &v35);
                    v40 = v75;
                    v82.x = 0.0;
                    v82.y = 0.0;
                    v82.z = 1.0;
                    AI_EvalFunc_MATHVecCrossProduct_C(&v40, &v82, &v60);
                    v55 = v60;
                    v53 = a1.z * v60.z + a1.y * v60.y + v60.x * a1.x;
                    v70 = AI_EvalFunc_MTHFloatSign_C(v53);
                    v11 = -v70;
                    v39.x = v39.x * v11;
                    v39.y = v39.y * v11;
                    v39.z = v39.z * v11;
                    AI_EvalFunc_MATHVecSetNormVector_C(&v40, v77->f_mon_rayon_daction);
                    v37 = a1.z * v55.z + a1.y * v55.y + v55.x * a1.x;
                    v54 = AI_EvalFunc_MTHFloatSign_C(v37);
                    if (v54 < 0.0)
                    {
                        v31.x = 0.0;
                        v31.y = 0.0;
                        v31.z = 1.0;
                        AI_EvalFunc_MATHVecRotate_C(&v40, &v31, 0.40000001f, &v72);
                        v40 = v72;
                    }
                    else
                    {
                        v87.x = 0.0;
                        v87.y = 0.0;
                        v87.z = 1.0;
                        AI_EvalFunc_MATHVecRotate_C(&v40, &v87, -0.40000001f, &v73);
                        v40 = v73;
                    }
                    v10 = (tGameObjectMain*)v77->o_center;
                    if (v10)
                    {
                        if ((unsigned int)v10 <= 2)
                            v10 = AI_MainActor[(uint32_t)v10 - 1];
                        AI_EvalFunc_OBJGetPos_C(v10, &v89);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v89);
                    }
                    MATH_VecAddVec(v36, &v89, &v40);
                    v77->v_way_destpos = v36[0];
                    v58.x = 0.0;
                    v58.y = 0.0;
                    v58.z = 5.0;
                }
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(v22);
                MATH_VecAddVec(&v77->v_way_destpos, v22, &v77->v_way_sight_force);
            }
            v77->i_way_moving = 1;
            v77->f_way_blendrotate = 0.125;
            v78 = 2;
        }
    }
    else
    {
        v16 = (tGameObjectMain*)v77->o_point_attente;
        if (v16)
        {
            if ((unsigned int)v16 <= 2)
                v16 = AI_MainActor[(uint32_t)v16 - 1];
            AI_EvalFunc_OBJGetPos_C(v16, &v77->v_way_destpos);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v77->v_way_destpos);
        }
        v77->i_way_moving = 1;
        v77->f_way_blendrotate = 0.2f;
        v78 = 1;
    }
    if (v77->i_way_moving)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v77->v_way_currentpos);
        v77->i_way_etage_dest = 0;
        v84 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Waf_Appareil_Photo_gam_comportement_deplacements_Waf_Appareil_Photo_gam_exec_recherche_chemin);
        if (v84 != 1000)
            return v84;
        if (!v77->i_way_moving)
        {
            v77->i_way_force_recompute = 1;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v77->v_way_sight);
            v78 = 0;
        }
    }
    if (!AI_EvalFunc_ANIIsInBlend_C_CURRENT())
        AI_EvalFunc_ACTActionSet_C_CURRENT(v78);
    v49.x = 0.0;
    v49.y = 0.0;
    v49.z = 1.0;
    AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v77->v_way_sight, &v49);
    return 1000;
}

int Waf_Appareil_Photo_gam_tag_on()
{
    Vec3* v0; // eax
    float a4; // [esp+0h] [ebp-60h]
    int v3; // [esp+4h] [ebp-5Ch]
    tScriptStruct_ActorWafwaf* v4; // [esp+8h] [ebp-58h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_ActorWafwaf*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
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
                    v0 = (Vec3*)AI_EvalVarArray_C3((char*)&v4->__tag_2x2dyn_pos_Dimension1, 0);
                    v4->tag_flag = AI_EvalFunc_GRIDTagDyn_C(AI_gpst_CurrentGameObject, &v4->tag_vectorpos, v0, a4, v3);
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

int Waf_Appareil_Photo_gam_comportement_attrape()
{
    double v0; // st7
    unsigned int v1; // eax
    int result; // eax
    float a4; // [esp+0h] [ebp-178h]
    tScriptStruct_ActorWafwaf* v4; // [esp+8h] [ebp-170h]
    tGameObjectMain* v5; // [esp+10h] [ebp-168h]
    tGameObjectMain* v6; // [esp+18h] [ebp-160h]
    tGameObjectMain* v7; // [esp+20h] [ebp-158h]
    tGameObjectMain* v8; // [esp+28h] [ebp-150h]
    tGameObjectMain* v9; // [esp+30h] [ebp-148h]
    tGameObjectMain* v10; // [esp+34h] [ebp-144h]
    tGameObjectMain* v11; // [esp+38h] [ebp-140h]
    tAI* v12; // [esp+3Ch] [ebp-13Ch]
    tAI* v13; // [esp+40h] [ebp-138h]
    tGameObjectMain* v14; // [esp+44h] [ebp-134h]
    tGameObjectMain* v15; // [esp+48h] [ebp-130h]
    tGameObjectMain* v16; // [esp+60h] [ebp-118h]
    tGameObjectMain* v17; // [esp+68h] [ebp-110h]
    Vec3 a3; // [esp+6Ch] [ebp-10Ch] BYREF
    Vec3 v19; // [esp+7Ch] [ebp-FCh] BYREF
    Vec3 dest; // [esp+8Ch] [ebp-ECh] BYREF
    Vec3 v21; // [esp+98h] [ebp-E0h] BYREF
    Vec3 v22; // [esp+A4h] [ebp-D4h] BYREF
    float v23; // [esp+B0h] [ebp-C8h]
    Vec3 v27; // [esp+D8h] [ebp-A0h] BYREF
    float v28; // [esp+E4h] [ebp-94h]
    int v29; // [esp+E8h] [ebp-90h]
    Vec3 v30; // [esp+ECh] [ebp-8Ch] BYREF
    Vec3 v31; // [esp+F8h] [ebp-80h] BYREF
    float v32; // [esp+104h] [ebp-74h]
    Vec3 v33; // [esp+108h] [ebp-70h] BYREF
    Vec3 v34; // [esp+114h] [ebp-64h] BYREF
    Vec3 a2; // [esp+12Ch] [ebp-4Ch] BYREF
    tScriptStruct_ActorWafwaf* v37; // [esp+138h] [ebp-40h]
    unsigned int v38; // [esp+13Ch] [ebp-3Ch]
    BOOL v39; // [esp+140h] [ebp-38h]
    Vec3 v40; // [esp+144h] [ebp-34h] BYREF
    Vec3 a5; // [esp+150h] [ebp-28h] BYREF
    tGameObjectMain* v42; // [esp+15Ch] [ebp-1Ch]
    Vec3 v43; // [esp+160h] [ebp-18h] BYREF
    Vec3 v44; // [esp+16Ch] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_ActorWafwaf*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v37 = v4;
    v39 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Waf_Appareil_Photo_gam_comportement_attrape__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Waf_Appareil_Photo_gam_comportement_attrape_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_attrape_GST_Global_gao_o_joy_acteur,
            (void*)Waf_Appareil_Photo_gam_comportement_attrape_GST_Global_gao,
            136);
        Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao = AI_ul_RealResolveOneRefObject(0x71003F56u);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_i_dynplan,
            (void*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao,
            440);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_f_timedynplan,
            (void*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao,
            444);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_o_viseedynplan,
            (void*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao,
            468);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_f_zdynplan,
            (void*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao,
            472);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_f_distdynplan,
            (void*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao,
            476);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_i_contrechampdynplan,
            (void*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao,
            464);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_v_sightdynplan,
            (void*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao,
            480);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_v_offsetviseedynplan,
            (void*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao,
            448);
        Waf_Appareil_Photo_gam_comportement_attrape_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_attrape_univVars_Flag_speciaux_sauve,
            (void*)Waf_Appareil_Photo_gam_comportement_attrape_univVars,
            4488);
    }
    AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 2u, -3);
    if (!AI_EvalFunc_TIMEElapsed_C(v37->f_time_attrape, 0.85000002f))
    {
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Sally_gao, &v34);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v19);
        MATH_SubstractVec(&a3, &v34, &v19);
        v28 = TIM_gf_dt;
        a4 = 5.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
        v21.x = 0.0;
        v21.y = 0.0;
        v21.z = 1.0;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a5, &v21);
    }
    if (!v37->i_flag_sally_vers_moi)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v37->f_time_attrape, 3.0))
            goto LABEL_17;
        AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Sally_gao, &v40);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v22);
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Sally_gao, &v31);
        MATH_SubstractVec(&dest, &v22, &v31);
        AI_EvalFunc_MATHVecNormalize_C(&dest, &v44);
        v32 = v44.z * v40.z + v44.y * v40.y + v40.x * v44.x;
        if (v32 < 0.85000002)
            goto LABEL_19;
        v17 = (tGameObjectMain*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Sally_gao;
        if (Waf_Appareil_Photo_gam_comportement_attrape__PJ_Sally_gao)
        {
            if ((unsigned int)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Sally_gao <= 2)
                v17 = AI_MainActor[Waf_Appareil_Photo_gam_comportement_attrape__PJ_Sally_gao - 1];
            v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v17);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
        }
        v23 = (float)v0;
        if (v23 < 2.25)
        {
        LABEL_17:
            if (AI_EvalFunc_TIMEElapsed_C(v37->f_time_attrape, 0.30000001f))
            {
                *(uint32_t*)Waf_Appareil_Photo_gam_comportement_attrape_GST_Global_gao_o_joy_acteur = 0;
                v37->i_flag_sally_vers_moi = 1;
            }
        }
        else
        {
        LABEL_19:
            *(uint32_t*)Waf_Appareil_Photo_gam_comportement_attrape_GST_Global_gao_o_joy_acteur = (uint32_t)AI_gpst_CurrentGameObject;
        }
    }
    if (!v37->i_flag)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v37->f_time_attrape, 0.64999998f))
        {
            v16 = (tGameObjectMain*)v37->o_app_photo;
            if (v16)
            {
                if ((unsigned int)v16 <= 2)
                    v16 = AI_MainActor[(uint32_t)v16 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v16, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
            }
            *(uint32_t*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_i_dynplan = 1;
            *(uint32_t*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_f_timedynplan = 0x40000000;
            *(uint32_t*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_o_viseedynplan = (uint32_t)AI_gpst_CurrentGameObject;
            *(uint32_t*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_f_zdynplan = 1065353216;
            *(uint32_t*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_f_distdynplan = 1077936128;
            *(uint32_t*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_i_contrechampdynplan = 1;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v30);
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v43);
            v33.x = -v43.x;
            v33.y = -v43.y;
            v33.z = -v43.z;
            AI_EvalFunc_MATHVecBlendRotate_C(
                &v30,
                &v33,
                0.64999998f,
                (Vec3*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_v_sightdynplan);
            *(uint32_t*)Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_v_offsetviseedynplan = 0;
            *(uint32_t*)(Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_v_offsetviseedynplan + 4) = 0;
            *(uint32_t*)(Waf_Appareil_Photo_gam_comportement_attrape__PJ_Camera_gao_des_v_offsetviseedynplan + 8) = 1062836634;
            v37->i_flag = 1;
        }
    }
    if (v37->SF_app_photo <= 0)
        v39 = 0;
    else
        v39 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Waf_Appareil_Photo_gam_comportement_attrape_univVars_Flag_speciaux_sauve + 4),
            v37->SF_app_photo);
    if (v39)
        AI_EvalFunc_ACTActionSet_C_CURRENT(9);
    v39 = AI_EvalFunc_ARRBitTest_C(
        (int*)(Waf_Appareil_Photo_gam_comportement_attrape_univVars_Flag_speciaux_sauve + 4),
        2730);
    if (!v39)
        goto LABEL_74;
    v13 = (tAI*)v37->o_perso_a_activer_si_jai_lapp;// TODO FIXME, ez mi?
    if (v13 == WOR_gst_Universe.univAIInstance)
    {
        v12 = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        v15 = (tGameObjectMain*)v37->o_perso_a_activer_si_jai_lapp;
        if (v13)
        {
            if ((unsigned int)v13 <= 2)
                v15 = AI_MainActor[(uint32_t)v13 - 1];
            v14 = v15;
        }
        else
        {
            v14 = 0;
        }
        v12 = v14->extended->AIInstance;
    }
    *((uint32_t*)v12->instanceVarDataCurrent + 729) = 1;
    v42 = AI_gpst_CurrentGameObject;
    v11 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v11 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        v10 = v11;
    }
    else
    {
        v10 = 0;
    }
    v9 = (tGameObjectMain*)v37->o_perso_a_activer_si_jai_lapp;
    if (v9)
    {
        if ((unsigned int)v9 <= 2)
            v9 = AI_MainActor[(uint32_t)v9 - 1];
        AI_EvalFunc_OBJ_RejoinAndOrient_C(v9, v10);
    }
    else
    {
        AI_EvalFunc_OBJ_RejoinAndOrient_C(0, v10);
    }
    v8 = (tGameObjectMain*)v37->o_perso_a_activer_si_jai_lapp;
    if (v8)
    {
        if ((unsigned int)v8 <= 2)
            v8 = AI_MainActor[(uint32_t)v8 - 1];
        AI_EvalFunc_OBJGetPos_C(v8, &v27);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v27);
    }
    v7 = (tGameObjectMain*)v37->o_perso_a_activer_si_jai_lapp;
    if (v7)
    {
        if ((unsigned int)v7 <= 2)
            v7 = AI_MainActor[(uint32_t)v7 - 1];
        AI_EvalFunc_COLStartMatrixSet_C(v7, &v27);
    }
    else
    {
        AI_EvalFunc_COLStartMatrixSet_C(0, &v27);
    }
    v6 = (tGameObjectMain*)v37->o_perso_a_activer_si_jai_lapp;
    if (v6)
    {
        if ((unsigned int)v6 <= 2)
            v6 = AI_MainActor[(uint32_t)v6 - 1];
        v1 = AI_EvalFunc_OBJ_FlagsControlSet_C(v6, 0, 0x600000);
    }
    else
    {
        v1 = AI_EvalFunc_OBJ_FlagsControlSet_C(0, 0, 0x600000);
    }
    v38 = v1;
    v5 = (tGameObjectMain*)v37->o_perso_a_activer_si_jai_lapp;
    if (v5)
    {
        if ((unsigned int)v5 <= 2)
            v5 = AI_MainActor[(uint32_t)v5 - 1];
        AI_EvalFunc_ACTActionSet_C(v5, 5);
    }
    else
    {
        AI_EvalFunc_ACTActionSet_C(0, 5);
    }
    v29 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
    if (v29 == 1000)
        LABEL_74:
    result = 1000;
    else
        result = v29;
    return result;
}

int Waf_Appareil_Photo_gam_comportement_attente_SF()
{
    double v0; // st7
    float a4; // [esp+0h] [ebp-124h]
    tScriptStruct_ActorWafwaf* v3; // [esp+Ch] [ebp-118h]
    tGameObjectMain* v4; // [esp+28h] [ebp-FCh]
    tGameObjectMain* v5; // [esp+34h] [ebp-F0h]
    tGameObjectMain* v6; // [esp+38h] [ebp-ECh]
    tGameObjectMain* v7; // [esp+3Ch] [ebp-E8h]
    void* Src; // [esp+58h] [ebp-CCh]
    Vec3 v9; // [esp+5Ch] [ebp-C8h] BYREF
    Vec3 v11; // [esp+7Ch] [ebp-A8h] BYREF
    tMessage v12; // [esp+88h] [ebp-9Ch] BYREF
    Vec3 v13; // [esp+C0h] [ebp-64h] BYREF
    Vec3 a3; // [esp+CCh] [ebp-58h] BYREF
    Vec3 a2; // [esp+DCh] [ebp-48h] BYREF
    float v16; // [esp+E8h] [ebp-3Ch]
    BOOL v17; // [esp+ECh] [ebp-38h]
    float v18; // [esp+F4h] [ebp-30h]
    tScriptStruct_ActorWafwaf* v19; // [esp+F8h] [ebp-2Ch]
    BOOL v20; // [esp+FCh] [ebp-28h]
    Vec3 v21; // [esp+100h] [ebp-24h] BYREF
    Vec3 a5; // [esp+118h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_ActorWafwaf*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v19 = v3;
    v17 = 0;
    v20 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Waf_Appareil_Photo_gam_comportement_attente_SF_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_attente_SF_univVars_Flag_speciaux_sauve,
            (void*)Waf_Appareil_Photo_gam_comportement_attente_SF_univVars,
            4488);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao_current_pm_mode,
            (void*)Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao_current_pm_priority,
            (void*)Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao,
            576);
        Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao_i_sally_priority,
            (void*)Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao,
            396);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao_o_sally_contextuel,
            (void*)Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao,
            388);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao_i_sally_action,
            (void*)Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao,
            412);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao_v_sally_offset,
            (void*)Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao,
            400);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao_i_force_interface,
            (void*)Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao,
            3292);
        fct_Waf_Appareil_Photo_gam_comportement_attente_SF_Waf_Appareil_Photo_gam_comportement_deplacements = (int)AI_ul_RealResolveOneRefFunction(0x26000156u);
    }
    if (*AI_gppst_CurrentJumpNode != (tAINode*)9)
    {
        if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
            goto LABEL_41;
        v7 = (tGameObjectMain*)Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao;
        if (Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao)
        {
            if ((unsigned int)Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao <= 2)
                v7 = AI_MainActor[Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao - 1];
            v6 = v7;
        }
        else
        {
            v6 = 0;
        }
        v5 = (tGameObjectMain*)v19->o_app_photo_debut;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            v0 = AI_EvalFunc_OBJ_SqrDist_C(v5, v6);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_SqrDist_C(0, v6);
        }
        v18 = (float)v0;
        if (v18 >= 1.5)
            goto LABEL_41;
        v16 = AI_EvalFunc_DYNSpeedGet_C((tGameObjectMain*)Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao);
        if (v16 >= 1.5)
            goto LABEL_41;
        v20 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Waf_Appareil_Photo_gam_comportement_attente_SF_univVars_Flag_speciaux_sauve + 4),
            3799);
        if (!v20)
            goto LABEL_41;
        if ((*(uint32_t*)(Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao + 4) & 1) != 0)
            v17 = !*(uint32_t*)Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao_current_pm_mode
            || *(int*)Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao_current_pm_priority < 20;
        else
            v17 = 0;
        if (v17 && *(float*)Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao_i_sally_priority < 27.0)
        {
            *(uint32_t*)Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao_o_sally_contextuel = (uint32_t)AI_gpst_CurrentGameObject;
            *(uint32_t*)Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao_i_sally_priority = 0x41D80000;
            *(uint32_t*)Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao_i_sally_action = 6;
            *(uint32_t*)Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao_v_sally_offset = 0;
            *(uint32_t*)(Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao_v_sally_offset + 4) = 0;
            *(uint32_t*)(Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao_v_sally_offset + 8) = 0;
            *(uint32_t*)Waf_Appareil_Photo_gam_comportement_attente_SF_GST_Board_panel_gao_i_force_interface = 1;
        }
        Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v12, Src, sizeof(v12));
        if (v12.msgID != 10)
        {
            if (v12.msgID == 37)
            {
                AI_EvalFunc_OBJ_CapaSet_C_CURRENT(256, 0);
                v12.msgID = 37;
                v12.gao = AI_gpst_CurrentGameObject;
                v12.gao2 = 0;
                v12.Int1 = 4;
                v12.Int2 = 0;
                AI_EvalFunc_OBJ_SightGet_C(
                    (tGameObjectMain*)Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao,
                    &v11);
                v12.v1.x = -v11.x;
                v12.v1.y = -v11.y;
                v12.v1.z = -v11.z;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v12.v2);
                v12.f2 = 0.0;
                v12.f1 = 1000.0;
                AI_EvalFunc_MSGSend_C((tGameObjectMain*)Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao, &v12);
            }
            goto LABEL_41;
        }
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(128, 0);
        AI_gpst_CurrentInstance->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -0.5;
    }
    if (!AI_EvalKeyword_During_C())
    {
    LABEL_41:
        if (v19->SF_joue <= 0)
            v20 = 0;
        else
            v20 = AI_EvalFunc_ARRBitTest_C(
                (int*)(Waf_Appareil_Photo_gam_comportement_attente_SF_univVars_Flag_speciaux_sauve + 4),
                v19->SF_joue);
        if (v20)
        {
            v19->i_joujou = 1;
            v19->i_r2_pressed = 0;
            v19->f_time_message_r2 = TIM_gf_MainClock - TIM_gf_LockTime;
            AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 1u, 0);
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Waf_Appareil_Photo_gam_comportement_attente_SF_Waf_Appareil_Photo_gam_comportement_deplacements);
        }
        else
        {
            AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, 1);
        }
        return 1000;
    }
    AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao, &a2);
    v4 = (tGameObjectMain*)v19->o_app_photo_debut;
    if (v4)
    {
        if ((unsigned int)v4 <= 2)
            v4 = AI_MainActor[(uint32_t)v4 - 1];
        AI_EvalFunc_OBJGetPos_C(v4, &v21);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v21);
    }
    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao, &v9);
    MATH_SubstractVec(&a3, &v21, &v9);
    a4 = 5.0f * TIM_gf_dt;
    AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
    v13.x = 0.0;
    v13.y = 0.0;
    v13.z = 1.0;
    AI_EvalFunc_OBJ_BankingGeneralSet_C(
        (tGameObjectMain*)Waf_Appareil_Photo_gam_comportement_attente_SF__PJ_Sally_gao,
        &a5,
        &v13);
    *AI_gppst_CurrentJumpNode = (tAINode*)9;
    return 0;
}

int Waf_Appareil_Photo_gam_after_rec()
{
    tScriptStruct_ActorWafwaf* v1; // [esp+0h] [ebp-168h]
    tGameObjectMain* v2; // [esp+8h] [ebp-160h]
    tGameObjectMain* v3; // [esp+10h] [ebp-158h]
    tGameObjectMain* v4; // [esp+18h] [ebp-150h]
    tGameObjectMain* v5; // [esp+28h] [ebp-140h]
    tGameObjectMain* v6; // [esp+30h] [ebp-138h]
    tGameObjectMain* v7; // [esp+40h] [ebp-128h]
    tGameObjectMain* v8; // [esp+48h] [ebp-120h]
    tGameObjectMain* v9; // [esp+50h] [ebp-118h]
    tGameObjectMain* v10; // [esp+58h] [ebp-110h]
    tGameObjectMain* v11; // [esp+68h] [ebp-100h]
    tGameObjectMain* v12; // [esp+70h] [ebp-F8h]
    tGameObjectMain* v13; // [esp+78h] [ebp-F0h]
    tGameObjectMain* v14; // [esp+94h] [ebp-D4h]
    tGameObjectMain* v15; // [esp+9Ch] [ebp-CCh]
    tGameObjectMain* v16; // [esp+A0h] [ebp-C8h]
    Vec3 a3; // [esp+ACh] [ebp-BCh] BYREF
    float v18; // [esp+B8h] [ebp-B0h]
    float v19; // [esp+BCh] [ebp-ACh]
    float v20; // [esp+C0h] [ebp-A8h]
    float v21; // [esp+C4h] [ebp-A4h]
    float v22; // [esp+C8h] [ebp-A0h]
    float v23; // [esp+CCh] [ebp-9Ch]
    Vec3 v24; // [esp+D0h] [ebp-98h] BYREF
    Vec3 v25; // [esp+DCh] [ebp-8Ch] BYREF
    Vec3 v26; // [esp+E8h] [ebp-80h] BYREF
    Vec3 v27; // [esp+F8h] [ebp-70h] BYREF
    Vec3 v28; // [esp+104h] [ebp-64h] BYREF
    float v29; // [esp+110h] [ebp-58h]
    float v30; // [esp+114h] [ebp-54h]
    float v31; // [esp+118h] [ebp-50h]
    Vec3 v32; // [esp+11Ch] [ebp-4Ch] BYREF
    Vec3 v33; // [esp+128h] [ebp-40h] BYREF
    tScriptStruct_ActorWafwaf* v34; // [esp+13Ch] [ebp-2Ch]
    BOOL v35; // [esp+140h] [ebp-28h]
    float v36; // [esp+144h] [ebp-24h]
    float v37; // [esp+148h] [ebp-20h]
    float v38; // [esp+14Ch] [ebp-1Ch]
    Vec3 v39; // [esp+150h] [ebp-18h] BYREF
    Vec3 v40; // [esp+15Ch] [ebp-Ch] BYREF

    v24.x = 1.0;
    v24.y = 0.0;
    v24.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ActorWafwaf*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v34 = v1;
    v35 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Waf_Appareil_Photo_gam_after_rec_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Waf_Appareil_Photo_gam_after_rec_univVars_Flag_speciaux_sauve,
            (void*)Waf_Appareil_Photo_gam_after_rec_univVars,
            4488);
        Waf_Appareil_Photo_gam_after_rec__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
    }
    if (v34->SF_app_photo <= 0)
        v35 = 0;
    else
        v35 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Waf_Appareil_Photo_gam_after_rec_univVars_Flag_speciaux_sauve + 4),
            v34->SF_app_photo);
    if (!v35)
    {
        v16 = (tGameObjectMain*)v34->o_app_photo;
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
            v35 = AI_EvalFunc_ARRBitTest_C((int*)(Waf_Appareil_Photo_gam_after_rec_univVars_Flag_speciaux_sauve + 4), 2668);
            if (v35)
            {
                v35 = AI_EvalFunc_ARRBitTest_C((int*)(Waf_Appareil_Photo_gam_after_rec_univVars_Flag_speciaux_sauve + 4), 3799);
                if (!v35)
                {
                    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Waf_Appareil_Photo_gam_after_rec__PJ_Sally_gao, &a3);
                    v24 = a3;
                    if (a3.z < 60.0 || v24.z > 65.0)
                        AI_EvalFunc_ARRBitSet_C((int*)(Waf_Appareil_Photo_gam_after_rec_univVars_Flag_speciaux_sauve + 4), 3799);
                }
                v35 = AI_EvalFunc_ARRBitTest_C((int*)(Waf_Appareil_Photo_gam_after_rec_univVars_Flag_speciaux_sauve + 4), 3799);
                if (v35)
                {
                    v13 = (tGameObjectMain*)v34->o_app_photo;
                    if (v13)
                    {
                        if ((unsigned int)v13 <= 2)
                            v13 = AI_MainActor[(uint32_t)v13 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v13, 0);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
                    }
                    v12 = (tGameObjectMain*)v34->o_app_photo;
                    if (v12)
                    {
                        if ((unsigned int)v12 <= 2)
                            v12 = AI_MainActor[(uint32_t)v12 - 1];
                        AI_EvalFunc_OBJ_ZoomSet_C(v12, 1.4f);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_ZoomSet_C(0, 1.4f);
                    }
                    v11 = (tGameObjectMain*)v34->o_bones_app_photo;
                    if (v11)
                    {
                        if ((unsigned int)v11 <= 2)
                            v11 = AI_MainActor[(uint32_t)v11 - 1];
                        AI_EvalFunc_OBJGetPos_C(v11, &v28);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v28);
                    }
                    v24 = v28;
                    v10 = (tGameObjectMain*)v34->o_bones_app_photo;
                    if (v10)
                    {
                        if ((unsigned int)v10 <= 2)
                            v10 = AI_MainActor[(uint32_t)v10 - 1];
                        AI_EvalFunc_OBJ_SightGet_C(v10, &v39);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_SightGet_C(0, &v39);
                    }
                    v21 = v39.x * 0.1f;
                    v22 = v39.y * 0.1f;
                    v23 = v39.z * 0.1f;
                    v24.x = v24.x + v21;
                    v24.y = v24.y + v22;
                    v24.z = v24.z + v23;
                    v9 = (tGameObjectMain*)v34->o_bones_app_photo;
                    if (v9)
                    {
                        if ((unsigned int)v9 <= 2)
                            v9 = AI_MainActor[(uint32_t)v9 - 1];
                        AI_EvalFunc_OBJ_HorizonGet_C(v9, &v33);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_HorizonGet_C(0, &v33);
                    }
                    v36 = v33.x * 0.1f;
                    v37 = v33.y * 0.1f;
                    v38 = v33.z * 0.1f;
                    v24.x = v24.x - v36;
                    v24.y = v24.y - v37;
                    v24.z = v24.z - v38;
                    v8 = (tGameObjectMain*)v34->o_app_photo;
                    if (v8)
                    {
                        if ((unsigned int)v8 <= 2)
                            v8 = AI_MainActor[(uint32_t)v8 - 1];
                        AI_EvalFunc_OBJSetPos_C(v8, &v24);
                    }
                    else
                    {
                        AI_EvalFunc_OBJSetPos_C(0, &v24);
                    }
                    v7 = (tGameObjectMain*)v34->o_bones_app_photo;
                    if (v7)
                    {
                        if ((unsigned int)v7 <= 2)
                            v7 = AI_MainActor[(uint32_t)v7 - 1];
                        AI_EvalFunc_OBJ_SightGet_C(v7, &v26);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_SightGet_C(0, &v26);
                    }
                    v24 = v26;
                    v6 = (tGameObjectMain*)v34->o_bones_app_photo;
                    if (v6)
                    {
                        if ((unsigned int)v6 <= 2)
                            v6 = AI_MainActor[(uint32_t)v6 - 1];
                        AI_EvalFunc_OBJ_HorizonGet_C(v6, &v32);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_HorizonGet_C(0, &v32);
                    }
                    v18 = v32.x * 0.60000002f;
                    v19 = v32.y * 0.60000002f;
                    v20 = v32.z * 0.60000002f;
                    v24.x = v24.x + v18;
                    v24.y = v24.y + v19;
                    v24.z = v24.z + v20;
                    v5 = (tGameObjectMain*)v34->o_bones_app_photo;
                    if (v5)
                    {
                        if ((unsigned int)v5 <= 2)
                            v5 = AI_MainActor[(uint32_t)v5 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v5, &v40);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v40);
                    }
                    v29 = v40.x * 0.80000001f;
                    v30 = v40.y * 0.80000001f;
                    v31 = v40.z * 0.80000001f;
                    v24.x = v24.x + v29;
                    v24.y = v24.y + v30;
                    v24.z = v24.z + v31;
                    v25.x = -v24.x;
                    v25.y = -v24.y;
                    v25.z = -v24.z;
                    v4 = (tGameObjectMain*)v34->o_bones_app_photo;
                    if (v4)
                    {
                        if ((unsigned int)v4 <= 2)
                            v4 = AI_MainActor[(uint32_t)v4 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v4, &v27);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v27);
                    }
                    v3 = (tGameObjectMain*)v34->o_app_photo;
                    if (v3)
                    {
                        if ((unsigned int)v3 <= 2)
                            v3 = AI_MainActor[(uint32_t)v3 - 1];
                        AI_EvalFunc_OBJ_SightGeneralSet_C(v3, &v25, &v27);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_SightGeneralSet_C(0, &v25, &v27);
                    }
                }
                else
                {
                    v2 = (tGameObjectMain*)v34->o_app_photo;
                    if (v2)
                    {
                        if ((unsigned int)v2 <= 2)
                            v2 = AI_MainActor[(uint32_t)v2 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v2, 1);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
                    }
                }
            }
            else
            {
                v14 = (tGameObjectMain*)v34->o_app_photo;
                if (v14)
                {
                    if ((unsigned int)v14 <= 2)
                        v14 = AI_MainActor[(uint32_t)v14 - 1];
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(v14, 1);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
                }
            }
        }
    }
    return 1000;
}

int Waf_Appareil_Photo_gam_tag_off()
{
    Vec3* v0; // eax
    float a4; // [esp+0h] [ebp-30h]
    int v3; // [esp+4h] [ebp-2Ch]
    tScriptStruct_ActorWafwaf* v4; // [esp+8h] [ebp-28h]
    BOOL v5; // [esp+2Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_ActorWafwaf*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    if (v4->tag_etage_force == -1 && AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x40))
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
                v0 = (Vec3*)AI_EvalVarArray_C3((char*)&v4->__tag_2x2dyn_pos_Dimension1, 0);
                AI_EvalFunc_GRIDTagDyn_C(AI_gpst_CurrentGameObject, &v4->tag_vectorpos, v0, a4, v3);
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

int Waf_Appareil_Photo_gam_exec_recherche_chemin()
{
    float a4; // [esp+0h] [ebp-B4Ch]
    float a4a; // [esp+0h] [ebp-B4Ch]
    float a4b; // [esp+0h] [ebp-B4Ch]
    float a7; // [esp+8h] [ebp-B44h]
    float v5; // [esp+Ch] [ebp-B40h]
    tScriptStruct_ActorWafwaf* v6; // [esp+10h] [ebp-B3Ch]
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
    Vec3 v26; // [esp+A90h] [ebp-BCh] BYREF
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
    tScriptStruct_ActorWafwaf* v37; // [esp+B0Ch] [ebp-40h]
    float v38; // [esp+B10h] [ebp-3Ch]
    float v39; // [esp+B14h] [ebp-38h]
    Vec3 v40; // [esp+B18h] [ebp-34h] BYREF
    int v41[7]; // [esp+B24h] [ebp-28h] BYREF
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
        v6 = (tScriptStruct_ActorWafwaf*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
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
            else if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 128))
            {
                v37->i_way_force_recompute = 1;
                AI_EvalFunc_COLNormalGet_C(AI_gpst_CurrentGameObject, 128, &v21);
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
                AI_EvalFunc_GRIDCptDest_C(AI_gpst_CurrentGameObject, &v37->v_way_destpos, v5, (Vec3*)v41);
                v34 = *(Vec3*)v41;
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
                (unsigned __int64)v38,
                a7,
                (unsigned __int64)v25,
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
            AI_EvalFunc_MATHVecNormalize_C(&v13, &v26);
            v39 = TIM_gf_dt;
            a4b = v37->f_way_blendrotate * (TIM_gf_dt * 30.0f);
            AI_EvalFunc_MATHVecBlendRotate_C(&v40, &v26, a4b, &v37->v_way_sight);
        }
    }
    return 1000;
}

int Waf_Appareil_Photo__gam_comportement_coince()
{
    double v0; // st7
    int result; // eax
    float a4; // [esp+0h] [ebp-10Ch]
    tScriptStruct_ActorWafwaf* v3; // [esp+Ch] [ebp-100h]
    tGameObjectMain* v4; // [esp+14h] [ebp-F8h]
    tGameObjectMain* v5; // [esp+3Ch] [ebp-D0h]
    tGameObjectMain* v6; // [esp+44h] [ebp-C8h]
    float v7; // [esp+48h] [ebp-C4h]
    Vec3 v8; // [esp+4Ch] [ebp-C0h] BYREF
    Vec3 v9; // [esp+58h] [ebp-B4h] BYREF
    float v10; // [esp+64h] [ebp-A8h]
    Vec3 v11; // [esp+68h] [ebp-A4h] BYREF
    Vec3 a1; // [esp+78h] [ebp-94h] BYREF
    Vec3 a2[2]; // [esp+84h] [ebp-88h] BYREF
    Vec3 v14; // [esp+9Ch] [ebp-70h] BYREF
    Vec3 v15; // [esp+A8h] [ebp-64h] BYREF
    Vec3 dest; // [esp+B4h] [ebp-58h] BYREF
    Vec3 v17; // [esp+C0h] [ebp-4Ch] BYREF
    float v18; // [esp+CCh] [ebp-40h]
    float v19; // [esp+D8h] [ebp-34h]
    int v20; // [esp+DCh] [ebp-30h]
    Vec3 v21; // [esp+E0h] [ebp-2Ch] BYREF
    tScriptStruct_ActorWafwaf* v22; // [esp+ECh] [ebp-20h]
    Vec3 v23; // [esp+F0h] [ebp-1Ch] BYREF
    Vec3 a3; // [esp+100h] [ebp-Ch] BYREF

    v14.x = 1.0;
    v14.y = 0.0;
    v14.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_ActorWafwaf*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v22 = v3;
    v10 = 0.0;
    v20 = 0;
    v19 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Waf_Appareil_Photo__gam_comportement_coince_Waf_Appareil_Photo_gam_comportement_deplacements = (int)AI_ul_RealResolveOneRefFunction(0x26000156u);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)4)
        goto LABEL_22;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v21);
    v6 = (tGameObjectMain*)v22->o_le_loup;
    if (v6)
    {
        if ((unsigned int)v6 <= 2)
            v6 = AI_MainActor[(uint32_t)v6 - 1];
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v6);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v7 = (float)v0;
    if (v7 < 49.0)
    {
        v5 = (tGameObjectMain*)v22->o_le_loup;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            AI_EvalFunc_OBJGetPos_C(v5, &v9);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v9);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v8);
        MATH_SubstractVec(&dest, &v9, &v8);
        AI_EvalFunc_MATHVecNormalize_C(&dest, &v23);
        v14 = v23;
        v10 = v22->v_fuite_axe1.z * v23.z + v22->v_fuite_axe1.y * v23.y + v23.x * v22->v_fuite_axe1.x;
        v19 = v22->v_fuite_axe2.z * v23.z + v22->v_fuite_axe2.y * v23.y + v23.x * v22->v_fuite_axe2.x;
        if (v10 >= (double)v19)
        {
            v14 = v22->v_fuite_axe2;
            v20 = 4;
        }
        else
        {
            v14 = v22->v_fuite_axe1;
            v20 = 5;
        }
        AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v14);
        AI_EvalFunc_ACTActionSet_C_CURRENT(v20);
    LABEL_22:
        if (AI_EvalFunc_ACTActionFinished_C_CURRENT())
        {
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Waf_Appareil_Photo__gam_comportement_coince_Waf_Appareil_Photo_gam_comportement_deplacements);
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)4;
            result = 0;
        }
        return result;
    }
    if (v7 <= 72.25)
    {
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(a2);
        v4 = (tGameObjectMain*)v22->o_le_loup;
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            AI_EvalFunc_OBJGetPos_C(v4, &v17);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v17);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v11);
        MATH_SubstractVec(&a3, &v17, &v11);
        v18 = TIM_gf_dt;
        a4 = 6.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(a2, &a3, a4, &v22->v_way_sight);
        AI_EvalFunc_ACTActionSet_C_CURRENT(3);
        v15.x = 0.0;
        v15.y = 0.0;
        v15.z = 1.0;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v22->v_way_sight, &v15);
        result = 1000;
    }
    else
    {
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Waf_Appareil_Photo__gam_comportement_coince_Waf_Appareil_Photo_gam_comportement_deplacements);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}