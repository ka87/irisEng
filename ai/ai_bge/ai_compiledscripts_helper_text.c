#include "ai_compiledscripts_inc.h"

//helper_text_hlptxt_init
int helper_text_hlptxt_init_univVars;
int helper_text_hlptxt_init_univVars_Flag_speciaux_sauve;
int fct_helper_text_hlptxt_init_helper_text_hlptxt_init;
int fct_helper_text_hlptxt_init_helper_text_hlptxt_tests_depart;

//helper_text_hlptxt_exec_fin_ou_abort
int helper_text_hlptxt_exec_fin_ou_abort_univVars;
int helper_text_hlptxt_exec_fin_ou_abort_univVars_Flag_speciaux_sauve;
int helper_text_hlptxt_exec_fin_ou_abort_GST_AffichageTexte_gao;
int helper_text_hlptxt_exec_fin_ou_abort_GST_AffichageTexte_gao_i_affiche;
int helper_text_hlptxt_exec_fin_ou_abort_GST_AffichageTexte_gao_prio_message_a_traiter_displa;

//helper_text_hlptxt_exec_tests_interruption
int fct_helper_text_hlptxt_exec_tests_interruption_helper_text_hlptxt_exec_fin_ou_abort;
int helper_text_hlptxt_exec_tests_interruption_GST_Global_gao;
int helper_text_hlptxt_exec_tests_interruption_GST_Global_gao_i_HlpTxt_Interdit_old;
int helper_text_hlptxt_exec_tests_interruption_univVars;
int helper_text_hlptxt_exec_tests_interruption_univVars_i_INV_OptionsFlags;
int helper_text_hlptxt_exec_tests_interruption_GST_AffichageTexte_gao;
int helper_text_hlptxt_exec_tests_interruption_GST_AffichageTexte_gao_o_dernier_snd_txt;
int helper_text_hlptxt_exec_tests_interruption_GST_AffichageTexte_gao_o_dernier_visual_txt;
int helper_text_hlptxt_exec_tests_interruption_GST_AffichageTexte_gao_message_a_traiter;
int helper_text_hlptxt_exec_tests_interruption_GST_Board_panel_gao;
int helper_text_hlptxt_exec_tests_interruption_GST_Board_panel_gao_i_force_interface;

//helper_text_hlptxt_tests_wait
int helper_text_hlptxt_tests_wait_univVars;
int helper_text_hlptxt_tests_wait_univVars_Perso_ID_actor;
int helper_text_hlptxt_tests_wait_GST_AffichageTexte_gao;
int helper_text_hlptxt_tests_wait_GST_AffichageTexte_gao_prio_message_a_traiter_displa;
int fct_helper_text_hlptxt_tests_wait_helper_text_hlptxt_tests_depart;
int helper_text_hlptxt_tests_wait_GST_Global_gao;
int fct_helper_text_hlptxt_tests_wait_helper_text_hlptxt_exec_fin_ou_abort;
int helper_text_hlptxt_tests_wait_GST_Board_panel_gao;
int helper_text_hlptxt_tests_wait_INV_MatFade_gao;
int fct_helper_text_hlptxt_tests_wait_helper_text_hlptxt_exec_ask_display;
int helper_text_hlptxt_tests_wait_univVars_Flag_speciaux_sauve;
int fct_helper_text_hlptxt_tests_wait_helper_text_hlptxt_exec_tests_interruption;
int helper_text_hlptxt_tests_wait_GST_AffichageTexte_gao_prio_message_a_traiter_displa_message_a_traiter;
int helper_text_hlptxt_tests_wait_GST_AffichageTexte_gao_prio_message_a_traiter;

//helper_text_hlptxt_tests_depart
int helper_text_hlptxt_tests_depart_univVars;
int helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve;
int fctvar_helper_text_hlptxt_tests_depart_UNKNOWN1;
int fctvar_helper_text_hlptxt_tests_depart_UNKNOWN2;
int fctr_helper_text_hlptxt_tests_depart_helper_text_hlptxt_tests_wait;

//helper_text_hlptxt_exec_ask_display
int fct_helper_text_hlptxt_exec_ask_display_helper_text_hlptxt_tests_depart;
int helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao;
int helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_texte_message_a_traiter;
int helper_text_hlptxt_exec_ask_display_GST_Global_gao;
int helper_text_hlptxt_exec_ask_display_GST_Global_gao_f_game_speed;
int helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_i_encart;
int helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_i_Sound_ID;
int helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_f_Sound_Volume;
int helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_f_near;
int helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_f_far;
int helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_i_Sound_Localisation_type;
int helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_message_a_traiter;
int helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_prio_message_a_traiter;
int helper_text_hlptxt_exec_ask_display_GST_Board_panel_gao;

int helper_text_hlptxt_init()
{
    tScriptStruct_HelperText* v1; // [esp+0h] [ebp-90h]
    tGameObjectMain* v2; // [esp+4h] [ebp-8Ch]
    tGameObjectMain* v3; // [esp+8h] [ebp-88h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-84h]
    tGameObjectMain* v5; // [esp+10h] [ebp-80h]
    tGameObjectMain* v6; // [esp+14h] [ebp-7Ch]
    tGameObjectMain* v7; // [esp+18h] [ebp-78h]
    tGameObjectMain* v8; // [esp+1Ch] [ebp-74h]
    tGameObjectMain* v9; // [esp+20h] [ebp-70h]
    tGameObjectMain* v10; // [esp+24h] [ebp-6Ch]
    tGameObjectMain* v11; // [esp+28h] [ebp-68h]
    tGameObjectMain* v12; // [esp+2Ch] [ebp-64h]
    tGameObjectMain* v13; // [esp+30h] [ebp-60h]
    tGameObjectMain* v14; // [esp+3Ch] [ebp-54h]
    tGameObjectMain* v15; // [esp+40h] [ebp-50h]
    tGameObjectMain* v16; // [esp+4Ch] [ebp-44h]
    tGameObjectMain* v17; // [esp+50h] [ebp-40h]
    tGameObjectMain* v18; // [esp+54h] [ebp-3Ch]
    tGameObjectMain* v19; // [esp+58h] [ebp-38h]
    int v20; // [esp+6Ch] [ebp-24h]
    int v21; // [esp+7Ch] [ebp-14h]
    int v22; // [esp+8Ch] [ebp-4h]
    int v23; // [esp+8Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_HelperText*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        helper_text_hlptxt_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_init_univVars_Flag_speciaux_sauve,
            (void*)helper_text_hlptxt_init_univVars,
            4488);
        fct_helper_text_hlptxt_init_helper_text_hlptxt_init = (int)AI_ul_RealResolveOneRefFunction(0x35007412u);
        fct_helper_text_hlptxt_init_helper_text_hlptxt_tests_depart = (int)AI_ul_RealResolveOneRefFunction(0x35007498u);
    }
    v1->des_i_look_interdit = 0;
    v1->des_i_inventaire_interdit = 0;
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0xFFFF);
    AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(0, 65280);
    if (v1->des_i_cine_temporaire)
        AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x8000, 0);
    if (!v1->des_i_ripley && (v1->des_i_SPFlag_associe_vrai || v1->des_i_SPFlag_associe_faux))
    {
        v21 = 0;
        v1->i_tests_a_reussir = 0;
        if (v1->des_i_SPFlag_associe_vrai)
        {
            ++v1->i_tests_a_reussir;
            if (v1->des_i_SPFlag_associe_vrai <= 0)
                v22 = 0;
            else
                v22 = v1->des_i_SPFlag_associe_vrai < 10000 ? AI_EvalFunc_ARRBitTest_C(
                    (int*)(helper_text_hlptxt_init_univVars_Flag_speciaux_sauve + 4),
                    v1->des_i_SPFlag_associe_vrai) : 1
                - AI_EvalFunc_ARRBitTest_C(
                    (int*)(helper_text_hlptxt_init_univVars_Flag_speciaux_sauve
                        + 4),
                    v1->des_i_SPFlag_associe_vrai
                    - 10000);
            if (v22 == 1)
                v21 = 1;
        }
        if (v1->des_i_SPFlag_associe_faux)
        {
            ++v1->i_tests_a_reussir;
            if (v1->des_i_SPFlag_associe_faux <= 0)
                v23 = 0;
            else
                v23 = v1->des_i_SPFlag_associe_faux < 10000 ? AI_EvalFunc_ARRBitTest_C(
                    (int*)(helper_text_hlptxt_init_univVars_Flag_speciaux_sauve + 4),
                    v1->des_i_SPFlag_associe_faux) : 1
                - AI_EvalFunc_ARRBitTest_C(
                    (int*)(helper_text_hlptxt_init_univVars_Flag_speciaux_sauve
                        + 4),
                    v1->des_i_SPFlag_associe_faux
                    - 10000);
            if (!v23)
                ++v21;
        }
        if (v21 >= v1->i_tests_a_reussir)
        {
            v20 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v20 != 1000)
                return v20;
        }
    }
    v1->f_track_init = fct_helper_text_hlptxt_init_helper_text_hlptxt_init;
    v1->i_index_ligne = -1;
    v1->f_tempo = v1->des_f_tempo;
    v1->i_tests_a_reussir = 0;
    if (v1->des_i_choix_aleatoire)
        v1->i_display_once = 0;
    if (v1->des_i_actor_type1)
    {
        v19 = (tGameObjectMain*)v1->des_o_actor_zone1;
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
        if (!v18)
            v1->des_o_actor_zone1 = (int)AI_gpst_CurrentGameObject;
    }
    v17 = (tGameObjectMain*)v1->des_o_actor_zone1;
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
        ++v1->i_tests_a_reussir;
    v15 = (tGameObjectMain*)v1->des_o_actor_zone2;
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
    if (v14)
        v1->i_tests_a_reussir += 2;
    v13 = (tGameObjectMain*)v1->des_o_actor_status1;
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
        v1->i_tests_a_reussir += 4;
    v11 = (tGameObjectMain*)v1->des_o_actor_capa1;
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
    if (v10)
        v1->i_tests_a_reussir += 8;
    v9 = (tGameObjectMain*)v1->des_o_actor_cbit1;
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
        v1->i_tests_a_reussir += 0x10;
    v7 = (tGameObjectMain*)v1->des_o_acteur_msg;
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
        v1->i_tests_a_reussir += 0x20;
    v5 = (tGameObjectMain*)v1->des_o_actor_visible1;
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
    if (v4)
        v1->i_tests_a_reussir += 0x40;
    if (v1->des_i_specialFlag_test1)
        v1->i_tests_a_reussir += 0x80;
    if (v1->des_i_specialFlag_test2)
        v1->i_tests_a_reussir += 0x100;
    v3 = (tGameObjectMain*)v1->des_o_actor_desstruct;
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
    if (v2)
        v1->i_tests_a_reussir += 0x200;
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_helper_text_hlptxt_init_helper_text_hlptxt_tests_depart);
    return 1000;
}

int helper_text_hlptxt_exec_fin_ou_abort()
{
    int result; // eax
    tScriptStruct_HelperText* v1; // [esp+0h] [ebp-20h]
    int a1; // [esp+10h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_HelperText*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        helper_text_hlptxt_exec_fin_ou_abort_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_exec_fin_ou_abort_univVars_Flag_speciaux_sauve,
            (void*)helper_text_hlptxt_exec_fin_ou_abort_univVars,
            4488);
        helper_text_hlptxt_exec_fin_ou_abort_GST_AffichageTexte_gao = AI_ul_RealResolveOneRefObject(0x7100E7ACu);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_exec_fin_ou_abort_GST_AffichageTexte_gao_i_affiche,
            (void*)helper_text_hlptxt_exec_fin_ou_abort_GST_AffichageTexte_gao,
            204);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_exec_fin_ou_abort_GST_AffichageTexte_gao_prio_message_a_traiter_displa,
            (void*)helper_text_hlptxt_exec_fin_ou_abort_GST_AffichageTexte_gao,
            140);
    }
    if (!v1->des_i_Flag_debut)
    {
        if (v1->des_i_SPFlag_associe_vrai > 0)
            AI_EvalFunc_ARRBitSet_C(
                (int*)(helper_text_hlptxt_exec_fin_ou_abort_univVars_Flag_speciaux_sauve + 4),
                v1->des_i_SPFlag_associe_vrai);
        if (v1->des_i_SPFlag_associe_faux > 0)
            AI_EvalFunc_ARRBitClear_C(
                (int*)(helper_text_hlptxt_exec_fin_ou_abort_univVars_Flag_speciaux_sauve + 4),
                v1->des_i_SPFlag_associe_faux);
    }
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
    if (v1->des_i_cine_temporaire)
    {
        a1 = AI_EvalFunc_WORCurIndexGet_C();
        AI_EvalFunc_WORUnPause_C(a1, 0x20);
        AI_EvalFunc_GFXDel_C_CURRENT(v1->i_sfx_temp);
        AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0, 0x8000000);
        AI_EvalFunc_OBJ_FlagsControlSet_C(
            (tGameObjectMain*)helper_text_hlptxt_exec_fin_ou_abort_GST_AffichageTexte_gao,
            0,
            0x8000000);
    }
    if (v1->des_i_ripley == 1)
    {
        if (v1->f_dist_abort != 0.0 && *(uint32_t*)helper_text_hlptxt_exec_fin_ou_abort_GST_AffichageTexte_gao_i_affiche)
        {
            *(uint32_t*)helper_text_hlptxt_exec_fin_ou_abort_GST_AffichageTexte_gao_prio_message_a_traiter_displa = 1;
            *(uint32_t*)helper_text_hlptxt_exec_fin_ou_abort_GST_AffichageTexte_gao_i_affiche = -1;
        }
        v1->i_index_ligne = -1;
        if (!v1->i_replay_loop)
            v1->i_ready = 0;
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)v1->f_track_init);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 4;
    }
    else
    {
        AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 4;
    }
    return result;
}

int helper_text_hlptxt_exec_tests_interruption()
{
    double v0; // st7
    int result; // eax
    int v2; // edx
    tScriptStruct_HelperText* v3; // [esp+8h] [ebp-F8h]
    tAI* v4; // [esp+Ch] [ebp-F4h]
    tGameObjectMain* v5; // [esp+10h] [ebp-F0h]
    tGameObjectMain* v6; // [esp+14h] [ebp-ECh]
    tAI* v7; // [esp+18h] [ebp-E8h]
    tGameObjectMain* v8; // [esp+1Ch] [ebp-E4h]
    tGameObjectMain* v9; // [esp+20h] [ebp-E0h]
    tAI* v10; // [esp+24h] [ebp-DCh]
    tGameObjectMain* v11; // [esp+28h] [ebp-D8h]
    tGameObjectMain* v12; // [esp+2Ch] [ebp-D4h]
    tAI* v13; // [esp+30h] [ebp-D0h]
    tGameObjectMain* v14; // [esp+34h] [ebp-CCh]
    tGameObjectMain* v15; // [esp+38h] [ebp-C8h]
    tAI* v16; // [esp+3Ch] [ebp-C4h]
    tGameObjectMain* v17; // [esp+40h] [ebp-C0h]
    tGameObjectMain* v18; // [esp+44h] [ebp-BCh]
    tAI* v19; // [esp+48h] [ebp-B8h]
    tGameObjectMain* v20; // [esp+4Ch] [ebp-B4h]
    tGameObjectMain* v21; // [esp+50h] [ebp-B0h]
    tAI* v22; // [esp+54h] [ebp-ACh]
    tGameObjectMain* v23; // [esp+58h] [ebp-A8h]
    tGameObjectMain* v24; // [esp+5Ch] [ebp-A4h]
    tGameObjectMain* v25; // [esp+60h] [ebp-A0h]
    tGameObjectMain* v26; // [esp+64h] [ebp-9Ch]
    tGameObjectMain* v27; // [esp+68h] [ebp-98h]
    tGameObjectMain* v28; // [esp+6Ch] [ebp-94h]
    tGameObjectMain* v29; // [esp+74h] [ebp-8Ch]
    tAI* v30; // [esp+7Ch] [ebp-84h]
    int v31; // [esp+80h] [ebp-80h]
    void* v32; // [esp+8Ch] [ebp-74h]
    int v33; // [esp+94h] [ebp-6Ch]
    tMessage Src; // [esp+98h] [ebp-68h] BYREF
    tGameObjectMain* v35; // [esp+CCh] [ebp-34h]
    int v37; // [esp+E4h] [ebp-1Ch]
    float v38; // [esp+E8h] [ebp-18h]
    int v39; // [esp+ECh] [ebp-14h]
    tScriptStruct_HelperText* v40; // [esp+F0h] [ebp-10h]
    int v41; // [esp+F8h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_HelperText*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v40 = v3;
    v39 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_helper_text_hlptxt_exec_tests_interruption_helper_text_hlptxt_exec_fin_ou_abort = (int)AI_ul_RealResolveOneRefFunction(0x7100E7B7u);
        helper_text_hlptxt_exec_tests_interruption_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_exec_tests_interruption_GST_Global_gao_i_HlpTxt_Interdit_old,
            (void*)helper_text_hlptxt_exec_tests_interruption_GST_Global_gao,
            260);
        helper_text_hlptxt_exec_tests_interruption_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_exec_tests_interruption_univVars_i_INV_OptionsFlags,
            (void*)helper_text_hlptxt_exec_tests_interruption_univVars,
            7656);
        helper_text_hlptxt_exec_tests_interruption_GST_AffichageTexte_gao = AI_ul_RealResolveOneRefObject(0x7100E7ACu);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_exec_tests_interruption_GST_AffichageTexte_gao_o_dernier_snd_txt,
            (void*)helper_text_hlptxt_exec_tests_interruption_GST_AffichageTexte_gao,
            168);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_exec_tests_interruption_GST_AffichageTexte_gao_o_dernier_visual_txt,
            (void*)helper_text_hlptxt_exec_tests_interruption_GST_AffichageTexte_gao,
            172);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_exec_tests_interruption_GST_AffichageTexte_gao_message_a_traiter,
            (void*)helper_text_hlptxt_exec_tests_interruption_GST_AffichageTexte_gao,
            0);
        helper_text_hlptxt_exec_tests_interruption_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_exec_tests_interruption_GST_Board_panel_gao_i_force_interface,
            (void*)helper_text_hlptxt_exec_tests_interruption_GST_Board_panel_gao,
            3292);
    }
    if (v40->f_dist_abort == 0.0f)
        goto LABEL_108;
    v29 = *AI_MainActor;
    if (*AI_MainActor)
    {
        if ((unsigned int)v29 <= 2)
            v29 = AI_MainActor[(uint32_t)v29 - 1];
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v29);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v38 = (float)v0;
    if (v40->f_dist_abort * v40->f_dist_abort >= v38)
    {
    LABEL_108:
        if (*(uint32_t*)helper_text_hlptxt_exec_tests_interruption_GST_Global_gao_i_HlpTxt_Interdit_old)
        {
            v31 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_helper_text_hlptxt_exec_tests_interruption_helper_text_hlptxt_exec_fin_ou_abort);
            if (v31 == 1000)
            {
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 1000;
            }
            else
            {
                result = v31;
            }
        }
        else
        {
            v2 = *(uint32_t*)helper_text_hlptxt_exec_tests_interruption_univVars_i_INV_OptionsFlags & 1;
            v39 = 1 - v2;
            if (v2 == 1)
            {
                v28 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                        v28 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                    v27 = v28;
                }
                else
                {
                    v27 = 0;
                }
                if (*(tGameObjectMain**)helper_text_hlptxt_exec_tests_interruption_GST_AffichageTexte_gao_o_dernier_snd_txt != v27)
                    goto LABEL_34;
            }
            if (!v39)
                goto LABEL_109;
            v35 = AI_gpst_CurrentGameObject;
            v26 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v26 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v25 = v26;
            }
            else
            {
                v25 = 0;
            }
            if (*(tGameObjectMain**)helper_text_hlptxt_exec_tests_interruption_GST_AffichageTexte_gao_o_dernier_visual_txt == v25)
            {
            LABEL_109:
                if (v40->des_i_cine_temporaire)
                {
                    if (AI_EvalFunc_TIMEElapsed_C(v40->f_temps, 2.0))
                    {
                        v30 = (tAI*)helper_text_hlptxt_exec_tests_interruption_GST_Board_panel_gao;
                        if ((tAI*)helper_text_hlptxt_exec_tests_interruption_GST_Board_panel_gao == WOR_gst_Universe.univAIInstance)
                        {
                            v22 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v24 = (tGameObjectMain*)helper_text_hlptxt_exec_tests_interruption_GST_Board_panel_gao;
                            if (helper_text_hlptxt_exec_tests_interruption_GST_Board_panel_gao)
                            {
                                if ((unsigned int)helper_text_hlptxt_exec_tests_interruption_GST_Board_panel_gao <= 2)
                                    v24 = AI_MainActor[helper_text_hlptxt_exec_tests_interruption_GST_Board_panel_gao - 1];
                                v23 = v24;
                            }
                            else
                            {
                                v23 = 0;
                            }
                            v22 = v23->extended->AIInstance;
                        }
                        if (((tScriptStruct_GSTBoardPanel*)(v22->instanceVarDataCurrent))->i_sally_priority < 1000.0)
                        {
                            if ((tAI*)helper_text_hlptxt_exec_tests_interruption_GST_Board_panel_gao == WOR_gst_Universe.univAIInstance)
                            {
                                v19 = WOR_gst_Universe.univAIInstance;
                            }
                            else
                            {
                                v21 = (tGameObjectMain*)helper_text_hlptxt_exec_tests_interruption_GST_Board_panel_gao;
                                if (helper_text_hlptxt_exec_tests_interruption_GST_Board_panel_gao)
                                {
                                    if ((unsigned int)helper_text_hlptxt_exec_tests_interruption_GST_Board_panel_gao <= 2)
                                        v21 = AI_MainActor[helper_text_hlptxt_exec_tests_interruption_GST_Board_panel_gao - 1];
                                    v20 = v21;
                                }
                                else
                                {
                                    v20 = 0;
                                }
                                v19 = v20->extended->AIInstance;
                            }
                            ((tScriptStruct_GSTBoardPanel*)(v19->instanceVarDataCurrent))->o_sally_contextuel = (int)AI_gpst_CurrentGameObject;
                            if (v30 == WOR_gst_Universe.univAIInstance)
                            {
                                v16 = WOR_gst_Universe.univAIInstance;
                            }
                            else
                            {
                                v18 = (tGameObjectMain*)v30;
                                if (v30)
                                {
                                    if ((unsigned int)v30 <= 2)
                                        v18 = AI_MainActor[(uint32_t)v30 - 1];
                                    v17 = v18;
                                }
                                else
                                {
                                    v17 = 0;
                                }
                                v16 = v17->extended->AIInstance;
                            }
                            ((tScriptStruct_GSTBoardPanel*)(v16->instanceVarDataCurrent))->i_sally_priority = 1000.0;
                            if (v30 == WOR_gst_Universe.univAIInstance)
                            {
                                v13 = WOR_gst_Universe.univAIInstance;
                            }
                            else
                            {
                                v15 = (tGameObjectMain*)v30;
                                if (v30)
                                {
                                    if ((unsigned int)v30 <= 2)
                                        v15 = AI_MainActor[(uint32_t)v30 - 1];
                                    v14 = v15;
                                }
                                else
                                {
                                    v14 = 0;
                                }
                                v13 = v14->extended->AIInstance;
                            }
                            ((tScriptStruct_GSTBoardPanel*)(v13->instanceVarDataCurrent))->i_sally_action = 0;
                            if (v30 == WOR_gst_Universe.univAIInstance)
                            {
                                v10 = WOR_gst_Universe.univAIInstance;
                            }
                            else
                            {
                                v12 = (tGameObjectMain*)v30;
                                if (v30)
                                {
                                    if ((unsigned int)v30 <= 2)
                                        v12 = AI_MainActor[(uint32_t)v30 - 1];
                                    v11 = v12;
                                }
                                else
                                {
                                    v11 = 0;
                                }
                                v10 = v11->extended->AIInstance;
                            }
                            ((tScriptStruct_GSTBoardPanel*)(v10->instanceVarDataCurrent))->v_sally_offset.x = 0.0;
                            if (v30 == WOR_gst_Universe.univAIInstance)
                            {
                                v7 = WOR_gst_Universe.univAIInstance;
                            }
                            else
                            {
                                v9 = (tGameObjectMain*)v30;
                                if (v30)
                                {
                                    if ((unsigned int)v30 <= 2)
                                        v9 = AI_MainActor[(uint32_t)v30 - 1];
                                    v8 = v9;
                                }
                                else
                                {
                                    v8 = 0;
                                }
                                v7 = v8->extended->AIInstance;
                            }
                            ((tScriptStruct_GSTBoardPanel*)(v7->instanceVarDataCurrent))->v_sally_offset.y = 0.0;
                            if (v30 == WOR_gst_Universe.univAIInstance)
                            {
                                v4 = WOR_gst_Universe.univAIInstance;
                            }
                            else
                            {
                                v6 = (tGameObjectMain*)v30;
                                if (v30)
                                {
                                    if ((unsigned int)v30 <= 2)
                                        v6 = AI_MainActor[(uint32_t)v30 - 1];
                                    v5 = v6;
                                }
                                else
                                {
                                    v5 = 0;
                                }
                                v4 = v5->extended->AIInstance;
                            }
                            ((tScriptStruct_GSTBoardPanel*)(v4->instanceVarDataCurrent))->v_sally_offset.z = 0.0;
                            *(uint32_t*)helper_text_hlptxt_exec_tests_interruption_GST_Board_panel_gao_i_force_interface = 1;
                        }
                    }
                    v32 = AI_EvalFunc_MSGGetByID_C(AI_gpst_CurrentGameObject, 10);
                    memcpy(&Src, v32, sizeof(Src));
                    if (Src.msgID == 10)
                        v40->f_tempo = 0.0;
                }
                else if (AI_EvalFunc_IoButtonJustPressed_C(6))
                {
                    if (AI_EvalFunc_TIMEElapsed_C(v40->f_temps, 0.25))
                    {
                        v40->f_temps = TIM_gf_MainClock - TIM_gf_LockTime;
                    }
                    else
                    {
                        Src.msgID = 30;
                        Src.gao = AI_gpst_CurrentGameObject;
                        Src.Int1 = -1;
                        Src.f1 = -1.0;
                        Src.gao2 = 0;
                        memcpy(
                            (void*)helper_text_hlptxt_exec_tests_interruption_GST_AffichageTexte_gao_message_a_traiter,
                            &Src,
                            0x34u);
                        v33 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_helper_text_hlptxt_exec_tests_interruption_helper_text_hlptxt_exec_fin_ou_abort);
                        if (v33 != 1000)
                            return v33;
                    }
                }
            }
            else
            {
            LABEL_34:
                v41 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_helper_text_hlptxt_exec_tests_interruption_helper_text_hlptxt_exec_fin_ou_abort);
                if (v41 != 1000)
                    return v41;
            }
            result = 1000;
        }
    }
    else
    {
        v37 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_helper_text_hlptxt_exec_tests_interruption_helper_text_hlptxt_exec_fin_ou_abort);
        if (v37 == 1000)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            result = v37;
        }
    }
    return result;
}

int helper_text_hlptxt_tests_wait()
{
    int* v1; // eax
    tScriptStruct_HelperText* v2; // [esp+8h] [ebp-F8h]
    tGameObjectMain* v3; // [esp+10h] [ebp-F0h]
    tGameObjectMain* v4; // [esp+18h] [ebp-E8h]
    tGameObjectMain* v5; // [esp+28h] [ebp-D8h]
    tGameObjectMain* v6; // [esp+2Ch] [ebp-D4h]
    tGameObjectMain* v7; // [esp+30h] [ebp-D0h]
    tGameObjectMain* v8; // [esp+3Ch] [ebp-C4h]
    tGameObjectMain* v9; // [esp+40h] [ebp-C0h]
    tGameObjectMain* v10; // [esp+48h] [ebp-B8h]
    tGameObjectMain* v11; // [esp+4Ch] [ebp-B4h]
    tGameObjectMain* v12; // [esp+58h] [ebp-A8h]
    tGameObjectMain* v13; // [esp+5Ch] [ebp-A4h]
    tGameObjectMain* v14; // [esp+64h] [ebp-9Ch]
    tGameObjectMain* v15; // [esp+68h] [ebp-98h]
    int v16; // [esp+70h] [ebp-90h]
    tMessage Src; // [esp+7Ch] [ebp-84h] BYREF
    BOOL v19; // [esp+B8h] [ebp-48h]
    unsigned int v22; // [esp+C4h] [ebp-3Ch]
    int v24; // [esp+D0h] [ebp-30h]
    int v25; // [esp+D4h] [ebp-2Ch]
    int v26; // [esp+D8h] [ebp-28h]
    int v27; // [esp+E0h] [ebp-20h]
    tScriptStruct_HelperText* v29; // [esp+F0h] [ebp-10h]
    unsigned int v30; // [esp+F4h] [ebp-Ch]
    int v31; // [esp+FCh] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_HelperText*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v29 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        helper_text_hlptxt_tests_wait_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_tests_wait_univVars_Perso_ID_actor,
            (void*)helper_text_hlptxt_tests_wait_univVars,
            988);
        helper_text_hlptxt_tests_wait_GST_AffichageTexte_gao = AI_ul_RealResolveOneRefObject(0x7100E7ACu);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_tests_wait_GST_AffichageTexte_gao_prio_message_a_traiter_displa,
            (void*)helper_text_hlptxt_tests_wait_GST_AffichageTexte_gao,
            140);
        fct_helper_text_hlptxt_tests_wait_helper_text_hlptxt_tests_depart = (int)AI_ul_RealResolveOneRefFunction(0x35007498u);
        helper_text_hlptxt_tests_wait_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        fct_helper_text_hlptxt_tests_wait_helper_text_hlptxt_exec_fin_ou_abort = (int)AI_ul_RealResolveOneRefFunction(0x7100E7B7u);
        helper_text_hlptxt_tests_wait_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        helper_text_hlptxt_tests_wait_INV_MatFade_gao = AI_ul_RealResolveOneRefObject(0x5E006934u);
        fct_helper_text_hlptxt_tests_wait_helper_text_hlptxt_exec_ask_display = (int)AI_ul_RealResolveOneRefFunction(0x7100E7B5u);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_tests_wait_univVars_Flag_speciaux_sauve,
            (void*)helper_text_hlptxt_tests_wait_univVars,
            4488);
        fct_helper_text_hlptxt_tests_wait_helper_text_hlptxt_exec_tests_interruption = (int)AI_ul_RealResolveOneRefFunction(0x350082FFu);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_tests_wait_GST_AffichageTexte_gao_prio_message_a_traiter_displa_message_a_traiter,
            (void*)helper_text_hlptxt_tests_wait_GST_AffichageTexte_gao,
            0);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_tests_wait_GST_AffichageTexte_gao_prio_message_a_traiter,
            (void*)helper_text_hlptxt_tests_wait_GST_AffichageTexte_gao,
            60);
    }
    if (AI_EvalFunc_TIMEElapsed_C(v29->f_temps_ecoule, v29->f_tempo))
    {
        v15 = (tGameObjectMain*)v29->des_ao_acteur_dialog[0];
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
        if (v14)
        {
            v13 = (tGameObjectMain*)v29->des_ai_perso_localisation[0];
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
            if (v12 == *(tGameObjectMain**)(helper_text_hlptxt_tests_wait_univVars_Perso_ID_actor + 4))
            {
                if (AI_EvalFunc_ARRGlobalIntFlagTest_C(930))
                    goto LABEL_26;
                v11 = (tGameObjectMain*)v29->des_ai_perso_localisation[0];
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
                if ((v10->extended->fields->designFlags & 0x10000000) != 0)
                {
                LABEL_26:
                    v29->f_temps_ecoule = TIM_gf_MainClock - TIM_gf_LockTime;
                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                    return 1000;
                }
            }
            v9 = (tGameObjectMain*)v29->des_ai_perso_localisation[0];
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
            if (v8 == *(tGameObjectMain**)(helper_text_hlptxt_tests_wait_univVars_Perso_ID_actor + 8))
            {
                if (AI_EvalFunc_ARRGlobalIntFlagTest_C(931))
                    goto LABEL_40;
                v7 = (tGameObjectMain*)v29->des_ai_perso_localisation[0];
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
                if ((v6->extended->fields->designFlags & 0x10000000) != 0)
                {
                LABEL_40:
                    v29->f_temps_ecoule = TIM_gf_MainClock - TIM_gf_LockTime;
                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                    return 1000;
                }
            }
        }
        if (v29->des_i_prio_des_textes <= *(int32_t*)helper_text_hlptxt_tests_wait_GST_AffichageTexte_gao_prio_message_a_traiter_displa)
        {
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_helper_text_hlptxt_tests_wait_helper_text_hlptxt_tests_depart);
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        if (v29->des_i_choix_aleatoire)
        {
            if (v29->des_i_blocage_joy)
                AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)helper_text_hlptxt_tests_wait_GST_Global_gao, 0x8000, 0);
            if (v29->i_display_once)
            {
                v24 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_helper_text_hlptxt_tests_wait_helper_text_hlptxt_exec_fin_ou_abort);
                if (v24 != 1000)
                    return v24;
            }
            v29->i_index_ligne = AI_EvalFunc_MATHRandInt_C(0, 5);
        }
        else
        {
            if (v29->des_i_blocage_joy)
                AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)helper_text_hlptxt_tests_wait_GST_Global_gao, 0x8000, 0);
            if (++v29->i_index_ligne >= 5
                || (v1 = (int*)AI_EvalVarArray_C(10, (char*)&v29->__des_at_lignes_texte_Dimension1, v29->i_index_ligne),
                    v19 = AI_EvalFunc_TEXTIsEmpty_C(v1)))
            {
                v27 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_helper_text_hlptxt_tests_wait_helper_text_hlptxt_exec_fin_ou_abort);
                if (v27 != 1000)
                    return v27;
            }
        }
        if (v29->des_i_cine_temporaire && !v29->i_display_once)
        {
            v30 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x8000000, 0);
            v22 = AI_EvalFunc_OBJ_FlagsControlSet_C(
                (tGameObjectMain*)helper_text_hlptxt_tests_wait_GST_AffichageTexte_gao,
                0x8000000,
                0);
            v29->i_sfx_temp = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 80);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v29->i_sfx_temp, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v29->i_sfx_temp, 2, 1);
            v5 = (tGameObjectMain*)helper_text_hlptxt_tests_wait_INV_MatFade_gao;
            if (helper_text_hlptxt_tests_wait_INV_MatFade_gao)
            {
                if ((unsigned int)helper_text_hlptxt_tests_wait_INV_MatFade_gao <= 2)
                    v5 = AI_MainActor[helper_text_hlptxt_tests_wait_INV_MatFade_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v29->i_sfx_temp, v5, 9);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v29->i_sfx_temp, 0, 9);
            }
            GFXSetCurGlobal(v29->i_sfx_temp);
            AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0xFF000000);
            AI_EvalFunc_GFXSeti_C_CURGLOB(11101, 0xFF000000);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 10000000.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 10000000.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 10000000.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11005, 0.2f);
            v29->f_temps = TIM_gf_MainClock - TIM_gf_LockTime;
            v31 = AI_EvalFunc_WORCurIndexGet_C();
            AI_EvalFunc_WORPause_C(v31, 32);
        }
        v29->i_display_once = 1;
        v25 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_helper_text_hlptxt_tests_wait_helper_text_hlptxt_exec_ask_display);
        if (v25 != 1000)
            return v25;
        if (v29->des_i_Flag_debut)
        {
            if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
            {
                if (v29->des_i_SPFlag_associe_vrai > 0)
                    AI_EvalFunc_ARRBitSet_C(
                        (int*)(helper_text_hlptxt_tests_wait_univVars_Flag_speciaux_sauve + 4),
                        v29->des_i_SPFlag_associe_vrai);
                if (v29->des_i_SPFlag_associe_faux > 0)
                    AI_EvalFunc_ARRBitClear_C(
                        (int*)(helper_text_hlptxt_tests_wait_univVars_Flag_speciaux_sauve + 4),
                        v29->des_i_SPFlag_associe_faux);
            }
        }
        v29->f_temps_ecoule = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    else if (v29->i_display_once)
    {
        v16 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_helper_text_hlptxt_tests_wait_helper_text_hlptxt_exec_tests_interruption);
        if (v16 != 1000)
            return v16;
    }
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
        goto LABEL_97;
    if (!v29->i_display_once)
        goto LABEL_97;
    v29->f_tempo = 0.0;
    v29->i_index_ligne = 5;
    Src.msgID = 30;
    Src.gao = AI_gpst_CurrentGameObject;
    Src.Int1 = -1;
    Src.f1 = -1.0;
    Src.gao2 = 0;
    memcpy(
        (void*)helper_text_hlptxt_tests_wait_GST_AffichageTexte_gao_prio_message_a_traiter_displa_message_a_traiter,
        &Src,
        0x34u);
    *(uint32_t*)helper_text_hlptxt_tests_wait_GST_AffichageTexte_gao_prio_message_a_traiter = -1;
    v26 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_helper_text_hlptxt_tests_wait_helper_text_hlptxt_exec_fin_ou_abort);
    if (v26 != 1000)
        return v26;
LABEL_97:
    if (v29->des_i_inventaire_interdit)
    {
        v4 = (tGameObjectMain*)helper_text_hlptxt_tests_wait_GST_Board_panel_gao;
        if (helper_text_hlptxt_tests_wait_GST_Board_panel_gao)
        {
            if ((unsigned int)helper_text_hlptxt_tests_wait_GST_Board_panel_gao <= 2)
                v4 = AI_MainActor[helper_text_hlptxt_tests_wait_GST_Board_panel_gao - 1];
            AI_EvalFunc_OBJ_CustomBitsSet_C(v4, 0x100, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_CustomBitsSet_C(0, 0x100, 0);
        }
    }
    if (v29->des_i_look_interdit)
    {
        v3 = (tGameObjectMain*)helper_text_hlptxt_tests_wait_GST_Board_panel_gao;
        if (helper_text_hlptxt_tests_wait_GST_Board_panel_gao)
        {
            if ((unsigned int)helper_text_hlptxt_tests_wait_GST_Board_panel_gao <= 2)
                v3 = AI_MainActor[helper_text_hlptxt_tests_wait_GST_Board_panel_gao - 1];
            AI_EvalFunc_OBJ_CustomBitsSet_C(v3, 0x200, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_CustomBitsSet_C(0, 0x200, 0);
        }
    }
    if (v29->des_i_blocage_joy)
        AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)helper_text_hlptxt_tests_wait_GST_Global_gao, 0x8000, 0);
    return 1000;
}

int helper_text_hlptxt_tests_depart()
{
    int result; // eax
    int v1; // eax
    BOOL v2; // eax
    BOOL v3; // eax
    BOOL v4; // eax
    int v5; // eax
    BOOL v6; // eax
    BOOL v7; // eax
    int v8; // eax
    int v9; // eax
    BOOL v10; // eax
    BOOL v11; // eax
    int v12; // eax
    int v13; // eax
    int v14; // eax
    tMessage* v15; // eax
    double v16; // st7
    tScriptStruct_HelperText* v17; // [esp+0h] [ebp-348h]
    tGameObjectMain* v18; // [esp+Ch] [ebp-33Ch]
    tGameObjectMain* v19; // [esp+10h] [ebp-338h]
    tGameObjectMain* v20; // [esp+14h] [ebp-334h]
    tGameObjectMain* v21; // [esp+18h] [ebp-330h]
    tGameObjectMain* v22; // [esp+1Ch] [ebp-32Ch]
    tGameObjectMain* v23; // [esp+20h] [ebp-328h]
    tGameObjectMain* v24; // [esp+24h] [ebp-324h]
    tGameObjectMain* v25; // [esp+28h] [ebp-320h]
    tGameObjectMain* v26; // [esp+2Ch] [ebp-31Ch]
    tGameObjectMain* v27; // [esp+30h] [ebp-318h]
    tGameObjectMain* v28; // [esp+34h] [ebp-314h]
    tGameObjectMain* v29; // [esp+38h] [ebp-310h]
    tGameObjectMain* v30; // [esp+3Ch] [ebp-30Ch]
    tGameObjectMain* v31; // [esp+40h] [ebp-308h]
    tGameObjectMain* v32; // [esp+44h] [ebp-304h]
    tGameObjectMain* v33; // [esp+48h] [ebp-300h]
    tGameObjectMain* v34; // [esp+4Ch] [ebp-2FCh]
    tGameObjectMain* v35; // [esp+50h] [ebp-2F8h]
    tGameObjectMain* v36; // [esp+54h] [ebp-2F4h]
    tGameObjectMain* v37; // [esp+58h] [ebp-2F0h]
    tGameObjectMain* v38; // [esp+5Ch] [ebp-2ECh]
    tGameObjectMain* v39; // [esp+60h] [ebp-2E8h]
    tGameObjectMain* v40; // [esp+64h] [ebp-2E4h]
    tGameObjectMain* v41; // [esp+68h] [ebp-2E0h]
    tGameObjectMain* v42; // [esp+6Ch] [ebp-2DCh]
    tGameObjectMain* v43; // [esp+70h] [ebp-2D8h]
    tGameObjectMain* v44; // [esp+74h] [ebp-2D4h]
    tGameObjectMain* v45; // [esp+78h] [ebp-2D0h]
    tGameObjectMain* v46; // [esp+84h] [ebp-2C4h]
    tGameObjectMain* v47; // [esp+88h] [ebp-2C0h]
    tGameObjectMain* v48; // [esp+A0h] [ebp-2A8h]
    tGameObjectMain* v49; // [esp+A8h] [ebp-2A0h]
    tGameObjectMain* v50; // [esp+ACh] [ebp-29Ch]
    tGameObjectMain* v51; // [esp+B0h] [ebp-298h]
    tGameObjectMain* v52; // [esp+B4h] [ebp-294h]
    tGameObjectMain* v53; // [esp+B8h] [ebp-290h]
    tGameObjectMain* v54; // [esp+C8h] [ebp-280h]
    tGameObjectMain* v55; // [esp+D0h] [ebp-278h]
    tGameObjectMain* v56; // [esp+DCh] [ebp-26Ch]
    tGameObjectMain* v57; // [esp+E0h] [ebp-268h]
    tGameObjectMain* v58; // [esp+F0h] [ebp-258h]
    tGameObjectMain* v59; // [esp+100h] [ebp-248h]
    tGameObjectMain* v60; // [esp+110h] [ebp-238h]
    tGameObjectMain* v61; // [esp+114h] [ebp-234h]
    tGameObjectMain* v62; // [esp+118h] [ebp-230h]
    tGameObjectMain* v63; // [esp+120h] [ebp-228h]
    tGameObjectMain* v64; // [esp+124h] [ebp-224h]
    tGameObjectMain* v65; // [esp+128h] [ebp-220h]
    tGameObjectMain* v66; // [esp+130h] [ebp-218h]
    tGameObjectMain* v67; // [esp+134h] [ebp-214h]
    tGameObjectMain* v68; // [esp+138h] [ebp-210h]
    tGameObjectMain* v69; // [esp+140h] [ebp-208h]
    tGameObjectMain* v70; // [esp+144h] [ebp-204h]
    tGameObjectMain* v71; // [esp+148h] [ebp-200h]
    tGameObjectMain* v72; // [esp+150h] [ebp-1F8h]
    tGameObjectMain* v73; // [esp+154h] [ebp-1F4h]
    tGameObjectMain* v74; // [esp+158h] [ebp-1F0h]
    tGameObjectMain* v75; // [esp+178h] [ebp-1D0h]
    tGameObjectMain* v76; // [esp+17Ch] [ebp-1CCh]
    tGameObjectMain* v77; // [esp+180h] [ebp-1C8h]
    tGameObjectMain* v78; // [esp+198h] [ebp-1B0h]
    tGameObjectMain* v79; // [esp+19Ch] [ebp-1ACh]
    tGameObjectMain* v80; // [esp+1A0h] [ebp-1A8h]
    tGameObjectMain* v81; // [esp+1A8h] [ebp-1A0h]
    tGameObjectMain* v82; // [esp+1B0h] [ebp-198h]
    tGameObjectMain* v83; // [esp+1B8h] [ebp-190h]
    tGameObjectMain* v84; // [esp+1BCh] [ebp-18Ch]
    tGameObjectMain* v85; // [esp+1C0h] [ebp-188h]
    tGameObjectMain* v86; // [esp+1C8h] [ebp-180h]
    tGameObjectMain* v87; // [esp+1D0h] [ebp-178h]
    tGameObjectMain* v88; // [esp+1D4h] [ebp-174h]
    Vec3 v89; // [esp+204h] [ebp-144h] BYREF
    unsigned int v90; // [esp+214h] [ebp-134h]
    unsigned int v91; // [esp+218h] [ebp-130h]
    int v92; // [esp+21Ch] [ebp-12Ch]
    Vec3 v93; // [esp+220h] [ebp-128h] BYREF
    unsigned int v94; // [esp+22Ch] [ebp-11Ch]
    unsigned int v95; // [esp+230h] [ebp-118h]
    int v97; // [esp+238h] [ebp-110h]
    unsigned int v98; // [esp+23Ch] [ebp-10Ch]
    BOOL v99; // [esp+244h] [ebp-104h]
    BOOL v100; // [esp+248h] [ebp-100h]
    unsigned int v101; // [esp+24Ch] [ebp-FCh]
    tMessage v102; // [esp+254h] [ebp-F4h] BYREF
    Vec3 v103; // [esp+288h] [ebp-C0h] BYREF
    float v104; // [esp+294h] [ebp-B4h]
    BOOL v105; // [esp+298h] [ebp-B0h]
    int v106; // [esp+2A4h] [ebp-A4h]
    unsigned int v107; // [esp+2A8h] [ebp-A0h]
    Vec3 a3; // [esp+2B0h] [ebp-98h] BYREF
    BOOL v109; // [esp+2BCh] [ebp-8Ch]
    int v110; // [esp+2C0h] [ebp-88h]
    int v111; // [esp+2C4h] [ebp-84h]
    int i; // [esp+2C8h] [ebp-80h]
    int v113; // [esp+2CCh] [ebp-7Ch]
    int v114; // [esp+2D0h] [ebp-78h]
    BOOL v115; // [esp+2D4h] [ebp-74h]
    BOOL v116; // [esp+2E0h] [ebp-68h]
    Vec3 v117; // [esp+2E4h] [ebp-64h] BYREF
    int v118; // [esp+2F0h] [ebp-58h]
    int v119; // [esp+2F4h] [ebp-54h]
    int v120; // [esp+2F8h] [ebp-50h]
    void* Src; // [esp+2FCh] [ebp-4Ch]
    int v122; // [esp+300h] [ebp-48h]
    tScriptStruct_HelperText* v123; // [esp+310h] [ebp-38h]
    float v124; // [esp+314h] [ebp-34h]
    float v125; // [esp+318h] [ebp-30h]
    BOOL v126; // [esp+320h] [ebp-28h]
    unsigned int v127; // [esp+328h] [ebp-20h]
    int v128; // [esp+32Ch] [ebp-1Ch]
    unsigned int v129; // [esp+338h] [ebp-10h]
    BOOL v130; // [esp+33Ch] [ebp-Ch]
    int v131; // [esp+340h] [ebp-8h]
    int v132; // [esp+344h] [ebp-4h]

    v89.x = 1.0;
    v89.y = 0.0;
    v89.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v17 = (tScriptStruct_HelperText*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v17 = 0;
    v123 = v17;
    v124 = 0.0;
    v110 = 0;
    v111 = 0;
    v125 = 0.0;
    i = 0;
    v120 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        helper_text_hlptxt_tests_depart_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve,
            (void*)helper_text_hlptxt_tests_depart_univVars,
            4488);
        fctvar_helper_text_hlptxt_tests_depart_UNKNOWN1 = AI_ul_RealResolveOneRef(42, 0x5E005FB8u);
        fctvar_helper_text_hlptxt_tests_depart_UNKNOWN2 = AI_ul_RealResolveOneRef(42, 0x720004C0u);
        fctr_helper_text_hlptxt_tests_depart_helper_text_hlptxt_tests_wait = (int)AI_ul_RealResolveOneRefFunction(0x7100E7B3u);
    }
    v110 = 0;
    if ((v123->i_tests_a_reussir & 0x80) != 0)
    {
        if (v123->des_i_specialFlag_test1 <= 0)
        {
            v111 = 0;
        }
        else if (v123->des_i_specialFlag_test1 < 10000)
        {
            v111 = AI_EvalFunc_ARRBitTest_C(
                (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                v123->des_i_specialFlag_test1);
        }
        else
        {
            v99 = AI_EvalFunc_ARRBitTest_C(
                (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                v123->des_i_specialFlag_test1 - 10000);
            v111 = 1 - v99;
        }
        if (v111 == 1)
            ++v110;
        if (!v110 && v123->i_was_ready)
        {
            v123->des_f_tempo = v123->des_f_tempo - TIM_gf_dt;
            if (v123->des_f_tempo > 0.0)
            {
                v110 = 1;
            }
            else
            {
                if (v123->des_i_SPFlag_associe_vrai > 0)
                    AI_EvalFunc_ARRBitSet_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_vrai);
                if (v123->des_i_SPFlag_associe_faux > 0)
                    AI_EvalFunc_ARRBitClear_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_faux);
                v90 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 6291456, 0);
            }
        }
        if (!v110)
        {
            v123->i_ready = 1;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v110 = 0;
    }
    if ((v123->i_tests_a_reussir & 0x100) != 0)
    {
        if (v123->des_i_specialFlag_test2 <= 0)
        {
            v111 = 0;
        }
        else if (v123->des_i_specialFlag_test2 < 10000)
        {
            v111 = AI_EvalFunc_ARRBitTest_C(
                (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                v123->des_i_specialFlag_test2);
        }
        else
        {
            v111 = 1
                - AI_EvalFunc_ARRBitTest_C(
                    (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                    v123->des_i_specialFlag_test2 - 10000);
        }
        if (!v111)
            ++v110;
        if (!v110 && v123->i_was_ready)
        {
            v123->des_f_tempo = v123->des_f_tempo - TIM_gf_dt;
            if (v123->des_f_tempo > 0.0)
            {
                v110 = 1;
            }
            else
            {
                if (v123->des_i_SPFlag_associe_vrai > 0)
                    AI_EvalFunc_ARRBitSet_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_vrai);
                if (v123->des_i_SPFlag_associe_faux > 0)
                    AI_EvalFunc_ARRBitClear_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_faux);
                v94 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
            }
        }
        if (!v110)
        {
            v123->i_ready = 1;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v110 = 0;
    }
    if ((v123->i_tests_a_reussir & 1) != 0)
    {
        v88 = (tGameObjectMain*)v123->des_o_actor_detec1;
        if (v88)
        {
            if ((unsigned int)v88 <= 2)
                v88 = AI_MainActor[(uint32_t)v88 - 1];
            v87 = v88;
        }
        else
        {
            v87 = 0;
        }
        if (!v87)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        if (v123->des_i_actor_type1)
        {
            i = 0;
            v120 = 0;
            v86 = (tGameObjectMain*)v123->des_o_actor_zone1;
            if (v86)
            {
                if ((unsigned int)v86 <= 2)
                    v86 = AI_MainActor[(uint32_t)v86 - 1];
                v1 = AI_EvalFunc_COLBVGetAllOverlaps_C(v86, (tGameObjectMain**)v123->ao_objets_collisions, 0x20000, 0, 0);
            }
            else
            {
                v1 = AI_EvalFunc_COLBVGetAllOverlaps_C(0, (tGameObjectMain**)v123->ao_objets_collisions, 0x20000, 0, 0);
            }
            v120 = v1;
            if (v1)
            {
                while (i < v120)
                {
                    v85 = (tGameObjectMain*)v123->ao_objets_collisions[i];
                    if (v85)
                    {
                        if ((unsigned int)v85 <= 2)
                            v85 = AI_MainActor[(uint32_t)v85 - 1];
                        v84 = v85;
                    }
                    else
                    {
                        v84 = 0;
                    }
                    if (v84)
                    {
                        if (v123->des_i_actor_type1 != 1)
                            goto LABEL_75;
                        v83 = (tGameObjectMain*)v123->ao_objets_collisions[i];
                        if (v83)
                        {
                            if ((unsigned int)v83 <= 2)
                                v83 = AI_MainActor[(uint32_t)v83 - 1];
                            v2 = AI_EvalFunc_AIIsModel_C(v83, (tAIModel*)fctvar_helper_text_hlptxt_tests_depart_UNKNOWN1);
                        }
                        else
                        {
                            v2 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_helper_text_hlptxt_tests_depart_UNKNOWN1);
                        }
                        v126 = v2;
                        if (v2)
                        {
                            ++v110;
                        }
                        else
                        {
                        LABEL_75:
                            if (v123->des_i_actor_type1 == 2)
                            {
                                v82 = (tGameObjectMain*)v123->ao_objets_collisions[i];
                                if (v82)
                                {
                                    if ((unsigned int)v82 <= 2)
                                        v82 = AI_MainActor[(uint32_t)v82 - 1];
                                    v3 = AI_EvalFunc_AIIsModel_C(v82, (tAIModel*)fctvar_helper_text_hlptxt_tests_depart_UNKNOWN2);
                                }
                                else
                                {
                                    v3 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_helper_text_hlptxt_tests_depart_UNKNOWN2);
                                }
                                v100 = v3;
                                if (v3)
                                    ++v110;
                            }
                        }
                    }
                    ++i;
                }
            }
        }
        else
        {
            v81 = (tGameObjectMain*)v123->des_o_actor_zone1;
            if (v81)
            {
                if ((unsigned int)v81 <= 2)
                    v81 = AI_MainActor[(uint32_t)v81 - 1];
                v4 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v81, 0x400);
            }
            else
            {
                v4 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x400);
            }
            v115 = v4;
            if (v4)
            {
                v80 = (tGameObjectMain*)v123->des_o_actor_detec1;
                if (v80)
                {
                    if ((unsigned int)v80 <= 2)
                        v80 = AI_MainActor[(uint32_t)v80 - 1];
                    v79 = v80;
                }
                else
                {
                    v79 = 0;
                }
                v78 = (tGameObjectMain*)v123->des_o_actor_zone1;
                if (v78)
                {
                    if ((unsigned int)v78 <= 2)
                        v78 = AI_MainActor[(uint32_t)v78 - 1];
                    v5 = AI_EvalFunc_COLZDEPivotCollide_C(v78, v79, 0);
                }
                else
                {
                    v5 = AI_EvalFunc_COLZDEPivotCollide_C(0, v79, 0);
                }
                v92 = v5;
                if (v5)
                    ++v110;
            }
            else
            {
                v77 = (tGameObjectMain*)v123->des_o_actor_zone1;
                if (v77)
                {
                    if ((unsigned int)v77 <= 2)
                        v77 = AI_MainActor[(uint32_t)v77 - 1];
                    v76 = v77;
                }
                else
                {
                    v76 = 0;
                }
                v75 = (tGameObjectMain*)v123->des_o_actor_detec1;
                if (v75)
                {
                    if ((unsigned int)v75 <= 2)
                        v75 = AI_MainActor[(uint32_t)v75 - 1];
                    v6 = AI_EvalFunc_COLBVObjectObject_C(v75, v76);
                }
                else
                {
                    v6 = AI_EvalFunc_COLBVObjectObject_C(0, v76);
                }
                v109 = v6;
                if (v6)
                    ++v110;
            }
        }
        if (!v110 && v123->i_was_ready)
        {
            v123->des_f_tempo = v123->des_f_tempo - TIM_gf_dt;
            if (v123->des_f_tempo > 0.0)
            {
                v110 = 1;
            }
            else
            {
                if (v123->des_i_SPFlag_associe_vrai > 0)
                    AI_EvalFunc_ARRBitSet_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_vrai);
                if (v123->des_i_SPFlag_associe_faux > 0)
                    AI_EvalFunc_ARRBitClear_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_faux);
                AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
            }
        }
        if (!v110)
        {
            v123->i_ready = 1;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v110 = 0;
    }
    if ((v123->i_tests_a_reussir & 2) != 0)
    {
        v74 = (tGameObjectMain*)v123->des_o_actor_detec2;
        if (v74)
        {
            if ((unsigned int)v74 <= 2)
                v74 = AI_MainActor[(uint32_t)v74 - 1];
            v73 = v74;
        }
        else
        {
            v73 = 0;
        }
        if (!v73)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v72 = (tGameObjectMain*)v123->des_o_actor_zone2;
        if (v72)
        {
            if ((unsigned int)v72 <= 2)
                v72 = AI_MainActor[(uint32_t)v72 - 1];
            v7 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v72, 0x400);
        }
        else
        {
            v7 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x400);
        }
        v105 = v7;
        if (v7)
        {
            if (v123->des_i_collision_type2)
            {
                v71 = (tGameObjectMain*)v123->des_o_actor_detec2;
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
                v69 = (tGameObjectMain*)v123->des_o_actor_zone2;
                if (v69)
                {
                    if ((unsigned int)v69 <= 2)
                        v69 = AI_MainActor[(uint32_t)v69 - 1];
                    v8 = AI_EvalFunc_COLZDEPivotCollide_C(v69, v70, 0);
                }
                else
                {
                    v8 = AI_EvalFunc_COLZDEPivotCollide_C(0, v70, 0);
                }
                v119 = v8;
                if (v8)
                    ++v110;
            }
            else
            {
                v68 = (tGameObjectMain*)v123->des_o_actor_detec2;
                if (v68)
                {
                    if ((unsigned int)v68 <= 2)
                        v68 = AI_MainActor[(uint32_t)v68 - 1];
                    v67 = v68;
                }
                else
                {
                    v67 = 0;
                }
                v66 = (tGameObjectMain*)v123->des_o_actor_zone2;
                if (v66)
                {
                    if ((unsigned int)v66 <= 2)
                        v66 = AI_MainActor[(uint32_t)v66 - 1];
                    v9 = AI_EvalFunc_COLZDEPivotCollide_C(v66, v67, 0);
                }
                else
                {
                    v9 = AI_EvalFunc_COLZDEPivotCollide_C(0, v67, 0);
                }
                v114 = v9;
                if (!v9)
                    ++v110;
            }
        }
        else if (v123->des_i_collision_type2)
        {
            v65 = (tGameObjectMain*)v123->des_o_actor_zone2;
            if (v65)
            {
                if ((unsigned int)v65 <= 2)
                    v65 = AI_MainActor[(uint32_t)v65 - 1];
                v64 = v65;
            }
            else
            {
                v64 = 0;
            }
            v63 = (tGameObjectMain*)v123->des_o_actor_detec2;
            if (v63)
            {
                if ((unsigned int)v63 <= 2)
                    v63 = AI_MainActor[(uint32_t)v63 - 1];
                v10 = AI_EvalFunc_COLBVObjectObject_C(v63, v64);
            }
            else
            {
                v10 = AI_EvalFunc_COLBVObjectObject_C(0, v64);
            }
            v130 = v10;
            if (v10)
                ++v110;
        }
        else
        {
            v62 = (tGameObjectMain*)v123->des_o_actor_zone2;
            if (v62)
            {
                if ((unsigned int)v62 <= 2)
                    v62 = AI_MainActor[(uint32_t)v62 - 1];
                v61 = v62;
            }
            else
            {
                v61 = 0;
            }
            v60 = (tGameObjectMain*)v123->des_o_actor_detec2;
            if (v60)
            {
                if ((unsigned int)v60 <= 2)
                    v60 = AI_MainActor[(uint32_t)v60 - 1];
                v11 = AI_EvalFunc_COLBVObjectObject_C(v60, v61);
            }
            else
            {
                v11 = AI_EvalFunc_COLBVObjectObject_C(0, v61);
            }
            v116 = v11;
            if (!v11)
                ++v110;
        }
        if (!v110 && v123->i_was_ready)
        {
            v123->des_f_tempo = v123->des_f_tempo - TIM_gf_dt;
            if (v123->des_f_tempo > 0.0)
            {
                v110 = 1;
            }
            else
            {
                if (v123->des_i_SPFlag_associe_vrai > 0)
                    AI_EvalFunc_ARRBitSet_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_vrai);
                if (v123->des_i_SPFlag_associe_faux > 0)
                    AI_EvalFunc_ARRBitClear_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_faux);
                v101 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
            }
        }
        if (!v110)
        {
            v123->i_ready = 1;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v110 = 0;
    }
    if ((v123->i_tests_a_reussir & 8) != 0)
    {
        v59 = (tGameObjectMain*)v123->des_o_actor_capa1;
        if (v59)
        {
            if ((unsigned int)v59 <= 2)
                v59 = AI_MainActor[(uint32_t)v59 - 1];
            v12 = AI_EvalFunc_OBJ_CapaGet_C(v59);
        }
        else
        {
            v12 = AI_EvalFunc_OBJ_CapaGet_C(0);
        }
        v118 = v12;
        v111 = v123->des_i_capa_test1 & v12;
        if (v111 == v123->des_i_capa_etat1 * v123->des_i_capa_test1)
            ++v110;
        if (!v110 && v123->i_was_ready)
        {
            v123->des_f_tempo = v123->des_f_tempo - TIM_gf_dt;
            if (v123->des_f_tempo > 0.0)
            {
                v110 = 1;
            }
            else
            {
                if (v123->des_i_SPFlag_associe_vrai > 0)
                    AI_EvalFunc_ARRBitSet_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_vrai);
                if (v123->des_i_SPFlag_associe_faux > 0)
                    AI_EvalFunc_ARRBitClear_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_faux);
                v127 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
            }
        }
        if (!v110)
        {
            v123->i_ready = 1;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v110 = 0;
    }
    if ((v123->i_tests_a_reussir & 0x10) != 0)
    {
        v58 = (tGameObjectMain*)v123->des_o_actor_cbit1;
        if (v58)
        {
            if ((unsigned int)v58 <= 2)
                v58 = AI_MainActor[(uint32_t)v58 - 1];
            v13 = AI_EvalFunc_OBJ_CustomBitsGet_C(v58);
        }
        else
        {
            v13 = AI_EvalFunc_OBJ_CustomBitsGet_C(0);
        }
        v106 = v13;
        v111 = v123->des_i_cbit_test1 & v13;
        if (v111 == v123->des_i_cbit_etat1 * v123->des_i_cbit_test1)
            ++v110;
        if (!v110 && v123->i_was_ready)
        {
            v123->des_f_tempo = v123->des_f_tempo - TIM_gf_dt;
            if (v123->des_f_tempo > 0.0)
            {
                v110 = 1;
            }
            else
            {
                if (v123->des_i_SPFlag_associe_vrai > 0)
                    AI_EvalFunc_ARRBitSet_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_vrai);
                if (v123->des_i_SPFlag_associe_faux > 0)
                    AI_EvalFunc_ARRBitClear_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_faux);
                v107 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
            }
        }
        if (!v110)
        {
            v123->i_ready = 1;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v110 = 0;
    }
    if ((v123->i_tests_a_reussir & 4) != 0)
    {
        v57 = (tGameObjectMain*)v123->des_o_actor_status1;
        if (v57)
        {
            if ((unsigned int)v57 <= 2)
                v57 = AI_MainActor[(uint32_t)v57 - 1];
            v56 = v57;
        }
        else
        {
            v56 = 0;
        }
        v113 = (unsigned __int16)v56->flags2;
        if ((v113 & 1) == v123->des_i_activity_test1)
            ++v110;
        if (!v110 && v123->i_was_ready)
        {
            v123->des_f_tempo = v123->des_f_tempo - TIM_gf_dt;
            if (v123->des_f_tempo > 0.0)
            {
                v110 = 1;
            }
            else
            {
                if (v123->des_i_SPFlag_associe_vrai > 0)
                    AI_EvalFunc_ARRBitSet_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_vrai);
                if (v123->des_i_SPFlag_associe_faux > 0)
                    AI_EvalFunc_ARRBitClear_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_faux);
                v98 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
            }
        }
        if (!v110)
        {
            v123->i_ready = 1;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v110 = 0;
    }
    if ((v123->i_tests_a_reussir & 0x20) != 0)
    {
        v55 = (tGameObjectMain*)v123->des_o_acteur_msg;
        if (v55)
        {
            if ((unsigned int)v55 <= 2)
                v55 = AI_MainActor[(uint32_t)v55 - 1];
            v14 = AI_EvalFunc_MSGGetCount_C(v55);
        }
        else
        {
            v14 = AI_EvalFunc_MSGGetCount_C(0);
        }
        v132 = v14;
        if (v14)
        {
            for (i = 0; i < 4; ++i)
            {
                v54 = (tGameObjectMain*)v123->des_o_acteur_msg;
                if (v54)
                {
                    if ((unsigned int)v54 <= 2)
                        v54 = AI_MainActor[(uint32_t)v54 - 1];
                    v15 = AI_EvalFunc_MSGRead_C(v54, i);
                }
                else
                {
                    v15 = AI_EvalFunc_MSGRead_C(0, i);
                }
                Src = v15;
                memcpy(&v102, v15, sizeof(v102));
                if (v102.msgID == v123->des_i_msg_ID && (!v123->des_i_msg_INT1 || (v123->des_i_msg_INT1 & v102.Int1) != 0))
                {
                    ++v110;
                    break;
                }
            }
        }
        if (!v110 && v123->i_was_ready)
        {
            v123->des_f_tempo = v123->des_f_tempo - TIM_gf_dt;
            if (v123->des_f_tempo > 0.0)
            {
                v110 = 1;
            }
            else
            {
                if (v123->des_i_SPFlag_associe_vrai > 0)
                    AI_EvalFunc_ARRBitSet_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_vrai);
                if (v123->des_i_SPFlag_associe_faux > 0)
                    AI_EvalFunc_ARRBitClear_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_faux);
                v95 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
            }
        }
        if (!v110)
        {
            v123->i_ready = 1;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v110 = 0;
    }
    if ((v123->i_tests_a_reussir & 0x40) != 0)
    {
        if (v123->des_f_aire_visibilite == 0.0)
        {
            v47 = (tGameObjectMain*)v123->des_o_actor_visible1;
            if (v47)
            {
                if ((unsigned int)v47 <= 2)
                    v47 = AI_MainActor[(uint32_t)v47 - 1];
                v46 = v47;
            }
            else
            {
                v46 = 0;
            }
            v131 = (unsigned __int16)v46->flags2;
            if ((v131 & 4) != 0)
                ++v110;
        }
        else
        {
            v53 = (tGameObjectMain*)v123->des_o_actor_visible1;
            if (v53)
            {
                if ((unsigned int)v53 <= 2)
                    v53 = AI_MainActor[(uint32_t)v53 - 1];
                v52 = v53;
            }
            else
            {
                v52 = 0;
            }
            v122 = (unsigned __int16)v52->flags2;
            if ((v122 & 2) != 0)
            {
                v51 = (tGameObjectMain*)v123->des_o_actor_visible1;
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
                v128 = (unsigned __int16)v50->flags2;
                if ((v128 & 4) == 0)
                {
                    if (v123->des_f_aire_visibilite == 1.0)
                    {
                        ++v110;
                    }
                    else
                    {
                        v124 = v123->des_f_aire_visibilite * 0.5f;
                        v125 = v124 + 0.5f;
                        v124 = 0.5f - v124;
                        v49 = (tGameObjectMain*)v123->des_o_actor_visible1;
                        if (v49)
                        {
                            if ((unsigned int)v49 <= 2)
                                v49 = AI_MainActor[(uint32_t)v49 - 1];
                            AI_EvalFunc_OBJGetPos_C(v49, &a3);
                        }
                        else
                        {
                            AI_EvalFunc_OBJGetPos_C(0, &a3);
                        }
                        v48 = (tGameObjectMain*)v123->des_o_actor_visible1;
                        if (v48)
                        {
                            if ((unsigned int)v48 <= 2)
                                v48 = AI_MainActor[(uint32_t)v48 - 1];
                            v16 = AI_EvalFunc_BVSizeGet_C(v48);
                        }
                        else
                        {
                            v16 = AI_EvalFunc_BVSizeGet_C(0);
                        }
                        v104 = (float)v16;
                        v117.x = 0.0;
                        v117.y = 0.0;
                        v117.z = v104;
                        MATH_VecAddVec(&v93, &a3, &v117);
                        v89 = v93;
                        AI_EvalFunc_VIEW3dWorldTo2d_C(0, &v89, &v103);
                        v89 = v103;
                        if (v103.x >= (double)v124 && v89.x <= (double)v125 && v89.y >= (double)v124 && v89.y <= (double)v125)
                            ++v110;
                    }
                }
            }
        }
        if (!v110 && v123->i_was_ready)
        {
            v123->des_f_tempo = v123->des_f_tempo - TIM_gf_dt;
            if (v123->des_f_tempo > 0.0)
            {
                v110 = 1;
            }
            else
            {
                if (v123->des_i_SPFlag_associe_vrai > 0)
                    AI_EvalFunc_ARRBitSet_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_vrai);
                if (v123->des_i_SPFlag_associe_faux > 0)
                    AI_EvalFunc_ARRBitClear_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_faux);
                v91 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
            }
        }
        if (!v110)
        {
            v123->i_ready = 1;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v110 = 0;
    }
    if ((v123->i_tests_a_reussir & 0x200) != 0)
    {
        switch (v123->des_i_desstruct_num)
        {
        case 1:
            if (v123->des_i_desstruct_flag <= 0)
            {
                v41 = (tGameObjectMain*)v123->des_o_actor_desstruct;
                if (v41)
                {
                    if ((unsigned int)v41 <= 2)
                        v41 = AI_MainActor[(uint32_t)v41 - 1];
                    v40 = v41;
                }
                else
                {
                    v40 = 0;
                }
                if ((-v123->des_i_desstruct_flag & v40->extended->fields->designFlags) == 0)
                {
                    v39 = (tGameObjectMain*)v123->des_o_actor_desstruct;
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
                    if ((unsigned __int8)v38->extended->fields->designInt1 == v123->des_i_desstruct_int)
                        ++v110;
                }
            }
            else
            {
                v45 = (tGameObjectMain*)v123->des_o_actor_desstruct;
                if (v45)
                {
                    if ((unsigned int)v45 <= 2)
                        v45 = AI_MainActor[(uint32_t)v45 - 1];
                    v44 = v45;
                }
                else
                {
                    v44 = 0;
                }
                if ((v123->des_i_desstruct_flag & v44->extended->fields->designFlags) != 0)
                {
                    v43 = (tGameObjectMain*)v123->des_o_actor_desstruct;
                    if (v43)
                    {
                        if ((unsigned int)v43 <= 2)
                            v43 = AI_MainActor[(uint32_t)v43 - 1];
                        v42 = v43;
                    }
                    else
                    {
                        v42 = 0;
                    }
                    if ((unsigned __int8)v42->extended->fields->designInt1 == v123->des_i_desstruct_int)
                        ++v110;
                }
            }
            break;
        case 2:
            if (v123->des_i_desstruct_flag <= 0)
            {
                v33 = (tGameObjectMain*)v123->des_o_actor_desstruct;
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
                if ((-v123->des_i_desstruct_flag & v32->extended->fields->designFlags) == 0)
                {
                    v31 = (tGameObjectMain*)v123->des_o_actor_desstruct;
                    if (v31)
                    {
                        if ((unsigned int)v31 <= 2)
                            v31 = AI_MainActor[(uint32_t)v31 - 1];
                        v30 = v31;
                    }
                    else
                    {
                        v30 = 0;
                    }
                    if ((unsigned __int8)v30->extended->fields->designInt2 == v123->des_i_desstruct_int)
                        ++v110;
                }
            }
            else
            {
                v37 = (tGameObjectMain*)v123->des_o_actor_desstruct;
                if (v37)
                {
                    if ((unsigned int)v37 <= 2)
                        v37 = AI_MainActor[(uint32_t)v37 - 1];
                    v36 = v37;
                }
                else
                {
                    v36 = 0;
                }
                if ((v123->des_i_desstruct_flag & v36->extended->fields->designFlags) != 0)
                {
                    v35 = (tGameObjectMain*)v123->des_o_actor_desstruct;
                    if (v35)
                    {
                        if ((unsigned int)v35 <= 2)
                            v35 = AI_MainActor[(uint32_t)v35 - 1];
                        v34 = v35;
                    }
                    else
                    {
                        v34 = 0;
                    }
                    if ((unsigned __int8)v34->extended->fields->designInt2 == v123->des_i_desstruct_int)
                        ++v110;
                }
            }
            break;
        case 3:
            if (v123->des_i_desstruct_flag <= 0)
            {
                v25 = (tGameObjectMain*)v123->des_o_actor_desstruct;
                if (v25)
                {
                    if ((unsigned int)v25 <= 2)
                        v25 = AI_MainActor[(uint32_t)v25 - 1];
                    v24 = v25;
                }
                else
                {
                    v24 = 0;
                }
                if ((-v123->des_i_desstruct_flag & v24->extended->fields->designFlags) == 0)
                {
                    v23 = (tGameObjectMain*)v123->des_o_actor_desstruct;
                    if (v23)
                    {
                        if ((unsigned int)v23 <= 2)
                            v23 = AI_MainActor[(uint32_t)v23 - 1];
                        v22 = v23;
                    }
                    else
                    {
                        v22 = 0;
                    }
                    if ((unsigned __int8)v22->extended->fields->designInt3 == v123->des_i_desstruct_int)
                        ++v110;
                }
            }
            else
            {
                v29 = (tGameObjectMain*)v123->des_o_actor_desstruct;
                if (v29)
                {
                    if ((unsigned int)v29 <= 2)
                        v29 = AI_MainActor[(uint32_t)v29 - 1];
                    v28 = v29;
                }
                else
                {
                    v28 = 0;
                }
                if ((v123->des_i_desstruct_flag & v28->extended->fields->designFlags) != 0)
                {
                    v27 = (tGameObjectMain*)v123->des_o_actor_desstruct;
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
                    if ((unsigned __int8)v26->extended->fields->designInt3 == v123->des_i_desstruct_int)
                        ++v110;
                }
            }
            break;
        case 0:
            if (v123->des_i_desstruct_flag <= 0)
            {
                v19 = (tGameObjectMain*)v123->des_o_actor_desstruct;
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
                if ((-v123->des_i_desstruct_flag & v18->extended->fields->designFlags) == 0)
                    ++v110;
            }
            else
            {
                v21 = (tGameObjectMain*)v123->des_o_actor_desstruct;
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
                if ((v123->des_i_desstruct_flag & v20->extended->fields->designFlags) != 0)
                    ++v110;
            }
            break;
        }
        if (!v110 && v123->i_was_ready)
        {
            v123->des_f_tempo = v123->des_f_tempo - TIM_gf_dt;
            if (v123->des_f_tempo > 0.0)
            {
                v110 = 1;
            }
            else
            {
                if (v123->des_i_SPFlag_associe_vrai > 0)
                    AI_EvalFunc_ARRBitSet_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_vrai);
                if (v123->des_i_SPFlag_associe_faux > 0)
                    AI_EvalFunc_ARRBitClear_C(
                        (int*)(helper_text_hlptxt_tests_depart_univVars_Flag_speciaux_sauve + 4),
                        v123->des_i_SPFlag_associe_faux);
                v129 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
            }
        }
        if (!v110)
        {
            v123->i_ready = 1;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v110 = 0;
    }
    if (!v123->i_ready)
        goto LABEL_462;
    v123->i_was_ready = 1;
    if (SND_i_IsTrackPlaying(1040))
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    v123->f_temps_ecoule = TIM_gf_MainClock - TIM_gf_LockTime;
    v97 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fctr_helper_text_hlptxt_tests_depart_helper_text_hlptxt_tests_wait);
    if (v97 == 1000)
        LABEL_462:
    result = 1000;
    else
        result = v97;
    return result;
}

int helper_text_hlptxt_exec_ask_display()
{
    int* v0; // eax
    char* v1; // eax
    char* v2; // eax
    float v4; // [esp+0h] [ebp-90h]
    tScriptStruct_HelperText* v5; // [esp+4h] [ebp-8Ch]
    tGameObjectMain* v6; // [esp+Ch] [ebp-84h]
    tGameObjectMain* v7; // [esp+14h] [ebp-7Ch]
    float v8; // [esp+18h] [ebp-78h]
    float v9; // [esp+34h] [ebp-5Ch]
    tMessage Src; // [esp+4Ch] [ebp-44h] BYREF
    tScriptStruct_HelperText* v11; // [esp+84h] [ebp-Ch]
    int v12; // [esp+88h] [ebp-8h]
    int v13; // [esp+8Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_HelperText*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v11 = v5;
    v12 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_helper_text_hlptxt_exec_ask_display_helper_text_hlptxt_tests_depart = (int)AI_ul_RealResolveOneRefFunction(0x35007498u);
        helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao = AI_ul_RealResolveOneRefObject(0x7100E7ACu);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_texte_message_a_traiter,
            (void*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao,
            52);
        helper_text_hlptxt_exec_ask_display_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_exec_ask_display_GST_Global_gao_f_game_speed,
            (void*)helper_text_hlptxt_exec_ask_display_GST_Global_gao,
            3548);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_i_encart,
            (void*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao,
            76);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_i_Sound_ID,
            (void*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao,
            64);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_f_Sound_Volume,
            (void*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao,
            72);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_f_near,
            (void*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao,
            220);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_f_far,
            (void*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao,
            224);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_i_Sound_Localisation_type,
            (void*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao,
            156);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_message_a_traiter,
            (void*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao,
            0);
        AI2C_ResolveBuffer(
            &helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_prio_message_a_traiter,
            (void*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao,
            60);
        helper_text_hlptxt_exec_ask_display_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
    }
    v0 = (int*)AI_EvalVarArray_C(10, (char*)&v11->__des_at_lignes_texte_Dimension1, v11->i_index_ligne);
    if (AI_EvalFunc_TEXTIsEmpty_C(v0))
    {
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_helper_text_hlptxt_exec_ask_display_helper_text_hlptxt_tests_depart);
    }
    else
    {
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x100, 0);
        v1 = AI_EvalVarArray_C(10, (char*)&v11->__des_at_lignes_texte_Dimension1, v11->i_index_ligne);
        memcpy((void*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_texte_message_a_traiter, v1, 8u);
        Src.msgID = 30;
        Src.gao = AI_gpst_CurrentGameObject;
        Src.Int1 = 0;
        Src.f1 = -1.0;
        Src.v2.x = 0.0;
        Src.v2.y = 0.0;
        Src.v2.z = 0.0;
        if (v11->des_i_cine_temporaire)
        {
            Src.v1.x = 0.5;
            Src.v1.y = 0.40000001f;
        }
        else
        {
            Src.v1.x = 0.0;
            Src.v1.y = 0.0;
        }
        Src.v1.z = 0.0;
        if (v11->des_i_auto_delay == 0.0)
            Src.f2 = SND_SoundDurationGet(AI_gpst_CurrentGameObject, v11->des_ai_son_associe[v11->i_index_ligne]);
        else
            Src.f2 = 0.0f;
        if (Src.f2 == 0.0f)
        {
            if (*(float*)AI_EvalVarArray_C2((char*)&v11->__des_f_duree_dialogues_Dimension1, v11->i_index_ligne) == 0.0)
            {
                v2 = AI_EvalFunc_TEXTToString_C((tTextGet*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_texte_message_a_traiter);
                v13 = AI_EvalFunc_TEXTLength_C(v2);
                v4 = (float)v13;
                v8 = v4 * 0.079999998f;
                if (v8 <= 1.0e14f)
                {
                    if (v8 >= 1.0f)
                        v9 = v4 * 0.079999998f;
                    else
                        v9 = 1.0f;
                }
                else
                {
                    v9 = 1.0e14f;
                }
                Src.f2 = v9;
            }
            else
            {
                Src.f2 = *(float*)AI_EvalVarArray_C2((char*)&v11->__des_f_duree_dialogues_Dimension1, v11->i_index_ligne);
            }
        }
        Src.f2 = Src.f2 * *(float*)helper_text_hlptxt_exec_ask_display_GST_Global_gao_f_game_speed;
        v11->f_tempo = Src.f2 + 1.0f;
        *(uint32_t*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_i_encart = 0;
        *(uint32_t*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_i_encart = v11->des_ai_encart[v11->i_index_ligne];
        *(uint32_t*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_i_Sound_ID = v11->des_ai_son_associe[v11->i_index_ligne];
        Src.gao2 = (tGameObjectMain*)v11->des_ai_perso_localisation[v11->i_index_ligne];
        if (*(float*)AI_EvalVarArray_C2((char*)&v11->__des_af_volume_associe_Dimension1, v11->i_index_ligne) == 0.0f)
            *(uint32_t*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_f_Sound_Volume = 1061997773;
        else
            *(float*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_f_Sound_Volume = *(float*)AI_EvalVarArray_C2((char*)&v11->__des_af_volume_associe_Dimension1, v11->i_index_ligne);
        if (*(float*)AI_EvalVarArray_C2((char*)&v11->__des_af_far_associe_Dimension1, v11->i_index_ligne) != 0.0)
        {
            *(float*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_f_near = *(float*)AI_EvalVarArray_C2(
                (char*)&v11->__des_af_near_associe_Dimension1,
                v11->i_index_ligne);
            *(float*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_f_far = *(float*)AI_EvalVarArray_C2(
                (char*)&v11->__des_af_far_associe_Dimension1,
                v11->i_index_ligne);
        }
        *(uint32_t*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_i_Sound_Localisation_type = v11->i_Sound_Localisation_type;
        memcpy((void*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_message_a_traiter, &Src, 0x34u);
        *(uint32_t*)helper_text_hlptxt_exec_ask_display_GST_AffichageTexte_gao_prio_message_a_traiter = v11->des_i_prio_des_textes;
    }
    if (v11->des_i_inventaire_interdit)
    {
        v12 = helper_text_hlptxt_exec_ask_display_GST_Board_panel_gao;
        v7 = (tGameObjectMain*)helper_text_hlptxt_exec_ask_display_GST_Board_panel_gao;
        if (helper_text_hlptxt_exec_ask_display_GST_Board_panel_gao)
        {
            if ((unsigned int)helper_text_hlptxt_exec_ask_display_GST_Board_panel_gao <= 2)
                v7 = AI_MainActor[helper_text_hlptxt_exec_ask_display_GST_Board_panel_gao - 1];
            AI_EvalFunc_OBJ_CustomBitsSet_C(v7, 0x100, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_CustomBitsSet_C(0, 0x100, 0);
        }
    }
    if (v11->des_i_look_interdit)
    {
        v12 = helper_text_hlptxt_exec_ask_display_GST_Board_panel_gao;
        v6 = (tGameObjectMain*)helper_text_hlptxt_exec_ask_display_GST_Board_panel_gao;
        if (helper_text_hlptxt_exec_ask_display_GST_Board_panel_gao)
        {
            if ((unsigned int)helper_text_hlptxt_exec_ask_display_GST_Board_panel_gao <= 2)
                v6 = AI_MainActor[helper_text_hlptxt_exec_ask_display_GST_Board_panel_gao - 1];
            AI_EvalFunc_OBJ_CustomBitsSet_C(v6, 0x200, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_CustomBitsSet_C(0, 0x200, 0);
        }
    }
    if (v11->des_i_blocage_joy)
        AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)helper_text_hlptxt_exec_ask_display_GST_Global_gao, 0x8000, 0);
    return 1000;
}