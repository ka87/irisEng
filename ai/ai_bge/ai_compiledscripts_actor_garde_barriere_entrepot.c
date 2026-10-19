#include "ai_compiledscripts_inc.h"

//Garde_Barriere_Entrepot_GBE_init
int Garde_Barriere_Entrepot_GBE_init_univVars;
int Garde_Barriere_Entrepot_GBE_init_univVars_Flag_speciaux_sauve;
int Garde_Barriere_Entrepot_GBE_init__PJ_Aircraft_gao;
int Garde_Barriere_Entrepot_GBE_init_GST_Global_gao;
int Garde_Barriere_Entrepot_GBE_init_GST_Global_gao_o_targets;
int Garde_Barriere_Entrepot_GBE_init_GST_Global_gao_i_targets_nb;
int Garde_Barriere_Entrepot_GBE_init_GST_Global_gao_v_targets_offset;
int Garde_Barriere_Entrepot_GBE_init_GST_Global_gao_i_nb_tesla_recepteurs;
int Garde_Barriere_Entrepot_GBE_init_GST_Global_gao_o_tesla_recepteur;
int fct_Garde_Barriere_Entrepot_GBE_init_Garde_Barriere_Entrepot_GBE_mode_attente;
int fct_Garde_Barriere_Entrepot_GBE_init_Garde_Barriere_Entrepot_GBE_exec_GFX_Reacteur;
int fct_Garde_Barriere_Entrepot_GBE_init_Garde_Barriere_Entrepot_GBE_reflex;

//Garde_Barriere_Entrepot_GBE_reflex
int Garde_Barriere_Entrepot_GBE_reflex_univVars;
int Garde_Barriere_Entrepot_GBE_reflex_univVars_Perso_ID_actor;
int Garde_Barriere_Entrepot_GBE_reflex_TXT_Sally_gao;
int Garde_Barriere_Entrepot_GBE_reflex_univVars_Flag_speciaux_sauve;
int fctvar_Garde_Barriere_Entrepot_GBE_reflex_Hovercraft_shoot;
int Garde_Barriere_Entrepot_GBE_reflex_SFX_explosion_gao;
int Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_unknown_demo;
int fct_Garde_Barriere_Entrepot_GBE_reflex__PJ_Sally_gao;
int Garde_Barriere_Entrepot_GBE_reflex__PJ_Aircraft_gao;
int fct_Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_mode_repousse;
int fct_Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_mode_choppe;
int fct_Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_mode_attente;
int fct_Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_exec_trace;
int fct_Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_mode_flotte;

//Garde_Barriere_Entrepot_GBE_unknown_demo
int Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao;
int Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_o_targets;
int Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_i_targets_nb;
int Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_v_targets_offset;
int Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_o_selected_target;
int Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_i_selected_target_nb;
int Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_v_selected_target_offset;

//Garde_Barriere_Entrepot_GBE_mode_attente
int Garde_Barriere_Entrepot_GBE_mode_attente__PJ_Aircraft_gao;
int Garde_Barriere_Entrepot_GBE_mode_attente_univVars;
int Garde_Barriere_Entrepot_GBE_mode_attente_univVars_Flag_speciaux_sauve;
int Garde_Barriere_Entrepot_GBE_mode_attente_univVars_Perso_ID_passengers_list;
int fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_mode_choppe;
int fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_mode_attente;
int Garde_Barriere_Entrepot_GBE_mode_attente__PJ_Sally_gao;
int Garde_Barriere_Entrepot_GBE_mode_attente_TXT_Sally_gao;
int fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_exec_camera;
int fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning;

//Garde_Barriere_Entrepot_GBE_exec_trace
int Garde_Barriere_Entrepot_GBE_exec_trace_PJ_aircraft_material_gao;

//Garde_Barriere_Entrepot_GBE_exec_camera
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao;
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_o_gencam_mario;
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_CameraNew_gao;
int Garde_Barriere_Entrepot_GBE_exec_camera_GST_Board_panel_gao;
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_current_camera_mode;
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_i_canuserctrl;
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_i_canusercut;
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_i_priority;
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_i_test_vis;
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_f_focal_gc;
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_f_dist_cut;
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_f_ideal_dist;
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_Target_gao;
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_Target_gao_v_offset_sight_mario;
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_Target_gao_f_speed_trans;
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_Target_gao_o_followed_actor1;
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_f_ideal_Z;
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Aircraft_gao;
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_f_inertie;
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_v_ideal_axis;
int Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_f_AngleZ;

//Garde_Barriere_Entrepot_GBE_exec_GFX_Smoke
int Garde_Barriere_Entrepot_GBE_exec_GFX_Smoke_SFX_explosion_gao;

//Garde_Barriere_Entrepot_GBE_mode_flotte
int Garde_Barriere_Entrepot_GBE_mode_flotte__PJ_Camera_gao;
int Garde_Barriere_Entrepot_GBE_mode_flotte_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning;
int Garde_Barriere_Entrepot_GBE_mode_flotte_Garde_Barriere_Entrepot_GBE_exec_GFX_Reacteur;
int Garde_Barriere_Entrepot_GBE_mode_flotte_univVars;
int Garde_Barriere_Entrepot_GBE_mode_flotte_univVars_Flag_speciaux_sauve;
int fctvar_Garde_Barriere_Entrepot_GBE_mode_flotte_camera;
int fct_Garde_Barriere_Entrepot_GBE_mode_flotte_Garde_Barriere_Entrepot_GBE_exec_GFX_Smoke;

//Garde_Barriere_Entrepot_GBE_mode_repousse
int Garde_Barriere_Entrepot_GBE_mode_repousse__PJ_Aircraft_gao;
int Garde_Barriere_Entrepot_GBE_mode_repousse_univVars;
int Garde_Barriere_Entrepot_GBE_mode_repousse_univVars_Perso_ID_passengers_list;
int Garde_Barriere_Entrepot_GBE_mode_repousse__PJ_Sally_gao;
int Garde_Barriere_Entrepot_GBE_mode_repousse_TXT_Sally_gao;
int fct_Garde_Barriere_Entrepot_GBE_mode_repousse_Garde_Barriere_Entrepot_GBE_mode_repousse;
int fct_Garde_Barriere_Entrepot_GBE_mode_repousse_Garde_Barriere_Entrepot_GBE_mode_attente;
int fct_Garde_Barriere_Entrepot_GBE_mode_repousse_Garde_Barriere_Entrepot_GBE_exec_camera;
int fct_Garde_Barriere_Entrepot_GBE_mode_repousse_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning;

//Garde_Barriere_Entrepot_GBE_exec_GFX_anneaux
int Garde_Barriere_Entrepot_GBE_exec_GFX_anneaux_SFX_light_smoke_gao;

//Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning
int fct_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning;
int Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning_SFX_light_smoke_gao;

//Garde_Barriere_Entrepot_GBE_mode_choppe
int Garde_Barriere_Entrepot_GBE_mode_choppe__PJ_Aircraft_gao;
int Garde_Barriere_Entrepot_GBE_mode_choppe__PJ_Sally_gao;
int Garde_Barriere_Entrepot_GBE_mode_choppe_TXT_Sally_gao;
int fct_Garde_Barriere_Entrepot_GBE_mode_choppe_Garde_Barriere_Entrepot_GBE_exec_camera;
int fct_Garde_Barriere_Entrepot_GBE_mode_choppe_Garde_Barriere_Entrepot_GBE_mode_choppe;
int fct_Garde_Barriere_Entrepot_GBE_mode_choppe_Garde_Barriere_Entrepot_GBE_mode_repousse;
int fct_Garde_Barriere_Entrepot_GBE_mode_choppe_Garde_Barriere_Entrepot_GBE_exec_GFX_anneaux;
int fct_Garde_Barriere_Entrepot_GBE_mode_choppe_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning;

int Garde_Barriere_Entrepot_GBE_init()
{
    int result; // eax
    double v1; // st7
    double v2; // st7
    float v3; // [esp+8h] [ebp-340h]
    tScriptStruct_PNJGardeBarriereEntrepot* v4; // [esp+10h] [ebp-338h]
    tGameObjectMain* v5; // [esp+18h] [ebp-330h]
    tGameObjectMain* v6; // [esp+20h] [ebp-328h]
    tGameObjectMain* v7; // [esp+B8h] [ebp-290h]
    tGameObjectMain* v8; // [esp+C0h] [ebp-288h]
    tGameObjectMain* v9; // [esp+C8h] [ebp-280h]
    tGameObjectMain* v10; // [esp+D0h] [ebp-278h]
    tGameObjectMain* v11; // [esp+E8h] [ebp-260h]
    tGameObjectMain* v12; // [esp+F0h] [ebp-258h]
    tGameObjectMain* v13; // [esp+F8h] [ebp-250h]
    tGameObjectMain* v14; // [esp+100h] [ebp-248h]
    tGameObjectMain* v15; // [esp+104h] [ebp-244h]
    tGameObjectMain* v16; // [esp+108h] [ebp-240h]
    tGameObjectMain* v17; // [esp+10Ch] [ebp-23Ch]
    tGameObjectMain* v18; // [esp+110h] [ebp-238h]
    float v19; // [esp+11Ch] [ebp-22Ch]
    tGameObjectMain* v20; // [esp+12Ch] [ebp-21Ch]
    tGameObjectMain* v21; // [esp+134h] [ebp-214h]
    float v22; // [esp+138h] [ebp-210h]
    float v23; // [esp+13Ch] [ebp-20Ch]
    tGameObjectMain* v24; // [esp+14Ch] [ebp-1FCh]
    tGameObjectMain* v25; // [esp+154h] [ebp-1F4h]
    tGameObjectMain* v26; // [esp+158h] [ebp-1F0h]
    tGameObjectMain* v27; // [esp+15Ch] [ebp-1ECh]
    tGameObjectMain* v28; // [esp+17Ch] [ebp-1CCh]
    tGameObjectMain* v29; // [esp+180h] [ebp-1C8h]
    Vec3 v30; // [esp+194h] [ebp-1B4h] BYREF
    int v31; // [esp+1A4h] [ebp-1A4h]
    Vec3 v32; // [esp+1A8h] [ebp-1A0h] BYREF
    Vec3 v33; // [esp+1B4h] [ebp-194h] BYREF
    int v34; // [esp+1C0h] [ebp-188h]
    Vec3 v35; // [esp+1C4h] [ebp-184h] BYREF
    Vec3 v36; // [esp+1D0h] [ebp-178h] BYREF
    int i; // [esp+1DCh] [ebp-16Ch]
    int v38; // [esp+1E0h] [ebp-168h]
    tGameObjectMain* v39; // [esp+1E8h] [ebp-160h]
    int v40; // [esp+1ECh] [ebp-15Ch]
    Vec3 v41; // [esp+1F0h] [ebp-158h] BYREF
    Vec3 v42; // [esp+204h] [ebp-144h] BYREF
    Vec3 a2; // [esp+210h] [ebp-138h] BYREF
    Vec3 v44; // [esp+21Ch] [ebp-12Ch] BYREF
    Vec3 v45; // [esp+228h] [ebp-120h] BYREF
    int v46; // [esp+234h] [ebp-114h]
    tGameObjectMain* v47; // [esp+238h] [ebp-110h]
    Vec3 a5; // [esp+23Ch] [ebp-10Ch] BYREF
    Vec3 v49; // [esp+248h] [ebp-100h]
    Vec3 v50; // [esp+254h] [ebp-F4h] BYREF
    Vec3 v51; // [esp+260h] [ebp-E8h] BYREF
    Vec3 v52; // [esp+26Ch] [ebp-DCh] BYREF
    Vec3 v53; // [esp+278h] [ebp-D0h]
    Vec3 dest; // [esp+284h] [ebp-C4h] BYREF
    Vec3 v55; // [esp+290h] [ebp-B8h] BYREF
    BOOL v56; // [esp+2A0h] [ebp-A8h]
    Vec3 v57; // [esp+2A4h] [ebp-A4h] BYREF
    Vec3 v58; // [esp+2B8h] [ebp-90h] BYREF
    Vec3 v59; // [esp+2C4h] [ebp-84h] BYREF
    Vec3 v60; // [esp+2D0h] [ebp-78h] BYREF
    tGameObjectMain* v61; // [esp+2DCh] [ebp-6Ch]
    int v62; // [esp+2E4h] [ebp-64h]
    Vec3 v63; // [esp+2E8h] [ebp-60h] BYREF
    tScriptStruct_PNJGardeBarriereEntrepot* v64; // [esp+2F4h] [ebp-54h]
    Vec3 a3; // [esp+2F8h] [ebp-50h] BYREF
    Vec3 v66; // [esp+304h] [ebp-44h] BYREF
    Vec3 v67; // [esp+310h] [ebp-38h] BYREF
    int v68; // [esp+320h] [ebp-28h]
    Vec3 v69; // [esp+330h] [ebp-18h] BYREF
    Vec3 v70; // [esp+33Ch] [ebp-Ch] BYREF

    v53.x = 1.0;
    v53.y = 0.0;
    v53.z = 0.0;
    v42.x = 1.0;
    v42.y = 0.0;
    v42.z = 0.0;
    v33.x = 1.0;
    v33.y = 0.0;
    v33.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_PNJGardeBarriereEntrepot*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v64 = v4;
    i = 0;
    v38 = 0;
    v56 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Garde_Barriere_Entrepot_GBE_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_init_univVars_Flag_speciaux_sauve,
            (void*)Garde_Barriere_Entrepot_GBE_init_univVars,
            4488);
        Garde_Barriere_Entrepot_GBE_init__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        Garde_Barriere_Entrepot_GBE_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_init_GST_Global_gao_o_targets,
            (void*)Garde_Barriere_Entrepot_GBE_init_GST_Global_gao,
            964);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_init_GST_Global_gao_i_targets_nb,
            (void*)Garde_Barriere_Entrepot_GBE_init_GST_Global_gao,
            2572);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_init_GST_Global_gao_v_targets_offset,
            (void*)Garde_Barriere_Entrepot_GBE_init_GST_Global_gao,
            1368);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_init_GST_Global_gao_i_nb_tesla_recepteurs,
            (void*)Garde_Barriere_Entrepot_GBE_init_GST_Global_gao,
            3724);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_init_GST_Global_gao_o_tesla_recepteur,
            (void*)Garde_Barriere_Entrepot_GBE_init_GST_Global_gao,
            3640);
        fct_Garde_Barriere_Entrepot_GBE_init_Garde_Barriere_Entrepot_GBE_mode_attente = (int)AI_ul_RealResolveOneRefFunction(0x720026E7u);
        fct_Garde_Barriere_Entrepot_GBE_init_Garde_Barriere_Entrepot_GBE_exec_GFX_Reacteur = (int)AI_ul_RealResolveOneRefFunction(0x720060FFu);
        fct_Garde_Barriere_Entrepot_GBE_init_Garde_Barriere_Entrepot_GBE_reflex = (int)AI_ul_RealResolveOneRefFunction(0x720026EDu);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)6)
    {
        v49.x = 0.0;
        v49.y = 0.0;
        v49.z = -7.0;
        v53.x = 0.0;
        v53.y = 0.0;
        v53.z = -7.0;
        v68 = AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 8, 0);
        v39 = AI_gpst_CurrentGameObject;
        v29 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v29 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            v28 = v29;
        }
        else
        {
            v28 = 0;
        }
        v38 = AI_EvalFunc_ARRObjSearch_C(
            (tGameObjectMain**)(Garde_Barriere_Entrepot_GBE_init_GST_Global_gao_o_targets + 4),
            *(uint32_t*)Garde_Barriere_Entrepot_GBE_init_GST_Global_gao_i_targets_nb,
            v28);
        if (v38 == -1)
        {
            v38 = AI_EvalFunc_ARRObjSearch_C(
                (tGameObjectMain**)(Garde_Barriere_Entrepot_GBE_init_GST_Global_gao_o_targets + 4),
                100,
                0);
            if (v38 != -1)
            {
                ++* (uint32_t*)Garde_Barriere_Entrepot_GBE_init_GST_Global_gao_i_targets_nb;
                *(uint32_t*)(Garde_Barriere_Entrepot_GBE_init_GST_Global_gao_o_targets + 4 * v38 + 4) = (uint32_t)AI_gpst_CurrentGameObject;
                *(Vec3*)AI_EvalVarArray_C3((char*)Garde_Barriere_Entrepot_GBE_init_GST_Global_gao_v_targets_offset, v38) = v53;
            }
        }
        *(uint32_t*)(Garde_Barriere_Entrepot_GBE_init_GST_Global_gao_o_tesla_recepteur
            + 4 * (*(uint32_t*)Garde_Barriere_Entrepot_GBE_init_GST_Global_gao_i_nb_tesla_recepteurs)++
            + 4) = (uint32_t)AI_gpst_CurrentGameObject;
        for (i = 0; i < 3; ++i)
        {
            v64->GFX_Lightning[i] = -1;
            v64->GFX_Halo[i] = -1;
        }
        for (i = 0; i < 10; ++i)
        {
            v27 = (tGameObjectMain*)v64->des_wp_repoussing[i];
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
            if (!v26)
                break;
            ++v64->i_nb_wp;
            v25 = (tGameObjectMain*)v64->des_wp_repoussing[i];
            if (v25)
            {
                if ((unsigned int)v25 <= 2)
                    v25 = AI_MainActor[(uint32_t)v25 - 1];
                AI_EvalFunc_OBJGetPos_C(v25, &v35);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v35);
            }
            v24 = (tGameObjectMain*)v64->des_o_wp;
            if (v24)
            {
                if ((unsigned int)v24 <= 2)
                    v24 = AI_MainActor[(uint32_t)v24 - 1];
                AI_EvalFunc_OBJGetPos_C(v24, &v70);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v70);
            }
            MATH_SubstractVec(&dest, &v35, &v70);
            v42 = dest;
            AI_EvalFunc_MATHVecSetHorzNormalize_C(&v42);
            if (v64->des_mode)
            {
                v23 = v64->des_f_dist_min;
                v42.x = v42.x * v23;
                v42.y = v42.y * v23;
                v42.z = v42.z * v23;
            }
            else
            {
                v22 = v64->des_f_dist_max;
                v42.x = v42.x * v22;
                v42.y = v42.y * v22;
                v42.z = v42.z * v22;
            }
            v21 = (tGameObjectMain*)v64->des_o_wp;
            if (v21)
            {
                if ((unsigned int)v21 <= 2)
                    v21 = AI_MainActor[(uint32_t)v21 - 1];
                AI_EvalFunc_OBJGetPos_C(v21, &v41);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v41);
            }
            v42.x = v42.x + v41.x;
            v42.y = v42.y + v41.y;
            v42.z = v42.z + v41.z;
            v20 = (tGameObjectMain*)v64->des_wp_repoussing[i];
            if (v20)
            {
                if ((unsigned int)v20 <= 2)
                    v20 = AI_MainActor[(uint32_t)v20 - 1];
                AI_EvalFunc_OBJSetPos_C(v20, &v42);
            }
            else
            {
                AI_EvalFunc_OBJSetPos_C(0, &v42);
            }
        }
        v46 = AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 2, 1);
        AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x8000, 0);
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 2, 1);
        AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0xFFu, 0);
        AI_EvalFunc_COLPrioritySet_C(AI_gpst_CurrentGameObject, 100);
        AI_EvalFunc_COLPosGet_C(AI_gpst_CurrentGameObject, 0, &v30);
        v33 = v30;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v45);
        v19 = v64->des_f_dist_centre_grav;
        v60.x = v45.x * v19;
        v60.y = v45.y * v19;
        v60.z = v45.z * v19;
        MATH_SubstractVec(&v52, &v33, &v60);
        v64->v_grav_pos = v52;
        v64->v_grav_speed.x = 0.0;
        v64->v_grav_speed.y = 0.0;
        v64->v_grav_speed.z = 0.0;
        v64->f_dist_range = v64->des_f_dist_max - v64->des_f_dist_min;
        v18 = (tGameObjectMain*)v64->des_wp_cone_1;
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
            v16 = (tGameObjectMain*)v64->des_wp_cone_2;
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
                v64->i_flag_stay_in_cone = 1;
                v14 = (tGameObjectMain*)v64->des_wp_cone_1;
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
                v13 = (tGameObjectMain*)v64->des_o_wp;
                if (v13)
                {
                    if ((unsigned int)v13 <= 2)
                        v13 = AI_MainActor[(uint32_t)v13 - 1];
                    AI_EvalFunc_OBJGetPos_C(v13, &v44);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v44);
                }
                MATH_SubstractVec(&a2, &v57, &v44);
                v12 = (tGameObjectMain*)v64->des_wp_cone_2;
                if (v12)
                {
                    if ((unsigned int)v12 <= 2)
                        v12 = AI_MainActor[(uint32_t)v12 - 1];
                    AI_EvalFunc_OBJGetPos_C(v12, &v59);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v59);
                }
                v11 = (tGameObjectMain*)v64->des_o_wp;
                if (v11)
                {
                    if ((unsigned int)v11 <= 2)
                        v11 = AI_MainActor[(uint32_t)v11 - 1];
                    AI_EvalFunc_OBJGetPos_C(v11, &v32);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v32);
                }
                MATH_SubstractVec(&a3, &v59, &v32);
                AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, 0.5, &a5);
                v42 = a5;
                AI_EvalFunc_MATHVecSetHorzNormalize_C(&v42);
                v64->v_cone_axe = v42;
                v10 = (tGameObjectMain*)v64->des_wp_cone_2;
                if (v10)
                {
                    if ((unsigned int)v10 <= 2)
                        v10 = AI_MainActor[(uint32_t)v10 - 1];
                    AI_EvalFunc_OBJGetPos_C(v10, &v63);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v63);
                }
                v9 = (tGameObjectMain*)v64->des_o_wp;
                if (v9)
                {
                    if ((unsigned int)v9 <= 2)
                        v9 = AI_MainActor[(uint32_t)v9 - 1];
                    AI_EvalFunc_OBJGetPos_C(v9, &v50);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v50);
                }
                MATH_SubstractVec(&v55, &v63, &v50);
                v8 = (tGameObjectMain*)v64->des_wp_cone_1;
                if (v8)
                {
                    if ((unsigned int)v8 <= 2)
                        v8 = AI_MainActor[(uint32_t)v8 - 1];
                    AI_EvalFunc_OBJGetPos_C(v8, &v67);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v67);
                }
                v7 = (tGameObjectMain*)v64->des_o_wp;
                if (v7)
                {
                    if ((unsigned int)v7 <= 2)
                        v7 = AI_MainActor[(uint32_t)v7 - 1];
                    AI_EvalFunc_OBJGetPos_C(v7, &v36);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v36);
                }
                MATH_SubstractVec(&v69, &v67, &v36);
                v58.x = 0.0;
                v58.y = 0.0;
                v58.z = 1.0;
                v1 = MATH_f_VecAngle(&v55, &v69, &v58);
                v64->f_cone_angle = (float)v1;
                v3 = (float)fabs(v64->f_cone_angle * 0.5f);
                v64->f_cone_angle = v3;
                v2 = cosf(v64->f_cone_angle);
                v64->f_cone_cos = (float)v2;
            }
        }
        AI_EvalFunc_COLSwapToSpecific_C(AI_gpst_CurrentGameObject, 0);
        v66.x = 0.0;
        v66.y = 0.0;
        v66.z = -6.5;
        AI_EvalFunc_COLSpecificPosSet_C(AI_gpst_CurrentGameObject, 0, &v66);
        v51.x = 9.0;
        v51.y = 9.0;
        v51.z = 9.0;
        AI_EvalFunc_COLSpecificSizeSet_C(AI_gpst_CurrentGameObject, 0, &v51);
        v47 = AI_gpst_CurrentGameObject;
        v6 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v6 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v6,
                2,
                (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_init_Garde_Barriere_Entrepot_GBE_mode_attente);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_init_Garde_Barriere_Entrepot_GBE_mode_attente);
        }
        v61 = AI_gpst_CurrentGameObject;
        v5 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v5 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v5,
                2,
                (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_init_Garde_Barriere_Entrepot_GBE_exec_GFX_Reacteur);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_init_Garde_Barriere_Entrepot_GBE_exec_GFX_Reacteur);
        }
        v40 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_init_Garde_Barriere_Entrepot_GBE_reflex);
        if (v40 == 1000)
            result = 1000;
        else
            result = v40;
    }
    else
    {
        v56 = AI_EvalFunc_ARRBitTest_C((int*)(Garde_Barriere_Entrepot_GBE_init_univVars_Flag_speciaux_sauve + 4), 2506);
        if (!v56 || (v62 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject), v62 == 1000))
        {
            if (v64->des_SF_inactif <= 0)
                v56 = 0;
            else
                v56 = AI_EvalFunc_ARRBitTest_C(
                    (int*)(Garde_Barriere_Entrepot_GBE_init_univVars_Flag_speciaux_sauve + 4),
                    v64->des_SF_inactif);
            if (!v56 || (v34 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject), v34 == 1000))
            {
                v64->f_time_keep_camera = -10000.0;
                v31 = AI_EvalFunc_AIPriorityGet_C((tGameObjectMain*)Garde_Barriere_Entrepot_GBE_init__PJ_Aircraft_gao);
                AI_EvalFunc_AIPrioritySet_C(AI_gpst_CurrentGameObject, v31 - 1);
                *AI_gppst_CurrentJumpNode = (tAINode*)6;
                TIME_Wait_C(0.1f);
                result = 0;
            }
            else
            {
                result = v34;
            }
        }
        else
        {
            result = v62;
        }
    }
    return result;
}

int Garde_Barriere_Entrepot_GBE_reflex()
{
    BOOL v0; // eax
    BOOL v1; // eax
    BOOL v2; // eax
    float v4; // [esp+4h] [ebp-2C0h]
    float v5; // [esp+8h] [ebp-2BCh]
    tScriptStruct_PNJGardeBarriereEntrepot* v6; // [esp+Ch] [ebp-2B8h]
    tGameObjectMain* v7; // [esp+14h] [ebp-2B0h]
    tGameObjectMain* v8; // [esp+1Ch] [ebp-2A8h]
    tGameObjectMain* v9; // [esp+24h] [ebp-2A0h]
    tGameObjectMain* v10; // [esp+2Ch] [ebp-298h]
    tGameObjectMain* v11; // [esp+34h] [ebp-290h]
    tGameObjectMain* v12; // [esp+3Ch] [ebp-288h]
    tGameObjectMain* v13; // [esp+44h] [ebp-280h]
    float v14; // [esp+4Ch] [ebp-278h]
    tGameObjectMain* v15; // [esp+68h] [ebp-25Ch]
    tGameObjectMain* v16; // [esp+7Ch] [ebp-248h]
    tGameObjectMain* v17; // [esp+84h] [ebp-240h]
    tGameObjectMain* v18; // [esp+90h] [ebp-234h]
    tGameObjectMain* v19; // [esp+A0h] [ebp-224h]
    tGameObjectMain* v20; // [esp+A4h] [ebp-220h]
    tGameObjectMain* v21; // [esp+A8h] [ebp-21Ch]
    tGameObjectMain* v22; // [esp+B0h] [ebp-214h]
    tGameObjectMain* v23; // [esp+B4h] [ebp-210h]
    tGameObjectMain* v24; // [esp+B8h] [ebp-20Ch]
    tGameObjectMain* v25; // [esp+BCh] [ebp-208h]
    tGameObjectMain* v26; // [esp+C0h] [ebp-204h]
    tGameObjectMain* v27; // [esp+C4h] [ebp-200h]
    tGameObjectMain* v28; // [esp+C8h] [ebp-1FCh]
    Vec3 v29; // [esp+DCh] [ebp-1E8h] BYREF
    BOOL v30; // [esp+E8h] [ebp-1DCh]
    Vec3 v31; // [esp+ECh] [ebp-1D8h]
    Vec3 v32; // [esp+F8h] [ebp-1CCh]
    Vec3 a3; // [esp+104h] [ebp-1C0h] BYREF
    tGameObjectMain* v34; // [esp+110h] [ebp-1B4h]
    Vec3 v35; // [esp+11Ch] [ebp-1A8h]
    BOOL v36; // [esp+128h] [ebp-19Ch]
    BOOL v37; // [esp+140h] [ebp-184h]
    tGameObjectMain* v38; // [esp+144h] [ebp-180h]
    void* v39; // [esp+14Ch] [ebp-178h]
    int v40; // [esp+154h] [ebp-170h]
    Vec3 v; // [esp+168h] [ebp-15Ch] BYREF
    Vec3 v43; // [esp+174h] [ebp-150h] BYREF
    Vec3 v44; // [esp+180h] [ebp-144h]
    tGameObjectMain* v45; // [esp+18Ch] [ebp-138h]
    int v46; // [esp+190h] [ebp-134h]
    int v47; // [esp+194h] [ebp-130h]
    BOOL v48; // [esp+198h] [ebp-12Ch]
    void* Src; // [esp+19Ch] [ebp-128h]
    float v50; // [esp+1A0h] [ebp-124h]
    float v51; // [esp+1A4h] [ebp-120h]
    float v52; // [esp+1A8h] [ebp-11Ch]
    Vec3 v53; // [esp+1ACh] [ebp-118h] BYREF
    tMessage v54; // [esp+1B8h] [ebp-10Ch] BYREF
    Vec3 v56; // [esp+1F0h] [ebp-D4h] BYREF
    Vec3 pos; // [esp+1FCh] [ebp-C8h] BYREF
    int v58; // [esp+208h] [ebp-BCh]
    int v59; // [esp+20Ch] [ebp-B8h]
    tGameObjectMain* v60; // [esp+210h] [ebp-B4h]
    tScriptStruct_PNJGardeBarriereEntrepot* v61; // [esp+228h] [ebp-9Ch]
    float v62; // [esp+22Ch] [ebp-98h]
    BOOL v63; // [esp+230h] [ebp-94h]
    unsigned int v64; // [esp+234h] [ebp-90h]
    int v65; // [esp+238h] [ebp-8Ch]
    tGameObjectMain* v66; // [esp+23Ch] [ebp-88h]
    int v67; // [esp+240h] [ebp-84h]
    Vec3 v68; // [esp+244h] [ebp-80h]
    float v69[10]; // [esp+250h] [ebp-74h] BYREF
    Vec3 v70; // [esp+278h] [ebp-4Ch] BYREF
    tMessage v71; // [esp+290h] [ebp-34h] BYREF

    v31.x = 1.0;
    v31.y = 0.0;
    v31.z = 0.0;
    pos.x = 1.0;
    pos.y = 0.0;
    pos.z = 0.0;
    v.x = 1.0;
    v.y = 0.0;
    v.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_PNJGardeBarriereEntrepot*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v61 = v6;
    v59 = 0;
    v62 = 0.0;
    v46 = 0;
    v64 = 0;
    v47 = 0;
    v65 = 0;
    v66 = 0;
    v48 = 0;
    v67 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Garde_Barriere_Entrepot_GBE_reflex_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_reflex_univVars_Perso_ID_actor,
            (void*)Garde_Barriere_Entrepot_GBE_reflex_univVars,
            988);
        Garde_Barriere_Entrepot_GBE_reflex_TXT_Sally_gao = AI_ul_RealResolveOneRefObject(0x7200064Eu);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_reflex_univVars_Flag_speciaux_sauve,
            (void*)Garde_Barriere_Entrepot_GBE_reflex_univVars,
            4488);
        fctvar_Garde_Barriere_Entrepot_GBE_reflex_Hovercraft_shoot = AI_ul_RealResolveOneRef(42, 0x49020714u);
        Garde_Barriere_Entrepot_GBE_reflex_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
        Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_unknown_demo = (int)AI_ul_RealResolveOneRefFunction(0x7200682Cu);
        fct_Garde_Barriere_Entrepot_GBE_reflex__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Garde_Barriere_Entrepot_GBE_reflex__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        fct_Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_mode_repousse = (int)AI_ul_RealResolveOneRefFunction(0x720026E8u);
        fct_Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_mode_choppe = (int)AI_ul_RealResolveOneRefFunction(0x72002701u);
        fct_Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_mode_attente = (int)AI_ul_RealResolveOneRefFunction(0x720026E7u);
        fct_Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_exec_trace = (int)AI_ul_RealResolveOneRefFunction(0x720027DFu);
        fct_Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_mode_flotte = (int)AI_ul_RealResolveOneRefFunction(0x720026E9u);
    }
    v47 = -1;
    v61->o_main_actor = (int)*AI_MainActor;
    v61->o_aircraft = *(uint32_t*)(Garde_Barriere_Entrepot_GBE_reflex_univVars_Perso_ID_actor + 44);
    v28 = (tGameObjectMain*)v61->o_aircraft;
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
    v26 = (tGameObjectMain*)v61->o_main_actor;
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
    if (v27 != v25)
    {
        v61->f_time_main_actor = TIM_gf_MainClock - TIM_gf_LockTime;
        v61->o_aircraft = 0;
    }
    if (!AI_EvalFunc_TIMEElapsed_C(v61->f_time_main_actor, 0.1f))
        v61->o_aircraft = 0;
    v24 = (tGameObjectMain*)v61->des_o_acteur_texte;
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
        v22 = (tGameObjectMain*)v61->des_o_acteur_texte;
        if (v22)
        {
            if ((unsigned int)v22 <= 2)
                v22 = AI_MainActor[(uint32_t)v22 - 1];
            v0 = AI_EvalFunc_OBJ_CapaTest_C(v22, 0x100);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x100);
        }
        v30 = v0;
        if (v0)
            v61->f_time_affiche_texte = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    if (AI_EvalFunc_OBJ_CapaTest_C((tGameObjectMain*)Garde_Barriere_Entrepot_GBE_reflex_TXT_Sally_gao, 0x100))
        v61->f_time_affiche_texte = TIM_gf_MainClock - TIM_gf_LockTime;
    v21 = (tGameObjectMain*)v61->o_aircraft;
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
        v19 = (tGameObjectMain*)v61->des_o_wp;
        if (v19)
        {
            if ((unsigned int)v19 <= 2)
                v19 = AI_MainActor[(uint32_t)v19 - 1];
            AI_EvalFunc_OBJGetPos_C(v19, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        v31 = a3;
        v18 = (tGameObjectMain*)v61->o_aircraft;
        if (v18)
        {
            if ((unsigned int)v18 <= 2)
                v18 = AI_MainActor[(uint32_t)v18 - 1];
            AI_EvalFunc_OBJGetPos_C(v18, &v53);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v53);
        }
        v31.z = v53.z + 2.0f;
        if (v61->des_mode)
        {
            v68.x = 0.0;
            v68.y = 0.0;
            v68.z = 1.0;
            v35.x = 0.0;
            v35.y = 0.0;
            v35.z = 1.0;
        }
        else
        {
            v44.x = 0.0;
            v44.y = 0.0;
            v44.z = 1.0;
            v32.x = 0.0;
            v32.y = 0.0;
            v32.z = 1.0;
        }
    }
    v48 = AI_EvalFunc_ARRBitTest_C((int*)(Garde_Barriere_Entrepot_GBE_reflex_univVars_Flag_speciaux_sauve + 4), 126);
    Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v61->m_msg, Src, sizeof(v61->m_msg));
    while (v61->m_msg.msgID)
    {
        if (v61->m_msg.msgID == 37 && v48)
        {
            v67 = v61->m_msg.Int1 & 7;
            v59 = v61->m_msg.Int1 & 0x60;
            v46 = v61->m_msg.Int1 & 0x18;
            v64 = (unsigned int)v61->m_msg.gao;
            v17 = (tGameObjectMain*)v64;
            if (v64)
            {
                if (v64 <= 2)
                    v17 = AI_MainActor[v64 - 1];
                v1 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v17, 32);
            }
            else
            {
                v1 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 32);
            }
            v36 = v1;
            if (v1)
            {
                v16 = (tGameObjectMain*)v64;
                if (v64)
                {
                    if (v64 <= 2)
                        v16 = AI_MainActor[v64 - 1];
                    v2 = AI_EvalFunc_AIIsModel_C(v16, (tAIModel*)fctvar_Garde_Barriere_Entrepot_GBE_reflex_Hovercraft_shoot);
                }
                else
                {
                    v2 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_Garde_Barriere_Entrepot_GBE_reflex_Hovercraft_shoot);
                }
                v37 = v2;
                if (v2)
                {
                    if (v59 && v67 >= 2)
                    {
                        if (v61->i_current_mode == 3)
                        {
                            v61->f_time_paf = TIM_gf_MainClock - TIM_gf_LockTime;
                            if (v61->f_energie_courante != 0.0)
                            {
                                v61->f_time_flotte = TIM_gf_MainClock - TIM_gf_LockTime;
                                v61->f_energie_courante = v61->f_energie_courante - v61->m_msg.f2;
                                if (v61->f_energie_courante <= 0.0 || v67 >= 5)
                                {
                                    v61->f_energie_courante = 0.0;
                                    if (v61->des_SF_inactif > 0)
                                        AI_EvalFunc_ARRBitSet_C(
                                            (int*)(Garde_Barriere_Entrepot_GBE_reflex_univVars_Flag_speciaux_sauve + 4),
                                            v61->des_SF_inactif);
                                    v47 = AI_EvalFunc_SNDRequest_C(
                                        (tGameObjectMain*)Garde_Barriere_Entrepot_GBE_reflex_SFX_explosion_gao,
                                        2,
                                        402653312);
                                    AI_EvalFunc_SNDUltraPlaySound_C(AI_gpst_CurrentGameObject, v47);
                                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v43);
                                    pos = v43;
                                    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v70);
                                    v50 = v70.x * 8.0f;
                                    v51 = v70.y * 8.0f;
                                    v52 = v70.z * 8.0f;
                                    pos.x = pos.x - v50;
                                    pos.y = pos.y - v51;
                                    pos.z = pos.z - v52;
                                    v66 = AI_EvalFunc_OBJDuplicate_C(
                                        (tGameObjectMain*)Garde_Barriere_Entrepot_GBE_reflex_SFX_explosion_gao,
                                        &pos);
                                    v54.msgID = 1;
                                    v54.Int1 = 37;
                                    v54.f2 = 7.0;
                                    v54.f1 = 0.0;
                                    v54.v1.x = 1.5;
                                    v54.v1.y = 0.0;
                                    v54.v1.z = 0.0;
                                    v15 = v66;
                                    if (v66)
                                    {
                                        if ((unsigned int)v66 <= 2)
                                            v15 = AI_MainActor[(uint32_t)v66 - 1];
                                        AI_EvalFunc_MSGSend_C(v15, &v54);
                                    }
                                    else
                                    {
                                        AI_EvalFunc_MSGSend_C(0, &v54);
                                    }
                                    AI_EvalFunc_AITrackChange_C(
                                        1,
                                        (tAIFunction*)Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_unknown_demo);
                                    AI_EvalFunc_AITrackCurStop_C();
                                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                                    return 4;
                                }
                            }
                            v58 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(AI_gpst_CurrentGameObject, 3, -1.0, 0.89999998f, 1.1f);
                            v = v61->m_msg.v1;
                            MATH_NormalizeEqualVector(&v);
                            v5 = (float)v67;
                            v14 = v5 * 5.0f;
                            v.x = v.x * v14;
                            v.y = v.y * v14;
                            v.z = v.z * v14;
                            AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v29);
                            MATH_VecAddVec((Vec3*)v69, &v29, &v);
                            v40 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, (Vec3*)v69);
                            v63 = AI_EvalFunc_TIMEElapsed_C(v61->f_time_text, 1.0);
                            if (v63 && v61->i_current_mode == 3)
                            {
                                v61->f_time_text = TIM_gf_MainClock - TIM_gf_LockTime;
                                v71.msgID = 30;
                                v71.Int1 = 39;
                                v71.Int2 = 4900;
                                v71.f2 = 1.0;
                                v71.v1.x = 0.0;
                                v71.v1.y = 0.0;
                                v71.v1.z = 0.0;
                                v71.v2.x = 0.0;
                                v71.v2.y = 0.0;
                                v71.v2.z = 0.0;
                                v71.gao = 0;
                                v71.gao2 = (tGameObjectMain*)fct_Garde_Barriere_Entrepot_GBE_reflex__PJ_Sally_gao;
                                AI_EvalFunc_MSGSend_C((tGameObjectMain*)Garde_Barriere_Entrepot_GBE_reflex_TXT_Sally_gao, &v71);
                            }
                        }
                        else if (v67 >= 5)
                        {
                            if (v61->des_SF_inactif > 0)
                                AI_EvalFunc_ARRBitSet_C(
                                    (int*)(Garde_Barriere_Entrepot_GBE_reflex_univVars_Flag_speciaux_sauve + 4),
                                    v61->des_SF_inactif);
                            v47 = AI_EvalFunc_SNDRequest_C(
                                (tGameObjectMain*)Garde_Barriere_Entrepot_GBE_reflex_SFX_explosion_gao,
                                1,
                                402653312);
                            AI_EvalFunc_SNDUltraPlaySound_C(AI_gpst_CurrentGameObject, v47);
                            v61->f_time_keep_camera = TIM_gf_MainClock - TIM_gf_LockTime;
                            v61->f_time_flotte = v61->f_time_keep_camera;
                            v61->i_flag_lightning = 0;
                            if (v61->i_flag_capture_done)
                            {
                                v61->i_flag_capture_done = 0;
                                v56.x = 0.0;
                                v56.y = 0.0;
                                v56.z = 0.0;
                                v13 = (tGameObjectMain*)v61->o_aircraft;
                                if (v13)
                                {
                                    if ((unsigned int)v13 <= 2)
                                        v13 = AI_MainActor[(uint32_t)v13 - 1];
                                    AI_EvalFunc_DYNSpeedSetVector_C(v13, &v56);
                                }
                                else
                                {
                                    AI_EvalFunc_DYNSpeedSetVector_C(0, &v56);
                                }
                                v12 = (tGameObjectMain*)v61->o_aircraft;
                                if (v12)
                                {
                                    if ((unsigned int)v12 <= 2)
                                        v12 = AI_MainActor[(uint32_t)v12 - 1];
                                    AI_EvalFunc_MSGClear_C(v12);
                                }
                                else
                                {
                                    AI_EvalFunc_MSGClear_C(0);
                                }
                                v61->m_msg.msgID = 32;
                                v61->m_msg.gao = AI_gpst_CurrentGameObject;
                                v61->m_msg.gao2 = 0;
                                v61->m_msg.Int1 = 1;
                                v61->m_msg.Int2 = 1;
                                v61->m_msg.f2 = 0.0;
                                v61->m_msg.f1 = 0.0;
                                v61->m_msg.v1.x = 0.0;
                                v61->m_msg.v1.y = 0.0;
                                v61->m_msg.v1.z = 0.0;
                                v61->m_msg.v2.x = 0.0;
                                v61->m_msg.v2.y = 0.0;
                                v61->m_msg.v2.z = 0.0;
                                AI_EvalFunc_MSGSend_C(
                                    (tGameObjectMain*)Garde_Barriere_Entrepot_GBE_reflex__PJ_Aircraft_gao,
                                    &v61->m_msg);
                            }
                            switch (v61->i_current_mode)
                            {
                            case 2:
                                v45 = AI_gpst_CurrentGameObject;
                                v11 = AI_gpst_CurrentGameObject;
                                if (AI_gpst_CurrentGameObject)
                                {
                                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                                        v11 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                                    AI_EvalFunc_AICBDel_C(
                                        AI_gpst_CurrentGameObject,
                                        v11,
                                        2,
                                        (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_mode_repousse);
                                }
                                else
                                {
                                    AI_EvalFunc_AICBDel_C(
                                        AI_gpst_CurrentGameObject,
                                        0,
                                        2,
                                        (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_mode_repousse);
                                }
                                break;
                            case 1:
                                v38 = AI_gpst_CurrentGameObject;
                                v10 = AI_gpst_CurrentGameObject;
                                if (AI_gpst_CurrentGameObject)
                                {
                                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                                        v10 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                                    AI_EvalFunc_AICBDel_C(
                                        AI_gpst_CurrentGameObject,
                                        v10,
                                        2,
                                        (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_mode_choppe);
                                }
                                else
                                {
                                    AI_EvalFunc_AICBDel_C(
                                        AI_gpst_CurrentGameObject,
                                        0,
                                        2,
                                        (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_mode_choppe);
                                }
                                break;
                            case 0:
                                v60 = AI_gpst_CurrentGameObject;
                                v9 = AI_gpst_CurrentGameObject;
                                if (AI_gpst_CurrentGameObject)
                                {
                                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                                        v9 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                                    AI_EvalFunc_AICBDel_C(
                                        AI_gpst_CurrentGameObject,
                                        v9,
                                        2,
                                        (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_mode_attente);
                                }
                                else
                                {
                                    AI_EvalFunc_AICBDel_C(
                                        AI_gpst_CurrentGameObject,
                                        0,
                                        2,
                                        (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_mode_attente);
                                }
                                break;
                            }
                            v34 = AI_gpst_CurrentGameObject;
                            v8 = AI_gpst_CurrentGameObject;
                            if (AI_gpst_CurrentGameObject)
                            {
                                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                                    v8 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                                AI_EvalFunc_AICBAdd_C(
                                    AI_gpst_CurrentGameObject,
                                    v8,
                                    2,
                                    (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_exec_trace);
                            }
                            else
                            {
                                AI_EvalFunc_AICBAdd_C(
                                    AI_gpst_CurrentGameObject,
                                    0,
                                    2,
                                    (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_exec_trace);
                            }
                            AI_EvalFunc_AITrackChange_C(
                                1,
                                (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_reflex_Garde_Barriere_Entrepot_GBE_mode_flotte);
                        }
                    }
                }
            }
        }
        v39 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v61->m_msg, v39, sizeof(v61->m_msg));
    }
    if (v61->i_snd_ID != -1)
    {
        if (v61->des_mode)
            v62 = (v61->f_dist - v61->des_f_dist_min) / v61->f_dist_range;
        else
            v62 = (v61->des_f_dist_max - v61->f_dist) / v61->f_dist_range;
        v4 = v61->f_snd_vol * v62 * 0.5f;
        SND_SetVol(v61->i_snd_ID, v4);
    }
    v7 = (tGameObjectMain*)v61->o_main_actor;
    if (v7)
    {
        if ((unsigned int)v7 <= 2)
            v7 = AI_MainActor[(uint32_t)v7 - 1];
        AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v7);
    }
    else
    {
        AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    return 1000;
}

int Garde_Barriere_Entrepot_GBE_unknown_demo()
{
    int result; // eax
    Vec3* v1; // [esp+10h] [ebp-68h]
    tGameObjectMain* v2; // [esp+20h] [ebp-58h]
    tGameObjectMain* v3; // [esp+24h] [ebp-54h]
    Vec3* v4; // [esp+2Ch] [ebp-4Ch]
    tGameObjectMain* a3; // [esp+3Ch] [ebp-3Ch]
    tGameObjectMain* v6; // [esp+40h] [ebp-38h]
    int a2; // [esp+70h] [ebp-8h]
    int a2a; // [esp+70h] [ebp-8h]

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_o_targets,
            (void*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao,
            964);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_i_targets_nb,
            (void*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao,
            2572);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_v_targets_offset,
            (void*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao,
            1368);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_o_selected_target,
            (void*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao,
            2580);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_i_selected_target_nb,
            (void*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao,
            2748);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_v_selected_target_offset,
            (void*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao,
            2624);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)2)
    {
        result = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (result == 1000)
            result = 1000;
    }
    else
    {
        v6 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v6 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            a3 = v6;
        }
        else
        {
            a3 = 0;
        }
        a2 = AI_EvalFunc_ARRObjSearch_C(
            (tGameObjectMain**)(Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_o_targets + 4),
            100,
            a3);
        if (a2 != -1)
        {
            *(uint32_t*)(Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_o_targets + 4 * a2 + 4) = *(uint32_t*)(Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_o_targets + 4 * *(uint32_t*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_i_targets_nb);
            v4 = (Vec3*)AI_EvalVarArray_C3(
                (char*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_v_targets_offset,
                *(uint32_t*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_i_targets_nb - 1);
            *(Vec3*)AI_EvalVarArray_C3((char*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_v_targets_offset, a2) = *v4;
            *(uint32_t*)(Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_o_targets
                + 4 * *(uint32_t*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_i_targets_nb) = 0;
            *(uint32_t*)AI_EvalVarArray_C3(
                (char*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_v_targets_offset,
                *(uint32_t*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_i_targets_nb - 1) = 0;
            *(uint32_t*)(AI_EvalVarArray_C3(
                (char*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_v_targets_offset,
                *(uint32_t*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_i_targets_nb - 1)
                + 4) = 0;
            *(uint32_t*)(AI_EvalVarArray_C3(
                (char*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_v_targets_offset,
                *(uint32_t*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_i_targets_nb - 1)
                + 8) = 0;
            --* (uint32_t*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_i_targets_nb;
        }
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
        a2a = AI_EvalFunc_ARRObjSearch_C(
            (tGameObjectMain**)(Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_o_selected_target + 4),
            10,
            v2);
        if (a2a != -1)
        {
            *(uint32_t*)(Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_o_selected_target + 4 * a2a + 4) = *(uint32_t*)(Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_o_selected_target + 4 * *(uint32_t*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_i_selected_target_nb);
            v1 = (Vec3*)AI_EvalVarArray_C3(
                (char*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_v_selected_target_offset,
                *(uint32_t*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_i_selected_target_nb - 1);
            *(Vec3*)AI_EvalVarArray_C3(
                (char*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_v_selected_target_offset,
                a2a) = *v1;
            *(uint32_t*)(Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_o_selected_target
                + 4 * *(uint32_t*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_i_selected_target_nb) = 0;
            *(uint32_t*)AI_EvalVarArray_C3(
                (char*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_v_selected_target_offset,
                *(uint32_t*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_i_selected_target_nb - 1) = 0;
            *(uint32_t*)(AI_EvalVarArray_C3(
                (char*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_v_selected_target_offset,
                *(uint32_t*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_i_selected_target_nb - 1)
                + 4) = 0;
            *(uint32_t*)(AI_EvalVarArray_C3(
                (char*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_v_selected_target_offset,
                *(uint32_t*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_i_selected_target_nb - 1)
                + 8) = 0;
            --* (uint32_t*)Garde_Barriere_Entrepot_GBE_unknown_demo_GST_Global_gao_i_selected_target_nb;
        }
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        *AI_gppst_CurrentJumpNode = (tAINode*)2;
        TIME_Wait_C(2.0);
        result = 0;
    }
    return result;
}

int Garde_Barriere_Entrepot_GBE_mode_attente()
{
    int result; // eax
    tGameObjectMain* v1; // eax
    double v2; // st7
    int v3; // eax
    double v4; // st7
    int v5; // eax
    double v6; // st7
    float v7; // [esp+4h] [ebp-6F4h]
    float v8; // [esp+4h] [ebp-6F4h]
    float v9; // [esp+4h] [ebp-6F4h]
    float v10; // [esp+Ch] [ebp-6ECh]
    float v11; // [esp+10h] [ebp-6E8h]
    float v12; // [esp+14h] [ebp-6E4h]
    float v13; // [esp+18h] [ebp-6E0h]
    tScriptStruct_PNJGardeBarriereEntrepot* v14; // [esp+1Ch] [ebp-6DCh]
    Vec3* v15; // [esp+3Ch] [ebp-6BCh]
    Vec3* v16; // [esp+40h] [ebp-6B8h]
    Vec3* v17; // [esp+44h] [ebp-6B4h]
    float v18; // [esp+4Ch] [ebp-6ACh]
    Vec3* v19; // [esp+58h] [ebp-6A0h]
    float v20; // [esp+64h] [ebp-694h]
    float v21; // [esp+68h] [ebp-690h]
    float v22; // [esp+6Ch] [ebp-68Ch]
    Vec3* v23; // [esp+70h] [ebp-688h]
    float v24; // [esp+80h] [ebp-678h]
    tGameObjectMain* v25; // [esp+88h] [ebp-670h]
    tGameObjectMain* v26; // [esp+90h] [ebp-668h]
    tGameObjectMain* v27; // [esp+98h] [ebp-660h]
    tGameObjectMain* v28; // [esp+A8h] [ebp-650h]
    tGameObjectMain* v29; // [esp+B0h] [ebp-648h]
    tGameObjectMain* v30; // [esp+C8h] [ebp-630h]
    float v31; // [esp+CCh] [ebp-62Ch]
    float v32; // [esp+D0h] [ebp-628h]
    float v33; // [esp+D4h] [ebp-624h]
    tGameObjectMain* v34; // [esp+DCh] [ebp-61Ch]
    tGameObjectMain* v35; // [esp+E4h] [ebp-614h]
    tGameObjectMain* v36; // [esp+ECh] [ebp-60Ch]
    float v37; // [esp+F0h] [ebp-608h]
    float v38; // [esp+F4h] [ebp-604h]
    float v39; // [esp+F8h] [ebp-600h]
    Vec3* v40; // [esp+FCh] [ebp-5FCh]
    float v41; // [esp+10Ch] [ebp-5ECh]
    tGameObjectMain* v42; // [esp+114h] [ebp-5E4h]
    tGameObjectMain* v43; // [esp+11Ch] [ebp-5DCh]
    tGameObjectMain* v44; // [esp+124h] [ebp-5D4h]
    tGameObjectMain* v45; // [esp+12Ch] [ebp-5CCh]
    tGameObjectMain* v46; // [esp+134h] [ebp-5C4h]
    tGameObjectMain* v47; // [esp+144h] [ebp-5B4h]
    float v48; // [esp+148h] [ebp-5B0h]
    float v49; // [esp+14Ch] [ebp-5ACh]
    float v50; // [esp+150h] [ebp-5A8h]
    tGameObjectMain* v51; // [esp+158h] [ebp-5A0h]
    tGameObjectMain* v52; // [esp+160h] [ebp-598h]
    tGameObjectMain* v53; // [esp+168h] [ebp-590h]
    float v54; // [esp+17Ch] [ebp-57Ch]
    tGameObjectMain* v55; // [esp+190h] [ebp-568h]
    tGameObjectMain* v56; // [esp+1A0h] [ebp-558h]
    tGameObjectMain* v57; // [esp+1A8h] [ebp-550h]
    float v58; // [esp+1ACh] [ebp-54Ch]
    tGameObjectMain* v59; // [esp+1B4h] [ebp-544h]
    tGameObjectMain* v60; // [esp+1CCh] [ebp-52Ch]
    tGameObjectMain* v61; // [esp+1D0h] [ebp-528h]
    tGameObjectMain* v62; // [esp+1D8h] [ebp-520h]
    tGameObjectMain* v63; // [esp+1DCh] [ebp-51Ch]
    Vec3 v64; // [esp+1E0h] [ebp-518h] BYREF
    float v65; // [esp+1ECh] [ebp-50Ch]
    float v66; // [esp+1F0h] [ebp-508h]
    float v67; // [esp+1F4h] [ebp-504h]
    Vec3 v68; // [esp+210h] [ebp-4E8h] BYREF
    Vec3 v69; // [esp+21Ch] [ebp-4DCh] BYREF
    Vec3 v70; // [esp+228h] [ebp-4D0h] BYREF
    Vec3 v71; // [esp+234h] [ebp-4C4h] BYREF
    float v72; // [esp+24Ch] [ebp-4ACh]
    float v73; // [esp+250h] [ebp-4A8h]
    float v74; // [esp+254h] [ebp-4A4h]
    float v75; // [esp+258h] [ebp-4A0h]
    float a4; // [esp+25Ch] [ebp-49Ch]
    float v77; // [esp+260h] [ebp-498h]
    int v78; // [esp+264h] [ebp-494h]
    Vec3 dest; // [esp+268h] [ebp-490h] BYREF
    Vec3 v80; // [esp+274h] [ebp-484h] BYREF
    int v81; // [esp+280h] [ebp-478h]
    int v82; // [esp+284h] [ebp-474h]
    float v83; // [esp+288h] [ebp-470h]
    Vec3 v84; // [esp+28Ch] [ebp-46Ch] BYREF
    float v85; // [esp+298h] [ebp-460h]
    float v86; // [esp+29Ch] [ebp-45Ch]
    tGameObjectMain* v87; // [esp+2A0h] [ebp-458h]
    float v88; // [esp+2A4h] [ebp-454h]
    float v89; // [esp+2ACh] [ebp-44Ch]
    float v91; // [esp+2B4h] [ebp-444h]
    tMessage Src; // [esp+2B8h] [ebp-440h] BYREF
    Vec3 v93; // [esp+2ECh] [ebp-40Ch] BYREF
    Vec3 v94; // [esp+2F8h] [ebp-400h] BYREF
    float v95; // [esp+304h] [ebp-3F4h]
    Vec3 v96; // [esp+30Ch] [ebp-3ECh] BYREF
    Vec3 v97; // [esp+318h] [ebp-3E0h] BYREF
    Vec3 v98; // [esp+324h] [ebp-3D4h] BYREF
    Vec3 v99; // [esp+330h] [ebp-3C8h] BYREF
    float v100; // [esp+33Ch] [ebp-3BCh]
    float v101; // [esp+340h] [ebp-3B8h]
    float v102; // [esp+344h] [ebp-3B4h]
    Vec3 v103; // [esp+348h] [ebp-3B0h] BYREF
    Vec3 v104; // [esp+360h] [ebp-398h] BYREF
    float v105; // [esp+36Ch] [ebp-38Ch]
    float v106; // [esp+370h] [ebp-388h]
    float v107; // [esp+374h] [ebp-384h]
    Vec3 v108; // [esp+378h] [ebp-380h] BYREF
    float v109; // [esp+390h] [ebp-368h]
    float v110; // [esp+394h] [ebp-364h]
    float v111; // [esp+398h] [ebp-360h]
    int v112; // [esp+39Ch] [ebp-35Ch]
    float v113; // [esp+3A0h] [ebp-358h]
    float v114; // [esp+3A4h] [ebp-354h]
    float v115; // [esp+3A8h] [ebp-350h]
    float v116; // [esp+3B0h] [ebp-348h]
    Vec3 v117; // [esp+3C0h] [ebp-338h] BYREF
    float v118; // [esp+3CCh] [ebp-32Ch]
    float v119; // [esp+3D0h] [ebp-328h]
    float v120; // [esp+3D4h] [ebp-324h]
    Vec3 v121; // [esp+3E4h] [ebp-314h] BYREF
    Vec3 v122; // [esp+3F0h] [ebp-308h] BYREF
    float v123; // [esp+400h] [ebp-2F8h]
    Vec3 a2; // [esp+404h] [ebp-2F4h] BYREF
    Vec3 v125; // [esp+410h] [ebp-2E8h] BYREF
    Vec3 v127; // [esp+428h] [ebp-2D0h] BYREF
    Vec3 v128; // [esp+434h] [ebp-2C4h] BYREF
    Vec3 a3; // [esp+440h] [ebp-2B8h] BYREF
    Vec3 v130; // [esp+44Ch] [ebp-2ACh] BYREF
    Vec3 v131; // [esp+458h] [ebp-2A0h] BYREF
    Vec3 v132; // [esp+464h] [ebp-294h] BYREF
    float v133; // [esp+474h] [ebp-284h]
    float v134; // [esp+478h] [ebp-280h]
    float v135; // [esp+47Ch] [ebp-27Ch]
    Vec3 v136; // [esp+484h] [ebp-274h] BYREF
    Vec3 v137; // [esp+490h] [ebp-268h] BYREF
    Vec3 v138; // [esp+49Ch] [ebp-25Ch] BYREF
    Vec3 v139; // [esp+4A8h] [ebp-250h] BYREF
    float v140; // [esp+4B4h] [ebp-244h]
    float v141; // [esp+4B8h] [ebp-240h]
    float v142; // [esp+4C4h] [ebp-234h]
    float v143; // [esp+4C8h] [ebp-230h]
    Vec3 v144; // [esp+4D4h] [ebp-224h] BYREF
    Vec3 v145; // [esp+4E0h] [ebp-218h] BYREF
    Vec3 v146; // [esp+4ECh] [ebp-20Ch] BYREF
    float v147; // [esp+504h] [ebp-1F4h]
    float v148; // [esp+508h] [ebp-1F0h]
    float v149; // [esp+50Ch] [ebp-1ECh]
    Vec3 v150; // [esp+510h] [ebp-1E8h] BYREF
    Vec3 v151; // [esp+51Ch] [ebp-1DCh] BYREF
    float v152; // [esp+540h] [ebp-1B8h]
    float v153; // [esp+544h] [ebp-1B4h]
    int v154; // [esp+548h] [ebp-1B0h]
    int v155; // [esp+54Ch] [ebp-1ACh]
    tGameObjectMain* v156; // [esp+568h] [ebp-190h]
    tGameObjectMain* v157; // [esp+56Ch] [ebp-18Ch]
    float v158; // [esp+570h] [ebp-188h]
    float v159; // [esp+574h] [ebp-184h]
    Vec3 v160; // [esp+578h] [ebp-180h] BYREF
    Vec3 v161; // [esp+584h] [ebp-174h] BYREF
    Vec3 v162; // [esp+59Ch] [ebp-15Ch] BYREF
    Vec3 v163; // [esp+5A8h] [ebp-150h] BYREF
    int v165; // [esp+5B8h] [ebp-140h]
    float v166; // [esp+5BCh] [ebp-13Ch]
    float v167; // [esp+5C0h] [ebp-138h]
    float v169; // [esp+5C8h] [ebp-130h]
    float v170; // [esp+5CCh] [ebp-12Ch]
    Vec3 v171; // [esp+5D0h] [ebp-128h] BYREF
    Vec3 v172; // [esp+5DCh] [ebp-11Ch] BYREF
    Vec3 v173; // [esp+5F4h] [ebp-104h] BYREF
    Vec3 v174; // [esp+600h] [ebp-F8h] BYREF
    Vec3 v175; // [esp+60Ch] [ebp-ECh] BYREF
    Vec3 v176; // [esp+618h] [ebp-E0h] BYREF
    int v177; // [esp+634h] [ebp-C4h]
    int v178; // [esp+638h] [ebp-C0h]
    float v179; // [esp+63Ch] [ebp-BCh]
    float v180; // [esp+640h] [ebp-B8h]
    float v181; // [esp+644h] [ebp-B4h]
    Vec3 v182; // [esp+648h] [ebp-B0h] BYREF
    int v183; // [esp+654h] [ebp-A4h]
    float v184; // [esp+658h] [ebp-A0h]
    float v185; // [esp+65Ch] [ebp-9Ch]
    tScriptStruct_PNJGardeBarriereEntrepot* v186; // [esp+660h] [ebp-98h]
    float v188; // [esp+668h] [ebp-90h]
    float v189; // [esp+66Ch] [ebp-8Ch]
    float v190; // [esp+670h] [ebp-88h]
    Vec3 v191; // [esp+674h] [ebp-84h] BYREF
    Vec3 v192; // [esp+680h] [ebp-78h] BYREF
    Vec3 v193; // [esp+68Ch] [ebp-6Ch] BYREF
    float v194; // [esp+698h] [ebp-60h]
    float v195; // [esp+6A0h] [ebp-58h]
    float v197; // [esp+6ACh] [ebp-4Ch]
    float v198; // [esp+6B0h] [ebp-48h]
    float v199; // [esp+6B4h] [ebp-44h]
    Vec3 v200; // [esp+6B8h] [ebp-40h]
    Vec3 a5; // [esp+6C4h] [ebp-34h] BYREF
    Vec3 v202; // [esp+6D0h] [ebp-28h] BYREF
    Vec3 v203; // [esp+6DCh] [ebp-1Ch] BYREF
    float v204; // [esp+6E8h] [ebp-10h]
    tGameObjectMain* v205; // [esp+6ECh] [ebp-Ch]
    float v206; // [esp+6F0h] [ebp-8h]
    float v207; // [esp+6F4h] [ebp-4h]

    v117.x = 1.0;
    v117.y = 0.0;
    v117.z = 0.0;
    v145.x = 1.0;
    v145.y = 0.0;
    v145.z = 0.0;
    v160.x = 1.0;
    v160.y = 0.0;
    v160.z = 0.0;
    a2.x = 1.0;
    a2.y = 0.0;
    a2.z = 0.0;
    v139.x = 1.0;
    v139.y = 0.0;
    v139.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v14 = (tScriptStruct_PNJGardeBarriereEntrepot*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v14 = 0;
    v186 = v14;
    v165 = 0;
    v72 = 0.0;
    v154 = 0;
    a4 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Garde_Barriere_Entrepot_GBE_mode_attente__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        Garde_Barriere_Entrepot_GBE_mode_attente_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_mode_attente_univVars_Flag_speciaux_sauve,
            (void*)Garde_Barriere_Entrepot_GBE_mode_attente_univVars,
            4488);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_mode_attente_univVars_Perso_ID_passengers_list,
            (void*)Garde_Barriere_Entrepot_GBE_mode_attente_univVars,
            936);
        fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_mode_choppe = (int)AI_ul_RealResolveOneRefFunction(0x72002701u);
        fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_mode_attente = (int)AI_ul_RealResolveOneRefFunction(0x720026E7u);
        Garde_Barriere_Entrepot_GBE_mode_attente__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Garde_Barriere_Entrepot_GBE_mode_attente_TXT_Sally_gao = AI_ul_RealResolveOneRefObject(0x7200064Eu);
        fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_exec_camera = (int)AI_ul_RealResolveOneRefFunction(0x72002703u);
        fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning = (int)AI_ul_RealResolveOneRefFunction(0x72004A10u);
    }
    if (v186->i_current_mode)
    {
        v186->i_current_mode = 0;
        if (v186->i_flag_capture_done)
        {
            v186->i_flag_capture_done = 0;
            v186->m_msg.msgID = 32;
            v186->m_msg.gao = AI_gpst_CurrentGameObject;
            v186->m_msg.gao2 = 0;
            v186->m_msg.Int1 = 1;
            v186->m_msg.Int2 = 1;
            v186->m_msg.f2 = 0.0;
            v186->m_msg.f1 = 0.0;
            v186->m_msg.v1.x = 0.0;
            v186->m_msg.v1.y = 0.0;
            v186->m_msg.v1.z = 0.0;
            v186->m_msg.v2.x = 0.0;
            v186->m_msg.v2.y = 0.0;
            v186->m_msg.v2.z = 0.0;
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)Garde_Barriere_Entrepot_GBE_mode_attente__PJ_Aircraft_gao, &v186->m_msg);
        }
        AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0xFFu, 0);
        v186->f_time_change_mode = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    v63 = (tGameObjectMain*)v186->o_aircraft;
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
    if (!v62)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 4;
    }
    if (v186->i_flag_helper)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v186->f_time_affiche_texte, 5.0))
        {
            v186->i_flag_helper = 0;
            v154 = AI_EvalFunc_ARRBitTest_C(
                (int*)(Garde_Barriere_Entrepot_GBE_mode_attente_univVars_Flag_speciaux_sauve + 4),
                126);
            if (!v154)
            {
                if (AI_EvalFunc_TIMEElapsed_C(v186->f_time_helper, 600.0))
                {
                    v61 = (tGameObjectMain*)v186->des_o_acteur_texte;
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
                    if (v60
                        && (*(uint32_t*)(Garde_Barriere_Entrepot_GBE_mode_attente_univVars_Perso_ID_passengers_list + 44) & 1) != 0)
                    {
                        v186->f_time_helper = TIM_gf_MainClock - TIM_gf_LockTime;
                        if (AI_EvalFunc_ARRBitTest_C(
                            (int*)(Garde_Barriere_Entrepot_GBE_mode_attente_univVars_Flag_speciaux_sauve + 4),
                            2253))
                        {
                            v154 = 44;
                            AI_EvalFunc_ARRBitSet_C(
                                (int*)(Garde_Barriere_Entrepot_GBE_mode_attente_univVars_Flag_speciaux_sauve + 4),
                                3779);
                        }
                        else if (AI_EvalFunc_ARRBitTest_C(
                            (int*)(Garde_Barriere_Entrepot_GBE_mode_attente_univVars_Flag_speciaux_sauve + 4),
                            2138))
                        {
                            v154 = 43;
                            AI_EvalFunc_ARRBitSet_C(
                                (int*)(Garde_Barriere_Entrepot_GBE_mode_attente_univVars_Flag_speciaux_sauve + 4),
                                3779);
                        }
                        else
                        {
                            v154 = 42;
                        }
                        Src.msgID = 30;
                        Src.Int1 = v154;
                        Src.Int2 = 100;
                        Src.f2 = 3.0;
                        Src.v1.x = 0.0;
                        Src.v1.y = 0.0;
                        Src.v1.z = 0.0;
                        Src.v2.x = 0.0;
                        Src.v2.y = 0.0;
                        Src.v2.z = 0.0;
                        Src.gao = 0;
                        Src.gao2 = 0;
                        v59 = (tGameObjectMain*)v186->des_o_acteur_texte;
                        if (v59)
                        {
                            if ((unsigned int)v59 <= 2)
                                v59 = AI_MainActor[(uint32_t)v59 - 1];
                            AI_EvalFunc_MSGSend_C(v59, &Src);
                        }
                        else
                        {
                            AI_EvalFunc_MSGSend_C(0, &Src);
                        }
                    }
                }
            }
        }
    }
    v101 = TIM_gf_dt;
    v58 = TIM_gf_dt;
    if (TIM_gf_dt < 0.0)
        v58 = 0.0;
    if (v58 > 1.0)
        v58 = 1.0;
    v206 = 8.0f * v58 + v186->f_choppe_blend_speed * (1.0f - v58);
    v186->f_choppe_blend_speed = v206;
    v57 = (tGameObjectMain*)v186->o_aircraft;
    if (v57)
    {
        if ((unsigned int)v57 <= 2)
            v57 = AI_MainActor[(uint32_t)v57 - 1];
        v1 = AI_EvalFunc_ANICanalGaoGet_C(v57, 0);
    }
    else
    {
        v1 = AI_EvalFunc_ANICanalGaoGet_C(0, 0);
    }
    v186->o_aircraft_visuel = (int)v1;
    v56 = (tGameObjectMain*)v186->o_aircraft;
    if (v56)
    {
        if ((unsigned int)v56 <= 2)
            v56 = AI_MainActor[(uint32_t)v56 - 1];
        AI_EvalFunc_OBJGetPos_C(v56, &v121);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v121);
    }
    v145 = v121;
    v55 = (tGameObjectMain*)v186->des_o_wp;
    if (v55)
    {
        if ((unsigned int)v55 <= 2)
            v55 = AI_MainActor[(uint32_t)v55 - 1];
        AI_EvalFunc_OBJGetPos_C(v55, &v132);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v132);
    }
    MATH_SubstractVec(&dest, &v145, &v132);
    a2.x = dest.x;
    a2.y = dest.y;
    a2.z = 0.0;
    v2 = MATH_VecLen(&a2);
    v186->f_dist = (float)v2;
    v54 = 1.0f / v186->f_dist;
    a2.x = a2.x * v54;
    a2.y = a2.y * v54;
    a2.z = a2.z * v54;
    v165 = 1;
    if (v186->i_flag_stay_in_cone)
    {
        v170 = v186->v_cone_axe.z * a2.z + v186->v_cone_axe.y * a2.y + a2.x * v186->v_cone_axe.x;
        if (v186->f_cone_cos > (double)v170)
            v165 = 0;
        AI_EvalFunc_MATHVecInCone_C(&a2, &v186->v_cone_axe, v186->f_cone_angle, 0, &v150);
        a2 = v150;
    }
    if (v186->des_mode)
    {
        if (v165 && v186->des_f_dist_max < (double)v186->f_dist)
        {
            v205 = AI_gpst_CurrentGameObject;
            v53 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v53 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v53,
                    2,
                    (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_mode_choppe);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_mode_choppe);
            }
            v157 = AI_gpst_CurrentGameObject;
            v52 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v52 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v52,
                    2,
                    (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_mode_attente);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_mode_attente);
            }
        }
        if (v165 && v186->des_f_dist_min < (double)v186->f_dist)
        {
            v186->i_flag_lightning = 1;
            if (v186->i_snd_ID == -1)
            {
                v3 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 1, 0x18000000);
                v186->i_snd_ID = v3;
                AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v186->i_snd_ID);
                v4 = SND_f_GetVol(v186->i_snd_ID);
                v186->f_snd_vol = (float)v4;
                SND_SetVol(v186->i_snd_ID, 0.0);
                v186->i_snd_play = 0;
            }
            v51 = (tGameObjectMain*)v186->o_aircraft;
            if (v51)
            {
                if ((unsigned int)v51 <= 2)
                    v51 = AI_MainActor[(uint32_t)v51 - 1];
                AI_EvalFunc_OBJGetPos_C(v51, &v186->v_my_pos);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v186->v_my_pos);
            }
            v186->v_my_pos.z = v186->v_my_pos.z + 20.0f;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v94);
            v195 = TIM_gf_dt;
            v50 = v186->f_choppe_blend_speed * TIM_gf_dt;
            if (v50 < 0.0)
                v50 = 0.0;
            if (v50 > 1.0)
                v50 = 1.0;
            v142 = v186->v_my_pos.x * v50 + (1.0f - v50) * v94.x;
            v186->v_my_pos.x = v142;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v182);
            v141 = TIM_gf_dt;
            v49 = v186->f_choppe_blend_speed * TIM_gf_dt;
            if (v49 < 0.0)
                v49 = 0.0;
            if (v49 > 1.0)
                v49 = 1.0;
            v152 = v186->v_my_pos.y * v49 + (1.0f - v49) * v182.y;
            v186->v_my_pos.y = v152;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v192);
            v113 = TIM_gf_dt;
            v48 = 4.0f * TIM_gf_dt;
            if (v48 < 0.0)
                v48 = 0.0;
            if (v48 > 1.0)
                v48 = 1.0;
            v204 = v186->v_my_pos.z * v48 + (1.0f - v48) * v192.z;
            v186->v_my_pos.z = v204;
            if (AI_EvalFunc_TIMEElapsed_C(v186->f_time_text, 8.0))
            {
                v186->f_time_text = TIM_gf_MainClock - TIM_gf_LockTime;
                Src.msgID = 30;
                Src.Int1 = 37;
                Src.Int2 = 4900;
                Src.f2 = 2.0;
                Src.v1.x = 0.0;
                Src.v1.y = 0.0;
                Src.v1.z = 0.0;
                Src.v2.x = 0.0;
                Src.v2.y = 0.0;
                Src.v2.z = 0.0;
                Src.gao = 0;
                Src.gao2 = (tGameObjectMain*)Garde_Barriere_Entrepot_GBE_mode_attente__PJ_Sally_gao;
                AI_EvalFunc_MSGSend_C((tGameObjectMain*)Garde_Barriere_Entrepot_GBE_mode_attente_TXT_Sally_gao, &Src);
            }
            a4 = (v186->f_dist - v186->des_f_dist_min) / v186->f_dist_range;
            v47 = (tGameObjectMain*)v186->o_aircraft;
            if (v47)
            {
                if ((unsigned int)v47 <= 2)
                    v47 = AI_MainActor[(uint32_t)v47 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v47, &v191);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v191);
            }
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v144);
            v72 = v144.z * v191.z + v144.y * v191.y + v191.x * v144.x;
            v166 = AI_EvalFunc_MTHFloatSign_C(v72);
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v104);
            a3.x = v104.x * v166;
            a3.y = v104.y * v166;
            a3.z = v104.z * v166;
            AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
            v160 = a5;
            v46 = (tGameObjectMain*)v186->o_aircraft;
            if (v46)
            {
                if ((unsigned int)v46 <= 2)
                    v46 = AI_MainActor[(uint32_t)v46 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v46, &v176);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v176);
            }
            v102 = AI_EvalFunc_MTHFloatSign_C(v72);
            v174.x = 0.0;
            v174.y = 0.0;
            v174.z = 1.0;
            AI_EvalFunc_MATHVecCrossProduct_C(&v160, &v174, &v203);
            v109 = v203.x * v102;
            v110 = v203.y * v102;
            v111 = v203.z * v102;
            v140 = v111 * v176.z + v110 * v176.y + v176.x * v109;
            if (v140 > 0.0)
            {
                v45 = (tGameObjectMain*)v186->o_aircraft;
                if (v45)
                {
                    if ((unsigned int)v45 <= 2)
                        v45 = AI_MainActor[(uint32_t)v45 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v45, &v103);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v103);
                }
                v133 = TIM_gf_dt;
                v7 = 8.0f * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlend_C(&v103, &v160, v7, &v64);
                v173.x = 0.0;
                v173.y = 0.0;
                v173.z = 1.0;
                v44 = (tGameObjectMain*)v186->o_aircraft;
                if (v44)
                {
                    if ((unsigned int)v44 <= 2)
                        v44 = AI_MainActor[(uint32_t)v44 - 1];
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(v44, &v64, &v173);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v64, &v173);
                }
            }
            v78 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_exec_camera);
            if (v78 != 1000)
                return v78;
        }
        else
        {
            v186->i_flag_lightning = 0;
            if (v186->i_snd_ID != -1)
            {
                SND_StopRq(v186->i_snd_ID);
                v186->i_snd_ID = -1;
            }
            v43 = (tGameObjectMain*)v186->des_o_wp;
            if (v43)
            {
                if ((unsigned int)v43 <= 2)
                    v43 = AI_MainActor[(uint32_t)v43 - 1];
                AI_EvalFunc_OBJGetPos_C(v43, &v186->v_my_pos);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v186->v_my_pos);
            }
            v42 = (tGameObjectMain*)v186->o_aircraft;
            if (v42)
            {
                if ((unsigned int)v42 <= 2)
                    v42 = AI_MainActor[(uint32_t)v42 - 1];
                AI_EvalFunc_OBJGetPos_C(v42, &v99);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v99);
            }
            v41 = v99.z + 20.0f;
            if (v186->v_my_pos.z < (double)v41)
                v13 = v99.z + 20.0f;
            else
                v13 = v186->v_my_pos.z;
            v186->v_my_pos.z = v13;
            if (v186->f_dist < (double)v186->des_f_dist_min)
                v12 = v186->des_f_dist_min;
            else
                v12 = v186->f_dist;
            v77 = v12;
            v105 = a2.x * v12;
            v106 = a2.y * v12;
            v107 = a2.z * v12;
            v40 = &v186->v_my_pos;
            v186->v_my_pos.x = v105 + v186->v_my_pos.x;
            v40->y = v106 + v40->y;
            v40->z = v107 + v40->z;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v68);
            v194 = TIM_gf_dt;
            v39 = v186->f_choppe_blend_speed * TIM_gf_dt;
            if (v39 < 0.0)
                v39 = 0.0;
            if (v39 > 1.0)
                v39 = 1.0;
            v86 = v186->v_my_pos.x * v39 + (1.0f - v39) * v68.x;
            v186->v_my_pos.x = v86;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v71);
            v134 = TIM_gf_dt;
            v38 = v186->f_choppe_blend_speed * TIM_gf_dt;
            if (v38 < 0.0)
                v38 = 0.0;
            if (v38 > 1.0)
                v38 = 1.0;
            v73 = v186->v_my_pos.y * v38 + (1.0f - v38) * v71.y;
            v186->v_my_pos.y = v73;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v163);
            v135 = TIM_gf_dt;
            v37 = 0.5f * TIM_gf_dt;
            if (v37 < 0.0)
                v37 = 0.0;
            if (v37 > 1.0)
                v37 = 1.0;
            v158 = v186->v_my_pos.z * v37 + (1.0f - v37) * v163.z;
            v186->v_my_pos.z = v158;
            if (!AI_EvalFunc_TIMEElapsed_C(v186->f_time_keep_camera, 3.0))
            {
                v178 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_exec_camera);
                if (v178 != 1000)
                    return v178;
            }
        }
    }
    else
    {
        if (v165 && v186->des_f_dist_min >= (double)v186->f_dist)
        {
            v87 = AI_gpst_CurrentGameObject;
            v36 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v36 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v36,
                    2,
                    (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_mode_choppe);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_mode_choppe);
            }
            v156 = AI_gpst_CurrentGameObject;
            v35 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v35 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v35,
                    2,
                    (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_mode_attente);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_mode_attente);
            }
        }
        if (v165 && v186->des_f_dist_max >= (double)v186->f_dist)
        {
            v186->i_flag_lightning = 1;
            if (v186->i_snd_ID == -1)
            {
                v5 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 1, 0x18000000);
                v186->i_snd_ID = v5;
                AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v186->i_snd_ID);
                v6 = SND_f_GetVol(v186->i_snd_ID);
                v186->f_snd_vol = (float)v6;
                SND_SetVol(v186->i_snd_ID, 0.0);
                v186->i_snd_play = 0;
            }
            v34 = (tGameObjectMain*)v186->o_aircraft;
            if (v34)
            {
                if ((unsigned int)v34 <= 2)
                    v34 = AI_MainActor[(uint32_t)v34 - 1];
                AI_EvalFunc_OBJGetPos_C(v34, &v186->v_my_pos);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v186->v_my_pos);
            }
            v186->v_my_pos.z = v186->v_my_pos.z + 20.0f;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v93);
            v74 = TIM_gf_dt;
            v33 = v186->f_choppe_blend_speed * TIM_gf_dt;
            if (v33 < 0.0)
                v33 = 0.0;
            if (v33 > 1.0)
                v33 = 1.0;
            v75 = v186->v_my_pos.x * v33 + (1.0f - v33) * v93.x;
            v186->v_my_pos.x = v75;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v202);
            v91 = TIM_gf_dt;
            v32 = v186->f_choppe_blend_speed * TIM_gf_dt;
            if (v32 < 0.0)
                v32 = 0.0;
            if (v32 > 1.0)
                v32 = 1.0;
            v159 = v186->v_my_pos.y * v32 + (1.0f - v32) * v202.y;
            v186->v_my_pos.y = v159;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v161);
            v85 = TIM_gf_dt;
            v31 = 4.0f * TIM_gf_dt;
            if (v31 < 0.0)
                v31 = 0.0;
            if (v31 > 1.0)
                v31 = 1.0;
            v95 = v186->v_my_pos.z * v31 + (1.0f - v31) * v161.z;
            v186->v_my_pos.z = v95;
            if (AI_EvalFunc_TIMEElapsed_C(v186->f_time_text, 8.0))
            {
                v186->f_time_text = TIM_gf_MainClock - TIM_gf_LockTime;
                Src.msgID = 30;
                Src.Int1 = 37;
                Src.Int2 = 4900;
                Src.f2 = 2.0;
                Src.v1.x = 0.0;
                Src.v1.y = 0.0;
                Src.v1.z = 0.0;
                Src.v2.x = 0.0;
                Src.v2.y = 0.0;
                Src.v2.z = 0.0;
                Src.gao = 0;
                Src.gao2 = (tGameObjectMain*)Garde_Barriere_Entrepot_GBE_mode_attente__PJ_Sally_gao;
                AI_EvalFunc_MSGSend_C((tGameObjectMain*)Garde_Barriere_Entrepot_GBE_mode_attente_TXT_Sally_gao, &Src);
            }
            a4 = (v186->des_f_dist_max - v186->f_dist) / v186->f_dist_range;
            v30 = (tGameObjectMain*)v186->o_aircraft;
            if (v30)
            {
                if ((unsigned int)v30 <= 2)
                    v30 = AI_MainActor[(uint32_t)v30 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v30, &v138);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v138);
            }
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v127);
            v200.x = -v127.x;
            v200.y = -v127.y;
            v200.z = -v127.z;
            v72 = -v127.z * v138.z + -v127.y * v138.y + v138.x * -v127.x;
            v100 = AI_EvalFunc_MTHFloatSign_C(v72);
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v193);
            v108.x = v193.x * v100;
            v108.y = v193.y * v100;
            v108.z = v193.z * v100;
            AI_EvalFunc_MATHVecBlendRotate_C(&a2, &v108, a4, &v125);
            v160 = v125;
            v29 = (tGameObjectMain*)v186->o_aircraft;
            if (v29)
            {
                if ((unsigned int)v29 <= 2)
                    v29 = AI_MainActor[(uint32_t)v29 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v29, &v96);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v96);
            }
            v167 = AI_EvalFunc_MTHFloatSign_C(v72);
            v131.x = 0.0;
            v131.y = 0.0;
            v131.z = 1.0;
            AI_EvalFunc_MATHVecCrossProduct_C(&v160, &v131, &v97);
            v197 = v97.x * v167;
            v198 = v97.y * v167;
            v199 = v97.z * v167;
            v169 = v199 * v96.z + v198 * v96.y + v96.x * v197;
            if (v169 < 0.0)
            {
                v28 = (tGameObjectMain*)v186->o_aircraft;
                if (v28)
                {
                    if ((unsigned int)v28 <= 2)
                        v28 = AI_MainActor[(uint32_t)v28 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v28, &v130);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v130);
                }
                v98.x = -v160.x;
                v98.y = -v160.y;
                v98.z = -v160.z;
                v207 = TIM_gf_dt;
                v8 = 8.0f * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlend_C(&v130, &v98, v8, &v162);
                v128.x = 0.0;
                v128.y = 0.0;
                v128.z = 1.0;
                v27 = (tGameObjectMain*)v186->o_aircraft;
                if (v27)
                {
                    if ((unsigned int)v27 <= 2)
                        v27 = AI_MainActor[(uint32_t)v27 - 1];
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(v27, &v162, &v128);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v162, &v128);
                }
            }
            v112 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_exec_camera);
            if (v112 != 1000)
                return v112;
        }
        else
        {
            v186->i_flag_lightning = 0;
            if (v186->i_snd_ID != -1)
            {
                SND_StopRq(v186->i_snd_ID);
                v186->i_snd_ID = -1;
            }
            v26 = (tGameObjectMain*)v186->des_o_wp;
            if (v26)
            {
                if ((unsigned int)v26 <= 2)
                    v26 = AI_MainActor[(uint32_t)v26 - 1];
                AI_EvalFunc_OBJGetPos_C(v26, &v186->v_my_pos);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v186->v_my_pos);
            }
            v25 = (tGameObjectMain*)v186->o_aircraft;
            if (v25)
            {
                if ((unsigned int)v25 <= 2)
                    v25 = AI_MainActor[(uint32_t)v25 - 1];
                AI_EvalFunc_OBJGetPos_C(v25, &v175);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v175);
            }
            v24 = v175.z + 20.0f;
            if (v186->v_my_pos.z < (double)v24)
                v11 = v175.z + 20.0f;
            else
                v11 = v186->v_my_pos.z;
            v186->v_my_pos.z = v11;
            if (v186->f_dist > (double)v186->des_f_dist_max)
                v10 = v186->des_f_dist_max;
            else
                v10 = v186->f_dist;
            v115 = v10;
            v65 = a2.x * v10;
            v66 = a2.y * v10;
            v67 = a2.z * v10;
            v23 = &v186->v_my_pos;
            v186->v_my_pos.x = v65 + v186->v_my_pos.x;
            v23->y = v66 + v23->y;
            v23->z = v67 + v23->z;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v172);
            v143 = TIM_gf_dt;
            v22 = v186->f_choppe_blend_speed * TIM_gf_dt;
            if (v22 < 0.0)
                v22 = 0.0;
            if (v22 > 1.0)
                v22 = 1.0;
            v116 = v186->v_my_pos.x * v22 + (1.0f - v22) * v172.x;
            v186->v_my_pos.x = v116;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v69);
            v88 = TIM_gf_dt;
            v21 = v186->f_choppe_blend_speed * TIM_gf_dt;
            if (v21 < 0.0)
                v21 = 0.0;
            if (v21 > 1.0)
                v21 = 1.0;
            v188 = v186->v_my_pos.y * v21 + (1.0f - v21) * v69.y;
            v186->v_my_pos.y = v188;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v151);
            v123 = TIM_gf_dt;
            v20 = 0.5f * TIM_gf_dt;
            if (v20 < 0.0)
                v20 = 0.0;
            if (v20 > 1.0)
                v20 = 1.0;
            v89 = v186->v_my_pos.z * v20 + (1.0f - v20) * v151.z;
            v186->v_my_pos.z = v89;
            if (!AI_EvalFunc_TIMEElapsed_C(v186->f_time_keep_camera, 3.0))
            {
                v155 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_exec_camera);
                if (v155 != 1000)
                    return v155;
            }
        }
    }
    AI_EvalFunc_OBJSetPos_C_CURRENT(&v186->v_my_pos);
    AI_EvalFunc_COLPosGet_C(AI_gpst_CurrentGameObject, 0, &v122);
    v117 = v122;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v80);
    v183 = 0;
    v184 = 0.0;
    v185 = v80.z - v186->v_last_pos.z;
    v19 = &v186->v_grav_pos;
    v186->v_grav_pos.x = v186->v_grav_pos.x + 0.0f;
    v19->y = v184 + v19->y;
    v19->z = v185 + v19->z;
    v160 = v186->v_grav_pos;
    v18 = v186->des_f_air_friction;
    v179 = v18 * v186->v_grav_speed.x;
    v180 = v186->v_grav_speed.y * v18;
    v181 = v186->v_grav_speed.z * v18;
    v153 = TIM_gf_dt;
    v118 = v179 * TIM_gf_dt;
    v119 = v180 * TIM_gf_dt;
    v120 = v181 * TIM_gf_dt;
    v17 = &v186->v_grav_pos;
    v186->v_grav_pos.x = v118 + v186->v_grav_pos.x;
    v17->y = v119 + v17->y;
    v17->z = v120 + v17->z;
    v81 = 0;
    v82 = 0;
    v83 = v186->des_f_air_gravity;
    v189 = TIM_gf_dt;
    v147 = 0.0f * TIM_gf_dt;
    v148 = v147;
    v149 = v83 * TIM_gf_dt;
    v16 = &v186->v_grav_pos;
    v186->v_grav_pos.x = v147 + v186->v_grav_pos.x;
    v16->y = v148 + v16->y;
    v16->z = v149 + v16->z;
    MATH_SubstractVec(&v186->v_grav_pos, &v186->v_grav_pos, &v117);
    AI_EvalFunc_MATHVecSetNormVector_C(&v186->v_grav_pos, v186->des_f_dist_centre_grav);
    v15 = &v186->v_grav_pos;
    v186->v_grav_pos.x = v117.x + v186->v_grav_pos.x;
    v15->y = v117.y + v15->y;
    v15->z = v117.z + v15->z;
    MATH_SubstractVec(&v84, &v186->v_grav_pos, &v160);
    v190 = TIM_gf_dt;
    MATH_VecMultNormScalar(&v146, &v84, TIM_gf_dt);
    v186->v_grav_speed = v146;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v186->v_last_pos);
    MATH_SubstractVec(&v136, &v117, &v186->v_grav_pos);
    AI_EvalFunc_MATHVecNormalize_C(&v136, &v171);
    v139 = v171;
    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v70);
    v114 = TIM_gf_dt;
    v9 = 8.0f * TIM_gf_dt;
    AI_EvalFunc_MATHVecBlendRotate_C(&v70, &v139, v9, &v137);
    v139 = v137;
    AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a2, &v139);
    v177 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_attente_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning);
    if (v177 == 1000)
        result = 1000;
    else
        result = v177;
    return result;
}

int Garde_Barriere_Entrepot_GBE_exec_trace()
{
    float v1; // [esp+4h] [ebp-3A8h]
    float v2; // [esp+4h] [ebp-3A8h]
    float v3; // [esp+4h] [ebp-3A8h]
    float v4; // [esp+4h] [ebp-3A8h]
    float v5; // [esp+4h] [ebp-3A8h]
    float v6; // [esp+4h] [ebp-3A8h]
    float v7; // [esp+8h] [ebp-3A4h]
    tScriptStruct_PNJGardeBarriereEntrepot* v8; // [esp+Ch] [ebp-3A0h]
    float v9; // [esp+20h] [ebp-38Ch]
    float v10; // [esp+24h] [ebp-388h]
    float v11; // [esp+28h] [ebp-384h]
    float v12; // [esp+2Ch] [ebp-380h]
    float v13; // [esp+30h] [ebp-37Ch]
    float v14; // [esp+34h] [ebp-378h]
    float v15; // [esp+38h] [ebp-374h]
    float v16; // [esp+3Ch] [ebp-370h]
    float v17; // [esp+40h] [ebp-36Ch]
    float v18; // [esp+44h] [ebp-368h]
    float v19; // [esp+48h] [ebp-364h]
    float v20; // [esp+80h] [ebp-32Ch]
    tGameObjectMain* v21; // [esp+94h] [ebp-318h]
    Vec3 v22; // [esp+98h] [ebp-314h] BYREF
    Vec3 dest; // [esp+A4h] [ebp-308h] BYREF
    Vec3 v24; // [esp+B0h] [ebp-2FCh] BYREF
    float v25; // [esp+BCh] [ebp-2F0h]
    float v26; // [esp+C0h] [ebp-2ECh]
    float v27; // [esp+C4h] [ebp-2E8h]
    float v28; // [esp+C8h] [ebp-2E4h]
    float v29; // [esp+CCh] [ebp-2E0h]
    float v30; // [esp+D0h] [ebp-2DCh]
    float v31; // [esp+D4h] [ebp-2D8h]
    float v32; // [esp+D8h] [ebp-2D4h]
    int v33; // [esp+DCh] [ebp-2D0h]
    float v34; // [esp+E0h] [ebp-2CCh]
    Vec3 v35; // [esp+E4h] [ebp-2C8h] BYREF
    Vec3 v36; // [esp+F0h] [ebp-2BCh] BYREF
    Vec3 v37; // [esp+FCh] [ebp-2B0h] BYREF
    float v38; // [esp+10Ch] [ebp-2A0h]
    float v39; // [esp+110h] [ebp-29Ch]
    Vec3 v40; // [esp+114h] [ebp-298h] BYREF
    Vec3 v41; // [esp+120h] [ebp-28Ch]
    Vec3 v42; // [esp+12Ch] [ebp-280h] BYREF
    Vec3 v43; // [esp+138h] [ebp-274h] BYREF
    float v44; // [esp+144h] [ebp-268h]
    Vec3 v45; // [esp+148h] [ebp-264h] BYREF
    Vec3 v46; // [esp+154h] [ebp-258h] BYREF
    Vec3 v47; // [esp+160h] [ebp-24Ch] BYREF
    Vec3 v48; // [esp+16Ch] [ebp-240h] BYREF
    Vec3 v49; // [esp+178h] [ebp-234h] BYREF
    Vec3 v50; // [esp+184h] [ebp-228h] BYREF
    float v51; // [esp+190h] [ebp-21Ch]
    float v52; // [esp+194h] [ebp-218h]
    Vec3 v53; // [esp+198h] [ebp-214h] BYREF
    Vec3 v54; // [esp+1A4h] [ebp-208h] BYREF
    float v55; // [esp+1B8h] [ebp-1F4h]
    int v56; // [esp+1BCh] [ebp-1F0h]
    float v58; // [esp+1C4h] [ebp-1E8h]
    int v59; // [esp+1C8h] [ebp-1E4h]
    float v60; // [esp+1CCh] [ebp-1E0h]
    Vec3 v61; // [esp+1D0h] [ebp-1DCh] BYREF
    float v62; // [esp+1DCh] [ebp-1D0h]
    int v63; // [esp+1E0h] [ebp-1CCh]
    float v64; // [esp+1E4h] [ebp-1C8h]
    Vec3 v65; // [esp+1E8h] [ebp-1C4h] BYREF
    Vec3 v66; // [esp+1F4h] [ebp-1B8h] BYREF
    Vec3 v67; // [esp+200h] [ebp-1ACh] BYREF
    Vec3 v68; // [esp+20Ch] [ebp-1A0h] BYREF
    Vec3 v69; // [esp+218h] [ebp-194h] BYREF
    Vec3 a1; // [esp+224h] [ebp-188h] BYREF
    Vec3 v71; // [esp+230h] [ebp-17Ch] BYREF
    float v72; // [esp+23Ch] [ebp-170h]
    Vec3 v73; // [esp+244h] [ebp-168h] BYREF
    float v74; // [esp+250h] [ebp-15Ch]
    int v75; // [esp+254h] [ebp-158h]
    float v76; // [esp+258h] [ebp-154h]
    Vec3 v77; // [esp+25Ch] [ebp-150h] BYREF
    Vec3 v78; // [esp+268h] [ebp-144h] BYREF
    float v79; // [esp+278h] [ebp-134h]
    float v80; // [esp+27Ch] [ebp-130h]
    int v81; // [esp+280h] [ebp-12Ch]
    Vec3 v82; // [esp+284h] [ebp-128h] BYREF
    Vec3 a2; // [esp+290h] [ebp-11Ch] BYREF
    float v84; // [esp+29Ch] [ebp-110h]
    Vec3 v86; // [esp+2A4h] [ebp-108h]
    Vec3 v87; // [esp+2B0h] [ebp-FCh] BYREF
    Vec3 a3; // [esp+2BCh] [ebp-F0h] BYREF
    Vec3 v89; // [esp+2C8h] [ebp-E4h] BYREF
    Vec3 v90; // [esp+2D4h] [ebp-D8h] BYREF
    float v91; // [esp+2E4h] [ebp-C8h]
    int v92; // [esp+2E8h] [ebp-C4h]
    Vec3 v93; // [esp+2ECh] [ebp-C0h] BYREF
    Vec3 v94; // [esp+2F8h] [ebp-B4h] BYREF
    Vec3 v95; // [esp+304h] [ebp-A8h] BYREF
    float v96; // [esp+310h] [ebp-9Ch]
    int v97; // [esp+314h] [ebp-98h]
    float v98; // [esp+318h] [ebp-94h]
    Vec3 v99; // [esp+31Ch] [ebp-90h] BYREF
    float v100; // [esp+328h] [ebp-84h]
    int v101; // [esp+32Ch] [ebp-80h]
    float v102; // [esp+330h] [ebp-7Ch]
    tScriptStruct_PNJGardeBarriereEntrepot* v103; // [esp+334h] [ebp-78h]
    float v104; // [esp+338h] [ebp-74h]
    float v105; // [esp+33Ch] [ebp-70h]
    float v106; // [esp+340h] [ebp-6Ch]
    float v107; // [esp+344h] [ebp-68h]
    Vec3 v108; // [esp+348h] [ebp-64h] BYREF
    float v109; // [esp+354h] [ebp-58h]
    int v110; // [esp+358h] [ebp-54h]
    float v111; // [esp+35Ch] [ebp-50h]
    Vec3 v112; // [esp+360h] [ebp-4Ch] BYREF
    float v113; // [esp+36Ch] [ebp-40h]
    float v114; // [esp+378h] [ebp-34h]
    int v115; // [esp+37Ch] [ebp-30h]
    float v116; // [esp+380h] [ebp-2Ch]
    Vec3 v117; // [esp+384h] [ebp-28h] BYREF
    Vec3 v118; // [esp+390h] [ebp-1Ch] BYREF
    Vec3 v119; // [esp+39Ch] [ebp-10h] BYREF

    v40.x = 1.0;
    v40.y = 0.0;
    v40.z = 0.0;
    v54.x = 1.0;
    v54.y = 0.0;
    v54.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    v90.x = 1.0;
    v90.y = 0.0;
    v90.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    v71.x = 1.0;
    v71.y = 0.0;
    v71.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v8 = (tScriptStruct_PNJGardeBarriereEntrepot*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v8 = 0;
    v103 = v8;
    v28 = 0.0;
    v91 = 0.0;
    v104 = 0.0;
    v38 = 0.0;
    v105 = 0.0;
    v113 = 0.0;
    v56 = 0;
    v72 = 0.0;
    v29 = 0.0;
    v106 = 0.0;
    v81 = 0;
    v39 = 0.0;
    v51 = 0.0;
    v84 = 0.0;
    v30 = 0.0;
    v92 = 0;
    v107 = 0.0;
    v44 = 0.0;
    v31 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Garde_Barriere_Entrepot_GBE_exec_trace_PJ_aircraft_material_gao = AI_ul_RealResolveOneRefObject(0x720006F2u);
    }
    if (v103->GFX_Airtrace == -1)
    {
        v103->GFX_Airtrace = AI_EvalFunc_GFXAdd_C_CURRENT(12);
        v21 = (tGameObjectMain*)Garde_Barriere_Entrepot_GBE_exec_trace_PJ_aircraft_material_gao;
        if (Garde_Barriere_Entrepot_GBE_exec_trace_PJ_aircraft_material_gao)
        {
            if ((unsigned int)Garde_Barriere_Entrepot_GBE_exec_trace_PJ_aircraft_material_gao <= 2)
                v21 = AI_MainActor[Garde_Barriere_Entrepot_GBE_exec_trace_PJ_aircraft_material_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v103->GFX_Airtrace, v21, 0);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v103->GFX_Airtrace, 0, 0);
        }
        GFXSetCurGlobal(v103->GFX_Airtrace);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12100, 6);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12101, 5);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12102, 15);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12103, 1);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12111, 8);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12000, -95.5);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12004, 2.0);
        v99.x = 0.2f;
        v99.y = 0.2f;
        v99.z = 0.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(12202, &v99);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12105, 0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12106, 0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12107, 1);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12001, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12002, 0.2f);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12105, 1);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12106, 1);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12107, 2);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12001, 0.2f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12002, 0.40000001f);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12105, 2);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12106, 2);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12107, 3);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12001, 0.40000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12002, 0.60000002f);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12105, 3);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12106, 3);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12107, 4);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12001, 0.60000002f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12002, 0.80000001f);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12105, 4);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12106, 4);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12107, 5);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12001, 0.80000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12002, 1.0);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v103->GFX_Airtrace, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v103->GFX_Airtrace, 2, 1);
        v103->i_flag_trace_on = 0;
    }
    v81 = 0;
    if (!v103->i_flag_real_water && v103->f_water_Z == -1.0e14f)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v112);
        if (v103->f_last_valid_water_Z > v112.z - 5.0)
            v81 = 1;
        v31 = v103->f_last_valid_water_Z + 0.5f;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v47);
        v38 = v47.z - 5.0f - v31;
    }
    else
    {
        if (v103->i_flag_real_water)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v82);
            AI_EvalFunc_WTR_GetDif_C(&v82, &v37);
            v31 = v37.z;
        }
        else
        {
            v31 = v103->f_water_Z + 0.5f;
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v36);
        v38 = v36.z - 5.0f - v31;
        if (v38 < 0.5)
            v103->f_time_gfx_trace = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    v103->f_last_water_Z = v103->f_water_Z;
    if (v81 && !v103->i_flag_trace_on)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    v28 = 5.0;
    v29 = 4.0;
    v104 = 3.0;
    v51 = 0.25;
    v113 = 10.0;
    v30 = 10.0;
    v72 = 30.0;
    v44 = 30.0;
    AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&a2);
    a1.x = a2.x;
    a1.y = a2.y;
    a1.z = 0.0;
    v39 = MATH_VecLen(&a1);
    v20 = v39 / 40.0f;
    if (v20 > 1.0)
        v7 = 1.0;
    else
        v7 = v39 / 40.0f;
    v106 = v7;
    v79 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
    if (v79 == 0.0)
    {
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v35);
        v71 = v35;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v65);
        v90 = v65;
        v86.x = 0.0;
        v86.y = 0.0;
        v86.z = 1.0;
        v54.x = 0.0;
        v54.y = 0.0;
        v54.z = 1.0;
        v105 = 0.0;
        v91 = 0.0;
    }
    else
    {
        v41.x = 0.0;
        v41.y = 0.0;
        v41.z = 1.0;
        v54.x = 0.0;
        v54.y = 0.0;
        v54.z = 1.0;
        AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v117);
        v80 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
        MATH_VecMultNormScalar(&v53, &v117, v80);
        v90 = v53;
        AI_EvalFunc_MATHVecCrossProduct_C(&v90, &v54, &v66);
        v71 = v66;
        v72 = v72 * 0.5f;
        v113 = v113 * 0.5f;
        v105 = 1.0;
        v44 = v44 * 0.5f;
        v30 = v30 * 0.5f;
        v91 = 1.0;
    }
    v19 = v106 * 4.0f;
    if (v19 <= 1.0)
    {
        if (v19 >= 0.2)
            v18 = v106 * 4.0f;
        else
            v18 = 0.2f;
    }
    else
    {
        v18 = 1.0;
    }
    v105 = v105 * v18;
    v17 = v106 * 4.0f;
    if (v17 <= 1.0)
    {
        if (v17 >= 0.2)
            v16 = v106 * 4.0f;
        else
            v16 = 0.2f;
    }
    else
    {
        v16 = 1.0;
    }
    v91 = v91 * v16;
    v72 = v72 * v106 * 0.25f + v72;
    v44 = v44 * v106 * 0.25f + v44;
    v113 = v113 * v106 * 0.5f + v113;
    v30 = v30 * v106 * 0.5f + v30;
    if (v38 >= 0.5)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v103->f_time_gfx_trace, 0.2f))
        {
            v105 = 0.0;
            v91 = 0.0;
            if (AI_EvalFunc_TIMEElapsed_C(v103->f_time_gfx_trace, 0.5))
            {
            LABEL_45:
                AI_EvalFunc_GFXSeti_C_CURRENT(v103->GFX_Airtrace, 12103, 0);
                v103->i_flag_trace_on = 0;
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                return 1000;
            }
        }
        else
        {
            v55 = TIM_gf_MainClock - TIM_gf_LockTime;
            v84 = 1.0f - (v55 - v103->f_time_gfx_trace) / 0.2f;
            v105 = v105 * v84;
            v91 = v91 * v84;
        }
    }
    if (v81)
        goto LABEL_45;
    if (!v103->i_flag_trace_on)
    {
        v105 = 0.0;
        v91 = 0.0;
        v103->i_flag_trace_on = 1;
    }
    v92 = AI_EvalFunc_COLORBlend_C(0, -1, v105);
    v56 = AI_EvalFunc_COLORBlend_C(0, -1, v91);
    v15 = -v38;
    if (v15 <= 1.0)
    {
        if (v15 >= -1.0)
            v107 = -v38;
        else
            v107 = -1.0;
    }
    else
    {
        v107 = 1.0;
    }
    v107 = v107 * 0.5f + 0.5f;
    v107 = v107 * 1.5f;
    v30 = v107 * v106 * v30;
    v113 = v107 * v106 * v113;
    v44 = v107 * v106 * v44;
    v72 = v107 * v106 * v72;
    GFXSetCurGlobal(v103->GFX_Airtrace);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12104, 0);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12109, v92);
    v67.x = -v28;
    v67.y = 0.0;
    v67.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12200, &v67);
    v100 = -v72 * 1.5f;
    v101 = 0;
    v102 = v113 * 1.5f;
    v14 = 1.0f - v51 + 1.0f - v51;
    v68.x = v100 * v14;
    v68.y = 0.0f * v14;
    v68.z = v102 * v14;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12201, &v68);
    v74 = -v72 * 1.5f;
    v75 = 0;
    v76 = v113 * 1.5f;
    v13 = 2.0f * v51;
    v108.x = v74 * v13;
    v108.y = 0.0f * v13;
    v108.z = v76 * v13;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12208, &v108);
    v1 = v103->f_last_valid_water_Z + 0.2f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(12003, v1);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12104, 5);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12109, v56);
    v46.x = v28;
    v46.y = 0.0;
    v46.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12200, &v46);
    v114 = v44 * 1.5f;
    v115 = 0;
    v116 = v30 * 1.5f;
    v12 = 1.0f - v51 + 1.0f - v51;
    v89.x = v114 * v12;
    v89.y = 0.0f * v12;
    v89.z = v116 * v12;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12201, &v89);
    v32 = v44 * 1.5f;
    v33 = 0;
    v34 = v30 * 1.5f;
    v11 = 2.0f * v51;
    v50.x = v32 * v11;
    v50.y = 0.0f * v11;
    v50.z = v34 * v11;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12208, &v50);
    v2 = v103->f_last_valid_water_Z + 0.2f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(12003, v2);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12104, 1);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12109, v92);
    v93.x = -v29;
    v93.y = 0.0;
    v93.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12200, &v93);
    v109 = -v72;
    v110 = 0;
    v111 = v113;
    v10 = 1.0f - v51;
    v78.x = v109 * v10;
    v78.y = 0.0f * v10;
    v78.z = v113 * v10;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12201, &v78);
    v58 = -v72;
    v59 = 0;
    v60 = v113;
    v87.x = v58 * v51;
    v87.y = 0.0f * v51;
    v87.z = v113 * v51;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12208, &v87);
    v3 = v103->f_last_valid_water_Z + 0.2f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(12003, v3);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12104, 4);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12109, v56);
    v49.x = v29;
    v49.y = 0.0;
    v49.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12200, &v49);
    v62 = v44;
    v63 = 0;
    v64 = v30;
    v9 = 1.0f - v51;
    v118.x = v44 * v9;
    v118.y = 0.0f * v9;
    v118.z = v30 * v9;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12201, &v118);
    v96 = v44;
    v97 = 0;
    v98 = v30;
    v42.x = v44 * v51;
    v42.y = 0.0f * v51;
    v42.z = v30 * v51;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12208, &v42);
    v4 = v103->f_last_valid_water_Z + 0.2f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(12003, v4);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12104, 2);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12109, v92);
    v95.x = -v104;
    v95.y = 0.0;
    v95.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12200, &v95);
    v119.x = 2.0;
    v119.y = 0.0;
    v119.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12201, &v119);
    v48.x = 0.5;
    v48.y = 0.0;
    v48.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12208, &v48);
    v69.x = 0.0;
    v69.y = 0.0;
    v69.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12207, &v69);
    v5 = v103->f_last_valid_water_Z + 0.2f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(12003, v5);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12104, 3);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12109, v56);
    v45.x = v104;
    v45.y = 0.0;
    v45.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12200, &v45);
    v61.x = -2.0;
    v61.y = 0.0;
    v61.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12201, &v61);
    v22.x = -0.5;
    v22.y = 0.0;
    v22.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12208, &v22);
    v94.x = 0.0;
    v94.y = 0.0;
    v94.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12207, &v94);
    v6 = v103->f_last_valid_water_Z + 0.2f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(12003, v6);
    AI_EvalFunc_GFXSetv_C_CURGLOB(12203, &v71);
    AI_EvalFunc_GFXSetv_C_CURGLOB(12204, &v90);
    AI_EvalFunc_GFXSetv_C_CURGLOB(12205, &v54);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12103, 1);
    if (v103->GFX_Airtrace != -1)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v24);
        v40 = v24;
        v40.z = v103->f_last_valid_water_Z + 0.2f;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v77);
        a3 = v77;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v43);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v73);
        MATH_SubstractVec(&dest, &v40, &a3);
        v52 = dest.z * v73.z + dest.y * v73.y + v73.x * dest.x;
        v25 = v43.x * v52;
        v26 = v43.y * v52;
        v27 = v43.z * v52;
        a3.x = a3.x + v25;
        a3.y = a3.y + v26;
        a3.z = a3.z + v27;
        AI_EvalFunc_GFXSetv_C_CURRENT(v103->GFX_Airtrace, 12206, &a3);
    }
    return 1000;
}

int Garde_Barriere_Entrepot_GBE_exec_camera()
{
    int result; // eax
    tScriptStruct_PNJGardeBarriereEntrepot* v1; // [esp+0h] [ebp-ECh]
    tGameObjectMain* v2; // [esp+28h] [ebp-C4h]
    tGameObjectMain* v3; // [esp+2Ch] [ebp-C0h]
    tGameObjectMain* v4; // [esp+30h] [ebp-BCh]
    tGameObjectMain* v5; // [esp+34h] [ebp-B8h]
    Vec3 v6; // [esp+38h] [ebp-B4h] BYREF
    Vec3 dest; // [esp+44h] [ebp-A8h] BYREF
    Vec3 v8; // [esp+50h] [ebp-9Ch] BYREF
    Vec3 v9; // [esp+5Ch] [ebp-90h] BYREF
    Vec3 a3; // [esp+68h] [ebp-84h] BYREF
    Vec3 v11; // [esp+74h] [ebp-78h] BYREF
    unsigned int v12; // [esp+80h] [ebp-6Ch]
    float v13; // [esp+84h] [ebp-68h]
    Vec3 v14; // [esp+88h] [ebp-64h] BYREF
    Vec3 v15; // [esp+94h] [ebp-58h] BYREF
    Vec3 a1; // [esp+A0h] [ebp-4Ch] BYREF
    Vec3 a5; // [esp+ACh] [ebp-40h] BYREF
    Vec3 v18; // [esp+B8h] [ebp-34h] BYREF
    tScriptStruct_PNJGardeBarriereEntrepot* v19; // [esp+C4h] [ebp-28h]
    Vec3 v20; // [esp+C8h] [ebp-24h] BYREF
    Vec3 a2[2]; // [esp+D4h] [ebp-18h] BYREF

    v8.x = 1.0;
    v8.y = 0.0;
    v8.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJGardeBarriereEntrepot*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v19 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao = AI_ul_RealResolveOneRefObject(0x71003F56u);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_o_gencam_mario,
            (void*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao,
            56);
        Garde_Barriere_Entrepot_GBE_exec_camera__PJ_CameraNew_gao = AI_ul_RealResolveOneRefObject(0x3D002FBAu);
        Garde_Barriere_Entrepot_GBE_exec_camera_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_current_camera_mode,
            (void*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao,
            92);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_i_canuserctrl,
            (void*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao,
            248);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_i_canusercut,
            (void*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao,
            252);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_i_priority,
            (void*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao,
            28);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_i_test_vis,
            (void*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao,
            152);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_f_focal_gc,
            (void*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao,
            40);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_f_dist_cut,
            (void*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao,
            276);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_f_ideal_dist,
            (void*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao,
            132);
        Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_Target_gao = AI_ul_RealResolveOneRefObject(0x35006499u);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_Target_gao_v_offset_sight_mario,
            (void*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_Target_gao,
            68);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_Target_gao_f_speed_trans,
            (void*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_Target_gao,
            80);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_Target_gao_o_followed_actor1,
            (void*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_Target_gao,
            12);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_f_ideal_Z,
            (void*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao,
            124);
        Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_f_inertie,
            (void*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao,
            156);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_v_ideal_axis,
            (void*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao,
            112);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_f_AngleZ,
            (void*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao,
            128);
    }
    v5 = *(tGameObjectMain**)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_o_gencam_mario;
    if (*(uint32_t*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_o_gencam_mario)
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
        goto LABEL_19;
    v3 = (tGameObjectMain*)v19->o_aircraft;
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
    if (*(tGameObjectMain**)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_o_gencam_mario == v2)
    {
    LABEL_19:
        v12 = AI_EvalFunc_OBJ_FlagsControlGet_C((tGameObjectMain*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_CameraNew_gao);
        if ((v12 & 0x400000) != 0)
        {
            AI_EvalFunc_OBJ_CustomBitsSet_C(
                (tGameObjectMain*)Garde_Barriere_Entrepot_GBE_exec_camera_GST_Board_panel_gao,
                0x200,
                0);
            *(uint32_t*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_current_camera_mode = 0;
            *(uint32_t*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_o_gencam_mario = v19->o_aircraft;
            *(uint32_t*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_i_canuserctrl = 1;
            *(uint32_t*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_i_canusercut = 1;
            *(uint32_t*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_i_priority = 0x44FA0000;
            *(uint32_t*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_i_test_vis = 1;
            *(uint32_t*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_f_focal_gc = 0x3FCCCCCD;
            *(uint32_t*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_f_dist_cut = 0x41F00000;
            *(uint32_t*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_f_ideal_dist = 0x41F00000;
            AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao, &v18);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
            MATH_SubstractVec(&dest, &v18, &a1);
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v6);
            v13 = v6.z * dest.z + v6.y * dest.y + dest.x * v6.x;
            if (v13 <= 0.0)
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v20);
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v11);
                v15.x = -v11.x;
                v15.y = -v11.y;
                v15.z = -v11.z;
                AI_EvalFunc_MATHVecBlendRotate_C(&v20, &v15, 0.5, &v9);
                v8 = v9;
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(a2);
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&a3);
                AI_EvalFunc_MATHVecBlendRotate_C(a2, &a3, 0.5, &a5);
                v8 = a5;
            }
            AI_EvalFunc_MATHVecSetHorzNormalize_C(&v8);
            *(uint32_t*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_Target_gao_v_offset_sight_mario = 0;
            *(uint32_t*)(Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_Target_gao_v_offset_sight_mario + 4) = 0;
            *(uint32_t*)(Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_Target_gao_v_offset_sight_mario + 8) = 0x40A00000;
            *(uint32_t*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_Target_gao_f_speed_trans = 0x41200000;
            *(uint32_t*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_Target_gao_o_followed_actor1 = v19->o_aircraft;
            AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Aircraft_gao, &v14);
            *(float*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_f_ideal_Z = v14.z + 5.0f;
            *(uint32_t*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_f_inertie = 0x41A00000;
            *(Vec3*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_v_ideal_axis = v8;
            *(uint32_t*)Garde_Barriere_Entrepot_GBE_exec_camera__PJ_Camera_gao_cam_f_AngleZ = 0x3ECCCCCD;
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
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int Garde_Barriere_Entrepot_GBE_exec_GFX_Smoke()
{
    int result; // eax
    tScriptStruct_PNJGardeBarriereEntrepot* v1; // [esp+0h] [ebp-D0h]
    tGameObjectMain* v2; // [esp+10h] [ebp-C0h]
    Vec3 v3; // [esp+14h] [ebp-BCh] BYREF
    Vec3 v4; // [esp+20h] [ebp-B0h] BYREF
    Vec3 v5; // [esp+2Ch] [ebp-A4h] BYREF
    Vec3 v6; // [esp+38h] [ebp-98h] BYREF
    Vec3 v7; // [esp+44h] [ebp-8Ch] BYREF
    Vec3 v8; // [esp+50h] [ebp-80h] BYREF
    Vec3 v9; // [esp+60h] [ebp-70h] BYREF
    Vec3 v10; // [esp+6Ch] [ebp-64h] BYREF
    Vec3 v11; // [esp+78h] [ebp-58h] BYREF
    Vec3 v12; // [esp+84h] [ebp-4Ch] BYREF
    float v13; // [esp+90h] [ebp-40h]
    float v14; // [esp+94h] [ebp-3Ch]
    float v15; // [esp+98h] [ebp-38h]
    Vec3 v16; // [esp+9Ch] [ebp-34h] BYREF
    tScriptStruct_PNJGardeBarriereEntrepot* v17; // [esp+A8h] [ebp-28h]
    Vec3 v18; // [esp+ACh] [ebp-24h] BYREF
    Vec3 a1; // [esp+B8h] [ebp-18h] BYREF
    Vec3 a2; // [esp+C4h] [ebp-Ch] BYREF

    v5.x = 1.0;
    v5.y = 0.0;
    v5.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJGardeBarriereEntrepot*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v17 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Garde_Barriere_Entrepot_GBE_exec_GFX_Smoke_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
    }
    if (v17->f_energie_courante == 0.0f)
    {
        if (v17->GFX_Smoke == -1)
        {
            v17->GFX_Smoke = AI_EvalFunc_GFXAdd_C_CURRENT(13);
            GFXSetCurGlobal(v17->GFX_Smoke);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 400);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13106, 0x107A3FFF);
            v2 = (tGameObjectMain*)Garde_Barriere_Entrepot_GBE_exec_GFX_Smoke_SFX_explosion_gao;
            if (Garde_Barriere_Entrepot_GBE_exec_GFX_Smoke_SFX_explosion_gao)
            {
                if ((unsigned int)Garde_Barriere_Entrepot_GBE_exec_GFX_Smoke_SFX_explosion_gao <= 2)
                    v2 = AI_MainActor[Garde_Barriere_Entrepot_GBE_exec_GFX_Smoke_SFX_explosion_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v17->GFX_Smoke, v2, -1);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v17->GFX_Smoke, 0, -1);
            }
            GFXSetCurGlobal(v17->GFX_Smoke);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13101, 7);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13103, 0x10101010);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13104, 0x20202020);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13005, 1.5);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13006, 3.0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13105, 0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13107, 0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13000, 0.80000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13001, 1.6f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13002, 0.001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13003, 0.1f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13004, 0.89999998f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13007, 5.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.0040000002f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13009, 8.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13010, 30.0);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v17->GFX_Smoke, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v17->GFX_Smoke, 2, 1);
            GFXSetCurGlobal(v17->GFX_Smoke);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13012, 0.0);
            v16.x = -1.0;
            v16.y = -1.0;
            v16.z = 2.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v16);
            v12.x = 1.0;
            v12.y = 1.0;
            v12.z = 2.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v12);
            v4.x = 0.0099999998f;
            v4.y = 0.0099999998f;
            v4.z = 0.02f;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v4);
            v3.x = 0.0;
            v3.y = 0.0;
            v3.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13205, &v3);
            v9.x = 0.0;
            v9.y = 0.0;
            v9.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13206, &v9);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
        v5 = a1;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v7);
        v13 = v7.x * 5.0f;
        v14 = v7.y * 5.0f;
        v15 = v7.z * 5.0f;
        v5.x = v5.x - v13;
        v5.y = v5.y - v14;
        v5.z = v5.z - v15;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v18);
        v8.x = v18.x * 3.0f;
        v8.y = v18.y * 3.0f;
        v8.z = v18.z * 3.0f;
        GFXSetCurGlobal(v17->GFX_Smoke);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13207, &v8);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v11);
        v6.x = v11.x * 3.0f;
        v6.y = v11.y * 3.0f;
        v6.z = v11.z * 3.0f;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13208, &v6);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13200, &v5);
        AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&a2);
        v10.x = a2.x * 0.5f;
        v10.y = a2.y * 0.5f;
        v10.z = a2.z * 0.5f;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13204, &v10);
        result = 1000;
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int Garde_Barriere_Entrepot_GBE_mode_flotte()
{
    double v0; // st7
    int result; // eax
    double v2; // st7
    BOOL v3; // eax
    BOOL v4; // eax
    BOOL v5; // eax
    BOOL v6; // eax
    BOOL v7; // eax
    double v8; // st7
    float X; // [esp+0h] [ebp-868h]
    float Xa; // [esp+0h] [ebp-868h]
    float Xb; // [esp+0h] [ebp-868h]
    float Xc; // [esp+0h] [ebp-868h]
    float Xd; // [esp+0h] [ebp-868h]
    float Xe; // [esp+0h] [ebp-868h]
    float X_4; // [esp+4h] [ebp-864h]
    float v16; // [esp+8h] [ebp-860h]
    float v17; // [esp+Ch] [ebp-85Ch]
    float v18; // [esp+10h] [ebp-858h]
    float v19; // [esp+14h] [ebp-854h]
    float v20; // [esp+18h] [ebp-850h]
    float v21; // [esp+1Ch] [ebp-84Ch]
    float v22; // [esp+20h] [ebp-848h]
    float v23; // [esp+24h] [ebp-844h]
    tScriptStruct_PNJGardeBarriereEntrepot* v24; // [esp+28h] [ebp-840h]
    Vec3* v25; // [esp+40h] [ebp-828h]
    float v26; // [esp+4Ch] [ebp-81Ch]
    float v27; // [esp+C4h] [ebp-7A4h]
    Vec3* v28; // [esp+ECh] [ebp-77Ch]
    Vec3* v29; // [esp+F0h] [ebp-778h]
    Vec3* v30; // [esp+F4h] [ebp-774h]
    float v31; // [esp+FCh] [ebp-76Ch]
    float v32; // [esp+11Ch] [ebp-74Ch]
    float v33; // [esp+130h] [ebp-738h]
    Vec3* v34; // [esp+150h] [ebp-718h]
    Vec3* v35; // [esp+154h] [ebp-714h]
    Vec3* v36; // [esp+158h] [ebp-710h]
    float v37; // [esp+160h] [ebp-708h]
    float v38; // [esp+174h] [ebp-6F4h]
    float v39; // [esp+188h] [ebp-6E0h]
    tGameObjectMain* v40; // [esp+1A0h] [ebp-6C8h]
    float v41; // [esp+1A4h] [ebp-6C4h]
    float v42; // [esp+1A8h] [ebp-6C0h]
    tGameObjectMain* v43; // [esp+1B0h] [ebp-6B8h]
    Vec3* v44; // [esp+1B4h] [ebp-6B4h]
    float v45; // [esp+1C0h] [ebp-6A8h]
    tGameObjectMain* v46; // [esp+1C8h] [ebp-6A0h]
    tGameObjectMain* v47; // [esp+1E0h] [ebp-688h]
    tGameObjectMain* v48; // [esp+1F0h] [ebp-678h]
    Vec3* v49; // [esp+21Ch] [ebp-64Ch]
    tGameObjectMain* v50; // [esp+24Ch] [ebp-61Ch]
    tGameObjectMain* v51; // [esp+258h] [ebp-610h]
    tGameObjectMain* v52; // [esp+260h] [ebp-608h]
    Vec3* v53; // [esp+264h] [ebp-604h]
    Vec3* v54; // [esp+268h] [ebp-600h]
    tGameObjectMain* v55; // [esp+278h] [ebp-5F0h]
    tGameObjectMain* v56; // [esp+288h] [ebp-5E0h]
    Vec3 v57; // [esp+28Ch] [ebp-5DCh] BYREF
    float v58; // [esp+298h] [ebp-5D0h]
    float v59; // [esp+29Ch] [ebp-5CCh]
    float v60; // [esp+2A0h] [ebp-5C8h]
    int v61; // [esp+2A4h] [ebp-5C4h]
    int v62; // [esp+2A8h] [ebp-5C0h]
    float v63; // [esp+2ACh] [ebp-5BCh]
    Vec3 v64; // [esp+2B0h] [ebp-5B8h] BYREF
    float v65; // [esp+2BCh] [ebp-5ACh]
    float v66; // [esp+2C0h] [ebp-5A8h]
    float v67; // [esp+2C4h] [ebp-5A4h]
    float v68; // [esp+2C8h] [ebp-5A0h]
    BOOL v70; // [esp+2D0h] [ebp-598h]
    float v71; // [esp+2D4h] [ebp-594h]
    float v72; // [esp+2D8h] [ebp-590h]
    float v73; // [esp+2DCh] [ebp-58Ch]
    int v74; // [esp+2E0h] [ebp-588h]
    float v75; // [esp+2E4h] [ebp-584h]
    float v77; // [esp+2ECh] [ebp-57Ch]
    int v78; // [esp+2F0h] [ebp-578h]
    Vec3 v79; // [esp+2F4h] [ebp-574h] BYREF
    Vec3 v80; // [esp+300h] [ebp-568h] BYREF
    Vec3 v81; // [esp+30Ch] [ebp-55Ch] BYREF
    Vec3 v82; // [esp+318h] [ebp-550h] BYREF
    Vec3 v83; // [esp+324h] [ebp-544h] BYREF
    Vec3 v84; // [esp+330h] [ebp-538h] BYREF
    Vec3 v85; // [esp+33Ch] [ebp-52Ch] BYREF
    float v86; // [esp+348h] [ebp-520h]
    float v87; // [esp+34Ch] [ebp-51Ch]
    float v88; // [esp+350h] [ebp-518h]
    Vec3 v89; // [esp+354h] [ebp-514h] BYREF
    float v90; // [esp+36Ch] [ebp-4FCh]
    float v91; // [esp+370h] [ebp-4F8h]
    float v92; // [esp+378h] [ebp-4F0h]
    float v93; // [esp+380h] [ebp-4E8h]
    float v94; // [esp+384h] [ebp-4E4h]
    int v95; // [esp+38Ch] [ebp-4DCh]
    Vec3 v96; // [esp+390h] [ebp-4D8h] BYREF
    Vec3 v97; // [esp+39Ch] [ebp-4CCh]
    Vec3 v98; // [esp+3A8h] [ebp-4C0h]
    float v99; // [esp+3B4h] [ebp-4B4h]
    BOOL v100; // [esp+3B8h] [ebp-4B0h]
    Vec3 a5; // [esp+3BCh] [ebp-4ACh] BYREF
    Vec3 v102; // [esp+3C8h] [ebp-4A0h] BYREF
    Vec3 v103; // [esp+3D4h] [ebp-494h] BYREF
    Vec3 v104; // [esp+3E0h] [ebp-488h] BYREF
    Vec3 v105; // [esp+3ECh] [ebp-47Ch] BYREF
    float v106; // [esp+3F8h] [ebp-470h]
    float v107; // [esp+3FCh] [ebp-46Ch]
    float v108; // [esp+400h] [ebp-468h]
    Vec3 v109; // [esp+404h] [ebp-464h] BYREF
    BOOL v110; // [esp+410h] [ebp-458h]
    int v111; // [esp+414h] [ebp-454h]
    int v112; // [esp+418h] [ebp-450h]
    int v113; // [esp+41Ch] [ebp-44Ch]
    Vec3 a2; // [esp+420h] [ebp-448h] BYREF
    Vec3 v115; // [esp+42Ch] [ebp-43Ch]
    float v116; // [esp+438h] [ebp-430h]
    float v117; // [esp+43Ch] [ebp-42Ch]
    float v118; // [esp+440h] [ebp-428h]
    Vec3 v119; // [esp+444h] [ebp-424h] BYREF
    Vec3 v120; // [esp+450h] [ebp-418h] BYREF
    Vec3 v121; // [esp+45Ch] [ebp-40Ch] BYREF
    float v122; // [esp+468h] [ebp-400h]
    BOOL v124; // [esp+474h] [ebp-3F4h]
    BOOL v125; // [esp+47Ch] [ebp-3ECh]
    int v126; // [esp+480h] [ebp-3E8h]
    Vec3 v127; // [esp+484h] [ebp-3E4h] BYREF
    Vec3 v128; // [esp+490h] [ebp-3D8h] BYREF
    Vec3 v129; // [esp+49Ch] [ebp-3CCh] BYREF
    Vec3 v130; // [esp+4A8h] [ebp-3C0h] BYREF
    Vec3 v131; // [esp+4B4h] [ebp-3B4h] BYREF
    Vec3 v132; // [esp+4C0h] [ebp-3A8h] BYREF
    Vec3 v133; // [esp+4CCh] [ebp-39Ch] BYREF
    Vec3 v134; // [esp+4D8h] [ebp-390h] BYREF
    float v136; // [esp+4E8h] [ebp-380h]
    int v137; // [esp+4ECh] [ebp-37Ch]
    float v138; // [esp+4F0h] [ebp-378h]
    float v139; // [esp+4F4h] [ebp-374h]
    Vec3 v140; // [esp+4F8h] [ebp-370h] BYREF
    float v141; // [esp+504h] [ebp-364h]
    float v142; // [esp+508h] [ebp-360h]
    float v143; // [esp+50Ch] [ebp-35Ch]
    Vec3 v144; // [esp+510h] [ebp-358h] BYREF
    float v145; // [esp+51Ch] [ebp-34Ch]
    int v146; // [esp+524h] [ebp-344h]
    int v147; // [esp+528h] [ebp-340h]
    Vec3 v148; // [esp+52Ch] [ebp-33Ch] BYREF
    Vec3 v149; // [esp+538h] [ebp-330h] BYREF
    Vec3 dest; // [esp+544h] [ebp-324h] BYREF
    Vec3 v151; // [esp+550h] [ebp-318h] BYREF
    Vec3 v152; // [esp+55Ch] [ebp-30Ch] BYREF
    Vec3 v153; // [esp+568h] [ebp-300h] BYREF
    float v154; // [esp+574h] [ebp-2F4h]
    int v155; // [esp+578h] [ebp-2F0h]
    float v156; // [esp+57Ch] [ebp-2ECh]
    float v157; // [esp+580h] [ebp-2E8h]
    float v158; // [esp+584h] [ebp-2E4h]
    int v159; // [esp+5A0h] [ebp-2C8h]
    int v160; // [esp+5A4h] [ebp-2C4h]
    float v161; // [esp+5A8h] [ebp-2C0h]
    Vec3 a1; // [esp+5ACh] [ebp-2BCh] BYREF
    Vec3 v163; // [esp+5B8h] [ebp-2B0h] BYREF
    float v165; // [esp+5CCh] [ebp-29Ch]
    float v166; // [esp+5D8h] [ebp-290h]
    float v168; // [esp+5E0h] [ebp-288h]
    int v169; // [esp+5E4h] [ebp-284h]
    Vec3 v170; // [esp+5F4h] [ebp-274h] BYREF
    Vec3 v171; // [esp+600h] [ebp-268h] BYREF
    Vec3 v172; // [esp+60Ch] [ebp-25Ch] BYREF
    Vec3 a3; // [esp+618h] [ebp-250h] BYREF
    Vec3 v174; // [esp+624h] [ebp-244h] BYREF
    Vec3 v175; // [esp+630h] [ebp-238h] BYREF
    Vec3 v176; // [esp+63Ch] [ebp-22Ch] BYREF
    Vec3 v177; // [esp+648h] [ebp-220h] BYREF
    BOOL v179; // [esp+658h] [ebp-210h]
    float v180; // [esp+65Ch] [ebp-20Ch]
    tGameObjectMain* v181; // [esp+660h] [ebp-208h]
    tGameObjectMain* v182; // [esp+668h] [ebp-200h]
    float v184; // [esp+670h] [ebp-1F8h]
    int v186; // [esp+678h] [ebp-1F0h]
    Vec3 v187; // [esp+67Ch] [ebp-1ECh] BYREF
    Vec3 v188; // [esp+688h] [ebp-1E0h] BYREF
    float v190; // [esp+69Ch] [ebp-1CCh]
    BOOL v191; // [esp+6A0h] [ebp-1C8h]
    Vec3 v192; // [esp+6A4h] [ebp-1C4h] BYREF
    Vec3 v193; // [esp+6B0h] [ebp-1B8h] BYREF
    Vec3 v194; // [esp+6BCh] [ebp-1ACh] BYREF
    Vec3 v195; // [esp+6C8h] [ebp-1A0h] BYREF
    Vec3 v196; // [esp+6D4h] [ebp-194h] BYREF
    Vec3 v197; // [esp+6E0h] [ebp-188h] BYREF
    float v200; // [esp+6F4h] [ebp-174h]
    float v201; // [esp+6F8h] [ebp-170h]
    float v203; // [esp+700h] [ebp-168h]
    float v204; // [esp+704h] [ebp-164h]
    float v205; // [esp+708h] [ebp-160h]
    float v206; // [esp+70Ch] [ebp-15Ch]
    float v207; // [esp+710h] [ebp-158h]
    Vec3 v208; // [esp+714h] [ebp-154h] BYREF
    Vec3 v209; // [esp+720h] [ebp-148h] BYREF
    float v210; // [esp+72Ch] [ebp-13Ch]
    float v211; // [esp+730h] [ebp-138h]
    float v212; // [esp+734h] [ebp-134h]
    BOOL v213; // [esp+744h] [ebp-124h]
    BOOL v214; // [esp+748h] [ebp-120h]
    float v216; // [esp+754h] [ebp-114h]
    BOOL v217; // [esp+758h] [ebp-110h]
    Vec3 v218; // [esp+75Ch] [ebp-10Ch] BYREF
    Vec3 v219; // [esp+768h] [ebp-100h] BYREF
    Vec3 v220; // [esp+774h] [ebp-F4h] BYREF
    Vec3 v221; // [esp+780h] [ebp-E8h] BYREF
    Vec3 v222; // [esp+78Ch] [ebp-DCh] BYREF
    Vec3 v223; // [esp+798h] [ebp-D0h] BYREF
    float v224; // [esp+7A4h] [ebp-C4h]
    float v225; // [esp+7A8h] [ebp-C0h]
    float v226; // [esp+7ACh] [ebp-BCh]
    float v227; // [esp+7B0h] [ebp-B8h]
    float v228; // [esp+7B4h] [ebp-B4h]
    float v229; // [esp+7B8h] [ebp-B0h]
    tScriptStruct_PNJGardeBarriereEntrepot* v230; // [esp+7BCh] [ebp-ACh]
    int v231; // [esp+7C0h] [ebp-A8h]
    float v232; // [esp+7C8h] [ebp-A0h]
    float v233; // [esp+7CCh] [ebp-9Ch]
    Vec3 v234; // [esp+7D0h] [ebp-98h] BYREF
    Vec3 v235; // [esp+7DCh] [ebp-8Ch] BYREF
    Vec3 v236; // [esp+7F4h] [ebp-74h] BYREF
    Vec3 v237; // [esp+800h] [ebp-68h] BYREF
    Vec3 v238; // [esp+80Ch] [ebp-5Ch] BYREF
    float v239; // [esp+818h] [ebp-50h]
    BOOL v240; // [esp+81Ch] [ebp-4Ch]
    Vec3 v241; // [esp+820h] [ebp-48h] BYREF
    float v242; // [esp+82Ch] [ebp-3Ch]
    float v243; // [esp+830h] [ebp-38h]
    float v244; // [esp+834h] [ebp-34h]
    Vec3 v245; // [esp+838h] [ebp-30h] BYREF
    Vec3 v246; // [esp+844h] [ebp-24h] BYREF
    float v247; // [esp+850h] [ebp-18h]
    float v248; // [esp+858h] [ebp-10h]
    tGameObjectMain* v249; // [esp+85Ch] [ebp-Ch]
    float v250; // [esp+860h] [ebp-8h]
    float v251; // [esp+864h] [ebp-4h]

    v102.x = 1.0;
    v102.y = 0.0;
    v102.z = 0.0;
    v129.x = 1.0;
    v129.y = 0.0;
    v129.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    v241.x = 1.0;
    v241.y = 0.0;
    v241.z = 0.0;
    v194.x = 1.0;
    v194.y = 0.0;
    v194.z = 0.0;
    v144.x = 1.0;
    v144.y = 0.0;
    v144.z = 0.0;
    v195.x = 1.0;
    v195.y = 0.0;
    v195.z = 0.0;
    v89.x = 1.0;
    v89.y = 0.0;
    v89.z = 0.0;
    v97.x = 1.0;
    v97.y = 0.0;
    v97.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    v163.x = 1.0;
    v163.y = 0.0;
    v163.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v24 = (tScriptStruct_PNJGardeBarriereEntrepot*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v24 = 0;
    v230 = v24;
    v200 = 0.0;
    v181 = 0;
    v146 = 0;
    v77 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Garde_Barriere_Entrepot_GBE_mode_flotte__PJ_Camera_gao = AI_ul_RealResolveOneRefObject(0x71003F56u);
        Garde_Barriere_Entrepot_GBE_mode_flotte_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning = (int)AI_ul_RealResolveOneRefFunction(0x72004A10u);
        Garde_Barriere_Entrepot_GBE_mode_flotte_Garde_Barriere_Entrepot_GBE_exec_GFX_Reacteur = (int)AI_ul_RealResolveOneRefFunction(0x720060FFu);
        Garde_Barriere_Entrepot_GBE_mode_flotte_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_mode_flotte_univVars_Flag_speciaux_sauve,
            (void*)Garde_Barriere_Entrepot_GBE_mode_flotte_univVars,
            4488);
        fctvar_Garde_Barriere_Entrepot_GBE_mode_flotte_camera = AI_ul_RealResolveOneRef(42, 0x7100108Bu);
        fct_Garde_Barriere_Entrepot_GBE_mode_flotte_Garde_Barriere_Entrepot_GBE_exec_GFX_Smoke = (int)AI_ul_RealResolveOneRefFunction(0x72004A15u);
    }
    v56 = (tGameObjectMain*)v230->o_main_actor;
    if (v56)
    {
        if ((unsigned int)v56 <= 2)
            v56 = AI_MainActor[(uint32_t)v56 - 1];
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v56);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v136 = (float)v0;
    if (v136 <= 1000000.0)
    {
        AI_EvalFunc_COLPosGet_C(AI_gpst_CurrentGameObject, 0, &v187);
        v194 = v187;
        if (v230->i_current_mode != 3)
        {
            v230->i_last_mode = v230->i_current_mode;
            v230->i_current_mode = 3;
            v100 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
            v95 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x3029004, 0);
            AI_EvalFunc_COLPrioritySet_C(AI_gpst_CurrentGameObject, 100);
            AI_EvalFunc_COLLODSet_C(AI_gpst_CurrentGameObject, 3, 3);
            v230->f_water_Z = -1.0e14f;
            v2 = AI_EvalFunc_COLSizeGet_C(AI_gpst_CurrentGameObject, 0);
            v230->f_zdm_rayon = (float)v2;
            v238.x = 0.0;
            v238.y = 0.0;
            v238.z = v230->des_f_base_gravity;
            AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v238);
            if (v230->i_last_mode != -1)
            {
                v181 = AI_EvalFunc_VIEWGetObject_C(0);
                v55 = v181;
                if (v181)
                {
                    if ((unsigned int)v181 <= 2)
                        v55 = AI_MainActor[(uint32_t)v181 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v55, &v188);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v188);
                }
                v241 = v188;
                AI_EvalFunc_MATHVecSetHorzNormalize_C(&v241);
                v241.x = v241.x * 60.0f;
                v241.y = v241.y * 60.0f;
                v241.z = v241.z * 60.0f;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v234);
                v241.x = v241.x + v234.x;
                v241.y = v241.y + v234.y;
                v241.z = v241.z + v234.z;
                AI_EvalFunc_DYNImpulsion_C(AI_gpst_CurrentGameObject, &v241, 3.0);
                v230->f_rotation_speed = 3.141593f;
            }
            v230->f_air_friction = 1.2f;
            AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v130);
            v54 = &v230->v_grav_speed;
            v230->v_grav_speed.x = v130.x + v230->v_grav_speed.x;
            v54->y = v130.y + v54->y;
            v54->z = v130.z + v54->z;
            AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Garde_Barriere_Entrepot_GBE_mode_flotte__PJ_Camera_gao, &v177);
            v106 = v177.x * 40.0f;
            v107 = v177.y * 40.0f;
            v108 = v177.z * 40.0f;
            v53 = &v230->v_grav_speed;
            v230->v_grav_speed.x = v106 + v230->v_grav_speed.x;
            v53->y = v107 + v53->y;
            v53->z = v108 + v53->z;
            v230->f_choppe_blend_speed = 0.0;
            v249 = AI_gpst_CurrentGameObject;
            v52 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v52 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v52,
                    2,
                    (tAIFunction*)Garde_Barriere_Entrepot_GBE_mode_flotte_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)Garde_Barriere_Entrepot_GBE_mode_flotte_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning);
            }
            v182 = AI_gpst_CurrentGameObject;
            v51 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v51 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v51,
                    2,
                    (tAIFunction*)Garde_Barriere_Entrepot_GBE_mode_flotte_Garde_Barriere_Entrepot_GBE_exec_GFX_Reacteur);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)Garde_Barriere_Entrepot_GBE_mode_flotte_Garde_Barriere_Entrepot_GBE_exec_GFX_Reacteur);
            }
            if (v230->des_SF_inactif > 0)
                AI_EvalFunc_ARRBitSet_C(
                    (int*)(Garde_Barriere_Entrepot_GBE_mode_flotte_univVars_Flag_speciaux_sauve + 4),
                    v230->des_SF_inactif);
        }
        if (AI_EvalFunc_TIMEElapsed_C(v230->f_time_keep_camera, 0.1f))
        {
            AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 1, 1);
            AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0xFFu, 0);
        }
        v155 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x800000, 0);
        AI_EvalFunc_DYNReboundSet_C(AI_gpst_CurrentGameObject, 0.5);
        AI_EvalFunc_DYNApplyRectFactorSet_C(AI_gpst_CurrentGameObject, 0.89999998f);
        v83.x = 0.0;
        v83.y = 0.0;
        v83.z = v230->des_f_base_gravity;
        AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v83);
        v152.x = 0.0;
        v152.y = 0.0;
        v152.z = 0.0;
        AI_EvalFunc_DYNStreamSet_C(AI_gpst_CurrentGameObject, &v152);
        v193.x = 0.0;
        v193.y = 0.0;
        v193.z = 0.0;
        AI_EvalFunc_DYNTractionSet_C_CURRENT(&v193);
        if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 64))
        {
            v181 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 0x40);
            v50 = v181;
            if (v181)
            {
                if ((unsigned int)v181 <= 2)
                    v50 = AI_MainActor[(uint32_t)v181 - 1];
                v3 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v50, 4);
            }
            else
            {
                v3 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 4);
            }
            v191 = v3;
            if (!v3)
            {
                v230->f_time_ground_col = TIM_gf_MainClock - TIM_gf_LockTime;
                AI_EvalFunc_COLPointGet_C(AI_gpst_CurrentGameObject, 64, &v140);
                a1 = v140;
                AI_EvalFunc_COLPosGet_C(AI_gpst_CurrentGameObject, 0, &v197);
                a1.x = a1.x - v197.x;
                a1.y = a1.y - v197.y;
                a1.z = a1.z - v197.z;
                MATH_NormalizeEqualVector(&a1);
                if (a1.z <= 0.0)
                {
                    v98.x = -a1.x;
                    v98.y = -a1.y;
                    v98.z = -a1.z;
                    v49 = &v230->v_normale_sol;
                    v230->v_normale_sol.x = -a1.x;
                    v49->y = v98.y;
                    v49->z = v98.z;
                }
                else
                {
                    v230->v_normale_sol = a1;
                }
            }
        }
        v230->v_water_force.x = 0.0;
        v230->v_water_force.y = 0.0;
        v230->v_water_force.z = 0.0;
        v146 = AI_EvalFunc_COLGMatReportGet_C(AI_gpst_CurrentGameObject, 16);
        if (v146 == -1)
        {
            if (v230->f_water_Z > v230->f_zdm_rayon + v194.z)
            {
                v230->i_flag_in_water = 1;
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v174);
                AI_EvalFunc_WTR_GetDif_C(&v174, &v236);
                v97 = v236;
                if (v194.z - 6.0 >= v236.z)
                {
                    v230->i_flag_in_water = 0;
                    v230->f_water_Z = -1.0e14f;
                }
                else
                {
                    if (v230->i_flag_play_plouf)
                    {
                        v230->i_flag_play_plouf = 0;
                        v126 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(AI_gpst_CurrentGameObject, 4, -1.0, 0.94999999f, 1.05f);
                    }
                    v230->i_flag_real_water = 1;
                    v230->f_water_Z = v97.z;
                    v230->f_last_valid_water_Z = v230->f_water_Z;
                    v230->i_flag_in_water = 1;
                }
            }
        }
        else
        {
            AI_EvalFunc_COLPointGet_C(AI_gpst_CurrentGameObject, v146 + 0x80000000, &v222);
            v97 = v222;
            if (v230->i_flag_play_plouf)
            {
                v230->i_flag_play_plouf = 0;
                v169 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(AI_gpst_CurrentGameObject, 4, -1.0, 0.94999999f, 1.05f);
            }
            v230->i_flag_real_water = 0;
            v230->f_water_Z = v97.z;
            v230->f_last_valid_water_Z = v230->f_water_Z;
            v230->i_flag_in_water = 1;
        }
        AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v245);
        v163 = v245;
        if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x80))
        {
            v181 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 0x80);
            AI_EvalFunc_COLPointGet_C(AI_gpst_CurrentGameObject, 0x80, &v121);
            v89 = v121;
            if (!AI2C_VecEqNull(&v89))
            {
                v48 = v181;
                if (v181)
                {
                    if ((unsigned int)v181 <= 2)
                        v48 = AI_MainActor[(uint32_t)v181 - 1];
                    v4 = AI_EvalFunc_AIIsModel_C(v48, (tAIModel*)fctvar_Garde_Barriere_Entrepot_GBE_mode_flotte_camera);
                }
                else
                {
                    v4 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_Garde_Barriere_Entrepot_GBE_mode_flotte_camera);
                }
                v240 = v4;
                if (!v4)
                {
                    v89.x = v89.x - v194.x;
                    v89.y = v89.y - v194.y;
                    v89.z = v89.z - v194.z;
                    if (AI2C_VecEqNull(&v163))
                    {
                        v47 = v181;
                        if (v181)
                        {
                            if ((unsigned int)v181 <= 2)
                                v47 = AI_MainActor[(uint32_t)v181 - 1];
                            AI_EvalFunc_DYNSpeedGetVector_C(v47, &v148);
                        }
                        else
                        {
                            AI_EvalFunc_DYNSpeedGetVector_C(0, &v148);
                        }
                        v163 = v148;
                    }
                    if (!AI2C_VecEqNull(&v163))
                    {
                        v46 = v181;
                        if (v181)
                        {
                            if ((unsigned int)v181 <= 2)
                                v46 = AI_MainActor[(uint32_t)v181 - 1];
                            v5 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v46, 0x204);
                        }
                        else
                        {
                            v5 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x204);
                        }
                        v125 = v5;
                        if (v5)
                        {
                            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v151);
                            v247 = v89.z * v151.z + v89.y * v151.y + v151.x * v89.x;
                            v45 = -v247;
                            if (v45 <= 3.0)
                            {
                                if (v45 >= -3.0)
                                    v216 = -v247;
                                else
                                    v216 = -3.0;
                            }
                            else
                            {
                                v216 = 3.0;
                            }
                            v77 = v216;
                        }
                        else
                        {
                            v77 = 0.0;
                        }
                        v195 = v163;
                        v110 = AI_EvalFunc_TIMEElapsed_C(v230->f_time_ground_col, 0.1f);
                        if (v110 || !(v213 = AI_EvalFunc_TIMEElapsed_C(v230->f_time_paf, 0.1f)))
                        {
                            v242 = v195.x * v77;
                            v243 = v195.y * v77;
                            v244 = v195.z * v77;
                            v44 = &v230->v_grav_speed;
                            v230->v_grav_speed.x = v242 + v230->v_grav_speed.x;
                            v44->y = v243 + v44->y;
                            v44->z = v244 + v44->z;
                        }
                        if (v77 > 0.0)
                        {
                            v43 = v181;
                            if (v181)
                            {
                                if ((unsigned int)v181 <= 2)
                                    v43 = AI_MainActor[(uint32_t)v181 - 1];
                                v6 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v43, 0x204);
                            }
                            else
                            {
                                v6 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x204);
                            }
                            v124 = v6;
                            if (v6)
                            {
                                if (!AI_EvalFunc_TIMEElapsed_C(v230->f_time_ground_col, 0.1f))
                                {
                                    v122 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
                                    v42 = v122 - v230->f_last_speed;
                                    if (v42 <= 20.0)
                                    {
                                        if (v42 >= 0.0)
                                            v154 = v122 - v230->f_last_speed;
                                        else
                                            v154 = 0.0;
                                    }
                                    else
                                    {
                                        v154 = 20.0;
                                    }
                                    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v57);
                                    v41 = v77 * v154;
                                    v205 = v57.x * v41;
                                    v206 = v57.y * v41;
                                    v207 = v57.z * v41;
                                    v195.x = v195.x - v205;
                                    v195.y = v195.y - v206;
                                    v195.z = v195.z - v207;
                                }
                            }
                        }
                        v78 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v195);
                        v40 = v181;
                        if (v181)
                        {
                            if ((unsigned int)v181 <= 2)
                                v40 = AI_MainActor[(uint32_t)v181 - 1];
                            v7 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v40, 0x204);
                        }
                        else
                        {
                            v7 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x204);
                        }
                        v179 = v7;
                        if (v7)
                        {
                            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v105);
                            AI_EvalFunc_MATHVecCrossProduct_C(&v163, &v105, &v172);
                            v129 = v172;
                            if (!AI2C_VecEqNull(&v129))
                            {
                                if (AI_EvalFunc_TIMEElapsed_C(v230->f_time_ground_col, 0.1f))
                                {
                                    v68 = v89.z * v129.z + v89.y * v129.y + v129.x * v89.x;
                                    if (v68 <= 3.0)
                                    {
                                        if (v68 >= -3.0)
                                            v239 = v68;
                                        else
                                            v239 = -3.0;
                                    }
                                    else
                                    {
                                        v239 = 3.0;
                                    }
                                    v230->f_rotation_speed = v239 * 1.570796f;
                                }
                                else
                                {
                                    v90 = v89.z * v129.z + v89.y * v129.y + v129.x * v89.x;
                                    if (v90 <= 3.0)
                                    {
                                        if (v90 >= -3.0)
                                            v71 = v90;
                                        else
                                            v71 = -3.0;
                                    }
                                    else
                                    {
                                        v71 = 3.0;
                                    }
                                    v230->f_rotation_speed = v71 * 0.392699f;
                                }
                                v156 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
                                v39 = (v156 - v230->f_last_speed) / 100.0f;
                                if (v39 <= 1.0)
                                {
                                    if (v39 >= 0.0)
                                        v73 = (v156 - v230->f_last_speed) / 100.0f;
                                    else
                                        v73 = 0.0;
                                }
                                else
                                {
                                    v73 = 1.0;
                                }
                                v230->f_rotation_speed = v73 * v230->f_rotation_speed;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if (AI_EvalFunc_TIMEElapsed_C(v230->f_time_ground_col, 0.1f))
            {
                v158 = TIM_gf_dt;
                v230->f_rotation_speed = v230->f_rotation_speed - v230->f_rotation_speed * 0.40000001f * TIM_gf_dt;
            }
            else
            {
                v190 = TIM_gf_dt;
                v230->f_rotation_speed = v230->f_rotation_speed - v230->f_rotation_speed * 0.89999998f * TIM_gf_dt;
            }
            v204 = TIM_gf_dt;
            X_4 = v230->f_rotation_speed * TIM_gf_dt;
            AI_EvalFunc_OBJRotateLocalZ_c(AI_gpst_CurrentGameObject, X_4);
        }
        if (v230->i_flag_in_water)
        {
            v230->f_air_friction = v230->des_f_air_friction;
            v230->v_last_collide_pos.x = 0.0;
            v230->v_last_collide_pos.y = 0.0;
            v230->v_last_collide_pos.z = 0.0;
            AI_EvalFunc_MATHVecGlobalToLocal_C(AI_gpst_CurrentGameObject, &v230->des_v_water_friction, &v79);
            v102 = v79;
            v23 = (float)fabs(v79.x);
            v102.x = v23;
            v22 = (float)fabs(v102.y);
            v102.y = v22;
            v21 = (float)fabs(v102.z);
            v102.z = v21;
            v186 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v102);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v96);
            v144 = v96;
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v64);
            v58 = v64.x * 10.0f;
            v59 = v64.y * 10.0f;
            v60 = v64.z * 10.0f;
            v144.x = v144.x - v58;
            v144.y = v144.y - v59;
            v144.z = v144.z - v60;
            v38 = v230->f_water_Z - v144.z;
            if (v38 < 0.0)
                v20 = 0.0;
            else
                v20 = v230->f_water_Z - v144.z;
            v250 = v20;
            v230->v_water_force.x = 0.0;
            v230->v_water_force.y = 0.0;
            v230->v_water_force.z = v230->des_f_archimede * v250;
            if (AI_EvalFunc_TIMEElapsed_C(v230->f_time_ground_col, 0.1f))
            {
                AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v196);
                v163 = v196;
                v144 = v230->v_grav_pos;
                v37 = v230->des_f_water_friction;
                v86 = v37 * v230->v_grav_speed.x;
                v87 = v230->v_grav_speed.y * v37;
                v88 = v230->v_grav_speed.z * v37;
                v99 = TIM_gf_dt;
                v141 = v86 * TIM_gf_dt;
                v142 = v87 * TIM_gf_dt;
                v143 = v88 * TIM_gf_dt;
                v36 = &v230->v_grav_pos;
                v230->v_grav_pos.x = v141 + v230->v_grav_pos.x;
                v36->y = v142 + v36->y;
                v36->z = v143 + v36->z;
                v159 = 0;
                v160 = 0;
                v161 = v230->des_f_water_gravity;
                v72 = TIM_gf_dt;
                v65 = 0.0f * TIM_gf_dt;
                v66 = v65;
                v67 = v161 * TIM_gf_dt;
                v35 = &v230->v_grav_pos;
                v230->v_grav_pos.x = v65 + v230->v_grav_pos.x;
                v35->y = v66 + v35->y;
                v35->z = v67 + v35->z;
                MATH_SubstractVec(&v230->v_grav_pos, &v230->v_grav_pos, &v194);
                AI_EvalFunc_MATHVecSetNormVector_C(&v230->v_grav_pos, v230->des_f_dist_centre_grav);
                v34 = &v230->v_grav_pos;
                v230->v_grav_pos.x = v194.x + v230->v_grav_pos.x;
                v34->y = v194.y + v34->y;
                v34->z = v194.z + v34->z;
                MATH_SubstractVec(&dest, &v230->v_grav_pos, &v144);
                v251 = TIM_gf_dt;
                MATH_VecMultNormScalar(&v235, &dest, TIM_gf_dt);
                v230->v_grav_speed = v235;
                MATH_SubstractVec(&v109, &v194, &v230->v_grav_pos);
                AI_EvalFunc_MATHVecNormalize_C(&v109, &v237);
                a3 = v237;
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&a2);
                v145 = TIM_gf_dt;
                X = 8.0f * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, X, &a5);
                a3 = a5;
            }
            else
            {
                v201 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
                v33 = v201 / 10.0f;
                if (v33 > 6.0)
                    v19 = 6.0;
                else
                    v19 = v201 / 10.0f;
                v157 = v19;
                v200 = v19;
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v103);
                v248 = TIM_gf_dt;
                Xa = v200 * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&v103, &v230->v_normale_sol, Xa, &v192);
                a3 = v192;
                v144 = v230->v_grav_pos;
                v32 = v230->des_f_dist_centre_grav;
                v153.x = v192.x * v32;
                v153.y = v192.y * v32;
                v153.z = v192.z * v32;
                MATH_SubstractVec(&v104, &v194, &v153);
                v230->v_grav_pos = v104;
                MATH_SubstractVec(&v246, &v230->v_grav_pos, &v144);
                v203 = TIM_gf_dt;
                MATH_VecMultNormScalar(&v149, &v246, TIM_gf_dt);
                v230->v_grav_speed = v149;
            }
            v230->f_time_air = TIM_gf_MainClock - TIM_gf_LockTime;
        }
        else
        {
            v70 = AI_EvalFunc_TIMEElapsed_C(v230->f_time_ground_col, 0.1f);
            if (!v70 && (v214 = AI_EvalFunc_TIMEElapsed_C(v230->f_time_paf, 0.30000001f)))
            {
                if (!AI_EvalFunc_TIMEElapsed_C(v230->f_time_air, 0.5))
                    AI_EvalFunc_DYNReboundSet_C(AI_gpst_CurrentGameObject, 0.0);
                AI_EvalFunc_MATHVecGlobalToLocal_C(AI_gpst_CurrentGameObject, &v230->des_v_ground_friction, &v128);
                v102 = v128;
                v18 = (float)fabs(v128.x);
                v102.x = v18;
                v17 = (float)fabs(v102.y);
                v102.y = v17;
                v16 = (float)fabs(v102.z);
                v102.z = v16;
                v137 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v102);
                v94 = TIM_gf_MainClock - TIM_gf_LockTime;
                v230->f_time_air = v94;
                v232 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
                v27 = v232 / 10.0f;
                if (v27 <= 6.0)
                {
                    if (v27 >= 2.0)
                        v184 = v232 / 10.0f;
                    else
                        v184 = 2.0;
                }
                else
                {
                    v184 = 6.0;
                }
                v200 = v184;
                v115.x = 0.0;
                v115.y = 0.0;
                v115.z = 1.0;
                v165 = 1.0f * v230->v_normale_sol.z + 0.0f * v230->v_normale_sol.y + 0.0f * v230->v_normale_sol.x;
                if (v165 <= 0.99619502)
                {
                    v111 = 0;
                    v112 = 0;
                    v113 = 0;
                    a3.x = 0.0;
                    a3.y = 0.0;
                    a3.z = 0.0;
                }
                else
                {
                    AI_EvalFunc_COLPointGet_C(AI_gpst_CurrentGameObject, 64, &v209);
                    a3 = v209;
                    if (AI2C_VecEqNull(&a3))
                        a3 = v230->v_last_collide_pos;
                    else
                        v230->v_last_collide_pos = a3;
                }
                if (AI2C_VecEqNull(&a3))
                {
                    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v223);
                    v93 = TIM_gf_dt;
                    Xe = v200 * TIM_gf_dt;
                    AI_EvalFunc_MATHVecBlendRotate_C(&v223, &v230->v_normale_sol, Xe, &v131);
                    a3 = v131;
                }
                else
                {
                    MATH_SubstractVec(&v208, &v194, &a3);
                    a3 = v208;
                    MATH_NormalizeEqualVector(&a3);
                    v144.x = a3.x;
                    v144.y = a3.y;
                    v144.z = 0.0;
                    v75 = MATH_VecLen(&v144);
                    if (v75 <= 0.1)
                    {
                        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v81);
                        v139 = TIM_gf_dt;
                        Xd = v200 * TIM_gf_dt;
                        AI_EvalFunc_MATHVecBlendRotate_C(&v81, &v230->v_normale_sol, Xd, &v171);
                        a3 = v171;
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v120);
                        v166 = TIM_gf_dt;
                        Xc = 8.0f * TIM_gf_dt;
                        AI_EvalFunc_MATHVecBlendRotate_C(&v120, &a3, Xc, &v85);
                        a3 = v85;
                        v144.x = v144.x * 5.0f;
                        v144.y = v144.y * 5.0f;
                        v144.z = v144.z * 5.0f;
                        AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v127);
                        MATH_VecAddVec(&v175, &v127, &v144);
                        v231 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v175);
                    }
                }
                v144 = v230->v_grav_pos;
                v26 = v230->des_f_dist_centre_grav;
                v218.x = a3.x * v26;
                v218.y = a3.y * v26;
                v218.z = a3.z * v26;
                MATH_SubstractVec(&v84, &v194, &v218);
                v230->v_grav_pos = v84;
                AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v133);
                v180 = TIM_gf_dt;
                v227 = v133.x * TIM_gf_dt;
                v228 = v133.y * TIM_gf_dt;
                v229 = v133.z * TIM_gf_dt;
                v25 = &v230->v_grav_pos;
                v230->v_grav_pos.x = v230->v_grav_pos.x - v227;
                v25->y = v25->y - v228;
                v25->z = v25->z - v229;
                MATH_SubstractVec(&v80, &v230->v_grav_pos, &v144);
                v138 = TIM_gf_dt;
                MATH_VecMultNormScalar(&v170, &v80, TIM_gf_dt);
                v230->v_grav_speed = v170;
            }
            else
            {
                v230->v_last_collide_pos.x = 0.0;
                v230->v_last_collide_pos.y = 0.0;
                v230->v_last_collide_pos.z = 0.0;
                v119.x = 0.0;
                v119.y = 0.0;
                v119.z = 0.0;
                v74 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v119);
                v144 = v230->v_grav_pos;
                v31 = v230->f_air_friction;
                v210 = v31 * v230->v_grav_speed.x;
                v211 = v230->v_grav_speed.y * v31;
                v212 = v230->v_grav_speed.z * v31;
                v168 = TIM_gf_dt;
                v116 = v210 * TIM_gf_dt;
                v117 = v211 * TIM_gf_dt;
                v118 = v212 * TIM_gf_dt;
                v30 = &v230->v_grav_pos;
                v230->v_grav_pos.x = v116 + v230->v_grav_pos.x;
                v30->y = v117 + v30->y;
                v30->z = v118 + v30->z;
                v61 = 0;
                v62 = 0;
                v63 = v230->des_f_air_gravity;
                v91 = TIM_gf_dt;
                v224 = 0.0f * TIM_gf_dt;
                v225 = v224;
                v226 = v63 * TIM_gf_dt;
                v29 = &v230->v_grav_pos;
                v230->v_grav_pos.x = v224 + v230->v_grav_pos.x;
                v29->y = v225 + v29->y;
                v29->z = v226 + v29->z;
                MATH_SubstractVec(&v230->v_grav_pos, &v230->v_grav_pos, &v194);
                AI_EvalFunc_MATHVecSetNormVector_C(&v230->v_grav_pos, v230->des_f_dist_centre_grav);
                v28 = &v230->v_grav_pos;
                v230->v_grav_pos.x = v194.x + v230->v_grav_pos.x;
                v28->y = v194.y + v28->y;
                v28->z = v194.z + v28->z;
                MATH_SubstractVec(&v132, &v230->v_grav_pos, &v144);
                v92 = TIM_gf_dt;
                MATH_VecMultNormScalar(&v221, &v132, TIM_gf_dt);
                v230->v_grav_speed = v221;
                MATH_SubstractVec(&v176, &v194, &v230->v_grav_pos);
                AI_EvalFunc_MATHVecNormalize_C(&v176, &v134);
                a3 = v134;
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v82);
                v233 = TIM_gf_dt;
                Xb = 8.0f * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&v82, &a3, Xb, &v219);
                a3 = v219;
            }
        }
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v220);
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v220, &a3);
        v230->v_last_pos = v194;
        v8 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
        v230->f_last_speed = (float)v8;
        AI_EvalFunc_DYNTractionSet_C_CURRENT(&v230->v_water_force);
        v147 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_flotte_Garde_Barriere_Entrepot_GBE_exec_GFX_Smoke);
        if (v147 == 1000)
            result = 1000;
        else
            result = v147;
    }
    else
    {
        if (v230->GFX_Smoke != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v230->GFX_Smoke);
            v230->GFX_Smoke = -1;
        }
        if (v230->i_current_mode != -1)
        {
            v230->i_current_mode = -1;
            v217 = AI_EvalFunc_DYNOff_C(AI_gpst_CurrentGameObject);
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 4;
    }
    return result;
}

int Garde_Barriere_Entrepot_GBE_mode_repousse()
{
    int v0; // eax
    double v1; // st7
    int result; // eax
    int v3; // eax
    float a4; // [esp+4h] [ebp-450h]
    tScriptStruct_PNJGardeBarriereEntrepot* v5; // [esp+Ch] [ebp-448h]
    tGameObjectMain* v6; // [esp+14h] [ebp-440h]
    tGameObjectMain* v7; // [esp+1Ch] [ebp-438h]
    tGameObjectMain* v8; // [esp+24h] [ebp-430h]
    tGameObjectMain* v9; // [esp+2Ch] [ebp-428h]
    tGameObjectMain* v10; // [esp+3Ch] [ebp-418h]
    tGameObjectMain* v11; // [esp+44h] [ebp-410h]
    tGameObjectMain* v12; // [esp+4Ch] [ebp-408h]
    tGameObjectMain* v13; // [esp+54h] [ebp-400h]
    Vec3* v14; // [esp+7Ch] [ebp-3D8h]
    Vec3* v15; // [esp+80h] [ebp-3D4h]
    Vec3* v16; // [esp+84h] [ebp-3D0h]
    float v17; // [esp+8Ch] [ebp-3C8h]
    Vec3* v18; // [esp+98h] [ebp-3BCh]
    Vec3* v19; // [esp+ACh] [ebp-3A8h]
    float v20; // [esp+B0h] [ebp-3A4h]
    Vec3* v21; // [esp+B4h] [ebp-3A0h]
    float v22; // [esp+B8h] [ebp-39Ch]
    tGameObjectMain* v23; // [esp+C0h] [ebp-394h]
    Vec3* v24; // [esp+C4h] [ebp-390h]
    Vec3* v25; // [esp+C8h] [ebp-38Ch]
    tGameObjectMain* v26; // [esp+D0h] [ebp-384h]
    tGameObjectMain* v27; // [esp+D8h] [ebp-37Ch]
    float v28; // [esp+E4h] [ebp-370h]
    float v29; // [esp+E8h] [ebp-36Ch]
    tGameObjectMain* v30; // [esp+F8h] [ebp-35Ch]
    float v31; // [esp+108h] [ebp-34Ch]
    tGameObjectMain* v32; // [esp+11Ch] [ebp-338h]
    tGameObjectMain* v33; // [esp+124h] [ebp-330h]
    tGameObjectMain* v34; // [esp+12Ch] [ebp-328h]
    tGameObjectMain* v35; // [esp+134h] [ebp-320h]
    tGameObjectMain* v36; // [esp+138h] [ebp-31Ch]
    tGameObjectMain* v37; // [esp+140h] [ebp-314h]
    Vec3 a5; // [esp+148h] [ebp-30Ch] BYREF
    Vec3 v39; // [esp+154h] [ebp-300h] BYREF
    Vec3 v40; // [esp+160h] [ebp-2F4h] BYREF
    Vec3 v41; // [esp+16Ch] [ebp-2E8h] BYREF
    Vec3 v42; // [esp+178h] [ebp-2DCh] BYREF
    Vec3 v43; // [esp+188h] [ebp-2CCh] BYREF
    Vec3 v44; // [esp+194h] [ebp-2C0h] BYREF
    Vec3 v45; // [esp+1A0h] [ebp-2B4h] BYREF
    Vec3 v46; // [esp+1BCh] [ebp-298h] BYREF
    Vec3 a1; // [esp+1C8h] [ebp-28Ch] BYREF
    Vec3 v48; // [esp+1D4h] [ebp-280h] BYREF
    Vec3 v49; // [esp+1E0h] [ebp-274h] BYREF
    float v50; // [esp+1ECh] [ebp-268h]
    Vec3 a2; // [esp+1F0h] [ebp-264h] BYREF
    float v52; // [esp+1FCh] [ebp-258h]
    float v53; // [esp+200h] [ebp-254h]
    float v54; // [esp+204h] [ebp-250h]
    Vec3 v55; // [esp+208h] [ebp-24Ch] BYREF
    Vec3 v56; // [esp+214h] [ebp-240h] BYREF
    Vec3 v57; // [esp+220h] [ebp-234h] BYREF
    float v59; // [esp+230h] [ebp-224h]
    Vec3 v60; // [esp+240h] [ebp-214h] BYREF
    Vec3 v61; // [esp+25Ch] [ebp-1F8h] BYREF
    float v62; // [esp+268h] [ebp-1ECh]
    float v63; // [esp+26Ch] [ebp-1E8h]
    float v64; // [esp+270h] [ebp-1E4h]
    Vec3 v65; // [esp+274h] [ebp-1E0h] BYREF
    float v66; // [esp+280h] [ebp-1D4h]
    float v67; // [esp+284h] [ebp-1D0h]
    float v68; // [esp+288h] [ebp-1CCh]
    float v69; // [esp+28Ch] [ebp-1C8h]
    float v70; // [esp+290h] [ebp-1C4h]
    float v71; // [esp+294h] [ebp-1C0h]
    float v72; // [esp+298h] [ebp-1BCh]
    float v73; // [esp+29Ch] [ebp-1B8h]
    float v74; // [esp+2A0h] [ebp-1B4h]
    float v75; // [esp+2A4h] [ebp-1B0h]
    Vec3 v77; // [esp+2B8h] [ebp-19Ch] BYREF
    Vec3 v78; // [esp+2C4h] [ebp-190h] BYREF
    float v79; // [esp+2D0h] [ebp-184h]
    tGameObjectMain* v80; // [esp+2D4h] [ebp-180h]
    Vec3 v81; // [esp+2D8h] [ebp-17Ch] BYREF
    Vec3 v82; // [esp+2F0h] [ebp-164h] BYREF
    Vec3 v83; // [esp+2FCh] [ebp-158h] BYREF
    Vec3 v84; // [esp+308h] [ebp-14Ch] BYREF
    float v85; // [esp+314h] [ebp-140h]
    int v86; // [esp+318h] [ebp-13Ch]
    float v87; // [esp+330h] [ebp-124h]
    Vec3 a3[2]; // [esp+334h] [ebp-120h] BYREF
    float v89; // [esp+34Ch] [ebp-108h]
    float v90; // [esp+350h] [ebp-104h]
    float v91; // [esp+354h] [ebp-100h]
    int v92; // [esp+358h] [ebp-FCh]
    int v93; // [esp+35Ch] [ebp-F8h]
    float v94; // [esp+360h] [ebp-F4h]
    int v95; // [esp+364h] [ebp-F0h]
    int v96; // [esp+368h] [ebp-ECh]
    Vec3 v97; // [esp+36Ch] [ebp-E8h] BYREF
    Vec3 v98; // [esp+378h] [ebp-DCh] BYREF
    int v99; // [esp+384h] [ebp-D0h]
    float v100; // [esp+388h] [ebp-CCh]
    float v101; // [esp+38Ch] [ebp-C8h]
    Vec3 v102; // [esp+390h] [ebp-C4h] BYREF
    tScriptStruct_PNJGardeBarriereEntrepot* v103; // [esp+3A8h] [ebp-ACh]
    tGameObjectMain* v104; // [esp+3ACh] [ebp-A8h]
    float v105; // [esp+3BCh] [ebp-98h]
    float v106; // [esp+3C0h] [ebp-94h]
    float v107; // [esp+3C4h] [ebp-90h]
    float v108; // [esp+3C8h] [ebp-8Ch]
    float v109; // [esp+3CCh] [ebp-88h]
    float v110; // [esp+3D0h] [ebp-84h]
    Vec3 v111; // [esp+3D4h] [ebp-80h] BYREF
    int v112; // [esp+3E0h] [ebp-74h]
    tMessage Src; // [esp+3E8h] [ebp-6Ch] BYREF
    Vec3 dest; // [esp+41Ch] [ebp-38h] BYREF
    Vec3 v115; // [esp+428h] [ebp-2Ch] BYREF
    int v116; // [esp+44Ch] [ebp-8h]

    v81.x = 1.0;
    v81.y = 0.0;
    v81.z = 0.0;
    a3[0].x = 1.0;
    a3[0].y = 0.0;
    a3[0].z = 0.0;
    v61.x = 1.0;
    v61.y = 0.0;
    v61.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    v78.x = 1.0;
    v78.y = 0.0;
    v78.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_PNJGardeBarriereEntrepot*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v103 = v5;
    v116 = 0;
    v62 = 0.0;
    v64 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Garde_Barriere_Entrepot_GBE_mode_repousse__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        Garde_Barriere_Entrepot_GBE_mode_repousse_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Entrepot_GBE_mode_repousse_univVars_Perso_ID_passengers_list,
            (void*)Garde_Barriere_Entrepot_GBE_mode_repousse_univVars,
            936);
        Garde_Barriere_Entrepot_GBE_mode_repousse__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Garde_Barriere_Entrepot_GBE_mode_repousse_TXT_Sally_gao = AI_ul_RealResolveOneRefObject(0x7200064Eu);
        fct_Garde_Barriere_Entrepot_GBE_mode_repousse_Garde_Barriere_Entrepot_GBE_mode_repousse = (int)AI_ul_RealResolveOneRefFunction(0x720026E8u);
        fct_Garde_Barriere_Entrepot_GBE_mode_repousse_Garde_Barriere_Entrepot_GBE_mode_attente = (int)AI_ul_RealResolveOneRefFunction(0x720026E7u);
        fct_Garde_Barriere_Entrepot_GBE_mode_repousse_Garde_Barriere_Entrepot_GBE_exec_camera = (int)AI_ul_RealResolveOneRefFunction(0x72002703u);
        fct_Garde_Barriere_Entrepot_GBE_mode_repousse_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning = (int)AI_ul_RealResolveOneRefFunction(0x72004A10u);
    }
    if (v103->i_current_mode != 2)
    {
        v103->i_current_mode = 2;
        v103->m_msg.msgID = 32;
        v103->m_msg.gao = AI_gpst_CurrentGameObject;
        v103->m_msg.gao2 = 0;
        v103->m_msg.Int1 = 0;
        v103->m_msg.Int2 = 0;
        v103->m_msg.f2 = 1.0;
        v103->m_msg.f1 = 0.0;
        v103->m_msg.v1.x = 0.0;
        v103->m_msg.v1.y = 0.0;
        v103->m_msg.v1.z = 0.0;
        v103->m_msg.v2.x = 0.0;
        v103->m_msg.v2.y = 0.0;
        v103->m_msg.v2.z = 0.0;
        AI_EvalFunc_MSGSend_C((tGameObjectMain*)Garde_Barriere_Entrepot_GBE_mode_repousse__PJ_Aircraft_gao, &v103->m_msg);
        if (v103->i_nb_wp)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v46);
            v116 = AI_EvalFunc_ARRObjNearestOfPoint_C((tGameObjectMain**)v103->des_wp_repoussing, v103->i_nb_wp, &v46);
            v37 = (tGameObjectMain*)v103->des_wp_repoussing[v116];
            if (v37)
            {
                if ((unsigned int)v37 <= 2)
                    v37 = AI_MainActor[(uint32_t)v37 - 1];
                AI_EvalFunc_OBJGetPos_C(v37, &v103->v_dest_pos);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v103->v_dest_pos);
            }
            v103->v_dest_pos.z = v103->v_my_pos.z;
        }
        v103->f_time_change_mode = TIM_gf_MainClock - TIM_gf_LockTime;
        v36 = (tGameObjectMain*)v103->des_o_acteur_texte;
        if (v36)
        {
            if ((unsigned int)v36 <= 2)
                v36 = AI_MainActor[(uint32_t)v36 - 1];
            v35 = v36;
        }
        else
        {
            v35 = 0;
        }
        if (v35)
        {
            if (AI_EvalFunc_TIMEElapsed_C(v103->f_time_affiche_texte, 5.0))
            {
                if ((*(uint32_t*)(Garde_Barriere_Entrepot_GBE_mode_repousse_univVars_Perso_ID_passengers_list + 44) & 1) != 0)
                {
                    v116 = v103->des_i_num_1st_phrase;
                    v0 = AI_EvalFunc_MATHRandInt_C(0, v103->des_i_nb_phrases);
                    v116 += v0;
                    Src.msgID = 30;
                    Src.Int1 = v116;
                    Src.Int2 = 100;
                    Src.f2 = 3.0;
                    Src.v1.x = 0.0;
                    Src.v1.y = 0.0;
                    Src.v1.z = 0.0;
                    Src.v2.x = 0.0;
                    Src.v2.y = 0.0;
                    Src.v2.z = 0.0;
                    Src.gao = 0;
                    Src.gao2 = 0;
                    v34 = (tGameObjectMain*)v103->des_o_acteur_texte;
                    if (v34)
                    {
                        if ((unsigned int)v34 <= 2)
                            v34 = AI_MainActor[(uint32_t)v34 - 1];
                        AI_EvalFunc_MSGSend_C(v34, &Src);
                    }
                    else
                    {
                        AI_EvalFunc_MSGSend_C(0, &Src);
                    }
                }
            }
        }
    }
    v33 = (tGameObjectMain*)v103->o_aircraft;
    if (v33)
    {
        if ((unsigned int)v33 <= 2)
            v33 = AI_MainActor[(uint32_t)v33 - 1];
        AI_EvalFunc_OBJGetPos_C(v33, &v83);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v83);
    }
    v32 = (tGameObjectMain*)v103->des_o_wp;
    if (v32)
    {
        if ((unsigned int)v32 <= 2)
            v32 = AI_MainActor[(uint32_t)v32 - 1];
        AI_EvalFunc_OBJGetPos_C(v32, &v48);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v48);
    }
    MATH_SubstractVec(&dest, &v83, &v48);
    a1.x = dest.x;
    a1.y = dest.y;
    a1.z = 0.0;
    v1 = MATH_VecLen(&a1);
    v103->f_dist = (float)v1;
    v31 = 1.0f / v103->f_dist;
    a1.x = a1.x * v31;
    a1.y = a1.y * v31;
    a1.z = a1.z * v31;
    if (v103->i_flag_stay_in_cone)
    {
        AI_EvalFunc_MATHVecInCone_C(&a1, &v103->v_cone_axe, v103->f_cone_angle, 0, &v60);
        a1 = v60;
    }
    if (AI_EvalFunc_TIMEElapsed_C(v103->f_time_change_mode, v103->des_f_time))
    {
        v30 = (tGameObjectMain*)v103->des_o_wp;
        if (v30)
        {
            if ((unsigned int)v30 <= 2)
                v30 = AI_MainActor[(uint32_t)v30 - 1];
            AI_EvalFunc_OBJGetPos_C(v30, &v45);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v45);
        }
        v61 = v45;
        if (v103->des_mode)
        {
            v29 = v103->des_f_dist_min;
            v72 = a1.x * v29;
            v73 = a1.y * v29;
            v74 = a1.z * v29;
            v61.x = v61.x + v72;
            v61.y = v61.y + v73;
            v61.z = v61.z + v74;
        }
        else
        {
            v28 = v103->des_f_dist_max;
            v89 = a1.x * v28;
            v90 = a1.y * v28;
            v91 = a1.z * v28;
            v61.x = v61.x + v89;
            v61.y = v61.y + v90;
            v61.z = v61.z + v91;
        }
        v61.z = v103->v_my_pos.z;
        AI_EvalFunc_OBJSetPos_C_CURRENT(&v61);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v82);
        v62 = a1.z * v82.z + a1.y * v82.y + v82.x * a1.x;
        if (v103->des_mode)
            v62 = v62 * -1.0f;
        if (v62 < 0.0)
        {
            v87 = MATH_VecLen(&v103->v_last_grav_speed);
            v63 = MATH_VecLen(&v103->v_grav_speed);
            if (v87 > (double)v63)
            {
                v103->v_last_grav_speed.x = 0.0;
                v103->v_last_grav_speed.y = 0.0;
                v103->v_last_grav_speed.z = 0.0;
                v103->i_flag_helper = 1;
                Src.msgID = 30;
                Src.Int1 = 39;
                Src.Int2 = 4900;
                Src.f2 = 1.0;
                Src.v1.x = 0.0;
                Src.v1.y = 0.0;
                Src.v1.z = 0.0;
                Src.v2.x = 0.0;
                Src.v2.y = 0.0;
                Src.v2.z = 0.0;
                Src.gao = 0;
                Src.gao2 = (tGameObjectMain*)Garde_Barriere_Entrepot_GBE_mode_repousse__PJ_Sally_gao;
                AI_EvalFunc_MSGSend_C((tGameObjectMain*)Garde_Barriere_Entrepot_GBE_mode_repousse_TXT_Sally_gao, &Src);
                if (v103->i_snd_ID != -1)
                {
                    SND_StopRq(v103->i_snd_ID);
                    v103->i_snd_ID = -1;
                }
                v112 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(2, -1.0);
                v80 = AI_gpst_CurrentGameObject;
                v27 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                        v27 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                    AI_EvalFunc_AICBDel_C(
                        AI_gpst_CurrentGameObject,
                        v27,
                        2,
                        (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_repousse_Garde_Barriere_Entrepot_GBE_mode_repousse);
                }
                else
                {
                    AI_EvalFunc_AICBDel_C(
                        AI_gpst_CurrentGameObject,
                        0,
                        2,
                        (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_repousse_Garde_Barriere_Entrepot_GBE_mode_repousse);
                }
                v104 = AI_gpst_CurrentGameObject;
                v26 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                        v26 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                    AI_EvalFunc_AICBAdd_C(
                        AI_gpst_CurrentGameObject,
                        v26,
                        2,
                        (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_repousse_Garde_Barriere_Entrepot_GBE_mode_attente);
                }
                else
                {
                    AI_EvalFunc_AICBAdd_C(
                        AI_gpst_CurrentGameObject,
                        0,
                        2,
                        (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_repousse_Garde_Barriere_Entrepot_GBE_mode_attente);
                }
            }
        }
        v24 = &v103->v_last_grav_speed;
        v25 = &v103->v_grav_speed;
        v103->v_last_grav_speed.x = v103->v_grav_speed.x;
        v24->y = v25->y;
        v24->z = v25->z;
    }
    else
    {
        v79 = TIM_gf_MainClock - TIM_gf_LockTime;
        v64 = (v79 - v103->f_time_change_mode) / v103->des_f_time;
        v64 = v64 * v64;
        if (!v103->i_nb_wp)
        {
            v23 = (tGameObjectMain*)v103->des_o_wp;
            if (v23)
            {
                if ((unsigned int)v23 <= 2)
                    v23 = AI_MainActor[(uint32_t)v23 - 1];
                AI_EvalFunc_OBJGetPos_C(v23, &v103->v_dest_pos);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v103->v_dest_pos);
            }
            if (v103->des_mode)
            {
                v22 = v103->des_f_dist_min;
                v108 = a1.x * v22;
                v109 = a1.y * v22;
                v110 = a1.z * v22;
                v21 = &v103->v_dest_pos;
                v103->v_dest_pos.x = v108 + v103->v_dest_pos.x;
                v21->y = v109 + v21->y;
                v21->z = v110 + v21->z;
            }
            else
            {
                v20 = v103->des_f_dist_max;
                v52 = a1.x * v20;
                v53 = a1.y * v20;
                v54 = a1.z * v20;
                v19 = &v103->v_dest_pos;
                v103->v_dest_pos.x = v52 + v103->v_dest_pos.x;
                v19->y = v53 + v19->y;
                v19->z = v54 + v19->z;
            }
            v103->v_dest_pos.z = v103->v_my_pos.z;
        }
        AI_EvalFunc_MATHVecBlend_C(&v103->v_my_pos, &v103->v_dest_pos, v64, &v115);
        v61 = v115;
        AI_EvalFunc_OBJSetPos_C_CURRENT(&v61);
    }
    v96 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_repousse_Garde_Barriere_Entrepot_GBE_exec_camera);
    if (v96 != 1000)
        return v96;
    AI_EvalFunc_COLPosGet_C(AI_gpst_CurrentGameObject, 0, &v44);
    v78 = v44;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v55);
    v99 = 0;
    v100 = 0.0;
    v101 = v55.z - v103->v_last_pos.z;
    v18 = &v103->v_grav_pos;
    v103->v_grav_pos.x = v103->v_grav_pos.x + 0.0f;
    v18->y = v100 + v18->y;
    v18->z = v101 + v18->z;
    v81 = v103->v_grav_pos;
    v17 = v103->des_f_air_friction;
    v105 = v17 * v103->v_grav_speed.x;
    v106 = v103->v_grav_speed.y * v17;
    v107 = v103->v_grav_speed.z * v17;
    v85 = TIM_gf_dt;
    v66 = v105 * TIM_gf_dt;
    v67 = v106 * TIM_gf_dt;
    v68 = v107 * TIM_gf_dt;
    v16 = &v103->v_grav_pos;
    v103->v_grav_pos.x = v66 + v103->v_grav_pos.x;
    v16->y = v67 + v16->y;
    v16->z = v68 + v16->z;
    v92 = 0;
    v93 = 0;
    v94 = v103->des_f_air_gravity;
    v59 = TIM_gf_dt;
    v69 = 0.0f * TIM_gf_dt;
    v70 = v69;
    v71 = v94 * TIM_gf_dt;
    v15 = &v103->v_grav_pos;
    v103->v_grav_pos.x = v69 + v103->v_grav_pos.x;
    v15->y = v70 + v15->y;
    v15->z = v71 + v15->z;
    MATH_SubstractVec(&v103->v_grav_pos, &v103->v_grav_pos, &v78);
    AI_EvalFunc_MATHVecSetNormVector_C(&v103->v_grav_pos, v103->des_f_dist_centre_grav);
    v14 = &v103->v_grav_pos;
    v103->v_grav_pos.x = v78.x + v103->v_grav_pos.x;
    v14->y = v78.y + v14->y;
    v14->z = v78.z + v14->z;
    MATH_SubstractVec(&v98, &v103->v_grav_pos, &v81);
    v50 = TIM_gf_dt;
    MATH_VecMultNormScalar(&v97, &v98, TIM_gf_dt);
    v103->v_grav_speed = v97;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v103->v_last_pos);
    MATH_SubstractVec(&v56, &v78, &v103->v_grav_pos);
    AI_EvalFunc_MATHVecNormalize_C(&v56, &v77);
    a3[0] = v77;
    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&a2);
    v75 = TIM_gf_dt;
    a4 = 8.0f * TIM_gf_dt;
    AI_EvalFunc_MATHVecBlendRotate_C(&a2, a3, a4, &a5);
    a3[0] = a5;
    AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a1, a3);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v102);
    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v41);
    v84.x = v41.x * 15.0f;
    v84.y = v41.y * 15.0f;
    v84.z = v41.z * 15.0f;
    MATH_SubstractVec(&v49, &v102, &v84);
    v103->v_aircraft_pos = v49;
    v13 = (tGameObjectMain*)v103->o_aircraft;
    if (v13)
    {
        if ((unsigned int)v13 <= 2)
            v13 = AI_MainActor[(uint32_t)v13 - 1];
        AI_EvalFunc_OBJSetPos_C(v13, &v103->v_aircraft_pos);
    }
    else
    {
        AI_EvalFunc_OBJSetPos_C(0, &v103->v_aircraft_pos);
    }
    v12 = (tGameObjectMain*)v103->o_aircraft;
    if (v12)
    {
        if ((unsigned int)v12 <= 2)
            v12 = AI_MainActor[(uint32_t)v12 - 1];
        v3 = AI_EvalFunc_DYNSpeedSetVector_C(v12, &v103->v_grav_speed);
    }
    else
    {
        v3 = AI_EvalFunc_DYNSpeedSetVector_C(0, &v103->v_grav_speed);
    }
    v86 = v3;
    v39.x = 0.0;
    v39.y = 0.0;
    v39.z = 0.0;
    v11 = (tGameObjectMain*)v103->o_aircraft;
    if (v11)
    {
        if ((unsigned int)v11 <= 2)
            v11 = AI_MainActor[(uint32_t)v11 - 1];
        AI_EvalFunc_DYNTractionSet_C(v11, &v39);
    }
    else
    {
        AI_EvalFunc_DYNTractionSet_C(0, &v39);
    }
    v57.x = 0.0;
    v57.y = 0.0;
    v57.z = 0.0;
    v10 = (tGameObjectMain*)v103->o_aircraft;
    if (v10)
    {
        if ((unsigned int)v10 <= 2)
            v10 = AI_MainActor[(uint32_t)v10 - 1];
        AI_EvalFunc_DYNStreamSet_C(v10, &v57);
    }
    else
    {
        AI_EvalFunc_DYNStreamSet_C(0, &v57);
    }
    if (v103->des_mode)
    {
        v42.x = -a1.x;
        v42.y = -a1.y;
        v42.z = -a1.z;
        v111.x = 0.0;
        v111.y = 0.0;
        v111.z = 1.0;
        v9 = (tGameObjectMain*)v103->o_aircraft;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            AI_EvalFunc_OBJ_BankingGeneralSet_C(v9, &v42, &v111);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v42, &v111);
        }
    }
    else
    {
        v43.x = 0.0;
        v43.y = 0.0;
        v43.z = 1.0;
        v8 = (tGameObjectMain*)v103->o_aircraft;
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v8 = AI_MainActor[(uint32_t)v8 - 1];
            AI_EvalFunc_OBJ_BankingGeneralSet_C(v8, &a1, &v43);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &a1, &v43);
        }
    }
    v7 = (tGameObjectMain*)v103->o_aircraft;
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
    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v65);
    v6 = (tGameObjectMain*)v103->o_aircraft_visuel;
    if (v6)
    {
        if ((unsigned int)v6 <= 2)
            v6 = AI_MainActor[(uint32_t)v6 - 1];
        AI_EvalFunc_OBJ_BankingGeneralSet_C(v6, &v40, &v65);
    }
    else
    {
        AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v40, &v65);
    }
    v103->f_time_keep_camera = TIM_gf_MainClock - TIM_gf_LockTime;
    v95 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_repousse_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning);
    if (v95 == 1000)
        result = 1000;
    else
        result = v95;
    return result;
}

int Garde_Barriere_Entrepot_GBE_exec_GFX_anneaux()
{
    int result; // eax
    tScriptStruct_PNJGardeBarriereEntrepot* v1; // [esp+8h] [ebp-ACh]
    tGameObjectMain* v2; // [esp+18h] [ebp-9Ch]
    Vec3 a2; // [esp+1Ch] [ebp-98h] BYREF
    Vec3 v4; // [esp+28h] [ebp-8Ch] BYREF
    Vec3 v5; // [esp+34h] [ebp-80h] BYREF
    Vec3 dest; // [esp+40h] [ebp-74h] BYREF
    Vec3 v7; // [esp+4Ch] [ebp-68h] BYREF
    Vec3 v9; // [esp+5Ch] [ebp-58h] BYREF
    Vec3 v10; // [esp+68h] [ebp-4Ch] BYREF
    Vec3 v11; // [esp+74h] [ebp-40h] BYREF
    Vec3 v12; // [esp+80h] [ebp-34h] BYREF
    tScriptStruct_PNJGardeBarriereEntrepot* v13; // [esp+94h] [ebp-20h]
    int a1; // [esp+98h] [ebp-1Ch]
    Vec3 v15; // [esp+9Ch] [ebp-18h] BYREF
    Vec3 a3; // [esp+A8h] [ebp-Ch] BYREF

    v9.x = 1.0;
    v9.y = 0.0;
    v9.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJGardeBarriereEntrepot*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v13 = v1;
    a1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Garde_Barriere_Entrepot_GBE_exec_GFX_anneaux_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    if (AI_EvalFunc_TIMEElapsed_C(v13->f_time_GFX_anneaux, 0.1f))
    {
        v13->f_time_GFX_anneaux = TIM_gf_MainClock - TIM_gf_LockTime;
        a1 = AI_EvalFunc_GFXAdd_C_CURRENT(13);
        v2 = (tGameObjectMain*)Garde_Barriere_Entrepot_GBE_exec_GFX_anneaux_SFX_light_smoke_gao;
        if (Garde_Barriere_Entrepot_GBE_exec_GFX_anneaux_SFX_light_smoke_gao)
        {
            if ((unsigned int)Garde_Barriere_Entrepot_GBE_exec_GFX_anneaux_SFX_light_smoke_gao <= 2)
                v2 = AI_MainActor[Garde_Barriere_Entrepot_GBE_exec_GFX_anneaux_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, v2, -1);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, 0, -1);
        }
        AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 2, 1);
        GFXSetCurGlobal(a1);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13000, 0.050000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13001, 0.2f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13002, 0.0099999998f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13003, 0.1f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13004, 0.30000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13005, 0.40000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13006, 0.60000002f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13007, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.00019999999f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13009, 30.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13010, 30.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13012, 0.0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 50);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13101, 0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13103, 0x4080C080);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13104, 0x8080C080);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13105, 0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13106, 50);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13107, 0);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v4);
        v9.x = v4.x * 40.0f;
        v9.y = v4.y * 40.0f;
        v9.z = v4.z * 40.0f;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v10);
        a3.x = v10.x * 20.0f;
        a3.y = v10.y * 20.0f;
        a3.z = v10.z * 20.0f;
        MATH_SubstractVec(&dest, &a2, &a3);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13200, &dest);
        v12.x = -1.0;
        v12.y = -1.0;
        v12.z = 0.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v12);
        v15.x = 1.0;
        v15.y = 1.0;
        v15.z = 0.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v15);
        v7.x = 0.2f;
        v7.y = 0.2f;
        v7.z = 0.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v7);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13204, &v9);
        v11.x = 0.0;
        v11.y = 0.0;
        v11.z = 0.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13205, &v11);
        v5.x = 0.0;
        v5.y = 0.0;
        v5.z = 0.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13206, &v5);
        result = 1000;
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning()
{
    tScriptStruct_PNJGardeBarriereEntrepot* v1; // [esp+8h] [ebp-138h]
    tGameObjectMain* v2; // [esp+10h] [ebp-130h]
    tGameObjectMain* v3; // [esp+18h] [ebp-128h]
    tGameObjectMain* v4; // [esp+20h] [ebp-120h]
    tGameObjectMain* v5; // [esp+30h] [ebp-110h]
    tGameObjectMain* v6; // [esp+38h] [ebp-108h]
    tGameObjectMain* v7; // [esp+40h] [ebp-100h]
    tGameObjectMain* v8; // [esp+48h] [ebp-F8h]
    tGameObjectMain* v9; // [esp+58h] [ebp-E8h]
    tGameObjectMain* v10; // [esp+60h] [ebp-E0h]
    float v11; // [esp+68h] [ebp-D8h]
    float v12; // [esp+6Ch] [ebp-D4h]
    float v13; // [esp+70h] [ebp-D0h]
    float v14; // [esp+78h] [ebp-C8h]
    float v15; // [esp+78h] [ebp-C8h]
    float v16; // [esp+84h] [ebp-BCh]
    float v17; // [esp+88h] [ebp-B8h]
    float v18; // [esp+8Ch] [ebp-B4h]
    Vec3 v19; // [esp+90h] [ebp-B0h] BYREF
    Vec3 a1; // [esp+9Ch] [ebp-A4h] BYREF
    Vec3 v22; // [esp+ACh] [ebp-94h] BYREF
    Vec3 v23; // [esp+B8h] [ebp-88h] BYREF
    Vec3 a3; // [esp+C4h] [ebp-7Ch] BYREF
    Vec3 v25; // [esp+D0h] [ebp-70h] BYREF
    Vec3 v26; // [esp+E4h] [ebp-5Ch] BYREF
    float v27; // [esp+F0h] [ebp-50h]
    Vec3 v28; // [esp+F4h] [ebp-4Ch] BYREF
    Vec3 v29; // [esp+100h] [ebp-40h] BYREF
    tGameObjectMain* v30; // [esp+10Ch] [ebp-34h]
    tGameObjectMain* v31; // [esp+110h] [ebp-30h]
    tScriptStruct_PNJGardeBarriereEntrepot* v32; // [esp+114h] [ebp-2Ch]
    int v33; // [esp+118h] [ebp-28h]
    int i; // [esp+11Ch] [ebp-24h]
    Vec3 v35; // [esp+120h] [ebp-20h] BYREF
    Vec3 v36; // [esp+12Ch] [ebp-14h] BYREF

    v29.x = 1.0;
    v29.y = 0.0;
    v29.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJGardeBarriereEntrepot*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v32 = v1;
    v33 = 0;
    v30 = 0;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning = (int)AI_ul_RealResolveOneRefFunction(0x72004A10u);
        Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    if (v32->i_flag_lightning)
    {
        v32->f_time_gfx_lighning = TIM_gf_MainClock - TIM_gf_LockTime;
        if (v32->des_mode)
            v15 = (v32->f_dist - v32->des_f_dist_min) / v32->f_dist_range;
        else
            v15 = (v32->des_f_dist_max - v32->f_dist) / v32->f_dist_range;
        v32->c_lighning_color = AI_EvalFunc_COLORBlend_C(0, -4128832, v15);
        for (i = 0; i < 3; ++i)
        {
            if (v32->GFX_Lightning[i] == -1)
            {
                v32->GFX_Lightning[i] = AI_EvalFunc_GFXAdd_C_CURRENT(2);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v32->GFX_Lightning[i], 0, 1);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v32->GFX_Lightning[i], 2, 1);
                v6 = (tGameObjectMain*)Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning_SFX_light_smoke_gao;
                if (Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning_SFX_light_smoke_gao)
                {
                    if ((unsigned int)Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning_SFX_light_smoke_gao <= 2)
                        v6 = AI_MainActor[Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v32->GFX_Lightning[i], v6, 20);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v32->GFX_Lightning[i], 0, 20);
                }
                GFXSetCurGlobal(v32->GFX_Lightning[i]);
                AI_EvalFunc_GFXSeti_C_CURGLOB(2101, 2);
                AI_EvalFunc_GFXSeti_C_CURGLOB(2100, 16);
                AI_EvalFunc_GFXSetf_C_CURGLOB(2001, 0.5);
                AI_EvalFunc_GFXSetf_C_CURGLOB(2000, 0.5);
            }
            GFXSetCurGlobal(v32->GFX_Lightning[i]);
            AI_EvalFunc_GFXSeti_C_CURGLOB(2102, v32->c_lighning_color);
            v30 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(i + 9);
            v5 = v30;
            if (v30)
            {
                if ((unsigned int)v30 <= 2)
                    v5 = AI_MainActor[(uint32_t)v30 - 1];
                AI_EvalFunc_OBJGetPos_C(v5, &v19);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v19);
            }
            v29 = v19;
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v23);
            v11 = v23.x * 3.0f;
            v12 = v23.y * 3.0f;
            v13 = v23.z * 3.0f;
            v29.x = v29.x - v11;
            v29.y = v29.y - v12;
            v29.z = v29.z - v13;
            AI_EvalFunc_GFXSetv_C_CURGLOB(2200, &v29);
            v4 = (tGameObjectMain*)v32->o_aircraft;
            if (v4)
            {
                if ((unsigned int)v4 <= 2)
                    v4 = AI_MainActor[(uint32_t)v4 - 1];
                AI_EvalFunc_OBJGetPos_C(v4, &v22);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v22);
            }
            v3 = (tGameObjectMain*)v32->o_aircraft_visuel;
            if (v3)
            {
                if ((unsigned int)v3 <= 2)
                    v3 = AI_MainActor[(uint32_t)v3 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v3, &v28);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v28);
            }
            MATH_VecAddVec(&v25, &v22, &v28);
            AI_EvalFunc_GFXSetv_C_CURGLOB(2201, &v25);
            if (v32->GFX_Halo[i] == -1)
            {
                v32->GFX_Halo[i] = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v32->GFX_Halo[i], 4, 1);
                v2 = (tGameObjectMain*)Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning_SFX_light_smoke_gao;
                if (Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning_SFX_light_smoke_gao)
                {
                    if ((unsigned int)Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning_SFX_light_smoke_gao <= 2)
                        v2 = AI_MainActor[Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v32->GFX_Halo[i], v2, 29);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v32->GFX_Halo[i], 0, 29);
                }
                GFXSetCurGlobal(v32->GFX_Halo[i]);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 2.0);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 5.0);
            }
            GFXSetCurGlobal(v32->GFX_Halo[i]);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v32->c_lighning_color);
            AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v29);
        }
    }
    else if (AI_EvalFunc_TIMEElapsed_C(v32->f_time_gfx_lighning, 1.0))
    {
        if (v32->GFX_Lightning[0] != -1)
        {
            v32->c_lighning_color = 0;
            for (i = 0; i < 3; ++i)
            {
                AI_EvalFunc_GFXDel_C_CURRENT(v32->GFX_Lightning[i]);
                v32->GFX_Lightning[i] = -1;
                AI_EvalFunc_GFXDel_C_CURRENT(v32->GFX_Halo[i]);
                v32->GFX_Halo[i] = -1;
            }
            if (v32->i_current_mode == 3)
            {
                v31 = AI_gpst_CurrentGameObject;
                v10 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                        v10 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                    AI_EvalFunc_AICBDel_C(
                        AI_gpst_CurrentGameObject,
                        v10,
                        2,
                        (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning);
                }
                else
                {
                    AI_EvalFunc_AICBDel_C(
                        AI_gpst_CurrentGameObject,
                        0,
                        2,
                        (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning);
                }
            }
        }
    }
    else
    {
        v27 = TIM_gf_MainClock - TIM_gf_LockTime;
        v14 = v27 - v32->f_time_gfx_lighning;
        v33 = AI_EvalFunc_COLORBlend_C(v32->c_lighning_color, 0, v14);
        for (i = 0; i < 3; ++i)
        {
            GFXSetCurGlobal(v32->GFX_Lightning[i]);
            AI_EvalFunc_GFXSeti_C_CURGLOB(2102, v33);
            v30 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(i + 9);
            v9 = v30;
            if (v30)
            {
                if ((unsigned int)v30 <= 2)
                    v9 = AI_MainActor[(uint32_t)v30 - 1];
                AI_EvalFunc_OBJGetPos_C(v9, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            v29 = a3;
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v36);
            v16 = v36.x * 3.0f;
            v17 = v36.y * 3.0f;
            v18 = v36.z * 3.0f;
            v29.x = v29.x - v16;
            v29.y = v29.y - v17;
            v29.z = v29.z - v18;
            AI_EvalFunc_GFXSetv_C_CURGLOB(2200, &v29);
            v8 = (tGameObjectMain*)v32->o_aircraft;
            if (v8)
            {
                if ((unsigned int)v8 <= 2)
                    v8 = AI_MainActor[(uint32_t)v8 - 1];
                AI_EvalFunc_OBJGetPos_C(v8, &v26);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v26);
            }
            v7 = (tGameObjectMain*)v32->o_aircraft_visuel;
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v7, &v35);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v35);
            }
            MATH_VecAddVec(&a1, &v26, &v35);
            AI_EvalFunc_GFXSetv_C_CURGLOB(2201, &a1);
            AI_EvalFunc_GFXSeti_C_CURRENT(v32->GFX_Halo[i], 21101, v32->c_lighning_color);
            AI_EvalFunc_GFXSetv_C_CURRENT(v32->GFX_Halo[i], 21200, &v29);
        }
    }
    return 1000;
}

int Garde_Barriere_Entrepot_GBE_mode_choppe()
{
    int result; // eax
    double v1; // st7
    tGameObjectMain* v2; // eax
    float a4; // [esp+4h] [ebp-560h]
    float a4a; // [esp+4h] [ebp-560h]
    float X; // [esp+8h] [ebp-55Ch]
    tScriptStruct_PNJGardeBarriereEntrepot* v6; // [esp+Ch] [ebp-558h]
    tGameObjectMain* v7; // [esp+14h] [ebp-550h]
    tGameObjectMain* v8; // [esp+1Ch] [ebp-548h]
    tGameObjectMain* v9; // [esp+24h] [ebp-540h]
    tGameObjectMain* v10; // [esp+2Ch] [ebp-538h]
    tGameObjectMain* v11; // [esp+34h] [ebp-530h]
    tGameObjectMain* v12; // [esp+44h] [ebp-520h]
    tGameObjectMain* v13; // [esp+4Ch] [ebp-518h]
    tGameObjectMain* v14; // [esp+64h] [ebp-500h]
    tGameObjectMain* v15; // [esp+6Ch] [ebp-4F8h]
    tGameObjectMain* v16; // [esp+74h] [ebp-4F0h]
    tGameObjectMain* v17; // [esp+7Ch] [ebp-4E8h]
    tGameObjectMain* v18; // [esp+84h] [ebp-4E0h]
    tGameObjectMain* v19; // [esp+94h] [ebp-4D0h]
    tGameObjectMain* v20; // [esp+9Ch] [ebp-4C8h]
    tGameObjectMain* v21; // [esp+A4h] [ebp-4C0h]
    tGameObjectMain* v22; // [esp+ACh] [ebp-4B8h]
    tGameObjectMain* v23; // [esp+B4h] [ebp-4B0h]
    tGameObjectMain* v24; // [esp+BCh] [ebp-4A8h]
    tGameObjectMain* v25; // [esp+C4h] [ebp-4A0h]
    tGameObjectMain* v26; // [esp+DCh] [ebp-488h]
    Vec3* v27; // [esp+100h] [ebp-464h]
    Vec3* v28; // [esp+104h] [ebp-460h]
    Vec3* v29; // [esp+108h] [ebp-45Ch]
    Vec3* v30; // [esp+118h] [ebp-44Ch]
    Vec3* v31; // [esp+12Ch] [ebp-438h]
    tGameObjectMain* v32; // [esp+134h] [ebp-430h]
    float v33; // [esp+138h] [ebp-42Ch]
    tGameObjectMain* v34; // [esp+140h] [ebp-424h]
    tGameObjectMain* v35; // [esp+148h] [ebp-41Ch]
    float v36; // [esp+158h] [ebp-40Ch]
    tGameObjectMain* v37; // [esp+16Ch] [ebp-3F8h]
    tGameObjectMain* v38; // [esp+17Ch] [ebp-3E8h]
    tGameObjectMain* v39; // [esp+184h] [ebp-3E0h]
    tGameObjectMain* v40; // [esp+18Ch] [ebp-3D8h]
    Vec3 v41; // [esp+190h] [ebp-3D4h] BYREF
    Vec3 v42; // [esp+19Ch] [ebp-3C8h] BYREF
    Vec3 v43; // [esp+1A8h] [ebp-3BCh] BYREF
    Vec3 v44; // [esp+1B4h] [ebp-3B0h] BYREF
    Vec3 v45; // [esp+1C0h] [ebp-3A4h] BYREF
    Vec3 v46; // [esp+1CCh] [ebp-398h] BYREF
    float v47; // [esp+1D8h] [ebp-38Ch]
    float v48; // [esp+1DCh] [ebp-388h]
    Vec3 v49; // [esp+1E4h] [ebp-380h] BYREF
    Vec3 v50; // [esp+1FCh] [ebp-368h] BYREF
    Vec3 v51; // [esp+214h] [ebp-350h] BYREF
    int v52; // [esp+22Ch] [ebp-338h]
    tMessage Src; // [esp+230h] [ebp-334h] BYREF
    Vec3 v54; // [esp+264h] [ebp-300h] BYREF
    int v55; // [esp+270h] [ebp-2F4h]
    float v56; // [esp+274h] [ebp-2F0h]
    float v57; // [esp+278h] [ebp-2ECh]
    Vec3 a1; // [esp+288h] [ebp-2DCh] BYREF
    int v59; // [esp+294h] [ebp-2D0h]
    Vec3 v60; // [esp+298h] [ebp-2CCh] BYREF
    int v61; // [esp+2A4h] [ebp-2C0h]
    tGameObjectMain* v62; // [esp+2A8h] [ebp-2BCh]
    Vec3 v63; // [esp+2ACh] [ebp-2B8h] BYREF
    Vec3 v64; // [esp+2B8h] [ebp-2ACh] BYREF
    Vec3 v65; // [esp+2C4h] [ebp-2A0h] BYREF
    Vec3 v66; // [esp+2D0h] [ebp-294h] BYREF
    Vec3 a3; // [esp+2F8h] [ebp-26Ch] BYREF
    Vec3 dest; // [esp+304h] [ebp-260h] BYREF
    float v69; // [esp+314h] [ebp-250h]
    Vec3 v70; // [esp+318h] [ebp-24Ch] BYREF
    Vec3 v71; // [esp+324h] [ebp-240h] BYREF
    unsigned int v72; // [esp+330h] [ebp-234h]
    int v73; // [esp+334h] [ebp-230h]
    Vec3 v74; // [esp+338h] [ebp-22Ch] BYREF
    Vec3 v75; // [esp+344h] [ebp-220h] BYREF
    Vec3 v76; // [esp+350h] [ebp-214h] BYREF
    int v77; // [esp+35Ch] [ebp-208h]
    int v78; // [esp+360h] [ebp-204h]
    int v79; // [esp+364h] [ebp-200h]
    int v80; // [esp+368h] [ebp-1FCh]
    int v81; // [esp+36Ch] [ebp-1F8h]
    int v82; // [esp+370h] [ebp-1F4h]
    float v83; // [esp+374h] [ebp-1F0h]
    float v84; // [esp+378h] [ebp-1ECh]
    float v85; // [esp+37Ch] [ebp-1E8h]
    float v86; // [esp+380h] [ebp-1E4h]
    float v87; // [esp+384h] [ebp-1E0h]
    Vec3 v88; // [esp+394h] [ebp-1D0h] BYREF
    int v89; // [esp+3A0h] [ebp-1C4h]
    int v90; // [esp+3A4h] [ebp-1C0h]
    int v91; // [esp+3A8h] [ebp-1BCh]
    Vec3 v92; // [esp+3ACh] [ebp-1B8h] BYREF
    float v93; // [esp+3B8h] [ebp-1ACh]
    Vec3 a2; // [esp+3BCh] [ebp-1A8h] BYREF
    Vec3 v95; // [esp+3C8h] [ebp-19Ch] BYREF
    Vec3 v96; // [esp+3D4h] [ebp-190h] BYREF
    Vec3 v97; // [esp+3E0h] [ebp-184h] BYREF
    Vec3 v98; // [esp+3ECh] [ebp-178h] BYREF
    float v99; // [esp+3F8h] [ebp-16Ch]
    float v100; // [esp+400h] [ebp-164h]
    tGameObjectMain* v101; // [esp+404h] [ebp-160h]
    Vec3 v102; // [esp+414h] [ebp-150h] BYREF
    Vec3 v103; // [esp+420h] [ebp-144h] BYREF
    Vec3 v104; // [esp+42Ch] [ebp-138h] BYREF
    Vec3 v105; // [esp+438h] [ebp-12Ch] BYREF
    Vec3 v106; // [esp+448h] [ebp-11Ch] BYREF
    float v107; // [esp+460h] [ebp-104h]
    float v108; // [esp+464h] [ebp-100h]
    float v109; // [esp+468h] [ebp-FCh]
    Vec3 v110; // [esp+46Ch] [ebp-F8h] BYREF
    float v111; // [esp+478h] [ebp-ECh]
    float v112; // [esp+47Ch] [ebp-E8h]
    float v113; // [esp+480h] [ebp-E4h]
    Vec3 a5; // [esp+484h] [ebp-E0h] BYREF
    Vec3 v115; // [esp+490h] [ebp-D4h] BYREF
    float v116; // [esp+49Ch] [ebp-C8h]
    Vec3 v117; // [esp+4A0h] [ebp-C4h] BYREF
    Vec3 v118; // [esp+4ACh] [ebp-B8h] BYREF
    Vec3 v119; // [esp+4C4h] [ebp-A0h] BYREF
    float v120; // [esp+4D0h] [ebp-94h]
    float v121; // [esp+4D4h] [ebp-90h]
    float v122; // [esp+4DCh] [ebp-88h]
    float v123; // [esp+4E0h] [ebp-84h]
    float v124; // [esp+4E4h] [ebp-80h]
    tScriptStruct_PNJGardeBarriereEntrepot* v125; // [esp+4E8h] [ebp-7Ch]
    Vec3 v126; // [esp+4ECh] [ebp-78h] BYREF
    float v127; // [esp+4F8h] [ebp-6Ch]
    float v128; // [esp+4FCh] [ebp-68h]
    float v129; // [esp+500h] [ebp-64h]
    float v130; // [esp+504h] [ebp-60h]
    float v131; // [esp+508h] [ebp-5Ch]
    float v132; // [esp+50Ch] [ebp-58h]
    Vec3 v133; // [esp+510h] [ebp-54h] BYREF
    Vec3 v134; // [esp+51Ch] [ebp-48h] BYREF
    Vec3 v135; // [esp+534h] [ebp-30h] BYREF
    Vec3 v136; // [esp+540h] [ebp-24h] BYREF
    Vec3 v137; // [esp+54Ch] [ebp-18h] BYREF
    float v139; // [esp+55Ch] [ebp-8h]
    float v140; // [esp+560h] [ebp-4h]

    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    v96.x = 1.0;
    v96.y = 0.0;
    v96.z = 0.0;
    v106.x = 1.0;
    v106.y = 0.0;
    v106.z = 0.0;
    v71.x = 1.0;
    v71.y = 0.0;
    v71.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    v92.x = 1.0;
    v92.y = 0.0;
    v92.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_PNJGardeBarriereEntrepot*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v125 = v6;
    v47 = 0.0;
    v72 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Garde_Barriere_Entrepot_GBE_mode_choppe__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        Garde_Barriere_Entrepot_GBE_mode_choppe__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Garde_Barriere_Entrepot_GBE_mode_choppe_TXT_Sally_gao = AI_ul_RealResolveOneRefObject(0x7200064Eu);
        fct_Garde_Barriere_Entrepot_GBE_mode_choppe_Garde_Barriere_Entrepot_GBE_exec_camera = (int)AI_ul_RealResolveOneRefFunction(0x72002703u);
        fct_Garde_Barriere_Entrepot_GBE_mode_choppe_Garde_Barriere_Entrepot_GBE_mode_choppe = (int)AI_ul_RealResolveOneRefFunction(0x72002701u);
        fct_Garde_Barriere_Entrepot_GBE_mode_choppe_Garde_Barriere_Entrepot_GBE_mode_repousse = (int)AI_ul_RealResolveOneRefFunction(0x720026E8u);
        fct_Garde_Barriere_Entrepot_GBE_mode_choppe_Garde_Barriere_Entrepot_GBE_exec_GFX_anneaux = (int)AI_ul_RealResolveOneRefFunction(0x72004A0Eu);
        fct_Garde_Barriere_Entrepot_GBE_mode_choppe_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning = (int)AI_ul_RealResolveOneRefFunction(0x72004A10u);
    }
    if (v125->i_current_mode == 1)
    {
        v38 = (tGameObjectMain*)v125->o_aircraft;
        if (v38)
        {
            if ((unsigned int)v38 <= 2)
                v38 = AI_MainActor[(uint32_t)v38 - 1];
            AI_EvalFunc_OBJGetPos_C(v38, &v105);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v105);
        }
        v71 = v105;
        v37 = (tGameObjectMain*)v125->des_o_wp;
        if (v37)
        {
            if ((unsigned int)v37 <= 2)
                v37 = AI_MainActor[(uint32_t)v37 - 1];
            AI_EvalFunc_OBJGetPos_C(v37, &v126);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v126);
        }
        MATH_SubstractVec(&dest, &v71, &v126);
        a1.x = dest.x;
        a1.y = dest.y;
        a1.z = 0.0;
        v1 = MATH_VecLen(&a1);
        v125->f_dist = (float)v1;
        v36 = 1.0f / v125->f_dist;
        a1.x = a1.x * v36;
        a1.y = a1.y * v36;
        a1.z = a1.z * v36;
        if (v125->i_flag_stay_in_cone)
        {
            AI_EvalFunc_MATHVecInCone_C(&a1, &v125->v_cone_axe, v125->f_cone_angle, 0, &v60);
            a1 = v60;
        }
        v137.x = 0.0;
        v137.y = 0.0;
        v137.z = 0.0;
        v35 = (tGameObjectMain*)v125->o_aircraft;
        if (v35)
        {
            if ((unsigned int)v35 <= 2)
                v35 = AI_MainActor[(uint32_t)v35 - 1];
            AI_EvalFunc_DYNTractionSet_C(v35, &v137);
        }
        else
        {
            AI_EvalFunc_DYNTractionSet_C(0, &v137);
        }
        v95.x = 0.0;
        v95.y = 0.0;
        v95.z = 0.0;
        v34 = (tGameObjectMain*)v125->o_aircraft;
        if (v34)
        {
            if ((unsigned int)v34 <= 2)
                v34 = AI_MainActor[(uint32_t)v34 - 1];
            AI_EvalFunc_DYNStreamSet_C(v34, &v95);
        }
        else
        {
            AI_EvalFunc_DYNStreamSet_C(0, &v95);
        }
        v69 = TIM_gf_dt;
        v33 = TIM_gf_dt;
        if (TIM_gf_dt < 0.0)
            v33 = 0.0;
        if (v33 > 1.0)
            v33 = 1.0;
        v84 = 8.0f * v33 + v125->f_choppe_blend_speed * (1.0f - v33);
        v125->f_choppe_blend_speed = v84;
        v32 = (tGameObjectMain*)v125->o_aircraft;
        if (v32)
        {
            if ((unsigned int)v32 <= 2)
                v32 = AI_MainActor[(uint32_t)v32 - 1];
            AI_EvalFunc_OBJGetPos_C(v32, &v125->v_my_pos);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v125->v_my_pos);
        }
        v80 = 0;
        v81 = 0;
        v82 = 1065353216;
        v111 = 0.0 * 20.0;
        v112 = v111;
        v113 = 1.0 * 20.0;
        v31 = &v125->v_my_pos;
        v125->v_my_pos.x = v111 + v125->v_my_pos.x;
        v31->y = v112 + v31->y;
        v31->z = v113 + v31->z;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v97);
        v139 = TIM_gf_dt;
        a4 = v125->f_choppe_blend_speed * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlend_C(&v97, &v125->v_my_pos, a4, &v103);
        v125->v_my_pos = v103;
        AI_EvalFunc_OBJSetPos_C_CURRENT(&v125->v_my_pos);
        v73 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_choppe_Garde_Barriere_Entrepot_GBE_exec_camera);
        if (v73 != 1000)
            return v73;
        AI_EvalFunc_COLPosGet_C(AI_gpst_CurrentGameObject, 0, &v70);
        v106 = v70;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v110);
        v55 = 0;
        v56 = 0.0;
        v57 = v110.z - v125->v_last_pos.z;
        v30 = &v125->v_grav_pos;
        v125->v_grav_pos.x = v125->v_grav_pos.x + 0.0f;
        v30->y = v56 + v30->y;
        v30->z = v57 + v30->z;
        v92 = v125->v_grav_pos;
        v127 = v125->v_grav_speed.x * 0.85000002f;
        v128 = v125->v_grav_speed.y * 0.85000002f;
        v129 = v125->v_grav_speed.z * 0.85000002f;
        v93 = TIM_gf_dt;
        v122 = v127 * TIM_gf_dt;
        v123 = v128 * TIM_gf_dt;
        v124 = v129 * TIM_gf_dt;
        v29 = &v125->v_grav_pos;
        v125->v_grav_pos.x = v122 + v125->v_grav_pos.x;
        v29->y = v123 + v29->y;
        v29->z = v124 + v29->z;
        v89 = 0;
        v90 = 0;
        v91 = -1041235968;
        v99 = TIM_gf_dt;
        v130 = 0.0f * TIM_gf_dt;
        v131 = v130;
        v132 = -30.0f * TIM_gf_dt;
        v28 = &v125->v_grav_pos;
        v125->v_grav_pos.x = v130 + v125->v_grav_pos.x;
        v28->y = v131 + v28->y;
        v28->z = v132 + v28->z;
        MATH_SubstractVec(&v125->v_grav_pos, &v125->v_grav_pos, &v106);
        AI_EvalFunc_MATHVecSetNormVector_C(&v125->v_grav_pos, v125->des_f_dist_centre_grav);
        v27 = &v125->v_grav_pos;
        v125->v_grav_pos.x = v106.x + v125->v_grav_pos.x;
        v27->y = v106.y + v27->y;
        v27->z = v106.z + v27->z;
        MATH_SubstractVec(&v135, &v125->v_grav_pos, &v92);
        v116 = TIM_gf_dt;
        MATH_VecMultNormScalar(&v49, &v135, TIM_gf_dt);
        v125->v_grav_speed = v49;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v125->v_last_pos);
        MATH_SubstractVec(&v119, &v106, &v125->v_grav_pos);
        AI_EvalFunc_MATHVecNormalize_C(&v119, &v133);
        a3 = v133;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&a2);
        v83 = TIM_gf_dt;
        a4a = 8.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4a, &a5);
        a3 = a5;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a1, &a3);
        if (v125->i_flag_vert_align_ok
            || (AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v64),
                v77 = 0,
                v78 = 0,
                v79 = 1065353216,
                v140 = 1.0f * v64.z + 0.0f * v64.y + v64.x * 0.0f,
                v140 > 0.98480803)
            && (v121 = MATH_VecLen(&v125->v_grav_speed), v121 < 10.0))
        {
            v125->i_flag_vert_align_ok = 1;
            if (!v125->i_snd_play)
            {
                v125->i_snd_play = 1;
                v61 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(0, -1.0);
            }
            v26 = (tGameObjectMain*)v125->o_aircraft;
            if (v26)
            {
                if ((unsigned int)v26 <= 2)
                    v26 = AI_MainActor[(uint32_t)v26 - 1];
                v2 = AI_EvalFunc_ANICanalGaoGet_C(v26, 0);
            }
            else
            {
                v2 = AI_EvalFunc_ANICanalGaoGet_C(0, 0);
            }
            v72 = (unsigned int)v2;
            if (AI_EvalFunc_TIMEElapsed_C(v125->f_time_change_mode, 0.5))
            {
                v96 = v125->v_my_pos;
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v63);
                v85 = v63.x * 15.0f;
                v86 = v63.y * 15.0f;
                v87 = v63.z * 15.0f;
                v96.x = v96.x - v85;
                v96.y = v96.y - v86;
                v96.z = v96.z - v87;
                if (v125->des_mode)
                {
                    v66.x = -a1.x;
                    v66.y = -a1.y;
                    v66.z = -a1.z;
                    v76.x = 0.0;
                    v76.y = 0.0;
                    v76.z = 1.0;
                    v25 = (tGameObjectMain*)v125->o_aircraft;
                    if (v25)
                    {
                        if ((unsigned int)v25 <= 2)
                            v25 = AI_MainActor[(uint32_t)v25 - 1];
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(v25, &v66, &v76);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v66, &v76);
                    }
                }
                else
                {
                    v41.x = 0.0;
                    v41.y = 0.0;
                    v41.z = 1.0;
                    v24 = (tGameObjectMain*)v125->o_aircraft;
                    if (v24)
                    {
                        if ((unsigned int)v24 <= 2)
                            v24 = AI_MainActor[(uint32_t)v24 - 1];
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(v24, &a1, &v41);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &a1, &v41);
                    }
                }
                v23 = (tGameObjectMain*)v125->o_aircraft;
                if (v23)
                {
                    if ((unsigned int)v23 <= 2)
                        v23 = AI_MainActor[(uint32_t)v23 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v23, &v117);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v117);
                }
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v46);
                v22 = (tGameObjectMain*)v72;
                if (v72)
                {
                    if (v72 <= 2)
                        v22 = AI_MainActor[v72 - 1];
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(v22, &v117, &v46);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v117, &v46);
                }
                v21 = (tGameObjectMain*)v125->o_aircraft;
                if (v21)
                {
                    if ((unsigned int)v21 <= 2)
                        v21 = AI_MainActor[(uint32_t)v21 - 1];
                    AI_EvalFunc_OBJSetPos_C(v21, &v96);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &v96);
                }
                v101 = AI_gpst_CurrentGameObject;
                v20 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                        v20 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                    AI_EvalFunc_AICBDel_C(
                        AI_gpst_CurrentGameObject,
                        v20,
                        2,
                        (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_choppe_Garde_Barriere_Entrepot_GBE_mode_choppe);
                }
                else
                {
                    AI_EvalFunc_AICBDel_C(
                        AI_gpst_CurrentGameObject,
                        0,
                        2,
                        (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_choppe_Garde_Barriere_Entrepot_GBE_mode_choppe);
                }
                v62 = AI_gpst_CurrentGameObject;
                v19 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                        v19 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                    AI_EvalFunc_AICBAdd_C(
                        AI_gpst_CurrentGameObject,
                        v19,
                        2,
                        (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_choppe_Garde_Barriere_Entrepot_GBE_mode_repousse);
                }
                else
                {
                    AI_EvalFunc_AICBAdd_C(
                        AI_gpst_CurrentGameObject,
                        0,
                        2,
                        (tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_choppe_Garde_Barriere_Entrepot_GBE_mode_repousse);
                }
            }
            else
            {
                v100 = TIM_gf_MainClock - TIM_gf_LockTime;
                v47 = (v100 - v125->f_time_change_mode) / 0.5f;
                X = v47 * 1.570796f;
                v47 = (float)sinf(X);
                if (v125->des_mode)
                {
                    v65.x = -a1.x;
                    v65.y = -a1.y;
                    v65.z = -a1.z;
                    AI_EvalFunc_MATHVecBlend_C(&v125->v_aircraft_sight, &v65, v47, &v42);
                    v134.x = 0.0;
                    v134.y = 0.0;
                    v134.z = 1.0;
                    v18 = (tGameObjectMain*)v125->o_aircraft;
                    if (v18)
                    {
                        if ((unsigned int)v18 <= 2)
                            v18 = AI_MainActor[(uint32_t)v18 - 1];
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(v18, &v42, &v134);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v42, &v134);
                    }
                }
                else
                {
                    AI_EvalFunc_MATHVecBlend_C(&v125->v_aircraft_sight, &a1, v47, &v50);
                    v45.x = 0.0;
                    v45.y = 0.0;
                    v45.z = 1.0;
                    v17 = (tGameObjectMain*)v125->o_aircraft;
                    if (v17)
                    {
                        if ((unsigned int)v17 <= 2)
                            v17 = AI_MainActor[(uint32_t)v17 - 1];
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(v17, &v50, &v45);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v50, &v45);
                    }
                }
                v16 = (tGameObjectMain*)v125->o_aircraft;
                if (v16)
                {
                    if ((unsigned int)v16 <= 2)
                        v16 = AI_MainActor[(uint32_t)v16 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v16, &v74);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v74);
                }
                v15 = (tGameObjectMain*)v72;
                if (v72)
                {
                    if (v72 <= 2)
                        v15 = AI_MainActor[v72 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v15, &v43);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v43);
                }
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v115);
                AI_EvalFunc_MATHVecBlend_C(&v43, &v115, v47, &v75);
                v14 = (tGameObjectMain*)v72;
                if (v72)
                {
                    if (v72 <= 2)
                        v14 = AI_MainActor[v72 - 1];
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(v14, &v74, &v75);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v74, &v75);
                }
                v96 = v125->v_my_pos;
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v104);
                v107 = v104.x * 15.0f;
                v108 = v104.y * 15.0f;
                v109 = v104.z * 15.0f;
                v96.x = v96.x - v107;
                v96.y = v96.y - v108;
                v96.z = v96.z - v109;
                AI_EvalFunc_MATHVecBlend_C(&v125->v_aircraft_pos, &v96, v47, &v118);
                v96 = v118;
                v13 = (tGameObjectMain*)v125->o_aircraft;
                if (v13)
                {
                    if ((unsigned int)v13 <= 2)
                        v13 = AI_MainActor[(uint32_t)v13 - 1];
                    AI_EvalFunc_OBJSetPos_C(v13, &v96);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &v96);
                }
                v52 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_choppe_Garde_Barriere_Entrepot_GBE_exec_GFX_anneaux);
                if (v52 != 1000)
                    return v52;
            }
        }
        else
        {
            if (v125->des_mode)
            {
                v12 = (tGameObjectMain*)v125->o_aircraft;
                if (v12)
                {
                    if ((unsigned int)v12 <= 2)
                        v12 = AI_MainActor[(uint32_t)v12 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v12, &v102);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v102);
                }
                v88.x = -a1.x;
                v88.y = -a1.y;
                v88.z = -a1.z;
                v120 = TIM_gf_dt;
                AI_EvalFunc_MATHVecBlend_C(&v102, &v88, TIM_gf_dt, &v98);
                v54.x = 0.0;
                v54.y = 0.0;
                v54.z = 1.0;
                v11 = (tGameObjectMain*)v125->o_aircraft;
                if (v11)
                {
                    if ((unsigned int)v11 <= 2)
                        v11 = AI_MainActor[(uint32_t)v11 - 1];
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(v11, &v98, &v54);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v98, &v54);
                }
            }
            else
            {
                v10 = (tGameObjectMain*)v125->o_aircraft;
                if (v10)
                {
                    if ((unsigned int)v10 <= 2)
                        v10 = AI_MainActor[(uint32_t)v10 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v10, &v44);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v44);
                }
                v48 = TIM_gf_dt;
                AI_EvalFunc_MATHVecBlend_C(&v44, &a1, TIM_gf_dt, &v136);
                v51.x = 0.0;
                v51.y = 0.0;
                v51.z = 1.0;
                v9 = (tGameObjectMain*)v125->o_aircraft;
                if (v9)
                {
                    if ((unsigned int)v9 <= 2)
                        v9 = AI_MainActor[(uint32_t)v9 - 1];
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(v9, &v136, &v51);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v136, &v51);
                }
            }
            v125->f_time_change_mode = TIM_gf_MainClock - TIM_gf_LockTime;
            v8 = (tGameObjectMain*)v125->o_aircraft;
            if (v8)
            {
                if ((unsigned int)v8 <= 2)
                    v8 = AI_MainActor[(uint32_t)v8 - 1];
                AI_EvalFunc_OBJGetPos_C(v8, &v125->v_aircraft_pos);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v125->v_aircraft_pos);
            }
            v7 = (tGameObjectMain*)v125->o_aircraft;
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v7, &v125->v_aircraft_sight);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v125->v_aircraft_sight);
            }
        }
        v59 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Garde_Barriere_Entrepot_GBE_mode_choppe_Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning);
        if (v59 == 1000)
            result = 1000;
        else
            result = v59;
    }
    else
    {
        v125->i_current_mode = 1;
        v125->i_flag_vert_align_ok = 0;
        v125->i_flag_capture_done = 1;
        v125->m_msg.msgID = 32;
        v125->m_msg.gao = AI_gpst_CurrentGameObject;
        v125->m_msg.gao2 = 0;
        v125->m_msg.Int1 = 0;
        v125->m_msg.Int2 = 0;
        v125->m_msg.f2 = 0.0;
        v125->m_msg.f1 = 0.0;
        v125->m_msg.v1.x = 0.0;
        v125->m_msg.v1.y = 0.0;
        v125->m_msg.v1.z = 0.0;
        v125->m_msg.v2.x = 0.0;
        v125->m_msg.v2.y = 0.0;
        v125->m_msg.v2.z = 0.0;
        AI_EvalFunc_MSGSend_C((tGameObjectMain*)Garde_Barriere_Entrepot_GBE_mode_choppe__PJ_Aircraft_gao, &v125->m_msg);
        Src.msgID = 30;
        Src.Int1 = 38;
        Src.Int2 = 4900;
        Src.f2 = 2.0;
        Src.v1.x = 0.0;
        Src.v1.y = 0.0;
        Src.v1.z = 0.0;
        Src.v2.x = 0.0;
        Src.v2.y = 0.0;
        Src.v2.z = 0.0;
        Src.gao = 0;
        Src.gao2 = (tGameObjectMain*)Garde_Barriere_Entrepot_GBE_mode_choppe__PJ_Sally_gao;
        AI_EvalFunc_MSGSend_C((tGameObjectMain*)Garde_Barriere_Entrepot_GBE_mode_choppe_TXT_Sally_gao, &Src);
        v40 = (tGameObjectMain*)v125->o_aircraft;
        if (v40)
        {
            if ((unsigned int)v40 <= 2)
                v40 = AI_MainActor[(uint32_t)v40 - 1];
            AI_EvalFunc_OBJGetPos_C(v40, &v125->v_aircraft_pos);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v125->v_aircraft_pos);
        }
        v39 = (tGameObjectMain*)v125->o_aircraft;
        if (v39)
        {
            if ((unsigned int)v39 <= 2)
                v39 = AI_MainActor[(uint32_t)v39 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v39, &v125->v_aircraft_sight);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &v125->v_aircraft_sight);
        }
        AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        v125->f_time_change_mode = TIM_gf_MainClock - TIM_gf_LockTime;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 4;
    }
    return result;
}