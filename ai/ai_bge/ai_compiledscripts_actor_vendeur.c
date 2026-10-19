#include "ai_compiledscripts_inc.h"

//Vendeur_Vendeur_init
int Vendeur_Vendeur_init_INV_manager_gao;
int Vendeur_Vendeur_init_INV_manager_gao_i_Animals_Nb;
int Vendeur_Vendeur_init_INV_manager_gao_ao_Animals;
int Vendeur_Vendeur_init_INV_manager_gao_ai_AnimalsData;
int fct_Vendeur_Vendeur_init_Vendeur_Vendeur_AfterBlend;
int Vendeur_Vendeur_init_univVars;
int Vendeur_Vendeur_init_univVars_Flag_speciaux_sauve;
int Vendeur_Vendeur_init_univVars_ai_VEND_art_qte;
int fct_Vendeur_Vendeur_init_Vendeur_Vendeur_reflex;
int fct_Vendeur_Vendeur_init_Vendeur_Vendeur_Comportements;

//Vendeur_Vendeur_reflex
int Vendeur_Vendeur_reflex_univVars;
int Vendeur_Vendeur_reflex_univVars_i_Nb_Perles;
int Vendeur_Vendeur_reflex_univVars_f_sally_fric;
int Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve;
int Vendeur_Vendeur_reflex_GST_Board_panel_gao;
int Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_snd_vendeur_loop;
int Vendeur_Vendeur_reflex_TXT_Sally_gao;
int Vendeur_Vendeur_reflex__PJ_Sally_gao;
int Vendeur_Vendeur_reflex_univVars_ai_VEND_art_qte;
int Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_commerce_perle;
int Vendeur_Vendeur_reflex__PJ_Sally_gao_current_pm_mode;
int Vendeur_Vendeur_reflex__PJ_Sally_gao_current_pm_priority;
int Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_sally_priority;
int Vendeur_Vendeur_reflex_GST_Board_panel_gao_o_sally_contextuel;
int Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_sally_action;
int Vendeur_Vendeur_reflex_GST_Board_panel_gao_v_sally_offset;
int Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_force_interface;
int Vendeur_Vendeur_reflex_GST_AffichageTexte_gao;
int Vendeur_Vendeur_reflex_GST_AffichageTexte_gao_i_affiche;
int Vendeur_Vendeur_reflex_GST_AffichageTexte_gao_message_a_traiter;
int Vendeur_Vendeur_reflex_GST_AffichageTexte_gao_i_encart;
int Vendeur_Vendeur_reflex_GST_AffichageTexte_gao_i_Sound_ID;
int Vendeur_Vendeur_reflex_GST_AffichageTexte_gao_prio_message_a_traiter;
int Vendeur_Vendeur_reflex_univVars_Perso_ID_equipement;
int Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_use_prio;
int Vendeur_Vendeur_reflex_GST_Board_panel_gao_o_use_object;
int Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_use_action;
int Vendeur_Vendeur_reflex_GST_Board_panel_gao_f_use_offset;
int Vendeur_Vendeur_reflex_univVars_i_PerleNbMax;
int Vendeur_Vendeur_reflex_univVars_ai_PerleOrder;
int Vendeur_Vendeur_reflex_univVars_objet_inventaire;
int Vendeur_Vendeur_reflex_univVars_objet_status;
int Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_max;
int Vendeur_Vendeur_reflex_univVars_objet_valeur1;
int Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_courante;
int Vendeur_Vendeur_reflex_univVars_mdisk_possessionflags;
int Vendeur_Vendeur_reflex_GST_Global_gao;
int Vendeur_Vendeur_reflex_GST_Global_gao_i_Perso_ID_a_checker;
int fct_Vendeur_Vendeur_reflex_Vendeur_Vendeur_exec_dialog;
int Vendeur_Vendeur_reflex_GST_Board_panel_gao_o_dialogue_actor;
int Vendeur_Vendeur_reflex__PJ_Camera_gao;
int Vendeur_Vendeur_reflex__PJ_Camera_Derriere_gao;
int Vendeur_Vendeur_reflex__PJ_Camera_Derriere_gao_i_sim_click;

//Vendeur_Vendeur_Comp_Bio
int Vendeur_Vendeur_Comp_Bio__PJ_Sally_gao;

//Vendeur_Vendeur_mamago
int Vendeur_Vendeur_mamago_univVars;
int Vendeur_Vendeur_mamago_univVars_Flag_speciaux_sauve;
int Vendeur_Vendeur_mamago__PJ_Sally_gao;
int Vendeur_Vendeur_mamago_GST_Board_panel_gao;
int Vendeur_Vendeur_mamago_GST_Board_panel_gao_i_dialog_phase;
int Vendeur_Vendeur_mamago_GST_Board_panel_gao_i_dialog_page_actu;

//Vendeur_Vendeur_exec_dialog
int Vendeur_Vendeur_exec_dialog__PJ_Sally_gao;
int Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao;
int Vendeur_Vendeur_exec_dialog__PJ_Sally_gao_current_pm_mode;
int Vendeur_Vendeur_exec_dialog__PJ_Sally_gao_current_pm_priority;
int Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_i_sally_priority;
int Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_o_sally_contextuel;
int Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_i_sally_action;
int Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_v_sally_offset;
int Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_i_force_interface;
int Vendeur_Vendeur_exec_dialog_univVars;
int Vendeur_Vendeur_exec_dialog_univVars_Flag_speciaux_sauve;
int Vendeur_Vendeur_exec_dialog_TXT_Sally_gao;
int Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_ai_dialog_consequences;
int Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_o_dialogue_actor;
int Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_at_dialog_questions;
int Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_at_dialog_reponses;
int Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_ai_dialog_grisage;

//Vendeur_Vendeur_Comportements
int Vendeur_Vendeur_Comportements_Vendeur_Vendeur_Comp_Bio;
int Vendeur_Vendeur_Comportements_univVars;
int Vendeur_Vendeur_Comportements_univVars_Flag_speciaux_sauve;
int fct_Vendeur_Vendeur_Comportements_Vendeur_Vendeur_Comp_Journaux;
int fct_Vendeur_Vendeur_Comportements_Vendeur_Vendeur_mamago;
int fct_Vendeur_Vendeur_Comportements_Vendeur_Vendeur_Comp_Simple;

//Vendeur_Vendeur_Comp_Simple
int Vendeur_Vendeur_Comp_Simple_GST_Board_panel_gao;
int Vendeur_Vendeur_Comp_Simple_GST_Board_panel_gao_o_dialogue_actor;
int Vendeur_Vendeur_Comp_Simple__PJ_Sally_gao;

//Vendeur_Vendeur_Comp_Journaux
int Vendeur_Vendeur_Comp_Journaux_univVars;
int Vendeur_Vendeur_Comp_Journaux_univVars_Flag_speciaux_sauve;
int Vendeur_Vendeur_Comp_Journaux_GST_Board_panel_gao;
int Vendeur_Vendeur_Comp_Journaux_GST_Board_panel_gao_o_dialogue_actor;
int Vendeur_Vendeur_Comp_Journaux__PJ_Sally_gao;

int Vendeur_Vendeur_init()
{
    int result; // eax
    float X; // [esp+0h] [ebp-84h]
    tScriptStruct_ActorVendeur* v2; // [esp+4h] [ebp-80h]
    tGameObjectMain* v3; // [esp+Ch] [ebp-78h]
    tGameObjectMain* v4; // [esp+14h] [ebp-70h]
    tGameObjectMain* v5; // [esp+1Ch] [ebp-68h]
    tGameObjectMain* v6; // [esp+20h] [ebp-64h]
    tGameObjectMain* v7; // [esp+2Ch] [ebp-58h]
    tGameObjectMain* v8; // [esp+30h] [ebp-54h]
    tGameObjectMain* v9; // [esp+38h] [ebp-4Ch]
    tGameObjectMain* v10; // [esp+3Ch] [ebp-48h]
    tGameObjectMain* v11; // [esp+40h] [ebp-44h]
    tGameObjectMain* v12; // [esp+44h] [ebp-40h]
    tGameObjectMain* v13; // [esp+48h] [ebp-3Ch]
    int i; // [esp+60h] [ebp-24h]
    int v15; // [esp+68h] [ebp-1Ch]
    int a3; // [esp+70h] [ebp-14h]
    int a3a; // [esp+70h] [ebp-14h]
    BOOL v18; // [esp+7Ch] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorVendeur*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Vendeur_Vendeur_init_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_init_INV_manager_gao_i_Animals_Nb,
            (void*)Vendeur_Vendeur_init_INV_manager_gao,
            23888);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_init_INV_manager_gao_ao_Animals,
            (void*)Vendeur_Vendeur_init_INV_manager_gao,
            23368);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_init_INV_manager_gao_ai_AnimalsData,
            (void*)Vendeur_Vendeur_init_INV_manager_gao,
            23628);
        fct_Vendeur_Vendeur_init_Vendeur_Vendeur_AfterBlend = (int)AI_ul_RealResolveOneRefFunction(0x26000C9Bu);
        Vendeur_Vendeur_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&Vendeur_Vendeur_init_univVars_Flag_speciaux_sauve, (void*)Vendeur_Vendeur_init_univVars, 4488);
        AI2C_ResolveBuffer(&Vendeur_Vendeur_init_univVars_ai_VEND_art_qte, (void*)Vendeur_Vendeur_init_univVars, 7668);
        fct_Vendeur_Vendeur_init_Vendeur_Vendeur_reflex = (int)AI_ul_RealResolveOneRefFunction(0x72000A2Bu);
        fct_Vendeur_Vendeur_init_Vendeur_Vendeur_Comportements = (int)AI_ul_RealResolveOneRefFunction(0x72000A2Cu);
    }
    if (v2->des_i_animaux != -1)
    {
        AI_EvalFunc_OBJ_InfoPhotoParamSet_C(
            AI_gpst_CurrentGameObject,
            4,
            v2->des_i_animaux,
            0,
            0,
            0.15000001f,
            1.0,
            0.89999998f,
            0.5);
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
        v12->extended->fields->designFlags = 0x40;
        if (*(int*)Vendeur_Vendeur_init_INV_manager_gao_i_Animals_Nb < 64)
        {
            for (i = 0; i < *(int32_t*)Vendeur_Vendeur_init_INV_manager_gao_i_Animals_Nb; ++i)
            {
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
                if (*(tGameObjectMain**)(Vendeur_Vendeur_init_INV_manager_gao_ao_Animals + 4 * i + 4) == v10)
                    break;
            }
            if (i == *(uint32_t*)Vendeur_Vendeur_init_INV_manager_gao_i_Animals_Nb)
            {
                *(uint32_t*)(Vendeur_Vendeur_init_INV_manager_gao_ao_Animals
                    + 4 * *(uint32_t*)Vendeur_Vendeur_init_INV_manager_gao_i_Animals_Nb
                    + 4) = (uint32_t)AI_gpst_CurrentGameObject;
                *(uint32_t*)(Vendeur_Vendeur_init_INV_manager_gao_ai_AnimalsData
                    + 4 * (*(uint32_t*)Vendeur_Vendeur_init_INV_manager_gao_i_Animals_Nb)++
                    + 4) = v2->des_i_animaux + (v2->des_i_etage << 8);
            }
        }
    }
    AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x8000, 0);
    if (AI_EvalFunc_ACTHasKit_C(AI_gpst_CurrentGameObject) && v2->i_active_la_rotationtete)
    {
        v9 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v9 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v9,
                3,
                (tAIFunction*)fct_Vendeur_Vendeur_init_Vendeur_Vendeur_AfterBlend);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                3,
                (tAIFunction*)fct_Vendeur_Vendeur_init_Vendeur_Vendeur_AfterBlend);
        }
        v2->f_dist_regarde_sally = v2->f_dist_regarde_sally * v2->f_dist_regarde_sally;
    }
    v8 = (tGameObjectMain*)v2->des_o_cam_wp;
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
    if (v7)
        AI_EvalFunc_AIPrioritySet_C(AI_gpst_CurrentGameObject, 255);
    if (v2->des_i_vendeur_ID + 950 <= 0)
        v18 = 0;
    else
        v18 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Vendeur_Vendeur_init_univVars_Flag_speciaux_sauve + 4),
            v2->des_i_vendeur_ID + 950);
    if (!v18)
    {
        for (a3 = 0; a3 < 10; ++a3)
            *(uint32_t*)AI_EvalVarArray2_C(1, (char*)Vendeur_Vendeur_init_univVars_ai_VEND_art_qte, a3, v2->des_i_vendeur_ID) = v2->des_i_art_qte[a3];
        if (v2->des_i_vendeur_ID + 950 > 0)
            AI_EvalFunc_ARRBitSet_C(
                (int*)(Vendeur_Vendeur_init_univVars_Flag_speciaux_sauve + 4),
                v2->des_i_vendeur_ID + 950);
    }
    for (a3a = 0; a3a < 10; ++a3a)
    {
        v6 = (tGameObjectMain*)v2->des_o_art_wp[a3a];
        if (v6)
        {
            if ((unsigned int)v6 <= 2)
                v6 = AI_MainActor[(uint32_t)v6 - 1];
            v5 = v6;
        }
        else
        {
            v5 = 0;
        }
        if (v5)
        {
            ++v2->i_nb_articles;
            if (*(uint32_t*)AI_EvalVarArray2_C(
                1,
                (char*)Vendeur_Vendeur_init_univVars_ai_VEND_art_qte,
                a3a,
                v2->des_i_vendeur_ID))
            {
                if (v2->des_i_art_if_SF[a3a])
                {
                    if (v2->des_i_art_if_SF[a3a] <= 0)
                        v2->i_show_art[a3a] = 0;
                    else
                        v2->i_show_art[a3a] = AI_EvalFunc_ARRBitTest_C(
                            (int*)(Vendeur_Vendeur_init_univVars_Flag_speciaux_sauve + 4),
                            v2->des_i_art_if_SF[a3a]);
                }
                else
                {
                    v2->i_show_art[a3a] = 1;
                }
            }
            else
            {
                v2->i_show_art[a3a] = 0;
            }
            if (v2->i_show_art[a3a])
            {
                v3 = (tGameObjectMain*)v2->des_o_art_wp[a3a];
                if (v3)
                {
                    if ((unsigned int)v3 <= 2)
                        v3 = AI_MainActor[(uint32_t)v3 - 1];
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(v3, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
                }
            }
            else
            {
                v4 = (tGameObjectMain*)v2->des_o_art_wp[a3a];
                if (v4)
                {
                    if ((unsigned int)v4 <= 2)
                        v4 = AI_MainActor[(uint32_t)v4 - 1];
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(v4, 1);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
                }
            }
        }
    }
    v2->des_f_dist_buy = v2->des_f_dist_buy * v2->des_f_dist_buy;
    v2->des_f_distance_talk = v2->des_f_distance_talk * v2->des_f_distance_talk;
    X = v2->des_f_angle_talk * 0.017453f;
    v2->des_f_angle_talk = cosf(X);
    AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_Vendeur_Vendeur_init_Vendeur_Vendeur_reflex);
    v15 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Vendeur_Vendeur_init_Vendeur_Vendeur_Comportements);
    if (v15 == 1000)
        result = 1000;
    else
        result = v15;
    return result;
}

int Vendeur_Vendeur_reflex()
{
    BOOL v1; // eax
    int v2; // eax
    int v3; // eax
    BOOL v4; // eax
    double v5; // st7
    double v6; // st7
    uint32_t* v7; // eax
    int v8; // esi
    uint32_t* v9; // eax
    int v10; // esi
    uint32_t* v11; // eax
    uint32_t* v12; // ecx
    tGameObjectMain* v13; // eax
    float v14; // [esp+1Ch] [ebp-7A8h]
    float v15; // [esp+20h] [ebp-7A4h]
    int v16; // [esp+24h] [ebp-7A0h]
    uint32_t* v17; // [esp+28h] [ebp-79Ch]
    uint32_t* v18; // [esp+2Ch] [ebp-798h]
    float v19; // [esp+30h] [ebp-794h]
    float v20; // [esp+34h] [ebp-790h]
    int v21; // [esp+38h] [ebp-78Ch]
    float v22; // [esp+3Ch] [ebp-788h]
    float v23; // [esp+40h] [ebp-784h]
    int v24; // [esp+44h] [ebp-780h]
    char* v25; // [esp+48h] [ebp-77Ch]
    uint32_t* v26; // [esp+4Ch] [ebp-778h]
    uint32_t* v27; // [esp+50h] [ebp-774h]
    float v28; // [esp+54h] [ebp-770h]
    float v29; // [esp+58h] [ebp-76Ch]
    int v30; // [esp+5Ch] [ebp-768h]
    float v31; // [esp+60h] [ebp-764h]
    float v32; // [esp+64h] [ebp-760h]
    int v33; // [esp+68h] [ebp-75Ch]
    char* v34; // [esp+6Ch] [ebp-758h]
    uint32_t* v35; // [esp+70h] [ebp-754h]
    uint32_t* v36; // [esp+8Ch] [ebp-738h]
    float v37; // [esp+90h] [ebp-734h]
    float v38; // [esp+94h] [ebp-730h]
    float v39; // [esp+98h] [ebp-72Ch]
    float v40; // [esp+9Ch] [ebp-728h]
    float v41; // [esp+A0h] [ebp-724h]
    float v42; // [esp+A4h] [ebp-720h]
    float v43; // [esp+A8h] [ebp-71Ch]
    float v44; // [esp+ACh] [ebp-718h]
    float v45; // [esp+B0h] [ebp-714h]
    tScriptStruct_ActorVendeur* v46; // [esp+B8h] [ebp-70Ch]
    tGameObjectMain* v47; // [esp+C0h] [ebp-704h]
    tGameObjectMain* v48; // [esp+C4h] [ebp-700h]
    tGameObjectMain* v49; // [esp+C8h] [ebp-6FCh]
    tGameObjectMain* v50; // [esp+CCh] [ebp-6F8h]
    tGameObjectMain* v51; // [esp+D0h] [ebp-6F4h]
    uint32_t* v52; // [esp+DCh] [ebp-6E8h]
    tGameObjectMain* v53; // [esp+E8h] [ebp-6DCh]
    tGameObjectMain* v54; // [esp+ECh] [ebp-6D8h]
    tGameObjectMain* v55; // [esp+F0h] [ebp-6D4h]
    tGameObjectMain* v56; // [esp+F8h] [ebp-6CCh]
    tGameObjectMain* v57; // [esp+100h] [ebp-6C4h]
    tGameObjectMain* v58; // [esp+104h] [ebp-6C0h]
    tGameObjectMain* v59; // [esp+108h] [ebp-6BCh]
    int* v60; // [esp+110h] [ebp-6B4h]
    int* v61; // [esp+114h] [ebp-6B0h]
    int* v62; // [esp+118h] [ebp-6ACh]
    uint32_t* v63; // [esp+11Ch] [ebp-6A8h]
    int* v64; // [esp+124h] [ebp-6A0h]
    int* v65; // [esp+128h] [ebp-69Ch]
    int* v66; // [esp+12Ch] [ebp-698h]
    uint32_t* v67; // [esp+130h] [ebp-694h]
    tGameObjectMain* v68; // [esp+144h] [ebp-680h]
    int* v69; // [esp+150h] [ebp-674h]
    int* v70; // [esp+154h] [ebp-670h]
    int* v71; // [esp+158h] [ebp-66Ch]
    uint32_t* v72; // [esp+15Ch] [ebp-668h]
    int* v73; // [esp+164h] [ebp-660h]
    int* v74; // [esp+168h] [ebp-65Ch]
    int* v75; // [esp+16Ch] [ebp-658h]
    uint32_t* v76; // [esp+170h] [ebp-654h]
    tGameObjectMain* v77; // [esp+1A8h] [ebp-61Ch]
    tGameObjectMain* v78; // [esp+1ACh] [ebp-618h]
    tGameObjectMain* v79; // [esp+1B0h] [ebp-614h]
    tGameObjectMain* v80; // [esp+1B8h] [ebp-60Ch]
    tGameObjectMain* v81; // [esp+1C0h] [ebp-604h]
    tGameObjectMain* v82; // [esp+1C4h] [ebp-600h]
    tGameObjectMain* v83; // [esp+1C8h] [ebp-5FCh]
    tGameObjectMain* v84; // [esp+1D0h] [ebp-5F4h]
    tGameObjectMain* v85; // [esp+1D4h] [ebp-5F0h]
    tGameObjectMain* v86; // [esp+1D8h] [ebp-5ECh]
    tGameObjectMain* v87; // [esp+1E0h] [ebp-5E4h]
    tGameObjectMain* v88; // [esp+1E8h] [ebp-5DCh]
    tGameObjectMain* v89; // [esp+1F4h] [ebp-5D0h]
    double v90; // [esp+20Ch] [ebp-5B8h]
    tGameObjectMain* v91; // [esp+220h] [ebp-5A4h]
    tGameObjectMain* v92; // [esp+23Ch] [ebp-588h]
    tGameObjectMain* v93; // [esp+258h] [ebp-56Ch]
    tGameObjectMain* v94; // [esp+260h] [ebp-564h]
    tGameObjectMain* v95; // [esp+268h] [ebp-55Ch]
    tGameObjectMain* v96; // [esp+26Ch] [ebp-558h]
    tGameObjectMain* v97; // [esp+270h] [ebp-554h]
    tGameObjectMain* v98; // [esp+278h] [ebp-54Ch]
    tGameObjectMain* v99; // [esp+27Ch] [ebp-548h]
    tGameObjectMain* v100; // [esp+280h] [ebp-544h]
    tGameObjectMain* v101; // [esp+288h] [ebp-53Ch]
    tGameObjectMain* v102; // [esp+28Ch] [ebp-538h]
    tGameObjectMain* v103; // [esp+290h] [ebp-534h]
    tGameObjectMain* v104; // [esp+298h] [ebp-52Ch]
    tGameObjectMain* v105; // [esp+29Ch] [ebp-528h]
    tGameObjectMain* v106; // [esp+2A0h] [ebp-524h]
    tGameObjectMain* v107; // [esp+2A8h] [ebp-51Ch]
    tGameObjectMain* v108; // [esp+2ACh] [ebp-518h]
    tGameObjectMain* v109; // [esp+2B0h] [ebp-514h]
    tGameObjectMain* v110; // [esp+2B8h] [ebp-50Ch]
    tGameObjectMain* v111; // [esp+2BCh] [ebp-508h]
    tGameObjectMain* v112; // [esp+2C0h] [ebp-504h]
    tGameObjectMain* v113; // [esp+2C8h] [ebp-4FCh]
    tGameObjectMain* v114; // [esp+2CCh] [ebp-4F8h]
    tGameObjectMain* v115; // [esp+2D0h] [ebp-4F4h]
    tGameObjectMain* v116; // [esp+2D8h] [ebp-4ECh]
    tGameObjectMain* v117; // [esp+2DCh] [ebp-4E8h]
    tGameObjectMain* v118; // [esp+2E0h] [ebp-4E4h]
    tGameObjectMain* v119; // [esp+2E8h] [ebp-4DCh]
    tGameObjectMain* v120; // [esp+2ECh] [ebp-4D8h]
    tGameObjectMain* v121; // [esp+2F0h] [ebp-4D4h]
    tGameObjectMain* v122; // [esp+2F8h] [ebp-4CCh]
    tGameObjectMain* v123; // [esp+2FCh] [ebp-4C8h]
    tGameObjectMain* v124; // [esp+300h] [ebp-4C4h]
    tGameObjectMain* v125; // [esp+308h] [ebp-4BCh]
    tGameObjectMain* v126; // [esp+310h] [ebp-4B4h]
    tGameObjectMain* v127; // [esp+314h] [ebp-4B0h]
    tGameObjectMain* v128; // [esp+320h] [ebp-4A4h]
    tGameObjectMain* v129; // [esp+324h] [ebp-4A0h]
    tGameObjectMain* v130; // [esp+328h] [ebp-49Ch]
    tGameObjectMain* v131; // [esp+330h] [ebp-494h]
    tGameObjectMain* v132; // [esp+334h] [ebp-490h]
    tGameObjectMain* v133; // [esp+338h] [ebp-48Ch]
    tGameObjectMain* v134; // [esp+33Ch] [ebp-488h]
    tGameObjectMain* v135; // [esp+340h] [ebp-484h]
    tGameObjectMain* v136; // [esp+348h] [ebp-47Ch]
    tGameObjectMain* v137; // [esp+350h] [ebp-474h]
    tGameObjectMain* v138; // [esp+384h] [ebp-440h]
    tGameObjectMain* v139; // [esp+388h] [ebp-43Ch]
    double v140; // [esp+38Ch] [ebp-438h]
    double v141; // [esp+394h] [ebp-430h]
    double v142; // [esp+39Ch] [ebp-428h]
    double v143; // [esp+3A4h] [ebp-420h]
    tGameObjectMain* v144; // [esp+3B4h] [ebp-410h]
    tGameObjectMain* v145; // [esp+3BCh] [ebp-408h]
    tGameObjectMain* v146; // [esp+3DCh] [ebp-3E8h]
    float v147; // [esp+420h] [ebp-3A4h]
    tMessage Src; // [esp+424h] [ebp-3A0h] BYREF
    float v149; // [esp+478h] [ebp-34Ch]
    int v150; // [esp+480h] [ebp-344h]
    tGameObjectMain* v153; // [esp+4BCh] [ebp-308h]
    void* v154; // [esp+4C4h] [ebp-300h]
    BOOL v155; // [esp+4E0h] [ebp-2E4h]
    int v156; // [esp+4E8h] [ebp-2DCh]
    float v157; // [esp+4ECh] [ebp-2D8h]
    int v158; // [esp+4F0h] [ebp-2D4h]
    int v159; // [esp+4F4h] [ebp-2D0h]
    int v160; // [esp+4FCh] [ebp-2C8h]
    Vec3 v161; // [esp+500h] [ebp-2C4h]
    int v162; // [esp+518h] [ebp-2ACh]
    BOOL v163; // [esp+51Ch] [ebp-2A8h]
    Vec3 dest; // [esp+520h] [ebp-2A4h] BYREF
    int v165; // [esp+530h] [ebp-294h]
    Vec3 v166; // [esp+550h] [ebp-274h] BYREF
    tGameObjectMain* v167; // [esp+560h] [ebp-264h]
    int v168; // [esp+564h] [ebp-260h]
    Vec3 v169; // [esp+574h] [ebp-250h] BYREF
    Vec3 v170; // [esp+580h] [ebp-244h] BYREF
    int v171; // [esp+5A4h] [ebp-220h]
    int v172; // [esp+5A8h] [ebp-21Ch]
    tMessage v173; // [esp+5B0h] [ebp-214h] BYREF
    Vec3 v174; // [esp+5F8h] [ebp-1CCh] BYREF
    float v175; // [esp+604h] [ebp-1C0h]
    int v176; // [esp+608h] [ebp-1BCh]
    int v177; // [esp+60Ch] [ebp-1B8h]
    int v178; // [esp+610h] [ebp-1B4h]
    BOOL v179; // [esp+614h] [ebp-1B0h]
    int v180; // [esp+618h] [ebp-1ACh]
    BOOL v181; // [esp+61Ch] [ebp-1A8h]
    int v182; // [esp+620h] [ebp-1A4h]
    BOOL v183; // [esp+628h] [ebp-19Ch]
    int v184; // [esp+62Ch] [ebp-198h]
    int v185; // [esp+630h] [ebp-194h]
    int v186; // [esp+634h] [ebp-190h]
    Vec3 v187; // [esp+644h] [ebp-180h] BYREF
    float v188; // [esp+65Ch] [ebp-168h]
    int v189; // [esp+660h] [ebp-164h]
    int v190; // [esp+664h] [ebp-160h]
    int v191; // [esp+668h] [ebp-15Ch]
    Vec3 v192; // [esp+688h] [ebp-13Ch]
    Vec3 v193; // [esp+694h] [ebp-130h]
    Vec3 v194; // [esp+6ACh] [ebp-118h]
    Vec3 v195; // [esp+6B8h] [ebp-10Ch] BYREF
    BOOL v196; // [esp+6C8h] [ebp-FCh]
    int v198; // [esp+6D0h] [ebp-F4h]
    int v199; // [esp+6D4h] [ebp-F0h]
    int v200; // [esp+6D8h] [ebp-ECh]
    int v201; // [esp+6E0h] [ebp-E4h]
    float v202; // [esp+6E4h] [ebp-E0h]
    Vec3 v203; // [esp+6E8h] [ebp-DCh] BYREF
    Vec3 v204; // [esp+6F4h] [ebp-D0h] BYREF
    void* v205; // [esp+718h] [ebp-ACh]
    int v206; // [esp+71Ch] [ebp-A8h]
    Vec3 v207; // [esp+724h] [ebp-A0h] BYREF
    tScriptStruct_ActorVendeur* v208; // [esp+730h] [ebp-94h]
    int i; // [esp+734h] [ebp-90h]
    float v210; // [esp+738h] [ebp-8Ch]
    Vec3 v211; // [esp+754h] [ebp-70h] BYREF
    float v212; // [esp+780h] [ebp-44h]
    Vec3 a3[4]; // [esp+784h] [ebp-40h] BYREF
    int v214; // [esp+7B4h] [ebp-10h]

    v161.x = 1.0;
    v161.y = 0.0;
    v161.z = 0.0;
    v192.x = 1.0;
    v192.y = 0.0;
    v192.z = 0.0;
    v193.x = 1.0;
    v193.y = 0.0;
    v193.z = 0.0;
    v194.x = 1.0;
    v194.y = 0.0;
    v194.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v46 = (tScriptStruct_ActorVendeur*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v46 = 0;
    v208 = v46;
    v214 = 0;
    v196 = 0;
    v155 = 0;
    v198 = 0;
    v199 = 0;
    v180 = 0;
    v171 = 0;
    v181 = 0;
    v158 = 0;
    v182 = 0;
    v200 = 0;
    v183 = 0;
    v172 = 0;
    v184 = 0;
    v201 = 0;
    i = 0;
    v210 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Vendeur_Vendeur_reflex_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&Vendeur_Vendeur_reflex_univVars_i_Nb_Perles, (void*)Vendeur_Vendeur_reflex_univVars, 18220);
        AI2C_ResolveBuffer(&Vendeur_Vendeur_reflex_univVars_f_sally_fric, (void*)Vendeur_Vendeur_reflex_univVars, 1200);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve,
            (void*)Vendeur_Vendeur_reflex_univVars,
            4488);
        Vendeur_Vendeur_reflex_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_snd_vendeur_loop,
            (void*)Vendeur_Vendeur_reflex_GST_Board_panel_gao,
            3356);
        Vendeur_Vendeur_reflex_TXT_Sally_gao = AI_ul_RealResolveOneRefObject(0x7200064Eu);
        Vendeur_Vendeur_reflex__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(&Vendeur_Vendeur_reflex_univVars_ai_VEND_art_qte, (void*)Vendeur_Vendeur_reflex_univVars, 7668);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_commerce_perle,
            (void*)Vendeur_Vendeur_reflex_GST_Board_panel_gao,
            2216);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex__PJ_Sally_gao_current_pm_mode,
            (void*)Vendeur_Vendeur_reflex__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex__PJ_Sally_gao_current_pm_priority,
            (void*)Vendeur_Vendeur_reflex__PJ_Sally_gao,
            576);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_sally_priority,
            (void*)Vendeur_Vendeur_reflex_GST_Board_panel_gao,
            396);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_GST_Board_panel_gao_o_sally_contextuel,
            (void*)Vendeur_Vendeur_reflex_GST_Board_panel_gao,
            388);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_sally_action,
            (void*)Vendeur_Vendeur_reflex_GST_Board_panel_gao,
            412);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_GST_Board_panel_gao_v_sally_offset,
            (void*)Vendeur_Vendeur_reflex_GST_Board_panel_gao,
            400);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_force_interface,
            (void*)Vendeur_Vendeur_reflex_GST_Board_panel_gao,
            3292);
        Vendeur_Vendeur_reflex_GST_AffichageTexte_gao = AI_ul_RealResolveOneRefObject(0x7100E7ACu);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_GST_AffichageTexte_gao_i_affiche,
            (void*)Vendeur_Vendeur_reflex_GST_AffichageTexte_gao,
            204);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_GST_AffichageTexte_gao_message_a_traiter,
            (void*)Vendeur_Vendeur_reflex_GST_AffichageTexte_gao,
            0);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_GST_AffichageTexte_gao_i_encart,
            (void*)Vendeur_Vendeur_reflex_GST_AffichageTexte_gao,
            76);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_GST_AffichageTexte_gao_i_Sound_ID,
            (void*)Vendeur_Vendeur_reflex_GST_AffichageTexte_gao,
            64);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_GST_AffichageTexte_gao_prio_message_a_traiter,
            (void*)Vendeur_Vendeur_reflex_GST_AffichageTexte_gao,
            60);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_univVars_Perso_ID_equipement,
            (void*)Vendeur_Vendeur_reflex_univVars,
            40);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_use_prio,
            (void*)Vendeur_Vendeur_reflex_GST_Board_panel_gao,
            496);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_GST_Board_panel_gao_o_use_object,
            (void*)Vendeur_Vendeur_reflex_GST_Board_panel_gao,
            488);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_use_action,
            (void*)Vendeur_Vendeur_reflex_GST_Board_panel_gao,
            500);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_GST_Board_panel_gao_f_use_offset,
            (void*)Vendeur_Vendeur_reflex_GST_Board_panel_gao,
            504);
        AI2C_ResolveBuffer(&Vendeur_Vendeur_reflex_univVars_i_PerleNbMax, (void*)Vendeur_Vendeur_reflex_univVars, 20240);
        AI2C_ResolveBuffer(&Vendeur_Vendeur_reflex_univVars_ai_PerleOrder, (void*)Vendeur_Vendeur_reflex_univVars, 19636);
        AI2C_ResolveBuffer(&Vendeur_Vendeur_reflex_univVars_objet_inventaire, (void*)Vendeur_Vendeur_reflex_univVars, 1480);
        AI2C_ResolveBuffer(&Vendeur_Vendeur_reflex_univVars_objet_status, (void*)Vendeur_Vendeur_reflex_univVars, 3896);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_max,
            (void*)Vendeur_Vendeur_reflex_univVars,
            1096);
        AI2C_ResolveBuffer(&Vendeur_Vendeur_reflex_univVars_objet_valeur1, (void*)Vendeur_Vendeur_reflex_univVars, 4100);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_courante,
            (void*)Vendeur_Vendeur_reflex_univVars,
            1148);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_univVars_mdisk_possessionflags,
            (void*)Vendeur_Vendeur_reflex_univVars,
            6756);
        Vendeur_Vendeur_reflex_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_GST_Global_gao_i_Perso_ID_a_checker,
            (void*)Vendeur_Vendeur_reflex_GST_Global_gao,
            3288);
        fct_Vendeur_Vendeur_reflex_Vendeur_Vendeur_exec_dialog = (int)AI_ul_RealResolveOneRefFunction(0x72000A44u);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex_GST_Board_panel_gao_o_dialogue_actor,
            (void*)Vendeur_Vendeur_reflex_GST_Board_panel_gao,
            1100);
        Vendeur_Vendeur_reflex__PJ_Camera_gao = AI_ul_RealResolveOneRefObject(0x71003F56u);
        Vendeur_Vendeur_reflex__PJ_Camera_Derriere_gao = AI_ul_RealResolveOneRefObject(0x5E005FA2u);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_reflex__PJ_Camera_Derriere_gao_i_sim_click,
            (void*)Vendeur_Vendeur_reflex__PJ_Camera_Derriere_gao,
            40);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)122)
    {
        Src.msgID = 30;
        Src.Int1 = 12;
        Src.Int2 = 0;
        Src.f2 = 5.0;
        Src.v1.x = 0.0;
        Src.v1.y = 0.0;
        Src.v1.z = 0.0;
        Src.v2.x = 0.0;
        Src.v2.y = 0.0;
        Src.v2.z = 0.0;
        Src.gao = 0;
        Src.gao2 = 0;
        v87 = (tGameObjectMain*)v208->des_o_acteur_txt;
        if (v87)
        {
            if ((unsigned int)v87 <= 2)
                v87 = AI_MainActor[(uint32_t)v87 - 1];
            AI_EvalFunc_MSGSend_C(v87, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
        goto LABEL_610;
    }
    if (v208->f_time_stop_mamago != 0.0)
        v163 = AI_EvalFunc_TIMEElapsed_C(v208->f_time_stop_mamago, 5.0);
    v208->i_talk_propose = 0;
    if (v208->des_monaie)
        v184 = *(uint32_t*)Vendeur_Vendeur_reflex_univVars_i_Nb_Perles;
    else
        v184 = *(uint32_t*)Vendeur_Vendeur_reflex_univVars_f_sally_fric;
    if (v208->des_i_SF_show_lecteur_code <= 0)
        v196 = 0;
    else
        v196 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4),
            v208->des_i_SF_show_lecteur_code);
    if (v196)
    {
        v208->m_msg.msgID = 57;
        v146 = (tGameObjectMain*)v208->des_o_lecteur_code;
        if (v146)
        {
            if ((unsigned int)v146 <= 2)
                v146 = AI_MainActor[(uint32_t)v146 - 1];
            AI_EvalFunc_MSGSend_C(v146, &v208->m_msg);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &v208->m_msg);
        }
        if (v208->des_i_SF_show_lecteur_code > 0)
            AI_EvalFunc_ARRBitClear_C(
                (int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4),
                v208->des_i_SF_show_lecteur_code);
    }
    if (v208->des_i_SF_pay_x_pognes <= 0)
        v196 = 0;
    else
        v196 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4),
            v208->des_i_SF_pay_x_pognes);
    if (v196)
    {
        *(uint32_t*)Vendeur_Vendeur_reflex_univVars_f_sally_fric -= v208->i_nb_pognes;
        if (v208->des_i_SF_pay_x_pognes > 0)
            AI_EvalFunc_ARRBitClear_C(
                (int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4),
                v208->des_i_SF_pay_x_pognes);
    }
    if (v208->i_nb_pognes)
    {
        if (*(int32_t*)Vendeur_Vendeur_reflex_univVars_f_sally_fric < v208->i_nb_pognes)
        {
            if (v208->i_SF_gestion_pognes > 0)
                AI_EvalFunc_ARRBitClear_C(
                    (int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4),
                    v208->i_SF_gestion_pognes);
        }
        else if (v208->i_SF_gestion_pognes > 0)
        {
            goto LABEL_36;
        }
    }
    else if (v208->i_SF_gestion_pognes > 0)
    {
    LABEL_36:
        AI_EvalFunc_ARRBitSet_C((int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4), v208->i_SF_gestion_pognes);
        goto LABEL_37;
    }
LABEL_37:
    if (v208->i_transaction_done && v208->i_last_art_index != -1)
    {
        if (v208->des_i_art_SELL_if_SF[v208->i_last_art_index])
        {
            v145 = (tGameObjectMain*)v208->des_o_art_wp[v208->i_last_art_index];
            if (v145)
            {
                if ((unsigned int)v145 <= 2)
                    v145 = AI_MainActor[(uint32_t)v145 - 1];
                AI_EvalFunc_OBJ_CapaSet_C(v145, 0, 256);
            }
            else
            {
                AI_EvalFunc_OBJ_CapaSet_C(0, 0, 256);
            }
        }
        if (AI_EvalFunc_TIMEElapsed_C(v208->f_time_transaction_done, v208->des_f_delai_remplacement))
        {
            if (v208->i_show_art[v208->i_last_art_index])
            {
                v144 = (tGameObjectMain*)v208->des_o_art_wp[v208->i_last_art_index];
                if (v144)
                {
                    if ((unsigned int)v144 <= 2)
                        v144 = AI_MainActor[(uint32_t)v144 - 1];
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(v144, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
                }
            }
            v208->i_last_art_index = -1;
        }
    }
    if (v208->f_time_decompte != 0.0 && !AI_EvalFunc_TIMEElapsed_C(v208->f_time_decompte, 1.0))
    {
        if (v208->i_snd_loop == -1)
        {
            v208->i_snd_loop = AI_EvalFunc_SNDRequest_C((tGameObjectMain*)Vendeur_Vendeur_reflex_GST_Board_panel_gao, 10, 0);
            AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v208->i_snd_loop);
            *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_snd_vendeur_loop = v208->i_snd_loop;
        }
        v212 = TIM_gf_MainClock - TIM_gf_LockTime;
        v210 = (v212 - v208->f_time_decompte) / 1.0f;
        if (v208->i_flag_vente)
        {
            if (v208->des_monaie)
            {
                v45 = (float)v208->des_i_art_prix_vente[v208->i_last_art_index];
                v44 = (float)v208->i_last_thune;
                v143 = v44 - v45 * v210 + MATH_gd_magicDoubleForRound;
                *(uint32_t*)Vendeur_Vendeur_reflex_univVars_i_Nb_Perles = (*(uint32_t*)&v143);
            }
            else
            {
                v43 = (float)v208->des_i_art_prix_vente[v208->i_last_art_index];
                v42 = (float)v208->i_last_thune;
                v142 = v42 - v43 * v210 + MATH_gd_magicDoubleForRound;
                *(uint32_t*)Vendeur_Vendeur_reflex_univVars_f_sally_fric = (*(uint32_t*)&v142);
            }
        }
        else if (v208->des_monaie)
        {
            v41 = (float)v208->des_i_art_prix_achat[v208->i_last_art_index];
            v40 = (float)v208->i_last_thune;
            v141 = v41 * v210 + v40 + MATH_gd_magicDoubleForRound;
            *(uint32_t*)Vendeur_Vendeur_reflex_univVars_i_Nb_Perles = (*(uint32_t*)&v141);
        }
        else
        {
            v39 = (float)v208->des_i_art_prix_achat[v208->i_last_art_index];
            v38 = (float)v208->i_last_thune;
            v140 = v39 * v210 + v38 + MATH_gd_magicDoubleForRound;
            *(uint32_t*)Vendeur_Vendeur_reflex_univVars_f_sally_fric = (*(uint32_t*)&v140);
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    if (v208->i_transaction_done)
        goto LABEL_104;
    v208->i_transaction_done = 1;
    v208->f_time_transaction_done = TIM_gf_MainClock - TIM_gf_LockTime;
    if (v208->i_snd_loop != -1)
    {
        if (*(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_snd_vendeur_loop != -1)
            SND_Release(v208->i_snd_loop);
        v208->i_snd_loop = -1;
        *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_snd_vendeur_loop = -1;
    }
    if (v208->i_flag_vente)
    {
        if (v208->des_monaie)
            *(uint32_t*)Vendeur_Vendeur_reflex_univVars_i_Nb_Perles = v208->i_last_thune
            - v208->des_i_art_prix_vente[v208->i_last_art_index];
        else
            *(uint32_t*)Vendeur_Vendeur_reflex_univVars_f_sally_fric = v208->i_last_thune
            - v208->des_i_art_prix_vente[v208->i_last_art_index];
        v139 = (tGameObjectMain*)v208->des_o_acteur_txt;
        if (v139)
        {
            if ((unsigned int)v139 <= 2)
                v139 = AI_MainActor[(uint32_t)v139 - 1];
            v138 = v139;
        }
        else
        {
            v138 = 0;
        }
        if (v138 && v208->des_i_TXT_index_vente[v208->i_last_art_index] != -1)
            goto LABEL_93;
        v188 = (float)v208->des_i_art_prix_vente[v208->i_last_art_index];
        v189 = 0;
        v190 = 0;
        v194.x = v188;
        v194.y = 0.0;
        v194.z = 0.0;
        if (v208->des_monaie)
        {
            Src.msgID = 30;
            Src.Int1 = 68;
        }
        else
        {
            Src.msgID = 30;
            Src.Int1 = 49;
        }
        Src.Int2 = 0;
        Src.f2 = 1.5;
        Src.v1.x = 0.0;
        Src.v1.y = 0.0;
        Src.v1.z = 0.0;
    }
    else
    {
        if (v208->des_monaie)
            *(uint32_t*)Vendeur_Vendeur_reflex_univVars_i_Nb_Perles = v208->des_i_art_prix_achat[v208->i_last_art_index]
            + v208->i_last_thune;
        else
            *(uint32_t*)Vendeur_Vendeur_reflex_univVars_f_sally_fric = v208->des_i_art_prix_achat[v208->i_last_art_index]
            + v208->i_last_thune;
        v175 = (float)v208->des_i_art_prix_achat[v208->i_last_art_index];
        v176 = 0;
        v177 = 0;
        v194.x = v175;
        v194.y = 0.0;
        v194.z = 0.0;
        if (v208->des_monaie)
        {
            Src.msgID = 30;
            Src.Int1 = 67;
        }
        else
        {
            Src.msgID = 30;
            Src.Int1 = 48;
        }
        Src.Int2 = 0;
        Src.f2 = 1.5;
        Src.v1.x = 0.0;
        Src.v1.y = 0.0;
        Src.v1.z = 0.0;
    }
    Src.v2 = v194;
    Src.gao = 0;
    Src.gao2 = 0;
    AI_EvalFunc_MSGSend_C((tGameObjectMain*)Vendeur_Vendeur_reflex_TXT_Sally_gao, &Src);
LABEL_93:
    v137 = (tGameObjectMain*)v208->des_o_art_wp[v208->i_last_art_index];
    if (v137)
    {
        if ((unsigned int)v137 <= 2)
            v137 = AI_MainActor[(uint32_t)v137 - 1];
        v1 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v137, 0x2000000);
    }
    else
    {
        v1 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x2000000);
    }
    v179 = v1;
    if (v1)
    {
        v136 = (tGameObjectMain*)v208->des_o_art_wp[v208->i_last_art_index];
        if (v136)
        {
            if ((unsigned int)v136 <= 2)
                v136 = AI_MainActor[(uint32_t)v136 - 1];
            AI_EvalFunc_TRAPlayAll_C(v136);
        }
        else
        {
            AI_EvalFunc_TRAPlayAll_C(0);
        }
    }
LABEL_104:
    v135 = (tGameObjectMain*)v208->des_o_ma_BV;
    if (v135)
    {
        if ((unsigned int)v135 <= 2)
            v135 = AI_MainActor[(uint32_t)v135 - 1];
        v134 = v135;
    }
    else
    {
        v134 = 0;
    }
    if (v134)
    {
        v133 = (tGameObjectMain*)Vendeur_Vendeur_reflex__PJ_Sally_gao;
        if (Vendeur_Vendeur_reflex__PJ_Sally_gao)
        {
            if ((unsigned int)Vendeur_Vendeur_reflex__PJ_Sally_gao <= 2)
                v133 = AI_MainActor[Vendeur_Vendeur_reflex__PJ_Sally_gao - 1];
            v132 = v133;
        }
        else
        {
            v132 = 0;
        }
        v131 = (tGameObjectMain*)v208->des_o_ma_BV;
        if (v131)
        {
            if ((unsigned int)v131 <= 2)
                v131 = AI_MainActor[(uint32_t)v131 - 1];
            v2 = AI_EvalFunc_COLBVPivotCollide_C(v131, v132);
        }
        else
        {
            v2 = AI_EvalFunc_COLBVPivotCollide_C(0, v132);
        }
        v162 = v2;
        if (v2)
            goto LABEL_133;
    }
    v130 = (tGameObjectMain*)v208->des_o_ma_BV;
    if (v130)
    {
        if ((unsigned int)v130 <= 2)
            v130 = AI_MainActor[(uint32_t)v130 - 1];
        v129 = v130;
    }
    else
    {
        v129 = 0;
    }
    if (v129)
        goto LABEL_635;
    v128 = (tGameObjectMain*)Vendeur_Vendeur_reflex__PJ_Sally_gao;
    if (Vendeur_Vendeur_reflex__PJ_Sally_gao)
    {
        if ((unsigned int)Vendeur_Vendeur_reflex__PJ_Sally_gao <= 2)
            v128 = AI_MainActor[Vendeur_Vendeur_reflex__PJ_Sally_gao - 1];
        v3 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, v128);
    }
    else
    {
        v3 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, 0);
    }
    v168 = v3;
    if (v3)
    {
    LABEL_133:
        v208->i_reset_to_do = 1;
        if (v208->i_pas_la_peine_de_carte)
            v196 = 1;
        else
            v196 = AI_EvalFunc_ARRBitTest_C((int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4), 998);
        if (v196)
        {
            for (i = 0; i < 10; ++i)
            {
                v127 = (tGameObjectMain*)v208->des_o_art_wp[i];
                if (v127)
                {
                    if ((unsigned int)v127 <= 2)
                        v127 = AI_MainActor[(uint32_t)v127 - 1];
                    v126 = v127;
                }
                else
                {
                    v126 = 0;
                }
                if (v126)
                {
                    if (*(uint32_t*)AI_EvalVarArray2_C(
                        1,
                        (char*)Vendeur_Vendeur_reflex_univVars_ai_VEND_art_qte,
                        i,
                        v208->des_i_vendeur_ID))
                    {
                        if (v208->des_i_art_if_SF[i])
                        {
                            if (v208->des_i_art_if_SF[i] <= 0)
                            {
                                v208->i_show_art[i] = 0;
                            }
                            else
                            {
                                v4 = AI_EvalFunc_ARRBitTest_C(
                                    (int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4),
                                    v208->des_i_art_if_SF[i]);
                                v208->i_show_art[i] = v4;
                            }
                        }
                        else
                        {
                            v208->i_show_art[i] = 1;
                        }
                    }
                    else
                    {
                        v208->i_show_art[i] = 0;
                    }
                    if (v208->i_show_art[i])
                    {
                        v125 = (tGameObjectMain*)v208->des_o_art_wp[i];
                        if (v125)
                        {
                            if ((unsigned int)v125 <= 2)
                                v125 = AI_MainActor[(uint32_t)v125 - 1];
                            AI_EvalFunc_OBJ_FlagInvisibleSet_C(v125, 0);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
                        }
                        v124 = (tGameObjectMain*)v208->des_o_art_cacher1[i];
                        if (v124)
                        {
                            if ((unsigned int)v124 <= 2)
                                v124 = AI_MainActor[(uint32_t)v124 - 1];
                            v123 = v124;
                        }
                        else
                        {
                            v123 = 0;
                        }
                        if (v123)
                        {
                            v122 = (tGameObjectMain*)v208->des_o_art_cacher1[i];
                            if (v122)
                            {
                                if ((unsigned int)v122 <= 2)
                                    v122 = AI_MainActor[(uint32_t)v122 - 1];
                                AI_EvalFunc_OBJDrawMaskSet_C(v122, 8, 0);
                            }
                            else
                            {
                                AI_EvalFunc_OBJDrawMaskSet_C(0, 8, 0);
                            }
                        }
                        v121 = (tGameObjectMain*)v208->des_o_art_cacher2[i];
                        if (v121)
                        {
                            if ((unsigned int)v121 <= 2)
                                v121 = AI_MainActor[(uint32_t)v121 - 1];
                            v120 = v121;
                        }
                        else
                        {
                            v120 = 0;
                        }
                        if (v120)
                        {
                            v119 = (tGameObjectMain*)v208->des_o_art_cacher2[i];
                            if (v119)
                            {
                                if ((unsigned int)v119 <= 2)
                                    v119 = AI_MainActor[(uint32_t)v119 - 1];
                                AI_EvalFunc_OBJDrawMaskSet_C(v119, 8, 0);
                            }
                            else
                            {
                                AI_EvalFunc_OBJDrawMaskSet_C(0, 8, 0);
                            }
                        }
                        v118 = (tGameObjectMain*)v208->des_o_art_cacher3[i];
                        if (v118)
                        {
                            if ((unsigned int)v118 <= 2)
                                v118 = AI_MainActor[(uint32_t)v118 - 1];
                            v117 = v118;
                        }
                        else
                        {
                            v117 = 0;
                        }
                        if (v117)
                        {
                            v116 = (tGameObjectMain*)v208->des_o_art_cacher3[i];
                            if (v116)
                            {
                                if ((unsigned int)v116 <= 2)
                                    v116 = AI_MainActor[(uint32_t)v116 - 1];
                                AI_EvalFunc_OBJDrawMaskSet_C(v116, 8, 0);
                            }
                            else
                            {
                                AI_EvalFunc_OBJDrawMaskSet_C(0, 8, 0);
                            }
                        }
                        v115 = (tGameObjectMain*)v208->des_o_art_cacher4[i];
                        if (v115)
                        {
                            if ((unsigned int)v115 <= 2)
                                v115 = AI_MainActor[(uint32_t)v115 - 1];
                            v114 = v115;
                        }
                        else
                        {
                            v114 = 0;
                        }
                        if (v114)
                        {
                            v113 = (tGameObjectMain*)v208->des_o_art_cacher4[i];
                            if (v113)
                            {
                                if ((unsigned int)v113 <= 2)
                                    v113 = AI_MainActor[(uint32_t)v113 - 1];
                                AI_EvalFunc_OBJDrawMaskSet_C(v113, 8, 0);
                            }
                            else
                            {
                                AI_EvalFunc_OBJDrawMaskSet_C(0, 8, 0);
                            }
                        }
                        v112 = (tGameObjectMain*)v208->des_o_art_cacher5[i];
                        if (v112)
                        {
                            if ((unsigned int)v112 <= 2)
                                v112 = AI_MainActor[(uint32_t)v112 - 1];
                            v111 = v112;
                        }
                        else
                        {
                            v111 = 0;
                        }
                        if (v111)
                        {
                            v110 = (tGameObjectMain*)v208->des_o_art_cacher5[i];
                            if (v110)
                            {
                                if ((unsigned int)v110 <= 2)
                                    v110 = AI_MainActor[(uint32_t)v110 - 1];
                                AI_EvalFunc_OBJDrawMaskSet_C(v110, 8, 0);
                            }
                            else
                            {
                                AI_EvalFunc_OBJDrawMaskSet_C(0, 8, 0);
                            }
                        }
                    }
                    else
                    {
                        v109 = (tGameObjectMain*)v208->des_o_art_cacher1[i];
                        if (v109)
                        {
                            if ((unsigned int)v109 <= 2)
                                v109 = AI_MainActor[(uint32_t)v109 - 1];
                            v108 = v109;
                        }
                        else
                        {
                            v108 = 0;
                        }
                        if (v108)
                        {
                            v107 = (tGameObjectMain*)v208->des_o_art_cacher1[i];
                            if (v107)
                            {
                                if ((unsigned int)v107 <= 2)
                                    v107 = AI_MainActor[(uint32_t)v107 - 1];
                                AI_EvalFunc_OBJDrawMaskSet_C(v107, 0, 8);
                            }
                            else
                            {
                                AI_EvalFunc_OBJDrawMaskSet_C(0, 0, 8);
                            }
                        }
                        v106 = (tGameObjectMain*)v208->des_o_art_cacher2[i];
                        if (v106)
                        {
                            if ((unsigned int)v106 <= 2)
                                v106 = AI_MainActor[(uint32_t)v106 - 1];
                            v105 = v106;
                        }
                        else
                        {
                            v105 = 0;
                        }
                        if (v105)
                        {
                            v104 = (tGameObjectMain*)v208->des_o_art_cacher2[i];
                            if (v104)
                            {
                                if ((unsigned int)v104 <= 2)
                                    v104 = AI_MainActor[(uint32_t)v104 - 1];
                                AI_EvalFunc_OBJDrawMaskSet_C(v104, 0, 8);
                            }
                            else
                            {
                                AI_EvalFunc_OBJDrawMaskSet_C(0, 0, 8);
                            }
                        }
                        v103 = (tGameObjectMain*)v208->des_o_art_cacher3[i];
                        if (v103)
                        {
                            if ((unsigned int)v103 <= 2)
                                v103 = AI_MainActor[(uint32_t)v103 - 1];
                            v102 = v103;
                        }
                        else
                        {
                            v102 = 0;
                        }
                        if (v102)
                        {
                            v101 = (tGameObjectMain*)v208->des_o_art_cacher3[i];
                            if (v101)
                            {
                                if ((unsigned int)v101 <= 2)
                                    v101 = AI_MainActor[(uint32_t)v101 - 1];
                                AI_EvalFunc_OBJDrawMaskSet_C(v101, 0, 8);
                            }
                            else
                            {
                                AI_EvalFunc_OBJDrawMaskSet_C(0, 0, 8);
                            }
                        }
                        v100 = (tGameObjectMain*)v208->des_o_art_cacher4[i];
                        if (v100)
                        {
                            if ((unsigned int)v100 <= 2)
                                v100 = AI_MainActor[(uint32_t)v100 - 1];
                            v99 = v100;
                        }
                        else
                        {
                            v99 = 0;
                        }
                        if (v99)
                        {
                            v98 = (tGameObjectMain*)v208->des_o_art_cacher4[i];
                            if (v98)
                            {
                                if ((unsigned int)v98 <= 2)
                                    v98 = AI_MainActor[(uint32_t)v98 - 1];
                                AI_EvalFunc_OBJDrawMaskSet_C(v98, 0, 8);
                            }
                            else
                            {
                                AI_EvalFunc_OBJDrawMaskSet_C(0, 0, 8);
                            }
                        }
                        v97 = (tGameObjectMain*)v208->des_o_art_cacher5[i];
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
                        if (v96)
                        {
                            v95 = (tGameObjectMain*)v208->des_o_art_cacher5[i];
                            if (v95)
                            {
                                if ((unsigned int)v95 <= 2)
                                    v95 = AI_MainActor[(uint32_t)v95 - 1];
                                AI_EvalFunc_OBJDrawMaskSet_C(v95, 0, 8);
                            }
                            else
                            {
                                AI_EvalFunc_OBJDrawMaskSet_C(0, 0, 8);
                            }
                        }
                    }
                }
            }
            if (v208->i_last_art_index == -1)
            {
                *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_commerce_perle = 0;
                v158 = 0;
                for (i = 0; i < v208->i_nb_articles; ++i)
                {
                    if (v208->i_show_art[i])
                    {
                        v94 = (tGameObjectMain*)v208->des_o_art_wp[i];
                        if (v94)
                        {
                            if ((unsigned int)v94 <= 2)
                                v94 = AI_MainActor[(uint32_t)v94 - 1];
                            v5 = AI_EvalFunc_OBJ_SqrDistHorz_C((tGameObjectMain*)Vendeur_Vendeur_reflex__PJ_Sally_gao, v94);
                        }
                        else
                        {
                            v5 = AI_EvalFunc_OBJ_SqrDistHorz_C((tGameObjectMain*)Vendeur_Vendeur_reflex__PJ_Sally_gao, 0);
                        }
                        v202 = (float)v5;
                        if (v208->des_f_dist_buy > (double)v202)
                        {
                            if (v208->des_i_type)
                            {
                                v186 = (unsigned __int16)*(uint32_t*)(Vendeur_Vendeur_reflex__PJ_Sally_gao + 4);
                                if ((v186 & 1) != 0)
                                    v181 = !*(uint32_t*)Vendeur_Vendeur_reflex__PJ_Sally_gao_current_pm_mode
                                    || *(int*)Vendeur_Vendeur_reflex__PJ_Sally_gao_current_pm_priority < 20;
                                else
                                    v181 = 0;
                                v93 = (tGameObjectMain*)v208->des_o_art_wp[i];
                                if (v93)
                                {
                                    if ((unsigned int)v93 <= 2)
                                        v93 = AI_MainActor[(uint32_t)v93 - 1];
                                    AI_EvalFunc_OBJGetPos_C(v93, a3);
                                }
                                else
                                {
                                    AI_EvalFunc_OBJGetPos_C(0, a3);
                                }
                                AI_EvalFunc_OBJGetPos_C_CURRENT(&v187);
                                MATH_SubstractVec(&dest, a3, &v187);
                                v192.x = dest.x;
                                v192.y = dest.y;
                                v192.z = dest.z + 10000.0f;
                                if (v181 && *(float*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_sally_priority < 26.0)
                                {
                                    *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_o_sally_contextuel = (uint32_t)AI_gpst_CurrentGameObject;
                                    *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_sally_priority = 1104150528;
                                    *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_sally_action = 6;
                                    *(Vec3*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_v_sally_offset = v192;
                                    *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_force_interface = 1;
                                }
                                if (!*(uint32_t*)Vendeur_Vendeur_reflex_GST_AffichageTexte_gao_i_affiche)
                                {
                                    v173.msgID = 30;
                                    v173.gao = AI_gpst_CurrentGameObject;
                                    v173.gao2 = 0;
                                    v173.Int1 = 0;
                                    v173.Int2 = v208->des_i_art_prix_vente[i];
                                    v173.f2 = 1.5;
                                    v173.f1 = (float)v208->des_i_art_type[i];
                                    v173.v1.x = 0.0;
                                    v173.v1.y = 0.0;
                                    v173.v1.z = 0.0;
                                    v173.v2.x = 0.0;
                                    v173.v2.y = 0.0;
                                    v173.v2.z = 0.0;
                                    memcpy((void*)Vendeur_Vendeur_reflex_GST_AffichageTexte_gao_message_a_traiter, &v173, 0x34u);
                                    *(uint32_t*)Vendeur_Vendeur_reflex_GST_AffichageTexte_gao_i_encart = 2;
                                    *(uint32_t*)Vendeur_Vendeur_reflex_GST_AffichageTexte_gao_i_Sound_ID = -1;
                                    *(uint32_t*)Vendeur_Vendeur_reflex_GST_AffichageTexte_gao_prio_message_a_traiter = 2;
                                }
                            }
                            else if (v208->des_monaie)
                            {
                                v158 = 1;
                                v198 = *(uint32_t*)AI_EvalVarArray2_C(
                                    1,
                                    (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_equipement,
                                    0,
                                    8);
                                if (v198 != 45 && !v208->i_swap_force)
                                {
                                    v208->i_swap_force = 1;
                                    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_equipement, 0, 8) = 45;
                                    v198 = 45;
                                }
                                if (v198 == 45)
                                {
                                    v92 = (tGameObjectMain*)v208->des_o_art_wp[i];
                                    if (v92)
                                    {
                                        if ((unsigned int)v92 <= 2)
                                            v92 = AI_MainActor[(uint32_t)v92 - 1];
                                        AI_EvalFunc_OBJGetPos_C(v92, &v169);
                                    }
                                    else
                                    {
                                        AI_EvalFunc_OBJGetPos_C(0, &v169);
                                    }
                                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v207);
                                    MATH_SubstractVec(&v170, &v169, &v207);
                                    v161.x = v170.x;
                                    v161.y = v170.y;
                                    v161.z = v170.z + 10000.0f;
                                    v185 = (unsigned __int16)*(uint32_t*)(Vendeur_Vendeur_reflex__PJ_Sally_gao + 4);
                                    if ((v185 & 1) != 0)
                                        v181 = !*(uint32_t*)Vendeur_Vendeur_reflex__PJ_Sally_gao_current_pm_mode
                                        || *(int*)Vendeur_Vendeur_reflex__PJ_Sally_gao_current_pm_priority < 20;
                                    else
                                        v181 = 0;
                                    if (v181 && *(float*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_sally_priority < 28.0)
                                    {
                                        *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_o_sally_contextuel = (uint32_t)AI_gpst_CurrentGameObject;
                                        *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_sally_priority = 1105199104;
                                        *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_sally_action = 20;
                                        *(Vec3*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_v_sally_offset = v161;
                                        *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_force_interface = 1;
                                    }
                                    *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_commerce_perle = -1;
                                }
                            }
                            else
                            {
                                v160 = (unsigned __int16)*(uint32_t*)(Vendeur_Vendeur_reflex__PJ_Sally_gao + 4);
                                if ((v160 & 1) != 0)
                                    v181 = !*(uint32_t*)Vendeur_Vendeur_reflex__PJ_Sally_gao_current_pm_mode
                                    || *(int*)Vendeur_Vendeur_reflex__PJ_Sally_gao_current_pm_priority < 20;
                                else
                                    v181 = 0;
                                if (v181 && *(float*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_sally_priority < 26.0)
                                {
                                    *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_o_sally_contextuel = (uint32_t)AI_gpst_CurrentGameObject;
                                    *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_sally_priority = 1104150528;
                                    *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_sally_action = 13;
                                    *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_v_sally_offset = 0;
                                    *(uint32_t*)(Vendeur_Vendeur_reflex_GST_Board_panel_gao_v_sally_offset + 4) = 0;
                                    *(uint32_t*)(Vendeur_Vendeur_reflex_GST_Board_panel_gao_v_sally_offset + 8) = 0;
                                    *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_force_interface = 1;
                                }
                            }
                            v208->i_last_art_index = i;
                            break;
                        }
                    }
                }
                if (!v158)
                    v208->i_swap_force = 0;
            }
        }
        v91 = (tGameObjectMain*)Vendeur_Vendeur_reflex__PJ_Sally_gao;
        if (Vendeur_Vendeur_reflex__PJ_Sally_gao)
        {
            if ((unsigned int)Vendeur_Vendeur_reflex__PJ_Sally_gao <= 2)
                v91 = AI_MainActor[Vendeur_Vendeur_reflex__PJ_Sally_gao - 1];
            v6 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v91);
        }
        else
        {
            v6 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
        }
        v149 = (float)v6;
        if (v208->des_f_distance_talk > (double)v149)
        {
            v156 = (unsigned __int16)*(uint32_t*)(Vendeur_Vendeur_reflex__PJ_Sally_gao + 4);
            if ((v156 & 1) != 0)
                v181 = !*(uint32_t*)Vendeur_Vendeur_reflex__PJ_Sally_gao_current_pm_mode
                || *(int*)Vendeur_Vendeur_reflex__PJ_Sally_gao_current_pm_priority < 20;
            else
                v181 = 0;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v204);
            AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Vendeur_Vendeur_reflex__PJ_Sally_gao, &v211);
            MATH_SubstractVec(&v174, &v204, &v211);
            AI_EvalFunc_MATHVecNormalize_C(&v174, &v166);
            v193 = v166;
            AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Vendeur_Vendeur_reflex__PJ_Sally_gao, &v195);
            v157 = v193.z * v195.z + v193.y * v195.y + v195.x * v193.x;
            v147 = v157 * 5.0f;
            v90 = v147 + -10.0 + MATH_gd_magicDoubleForRound;
            v171 = (*(uint32_t*)&v90);
            if (v181)
            {
                v37 = (float)v171;
                if (v37 > (double)*(float*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_sally_priority)
                {
                    *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_o_sally_contextuel = (uint32_t)AI_gpst_CurrentGameObject;
                    *(float*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_sally_priority = (float)v171;
                    *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_sally_action = 5;
                    *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_v_sally_offset = 0;
                    *(uint32_t*)(Vendeur_Vendeur_reflex_GST_Board_panel_gao_v_sally_offset + 4) = 0;
                    *(uint32_t*)(Vendeur_Vendeur_reflex_GST_Board_panel_gao_v_sally_offset + 8) = 0x3F800000;
                    *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_force_interface = 1;
                }
            }
            v208->i_talk_propose = 1;
            if (v196)
            {
                for (i = 0; i < v208->i_nb_articles; ++i)
                {
                    if (v208->des_i_art_prix_achat[i])
                    {
                        v7 = (uint32_t*)AI_EvalVarArray2_C(1, (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_equipement, 0, 8);
                        if (*v7 == v208->des_i_art_type[i])
                        {
                            v206 = (unsigned __int16)*(uint32_t*)(Vendeur_Vendeur_reflex__PJ_Sally_gao + 4);
                            if ((v206 & 1) != 0)
                                v181 = !*(uint32_t*)Vendeur_Vendeur_reflex__PJ_Sally_gao_current_pm_mode
                                || *(int*)Vendeur_Vendeur_reflex__PJ_Sally_gao_current_pm_priority < 20;
                            else
                                v181 = 0;
                            if (v181 && *(int*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_use_prio < 25)
                            {
                                *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_o_use_object = (uint32_t)AI_gpst_CurrentGameObject;
                                *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_use_prio = 25;
                                *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_i_use_action = 7;
                                *(uint32_t*)Vendeur_Vendeur_reflex_GST_Board_panel_gao_f_use_offset = 0;
                            }
                            v208->i_last_art_index = i;
                            break;
                        }
                    }
                }
            }
        }
    }
    else
    {
    LABEL_635:
        if (v208->i_reset_to_do)
        {
            for (i = 0; i < 10; ++i)
            {
                if (v208->ai_grisage[i] > 0)
                    AI_EvalFunc_ARRBitClear_C(
                        (int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4),
                        v208->ai_grisage[i]);
            }
            v208->i_reset_to_do = 0;
        }
    }
    v205 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v208->m_msg, v205, sizeof(v208->m_msg));
    while (v208->m_msg.msgID)
    {
        if (v208->m_msg.msgID != 10 || v208->m_msg.f2 <= 0.0 || v208->des_monaie)
        {
            if (v208->m_msg.msgID == 9 || v208->des_monaie && v208->m_msg.msgID == 10 && v208->m_msg.f2 > 0.0)
            {
                if (v208->m_msg.f2 == 25.0)
                {
                    v173.msgID = 55;
                    v173.Int1 = 1;
                    v173.Int2 = v208->des_i_art_type[v208->i_last_art_index];
                    v173.f2 = 1.0;
                    v173.f1 = 0.0;
                    v173.gao2 = (tGameObjectMain*)v208->des_o_art_wp[v208->i_last_art_index];
                    v173.gao = AI_gpst_CurrentGameObject;
                    AI_EvalFunc_MSGSend_C((tGameObjectMain*)Vendeur_Vendeur_reflex__PJ_Sally_gao, &v173);
                }
                else if (v208->des_monaie && v208->m_msg.f2 == 28.0)
                {
                    if (v184 >= v208->des_i_art_prix_vente[v208->i_last_art_index] && v196)
                    {
                        v183 = AI_EvalFunc_ARRBitTest_C((int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4), 3712);
                        v155 = AI_EvalFunc_ARRBitTest_C((int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4), 2255);
                        if (v208->i_comportement == 4 && v208->i_last_art_index == 1 && !v183)
                        {
                            Src.msgID = 30;
                            Src.Int1 = 10;
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
                            v89 = (tGameObjectMain*)v208->des_o_acteur_txt;
                            if (v89)
                            {
                                if ((unsigned int)v89 <= 2)
                                    v89 = AI_MainActor[(uint32_t)v89 - 1];
                                AI_EvalFunc_MSGSend_C(v89, &Src);
                            }
                            else
                            {
                                AI_EvalFunc_MSGSend_C(0, &Src);
                            }
                        }
                        else
                        {
                            if (v208->i_comportement == 4 && !v208->i_last_art_index && !v155)
                            {
                                AI_EvalFunc_ARRBitSet_C((int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4), 3743);
                                Src.msgID = 30;
                                Src.Int1 = 11;
                                Src.Int2 = 0;
                                Src.f2 = 9.0;
                                Src.v1.x = 0.0;
                                Src.v1.y = 0.0;
                                Src.v1.z = 0.0;
                                Src.v2.x = 0.0;
                                Src.v2.y = 0.0;
                                Src.v2.z = 0.0;
                                Src.gao = 0;
                                Src.gao2 = 0;
                                v88 = (tGameObjectMain*)v208->des_o_acteur_txt;
                                if (v88)
                                {
                                    if ((unsigned int)v88 <= 2)
                                        v88 = AI_MainActor[(uint32_t)v88 - 1];
                                    AI_EvalFunc_MSGSend_C(v88, &Src);
                                }
                                else
                                {
                                    AI_EvalFunc_MSGSend_C(0, &Src);
                                }
                                *AI_gppst_CurrentJumpNode = (tAINode*)122;
                                TIME_Wait_C(9.0);
                                return 0;
                            }
                            v86 = (tGameObjectMain*)v208->des_o_art_Prefetch[v208->i_last_art_index];
                            if (v86)
                            {
                                if ((unsigned int)v86 <= 2)
                                    v86 = AI_MainActor[(uint32_t)v86 - 1];
                                v85 = v86;
                            }
                            else
                            {
                                v85 = 0;
                            }
                            if (v85)
                            {
                                v84 = (tGameObjectMain*)v208->des_o_art_Prefetch[v208->i_last_art_index];
                                if (v84)
                                {
                                    if ((unsigned int)v84 <= 2)
                                        v84 = AI_MainActor[(uint32_t)v84 - 1];
                                    AI_EvalFunc_SNDPrefetchModifier_C(v84, 0);
                                }
                                else
                                {
                                    AI_EvalFunc_SNDPrefetchModifier_C(0, 0);
                                }
                            }
                            v173.msgID = 55;
                            v173.Int1 = 1;
                            v173.Int2 = -1;
                            v173.f2 = 0.0;
                            v173.f1 = 0.0;
                            if (!v208->des_monaie)
                                goto LABEL_457;
                            v83 = (tGameObjectMain*)v208->des_o_perle;
                            if (v83)
                            {
                                if ((unsigned int)v83 <= 2)
                                    v83 = AI_MainActor[(uint32_t)v83 - 1];
                                v82 = v83;
                            }
                            else
                            {
                                v82 = 0;
                            }
                            if (v82)
                            {
                                v81 = (tGameObjectMain*)v208->des_o_art_wp[v208->i_last_art_index];
                                if (v81)
                                {
                                    if ((unsigned int)v81 <= 2)
                                        v81 = AI_MainActor[(uint32_t)v81 - 1];
                                    AI_EvalFunc_OBJGetPos_C(v81, &v203);
                                }
                                else
                                {
                                    AI_EvalFunc_OBJGetPos_C(0, &v203);
                                }
                                v80 = (tGameObjectMain*)v208->des_o_perle;
                                if (v80)
                                {
                                    if ((unsigned int)v80 <= 2)
                                        v80 = AI_MainActor[(uint32_t)v80 - 1];
                                    AI_EvalFunc_OBJSetPos_C(v80, &v203);
                                }
                                else
                                {
                                    AI_EvalFunc_OBJSetPos_C(0, &v203);
                                }
                                v173.gao2 = (tGameObjectMain*)v208->des_o_perle;
                            }
                            else
                            {
                            LABEL_457:
                                v173.gao2 = (tGameObjectMain*)v208->des_o_art_wp[v208->i_last_art_index];
                            }
                            v173.gao = AI_gpst_CurrentGameObject;
                            AI_EvalFunc_MSGSend_C((tGameObjectMain*)Vendeur_Vendeur_reflex__PJ_Sally_gao, &v173);
                            v200 = 0;
                            v79 = (tGameObjectMain*)v208->des_o_sound_mamago;
                            if (v79)
                            {
                                if ((unsigned int)v79 <= 2)
                                    v79 = AI_MainActor[(uint32_t)v79 - 1];
                                v78 = v79;
                            }
                            else
                            {
                                v78 = 0;
                            }
                            if (v78)
                            {
                                v77 = (tGameObjectMain*)v208->des_o_sound_mamago;
                                if (v77)
                                {
                                    if ((unsigned int)v77 <= 2)
                                        v77 = AI_MainActor[(uint32_t)v77 - 1];
                                    AI_EvalFunc_SNDStopModifier_C(v77, 0);
                                }
                                else
                                {
                                    AI_EvalFunc_SNDStopModifier_C(0, 0);
                                }
                                v208->f_time_stop_mamago = TIM_gf_MainClock - TIM_gf_LockTime;
                            }
                            for (i = 0; i < v208->des_i_art_prix_vente[v208->i_last_art_index]; ++i)
                            {
                                while (v200 < *(int32_t*)Vendeur_Vendeur_reflex_univVars_i_PerleNbMax)
                                {
                                    v201 = (unsigned __int8)*(uint32_t*)(Vendeur_Vendeur_reflex_univVars_ai_PerleOrder + 4 * v200 + 4);
                                    v180 = (*(uint32_t*)(Vendeur_Vendeur_reflex_univVars_ai_PerleOrder + 4 * v200 + 4) & 0xFF00) >> 8;
                                    if (v180 + 2900 <= 0)
                                        v183 = 0;
                                    else
                                        v183 = AI_EvalFunc_ARRBitTest_C(
                                            (int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4),
                                            v180 + 2900);
                                    if (v183)
                                    {
                                        v183 = v201 + 3400 > 0
                                            && AI_EvalFunc_ARRBitTest_C(
                                                (int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4),
                                                v201 + 3400);
                                        if (!v183)
                                        {
                                            if (v201 + 3400 > 0)
                                                AI_EvalFunc_ARRBitSet_C(
                                                    (int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4),
                                                    v201 + 3400);
                                            break;
                                        }
                                    }
                                    ++v200;
                                }
                                if (*(int*)AI_EvalVarArray2_C(1, (char*)Vendeur_Vendeur_reflex_univVars_objet_inventaire, 45, 8) > 0
                                    && (*(uint32_t*)(Vendeur_Vendeur_reflex_univVars_objet_status + 184) & 0x400000) == 0)
                                {
                                    v36 = (uint32_t*)AI_EvalVarArray2_C(1, (char*)Vendeur_Vendeur_reflex_univVars_objet_inventaire, 45, 8);
                                    --* v36;
                                    if (!*(uint32_t*)AI_EvalVarArray2_C(
                                        1,
                                        (char*)Vendeur_Vendeur_reflex_univVars_objet_inventaire,
                                        45,
                                        8)
                                        && (*(uint32_t*)(Vendeur_Vendeur_reflex_univVars_objet_status + 184) & 0x7000000) == 0)
                                    {
                                        *(uint32_t*)AI_EvalVarArray2_C(1, (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_equipement, 0, 8) = -1;
                                    }
                                }
                            }
                            v208->f_time_decompte = TIM_gf_MainClock - TIM_gf_LockTime;
                            v208->i_flag_vente = 1;
                            v208->i_transaction_done = 0;
                            v208->i_last_thune = *(uint32_t*)Vendeur_Vendeur_reflex_univVars_i_Nb_Perles;
                            if (v208->des_i_art_type[v208->i_last_art_index] < 1100)
                            {
                                if (((*(uint32_t*)(Vendeur_Vendeur_reflex_univVars_objet_status
                                    + 4 * v208->des_i_art_type[v208->i_last_art_index]
                                    + 4) & 0x400000) == 0
                                    || !*(uint32_t*)AI_EvalVarArray2_C(
                                        1,
                                        (char*)Vendeur_Vendeur_reflex_univVars_objet_inventaire,
                                        v208->des_i_art_type[v208->i_last_art_index],
                                        v208->des_i_destinataire[v208->i_last_art_index]))
                                    && *(int*)AI_EvalVarArray2_C(
                                        1,
                                        (char*)Vendeur_Vendeur_reflex_univVars_objet_inventaire,
                                        v208->des_i_art_type[v208->i_last_art_index],
                                        v208->des_i_destinataire[v208->i_last_art_index]) < 99)
                                {
                                    v35 = (uint32_t*)AI_EvalVarArray2_C(
                                        1,
                                        (char*)Vendeur_Vendeur_reflex_univVars_objet_inventaire,
                                        v208->des_i_art_type[v208->i_last_art_index],
                                        v208->des_i_destinataire[v208->i_last_art_index]);
                                    ++* v35;
                                }
                                if (v208->des_i_art_type[v208->i_last_art_index] + 1100 <= 0)
                                    v172 = 0;
                                else
                                    v172 = AI_EvalFunc_ARRBitTest_C(
                                        (int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4),
                                        v208->des_i_art_type[v208->i_last_art_index] + 1100);
                                if (v172)
                                {
                                    v214 = Vendeur_Vendeur_reflex_GST_Board_panel_gao;
                                    v172 = *(uint32_t*)(Vendeur_Vendeur_reflex_univVars_objet_status
                                        + 4 * v208->des_i_art_type[v208->i_last_art_index]
                                        + 4) & 0x7000000;
                                    v172 >>= 24;
                                    if (v172 != 7
                                        && ((1 << v208->des_i_destinataire[v208->i_last_art_index]) & (unsigned __int16)*(uint32_t*)(Vendeur_Vendeur_reflex_univVars_objet_status + 4 * v208->des_i_art_type[v208->i_last_art_index] + 4)) != 0)
                                    {
                                        v8 = v208->i_last_art_index;
                                        v9 = (uint32_t*)AI_EvalVarArray2_C(
                                            1,
                                            (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_equipement,
                                            v172,
                                            v208->des_i_destinataire[v8]);
                                        *v9 = v208->des_i_art_type[v8];
                                        *(uint32_t*)Vendeur_Vendeur_reflex_GST_Global_gao_i_Perso_ID_a_checker |= 1 << v208->des_i_destinataire[v208->i_last_art_index];
                                    }
                                    if (v208->des_i_art_type[v208->i_last_art_index] == 28
                                        && v208->des_i_destinataire[v208->i_last_art_index] < 9
                                        || v208->des_i_art_type[v208->i_last_art_index] == 14
                                        && v208->des_i_destinataire[v208->i_last_art_index] >= 9)
                                    {
                                        v34 = AI_EvalVarArray_C2(
                                            (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_courante,
                                            v208->des_i_destinataire[v208->i_last_art_index]);
                                        *(float*)v34 = *(float*)v34 + 980.0f;
                                        if (v208->des_i_destinataire[v208->i_last_art_index] >= 9)
                                        {
                                            v72 = (uint32_t*)(Vendeur_Vendeur_reflex_univVars_objet_valeur1 + 60);
                                            v71 = &v208->des_i_destinataire[v208->i_last_art_index];
                                            v30 = (*v72 - 1000)
                                                * (*(uint32_t*)AI_EvalVarArray2_C(
                                                    1,
                                                    (char*)Vendeur_Vendeur_reflex_univVars_objet_inventaire,
                                                    14,
                                                    *v71)
                                                    + 2);
                                            *(float*)AI_EvalVarArray_C2((char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_max, *v71) = (float)v30;
                                            v70 = &v208->des_i_destinataire[v208->i_last_art_index];
                                            v29 = *(float*)AI_EvalVarArray_C2(
                                                (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_courante,
                                                *v70);
                                            if (*(float*)AI_EvalVarArray_C2(
                                                (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_max,
                                                *v70) < (double)v29)
                                            {
                                                v69 = &v208->des_i_destinataire[v208->i_last_art_index];
                                                v28 = *(float*)AI_EvalVarArray_C2(
                                                    (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_max,
                                                    *v69);
                                                *(float*)AI_EvalVarArray_C2(
                                                    (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_courante,
                                                    *v69) = v28;
                                            }
                                        }
                                        else
                                        {
                                            v76 = (uint32_t*)(Vendeur_Vendeur_reflex_univVars_objet_valeur1 + 116);
                                            v75 = &v208->des_i_destinataire[v208->i_last_art_index];
                                            v33 = (*v76 - 1000)
                                                * (*(uint32_t*)AI_EvalVarArray2_C(
                                                    1,
                                                    (char*)Vendeur_Vendeur_reflex_univVars_objet_inventaire,
                                                    28,
                                                    *v75)
                                                    + 2);
                                            *(float*)AI_EvalVarArray_C2((char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_max, *v75) = (float)v33;
                                            v74 = &v208->des_i_destinataire[v208->i_last_art_index];
                                            v32 = *(float*)AI_EvalVarArray_C2(
                                                (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_courante,
                                                *v74);
                                            if (*(float*)AI_EvalVarArray_C2(
                                                (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_max,
                                                *v74) < (double)v32)
                                            {
                                                v73 = &v208->des_i_destinataire[v208->i_last_art_index];
                                                v31 = *(float*)AI_EvalVarArray_C2(
                                                    (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_max,
                                                    *v73);
                                                *(float*)AI_EvalVarArray_C2(
                                                    (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_courante,
                                                    *v73) = v31;
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                *(uint32_t*)Vendeur_Vendeur_reflex_univVars_mdisk_possessionflags |= 1 << (v208->des_i_art_type[v208->i_last_art_index]
                                    - 76);
                                *(uint32_t*)AI_EvalVarArray2_C(
                                    1,
                                    (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_equipement,
                                    0,
                                    v208->des_i_destinataire[v208->i_last_art_index]) = 27;
                                *(uint32_t*)(Vendeur_Vendeur_reflex_univVars_objet_valeur1 + 112) = v208->des_i_art_type[v208->i_last_art_index]
                                    - 1100;
                            }
                            if (v208->des_i_art_SF[v208->i_last_art_index] > 0)
                                AI_EvalFunc_ARRBitSet_C(
                                    (int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4),
                                    v208->des_i_art_SF[v208->i_last_art_index]);
                        }
                    }
                    else
                    {
                        v191 = AI_EvalFunc_MATHRandInt_C(0, 3);
                        i = v191 + 64;
                        Src.msgID = 30;
                        Src.Int1 = v191 + 64;
                        Src.Int2 = 0;
                        Src.f2 = 1.0;
                        Src.v1.x = 0.0;
                        Src.v1.y = 0.0;
                        Src.v1.z = 0.0;
                        Src.v2.x = 0.0;
                        Src.v2.y = 0.0;
                        Src.v2.z = 0.0;
                        Src.gao = 0;
                        Src.gao2 = 0;
                        AI_EvalFunc_MSGSend_C((tGameObjectMain*)Vendeur_Vendeur_reflex_TXT_Sally_gao, &Src);
                        v199 = AI_EvalFunc_SNDRequest_C((tGameObjectMain*)Vendeur_Vendeur_reflex_GST_Board_panel_gao, 3, 128);
                        AI_EvalFunc_SNDUltraPlaySound_C(AI_gpst_CurrentGameObject, v199);
                    }
                }
            }
            else if (v208->m_msg.msgID == 37)
            {
                v182 = v208->m_msg.Int1 & 7;
                if (v182 != 1)
                    goto LABEL_610;
                if (v208->m_msg.f2 == -1.0)
                {
                    v196 = 1;
                    if (v208->des_i_art_SELL_if_SF[v208->i_last_art_index])
                    {
                        if (v208->des_i_art_SELL_if_SF[v208->i_last_art_index] <= 0)
                            v196 = 0;
                        else
                            v196 = AI_EvalFunc_ARRBitTest_C(
                                (int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4),
                                v208->des_i_art_SELL_if_SF[v208->i_last_art_index]);
                    }
                    if (v196)
                    {
                        v199 = AI_EvalFunc_SNDRequest_C((tGameObjectMain*)Vendeur_Vendeur_reflex_GST_Board_panel_gao, 5, 128);
                        AI_EvalFunc_SNDUltraPlaySound_C(AI_gpst_CurrentGameObject, v199);
                        v208->f_time_decompte = TIM_gf_MainClock - TIM_gf_LockTime;
                        v208->i_flag_vente = 1;
                        v208->i_transaction_done = 0;
                        if (v208->des_monaie)
                            v208->i_last_thune = *(uint32_t*)Vendeur_Vendeur_reflex_univVars_i_Nb_Perles;
                        else
                            v208->i_last_thune = *(uint32_t*)Vendeur_Vendeur_reflex_univVars_f_sally_fric;
                        if (*(uint32_t*)AI_EvalVarArray2_C(
                            1,
                            (char*)Vendeur_Vendeur_reflex_univVars_ai_VEND_art_qte,
                            v208->i_last_art_index,
                            v208->des_i_vendeur_ID) != -1)
                        {
                            v27 = (uint32_t*)AI_EvalVarArray2_C(
                                1,
                                (char*)Vendeur_Vendeur_reflex_univVars_ai_VEND_art_qte,
                                v208->i_last_art_index,
                                v208->des_i_vendeur_ID);
                            --* v27;
                        }
                        if (v208->des_i_type)
                        {
                            v68 = (tGameObjectMain*)v208->des_o_art_wp[v208->i_last_art_index];
                            if (v68)
                            {
                                if ((unsigned int)v68 <= 2)
                                    v68 = AI_MainActor[(uint32_t)v68 - 1];
                                AI_EvalFunc_OBJ_FlagInvisibleSet_C(v68, 1);
                            }
                            else
                            {
                                AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
                            }
                        }
                        if (v208->des_i_art_type[v208->i_last_art_index] < 1100)
                        {
                            if (((*(uint32_t*)(Vendeur_Vendeur_reflex_univVars_objet_status
                                + 4 * v208->des_i_art_type[v208->i_last_art_index]
                                + 4) & 0x400000) == 0
                                || !*(uint32_t*)AI_EvalVarArray2_C(
                                    1,
                                    (char*)Vendeur_Vendeur_reflex_univVars_objet_inventaire,
                                    v208->des_i_art_type[v208->i_last_art_index],
                                    v208->des_i_destinataire[v208->i_last_art_index]))
                                && *(int*)AI_EvalVarArray2_C(
                                    1,
                                    (char*)Vendeur_Vendeur_reflex_univVars_objet_inventaire,
                                    v208->des_i_art_type[v208->i_last_art_index],
                                    v208->des_i_destinataire[v208->i_last_art_index]) < 99)
                            {
                                v26 = (uint32_t*)AI_EvalVarArray2_C(
                                    1,
                                    (char*)Vendeur_Vendeur_reflex_univVars_objet_inventaire,
                                    v208->des_i_art_type[v208->i_last_art_index],
                                    v208->des_i_destinataire[v208->i_last_art_index]);
                                ++* v26;
                            }
                            if (v208->des_i_art_type[v208->i_last_art_index] + 1100 <= 0)
                                v172 = 0;
                            else
                                v172 = AI_EvalFunc_ARRBitTest_C(
                                    (int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4),
                                    v208->des_i_art_type[v208->i_last_art_index] + 1100);
                            if (v172)
                            {
                                v214 = Vendeur_Vendeur_reflex_GST_Board_panel_gao;
                                v172 = *(uint32_t*)(Vendeur_Vendeur_reflex_univVars_objet_status
                                    + 4 * v208->des_i_art_type[v208->i_last_art_index]
                                    + 4) & 0x7000000;
                                v172 >>= 24;
                                if (v172 != 7
                                    && ((1 << v208->des_i_destinataire[v208->i_last_art_index]) & (unsigned __int16)*(uint32_t*)(Vendeur_Vendeur_reflex_univVars_objet_status + 4 * v208->des_i_art_type[v208->i_last_art_index] + 4)) != 0)
                                {
                                    v10 = v208->i_last_art_index;
                                    v11 = (uint32_t*)AI_EvalVarArray2_C(
                                        1,
                                        (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_equipement,
                                        v172,
                                        v208->des_i_destinataire[v10]);
                                    *v11 = v208->des_i_art_type[v10];
                                    *(uint32_t*)Vendeur_Vendeur_reflex_GST_Global_gao_i_Perso_ID_a_checker |= 1 << v208->des_i_destinataire[v208->i_last_art_index];
                                }
                                if (v208->des_i_art_type[v208->i_last_art_index] == 28
                                    && v208->des_i_destinataire[v208->i_last_art_index] < 9
                                    || v208->des_i_art_type[v208->i_last_art_index] == 14
                                    && v208->des_i_destinataire[v208->i_last_art_index] >= 9)
                                {
                                    v25 = AI_EvalVarArray_C2(
                                        (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_courante,
                                        v208->des_i_destinataire[v208->i_last_art_index]);
                                    *(float*)v25 = *(float*)v25 + 980.0f;
                                    if (v208->des_i_destinataire[v208->i_last_art_index] >= 9)
                                    {
                                        v63 = (uint32_t*)(Vendeur_Vendeur_reflex_univVars_objet_valeur1 + 60);
                                        v62 = &v208->des_i_destinataire[v208->i_last_art_index];
                                        v21 = (*v63 - 1000)
                                            * (*(uint32_t*)AI_EvalVarArray2_C(
                                                1,
                                                (char*)Vendeur_Vendeur_reflex_univVars_objet_inventaire,
                                                14,
                                                *v62)
                                                + 2);
                                        *(float*)AI_EvalVarArray_C2((char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_max, *v62) = (float)v21;
                                        v61 = &v208->des_i_destinataire[v208->i_last_art_index];
                                        v20 = *(float*)AI_EvalVarArray_C2(
                                            (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_courante,
                                            *v61);
                                        if (*(float*)AI_EvalVarArray_C2(
                                            (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_max,
                                            *v61) < (double)v20)
                                        {
                                            v60 = &v208->des_i_destinataire[v208->i_last_art_index];
                                            v19 = *(float*)AI_EvalVarArray_C2(
                                                (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_max,
                                                *v60);
                                            *(float*)AI_EvalVarArray_C2(
                                                (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_courante,
                                                *v60) = v19;
                                        }
                                    }
                                    else
                                    {
                                        v67 = (uint32_t*)(Vendeur_Vendeur_reflex_univVars_objet_valeur1 + 116);
                                        v66 = &v208->des_i_destinataire[v208->i_last_art_index];
                                        v24 = (*v67 - 1000)
                                            * (*(uint32_t*)AI_EvalVarArray2_C(
                                                1,
                                                (char*)Vendeur_Vendeur_reflex_univVars_objet_inventaire,
                                                28,
                                                *v66)
                                                + 2);
                                        *(float*)AI_EvalVarArray_C2((char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_max, *v66) = (float)v24;
                                        v65 = &v208->des_i_destinataire[v208->i_last_art_index];
                                        v23 = *(float*)AI_EvalVarArray_C2(
                                            (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_courante,
                                            *v65);
                                        if (*(float*)AI_EvalVarArray_C2(
                                            (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_max,
                                            *v65) < (double)v23)
                                        {
                                            v64 = &v208->des_i_destinataire[v208->i_last_art_index];
                                            v22 = *(float*)AI_EvalVarArray_C2(
                                                (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_max,
                                                *v64);
                                            *(float*)AI_EvalVarArray_C2(
                                                (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_courante,
                                                *v64) = v22;
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            *(uint32_t*)Vendeur_Vendeur_reflex_univVars_mdisk_possessionflags |= 1 << (v208->des_i_art_type[v208->i_last_art_index]
                                - 76);
                            *(uint32_t*)AI_EvalVarArray2_C(
                                1,
                                (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_equipement,
                                0,
                                v208->des_i_destinataire[v208->i_last_art_index]) = 27;
                            *(uint32_t*)(Vendeur_Vendeur_reflex_univVars_objet_valeur1 + 112) = v208->des_i_art_type[v208->i_last_art_index]
                                - 1100;
                        }
                        if (v208->des_i_art_SF[v208->i_last_art_index] > 0)
                            AI_EvalFunc_ARRBitSet_C(
                                (int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4),
                                v208->des_i_art_SF[v208->i_last_art_index]);
                        v59 = (tGameObjectMain*)v208->des_o_acteur_txt;
                        if (v59)
                        {
                            if ((unsigned int)v59 <= 2)
                                v59 = AI_MainActor[(uint32_t)v59 - 1];
                            v58 = v59;
                        }
                        else
                        {
                            v58 = 0;
                        }
                        if (v58 && v208->des_i_TXT_index_vente[v208->i_last_art_index] != -1)
                        {
                            v208->m_msg.msgID = 30;
                            v208->m_msg.gao = AI_gpst_CurrentGameObject;
                            v208->m_msg.gao2 = 0;
                            v208->m_msg.Int1 = v208->des_i_TXT_index_vente[v208->i_last_art_index];
                            v208->m_msg.Int2 = 5;
                            v208->m_msg.f2 = 3.0;
                            v208->m_msg.f1 = 0.0;
                            v208->m_msg.v1.x = 0.0;
                            v208->m_msg.v1.y = 0.0;
                            v208->m_msg.v1.z = 0.0;
                            v208->m_msg.v2.x = 0.0;
                            v208->m_msg.v2.y = 0.0;
                            v208->m_msg.v2.z = 0.0;
                            v57 = (tGameObjectMain*)v208->des_o_acteur_txt;
                            if (!v57)
                                goto LABEL_566;
                            if ((unsigned int)v57 <= 2)
                                v57 = AI_MainActor[(uint32_t)v57 - 1];
                            AI_EvalFunc_MSGSend_C(v57, &v208->m_msg);
                        }
                    }
                    else if (v208->i_last_art_index != -1)
                    {
                        v208->i_transaction_done = 1;
                        v208->f_time_decompte = 0.0;
                        v199 = AI_EvalFunc_SNDRequest_C((tGameObjectMain*)Vendeur_Vendeur_reflex_GST_Board_panel_gao, 3, 128);
                        AI_EvalFunc_SNDUltraPlaySound_C(AI_gpst_CurrentGameObject, v199);
                        v56 = (tGameObjectMain*)v208->des_o_art_wp[v208->i_last_art_index];
                        if (v56)
                        {
                            if ((unsigned int)v56 <= 2)
                                v56 = AI_MainActor[(uint32_t)v56 - 1];
                            AI_EvalFunc_OBJ_CapaSet_C(v56, 256, 0);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_CapaSet_C(0, 256, 0);
                        }
                        v55 = (tGameObjectMain*)v208->des_o_acteur_txt;
                        if (v55)
                        {
                            if ((unsigned int)v55 <= 2)
                                v55 = AI_MainActor[(uint32_t)v55 - 1];
                            v54 = v55;
                        }
                        else
                        {
                            v54 = 0;
                        }
                        if (v54 && v208->des_i_TXT_index[v208->i_last_art_index] != -1)
                        {
                            v208->m_msg.msgID = 30;
                            v208->m_msg.gao = AI_gpst_CurrentGameObject;
                            v208->m_msg.gao2 = 0;
                            v208->m_msg.Int1 = v208->des_i_TXT_index[v208->i_last_art_index];
                            v208->m_msg.Int2 = 5;
                            v208->m_msg.f2 = 3.0;
                            v208->m_msg.f1 = 0.0;
                            v208->m_msg.v1.x = 0.0;
                            v208->m_msg.v1.y = 0.0;
                            v208->m_msg.v1.z = 0.0;
                            v208->m_msg.v2.x = 0.0;
                            v208->m_msg.v2.y = 0.0;
                            v208->m_msg.v2.z = 0.0;
                            v53 = (tGameObjectMain*)v208->des_o_acteur_txt;
                            if (!v53)
                            {
                            LABEL_566:
                                AI_EvalFunc_MSGSend_C(0, &v208->m_msg);
                                goto LABEL_610;
                            }
                            if ((unsigned int)v53 <= 2)
                                v53 = AI_MainActor[(uint32_t)v53 - 1];
                            AI_EvalFunc_MSGSend_C(v53, &v208->m_msg);
                        }
                    }
                }
                else if (v208->m_msg.f2 == -2.0)
                {
                    if (v208->des_monaie)
                    {
                        if (*(uint32_t*)AI_EvalVarArray2_C(
                            1,
                            (char*)Vendeur_Vendeur_reflex_univVars_ai_VEND_art_qte,
                            v208->i_last_art_index,
                            v208->des_i_vendeur_ID) != -1)
                        {
                            v12 = (uint32_t*)AI_EvalVarArray2_C(
                                1,
                                (char*)Vendeur_Vendeur_reflex_univVars_ai_VEND_art_qte,
                                v208->i_last_art_index,
                                v208->des_i_vendeur_ID);
                            --* v12;
                        }
                    }
                    else
                    {
                        v208->f_time_decompte = 0.0;
                        v208->i_flag_vente = 0;
                        v208->i_transaction_done = 0;
                        if (v208->des_monaie)
                            v208->i_last_thune = *(uint32_t*)Vendeur_Vendeur_reflex_univVars_i_Nb_Perles;
                        else
                            v208->i_last_thune = *(uint32_t*)Vendeur_Vendeur_reflex_univVars_f_sally_fric;
                        if (*(uint32_t*)AI_EvalVarArray2_C(
                            1,
                            (char*)Vendeur_Vendeur_reflex_univVars_ai_VEND_art_qte,
                            v208->i_last_art_index,
                            v208->des_i_vendeur_ID) != -1)
                        {
                            v18 = (uint32_t*)AI_EvalVarArray2_C(
                                1,
                                (char*)Vendeur_Vendeur_reflex_univVars_ai_VEND_art_qte,
                                v208->i_last_art_index,
                                v208->des_i_vendeur_ID);
                            ++* v18;
                        }
                        if (*(int*)AI_EvalVarArray2_C(
                            1,
                            (char*)Vendeur_Vendeur_reflex_univVars_objet_inventaire,
                            v208->des_i_art_type[v208->i_last_art_index],
                            8) > 0
                            && (*(uint32_t*)(Vendeur_Vendeur_reflex_univVars_objet_status
                                + 4 * v208->des_i_art_type[v208->i_last_art_index]
                                + 4) & 0x400000) == 0)
                        {
                            v17 = (uint32_t*)AI_EvalVarArray2_C(
                                1,
                                (char*)Vendeur_Vendeur_reflex_univVars_objet_inventaire,
                                v208->des_i_art_type[v208->i_last_art_index],
                                8);
                            --* v17;
                            if (!*(uint32_t*)AI_EvalVarArray2_C(
                                1,
                                (char*)Vendeur_Vendeur_reflex_univVars_objet_inventaire,
                                v208->des_i_art_type[v208->i_last_art_index],
                                8)
                                && (*(uint32_t*)(Vendeur_Vendeur_reflex_univVars_objet_status
                                    + 4 * v208->des_i_art_type[v208->i_last_art_index]
                                    + 4) & 0x7000000) == 0)
                            {
                                *(uint32_t*)AI_EvalVarArray2_C(1, (char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_equipement, 0, 8) = -1;
                            }
                        }
                        if (v208->des_i_art_type[v208->i_last_art_index] == 28)
                        {
                            v52 = (uint32_t*)(Vendeur_Vendeur_reflex_univVars_objet_valeur1 + 116);
                            v16 = (*v52 - 1000)
                                * (*(uint32_t*)AI_EvalVarArray2_C(1, (char*)Vendeur_Vendeur_reflex_univVars_objet_inventaire, 28, 8)
                                    + 2);
                            *(float*)AI_EvalVarArray_C2((char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_max, 8) = (float)v16;
                            v15 = *(float*)AI_EvalVarArray_C2((char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_courante, 8);
                            if (*(float*)AI_EvalVarArray_C2((char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_max, 8) < (double)v15)
                            {
                                v14 = *(float*)AI_EvalVarArray_C2((char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_max, 8);
                                *(float*)AI_EvalVarArray_C2((char*)Vendeur_Vendeur_reflex_univVars_Perso_ID_Energie_courante, 8) = v14;
                            }
                        }
                    }
                }
            }
            else if (v208->m_msg.msgID == 10 || v208->m_msg.msgID == 20)
            {
                memcpy(&v208->m_dialogue, &v208->m_msg, sizeof(v208->m_dialogue));
                v165 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Vendeur_Vendeur_reflex_Vendeur_Vendeur_exec_dialog);
                if (v165 != 1000)
                    return v165;
            }
        }
        else if (v208->m_msg.f2 == 26.0)
        {
            v196 = AI_EvalFunc_ARRBitTest_C((int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4), 998);
            if (v208->des_monaie)
                v196 = 1;
            if (v208->i_last_art_index == -1)
                v208->i_last_art_index = -1;
            if (v208->i_last_art_index == -1 || v184 < v208->des_i_art_prix_vente[v208->i_last_art_index] || !v196)
            {
                v199 = AI_EvalFunc_SNDRequest_C((tGameObjectMain*)Vendeur_Vendeur_reflex_GST_Board_panel_gao, 3, 128);
                AI_EvalFunc_SNDUltraPlaySound_C(AI_gpst_CurrentGameObject, v199);
                if (v208->des_monaie)
                {
                    v178 = AI_EvalFunc_MATHRandInt_C(0, 3);
                    i = v178 + 64;
                    Src.msgID = 30;
                    Src.Int1 = v178 + 64;
                    Src.Int2 = 0;
                    Src.f2 = 1.0;
                    Src.v1.x = 0.0;
                    Src.v1.y = 0.0;
                    Src.v1.z = 0.0;
                    Src.v2.x = 0.0;
                    Src.v2.y = 0.0;
                    Src.v2.z = 0.0;
                    Src.gao = 0;
                    Src.gao2 = 0;
                    AI_EvalFunc_MSGSend_C((tGameObjectMain*)Vendeur_Vendeur_reflex_TXT_Sally_gao, &Src);
                }
                else
                {
                    i = AI_EvalFunc_ARRBitTest_C((int*)(Vendeur_Vendeur_reflex_univVars_Flag_speciaux_sauve + 4), 122);
                    if (i)
                    {
                        v159 = AI_EvalFunc_MATHRandInt_C(0, 3);
                        i = v159 + 61;
                        Src.msgID = 30;
                        Src.Int1 = v159 + 61;
                        Src.Int2 = 0;
                        Src.f2 = 1.0;
                    }
                    else
                    {
                        v150 = AI_EvalFunc_MATHRandInt_C(0, 3);
                        i = v150 + 45;
                        Src.msgID = 30;
                        Src.Int1 = v150 + 45;
                        Src.Int2 = 0;
                        Src.f2 = 2.0;
                    }
                    Src.v1.x = 0.0;
                    Src.v1.y = 0.0;
                    Src.v1.z = 0.0;
                    Src.v2.x = 0.0;
                    Src.v2.y = 0.0;
                    Src.v2.z = 0.0;
                    Src.gao = 0;
                    Src.gao2 = 0;
                    AI_EvalFunc_MSGSend_C((tGameObjectMain*)Vendeur_Vendeur_reflex_TXT_Sally_gao, &Src);
                }
            }
            else
            {
                if (v208->des_i_type)
                {
                    v173.msgID = 55;
                    v173.gao = AI_gpst_CurrentGameObject;
                    v173.gao2 = (tGameObjectMain*)v208->des_o_art_wp[v208->i_last_art_index];
                    v173.Int1 = 0;
                    v173.Int2 = v208->des_i_art_type[v208->i_last_art_index];
                }
                else
                {
                    v173.msgID = 7;
                    v173.gao = (tGameObjectMain*)v208->des_o_art_wp[v208->i_last_art_index];
                    v173.gao2 = AI_gpst_CurrentGameObject;
                    v173.Int1 = -1;
                    v173.Int2 = 0;
                }
                v173.f2 = 0.0;
                v173.f1 = 0.0;
                v173.v1.x = 0.0;
                v173.v1.y = 0.0;
                v173.v1.z = 0.0;
                v173.v2.x = 0.0;
                v173.v2.y = 0.0;
                v173.v2.z = 0.0;
                AI_EvalFunc_MSGSend_C((tGameObjectMain*)Vendeur_Vendeur_reflex__PJ_Sally_gao, &v173);
            }
        }
        else if (v208->m_msg.f2 == 25.0)
        {
            v208->i_flag_talk = 1;
        }
    LABEL_610:
        v154 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v208->m_msg, v154, sizeof(v208->m_msg));
    }
    v51 = (tGameObjectMain*)v208->des_o_cam_wp;
    if (v51)
    {
        if ((unsigned int)v51 <= 2)
            v51 = AI_MainActor[(uint32_t)v51 - 1];
        v50 = v51;
    }
    else
    {
        v50 = 0;
    }
    if (!v50)
        goto LABEL_636;
    if (!AI_EvalFunc_OBJ_CustomBitsTest_C((tGameObjectMain*)Vendeur_Vendeur_reflex_GST_Board_panel_gao, 2048))
        goto LABEL_636;
    v167 = AI_gpst_CurrentGameObject;
    v49 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v49 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        v48 = v49;
    }
    else
    {
        v48 = 0;
    }
    if (*(tGameObjectMain**)Vendeur_Vendeur_reflex_GST_Board_panel_gao_o_dialogue_actor == v48)
    {
        v208->i_flag_je_desactive_la_camera = 1;
        AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)Vendeur_Vendeur_reflex_GST_Board_panel_gao, 0x200, 0);
        AI_EvalFunc_OBJ_FlagInactiveSet_C((tGameObjectMain*)Vendeur_Vendeur_reflex__PJ_Camera_gao, 1);
        v47 = (tGameObjectMain*)v208->des_o_cam_wp;
        if (v47)
        {
            if ((unsigned int)v47 <= 2)
                v47 = AI_MainActor[(uint32_t)v47 - 1];
            v13 = AI_EvalFunc_VIEWAssignObject_C(v47, 0);
        }
        else
        {
            v13 = AI_EvalFunc_VIEWAssignObject_C(0, 0);
        }
        v153 = v13;
    }
    else
    {
    LABEL_636:
        if (v208->i_flag_je_desactive_la_camera)
        {
            v208->i_flag_je_desactive_la_camera = 0;
            AI_EvalFunc_OBJ_FlagInactiveSet_C((tGameObjectMain*)Vendeur_Vendeur_reflex__PJ_Camera_gao, 0);
            *(uint32_t*)Vendeur_Vendeur_reflex__PJ_Camera_Derriere_gao_i_sim_click = 1;
        }
    }
    return 1000;
}

int Vendeur_Vendeur_Comp_Bio()
{
    int v0; // eax
    tScriptStruct_ActorVendeur* v2; // [esp+0h] [ebp-38h]
    tGameObjectMain* v3; // [esp+8h] [ebp-30h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-2Ch]
    tGameObjectMain* v5; // [esp+10h] [ebp-28h]
    unsigned int v6; // [esp+14h] [ebp-24h]
    int i; // [esp+1Ch] [ebp-1Ch]
    int v8; // [esp+34h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorVendeur*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Vendeur_Vendeur_Comp_Bio__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
    }
    v8 = 0;
    v2->des_o_acteur_suividesyeux = 0;
    if (v2->i_regarde_si_talk && v2->i_talk_propose)
    {
        v2->des_o_acteur_suividesyeux = Vendeur_Vendeur_Comp_Bio__PJ_Sally_gao;
        v2->des_f_RT_vitesse_de_rotation = 0.1f;
        v2->des_i_RT_flags = 0x80010000;
        v8 = 1;
    }
    else
    {
        for (i = 0; i < 5; ++i)
        {
            v6 = v2->o_pos_acheteur[i];
            v5 = (tGameObjectMain*)v6;
            if (v6)
            {
                if (v6 <= 2)
                    v5 = AI_MainActor[v6 - 1];
                v4 = v5;
            }
            else
            {
                v4 = 0;
            }
            if (v4)
            {
                v3 = (tGameObjectMain*)v2->o_pos_acheteur[i];
                if (v6)
                {
                    if (v6 <= 2)
                        v3 = AI_MainActor[v6 - 1];
                    v0 = AI_EvalFunc_OBJ_CapaGet_C(v3);
                }
                else
                {
                    v0 = AI_EvalFunc_OBJ_CapaGet_C(0);
                }
                if (!v0)
                {
                    v2->des_o_acteur_suividesyeux = v6;
                    v2->des_f_RT_vitesse_de_rotation = 0.1f;
                    v2->des_i_RT_flags = 0x80010000;
                    v2->des_v_hauteur_cible.x = 0.0;
                    v2->des_v_hauteur_cible.y = 0.0;
                    v2->des_v_hauteur_cible.z = 1.5;
                    v8 = 1;
                    break;
                }
            }
        }
    }
    if (AI_EvalFunc_ACTActionFinished_C_CURRENT())
    {
        if (v8)
            AI_EvalFunc_ACTActionSet_C_CURRENT(1);
        else
            AI_EvalFunc_ACTActionSet_C_CURRENT(0);
    }
    return 1000;
}

int Vendeur_Vendeur_mamago()
{
    double v0; // st7
    tScriptStruct_ActorVendeur* v2; // [esp+0h] [ebp-7Ch]
    tGameObjectMain* v3; // [esp+8h] [ebp-74h]
    tGameObjectMain* v4; // [esp+10h] [ebp-6Ch]
    tGameObjectMain* v5; // [esp+18h] [ebp-64h]
    tGameObjectMain* v6; // [esp+20h] [ebp-5Ch]
    tGameObjectMain* v7; // [esp+28h] [ebp-54h]
    tGameObjectMain* v8; // [esp+30h] [ebp-4Ch]
    tGameObjectMain* v9; // [esp+38h] [ebp-44h]
    tGameObjectMain* v10; // [esp+40h] [ebp-3Ch]
    tGameObjectMain* v11; // [esp+48h] [ebp-34h]
    tGameObjectMain* v12; // [esp+50h] [ebp-2Ch]
    tGameObjectMain* v13; // [esp+58h] [ebp-24h]
    float v14; // [esp+70h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorVendeur*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Vendeur_Vendeur_mamago_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_mamago_univVars_Flag_speciaux_sauve,
            (void*)Vendeur_Vendeur_mamago_univVars,
            4488);
        Vendeur_Vendeur_mamago__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Vendeur_Vendeur_mamago_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_mamago_GST_Board_panel_gao_i_dialog_phase,
            (void*)Vendeur_Vendeur_mamago_GST_Board_panel_gao,
            1604);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_mamago_GST_Board_panel_gao_i_dialog_page_actu,
            (void*)Vendeur_Vendeur_mamago_GST_Board_panel_gao,
            1616);
    }
    v2->i_nb_articles = 6;
    if (AI_EvalFunc_ARRBitTest_C((int*)(Vendeur_Vendeur_mamago_univVars_Flag_speciaux_sauve + 4), 2189))
    {
        v13 = (tGameObjectMain*)v2->des_o_mamago_decor;
        if (v13)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(v13, 1);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
        }
        v12 = (tGameObjectMain*)v2->des_o_mamago_decor;
        if (v12)
        {
            if ((unsigned int)v12 <= 2)
                v12 = AI_MainActor[(uint32_t)v12 - 1];
            AI_EvalFunc_OBJ_FlagInactiveSet_C(v12, 1);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
        }
        v11 = (tGameObjectMain*)v2->des_o_mamago_decor1;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v11 = AI_MainActor[(uint32_t)v11 - 1];
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(v11, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
        }
        v10 = (tGameObjectMain*)v2->des_o_mamago_decor1;
        if (v10)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            AI_EvalFunc_OBJ_FlagInactiveSet_C(v10, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
        }
        v9 = (tGameObjectMain*)v2->des_o_mamago_decor_f;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            AI_EvalFunc_OBJ_FlagInactiveSet_C(v9, 1);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
        }
        v8 = (tGameObjectMain*)v2->des_o_mamago_decor_f;
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v8 = AI_MainActor[(uint32_t)v8 - 1];
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(v8, 1);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
        }
    }
    else
    {
        v7 = (tGameObjectMain*)v2->des_o_mamago_decor;
        if (v7)
        {
            if ((unsigned int)v7 <= 2)
                v7 = AI_MainActor[(uint32_t)v7 - 1];
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(v7, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
        }
        v6 = (tGameObjectMain*)v2->des_o_mamago_decor;
        if (v6)
        {
            if ((unsigned int)v6 <= 2)
                v6 = AI_MainActor[(uint32_t)v6 - 1];
            AI_EvalFunc_OBJ_FlagInactiveSet_C(v6, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
        }
        v5 = (tGameObjectMain*)v2->des_o_mamago_decor1;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(v5, 1);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
        }
        v4 = (tGameObjectMain*)v2->des_o_mamago_decor1;
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
    }
    v2->des_o_acteur_suividesyeux = 0;
    v3 = (tGameObjectMain*)Vendeur_Vendeur_mamago__PJ_Sally_gao;
    if (Vendeur_Vendeur_mamago__PJ_Sally_gao)
    {
        if ((unsigned int)Vendeur_Vendeur_mamago__PJ_Sally_gao <= 2)
            v3 = AI_MainActor[Vendeur_Vendeur_mamago__PJ_Sally_gao - 1];
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v3);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v14 = (float)v0;
    if (v2->f_dist_regarde_sally > (double)v14)
    {
        v2->des_o_acteur_suividesyeux = Vendeur_Vendeur_mamago__PJ_Sally_gao;
        v2->des_f_RT_vitesse_de_rotation = 0.1f;
        v2->des_i_RT_flags = 0x80010000;
    }
    if (!AI_EvalFunc_OBJ_CapaTest_C_CURRENT(1))
    {
        if (*(uint32_t*)Vendeur_Vendeur_mamago_GST_Board_panel_gao_i_dialog_phase == 3
            && *(uint32_t*)Vendeur_Vendeur_mamago_GST_Board_panel_gao_i_dialog_page_actu == 1)
        {
            if (AI_EvalFunc_ACTActionGet_C_CURRENT() == 1 && AI_EvalFunc_ACTActionFinished_C_CURRENT())
            {
                AI_EvalFunc_ACTActionSet_C_CURRENT(0);
            }
            else if (v2->i_mamago_replique)
            {
                AI_EvalFunc_ACTActionSet_C_CURRENT(1);
                v2->i_mamago_replique = 0;
            }
        }
        else
        {
            v2->i_mamago_replique = 1;
            AI_EvalFunc_ACTActionSet_C_CURRENT(0);
        }
    }
    return 1000;
};

int Vendeur_Vendeur_AfterBlend()
{
    int v0; // eax
    tGameObjectMain* v1; // eax
    tScriptStruct_ActorVendeur* v3; // [esp+0h] [ebp-118h]
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
    tScriptStruct_ActorVendeur* v32; // [esp+F0h] [ebp-28h]
    float v33; // [esp+F4h] [ebp-24h]
    unsigned int v34; // [esp+F8h] [ebp-20h]
    Vec3 v35; // [esp+108h] [ebp-10h] BYREF
    float v36; // [esp+114h] [ebp-4h]

    v21.x = 1.0;
    v21.y = 0.0;
    v21.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_ActorVendeur*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v32 = v3;
    v33 = 0.0;
    v34 = 0;
    v25 = 0;
    if (v3->i_ne_tourne_que_la_tete)
        v32->des_i_RT_flags |= 0x10000000u;
    v17 = (tGameObjectMain*)v32->des_o_acteur_suividesyeux;
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

int Vendeur_Vendeur_exec_dialog()
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
    tScriptStruct_ActorVendeur* v19; // [esp+10h] [ebp-194h]
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
    tScriptStruct_ActorVendeur* v56; // [esp+170h] [ebp-34h]
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
        v19 = (tScriptStruct_ActorVendeur*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
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
        Vendeur_Vendeur_exec_dialog__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_exec_dialog__PJ_Sally_gao_current_pm_mode,
            (void*)Vendeur_Vendeur_exec_dialog__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_exec_dialog__PJ_Sally_gao_current_pm_priority,
            (void*)Vendeur_Vendeur_exec_dialog__PJ_Sally_gao,
            576);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_i_sally_priority,
            (void*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao,
            396);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_o_sally_contextuel,
            (void*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao,
            388);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_i_sally_action,
            (void*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao,
            412);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_v_sally_offset,
            (void*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao,
            400);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_i_force_interface,
            (void*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao,
            3292);
        Vendeur_Vendeur_exec_dialog_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_exec_dialog_univVars_Flag_speciaux_sauve,
            (void*)Vendeur_Vendeur_exec_dialog_univVars,
            4488);
        Vendeur_Vendeur_exec_dialog_TXT_Sally_gao = AI_ul_RealResolveOneRefObject(0x7200064Eu);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_ai_dialog_consequences,
            (void*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao,
            1432);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_o_dialogue_actor,
            (void*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao,
            1100);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_at_dialog_questions,
            (void*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao,
            1104);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_at_dialog_reponses,
            (void*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao,
            1268);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_ai_dialog_grisage,
            (void*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao,
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
    v28 = (tGameObjectMain*)Vendeur_Vendeur_exec_dialog__PJ_Sally_gao;
    if (Vendeur_Vendeur_exec_dialog__PJ_Sally_gao)
    {
        if ((unsigned int)Vendeur_Vendeur_exec_dialog__PJ_Sally_gao <= 2)
            v28 = AI_MainActor[Vendeur_Vendeur_exec_dialog__PJ_Sally_gao - 1];
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v28);
    }
    else
    {
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v55 = (float)v1;
    if (v56->des_f_distance_talk * v56->des_f_distance_talk > v55
        && !AI_EvalFunc_OBJ_CustomBitsTest_C((tGameObjectMain*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao, 2048))
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Vendeur_Vendeur_exec_dialog__PJ_Sally_gao, &a3);
        MATH_SubstractVec(&dest, &a2, &a3);
        v40 = dest;
        AI_EvalFunc_MATHVecSetHorzNormalize_C(&v40);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v59);
        v35 = v40.z * v59.z + v40.y * v59.y + v59.x * v40.x;
        if (v56->des_f_angle_talk <= (double)v35)
        {
            AI_EvalFunc_MATHVecNormalize_C(&v40, &v47);
            v50 = v47;
            AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Vendeur_Vendeur_exec_dialog__PJ_Sally_gao, &v58);
            v36 = v50.z * v58.z + v50.y * v58.y + v58.x * v50.x;
            v38 = v36 * 5.0f;
            v27 = v38 + -10.0 + MATH_gd_magicDoubleForRound;
            v60 = (*(uint32_t*)&v27);
            v46 = (unsigned __int16)*(uint32_t*)(Vendeur_Vendeur_exec_dialog__PJ_Sally_gao + 4);
            if ((v46 & 1) != 0)
                v57 = !*(uint32_t*)Vendeur_Vendeur_exec_dialog__PJ_Sally_gao_current_pm_mode
                || *(int*)Vendeur_Vendeur_exec_dialog__PJ_Sally_gao_current_pm_priority < 20;
            else
                v57 = 0;
            if (v57)
            {
                v18 = (float)v60;
                if (v18 > (double)*(float*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_i_sally_priority)
                {
                    *(uint32_t*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_o_sally_contextuel = (uint32_t)AI_gpst_CurrentGameObject;
                    *(float*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_i_sally_priority = (float)v60;
                    *(uint32_t*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_i_sally_action = 5;
                    *(uint32_t*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_v_sally_offset = 0;
                    *(uint32_t*)(Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_v_sally_offset + 4) = 0;
                    *(uint32_t*)(Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_v_sally_offset + 8) = 0x3F800000;
                    *(uint32_t*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_i_force_interface = 1;
                }
            }
            v56->i_talk_propose = v57;
        }
    }
    v43 = 0;
    v44 = 0;
    if (v56->m_dialogue.msgID == 10 && (v41 = AI_EvalFunc_TIMEElapsed_C(v56->f_time_fin_talk, 0.5)))
    {
        v51 = AI_EvalFunc_ARRBitTest_C((int*)(Vendeur_Vendeur_exec_dialog_univVars_Flag_speciaux_sauve + 4), 2083);
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
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)Vendeur_Vendeur_exec_dialog_TXT_Sally_gao, &Src);
            v56->m_dialogue.msgID = 0;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v43 = 1;
        if (v56->Valide_Effet_1_Question)
            AI_EvalFunc_OBJ_CapaSet_C((tGameObjectMain*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao, 0x80, 0);
        else
            AI_EvalFunc_OBJ_CapaSet_C((tGameObjectMain*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao, 0, 0x80);
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
                    AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao, 0x400, 0);
                    continue;
                }
                if (*(int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4) <= 10000)
                {
                    if (*(int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4) > 0)
                    {
                        v21 = (int*)(Vendeur_Vendeur_exec_dialog_univVars_Flag_speciaux_sauve + 4);
                        v5 = (int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4);
                        AI_EvalFunc_ARRBitSet_C(v21, *v5);
                    }
                }
                else if (*(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_plus_Dimension1, i - 3, a4) - 10000 > 0)
                {
                    v22 = (int*)(Vendeur_Vendeur_exec_dialog_univVars_Flag_speciaux_sauve + 4);
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
                        a1 = (int*)(Vendeur_Vendeur_exec_dialog_univVars_Flag_speciaux_sauve + 4);
                        v3 = (int*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_Dimension1, i, a4);
                        AI_EvalFunc_ARRBitSet_C(a1, *v3);
                    }
                }
                else if (*(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_Dimension1, i, a4) - 10000 > 0)
                {
                    v24 = (int*)(Vendeur_Vendeur_exec_dialog_univVars_Flag_speciaux_sauve + 4);
                    v2 = (uint32_t*)AI_EvalVarArray2_C(1, (char*)&v56->__ai_consequences_Dimension1, i, a4);
                    AI_EvalFunc_ARRBitClear_C(v24, *v2 - 10000);
                }
            }
        }
    }
    v56->f_time_fin_talk = TIM_gf_MainClock - TIM_gf_LockTime;
    for (i = 0; i < 5; ++i)
        *(uint32_t*)(Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_ai_dialog_consequences + 4 * i + 4) = 0;
LABEL_73:
    if (v43)
    {
        *(uint32_t*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_o_dialogue_actor = (uint32_t)AI_gpst_CurrentGameObject;
        num = 0;
        for (i = 0; i < 20; ++i)
        {
            v20 = (int*)(Vendeur_Vendeur_exec_dialog_univVars_Flag_speciaux_sauve + 4);
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
                    v7 = AI_EvalVarArray_C(10, (char*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_at_dialog_questions, num);
                    memcpy(v7, v13, 8u);
                    v14 = (const void*)AI_EvalVarArray2_C(10, (char*)&v56->__at_questionsReponses_Dimension1, 1, i);
                    v8 = AI_EvalVarArray_C(10, (char*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_at_dialog_reponses, num);
                    memcpy(v8, v14, 8u);
                    *(uint32_t*)(Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_ai_dialog_grisage + 4 * num + 4) = v56->ai_grisage[i];
                    *(uint32_t*)(Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_ai_dialog_consequences + 4 * num++ + 4) = i + 1;
                }
            }
        }
        v15 = AI_EvalVarArray_C(10, (char*)&v56->__at_question_reponse_OK_Dimension1, 0);
        v9 = AI_EvalVarArray_C(10, (char*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_at_dialog_questions, num);
        memcpy(v9, v15, 8u);
        v16 = AI_EvalVarArray_C(10, (char*)&v56->__at_question_reponse_OK_Dimension1, 1);
        v10 = AI_EvalVarArray_C(10, (char*)Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_at_dialog_reponses, num);
        memcpy(v10, v16, 8u);
        *(uint32_t*)(Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_ai_dialog_grisage + 4 * num + 4) = -1;
        *(uint32_t*)(Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_ai_dialog_consequences + 4 * num + 4) = 253;
        for (i = num + 1; i < 20; ++i)
        {
            *(uint32_t*)(Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_ai_dialog_grisage + 4 * i + 4) = 0;
            *(uint32_t*)(Vendeur_Vendeur_exec_dialog_GST_Board_panel_gao_ai_dialog_consequences + 4 * i + 4) = 0;
        }
    }
    v56->m_dialogue.msgID = 0;
    return 1000;
}

int Vendeur_Vendeur_Comportements()
{
    tScriptStruct_ActorVendeur* v1; // [esp+0h] [ebp-28h]
    int v2; // [esp+Ch] [ebp-1Ch]
    int v3; // [esp+10h] [ebp-18h]
    int v4; // [esp+14h] [ebp-14h]
    int v5; // [esp+18h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ActorVendeur*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Vendeur_Vendeur_Comportements_Vendeur_Vendeur_Comp_Bio = (int)AI_ul_RealResolveOneRefFunction(0x26000D31u);
        Vendeur_Vendeur_Comportements_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_Comportements_univVars_Flag_speciaux_sauve,
            (void*)Vendeur_Vendeur_Comportements_univVars,
            4488);
        fct_Vendeur_Vendeur_Comportements_Vendeur_Vendeur_Comp_Journaux = (int)AI_ul_RealResolveOneRefFunction(0x26000D32u);
        fct_Vendeur_Vendeur_Comportements_Vendeur_Vendeur_mamago = (int)AI_ul_RealResolveOneRefFunction(0x3D003032u);
        fct_Vendeur_Vendeur_Comportements_Vendeur_Vendeur_Comp_Simple = (int)AI_ul_RealResolveOneRefFunction(0x260013F8u);
    }
    switch (v1->i_comportement)
    {
    case 1:
        v4 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)Vendeur_Vendeur_Comportements_Vendeur_Vendeur_Comp_Bio);
        if (v4 != 1000)
            return v4;
        break;
    case 2:
        v1->i_status1 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Vendeur_Vendeur_Comportements_univVars_Flag_speciaux_sauve + 4),
            2272);
        v1->i_status2 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Vendeur_Vendeur_Comportements_univVars_Flag_speciaux_sauve + 4),
            2273);
        v3 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Vendeur_Vendeur_Comportements_Vendeur_Vendeur_Comp_Journaux);
        if (v3 != 1000)
            return v3;
        break;
    case 4:
        v2 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Vendeur_Vendeur_Comportements_Vendeur_Vendeur_mamago);
        if (v2 != 1000)
            return v2;
        break;
    default:
        v5 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Vendeur_Vendeur_Comportements_Vendeur_Vendeur_Comp_Simple);
        if (v5 != 1000)
            return v5;
        break;
    }
    return 1000;
}

int Vendeur_Vendeur_Comp_Simple()
{
    tScriptStruct_ActorVendeur* v1; // [esp+0h] [ebp-18h]
    tGameObjectMain* v2; // [esp+4h] [ebp-14h]
    tGameObjectMain* v3; // [esp+8h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ActorVendeur*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Vendeur_Vendeur_Comp_Simple_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_Comp_Simple_GST_Board_panel_gao_o_dialogue_actor,
            (void*)Vendeur_Vendeur_Comp_Simple_GST_Board_panel_gao,
            1100);
        Vendeur_Vendeur_Comp_Simple__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
    }
    v1->des_o_acteur_suividesyeux = 0;
    if (AI_EvalFunc_OBJ_CustomBitsTest_C((tGameObjectMain*)Vendeur_Vendeur_Comp_Simple_GST_Board_panel_gao, 0x800))
    {
        v3 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v3 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            v2 = v3;
        }
        else
        {
            v2 = 0;
        }
        if (*(tGameObjectMain**)Vendeur_Vendeur_Comp_Simple_GST_Board_panel_gao_o_dialogue_actor == v2)
            goto LABEL_15;
    }
    if (v1->i_regarde_si_talk && v1->i_talk_propose)
    {
    LABEL_15:
        v1->des_o_acteur_suividesyeux = Vendeur_Vendeur_Comp_Simple__PJ_Sally_gao;
        v1->des_f_RT_vitesse_de_rotation = 0.1f;
        v1->des_i_RT_flags = 0x80010000;
    }
    return 1000;
}

int Vendeur_Vendeur_Comp_Journaux()
{
    int v0; // eax
    tScriptStruct_ActorVendeur* v2; // [esp+0h] [ebp-64h]
    tGameObjectMain* v3; // [esp+8h] [ebp-5Ch]
    tGameObjectMain* v4; // [esp+Ch] [ebp-58h]
    tGameObjectMain* v5; // [esp+10h] [ebp-54h]
    tGameObjectMain* v6; // [esp+14h] [ebp-50h]
    tGameObjectMain* v7; // [esp+18h] [ebp-4Ch]
    unsigned int v8; // [esp+2Ch] [ebp-38h]
    int i; // [esp+44h] [ebp-20h]
    int v10; // [esp+48h] [ebp-1Ch]
    int v11; // [esp+4Ch] [ebp-18h]
    int v12; // [esp+4Ch] [ebp-18h]
    int v13; // [esp+5Ch] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorVendeur*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Vendeur_Vendeur_Comp_Journaux_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_Comp_Journaux_univVars_Flag_speciaux_sauve,
            (void*)Vendeur_Vendeur_Comp_Journaux_univVars,
            4488);
        Vendeur_Vendeur_Comp_Journaux_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &Vendeur_Vendeur_Comp_Journaux_GST_Board_panel_gao_o_dialogue_actor,
            (void*)Vendeur_Vendeur_Comp_Journaux_GST_Board_panel_gao,
            1100);
        Vendeur_Vendeur_Comp_Journaux__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
    }
    v13 = 0;
    v10 = 0;
    v2->des_o_acteur_suividesyeux = 0;
    v11 = AI_EvalFunc_ARRBitTest_C((int*)(Vendeur_Vendeur_Comp_Journaux_univVars_Flag_speciaux_sauve + 4), 2272);
    if (!v2->i_status1 && v11)
        v13 = 1;
    v2->i_status1 = v11;
    v12 = AI_EvalFunc_ARRBitTest_C((int*)(Vendeur_Vendeur_Comp_Journaux_univVars_Flag_speciaux_sauve + 4), 2273);
    if (!v2->i_status2 && v12)
        v13 = 1;
    v2->i_status2 = v12;
    if (!v13)
    {
        if (!AI_EvalFunc_OBJ_CustomBitsTest_C((tGameObjectMain*)Vendeur_Vendeur_Comp_Journaux_GST_Board_panel_gao, 2048))
            goto LABEL_21;
        v7 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v7 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            v6 = v7;
        }
        else
        {
            v6 = 0;
        }
        if (*(tGameObjectMain**)Vendeur_Vendeur_Comp_Journaux_GST_Board_panel_gao_o_dialogue_actor == v6)
        {
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x100, 0);
            v2->des_o_acteur_suividesyeux = Vendeur_Vendeur_Comp_Journaux__PJ_Sally_gao;
            v2->des_f_RT_vitesse_de_rotation = 0.1f;
            v2->des_i_RT_flags = 0x80010000;
            v2->f_counter = 0.0;
        }
        else
        {
        LABEL_21:
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
            for (i = 0; i < 5; ++i)
            {
                v8 = v2->o_pos_acheteur[i];
                v5 = (tGameObjectMain*)v8;
                if (v8)
                {
                    if (v8 <= 2)
                        v5 = AI_MainActor[v8 - 1];
                    v4 = v5;
                }
                else
                {
                    v4 = 0;
                }
                if (v4)
                {
                    v3 = (tGameObjectMain*)v2->o_pos_acheteur[i];
                    if (v8)
                    {
                        if (v8 <= 2)
                            v3 = AI_MainActor[v8 - 1];
                        v0 = AI_EvalFunc_OBJ_CapaGet_C(v3);
                    }
                    else
                    {
                        v0 = AI_EvalFunc_OBJ_CapaGet_C(0);
                    }
                    if (!v0)
                    {
                        v2->des_o_acteur_suividesyeux = v8;
                        v2->des_f_RT_vitesse_de_rotation = 0.1f;
                        v2->des_i_RT_flags = 0x80010000;
                        v2->des_v_hauteur_cible.x = 0.0;
                        v2->des_v_hauteur_cible.y = 0.0;
                        v2->des_v_hauteur_cible.z = 1.5;
                        v10 = 1;
                        v2->f_counter = TIM_gf_dt + v2->f_counter;
                        if (v2->f_counter > 2.0)
                        {
                            v13 = 1;
                            v2->f_counter = 0.0;
                        }
                        break;
                    }
                }
            }
            if (!v10)
                v2->f_counter = 0.0;
        }
    }
    if (!v10 && v2->i_regarde_si_talk && v2->i_talk_propose)
    {
        v2->des_o_acteur_suividesyeux = Vendeur_Vendeur_Comp_Journaux__PJ_Sally_gao;
        v2->des_f_RT_vitesse_de_rotation = 0.1f;
        v2->des_i_RT_flags = -2147418112;
    }
    if (v13)
    {
        AI_EvalFunc_ACTActionSet_C_CURRENT(1);
    }
    else if (AI_EvalFunc_ACTActionFinished_C_CURRENT())
    {
        AI_EvalFunc_ACTActionSet_C_CURRENT(0);
    }
    return 1000;
}