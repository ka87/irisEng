#include "ai_compiledscripts_inc.h"

//tir_blok_hyllis_tbh_init
int fct_tir_blok_hyllis_tbh_init_tir_blok_hyllis_tbh_attend;

//tir_blok_hyllis_tbh_sort
int fct_tir_blok_hyllis_tbh_sort_tir_blok_hyllis_tbh_shoot;

//tir_blok_hyllis_tbh_sort
int fct_tir_blok_hyllis_tbh_attend_tir_blok_hyllis_tbh_sort;

//tir_blok_hyllis_tbh_entre
int fct_tir_blok_hyllis_tbh_entre_tir_blok_hyllis_tbh_attend;

//tir_blok_hyllis_tbh_shoot
int tir_blok_hyllis_tbh_shoot_SFX_light_smoke_gao;
int tir_blok_hyllis_tbh_shoot_univVars;
int tir_blok_hyllis_tbh_shoot_univVars_i_text1_trucquiblock;
int tir_blok_hyllis_tbh_shoot_TXT_Sally_gao;
int tir_blok_hyllis_tbh_shoot_univVars_i_text2_trucquiblock;
int tir_blok_hyllis_tbh_shoot_tir_blok_hyllis_tbh_entre;
int tir_blok_hyllis_tbh_shoot_GST_Global_gao;
int tir_blok_hyllis_tbh_shoot_GST_Global_gao_f_alarme_timer_start;
int tir_blok_hyllis_tbh_shoot_GST_Global_gao_f_alarme_timer_autooff;

int tir_blok_hyllis_tbh_init()
{
    int result; // eax
    tScriptStruct_TirBlokHyllis* v1; // [esp+0h] [ebp-28h]
    int v2; // [esp+4h] [ebp-24h]
    Vec3 a2; // [esp+Ch] [ebp-1Ch] BYREF
    Vec3 v4; // [esp+18h] [ebp-10h] BYREF
    tScriptStruct_TirBlokHyllis* v5; // [esp+24h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_TirBlokHyllis*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v5 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_tir_blok_hyllis_tbh_init_tir_blok_hyllis_tbh_attend = (int)AI_ul_RealResolveOneRefFunction(0x3D001250u);
    }
    v5->o_obj_detect = (int)*AI_MainActor;
    v5->des_f_dist = v5->des_f_dist * v5->des_f_dist;
    v5->des_f_dist_shoot = v5->des_f_dist_shoot * v5->des_f_dist_shoot;
    v5->des_f_dist_text2 = v5->des_f_dist_text2 * v5->des_f_dist_text2;
    AI_EvalFunc_OBJDrawMaskSet_C(AI_gpst_CurrentGameObject, 0, 8);
    v4.x = -20.0;
    v4.y = -20.0;
    v4.z = -100.0;
    AI_EvalFunc_BVMinSet_C(AI_gpst_CurrentGameObject, &v4);
    a2.x = 20.0;
    a2.y = 20.0;
    a2.z = 1500.0;
    AI_EvalFunc_BVMaxSet_C(AI_gpst_CurrentGameObject, &a2);
    AI_EvalFunc_OBJ_SetChannelProg_C(AI_gpst_CurrentGameObject, 0, 1.0);
    v2 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_tir_blok_hyllis_tbh_init_tir_blok_hyllis_tbh_attend);
    if (v2 == 1000)
        result = 1000;
    else
        result = v2;
    return result;
}

int tir_blok_hyllis_tbh_sort()
{
    tScriptStruct_TirBlokHyllis* v1; // [esp+4h] [ebp-60h]
    float v2; // [esp+8h] [ebp-5Ch]
    float v3; // [esp+1Ch] [ebp-48h]
    float v4; // [esp+28h] [ebp-3Ch]
    Vec3 v5; // [esp+34h] [ebp-30h] BYREF
    float v6; // [esp+40h] [ebp-24h]
    Vec3 a1; // [esp+44h] [ebp-20h] BYREF
    Vec3 a2; // [esp+50h] [ebp-14h] BYREF
    tScriptStruct_TirBlokHyllis* v9; // [esp+5Ch] [ebp-8h]
    int v10; // [esp+60h] [ebp-4h]

    v5.x = 1.0;
    v5.y = 0.0;
    v5.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_TirBlokHyllis*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v9 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_tir_blok_hyllis_tbh_sort_tir_blok_hyllis_tbh_shoot = (int)AI_ul_RealResolveOneRefFunction(0x3D001251u);
    }
    v9->o_obj_detect = (int)*AI_MainActor;
    AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &a2);
    v4 = a2.z;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
    v5 = a1;
    v2 = TIM_gf_dt;
    if (TIM_gf_dt < 0.0)
        v2 = 0.0;
    if (v2 > 1.0)
        v2 = 1.0;
    v3 = (v4 + 130.0f) * v2 + (1.0f - v2) * v5.z;
    v5.z = v3;
    AI_EvalFunc_OBJSetPos_C_CURRENT(&v5);
    v6 = TIM_gf_dt;
    v9->f_morph = 5.0f * TIM_gf_dt + v9->f_morph;
    if (v9->f_morph >= 5.0)
    {
        v9->f_morph = 5.0;
        AI_EvalFunc_OBJ_SetChannelProg_C(AI_gpst_CurrentGameObject, 0, 5.0);
        v10 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_tir_blok_hyllis_tbh_sort_tir_blok_hyllis_tbh_shoot);
        if (v10 != 1000)
            return v10;
    }
    AI_EvalFunc_OBJ_SetChannelProg_C(AI_gpst_CurrentGameObject, 0, v9->f_morph);
    return 1000;
}

int tir_blok_hyllis_tbh_attend()
{
    double v0; // st7
    int result; // eax
    tScriptStruct_TirBlokHyllis* v2; // [esp+4h] [ebp-70h]
    tGameObjectMain* v3; // [esp+Ch] [ebp-68h]
    Vec3 v4; // [esp+14h] [ebp-60h] BYREF
    float v5; // [esp+20h] [ebp-54h]
    Vec3 a1; // [esp+24h] [ebp-50h] BYREF
    float v7; // [esp+30h] [ebp-44h]
    Vec3 v8; // [esp+34h] [ebp-40h]
    int v9; // [esp+40h] [ebp-34h]
    Vec3 v10; // [esp+44h] [ebp-30h]
    Vec3 v11; // [esp+50h] [ebp-24h]
    float v12; // [esp+5Ch] [ebp-18h]
    tScriptStruct_TirBlokHyllis* v13; // [esp+60h] [ebp-14h]
    Vec3 v14; // [esp+64h] [ebp-10h] BYREF

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_TirBlokHyllis*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v13 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_tir_blok_hyllis_tbh_attend_tir_blok_hyllis_tbh_sort = (int)AI_ul_RealResolveOneRefFunction(0x3D00126Bu);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
    v12 = sqrtf(v13->des_f_dist);
    v10.x = 0.0;
    v10.y = 0.0;
    v10.z = 1.0;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v4);
    sqrtf(v13->des_f_dist_shoot);
    v8.x = 0.0;
    v8.y = 0.0;
    v8.z = 1.0;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v14);
    v5 = sqrtf(v13->des_f_dist_text2);
    v11.x = 0.0;
    v11.y = 0.0;
    v11.z = 1.0;
    v13->o_obj_detect = (int)*AI_MainActor;
    v3 = (tGameObjectMain*)v13->o_obj_detect;
    if (v13->o_obj_detect)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        v0 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v3);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
    }
    v7 = (float)v0;
    if (v13->des_f_dist <= (double)v7
        || (AI_EvalFunc_OBJDrawMaskSet_C(AI_gpst_CurrentGameObject, 8, 0),
            v13->f_morph = 1.0,
            v9 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_tir_blok_hyllis_tbh_attend_tir_blok_hyllis_tbh_sort),
            v9 == 1000))
    {
        result = 1000;
    }
    else
    {
        result = v9;
    }
    return result;
}

int tir_blok_hyllis_tbh_entre()
{
    tScriptStruct_TirBlokHyllis* v1; // [esp+4h] [ebp-60h]
    float v2; // [esp+8h] [ebp-5Ch]
    float v3; // [esp+1Ch] [ebp-48h]
    float v4; // [esp+28h] [ebp-3Ch]
    Vec3 v5; // [esp+34h] [ebp-30h] BYREF
    float v6; // [esp+40h] [ebp-24h]
    Vec3 a1; // [esp+44h] [ebp-20h] BYREF
    Vec3 a2; // [esp+50h] [ebp-14h] BYREF
    tScriptStruct_TirBlokHyllis* v9; // [esp+5Ch] [ebp-8h]
    int v10; // [esp+60h] [ebp-4h]

    v5.x = 1.0;
    v5.y = 0.0;
    v5.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_TirBlokHyllis*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v9 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_tir_blok_hyllis_tbh_entre_tir_blok_hyllis_tbh_attend = (int)AI_ul_RealResolveOneRefFunction(0x3D001250u);
    }
    v9->o_obj_detect = (int)*AI_MainActor;
    AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &a2);
    v4 = a2.z;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
    v5 = a1;
    v2 = TIM_gf_dt;
    if (TIM_gf_dt < 0.0)
        v2 = 0.0;
    if (v2 > 1.0)
        v2 = 1.0;
    v3 = v4 * v2 + (1.0f - v2) * v5.z;
    v5.z = v3;
    AI_EvalFunc_OBJSetPos_C_CURRENT(&v5);
    v6 = TIM_gf_dt;
    v9->f_morph = v9->f_morph - 5.0f * TIM_gf_dt;
    if (v9->f_morph <= 1.0)
    {
        AI_EvalFunc_OBJDrawMaskSet_C(AI_gpst_CurrentGameObject, 0, 8);
        v9->f_morph = 1.0;
        AI_EvalFunc_OBJ_SetChannelProg_C(AI_gpst_CurrentGameObject, 0, 1.0);
        v10 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_tir_blok_hyllis_tbh_entre_tir_blok_hyllis_tbh_attend);
        if (v10 != 1000)
            return v10;
    }
    AI_EvalFunc_OBJ_SetChannelProg_C(AI_gpst_CurrentGameObject, 0, v9->f_morph);
    return 1000;
}

int tir_blok_hyllis_tbh_shoot()
{
    double v0; // st7
    double v1; // st7
    double v2; // st7
    double v3; // st7
    double v4; // st7
    double v5; // st7
    double v7; // st7
    tGameObjectMain* v8; // eax
    tGameObjectMain* v9; // eax
    float a4; // [esp+0h] [ebp-3B0h]
    tScriptStruct_TirBlokHyllis* v11; // [esp+Ch] [ebp-3A4h]
    tGameObjectMain* v12; // [esp+14h] [ebp-39Ch]
    float v13; // [esp+28h] [ebp-388h]
    tGameObjectMain* v14; // [esp+30h] [ebp-380h]
    float v15; // [esp+3Ch] [ebp-374h]
    tGameObjectMain* v16; // [esp+58h] [ebp-358h]
    tAI* v17; // [esp+64h] [ebp-34Ch]
    tAI* v18; // [esp+68h] [ebp-348h]
    tGameObjectMain* v19; // [esp+6Ch] [ebp-344h]
    tGameObjectMain* v20; // [esp+70h] [ebp-340h]
    tGameObjectMain* v21; // [esp+74h] [ebp-33Ch]
    tGameObjectMain* v22; // [esp+78h] [ebp-338h]
    tAI* v23; // [esp+7Ch] [ebp-334h]
    tAI* v24; // [esp+80h] [ebp-330h]
    tGameObjectMain* v25; // [esp+84h] [ebp-32Ch]
    tGameObjectMain* v26; // [esp+88h] [ebp-328h]
    tGameObjectMain* v27; // [esp+90h] [ebp-320h]
    tAI* v28; // [esp+94h] [ebp-31Ch]
    tAI* v29; // [esp+98h] [ebp-318h]
    tGameObjectMain* v30; // [esp+9Ch] [ebp-314h]
    tGameObjectMain* v31; // [esp+A0h] [ebp-310h]
    tGameObjectMain* v32; // [esp+A4h] [ebp-30Ch]
    tGameObjectMain* v33; // [esp+A8h] [ebp-308h]
    tAI* v34; // [esp+ACh] [ebp-304h]
    tAI* v35; // [esp+B0h] [ebp-300h]
    tGameObjectMain* v36; // [esp+B4h] [ebp-2FCh]
    tGameObjectMain* v37; // [esp+B8h] [ebp-2F8h]
    tGameObjectMain* v38; // [esp+C0h] [ebp-2F0h]
    tGameObjectMain* v39; // [esp+D0h] [ebp-2E0h]
    tGameObjectMain* v40; // [esp+F8h] [ebp-2B8h]
    tGameObjectMain* v41; // [esp+100h] [ebp-2B0h]
    tGameObjectMain* v42; // [esp+108h] [ebp-2A8h]
    tGameObjectMain* v43; // [esp+110h] [ebp-2A0h]
    tGameObjectMain* v44; // [esp+118h] [ebp-298h]
    tGameObjectMain* v45; // [esp+120h] [ebp-290h]
    tGameObjectMain* v46; // [esp+128h] [ebp-288h]
    tGameObjectMain* v47; // [esp+130h] [ebp-280h]
    tGameObjectMain* v48; // [esp+138h] [ebp-278h]
    Vec3 v49; // [esp+13Ch] [ebp-274h] BYREF
    Vec3 v50; // [esp+148h] [ebp-268h] BYREF
    float v51; // [esp+160h] [ebp-250h]
    Vec3 a1; // [esp+164h] [ebp-24Ch] BYREF
    Vec3 v53; // [esp+170h] [ebp-240h] BYREF
    Vec3 v; // [esp+17Ch] [ebp-234h] BYREF
    Vec3 v55; // [esp+188h] [ebp-228h] BYREF
    float v56; // [esp+194h] [ebp-21Ch]
    Vec3 v57; // [esp+198h] [ebp-218h] BYREF
    Vec3 v58; // [esp+1A4h] [ebp-20Ch] BYREF
    Vec3 v59; // [esp+1B0h] [ebp-200h] BYREF
    Vec3 v60; // [esp+1BCh] [ebp-1F4h] BYREF
    float v61; // [esp+1CCh] [ebp-1E4h]
    tMessage Src; // [esp+1D0h] [ebp-1E0h] BYREF
    Vec3 v63; // [esp+204h] [ebp-1ACh] BYREF
    float v64; // [esp+214h] [ebp-19Ch]
    Vec3 v65; // [esp+218h] [ebp-198h] BYREF
    Vec3 v66; // [esp+224h] [ebp-18Ch] BYREF
    Vec3 v67; // [esp+230h] [ebp-180h] BYREF
    Vec3 v68; // [esp+23Ch] [ebp-174h] BYREF
    Vec3 v69; // [esp+248h] [ebp-168h] BYREF
    Vec3 v70; // [esp+254h] [ebp-15Ch] BYREF
    float v71; // [esp+26Ch] [ebp-144h]
    Vec3 a2; // [esp+270h] [ebp-140h] BYREF
    float v73; // [esp+27Ch] [ebp-134h]
    float v74; // [esp+280h] [ebp-130h]
    float v75; // [esp+284h] [ebp-12Ch]
    float v76; // [esp+288h] [ebp-128h]
    float v77; // [esp+28Ch] [ebp-124h]
    Vec3 pos; // [esp+29Ch] [ebp-114h] BYREF
    Vec3 a5; // [esp+2A8h] [ebp-108h] BYREF
    Vec3 v80; // [esp+2B4h] [ebp-FCh] BYREF
    Vec3 v81; // [esp+2C0h] [ebp-F0h] BYREF
    Vec3 v82; // [esp+2D0h] [ebp-E0h] BYREF
    Vec3 v83; // [esp+2DCh] [ebp-D4h] BYREF
    Vec3 dest; // [esp+2ECh] [ebp-C4h] BYREF
    Vec3 v85; // [esp+2F8h] [ebp-B8h] BYREF
    int v86; // [esp+308h] [ebp-A8h]
    Vec3 v87; // [esp+30Ch] [ebp-A4h] BYREF
    Vec3 v88; // [esp+318h] [ebp-98h] BYREF
    Vec3 v89; // [esp+324h] [ebp-8Ch] BYREF
    Vec3 v90; // [esp+330h] [ebp-80h] BYREF
    Vec3 a3; // [esp+33Ch] [ebp-74h] BYREF
    tScriptStruct_TirBlokHyllis* v92; // [esp+348h] [ebp-68h]
    Vec3 v93; // [esp+34Ch] [ebp-64h] BYREF
    Vec3 v94; // [esp+358h] [ebp-58h] BYREF
    Vec3 v95; // [esp+364h] [ebp-4Ch] BYREF
    float v96; // [esp+370h] [ebp-40h]
    Vec3 v97; // [esp+378h] [ebp-38h] BYREF
    Vec3 v98; // [esp+390h] [ebp-20h] BYREF
    Vec3 v99; // [esp+39Ch] [ebp-14h] BYREF
    float v100; // [esp+3A8h] [ebp-8h]

    pos.x = 1.0;
    pos.y = 0.0;
    pos.z = 0.0;
    v66.x = 1.0;
    v66.y = 0.0;
    v66.z = 0.0;
    v.x = 1.0;
    v.y = 0.0;
    v.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v11 = (tScriptStruct_TirBlokHyllis*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v11 = 0;
    v92 = v11;
    v100 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        tir_blok_hyllis_tbh_shoot_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        tir_blok_hyllis_tbh_shoot_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &tir_blok_hyllis_tbh_shoot_univVars_i_text1_trucquiblock,
            (void*)tir_blok_hyllis_tbh_shoot_univVars,
            20384);
        tir_blok_hyllis_tbh_shoot_TXT_Sally_gao = AI_ul_RealResolveOneRefObject(0x7200064Eu);
        AI2C_ResolveBuffer(
            &tir_blok_hyllis_tbh_shoot_univVars_i_text2_trucquiblock,
            (void*)tir_blok_hyllis_tbh_shoot_univVars,
            20388);
        tir_blok_hyllis_tbh_shoot_tir_blok_hyllis_tbh_entre = (int)AI_ul_RealResolveOneRefFunction(0x3D00126Eu);
        tir_blok_hyllis_tbh_shoot_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &tir_blok_hyllis_tbh_shoot_GST_Global_gao_f_alarme_timer_start,
            (void*)tir_blok_hyllis_tbh_shoot_GST_Global_gao,
            8);
        AI2C_ResolveBuffer(
            &tir_blok_hyllis_tbh_shoot_GST_Global_gao_f_alarme_timer_autooff,
            (void*)tir_blok_hyllis_tbh_shoot_GST_Global_gao,
            4);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)10)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v69);
        v85.x = 0.0;
        v85.y = 0.0;
        v85.z = 50.0;
        MATH_VecAddVec(&v60, &v69, &v85);
        pos = v60;
        v16 = (tGameObjectMain*)v92->o_obj_detect;
        if (v92->o_obj_detect)
        {
            if ((unsigned int)v16 <= 2)
                v16 = AI_MainActor[(uint32_t)v16 - 1];
            AI_EvalFunc_OBJGetPos_C(v16, &v70);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v70);
        }
        MATH_SubstractVec(&v98, &v70, &pos);
        v = v98;
        MATH_NormalizeEqualVector(&v);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v88);
        v15 = v92->des_f_factor_sight;
        v55.x = v88.x * v15;
        v55.y = v88.y * v15;
        v55.z = v88.z * v15;
        MATH_VecAddVec(&v87, &v55, &v);
        v66 = v87;
        v99.x = 0.0;
        v99.y = 0.0;
        v99.z = 1.0;
        v14 = (tGameObjectMain*)v92->o_obj1;
        if (v14)
        {
            if ((unsigned int)v14 <= 2)
                v14 = AI_MainActor[(uint32_t)v14 - 1];
            AI_EvalFunc_OBJ_SightGeneralSet_C(v14, &v66, &v99);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGeneralSet_C(0, &v66, &v99);
        }
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v57);
        v13 = v92->des_f_factor_sight;
        v73 = v57.x * v13;
        v74 = v57.y * v13;
        v75 = v57.z * v13;
        v58.x = -v73;
        v58.y = -v74;
        v58.z = -v75;
        MATH_VecAddVec(&v68, &v58, &v);
        v66 = v68;
        v49.x = 0.0;
        v49.y = 0.0;
        v49.z = 1.0;
        v12 = (tGameObjectMain*)v92->o_obj2;
        if (v12)
        {
            if ((unsigned int)v12 <= 2)
                v12 = AI_MainActor[(uint32_t)v12 - 1];
            AI_EvalFunc_OBJ_SightGeneralSet_C(v12, &v66, &v49);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGeneralSet_C(0, &v66, &v49);
        }
        return 1000;
    }
    v92->o_obj_detect = (int)*AI_MainActor;
    if (v92->GFX_Halo == -1)
    {
        v46 = (tGameObjectMain*)v92->o_obj_detect;
        if (v92->o_obj_detect)
        {
            if ((unsigned int)v46 <= 2)
                v46 = AI_MainActor[(uint32_t)v46 - 1];
            v2 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v46);
        }
        else
        {
            v2 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
        }
        v96 = (float)v2;
        if (v92->des_f_dist > (double)v96)
        {
            v92->GFX_Halo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v92->GFX_Halo, 4, 1);
            v45 = (tGameObjectMain*)tir_blok_hyllis_tbh_shoot_SFX_light_smoke_gao;
            if (tir_blok_hyllis_tbh_shoot_SFX_light_smoke_gao)
            {
                if ((unsigned int)tir_blok_hyllis_tbh_shoot_SFX_light_smoke_gao <= 2)
                    v45 = AI_MainActor[tir_blok_hyllis_tbh_shoot_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v92->GFX_Halo, v45, 0);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v92->GFX_Halo, 0, 0);
            }
            GFXSetCurGlobal(v92->GFX_Halo);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 60.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, -20.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 5.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21004, 50.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21006, 0.0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 0x3080A0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21104, 2);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v83);
            v93.x = 0.0;
            v93.y = 0.0;
            v93.z = -150.0;
            MATH_VecAddVec(&v63, &v83, &v93);
            AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v63);
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v81);
            AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v81);
        }
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v89);
        v82.x = 0.0;
        v82.y = 0.0;
        v82.z = -150.0;
        MATH_VecAddVec(&a1, &v89, &v82);
        AI_EvalFunc_GFXSetv_C_CURRENT(v92->GFX_Halo, 21200, &a1);
        v48 = (tGameObjectMain*)v92->o_obj_detect;
        if (v92->o_obj_detect)
        {
            if ((unsigned int)v48 <= 2)
                v48 = AI_MainActor[(uint32_t)v48 - 1];
            v0 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v48);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
        }
        v51 = (float)v0;
        if (v92->des_f_dist >= (double)v51)
        {
            v47 = (tGameObjectMain*)v92->o_obj_detect;
            if (v92->o_obj_detect)
            {
                if ((unsigned int)v47 <= 2)
                    v47 = AI_MainActor[(uint32_t)v47 - 1];
                v1 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v47);
            }
            else
            {
                v1 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
            }
            v77 = (float)v1;
            if (v92->des_f_dist_text2 >= (double)v77)
                AI_EvalFunc_GFXSeti_C_CURRENT(v92->GFX_Halo, 21101, 0xFF);
            else
                AI_EvalFunc_GFXSeti_C_CURRENT(v92->GFX_Halo, 21101, 0x3080A0);
        }
        else
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v92->GFX_Halo);
            v92->GFX_Halo = -1;
        }
    }
    if (!*(uint32_t*)tir_blok_hyllis_tbh_shoot_univVars_i_text1_trucquiblock)
    {
        v44 = (tGameObjectMain*)v92->o_obj_detect;
        if (v92->o_obj_detect)
        {
            if ((unsigned int)v44 <= 2)
                v44 = AI_MainActor[(uint32_t)v44 - 1];
            v3 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v44);
        }
        else
        {
            v3 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
        }
        v56 = (float)v3;
        if (v92->des_f_dist > (double)v56)
        {
            *(uint32_t*)tir_blok_hyllis_tbh_shoot_univVars_i_text1_trucquiblock = 1;
            Src.msgID = 30;
            Src.Int1 = 57;
            Src.Int2 = 10;
            Src.f2 = 4.0;
            Src.v1.x = 0.0;
            Src.v1.y = 0.0;
            Src.v1.z = 0.0;
            Src.v2.x = 0.0;
            Src.v2.y = 0.0;
            Src.v2.z = 0.0;
            Src.gao = 0;
            Src.gao2 = (tGameObjectMain*)v92->o_obj_detect;
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)tir_blok_hyllis_tbh_shoot_TXT_Sally_gao, &Src);
        }
    }
    if (!*(uint32_t*)tir_blok_hyllis_tbh_shoot_univVars_i_text2_trucquiblock)
    {
        v43 = (tGameObjectMain*)v92->o_obj_detect;
        if (v92->o_obj_detect)
        {
            if ((unsigned int)v43 <= 2)
                v43 = AI_MainActor[(uint32_t)v43 - 1];
            v4 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v43);
        }
        else
        {
            v4 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
        }
        v64 = (float)v4;
        if (v92->des_f_dist_text2 > (double)v64)
        {
            *(uint32_t*)tir_blok_hyllis_tbh_shoot_univVars_i_text2_trucquiblock = 1;
            Src.msgID = 30;
            Src.Int1 = 37;
            Src.Int2 = 10;
            Src.f2 = 4.0;
            Src.v1.x = 0.0;
            Src.v1.y = 0.0;
            Src.v1.z = 0.0;
            Src.v2.x = 0.0;
            Src.v2.y = 0.0;
            Src.v2.z = 0.0;
            Src.gao = 0;
            Src.gao2 = (tGameObjectMain*)v92->o_obj_detect;
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)tir_blok_hyllis_tbh_shoot_TXT_Sally_gao, &Src);
        }
    }
    v42 = (tGameObjectMain*)v92->o_obj_detect;
    if (v92->o_obj_detect)
    {
        if ((unsigned int)v42 <= 2)
            v42 = AI_MainActor[(uint32_t)v42 - 1];
        v5 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v42);
    }
    else
    {
        v5 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
    }
    v71 = (float)v5;
    if (v92->des_f_dist < (double)v71)
    {
        *(uint32_t*)tir_blok_hyllis_tbh_shoot_univVars_i_text1_trucquiblock = 0;
        *(uint32_t*)tir_blok_hyllis_tbh_shoot_univVars_i_text2_trucquiblock = 0;
        v86 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)tir_blok_hyllis_tbh_shoot_tir_blok_hyllis_tbh_entre);
        if (v86 != 1000)
            return v86;
    }
    *(uint32_t*)tir_blok_hyllis_tbh_shoot_GST_Global_gao_f_alarme_timer_start = 0;
    *(uint32_t*)tir_blok_hyllis_tbh_shoot_GST_Global_gao_f_alarme_timer_autooff = 0x3F800000;
    AI_EvalFunc_OBJ_SightInitGet_C(AI_gpst_CurrentGameObject, &v80);
    v41 = (tGameObjectMain*)v92->o_obj_detect;
    if (v92->o_obj_detect)
    {
        if ((unsigned int)v41 <= 2)
            v41 = AI_MainActor[(uint32_t)v41 - 1];
        AI_EvalFunc_OBJ_SightGet_C(v41, &v97);
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C(0, &v97);
    }
    v100 = v97.z * v80.z + v97.y * v80.y + v80.x * v97.x;
    v40 = (tGameObjectMain*)v92->o_obj_detect;
    if (v92->o_obj_detect)
    {
        if ((unsigned int)v40 <= 2)
            v40 = AI_MainActor[(uint32_t)v40 - 1];
        AI_EvalFunc_OBJGetPos_C(v40, &v67);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v67);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v65);
    MATH_SubstractVec(&dest, &v67, &v65);
    v = dest;
    AI_EvalFunc_MATHVecSetHorzNormalize_C(&v);
    v53.x = 0.0;
    v53.y = 0.0;
    v53.z = 1.0;
    AI_EvalFunc_MATHVecCrossProduct_C(&v, &v53, &v94);
    v = v94;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
    a3.x = -v.x;
    a3.y = -v.y;
    a3.z = -v.z;
    v76 = TIM_gf_dt;
    a4 = 6.0f * TIM_gf_dt;
    AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
    v = a5;
    v95.x = 0.0;
    v95.y = 0.0;
    v95.z = 1.0;
    AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v, &v95);
    v39 = (tGameObjectMain*)v92->o_obj_detect;
    if (v92->o_obj_detect)
    {
        if ((unsigned int)v39 <= 2)
            v39 = AI_MainActor[(uint32_t)v39 - 1];
        v7 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v39);
    }
    else
    {
        v7 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
    }
    v61 = (float)v7;
    if (v92->des_f_dist_shoot < (double)v61)
        return 1000;
    if (!AI_EvalFunc_TIMEElapsed_C(v92->f_time_tir, 0.5))
        return 1000;
    v92->f_time_tir = TIM_gf_MainClock - TIM_gf_LockTime;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v50);
    v59.x = 0.0;
    v59.y = 0.0;
    v59.z = 50.0;
    MATH_VecAddVec(&v90, &v50, &v59);
    pos = v90;
    v38 = (tGameObjectMain*)v92->o_laser;
    if (v38)
    {
        if ((unsigned int)v38 <= 2)
            v38 = AI_MainActor[(uint32_t)v38 - 1];
        v8 = AI_EvalFunc_OBJDuplicate_C(v38, &pos);
    }
    else
    {
        v8 = AI_EvalFunc_OBJDuplicate_C(0, &pos);
    }
    v92->o_obj1 = (int)v8;
    v35 = (tAI*)v92->o_obj1;
    if (v35 == WOR_gst_Universe.univAIInstance)
    {
        v34 = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        v37 = (tGameObjectMain*)v92->o_obj1;
        if (v35)
        {
            if ((unsigned int)v35 <= 2)
                v37 = AI_MainActor[(uint32_t)v35 - 1];
            v36 = v37;
        }
        else
        {
            v36 = 0;
        }
        v34 = v36->extended->AIInstance;
    }
    ((tScriptStruct_Laser*)(v34->instanceVarDataCurrent))->des_target = v92->o_obj_detect;
    v33 = (tGameObjectMain*)v92->o_gao_axis_paf;
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
    if (v32)
    {
        v29 = (tAI*)v92->o_obj1;
        if (v29 == WOR_gst_Universe.univAIInstance)
        {
            v28 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v31 = (tGameObjectMain*)v92->o_obj1;
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
        ((tScriptStruct_Laser*)(v28->instanceVarDataCurrent))->o_gao_axis_paf = v92->o_gao_axis_paf;
    }
    v27 = (tGameObjectMain*)v92->o_laser;
    if (v27)
    {
        if ((unsigned int)v27 <= 2)
            v27 = AI_MainActor[(uint32_t)v27 - 1];
        v9 = AI_EvalFunc_OBJDuplicate_C(v27, &pos);
    }
    else
    {
        v9 = AI_EvalFunc_OBJDuplicate_C(0, &pos);
    }
    v92->o_obj2 = (int)v9;
    v24 = (tAI*)v92->o_obj2;
    if (v24 == WOR_gst_Universe.univAIInstance)
    {
        v23 = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        v26 = (tGameObjectMain*)v92->o_obj2;
        if (v24)
        {
            if ((unsigned int)v24 <= 2)
                v26 = AI_MainActor[(uint32_t)v24 - 1];
            v25 = v26;
        }
        else
        {
            v25 = 0;
        }
        v23 = v25->extended->AIInstance;
    }
    ((tScriptStruct_Laser*)(v23->instanceVarDataCurrent))->des_target = v92->o_obj_detect;
    v22 = (tGameObjectMain*)v92->o_gao_axis_paf;
    if (v22)
    {
        if ((unsigned int)v22 <= 2)
            v22 = AI_MainActor[(uint32_t)v22 - 1];
        v21 = v22;
    }
    else
    {
        v21 = 0;
    }
    if (v21)
    {
        v18 = (tAI*)v92->o_obj1;
        if (v18 == WOR_gst_Universe.univAIInstance)
        {
            v17 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v20 = (tGameObjectMain*)v92->o_obj1;
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
        ((tScriptStruct_Laser*)(v17->instanceVarDataCurrent))->o_gao_axis_paf = v92->o_gao_axis_paf;
    }
    *AI_gppst_CurrentJumpNode = (tAINode*)10;
    TIME_Wait_C(0.1f);
    return 0;
}