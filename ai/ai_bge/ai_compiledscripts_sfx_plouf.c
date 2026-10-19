#include "ai_compiledscripts_inc.h"

//sfx_plouf_sp_init
int fct_sfx_plouf_sp_init_sfx_plouf_sp_bdis;
int fct_sfx_plouf_sp_init_sfx_plouf_sp_adis;
int sfx_plouf_sp_init_SFX_light_smoke_gao;
int fct_sfx_plouf_sp_init_sfx_plouf_sfx_loop;

int sfx_plouf_sfx_loop()
{
    int result; // eax
    tScriptStruct_SFXPlouf* v1; // [esp+4h] [ebp-28h]
    float v2; // [esp+8h] [ebp-24h]
    float v3; // [esp+18h] [ebp-14h]
    int v4; // [esp+28h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXPlouf*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v1->i_after = 0;
    v1->f_transparency_coef = 0.5f * TIM_gf_dt + v1->f_transparency_coef;
    if (v1->f_prog > 3.0)
        v1->f_transparency_coef1 = 0.5f * TIM_gf_dt + v1->f_transparency_coef1;
    if (v1->f_prog <= 9.0)
        v1->f_prog = v1->f_speed * TIM_gf_dt + v1->f_prog;
    else
        v1->f_prog = v1->f_speed * TIM_gf_dt * 0.30000001f + v1->f_prog;
    if (v1->f_prog > 10.0)
        v1->f_prog = 10.0;
    AI_EvalFunc_OBJ_SetChannelProg_C(AI_gpst_CurrentGameObject, 0, v1->f_prog);
    v2 = 3.0f * TIM_gf_dt;
    if (v2 < 0.0)
        v2 = 0.0;
    if (v2 > 1.0)
        v2 = 1.0;
    v3 = 5.0f * v2 + v1->f_scale_spark * (1.0f - v2);
    v1->f_scale_spark = v3;
    if (v1->des_spark)
        AI_EvalFunc_GFXSetf_C_CURRENT(v1->i_Gfx, 9007, v1->f_scale_spark);
    if (v1->f_transparency_coef <= 0.94999999 || (v4 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject), v4 == 1000))
        result = 1000;
    else
        result = v4;
    return result;
}

int sfx_plouf_sp_adis()
{
    tScriptStruct_SFXPlouf* v1; // [esp+0h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXPlouf*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v1->c_col, 0);
    AI_EvalFunc_SetALPHATRESH_C(AI_gpst_CurrentGameObject, 0, 0, 0);
    return 1000;
}

int sfx_plouf_sp_bdis()
{
    float v1; // [esp+4h] [ebp-18h]
    tScriptStruct_SFXPlouf* v2; // [esp+8h] [ebp-14h]
    float v3; // [esp+Ch] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SFXPlouf*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v2->col = AI_EvalFunc_COLORBlend_C(v2->c_col, 0, v2->f_transparency_coef);
    v3 = v2->f_transparency_coef1 * 500.0f;
    if (v3 > 255.0)
        v1 = 255.0;
    else
        v1 = v2->f_transparency_coef1 * 500.0f;
    AI_EvalFunc_SetALPHATRESH_C(AI_gpst_CurrentGameObject, 0, 0, (unsigned int)v1);
    AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v2->col, 0);
    return 1000;
}

int sfx_plouf_sp_init()
{
    double v0; // st7
    int result; // eax
    float zoom; // [esp+0h] [ebp-B4h]
    float zooma; // [esp+0h] [ebp-B4h]
    float zoomb; // [esp+0h] [ebp-B4h]
    tScriptStruct_SFXPlouf* v5; // [esp+4h] [ebp-B0h]
    tGameObjectMain* v6; // [esp+Ch] [ebp-A8h]
    tGameObjectMain* v7; // [esp+14h] [ebp-A0h]
    tGameObjectMain* v8; // [esp+1Ch] [ebp-98h]
    tGameObjectMain* v9; // [esp+24h] [ebp-90h]
    float v10; // [esp+28h] [ebp-8Ch]
    float v11; // [esp+2Ch] [ebp-88h]
    float v12; // [esp+30h] [ebp-84h]
    Vec3 v13; // [esp+38h] [ebp-7Ch] BYREF
    tGameObjectMain* v14; // [esp+44h] [ebp-70h]
    float v15; // [esp+48h] [ebp-6Ch]
    Vec3 a3; // [esp+4Ch] [ebp-68h] BYREF
    float v18; // [esp+60h] [ebp-54h]
    float v19; // [esp+64h] [ebp-50h]
    Vec3 a1; // [esp+6Ch] [ebp-48h] BYREF
    float v21; // [esp+78h] [ebp-3Ch]
    float v22; // [esp+7Ch] [ebp-38h]
    tGameObjectMain* v23; // [esp+84h] [ebp-30h]
    float v24; // [esp+88h] [ebp-2Ch]
    tScriptStruct_SFXPlouf* v25; // [esp+90h] [ebp-24h]
    Vec3 a2; // [esp+94h] [ebp-20h] BYREF
    Vec3 v27; // [esp+A4h] [ebp-10h] BYREF
    int v28; // [esp+B0h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_SFXPlouf*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v25 = v5;
    v14 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_sfx_plouf_sp_init_sfx_plouf_sp_bdis = (int)AI_ul_RealResolveOneRefFunction(0x3D002E2Du);
        fct_sfx_plouf_sp_init_sfx_plouf_sp_adis = (int)AI_ul_RealResolveOneRefFunction(0x3D002E2Eu);
        sfx_plouf_sp_init_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        fct_sfx_plouf_sp_init_sfx_plouf_sfx_loop = (int)AI_ul_RealResolveOneRefFunction(0x3D002E26u);
    }
    v14 = *AI_MainActor;
    v9 = v14;
    if (v14)
    {
        if ((unsigned int)v14 <= 2)
            v9 = AI_MainActor[(uint32_t)v14 - 1];
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v9);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v10 = (float)v0;
    if (v10 < 10000.0)
        v25->c_col = 0xAAAAAAAA;
    v23 = AI_gpst_CurrentGameObject;
    v8 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v8 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        AI_EvalFunc_AICBAdd_C(AI_gpst_CurrentGameObject, v8, 0, (tAIFunction*)fct_sfx_plouf_sp_init_sfx_plouf_sp_bdis);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(AI_gpst_CurrentGameObject, 0, 0, (tAIFunction*)fct_sfx_plouf_sp_init_sfx_plouf_sp_bdis);
    }
    v7 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v7 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        AI_EvalFunc_AICBAdd_C(AI_gpst_CurrentGameObject, v7, 1, (tAIFunction*)fct_sfx_plouf_sp_init_sfx_plouf_sp_adis);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(AI_gpst_CurrentGameObject, 0, 1, (tAIFunction*)fct_sfx_plouf_sp_init_sfx_plouf_sp_adis);
    }
    v11 = AI_EvalFunc_MATHRandFloat_C(-3.0, 3.0);
    AI_EvalFunc_OBJRotateGlobalZ_C(AI_gpst_CurrentGameObject, v11);
    v25->i_rand = AI_EvalFunc_MATHRandInt_C(-1, 2);
    v25->f_speed = AI_EvalFunc_MATHRandFloat_C(5.0, 7.0);
    v15 = AI_EvalFunc_OBJ_ZoomGet_C(AI_gpst_CurrentGameObject);
    zoom = v15 * 1.5f;
    AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, zoom);
    if (v25->des_spark)
    {
        v25->i_Gfx = AI_EvalFunc_GFXAdd_C_CURRENT(9);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v25->i_Gfx, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v25->i_Gfx, 2, 1);
        v6 = (tGameObjectMain*)sfx_plouf_sp_init_SFX_light_smoke_gao;
        if (sfx_plouf_sp_init_SFX_light_smoke_gao)
        {
            if ((unsigned int)sfx_plouf_sp_init_SFX_light_smoke_gao <= 2)
                v6 = AI_MainActor[sfx_plouf_sp_init_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v25->i_Gfx, v6, 5);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v25->i_Gfx, 0, 5);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
        a3.x = 0.0;
        a3.y = 0.0;
        a3.z = -0.5;
        MATH_VecAddVec(&a1, &a2, &a3);
        GFXSetCurGlobal(v25->i_Gfx);
        AI_EvalFunc_GFXSetv_C_CURGLOB(9200, &a1);
        v13.x = 0.0;
        v13.y = 0.0;
        v13.z = 1.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(9201, &v13);
        v27.x = 0.0;
        v27.y = 0.0;
        v27.z = -14.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(9202, &v27);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9100, 20);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9101, 0xFFAAAAAA);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9102, 24);
        v22 = AI_EvalFunc_MATHRandFloat_C(0.40000001f, 0.80000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9000, v22);
        if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(1))
        {
            v18 = AI_EvalFunc_OBJ_ZoomGet_C(AI_gpst_CurrentGameObject);
            GFXSetCurGlobal(v25->i_Gfx);
            zooma = 10.0f * v18;
            AI_EvalFunc_GFXSetf_C_CURGLOB(9001, zooma);
            v12 = AI_EvalFunc_OBJ_ZoomGet_C(AI_gpst_CurrentGameObject);
            zoomb = 30.0f * v12;
            AI_EvalFunc_GFXSetf_C_CURGLOB(9002, zoomb);
            v19 = AI_EvalFunc_OBJ_ZoomGet_C(AI_gpst_CurrentGameObject);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9004, v19);
        }
        else
        {
            v24 = AI_EvalFunc_MATHRandFloat_C(20.0, 40.0);
            GFXSetCurGlobal(v25->i_Gfx);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9001, v24);
            v21 = AI_EvalFunc_MATHRandFloat_C(40.0, 60.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9002, v21);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9004, 10.0);
        }
        GFXSetCurGlobal(v25->i_Gfx);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9003, 0.99000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9005, 0.80000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9006, 1.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9007, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9008, 0.0099999998f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9009, 0.40000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9010, 0.5);
    }
    v28 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_sfx_plouf_sp_init_sfx_plouf_sfx_loop);
    if (v28 == 1000)
        result = 1000;
    else
        result = v28;
    return result;
}