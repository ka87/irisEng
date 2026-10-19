#include "ai_compiledscripts_inc.h"

//get_object_on_ground_goog_reflex
int get_object_on_ground_goog_reflex__PJ_Sally_gao;
int get_object_on_ground_goog_reflex_univVars;
int get_object_on_ground_goog_reflex_univVars_mdisk_possessionflags;
int get_object_on_ground_goog_reflex_univVars_Perso_ID_equipement;
int get_object_on_ground_goog_reflex_univVars_objet_valeur1;
int get_object_on_ground_goog_reflex_univVars_objet_status;
int get_object_on_ground_goog_reflex_univVars_objet_inventaire;
int get_object_on_ground_goog_reflex_univVars_Flag_speciaux_sauve;
int get_object_on_ground_goog_reflex_GST_Board_panel_gao;
int get_object_on_ground_goog_reflex_GST_Global_gao;
int get_object_on_ground_goog_reflex_GST_Global_gao_i_Perso_ID_a_checker;
int get_object_on_ground_goog_reflex_univVars_Perso_ID_Energie_courante;
int get_object_on_ground_goog_reflex_univVars_Perso_ID_Energie_max;
int get_object_on_ground_goog_reflex_GST_AffichageTexte_gao;
int get_object_on_ground_goog_reflex_GST_AffichageTexte_gao_i_affiche;
int get_object_on_ground_goog_reflex_GST_AffichageTexte_gao_message_a_traiter;
int get_object_on_ground_goog_reflex_GST_AffichageTexte_gao_i_encart;
int get_object_on_ground_goog_reflex_GST_AffichageTexte_gao_i_Sound_ID;
int get_object_on_ground_goog_reflex_GST_AffichageTexte_gao_prio_message_a_traiter;
int get_object_on_ground_goog_reflex_univVars_Perso_ID_Companion;
int get_object_on_ground_goog_reflex_GST_Global_gao_o_actor_texte;

//get_object_on_ground_goog_init
int get_object_on_ground_goog_init_univVars;
int get_object_on_ground_goog_init_univVars_Flag_speciaux_sauve;
int get_object_on_ground_goog_init_GST_Global_gao;
int get_object_on_ground_goog_init_GST_Global_gao_i_map_just_reset;
int get_object_on_ground_goog_init_univVars_Special_vectors;
int fct_get_object_on_ground_goog_init_get_object_on_ground_goog_chute;
int get_object_on_ground_goog_init_SFX_light_smoke_gao;
int fct_get_object_on_ground_goog_init_get_object_on_ground_goog_reflex;
int fct_get_object_on_ground_goog_init_get_object_on_ground_goog_go;

//get_object_on_ground_goog_go
int get_object_on_ground_goog_go_GST_Fight_Manager_gao;
int get_object_on_ground_goog_go_GST_Fight_Manager_gao_i_Evil_NbActor;
int get_object_on_ground_goog_go__PJ_Sally_gao;
int get_object_on_ground_goog_go__PJ_Sally_gao_current_pm_mode;
int get_object_on_ground_goog_go__PJ_Sally_gao_current_pm_priority;
int get_object_on_ground_goog_go_GST_Board_panel_gao;
int get_object_on_ground_goog_go_GST_Board_panel_gao_i_sally_priority;
int get_object_on_ground_goog_go_GST_Board_panel_gao_o_sally_contextuel;
int get_object_on_ground_goog_go_GST_Board_panel_gao_i_sally_action;
int get_object_on_ground_goog_go_GST_Board_panel_gao_v_sally_offset;
int get_object_on_ground_goog_go_GST_Board_panel_gao_i_force_interface;

//get_object_on_ground_goog_chute
int fct_get_object_on_ground_goog_chute_get_object_on_ground_goog_init;

int get_object_on_ground_goog_reflex()
{
    int* v0; // eax
    float X; // [esp+4h] [ebp-1BCh]
    float v3; // [esp+18h] [ebp-1A8h]
    float v4; // [esp+1Ch] [ebp-1A4h]
    int v5; // [esp+20h] [ebp-1A0h]
    char* v6; // [esp+24h] [ebp-19Ch]
    uint32_t* v7; // [esp+28h] [ebp-198h]
    float v8; // [esp+2Ch] [ebp-194h]
    tScriptStruct_GetObjectOnGround* v9; // [esp+30h] [ebp-190h]
    tGameObjectMain* v10; // [esp+38h] [ebp-188h]
    uint32_t* v11; // [esp+48h] [ebp-178h]
    tGameObjectMain* v12; // [esp+58h] [ebp-168h]
    tGameObjectMain* v13; // [esp+5Ch] [ebp-164h]
    double v14; // [esp+60h] [ebp-160h]
    float v15; // [esp+6Ch] [ebp-154h]
    float v16; // [esp+70h] [ebp-150h]
    float v17; // [esp+74h] [ebp-14Ch]
    Vec3 a2; // [esp+84h] [ebp-13Ch] BYREF
    Vec3 v19; // [esp+94h] [ebp-12Ch] BYREF
    int v20; // [esp+A0h] [ebp-120h]
    Vec3 v21; // [esp+A4h] [ebp-11Ch] BYREF
    Vec3 v22; // [esp+B0h] [ebp-110h] BYREF
    float v23; // [esp+BCh] [ebp-104h]
    float v24; // [esp+D0h] [ebp-F0h]
    int v25; // [esp+E0h] [ebp-E0h]
    Vec3 a3; // [esp+E4h] [ebp-DCh] BYREF
    Vec3 v27; // [esp+F0h] [ebp-D0h] BYREF
    void* v28; // [esp+100h] [ebp-C0h]
    Vec3 v29; // [esp+104h] [ebp-BCh] BYREF
    tMessage v30; // [esp+120h] [ebp-A0h] BYREF
    tMessage v31; // [esp+154h] [ebp-6Ch] BYREF
    int v32; // [esp+188h] [ebp-38h]
    tScriptStruct_GetObjectOnGround* v33; // [esp+190h] [ebp-30h]
    int v34; // [esp+194h] [ebp-2Ch]
    unsigned int v35; // [esp+198h] [ebp-28h]
    Vec3 v36; // [esp+19Ch] [ebp-24h] BYREF
    Vec3 v37; // [esp+1A8h] [ebp-18h] BYREF
    int v38; // [esp+1B4h] [ebp-Ch]
    void* Src; // [esp+1BCh] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v9 = (tScriptStruct_GetObjectOnGround*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v9 = 0;
    v33 = v9;
    v34 = 0;
    v38 = 0;
    v35 = 0;
    v24 = 0.0;
    v20 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        get_object_on_ground_goog_reflex__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        get_object_on_ground_goog_reflex_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_reflex_univVars_mdisk_possessionflags,
            (void*)get_object_on_ground_goog_reflex_univVars,
            6756);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_reflex_univVars_Perso_ID_equipement,
            (void*)get_object_on_ground_goog_reflex_univVars,
            40);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_reflex_univVars_objet_valeur1,
            (void*)get_object_on_ground_goog_reflex_univVars,
            4100);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_reflex_univVars_objet_status,
            (void*)get_object_on_ground_goog_reflex_univVars,
            3896);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_reflex_univVars_objet_inventaire,
            (void*)get_object_on_ground_goog_reflex_univVars,
            1480);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_reflex_univVars_Flag_speciaux_sauve,
            (void*)get_object_on_ground_goog_reflex_univVars,
            4488);
        get_object_on_ground_goog_reflex_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        get_object_on_ground_goog_reflex_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_reflex_GST_Global_gao_i_Perso_ID_a_checker,
            (void*)get_object_on_ground_goog_reflex_GST_Global_gao,
            3288);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_reflex_univVars_Perso_ID_Energie_courante,
            (void*)get_object_on_ground_goog_reflex_univVars,
            1148);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_reflex_univVars_Perso_ID_Energie_max,
            (void*)get_object_on_ground_goog_reflex_univVars,
            1096);
        get_object_on_ground_goog_reflex_GST_AffichageTexte_gao = AI_ul_RealResolveOneRefObject(0x7100E7ACu);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_reflex_GST_AffichageTexte_gao_i_affiche,
            (void*)get_object_on_ground_goog_reflex_GST_AffichageTexte_gao,
            204);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_reflex_GST_AffichageTexte_gao_message_a_traiter,
            (void*)get_object_on_ground_goog_reflex_GST_AffichageTexte_gao,
            0);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_reflex_GST_AffichageTexte_gao_i_encart,
            (void*)get_object_on_ground_goog_reflex_GST_AffichageTexte_gao,
            76);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_reflex_GST_AffichageTexte_gao_i_Sound_ID,
            (void*)get_object_on_ground_goog_reflex_GST_AffichageTexte_gao,
            64);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_reflex_GST_AffichageTexte_gao_prio_message_a_traiter,
            (void*)get_object_on_ground_goog_reflex_GST_AffichageTexte_gao,
            60);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_reflex_univVars_Perso_ID_Companion,
            (void*)get_object_on_ground_goog_reflex_univVars,
            1040);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_reflex_GST_Global_gao_o_actor_texte,
            (void*)get_object_on_ground_goog_reflex_GST_Global_gao,
            3616);
    }
    if (v33->GFX_Halo != -1)
    {
        v23 = TIM_gf_dt;
        v33->f_angle_halo = TIM_gf_dt * 3.141593f + v33->f_angle_halo;
        if (v33->f_angle_halo >= 6.283185)
            v33->f_angle_halo = v33->f_angle_halo - 6.283185f;
        v24 = sinf(v33->f_angle_halo);
        v8 = (float)fabs(v24);
        v24 = v8;
        v20 = AI_EvalFunc_COLORBlend_C(0, v33->des_halo_couleur, v8);
        GFXSetCurGlobal(v33->GFX_Halo);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v20);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21000, v33->des_halo_radius);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21104, v33->des_halo_number);
        X = v33->des_halo_extrude * (0.89999998f * v8 + 0.1f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21004, X);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
        AI_EvalFunc_MATHVecLocalToGlobal_C(AI_gpst_CurrentGameObject, &v33->des_v_halo_offset, &a3);
        MATH_VecAddVec(&v37, &a2, &a3);
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v37);
        if (v33->des_halo_extrude_axe)
        {
            if (v33->des_halo_extrude_axe == 1)
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v36);
                v16 = (float)v33->des_halo_extrude_sens;
                v22.x = v36.x * v16;
                v22.y = v36.y * v16;
                v22.z = v36.z * v16;
                AI_EvalFunc_GFXSetv_C_CURRENT(v33->GFX_Halo, 21201, &v22);
            }
            else if (v33->des_halo_extrude_axe == 2)
            {
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v27);
                v15 = (float)v33->des_halo_extrude_sens;
                v19.x = v27.x * v15;
                v19.y = v27.y * v15;
                v19.z = v27.z * v15;
                AI_EvalFunc_GFXSetv_C_CURRENT(v33->GFX_Halo, 21201, &v19);
            }
        }
        else
        {
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v21);
            v17 = (float)v33->des_halo_extrude_sens;
            v29.x = v21.x * v17;
            v29.y = v21.y * v17;
            v29.z = v21.z * v17;
            AI_EvalFunc_GFXSetv_C_CURRENT(v33->GFX_Halo, 21201, &v29);
        }
    }
    v33->i_reflex_prio_contextuel = 0;
    Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v31, Src, sizeof(v31));
    while (v31.msgID)
    {
        if (v31.msgID == 10)
        {
            v14 = v31.f2 + MATH_gd_magicDoubleForRound;
            v33->i_reflex_prio_contextuel = (*(uint32_t *)&v14);
        }
        else if (v31.msgID == 37)
        {
            if (v33->Indice_de_lobjet >= 0)
            {
                v13 = v31.gao;
                if (v31.gao)
                {
                    if (v31.gao <= (tGameObjectMain*)2)
                        v13 = AI_MainActor[(int)v31.gao - 1];
                    v12 = v13;
                }
                else
                {
                    v12 = 0;
                }
                if (v12 == (tGameObjectMain*)get_object_on_ground_goog_reflex__PJ_Sally_gao)
                {
                    if (v33->Indice_de_lobjet < 1100)
                    {
                        if (((*(uint32_t*)(get_object_on_ground_goog_reflex_univVars_objet_status + 4 * v33->Indice_de_lobjet + 4) & 0x400000) == 0
                            || !*(uint32_t*)AI_EvalVarArray2_C(
                                1,
                                (char*)get_object_on_ground_goog_reflex_univVars_objet_inventaire,
                                v33->Indice_de_lobjet,
                                8))
                            && *(int*)AI_EvalVarArray2_C(
                                1,
                                (char*)get_object_on_ground_goog_reflex_univVars_objet_inventaire,
                                v33->Indice_de_lobjet,
                                8) < 99)
                        {
                            v7 = (uint32_t*)AI_EvalVarArray2_C(
                                1,
                                (char*)get_object_on_ground_goog_reflex_univVars_objet_inventaire,
                                v33->Indice_de_lobjet,
                                8);
                            ++* v7;
                        }
                        if (v33->Indice_de_lobjet + 1100 <= 0)
                            v38 = 0;
                        else
                            v38 = AI_EvalFunc_ARRBitTest_C(
                                (int*)(get_object_on_ground_goog_reflex_univVars_Flag_speciaux_sauve + 4),
                                v33->Indice_de_lobjet + 1100);
                        if (v38)
                        {
                            v34 = get_object_on_ground_goog_reflex_GST_Board_panel_gao;
                            v38 = *(uint32_t*)(get_object_on_ground_goog_reflex_univVars_objet_status + 4 * v33->Indice_de_lobjet + 4) & 0x7000000;
                            v38 >>= 24;
                            if (v38 != 7
                                && (*(uint32_t*)(get_object_on_ground_goog_reflex_univVars_objet_status + 4 * v33->Indice_de_lobjet + 4) & 0x100) != 0)
                            {
                                v0 = (int*)AI_EvalVarArray2_C(
                                    1,
                                    (char*)get_object_on_ground_goog_reflex_univVars_Perso_ID_equipement,
                                    v38,
                                    8);
                                *v0 = v33->Indice_de_lobjet;
                                *(uint32_t*)get_object_on_ground_goog_reflex_GST_Global_gao_i_Perso_ID_a_checker |= 0x100u;
                            }
                            if (v33->Indice_de_lobjet == 28)
                            {
                                v6 = AI_EvalVarArray_C2((char*)get_object_on_ground_goog_reflex_univVars_Perso_ID_Energie_courante, 8);
                                *(float*)v6 = *(float*)v6 + 980.0f;
                                v11 = (uint32_t*)(get_object_on_ground_goog_reflex_univVars_objet_valeur1 + 116);
                                v5 = (*v11 - 1000)
                                    * (*(uint32_t*)AI_EvalVarArray2_C(
                                        1,
                                        (char*)get_object_on_ground_goog_reflex_univVars_objet_inventaire,
                                        28,
                                        8)
                                        + 2);
                                *(float*)AI_EvalVarArray_C2((char*)get_object_on_ground_goog_reflex_univVars_Perso_ID_Energie_max, 8) = (float)v5;
                                v4 = *(float*)AI_EvalVarArray_C2(
                                    (char*)get_object_on_ground_goog_reflex_univVars_Perso_ID_Energie_courante,
                                    8);
                                if (*(float*)AI_EvalVarArray_C2(
                                    (char*)get_object_on_ground_goog_reflex_univVars_Perso_ID_Energie_max,
                                    8) < (double)v4)
                                {
                                    v3 = *(float*)AI_EvalVarArray_C2(
                                        (char*)get_object_on_ground_goog_reflex_univVars_Perso_ID_Energie_max,
                                        8);
                                    *(float*)AI_EvalVarArray_C2(
                                        (char*)get_object_on_ground_goog_reflex_univVars_Perso_ID_Energie_courante,
                                        8) = v3;
                                }
                            }
                        }
                    }
                    else
                    {
                        *(uint32_t*)get_object_on_ground_goog_reflex_univVars_mdisk_possessionflags |= 1 << (v33->Indice_de_lobjet
                            - 76);
                        *(uint32_t*)AI_EvalVarArray2_C(
                            1,
                            (char*)get_object_on_ground_goog_reflex_univVars_Perso_ID_equipement,
                            0,
                            8) = 27;
                        *(uint32_t*)(get_object_on_ground_goog_reflex_univVars_objet_valeur1 + 112) = v33->Indice_de_lobjet - 1100;
                    }
                    if (!*(uint32_t*)get_object_on_ground_goog_reflex_GST_AffichageTexte_gao_i_affiche)
                    {
                        v31.msgID = 30;
                        v31.gao = AI_gpst_CurrentGameObject;
                        v31.Int1 = 0;
                        v31.Int2 = 0;
                        v31.f2 = 2.0;
                        v31.f1 = (float)v33->Indice_de_lobjet;
                        v31.v1.x = 0.0;
                        v31.v1.y = 0.0;
                        v31.v1.z = 0.0;
                        v31.v2.x = 0.0;
                        v31.v2.y = 0.0;
                        v31.v2.z = 0.0;
                        v31.gao2 = 0;
                        memcpy((void*)get_object_on_ground_goog_reflex_GST_AffichageTexte_gao_message_a_traiter, &v31, 0x34u);
                        *(uint32_t*)get_object_on_ground_goog_reflex_GST_AffichageTexte_gao_i_encart = 2;
                        *(uint32_t*)get_object_on_ground_goog_reflex_GST_AffichageTexte_gao_i_Sound_ID = -1;
                        *(uint32_t*)get_object_on_ground_goog_reflex_GST_AffichageTexte_gao_prio_message_a_traiter = 2;
                    }
                }
            }
            if (v33->SF_objet_deja_recupere && v33->SF_objet_deja_recupere > 0)
                AI_EvalFunc_ARRBitSet_C(
                    (int*)(get_object_on_ground_goog_reflex_univVars_Flag_speciaux_sauve + 4),
                    v33->SF_objet_deja_recupere);
            if (v33->SF_objet_recuperable)
            {
                if (v33->GFX_Halo != -1)
                {
                    AI_EvalFunc_GFXDel_C_CURRENT(v33->GFX_Halo);
                    v33->GFX_Halo = -1;
                }
                v25 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
                if (v25 != 1000)
                    return v25;
            }
            if (v33->i_nbr_dobjets)
            {
                if (!--v33->i_nbr_dobjets)
                {
                    if (v33->GFX_Halo != -1)
                    {
                        AI_EvalFunc_GFXDel_C_CURRENT(v33->GFX_Halo);
                        v33->GFX_Halo = -1;
                    }
                    v32 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
                    if (v32 != 1000)
                        return v32;
                }
            }
        }
        v28 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v31, v28, sizeof(v31));
    }
    if (v33->i_plus_de_place)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v33->f_time_place, 5.0))
        {
            v33->i_plus_de_place = 0;
            if (*(uint32_t*)get_object_on_ground_goog_reflex_univVars_Perso_ID_Companion != -1)
            {
                v35 = *(uint32_t*)(get_object_on_ground_goog_reflex_GST_Global_gao_o_actor_texte
                    + 4 * *(uint32_t*)get_object_on_ground_goog_reflex_univVars_Perso_ID_Companion
                    + 4);
                v30.msgID = 30;
                v30.Int1 = 70;
                v30.Int2 = 0;
                v30.f2 = 2.0;
                v30.v1.x = 0.0;
                v30.v1.y = 0.0;
                v30.v1.z = 0.0;
                v30.v2.x = 0.0;
                v30.v2.y = 0.0;
                v30.v2.z = 0.0;
                v30.gao = 0;
                v30.gao2 = (tGameObjectMain*)get_object_on_ground_goog_reflex__PJ_Sally_gao;
                v10 = (tGameObjectMain*)v35;
                if (v35)
                {
                    if (v35 <= 2)
                        v10 = AI_MainActor[v35 - 1];
                    AI_EvalFunc_MSGSend_C(v10, &v30);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &v30);
                }
            }
        }
    }
    return 1000;
}

int get_object_on_ground_goog_init()
{
    char* v0; // eax
    char* v1; // eax
    char* v3; // eax
    tScriptStruct_GetObjectOnGround* v4; // [esp+0h] [ebp-C8h]
    tGameObjectMain* v5; // [esp+8h] [ebp-C0h]
    int v6; // [esp+20h] [ebp-A8h]
    Vec3 v7; // [esp+2Ch] [ebp-9Ch] BYREF
    Vec3 a1; // [esp+38h] [ebp-90h] BYREF
    Vec3 v9; // [esp+44h] [ebp-84h] BYREF
    Vec3 v10; // [esp+50h] [ebp-78h] BYREF
    unsigned int v11; // [esp+64h] [ebp-64h]
    Vec3 v12; // [esp+74h] [ebp-54h] BYREF
    int v13; // [esp+80h] [ebp-48h]
    Vec3 a2; // [esp+84h] [ebp-44h] BYREF
    unsigned int v15; // [esp+90h] [ebp-38h]
    int v16; // [esp+94h] [ebp-34h]
    tScriptStruct_GetObjectOnGround* v17; // [esp+9Ch] [ebp-2Ch]
    int v18; // [esp+A0h] [ebp-28h]
    Vec3 v19; // [esp+A4h] [ebp-24h] BYREF
    Vec3 a3; // [esp+B0h] [ebp-18h] BYREF
    Vec3 v21; // [esp+BCh] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_GetObjectOnGround*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v17 = v4;
    v18 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        get_object_on_ground_goog_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_init_univVars_Flag_speciaux_sauve,
            (void*)get_object_on_ground_goog_init_univVars,
            4488);
        get_object_on_ground_goog_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_init_GST_Global_gao_i_map_just_reset,
            (void*)get_object_on_ground_goog_init_GST_Global_gao,
            3300);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_init_univVars_Special_vectors,
            (void*)get_object_on_ground_goog_init_univVars,
            5260);
        fct_get_object_on_ground_goog_init_get_object_on_ground_goog_chute = (int)AI_ul_RealResolveOneRefFunction(0x9E00459B);
        get_object_on_ground_goog_init_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        fct_get_object_on_ground_goog_init_get_object_on_ground_goog_reflex = (int)AI_ul_RealResolveOneRefFunction(0x7100CAF9u);
        fct_get_object_on_ground_goog_init_get_object_on_ground_goog_go = (int)AI_ul_RealResolveOneRefFunction(0x7100CAFDu);
    }
    v17->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
    if (v17->SF_objet_deja_recupere)
    {
        v18 = v17->SF_objet_deja_recupere > 0
            && AI_EvalFunc_ARRBitTest_C(
                (int*)(get_object_on_ground_goog_init_univVars_Flag_speciaux_sauve + 4),
                v17->SF_objet_deja_recupere);
        if (v18)
            v15 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
    }
    AI_EvalFunc_OBJ_ExtraFlagsSet_C(AI_gpst_CurrentGameObject, 8, 0);
    AI_EvalFunc_OBJ_AILodSet_C(AI_gpst_CurrentGameObject, 4.0);
    if (v17->SF_objet_recuperable)
    {
        if (v17->SF_objet_recuperable <= 0)
        {
            v18 = 0;
        }
        else if (v17->SF_objet_recuperable < 10000)
        {
            v18 = AI_EvalFunc_ARRBitTest_C(
                (int*)(get_object_on_ground_goog_init_univVars_Flag_speciaux_sauve + 4),
                v17->SF_objet_recuperable);
        }
        else
        {
            v18 = 1
                - AI_EvalFunc_ARRBitTest_C(
                    (int*)(get_object_on_ground_goog_init_univVars_Flag_speciaux_sauve + 4),
                    v17->SF_objet_recuperable - 10000);
        }
        if (v18)
        {
            if (*(uint32_t*)get_object_on_ground_goog_init_GST_Global_gao_i_map_just_reset)
            {
                if (v17->SV_objet_recuperable_position != -1)
                {
                    v7.x = 0.0;
                    v7.y = 0.0;
                    v7.z = v17->des_f_OffsetZ;
                    v3 = AI_EvalVarArray_C3(
                        (char*)get_object_on_ground_goog_init_univVars_Special_vectors,
                        v17->SV_objet_recuperable_position);
                    MATH_VecAddVec(&v21, (Vec3*)v3, &v7);
                    AI_EvalFunc_OBJSetPos_C_CURRENT(&v21);
                    v12.x = 0.0;
                    v12.y = 0.0;
                    v12.z = 1.0;
                    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v9);
                    AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v12, &v9);
                }
            }
            else
            {
                if (v17->SV_objet_recuperable_position != -1 && !v17->i_chute_termine)
                {
                    v0 = AI_EvalVarArray_C3(
                        (char*)get_object_on_ground_goog_init_univVars_Special_vectors,
                        v17->SV_objet_recuperable_position);
                    AI_EvalFunc_OBJGetPos_C_CURRENT((Vec3*)v0);
                    a3.x = 0.0;
                    a3.y = 0.0;
                    a3.z = v17->des_f_OffsetZ;
                    v1 = AI_EvalVarArray_C3(
                        (char*)get_object_on_ground_goog_init_univVars_Special_vectors,
                        v17->SV_objet_recuperable_position);
                    MATH_VecAddVec(&a1, (Vec3*)v1, &a3);
                    AI_EvalFunc_OBJSetPos_C_CURRENT(&a1);
                }
                if (v17->i_chute_avec_rebond)
                {
                    if (!v17->i_chute_termine)
                    {
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
                        v19.x = 0.0;
                        v19.y = 0.0;
                        v19.z = v17->des_f_OffsetZ;
                        MATH_VecAddVec(&v10, &a2, &v19);
                        v17->v_pos_init = v10;
                        v17->px = v17->v_pos_init.x;
                        v17->py = v17->v_pos_init.y;
                        v17->f_time = 0.0;
                        v17->f_alfa = 1.0;
                        v17->i_rebond = 0;
                        v13 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_get_object_on_ground_goog_init_get_object_on_ground_goog_chute);
                        if (v13 != 1000)
                            return v13;
                    }
                }
            }
            if (v17->Indice_de_lobjet == 28)
            {
                v17->des_halo = 1;
                v17->des_halo_extrude_axe = 2;
                v17->des_halo_number = 1;
                v17->des_halo_angle = 0.0;
                v17->des_halo_radius = 0.2f;
                v17->des_halo_extrude = 10.0;
                v17->des_halo_couleur = 0x40A0A0;
            }
            if (v17->des_halo)
            {
                v17->GFX_Halo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v17->GFX_Halo, 4, 1);
                v5 = (tGameObjectMain*)get_object_on_ground_goog_init_SFX_light_smoke_gao;
                if (get_object_on_ground_goog_init_SFX_light_smoke_gao)
                {
                    if ((unsigned int)get_object_on_ground_goog_init_SFX_light_smoke_gao <= 2)
                        v5 = AI_MainActor[get_object_on_ground_goog_init_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v17->GFX_Halo, v5, 0);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v17->GFX_Halo, 0, 0);
                }
                AI_EvalFunc_GFXSetf_C_CURRENT(v17->GFX_Halo, 21002, 0.0);
            }
            AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_get_object_on_ground_goog_init_get_object_on_ground_goog_reflex);
            v6 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_get_object_on_ground_goog_init_get_object_on_ground_goog_go);
            if (v6 != 1000)
                return v6;
        }
        else
        {
            v11 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
        }
    }
    else
    {
        AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_get_object_on_ground_goog_init_get_object_on_ground_goog_reflex);
        v16 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_get_object_on_ground_goog_init_get_object_on_ground_goog_go);
        if (v16 != 1000)
            return v16;
    }
    return 1000;
}

int get_object_on_ground_goog_go()
{
    double v0; // st7
    float v2; // [esp+4h] [ebp-D4h]
    float v3; // [esp+4h] [ebp-D4h]
    float v4; // [esp+8h] [ebp-D0h]
    tScriptStruct_GetObjectOnGround* v5; // [esp+Ch] [ebp-CCh]
    tGameObjectMain* v6; // [esp+14h] [ebp-C4h]
    float v7; // [esp+18h] [ebp-C0h]
    float v8; // [esp+1Ch] [ebp-BCh]
    tGameObjectMain* v9; // [esp+30h] [ebp-A8h]
    tGameObjectMain* v10; // [esp+38h] [ebp-A0h]
    tGameObjectMain* v11; // [esp+44h] [ebp-94h]
    int v12; // [esp+48h] [ebp-90h]
    Vec3 v13; // [esp+50h] [ebp-88h] BYREF
    float v14; // [esp+5Ch] [ebp-7Ch]
    float v15; // [esp+64h] [ebp-74h]
    tMessage Src; // [esp+68h] [ebp-70h] BYREF
    float v17; // [esp+9Ch] [ebp-3Ch]
    Vec3 a2; // [esp+A4h] [ebp-34h] BYREF
    tScriptStruct_GetObjectOnGround* v20; // [esp+C0h] [ebp-18h]
    BOOL v21; // [esp+C4h] [ebp-14h]
    float v22; // [esp+C8h] [ebp-10h]
    float v23; // [esp+CCh] [ebp-Ch]
    int v24; // [esp+D0h] [ebp-8h]

    v13.x = 1.0;
    v13.y = 0.0;
    v13.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_GetObjectOnGround*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v20 = v5;
    v15 = 0.0;
    v21 = 0;
    v22 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        get_object_on_ground_goog_go_GST_Fight_Manager_gao = AI_ul_RealResolveOneRefObject(0x9E002FFA);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_go_GST_Fight_Manager_gao_i_Evil_NbActor,
            (void*)get_object_on_ground_goog_go_GST_Fight_Manager_gao,
            2624);
        get_object_on_ground_goog_go__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_go__PJ_Sally_gao_current_pm_mode,
            (void*)get_object_on_ground_goog_go__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_go__PJ_Sally_gao_current_pm_priority,
            (void*)get_object_on_ground_goog_go__PJ_Sally_gao,
            576);
        get_object_on_ground_goog_go_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_go_GST_Board_panel_gao_i_sally_priority,
            (void*)get_object_on_ground_goog_go_GST_Board_panel_gao,
            396);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_go_GST_Board_panel_gao_o_sally_contextuel,
            (void*)get_object_on_ground_goog_go_GST_Board_panel_gao,
            388);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_go_GST_Board_panel_gao_i_sally_action,
            (void*)get_object_on_ground_goog_go_GST_Board_panel_gao,
            412);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_go_GST_Board_panel_gao_v_sally_offset,
            (void*)get_object_on_ground_goog_go_GST_Board_panel_gao,
            400);
        AI2C_ResolveBuffer(
            &get_object_on_ground_goog_go_GST_Board_panel_gao_i_force_interface,
            (void*)get_object_on_ground_goog_go_GST_Board_panel_gao,
            3292);
    }
    v11 = *AI_MainActor;
    v10 = *AI_MainActor;
    if (*AI_MainActor)
    {
        if ((unsigned int)v10 <= 2)
            v10 = AI_MainActor[(uint32_t)v10 - 1];
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v10);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v22 = (float)v0;
    if (v20->f_distance_propose_action + 1.0 <= v22)
    {
        if (v20->i_snd_pas_nutrille != -1 && v20->i_snd_play)
        {
            v23 = TIM_gf_dt;
            v7 = 10.0f * TIM_gf_dt;
            if (v7 < 0.0)
                v7 = 0.0;
            if (v7 > 1.0)
                v7 = 1.0;
            v14 = 0.0f * v7 + v20->f_snd_vol * (1.0f - v7);
            v20->f_snd_vol = v14;
            v3 = v20->f_snd_vol + v20->f_snd_vol;
            SND_SetVol(v20->i_snd_pas_nutrille, v3);
            if (v20->f_snd_vol < 0.0099999998)
            {
                SND_StopRq(v20->i_snd_pas_nutrille);
                v20->i_snd_play = 0;
            }
        }
    }
    else
    {
        v9 = v11;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v9 = AI_MainActor[(uint32_t)v11 - 1];
            AI_EvalFunc_DYNSpeedGetVector_C(v9, &a2);
        }
        else
        {
            AI_EvalFunc_DYNSpeedGetVector_C(0, &a2);
        }
        v13.x = a2.x;
        v13.y = a2.y;
        v13.z = 0.0;
        v15 = MATH_VecLen(&v13);
        if (v20->i_snd_pas_nutrille != -1)
        {
            if (!v20->i_snd_play)
            {
                AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v20->i_snd_pas_nutrille);
                v20->i_snd_play = 1;
            }
            v8 = 10.0f * TIM_gf_dt;
            if (v8 < 0.0)
                v8 = 0.0;
            if (v8 > 1.0)
                v8 = 1.0;
            v17 = v15 / 30.0f * v8 + v20->f_snd_vol * (1.0f - v8);
            v20->f_snd_vol = v17;
            v2 = v20->f_snd_vol + v20->f_snd_vol;
            SND_SetVol(v20->i_snd_pas_nutrille, v2);
        }
        if (v20->f_distance_propose_action > (double)v22
            && v15 < 1.5
            && (!*(uint32_t*)get_object_on_ground_goog_go_GST_Fight_Manager_gao_i_Evil_NbActor || !v20->des_i_interdire_en_fight))
        {
            v24 = (unsigned __int16)*(uint32_t*)(get_object_on_ground_goog_go__PJ_Sally_gao + 4);
            if ((v24 & 1) != 0)
                v21 = !*(uint32_t*)get_object_on_ground_goog_go__PJ_Sally_gao_current_pm_mode
                || *(int*)get_object_on_ground_goog_go__PJ_Sally_gao_current_pm_priority < 20;
            else
                v21 = 0;
            if (v21)
            {
                v4 = (float)v20->des_i_prio_action;
                if (v4 > (double)*(float*)get_object_on_ground_goog_go_GST_Board_panel_gao_i_sally_priority)
                {
                    *(uint32_t*)get_object_on_ground_goog_go_GST_Board_panel_gao_o_sally_contextuel = (uint32_t)AI_gpst_CurrentGameObject;
                    *(float*)get_object_on_ground_goog_go_GST_Board_panel_gao_i_sally_priority = (float)v20->des_i_prio_action;
                    *(uint32_t*)get_object_on_ground_goog_go_GST_Board_panel_gao_i_sally_action = 6;
                    *(uint32_t*)get_object_on_ground_goog_go_GST_Board_panel_gao_v_sally_offset = 0;
                    *(uint32_t*)(get_object_on_ground_goog_go_GST_Board_panel_gao_v_sally_offset + 4) = 0;
                    *(float*)(get_object_on_ground_goog_go_GST_Board_panel_gao_v_sally_offset + 8) = v20->des_f_OffsetZ;
                    *(uint32_t*)get_object_on_ground_goog_go_GST_Board_panel_gao_i_force_interface = 1;
                }
            }
        }
    }
    if (v20->des_f_life_time != 0.0)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v20->f_time_start, v20->des_f_life_time))
        {
            if (v20->GFX_Halo != -1)
            {
                AI_EvalFunc_GFXDel_C_CURRENT(v20->GFX_Halo);
                v20->GFX_Halo = -1;
            }
            v12 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v12 != 1000)
                return v12;
        }
    }
    if (v20->i_reflex_prio_contextuel == v20->des_i_prio_action)
    {
        Src.msgID = 55;
        Src.Int1 = 2;
        if (v20->des_i_saute_dans_linventaire)
            Src.Int2 = v20->Indice_de_lobjet;
        else
            Src.Int2 = -1;
        Src.f2 = 0.0;
        Src.f1 = 0.0;
        Src.gao2 = 0;
        Src.gao = AI_gpst_CurrentGameObject;
        v6 = v11;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v6 = AI_MainActor[(uint32_t)v11 - 1];
            AI_EvalFunc_MSGSend_C(v6, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
    }
    return 1000;
}

int get_object_on_ground_goog_chute()
{
    int result; // eax
    float a4; // [esp+0h] [ebp-E4h]
    float a4a; // [esp+0h] [ebp-E4h]
    float v3; // [esp+4h] [ebp-E0h]
    tScriptStruct_GetObjectOnGround* v4; // [esp+8h] [ebp-DCh]
    float v5; // [esp+18h] [ebp-CCh]
    Vec3 v6; // [esp+1Ch] [ebp-C8h] BYREF
    Vec3 v7; // [esp+30h] [ebp-B4h] BYREF
    float v8; // [esp+3Ch] [ebp-A8h]
    Vec3 dest; // [esp+40h] [ebp-A4h] BYREF
    float v10; // [esp+4Ch] [ebp-98h]
    Vec3 v12; // [esp+54h] [ebp-90h] BYREF
    Vec3 a1; // [esp+64h] [ebp-80h] BYREF
    Vec3 v15; // [esp+70h] [ebp-74h] BYREF
    Vec3 v16; // [esp+7Ch] [ebp-68h] BYREF
    Vec3 a5; // [esp+88h] [ebp-5Ch] BYREF
    Vec3 a2; // [esp+94h] [ebp-50h] BYREF
    int v19; // [esp+A4h] [ebp-40h]
    float v20; // [esp+A8h] [ebp-3Ch]
    tScriptStruct_GetObjectOnGround* v21; // [esp+B0h] [ebp-34h]
    Vec3 a3; // [esp+B4h] [ebp-30h] BYREF
    Vec3 v23; // [esp+C0h] [ebp-24h] BYREF
    Vec3 v24; // [esp+CCh] [ebp-18h] BYREF
    Vec3 v25; // [esp+D8h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_GetObjectOnGround*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v21 = v4;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_get_object_on_ground_goog_chute_get_object_on_ground_goog_init = (int)AI_ul_RealResolveOneRefFunction(0x7100CAFAu);
    }
    v5 = (v21->f_alfa * 18.0f - 1.5f) * v21->f_time - v21->f_time * 20.0f * v21->f_time;
    if (v5 < 0.0)
    {
        if (v21->i_rebond >= 4)
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v25);
            v15.x = 0.0;
            v15.y = 0.0;
            v15.z = 1.0;
            MATH_SubstractVec(&dest, &v25, &v15);
            if (AI_EvalFunc_MTHVecNullToler_C(&dest, 0.0099999998f))
            {
                v21->i_chute_termine = 1;
                v19 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_get_object_on_ground_goog_chute_get_object_on_ground_goog_init);
                if (v19 == 1000)
                    result = 1000;
                else
                    result = v19;
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v6);
                v12.x = 0.0;
                v12.y = 0.0;
                v12.z = 1.0;
                v20 = TIM_gf_dt;
                a4a = 15.0f * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&v6, &v12, a4a, &v16);
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v24);
                AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v16, &v24);
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 1000;
            }
        }
        else
        {
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v21->v_pos_init);
            ++v21->i_rebond;
            v21->f_alfa = v21->f_alfa * 0.64999998f;
            v21->f_time = 0.0;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
    }
    else
    {
        if (AI_EvalFunc_MATHFloatNullEps_C(v21->f_time))
        {
            v21->v_axe.x = AI_EvalFunc_MATHRandFloat_C(-0.2f, 0.2f);
            v21->v_axe.z = AI_EvalFunc_MATHRandFloat_C(-0.2f, 0.2f);
            v21->v_axe.z = AI_EvalFunc_MATHRandFloat_C(-0.2f, 0.2f);
            MATH_NormalizeEqualVector(&v21->v_axe);
        }
        v21->f_time = TIM_gf_dt + v21->f_time;
        a1.x = v21->px;
        a1.y = v21->py;
        a1.z = v21->v_pos_init.z + v5;
        AI_EvalFunc_OBJSetPos_C_CURRENT(&a1);
        if (v21->i_rebond >= 4)
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
            a3.x = 0.0;
            a3.y = 0.0;
            a3.z = 1.0;
            v10 = TIM_gf_dt;
            a4 = 8.0f * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v7);
            AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a5, &v7);
        }
        else
        {
            AI_EvalFunc_MATHVecGlobalToLocal_C(AI_gpst_CurrentGameObject, &v21->v_axe, &v23);
            v8 = TIM_gf_dt;
            v3 = 6.0f * TIM_gf_dt;
            AI_EvalFunc_OBJRotateAround_C(AI_gpst_CurrentGameObject, &v23, v3);
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}