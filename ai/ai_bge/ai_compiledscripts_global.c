#include "ai_compiledscripts_inc.h"

//Global_glob_init
void* Global_glob_init_univVars;
int Global_glob_init_univVars_i_engine_mode;
Global_glob_init_univVars_f_volume_inventaire;
Global_glob_init_univVars_tmp_save_pour_pas_killer_les_;
int Global_glob_init_univVars_i_INV_OptionsFlags;
tGameObjectMain* Global_glob_init__PJ_Camera_gao;
tGameObjectMain* Global_glob_init__PJ_Sally_gao;
tGameObjectMain* Global_glob_init_GST_Global_gao;
tGameObjectMain* Global_glob_init_GST_Sound_Manager_gao;
int Global_glob_init_univVars_Flag_speciaux_sauve;
int Global_glob_init_univVars_o_course_manager;
int fct_Global_glob_init_Global_glob_raz_univers;
int fct_Global_glob_init_Global_glob_init_object;
int Global_glob_init_univVars_Perso_ID_cran_de_vie_nini;
int Global_glob_init_univVars_Perso_ID_Energie_courante;
int Global_glob_init_univVars_Perso_ID_Energie_max;
int Global_glob_init_GST_Global_gao_i_fade_priority;
int Global_glob_init_GST_Global_gao_f_fade_start_time;
int Global_glob_init_GST_Global_gao_f_fade_delay;
tGameObjectMain* Global_glob_init_INV_MatFade_gao;
int Global_glob_init_univVars_i_num_codes;
int Global_glob_init_univVars_ai_codes;
int fct_Global_glob_init_Global_glob_go;

// Global_glob_go
int fct_Global_glob_go_Global_glob_exec_flash;
int fct_Global_glob_go_Global_glob_exec_world_cycle;
int fct_Global_glob_go_Global_glob_exec_alarme;
int fct_Global_glob_go_Global_glob_exec_event_sound;
int fct_Global_glob_go_Global_glob_exec_music_machine;
int fct_Global_glob_go_Global_glob_exec_volumes_manager;
int fct_Global_glob_go_Global_glob_exec_joy;
int fct_Global_glob_go_Global_glob_exec_buttons;
int fct_Global_glob_go_Global_Library_Perso_TreatEquipement;
int fct_Global_glob_go_Global_glob_exec_hauteur_eau;
int fct_Global_glob_go_Global_glob_exec_game_speed;
int Global_glob_go_univVars;
int Global_glob_go_univVars_Flag_speciaux_sauve;
int Global_glob_go_univVars_Perso_ID_Energie_courante;
int Global_glob_go_GST_Fight_Manager_gao;
int Global_glob_go_GST_Fight_Manager_gao_i_Evil_NbActor;
int Global_glob_go__PJ_Sally_gao;
int Global_glob_go__PJ_Sally_gao_i_fight_on;
int Global_glob_go_univVars_objet_inventaire;
int Global_glob_go_Perso_ID_status;
int Global_glob_go_World_ID;
int Global_glob_go_i_sally_nbmorts;
int Global_glob_go_GST_Global_gao;
int Global_glob_go_GST_Global_gao_f_joy_norm;
int Global_glob_go_GST_Global_gao_v_joy_camera;
int Global_glob_go_GST_Global_gao_v_joy_camera_normalized;
int fct_Global_glob_go_Global_glob_mort;
int Global_glob_go_univVars_f_TempsDeJeuTotal;
int Global_glob_go_univVars_f_Delai_Avant_Mort_Mili;
int Global_glob_go_GST_Board_panel_gao;
int Global_glob_go_GST_Global_gao_i_fade_priority;
int Global_glob_go_GST_Global_gao_f_fade_start_time;
int Global_glob_go_GST_Global_gao_f_fade_delay;
int Global_glob_go_univVars_i_fade_fx;
int Global_glob_go_INV_MatFade_gao;
int Global_glob_go__PJ_Camera_Target_gao;
int Global_glob_go__PJ_Camera_Target_gao_f_rot_local_y;
int Global_glob_go_GST_Global_gao_f_game_speed;
int Global_glob_go__PJ_Camera_Speciale_gao;
int Global_glob_go_GST_Global_gao_f_time_reinit;

//Global_glob_exec_volumes_manager
int Global_glob_exec_volumes_manager__PJ_Sally_gao;
int Global_glob_exec_volumes_manager_univVars;
int Global_glob_exec_volumes_manager_univVars_i_engine_mode;
int Global_glob_exec_volumes_manager_univVars_f_volume_infodesign;
int Global_glob_exec_volumes_manager_univVars_f_volume_inventaire;

//Global_glob_exec_world_cycle
int Global_glob_exec_world_cycle_univVars;
int Global_glob_exec_world_cycle_univVars_Planetes_CurHour;
int Global_glob_exec_world_cycle_univVars_Planetes_CurHourOld;
int Global_glob_exec_world_cycle_univVars_Planetes_CurRotation;
int Global_glob_exec_world_cycle_GST_Board_panel_gao;
int Global_glob_exec_world_cycle_univVars_Flag_speciaux_sauve;
int Global_glob_exec_world_cycle_GST_Global_gao;
int Global_glob_exec_world_cycle_GST_Global_gao_Planetes_Rotations;
int Global_glob_exec_world_cycle_univVars_Planetes_CurDay;


//Global_glob_raz_univers
int Global_glob_raz_univers_univVars;
int Global_glob_raz_univers_univVars_i_engine_mode;
int Global_glob_raz_univers_univVars_load_world;
int Global_glob_raz_univers_univVars_world_ID;
int Global_glob_raz_univers_univVars_world_key;
int Global_glob_raz_univers_univVars_world_loaded_id;
int Global_glob_raz_univers_univVars_Perso_ID_world_ID;
int Global_glob_raz_univers_univVars_Perso_ID_pos;
int Global_glob_raz_univers_univVars_Perso_ID_sight;
int Global_glob_raz_univers_univVars_Perso_ID_status;
int Global_glob_raz_univers_univVars_Perso_ID_actor;
int Global_glob_raz_univers_univVars_Perso_ID_Companion;
int Global_glob_raz_univers_univVars_deguisement_world_ID;
int Global_glob_raz_univers_univVars_deguisement_pos;
int Global_glob_raz_univers_univVars_deguisement_sight;
int Global_glob_raz_univers_univVars_deguisement_status;
int Global_glob_raz_univers_univVars_objet_inventaire;
int Global_glob_raz_univers_univVars_Carte_i_NbSpot;

//Global_glob_reflex
int Global_glob_reflex_INV_Manager_gao;
int Global_glob_reflex_INV_Manager_gao_i_SelObject;
int Global_glob_reflex_INV_Manager_gao_i_OpeningMode;
int Global_glob_reflex_INV_Manager_gao_i_CineMode;
int Global_glob_reflex_INV_Manager_gao_f_ObjInfo_Wait;
int Global_glob_reflex_univVars;
int Global_glob_reflex_GST_Board_panel_gao;
int Global_glob_reflex_INV_Manager_gao_i_OpeningParamInt;
int Global_glob_reflex_INV_Manager_gao_i_OpeningParamFloat;
int Global_glob_reflex_GST_Global_gao;
int Global_glob_reflex_GST_Global_gao_i_fade_priority;
int Global_glob_reflex_GST_Global_gao_f_fade_start_time;
int Global_glob_reflex_GST_Global_gao_f_fade_delay;
int Global_glob_reflex_INV_MatFade_gao;
int Global_glob_reflex_univVars_i_special_init;
int Global_glob_reflex_univVars_load_world;
int Global_glob_reflex_univVars_world_ID;
int Global_glob_reflex_univVars_world_entrance_ID;
int Global_glob_reflex_univVars_Perso_ID_inventaire_max;
int Global_glob_reflex_univVars_Perso_ID_actor;
int Global_glob_reflex__PJ_Camera_gao;
int Global_glob_reflex_SFX_Flash_texture_gao;
int Global_glob_reflex_univVars_i_fade_fx;
int Global_glob_reflex_GST_Board_panel_gao_Gen_Show;
int Global_glob_reflex__PJ_Camera_Speciale_gao;
int Global_glob_reflex_SFX_AfterFX_gao;
int Global_glob_reflex_univVars_mdisk_possessionflags;
int Global_glob_reflex_univVars_Perso_ID_equipement;
int Global_glob_reflex_univVars_objet_status;
int Global_glob_reflex_univVars_objet_inventaire;
int Global_glob_reflex_univVars_Flag_speciaux_sauve;
int Global_glob_reflex_GST_Global_gao_i_Perso_ID_a_checker;
int Global_glob_reflex_univVars_Perso_ID_Energie_courante;
int Global_glob_reflex_univVars_Perso_ID_Energie_max;
int Global_glob_reflex_INV_Manager_gao_i_SaveDisable;
int Global_glob_reflex_univVars_Perso_ID_status;
int Global_glob_reflex_univVars_Perso_ID_world_ID;
int Global_glob_reflex_univVars_Perso_ID_pos;
int Global_glob_reflex_univVars_Perso_ID_sight;
int Global_glob_reflex_univVars_objet_valeur1;

// Global_glob_exec_joy
int Global_glob_exec_joy_GST_Global_gao;
int Global_glob_exec_joy_GST_Cheat_Manager_gao;
int Global_glob_exec_joy_GST_Cheat_Manager_gao_i_pause_mode;
int Global_glob_exec_joy_GST_Board_panel_gao;
int Global_glob_exec_joy_GST_Board_panel_gao_f_time_on_sort_de_linventaire;
int Global_glob_exec_joy__PJ_Sally_gao;

// Global_glob_init_object
int Global_glob_init_object_univVars;
int Global_glob_init_object_univVars_objet_status;
int Global_glob_init_object_univVars_objet_valeur1;

// ===========================

//world_cycle_init_wsi_init
int world_cycle_init_wsi_init_univVars;
int world_cycle_init_wsi_init_univVars_Flag_speciaux_sauve;
int world_cycle_init_wsi_init_univVars_Planetes_CurRotation;

// ====================================

//move_with_me_move_with_me_init
int fct_move_with_me_move_with_me_init_move_with_me_move_with_me_loop;

//planete_rotation_pr_init
int planete_rotation_pr_init_univVars;
int planete_rotation_pr_init_univVars_Planetes_CurRotation;
int fct_planete_rotation_pr_init_planete_rotation_pr_loop;

//planete_rotation_pr_loop
int planete_rotation_pr_loop_GST_Global_gao;
int planete_rotation_pr_loop_GST_Global_gao_Planetes_Rotations;
int planete_rotation_pr_loop_univVars;
int planete_rotation_pr_loop_univVars_Planetes_CurRotation;

// ======================================

//Zone_OccupationTerrain_zo_init
int Zone_OccupationTerrain_zo_init_univVars;
int Zone_OccupationTerrain_zo_init_univVars_Flag_speciaux_sauve;
int Zone_OccupationTerrain_zo_init_univVars_Perso_ID_Companion;
int Zone_OccupationTerrain_zo_init_univVars_Perso_ID_actor;
int fct_Zone_OccupationTerrain_zo_init_Zone_OccupationTerrain_zo_dehors;

//Zone_OccupationTerrain_zo_dehors
int Zone_OccupationTerrain_zo_dehors_univVars;
int Zone_OccupationTerrain_zo_dehors_univVars_Perso_ID_Companion;
int Zone_OccupationTerrain_zo_dehors_univVars_Perso_ID_status;
int fct_Zone_OccupationTerrain_zo_dehors_Zone_OccupationTerrain_zo_exec_transfert;
int fct_Zone_OccupationTerrain_zo_dehors_Zone_OccupationTerrain_zo_dans_la_place;

//Zone_OccupationTerrain_zo_dans_la_place
int Zone_OccupationTerrain_zo_dans_la_place_univVars;
int Zone_OccupationTerrain_zo_dans_la_place_univVars_Perso_ID_Companion;
int Zone_OccupationTerrain_zo_dans_la_place_univVars_Perso_ID_status;
int fct_Zone_OccupationTerrain_zo_dans_la_place_Zone_OccupationTerrain_zo_exec_clear;
int fct_Zone_OccupationTerrain_zo_dans_la_place_Zone_OccupationTerrain_zo_dehors;

//Zone_OccupationTerrain_Zo_reflex
int Zone_OccupationTerrain_Zo_reflex_univVars;
int Zone_OccupationTerrain_Zo_reflex_univVars_Flag_speciaux_sauve;
int fct_Zone_OccupationTerrain_Zo_reflex_Zone_OccupationTerrain_zo_exec_clear;

// =====================================

//Zone_ChangeStatus_MultiCompagnon_zcsmc_init
int fct_Zone_ChangeStatus_MultiCompagnon_zcsmc_init_Zone_ChangeStatus_MultiCompagnon_zcsmc_wait;

//Zone_ChangeStatus_MultiCompagnon_zcsmc_wait
int Zone_ChangeStatus_MultiCompagnon_zcsmc_wait__PJ_Sally_gao;
int Zone_ChangeStatus_MultiCompagnon_zcsmc_wait__univVars;
int Zone_ChangeStatus_MultiCompagnon_zcsmc_wait__univVars_Perso_ID_actor;
int Zone_ChangeStatus_MultiCompagnon_zcsmc_wait__univVars_Perso_ID_status;

// ====================================

//Zone_ChangeStatus_zcs_init
int Zone_ChangeStatus_zcs_init_univVars;
int Zone_ChangeStatus_zcs_init_univVars_Perso_ID_Companion;
int Zone_ChangeStatus_zcs_init_univVars_Perso_ID_actor;
int fct_Zone_ChangeStatus_zcs_init_Zone_ChangeStatus_zcs_wait;

//Zone_ChangeStatus_zcs_wait
int Zone_ChangeStatus_zcs_wait_univVars;
int Zone_ChangeStatus_zcs_wait_univVars_Flag_speciaux_sauve;
int Zone_ChangeStatus_zcs_wait__PJ_Sally_gao;
int Zone_ChangeStatus_zcs_wait_univVars_Perso_ID_status;
int fct_Zone_ChangeStatus_zcs_wait_Zone_ChangeStatus_zcs_exec_texte;

//Zone_ChangeStatus_zcs_exec_texte
int Zone_ChangeStatus_zcs_exec_texte_univVars;
int Zone_ChangeStatus_zcs_exec_texte_univVars_Perso_ID_Companion;

// =======================================

//Zone_CompanionNetwork_nz_init
int fct_Zone_CompanionNetwork_nz_init_Zone_CompanionNetwork_nz_set;

//Zone_CompanionNetwork_nz_set
int Zone_CompanionNetwork_nz_set_univVars;
int Zone_CompanionNetwork_nz_set_univVars_Perso_ID_actor;

int Global_glob_init()
{
    int result; // eax
    float v1; // [esp+0h] [ebp-D8h]
    float* v2; // [esp+4h] [ebp-D4h]
    tScriptStruct_GSTGlobal* v3; // [esp+8h] [ebp-D0h]
    tGameObjectMain* v4; // [esp+10h] [ebp-C8h]
    float v5; // [esp+1Ch] [ebp-BCh]
    double v6; // [esp+28h] [ebp-B0h]
    double v7; // [esp+38h] [ebp-A0h]
    int v8; // [esp+64h] [ebp-74h]
    int a1; // [esp+7Ch] [ebp-5Ch]
    int a2; // [esp+80h] [ebp-58h]
    int a3; // [esp+88h] [ebp-50h]
    char a3a; // [esp+88h] [ebp-50h]
    int v13; // [esp+90h] [ebp-48h]
    int v14; // [esp+A0h] [ebp-38h]
    int v15; // [esp+A8h] [ebp-30h]
    int v16; // [esp+ACh] [ebp-2Ch]
    float v17; // [esp+B8h] [ebp-20h]
    float v18; // [esp+B8h] [ebp-20h]
    int i; // [esp+BCh] [ebp-1Ch]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_GSTGlobal*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Global_glob_init_univVars = (void*)AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&Global_glob_init_univVars_i_engine_mode, Global_glob_init_univVars, 28);
        AI2C_ResolveBuffer(&Global_glob_init_univVars_f_volume_inventaire, Global_glob_init_univVars, 20248);
        AI2C_ResolveBuffer(&Global_glob_init_univVars_tmp_save_pour_pas_killer_les_, Global_glob_init_univVars, 20280);
        AI2C_ResolveBuffer(&Global_glob_init_univVars_i_INV_OptionsFlags, Global_glob_init_univVars, 7656);
        Global_glob_init__PJ_Camera_gao = (tGameObjectMain*)AI_ul_RealResolveOneRefObject(0x71003F56u);
        Global_glob_init__PJ_Sally_gao = (tGameObjectMain*)AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Global_glob_init_GST_Global_gao = (tGameObjectMain*)AI_ul_RealResolveOneRefObject(0x710047BCu);
        Global_glob_init_GST_Sound_Manager_gao = (tGameObjectMain*)AI_ul_RealResolveOneRefObject(0x7100F33Fu);
        AI2C_ResolveBuffer(&Global_glob_init_univVars_Flag_speciaux_sauve, Global_glob_init_univVars, 4488);
        AI2C_ResolveBuffer(&Global_glob_init_univVars_o_course_manager, Global_glob_init_univVars, 7660);
        fct_Global_glob_init_Global_glob_raz_univers = AI_ul_RealResolveOneRefFunction(0x71008011u);
        fct_Global_glob_init_Global_glob_init_object = AI_ul_RealResolveOneRefFunction(0x7100DF93u);
        AI2C_ResolveBuffer(&Global_glob_init_univVars_Perso_ID_cran_de_vie_nini, Global_glob_init_univVars, 8076);
        AI2C_ResolveBuffer(&Global_glob_init_univVars_Perso_ID_Energie_courante, Global_glob_init_univVars, 1148);
        AI2C_ResolveBuffer(&Global_glob_init_univVars_Perso_ID_Energie_max, Global_glob_init_univVars, 1096);
        AI2C_ResolveBuffer(&Global_glob_init_GST_Global_gao_i_fade_priority, Global_glob_init_GST_Global_gao, 3268);
        AI2C_ResolveBuffer(&Global_glob_init_GST_Global_gao_f_fade_start_time, Global_glob_init_GST_Global_gao, 3260);
        AI2C_ResolveBuffer(&Global_glob_init_GST_Global_gao_f_fade_delay, Global_glob_init_GST_Global_gao, 3264);
        Global_glob_init_INV_MatFade_gao = (tGameObjectMain*)AI_ul_RealResolveOneRefObject(0x5E006934u);
        AI2C_ResolveBuffer(&Global_glob_init_univVars_i_num_codes, Global_glob_init_univVars, 6784);
        AI2C_ResolveBuffer(&Global_glob_init_univVars_ai_codes, Global_glob_init_univVars, 6788);
        fct_Global_glob_init_Global_glob_go = AI_ul_RealResolveOneRefFunction(0x71003B29u);
    }
    AI_EvalFunc_IoJoyFreeze_C(0);
    for (i = 0; i < 20; ++i)
        AI_EvalFunc_Gsp_AE_MASTER_C(i, 0, 0, 0.0);
    AI_EvalFunc_Gsp_AE_MASTER_C(8, 0, 0, 0.5);
    AI_EvalFunc_Gsp_AE_MASTER_C(9, 0, 0, 0.5);
    AI_EvalFunc_AIPriorityGet_C(AI_gpst_CurrentGameObject);
    if (*(uint32_t*)Global_glob_init_univVars_i_engine_mode == 1)
    {
        AI_ReinitUniverse();
        AI_EvalFunc_SAVEUnvalidate_C();
    }
    else if (*(float*)AI_EvalVarArray_C2((char *)Global_glob_init_univVars_f_volume_inventaire, 0) == 0.0)
    {
        *(uint32_t*)AI_EvalVarArray_C2((char*)Global_glob_init_univVars_f_volume_inventaire, 0) = 0x3F800000;
        *(uint32_t*)AI_EvalVarArray_C2((char*)Global_glob_init_univVars_f_volume_inventaire, 1) = 0x3F800000;
        *(uint32_t*)AI_EvalVarArray_C2((char*)Global_glob_init_univVars_f_volume_inventaire, 2) = 0x3F800000;
        v17 = *(float*)AI_EvalVarArray_C2((char*)Global_glob_init_univVars_f_volume_inventaire, 0) * 10000.0f;
        v7 = v17 + MATH_gd_magicDoubleForRound;
        *(uint32_t*)(Global_glob_init_univVars_tmp_save_pour_pas_killer_les_ + 8) = (*(uint32_t *)&v7); // FIXME
    }
    if ((*(uint32_t*)Global_glob_init_univVars_i_INV_OptionsFlags & 8) != 0)
        INO_i_PifPafEnable(0);
    else
        INO_i_PifPafEnable(1);
    if (ConsoleSpecificOption(1) != -1)
    {
        if ((*(uint32_t*)Global_glob_init_univVars_i_INV_OptionsFlags & 0x10) != 0)
            ConsoleSpecificOption(2);
        else
            ConsoleSpecificOption(3);
    }
    *(float*)AI_EvalVarArray_C2((char*)Global_glob_init_univVars_f_volume_inventaire, 0) = (float)*(int*)(Global_glob_init_univVars_tmp_save_pour_pas_killer_les_
        + 8);
    v2 = (float*)AI_EvalVarArray_C2((char*)Global_glob_init_univVars_f_volume_inventaire, 0);
    *v2 = *v2 / 10000.0f;
    if (*(float*)AI_EvalVarArray_C2((char*)Global_glob_init_univVars_f_volume_inventaire, 0) == 0.0)
    {
        *(uint32_t*)AI_EvalVarArray_C2((char*)Global_glob_init_univVars_f_volume_inventaire, 0) = 0x3F800000;
        v18 = *(float*)AI_EvalVarArray_C2((char*)Global_glob_init_univVars_f_volume_inventaire, 0) * 10000.0f;
        v6 = v18 + MATH_gd_magicDoubleForRound;
        *(uint32_t*)(Global_glob_init_univVars_tmp_save_pour_pas_killer_les_ + 8) = (*(uint32_t *)&v6); // FIXME
    }
    AI_EvalFunc_VIEWAssignObject_C(Global_glob_init__PJ_Camera_gao, 0);
    AI_EvalFunc_SNDMicroPosSet_C(Global_glob_init__PJ_Sally_gao);
    AI_EvalFunc_COLGlobalWallCosAngleSet_C(0.5f);
    AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(0, 65280);
    AI_EvalFunc_OBJ_CustomBitsSet_C(Global_glob_init_GST_Global_gao, 0x8000, 0);
    AI_EvalFunc_Gsp_AE_MASTER_C(4, 0, 0, 0.0);
    AI_EvalFunc_MSGRegOwner_C(Global_glob_init_GST_Sound_Manager_gao);
    AI_EvalFunc_ARRGlobalIntSet_C((int*)(Global_glob_init_univVars_Flag_speciaux_sauve + 4));
    v3->f_reinit_time = TIM_gf_MainClock - TIM_gf_LockTime;
    v3->f_time_active_avancement_jeu = TIM_gf_MainClock - TIM_gf_LockTime;
    v3->i_map_just_reset = 10;
    *(uint32_t*)Global_glob_init_univVars_o_course_manager = 0;
    v8 = AI_EvalFunc_AIExecute_C((tAIFunction *)fct_Global_glob_init_Global_glob_raz_univers);
    if (v8 != 1000)
        return v8;
    v13 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Global_glob_init_Global_glob_init_object);
    if (v13 != 1000)
        return v13;
    v3->v_joy_cam_sight.x = 0.0;
    v3->v_joy_cam_sight.y = 0.0;
    v3->v_joy_cam_sight.z = 0.0;
    v3->v_joy_cam_sight_old.x = 0.0;
    v3->v_joy_cam_sight_old.y = 0.0;
    v3->v_joy_cam_sight_old.z = 0.0;
    v3->i_joy_transition_en_cours = 0;
    v3->i_mort_en_cours_old = -1;
    v3->i_mort_en_cours = -1;
    v15 = *(uint32_t*)Global_glob_init_univVars_Perso_ID_cran_de_vie_nini;
    if (*(uint32_t*)Global_glob_init_univVars_Perso_ID_cran_de_vie_nini != -1)
    {
        v5 = *(float*)AI_EvalVarArray_C2((char*)Global_glob_init_univVars_Perso_ID_Energie_max, v15);
        if (v5 > 160.0)
            v1 = 160.0;
        else
            v1 = v5;
        *(float*)AI_EvalVarArray_C2((char*)Global_glob_init_univVars_Perso_ID_Energie_courante, v15) = v1;
        AI_EvalFunc_SAVEValidate_C();
        *(uint32_t*)Global_glob_init_univVars_Perso_ID_cran_de_vie_nini = -1;
    }
    v3->i_snd_mm_instance[0] = -17;
    v3->i_snd_mm_instance[1] = -1;
    SND_MuteEnable(3, 0);
    SND_MuteEnable(0, 0);
    SND_MuteEnable(10, 0);
    SND_MuteEnable(11, 0);
    SND_i_TrackSeti(1020u, 2, 0);
    SND_i_TrackSetf(1020, 3, 1.0);
    SND_i_TrackSeti(1040u, 2, 0);
    SND_i_TrackSetf(1040, 3, 1.0);
    SND_i_Seti(7, 0);
    SND_i_Seti(8, 0);
    AI_EvalFunc_Gsp_AE_MASTER_C(17, 1, 0, 0.0);
    if (*(int*)Global_glob_init_GST_Global_gao_i_fade_priority <= 5000
        || AI_EvalFunc_TIMEElapsed_C(
            *(float*)Global_glob_init_GST_Global_gao_f_fade_start_time,
            *(float*)Global_glob_init_GST_Global_gao_f_fade_delay))
    {
        *(float*)Global_glob_init_GST_Global_gao_f_fade_start_time = TIM_gf_MainClock - TIM_gf_LockTime;
        *(uint32_t*)Global_glob_init_GST_Global_gao_f_fade_delay = 0x3F8CCCCD;
        *(uint32_t*)Global_glob_init_GST_Global_gao_i_fade_priority = 5000;
        AI_EvalFunc_GFXDel_C_CURRENT(10);
        v16 = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v16, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v16, 2, 1);
        v4 = (tGameObjectMain*)Global_glob_init_INV_MatFade_gao;
        if (Global_glob_init_INV_MatFade_gao)
        {
            if ((unsigned int)Global_glob_init_INV_MatFade_gao <= 2)
                v4 = AI_MainActor[(uint32_t)Global_glob_init_INV_MatFade_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v16, v4, 8);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v16, 0, 8);
        }
        GFXSetCurGlobal(v16);
        AI_EvalFunc_GFXSeti_C_CURGLOB(11100, -1);
        AI_EvalFunc_GFXSeti_C_CURGLOB(11101, 0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.1f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 1.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
    }
    SND_i_Seti(10, 1);
    AI_EvalFunc_AIMainActorSet_C(Global_glob_init__PJ_Sally_gao, 0);
    AI_EvalFunc_AIMainActorReset_C(1);
    for (a1 = 0; a1 < *(int32_t*)Global_glob_init_univVars_i_num_codes; ++a1)
    {
        STR_GlobalSet(a1, (char*)AI_gpst_CurrentFunction->localStringConstsData + 43);
        for (a2 = 0; a2 < 7 && *(uint32_t*)AI_EvalVarArray2_C(1, (char*)Global_glob_init_univVars_ai_codes, a2, a1); ++a2)
        {
            a3 = *(uint32_t*)AI_EvalVarArray2_C(1, (char*)Global_glob_init_univVars_ai_codes, a2, a1);
            if (a3 > 26)
                a3a = a3 + 21;
            else
                a3a = a3 + 64;
            STR_GlobalSetChar(a1, a2, a3a);
        }
        STR_GlobalSetChar(a1, a2, 0);
    }
    v14 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Global_glob_init_Global_glob_go);
    if (v14 == 1000)
        result = 1000;
    else
        result = v14;
    return result;
}

int Global_glob_go()
{
    int result; // eax
    tScriptStruct_GSTGlobal* v1; // [esp+Ch] [ebp-108h]
    tGameObjectMain* v2; // [esp+18h] [ebp-FCh]
    tGameObjectMain* v3; // [esp+34h] [ebp-E0h]
    int v4; // [esp+64h] [ebp-B0h]
    int v5; // [esp+6Ch] [ebp-A8h]
    int v6; // [esp+74h] [ebp-A0h]
    int v7; // [esp+80h] [ebp-94h]
    int v8; // [esp+84h] [ebp-90h]
    int v9; // [esp+ACh] [ebp-68h]
    int i; // [esp+BCh] [ebp-58h]
    BOOL v11; // [esp+C0h] [ebp-54h]
    int v12; // [esp+C8h] [ebp-4Ch]
    int v13; // [esp+CCh] [ebp-48h]
    int v14; // [esp+DCh] [ebp-38h]
    int a1; // [esp+E0h] [ebp-34h]
    int v16; // [esp+E8h] [ebp-2Ch]
    float v17; // [esp+F0h] [ebp-24h]
    int v18; // [esp+FCh] [ebp-18h]
    int v19; // [esp+108h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GSTGlobal*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Global_glob_go_Global_glob_exec_flash = (int)AI_ul_RealResolveOneRefFunction(0x9E000A32);
        fct_Global_glob_go_Global_glob_exec_world_cycle = (int)AI_ul_RealResolveOneRefFunction(0x2600055Du);
        fct_Global_glob_go_Global_glob_exec_alarme = (int)AI_ul_RealResolveOneRefFunction(0x7100AE61u);
        fct_Global_glob_go_Global_glob_exec_event_sound = (int)AI_ul_RealResolveOneRefFunction(0x7100BB90u);
        fct_Global_glob_go_Global_glob_exec_music_machine = (int)AI_ul_RealResolveOneRefFunction(0x2600080Eu);
        fct_Global_glob_go_Global_glob_exec_volumes_manager = (int)AI_ul_RealResolveOneRefFunction(0x260032BFu);
        fct_Global_glob_go_Global_glob_exec_joy = (int)AI_ul_RealResolveOneRefFunction(0x7100AE5Fu);
        fct_Global_glob_go_Global_glob_exec_buttons = (int)AI_ul_RealResolveOneRefFunction(0x7100BA78u);
        fct_Global_glob_go_Global_Library_Perso_TreatEquipement = (int)AI_ul_RealResolveOneRefFunction(0x49018B8Du);
        fct_Global_glob_go_Global_glob_exec_hauteur_eau = (int)AI_ul_RealResolveOneRefFunction(0x7100BBE5u);
        fct_Global_glob_go_Global_glob_exec_game_speed = (int)AI_ul_RealResolveOneRefFunction(0x260011F9u);
        Global_glob_go_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&Global_glob_go_univVars_Flag_speciaux_sauve, (void*)Global_glob_go_univVars, 4488);
        AI2C_ResolveBuffer(&Global_glob_go_univVars_Perso_ID_Energie_courante, (void*)Global_glob_go_univVars, 1148);
        Global_glob_go_GST_Fight_Manager_gao = AI_ul_RealResolveOneRefObject(0x9E002FFA);
        AI2C_ResolveBuffer(
            &Global_glob_go_GST_Fight_Manager_gao_i_Evil_NbActor,
            (void*)Global_glob_go_GST_Fight_Manager_gao,
            2624);
        Global_glob_go__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(&Global_glob_go__PJ_Sally_gao_i_fight_on, (void*)Global_glob_go__PJ_Sally_gao, 1216);
        AI2C_ResolveBuffer(&Global_glob_go_univVars_objet_inventaire, (void*)Global_glob_go_univVars, 1480);
        AI2C_ResolveBuffer(&Global_glob_go_Perso_ID_status, (void*)Global_glob_go_univVars, 484);
        AI2C_ResolveBuffer(&Global_glob_go_World_ID, (void*)Global_glob_go_univVars, 4);
        AI2C_ResolveBuffer(&Global_glob_go_i_sally_nbmorts, (void*)Global_glob_go_univVars, 1208);
        Global_glob_go_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(&Global_glob_go_GST_Global_gao_f_joy_norm, (void*)Global_glob_go_GST_Global_gao, 116);
        AI2C_ResolveBuffer(&Global_glob_go_GST_Global_gao_v_joy_camera, (void*)Global_glob_go_GST_Global_gao, 20);
        AI2C_ResolveBuffer(
            &Global_glob_go_GST_Global_gao_v_joy_camera_normalized,
            (void*)Global_glob_go_GST_Global_gao,
            56);
        fct_Global_glob_go_Global_glob_mort = (int)AI_ul_RealResolveOneRefFunction(0x26001A8Du);
        AI2C_ResolveBuffer(&Global_glob_go_univVars_f_TempsDeJeuTotal, (void*)Global_glob_go_univVars, 8336);
        AI2C_ResolveBuffer(&Global_glob_go_univVars_f_Delai_Avant_Mort_Mili, (void*)Global_glob_go_univVars, 18216);
        Global_glob_go_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(&Global_glob_go_GST_Global_gao_i_fade_priority, (void*)Global_glob_go_GST_Global_gao, 3268);
        AI2C_ResolveBuffer(&Global_glob_go_GST_Global_gao_f_fade_start_time, (void*)Global_glob_go_GST_Global_gao, 3260);
        AI2C_ResolveBuffer(&Global_glob_go_GST_Global_gao_f_fade_delay, (void*)Global_glob_go_GST_Global_gao, 3264);
        AI2C_ResolveBuffer(&Global_glob_go_univVars_i_fade_fx, (void*)Global_glob_go_univVars, 20400);
        Global_glob_go_INV_MatFade_gao = AI_ul_RealResolveOneRefObject(0x5E006934u);
        Global_glob_go__PJ_Camera_Target_gao = AI_ul_RealResolveOneRefObject(0x35006499u);
        AI2C_ResolveBuffer(
            &Global_glob_go__PJ_Camera_Target_gao_f_rot_local_y,
            (void*)Global_glob_go__PJ_Camera_Target_gao,
            244);
        AI2C_ResolveBuffer(&Global_glob_go_GST_Global_gao_f_game_speed, (void*)Global_glob_go_GST_Global_gao, 3548);
        Global_glob_go__PJ_Camera_Speciale_gao = AI_ul_RealResolveOneRefObject(0x9E0007E2);
        AI2C_ResolveBuffer(&Global_glob_go_GST_Global_gao_f_time_reinit, (void*)Global_glob_go_GST_Global_gao, 3780);
    }
    if (*AI_gppst_CurrentJumpNode != (tAINode*)39)
    {
        AI_EvalFunc_DbgTraceInt_C();
        v1->f_dist_mine = 1.0e14f;
        v1->f_crochax_DT = -1.0;
        if (v1->i_map_just_reset)
            --v1->i_map_just_reset;
        v7 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Global_glob_go_Global_glob_exec_flash);
        if (v7 != 1000)
            return v7;
        v16 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Global_glob_go_Global_glob_exec_world_cycle);
        if (v16 != 1000)
            return v16;
        v13 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Global_glob_go_Global_glob_exec_alarme);
        if (v13 != 1000)
            return v13;
        v5 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Global_glob_go_Global_glob_exec_event_sound);
        if (v5 != 1000)
            return v5;
        v4 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Global_glob_go_Global_glob_exec_music_machine);
        if (v4 != 1000)
            return v4;
        v9 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Global_glob_go_Global_glob_exec_volumes_manager);
        if (v9 != 1000)
            return v9;
        v19 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Global_glob_go_Global_glob_exec_joy);
        if (v19 != 1000)
            return v19;
        v6 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Global_glob_go_Global_glob_exec_buttons);
        if (v6 != 1000)
            return v6;
        if (v1->i_Perso_ID_a_checker)
        {
            v12 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Global_glob_go_Global_Library_Perso_TreatEquipement);
            if (v12 != 1000)
                return v12;
            v1->i_Perso_ID_a_checker = 0;
        }
        v18 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Global_glob_go_Global_glob_exec_hauteur_eau);
        if (v18 != 1000)
            return v18;
        v8 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Global_glob_go_Global_glob_exec_game_speed);
        if (v8 != 1000)
            return v8;
        if (!AI_EvalFunc_ARRBitTest_C((int*)(Global_glob_go_univVars_Flag_speciaux_sauve + 4), 1049))
        {
            if (AI_EvalFunc_ARRBitTest_C((int*)(Global_glob_go_univVars_Flag_speciaux_sauve + 4), 1048))
            {
                if (*(float*)AI_EvalVarArray_C2((char*)Global_glob_go_univVars_Perso_ID_Energie_courante, 8) > 20.0)
                    AI_EvalFunc_ARRBitSet_C((int*)(Global_glob_go_univVars_Flag_speciaux_sauve + 4), 1049);
            }
            else if (!*(uint32_t*)Global_glob_go_GST_Fight_Manager_gao_i_Evil_NbActor
                && *(float*)AI_EvalVarArray_C2((char*)Global_glob_go_univVars_Perso_ID_Energie_courante, 8) <= 20.0
                && !*(uint32_t*)Global_glob_go__PJ_Sally_gao_i_fight_on
                && (*(int*)AI_EvalVarArray2_C(1, (char*)Global_glob_go_univVars_objet_inventaire, 13, 8) > 0
                    || *(int*)AI_EvalVarArray2_C(1, (char*)Global_glob_go_univVars_objet_inventaire, 23, 8) > 0
                    || *(int*)AI_EvalVarArray2_C(1, (char*)Global_glob_go_univVars_objet_inventaire, 30, 8) > 0))
            {
                AI_EvalFunc_ARRBitSet_C((int*)(Global_glob_go_univVars_Flag_speciaux_sauve + 4), 1048);
            }
        }
        if (!AI_EvalFunc_ARRBitTest_C((int*)(Global_glob_go_univVars_Flag_speciaux_sauve + 4), 1051))
        {
            AI_EvalFunc_ARRBitClear_C((int*)(Global_glob_go_univVars_Flag_speciaux_sauve + 4), 1050);
            if (!*(uint32_t*)Global_glob_go_GST_Fight_Manager_gao_i_Evil_NbActor
                && *(float*)AI_EvalVarArray_C2((char*)Global_glob_go_univVars_Perso_ID_Energie_courante, 0) <= 20.0
                && !*(uint32_t*)(Global_glob_go_Perso_ID_status + 4)
                && !*(uint32_t*)Global_glob_go__PJ_Sally_gao_i_fight_on
                && *(uint32_t*)Global_glob_go_World_ID != 55
                && (*(int*)AI_EvalVarArray2_C(1, (char*)Global_glob_go_univVars_objet_inventaire, 13, 8) > 0
                    || *(int*)AI_EvalVarArray2_C(1, (char*)Global_glob_go_univVars_objet_inventaire, 23, 8) > 0
                    || *(int*)AI_EvalVarArray2_C(1, (char*)Global_glob_go_univVars_objet_inventaire, 30, 8) > 0))
            {
                AI_EvalFunc_ARRBitSet_C((int*)(Global_glob_go_univVars_Flag_speciaux_sauve + 4), 1050);
            }
        }
        v1->o_ambiance_acteur = 0;
        v1->i_blabla_priorite = 0;
        v1->o_acteur_cine_actif = 0;
        v1->o_acteur_blabla_actif = 0;
        v1->i_HlpTxt_Interdit_old = v1->i_HlpTxt_Interdit;
        v1->i_HlpTxt_Interdit = 0;
        if (AI_EvalFunc_TIMEElapsed_C(v1->f_time_active_avancement_jeu, 60.0))
        {
            v1->f_time_active_avancement_jeu = TIM_gf_MainClock - TIM_gf_LockTime;
            for (i = 0; i < v1->i_optim_nb_avancement; ++i)
            {
                v3 = (tGameObjectMain*)v1->ao_optim_dialogue[i];
                if (v3)
                {
                    if ((unsigned int)v3 <= 2)
                        v3 = AI_MainActor[(uint32_t)v3 - 1];
                    AI_EvalFunc_OBJ_FlagsControlSet_C(v3, 0, 0x600000);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagsControlSet_C(0, 0, 0x600000);
                }
            }
        }
        if (v1->i_mort_en_cours != v1->i_mort_en_cours_old)
        {
            AI_EvalFunc_SNDPlayModifier_C(AI_gpst_CurrentGameObject, 0);
            AI_EvalFunc_SNDPlayModifier_C(AI_gpst_CurrentGameObject, 1);
            ++* (uint32_t*)Global_glob_go_i_sally_nbmorts;
            *(uint32_t*)Global_glob_go_GST_Global_gao_f_joy_norm = 0;
            *(uint32_t*)Global_glob_go_GST_Global_gao_v_joy_camera = 0;
            *(uint32_t*)(Global_glob_go_GST_Global_gao_v_joy_camera + 4) = 0;
            *(uint32_t*)(Global_glob_go_GST_Global_gao_v_joy_camera + 8) = 0;
            *(uint32_t*)Global_glob_go_GST_Global_gao_v_joy_camera_normalized = 0;
            *(uint32_t*)(Global_glob_go_GST_Global_gao_v_joy_camera_normalized + 4) = 0;
            *(uint32_t*)(Global_glob_go_GST_Global_gao_v_joy_camera_normalized + 8) = 0;
            v1->f_vol_mort = 1.0;
            SND_MuteEnable(11, 1);
            AI_EvalFunc_AITrackStop_C(1);
            v14 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Global_glob_go_Global_glob_mort);
            if (v14 != 1000)
                return v14;
        }
        v1->i_mort_en_cours_old = v1->i_mort_en_cours;
        v1->i_nb_compagnon_en_suivi_old = v1->i_nb_compagnon_en_suivi;
        v1->i_nb_compagnon_en_suivi = 0;
        v1->i_id_compagnon_en_suivi_old = v1->i_id_compagnon_en_suivi;
        v1->i_id_compagnon_en_suivi = 0;
        *(float*)Global_glob_go_univVars_f_TempsDeJeuTotal = TIM_gf_dt
            + *(float*)Global_glob_go_univVars_f_TempsDeJeuTotal;
        v11 = AI_EvalFunc_ARRBitTest_C((int*)(Global_glob_go_univVars_Flag_speciaux_sauve + 4), 2083);
        if (!v11)
        {
            if (AI_EvalFunc_ARRBitTest_C((int*)(Global_glob_go_univVars_Flag_speciaux_sauve + 4), 4140))
            {
                v11 = 0;
            }
            else
            {
                v11 = AI_EvalFunc_ARRBitTest_C((int*)(Global_glob_go_univVars_Flag_speciaux_sauve + 4), 3800);
                if (v11 && *(uint32_t*)Global_glob_go_World_ID == 92)
                {
                    AI_EvalFunc_ARRBitSet_C((int*)(Global_glob_go_univVars_Flag_speciaux_sauve + 4), 4140);
                    v11 = 0;
                }
            }
        }
        if (!v11)
        {
            *(uint32_t*)Global_glob_go_univVars_f_Delai_Avant_Mort_Mili = 0;
            goto LABEL_100;
        }
        v17 = *(float*)Global_glob_go_univVars_f_Delai_Avant_Mort_Mili;
        if (AI_EvalFunc_MATHFloatNullEps_C(*(float*)Global_glob_go_univVars_f_Delai_Avant_Mort_Mili))
        {
            if (v1->f_time_reinit == 0.0)
            {
                if (AI_EvalFunc_ARRBitTest_C((int*)(Global_glob_go_univVars_Flag_speciaux_sauve + 4), 2083))
                    *(uint32_t*)Global_glob_go_univVars_f_Delai_Avant_Mort_Mili = 1127481344;
                else
                    *(uint32_t*)Global_glob_go_univVars_f_Delai_Avant_Mort_Mili = 1119092736;
            }
            else
            {
                *(float*)Global_glob_go_univVars_f_Delai_Avant_Mort_Mili = v1->f_time_reinit;
            }
            goto LABEL_100;
        }
        if (v17 <= 0.0)
        {
        LABEL_100:
            AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(0, 512);
            v1->i_mode_matrix_old = v1->i_mode_matrix;
            v1->i_mode_matrix = 0;
            AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(0, 3072);
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 16);
            v1->i_nb_materia_on = 0;
            v1->i_disable_hysteresis = 0;
            return 1000;
        }
        if (!AI_EvalFunc_OBJ_CapaTest_C((tGameObjectMain*)Global_glob_go_GST_Board_panel_gao, 0x100))
            v17 = v17 - TIM_gf_dt;
        if (v17 > 0.0)
        {
            *(float*)Global_glob_go_univVars_f_Delai_Avant_Mort_Mili = v17;
            goto LABEL_100;
        }
        *(uint32_t*)Global_glob_go_univVars_f_Delai_Avant_Mort_Mili = 0;
        if (*(int*)Global_glob_go_GST_Global_gao_i_fade_priority <= 5000
            || AI_EvalFunc_TIMEElapsed_C(
                *(float*)Global_glob_go_GST_Global_gao_f_fade_start_time,
                *(float*)Global_glob_go_GST_Global_gao_f_fade_delay))
        {
            *(float*)Global_glob_go_GST_Global_gao_f_fade_start_time = TIM_gf_MainClock - TIM_gf_LockTime;
            *(uint32_t*)Global_glob_go_GST_Global_gao_f_fade_delay = 0x41300000;
            *(uint32_t*)Global_glob_go_GST_Global_gao_i_fade_priority = 5000;
            AI_EvalFunc_GFXDel_C_CURRENT(10);
            a1 = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
            *(uint32_t*)Global_glob_go_univVars_i_fade_fx = a1;
            AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 2, 1);
            v2 = (tGameObjectMain*)Global_glob_go_INV_MatFade_gao;
            if (Global_glob_go_INV_MatFade_gao)
            {
                if ((unsigned int)Global_glob_go_INV_MatFade_gao <= 2)
                    v2 = AI_MainActor[Global_glob_go_INV_MatFade_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, v2, 9);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, 0, 9);
            }
            GFXSetCurGlobal(a1);
            AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(11101, 0xFF000000);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 1.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 10.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
        }
        AI_gpst_CurrentInstance->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -1.0;
    }
    if (AI_EvalKeyword_During_C())
    {
        *(float*)Global_glob_go__PJ_Camera_Target_gao_f_rot_local_y = *(float*)Global_glob_go__PJ_Camera_Target_gao_f_rot_local_y
            - TIM_gf_dt * 0.25f;
        *(uint32_t*)Global_glob_go_GST_Global_gao_f_game_speed = 0x3E800000;
        AI_EvalFunc_OBJ_CapaSet_C((tGameObjectMain*)Global_glob_go__PJ_Camera_Speciale_gao, 0, 0x100);
        AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)Global_glob_go_GST_Board_panel_gao, 0x300, 0);
        *AI_gppst_CurrentJumpNode = (tAINode*)39;
        result = 0;
    }
    else
    {
        if (AI_EvalFunc_ARRBitTest_C((int*)(Global_glob_go_univVars_Flag_speciaux_sauve + 4), 0x823)
            && *(float*)Global_glob_go_univVars_f_Delai_Avant_Mort_Mili < (double)*(float*)Global_glob_go_GST_Global_gao_f_time_reinit)
        {
            *(uint32_t*)Global_glob_go_univVars_f_Delai_Avant_Mort_Mili = *(uint32_t*)Global_glob_go_GST_Global_gao_f_time_reinit;
        }
        AI_EvalFunc_WORReinit_C();
        result = 3;
    }
    return result;
}

int Global_glob_exec_buttons()
{
    float* v1; // [esp+0h] [ebp-18h]
    tScriptStruct_GSTGlobal* v2; // [esp+4h] [ebp-14h]
    int a2; // [esp+Ch] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_GSTGlobal*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    for (a2 = 0; a2 < 6; ++a2)
    {
        if (INO_b_IoButtonPressed(a2))
        {
            v1 = (float*)AI_EvalVarArray_C2((char*)&v2->__f_button_time_Dimension1, a2);
            *v1 = TIM_gf_dt + *v1;
            v2->i_button_SpecialLongPress[a2] = *(float*)AI_EvalVarArray_C2((char*)&v2->__f_button_time_Dimension1, a2) > 0.2;
        }
        else
        {
            v2->i_button_SpecialJustPressed[a2] = *(float*)AI_EvalVarArray_C2((char*)&v2->__f_button_time_Dimension1, a2) > 0.0
                && *(float*)AI_EvalVarArray_C2((char*)&v2->__f_button_time_Dimension1, a2) < 0.2;
            v2->i_button_SpecialLongPress[a2] = 0;
            *(uint32_t*)AI_EvalVarArray_C2((char*)&v2->__f_button_time_Dimension1, a2) = 0;
        }
    }
    return 1000;
}

int Global_glob_exec_alarme()
{
    tScriptStruct_GSTGlobal* v1; // [esp+8h] [ebp-14h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GSTGlobal*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (v1->f_alarme_timer_start == 0.0)
    {
        v1->f_alarme_timer_start = TIM_gf_MainClock - TIM_gf_LockTime;
        AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(256, 0);
    }
    else if (AI_EvalFunc_OBJ_CustomBitsTest_C(AI_gpst_CurrentGameObject, 256)
        && AI_EvalFunc_TIMEElapsed_C(v1->f_alarme_timer_start, v1->f_alarme_timer_autooff))
    {
        AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(0, 0x100);
    }
    return 1000;
}

int Global_glob_exec_volumes_manager()
{
    float* v0; // eax
    float* v1; // eax
    float* v2; // eax
    float* v3; // eax
    float* v4; // eax
    float* v5; // eax
    float* v6; // eax
    float* v7; // eax
    tScriptStruct_GSTGlobal* v9; // [esp+8h] [ebp-18h]
    tGameObjectMain* v10; // [esp+10h] [ebp-10h]
    tGameObjectMain* v11; // [esp+14h] [ebp-Ch]
    tGameObjectMain* v12; // [esp+18h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v9 = (tScriptStruct_GSTGlobal*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v9 = 0;    
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Global_glob_exec_volumes_manager__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Global_glob_exec_volumes_manager_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Global_glob_exec_volumes_manager_univVars_i_engine_mode,
            (void*)Global_glob_exec_volumes_manager_univVars,
            28);
        AI2C_ResolveBuffer(
            &Global_glob_exec_volumes_manager_univVars_f_volume_infodesign,
            (void*)Global_glob_exec_volumes_manager_univVars,
            20264);
        AI2C_ResolveBuffer(
            &Global_glob_exec_volumes_manager_univVars_f_volume_inventaire,
            (void*)Global_glob_exec_volumes_manager_univVars,
            20248);
    }
    v12 = (tGameObjectMain*)v9->o_micro;
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
        v10 = (tGameObjectMain*)v9->o_micro;
        if (v10)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            AI_EvalFunc_SNDMicroPosSet_C(v10);
        }
        else
        {
            AI_EvalFunc_SNDMicroPosSet_C(0);
        }
        v9->o_micro = 0;
    }
    else
    {
        AI_EvalFunc_SNDMicroPosSet_C((tGameObjectMain*)Global_glob_exec_volumes_manager__PJ_Sally_gao);
    }
    if (!*(uint32_t*)Global_glob_exec_volumes_manager_univVars_i_engine_mode)
    {
        if (*(float*)AI_EvalVarArray_C2((char*)Global_glob_exec_volumes_manager_univVars_f_volume_infodesign, 0) < 0.0)
        {
            v1 = (float*)AI_EvalVarArray_C2((char*)Global_glob_exec_volumes_manager_univVars_f_volume_inventaire, 0);
            SND_SetGlobalVol(*v1);
        }
        else
        {
            v0 = (float*)AI_EvalVarArray_C2((char*)Global_glob_exec_volumes_manager_univVars_f_volume_infodesign, 0);
            SND_SetGlobalVol(*v0);
            *(uint32_t*)AI_EvalVarArray_C2((char*)Global_glob_exec_volumes_manager_univVars_f_volume_infodesign, 0) = 0xBF800000;
        }
        SND_MuteEnable(7, 1);
        SND_MuteEnable(8, 1);
        if (*(float*)AI_EvalVarArray_C2((char*)Global_glob_exec_volumes_manager_univVars_f_volume_infodesign, 1) < 0.0)
        {
            v4 = (float*)AI_EvalVarArray_C2((char*)Global_glob_exec_volumes_manager_univVars_f_volume_inventaire, 1);
            SND_MuteSetUp(7, *v4, 1.0);
            v5 = (float*)AI_EvalVarArray_C2((char*)Global_glob_exec_volumes_manager_univVars_f_volume_inventaire, 1);
            SND_MuteSetUp(8, *v5, 1.0);
        }
        else
        {
            v2 = (float*)AI_EvalVarArray_C2((char*)Global_glob_exec_volumes_manager_univVars_f_volume_infodesign, 1);
            SND_MuteSetUp(7, *v2, 1.0);
            v3 = (float*)AI_EvalVarArray_C2((char*)Global_glob_exec_volumes_manager_univVars_f_volume_infodesign, 1);
            SND_MuteSetUp(8, *v3, 1.0);
            *(uint32_t*)AI_EvalVarArray_C2((char*)Global_glob_exec_volumes_manager_univVars_f_volume_infodesign, 1) = 0xBF800000;
        }
        SND_MuteEnable(9, 1);
        if (*(float*)AI_EvalVarArray_C2((char*)Global_glob_exec_volumes_manager_univVars_f_volume_infodesign, 2) < 0.0)
        {
            v7 = (float*)AI_EvalVarArray_C2((char*)Global_glob_exec_volumes_manager_univVars_f_volume_inventaire, 2);
            SND_MuteSetUp(9, *v7, 1.0);
        }
        else
        {
            v6 = (float*)AI_EvalVarArray_C2((char*)Global_glob_exec_volumes_manager_univVars_f_volume_infodesign, 2);
            SND_MuteSetUp(9, *v6, 1.0);
            *(uint32_t*)AI_EvalVarArray_C2((char*)Global_glob_exec_volumes_manager_univVars_f_volume_infodesign, 2) = 0xBF800000;
        }
    }
    return 1000;
}

int Global_glob_exec_music_machine()
{
    float v1; // [esp+0h] [ebp-4Ch]
    float v2; // [esp+4h] [ebp-48h]
    float v3; // [esp+8h] [ebp-44h]
    float v4; // [esp+Ch] [ebp-40h]
    float v5; // [esp+10h] [ebp-3Ch]
    float v6; // [esp+14h] [ebp-38h]
    float v7; // [esp+18h] [ebp-34h]
    float v8; // [esp+1Ch] [ebp-30h]
    tScriptStruct_GSTGlobal* v9; // [esp+20h] [ebp-2Ch]
    float v10; // [esp+24h] [ebp-28h]
    float v11; // [esp+28h] [ebp-24h]
    float v12; // [esp+2Ch] [ebp-20h]
    float v13; // [esp+30h] [ebp-1Ch]
    float v14; // [esp+38h] [ebp-14h]
    float v15; // [esp+3Ch] [ebp-10h]
    int i; // [esp+48h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v9 = (tScriptStruct_GSTGlobal*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v9 = 0;
    for (i = 0; i < 4; ++i)
    {
        if (v9->i_snd_mm_instance[i] != -1)
        {
            v8 = *(float*)AI_EvalVarArray_C2((char*)&v9->__f_snd_mm_volume_tgt_Dimension1, i);
            if (*(float*)AI_EvalVarArray_C2((char*)&v9->__f_snd_mm_volume_cur_Dimension1, i) != v8)
            {
                v7 = *(float*)AI_EvalVarArray_C2((char*)&v9->__f_snd_mm_volume_cur_Dimension1, i);
                if (*(float*)AI_EvalVarArray_C2((char*)&v9->__f_snd_mm_volume_tgt_Dimension1, i) >= (double)v7)
                {
                    v15 = TIM_gf_dt;
                    v11 = *(float*)AI_EvalVarArray_C2((char*)&v9->__f_snd_mm_volume_tgt_Dimension1, i);
                    v4 = *(float*)AI_EvalVarArray_C2((char*)&v9->__f_snd_mm_vitessefade_Dimension1, i) * v15;
                    v10 = *(float*)AI_EvalVarArray_C2((char*)&v9->__f_snd_mm_volume_cur_Dimension1, i) + v4;
                    if (v10 > (double)v11)
                        v3 = v11;
                    else
                        v3 = v10;
                    *(float*)AI_EvalVarArray_C2((char*)&v9->__f_snd_mm_volume_cur_Dimension1, i) = v3;
                }
                else
                {
                    v14 = TIM_gf_dt;
                    v13 = *(float*)AI_EvalVarArray_C2((char*)&v9->__f_snd_mm_volume_tgt_Dimension1, i);
                    v6 = *(float*)AI_EvalVarArray_C2((char*)&v9->__f_snd_mm_vitessefade_Dimension1, i) * v14;
                    v12 = *(float*)AI_EvalVarArray_C2((char*)&v9->__f_snd_mm_volume_cur_Dimension1, i) - v6;
                    if (v12 < (double)v13)
                        v5 = v13;
                    else
                        v5 = v12;
                    *(float*)AI_EvalVarArray_C2((char*)&v9->__f_snd_mm_volume_cur_Dimension1, i) = v5;
                }
                v2 = *(float*)AI_EvalVarArray_C2((char*)&v9->__f_snd_mm_volume_cur_Dimension1, i);
                v1 = *(float*)AI_EvalVarArray_C2((char*)&v9->__f_snd_mm_volume_ref_Dimension1, i) * v2;
                SND_SetVol(v9->i_snd_mm_instance[i], v1);
            }
        }
    }
    return 1000;
}

int Global_glob_exec_world_cycle()
{
    int result; // eax
    float v1; // [esp+0h] [ebp-48h]
    float v2; // [esp+4h] [ebp-44h]
    char* v3; // [esp+8h] [ebp-40h]
    tScriptStruct_GSTGlobal* v4; // [esp+Ch] [ebp-3Ch]
    double v5; // [esp+10h] [ebp-38h]
    float v6; // [esp+20h] [ebp-28h]
    float v7; // [esp+20h] [ebp-28h]
    float v8; // [esp+20h] [ebp-28h]
    float v9; // [esp+20h] [ebp-28h]
    float v10; // [esp+20h] [ebp-28h]
    float v11; // [esp+24h] [ebp-24h]
    int v12; // [esp+30h] [ebp-18h]
    int v13; // [esp+30h] [ebp-18h]
    float v14; // [esp+3Ch] [ebp-Ch]
    float v15; // [esp+3Ch] [ebp-Ch]
    float v16; // [esp+3Ch] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_GSTGlobal*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Global_glob_exec_world_cycle_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Global_glob_exec_world_cycle_univVars_Planetes_CurHour,
            (void*)Global_glob_exec_world_cycle_univVars,
            6116);
        AI2C_ResolveBuffer(
            &Global_glob_exec_world_cycle_univVars_Planetes_CurHourOld,
            (void*)Global_glob_exec_world_cycle_univVars,
            6120);
        AI2C_ResolveBuffer(
            &Global_glob_exec_world_cycle_univVars_Planetes_CurRotation,
            (void*)Global_glob_exec_world_cycle_univVars,
            5984);
        Global_glob_exec_world_cycle_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &Global_glob_exec_world_cycle_univVars_Flag_speciaux_sauve,
            (void*)Global_glob_exec_world_cycle_univVars,
            4488);
        Global_glob_exec_world_cycle_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Global_glob_exec_world_cycle_GST_Global_gao_Planetes_Rotations,
            (void*)Global_glob_exec_world_cycle_GST_Global_gao,
            2784);
        AI2C_ResolveBuffer(
            &Global_glob_exec_world_cycle_univVars_Planetes_CurDay,
            (void*)Global_glob_exec_world_cycle_univVars,
            6124);
    }

    if (v4->f_Hour_Set >= 0.0)
    {
        *(float*)Global_glob_exec_world_cycle_univVars_Planetes_CurHour = v4->f_Hour_Set;
        *(float*)Global_glob_exec_world_cycle_univVars_Planetes_CurHourOld = v4->f_Hour_Set;
        v6 = v4->f_Hour_Set - 12.0f;
        if (v6 < 0.0)
            v6 = v6 + 24.0f;
        v7 = v6 * 6.283185f / 24.0f;
        *(float*)AI_EvalVarArray_C2((char*)Global_glob_exec_world_cycle_univVars_Planetes_CurRotation, 0) = v7;
        v4->f_Hour_Set = -1.0;
    }
    if (AI_EvalFunc_OBJ_CapaTest_C((tGameObjectMain*)Global_glob_exec_world_cycle_GST_Board_panel_gao, 256))
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else if (AI_EvalFunc_ARRBitTest_C((int*)(Global_glob_exec_world_cycle_univVars_Flag_speciaux_sauve + 4), 41))
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        v8 = *(float*)AI_EvalVarArray_C2(
            (char*)Global_glob_exec_world_cycle_GST_Global_gao_Planetes_Rotations,
            v4->des_i_numplanete);
        if (*(float*)Global_glob_exec_world_cycle_univVars_Planetes_CurHour <= 6.0
            || *(float*)Global_glob_exec_world_cycle_univVars_Planetes_CurHour >= 18.0)
        {
            v9 = v8 * 3.0f;
        }
        else
        {
            v9 = v8 + v8;
        }
        v10 = v9 * TIM_gf_dt;
        v3 = AI_EvalVarArray_C2((char*)Global_glob_exec_world_cycle_univVars_Planetes_CurRotation, v4->des_i_numplanete);
        *(float*)v3 = v10 + *(float*)v3;
        v11 = *(float*)AI_EvalVarArray_C2(
            (char*)Global_glob_exec_world_cycle_univVars_Planetes_CurRotation,
            v4->des_i_numplanete)
            / 1.570796f;
        v5 = v11 - 0.5 + MATH_gd_magicDoubleForRound;
        v2 = (float)(*(int32_t*)&v5);
        v14 = v2 * 1.570796f;
        v12 = AI_EvalFunc_MTHModulo_C((*(int32_t *)&v5), 4);
        v15 = *(float*)AI_EvalVarArray_C2(
            (char*)Global_glob_exec_world_cycle_univVars_Planetes_CurRotation,
            v4->des_i_numplanete)
            - v14;
        if (v12)
        {
            if (v12 == 1)
            {
                v13 = 18;
            }
            else if (v12 == 2)
            {
                v13 = 0;
            }
            else
            {
                v13 = 6;
            }
        }
        else
        {
            v13 = 12;
        }
        v1 = (float)v13;
        v16 = v15 * 6.0f / 1.570796f + v1;
        *(float*)Global_glob_exec_world_cycle_univVars_Planetes_CurHour = v16;
        if (*(float*)Global_glob_exec_world_cycle_univVars_Planetes_CurHourOld > (double)*(float*)Global_glob_exec_world_cycle_univVars_Planetes_CurHour)
            ++*(uint32_t*)Global_glob_exec_world_cycle_univVars_Planetes_CurDay;
        *(uint32_t*)Global_glob_exec_world_cycle_univVars_Planetes_CurHourOld = *(uint32_t*)Global_glob_exec_world_cycle_univVars_Planetes_CurHour;
        result = 1000;
    }
    return result;
}

int Global_glob_mort()
{
    tScriptStruct_GSTGlobal* v1;
    float v2;
    float v3;

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GSTGlobal*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    AI_EvalFunc_IoJoyFreeze_C(-2);
    v2 = 0.1f;
    if (0.1 < 0.0)
        v2 = 0.0;
    if (v2 > 1.0)
        v2 = 1.0;
    v3 = 0.0f * v2 + v1->f_vol_mort * (1.0f - v2);
    v1->f_vol_mort = v3;
    SND_MuteSetUp(11, 1.0, v1->f_vol_mort);       // f_vol_mort
    return 1000;
}

int Global_glob_exec_game_speed()
{
    tScriptStruct_GSTGlobal* v1;
    float a1;

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GSTGlobal*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (v1->f_cheat_speed == 1.0)
        a1 = v1->f_game_speed;
    else
        a1 = v1->f_cheat_speed;
    v1->f_old_old_game_speed = v1->f_old_game_speed;
    if (v1->f_old_game_speed != a1)
    {
        AI_EvalFunc_TIMEFactorSet_C(a1);
        v1->f_old_game_speed = a1;
    }
    v1->f_game_speed = 1.0;
    return 1000;
}

int Global_glob_exec_event_sound()
{
    float v1;
    tScriptStruct_GSTGlobal* v2;
    float v3;
    float v4;
    int v5;
    int a2;

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_GSTGlobal*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v4 = TIM_gf_dt;
    a2 = 0;
    v5 = v2->i_event_sound_nbr;
    while (a2 < 20 && v5 > 0)
    {
        if (*(float*)AI_EvalVarArray_C2((char*)&v2->__f_event_sound_time_Dimension1, a2) != 0.0)
        {
            --v5;
            v3 = *(float*)AI_EvalVarArray_C2((char*)&v2->__f_event_sound_time_Dimension1, a2) - v4;
            if (v3 > 0.0)
                v1 = v3;
            else
                v1 = 0.0;
            *(float*)AI_EvalVarArray_C2((char*)&v2->__f_event_sound_time_Dimension1, a2) = v1;
            if (*(float*)AI_EvalVarArray_C2((char*)&v2->__f_event_sound_time_Dimension1, a2) == 0.0)
                --v2->i_event_sound_nbr;
        }
        ++a2;
    }
    return 1000;
}

int Global_glob_raz_univers()
{
    int i; // [esp+20h] [ebp-28h]
    int j; // [esp+30h] [ebp-18h]
    int k; // [esp+44h] [ebp-4h]

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Global_glob_raz_univers_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&Global_glob_raz_univers_univVars_i_engine_mode, (void*)Global_glob_raz_univers_univVars, 28);
        AI2C_ResolveBuffer(&Global_glob_raz_univers_univVars_load_world, (void*)Global_glob_raz_univers_univVars, 0);
        AI2C_ResolveBuffer(&Global_glob_raz_univers_univVars_world_ID, (void*)Global_glob_raz_univers_univVars, 4);
        AI2C_ResolveBuffer(&Global_glob_raz_univers_univVars_world_key, (void*)Global_glob_raz_univers_univVars, 20);
        AI2C_ResolveBuffer(&Global_glob_raz_univers_univVars_world_loaded_id, (void*)Global_glob_raz_univers_univVars, 24);
        AI2C_ResolveBuffer(
            &Global_glob_raz_univers_univVars_Perso_ID_world_ID,
            (void*)Global_glob_raz_univers_univVars,
            536);
        AI2C_ResolveBuffer(&Global_glob_raz_univers_univVars_Perso_ID_pos, (void*)Global_glob_raz_univers_univVars, 588);
        AI2C_ResolveBuffer(&Global_glob_raz_univers_univVars_Perso_ID_sight, (void*)Global_glob_raz_univers_univVars, 736);
        AI2C_ResolveBuffer(&Global_glob_raz_univers_univVars_Perso_ID_status, (void*)Global_glob_raz_univers_univVars, 484);
        AI2C_ResolveBuffer(&Global_glob_raz_univers_univVars_Perso_ID_actor, (void*)Global_glob_raz_univers_univVars, 988);
        AI2C_ResolveBuffer(
            &Global_glob_raz_univers_univVars_Perso_ID_Companion,
            (void*)Global_glob_raz_univers_univVars,
            1040);
        AI2C_ResolveBuffer(
            &Global_glob_raz_univers_univVars_deguisement_world_ID,
            (void*)Global_glob_raz_univers_univVars,
            1216);
        AI2C_ResolveBuffer(
            &Global_glob_raz_univers_univVars_deguisement_pos,
            (void*)Global_glob_raz_univers_univVars,
            1220);
        AI2C_ResolveBuffer(
            &Global_glob_raz_univers_univVars_deguisement_sight,
            (void*)Global_glob_raz_univers_univVars,
            1232);
        AI2C_ResolveBuffer(
            &Global_glob_raz_univers_univVars_deguisement_status,
            (void*)Global_glob_raz_univers_univVars,
            1212);
        AI2C_ResolveBuffer(
            &Global_glob_raz_univers_univVars_objet_inventaire,
            (void*)Global_glob_raz_univers_univVars,
            1480);
        AI2C_ResolveBuffer(&Global_glob_raz_univers_univVars_Carte_i_NbSpot, (void*)Global_glob_raz_univers_univVars, 4472);
    }
    if (!*(uint32_t*)Global_glob_raz_univers_univVars_i_engine_mode)
    {
        *(uint32_t*)Global_glob_raz_univers_univVars_load_world = 0;
        *(uint32_t*)Global_glob_raz_univers_univVars_world_ID = -1;
        *(uint32_t*)Global_glob_raz_univers_univVars_world_key = 0;
        *(uint32_t*)Global_glob_raz_univers_univVars_world_loaded_id = 0;
        for (i = 0; i < 12; ++i)
        {
            *(uint32_t*)(Global_glob_raz_univers_univVars_Perso_ID_world_ID + 4 * i + 4) = 0;
            *(uint32_t*)AI_EvalVarArray_C3((char*)Global_glob_raz_univers_univVars_Perso_ID_pos, i) = 0;
            *((uint32_t*)AI_EvalVarArray_C3((char*)Global_glob_raz_univers_univVars_Perso_ID_pos, i) + 1) = 0;
            *((uint32_t*)AI_EvalVarArray_C3((char*)Global_glob_raz_univers_univVars_Perso_ID_pos, i) + 2) = 0;
            *(uint32_t*)AI_EvalVarArray_C3((char*)Global_glob_raz_univers_univVars_Perso_ID_sight, i) = 0;
            *((uint32_t*)AI_EvalVarArray_C3((char*)Global_glob_raz_univers_univVars_Perso_ID_sight, i) + 1) = 0;
            *((uint32_t*)AI_EvalVarArray_C3((char*)Global_glob_raz_univers_univVars_Perso_ID_sight, i) + 2) = 0;
            *(uint32_t*)(Global_glob_raz_univers_univVars_Perso_ID_status + 4 * i + 4) = 0;
            *(uint32_t*)(Global_glob_raz_univers_univVars_Perso_ID_actor + 4 * i + 4) = 0;
        }
        *(uint32_t*)Global_glob_raz_univers_univVars_Perso_ID_Companion = -1;
        *(uint32_t*)Global_glob_raz_univers_univVars_deguisement_world_ID = 0;
        *(uint32_t*)Global_glob_raz_univers_univVars_deguisement_pos = 0;
        *(uint32_t*)(Global_glob_raz_univers_univVars_deguisement_pos + 4) = 0;
        *(uint32_t*)(Global_glob_raz_univers_univVars_deguisement_pos + 8) = 0;
        *(uint32_t*)Global_glob_raz_univers_univVars_deguisement_sight = 0;
        *(uint32_t*)(Global_glob_raz_univers_univVars_deguisement_sight + 4) = 0;
        *(uint32_t*)(Global_glob_raz_univers_univVars_deguisement_sight + 8) = 0;
        *(uint32_t*)Global_glob_raz_univers_univVars_deguisement_status = 0;
        *(uint32_t*)(Global_glob_raz_univers_univVars_Perso_ID_status + 36) = 0;
        for (j = 0; j < 12; ++j)
        {
            for (k = 0; k < 50; ++k)
                *(uint32_t*)AI_EvalVarArray2_C(1, (char*)Global_glob_raz_univers_univVars_objet_inventaire, k, j) = 0;
        }
    }
    *(uint32_t*)Global_glob_raz_univers_univVars_Carte_i_NbSpot = 0;
    return 1000;
}

int Global_glob_reflex()
{
    int v0; // eax
    int result; // eax
    char* v2; // eax
    char* v3; // eax
    char* v4; // eax
    char* v5; // eax
    tGameObjectMain* v6; // eax
    int v7; // eax
    int v8; // eax
    int v9; // eax
    int v10; // eax
    char* v11; // eax
    char* v12; // eax
    char* v13; // eax
    char* v14; // eax
    char* v15; // eax
    char* v16; // eax
    char* v17; // eax
    Vec3* v18; // [esp+0h] [ebp-678h]
    float v19; // [esp+8h] [ebp-670h]
    float v20; // [esp+Ch] [ebp-66Ch]
    int v21; // [esp+10h] [ebp-668h]
    float v22; // [esp+14h] [ebp-664h]
    float v23; // [esp+18h] [ebp-660h]
    int v24; // [esp+1Ch] [ebp-65Ch]
    int v25; // [esp+20h] [ebp-658h]
    int v26; // [esp+24h] [ebp-654h]
    int v27; // [esp+28h] [ebp-650h]
    int v28; // [esp+2Ch] [ebp-64Ch]
    char* v29; // [esp+30h] [ebp-648h]
    int v30; // [esp+34h] [ebp-644h]
    int v31; // [esp+38h] [ebp-640h]
    int v32; // [esp+3Ch] [ebp-63Ch]
    float v33; // [esp+40h] [ebp-638h]
    float v34; // [esp+44h] [ebp-634h]
    int v35; // [esp+48h] [ebp-630h]
    float v36; // [esp+4Ch] [ebp-62Ch]
    float v37; // [esp+50h] [ebp-628h]
    int v38; // [esp+54h] [ebp-624h]
    char* v39; // [esp+58h] [ebp-620h]
    float v40; // [esp+60h] [ebp-618h]
    float v41; // [esp+64h] [ebp-614h]
    float v42; // [esp+68h] [ebp-610h]
    float v43; // [esp+70h] [ebp-608h]
    tScriptStruct_GSTGlobal* v44; // [esp+7Ch] [ebp-5FCh]
    tGameObjectMain* v45; // [esp+80h] [ebp-5F8h]
    tGameObjectMain* v46; // [esp+84h] [ebp-5F4h]
    tGameObjectMain* v47; // [esp+88h] [ebp-5F0h]
    tGameObjectMain* v48; // [esp+8Ch] [ebp-5ECh]
    tGameObjectMain* v49; // [esp+90h] [ebp-5E8h]
    tGameObjectMain* v50; // [esp+94h] [ebp-5E4h]
    tGameObjectMain* v51; // [esp+98h] [ebp-5E0h]
    tGameObjectMain* v52; // [esp+9Ch] [ebp-5DCh]
    tGameObjectMain* v53; // [esp+A0h] [ebp-5D8h]
    tGameObjectMain* v54; // [esp+A4h] [ebp-5D4h]
    tGameObjectMain* v55; // [esp+A8h] [ebp-5D0h]
    tGameObjectMain* v56; // [esp+ACh] [ebp-5CCh]
    tGameObjectMain* v57; // [esp+B0h] [ebp-5C8h]
    tGameObjectMain* v58; // [esp+B4h] [ebp-5C4h]
    char* v59; // [esp+B8h] [ebp-5C0h]
    char* v60; // [esp+C4h] [ebp-5B4h]
    double v61; // [esp+D0h] [ebp-5A8h]
    uint32_t* v62; // [esp+D8h] [ebp-5A0h]
    uint32_t* v63; // [esp+E0h] [ebp-598h]
    uint32_t* v64; // [esp+E8h] [ebp-590h]
    uint32_t* v65; // [esp+F0h] [ebp-588h]
    tGameObjectMain* v66; // [esp+108h] [ebp-570h]
    tGameObjectMain* v67; // [esp+110h] [ebp-568h]
    tAI* v68; // [esp+114h] [ebp-564h]
    tAI* v69; // [esp+118h] [ebp-560h]
    tGameObjectMain* v70; // [esp+11Ch] [ebp-55Ch]
    tGameObjectMain* v71; // [esp+120h] [ebp-558h]
    tAI* v72; // [esp+124h] [ebp-554h]
    tAI* v73; // [esp+128h] [ebp-550h]
    tGameObjectMain* v74; // [esp+12Ch] [ebp-54Ch]
    tGameObjectMain* v75; // [esp+130h] [ebp-548h]
    tAI* v76; // [esp+134h] [ebp-544h]
    tAI* v77; // [esp+138h] [ebp-540h]
    tGameObjectMain* v78; // [esp+13Ch] [ebp-53Ch]
    tGameObjectMain* v79; // [esp+140h] [ebp-538h]
    tAI* v80; // [esp+144h] [ebp-534h]
    tAI* v81; // [esp+148h] [ebp-530h]
    tGameObjectMain* v82; // [esp+14Ch] [ebp-52Ch]
    tGameObjectMain* v83; // [esp+150h] [ebp-528h]
    tAI* v84; // [esp+154h] [ebp-524h]
    tAI* v85; // [esp+158h] [ebp-520h]
    tGameObjectMain* v86; // [esp+15Ch] [ebp-51Ch]
    tGameObjectMain* v87; // [esp+160h] [ebp-518h]
    tAI* v88; // [esp+164h] [ebp-514h]
    tAI* v89; // [esp+168h] [ebp-510h]
    tGameObjectMain* v90; // [esp+16Ch] [ebp-50Ch]
    tGameObjectMain* v91; // [esp+170h] [ebp-508h]
    tAI* v92; // [esp+174h] [ebp-504h]
    tAI* v93; // [esp+178h] [ebp-500h]
    tGameObjectMain* v94; // [esp+17Ch] [ebp-4FCh]
    tGameObjectMain* v95; // [esp+180h] [ebp-4F8h]
    tAI* v96; // [esp+184h] [ebp-4F4h]
    tAI* v97; // [esp+188h] [ebp-4F0h]
    tGameObjectMain* v98; // [esp+18Ch] [ebp-4ECh]
    tGameObjectMain* v99; // [esp+190h] [ebp-4E8h]
    tGameObjectMain* v100; // [esp+198h] [ebp-4E0h]
    int* v101; // [esp+19Ch] [ebp-4DCh]
    tGameObjectMain* v102; // [esp+1A0h] [ebp-4D8h]
    tGameObjectMain* v103; // [esp+1A4h] [ebp-4D4h]
    tGameObjectMain* v104; // [esp+1ACh] [ebp-4CCh]
    tGameObjectMain* v105; // [esp+1B4h] [ebp-4C4h]
    tGameObjectMain* v106; // [esp+1B8h] [ebp-4C0h]
    tGameObjectMain* v107; // [esp+1BCh] [ebp-4BCh]
    tGameObjectMain* v108; // [esp+1C8h] [ebp-4B0h]
    tGameObjectMain* v109; // [esp+1D0h] [ebp-4A8h]
    tGameObjectMain* v110; // [esp+1D4h] [ebp-4A4h]
    tAI* v111; // [esp+1DCh] [ebp-49Ch]
    tGameObjectMain* v112; // [esp+1E0h] [ebp-498h]
    tGameObjectMain* v113; // [esp+1E4h] [ebp-494h]
    tGameObjectMain* v114; // [esp+1F0h] [ebp-488h]
    tGameObjectMain* v115; // [esp+1F8h] [ebp-480h]
    tGameObjectMain* v116; // [esp+1FCh] [ebp-47Ch]
    tAI* v117; // [esp+204h] [ebp-474h]
    tGameObjectMain* v118; // [esp+208h] [ebp-470h]
    tGameObjectMain* v119; // [esp+20Ch] [ebp-46Ch]
    tGameObjectMain* v120; // [esp+218h] [ebp-460h]
    tGameObjectMain* v121; // [esp+220h] [ebp-458h]
    tGameObjectMain* v122; // [esp+224h] [ebp-454h]
    tAI* v123; // [esp+22Ch] [ebp-44Ch]
    tGameObjectMain* v124; // [esp+230h] [ebp-448h]
    tGameObjectMain* v125; // [esp+234h] [ebp-444h]
    tGameObjectMain* v126; // [esp+238h] [ebp-440h]
    tGameObjectMain* v127; // [esp+23Ch] [ebp-43Ch]
    tGameObjectMain* v128; // [esp+248h] [ebp-430h]
    tGameObjectMain* v129; // [esp+250h] [ebp-428h]
    tGameObjectMain* v130; // [esp+254h] [ebp-424h]
    tGameObjectMain* v131; // [esp+264h] [ebp-414h]
    tGameObjectMain* v132; // [esp+26Ch] [ebp-40Ch]
    tGameObjectMain* v133; // [esp+270h] [ebp-408h]
    tAI* v134; // [esp+278h] [ebp-400h]
    tGameObjectMain* v135; // [esp+27Ch] [ebp-3FCh]
    tGameObjectMain* v136; // [esp+280h] [ebp-3F8h]
    tAI* v137; // [esp+284h] [ebp-3F4h]
    tGameObjectMain* v138; // [esp+288h] [ebp-3F0h]
    tGameObjectMain* v139; // [esp+28Ch] [ebp-3ECh]
    tAI* v140; // [esp+290h] [ebp-3E8h]
    tGameObjectMain* v141; // [esp+294h] [ebp-3E4h]
    tGameObjectMain* v142; // [esp+298h] [ebp-3E0h]
    tAI* v143; // [esp+29Ch] [ebp-3DCh]
    tGameObjectMain* v144; // [esp+2A0h] [ebp-3D8h]
    tGameObjectMain* v145; // [esp+2A4h] [ebp-3D4h]
    tAI* v146; // [esp+2A8h] [ebp-3D0h]
    tGameObjectMain* v147; // [esp+2ACh] [ebp-3CCh]
    tGameObjectMain* v148; // [esp+2B0h] [ebp-3C8h]
    tAI* v149; // [esp+2B4h] [ebp-3C4h]
    tGameObjectMain* v150; // [esp+2B8h] [ebp-3C0h]
    tGameObjectMain* v151; // [esp+2BCh] [ebp-3BCh]
    tAI* v152; // [esp+2C0h] [ebp-3B8h]
    tGameObjectMain* v153; // [esp+2C4h] [ebp-3B4h]
    tGameObjectMain* v154; // [esp+2C8h] [ebp-3B0h]
    tAI* v155; // [esp+2CCh] [ebp-3ACh]
    tGameObjectMain* v156; // [esp+2D0h] [ebp-3A8h]
    tGameObjectMain* v157; // [esp+2D4h] [ebp-3A4h]
    tAI* v158; // [esp+2D8h] [ebp-3A0h]
    tGameObjectMain* v159; // [esp+2DCh] [ebp-39Ch]
    tGameObjectMain* v160; // [esp+2E0h] [ebp-398h]
    tAI* v161; // [esp+2E4h] [ebp-394h]
    tGameObjectMain* v162; // [esp+2E8h] [ebp-390h]
    tGameObjectMain* v163; // [esp+2ECh] [ebp-38Ch]
    tAI* v164; // [esp+2F0h] [ebp-388h]
    tGameObjectMain* v165; // [esp+2F4h] [ebp-384h]
    tGameObjectMain* v166; // [esp+2F8h] [ebp-380h]
    tAI* v167; // [esp+2FCh] [ebp-37Ch]
    tGameObjectMain* v168; // [esp+300h] [ebp-378h]
    tGameObjectMain* v169; // [esp+304h] [ebp-374h]
    tAI* v170; // [esp+308h] [ebp-370h]
    tGameObjectMain* v171; // [esp+30Ch] [ebp-36Ch]
    tGameObjectMain* v172; // [esp+310h] [ebp-368h]
    tAI* v173; // [esp+314h] [ebp-364h]
    tGameObjectMain* v174; // [esp+318h] [ebp-360h]
    tGameObjectMain* v175; // [esp+31Ch] [ebp-35Ch]
    tAI* v176; // [esp+320h] [ebp-358h]
    tGameObjectMain* v177; // [esp+324h] [ebp-354h]
    tGameObjectMain* v178; // [esp+328h] [ebp-350h]
    tAI* v179; // [esp+32Ch] [ebp-34Ch]
    tGameObjectMain* v180; // [esp+330h] [ebp-348h]
    tGameObjectMain* v181; // [esp+334h] [ebp-344h]
    tAI* v182; // [esp+338h] [ebp-340h]
    tGameObjectMain* v183; // [esp+33Ch] [ebp-33Ch]
    tGameObjectMain* v184; // [esp+340h] [ebp-338h]
    tAI* v185; // [esp+344h] [ebp-334h]
    tGameObjectMain* v186; // [esp+348h] [ebp-330h]
    tGameObjectMain* v187; // [esp+34Ch] [ebp-32Ch]
    tAI* v188; // [esp+350h] [ebp-328h]
    tGameObjectMain* v189; // [esp+354h] [ebp-324h]
    tGameObjectMain* v190; // [esp+358h] [ebp-320h]
    tAI* v191; // [esp+35Ch] [ebp-31Ch]
    tGameObjectMain* v192; // [esp+360h] [ebp-318h]
    tGameObjectMain* v193; // [esp+364h] [ebp-314h]
    tGameObjectMain* v194; // [esp+368h] [ebp-310h]
    tGameObjectMain* v195; // [esp+36Ch] [ebp-30Ch]
    tGameObjectMain* v196; // [esp+384h] [ebp-2F4h]
    tGameObjectMain* v197; // [esp+38Ch] [ebp-2ECh]
    tAI* v198; // [esp+3A0h] [ebp-2D8h]
    tAI* v199; // [esp+3A4h] [ebp-2D4h]
    tGameObjectMain* v200; // [esp+3A8h] [ebp-2D0h]
    tGameObjectMain* v201; // [esp+3ACh] [ebp-2CCh]
    tAI* v202; // [esp+3B0h] [ebp-2C8h]
    tAI* v203; // [esp+3B4h] [ebp-2C4h]
    tGameObjectMain* v204; // [esp+3B8h] [ebp-2C0h]
    tGameObjectMain* v205; // [esp+3BCh] [ebp-2BCh]
    tAI* v206; // [esp+3C0h] [ebp-2B8h]
    tAI* v207; // [esp+3C4h] [ebp-2B4h]
    tGameObjectMain* v208; // [esp+3C8h] [ebp-2B0h]
    tGameObjectMain* v209; // [esp+3CCh] [ebp-2ACh]
    tAI* v210; // [esp+3D0h] [ebp-2A8h]
    tAI* v211; // [esp+3D4h] [ebp-2A4h]
    tGameObjectMain* v212; // [esp+3D8h] [ebp-2A0h]
    tGameObjectMain* v213; // [esp+3DCh] [ebp-29Ch]
    tAI* v214; // [esp+3E0h] [ebp-298h]
    tAI* v215; // [esp+3E4h] [ebp-294h]
    tGameObjectMain* v216; // [esp+3E8h] [ebp-290h]
    tGameObjectMain* v217; // [esp+3ECh] [ebp-28Ch]
    tAI* v218; // [esp+3F0h] [ebp-288h]
    tAI* v219; // [esp+3F4h] [ebp-284h]
    tGameObjectMain* v220; // [esp+3F8h] [ebp-280h]
    tGameObjectMain* v221; // [esp+3FCh] [ebp-27Ch]
    tAI* v222; // [esp+400h] [ebp-278h]
    tAI* v223; // [esp+404h] [ebp-274h]
    tGameObjectMain* v224; // [esp+408h] [ebp-270h]
    tGameObjectMain* v225; // [esp+40Ch] [ebp-26Ch]
    tAI* v226; // [esp+410h] [ebp-268h]
    tAI* v227; // [esp+414h] [ebp-264h]
    tGameObjectMain* v228; // [esp+418h] [ebp-260h]
    tGameObjectMain* v229; // [esp+41Ch] [ebp-25Ch]
    tAI* v230; // [esp+420h] [ebp-258h]
    tAI* v231; // [esp+424h] [ebp-254h]
    tGameObjectMain* v232; // [esp+428h] [ebp-250h]
    tGameObjectMain* v233; // [esp+42Ch] [ebp-24Ch]
    tGameObjectMain* v234; // [esp+434h] [ebp-244h]
    tGameObjectMain* v235; // [esp+438h] [ebp-240h]
    tGameObjectMain* v236; // [esp+43Ch] [ebp-23Ch]
    double v237; // [esp+440h] [ebp-238h]
    double v238; // [esp+448h] [ebp-230h]
    double v239; // [esp+450h] [ebp-228h]
    tGameObjectMain* v240; // [esp+468h] [ebp-210h]
    tGameObjectMain* v241; // [esp+46Ch] [ebp-20Ch]
    tGameObjectMain* v242; // [esp+474h] [ebp-204h]
    tGameObjectMain* v243; // [esp+47Ch] [ebp-1FCh]
    tGameObjectMain* v244; // [esp+484h] [ebp-1F4h]
    tGameObjectMain* v245; // [esp+488h] [ebp-1F0h]
    tGameObjectMain* v246; // [esp+48Ch] [ebp-1ECh]
    tGameObjectMain* a2; // [esp+490h] [ebp-1E8h]
    tGameObjectMain* v248; // [esp+494h] [ebp-1E4h]
    tGameObjectMain* v249; // [esp+498h] [ebp-1E0h]
    tGameObjectMain* v250; // [esp+49Ch] [ebp-1DCh]
    double v251; // [esp+4A0h] [ebp-1D8h]
    tGameObjectMain* v252; // [esp+4A8h] [ebp-1D0h]
    tGameObjectMain* v253; // [esp+4ACh] [ebp-1CCh]
    tGameObjectMain* v254; // [esp+4B0h] [ebp-1C8h]
    tGameObjectMain* v255; // [esp+4B4h] [ebp-1C4h]
    tGameObjectMain* v256; // [esp+4B8h] [ebp-1C0h]
    tGameObjectMain* v257; // [esp+4BCh] [ebp-1BCh]
    double v258; // [esp+4C0h] [ebp-1B8h]
    tGameObjectMain* v259; // [esp+4D0h] [ebp-1A8h]
    double v260; // [esp+4D8h] [ebp-1A0h]
    int i; // [esp+4E4h] [ebp-194h]
    float v262; // [esp+4ECh] [ebp-18Ch]
    float v263; // [esp+4ECh] [ebp-18Ch]
    int v264; // [esp+504h] [ebp-174h]
    int v265; // [esp+50Ch] [ebp-16Ch]
    int v266; // [esp+50Ch] [ebp-16Ch]
    Vec3 v267; // [esp+510h] [ebp-168h] BYREF
    int v268; // [esp+528h] [ebp-150h]
    int v269; // [esp+52Ch] [ebp-14Ch]
    int v270; // [esp+530h] [ebp-148h]
    int v271; // [esp+534h] [ebp-144h]
    int v272; // [esp+538h] [ebp-140h]
    Vec3 a3; // [esp+53Ch] [ebp-13Ch] BYREF
    int v274; // [esp+548h] [ebp-130h]
    float v275; // [esp+550h] [ebp-128h]
    Vec3 v276; // [esp+558h] [ebp-120h]
    int v277; // [esp+564h] [ebp-114h]
    float v278; // [esp+568h] [ebp-110h]
    float v279; // [esp+56Ch] [ebp-10Ch]
    unsigned int v280; // [esp+570h] [ebp-108h]
    float v281; // [esp+574h] [ebp-104h]
    int v282; // [esp+578h] [ebp-100h]
    Vec3 v283; // [esp+57Ch] [ebp-FCh] BYREF
    Vec3 v285; // [esp+590h] [ebp-E8h] BYREF
    Vec3 v286; // [esp+5B0h] [ebp-C8h]
    Vec3 pos; // [esp+5BCh] [ebp-BCh] BYREF
    Vec3 v288; // [esp+5C8h] [ebp-B0h] BYREF
    unsigned int v289; // [esp+5D8h] [ebp-A0h]
    tGameObjectMain* v290; // [esp+5ECh] [ebp-8Ch]
    int v291; // [esp+5F0h] [ebp-88h]
    int v292; // [esp+5F4h] [ebp-84h]
    int v293; // [esp+5F8h] [ebp-80h]
    int v294; // [esp+5FCh] [ebp-7Ch]
    int v295; // [esp+600h] [ebp-78h]
    int k; // [esp+604h] [ebp-74h]
    int l; // [esp+608h] [ebp-70h]
    void* v299; // [esp+610h] [ebp-68h]
    int v300; // [esp+614h] [ebp-64h]
    int v301; // [esp+618h] [ebp-60h]
    int j; // [esp+61Ch] [ebp-5Ch]
    void* Src; // [esp+620h] [ebp-58h]
    Vec3 v304; // [esp+624h] [ebp-54h] BYREF
    tScriptStruct_GSTGlobal* v305; // [esp+630h] [ebp-48h]
    int v306; // [esp+634h] [ebp-44h]
    Vec3 v307; // [esp+638h] [ebp-40h] BYREF
    Vec3 a1; // [esp+644h] [ebp-34h] BYREF
    int v309; // [esp+65Ch] [ebp-1Ch]
    int v310; // [esp+660h] [ebp-18h]
    Vec3 v311; // [esp+664h] [ebp-14h] BYREF
    int v312; // [esp+674h] [ebp-4h]

    v286.x = 1.0;
    v286.y = 0.0;
    v286.z = 0.0;
    v276.x = 1.0;
    v276.y = 0.0;
    v276.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v44 = (tScriptStruct_GSTGlobal*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v44 = 0;
    v305 = v44;
    k = 0;
    v278 = 0.0;
    v269 = 0;
    v289 = 0;
    j = 0;
    v279 = 0.0;
    l = 0;
    v270 = 0;
    v274 = 0;
    v290 = 0;
    v271 = 0;
    v280 = 0;
    v291 = 0;
    v275 = 0.0;
    v281 = 0.0;
    v306 = 0;
    v300 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Global_glob_reflex_INV_Manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(&Global_glob_reflex_INV_Manager_gao_i_SelObject, (void*)Global_glob_reflex_INV_Manager_gao, 116);
        AI2C_ResolveBuffer(
            &Global_glob_reflex_INV_Manager_gao_i_OpeningMode,
            (void*)Global_glob_reflex_INV_Manager_gao,
            100);
        AI2C_ResolveBuffer(&Global_glob_reflex_INV_Manager_gao_i_CineMode, (void*)Global_glob_reflex_INV_Manager_gao, 104);
        Global_glob_reflex_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&Global_glob_reflex_univVars_objet_valeur1, (void*)Global_glob_reflex_univVars, 4100);
        AI2C_ResolveBuffer(
            &Global_glob_reflex_INV_Manager_gao_f_ObjInfo_Wait,
            (void*)Global_glob_reflex_INV_Manager_gao,
            21900);
        Global_glob_reflex_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &Global_glob_reflex_INV_Manager_gao_i_OpeningParamInt,
            (void*)Global_glob_reflex_INV_Manager_gao,
            212);
        AI2C_ResolveBuffer(
            &Global_glob_reflex_INV_Manager_gao_i_OpeningParamFloat,
            (void*)Global_glob_reflex_INV_Manager_gao,
            216);
        Global_glob_reflex_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Global_glob_reflex_GST_Global_gao_i_fade_priority,
            (void*)Global_glob_reflex_GST_Global_gao,
            3268);
        AI2C_ResolveBuffer(
            &Global_glob_reflex_GST_Global_gao_f_fade_start_time,
            (void*)Global_glob_reflex_GST_Global_gao,
            3260);
        AI2C_ResolveBuffer(&Global_glob_reflex_GST_Global_gao_f_fade_delay, (void*)Global_glob_reflex_GST_Global_gao, 3264);
        Global_glob_reflex_INV_MatFade_gao = AI_ul_RealResolveOneRefObject(0x5E006934u);
        AI2C_ResolveBuffer(&Global_glob_reflex_univVars_i_special_init, (void*)Global_glob_reflex_univVars, 6780);
        AI2C_ResolveBuffer(&Global_glob_reflex_univVars_load_world, (void*)Global_glob_reflex_univVars, 0);
        AI2C_ResolveBuffer(&Global_glob_reflex_univVars_world_ID, (void*)Global_glob_reflex_univVars, 4);
        AI2C_ResolveBuffer(&Global_glob_reflex_univVars_world_entrance_ID, (void*)Global_glob_reflex_univVars, 8);
        AI2C_ResolveBuffer(&Global_glob_reflex_univVars_Perso_ID_inventaire_max, (void*)Global_glob_reflex_univVars, 432);
        AI2C_ResolveBuffer(&Global_glob_reflex_univVars_Perso_ID_actor, (void*)Global_glob_reflex_univVars, 988);
        Global_glob_reflex__PJ_Camera_gao = AI_ul_RealResolveOneRefObject(0x71003F56u);
        Global_glob_reflex_SFX_Flash_texture_gao = AI_ul_RealResolveOneRefObject(0x3500941Au);
        AI2C_ResolveBuffer(&Global_glob_reflex_univVars_i_fade_fx, (void*)Global_glob_reflex_univVars, 20400);
        AI2C_ResolveBuffer(
            &Global_glob_reflex_GST_Board_panel_gao_Gen_Show,
            (void*)Global_glob_reflex_GST_Board_panel_gao,
            2260);
        Global_glob_reflex__PJ_Camera_Speciale_gao = AI_ul_RealResolveOneRefObject(0x9E0007E2);
        Global_glob_reflex_SFX_AfterFX_gao = AI_ul_RealResolveOneRefObject(0x3500A3A3u);
        AI2C_ResolveBuffer(&Global_glob_reflex_univVars_mdisk_possessionflags, (void*)Global_glob_reflex_univVars, 6756);
        AI2C_ResolveBuffer(&Global_glob_reflex_univVars_Perso_ID_equipement, (void*)Global_glob_reflex_univVars, 40);
        AI2C_ResolveBuffer(&Global_glob_reflex_univVars_objet_status, (void*)Global_glob_reflex_univVars, 3896);
        AI2C_ResolveBuffer(&Global_glob_reflex_univVars_objet_inventaire, (void*)Global_glob_reflex_univVars, 1480);
        AI2C_ResolveBuffer(&Global_glob_reflex_univVars_Flag_speciaux_sauve, (void*)Global_glob_reflex_univVars, 4488);
        AI2C_ResolveBuffer(
            &Global_glob_reflex_GST_Global_gao_i_Perso_ID_a_checker,
            (void*)Global_glob_reflex_GST_Global_gao,
            3288);
        AI2C_ResolveBuffer(
            &Global_glob_reflex_univVars_Perso_ID_Energie_courante,
            (void*)Global_glob_reflex_univVars,
            1148);
        AI2C_ResolveBuffer(&Global_glob_reflex_univVars_Perso_ID_Energie_max, (void*)Global_glob_reflex_univVars, 1096);
        AI2C_ResolveBuffer(
            &Global_glob_reflex_INV_Manager_gao_i_SaveDisable,
            (void*)Global_glob_reflex_INV_Manager_gao,
            108);
        AI2C_ResolveBuffer(&Global_glob_reflex_univVars_Perso_ID_status, (void*)Global_glob_reflex_univVars, 484);
        AI2C_ResolveBuffer(&Global_glob_reflex_univVars_Perso_ID_world_ID, (void*)Global_glob_reflex_univVars, 536);
        AI2C_ResolveBuffer(&Global_glob_reflex_univVars_Perso_ID_pos, (void*)Global_glob_reflex_univVars, 588);
        AI2C_ResolveBuffer(&Global_glob_reflex_univVars_Perso_ID_sight, (void*)Global_glob_reflex_univVars, 736);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)12 || *AI_gppst_CurrentJumpNode == (tAINode*)13)
    {
        *(uint32_t*)Global_glob_reflex_univVars_i_special_init = 0;
        *(uint32_t*)Global_glob_reflex_univVars_load_world = 1;
        *(uint32_t*)Global_glob_reflex_univVars_world_ID = v305->m_msg.Int1;
        v258 = v305->m_msg.f2 + MATH_gd_magicDoubleForRound;
        *(uint32_t*)Global_glob_reflex_univVars_world_entrance_ID = (*(uint32_t *)&v258);
        goto LABEL_737;
    }
    v0 = AI_EvalFunc_MSGGetCount_C(AI_gpst_CurrentGameObject);
    v300 = v0;
    if (!v0)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    Src = AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v305->m_msg, Src, sizeof(v305->m_msg));
    while (1)
    {
        switch (v305->m_msg.msgID)
        {
        case 0:
            return 1000;
        case 45:
            v305->o_joy_acteur = (int)v305->m_msg.gao;
            goto LABEL_737;
        case 62:
            *(uint32_t*)Global_glob_reflex_INV_Manager_gao_i_SelObject = v305->m_msg.Int1;
            *(uint32_t*)Global_glob_reflex_INV_Manager_gao_i_OpeningMode = 2;
            *(uint32_t*)Global_glob_reflex_INV_Manager_gao_i_CineMode = v305->m_msg.Int2;
            if (*(uint32_t*)Global_glob_reflex_INV_Manager_gao_i_SelObject == 45 && v305->m_msg.f2 >= 1.0)
            {
                v260 = v305->m_msg.f2 + MATH_gd_magicDoubleForRound;
                *(uint32_t*)(Global_glob_reflex_univVars_objet_valeur1 + 184) = (*(uint32_t*)&v260);
                *(float*)Global_glob_reflex_INV_Manager_gao_f_ObjInfo_Wait = v305->m_msg.f1;
            }
            AI_EvalFunc_OBJ_CapaSet_C((tGameObjectMain*)Global_glob_reflex_GST_Board_panel_gao, 4096, 0);
            goto LABEL_737;
        case 91:
            *(uint32_t*)Global_glob_reflex_INV_Manager_gao_i_OpeningMode = v305->m_msg.Int1;
            *(uint32_t*)Global_glob_reflex_INV_Manager_gao_i_OpeningParamInt = v305->m_msg.Int2;
            *(float*)Global_glob_reflex_INV_Manager_gao_i_OpeningParamFloat = v305->m_msg.f2;
            AI_EvalFunc_OBJ_CapaSet_C((tGameObjectMain*)Global_glob_reflex_GST_Board_panel_gao, 4096, 0);
            goto LABEL_737;
        }
        if (v305->m_msg.msgID == 87)
            break;
        switch (v305->m_msg.msgID)
        {
        case 84:
            v305->f_Hour_Set = v305->m_msg.f2;
            break;
        case 66:
            *(uint32_t*)(Global_glob_reflex_univVars_Perso_ID_inventaire_max + 4 * v305->m_msg.Int2 + 4) = v305->m_msg.Int1;
            break;
        case 46:
            v305->o_look_acteur = (int)v305->m_msg.gao;
            v305->o_pupille_acteur = (int)v305->m_msg.gao2;
            v257 = (tGameObjectMain*)v305->o_pupille_acteur;
            if (v257)
            {
                if ((unsigned int)v257 <= 2)
                    v257 = AI_MainActor[(uint32_t)v257 - 1];
                v256 = v257;
            }
            else
            {
                v256 = 0;
            }
            if (v256)
                v305->o_pupille_acteur_bis = 0;
            v255 = (tGameObjectMain*)v305->o_pupille_acteur;
            if (v255)
            {
                if ((unsigned int)v255 <= 2)
                    v255 = AI_MainActor[(uint32_t)v255 - 1];
                v254 = v255;
            }
            else
            {
                v254 = 0;
            }
            if (!v254)
                goto LABEL_61;
            v253 = (tGameObjectMain*)v305->o_look_acteur;
            if (v253)
            {
                if ((unsigned int)v253 <= 2)
                    v253 = AI_MainActor[(uint32_t)v253 - 1];
                v252 = v253;
            }
            else
            {
                v252 = 0;
            }
            if (v252)
            {
            LABEL_61:
                if (v305->m_msg.f2 == 0.0)
                    v305->f_vitesse_suiviregard = 1.0;
                else
                    v305->f_vitesse_suiviregard = v305->m_msg.f2;
            }
            break;
        case 88:
            v306 = v305->m_msg.Int1;
            v251 = v305->m_msg.f2 + MATH_gd_magicDoubleForRound;
            for (i = 0; i < 12; ++i)
            {
                if (((1 << i) & v306) != 0)
                {
                    v289 = *(uint32_t*)(Global_glob_reflex_univVars_Perso_ID_actor + 4 * i + 4);
                    v250 = (tGameObjectMain*)v289;
                    if (v289)
                    {
                        if (v289 <= 2)
                            v250 = AI_MainActor[v289 - 1];
                        v249 = v250;
                    }
                    else
                    {
                        v249 = 0;
                    }
                    if (v249)
                    {
                        if ((*(uint32_t*)&v251))
                        {
                            v248 = (tGameObjectMain*)v289;
                            if (v289)
                            {
                                if (v289 <= 2)
                                    v248 = AI_MainActor[v289 - 1];
                                a2 = v248;
                            }
                            else
                            {
                                a2 = 0;
                            }
                            v2 = AI_EvalVarArray_C3((char*)&v305->__v_Perso_ID_actor_stockage_pos_Dimension1, i);
                            AI_EvalFunc_OBJGetPos_C(a2, (Vec3*)v2);
                            v246 = (tGameObjectMain*)v289;
                            if (v289)
                            {
                                if (v289 <= 2)
                                    v246 = AI_MainActor[v289 - 1];
                                v245 = v246;
                            }
                            else
                            {
                                v245 = 0;
                            }
                            v3 = AI_EvalVarArray_C3((char*)&v305->__v_Perso_ID_actor_stockage_sig_Dimension1, i);
                            AI_EvalFunc_OBJ_SightGet_C(v245, (Vec3*)v3);
                        }
                        else
                        {
                            v18 = &v305->m_msg.v1;
                            v4 = AI_EvalVarArray_C3((char*)&v305->__v_Perso_ID_actor_stockage_pos_Dimension1, i);
                            MATH_VecAddVec(&a1, (Vec3*)v4, v18);
                            v244 = (tGameObjectMain*)v289;
                            if (v289)
                            {
                                if (v289 <= 2)
                                    v244 = AI_MainActor[v289 - 1];
                                AI_EvalFunc_OBJSetPos_C(v244, &a1);
                            }
                            else
                            {
                                AI_EvalFunc_OBJSetPos_C(0, &a1);
                            }
                            v243 = (tGameObjectMain*)v289;
                            if (v289)
                            {
                                if (v289 <= 2)
                                    v243 = AI_MainActor[v289 - 1];
                                AI_EvalFunc_OBJGetPos_C(v243, &a3);
                            }
                            else
                            {
                                AI_EvalFunc_OBJGetPos_C(0, &a3);
                            }
                            v242 = (tGameObjectMain*)v289;
                            if (v289)
                            {
                                if (v289 <= 2)
                                    v242 = AI_MainActor[v289 - 1];
                                AI_EvalFunc_COLStartMatrixSet_C(v242, &a3);
                            }
                            else
                            {
                                AI_EvalFunc_COLStartMatrixSet_C(0, &a3);
                            }
                            v288.x = 0.0;
                            v288.y = 0.0;
                            v288.z = 1.0;
                            v241 = (tGameObjectMain*)v289;
                            if (v289)
                            {
                                if (v289 <= 2)
                                    v241 = AI_MainActor[v289 - 1];
                                v240 = v241;
                            }
                            else
                            {
                                v240 = 0;
                            }
                            v5 = AI_EvalVarArray_C3((char*)&v305->__v_Perso_ID_actor_stockage_sig_Dimension1, i);
                            AI_EvalFunc_OBJ_BankingGeneralSet_C(v240, (Vec3*)v5, &v288);
                        }
                    }
                }
            }
            break;
        case 69:
            v305->o_pupille_acteur = (int)v305->m_msg.gao;
            v305->o_pupille_acteur_bis = (int)v305->m_msg.gao2;
            break;
        case 48:
            v276 = v305->m_msg.v1;
            v286 = v305->m_msg.v2;
            v239 = v276.x + MATH_gd_magicDoubleForRound;
            v269 = (*(uint32_t*)&v239);
            v43 = (float)(int)((unsigned __int64)v276.y << 8);
            v238 = v43 + MATH_gd_magicDoubleForRound;
            v270 = (*(uint32_t*)&v238);
            v42 = (float)(int)((unsigned __int64)v276.z << 16);
            v237 = v42 + MATH_gd_magicDoubleForRound;
            v271 = (*(uint32_t*)&v237);
            v278 = v286.x / 255.0f;
            v236 = (tGameObjectMain*)v305->o_objet_flash;
            if (v236)
            {
                if ((unsigned int)v236 <= 2)
                    v236 = AI_MainActor[(uint32_t)v236 - 1];
                v235 = v236;
            }
            else
            {
                v235 = 0;
            }
            if (!v235)
            {
                AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Global_glob_reflex__PJ_Camera_gao, &v267);
                v311.x = 0.0;
                v311.y = 0.0;
                v311.z = 0.0;
                MATH_VecAddVec(&pos, &v267, &v311);
                v6 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)Global_glob_reflex_SFX_Flash_texture_gao, &pos);
                v305->o_objet_flash = (int)v6;
                v234 = (tGameObjectMain*)v305->o_objet_flash;
                if (v234)
                {
                    if ((unsigned int)v234 <= 2)
                        v234 = AI_MainActor[(uint32_t)v234 - 1];
                    AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, v234, &v305->o_objet_flash);
                }
                else
                {
                    AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, 0, &v305->o_objet_flash);
                }
            }
            v231 = (tAI*)v305->o_objet_flash;
            if (v231 == WOR_gst_Universe.univAIInstance)
            {
                v230 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v233 = (tGameObjectMain*)v305->o_objet_flash;
                if (v231)
                {
                    if ((unsigned int)v231 <= 2)
                        v233 = AI_MainActor[(uint32_t)v231 - 1];
                    v232 = v233;
                }
                else
                {
                    v232 = 0;
                }
                v230 = v232->extended->AIInstance;
            }
            ((tScriptStruct_SFXFlashTexture*)(v230->instanceVarDataCurrent))->i_flash_phase1 = 1;
            v227 = (tAI*)v305->o_objet_flash;
            if (v227 == WOR_gst_Universe.univAIInstance)
            {
                v226 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v229 = (tGameObjectMain*)v305->o_objet_flash;
                if (v227)
                {
                    if ((unsigned int)v227 <= 2)
                        v229 = AI_MainActor[(uint32_t)v227 - 1];
                    v228 = v229;
                }
                else
                {
                    v228 = 0;
                }
                v226 = v228->extended->AIInstance;
            }
            ((tScriptStruct_SFXFlashTexture*)(v226->instanceVarDataCurrent))->o_objet_cible = (uint32_t)v305->m_msg.gao;
            v223 = (tAI*)v305->o_objet_flash;
            if (v223 == WOR_gst_Universe.univAIInstance)
            {
                v222 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v225 = (tGameObjectMain*)v305->o_objet_flash;
                if (v223)
                {
                    if ((unsigned int)v223 <= 2)
                        v225 = AI_MainActor[(uint32_t)v223 - 1];
                    v224 = v225;
                }
                else
                {
                    v224 = 0;
                }
                v222 = v224->extended->AIInstance;
            }
            ((tScriptStruct_SFXFlashTexture*)(v222->instanceVarDataCurrent))->des_i_submat = v305->m_msg.Int1;
            v219 = (tAI*)v305->o_objet_flash;
            if (v219 == WOR_gst_Universe.univAIInstance)
            {
                v218 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v221 = (tGameObjectMain*)v305->o_objet_flash;
                if (v219)
                {
                    if ((unsigned int)v219 <= 2)
                        v221 = AI_MainActor[(uint32_t)v219 - 1];
                    v220 = v221;
                }
                else
                {
                    v220 = 0;
                }
                v218 = v220->extended->AIInstance;
            }
            ((tScriptStruct_SFXFlashTexture*)(v218->instanceVarDataCurrent))->des_c_couleur = v271 + v270 + v269;
            if (v278 <= 1.0)
            {
                v211 = (tAI*)v305->o_objet_flash;
                if (v211 == WOR_gst_Universe.univAIInstance)
                {
                    v210 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v213 = (tGameObjectMain*)v305->o_objet_flash;
                    if (v211)
                    {
                        if ((unsigned int)v211 <= 2)
                            v213 = AI_MainActor[(uint32_t)v211 - 1];
                        v212 = v213;
                    }
                    else
                    {
                        v212 = 0;
                    }
                    v210 = v212->extended->AIInstance;
                }
                ((tScriptStruct_SFXFlashTexture*)(v210->instanceVarDataCurrent))->des_f_opacite = v278;
            }
            else
            {
                v215 = (tAI*)v305->o_objet_flash;
                if (v215 == WOR_gst_Universe.univAIInstance)
                {
                    v214 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v217 = (tGameObjectMain*)v305->o_objet_flash;
                    if (v215)
                    {
                        if ((unsigned int)v215 <= 2)
                            v217 = AI_MainActor[(uint32_t)v215 - 1];
                        v216 = v217;
                    }
                    else
                    {
                        v216 = 0;
                    }
                    v214 = v216->extended->AIInstance;
                }
                ((tScriptStruct_SFXFlashTexture*)(v214->instanceVarDataCurrent))->des_f_opacite = v278 / 255.0f;
            }
            v207 = (tAI*)v305->o_objet_flash;
            if (v207 == WOR_gst_Universe.univAIInstance)
            {
                v206 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v209 = (tGameObjectMain*)v305->o_objet_flash;
                if (v207)
                {
                    if ((unsigned int)v207 <= 2)
                        v209 = AI_MainActor[(uint32_t)v207 - 1];
                    v208 = v209;
                }
                else
                {
                    v208 = 0;
                }
                v206 = v208->extended->AIInstance;
            }
            ((tScriptStruct_SFXFlashTexture*)(v206->instanceVarDataCurrent))->des_f_temps_deb = v305->m_msg.f2;
            v203 = (tAI*)v305->o_objet_flash;
            if (v203 == WOR_gst_Universe.univAIInstance)
            {
                v202 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v205 = (tGameObjectMain*)v305->o_objet_flash;
                if (v203)
                {
                    if ((unsigned int)v203 <= 2)
                        v205 = AI_MainActor[(uint32_t)v203 - 1];
                    v204 = v205;
                }
                else
                {
                    v204 = 0;
                }
                v202 = v204->extended->AIInstance;
            }
            ((tScriptStruct_SFXFlashTexture*)(v202->instanceVarDataCurrent))->des_f_temps_fin = v305->m_msg.f1;
            v199 = (tAI*)v305->o_objet_flash;
            if (v199 == WOR_gst_Universe.univAIInstance)
            {
                v198 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v201 = (tGameObjectMain*)v305->o_objet_flash;
                if (v199)
                {
                    if ((unsigned int)v199 <= 2)
                        v201 = AI_MainActor[(uint32_t)v199 - 1];
                    v200 = v201;
                }
                else
                {
                    v200 = 0;
                }
                v198 = v200->extended->AIInstance;
            }
            ((tScriptStruct_SFXFlashTexture*)(v198->instanceVarDataCurrent))->des_i_type_effet = v305->m_msg.Int2;
            break;
        case 50:
            v276 = v305->m_msg.v1;
            v286 = v305->m_msg.v2;
            v275 = v276.x;
            v279 = v276.y;
            v281 = v276.z;
            v278 = v286.x;
            v262 = v286.y;
            if (v305->m_msg.f2 == 0.0)
            {
                if (*(int*)Global_glob_reflex_GST_Global_gao_i_fade_priority > 5000
                    && !AI_EvalFunc_TIMEElapsed_C(
                        *(float*)Global_glob_reflex_GST_Global_gao_f_fade_start_time,
                        *(float*)Global_glob_reflex_GST_Global_gao_f_fade_delay))
                {
                    j = -1;
                    break;
                }
                *(float*)Global_glob_reflex_GST_Global_gao_f_fade_start_time = TIM_gf_MainClock - TIM_gf_LockTime;
                *(float*)Global_glob_reflex_GST_Global_gao_f_fade_delay = v275 + v279 + v281 + v278 + v262;
                *(uint32_t*)Global_glob_reflex_GST_Global_gao_i_fade_priority = 5000;
                AI_EvalFunc_GFXDel_C_CURRENT(10);
                j = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
                *(uint32_t*)Global_glob_reflex_univVars_i_fade_fx = j;
                AI_EvalFunc_GFXFlagSet_C_CURRENT(j, 0, 1);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(j, 2, 1);
                v197 = (tGameObjectMain*)Global_glob_reflex_INV_MatFade_gao;
                if (Global_glob_reflex_INV_MatFade_gao)
                {
                    if ((unsigned int)Global_glob_reflex_INV_MatFade_gao <= 2)
                        v197 = AI_MainActor[Global_glob_reflex_INV_MatFade_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(j, v197, 9);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(j, 0, 9);
                }
            }
            else
            {
                if (*(int*)Global_glob_reflex_GST_Global_gao_i_fade_priority > 5000
                    && !AI_EvalFunc_TIMEElapsed_C(
                        *(float*)Global_glob_reflex_GST_Global_gao_f_fade_start_time,
                        *(float*)Global_glob_reflex_GST_Global_gao_f_fade_delay))
                {
                    j = -1;
                    break;
                }
                *(float*)Global_glob_reflex_GST_Global_gao_f_fade_start_time = TIM_gf_MainClock - TIM_gf_LockTime;
                *(float*)Global_glob_reflex_GST_Global_gao_f_fade_delay = v275 + v279 + v281 + v278 + v262;
                *(uint32_t*)Global_glob_reflex_GST_Global_gao_i_fade_priority = 5000;
                AI_EvalFunc_GFXDel_C_CURRENT(10);
                j = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(j, 0, 1);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(j, 2, 1);
                v196 = (tGameObjectMain*)Global_glob_reflex_INV_MatFade_gao;
                if (Global_glob_reflex_INV_MatFade_gao)
                {
                    if ((unsigned int)Global_glob_reflex_INV_MatFade_gao <= 2)
                        v196 = AI_MainActor[Global_glob_reflex_INV_MatFade_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(j, v196, 8);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(j, 0, 8);
                }
            }
            GFXSetCurGlobal(j);
            AI_EvalFunc_GFXSeti_C_CURGLOB(11100, v305->m_msg.Int1);
            AI_EvalFunc_GFXSeti_C_CURGLOB(11101, v305->m_msg.Int2);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11000, v275);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11001, v279);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11002, v281);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11003, v278);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11004, v262);
            break;
        case 61:
            *(uint32_t*)Global_glob_reflex_GST_Board_panel_gao_Gen_Show = v305->m_msg.f1 != 0.0;
            if (v305->m_msg.Int1)
            {
                AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)Global_glob_reflex_GST_Board_panel_gao, 0, 512);
            }
            else
            {
                AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)Global_glob_reflex_GST_Board_panel_gao, 512, 0);
                AI_EvalFunc_OBJ_CapaSet_C((tGameObjectMain*)Global_glob_reflex__PJ_Camera_Speciale_gao, 0, 256);
            }
            if (v305->m_msg.Int2)
                AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)Global_glob_reflex_GST_Board_panel_gao, 0, 256);
            else
                AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)Global_glob_reflex_GST_Board_panel_gao, 256, 0);
            break;
        case 60:
            v276 = v305->m_msg.v1;
            v286 = v305->m_msg.v2;
            v275 = v276.x;
            v279 = v276.y;
            v281 = v276.z;
            v278 = v286.x;
            v263 = v286.y;
            if (v305->m_msg.Int1 == -1)
            {
                v290 = v305->m_msg.gao;
                v195 = v290;
                if (v290)
                {
                    if ((unsigned int)v290 <= 2)
                        v195 = AI_MainActor[(uint32_t)v290 - 1];
                    v194 = v195;
                }
                else
                {
                    v194 = 0;
                }
                if (v194)
                {
                    if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                    {
                        v191 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v193 = v290;
                        if (v290)
                        {
                            if ((unsigned int)v290 <= 2)
                                v193 = AI_MainActor[(uint32_t)v290 - 1];
                            v192 = v193;
                        }
                        else
                        {
                            v192 = 0;
                        }
                        v191 = v192->extended->AIInstance;
                    }
                    if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                    {
                        v188 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v190 = v290;
                        if (v290)
                        {
                            if ((unsigned int)v290 <= 2)
                                v190 = AI_MainActor[(uint32_t)v290 - 1];
                            v189 = v190;
                        }
                        else
                        {
                            v189 = 0;
                        }
                        v188 = v189->extended->AIInstance;
                    }
                    AI_EvalFunc_Gsp_AE_MASTER_C(
                        8,
                        1,
                        (unsigned __int64)*((float*)v188->instanceVarDataCurrent + 174),
                        *((float*)v191->instanceVarDataCurrent + 174));
                    if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                    {
                        v185 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v187 = v290;
                        if (v290)
                        {
                            if ((unsigned int)v290 <= 2)
                                v187 = AI_MainActor[(uint32_t)v290 - 1];
                            v186 = v187;
                        }
                        else
                        {
                            v186 = 0;
                        }
                        v185 = v186->extended->AIInstance;
                    }
                    if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                    {
                        v182 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v184 = v290;
                        if (v290)
                        {
                            if ((unsigned int)v290 <= 2)
                                v184 = AI_MainActor[(uint32_t)v290 - 1];
                            v183 = v184;
                        }
                        else
                        {
                            v183 = 0;
                        }
                        v182 = v183->extended->AIInstance;
                    }
                    AI_EvalFunc_Gsp_AE_MASTER_C(
                        9,
                        1,
                        (unsigned __int64)*((float*)v182->instanceVarDataCurrent + 175),// todo: debug, ez mi????
                        *((float*)v185->instanceVarDataCurrent + 175));
                    if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                    {
                        v179 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v181 = v290;
                        if (v290)
                        {
                            if ((unsigned int)v290 <= 2)
                                v181 = AI_MainActor[(uint32_t)v290 - 1];
                            v180 = v181;
                        }
                        else
                        {
                            v180 = 0;
                        }
                        v179 = v180->extended->AIInstance;
                    }
                    if (*((uint32_t*)v179->instanceVarDataCurrent + 176))
                    {
                        if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                        {
                            v176 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v178 = v290;
                            if (v290)
                            {
                                if ((unsigned int)v290 <= 2)
                                    v178 = AI_MainActor[(uint32_t)v290 - 1];
                                v177 = v178;
                            }
                            else
                            {
                                v177 = 0;
                            }
                            v176 = v177->extended->AIInstance;
                        }
                        v41 = (float)*((int*)v176->instanceVarDataCurrent + 177);
                        AI_EvalFunc_Gsp_AE_MASTER_C(0, 1, 0, v41);
                        if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                        {
                            v173 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v175 = v290;
                            if (v290)
                            {
                                if ((unsigned int)v290 <= 2)
                                    v175 = AI_MainActor[(uint32_t)v290 - 1];
                                v174 = v175;
                            }
                            else
                            {
                                v174 = 0;
                            }
                            v173 = v174->extended->AIInstance;
                        }
                        v40 = (float)*((int*)v173->instanceVarDataCurrent + 178);
                        AI_EvalFunc_Gsp_AE_MASTER_C(0, 1, 1, v40);
                        if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                        {
                            v170 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v172 = v290;
                            if (v290)
                            {
                                if ((unsigned int)v290 <= 2)
                                    v172 = AI_MainActor[(uint32_t)v290 - 1];
                                v171 = v172;
                            }
                            else
                            {
                                v171 = 0;
                            }
                            v170 = v171->extended->AIInstance;
                        }
                        AI_EvalFunc_Gsp_AE_MASTER_C(0, 1, 2, *((float*)v170->instanceVarDataCurrent + 179));
                    }
                    else
                    {
                        AI_EvalFunc_Gsp_AE_MASTER_C(0, 0, 0, 0.0);
                        AI_EvalFunc_Gsp_AE_MASTER_C(0, 0, 1, 0.0);
                        AI_EvalFunc_Gsp_AE_MASTER_C(0, 0, 2, 0.0);
                    }
                    if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                    {
                        v167 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v169 = v290;
                        if (v290)
                        {
                            if ((unsigned int)v290 <= 2)
                                v169 = AI_MainActor[(uint32_t)v290 - 1];
                            v168 = v169;
                        }
                        else
                        {
                            v168 = 0;
                        }
                        v167 = v168->extended->AIInstance;
                    }
                    if (*((uint32_t*)v167->instanceVarDataCurrent + 183))
                    {
                        if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                        {
                            v164 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v166 = v290;
                            if (v290)
                            {
                                if ((unsigned int)v290 <= 2)
                                    v166 = AI_MainActor[(uint32_t)v290 - 1];
                                v165 = v166;
                            }
                            else
                            {
                                v165 = 0;
                            }
                            v164 = v165->extended->AIInstance;
                        }
                        if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                        {
                            v161 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v163 = v290;
                            if (v290)
                            {
                                if ((unsigned int)v290 <= 2)
                                    v163 = AI_MainActor[(uint32_t)v290 - 1];
                                v162 = v163;
                            }
                            else
                            {
                                v162 = 0;
                            }
                            v161 = v162->extended->AIInstance;
                        }
                        AI_EvalFunc_Gsp_AE_MASTER_C(
                            7,
                            1,
                            (unsigned __int64)*((float*)v161->instanceVarDataCurrent + 184),
                            *((float*)v164->instanceVarDataCurrent + 184));
                    }
                    else
                    {
                        if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                        {
                            v158 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v160 = v290;
                            if (v290)
                            {
                                if ((unsigned int)v290 <= 2)
                                    v160 = AI_MainActor[(uint32_t)v290 - 1];
                                v159 = v160;
                            }
                            else
                            {
                                v159 = 0;
                            }
                            v158 = v159->extended->AIInstance;
                        }
                        AI_EvalFunc_Gsp_AE_MASTER_C(7, 0, (unsigned __int64)*((float*)v158->instanceVarDataCurrent + 184), 0.0);
                    }
                    if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                    {
                        v155 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v157 = v290;
                        if (v290)
                        {
                            if ((unsigned int)v290 <= 2)
                                v157 = AI_MainActor[(uint32_t)v290 - 1];
                            v156 = v157;
                        }
                        else
                        {
                            v156 = 0;
                        }
                        v155 = v156->extended->AIInstance;
                    }
                    if (*((uint32_t*)v155->instanceVarDataCurrent + 180))
                    {
                        if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                        {
                            v152 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v154 = v290;
                            if (v290)
                            {
                                if ((unsigned int)v290 <= 2)
                                    v154 = AI_MainActor[(uint32_t)v290 - 1];
                                v153 = v154;
                            }
                            else
                            {
                                v153 = 0;
                            }
                            v152 = v153->extended->AIInstance;
                        }
                        AI_EvalFunc_Gsp_AE_MASTER_C(12, 1, 0, *((float*)v152->instanceVarDataCurrent + 181));
                        if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                        {
                            v149 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v151 = v290;
                            if (v290)
                            {
                                if ((unsigned int)v290 <= 2)
                                    v151 = AI_MainActor[(uint32_t)v290 - 1];
                                v150 = v151;
                            }
                            else
                            {
                                v150 = 0;
                            }
                            v149 = v150->extended->AIInstance;
                        }
                        AI_EvalFunc_Gsp_AE_MASTER_C(12, 1, 1, *((float*)v149->instanceVarDataCurrent + 182));
                    }
                    else
                    {
                        AI_EvalFunc_Gsp_AE_MASTER_C(12, 0, 0, 0.0);
                        AI_EvalFunc_Gsp_AE_MASTER_C(12, 0, 1, 0.0);
                    }
                    if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                    {
                        v146 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v148 = v290;
                        if (v290)
                        {
                            if ((unsigned int)v290 <= 2)
                                v148 = AI_MainActor[(uint32_t)v290 - 1];
                            v147 = v148;
                        }
                        else
                        {
                            v147 = 0;
                        }
                        v146 = v147->extended->AIInstance;
                    }
                    if (*((uint32_t*)v146->instanceVarDataCurrent + 189))
                    {
                        if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                        {
                            v143 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v145 = v290;
                            if (v290)
                            {
                                if ((unsigned int)v290 <= 2)
                                    v145 = AI_MainActor[(uint32_t)v290 - 1];
                                v144 = v145;
                            }
                            else
                            {
                                v144 = 0;
                            }
                            v143 = v144->extended->AIInstance;
                        }
                        AI_EvalFunc_Gsp_AE_MASTER_C(16, 1, 0, *((float*)v143->instanceVarDataCurrent + 190));
                        if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                        {
                            v140 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v142 = v290;
                            if (v290)
                            {
                                if ((unsigned int)v290 <= 2)
                                    v142 = AI_MainActor[(uint32_t)v290 - 1];
                                v141 = v142;
                            }
                            else
                            {
                                v141 = 0;
                            }
                            v140 = v141->extended->AIInstance;
                        }
                        AI_EvalFunc_Gsp_AE_MASTER_C(16, 1, 1, *((float*)v140->instanceVarDataCurrent + 191));
                    }
                    else
                    {
                        AI_EvalFunc_Gsp_AE_MASTER_C(16, 0, 0, 0.0);
                        AI_EvalFunc_Gsp_AE_MASTER_C(16, 0, 1, 0.0);
                    }
                    if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                    {
                        v137 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v139 = v290;
                        if (v290)
                        {
                            if ((unsigned int)v290 <= 2)
                                v139 = AI_MainActor[(uint32_t)v290 - 1];
                            v138 = v139;
                        }
                        else
                        {
                            v138 = 0;
                        }
                        v137 = v138->extended->AIInstance;
                    }
                    AI_EvalFunc_Gsp_AE_MASTER_C(13, 1, 0, *((float*)v137->instanceVarDataCurrent + 187));
                    if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                    {
                        v134 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v136 = v290;
                        if (v290)
                        {
                            if ((unsigned int)v290 <= 2)
                                v136 = AI_MainActor[(uint32_t)v290 - 1];
                            v135 = v136;
                        }
                        else
                        {
                            v135 = 0;
                        }
                        v134 = v135->extended->AIInstance;
                    }
                    AI_EvalFunc_Gsp_AE_MASTER_C(13, 1, 1, *((float*)v134->instanceVarDataCurrent + 188)); // TODO: ez mi?
                }
                v133 = (tGameObjectMain*)v305->o_objet_FX[8];
                if (v133)
                {
                    if ((unsigned int)v133 <= 2)
                        v133 = AI_MainActor[(uint32_t)v133 - 1];
                    v132 = v133;
                }
                else
                {
                    v132 = 0;
                }
                if (v132)
                {
                    v131 = (tGameObjectMain*)v305->o_objet_FX[8];
                    if (v131)
                    {
                        if ((unsigned int)v131 <= 2)
                            v131 = AI_MainActor[(uint32_t)v131 - 1];
                        v7 = AI_EvalFunc_OBJDestroy_C(v131);
                    }
                    else
                    {
                        v7 = AI_EvalFunc_OBJDestroy_C(0);
                    }
                    v293 = v7;
                    if (v7 != 1000)
                        return v293;
                    v305->o_objet_FX[8] = 0;
                }
                v130 = (tGameObjectMain*)v305->o_objet_FX[9];
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
                {
                    v128 = (tGameObjectMain*)v305->o_objet_FX[9];
                    if (v128)
                    {
                        if ((unsigned int)v128 <= 2)
                            v128 = AI_MainActor[(uint32_t)v128 - 1];
                        v8 = AI_EvalFunc_OBJDestroy_C(v128);
                    }
                    else
                    {
                        v8 = AI_EvalFunc_OBJDestroy_C(0);
                    }
                    v282 = v8;
                    if (v8 != 1000)
                        return v282;
                    v305->o_objet_FX[9] = 0;
                }
                v127 = v290;
                if (v290)
                {
                    if ((unsigned int)v290 <= 2)
                        v127 = AI_MainActor[(uint32_t)v290 - 1];
                    v126 = v127;
                }
                else
                {
                    v126 = 0;
                }
                if (v126)
                {
                    if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                    {
                        v123 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v125 = v290;
                        if (v290)
                        {
                            if ((unsigned int)v290 <= 2)
                                v125 = AI_MainActor[(uint32_t)v290 - 1];
                            v124 = v125;
                        }
                        else
                        {
                            v124 = 0;
                        }
                        v123 = v124->extended->AIInstance;
                    }
                    if (*((uint32_t*)v123->instanceVarDataCurrent + 176))
                    {
                        v122 = (tGameObjectMain*)v305->o_objet_FX[0];
                        if (v122)
                        {
                            if ((unsigned int)v122 <= 2)
                                v122 = AI_MainActor[(uint32_t)v122 - 1];
                            v121 = v122;
                        }
                        else
                        {
                            v121 = 0;
                        }
                        if (v121)
                        {
                            v120 = (tGameObjectMain*)v305->o_objet_FX[0];
                            if (v120)
                            {
                                if ((unsigned int)v120 <= 2)
                                    v120 = AI_MainActor[(uint32_t)v120 - 1];
                                v9 = AI_EvalFunc_OBJDestroy_C(v120);
                            }
                            else
                            {
                                v9 = AI_EvalFunc_OBJDestroy_C(0);
                            }
                            v277 = v9;
                            if (v9 != 1000)
                                return v277;
                            v305->o_objet_FX[0] = 0;
                        }
                    }
                    if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                    {
                        v117 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v119 = v290;
                        if (v290)
                        {
                            if ((unsigned int)v290 <= 2)
                                v119 = AI_MainActor[(uint32_t)v290 - 1];
                            v118 = v119;
                        }
                        else
                        {
                            v118 = 0;
                        }
                        v117 = v118->extended->AIInstance;
                    }
                    if (*((uint32_t*)v117->instanceVarDataCurrent + 183))
                    {
                        v116 = (tGameObjectMain*)v305->o_objet_FX[7];
                        if (v116)
                        {
                            if ((unsigned int)v116 <= 2)
                                v116 = AI_MainActor[(uint32_t)v116 - 1];
                            v115 = v116;
                        }
                        else
                        {
                            v115 = 0;
                        }
                        if (v115)
                        {
                            v114 = (tGameObjectMain*)v305->o_objet_FX[7];
                            if (v114)
                            {
                                if ((unsigned int)v114 <= 2)
                                    v114 = AI_MainActor[(uint32_t)v114 - 1];
                                v10 = AI_EvalFunc_OBJDestroy_C(v114);
                            }
                            else
                            {
                                v10 = AI_EvalFunc_OBJDestroy_C(0);
                            }
                            v295 = v10;
                            if (v10 != 1000)
                                return v295;
                            v305->o_objet_FX[7] = 0;
                        }
                    }
                    if (v290 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                    {
                        v111 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v113 = v290;
                        if (v290)
                        {
                            if ((unsigned int)v290 <= 2)
                                v113 = AI_MainActor[(uint32_t)v290 - 1];
                            v112 = v113;
                        }
                        else
                        {
                            v112 = 0;
                        }
                        v111 = v112->extended->AIInstance;
                    }
                    if (*((uint32_t*)v111->instanceVarDataCurrent + 180))
                    {
                        v110 = (tGameObjectMain*)v305->o_objet_FX[11];
                        if (v110)
                        {
                            if ((unsigned int)v110 <= 2)
                                v110 = AI_MainActor[(uint32_t)v110 - 1];
                            v109 = v110;
                        }
                        else
                        {
                            v109 = 0;
                        }
                        if (v109)
                        {
                            v108 = (tGameObjectMain*)v305->o_objet_FX[11];
                            if (v108)
                            {
                                if ((unsigned int)v108 <= 2)
                                    v108 = AI_MainActor[(uint32_t)v108 - 1];
                                result = AI_EvalFunc_OBJDestroy_C(v108);
                            }
                            else
                            {
                                result = AI_EvalFunc_OBJDestroy_C(0);
                            }
                            if (result != 1000)
                                return result;
                            v305->o_objet_FX[11] = 0;
                        }
                    }
                }
                for (j = 0; j < 15; ++j)
                {
                    v107 = (tGameObjectMain*)v305->o_objet_FX[j];
                    if (v107)
                    {
                        if ((unsigned int)v107 <= 2)
                            v107 = AI_MainActor[(uint32_t)v107 - 1];
                        v106 = v107;
                    }
                    else
                    {
                        v106 = 0;
                    }
                    if (v106)
                    {
                        v105 = (tGameObjectMain*)v305->o_objet_FX[j];
                        if (v105)
                        {
                            if ((unsigned int)v105 <= 2)
                                v105 = AI_MainActor[(uint32_t)v105 - 1];
                            AI_EvalFunc_OBJ_FlagInactiveSet_C(v105, 0);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
                        }
                        v104 = (tGameObjectMain*)v305->o_objet_FX[j];
                        if (v104)
                        {
                            if ((unsigned int)v104 <= 2)
                                v104 = AI_MainActor[(uint32_t)v104 - 1];
                            AI_EvalFunc_OBJ_CapaSet_C(v104, 0, 256);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_CapaSet_C(0, 0, 256);
                        }
                    }
                }
            }
            else
            {
                v103 = (tGameObjectMain*)v305->o_objet_FX[v305->m_msg.Int1];
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
                if (!v102)
                {
                    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Global_glob_reflex__PJ_Camera_gao, &v307);
                    v285.x = 0.0;
                    v285.y = 0.0;
                    v285.z = 0.0;
                    MATH_VecAddVec(&v304, &v307, &v285);
                    v101 = &v305->o_objet_FX[v305->m_msg.Int1];
                    *v101 = (int)AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)Global_glob_reflex_SFX_AfterFX_gao, &v304);
                    v100 = (tGameObjectMain*)v305->o_objet_FX[v305->m_msg.Int1];
                    if (v100)
                    {
                        if ((unsigned int)v100 <= 2)
                            v100 = AI_MainActor[(uint32_t)v100 - 1];
                        AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, v100, &v305->o_objet_FX[v305->m_msg.Int1]);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, 0, &v305->o_objet_FX[v305->m_msg.Int1]);
                    }
                }
                v97 = (tAI*)v305->o_objet_FX[v305->m_msg.Int1];
                if (v97 == WOR_gst_Universe.univAIInstance)
                {
                    v96 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v99 = (tGameObjectMain*)v305->o_objet_FX[v305->m_msg.Int1];
                    if (v97)
                    {
                        if ((unsigned int)v97 <= 2)
                            v99 = AI_MainActor[(uint32_t)v97 - 1];
                        v98 = v99;
                    }
                    else
                    {
                        v98 = 0;
                    }
                    v96 = v98->extended->AIInstance;
                }
                *(uint32_t*)v96->instanceVarDataCurrent = v305->m_msg.Int1;
                v93 = (tAI*)v305->o_objet_FX[v305->m_msg.Int1];
                if (v93 == WOR_gst_Universe.univAIInstance)
                {
                    v92 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v95 = (tGameObjectMain*)v305->o_objet_FX[v305->m_msg.Int1];
                    if (v93)
                    {
                        if ((unsigned int)v93 <= 2)
                            v95 = AI_MainActor[(uint32_t)v93 - 1];
                        v94 = v95;
                    }
                    else
                    {
                        v94 = 0;
                    }
                    v92 = v94->extended->AIInstance;
                }
                *((float*)v92->instanceVarDataCurrent + 1) = v275;
                v89 = (tAI*)v305->o_objet_FX[v305->m_msg.Int1];
                if (v89 == WOR_gst_Universe.univAIInstance)
                {
                    v88 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v91 = (tGameObjectMain*)v305->o_objet_FX[v305->m_msg.Int1];
                    if (v89)
                    {
                        if ((unsigned int)v89 <= 2)
                            v91 = AI_MainActor[(uint32_t)v89 - 1];
                        v90 = v91;
                    }
                    else
                    {
                        v90 = 0;
                    }
                    v88 = v90->extended->AIInstance;
                }
                *((float*)v88->instanceVarDataCurrent + 2) = v279;
                v85 = (tAI*)v305->o_objet_FX[v305->m_msg.Int1];
                if (v85 == WOR_gst_Universe.univAIInstance)
                {
                    v84 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v87 = (tGameObjectMain*)v305->o_objet_FX[v305->m_msg.Int1];
                    if (v85)
                    {
                        if ((unsigned int)v85 <= 2)
                            v87 = AI_MainActor[(uint32_t)v85 - 1];
                        v86 = v87;
                    }
                    else
                    {
                        v86 = 0;
                    }
                    v84 = v86->extended->AIInstance;
                }
                *((float*)v84->instanceVarDataCurrent + 3) = v281;
                v81 = (tAI*)v305->o_objet_FX[v305->m_msg.Int1];
                if (v81 == WOR_gst_Universe.univAIInstance)
                {
                    v80 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v83 = (tGameObjectMain*)v305->o_objet_FX[v305->m_msg.Int1];
                    if (v81)
                    {
                        if ((unsigned int)v81 <= 2)
                            v83 = AI_MainActor[(uint32_t)v81 - 1];
                        v82 = v83;
                    }
                    else
                    {
                        v82 = 0;
                    }
                    v80 = v82->extended->AIInstance;
                }
                *((float*)v80->instanceVarDataCurrent + 4) = v278;
                v77 = (tAI*)v305->o_objet_FX[v305->m_msg.Int1];
                if (v77 == WOR_gst_Universe.univAIInstance)
                {
                    v76 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v79 = (tGameObjectMain*)v305->o_objet_FX[v305->m_msg.Int1];
                    if (v77)
                    {
                        if ((unsigned int)v77 <= 2)
                            v79 = AI_MainActor[(uint32_t)v77 - 1];
                        v78 = v79;
                    }
                    else
                    {
                        v78 = 0;
                    }
                    v76 = v78->extended->AIInstance;
                }
                *((float*)v76->instanceVarDataCurrent + 5) = v263;
                v73 = (tAI*)v305->o_objet_FX[v305->m_msg.Int1];
                if (v73 == WOR_gst_Universe.univAIInstance)
                {
                    v72 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v75 = (tGameObjectMain*)v305->o_objet_FX[v305->m_msg.Int1];
                    if (v73)
                    {
                        if ((unsigned int)v73 <= 2)
                            v75 = AI_MainActor[(uint32_t)v73 - 1];
                        v74 = v75;
                    }
                    else
                    {
                        v74 = 0;
                    }
                    v72 = v74->extended->AIInstance;
                }
                *((float*)v72->instanceVarDataCurrent + 7) = v305->m_msg.f2;
                v69 = (tAI*)v305->o_objet_FX[v305->m_msg.Int1];
                if (v69 == WOR_gst_Universe.univAIInstance)
                {
                    v68 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v71 = (tGameObjectMain*)v305->o_objet_FX[v305->m_msg.Int1];
                    if (v69)
                    {
                        if ((unsigned int)v69 <= 2)
                            v71 = AI_MainActor[(uint32_t)v69 - 1];
                        v70 = v71;
                    }
                    else
                    {
                        v70 = 0;
                    }
                    v68 = v70->extended->AIInstance;
                }
                *((float*)v68->instanceVarDataCurrent + 8) = v305->m_msg.f1;
                v67 = (tGameObjectMain*)v305->o_objet_FX[v305->m_msg.Int1];
                if (v67)
                {
                    if ((unsigned int)v67 <= 2)
                        v67 = AI_MainActor[(uint32_t)v67 - 1];
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(v67, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
                }
                v66 = (tGameObjectMain*)v305->o_objet_FX[v305->m_msg.Int1];
                if (v66)
                {
                    if ((unsigned int)v66 <= 2)
                        v66 = AI_MainActor[(uint32_t)v66 - 1];
                    AI_EvalFunc_OBJ_CustomBitsSet_C(v66, 0x4000, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_CustomBitsSet_C(0, 0x4000, 0);
                }
            }
            break;
        case 55:
            if (v305->m_msg.Int1 == 45)
            {
                *(uint32_t*)(Global_glob_reflex_univVars_objet_valeur1 + 184) = v305->m_msg.Int2;
            }
            else if (v305->m_msg.Int1 == 10000)
            {
                for (k = 0; k < 8; ++k)
                    *(uint32_t*)AI_EvalVarArray2_C(
                        1,
                        (char*)Global_glob_reflex_univVars_Perso_ID_equipement,
                        k,
                        v305->m_msg.Int2) = -1;
                for (k = 0; k < 50; ++k)
                    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)Global_glob_reflex_univVars_objet_inventaire, k, v305->m_msg.Int2) = 0;
            }
            else if (v305->m_msg.Int1 <= -1)
            {
                if (v305->m_msg.Int1 < 0)
                    v32 = -v305->m_msg.Int1;
                else
                    v32 = v305->m_msg.Int1;
                v309 = v32;
                if (*(int*)AI_EvalVarArray2_C(1, (char*)Global_glob_reflex_univVars_objet_inventaire, v32, v305->m_msg.Int2) > 0)
                {
                    v31 = v305->m_msg.Int1 < 0 ? -v305->m_msg.Int1 : v305->m_msg.Int1;
                    v272 = v31;
                    if ((*(uint32_t*)(Global_glob_reflex_univVars_objet_status + 4 * v31 + 4) & 0x400000) == 0)
                    {
                        v30 = v305->m_msg.Int1 < 0 ? -v305->m_msg.Int1 : v305->m_msg.Int1;
                        v312 = v30;
                        v29 = AI_EvalVarArray2_C(1, (char*)Global_glob_reflex_univVars_objet_inventaire, v30, v305->m_msg.Int2);
                        --* (uint32_t*)v29;
                        v28 = v305->m_msg.Int1 < 0 ? -v305->m_msg.Int1 : v305->m_msg.Int1;
                        v294 = v28;
                        if (!*(uint32_t*)AI_EvalVarArray2_C(
                            1,
                            (char*)Global_glob_reflex_univVars_objet_inventaire,
                            v28,
                            v305->m_msg.Int2))
                        {
                            v27 = v305->m_msg.Int1 < 0 ? -v305->m_msg.Int1 : v305->m_msg.Int1;
                            if ((*(uint32_t*)(Global_glob_reflex_univVars_objet_status + 4 * v27 + 4) & 0x7000000) == 0)
                                *(uint32_t*)AI_EvalVarArray2_C(
                                    1,
                                    (char*)Global_glob_reflex_univVars_Perso_ID_equipement,
                                    0,
                                    v305->m_msg.Int2) = -1;
                        }
                    }
                }
                if (v305->m_msg.Int1 < 0)
                    v26 = -v305->m_msg.Int1;
                else
                    v26 = v305->m_msg.Int1;
                v268 = v26;
                if (v26 == 28 && v305->m_msg.Int2 < 9
                    || (v305->m_msg.Int1 < 0 ? (v25 = -v305->m_msg.Int1) : (v25 = v305->m_msg.Int1),
                        (v301 = v25, v25 == 14) && v305->m_msg.Int2 >= 9))
                {
                    if (v305->m_msg.Int2 >= 9)
                    {
                        v62 = (uint32_t*)(Global_glob_reflex_univVars_objet_valeur1 + 60);
                        v21 = (*v62 - 1000)
                            * (*(uint32_t*)AI_EvalVarArray2_C(
                                1,
                                (char*)Global_glob_reflex_univVars_objet_inventaire,
                                14,
                                v305->m_msg.Int2)
                                + 2);
                        *(float*)AI_EvalVarArray_C2((char*)Global_glob_reflex_univVars_Perso_ID_Energie_max, v305->m_msg.Int2) = (float)v21;
                        v20 = *(float*)AI_EvalVarArray_C2(
                            (char*)Global_glob_reflex_univVars_Perso_ID_Energie_courante,
                            v305->m_msg.Int2);
                        if (*(float*)AI_EvalVarArray_C2(
                            (char*)Global_glob_reflex_univVars_Perso_ID_Energie_max,
                            v305->m_msg.Int2) < (double)v20)
                        {
                            v19 = *(float*)AI_EvalVarArray_C2(
                                (char*)Global_glob_reflex_univVars_Perso_ID_Energie_max,
                                v305->m_msg.Int2);
                            *(float*)AI_EvalVarArray_C2(
                                (char*)Global_glob_reflex_univVars_Perso_ID_Energie_courante,
                                v305->m_msg.Int2) = v19;
                        }
                    }
                    else
                    {
                        v63 = (uint32_t*)(Global_glob_reflex_univVars_objet_valeur1 + 116);
                        v24 = (*v63 - 1000)
                            * (*(uint32_t*)AI_EvalVarArray2_C(
                                1,
                                (char*)Global_glob_reflex_univVars_objet_inventaire,
                                28,
                                v305->m_msg.Int2)
                                + 2);
                        *(float*)AI_EvalVarArray_C2((char*)Global_glob_reflex_univVars_Perso_ID_Energie_max, v305->m_msg.Int2) = (float)v24;
                        v23 = *(float*)AI_EvalVarArray_C2(
                            (char*)Global_glob_reflex_univVars_Perso_ID_Energie_courante,
                            v305->m_msg.Int2);
                        if (*(float*)AI_EvalVarArray_C2(
                            (char*)Global_glob_reflex_univVars_Perso_ID_Energie_max,
                            v305->m_msg.Int2) < (double)v23)
                        {
                            v22 = *(float*)AI_EvalVarArray_C2(
                                (char*)Global_glob_reflex_univVars_Perso_ID_Energie_max,
                                v305->m_msg.Int2);
                            *(float*)AI_EvalVarArray_C2(
                                (char*)Global_glob_reflex_univVars_Perso_ID_Energie_courante,
                                v305->m_msg.Int2) = v22;
                        }
                    }
                }
            }
            else if (v305->m_msg.Int1 < 1100)
            {
                if (((*(uint32_t*)(Global_glob_reflex_univVars_objet_status + 4 * v305->m_msg.Int1 + 4) & 0x400000) == 0
                    || !*(uint32_t*)AI_EvalVarArray2_C(
                        1,
                        (char*)Global_glob_reflex_univVars_objet_inventaire,
                        v305->m_msg.Int1,
                        v305->m_msg.Int2))
                    && *(int*)AI_EvalVarArray2_C(
                        1,
                        (char*)Global_glob_reflex_univVars_objet_inventaire,
                        v305->m_msg.Int1,
                        v305->m_msg.Int2) < 99)
                {
                    v11 = AI_EvalVarArray2_C(
                        1,
                        (char*)Global_glob_reflex_univVars_objet_inventaire,
                        v305->m_msg.Int1,
                        v305->m_msg.Int2);
                    ++* (uint32_t*)v11;
                }
                if (v305->m_msg.Int1 + 1100 <= 0)
                    v291 = 0;
                else
                    v291 = AI_EvalFunc_ARRBitTest_C(
                        (int*)(Global_glob_reflex_univVars_Flag_speciaux_sauve + 4),
                        v305->m_msg.Int1 + 1100);
                if (v291)
                {
                    v291 = *(uint32_t*)(Global_glob_reflex_univVars_objet_status + 4 * v305->m_msg.Int1 + 4) & 0x7000000;
                    v291 >>= 24;
                    if (v291 != 7
                        && ((1 << v305->m_msg.Int2) & (unsigned __int16)*(uint32_t*)(Global_glob_reflex_univVars_objet_status
                            + 4 * v305->m_msg.Int1
                            + 4)) != 0)
                    {
                        v12 = AI_EvalVarArray2_C(
                            1,
                            (char*)Global_glob_reflex_univVars_Perso_ID_equipement,
                            v291,
                            v305->m_msg.Int2);
                        *(uint32_t*)v12 = v305->m_msg.Int1;
                        *(uint32_t*)Global_glob_reflex_GST_Global_gao_i_Perso_ID_a_checker |= 1 << v305->m_msg.Int2;
                    }
                    if (v305->m_msg.Int1 == 28 && v305->m_msg.Int2 < 9 || v305->m_msg.Int1 == 14 && v305->m_msg.Int2 >= 9)
                    {
                        v39 = AI_EvalVarArray_C2((char*)Global_glob_reflex_univVars_Perso_ID_Energie_courante, v305->m_msg.Int2);
                        *(float*)v39 = *(float*)v39 + 980.0f;
                        if (v305->m_msg.Int2 >= 9)
                        {
                            v64 = (uint32_t*)(Global_glob_reflex_univVars_objet_valeur1 + 60);
                            v35 = (*v64 - 1000)
                                * (*(uint32_t*)AI_EvalVarArray2_C(
                                    1,
                                    (char*)Global_glob_reflex_univVars_objet_inventaire,
                                    14,
                                    v305->m_msg.Int2)
                                    + 2);
                            *(float*)AI_EvalVarArray_C2((char*)Global_glob_reflex_univVars_Perso_ID_Energie_max, v305->m_msg.Int2) = (float)v35;
                            v34 = *(float*)AI_EvalVarArray_C2(
                                (char*)Global_glob_reflex_univVars_Perso_ID_Energie_courante,
                                v305->m_msg.Int2);
                            if (*(float*)AI_EvalVarArray_C2(
                                (char*)Global_glob_reflex_univVars_Perso_ID_Energie_max,
                                v305->m_msg.Int2) < (double)v34)
                            {
                                v33 = *(float*)AI_EvalVarArray_C2(
                                    (char*)Global_glob_reflex_univVars_Perso_ID_Energie_max,
                                    v305->m_msg.Int2);
                                *(float*)AI_EvalVarArray_C2(
                                    (char*)Global_glob_reflex_univVars_Perso_ID_Energie_courante,
                                    v305->m_msg.Int2) = v33;
                            }
                        }
                        else
                        {
                            v65 = (uint32_t*)(Global_glob_reflex_univVars_objet_valeur1 + 116);
                            v38 = (*v65 - 1000)
                                * (*(uint32_t*)AI_EvalVarArray2_C(
                                    1,
                                    (char*)Global_glob_reflex_univVars_objet_inventaire,
                                    28,
                                    v305->m_msg.Int2)
                                    + 2);
                            *(float*)AI_EvalVarArray_C2((char*)Global_glob_reflex_univVars_Perso_ID_Energie_max, v305->m_msg.Int2) = (float)v38;
                            v37 = *(float*)AI_EvalVarArray_C2(
                                (char*)Global_glob_reflex_univVars_Perso_ID_Energie_courante,
                                v305->m_msg.Int2);
                            if (*(float*)AI_EvalVarArray_C2(
                                (char*)Global_glob_reflex_univVars_Perso_ID_Energie_max,
                                v305->m_msg.Int2) < (double)v37)
                            {
                                v36 = *(float*)AI_EvalVarArray_C2(
                                    (char*)Global_glob_reflex_univVars_Perso_ID_Energie_max,
                                    v305->m_msg.Int2);
                                *(float*)AI_EvalVarArray_C2(
                                    (char*)Global_glob_reflex_univVars_Perso_ID_Energie_courante,
                                    v305->m_msg.Int2) = v36;
                            }
                        }
                    }
                }
            }
            else
            {
                *(uint32_t*)Global_glob_reflex_univVars_mdisk_possessionflags |= 1 << (v305->m_msg.Int1 - 76);
                *(uint32_t*)AI_EvalVarArray2_C(1, (char*)Global_glob_reflex_univVars_Perso_ID_equipement, 0, v305->m_msg.Int2) = 27;
                *(uint32_t*)(Global_glob_reflex_univVars_objet_valeur1 + 112) = v305->m_msg.Int1 - 1100;
            }
            break;
        case 68:
            *(uint32_t*)Global_glob_reflex_univVars_world_entrance_ID = v305->m_msg.Int1;
            *(uint32_t*)Global_glob_reflex_INV_Manager_gao_i_SaveDisable = 0;
            AI_EvalFunc_SAVEValidate_C();
            break;
        case 98:
            v274 = v305->m_msg.Int1;
            *(uint32_t*)(Global_glob_reflex_univVars_Perso_ID_status + 4 * v274 + 4) = v305->m_msg.Int2;
            v61 = v305->m_msg.f2 + MATH_gd_magicDoubleForRound;
            *(uint32_t*)(Global_glob_reflex_univVars_Perso_ID_world_ID + 4 * v274 + 4) = (*(uint32_t*)&v61);
            v60 = AI_EvalVarArray_C3((char*)Global_glob_reflex_univVars_Perso_ID_pos, v274);
            *(Vec3*)v60 = v305->m_msg.v1;
            v59 = AI_EvalVarArray_C3((char*)Global_glob_reflex_univVars_Perso_ID_sight, v274);
            *(Vec3*)v59 = v305->m_msg.v2;
            break;
        case 90:
            if (v305->m_msg.f2 == 1.0)
            {
                v265 = v305->m_msg.Int1;
                for (l = 0; l < 12; ++l)
                {
                    if (((1 << l) & v265) != 0)
                    {
                        v280 = *(uint32_t*)(Global_glob_reflex_univVars_Perso_ID_actor + 4 * l + 4);
                        v58 = (tGameObjectMain*)v280;
                        if (v280)
                        {
                            if (v280 <= 2)
                                v58 = AI_MainActor[v280 - 1];
                            v57 = v58;
                        }
                        else
                        {
                            v57 = 0;
                        }
                        if (v57)
                        {
                            v56 = (tGameObjectMain*)v280;
                            if (v280)
                            {
                                if (v280 <= 2)
                                    v56 = AI_MainActor[v280 - 1];
                                v55 = v56;
                            }
                            else
                            {
                                v55 = 0;
                            }
                            v13 = AI_EvalVarArray_C3((char*)&v305->__ao_actors_pos_Dimension1, l);
                            AI_EvalFunc_OBJGetPos_C(v55, (Vec3*)v13);
                            v54 = (tGameObjectMain*)v280;
                            if (v280)
                            {
                                if (v280 <= 2)
                                    v54 = AI_MainActor[v280 - 1];
                                v53 = v54;
                            }
                            else
                            {
                                v53 = 0;
                            }
                            v14 = AI_EvalVarArray_C3((char*)&v305->__ao_actors_sight_Dimension1, l);
                            AI_EvalFunc_OBJ_SightGet_C(v53, (Vec3*)v14);
                        }
                    }
                }
            }
            else
            {
                v266 = v305->m_msg.Int1;
                for (l = 0; l < 12; ++l)
                {
                    if (((1 << l) & v266) != 0)
                    {
                        v280 = *(uint32_t*)(Global_glob_reflex_univVars_Perso_ID_actor + 4 * l + 4);
                        v52 = (tGameObjectMain*)v280;
                        if (v280)
                        {
                            if (v280 <= 2)
                                v52 = AI_MainActor[v280 - 1];
                            v51 = v52;
                        }
                        else
                        {
                            v51 = 0;
                        }
                        if (v51)
                        {
                            v50 = (tGameObjectMain*)v280;
                            if (v280)
                            {
                                if (v280 <= 2)
                                    v50 = AI_MainActor[v280 - 1];
                                v49 = v50;
                            }
                            else
                            {
                                v49 = 0;
                            }
                            v15 = AI_EvalVarArray_C3((char*)&v305->__ao_actors_pos_Dimension1, l);
                            AI_EvalFunc_OBJSetPos_C(v49, (Vec3*)v15);
                            v283.x = 0.0;
                            v283.y = 0.0;
                            v283.z = 1.0;
                            v48 = (tGameObjectMain*)v280;
                            if (v280)
                            {
                                if (v280 <= 2)
                                    v48 = AI_MainActor[v280 - 1];
                                v47 = v48;
                            }
                            else
                            {
                                v47 = 0;
                            }
                            v16 = AI_EvalVarArray_C3((char*)&v305->__ao_actors_pos_Dimension1, l);
                            AI_EvalFunc_OBJ_BankingGeneralSet_C(v47, (Vec3*)v16, &v283);
                            v46 = (tGameObjectMain*)v280;
                            if (v280)
                            {
                                if (v280 <= 2)
                                    v46 = AI_MainActor[v280 - 1];
                                v45 = v46;
                            }
                            else
                            {
                                v45 = 0;
                            }
                            v17 = AI_EvalVarArray_C3((char*)&v305->__ao_actors_pos_Dimension1, l);
                            AI_EvalFunc_COLStartMatrixSet_C(v45, (Vec3*)v17);
                        }
                    }
                }
            }
            break;
        }
    LABEL_737:
        v299 = AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v305->m_msg, v299, sizeof(v305->m_msg));
    }
    if (v305->m_msg.Int2)
    {
        v310 = SND_i_Seti(9, 1);
        *AI_gppst_CurrentJumpNode = (tAINode*)13;
        TIME_Wait_C(0.40000001f);
        result = 0;
    }
    else
    {
        v292 = SND_i_Seti(9, 1);
        if (*(int*)Global_glob_reflex_GST_Global_gao_i_fade_priority <= 5000
            || AI_EvalFunc_TIMEElapsed_C(
                *(float*)Global_glob_reflex_GST_Global_gao_f_fade_start_time,
                *(float*)Global_glob_reflex_GST_Global_gao_f_fade_delay))
        {
            *(float*)Global_glob_reflex_GST_Global_gao_f_fade_start_time = TIM_gf_MainClock - TIM_gf_LockTime;
            *(uint32_t*)Global_glob_reflex_GST_Global_gao_f_fade_delay = 0x3FB33333;
            *(uint32_t*)Global_glob_reflex_GST_Global_gao_i_fade_priority = 5000;
            AI_EvalFunc_GFXDel_C_CURRENT(10);
            v264 = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v264, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v264, 2, 1);
            v259 = (tGameObjectMain*)Global_glob_reflex_INV_MatFade_gao;
            if (Global_glob_reflex_INV_MatFade_gao)
            {
                if ((unsigned int)Global_glob_reflex_INV_MatFade_gao <= 2)
                    v259 = AI_MainActor[Global_glob_reflex_INV_MatFade_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v264, v259, 8);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v264, 0, 8);
            }
            GFXSetCurGlobal(v264);
            AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(11101, 0xFF000000);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 0.40000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 1.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)12;
        TIME_Wait_C(0.40000001f);
        result = 0;
    }
    return result;
}

int Global_glob_exec_joy()
{
    int result; // eax
    float v1; // [esp+8h] [ebp-368h]
    tScriptStruct_GSTGlobal* v2; // [esp+Ch] [ebp-364h]
    Vec3* v3; // [esp+10h] [ebp-360h]
    Vec3* v4; // [esp+14h] [ebp-35Ch]
    Vec3* v5; // [esp+18h] [ebp-358h]
    Vec3* v6; // [esp+1Ch] [ebp-354h]
    Vec3* v7; // [esp+20h] [ebp-350h]
    Vec3* v8; // [esp+24h] [ebp-34Ch]
    Vec3* v9; // [esp+28h] [ebp-348h]
    Vec3* v10; // [esp+2Ch] [ebp-344h]
    float v11; // [esp+30h] [ebp-340h]
    tGameObjectMain* v12; // [esp+40h] [ebp-330h]
    Vec3* v13; // [esp+44h] [ebp-32Ch]
    Vec3* v14; // [esp+48h] [ebp-328h]
    float v15; // [esp+4Ch] [ebp-324h]
    Vec3* v16; // [esp+54h] [ebp-31Ch]
    Vec3* v17; // [esp+58h] [ebp-318h]
    Vec3* v18; // [esp+5Ch] [ebp-314h]
    Vec3* v19; // [esp+60h] [ebp-310h]
    Vec3* v20; // [esp+78h] [ebp-2F8h]
    Vec3* v21; // [esp+7Ch] [ebp-2F4h]
    Vec3* v22; // [esp+80h] [ebp-2F0h]
    Vec3* v23; // [esp+84h] [ebp-2ECh]
    Vec3* v24; // [esp+94h] [ebp-2DCh]
    Vec3* v25; // [esp+98h] [ebp-2D8h]
    Vec3* v26; // [esp+9Ch] [ebp-2D4h]
    Vec3* v27; // [esp+A0h] [ebp-2D0h]
    Vec3* v28; // [esp+B4h] [ebp-2BCh]
    Vec3* v29; // [esp+B8h] [ebp-2B8h]
    tGameObjectMain* v30; // [esp+CCh] [ebp-2A4h]
    Vec3* v31; // [esp+D0h] [ebp-2A0h]
    Vec3* v32; // [esp+D4h] [ebp-29Ch]
    Vec3* v33; // [esp+D8h] [ebp-298h]
    Vec3* v34; // [esp+DCh] [ebp-294h]
    Vec3* v35; // [esp+E0h] [ebp-290h]
    Vec3* v36; // [esp+E4h] [ebp-28Ch]
    float v37; // [esp+F4h] [ebp-27Ch]
    tGameObjectMain* v38; // [esp+104h] [ebp-26Ch]
    tGameObjectMain* v39; // [esp+10Ch] [ebp-264h]
    tGameObjectMain* v40; // [esp+114h] [ebp-25Ch]
    tGameObjectMain* v41; // [esp+11Ch] [ebp-254h]
    tGameObjectMain* v42; // [esp+120h] [ebp-250h]
    tGameObjectMain* v43; // [esp+124h] [ebp-24Ch]
    tGameObjectMain* v44; // [esp+128h] [ebp-248h]
    tGameObjectMain* v45; // [esp+12Ch] [ebp-244h]
    tGameObjectMain* v46; // [esp+130h] [ebp-240h]
    tGameObjectMain* v47; // [esp+134h] [ebp-23Ch]
    double v48; // [esp+138h] [ebp-238h]
    Vec3 a3; // [esp+158h] [ebp-218h] BYREF
    int v50; // [esp+168h] [ebp-208h]
    Vec3 v51; // [esp+16Ch] [ebp-204h] BYREF
    unsigned int v52; // [esp+178h] [ebp-1F8h]
    float v53; // [esp+17Ch] [ebp-1F4h]
    int v54; // [esp+180h] [ebp-1F0h]
    Vec3 v55; // [esp+184h] [ebp-1ECh] BYREF
    Vec3 v56; // [esp+190h] [ebp-1E0h] BYREF
    Vec3 v57; // [esp+1ACh] [ebp-1C4h] BYREF
    Vec3 v58; // [esp+1B8h] [ebp-1B8h] BYREF
    Vec3 v59; // [esp+1C4h] [ebp-1ACh] BYREF
    float v60; // [esp+1D0h] [ebp-1A0h]
    BOOL v61; // [esp+1D4h] [ebp-19Ch]
    Vec3 v63; // [esp+1F4h] [ebp-17Ch]
    Vec3 v64; // [esp+210h] [ebp-160h] BYREF
    Vec3 v65; // [esp+21Ch] [ebp-154h] BYREF
    Vec3 v66; // [esp+228h] [ebp-148h] BYREF
    Vec3 v67; // [esp+250h] [ebp-120h] BYREF
    BOOL v69; // [esp+260h] [ebp-110h]
    Vec3 a1; // [esp+270h] [ebp-100h] BYREF
    Vec3 v71; // [esp+27Ch] [ebp-F4h] BYREF
    Vec3 v72; // [esp+288h] [ebp-E8h] BYREF
    float v75; // [esp+2ACh] [ebp-C4h]
    tGameObjectMain* v76; // [esp+2B0h] [ebp-C0h]
    int v77; // [esp+2B4h] [ebp-BCh]
    Vec3 dest; // [esp+2D0h] [ebp-A0h] BYREF
    Vec3 v79; // [esp+2DCh] [ebp-94h] BYREF
    int v80; // [esp+2E8h] [ebp-88h]
    Vec3 v81; // [esp+2ECh] [ebp-84h] BYREF
    Vec3 v82; // [esp+2F8h] [ebp-78h] BYREF
    float v83; // [esp+304h] [ebp-6Ch]
    float v84; // [esp+308h] [ebp-68h]
    float v85; // [esp+30Ch] [ebp-64h]
    BOOL v87; // [esp+314h] [ebp-5Ch]
    tScriptStruct_GSTGlobal* v88; // [esp+318h] [ebp-58h]
    BOOL v89; // [esp+31Ch] [ebp-54h]
    float v90; // [esp+32Ch] [ebp-44h]
    float v91; // [esp+330h] [ebp-40h]
    float v92; // [esp+334h] [ebp-3Ch]
    BOOL v94; // [esp+348h] [ebp-28h]
    Vec3 v95; // [esp+34Ch] [ebp-24h] BYREF
    BOOL v96; // [esp+364h] [ebp-Ch]
    float v97; // [esp+368h] [ebp-8h]
    float v98; // [esp+36Ch] [ebp-4h]

    v63.x = 1.0;
    v63.y = 0.0;
    v63.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_GSTGlobal*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v88 = v2;
    v52 = 0;
    v53 = 0.0;
    v80 = 0;
    v76 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Global_glob_exec_joy_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        Global_glob_exec_joy_GST_Cheat_Manager_gao = AI_ul_RealResolveOneRefObject(0x7100E3AFu);
        AI2C_ResolveBuffer(
            &Global_glob_exec_joy_GST_Cheat_Manager_gao_i_pause_mode,
            (void*)Global_glob_exec_joy_GST_Cheat_Manager_gao,
            52);
        Global_glob_exec_joy_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &Global_glob_exec_joy_GST_Board_panel_gao_f_time_on_sort_de_linventaire,
            (void*)Global_glob_exec_joy_GST_Board_panel_gao,
            2140);
        Global_glob_exec_joy__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
    }
    v48 = MATH_gd_magicDoubleForRound + 0.94999999;
    v80 = (*(uint32_t *)&v48);
    if (v88->i_freeze_joy)
        AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)Global_glob_exec_joy_GST_Global_gao, 0x8000, 0);
    if (AI_EvalFunc_OBJ_CustomBitsTest_C(AI_gpst_CurrentGameObject, 0x8000))
        AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(0x4000, 0);
    else
        AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(0, 0x4000);
    if (*(uint32_t*)Global_glob_exec_joy_GST_Cheat_Manager_gao_i_pause_mode == 2
        || (v87 = AI_EvalFunc_OBJ_CustomBitsTest_C(AI_gpst_CurrentGameObject, 0x8000))
        || (v77 = INO_b_IoButtonPressed(6)) != 0
        && (v54 = INO_b_IoButtonPressed(4)) != 0
        && (v50 = INO_b_IoButtonPressed(7)) == 0)
    {
        v88->v_joy_camera.x = 0.0;
        v88->v_joy_camera.y = 0.0;
        v88->v_joy_camera.z = 0.0;
        v88->v_joy_camera_old.x = 0.0;
        v88->v_joy_camera_old.y = 0.0;
        v88->v_joy_camera_old.z = 0.0;
        v88->v_joy_camera_normalized.x = 0.0;
        v88->v_joy_camera_normalized.y = 0.0;
        v88->v_joy_camera_normalized.z = 0.0;
        v88->v_joy_camera_normalized_old.x = 0.0;
        v88->v_joy_camera_normalized_old.y = 0.0;
        v88->v_joy_camera_normalized_old.z = 0.0;
        v88->f_joy_norm = 0.0;
        v88->f_joy_norm_old = 0.0;
        v88->i_joy_transition_en_cours = 0;
        v88->v_joy_cam_sight.x = 0.0;
        v88->v_joy_cam_sight.y = 0.0;
        v88->v_joy_cam_sight.z = 0.0;
        v88->v_joy_cam_sight_old.x = 0.0;
        v88->v_joy_cam_sight_old.y = 0.0;
        v88->v_joy_cam_sight_old.z = 0.0;
        AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(0, 0x8000);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        v47 = (tGameObjectMain*)v88->o_joy_acteur;
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
        if (v46)
        {
            v88->i_joy_transition_en_cours = 0;
            v76 = *AI_MainActor;
            v52 = (uint32_t) AI_EvalFunc_VIEWGetObject_C(0);
            v45 = v76;
            if (v76)
            {
                if ((unsigned int)v76 <= 2)
                    v45 = AI_MainActor[(uint32_t)v76 - 1];
                v44 = v45;
            }
            else
            {
                v44 = 0;
            }
            v43 = (tGameObjectMain*)v88->o_joy_acteur;
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
            if (v44 == v42)
            {
                v41 = (tGameObjectMain*)v52;
                if (v52)
                {
                    if (v52 <= 2)
                        v41 = AI_MainActor[v52 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v41, &v88->v_joy_cam_sight);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v88->v_joy_cam_sight);
                }
                v88->v_joy_camera.x = 0.0;
                v88->v_joy_camera.y = 0.0;
                v88->v_joy_camera.z = 0.0;
                v88->v_joy_camera_normalized.x = 0.0;
                v88->v_joy_camera_normalized.y = 0.0;
                v88->v_joy_camera_normalized.z = 0.0;
                v88->f_joy_norm = 0.0;
            }
            else
            {
                v40 = (tGameObjectMain*)v52;
                if (v52)
                {
                    if (v52 <= 2)
                        v40 = AI_MainActor[v52 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v40, &v88->v_joy_cam_sight);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v88->v_joy_cam_sight);
                }
                v39 = (tGameObjectMain*)v88->o_joy_acteur;
                if (v39)
                {
                    if ((unsigned int)v39 <= 2)
                        v39 = AI_MainActor[(uint32_t)v39 - 1];
                    AI_EvalFunc_OBJGetPos_C(v39, &a3);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &a3);
                }
                v38 = v76;
                if (v76)
                {
                    if ((unsigned int)v76 <= 2)
                        v38 = AI_MainActor[(uint32_t)v76 - 1];
                    AI_EvalFunc_OBJGetPos_C(v38, &v66);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v66);
                }
                MATH_SubstractVec(&dest, &a3, &v66);
                v88->v_joy_camera = dest;
                v88->v_joy_camera.z = 0.0;
                v37 = 1.0f / 3.0f;
                a1.x = v37 * v88->v_joy_camera.x;
                a1.y = v88->v_joy_camera.y * v37;
                a1.z = v88->v_joy_camera.z * v37;
                v97 = MATH_VecLen(&a1);
                if (v97 <= 1.0)
                {
                    if (v97 >= (double)v88->f_joy_min)
                        v75 = v97;
                    else
                        v75 = v88->f_joy_min;
                }
                else
                {
                    v75 = 1.0;
                }
                v88->f_joy_norm = v75;
                AI_EvalFunc_MATHVecSetNormVector_C(&v88->v_joy_camera, v88->f_joy_norm);
                AI_EvalFunc_MATHVecNormalize_C(&v88->v_joy_camera, &v88->v_joy_camera_normalized);
            }
            v35 = &v88->v_joy_cam_sight_old;
            v36 = &v88->v_joy_cam_sight;
            v88->v_joy_cam_sight_old.x = v88->v_joy_cam_sight.x;
            v35->y = v36->y;
            v35->z = v36->z;
            v88->f_joy_norm_old = v88->f_joy_norm;
            v33 = &v88->v_joy_camera_old;
            v34 = &v88->v_joy_camera;
            v88->v_joy_camera_old.x = v88->v_joy_camera.x;
            v33->y = v34->y;
            v33->z = v34->z;
            v31 = &v88->v_joy_camera_normalized_old;
            v32 = &v88->v_joy_camera_normalized;
            v88->v_joy_camera_normalized_old.x = v88->v_joy_camera_normalized.x;
            v31->y = v32->y;
            v31->z = v32->z;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            v88->f_joy_min = 0.0;
            v52 = (uint32_t)AI_EvalFunc_VIEWGetObject_C(0);
            v30 = (tGameObjectMain*)v52;
            if (v52)
            {
                if (v52 <= 2)
                    v30 = AI_MainActor[v52 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v30, &v88->v_joy_cam_sight);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v88->v_joy_cam_sight);
            }
            AI_EvalFunc_MATHVecSetHorzNormalize_C(&v88->v_joy_cam_sight);
            AI_EvalFunc_IoJoyGetMove_C(&v51);
            v90 = -v51.x;
            v91 = -v51.y;
            v92 = -v51.z;
            v29 = &v88->v_joy_camera;
            v88->v_joy_camera.x = -v51.x;
            v29->y = v91;
            v29->z = v92;
            if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(8))
            {
                v28 = &v88->v_joy_camera;
                v88->v_joy_camera.x = v88->v_joy_camera.x * -1.0f;
                v28->y = v28->y * -1.0f;
                v28->z = v28->z * -1.0f;
            }
            v89 = AI_EvalFunc_TIMEElapsed_C(
                *(float*)Global_glob_exec_joy_GST_Board_panel_gao_f_time_on_sort_de_linventaire,
                0.5);
            if (v89
                && !(v69 = AI_EvalFunc_OBJ_CapaTest_C((tGameObjectMain*)Global_glob_exec_joy_GST_Board_panel_gao, 0x100)))
            {
                if (!v88->i_joy_transition_en_cours && !AI_EvalFunc_MTHVecNullToler_C(&v88->v_joy_camera, 0.34999999f))
                {
                    if (fabs(v88->v_joy_cam_sight_old.x) != 0.0)
                        v94 = 0;
                    else
                        v94 = fabs(v88->v_joy_cam_sight_old.y) == 0.0
                        && fabs(v88->v_joy_cam_sight_old.z) == 0.0;
                    if (!v94)
                    {
                        if (fabs(v88->v_joy_cam_sight.x) != 0.0)
                            v61 = 0;
                        else
                            v61 = fabs(v88->v_joy_cam_sight.y) == 0.0
                            && fabs(v88->v_joy_cam_sight.z) == 0.0;
                        if (!v61)
                        {
                            v98 = v88->v_joy_cam_sight.z * v88->v_joy_cam_sight_old.z
                                + v88->v_joy_cam_sight.y * v88->v_joy_cam_sight_old.y
                                + v88->v_joy_cam_sight_old.x * v88->v_joy_cam_sight.x;
                            if (v98 < 0.70710701 && !v88->i_disable_hysteresis)
                            {
                                v88->i_joy_transition_en_cours = 1;
                                v26 = &v88->v_joy_camera_startblend;
                                v27 = &v88->v_joy_camera_old;
                                v88->v_joy_camera_startblend.x = v88->v_joy_camera_old.x;
                                v26->y = v27->y;
                                v26->z = v27->z;
                                v24 = &v88->v_joy_camera_normalized_start;
                                v25 = &v88->v_joy_camera_normalized_old;
                                v88->v_joy_camera_normalized_start.x = v88->v_joy_camera_normalized_old.x;
                                v24->y = v25->y;
                                v24->z = v25->z;
                                v88->f_joy_norm_startblend = v88->f_joy_norm_old;
                                AI_EvalFunc_MATHVecNormalize_C(&v88->v_joy_camera, &v88->v_joy_camera_pour_blend);
                                v88->f_joy_timeref = TIM_gf_MainClock - TIM_gf_LockTime;
                            }
                        }
                    }
                }
            }
            else
            {
                v88->i_joy_transition_en_cours = 0;
            }
            AI_EvalFunc_IoJoyGetMove_C(&v57);
            v83 = -v57.x;
            v84 = -v57.y;
            v85 = -v57.z;
            v23 = &v88->v_joy_camera;
            v88->v_joy_camera.x = -v57.x;
            v23->y = v84;
            v23->z = v85;
            if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(8))
            {
                v22 = &v88->v_joy_camera;
                v88->v_joy_camera.x = v88->v_joy_camera.x * -1.0f;
                v22->y = v22->y * -1.0f;
                v22->z = v22->z * -1.0f;
            }
            if (AI_EvalFunc_MTHVecNullToler_C(&v88->v_joy_camera, 0.34999999f))
            {
                v21 = &v88->v_joy_camera;
                v88->v_joy_camera.x = 0.0;
                v21->y = 0.0;
                v21->z = 0.0;
                v20 = &v88->v_joy_camera_normalized;
                v88->v_joy_camera_normalized.x = 0.0;
                v20->y = 0.0;
                v20->z = 0.0;
                v88->f_joy_norm = 0.0;
                v88->i_joy_transition_en_cours = 0;
            }
            else
            {
                v53 = 0.0;
                if (v88->i_joy_transition_en_cours)
                {
                    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Global_glob_exec_joy__PJ_Sally_gao, &v64);
                    v79.x = 0.0;
                    v79.y = 0.0;
                    v79.z = 0.5;
                    MATH_VecAddVec(&v59, &v64, &v79);
                    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Global_glob_exec_joy__PJ_Sally_gao, &v65);
                    v95.x = 0.0;
                    v95.y = 0.0;
                    v95.z = 0.5;
                    MATH_VecAddVec(&v81, &v65, &v95);
                    AI_EvalFunc_IoJoyGetMove_C(&v55);
                    v63 = v55;
                    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(8))
                    {
                        v63.x = v63.x * -1.0f;
                        v63.y = v63.y * -1.0f;
                        v63.z = v63.z * -1.0f;
                    }
                    v96 = AI_EvalFunc_TIMEElapsed_C(v88->f_joy_timeref, 0.80000001f);
                    if (v96
                        && (v58.x = -v63.x,
                            v58.y = -v63.y,
                            v58.z = -v63.z,
                            AI_EvalFunc_MATHVecNormalize_C(&v58, &v67),
                            v60 = v67.z * v88->v_joy_camera_pour_blend.z
                            + v67.y * v88->v_joy_camera_pour_blend.y
                            + v67.x * v88->v_joy_camera_pour_blend.x,
                            v1 = (float)v80,
                            v60 < (double)v1))
                    {
                        v88->i_joy_transition_en_cours = 0;
                    }
                    else
                    {
                        v18 = &v88->v_joy_camera;
                        v19 = &v88->v_joy_camera_startblend;
                        v88->v_joy_camera.x = v88->v_joy_camera_startblend.x;
                        v18->y = v19->y;
                        v18->z = v19->z;
                        v16 = &v88->v_joy_camera_normalized;
                        v17 = &v88->v_joy_camera_normalized_start;
                        v88->v_joy_camera_normalized.x = v88->v_joy_camera_normalized_start.x;
                        v16->y = v17->y;
                        v16->z = v17->z;
                        v53 = v88->f_joy_norm_startblend;
                    }
                }
                if (!v88->i_joy_transition_en_cours)
                {
                    v53 = MATH_VecLen(&v88->v_joy_camera);
                    if (v53 > 0.92500001)
                    {
                        v14 = &v88->v_joy_camera;
                        v13 = &v88->v_joy_camera;
                        v15 = 1.0f / v53;
                        v88->v_joy_camera.x = v15 * v88->v_joy_camera.x;
                        v13->y = v14->y * v15;
                        v13->z = v14->z * v15;
                        v53 = 1.0;
                    }
                    v12 = (tGameObjectMain*)v52;
                    if (v52)
                    {
                        if (v52 <= 2)
                            v12 = AI_MainActor[v52 - 1];
                        AI_EvalFunc_MATHVecLocalToGlobal_C(v12, &v88->v_joy_camera, &v82);
                    }
                    else
                    {
                        AI_EvalFunc_MATHVecLocalToGlobal_C(0, &v88->v_joy_camera, &v82);
                    }
                    v88->v_joy_camera = v82;
                    v88->v_joy_camera.z = 0.0;
                    AI_EvalFunc_MATHVecSetNormVector_C(&v88->v_joy_camera, v53);
                    v10 = &v88->v_joy_camera;
                    v9 = &v88->v_joy_camera_normalized;
                    v11 = 1.0f / v53;
                    v88->v_joy_camera_normalized.x = v11 * v88->v_joy_camera.x;
                    v9->y = v10->y * v11;
                    v9->z = v10->z * v11;
                }
                AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Global_glob_exec_joy__PJ_Sally_gao, &v71);
                v56.x = 0.0;
                v56.y = 0.0;
                v56.z = 1.0;
                MATH_VecAddVec(&v72, &v71, &v56);
                v88->f_joy_norm = v53;
            }
            v7 = &v88->v_joy_cam_sight_old;
            v8 = &v88->v_joy_cam_sight;
            v88->v_joy_cam_sight_old.x = v88->v_joy_cam_sight.x;
            v7->y = v8->y;
            v7->z = v8->z;
            v88->f_joy_norm_old = v88->f_joy_norm;
            v5 = &v88->v_joy_camera_old;
            v6 = &v88->v_joy_camera;
            v88->v_joy_camera_old.x = v88->v_joy_camera.x;
            v5->y = v6->y;
            v5->z = v6->z;
            v3 = &v88->v_joy_camera_normalized_old;
            v4 = &v88->v_joy_camera_normalized;
            v88->v_joy_camera_normalized_old.x = v88->v_joy_camera_normalized.x;
            v3->y = v4->y;
            v3->z = v4->z;
            result = 1000;
        }
    }
    return result;
}

int Global_glob_init_object()
{
    int i; // [esp+ECh] [ebp-4h]

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Global_glob_init_object_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&Global_glob_init_object_univVars_objet_status, (void*)Global_glob_init_object_univVars, 3896);
        AI2C_ResolveBuffer(&Global_glob_init_object_univVars_objet_valeur1, (void*)Global_glob_init_object_univVars, 4100);
    }
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 4) = 0x5000100;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 8) = 1;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 12) = 0x100100;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 20) = 0x6100100;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 24) = 0x4100100;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 28) = 0x10FFFF;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 36) = 1;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 40) = 0xFFFF;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 44) = 0x2400100;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_valeur1 + 44) = 5;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 48) = 0x10FFFF;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 52) = 0x100;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 56) = 0x10020103;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_valeur1 + 56) = 0x50;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 60) = 0x7000600;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_valeur1 + 60) = 0x410;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 64) = 0x10020600;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_valeur1 + 64) = 0x28;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 68) = 0x10020600;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_valeur1 + 68) = 0x3D4;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 72) = 0x100;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 76) = 0x3100100;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 80) = 0x1100100;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 88) = 0x10FFFF;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 92) = 0x10FFFF;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 96) = 0x10020103;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_valeur1 + 96) = 0x3D4;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 100) = 0x10FFFF;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 104) = 0x3000002;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 108) = 0x1FFFF;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 112) = 0x100100;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 116) = 0x7000103;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_valeur1 + 116) = 0x410;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 120) = 0x1000001;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 124) = 0x10020103;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_valeur1 + 124) = 0x28;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 128) = 0x7000100;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 132) = 0x100100;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 136) = 0x100100;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 140) = 0x100100;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 144) = 2;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 148) = 0x2000400;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 152) = 0x400;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 156) = 0x6000400;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 160) = 0x5000400;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 164) = 0x7000600;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_valeur1 + 164) = 0;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 168) = 0x3000400;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 172) = 0x1000400;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 176) = 0x200;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 180) = 0x3000200;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 184) = 0x100;
    for (i = 0; i < 50; ++i)
    {
        if ((*(uint32_t*)(Global_glob_init_object_univVars_objet_status + 4 * i + 4) & 0x7000000) == 0)
            *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 4 * i + 4) |= 0x200000u;
    }
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 132) &= ~0x200000u;
    *(uint32_t*)(Global_glob_init_object_univVars_objet_status + 136) &= ~0x200000u;
    return 1000;
}

int Global_glob_exec_flash()
{
    tScriptStruct_GSTGlobal* v1; // [esp+8h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GSTGlobal*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (v1->i_flash_on)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v1->f_flash_time, 0.30000001f))
            v1->i_flash_on = 0;
    }
    else
    {
        v1->f_flash_time = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    return 1000;
}

int Global_glob_exec_hauteur_eau()
{
    tScriptStruct_GSTGlobal* v1; // [esp+0h] [ebp-40h]
    tGameObjectMain* v2; // [esp+8h] [ebp-38h]
    tGameObjectMain* v3; // [esp+Ch] [ebp-34h]
    tGameObjectMain* v4; // [esp+10h] [ebp-30h]
    tGameObjectMain* v5; // [esp+18h] [ebp-28h]
    tGameObjectMain* v6; // [esp+1Ch] [ebp-24h]
    tGameObjectMain* v7; // [esp+20h] [ebp-20h]
    Vec3 a3; // [esp+24h] [ebp-1Ch] BYREF
    Vec3 v9; // [esp+30h] [ebp-10h] BYREF
    tScriptStruct_GSTGlobal* v10; // [esp+3Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GSTGlobal*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v10 = v1;
    v7 = (tGameObjectMain*)v1->o_eau_acteur;
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
        v5 = (tGameObjectMain*)v10->o_eau_acteur;
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
        v10->f_eau_hauteurZ = a3.z + 0.0f;
    }
    else
    {
        v10->f_eau_hauteurZ = -1.0e14f;
    }
    v4 = (tGameObjectMain*)v10->o_ciel_acteur;
    if (v4)
    {
        if ((unsigned int)v4 <= 2)
            v4 = AI_MainActor[(uint32_t)v4 - 1];
        v3 = v4;
    }
    else
    {
        v3 = 0;
    }
    if (v3)
    {
        v2 = (tGameObjectMain*)v10->o_ciel_acteur;
        if (v2)
        {
            if ((unsigned int)v2 <= 2)
                v2 = AI_MainActor[(uint32_t)v2 - 1];
            AI_EvalFunc_OBJGetPos_C(v2, &v9);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v9);
        }
        v10->f_ciel_hauteurZ = v9.z;
    }
    else
    {
        v10->f_ciel_hauteurZ = 1.0e14f;
    }
    return 1000;
}

// ============================================

int world_cycle_init_wsi_init()
{
    int result; // eax
    float v1; // [esp+0h] [ebp-74h]
    float v2; // [esp+4h] [ebp-70h]
    float v3; // [esp+8h] [ebp-6Ch]
    float v4; // [esp+Ch] [ebp-68h]
    tScriptStruct_WorldCycleInit* v5; // [esp+10h] [ebp-64h]
    tAI* v6; // [esp+20h] [ebp-54h]
    tAI* v7; // [esp+24h] [ebp-50h]
    tGameObjectMain* v8; // [esp+28h] [ebp-4Ch]
    tGameObjectMain* v9; // [esp+2Ch] [ebp-48h]
    tGameObjectMain* v10; // [esp+30h] [ebp-44h]
    tGameObjectMain* v11; // [esp+34h] [ebp-40h]
    int v12; // [esp+58h] [ebp-1Ch]
    BOOL v13; // [esp+70h] [ebp-4h]
    BOOL v14; // [esp+70h] [ebp-4h]
    BOOL v15; // [esp+70h] [ebp-4h]
    BOOL v16; // [esp+70h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_WorldCycleInit*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        world_cycle_init_wsi_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &world_cycle_init_wsi_init_univVars_Flag_speciaux_sauve,
            (void*)world_cycle_init_wsi_init_univVars,
            4488);
        AI2C_ResolveBuffer(
            &world_cycle_init_wsi_init_univVars_Planetes_CurRotation,
            (void*)world_cycle_init_wsi_init_univVars,
            5984);
    }
    if (v5->des_i_specialflag1 <= 0)
        v13 = 0;
    else
        v13 = AI_EvalFunc_ARRBitTest_C(
            (int*)(world_cycle_init_wsi_init_univVars_Flag_speciaux_sauve + 4),
            v5->des_i_specialflag1);
    if (v13)
    {
        v4 = v5->des_f_specialtime1;
        *(float*)AI_EvalVarArray_C2((char*)world_cycle_init_wsi_init_univVars_Planetes_CurRotation, v5->des_i_numplanete) = v4;
        if (v5->des_i_specialflag1 > 0)
            AI_EvalFunc_ARRBitClear_C(
                (int*)(world_cycle_init_wsi_init_univVars_Flag_speciaux_sauve + 4),
                v5->des_i_specialflag1);
    }
    if (v5->des_i_specialflag2 <= 0)
        v14 = 0;
    else
        v14 = AI_EvalFunc_ARRBitTest_C(
            (int*)(world_cycle_init_wsi_init_univVars_Flag_speciaux_sauve + 4),
            v5->des_i_specialflag2);
    if (v14)
    {
        v3 = v5->des_f_specialtime2;
        *(float*)AI_EvalVarArray_C2((char*)world_cycle_init_wsi_init_univVars_Planetes_CurRotation, v5->des_i_numplanete) = v3;
        if (v5->des_i_specialflag2 > 0)
            AI_EvalFunc_ARRBitClear_C(
                (int*)(world_cycle_init_wsi_init_univVars_Flag_speciaux_sauve + 4),
                v5->des_i_specialflag2);
    }
    if (v5->des_i_specialflag3 <= 0)
        v15 = 0;
    else
        v15 = AI_EvalFunc_ARRBitTest_C(
            (int*)(world_cycle_init_wsi_init_univVars_Flag_speciaux_sauve + 4),
            v5->des_i_specialflag3);
    if (v15)
    {
        v2 = v5->des_f_specialtime3;
        *(float*)AI_EvalVarArray_C2((char*)world_cycle_init_wsi_init_univVars_Planetes_CurRotation, v5->des_i_numplanete) = v2;
        if (v5->des_i_specialflag3 > 0)
            AI_EvalFunc_ARRBitClear_C(
                (int*)(world_cycle_init_wsi_init_univVars_Flag_speciaux_sauve + 4),
                v5->des_i_specialflag3);
    }
    if (v5->des_i_specialflag4 <= 0)
        v16 = 0;
    else
        v16 = AI_EvalFunc_ARRBitTest_C(
            (int*)(world_cycle_init_wsi_init_univVars_Flag_speciaux_sauve + 4),
            v5->des_i_specialflag4);
    if (v16)
    {
        v1 = v5->des_f_specialtime4;
        *(float*)AI_EvalVarArray_C2((char*)world_cycle_init_wsi_init_univVars_Planetes_CurRotation, v5->des_i_numplanete) = v1;
        if (v5->des_i_specialflag4 > 0)
            AI_EvalFunc_ARRBitClear_C(
                (int*)(world_cycle_init_wsi_init_univVars_Flag_speciaux_sauve + 4),
                v5->des_i_specialflag4);
    }
    v11 = (tGameObjectMain*)v5->des_o_cycle;
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
    {
        v7 = (tAI*)v5->des_o_cycle;
        if (v7 == WOR_gst_Universe.univAIInstance)
        {
            v6 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v9 = (tGameObjectMain*)v5->des_o_cycle;
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
        *((Vec3*)v6->instanceVarDataCurrent + 3) = v5->des_v_center_world;// todo fixme ez mi?
    }
    v12 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
    if (v12 == 1000)
        result = 1000;
    else
        result = v12;
    return result;
}

// ====================================

int move_with_me_move_with_me_init()
{
    int result; // eax
    tScriptStruct_MoveWithMe* v1; // [esp+0h] [ebp-5Ch]
    tGameObjectMain* v2; // [esp+20h] [ebp-3Ch]
    Vec3 v3; // [esp+24h] [ebp-38h] BYREF
    Vec3 a3; // [esp+30h] [ebp-2Ch] BYREF
    Vec3 v5; // [esp+3Ch] [ebp-20h]
    int v6; // [esp+48h] [ebp-14h]
    Vec3 a1; // [esp+4Ch] [ebp-10h] BYREF
    tScriptStruct_MoveWithMe* v8; // [esp+58h] [ebp-4h]

    v5.x = 1.0;
    v5.y = 0.0;
    v5.z = 0.0;
    v3.x = 1.0;
    v3.y = 0.0;
    v3.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_MoveWithMe*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v8 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_move_with_me_move_with_me_init_move_with_me_move_with_me_loop = (int)AI_ul_RealResolveOneRefFunction(0x5B000387u);
    }
    v2 = (tGameObjectMain*)v8->des_o_followed;
    if (v8->des_o_followed)
    {
        if ((unsigned int)v2 <= 2)
            v2 = AI_MainActor[(uint32_t)v2 - 1];
        AI_EvalFunc_OBJGetPos_C(v2, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    v8->v_lastpos = a3;
    v3 = v8->v_lastpos;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
    v5 = a1;
    v3.z = a1.z;
    AI_EvalFunc_OBJSetPos_C_CURRENT(&v3);
    v6 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_move_with_me_move_with_me_init_move_with_me_move_with_me_loop);
    if (v6 == 1000)
        result = 1000;
    else
        result = v6;
    return result;
}

int move_with_me_move_with_me_loop()
{
    tScriptStruct_MoveWithMe* v1; // [esp+0h] [ebp-C0h]
    tGameObjectMain* v2; // [esp+8h] [ebp-B8h]
    tGameObjectMain* v3; // [esp+10h] [ebp-B0h]
    tGameObjectMain* v4; // [esp+18h] [ebp-A8h]
    tGameObjectMain* v5; // [esp+28h] [ebp-98h]
    tGameObjectMain* v6; // [esp+2Ch] [ebp-94h]
    tGameObjectMain* v7; // [esp+30h] [ebp-90h]
    tGameObjectMain* v8; // [esp+40h] [ebp-80h]
    Vec3 v9; // [esp+44h] [ebp-7Ch] BYREF
    Vec3 v10; // [esp+50h] [ebp-70h] BYREF
    Vec3 v11; // [esp+5Ch] [ebp-64h] BYREF
    Vec3 v12; // [esp+68h] [ebp-58h] BYREF
    Vec3 a3; // [esp+74h] [ebp-4Ch] BYREF
    Vec3 v14; // [esp+80h] [ebp-40h] BYREF
    Vec3 v15; // [esp+8Ch] [ebp-34h] BYREF
    Vec3 dest; // [esp+A4h] [ebp-1Ch] BYREF
    tScriptStruct_MoveWithMe* v17; // [esp+B0h] [ebp-10h]
    Vec3 v18; // [esp+B4h] [ebp-Ch] BYREF

    v11.x = 1.0;
    v11.y = 0.0;
    v11.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_MoveWithMe*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v17 = v1;
    v8 = (tGameObjectMain*)v1->des_o_followed;
    if (v1->des_o_followed)
    {
        if ((unsigned int)v8 <= 2)
            v8 = AI_MainActor[(uint32_t)v8 - 1];
        AI_EvalFunc_OBJGetPos_C(v8, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    MATH_SubstractVec(&dest, &a3, &v17->v_lastpos);
    v11.x = dest.x;
    v11.y = dest.y;
    v11.z = 0.0;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v15);
    MATH_VecAddVec(&v10, &v15, &v11);
    AI_EvalFunc_OBJSetPos_C_CURRENT(&v10);
    v7 = (tGameObjectMain*)v17->des_o_sky;
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
        v5 = (tGameObjectMain*)v17->des_o_followed;
        if (v17->des_o_followed)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            AI_EvalFunc_OBJGetPos_C(v5, &v9);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v9);
        }
        MATH_SubstractVec(&v14, &v9, &v17->v_lastpos);
        v11.x = 0.0;
        v11.y = 0.0;
        v11.z = v14.z * 0.5f;
        v4 = (tGameObjectMain*)v17->des_o_sky;
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            AI_EvalFunc_OBJGetPos_C(v4, &v18);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v18);
        }
        MATH_VecAddVec(&v12, &v18, &v11);
        v3 = (tGameObjectMain*)v17->des_o_sky;
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            AI_EvalFunc_OBJSetPos_C(v3, &v12);
        }
        else
        {
            AI_EvalFunc_OBJSetPos_C(0, &v12);
        }
    }
    v2 = (tGameObjectMain*)v17->des_o_followed;
    if (v17->des_o_followed)
    {
        if ((unsigned int)v2 <= 2)
            v2 = AI_MainActor[(uint32_t)v2 - 1];
        AI_EvalFunc_OBJGetPos_C(v2, &v17->v_lastpos);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v17->v_lastpos);
    }
    return 1000;
}

// =======================================

int planete_rotation_pr_init()
{
    float* v0; // eax
    int result; // eax
    tScriptStruct_PlaneteRotation* v2; // [esp+4h] [ebp-14h]
    tGameObjectMain* a1; // [esp+8h] [ebp-10h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-Ch]
    int v5; // [esp+10h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PlaneteRotation*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        planete_rotation_pr_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &planete_rotation_pr_init_univVars_Planetes_CurRotation,
            (void*)planete_rotation_pr_init_univVars,
            5984);
        fct_planete_rotation_pr_init_planete_rotation_pr_loop = (int)AI_ul_RealResolveOneRefFunction(0x5E00021Eu);
    }
    v4 = (tGameObjectMain*)v2->des_o_env;
    if (v4)
    {
        if ((unsigned int)v4 <= 2)
            v4 = AI_MainActor[(uint32_t)v4 - 1];
        a1 = v4;
    }
    else
    {
        a1 = 0;
    }
    v0 = (float*)AI_EvalVarArray_C2((char*)planete_rotation_pr_init_univVars_Planetes_CurRotation, v2->des_i_numplanete);
    AI_EvalFunc_OBJRotateLocalZ_c(a1, *v0);
    v5 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_planete_rotation_pr_init_planete_rotation_pr_loop);
    if (v5 == 1000)
        result = 1000;
    else
        result = v5;
    return result;
}

int planete_rotation_pr_loop()
{
    float* v0; // edx
    tScriptStruct_PlaneteRotation* v2; // [esp+4h] [ebp-18h]
    tGameObjectMain* v3; // [esp+Ch] [ebp-10h]
    float a2; // [esp+10h] [ebp-Ch]
    float v5; // [esp+14h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PlaneteRotation*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        planete_rotation_pr_loop_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &planete_rotation_pr_loop_GST_Global_gao_Planetes_Rotations,
            (void*)planete_rotation_pr_loop_GST_Global_gao,
            2784);
        planete_rotation_pr_loop_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &planete_rotation_pr_loop_univVars_Planetes_CurRotation,
            (void*)planete_rotation_pr_loop_univVars,
            5984);
    }
    v5 = TIM_gf_dt;
    a2 = *(float*)AI_EvalVarArray_C2(
        (char*)planete_rotation_pr_loop_GST_Global_gao_Planetes_Rotations,
        v2->des_i_numplanete)
        * v5;
    v3 = (tGameObjectMain*)v2->des_o_env;
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        AI_EvalFunc_OBJRotateLocalZ_c(v3, a2);
    }
    else
    {
        AI_EvalFunc_OBJRotateLocalZ_c(0, a2);
    }
    v0 = (float*)AI_EvalVarArray_C2((char*)planete_rotation_pr_loop_univVars_Planetes_CurRotation, v2->des_i_numplanete);
    *v0 = a2 + *v0;
    return 1000;
}

// ==========================================

int Zone_OccupationTerrain_zo_init()
{
    int result; // eax
    tScriptStruct_ZoneOccupationTerrain* v1; // [esp+0h] [ebp-34h]
    tGameObjectMain* v2; // [esp+4h] [ebp-30h]
    tGameObjectMain* v3; // [esp+8h] [ebp-2Ch]
    tGameObjectMain* v4; // [esp+10h] [ebp-24h]
    int v5; // [esp+20h] [ebp-14h]
    int i; // [esp+28h] [ebp-Ch]
    int j; // [esp+28h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ZoneOccupationTerrain*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Zone_OccupationTerrain_zo_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Zone_OccupationTerrain_zo_init_univVars_Flag_speciaux_sauve,
            (void*)Zone_OccupationTerrain_zo_init_univVars,
            4488);
        AI2C_ResolveBuffer(
            &Zone_OccupationTerrain_zo_init_univVars_Perso_ID_Companion,
            (void*)Zone_OccupationTerrain_zo_init_univVars,
            1040);
        AI2C_ResolveBuffer(
            &Zone_OccupationTerrain_zo_init_univVars_Perso_ID_actor,
            (void*)Zone_OccupationTerrain_zo_init_univVars,
            988);
        fct_Zone_OccupationTerrain_zo_init_Zone_OccupationTerrain_zo_dehors = (int)AI_ul_RealResolveOneRefFunction(0x9E002535);
    }
    if (v1->SF_Activation)
    {
        if (v1->SF_Activation <= 0)
            v5 = 0;
        else
            v5 = v1->SF_Activation < 10000 ? AI_EvalFunc_ARRBitTest_C(
                (int*)(Zone_OccupationTerrain_zo_init_univVars_Flag_speciaux_sauve + 4),
                v1->SF_Activation) : 1
            - AI_EvalFunc_ARRBitTest_C(
                (int*)(Zone_OccupationTerrain_zo_init_univVars_Flag_speciaux_sauve
                    + 4),
                v1->SF_Activation - 10000);
        if (!v5)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
    }
    for (i = 0; i < v1->des_i_nbWps; ++i)
    {
        v4 = (tGameObjectMain*)v1->des_o_tabWps[i];
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            AI_EvalFunc_OBJ_FlagsIdentitySet_C(v4, 0x2000, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagsIdentitySet_C(0, 0x2000, 0);
        }
    }
    if (v1->des_i_qui == -1)
    {
        v1->o_Actor_Test = (int)AI_MainActor[1];
        v1->i_Actor_Test = *(uint32_t*)Zone_OccupationTerrain_zo_init_univVars_Perso_ID_Companion;
        if (v1->i_Actor_Test > 5)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
    }
    else
    {
        v1->o_Actor_Test = *(uint32_t*)(Zone_OccupationTerrain_zo_init_univVars_Perso_ID_actor + 4 * v1->des_i_qui + 4);
        v1->i_Actor_Test = v1->des_i_qui;
    }
    v3 = (tGameObjectMain*)v1->o_Actor_Test;
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
    {
        for (j = v1->des_i_nbWps; j < 5; ++j)
            v1->des_o_tabWps[j] = 0;
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Zone_OccupationTerrain_zo_init_Zone_OccupationTerrain_zo_dehors);
        result = 1000;
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int Zone_OccupationTerrain_zo_dehors()
{
    int v1; // eax
    int v2; // eax
    int v3; // eax
    tScriptStruct_ZoneOccupationTerrain* v4; // [esp+8h] [ebp-40h]
    tGameObjectMain* v5; // [esp+10h] [ebp-38h]
    tGameObjectMain* v6; // [esp+18h] [ebp-30h]
    tGameObjectMain* v7; // [esp+20h] [ebp-28h]
    int v8; // [esp+38h] [ebp-10h]
    int v9; // [esp+44h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_ZoneOccupationTerrain*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Zone_OccupationTerrain_zo_dehors_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Zone_OccupationTerrain_zo_dehors_univVars_Perso_ID_Companion,
            (void*)Zone_OccupationTerrain_zo_dehors_univVars,
            1040);
        AI2C_ResolveBuffer(
            &Zone_OccupationTerrain_zo_dehors_univVars_Perso_ID_status,
            (void*)Zone_OccupationTerrain_zo_dehors_univVars,
            484);
        fct_Zone_OccupationTerrain_zo_dehors_Zone_OccupationTerrain_zo_exec_transfert = (int)AI_ul_RealResolveOneRefFunction(0x9E002533);
        fct_Zone_OccupationTerrain_zo_dehors_Zone_OccupationTerrain_zo_dans_la_place = (int)AI_ul_RealResolveOneRefFunction(0x9E002534);
    }
    if (*(uint32_t*)Zone_OccupationTerrain_zo_dehors_univVars_Perso_ID_Companion == -1
        || *(int*)Zone_OccupationTerrain_zo_dehors_univVars_Perso_ID_Companion >= 5)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    v8 = 0;
    if (*(uint32_t*)(Zone_OccupationTerrain_zo_dehors_univVars_Perso_ID_status + 4 * v4->i_Actor_Test + 4))
    {
        if (*(uint32_t*)(Zone_OccupationTerrain_zo_dehors_univVars_Perso_ID_status + 4 * v4->i_Actor_Test + 4) == 4)
        {
            v5 = (tGameObjectMain*)v4->o_Actor_Test;
            if (v5)
            {
                if ((unsigned int)v5 <= 2)
                    v5 = AI_MainActor[(uint32_t)v5 - 1];
                v3 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, v5);
            }
            else
            {
                v3 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, 0);
            }
            if (v3)
            {
                v8 = 1;
                v4->i_capa_out = 1;
            }
        }
    }
    else
    {
        v7 = *AI_MainActor;
        if (*AI_MainActor)
        {
            if ((unsigned int)v7 <= 2)
                v7 = AI_MainActor[(uint32_t)v7 - 1];
            v1 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, v7);
        }
        else
        {
            v1 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, 0);
        }
        if (v1)
        {
            v6 = (tGameObjectMain*)v4->o_Actor_Test;
            if (v6)
            {
                if ((unsigned int)v6 <= 2)
                    v6 = AI_MainActor[(uint32_t)v6 - 1];
                v2 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, v6);
            }
            else
            {
                v2 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, 0);
            }
            if (v2)
            {
                v8 = 1;
                v4->i_capa_out = 0;
            }
        }
    }
    if (v8)
    {
        if (v4->f_time == 0.0)
            v4->f_time = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    else
    {
        v4->f_time = 0.0;
    }
    if (v4->f_time != 0.0 && AI_EvalFunc_TIMEElapsed_C(v4->f_time, v4->des_f_delai))
    {
        v9 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Zone_OccupationTerrain_zo_dehors_Zone_OccupationTerrain_zo_exec_transfert);
        if (v9 != 1000)
            return v9;
        v4->f_time = 0.0;
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Zone_OccupationTerrain_zo_dehors_Zone_OccupationTerrain_zo_dans_la_place);
    }
    return 1000;
}

int Zone_OccupationTerrain_zo_exec_transfert()
{
    tScriptStruct_ZoneOccupationTerrain* v1; // [esp+4h] [ebp-44h]
    tAI* v2; // [esp+Ch] [ebp-3Ch]
    tAI* v3; // [esp+10h] [ebp-38h]
    tGameObjectMain* v4; // [esp+14h] [ebp-34h]
    tGameObjectMain* v5; // [esp+18h] [ebp-30h]
    tAI* v6; // [esp+1Ch] [ebp-2Ch]
    tAI* v7; // [esp+20h] [ebp-28h]
    tGameObjectMain* v8; // [esp+24h] [ebp-24h]
    tGameObjectMain* v9; // [esp+28h] [ebp-20h]
    tAI* v10; // [esp+2Ch] [ebp-1Ch]
    tAI* v11; // [esp+30h] [ebp-18h]
    tGameObjectMain* v12; // [esp+34h] [ebp-14h]
    tGameObjectMain* v13; // [esp+38h] [ebp-10h]
    int i; // [esp+44h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ZoneOccupationTerrain*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v1->i_transfered = 1;
    v11 = (tAI*)v1->o_Actor_Test;
    if (v11 == WOR_gst_Universe.univAIInstance)
    {
        v10 = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        v13 = (tGameObjectMain*)v1->o_Actor_Test;
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
    ((tScriptStruct_Companion*)(v10->instanceVarDataCurrent))->i_OT_nbWps = v1->des_i_nbWps;
    for (i = 0; i < 5; ++i)
    {
        v7 = (tAI*)v1->o_Actor_Test;
        if (v7 == WOR_gst_Universe.univAIInstance)
        {
            v6 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v9 = (tGameObjectMain*)v1->o_Actor_Test;
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
        ((tScriptStruct_Companion*)(v6->instanceVarDataCurrent))->o_OT_tabWps[i] = v1->des_o_tabWps[i];
    }
    v3 = (tAI*)v1->o_Actor_Test;
    if (v3 == WOR_gst_Universe.univAIInstance)
    {
        v2 = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        v5 = (tGameObjectMain*)v1->o_Actor_Test;
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
    ((tScriptStruct_Companion*)(v2->instanceVarDataCurrent))->o_OT_tabWps[5] = (int)AI_gpst_CurrentGameObject;
    return 1000;
}

int Zone_OccupationTerrain_zo_dans_la_place()
{
    int v1; // eax
    tScriptStruct_ZoneOccupationTerrain* v2; // [esp+0h] [ebp-1Ch]
    tGameObjectMain* v3; // [esp+8h] [ebp-14h]
    int v4; // [esp+10h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ZoneOccupationTerrain*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Zone_OccupationTerrain_zo_dans_la_place_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Zone_OccupationTerrain_zo_dans_la_place_univVars_Perso_ID_Companion,
            (void*)Zone_OccupationTerrain_zo_dans_la_place_univVars,
            1040);
        AI2C_ResolveBuffer(
            &Zone_OccupationTerrain_zo_dans_la_place_univVars_Perso_ID_status,
            (void*)Zone_OccupationTerrain_zo_dans_la_place_univVars,
            484);
        fct_Zone_OccupationTerrain_zo_dans_la_place_Zone_OccupationTerrain_zo_exec_clear = (int)AI_ul_RealResolveOneRefFunction(0x7100F59Cu);
        fct_Zone_OccupationTerrain_zo_dans_la_place_Zone_OccupationTerrain_zo_dehors = (int)AI_ul_RealResolveOneRefFunction(0x9E002535);
    }
    if (v2->Mode)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    if (*(uint32_t*)Zone_OccupationTerrain_zo_dans_la_place_univVars_Perso_ID_Companion == -1
        || *(int*)Zone_OccupationTerrain_zo_dans_la_place_univVars_Perso_ID_Companion >= 5)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    if (v2->i_capa_out
        && *(uint32_t*)(Zone_OccupationTerrain_zo_dans_la_place_univVars_Perso_ID_status + 4 * v2->i_Actor_Test + 4) != 4)
    {
        goto LABEL_20;
    }
    if (!v2->i_capa_out)
    {
        v3 = *AI_MainActor;
        if (*AI_MainActor)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            v1 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, v3);
        }
        else
        {
            v1 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, 0);
        }
        if (!v1)
        {
        LABEL_20:
            v4 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Zone_OccupationTerrain_zo_dans_la_place_Zone_OccupationTerrain_zo_exec_clear);
            if (v4 != 1000)
                return v4;
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Zone_OccupationTerrain_zo_dans_la_place_Zone_OccupationTerrain_zo_dehors);
        }
    }
    return 1000;
}

int Zone_OccupationTerrain_Zo_reflex()
{
    int result; // eax
    tScriptStruct_ZoneOccupationTerrain* v1; // [esp+0h] [ebp-24h]
    int v2; // [esp+Ch] [ebp-18h]
    int v3; // [esp+14h] [ebp-10h]
    int v4; // [esp+20h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ZoneOccupationTerrain*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Zone_OccupationTerrain_Zo_reflex_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Zone_OccupationTerrain_Zo_reflex_univVars_Flag_speciaux_sauve,
            (void*)Zone_OccupationTerrain_Zo_reflex_univVars,
            4488);
        fct_Zone_OccupationTerrain_Zo_reflex_Zone_OccupationTerrain_zo_exec_clear = (int)AI_ul_RealResolveOneRefFunction(0x7100F59Cu);
    }
    if (!v1->SF_Desactivation)
        goto LABEL_18;
    if (v1->SF_Desactivation <= 0)
        v4 = 0;
    else
        v4 = v1->SF_Desactivation < 10000 ? AI_EvalFunc_ARRBitTest_C(
            (int*)(Zone_OccupationTerrain_Zo_reflex_univVars_Flag_speciaux_sauve + 4),
            v1->SF_Desactivation) : 1
        - AI_EvalFunc_ARRBitTest_C(
            (int*)(Zone_OccupationTerrain_Zo_reflex_univVars_Flag_speciaux_sauve
                + 4),
            v1->SF_Desactivation - 10000);
    if (!v4)
        goto LABEL_18;
    if (v1->i_transfered)
    {
        v3 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Zone_OccupationTerrain_Zo_reflex_Zone_OccupationTerrain_zo_exec_clear);
        if (v3 != 1000)
            return v3;
    }
    v2 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
    if (v2 != 1000)
        result = v2;
    else
        LABEL_18:
    result = 1000;
    return result;
}

int Zone_OccupationTerrain_zo_exec_clear()
{
    tScriptStruct_ZoneOccupationTerrain* v1; // [esp+0h] [ebp-50h]
    tAI* v2; // [esp+8h] [ebp-48h]
    tAI* v3; // [esp+Ch] [ebp-44h]
    tGameObjectMain* v4; // [esp+10h] [ebp-40h]
    tGameObjectMain* v5; // [esp+14h] [ebp-3Ch]
    tAI* v6; // [esp+18h] [ebp-38h]
    tAI* v7; // [esp+1Ch] [ebp-34h]
    tGameObjectMain* v8; // [esp+20h] [ebp-30h]
    tGameObjectMain* v9; // [esp+24h] [ebp-2Ch]
    tAI* v10; // [esp+28h] [ebp-28h]
    tAI* v11; // [esp+2Ch] [ebp-24h]
    tGameObjectMain* v12; // [esp+30h] [ebp-20h]
    tGameObjectMain* v13; // [esp+34h] [ebp-1Ch]
    tAI* v14; // [esp+38h] [ebp-18h]
    tAI* v15; // [esp+3Ch] [ebp-14h]
    tGameObjectMain* v16; // [esp+40h] [ebp-10h]
    tGameObjectMain* v17; // [esp+44h] [ebp-Ch]
    int i; // [esp+4Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ZoneOccupationTerrain*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v1->i_transfered = 0;
    v15 = (tAI*)v1->o_Actor_Test;
    if (v15 == WOR_gst_Universe.univAIInstance)
    {
        v14 = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        v17 = (tGameObjectMain*)v1->o_Actor_Test;
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
    ((tScriptStruct_Companion*)(v14->instanceVarDataCurrent))->i_OT_nbWps = 0;
    v11 = (tAI*)v1->o_Actor_Test;
    if (v11 == WOR_gst_Universe.univAIInstance)
    {
        v10 = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        v13 = (tGameObjectMain*)v1->o_Actor_Test;
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
    ((tScriptStruct_Companion*)(v10->instanceVarDataCurrent))->i_OT_force_walk = 0;
    for (i = 0; i < 5; ++i)
    {
        v7 = (tAI*)v1->o_Actor_Test;
        if (v7 == WOR_gst_Universe.univAIInstance)
        {
            v6 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v9 = (tGameObjectMain*)v1->o_Actor_Test;
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
        ((tScriptStruct_Companion*)(v6->instanceVarDataCurrent))->o_OT_tabWps[i] = 0;
    }
    v3 = (tAI*)v1->o_Actor_Test;
    if (v3 == WOR_gst_Universe.univAIInstance)
    {
        v2 = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        v5 = (tGameObjectMain*)v1->o_Actor_Test;
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
    ((tScriptStruct_Companion*)(v2->instanceVarDataCurrent))->o_OT_tabWps[5] = 0;
    return 1000;
}

// ===================================

int Zone_ChangeStatus_MultiCompagnon_zcsmc_init()
{
    tScriptStruct_ZoneChangeStatusMultiCompanion* v1; // [esp+0h] [ebp-1Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ZoneChangeStatusMultiCompanion*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Zone_ChangeStatus_MultiCompagnon_zcsmc_init_Zone_ChangeStatus_MultiCompagnon_zcsmc_wait = (int)AI_ul_RealResolveOneRefFunction(0x9E003D8B);
    }
    v1->ai_compagnon_qui_change_de_st[0] = v1->i_Peyj_change_status;
    v1->ai_compagnon_qui_change_de_st[1] = v1->i_Mili_change_status;
    v1->ai_compagnon_qui_change_de_st[2] = v1->i_Domz_change_status;
    v1->ai_compagnon_qui_change_de_st[3] = v1->i_XB25_change_status;
    v1->ai_compagnon_qui_change_de_st[4] = v1->i_XB26_change_status;
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Zone_ChangeStatus_MultiCompagnon_zcsmc_init_Zone_ChangeStatus_MultiCompagnon_zcsmc_wait);
    return 1000;
}

int Zone_ChangeStatus_MultiCompagnon_zcsmc_wait()
{
    BOOL v0; // eax
    unsigned int v1; // eax
    tScriptStruct_ZoneChangeStatusMultiCompanion* v3; // [esp+0h] [ebp-108h]
    tAI* v4; // [esp+4h] [ebp-104h]
    tAI* v5; // [esp+8h] [ebp-100h]
    tGameObjectMain* v6; // [esp+Ch] [ebp-FCh]
    tGameObjectMain* v7; // [esp+10h] [ebp-F8h]
    tAI* v8; // [esp+14h] [ebp-F4h]
    tGameObjectMain* v9; // [esp+18h] [ebp-F0h]
    tGameObjectMain* v10; // [esp+1Ch] [ebp-ECh]
    tAI* v11; // [esp+20h] [ebp-E8h]
    tGameObjectMain* v12; // [esp+24h] [ebp-E4h]
    tGameObjectMain* v13; // [esp+28h] [ebp-E0h]
    tGameObjectMain* v14; // [esp+2Ch] [ebp-DCh]
    tGameObjectMain* v15; // [esp+30h] [ebp-D8h]
    tAI* v16; // [esp+34h] [ebp-D4h]
    tGameObjectMain* v17; // [esp+38h] [ebp-D0h]
    tGameObjectMain* v18; // [esp+3Ch] [ebp-CCh]
    tAI* v19; // [esp+40h] [ebp-C8h]
    tGameObjectMain* v20; // [esp+44h] [ebp-C4h]
    tGameObjectMain* v21; // [esp+48h] [ebp-C0h]
    tGameObjectMain* v22; // [esp+50h] [ebp-B8h]
    tGameObjectMain* v23; // [esp+54h] [ebp-B4h]
    tGameObjectMain* v24; // [esp+58h] [ebp-B0h]
    tGameObjectMain* v25; // [esp+60h] [ebp-A8h]
    tGameObjectMain* v26; // [esp+68h] [ebp-A0h]
    tGameObjectMain* v27; // [esp+70h] [ebp-98h]
    tGameObjectMain* v28; // [esp+74h] [ebp-94h]
    tGameObjectMain* v29; // [esp+78h] [ebp-90h]
    tGameObjectMain* v30; // [esp+7Ch] [ebp-8Ch]
    tGameObjectMain* v31; // [esp+80h] [ebp-88h]
    tGameObjectMain* v32; // [esp+88h] [ebp-80h]
    tMessage Src; // [esp+A8h] [ebp-60h] BYREF
    unsigned int v34; // [esp+DCh] [ebp-2Ch]
    int i; // [esp+F8h] [ebp-10h]
    tScriptStruct_ZoneChangeStatusMultiCompanion* v36; // [esp+FCh] [ebp-Ch]
    int v37; // [esp+100h] [ebp-8h]
    tAI* v38; // [esp+104h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_ZoneChangeStatusMultiCompanion*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v36 = v3;
    i = 0;
    v38 = 0;
    v37 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Zone_ChangeStatus_MultiCompagnon_zcsmc_wait__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Zone_ChangeStatus_MultiCompagnon_zcsmc_wait__univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Zone_ChangeStatus_MultiCompagnon_zcsmc_wait__univVars_Perso_ID_actor,
            (void*)Zone_ChangeStatus_MultiCompagnon_zcsmc_wait__univVars,
            988);
        AI2C_ResolveBuffer(
            &Zone_ChangeStatus_MultiCompagnon_zcsmc_wait__univVars_Perso_ID_status,
            (void*)Zone_ChangeStatus_MultiCompagnon_zcsmc_wait__univVars,
            484);
    }
    v32 = (tGameObjectMain*)Zone_ChangeStatus_MultiCompagnon_zcsmc_wait__PJ_Sally_gao;
    if (Zone_ChangeStatus_MultiCompagnon_zcsmc_wait__PJ_Sally_gao)
    {
        if ((unsigned int)Zone_ChangeStatus_MultiCompagnon_zcsmc_wait__PJ_Sally_gao <= 2)
            v32 = AI_MainActor[Zone_ChangeStatus_MultiCompagnon_zcsmc_wait__PJ_Sally_gao - 1];
        v0 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, v32);
    }
    else
    {
        v0 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, 0);
    }
    if (v0)
    {
        for (i = 0; i < 5; ++i)
        {
            v38 = *(tAI**)(Zone_ChangeStatus_MultiCompagnon_zcsmc_wait__univVars_Perso_ID_actor + 4 * i + 4);
            if (v36->ai_compagnon_qui_change_de_st[i])
            {
                v31 = (tGameObjectMain*)v38;
                if (v38)
                {
                    if ((unsigned int)v38 <= 2)
                        v31 = AI_MainActor[(uint32_t)v38 - 1];
                    v30 = v31;
                }
                else
                {
                    v30 = 0;
                }
                if (v30)
                {
                    if ((v37 = *(uint32_t*)(Zone_ChangeStatus_MultiCompagnon_zcsmc_wait__univVars_Perso_ID_status + 4 * i + 4),
                        v37 == 4)
                        && !v36->i_compagnon_status_objectif
                        || !v37 && v36->i_compagnon_status_objectif == 4)
                    {
                        Src.msgID = 49;
                        Src.Int1 = v36->i_compagnon_status_objectif;
                        Src.Int2 = 0;
                        Src.f2 = 0.0;
                        Src.f1 = 0.0;
                        v29 = (tGameObjectMain*)v36->des_o_pos_reinit_perso[i];
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
                        if (v28)
                        {
                            v27 = (tGameObjectMain*)v36->des_o_pos_reinit_perso[i];
                            if (v27)
                            {
                                if ((unsigned int)v27 <= 2)
                                    v27 = AI_MainActor[(uint32_t)v27 - 1];
                                AI_EvalFunc_OBJGetPos_C(v27, &Src.v1);
                            }
                            else
                            {
                                AI_EvalFunc_OBJGetPos_C(0, &Src.v1);
                            }
                            v26 = (tGameObjectMain*)v36->des_o_pos_reinit_perso[i];
                            if (v26)
                            {
                                if ((unsigned int)v26 <= 2)
                                    v26 = AI_MainActor[(uint32_t)v26 - 1];
                                AI_EvalFunc_OBJ_SightGet_C(v26, &Src.v2);
                            }
                            else
                            {
                                AI_EvalFunc_OBJ_SightGet_C(0, &Src.v2);
                            }
                        }
                        else
                        {
                            Src.v1.x = 0.0;
                            Src.v1.y = 0.0;
                            Src.v1.z = 0.0;
                            Src.v2.x = 0.0;
                            Src.v2.y = 0.0;
                            Src.v2.z = 0.0;
                        }
                        v25 = (tGameObjectMain*)v38;
                        if (v38)
                        {
                            if ((unsigned int)v38 <= 2)
                                v25 = AI_MainActor[(uint32_t)v38 - 1];
                            AI_EvalFunc_MSGSend_C(v25, &Src);
                        }
                        else
                        {
                            AI_EvalFunc_MSGSend_C(0, &Src);
                        }
                        if (v36->i_compagnon_status_objectif != 4 || v37 == 4)
                        {
                            if (!v36->i_compagnon_status_objectif && v37)
                            {
                                v15 = (tGameObjectMain*)v36->OT_a_activer_en_attente[i];
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
                                    if (v38 == WOR_gst_Universe.univAIInstance)
                                    {
                                        v11 = WOR_gst_Universe.univAIInstance;
                                    }
                                    else
                                    {
                                        v13 = (tGameObjectMain*)v38;
                                        if (v38)
                                        {
                                            if ((unsigned int)v38 <= 2)
                                                v13 = AI_MainActor[(uint32_t)v38 - 1];
                                            v12 = v13;
                                        }
                                        else
                                        {
                                            v12 = 0;
                                        }
                                        v11 = v12->extended->AIInstance;
                                    }
                                    ((tScriptStruct_Companion*)(v11->instanceVarDataCurrent))->i_OT_nbWps = 0;
                                    for (i = 0; i < 5; ++i)
                                    {
                                        if (v36->ai_compagnon_qui_change_de_st[i])
                                        {
                                            if (v38 == WOR_gst_Universe.univAIInstance)
                                            {
                                                v8 = WOR_gst_Universe.univAIInstance;
                                            }
                                            else
                                            {
                                                v10 = (tGameObjectMain*)v38;
                                                if (v38)
                                                {
                                                    if ((unsigned int)v38 <= 2)
                                                        v10 = AI_MainActor[(uint32_t)v38 - 1];
                                                    v9 = v10;
                                                }
                                                else
                                                {
                                                    v9 = 0;
                                                }
                                                v8 = v9->extended->AIInstance;
                                            }
                                            ((tScriptStruct_Companion*)(v8->instanceVarDataCurrent))->o_OT_tabWps[i] = 0;
                                            v5 = (tAI*)v36->OT_a_activer_en_attente[i];
                                            if (v5 == WOR_gst_Universe.univAIInstance)
                                            {
                                                v4 = WOR_gst_Universe.univAIInstance;
                                            }
                                            else
                                            {
                                                v7 = (tGameObjectMain*)v36->OT_a_activer_en_attente[i];
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
                                            ((tScriptStruct_Companion*)(v4->instanceVarDataCurrent))->ao_way_wp_list[1] = 1;
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            v24 = (tGameObjectMain*)v36->OT_a_activer_en_attente[i];
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
                            if (v23)
                            {
                                v22 = (tGameObjectMain*)v36->OT_a_activer_en_attente[i];
                                if (v22)
                                {
                                    if ((unsigned int)v22 <= 2)
                                        v22 = AI_MainActor[(uint32_t)v22 - 1];
                                    v1 = AI_EvalFunc_OBJ_FlagsControlSet_C(v22, 0, 0x600000);
                                }
                                else
                                {
                                    v1 = AI_EvalFunc_OBJ_FlagsControlSet_C(0, 0, 0x600000);
                                }
                                v34 = v1;
                            }
                            else
                            {
                                if (v38 == WOR_gst_Universe.univAIInstance)
                                {
                                    v19 = WOR_gst_Universe.univAIInstance;
                                }
                                else
                                {
                                    v21 = (tGameObjectMain*)v38;
                                    if (v38)
                                    {
                                        if ((unsigned int)v38 <= 2)
                                            v21 = AI_MainActor[(uint32_t)v38 - 1];
                                        v20 = v21;
                                    }
                                    else
                                    {
                                        v20 = 0;
                                    }
                                    v19 = v20->extended->AIInstance;
                                }
                                ((tScriptStruct_Companion*)(v19->instanceVarDataCurrent))->i_OT_nbWps = 0;
                                for (i = 0; i < 5; ++i)
                                {
                                    if (v38 == WOR_gst_Universe.univAIInstance)
                                    {
                                        v16 = WOR_gst_Universe.univAIInstance;
                                    }
                                    else
                                    {
                                        v18 = (tGameObjectMain*)v38;
                                        if (v38)
                                        {
                                            if ((unsigned int)v38 <= 2)
                                                v18 = AI_MainActor[(uint32_t)v38 - 1];
                                            v17 = v18;
                                        }
                                        else
                                        {
                                            v17 = 0;
                                        }
                                        v16 = v17->extended->AIInstance;
                                    }
                                    ((tScriptStruct_Companion*)(v16->instanceVarDataCurrent))->o_OT_tabWps[i] = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 1000;
}

// ==============================

int Zone_ChangeStatus_zcs_init()
{
    tScriptStruct_ZoneChangeStatus* v1; // [esp+4h] [ebp-10h]
    tGameObjectMain* v2; // [esp+8h] [ebp-Ch]
    tGameObjectMain* v3; // [esp+Ch] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ZoneChangeStatus*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Zone_ChangeStatus_zcs_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Zone_ChangeStatus_zcs_init_univVars_Perso_ID_Companion,
            (void*)Zone_ChangeStatus_zcs_init_univVars,
            1040);
        AI2C_ResolveBuffer(
            &Zone_ChangeStatus_zcs_init_univVars_Perso_ID_actor,
            (void*)Zone_ChangeStatus_zcs_init_univVars,
            988);
        fct_Zone_ChangeStatus_zcs_init_Zone_ChangeStatus_zcs_wait = (int)AI_ul_RealResolveOneRefFunction(0x9E002F71);
    }
    if (*AI_gppst_CurrentJumpNode)
    {
        *AI_gppst_CurrentJumpNode = 0;
        TIME_Wait_C(0.000001f);
        return 0;
    }
    if (v1->des_companion_status_objectif)
    {
        if (v1->des_companion_status_objectif == 1)
            v1->des_companion_status_objectif = 4;
    }
    else
    {
        v1->des_companion_status_objectif = 0;
    }
    if (v1->des_companion_ID != -1 || *(uint32_t*)Zone_ChangeStatus_zcs_init_univVars_Perso_ID_Companion == -1)
    {
        if (v1->des_companion_ID == -1)
            goto LABEL_24;
        v3 = *(tGameObjectMain**)(Zone_ChangeStatus_zcs_init_univVars_Perso_ID_actor + 4 * v1->des_companion_ID + 4);
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
        if (!v2)
        {
        LABEL_24:
            v1->i_ind_compagnon = -1;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v1->i_ind_compagnon = v1->des_companion_ID;
    }
    else
    {
        v1->i_ind_compagnon = *(uint32_t*)Zone_ChangeStatus_zcs_init_univVars_Perso_ID_Companion;
        if (v1->i_ind_compagnon >= 5)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
    }
    v1->o_compagnon = *(uint32_t*)(Zone_ChangeStatus_zcs_init_univVars_Perso_ID_actor + 4 * v1->i_ind_compagnon + 4);
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Zone_ChangeStatus_zcs_init_Zone_ChangeStatus_zcs_wait);
    return 1000;
}

int Zone_ChangeStatus_zcs_wait()
{
    BOOL v1; // eax
    tScriptStruct_ZoneChangeStatus* v2; // [esp+0h] [ebp-13Ch]
    tAI* v3; // [esp+4h] [ebp-138h]
    tAI* v4; // [esp+8h] [ebp-134h]
    tGameObjectMain* v5; // [esp+Ch] [ebp-130h]
    tGameObjectMain* v6; // [esp+10h] [ebp-12Ch]
    tAI* v7; // [esp+14h] [ebp-128h]
    tAI* v8; // [esp+18h] [ebp-124h]
    tGameObjectMain* v9; // [esp+1Ch] [ebp-120h]
    tGameObjectMain* v10; // [esp+20h] [ebp-11Ch]
    tAI* v11; // [esp+24h] [ebp-118h]
    tAI* v12; // [esp+28h] [ebp-114h]
    tGameObjectMain* v13; // [esp+2Ch] [ebp-110h]
    tGameObjectMain* v14; // [esp+30h] [ebp-10Ch]
    tGameObjectMain* v15; // [esp+34h] [ebp-108h]
    tGameObjectMain* v16; // [esp+38h] [ebp-104h]
    tAI* v17; // [esp+3Ch] [ebp-100h]
    tAI* v18; // [esp+40h] [ebp-FCh]
    tGameObjectMain* v19; // [esp+44h] [ebp-F8h]
    tGameObjectMain* v20; // [esp+48h] [ebp-F4h]
    tAI* v21; // [esp+4Ch] [ebp-F0h]
    tAI* v22; // [esp+50h] [ebp-ECh]
    tGameObjectMain* v23; // [esp+54h] [ebp-E8h]
    tGameObjectMain* v24; // [esp+58h] [ebp-E4h]
    tGameObjectMain* v25; // [esp+60h] [ebp-DCh]
    tGameObjectMain* v26; // [esp+64h] [ebp-D8h]
    tGameObjectMain* v27; // [esp+68h] [ebp-D4h]
    tGameObjectMain* v28; // [esp+70h] [ebp-CCh]
    tGameObjectMain* v29; // [esp+78h] [ebp-C4h]
    tGameObjectMain* v30; // [esp+80h] [ebp-BCh]
    tGameObjectMain* v31; // [esp+84h] [ebp-B8h]
    tGameObjectMain* v32; // [esp+88h] [ebp-B4h]
    tGameObjectMain* v33; // [esp+90h] [ebp-ACh]
    tMessage Src; // [esp+C0h] [ebp-7Ch] BYREF
    BOOL v35; // [esp+F4h] [ebp-48h]
    int v36; // [esp+F8h] [ebp-44h]
    int v37; // [esp+108h] [ebp-34h]
    int i; // [esp+10Ch] [ebp-30h]
    tScriptStruct_ZoneChangeStatus* v39; // [esp+114h] [ebp-28h]
    int v40; // [esp+118h] [ebp-24h]
    int v41; // [esp+134h] [ebp-8h]
    int v42; // [esp+138h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ZoneChangeStatus*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v39 = v2;
    v41 = 0;
    v42 = 0;
    i = 0;
    v40 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Zone_ChangeStatus_zcs_wait_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Zone_ChangeStatus_zcs_wait_univVars_Flag_speciaux_sauve,
            (void*)Zone_ChangeStatus_zcs_wait_univVars,
            4488);
        Zone_ChangeStatus_zcs_wait__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(
            &Zone_ChangeStatus_zcs_wait_univVars_Perso_ID_status,
            (void*)Zone_ChangeStatus_zcs_wait_univVars,
            484);
        fct_Zone_ChangeStatus_zcs_wait_Zone_ChangeStatus_zcs_exec_texte = (int)AI_ul_RealResolveOneRefFunction(0x9E002FB1);
    }
    if (v39->SF_destruction != -1)
    {
        if (v39->SF_destruction <= 0)
            v42 = 0;
        else
            v42 = v39->SF_destruction < 10000 ? AI_EvalFunc_ARRBitTest_C(
                (int*)(Zone_ChangeStatus_zcs_wait_univVars_Flag_speciaux_sauve + 4),
                v39->SF_destruction) : 1
            - AI_EvalFunc_ARRBitTest_C(
                (int*)(Zone_ChangeStatus_zcs_wait_univVars_Flag_speciaux_sauve
                    + 4),
                v39->SF_destruction - 10000);
        if (v42)
        {
            v37 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v37 != 1000)
                return v37;
        }
    }
    v41 = 1;
    if (v39->SF_activation != -1)
    {
        if (v39->SF_activation <= 0)
            v42 = 0;
        else
            v42 = v39->SF_activation < 10000 ? AI_EvalFunc_ARRBitTest_C(
                (int*)(Zone_ChangeStatus_zcs_wait_univVars_Flag_speciaux_sauve + 4),
                v39->SF_activation) : 1
            - AI_EvalFunc_ARRBitTest_C(
                (int*)(Zone_ChangeStatus_zcs_wait_univVars_Flag_speciaux_sauve
                    + 4),
                v39->SF_activation - 10000);
        if (!v42)
            v41 = 0;
    }
    v33 = (tGameObjectMain*)Zone_ChangeStatus_zcs_wait__PJ_Sally_gao;
    if (Zone_ChangeStatus_zcs_wait__PJ_Sally_gao)
    {
        if ((unsigned int)Zone_ChangeStatus_zcs_wait__PJ_Sally_gao <= 2)
            v33 = AI_MainActor[Zone_ChangeStatus_zcs_wait__PJ_Sally_gao - 1];
        v1 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, v33);
    }
    else
    {
        v1 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, 0);
    }
    v35 = v1;
    if (v1 && v41)
    {
        if ((v40 = *(uint32_t*)(Zone_ChangeStatus_zcs_wait_univVars_Perso_ID_status + 4 * v39->i_ind_compagnon + 4), v40 == 4)
            && !v39->des_companion_status_objectif
            || !v40 && v39->des_companion_status_objectif == 4)
        {
            Src.msgID = 49;
            Src.Int1 = v39->des_companion_status_objectif;
            Src.Int2 = 0;
            Src.f2 = 0.0;
            Src.f1 = 0.0;
            v32 = (tGameObjectMain*)v39->des_o_pos_reinit_perso;
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
                v30 = (tGameObjectMain*)v39->des_o_pos_reinit_perso;
                if (v30)
                {
                    if ((unsigned int)v30 <= 2)
                        v30 = AI_MainActor[(uint32_t)v30 - 1];
                    AI_EvalFunc_OBJGetPos_C(v30, &Src.v1);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &Src.v1);
                }
                v29 = (tGameObjectMain*)v39->des_o_pos_reinit_perso;
                if (v29)
                {
                    if ((unsigned int)v29 <= 2)
                        v29 = AI_MainActor[(uint32_t)v29 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v29, &Src.v2);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &Src.v2);
                }
            }
            else
            {
                Src.v1.x = 0.0;
                Src.v1.y = 0.0;
                Src.v1.z = 0.0;
                Src.v2.x = 0.0;
                Src.v2.y = 0.0;
                Src.v2.z = 0.0;
            }
            v28 = (tGameObjectMain*)v39->o_compagnon;
            if (v28)
            {
                if ((unsigned int)v28 <= 2)
                    v28 = AI_MainActor[(uint32_t)v28 - 1];
                AI_EvalFunc_MSGSend_C(v28, &Src);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &Src);
            }
            if (v39->des_companion_status_objectif != 4 || v40 == 4)
            {
                if (!v39->des_companion_status_objectif && v40)
                {
                    v16 = (tGameObjectMain*)v39->des_OT_a_activer_en_attente;
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
                        v12 = (tAI*)v39->o_compagnon;
                        if (v12 == WOR_gst_Universe.univAIInstance)
                        {
                            v11 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v14 = (tGameObjectMain*)v39->o_compagnon;
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
                        ((tScriptStruct_Companion*)(v11->instanceVarDataCurrent))->i_OT_nbWps = 0;
                        for (i = 0; i < 5; ++i)
                        {
                            v8 = (tAI*)v39->o_compagnon;
                            if (v8 == WOR_gst_Universe.univAIInstance)
                            {
                                v7 = WOR_gst_Universe.univAIInstance;
                            }
                            else
                            {
                                v10 = (tGameObjectMain*)v39->o_compagnon;
                                if (v8)
                                {
                                    if ((unsigned int)v8 <= 2)
                                        v10 = AI_MainActor[(uint32_t)v8 - 1];
                                    v9 = v10;
                                }
                                else
                                {
                                    v9 = 0;
                                }
                                v7 = v9->extended->AIInstance;
                            }
                            ((tScriptStruct_Companion*)(v7->instanceVarDataCurrent))->o_OT_tabWps[i] = 0;
                        }
                        v4 = (tAI*)v39->des_OT_a_activer_en_attente;
                        if (v4 == WOR_gst_Universe.univAIInstance)
                        {
                            v3 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v6 = (tGameObjectMain*)v39->des_OT_a_activer_en_attente;
                            if (v4)
                            {
                                if ((unsigned int)v4 <= 2)
                                    v6 = AI_MainActor[(uint32_t)v4 - 1];
                                v5 = v6;
                            }
                            else
                            {
                                v5 = 0;
                            }
                            v3 = v5->extended->AIInstance;
                        }
                        ((tScriptStruct_Companion*)(v3->instanceVarDataCurrent))->ao_way_wp_list[1] = 1;
                    }
                }
            }
            else
            {
                v27 = (tGameObjectMain*)v39->des_OT_a_activer_en_attente;
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
                if (v26)
                {
                    v25 = (tGameObjectMain*)v39->des_OT_a_activer_en_attente;
                    if (v25)
                    {
                        if ((unsigned int)v25 <= 2)
                            v25 = AI_MainActor[(uint32_t)v25 - 1];
                        AI_EvalFunc_OBJ_FlagsControlSet_C(v25, 0, 0x600000);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagsControlSet_C(0, 0, 0x600000);
                    }
                }
                else
                {
                    v22 = (tAI*)v39->o_compagnon;
                    if (v22 == WOR_gst_Universe.univAIInstance)
                    {
                        v21 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v24 = (tGameObjectMain*)v39->o_compagnon;
                        if (v22)
                        {
                            if ((unsigned int)v22 <= 2)
                                v24 = AI_MainActor[(uint32_t)v22 - 1];
                            v23 = v24;
                        }
                        else
                        {
                            v23 = 0;
                        }
                        v21 = v23->extended->AIInstance;
                    }
                    ((tScriptStruct_Companion*)(v21->instanceVarDataCurrent))->i_OT_nbWps = 0;
                    for (i = 0; i < 5; ++i)
                    {
                        v18 = (tAI*)v39->o_compagnon;
                        if (v18 == WOR_gst_Universe.univAIInstance)
                        {
                            v17 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v20 = (tGameObjectMain*)v39->o_compagnon;
                            if (v18)
                            {
                                if ((unsigned int)v18 <= 2)
                                    v20 = AI_MainActor[(uint32_t)v18 - 1];
                                v19 = v20;
                            }
                            else
                            {
                                v19 = 0;
                            }
                            v17 = v19->extended->AIInstance;
                        }
                        ((tScriptStruct_Companion*)(v17->instanceVarDataCurrent))->o_OT_tabWps[i] = 0;
                    }
                }
                if (v39->des_i_affiche_texte)
                {
                    v36 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Zone_ChangeStatus_zcs_wait_Zone_ChangeStatus_zcs_exec_texte);
                    if (v36 != 1000)
                        return v36;
                }
            }
        }
    }
    return 1000;
}

int Zone_ChangeStatus_zcs_exec_texte()
{
    char* v0; // eax
    tScriptStruct_ZoneChangeStatus* v2; // [esp+0h] [ebp-84h]
    tGameObjectMain* v3; // [esp+8h] [ebp-7Ch]
    tAI* v4; // [esp+Ch] [ebp-78h]
    tGameObjectMain* v5; // [esp+10h] [ebp-74h]
    tGameObjectMain* v6; // [esp+14h] [ebp-70h]
    tAI* v7; // [esp+18h] [ebp-6Ch]
    tAI* v8; // [esp+1Ch] [ebp-68h]
    tGameObjectMain* v9; // [esp+20h] [ebp-64h]
    tGameObjectMain* v10; // [esp+24h] [ebp-60h]
    tAI* v11; // [esp+28h] [ebp-5Ch]
    char Src[20]; // [esp+2Ch] [ebp-58h] BYREF
    tMessage v13; // [esp+40h] [ebp-44h] BYREF
    tScriptStruct_ZoneChangeStatus* v14; // [esp+80h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ZoneChangeStatus*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v14 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Zone_ChangeStatus_zcs_exec_texte_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Zone_ChangeStatus_zcs_exec_texte_univVars_Perso_ID_Companion,
            (void*)Zone_ChangeStatus_zcs_exec_texte_univVars,
            1040);
    }
    v8 = (tAI*)v14->o_compagnon;
    if (v8 == WOR_gst_Universe.univAIInstance)
    {
        v7 = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        v10 = (tGameObjectMain*)v14->o_compagnon;
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v10 = AI_MainActor[(uint32_t)v8 - 1];
            v9 = v10;
        }
        else
        {
            v9 = 0;
        }
        v7 = v9->extended->AIInstance;
    }
    v11 = (tAI*)((tScriptStruct_Companion*)(v7->instanceVarDataCurrent))->des_o_acteur_text;
    if (*(uint32_t*)Zone_ChangeStatus_zcs_exec_texte_univVars_Perso_ID_Companion)
    {
        if (*(uint32_t*)Zone_ChangeStatus_zcs_exec_texte_univVars_Perso_ID_Companion == 1)
            memcpy(Src, &v14->des_t_texte_attente_mili, 8u);
    }
    else
    {
        memcpy(Src, &v14->des_t_texte_attente, 8u);
    }
    if (v11 == WOR_gst_Universe.univAIInstance)
    {
        v4 = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        v6 = (tGameObjectMain*)v11;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v6 = AI_MainActor[(uint32_t)v11 - 1];
            v5 = v6;
        }
        else
        {
            v5 = 0;
        }
        v4 = v5->extended->AIInstance;
    }
    v0 = AI_EvalVarArray_C(10, (char*)v4->instanceVarDataCurrent, 63);
    memcpy(v0, Src, 8u);
    v13.msgID = 30;
    v13.Int1 = 63;
    v13.Int2 = 0;
    v13.f2 = 3.0;
    v13.v1.x = 0.0;
    v13.v1.y = 0.0;
    v13.v1.z = 0.0;
    v13.v2.x = 0.0;
    v13.v2.y = 0.0;
    v13.v2.z = 0.0;
    v13.gao = 0;
    v13.gao2 = 0;
    v3 = (tGameObjectMain*)v11;
    if (v11)
    {
        if ((unsigned int)v11 <= 2)
            v3 = AI_MainActor[(uint32_t)v11 - 1];
        AI_EvalFunc_MSGSend_C(v3, &v13);
    }
    else
    {
        AI_EvalFunc_MSGSend_C(0, &v13);
    }
    return 1000;
}

// ==============================

int Zone_CompanionNetwork_nz_init()
{
    tScriptStruct_ZoneCompanionNetwork* v1; // [esp+0h] [ebp-Ch]
    int i; // [esp+8h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ZoneCompanionNetwork*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Zone_CompanionNetwork_nz_init_Zone_CompanionNetwork_nz_set = (int)AI_ul_RealResolveOneRefFunction(0x6C005D7Bu);
    }
    for (i = 0; i < 5; ++i)
        v1->i_annuler[i] = 0;
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Zone_CompanionNetwork_nz_init_Zone_CompanionNetwork_nz_set);
    return 1000;
}

int Zone_CompanionNetwork_nz_set()
{
    int v0; // eax
    int v1; // eax
    tScriptStruct_ZoneCompanionNetwork* v3; // [esp+0h] [ebp-68h]
    tAI* v4; // [esp+4h] [ebp-64h]
    tGameObjectMain* v5; // [esp+8h] [ebp-60h]
    tGameObjectMain* v6; // [esp+Ch] [ebp-5Ch]
    tAI* v7; // [esp+10h] [ebp-58h]
    tGameObjectMain* v8; // [esp+14h] [ebp-54h]
    tGameObjectMain* v9; // [esp+18h] [ebp-50h]
    tGameObjectMain* v10; // [esp+20h] [ebp-48h]
    tGameObjectMain* v11; // [esp+24h] [ebp-44h]
    tGameObjectMain* v12; // [esp+28h] [ebp-40h]
    tAI* v13; // [esp+2Ch] [ebp-3Ch]
    tGameObjectMain* v14; // [esp+30h] [ebp-38h]
    tGameObjectMain* v15; // [esp+34h] [ebp-34h]
    tAI* v16; // [esp+38h] [ebp-30h]
    tGameObjectMain* v17; // [esp+3Ch] [ebp-2Ch]
    tGameObjectMain* v18; // [esp+40h] [ebp-28h]
    tGameObjectMain* v19; // [esp+48h] [ebp-20h]
    tGameObjectMain* v20; // [esp+4Ch] [ebp-1Ch]
    tGameObjectMain* v21; // [esp+50h] [ebp-18h]
    tAI* v22; // [esp+54h] [ebp-14h]
    tAI* v23; // [esp+54h] [ebp-14h]
    int i; // [esp+64h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_ZoneCompanionNetwork*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Zone_CompanionNetwork_nz_set_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Zone_CompanionNetwork_nz_set_univVars_Perso_ID_actor,
            (void*)Zone_CompanionNetwork_nz_set_univVars,
            988);
    }
    if (v3->des_i_companion == -1)
    {
        for (i = 0; i < 5; ++i)
        {
            v22 = *(tAI**)(Zone_CompanionNetwork_nz_set_univVars_Perso_ID_actor + 4 * i + 4);
            v21 = (tGameObjectMain*)v22;
            if (v22)
            {
                if ((unsigned int)v22 <= 2)
                    v21 = AI_MainActor[(uint32_t)v22 - 1];
                v20 = v21;
            }
            else
            {
                v20 = 0;
            }
            if (v20)
            {
                v19 = *(tGameObjectMain**)(Zone_CompanionNetwork_nz_set_univVars_Perso_ID_actor + 4 * i + 4);
                if (v22)
                {
                    if ((unsigned int)v22 <= 2)
                        v19 = AI_MainActor[(uint32_t)v22 - 1];
                    v0 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, v19);
                }
                else
                {
                    v0 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, 0);
                }
                if (v0)
                {
                    if (v3->des_n_network_order)
                    {
                        if (v22 == WOR_gst_Universe.univAIInstance)
                        {
                            v16 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v18 = (tGameObjectMain*)v22;
                            if (v22)
                            {
                                if ((unsigned int)v22 <= 2)
                                    v18 = AI_MainActor[(uint32_t)v22 - 1];
                                v17 = v18;
                            }
                            else
                            {
                                v17 = 0;
                            }
                            v16 = v17->extended->AIInstance;
                        }
                        ((tScriptStruct_Companion*)(v16->instanceVarDataCurrent))->des_n_walk_network = v3->des_n_network_order;
                    }
                    v3->i_annuler[i] = 1;
                }
                else if (v3->i_annuler[i])
                {
                    if (v3->des_n_network_order)
                    {
                        if (v22 == WOR_gst_Universe.univAIInstance)
                        {
                            v13 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v15 = (tGameObjectMain*)v22;
                            if (v22)
                            {
                                if ((unsigned int)v22 <= 2)
                                    v15 = AI_MainActor[(uint32_t)v22 - 1];
                                v14 = v15;
                            }
                            else
                            {
                                v14 = 0;
                            }
                            v13 = v14->extended->AIInstance;
                        }
                        ((tScriptStruct_Companion*)(v13->instanceVarDataCurrent))->des_n_walk_network = 0;
                    }
                    v3->i_annuler[i] = 0;
                }
            }
        }
    }
    else
    {
        v23 = *(tAI**)(Zone_CompanionNetwork_nz_set_univVars_Perso_ID_actor + 4 * v3->des_i_companion + 4);
        v12 = (tGameObjectMain*)v23;
        if (v23)
        {
            if ((unsigned int)v23 <= 2)
                v12 = AI_MainActor[(uint32_t)v23 - 1];
            v11 = v12;
        }
        else
        {
            v11 = 0;
        }
        if (v11)
        {
            v10 = *(tGameObjectMain**)(Zone_CompanionNetwork_nz_set_univVars_Perso_ID_actor + 4 * v3->des_i_companion + 4);
            if (v23)
            {
                if ((unsigned int)v23 <= 2)
                    v10 = AI_MainActor[(uint32_t)v23 - 1];
                v1 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, v10);
            }
            else
            {
                v1 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, 0);
            }
            if (v1)
            {
                if (v3->des_n_network_order)
                {
                    if (v23 == WOR_gst_Universe.univAIInstance)
                    {
                        v7 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v9 = (tGameObjectMain*)v23;
                        if (v23)
                        {
                            if ((unsigned int)v23 <= 2)
                                v9 = AI_MainActor[(uint32_t)v23 - 1];
                            v8 = v9;
                        }
                        else
                        {
                            v8 = 0;
                        }
                        v7 = v8->extended->AIInstance;
                    }
                    ((tScriptStruct_Companion*)(v7->instanceVarDataCurrent))->des_n_walk_network = v3->des_n_network_order;
                }
                v3->i_annuler[v3->des_i_companion] = 1;
            }
            else if (v3->i_annuler[v3->des_i_companion])
            {
                if (v3->des_n_network_order)
                {
                    if (v23 == WOR_gst_Universe.univAIInstance)
                    {
                        v4 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v6 = (tGameObjectMain*)v23;
                        if (v23)
                        {
                            if ((unsigned int)v23 <= 2)
                                v6 = AI_MainActor[(uint32_t)v23 - 1];
                            v5 = v6;
                        }
                        else
                        {
                            v5 = 0;
                        }
                        v4 = v5->extended->AIInstance;
                    }
                    ((tScriptStruct_Companion*)(v4->instanceVarDataCurrent))->des_n_walk_network = 0;
                }
                v3->i_annuler[v3->des_i_companion] = 0;
            }
        }
    }
    return 1000;
}