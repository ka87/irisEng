#include "ai_compiledscripts_inc.h"

//Sally_TeteChercheuses_stc_go
int Sally_TeteChercheuses_stc_go_GST_Fight_Manager_gao;
int Sally_TeteChercheuses_stc_go_GST_Fight_Manager_gao_ao_Evil_Actor;
int fct_Sally_TeteChercheuses_stc_go_Sally_TeteChercheuses_stc_end;
int fct_Sally_TeteChercheuses_stc_go_Sally_TeteChercheuses_stc_go;
int Sally_TeteChercheuses_stc_go_SFX_light_smoke_gao;
int Sally_TeteChercheuses_stc_go_univVars;
int Sally_TeteChercheuses_stc_go_univVars_Perso_ID_actor;

//Sally_TeteChercheuses_stc_loop
int Sally_TeteChercheuses_stc_loop__PJ_Sally_gao;
int fct_Sally_TeteChercheuses_stc_loop_Sally_TeteChercheuses_stc_loop_end;

//Sally_TeteChercheuses_stc_init
int Sally_TeteChercheuses_stc_init_SFX_light_smoke_gao;
int Sally_TeteChercheuses_stc_init__PJ_Sally_gao;
int fct_Sally_TeteChercheuses_stc_init_Sally_TeteChercheuses_stc_loop;
int fct_Sally_TeteChercheuses_stc_init_Sally_TeteChercheuses_stc_go;

int Sally_TeteChercheuses_stc_go()
{
    double v0; // st7
    tScriptStruct_SallyTeteChercheuses* v2; // [esp+8h] [ebp-2F0h]
    tGameObjectMain* v3; // [esp+10h] [ebp-2E8h]
    tGameObjectMain* v4; // [esp+18h] [ebp-2E0h]
    tGameObjectMain* v5; // [esp+20h] [ebp-2D8h]
    tGameObjectMain* v6; // [esp+24h] [ebp-2D4h]
    tGameObjectMain* v7; // [esp+2Ch] [ebp-2CCh]
    tGameObjectMain* v8; // [esp+34h] [ebp-2C4h]
    tGameObjectMain* v9; // [esp+38h] [ebp-2C0h]
    tGameObjectMain* v10; // [esp+3Ch] [ebp-2BCh]
    tGameObjectMain* v11; // [esp+54h] [ebp-2A4h]
    tGameObjectMain* v12; // [esp+78h] [ebp-280h]
    tGameObjectMain* v13; // [esp+7Ch] [ebp-27Ch]
    tGameObjectMain* v14; // [esp+80h] [ebp-278h]
    tGameObjectMain* v15; // [esp+88h] [ebp-270h]
    tGameObjectMain* v16; // [esp+8Ch] [ebp-26Ch]
    float v17; // [esp+90h] [ebp-268h]
    Vec3 v18; // [esp+94h] [ebp-264h] BYREF
    Vec3 v19; // [esp+A0h] [ebp-258h] BYREF
    tGameObjectMain* v20; // [esp+ACh] [ebp-24Ch]
    Vec3 v; // [esp+BCh] [ebp-23Ch] BYREF
    float v22; // [esp+C8h] [ebp-230h]
    tMessage Src; // [esp+CCh] [ebp-22Ch] BYREF
    Vec3 v24; // [esp+100h] [ebp-1F8h] BYREF
    tGameObjectMain* v25; // [esp+10Ch] [ebp-1ECh]
    Vec3 v26; // [esp+110h] [ebp-1E8h] BYREF
    float v27; // [esp+12Ch] [ebp-1CCh]
    Vec3 v28; // [esp+130h] [ebp-1C8h] BYREF
    Vec3 v31; // [esp+144h] [ebp-1B4h] BYREF
    Vec3 v32; // [esp+154h] [ebp-1A4h] BYREF
    float v34; // [esp+168h] [ebp-190h]
    Vec3 v35; // [esp+16Ch] [ebp-18Ch] BYREF
    Vec3 v36; // [esp+178h] [ebp-180h] BYREF
    Vec3 v37; // [esp+184h] [ebp-174h] BYREF
    tGameObjectMain* a7[31]; // [esp+190h] [ebp-168h] BYREF
    Vec3 v39; // [esp+20Ch] [ebp-ECh] BYREF
    Vec3 a3; // [esp+218h] [ebp-E0h] BYREF
    int v42; // [esp+22Ch] [ebp-CCh]
    int v43; // [esp+230h] [ebp-C8h]
    Vec3 v44; // [esp+234h] [ebp-C4h] BYREF
    Vec3 v45; // [esp+240h] [ebp-B8h] BYREF
    int v46; // [esp+24Ch] [ebp-ACh]
    Vec3 v47; // [esp+250h] [ebp-A8h] BYREF
    Vec3 v48; // [esp+25Ch] [ebp-9Ch] BYREF
    int v49; // [esp+274h] [ebp-84h]
    Vec3 v50; // [esp+278h] [ebp-80h] BYREF
    Vec3 v51; // [esp+284h] [ebp-74h] BYREF
    int v52; // [esp+290h] [ebp-68h]
    tScriptStruct_SallyTeteChercheuses* v53; // [esp+294h] [ebp-64h]
    float v54; // [esp+298h] [ebp-60h]
    Vec3 v55; // [esp+29Ch] [ebp-5Ch] BYREF
    Vec3 v56; // [esp+2A8h] [ebp-50h] BYREF
    int v57; // [esp+2C0h] [ebp-38h]
    float v58; // [esp+2C4h] [ebp-34h]
    tGameObjectMain* v59; // [esp+2C8h] [ebp-30h]
    Vec3 v60; // [esp+2CCh] [ebp-2Ch] BYREF
    Vec3 dest; // [esp+2D8h] [ebp-20h] BYREF
    Vec3 v62; // [esp+2E4h] [ebp-14h] BYREF
    float v63; // [esp+2F0h] [ebp-8h]
    int v64; // [esp+2F4h] [ebp-4h]

    v.x = 1.0;
    v.y = 0.0;
    v.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SallyTeteChercheuses*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v53 = v2;
    v42 = 0;
    v64 = 0;
    v59 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Sally_TeteChercheuses_stc_go_GST_Fight_Manager_gao = AI_ul_RealResolveOneRefObject(0x9E002FFA);
        AI2C_ResolveBuffer(
            &Sally_TeteChercheuses_stc_go_GST_Fight_Manager_gao_ao_Evil_Actor,
            (void*)Sally_TeteChercheuses_stc_go_GST_Fight_Manager_gao,
            520);
        fct_Sally_TeteChercheuses_stc_go_Sally_TeteChercheuses_stc_end = (int)AI_ul_RealResolveOneRefFunction(0x26002A39u);
        fct_Sally_TeteChercheuses_stc_go_Sally_TeteChercheuses_stc_go = (int)AI_ul_RealResolveOneRefFunction(0x26002A2Au);
        Sally_TeteChercheuses_stc_go_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        Sally_TeteChercheuses_stc_go_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Sally_TeteChercheuses_stc_go_univVars_Perso_ID_actor,
            (void*)Sally_TeteChercheuses_stc_go_univVars,
            988);
    }
    if (v53->i_init)
    {
        v53->i_init = 0;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v28);
        v50.x = v28.x * 15.0f;
        v50.y = v28.y * 15.0f;
        v50.z = v28.z * 15.0f;
        v46 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v50);
        v22 = TIM_gf_MainClock - TIM_gf_LockTime;
        v53->f_time_colset = v22;
        if (v53->i_stop_orientation)
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v19);
            v37.x = v19.x * 40.0f;
            v37.y = v19.y * 40.0f;
            v37.z = v19.z * 40.0f;
            AI_EvalFunc_DYNTractionSet_C_CURRENT(&v37);
        }
    }
    v58 = TIM_gf_dt;
    v17 = 10.0f * TIM_gf_dt;
    if (v17 < 0.0)
        v17 = 0.0;
    if (v17 > 1.0)
        v17 = 1.0;
    v27 = 2.0f * v17 + v53->haloG_size * (1.0f - v17);
    v53->haloG_size = v27;
    GFXSetCurGlobal(v53->haloG);
    AI_EvalFunc_GFXSetf_C_CURGLOB(21000, v53->haloG_size);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v45);
    AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v45);
    v42 = 0;
    v16 = (tGameObjectMain*)v53->o_FM;
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
    v57 = AI_EvalFunc_ARRObjSearch_C(
        (tGameObjectMain**)(Sally_TeteChercheuses_stc_go_GST_Fight_Manager_gao_ao_Evil_Actor + 4),
        30,
        v15);
    if (v57 == -1)
        v53->o_target = 0;
    if (AI_EvalFunc_TIMEElapsed_C(v53->f_time_colset, 4.0))
        v53->o_target = 0;
    v14 = (tGameObjectMain*)v53->o_target;
    if (v53->o_target)
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
        v12 = (tGameObjectMain*)v53->o_target;
        if (v53->o_target)
        {
            if ((unsigned int)v12 <= 2)
                v12 = AI_MainActor[(uint32_t)v12 - 1];
            AI_EvalFunc_OBJGetPos_C(v12, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v24);
        MATH_SubstractVec(&dest, &a3, &v24);
        v = dest;
        if (!v53->i_stop_orientation && !AI2C_VecEqNull(&v))
        {
            MATH_NormalizeEqualVector(&v);
            if (AI_EvalFunc_TIMEElapsed_C(v53->f_time_colset, 1.5))
            {
                v11 = (tGameObjectMain*)v53->o_target;
                if (v53->o_target)
                {
                    if ((unsigned int)v11 <= 2)
                        v11 = AI_MainActor[(uint32_t)v11 - 1];
                    v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v11);
                }
                else
                {
                    v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
                }
                v34 = (float)v0;
                if (v34 < 4.0)
                    v53->i_stop_orientation = 1;
            }
            AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v18);
            if (!AI2C_VecEqNull(&v18))
            {
                AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v47);
                AI_EvalFunc_MATHVecNormalize_C(&v47, &v39);
                v63 = v.z * v39.z + v.y * v39.y + v39.x * v.x;
                if (v63 > 0.96592599)
                    v53->i_stop_orientation = 1;
            }
            v44.x = v.x * 40.0f;
            v44.y = v.y * 40.0f;
            v44.z = v.z * 40.0f;
            AI_EvalFunc_DYNTractionSet_C_CURRENT(&v44);
        }
        v64 = AI_EvalFunc_COLZDEZDEListGet_C(AI_gpst_CurrentGameObject, a7, 6, 1, 2, 0, 1);
        if (v64)
        {
            AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v31);
            v48.x = -v31.x;
            v48.y = -v31.y;
            v48.z = -v31.z;
            AI_EvalFunc_MATHVecNormalize_C(&v48, &v53->v_normal);
            v53->o_perso = (int)a7[0];
            v42 = 1;
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v55);
            AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v35);
            v54 = TIM_gf_dt;
            v36.x = v35.x * TIM_gf_dt;
            v36.y = v35.y * TIM_gf_dt;
            v36.z = v35.z * TIM_gf_dt;
            v59 = AI_EvalFunc_COLRayObjectVector_C(AI_gpst_CurrentGameObject, &v55, &v36, -1, 0, 0, 1);
            v10 = v59;
            if (v59)
            {
                if ((unsigned int)v59 <= 2)
                    v10 = AI_MainActor[(uint32_t)v59 - 1];
                v9 = v10;
            }
            else
            {
                v9 = 0;
            }
            if (v9)
            {
                AI_EvalFunc_COLRayNormalGet_C(AI_gpst_CurrentGameObject, &v53->v_normal);
                v53->o_perso = (int)v59;
                v42 = 1;
            }
        }
    }
    else
    {
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        AI_EvalFunc_OBJDrawMaskSet_C(AI_gpst_CurrentGameObject, 0, 8);
        v26.x = 0.0;
        v26.y = 0.0;
        v26.z = 0.0;
        AI_EvalFunc_DYNTractionSet_C_CURRENT(&v26);
        v62.x = 0.0;
        v62.y = 0.0;
        v62.z = 0.0;
        v49 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v62);
        AI_EvalFunc_AITrackChange_C(2, (tAIFunction*)fct_Sally_TeteChercheuses_stc_go_Sally_TeteChercheuses_stc_end);
        v20 = AI_gpst_CurrentGameObject;
        v8 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v8 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                v8,
                0,
                (tAIFunction*)fct_Sally_TeteChercheuses_stc_go_Sally_TeteChercheuses_stc_go);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                0,
                0,
                (tAIFunction*)fct_Sally_TeteChercheuses_stc_go_Sally_TeteChercheuses_stc_go);
        }
    }
    if (v42)
    {
        v53->GFX_spark = AI_EvalFunc_GFXAdd_C_CURRENT(9);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v53->GFX_spark, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v53->GFX_spark, 2, 1);
        v7 = (tGameObjectMain*)Sally_TeteChercheuses_stc_go_SFX_light_smoke_gao;
        if (Sally_TeteChercheuses_stc_go_SFX_light_smoke_gao)
        {
            if ((unsigned int)Sally_TeteChercheuses_stc_go_SFX_light_smoke_gao <= 2)
                v7 = AI_MainActor[Sally_TeteChercheuses_stc_go_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v53->GFX_spark, v7, 5);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v53->GFX_spark, 0, 5);
        }
        GFXSetCurGlobal(v53->GFX_spark);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9000, 1.570796f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9003, 0.89999998f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9004, 0.2f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9005, 0.40000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9006, 0.80000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9007, 2.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9008, 0.0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9100, 10);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9102, 8);
        v51.x = 0.0;
        v51.y = 0.0;
        v51.z = -1.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(9202, &v51);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9101, 0xFFC280BE);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9001, 8.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9002, 15.0);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v56);
        AI_EvalFunc_GFXSetv_C_CURGLOB(9200, &v56);
        AI_EvalFunc_GFXSetv_C_CURGLOB(9201, &v53->v_normal);
        v6 = (tGameObjectMain*)v53->o_perso;
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
        v43 = AI_EvalFunc_ARRObjSearch_C(
            (tGameObjectMain**)(Sally_TeteChercheuses_stc_go_univVars_Perso_ID_actor + 4),
            12,
            v5);
        if (v43 == -1)
        {
            Src.msgID = 37;
            Src.gao = AI_gpst_CurrentGameObject;
            Src.gao2 = 0;
            Src.Int1 = 4;
            Src.Int2 = 0;
            AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&Src.v1);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&Src.v2);
            Src.f2 = 20.0;
            Src.f1 = 1000.0;
            v4 = (tGameObjectMain*)v53->o_perso;
            if (v4)
            {
                if ((unsigned int)v4 <= 2)
                    v4 = AI_MainActor[(uint32_t)v4 - 1];
                AI_EvalFunc_MSGSend_C(v4, &Src);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &Src);
            }
        }
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        AI_EvalFunc_OBJDrawMaskSet_C(AI_gpst_CurrentGameObject, 0, 8);
        v32.x = 0.0;
        v32.y = 0.0;
        v32.z = 0.0;
        AI_EvalFunc_DYNTractionSet_C_CURRENT(&v32);
        v60.x = 0.0;
        v60.y = 0.0;
        v60.z = 0.0;
        v52 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v60);
        AI_EvalFunc_AITrackChange_C(2, (tAIFunction*)fct_Sally_TeteChercheuses_stc_go_Sally_TeteChercheuses_stc_end);
        v25 = AI_gpst_CurrentGameObject;
        v3 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v3 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                v3,
                0,
                (tAIFunction*)fct_Sally_TeteChercheuses_stc_go_Sally_TeteChercheuses_stc_go);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                0,
                0,
                (tAIFunction*)fct_Sally_TeteChercheuses_stc_go_Sally_TeteChercheuses_stc_go);
        }
    }
    return 1000;
}

int Sally_TeteChercheuses_stc_loop()
{
    int result; // eax
    float v1; // [esp+8h] [ebp-F0h]
    tScriptStruct_SallyTeteChercheuses* v2; // [esp+Ch] [ebp-ECh]
    float v3; // [esp+10h] [ebp-E8h]
    float v4; // [esp+1Ch] [ebp-DCh]
    float v5; // [esp+20h] [ebp-D8h]
    Vec3 v6; // [esp+24h] [ebp-D4h] BYREF
    Vec3 v7; // [esp+30h] [ebp-C8h] BYREF
    float v8; // [esp+3Ch] [ebp-BCh]
    Vec3 v9; // [esp+40h] [ebp-B8h] BYREF
    Vec3 v10; // [esp+4Ch] [ebp-ACh] BYREF
    float v11; // [esp+64h] [ebp-94h]
    float v12; // [esp+68h] [ebp-90h]
    Vec3 v13; // [esp+6Ch] [ebp-8Ch] BYREF
    Vec3 v14; // [esp+78h] [ebp-80h] BYREF
    Vec3 a4; // [esp+84h] [ebp-74h] BYREF
    Vec3 a2; // [esp+90h] [ebp-68h] BYREF
    float v17; // [esp+9Ch] [ebp-5Ch]
    BOOL v18; // [esp+A0h] [ebp-58h]
    Vec3 v19; // [esp+A4h] [ebp-54h] BYREF
    Vec3 a1; // [esp+B0h] [ebp-48h] BYREF
    Vec3 v21; // [esp+BCh] [ebp-3Ch] BYREF
    float v22; // [esp+C8h] [ebp-30h]
    tScriptStruct_SallyTeteChercheuses* v23; // [esp+CCh] [ebp-2Ch]
    Vec3 v24; // [esp+D0h] [ebp-28h] BYREF
    Vec3 a3; // [esp+DCh] [ebp-1Ch] BYREF
    Vec3 v26; // [esp+E8h] [ebp-10h] BYREF
    int v27; // [esp+F4h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SallyTeteChercheuses*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v23 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Sally_TeteChercheuses_stc_loop__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        fct_Sally_TeteChercheuses_stc_loop_Sally_TeteChercheuses_stc_loop_end = (int)AI_ul_RealResolveOneRefFunction(0x26002A44u);
    }
    v11 = TIM_gf_dt;
    v4 = 5.0f * TIM_gf_dt;
    if (v4 < 0.0)
        v4 = 0.0;
    if (v4 > 1.0)
        v4 = 1.0;
    v22 = 1.0f * v4 + v23->haloG_size * (1.0f - v4);
    v23->haloG_size = v22;
    GFXSetCurGlobal(v23->haloG);
    AI_EvalFunc_GFXSetf_C_CURGLOB(21000, v23->haloG_size);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v19);
    AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v19);
    v8 = TIM_gf_dt;
    v1 = (float)v23->i_rot_sens;
    v5 = v23->f_rot_speed * TIM_gf_dt * v1;
    v7.x = 0.0;
    v7.y = 0.0;
    v7.z = 1.0;
    AI_EvalFunc_MATHVecRotate_C(&v23->v_rot_vector, &v7, v5, &a4);
    v23->v_rot_vector = a4;
    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Sally_TeteChercheuses_stc_loop__PJ_Sally_gao, &a3);
    v10.x = 0.0;
    v10.y = 0.0;
    v10.z = 1.3f;
    MATH_VecAddVec(&a1, &a3, &v10);
    v23->f_angle_cercle = v23->f_rot_speedREF + v23->f_angle_cercle;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v24);
    AI_EvalFunc_MATHVecRotate_C(&v23->v_rot_vector, &v24, v23->f_angle_cercle, &v23->v_rot_vectorRef);
    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Sally_TeteChercheuses_stc_loop__PJ_Sally_gao, &a2);
    v9.x = 0.0;
    v9.y = 0.0;
    v9.z = 1.3f;
    MATH_VecAddVec(&v26, &a2, &v9);
    v17 = TIM_gf_dt;
    v3 = 12.0f * TIM_gf_dt;
    if (v3 < 0.0)
        v3 = 0.0;
    if (v3 > 1.0)
        v3 = 1.0;
    v12 = v23->f_rayon_length_max * v3 + v23->f_rayon_length * (1.0f - v3);
    v23->f_rayon_length = v12;
    AI_EvalFunc_MATHVecSetNormVector_C(&v23->v_rot_vectorRef, v23->f_rayon_length);
    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Sally_TeteChercheuses_stc_loop__PJ_Sally_gao, &v14);
    v6.x = 0.0;
    v6.y = 0.0;
    v6.z = 1.3f;
    MATH_VecAddVec(&v13, &v14, &v6);
    MATH_VecAddVec(&v21, &v13, &v23->v_rot_vectorRef);
    AI_EvalFunc_OBJSetPos_C_CURRENT(&v21);
    v18 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100);
    if (!v18
        || (v27 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Sally_TeteChercheuses_stc_loop_Sally_TeteChercheuses_stc_loop_end),
            v27 == 1000))
    {
        result = 1000;
    }
    else
    {
        result = v27;
    }
    return result;
}

int Sally_TeteChercheuses_stc_end()
{
    int result; // eax
    tScriptStruct_SallyTeteChercheuses* v1; // [esp+4h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SallyTeteChercheuses*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (*AI_gppst_CurrentJumpNode)
    {
        GFXSetCurGlobal(v1->haloG);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21005, 1.0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21100, 1);
        *AI_gppst_CurrentJumpNode = 0;
        TIME_Wait_C(1.0);
        result = 0;
    }
    else
    {
        result = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (result == 1000)
            result = 1000;
    }
    return result;
}

int Sally_TeteChercheuses_stc_init()
{
    tScriptStruct_SallyTeteChercheuses* v1; // [esp+0h] [ebp-A8h]
    tGameObjectMain* v2; // [esp+8h] [ebp-A0h]
    tGameObjectMain* v3; // [esp+Ch] [ebp-9Ch]
    tGameObjectMain* v4; // [esp+10h] [ebp-98h]
    tGameObjectMain* v5; // [esp+18h] [ebp-90h]
    tGameObjectMain* v6; // [esp+1Ch] [ebp-8Ch]
    tGameObjectMain* v7; // [esp+20h] [ebp-88h]
    int v8; // [esp+3Ch] [ebp-6Ch]
    void* Src; // [esp+40h] [ebp-68h]
    tMessage v10; // [esp+60h] [ebp-48h] BYREF
    tScriptStruct_SallyTeteChercheuses* v11; // [esp+98h] [ebp-10h]
    int v12; // [esp+A4h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SallyTeteChercheuses*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v11 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Sally_TeteChercheuses_stc_init_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        Sally_TeteChercheuses_stc_init__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        fct_Sally_TeteChercheuses_stc_init_Sally_TeteChercheuses_stc_loop = (int)AI_ul_RealResolveOneRefFunction(0x26002A42u);
        fct_Sally_TeteChercheuses_stc_init_Sally_TeteChercheuses_stc_go = (int)AI_ul_RealResolveOneRefFunction(0x26002A2Au);
    }
    Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v10, Src, sizeof(v10));
    v11->o_FM = (int)v10.gao;
    v11->o_target = (int)v10.gao2;
    v7 = (tGameObjectMain*)v11->o_target;
    if (v11->o_target)
    {
        if ((unsigned int)v7 <= 2)
            v7 = AI_MainActor[(uint32_t)v7 - 1];
        v6 = v7;
    }
    else
    {
        v6 = 0;
    }
    if (!v6)
        v11->o_target = v11->o_FM;
    if (v10.Int1 == 666)
        v11->i_stop_orientation = 1;
    v11->haloG = AI_EvalFunc_GFXAdd_C_CURRENT(21);
    AI_EvalFunc_GFXFlagSet_C_CURRENT(v11->haloG, 4, 1);
    v5 = (tGameObjectMain*)Sally_TeteChercheuses_stc_init_SFX_light_smoke_gao;
    if (Sally_TeteChercheuses_stc_init_SFX_light_smoke_gao)
    {
        if ((unsigned int)Sally_TeteChercheuses_stc_init_SFX_light_smoke_gao <= 2)
            v5 = AI_MainActor[Sally_TeteChercheuses_stc_init_SFX_light_smoke_gao - 1];
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v11->haloG, v5, 0);
    }
    else
    {
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v11->haloG, 0, 0);
    }
    GFXSetCurGlobal(v11->haloG);
    AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.80000001f);
    AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
    AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 0xFFC280BE);
    v11->haloG_size = 0.0;
    XMEN_OnOff(AI_gpst_CurrentGameObject, 1, 1);
    XMEN_SetSize(AI_gpst_CurrentGameObject, 1, -1, 2.0);
    v4 = (tGameObjectMain*)v11->o_FM;
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
    if (v3 == (tGameObjectMain*)Sally_TeteChercheuses_stc_init__PJ_Sally_gao)
    {
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v11->v_rot_vector);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v11->v_rot_vectorRef);
        v11->f_rot_speed = AI_EvalFunc_MATHRandFloat_C(4.0, 7.0);
        v11->f_rot_speed = 5.0;
        v11->f_rayon_length_max = AI_EvalFunc_MATHRandFloat_C(1.2f, 1.5f);
        v11->i_rot_sens = AI_EvalFunc_MATHRandInt_C(-1, 2);
        if (!v11->i_rot_sens)
            v11->i_rot_sens = AI_EvalFunc_MATHRandInt_C(-1, 2);
        if (!v11->i_rot_sens)
            v11->i_rot_sens = -1;
        v11->i_rot_sens = -1;
        v11->f_rot_speedREF = 0.059999999f;
        v11->i_rot_sensREF = AI_EvalFunc_MATHRandInt_C(-1, 2);
        if (!v11->i_rot_sensREF)
            v11->i_rot_sensREF = AI_EvalFunc_MATHRandInt_C(-1, 2);
        if (!v11->i_rot_sensREF)
            v11->i_rot_sensREF = -1;
        v11->i_rot_sensREF = -1;
        v11->f_rayon_length = 0.0099999998f;
        v8 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Sally_TeteChercheuses_stc_init_Sally_TeteChercheuses_stc_loop);
        if (v8 != 1000)
            return v8;
    }
    else
    {
        AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
        AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 4, 0);
        v12 = AI_EvalFunc_DYNFrictionSet_C(AI_gpst_CurrentGameObject, 3.0);
        v11->i_init = 1;
        v2 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v2 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v2,
                0,
                (tAIFunction*)fct_Sally_TeteChercheuses_stc_init_Sally_TeteChercheuses_stc_go);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                0,
                (tAIFunction*)fct_Sally_TeteChercheuses_stc_init_Sally_TeteChercheuses_stc_go);
        }
        AI_EvalFunc_AITrackCurStop_C();
    }
    return 1000;
}

int Sally_TeteChercheuses_stc_loop_end()
{
    tScriptStruct_SallyTeteChercheuses* v1; // [esp+0h] [ebp-40h]
    float v2; // [esp+8h] [ebp-38h]
    Vec3 v3; // [esp+Ch] [ebp-34h] BYREF
    Vec3 a2; // [esp+18h] [ebp-28h] BYREF
    Vec3 a1; // [esp+24h] [ebp-1Ch] BYREF
    Vec3 a3; // [esp+30h] [ebp-10h] BYREF
    tScriptStruct_SallyTeteChercheuses* v7; // [esp+3Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SallyTeteChercheuses*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v7 = v1;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
    v2 = 1.0 / 2.0;
    a3.x = v2 * v7->v_rot_vectorRef.x;
    a3.y = v7->v_rot_vectorRef.y * v2;
    a3.z = v7->v_rot_vectorRef.z * v2;
    MATH_VecAddVec(&a1, &a2, &a3);
    AI_EvalFunc_OBJSetPos_C_CURRENT(&a1);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v3);
    AI_EvalFunc_GFXSetv_C_CURRENT(v7->haloG, 21200, &v3);
    return 1000;
}