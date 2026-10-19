#include "ai_compiledscripts_inc.h"

//PNJ_Spectre_sp_init
int PNJ_Spectre_sp_init_univVars;
int PNJ_Spectre_sp_init_univVars_Flag_speciaux_sauve;
int PNJ_Spectre_sp_init_INV_manager_gao;
int PNJ_Spectre_sp_init_INV_manager_gao_i_Animals_Nb;
int PNJ_Spectre_sp_init_INV_manager_gao_ao_Animals;
int PNJ_Spectre_sp_init_INV_manager_gao_ai_AnimalsData;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ao_Evil_Actor;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_NRJ_Courante;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_i_Evil_NbActor;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_i_Evil_NbActorAlive;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_NRJShieldMax;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_o_last_mort;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ao_Good_Actor;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ao_Evil_Target;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ai_Good_Targeted;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ai_Evil_Targeted;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ao_Good_Target;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_Dmin;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_DMax;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_Angle;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ai_Evil_Icone;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_Last_Hit;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ao_Evil_Last_Good;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_Last_Dmg;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_Last_Eff;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_Cumul_Dmg;
int PNJ_Spectre_sp_init_GST_Fight_Manager_gao_o_musique_fight;
int PNJ_Spectre_sp_init_GST_Global_gao;
int PNJ_Spectre_sp_init_GST_Global_gao_o_musique_fight_list;
int PNJ_Spectre_sp_init_univVars_Perso_ID_Companion;
int PNJ_Spectre_sp_init_univVars_Perso_ID_status;
int PNJ_Spectre_sp_init__PJ_Sally_gao;
int PNJ_Spectre_sp_init_univVars_Perso_ID_actor;
int fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_tag_off;
int fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_reflex;
int fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_tag_on;
int fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_mode_combat;
int fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_after_blend;
int fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_mode_apparition_xmen;
int fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_mode_apparition_anim;

//PNJ_Spectre_sp_mode_combat
int PNJ_Spectre_sp_mode_combat__PJ_Sally_gao;
int PNJ_Spectre_sp_mode_combat__PJ_Sally_gao_i_fight_action;
int PNJ_Spectre_sp_mode_combat_GST_Fight_Manager_gao;
int PNJ_Spectre_sp_mode_combat_GST_Fight_Manager_gao_ao_Good_Actor;

//PNJ_Spectre_sp_mode_apparition_anim
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ao_Evil_Actor;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_NRJ_Courante;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_i_Evil_NbActor;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_i_Evil_NbActorAlive;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_NRJShieldMax;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_o_last_mort;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ao_Good_Actor;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ao_Evil_Target;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ai_Good_Targeted;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ai_Evil_Targeted;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ao_Good_Target;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_Dmin;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_DMax;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_Angle;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ai_Evil_Icone;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_Last_Hit;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ao_Evil_Last_Good;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_Last_Dmg;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_Last_Eff;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_Cumul_Dmg;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_o_musique_fight;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Global_gao;
int PNJ_Spectre_sp_mode_apparition_anim_GST_Global_gao_o_musique_fight_list;
int fct_PNJ_Spectre_sp_mode_apparition_anim_PNJ_Spectre_sp_reflex;
int fct_PNJ_Spectre_sp_mode_apparition_anim_PNJ_Spectre_sp_mode_combat;
int fct_PNJ_Spectre_sp_mode_apparition_anim_PNJ_Spectre_sp_after_blend;

//PNJ_Spectre_sp_reflex
int PNJ_Spectre_sp_reflex_univVars;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao;
int PNJ_Spectre_sp_reflex_univVars_Perso_ID_Companion;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Good_Last_Paf;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ao_Evil_Actor;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_NRJ_Courante;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_i_Evil_NbActor;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_i_Evil_NbActorAlive;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_NRJShieldMax;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_o_last_mort;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ao_Good_Actor;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ao_Evil_Target;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ai_Good_Targeted;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ai_Evil_Targeted;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ao_Good_Target;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_Dmin;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_DMax;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_Angle;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ai_Evil_Icone;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_Last_Hit;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ao_Evil_Last_Good;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_Last_Dmg;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_Last_Eff;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_Cumul_Dmg;
int PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_o_musique_fight;
int PNJ_Spectre_sp_reflex_GST_Global_gao;
int PNJ_Spectre_sp_reflex_GST_Global_gao_o_musique_fight_list;
int PNJ_Spectre_sp_reflex__PJ_Sally_gao;
int PNJ_Spectre_sp_reflex__PJ_Sally_gao_current_pm_mode;
int PNJ_Spectre_sp_reflex__PJ_Sally_gao_current_pm_priority;
int PNJ_Spectre_sp_reflex__PJ_Sally_gao_i_Fight_list_nbr;
int PNJ_Spectre_sp_reflex__PJ_Sally_gao_ao_Fight_list_of_actors;
int PNJ_Spectre_sp_reflex__PJ_Sally_gao_af_Fight_list_of_distances;
int PNJ_Spectre_sp_reflex__PJ_Sally_gao_ai_Fight_list_of_bits;
int PNJ_Spectre_sp_reflex_univVars_Flag_speciaux_sauve;
int fct_PNJ_Spectre_sp_reflex_PNJ_Spectre_sp_mode_mort;
int fct_PNJ_Spectre_sp_reflex_PNJ_Spectre_sp_exec_decode_paf_univ;
int PNJ_Spectre_sp_reflex__PJ_Sally_gao_v_fight_kick_axis_non_tourne;

//PNJ_Spectre_sp_mode_mort
int PNJ_Spectre_sp_mode_mort_GST_Global_gao;
int PNJ_Spectre_sp_mode_mort_GST_Global_gao_i_sens;
int PNJ_Spectre_sp_mode_mort_SFX_light_smoke_gao;
int PNJ_Spectre_sp_mode_mort_univVars;
int PNJ_Spectre_sp_mode_mort_univVars_Flag_speciaux_sauve;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Evil_Actor;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_NRJ_Courante;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_i_Evil_NbActor;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_i_Evil_NbActorAlive;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_NRJShieldMax;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_o_last_mort;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Good_Actor;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Evil_Target;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ai_Good_Targeted;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ai_Evil_Targeted;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Good_Target;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_Dmin;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_DMax;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_Angle;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ai_Evil_Icone;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_Last_Hit;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Evil_Last_Good;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_Last_Dmg;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_Last_Eff;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_Cumul_Dmg;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_f_last_paf_time;
int PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_o_last_paf;
int PNJ_Spectre_sp_mode_mort_DUP_SFX_XMEN_Dark_gao;
int PNJ_Spectre_sp_mode_mort__PJ_Sally_gao;
int fct_PNJ_Spectre_sp_mode_mort_PNJ_Spectre_sp_after_blend;
int PNJ_Spectre_sp_mode_mort_INV_manager_gao;
int PNJ_Spectre_sp_mode_mort_INV_manager_gao_i_Animals_Nb;
int PNJ_Spectre_sp_mode_mort_INV_manager_gao_ao_Animals;
int PNJ_Spectre_sp_mode_mort_INV_manager_gao_ai_AnimalsData;

int PNJ_Spectre_sp_exec_decode_paf_univ_GST_Fight_Manager_gao;
int PNJ_Spectre_sp_exec_decode_paf_univ_GST_Fight_Manager_gao_ao_Good_Actor;
int PNJ_Spectre_sp_exec_decode_paf_univ_univVars;
int PNJ_Spectre_sp_exec_decode_paf_univ_univVars_world_ID;
int PNJ_Spectre_sp_exec_decode_paf_univ_univVars_Flag_speciaux_sauve;
int PNJ_Spectre_sp_exec_decode_paf_univ__PJ_Sally_gao;
int fct_PNJ_Spectre_sp_exec_decode_paf_univ_PNJ_Spectre_sp_mode_mort;
int fct_PNJ_Spectre_sp_exec_decode_paf_univ_PNJ_Spectre_sp_mode_paf;

//PNJ_Spectre_sp_mode_paf
int PNJ_Spectre_sp_mode_paf_SFX_light_smoke_gao;
int fct_PNJ_Spectre_sp_mode_paf_PNJ_Spectre_sp_mode_combat;

//PNJ_Spectre_sp_mode_apparition_xmen
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ao_Evil_Actor;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_NRJ_Courante;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_i_Evil_NbActor;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_i_Evil_NbActorAlive;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_NRJShieldMax;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_o_last_mort;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ao_Good_Actor;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ao_Evil_Target;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ai_Good_Targeted;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ai_Evil_Targeted;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ao_Good_Target;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_Dmin;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_DMax;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_Angle;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ai_Evil_Icone;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_Last_Hit;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ao_Evil_Last_Good;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_Last_Dmg;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_Last_Eff;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_Cumul_Dmg;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_o_musique_fight;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Global_gao;
int PNJ_Spectre_sp_mode_apparition_xmen_GST_Global_gao_o_musique_fight_list;
int PNJ_Spectre_sp_mode_apparition_xmen_DUP_SFX_XMEN_Dark_gao;
int PNJ_Spectre_sp_mode_apparition_xmen_SFX_light_smoke_gao;
int fct_PNJ_Spectre_sp_mode_apparition_xmen_PNJ_Spectre_sp_tag_off;
int fct_PNJ_Spectre_sp_mode_apparition_xmen_PNJ_Spectre_sp_reflex;
int fct_PNJ_Spectre_sp_mode_apparition_xmen_PNJ_Spectre_sp_tag_on;
int fct_PNJ_Spectre_sp_mode_apparition_xmen_PNJ_Spectre_sp_mode_combat;
int fct_PNJ_Spectre_sp_mode_apparition_xmen_PNJ_Spectre_sp_after_blend;

// ========================================

//PNJ_Spectre_bras_PNJ_Spectre_Bras_init
int fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init;
int fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_before_display;
int fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_after_display;
int fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_reflex;

//PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move
int fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_init_snake;
int fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_GFX_Part;
int PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_SFX_light_smoke_gao;
int fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move;
int fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_PNJ_Spectre_bras_PNJ_Spectre_Bras_Destroy;
int fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_do_snake;

//PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init
int fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init;
int fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move;
int fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterrec_move;

//PNJ_Spectre_bras_PNJ_Spectre_Bras_afterrec_move
int fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterrec_move_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move;

//PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_GFX_Part_
int PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_GFX_Part_SFX_light_smoke_gao;

int PNJ_Spectre_sp_init()
{
    int v1; // eax
    int v2; // [esp+8h] [ebp-2E0h]
    tScriptStruct_PNJSpectre* v3; // [esp+18h] [ebp-2D0h]
    tGameObjectMain* v4; // [esp+20h] [ebp-2C8h]
    tGameObjectMain* v5; // [esp+28h] [ebp-2C0h]
    tGameObjectMain* v6; // [esp+30h] [ebp-2B8h]
    tGameObjectMain* v7; // [esp+40h] [ebp-2A8h]
    tGameObjectMain* v8; // [esp+48h] [ebp-2A0h]
    tGameObjectMain* v9; // [esp+50h] [ebp-298h]
    tGameObjectMain* v10; // [esp+58h] [ebp-290h]
    tGameObjectMain* v11; // [esp+60h] [ebp-288h]
    tGameObjectMain* v12; // [esp+68h] [ebp-280h]
    tGameObjectMain* v13; // [esp+70h] [ebp-278h]
    tGameObjectMain* v14; // [esp+78h] [ebp-270h]
    tGameObjectMain* v15; // [esp+80h] [ebp-268h]
    tGameObjectMain* v16; // [esp+84h] [ebp-264h]
    tGameObjectMain* v17; // [esp+88h] [ebp-260h]
    tGameObjectMain* v18; // [esp+8Ch] [ebp-25Ch]
    tGameObjectMain* v19; // [esp+90h] [ebp-258h]
    tGameObjectMain* v20; // [esp+98h] [ebp-250h]
    tGameObjectMain* v21; // [esp+9Ch] [ebp-24Ch]
    tGameObjectMain* v22; // [esp+A0h] [ebp-248h]
    tGameObjectMain* v23; // [esp+A4h] [ebp-244h]
    tGameObjectMain* v24; // [esp+10Ch] [ebp-1DCh]
    tGameObjectMain* v25; // [esp+110h] [ebp-1D8h]
    tGameObjectMain* v26; // [esp+114h] [ebp-1D4h]
    tGameObjectMain* v27; // [esp+118h] [ebp-1D0h]
    tGameObjectMain* v28; // [esp+128h] [ebp-1C0h]
    tGameObjectMain* v29; // [esp+12Ch] [ebp-1BCh]
    tGameObjectMain* v30; // [esp+130h] [ebp-1B8h]
    tGameObjectMain* v31; // [esp+134h] [ebp-1B4h]
    tGameObjectMain* v32; // [esp+138h] [ebp-1B0h]
    tGameObjectMain* v33; // [esp+13Ch] [ebp-1ACh]
    tGameObjectMain* v34; // [esp+140h] [ebp-1A8h]
    tGameObjectMain* v35; // [esp+144h] [ebp-1A4h]
    tGameObjectMain* v36; // [esp+148h] [ebp-1A0h]
    tGameObjectMain* v37; // [esp+14Ch] [ebp-19Ch]
    tGameObjectMain* v38; // [esp+150h] [ebp-198h]
    tGameObjectMain* v39; // [esp+154h] [ebp-194h]
    Vec3 v40; // [esp+180h] [ebp-168h] BYREF
    BOOL v41; // [esp+190h] [ebp-158h]
    Vec3 v42; // [esp+194h] [ebp-154h] BYREF
    tGameObjectMain* v43; // [esp+1A4h] [ebp-144h]
    Vec3 v44; // [esp+1ACh] [ebp-13Ch] BYREF
    Vec3 v45; // [esp+1B8h] [ebp-130h] BYREF
    tGameObjectMain* v46; // [esp+1C4h] [ebp-124h]
    int v47; // [esp+1C8h] [ebp-120h]
    int v48; // [esp+1CCh] [ebp-11Ch]
    Vec3 dest; // [esp+1E8h] [ebp-100h] BYREF
    Vec3 v50; // [esp+1F4h] [ebp-F4h] BYREF
    int v51; // [esp+204h] [ebp-E4h]
    float zoom; // [esp+20Ch] [ebp-DCh]
    int v53; // [esp+214h] [ebp-D4h]
    Vec3 v54; // [esp+21Ch] [ebp-CCh] BYREF
    int i; // [esp+228h] [ebp-C0h]
    int v56; // [esp+230h] [ebp-B8h]
    int v57; // [esp+238h] [ebp-B0h]
    int v58; // [esp+23Ch] [ebp-ACh]
    tGameObjectMain* v59; // [esp+248h] [ebp-A0h]
    int v60; // [esp+250h] [ebp-98h]
    Vec3 a3; // [esp+254h] [ebp-94h] BYREF
    Vec3 v62; // [esp+260h] [ebp-88h] BYREF
    int v63; // [esp+26Ch] [ebp-7Ch]
    int v64; // [esp+270h] [ebp-78h]
    int v65; // [esp+274h] [ebp-74h]
    int v66; // [esp+288h] [ebp-60h]
    tScriptStruct_PNJSpectre* v67; // [esp+294h] [ebp-54h]
    BOOL v68; // [esp+29Ch] [ebp-4Ch]
    Vec3 v69; // [esp+2A0h] [ebp-48h] BYREF
    Vec3 v70; // [esp+2ACh] [ebp-3Ch] BYREF
    tGameObjectMain* v71; // [esp+2B8h] [ebp-30h]
    BOOL v72; // [esp+2C0h] [ebp-28h]
    Vec3 v73; // [esp+2D0h] [ebp-18h] BYREF

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_PNJSpectre*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v67 = v3;
    v64 = 0;
    i = 0;
    v56 = 0;
    v65 = 0;
    v59 = 0;
    v58 = 0;
    v68 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        PNJ_Spectre_sp_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&PNJ_Spectre_sp_init_univVars_Flag_speciaux_sauve, (void*)PNJ_Spectre_sp_init_univVars, 4488);
        PNJ_Spectre_sp_init_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_INV_manager_gao_i_Animals_Nb,
            (void*)PNJ_Spectre_sp_init_INV_manager_gao,
            23888);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_INV_manager_gao_ao_Animals,
            (void*)PNJ_Spectre_sp_init_INV_manager_gao,
            23368);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_INV_manager_gao_ai_AnimalsData,
            (void*)PNJ_Spectre_sp_init_INV_manager_gao,
            23628);
        PNJ_Spectre_sp_init_GST_Fight_Manager_gao = AI_ul_RealResolveOneRefObject(0x9E002FFA);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ao_Evil_Actor,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            520);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_NRJ_Courante,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            892);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_i_Evil_NbActor,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            2624);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_i_Evil_NbActorAlive,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            2632);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_NRJShieldMax,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            768);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_o_last_mort,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            3788);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ao_Good_Actor,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            0);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ao_Evil_Target,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            644);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ai_Good_Targeted,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            416);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ai_Evil_Targeted,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            1636);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ao_Good_Target,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            52);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_Dmin,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            1884);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_DMax,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            2008);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_Angle,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            2132);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ai_Evil_Icone,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            1760);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_Last_Hit,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            1016);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ao_Evil_Last_Good,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            1140);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_Last_Dmg,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            1264);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_Last_Eff,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            1388);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_Cumul_Dmg,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            1512);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Fight_Manager_gao_o_musique_fight,
            (void*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao,
            2660);
        PNJ_Spectre_sp_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_init_GST_Global_gao_o_musique_fight_list,
            (void*)PNJ_Spectre_sp_init_GST_Global_gao,
            3460);
        AI2C_ResolveBuffer(&PNJ_Spectre_sp_init_univVars_Perso_ID_Companion, (void*)PNJ_Spectre_sp_init_univVars, 1040);
        AI2C_ResolveBuffer(&PNJ_Spectre_sp_init_univVars_Perso_ID_status, (void*)PNJ_Spectre_sp_init_univVars, 484);
        PNJ_Spectre_sp_init__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(&PNJ_Spectre_sp_init_univVars_Perso_ID_actor, (void*)PNJ_Spectre_sp_init_univVars, 988);
        fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_tag_off = (int)AI_ul_RealResolveOneRefFunction(0x26003615u);
        fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_reflex = (int)AI_ul_RealResolveOneRefFunction(0x9E006692);
        fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_tag_on = (int)AI_ul_RealResolveOneRefFunction(0x26003614u);
        fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_mode_combat = (int)AI_ul_RealResolveOneRefFunction(0x9E006691);
        fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_after_blend = (int)AI_ul_RealResolveOneRefFunction(0x9E00669B);
        fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_mode_apparition_xmen = (int)AI_ul_RealResolveOneRefFunction(0x26003638u);
        fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_mode_apparition_anim = (int)AI_ul_RealResolveOneRefFunction(0x9E006690);
    }
    if (*AI_gppst_CurrentJumpNode != (tAINode*)32)
    {
        if (v67->SF_ON != -1)
        {
            v68 = v67->SF_ON > 0
                && AI_EvalFunc_ARRBitTest_C((int*)(PNJ_Spectre_sp_init_univVars_Flag_speciaux_sauve + 4), v67->SF_ON);
            if (!v68)
            {
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                return 1000;
            }
        }
        if (v67->SF_OFF != -1)
        {
            v68 = v67->SF_OFF > 0
                && AI_EvalFunc_ARRBitTest_C((int*)(PNJ_Spectre_sp_init_univVars_Flag_speciaux_sauve + 4), v67->SF_OFF);
            if (v68)
            {
                v60 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
                if (v60 != 1000)
                    return v60;
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                return 1000;
            }
        }
        v67->o_me = (int)AI_gpst_CurrentGameObject;
        v39 = (tGameObjectMain*)v67->o_me;
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
        v38->extended->fields->designInt1 = 0;
        v37 = (tGameObjectMain*)v67->o_me;
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
        v36->extended->fields->designInt2 = 0;
        v35 = (tGameObjectMain*)v67->o_me;
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
        v34->extended->fields->designFlags = 64;
        AI_EvalFunc_OBJ_InfoPhotoParamSet_C(AI_gpst_CurrentGameObject, 4, 36, 2, 2, 0.013f, 1.0, 0.89999998f, 0.5);
        v56 = 0;
        if (*(int*)PNJ_Spectre_sp_init_INV_manager_gao_i_Animals_Nb < 64)
        {
            for (i = 0; i < *(int32_t*)PNJ_Spectre_sp_init_INV_manager_gao_i_Animals_Nb; ++i)
            {
                v33 = (tGameObjectMain*)v67->o_me;
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
                if (*(tGameObjectMain**)(PNJ_Spectre_sp_init_INV_manager_gao_ao_Animals + 4 * i + 4) == v32)
                    break;
            }
            if (i == *(uint32_t*)PNJ_Spectre_sp_init_INV_manager_gao_i_Animals_Nb)
            {
                *(uint32_t*)(PNJ_Spectre_sp_init_INV_manager_gao_ao_Animals
                    + 4 * *(uint32_t*)PNJ_Spectre_sp_init_INV_manager_gao_i_Animals_Nb
                    + 4) = v67->o_me;
                *(uint32_t*)(PNJ_Spectre_sp_init_INV_manager_gao_ai_AnimalsData
                    + 4 * (*(uint32_t*)PNJ_Spectre_sp_init_INV_manager_gao_i_Animals_Nb)++
                    + 4) = (v67->des_i_etage << 8) + 36;
            }
        }
        if (v67->i_non_selectionnable)
        {
            v31 = (tGameObjectMain*)v67->o_me;
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
            i = v30->extended->fields->designFlags | 0x20000000;
            v29 = (tGameObjectMain*)v67->o_me;
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
            v28->extended->fields->designFlags = i;
        }
        v41 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(2);
        if (v41 && !v67->i_fight_on)
        {
            v67->i_fight_on = 1;
            AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(0x1000, 0);
            i = AI_EvalFunc_ARRObjSearch_C(
                (tGameObjectMain**)(PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ao_Evil_Actor + 4),
                30,
                0);
            if (i != -1)
            {
                v71 = AI_gpst_CurrentGameObject;
                v27 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                        v27 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                    v26 = v27;
                }
                else
                {
                    v26 = 0;
                }
                v56 = v26->extended->fields->designFlags & 0x7FFFFFFF;
                v46 = AI_gpst_CurrentGameObject;
                v25 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                        v25 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                    v24 = v25;
                }
                else
                {
                    v24 = 0;
                }
                v24->extended->fields->designFlags = v56;
                *(uint32_t*)(PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ao_Evil_Actor + 4 * i + 4) = (uint32_t)AI_gpst_CurrentGameObject;
                *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_NRJ_Courante, i) = 0x42A00000;
                if (!*(uint32_t*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao_i_Evil_NbActor)
                    *(uint32_t*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao_i_Evil_NbActorAlive = 0;
                ++* (uint32_t*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao_i_Evil_NbActorAlive;
                ++* (uint32_t*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao_i_Evil_NbActor;
                *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_NRJShieldMax, i) = 0x42A00000;
            }
            *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_Dmin, i) = 0x40400000;
            *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_DMax, i) = 0x40C00000;
            *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_Angle, i) = 0x40490FDC;
            *(uint32_t*)(PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ai_Evil_Icone + 4 * i + 4) = 9;
            *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_Last_Hit, i) = 0;
            *(uint32_t*)(PNJ_Spectre_sp_init_GST_Fight_Manager_gao_ao_Evil_Last_Good + 4 * i + 4) = 0;
            *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_Last_Dmg, i) = 0;
            *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_Last_Eff, i) = 0;
            *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao_af_Evil_Cumul_Dmg, i) = 0;
            v23 = (tGameObjectMain*)v67->acteur_musique_fight;
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
            if (v22)
            {
                v21 = (tGameObjectMain*)v67->acteur_musique_fight;
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
                if (v20 == (tGameObjectMain*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao)
                    *(uint32_t*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao_o_musique_fight = *(uint32_t*)(PNJ_Spectre_sp_init_GST_Global_gao_o_musique_fight_list
                        + 44);
                else
                    *(uint32_t*)PNJ_Spectre_sp_init_GST_Fight_Manager_gao_o_musique_fight = v67->acteur_musique_fight;
            }
        }
        v67->o_body = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(3u);
        v67->o_snap = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(2u);
        if (v67->i_NMI == 1
            || *(uint32_t*)PNJ_Spectre_sp_init_univVars_Perso_ID_Companion == -1
            || *(uint32_t*)(PNJ_Spectre_sp_init_univVars_Perso_ID_status
                + 4 * *(uint32_t*)PNJ_Spectre_sp_init_univVars_Perso_ID_Companion
                + 4) == 3)
        {
            v67->i_NMI = 1;
            v67->o_NMI = PNJ_Spectre_sp_init__PJ_Sally_gao;
        }
        else
        {
            v67->f_time_last_attak = TIM_gf_MainClock - TIM_gf_LockTime;
            v67->i_flag_order = 64;
            v67->o_NMI = *(uint32_t*)(PNJ_Spectre_sp_init_univVars_Perso_ID_actor
                + 4 * *(uint32_t*)PNJ_Spectre_sp_init_univVars_Perso_ID_Companion
                + 4);
        }
        if (v67->f_time_wait != 0.0)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)32;
            TIME_Wait_C(v67->f_time_wait);
            return 0;
        }
    }
    v19 = (tGameObjectMain*)v67->WP_init_combat;
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
        v67->WP_init_combat = v67->WP_pos_combat;
    v67->f_retard = AI_EvalFunc_MATHRandFloat_C(0.1f, 0.25f);
    v17 = (tGameObjectMain*)v67->o_gamin_snap;
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
        v15 = (tGameObjectMain*)v67->o_gamin_snap;
        if (v15)
        {
            if ((unsigned int)v15 <= 2)
                v15 = AI_MainActor[(uint32_t)v15 - 1];
            AI_EvalFunc_OBJ_HierarchyReset_C(v15);
        }
        else
        {
            AI_EvalFunc_OBJ_HierarchyReset_C(0);
        }
        v14 = (tGameObjectMain*)v67->o_snap;
        if (v14)
        {
            if ((unsigned int)v14 <= 2)
                v14 = AI_MainActor[(uint32_t)v14 - 1];
            AI_EvalFunc_OBJGetPos_C(v14, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        v13 = (tGameObjectMain*)v67->o_gamin_snap;
        if (v13)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            AI_EvalFunc_OBJSetPos_C(v13, &a3);
        }
        else
        {
            AI_EvalFunc_OBJSetPos_C(0, &a3);
        }
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v42);
        v12 = (tGameObjectMain*)v67->o_gamin_snap;
        if (v12)
        {
            if ((unsigned int)v12 <= 2)
                v12 = AI_MainActor[(uint32_t)v12 - 1];
            AI_EvalFunc_OBJ_SightSet_C(v12, &v42);
        }
        else
        {
            AI_EvalFunc_OBJ_SightSet_C(0, &v42);
        }
        v11 = (tGameObjectMain*)v67->o_snap;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v11 = AI_MainActor[(uint32_t)v11 - 1];
            AI_EvalFunc_OBJGetPos_C(v11, &v69);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v69);
        }
        v10 = (tGameObjectMain*)v67->o_gamin_snap;
        if (v10)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            AI_EvalFunc_COLStartMatrixSet_C(v10, &v69);
        }
        else
        {
            AI_EvalFunc_COLStartMatrixSet_C(0, &v69);
        }
        v9 = (tGameObjectMain*)v67->o_gamin_snap;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(v9, 1);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
        }
        v8 = (tGameObjectMain*)v67->o_gamin_snap;
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v8 = AI_MainActor[(uint32_t)v8 - 1];
            AI_EvalFunc_OBJ_FlagInactiveSet_C(v8, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
        }
        v7 = (tGameObjectMain*)v67->o_gamin_snap;
        if (v7)
        {
            if ((unsigned int)v7 <= 2)
                v7 = AI_MainActor[(uint32_t)v7 - 1];
            AI_EvalFunc_ACTActionSet_C(v7, 3);
        }
        else
        {
            AI_EvalFunc_ACTActionSet_C(0, 3);
        }
    }
    v1 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000000);
    v67->i_snd_deplace_id = v1;
    AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v67->i_snd_deplace_id);
    v2 = SND_l_GetFreq(v67->i_snd_deplace_id);
    v67->f_snd_deplace_freq = (float)v2;
    v67->i_Compagnon_attaque = 0.0;
    AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 41024, 0);
    AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
    if (v67->f_zoom != 0.0)
    {
        if (v67->f_zoom == -1.0)
        {
            zoom = AI_EvalFunc_MATHRandFloat_C(v67->f_zoom_min, v67->f_zoom_max);
            AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, zoom);
        }
        else
        {
            AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, v67->f_zoom);
        }
    }
    AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
    AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, -1, 0);
    v54.x = 0.34999999f;
    v54.y = 0.34999999f;
    v54.z = 0.34999999f;
    AI_EvalFunc_COLSpecificSizeSet_C(AI_gpst_CurrentGameObject, 6u, &v54);
    AI_EvalFunc_COLCrossableSet_C(AI_gpst_CurrentGameObject, 194, 0);
    v72 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
    v48 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x40021004, 0);
    AI_EvalFunc_DYNSlideHorizCosAngleSet_C(AI_gpst_CurrentGameObject, 0.70710701f);
    v73.x = 0.0;
    v73.y = 0.0;
    v73.z = 0.0;
    v63 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v73);
    v40.x = 0.0;
    v40.y = 0.0;
    v40.z = 0.0;
    AI_EvalFunc_DYNTractionSet_C_CURRENT(&v40);
    v44.x = 5.0;
    v44.y = 5.0;
    v44.z = 1.0;
    v66 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v44);
    v70.x = 0.0;
    v70.y = 0.0;
    v70.z = 0.0;
    AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v70);
    v57 = AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 2, 0);
    v67->i_color_membrane = 1720142016;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v50);
    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v62);
    v45.x = v62.x + v62.x;
    v45.y = v62.y + v62.y;
    v45.z = v62.z + v62.z;
    MATH_SubstractVec(&dest, &v50, &v45);
    v67->v_grav_pos = dest;
    v67->v_grav_speed.x = 0.0;
    v67->v_grav_speed.y = 0.0;
    v67->v_grav_speed.z = 0.0;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v67->v_last_pos);
    v67->f_pos_dist = 0.0;
    if (v67->i_ZDM_ON_OFF)
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 3, 0);
    else
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 2, 1);
    AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0xFFu, 0);
    if (v67->i_depart == 1)
    {
        AI_EvalFunc_AITrackChange_C(0, (tAIFunction*)fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_tag_off);
        AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_reflex);
        AI_EvalFunc_AITrackChange_C(4, (tAIFunction*)fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_tag_on);
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_mode_combat);
        v43 = AI_gpst_CurrentGameObject;
        v6 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v6 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v6,
                3,
                (tAIFunction*)fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_after_blend);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                3,
                (tAIFunction*)fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_after_blend);
        }
    }
    else if (v67->i_depart == 3)
    {
        v59 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(3u);
        v5 = v59;
        if (v59)
        {
            if ((unsigned int)v59 <= 2)
                v5 = AI_MainActor[(uint32_t)v59 - 1];
            AI_EvalFunc_OBJDrawMaskSet_C(v5, 8, 0);
        }
        else
        {
            AI_EvalFunc_OBJDrawMaskSet_C(0, 8, 0);
        }
        v59 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(5u);
        v4 = v59;
        if (v59)
        {
            if ((unsigned int)v59 <= 2)
                v4 = AI_MainActor[(uint32_t)v59 - 1];
            AI_EvalFunc_OBJDrawMaskSet_C(v4, 8, 0);
        }
        else
        {
            AI_EvalFunc_OBJDrawMaskSet_C(0, 8, 0);
        }
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        v67->f_local_tresh_alpha = 0.0;
        v67->f_vitre_local_alpha = 0.0;
        v67->i_color_membrane = 0;
        v67->i_auto_color_membrane = 0;
        v67->f_halo_time = TIM_gf_MainClock - TIM_gf_LockTime;
        v53 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(AI_gpst_CurrentGameObject, 9, -1.0, 0.80000001f, 1.1f);
        v47 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_mode_apparition_xmen);
        if (v47 != 1000)
            return v47;
    }
    else
    {
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, 2);
        v51 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_PNJ_Spectre_sp_init_PNJ_Spectre_sp_mode_apparition_anim);
        if (v51 != 1000)
            return v51;
    }
    return 1000;
}

int PNJ_Spectre_sp_mode_combat()
{
    float X; // [esp+0h] [ebp-56Ch]
    float Xa; // [esp+0h] [ebp-56Ch]
    float v3; // [esp+8h] [ebp-564h]
    tScriptStruct_PNJSpectre* v4; // [esp+Ch] [ebp-560h]
    float v5; // [esp+18h] [ebp-554h]
    float v6; // [esp+28h] [ebp-544h]
    tGameObjectMain* v7; // [esp+38h] [ebp-534h]
    tGameObjectMain* v8; // [esp+40h] [ebp-52Ch]
    tGameObjectMain* v9; // [esp+48h] [ebp-524h]
    tGameObjectMain* v10; // [esp+50h] [ebp-51Ch]
    tGameObjectMain* v11; // [esp+5Ch] [ebp-510h]
    tGameObjectMain* v12; // [esp+6Ch] [ebp-500h]
    float v13; // [esp+88h] [ebp-4E4h]
    tGameObjectMain* v14; // [esp+90h] [ebp-4DCh]
    tGameObjectMain* v15; // [esp+98h] [ebp-4D4h]
    tGameObjectMain* v16; // [esp+9Ch] [ebp-4D0h]
    tGameObjectMain* v17; // [esp+A0h] [ebp-4CCh]
    tGameObjectMain* v18; // [esp+B0h] [ebp-4BCh]
    tGameObjectMain* v19; // [esp+B8h] [ebp-4B4h]
    tGameObjectMain* v20; // [esp+C0h] [ebp-4ACh]
    tGameObjectMain* v21; // [esp+C8h] [ebp-4A4h]
    tGameObjectMain* v22; // [esp+D4h] [ebp-498h]
    tGameObjectMain* v23; // [esp+E4h] [ebp-488h]
    tGameObjectMain* v24; // [esp+E8h] [ebp-484h]
    tGameObjectMain* v25; // [esp+ECh] [ebp-480h]
    tGameObjectMain* v26; // [esp+F0h] [ebp-47Ch]
    tGameObjectMain* v27; // [esp+F4h] [ebp-478h]
    tGameObjectMain* v28; // [esp+F8h] [ebp-474h]
    tGameObjectMain* v29; // [esp+FCh] [ebp-470h]
    tGameObjectMain* v30; // [esp+108h] [ebp-464h]
    tGameObjectMain* v31; // [esp+10Ch] [ebp-460h]
    float v32; // [esp+110h] [ebp-45Ch]
    tGameObjectMain* v33; // [esp+11Ch] [ebp-450h]
    tGameObjectMain* v34; // [esp+124h] [ebp-448h]
    tGameObjectMain* v35; // [esp+130h] [ebp-43Ch]
    tGameObjectMain* v36; // [esp+138h] [ebp-434h]
    tGameObjectMain* v37; // [esp+140h] [ebp-42Ch]
    tGameObjectMain* v38; // [esp+144h] [ebp-428h]
    tGameObjectMain* v39; // [esp+14Ch] [ebp-420h]
    tGameObjectMain* v40; // [esp+170h] [ebp-3FCh]
    tGameObjectMain* v41; // [esp+178h] [ebp-3F4h]
    tGameObjectMain* v42; // [esp+19Ch] [ebp-3D0h]
    Vec3 v43; // [esp+1A0h] [ebp-3CCh] BYREF
    Vec3 v44; // [esp+1ACh] [ebp-3C0h] BYREF
    Vec3 v45; // [esp+1B8h] [ebp-3B4h] BYREF
    Vec3 v46; // [esp+1C4h] [ebp-3A8h] BYREF
    Vec3 v47; // [esp+1D0h] [ebp-39Ch] BYREF
    Vec3 v48; // [esp+1DCh] [ebp-390h] BYREF
    tGameObjectMain* v49; // [esp+1E8h] [ebp-384h]
    float v50; // [esp+1F0h] [ebp-37Ch]
    float v51; // [esp+1F4h] [ebp-378h]
    float v52; // [esp+1F8h] [ebp-374h]
    float v53; // [esp+1FCh] [ebp-370h]
    Vec3 v54; // [esp+200h] [ebp-36Ch] BYREF
    Vec3 v55; // [esp+20Ch] [ebp-360h] BYREF
    Vec3 a4; // [esp+218h] [ebp-354h] BYREF
    Vec3 v57; // [esp+224h] [ebp-348h] BYREF
    Vec3 v58; // [esp+230h] [ebp-33Ch] BYREF
    Vec3 v59; // [esp+23Ch] [ebp-330h] BYREF
    Vec3 v60; // [esp+248h] [ebp-324h] BYREF
    Vec3 v61; // [esp+254h] [ebp-318h] BYREF
    Vec3 v62; // [esp+260h] [ebp-30Ch] BYREF
    float v63; // [esp+26Ch] [ebp-300h]
    Vec3 a5; // [esp+274h] [ebp-2F8h] BYREF
    Vec3 v66; // [esp+280h] [ebp-2ECh] BYREF
    Vec3 v67; // [esp+28Ch] [ebp-2E0h] BYREF
    Vec3 v68; // [esp+298h] [ebp-2D4h] BYREF
    int v69; // [esp+2A4h] [ebp-2C8h]
    Vec3 v70; // [esp+2A8h] [ebp-2C4h] BYREF
    Vec3 v71; // [esp+2B4h] [ebp-2B8h] BYREF
    Vec3 v72; // [esp+2C0h] [ebp-2ACh] BYREF
    Vec3 v73; // [esp+2CCh] [ebp-2A0h] BYREF
    int v74; // [esp+2D8h] [ebp-294h]
    int v75; // [esp+2E0h] [ebp-28Ch]
    Vec3 v76; // [esp+2E4h] [ebp-288h] BYREF
    Vec3 v77; // [esp+2F0h] [ebp-27Ch] BYREF
    Vec3 v78; // [esp+2FCh] [ebp-270h] BYREF
    Vec3 v79; // [esp+30Ch] [ebp-260h] BYREF
    Vec3 v80; // [esp+318h] [ebp-254h] BYREF
    Vec3 v81; // [esp+324h] [ebp-248h] BYREF
    Vec3 v82; // [esp+330h] [ebp-23Ch] BYREF
    Vec3 v83; // [esp+33Ch] [ebp-230h] BYREF
    Vec3 v84; // [esp+348h] [ebp-224h] BYREF
    Vec3 v85; // [esp+354h] [ebp-218h] BYREF
    Vec3 v86; // [esp+360h] [ebp-20Ch] BYREF
    Vec3 v87; // [esp+36Ch] [ebp-200h] BYREF
    float v88; // [esp+378h] [ebp-1F4h]
    int v90; // [esp+380h] [ebp-1ECh]
    Vec3 v91; // [esp+384h] [ebp-1E8h] BYREF
    Vec3 v92; // [esp+390h] [ebp-1DCh] BYREF
    Vec3 v93; // [esp+39Ch] [ebp-1D0h] BYREF
    float v94; // [esp+3ACh] [ebp-1C0h]
    float v95; // [esp+3B0h] [ebp-1BCh]
    BOOL v96; // [esp+3B4h] [ebp-1B8h]
    Vec3 v97; // [esp+3B8h] [ebp-1B4h] BYREF
    Vec3 v98; // [esp+3C4h] [ebp-1A8h] BYREF
    Vec3 v99; // [esp+3D0h] [ebp-19Ch] BYREF
    Vec3 a3; // [esp+3DCh] [ebp-190h] BYREF
    Vec3 v101; // [esp+3E8h] [ebp-184h] BYREF
    Vec3 v102; // [esp+3F4h] [ebp-178h] BYREF
    int v103; // [esp+404h] [ebp-168h]
    int v104; // [esp+408h] [ebp-164h]
    Vec3 v105; // [esp+40Ch] [ebp-160h] BYREF
    Vec3 dest; // [esp+418h] [ebp-154h] BYREF
    Vec3 v107; // [esp+424h] [ebp-148h] BYREF
    float v109; // [esp+434h] [ebp-138h]
    Vec3 a1; // [esp+43Ch] [ebp-130h] BYREF
    Vec3 v111; // [esp+448h] [ebp-124h] BYREF
    Vec3 v112; // [esp+454h] [ebp-118h] BYREF
    Vec3 v113; // [esp+460h] [ebp-10Ch]
    Vec3 v114; // [esp+46Ch] [ebp-100h] BYREF
    Vec3 v115; // [esp+478h] [ebp-F4h] BYREF
    Vec3 v116; // [esp+484h] [ebp-E8h] BYREF
    Vec3 v117; // [esp+494h] [ebp-D8h] BYREF
    Vec3 v118; // [esp+4A0h] [ebp-CCh] BYREF
    Vec3 v119; // [esp+4ACh] [ebp-C0h] BYREF
    Vec3 v120; // [esp+4B8h] [ebp-B4h] BYREF
    Vec3 a2; // [esp+4C4h] [ebp-A8h] BYREF
    Vec3 v122; // [esp+4D0h] [ebp-9Ch] BYREF
    tScriptStruct_PNJSpectre* v124; // [esp+4E0h] [ebp-8Ch]
    Vec3 v125; // [esp+4E8h] [ebp-84h] BYREF
    Vec3 v126; // [esp+4F4h] [ebp-78h] BYREF
    Vec3 v127; // [esp+500h] [ebp-6Ch] BYREF
    Vec3 v128; // [esp+50Ch] [ebp-60h] BYREF
    int v129; // [esp+51Ch] [ebp-50h]
    float v130; // [esp+524h] [ebp-48h]
    Vec3 v131; // [esp+528h] [ebp-44h] BYREF
    Vec3 v132; // [esp+534h] [ebp-38h] BYREF
    Vec3 v133; // [esp+540h] [ebp-2Ch] BYREF
    Vec3 v134; // [esp+54Ch] [ebp-20h] BYREF
    int v136; // [esp+55Ch] [ebp-10h]
    int v137; // [esp+560h] [ebp-Ch]
    float v138; // [esp+564h] [ebp-8h]
    BOOL v139; // [esp+568h] [ebp-4h]

    v76.x = 1.0;
    v76.y = 0.0;
    v76.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    v67.x = 1.0;
    v67.y = 0.0;
    v67.z = 0.0;
    v81.x = 1.0;
    v81.y = 0.0;
    v81.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_PNJSpectre*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v124 = v4;
    v136 = 0;
    v137 = 0;
    v51 = 0.0;
    v53 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        PNJ_Spectre_sp_mode_combat__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_combat__PJ_Sally_gao_i_fight_action,
            (void*)PNJ_Spectre_sp_mode_combat__PJ_Sally_gao,
            1240);
        PNJ_Spectre_sp_mode_combat_GST_Fight_Manager_gao = AI_ul_RealResolveOneRefObject(0x9E002FFA);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_combat_GST_Fight_Manager_gao_ao_Good_Actor,
            (void*)PNJ_Spectre_sp_mode_combat_GST_Fight_Manager_gao,
            0);
    }
    v124->i_mode = 3;
    v42 = (tGameObjectMain*)v124->o_NMI;
    if (v42)
    {
        if ((unsigned int)v42 <= 2)
            v42 = AI_MainActor[(uint32_t)v42 - 1];
        AI_EvalFunc_OBJGetPos_C(v42, &v77);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v77);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v122);
    MATH_SubstractVec(&dest, &v77, &v122);
    a3 = dest;
    if (AI_EvalFunc_MTHVecNullEpsilon_C(&a3))
    {
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v60);
        a3 = v60;
    }
    else
    {
        MATH_NormalizeEqualVector(&a3);
    }
    v41 = (tGameObjectMain*)v124->o_NMI;
    if (v41)
    {
        if ((unsigned int)v41 <= 2)
            v41 = AI_MainActor[(uint32_t)v41 - 1];
        AI_EvalFunc_OBJGetPos_C(v41, &v92);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v92);
    }
    v40 = (tGameObjectMain*)v124->WP_pos_combat;
    if (v40)
    {
        if ((unsigned int)v40 <= 2)
            v40 = AI_MainActor[(uint32_t)v40 - 1];
        AI_EvalFunc_OBJGetPos_C(v40, &v128);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v128);
    }
    MATH_SubstractVec(&v71, &v92, &v128);
    a1 = v71;
    if (AI_EvalFunc_MTHVecNullEpsilon_C(&a1))
    {
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v125);
        a1 = v125;
    }
    else
    {
        MATH_NormalizeEqualVector(&a1);
    }
    v137 = AI_EvalFunc_ACTActionGet_C_CURRENT();
    v136 = 1;
    if (*(uint32_t*)PNJ_Spectre_sp_mode_combat__PJ_Sally_gao_i_fight_action == 20
        && (v104 = AI_EvalFunc_ACTActionGet_C((tGameObjectMain*)PNJ_Spectre_sp_mode_combat__PJ_Sally_gao), v104 != 197)
        || (v69 = AI_EvalFunc_ACTDesignFlagsTest_C((tGameObjectMain*)PNJ_Spectre_sp_mode_combat__PJ_Sally_gao, 0x100u)) != 0)
    {
        v136 = 0;
    }
    if ((v124->i_flag_order & 8) != 0)
    {
        if (v136)
        {
            if (AI_EvalFunc_TIMEElapsed_C(v124->f_time_paf_sally, 0.5))
            {
                v39 = (tGameObjectMain*)v124->o_NMI;
                if (v39)
                {
                    if ((unsigned int)v39 <= 2)
                        v39 = AI_MainActor[(uint32_t)v39 - 1];
                    AI_EvalFunc_OBJGetPos_C(v39, &v97);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v97);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v78);
                v51 = v97.z - v78.z;
                if (v124->f_distance_actuelle < 6.0)
                {
                    v3 = (float)fabs(v51);
                    v88 = v3;
                    if (v3 < 0.5)
                        goto LABEL_47;
                }
                v38 = (tGameObjectMain*)v124->o_NMI;
                if (v38)
                {
                    if ((unsigned int)v38 <= 2)
                        v38 = AI_MainActor[(uint32_t)v38 - 1];
                    v37 = v38;
                }
                else
                {
                    v37 = 0;
                }
                if (v37 == *(tGameObjectMain**)(PNJ_Spectre_sp_mode_combat_GST_Fight_Manager_gao_ao_Good_Actor + 4))
                {
                LABEL_47:
                    v124->f_pos_dt = 0.0;
                    v36 = (tGameObjectMain*)v124->o_NMI;
                    if (v36)
                    {
                        if ((unsigned int)v36 <= 2)
                            v36 = AI_MainActor[(uint32_t)v36 - 1];
                        AI_EvalFunc_OBJGetPos_C(v36, &v58);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v58);
                    }
                    v86.x = a1.x * 3.0f;
                    v86.y = a1.y * 3.0f;
                    v86.z = a1.z * 3.0f;
                    MATH_SubstractVec(&v116, &v58, &v86);
                    v35 = (tGameObjectMain*)v124->WP_pos_combat;
                    if (v35)
                    {
                        if ((unsigned int)v35 <= 2)
                            v35 = AI_MainActor[(uint32_t)v35 - 1];
                        AI_EvalFunc_OBJGetPos_C(v35, &v101);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v101);
                    }
                    MATH_SubstractVec(&v133, &v116, &v101);
                    v109 = MATH_VecLen(&v133);
                    v34 = (tGameObjectMain*)v124->o_NMI;
                    if (v34)
                    {
                        if ((unsigned int)v34 <= 2)
                            v34 = AI_MainActor[(uint32_t)v34 - 1];
                        AI_EvalFunc_OBJGetPos_C(v34, &v82);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v82);
                    }
                    v33 = (tGameObjectMain*)v124->WP_pos_combat;
                    if (v33)
                    {
                        if ((unsigned int)v33 <= 2)
                            v33 = AI_MainActor[(uint32_t)v33 - 1];
                        AI_EvalFunc_OBJGetPos_C(v33, &v79);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v79);
                    }
                    MATH_SubstractVec(&v111, &v82, &v79);
                    v63 = MATH_VecLen(&v111);
                    v124->f_pos_dist = v109 / v63;
                    if (v137 != 2 && v137 != 3)
                    {
                        v124->i_attaque_on = 1;
                        v124->f_time_debut_attaque = TIM_gf_MainClock - TIM_gf_LockTime;
                        v95 = AI_EvalFunc_MATHRandFloat_C(0.0, 1.0);
                        if (v95 >= 0.5)
                        {
                            v124->f_color_duration = 1.3f;
                            AI_EvalFunc_ACTActionSet_C_CURRENT(3);
                        }
                        else
                        {
                            v124->f_color_duration = 1.4f;
                            AI_EvalFunc_ACTActionSet_C_CURRENT(2);
                        }
                    }
                }
            }
            else
            {
                v32 = v124->f_dt * 6.0f;
                if (v32 < 0.0)
                    v32 = 0.0;
                if (v32 > 1.0)
                    v32 = 1.0;
                v124->f_pos_dt = 0.0f * v32 + v124->f_pos_dt * (1.0f - v32);
            }
        }
        if ((v137 == 2 || v137 == 3) && (v96 = AI_EvalFunc_ACTActionFinished_C_CURRENT()))
        {
            v31 = (tGameObjectMain*)v124->o_me;
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
            v30->extended->fields->designInt2 = 8;
            v124->f_time_last_attak = TIM_gf_MainClock - TIM_gf_LockTime;
            v124->f_time_paf_sally = TIM_gf_MainClock - TIM_gf_LockTime;
            v124->i_attaque_on = 0;
            v75 = AI_EvalFunc_MATHRandInt_C(0, 2);
            AI_EvalFunc_ACTActionSet_C_CURRENT(v75);
        }
        else if (!v136)
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v132);
            v113.x = -v132.x;
            v113.y = -v132.y;
            v113.z = -v132.z;
            v55.x = v113.x + v113.x;
            v55.y = v113.y + v113.y;
            v55.z = v113.z + v113.z;
            v74 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v55);
        }
    }
    else if ((v124->i_flag_order & 2) != 0)
    {
        v124->i_attaque_on = 0;
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 1);
        if (!AI_EvalFunc_ANIIsInBlend_C_CURRENT())
        {
            v129 = AI_EvalFunc_ACTActionGet_C_CURRENT();
            if (v129 > 1)
            {
                v103 = AI_EvalFunc_MATHRandInt_C(0, 2);
                AI_EvalFunc_ACTActionSet_C_CURRENT(v103);
            }
        }
        v29 = (tGameObjectMain*)v124->o_me;
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
        v28->extended->fields->designInt2 = 2;
    }
    else
    {
        v27 = (tGameObjectMain*)v124->o_me;
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
        if ((v26->extended->fields->designInt2 & 8) != 0)
        {
            v25 = (tGameObjectMain*)v124->o_me;
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
            v24->extended->fields->designInt2 = 64;
        }
        else if (v137 == 2 || v137 == 3)
        {
            if (AI_EvalFunc_ACTActionFinished_C_CURRENT())
            {
                v124->f_time_last_attak = TIM_gf_MainClock - TIM_gf_LockTime;
                v124->f_time_paf_sally = TIM_gf_MainClock - TIM_gf_LockTime;
                v124->i_attaque_on = 0;
                v90 = AI_EvalFunc_MATHRandInt_C(0, 2);
                AI_EvalFunc_ACTActionSet_C_CURRENT(v90);
            }
        }
    }
    v124->f_pos_dt = 0.0;
    if ((v137 == 2 || v137 == 3) && !(v139 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(4)))
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v85);
        v76 = v85;
        v124->f_retard = 0.0;
        v12 = (tGameObjectMain*)v124->o_NMI;
        if (v12)
        {
            if ((unsigned int)v12 <= 2)
                v12 = AI_MainActor[(uint32_t)v12 - 1];
            AI_EvalFunc_OBJGetPos_C(v12, &v107);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v107);
        }
        v115.x = a1.x + a1.x;
        v115.y = a1.y + a1.y;
        v115.z = a1.z + a1.z;
        MATH_SubstractVec(&v117, &v107, &v115);
        v81 = v117;
        v11 = (tGameObjectMain*)v124->WP_pos_combat;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v11 = AI_MainActor[(uint32_t)v11 - 1];
            AI_EvalFunc_OBJGetPos_C(v11, &v54);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v54);
        }
        MATH_SubstractVec(&v105, &v54, &v81);
        v53 = MATH_VecLen(&v105);
        v10 = (tGameObjectMain*)v124->WP_pos_combat;
        if (v10)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            AI_EvalFunc_OBJGetPos_C(v10, &v118);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v118);
        }
        MATH_SubstractVec(&v99, &v81, &v118);
        v9 = (tGameObjectMain*)v124->o_NMI;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            AI_EvalFunc_OBJGetPos_C(v9, &v61);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v61);
        }
        v8 = (tGameObjectMain*)v124->WP_pos_combat;
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v8 = AI_MainActor[(uint32_t)v8 - 1];
            AI_EvalFunc_OBJGetPos_C(v8, &v48);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v48);
        }
        MATH_SubstractVec(&v47, &v61, &v48);
        v138 = v47.z * v99.z + v47.y * v99.y + v99.x * v47.x;
        if (v138 < 0.0)
        {
            v7 = (tGameObjectMain*)v124->WP_pos_combat;
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                AI_EvalFunc_OBJGetPos_C(v7, &v59);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v59);
            }
            v76 = v59;
        }
    }
    else
    {
        if (v124->i_NMI == 1)
            v124->f_retard = 3.0;
        else
            v124->f_retard = 1.0;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
        X = v124->f_dt * 6.0f;
        AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, X, &a5);
        v120.x = 0.0;
        v120.y = 0.0;
        v120.z = 1.0;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a5, &v120);
        v23 = (tGameObjectMain*)v124->o_NMI;
        if (v23)
        {
            if ((unsigned int)v23 <= 2)
                v23 = AI_MainActor[(uint32_t)v23 - 1];
            AI_EvalFunc_OBJGetPos_C(v23, &v134);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v134);
        }
        v72.x = a1.x + a1.x;
        v72.y = a1.y + a1.y;
        v72.z = a1.z + a1.z;
        MATH_SubstractVec(&v91, &v134, &v72);
        v81 = v91;
        v22 = (tGameObjectMain*)v124->WP_pos_combat;
        if (v22)
        {
            if ((unsigned int)v22 <= 2)
                v22 = AI_MainActor[(uint32_t)v22 - 1];
            AI_EvalFunc_OBJGetPos_C(v22, &v70);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v70);
        }
        MATH_SubstractVec(&v84, &v70, &v81);
        v53 = MATH_VecLen(&v84);
        v21 = (tGameObjectMain*)v124->WP_pos_combat;
        if (v21)
        {
            if ((unsigned int)v21 <= 2)
                v21 = AI_MainActor[(uint32_t)v21 - 1];
            AI_EvalFunc_OBJGetPos_C(v21, &v119);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v119);
        }
        MATH_SubstractVec(&v46, &v81, &v119);
        v20 = (tGameObjectMain*)v124->o_NMI;
        if (v20)
        {
            if ((unsigned int)v20 <= 2)
                v20 = AI_MainActor[(uint32_t)v20 - 1];
            AI_EvalFunc_OBJGetPos_C(v20, &v98);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v98);
        }
        v19 = (tGameObjectMain*)v124->WP_pos_combat;
        if (v19)
        {
            if ((unsigned int)v19 <= 2)
                v19 = AI_MainActor[(uint32_t)v19 - 1];
            AI_EvalFunc_OBJGetPos_C(v19, &v66);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v66);
        }
        MATH_SubstractVec(&v102, &v98, &v66);
        v52 = v102.z * v46.z + v102.y * v46.y + v46.x * v102.x;
        if (v52 >= 0.0)
        {
            if (v124->f_dist_max == 0.0 || v124->f_dist_max >= (double)v53)
                goto LABEL_171;
            v49 = AI_gpst_CurrentGameObject;
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
            if (v16->extended->fields->designFlags < 0)
            {
            LABEL_171:
                v76 = v81;
            }
            else
            {
                v15 = (tGameObjectMain*)v124->WP_pos_combat;
                if (v15)
                {
                    if ((unsigned int)v15 <= 2)
                        v15 = AI_MainActor[(uint32_t)v15 - 1];
                    AI_EvalFunc_OBJGetPos_C(v15, &v127);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v127);
                }
                v14 = (tGameObjectMain*)v124->WP_pos_combat;
                if (v14)
                {
                    if ((unsigned int)v14 <= 2)
                        v14 = AI_MainActor[(uint32_t)v14 - 1];
                    AI_EvalFunc_OBJGetPos_C(v14, &v57);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v57);
                }
                MATH_SubstractVec(&v45, &v81, &v57);
                AI_EvalFunc_MATHVecNormalize_C(&v45, &v83);
                v13 = v124->f_dist_max;
                v43.x = v83.x * v13;
                v43.y = v83.y * v13;
                v43.z = v83.z * v13;
                MATH_VecAddVec(&v112, &v127, &v43);
                v76 = v112;
            }
        }
        else
        {
            v18 = (tGameObjectMain*)v124->WP_pos_combat;
            if (v18)
            {
                if ((unsigned int)v18 <= 2)
                    v18 = AI_MainActor[(uint32_t)v18 - 1];
                AI_EvalFunc_OBJGetPos_C(v18, &v73);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v73);
            }
            v76 = v73;
        }
    }
    v6 = v124->f_dt * 4.0f;
    if (v6 < 0.0)
        v6 = 0.0;
    if (v6 > 1.0)
        v6 = 1.0;
    v124->f_blend_current = v124->f_retard * v6 + v124->f_blend_current * (1.0f - v6);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v114);
    Xa = v124->f_dt * v124->f_blend_current;
    AI_EvalFunc_MATHVecBlend_C(&v114, &v76, Xa, &a4);
    v67 = a4;
    if (v124->f_deplacement_speed_max != 0.0)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v62);
        MATH_SubstractVec(&v80, &v67, &v62);
        v94 = MATH_VecLen(&v80);
        v50 = TIM_gf_dt;
        if (v124->f_deplacement_speed_max * TIM_gf_dt < v94)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v44);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v93);
            MATH_SubstractVec(&v87, &v67, &v93);
            AI_EvalFunc_MATHVecNormalize_C(&v87, &v131);
            v130 = TIM_gf_dt;
            v5 = v124->f_deplacement_speed_max * TIM_gf_dt;
            v68.x = v131.x * v5;
            v68.y = v131.y * v5;
            v68.z = v131.z * v5;
            MATH_VecAddVec(&v126, &v44, &v68);
            v67 = v126;
        }
    }
    AI_EvalFunc_OBJSetPos_C_CURRENT(&v67);
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 4);
    return 1000;
}

int PNJ_Spectre_sp_mode_apparition_anim()
{
    float v1; // [esp+0h] [ebp-1C8h]
    tScriptStruct_PNJSpectre* v2; // [esp+10h] [ebp-1B8h]
    tGameObjectMain* v3; // [esp+18h] [ebp-1B0h]
    tGameObjectMain* v4; // [esp+1Ch] [ebp-1ACh]
    tGameObjectMain* v5; // [esp+20h] [ebp-1A8h]
    tGameObjectMain* v6; // [esp+28h] [ebp-1A0h]
    tGameObjectMain* v7; // [esp+30h] [ebp-198h]
    tGameObjectMain* v8; // [esp+34h] [ebp-194h]
    tGameObjectMain* v9; // [esp+38h] [ebp-190h]
    tGameObjectMain* v10; // [esp+3Ch] [ebp-18Ch]
    tGameObjectMain* v11; // [esp+A4h] [ebp-124h]
    tGameObjectMain* v12; // [esp+A8h] [ebp-120h]
    tGameObjectMain* v13; // [esp+ACh] [ebp-11Ch]
    tGameObjectMain* v14; // [esp+B0h] [ebp-118h]
    float v15; // [esp+DCh] [ebp-ECh]
    Vec3 a3; // [esp+F0h] [ebp-D8h] BYREF
    BOOL v18; // [esp+104h] [ebp-C4h]
    BOOL v19; // [esp+108h] [ebp-C0h]
    Vec3 v20; // [esp+10Ch] [ebp-BCh] BYREF
    Vec3 v22; // [esp+120h] [ebp-A8h] BYREF
    int v23; // [esp+12Ch] [ebp-9Ch]
    int v24; // [esp+134h] [ebp-94h]
    BOOL v25; // [esp+13Ch] [ebp-8Ch]
    int v26; // [esp+140h] [ebp-88h]
    tGameObjectMain* v28; // [esp+14Ch] [ebp-7Ch]
    int v29; // [esp+150h] [ebp-78h]
    Vec3 v30; // [esp+154h] [ebp-74h] BYREF
    int v32; // [esp+164h] [ebp-64h]
    tGameObjectMain* v33; // [esp+168h] [ebp-60h]
    int v34; // [esp+16Ch] [ebp-5Ch]
    int v35; // [esp+174h] [ebp-54h]
    Vec3 v37; // [esp+180h] [ebp-48h] BYREF
    tScriptStruct_PNJSpectre* v38; // [esp+18Ch] [ebp-3Ch]
    Vec3 a1; // [esp+190h] [ebp-38h] BYREF
    Vec3 v40; // [esp+19Ch] [ebp-2Ch] BYREF
    int v41; // [esp+1ACh] [ebp-1Ch]
    Vec3 a4; // [esp+1B4h] [ebp-14h] BYREF

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJSpectre*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v38 = v2;
    v32 = 0;
    v23 = 0;
    v24 = 0;
    v34 = 0;
    v26 = 0;
    v35 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao = AI_ul_RealResolveOneRefObject(0x9E002FFA);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ao_Evil_Actor,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            520);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_NRJ_Courante,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            892);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_i_Evil_NbActor,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            2624);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_i_Evil_NbActorAlive,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            2632);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_NRJShieldMax,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            768);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_o_last_mort,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            3788);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ao_Good_Actor,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            0);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ao_Evil_Target,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            644);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ai_Good_Targeted,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            416);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ai_Evil_Targeted,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            1636);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ao_Good_Target,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            52);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_Dmin,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            1884);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_DMax,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            2008);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_Angle,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            2132);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ai_Evil_Icone,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            1760);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_Last_Hit,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            1016);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ao_Evil_Last_Good,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            1140);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_Last_Dmg,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            1264);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_Last_Eff,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            1388);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_Cumul_Dmg,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            1512);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_o_musique_fight,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao,
            2660);
        PNJ_Spectre_sp_mode_apparition_anim_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_anim_GST_Global_gao_o_musique_fight_list,
            (void*)PNJ_Spectre_sp_mode_apparition_anim_GST_Global_gao,
            3460);
        fct_PNJ_Spectre_sp_mode_apparition_anim_PNJ_Spectre_sp_reflex = (int)AI_ul_RealResolveOneRefFunction(0x9E006692);
        fct_PNJ_Spectre_sp_mode_apparition_anim_PNJ_Spectre_sp_mode_combat = (int)AI_ul_RealResolveOneRefFunction(0x9E006691);
        fct_PNJ_Spectre_sp_mode_apparition_anim_PNJ_Spectre_sp_after_blend = (int)AI_ul_RealResolveOneRefFunction(0x9E00669B);
    }
    v38->i_mode = 2;
    v24 = 0;
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(2) && !v38->i_fight_on)
    {
        v38->i_fight_on = 1;
        AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(0x1000, 0);
        v26 = AI_EvalFunc_ARRObjSearch_C(
            (tGameObjectMain**)(PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ao_Evil_Actor + 4),
            30,
            0);
        if (v26 != -1)
        {
            v28 = AI_gpst_CurrentGameObject;
            v14 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v14 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v13 = v14;
            }
            else
            {
                v13 = 0;
            }
            v32 = v13->extended->fields->designFlags & 0x7FFFFFFF;
            v12 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v12 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v11 = v12;
            }
            else
            {
                v11 = 0;
            }
            v11->extended->fields->designFlags = v32;
            *(uint32_t*)(PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ao_Evil_Actor + 4 * v26 + 4) = (uint32_t)AI_gpst_CurrentGameObject;
            *(uint32_t*)AI_EvalVarArray_C2(
                (char*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_NRJ_Courante,
                v26) = 0x42A00000;
            if (!*(uint32_t*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_i_Evil_NbActor)
                *(uint32_t*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_i_Evil_NbActorAlive = 0;
            ++* (uint32_t*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_i_Evil_NbActorAlive;
            ++* (uint32_t*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_i_Evil_NbActor;
            *(uint32_t*)AI_EvalVarArray_C2(
                (char*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_NRJShieldMax,
                v26) = 0x42A00000;
        }
        *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_Dmin, v26) = 0x40400000;
        *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_DMax, v26) = 0x40C00000;
        *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_Angle, v26) = 0x40490FDC;
        *(uint32_t*)(PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ai_Evil_Icone + 4 * v26 + 4) = 9;
        *(uint32_t*)AI_EvalVarArray_C2(
            (char*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_Last_Hit,
            v26) = 0;
        *(uint32_t*)(PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_ao_Evil_Last_Good + 4 * v26 + 4) = 0;
        *(uint32_t*)AI_EvalVarArray_C2(
            (char*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_Last_Dmg,
            v26) = 0;
        *(uint32_t*)AI_EvalVarArray_C2(
            (char*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_Last_Eff,
            v26) = 0;
        *(uint32_t*)AI_EvalVarArray_C2(
            (char*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_af_Evil_Cumul_Dmg,
            v26) = 0;
        v10 = (tGameObjectMain*)v38->acteur_musique_fight;
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
            v8 = (tGameObjectMain*)v38->acteur_musique_fight;
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
            if (v7 == (tGameObjectMain*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao)
                *(uint32_t*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_o_musique_fight = *(uint32_t*)(PNJ_Spectre_sp_mode_apparition_anim_GST_Global_gao_o_musique_fight_list + 44);
            else
                *(uint32_t*)PNJ_Spectre_sp_mode_apparition_anim_GST_Fight_Manager_gao_o_musique_fight = v38->acteur_musique_fight;
        }
    }
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(4))
    {
        if (v38->i_position_start == 3 || (v18 = AI_EvalFunc_ACTActionFinished_C_CURRENT()))
        {
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(1, 4);
            v38->f_time = TIM_gf_MainClock - TIM_gf_LockTime;
        }
        else if (v38->i_apparition_etape != 1)
        {
            v38->i_apparition_etape = 1;
            AI_EvalFunc_ACTActionSet_C_CURRENT(10 * v38->i_position_start + 4);
        }
    }
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(1))
    {
        if (!AI_EvalFunc_OBJ_CapaTest_C_CURRENT(4))
        {
            if (v38->i_position_start == 3)
            {
                if (AI_EvalFunc_TIMEElapsed_C(v38->f_time, 1.5))
                {
                    if (AI_EvalFunc_TIMEElapsed_C(v38->f_time, 2.0))
                        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(16, 1);
                }
                else
                {
                    AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &v37);
                    AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &v20);
                    v22.x = 0.0;
                    v22.y = 0.0;
                    v22.z = v38->f_altitude_offset;
                    MATH_VecAddVec(&a1, &v20, &v22);
                    v15 = TIM_gf_MainClock - TIM_gf_LockTime;
                    v1 = (v15 - v38->f_time) / 1.0f;
                    AI_EvalFunc_MATHVecBlend_C(&v37, &a1, v1, &a4);
                    AI_EvalFunc_OBJSetPos_C_CURRENT(&a4);
                }
            }
            else
            {
                v29 = AI_EvalFunc_ACTActionGet_C_CURRENT();
                if (v29 == 10 * v38->i_position_start + 1)
                {
                    if (AI_EvalFunc_ACTActionFinished_C_CURRENT())
                    {
                        AI_EvalFunc_ACTActionSet_C_CURRENT(10 * v38->i_position_start + 2);
                        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 1);
                    }
                }
                else
                {
                    AI_EvalFunc_ACTActionSet_C_CURRENT(10 * v38->i_position_start + 1);
                    v6 = (tGameObjectMain*)v38->WP_init_combat;
                    if (v6)
                    {
                        if ((unsigned int)v6 <= 2)
                            v6 = AI_MainActor[(uint32_t)v6 - 1];
                        AI_EvalFunc_OBJGetPos_C(v6, &a3);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &a3);
                    }
                    AI_EvalFunc_OBJSetPos_C_CURRENT(&a3);
                    v30.x = 0.0;
                    v30.y = 0.0;
                    v30.z = 0.0;
                    AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v30);
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v40);
                    AI_EvalFunc_COLStartMatrixSet_C(AI_gpst_CurrentGameObject, &v40);
                }
            }
        }
    }
    if (!AI_EvalFunc_OBJ_CapaTest_C_CURRENT(1))
    {
        if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(2))
        {
            if (!AI_EvalFunc_OBJ_CapaTest_C_CURRENT(4))
            {
                if (v38->i_position_start != 3)
                {
                    v41 = AI_EvalFunc_ACTActionGet_C_CURRENT();
                    if (v41 != 10 * v38->i_position_start + 3)
                        AI_EvalFunc_ACTActionSet_C_CURRENT(10 * v38->i_position_start + 3);
                }
                v25 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x10);
                if (v25 || (v19 = AI_EvalFunc_ACTActionFinished_C_CURRENT()))
                {
                    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x17);
                    v38->f_blend_current = 0.0;
                    AI_EvalFunc_ACTActionSet_C_CURRENT(0);
                    v5 = (tGameObjectMain*)v38->o_me;
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
                    v4->extended->fields->designFlags = 0;
                    AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 2, 0);
                    AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_PNJ_Spectre_sp_mode_apparition_anim_PNJ_Spectre_sp_reflex);
                    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_PNJ_Spectre_sp_mode_apparition_anim_PNJ_Spectre_sp_mode_combat);
                    v33 = AI_gpst_CurrentGameObject;
                    v3 = AI_gpst_CurrentGameObject;
                    if (AI_gpst_CurrentGameObject)
                    {
                        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                            v3 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                        AI_EvalFunc_AICBAdd_C(
                            AI_gpst_CurrentGameObject,
                            v3,
                            3,
                            (tAIFunction*)fct_PNJ_Spectre_sp_mode_apparition_anim_PNJ_Spectre_sp_after_blend);
                    }
                    else
                    {
                        AI_EvalFunc_AICBAdd_C(
                            AI_gpst_CurrentGameObject,
                            0,
                            3,
                            (tAIFunction*)fct_PNJ_Spectre_sp_mode_apparition_anim_PNJ_Spectre_sp_after_blend);
                    }
                }
            }
        }
    }
    return 1000;
}

int PNJ_Spectre_sp_reflex()
{
    float* v0; // eax
    BOOL v1; // eax
    double v2; // st7
    tGameObjectMain* v3; // eax
    tGameObjectMain* v4; // eax
    int result; // eax
    float v6; // [esp+4h] [ebp-310h]
    float v7; // [esp+8h] [ebp-30Ch]
    float v8; // [esp+Ch] [ebp-308h]
    float v9; // [esp+10h] [ebp-304h]
    tScriptStruct_PNJSpectre* v10; // [esp+1Ch] [ebp-2F8h]
    tGameObjectMain* v11; // [esp+24h] [ebp-2F0h]
    tGameObjectMain* v12; // [esp+50h] [ebp-2C4h]
    tGameObjectMain* v13; // [esp+54h] [ebp-2C0h]
    tGameObjectMain* v14; // [esp+58h] [ebp-2BCh]
    tGameObjectMain* v15; // [esp+68h] [ebp-2ACh]
    tAI* v16; // [esp+6Ch] [ebp-2A8h]
    tGameObjectMain* v17; // [esp+70h] [ebp-2A4h]
    tGameObjectMain* v18; // [esp+74h] [ebp-2A0h]
    tGameObjectMain* v19; // [esp+7Ch] [ebp-298h]
    tGameObjectMain* v20; // [esp+84h] [ebp-290h]
    tAI* v21; // [esp+88h] [ebp-28Ch]
    tGameObjectMain* v22; // [esp+8Ch] [ebp-288h]
    tGameObjectMain* v23; // [esp+90h] [ebp-284h]
    tGameObjectMain* v24; // [esp+98h] [ebp-27Ch]
    tGameObjectMain* v25; // [esp+A0h] [ebp-274h]
    tGameObjectMain* v26; // [esp+ACh] [ebp-268h]
    tGameObjectMain* v27; // [esp+B4h] [ebp-260h]
    tGameObjectMain* v28; // [esp+B8h] [ebp-25Ch]
    tGameObjectMain* v29; // [esp+BCh] [ebp-258h]
    tGameObjectMain* v30; // [esp+C0h] [ebp-254h]
    tGameObjectMain* v31; // [esp+128h] [ebp-1ECh]
    tGameObjectMain* v32; // [esp+12Ch] [ebp-1E8h]
    tGameObjectMain* v33; // [esp+130h] [ebp-1E4h]
    tGameObjectMain* v34; // [esp+134h] [ebp-1E0h]
    tGameObjectMain* v35; // [esp+148h] [ebp-1CCh]
    tGameObjectMain* v36; // [esp+14Ch] [ebp-1C8h]
    tGameObjectMain* v37; // [esp+150h] [ebp-1C4h]
    tGameObjectMain* v38; // [esp+154h] [ebp-1C0h]
    tGameObjectMain* v39; // [esp+158h] [ebp-1BCh]
    tGameObjectMain* v40; // [esp+15Ch] [ebp-1B8h]
    tGameObjectMain* v41; // [esp+160h] [ebp-1B4h]
    tGameObjectMain* v42; // [esp+164h] [ebp-1B0h]
    tGameObjectMain* v43; // [esp+168h] [ebp-1ACh]
    tGameObjectMain* v44; // [esp+16Ch] [ebp-1A8h]
    tGameObjectMain* v45; // [esp+170h] [ebp-1A4h]
    tGameObjectMain* v46; // [esp+174h] [ebp-1A0h]
    tGameObjectMain* v47; // [esp+178h] [ebp-19Ch]
    tGameObjectMain* v48; // [esp+17Ch] [ebp-198h]
    tGameObjectMain* v49; // [esp+180h] [ebp-194h]
    int v50; // [esp+194h] [ebp-180h]
    Vec3 v51; // [esp+19Ch] [ebp-178h] BYREF
    int v52; // [esp+1A8h] [ebp-16Ch]
    float v53; // [esp+1ACh] [ebp-168h]
    Vec3 v54; // [esp+1B4h] [ebp-160h] BYREF
    Vec3 v55; // [esp+1C0h] [ebp-154h] BYREF
    tGameObjectMain* v56; // [esp+1D4h] [ebp-140h]
    int v57; // [esp+1D8h] [ebp-13Ch]
    int v58; // [esp+1E8h] [ebp-12Ch]
    int v59; // [esp+1ECh] [ebp-128h]
    float v60; // [esp+204h] [ebp-110h]
    int v62; // [esp+210h] [ebp-104h]
    uint32_t v63[5]; // [esp+214h] [ebp-100h] BYREF
    float v64; // [esp+228h] [ebp-ECh]
    //int v65; // [esp+22Ch] [ebp-E8h]
    tGameObjectMain* a7[10]; // [esp+230h] [ebp-E4h] BYREF
    BOOL v67; // [esp+258h] [ebp-BCh]
    BOOL v68; // [esp+25Ch] [ebp-B8h]
    int v69; // [esp+264h] [ebp-B0h]
    void* Src; // [esp+270h] [ebp-A4h]
    int v72; // [esp+280h] [ebp-94h]
    Vec3 v; // [esp+284h] [ebp-90h] BYREF
    Vec3 pos; // [esp+290h] [ebp-84h] BYREF
    BOOL v75; // [esp+29Ch] [ebp-78h]
    float v76; // [esp+2A0h] [ebp-74h]
    tAI* v77; // [esp+2A4h] [ebp-70h]
    Vec3 v78; // [esp+2ACh] [ebp-68h] BYREF
    Vec3 a3; // [esp+2B8h] [ebp-5Ch] BYREF
    Vec3 v80; // [esp+2C4h] [ebp-50h] BYREF
    tScriptStruct_PNJSpectre* v82; // [esp+2D4h] [ebp-40h]
    int v83; // [esp+2D8h] [ebp-3Ch]
    Vec3 dest; // [esp+2E0h] [ebp-34h] BYREF
    tGameObjectMain* v85; // [esp+2ECh] [ebp-28h]
    int v86; // [esp+2F8h] [ebp-1Ch]
    int v88; // [esp+308h] [ebp-Ch]
    void* v89; // [esp+30Ch] [ebp-8h]
    int v90; // [esp+310h] [ebp-4h]

    v.x = 1.0;
    v.y = 0.0;
    v.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v10 = (tScriptStruct_PNJSpectre*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v10 = 0;
    v82 = v10;
    v67 = 0;
    v52 = 0;
    v60 = 0.0;
    v68 = 0;
    v64 = 0.0;
    v88 = 0;
    v59 = 0;
    v69 = 0;
    v77 = 0;
    v83 = 0;
    v53 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        PNJ_Spectre_sp_reflex_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&PNJ_Spectre_sp_reflex_univVars_Perso_ID_Companion, (void*)PNJ_Spectre_sp_reflex_univVars, 1040);
        PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao = AI_ul_RealResolveOneRefObject(0x9E002FFA);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Good_Last_Paf,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            312);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ao_Evil_Actor,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            520);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_NRJ_Courante,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            892);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_i_Evil_NbActor,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            2624);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_i_Evil_NbActorAlive,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            2632);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_NRJShieldMax,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            768);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_o_last_mort,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            3788);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ao_Good_Actor,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            0);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ao_Evil_Target,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            644);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ai_Good_Targeted,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            416);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ai_Evil_Targeted,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            1636);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ao_Good_Target,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            52);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_Dmin,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            1884);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_DMax,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            2008);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_Angle,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            2132);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ai_Evil_Icone,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            1760);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_Last_Hit,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            1016);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ao_Evil_Last_Good,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            1140);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_Last_Dmg,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            1264);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_Last_Eff,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            1388);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_Cumul_Dmg,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            1512);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_o_musique_fight,
            (void*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao,
            2660);
        PNJ_Spectre_sp_reflex_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_GST_Global_gao_o_musique_fight_list,
            (void*)PNJ_Spectre_sp_reflex_GST_Global_gao,
            3460);
        PNJ_Spectre_sp_reflex__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex__PJ_Sally_gao_current_pm_mode,
            (void*)PNJ_Spectre_sp_reflex__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex__PJ_Sally_gao_current_pm_priority,
            (void*)PNJ_Spectre_sp_reflex__PJ_Sally_gao,
            576);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex__PJ_Sally_gao_i_Fight_list_nbr,
            (void*)PNJ_Spectre_sp_reflex__PJ_Sally_gao,
            3876);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex__PJ_Sally_gao_ao_Fight_list_of_actors,
            (void*)PNJ_Spectre_sp_reflex__PJ_Sally_gao,
            3504);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex__PJ_Sally_gao_af_Fight_list_of_distances,
            (void*)PNJ_Spectre_sp_reflex__PJ_Sally_gao,
            3628);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex__PJ_Sally_gao_ai_Fight_list_of_bits,
            (void*)PNJ_Spectre_sp_reflex__PJ_Sally_gao,
            3752);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex_univVars_Flag_speciaux_sauve,
            (void*)PNJ_Spectre_sp_reflex_univVars,
            4488);
        fct_PNJ_Spectre_sp_reflex_PNJ_Spectre_sp_mode_mort = (int)AI_ul_RealResolveOneRefFunction(0x9E006695);
        fct_PNJ_Spectre_sp_reflex_PNJ_Spectre_sp_exec_decode_paf_univ = (int)AI_ul_RealResolveOneRefFunction(0x9E006697);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_reflex__PJ_Sally_gao_v_fight_kick_axis_non_tourne,
            (void*)PNJ_Spectre_sp_reflex__PJ_Sally_gao,
            3940);
    }
    v82->f_dt = TIM_gf_dt;
    if (v82->i_NMI == 1)
    {
        v49 = (tGameObjectMain*)v82->o_me;
        if (v49)
        {
            if ((unsigned int)v49 <= 2)
                v49 = AI_MainActor[(uint32_t)v49 - 1];
            v48 = v49;
        }
        else
        {
            v48 = 0;
        }
        v82->i_flag_order = v48->extended->fields->designInt1;
    }
    else
    {
        v82->i_flag_order = 64;
        v47 = (tGameObjectMain*)v82->o_me;
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
        v59 = v46->extended->fields->designFlags & 0x3FFFFFFF | 0x8000000;
        v45 = (tGameObjectMain*)v82->o_me;
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
        v44->extended->fields->designFlags = v59;
        if (AI_EvalFunc_TIMEElapsed_C(v82->f_time_last_attak, v82->f_Compagnon_delai_attaque))
        {
            v43 = (tGameObjectMain*)v82->o_me;
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
            v59 = v42->extended->fields->designFlags | 0xC0000000;
            v41 = (tGameObjectMain*)v82->o_me;
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
            v40->extended->fields->designFlags = v59;
            v82->i_flag_order = 8;
        }
        v6 = v82->f_Compagnon_delai_touche;
        v0 = (float*)AI_EvalVarArray_C2(
            (char*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Good_Last_Paf,
            *(uint32_t*)PNJ_Spectre_sp_reflex_univVars_Perso_ID_Companion);
        if (AI_EvalFunc_TIMEElapsed_C(*v0, v6))
        {
            v82->i_flag_order = 8;
            v39 = (tGameObjectMain*)v82->o_me;
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
            v59 = v38->extended->fields->designFlags & 0xBFFFFFFF | 0x80000000;
            v37 = (tGameObjectMain*)v82->o_me;
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
            v36->extended->fields->designFlags = v59;
        }
    }
    v35 = (tGameObjectMain*)v82->o_NMI;
    if (v35)
    {
        if ((unsigned int)v35 <= 2)
            v35 = AI_MainActor[(uint32_t)v35 - 1];
        v1 = AI_EvalFunc_OBJ_CapaTest_C(v35, 512);
    }
    else
    {
        v1 = AI_EvalFunc_OBJ_CapaTest_C(0, 512);
    }
    if (v1)
        v82->f_time_paf_sally = TIM_gf_MainClock - TIM_gf_LockTime;
    if (!v82->i_fight_on)
    {
        v82->i_fight_on = 1;
        AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(4096, 0);
        v59 = AI_EvalFunc_ARRObjSearch_C(
            (tGameObjectMain**)(PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ao_Evil_Actor + 4),
            30,
            0);
        if (v59 != -1)
        {
            v85 = AI_gpst_CurrentGameObject;
            v34 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v34 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v33 = v34;
            }
            else
            {
                v33 = 0;
            }
            v50 = v33->extended->fields->designFlags & 0x7FFFFFFF;
            v56 = AI_gpst_CurrentGameObject;
            v32 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v32 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v31 = v32;
            }
            else
            {
                v31 = 0;
            }
            v31->extended->fields->designFlags = v50;
            *(uint32_t*)(PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ao_Evil_Actor + 4 * v59 + 4) = (uint32_t)AI_gpst_CurrentGameObject;
            *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_NRJ_Courante, v59) = 0x42A00000;
            if (!*(uint32_t*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_i_Evil_NbActor)
                *(uint32_t*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_i_Evil_NbActorAlive = 0;
            ++* (uint32_t*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_i_Evil_NbActorAlive;
            ++* (uint32_t*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_i_Evil_NbActor;
            *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_NRJShieldMax, v59) = 0x42A00000;
        }
        *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_Dmin, v59) = 0x40400000;
        *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_DMax, v59) = 0x40C00000;
        *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_Angle, v59) = 0x40490FDC;
        *(uint32_t*)(PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ai_Evil_Icone + 4 * v59 + 4) = 9;
        *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_Last_Hit, v59) = 0;
        *(uint32_t*)(PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_ao_Evil_Last_Good + 4 * v59 + 4) = 0;
        *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_Last_Dmg, v59) = 0;
        *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_Last_Eff, v59) = 0;
        *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_af_Evil_Cumul_Dmg, v59) = 0;
        v30 = (tGameObjectMain*)v82->acteur_musique_fight;
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
        if (v29)
        {
            v28 = (tGameObjectMain*)v82->acteur_musique_fight;
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
            if (v27 == (tGameObjectMain*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao)
                *(uint32_t*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_o_musique_fight = *(uint32_t*)(PNJ_Spectre_sp_reflex_GST_Global_gao_o_musique_fight_list
                    + 44);
            else
                *(uint32_t*)PNJ_Spectre_sp_reflex_GST_Fight_Manager_gao_o_musique_fight = v82->acteur_musique_fight;
        }
    }
    v26 = (tGameObjectMain*)v82->o_NMI;
    if (v26)
    {
        if ((unsigned int)v26 <= 2)
            v26 = AI_MainActor[(uint32_t)v26 - 1];
        v2 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v26);
    }
    else
    {
        v2 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
    }
    v82->f_distance_actuelle = (float)v2;
    if (v82->f_distance_actuelle < 144.0)
    {
        v72 = (unsigned __int16)*(uint32_t*)(PNJ_Spectre_sp_reflex__PJ_Sally_gao + 4);
        if ((v72 & 1) != 0)
            v68 = !*(uint32_t*)PNJ_Spectre_sp_reflex__PJ_Sally_gao_current_pm_mode
            || *(int*)PNJ_Spectre_sp_reflex__PJ_Sally_gao_current_pm_priority < 95;
        else
            v68 = 0;
        if (v68)
        {
            AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(4096, 0);
            *(uint32_t*)(PNJ_Spectre_sp_reflex__PJ_Sally_gao_ao_Fight_list_of_actors
                + 4 * *(uint32_t*)PNJ_Spectre_sp_reflex__PJ_Sally_gao_i_Fight_list_nbr
                + 4) = v82->o_me;
            *(uint32_t*)AI_EvalVarArray_C2(
                (char*)PNJ_Spectre_sp_reflex__PJ_Sally_gao_af_Fight_list_of_distances,
                *(uint32_t*)PNJ_Spectre_sp_reflex__PJ_Sally_gao_i_Fight_list_nbr) = 0x43800000;
            *(uint32_t*)(PNJ_Spectre_sp_reflex__PJ_Sally_gao_ai_Fight_list_of_bits
                + 4 * (*(uint32_t*)PNJ_Spectre_sp_reflex__PJ_Sally_gao_i_Fight_list_nbr)++
                + 4) = 0;
        }
    }
    if (v82->SF_OFF <= 0)
        v67 = 0;
    else
        v67 = AI_EvalFunc_ARRBitTest_C((int*)(PNJ_Spectre_sp_reflex_univVars_Flag_speciaux_sauve + 4), v82->SF_OFF);
    if (v67
        && (v58 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(AI_gpst_CurrentGameObject, 7, -1.0, 0.94999999f, 1.05f),
            v82->i_paf_type >= 0))
    {
        if (v82->i_brasD_on)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
            v25 = (tGameObjectMain*)v82->des_bras_droit;
            if (v82->des_bras_droit)
            {
                if ((unsigned int)v25 <= 2)
                    v25 = AI_MainActor[(uint32_t)v25 - 1];
                v3 = AI_EvalFunc_OBJDuplicate_C(v25, &pos);
            }
            else
            {
                v3 = AI_EvalFunc_OBJDuplicate_C(0, &pos);
            }
            v77 = (tAI*)v3;
            v24 = v3;
            if (v3)
            {
                if ((unsigned int)v3 <= 2)
                    v24 = AI_MainActor[(uint32_t)v3 - 1];
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(v24, 1);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
            }
            if (v77 == WOR_gst_Universe.univAIInstance)
            {
                v21 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v23 = (tGameObjectMain*)v77;
                if (v77)
                {
                    if ((unsigned int)v77 <= 2)
                        v23 = AI_MainActor[(uint32_t)v77 - 1];
                    v22 = v23;
                }
                else
                {
                    v22 = 0;
                }
                v21 = v22->extended->AIInstance;
            }
            ((tScriptStruct_PNJSectreBras*)(v21->instanceVarDataCurrent))->des_papa = (int)AI_gpst_CurrentGameObject;
            v82->i_brasD_on = 0;
        }
        if (v82->i_brasG_on)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT((Vec3*)v63);
            v20 = (tGameObjectMain*)v82->des_bras_gauche;
            if (v20)
            {
                if ((unsigned int)v20 <= 2)
                    v20 = AI_MainActor[(uint32_t)v20 - 1];
                v4 = AI_EvalFunc_OBJDuplicate_C(v20, (Vec3*)v63);
            }
            else
            {
                v4 = AI_EvalFunc_OBJDuplicate_C(0, (Vec3*)v63);
            }
            v77 = (tAI*)v4;
            v19 = v4;
            if (v4)
            {
                if ((unsigned int)v4 <= 2)
                    v19 = AI_MainActor[(uint32_t)v4 - 1];
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(v19, 1);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
            }
            if (v77 == WOR_gst_Universe.univAIInstance)
            {
                v16 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v18 = (tGameObjectMain*)v77;
                if (v77)
                {
                    if ((unsigned int)v77 <= 2)
                        v18 = AI_MainActor[(uint32_t)v77 - 1];
                    v17 = v18;
                }
                else
                {
                    v17 = 0;
                }
                v16 = v17->extended->AIInstance;
            }
            ((tScriptStruct_PNJSectreBras*)(v16->instanceVarDataCurrent))->des_papa = (int)AI_gpst_CurrentGameObject;
            v82->i_brasG_on = 0;
        }
        AI_EvalFunc_AITrackChange_C(2, (tAIFunction*)fct_PNJ_Spectre_sp_reflex_PNJ_Spectre_sp_mode_mort);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        Src = AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v82->t_mes, Src, sizeof(v82->t_mes));
        while (v82->t_mes.msgID)
        {
            if (v82->t_mes.msgID == 37 && v82->i_mode != 2)
            {
                v86 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_PNJ_Spectre_sp_reflex_PNJ_Spectre_sp_exec_decode_paf_univ);
                if (v86 != 1000)
                    return v86;
                v82->f_frappe_dernier = TIM_gf_MainClock - TIM_gf_LockTime;
            }
            v89 = AI_EvalFunc_MSGGet_C_CURRENT();
            memcpy(&v82->t_mes, v89, sizeof(v82->t_mes));
        }
        v82->t_mes.msgID = 0;
        v75 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(8);
        if (v75 && AI_EvalFunc_TIMEElapsed_C(v82->f_frappe_dernier, 1.0))
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v55);
            v15 = (tGameObjectMain*)v82->o_NMI;
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
            MATH_SubstractVec(&dest, &v55, &a3);
            v = dest;
            AI_EvalFunc_MATHVecSetHorzNormalize_C(&v);
            v88 = AI_EvalFunc_COLZDEZDEListGet_C(AI_gpst_CurrentGameObject, a7, 6, 1, 1, 0, 1);
            if (v88 > 0)
            {
                v14 = (tGameObjectMain*)v82->o_NMI;
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
                if (v13 == (tGameObjectMain*)PNJ_Spectre_sp_reflex__PJ_Sally_gao)
                {
                    if (AI_EvalFunc_ACTDesignFlagsTest_C((tGameObjectMain*)PNJ_Spectre_sp_reflex__PJ_Sally_gao, 0x4000u))
                    {
                        v76 = v.z * *(float*)(PNJ_Spectre_sp_reflex__PJ_Sally_gao_v_fight_kick_axis_non_tourne + 8)
                            + v.y * *(float*)(PNJ_Spectre_sp_reflex__PJ_Sally_gao_v_fight_kick_axis_non_tourne + 4)
                            + v.x * *(float*)PNJ_Spectre_sp_reflex__PJ_Sally_gao_v_fight_kick_axis_non_tourne;
                        if (v76 >= 0.93969297)
                        {
                            v57 = AI_EvalFunc_ANIFrequencyGet_C((tGameObjectMain*)PNJ_Spectre_sp_reflex__PJ_Sally_gao, 0);
                            v9 = (float)v57;
                            v64 = v9 / 60.0f;
                            v62 = AI_EvalFunc_ANINbFrameGet_C((tGameObjectMain*)PNJ_Spectre_sp_reflex__PJ_Sally_gao, 0);
                            v8 = (float)v62;
                            v53 = v8 * v64;
                            v90 = AI_EvalFunc_ANICurrentFrameGet_C((tGameObjectMain*)PNJ_Spectre_sp_reflex__PJ_Sally_gao, 0);
                            v7 = (float)v90;
                            v60 = v7 * v64;
                            if (v60 / v53 > 0.5)
                                v88 = 0;
                        }
                    }
                }
                while (v88 > 0)
                {
                    v12 = a7[v88 - 1];
                    if (v12)
                    {
                        if ((unsigned int)v12 <= 2)
                            v12 = AI_MainActor[(uint32_t)v12 - 1];
                        AI_EvalFunc_OBJGetPos_C(v12, &v80);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v80);
                    }
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v51);
                    MATH_SubstractVec(&v78, &v80, &v51);
                    v.x = v78.x;
                    v.y = v78.y;
                    v.z = 0.0;
                    if (AI_EvalFunc_MTHVecNullEpsilon_C(&v))
                    {
                        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v54);
                        v = v54;
                    }
                    else
                    {
                        MATH_NormalizeEqualVector(&v);
                    }
                    v82->f_frappe_dernier = TIM_gf_MainClock - TIM_gf_LockTime;
                    v82->t_mes.msgID = 37;
                    v82->t_mes.gao = AI_gpst_CurrentGameObject;
                    v82->t_mes.gao2 = 0;
                    v82->t_mes.Int1 = 3;
                    v82->t_mes.Int2 = 0;
                    v82->t_mes.v1 = v;
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v82->t_mes.v2);
                    v82->t_mes.f2 = 20.0;
                    v82->t_mes.f1 = 1000.0;
                    v11 = a7[v88 - 1];
                    if (v11)
                    {
                        if ((unsigned int)v11 <= 2)
                            v11 = AI_MainActor[(uint32_t)v11 - 1];
                        AI_EvalFunc_MSGSend_C(v11, &v82->t_mes);
                    }
                    else
                    {
                        AI_EvalFunc_MSGSend_C(0, &v82->t_mes);
                    }
                    --v88;
                }
            }
        }
        v82->i_flag_order_old = v82->i_flag_order;
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 8);
        result = 1000;
    }
    return result;
}

int PNJ_Spectre_sp_mode_mort()
{
    BOOL v0; // eax
    BOOL v1; // eax
    int result; // eax
    BOOL v3; // eax
    float v4; // [esp+4h] [ebp-620h]
    float v5; // [esp+4h] [ebp-620h]
    float v6; // [esp+Ch] [ebp-618h]
    float v7; // [esp+10h] [ebp-614h]
    float v8; // [esp+14h] [ebp-610h]
    float v9; // [esp+18h] [ebp-60Ch]
    float v10; // [esp+1Ch] [ebp-608h]
    float v11; // [esp+20h] [ebp-604h]
    tScriptStruct_PNJSpectre* v12; // [esp+34h] [ebp-5F0h]
    tGameObjectMain* v13; // [esp+38h] [ebp-5ECh]
    tGameObjectMain* v14; // [esp+3Ch] [ebp-5E8h]
    tGameObjectMain* v15; // [esp+48h] [ebp-5DCh]
    tGameObjectMain* v16; // [esp+4Ch] [ebp-5D8h]
    tGameObjectMain* v17; // [esp+50h] [ebp-5D4h]
    tGameObjectMain* v18; // [esp+5Ch] [ebp-5C8h]
    tGameObjectMain* v19; // [esp+64h] [ebp-5C0h]
    tAI* v20; // [esp+68h] [ebp-5BCh]
    tGameObjectMain* v21; // [esp+6Ch] [ebp-5B8h]
    tGameObjectMain* v22; // [esp+70h] [ebp-5B4h]
    tAI* v23; // [esp+74h] [ebp-5B0h]
    tGameObjectMain* v24; // [esp+78h] [ebp-5ACh]
    tGameObjectMain* v25; // [esp+7Ch] [ebp-5A8h]
    tAI* v26; // [esp+80h] [ebp-5A4h]
    tGameObjectMain* v27; // [esp+84h] [ebp-5A0h]
    tGameObjectMain* v28; // [esp+88h] [ebp-59Ch]
    tAI* v29; // [esp+8Ch] [ebp-598h]
    tGameObjectMain* v30; // [esp+90h] [ebp-594h]
    tGameObjectMain* v31; // [esp+94h] [ebp-590h]
    tAI* v32; // [esp+98h] [ebp-58Ch]
    tGameObjectMain* v33; // [esp+9Ch] [ebp-588h]
    tGameObjectMain* v34; // [esp+A0h] [ebp-584h]
    tAI* v35; // [esp+A4h] [ebp-580h]
    tGameObjectMain* v36; // [esp+A8h] [ebp-57Ch]
    tGameObjectMain* v37; // [esp+ACh] [ebp-578h]
    tAI* v38; // [esp+B0h] [ebp-574h]
    tGameObjectMain* v39; // [esp+B4h] [ebp-570h]
    tGameObjectMain* v40; // [esp+B8h] [ebp-56Ch]
    tAI* v41; // [esp+BCh] [ebp-568h]
    tGameObjectMain* v42; // [esp+C0h] [ebp-564h]
    tGameObjectMain* v43; // [esp+C4h] [ebp-560h]
    tAI* v44; // [esp+C8h] [ebp-55Ch]
    tGameObjectMain* v45; // [esp+CCh] [ebp-558h]
    tGameObjectMain* v46; // [esp+D0h] [ebp-554h]
    tAI* v47; // [esp+D4h] [ebp-550h]
    tGameObjectMain* v48; // [esp+D8h] [ebp-54Ch]
    tGameObjectMain* v49; // [esp+DCh] [ebp-548h]
    tAI* v50; // [esp+E0h] [ebp-544h]
    tGameObjectMain* v51; // [esp+E4h] [ebp-540h]
    tGameObjectMain* v52; // [esp+E8h] [ebp-53Ch]
    tAI* v53; // [esp+ECh] [ebp-538h]
    tGameObjectMain* v54; // [esp+F0h] [ebp-534h]
    tGameObjectMain* v55; // [esp+F4h] [ebp-530h]
    tAI* v56; // [esp+F8h] [ebp-52Ch]
    tGameObjectMain* v57; // [esp+FCh] [ebp-528h]
    tGameObjectMain* v58; // [esp+100h] [ebp-524h]
    tAI* v59; // [esp+104h] [ebp-520h]
    tGameObjectMain* v60; // [esp+108h] [ebp-51Ch]
    tGameObjectMain* v61; // [esp+10Ch] [ebp-518h]
    tAI* v62; // [esp+110h] [ebp-514h]
    tGameObjectMain* v63; // [esp+114h] [ebp-510h]
    tGameObjectMain* v64; // [esp+118h] [ebp-50Ch]
    tGameObjectMain* v65; // [esp+11Ch] [ebp-508h]
    tGameObjectMain* v66; // [esp+120h] [ebp-504h]
    float v67; // [esp+124h] [ebp-500h]
    float v68; // [esp+12Ch] [ebp-4F8h]
    float v69; // [esp+134h] [ebp-4F0h]
    tGameObjectMain* v70; // [esp+13Ch] [ebp-4E8h]
    tGameObjectMain* v71; // [esp+140h] [ebp-4E4h]
    tGameObjectMain* v72; // [esp+144h] [ebp-4E0h]
    tGameObjectMain* v73; // [esp+14Ch] [ebp-4D8h]
    tGameObjectMain* v74; // [esp+150h] [ebp-4D4h]
    tGameObjectMain* v75; // [esp+154h] [ebp-4D0h]
    tGameObjectMain* v76; // [esp+15Ch] [ebp-4C8h]
    tGameObjectMain* v77; // [esp+164h] [ebp-4C0h]
    tGameObjectMain* v78; // [esp+16Ch] [ebp-4B8h]
    tGameObjectMain* v79; // [esp+174h] [ebp-4B0h]
    float v80; // [esp+178h] [ebp-4ACh]
    tGameObjectMain* v81; // [esp+180h] [ebp-4A4h]
    tAI* v82; // [esp+184h] [ebp-4A0h]
    tGameObjectMain* v83; // [esp+188h] [ebp-49Ch]
    tGameObjectMain* v84; // [esp+18Ch] [ebp-498h]
    tAI* v85; // [esp+190h] [ebp-494h]
    tGameObjectMain* v86; // [esp+194h] [ebp-490h]
    tGameObjectMain* v87; // [esp+198h] [ebp-48Ch]
    tAI* v88; // [esp+19Ch] [ebp-488h]
    tGameObjectMain* v89; // [esp+1A0h] [ebp-484h]
    tGameObjectMain* v90; // [esp+1A4h] [ebp-480h]
    tAI* v91; // [esp+1A8h] [ebp-47Ch]
    tGameObjectMain* v92; // [esp+1ACh] [ebp-478h]
    tGameObjectMain* v93; // [esp+1B0h] [ebp-474h]
    tAI* v94; // [esp+1B4h] [ebp-470h]
    tGameObjectMain* v95; // [esp+1B8h] [ebp-46Ch]
    tGameObjectMain* v96; // [esp+1BCh] [ebp-468h]
    tAI* v97; // [esp+1C0h] [ebp-464h]
    tGameObjectMain* v98; // [esp+1C4h] [ebp-460h]
    tGameObjectMain* v99; // [esp+1C8h] [ebp-45Ch]
    tAI* v100; // [esp+1CCh] [ebp-458h]
    tGameObjectMain* v101; // [esp+1D0h] [ebp-454h]
    tGameObjectMain* v102; // [esp+1D4h] [ebp-450h]
    tAI* v103; // [esp+1D8h] [ebp-44Ch]
    tGameObjectMain* v104; // [esp+1DCh] [ebp-448h]
    tGameObjectMain* v105; // [esp+1E0h] [ebp-444h]
    tAI* v106; // [esp+1E4h] [ebp-440h]
    tGameObjectMain* v107; // [esp+1E8h] [ebp-43Ch]
    tGameObjectMain* v108; // [esp+1ECh] [ebp-438h]
    tAI* v109; // [esp+1F0h] [ebp-434h]
    tGameObjectMain* v110; // [esp+1F4h] [ebp-430h]
    tGameObjectMain* v111; // [esp+1F8h] [ebp-42Ch]
    tAI* v112; // [esp+1FCh] [ebp-428h]
    tGameObjectMain* v113; // [esp+200h] [ebp-424h]
    tGameObjectMain* v114; // [esp+204h] [ebp-420h]
    tAI* v115; // [esp+208h] [ebp-41Ch]
    tGameObjectMain* v116; // [esp+20Ch] [ebp-418h]
    tGameObjectMain* v117; // [esp+210h] [ebp-414h]
    tAI* v118; // [esp+214h] [ebp-410h]
    tGameObjectMain* v119; // [esp+218h] [ebp-40Ch]
    tGameObjectMain* v120; // [esp+21Ch] [ebp-408h]
    tAI* v121; // [esp+220h] [ebp-404h]
    tGameObjectMain* v122; // [esp+224h] [ebp-400h]
    tGameObjectMain* v123; // [esp+228h] [ebp-3FCh]
    tAI* v124; // [esp+22Ch] [ebp-3F8h]
    tGameObjectMain* v125; // [esp+230h] [ebp-3F4h]
    tGameObjectMain* v126; // [esp+234h] [ebp-3F0h]
    tGameObjectMain* v127; // [esp+238h] [ebp-3ECh]
    tGameObjectMain* v128; // [esp+23Ch] [ebp-3E8h]
    float v129; // [esp+240h] [ebp-3E4h]
    float v130; // [esp+248h] [ebp-3DCh]
    float v131; // [esp+250h] [ebp-3D4h]
    tGameObjectMain* v132; // [esp+258h] [ebp-3CCh]
    tGameObjectMain* v133; // [esp+25Ch] [ebp-3C8h]
    tGameObjectMain* v134; // [esp+260h] [ebp-3C4h]
    tGameObjectMain* v135; // [esp+268h] [ebp-3BCh]
    tGameObjectMain* v136; // [esp+26Ch] [ebp-3B8h]
    tGameObjectMain* v137; // [esp+270h] [ebp-3B4h]
    tGameObjectMain* v138; // [esp+274h] [ebp-3B0h]
    tGameObjectMain* v139; // [esp+27Ch] [ebp-3A8h]
    tGameObjectMain* v140; // [esp+280h] [ebp-3A4h]
    tGameObjectMain* v141; // [esp+284h] [ebp-3A0h]
    tGameObjectMain* v142; // [esp+288h] [ebp-39Ch]
    tGameObjectMain* v143; // [esp+28Ch] [ebp-398h]
    tGameObjectMain* v144; // [esp+290h] [ebp-394h]
    tGameObjectMain* v145; // [esp+298h] [ebp-38Ch]
    tGameObjectMain* v146; // [esp+29Ch] [ebp-388h]
    tGameObjectMain* v147; // [esp+2A0h] [ebp-384h]
    tGameObjectMain* v148; // [esp+2A4h] [ebp-380h]
    tGameObjectMain* v149; // [esp+2B8h] [ebp-36Ch]
    tGameObjectMain* v150; // [esp+2BCh] [ebp-368h]
    tGameObjectMain* v151; // [esp+2C0h] [ebp-364h]
    tGameObjectMain* v152; // [esp+2C4h] [ebp-360h]
    tGameObjectMain* v153; // [esp+2E8h] [ebp-33Ch]
    tGameObjectMain* v154; // [esp+2F0h] [ebp-334h]
    tGameObjectMain* v155; // [esp+2F8h] [ebp-32Ch]
    tGameObjectMain* v156; // [esp+300h] [ebp-324h]
    tGameObjectMain* v157; // [esp+304h] [ebp-320h]
    tGameObjectMain* v158; // [esp+308h] [ebp-31Ch]
    tAI* v159; // [esp+30Ch] [ebp-318h]
    tGameObjectMain* v160; // [esp+310h] [ebp-314h]
    tGameObjectMain* v161; // [esp+314h] [ebp-310h]
    tGameObjectMain* v162; // [esp+318h] [ebp-30Ch]
    tGameObjectMain* v163; // [esp+31Ch] [ebp-308h]
    tGameObjectMain* v164; // [esp+320h] [ebp-304h]
    tGameObjectMain* v165; // [esp+324h] [ebp-300h]
    tGameObjectMain* v166; // [esp+328h] [ebp-2FCh]
    tGameObjectMain* v167; // [esp+32Ch] [ebp-2F8h]
    tGameObjectMain* v168; // [esp+350h] [ebp-2D4h]
    Vec3 v169; // [esp+354h] [ebp-2D0h] BYREF
    Vec3 v170; // [esp+360h] [ebp-2C4h] BYREF
    Vec3 v171; // [esp+36Ch] [ebp-2B8h] BYREF
    Vec3 v172; // [esp+384h] [ebp-2A0h] BYREF
    Vec3 v173; // [esp+390h] [ebp-294h] BYREF
    int v174; // [esp+39Ch] [ebp-288h]
    tGameObjectMain* v175; // [esp+3A0h] [ebp-284h]
    Vec3 v176[2]; // [esp+3B8h] [ebp-26Ch] BYREF
    Vec3 v177; // [esp+3D0h] [ebp-254h] BYREF
    float v178; // [esp+3DCh] [ebp-248h]
    float v179; // [esp+3E0h] [ebp-244h]
    Vec3 v180; // [esp+3E8h] [ebp-23Ch] BYREF
    Vec3 v181; // [esp+3F4h] [ebp-230h] BYREF
    tGameObjectMain* v182; // [esp+40Ch] [ebp-218h]
    int v183; // [esp+410h] [ebp-214h]
    Vec3 a3; // [esp+414h] [ebp-210h] BYREF
    Vec3 v185; // [esp+420h] [ebp-204h] BYREF
    int v186; // [esp+42Ch] [ebp-1F8h]
    float v187; // [esp+430h] [ebp-1F4h]
    Vec3 v188; // [esp+440h] [ebp-1E4h] BYREF
    float v190; // [esp+458h] [ebp-1CCh]
    float v191; // [esp+45Ch] [ebp-1C8h]
    BOOL v192; // [esp+460h] [ebp-1C4h]
    int v193; // [esp+468h] [ebp-1BCh]
    Vec3 v194; // [esp+46Ch] [ebp-1B8h] BYREF
    int v195; // [esp+480h] [ebp-1A4h]
    int v196; // [esp+484h] [ebp-1A0h]
    Vec3 pos; // [esp+488h] [ebp-19Ch] BYREF
    tGameObjectMain* v198; // [esp+494h] [ebp-190h]
    float v199; // [esp+498h] [ebp-18Ch]
    float v200; // [esp+4A4h] [ebp-180h]
    int v201; // [esp+4ACh] [ebp-178h]
    Vec3 v202; // [esp+4B0h] [ebp-174h] BYREF
    float v203; // [esp+4C8h] [ebp-15Ch]
    tGameObjectMain* v204; // [esp+4CCh] [ebp-158h]
    BOOL v205; // [esp+4D4h] [ebp-150h]
    tGameObjectMain* v206; // [esp+4D8h] [ebp-14Ch]
    Vec3 dest; // [esp+4DCh] [ebp-148h] BYREF
    Vec3 v208; // [esp+4E8h] [ebp-13Ch]
    Vec3 v209; // [esp+4F4h] [ebp-130h] BYREF
    float v210; // [esp+500h] [ebp-124h]
    BOOL v211; // [esp+504h] [ebp-120h]
    int v212; // [esp+510h] [ebp-114h]
    int v213; // [esp+514h] [ebp-110h]
    Vec3 v214; // [esp+518h] [ebp-10Ch] BYREF
    Vec3 v215; // [esp+524h] [ebp-100h] BYREF
    float v216; // [esp+530h] [ebp-F4h]
    tGameObjectMain* v217; // [esp+534h] [ebp-F0h]
    int i; // [esp+538h] [ebp-ECh]
    float v219; // [esp+53Ch] [ebp-E8h]
    float v220; // [esp+540h] [ebp-E4h]
    float v221; // [esp+550h] [ebp-D4h]
    float v222; // [esp+554h] [ebp-D0h]
    tGameObjectMain* v223; // [esp+558h] [ebp-CCh]
    BOOL v224; // [esp+55Ch] [ebp-C8h]
    float v225; // [esp+560h] [ebp-C4h]
    int v226; // [esp+568h] [ebp-BCh]
    Vec3 v227; // [esp+56Ch] [ebp-B8h] BYREF
    Vec3 v228; // [esp+584h] [ebp-A0h] BYREF
    float v229; // [esp+594h] [ebp-90h]
    BOOL v230; // [esp+598h] [ebp-8Ch]
    float v231; // [esp+59Ch] [ebp-88h]
    tScriptStruct_PNJSpectre* v232; // [esp+5A0h] [ebp-84h]
    tGameObjectMain* v233; // [esp+5A4h] [ebp-80h]
    tGameObjectMain* v234; // [esp+5A8h] [ebp-7Ch]
    Vec3 v235; // [esp+5ACh] [ebp-78h]
    float v236; // [esp+5B8h] [ebp-6Ch]
    int v237; // [esp+5BCh] [ebp-68h]
    float v238; // [esp+5C0h] [ebp-64h]
    tAI* v239; // [esp+5C4h] [ebp-60h]
    tMessage Src; // [esp+5C8h] [ebp-5Ch] BYREF
    int v241; // [esp+5FCh] [ebp-28h]
    int v242; // [esp+604h] [ebp-20h]
    BOOL v243; // [esp+608h] [ebp-1Ch]
    tGameObjectMain* v244; // [esp+60Ch] [ebp-18h]
    int v245; // [esp+618h] [ebp-Ch]
    float v246; // [esp+61Ch] [ebp-8h]
    float v247; // [esp+620h] [ebp-4h]

    v194.x = 1.0;
    v194.y = 0.0;
    v194.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v12 = (tScriptStruct_PNJSpectre*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v12 = 0;
    v232 = v12;
    v243 = 0;
    v186 = 0;
    v233 = 0;
    v195 = 0;
    v212 = 0;
    i = 0;
    v183 = 0;
    v239 = 0;
    v226 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        PNJ_Spectre_sp_mode_mort_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(&PNJ_Spectre_sp_mode_mort_GST_Global_gao_i_sens, (void*)PNJ_Spectre_sp_mode_mort_GST_Global_gao, 4652);
        PNJ_Spectre_sp_mode_mort_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        PNJ_Spectre_sp_mode_mort_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&PNJ_Spectre_sp_mode_mort_univVars_Flag_speciaux_sauve, (void*)PNJ_Spectre_sp_mode_mort_univVars, 4488);
        PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao = AI_ul_RealResolveOneRefObject(0x9E002FFA);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Evil_Actor,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            520);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_NRJ_Courante,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            892);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_i_Evil_NbActor,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            2624);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_i_Evil_NbActorAlive,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            2632);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_NRJShieldMax,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            768);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_o_last_mort,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            3788);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Good_Actor,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            0);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Evil_Target,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            644);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ai_Good_Targeted,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            416);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ai_Evil_Targeted,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            1636);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Good_Target,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            52);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_Dmin,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            1884);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_DMax,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            2008);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_Angle,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            2132);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ai_Evil_Icone,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            1760);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_Last_Hit,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            1016);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Evil_Last_Good,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            1140);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_Last_Dmg,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            1264);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_Last_Eff,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            1388);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_Cumul_Dmg,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            1512);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_f_last_paf_time,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            3776);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_o_last_paf,
            (void*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao,
            3772);
        PNJ_Spectre_sp_mode_mort_DUP_SFX_XMEN_Dark_gao = AI_ul_RealResolveOneRefObject(0x2600363Bu);
        PNJ_Spectre_sp_mode_mort__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        fct_PNJ_Spectre_sp_mode_mort_PNJ_Spectre_sp_after_blend = AI_ul_RealResolveOneRefFunction(0x9E00669B);
        PNJ_Spectre_sp_mode_mort_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_INV_manager_gao_i_Animals_Nb,
            (void*)PNJ_Spectre_sp_mode_mort_INV_manager_gao,
            23888);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_INV_manager_gao_ao_Animals,
            (void*)PNJ_Spectre_sp_mode_mort_INV_manager_gao,
            23368);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_mort_INV_manager_gao_ai_AnimalsData,
            (void*)PNJ_Spectre_sp_mode_mort_INV_manager_gao,
            23628);
    }
    if (*AI_gppst_CurrentJumpNode != (tAINode*)20)
    {
        if (*AI_gppst_CurrentJumpNode == (tAINode*)29)
            goto LABEL_343;
        if (v232->i_mode != 9)
        {
            v232->i_mode = 9;
            *(int32_t*)PNJ_Spectre_sp_mode_mort_GST_Global_gao_i_sens = -*(int32_t*)PNJ_Spectre_sp_mode_mort_GST_Global_gao_i_sens;
            v232->f_speed_rot = 8.0;
            v232->f_color_duration = 0.0;
            i = AI_EvalFunc_GFXAdd_C_CURRENT(9);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(i, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(i, 2, 1);
            v168 = (tGameObjectMain*)PNJ_Spectre_sp_mode_mort_SFX_light_smoke_gao;
            if (PNJ_Spectre_sp_mode_mort_SFX_light_smoke_gao)
            {
                if ((unsigned int)PNJ_Spectre_sp_mode_mort_SFX_light_smoke_gao <= 2)
                    v168 = AI_MainActor[PNJ_Spectre_sp_mode_mort_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(i, v168, 5);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(i, 0, 5);
            }
            GFXSetCurGlobal(i);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9000, 2.0943961f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9001, 3.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9002, 10.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9003, 0.98000002f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9004, 0.050000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9005, 0.2f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9006, 1.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9007, 5.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9008, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9009, 0.2f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9010, 0.60000002f);
            AI_EvalFunc_GFXSeti_C_CURGLOB(9100, 30);
            AI_EvalFunc_GFXSeti_C_CURGLOB(9101, 0xC06BAFE3);
            AI_EvalFunc_GFXSeti_C_CURGLOB(9102, 24);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v228);
            v214.x = 0.0;
            v214.y = 0.0;
            v214.z = 1.5;
            MATH_VecAddVec(&v180, &v228, &v214);
            AI_EvalFunc_GFXSetv_C_CURGLOB(9200, &v180);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v177);
            AI_EvalFunc_GFXSetv_C_CURGLOB(9201, &v177);
            v202.x = 0.0;
            v202.y = 0.0;
            v202.z = -1.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(9202, &v202);
            AI_EvalFunc_ACTActionSet_C_CURRENT(9);
            v194 = v232->v_paf_sens_coup;
            v243 = AI_EvalFunc_ARRBitTest_C((int*)(PNJ_Spectre_sp_mode_mort_univVars_Flag_speciaux_sauve + 4), 2764);
            if (AI_EvalFunc_MTHVecNullEpsilon_C(&v194))
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v215);
                v235.x = -v215.x;
                v235.y = -v215.y;
                v235.z = -v215.z;
                v194.x = -v215.x;
                v194.y = -v215.y;
                v194.z = -v215.z;
            }
            if (v243)
                AI_EvalFunc_MATHVecSetNormVector_C(&v194, 10.0);
            else
                AI_EvalFunc_MATHVecSetNormVector_C(&v194, 4.0);
            v193 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v194);
            v208.x = 5.0;
            v208.y = 5.0;
            v208.z = 1.0;
            v185.x = 5.0 * 0.5;
            v185.y = v185.x;
            v185.z = 1.0 * 0.5;
            v242 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v185);
            v167 = (tGameObjectMain*)v232->o_me;
            if (v167)
            {
                if ((unsigned int)v167 <= 2)
                    v167 = AI_MainActor[(uint32_t)v167 - 1];
                v166 = v167;
            }
            else
            {
                v166 = 0;
            }
            v166->extended->fields->designInt2 = 0x9000;
            v165 = (tGameObjectMain*)v232->o_me;
            if (v165)
            {
                if ((unsigned int)v165 <= 2)
                    v165 = AI_MainActor[(uint32_t)v165 - 1];
                v164 = v165;
            }
            else
            {
                v164 = 0;
            }
            v239 = (tAI*)v164->extended->fields->designGO2;
            v163 = (tGameObjectMain*)v239;
            if (v239)
            {
                if ((unsigned int)v239 <= 2)
                    v163 = AI_MainActor[(uint32_t)v239 - 1];
                v162 = v163;
            }
            else
            {
                v162 = 0;
            }
            if (v162)
            {
                if (v239 == WOR_gst_Universe.univAIInstance)
                {
                    v159 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v161 = (tGameObjectMain*)v239;
                    if (v239)
                    {
                        if ((unsigned int)v239 <= 2)
                            v161 = AI_MainActor[(uint32_t)v239 - 1];
                        v160 = v161;
                    }
                    else
                    {
                        v160 = 0;
                    }
                    v159 = v160->extended->AIInstance;
                }
                ++* ((uint32_t*)v159->instanceVarDataCurrent + 458); // todo fixme ez mi?
                v158 = (tGameObjectMain*)v232->o_actor_a_reactiver_a_sa_mort;
                if (v158)
                {
                    if ((unsigned int)v158 <= 2)
                        v158 = AI_MainActor[(uint32_t)v158 - 1];
                    v157 = v158;
                }
                else
                {
                    v157 = 0;
                }
                if (v157)
                {
                    v156 = (tGameObjectMain*)v239;
                    if (v239)
                    {
                        if ((unsigned int)v239 <= 2)
                            v156 = AI_MainActor[(uint32_t)v239 - 1];
                        v0 = AI_EvalFunc_OBJ_CapaTest_C(v156, 2);
                    }
                    else
                    {
                        v0 = AI_EvalFunc_OBJ_CapaTest_C(0, 2);
                    }
                    v211 = v0;
                    if (!v0)
                    {
                        v155 = (tGameObjectMain*)v239;
                        if (v239)
                        {
                            if ((unsigned int)v239 <= 2)
                                v155 = AI_MainActor[(uint32_t)v239 - 1];
                            v1 = AI_EvalFunc_OBJ_CapaTest_C(v155, 0x100);
                        }
                        else
                        {
                            v1 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x100);
                        }
                        v192 = v1;
                        if (!v1)
                        {
                            v154 = (tGameObjectMain*)v232->o_actor_a_reactiver_a_sa_mort;
                            if (v154)
                            {
                                if ((unsigned int)v154 <= 2)
                                    v154 = AI_MainActor[(uint32_t)v154 - 1];
                                AI_EvalFunc_OBJReinit_C(v154, 1);
                            }
                            else
                            {
                                AI_EvalFunc_OBJReinit_C(0, 1);
                            }
                            v153 = (tGameObjectMain*)v232->o_actor_a_reactiver_a_sa_mort;
                            if (v153)
                            {
                                if ((unsigned int)v153 <= 2)
                                    v153 = AI_MainActor[(uint32_t)v153 - 1];
                                AI_EvalFunc_OBJ_FlagInactiveSet_C(v153, 0);
                            }
                            else
                            {
                                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
                            }
                        }
                    }
                }
            }
            *(uint32_t*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_o_last_mort = (uint32_t)AI_gpst_CurrentGameObject;
            v226 = -1;
            v244 = AI_gpst_CurrentGameObject;
            v152 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v152 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v151 = v152;
            }
            else
            {
                v151 = 0;
            }
            if (v151)
            {
                v217 = AI_gpst_CurrentGameObject;
                v150 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                        v150 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                    v149 = v150;
                }
                else
                {
                    v149 = 0;
                }
                v196 = AI_EvalFunc_ARRObjSearch_C(
                    (tGameObjectMain**)(PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Evil_Actor + 4),
                    30,
                    v149);
                v226 = v196;
            }
            v183 = -1;
            v212 = -1;
            v223 = AI_gpst_CurrentGameObject;
            v148 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v148 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v147 = v148;
            }
            else
            {
                v147 = 0;
            }
            if (v147)
            {
                v182 = AI_gpst_CurrentGameObject;
                v146 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                        v146 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                    v145 = v146;
                }
                else
                {
                    v145 = 0;
                }
                v237 = AI_EvalFunc_ARRObjSearch_C(
                    (tGameObjectMain**)(PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Evil_Actor + 4),
                    30,
                    v145);
                v212 = v237;
            }
            if (v212 != -1)
            {
                v144 = *(tGameObjectMain**)(PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Evil_Target + 4 * v212 + 4);
                if (v144)
                {
                    if ((unsigned int)v144 <= 2)
                        v144 = AI_MainActor[(uint32_t)v144 - 1];
                    v143 = v144;
                }
                else
                {
                    v143 = 0;
                }
                if (v143)
                {
                    v186 = -1;
                    v142 = *(tGameObjectMain**)(PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Evil_Target + 4 * v212 + 4);
                    if (v142)
                    {
                        if ((unsigned int)v142 <= 2)
                            v142 = AI_MainActor[(uint32_t)v142 - 1];
                        v141 = v142;
                    }
                    else
                    {
                        v141 = 0;
                    }
                    if (v141)
                    {
                        v140 = *(tGameObjectMain**)(PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Evil_Target + 4 * v212 + 4);
                        if (v140)
                        {
                            if ((unsigned int)v140 <= 2)
                                v140 = AI_MainActor[(uint32_t)v140 - 1];
                            v139 = v140;
                        }
                        else
                        {
                            v139 = 0;
                        }
                        v186 = AI_EvalFunc_ARRObjSearch_C(
                            (tGameObjectMain**)(PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Good_Actor + 4),
                            12,
                            v139);
                    }
                    if (v186 != -1)
                        --* (uint32_t*)(PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ai_Good_Targeted + 4 * v186 + 4);
                }
            }
            if (v183 != -1)
                ++* (uint32_t*)(PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ai_Good_Targeted + 4 * v183 + 4);
            if (v212 != -1)
            {
                *(uint32_t*)(PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Evil_Target + 4 * v212 + 4) = 0;
                v234 = AI_gpst_CurrentGameObject;
                v138 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                        v138 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                    v137 = v138;
                }
                else
                {
                    v137 = 0;
                }
                v137->extended->fields->designGO1 = 0;
            }
            *(uint32_t*)(PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ai_Evil_Targeted + 4 * v226 + 4) = 0;
            --* (uint32_t*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_i_Evil_NbActor;
            *(uint32_t*)(PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Evil_Actor + 4 * v226 + 4) = 0;
            v204 = AI_gpst_CurrentGameObject;
            v136 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v136 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v135 = v136;
            }
            else
            {
                v135 = 0;
            }
            v213 = AI_EvalFunc_ARRObjSearch_C(
                (tGameObjectMain**)(PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Good_Target + 4),
                12,
                v135);
            v195 = v213;
            if (v213 != -1)
                *(uint32_t*)(PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Good_Target + 4 * v195 + 4) = 0;
            if (!*(uint32_t*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_i_Evil_NbActor
                && !*(uint32_t*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_i_Evil_NbActorAlive)
            {
                AI_EvalFunc_OBJ_CapaSet_C((tGameObjectMain*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao, 2, 0);
            }
            *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_NRJShieldMax, v226) = 0;
            *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_Dmin, v226) = 0;
            *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_DMax, v226) = 0;
            *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_Angle, v226) = 0x40490FDC;
            *(uint32_t*)(PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ai_Evil_Icone + 4 * v226 + 4) = 0;
            *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_Last_Hit, v226) = 0;
            *(uint32_t*)(PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_ao_Evil_Last_Good + 4 * v226 + 4) = 0;
            *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_Last_Dmg, v226) = 0;
            *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_Last_Eff, v226) = 0;
            *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_af_Evil_Cumul_Dmg, v226) = 0;
            v232->i_fight_on = 0;
            AI_EvalFunc_AITrackStop_C(0);
            AI_EvalFunc_AITrackStop_C(1);
            AI_EvalFunc_AITrackStop_C(4);
            v241 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(AI_gpst_CurrentGameObject, 8, -1.0, 0.94999999f, 1.05f);
            AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        }
        *(float*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_f_last_paf_time = TIM_gf_MainClock - TIM_gf_LockTime;
        *(uint32_t*)PNJ_Spectre_sp_mode_mort_GST_Fight_Manager_gao_o_last_paf = (uint32_t)AI_gpst_CurrentGameObject;
        v134 = (tGameObjectMain*)v232->o_gamin_snap;
        if (v134)
        {
            if ((unsigned int)v134 <= 2)
                v134 = AI_MainActor[(uint32_t)v134 - 1];
            v133 = v134;
        }
        else
        {
            v133 = 0;
        }
        if (v133)
        {
            v132 = (tGameObjectMain*)v232->o_gamin_snap;
            if (v132)
            {
                if ((unsigned int)v132 <= 2)
                    v132 = AI_MainActor[(uint32_t)v132 - 1];
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(v132, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
            }
            v232->i_snap_le_gamin = 1;
        }
    }
    if (v232->f_local_tresh_alpha > 25.0)
    {
        v222 = TIM_gf_dt;
        v11 = (float)*(int*)PNJ_Spectre_sp_mode_mort_GST_Global_gao_i_sens;
        v4 = v232->f_speed_rot * TIM_gf_dt * v11;
        AI_EvalFunc_OBJRotateLocalZ_c(AI_gpst_CurrentGameObject, v4);
        v179 = TIM_gf_dt;
        v131 = TIM_gf_dt;
        if (TIM_gf_dt < 0.0)
            v131 = 0.0;
        if (v131 > 1.0)
            v131 = 1.0;
        v190 = 0.0f * v131 + v232->f_speed_rot * (1.0f - v131);
        v232->f_speed_rot = v190;
        v229 = TIM_gf_dt;
        v130 = TIM_gf_dt * 200.0f;
        if (v130 > (double)v232->f_local_tresh_alpha)
            v10 = v232->f_local_tresh_alpha;
        else
            v10 = TIM_gf_dt * 200.0f;
        v238 = v10;
        v232->f_local_tresh_alpha = v232->f_local_tresh_alpha - v10;
        v210 = TIM_gf_dt;
        v129 = TIM_gf_dt * 200.0f;
        if (v129 > (double)v232->f_vitre_local_alpha)
            v9 = v232->f_vitre_local_alpha;
        else
            v9 = TIM_gf_dt * 200.0f;
        v199 = v9;
        v232->f_vitre_local_alpha = v232->f_vitre_local_alpha - v9;
        v224 = AI_EvalFunc_TIMEElapsed_C(v232->f_time_gen_XMEN, 0.079999998f);
        if (v224 && v232->f_local_tresh_alpha > 50.0)
        {
            v232->f_time_gen_XMEN = TIM_gf_MainClock - TIM_gf_LockTime;
            v128 = (tGameObjectMain*)v232->des_fuite_WP;
            if (v128)
            {
                if ((unsigned int)v128 <= 2)
                    v128 = AI_MainActor[(uint32_t)v128 - 1];
                v127 = v128;
            }
            else
            {
                v127 = 0;
            }
            if (v127)
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
                v233 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)PNJ_Spectre_sp_mode_mort_DUP_SFX_XMEN_Dark_gao, &pos);
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v124 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v126 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v126 = AI_MainActor[(uint32_t)v233 - 1];
                        v125 = v126;
                    }
                    else
                    {
                        v125 = 0;
                    }
                    v124 = v125->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v124->instanceVarDataCurrent))->des_start_actor = (int)AI_gpst_CurrentGameObject;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v121 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v123 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v123 = AI_MainActor[(uint32_t)v233 - 1];
                        v122 = v123;
                    }
                    else
                    {
                        v122 = 0;
                    }
                    v121 = v122->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v121->instanceVarDataCurrent))->des_start_bone_num = 3;
                v187 = AI_EvalFunc_MATHRandFloat_C(-0.5, 1.5);
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v118 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v120 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v120 = AI_MainActor[(uint32_t)v233 - 1];
                        v119 = v120;
                    }
                    else
                    {
                        v119 = 0;
                    }
                    v118 = v119->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v118->instanceVarDataCurrent))->des_start_bone_offset.x = 0.0;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v115 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v117 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v117 = AI_MainActor[(uint32_t)v233 - 1];
                        v116 = v117;
                    }
                    else
                    {
                        v116 = 0;
                    }
                    v115 = v116->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v115->instanceVarDataCurrent))->des_start_bone_offset.y = 0.0;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v112 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v114 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v114 = AI_MainActor[(uint32_t)v233 - 1];
                        v113 = v114;
                    }
                    else
                    {
                        v113 = 0;
                    }
                    v112 = v113->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v112->instanceVarDataCurrent))->des_start_bone_offset.z = v187;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v109 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v111 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v111 = AI_MainActor[(uint32_t)v233 - 1];
                        v110 = v111;
                    }
                    else
                    {
                        v110 = 0;
                    }
                    v109 = v110->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v109->instanceVarDataCurrent))->des_dest_actor = v232->des_fuite_WP;
                v246 = AI_EvalFunc_MATHRandFloat_C(-1.5, 1.5);
                v191 = AI_EvalFunc_MATHRandFloat_C(-2.0, 0.0);
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v106 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v108 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v108 = AI_MainActor[(uint32_t)v233 - 1];
                        v107 = v108;
                    }
                    else
                    {
                        v107 = 0;
                    }
                    v106 = v107->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v106->instanceVarDataCurrent))->des_dest_bone_offset.x = v246;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v103 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v105 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v105 = AI_MainActor[(uint32_t)v233 - 1];
                        v104 = v105;
                    }
                    else
                    {
                        v104 = 0;
                    }
                    v103 = v104->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v103->instanceVarDataCurrent))->des_dest_bone_offset.y = v191;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v100 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v102 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v102 = AI_MainActor[(uint32_t)v233 - 1];
                        v101 = v102;
                    }
                    else
                    {
                        v101 = 0;
                    }
                    v100 = v101->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v100->instanceVarDataCurrent))->des_dest_bone_offset.z = 0.0;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v97 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v99 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v99 = AI_MainActor[(uint32_t)v233 - 1];
                        v98 = v99;
                    }
                    else
                    {
                        v98 = 0;
                    }
                    v97 = v98->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v97->instanceVarDataCurrent))->des_dest_bone_dir.x = 0.0;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v94 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v96 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v96 = AI_MainActor[(uint32_t)v233 - 1];
                        v95 = v96;
                    }
                    else
                    {
                        v95 = 0;
                    }
                    v94 = v95->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v94->instanceVarDataCurrent))->des_dest_bone_dir.y = 0.0;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v91 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v93 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v93 = AI_MainActor[(uint32_t)v233 - 1];
                        v92 = v93;
                    }
                    else
                    {
                        v92 = 0;
                    }
                    v91 = v92->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v91->instanceVarDataCurrent))->des_dest_bone_dir.z = 0.1f;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v88 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v90 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v90 = AI_MainActor[(uint32_t)v233 - 1];
                        v89 = v90;
                    }
                    else
                    {
                        v89 = 0;
                    }
                    v88 = v89->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v88->instanceVarDataCurrent))->des_XMEN_width = 0.40000001f;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v85 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v87 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v87 = AI_MainActor[(uint32_t)v233 - 1];
                        v86 = v87;
                    }
                    else
                    {
                        v86 = 0;
                    }
                    v85 = v86->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v85->instanceVarDataCurrent))->des_f_move_delay = AI_EvalFunc_MATHRandFloat_C(1.0, 2.0);
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v82 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v84 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v84 = AI_MainActor[(uint32_t)v233 - 1];
                        v83 = v84;
                    }
                    else
                    {
                        v83 = 0;
                    }
                    v82 = v83->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v82->instanceVarDataCurrent))->des_sfx_halo = 0;
                v81 = v233;
                if (v233)
                {
                    if ((unsigned int)v233 <= 2)
                        v81 = AI_MainActor[(uint32_t)v233 - 1];
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(v81, 1);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
                }
            }
        }
        if (v232->f_local_tresh_alpha < 125.0)
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x8000, 0);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v169);
        AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v227);
        v80 = v232->f_dt;
        v171.x = v227.x * v80;
        v171.y = v227.y * v80;
        v171.z = v227.z * v80;
        MATH_VecAddVec(&v209, &v169, &v171);
        v79 = (tGameObjectMain*)v232->WP_pos_combat;
        if (v79)
        {
            if ((unsigned int)v79 <= 2)
                v79 = AI_MainActor[(uint32_t)v79 - 1];
            AI_EvalFunc_OBJGetPos_C(v79, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        MATH_SubstractVec(&dest, &v209, &a3);
        v78 = (tGameObjectMain*)v232->o_NMI;
        if (v78)
        {
            if ((unsigned int)v78 <= 2)
                v78 = AI_MainActor[(uint32_t)v78 - 1];
            AI_EvalFunc_OBJGetPos_C(v78, &v170);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v170);
        }
        v77 = (tGameObjectMain*)v232->WP_pos_combat;
        if (v77)
        {
            if ((unsigned int)v77 <= 2)
                v77 = AI_MainActor[(uint32_t)v77 - 1];
            AI_EvalFunc_OBJGetPos_C(v77, &v188);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v188);
        }
        MATH_SubstractVec(&v173, &v170, &v188);
        v236 = v173.z * dest.z + v173.y * dest.y + dest.x * v173.x;
        if (v236 < 0.0)
        {
            v76 = (tGameObjectMain*)v232->WP_pos_combat;
            if (v76)
            {
                if ((unsigned int)v76 <= 2)
                    v76 = AI_MainActor[(uint32_t)v76 - 1];
                AI_EvalFunc_OBJGetPos_C(v76, v176);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, v176);
            }
            AI_EvalFunc_OBJSetPos_C_CURRENT(v176);
            v172.x = 0.0;
            v172.y = 0.0;
            v172.z = 0.0;
            v201 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v172);
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)20;
        return 0;
    }
    v75 = (tGameObjectMain*)v232->o_gamin_snap;
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
    if (v74)
    {
        v232->i_snap_le_gamin = 0;
        v73 = (tGameObjectMain*)v232->o_gamin_snap;
        if (v73)
        {
            if ((unsigned int)v73 <= 2)
                v73 = AI_MainActor[(uint32_t)v73 - 1];
            AI_EvalFunc_OBJ_CapaSet_C(v73, 0x100, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_CapaSet_C(0, 0x100, 0);
        }
    }
    v72 = (tGameObjectMain*)v232->des_helper_libe;
    if (v72)
    {
        if ((unsigned int)v72 <= 2)
            v72 = AI_MainActor[(uint32_t)v72 - 1];
        v71 = v72;
    }
    else
    {
        v71 = 0;
    }
    if (v71)
    {
        Src.msgID = 30;
        Src.Int1 = v232->i_replique;
        Src.Int2 = 10;
        Src.f2 = 2.0;
        Src.v1.x = 0.0;
        Src.v1.y = 0.0;
        Src.v1.z = 0.0;
        Src.v2.x = 0.0;
        Src.v2.y = 0.0;
        Src.v2.z = 0.0;
        Src.gao = 0;
        Src.gao2 = (tGameObjectMain*)PNJ_Spectre_sp_mode_mort__PJ_Sally_gao;
        v70 = (tGameObjectMain*)v232->des_helper_libe;
        if (v70)
        {
            if ((unsigned int)v70 <= 2)
                v70 = AI_MainActor[(uint32_t)v70 - 1];
            AI_EvalFunc_MSGSend_C(v70, &Src); // ez kuldi a got it handled get yourself to safety 61009c41? 61009c43 (ez az) hangot
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
    }
    AI_gpst_CurrentInstance->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -0.5;
LABEL_343:
    if (AI_EvalKeyword_During_C())
    {
        v200 = TIM_gf_dt;
        v8 = (float)*(int*)PNJ_Spectre_sp_mode_mort_GST_Global_gao_i_sens;
        v5 = v232->f_speed_rot * TIM_gf_dt * v8;
        AI_EvalFunc_OBJRotateLocalZ_c(AI_gpst_CurrentGameObject, v5);
        v219 = TIM_gf_dt;
        v69 = TIM_gf_dt;
        if (TIM_gf_dt < 0.0)
            v69 = 0.0;
        if (v69 > 1.0)
            v69 = 1.0;
        v225 = 0.0f * v69 + v232->f_speed_rot * (1.0f - v69);
        v232->f_speed_rot = v225;
        v231 = TIM_gf_dt;
        v68 = TIM_gf_dt * 200.0f;
        DBG_OutputDebugString("Spectre: bef f_local_tresh_alpha: %f\n", v232->f_local_tresh_alpha);
        if (v68 > (double)v232->f_local_tresh_alpha)
            v7 = v232->f_local_tresh_alpha;
        else
            v7 = TIM_gf_dt * 200.0f;
        v178 = v7;
        v232->f_local_tresh_alpha = v232->f_local_tresh_alpha - v7;
        v216 = TIM_gf_dt;
        v67 = TIM_gf_dt * 200.0f;
        if (v67 > (double)v232->f_vitre_local_alpha)
            v6 = v232->f_vitre_local_alpha;
        else
            v6 = TIM_gf_dt * 200.0f;
        v203 = v6;
        v232->f_vitre_local_alpha = v232->f_vitre_local_alpha - v6;
        v230 = AI_EvalFunc_TIMEElapsed_C(v232->f_time_gen_XMEN, 0.079999998f);
        DBG_OutputDebugString("Spectre: af f_local_tresh_alpha: %f\n", v232->f_local_tresh_alpha); //  Todo fixme ez itt még bugos, nem ugyanaz mint az eredeti. Az olyan 50 folottrol indul ez 22
        if (v230 && v232->f_local_tresh_alpha > 50.0)
        {
            v232->f_time_gen_XMEN = TIM_gf_MainClock - TIM_gf_LockTime;
            v66 = (tGameObjectMain*)v232->des_fuite_WP;
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
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v181);
                v233 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)PNJ_Spectre_sp_mode_mort_DUP_SFX_XMEN_Dark_gao, &v181);
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v62 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v64 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v64 = AI_MainActor[(uint32_t)v233 - 1];
                        v63 = v64;
                    }
                    else
                    {
                        v63 = 0;
                    }
                    v62 = v63->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v62->instanceVarDataCurrent))->des_start_actor = (int)AI_gpst_CurrentGameObject;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v59 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v61 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v61 = AI_MainActor[(uint32_t)v233 - 1];
                        v60 = v61;
                    }
                    else
                    {
                        v60 = 0;
                    }
                    v59 = v60->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v59->instanceVarDataCurrent))->des_start_bone_num = 3;
                v247 = AI_EvalFunc_MATHRandFloat_C(-0.5, 1.5);
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v56 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v58 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v58 = AI_MainActor[(uint32_t)v233 - 1];
                        v57 = v58;
                    }
                    else
                    {
                        v57 = 0;
                    }
                    v56 = v57->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v56->instanceVarDataCurrent))->des_start_bone_offset.x = 0.0;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v53 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v55 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v55 = AI_MainActor[(uint32_t)v233 - 1];
                        v54 = v55;
                    }
                    else
                    {
                        v54 = 0;
                    }
                    v53 = v54->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v53->instanceVarDataCurrent))->des_start_bone_offset.y = 0.0;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v50 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v52 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v52 = AI_MainActor[(uint32_t)v233 - 1];
                        v51 = v52;
                    }
                    else
                    {
                        v51 = 0;
                    }
                    v50 = v51->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v50->instanceVarDataCurrent))->des_start_bone_offset.z = v247;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v47 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v49 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v49 = AI_MainActor[(uint32_t)v233 - 1];
                        v48 = v49;
                    }
                    else
                    {
                        v48 = 0;
                    }
                    v47 = v48->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v47->instanceVarDataCurrent))->des_dest_actor = v232->des_fuite_WP;
                v220 = AI_EvalFunc_MATHRandFloat_C(-1.5, 1.5);
                v221 = AI_EvalFunc_MATHRandFloat_C(-2.0, 0.0);
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v44 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v46 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v46 = AI_MainActor[(uint32_t)v233 - 1];
                        v45 = v46;
                    }
                    else
                    {
                        v45 = 0;
                    }
                    v44 = v45->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v44->instanceVarDataCurrent))->des_dest_bone_offset.x = v220;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v41 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v43 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v43 = AI_MainActor[(uint32_t)v233 - 1];
                        v42 = v43;
                    }
                    else
                    {
                        v42 = 0;
                    }
                    v41 = v42->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v41->instanceVarDataCurrent))->des_dest_bone_offset.y = v221;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v38 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v40 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v40 = AI_MainActor[(uint32_t)v233 - 1];
                        v39 = v40;
                    }
                    else
                    {
                        v39 = 0;
                    }
                    v38 = v39->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v38->instanceVarDataCurrent))->des_dest_bone_offset.z = 0.0;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v35 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v37 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v37 = AI_MainActor[(uint32_t)v233 - 1];
                        v36 = v37;
                    }
                    else
                    {
                        v36 = 0;
                    }
                    v35 = v36->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v35->instanceVarDataCurrent))->des_dest_bone_dir.x = 0.0;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v32 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v34 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v34 = AI_MainActor[(uint32_t)v233 - 1];
                        v33 = v34;
                    }
                    else
                    {
                        v33 = 0;
                    }
                    v32 = v33->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v32->instanceVarDataCurrent))->des_dest_bone_dir.y = 0.0;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v29 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v31 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v31 = AI_MainActor[(uint32_t)v233 - 1];
                        v30 = v31;
                    }
                    else
                    {
                        v30 = 0;
                    }
                    v29 = v30->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v29->instanceVarDataCurrent))->des_dest_bone_dir.z = 0.1f;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v26 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v28 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v28 = AI_MainActor[(uint32_t)v233 - 1];
                        v27 = v28;
                    }
                    else
                    {
                        v27 = 0;
                    }
                    v26 = v27->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v26->instanceVarDataCurrent))->des_XMEN_width = 0.40000001f;
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v23 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v25 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v25 = AI_MainActor[(uint32_t)v233 - 1];
                        v24 = v25;
                    }
                    else
                    {
                        v24 = 0;
                    }
                    v23 = v24->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v23->instanceVarDataCurrent))->des_f_move_delay = AI_EvalFunc_MATHRandFloat_C(1.0, 2.0);
                if (v233 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v20 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v22 = v233;
                    if (v233)
                    {
                        if ((unsigned int)v233 <= 2)
                            v22 = AI_MainActor[(uint32_t)v233 - 1];
                        v21 = v22;
                    }
                    else
                    {
                        v21 = 0;
                    }
                    v20 = v21->extended->AIInstance;
                }
                ((tScriptStruct_SFXXMEN*)(v20->instanceVarDataCurrent))->des_sfx_halo = 0;
                v19 = v233;
                if (v233)
                {
                    if ((unsigned int)v233 <= 2)
                        v19 = AI_MainActor[(uint32_t)v233 - 1];
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(v19, 1);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
                }
            }
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)29;
        result = 0;
    }
    else
    {
        DBG_OutputDebugString("Spectre mort: CB del\n");
        v206 = AI_gpst_CurrentGameObject;
        v18 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v18 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBDel_C(AI_gpst_CurrentGameObject, v18, 3, (tAIFunction*)fct_PNJ_Spectre_sp_mode_mort_PNJ_Spectre_sp_after_blend);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(AI_gpst_CurrentGameObject, 0, 3, (tAIFunction*)fct_PNJ_Spectre_sp_mode_mort_PNJ_Spectre_sp_after_blend);
        }
        if (v232->i_NMI == 1)
        {
            if (v232->SF_MORT != -1 && v232->SF_MORT > 0)
                AI_EvalFunc_ARRBitSet_C((int*)(PNJ_Spectre_sp_mode_mort_univVars_Flag_speciaux_sauve + 4), v232->SF_MORT);
            for (i = 0; i < *(int32_t*)PNJ_Spectre_sp_mode_mort_INV_manager_gao_i_Animals_Nb; ++i)
            {
                v175 = AI_gpst_CurrentGameObject;
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
                if (*(tGameObjectMain**)(PNJ_Spectre_sp_mode_mort_INV_manager_gao_ao_Animals + 4 * i + 4) == v16)
                {
                    --* (uint32_t*)PNJ_Spectre_sp_mode_mort_INV_manager_gao_i_Animals_Nb;
                    *(uint32_t*)(PNJ_Spectre_sp_mode_mort_INV_manager_gao_ao_Animals + 4 * i + 4) = *(uint32_t*)(PNJ_Spectre_sp_mode_mort_INV_manager_gao_ao_Animals + 4 * *(uint32_t*)PNJ_Spectre_sp_mode_mort_INV_manager_gao_i_Animals_Nb + 4);
                    *(uint32_t*)(PNJ_Spectre_sp_mode_mort_INV_manager_gao_ai_AnimalsData + 4 * i + 4) = *(uint32_t*)(PNJ_Spectre_sp_mode_mort_INV_manager_gao_ai_AnimalsData + 4 * *(uint32_t*)PNJ_Spectre_sp_mode_mort_INV_manager_gao_i_Animals_Nb + 4);
                    break;
                }
            }
            DBG_OutputDebugString("Spectre mort: Destroy\n");
            v174 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v174 != 1000)
                return v174;
        }
        else
        {
            if (!v232->se_reactive_tant_que_pas_capa)
                goto LABEL_521;
            v15 = (tGameObjectMain*)v232->o_actor_capaswitch;
            if (v15)
            {
                if ((unsigned int)v15 <= 2)
                    v15 = AI_MainActor[(uint32_t)v15 - 1];
                v3 = AI_EvalFunc_OBJ_CapaTest_C(v15, 256);
            }
            else
            {
                v3 = AI_EvalFunc_OBJ_CapaTest_C(0, 256);
            }
            v205 = v3;
            if (v3)
            {
            LABEL_521:
                if (v232->SF_MORT != -1 && v232->SF_MORT > 0)
                    AI_EvalFunc_ARRBitSet_C((int*)(PNJ_Spectre_sp_mode_mort_univVars_Flag_speciaux_sauve + 4), v232->SF_MORT);
                for (i = 0; i < *(int32_t*)PNJ_Spectre_sp_mode_mort_INV_manager_gao_i_Animals_Nb; ++i)
                {
                    v198 = AI_gpst_CurrentGameObject;
                    v14 = AI_gpst_CurrentGameObject;
                    if (AI_gpst_CurrentGameObject)
                    {
                        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                            v14 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                        v13 = v14;
                    }
                    else
                    {
                        v13 = 0;
                    }
                    if (*(tGameObjectMain**)(PNJ_Spectre_sp_mode_mort_INV_manager_gao_ao_Animals + 4 * i + 4) == v13)
                    {
                        --* (uint32_t*)PNJ_Spectre_sp_mode_mort_INV_manager_gao_i_Animals_Nb;
                        *(uint32_t*)(PNJ_Spectre_sp_mode_mort_INV_manager_gao_ao_Animals + 4 * i + 4) = *(uint32_t*)(PNJ_Spectre_sp_mode_mort_INV_manager_gao_ao_Animals + 4 * *(uint32_t*)PNJ_Spectre_sp_mode_mort_INV_manager_gao_i_Animals_Nb + 4);
                        *(uint32_t*)(PNJ_Spectre_sp_mode_mort_INV_manager_gao_ai_AnimalsData + 4 * i + 4) = *(uint32_t*)(PNJ_Spectre_sp_mode_mort_INV_manager_gao_ai_AnimalsData + 4 * *(uint32_t*)PNJ_Spectre_sp_mode_mort_INV_manager_gao_i_Animals_Nb + 4);
                        break;
                    }
                }
                v245 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
                if (v245 != 1000)
                    return v245;
            }
            else
            {
                DBG_OutputDebugString("Spectre mort: reinit\n");
                AI_EvalFunc_OBJReinit_C(AI_gpst_CurrentGameObject, 1);
                AI_EvalFunc_OBJ_FlagInactiveSet_C(AI_gpst_CurrentGameObject, 0);
                if (v232->i_depart != 3)
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
            }
        }
        result = 1000;
    }
    return result;
}

int PNJ_Spectre_sp_tag_off()
{
    char* v0; // eax
    float a4; // [esp+0h] [ebp-30h]
    int v3; // [esp+4h] [ebp-2Ch]
    tScriptStruct_PNJSpectre* v4; // [esp+8h] [ebp-28h]
    BOOL v5; // [esp+2Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_PNJSpectre*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    if (v4->tag_etage_force == -1 && AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 64))
    {
        if ((AI_EvalFunc_COLGMFlagsGet_C(AI_gpst_CurrentGameObject, 64) & 0x800000) != 0)
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

int PNJ_Spectre_sp_bef_disp()
{
    float v1; // [esp+4h] [ebp-60h]
    tScriptStruct_PNJSpectre* v2; // [esp+8h] [ebp-5Ch]
    tGameObjectMain* v3; // [esp+Ch] [ebp-58h]
    tGameObjectMain* v4; // [esp+10h] [ebp-54h]
    tGameObjectMain* v5; // [esp+14h] [ebp-50h]
    tGameObjectMain* v6; // [esp+18h] [ebp-4Ch]
    tGameObjectMain* v7; // [esp+1Ch] [ebp-48h]
    tGameObjectMain* v8; // [esp+20h] [ebp-44h]
    tGameObjectMain* a1; // [esp+24h] [ebp-40h]
    tGameObjectMain* v10; // [esp+28h] [ebp-3Ch]
    tGameObjectMain* v11; // [esp+2Ch] [ebp-38h]
    tGameObjectMain* v12; // [esp+30h] [ebp-34h]
    float v13; // [esp+34h] [ebp-30h]
    float v14; // [esp+38h] [ebp-2Ch]
    float v15; // [esp+3Ch] [ebp-28h]
    float v16; // [esp+44h] [ebp-20h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJSpectre*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!v2->i_brasG_on)
        AI_EvalFunc_MATEnableMltxtLvl_C(AI_gpst_CurrentGameObject, v2->i_key_brasG, 0, 0);
    if (!v2->i_brasD_on)
        AI_EvalFunc_MATEnableMltxtLvl_C(AI_gpst_CurrentGameObject, v2->i_key_brasD, 0, 0);
    if (v2->i_mode == 9)
    {
        v1 = 5.0f * TIM_gf_dt;
        v2->i_color_membrane = AI_EvalFunc_COLORBlend_C(v2->i_color_membrane, 0, v1);
    }
    else if (!v2->i_attaque_on || AI_EvalFunc_TIMEElapsed_C(v2->f_time_debut_attaque, v2->f_color_duration))
    {
        if (v2->i_auto_color_membrane)
        {
            v13 = 1.0f * TIM_gf_dt;
            if (v13 < 0.0)
                v13 = 0.0;
            if (v13 > 1.0)
                v13 = 1.0;
            v16 = 0.0f * v13 + v2->f_blend_color_membrane * (1.0f - v13);
            v2->f_blend_color_membrane = v16;
            v2->i_color_membrane = AI_EvalFunc_COLORBlend_C(0x668748C0, 0x66FFFFFF, v2->f_blend_color_membrane);
        }
    }
    else
    {
        v14 = 4.0f * TIM_gf_dt;
        if (v14 < 0.0)
            v14 = 0.0;
        if (v14 > 1.0)
            v14 = 1.0;
        v15 = 1.0f * v14 + v2->f_blend_color_membrane * (1.0f - v14);
        v2->f_blend_color_membrane = v15;
        v2->i_color_membrane = AI_EvalFunc_COLORBlend_C(0x668748C0, 0x66FFFFFF, v2->f_blend_color_membrane);
    }
    AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v2->i_color_membrane, v2->i_key_mask);
    v12 = (tGameObjectMain*)v2->o_body;
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
        v10 = (tGameObjectMain*)v2->o_body;
        if (v10)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            a1 = v10;
        }
        else
        {
            a1 = 0;
        }
        AI_EvalFunc_SetLocalAlpha_C(a1, 2u, 0, (unsigned int)v2->f_vitre_local_alpha);
        v8 = (tGameObjectMain*)v2->o_body;
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
        AI_EvalFunc_SetALPHATRESH_C(v7, 0, 0, (unsigned int)v2->f_local_tresh_alpha);
        v6 = (tGameObjectMain*)v2->o_body;
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
        AI_EvalFunc_SetALPHATRESH_C(v5, 4u, 0, (unsigned int)v2->f_local_tresh_alpha);
        v4 = (tGameObjectMain*)v2->o_body;
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
        AI_EvalFunc_SetALPHATRESH_C(v3, 5u, 0, (unsigned int)v2->f_local_tresh_alpha);
    }
    return 1000;
}

int PNJ_Spectre_sp_after_disp()
{
    tScriptStruct_PNJSpectre* v1; // [esp+0h] [ebp-30h]
    tGameObjectMain* v2; // [esp+8h] [ebp-28h]
    tGameObjectMain* v3; // [esp+10h] [ebp-20h]
    tGameObjectMain* v4; // [esp+18h] [ebp-18h]
    tGameObjectMain* v5; // [esp+20h] [ebp-10h]
    tGameObjectMain* v6; // [esp+24h] [ebp-Ch]
    tGameObjectMain* v7; // [esp+28h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJSpectre*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    AI_EvalFunc_MATEnableMltxtLvl_C(AI_gpst_CurrentGameObject, v1->i_key_brasG, 0, 1);
    AI_EvalFunc_MATEnableMltxtLvl_C(AI_gpst_CurrentGameObject, v1->i_key_brasD, 0, 1);
    AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, 0x668748C0, v1->i_key_mask);
    v7 = (tGameObjectMain*)v1->o_body;
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
        v5 = (tGameObjectMain*)v1->o_body;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            AI_EvalFunc_SetLocalAlpha_C(v5, 2u, 0, 255);
        }
        else
        {
            AI_EvalFunc_SetLocalAlpha_C(0, 2u, 0, 255);
        }
        v4 = (tGameObjectMain*)v1->o_body;
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            AI_EvalFunc_SetALPHATRESH_C(v4, 0, 0, 255);
        }
        else
        {
            AI_EvalFunc_SetALPHATRESH_C(0, 0, 0, 255);
        }
        v3 = (tGameObjectMain*)v1->o_body;
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            AI_EvalFunc_SetALPHATRESH_C(v3, 4u, 0, 255);
        }
        else
        {
            AI_EvalFunc_SetALPHATRESH_C(0, 4u, 0, 255);
        }
        v2 = (tGameObjectMain*)v1->o_body;
        if (v2)
        {
            if ((unsigned int)v2 <= 2)
                v2 = AI_MainActor[(uint32_t)v2 - 1];
            AI_EvalFunc_SetALPHATRESH_C(v2, 5u, 0, 255);
        }
        else
        {
            AI_EvalFunc_SetALPHATRESH_C(0, 5u, 0, 255);
        }
    }
    return 1000;
}

int PNJ_Spectre_sp_after_blend()
{
    tScriptStruct_PNJSpectre* v1; // [esp+0h] [ebp-5Ch]
    tGameObjectMain* v2; // [esp+8h] [ebp-54h]
    tGameObjectMain* v3; // [esp+10h] [ebp-4Ch]
    tGameObjectMain* v4; // [esp+18h] [ebp-44h]
    tGameObjectMain* v5; // [esp+20h] [ebp-3Ch]
    tGameObjectMain* v6; // [esp+28h] [ebp-34h]
    Vec3 v7; // [esp+2Ch] [ebp-30h] BYREF
    tGameObjectMain* v8; // [esp+38h] [ebp-24h]
    Vec3 v9; // [esp+40h] [ebp-1Ch] BYREF
    Vec3 a3; // [esp+4Ch] [ebp-10h] BYREF
    tScriptStruct_PNJSpectre* v11; // [esp+58h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJSpectre*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v11 = v1;
    v8 = 0;
    if (v1->i_snap_le_gamin)
    {
        v8 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(3u);
        v6 = v8;
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v6 = AI_MainActor[(uint32_t)v8 - 1];
            AI_EvalFunc_OBJGetPos_C(v6, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        v5 = (tGameObjectMain*)v11->o_gamin_snap;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            AI_EvalFunc_OBJSetPos_C(v5, &a3);
        }
        else
        {
            AI_EvalFunc_OBJSetPos_C(0, &a3);
        }
        v4 = v8;
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v4 = AI_MainActor[(uint32_t)v8 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v4, &v9);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &v9);
        }
        v3 = v8;
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v3 = AI_MainActor[(uint32_t)v8 - 1];
            AI_EvalFunc_OBJ_BankingGet_C(v3, &v7);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGet_C(0, &v7);
        }
        v2 = (tGameObjectMain*)v11->o_gamin_snap;
        if (v2)
        {
            if ((unsigned int)v2 <= 2)
                v2 = AI_MainActor[(uint32_t)v2 - 1];
            AI_EvalFunc_OBJ_BankingGeneralSet_C(v2, &v9, &v7);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v9, &v7);
        }
    }
    else
    {
        v11->i_snap_le_gamin = v11->i_snap_le_gamin;
    }
    return 1000;
}

int PNJ_Spectre_sp_tag_on()
{
    char* v0; // eax
    float a4; // [esp+0h] [ebp-60h]
    int v3; // [esp+4h] [ebp-5Ch]
    tScriptStruct_PNJSpectre* v4; // [esp+8h] [ebp-58h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_PNJSpectre*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
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

int PNJ_Spectre_sp_exec_decode_paf_univ()
{
    tGameObjectMain* v0; // eax
    tGameObjectMain* v1; // eax
    float v3; // [esp+0h] [ebp-158h]
    tScriptStruct_PNJSpectre* v4; // [esp+4h] [ebp-154h]
    tAI* v5; // [esp+8h] [ebp-150h]
    tGameObjectMain* v6; // [esp+Ch] [ebp-14Ch]
    tGameObjectMain* v7; // [esp+10h] [ebp-148h]
    tGameObjectMain* v8; // [esp+18h] [ebp-140h]
    tGameObjectMain* v9; // [esp+20h] [ebp-138h]
    tAI* v10; // [esp+24h] [ebp-134h]
    tGameObjectMain* v11; // [esp+28h] [ebp-130h]
    tGameObjectMain* v12; // [esp+2Ch] [ebp-12Ch]
    tGameObjectMain* v13; // [esp+34h] [ebp-124h]
    tGameObjectMain* v14; // [esp+3Ch] [ebp-11Ch]
    double v15; // [esp+40h] [ebp-118h]
    tGameObjectMain* v16; // [esp+48h] [ebp-110h]
    tGameObjectMain* v17; // [esp+4Ch] [ebp-10Ch]
    double v18; // [esp+50h] [ebp-108h]
    tGameObjectMain* v19; // [esp+64h] [ebp-F4h]
    tGameObjectMain* v20; // [esp+68h] [ebp-F0h]
    tGameObjectMain* v21; // [esp+6Ch] [ebp-ECh]
    Vec3* v22; // [esp+70h] [ebp-E8h]
    Vec3* v23; // [esp+74h] [ebp-E4h]
    Vec3* v24; // [esp+80h] [ebp-D8h]
    Vec3* v25; // [esp+84h] [ebp-D4h]
    tGameObjectMain* v26; // [esp+A0h] [ebp-B8h]
    tGameObjectMain* v27; // [esp+A8h] [ebp-B0h]
    tGameObjectMain* v28; // [esp+ACh] [ebp-ACh]
    tGameObjectMain* v29; // [esp+B0h] [ebp-A8h]
    tGameObjectMain* v30; // [esp+B4h] [ebp-A4h]
    Vec3 v31; // [esp+B8h] [ebp-A0h] BYREF
    Vec3 dest; // [esp+C4h] [ebp-94h] BYREF
    Vec3 v33; // [esp+D0h] [ebp-88h] BYREF
    Vec3 pos; // [esp+ECh] [ebp-6Ch] BYREF
    int v35; // [esp+F8h] [ebp-60h]
    unsigned int v36; // [esp+100h] [ebp-58h]
    Vec3 v37; // [esp+104h] [ebp-54h] BYREF
    BOOL v39; // [esp+114h] [ebp-44h]
    int v40; // [esp+11Ch] [ebp-3Ch]
    Vec3 a3; // [esp+124h] [ebp-34h] BYREF
    tAI* v42; // [esp+134h] [ebp-24h]
    tScriptStruct_PNJSpectre* v43; // [esp+13Ch] [ebp-1Ch]
    int v44; // [esp+140h] [ebp-18h]
    int v45; // [esp+144h] [ebp-14h]
    float v46; // [esp+148h] [ebp-10h]
    Vec3 v47; // [esp+14Ch] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_PNJSpectre*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v43 = v4;
    v39 = 0;
    v40 = 0;
    v42 = 0;
    v44 = 0;
    v36 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;

        PNJ_Spectre_sp_exec_decode_paf_univ_GST_Fight_Manager_gao = AI_ul_RealResolveOneRefObject(0x9E002FFA);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_exec_decode_paf_univ_GST_Fight_Manager_gao_ao_Good_Actor,
            (void*)PNJ_Spectre_sp_exec_decode_paf_univ_GST_Fight_Manager_gao,
            0);
        PNJ_Spectre_sp_exec_decode_paf_univ_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_exec_decode_paf_univ_univVars_world_ID,
            (void*)PNJ_Spectre_sp_exec_decode_paf_univ_univVars,
            4);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_exec_decode_paf_univ_univVars_Flag_speciaux_sauve,
            (void*)PNJ_Spectre_sp_exec_decode_paf_univ_univVars,
            4488);
        PNJ_Spectre_sp_exec_decode_paf_univ__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        fct_PNJ_Spectre_sp_exec_decode_paf_univ_PNJ_Spectre_sp_mode_mort = (int)AI_ul_RealResolveOneRefFunction(0x9E006695);
        fct_PNJ_Spectre_sp_exec_decode_paf_univ_PNJ_Spectre_sp_mode_paf = (int)AI_ul_RealResolveOneRefFunction(0x9E006699);
    }
    v36 = (unsigned int)v43->t_mes.gao;
    v43->i_paf_type = v43->t_mes.Int1;
    v44 = v43->i_paf_type & 7;
    if (v44 == 2 || (v43->i_paf_type & 0x40000000) != 0)
        return 1000;
    v35 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(AI_gpst_CurrentGameObject, 1, -1.0, 0.89999998f, 1.15f);
    if (v43->i_jattaque_le_dernier_qui_mef)
    {
        v40 = -1;
        v30 = (tGameObjectMain*)v36;
        if (v36)
        {
            if (v36 <= 2)
                v30 = AI_MainActor[v36 - 1];
            v29 = v30;
        }
        else
        {
            v29 = 0;
        }
        if (v29)
        {
            v28 = (tGameObjectMain*)v36;
            if (v36)
            {
                if (v36 <= 2)
                    v28 = AI_MainActor[v36 - 1];
                v27 = v28;
            }
            else
            {
                v27 = 0;
            }
            v40 = AI_EvalFunc_ARRObjSearch_C(
                (tGameObjectMain**)(PNJ_Spectre_sp_exec_decode_paf_univ_GST_Fight_Manager_gao_ao_Good_Actor + 4),
                12,
                v27);
        }
        if (v43->i_NMI == 1 && v40 != -1)
            v43->o_NMI = v36;
    }
    v26 = (tGameObjectMain*)v43->WP_pos_combat;
    if (v26)
    {
        if ((unsigned int)v26 <= 2)
            v26 = AI_MainActor[(uint32_t)v26 - 1];
        AI_EvalFunc_OBJGetPos_C(v26, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v33);
    MATH_SubstractVec(&dest, &a3, &v33);
    v43->v_paf_sens_coup = dest;
    v43->v_paf_sens_coup.z = 0.0;
    if (AI_EvalFunc_MTHVecNullEpsilon_C(&v43->v_paf_sens_coup))
    {
        v24 = &v43->v_paf_sens_coup;
        v25 = &v43->t_mes.v1;
        v43->v_paf_sens_coup.x = v43->t_mes.v1.x;
        v24->y = v25->y;
        v24->z = v25->z;
    }
    else
    {
        MATH_NormalizeEqualVector(&v43->v_paf_sens_coup);
    }
    v46 = v43->t_mes.v1.z * v43->v_paf_sens_coup.z
        + v43->t_mes.v1.y * v43->v_paf_sens_coup.y
        + v43->v_paf_sens_coup.x * v43->t_mes.v1.x;
    if (v46 < 0.0)
    {
        v23 = &v43->v_paf_sens_coup;
        v22 = &v43->v_paf_sens_coup;
        v43->v_paf_sens_coup.x = -v43->v_paf_sens_coup.x;
        v22->y = -v23->y;
        v22->z = -v23->z;
    }
    v21 = (tGameObjectMain*)v43->o_Monstro;
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
        v19 = (tGameObjectMain*)v43->o_Monstro;
        if (v19)
        {
            if ((unsigned int)v19 <= 2)
                v19 = AI_MainActor[(uint32_t)v19 - 1];
            AI_EvalFunc_OBJ_CapaSet_C(v19, 2, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_CapaSet_C(0, 2, 0);
        }
    }
    if (*(uint32_t*)PNJ_Spectre_sp_exec_decode_paf_univ_univVars_world_ID == 181)
    {
        if (v44 > 2)
            v43->i_paf_nb_coup += 10000;
    }
    else
    {
        v39 = AI_EvalFunc_ARRBitTest_C((int*)(PNJ_Spectre_sp_exec_decode_paf_univ_univVars_Flag_speciaux_sauve + 4), 2764);
        if (v39)
        {
            v17 = (tGameObjectMain*)v36;
            if (v36)
            {
                if (v36 <= 2)
                    v17 = AI_MainActor[v36 - 1];
                v16 = v17;
            }
            else
            {
                v16 = 0;
            }
            if (v16 == (tGameObjectMain*)PNJ_Spectre_sp_exec_decode_paf_univ__PJ_Sally_gao)
            {
                if ((v43->i_paf_type & 0x2000000) != 0)
                    v43->i_paf_nb_coup += 15;
                else
                    v43->i_paf_nb_coup += 3;
            }
            else if (v43->i_NMI == 2)
            {
                v3 = (float)v43->Nb_Coup;
                v15 = v3 * 0.5 + MATH_gd_magicDoubleForRound;
                v43->i_paf_nb_coup += (*(uint32_t*)&v15);
            }
            else
            {
                v43->i_paf_nb_coup += 3;
            }
        }
        else
        {
            v18 = v43->t_mes.f2 / 20.0 + MATH_gd_magicDoubleForRound;
            v43->i_paf_nb_coup += (*(uint32_t *)&v18);
        }
    }
    if (v43->i_paf_nb_coup < v43->Nb_Coup
        || (v45 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(AI_gpst_CurrentGameObject, 7, -1.0, 0.94999999f, 1.05f),
            v43->i_paf_type < 0))
    {
        if (v43->i_mode != 5)
            v43->i_mode_avt_paf = v43->i_mode;
        if (v43->i_mode_avt_paf == 1)
        {
            v37.x = 0.0;
            v37.y = 0.0;
            v37.z = 0.0;
            AI_EvalFunc_DYNTractionSet_C_CURRENT(&v37);
            v31.x = 0.0;
            v31.y = 0.0;
            v31.z = -20.0;
            AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v31);
            AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0xFFu, 0);
            AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 1, 64);
        }
        v43->i_mode = -1;
        AI_EvalFunc_AITrackChange_C(2, (tAIFunction*)fct_PNJ_Spectre_sp_exec_decode_paf_univ_PNJ_Spectre_sp_mode_paf);
        v43->i_mode = -1;
        return 1000;
    }
    if (v43->i_brasD_on)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
        v14 = (tGameObjectMain*)v43->des_bras_droit;
        if (v43->des_bras_droit)
        {
            if ((unsigned int)v14 <= 2)
                v14 = AI_MainActor[(uint32_t)v14 - 1];
            v0 = AI_EvalFunc_OBJDuplicate_C(v14, &pos);
        }
        else
        {
            v0 = AI_EvalFunc_OBJDuplicate_C(0, &pos);
        }
        v42 = (tAI*)v0;
        v13 = v0;
        if (v0)
        {
            if ((unsigned int)v0 <= 2)
                v13 = AI_MainActor[(uint32_t)v0 - 1];
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(v13, 1);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
        }
        if (v42 == WOR_gst_Universe.univAIInstance)
        {
            v10 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v12 = (tGameObjectMain*)v42;
            if (v42)
            {
                if ((unsigned int)v42 <= 2)
                    v12 = AI_MainActor[(uint32_t)v42 - 1];
                v11 = v12;
            }
            else
            {
                v11 = 0;
            }
            v10 = v11->extended->AIInstance;
        }
        ((tScriptStruct_PNJSectreBras*)(v10->instanceVarDataCurrent))->des_papa = (int)AI_gpst_CurrentGameObject;
        v43->i_brasD_on = 0;
    }
    if (v43->i_brasG_on)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v47);
        v9 = (tGameObjectMain*)v43->des_bras_gauche;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            v1 = AI_EvalFunc_OBJDuplicate_C(v9, &v47);
        }
        else
        {
            v1 = AI_EvalFunc_OBJDuplicate_C(0, &v47);
        }
        v42 = (tAI*)v1;
        v8 = v1;
        if (v1)
        {
            if ((unsigned int)v1 <= 2)
                v8 = AI_MainActor[(uint32_t)v1 - 1];
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(v8, 1);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
        }
        if (v42 == WOR_gst_Universe.univAIInstance)
        {
            v5 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v7 = (tGameObjectMain*)v42;
            if (v42)
            {
                if ((unsigned int)v42 <= 2)
                    v7 = AI_MainActor[(uint32_t)v42 - 1];
                v6 = v7;
            }
            else
            {
                v6 = 0;
            }
            v5 = v6->extended->AIInstance;
        }
        ((tScriptStruct_PNJSectreBras*)(v5->instanceVarDataCurrent))->des_papa = (int)AI_gpst_CurrentGameObject;
        v43->i_brasG_on = 0;
    }
    AI_EvalFunc_AITrackChange_C(2, (tAIFunction*)fct_PNJ_Spectre_sp_exec_decode_paf_univ_PNJ_Spectre_sp_mode_mort);
    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
    return 1000;
}

int PNJ_Spectre_sp_mode_paf()
{
    int result; // eax
    float v1; // [esp+0h] [ebp-268h]
    tScriptStruct_PNJSpectre* v2; // [esp+Ch] [ebp-25Ch]
    tGameObjectMain* v3; // [esp+14h] [ebp-254h]
    tGameObjectMain* v4; // [esp+1Ch] [ebp-24Ch]
    tGameObjectMain* v5; // [esp+24h] [ebp-244h]
    tGameObjectMain* v6; // [esp+2Ch] [ebp-23Ch]
    tGameObjectMain* v7; // [esp+34h] [ebp-234h]
    float v8; // [esp+38h] [ebp-230h]
    tGameObjectMain* v9; // [esp+50h] [ebp-218h]
    tGameObjectMain* v10; // [esp+58h] [ebp-210h]
    tGameObjectMain* v11; // [esp+68h] [ebp-200h]
    tGameObjectMain* v12; // [esp+74h] [ebp-1F4h]
    tGameObjectMain* v13; // [esp+78h] [ebp-1F0h]
    tGameObjectMain* v14; // [esp+7Ch] [ebp-1ECh]
    tGameObjectMain* v15; // [esp+80h] [ebp-1E8h]
    tGameObjectMain* v16; // [esp+88h] [ebp-1E0h]
    Vec3 v17; // [esp+8Ch] [ebp-1DCh] BYREF
    Vec3 v18; // [esp+98h] [ebp-1D0h] BYREF
    Vec3 v19; // [esp+A4h] [ebp-1C4h] BYREF
    Vec3 v20; // [esp+B0h] [ebp-1B8h] BYREF
    Vec3 v21; // [esp+BCh] [ebp-1ACh] BYREF
    Vec3 v22; // [esp+C8h] [ebp-1A0h] BYREF
    Vec3 v23; // [esp+D4h] [ebp-194h] BYREF
    Vec3 v24; // [esp+E0h] [ebp-188h] BYREF
    Vec3 v25; // [esp+ECh] [ebp-17Ch] BYREF
    Vec3 v26; // [esp+F8h] [ebp-170h] BYREF
    Vec3 a3; // [esp+104h] [ebp-164h] BYREF
    Vec3 v28; // [esp+114h] [ebp-154h] BYREF
    Vec3 v29; // [esp+120h] [ebp-148h] BYREF
    Vec3 a4; // [esp+12Ch] [ebp-13Ch] BYREF
    Vec3 dest; // [esp+138h] [ebp-130h] BYREF
    Vec3 v32; // [esp+144h] [ebp-124h] BYREF
    Vec3 v33; // [esp+150h] [ebp-118h] BYREF
    Vec3 v34; // [esp+15Ch] [ebp-10Ch] BYREF
    Vec3 v35; // [esp+168h] [ebp-100h] BYREF
    Vec3 v36; // [esp+174h] [ebp-F4h] BYREF
    Vec3 v37; // [esp+180h] [ebp-E8h] BYREF
    Vec3 v38; // [esp+18Ch] [ebp-DCh] BYREF
    Vec3 a1; // [esp+198h] [ebp-D0h] BYREF
    float v40; // [esp+1A4h] [ebp-C4h]
    float v41; // [esp+1A8h] [ebp-C0h]
    int v42; // [esp+1ACh] [ebp-BCh]
    Vec3 v43; // [esp+1B0h] [ebp-B8h] BYREF
    int v44; // [esp+1C8h] [ebp-A0h]
    Vec3 v45; // [esp+1CCh] [ebp-9Ch] BYREF
    Vec3 v46; // [esp+1D8h] [ebp-90h] BYREF
    Vec3 v47; // [esp+1E4h] [ebp-84h] BYREF
    Vec3 v48; // [esp+1F0h] [ebp-78h] BYREF
    int v49; // [esp+1FCh] [ebp-6Ch]
    Vec3 v50; // [esp+200h] [ebp-68h] BYREF
    Vec3 v51; // [esp+20Ch] [ebp-5Ch] BYREF
    int v52; // [esp+218h] [ebp-50h]
    Vec3 v53; // [esp+21Ch] [ebp-4Ch] BYREF
    tScriptStruct_PNJSpectre* v54; // [esp+228h] [ebp-40h]
    int v55; // [esp+22Ch] [ebp-3Ch]
    Vec3 v56; // [esp+230h] [ebp-38h] BYREF
    Vec3 v57; // [esp+23Ch] [ebp-2Ch] BYREF
    float v59; // [esp+24Ch] [ebp-1Ch]
    int v60; // [esp+250h] [ebp-18h]
    Vec3 v61; // [esp+254h] [ebp-14h] BYREF
    int v62; // [esp+260h] [ebp-8h]
    float v63; // [esp+264h] [ebp-4h]

    v46.x = 1.0;
    v46.y = 0.0;
    v46.z = 0.0;
    v35.x = 1.0;
    v35.y = 0.0;
    v35.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJSpectre*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v54 = v2;
    v52 = 0;
    v55 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        PNJ_Spectre_sp_mode_paf_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        fct_PNJ_Spectre_sp_mode_paf_PNJ_Spectre_sp_mode_combat = (int)AI_ul_RealResolveOneRefFunction(0x9E006691);
    }
    if (v54->i_mode != 5)
    {
        v54->f_color_duration = 0.0;
        v52 = AI_EvalFunc_GFXAdd_C_CURRENT(9);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v52, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v52, 2, 1);
        v16 = (tGameObjectMain*)PNJ_Spectre_sp_mode_paf_SFX_light_smoke_gao;
        if (PNJ_Spectre_sp_mode_paf_SFX_light_smoke_gao)
        {
            if ((unsigned int)PNJ_Spectre_sp_mode_paf_SFX_light_smoke_gao <= 2)
                v16 = AI_MainActor[PNJ_Spectre_sp_mode_paf_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v52, v16, 5);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v52, 0, 5);
        }
        GFXSetCurGlobal(v52);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9000, 2.0943961f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9001, 0.5);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9002, 6.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9003, 0.94999999f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9004, 0.039999999f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9005, 0.2f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9006, 1.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9007, 3.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9008, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9009, 0.2f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9010, 1.0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9100, 30);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9101, 0xC06BAFE3);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9102, 24);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v51);
        v43.x = 0.0;
        v43.y = 0.0;
        v43.z = 1.5;
        MATH_VecAddVec(&v20, &v51, &v43);
        AI_EvalFunc_GFXSetv_C_CURGLOB(9200, &v20);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v19);
        AI_EvalFunc_GFXSetv_C_CURGLOB(9201, &v19);
        v33.x = 0.0;
        v33.y = 0.0;
        v33.z = -1.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(9202, &v33);
        v15 = (tGameObjectMain*)v54->o_NMI;
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
        v14->extended->fields->designFlags &= 0xFFBFFFFF;
        v55 = v54->i_paf_type & 7;
        v13 = (tGameObjectMain*)v54->o_me;
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
        v12->extended->fields->designInt2 = v54->i_flag_order;
        if (v55)
        {
            if (++v54->i_sens_paf > 6)
                v54->i_sens_paf = 4;
            AI_EvalFunc_ACTActionSet_C_CURRENT(v54->i_sens_paf);
            v46 = v54->v_paf_sens_coup;
            if (v54->i_paf_type >= 0)
                AI_EvalFunc_MATHVecSetNormVector_C(&v46, 4.0);
            else
                AI_EvalFunc_MATHVecSetNormVector_C(&v46, 3.0);
            v60 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v46);
            v24.x = 5.0;
            v24.y = 5.0;
            v24.z = 1.0;
            v44 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v24);
        }
        v54->i_mode_avt_paf = v54->i_mode;
        v54->i_mode = 5;
    }
    if (AI_EvalFunc_ACTActionFinished_C_CURRENT())
    {
        v26.x = 0.0;
        v26.y = 0.0;
        v26.z = 0.0;
        v42 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v26);
        v22.x = 5.0;
        v22.y = 5.0;
        v22.z = 1.0;
        v62 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v22);
        AI_EvalFunc_ACTActionSet_C_CURRENT(0);
        v54->i_mode = v54->i_mode_avt_paf;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v37);
        v11 = (tGameObjectMain*)v54->WP_pos_combat;
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
        MATH_SubstractVec(&dest, &v37, &a3);
        a1.x = dest.x;
        a1.y = dest.y;
        a1.z = 0.0;
        v10 = (tGameObjectMain*)v54->o_NMI;
        if (v10)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            AI_EvalFunc_OBJGetPos_C(v10, &v18);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v18);
        }
        v9 = (tGameObjectMain*)v54->WP_pos_combat;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            AI_EvalFunc_OBJGetPos_C(v9, &v32);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v32);
        }
        MATH_SubstractVec(&v47, &v18, &v32);
        v35.x = v47.x;
        v35.y = v47.y;
        v35.z = 0.0;
        v41 = MATH_VecLen(&a1);
        v63 = MATH_VecLen(&v35);
        v54->f_pos_dist = v41 / v63;
        v54->v_paf_speed.x = 0.0;
        v54->v_paf_speed.y = 0.0;
        v54->v_paf_speed.z = 0.0;
        v54->f_pos_dt = 0.0;
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_PNJ_Spectre_sp_mode_paf_PNJ_Spectre_sp_mode_combat);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v29);
        AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v28);
        v8 = v54->f_dt;
        v45.x = v28.x * v8;
        v45.y = v28.y * v8;
        v45.z = v28.z * v8;
        MATH_VecAddVec(&v21, &v29, &v45);
        v7 = (tGameObjectMain*)v54->WP_pos_combat;
        if (v7)
        {
            if ((unsigned int)v7 <= 2)
                v7 = AI_MainActor[(uint32_t)v7 - 1];
            AI_EvalFunc_OBJGetPos_C(v7, &v57);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v57);
        }
        MATH_SubstractVec(&v34, &v21, &v57);
        v6 = (tGameObjectMain*)v54->o_NMI;
        if (v6)
        {
            if ((unsigned int)v6 <= 2)
                v6 = AI_MainActor[(uint32_t)v6 - 1];
            AI_EvalFunc_OBJGetPos_C(v6, &v53);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v53);
        }
        v5 = (tGameObjectMain*)v54->WP_pos_combat;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            AI_EvalFunc_OBJGetPos_C(v5, &v36);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v36);
        }
        MATH_SubstractVec(&v38, &v53, &v36);
        v59 = v38.z * v34.z + v38.y * v34.y + v34.x * v38.x;
        if (v59 < 0.0)
        {
            v4 = (tGameObjectMain*)v54->WP_pos_combat;
            if (v4)
            {
                if ((unsigned int)v4 <= 2)
                    v4 = AI_MainActor[(uint32_t)v4 - 1];
                AI_EvalFunc_OBJGetPos_C(v4, &v23);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v23);
            }
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v23);
            v61.x = 0.0;
            v61.y = 0.0;
            v61.z = 0.0;
            v49 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v61);
        }
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v17);
        v3 = (tGameObjectMain*)v54->o_NMI;
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            AI_EvalFunc_OBJGetPos_C(v3, &v25);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v25);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v50);
        MATH_SubstractVec(&v56, &v25, &v50);
        v40 = TIM_gf_dt;
        v1 = 6.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlend_C(&v17, &v56, v1, &a4);
        v48.x = 0.0;
        v48.y = 0.0;
        v48.z = 1.0;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a4, &v48);
        result = 1000;
    }
    return result;
}

int PNJ_Spectre_sp_mode_apparition_xmen()
{
    tGameObjectMain* v0; // eax
    tGameObjectMain* v1; // eax
    float v3; // [esp+4h] [ebp-37Ch]
    float v4; // [esp+8h] [ebp-378h]
    tScriptStruct_PNJSpectre* v5; // [esp+14h] [ebp-36Ch]
    tGameObjectMain* v6; // [esp+1Ch] [ebp-364h]
    float v7; // [esp+20h] [ebp-360h]
    float v8; // [esp+24h] [ebp-35Ch]
    tGameObjectMain* v9; // [esp+2Ch] [ebp-354h]
    tGameObjectMain* v10; // [esp+34h] [ebp-34Ch]
    tAI* v11; // [esp+38h] [ebp-348h]
    tGameObjectMain* v12; // [esp+3Ch] [ebp-344h]
    tGameObjectMain* v13; // [esp+40h] [ebp-340h]
    tAI* v14; // [esp+44h] [ebp-33Ch]
    tGameObjectMain* v15; // [esp+48h] [ebp-338h]
    tGameObjectMain* v16; // [esp+4Ch] [ebp-334h]
    tAI* v17; // [esp+50h] [ebp-330h]
    tGameObjectMain* v18; // [esp+54h] [ebp-32Ch]
    tGameObjectMain* v19; // [esp+58h] [ebp-328h]
    tAI* v20; // [esp+5Ch] [ebp-324h]
    tGameObjectMain* v21; // [esp+60h] [ebp-320h]
    tGameObjectMain* v22; // [esp+64h] [ebp-31Ch]
    tAI* v23; // [esp+68h] [ebp-318h]
    tGameObjectMain* v24; // [esp+6Ch] [ebp-314h]
    tGameObjectMain* v25; // [esp+70h] [ebp-310h]
    tAI* v26; // [esp+74h] [ebp-30Ch]
    tGameObjectMain* v27; // [esp+78h] [ebp-308h]
    tGameObjectMain* v28; // [esp+7Ch] [ebp-304h]
    tAI* v29; // [esp+80h] [ebp-300h]
    tGameObjectMain* v30; // [esp+84h] [ebp-2FCh]
    tGameObjectMain* v31; // [esp+88h] [ebp-2F8h]
    tAI* v32; // [esp+8Ch] [ebp-2F4h]
    tGameObjectMain* v33; // [esp+90h] [ebp-2F0h]
    tGameObjectMain* v34; // [esp+94h] [ebp-2ECh]
    tAI* v35; // [esp+98h] [ebp-2E8h]
    tGameObjectMain* v36; // [esp+9Ch] [ebp-2E4h]
    tGameObjectMain* v37; // [esp+A0h] [ebp-2E0h]
    tAI* v38; // [esp+A4h] [ebp-2DCh]
    tGameObjectMain* v39; // [esp+A8h] [ebp-2D8h]
    tGameObjectMain* v40; // [esp+ACh] [ebp-2D4h]
    tAI* v41; // [esp+B0h] [ebp-2D0h]
    tGameObjectMain* v42; // [esp+B4h] [ebp-2CCh]
    tGameObjectMain* v43; // [esp+B8h] [ebp-2C8h]
    tAI* v44; // [esp+BCh] [ebp-2C4h]
    tGameObjectMain* v45; // [esp+C0h] [ebp-2C0h]
    tGameObjectMain* v46; // [esp+C4h] [ebp-2BCh]
    tAI* v47; // [esp+C8h] [ebp-2B8h]
    tGameObjectMain* v48; // [esp+CCh] [ebp-2B4h]
    tGameObjectMain* v49; // [esp+D0h] [ebp-2B0h]
    tAI* v50; // [esp+D4h] [ebp-2ACh]
    tGameObjectMain* v51; // [esp+D8h] [ebp-2A8h]
    tGameObjectMain* v52; // [esp+DCh] [ebp-2A4h]
    tAI* v53; // [esp+E0h] [ebp-2A0h]
    tGameObjectMain* v54; // [esp+E4h] [ebp-29Ch]
    tGameObjectMain* v55; // [esp+E8h] [ebp-298h]
    tAI* v56; // [esp+ECh] [ebp-294h]
    tGameObjectMain* v57; // [esp+F0h] [ebp-290h]
    tGameObjectMain* v58; // [esp+F4h] [ebp-28Ch]
    tAI* v59; // [esp+F8h] [ebp-288h]
    tGameObjectMain* v60; // [esp+FCh] [ebp-284h]
    tGameObjectMain* v61; // [esp+100h] [ebp-280h]
    tAI* v62; // [esp+104h] [ebp-27Ch]
    tGameObjectMain* v63; // [esp+108h] [ebp-278h]
    tGameObjectMain* v64; // [esp+10Ch] [ebp-274h]
    tAI* v65; // [esp+110h] [ebp-270h]
    tGameObjectMain* v66; // [esp+114h] [ebp-26Ch]
    tGameObjectMain* v67; // [esp+118h] [ebp-268h]
    tAI* v68; // [esp+11Ch] [ebp-264h]
    tGameObjectMain* v69; // [esp+120h] [ebp-260h]
    tGameObjectMain* v70; // [esp+124h] [ebp-25Ch]
    tAI* v71; // [esp+128h] [ebp-258h]
    tGameObjectMain* v72; // [esp+12Ch] [ebp-254h]
    tGameObjectMain* v73; // [esp+130h] [ebp-250h]
    tGameObjectMain* v74; // [esp+138h] [ebp-248h]
    tGameObjectMain* v75; // [esp+144h] [ebp-23Ch]
    tGameObjectMain* v76; // [esp+148h] [ebp-238h]
    tGameObjectMain* v77; // [esp+150h] [ebp-230h]
    tGameObjectMain* v78; // [esp+154h] [ebp-22Ch]
    tGameObjectMain* v79; // [esp+158h] [ebp-228h]
    tGameObjectMain* v80; // [esp+170h] [ebp-210h]
    tGameObjectMain* v81; // [esp+174h] [ebp-20Ch]
    tGameObjectMain* v82; // [esp+178h] [ebp-208h]
    tGameObjectMain* v83; // [esp+17Ch] [ebp-204h]
    tGameObjectMain* v84; // [esp+1E4h] [ebp-19Ch]
    tGameObjectMain* v85; // [esp+1E8h] [ebp-198h]
    tGameObjectMain* v86; // [esp+1ECh] [ebp-194h]
    tGameObjectMain* v87; // [esp+1F0h] [ebp-190h]
    float v88; // [esp+200h] [ebp-180h]
    float v89; // [esp+20Ch] [ebp-174h]
    Vec3 pos; // [esp+210h] [ebp-170h] BYREF
    tAI* v91; // [esp+21Ch] [ebp-164h]
    Vec3 v92; // [esp+22Ch] [ebp-154h] BYREF
    int v93; // [esp+248h] [ebp-138h]
    tGameObjectMain* v94; // [esp+24Ch] [ebp-134h]
    int v95; // [esp+250h] [ebp-130h]
    int v96; // [esp+254h] [ebp-12Ch]
    int v97; // [esp+258h] [ebp-128h]
    float v98; // [esp+25Ch] [ebp-124h]
    Vec3 a3; // [esp+26Ch] [ebp-114h] BYREF
    Vec3 v100; // [esp+278h] [ebp-108h] BYREF
    Vec3 a2; // [esp+284h] [ebp-FCh] BYREF
    Vec3 v102; // [esp+290h] [ebp-F0h] BYREF
    Vec3 v103; // [esp+29Ch] [ebp-E4h]
    Vec3 v104; // [esp+2A8h] [ebp-D8h] BYREF
    float v105; // [esp+2B8h] [ebp-C8h]
    int v106; // [esp+2C0h] [ebp-C0h]
    Vec3 v107; // [esp+2C8h] [ebp-B8h] BYREF
    int v108; // [esp+2D8h] [ebp-A8h]
    int v109; // [esp+2DCh] [ebp-A4h]
    int v110; // [esp+2E0h] [ebp-A0h]
    Vec3 a4; // [esp+2F0h] [ebp-90h] BYREF
    tGameObjectMain* v113; // [esp+300h] [ebp-80h]
    Vec3 a1; // [esp+310h] [ebp-70h] BYREF
    Vec3 v115; // [esp+31Ch] [ebp-64h] BYREF
    unsigned int i; // [esp+32Ch] [ebp-54h]
    tGameObjectMain* v117; // [esp+33Ch] [ebp-44h]
    float v118; // [esp+340h] [ebp-40h]
    float v119; // [esp+344h] [ebp-3Ch]
    tScriptStruct_PNJSpectre* v120; // [esp+348h] [ebp-38h]
    int v121; // [esp+34Ch] [ebp-34h]
    float v122; // [esp+36Ch] [ebp-14h]
    float v123; // [esp+374h] [ebp-Ch]
    int v124; // [esp+37Ch] [ebp-4h]

    v100.x = 1.0;
    v100.y = 0.0;
    v100.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_PNJSpectre*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v120 = v5;
    v91 = 0;
    v93 = 0;
    v108 = 0;
    v109 = 0;
    v95 = 0;
    v110 = 0;
    v96 = 0;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao = AI_ul_RealResolveOneRefObject(0x9E002FFA);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ao_Evil_Actor,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            520);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_NRJ_Courante,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            892);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_i_Evil_NbActor,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            2624);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_i_Evil_NbActorAlive,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            2632);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_NRJShieldMax,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            768);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_o_last_mort,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            3788);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ao_Good_Actor,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            0);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ao_Evil_Target,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            644);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ai_Good_Targeted,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            416);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ai_Evil_Targeted,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            1636);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ao_Good_Target,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            52);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_Dmin,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            1884);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_DMax,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            2008);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_Angle,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            2132);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ai_Evil_Icone,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            1760);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_Last_Hit,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            1016);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ao_Evil_Last_Good,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            1140);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_Last_Dmg,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            1264);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_Last_Eff,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            1388);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_Cumul_Dmg,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            1512);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_o_musique_fight,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao,
            2660);
        PNJ_Spectre_sp_mode_apparition_xmen_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &PNJ_Spectre_sp_mode_apparition_xmen_GST_Global_gao_o_musique_fight_list,
            (void*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Global_gao,
            3460);
        PNJ_Spectre_sp_mode_apparition_xmen_DUP_SFX_XMEN_Dark_gao = AI_ul_RealResolveOneRefObject(0x2600363Bu);
        PNJ_Spectre_sp_mode_apparition_xmen_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        fct_PNJ_Spectre_sp_mode_apparition_xmen_PNJ_Spectre_sp_tag_off = (int)AI_ul_RealResolveOneRefFunction(0x26003615u);
        fct_PNJ_Spectre_sp_mode_apparition_xmen_PNJ_Spectre_sp_reflex = (int)AI_ul_RealResolveOneRefFunction(0x9E006692);
        fct_PNJ_Spectre_sp_mode_apparition_xmen_PNJ_Spectre_sp_tag_on = (int)AI_ul_RealResolveOneRefFunction(0x26003614u);
        fct_PNJ_Spectre_sp_mode_apparition_xmen_PNJ_Spectre_sp_mode_combat = (int)AI_ul_RealResolveOneRefFunction(0x9E006691);
        fct_PNJ_Spectre_sp_mode_apparition_xmen_PNJ_Spectre_sp_after_blend = (int)AI_ul_RealResolveOneRefFunction(0x9E00669B);
    }
    if (!v120->i_fight_on)
    {
        v120->i_fight_on = 1;
        AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(4096, 0);
        v109 = AI_EvalFunc_ARRObjSearch_C(
            (tGameObjectMain**)(PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ao_Evil_Actor + 4),
            30,
            0);
        if (v109 != -1)
        {
            v117 = AI_gpst_CurrentGameObject;
            v87 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v87 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v86 = v87;
            }
            else
            {
                v86 = 0;
            }
            v110 = v86->extended->fields->designFlags & 0x7FFFFFFF;
            v113 = AI_gpst_CurrentGameObject;
            v85 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v85 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v84 = v85;
            }
            else
            {
                v84 = 0;
            }
            v84->extended->fields->designFlags = v110;
            *(uint32_t*)(PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ao_Evil_Actor + 4 * v109 + 4) = (uint32_t)AI_gpst_CurrentGameObject;
            *(uint32_t*)AI_EvalVarArray_C2(
                (char*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_NRJ_Courante,
                v109) = 1117782016;
            if (!*(uint32_t*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_i_Evil_NbActor)
                *(uint32_t*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_i_Evil_NbActorAlive = 0;
            ++* (uint32_t*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_i_Evil_NbActorAlive;
            ++* (uint32_t*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_i_Evil_NbActor;
            *(uint32_t*)AI_EvalVarArray_C2(
                (char*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_NRJShieldMax,
                v109) = 1117782016;
        }
        *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_Dmin, v109) = 0x40400000;
        *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_DMax, v109) = 0x40C00000;
        *(uint32_t*)AI_EvalVarArray_C2((char*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_Angle, v109) = 0x40490FDC;
        *(uint32_t*)(PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ai_Evil_Icone + 4 * v109 + 4) = 9;
        *(uint32_t*)AI_EvalVarArray_C2(
            (char*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_Last_Hit,
            v109) = 0;
        *(uint32_t*)(PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_ao_Evil_Last_Good + 4 * v109 + 4) = 0;
        *(uint32_t*)AI_EvalVarArray_C2(
            (char*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_Last_Dmg,
            v109) = 0;
        *(uint32_t*)AI_EvalVarArray_C2(
            (char*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_Last_Eff,
            v109) = 0;
        *(uint32_t*)AI_EvalVarArray_C2(
            (char*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_af_Evil_Cumul_Dmg,
            v109) = 0;
        v83 = (tGameObjectMain*)v120->acteur_musique_fight;
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
            v81 = (tGameObjectMain*)v120->acteur_musique_fight;
            if (v81)
            {
                if ((unsigned int)v81 <= 2)
                    v81 = AI_MainActor[(uint32_t)v81 - 1];
                v80 = v81;
            }
            else
            {
                v80 = 0;
            }
            if (v80 == (tGameObjectMain*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao)
                *(uint32_t*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_o_musique_fight = *(uint32_t*)(PNJ_Spectre_sp_mode_apparition_xmen_GST_Global_gao_o_musique_fight_list + 44);
            else
                *(uint32_t*)PNJ_Spectre_sp_mode_apparition_xmen_GST_Fight_Manager_gao_o_musique_fight = v120->acteur_musique_fight;
        }
    }
    v108 = 0;
    if (v120->i_xmen_nbr)
    {
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v104);
        v89 = AI_EvalFunc_MATHRandFloat_C(1.0, 3.5);
        v103.x = v104.x * v89;
        v103.y = v104.y * v89;
        v103.z = v104.z * v89;
        v100 = v103;
        v100.z = AI_EvalFunc_MATHRandFloat_C(1.0, -1.0) + v100.z;
        v107.x = 0.0;
        v107.y = 0.0;
        v107.z = 1.0;
        v122 = AI_EvalFunc_MATHRandFloat_C(-1.0471981f, 1.0471981f);
        AI_EvalFunc_MATHVecRotate_C(&v100, &v107, v122, &a4);
        v100 = a4;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
        a3.x = 0.0;
        a3.y = 0.0;
        a3.z = 1.2f;
        MATH_VecAddVec(&a1, &a2, &a3);
        MATH_VecAddVec(&pos, &a1, &v100);
        v91 = (tAI*)AI_EvalFunc_OBJDuplicate_C(
            (tGameObjectMain*)PNJ_Spectre_sp_mode_apparition_xmen_DUP_SFX_XMEN_Dark_gao,
            &pos);
        v79 = (tGameObjectMain*)v120->o_xmen_father;
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
            v77 = (tGameObjectMain*)v120->o_xmen_father;
            if (v77)
            {
                if ((unsigned int)v77 <= 2)
                    v77 = AI_MainActor[(uint32_t)v77 - 1];
                v0 = AI_EvalFunc_ANICanalGaoGet_C(v77, v120->i_xmen_father_bone_num);
            }
            else
            {
                v0 = AI_EvalFunc_ANICanalGaoGet_C(0, v120->i_xmen_father_bone_num);
            }
            for (i = (unsigned int)v0; ; i = (unsigned int)v1)
            {
                v76 = (tGameObjectMain*)i;
                if (i)
                {
                    if (i <= 2)
                        v76 = AI_MainActor[i - 1];
                    v75 = v76;
                }
                else
                {
                    v75 = 0;
                }
                if (v75)
                    break;
                v121 = AI_EvalFunc_MATHRandInt_C(1, 5);
                v106 = AI_EvalFunc_MTHModulo_C(v121 + v120->i_xmen_father_bone_num, 20);
                v120->i_xmen_father_bone_num = v106;
                v74 = (tGameObjectMain*)v120->o_xmen_father;
                if (v74)
                {
                    if ((unsigned int)v74 <= 2)
                        v74 = AI_MainActor[(uint32_t)v74 - 1];
                    v1 = AI_EvalFunc_ANICanalGaoGet_C(v74, v120->i_xmen_father_bone_num);
                }
                else
                {
                    v1 = AI_EvalFunc_ANICanalGaoGet_C(0, v120->i_xmen_father_bone_num);
                }
            }
            if (v91 == WOR_gst_Universe.univAIInstance)
            {
                v71 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v73 = (tGameObjectMain*)v91;
                if (v91)
                {
                    if ((unsigned int)v91 <= 2)
                        v73 = AI_MainActor[(uint32_t)v91 - 1];
                    v72 = v73;
                }
                else
                {
                    v72 = 0;
                }
                v71 = v72->extended->AIInstance;
            }
            ((tScriptStruct_SFXXMEN*)(v71->instanceVarDataCurrent))->des_start_actor = v120->o_xmen_father;
            if (v91 == WOR_gst_Universe.univAIInstance)
            {
                v68 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v70 = (tGameObjectMain*)v91;
                if (v91)
                {
                    if ((unsigned int)v91 <= 2)
                        v70 = AI_MainActor[(uint32_t)v91 - 1];
                    v69 = v70;
                }
                else
                {
                    v69 = 0;
                }
                v68 = v69->extended->AIInstance;
            }
            ((tScriptStruct_SFXXMEN*)(v68->instanceVarDataCurrent))->des_start_bone_num = v120->i_xmen_father_bone_num;
            if (v91 == WOR_gst_Universe.univAIInstance)
            {
                v65 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v67 = (tGameObjectMain*)v91;
                if (v91)
                {
                    if ((unsigned int)v91 <= 2)
                        v67 = AI_MainActor[(uint32_t)v91 - 1];
                    v66 = v67;
                }
                else
                {
                    v66 = 0;
                }
                v65 = v66->extended->AIInstance;
            }
            ((tScriptStruct_SFXXMEN*)(v65->instanceVarDataCurrent))->des_start_bone_offset.x = 0.0;
            if (v91 == WOR_gst_Universe.univAIInstance)
            {
                v62 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v64 = (tGameObjectMain*)v91;
                if (v91)
                {
                    if ((unsigned int)v91 <= 2)
                        v64 = AI_MainActor[(uint32_t)v91 - 1];
                    v63 = v64;
                }
                else
                {
                    v63 = 0;
                }
                v62 = v63->extended->AIInstance;
            }
            ((tScriptStruct_SFXXMEN*)(v62->instanceVarDataCurrent))->des_start_bone_offset.y = 0.0;
            if (v91 == WOR_gst_Universe.univAIInstance)
            {
                v59 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v61 = (tGameObjectMain*)v91;
                if (v91)
                {
                    if ((unsigned int)v91 <= 2)
                        v61 = AI_MainActor[(uint32_t)v91 - 1];
                    v60 = v61;
                }
                else
                {
                    v60 = 0;
                }
                v59 = v60->extended->AIInstance;
            }
            ((tScriptStruct_SFXXMEN*)(v59->instanceVarDataCurrent))->des_start_bone_offset.z = 0.0;
            v97 = AI_EvalFunc_MATHRandInt_C(1, 5);
            v124 = AI_EvalFunc_MTHModulo_C(v97 + v120->i_xmen_father_bone_num, 20);
            v120->i_xmen_father_bone_num = v124;
        }
        else
        {
            if (v91 == WOR_gst_Universe.univAIInstance)
            {
                v56 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v58 = (tGameObjectMain*)v91;
                if (v91)
                {
                    if ((unsigned int)v91 <= 2)
                        v58 = AI_MainActor[(uint32_t)v91 - 1];
                    v57 = v58;
                }
                else
                {
                    v57 = 0;
                }
                v56 = v57->extended->AIInstance;
            }
            ((tScriptStruct_SFXXMEN*)(v56->instanceVarDataCurrent))->des_start_actor = 0;
            if (v91 == WOR_gst_Universe.univAIInstance)
            {
                v53 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v55 = (tGameObjectMain*)v91;
                if (v91)
                {
                    if ((unsigned int)v91 <= 2)
                        v55 = AI_MainActor[(uint32_t)v91 - 1];
                    v54 = v55;
                }
                else
                {
                    v54 = 0;
                }
                v53 = v54->extended->AIInstance;
            }
            ((tScriptStruct_SFXXMEN*)(v53->instanceVarDataCurrent))->des_start_bone_num = 0;
            if (v91 == WOR_gst_Universe.univAIInstance)
            {
                v50 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v52 = (tGameObjectMain*)v91;
                if (v91)
                {
                    if ((unsigned int)v91 <= 2)
                        v52 = AI_MainActor[(uint32_t)v91 - 1];
                    v51 = v52;
                }
                else
                {
                    v51 = 0;
                }
                v50 = v51->extended->AIInstance;
            }
            ((tScriptStruct_SFXXMEN*)(v50->instanceVarDataCurrent))->des_start_bone_offset.x = 0.0;
            if (v91 == WOR_gst_Universe.univAIInstance)
            {
                v47 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v49 = (tGameObjectMain*)v91;
                if (v91)
                {
                    if ((unsigned int)v91 <= 2)
                        v49 = AI_MainActor[(uint32_t)v91 - 1];
                    v48 = v49;
                }
                else
                {
                    v48 = 0;
                }
                v47 = v48->extended->AIInstance;
            }
            ((tScriptStruct_SFXXMEN*)(v47->instanceVarDataCurrent))->des_start_bone_offset.y = 0.0;
            if (v91 == WOR_gst_Universe.univAIInstance)
            {
                v44 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v46 = (tGameObjectMain*)v91;
                if (v91)
                {
                    if ((unsigned int)v91 <= 2)
                        v46 = AI_MainActor[(uint32_t)v91 - 1];
                    v45 = v46;
                }
                else
                {
                    v45 = 0;
                }
                v44 = v45->extended->AIInstance;
            }
            ((tScriptStruct_SFXXMEN*)(v44->instanceVarDataCurrent))->des_start_bone_offset.z = 0.0;
        }
        if (v91 == WOR_gst_Universe.univAIInstance)
        {
            v41 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v43 = (tGameObjectMain*)v91;
            if (v91)
            {
                if ((unsigned int)v91 <= 2)
                    v43 = AI_MainActor[(uint32_t)v91 - 1];
                v42 = v43;
            }
            else
            {
                v42 = 0;
            }
            v41 = v42->extended->AIInstance;
        }
        ((tScriptStruct_SFXXMEN*)(v41->instanceVarDataCurrent))->des_dest_actor = (int)AI_gpst_CurrentGameObject;
        if (v91 == WOR_gst_Universe.univAIInstance)
        {
            v38 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v40 = (tGameObjectMain*)v91;
            if (v91)
            {
                if ((unsigned int)v91 <= 2)
                    v40 = AI_MainActor[(uint32_t)v91 - 1];
                v39 = v40;
            }
            else
            {
                v39 = 0;
            }
            v38 = v39->extended->AIInstance;
        }
        ((tScriptStruct_SFXXMEN*)(v38->instanceVarDataCurrent))->des_dest_bone_offset.x = 0.0;
        if (v91 == WOR_gst_Universe.univAIInstance)
        {
            v35 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v37 = (tGameObjectMain*)v91;
            if (v91)
            {
                if ((unsigned int)v91 <= 2)
                    v37 = AI_MainActor[(uint32_t)v91 - 1];
                v36 = v37;
            }
            else
            {
                v36 = 0;
            }
            v35 = v36->extended->AIInstance;
        }
        ((tScriptStruct_SFXXMEN*)(v35->instanceVarDataCurrent))->des_dest_bone_offset.y = 0.0;
        if (v91 == WOR_gst_Universe.univAIInstance)
        {
            v32 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v34 = (tGameObjectMain*)v91;
            if (v91)
            {
                if ((unsigned int)v91 <= 2)
                    v34 = AI_MainActor[(uint32_t)v91 - 1];
                v33 = v34;
            }
            else
            {
                v33 = 0;
            }
            v32 = v33->extended->AIInstance;
        }
        ((tScriptStruct_SFXXMEN*)(v32->instanceVarDataCurrent))->des_dest_bone_offset.z = 1.0;
        if (v91 == WOR_gst_Universe.univAIInstance)
        {
            v29 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v31 = (tGameObjectMain*)v91;
            if (v91)
            {
                if ((unsigned int)v91 <= 2)
                    v31 = AI_MainActor[(uint32_t)v91 - 1];
                v30 = v31;
            }
            else
            {
                v30 = 0;
            }
            v29 = v30->extended->AIInstance;
        }
        ((tScriptStruct_SFXXMEN*)(v29->instanceVarDataCurrent))->des_dest_bone_dir.x = 0.0;
        if (v91 == WOR_gst_Universe.univAIInstance)
        {
            v26 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v28 = (tGameObjectMain*)v91;
            if (v91)
            {
                if ((unsigned int)v91 <= 2)
                    v28 = AI_MainActor[(uint32_t)v91 - 1];
                v27 = v28;
            }
            else
            {
                v27 = 0;
            }
            v26 = v27->extended->AIInstance;
        }
        ((tScriptStruct_SFXXMEN*)(v26->instanceVarDataCurrent))->des_dest_bone_dir.y = 0.0;
        if (v91 == WOR_gst_Universe.univAIInstance)
        {
            v23 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v25 = (tGameObjectMain*)v91;
            if (v91)
            {
                if ((unsigned int)v91 <= 2)
                    v25 = AI_MainActor[(uint32_t)v91 - 1];
                v24 = v25;
            }
            else
            {
                v24 = 0;
            }
            v23 = v24->extended->AIInstance;
        }
        ((tScriptStruct_SFXXMEN*)(v23->instanceVarDataCurrent))->des_dest_bone_dir.z = 1.0;
        if (v91 == WOR_gst_Universe.univAIInstance)
        {
            v20 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v22 = (tGameObjectMain*)v91;
            if (v91)
            {
                if ((unsigned int)v91 <= 2)
                    v22 = AI_MainActor[(uint32_t)v91 - 1];
                v21 = v22;
            }
            else
            {
                v21 = 0;
            }
            v20 = v21->extended->AIInstance;
        }
        ((tScriptStruct_SFXXMEN*)(v20->instanceVarDataCurrent))->des_XMEN_width = 0.40000001f;
        if (v91 == WOR_gst_Universe.univAIInstance)
        {
            v17 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v19 = (tGameObjectMain*)v91;
            if (v91)
            {
                if ((unsigned int)v91 <= 2)
                    v19 = AI_MainActor[(uint32_t)v91 - 1];
                v18 = v19;
            }
            else
            {
                v18 = 0;
            }
            v17 = v18->extended->AIInstance;
        }
        ((tScriptStruct_SFXXMEN*)(v17->instanceVarDataCurrent))->des_f_move_delay = AI_EvalFunc_MATHRandFloat_C(0.5, 1.0);
        if (v120->f_halo_first_delay == 0.0)
        {
            if (v91 == WOR_gst_Universe.univAIInstance)
            {
                v14 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v16 = (tGameObjectMain*)v91;
                if (v91)
                {
                    if ((unsigned int)v91 <= 2)
                        v16 = AI_MainActor[(uint32_t)v91 - 1];
                    v15 = v16;
                }
                else
                {
                    v15 = 0;
                }
                v14 = v15->extended->AIInstance;
            }
            v120->f_halo_first_delay = ((tScriptStruct_SFXXMEN*)(v14->instanceVarDataCurrent))->des_f_move_delay;
        }
        if (v91 == WOR_gst_Universe.univAIInstance)
        {
            v11 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v13 = (tGameObjectMain*)v91;
            if (v91)
            {
                if ((unsigned int)v91 <= 2)
                    v13 = AI_MainActor[(uint32_t)v91 - 1];
                v12 = v13;
            }
            else
            {
                v12 = 0;
            }
            v11 = v12->extended->AIInstance;
        }
        ((tScriptStruct_SFXXMEN*)(v11->instanceVarDataCurrent))->des_sfx_halo = 0;
        v10 = (tGameObjectMain*)v91;
        if (v91)
        {
            if ((unsigned int)v91 <= 2)
                v10 = AI_MainActor[(uint32_t)v91 - 1];
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(v10, 1);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
        }
        --v120->i_xmen_nbr;
    }
    v3 = v120->f_halo_first_delay - 0.2f;
    if (AI_EvalFunc_TIMEElapsed_C(v120->f_halo_time, v3))
    {
        if (v120->GFX_Halo == -1)
        {
            v120->GFX_Halo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v120->GFX_Halo, 4, 1);
            v9 = (tGameObjectMain*)PNJ_Spectre_sp_mode_apparition_xmen_SFX_light_smoke_gao;
            if (PNJ_Spectre_sp_mode_apparition_xmen_SFX_light_smoke_gao)
            {
                if ((unsigned int)PNJ_Spectre_sp_mode_apparition_xmen_SFX_light_smoke_gao <= 2)
                    v9 = AI_MainActor[PNJ_Spectre_sp_mode_apparition_xmen_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v120->GFX_Halo, v9, 22);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v120->GFX_Halo, 0, 22);
            }
            GFXSetCurGlobal(v120->GFX_Halo);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21100, 8);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 2.0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21102, 0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21101, -1);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v115);
            v92.x = 0.0;
            v92.y = 0.0;
            v92.z = 1.2f;
            MATH_VecAddVec(&v102, &v115, &v92);
            AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v102);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21005, 0.30000001f);
            v120->f_halo_size = 0.0;
        }
        if (v120->f_time_tresh_start == 0.0)
            v120->f_time_tresh_start = TIM_gf_MainClock - TIM_gf_LockTime;
        v118 = TIM_gf_MainClock - TIM_gf_LockTime;
        v88 = (v118 - v120->f_time_tresh_start) / 1.0f;
        if (v88 > 1.0)
            v4 = 1.0;
        else
            v4 = (v118 - v120->f_time_tresh_start) / 1.0f;
        v120->f_local_tresh_alpha = v4 * 255.0f;
        v120->f_vitre_local_alpha = v120->f_local_tresh_alpha;
        if (v120->GFX_Halo != -1)
        {
            if (v120->f_halo_size_up)
            {
                v119 = TIM_gf_dt;
                v8 = 4.5f * TIM_gf_dt;
                if (v8 < 0.0)
                    v8 = 0.0;
                if (v8 > 1.0)
                    v8 = 1.0;
                v123 = 3.0f * v8 + v120->f_halo_size * (1.0f - v8);
                v120->f_halo_size = v123;
                if (v120->f_halo_size > 2.8499999)
                {
                    v120->f_halo_size_up = 0;
                    v120->i_auto_color_membrane = 1;
                    AI_EvalFunc_GFXLifeTimeSet_C(AI_gpst_CurrentGameObject, v120->GFX_Halo, 0.0);
                }
                if (v120->GFX_Halo)
                    AI_EvalFunc_GFXSetf_C_CURRENT(v120->GFX_Halo, 21000, v120->f_halo_size);
            }
            else
            {
                v98 = TIM_gf_dt;
                v7 = 10.0f * TIM_gf_dt;
                if (v7 < 0.0)
                    v7 = 0.0;
                if (v7 > 1.0)
                    v7 = 1.0;
                v105 = 0.0f * v7 + v120->f_halo_size * (1.0f - v7);
                v120->f_halo_size = v105;
                if (v120->f_halo_size < 0.30000001)
                {
                    v120->f_local_tresh_alpha = 255.0;
                    v108 = 1;
                    AI_EvalFunc_GFXDel_C_CURRENT(v120->GFX_Halo);
                    v120->GFX_Halo = -1;
                }
            }
        }
    }
    if (v108)
    {
        AI_EvalFunc_AITrackChange_C(0, (tAIFunction*)fct_PNJ_Spectre_sp_mode_apparition_xmen_PNJ_Spectre_sp_tag_off);
        AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_PNJ_Spectre_sp_mode_apparition_xmen_PNJ_Spectre_sp_reflex);
        AI_EvalFunc_AITrackChange_C(4, (tAIFunction*)fct_PNJ_Spectre_sp_mode_apparition_xmen_PNJ_Spectre_sp_tag_on);
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_PNJ_Spectre_sp_mode_apparition_xmen_PNJ_Spectre_sp_mode_combat);
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, -1, 0);
        v94 = AI_gpst_CurrentGameObject;
        v6 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v6 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v6,
                3,
                (tAIFunction*)fct_PNJ_Spectre_sp_mode_apparition_xmen_PNJ_Spectre_sp_after_blend);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                3,
                (tAIFunction*)fct_PNJ_Spectre_sp_mode_apparition_xmen_PNJ_Spectre_sp_after_blend);
        }
    }
    return 1000;
}

// =============================

int PNJ_Spectre_bras_PNJ_Spectre_Bras_init()
{
    tGameObjectMain* v0; // eax
    tGameObjectMain* v1; // eax
    tGameObjectMain* v2; // eax
    tGameObjectMain* v3; // eax
    tGameObjectMain* v4; // eax
    tGameObjectMain* v5; // eax
    int result; // eax
    tScriptStruct_PNJSectreBras* v7; // [esp+0h] [ebp-80h]
    tGameObjectMain* v8; // [esp+8h] [ebp-78h]
    tGameObjectMain* v9; // [esp+10h] [ebp-70h]
    tGameObjectMain* v10; // [esp+18h] [ebp-68h]
    tGameObjectMain* v11; // [esp+20h] [ebp-60h]
    tGameObjectMain* v12; // [esp+28h] [ebp-58h]
    tGameObjectMain* v13; // [esp+30h] [ebp-50h]
    tGameObjectMain* v14; // [esp+38h] [ebp-48h]
    tGameObjectMain* v15; // [esp+40h] [ebp-40h]
    tGameObjectMain* v16; // [esp+48h] [ebp-38h]
    int v17; // [esp+5Ch] [ebp-24h]
    int i; // [esp+74h] [ebp-Ch]
    int j; // [esp+74h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_PNJSectreBras*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init = (int)AI_ul_RealResolveOneRefFunction(0x72005509u);
        fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_before_display = (int)AI_ul_RealResolveOneRefFunction(0x72005572u);
        fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_after_display = (int)AI_ul_RealResolveOneRefFunction(0x72005571u);
        fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_reflex = (int)AI_ul_RealResolveOneRefFunction(0x720056C8u);
    }
    AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
    for (i = 0; i < 5; ++i)
        v7->GFX_Halo[i] = -1;
    for (j = 0; j < v7->des_i_numbones; ++j)
        v7->ao_obj[j] = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(j);
    v7->f_time_start = 0.0;
    if (v7->des_bras == -1.0)
    {
        v16 = (tGameObjectMain*)v7->des_papa;
        if (v16)
        {
            if ((unsigned int)v16 <= 2)
                v16 = AI_MainActor[(uint32_t)v16 - 1];
            v0 = AI_EvalFunc_ANICanalGaoGet_C(v16, 13u);
        }
        else
        {
            v0 = AI_EvalFunc_ANICanalGaoGet_C(0, 13u);
        }
        v7->o_bras = (int)v0;
        v15 = (tGameObjectMain*)v7->des_papa;
        if (v15)
        {
            if ((unsigned int)v15 <= 2)
                v15 = AI_MainActor[(uint32_t)v15 - 1];
            v1 = AI_EvalFunc_ANICanalGaoGet_C(v15, 15u);
        }
        else
        {
            v1 = AI_EvalFunc_ANICanalGaoGet_C(0, 15u);
        }
        v7->o_avant_bras = (int)v1;
        v14 = (tGameObjectMain*)v7->des_papa;
        if (v14)
        {
            if ((unsigned int)v14 <= 2)
                v14 = AI_MainActor[(uint32_t)v14 - 1];
            v2 = AI_EvalFunc_ANICanalGaoGet_C(v14, 50u);
        }
        else
        {
            v2 = AI_EvalFunc_ANICanalGaoGet_C(0, 50u);
        }
        v7->o_main = (int)v2;
    }
    else
    {
        v13 = (tGameObjectMain*)v7->des_papa;
        if (v13)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            v3 = AI_EvalFunc_ANICanalGaoGet_C(v13, 14u);
        }
        else
        {
            v3 = AI_EvalFunc_ANICanalGaoGet_C(0, 14u);
        }
        v7->o_bras = (int)v3;
        v12 = (tGameObjectMain*)v7->des_papa;
        if (v12)
        {
            if ((unsigned int)v12 <= 2)
                v12 = AI_MainActor[(uint32_t)v12 - 1];
            v4 = AI_EvalFunc_ANICanalGaoGet_C(v12, 16u);
        }
        else
        {
            v4 = AI_EvalFunc_ANICanalGaoGet_C(0, 16u);
        }
        v7->o_avant_bras = (int)v4;
        v11 = (tGameObjectMain*)v7->des_papa;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v11 = AI_MainActor[(uint32_t)v11 - 1];
            v5 = AI_EvalFunc_ANICanalGaoGet_C(v11, 51u);
        }
        else
        {
            v5 = AI_EvalFunc_ANICanalGaoGet_C(0, 51u);
        }
        v7->o_main = (int)v5;
    }
    v7->o_gmat_gao = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0);
    v10 = (tGameObjectMain*)v7->des_papa;
    if (v10)
    {
        if ((unsigned int)v10 <= 2)
            v10 = AI_MainActor[(uint32_t)v10 - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v10,
            3,
            (tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            3,
            (tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init);
    }
    v9 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v9 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v9,
            0,
            (tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_before_display);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            0,
            (tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_before_display);
    }
    v8 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v8 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v8,
            1,
            (tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_after_display);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            1,
            (tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_after_display);
    }
    v17 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_reflex);
    if (v17 == 1000)
        result = 1000;
    else
        result = v17;
    return result;
}

int PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_do_snake()
{
    float a4; // [esp+0h] [ebp-11Ch]
    float a4a; // [esp+0h] [ebp-11Ch]
    tScriptStruct_PNJSectreBras* v3; // [esp+8h] [ebp-114h]
    tGameObjectMain* v4; // [esp+10h] [ebp-10Ch]
    tGameObjectMain* v5; // [esp+18h] [ebp-104h]
    tGameObjectMain* v6; // [esp+20h] [ebp-FCh]
    float v7; // [esp+24h] [ebp-F8h]
    tGameObjectMain* v8; // [esp+3Ch] [ebp-E0h]
    tGameObjectMain* v9; // [esp+58h] [ebp-C4h]
    tGameObjectMain* v10; // [esp+60h] [ebp-BCh]
    tGameObjectMain* v11; // [esp+70h] [ebp-ACh]
    tGameObjectMain* v12; // [esp+78h] [ebp-A4h]
    Vec3 v13; // [esp+7Ch] [ebp-A0h] BYREF
    Vec3 v14; // [esp+88h] [ebp-94h] BYREF
    Vec3 v15; // [esp+94h] [ebp-88h] BYREF
    Vec3 v16; // [esp+A0h] [ebp-7Ch] BYREF
    Vec3 v17; // [esp+ACh] [ebp-70h] BYREF
    Vec3 v18; // [esp+B8h] [ebp-64h] BYREF
    Vec3 a2; // [esp+C4h] [ebp-58h] BYREF
    Vec3 v20; // [esp+D0h] [ebp-4Ch] BYREF
    float v21; // [esp+DCh] [ebp-40h]
    float v22; // [esp+E0h] [ebp-3Ch]
    Vec3 a3; // [esp+E4h] [ebp-38h] BYREF
    Vec3 v24; // [esp+F0h] [ebp-2Ch] BYREF
    tScriptStruct_PNJSectreBras* v25; // [esp+FCh] [ebp-20h]
    int i; // [esp+100h] [ebp-1Ch]
    Vec3 a5; // [esp+104h] [ebp-18h] BYREF
    Vec3 dest; // [esp+110h] [ebp-Ch] BYREF

    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    v16.x = 1.0;
    v16.y = 0.0;
    v16.z = 0.0;
    a2.x = 1.0;
    a2.y = 0.0;
    a2.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_PNJSectreBras*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v25 = v3;
    for (i = 1; i < v25->des_i_numbones; ++i)
    {
        v12 = (tGameObjectMain*)v25->ao_obj[i];
        if (v12)
        {
            if ((unsigned int)v12 <= 2)
                v12 = AI_MainActor[(uint32_t)v12 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v12, &v17);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &v17);
        }
        v11 = (tGameObjectMain*)*(&v25->__ao_obj_Dimension1 + i);
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v11 = AI_MainActor[(uint32_t)v11 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v11, &v24);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &v24);
        }
        v21 = TIM_gf_dt;
        a4 = TIM_gf_dt * 20.0f * 1.5f;
        AI_EvalFunc_MATHVecBlend_C(&v17, &v24, a4, &v14);
        v16 = v14;
        v10 = (tGameObjectMain*)v25->ao_obj[i];
        if (v10)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            AI_EvalFunc_OBJGetPos_C(v10, &v13);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v13);
        }
        v9 = (tGameObjectMain*)*(&v25->__ao_obj_Dimension1 + i);
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            AI_EvalFunc_OBJGetPos_C(v9, &v20);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v20);
        }
        MATH_SubstractVec(&dest, &v13, &v20);
        a2 = dest;
        MATH_NormalizeEqualVector(&a2);
        v8 = (tGameObjectMain*)*(&v25->__ao_obj_Dimension1 + i);
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v8 = AI_MainActor[(uint32_t)v8 - 1];
            AI_EvalFunc_OBJ_BankingGet_C(v8, &v15);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGet_C(0, &v15);
        }
        a3 = v15;
        v22 = TIM_gf_dt;
        a4a = TIM_gf_dt * 20.0f * 0.30000001f;
        AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4a, &a5);
        a2 = a5;
        v7 = *(float*)AI_EvalVarArray_C2((char*)&v25->__af_lenos_Dimension1, i);
        a2.x = a2.x * v7;
        a2.y = a2.y * v7;
        a2.z = a2.z * v7;
        v6 = (tGameObjectMain*)v25->ao_obj[i];
        if (v6)
        {
            if ((unsigned int)v6 <= 2)
                v6 = AI_MainActor[(uint32_t)v6 - 1];
            AI_EvalFunc_OBJ_BankingGeneralSet_C(v6, &v16, &a2);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v16, &a2);
        }
        v5 = (tGameObjectMain*)*(&v25->__ao_obj_Dimension1 + i);
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            AI_EvalFunc_OBJGetPos_C(v5, &v18);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v18);
        }
        a2.x = a2.x + v18.x;
        a2.y = a2.y + v18.y;
        a2.z = a2.z + v18.z;
        v4 = (tGameObjectMain*)v25->ao_obj[i];
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            AI_EvalFunc_OBJSetPos_C(v4, &a2);
        }
        else
        {
            AI_EvalFunc_OBJSetPos_C(0, &a2);
        }
    }
    return 1000;
}

int PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move()
{
    int result; // eax
    unsigned int v1; // eax
    BOOL v2; // eax
    float a4; // [esp+0h] [ebp-274h]
    float a4a; // [esp+0h] [ebp-274h]
    float a4b; // [esp+0h] [ebp-274h]
    float v6; // [esp+4h] [ebp-270h]
    float v7; // [esp+8h] [ebp-26Ch]
    float v8; // [esp+Ch] [ebp-268h]
    float v9; // [esp+10h] [ebp-264h]
    tScriptStruct_PNJSectreBras* v10; // [esp+14h] [ebp-260h]
    tGameObjectMain* v11; // [esp+2Ch] [ebp-248h]
    float v12; // [esp+34h] [ebp-240h]
    tGameObjectMain* v13; // [esp+54h] [ebp-220h]
    tGameObjectMain* v14; // [esp+6Ch] [ebp-208h]
    tGameObjectMain* v15; // [esp+7Ch] [ebp-1F8h]
    tGameObjectMain* v16; // [esp+8Ch] [ebp-1E8h]
    tGameObjectMain* v17; // [esp+9Ch] [ebp-1D8h]
    tGameObjectMain* v18; // [esp+A4h] [ebp-1D0h]
    tGameObjectMain* v19; // [esp+B4h] [ebp-1C0h]
    tGameObjectMain* v20; // [esp+BCh] [ebp-1B8h]
    tGameObjectMain* v21; // [esp+C8h] [ebp-1ACh]
    tGameObjectMain* v22; // [esp+D0h] [ebp-1A4h]
    float v23; // [esp+E4h] [ebp-190h]
    float v24; // [esp+E4h] [ebp-190h]
    Vec3 v25; // [esp+ECh] [ebp-188h] BYREF
    Vec3 v26; // [esp+F8h] [ebp-17Ch] BYREF
    Vec3 a2; // [esp+104h] [ebp-170h] BYREF
    int v28; // [esp+114h] [ebp-160h]
    float v29; // [esp+118h] [ebp-15Ch]
    float v30; // [esp+11Ch] [ebp-158h]
    float v31; // [esp+120h] [ebp-154h]
    Vec3 v32; // [esp+12Ch] [ebp-148h] BYREF
    Vec3 v33; // [esp+138h] [ebp-13Ch] BYREF
    int v34; // [esp+144h] [ebp-130h]
    Vec3 v35; // [esp+148h] [ebp-12Ch] BYREF
    int v36; // [esp+154h] [ebp-120h]
    int v37; // [esp+15Ch] [ebp-118h]
    Vec3 v38; // [esp+160h] [ebp-114h] BYREF
    float v39; // [esp+16Ch] [ebp-108h]
    BOOL v40; // [esp+170h] [ebp-104h]
    Vec3 v41; // [esp+174h] [ebp-100h] BYREF
    Vec3 v42; // [esp+180h] [ebp-F4h] BYREF
    Vec3 v43; // [esp+18Ch] [ebp-E8h] BYREF
    Vec3 a1; // [esp+198h] [ebp-DCh] BYREF
    Vec3 v45; // [esp+1A4h] [ebp-D0h] BYREF
    Vec3 v46; // [esp+1B0h] [ebp-C4h] BYREF
    Vec3 v47; // [esp+1BCh] [ebp-B8h] BYREF
    Vec3 a3; // [esp+1C8h] [ebp-ACh] BYREF
    float v49; // [esp+1D4h] [ebp-A0h]
    float v50; // [esp+1D8h] [ebp-9Ch]
    Vec3 v51; // [esp+1DCh] [ebp-98h] BYREF
    Vec3 v52; // [esp+1E8h] [ebp-8Ch] BYREF
    Vec3 v53; // [esp+1F4h] [ebp-80h]
    float v54; // [esp+200h] [ebp-74h]
    float v55; // [esp+204h] [ebp-70h]
    Vec3 v56; // [esp+208h] [ebp-6Ch] BYREF
    int v57; // [esp+214h] [ebp-60h]
    Vec3 v58; // [esp+218h] [ebp-5Ch] BYREF
    tScriptStruct_PNJSectreBras* v59; // [esp+224h] [ebp-50h]
    int i; // [esp+228h] [ebp-4Ch]
    Vec3 a5; // [esp+22Ch] [ebp-48h] BYREF
    Vec3 v62; // [esp+238h] [ebp-3Ch] BYREF
    Vec3 v63; // [esp+244h] [ebp-30h] BYREF
    int v64; // [esp+254h] [ebp-20h]
    Vec3 v65; // [esp+258h] [ebp-1Ch]
    Vec3 v66; // [esp+264h] [ebp-10h] BYREF
    float v67; // [esp+270h] [ebp-4h]

    v51.x = 1.0;
    v51.y = 0.0;
    v51.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    v32.x = 1.0;
    v32.y = 0.0;
    v32.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v10 = (tScriptStruct_PNJSectreBras*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v10 = 0;
    v59 = v10;
    v50 = 0.0;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_init_snake = (int)AI_ul_RealResolveOneRefFunction(0x72005528u);
        fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_GFX_Part = (int)AI_ul_RealResolveOneRefFunction(0x72005525u);
        PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move = (int)AI_ul_RealResolveOneRefFunction(0x72005530u);
        fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_PNJ_Spectre_bras_PNJ_Spectre_Bras_Destroy = (int)AI_ul_RealResolveOneRefFunction(0x72005679u);
        fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_do_snake = (int)AI_ul_RealResolveOneRefFunction(0x72005529u);
    }
    if (v59->f_time_start == 0.0)
    {
        v59->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
        v57 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_init_snake);
        if (v57 == 1000)
        {
            v59->i_flag_dyn_on = 1;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            result = v57;
        }
    }
    else
    {
        v49 = TIM_gf_MainClock - TIM_gf_LockTime;
        v23 = (v49 - v59->f_time_start) / 0.75f;
        if (v23 > 1.0)
            v9 = 1.0;
        else
            v9 = (v49 - v59->f_time_start) / 0.75f;
        v22 = (tGameObjectMain*)v59->des_papa;
        if (v22)
        {
            if ((unsigned int)v22 <= 2)
                v22 = AI_MainActor[(uint32_t)v22 - 1];
            v1 = AI_EvalFunc_OBJ_FlagsControlGet_C(v22);
        }
        else
        {
            v1 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
        }
        if ((v1 & 0x200000) != 0)
            goto LABEL_25;
        v21 = (tGameObjectMain*)v59->des_papa;
        if (v21)
        {
            if ((unsigned int)v21 <= 2)
                v21 = AI_MainActor[(uint32_t)v21 - 1];
            v2 = AI_EvalFunc_OBJ_CapaTest_C(v21, 0x8000);
        }
        else
        {
            v2 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x8000);
        }
        v40 = v2;
        if (v2)
        {
        LABEL_25:
            if (v59->GFX_Halo[0] != -1)
            {
                for (i = 0; i < 5; ++i)
                {
                    AI_EvalFunc_GFXDel_C_CURRENT(v59->GFX_Halo[i]);
                    v59->GFX_Halo[i] = -1;
                }
            }
            if (v59->GFX_Part != -1)
            {
                AI_EvalFunc_GFXSeti_C_CURRENT(v59->GFX_Part, 13106, 0);
                v59->GFX_Part = -1;
            }
        }
        else
        {
            v64 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_GFX_Part);
            if (v64 != 1000)
                return v64;
            if (v9 >= 1.0)
            {
                if (v59->GFX_Halo[0] != -1)
                {
                    for (i = 0; i < 5; ++i)
                    {
                        AI_EvalFunc_GFXDel_C_CURRENT(v59->GFX_Halo[i]);
                        v59->GFX_Halo[i] = -1;
                    }
                }
            }
            else
            {
                v20 = (tGameObjectMain*)v59->o_bras;
                if (v20)
                {
                    if ((unsigned int)v20 <= 2)
                        v20 = AI_MainActor[(uint32_t)v20 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v20, &a2);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &a2);
                }
                v19 = (tGameObjectMain*)v59->o_bras;
                if (v19)
                {
                    if ((unsigned int)v19 <= 2)
                        v19 = AI_MainActor[(uint32_t)v19 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v19, &a3);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &a3);
                }
                a4 = v9 * 0.5f + 0.5f;
                AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
                a1 = a5;
                for (i = 0; i < 5; ++i)
                {
                    if (v59->GFX_Halo[i] == -1)
                    {
                        v59->GFX_Halo[i] = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                        AI_EvalFunc_GFXFlagSet_C_CURRENT(v59->GFX_Halo[i], 4, 1);
                        v18 = (tGameObjectMain*)PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_SFX_light_smoke_gao;
                        if (PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_SFX_light_smoke_gao)
                        {
                            if ((unsigned int)PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_SFX_light_smoke_gao <= 2)
                                v18 = AI_MainActor[PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_SFX_light_smoke_gao - 1];
                            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v59->GFX_Halo[i], v18, 0);
                        }
                        else
                        {
                            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v59->GFX_Halo[i], 0, 0);
                        }
                        GFXSetCurGlobal(v59->GFX_Halo[i]);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 0.15000001f);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0f);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 5.0f);
                        AI_EvalFunc_GFXSeti_C_CURGLOB(21104, 3);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(21006, 0.02f);
                    }
                    v17 = (tGameObjectMain*)v59->o_bras;
                    if (v17)
                    {
                        if ((unsigned int)v17 <= 2)
                            v17 = AI_MainActor[(uint32_t)v17 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v17, &v46);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v46);
                    }
                    v8 = (float)i;
                    a4a = v8 * 1.256637f;
                    AI_EvalFunc_MATHVecRotate_C(&a1, &v46, a4a, &v25);
                    v51 = v25;
                    GFXSetCurGlobal(v59->GFX_Halo[i]);
                    AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v51);
                    v16 = (tGameObjectMain*)v59->o_bras;
                    if (v16)
                    {
                        if ((unsigned int)v16 <= 2)
                            v16 = AI_MainActor[(uint32_t)v16 - 1];
                        AI_EvalFunc_OBJGetPos_C(v16, &v66);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v66);
                    }
                    v32 = v66;
                    v15 = (tGameObjectMain*)v59->o_bras;
                    if (v15)
                    {
                        if ((unsigned int)v15 <= 2)
                            v15 = AI_MainActor[(uint32_t)v15 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v15, &v42);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v42);
                    }
                    v29 = v42.x * 0.5f;
                    v30 = v42.y * 0.5f;
                    v31 = v42.z * 0.5f;
                    v32.x = v32.x + v29;
                    v32.y = v32.y + v30;
                    v32.z = v32.z + v31;
                    AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v32);
                    v36 = AI_EvalFunc_COLORBlend_C(12648384, 0, v9);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v36);
                    v50 = 10.0f * v9;
                    v50 = AI_EvalFunc_MATHRandFloat_C(0.0, 20.0) + v50;
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v50);
                }
            }
        }
        if (v9 == 1.0
            && (v59->f_local_tresh_alpha == 0.0 || (v37 = (unsigned __int16)AI_gpst_CurrentGameObject->flags2, (v37 & 4) != 0)))
        {
            if (v59->GFX_Halo[0] != -1)
            {
                for (i = 0; i < 5; ++i)
                {
                    AI_EvalFunc_GFXDel_C_CURRENT(v59->GFX_Halo[i]);
                    v59->GFX_Halo[i] = -1;
                }
            }
            if (v59->GFX_Halo_Bras != -1)
            {
                AI_EvalFunc_GFXDel_C_CURRENT(v59->GFX_Halo_Bras);
                v59->GFX_Halo_Bras = -1;
            }
            if (v59->GFX_Part != -1)
            {
                AI_EvalFunc_GFXSeti_C_CURRENT(v59->GFX_Part, 13106, 0);
                v59->GFX_Part = -1;
            }
            v14 = (tGameObjectMain*)v59->des_papa;
            if (v14)
            {
                if ((unsigned int)v14 <= 2)
                    v14 = AI_MainActor[(uint32_t)v14 - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v14,
                    3,
                    (tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    3,
                    (tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move);
            }
            AI_EvalFunc_AITrackChange_C(
                0,
                (tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_PNJ_Spectre_bras_PNJ_Spectre_Bras_Destroy);
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 4;
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v52);
            a1 = v52;
            AI_EvalFunc_MATHVecSetHorzNormalize_C(&a1);
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v45);
            v67 = TIM_gf_dt;
            a4b = 4.0f * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlendRotate_C(&v45, &a1, a4b, &v47);
            a1 = v47;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v33);
            AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v33, &a1);
            v34 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_do_snake);
            if (v34 == 1000)
            {
                if (v59->i_nb_rebond >= 3)
                {
                    v54 = TIM_gf_dt;
                    v12 = 255.0f * TIM_gf_dt;
                    if (v59->f_local_tresh_alpha > (double)v12)
                        v7 = 255.0f * TIM_gf_dt;
                    else
                        v7 = v59->f_local_tresh_alpha;
                    v59->f_local_tresh_alpha = v59->f_local_tresh_alpha - v7;
                }
                else
                {
                    v55 = TIM_gf_dt;
                    v6 = 0.5f * TIM_gf_dt;
                    AI_EvalFunc_OBJRotateLocalZ_c(AI_gpst_CurrentGameObject, v6);
                    if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 64))
                    {
                        if (v59->i_nb_rebond == 2)
                        {
                            v59->i_flag_dyn_on = 0;
                            AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
                        }
                        else
                        {
                            v13 = (tGameObjectMain*)*(&v59->__ao_obj_Dimension1 + v59->des_i_numbones);
                            if (v13)
                            {
                                if ((unsigned int)v13 <= 2)
                                    v13 = AI_MainActor[(uint32_t)v13 - 1];
                                AI_EvalFunc_OBJ_BankingGet_C(v13, &v63);
                            }
                            else
                            {
                                AI_EvalFunc_OBJ_BankingGet_C(0, &v63);
                            }
                            a1 = v63;
                            AI_EvalFunc_MATHVecSetHorzNormalize_C(&a1);
                            v38.x = 0.0;
                            v38.y = 0.0;
                            v38.z = 1.0;
                            AI_EvalFunc_MATHVecCrossProduct_C(&a1, &v38, &v58);
                            v39 = AI_EvalFunc_MATHRandFloat_C(0.25, 0.75);
                            AI_EvalFunc_MATHVecBlendRotate_C(&a1, &v58, v39, &v43);
                            a1 = v43;
                            AI_EvalFunc_MATHRandFloat_C(0.050000001f, 0.1f);
                            AI_EvalFunc_OBJGetPos_C_CURRENT(&v26);
                            v65.x = a1.x * 0.0099999998f;
                            v65.y = a1.y * 0.0099999998f;
                            v65.z = a1.z * 0.0099999998f;
                            v32.x = v26.x + v65.x;
                            v32.y = v26.y + v65.y;
                            v32.z = v26.z + v65.z;
                            AI_EvalFunc_DYNImpulsion_C(AI_gpst_CurrentGameObject, &v32, 0.0099999998f);
                        }
                        ++v59->i_nb_rebond;
                    }
                }
                v24 = v59->f_local_tresh_alpha / 255.0f;
                if (v24 == 0.0)
                {
                    if (v59->GFX_Halo_Bras != -1)
                    {
                        AI_EvalFunc_GFXDel_C_CURRENT(v59->GFX_Halo_Bras);
                        v59->GFX_Halo_Bras = -1;
                    }
                }
                else
                {
                    if (v59->GFX_Halo_Bras == -1)
                    {
                        v59->GFX_Halo_Bras = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                        AI_EvalFunc_GFXFlagSet_C_CURRENT(v59->GFX_Halo_Bras, 4, 1);
                        v11 = (tGameObjectMain*)PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_SFX_light_smoke_gao;
                        if (PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_SFX_light_smoke_gao)
                        {
                            if ((unsigned int)PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_SFX_light_smoke_gao <= 2)
                                v11 = AI_MainActor[PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move_SFX_light_smoke_gao - 1];
                            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v59->GFX_Halo_Bras, v11, 0);
                        }
                        else
                        {
                            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v59->GFX_Halo_Bras, 0, 0);
                        }
                        GFXSetCurGlobal(v59->GFX_Halo_Bras);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 0.2f);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.1f);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 5.0f);
                        AI_EvalFunc_GFXSeti_C_CURGLOB(21104, 3);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(21006, 0.02f);
                        AI_EvalFunc_GFXSeti_C_CURGLOB(21100, 4);
                    }
                    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v56);
                    v62.x = -v56.x;
                    v62.y = -v56.y;
                    v62.z = -v56.z;
                    GFXSetCurGlobal(v59->GFX_Halo_Bras);
                    AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v62);
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v41);
                    v32 = v41;
                    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v35);
                    v53.x = v35.x * 0.44999999f;
                    v53.y = v35.y * 0.44999999f;
                    v53.z = v35.z * 0.44999999f;
                    v32.x = v32.x + v53.x;
                    v32.y = v32.y + v53.y;
                    v32.z = v32.z + v53.z;
                    AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v32);
                    v28 = AI_EvalFunc_COLORBlend_C(0, 0xC0FFC0, v24);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v28);
                    v50 = 1.0;
                    v50 = AI_EvalFunc_MATHRandFloat_C(0.0, 3.0f) + 1.0f;
                    v50 = v50 * v24;
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v50);
                }
                result = 1000;
            }
            else
            {
                result = v34;
            }
        }
    }
    return result;
}

int PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_after_display()
{
    tScriptStruct_PNJSectreBras* v1; // [esp+0h] [ebp-10h]
    tGameObjectMain* v2; // [esp+8h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJSectreBras*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v2 = (tGameObjectMain*)v1->o_gmat_gao;
    if (v2)
    {
        if ((unsigned int)v2 <= 2)
            v2 = AI_MainActor[(uint32_t)v2 - 1];
        AI_EvalFunc_SetALPHATRESH_C(v2, 0, 0, v1->i_global_tresh_alpha);
    }
    else
    {
        AI_EvalFunc_SetALPHATRESH_C(0, 0, 0, v1->i_global_tresh_alpha);
    }
    return 1000;
}

int PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_before_display()
{
    tScriptStruct_PNJSectreBras* v1; // [esp+0h] [ebp-14h]
    tGameObjectMain* a1; // [esp+4h] [ebp-10h]
    tGameObjectMain* v3; // [esp+8h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJSectreBras*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v1->i_global_tresh_alpha = AI_EvalFunc_GetALPHATRESH_C(AI_gpst_CurrentGameObject, 0, 0);
    v3 = (tGameObjectMain*)v1->o_gmat_gao;
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        a1 = v3;
    }
    else
    {
        a1 = 0;
    }
    AI_EvalFunc_SetALPHATRESH_C(a1, 0, 0, (unsigned int)v1->f_local_tresh_alpha);
    return 1000;
}

int PNJ_Spectre_bras_PNJ_Spectre_Bras_reflex()
{
    tScriptStruct_PNJSectreBras* v1; // [esp+0h] [ebp-8Ch]
    tGameObjectMain* v2; // [esp+8h] [ebp-84h]
    float v3; // [esp+Ch] [ebp-80h]
    tGameObjectMain* v4; // [esp+14h] [ebp-78h]
    Vec3 v5; // [esp+20h] [ebp-6Ch] BYREF
    Vec3 v6; // [esp+2Ch] [ebp-60h] BYREF
    float v7; // [esp+38h] [ebp-54h]
    BOOL v8; // [esp+3Ch] [ebp-50h]
    float v9; // [esp+40h] [ebp-4Ch]
    float a4; // [esp+44h] [ebp-48h]
    Vec3 a1; // [esp+4Ch] [ebp-40h] BYREF
    float v13; // [esp+5Ch] [ebp-30h]
    float v14; // [esp+60h] [ebp-2Ch]
    float v15; // [esp+64h] [ebp-28h]
    int v16; // [esp+68h] [ebp-24h]
    BOOL v17; // [esp+6Ch] [ebp-20h]
    tScriptStruct_PNJSectreBras* v18; // [esp+70h] [ebp-1Ch]
    Vec3 a3; // [esp+74h] [ebp-18h] BYREF
    Vec3 v20; // [esp+80h] [ebp-Ch] BYREF

    v6.x = 1.0;
    v6.y = 0.0;
    v6.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJSectreBras*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v18 = v1;
    if (v1->i_flag_dyn_on)
    {
        if (!AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 4))
        {
            v17 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
            v16 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 4, 0);
            v7 = AI_EvalFunc_MATHRandFloat_C(15.0, 17.0);
            v5.x = 0.0;
            v5.y = 0.0;
            v5.z = -v7;
            AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v5);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
            v6 = a1;
            v4 = (tGameObjectMain*)v18->des_papa;
            if (v4)
            {
                if ((unsigned int)v4 <= 2)
                    v4 = AI_MainActor[(uint32_t)v4 - 1];
                AI_EvalFunc_OBJ_HorizonGet_C(v4, &v20);
            }
            else
            {
                AI_EvalFunc_OBJ_HorizonGet_C(0, &v20);
            }
            v9 = AI_EvalFunc_MATHRandFloat_C(0.40000001f, 0.80000001f);
            v3 = v9 * v18->des_bras;
            v13 = v20.x * v3;
            v14 = v20.y * v3;
            v15 = v20.z * v3;
            v6.x = v6.x + v13;
            v6.y = v6.y + v14;
            v6.z = v6.z + v15;
            v2 = (tGameObjectMain*)v18->des_papa;
            if (v2)
            {
                if ((unsigned int)v2 <= 2)
                    v2 = AI_MainActor[(uint32_t)v2 - 1];
                AI_EvalFunc_OBJGetPos_C(v2, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            v6.z = a3.z;
            a4 = AI_EvalFunc_MATHRandFloat_C(0.40000001f, 0.80000001f);
            AI_EvalFunc_DYNImpulsion_C(AI_gpst_CurrentGameObject, &v6, a4);
            AI_EvalFunc_COLPrioritySet_C(AI_gpst_CurrentGameObject, 1);
        }
    }
    else if (AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 4))
    {
        v8 = AI_EvalFunc_DYNOff_C(AI_gpst_CurrentGameObject);
    }
    return 1000;
}

int PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init()
{
    tScriptStruct_PNJSectreBras* v1; // [esp+8h] [ebp-164h]
    tGameObjectMain* v2; // [esp+10h] [ebp-15Ch]
    tGameObjectMain* v3; // [esp+18h] [ebp-154h]
    tGameObjectMain* v4; // [esp+20h] [ebp-14Ch]
    tGameObjectMain* v5; // [esp+28h] [ebp-144h]
    tGameObjectMain* v6; // [esp+34h] [ebp-138h]
    tGameObjectMain* v7; // [esp+3Ch] [ebp-130h]
    tGameObjectMain* v8; // [esp+44h] [ebp-128h]
    tGameObjectMain* v9; // [esp+50h] [ebp-11Ch]
    tGameObjectMain* v10; // [esp+58h] [ebp-114h]
    tGameObjectMain* v11; // [esp+64h] [ebp-108h]
    tGameObjectMain* v12; // [esp+6Ch] [ebp-100h]
    tGameObjectMain* v13; // [esp+74h] [ebp-F8h]
    tGameObjectMain* v14; // [esp+80h] [ebp-ECh]
    tGameObjectMain* v15; // [esp+88h] [ebp-E4h]
    tGameObjectMain* v16; // [esp+94h] [ebp-D8h]
    tGameObjectMain* v17; // [esp+9Ch] [ebp-D0h]
    tGameObjectMain* v18; // [esp+A4h] [ebp-C8h]
    tGameObjectMain* v19; // [esp+B0h] [ebp-BCh]
    tGameObjectMain* v20; // [esp+B8h] [ebp-B4h]
    tGameObjectMain* v21; // [esp+C0h] [ebp-ACh]
    tGameObjectMain* v22; // [esp+C8h] [ebp-A4h]
    Vec3 v23; // [esp+CCh] [ebp-A0h] BYREF
    Vec3 v24; // [esp+D8h] [ebp-94h] BYREF
    Vec3 v25; // [esp+E4h] [ebp-88h] BYREF
    Vec3 v26; // [esp+F0h] [ebp-7Ch] BYREF
    Vec3 v27; // [esp+FCh] [ebp-70h] BYREF
    Vec3 v28; // [esp+10Ch] [ebp-60h] BYREF
    Vec3 v29; // [esp+118h] [ebp-54h] BYREF
    tGameObjectMain* v30; // [esp+124h] [ebp-48h]
    Vec3 v31; // [esp+128h] [ebp-44h] BYREF
    Vec3 v32; // [esp+134h] [ebp-38h] BYREF
    Vec3 a3; // [esp+140h] [ebp-2Ch] BYREF
    tScriptStruct_PNJSectreBras* v34; // [esp+14Ch] [ebp-20h]
    Vec3 v35; // [esp+150h] [ebp-1Ch] BYREF
    Vec3 v36; // [esp+15Ch] [ebp-10h] BYREF

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJSectreBras*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v34 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init = (int)AI_ul_RealResolveOneRefFunction(0x72005509u);
        fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move = (int)AI_ul_RealResolveOneRefFunction(0x72005530u);
        fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterrec_move = (int)AI_ul_RealResolveOneRefFunction(0x7200554Eu);
    }
    if (v34->f_time_start == 0.0)
    {
        v34->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
    }
    v22 = (tGameObjectMain*)v34->o_bras;
    if (v22)
    {
        if ((unsigned int)v22 <= 2)
            v22 = AI_MainActor[(uint32_t)v22 - 1];
        AI_EvalFunc_OBJGetPos_C(v22, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    AI_EvalFunc_OBJSetPos_C_CURRENT(&a3);
    v21 = (tGameObjectMain*)v34->o_bras;
    if (v21)
    {
        if ((unsigned int)v21 <= 2)
            v21 = AI_MainActor[(uint32_t)v21 - 1];
        AI_EvalFunc_OBJ_SightGet_C(v21, &v31);
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C(0, &v31);
    }
    v20 = (tGameObjectMain*)v34->o_bras;
    if (v20)
    {
        if ((unsigned int)v20 <= 2)
            v20 = AI_MainActor[(uint32_t)v20 - 1];
        AI_EvalFunc_OBJ_BankingGet_C(v20, &v24);
    }
    else
    {
        AI_EvalFunc_OBJ_BankingGet_C(0, &v24);
    }
    AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v31, &v24);
    v19 = (tGameObjectMain*)v34->o_bras;
    if (v19)
    {
        if ((unsigned int)v19 <= 2)
            v19 = AI_MainActor[(uint32_t)v19 - 1];
        AI_EvalFunc_OBJGetPos_C(v19, &v23);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v23);
    }
    v18 = (tGameObjectMain*)v34->ao_obj[0];
    if (v18)
    {
        if ((unsigned int)v18 <= 2)
            v18 = AI_MainActor[(uint32_t)v18 - 1];
        AI_EvalFunc_OBJSetPos_C(v18, &v23);
    }
    else
    {
        AI_EvalFunc_OBJSetPos_C(0, &v23);
    }
    v17 = (tGameObjectMain*)v34->o_bras;
    if (v17)
    {
        if ((unsigned int)v17 <= 2)
            v17 = AI_MainActor[(uint32_t)v17 - 1];
        AI_EvalFunc_OBJ_SightGet_C(v17, &v28);
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C(0, &v28);
    }
    v16 = (tGameObjectMain*)v34->o_bras;
    if (v16)
    {
        if ((unsigned int)v16 <= 2)
            v16 = AI_MainActor[(uint32_t)v16 - 1];
        AI_EvalFunc_OBJ_BankingGet_C(v16, &v36);
    }
    else
    {
        AI_EvalFunc_OBJ_BankingGet_C(0, &v36);
    }
    v15 = (tGameObjectMain*)v34->ao_obj[0];
    if (v15)
    {
        if ((unsigned int)v15 <= 2)
            v15 = AI_MainActor[(uint32_t)v15 - 1];
        AI_EvalFunc_OBJ_BankingGeneralSet_C(v15, &v28, &v36);
    }
    else
    {
        AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v28, &v36);
    }
    v14 = (tGameObjectMain*)v34->o_avant_bras;
    if (v14)
    {
        if ((unsigned int)v14 <= 2)
            v14 = AI_MainActor[(uint32_t)v14 - 1];
        AI_EvalFunc_OBJGetPos_C(v14, &v26);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v26);
    }
    v13 = (tGameObjectMain*)v34->ao_obj[1];
    if (v13)
    {
        if ((unsigned int)v13 <= 2)
            v13 = AI_MainActor[(uint32_t)v13 - 1];
        AI_EvalFunc_OBJSetPos_C(v13, &v26);
    }
    else
    {
        AI_EvalFunc_OBJSetPos_C(0, &v26);
    }
    v12 = (tGameObjectMain*)v34->o_avant_bras;
    if (v12)
    {
        if ((unsigned int)v12 <= 2)
            v12 = AI_MainActor[(uint32_t)v12 - 1];
        AI_EvalFunc_OBJ_SightGet_C(v12, &v32);
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C(0, &v32);
    }
    v11 = (tGameObjectMain*)v34->o_avant_bras;
    if (v11)
    {
        if ((unsigned int)v11 <= 2)
            v11 = AI_MainActor[(uint32_t)v11 - 1];
        AI_EvalFunc_OBJ_BankingGet_C(v11, &v35);
    }
    else
    {
        AI_EvalFunc_OBJ_BankingGet_C(0, &v35);
    }
    v10 = (tGameObjectMain*)v34->ao_obj[1];
    if (v10)
    {
        if ((unsigned int)v10 <= 2)
            v10 = AI_MainActor[(uint32_t)v10 - 1];
        AI_EvalFunc_OBJ_BankingGeneralSet_C(v10, &v32, &v35);
    }
    else
    {
        AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v32, &v35);
    }
    v9 = (tGameObjectMain*)v34->o_main;
    if (v9)
    {
        if ((unsigned int)v9 <= 2)
            v9 = AI_MainActor[(uint32_t)v9 - 1];
        AI_EvalFunc_OBJGetPos_C(v9, &v27);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v27);
    }
    v8 = (tGameObjectMain*)v34->ao_obj[2];
    if (v8)
    {
        if ((unsigned int)v8 <= 2)
            v8 = AI_MainActor[(uint32_t)v8 - 1];
        AI_EvalFunc_OBJSetPos_C(v8, &v27);
    }
    else
    {
        AI_EvalFunc_OBJSetPos_C(0, &v27);
    }
    v7 = (tGameObjectMain*)v34->o_main;
    if (v7)
    {
        if ((unsigned int)v7 <= 2)
            v7 = AI_MainActor[(uint32_t)v7 - 1];
        AI_EvalFunc_OBJ_SightGet_C(v7, &v29);
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C(0, &v29);
    }
    v6 = (tGameObjectMain*)v34->o_main;
    if (v6)
    {
        if ((unsigned int)v6 <= 2)
            v6 = AI_MainActor[(uint32_t)v6 - 1];
        AI_EvalFunc_OBJ_BankingGet_C(v6, &v25);
    }
    else
    {
        AI_EvalFunc_OBJ_BankingGet_C(0, &v25);
    }
    v5 = (tGameObjectMain*)v34->ao_obj[2];
    if (v5)
    {
        if ((unsigned int)v5 <= 2)
            v5 = AI_MainActor[(uint32_t)v5 - 1];
        AI_EvalFunc_OBJ_BankingGeneralSet_C(v5, &v29, &v25);
    }
    else
    {
        AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v29, &v25);
    }
    if (AI_EvalFunc_TIMEElapsed_C(v34->f_time_start, 0.050000001f))
    {
        v34->f_time_start = 0.0;
        v4 = (tGameObjectMain*)v34->des_papa;
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                v4,
                3,
                (tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                0,
                3,
                (tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init);
        }
        v3 = (tGameObjectMain*)v34->des_papa;
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v3,
                3,
                (tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                3,
                (tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move);
        }
        v30 = AI_gpst_CurrentGameObject;
        v2 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v2 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v2,
                2,
                (tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterrec_move);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterrec_move);
        }
    }
    return 1000;
}

int PNJ_Spectre_bras_PNJ_Spectre_Bras_afterrec_move()
{
    unsigned int v0; // eax
    int result; // eax
    tScriptStruct_PNJSectreBras* v2; // [esp+0h] [ebp-18h]
    tGameObjectMain* v3; // [esp+8h] [ebp-10h]
    int v4; // [esp+10h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJSectreBras*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterrec_move_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move = (int)AI_ul_RealResolveOneRefFunction(0x72005530u);
    }
    v3 = (tGameObjectMain*)v2->des_papa;
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(v3);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
    }
    if ((v0 & 0x220000) == 0
        || (v4 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterrec_move_PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move),
            v4 == 1000))
    {
        result = 1000;
    }
    else
    {
        result = v4;
    }
    return result;
}

int PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_GFX_Part()
{
    float v1; // [esp+0h] [ebp-138h]
    float v2; // [esp+4h] [ebp-134h]
    float v3; // [esp+8h] [ebp-130h]
    float v4; // [esp+Ch] [ebp-12Ch]
    float v5; // [esp+10h] [ebp-128h]
    float v6; // [esp+14h] [ebp-124h]
    tScriptStruct_PNJSectreBras* v7; // [esp+18h] [ebp-120h]
    tGameObjectMain* v8; // [esp+20h] [ebp-118h]
    tGameObjectMain* v9; // [esp+30h] [ebp-108h]
    tGameObjectMain* v10; // [esp+58h] [ebp-E0h]
    tGameObjectMain* v11; // [esp+60h] [ebp-D8h]
    Vec3 v12; // [esp+64h] [ebp-D4h]
    float v13; // [esp+70h] [ebp-C8h]
    float v14; // [esp+74h] [ebp-C4h]
    float v15; // [esp+78h] [ebp-C0h]
    Vec3 v16; // [esp+7Ch] [ebp-BCh] BYREF
    Vec3 v17; // [esp+88h] [ebp-B0h] BYREF
    float v18; // [esp+94h] [ebp-A4h]
    float v19; // [esp+98h] [ebp-A0h]
    float v20; // [esp+9Ch] [ebp-9Ch]
    float v21; // [esp+A0h] [ebp-98h]
    float v22; // [esp+A4h] [ebp-94h]
    Vec3 a3; // [esp+A8h] [ebp-90h] BYREF
    float v24; // [esp+B8h] [ebp-80h]
    float v25; // [esp+BCh] [ebp-7Ch]
    float v26; // [esp+C0h] [ebp-78h]
    Vec3 v27; // [esp+C4h] [ebp-74h] BYREF
    Vec3 v28; // [esp+D0h] [ebp-68h]
    Vec3 v29; // [esp+DCh] [ebp-5Ch] BYREF
    float v30; // [esp+E8h] [ebp-50h]
    float v31; // [esp+ECh] [ebp-4Ch]
    float v32; // [esp+F0h] [ebp-48h]
    Vec3 v33; // [esp+F4h] [ebp-44h] BYREF
    float v34; // [esp+100h] [ebp-38h]
    Vec3 v35; // [esp+104h] [ebp-34h] BYREF
    float v36; // [esp+110h] [ebp-28h]
    Vec3 v37; // [esp+114h] [ebp-24h] BYREF
    tScriptStruct_PNJSectreBras* v38; // [esp+120h] [ebp-18h]
    Vec3 v39; // [esp+124h] [ebp-14h]
    float v40; // [esp+130h] [ebp-8h]
    float v41; // [esp+134h] [ebp-4h]

    v33.x = 1.0;
    v33.y = 0.0;
    v33.z = 0.0;
    v24 = 1.0;
    v25 = 0.0;
    v26 = 0.0;
    v17.x = 1.0;
    v17.y = 0.0;
    v17.z = 0.0;
    v29.x = 1.0;
    v29.y = 0.0;
    v29.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_PNJSectreBras*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    v38 = v7;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_GFX_Part_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    if (v38->GFX_Part == -1)
    {
        v38->GFX_Part = AI_EvalFunc_GFXAdd_C_CURRENT(13);
        GFXSetCurGlobal(v38->GFX_Part);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 35);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13106, 0x540BE3FF);
        v11 = (tGameObjectMain*)PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_GFX_Part_SFX_light_smoke_gao;
        if (PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_GFX_Part_SFX_light_smoke_gao)
        {
            if ((unsigned int)PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_GFX_Part_SFX_light_smoke_gao <= 2)
                v11 = AI_MainActor[PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_GFX_Part_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v38->GFX_Part, v11, -1);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v38->GFX_Part, 0, -1);
        }
        GFXSetCurGlobal(v38->GFX_Part);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13101, 0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13103, 0xFF408040);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13104, 0x80408040);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13105, 0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13107, 0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13000, 0.1f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13001, 0.2f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13002, 0.001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13003, 0.5f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13004, 0.5f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13005, 0.1f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13006, 0.2f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13007, -0.5f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.050000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13009, 0.5);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13010, 2.0);
        v37.x = 0.039999999f;
        v37.y = 0.039999999f;
        v37.z = 0.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v37);
        v35.x = 0.0;
        v35.y = 0.0;
        v35.z = 0.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13204, &v35);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v38->GFX_Part, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v38->GFX_Part, 2, 1);
        AI_EvalFunc_GFXSetf_C_CURRENT(v38->GFX_Part, 13012, 0.75);
    }
    v10 = (tGameObjectMain*)v38->o_bras;
    if (v10)
    {
        if ((unsigned int)v10 <= 2)
            v10 = AI_MainActor[(uint32_t)v10 - 1];
        AI_EvalFunc_OBJ_BankingGet_C(v10, &v16);
    }
    else
    {
        AI_EvalFunc_OBJ_BankingGet_C(0, &v16);
    }
    v13 = v16.x * 3.0f;
    v14 = v16.y * 3.0f;
    v15 = v16.z * 3.0f;
    v28.x = 1.0;
    v28.y = 1.0;
    v28.z = 0.0;
    v17.x = v13 - 1.0f;
    v17.y = v14 - 1.0f;
    v17.z = v15 - 0.0f;
    v39.x = 1.0;
    v39.y = 1.0;
    v39.z = 0.0;
    v24 = v13 + 1.0f;
    v25 = v14 + 1.0f;
    v26 = v15 + 0.0f;
    if (v17.x > (double)v24)
        v6 = v24;
    else
        v6 = v17.x;
    v19 = v6;
    if (v17.y > (double)v25)
        v5 = v25;
    else
        v5 = v17.y;
    v36 = v5;
    if (v17.z > (double)v26)
        v4 = v26;
    else
        v4 = v17.z;
    v40 = v4;
    v20 = v19;
    v21 = v36;
    v22 = v4;
    v33.x = v19;
    v33.y = v36;
    v33.z = v4;
    if (v17.x < (double)v24)
        v3 = v24;
    else
        v3 = v17.x;
    v34 = v3;
    if (v17.y < (double)v25)
        v2 = v25;
    else
        v2 = v17.y;
    v18 = v2;
    if (v17.z < (double)v26)
        v1 = v26;
    else
        v1 = v17.z;
    v41 = v1;
    v30 = v34;
    v31 = v18;
    v32 = v1;
    v29.x = v34;
    v29.y = v18;
    v29.z = v1;
    GFXSetCurGlobal(v38->GFX_Part);
    AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v33);
    AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v29);
    v9 = (tGameObjectMain*)v38->o_bras;
    if (v9)
    {
        if ((unsigned int)v9 <= 2)
            v9 = AI_MainActor[(uint32_t)v9 - 1];
        AI_EvalFunc_OBJGetPos_C(v9, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    v17 = a3;
    v8 = (tGameObjectMain*)v38->o_bras;
    if (v8)
    {
        if ((unsigned int)v8 <= 2)
            v8 = AI_MainActor[(uint32_t)v8 - 1];
        AI_EvalFunc_OBJ_BankingGet_C(v8, &v27);
    }
    else
    {
        AI_EvalFunc_OBJ_BankingGet_C(0, &v27);
    }
    v12.x = v27.x * 0.40000001f;
    v12.y = v27.y * 0.40000001f;
    v12.z = v27.z * 0.40000001f;
    v17.x = v17.x + v12.x;
    v17.y = v17.y + v12.y;
    v17.z = v17.z + v12.z;
    AI_EvalFunc_GFXSetv_C_CURGLOB(13200, &v17);
    return 1000;
}

int PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_init_snake()
{
    float v1; // [esp+0h] [ebp-74h]
    tScriptStruct_PNJSectreBras* v2; // [esp+4h] [ebp-70h]
    tGameObjectMain* v3; // [esp+Ch] [ebp-68h]
    tGameObjectMain* v4; // [esp+20h] [ebp-54h]
    tGameObjectMain* v5; // [esp+28h] [ebp-4Ch]
    tGameObjectMain* v6; // [esp+30h] [ebp-44h]
    Vec3 a1; // [esp+3Ch] [ebp-38h] BYREF
    Vec3 a3; // [esp+48h] [ebp-2Ch] BYREF
    Vec3 dest; // [esp+54h] [ebp-20h] BYREF
    Vec3 v10; // [esp+60h] [ebp-14h] BYREF
    tScriptStruct_PNJSectreBras* v11; // [esp+6Ch] [ebp-8h]
    int i; // [esp+70h] [ebp-4h]

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJSectreBras*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v11 = v2;
    i = 0;
    v6 = (tGameObjectMain*)v2->ao_obj[0];
    if (v6)
    {
        if ((unsigned int)v6 <= 2)
            v6 = AI_MainActor[(uint32_t)v6 - 1];
        OBJ_RestoreInitialPos(v6);
    }
    else
    {
        OBJ_RestoreInitialPos(0);
    }
    for (i = 1; i < v11->des_i_numbones; ++i)
    {
        v5 = (tGameObjectMain*)v11->ao_obj[i];
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
        v4 = (tGameObjectMain*)*(&v11->__ao_obj_Dimension1 + i);
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
        MATH_SubstractVec(&dest, &a3, &v10);
        a1 = dest;
        v1 = MATH_VecLen(&a1);
        *(float*)AI_EvalVarArray_C2((char*)&v11->__af_lenos_Dimension1, i) = v1;
    }
    for (i = 1; i < v11->des_i_numbones; ++i)
    {
        v3 = (tGameObjectMain*)v11->ao_obj[i];
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            AI_EvalFunc_OBJ_HierarchyReset_C(v3);
        }
        else
        {
            AI_EvalFunc_OBJ_HierarchyReset_C(0);
        }
    }
    return 1000;
}

int PNJ_Spectre_bras_PNJ_Spectre_Bras_Destroy()
{
    int result; // eax
    int v1; // [esp+4h] [ebp-8h]

    v1 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
    if (v1 == 1000)
        result = 1000;
    else
        result = v1;
    return result;
}