#include "ai_compiledscripts_inc.h"

//Bouee_Anti_Aircraft_BAA_init
int Bouee_Anti_Aircraft_BAA_init__PJ_Aircraft_gao;
int Bouee_Anti_Aircraft_BAA_init_univVars;
int Bouee_Anti_Aircraft_BAA_init_univVars_Flag_speciaux_sauve;
int Bouee_Anti_Aircraft_BAA_init_SFX_light_smoke_gao;
int fct_Bouee_Anti_Aircraft_BAA_init_Bouee_Anti_Aircraft_BAA_mode_statique;
int fct_Bouee_Anti_Aircraft_BAA_init_Bouee_Anti_Aircraft_BAA_mode_dynamique;
int fct_Bouee_Anti_Aircraft_BAA_init_Bouee_Anti_Aircraft_BAA_exec_afterrec;
int fct_Bouee_Anti_Aircraft_BAA_init_Bouee_Anti_Aircraft_BAA_reflex;

//Bouee_Anti_Aircraft_BAA_reflex
int Bouee_Anti_Aircraft_BAA_reflex_univVars;
int Bouee_Anti_Aircraft_BAA_reflex_univVars_Perso_ID_actor;

//Bouee_Anti_Aircraft_BAA_mode_statique
int fct_Bouee_Anti_Aircraft_BAA_mode_statique_Bouee_Anti_Aircraft_BAA_exec_interception;

//Bouee_Anti_Aircraft_BAA_exec_interception
int Bouee_Anti_Aircraft_BAA_exec_interception__PJ_Aircraft_gao;
int Bouee_Anti_Aircraft_BAA_exec_interception_univVars;
int Bouee_Anti_Aircraft_BAA_exec_interception_univVars_Flag_speciaux_sauve;
int Bouee_Anti_Aircraft_BAA_exec_interception_SFX_light_smoke_gao;
int Bouee_Anti_Aircraft_BAA_exec_interception_univVars_Perso_ID_passengers_list;

//Bouee_Anti_Aircraft_BAA_exec_afterrec
int Bouee_Anti_Aircraft_BAA_exec_afterrec__PJ_Camera_gao;
int Bouee_Anti_Aircraft_BAA_exec_afterrec_SFX_Flash_texture_gao;

//Bouee_Anti_Aircraft_BAA_mode_dynamique
int fctvar_Bouee_Anti_Aircraft_BAA_mode_dynamique_camera;
int fct_Bouee_Anti_Aircraft_BAA_mode_dynamique_Bouee_Anti_Aircraft_BAA_exec_GFX_trace;
int fct_Bouee_Anti_Aircraft_BAA_mode_dynamique_Bouee_Anti_Aircraft_BAA_exec_interception;

//Bouee_Anti_Aircraft_BAA_exec_GFX_trace
int Bouee_Anti_Aircraft_BAA_exec_GFX_trace_PJ_aircraft_material_gao;

int Bouee_Anti_Aircraft_BAA_init()
{
    int result; // eax
    tScriptStruct_BoueeAntiAircraft* v1; // [esp+8h] [ebp-70h]
    tGameObjectMain* v2; // [esp+10h] [ebp-68h]
    tGameObjectMain* v3; // [esp+18h] [ebp-60h]
    tGameObjectMain* v4; // [esp+20h] [ebp-58h]
    tGameObjectMain* v5; // [esp+28h] [ebp-50h]
    Vec3 a2; // [esp+38h] [ebp-40h] BYREF
    tGameObjectMain* v7; // [esp+44h] [ebp-34h]
    int v8; // [esp+48h] [ebp-30h]
    int v9; // [esp+50h] [ebp-28h]
    tGameObjectMain* v10; // [esp+54h] [ebp-24h]
    unsigned int v11; // [esp+58h] [ebp-20h]
    BOOL v12; // [esp+5Ch] [ebp-1Ch]
    tScriptStruct_BoueeAntiAircraft* v13; // [esp+64h] [ebp-14h]
    Vec3 v14; // [esp+68h] [ebp-10h] BYREF
    tGameObjectMain* v15; // [esp+74h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_BoueeAntiAircraft*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v13 = v1;
    v12 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Bouee_Anti_Aircraft_BAA_init__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        Bouee_Anti_Aircraft_BAA_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Bouee_Anti_Aircraft_BAA_init_univVars_Flag_speciaux_sauve,
            (void*)Bouee_Anti_Aircraft_BAA_init_univVars,
            4488);
        Bouee_Anti_Aircraft_BAA_init_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        fct_Bouee_Anti_Aircraft_BAA_init_Bouee_Anti_Aircraft_BAA_mode_statique = (int)AI_ul_RealResolveOneRefFunction(0x72004B25u);
        fct_Bouee_Anti_Aircraft_BAA_init_Bouee_Anti_Aircraft_BAA_mode_dynamique = (int)AI_ul_RealResolveOneRefFunction(0x720046AEu);
        fct_Bouee_Anti_Aircraft_BAA_init_Bouee_Anti_Aircraft_BAA_exec_afterrec = (int)AI_ul_RealResolveOneRefFunction(0x72004AD5u);
        fct_Bouee_Anti_Aircraft_BAA_init_Bouee_Anti_Aircraft_BAA_reflex = (int)AI_ul_RealResolveOneRefFunction(0x720046C5u);
    }
    if (*AI_gppst_CurrentJumpNode)
    {
        v8 = AI_EvalFunc_AIPriorityGet_C((tGameObjectMain*)Bouee_Anti_Aircraft_BAA_init__PJ_Aircraft_gao);
        AI_EvalFunc_AIPrioritySet_C(AI_gpst_CurrentGameObject, v8 - 1);
        *AI_gppst_CurrentJumpNode = 0;
        TIME_Wait_C(0.1f);
        result = 0;
    }
    else
    {
        if (v13->des_SF_inactif <= 0)
            v12 = 0;
        else
            v12 = AI_EvalFunc_ARRBitTest_C(
                (int*)(Bouee_Anti_Aircraft_BAA_init_univVars_Flag_speciaux_sauve + 4),
                v13->des_SF_inactif);
        if (v12)
        {
            v11 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1;
        }
        else
        {
            AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 0, 3);
            AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x8000, 0);
            AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 1, 0);
            AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0xFFu, 0);
            AI_EvalFunc_COLPrioritySet_C(AI_gpst_CurrentGameObject, 100);
            AI_EvalFunc_COLCrossableSet_C(AI_gpst_CurrentGameObject, 17, 0);
            v13->f_sqr_dist_max = v13->des_f_dist_max * v13->des_f_dist_max;
            v13->i_text_index = AI_EvalFunc_MATHRandInt_C(0, 4);
            if (v13->des_f_halo_extrude != 0.0)
            {
                v13->GFX_Halo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v13->GFX_Halo, 4, 1);
                v5 = (tGameObjectMain*)Bouee_Anti_Aircraft_BAA_init_SFX_light_smoke_gao;
                if (Bouee_Anti_Aircraft_BAA_init_SFX_light_smoke_gao)
                {
                    if ((unsigned int)Bouee_Anti_Aircraft_BAA_init_SFX_light_smoke_gao <= 2)
                        v5 = AI_MainActor[Bouee_Anti_Aircraft_BAA_init_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v13->GFX_Halo, v5, 0);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v13->GFX_Halo, 0, 0);
                }
                GFXSetCurGlobal(v13->GFX_Halo);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 8.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21002, -20.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 5.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21006, 0.15000001f);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 0x3080A0);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21104, 1);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v13->des_f_halo_extrude);
                v13->c_halo = 0x3080A0;
            }
            if (v13->des_line_width == 0.0f)
            {
                if (v13->des_comportement)
                    v13->f_line_width = v13->des_f_Z_max - v13->des_f_Z_min;
                else
                    v13->f_line_width = 3.0;
                v13->f_line_Z_offset = v13->f_line_width * 0.5f;
            }
            else
            {
                v13->f_line_width = v13->des_line_width;
                v13->f_line_Z_offset = 0.0;
            }
            v14.x = -20.0;
            v14.y = -20.0;
            v14.z = -250.0;
            AI_EvalFunc_BVMinSet_C(AI_gpst_CurrentGameObject, &v14);
            a2.x = 20.0;
            a2.y = 20.0;
            a2.z = 250.0;
            AI_EvalFunc_BVMaxSet_C(AI_gpst_CurrentGameObject, &a2);
            if (v13->des_comportement)
            {
                v10 = AI_gpst_CurrentGameObject;
                v4 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                        v4 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                    AI_EvalFunc_AICBAdd_C(
                        AI_gpst_CurrentGameObject,
                        v4,
                        2,
                        (tAIFunction*)fct_Bouee_Anti_Aircraft_BAA_init_Bouee_Anti_Aircraft_BAA_mode_statique);
                }
                else
                {
                    AI_EvalFunc_AICBAdd_C(
                        AI_gpst_CurrentGameObject,
                        0,
                        2,
                        (tAIFunction*)fct_Bouee_Anti_Aircraft_BAA_init_Bouee_Anti_Aircraft_BAA_mode_statique);
                }
            }
            else
            {
                v15 = AI_gpst_CurrentGameObject;
                v3 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                        v3 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                    AI_EvalFunc_AICBAdd_C(
                        AI_gpst_CurrentGameObject,
                        v3,
                        2,
                        (tAIFunction*)fct_Bouee_Anti_Aircraft_BAA_init_Bouee_Anti_Aircraft_BAA_mode_dynamique);
                }
                else
                {
                    AI_EvalFunc_AICBAdd_C(
                        AI_gpst_CurrentGameObject,
                        0,
                        2,
                        (tAIFunction*)fct_Bouee_Anti_Aircraft_BAA_init_Bouee_Anti_Aircraft_BAA_mode_dynamique);
                }
            }
            v7 = AI_gpst_CurrentGameObject;
            v2 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v2 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v2,
                    2,
                    (tAIFunction*)fct_Bouee_Anti_Aircraft_BAA_init_Bouee_Anti_Aircraft_BAA_exec_afterrec);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_Bouee_Anti_Aircraft_BAA_init_Bouee_Anti_Aircraft_BAA_exec_afterrec);
            }
            v9 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Bouee_Anti_Aircraft_BAA_init_Bouee_Anti_Aircraft_BAA_reflex);
            if (v9 == 1000)
                result = 1000;
            else
                result = v9;
        }
    }
    return result;
}

int Bouee_Anti_Aircraft_BAA_reflex()
{
    BOOL v0; // eax
    double v1; // st7
    double v2; // st7
    BOOL v3; // eax
    BOOL v4; // eax
    tScriptStruct_BoueeAntiAircraft* v6; // [esp+0h] [ebp-180h]
    tGameObjectMain* v7; // [esp+8h] [ebp-178h]
    tGameObjectMain* v8; // [esp+20h] [ebp-160h]
    tGameObjectMain* v9; // [esp+28h] [ebp-158h]
    tGameObjectMain* v10; // [esp+2Ch] [ebp-154h]
    tGameObjectMain* v11; // [esp+30h] [ebp-150h]
    tGameObjectMain* v12; // [esp+38h] [ebp-148h]
    tGameObjectMain* v13; // [esp+50h] [ebp-130h]
    tGameObjectMain* v14; // [esp+58h] [ebp-128h]
    tGameObjectMain* v15; // [esp+5Ch] [ebp-124h]
    tGameObjectMain* v16; // [esp+60h] [ebp-120h]
    tGameObjectMain* v17; // [esp+68h] [ebp-118h]
    tGameObjectMain* v18; // [esp+70h] [ebp-110h]
    tGameObjectMain* v19; // [esp+78h] [ebp-108h]
    tGameObjectMain* v20; // [esp+7Ch] [ebp-104h]
    tGameObjectMain* v21; // [esp+84h] [ebp-FCh]
    tGameObjectMain* v22; // [esp+88h] [ebp-F8h]
    tGameObjectMain* v23; // [esp+8Ch] [ebp-F4h]
    float v24; // [esp+9Ch] [ebp-E4h]
    Vec3 v25; // [esp+A0h] [ebp-E0h] BYREF
    Vec3 a3; // [esp+ACh] [ebp-D4h] BYREF
    Vec3 v27; // [esp+B8h] [ebp-C8h] BYREF
    Vec3 v28; // [esp+C4h] [ebp-BCh] BYREF
    float v29; // [esp+D0h] [ebp-B0h]
    BOOL v30; // [esp+D4h] [ebp-ACh]
    Vec3 v31; // [esp+D8h] [ebp-A8h] BYREF
    Vec3 v32; // [esp+E4h] [ebp-9Ch] BYREF
    Vec3 v33; // [esp+F0h] [ebp-90h] BYREF
    BOOL v34; // [esp+FCh] [ebp-84h]
    BOOL v35; // [esp+100h] [ebp-80h]
    Vec3 v36; // [esp+104h] [ebp-7Ch] BYREF
    Vec3 v37; // [esp+110h] [ebp-70h] BYREF
    float a1; // [esp+11Ch] [ebp-64h]
    Vec3 v39; // [esp+120h] [ebp-60h] BYREF
    Vec3 v40; // [esp+12Ch] [ebp-54h] BYREF
    Vec3 v41; // [esp+13Ch] [ebp-44h] BYREF
    float v42; // [esp+148h] [ebp-38h]
    tScriptStruct_BoueeAntiAircraft* v43; // [esp+150h] [ebp-30h]
    int v44; // [esp+154h] [ebp-2Ch]
    Vec3 v45; // [esp+158h] [ebp-28h] BYREF
    Vec3 v47; // [esp+168h] [ebp-18h] BYREF
    Vec3 dest; // [esp+174h] [ebp-Ch] BYREF

    v27.x = 1.0;
    v27.y = 0.0;
    v27.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_BoueeAntiAircraft*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v43 = v6;
    v44 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Bouee_Anti_Aircraft_BAA_reflex_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Bouee_Anti_Aircraft_BAA_reflex_univVars_Perso_ID_actor,
            (void*)Bouee_Anti_Aircraft_BAA_reflex_univVars,
            988);
    }
    v44 = 0;
    v23 = (tGameObjectMain*)v43->des_o_acteur_texte;
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
        v21 = (tGameObjectMain*)v43->des_o_acteur_texte;
        if (v21)
        {
            if ((unsigned int)v21 <= 2)
                v21 = AI_MainActor[(uint32_t)v21 - 1];
            v0 = AI_EvalFunc_OBJ_CapaTest_C(v21, 0x100);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x100);
        }
        v30 = v0;
        if (v0)
            v43->f_time_affiche_texte = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    v43->o_main_actor = (int)*AI_MainActor;
    v20 = (tGameObjectMain*)v43->o_main_actor;
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
    if (v19 == *(tGameObjectMain**)(Bouee_Anti_Aircraft_BAA_reflex_univVars_Perso_ID_actor + 44))
    {
        v17 = (tGameObjectMain*)v43->o_main_actor;
        if (v17)
        {
            if ((unsigned int)v17 <= 2)
                v17 = AI_MainActor[(uint32_t)v17 - 1];
            v2 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v17);
        }
        else
        {
            v2 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
        }
        v43->f_sqr_dist_aircraft = (float)v2;
        v16 = (tGameObjectMain*)v43->des_o_suivant;
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
            if (v43->des_permanent)
                v43->f_sqr_dist_suivant = 1.0e14f;
            else
                v43->f_sqr_dist_suivant = 160000.0;
            v14 = (tGameObjectMain*)v43->des_o_suivant;
            if (v14)
            {
                if ((unsigned int)v14 <= 2)
                    v14 = AI_MainActor[(uint32_t)v14 - 1];
                v3 = AI_EvalFunc_OBJ_CapaTest_C(v14, 2);
            }
            else
            {
                v3 = AI_EvalFunc_OBJ_CapaTest_C(0, 2);
            }
            v35 = v3;
            if (v3)
                v44 = 1;
            if (v43->f_sqr_dist_suivant * v43->des_f_coef_distance_detect <= v43->f_sqr_dist_aircraft)
            {
                AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 1);
            }
            else if (v44 != 1 && !AI_EvalFunc_OBJ_CapaTest_C_CURRENT(1))
            {
                AI_EvalFunc_OBJ_CapaSet_C_CURRENT(1, 0);
                v13 = (tGameObjectMain*)v43->des_o_suivant;
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
                v27 = a3;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v40);
                v27.x = v27.x - v40.x;
                v27.y = v27.y - v40.y;
                v27.z = v27.z - v40.z;
                AI_EvalFunc_MATHVecSetHorzNormalize_C(&v27);
                v45.x = 0.0;
                v45.y = 0.0;
                v45.z = 1.0;
                AI_EvalFunc_MATHVecCrossProduct_C(&v27, &v45, &v28);
                v27 = v28;
                v12 = (tGameObjectMain*)v43->o_main_actor;
                if (v12)
                {
                    if ((unsigned int)v12 <= 2)
                        v12 = AI_MainActor[(uint32_t)v12 - 1];
                    AI_EvalFunc_OBJGetPos_C(v12, &v37);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v37);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v25);
                MATH_SubstractVec(&dest, &v37, &v25);
                a1 = dest.z * v27.z + dest.y * v27.y + v27.x * dest.x;
                v29 = AI_EvalFunc_MTHFloatSign_C(a1);
                v43->f_air_suiv_last_side = v29;
            }
        }
        v11 = (tGameObjectMain*)v43->des_o_precedent;
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
            if (v43->des_permanent)
                v43->f_sqr_dist_precedent = 1.0e14f;
            else
                v43->f_sqr_dist_precedent = 160000.0;
            v9 = (tGameObjectMain*)v43->des_o_precedent;
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                v4 = AI_EvalFunc_OBJ_CapaTest_C(v9, 1);
            }
            else
            {
                v4 = AI_EvalFunc_OBJ_CapaTest_C(0, 1);
            }
            v34 = v4;
            if (v4)
                v44 += 2;
            if (v43->f_sqr_dist_precedent * v43->des_f_coef_distance_detect <= v43->f_sqr_dist_aircraft)
            {
                AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 2);
            }
            else if (v44 < 2 && !AI_EvalFunc_OBJ_CapaTest_C_CURRENT(2))
            {
                AI_EvalFunc_OBJ_CapaSet_C_CURRENT(2, 0);
                v8 = (tGameObjectMain*)v43->des_o_precedent;
                if (v8)
                {
                    if ((unsigned int)v8 <= 2)
                        v8 = AI_MainActor[(uint32_t)v8 - 1];
                    AI_EvalFunc_OBJGetPos_C(v8, &v33);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v33);
                }
                v27 = v33;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v41);
                v27.x = v27.x - v41.x;
                v27.y = v27.y - v41.y;
                v27.z = v27.z - v41.z;
                AI_EvalFunc_MATHVecSetHorzNormalize_C(&v27);
                v36.x = 0.0;
                v36.y = 0.0;
                v36.z = 1.0;
                AI_EvalFunc_MATHVecCrossProduct_C(&v27, &v36, &v47);
                v27 = v47;
                v7 = (tGameObjectMain*)v43->o_main_actor;
                if (v7)
                {
                    if ((unsigned int)v7 <= 2)
                        v7 = AI_MainActor[(uint32_t)v7 - 1];
                    AI_EvalFunc_OBJGetPos_C(v7, &v39);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v39);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v32);
                MATH_SubstractVec(&v31, &v39, &v32);
                v42 = v31.z * v27.z + v31.y * v27.y + v27.x * v31.x;
                v24 = AI_EvalFunc_MTHFloatSign_C(v42);
                v43->f_air_prec_last_side = v24;
            }
        }
    }
    else
    {
        v18 = (tGameObjectMain*)v43->o_main_actor;
        if (v18)
        {
            if ((unsigned int)v18 <= 2)
                v18 = AI_MainActor[(uint32_t)v18 - 1];
            v1 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v18);
        }
        else
        {
            v1 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
        }
        v43->f_sqr_dist_aircraft = (float)v1;
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 3);
    }
    return 1000;
}

int Bouee_Anti_Aircraft_BAA_mode_statique()
{
    int result; // eax
    tScriptStruct_BoueeAntiAircraft* v1; // [esp+0h] [ebp-B8h]
    tGameObjectMain* v2; // [esp+8h] [ebp-B0h]
    tGameObjectMain* v3; // [esp+18h] [ebp-A0h]
    tGameObjectMain* v4; // [esp+20h] [ebp-98h]
    tGameObjectMain* v5; // [esp+30h] [ebp-88h]
    Vec3 v6; // [esp+34h] [ebp-84h] BYREF
    Vec3 a3; // [esp+40h] [ebp-78h] BYREF
    tGameObjectMain* v8; // [esp+4Ch] [ebp-6Ch]
    Vec3 v9; // [esp+50h] [ebp-68h] BYREF
    Vec3 v11; // [esp+60h] [ebp-58h] BYREF
    Vec3 dest; // [esp+70h] [ebp-48h] BYREF
    Vec3 v14; // [esp+7Ch] [ebp-3Ch] BYREF
    int v15; // [esp+88h] [ebp-30h]
    tScriptStruct_BoueeAntiAircraft* v16; // [esp+98h] [ebp-20h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_BoueeAntiAircraft*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v16 = v1;
    v8 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Bouee_Anti_Aircraft_BAA_mode_statique_Bouee_Anti_Aircraft_BAA_exec_interception = (int)AI_ul_RealResolveOneRefFunction(0x72004B26u);
    }
    if (v16->i_current_mode != 1)
        v16->i_current_mode = 1;
    if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x40))
    {
        v8 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 0x40);
        v16->m_msg.msgID = 37;
        v16->m_msg.gao = AI_gpst_CurrentGameObject;
        v16->m_msg.gao2 = 0;
        v16->m_msg.Int1 = 262150;
        v16->m_msg.Int2 = 0;
        v5 = v8;
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v5 = AI_MainActor[(uint32_t)v8 - 1];
            AI_EvalFunc_OBJGetPos_C(v5, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v6);
        MATH_SubstractVec(&dest, &a3, &v6);
        v16->m_msg.v1 = dest;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v16->m_msg.v2);
        v16->m_msg.f2 = 0.0;
        v16->m_msg.f1 = 1000.0;
        v4 = v8;
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v4 = AI_MainActor[(uint32_t)v8 - 1];
            AI_EvalFunc_MSGSend_C(v4, &v16->m_msg);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &v16->m_msg);
        }
    }
    if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x80))
    {
        v8 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 0x80);
        v16->m_msg.msgID = 37;
        v16->m_msg.gao = AI_gpst_CurrentGameObject;
        v16->m_msg.gao2 = 0;
        v16->m_msg.Int1 = 262150;
        v16->m_msg.Int2 = 0;
        v3 = v8;
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v3 = AI_MainActor[(uint32_t)v8 - 1];
            AI_EvalFunc_OBJGetPos_C(v3, &v11);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v11);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v14);
        MATH_SubstractVec(&v9, &v11, &v14);
        v16->m_msg.v1 = v9;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v16->m_msg.v2);
        v16->m_msg.f2 = 0.0;
        v16->m_msg.f1 = 1000.0;
        v2 = v8;
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v2 = AI_MainActor[(uint32_t)v8 - 1];
            AI_EvalFunc_MSGSend_C(v2, &v16->m_msg);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &v16->m_msg);
        }
    }
    v15 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Bouee_Anti_Aircraft_BAA_mode_statique_Bouee_Anti_Aircraft_BAA_exec_interception);
    if (v15 == 1000)
        result = 1000;
    else
        result = v15;
    return result;
}

int Bouee_Anti_Aircraft_BAA_exec_interception()
{
    int v0; // eax
    int v1; // eax
    int v2; // eax
    int v3; // eax
    int v4; // eax
    int v5; // eax
    int v6; // eax
    float v8; // [esp+4h] [ebp-784h]
    float v9; // [esp+4h] [ebp-784h]
    float v10; // [esp+8h] [ebp-780h]
    float v11; // [esp+Ch] [ebp-77Ch]
    tScriptStruct_BoueeAntiAircraft* v12; // [esp+10h] [ebp-778h]
    tGameObjectMain* v13; // [esp+20h] [ebp-768h]
    float v14; // [esp+2Ch] [ebp-75Ch]
    tGameObjectMain* v15; // [esp+34h] [ebp-754h]
    tGameObjectMain* v16; // [esp+44h] [ebp-744h]
    float v17; // [esp+48h] [ebp-740h]
    float v18; // [esp+54h] [ebp-734h]
    tGameObjectMain* v19; // [esp+70h] [ebp-718h]
    tGameObjectMain* v20; // [esp+78h] [ebp-710h]
    tGameObjectMain* v21; // [esp+8Ch] [ebp-6FCh]
    tGameObjectMain* v22; // [esp+94h] [ebp-6F4h]
    tGameObjectMain* v23; // [esp+98h] [ebp-6F0h]
    Vec3* v24; // [esp+A4h] [ebp-6E4h]
    float v25; // [esp+A8h] [ebp-6E0h]
    float v26; // [esp+C4h] [ebp-6C4h]
    tGameObjectMain* v27; // [esp+D4h] [ebp-6B4h]
    float v28; // [esp+D8h] [ebp-6B0h]
    float v29; // [esp+E4h] [ebp-6A4h]
    float v30; // [esp+E8h] [ebp-6A0h]
    tGameObjectMain* v31; // [esp+F0h] [ebp-698h]
    tGameObjectMain* v32; // [esp+100h] [ebp-688h]
    float v33; // [esp+104h] [ebp-684h]
    float v34; // [esp+108h] [ebp-680h]
    float v35; // [esp+114h] [ebp-674h]
    tGameObjectMain* v36; // [esp+130h] [ebp-658h]
    tGameObjectMain* v37; // [esp+138h] [ebp-650h]
    tGameObjectMain* v38; // [esp+148h] [ebp-640h]
    float v39; // [esp+154h] [ebp-634h]
    tGameObjectMain* v40; // [esp+15Ch] [ebp-62Ch]
    tGameObjectMain* v41; // [esp+16Ch] [ebp-61Ch]
    float v42; // [esp+170h] [ebp-618h]
    float v43; // [esp+17Ch] [ebp-60Ch]
    tGameObjectMain* v44; // [esp+198h] [ebp-5F0h]
    tGameObjectMain* v45; // [esp+1A0h] [ebp-5E8h]
    tGameObjectMain* v46; // [esp+1B4h] [ebp-5D4h]
    tGameObjectMain* v47; // [esp+1BCh] [ebp-5CCh]
    tGameObjectMain* v48; // [esp+1C0h] [ebp-5C8h]
    Vec3* v49; // [esp+1CCh] [ebp-5BCh]
    float v50; // [esp+1D0h] [ebp-5B8h]
    float v51; // [esp+1ECh] [ebp-59Ch]
    tGameObjectMain* v52; // [esp+1FCh] [ebp-58Ch]
    float v53; // [esp+200h] [ebp-588h]
    float v54; // [esp+20Ch] [ebp-57Ch]
    float v55; // [esp+210h] [ebp-578h]
    tGameObjectMain* v56; // [esp+218h] [ebp-570h]
    tGameObjectMain* v57; // [esp+228h] [ebp-560h]
    float v58; // [esp+22Ch] [ebp-55Ch]
    float v59; // [esp+230h] [ebp-558h]
    float v60; // [esp+23Ch] [ebp-54Ch]
    tGameObjectMain* v61; // [esp+250h] [ebp-538h]
    tGameObjectMain* v62; // [esp+258h] [ebp-530h]
    float v63; // [esp+268h] [ebp-520h]
    float v64; // [esp+26Ch] [ebp-51Ch]
    float v65; // [esp+270h] [ebp-518h]
    Vec3 v66; // [esp+280h] [ebp-508h] BYREF
    float v67; // [esp+28Ch] [ebp-4FCh]
    float v68; // [esp+290h] [ebp-4F8h]
    float v69; // [esp+294h] [ebp-4F4h]
    Vec3 v70; // [esp+298h] [ebp-4F0h] BYREF
    float v71; // [esp+2A4h] [ebp-4E4h]
    float v72; // [esp+2A8h] [ebp-4E0h]
    float v73; // [esp+2ACh] [ebp-4DCh]
    Vec3 v74; // [esp+2B0h] [ebp-4D8h] BYREF
    Vec3 v75; // [esp+2BCh] [ebp-4CCh] BYREF
    Vec3 v76; // [esp+2C8h] [ebp-4C0h] BYREF
    Vec3 v77; // [esp+2E4h] [ebp-4A4h] BYREF
    float v78; // [esp+2F0h] [ebp-498h]
    float v79; // [esp+2F4h] [ebp-494h]
    float v80; // [esp+2F8h] [ebp-490h]
    Vec3 v81; // [esp+2FCh] [ebp-48Ch] BYREF
    Vec3 v82; // [esp+308h] [ebp-480h] BYREF
    Vec3 v83; // [esp+314h] [ebp-474h] BYREF
    Vec3 v84; // [esp+320h] [ebp-468h] BYREF
    float v85; // [esp+32Ch] [ebp-45Ch]
    float v86; // [esp+330h] [ebp-458h]
    float v87; // [esp+334h] [ebp-454h]
    tMessage Src; // [esp+338h] [ebp-450h] BYREF
    float v90; // [esp+374h] [ebp-414h]
    float v91; // [esp+378h] [ebp-410h]
    float v92; // [esp+37Ch] [ebp-40Ch]
    Vec3 v93; // [esp+380h] [ebp-408h] BYREF
    float v94; // [esp+38Ch] [ebp-3FCh]
    float v95; // [esp+390h] [ebp-3F8h]
    float v96; // [esp+394h] [ebp-3F4h]
    Vec3 v97; // [esp+398h] [ebp-3F0h]
    Vec3 v98; // [esp+3B4h] [ebp-3D4h] BYREF
    Vec3 v99; // [esp+3C0h] [ebp-3C8h] BYREF
    Vec3 v100; // [esp+3D8h] [ebp-3B0h] BYREF
    Vec3 v101; // [esp+3E4h] [ebp-3A4h] BYREF
    Vec3 v102; // [esp+3F4h] [ebp-394h] BYREF
    Vec3 v103; // [esp+400h] [ebp-388h] BYREF
    Vec3 v104; // [esp+418h] [ebp-370h] BYREF
    Vec3 v105; // [esp+424h] [ebp-364h] BYREF
    float v106; // [esp+440h] [ebp-348h]
    Vec3 v108; // [esp+448h] [ebp-340h] BYREF
    Vec3 a3; // [esp+454h] [ebp-334h] BYREF
    Vec3 v110; // [esp+460h] [ebp-328h] BYREF
    float v111; // [esp+46Ch] [ebp-31Ch]
    float v112; // [esp+470h] [ebp-318h]
    int v113; // [esp+474h] [ebp-314h]
    Vec3 v114; // [esp+478h] [ebp-310h] BYREF
    float v115; // [esp+484h] [ebp-304h]
    float v116; // [esp+488h] [ebp-300h]
    float v117; // [esp+48Ch] [ebp-2FCh]
    Vec3 v118; // [esp+490h] [ebp-2F8h] BYREF
    Vec3 v119; // [esp+49Ch] [ebp-2ECh] BYREF
    float v120; // [esp+4ACh] [ebp-2DCh]
    float v121; // [esp+4B0h] [ebp-2D8h]
    float v122; // [esp+4B4h] [ebp-2D4h]
    float v123; // [esp+4B8h] [ebp-2D0h]
    float v124; // [esp+4BCh] [ebp-2CCh]
    float v125; // [esp+4D8h] [ebp-2B0h]
    float v126; // [esp+4DCh] [ebp-2ACh]
    float v127; // [esp+4E0h] [ebp-2A8h]
    float v128; // [esp+4E4h] [ebp-2A4h]
    float v129; // [esp+4E8h] [ebp-2A0h]
    float v130; // [esp+4ECh] [ebp-29Ch]
    Vec3 v131; // [esp+4F0h] [ebp-298h] BYREF
    float v132; // [esp+500h] [ebp-288h]
    int v133; // [esp+508h] [ebp-280h]
    float v135; // [esp+510h] [ebp-278h]
    float v136; // [esp+514h] [ebp-274h]
    float v137; // [esp+518h] [ebp-270h]
    Vec3 v138; // [esp+51Ch] [ebp-26Ch] BYREF
    float v139; // [esp+528h] [ebp-260h]
    float v140; // [esp+52Ch] [ebp-25Ch]
    float v141; // [esp+530h] [ebp-258h]
    Vec3 v142; // [esp+534h] [ebp-254h] BYREF
    Vec3 v143; // [esp+540h] [ebp-248h] BYREF
    Vec3 v144; // [esp+558h] [ebp-230h] BYREF
    BOOL v145; // [esp+564h] [ebp-224h]
    int v147; // [esp+56Ch] [ebp-21Ch]
    float v148; // [esp+574h] [ebp-214h]
    Vec3 v149; // [esp+578h] [ebp-210h] BYREF
    Vec3 v150; // [esp+584h] [ebp-204h] BYREF
    Vec3 v; // [esp+590h] [ebp-1F8h] BYREF
    Vec3 v152; // [esp+59Ch] [ebp-1ECh] BYREF
    Vec3 v153; // [esp+5A8h] [ebp-1E0h] BYREF
    Vec3 v154; // [esp+5B4h] [ebp-1D4h] BYREF
    Vec3 v155; // [esp+5C0h] [ebp-1C8h] BYREF
    Vec3 v156; // [esp+5CCh] [ebp-1BCh] BYREF
    Vec3 v157; // [esp+5D8h] [ebp-1B0h] BYREF
    int v158; // [esp+5E8h] [ebp-1A0h]
    float v159; // [esp+5ECh] [ebp-19Ch]
    float v160; // [esp+5F0h] [ebp-198h]
    float v161; // [esp+5F4h] [ebp-194h]
    Vec3 v162; // [esp+5F8h] [ebp-190h] BYREF
    Vec3 dest; // [esp+604h] [ebp-184h] BYREF
    float v164; // [esp+610h] [ebp-178h]
    float v165; // [esp+614h] [ebp-174h]
    float v166; // [esp+618h] [ebp-170h]
    Vec3 v167; // [esp+624h] [ebp-164h] BYREF
    float v168; // [esp+630h] [ebp-158h]
    float v169; // [esp+634h] [ebp-154h]
    float v170; // [esp+638h] [ebp-150h]
    Vec3 v171; // [esp+63Ch] [ebp-14Ch] BYREF
    Vec3 v172; // [esp+648h] [ebp-140h] BYREF
    Vec3 v173; // [esp+654h] [ebp-134h] BYREF
    int v174; // [esp+670h] [ebp-118h]
    int v175; // [esp+674h] [ebp-114h]
    float v176; // [esp+678h] [ebp-110h]
    int v177; // [esp+67Ch] [ebp-10Ch]
    Vec3 v178; // [esp+680h] [ebp-108h] BYREF
    Vec3 v179; // [esp+68Ch] [ebp-FCh] BYREF
    Vec3 v180; // [esp+698h] [ebp-F0h] BYREF
    Vec3 v181; // [esp+6BCh] [ebp-CCh] BYREF
    int v182; // [esp+6D0h] [ebp-B8h]
    Vec3 v183; // [esp+6D4h] [ebp-B4h] BYREF
    float v184; // [esp+6E0h] [ebp-A8h]
    float v185; // [esp+6E4h] [ebp-A4h]
    float v186; // [esp+6E8h] [ebp-A0h]
    tScriptStruct_BoueeAntiAircraft* v187; // [esp+6ECh] [ebp-9Ch]
    float v188; // [esp+6F0h] [ebp-98h]
    float v189; // [esp+6F4h] [ebp-94h]
    float v190; // [esp+6F8h] [ebp-90h]
    float v191; // [esp+6FCh] [ebp-8Ch]
    float v192; // [esp+700h] [ebp-88h]
    float v193; // [esp+704h] [ebp-84h]
    float v194; // [esp+708h] [ebp-80h]
    Vec3 v195; // [esp+70Ch] [ebp-7Ch] BYREF
    Vec3 v196; // [esp+718h] [ebp-70h] BYREF
    Vec3 v197; // [esp+724h] [ebp-64h] BYREF
    float v198; // [esp+734h] [ebp-54h]
    Vec3 v199; // [esp+73Ch] [ebp-4Ch] BYREF
    Vec3 v200; // [esp+748h] [ebp-40h] BYREF
    Vec3 v201; // [esp+754h] [ebp-34h] BYREF
    float v202; // [esp+760h] [ebp-28h]
    float v203; // [esp+764h] [ebp-24h]
    float v204; // [esp+768h] [ebp-20h]
    int v205; // [esp+770h] [ebp-18h]
    int v206; // [esp+774h] [ebp-14h]
    int v208; // [esp+77Ch] [ebp-Ch]
    float v209; // [esp+780h] [ebp-8h]

    v.x = 1.0;
    v.y = 0.0;
    v.z = 0.0;
    v114.x = 1.0;
    v114.y = 0.0;
    v114.z = 0.0;
    v97.x = 1.0;
    v97.y = 0.0;
    v97.z = 0.0;
    v144.x = 1.0;
    v144.y = 0.0;
    v144.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v12 = (tScriptStruct_BoueeAntiAircraft*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v12 = 0;
    v187 = v12;
    v145 = 0;
    v111 = 0.0;
    v120 = 0.0;
    v147 = 0;
    v206 = 0;
    v209 = 0.0;
    v112 = 0.0;
    v121 = 0.0;
    v177 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;        
        Bouee_Anti_Aircraft_BAA_exec_interception__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        Bouee_Anti_Aircraft_BAA_exec_interception_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Bouee_Anti_Aircraft_BAA_exec_interception_univVars_Flag_speciaux_sauve,
            (void*)Bouee_Anti_Aircraft_BAA_exec_interception_univVars,
            4488);
        Bouee_Anti_Aircraft_BAA_exec_interception_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        AI2C_ResolveBuffer(
            &Bouee_Anti_Aircraft_BAA_exec_interception_univVars_Perso_ID_passengers_list,
            (void*)Bouee_Anti_Aircraft_BAA_exec_interception_univVars,
            936);
    }
    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_interception__PJ_Aircraft_gao, &a3);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v181);
    MATH_SubstractVec(&dest, &a3, &v181);
    v97 = dest;
    v145 = AI_EvalFunc_ARRBitTest_C(
        (int*)(Bouee_Anti_Aircraft_BAA_exec_interception_univVars_Flag_speciaux_sauve + 4),
        2197);
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(1))
    {
        v187->f_time_GFX_suivant = TIM_gf_MainClock - TIM_gf_LockTime;
        if (v187->i_snd_laser_1 == -1)
        {
            v0 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 1, 0x18000000);
            v187->i_snd_laser_1 = v0;
            AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v187->i_snd_laser_1);
            v1 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 2, 0x18000000);
            v187->i_snd_laser_pres_1 = v1;
            AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v187->i_snd_laser_pres_1);
        }
        if (v187->GFX_Line_1 == -1)
        {
            v187->GFX_Line_1 = AI_EvalFunc_GFXAdd_C_CURRENT(1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v187->GFX_Line_1, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v187->GFX_Line_1, 2, 1);
            v62 = (tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_interception_SFX_light_smoke_gao;
            if (Bouee_Anti_Aircraft_BAA_exec_interception_SFX_light_smoke_gao)
            {
                if ((unsigned int)Bouee_Anti_Aircraft_BAA_exec_interception_SFX_light_smoke_gao <= 2)
                    v62 = AI_MainActor[Bouee_Anti_Aircraft_BAA_exec_interception_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v187->GFX_Line_1, v62, v187->des_mat_num);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v187->GFX_Line_1, 0, v187->des_mat_num);
            }
            AI_EvalFunc_GFXSetf_C_CURRENT(v187->GFX_Line_1, 1000, v187->f_line_width);
        }
        v198 = TIM_gf_dt;
        v8 = 4.0f * TIM_gf_dt;
        v113 = AI_EvalFunc_COLORBlend_C(v187->c_suivant, v187->des_line_color, v8);
        v187->c_suivant = v113;
        AI_EvalFunc_GFXSeti_C_CURRENT(v187->GFX_Line_1, 1100, v187->c_suivant);
        v61 = (tGameObjectMain*)v187->des_o_suivant;
        if (v61)
        {
            if ((unsigned int)v61 <= 2)
                v61 = AI_MainActor[(uint32_t)v61 - 1];
            AI_EvalFunc_OBJGetPos_C(v61, &v156);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v156);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v98);
        MATH_SubstractVec(&v199, &v156, &v98);
        v = v199;
        v209 = MATH_VecLen(&v);
        v60 = 1.0f / v209;
        v.x = v.x * v60;
        v.y = v.y * v60;
        v.z = v.z * v60;
        if (v187->des_comportement)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v110);
            v114 = v110;
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v131);
            v59 = v187->f_line_Z_offset;
            v78 = v131.x * v59;
            v79 = v131.y * v59;
            v80 = v131.z * v59;
            v114.x = v114.x + v78;
            v114.y = v114.y + v79;
            v114.z = v114.z + v80;
            v58 = v187->des_bouee_rayon;
            v125 = v.x * v58;
            v126 = v.y * v58;
            v127 = v.z * v58;
            v114.x = v114.x + v125;
            v114.y = v114.y + v126;
            v114.z = v114.z + v127;
            GFXSetCurGlobal(v187->GFX_Line_1);
            AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &v114);
            v57 = (tGameObjectMain*)v187->des_o_suivant;
            if (v57)
            {
                if ((unsigned int)v57 <= 2)
                    v57 = AI_MainActor[(uint32_t)v57 - 1];
                AI_EvalFunc_OBJGetPos_C(v57, &v171);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v171);
            }
            v114 = v171;
            v56 = (tGameObjectMain*)v187->des_o_suivant;
            if (v56)
            {
                if ((unsigned int)v56 <= 2)
                    v56 = AI_MainActor[(uint32_t)v56 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v56, &v153);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v153);
            }
            v55 = v187->f_line_Z_offset;
            v202 = v153.x * v55;
            v203 = v153.y * v55;
            v204 = v153.z * v55;
            v114.x = v114.x + v202;
            v114.y = v114.y + v203;
            v114.z = v114.z + v204;
            v54 = v187->des_bouee_rayon;
            v159 = v.x * v54;
            v160 = v.y * v54;
            v161 = v.z * v54;
            v114.x = v114.x - v159;
            v114.y = v114.y - v160;
            v114.z = v114.z - v161;
            AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &v114);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v118);
            v114 = v118;
            v114.z = v187->f_last_valid_water_Z + v187->f_line_Z_offset;
            v53 = v187->des_bouee_rayon;
            v115 = v.x * v53;
            v116 = v.y * v53;
            v117 = v.z * v53;
            v114.x = v118.x + v115;
            v114.y = v118.y + v116;
            v114.z = v114.z + v117;
            GFXSetCurGlobal(v187->GFX_Line_1);
            AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &v114);
            v52 = (tGameObjectMain*)v187->des_o_suivant;
            if (v52)
            {
                if ((unsigned int)v52 <= 2)
                    v52 = AI_MainActor[(uint32_t)v52 - 1];
                AI_EvalFunc_OBJGetPos_C(v52, &v173);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v173);
            }
            v114 = v173;
            v114.z = v187->f_last_valid_water_Z + v187->f_line_Z_offset;
            v51 = v187->des_bouee_rayon;
            v94 = v.x * v51;
            v95 = v.y * v51;
            v96 = v.z * v51;
            v114.x = v173.x - v94;
            v114.y = v173.y - v95;
            v114.z = v114.z - v96;
            AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &v114);
        }
        v111 = v.z * v97.z + v.y * v97.y + v97.x * v.x;
        if (v187->i_snd_laser_1 != -1)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v138);
            v144 = v138;
            if (v111 > (double)v209)
                v11 = v209;
            else
                v11 = v111;
            v188 = v11;
            v139 = v.x * v11;
            v140 = v.y * v11;
            v141 = v.z * v11;
            v144.x = v144.x + v139;
            v144.y = v144.y + v140;
            v144.z = v144.z + v141;
            AI_EvalFunc_SNDInstPosVSet_C(v187->i_snd_laser_1, &v144);
            AI_EvalFunc_SNDInstPosVSet_C(v187->i_snd_laser_pres_1, &v144);
        }
        v206 = 1;
        v158 = AI_EvalFunc_COLSetActivationGet_C((tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_interception__PJ_Aircraft_gao);
        if ((v158 & 1) != 0)
        {
            if (v111 >= -10.0 && v209 + 10.0 >= v111)
            {
                if (v145)
                {
                    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_interception__PJ_Aircraft_gao, &v195);
                    v121 = v195.z;
                    if (v187->des_comportement)
                    {
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&v102);
                        v121 = v121 - v102.z;
                    }
                    else
                    {
                        v121 = v121 - (v187->f_last_valid_water_Z + v187->f_line_Z_offset);
                    }
                    if (v187->des_f_Z_min > (double)v121 || v187->des_f_Z_max < (double)v121)
                        v206 = 0;
                }
            }
            else
            {
                v206 = 0;
            }
        }
        else
        {
            v206 = 0;
        }
        v201.x = 0.0;
        v201.y = 0.0;
        v201.z = 1.0;
        AI_EvalFunc_MATHVecCrossProduct_C(&v, &v201, &v167);
        v = v167;
        v111 = v97.z * v167.z + v97.y * v167.y + v167.x * v97.x;
        v112 = AI_EvalFunc_MTHFloatSign_C(v111);
        if (v206)
        {
            if (v187->f_air_suiv_last_side != v112)
            {
                AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_interception__PJ_Aircraft_gao, &v179);
                v189 = v.x * v111;
                v190 = v.y * v111;
                v191 = v.z * v111;
                v114.x = v179.x - v189;
                v114.y = v179.y - v190;
                v114.z = v179.z - v191;
                AI_EvalFunc_OBJSetPos_C((tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_interception__PJ_Aircraft_gao, &v114);
                v149.x = 0.0;
                v149.y = 0.0;
                v149.z = 0.0;
                v133 = AI_EvalFunc_DYNSpeedSetVector_C(
                    (tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_interception__PJ_Aircraft_gao,
                    &v149);
                v175 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000080);
                v177 = v175;
                AI_EvalFunc_SNDUltraPlaySound_C(
                    (tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_interception__PJ_Aircraft_gao,
                    v175);
                v.x = v.x * 50.0f;
                v.y = v.y * 50.0f;
                v.z = v.z * 50.0f;
                v187->m_msg.msgID = 37;
                v187->m_msg.gao = AI_gpst_CurrentGameObject;
                v187->m_msg.gao2 = 0;
                v187->m_msg.Int1 = 262246;
                v187->m_msg.Int2 = 1;
                v50 = v187->f_air_suiv_last_side;
                v49 = &v187->m_msg.v1;
                v187->m_msg.v1.x = v.x * v50;
                v49->y = v.y * v50;
                v49->z = v.z * v50;
                v187->m_msg.v2 = v114;
                v187->m_msg.f2 = 20.0;
                v187->m_msg.f1 = 1000.0;
                AI_EvalFunc_MSGSend_C(
                    (tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_interception__PJ_Aircraft_gao,
                    &v187->m_msg);
                v48 = (tGameObjectMain*)v187->des_o_acteur_texte;
                if (v48)
                {
                    if ((unsigned int)v48 <= 2)
                        v48 = AI_MainActor[(uint32_t)v48 - 1];
                    v47 = v48;
                }
                else
                {
                    v47 = 0;
                }
                if (v47)
                {
                    if (AI_EvalFunc_TIMEElapsed_C(v187->f_time_affiche_texte, 5.0))
                    {
                        if ((*(uint32_t*)(Bouee_Anti_Aircraft_BAA_exec_interception_univVars_Perso_ID_passengers_list + 44) & 1) != 0)
                        {
                            v2 = AI_EvalFunc_MTHModulo_C(v187->i_text_index + 1, 3);
                            v187->i_text_index = v2;
                            v147 = 10;
                            v147 = v187->i_text_index + 10;
                            Src.msgID = 30;
                            Src.Int1 = v147;
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
                            v46 = (tGameObjectMain*)v187->des_o_acteur_texte;
                            if (!v46)
                            {
                            LABEL_67:
                                AI_EvalFunc_MSGSend_C(0, &Src);
                                goto LABEL_106;
                            }
                            if ((unsigned int)v46 <= 2)
                                v46 = AI_MainActor[(uint32_t)v46 - 1];
                            AI_EvalFunc_MSGSend_C(v46, &Src);
                        }
                        else if ((*(uint32_t*)(Bouee_Anti_Aircraft_BAA_exec_interception_univVars_Perso_ID_passengers_list + 44) & 2) != 0)
                        {
                            v147 = AI_EvalFunc_ARRBitTest_C(
                                (int*)(Bouee_Anti_Aircraft_BAA_exec_interception_univVars_Flag_speciaux_sauve + 4),
                                2197);
                            if (!v147)
                            {
                                v147 = 15;
                                AI_EvalFunc_ARRBitSet_C(
                                    (int*)(Bouee_Anti_Aircraft_BAA_exec_interception_univVars_Flag_speciaux_sauve + 4),
                                    3776);
                                Src.msgID = 30;
                                Src.Int1 = v147;
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
                                v45 = (tGameObjectMain*)v187->des_o_acteur_texte;
                                if (!v45)
                                    goto LABEL_67;
                                if ((unsigned int)v45 <= 2)
                                    v45 = AI_MainActor[(uint32_t)v45 - 1];
                                AI_EvalFunc_MSGSend_C(v45, &Src);
                            }
                        }
                    }
                }
            }
        }
        else
        {
            v187->f_air_suiv_last_side = v112;
        }
    }
    else if (v187->GFX_Line_1 != -1)
    {
        if (v187->i_snd_laser_1 != -1)
        {
            SND_Release(v187->i_snd_laser_1);
            v187->i_snd_laser_1 = -1;
            SND_Release(v187->i_snd_laser_pres_1);
            v187->i_snd_laser_pres_1 = -1;
        }
        if (AI_EvalFunc_TIMEElapsed_C(v187->f_time_GFX_suivant, 1.0))
        {
            v187->c_suivant = 0;
            AI_EvalFunc_GFXDel_C_CURRENT(v187->GFX_Line_1);
            v187->GFX_Line_1 = -1;
        }
        else
        {
            v132 = TIM_gf_MainClock - TIM_gf_LockTime;
            v120 = v132 - v187->f_time_GFX_suivant;
            v3 = AI_EvalFunc_COLORBlend_C(v187->c_suivant, 0, v120);
            v187->c_suivant = v3;
            AI_EvalFunc_GFXSeti_C_CURRENT(v187->GFX_Line_1, 1100, v187->c_suivant);
            v44 = (tGameObjectMain*)v187->des_o_suivant;
            if (v44)
            {
                if ((unsigned int)v44 <= 2)
                    v44 = AI_MainActor[(uint32_t)v44 - 1];
                AI_EvalFunc_OBJGetPos_C(v44, &v180);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v180);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v76);
            MATH_SubstractVec(&v157, &v180, &v76);
            v = v157;
            MATH_NormalizeEqualVector(&v);
            v43 = v187->des_bouee_rayon;
            v.x = v.x * v43;
            v.y = v.y * v43;
            v.z = v.z * v43;
            if (v187->des_comportement)
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v101);
                v114 = v101;
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v150);
                v42 = v187->f_line_Z_offset;
                v63 = v150.x * v42;
                v64 = v150.y * v42;
                v65 = v150.z * v42;
                v114.x = v114.x + v63;
                v114.y = v114.y + v64;
                v114.z = v114.z + v65;
                v114.x = v114.x + v.x;
                v114.y = v114.y + v.y;
                v114.z = v114.z + v.z;
                GFXSetCurGlobal(v187->GFX_Line_1);
                AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &v114);
                v41 = (tGameObjectMain*)v187->des_o_suivant;
                if (v41)
                {
                    if ((unsigned int)v41 <= 2)
                        v41 = AI_MainActor[(uint32_t)v41 - 1];
                    AI_EvalFunc_OBJGetPos_C(v41, &v105);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v105);
                }
                v114 = v105;
                v40 = (tGameObjectMain*)v187->des_o_suivant;
                if (v40)
                {
                    if ((unsigned int)v40 <= 2)
                        v40 = AI_MainActor[(uint32_t)v40 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v40, &v70);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v70);
                }
                v39 = v187->f_line_Z_offset;
                v192 = v70.x * v39;
                v193 = v70.y * v39;
                v194 = v70.z * v39;
                v114.x = v114.x + v192;
                v114.y = v114.y + v193;
                v114.z = v114.z + v194;
                v114.x = v114.x - v.x;
                v114.y = v114.y - v.y;
                v114.z = v114.z - v.z;
                AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &v114);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v82);
                v114 = v82;
                v114.z = v187->f_last_valid_water_Z + v187->f_line_Z_offset;
                v114.x = v82.x + v.x;
                v114.y = v82.y + v.y;
                v114.z = v114.z + v.z;
                GFXSetCurGlobal(v187->GFX_Line_1);
                AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &v114);
                v38 = (tGameObjectMain*)v187->des_o_suivant;
                if (v38)
                {
                    if ((unsigned int)v38 <= 2)
                        v38 = AI_MainActor[(uint32_t)v38 - 1];
                    AI_EvalFunc_OBJGetPos_C(v38, &v75);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v75);
                }
                v114 = v75;
                v114.z = v187->f_last_valid_water_Z + v187->f_line_Z_offset;
                v114.x = v75.x - v.x;
                v114.y = v75.y - v.y;
                v114.z = v114.z - v.z;
                AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &v114);
            }
        }
    }
LABEL_106:
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(2))
    {
        v187->f_time_GFX_precedent = TIM_gf_MainClock - TIM_gf_LockTime;
        if (v187->i_snd_laser_2 == -1)
        {
            v4 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 1, 0x18000000);
            v187->i_snd_laser_2 = v4;
            AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v187->i_snd_laser_2);
            v5 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 2, 0x18000000);
            v187->i_snd_laser_pres_2 = v5;
            AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v187->i_snd_laser_pres_2);
        }
        if (v187->GFX_Line_2 == -1)
        {
            v187->GFX_Line_2 = AI_EvalFunc_GFXAdd_C_CURRENT(1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v187->GFX_Line_2, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v187->GFX_Line_2, 2, 1);
            v37 = (tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_interception_SFX_light_smoke_gao;
            if (Bouee_Anti_Aircraft_BAA_exec_interception_SFX_light_smoke_gao)
            {
                if ((unsigned int)Bouee_Anti_Aircraft_BAA_exec_interception_SFX_light_smoke_gao <= 2)
                    v37 = AI_MainActor[Bouee_Anti_Aircraft_BAA_exec_interception_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v187->GFX_Line_2, v37, v187->des_mat_num);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v187->GFX_Line_2, 0, v187->des_mat_num);
            }
            AI_EvalFunc_GFXSetf_C_CURRENT(v187->GFX_Line_2, 1000, v187->f_line_width);
        }
        v176 = TIM_gf_dt;
        v9 = 4.0f * TIM_gf_dt;
        v182 = AI_EvalFunc_COLORBlend_C(v187->c_precedent, v187->des_line_color, v9);
        v187->c_precedent = v182;
        AI_EvalFunc_GFXSeti_C_CURRENT(v187->GFX_Line_2, 1100, v187->c_precedent);
        v36 = (tGameObjectMain*)v187->des_o_precedent;
        if (v36)
        {
            if ((unsigned int)v36 <= 2)
                v36 = AI_MainActor[(uint32_t)v36 - 1];
            AI_EvalFunc_OBJGetPos_C(v36, &v77);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v77);
        }
        v114 = v77;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v162);
        MATH_SubstractVec(&v142, &v114, &v162);
        v = v142;
        v209 = MATH_VecLen(&v);
        v35 = 1.0f / v209;
        v.x = v.x * v35;
        v.y = v.y * v35;
        v.z = v.z * v35;
        if (v187->des_comportement)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v155);
            v114 = v155;
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v93);
            v34 = v187->f_line_Z_offset;
            v67 = v93.x * v34;
            v68 = v93.y * v34;
            v69 = v93.z * v34;
            v114.x = v114.x + v67;
            v114.y = v114.y + v68;
            v114.z = v114.z + v69;
            v33 = v187->des_bouee_rayon;
            v71 = v.x * v33;
            v72 = v.y * v33;
            v73 = v.z * v33;
            v114.x = v114.x + v71;
            v114.y = v114.y + v72;
            v114.z = v114.z + v73;
            GFXSetCurGlobal(v187->GFX_Line_2);
            AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &v114);
            v32 = (tGameObjectMain*)v187->des_o_precedent;
            if (v32)
            {
                if ((unsigned int)v32 <= 2)
                    v32 = AI_MainActor[(uint32_t)v32 - 1];
                AI_EvalFunc_OBJGetPos_C(v32, &v200);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v200);
            }
            v114 = v200;
            v31 = (tGameObjectMain*)v187->des_o_precedent;
            if (v31)
            {
                if ((unsigned int)v31 <= 2)
                    v31 = AI_MainActor[(uint32_t)v31 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v31, &v83);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v83);
            }
            v30 = v187->f_line_Z_offset;
            v164 = v83.x * v30;
            v165 = v83.y * v30;
            v166 = v83.z * v30;
            v114.x = v114.x + v164;
            v114.y = v114.y + v165;
            v114.z = v114.z + v166;
            v29 = v187->des_bouee_rayon;
            v168 = v.x * v29;
            v169 = v.y * v29;
            v170 = v.z * v29;
            v114.x = v114.x - v168;
            v114.y = v114.y - v169;
            v114.z = v114.z - v170;
            AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &v114);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v81);
            v114 = v81;
            v114.z = v187->f_last_valid_water_Z + v187->f_line_Z_offset;
            v28 = v187->des_bouee_rayon;
            v90 = v.x * v28;
            v91 = v.y * v28;
            v92 = v.z * v28;
            v114.x = v81.x + v90;
            v114.y = v81.y + v91;
            v114.z = v114.z + v92;
            GFXSetCurGlobal(v187->GFX_Line_2);
            AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &v114);
            v27 = (tGameObjectMain*)v187->des_o_precedent;
            if (v27)
            {
                if ((unsigned int)v27 <= 2)
                    v27 = AI_MainActor[(uint32_t)v27 - 1];
                AI_EvalFunc_OBJGetPos_C(v27, &v119);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v119);
            }
            v114 = v119;
            v114.z = v187->f_last_valid_water_Z + v187->f_line_Z_offset;
            v26 = v187->des_bouee_rayon;
            v135 = v.x * v26;
            v136 = v.y * v26;
            v137 = v.z * v26;
            v114.x = v119.x - v135;
            v114.y = v119.y - v136;
            v114.z = v114.z - v137;
            AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &v114);
        }
        v111 = v.z * v97.z + v.y * v97.y + v97.x * v.x;
        if (v187->i_snd_laser_2 != -1)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v66);
            v144 = v66;
            if (v111 > (double)v209)
                v10 = v209;
            else
                v10 = v111;
            v148 = v10;
            v128 = v.x * v10;
            v129 = v.y * v10;
            v130 = v.z * v10;
            v144.x = v144.x + v128;
            v144.y = v144.y + v129;
            v144.z = v144.z + v130;
            AI_EvalFunc_SNDInstPosVSet_C(v187->i_snd_laser_2, &v144);
            AI_EvalFunc_SNDInstPosVSet_C(v187->i_snd_laser_pres_2, &v144);
        }
        v206 = 1;
        v208 = AI_EvalFunc_COLSetActivationGet_C((tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_interception__PJ_Aircraft_gao);
        if ((v208 & 1) != 0)
        {
            if (v111 >= -10.0 && v209 + 10.0 >= v111)
            {
                if (v145)
                {
                    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_interception__PJ_Aircraft_gao, &v197);
                    v121 = v197.z;
                    if (v187->des_comportement)
                    {
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&v100);
                        v121 = v121 - v100.z;
                    }
                    else
                    {
                        v121 = v121 - (v187->f_last_valid_water_Z + v187->f_line_Z_offset);
                    }
                    if (v187->des_f_Z_min > (double)v121 || v187->des_f_Z_max < (double)v121)
                        v206 = 0;
                }
            }
            else
            {
                v206 = 0;
            }
        }
        else
        {
            v206 = 0;
        }
        v196.x = 0.0;
        v196.y = 0.0;
        v196.z = 1.0;
        AI_EvalFunc_MATHVecCrossProduct_C(&v, &v196, &v103);
        v = v103;
        v111 = v97.z * v103.z + v97.y * v103.y + v103.x * v97.x;
        v112 = AI_EvalFunc_MTHFloatSign_C(v111);
        if (!v206)
        {
            v187->f_air_prec_last_side = v112;
            return 1000;
        }
        if (v187->f_air_prec_last_side != v112)
        {
            AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_interception__PJ_Aircraft_gao, &v172);
            v122 = v.x * v111;
            v123 = v.y * v111;
            v124 = v.z * v111;
            v114.x = v172.x - v122;
            v114.y = v172.y - v123;
            v114.z = v172.z - v124;
            AI_EvalFunc_OBJSetPos_C((tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_interception__PJ_Aircraft_gao, &v114);
            v99.x = 0.0;
            v99.y = 0.0;
            v99.z = 0.0;
            v205 = AI_EvalFunc_DYNSpeedSetVector_C(
                (tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_interception__PJ_Aircraft_gao,
                &v99);
            v174 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000080);
            v177 = v174;
            AI_EvalFunc_SNDUltraPlaySound_C(
                (tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_interception__PJ_Aircraft_gao,
                v174);
            v.x = v.x * 50.0f;
            v.y = v.y * 50.0f;
            v.z = v.z * 50.0f;
            v187->m_msg.msgID = 37;
            v187->m_msg.gao = AI_gpst_CurrentGameObject;
            v187->m_msg.gao2 = 0;
            v187->m_msg.Int1 = 262246;
            v187->m_msg.Int2 = 1;
            v25 = v187->f_air_prec_last_side;
            v24 = &v187->m_msg.v1;
            v187->m_msg.v1.x = v.x * v25;
            v24->y = v.y * v25;
            v24->z = v.z * v25;
            v187->m_msg.v2 = v114;
            v187->m_msg.f2 = 20.0;
            v187->m_msg.f1 = 1000.0;
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_interception__PJ_Aircraft_gao, &v187->m_msg);
            v23 = (tGameObjectMain*)v187->des_o_acteur_texte;
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
                if (AI_EvalFunc_TIMEElapsed_C(v187->f_time_affiche_texte, 5.0))
                {
                    if ((*(uint32_t*)(Bouee_Anti_Aircraft_BAA_exec_interception_univVars_Perso_ID_passengers_list + 44) & 1) != 0)
                    {
                        if (++v187->i_text_index > 3)
                            v187->i_text_index = 0;
                        v147 = 10;
                        v147 = v187->i_text_index + 10;
                        Src.msgID = 30;
                        Src.Int1 = v147;
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
                        v21 = (tGameObjectMain*)v187->des_o_acteur_texte;
                        if (!v21)
                            goto LABEL_169;
                        if ((unsigned int)v21 <= 2)
                            v21 = AI_MainActor[(uint32_t)v21 - 1];
                        AI_EvalFunc_MSGSend_C(v21, &Src);
                    }
                    else if ((*(uint32_t*)(Bouee_Anti_Aircraft_BAA_exec_interception_univVars_Perso_ID_passengers_list + 44) & 2) != 0)
                    {
                        v147 = AI_EvalFunc_ARRBitTest_C(
                            (int*)(Bouee_Anti_Aircraft_BAA_exec_interception_univVars_Flag_speciaux_sauve + 4),
                            2197);
                        if (!v147)
                        {
                            v147 = 15;
                            AI_EvalFunc_ARRBitSet_C(
                                (int*)(Bouee_Anti_Aircraft_BAA_exec_interception_univVars_Flag_speciaux_sauve + 4),
                                3776);
                            Src.msgID = 30;
                            Src.Int1 = v147;
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
                            v20 = (tGameObjectMain*)v187->des_o_acteur_texte;
                            if (!v20)
                            {
                            LABEL_169:
                                AI_EvalFunc_MSGSend_C(0, &Src);
                                return 1000;
                            }
                            if ((unsigned int)v20 <= 2)
                                v20 = AI_MainActor[(uint32_t)v20 - 1];
                            AI_EvalFunc_MSGSend_C(v20, &Src);
                        }
                    }
                }
            }
        }
    }
    else if (v187->GFX_Line_2 != -1)
    {
        if (v187->i_snd_laser_2 != -1)
        {
            SND_Release(v187->i_snd_laser_2);
            v187->i_snd_laser_2 = -1;
            SND_Release(v187->i_snd_laser_pres_2);
            v187->i_snd_laser_pres_2 = -1;
        }
        if (AI_EvalFunc_TIMEElapsed_C(v187->f_time_GFX_precedent, 1.0))
        {
            v187->c_precedent = 0;
            AI_EvalFunc_GFXDel_C_CURRENT(v187->GFX_Line_2);
            v187->GFX_Line_2 = -1;
        }
        else
        {
            v106 = TIM_gf_MainClock - TIM_gf_LockTime;
            v120 = v106 - v187->f_time_GFX_precedent;
            v6 = AI_EvalFunc_COLORBlend_C(v187->c_precedent, 0, v120);
            v187->c_precedent = v6;
            AI_EvalFunc_GFXSeti_C_CURRENT(v187->GFX_Line_2, 1100, v187->c_precedent);
            v19 = (tGameObjectMain*)v187->des_o_precedent;
            if (v19)
            {
                if ((unsigned int)v19 <= 2)
                    v19 = AI_MainActor[(uint32_t)v19 - 1];
                AI_EvalFunc_OBJGetPos_C(v19, &v178);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v178);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v143);
            MATH_SubstractVec(&v104, &v178, &v143);
            v = v104;
            MATH_NormalizeEqualVector(&v);
            v18 = v187->des_bouee_rayon;
            v.x = v.x * v18;
            v.y = v.y * v18;
            v.z = v.z * v18;
            if (v187->des_comportement)
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v74);
                v114 = v74;
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v84);
                v17 = v187->f_line_Z_offset;
                v184 = v84.x * v17;
                v185 = v84.y * v17;
                v186 = v84.z * v17;
                v114.x = v114.x + v184;
                v114.y = v114.y + v185;
                v114.z = v114.z + v186;
                v114.x = v114.x + v.x;
                v114.y = v114.y + v.y;
                v114.z = v114.z + v.z;
                GFXSetCurGlobal(v187->GFX_Line_2);
                AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &v114);
                v16 = (tGameObjectMain*)v187->des_o_precedent;
                if (v16)
                {
                    if ((unsigned int)v16 <= 2)
                        v16 = AI_MainActor[(uint32_t)v16 - 1];
                    AI_EvalFunc_OBJGetPos_C(v16, &v154);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v154);
                }
                v114 = v154;
                v15 = (tGameObjectMain*)v187->des_o_precedent;
                if (v15)
                {
                    if ((unsigned int)v15 <= 2)
                        v15 = AI_MainActor[(uint32_t)v15 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v15, &v108);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v108);
                }
                v14 = v187->f_line_Z_offset;
                v85 = v108.x * v14;
                v86 = v108.y * v14;
                v87 = v108.z * v14;
                v114.x = v114.x + v85;
                v114.y = v114.y + v86;
                v114.z = v114.z + v87;
                v114.x = v114.x - v.x;
                v114.y = v114.y - v.y;
                v114.z = v114.z - v.z;
                AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &v114);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v183);
                v114 = v183;
                v114.z = v187->f_last_valid_water_Z + v187->f_line_Z_offset;
                v114.x = v183.x + v.x;
                v114.y = v183.y + v.y;
                v114.z = v114.z + v.z;
                GFXSetCurGlobal(v187->GFX_Line_2);
                AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &v114);
                v13 = (tGameObjectMain*)v187->des_o_precedent;
                if (v13)
                {
                    if ((unsigned int)v13 <= 2)
                        v13 = AI_MainActor[(uint32_t)v13 - 1];
                    AI_EvalFunc_OBJGetPos_C(v13, &v152);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v152);
                }
                v114 = v152;
                v114.z = v187->f_last_valid_water_Z + v187->f_line_Z_offset;
                v114.x = v152.x - v.x;
                v114.y = v152.y - v.y;
                v114.z = v114.z - v.z;
                AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &v114);
            }
        }
    }
    return 1000;
}

int Bouee_Anti_Aircraft_BAA_exec_afterrec()
{
    BOOL v0; // eax
    BOOL v1; // eax
    BOOL v2; // eax
    BOOL v3; // eax
    tGameObjectMain* v4; // eax
    tGameObjectMain* v5; // eax
    tScriptStruct_BoueeAntiAircraft* v7; // [esp+0h] [ebp-238h]
    tAI* v8; // [esp+4h] [ebp-234h]
    tAI* v9; // [esp+8h] [ebp-230h]
    tGameObjectMain* v10; // [esp+Ch] [ebp-22Ch]
    tGameObjectMain* v11; // [esp+10h] [ebp-228h]
    tAI* v12; // [esp+14h] [ebp-224h]
    tAI* v13; // [esp+18h] [ebp-220h]
    tGameObjectMain* v14; // [esp+1Ch] [ebp-21Ch]
    tGameObjectMain* v15; // [esp+20h] [ebp-218h]
    tAI* v16; // [esp+24h] [ebp-214h]
    tAI* v17; // [esp+28h] [ebp-210h]
    tGameObjectMain* v18; // [esp+2Ch] [ebp-20Ch]
    tGameObjectMain* v19; // [esp+30h] [ebp-208h]
    tAI* v20; // [esp+34h] [ebp-204h]
    tAI* v21; // [esp+38h] [ebp-200h]
    tGameObjectMain* v22; // [esp+3Ch] [ebp-1FCh]
    tGameObjectMain* v23; // [esp+40h] [ebp-1F8h]
    tAI* v24; // [esp+54h] [ebp-1E4h]
    tAI* v25; // [esp+58h] [ebp-1E0h]
    tGameObjectMain* v26; // [esp+5Ch] [ebp-1DCh]
    tGameObjectMain* v27; // [esp+60h] [ebp-1D8h]
    tAI* v28; // [esp+64h] [ebp-1D4h]
    tAI* v29; // [esp+68h] [ebp-1D0h]
    tGameObjectMain* v30; // [esp+6Ch] [ebp-1CCh]
    tGameObjectMain* v31; // [esp+70h] [ebp-1C8h]
    tAI* v32; // [esp+74h] [ebp-1C4h]
    tAI* v33; // [esp+78h] [ebp-1C0h]
    tGameObjectMain* v34; // [esp+7Ch] [ebp-1BCh]
    tGameObjectMain* v35; // [esp+80h] [ebp-1B8h]
    tAI* v36; // [esp+84h] [ebp-1B4h]
    tAI* v37; // [esp+88h] [ebp-1B0h]
    tGameObjectMain* v38; // [esp+8Ch] [ebp-1ACh]
    tGameObjectMain* v39; // [esp+90h] [ebp-1A8h]
    tGameObjectMain* v40; // [esp+98h] [ebp-1A0h]
    tGameObjectMain* v41; // [esp+9Ch] [ebp-19Ch]
    tGameObjectMain* v42; // [esp+A0h] [ebp-198h]
    tAI* v43; // [esp+A4h] [ebp-194h]
    tAI* v44; // [esp+A8h] [ebp-190h]
    tGameObjectMain* v45; // [esp+ACh] [ebp-18Ch]
    tGameObjectMain* v46; // [esp+B0h] [ebp-188h]
    tAI* v47; // [esp+B4h] [ebp-184h]
    tAI* v48; // [esp+B8h] [ebp-180h]
    tGameObjectMain* v49; // [esp+BCh] [ebp-17Ch]
    tGameObjectMain* v50; // [esp+C0h] [ebp-178h]
    tAI* v51; // [esp+C4h] [ebp-174h]
    tAI* v52; // [esp+C8h] [ebp-170h]
    tGameObjectMain* v53; // [esp+CCh] [ebp-16Ch]
    tGameObjectMain* v54; // [esp+D0h] [ebp-168h]
    tAI* v55; // [esp+D4h] [ebp-164h]
    tAI* v56; // [esp+D8h] [ebp-160h]
    tGameObjectMain* v57; // [esp+DCh] [ebp-15Ch]
    tGameObjectMain* v58; // [esp+E0h] [ebp-158h]
    tAI* v59; // [esp+F4h] [ebp-144h]
    tAI* v60; // [esp+F8h] [ebp-140h]
    tGameObjectMain* v61; // [esp+FCh] [ebp-13Ch]
    tGameObjectMain* v62; // [esp+100h] [ebp-138h]
    tAI* v63; // [esp+104h] [ebp-134h]
    tAI* v64; // [esp+108h] [ebp-130h]
    tGameObjectMain* v65; // [esp+10Ch] [ebp-12Ch]
    tGameObjectMain* v66; // [esp+110h] [ebp-128h]
    tAI* v67; // [esp+114h] [ebp-124h]
    tAI* v68; // [esp+118h] [ebp-120h]
    tGameObjectMain* v69; // [esp+11Ch] [ebp-11Ch]
    tGameObjectMain* v70; // [esp+120h] [ebp-118h]
    tAI* v71; // [esp+124h] [ebp-114h]
    tAI* v72; // [esp+128h] [ebp-110h]
    tGameObjectMain* v73; // [esp+12Ch] [ebp-10Ch]
    tGameObjectMain* v74; // [esp+130h] [ebp-108h]
    tGameObjectMain* v75; // [esp+138h] [ebp-100h]
    tGameObjectMain* v76; // [esp+13Ch] [ebp-FCh]
    tGameObjectMain* v77; // [esp+140h] [ebp-F8h]
    tGameObjectMain* v78; // [esp+148h] [ebp-F0h]
    tGameObjectMain* v79; // [esp+150h] [ebp-E8h]
    tGameObjectMain* v80; // [esp+154h] [ebp-E4h]
    tGameObjectMain* v81; // [esp+158h] [ebp-E0h]
    tGameObjectMain* v82; // [esp+160h] [ebp-D8h]
    tGameObjectMain* v83; // [esp+168h] [ebp-D0h]
    tGameObjectMain* v84; // [esp+16Ch] [ebp-CCh]
    tGameObjectMain* v85; // [esp+170h] [ebp-C8h]
    Vec3 v86; // [esp+174h] [ebp-C4h] BYREF
    Vec3 a1; // [esp+184h] [ebp-B4h] BYREF
    int v88; // [esp+190h] [ebp-A8h]
    BOOL v89; // [esp+194h] [ebp-A4h]
    Vec3 pos; // [esp+198h] [ebp-A0h] BYREF
    Vec3 a2; // [esp+1A4h] [ebp-94h] BYREF
    BOOL v92; // [esp+1B0h] [ebp-88h]
    float v93; // [esp+1BCh] [ebp-7Ch]
    BOOL v94; // [esp+1C4h] [ebp-74h]
    Vec3 v95; // [esp+1C8h] [ebp-70h] BYREF
    Vec3 v96; // [esp+1D4h] [ebp-64h] BYREF
    float v97; // [esp+1E0h] [ebp-58h]
    Vec3 v98; // [esp+1E4h] [ebp-54h] BYREF
    Vec3 v99; // [esp+1F0h] [ebp-48h] BYREF
    BOOL v100; // [esp+1FCh] [ebp-3Ch]
    int v101; // [esp+200h] [ebp-38h]
    Vec3 v102; // [esp+204h] [ebp-34h] BYREF
    tScriptStruct_BoueeAntiAircraft* v103; // [esp+210h] [ebp-28h]
    int v104; // [esp+214h] [ebp-24h]
    BOOL v105; // [esp+218h] [ebp-20h]
    BOOL v106; // [esp+21Ch] [ebp-1Ch]
    Vec3 v107; // [esp+220h] [ebp-18h] BYREF
    Vec3 a3; // [esp+22Ch] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_BoueeAntiAircraft*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    v103 = v7;
    v104 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Bouee_Anti_Aircraft_BAA_exec_afterrec__PJ_Camera_gao = AI_ul_RealResolveOneRefObject(0x71003F56u);
        Bouee_Anti_Aircraft_BAA_exec_afterrec_SFX_Flash_texture_gao = AI_ul_RealResolveOneRefObject(0x3500941Au);
    }
    if (v103->GFX_Halo != -1)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v102);
        v99.x = v102.x * 10.0f;
        v99.y = v102.y * 10.0f;
        v99.z = v102.z * 10.0f;
        MATH_VecAddVec(&a1, &a2, &v99);
        GFXSetCurGlobal(v103->GFX_Halo);
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &a1);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v86);
        AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v86);
    }
    v104 = 0;
    v94 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(1);
    if (v94 || (v106 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(2)))
    {
        v104 = 1;
    }
    else
    {
        v85 = (tGameObjectMain*)v103->des_o_suivant;
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
        if (!v84)
            goto LABEL_30;
        v83 = (tGameObjectMain*)v103->des_o_suivant;
        if (v83)
        {
            if ((unsigned int)v83 <= 2)
                v83 = AI_MainActor[(uint32_t)v83 - 1];
            v0 = AI_EvalFunc_OBJ_CapaTest_C(v83, 1);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_CapaTest_C(0, 1);
        }
        v89 = v0;
        if (v0)
            goto LABEL_29;
        v82 = (tGameObjectMain*)v103->des_o_suivant;
        if (v82)
        {
            if ((unsigned int)v82 <= 2)
                v82 = AI_MainActor[(uint32_t)v82 - 1];
            v1 = AI_EvalFunc_OBJ_CapaTest_C(v82, 2);
        }
        else
        {
            v1 = AI_EvalFunc_OBJ_CapaTest_C(0, 2);
        }
        v100 = v1;
        if (v1)
        {
        LABEL_29:
            v104 = 1;
        }
        else
        {
        LABEL_30:
            v81 = (tGameObjectMain*)v103->des_o_precedent;
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
            if (v80)
            {
                v79 = (tGameObjectMain*)v103->des_o_precedent;
                if (v79)
                {
                    if ((unsigned int)v79 <= 2)
                        v79 = AI_MainActor[(uint32_t)v79 - 1];
                    v2 = AI_EvalFunc_OBJ_CapaTest_C(v79, 1);
                }
                else
                {
                    v2 = AI_EvalFunc_OBJ_CapaTest_C(0, 1);
                }
                v105 = v2;
                if (v2)
                    goto LABEL_48;
                v78 = (tGameObjectMain*)v103->des_o_precedent;
                if (v78)
                {
                    if ((unsigned int)v78 <= 2)
                        v78 = AI_MainActor[(uint32_t)v78 - 1];
                    v3 = AI_EvalFunc_OBJ_CapaTest_C(v78, 2);
                }
                else
                {
                    v3 = AI_EvalFunc_OBJ_CapaTest_C(0, 2);
                }
                v92 = v3;
                if (v3)
                    LABEL_48:
                v104 = 1;
            }
        }
    }
    if (v104)
    {
        v97 = TIM_gf_dt;
        v88 = AI_EvalFunc_COLORBlend_C(v103->c_halo, 0x1010FF, TIM_gf_dt);
        v103->c_halo = v88;
        if (v103->i_actif != v104)
        {
            v103->i_actif = v104;
            v77 = (tGameObjectMain*)v103->o_sfx_flash;
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
            if (!v76)
            {
                AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_afterrec__PJ_Camera_gao, &a3);
                v95.x = 0.0;
                v95.y = 0.0;
                v95.z = 0.0;
                MATH_VecAddVec(&pos, &a3, &v95);
                v4 = AI_EvalFunc_OBJDuplicate_C(
                    (tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_afterrec_SFX_Flash_texture_gao,
                    &pos);
                v103->o_sfx_flash = (int)v4;
                v75 = (tGameObjectMain*)v103->o_sfx_flash;
                if (v75)
                {
                    if ((unsigned int)v75 <= 2)
                        v75 = AI_MainActor[(uint32_t)v75 - 1];
                    AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, v75, &v103->o_sfx_flash);
                }
                else
                {
                    AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, 0, &v103->o_sfx_flash);
                }
            }
            v72 = (tAI*)v103->o_sfx_flash;
            if (v72 == WOR_gst_Universe.univAIInstance)
            {
                v71 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v74 = (tGameObjectMain*)v103->o_sfx_flash;
                if (v72)
                {
                    if ((unsigned int)v72 <= 2)
                        v74 = AI_MainActor[(uint32_t)v72 - 1];
                    v73 = v74;
                }
                else
                {
                    v73 = 0;
                }
                v71 = v73->extended->AIInstance;
            }
            ((tScriptStruct_SFXFlashTexture*)(v71->instanceVarDataCurrent))->i_flash_phase1 = 1;
            v68 = (tAI*)v103->o_sfx_flash;
            if (v68 == WOR_gst_Universe.univAIInstance)
            {
                v67 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v70 = (tGameObjectMain*)v103->o_sfx_flash;
                if (v68)
                {
                    if ((unsigned int)v68 <= 2)
                        v70 = AI_MainActor[(uint32_t)v68 - 1];
                    v69 = v70;
                }
                else
                {
                    v69 = 0;
                }
                v67 = v69->extended->AIInstance;
            }
            ((tScriptStruct_SFXFlashTexture*)(v67->instanceVarDataCurrent))->o_objet_cible = (int)AI_gpst_CurrentGameObject;
            v64 = (tAI*)v103->o_sfx_flash;
            if (v64 == WOR_gst_Universe.univAIInstance)
            {
                v63 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v66 = (tGameObjectMain*)v103->o_sfx_flash;
                if (v64)
                {
                    if ((unsigned int)v64 <= 2)
                        v66 = AI_MainActor[(uint32_t)v64 - 1];
                    v65 = v66;
                }
                else
                {
                    v65 = 0;
                }
                v63 = v65->extended->AIInstance;
            }
            ((tScriptStruct_SFXFlashTexture*)(v63->instanceVarDataCurrent))->des_i_submat = 0;
            v60 = (tAI*)v103->o_sfx_flash;
            if (v60 == WOR_gst_Universe.univAIInstance)
            {
                v59 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v62 = (tGameObjectMain*)v103->o_sfx_flash;
                if (v60)
                {
                    if ((unsigned int)v60 <= 2)
                        v62 = AI_MainActor[(uint32_t)v60 - 1];
                    v61 = v62;
                }
                else
                {
                    v61 = 0;
                }
                v59 = v61->extended->AIInstance;
            }
            ((tScriptStruct_SFXFlashTexture*)(v59->instanceVarDataCurrent))->des_c_couleur = 255;
            v56 = (tAI*)v103->o_sfx_flash;
            if (v56 == WOR_gst_Universe.univAIInstance)
            {
                v55 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v58 = (tGameObjectMain*)v103->o_sfx_flash;
                if (v56)
                {
                    if ((unsigned int)v56 <= 2)
                        v58 = AI_MainActor[(uint32_t)v56 - 1];
                    v57 = v58;
                }
                else
                {
                    v57 = 0;
                }
                v55 = v57->extended->AIInstance;
            }
            ((tScriptStruct_SFXFlashTexture*)(v55->instanceVarDataCurrent))->des_f_opacite = 0.75;
            v52 = (tAI*)v103->o_sfx_flash;
            if (v52 == WOR_gst_Universe.univAIInstance)
            {
                v51 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v54 = (tGameObjectMain*)v103->o_sfx_flash;
                if (v52)
                {
                    if ((unsigned int)v52 <= 2)
                        v54 = AI_MainActor[(uint32_t)v52 - 1];
                    v53 = v54;
                }
                else
                {
                    v53 = 0;
                }
                v51 = v53->extended->AIInstance;
            }
            ((tScriptStruct_SFXFlashTexture*)(v51->instanceVarDataCurrent))->des_f_temps_deb = 1.0;
            v48 = (tAI*)v103->o_sfx_flash;
            if (v48 == WOR_gst_Universe.univAIInstance)
            {
                v47 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v50 = (tGameObjectMain*)v103->o_sfx_flash;
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
            ((tScriptStruct_SFXFlashTexture*)(v47->instanceVarDataCurrent))->des_f_temps_fin = 0.0;
            v44 = (tAI*)v103->o_sfx_flash;
            if (v44 == WOR_gst_Universe.univAIInstance)
            {
                v43 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v46 = (tGameObjectMain*)v103->o_sfx_flash;
                if (v44)
                {
                    if ((unsigned int)v44 <= 2)
                        v46 = AI_MainActor[(uint32_t)v44 - 1];
                    v45 = v46;
                }
                else
                {
                    v45 = 0;
                }
                v43 = v45->extended->AIInstance;
            }
            ((tScriptStruct_SFXFlashTexture*)(v43->instanceVarDataCurrent))->des_i_type_effet = 1;
        }
    }
    else
    {
        v93 = TIM_gf_dt;
        v101 = AI_EvalFunc_COLORBlend_C(v103->c_halo, 0x3080A0, TIM_gf_dt);
        v103->c_halo = v101;
        if (v103->i_actif != v104)
        {
            v103->i_actif = v104;
            v42 = (tGameObjectMain*)v103->o_sfx_flash;
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
            if (!v41)
            {
                AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_afterrec__PJ_Camera_gao, &v96);
                v107.x = 0.0;
                v107.y = 0.0;
                v107.z = 0.0;
                MATH_VecAddVec(&v98, &v96, &v107);
                v5 = AI_EvalFunc_OBJDuplicate_C(
                    (tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_afterrec_SFX_Flash_texture_gao,
                    &v98);
                v103->o_sfx_flash = (int)v5;
                v40 = (tGameObjectMain*)v103->o_sfx_flash;
                if (v40)
                {
                    if ((unsigned int)v40 <= 2)
                        v40 = AI_MainActor[(uint32_t)v40 - 1];
                    AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, v40, &v103->o_sfx_flash);
                }
                else
                {
                    AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, 0, &v103->o_sfx_flash);
                }
            }
            v37 = (tAI*)v103->o_sfx_flash;
            if (v37 == WOR_gst_Universe.univAIInstance)
            {
                v36 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v39 = (tGameObjectMain*)v103->o_sfx_flash;
                if (v37)
                {
                    if ((unsigned int)v37 <= 2)
                        v39 = AI_MainActor[(uint32_t)v37 - 1];
                    v38 = v39;
                }
                else
                {
                    v38 = 0;
                }
                v36 = v38->extended->AIInstance;
            }
            ((tScriptStruct_SFXFlashTexture*)(v36->instanceVarDataCurrent))->i_flash_phase1 = 1;
            v33 = (tAI*)v103->o_sfx_flash;
            if (v33 == WOR_gst_Universe.univAIInstance)
            {
                v32 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v35 = (tGameObjectMain*)v103->o_sfx_flash;
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
            ((tScriptStruct_SFXFlashTexture*)(v32->instanceVarDataCurrent))->o_objet_cible = (int)AI_gpst_CurrentGameObject;
            v29 = (tAI*)v103->o_sfx_flash;
            if (v29 == WOR_gst_Universe.univAIInstance)
            {
                v28 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v31 = (tGameObjectMain*)v103->o_sfx_flash;
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
            ((tScriptStruct_SFXFlashTexture*)(v28->instanceVarDataCurrent))->des_i_submat = 0;
            v25 = (tAI*)v103->o_sfx_flash;
            if (v25 == WOR_gst_Universe.univAIInstance)
            {
                v24 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v27 = (tGameObjectMain*)v103->o_sfx_flash;
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
            ((tScriptStruct_SFXFlashTexture*)(v24->instanceVarDataCurrent))->des_c_couleur = 255;
            v21 = (tAI*)v103->o_sfx_flash;
            if (v21 == WOR_gst_Universe.univAIInstance)
            {
                v20 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v23 = (tGameObjectMain*)v103->o_sfx_flash;
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
            ((tScriptStruct_SFXFlashTexture*)(v20->instanceVarDataCurrent))->des_f_opacite = 0.0;
            v17 = (tAI*)v103->o_sfx_flash;
            if (v17 == WOR_gst_Universe.univAIInstance)
            {
                v16 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v19 = (tGameObjectMain*)v103->o_sfx_flash;
                if (v17)
                {
                    if ((unsigned int)v17 <= 2)
                        v19 = AI_MainActor[(uint32_t)v17 - 1];
                    v18 = v19;
                }
                else
                {
                    v18 = 0;
                }
                v16 = v18->extended->AIInstance;
            }
            ((tScriptStruct_SFXFlashTexture*)(v16->instanceVarDataCurrent))->des_f_temps_deb = -1.0;
            v13 = (tAI*)v103->o_sfx_flash;
            if (v13 == WOR_gst_Universe.univAIInstance)
            {
                v12 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v15 = (tGameObjectMain*)v103->o_sfx_flash;
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
            ((tScriptStruct_SFXFlashTexture*)(v12->instanceVarDataCurrent))->des_f_temps_fin = 0.1f;
            v9 = (tAI*)v103->o_sfx_flash;
            if (v9 == WOR_gst_Universe.univAIInstance)
            {
                v8 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v11 = (tGameObjectMain*)v103->o_sfx_flash;
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
            ((tScriptStruct_SFXFlashTexture*)(v8->instanceVarDataCurrent))->des_i_type_effet = 0;
        }
    }
    AI_EvalFunc_GFXSeti_C_CURRENT(v103->GFX_Halo, 21101, v103->c_halo);
    return 1000;
}

int Bouee_Anti_Aircraft_BAA_mode_dynamique()
{
    double v0; // st7
    double v1; // st7
    BOOL v2; // eax
    BOOL v3; // eax
    BOOL v4; // eax
    BOOL v5; // eax
    BOOL v6; // eax
    double v7; // st7
    int result; // eax
    float X; // [esp+0h] [ebp-8C0h]
    float Xa; // [esp+0h] [ebp-8C0h]
    float Xb; // [esp+0h] [ebp-8C0h]
    float Xc; // [esp+0h] [ebp-8C0h]
    float Xd; // [esp+0h] [ebp-8C0h]
    float Xe; // [esp+0h] [ebp-8C0h]
    float X_4; // [esp+4h] [ebp-8BCh]
    float v16; // [esp+8h] [ebp-8B8h]
    float v17; // [esp+Ch] [ebp-8B4h]
    float v18; // [esp+10h] [ebp-8B0h]
    float v19; // [esp+14h] [ebp-8ACh]
    float v20; // [esp+18h] [ebp-8A8h]
    float v21; // [esp+1Ch] [ebp-8A4h]
    float v22; // [esp+20h] [ebp-8A0h]
    float v23; // [esp+24h] [ebp-89Ch]
    tScriptStruct_BoueeAntiAircraft* v24; // [esp+28h] [ebp-898h]
    Vec3* v25; // [esp+48h] [ebp-878h]
    float v26; // [esp+54h] [ebp-86Ch]
    float v27; // [esp+CCh] [ebp-7F4h]
    Vec3* v28; // [esp+F4h] [ebp-7CCh]
    Vec3* v29; // [esp+F8h] [ebp-7C8h]
    Vec3* v30; // [esp+FCh] [ebp-7C4h]
    float v31; // [esp+104h] [ebp-7BCh]
    float v32; // [esp+124h] [ebp-79Ch]
    float v33; // [esp+138h] [ebp-788h]
    Vec3* v34; // [esp+158h] [ebp-768h]
    Vec3* v35; // [esp+15Ch] [ebp-764h]
    Vec3* v36; // [esp+160h] [ebp-760h]
    float v37; // [esp+168h] [ebp-758h]
    float v38; // [esp+17Ch] [ebp-744h]
    float v39; // [esp+188h] [ebp-738h]
    tGameObjectMain* v40; // [esp+1A0h] [ebp-720h]
    float v41; // [esp+1A4h] [ebp-71Ch]
    float v42; // [esp+1A8h] [ebp-718h]
    tGameObjectMain* v43; // [esp+1B0h] [ebp-710h]
    Vec3* v44; // [esp+1B4h] [ebp-70Ch]
    float v45; // [esp+1C0h] [ebp-700h]
    tGameObjectMain* v46; // [esp+1C8h] [ebp-6F8h]
    tGameObjectMain* v47; // [esp+1E0h] [ebp-6E0h]
    tGameObjectMain* v48; // [esp+1F0h] [ebp-6D0h]
    tGameObjectMain* v49; // [esp+208h] [ebp-6B8h]
    tGameObjectMain* v50; // [esp+218h] [ebp-6A8h]
    Vec3* v51; // [esp+234h] [ebp-68Ch]
    tGameObjectMain* v52; // [esp+250h] [ebp-670h]
    tGameObjectMain* v53; // [esp+258h] [ebp-668h]
    tGameObjectMain* v54; // [esp+268h] [ebp-658h]
    tGameObjectMain* v55; // [esp+278h] [ebp-648h]
    float v56; // [esp+284h] [ebp-63Ch]
    Vec3 v57; // [esp+290h] [ebp-630h] BYREF
    Vec3 v58; // [esp+29Ch] [ebp-624h] BYREF
    Vec3 v59; // [esp+2A8h] [ebp-618h] BYREF
    Vec3 v60; // [esp+2B4h] [ebp-60Ch] BYREF
    Vec3 v61; // [esp+2C0h] [ebp-600h] BYREF
    int v62; // [esp+2CCh] [ebp-5F4h]
    int v63; // [esp+2D0h] [ebp-5F0h]
    float v64; // [esp+2D4h] [ebp-5ECh]
    float v65; // [esp+2D8h] [ebp-5E8h]
    float v66; // [esp+2DCh] [ebp-5E4h]
    float v67; // [esp+2E0h] [ebp-5E0h]
    int v68; // [esp+2ECh] [ebp-5D4h]
    float v69; // [esp+2F0h] [ebp-5D0h]
    BOOL v70; // [esp+2F8h] [ebp-5C8h]
    BOOL v72; // [esp+300h] [ebp-5C0h]
    Vec3 v73; // [esp+304h] [ebp-5BCh] BYREF
    Vec3 v74; // [esp+310h] [ebp-5B0h] BYREF
    Vec3 v75; // [esp+31Ch] [ebp-5A4h] BYREF
    float v76; // [esp+328h] [ebp-598h]
    float v77; // [esp+32Ch] [ebp-594h]
    float v78; // [esp+330h] [ebp-590h]
    Vec3 v79; // [esp+334h] [ebp-58Ch] BYREF
    Vec3 v80; // [esp+340h] [ebp-580h] BYREF
    Vec3 v81; // [esp+34Ch] [ebp-574h] BYREF
    Vec3 v82; // [esp+358h] [ebp-568h] BYREF
    Vec3 v83; // [esp+364h] [ebp-55Ch] BYREF
    Vec3 v84; // [esp+370h] [ebp-550h] BYREF
    int v85; // [esp+37Ch] [ebp-544h]
    int v86; // [esp+380h] [ebp-540h]
    float v87; // [esp+384h] [ebp-53Ch]
    Vec3 v88; // [esp+388h] [ebp-538h] BYREF
    Vec3 v89; // [esp+394h] [ebp-52Ch] BYREF
    float v90; // [esp+3A0h] [ebp-520h]
    int v91; // [esp+3A4h] [ebp-51Ch]
    float v92; // [esp+3A8h] [ebp-518h]
    int v94; // [esp+3B0h] [ebp-510h]
    float v95; // [esp+3B4h] [ebp-50Ch]
    float v96; // [esp+3B8h] [ebp-508h]
    float v97; // [esp+3BCh] [ebp-504h]
    float v98; // [esp+3C0h] [ebp-500h]
    Vec3 v99; // [esp+3C4h] [ebp-4FCh] BYREF
    Vec3 v100; // [esp+3D0h] [ebp-4F0h] BYREF
    Vec3 v101; // [esp+3DCh] [ebp-4E4h] BYREF
    Vec3 v102; // [esp+3E8h] [ebp-4D8h]
    int v103; // [esp+3F4h] [ebp-4CCh]
    Vec3 v104; // [esp+3FCh] [ebp-4C4h] BYREF
    Vec3 v105; // [esp+408h] [ebp-4B8h] BYREF
    Vec3 v106; // [esp+414h] [ebp-4ACh] BYREF
    Vec3 v107; // [esp+420h] [ebp-4A0h] BYREF
    float v108; // [esp+42Ch] [ebp-494h]
    float v109; // [esp+430h] [ebp-490h]
    float v110; // [esp+434h] [ebp-48Ch]
    float v111; // [esp+438h] [ebp-488h]
    BOOL v112; // [esp+43Ch] [ebp-484h]
    Vec3 v113; // [esp+440h] [ebp-480h] BYREF
    Vec3 v114; // [esp+44Ch] [ebp-474h] BYREF
    Vec3 v115; // [esp+458h] [ebp-468h] BYREF
    Vec3 v116; // [esp+464h] [ebp-45Ch] BYREF
    Vec3 v117; // [esp+470h] [ebp-450h] BYREF
    float v118; // [esp+488h] [ebp-438h]
    float v119; // [esp+48Ch] [ebp-434h]
    float v120; // [esp+490h] [ebp-430h]
    float v121; // [esp+494h] [ebp-42Ch]
    float v123; // [esp+49Ch] [ebp-424h]
    float v124; // [esp+4A0h] [ebp-420h]
    float v125; // [esp+4ACh] [ebp-414h]
    Vec3 v126; // [esp+4B0h] [ebp-410h] BYREF
    Vec3 v127; // [esp+4BCh] [ebp-404h] BYREF
    Vec3 v128; // [esp+4C8h] [ebp-3F8h] BYREF
    Vec3 v129; // [esp+4D4h] [ebp-3ECh] BYREF
    Vec3 v130; // [esp+4E0h] [ebp-3E0h] BYREF
    Vec3 v131; // [esp+4ECh] [ebp-3D4h] BYREF
    Vec3 v132; // [esp+504h] [ebp-3BCh] BYREF
    Vec3 v133; // [esp+510h] [ebp-3B0h] BYREF
    tGameObjectMain* v135; // [esp+520h] [ebp-3A0h]
    int v136; // [esp+524h] [ebp-39Ch]
    float v137; // [esp+52Ch] [ebp-394h]
    Vec3 v138; // [esp+534h] [ebp-38Ch]
    Vec3 v139; // [esp+540h] [ebp-380h] BYREF
    Vec3 a2; // [esp+54Ch] [ebp-374h] BYREF
    BOOL v141; // [esp+558h] [ebp-368h]
    float v142; // [esp+55Ch] [ebp-364h]
    float v143; // [esp+560h] [ebp-360h]
    Vec3 v144; // [esp+568h] [ebp-358h] BYREF
    Vec3 v145; // [esp+574h] [ebp-34Ch] BYREF
    Vec3 v146; // [esp+58Ch] [ebp-334h] BYREF
    float v147; // [esp+598h] [ebp-328h]
    float v148; // [esp+59Ch] [ebp-324h]
    float v149; // [esp+5A0h] [ebp-320h]
    BOOL v150; // [esp+5A8h] [ebp-318h]
    float v151; // [esp+5ACh] [ebp-314h]
    float v154; // [esp+5B8h] [ebp-308h]
    float v155; // [esp+5BCh] [ebp-304h]
    float v156; // [esp+5C0h] [ebp-300h]
    float v157; // [esp+5C4h] [ebp-2FCh]
    float v158; // [esp+5C8h] [ebp-2F8h]
    float v159; // [esp+5CCh] [ebp-2F4h]
    Vec3 v160; // [esp+5D0h] [ebp-2F0h] BYREF
    float v161; // [esp+5DCh] [ebp-2E4h]
    float v162; // [esp+5E0h] [ebp-2E0h]
    float v163; // [esp+5E4h] [ebp-2DCh]
    Vec3 v164; // [esp+5E8h] [ebp-2D8h] BYREF
    int v165; // [esp+5F4h] [ebp-2CCh]
    int v166; // [esp+5F8h] [ebp-2C8h]
    int v167; // [esp+5FCh] [ebp-2C4h]
    Vec3 v168; // [esp+600h] [ebp-2C0h]
    Vec3 v169; // [esp+618h] [ebp-2A8h]
    Vec3 v170; // [esp+624h] [ebp-29Ch] BYREF
    Vec3 v171; // [esp+630h] [ebp-290h] BYREF
    float v172; // [esp+63Ch] [ebp-284h]
    float v173; // [esp+640h] [ebp-280h]
    BOOL v174; // [esp+644h] [ebp-27Ch]
    float v175; // [esp+648h] [ebp-278h]
    float v176; // [esp+64Ch] [ebp-274h]
    float v177; // [esp+650h] [ebp-270h]
    Vec3 v178; // [esp+654h] [ebp-26Ch] BYREF
    Vec3 v179; // [esp+660h] [ebp-260h] BYREF
    Vec3 v180; // [esp+66Ch] [ebp-254h] BYREF
    Vec3 a3; // [esp+678h] [ebp-248h] BYREF
    Vec3 v182; // [esp+684h] [ebp-23Ch] BYREF
    Vec3 v183; // [esp+690h] [ebp-230h] BYREF
    Vec3 v184; // [esp+6A8h] [ebp-218h] BYREF
    Vec3 v185; // [esp+6B4h] [ebp-20Ch] BYREF
    float v187; // [esp+6C4h] [ebp-1FCh]
    float v188; // [esp+6C8h] [ebp-1F8h]
    float v189; // [esp+6D0h] [ebp-1F0h]
    float v190; // [esp+6D4h] [ebp-1ECh]
    int v191; // [esp+6DCh] [ebp-1E4h]
    Vec3 v192; // [esp+6F8h] [ebp-1C8h] BYREF
    Vec3 v193; // [esp+704h] [ebp-1BCh] BYREF
    tGameObjectMain* v194; // [esp+710h] [ebp-1B0h]
    float v195; // [esp+714h] [ebp-1ACh]
    float v196; // [esp+71Ch] [ebp-1A4h]
    float v197; // [esp+720h] [ebp-1A0h]
    Vec3 v198; // [esp+724h] [ebp-19Ch] BYREF
    Vec3 v199; // [esp+730h] [ebp-190h] BYREF
    Vec3 a5; // [esp+73Ch] [ebp-184h] BYREF
    Vec3 v201; // [esp+748h] [ebp-178h] BYREF
    Vec3 v202; // [esp+754h] [ebp-16Ch] BYREF
    Vec3 v203; // [esp+760h] [ebp-160h] BYREF
    int v204; // [esp+778h] [ebp-148h]
    float v205; // [esp+77Ch] [ebp-144h]
    float v206; // [esp+780h] [ebp-140h]
    float v208; // [esp+788h] [ebp-138h]
    float v209; // [esp+78Ch] [ebp-134h]
    float v210; // [esp+790h] [ebp-130h]
    float v211; // [esp+794h] [ebp-12Ch]
    BOOL v214; // [esp+7ACh] [ebp-114h]
    float v216; // [esp+7B4h] [ebp-10Ch]
    int v217; // [esp+7BCh] [ebp-104h]
    float v218; // [esp+7C4h] [ebp-FCh]
    Vec3 v219; // [esp+7C8h] [ebp-F8h] BYREF
    Vec3 v220; // [esp+7D4h] [ebp-ECh] BYREF
    Vec3 v221; // [esp+7E0h] [ebp-E0h] BYREF
    Vec3 v222; // [esp+7ECh] [ebp-D4h] BYREF
    Vec3 v223; // [esp+7F8h] [ebp-C8h] BYREF
    tScriptStruct_BoueeAntiAircraft* v224; // [esp+804h] [ebp-BCh]
    float v225; // [esp+808h] [ebp-B8h]
    float v226; // [esp+80Ch] [ebp-B4h]
    float v227; // [esp+810h] [ebp-B0h]
    int v228; // [esp+814h] [ebp-ACh]
    float v229; // [esp+81Ch] [ebp-A4h]
    float v230; // [esp+820h] [ebp-A0h]
    Vec3 v231; // [esp+824h] [ebp-9Ch] BYREF
    Vec3 dest; // [esp+830h] [ebp-90h] BYREF
    Vec3 v233; // [esp+83Ch] [ebp-84h] BYREF
    Vec3 v234; // [esp+848h] [ebp-78h] BYREF
    Vec3 v235; // [esp+854h] [ebp-6Ch] BYREF
    Vec3 v236; // [esp+860h] [ebp-60h] BYREF
    int v237; // [esp+86Ch] [ebp-54h]
    BOOL v239; // [esp+874h] [ebp-4Ch]
    float v240; // [esp+878h] [ebp-48h]
    Vec3 v241; // [esp+87Ch] [ebp-44h] BYREF
    Vec3 v242; // [esp+888h] [ebp-38h] BYREF
    Vec3 v243; // [esp+894h] [ebp-2Ch] BYREF
    Vec3 v244; // [esp+8A0h] [ebp-20h] BYREF
    BOOL v246; // [esp+8B0h] [ebp-10h]
    int v247; // [esp+8B4h] [ebp-Ch]
    BOOL v248; // [esp+8B8h] [ebp-8h]
    BOOL v249; // [esp+8BCh] [ebp-4h]

    v105.x = 1.0;
    v105.y = 0.0;
    v105.z = 0.0;
    v128.x = 1.0;
    v128.y = 0.0;
    v128.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    v201.x = 1.0;
    v201.y = 0.0;
    v201.z = 0.0;
    v139.x = 1.0;
    v139.y = 0.0;
    v139.z = 0.0;
    v202.x = 1.0;
    v202.y = 0.0;
    v202.z = 0.0;
    v84.x = 1.0;
    v84.y = 0.0;
    v84.z = 0.0;
    v102.x = 1.0;
    v102.y = 0.0;
    v102.z = 0.0;
    v169.x = 1.0;
    v169.y = 0.0;
    v169.z = 0.0;
    v170.x = 1.0;
    v170.y = 0.0;
    v170.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v24 = (tScriptStruct_BoueeAntiAircraft*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v24 = 0;
    v224 = v24;
    v194 = 0;
    v135 = 0;
    v147 = 0.0;
    v136 = 0;
    v111 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fctvar_Bouee_Anti_Aircraft_BAA_mode_dynamique_camera = AI_ul_RealResolveOneRef(42, 0x7100108Bu);
        fct_Bouee_Anti_Aircraft_BAA_mode_dynamique_Bouee_Anti_Aircraft_BAA_exec_GFX_trace = (int)AI_ul_RealResolveOneRefFunction(0x720046AFu);
        fct_Bouee_Anti_Aircraft_BAA_mode_dynamique_Bouee_Anti_Aircraft_BAA_exec_interception = (int)AI_ul_RealResolveOneRefFunction(0x72004B26u);
    }
    AI_EvalFunc_COLPosGet_C(AI_gpst_CurrentGameObject, 0, &v130);
    v201 = v130;
    if (v224->i_current_mode)
    {
        v224->i_current_mode = 0;
        AI_EvalFunc_COLPrioritySet_C(AI_gpst_CurrentGameObject, 100);
        AI_EvalFunc_COLLODSet_C(AI_gpst_CurrentGameObject, 3, 3);
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 1, 1);
        AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0xFFu, 0);
        v224->f_water_Z = -1.0e14f;
        v0 = AI_EvalFunc_COLSizeGet_C(AI_gpst_CurrentGameObject, 0);
        v224->f_zdm_rayon = (float)v0;
        v224->f_air_friction = 1.2f;
        v224->v_grav_speed.x = 0.0;
        v224->v_grav_speed.y = 0.0;
        v224->v_grav_speed.z = 0.0;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v100);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v89);
        v56 = v224->des_f_dist_centre_grav;
        v171.x = v89.x * v56;
        v171.y = v89.y * v56;
        v171.z = v89.z * v56;
        MATH_SubstractVec(&dest, &v100, &v171);
        v224->v_grav_pos = dest;
        v224->f_rotation_speed = 0.0;
    }
    v135 = AI_EvalFunc_VIEWGetObject_C(0);
    v55 = v135;
    if (v135)
    {
        if ((unsigned int)v135 <= 2)
            v55 = AI_MainActor[(uint32_t)v135 - 1];
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v55);
    }
    else
    {
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v196 = (float)v1;
    if (v196 <= 40000.0)
    {
        if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 64))
        {
            AI_EvalFunc_COLNormalGet_C(AI_gpst_CurrentGameObject, 64, &v180);
            v169 = v180;
            v194 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 64);
            v224->m_msg.msgID = 37;
            v224->m_msg.gao = AI_gpst_CurrentGameObject;
            v224->m_msg.gao2 = 0;
            v224->m_msg.Int1 = 262150;
            v224->m_msg.Int2 = 0;
            v54 = v194;
            if (v194)
            {
                if ((unsigned int)v194 <= 2)
                    v54 = AI_MainActor[(uint32_t)v194 - 1];
                AI_EvalFunc_OBJGetPos_C(v54, &v81);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v81);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v146);
            MATH_SubstractVec(&v203, &v81, &v146);
            v224->m_msg.v1 = v203;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v224->m_msg.v2);
            v224->m_msg.f2 = 0.0;
            v224->m_msg.f1 = 1000.0;
            v53 = v194;
            if (v194)
            {
                if ((unsigned int)v194 <= 2)
                    v53 = AI_MainActor[(uint32_t)v194 - 1];
                AI_EvalFunc_MSGSend_C(v53, &v224->m_msg);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &v224->m_msg);
            }
            v52 = v194;
            if (v194)
            {
                if ((unsigned int)v194 <= 2)
                    v52 = AI_MainActor[(uint32_t)v194 - 1];
                v2 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v52, 4);
            }
            else
            {
                v2 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 4);
            }
            v248 = v2;
            if (!v2)
            {
                v224->f_time_ground_col = TIM_gf_MainClock - TIM_gf_LockTime;
                if (v169.z <= 0.0)
                {
                    v138.x = -v169.x;
                    v138.y = -v169.y;
                    v138.z = -v169.z;
                    v51 = &v224->v_normale_sol;
                    v224->v_normale_sol.x = -v169.x;
                    v51->y = v138.y;
                    v51->z = v138.z;
                }
                else
                {
                    v224->v_normale_sol = v169;
                }
            }
        }
        v141 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
        v204 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 50499588, 0);
        AI_EvalFunc_DYNReboundSet_C(AI_gpst_CurrentGameObject, 0.89999998f);
        AI_EvalFunc_DYNApplyRectFactorSet_C(AI_gpst_CurrentGameObject, 0.1f);
        v99.x = 0.0;
        v99.y = 0.0;
        v99.z = v224->des_f_base_gravity;
        AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v99);
        AI_EvalFunc_DYNSpeedLimitSet_C(AI_gpst_CurrentGameObject, 40.0);
        v233.x = 0.0;
        v233.y = 0.0;
        v233.z = 0.0;
        AI_EvalFunc_DYNStreamSet_C(AI_gpst_CurrentGameObject, &v233);
        v160.x = 0.0;
        v160.y = 0.0;
        v160.z = 0.0;
        AI_EvalFunc_DYNTractionSet_C_CURRENT(&v160);
        v228 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x800000, 0);
        v224->v_water_force.x = 0.0;
        v224->v_water_force.y = 0.0;
        v224->v_water_force.z = 0.0;
        v136 = AI_EvalFunc_COLGMatReportGet_C(AI_gpst_CurrentGameObject, 16);
        if (v136 == -1)
        {
            if (v224->f_water_Z > v224->f_zdm_rayon + v201.z)
            {
                v224->i_flag_in_water = 1;
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v114);
                AI_EvalFunc_WTR_GetDif_C(&v114, &v242);
                v102 = v242;
                if (v201.z >= (double)v242.z)
                {
                    v224->i_flag_in_water = 0;
                    v224->f_water_Z = -1.0e14f;
                }
                else
                {
                    v224->f_water_Z = v102.z;
                    v224->f_last_valid_water_Z = v224->f_water_Z;
                    v224->i_flag_in_water = 1;
                }
            }
        }
        else
        {
            AI_EvalFunc_COLPointGet_C(AI_gpst_CurrentGameObject, v136 + 0x80000000, &v235);
            v102 = v235;
            v224->f_water_Z = v235.z;
            v224->f_last_valid_water_Z = v224->f_water_Z;
            v224->i_flag_in_water = 1;
        }
        AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v198);
        v170 = v198;
        if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x80))
        {
            v194 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 0x80);
            v224->m_msg.msgID = 37;
            v224->m_msg.gao = AI_gpst_CurrentGameObject;
            v224->m_msg.gao2 = 0;
            v224->m_msg.Int1 = 262150;
            v224->m_msg.Int2 = 0;
            v50 = v194;
            if (v194)
            {
                if ((unsigned int)v194 <= 2)
                    v50 = AI_MainActor[(uint32_t)v194 - 1];
                AI_EvalFunc_OBJGetPos_C(v50, &v234);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v234);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v178);
            MATH_SubstractVec(&v144, &v234, &v178);
            v224->m_msg.v1 = v144;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v224->m_msg.v2);
            v224->m_msg.f2 = 0.0;
            v224->m_msg.f1 = 1000.0;
            v49 = v194;
            if (v194)
            {
                if ((unsigned int)v194 <= 2)
                    v49 = AI_MainActor[(uint32_t)v194 - 1];
                AI_EvalFunc_MSGSend_C(v49, &v224->m_msg);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &v224->m_msg);
            }
            AI_EvalFunc_COLPointGet_C(AI_gpst_CurrentGameObject, 0x80, &v117);
            v84 = v117;
            if (!AI2C_VecEqNull(&v84))
            {
                v48 = v194;
                if (v194)
                {
                    if ((unsigned int)v194 <= 2)
                        v48 = AI_MainActor[(uint32_t)v194 - 1];
                    v3 = AI_EvalFunc_AIIsModel_C(v48, (tAIModel*)fctvar_Bouee_Anti_Aircraft_BAA_mode_dynamique_camera);
                }
                else
                {
                    v3 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_Bouee_Anti_Aircraft_BAA_mode_dynamique_camera);
                }
                v249 = v3;
                if (!v3)
                {
                    v84.x = v84.x - v201.x;
                    v84.y = v84.y - v201.y;
                    v84.z = v84.z - v201.z;
                    if (AI2C_VecEqNull(&v170))
                    {
                        v47 = v194;
                        if (v194)
                        {
                            if ((unsigned int)v194 <= 2)
                                v47 = AI_MainActor[(uint32_t)v194 - 1];
                            AI_EvalFunc_DYNSpeedGetVector_C(v47, &v104);
                        }
                        else
                        {
                            AI_EvalFunc_DYNSpeedGetVector_C(0, &v104);
                        }
                        v170 = v104;
                    }
                    if (!AI2C_VecEqNull(&v170))
                    {
                        v46 = v194;
                        if (v194)
                        {
                            if ((unsigned int)v194 <= 2)
                                v46 = AI_MainActor[(uint32_t)v194 - 1];
                            v4 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v46, 0x204);
                        }
                        else
                        {
                            v4 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x204);
                        }
                        v246 = v4;
                        if (v4)
                        {
                            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v116);
                            v172 = v84.z * v116.z + v84.y * v116.y + v116.x * v84.x;
                            v45 = -v172;
                            if (v45 <= 3.0)
                            {
                                if (v45 >= -3.0)
                                    v121 = -v172;
                                else
                                    v121 = -3.0;
                            }
                            else
                            {
                                v121 = 3.0;
                            }
                            v147 = v121;
                        }
                        else
                        {
                            v147 = 0.0;
                        }
                        v202 = v170;
                        v150 = AI_EvalFunc_TIMEElapsed_C(v224->f_time_ground_col, 0.1f);
                        if (v150 || !(v70 = AI_EvalFunc_TIMEElapsed_C(v224->f_time_paf, 0.1f)))
                        {
                            v209 = v202.x * v147;
                            v210 = v202.y * v147;
                            v211 = v202.z * v147;
                            v44 = &v224->v_grav_speed;
                            v224->v_grav_speed.x = v209 + v224->v_grav_speed.x;
                            v44->y = v210 + v44->y;
                            v44->z = v211 + v44->z;
                        }
                        if (v147 > 0.0)
                        {
                            v43 = v194;
                            if (v194)
                            {
                                if ((unsigned int)v194 <= 2)
                                    v43 = AI_MainActor[(uint32_t)v194 - 1];
                                v5 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v43, 516);
                            }
                            else
                            {
                                v5 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 516);
                            }
                            v72 = v5;
                            if (v5)
                            {
                                if (!AI_EvalFunc_TIMEElapsed_C(v224->f_time_ground_col, 0.1f))
                                {
                                    v109 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
                                    v42 = v109 - v224->f_last_speed;
                                    if (v42 <= 20.0)
                                    {
                                        if (v42 >= 0.0)
                                            v189 = v109 - v224->f_last_speed;
                                        else
                                            v189 = 0.0;
                                    }
                                    else
                                    {
                                        v189 = 20.0;
                                    }
                                    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v57);
                                    v41 = v147 * v189;
                                    v118 = v57.x * v41;
                                    v119 = v57.y * v41;
                                    v120 = v57.z * v41;
                                    v202.x = v202.x - v118;
                                    v202.y = v202.y - v119;
                                    v202.z = v202.z - v120;
                                }
                            }
                        }
                        v68 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v202);
                        v40 = v194;
                        if (v194)
                        {
                            if ((unsigned int)v194 <= 2)
                                v40 = AI_MainActor[(uint32_t)v194 - 1];
                            v6 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v40, 0x204);
                        }
                        else
                        {
                            v6 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x204);
                        }
                        v239 = v6;
                        if (v6)
                        {
                            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v75);
                            AI_EvalFunc_MATHVecCrossProduct_C(&v170, &v75, &v59);
                            v128 = v59;
                            if (!AI2C_VecEqNull(&v128))
                            {
                                if (AI_EvalFunc_TIMEElapsed_C(v224->f_time_ground_col, 0.1f))
                                {
                                    v205 = v84.z * v128.z + v84.y * v128.y + v128.x * v84.x;
                                    if (v205 <= 3.0)
                                    {
                                        if (v205 >= -3.0)
                                            v149 = v205;
                                        else
                                            v149 = -3.0;
                                    }
                                    else
                                    {
                                        v149 = 3.0;
                                    }
                                    v224->f_rotation_speed = v149 * 1.570796f;
                                }
                                else
                                {
                                    v197 = v84.z * v128.z + v84.y * v128.y + v128.x * v84.x;
                                    if (v197 <= 3.0)
                                    {
                                        if (v197 >= -3.0)
                                            v208 = v197;
                                        else
                                            v208 = -3.0;
                                    }
                                    else
                                    {
                                        v208 = 3.0;
                                    }
                                    v224->f_rotation_speed = v208 * 0.392699f;
                                }
                                v218 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
                                v39 = (v218 - v224->f_last_speed) / 100.0f;
                                if (v39 <= 1.0)
                                {
                                    if (v39 >= 0.0)
                                        v90 = (v218 - v224->f_last_speed) / 100.0f;
                                    else
                                        v90 = 0.0;
                                }
                                else
                                {
                                    v90 = 1.0;
                                }
                                v224->f_rotation_speed = v90 * v224->f_rotation_speed;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            v195 = TIM_gf_dt;
            v224->f_rotation_speed = v224->f_rotation_speed - v224->f_rotation_speed * 0.89999998f * TIM_gf_dt;
            v173 = TIM_gf_dt;
            X_4 = v224->f_rotation_speed * TIM_gf_dt;
            AI_EvalFunc_OBJRotateLocalZ_c(AI_gpst_CurrentGameObject, X_4);
        }
        if (v224->i_flag_in_water)
        {
            v224->f_air_friction = v224->des_f_air_friction;
            v224->v_last_collide_pos.x = 0.0;
            v224->v_last_collide_pos.y = 0.0;
            v224->v_last_collide_pos.z = 0.0;
            AI_EvalFunc_MATHVecGlobalToLocal_C(AI_gpst_CurrentGameObject, &v224->des_v_water_friction, &v184);
            v105 = v184;
            v23 = (float)fabs(v184.x);
            v105.x = v23;
            v22 = (float)fabs(v105.y);
            v105.y = v22;
            v21 = (float)fabs(v105.z);
            v105.z = v21;
            v247 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v105);
            v38 = v224->f_water_Z - v201.z;
            if (v38 < 0.0)
                v20 = 0.0;
            else
                v20 = v224->f_water_Z - v201.z;
            v95 = v20;
            v224->v_water_force.x = 0.0;
            v224->v_water_force.y = 0.0;
            v224->v_water_force.z = v224->des_f_archimede * v95;
            if (AI_EvalFunc_TIMEElapsed_C(v224->f_time_ground_col, 0.1f))
            {
                AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v74);
                v170 = v74;
                v139 = v224->v_grav_pos;
                v37 = v224->des_f_water_friction;
                v96 = v37 * v224->v_grav_speed.x;
                v97 = v224->v_grav_speed.y * v37;
                v98 = v224->v_grav_speed.z * v37;
                v143 = TIM_gf_dt;
                v157 = v96 * TIM_gf_dt;
                v158 = v97 * TIM_gf_dt;
                v159 = v98 * TIM_gf_dt;
                v36 = &v224->v_grav_pos;
                v224->v_grav_pos.x = v157 + v224->v_grav_pos.x;
                v36->y = v158 + v36->y;
                v36->z = v159 + v36->z;
                v62 = 0;
                v63 = 0;
                v64 = v224->des_f_water_gravity;
                v69 = TIM_gf_dt;
                v161 = 0.0f * TIM_gf_dt;
                v162 = v161;
                v163 = v64 * TIM_gf_dt;
                v35 = &v224->v_grav_pos;
                v224->v_grav_pos.x = v161 + v224->v_grav_pos.x;
                v35->y = v162 + v35->y;
                v35->z = v163 + v35->z;
                MATH_SubstractVec(&v224->v_grav_pos, &v224->v_grav_pos, &v201);
                AI_EvalFunc_MATHVecSetNormVector_C(&v224->v_grav_pos, v224->des_f_dist_centre_grav);
                v34 = &v224->v_grav_pos;
                v224->v_grav_pos.x = v201.x + v224->v_grav_pos.x;
                v34->y = v201.y + v34->y;
                v34->z = v201.z + v34->z;
                MATH_SubstractVec(&v241, &v224->v_grav_pos, &v139);
                v240 = TIM_gf_dt;
                MATH_VecMultNormScalar(&v107, &v241, TIM_gf_dt);
                v224->v_grav_speed = v107;
                MATH_SubstractVec(&v231, &v201, &v224->v_grav_pos);
                AI_EvalFunc_MATHVecNormalize_C(&v231, &v115);
                a3 = v115;
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&a2);
                v108 = TIM_gf_dt;
                X = 8.0f * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, X, &a5);
                a3 = a5;
            }
            else
            {
                v148 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
                v33 = v148 / 10.0f;
                if (v33 > 6.0)
                    v19 = 6.0;
                else
                    v19 = v148 / 10.0f;
                v110 = v19;
                v111 = v19;
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v244);
                v206 = TIM_gf_dt;
                Xa = v111 * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&v244, &v224->v_normale_sol, Xa, &v145);
                a3 = v145;
                v139 = v224->v_grav_pos;
                v32 = v224->des_f_dist_centre_grav;
                v106.x = v145.x * v32;
                v106.y = v145.y * v32;
                v106.z = v145.z * v32;
                MATH_SubstractVec(&v243, &v201, &v106);
                v224->v_grav_pos = v243;
                MATH_SubstractVec(&v199, &v224->v_grav_pos, &v139);
                v151 = TIM_gf_dt;
                MATH_VecMultNormScalar(&v113, &v199, TIM_gf_dt);
                v224->v_grav_speed = v113;
            }
            v224->f_time_air = TIM_gf_MainClock - TIM_gf_LockTime;
        }
        else
        {
            v214 = AI_EvalFunc_TIMEElapsed_C(v224->f_time_ground_col, 0.1f);
            if (!v214 && (v174 = AI_EvalFunc_TIMEElapsed_C(v224->f_time_paf, 0.30000001f)))
            {
                if (!AI_EvalFunc_TIMEElapsed_C(v224->f_time_air, 0.5))
                    AI_EvalFunc_DYNReboundSet_C(AI_gpst_CurrentGameObject, 0.0);
                AI_EvalFunc_MATHVecGlobalToLocal_C(AI_gpst_CurrentGameObject, &v224->des_v_ground_friction, &v80);
                v105 = v80;
                v18 = (float)fabs(v80.x);
                v105.x = v18;
                v17 = (float)fabs(v105.y);
                v105.y = v17;
                v16 = (float)fabs(v105.z);
                v105.z = v16;
                v94 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v105);
                v230 = TIM_gf_MainClock - TIM_gf_LockTime;
                v224->f_time_air = v230;
                v190 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
                v27 = v190 / 10.0f;
                if (v27 <= 6.0)
                {
                    if (v27 >= 2.0)
                        v123 = v190 / 10.0f;
                    else
                        v123 = 2.0;
                }
                else
                {
                    v123 = 6.0;
                }
                v111 = v123;
                v168.x = 0.0;
                v168.y = 0.0;
                v168.z = 1.0;
                v216 = 1.0f * v224->v_normale_sol.z + 0.0f * v224->v_normale_sol.y + 0.0f * v224->v_normale_sol.x;
                if (v216 <= 0.99619502)
                {
                    v165 = 0;
                    v166 = 0;
                    v167 = 0;
                    a3.x = 0.0;
                    a3.y = 0.0;
                    a3.z = 0.0;
                }
                else
                {
                    AI_EvalFunc_COLPointGet_C(AI_gpst_CurrentGameObject, 64, &v58);
                    a3 = v58;
                    if (AI2C_VecEqNull(&a3))
                        a3 = v224->v_last_collide_pos;
                    else
                        v224->v_last_collide_pos = a3;
                }
                if (AI2C_VecEqNull(&a3))
                {
                    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v73);
                    v137 = TIM_gf_dt;
                    Xe = v111 * TIM_gf_dt;
                    AI_EvalFunc_MATHVecBlendRotate_C(&v73, &v224->v_normale_sol, Xe, &v223);
                    a3 = v223;
                }
                else
                {
                    MATH_SubstractVec(&v60, &v201, &a3);
                    a3 = v60;
                    MATH_NormalizeEqualVector(&a3);
                    v139.x = a3.x;
                    v139.y = a3.y;
                    v139.z = 0.0;
                    v124 = MATH_VecLen(&v139);
                    if (v124 <= 0.1)
                    {
                        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v133);
                        v188 = TIM_gf_dt;
                        Xd = v111 * TIM_gf_dt;
                        AI_EvalFunc_MATHVecBlendRotate_C(&v133, &v224->v_normale_sol, Xd, &v221);
                        a3 = v221;
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v164);
                        v92 = TIM_gf_dt;
                        Xc = 8.0f * TIM_gf_dt;
                        AI_EvalFunc_MATHVecBlendRotate_C(&v164, &a3, Xc, &v127);
                        a3 = v127;
                        v139.x = v139.x * 5.0f;
                        v139.y = v139.y * 5.0f;
                        v139.z = v139.z * 5.0f;
                        AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v182);
                        MATH_VecAddVec(&v219, &v182, &v139);
                        v91 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v219);
                    }
                }
                v139 = v224->v_grav_pos;
                v26 = v224->des_f_dist_centre_grav;
                v82.x = a3.x * v26;
                v82.y = a3.y * v26;
                v82.z = a3.z * v26;
                MATH_SubstractVec(&v126, &v201, &v82);
                v224->v_grav_pos = v126;
                AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v179);
                v226 = TIM_gf_dt;
                v76 = v179.x * TIM_gf_dt;
                v77 = v179.y * TIM_gf_dt;
                v78 = v179.z * TIM_gf_dt;
                v25 = &v224->v_grav_pos;
                v224->v_grav_pos.x = v224->v_grav_pos.x - v76;
                v25->y = v25->y - v77;
                v25->z = v25->z - v78;
                MATH_SubstractVec(&v132, &v224->v_grav_pos, &v139);
                v187 = TIM_gf_dt;
                MATH_VecMultNormScalar(&v220, &v132, TIM_gf_dt);
                v224->v_grav_speed = v220;
            }
            else
            {
                v224->v_last_collide_pos.x = 0.0;
                v224->v_last_collide_pos.y = 0.0;
                v224->v_last_collide_pos.z = 0.0;
                v61.x = 0.0;
                v61.y = 0.0;
                v61.z = 0.0;
                v217 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v61);
                v139 = v224->v_grav_pos;
                v31 = v224->f_air_friction;
                v154 = v31 * v224->v_grav_speed.x;
                v155 = v224->v_grav_speed.y * v31;
                v156 = v224->v_grav_speed.z * v31;
                v125 = TIM_gf_dt;
                v65 = v154 * TIM_gf_dt;
                v66 = v155 * TIM_gf_dt;
                v67 = v156 * TIM_gf_dt;
                v30 = &v224->v_grav_pos;
                v224->v_grav_pos.x = v65 + v224->v_grav_pos.x;
                v30->y = v66 + v30->y;
                v30->z = v67 + v30->z;
                v85 = 0;
                v86 = 0;
                v87 = v224->des_f_air_gravity;
                v227 = TIM_gf_dt;
                v175 = 0.0f * TIM_gf_dt;
                v176 = v175;
                v177 = v87 * TIM_gf_dt;
                v29 = &v224->v_grav_pos;
                v224->v_grav_pos.x = v175 + v224->v_grav_pos.x;
                v29->y = v176 + v29->y;
                v29->z = v177 + v29->z;
                MATH_SubstractVec(&v224->v_grav_pos, &v224->v_grav_pos, &v201);
                AI_EvalFunc_MATHVecSetNormVector_C(&v224->v_grav_pos, v224->des_f_dist_centre_grav);
                v28 = &v224->v_grav_pos;
                v224->v_grav_pos.x = v201.x + v224->v_grav_pos.x;
                v28->y = v201.y + v28->y;
                v28->z = v201.z + v28->z;
                MATH_SubstractVec(&v88, &v224->v_grav_pos, &v139);
                v229 = TIM_gf_dt;
                MATH_VecMultNormScalar(&v185, &v88, TIM_gf_dt);
                v224->v_grav_speed = v185;
                MATH_SubstractVec(&v129, &v201, &v224->v_grav_pos);
                AI_EvalFunc_MATHVecNormalize_C(&v129, &v79);
                a3 = v79;
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v222);
                v225 = TIM_gf_dt;
                Xb = 8.0f * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&v222, &a3, Xb, &v183);
                a3 = v183;
            }
        }
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v83);
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v83, &a3);
        v224->v_last_pos = v201;
        v7 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
        v224->f_last_speed = (float)v7;
        if (v224->i_flag_in_water)
        {
            AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &v193);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v236);
            MATH_SubstractVec(&v101, &v193, &v236);
            v139.x = v101.x;
            v139.y = v101.y;
            v139.z = 0.0;
            v142 = AI_EvalFunc_DYNFrictionGet_C(AI_gpst_CurrentGameObject);
            v139.x = v139.x * v142;
            v139.y = v139.y * v142;
            v139.z = v139.z * v142;
            MATH_VecAddVec(&v192, &v224->v_water_force, &v139);
            AI_EvalFunc_DYNTractionSet_C_CURRENT(&v192);
        }
        else
        {
            AI_EvalFunc_DYNTractionSet_C_CURRENT(&v224->v_water_force);
        }
        v237 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Bouee_Anti_Aircraft_BAA_mode_dynamique_Bouee_Anti_Aircraft_BAA_exec_GFX_trace);
        if (v237 != 1000)
            return v237;
    }
    else
    {
        if (AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 4))
        {
            v131.x = 0.0;
            v131.y = 0.0;
            v131.z = 0.0;
            v191 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v131);
            v112 = AI_EvalFunc_DYNOff_C(AI_gpst_CurrentGameObject);
        }
        if (v224->GFX_Airtrace != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v224->GFX_Airtrace);
            v224->GFX_Airtrace = -1;
        }
    }
    v103 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Bouee_Anti_Aircraft_BAA_mode_dynamique_Bouee_Anti_Aircraft_BAA_exec_interception);
    if (v103 == 1000)
        result = 1000;
    else
        result = v103;
    return result;
}

int Bouee_Anti_Aircraft_BAA_exec_GFX_trace()
{
    float v1; // [esp+4h] [ebp-39Ch]
    float v2; // [esp+4h] [ebp-39Ch]
    float v3; // [esp+4h] [ebp-39Ch]
    float v4; // [esp+4h] [ebp-39Ch]
    float v5; // [esp+4h] [ebp-39Ch]
    float v6; // [esp+4h] [ebp-39Ch]
    float v7; // [esp+8h] [ebp-398h]
    tScriptStruct_BoueeAntiAircraft* v8; // [esp+Ch] [ebp-394h]
    float v9; // [esp+20h] [ebp-380h]
    float v10; // [esp+24h] [ebp-37Ch]
    float v11; // [esp+28h] [ebp-378h]
    float v12; // [esp+2Ch] [ebp-374h]
    float v13; // [esp+30h] [ebp-370h]
    float v14; // [esp+34h] [ebp-36Ch]
    float v15; // [esp+38h] [ebp-368h]
    float v16; // [esp+3Ch] [ebp-364h]
    float v17; // [esp+40h] [ebp-360h]
    float v18; // [esp+44h] [ebp-35Ch]
    float v19; // [esp+48h] [ebp-358h]
    float v20; // [esp+80h] [ebp-320h]
    float v21; // [esp+90h] [ebp-310h]
    tGameObjectMain* v22; // [esp+98h] [ebp-308h]
    Vec3 v23; // [esp+9Ch] [ebp-304h] BYREF
    Vec3 dest; // [esp+A8h] [ebp-2F8h] BYREF
    Vec3 v25; // [esp+B4h] [ebp-2ECh] BYREF
    float v26; // [esp+C0h] [ebp-2E0h]
    float v27; // [esp+C4h] [ebp-2DCh]
    float v28; // [esp+C8h] [ebp-2D8h]
    float v29; // [esp+CCh] [ebp-2D4h]
    float v30; // [esp+D0h] [ebp-2D0h]
    float v31; // [esp+D4h] [ebp-2CCh]
    float v32; // [esp+D8h] [ebp-2C8h]
    float v33; // [esp+DCh] [ebp-2C4h]
    int v34; // [esp+E0h] [ebp-2C0h]
    float v35; // [esp+E4h] [ebp-2BCh]
    Vec3 v36; // [esp+E8h] [ebp-2B8h] BYREF
    Vec3 v37; // [esp+F4h] [ebp-2ACh] BYREF
    float v38; // [esp+104h] [ebp-29Ch]
    float v39; // [esp+108h] [ebp-298h]
    float v40; // [esp+10Ch] [ebp-294h]
    Vec3 v41; // [esp+110h] [ebp-290h] BYREF
    Vec3 v42; // [esp+11Ch] [ebp-284h]
    Vec3 v43; // [esp+128h] [ebp-278h] BYREF
    Vec3 v44; // [esp+134h] [ebp-26Ch] BYREF
    float v45; // [esp+140h] [ebp-260h]
    Vec3 v46; // [esp+144h] [ebp-25Ch] BYREF
    Vec3 v47; // [esp+150h] [ebp-250h] BYREF
    Vec3 v48; // [esp+15Ch] [ebp-244h] BYREF
    Vec3 v49; // [esp+168h] [ebp-238h] BYREF
    Vec3 v50; // [esp+174h] [ebp-22Ch] BYREF
    Vec3 v51; // [esp+180h] [ebp-220h] BYREF
    float v52; // [esp+18Ch] [ebp-214h]
    float v53; // [esp+190h] [ebp-210h]
    Vec3 v54; // [esp+194h] [ebp-20Ch] BYREF
    Vec3 v55; // [esp+1A0h] [ebp-200h] BYREF
    float v56; // [esp+1B4h] [ebp-1ECh]
    int v57; // [esp+1B8h] [ebp-1E8h]
    float v59; // [esp+1C0h] [ebp-1E0h]
    int v60; // [esp+1C4h] [ebp-1DCh]
    float v61; // [esp+1C8h] [ebp-1D8h]
    Vec3 v62; // [esp+1CCh] [ebp-1D4h] BYREF
    float v63; // [esp+1D8h] [ebp-1C8h]
    int v64; // [esp+1DCh] [ebp-1C4h]
    float v65; // [esp+1E0h] [ebp-1C0h]
    Vec3 v66; // [esp+1E4h] [ebp-1BCh] BYREF
    Vec3 v67; // [esp+1F0h] [ebp-1B0h] BYREF
    Vec3 v68; // [esp+1FCh] [ebp-1A4h] BYREF
    Vec3 v69; // [esp+208h] [ebp-198h] BYREF
    Vec3 v70; // [esp+214h] [ebp-18Ch] BYREF
    Vec3 a1; // [esp+220h] [ebp-180h] BYREF
    Vec3 v72; // [esp+22Ch] [ebp-174h] BYREF
    float v73; // [esp+238h] [ebp-168h]
    Vec3 v74; // [esp+240h] [ebp-160h] BYREF
    float v75; // [esp+24Ch] [ebp-154h]
    int v76; // [esp+250h] [ebp-150h]
    float v77; // [esp+254h] [ebp-14Ch]
    Vec3 v78; // [esp+258h] [ebp-148h] BYREF
    Vec3 v79; // [esp+264h] [ebp-13Ch] BYREF
    float v80; // [esp+274h] [ebp-12Ch]
    float v81; // [esp+278h] [ebp-128h]
    int v82; // [esp+27Ch] [ebp-124h]
    Vec3 a2; // [esp+280h] [ebp-120h] BYREF
    float v84; // [esp+28Ch] [ebp-114h]
    float v85; // [esp+290h] [ebp-110h]
    Vec3 v87; // [esp+298h] [ebp-108h]
    Vec3 v88; // [esp+2A4h] [ebp-FCh] BYREF
    Vec3 a3; // [esp+2B0h] [ebp-F0h] BYREF
    Vec3 v90; // [esp+2BCh] [ebp-E4h] BYREF
    Vec3 v91; // [esp+2C8h] [ebp-D8h] BYREF
    float v92; // [esp+2D8h] [ebp-C8h]
    int v93; // [esp+2DCh] [ebp-C4h]
    Vec3 v94; // [esp+2E0h] [ebp-C0h] BYREF
    Vec3 v95; // [esp+2ECh] [ebp-B4h] BYREF
    Vec3 v96; // [esp+2F8h] [ebp-A8h] BYREF
    float v97; // [esp+304h] [ebp-9Ch]
    int v98; // [esp+308h] [ebp-98h]
    float v99; // [esp+30Ch] [ebp-94h]
    Vec3 v100; // [esp+310h] [ebp-90h] BYREF
    float v101; // [esp+31Ch] [ebp-84h]
    int v102; // [esp+320h] [ebp-80h]
    float v103; // [esp+324h] [ebp-7Ch]
    tScriptStruct_BoueeAntiAircraft* v104; // [esp+328h] [ebp-78h]
    float v105; // [esp+32Ch] [ebp-74h]
    float v106; // [esp+330h] [ebp-70h]
    float v107; // [esp+334h] [ebp-6Ch]
    float v108; // [esp+338h] [ebp-68h]
    Vec3 v109; // [esp+33Ch] [ebp-64h] BYREF
    float v110; // [esp+348h] [ebp-58h]
    int v111; // [esp+34Ch] [ebp-54h]
    float v112; // [esp+350h] [ebp-50h]
    Vec3 v113; // [esp+354h] [ebp-4Ch] BYREF
    float v114; // [esp+360h] [ebp-40h]
    float v115; // [esp+36Ch] [ebp-34h]
    int v116; // [esp+370h] [ebp-30h]
    float v117; // [esp+374h] [ebp-2Ch]
    Vec3 v118; // [esp+378h] [ebp-28h] BYREF
    Vec3 v119; // [esp+384h] [ebp-1Ch] BYREF
    Vec3 v120; // [esp+390h] [ebp-10h] BYREF

    v41.x = 1.0;
    v41.y = 0.0;
    v41.z = 0.0;
    v55.x = 1.0;
    v55.y = 0.0;
    v55.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    v91.x = 1.0;
    v91.y = 0.0;
    v91.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    v72.x = 1.0;
    v72.y = 0.0;
    v72.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v8 = (tScriptStruct_BoueeAntiAircraft*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v8 = 0;
    v104 = v8;
    v29 = 0.0;
    v92 = 0.0;
    v105 = 0.0;
    v38 = 0.0;
    v106 = 0.0;
    v114 = 0.0;
    v57 = 0;
    v73 = 0.0;
    v30 = 0.0;
    v107 = 0.0;
    v82 = 0;
    v40 = 0.0;
    v52 = 0.0;
    v85 = 0.0;
    v31 = 0.0;
    v93 = 0;
    v108 = 0.0;
    v45 = 0.0;
    v32 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Bouee_Anti_Aircraft_BAA_exec_GFX_trace_PJ_aircraft_material_gao = AI_ul_RealResolveOneRefObject(0x720006F2u);
    }
    if (v104->GFX_Airtrace == -1)
    {
        v104->GFX_Airtrace = AI_EvalFunc_GFXAdd_C_CURRENT(12);
        v22 = (tGameObjectMain*)Bouee_Anti_Aircraft_BAA_exec_GFX_trace_PJ_aircraft_material_gao;
        if (Bouee_Anti_Aircraft_BAA_exec_GFX_trace_PJ_aircraft_material_gao)
        {
            if ((unsigned int)Bouee_Anti_Aircraft_BAA_exec_GFX_trace_PJ_aircraft_material_gao <= 2)
                v22 = AI_MainActor[Bouee_Anti_Aircraft_BAA_exec_GFX_trace_PJ_aircraft_material_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v104->GFX_Airtrace, v22, 0);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v104->GFX_Airtrace, 0, 0);
        }
        GFXSetCurGlobal(v104->GFX_Airtrace);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12100, 6);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12101, 5);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12102, 15);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12103, 1);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12111, 8);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12000, -95.5);
        v100.x = 0.2f;
        v100.y = 0.2f;
        v100.z = 0.0f;
        AI_EvalFunc_GFXSetv_C_CURGLOB(12202, &v100);
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
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v104->GFX_Airtrace, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v104->GFX_Airtrace, 2, 1);
        v104->i_flag_trace_on = 0;
    }
    v84 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
    v21 = v84 / 30.0f;
    if (v21 <= 2.0)
    {
        if (v21 >= 0.1)
            v39 = v84 / 30.0f;
        else
            v39 = 0.1f;
    }
    else
    {
        v39 = 2.0;
    }
    AI_EvalFunc_GFXSetf_C_CURRENT(v104->GFX_Airtrace, 12004, v39);
    v82 = 0;
    if (v104->f_water_Z == -1.0e14)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v113);
        if (v104->f_last_valid_water_Z > v113.z - 5.0)
            v82 = 1;
        v32 = v104->f_last_valid_water_Z + 0.5f;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v48);
        v38 = v48.z - 5.0f - v32;
    }
    else
    {
        v32 = v104->f_water_Z + 0.5f;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v37);
        v38 = v37.z - 5.0f - v32;
        if (v38 < 0.5)
            v104->f_time_gfx_trace = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    v104->f_last_water_Z = v104->f_water_Z;
    if (v82 && !v104->i_flag_trace_on)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    v29 = 5.0;
    v30 = 4.0;
    v105 = 3.0;
    v52 = 0.25;
    v114 = 10.0;
    v31 = 10.0;
    v73 = 30.0;
    v45 = 30.0;
    AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&a2);
    a1.x = a2.x;
    a1.y = a2.y;
    a1.z = 0.0;
    v40 = MATH_VecLen(&a1);
    v20 = v40 / 40.0f;
    if (v20 > 1.0)
        v7 = 1.0;
    else
        v7 = v40 / 40.0f;
    v107 = v7;
    v80 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
    if (v80 == 0.0)
    {
        v106 = 0.0;
        v92 = 0.0;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v36);
        v72 = v36;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v66);
        v91 = v66;
        v87.x = 0.0;
        v87.y = 0.0;
        v87.z = 1.0;
        v55.x = 0.0;
        v55.y = 0.0;
        v55.z = 1.0;
    }
    else
    {
        v42.x = 0.0;
        v42.y = 0.0;
        v42.z = 1.0;
        v55.x = 0.0;
        v55.y = 0.0;
        v55.z = 1.0;
        AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v118);
        v81 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
        MATH_VecMultNormScalar(&v54, &v118, v81);
        v91 = v54;
        AI_EvalFunc_MATHVecCrossProduct_C(&v91, &v55, &v67);
        v72 = v67;
        v73 = v73 * 0.5f;
        v114 = v114 * 0.5f;
        v106 = 1.0;
        v45 = v45 * 0.5f;
        v31 = v31 * 0.5f;
        v92 = 1.0;
    }
    v19 = v107 * 4.0f;
    if (v19 <= 1.0)
    {
        if (v19 >= 0.0)
            v18 = v107 * 4.0f;
        else
            v18 = 0.0;
    }
    else
    {
        v18 = 1.0;
    }
    v106 = v106 * v18;
    v17 = v107 * 4.0f;
    if (v17 <= 1.0)
    {
        if (v17 >= 0.0)
            v16 = v107 * 4.0f;
        else
            v16 = 0.0;
    }
    else
    {
        v16 = 1.0;
    }
    v92 = v92 * v16;
    v73 = v73 * v107 * 0.25f + v73;
    v45 = v45 * v107 * 0.25f + v45;
    v114 = v114 * v107 * 0.5f + v114;
    v31 = v31 * v107 * 0.5f + v31;
    if (v38 >= 0.5)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v104->f_time_gfx_trace, 0.2f))
        {
            v106 = 0.0;
            v92 = 0.0;
            if (AI_EvalFunc_TIMEElapsed_C(v104->f_time_gfx_trace, 0.5))
            {
            LABEL_46:
                AI_EvalFunc_GFXSeti_C_CURRENT(v104->GFX_Airtrace, 12103, 0);
                v104->i_flag_trace_on = 0;
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                return 1000;
            }
        }
        else
        {
            v56 = TIM_gf_MainClock - TIM_gf_LockTime;
            v85 = 1.0f - (v56 - v104->f_time_gfx_trace) / 0.2f;
            v106 = v106 * v85;
            v92 = v92 * v85;
        }
    }
    if (v82)
        goto LABEL_46;
    if (!v104->i_flag_trace_on)
    {
        v106 = 0.0;
        v92 = 0.0;
        v104->i_flag_trace_on = 1;
    }
    v93 = AI_EvalFunc_COLORBlend_C(0, -1, v106);
    v57 = AI_EvalFunc_COLORBlend_C(0, -1, v92);
    v15 = -v38;
    if (v15 <= 1.0)
    {
        if (v15 >= -1.0)
            v108 = -v38;
        else
            v108 = -1.0;
    }
    else
    {
        v108 = 1.0;
    }
    v108 = v108 * 0.5f + 0.5f;
    v108 = v108 * 1.5f;
    v31 = v108 * v107 * v31;
    v114 = v108 * v107 * v114;
    v45 = v108 * v107 * v45;
    v73 = v108 * v107 * v73;
    GFXSetCurGlobal(v104->GFX_Airtrace);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12104, 0);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12109, v93);
    v68.x = -v29;
    v68.y = 0.0;
    v68.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12200, &v68);
    v101 = -v73 * 1.5f;
    v102 = 0;
    v103 = v114 * 1.5f;
    v14 = 1.0f - v52 + 1.0f - v52;
    v69.x = v101 * v14;
    v69.y = 0.0f * v14;
    v69.z = v103 * v14;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12201, &v69);
    v75 = -v73 * 1.5f;
    v76 = 0;
    v77 = v114 * 1.5f;
    v13 = 2.0f * v52;
    v109.x = v75 * v13;
    v109.y = 0.0f * v13;
    v109.z = v77 * v13;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12208, &v109);
    v1 = v104->f_last_valid_water_Z + 0.2f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(12003, v1);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12104, 5);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12109, v57);
    v47.x = v29;
    v47.y = 0.0;
    v47.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12200, &v47);
    v115 = v45 * 1.5f;
    v116 = 0;
    v117 = v31 * 1.5f;
    v12 = 1.0f - v52 + 1.0f - v52;
    v90.x = v115 * v12;
    v90.y = 0.0f * v12;
    v90.z = v117 * v12;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12201, &v90);
    v33 = v45 * 1.5f;
    v34 = 0;
    v35 = v31 * 1.5f;
    v11 = 2.0f * v52;
    v51.x = v33 * v11;
    v51.y = 0.0f * v11;
    v51.z = v35 * v11;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12208, &v51);
    v2 = v104->f_last_valid_water_Z + 0.2f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(12003, v2);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12104, 1);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12109, v93);
    v94.x = -v30;
    v94.y = 0.0;
    v94.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12200, &v94);
    v110 = -v73;
    v111 = 0;
    v112 = v114;
    v10 = 1.0f - v52;
    v79.x = v110 * v10;
    v79.y = 0.0f * v10;
    v79.z = v114 * v10;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12201, &v79);
    v59 = -v73;
    v60 = 0;
    v61 = v114;
    v88.x = v59 * v52;
    v88.y = 0.0f * v52;
    v88.z = v114 * v52;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12208, &v88);
    v3 = v104->f_last_valid_water_Z + 0.2f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(12003, v3);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12104, 4);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12109, v57);
    v50.x = v30;
    v50.y = 0.0;
    v50.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12200, &v50);
    v63 = v45;
    v64 = 0;
    v65 = v31;
    v9 = 1.0f - v52;
    v119.x = v45 * v9;
    v119.y = 0.0f * v9;
    v119.z = v31 * v9;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12201, &v119);
    v97 = v45;
    v98 = 0;
    v99 = v31;
    v43.x = v45 * v52;
    v43.y = 0.0f * v52;
    v43.z = v31 * v52;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12208, &v43);
    v4 = v104->f_last_valid_water_Z + 0.2f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(12003, v4);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12104, 2);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12109, v93);
    v96.x = -v105;
    v96.y = 0.0;
    v96.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12200, &v96);
    v120.x = 2.0;
    v120.y = 0.0;
    v120.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12201, &v120);
    v49.x = 0.5f;
    v49.y = 0.0;
    v49.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12208, &v49);
    v70.x = 0.0;
    v70.y = 0.0;
    v70.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12207, &v70);
    v5 = v104->f_last_valid_water_Z + 0.2f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(12003, v5);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12104, 3);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12109, v57);
    v46.x = v105;
    v46.y = 0.0;
    v46.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12200, &v46);
    v62.x = -2.0;
    v62.y = 0.0;
    v62.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12201, &v62);
    v23.x = -0.5;
    v23.y = 0.0;
    v23.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12208, &v23);
    v95.x = 0.0;
    v95.y = 0.0;
    v95.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12207, &v95);
    v6 = v104->f_last_valid_water_Z + 0.2f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(12003, v6);
    AI_EvalFunc_GFXSetv_C_CURGLOB(12203, &v72);
    AI_EvalFunc_GFXSetv_C_CURGLOB(12204, &v91);
    AI_EvalFunc_GFXSetv_C_CURGLOB(12205, &v55);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12103, 1);
    if (v104->GFX_Airtrace != -1)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v25);
        v41 = v25;
        v41.z = v104->f_last_valid_water_Z + 0.2f;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v78);
        a3 = v78;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v44);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v74);
        MATH_SubstractVec(&dest, &v41, &a3);
        v53 = dest.z * v74.z + dest.y * v74.y + v74.x * dest.x;
        v26 = v44.x * v53;
        v27 = v44.y * v53;
        v28 = v44.z * v53;
        a3.x = a3.x + v26;
        a3.y = a3.y + v27;
        a3.z = a3.z + v28;
        AI_EvalFunc_GFXSetv_C_CURRENT(v104->GFX_Airtrace, 12206, &a3);
    }
    return 1000;
}