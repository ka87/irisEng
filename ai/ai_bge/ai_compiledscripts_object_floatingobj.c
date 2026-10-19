#include "ai_compiledscripts_inc.h"

//Objet_Flottant_OF_init
int Objet_Flottant_OF_init_univVars;
int Objet_Flottant_OF_init_univVars_Flag_speciaux_sauve;
int Objet_Flottant_OF_init_GST_Global_gao;
int Objet_Flottant_OF_init_GST_Global_gao_o_targets;
int Objet_Flottant_OF_init_GST_Global_gao_i_targets_nb;
int Objet_Flottant_OF_init_GST_Global_gao_v_targets_offset;
int Objet_Flottant_OF_init_univVars_Special_vectors;
int fct_Objet_Flottant_OF_init_Objet_Flottant_OF_afterrec;
int fct_Objet_Flottant_OF_init_Objet_Flottant_OF_mode_ground;
int fct_Objet_Flottant_OF_init_Objet_Flottant_OF_comportement;

//Objet_Flottant_OF_Reward
int Objet_Flottant_OF_Reward_univVars;
int Objet_Flottant_OF_Reward_univVars_objet_valeur1;
int Objet_Flottant_OF_Reward_GST_Global_gao;
int Objet_Flottant_OF_Reward_GST_Board_panel_gao;
int Objet_Flottant_OF_Reward__PJ_Aircraft_gao;
int Objet_Flottant_OF_Reward_univVars_mdisk_possessionflags;
int Objet_Flottant_OF_Reward_univVars_Perso_ID_equipement;
int Objet_Flottant_OF_Reward_univVars_objet_status;
int Objet_Flottant_OF_Reward_univVars_objet_inventaire;
int Objet_Flottant_OF_Reward_univVars_Flag_speciaux_sauve;
int Objet_Flottant_OF_Reward_GST_Global_gao_i_Perso_ID_a_checker;
int Objet_Flottant_OF_Reward_univVars_Perso_ID_Energie_courante;
int Objet_Flottant_OF_Reward_univVars_Perso_ID_Energie_max;

//Objet_Flottant_OF_exec_FX_Smoke
int Objet_Flottant_OF_exec_FX_Smoke_SFX_explosion_gao;

//Objet_Flottant_OF_exec_GFX_Fire
int Objet_Flottant_OF_exec_GFX_Fire_SFX_explosion_gao;

//Objet_Flottant_OF_exec_GFX_Smoke
int Objet_Flottant_OF_exec_GFX_Smoke_SFX_explosion_gao;

//Objet_Flottant_OF_mode_ground
int Objet_Flottant_OF_mode_ground_univVars;
int Objet_Flottant_OF_mode_ground_univVars_Flag_speciaux_sauve;
int fct_Objet_Flottant_OF_mode_ground_Objet_Flottant_OF_destroy;
int fct_Objet_Flottant_OF_mode_ground_Objet_Flottant_OF_Message_Read;

//Objet_Flottant_OF_comportement
int Objet_Flottant_OF_comportement_univVars;
int Objet_Flottant_OF_comportement_univVars_Flag_speciaux_sauve;
int fct_Objet_Flottant_OF_comportement_Objet_Flottant_OF_destroy;
int fct_Objet_Flottant_OF_comportement_Objet_Flottant_OF_Message_Read;
int Objet_Flottant_OF_comportement__PJ_Sally_gao;
int Objet_Flottant_OF_comportement_univVars_Perso_ID_actor;
int fct_Objet_Flottant_OF_comportement__PJ_Camera_gao;

//Objet_Flottant_OF_afterrec
int fct_Objet_Flottant_OF_afterrec_Objet_Flottant_OF_exec_GFX_Fire;
int fct_Objet_Flottant_OF_afterrec_Objet_Flottant_OF_exec_GFX_Smoke;
int Objet_Flottant_OF_afterrec_univVars;
int Objet_Flottant_OF_afterrec_univVars_Special_vectors;

//Objet_Flottant_OF_Message_Read
int fctvar_Objet_Flottant_OF_Message_Read_DUP_HovercraftShoot_gao;
int Objet_Flottant_OF_Message_Read_GST_Global_gao;
int Objet_Flottant_OF_Message_Read_GST_Global_gao_o_targets;
int Objet_Flottant_OF_Message_Read_GST_Global_gao_i_targets_nb;
int Objet_Flottant_OF_Message_Read_GST_Global_gao_v_targets_offset;
int Objet_Flottant_OF_Message_Read_GST_Global_gao_o_selected_target;
int Objet_Flottant_OF_Message_Read_GST_Global_gao_i_selected_target_nb;
int Objet_Flottant_OF_Message_Read_GST_Global_gao_v_selected_target_offset;
int Objet_Flottant_OF_Message_Read__PJ_Sally_gao;
int Objet_Flottant_OF_Message_Read_INV_OBJ15_Starkos_Vehicule_gao;
int Objet_Flottant_OF_Message_Read_INV_OBJ37_HoverBoost_gao;
int Objet_Flottant_OF_Message_Read_INV_OBJ45_Perle_gao;
int Objet_Flottant_OF_Message_Read_univVars;
int Objet_Flottant_OF_Message_Read_univVars_Perso_ID_actor;
int Objet_Flottant_OF_Message_Read_univVars_Perso_ID_Energie_courante;
int Objet_Flottant_OF_Message_Read_univVars_Perso_ID_Energie_max;
int Objet_Flottant_OF_Message_Read_univVars_objet_inventaire;
int Objet_Flottant_OF_Message_Read_INV_OBJ45_PerleHalo_gao;
int Objet_Flottant_OF_Message_Read_univVars_objet_liste_gao;
int Objet_Flottant_OF_Message_Read_univVars_Flag_speciaux_sauve;
int Objet_Flottant_OF_Message_Read_Light_PAF_gao;
int Objet_Flottant_OF_Message_Read_SFX_explosion_gao;
int fct_Objet_Flottant_OF_Message_Read_Objet_Flottant_OF_exec_FX_Smoke;
int Objet_Flottant_OF_Message_Read_SFX_light_smoke_gao;
int fct_Objet_Flottant_OF_Message_Read_Objet_Flottant_OF_Reward;
int fct_Objet_Flottant_OF_Message_Read_Objet_Flottant_OF_NoReward;

// =============================================

//caisse_gene_init
int caisse_gene_init_univVars;
int caisse_gene_init_univVars_Flag_speciaux_sauve;
int fct_caisse_gene_init_caisse_gene_loop;

int Objet_Flottant_OF_destroy()
{
    int result; // eax
    tScriptStruct_ObjetFlottant* v1; // [esp+4h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ObjetFlottant*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (*AI_gppst_CurrentJumpNode == (tAINode*)4)
    {
        result = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (result == 1000)
            result = 1000;
    }
    else
    {
        if (v1->GFX_Airtrace != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v1->GFX_Airtrace);
            v1->GFX_Airtrace = -1;
        }
        if (v1->GFX_Halo != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v1->GFX_Halo);
            v1->GFX_Halo = -1;
        }
        if (v1->GFX_Fire != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v1->GFX_Fire);
            v1->GFX_Fire = -1;
        }
        if (v1->GFX_Smoke != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v1->GFX_Smoke);
            v1->GFX_Smoke = -1;
        }
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        *AI_gppst_CurrentJumpNode = (tAINode*)4;
        TIME_Wait_C(0.1f);
        result = 0;
    }
    return result;
}

int Objet_Flottant_OF_init()
{
    int result; // eax
    double v1; // st7
    tScriptStruct_ObjetFlottant* v2; // [esp+0h] [ebp-174h]
    tGameObjectMain* v3; // [esp+8h] [ebp-16Ch]
    float v4; // [esp+14h] [ebp-160h]
    tGameObjectMain* v5; // [esp+48h] [ebp-12Ch]
    tGameObjectMain* v6; // [esp+4Ch] [ebp-128h]
    int v7; // [esp+6Ch] [ebp-108h]
    Vec3 v8; // [esp+70h] [ebp-104h] BYREF
    unsigned int v9; // [esp+90h] [ebp-E4h]
    Vec3 a1; // [esp+94h] [ebp-E0h] BYREF
    int v11; // [esp+B4h] [ebp-C0h]
    BOOL v12; // [esp+B8h] [ebp-BCh]
    tGameObjectMain* v13; // [esp+C8h] [ebp-ACh]
    Vec3 a2; // [esp+CCh] [ebp-A8h] BYREF
    Vec3 a3; // [esp+D8h] [ebp-9Ch] BYREF
    Vec3 dest; // [esp+F4h] [ebp-80h] BYREF
    int v18; // [esp+100h] [ebp-74h]
    Vec3 v20; // [esp+10Ch] [ebp-68h] BYREF
    int v21; // [esp+118h] [ebp-5Ch]
    BOOL v22; // [esp+11Ch] [ebp-58h]
    int v23; // [esp+128h] [ebp-4Ch]
    Vec3 v24; // [esp+12Ch] [ebp-48h] BYREF
    tScriptStruct_ObjetFlottant* v25; // [esp+138h] [ebp-3Ch]
    int v26; // [esp+13Ch] [ebp-38h]
    int v27; // [esp+14Ch] [ebp-28h]
    tGameObjectMain* v28; // [esp+150h] [ebp-24h]
    Vec3 v29; // [esp+158h] [ebp-1Ch] BYREF
    tGameObjectMain* v30; // [esp+170h] [ebp-4h]

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ObjetFlottant*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v25 = v2;
    v18 = 0;
    v26 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Objet_Flottant_OF_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_init_univVars_Flag_speciaux_sauve,
            (void*)Objet_Flottant_OF_init_univVars,
            4488);
        Objet_Flottant_OF_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_init_GST_Global_gao_o_targets,
            (void*)Objet_Flottant_OF_init_GST_Global_gao,
            964);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_init_GST_Global_gao_i_targets_nb,
            (void*)Objet_Flottant_OF_init_GST_Global_gao,
            2572);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_init_GST_Global_gao_v_targets_offset,
            (void*)Objet_Flottant_OF_init_GST_Global_gao,
            1368);
        AI2C_ResolveBuffer(&Objet_Flottant_OF_init_univVars_Special_vectors, (void*)Objet_Flottant_OF_init_univVars, 5260);
        fct_Objet_Flottant_OF_init_Objet_Flottant_OF_afterrec = (int)AI_ul_RealResolveOneRefFunction(0x72001190u);
        fct_Objet_Flottant_OF_init_Objet_Flottant_OF_mode_ground = (int)AI_ul_RealResolveOneRefFunction(0x72005EA7u);
        fct_Objet_Flottant_OF_init_Objet_Flottant_OF_comportement = (int)AI_ul_RealResolveOneRefFunction(0x72000E6Du);
    }
    if (v25->des_SF_destruction)
    {
        v26 = v25->des_SF_destruction > 0
            && AI_EvalFunc_ARRBitTest_C(
                (int*)(Objet_Flottant_OF_init_univVars_Flag_speciaux_sauve + 4),
                v25->des_SF_destruction);
        if (v26)
        {
            v23 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v23 != 1000)
                return v23;
        }
    }
    AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
    AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
    AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
    AI_EvalFunc_OBJ_BoneDisplayOrder_C(AI_gpst_CurrentGameObject, -1, 1);
    if (!v25->des_i_sf_je_suis_la)
        goto LABEL_24;
    if (v25->des_i_sf_je_suis_la <= 0)
    {
        v26 = 0;
    }
    else if (v25->des_i_sf_je_suis_la < 10000)
    {
        v26 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Objet_Flottant_OF_init_univVars_Flag_speciaux_sauve + 4),
            v25->des_i_sf_je_suis_la);
    }
    else
    {
        v22 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Objet_Flottant_OF_init_univVars_Flag_speciaux_sauve + 4),
            v25->des_i_sf_je_suis_la - 10000);
        v26 = 1 - v22;
    }
    if (v26)
        goto LABEL_24;
    if (v25->des_wait_until_sf)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 4;
    }
    v7 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
    if (v7 != 1000)
        return v7;
LABEL_24:
    if (AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 0x20000))
    {
        v25->SV_objet_position_ID = -1;
        v25->des_SF_mort_ID = 0;
    }
    if (v25->des_SF_mort_ID <= 0)
        v26 = 0;
    else
        v26 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Objet_Flottant_OF_init_univVars_Flag_speciaux_sauve + 4),
            v25->des_SF_mort_ID);
    if (v26)
    {
        v9 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1;
    }
    else
    {
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
        AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0xFFu, 0);
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, -1, 0);
        AI_EvalFunc_COLCrossableSet_C(AI_gpst_CurrentGameObject, 0, 0x80);
        if (v25->des_i_settings)
        {
            v21 = AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 8, 0);
            v28 = AI_gpst_CurrentGameObject;
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
            v18 = AI_EvalFunc_ARRObjSearch_C(
                (tGameObjectMain**)(Objet_Flottant_OF_init_GST_Global_gao_o_targets + 4),
                *(uint32_t*)Objet_Flottant_OF_init_GST_Global_gao_i_targets_nb,
                v5);
            if (v18 == -1)
            {
                v18 = AI_EvalFunc_ARRObjSearch_C(
                    (tGameObjectMain**)(Objet_Flottant_OF_init_GST_Global_gao_o_targets + 4),
                    100,
                    0);
                if (v18 != -1)
                {
                    ++* (uint32_t*)Objet_Flottant_OF_init_GST_Global_gao_i_targets_nb;
                    *(uint32_t*)(Objet_Flottant_OF_init_GST_Global_gao_o_targets + 4 * v18 + 4) = (uint32_t)AI_gpst_CurrentGameObject;
                    *(uint32_t*)AI_EvalVarArray_C3((char *)Objet_Flottant_OF_init_GST_Global_gao_v_targets_offset, v18) = 0;
                    *(uint32_t*)(AI_EvalVarArray_C3((char*)Objet_Flottant_OF_init_GST_Global_gao_v_targets_offset, v18) + 4) = 0;
                    *(uint32_t*)(AI_EvalVarArray_C3((char*)Objet_Flottant_OF_init_GST_Global_gao_v_targets_offset, v18) + 8) = 0;
                }
            }
            switch (v25->des_i_settings)
            {
            case 1:
                v25->des_mobilite = 0;
                v25->des_rotation = 1;
                v25->des_f_base_gravity = -80.0;
                v25->des_f_perso_gravity = -20.0;
                v25->des_f_aircraft_gravity = -40.0;
                v25->des_v_water_friction.x = 3.0;
                v25->des_v_water_friction.y = 3.0;
                v25->des_v_water_friction.z = 6.0;
                v25->des_v_ground_friction.x = 4.0;
                v25->des_v_ground_friction.y = 4.0;
                v25->des_v_ground_friction.z = 0.0;
                v25->des_f_rebound_coef = 0.2f;
                v25->des_f_dist_centre_grav = 40.0;
                v25->des_f_water_gravity = -20.0;
                v25->des_f_water_friction = 0.89999998f;
                v25->des_f_air_gravity = -5.0;
                v25->des_f_air_friction = 0.5;
                v25->des_f_coef_banking_perso = 0.050000001f;
                v25->des_f_coef_banking_aircraft = 0.1f;
                v25->des_mode = 1;
                v25->des_resistance = 4;
                v25->des_energie = 0;
                v25->des_i_explosion_type = 20;
                v25->des_i_explosion_size = 8;
                v25->des_i_GFX_Smoke = 0;
                v25->i_expulse_special = 0;
                break;
            case 2:
                v25->des_mobilite = 0;
                v25->des_rotation = 1;
                v25->des_f_base_gravity = -80.0;
                v25->des_f_perso_gravity = -20.0;
                v25->des_f_aircraft_gravity = -40.0;
                v25->des_v_water_friction.x = 3.0;
                v25->des_v_water_friction.y = 3.0;
                v25->des_v_water_friction.z = 6.0;
                v25->des_v_ground_friction.x = 4.0;
                v25->des_v_ground_friction.y = 4.0;
                v25->des_v_ground_friction.z = 0.0;
                v25->des_f_rebound_coef = 0.2f;
                v25->des_f_dist_centre_grav = 40.0;
                v25->des_f_water_gravity = -20.0;
                v25->des_f_water_friction = 0.89999998f;
                v25->des_f_air_gravity = -5.0;
                v25->des_f_air_friction = 0.5;
                v25->des_f_coef_banking_perso = 0.050000001f;
                v25->des_f_coef_banking_aircraft = 0.1f;
                v25->des_mode = 1;
                v25->des_resistance = 2;
                v25->des_energie = 6;
                v25->des_i_explosion_type = 20;
                v25->des_i_explosion_size = 8;
                v25->des_i_GFX_Smoke = 0;
                v25->i_expulse_special = 0;
                break;
            case 3:
                v25->des_mobilite = 0;
                v25->des_rotation = 0;
                v25->i_optim_VF = 1;
                v25->des_f_base_gravity = 0.0;
                v25->des_f_perso_gravity = 0.0;
                v25->des_f_aircraft_gravity = 0.0;
                v25->des_f_coef_banking_perso = 0.050000001f;
                v25->des_f_coef_banking_aircraft = 0.1f;
                v25->des_mode = 1;
                v25->des_resistance = 2;
                v25->des_energie = 2;
                v25->des_i_explosion_type = 20;
                v25->des_i_explosion_size = 8;
                v25->des_i_GFX_Smoke = 1;
                v25->i_expulse_special = 1;
                v25->des_f_air_friction = 0.99000001f;
                break;
            case 4:
                v25->des_mobilite = 0;
                v25->des_rotation = 0;
                v25->i_optim_VF = 1;
                v25->des_f_base_gravity = -80.0;
                v25->des_f_perso_gravity = 0.0;
                v25->des_f_aircraft_gravity = 0.0;
                v25->des_f_coef_banking_perso = 0.050000001f;
                v25->des_f_coef_banking_aircraft = 0.1f;
                v25->des_mode = 1;
                v25->des_resistance = 2;
                v25->des_energie = 2;
                v25->des_i_explosion_type = 20;
                v25->des_i_explosion_size = 8;
                v25->des_i_GFX_Smoke = 1;
                v25->i_expulse_special = 1;
                v25->des_f_air_friction = 0.99000001f;
                break;
            case 5:
                v25->des_mobilite = 0;
                v25->des_rotation = 0;
                v25->i_optim_VF = 1;
                v25->des_f_base_gravity = 0.0;
                v25->des_f_perso_gravity = 0.0;
                v25->des_f_aircraft_gravity = 0.0;
                v25->des_f_coef_banking_perso = 0.050000001f;
                v25->des_f_coef_banking_aircraft = 0.1f;
                v25->des_mode = 1;
                v25->des_resistance = 2;
                v25->des_energie = 6;
                v25->des_i_explosion_type = 20;
                v25->des_i_explosion_size = 8;
                v25->des_i_GFX_Smoke = 1;
                v25->i_expulse_special = 1;
                v25->des_f_air_friction = 0.99000001f;
                break;
            }
        }
        else
        {
            v13 = AI_gpst_CurrentGameObject;
        }
        AI_EvalFunc_OBJDrawMaskSet_C(AI_gpst_CurrentGameObject, 8, 0);
        v25->f_sqr_dist_max = v25->des_f_dist_max * v25->des_f_dist_max;
        v25->i_obj_sup_nb = AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)v25->des_o_obj_list, 5, 0);
        if (v25->i_obj_sup_nb == -1)
            v25->i_obj_sup_nb = 5;
        if (v25->SV_objet_position_ID > 0)
        {
            a1 = *(Vec3*)AI_EvalVarArray_C3(
                (char*)Objet_Flottant_OF_init_univVars_Special_vectors,
                v25->SV_objet_position_ID);
            if (!AI2C_VecEqNull(&a1))
            {
                AI_EvalFunc_OBJSetPos_C_CURRENT(&a1);
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v29);
                v20.x = 0.0;
                v20.y = 0.0;
                v20.z = 1.0;
                AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v29, &v20);
            }
        }
        AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x8000, 0);
        v12 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
        v11 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x3021004, 0);
        AI_EvalFunc_COLPrioritySet_C(AI_gpst_CurrentGameObject, 100);
        AI_EvalFunc_COLLODSet_C(AI_gpst_CurrentGameObject, 3, 3);
        v25->f_water_Z = -1.0e14f;
        v1 = AI_EvalFunc_COLSizeGet_C(AI_gpst_CurrentGameObject, 0);
        v25->f_zdm_rayon = (float)v1;
        AI_EvalFunc_MATHVecLocalToGlobal_C(AI_gpst_CurrentGameObject, &v25->des_v_init_speed, &v8);
        v27 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v8);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v24);
        v4 = v25->des_f_dist_centre_grav;
        a3.x = v24.x * v4;
        a3.y = v24.y * v4;
        a3.z = v24.z * v4;
        MATH_SubstractVec(&dest, &a2, &a3);
        v25->v_grav_pos = dest;
        AI_EvalFunc_MATHVecLocalToGlobal_C(AI_gpst_CurrentGameObject, &v25->des_v_grav_init_speed, &v25->v_grav_speed);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v25->v_last_pos);
        v30 = AI_gpst_CurrentGameObject;
        v3 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v3 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v3,
                2,
                (tAIFunction*)fct_Objet_Flottant_OF_init_Objet_Flottant_OF_afterrec);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_Objet_Flottant_OF_init_Objet_Flottant_OF_afterrec);
        }
        if (v25->i_expulse_special)
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Objet_Flottant_OF_init_Objet_Flottant_OF_mode_ground);
        else
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Objet_Flottant_OF_init_Objet_Flottant_OF_comportement);
        result = 1000;
    }
    return result;
}

int Objet_Flottant_OF_Reward()
{
    void* v0; // eax
    int result; // eax
    int v2; // eax
    int v3; // eax
    void* v4; // eax
    double v5; // st7
    double v6; // st7
    int* v7; // eax
    double v8; // st7
    int v9; // eax
    int v10; // eax
    int v11; // eax
    double v12; // st7
    float v13; // [esp+0h] [ebp-284h]
    float X; // [esp+4h] [ebp-280h]
    float Xa; // [esp+4h] [ebp-280h]
    float Xb; // [esp+4h] [ebp-280h]
    float Xc; // [esp+4h] [ebp-280h]
    float Xd; // [esp+4h] [ebp-280h]
    float Xe; // [esp+4h] [ebp-280h]
    float v20; // [esp+Ch] [ebp-278h]
    float v21; // [esp+10h] [ebp-274h]
    int v22; // [esp+14h] [ebp-270h]
    char* v23; // [esp+24h] [ebp-260h]
    uint32_t* v24; // [esp+28h] [ebp-25Ch]
    tAINode* v25; // [esp+2Ch] [ebp-258h]
    tScriptStruct_ObjetFlottant* v26; // [esp+30h] [ebp-254h]
    tGameObjectMain* v27; // [esp+38h] [ebp-24Ch]
    tGameObjectMain* v28; // [esp+40h] [ebp-244h]
    tGameObjectMain* v29; // [esp+44h] [ebp-240h]
    tGameObjectMain* v30; // [esp+48h] [ebp-23Ch]
    tGameObjectMain* v31; // [esp+50h] [ebp-234h]
    tGameObjectMain* v32; // [esp+58h] [ebp-22Ch]
    tGameObjectMain* v33; // [esp+5Ch] [ebp-228h]
    tGameObjectMain* v34; // [esp+60h] [ebp-224h]
    tGameObjectMain* v35; // [esp+68h] [ebp-21Ch]
    tGameObjectMain* v36; // [esp+70h] [ebp-214h]
    tGameObjectMain* v37; // [esp+78h] [ebp-20Ch]
    tGameObjectMain* v38; // [esp+80h] [ebp-204h]
    tGameObjectMain* v39; // [esp+84h] [ebp-200h]
    tGameObjectMain* v40; // [esp+88h] [ebp-1FCh]
    tGameObjectMain* v41; // [esp+90h] [ebp-1F4h]
    tGameObjectMain* v42; // [esp+94h] [ebp-1F0h]
    tGameObjectMain* v43; // [esp+98h] [ebp-1ECh]
    uint32_t* v44; // [esp+9Ch] [ebp-1E8h]
    tGameObjectMain* v45; // [esp+B4h] [ebp-1D0h]
    tGameObjectMain* v46; // [esp+B8h] [ebp-1CCh]
    tGameObjectMain* v47; // [esp+C0h] [ebp-1C4h]
    tGameObjectMain* v48; // [esp+C8h] [ebp-1BCh]
    tGameObjectMain* v49; // [esp+CCh] [ebp-1B8h]
    tGameObjectMain* v50; // [esp+D0h] [ebp-1B4h]
    tGameObjectMain* v51; // [esp+D4h] [ebp-1B0h]
    tGameObjectMain* v52; // [esp+D8h] [ebp-1ACh]
    tGameObjectMain* v53; // [esp+E0h] [ebp-1A4h]
    tGameObjectMain* v54; // [esp+E8h] [ebp-19Ch]
    tGameObjectMain* v55; // [esp+F0h] [ebp-194h]
    float v56; // [esp+F4h] [ebp-190h]
    tGameObjectMain* v57; // [esp+FCh] [ebp-188h]
    tGameObjectMain* v58; // [esp+104h] [ebp-180h]
    tGameObjectMain* v59; // [esp+10Ch] [ebp-178h]
    tGameObjectMain* v60; // [esp+110h] [ebp-174h]
    tGameObjectMain* v61; // [esp+114h] [ebp-170h]
    tGameObjectMain* v62; // [esp+11Ch] [ebp-168h]
    tGameObjectMain* v63; // [esp+124h] [ebp-160h]
    tGameObjectMain* v64; // [esp+128h] [ebp-15Ch]
    tGameObjectMain* v65; // [esp+130h] [ebp-154h]
    float v66; // [esp+140h] [ebp-144h]
    Vec3 dest; // [esp+154h] [ebp-130h] BYREF
    int v68; // [esp+160h] [ebp-124h]
    float v69; // [esp+164h] [ebp-120h]
    Vec3 v71; // [esp+16Ch] [ebp-118h] BYREF
    void* v72; // [esp+178h] [ebp-10Ch]
    int v73; // [esp+17Ch] [ebp-108h]
    Vec3 a3; // [esp+180h] [ebp-104h] BYREF
    void* Src; // [esp+190h] [ebp-F4h]
    Vec3 v76; // [esp+194h] [ebp-F0h] BYREF
    float v77; // [esp+1A0h] [ebp-E4h]
    tMessage v78; // [esp+1B0h] [ebp-D4h] BYREF
    Vec3 a4; // [esp+1E8h] [ebp-9Ch] BYREF
    float v80; // [esp+1F4h] [ebp-90h]
    int v81; // [esp+1F8h] [ebp-8Ch]
    Vec3 v82; // [esp+1FCh] [ebp-88h] BYREF
    float v83; // [esp+208h] [ebp-7Ch]
    int v84; // [esp+210h] [ebp-74h]
    float v86; // [esp+228h] [ebp-5Ch]
    Vec3 v87; // [esp+22Ch] [ebp-58h] BYREF
    float v88; // [esp+238h] [ebp-4Ch]
    Vec3 v89; // [esp+240h] [ebp-44h] BYREF
    Vec3 v90; // [esp+250h] [ebp-34h] BYREF
    tScriptStruct_ObjetFlottant* v91; // [esp+25Ch] [ebp-28h]
    float v92; // [esp+260h] [ebp-24h]
    float v93; // [esp+264h] [ebp-20h]
    float zoom; // [esp+268h] [ebp-1Ch]
    int v95; // [esp+26Ch] [ebp-18h]
    int v96; // [esp+270h] [ebp-14h]
    int v97; // [esp+274h] [ebp-10h]
    float v98; // [esp+278h] [ebp-Ch]
    float v99; // [esp+27Ch] [ebp-8h]
    float v100; // [esp+280h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v26 = (tScriptStruct_ObjetFlottant*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v26 = 0;
    v91 = v26;
    v97 = 0;
    v68 = 0;
    v69 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Objet_Flottant_OF_Reward_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Reward_univVars_objet_valeur1,
            (void*)Objet_Flottant_OF_Reward_univVars,
            4100);
        Objet_Flottant_OF_Reward_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        Objet_Flottant_OF_Reward_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        Objet_Flottant_OF_Reward__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Reward_univVars_mdisk_possessionflags,
            (void*)Objet_Flottant_OF_Reward_univVars,
            6756);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Reward_univVars_Perso_ID_equipement,
            (void*)Objet_Flottant_OF_Reward_univVars,
            40);
        AI2C_ResolveBuffer(&Objet_Flottant_OF_Reward_univVars_objet_status, (void*)Objet_Flottant_OF_Reward_univVars, 3896);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Reward_univVars_objet_inventaire,
            (void*)Objet_Flottant_OF_Reward_univVars,
            1480);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Reward_univVars_Flag_speciaux_sauve,
            (void*)Objet_Flottant_OF_Reward_univVars,
            4488);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Reward_GST_Global_gao_i_Perso_ID_a_checker,
            (void*)Objet_Flottant_OF_Reward_GST_Global_gao,
            3288);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Reward_univVars_Perso_ID_Energie_courante,
            (void*)Objet_Flottant_OF_Reward_univVars,
            1148);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Reward_univVars_Perso_ID_Energie_max,
            (void*)Objet_Flottant_OF_Reward_univVars,
            1096);
    }
    v25 = *AI_gppst_CurrentJumpNode;
    if (*AI_gppst_CurrentJumpNode == (tAINode*)6)
    {
        v3 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        v96 = v3;
        if (v3 == 1000)
            goto LABEL_59;
        return v96;
    }
    if (v25 != (tAINode*)24)
    {
        if (v25 == (tAINode*)26)
        {
            v11 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            v73 = v11;
            if (v11 != 1000)
                return v73;
            goto LABEL_210;
        }
        if (v91->i_flag_msg_send)
        {
            v65 = (tGameObjectMain*)v91->o_reward_dup_gao;
            if (v65)
            {
                if ((unsigned int)v65 <= 2)
                    v65 = AI_MainActor[(uint32_t)v65 - 1];
                v0 = (void*)AI_EvalFunc_MSGGet_C(v65);
            }
            else
            {
                v0 = (void*)AI_EvalFunc_MSGGet_C(0);
            }
            Src = v0;
            memcpy(&v78, v0, sizeof(v78));
            while (1)
            {
                if (!v78.msgID)
                {
                    if (v91->GFX_Halo != -1)
                    {
                        v86 = TIM_gf_MainClock - TIM_gf_LockTime;
                        v69 = (v86 - v91->f_time_ref) / 2.0f;
                        if (v69 <= 1.0f)
                        {
                            v69 = 1.0f - v69;
                            v56 = v69;
                            if (v69 < 0.0)
                                v56 = 0.0;
                            if (v56 > 1.0)
                                v56 = 1.0;
                            zoom = v91->f_water_Z * v56 + v91->f_water_Z * 0.5f * (1.0f - v56);
                            v55 = (tGameObjectMain*)v91->o_reward_dup_gao;
                            if (v55)
                            {
                                if ((unsigned int)v55 <= 2)
                                    v55 = AI_MainActor[(uint32_t)v55 - 1];
                                AI_EvalFunc_OBJ_ZoomSet_C(v55, zoom);
                            }
                            else
                            {
                                AI_EvalFunc_OBJ_ZoomSet_C(0, zoom);
                            }
                            v69 = v69 * 5.0f;
                            v54 = (tGameObjectMain*)v91->o_reward_dup_gao;
                            if (v54)
                            {
                                if ((unsigned int)v54 <= 2)
                                    v54 = AI_MainActor[(uint32_t)v54 - 1];
                                AI_EvalFunc_OBJGetPos_C(v54, &a3);
                            }
                            else
                            {
                                AI_EvalFunc_OBJGetPos_C(0, &a3);
                            }
                            GFXSetCurGlobal(v91->GFX_Halo);
                            AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &a3);
                            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, v69);
                        }
                        else
                        {
                            AI_EvalFunc_GFXDel_C_CURRENT(v91->GFX_Halo);
                            v91->GFX_Halo = -1;
                        }
                    }
                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                    return 4;
                }
                if (v78.msgID == 43)
                    break;
            LABEL_59:
                v57 = (tGameObjectMain*)v91->o_reward_dup_gao;
                if (v57)
                {
                    if ((unsigned int)v57 <= 2)
                        v57 = AI_MainActor[(uint32_t)v57 - 1];
                    v4 = (void*)AI_EvalFunc_MSGGet_C(v57);
                }
                else
                {
                    v4 = (void*)AI_EvalFunc_MSGGet_C(0);
                }
                v72 = v4;
                memcpy(&v78, v4, sizeof(v78));
            }
            v64 = (tGameObjectMain*)v91->o_reward_dup_gao_extra;
            if (v64)
            {
                if ((unsigned int)v64 <= 2)
                    v64 = AI_MainActor[(uint32_t)v64 - 1];
                v63 = v64;
            }
            else
            {
                v63 = 0;
            }
            if (v63)
            {
                *(uint32_t*)(Objet_Flottant_OF_Reward_univVars_objet_valeur1 + 184) = v91->des_Perle_ID;
                v78.msgID = 62;
                v78.gao = AI_gpst_CurrentGameObject;
                v78.gao2 = 0;
                v78.Int1 = 45;
                v78.Int2 = 0;
                v78.f2 = 0.0;
                v78.f1 = 0.0;
                v78.v1.x = 0.0;
                v78.v1.y = 0.0;
                v78.v1.z = 0.0;
                v78.v2.x = 0.0;
                v78.v2.y = 0.0;
                v78.v2.z = 0.0;
                AI_EvalFunc_MSGSend_C((tGameObjectMain*)Objet_Flottant_OF_Reward_GST_Global_gao, &v78);
                AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)Objet_Flottant_OF_Reward_GST_Board_panel_gao, 0, 256);
            }
            v62 = (tGameObjectMain*)v91->o_reward_dup_gao;
            if (v62)
            {
                if ((unsigned int)v62 <= 2)
                    v62 = AI_MainActor[(uint32_t)v62 - 1];
                result = AI_EvalFunc_OBJDestroy_C(v62);
            }
            else
            {
                result = AI_EvalFunc_OBJDestroy_C(0);
            }
            if (result == 1000)
            {
                v61 = (tGameObjectMain*)v91->o_reward_dup_gao_extra;
                if (v61)
                {
                    if ((unsigned int)v61 <= 2)
                        v61 = AI_MainActor[(uint32_t)v61 - 1];
                    v60 = v61;
                }
                else
                {
                    v60 = 0;
                }
                if (v60)
                {
                    v59 = (tGameObjectMain*)v91->o_reward_dup_gao_extra;
                    if (v59)
                    {
                        if ((unsigned int)v59 <= 2)
                            v59 = AI_MainActor[(uint32_t)v59 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v59, 1);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
                    }
                    v58 = (tGameObjectMain*)v91->o_reward_dup_gao_extra;
                    if (v58)
                    {
                        if ((unsigned int)v58 <= 2)
                            v58 = AI_MainActor[(uint32_t)v58 - 1];
                        v2 = AI_EvalFunc_OBJDestroy_C(v58);
                    }
                    else
                    {
                        v2 = AI_EvalFunc_OBJDestroy_C(0);
                    }
                    v81 = v2;
                    if (v2 != 1000)
                        return v81;
                    v91->o_reward_dup_gao_extra = 0;
                }
                if (v91->GFX_Halo != -1)
                {
                    AI_EvalFunc_GFXDel_C_CURRENT(v91->GFX_Halo);
                    v91->GFX_Halo = -1;
                }
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
                *AI_gppst_CurrentJumpNode = (tAINode*)6;
                TIME_Wait_C(0.1f);
                result = 0;
            }
            return result;
        }
        v53 = (tGameObjectMain*)Objet_Flottant_OF_Reward__PJ_Aircraft_gao;
        if (Objet_Flottant_OF_Reward__PJ_Aircraft_gao)
        {
            if ((unsigned int)Objet_Flottant_OF_Reward__PJ_Aircraft_gao <= 2)
                v53 = AI_MainActor[Objet_Flottant_OF_Reward__PJ_Aircraft_gao - 1];
            v5 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v53);
        }
        else
        {
            v5 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
        }
        v83 = (float)v5;
        if (v83 < 100.0)
            goto LABEL_218;
        v52 = (tGameObjectMain*)v91->o_reward_dup_gao_extra;
        if (v52)
        {
            if ((unsigned int)v52 <= 2)
                v52 = AI_MainActor[(uint32_t)v52 - 1];
            v51 = v52;
        }
        else
        {
            v51 = 0;
        }
        if (v51)
        {
        LABEL_218:
            if (AI_EvalFunc_TIMEElapsed_C(v91->f_time_ref, 1.0))
            {
                v100 = TIM_gf_dt;
                v50 = (tGameObjectMain*)v91->o_reward_dup_gao;
                if (v50)
                {
                    if ((unsigned int)v50 <= 2)
                        v50 = AI_MainActor[(uint32_t)v50 - 1];
                    v49 = v50;
                }
                else
                {
                    v49 = 0;
                }
                X = 2.0f * v100;
                AI_EvalFunc_OBJRotateGlobalZ_C(v49, X);
                if (!v91->i_flag_msg_send)
                {
                    v91->i_flag_msg_send = 1;
                    v91->f_time_ref = TIM_gf_MainClock - TIM_gf_LockTime;
                    v48 = (tGameObjectMain*)v91->o_reward_dup_gao;
                    if (v48)
                    {
                        if ((unsigned int)v48 <= 2)
                            v48 = AI_MainActor[(uint32_t)v48 - 1];
                        AI_EvalFunc_OBJ_FlagsIdentitySet_C(v48, 0x8000, 0);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagsIdentitySet_C(0, 0x8000, 0);
                    }
                    v47 = (tGameObjectMain*)v91->o_reward_dup_gao;
                    if (v47)
                    {
                        if ((unsigned int)v47 <= 2)
                            v47 = AI_MainActor[(uint32_t)v47 - 1];
                        v6 = AI_EvalFunc_OBJ_ZoomGet_C(v47);
                    }
                    else
                    {
                        v6 = AI_EvalFunc_OBJ_ZoomGet_C(0);
                    }
                    v91->f_water_Z = (float)v6;
                    v78.gao = AI_gpst_CurrentGameObject;
                    v78.gao2 = (tGameObjectMain*)v91->o_reward_dup_gao;
                    v78.msgID = 42;
                    v78.Int1 = -1;
                    v78.Int2 = 0;
                    v78.f2 = 0.0;
                    v78.f1 = 0.0;
                    v78.v1.x = 0.0;
                    v78.v1.y = 0.0;
                    v78.v1.z = 0.0;
                    v78.v2.x = 0.0;
                    v78.v2.y = 0.0;
                    v78.v2.z = 0.0;
                    AI_EvalFunc_MSGSend_C((tGameObjectMain*)Objet_Flottant_OF_Reward__PJ_Aircraft_gao, &v78);
                    v46 = (tGameObjectMain*)v91->o_reward_dup_gao_extra;
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
                        if (v91->i_id_reward < 1100)
                        {
                            if (((*(uint32_t*)(Objet_Flottant_OF_Reward_univVars_objet_status + 4 * v91->i_id_reward + 4) & 0x400000) == 0
                                || !*(uint32_t*)AI_EvalVarArray2_C(
                                    1,
                                    (char*)Objet_Flottant_OF_Reward_univVars_objet_inventaire,
                                    v91->i_id_reward,
                                    10))
                                && *(int*)AI_EvalVarArray2_C(
                                    1,
                                    (char*)Objet_Flottant_OF_Reward_univVars_objet_inventaire,
                                    v91->i_id_reward,
                                    10) < 99)
                            {
                                v24 = (uint32_t*)AI_EvalVarArray2_C(
                                    1,
                                    (char*)Objet_Flottant_OF_Reward_univVars_objet_inventaire,
                                    v91->i_id_reward,
                                    10);
                                ++* v24;
                            }
                            if (v91->i_id_reward + 1100 <= 0)
                                v97 = 0;
                            else
                                v97 = AI_EvalFunc_ARRBitTest_C(
                                    (int*)(Objet_Flottant_OF_Reward_univVars_Flag_speciaux_sauve + 4),
                                    v91->i_id_reward + 1100);
                            if (v97)
                            {
                                v68 = Objet_Flottant_OF_Reward_GST_Board_panel_gao;
                                v97 = *(uint32_t*)(Objet_Flottant_OF_Reward_univVars_objet_status + 4 * v91->i_id_reward + 4) & 0x7000000;
                                v97 >>= 24;
                                if (v97 != 7
                                    && (*(uint32_t*)(Objet_Flottant_OF_Reward_univVars_objet_status + 4 * v91->i_id_reward + 4) & 0x400) != 0)
                                {
                                    v7 = (int*)AI_EvalVarArray2_C(
                                        1,
                                        (char*)Objet_Flottant_OF_Reward_univVars_Perso_ID_equipement,
                                        v97,
                                        10);
                                    *v7 = v91->i_id_reward;
                                    *(uint32_t*)Objet_Flottant_OF_Reward_GST_Global_gao_i_Perso_ID_a_checker |= 0x400u;
                                }
                                if (v91->i_id_reward == 14)
                                {
                                    v23 = AI_EvalVarArray_C2((char*)Objet_Flottant_OF_Reward_univVars_Perso_ID_Energie_courante, 10);
                                    *(float*)v23 = *(float*)v23 + 980.0f;
                                    v44 = (uint32_t*)(Objet_Flottant_OF_Reward_univVars_objet_valeur1 + 60);
                                    v22 = (*v44 - 1000)
                                        * (*(uint32_t*)AI_EvalVarArray2_C(
                                            1,
                                            (char*)Objet_Flottant_OF_Reward_univVars_objet_inventaire,
                                            14,
                                            10)
                                            + 2);
                                    *(float*)AI_EvalVarArray_C2((char*)Objet_Flottant_OF_Reward_univVars_Perso_ID_Energie_max, 10) = (float)v22;
                                    v21 = *(float*)AI_EvalVarArray_C2(
                                        (char*)Objet_Flottant_OF_Reward_univVars_Perso_ID_Energie_courante,
                                        10);
                                    if (*(float*)AI_EvalVarArray_C2((char*)Objet_Flottant_OF_Reward_univVars_Perso_ID_Energie_max, 10) < (double)v21)
                                    {
                                        v20 = *(float*)AI_EvalVarArray_C2(
                                            (char*)Objet_Flottant_OF_Reward_univVars_Perso_ID_Energie_max,
                                            10);
                                        *(float*)AI_EvalVarArray_C2(
                                            (char*)Objet_Flottant_OF_Reward_univVars_Perso_ID_Energie_courante,
                                            10) = v20;
                                    }
                                }
                            }
                        }
                        else
                        {
                            *(uint32_t*)Objet_Flottant_OF_Reward_univVars_mdisk_possessionflags |= 1 << (v91->i_id_reward - 76);
                            *(uint32_t*)AI_EvalVarArray2_C(1, (char*)Objet_Flottant_OF_Reward_univVars_Perso_ID_equipement, 0, 10) = 27;
                            *(uint32_t*)(Objet_Flottant_OF_Reward_univVars_objet_valeur1 + 112) = v91->i_id_reward - 1100;
                        }
                    }
                }
            LABEL_210:
                v27 = (tGameObjectMain*)v91->o_reward_dup_gao;
                if (v27)
                {
                    if ((unsigned int)v27 <= 2)
                        v27 = AI_MainActor[(uint32_t)v27 - 1];
                    AI_EvalFunc_OBJGetPos_C(v27, &v82);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v82);
                }
                AI_EvalFunc_GFXSetv_C_CURRENT(v91->GFX_Halo, 21200, &v82);
                return 1000;
            }
        }
        if (!AI_EvalFunc_TIMEElapsed_C(v91->f_time_ref, 20.0))
        {
            v99 = TIM_gf_dt;
            v30 = (tGameObjectMain*)v91->o_reward_dup_gao;
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
            Xd = 2.0f * v99;
            AI_EvalFunc_OBJRotateGlobalZ_C(v29, Xd);
            v88 = TIM_gf_MainClock - TIM_gf_LockTime;
            v12 = v88 - v91->f_time_ref;
            Xe = (float)(v12 + v12);
            v66 = cosf(Xe);
            v71.x = 0.0;
            v71.y = 0.0;
            v71.z = v66;
            MATH_VecAddVec(&v89, &v91->v_reward_ref_pos, &v71);
            v28 = (tGameObjectMain*)v91->o_reward_dup_gao;
            if (v28)
            {
                if ((unsigned int)v28 <= 2)
                    v28 = AI_MainActor[(uint32_t)v28 - 1];
                AI_EvalFunc_OBJSetPos_C(v28, &v89);
            }
            else
            {
                AI_EvalFunc_OBJSetPos_C(0, &v89);
            }
            v93 = AI_EvalFunc_MATHRandFloat_C(4.0, 6.0);
            AI_EvalFunc_GFXSetf_C_CURRENT(v91->GFX_Halo, 21000, v93);
            goto LABEL_210;
        }
        v98 = TIM_gf_dt;
        v43 = (tGameObjectMain*)v91->o_reward_dup_gao;
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
        Xa = 2.0f * v98;
        AI_EvalFunc_OBJRotateGlobalZ_C(v42, Xa);
        v91->f_time_ref = TIM_gf_MainClock - TIM_gf_LockTime;
        AI_gpst_CurrentInstance->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -1.0;
    }
    if (AI_EvalKeyword_During_C())
    {
        v41 = (tGameObjectMain*)v91->o_reward_dup_gao;
        if (v41)
        {
            if ((unsigned int)v41 <= 2)
                v41 = AI_MainActor[(uint32_t)v41 - 1];
            v8 = AI_EvalFunc_OBJ_ZoomGet_C(v41);
        }
        else
        {
            v8 = AI_EvalFunc_OBJ_ZoomGet_C(0);
        }
        v77 = (float)v8;
        v40 = (tGameObjectMain*)v91->o_reward_dup_gao;
        if (v40)
        {
            if ((unsigned int)v40 <= 2)
                v40 = AI_MainActor[(uint32_t)v40 - 1];
            v39 = v40;
        }
        else
        {
            v39 = 0;
        }
        Xb = v77 * 0.94999999f;
        AI_EvalFunc_OBJ_ZoomSet_C(v39, Xb);
        v38 = (tGameObjectMain*)v91->o_reward_dup_gao;
        if (v38)
        {
            if ((unsigned int)v38 <= 2)
                v38 = AI_MainActor[(uint32_t)v38 - 1];
            AI_EvalFunc_OBJGetPos_C(v38, &v76);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v76);
        }
        v87.x = 0.0;
        v87.y = 0.0;
        v87.z = 6.0;
        MATH_SubstractVec(&dest, &v91->v_reward_ref_pos, &v87);
        v92 = TIM_gf_MainClock - TIM_gf_LockTime;
        v13 = v92 - v91->f_time_ref;
        AI_EvalFunc_MATHVecBlend_C(&v76, &dest, v13, &a4);
        v37 = (tGameObjectMain*)v91->o_reward_dup_gao;
        if (v37)
        {
            if ((unsigned int)v37 <= 2)
                v37 = AI_MainActor[(uint32_t)v37 - 1];
            AI_EvalFunc_OBJSetPos_C(v37, &a4);
        }
        else
        {
            AI_EvalFunc_OBJSetPos_C(0, &a4);
        }
        v36 = (tGameObjectMain*)v91->o_reward_dup_gao;
        if (v36)
        {
            if ((unsigned int)v36 <= 2)
                v36 = AI_MainActor[(uint32_t)v36 - 1];
            AI_EvalFunc_OBJGetPos_C(v36, &v90);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v90);
        }
        GFXSetCurGlobal(v91->GFX_Halo);
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v90);
        v80 = TIM_gf_MainClock - TIM_gf_LockTime;
        Xc = (1.0f - (v80 - v91->f_time_ref)) * 5.0f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(21000, Xc);
        *AI_gppst_CurrentJumpNode = (tAINode*)24;
        result = 0;
    }
    else
    {
        v35 = (tGameObjectMain*)v91->o_reward_dup_gao;
        if (v35)
        {
            if ((unsigned int)v35 <= 2)
                v35 = AI_MainActor[(uint32_t)v35 - 1];
            v9 = AI_EvalFunc_OBJDestroy_C(v35);
        }
        else
        {
            v9 = AI_EvalFunc_OBJDestroy_C(0);
        }
        v84 = v9;
        if (v9 != 1000)
            return v84;
        v34 = (tGameObjectMain*)v91->o_reward_dup_gao_extra;
        if (v34)
        {
            if ((unsigned int)v34 <= 2)
                v34 = AI_MainActor[(uint32_t)v34 - 1];
            v33 = v34;
        }
        else
        {
            v33 = 0;
        }
        if (v33)
        {
            v32 = (tGameObjectMain*)v91->o_reward_dup_gao_extra;
            if (v32)
            {
                if ((unsigned int)v32 <= 2)
                    v32 = AI_MainActor[(uint32_t)v32 - 1];
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(v32, 1);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
            }
            v31 = (tGameObjectMain*)v91->o_reward_dup_gao_extra;
            if (v31)
            {
                if ((unsigned int)v31 <= 2)
                    v31 = AI_MainActor[(uint32_t)v31 - 1];
                v10 = AI_EvalFunc_OBJDestroy_C(v31);
            }
            else
            {
                v10 = AI_EvalFunc_OBJDestroy_C(0);
            }
            v95 = v10;
            if (v10 != 1000)
                return v95;
            v91->o_reward_dup_gao_extra = 0;
        }
        AI_EvalFunc_GFXDel_C_CURRENT(v91->GFX_Halo);
        v91->GFX_Halo = -1;
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        *AI_gppst_CurrentJumpNode = (tAINode*)26;
        TIME_Wait_C(0.1f);
        result = 0;
    }
    return result;
}

int Objet_Flottant_OF_exec_FX_Smoke()
{
    tScriptStruct_ObjetFlottant* v1; // [esp+0h] [ebp-6Ch]
    tGameObjectMain* v2; // [esp+8h] [ebp-64h]
    Vec3 v3; // [esp+Ch] [ebp-60h] BYREF
    Vec3 v4; // [esp+18h] [ebp-54h] BYREF
    Vec3 v5; // [esp+24h] [ebp-48h] BYREF
    Vec3 v6; // [esp+34h] [ebp-38h] BYREF
    Vec3 v7; // [esp+40h] [ebp-2Ch] BYREF
    Vec3 v8; // [esp+4Ch] [ebp-20h] BYREF
    tScriptStruct_ObjetFlottant* v9; // [esp+58h] [ebp-14h]
    int a1; // [esp+5Ch] [ebp-10h]
    Vec3 v11; // [esp+60h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ObjetFlottant*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v9 = v1;
    a1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Objet_Flottant_OF_exec_FX_Smoke_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
    }
    a1 = AI_EvalFunc_GFXAdd_C_CURRENT(13);
    GFXSetCurGlobal(a1);
    AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 100);
    AI_EvalFunc_GFXSeti_C_CURGLOB(13106, 100);
    v2 = (tGameObjectMain*)Objet_Flottant_OF_exec_FX_Smoke_SFX_explosion_gao;
    if (Objet_Flottant_OF_exec_FX_Smoke_SFX_explosion_gao)
    {
        if ((unsigned int)Objet_Flottant_OF_exec_FX_Smoke_SFX_explosion_gao <= 2)
            v2 = AI_MainActor[Objet_Flottant_OF_exec_FX_Smoke_SFX_explosion_gao - 1];
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, v2, -1);
    }
    else
    {
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, 0, -1);
    }
    GFXSetCurGlobal(a1);
    AI_EvalFunc_GFXSeti_C_CURGLOB(13101, 7);
    AI_EvalFunc_GFXSeti_C_CURGLOB(13103, 0x2066B6DD);
    AI_EvalFunc_GFXSeti_C_CURGLOB(13104, 0x4066B6DD);
    AI_EvalFunc_GFXSeti_C_CURGLOB(13105, 6731485);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13005, 2.5);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13006, 3.5);
    AI_EvalFunc_GFXSeti_C_CURGLOB(13107, 0);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13000, 2.0);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13001, 4.0);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13002, 0.001f);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13003, 0.5);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13004, 1.5);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13007, 1.0);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13009, 20.0);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13010, 40.0);
    AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 0, 1);
    AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 2, 1);
    GFXSetCurGlobal(a1);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13012, 0.75);
    v8.x = -1.0;
    v8.y = -1.0;
    v8.z = -0.5;
    AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v8);
    v7.x = 1.0;
    v7.y = 1.0;
    v7.z = 1.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v7);
    v4.x = 0.029999999f;
    v4.y = 0.029999999f;
    v4.z = 0.029999999f;
    AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v4);
    v3.x = 0.0;
    v3.y = 0.0;
    v3.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(13205, &v3);
    v6.x = 0.0;
    v6.y = 0.0;
    v6.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(13206, &v6);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v11);
    AI_EvalFunc_GFXSetv_C_CURGLOB(13200, &v11);
    v5.x = 0.0;
    v5.y = 0.0;
    v5.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(13204, &v5);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.001f);
    return 1000;
}

int Objet_Flottant_OF_exec_GFX_Fire()
{
    int result; // eax
    float v1; // [esp+0h] [ebp-16Ch]
    float v2; // [esp+4h] [ebp-168h]
    float v3; // [esp+8h] [ebp-164h]
    float v4; // [esp+Ch] [ebp-160h]
    float v5; // [esp+10h] [ebp-15Ch]
    float v6; // [esp+14h] [ebp-158h]
    tScriptStruct_ObjetFlottant* v7; // [esp+18h] [ebp-154h]
    tGameObjectMain* v8; // [esp+48h] [ebp-124h]
    Vec3 a2; // [esp+4Ch] [ebp-120h] BYREF
    float v10; // [esp+58h] [ebp-114h]
    int v11; // [esp+5Ch] [ebp-110h]
    int v12; // [esp+60h] [ebp-10Ch]
    int v13; // [esp+64h] [ebp-108h]
    float v14; // [esp+68h] [ebp-104h]
    float v15; // [esp+6Ch] [ebp-100h]
    float v16; // [esp+70h] [ebp-FCh]
    Vec3 v17; // [esp+74h] [ebp-F8h] BYREF
    Vec3 a1; // [esp+80h] [ebp-ECh] BYREF
    Vec3 v19; // [esp+8Ch] [ebp-E0h] BYREF
    Vec3 v20; // [esp+98h] [ebp-D4h] BYREF
    Vec3 v21; // [esp+A4h] [ebp-C8h]
    unsigned int v22; // [esp+B0h] [ebp-BCh]
    float v23; // [esp+B4h] [ebp-B8h]
    float v24; // [esp+B8h] [ebp-B4h]
    float v25; // [esp+BCh] [ebp-B0h]
    float v26; // [esp+C0h] [ebp-ACh]
    float v27; // [esp+C4h] [ebp-A8h]
    float v28; // [esp+C8h] [ebp-A4h]
    float v29; // [esp+CCh] [ebp-A0h]
    float v30; // [esp+D0h] [ebp-9Ch]
    Vec3 v31; // [esp+D4h] [ebp-98h] BYREF
    Vec3 v32; // [esp+E0h] [ebp-8Ch] BYREF
    Vec3 v33; // [esp+ECh] [ebp-80h]
    Vec3 v34; // [esp+F8h] [ebp-74h] BYREF
    Vec3 v35; // [esp+104h] [ebp-68h] BYREF
    float v36; // [esp+110h] [ebp-5Ch]
    Vec3 v37; // [esp+114h] [ebp-58h] BYREF
    Vec3 v38; // [esp+120h] [ebp-4Ch] BYREF
    float v39; // [esp+12Ch] [ebp-40h]
    float v40; // [esp+130h] [ebp-3Ch]
    float v41; // [esp+134h] [ebp-38h]
    float v42; // [esp+138h] [ebp-34h]
    tScriptStruct_ObjetFlottant* v43; // [esp+140h] [ebp-2Ch]
    Vec3 a3; // [esp+144h] [ebp-28h] BYREF
    Vec3 v45; // [esp+150h] [ebp-1Ch] BYREF
    float v46; // [esp+15Ch] [ebp-10h]
    float v47; // [esp+160h] [ebp-Ch]
    float v48; // [esp+164h] [ebp-8h]
    float v49; // [esp+168h] [ebp-4h]

    v19.x = 1.0;
    v19.y = 0.0;
    v19.z = 0.0;
    v34.x = 1.0;
    v34.y = 0.0;
    v34.z = 0.0;
    v23 = 1.0;
    v24 = 0.0;
    v25 = 0.0;
    v14 = 1.0;
    v15 = 0.0;
    v16 = 0.0;
    v32.x = 1.0;
    v32.y = 0.0;
    v32.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_ObjetFlottant*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    v43 = v7;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Objet_Flottant_OF_exec_GFX_Fire_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
    }
    if (v43->des_i_settings == 2)
    {
        if (v43->des_energie && (v22 = AI_EvalFunc_OBJ_FlagsControlGet_C(AI_gpst_CurrentGameObject), (v22 & 0x20000) == 0))
        {
            if (v43->des_energie <= 2)
            {
                if (v43->GFX_Fire == -1)
                {
                    v43->GFX_Fire = AI_EvalFunc_GFXAdd_C_CURRENT(13);
                    GFXSetCurGlobal(v43->GFX_Fire);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 30);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13106, 0xA43B7400);
                    v8 = (tGameObjectMain*)Objet_Flottant_OF_exec_GFX_Fire_SFX_explosion_gao;
                    if (Objet_Flottant_OF_exec_GFX_Fire_SFX_explosion_gao)
                    {
                        if ((unsigned int)Objet_Flottant_OF_exec_GFX_Fire_SFX_explosion_gao <= 2)
                            v8 = AI_MainActor[Objet_Flottant_OF_exec_GFX_Fire_SFX_explosion_gao - 1];
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v43->GFX_Fire, v8, -1);
                    }
                    else
                    {
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v43->GFX_Fire, 0, -1);
                    }
                    GFXSetCurGlobal(v43->GFX_Fire);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13101, 8);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13110, 9);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13111, 10);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13112, 10);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13003, 0.15000001f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13004, 0.64999998f);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13105, 0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13000, -0.2f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13001, -0.80000001f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13002, 0.0099999998f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13007, 7.0);
                    v37.x = 0.0099999998f;
                    v37.y = 0.0099999998f;
                    v37.z = 0.0;
                    AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v37);
                    AI_EvalFunc_GFXFlagSet_C_CURRENT(v43->GFX_Fire, 0, 1);
                    AI_EvalFunc_GFXFlagSet_C_CURRENT(v43->GFX_Fire, 2, 1);
                    GFXSetCurGlobal(v43->GFX_Fire);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13012, 0.25);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.039999999f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13009, 2.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13010, 7.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13005, 0.80000001f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13006, 1.3f);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13103, 0xFF000000);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13104, 0x800047C2);
                }
                v11 = 0;
                v12 = 0;
                v13 = 0;
                v19.x = 0.0;
                v19.y = 0.0;
                v19.z = 0.0;
                GFXSetCurGlobal(v43->GFX_Fire);
                AI_EvalFunc_GFXSetv_C_CURGLOB(13204, &v19);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v31);
                a3.x = v31.x * 3.0f;
                a3.y = v31.y * 3.0f;
                a3.z = v31.z * 3.0f;
                MATH_VecAddVec(&a1, &a2, &a3);
                AI_EvalFunc_GFXSetv_C_CURGLOB(13200, &a1);
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v38);
                v45.x = v38.x * 2.5f;
                v45.y = v38.y * 2.5f;
                v45.z = v38.z * 2.5f;
                AI_EvalFunc_GFXSetv_C_CURGLOB(13207, &v45);
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v20);
                v35.x = v20.x * 2.5f;
                v35.y = v20.y * 2.5f;
                v35.z = v20.z * 2.5f;
                AI_EvalFunc_GFXSetv_C_CURGLOB(13208, &v35);
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v17);
                v46 = v17.x * 6.0f;
                v47 = v17.y * 6.0f;
                v48 = v17.z * 6.0f;
                v33.x = 1.0;
                v33.y = 1.0;
                v33.z = 1.0;
                v14 = v46 - 1.0f;
                v15 = v47 - 1.0f;
                v16 = v48 - 1.0f;
                v21.x = 1.0;
                v21.y = 1.0;
                v21.z = 1.0;
                v23 = v46 + 1.0f;
                v24 = v47 + 1.0f;
                v25 = v48 + 1.0f;
                if (v14 > (double)v23)
                    v6 = v23;
                else
                    v6 = v14;
                v29 = v6;
                if (v15 > (double)v24)
                    v5 = v24;
                else
                    v5 = v15;
                v10 = v5;
                if (v16 > (double)v25)
                    v4 = v25;
                else
                    v4 = v16;
                v30 = v4;
                v40 = v29;
                v41 = v10;
                v42 = v4;
                v34.x = v29;
                v34.y = v10;
                v34.z = v4;
                if (v14 < (double)v23)
                    v3 = v23;
                else
                    v3 = v14;
                v36 = v3;
                if (v15 < (double)v24)
                    v2 = v24;
                else
                    v2 = v15;
                v49 = v2;
                if (v16 < (double)v25)
                    v1 = v25;
                else
                    v1 = v16;
                v39 = v1;
                v26 = v36;
                v27 = v49;
                v28 = v1;
                v32.x = v36;
                v32.y = v49;
                v32.z = v1;
                AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v34);
                AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v32);
                result = 1000;
            }
            else
            {
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 1000;
            }
        }
        else
        {
            if (v43->GFX_Fire != -1)
            {
                AI_EvalFunc_GFXSeti_C_CURRENT(v43->GFX_Fire, 13106, 0);
                v43->GFX_Fire = -1;
            }
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
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

int Objet_Flottant_OF_NoReward()
{
    int result; // eax
    int v1; // [esp+8h] [ebp-8h]

    if (*AI_gppst_CurrentJumpNode)
    {
        if (*AI_gppst_CurrentJumpNode == (tAINode*)1)
        {
            v1 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v1 == 1000)
                result = 1000;
            else
                result = v1;
        }
        else
        {
            *AI_gppst_CurrentJumpNode = 0;
            TIME_Wait_C(5.0f);
            result = 0;
        }
    }
    else
    {
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        *AI_gppst_CurrentJumpNode = (tAINode*)1;
        TIME_Wait_C(0.1f);
        result = 0;
    }
    return result;
}

int Objet_Flottant_OF_exec_GFX_Smoke()
{
    int result; // eax
    float v1; // [esp+0h] [ebp-168h]
    float v2; // [esp+4h] [ebp-164h]
    float v3; // [esp+8h] [ebp-160h]
    float v4; // [esp+Ch] [ebp-15Ch]
    float v5; // [esp+10h] [ebp-158h]
    float v6; // [esp+14h] [ebp-154h]
    tScriptStruct_ObjetFlottant* v7; // [esp+18h] [ebp-150h]
    tGameObjectMain* v8; // [esp+48h] [ebp-120h]
    float v9; // [esp+4Ch] [ebp-11Ch]
    float v10; // [esp+4Ch] [ebp-11Ch]
    float v11; // [esp+60h] [ebp-108h]
    float v12; // [esp+68h] [ebp-100h]
    int v13; // [esp+6Ch] [ebp-FCh]
    Vec3 v14; // [esp+70h] [ebp-F8h] BYREF
    Vec3 a3; // [esp+7Ch] [ebp-ECh] BYREF
    Vec3 v16; // [esp+88h] [ebp-E0h] BYREF
    Vec3 v17; // [esp+94h] [ebp-D4h]
    Vec3 v18; // [esp+A0h] [ebp-C8h] BYREF
    unsigned int v19; // [esp+ACh] [ebp-BCh]
    float v20; // [esp+B0h] [ebp-B8h]
    float v21; // [esp+B4h] [ebp-B4h]
    float v22; // [esp+B8h] [ebp-B0h]
    float v23; // [esp+BCh] [ebp-ACh]
    float v24; // [esp+C0h] [ebp-A8h]
    float v25; // [esp+C4h] [ebp-A4h]
    float v26; // [esp+C8h] [ebp-A0h]
    float v27; // [esp+CCh] [ebp-9Ch]
    Vec3 a2; // [esp+D0h] [ebp-98h] BYREF
    Vec3 v29; // [esp+DCh] [ebp-8Ch] BYREF
    Vec3 v30; // [esp+E8h] [ebp-80h]
    Vec3 v31; // [esp+F4h] [ebp-74h] BYREF
    Vec3 v32; // [esp+100h] [ebp-68h] BYREF
    float v33; // [esp+10Ch] [ebp-5Ch]
    Vec3 v34; // [esp+110h] [ebp-58h] BYREF
    Vec3 a1; // [esp+11Ch] [ebp-4Ch] BYREF
    float v36; // [esp+128h] [ebp-40h]
    float v37; // [esp+12Ch] [ebp-3Ch]
    float v38; // [esp+130h] [ebp-38h]
    float v39; // [esp+134h] [ebp-34h]
    tScriptStruct_ObjetFlottant* v40; // [esp+13Ch] [ebp-2Ch]
    Vec3 v41; // [esp+140h] [ebp-28h] BYREF
    Vec3 v42; // [esp+14Ch] [ebp-1Ch] BYREF
    int v43; // [esp+158h] [ebp-10h]
    int v44; // [esp+15Ch] [ebp-Ch]
    float v45; // [esp+160h] [ebp-8h]
    float v46; // [esp+164h] [ebp-4h]

    v16.x = 1.0;
    v16.y = 0.0;
    v16.z = 0.0;
    v31.x = 1.0;
    v31.y = 0.0;
    v31.z = 0.0;
    v20 = 1.0;
    v21 = 0.0;
    v22 = 0.0;
    v29.x = 1.0;
    v29.y = 0.0;
    v29.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_ObjetFlottant*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    v40 = v7;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Objet_Flottant_OF_exec_GFX_Smoke_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
    }
    if (v40->des_i_settings == 2)
    {
        if (v40->des_energie && (v19 = AI_EvalFunc_OBJ_FlagsControlGet_C(AI_gpst_CurrentGameObject), (v19 & 0x20000) == 0))
        {
            if (v40->des_energie == 6)
            {
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 1000;
            }
            else
            {
                if (v40->GFX_Smoke == -1)
                {
                    v40->GFX_Smoke = AI_EvalFunc_GFXAdd_C_CURRENT(13);
                    GFXSetCurGlobal(v40->GFX_Smoke);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 100);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13106, 0xA43B7400);
                    v8 = (tGameObjectMain*)Objet_Flottant_OF_exec_GFX_Smoke_SFX_explosion_gao;
                    if (Objet_Flottant_OF_exec_GFX_Smoke_SFX_explosion_gao)
                    {
                        if ((unsigned int)Objet_Flottant_OF_exec_GFX_Smoke_SFX_explosion_gao <= 2)
                            v8 = AI_MainActor[Objet_Flottant_OF_exec_GFX_Smoke_SFX_explosion_gao - 1];
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v40->GFX_Smoke, v8, -1);
                    }
                    else
                    {
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v40->GFX_Smoke, 0, -1);
                    }
                    GFXSetCurGlobal(v40->GFX_Smoke);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13101, 7);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13003, 0.34999999f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13004, 0.64999998f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13000, 0.5);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13001, 1.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13002, 0.000099999997f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13007, 10.0);
                    v34.x = 0.0099999998f;
                    v34.y = 0.0099999998f;
                    v34.z = 0.0;
                    AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v34);
                    AI_EvalFunc_GFXFlagSet_C_CURRENT(v40->GFX_Smoke, 0, 1);
                    AI_EvalFunc_GFXFlagSet_C_CURRENT(v40->GFX_Smoke, 2, 1);
                    GFXSetCurGlobal(v40->GFX_Smoke);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13012, 0.25);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.039999999f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13009, 4.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13010, 10.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13005, 1.5);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13006, 2.5);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13103, 0);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13105, 0x101010);
                }
                v9 = (float)v40->des_energie;
                v10 = v9 / 5.0f;
                v13 = AI_EvalFunc_COLORBlend_C(0x80101010, 0x10101010, v10);
                GFXSetCurGlobal(v40->GFX_Smoke);
                AI_EvalFunc_GFXSeti_C_CURGLOB(13104, v13);
                v16.x = 0.0;
                v16.y = 0.0;
                v16.z = 0.0;
                AI_EvalFunc_GFXSetv_C_CURGLOB(13204, &v16);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v42);
                a3.x = v42.x * 3.0f;
                a3.y = v42.y * 3.0f;
                a3.z = v42.z * 3.0f;
                MATH_VecAddVec(&a1, &a2, &a3);
                AI_EvalFunc_GFXSetv_C_CURGLOB(13200, &a1);
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v41);
                v18.x = v41.x * 4.0f;
                v18.y = v41.y * 4.0f;
                v18.z = v41.z * 4.0f;
                AI_EvalFunc_GFXSetv_C_CURGLOB(13207, &v18);
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v32);
                v14.x = v32.x * 4.0f;
                v14.y = v32.y * 4.0f;
                v14.z = v32.z * 4.0f;
                AI_EvalFunc_GFXSetv_C_CURGLOB(13208, &v14);
                v43 = 0;
                v44 = 0;
                v45 = 10.0;
                v30.x = 5.0;
                v30.y = 5.0;
                v30.z = 5.0;
                v11 = 0.0 - 5.0;
                v12 = 10.0 - 5.0;
                v17.x = 5.0;
                v17.y = 5.0;
                v17.z = 5.0;
                v20 = 0.0 + 5.0;
                v21 = v20;
                v22 = 10.0 + 5.0;
                if (v11 > (double)v20)
                    v6 = v20;
                else
                    v6 = 0.0 - 5.0;
                v26 = v6;
                if (v11 > (double)v21)
                    v5 = v21;
                else
                    v5 = 0.0 - 5.0;
                if (v12 > (double)v22)
                    v4 = v22;
                else
                    v4 = 10.0 - 5.0;
                v27 = v4;
                v37 = v26;
                v38 = v5;
                v39 = v4;
                v31.x = v26;
                v31.y = v5;
                v31.z = v4;
                if (v11 < (double)v20)
                    v3 = v20;
                else
                    v3 = 0.0 - 5.0;
                v33 = v3;
                if (v11 < (double)v21)
                    v2 = v21;
                else
                    v2 = 0.0 - 5.0;
                v46 = v2;
                if (v12 < (double)v22)
                    v1 = v22;
                else
                    v1 = 10.0 - 5.0;
                v36 = v1;
                v23 = v33;
                v24 = v46;
                v25 = v1;
                v29.x = v33;
                v29.y = v46;
                v29.z = v1;
                AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v31);
                AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v29);
                result = 1000;
            }
        }
        else
        {
            if (v40->GFX_Smoke != -1)
            {
                AI_EvalFunc_GFXSeti_C_CURRENT(v40->GFX_Smoke, 13106, 0);
                v40->GFX_Smoke = -1;
            }
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
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

int Objet_Flottant_OF_mode_ground()
{
    int result; // eax
    unsigned int v1; // eax
    double v2; // st7
    double v3; // st7
    float v4; // [esp+8h] [ebp-30Ch]
    tScriptStruct_ObjetFlottant* v5; // [esp+Ch] [ebp-308h]
    Vec3* v6; // [esp+1Ch] [ebp-2F8h]
    Vec3* v7; // [esp+20h] [ebp-2F4h]
    float v8; // [esp+30h] [ebp-2E4h]
    tGameObjectMain* v9; // [esp+40h] [ebp-2D4h]
    tGameObjectMain* v10; // [esp+50h] [ebp-2C4h]
    float v11; // [esp+5Ch] [ebp-2B8h]
    tGameObjectMain* v12; // [esp+64h] [ebp-2B0h]
    float v13; // [esp+68h] [ebp-2ACh]
    float v14; // [esp+6Ch] [ebp-2A8h]
    tGameObjectMain* v15; // [esp+74h] [ebp-2A0h]
    tGameObjectMain* v16; // [esp+78h] [ebp-29Ch]
    tGameObjectMain* v17; // [esp+7Ch] [ebp-298h]
    Vec3 v18; // [esp+84h] [ebp-290h] BYREF
    Vec3 v19; // [esp+A8h] [ebp-26Ch] BYREF
    int v20; // [esp+B4h] [ebp-260h]
    int v21; // [esp+B8h] [ebp-25Ch]
    float v22; // [esp+BCh] [ebp-258h]
    Vec3 v23; // [esp+C0h] [ebp-254h] BYREF
    Vec3 v24; // [esp+CCh] [ebp-248h] BYREF
    Vec3 v25; // [esp+D8h] [ebp-23Ch] BYREF
    int v26; // [esp+E4h] [ebp-230h]
    BOOL v27; // [esp+E8h] [ebp-22Ch]
    float v28; // [esp+ECh] [ebp-228h]
    BOOL v29; // [esp+F0h] [ebp-224h]
    tMessage Src; // [esp+F4h] [ebp-220h] BYREF
    unsigned int v31; // [esp+128h] [ebp-1ECh]
    Vec3 v32; // [esp+12Ch] [ebp-1E8h] BYREF
    float v33; // [esp+138h] [ebp-1DCh]
    float v34; // [esp+13Ch] [ebp-1D8h]
    float v35; // [esp+140h] [ebp-1D4h]
    Vec3 v36; // [esp+144h] [ebp-1D0h] BYREF
    float v37; // [esp+150h] [ebp-1C4h]
    tGameObjectMain* v38; // [esp+154h] [ebp-1C0h]
    Vec3 a2; // [esp+170h] [ebp-1A4h] BYREF
    Vec3 v40; // [esp+17Ch] [ebp-198h] BYREF
    BOOL v41; // [esp+188h] [ebp-18Ch]
    tGameObjectMain* v42; // [esp+18Ch] [ebp-188h]
    float v43; // [esp+190h] [ebp-184h]
    Vec3 v44; // [esp+194h] [ebp-180h] BYREF
    float v45; // [esp+1A0h] [ebp-174h]
    float v46; // [esp+1A4h] [ebp-170h]
    float v47; // [esp+1A8h] [ebp-16Ch]
    float v48; // [esp+1B4h] [ebp-160h]
    unsigned int v49; // [esp+1B8h] [ebp-15Ch]
    Vec3 v50; // [esp+1C8h] [ebp-14Ch] BYREF
    int v51; // [esp+1D4h] [ebp-140h]
    BOOL v52; // [esp+1D8h] [ebp-13Ch]
    Vec3 v54; // [esp+1E4h] [ebp-130h] BYREF
    Vec3 v55; // [esp+1F0h] [ebp-124h] BYREF
    int v56; // [esp+200h] [ebp-114h]
    Vec3 v57; // [esp+204h] [ebp-110h] BYREF
    Vec3 v58; // [esp+21Ch] [ebp-F8h] BYREF
    Vec3 v59; // [esp+228h] [ebp-ECh]
    int v60; // [esp+234h] [ebp-E0h]
    float v61; // [esp+238h] [ebp-DCh]
    BOOL v63; // [esp+244h] [ebp-D0h]
    int v64; // [esp+248h] [ebp-CCh]
    Vec3 v65; // [esp+254h] [ebp-C0h] BYREF
    int v66; // [esp+26Ch] [ebp-A8h]
    int v67; // [esp+270h] [ebp-A4h]
    BOOL v68; // [esp+274h] [ebp-A0h]
    tGameObjectMain* v69; // [esp+278h] [ebp-9Ch]
    tGameObjectMain* v70; // [esp+27Ch] [ebp-98h]
    tGameObjectMain* v72; // [esp+284h] [ebp-90h]
    float v73; // [esp+288h] [ebp-8Ch]
    float v74; // [esp+28Ch] [ebp-88h]
    float v75; // [esp+290h] [ebp-84h]
    Vec3 v76; // [esp+294h] [ebp-80h] BYREF
    Vec3 v77; // [esp+2A0h] [ebp-74h] BYREF
    int v78; // [esp+2ACh] [ebp-68h]
    int v79; // [esp+2B0h] [ebp-64h]
    Vec3 v80; // [esp+2B4h] [ebp-60h] BYREF
    tScriptStruct_ObjetFlottant* v81; // [esp+2C0h] [ebp-54h]
    int v82; // [esp+2C4h] [ebp-50h]
    Vec3 v83; // [esp+2C8h] [ebp-4Ch] BYREF
    Vec3 dest; // [esp+2D4h] [ebp-40h] BYREF
    float v85; // [esp+2E0h] [ebp-34h]
    int v86; // [esp+2E4h] [ebp-30h]
    int v87; // [esp+2E8h] [ebp-2Ch]
    int v88; // [esp+2ECh] [ebp-28h]
    Vec3 a3; // [esp+2F0h] [ebp-24h] BYREF
    BOOL v90; // [esp+308h] [ebp-Ch]
    float v91; // [esp+310h] [ebp-4h]

    v65.x = 1.0;
    v65.y = 0.0;
    v65.z = 0.0;
    v45 = 1.0;
    v46 = 0.0;
    v47 = 0.0;
    v36.x = 1.0;
    v36.y = 0.0;
    v36.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_ObjetFlottant*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v81 = v5;
    v67 = 0;
    v28 = 0.0;
    v60 = 0;
    v69 = 0;
    v38 = 0;
    v63 = 0;
    v70 = 0;
    v82 = 0;
    v72 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Objet_Flottant_OF_mode_ground_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_mode_ground_univVars_Flag_speciaux_sauve,
            (void*)Objet_Flottant_OF_mode_ground_univVars,
            4488);
        fct_Objet_Flottant_OF_mode_ground_Objet_Flottant_OF_destroy = (int)AI_ul_RealResolveOneRefFunction(0x72006309u);
        fct_Objet_Flottant_OF_mode_ground_Objet_Flottant_OF_Message_Read = (int)AI_ul_RealResolveOneRefFunction(0x26003700u);
    }
    if (v81->des_SF_destruction)
    {
        v63 = v81->des_SF_destruction > 0
            && AI_EvalFunc_ARRBitTest_C(
                (int*)(Objet_Flottant_OF_mode_ground_univVars_Flag_speciaux_sauve + 4),
                v81->des_SF_destruction);
        if (v63)
        {
            v79 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Objet_Flottant_OF_mode_ground_Objet_Flottant_OF_destroy);
            if (v79 != 1000)
                return v79;
        }
    }
    if (v81->des_i_settings == 5)
    {
        v38 = AI_EvalFunc_OBJ_HierarchyGet_C(AI_gpst_CurrentGameObject);
        v17 = v38;
        if (v38)
        {
            if ((unsigned int)v38 <= 2)
                v17 = AI_MainActor[(uint32_t)v38 - 1];
            v16 = v17;
        }
        else
        {
            v16 = 0;
        }
        if (v16)
        {
            v15 = v38;
            if (v38)
            {
                if ((unsigned int)v38 <= 2)
                    v15 = AI_MainActor[(uint32_t)v38 - 1];
                v1 = AI_EvalFunc_OBJ_FlagsControlGet_C(v15);
            }
            else
            {
                v1 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
            }
            v31 = v1;
            if ((v1 & 0x200000) != 0)
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
                AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
                AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0xFFu, 0);
            }
        }
        v27 = AI_EvalFunc_DYNOff_C(AI_gpst_CurrentGameObject);
        v56 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Objet_Flottant_OF_mode_ground_Objet_Flottant_OF_Message_Read);
        if (v56 == 1000)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            result = v56;
        }
    }
    else
    {
        v88 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Objet_Flottant_OF_mode_ground_Objet_Flottant_OF_Message_Read);
        if (v88 != 1000)
            return v88;
        if (!v81->i_flag_1st_ground_col)
        {
            v41 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
            v64 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 58855428, 0);
            v83.x = 0.0;
            v83.y = 0.0;
            v83.z = v81->des_f_base_gravity;
            AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v83);
            AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 1, 0);
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v44);
            v59.x = 0.0;
            v59.y = 0.0;
            v59.z = 1.0;
            v37 = 1.0f * v44.z + 0.0f * v44.y + v44.x * 0.0f;
            v4 = (float)fabs(v37);
            v91 = v4;
            v28 = v4;
            v14 = v4;
            if (v4 < 0.0)
                v14 = 0.0;
            if (v14 > 1.0)
                v14 = 1.0;
            v65.x = v81->des_v_ground_friction.x * v14 + v81->des_v_ground_friction.z * (1.0f - v14);
            v65.y = v65.x;
            v13 = v28;
            if (v28 < 0.0)
                v13 = 0.0;
            if (v13 > 1.0)
                v13 = 1.0;
            v65.z = v81->des_v_ground_friction.z * v13 + v81->des_v_ground_friction.x * (1.0f - v13);
            v51 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v65);
            v26 = AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 64);
            if (!v26 && v81->des_f_base_gravity != 0.0)
            {
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                return 4;
            }
            v81->i_flag_1st_ground_col = 1;
            v52 = AI_EvalFunc_DYNOff_C(AI_gpst_CurrentGameObject);
            AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, 1);
            v81->i_flag_LOD = 1;
            if (v81->des_f_base_gravity == 0.0)
            {
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v81->v_normale_sol);
            }
            else
            {
                AI_EvalFunc_COLNormalGet_C(AI_gpst_CurrentGameObject, 64, &v81->v_normale_sol);
                AI_EvalFunc_OBJ_BankingSet_C(AI_gpst_CurrentGameObject, &v81->v_normale_sol);
            }
        }
        v70 = AI_EvalFunc_VIEWGetObject_C(0);
        v69 = *AI_MainActor;
        v82 = 0;
        if (v81->f_time_air == 0.0)
        {
            v49 = AI_EvalFunc_OBJ_FlagsControlGet_C(AI_gpst_CurrentGameObject);
            if ((v49 & 0x20000) != 0)
            {
                v82 = 1;
            }
            else
            {
                v12 = v69;
                if (v69)
                {
                    if ((unsigned int)v69 <= 2)
                        v12 = AI_MainActor[(uint32_t)v69 - 1];
                    v2 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v12);
                }
                else
                {
                    v2 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
                }
                v48 = (float)v2;
                if (v48 > 160000.0)
                    v82 = 1;
            }
        }
        else
        {
            v82 = 0;
        }
        if (v82)
        {
            if (!v81->i_flag_LOD)
            {
                v81->i_flag_LOD = 1;
                v68 = AI_EvalFunc_DYNOff_C(AI_gpst_CurrentGameObject);
                AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, 1);
            }
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 4;
        }
        else
        {
            if (v81->i_flag_LOD)
            {
                v81->i_flag_LOD = 0;
                v29 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
                v86 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 50466820, 0);
                v54.x = 0.0;
                v54.y = 0.0;
                v54.z = 0.0;
                AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v54);
                AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 1, 0);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v80);
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v55);
                v11 = v81->des_f_dist_centre_grav;
                v58.x = v55.x * v11;
                v58.y = v55.y * v11;
                v58.z = v55.z * v11;
                MATH_SubstractVec(&dest, &v80, &v58);
                v81->v_grav_pos = dest;
                AI_EvalFunc_MATHVecLocalToGlobal_C(AI_gpst_CurrentGameObject, &v81->des_v_grav_init_speed, &v81->v_grav_speed);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v81->v_last_pos);
            }
            v66 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x800000, 0);
            v24.x = 0.0;
            v24.y = 0.0;
            v24.z = 0.0;
            AI_EvalFunc_DYNStreamSet_C(AI_gpst_CurrentGameObject, &v24);
            v10 = v69;
            if (v69)
            {
                if ((unsigned int)v69 <= 2)
                    v10 = AI_MainActor[(uint32_t)v69 - 1];
                v3 = AI_EvalFunc_DYNSpeedGet_C(v10);
            }
            else
            {
                v3 = AI_EvalFunc_DYNSpeedGet_C(0);
            }
            v43 = (float)v3;
            if (v43 <= 40.0)
            {
                v81->i_flag_ejection_allowed = 0;
                v87 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0, 0x2000000);
            }
            else
            {
                v81->i_flag_ejection_allowed = 1;
                v78 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x2000000, 0);
            }
            if (v81->f_time_air != 0.0)
            {
                v67 = 0;
                if (AI_EvalFunc_TIMEElapsed_C(v81->f_time_air, 0.5))
                {
                    if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 64))
                    {
                        v67 = 1;
                    }
                    else if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 128))
                    {
                        v67 = 1;
                    }
                }
                if (!v67)
                {
                    AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&a2);
                    if (a2.z < 0.0)
                    {
                        v60 = AI_EvalFunc_COLGMatReportGet_C(AI_gpst_CurrentGameObject, 16);
                        if (v60 == -1)
                        {
                            AI_EvalFunc_OBJGetPos_C_CURRENT(&v77);
                            AI_EvalFunc_WTR_GetDif_C(&v77, &v32);
                            v36 = v32;
                            AI_EvalFunc_OBJGetPos_C_CURRENT(&v76);
                            if (v76.z < (double)v36.z)
                            {
                                v67 = 1;
                                AI_EvalFunc_OBJSetPos_C_CURRENT(&v36);
                            }
                        }
                        else
                        {
                            v67 = 1;
                            AI_EvalFunc_COLPointGet_C(AI_gpst_CurrentGameObject, v60 + 0x80000000, &a3);
                            AI_EvalFunc_OBJSetPos_C_CURRENT(&a3);
                        }
                    }
                }
                if (v67)
                {
                    v90 = AI_EvalFunc_DYNOff_C(AI_gpst_CurrentGameObject);
                    v42 = AI_gpst_CurrentGameObject;
                    v72 = AI_gpst_CurrentGameObject;
                    Src.msgID = 37;
                    Src.gao = AI_gpst_CurrentGameObject;
                    Src.gao2 = 0;
                    Src.Int1 = 103;
                    Src.Int2 = 0;
                    Src.v1.x = 0.0;
                    Src.v1.y = 0.0;
                    Src.v1.z = 0.0;
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&Src.v2);
                    Src.f2 = 1.0e14f;
                    Src.f1 = 1000.0;
                    v9 = v72;
                    if (v72)
                    {
                        if ((unsigned int)v72 <= 2)
                            v9 = AI_MainActor[(uint32_t)v72 - 1];
                        AI_EvalFunc_MSGSend_C(v9, &Src);
                    }
                    else
                    {
                        AI_EvalFunc_MSGSend_C(0, &Src);
                    }
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v18);
                    v81->v_grav_pos.z = v18.z - v81->v_last_pos.z + v81->v_grav_pos.z;
                    v36 = v81->v_grav_pos;
                    v8 = v81->des_f_air_friction;
                    v73 = v8 * v81->v_grav_speed.x;
                    v74 = v81->v_grav_speed.y * v8;
                    v75 = v81->v_grav_speed.z * v8;
                    v45 = v73;
                    v46 = v74;
                    v47 = v75;
                    v20 = 0;
                    v21 = 0;
                    v22 = v81->des_f_air_gravity;
                    v45 = v73 + 0.0f;
                    v46 = v74 + 0.0f;
                    v47 = v75 + v22;
                    v61 = TIM_gf_dt;
                    v33 = v45 * TIM_gf_dt;
                    v34 = v46 * TIM_gf_dt;
                    v35 = v47 * TIM_gf_dt;
                    v7 = &v81->v_grav_pos;
                    v81->v_grav_pos.x = v33 + v81->v_grav_pos.x;
                    v7->y = v34 + v7->y;
                    v7->z = v35 + v7->z;
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v57);
                    MATH_SubstractVec(&v81->v_grav_pos, &v81->v_grav_pos, &v57);
                    AI_EvalFunc_MATHVecSetNormVector_C(&v81->v_grav_pos, v81->des_f_dist_centre_grav);
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v40);
                    v6 = &v81->v_grav_pos;
                    v81->v_grav_pos.x = v40.x + v81->v_grav_pos.x;
                    v6->y = v40.y + v6->y;
                    v6->z = v40.z + v6->z;
                    MATH_SubstractVec(&v23, &v81->v_grav_pos, &v36);
                    v85 = TIM_gf_dt;
                    MATH_VecMultNormScalar(&v25, &v23, TIM_gf_dt);
                    v81->v_grav_speed = v25;
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v19);
                    MATH_SubstractVec(&v50, &v19, &v81->v_grav_pos);
                    AI_EvalFunc_OBJ_BankingSet_C(AI_gpst_CurrentGameObject, &v50);
                }
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v81->v_last_pos);
            result = 1000;
        }
    }
    return result;
}

int Objet_Flottant_OF_comportement()
{
    int result; // eax
    double v1; // st7
    int v2; // eax
    tGameObjectMain* v3; // eax
    tGameObjectMain* v4; // eax
    tGameObjectMain* v5; // eax
    tGameObjectMain* v6; // eax
    tGameObjectMain* v7; // eax
    float* v8; // eax
    tGameObjectMain* v9; // eax
    BOOL v10; // eax
    BOOL v11; // eax
    BOOL v12; // eax
    BOOL v13; // eax
    BOOL v14; // eax
    double v15; // st7
    float v16; // [esp+0h] [ebp-C44h]
    float v17; // [esp+0h] [ebp-C44h]
    float v18; // [esp+0h] [ebp-C44h]
    float v19; // [esp+0h] [ebp-C44h]
    float v20; // [esp+0h] [ebp-C44h]
    float v21; // [esp+4h] [ebp-C40h]
    float v22; // [esp+Ch] [ebp-C38h]
    float v23; // [esp+10h] [ebp-C34h]
    float v24; // [esp+14h] [ebp-C30h]
    float v25; // [esp+18h] [ebp-C2Ch]
    float v26; // [esp+18h] [ebp-C2Ch]
    float v27; // [esp+1Ch] [ebp-C28h]
    float v28; // [esp+20h] [ebp-C24h]
    float v29; // [esp+24h] [ebp-C20h]
    float v30; // [esp+28h] [ebp-C1Ch]
    float v31; // [esp+2Ch] [ebp-C18h]
    float v32; // [esp+30h] [ebp-C14h]
    float v33; // [esp+34h] [ebp-C10h]
    float v34; // [esp+38h] [ebp-C0Ch]
    float v35; // [esp+3Ch] [ebp-C08h]
    tScriptStruct_ObjetFlottant* v36; // [esp+40h] [ebp-C04h]
    tGameObjectMain* v37; // [esp+48h] [ebp-BFCh]
    Vec3* v38; // [esp+58h] [ebp-BECh]
    float v39; // [esp+64h] [ebp-BE0h]
    float v40; // [esp+80h] [ebp-BC4h]
    float v41; // [esp+84h] [ebp-BC0h]
    float v42; // [esp+88h] [ebp-BBCh]
    Vec3* v43; // [esp+A8h] [ebp-B9Ch]
    Vec3* v44; // [esp+ACh] [ebp-B98h]
    Vec3* v45; // [esp+B0h] [ebp-B94h]
    float v46; // [esp+B8h] [ebp-B8Ch]
    float v47; // [esp+D8h] [ebp-B6Ch]
    float v48; // [esp+ECh] [ebp-B58h]
    Vec3* v49; // [esp+10Ch] [ebp-B38h]
    Vec3* v50; // [esp+110h] [ebp-B34h]
    Vec3* v51; // [esp+114h] [ebp-B30h]
    float v52; // [esp+11Ch] [ebp-B28h]
    float v53; // [esp+144h] [ebp-B00h]
    tGameObjectMain* v54; // [esp+164h] [ebp-AE0h]
    tGameObjectMain* v55; // [esp+168h] [ebp-ADCh]
    float v56; // [esp+16Ch] [ebp-AD8h]
    float v57; // [esp+170h] [ebp-AD4h]
    tGameObjectMain* v58; // [esp+178h] [ebp-ACCh]
    float v59; // [esp+184h] [ebp-AC0h]
    float v60; // [esp+188h] [ebp-ABCh]
    float v61; // [esp+194h] [ebp-AB0h]
    float v62; // [esp+198h] [ebp-AACh]
    float v63; // [esp+19Ch] [ebp-AA8h]
    tGameObjectMain* v64; // [esp+1B4h] [ebp-A90h]
    float v65; // [esp+1B8h] [ebp-A8Ch]
    float v66; // [esp+1BCh] [ebp-A88h]
    tGameObjectMain* v67; // [esp+1C4h] [ebp-A80h]
    Vec3* v68; // [esp+1C8h] [ebp-A7Ch]
    float v69; // [esp+1D4h] [ebp-A70h]
    tGameObjectMain* v70; // [esp+1DCh] [ebp-A68h]
    tGameObjectMain* v71; // [esp+1F4h] [ebp-A50h]
    tGameObjectMain* v72; // [esp+204h] [ebp-A40h]
    tGameObjectMain* v73; // [esp+218h] [ebp-A2Ch]
    tGameObjectMain* v74; // [esp+21Ch] [ebp-A28h]
    tGameObjectMain* v75; // [esp+220h] [ebp-A24h]
    tGameObjectMain* v76; // [esp+224h] [ebp-A20h]
    tGameObjectMain* v77; // [esp+228h] [ebp-A1Ch]
    tGameObjectMain* v78; // [esp+22Ch] [ebp-A18h]
    float v79; // [esp+240h] [ebp-A04h]
    float v80; // [esp+244h] [ebp-A00h]
    float v81; // [esp+248h] [ebp-9FCh]
    Vec3* v82; // [esp+26Ch] [ebp-9D8h]
    float v83; // [esp+284h] [ebp-9C0h]
    tGameObjectMain* v84; // [esp+2A4h] [ebp-9A0h]
    tGameObjectMain* v85; // [esp+2ACh] [ebp-998h]
    tGameObjectMain* v86; // [esp+2B0h] [ebp-994h]
    tGameObjectMain* v87; // [esp+2B4h] [ebp-990h]
    tGameObjectMain* v88; // [esp+2B8h] [ebp-98Ch]
    tGameObjectMain* v89; // [esp+2BCh] [ebp-988h]
    tGameObjectMain* v90; // [esp+2C4h] [ebp-980h]
    tGameObjectMain* v91; // [esp+2C8h] [ebp-97Ch]
    tGameObjectMain* v92; // [esp+2CCh] [ebp-978h]
    tGameObjectMain* v93; // [esp+2D0h] [ebp-974h]
    tGameObjectMain* v94; // [esp+2D4h] [ebp-970h]
    tGameObjectMain* v95; // [esp+2DCh] [ebp-968h]
    tGameObjectMain* v96; // [esp+2E0h] [ebp-964h]
    tGameObjectMain* v97; // [esp+2E4h] [ebp-960h]
    tGameObjectMain* v98; // [esp+2E8h] [ebp-95Ch]
    tGameObjectMain* v99; // [esp+2ECh] [ebp-958h]
    tGameObjectMain* v100; // [esp+2F4h] [ebp-950h]
    tGameObjectMain* v101; // [esp+2FCh] [ebp-948h]
    tGameObjectMain* v102; // [esp+300h] [ebp-944h]
    tGameObjectMain* v103; // [esp+304h] [ebp-940h]
    tGameObjectMain* v104; // [esp+308h] [ebp-93Ch]
    tGameObjectMain* v105; // [esp+30Ch] [ebp-938h]
    tGameObjectMain* v106; // [esp+310h] [ebp-934h]
    tGameObjectMain* v107; // [esp+318h] [ebp-92Ch]
    tGameObjectMain* v108; // [esp+320h] [ebp-924h]
    tGameObjectMain* v109; // [esp+324h] [ebp-920h]
    tGameObjectMain* v110; // [esp+328h] [ebp-91Ch]
    tGameObjectMain* v111; // [esp+32Ch] [ebp-918h]
    tGameObjectMain* v112; // [esp+330h] [ebp-914h]
    tGameObjectMain* v113; // [esp+334h] [ebp-910h]
    tGameObjectMain* v114; // [esp+33Ch] [ebp-908h]
    tGameObjectMain* v115; // [esp+344h] [ebp-900h]
    tGameObjectMain* v116; // [esp+348h] [ebp-8FCh]
    tGameObjectMain* v117; // [esp+350h] [ebp-8F4h]
    tGameObjectMain* v118; // [esp+354h] [ebp-8F0h]
    tGameObjectMain* v119; // [esp+358h] [ebp-8ECh]
    tGameObjectMain* v120; // [esp+35Ch] [ebp-8E8h]
    tGameObjectMain* v121; // [esp+360h] [ebp-8E4h]
    tGameObjectMain* v122; // [esp+368h] [ebp-8DCh]
    tGameObjectMain* v123; // [esp+36Ch] [ebp-8D8h]
    tGameObjectMain* v124; // [esp+370h] [ebp-8D4h]
    tGameObjectMain* v125; // [esp+378h] [ebp-8CCh]
    float v126; // [esp+384h] [ebp-8C0h]
    float v127; // [esp+398h] [ebp-8ACh]
    tGameObjectMain* v128; // [esp+3A0h] [ebp-8A4h]
    Vec3 v129; // [esp+3A8h] [ebp-89Ch] BYREF
    float v130; // [esp+3B4h] [ebp-890h]
    float v131; // [esp+3B8h] [ebp-88Ch]
    float v132; // [esp+3BCh] [ebp-888h]
    Vec3 v133; // [esp+3C0h] [ebp-884h] BYREF
    Vec3 v134; // [esp+3CCh] [ebp-878h] BYREF
    Vec3 v135; // [esp+3D8h] [ebp-86Ch] BYREF
    Vec3 v136; // [esp+3E4h] [ebp-860h] BYREF
    Vec3 v137; // [esp+3F0h] [ebp-854h] BYREF
    int v138; // [esp+3FCh] [ebp-848h]
    float v140; // [esp+408h] [ebp-83Ch]
    float v142; // [esp+414h] [ebp-830h]
    BOOL v143; // [esp+418h] [ebp-82Ch]
    float v144; // [esp+41Ch] [ebp-828h]
    float v145; // [esp+420h] [ebp-824h]
    Vec3 v147; // [esp+428h] [ebp-81Ch] BYREF
    Vec3 v148; // [esp+434h] [ebp-810h] BYREF
    Vec3 v149; // [esp+440h] [ebp-804h] BYREF
    Vec3 v150; // [esp+44Ch] [ebp-7F8h] BYREF
    Vec3 v151; // [esp+458h] [ebp-7ECh] BYREF
    Vec3 a5; // [esp+464h] [ebp-7E0h] BYREF
    Vec3 v153; // [esp+470h] [ebp-7D4h] BYREF
    Vec3 v154; // [esp+47Ch] [ebp-7C8h] BYREF
    int v155; // [esp+488h] [ebp-7BCh]
    int v156; // [esp+48Ch] [ebp-7B8h]
    int v157; // [esp+490h] [ebp-7B4h]
    Vec3 v158; // [esp+494h] [ebp-7B0h] BYREF
    Vec3 v159; // [esp+4A0h] [ebp-7A4h] BYREF
    Vec3 v160; // [esp+4ACh] [ebp-798h] BYREF
    Vec3 v161; // [esp+4C4h] [ebp-780h] BYREF
    Vec3 v162; // [esp+4D0h] [ebp-774h] BYREF
    Vec3 v163; // [esp+4DCh] [ebp-768h] BYREF
    int v165; // [esp+4ECh] [ebp-758h]
    int v166; // [esp+4F0h] [ebp-754h]
    float v167; // [esp+4F4h] [ebp-750h]
    float v169; // [esp+500h] [ebp-744h]
    float v170; // [esp+504h] [ebp-740h]
    Vec3 v172; // [esp+518h] [ebp-72Ch] BYREF
    Vec3 v173; // [esp+524h] [ebp-720h] BYREF
    float v174; // [esp+530h] [ebp-714h]
    float v175; // [esp+534h] [ebp-710h]
    float v176; // [esp+538h] [ebp-70Ch]
    Vec3 v177; // [esp+53Ch] [ebp-708h]
    tGameObjectMain* v178; // [esp+54Ch] [ebp-6F8h]
    float v179; // [esp+550h] [ebp-6F4h]
    float v180; // [esp+554h] [ebp-6F0h]
    float v181; // [esp+558h] [ebp-6ECh]
    int v183; // [esp+560h] [ebp-6E4h]
    Vec3 v184; // [esp+568h] [ebp-6DCh] BYREF
    Vec3 v185; // [esp+574h] [ebp-6D0h] BYREF
    Vec3 v186; // [esp+580h] [ebp-6C4h] BYREF
    int v187; // [esp+58Ch] [ebp-6B8h]
    int v188; // [esp+590h] [ebp-6B4h]
    float v189; // [esp+594h] [ebp-6B0h]
    float v190; // [esp+59Ch] [ebp-6A8h]
    int v192; // [esp+5A4h] [ebp-6A0h]
    tGameObjectMain* v193; // [esp+5A8h] [ebp-69Ch]
    tGameObjectMain* v194; // [esp+5ACh] [ebp-698h]
    float v195; // [esp+5B0h] [ebp-694h]
    Vec3 v196; // [esp+5C0h] [ebp-684h] BYREF
    Vec3 v197; // [esp+5CCh] [ebp-678h]
    Vec3 v198; // [esp+5D8h] [ebp-66Ch] BYREF
    Vec3 v199; // [esp+5E4h] [ebp-660h] BYREF
    Vec3 v200; // [esp+5F0h] [ebp-654h] BYREF
    Vec3 v201; // [esp+5FCh] [ebp-648h] BYREF
    Vec3 v202; // [esp+608h] [ebp-63Ch] BYREF
    BOOL v203; // [esp+614h] [ebp-630h]
    tGameObjectMain* v204; // [esp+61Ch] [ebp-628h]
    int v205; // [esp+620h] [ebp-624h]
    tGameObjectMain* v206; // [esp+624h] [ebp-620h]
    tGameObjectMain* v207; // [esp+628h] [ebp-61Ch]
    tGameObjectMain* v208; // [esp+62Ch] [ebp-618h]
    float v209; // [esp+630h] [ebp-614h]
    Vec3 v211; // [esp+638h] [ebp-60Ch] BYREF
    Vec3 v212; // [esp+644h] [ebp-600h] BYREF
    Vec3 v213; // [esp+65Ch] [ebp-5E8h] BYREF
    Vec3 v214; // [esp+668h] [ebp-5DCh] BYREF
    Vec3 v215; // [esp+674h] [ebp-5D0h] BYREF
    Vec3 v216; // [esp+680h] [ebp-5C4h] BYREF
    Vec3 v217; // [esp+698h] [ebp-5ACh] BYREF
    Vec3 v218; // [esp+6A4h] [ebp-5A0h] BYREF
    Vec3 v219; // [esp+6B0h] [ebp-594h] BYREF
    float v220; // [esp+6BCh] [ebp-588h]
    int v222; // [esp+6C4h] [ebp-580h]
    float v223; // [esp+6C8h] [ebp-57Ch]
    float v224; // [esp+6CCh] [ebp-578h]
    int v225; // [esp+6D0h] [ebp-574h]
    float v226; // [esp+6D4h] [ebp-570h]
    float v227; // [esp+6D8h] [ebp-56Ch]
    unsigned int v228; // [esp+6DCh] [ebp-568h]
    float v229; // [esp+6E4h] [ebp-560h]
    int v230; // [esp+6ECh] [ebp-558h]
    Vec3 v231; // [esp+6F0h] [ebp-554h] BYREF
    Vec3 v232; // [esp+6FCh] [ebp-548h] BYREF
    float v233; // [esp+708h] [ebp-53Ch]
    float v234; // [esp+70Ch] [ebp-538h]
    float v235; // [esp+710h] [ebp-534h]
    Vec3 v236; // [esp+714h] [ebp-530h] BYREF
    Vec3 v237; // [esp+720h] [ebp-524h] BYREF
    float v238; // [esp+72Ch] [ebp-518h]
    float v239; // [esp+730h] [ebp-514h]
    float v240; // [esp+734h] [ebp-510h]
    Vec3 v241; // [esp+738h] [ebp-50Ch] BYREF
    Vec3 v242; // [esp+744h] [ebp-500h] BYREF
    float v243; // [esp+75Ch] [ebp-4E8h]
    float v244; // [esp+764h] [ebp-4E0h]
    float X; // [esp+768h] [ebp-4DCh]
    BOOL v246; // [esp+76Ch] [ebp-4D8h]
    Vec3 v247; // [esp+770h] [ebp-4D4h] BYREF
    float v248; // [esp+77Ch] [ebp-4C8h]
    float v249; // [esp+780h] [ebp-4C4h]
    float v250; // [esp+784h] [ebp-4C0h]
    Vec3 v251; // [esp+788h] [ebp-4BCh] BYREF
    float v252; // [esp+794h] [ebp-4B0h]
    float v253; // [esp+798h] [ebp-4ACh]
    float v254; // [esp+79Ch] [ebp-4A8h]
    Vec3 v255; // [esp+7ACh] [ebp-498h] BYREF
    Vec3 v256; // [esp+7B8h] [ebp-48Ch] BYREF
    Vec3 v257; // [esp+7C4h] [ebp-480h]
    float v258; // [esp+7DCh] [ebp-468h]
    tGameObjectMain* v259; // [esp+7E0h] [ebp-464h]
    BOOL v260; // [esp+7E8h] [ebp-45Ch]
    float v261; // [esp+7ECh] [ebp-458h]
    Vec3 v262; // [esp+7F4h] [ebp-450h] BYREF
    Vec3 v263; // [esp+800h] [ebp-444h] BYREF
    Vec3 v264; // [esp+80Ch] [ebp-438h] BYREF
    Vec3 v265; // [esp+818h] [ebp-42Ch] BYREF
    Vec3 v266; // [esp+824h] [ebp-420h] BYREF
    Vec3 v267; // [esp+830h] [ebp-414h] BYREF
    Vec3 v268; // [esp+83Ch] [ebp-408h] BYREF
    Vec3 v269; // [esp+848h] [ebp-3FCh] BYREF
    Vec3 v270; // [esp+854h] [ebp-3F0h] BYREF
    Vec3 v271; // [esp+860h] [ebp-3E4h] BYREF
    BOOL v272; // [esp+870h] [ebp-3D4h]
    int v273; // [esp+874h] [ebp-3D0h]
    float v274; // [esp+878h] [ebp-3CCh]
    BOOL v275; // [esp+87Ch] [ebp-3C8h]
    float v276; // [esp+880h] [ebp-3C4h]
    int v277; // [esp+884h] [ebp-3C0h]
    float v278; // [esp+888h] [ebp-3BCh]
    Vec3 v279; // [esp+890h] [ebp-3B4h] BYREF
    Vec3 v280; // [esp+89Ch] [ebp-3A8h] BYREF
    Vec3 v281; // [esp+8A8h] [ebp-39Ch] BYREF
    Vec3 v282; // [esp+8B4h] [ebp-390h] BYREF
    Vec3 a2; // [esp+8C0h] [ebp-384h] BYREF
    Vec3 v284; // [esp+8CCh] [ebp-378h] BYREF
    Vec3 v285; // [esp+8D8h] [ebp-36Ch] BYREF
    Vec3 v286; // [esp+8E4h] [ebp-360h] BYREF
    Vec3 dest; // [esp+8F0h] [ebp-354h] BYREF
    Vec3 v288; // [esp+8FCh] [ebp-348h] BYREF
    Vec3 v289; // [esp+908h] [ebp-33Ch] BYREF
    float v291; // [esp+918h] [ebp-32Ch]
    float v292; // [esp+920h] [ebp-324h]
    float v293; // [esp+924h] [ebp-320h]
    float v294; // [esp+928h] [ebp-31Ch]
    int v295; // [esp+92Ch] [ebp-318h]
    float v296; // [esp+930h] [ebp-314h]
    float v297; // [esp+934h] [ebp-310h]
    int i; // [esp+938h] [ebp-30Ch]
    tGameObjectMain* v299; // [esp+93Ch] [ebp-308h]
    int v300; // [esp+940h] [ebp-304h]
    float v301; // [esp+944h] [ebp-300h]
    int v302; // [esp+948h] [ebp-2FCh]
    Vec3 v303; // [esp+94Ch] [ebp-2F8h] BYREF
    Vec3 v304; // [esp+958h] [ebp-2ECh] BYREF
    Vec3 v305; // [esp+970h] [ebp-2D4h] BYREF
    int v306; // [esp+988h] [ebp-2BCh]
    int v307; // [esp+98Ch] [ebp-2B8h]
    float v308; // [esp+990h] [ebp-2B4h]
    Vec3 v309; // [esp+994h] [ebp-2B0h] BYREF
    Vec3 v310; // [esp+9A0h] [ebp-2A4h] BYREF
    Vec3 v311; // [esp+9ACh] [ebp-298h] BYREF
    BOOL v312; // [esp+9B8h] [ebp-28Ch]
    BOOL v313; // [esp+9BCh] [ebp-288h]
    float v314; // [esp+9C0h] [ebp-284h]
    float v315; // [esp+9C4h] [ebp-280h]
    tGameObjectMain* v316; // [esp+9C8h] [ebp-27Ch]
    float v317; // [esp+9D0h] [ebp-274h]
    int v318; // [esp+9D4h] [ebp-270h]
    float v319; // [esp+9D8h] [ebp-26Ch]
    float v320; // [esp+9DCh] [ebp-268h]
    float v321; // [esp+9E0h] [ebp-264h]
    Vec3 v322; // [esp+9E4h] [ebp-260h] BYREF
    Vec3 v323; // [esp+9F0h] [ebp-254h] BYREF
    Vec3 v324; // [esp+9FCh] [ebp-248h] BYREF
    Vec3 v325; // [esp+A08h] [ebp-23Ch] BYREF
    float v326; // [esp+A14h] [ebp-230h]
    float v327; // [esp+A18h] [ebp-22Ch]
    float v328; // [esp+A1Ch] [ebp-228h]
    Vec3 v329; // [esp+A20h] [ebp-224h]
    Vec3 v330; // [esp+A2Ch] [ebp-218h] BYREF
    float v331; // [esp+A38h] [ebp-20Ch]
    BOOL v332; // [esp+A3Ch] [ebp-208h]
    tGameObjectMain* v334; // [esp+A44h] [ebp-200h]
    float v335; // [esp+A48h] [ebp-1FCh]
    BOOL v336; // [esp+A4Ch] [ebp-1F8h]
    float v337; // [esp+A50h] [ebp-1F4h]
    Vec3 v338; // [esp+A54h] [ebp-1F0h] BYREF
    Vec3 v339; // [esp+A60h] [ebp-1E4h] BYREF
    float v340; // [esp+A6Ch] [ebp-1D8h]
    float v341; // [esp+A70h] [ebp-1D4h]
    float v342; // [esp+A74h] [ebp-1D0h]
    float v343; // [esp+A84h] [ebp-1C0h]
    float v344; // [esp+A88h] [ebp-1BCh]
    float v345; // [esp+A8Ch] [ebp-1B8h]
    int v346; // [esp+A90h] [ebp-1B4h]
    int v347; // [esp+A94h] [ebp-1B0h]
    int v348; // [esp+A98h] [ebp-1ACh]
    int v349; // [esp+AA4h] [ebp-1A0h]
    float v350; // [esp+AA8h] [ebp-19Ch]
    int v351; // [esp+AACh] [ebp-198h]
    tGameObjectMain* v352; // [esp+AB0h] [ebp-194h]
    int v353; // [esp+AB8h] [ebp-18Ch]
    float v354; // [esp+ABCh] [ebp-188h]
    float v355; // [esp+AC0h] [ebp-184h]
    tGameObjectMain* v356; // [esp+AC4h] [ebp-180h]
    Vec3 v357; // [esp+AC8h] [ebp-17Ch] BYREF
    Vec3 v358; // [esp+AD4h] [ebp-170h] BYREF
    Vec3 v359; // [esp+AE0h] [ebp-164h] BYREF
    float v360; // [esp+AECh] [ebp-158h]
    float v361; // [esp+AF0h] [ebp-154h]
    float v362; // [esp+AF4h] [ebp-150h]
    float v363; // [esp+AF8h] [ebp-14Ch]
    float v364; // [esp+AFCh] [ebp-148h]
    float v365; // [esp+B00h] [ebp-144h]
    Vec3 v366; // [esp+B04h] [ebp-140h] BYREF
    Vec3 v367; // [esp+B10h] [ebp-134h] BYREF
    int v368; // [esp+B1Ch] [ebp-128h]
    int v369; // [esp+B20h] [ebp-124h]
    int v370; // [esp+B24h] [ebp-120h]
    Vec3 v371; // [esp+B28h] [ebp-11Ch]
    tScriptStruct_ObjetFlottant* v372; // [esp+B34h] [ebp-110h]
    float v375; // [esp+B40h] [ebp-104h]
    float v376; // [esp+B44h] [ebp-100h]
    float v377; // [esp+B48h] [ebp-FCh]
    float v378; // [esp+B4Ch] [ebp-F8h]
    float v379; // [esp+B54h] [ebp-F0h]
    float v380; // [esp+B58h] [ebp-ECh]
    float v381; // [esp+B60h] [ebp-E4h]
    Vec3 v382; // [esp+B64h] [ebp-E0h] BYREF
    Vec3 v383; // [esp+B70h] [ebp-D4h] BYREF
    Vec3 v384; // [esp+B7Ch] [ebp-C8h] BYREF
    Vec3 v385; // [esp+B88h] [ebp-BCh] BYREF
    Vec3 v386; // [esp+B94h] [ebp-B0h] BYREF
    Vec3 v387; // [esp+BA0h] [ebp-A4h] BYREF
    Vec3 v388; // [esp+BACh] [ebp-98h] BYREF
    Vec3 v389; // [esp+BB8h] [ebp-8Ch] BYREF
    int v390; // [esp+BC4h] [ebp-80h]
    BOOL v391; // [esp+BC8h] [ebp-7Ch]
    float v392; // [esp+BCCh] [ebp-78h]
    BOOL v393; // [esp+BD8h] [ebp-6Ch]
    tGameObjectMain* v394; // [esp+BDCh] [ebp-68h]
    unsigned int v395; // [esp+BE0h] [ebp-64h]
    Vec3 v396; // [esp+BE4h] [ebp-60h] BYREF
    Vec3 v397; // [esp+BF0h] [ebp-54h] BYREF
    Vec3 a3; // [esp+BFCh] [ebp-48h] BYREF
    float v399; // [esp+C08h] [ebp-3Ch]
    float v400; // [esp+C0Ch] [ebp-38h]
    float v401; // [esp+C10h] [ebp-34h]
    float v402; // [esp+C20h] [ebp-24h]
    BOOL v403; // [esp+C24h] [ebp-20h]
    float v404; // [esp+C28h] [ebp-1Ch]
    BOOL v405; // [esp+C2Ch] [ebp-18h]
    tGameObjectMain* v406; // [esp+C30h] [ebp-14h]
    float v407; // [esp+C38h] [ebp-Ch]
    float v408; // [esp+C3Ch] [ebp-8h]

    v185.x = 1.0;
    v185.y = 0.0;
    v185.z = 0.0;
    v214.x = 1.0;
    v214.y = 0.0;
    v214.z = 0.0;
    v281.x = 1.0;
    v281.y = 0.0;
    v281.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    v329.x = 1.0;
    v329.y = 0.0;
    v329.z = 0.0;
    v330.x = 1.0;
    v330.y = 0.0;
    v330.z = 0.0;
    v236.x = 1.0;
    v236.y = 0.0;
    v236.z = 0.0;
    v238 = 1.0;
    v239 = 0.0;
    v240 = 0.0;
    v162.x = 1.0;
    v162.y = 0.0;
    v162.z = 0.0;
    v242.x = 1.0;
    v242.y = 0.0;
    v242.z = 0.0;
    v177.x = 1.0;
    v177.y = 0.0;
    v177.z = 0.0;
    v268.x = 1.0;
    v268.y = 0.0;
    v268.z = 0.0;
    v371.x = 1.0;
    v371.y = 0.0;
    v371.z = 0.0;
    v271.x = 1.0;
    v271.y = 0.0;
    v271.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v36 = (tScriptStruct_ObjetFlottant*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v36 = 0;
    v372 = v36;
    v402 = 0.0;
    v206 = 0;
    v190 = 0.0;
    v273 = 0;
    v222 = 0;
    v274 = 0.0;
    v258 = 0.0;
    v316 = 0;
    v178 = 0;
    v225 = 0;
    v260 = 0;
    v351 = 0;
    v228 = 0;
    i = 0;
    v277 = 0;
    v354 = 0.0;
    v355 = 0.0;
    X = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Objet_Flottant_OF_comportement_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_comportement_univVars_Flag_speciaux_sauve,
            (void*)Objet_Flottant_OF_comportement_univVars,
            4488);
        fct_Objet_Flottant_OF_comportement_Objet_Flottant_OF_destroy = (int)AI_ul_RealResolveOneRefFunction(0x72006309u);
        fct_Objet_Flottant_OF_comportement_Objet_Flottant_OF_Message_Read = (int)AI_ul_RealResolveOneRefFunction(0x26003700u);
        Objet_Flottant_OF_comportement__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_comportement_univVars_Perso_ID_actor,
            (void*)Objet_Flottant_OF_comportement_univVars,
            988);
        fct_Objet_Flottant_OF_comportement__PJ_Camera_gao = AI_ul_RealResolveOneRef(42, 0x7100108Bu);
    }
    if (v372->des_SF_destruction)
    {
        v260 = v372->des_SF_destruction > 0
            && AI_EvalFunc_ARRBitTest_C(
                (int*)(Objet_Flottant_OF_comportement_univVars_Flag_speciaux_sauve + 4),
                v372->des_SF_destruction);
        if (v260)
        {
            v353 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Objet_Flottant_OF_comportement_Objet_Flottant_OF_destroy);
            if (v353 != 1000)
                return v353;
        }
    }
    v178 = AI_EvalFunc_VIEWGetObject_C(0);
    if (v372->i_optim_VF)
    {
        v277 = 1;
        v183 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Objet_Flottant_OF_comportement_Objet_Flottant_OF_Message_Read);
        if (v183 != 1000)
            return v183;
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v163);
        AI_EvalFunc_WTR_GetDif_C(&v163, &v263);
        v274 = v372->v_offset_water.z + v263.z;
        v277 = 0;
        v395 = AI_EvalFunc_OBJ_FlagsControlGet_C(AI_gpst_CurrentGameObject);
        if ((v395 & 0x20000) != 0)
        {
            v277 = 1;
        }
        else
        {
            v128 = v178;
            if (v178)
            {
                if ((unsigned int)v178 <= 2)
                    v128 = AI_MainActor[(uint32_t)v178 - 1];
                v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v128);
            }
            else
            {
                v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
            }
            v209 = (float)v1;
            if (v209 <= 160000.0)
            {
                if (v372->i_flag_real_water && v274 == -1000000000.0f)
                    v277 = 1;
            }
            else
            {
                v277 = 1;
            }
        }
    }
    if (v277)
    {
        AI_EvalFunc_MSGClear_C(AI_gpst_CurrentGameObject);
        if (!v372->i_flag_LOD)
        {
            v372->i_flag_LOD = 1;
            v312 = AI_EvalFunc_DYNOff_C(AI_gpst_CurrentGameObject);
            AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, 1);
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        if (v372->i_flag_LOD)
        {
            v372->i_flag_LOD = 0;
            v393 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
            v230 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x3021004, 0);
            AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 1, 0);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v289);
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v186);
            v127 = v372->des_f_dist_centre_grav;
            v397.x = v186.x * v127;
            v397.y = v186.y * v127;
            v397.z = v186.z * v127;
            MATH_SubstractVec(&dest, &v289, &v397);
            v372->v_grav_pos = dest;
            AI_EvalFunc_MATHVecLocalToGlobal_C(AI_gpst_CurrentGameObject, &v372->des_v_grav_init_speed, &v372->v_grav_speed);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v372->v_last_pos);
        }
        v165 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x800000, 0);
        AI_EvalFunc_DYNReboundSet_C(AI_gpst_CurrentGameObject, v372->des_f_rebound_coef);
        AI_EvalFunc_DYNApplyRectFactorSet_C(AI_gpst_CurrentGameObject, v372->des_f_ApplyRecFactor);
        v255.x = 0.0;
        v255.y = 0.0;
        v255.z = v372->des_f_base_gravity;
        AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v255);
        v324.x = 0.0;
        v324.y = 0.0;
        v324.z = 0.0;
        AI_EvalFunc_DYNStreamSet_C(AI_gpst_CurrentGameObject, &v324);
        if (v372->des_f_dist_max != 0.0)
        {
            AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &v286);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v396);
            MATH_SubstractVec(&v303, &v286, &v396);
            v236.x = v303.x;
            v236.y = v303.y;
            v236.z = 0.0;
            X = 0.0f * 0.0f + v236.y * v236.y + v236.x * v236.x;
            if (v372->f_sqr_dist_max < (double)X)
            {
                X = sqrtf(X);
                v126 = 1.0f / X;
                v236.x = v236.x * v126;
                v236.y = v236.y * v126;
                v236.z = v236.z * v126;
                X = X - v372->des_f_dist_max;
                v326 = v236.x * X;
                v327 = v236.y * X;
                v328 = v236.z * X;
                v238 = v326;
                v239 = v327;
                v240 = v328;
                v180 = AI_EvalFunc_DYNFrictionGet_C(AI_gpst_CurrentGameObject);
                v382.x = v238 * v180;
                v382.y = v239 * v180;
                v382.z = v240 * v180;
                AI_EvalFunc_DYNStreamSet_C(AI_gpst_CurrentGameObject, &v382);
            }
        }
        v206 = *AI_MainActor;
        v125 = v206;
        if (v206)
        {
            if ((unsigned int)v206 <= 2)
                v125 = AI_MainActor[(uint32_t)v206 - 1];
            v2 = AI_EvalFunc_COLGMatReportGet_C(v125, 16);
        }
        else
        {
            v2 = AI_EvalFunc_COLGMatReportGet_C(0, 16);
        }
        v225 = v2;
        if (v2 != -1)
            v372->f_time_water_col = TIM_gf_MainClock - TIM_gf_LockTime;
        v302 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Objet_Flottant_OF_comportement_Objet_Flottant_OF_Message_Read);
        if (v302 == 1000)
        {
            v124 = v206;
            if (v206)
            {
                if ((unsigned int)v206 <= 2)
                    v124 = AI_MainActor[(uint32_t)v206 - 1];
                v123 = v124;
            }
            else
            {
                v123 = 0;
            }
            if (v123 == (tGameObjectMain*)Objet_Flottant_OF_comportement__PJ_Sally_gao
                || (AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v388),
                    v197.x = 0.0,
                    v197.y = 0.0,
                    v197.z = 1.0,
                    v404 = 1.0f * v388.z + 0.0f * v388.y + v388.x * 0.0f,
                    v404 > 0.86602497)
                && (v332 = AI_EvalFunc_TIMEElapsed_C(v372->f_time_water_col, 0.30000001f)))
            {
                v372->i_flag_ignore_hierachy = 0;
                v300 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0, 0x8000);
            }
            else
            {
                v372->i_flag_ignore_hierachy = 1;
                v372->f_time_ignore_hier = TIM_gf_MainClock - TIM_gf_LockTime;
                v205 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x8000, 0);
                for (i = 0; i < v372->i_nb_fils; ++i)
                {
                    v122 = (tGameObjectMain*)v372->ao_hierarchy_object[i];
                    if (v122)
                    {
                        if ((unsigned int)v122 <= 2)
                            v122 = AI_MainActor[(uint32_t)v122 - 1];
                        v3 = AI_EvalFunc_OBJ_HierarchyGet_C(v122);
                    }
                    else
                    {
                        v3 = AI_EvalFunc_OBJ_HierarchyGet_C(0);
                    }
                    v356 = v3;
                    v394 = AI_gpst_CurrentGameObject;
                    v121 = v3;
                    if (v3)
                    {
                        if ((unsigned int)v3 <= 2)
                            v121 = AI_MainActor[(uint32_t)v3 - 1];
                        v120 = v121;
                    }
                    else
                    {
                        v120 = 0;
                    }
                    v119 = v394;
                    if (v394)
                    {
                        if ((unsigned int)v394 <= 2)
                            v119 = AI_MainActor[(uint32_t)v394 - 1];
                        v118 = v119;
                    }
                    else
                    {
                        v118 = 0;
                    }
                    if (v120 == v118)
                    {
                        v117 = (tGameObjectMain*)v372->ao_hierarchy_object[i];
                        if (v117)
                        {
                            if ((unsigned int)v117 <= 2)
                                v117 = AI_MainActor[(uint32_t)v117 - 1];
                            AI_EvalFunc_OBJ_HierarchyReset_C(v117);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_HierarchyReset_C(0);
                        }
                    }
                    v372->ao_hierarchy_object[i] = 0;
                    *(uint32_t*)AI_EvalVarArray_C2((char*)&v372->__f_time_dyn_hier_Dimension1, i) = 0;
                }
                v372->i_nb_fils = 0;
            }
            if (!v372->i_flag_ignore_hierachy)
            {
                v116 = v206;
                if (v206)
                {
                    if ((unsigned int)v206 <= 2)
                        v116 = AI_MainActor[(uint32_t)v206 - 1];
                    v115 = v116;
                }
                else
                {
                    v115 = 0;
                }
                v273 = AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)v372->ao_hierarchy_object, 10, v115);
                if (v273 != -1)
                    goto LABEL_105;
                v114 = v206;
                if (v206)
                {
                    if ((unsigned int)v206 <= 2)
                        v114 = AI_MainActor[(uint32_t)v206 - 1];
                    v4 = AI_EvalFunc_OBJ_HierarchyGet_C(v114);
                }
                else
                {
                    v4 = AI_EvalFunc_OBJ_HierarchyGet_C(0);
                }
                v334 = v4;
                v208 = AI_gpst_CurrentGameObject;
                v113 = v4;
                if (v4)
                {
                    if ((unsigned int)v4 <= 2)
                        v113 = AI_MainActor[(uint32_t)v4 - 1];
                    v112 = v113;
                }
                else
                {
                    v112 = 0;
                }
                v111 = v208;
                if (v208)
                {
                    if ((unsigned int)v208 <= 2)
                        v111 = AI_MainActor[(uint32_t)v208 - 1];
                    v110 = v111;
                }
                else
                {
                    v110 = 0;
                }
                if (v112 == v110)
                {
                    v372->ao_hierarchy_object[v372->i_nb_fils++] = (int)v206;
                }
                else
                {
                LABEL_105:
                    v372->v_last_speed.x = 0.0;
                    v372->v_last_speed.y = 0.0;
                    v372->v_last_speed.z = 0.0;
                }
                for (i = 0; i < 10; ++i)
                {
                    v228 = *(uint32_t*)(Objet_Flottant_OF_comportement_univVars_Perso_ID_actor + 4 * i + 4);
                    v109 = (tGameObjectMain*)v228;
                    if (v228)
                    {
                        if (v228 <= 2)
                            v109 = AI_MainActor[v228 - 1];
                        v108 = v109;
                    }
                    else
                    {
                        v108 = 0;
                    }
                    v273 = AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)v372->ao_hierarchy_object, 10, v108);
                    if (v273 == -1)
                    {
                        v107 = (tGameObjectMain*)v228;
                        if (v228)
                        {
                            if (v228 <= 2)
                                v107 = AI_MainActor[v228 - 1];
                            v5 = AI_EvalFunc_OBJ_HierarchyGet_C(v107);
                        }
                        else
                        {
                            v5 = AI_EvalFunc_OBJ_HierarchyGet_C(0);
                        }
                        v352 = v5;
                        v194 = AI_gpst_CurrentGameObject;
                        v106 = v5;
                        if (v5)
                        {
                            if ((unsigned int)v5 <= 2)
                                v106 = AI_MainActor[(uint32_t)v5 - 1];
                            v105 = v106;
                        }
                        else
                        {
                            v105 = 0;
                        }
                        v104 = v194;
                        if (v194)
                        {
                            if ((unsigned int)v194 <= 2)
                                v104 = AI_MainActor[(uint32_t)v194 - 1];
                            v103 = v104;
                        }
                        else
                        {
                            v103 = 0;
                        }
                        if (v105 == v103)
                            v372->ao_hierarchy_object[v372->i_nb_fils++] = v228;
                    }
                }
                if (v372->i_obj_sup_nb)
                {
                    for (i = 0; i < v372->i_obj_sup_nb; ++i)
                    {
                        v102 = (tGameObjectMain*)v372->des_o_obj_list[i];
                        if (v102)
                        {
                            if ((unsigned int)v102 <= 2)
                                v102 = AI_MainActor[(uint32_t)v102 - 1];
                            v101 = v102;
                        }
                        else
                        {
                            v101 = 0;
                        }
                        v273 = AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)v372->ao_hierarchy_object, 10, v101);
                        if (v273 == -1)
                        {
                            v100 = (tGameObjectMain*)v372->des_o_obj_list[i];
                            if (v100)
                            {
                                if ((unsigned int)v100 <= 2)
                                    v100 = AI_MainActor[(uint32_t)v100 - 1];
                                v6 = AI_EvalFunc_OBJ_HierarchyGet_C(v100);
                            }
                            else
                            {
                                v6 = AI_EvalFunc_OBJ_HierarchyGet_C(0);
                            }
                            v406 = v6;
                            v204 = AI_gpst_CurrentGameObject;
                            v99 = v6;
                            if (v6)
                            {
                                if ((unsigned int)v6 <= 2)
                                    v99 = AI_MainActor[(uint32_t)v6 - 1];
                                v98 = v99;
                            }
                            else
                            {
                                v98 = 0;
                            }
                            v97 = v204;
                            if (v204)
                            {
                                if ((unsigned int)v204 <= 2)
                                    v97 = AI_MainActor[(uint32_t)v204 - 1];
                                v96 = v97;
                            }
                            else
                            {
                                v96 = 0;
                            }
                            if (v98 == v96)
                                v372->ao_hierarchy_object[v372->i_nb_fils++] = v372->des_o_obj_list[i];
                        }
                    }
                }
            }
            AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v266);
            v402 = v266.z;
            for (i = 0; i < v372->i_nb_fils; ++i)
            {
                v95 = (tGameObjectMain*)v372->ao_hierarchy_object[i];
                if (v95)
                {
                    if ((unsigned int)v95 <= 2)
                        v95 = AI_MainActor[(uint32_t)v95 - 1];
                    v7 = AI_EvalFunc_COLGaoGet_C(v95, 0x40);
                }
                else
                {
                    v7 = AI_EvalFunc_COLGaoGet_C(0, 0x40);
                }
                v207 = v7;
                v259 = AI_gpst_CurrentGameObject;
                v94 = v7;
                if (v7)
                {
                    if ((unsigned int)v7 <= 2)
                        v94 = AI_MainActor[(uint32_t)v7 - 1];
                    v93 = v94;
                }
                else
                {
                    v93 = 0;
                }
                v92 = v259;
                if (v259)
                {
                    if ((unsigned int)v259 <= 2)
                        v92 = AI_MainActor[(uint32_t)v259 - 1];
                    v91 = v92;
                }
                else
                {
                    v91 = 0;
                }
                if (v93 == v91)
                {
                    v35 = TIM_gf_MainClock - TIM_gf_LockTime;
                    *(float*)AI_EvalVarArray_C2((char*)&v372->__f_time_dyn_hier_Dimension1, i) = v35;
                    v372->f_time_ground_col = TIM_gf_MainClock - TIM_gf_LockTime;
                    v372->f_time_dyn_ground_col = TIM_gf_MainClock - TIM_gf_LockTime;
                }
                v8 = (float*)AI_EvalVarArray_C2((char*)&v372->__f_time_dyn_hier_Dimension1, i);
                if (AI_EvalFunc_TIMEElapsed_C(*v8, 0.30000001f))
                {
                    v90 = (tGameObjectMain*)v372->ao_hierarchy_object[i];
                    if (v90)
                    {
                        if ((unsigned int)v90 <= 2)
                            v90 = AI_MainActor[(uint32_t)v90 - 1];
                        v9 = AI_EvalFunc_OBJ_HierarchyGet_C(v90);
                    }
                    else
                    {
                        v9 = AI_EvalFunc_OBJ_HierarchyGet_C(0);
                    }
                    v193 = v9;
                    v299 = AI_gpst_CurrentGameObject;
                    v89 = v9;
                    if (v9)
                    {
                        if ((unsigned int)v9 <= 2)
                            v89 = AI_MainActor[(uint32_t)v9 - 1];
                        v88 = v89;
                    }
                    else
                    {
                        v88 = 0;
                    }
                    v87 = v299;
                    if (v299)
                    {
                        if ((unsigned int)v299 <= 2)
                            v87 = AI_MainActor[(uint32_t)v299 - 1];
                        v86 = v87;
                    }
                    else
                    {
                        v86 = 0;
                    }
                    if (v88 == v86)
                    {
                        v85 = (tGameObjectMain*)v372->ao_hierarchy_object[i];
                        if (v85)
                        {
                            if ((unsigned int)v85 <= 2)
                                v85 = AI_MainActor[(uint32_t)v85 - 1];
                            AI_EvalFunc_OBJ_HierarchyReset_C(v85);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_HierarchyReset_C(0);
                        }
                    }
                    if (--v372->i_nb_fils <= i)
                    {
                        v372->ao_hierarchy_object[i] = 0;
                        *(uint32_t*)AI_EvalVarArray_C2((char*)&v372->__f_time_dyn_hier_Dimension1, i) = 0;
                    }
                    else
                    {
                        v372->ao_hierarchy_object[i] = v372->ao_hierarchy_object[v372->i_nb_fils];
                        v34 = *(float*)AI_EvalVarArray_C2((char*)&v372->__f_time_dyn_hier_Dimension1, v372->i_nb_fils);
                        *(float*)AI_EvalVarArray_C2((char*)&v372->__f_time_dyn_hier_Dimension1, i) = v34;
                        v372->ao_hierarchy_object[v372->i_nb_fils] = 0;
                        *(uint32_t*)AI_EvalVarArray_C2((char*)&v372->__f_time_dyn_hier_Dimension1, v372->i_nb_fils) = 0;
                        --i;
                    }
                }
            }
            if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 64))
            {
                v316 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 64);
                v84 = v316;
                if (v316)
                {
                    if ((unsigned int)v316 <= 2)
                        v84 = AI_MainActor[(uint32_t)v316 - 1];
                    v10 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v84, 4);
                }
                else
                {
                    v10 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 4);
                }
                v143 = v10;
                if (!v10)
                {
                    v372->f_time_ground_col = TIM_gf_MainClock - TIM_gf_LockTime;
                    AI_EvalFunc_COLPointGet_C(AI_gpst_CurrentGameObject, 64, &v150);
                    v268 = v150;
                    if (!AI2C_VecEqNull(&v268))
                    {
                        AI_EvalFunc_COLPosGet_C(AI_gpst_CurrentGameObject, 0, &v247);
                        MATH_SubstractVec(&v133, &v247, &v268);
                        v268 = v133;
                        v335 = AI_EvalFunc_COLSizeGet_C(AI_gpst_CurrentGameObject, 0);
                        v83 = 1.0f / v335;
                        v268.x = v268.x * v83;
                        v268.y = v268.y * v83;
                        v268.z = v268.z * v83;
                        if (v268.z <= 0.0)
                        {
                            v257.x = -v268.x;
                            v257.y = -v268.y;
                            v257.z = -v268.z;
                            v82 = &v372->v_normale_sol;
                            v372->v_normale_sol.x = -v268.x;
                            v82->y = v257.y;
                            v82->z = v257.z;
                        }
                        else
                        {
                            v372->v_normale_sol = v268;
                        }
                    }
                }
            }
            AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v310);
            v271 = v310;
            AI_EvalFunc_COLPosGet_C(AI_gpst_CurrentGameObject, 0, &v323);
            v329 = v323;
            v225 = AI_EvalFunc_COLGMatReportGet_C(AI_gpst_CurrentGameObject, 16);
            if (v225 == -1)
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v309);
                v371 = v309;
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v269);
                v81 = v372->des_f_archimede_offset;
                v343 = v269.x * v81;
                v344 = v269.y * v81;
                v345 = v269.z * v81;
                v371.x = v371.x + v343;
                v371.y = v371.y + v344;
                v371.z = v371.z + v345;
                if (v372->f_water_Z <= (double)v371.z)
                {
                    if (v371.z >= (double)v274)
                    {
                        v372->i_flag_in_water = 0;
                        v372->f_water_Z = -1.0e14f;
                    }
                    else
                    {
                        if (!v372->i_flag_in_water && v271.z < -5.0)
                        {
                            v79 = -v271.z / 80.0f;
                            if (v79 > 1.0)
                                v31 = 1.0;
                            else
                                v31 = -v271.z / 80.0f;
                            v190 = v31;
                            v222 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(2, v31);
                            v318 = SND_l_GetFreq(v222);
                            v331 = AI_EvalFunc_MATHRandFloat_C(0.89999998f, 1.0);
                            v30 = (float)v318;
                            SND_SetFreq(v222, (unsigned __int64)(v30 * v331));
                        }
                        v372->i_flag_real_water = 1;
                        v372->f_water_Z = v274;
                        v372->f_last_valid_water_Z = v372->f_water_Z;
                        v372->i_flag_in_water = 1;
                    }
                }
                else
                {
                    if (!v372->i_flag_in_water && v271.z < -5.0)
                    {
                        v80 = -v271.z / 80.0f;
                        if (v80 > 1.0)
                            v33 = 1.0;
                        else
                            v33 = -v271.z / 80.0f;
                        v190 = v33;
                        v222 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(2, v33);
                        v138 = SND_l_GetFreq(v222);
                        v145 = AI_EvalFunc_MATHRandFloat_C(0.89999998f, 1.0);
                        v32 = (float)v138;
                        SND_SetFreq(v222, (unsigned int)(v32 * v145));
                    }
                    v372->i_flag_in_water = 1;
                }
            }
            else
            {
                AI_EvalFunc_COLPointGet_C(AI_gpst_CurrentGameObject, v225 + 0x80000000, &v148);
                v177 = v148;
                v372->f_water_Z = v148.z;
                v372->f_last_valid_water_Z = v372->f_water_Z;
                v372->i_flag_in_water = 1;
            }
            if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 128))
            {
                if (AI_EvalFunc_TIMEElapsed_C(v372->f_snd_time_choc, 0.34999999f))
                {
                    v372->f_snd_time_choc = TIM_gf_MainClock - TIM_gf_LockTime;
                    AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v262);
                    MATH_SubstractVec(&v135, &v262, &v372->v_last_speed);
                    v242 = v135;
                    v144 = MATH_VecLen(&v242);
                    if (v144 > 5.0)
                    {
                        v278 = MATH_VecLen(&v242);
                        v355 = v278 / v372->f_snd_coef_vol_choc;
                        if (v355 >= 0.1)
                        {
                            v351 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(AI_gpst_CurrentGameObject, 0, -1.0, 0.94999999f, 1.05f);
                            v408 = SND_f_GetVol(v351);
                            v25 = v408 * v355;
                            SND_SetVol(v351, v25);
                        }
                    }
                }
                v316 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 128);
                v78 = v316;
                if (v316)
                {
                    if ((unsigned int)v316 <= 2)
                        v78 = AI_MainActor[(uint32_t)v316 - 1];
                    v77 = v78;
                }
                else
                {
                    v77 = 0;
                }
                v76 = v206;
                if (v206)
                {
                    if ((unsigned int)v206 <= 2)
                        v76 = AI_MainActor[(uint32_t)v206 - 1];
                    v75 = v76;
                }
                else
                {
                    v75 = 0;
                }
                if (v77 == v75)
                {
                    v74 = v206;
                    if (v206)
                    {
                        if ((unsigned int)v206 <= 2)
                            v74 = AI_MainActor[(uint32_t)v206 - 1];
                        v73 = v74;
                    }
                    else
                    {
                        v73 = 0;
                    }
                    if (v73 != (tGameObjectMain*)Objet_Flottant_OF_comportement__PJ_Sally_gao)
                        v372->f_time_main_col = TIM_gf_MainClock - TIM_gf_LockTime;
                }
                AI_EvalFunc_COLPointGet_C(AI_gpst_CurrentGameObject, 128, &v389);
                v162 = v389;
                if (!AI2C_VecEqNull(&v162))
                {
                    v72 = v316;
                    if (v316)
                    {
                        if ((unsigned int)v316 <= 2)
                            v72 = AI_MainActor[(uint32_t)v316 - 1];
                        v11 = AI_EvalFunc_AIIsModel_C(v72, (tAIModel*)fct_Objet_Flottant_OF_comportement__PJ_Camera_gao);
                    }
                    else
                    {
                        v11 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fct_Objet_Flottant_OF_comportement__PJ_Camera_gao);
                    }
                    v246 = v11;
                    if (!v11)
                    {
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&v184);
                        v162.x = v162.x - v184.x;
                        v162.y = v162.y - v184.y;
                        v162.z = v162.z - v184.z;
                        if (AI2C_VecEqNull(&v271))
                        {
                            v71 = v316;
                            if (v316)
                            {
                                if ((unsigned int)v316 <= 2)
                                    v71 = AI_MainActor[(uint32_t)v316 - 1];
                                AI_EvalFunc_DYNSpeedGetVector_C(v71, &v256);
                            }
                            else
                            {
                                AI_EvalFunc_DYNSpeedGetVector_C(0, &v256);
                            }
                            v271 = v256;
                        }
                        if (!AI2C_VecEqNull(&v271))
                        {
                            v70 = v316;
                            if (v316)
                            {
                                if ((unsigned int)v316 <= 2)
                                    v70 = AI_MainActor[(uint32_t)v316 - 1];
                                v12 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v70, 0x204);
                            }
                            else
                            {
                                v12 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x204);
                            }
                            v403 = v12;
                            if (v12)
                            {
                                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v322);
                                v261 = v162.z * v322.z + v162.y * v322.y + v322.x * v162.x;
                                v69 = -v261;
                                if (v69 <= 3.0)
                                {
                                    if (v69 >= -3.0)
                                        v195 = -v261;
                                    else
                                        v195 = -3.0;
                                }
                                else
                                {
                                    v195 = 3.0;
                                }
                                v354 = v195;
                            }
                            else
                            {
                                v354 = 0.0;
                            }
                            v330 = v271;
                            v405 = AI_EvalFunc_TIMEElapsed_C(v372->f_time_ground_col, 0.1f);
                            if (v405 || !(v336 = AI_EvalFunc_TIMEElapsed_C(v372->f_time_paf, 0.1f)))
                            {
                                v248 = v330.x * v354;
                                v249 = v330.y * v354;
                                v250 = v330.z * v354;
                                v68 = &v372->v_grav_speed;
                                v372->v_grav_speed.x = v248 + v372->v_grav_speed.x;
                                v68->y = v249 + v68->y;
                                v68->z = v250 + v68->z;
                            }
                            if (v354 > 0.0)
                            {
                                v67 = v316;
                                if (v316)
                                {
                                    if ((unsigned int)v316 <= 2)
                                        v67 = AI_MainActor[(uint32_t)v316 - 1];
                                    v13 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v67, 516);
                                }
                                else
                                {
                                    v13 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 516);
                                }
                                v203 = v13;
                                if (v13)
                                {
                                    if (!AI_EvalFunc_TIMEElapsed_C(v372->f_time_ground_col, 0.1f))
                                    {
                                        v350 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
                                        v66 = v350 - v372->f_last_speed;
                                        if (v66 <= 20.0)
                                        {
                                            if (v66 >= 0.0)
                                                v276 = v350 - v372->f_last_speed;
                                            else
                                                v276 = 0.0;
                                        }
                                        else
                                        {
                                            v276 = 20.0;
                                        }
                                        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v200);
                                        v65 = v354 * v276;
                                        v130 = v200.x * v65;
                                        v131 = v200.y * v65;
                                        v132 = v200.z * v65;
                                        v330.x = v330.x - v130;
                                        v330.y = v330.y - v131;
                                        v330.z = v330.z - v132;
                                    }
                                }
                            }
                            v349 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v330);
                            if (v372->des_rotation)
                            {
                                v64 = v316;
                                if (v316)
                                {
                                    if ((unsigned int)v316 <= 2)
                                        v64 = AI_MainActor[(uint32_t)v316 - 1];
                                    v14 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v64, 516);
                                }
                                else
                                {
                                    v14 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 516);
                                }
                                v275 = v14;
                                if (v14)
                                {
                                    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v196);
                                    AI_EvalFunc_MATHVecCrossProduct_C(&v271, &v196, &v137);
                                    v214 = v137;
                                    if (!AI2C_VecEqNull(&v214))
                                    {
                                        if (AI_EvalFunc_TIMEElapsed_C(v372->f_time_ground_col, 0.1f))
                                        {
                                            v294 = v162.z * v214.z + v162.y * v214.y + v214.x * v162.x;
                                            if (v294 <= 3.0)
                                            {
                                                if (v294 >= -3.0)
                                                    v224 = v294;
                                                else
                                                    v224 = -3.0;
                                            }
                                            else
                                            {
                                                v224 = 3.0;
                                            }
                                            v372->f_rotation_speed = v372->des_f_rot_speed_water * v224;
                                        }
                                        else
                                        {
                                            v169 = v162.z * v214.z + v162.y * v214.y + v214.x * v162.x;
                                            if (v169 <= 3.0)
                                            {
                                                if (v169 >= -3.0)
                                                    v379 = v169;
                                                else
                                                    v379 = -3.0;
                                            }
                                            else
                                            {
                                                v379 = 3.0;
                                            }
                                            v372->f_rotation_speed = v372->des_f_rot_speed_ground * v379;
                                        }
                                        v297 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
                                        v63 = (v297 - v372->f_last_speed) / 100.0f;
                                        if (v63 <= 1.0)
                                        {
                                            if (v63 >= 0.0)
                                                v229 = (v297 - v372->f_last_speed) / 100.0f;
                                            else
                                                v229 = 0.0;
                                        }
                                        else
                                        {
                                            v229 = 1.0;
                                        }
                                        v372->f_rotation_speed = v229 * v372->f_rotation_speed;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else if (v372->des_rotation)
            {
                if (AI_EvalFunc_TIMEElapsed_C(v372->f_time_ground_col, 0.1f))
                {
                    v381 = TIM_gf_dt;
                    v372->f_rotation_speed = v372->f_rotation_speed
                        - v372->f_rotation_speed * v372->des_f_rot_friction_water * TIM_gf_dt;
                }
                else
                {
                    v375 = TIM_gf_dt;
                    v372->f_rotation_speed = v372->f_rotation_speed
                        - v372->f_rotation_speed * v372->des_f_rot_friction_ground * TIM_gf_dt;
                }
                v291 = TIM_gf_dt;
                v21 = v372->f_rotation_speed * TIM_gf_dt;
                AI_EvalFunc_OBJRotateLocalZ_c(AI_gpst_CurrentGameObject, v21);
            }
            if (v372->i_flag_in_water)
            {
                v372->v_last_collide_pos.x = 0.0;
                v372->v_last_collide_pos.y = 0.0;
                v372->v_last_collide_pos.z = 0.0;
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v154);
                v368 = 0;
                v369 = 0;
                v370 = 1065353216;
                v296 = 1.0f * v154.z + 0.0f * v154.y + v154.x * 0.0f;
                v29 = (float)fabs(v296);
                v227 = v29;
                X = v29;
                v62 = v29;
                if (v29 < 0.0)
                    v62 = 0.0;
                if (v62 > 1.0)
                    v62 = 1.0;
                v185.x = v372->des_v_water_friction.x * v62 + v372->des_v_water_friction.z * (1.0f - v62);
                v185.y = v185.x;
                v61 = X;
                if (X < 0.0)
                    v61 = 0.0;
                if (v61 > 1.0)
                    v61 = 1.0;
                v185.z = v372->des_v_water_friction.z * v61 + v372->des_v_water_friction.x * (1.0f - v61);
                v295 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v185);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v198);
                v371 = v198;
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v267);
                v60 = v372->des_f_archimede_offset;
                v340 = v267.x * v60;
                v341 = v267.y * v60;
                v342 = v267.z * v60;
                v371.x = v371.x + v340;
                v371.y = v371.y + v341;
                v371.z = v371.z + v342;
                v59 = v372->f_water_Z - v371.z;
                if (v59 < 0.0)
                    v28 = 0.0;
                else
                    v28 = v372->f_water_Z - v371.z;
                v140 = v28;
                v372->v_water_force.x = 0.0;
                v372->v_water_force.y = 0.0;
                v372->v_water_force.z = v372->des_f_archimede * v140;
                AI_EvalFunc_DYNTractionSet_C_CURRENT(&v372->v_water_force);
                if (!v372->i_nb_fils
                    || v372->i_flag_ignore_hierachy
                    || (v272 = AI_EvalFunc_TIMEElapsed_C(v372->f_time_dyn_ground_col, 0.1f)))
                {
                    if (AI_EvalFunc_TIMEElapsed_C(v372->f_time_ground_col, 0.1f))
                    {
                        AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v288);
                        v271 = v288;
                        v236 = v372->v_grav_pos;
                        v52 = v372->des_f_water_friction;
                        v360 = v52 * v372->v_grav_speed.x;
                        v361 = v372->v_grav_speed.y * v52;
                        v362 = v372->v_grav_speed.z * v52;
                        v170 = TIM_gf_dt;
                        v233 = v360 * TIM_gf_dt;
                        v234 = v361 * TIM_gf_dt;
                        v235 = v362 * TIM_gf_dt;
                        v51 = &v372->v_grav_pos;
                        v372->v_grav_pos.x = v233 + v372->v_grav_pos.x;
                        v51->y = v234 + v51->y;
                        v51->z = v235 + v51->z;
                        v306 = 0;
                        v307 = 0;
                        v308 = v372->des_f_water_gravity;
                        v392 = TIM_gf_dt;
                        v174 = 0.0f * TIM_gf_dt;
                        v175 = v174;
                        v176 = v308 * TIM_gf_dt;
                        v50 = &v372->v_grav_pos;
                        v372->v_grav_pos.x = v174 + v372->v_grav_pos.x;
                        v50->y = v175 + v50->y;
                        v50->z = v176 + v50->z;
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&v241);
                        MATH_SubstractVec(&v372->v_grav_pos, &v372->v_grav_pos, &v241);
                        AI_EvalFunc_MATHVecSetNormVector_C(&v372->v_grav_pos, v372->des_f_dist_centre_grav);
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&v387);
                        v49 = &v372->v_grav_pos;
                        v372->v_grav_pos.x = v387.x + v372->v_grav_pos.x;
                        v49->y = v387.y + v49->y;
                        v49->z = v387.z + v49->z;
                        MATH_SubstractVec(&v173, &v372->v_grav_pos, &v236);
                        v244 = TIM_gf_dt;
                        MATH_VecMultNormScalar(&v304, &v173, TIM_gf_dt);
                        v372->v_grav_speed = v304;
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&v215);
                        MATH_SubstractVec(&v147, &v215, &v372->v_grav_pos);
                        AI_EvalFunc_MATHVecNormalize_C(&v147, &v358);
                        v281 = v358;
                        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v284);
                        v220 = TIM_gf_dt;
                        v17 = 8.0f * TIM_gf_dt;
                        AI_EvalFunc_MATHVecBlendRotate_C(&v284, &v281, v17, &v153);
                        v281 = v153;
                    }
                    else
                    {
                        v376 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
                        v48 = v376 / 10.0f;
                        if (v48 > 6.0)
                            v23 = 6.0;
                        else
                            v23 = v376 / 10.0f;
                        v292 = v23;
                        v258 = v23;
                        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v218);
                        v167 = TIM_gf_dt;
                        v18 = v258 * TIM_gf_dt;
                        AI_EvalFunc_MATHVecBlendRotate_C(&v218, &v372->v_normale_sol, v18, &v385);
                        v281 = v385;
                        v236 = v372->v_grav_pos;
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&v311);
                        v47 = v372->des_f_dist_centre_grav;
                        v237.x = v281.x * v47;
                        v237.y = v281.y * v47;
                        v237.z = v281.z * v47;
                        MATH_SubstractVec(&v172, &v311, &v237);
                        v372->v_grav_pos = v172;
                        MATH_SubstractVec(&v386, &v372->v_grav_pos, &v236);
                        v314 = TIM_gf_dt;
                        MATH_VecMultNormScalar(&v232, &v386, TIM_gf_dt);
                        v372->v_grav_speed = v232;
                    }
                }
                else
                {
                    v346 = 0;
                    v347 = 0;
                    v348 = 0;
                    a3.x = 0.0;
                    a3.y = 0.0;
                    a3.z = 0.0;
                    for (i = 0; i < v372->i_nb_fils; ++i)
                    {
                        v58 = (tGameObjectMain*)v372->ao_hierarchy_object[i];
                        if (v58)
                        {
                            if ((unsigned int)v58 <= 2)
                                v58 = AI_MainActor[(uint32_t)v58 - 1];
                            AI_EvalFunc_OBJGetPos_C(v58, &v134);
                        }
                        else
                        {
                            AI_EvalFunc_OBJGetPos_C(0, &v134);
                        }
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&v202);
                        MATH_SubstractVec(&v264, &v134, &v202);
                        a3.x = a3.x + v264.x;
                        a3.y = a3.y + v264.y;
                        a3.z = a3.z + v264.z;
                    }
                    v56 = (float)v372->i_nb_fils;
                    v57 = 1.0f / v56;
                    a3.x = a3.x * v57;
                    a3.y = a3.y * v57;
                    a3.z = a3.z * v57;
                    v55 = v206;
                    if (v206)
                    {
                        if ((unsigned int)v206 <= 2)
                            v55 = AI_MainActor[(uint32_t)v206 - 1];
                        v54 = v55;
                    }
                    else
                    {
                        v54 = 0;
                    }
                    if (v54 == (tGameObjectMain*)Objet_Flottant_OF_comportement__PJ_Sally_gao)
                    {
                        v160.x = 0.0;
                        v160.y = 0.0;
                        v27 = (float)v372->i_nb_fils;
                        v160.z = v372->des_f_base_gravity + v372->des_f_perso_gravity * v27;
                        AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v160);
                        v26 = (float)v372->i_nb_fils;
                        a3.x = v372->des_f_coef_banking_perso * v26 * a3.x;
                        v24 = (float)v372->i_nb_fils;
                        a3.y = v372->des_f_coef_banking_perso * v24 * a3.y;
                    }
                    else
                    {
                        v212.x = 0.0;
                        v212.y = 0.0;
                        v212.z = v372->des_f_base_gravity + v372->des_f_aircraft_gravity;
                        AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v212);
                        a3.x = v372->des_f_coef_banking_aircraft * a3.x;
                        a3.y = v372->des_f_coef_banking_aircraft * a3.y;
                    }
                    MATH_NormalizeEqualVector(&a3);
                    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&a2);
                    v378 = TIM_gf_dt;
                    v16 = 2.0f * TIM_gf_dt;
                    AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, v16, &a5);
                    v281 = a5;
                    AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v219);
                    v280.x = 0.0;
                    v280.y = 0.0;
                    v280.z = v372->v_last_speed.z * 0.80000001f;
                    MATH_VecAddVec(&v367, &v219, &v280);
                    v166 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v367);
                    v236 = v372->v_grav_pos;
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v216);
                    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v366);
                    v53 = v372->des_f_dist_centre_grav;
                    v159.x = v366.x * v53;
                    v159.y = v366.y * v53;
                    v159.z = v366.z * v53;
                    MATH_SubstractVec(&v211, &v216, &v159);
                    v372->v_grav_pos = v211;
                    MATH_SubstractVec(&v282, &v372->v_grav_pos, &v236);
                    v377 = TIM_gf_dt;
                    MATH_VecMultNormScalar(&v151, &v282, TIM_gf_dt);
                    v372->v_grav_speed = v151;
                }
                v372->f_time_air = TIM_gf_MainClock - TIM_gf_LockTime;
            }
            else
            {
                v391 = AI_EvalFunc_TIMEElapsed_C(v372->f_time_ground_col, 0.1f);
                if (!v391 && (v313 = AI_EvalFunc_TIMEElapsed_C(v372->f_time_paf, 0.30000001f)))
                {
                    if (!AI_EvalFunc_TIMEElapsed_C(v372->f_time_air, 0.5))
                        AI_EvalFunc_DYNReboundSet_C(AI_gpst_CurrentGameObject, 0.0);
                    v285.x = 0.0;
                    v285.y = 0.0;
                    v285.z = 0.0;
                    AI_EvalFunc_DYNTractionSet_C_CURRENT(&v285);
                    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v213);
                    v155 = 0;
                    v156 = 0;
                    v157 = 1065353216;
                    v380 = 1.0f * v213.z + 0.0f * v213.y + v213.x * 0.0f;
                    v22 = (float)fabs(v380);
                    v301 = v22;
                    X = v22;
                    v42 = v22;
                    if (v22 < 0.0)
                        v42 = 0.0;
                    if (v42 > 1.0)
                        v42 = 1.0;
                    v185.x = v372->des_v_ground_friction.x * v42 + v372->des_v_ground_friction.z * (1.0f - v42);
                    v185.y = v185.x;
                    v41 = X;
                    if (X < 0.0)
                        v41 = 0.0;
                    if (v41 > 1.0)
                        v41 = 1.0;
                    v185.z = v372->des_v_ground_friction.z * v41 + v372->des_v_ground_friction.x * (1.0f - v41);
                    v390 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v185);
                    v315 = TIM_gf_MainClock - TIM_gf_LockTime;
                    v372->f_time_air = v315;
                    v243 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
                    v40 = v243 / 10.0f;
                    if (v40 <= 6.0)
                    {
                        if (v40 >= 2.0)
                            v179 = v243 / 10.0f;
                        else
                            v179 = 2.0;
                    }
                    else
                    {
                        v179 = 6.0;
                    }
                    v258 = v179;
                    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v384);
                    v317 = TIM_gf_dt;
                    v20 = v258 * TIM_gf_dt;
                    AI_EvalFunc_MATHVecBlendRotate_C(&v384, &v372->v_normale_sol, v20, &v231);
                    v281 = v231;
                    v236.x = v231.x * 10.0f;
                    v236.y = v231.y * 10.0f;
                    v236.z = 0.0f * 10.0f;
                    v181 = AI_EvalFunc_DYNFrictionGet_C(AI_gpst_CurrentGameObject);
                    v383.x = v236.x * v181;
                    v383.y = v236.y * v181;
                    v383.z = v236.z * v181;
                    AI_EvalFunc_DYNTractionSet_C_CURRENT(&v383);
                    v236 = v372->v_grav_pos;
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v305);
                    v39 = v372->des_f_dist_centre_grav;
                    v357.x = v281.x * v39;
                    v357.y = v281.y * v39;
                    v357.z = v281.z * v39;
                    MATH_SubstractVec(&v158, &v305, &v357);
                    v372->v_grav_pos = v158;
                    AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v217);
                    v293 = TIM_gf_dt;
                    v363 = v217.x * TIM_gf_dt;
                    v364 = v217.y * TIM_gf_dt;
                    v365 = v217.z * TIM_gf_dt;
                    v38 = &v372->v_grav_pos;
                    v372->v_grav_pos.x = v372->v_grav_pos.x - v363;
                    v38->y = v38->y - v364;
                    v38->z = v38->z - v365;
                    MATH_SubstractVec(&v149, &v372->v_grav_pos, &v236);
                    v226 = TIM_gf_dt;
                    MATH_VecMultNormScalar(&v279, &v149, TIM_gf_dt);
                    v372->v_grav_speed = v279;
                }
                else
                {
                    v372->v_last_collide_pos.x = 0.0;
                    v372->v_last_collide_pos.y = 0.0;
                    v372->v_last_collide_pos.z = 0.0;
                    v325.x = 0.0;
                    v325.y = 0.0;
                    v325.z = 0.0;
                    AI_EvalFunc_DYNTractionSet_C_CURRENT(&v325);
                    v251.x = 0.0;
                    v251.y = 0.0;
                    v251.z = 0.0;
                    v192 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v251);
                    v236 = v372->v_grav_pos;
                    v46 = v372->des_f_air_friction;
                    v399 = v46 * v372->v_grav_speed.x;
                    v400 = v372->v_grav_speed.y * v46;
                    v401 = v372->v_grav_speed.z * v46;
                    v337 = TIM_gf_dt;
                    v252 = v399 * TIM_gf_dt;
                    v253 = v400 * TIM_gf_dt;
                    v254 = v401 * TIM_gf_dt;
                    v45 = &v372->v_grav_pos;
                    v372->v_grav_pos.x = v252 + v372->v_grav_pos.x;
                    v45->y = v253 + v45->y;
                    v45->z = v254 + v45->z;
                    v187 = 0;
                    v188 = 0;
                    v189 = v372->des_f_air_gravity;
                    v407 = TIM_gf_dt;
                    v319 = 0.0f * TIM_gf_dt;
                    v320 = v319;
                    v321 = v189 * TIM_gf_dt;
                    v44 = &v372->v_grav_pos;
                    v372->v_grav_pos.x = v319 + v372->v_grav_pos.x;
                    v44->y = v320 + v44->y;
                    v44->z = v321 + v44->z;
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v129);
                    MATH_SubstractVec(&v372->v_grav_pos, &v372->v_grav_pos, &v129);
                    AI_EvalFunc_MATHVecSetNormVector_C(&v372->v_grav_pos, v372->des_f_dist_centre_grav);
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v270);
                    v43 = &v372->v_grav_pos;
                    v372->v_grav_pos.x = v270.x + v372->v_grav_pos.x;
                    v43->y = v270.y + v43->y;
                    v43->z = v270.z + v43->z;
                    MATH_SubstractVec(&v201, &v372->v_grav_pos, &v236);
                    v142 = TIM_gf_dt;
                    MATH_VecMultNormScalar(&v339, &v201, TIM_gf_dt);
                    v372->v_grav_speed = v339;
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v265);
                    MATH_SubstractVec(&v199, &v265, &v372->v_grav_pos);
                    AI_EvalFunc_MATHVecNormalize_C(&v199, &v136);
                    v281 = v136;
                    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v338);
                    v223 = TIM_gf_dt;
                    v19 = 8.0f * TIM_gf_dt;
                    AI_EvalFunc_MATHVecBlendRotate_C(&v338, &v281, v19, &v161);
                    v281 = v161;
                }
            }
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v359);
            AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v359, &v281);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v372->v_last_pos);
            v15 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
            v372->f_last_speed = (float)v15;
            v37 = v206;
            if (v206)
            {
                if ((unsigned int)v206 <= 2)
                    v37 = AI_MainActor[(uint32_t)v206 - 1];
                AI_EvalFunc_DYNSpeedGetVector_C(v37, &v372->v_last_speed);
            }
            else
            {
                AI_EvalFunc_DYNSpeedGetVector_C(0, &v372->v_last_speed);
            }
            result = 1000;
        }
        else
        {
            result = v302;
        }
    }
    return result;
}

int Objet_Flottant_OF_afterrec()
{
    BOOL v0; // eax
    double v1; // st7
    int result; // eax
    tScriptStruct_ObjetFlottant* v3; // [esp+0h] [ebp-120h]
    Vec3* v4; // [esp+4h] [ebp-11Ch]
    float v5; // [esp+30h] [ebp-F0h]
    tGameObjectMain* v6; // [esp+38h] [ebp-E8h]
    Vec3* v7; // [esp+3Ch] [ebp-E4h]
    tGameObjectMain* v8; // [esp+58h] [ebp-C8h]
    tGameObjectMain* v9; // [esp+60h] [ebp-C0h]
    Vec3 v10; // [esp+64h] [ebp-BCh] BYREF
    int v11; // [esp+70h] [ebp-B0h]
    Vec3 a3; // [esp+74h] [ebp-ACh] BYREF
    tGameObjectMain* v13; // [esp+84h] [ebp-9Ch]
    int v14; // [esp+88h] [ebp-98h]
    Vec3 v15; // [esp+8Ch] [ebp-94h] BYREF
    Vec3 v16; // [esp+98h] [ebp-88h] BYREF
    Vec3 v17; // [esp+A4h] [ebp-7Ch] BYREF
    Vec3 v19; // [esp+B4h] [ebp-6Ch] BYREF
    Vec3 v20; // [esp+C0h] [ebp-60h] BYREF
    Vec3 a2; // [esp+CCh] [ebp-54h] BYREF
    Vec3 v22; // [esp+D8h] [ebp-48h] BYREF
    Vec3 v23; // [esp+E4h] [ebp-3Ch] BYREF
    int v24; // [esp+F0h] [ebp-30h]
    float v25; // [esp+F4h] [ebp-2Ch]
    BOOL v26; // [esp+F8h] [ebp-28h]
    tScriptStruct_ObjetFlottant* v27; // [esp+100h] [ebp-20h]
    Vec3 v28; // [esp+104h] [ebp-1Ch]
    float v29; // [esp+110h] [ebp-10h]
    float v30; // [esp+114h] [ebp-Ch]
    float v31; // [esp+118h] [ebp-8h]
    int v32; // [esp+11Ch] [ebp-4h]

    v23.x = 1.0;
    v23.y = 0.0;
    v23.z = 0.0;
    v19.x = 1.0;
    v19.y = 0.0;
    v19.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_ObjetFlottant*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v27 = v3;
    v13 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Objet_Flottant_OF_afterrec_Objet_Flottant_OF_exec_GFX_Fire = (int)AI_ul_RealResolveOneRefFunction(0x72005E12u);
        fct_Objet_Flottant_OF_afterrec_Objet_Flottant_OF_exec_GFX_Smoke = (int)AI_ul_RealResolveOneRefFunction(0x72005E11u);
        Objet_Flottant_OF_afterrec_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_afterrec_univVars_Special_vectors,
            (void*)Objet_Flottant_OF_afterrec_univVars,
            5260);
    }
    if (v27->i_expulse_special)
    {
        if (v27->f_time_air == 0.0)
        {
            if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x80))
            {
                v13 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 0x80);
                if (v27->i_flag_ejection_allowed)
                {
                    v9 = v13;
                    if (v13)
                    {
                        if ((unsigned int)v13 <= 2)
                            v9 = AI_MainActor[(uint32_t)v13 - 1];
                        v0 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v9, 4);
                    }
                    else
                    {
                        v0 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 4);
                    }
                    v26 = v0;
                    if (v0)
                    {
                        v27->f_time_air = TIM_gf_MainClock - TIM_gf_LockTime;
                        v11 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(0, -1.0);
                        v8 = v13;
                        if (v13)
                        {
                            if ((unsigned int)v13 <= 2)
                                v8 = AI_MainActor[(uint32_t)v13 - 1];
                            AI_EvalFunc_DYNSpeedGetVector_C(v8, &a2);
                        }
                        else
                        {
                            AI_EvalFunc_DYNSpeedGetVector_C(0, &a2);
                        }
                        v23 = a2;
                        v28.x = -a2.x;
                        v28.y = -a2.y;
                        v28.z = -a2.z;
                        v7 = &v27->v_grav_speed;
                        v27->v_grav_speed.x = -a2.x;
                        v7->y = v28.y;
                        v7->z = v28.z;
                        v23.x = v23.x + v23.x;
                        v23.y = v23.y + v23.y;
                        v23.z = v23.z + v23.z;
                        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v15);
                        v6 = v13;
                        if (v13)
                        {
                            if ((unsigned int)v13 <= 2)
                                v6 = AI_MainActor[(uint32_t)v13 - 1];
                            v1 = AI_EvalFunc_DYNSpeedGet_C(v6);
                        }
                        else
                        {
                            v1 = AI_EvalFunc_DYNSpeedGet_C(0);
                        }
                        v25 = (float)v1;
                        v5 = v25 * 0.75f;
                        v29 = v15.x * v5;
                        v30 = v15.y * v5;
                        v31 = v15.z * v5;
                        v23.x = v23.x + v29;
                        v23.y = v23.y + v30;
                        v23.z = v23.z + v31;
                        v16.x = 0.0;
                        v16.y = 0.0;
                        v16.z = v27->des_f_base_gravity;
                        AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v16);
                        v24 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v23);
                    }
                }
            }
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else if (v27->i_optim_VF)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        v14 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Objet_Flottant_OF_afterrec_Objet_Flottant_OF_exec_GFX_Fire);
        if (v14 == 1000)
        {
            v32 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Objet_Flottant_OF_afterrec_Objet_Flottant_OF_exec_GFX_Smoke);
            if (v32 == 1000)
            {
                if (v27->des_mobilite)
                {
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v22);
                    a3 = v22;
                    AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &v17);
                    v19.x = v17.x;
                    v19.y = v17.y;
                    v19.z = a3.z;
                    AI_EvalFunc_OBJSetPos_C_CURRENT(&v19);
                }
                if (v27->GFX_Airtrace != -1)
                {
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v20);
                    a3 = v20;
                    a3.z = v27->f_last_valid_water_Z + 0.2f;
                    AI_EvalFunc_GFXSetv_C_CURRENT(v27->GFX_Airtrace, 12206, &a3);
                }
                if (v27->SV_objet_position_ID > 0)
                {
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v10);
                    a3 = v10;
                    if (v27->i_flag_in_water)
                        a3.z = v27->f_last_valid_water_Z + 1.2f;
                    v4 = (Vec3*)AI_EvalVarArray_C3(
                        (char*)Objet_Flottant_OF_afterrec_univVars_Special_vectors,
                        v27->SV_objet_position_ID);
                    *v4 = a3;
                }
                result = 1000;
            }
            else
            {
                result = v32;
            }
        }
        else
        {
            result = v14;
        }
    }
    return result;
}

int Objet_Flottant_OF_Message_Read()
{
    int v0; // eax
    BOOL v1; // eax
    BOOL v2; // eax
    char* v3; // eax
    char* v4; // eax
    tGameObjectMain* v5; // eax
    tGameObjectMain* v6; // eax
    tGameObjectMain* v7; // eax
    tGameObjectMain* v8; // eax
    float v10; // [esp+4h] [ebp-414h]
    tScriptStruct_ObjetFlottant* v11; // [esp+8h] [ebp-410h]
    tGameObjectMain* v12; // [esp+10h] [ebp-408h]
    tGameObjectMain* v13; // [esp+18h] [ebp-400h]
    tGameObjectMain* v14; // [esp+20h] [ebp-3F8h]
    tGameObjectMain* v15; // [esp+24h] [ebp-3F4h]
    tGameObjectMain* v16; // [esp+28h] [ebp-3F0h]
    tGameObjectMain* v17; // [esp+2Ch] [ebp-3ECh]
    tGameObjectMain* v18; // [esp+30h] [ebp-3E8h]
    tAI* v19; // [esp+34h] [ebp-3E4h]
    tGameObjectMain* v20; // [esp+38h] [ebp-3E0h]
    tGameObjectMain* v21; // [esp+3Ch] [ebp-3DCh]
    tGameObjectMain* v22; // [esp+44h] [ebp-3D4h]
    tAI* v23; // [esp+50h] [ebp-3C8h]
    tGameObjectMain* v24; // [esp+54h] [ebp-3C4h]
    tGameObjectMain* v25; // [esp+58h] [ebp-3C0h]
    tAI* v26; // [esp+5Ch] [ebp-3BCh]
    tGameObjectMain* v27; // [esp+60h] [ebp-3B8h]
    tGameObjectMain* v28; // [esp+64h] [ebp-3B4h]
    tAI* v29; // [esp+68h] [ebp-3B0h]
    tGameObjectMain* v30; // [esp+6Ch] [ebp-3ACh]
    tGameObjectMain* v31; // [esp+70h] [ebp-3A8h]
    tAI* v32; // [esp+74h] [ebp-3A4h]
    tGameObjectMain* v33; // [esp+78h] [ebp-3A0h]
    tGameObjectMain* v34; // [esp+7Ch] [ebp-39Ch]
    tAI* v35; // [esp+80h] [ebp-398h]
    tGameObjectMain* v36; // [esp+84h] [ebp-394h]
    tGameObjectMain* v37; // [esp+88h] [ebp-390h]
    tAI* v38; // [esp+8Ch] [ebp-38Ch]
    tGameObjectMain* v39; // [esp+90h] [ebp-388h]
    tGameObjectMain* v40; // [esp+94h] [ebp-384h]
    tGameObjectMain* v41; // [esp+9Ch] [ebp-37Ch]
    tGameObjectMain* v42; // [esp+B0h] [ebp-368h]
    tGameObjectMain* v43; // [esp+B8h] [ebp-360h]
    tGameObjectMain* v44; // [esp+C0h] [ebp-358h]
    tGameObjectMain* v45; // [esp+D4h] [ebp-344h]
    tGameObjectMain* v46; // [esp+D8h] [ebp-340h]
    tGameObjectMain* v47; // [esp+DCh] [ebp-33Ch]
    tGameObjectMain* v48; // [esp+E4h] [ebp-334h]
    tGameObjectMain* v49; // [esp+E8h] [ebp-330h]
    tGameObjectMain* v50; // [esp+ECh] [ebp-32Ch]
    tGameObjectMain* v51; // [esp+F4h] [ebp-324h]
    tGameObjectMain* v52; // [esp+FCh] [ebp-31Ch]
    tGameObjectMain* v53; // [esp+100h] [ebp-318h]
    tGameObjectMain* v54; // [esp+104h] [ebp-314h]
    tGameObjectMain* v55; // [esp+108h] [ebp-310h]
    tGameObjectMain* v56; // [esp+10Ch] [ebp-30Ch]
    float v57; // [esp+110h] [ebp-308h]
    tGameObjectMain* v58; // [esp+114h] [ebp-304h]
    tGameObjectMain* v59; // [esp+118h] [ebp-300h]
    uint32_t* v60; // [esp+124h] [ebp-2F4h]
    uint32_t* v61; // [esp+128h] [ebp-2F0h]
    tGameObjectMain* v62; // [esp+134h] [ebp-2E4h]
    tGameObjectMain* v63; // [esp+138h] [ebp-2E0h]
    uint32_t* v64; // [esp+140h] [ebp-2D8h]
    uint32_t* v65; // [esp+144h] [ebp-2D4h]
    tGameObjectMain* v66; // [esp+150h] [ebp-2C8h]
    tGameObjectMain* v67; // [esp+154h] [ebp-2C4h]
    tGameObjectMain* v68; // [esp+15Ch] [ebp-2BCh]
    float v69; // [esp+160h] [ebp-2B8h]
    tGameObjectMain* v70; // [esp+174h] [ebp-2A4h]
    tGameObjectMain* v71; // [esp+178h] [ebp-2A0h]
    tGameObjectMain* v72; // [esp+17Ch] [ebp-29Ch]
    tGameObjectMain* v73; // [esp+184h] [ebp-294h]
    tGameObjectMain* v74; // [esp+18Ch] [ebp-28Ch]
    tGameObjectMain* v75; // [esp+190h] [ebp-288h]
    tGameObjectMain* v76; // [esp+194h] [ebp-284h]
    tGameObjectMain* v77; // [esp+198h] [ebp-280h]
    tGameObjectMain* v78; // [esp+19Ch] [ebp-27Ch]
    int a4; // [esp+1ACh] [ebp-26Ch]
    int v80; // [esp+1B0h] [ebp-268h]
    void* v81; // [esp+1C0h] [ebp-258h]
    Vec3 v82; // [esp+1D4h] [ebp-244h] BYREF
    Vec3 v83; // [esp+1E0h] [ebp-238h] BYREF
    unsigned int v84; // [esp+1ECh] [ebp-22Ch]
    BOOL v85; // [esp+1F0h] [ebp-228h]
    Vec3 a3; // [esp+1F4h] [ebp-224h] BYREF
    int v87; // [esp+200h] [ebp-218h]
    int v88; // [esp+204h] [ebp-214h]
    float v89; // [esp+208h] [ebp-210h]
    BOOL v90; // [esp+210h] [ebp-208h]
    int v91; // [esp+214h] [ebp-204h]
    int v92; // [esp+218h] [ebp-200h]
    int v93; // [esp+21Ch] [ebp-1FCh]
    Vec3 pos; // [esp+220h] [ebp-1F8h] BYREF
    tGameObjectMain* v95; // [esp+22Ch] [ebp-1ECh]
    float zoom; // [esp+234h] [ebp-1E4h]
    int v97; // [esp+258h] [ebp-1C0h]
    int v98; // [esp+25Ch] [ebp-1BCh]
    tMessage v99; // [esp+260h] [ebp-1B8h] BYREF
    Vec3 v100; // [esp+294h] [ebp-184h] BYREF
    float v101; // [esp+2A0h] [ebp-178h]
    int i; // [esp+2A4h] [ebp-174h]
    tGameObjectMain* v103; // [esp+2ACh] [ebp-16Ch]
    BOOL v104; // [esp+2B0h] [ebp-168h]
    Vec3 v105; // [esp+2B4h] [ebp-164h] BYREF
    tGameObjectMain* v106; // [esp+2C0h] [ebp-158h]
    float v107; // [esp+2C8h] [ebp-150h]
    tGameObjectMain* v108; // [esp+2CCh] [ebp-14Ch]
    tGameObjectMain* v109; // [esp+2D0h] [ebp-148h]
    void* v110; // [esp+2D4h] [ebp-144h]
    Vec3 v111; // [esp+2E4h] [ebp-134h] BYREF
    Vec3 v112; // [esp+300h] [ebp-118h] BYREF
    Vec3 v113; // [esp+30Ch] [ebp-10Ch] BYREF
    Vec3 v114; // [esp+318h] [ebp-100h] BYREF
    tMessage Src; // [esp+324h] [ebp-F4h] BYREF
    Vec3 v116; // [esp+35Ch] [ebp-BCh] BYREF
    Vec3 v117; // [esp+368h] [ebp-B0h] BYREF
    int v119; // [esp+378h] [ebp-A0h]
    int v120; // [esp+380h] [ebp-98h]
    tMessage v121; // [esp+384h] [ebp-94h] BYREF
    int v122; // [esp+3B8h] [ebp-60h]
    Vec3 v123; // [esp+3BCh] [ebp-5Ch] BYREF
    tScriptStruct_ObjetFlottant* v124; // [esp+3C8h] [ebp-50h]
    Vec3 v125; // [esp+3CCh] [ebp-4Ch] BYREF
    Vec3 v126; // [esp+3D8h] [ebp-40h] BYREF
    float v127; // [esp+3E4h] [ebp-34h]
    tGameObjectMain* v128; // [esp+3E8h] [ebp-30h]
    float v129; // [esp+3F0h] [ebp-28h]
    float v131; // [esp+3F8h] [ebp-20h]
    float v132; // [esp+3FCh] [ebp-1Ch]
    int v133; // [esp+400h] [ebp-18h]
    int v134; // [esp+404h] [ebp-14h]
    float v135; // [esp+408h] [ebp-10h]
    int j; // [esp+410h] [ebp-8h]
    int v137; // [esp+414h] [ebp-4h]

    v100.x = 1.0;
    v100.y = 0.0;
    v100.z = 0.0;
    v113.x = 1.0;
    v113.y = 0.0;
    v113.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v11 = (tScriptStruct_ObjetFlottant*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v11 = 0;
    v124 = v11;
    i = 0;
    v106 = 0;
    v119 = 0;
    v128 = 0;
    v95 = 0;
    v84 = 0;
    v120 = 0;
    j = 0;
    v98 = 0;
    zoom = 0.0;
    v129 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fctvar_Objet_Flottant_OF_Message_Read_DUP_HovercraftShoot_gao = AI_ul_RealResolveOneRef(42, 0x49020714u);
        Objet_Flottant_OF_Message_Read_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Message_Read_GST_Global_gao_o_targets,
            (void*)Objet_Flottant_OF_Message_Read_GST_Global_gao,
            964);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Message_Read_GST_Global_gao_i_targets_nb,
            (void*)Objet_Flottant_OF_Message_Read_GST_Global_gao,
            2572);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Message_Read_GST_Global_gao_v_targets_offset,
            (void*)Objet_Flottant_OF_Message_Read_GST_Global_gao,
            1368);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Message_Read_GST_Global_gao_o_selected_target,
            (void*)Objet_Flottant_OF_Message_Read_GST_Global_gao,
            2580);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Message_Read_GST_Global_gao_i_selected_target_nb,
            (void*)Objet_Flottant_OF_Message_Read_GST_Global_gao,
            2748);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Message_Read_GST_Global_gao_v_selected_target_offset,
            (void*)Objet_Flottant_OF_Message_Read_GST_Global_gao,
            2624);
        Objet_Flottant_OF_Message_Read__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Objet_Flottant_OF_Message_Read_INV_OBJ15_Starkos_Vehicule_gao = AI_ul_RealResolveOneRefObject(0x26003339u);
        Objet_Flottant_OF_Message_Read_INV_OBJ37_HoverBoost_gao = AI_ul_RealResolveOneRefObject(0x49021D76u);
        Objet_Flottant_OF_Message_Read_INV_OBJ45_Perle_gao = AI_ul_RealResolveOneRefObject(0x49022DFEu);
        Objet_Flottant_OF_Message_Read_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Message_Read_univVars_Perso_ID_actor,
            (void*)Objet_Flottant_OF_Message_Read_univVars,
            988);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Message_Read_univVars_Perso_ID_Energie_courante,
            (void*)Objet_Flottant_OF_Message_Read_univVars,
            1148);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Message_Read_univVars_Perso_ID_Energie_max,
            (void*)Objet_Flottant_OF_Message_Read_univVars,
            1096);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Message_Read_univVars_objet_inventaire,
            (void*)Objet_Flottant_OF_Message_Read_univVars,
            1480);
        Objet_Flottant_OF_Message_Read_INV_OBJ45_PerleHalo_gao = AI_ul_RealResolveOneRefObject(0x49022DFFu);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Message_Read_univVars_objet_liste_gao,
            (void*)Objet_Flottant_OF_Message_Read_univVars,
            1276);
        AI2C_ResolveBuffer(
            &Objet_Flottant_OF_Message_Read_univVars_Flag_speciaux_sauve,
            (void*)Objet_Flottant_OF_Message_Read_univVars,
            4488);
        Objet_Flottant_OF_Message_Read_Light_PAF_gao = AI_ul_RealResolveOneRefObject(0x9E005BDD);
        Objet_Flottant_OF_Message_Read_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
        fct_Objet_Flottant_OF_Message_Read_Objet_Flottant_OF_exec_FX_Smoke = (int)AI_ul_RealResolveOneRefFunction(0x260036E1u);
        Objet_Flottant_OF_Message_Read_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        fct_Objet_Flottant_OF_Message_Read_Objet_Flottant_OF_Reward = (int)AI_ul_RealResolveOneRefFunction(0x260036C7u);
        fct_Objet_Flottant_OF_Message_Read_Objet_Flottant_OF_NoReward = (int)AI_ul_RealResolveOneRefFunction(0x26003709u);
    }
    v78 = (tGameObjectMain*)v124->o_shoot_hover;
    if (v78)
    {
        if ((unsigned int)v78 <= 2)
            v78 = AI_MainActor[(uint32_t)v78 - 1];
        v77 = v78;
    }
    else
    {
        v77 = 0;
    }
    if (v77)
    {
        for (i = 0; i < 5; ++i)
        {
            v128 = (tGameObjectMain*)v124->o_explode_me[i];
            v76 = v128;
            if (v128)
            {
                if ((unsigned int)v128 <= 2)
                    v76 = AI_MainActor[(uint32_t)v128 - 1];
                v75 = v76;
            }
            else
            {
                v75 = 0;
            }
            if (v75)
            {
                v74 = v128;
                if (v128)
                {
                    if ((unsigned int)v128 <= 2)
                        v74 = AI_MainActor[(uint32_t)v128 - 1];
                    v0 = AI_EvalFunc_COLPivotBVCollide_C(AI_gpst_CurrentGameObject, v74);
                }
                else
                {
                    v0 = AI_EvalFunc_COLPivotBVCollide_C(AI_gpst_CurrentGameObject, 0);
                }
                v92 = v0;
                if (v0)
                {
                    v128 = AI_gpst_CurrentGameObject;
                    Src.msgID = 37;
                    Src.gao = (tGameObjectMain*)v124->o_shoot_hover;
                    Src.gao2 = 0;
                    Src.Int1 = 71;
                    Src.Int2 = 0;
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&Src.v1);
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&Src.v2);
                    Src.f2 = 40.0;
                    Src.f1 = 1000.0;
                    v73 = v128;
                    if (v128)
                    {
                        if ((unsigned int)v128 <= 2)
                            v73 = AI_MainActor[(uint32_t)v128 - 1];
                        AI_EvalFunc_MSGSend_C(v73, &Src);
                    }
                    else
                    {
                        AI_EvalFunc_MSGSend_C(0, &Src);
                    }
                    break;
                }
            }
        }
    }
    v81 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&Src, v81, sizeof(Src));
    while (Src.msgID)
    {
        v95 = Src.gao;
        if (Src.msgID == 37)
        {
            v119 = Src.Int1 & 7;
            v120 = Src.Int1 & 0x60;
            v72 = v95;
            if (v95)
            {
                if ((unsigned int)v95 <= 2)
                    v72 = AI_MainActor[(uint32_t)v95 - 1];
                v71 = v72;
            }
            else
            {
                v71 = 0;
            }
            if (v71)
            {
                v70 = v95;
                if (v95)
                {
                    if ((unsigned int)v95 <= 2)
                        v70 = AI_MainActor[(uint32_t)v95 - 1];
                    v1 = AI_EvalFunc_AIIsModel_C(v70, (tAIModel*)fctvar_Objet_Flottant_OF_Message_Read_DUP_HovercraftShoot_gao);
                }
                else
                {
                    v1 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_Objet_Flottant_OF_Message_Read_DUP_HovercraftShoot_gao);
                }
                v104 = v1;
                if (v1)
                {
                    if (AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 4))
                    {
                        v87 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(3, -1.0);
                        v100.x = Src.v1.x;
                        v100.y = Src.v1.y;
                        v100.z = 0.0;
                        v129 = MATH_VecLen(&v100);
                        if (v129 != 0.0f)
                        {
                            v69 = 1.0f / v129;
                            v100.x = v100.x * v69;
                            v100.y = v100.y * v69;
                            v100.z = v100.z * v69;
                            v100.x = v100.x * 25.0f;
                            v100.y = v100.y * 25.0f;
                            v100.z = v100.z * 25.0f;
                            AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v125);
                            v100.x = v100.x + v125.x;
                            v100.y = v100.y + v125.y;
                            v100.z = v100.z + v125.z;
                            v93 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v100);
                            v107 = AI_EvalFunc_MATHRandFloat_C(-1.0, 1.0);
                            v124->f_rotation_speed = v107 + v124->f_rotation_speed;
                        }
                    }
                    if (v124->des_i_settings != 1)
                        goto LABEL_60;
                    v68 = v95;
                    if (v95)
                    {
                        if ((unsigned int)v95 <= 2)
                            v68 = AI_MainActor[(uint32_t)v95 - 1];
                        v2 = AI_EvalFunc_OBJ_CapaTest_C(v68, 4);
                    }
                    else
                    {
                        v2 = AI_EvalFunc_OBJ_CapaTest_C(0, 4);
                    }
                    v85 = v2;
                    if (v2)
                    {
                        v119 = 0;
                        v120 = 0;
                    }
                    else
                    {
                    LABEL_60:
                        if (v119 >= v124->des_resistance && v119 > 5)
                            v124->des_energie = 2;
                    }
                }
            }
            if (v119 > 2)
                v124->f_time_paf = TIM_gf_MainClock - TIM_gf_LockTime;
            if (v124->des_mode && v120 && v119 >= v124->des_resistance)
            {
                if (v124->des_i_settings)
                {
                    v109 = AI_gpst_CurrentGameObject;
                    v67 = AI_gpst_CurrentGameObject;
                    if (AI_gpst_CurrentGameObject)
                    {
                        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                            v67 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                        v66 = v67;
                    }
                    else
                    {
                        v66 = 0;
                    }
                    v91 = AI_EvalFunc_ARRObjSearch_C(
                        (tGameObjectMain**)(Objet_Flottant_OF_Message_Read_GST_Global_gao_o_targets + 4),
                        100,
                        v66);
                    v98 = v91;
                    if (v91 != -1)
                    {
                        *(uint32_t*)(Objet_Flottant_OF_Message_Read_GST_Global_gao_o_targets + 4 * v98 + 4) = *(uint32_t*)(Objet_Flottant_OF_Message_Read_GST_Global_gao_o_targets + 4 * *(uint32_t*)Objet_Flottant_OF_Message_Read_GST_Global_gao_i_targets_nb);
                        v64 = (uint32_t*)AI_EvalVarArray_C3(
                            (char*)Objet_Flottant_OF_Message_Read_GST_Global_gao_v_targets_offset,
                            *(uint32_t*)Objet_Flottant_OF_Message_Read_GST_Global_gao_i_targets_nb - 1);
                        v65 = (uint32_t*)AI_EvalVarArray_C3(
                            (char*)Objet_Flottant_OF_Message_Read_GST_Global_gao_v_targets_offset,
                            v98);
                        *v65 = *v64;
                        v65[1] = v64[1];
                        v65[2] = v64[2];
                        *(uint32_t*)(Objet_Flottant_OF_Message_Read_GST_Global_gao_o_targets
                            + 4 * *(uint32_t*)Objet_Flottant_OF_Message_Read_GST_Global_gao_i_targets_nb) = 0;
                        *(uint32_t*)AI_EvalVarArray_C3(
                            (char*)Objet_Flottant_OF_Message_Read_GST_Global_gao_v_targets_offset,
                            *(uint32_t*)Objet_Flottant_OF_Message_Read_GST_Global_gao_i_targets_nb - 1) = 0;
                        *(uint32_t*)(AI_EvalVarArray_C3(
                            (char*)Objet_Flottant_OF_Message_Read_GST_Global_gao_v_targets_offset,
                            *(uint32_t*)Objet_Flottant_OF_Message_Read_GST_Global_gao_i_targets_nb - 1)
                            + 4) = 0;
                        *(uint32_t*)(AI_EvalVarArray_C3(
                            (char*)Objet_Flottant_OF_Message_Read_GST_Global_gao_v_targets_offset,
                            *(uint32_t*)Objet_Flottant_OF_Message_Read_GST_Global_gao_i_targets_nb - 1)
                            + 8) = 0;
                        --* (uint32_t*)Objet_Flottant_OF_Message_Read_GST_Global_gao_i_targets_nb;
                    }
                    v63 = AI_gpst_CurrentGameObject;
                    if (AI_gpst_CurrentGameObject)
                    {
                        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                            v63 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                        v62 = v63;
                    }
                    else
                    {
                        v62 = 0;
                    }
                    v97 = AI_EvalFunc_ARRObjSearch_C(
                        (tGameObjectMain**)(Objet_Flottant_OF_Message_Read_GST_Global_gao_o_selected_target + 4),
                        10,
                        v62);
                    v98 = v97;
                    if (v97 != -1)
                    {
                        *(uint32_t*)(Objet_Flottant_OF_Message_Read_GST_Global_gao_o_selected_target + 4 * v98 + 4) = *(uint32_t*)(Objet_Flottant_OF_Message_Read_GST_Global_gao_o_selected_target + 4 * *(uint32_t*)Objet_Flottant_OF_Message_Read_GST_Global_gao_i_selected_target_nb);
                        v60 = (uint32_t*)AI_EvalVarArray_C3(
                            (char*)Objet_Flottant_OF_Message_Read_GST_Global_gao_v_selected_target_offset,
                            *(uint32_t*)Objet_Flottant_OF_Message_Read_GST_Global_gao_i_selected_target_nb - 1);
                        v61 = (uint32_t*)AI_EvalVarArray_C3(
                            (char*)Objet_Flottant_OF_Message_Read_GST_Global_gao_v_selected_target_offset,
                            v98);
                        *v61 = *v60;
                        v61[1] = v60[1];
                        v61[2] = v60[2];
                        *(uint32_t*)(Objet_Flottant_OF_Message_Read_GST_Global_gao_o_selected_target
                            + 4 * *(uint32_t*)Objet_Flottant_OF_Message_Read_GST_Global_gao_i_selected_target_nb) = 0;
                        *(uint32_t*)AI_EvalVarArray_C3(
                            (char*)Objet_Flottant_OF_Message_Read_GST_Global_gao_v_selected_target_offset,
                            *(uint32_t*)Objet_Flottant_OF_Message_Read_GST_Global_gao_i_selected_target_nb - 1) = 0;
                        *(uint32_t*)(AI_EvalVarArray_C3(
                            (char*)Objet_Flottant_OF_Message_Read_GST_Global_gao_v_selected_target_offset,
                            *(uint32_t*)Objet_Flottant_OF_Message_Read_GST_Global_gao_i_selected_target_nb - 1)
                            + 4) = 0;
                        *(uint32_t*)(AI_EvalVarArray_C3(
                            (char*)Objet_Flottant_OF_Message_Read_GST_Global_gao_v_selected_target_offset,
                            *(uint32_t*)Objet_Flottant_OF_Message_Read_GST_Global_gao_i_selected_target_nb - 1)
                            + 8) = 0;
                        --* (uint32_t*)Objet_Flottant_OF_Message_Read_GST_Global_gao_i_selected_target_nb;
                    }
                }
                v124->des_energie -= 2;
                if (v124->des_energie <= 0 || v119 > 4)
                {
                    v124->des_energie = 0;
                    v128 = (tGameObjectMain*)Objet_Flottant_OF_Message_Read__PJ_Sally_gao;
                    zoom = 0.0;
                    if (v124->des_i_reward_force)
                    {
                        switch (v124->des_i_reward_force)
                        {
                        case 1:
                            v128 = (tGameObjectMain*)Objet_Flottant_OF_Message_Read_INV_OBJ15_Starkos_Vehicule_gao;
                            v124->i_id_reward = 15;
                            zoom = 10.0;
                            break;
                        case 2:
                            v128 = (tGameObjectMain*)Objet_Flottant_OF_Message_Read_INV_OBJ37_HoverBoost_gao;
                            v124->i_id_reward = 37;
                            zoom = 10.0;
                            break;
                        case 4:
                            v128 = (tGameObjectMain*)Objet_Flottant_OF_Message_Read_INV_OBJ45_Perle_gao;
                            v124->i_id_reward = 45;
                            zoom = 10.0;
                            break;
                        case 0xA:
                            v128 = 0;
                            break;
                        }
                    }
                    else
                    {
                        v108 = *AI_MainActor;
                        v59 = v108;
                        if (v108)
                        {
                            if ((unsigned int)v108 <= 2)
                                v59 = AI_MainActor[(uint32_t)v108 - 1];
                            v58 = v59;
                        }
                        else
                        {
                            v58 = 0;
                        }
                        if (*(tGameObjectMain**)(Objet_Flottant_OF_Message_Read_univVars_Perso_ID_actor + 44) == v58)
                        {
                            a4 = 10;
                            v3 = AI_EvalVarArray_C2((char*)Objet_Flottant_OF_Message_Read_univVars_Perso_ID_Energie_max, 10);
                        }
                        else
                        {
                            a4 = 9;
                            v3 = AI_EvalVarArray_C2((char*)Objet_Flottant_OF_Message_Read_univVars_Perso_ID_Energie_max, 9);
                        }
                        v57 = *(float*)v3 * 0.2f;
                        if (v57 > 20.0f)
                            v10 = *(float*)v3 * 0.2f;
                        else
                            v10 = 20.0f;
                        v135 = v10;
                        v4 = AI_EvalVarArray_C2((char*)Objet_Flottant_OF_Message_Read_univVars_Perso_ID_Energie_courante, a4);
                        if (*(float*)v4 > (double)v135
                            || *(int*)AI_EvalVarArray2_C(1, (char*)Objet_Flottant_OF_Message_Read_univVars_objet_inventaire, 15, a4) >= 2)
                        {
                            if (*(int*)AI_EvalVarArray2_C(
                                1,
                                (char*)Objet_Flottant_OF_Message_Read_univVars_objet_inventaire,
                                37,
                                a4) < 3)
                            {
                                v128 = (tGameObjectMain*)Objet_Flottant_OF_Message_Read_INV_OBJ37_HoverBoost_gao;
                                v124->i_id_reward = 37;
                                zoom = 10.0;
                            }
                        }
                        else
                        {
                            v128 = (tGameObjectMain*)Objet_Flottant_OF_Message_Read_INV_OBJ15_Starkos_Vehicule_gao;
                            v124->i_id_reward = 15;
                            zoom = 10.0;
                        }
                    }
                    v56 = v128;
                    if (v128)
                    {
                        if ((unsigned int)v128 <= 2)
                            v56 = AI_MainActor[(uint32_t)v128 - 1];
                        v55 = v56;
                    }
                    else
                    {
                        v55 = 0;
                    }
                    if (v55)
                    {
                        v54 = v128;
                        if (v128)
                        {
                            if ((unsigned int)v128 <= 2)
                                v54 = AI_MainActor[(uint32_t)v128 - 1];
                            v53 = v54;
                        }
                        else
                        {
                            v53 = 0;
                        }
                        if (v53 == (tGameObjectMain*)Objet_Flottant_OF_Message_Read__PJ_Sally_gao)
                        {
                            for (j = 0; j < v124->i_max_gen_materia; ++j)
                            {
                                AI_EvalFunc_OBJGetPos_C_CURRENT(&v123);
                                v100 = v123;
                                if (v124->i_flag_in_water)
                                {
                                    v101 = AI_EvalFunc_MATHRandFloat_C(2.5, 3.5);
                                    v100.z = v124->f_last_valid_water_Z + v101;
                                }
                                else
                                {
                                    v100.z = AI_EvalFunc_MATHRandFloat_C(2.5, 3.5) + v100.z;
                                }
                                v127 = AI_EvalFunc_MATHRandFloat_C(-0.5, 0.5);
                                v89 = AI_EvalFunc_MATHRandFloat_C(-0.5, 0.5);
                                v131 = v127;
                                v132 = v89;
                                v133 = 0;
                                v100.x = v100.x + v127;
                                v100.y = v100.y + v89;
                                v100.z = v100.z + 0.0f;
                                v106 = *(tGameObjectMain**)(Objet_Flottant_OF_Message_Read_univVars_objet_liste_gao + 16);
                                v44 = v106;
                                if (v106)
                                {
                                    if ((unsigned int)v106 <= 2)
                                        v44 = AI_MainActor[(uint32_t)v106 - 1];
                                    v8 = AI_EvalFunc_OBJDuplicate_C(v44, &v100);
                                }
                                else
                                {
                                    v8 = AI_EvalFunc_OBJDuplicate_C(0, &v100);
                                }
                                v84 = (unsigned int)v8;
                                v121.msgID = 1;
                                v121.Int1 = 1;
                                v121.Int2 = 0;
                                v121.f2 = 5.0;
                                v121.f1 = 10.0;
                                v121.v1.x = 0.0;
                                v121.v1.y = 0.0;
                                v121.v1.z = 1.0;
                                v43 = v8;
                                if (v8)
                                {
                                    if ((unsigned int)v8 <= 2)
                                        v43 = AI_MainActor[(uint32_t)v8 - 1];
                                    AI_EvalFunc_MSGSend_C(v43, &v121);
                                }
                                else
                                {
                                    AI_EvalFunc_MSGSend_C(0, &v121);
                                }
                                v42 = (tGameObjectMain*)v84;
                                if (v84)
                                {
                                    if (v84 <= 2)
                                        v42 = AI_MainActor[v84 - 1];
                                    AI_EvalFunc_COLSetActivationSet_C(v42, 0, 1);
                                }
                                else
                                {
                                    AI_EvalFunc_COLSetActivationSet_C(0, 0, 1);
                                }
                            }
                        }
                        else
                        {
                            AI_EvalFunc_OBJGetPos_C_CURRENT(&v111);
                            MATH_VecAddVec(&pos, &v111, &v124->des_v_offset_reward);
                            v52 = v128;
                            if (v128)
                            {
                                if ((unsigned int)v128 <= 2)
                                    v52 = AI_MainActor[(uint32_t)v128 - 1];
                                v5 = AI_EvalFunc_OBJDuplicate_C(v52, &pos);
                            }
                            else
                            {
                                v5 = AI_EvalFunc_OBJDuplicate_C(0, &pos);
                            }
                            v124->o_reward_dup_gao = (int)v5;
                            v51 = (tGameObjectMain*)v124->o_reward_dup_gao;
                            if (v51)
                            {
                                if ((unsigned int)v51 <= 2)
                                    v51 = AI_MainActor[(uint32_t)v51 - 1];
                                AI_EvalFunc_OBJ_ZoomSet_C(v51, zoom);
                            }
                            else
                            {
                                AI_EvalFunc_OBJ_ZoomSet_C(0, zoom);
                            }
                            v50 = v128;
                            if (v128)
                            {
                                if ((unsigned int)v128 <= 2)
                                    v50 = AI_MainActor[(uint32_t)v128 - 1];
                                v49 = v50;
                            }
                            else
                            {
                                v49 = 0;
                            }
                            if (v49 == (tGameObjectMain*)Objet_Flottant_OF_Message_Read_INV_OBJ45_Perle_gao)
                            {
                                AI_EvalFunc_OBJGetPos_C_CURRENT(&v112);
                                MATH_VecAddVec(&v82, &v112, &v124->des_v_offset_reward);
                                v6 = AI_EvalFunc_OBJDuplicate_C(
                                    (tGameObjectMain*)Objet_Flottant_OF_Message_Read_INV_OBJ45_PerleHalo_gao,
                                    &v82);
                                v124->o_reward_dup_gao_extra = (int)v6;
                                v48 = (tGameObjectMain*)v124->o_reward_dup_gao_extra;
                                if (v48)
                                {
                                    if ((unsigned int)v48 <= 2)
                                        v48 = AI_MainActor[(uint32_t)v48 - 1];
                                    AI_EvalFunc_OBJ_ZoomSet_C(v48, zoom);
                                }
                                else
                                {
                                    AI_EvalFunc_OBJ_ZoomSet_C(0, zoom);
                                }
                                v47 = (tGameObjectMain*)v124->o_reward_dup_gao;
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
                                v45 = (tGameObjectMain*)v124->o_reward_dup_gao_extra;
                                if (v45)
                                {
                                    if ((unsigned int)v45 <= 2)
                                        v45 = AI_MainActor[(uint32_t)v45 - 1];
                                    v7 = AI_EvalFunc_OBJ_HierarchySet_C(v45, v46);
                                }
                                else
                                {
                                    v7 = AI_EvalFunc_OBJ_HierarchySet_C(0, v46);
                                }
                                v103 = v7;
                            }
                        }
                    }
                    if (v124->des_SF_mort_ID > 0)
                        AI_EvalFunc_ARRBitSet_C(
                            (int*)(Objet_Flottant_OF_Message_Read_univVars_Flag_speciaux_sauve + 4),
                            v124->des_SF_mort_ID);
                    AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
                    AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
                    AI_EvalFunc_OBJDrawMaskSet_C(AI_gpst_CurrentGameObject, 0, 8);
                    v88 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
                    if (!v124->des_i_GFX_Smoke || v124->des_i_settings == 4)
                    {
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&v116);
                        v126.x = 0.0;
                        v126.y = 0.0;
                        v126.z = 3.0;
                        MATH_VecAddVec(&v105, &v116, &v126);
                        v113 = v105;
                        v106 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)Objet_Flottant_OF_Message_Read_Light_PAF_gao, &v113);
                        v41 = v106;
                        if (v106)
                        {
                            if ((unsigned int)v106 <= 2)
                                v41 = AI_MainActor[(uint32_t)v106 - 1];
                            AI_EvalFunc_OBJ_CapaSet_C(v41, 0x100, 0);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_CapaSet_C(0, 0x100, 0);
                        }
                        if (v106 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                        {
                            v38 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v40 = v106;
                            if (v106)
                            {
                                if ((unsigned int)v106 <= 2)
                                    v40 = AI_MainActor[(uint32_t)v106 - 1];
                                v39 = v40;
                            }
                            else
                            {
                                v39 = 0;
                            }
                            v38 = v39->extended->AIInstance;
                        }
                        ((tScriptStruct_LightPAF*)(v38->instanceVarDataCurrent))->f_far = 65.0;
                        if (v106 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                        {
                            v35 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v37 = v106;
                            if (v106)
                            {
                                if ((unsigned int)v106 <= 2)
                                    v37 = AI_MainActor[(uint32_t)v106 - 1];
                                v36 = v37;
                            }
                            else
                            {
                                v36 = 0;
                            }
                            v35 = v36->extended->AIInstance;
                        }
                        ((tScriptStruct_LightPAF*)(v35->instanceVarDataCurrent))->f_near = 25.0;
                        if (v106 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                        {
                            v32 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v34 = v106;
                            if (v106)
                            {
                                if ((unsigned int)v106 <= 2)
                                    v34 = AI_MainActor[(uint32_t)v106 - 1];
                                v33 = v34;
                            }
                            else
                            {
                                v33 = 0;
                            }
                            v32 = v33->extended->AIInstance;
                        }
                        ((tScriptStruct_LightPAF*)(v32->instanceVarDataCurrent))->i_color = 0x2F2FFA;
                        if (v106 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                        {
                            v29 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v31 = v106;
                            if (v106)
                            {
                                if ((unsigned int)v106 <= 2)
                                    v31 = AI_MainActor[(uint32_t)v106 - 1];
                                v30 = v31;
                            }
                            else
                            {
                                v30 = 0;
                            }
                            v29 = v30->extended->AIInstance;
                        }
                        ((tScriptStruct_LightPAF*)(v29->instanceVarDataCurrent))->f_blend = 40.0;
                        if (v106 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                        {
                            v26 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v28 = v106;
                            if (v106)
                            {
                                if ((unsigned int)v106 <= 2)
                                    v28 = AI_MainActor[(uint32_t)v106 - 1];
                                v27 = v28;
                            }
                            else
                            {
                                v27 = 0;
                            }
                            v26 = v27->extended->AIInstance;
                        }
                        ((tScriptStruct_LightPAF*)(v26->instanceVarDataCurrent))->f_blend2 = 100.0;
                        if (v106 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                        {
                            v23 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v25 = v106;
                            if (v106)
                            {
                                if ((unsigned int)v106 <= 2)
                                    v25 = AI_MainActor[(uint32_t)v106 - 1];
                                v24 = v25;
                            }
                            else
                            {
                                v24 = 0;
                            }
                            v23 = v24->extended->AIInstance;
                        }
                        ((tScriptStruct_LightPAF*)(v23->instanceVarDataCurrent))->i_type = 1;
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&v114);
                        v113 = v114;
                        v106 = AI_EvalFunc_OBJDuplicate_C(
                            (tGameObjectMain*)Objet_Flottant_OF_Message_Read_SFX_explosion_gao,
                            &v113);
                        v99.msgID = 1;
                        v99.Int1 = v124->des_i_explosion_type;
                        v99.f2 = (float)v124->des_i_explosion_size;
                        v99.f1 = 1.0;
                        v99.v1.x = 1.0;
                        v99.v1.y = 0.0;
                        v99.v1.z = 1.0;
                        v22 = v106;
                        if (v106)
                        {
                            if ((unsigned int)v106 <= 2)
                                v22 = AI_MainActor[(uint32_t)v106 - 1];
                            AI_EvalFunc_MSGSend_C(v22, &v99);
                        }
                        else
                        {
                            AI_EvalFunc_MSGSend_C(0, &v99);
                        }
                        if (v106 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                        {
                            v19 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v21 = v106;
                            if (v106)
                            {
                                if ((unsigned int)v106 <= 2)
                                    v21 = AI_MainActor[(uint32_t)v106 - 1];
                                v20 = v21;
                            }
                            else
                            {
                                v20 = 0;
                            }
                            v19 = v20->extended->AIInstance;
                        }
                        ((tScriptStruct_SFXExplosion*)(v19->instanceVarDataCurrent))->des_play_sound = 0;
                    }
                    if (v124->des_i_GFX_Smoke)
                    {
                        v137 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Objet_Flottant_OF_Message_Read_Objet_Flottant_OF_exec_FX_Smoke);
                        if (v137 != 1000)
                            return v137;
                    }
                    if (AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 4))
                    {
                        v83.x = 0.0;
                        v83.y = 0.0;
                        v83.z = 0.0;
                        AI_EvalFunc_DYNTractionSet_C_CURRENT(&v83);
                        v117.x = 0.0;
                        v117.y = 0.0;
                        v117.z = 0.0;
                        v134 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v117);
                        v90 = AI_EvalFunc_DYNOff_C(AI_gpst_CurrentGameObject);
                    }
                    v18 = (tGameObjectMain*)v124->o_reward_dup_gao;
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
                        v16 = v128;
                        if (v128)
                        {
                            if ((unsigned int)v128 <= 2)
                                v16 = AI_MainActor[(uint32_t)v128 - 1];
                            v15 = v16;
                        }
                        else
                        {
                            v15 = 0;
                        }
                        if (v15 != (tGameObjectMain*)Objet_Flottant_OF_Message_Read_INV_OBJ45_Perle_gao)
                        {
                            v124->GFX_Halo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                            v14 = (tGameObjectMain*)Objet_Flottant_OF_Message_Read_SFX_light_smoke_gao;
                            if (Objet_Flottant_OF_Message_Read_SFX_light_smoke_gao)
                            {
                                if ((unsigned int)Objet_Flottant_OF_Message_Read_SFX_light_smoke_gao <= 2)
                                    v14 = AI_MainActor[Objet_Flottant_OF_Message_Read_SFX_light_smoke_gao - 1];
                                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v124->GFX_Halo, v14, 29);
                            }
                            else
                            {
                                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v124->GFX_Halo, 0, 29);
                            }
                            AI_EvalFunc_GFXFlagSet_C_CURRENT(v124->GFX_Halo, 4, 1);
                            GFXSetCurGlobal(v124->GFX_Halo);
                            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 5.0);
                            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 3.0);
                            AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
                            AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 0xC1877E);
                            v13 = (tGameObjectMain*)v124->o_reward_dup_gao;
                            if (v13)
                            {
                                if ((unsigned int)v13 <= 2)
                                    v13 = AI_MainActor[(uint32_t)v13 - 1];
                                AI_EvalFunc_OBJGetPos_C(v13, &a3);
                            }
                            else
                            {
                                AI_EvalFunc_OBJGetPos_C(0, &a3);
                            }
                            AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &a3);
                            AI_EvalFunc_GFXSetf_C_CURGLOB(21005, 0.2f);
                            AI_EvalFunc_GFXSeti_C_CURGLOB(21100, 16);
                        }
                        v124->f_time_ref = TIM_gf_MainClock - TIM_gf_LockTime;
                        v12 = (tGameObjectMain*)v124->o_reward_dup_gao;
                        if (v12)
                        {
                            if ((unsigned int)v12 <= 2)
                                v12 = AI_MainActor[(uint32_t)v12 - 1];
                            AI_EvalFunc_OBJGetPos_C(v12, &v124->v_reward_ref_pos);
                        }
                        else
                        {
                            AI_EvalFunc_OBJGetPos_C(0, &v124->v_reward_ref_pos);
                        }
                        v122 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Objet_Flottant_OF_Message_Read_Objet_Flottant_OF_Reward);
                        if (v122 != 1000)
                            return v122;
                    }
                    else
                    {
                        v80 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Objet_Flottant_OF_Message_Read_Objet_Flottant_OF_NoReward);
                        if (v80 != 1000)
                            return v80;
                    }
                }
            }
        }
        v110 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&Src, v110, sizeof(Src));
    }
    return 1000;
}

// ====================================

int caisse_gene_init()
{
    int result; // eax
    tGameObjectMain* v1; // eax
    tScriptStruct_caisse* v2; // [esp+0h] [ebp-B0h]
    tGameObjectMain* v3; // [esp+8h] [ebp-A8h]
    tGameObjectMain* v4; // [esp+10h] [ebp-A0h]
    tGameObjectMain* v5; // [esp+18h] [ebp-98h]
    tGameObjectMain* v6; // [esp+20h] [ebp-90h]
    tGameObjectMain* v7; // [esp+28h] [ebp-88h]
    tGameObjectMain* v8; // [esp+40h] [ebp-70h]
    tGameObjectMain* v9; // [esp+48h] [ebp-68h]
    Vec3 pos; // [esp+58h] [ebp-58h] BYREF
    float v11; // [esp+64h] [ebp-4Ch]
    Vec3 a2; // [esp+68h] [ebp-48h] BYREF
    BOOL v13; // [esp+74h] [ebp-3Ch]
    Vec3 v14; // [esp+78h] [ebp-38h]
    Vec3 a3; // [esp+84h] [ebp-2Ch] BYREF
    int v16; // [esp+90h] [ebp-20h]
    int v17; // [esp+94h] [ebp-1Ch]
    tScriptStruct_caisse* v18; // [esp+9Ch] [ebp-14h]
    int i; // [esp+A0h] [ebp-10h]
    Vec3 v20; // [esp+A4h] [ebp-Ch] BYREF

    a2.x = 1.0;
    a2.y = 0.0;
    a2.z = 0.0;
    v14.x = 1.0;
    v14.y = 0.0;
    v14.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_caisse*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v18 = v2;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        caisse_gene_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&caisse_gene_init_univVars_Flag_speciaux_sauve, (void*)caisse_gene_init_univVars, 4488);
        fct_caisse_gene_init_caisse_gene_loop = (int)AI_ul_RealResolveOneRefFunction(0x3D005175u);
    }
    if (v18->des_i_sf_jesuisla)
    {
        if (v18->des_i_sf_jesuisla <= 0)
        {
            i = 0;
        }
        else if (v18->des_i_sf_jesuisla < 10000)
        {
            i = AI_EvalFunc_ARRBitTest_C((int*)(caisse_gene_init_univVars_Flag_speciaux_sauve + 4), v18->des_i_sf_jesuisla);
        }
        else
        {
            v13 = AI_EvalFunc_ARRBitTest_C(
                (int*)(caisse_gene_init_univVars_Flag_speciaux_sauve + 4),
                v18->des_i_sf_jesuisla - 10000);
            i = 1 - v13;
        }
        if (!i)
        {
            v17 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v17 != 1000)
                return v17;
        }
    }
    v18->des_f_dist_hide = v18->des_f_dist_hide * v18->des_f_dist_hide;
    v18->hide = 1;
    for (i = 0; i < v18->des_i_num; ++i)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
        v9 = (tGameObjectMain*)v18->des_o_caisse;
        if (v18->des_o_caisse)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            v1 = AI_EvalFunc_OBJDuplicate_C(v9, &pos);
        }
        else
        {
            v1 = AI_EvalFunc_OBJDuplicate_C(0, &pos);
        }
        v18->ao_all_caisses[i] = (int)v1;
        v8 = (tGameObjectMain*)v18->ao_all_caisses[i];
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v8 = AI_MainActor[(uint32_t)v8 - 1];
            AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, v8, &v18->ao_all_caisses[i]);
        }
        else
        {
            AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, 0, &v18->ao_all_caisses[i]);
        }
        AI_EvalFunc_BVRandomPosGet_C(AI_gpst_CurrentGameObject, 0, &a3);
        a2 = a3;
        AI_EvalFunc_WTR_GetDif_C(&a2, &v20);
        v14 = v20;
        a2.z = v20.z;
        v7 = (tGameObjectMain*)v18->ao_all_caisses[i];
        if (v7)
        {
            if ((unsigned int)v7 <= 2)
                v7 = AI_MainActor[(uint32_t)v7 - 1];
            AI_EvalFunc_OBJSetPos_C(v7, &a2);
        }
        else
        {
            AI_EvalFunc_OBJSetPos_C(0, &a2);
        }
        v6 = (tGameObjectMain*)v18->ao_all_caisses[i];
        if (v6)
        {
            if ((unsigned int)v6 <= 2)
                v6 = AI_MainActor[(uint32_t)v6 - 1];
            AI_EvalFunc_COLStartMatrixSet_C(v6, &a2);
        }
        else
        {
            AI_EvalFunc_COLStartMatrixSet_C(0, &a2);
        }
        v11 = AI_EvalFunc_MATHRandFloat_C(-3.141593f, 3.141593f);
        v5 = (tGameObjectMain*)v18->ao_all_caisses[i];
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            AI_EvalFunc_OBJRotateGlobalZ_C(v5, v11);
        }
        else
        {
            AI_EvalFunc_OBJRotateGlobalZ_C(0, v11);
        }
        v4 = (tGameObjectMain*)v18->ao_all_caisses[i];
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
        v3 = (tGameObjectMain*)v18->ao_all_caisses[i];
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(v3, 1);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
        }
    }
    v16 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_caisse_gene_init_caisse_gene_loop);
    if (v16 == 1000)
        result = 1000;
    else
        result = v16;
    return result;
}

int caisse_gene_loop()
{
    int v0; // eax
    double v1; // st7
    tScriptStruct_caisse* v3; // [esp+0h] [ebp-C8h]
    tGameObjectMain* v4; // [esp+8h] [ebp-C0h]
    tGameObjectMain* v5; // [esp+10h] [ebp-B8h]
    tGameObjectMain* v6; // [esp+14h] [ebp-B4h]
    tGameObjectMain* v7; // [esp+18h] [ebp-B0h]
    tGameObjectMain* v8; // [esp+20h] [ebp-A8h]
    tGameObjectMain* v9; // [esp+28h] [ebp-A0h]
    tGameObjectMain* v10; // [esp+30h] [ebp-98h]
    tGameObjectMain* v11; // [esp+48h] [ebp-80h]
    tGameObjectMain* v12; // [esp+50h] [ebp-78h]
    tGameObjectMain* v13; // [esp+54h] [ebp-74h]
    tGameObjectMain* v14; // [esp+58h] [ebp-70h]
    Vec3 dest; // [esp+60h] [ebp-68h] BYREF
    tGameObjectMain* v16; // [esp+6Ch] [ebp-5Ch]
    Vec3 v17; // [esp+70h] [ebp-58h] BYREF
    int v18; // [esp+7Ch] [ebp-4Ch]
    float v19; // [esp+80h] [ebp-48h]
    Vec3 v20; // [esp+84h] [ebp-44h] BYREF
    Vec3 v21; // [esp+94h] [ebp-34h] BYREF
    int i; // [esp+A0h] [ebp-28h]
    Vec3 a3; // [esp+A4h] [ebp-24h] BYREF
    tScriptStruct_caisse* v24; // [esp+B0h] [ebp-18h]
    float X; // [esp+B4h] [ebp-14h]
    Vec3 v26; // [esp+B8h] [ebp-10h] BYREF
    float v27; // [esp+C4h] [ebp-4h]

    v21.x = 1.0;
    v21.y = 0.0;
    v21.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_caisse*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v24 = v3;
    v16 = 0;
    i = 0;
    X = 0.0;
    if (!v3->hide)
    {
        for (i = 0; i < v24->des_i_num; ++i)
        {
            v14 = (tGameObjectMain*)v24->ao_all_caisses[i];
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
                v12 = (tGameObjectMain*)v24->ao_all_caisses[i];
                if (v12)
                {
                    if ((unsigned int)v12 <= 2)
                        v12 = AI_MainActor[(uint32_t)v12 - 1];
                    v0 = AI_EvalFunc_OBJ_FlagsIdentityGet_C(v12);
                }
                else
                {
                    v0 = AI_EvalFunc_OBJ_FlagsIdentityGet_C(0);
                }
                v18 = v0;
                if ((v0 & 4) != 0)
                {
                    v11 = (tGameObjectMain*)v24->ao_all_caisses[i];
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
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v20);
                    MATH_SubstractVec(&dest, &a3, &v20);
                    v21 = dest;
                    X = v21.z * v21.z + v21.y * v21.y + v21.x * v21.x;
                    if (v24->des_f_dist_hide / 2.0 >= X)
                    {
                        v17.x = 0.0;
                        v17.y = 0.0;
                        v17.z = 0.0;
                        v9 = (tGameObjectMain*)v24->ao_all_caisses[i];
                        if (v9)
                        {
                            if ((unsigned int)v9 <= 2)
                                v9 = AI_MainActor[(uint32_t)v9 - 1];
                            AI_EvalFunc_DYNStreamSet_C(v9, &v17);
                        }
                        else
                        {
                            AI_EvalFunc_DYNStreamSet_C(0, &v17);
                        }
                    }
                    else
                    {
                        X = X - v24->des_f_dist_hide / 2.0f;
                        v19 = sqrtf(X);
                        AI_EvalFunc_MATHVecSetNormVector_C(&v21, v19);
                        v26.x = -v21.x;
                        v26.y = -v21.y;
                        v26.z = -v21.z;
                        v10 = (tGameObjectMain*)v24->ao_all_caisses[i];
                        if (v10)
                        {
                            if ((unsigned int)v10 <= 2)
                                v10 = AI_MainActor[(uint32_t)v10 - 1];
                            AI_EvalFunc_DYNStreamSet_C(v10, &v26);
                        }
                        else
                        {
                            AI_EvalFunc_DYNStreamSet_C(0, &v26);
                        }
                    }
                }
            }
        }
    }
    v16 = *AI_MainActor;
    v8 = v16;
    if (v16)
    {
        if ((unsigned int)v16 <= 2)
            v8 = AI_MainActor[(uint32_t)v16 - 1];
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v8);
    }
    else
    {
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v27 = (float)v1;
    if (v24->des_f_dist_hide >= (double)v27)
    {
        if (!v24->hide)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v24->hide = 0;
    }
    else
    {
        if (v24->hide)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v24->hide = 1;
    }
    for (i = 0; i < v24->des_i_num; ++i)
    {
        v7 = (tGameObjectMain*)v24->ao_all_caisses[i];
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
            v5 = (tGameObjectMain*)v24->ao_all_caisses[i];
            if (v5)
            {
                if ((unsigned int)v5 <= 2)
                    v5 = AI_MainActor[(uint32_t)v5 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v5, v24->hide);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, v24->hide);
            }
            v4 = (tGameObjectMain*)v24->ao_all_caisses[i];
            if (v4)
            {
                if ((unsigned int)v4 <= 2)
                    v4 = AI_MainActor[(uint32_t)v4 - 1];
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(v4, v24->hide);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, v24->hide);
            }
        }
    }
    return 1000;
}