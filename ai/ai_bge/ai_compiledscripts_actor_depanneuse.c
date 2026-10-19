#include "ai_compiledscripts_inc.h"

//depaneuse_dp_exec_afterrec
int depaneuse_dp_exec_afterrec_SFX_light_smoke_gao;

//depaneuse_dp_toaircraftreel
int depaneuse_dp_toaircraftreel_GST_Board_panel_gao;
int fct_depaneuse_dp_toaircraftreel_depaneuse_dp_retour;
int fct_depaneuse_dp_toaircraftreel_depaneuse_dp_donnegrapin;

//depaneuse_dp_texte
int depaneuse_dp_texte_GST_Board_panel_gao;
int depaneuse_dp_texte_GST_Board_panel_gao_Gen_Show;
int depaneuse_dp_texte__PJ_Camera_gao;
int depaneuse_dp_texte__PJ_Camera_gao_des_i_dynplan;
int depaneuse_dp_texte__PJ_Camera_gao_i_lastdynplan;
int depaneuse_dp_texte__PJ_Camera_gao_des_f_timedynplan;
int depaneuse_dp_texte__PJ_Camera_gao_des_i_contrechampdynplan;
int depaneuse_dp_texte__PJ_Camera_gao_des_o_viseedynplan;
int depaneuse_dp_texte__PJ_Camera_gao_des_f_zdynplan;
int depaneuse_dp_texte__PJ_Camera_gao_des_f_distdynplan;
int depaneuse_dp_texte__PJ_Camera_gao_des_v_offsetviseedynplan;
int depaneuse_dp_texte__PJ_Camera_gao_des_v_sightdynplan;
int depaneuse_dp_texte__PJ_Camera_gao_des_i_dynplanviseedyn;
int depaneuse_dp_texte__PJ_Camera_gao_i_must_cut;

//depaneuse_dp_donnegrapin
int depaneuse_dp_donnegrapin_univVars;
int depaneuse_dp_donnegrapin_univVars_Flag_speciaux_sauve;
int fct_depaneuse_dp_donnegrapin_depaneuse_dp_retour;

//depaneuse_dp_retour
int depaneuse_dp_retour_univVars;
int depaneuse_dp_retour_univVars_Perso_ID_equipement;
int depaneuse_dp_retour_univVars_objet_inventaire;
int depaneuse_dp_retour_univVars_Flag_speciaux_sauve;
int depaneuse_dp_retour_INV_manager_gao;
int depaneuse_dp_retour_univVars_i_indice_de_map;
int depaneuse_dp_retour_GST_Global_gao;
int depaneuse_dp_retour_GST_Global_gao_i_fade_priority;
int depaneuse_dp_retour_GST_Global_gao_f_fade_start_time;
int depaneuse_dp_retour_GST_Global_gao_f_fade_delay;
int depaneuse_dp_retour_INV_MatFade_gao;
int depaneuse_dp_retour_GST_Board_panel_gao;
int depaneuse_dp_retour_GST_Board_panel_gao_Gen_Show;
int depaneuse_dp_retour_univVars_i_special_init;
int depaneuse_dp_retour_univVars_load_world;
int depaneuse_dp_retour_univVars_world_ID;
int depaneuse_dp_retour_univVars_world_entrance_ID;

//depaneuse_dp_toaircraft
int depaneuse_dp_toaircraft_GST_Board_panel_gao;
int fct_depaneuse_dp_toaircraft_depaneuse_dp_donnegrapin;
int fct_depaneuse_dp_toaircraft_depaneuse_dp_toaircraftreel;

//depaneuse_dp_init
int depaneuse_dp_init_univVars;
int depaneuse_dp_init_univVars_Flag_speciaux_sauve;
int depaneuse_dp_init_univVars_Perso_ID_actor;
int depaneuse_dp_init_SFX_light_smoke_gao;
int fct_depaneuse_dp_init_depaneuse_dp_exec_afterrec;
int depaneuse_dp_init_GST_Board_panel_gao;
int depaneuse_dp_init_depaneuse_dp_toaircraft;

//depaneuse_dp_reflex
int depaneuse_dp_reflex_GST_Board_panel_gao;
int depaneuse_dp_reflex_univVars;
int depaneuse_dp_reflex_univVars_Flag_speciaux_sauve;
int depaneuse_dp_reflex_GST_Global_gao;
int depaneuse_dp_reflex_GST_Global_gao_o_micro;

int depaneuse_dp_exec_afterrec()
{
    BOOL v0; // eax
    tGameObjectMain* v1; // eax
    tScriptStruct_ActorDepanneuse* v3; // [esp+0h] [ebp-154h]
    tGameObjectMain* v4; // [esp+8h] [ebp-14Ch]
    tGameObjectMain* v5; // [esp+18h] [ebp-13Ch]
    tGameObjectMain* v6; // [esp+20h] [ebp-134h]
    tGameObjectMain* v7; // [esp+28h] [ebp-12Ch]
    tGameObjectMain* v8; // [esp+30h] [ebp-124h]
    tGameObjectMain* v9; // [esp+38h] [ebp-11Ch]
    tGameObjectMain* v10; // [esp+3Ch] [ebp-118h]
    tGameObjectMain* v11; // [esp+40h] [ebp-114h]
    Vec3 v12; // [esp+44h] [ebp-110h] BYREF
    Vec3 v13; // [esp+50h] [ebp-104h] BYREF
    Vec3 a1; // [esp+5Ch] [ebp-F8h] BYREF
    Vec3 v15; // [esp+68h] [ebp-ECh] BYREF
    Vec3 v16; // [esp+74h] [ebp-E0h] BYREF
    Vec3 v17; // [esp+80h] [ebp-D4h] BYREF
    Vec3 v18; // [esp+8Ch] [ebp-C8h] BYREF
    Vec3 v19; // [esp+98h] [ebp-BCh] BYREF
    Vec3 v20; // [esp+A4h] [ebp-B0h] BYREF
    unsigned int v21; // [esp+B0h] [ebp-A4h]
    Vec3 v22; // [esp+B4h] [ebp-A0h] BYREF
    Vec3 v23; // [esp+C0h] [ebp-94h] BYREF
    Vec3 v24; // [esp+CCh] [ebp-88h] BYREF
    Vec3 a3; // [esp+D8h] [ebp-7Ch] BYREF
    Vec3 v26; // [esp+E4h] [ebp-70h] BYREF
    Vec3 v27; // [esp+F0h] [ebp-64h] BYREF
    float v28; // [esp+FCh] [ebp-58h]
    float v29; // [esp+100h] [ebp-54h]
    float v30; // [esp+104h] [ebp-50h]
    Vec3 v31; // [esp+108h] [ebp-4Ch] BYREF
    BOOL v32; // [esp+114h] [ebp-40h]
    Vec3 v33; // [esp+11Ch] [ebp-38h] BYREF
    tScriptStruct_ActorDepanneuse* v34; // [esp+128h] [ebp-2Ch]
    Vec3 v35; // [esp+12Ch] [ebp-28h] BYREF
    Vec3 v36; // [esp+13Ch] [ebp-18h] BYREF
    Vec3 v37; // [esp+148h] [ebp-Ch] BYREF

    v15.x = 1.0;
    v15.y = 0.0;
    v15.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_ActorDepanneuse*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v34 = v3;
    v21 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        depaneuse_dp_exec_afterrec_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v18);
    v33.x = 4.0;
    v33.y = 2.0;
    v33.z = 0.69999999f;
    AI_EvalFunc_MATHVecLocalToGlobal_C(AI_gpst_CurrentGameObject, &v33, &v31);
    MATH_VecAddVec(&a1, &v18, &v31);
    AI_EvalFunc_GFXSetv_C_CURRENT(v34->i_idhaloL, 21200, &a1);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v13);
    v24.x = -4.0;
    v24.y = 2.0;
    v24.z = 0.69999999f;
    AI_EvalFunc_MATHVecLocalToGlobal_C(AI_gpst_CurrentGameObject, &v24, &v35);
    MATH_VecAddVec(&v17, &v13, &v35);
    AI_EvalFunc_GFXSetv_C_CURRENT(v34->i_idhaloR, 21200, &v17);
    v11 = (tGameObjectMain*)v34->o_ventouse;
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
    if (!v10)
        goto LABEL_31;
    v9 = (tGameObjectMain*)v34->o_ventouse;
    if (v9)
    {
        if ((unsigned int)v9 <= 2)
            v9 = AI_MainActor[(uint32_t)v9 - 1];
        v0 = AI_EvalFunc_OBJ_CapaTest_C(v9, 0x100);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x100);
    }
    v32 = v0;
    if (v0)
    {
        if (v34->GFX_Line == -1)
        {
            v34->GFX_Line = AI_EvalFunc_GFXAdd_C_CURRENT(1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v34->GFX_Line, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v34->GFX_Line, 2, 1);
            v8 = (tGameObjectMain*)depaneuse_dp_exec_afterrec_SFX_light_smoke_gao;
            if (depaneuse_dp_exec_afterrec_SFX_light_smoke_gao)
            {
                if ((unsigned int)depaneuse_dp_exec_afterrec_SFX_light_smoke_gao <= 2)
                    v8 = AI_MainActor[depaneuse_dp_exec_afterrec_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v34->GFX_Line, v8, 3);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v34->GFX_Line, 0, 3);
            }
            GFXSetCurGlobal(v34->GFX_Line);
            AI_EvalFunc_GFXSetf_C_CURGLOB(1000, 0.5);
            AI_EvalFunc_GFXSeti_C_CURGLOB(1100, -1);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v19);
        v27.x = 0.0;
        v27.y = -3.0;
        v27.z = 7.0;
        AI_EvalFunc_MATHVecLocalToGlobal_C(AI_gpst_CurrentGameObject, &v27, &v16);
        MATH_VecAddVec(&v37, &v19, &v16);
        GFXSetCurGlobal(v34->GFX_Line);
        AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &v37);
        v7 = (tGameObjectMain*)v34->o_ventouse;
        if (v7)
        {
            if ((unsigned int)v7 <= 2)
                v7 = AI_MainActor[(uint32_t)v7 - 1];
            AI_EvalFunc_OBJGetPos_C(v7, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &a3);
    }
    else
    {
    LABEL_31:
        if (v34->GFX_Line != -1)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v20);
            v23.x = 0.0;
            v23.y = -3.0;
            v23.z = 7.0;
            AI_EvalFunc_MATHVecLocalToGlobal_C(AI_gpst_CurrentGameObject, &v23, &v12);
            MATH_VecAddVec(&v22, &v20, &v12);
            GFXSetCurGlobal(v34->GFX_Line);
            AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &v22);
            v6 = (tGameObjectMain*)v34->o_aircraft;
            if (v6)
            {
                if ((unsigned int)v6 <= 2)
                    v6 = AI_MainActor[(uint32_t)v6 - 1];
                v1 = AI_EvalFunc_ANICanalGaoGet_C(v6, 0);
            }
            else
            {
                v1 = AI_EvalFunc_ANICanalGaoGet_C(0, 0);
            }
            v21 = (unsigned int)v1;
            v5 = v1;
            if (v1)
            {
                if ((unsigned int)v1 <= 2)
                    v5 = AI_MainActor[(uint32_t)v1 - 1];
                AI_EvalFunc_OBJGetPos_C(v5, &v26);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v26);
            }
            v15 = v26;
            v4 = (tGameObjectMain*)v21;
            if (v21)
            {
                if (v21 <= 2)
                    v4 = AI_MainActor[v21 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v4, &v36);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v36);
            }
            v28 = v36.x * 3.0f;
            v29 = v36.y * 3.0f;
            v30 = v36.z * 3.0f;
            v15.x = v15.x + v28;
            v15.y = v15.y + v29;
            v15.z = v15.z + v30;
            AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &v15);
        }
    }
    return 1000;
}

int depaneuse_dp_toaircraftreel()
{
    float v1; // [esp+0h] [ebp-A4h]
    tScriptStruct_ActorDepanneuse* v2; // [esp+4h] [ebp-A0h]
    tGameObjectMain* v3; // [esp+14h] [ebp-90h]
    float v4; // [esp+1Ch] [ebp-88h]
    tGameObjectMain* v5; // [esp+30h] [ebp-74h]
    float v6; // [esp+34h] [ebp-70h]
    Vec3 a1; // [esp+3Ch] [ebp-68h] BYREF
    int v8; // [esp+4Ch] [ebp-58h]
    Vec3 a3; // [esp+50h] [ebp-54h] BYREF
    Vec3 v10; // [esp+5Ch] [ebp-48h] BYREF
    float v11; // [esp+68h] [ebp-3Ch]
    Vec3 dest; // [esp+6Ch] [ebp-38h] BYREF
    Vec3 v13; // [esp+78h] [ebp-2Ch] BYREF
    Vec3 v14; // [esp+84h] [ebp-20h] BYREF
    tScriptStruct_ActorDepanneuse* v15; // [esp+90h] [ebp-14h]
    Vec3 v16; // [esp+94h] [ebp-10h] BYREF
    int v17; // [esp+A0h] [ebp-4h]

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorDepanneuse*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v15 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        depaneuse_dp_toaircraftreel_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        fct_depaneuse_dp_toaircraftreel_depaneuse_dp_retour = (int)AI_ul_RealResolveOneRefFunction(0x3D0021C7u);
        fct_depaneuse_dp_toaircraftreel_depaneuse_dp_donnegrapin = (int)AI_ul_RealResolveOneRefFunction(0x3D0021B6u);
    }
    AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)depaneuse_dp_toaircraftreel_GST_Board_panel_gao, 0x200, 0);
    AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)depaneuse_dp_toaircraftreel_GST_Board_panel_gao, 0x100, 0);
    v15->f_dest_speed = 100.0;
    v15->f_inertie_speed = 2.0;
    v5 = (tGameObjectMain*)v15->o_aircraft;
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
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v14);
    MATH_SubstractVec(&dest, &a3, &v14);
    a1 = dest;
    v6 = MATH_VecLen(&a1);
    v4 = v6 * 0.30000001f;
    if (v4 > 100.0)
        v1 = v6 * 0.30000001f;
    else
        v1 = 100.0;
    v15->f_dest_speed = v1;
    v11 = MATH_VecLen(&a1);
    if (v15->f_dist_aircraft_debut > (double)v11)
    {
        if (v15->i_grappin_donne)
        {
            v17 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_depaneuse_dp_toaircraftreel_depaneuse_dp_retour);
            if (v17 != 1000)
                return v17;
        }
        else
        {
            v8 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_depaneuse_dp_toaircraftreel_depaneuse_dp_donnegrapin);
            if (v8 != 1000)
                return v8;
        }
    }
    v3 = (tGameObjectMain*)v15->o_aircraft;
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        AI_EvalFunc_OBJGetPos_C(v3, &v13);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v13);
    }
    v16.x = 0.0;
    v16.y = 0.0;
    v16.z = 5.0;
    MATH_VecAddVec(&v10, &v13, &v16);
    v15->v_destpos = v10;
    return 1000;
}

int depaneuse_dp_texte()
{
    int result; // eax
    tGameObjectMain* v1; // eax
    tGameObjectMain* v2; // eax
    float v3; // [esp+4h] [ebp-418h]
    float v4; // [esp+8h] [ebp-414h]
    float v5; // [esp+Ch] [ebp-410h]
    float v6; // [esp+10h] [ebp-40Ch]
    float v7; // [esp+14h] [ebp-408h]
    float v8; // [esp+18h] [ebp-404h]
    tScriptStruct_ActorDepanneuse* v9; // [esp+20h] [ebp-3FCh]
    tGameObjectMain* v10; // [esp+28h] [ebp-3F4h]
    tGameObjectMain* v11; // [esp+30h] [ebp-3ECh]
    tGameObjectMain* v12; // [esp+40h] [ebp-3DCh]
    tGameObjectMain* v13; // [esp+48h] [ebp-3D4h]
    tGameObjectMain* v14; // [esp+50h] [ebp-3CCh]
    Vec3* v15; // [esp+58h] [ebp-3C4h]
    int v16; // [esp+74h] [ebp-3A8h]
    tGameObjectMain* v17; // [esp+90h] [ebp-38Ch]
    tGameObjectMain* v18; // [esp+98h] [ebp-384h]
    tGameObjectMain* v19; // [esp+A0h] [ebp-37Ch]
    tGameObjectMain* v20; // [esp+A8h] [ebp-374h]
    tGameObjectMain* v21; // [esp+B0h] [ebp-36Ch]
    tGameObjectMain* v22; // [esp+B8h] [ebp-364h]
    tGameObjectMain* v23; // [esp+C0h] [ebp-35Ch]
    tGameObjectMain* v24; // [esp+C8h] [ebp-354h]
    int v25; // [esp+D0h] [ebp-34Ch]
    float v26; // [esp+E0h] [ebp-33Ch]
    float v27; // [esp+E4h] [ebp-338h]
    float v28; // [esp+E8h] [ebp-334h]
    float v29; // [esp+ECh] [ebp-330h]
    tGameObjectMain* v30; // [esp+F4h] [ebp-328h]
    int v31; // [esp+FCh] [ebp-320h]
    float v32; // [esp+10Ch] [ebp-310h]
    float v33; // [esp+110h] [ebp-30Ch]
    float v34; // [esp+114h] [ebp-308h]
    float v35; // [esp+118h] [ebp-304h]
    int v36; // [esp+120h] [ebp-2FCh]
    float v37; // [esp+130h] [ebp-2ECh]
    float v38; // [esp+134h] [ebp-2E8h]
    float v39; // [esp+138h] [ebp-2E4h]
    float v40; // [esp+13Ch] [ebp-2E0h]
    Vec3* v41; // [esp+140h] [ebp-2DCh]
    tGameObjectMain* v42; // [esp+154h] [ebp-2C8h]
    Vec3 v43; // [esp+158h] [ebp-2C4h] BYREF
    Vec3 a3; // [esp+17Ch] [ebp-2A0h] BYREF
    Vec3 v45; // [esp+188h] [ebp-294h] BYREF
    int v46; // [esp+194h] [ebp-288h]
    Vec3 v47; // [esp+198h] [ebp-284h] BYREF
    Vec3 v48; // [esp+1A4h] [ebp-278h] BYREF
    float v49; // [esp+1B0h] [ebp-26Ch]
    tGameObjectMain* v50; // [esp+1B4h] [ebp-268h]
    Vec3 v51; // [esp+1B8h] [ebp-264h] BYREF
    float v52; // [esp+1C4h] [ebp-258h]
    float v53; // [esp+1C8h] [ebp-254h]
    tMessage Src; // [esp+1CCh] [ebp-250h] BYREF
    float v55; // [esp+218h] [ebp-204h]
    Vec3 v56; // [esp+228h] [ebp-1F4h] BYREF
    Vec3 v57; // [esp+234h] [ebp-1E8h] BYREF
    int v58; // [esp+24Ch] [ebp-1D0h]
    Vec3 a4; // [esp+258h] [ebp-1C4h] BYREF
    float v60; // [esp+26Ch] [ebp-1B0h]
    float v61; // [esp+270h] [ebp-1ACh]
    tGameObjectMain* v62; // [esp+274h] [ebp-1A8h]
    Vec3 dest; // [esp+29Ch] [ebp-180h] BYREF
    Vec3 v64; // [esp+2A8h] [ebp-174h] BYREF
    Vec3 v65; // [esp+2B4h] [ebp-168h] BYREF
    Vec3 v66; // [esp+2D8h] [ebp-144h] BYREF
    Vec3 v67; // [esp+2E4h] [ebp-138h] BYREF
    float v68; // [esp+2F0h] [ebp-12Ch]
    Vec3 v69; // [esp+30Ch] [ebp-110h] BYREF
    float v70; // [esp+318h] [ebp-104h]
    float v71; // [esp+31Ch] [ebp-100h]
    int v72; // [esp+320h] [ebp-FCh]
    int v73; // [esp+324h] [ebp-F8h]
    Vec3 v74; // [esp+334h] [ebp-E8h]
    Vec3 v75; // [esp+358h] [ebp-C4h] BYREF
    float v76; // [esp+374h] [ebp-A8h]
    Vec3 v77; // [esp+37Ch] [ebp-A0h] BYREF
    Vec3 v78; // [esp+394h] [ebp-88h] BYREF
    Vec3 v79; // [esp+3A0h] [ebp-7Ch] BYREF
    Vec3 v80; // [esp+3B0h] [ebp-6Ch] BYREF
    tScriptStruct_ActorDepanneuse* v81; // [esp+3BCh] [ebp-60h]
    Vec3 v82; // [esp+3C0h] [ebp-5Ch] BYREF
    tGameObjectMain* v83; // [esp+3E4h] [ebp-38h]
    float v84; // [esp+3E8h] [ebp-34h]
    Vec3 a1; // [esp+3FCh] [ebp-20h] BYREF
    Vec3 v86; // [esp+408h] [ebp-14h] BYREF
    float v87; // [esp+414h] [ebp-8h]
    tGameObjectMain* v88; // [esp+418h] [ebp-4h]

    v51.x = 1.0;
    v51.y = 0.0;
    v51.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v9 = (tScriptStruct_ActorDepanneuse*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v9 = 0;
    v81 = v9;
    v62 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        depaneuse_dp_texte_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &depaneuse_dp_texte_GST_Board_panel_gao_Gen_Show,
            (void*)depaneuse_dp_texte_GST_Board_panel_gao,
            2260);
        depaneuse_dp_texte__PJ_Camera_gao = AI_ul_RealResolveOneRefObject(0x71003F56u);
        AI2C_ResolveBuffer(&depaneuse_dp_texte__PJ_Camera_gao_des_i_dynplan, (void*)depaneuse_dp_texte__PJ_Camera_gao, 440);
        AI2C_ResolveBuffer(&depaneuse_dp_texte__PJ_Camera_gao_i_lastdynplan, (void*)depaneuse_dp_texte__PJ_Camera_gao, 532);
        AI2C_ResolveBuffer(
            &depaneuse_dp_texte__PJ_Camera_gao_des_f_timedynplan,
            (void*)depaneuse_dp_texte__PJ_Camera_gao,
            444);
        AI2C_ResolveBuffer(
            &depaneuse_dp_texte__PJ_Camera_gao_des_i_contrechampdynplan,
            (void*)depaneuse_dp_texte__PJ_Camera_gao,
            464);
        AI2C_ResolveBuffer(
            &depaneuse_dp_texte__PJ_Camera_gao_des_o_viseedynplan,
            (void*)depaneuse_dp_texte__PJ_Camera_gao,
            468);
        AI2C_ResolveBuffer(
            &depaneuse_dp_texte__PJ_Camera_gao_des_f_zdynplan,
            (void*)depaneuse_dp_texte__PJ_Camera_gao,
            472);
        AI2C_ResolveBuffer(
            &depaneuse_dp_texte__PJ_Camera_gao_des_f_distdynplan,
            (void*)depaneuse_dp_texte__PJ_Camera_gao,
            476);
        AI2C_ResolveBuffer(
            &depaneuse_dp_texte__PJ_Camera_gao_des_v_offsetviseedynplan,
            (void*)depaneuse_dp_texte__PJ_Camera_gao,
            448);
        AI2C_ResolveBuffer(
            &depaneuse_dp_texte__PJ_Camera_gao_des_v_sightdynplan,
            (void*)depaneuse_dp_texte__PJ_Camera_gao,
            480);
        AI2C_ResolveBuffer(
            &depaneuse_dp_texte__PJ_Camera_gao_des_i_dynplanviseedyn,
            (void*)depaneuse_dp_texte__PJ_Camera_gao,
            512);
        AI2C_ResolveBuffer(&depaneuse_dp_texte__PJ_Camera_gao_i_must_cut, (void*)depaneuse_dp_texte__PJ_Camera_gao, 144);
    }
    switch ((unsigned int)*AI_gppst_CurrentJumpNode)
    {
    case 2u:
        v81->f_dist_cam = 15.0;
        AI_gpst_CurrentInstance->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -0.80000001f;
        goto LABEL_16;
    case 4u:
    LABEL_16:
        if (AI_EvalKeyword_During_C())
        {
            v52 = TIM_gf_dt;
            v40 = 10.0f * TIM_gf_dt;
            v39 = 30.0f - v81->f_Z_cam;
            if (v39 > (double)v40)
                v8 = 10.0f * TIM_gf_dt;
            else
                v8 = 30.0f - v81->f_Z_cam;
            v49 = v8;
            v81->f_Z_cam = v8 + v81->f_Z_cam;
            v68 = TIM_gf_dt;
            v38 = 20.0f * TIM_gf_dt;
            v37 = 40.0f - v81->f_dist_cam;
            if (v37 > (double)v38)
                v7 = 20.0f * TIM_gf_dt;
            else
                v7 = 40.0f - v81->f_dist_cam;
            v84 = v7;
            v81->f_dist_cam = v7 + v81->f_dist_cam;
            v62 = *AI_MainActor;
            *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_i_dynplan = 1;
            *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_i_lastdynplan = 0;
            *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_f_timedynplan = 0x41100000;
            *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_i_contrechampdynplan = 0;
            *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_o_viseedynplan = (uint32_t)v62;
            *(float*)depaneuse_dp_texte__PJ_Camera_gao_des_f_zdynplan = v81->f_Z_cam;
            *(float*)depaneuse_dp_texte__PJ_Camera_gao_des_f_distdynplan = v81->f_dist_cam;
            *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_v_offsetviseedynplan = 0;
            *(uint32_t*)(depaneuse_dp_texte__PJ_Camera_gao_des_v_offsetviseedynplan + 4) = 0;
            *(uint32_t*)(depaneuse_dp_texte__PJ_Camera_gao_des_v_offsetviseedynplan + 8) = 0x40000000;
            v82.x = 0.0;
            v82.y = 0.0;
            v82.z = 1.0;
            AI_EvalFunc_MATHVecRotate_C(&v81->v_cam_axis, &v82, 0.40000001f, &a4);
            v51 = a4;
            v36 = depaneuse_dp_texte__PJ_Camera_gao_des_v_sightdynplan;
            *(float*)depaneuse_dp_texte__PJ_Camera_gao_des_v_sightdynplan = a4.x;
            *(float*)(v36 + 4) = v51.y;
            *(float*)(v36 + 8) = v51.z;
            *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_i_dynplanviseedyn = 0;
            *AI_gppst_CurrentJumpNode = (tAINode*)4;
            result = 0;
        }
        else
        {
            v70 = TIM_gf_dt;
            v35 = 10.0f * TIM_gf_dt;
            v34 = 30.0f - v81->f_Z_cam;
            if (v34 > (double)v35)
                v6 = 10.0f * TIM_gf_dt;
            else
                v6 = 30.0f - v81->f_Z_cam;
            v55 = v6;
            v81->f_Z_cam = v6 + v81->f_Z_cam;
            v87 = TIM_gf_dt;
            v33 = 20.0f * TIM_gf_dt;
            v32 = 40.0f - v81->f_dist_cam;
            if (v32 > (double)v33)
                v5 = 20.0f * TIM_gf_dt;
            else
                v5 = 40.0f - v81->f_dist_cam;
            v71 = v5;
            v81->f_dist_cam = v5 + v81->f_dist_cam;
            v62 = *AI_MainActor;
            *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_i_dynplan = 1;
            *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_i_lastdynplan = 0;
            *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_f_timedynplan = 0x41100000;
            *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_i_contrechampdynplan = 0;
            *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_o_viseedynplan = (uint32_t)v62;
            *(float*)depaneuse_dp_texte__PJ_Camera_gao_des_f_zdynplan = v81->f_Z_cam;
            *(float*)depaneuse_dp_texte__PJ_Camera_gao_des_f_distdynplan = v81->f_dist_cam;
            *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_v_offsetviseedynplan = 0;
            *(uint32_t*)(depaneuse_dp_texte__PJ_Camera_gao_des_v_offsetviseedynplan + 4) = 0;
            *(uint32_t*)(depaneuse_dp_texte__PJ_Camera_gao_des_v_offsetviseedynplan + 8) = 0x40000000;
            v48.x = 0.0;
            v48.y = 0.0;
            v48.z = 1.0;
            AI_EvalFunc_MATHVecRotate_C(&v81->v_cam_axis, &v48, 0.40000001f, &v65);
            v51 = v65;
            v31 = depaneuse_dp_texte__PJ_Camera_gao_des_v_sightdynplan;
            *(float*)depaneuse_dp_texte__PJ_Camera_gao_des_v_sightdynplan = v65.x;
            *(float*)(v31 + 4) = v51.y;
            *(float*)(v31 + 8) = v51.z;
            *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_i_dynplanviseedyn = 0;
            v81->f_surround = SND_f_Getf(2);
            v73 = SND_i_Setf(2, 1.0);
            Src.msgID = 30;
            Src.Int1 = 0;
            Src.Int2 = 4502;
            Src.f2 = 20.0;
            Src.v1.x = 0.0;
            Src.v1.y = 0.0;
            Src.v1.z = 0.0;
            Src.v2.x = 0.0;
            Src.v2.y = 0.0;
            Src.v2.z = 0.0;
            Src.gao = 0;
            Src.gao2 = 0;
            v30 = (tGameObjectMain*)v81->des_o_text;
            if (v30)
            {
                if ((unsigned int)v30 <= 2)
                    v30 = AI_MainActor[(uint32_t)v30 - 1];
                AI_EvalFunc_MSGSend_C(v30, &Src);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &Src);
            }
            AI_gpst_CurrentInstance->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -4.0;
        LABEL_36:
            if (AI_EvalKeyword_During_C())
            {
                v60 = TIM_gf_dt;
                v29 = 10.0f * TIM_gf_dt;
                v28 = 30.0f - v81->f_Z_cam;
                if (v28 > (double)v29)
                    v4 = 10.0f * TIM_gf_dt;
                else
                    v4 = 30.0f - v81->f_Z_cam;
                v61 = v4;
                v81->f_Z_cam = v4 + v81->f_Z_cam;
                v76 = TIM_gf_dt;
                v27 = 20.0f * TIM_gf_dt;
                v26 = 40.0f - v81->f_dist_cam;
                if (v26 > (double)v27)
                    v3 = 20.0f * TIM_gf_dt;
                else
                    v3 = 40.0f - v81->f_dist_cam;
                v53 = v3;
                v81->f_dist_cam = v3 + v81->f_dist_cam;
                v62 = *AI_MainActor;
                *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_i_dynplan = 1;
                *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_i_lastdynplan = 0;
                *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_f_timedynplan = 0x41100000;
                *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_i_contrechampdynplan = 0;
                *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_o_viseedynplan = (uint32_t)v62;
                *(float*)depaneuse_dp_texte__PJ_Camera_gao_des_f_zdynplan = v81->f_Z_cam;
                *(float*)depaneuse_dp_texte__PJ_Camera_gao_des_f_distdynplan = v81->f_dist_cam;
                *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_v_offsetviseedynplan = 0;
                *(uint32_t*)(depaneuse_dp_texte__PJ_Camera_gao_des_v_offsetviseedynplan + 4) = 0;
                *(uint32_t*)(depaneuse_dp_texte__PJ_Camera_gao_des_v_offsetviseedynplan + 8) = 0x40000000;
                v80.x = 0.0;
                v80.y = 0.0;
                v80.z = 1.0;
                AI_EvalFunc_MATHVecRotate_C(&v81->v_cam_axis, &v80, 0.40000001f, &v67);
                v51 = v67;
                v25 = depaneuse_dp_texte__PJ_Camera_gao_des_v_sightdynplan;
                *(float*)depaneuse_dp_texte__PJ_Camera_gao_des_v_sightdynplan = v67.x;
                *(float*)(v25 + 4) = v51.y;
                *(float*)(v25 + 8) = v51.z;
                *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_i_dynplanviseedyn = 0;
                *AI_gppst_CurrentJumpNode = (tAINode*)6;
                result = 0;
            }
            else
            {
                v72 = SND_i_Setf(2, 0.5);
                Src.msgID = 30;
                Src.Int1 = 1;
                Src.Int2 = 4502;
                Src.f2 = 20.0;
                Src.v1.x = 0.0;
                Src.v1.y = 0.0;
                Src.v1.z = 0.0;
                Src.v2.x = 0.0;
                Src.v2.y = 0.0;
                Src.v2.z = 0.0;
                Src.gao = 0;
                Src.gao2 = 0;
                v24 = (tGameObjectMain*)v81->des_o_text;
                if (v24)
                {
                    if ((unsigned int)v24 <= 2)
                        v24 = AI_MainActor[(uint32_t)v24 - 1];
                    AI_EvalFunc_MSGSend_C(v24, &Src);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &Src);
                }
                *AI_gppst_CurrentJumpNode = (tAINode*)7;
                TIME_Wait_C(3.0);
                result = 0;
            }
        }
        return result;
    case 6u:
        goto LABEL_36;
    case 7u:
        AI_EvalFunc_OBJ_FlagInactiveSet_C((tGameObjectMain*)depaneuse_dp_texte__PJ_Camera_gao, 1);
        v86.x = -0.89999998f;
        v86.y = 0.30000001f;
        v86.z = 0.0;
        v75.x = 0.0;
        v75.y = 0.0;
        v75.z = 1.0;
        v23 = (tGameObjectMain*)v81->des_o_cam_arrivee;
        if (v23)
        {
            if ((unsigned int)v23 <= 2)
                v23 = AI_MainActor[(uint32_t)v23 - 1];
            AI_EvalFunc_OBJ_SightGeneralSet_C(v23, &v86, &v75);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGeneralSet_C(0, &v86, &v75);
        }
        v22 = (tGameObjectMain*)v81->des_o_cam_arrivee;
        if (v22)
        {
            if ((unsigned int)v22 <= 2)
                v22 = AI_MainActor[(uint32_t)v22 - 1];
            v1 = AI_EvalFunc_VIEWAssignObject_C(v22, 0);
        }
        else
        {
            v1 = AI_EvalFunc_VIEWAssignObject_C(0, 0);
        }
        v50 = v1;
        v58 = SND_i_Setf(2, 0.1f);
        Src.msgID = 30;
        Src.Int1 = 2;
        Src.Int2 = 4502;
        Src.f2 = 20.0;
        Src.v1.x = 0.0;
        Src.v1.y = 0.0;
        Src.v1.z = 0.0;
        Src.v2.x = 0.0;
        Src.v2.y = 0.0;
        Src.v2.z = 0.0;
        Src.gao = 0;
        Src.gao2 = 0;
        v21 = (tGameObjectMain*)v81->des_o_text;
        if (v21)
        {
            if ((unsigned int)v21 <= 2)
                v21 = AI_MainActor[(uint32_t)v21 - 1];
            AI_EvalFunc_MSGSend_C(v21, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)8;
        TIME_Wait_C(4.0);
        return 0;
    case 8u:
        v81->i_peut_y_aller = 1;
        Src.msgID = 30;
        Src.Int1 = 3;
        Src.Int2 = 4502;
        Src.f2 = 20.0;
        Src.v1.x = 0.0;
        Src.v1.y = 0.0;
        Src.v1.z = 0.0;
        Src.v2.x = 0.0;
        Src.v2.y = 0.0;
        Src.v2.z = 0.0;
        Src.gao = 0;
        Src.gao2 = AI_gpst_CurrentGameObject;
        v20 = (tGameObjectMain*)v81->des_o_text;
        if (v20)
        {
            if ((unsigned int)v20 <= 2)
                v20 = AI_MainActor[(uint32_t)v20 - 1];
            AI_EvalFunc_MSGSend_C(v20, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)9;
        TIME_Wait_C(8.5);
        return 0;
    case 9u:
        AI_EvalFunc_OBJ_FlagInactiveSet_C((tGameObjectMain*)depaneuse_dp_texte__PJ_Camera_gao, 1);
        v57.x = 0.7f;
        v57.y = 0.7f;
        v57.z = 0.0f;
        v64.x = 0.0;
        v64.y = 0.0;
        v64.z = 1.0;
        v19 = (tGameObjectMain*)v81->des_o_cam_arrivee;
        if (v19)
        {
            if ((unsigned int)v19 <= 2)
                v19 = AI_MainActor[(uint32_t)v19 - 1];
            AI_EvalFunc_OBJ_SightGeneralSet_C(v19, &v57, &v64);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGeneralSet_C(0, &v57, &v64);
        }
        v18 = (tGameObjectMain*)v81->des_o_cam_arrivee;
        if (v18)
        {
            if ((unsigned int)v18 <= 2)
                v18 = AI_MainActor[(uint32_t)v18 - 1];
            v2 = AI_EvalFunc_VIEWAssignObject_C(v18, 0);
        }
        else
        {
            v2 = AI_EvalFunc_VIEWAssignObject_C(0, 0);
        }
        v88 = v2;
        *AI_gppst_CurrentJumpNode = (tAINode*)10;
        TIME_Wait_C(2.0);
        return 0;
    case 10u:
        AI_EvalFunc_OBJ_FlagInactiveSet_C((tGameObjectMain*)depaneuse_dp_texte__PJ_Camera_gao, 0);
        v62 = *AI_MainActor;
        *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_i_lastdynplan = 0;
        *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_i_dynplan = 1;
        *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_f_timedynplan = 0x42F00000;
        *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_i_contrechampdynplan = 0;
        *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_o_viseedynplan = (uint32_t)v62;
        *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_f_zdynplan = 0x41000000;
        *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_f_distdynplan = 0x41C80000;
        *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_v_offsetviseedynplan = 0;
        *(uint32_t*)(depaneuse_dp_texte__PJ_Camera_gao_des_v_offsetviseedynplan + 4) = 0;
        *(uint32_t*)(depaneuse_dp_texte__PJ_Camera_gao_des_v_offsetviseedynplan + 8) = 0x40400000;
        v17 = v62;
        if (v62)
        {
            if ((unsigned int)v62 <= 2)
                v17 = AI_MainActor[(uint32_t)v62 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v17, &v78);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &v78);
        }
        a1.x = -v78.x;
        a1.y = -v78.y;
        a1.z = -v78.z;
        v56.x = 0.0;
        v56.y = 0.0;
        v56.z = 1.0;
        AI_EvalFunc_MATHVecRotate_C(&a1, &v56, -0.40000001f, &v66);
        v51 = v66;
        v16 = depaneuse_dp_texte__PJ_Camera_gao_des_v_sightdynplan;
        *(float*)depaneuse_dp_texte__PJ_Camera_gao_des_v_sightdynplan = v66.x;
        *(float*)(v16 + 4) = v51.y;
        *(float*)(v16 + 8) = v51.z;
        *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_i_dynplanviseedyn = 0;
        goto LABEL_89;
    case 12u:
    LABEL_89:
        if (v81->f_dest_speed == 0.0)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)13;
            TIME_Wait_C(0.5);
        }
        else
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)12;
        }
        return 0;
    case 13u:
        *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_i_lastdynplan = 0;
        *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_i_dynplan = 1;
        *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_f_timedynplan = 0x41200000;
        *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_i_contrechampdynplan = 0;
        *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_o_viseedynplan = (uint32_t)AI_gpst_CurrentGameObject;
        *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_f_zdynplan = 0x40400000;
        *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_f_distdynplan = 0x41700000;
        *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_v_offsetviseedynplan = 0;
        *(uint32_t*)(depaneuse_dp_texte__PJ_Camera_gao_des_v_offsetviseedynplan + 4) = 0x40000000;
        *(uint32_t*)(depaneuse_dp_texte__PJ_Camera_gao_des_v_offsetviseedynplan + 8) = 0x40000000;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v43);
        v77.x = -v43.x;
        v77.y = -v43.y;
        v77.z = -v43.z;
        v45.x = 0.0;
        v45.y = 0.0;
        v45.z = 1.0;
        AI_EvalFunc_MATHVecRotate_C(&v77, &v45, -0.80000001f, &v69);
        v51 = v69;
        v15 = (Vec3*)depaneuse_dp_texte__PJ_Camera_gao_des_v_sightdynplan;
        *(float*)depaneuse_dp_texte__PJ_Camera_gao_des_v_sightdynplan = v69.x;
        v15->y = v51.y;
        v15->z = v51.z;
        *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_des_i_dynplanviseedyn = 0;
        Src.msgID = 30;
        Src.Int1 = 4;
        Src.Int2 = 4502;
        Src.f2 = 20.0;
        Src.v1.x = 0.0;
        Src.v1.y = 0.0;
        Src.v1.z = 0.0;
        Src.v2.x = 0.0;
        Src.v2.y = 0.0;
        Src.v2.z = 0.0;
        Src.gao = 0;
        Src.gao2 = 0;
        v14 = (tGameObjectMain*)v81->des_o_text;
        if (v14)
        {
            if ((unsigned int)v14 <= 2)
                v14 = AI_MainActor[(uint32_t)v14 - 1];
            AI_EvalFunc_MSGSend_C(v14, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)14;
        TIME_Wait_C(4.0);
        return 0;
    case 14u:
        Src.msgID = 30;
        Src.Int1 = 5;
        Src.Int2 = 4502;
        Src.f2 = 7.0;
        Src.v1.x = 0.0;
        Src.v1.y = 0.0;
        Src.v1.z = 0.0;
        Src.v2.x = 0.0;
        Src.v2.y = 0.0;
        Src.v2.z = 0.0;
        Src.gao = 0;
        Src.gao2 = 0;
        v13 = (tGameObjectMain*)v81->des_o_text;
        if (v13)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            AI_EvalFunc_MSGSend_C(v13, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)15;
        TIME_Wait_C(4.5);
        return 0;
    case 15u:
        v81->i_replique = 24;
        v46 = SND_i_Setf(2, v81->f_surround);
        v83 = *AI_MainActor;
        v62 = v83;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v47);
        v12 = v62;
        if (v62)
        {
            if ((unsigned int)v62 <= 2)
                v12 = AI_MainActor[(uint32_t)v62 - 1];
            AI_EvalFunc_OBJGetPos_C(v12, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        MATH_SubstractVec(&dest, &v47, &a3);
        v51 = dest;
        AI_EvalFunc_MATHVecSetHorzNormalize_C(&v51);
        *(uint32_t*)depaneuse_dp_texte__PJ_Camera_gao_i_must_cut = 1;
        goto LABEL_111;
    case 17u:
    LABEL_111:
        if (v81->i_replique == 28)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)18;
            TIME_Wait_C(2.0);
        }
        else
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)17;
        }
        result = 0;
        break;
    case 18u:
        Src.msgID = 30;
        Src.Int1 = 6;
        Src.Int2 = 4502;
        Src.f2 = 20.0;
        Src.v1.x = 0.0;
        Src.v1.y = 0.0;
        Src.v1.z = 0.0;
        Src.v2.x = 0.0;
        Src.v2.y = 0.0;
        Src.v2.z = 0.0;
        Src.gao = 0;
        Src.gao2 = 0;
        v11 = (tGameObjectMain*)v81->des_o_text;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v11 = AI_MainActor[(uint32_t)v11 - 1];
            AI_EvalFunc_MSGSend_C(v11, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)19;
        TIME_Wait_C(2.0);
        result = 0;
        break;
    case 19u:
        Src.msgID = 30;
        Src.Int1 = 7;
        Src.Int2 = 4502;
        Src.f2 = 2.0;
        Src.v1.x = 0.0;
        Src.v1.y = 0.0;
        Src.v1.z = 0.0;
        Src.v2.x = 0.0;
        Src.v2.y = 0.0;
        Src.v2.z = 0.0;
        Src.gao = 0;
        Src.gao2 = 0;
        v10 = (tGameObjectMain*)v81->des_o_text;
        if (v10)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            AI_EvalFunc_MSGSend_C(v10, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)20;
        TIME_Wait_C(2.0);
        result = 0;
        break;
    case 20u:
        AI_EvalFunc_AITrackCurStop_C();
        result = 1000;
        break;
    default:
        if (v81->i_replique == 20)
        {
            *(uint32_t*)depaneuse_dp_texte_GST_Board_panel_gao_Gen_Show = 0;
            v62 = *AI_MainActor;
            v42 = v62;
            if (v62)
            {
                if ((unsigned int)v62 <= 2)
                    v42 = AI_MainActor[(uint32_t)v62 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v42, &v79);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v79);
            }
            v74.x = -v79.x;
            v74.y = -v79.y;
            v74.z = -v79.z;
            v41 = &v81->v_cam_axis;
            v81->v_cam_axis.x = -v79.x;
            v41->y = v74.y;
            v41->z = v74.z;
            *AI_gppst_CurrentJumpNode = (tAINode*)2;
            TIME_Wait_C(0.1f);
            result = 0;
        }
        else
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)1;
            result = 0;
        }
        break;
    }
    return result;
}

int depaneuse_dp_donnegrapin()
{
    int result; // eax
    tGameObjectMain* v1; // eax
    tScriptStruct_ActorDepanneuse* v2; // [esp+4h] [ebp-C0h]
    tGameObjectMain* v3; // [esp+Ch] [ebp-B8h]
    tGameObjectMain* v4; // [esp+14h] [ebp-B0h]
    tGameObjectMain* v5; // [esp+2Ch] [ebp-98h]
    tGameObjectMain* v6; // [esp+34h] [ebp-90h]
    tGameObjectMain* v7; // [esp+40h] [ebp-84h]
    Vec3 a2; // [esp+44h] [ebp-80h] BYREF
    Vec3 v9; // [esp+50h] [ebp-74h] BYREF
    Vec3 v10; // [esp+70h] [ebp-54h] BYREF
    Vec3 a3; // [esp+7Ch] [ebp-48h] BYREF
    int v12; // [esp+88h] [ebp-3Ch]
    Vec3 pos; // [esp+9Ch] [ebp-28h] BYREF
    tScriptStruct_ActorDepanneuse* v14; // [esp+A8h] [ebp-1Ch]
    Vec3 a5; // [esp+B8h] [ebp-Ch] BYREF

    v10.x = 1.0;
    v10.y = 0.0;
    v10.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorDepanneuse*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v14 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        depaneuse_dp_donnegrapin_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &depaneuse_dp_donnegrapin_univVars_Flag_speciaux_sauve,
            (void*)depaneuse_dp_donnegrapin_univVars,
            4488);
        fct_depaneuse_dp_donnegrapin_depaneuse_dp_retour = (int)AI_ul_RealResolveOneRefFunction(0x3D0021C7u);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)1)
        goto LABEL_11;
    if (*AI_gppst_CurrentJumpNode != (tAINode*)2)
    {
        v14->en_route = 0;
        v14->f_dest_speed = 0.0;
        v14->f_inertie_speed = 4.0;
        v14->i_grappin_donne = 1;
        v14->i_replique = 23;
    LABEL_11:
        if (v14->i_replique == 24)
        {
            v7 = (tGameObjectMain*)v14->o_end_wp;
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                AI_EvalFunc_OBJGetPos_C(v7, &v14->v_destpos);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v14->v_destpos);
            }
            *AI_gppst_CurrentJumpNode = (tAINode*)2;
            TIME_Wait_C(3.0);
            result = 0;
        }
        else
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)1;
            result = 0;
        }
        return result;
    }
    AI_EvalFunc_ARRBitSet_C((int*)(depaneuse_dp_donnegrapin_univVars_Flag_speciaux_sauve + 4), 2784);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
    v6 = (tGameObjectMain*)v14->des_ventouse;
    if (v6)
    {
        if ((unsigned int)v6 <= 2)
            v6 = AI_MainActor[(uint32_t)v6 - 1];
        v1 = AI_EvalFunc_OBJDuplicate_C(v6, &pos);
    }
    else
    {
        v1 = AI_EvalFunc_OBJDuplicate_C(0, &pos);
    }
    v14->o_ventouse = (int)v1;
    v5 = (tGameObjectMain*)v14->o_ventouse;
    if (v5)
    {
        if ((unsigned int)v5 <= 2)
            v5 = AI_MainActor[(uint32_t)v5 - 1];
        AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, v5, &v14->o_ventouse);
    }
    else
    {
        AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, 0, &v14->o_ventouse);
    }
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v9);
    a2.x = -v9.x;
    a2.y = -v9.y;
    a2.z = -v9.z;
    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&a3);
    AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, 0.5, &a5);
    v10 = a5;
    v4 = (tGameObjectMain*)v14->o_ventouse;
    if (v4)
    {
        if ((unsigned int)v4 <= 2)
            v4 = AI_MainActor[(uint32_t)v4 - 1];
        AI_EvalFunc_OBJ_SightSet_C(v4, &v10);
    }
    else
    {
        AI_EvalFunc_OBJ_SightSet_C(0, &v10);
    }
    v14->m_msg.msgID = 1;
    v14->m_msg.gao = AI_gpst_CurrentGameObject;
    v14->m_msg.gao2 = 0;
    v14->m_msg.f2 = -1.0;
    v14->m_msg.f1 = 0.0;
    v14->m_msg.Int1 = 5;
    v14->m_msg.Int2 = 50;
    v14->m_msg.v1.x = 0.0;
    v14->m_msg.v1.y = 10.0;
    v14->m_msg.v1.z = 11.0;
    v14->m_msg.v2.x = 0.0;
    v14->m_msg.v2.y = 0.0;
    v14->m_msg.v2.z = 0.0;
    v3 = (tGameObjectMain*)v14->o_ventouse;
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        AI_EvalFunc_MSGSend_C(v3, &v14->m_msg);
    }
    else
    {
        AI_EvalFunc_MSGSend_C(0, &v14->m_msg);
    }
    v14->o_next_wp = v14->o_end_wp;
    v14->f_time_retour = TIM_gf_MainClock - TIM_gf_LockTime;
    v14->i_replique = 28;
    v12 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_depaneuse_dp_donnegrapin_depaneuse_dp_retour);
    if (v12 == 1000)
        result = 1000;
    else
        result = v12;
    return result;
}

int depaneuse_dp_retour()
{
    BOOL v0; // eax
    int v1; // eax
    char* v2; // eax
    int result; // eax
    BOOL v4; // eax
    tGameObjectMain* v5; // eax
    tScriptStruct_ActorDepanneuse* v6; // [esp+4h] [ebp-16Ch]
    tGameObjectMain* v7; // [esp+Ch] [ebp-164h]
    tGameObjectMain* v8; // [esp+1Ch] [ebp-154h]
    tGameObjectMain* v9; // [esp+2Ch] [ebp-144h]
    tGameObjectMain* v10; // [esp+34h] [ebp-13Ch]
    tGameObjectMain* v11; // [esp+5Ch] [ebp-114h]
    tGameObjectMain* v12; // [esp+64h] [ebp-10Ch]
    tGameObjectMain* v13; // [esp+6Ch] [ebp-104h]
    tGameObjectMain* v14; // [esp+74h] [ebp-FCh]
    tGameObjectMain* v15; // [esp+78h] [ebp-F8h]
    tGameObjectMain* v16; // [esp+7Ch] [ebp-F4h]
    tGameObjectMain* v17; // [esp+84h] [ebp-ECh]
    tGameObjectMain* v18; // [esp+8Ch] [ebp-E4h]
    tGameObjectMain* v19; // [esp+98h] [ebp-D8h]
    tAI* v20; // [esp+9Ch] [ebp-D4h]
    tAI* v21; // [esp+A0h] [ebp-D0h]
    tGameObjectMain* v22; // [esp+A4h] [ebp-CCh]
    tGameObjectMain* v23; // [esp+A8h] [ebp-C8h]
    float v24; // [esp+ACh] [ebp-C4h]
    tTextGet a3[2]; // [esp+B0h] [ebp-C0h] BYREF
    Vec3 v26; // [esp+CCh] [ebp-A4h] BYREF
    float v27; // [esp+D8h] [ebp-98h]
    BOOL v28; // [esp+DCh] [ebp-94h]
    int v29; // [esp+E0h] [ebp-90h]
    Vec3 v30; // [esp+E4h] [ebp-8Ch] BYREF
    Vec3 v31; // [esp+F0h] [ebp-80h] BYREF
    Vec3 a5; // [esp+FCh] [ebp-74h] BYREF
    Vec3 v33; // [esp+108h] [ebp-68h] BYREF
    Vec3 v34; // [esp+114h] [ebp-5Ch] BYREF
    int i; // [esp+124h] [ebp-4Ch]
    BOOL v36; // [esp+134h] [ebp-3Ch]
    Vec3 v37; // [esp+138h] [ebp-38h] BYREF
    int v38; // [esp+144h] [ebp-2Ch]
    int v39; // [esp+148h] [ebp-28h]
    unsigned int v40; // [esp+14Ch] [ebp-24h]
    float v41; // [esp+150h] [ebp-20h]
    tScriptStruct_ActorDepanneuse* v42; // [esp+154h] [ebp-1Ch]
    int v43; // [esp+158h] [ebp-18h]
    BOOL v44; // [esp+16Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_ActorDepanneuse*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v42 = v6;
    v38 = 0;
    i = 0;
    v29 = 0;
    v39 = 0;
    v40 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        depaneuse_dp_retour_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&depaneuse_dp_retour_univVars_Perso_ID_equipement, (void*)depaneuse_dp_retour_univVars, 40);
        AI2C_ResolveBuffer(&depaneuse_dp_retour_univVars_objet_inventaire, (void*)depaneuse_dp_retour_univVars, 1480);
        AI2C_ResolveBuffer(&depaneuse_dp_retour_univVars_Flag_speciaux_sauve, (void*)depaneuse_dp_retour_univVars, 4488);
        depaneuse_dp_retour_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(&depaneuse_dp_retour_univVars_i_indice_de_map, (void*)depaneuse_dp_retour_univVars, 16156);
        depaneuse_dp_retour_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &depaneuse_dp_retour_GST_Global_gao_i_fade_priority,
            (void*)depaneuse_dp_retour_GST_Global_gao,
            3268);
        AI2C_ResolveBuffer(
            &depaneuse_dp_retour_GST_Global_gao_f_fade_start_time,
            (void*)depaneuse_dp_retour_GST_Global_gao,
            3260);
        AI2C_ResolveBuffer(
            &depaneuse_dp_retour_GST_Global_gao_f_fade_delay,
            (void*)depaneuse_dp_retour_GST_Global_gao,
            3264);
        depaneuse_dp_retour_INV_MatFade_gao = AI_ul_RealResolveOneRefObject(0x5E006934u);
        depaneuse_dp_retour_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &depaneuse_dp_retour_GST_Board_panel_gao_Gen_Show,
            (void*)depaneuse_dp_retour_GST_Board_panel_gao,
            2260);
        AI2C_ResolveBuffer(&depaneuse_dp_retour_univVars_i_special_init, (void*)depaneuse_dp_retour_univVars, 6780);
        AI2C_ResolveBuffer(&depaneuse_dp_retour_univVars_load_world, (void*)depaneuse_dp_retour_univVars, 0);
        AI2C_ResolveBuffer(&depaneuse_dp_retour_univVars_world_ID, (void*)depaneuse_dp_retour_univVars, 4);
        AI2C_ResolveBuffer(&depaneuse_dp_retour_univVars_world_entrance_ID, (void*)depaneuse_dp_retour_univVars, 8);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)10)
    {
        *(uint32_t*)depaneuse_dp_retour_GST_Board_panel_gao_Gen_Show = 1;
        *(uint32_t*)depaneuse_dp_retour_univVars_i_special_init = 0;
        *(uint32_t*)depaneuse_dp_retour_univVars_load_world = 1;
        *(uint32_t*)depaneuse_dp_retour_univVars_world_ID = 72;
        *(uint32_t*)depaneuse_dp_retour_univVars_world_entrance_ID = 33;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        v42->en_route = 0;
        v42->i_retour = 1;
        v27 = TIM_gf_dt;
        v24 = TIM_gf_dt;
        if (TIM_gf_dt < 0.0)
            v24 = 0.0;
        if (v24 > 1.0)
            v24 = 1.0;
        v41 = 50.0f * v24 + v42->f_dest_speed * (1.0f - v24);
        v42->f_dest_speed = v41;
        v42->f_inertie_speed = 2.0;
        v21 = (tAI*)v42->o_aircraft;
        if (v21 == WOR_gst_Universe.univAIInstance)
        {
            v20 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v23 = (tGameObjectMain*)v42->o_aircraft;
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
        v29 = ((tScriptStruct_PJAircraft*)(v20->instanceVarDataCurrent))->o_perso_ventoused;
        v19 = (tGameObjectMain*)v42->des_o_es_wp;
        if (v19)
        {
            if ((unsigned int)v19 <= 2)
                v19 = AI_MainActor[(uint32_t)v19 - 1];
            v0 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, v19);
        }
        else
        {
            v0 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, 0);
        }
        v36 = v0;
        if (v0)
        {
            *(uint32_t*)AI_EvalVarArray2_C(1, (char*)depaneuse_dp_retour_univVars_Perso_ID_equipement, 0, 10) = 0;
            *(uint32_t*)AI_EvalVarArray2_C(1, (char*)depaneuse_dp_retour_univVars_objet_inventaire, 39, 10) = 0;
            AI_EvalFunc_ARRBitClear_C((int*)(depaneuse_dp_retour_univVars_Flag_speciaux_sauve + 4), 2784);
            v40 = depaneuse_dp_retour_INV_manager_gao;
            v18 = (tGameObjectMain*)depaneuse_dp_retour_INV_manager_gao;
            if (depaneuse_dp_retour_INV_manager_gao)
            {
                if ((unsigned int)depaneuse_dp_retour_INV_manager_gao <= 2)
                    v18 = AI_MainActor[depaneuse_dp_retour_INV_manager_gao - 1];
                v1 = AI_EvalFunc_GFXAddSorted_C(v18, 19, 5);
            }
            else
            {
                v1 = AI_EvalFunc_GFXAddSorted_C(0, 19, 5);
            }
            v39 = v1;
            v17 = (tGameObjectMain*)v40;
            if (v40)
            {
                if (v40 <= 2)
                    v17 = AI_MainActor[v40 - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v39, v17, 1);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v39, 0, 1);
            }
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v39, 2, 1);
            for (i = 0; i < 110; ++i)
            {
                if (*(uint32_t*)(depaneuse_dp_retour_univVars_i_indice_de_map + 4 * i + 4) == 72)
                {
                    AI_EvalFunc_TEXTGet_C(637535806, i, a3);
                    v16 = (tGameObjectMain*)v40;
                    if (v40)
                    {
                        if (v40 <= 2)
                            v16 = AI_MainActor[v40 - 1];
                        v15 = v16;
                    }
                    else
                    {
                        v15 = 0;
                    }
                    v2 = AI_EvalFunc_TEXTToString_C(a3);
                    STR_SetString(v15, 30, v2);
                    v14 = (tGameObjectMain*)v40;
                    if (v40)
                    {
                        if (v40 <= 2)
                            v14 = AI_MainActor[v40 - 1];
                        STR_ToUpper(v14, 30, 0, -1);
                    }
                    else
                    {
                        STR_ToUpper(0, 30, 0, -1);
                    }
                    v13 = (tGameObjectMain*)v40;
                    if (v40)
                    {
                        if (v40 <= 2)
                            v13 = AI_MainActor[v40 - 1];
                        AI_EvalFunc_OBJ_TextPageSet_C(v13, 30, 0, -1, 1, -1);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_TextPageSet_C(0, 30, 0, -1, 1, -1);
                    }
                    a5.x = 0.5;
                    a5.y = 0.5;
                    a5.z = 0.0;
                    v12 = (tGameObjectMain*)v40;
                    if (v40)
                    {
                        if (v40 <= 2)
                            v12 = AI_MainActor[v40 - 1];
                        STR_SetStringExt(v12, 30, 1441813, (char*)AI_gpst_CurrentFunction->localStringConstsData, &a5);
                    }
                    else
                    {
                        STR_SetStringExt(0, 30, 1441813, (char*)AI_gpst_CurrentFunction->localStringConstsData, &a5);
                    }
                    AI_EvalFunc_GFXSeti_C_CURRENT(v39, 19100, 30);
                    v11 = (tGameObjectMain*)v40;
                    if (v40)
                    {
                        if (v40 <= 2)
                            v11 = AI_MainActor[v40 - 1];
                        STR_SetString(v11, 30, (char*)AI_gpst_CurrentFunction->localStringConstsData + 1);
                    }
                    else
                    {
                        STR_SetString(0, 30, (char*)AI_gpst_CurrentFunction->localStringConstsData + 1);
                    }
                    break;
                }
            }
            v43 = SND_i_Seti(9, 1);
            if (*(int*)depaneuse_dp_retour_GST_Global_gao_i_fade_priority <= 5000
                || (v28 = AI_EvalFunc_TIMEElapsed_C(
                    *(float*)depaneuse_dp_retour_GST_Global_gao_f_fade_start_time,
                    *(float*)depaneuse_dp_retour_GST_Global_gao_f_fade_delay)))
            {
                *(float*)depaneuse_dp_retour_GST_Global_gao_f_fade_start_time = TIM_gf_MainClock - TIM_gf_LockTime;
                *(uint32_t*)depaneuse_dp_retour_GST_Global_gao_f_fade_delay = 0x3F99999A;
                *(uint32_t*)depaneuse_dp_retour_GST_Global_gao_i_fade_priority = 5000;
                AI_EvalFunc_GFXDel_C_CURRENT(10);
                v38 = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v38, 0, 1);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v38, 2, 1);
                v10 = (tGameObjectMain*)depaneuse_dp_retour_INV_MatFade_gao;
                if (depaneuse_dp_retour_INV_MatFade_gao)
                {
                    if ((unsigned int)depaneuse_dp_retour_INV_MatFade_gao <= 2)
                        v10 = AI_MainActor[depaneuse_dp_retour_INV_MatFade_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v38, v10, 8);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v38, 0, 8);
                }
                GFXSetCurGlobal(v38);
                AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0xFFFFFF);
                AI_EvalFunc_GFXSeti_C_CURGLOB(11101, -1);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 0.2f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 1.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
            }
            else
            {
                v38 = -1;
            }
            *AI_gppst_CurrentJumpNode = (tAINode*)10;
            TIME_Wait_C(0.40000001f);
            result = 0;
        }
        else
        {
            v9 = (tGameObjectMain*)v42->o_next_wp;
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                v4 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, v9);
            }
            else
            {
                v4 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, 0);
            }
            v44 = v4;
            if (v4)
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v33);
                v26.x = -v33.x;
                v26.y = -v33.y;
                v26.z = -v33.z;
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v31);
                AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v26, &v31);
                v8 = (tGameObjectMain*)v42->o_next_wp;
                if (v8)
                {
                    if ((unsigned int)v8 <= 2)
                        v8 = AI_MainActor[(uint32_t)v8 - 1];
                    v5 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v42->des_o_net, v8, 3, 0);
                }
                else
                {
                    v5 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v42->des_o_net, 0, 3, 0);
                }
                v42->o_next_wp = (int)v5;
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v30);
                v37.x = -v30.x;
                v37.y = -v30.y;
                v37.z = -v30.z;
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v34);
                AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v37, &v34);
            }
            v7 = (tGameObjectMain*)v42->o_next_wp;
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                AI_EvalFunc_OBJGetPos_C(v7, &v42->v_destpos);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v42->v_destpos);
            }
            result = 1000;
        }
    }
    return result;
}

int depaneuse_dp_toaircraft()
{
    BOOL v1; // eax
    BOOL v2; // eax
    tGameObjectMain* v3; // eax
    tScriptStruct_ActorDepanneuse* v4; // [esp+0h] [ebp-E8h]
    tGameObjectMain* v5; // [esp+8h] [ebp-E0h]
    tGameObjectMain* v6; // [esp+18h] [ebp-D0h]
    tGameObjectMain* v7; // [esp+28h] [ebp-C0h]
    tGameObjectMain* v8; // [esp+30h] [ebp-B8h]
    tGameObjectMain* v9; // [esp+44h] [ebp-A4h]
    int v10; // [esp+48h] [ebp-A0h]
    Vec3 a1; // [esp+4Ch] [ebp-9Ch] BYREF
    float v12; // [esp+58h] [ebp-90h]
    Vec3 v13; // [esp+5Ch] [ebp-8Ch] BYREF
    BOOL v14; // [esp+68h] [ebp-80h]
    Vec3 a3; // [esp+6Ch] [ebp-7Ch] BYREF
    Vec3 v16; // [esp+78h] [ebp-70h] BYREF
    BOOL v17; // [esp+90h] [ebp-58h]
    Vec3 v18; // [esp+94h] [ebp-54h] BYREF
    Vec3 dest; // [esp+A4h] [ebp-44h] BYREF
    Vec3 v20; // [esp+B0h] [ebp-38h] BYREF
    Vec3 v21; // [esp+BCh] [ebp-2Ch] BYREF
    tScriptStruct_ActorDepanneuse* v22; // [esp+C8h] [ebp-20h]
    Vec3 v23; // [esp+CCh] [ebp-1Ch] BYREF
    int v24; // [esp+D8h] [ebp-10h]
    Vec3 v25; // [esp+DCh] [ebp-Ch] BYREF

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_ActorDepanneuse*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v22 = v4;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        depaneuse_dp_toaircraft_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        fct_depaneuse_dp_toaircraft_depaneuse_dp_donnegrapin = (int)AI_ul_RealResolveOneRefFunction(0x3D0021B6u);
        fct_depaneuse_dp_toaircraft_depaneuse_dp_toaircraftreel = (int)AI_ul_RealResolveOneRefFunction(0x3D0021B4u);
    }
    v22->f_dest_speed = 60.0;
    v22->f_inertie_speed = 2.0;
    AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)depaneuse_dp_toaircraft_GST_Board_panel_gao, 0x200, 0);
    AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)depaneuse_dp_toaircraft_GST_Board_panel_gao, 0x100, 0);
    v9 = (tGameObjectMain*)v22->o_aircraft;
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
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v21);
    MATH_SubstractVec(&dest, &a3, &v21);
    a1 = dest;
    v12 = MATH_VecLen(&a1);
    if (v22->f_dist_aircraft_debut > (double)v12)
    {
        v10 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_depaneuse_dp_toaircraft_depaneuse_dp_donnegrapin);
        if (v10 != 1000)
            return v10;
    }
    v8 = (tGameObjectMain*)v22->o_end_wp;
    if (v8)
    {
        if ((unsigned int)v8 <= 2)
            v8 = AI_MainActor[(uint32_t)v8 - 1];
        v1 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, v8);
    }
    else
    {
        v1 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, 0);
    }
    v17 = v1;
    if (v1)
    {
        v24 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_depaneuse_dp_toaircraft_depaneuse_dp_toaircraftreel);
        if (v24 != 1000)
            return v24;
    }
    v7 = (tGameObjectMain*)v22->o_next_wp;
    if (v7)
    {
        if ((unsigned int)v7 <= 2)
            v7 = AI_MainActor[(uint32_t)v7 - 1];
        v2 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, v7);
    }
    else
    {
        v2 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, 0);
    }
    v14 = v2;
    if (v2)
    {
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v20);
        v23.x = -v20.x;
        v23.y = -v20.y;
        v23.z = -v20.z;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v16);
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v23, &v16);
        v6 = (tGameObjectMain*)v22->o_next_wp;
        if (v6)
        {
            if ((unsigned int)v6 <= 2)
                v6 = AI_MainActor[(uint32_t)v6 - 1];
            v3 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v22->des_o_net, v6, 3, 0);
        }
        else
        {
            v3 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v22->des_o_net, 0, 3, 0);
        }
        v22->o_next_wp = (int)v3;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v13);
        v25.x = -v13.x;
        v25.y = -v13.y;
        v25.z = -v13.z;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v18);
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v25, &v18);
    }
    v5 = (tGameObjectMain*)v22->o_next_wp;
    if (v5)
    {
        if ((unsigned int)v5 <= 2)
            v5 = AI_MainActor[(uint32_t)v5 - 1];
        AI_EvalFunc_OBJGetPos_C(v5, &v22->v_destpos);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v22->v_destpos);
    }
    return 1000;
}

int depaneuse_dp_init()
{
    int result; // eax
    BOOL v1; // eax
    BOOL v2; // eax
    tGameObjectMain* v3; // eax
    tAINode* v4; // [esp+0h] [ebp-C4h]
    tScriptStruct_ActorDepanneuse* v5; // [esp+4h] [ebp-C0h]
    tGameObjectMain* v6; // [esp+14h] [ebp-B0h]
    tGameObjectMain* v7; // [esp+24h] [ebp-A0h]
    tGameObjectMain* v8; // [esp+2Ch] [ebp-98h]
    tGameObjectMain* v9; // [esp+30h] [ebp-94h]
    tGameObjectMain* v10; // [esp+34h] [ebp-90h]
    tGameObjectMain* v11; // [esp+3Ch] [ebp-88h]
    tGameObjectMain* v12; // [esp+40h] [ebp-84h]
    tGameObjectMain* v13; // [esp+44h] [ebp-80h]
    tGameObjectMain* v14; // [esp+4Ch] [ebp-78h]
    tGameObjectMain* v15; // [esp+54h] [ebp-70h]
    tGameObjectMain* v16; // [esp+5Ch] [ebp-68h]
    tGameObjectMain* v17; // [esp+60h] [ebp-64h]
    int v18; // [esp+74h] [ebp-50h]
    Vec3 v19; // [esp+7Ch] [ebp-48h] BYREF
    Vec3 v20; // [esp+88h] [ebp-3Ch] BYREF
    BOOL v21; // [esp+98h] [ebp-2Ch]
    float v22; // [esp+9Ch] [ebp-28h]
    Vec3 v23; // [esp+A0h] [ebp-24h] BYREF
    int v24; // [esp+B0h] [ebp-14h]
    tScriptStruct_ActorDepanneuse* v25; // [esp+B4h] [ebp-10h]
    BOOL v26; // [esp+B8h] [ebp-Ch]
    BOOL v27; // [esp+C0h] [ebp-4h]

    v19.x = 1.0;
    v19.y = 0.0;
    v19.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_ActorDepanneuse*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v25 = v5;
    v26 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        depaneuse_dp_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&depaneuse_dp_init_univVars_Flag_speciaux_sauve, (void*)depaneuse_dp_init_univVars, 4488);
        AI2C_ResolveBuffer(&depaneuse_dp_init_univVars_Perso_ID_actor, (void*)depaneuse_dp_init_univVars, 988);
        depaneuse_dp_init_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        fct_depaneuse_dp_init_depaneuse_dp_exec_afterrec = (int)AI_ul_RealResolveOneRefFunction(0x72004FBEu);
        depaneuse_dp_init_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        depaneuse_dp_init_depaneuse_dp_toaircraft = (int)AI_ul_RealResolveOneRefFunction(0x3D0021B0u);
    }
    v4 = *AI_gppst_CurrentJumpNode;
    if (*AI_gppst_CurrentJumpNode == (tAINode*)8)
    {
        AI_EvalFunc_ARRBitSet_C((int*)(depaneuse_dp_init_univVars_Flag_speciaux_sauve + 4), 2784);
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
        v25->o_next_wp = (int)AI_EvalFunc_WAYNearestWPOfObj_C(
            AI_gpst_CurrentGameObject,
            (tWayNetwork*)v25->des_o_net,
            -1,
            0,
            0);
        v6 = (tGameObjectMain*)v25->o_aircraft;
        if (v6)
        {
            if ((unsigned int)v6 <= 2)
                v6 = AI_MainActor[(uint32_t)v6 - 1];
            v3 = AI_EvalFunc_WAYNearestWPOfObj_C(v6, (tWayNetwork*)v25->des_o_net, -1, 0, 0);
        }
        else
        {
            v3 = AI_EvalFunc_WAYNearestWPOfObj_C(0, (tWayNetwork*)v25->des_o_net, -1, 0, 0);
        }
        v25->o_end_wp = (int)v3;
        goto LABEL_71;
    }
    if (v4 == (tAINode*)11)
    {
    LABEL_71:
        if (!v25->i_peut_y_aller)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)11;
            return 0;
        }
        v25->en_route = 1;
        v25->i_sound = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000000);
        AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v25->i_sound);
        v25->i_je_monte = 1;
        AI_gpst_CurrentInstance->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -3.0;
        goto LABEL_74;
    }
    if (v4 == (tAINode*)13)
    {
    LABEL_74:
        if (AI_EvalKeyword_During_C())
        {
            AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)depaneuse_dp_init_GST_Board_panel_gao, 0x200, 0);
            AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)depaneuse_dp_init_GST_Board_panel_gao, 0x100, 0);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v20);
            v19.x = v20.x;
            v19.y = v20.y;
            v22 = TIM_gf_dt;
            v19.z = 25.0f * TIM_gf_dt + v20.z;
            v25->f_last_z = v19.z;
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v19);
            *AI_gppst_CurrentJumpNode = (tAINode*)13;
            result = 0;
        }
        else
        {
            v25->i_je_monte = 0;
            v25->o_next_wp = v25->des_o_first_wp;
            v18 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)depaneuse_dp_init_depaneuse_dp_toaircraft);
            if (v18 == 1000)
                result = 1000;
            else
                result = v18;
        }
        return result;
    }
    v26 = AI_EvalFunc_ARRBitTest_C((int*)(depaneuse_dp_init_univVars_Flag_speciaux_sauve + 4), 2721);
    if (v26)
    {
        v24 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v24 == 1000)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            result = v24;
        }
    }
    else
    {
        v25->o_aircraft = *(uint32_t*)(depaneuse_dp_init_univVars_Perso_ID_actor + 44);
        v17 = (tGameObjectMain*)v25->o_aircraft;
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
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v23);
            v25->f_last_z = v23.z;
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
            AI_EvalFunc_ARRBitSet_C((int*)(depaneuse_dp_init_univVars_Flag_speciaux_sauve + 4), 2790);
            if (v25->i_idhaloL == -1)
            {
                v25->i_idhaloL = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v25->i_idhaloL, 4, 1);
                v15 = (tGameObjectMain*)depaneuse_dp_init_SFX_light_smoke_gao;
                if (depaneuse_dp_init_SFX_light_smoke_gao)
                {
                    if ((unsigned int)depaneuse_dp_init_SFX_light_smoke_gao <= 2)
                        v15 = AI_MainActor[depaneuse_dp_init_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v25->i_idhaloL, v15, 0);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v25->i_idhaloL, 0, 0);
                }
                GFXSetCurGlobal(v25->i_idhaloL);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 10.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 1.0);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 0x6FFF);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
                v25->i_idhaloR = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v25->i_idhaloR, 4, 1);
                v14 = (tGameObjectMain*)depaneuse_dp_init_SFX_light_smoke_gao;
                if (depaneuse_dp_init_SFX_light_smoke_gao)
                {
                    if ((unsigned int)depaneuse_dp_init_SFX_light_smoke_gao <= 2)
                        v14 = AI_MainActor[depaneuse_dp_init_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v25->i_idhaloR, v14, 0);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v25->i_idhaloR, 0, 0);
                }
                GFXSetCurGlobal(v25->i_idhaloR);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 10.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 1.0);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 0x6FFF);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
            }
            v25->i_retour = 1;
            v13 = (tGameObjectMain*)v25->des_o_bv_panne;
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
            v11 = (tGameObjectMain*)v25->o_aircraft;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v11 = AI_MainActor[(uint32_t)v11 - 1];
                v1 = AI_EvalFunc_COLBVObjectObject_C(v11, v12);
            }
            else
            {
                v1 = AI_EvalFunc_COLBVObjectObject_C(0, v12);
            }
            v21 = v1;
            if (v1)
                goto LABEL_59;
            v10 = (tGameObjectMain*)v25->des_o_bv_panne1;
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
            v8 = (tGameObjectMain*)v25->o_aircraft;
            if (v8)
            {
                if ((unsigned int)v8 <= 2)
                    v8 = AI_MainActor[(uint32_t)v8 - 1];
                v2 = AI_EvalFunc_COLBVObjectObject_C(v8, v9);
            }
            else
            {
                v2 = AI_EvalFunc_COLBVObjectObject_C(0, v9);
            }
            v27 = v2;
            if (v2)
            {
            LABEL_59:
                AI_EvalFunc_OBJ_CapaSet_C_CURRENT(256, 0);
                v7 = (tGameObjectMain*)v25->o_aircraft;
                if (v7)
                {
                    if ((unsigned int)v7 <= 2)
                        v7 = AI_MainActor[(uint32_t)v7 - 1];
                    AI_EvalFunc_AICBAdd_C(
                        AI_gpst_CurrentGameObject,
                        v7,
                        2,
                        (tAIFunction*)fct_depaneuse_dp_init_depaneuse_dp_exec_afterrec);
                }
                else
                {
                    AI_EvalFunc_AICBAdd_C(
                        AI_gpst_CurrentGameObject,
                        0,
                        2,
                        (tAIFunction*)fct_depaneuse_dp_init_depaneuse_dp_exec_afterrec);
                }
                v25->i_replique = 20;
                AI_EvalFunc_ARRBitSet_C((int*)(depaneuse_dp_init_univVars_Flag_speciaux_sauve + 4), 3919);
                v25->i_retour = 0;
                *AI_gppst_CurrentJumpNode = (tAINode*)8;
                TIME_Wait_C(5.0);
                result = 0;
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
    }
    return result;
}

int depaneuse_dp_reflex()
{
    int result; // eax
    float a4; // [esp+0h] [ebp-244h]
    float X; // [esp+4h] [ebp-240h]
    float Xa; // [esp+4h] [ebp-240h]
    float Xb; // [esp+4h] [ebp-240h]
    float Xc; // [esp+4h] [ebp-240h]
    tScriptStruct_ActorDepanneuse* v6; // [esp+8h] [ebp-23Ch]
    float v7; // [esp+Ch] [ebp-238h]
    float v8; // [esp+18h] [ebp-22Ch]
    tGameObjectMain* v9; // [esp+20h] [ebp-224h]
    tGameObjectMain* v10; // [esp+30h] [ebp-214h]
    float v11; // [esp+4Ch] [ebp-1F8h]
    Vec3 v12; // [esp+5Ch] [ebp-1E8h] BYREF
    float v13; // [esp+68h] [ebp-1DCh]
    Vec3 v14; // [esp+6Ch] [ebp-1D8h] BYREF
    float v15; // [esp+78h] [ebp-1CCh]
    float v16; // [esp+7Ch] [ebp-1C8h]
    float v17; // [esp+80h] [ebp-1C4h]
    tGameObjectMain* v18[50]; // [esp+84h] [ebp-1C0h] BYREF
    float v19; // [esp+150h] [ebp-F4h]
    float v20; // [esp+154h] [ebp-F0h]
    Vec3 v21; // [esp+15Ch] [ebp-E8h] BYREF
    Vec3 v22; // [esp+168h] [ebp-DCh] BYREF
    Vec3 v23; // [esp+178h] [ebp-CCh] BYREF
    tGameObjectMain* v24; // [esp+184h] [ebp-C0h]
    float v25; // [esp+188h] [ebp-BCh]
    Vec3 v26; // [esp+18Ch] [ebp-B8h] BYREF
    Vec3 a2; // [esp+198h] [ebp-ACh] BYREF
    float v28; // [esp+1A4h] [ebp-A0h]
    Vec3 v29; // [esp+1A8h] [ebp-9Ch] BYREF
    Vec3 v30; // [esp+1B4h] [ebp-90h] BYREF
    float v31; // [esp+1C0h] [ebp-84h]
    float v32; // [esp+1C8h] [ebp-7Ch]
    int v33; // [esp+1CCh] [ebp-78h]
    Vec3 dest; // [esp+1D0h] [ebp-74h] BYREF
    Vec3 a5; // [esp+1DCh] [ebp-68h] BYREF
    float v36; // [esp+1E8h] [ebp-5Ch]
    Vec3 v37; // [esp+1ECh] [ebp-58h] BYREF
    Vec3 v38; // [esp+1F8h] [ebp-4Ch] BYREF
    Vec3 v39; // [esp+204h] [ebp-40h] BYREF
    tScriptStruct_ActorDepanneuse* v40; // [esp+210h] [ebp-34h]
    float v41; // [esp+214h] [ebp-30h]
    Vec3 v42; // [esp+218h] [ebp-2Ch]
    Vec3 a3; // [esp+224h] [ebp-20h] BYREF
    float v44; // [esp+230h] [ebp-14h]
    float v45; // [esp+234h] [ebp-10h]
    float v46; // [esp+238h] [ebp-Ch]
    float v47; // [esp+23Ch] [ebp-8h]
    BOOL v48; // [esp+240h] [ebp-4h]
    float v18a;

    v30.x = 1.0;
    v30.y = 0.0;
    v30.z = 0.0;
    v22.x = 1.0;
    v22.y = 0.0;
    v22.z = 0.0;
    v14.x = 1.0;
    v14.y = 0.0;
    v14.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_ActorDepanneuse*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v40 = v6;
    v45 = 0.0;
    v24 = 0;
    v48 = 0;
    v33 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        depaneuse_dp_reflex_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        depaneuse_dp_reflex_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&depaneuse_dp_reflex_univVars_Flag_speciaux_sauve, (void*)depaneuse_dp_reflex_univVars, 4488);
        depaneuse_dp_reflex_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(&depaneuse_dp_reflex_GST_Global_gao_o_micro, (void*)depaneuse_dp_reflex_GST_Global_gao, 4648);
    }
    if (v40->i_je_monte)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        if (!v40->i_retour)
        {
            AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)depaneuse_dp_reflex_GST_Board_panel_gao, 0x200, 0);
            AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)depaneuse_dp_reflex_GST_Board_panel_gao, 0x100, 0);
        }
        v48 = AI_EvalFunc_ARRBitTest_C((int*)(depaneuse_dp_reflex_univVars_Flag_speciaux_sauve + 4), 2718);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v39);
        MATH_SubstractVec(&dest, &v39, &v40->v_destpos);
        v14 = dest;
        v15 = TIM_gf_dt;
        v11 = v40->f_inertie_speed * TIM_gf_dt;
        if (v11 < 0.0)
            v11 = 0.0;
        if (v11 > 1.0)
            v11 = 1.0;
        v13 = v40->f_dest_speed * v11 + v40->f_cur_speed * (1.0f - v11);
        v40->f_cur_speed = v13;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
        AI_EvalFunc_MATHVecNormalize_C(&v14, &a3);
        v18a = TIM_gf_dt;
        a4 = 2.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
        v14 = a5;
        v42.x = 0.0;
        v42.y = 0.0;
        v42.z = 1.0;
        v30.x = 0.0;
        v30.y = 0.0;
        v30.z = 1.0;
        v19 = TIM_gf_MainClock - TIM_gf_LockTime;
        X = v19 + v19;
        v31 = sinf(X);
        v45 = v31;
        if (v48)
        {
            v17 = TIM_gf_MainClock - TIM_gf_LockTime;
            Xa = v17 * 4.0f;
            v47 = sinf(Xa);
            v45 = v47 + v47;
        }
        else
        {
            v32 = TIM_gf_MainClock - TIM_gf_LockTime;
            Xb = v32 + v32;
            v20 = sinf(Xb);
            v45 = v20;
        }
        v23.x = v45 * 0.1f;
        v23.y = 0.0;
        v23.z = 0.0;
        AI_EvalFunc_MATHVecLocalToGlobal_C(AI_gpst_CurrentGameObject, &v23, &v12);
        v30.x = v30.x + v12.x;
        v30.y = v30.y + v12.y;
        v30.z = v30.z + v12.z;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v14, &v30);
        v25 = TIM_gf_dt;
        Xc = v40->f_cur_speed * TIM_gf_dt;
        AI_EvalFunc_MATHVecSetNormVector_C(&v14, Xc);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v37);
        MATH_SubstractVec(&v29, &v37, &v14);
        v14 = v29;
        v46 = TIM_gf_MainClock - TIM_gf_LockTime;
        v36 = sinf(v46);
        v14.z = v40->v_destpos.z + 2.0f * v36;
        v33 = AI_EvalFunc_COLBVGetAllOverlaps_C(AI_gpst_CurrentGameObject, v18, 2, 0, 1);
        while (v33)
        {
            v24 = v18[v33];
            --v33;
            v10 = v24;
            if (v24)
            {
                if ((unsigned int)v24 <= 2)
                    v10 = AI_MainActor[(uint32_t)v24 - 1];
                AI_EvalFunc_BVMaxGet_C(v10, &v21);
            }
            else
            {
                AI_EvalFunc_BVMaxGet_C(0, &v21);
            }
            v22 = v21;
            v9 = v24;
            if (v24)
            {
                if ((unsigned int)v24 <= 2)
                    v9 = AI_MainActor[(uint32_t)v24 - 1];
                AI_EvalFunc_OBJGetPos_C(v9, &v38);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v38);
            }
            v22.x = v22.x + v38.x;
            v22.y = v22.y + v38.y;
            v22.z = v22.z + v38.z;
            if (v14.z < (double)v22.z)
                v14.z = v22.z;
        }
        v16 = TIM_gf_dt;
        v8 = TIM_gf_dt;
        if (TIM_gf_dt < 0.0)
            v8 = 0.0;
        if (v8 > 1.0)
            v8 = 1.0;
        v44 = v14.z * v8 + v40->f_last_z * (1.0f - v8);
        v40->f_last_z = v44;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v26);
        v22 = v26;
        v41 = TIM_gf_dt;
        v7 = 2.0f * TIM_gf_dt;
        if (v7 < 0.0)
            v7 = 0.0;
        if (v7 > 1.0)
            v7 = 1.0;
        v28 = v40->f_last_z * v7 + (1.0f - v7) * v22.z;
        v22.z = v28;
        v22.x = v14.x;
        v22.y = v14.y;
        AI_EvalFunc_OBJSetPos_C_CURRENT(&v22);
        if (v40->en_route)
            *(uint32_t*)depaneuse_dp_reflex_GST_Global_gao_o_micro = (uint32_t)AI_EvalFunc_VIEWGetObject_C(0);
        result = 1000;
    }
    return result;
}