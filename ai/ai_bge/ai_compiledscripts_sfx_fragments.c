#include "ai_compiledscripts_inc.h"

//SFX_Fragments_SFX_frag_init
int fct_SFX_Fragments_SFX_frag_init_SFX_Fragments_SFX_frag_comportement;
int fct_SFX_Fragments_SFX_frag_init_SFX_Fragments_SFX_frag_disparition;

//SFX_Fragments_SFX_frag_comportement
int fct_SFX_Fragments_SFX_frag_comportement_SFX_Fragments_SFX_frag_disparition;
int SFX_Fragments_SFX_frag_comportement_SFX_Fragments_SFX_explosion_gao;

int SFX_Fragments_SFX_frag_disparition()
{
    int v1; // eax
    float angle; // [esp+4h] [ebp-60h]
    float anglea; // [esp+4h] [ebp-60h]
    float angleb; // [esp+4h] [ebp-60h]
    tScriptStruct_SFXFragments* v5; // [esp+Ch] [ebp-58h]
    tGameObjectMain* v6; // [esp+14h] [ebp-50h]
    tGameObjectMain* v7; // [esp+18h] [ebp-4Ch]
    tGameObjectMain* v8; // [esp+1Ch] [ebp-48h]
    float v9; // [esp+24h] [ebp-40h]
    float v10; // [esp+28h] [ebp-3Ch]
    float zoom; // [esp+2Ch] [ebp-38h]
    Vec3 a2; // [esp+34h] [ebp-30h] BYREF
    float v14; // [esp+44h] [ebp-20h]
    float v15; // [esp+48h] [ebp-1Ch]
    int v16; // [esp+4Ch] [ebp-18h]
    float v17; // [esp+50h] [ebp-14h]
    float v18; // [esp+54h] [ebp-10h]
    tScriptStruct_SFXFragments* v19; // [esp+58h] [ebp-Ch]
    float v20; // [esp+5Ch] [ebp-8h]
    float v21; // [esp+60h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_SFXFragments*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v19 = v5;
    if (*AI_gppst_CurrentJumpNode == (tAINode*)6)
    {
        v1 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        v16 = v1;
        if (v1 != 1000)
            return v16;
    }
    else
    {
        v14 = TIM_gf_dt;
        v10 = 0.25f * TIM_gf_dt;
        if (v10 < 0.0)
            v10 = 0.0;
        if (v10 > 1.0)
            v10 = 1.0;
        v18 = 0.0f * v10 + v19->f_rotation_speed * (1.0f - v10);
        v19->f_rotation_speed = v18;
        if (v19->i_rotate_axis == 3)
        {
            v17 = TIM_gf_dt;
            v9 = TIM_gf_dt * 30.0f;
            a2.x = v9 * v19->v_axe.x;
            a2.y = v19->v_axe.y * v9;
            a2.z = v19->v_axe.z * v9;
            AI_EvalFunc_OBJRotateGlobal_C(AI_gpst_CurrentGameObject, &a2);
        }
        else if (v19->i_rotate_axis == 2)
        {
            angle = -v19->f_rotation_speed * TIM_gf_dt;
            AI_EvalFunc_OBJRotateLocalZ_c(AI_gpst_CurrentGameObject, angle);
        }
        else if (v19->i_rotate_axis)
        {
            v15 = TIM_gf_dt;
            anglea = -v19->f_rotation_speed * TIM_gf_dt;
            AI_EvalFunc_OBJRotateLocalY_C(AI_gpst_CurrentGameObject, anglea);
        }
        else
        {
            v20 = TIM_gf_dt;
            angleb = -v19->f_rotation_speed * TIM_gf_dt;
            AI_EvalFunc_OBJRotateLocalX_C(AI_gpst_CurrentGameObject, angleb);
        }
        v8 = (tGameObjectMain*)v19->o_MDF;
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
        if (v7)
        {
            v6 = (tGameObjectMain*)v19->o_MDF;
            if (v6)
            {
                if ((unsigned int)v6 <= 2)
                    v6 = AI_MainActor[(uint32_t)v6 - 1];
                AI_EvalFunc_SNDStopModifier_C(v6, -1);
            }
            else
            {
                AI_EvalFunc_SNDStopModifier_C(0, -1);
            }
        }
        if (AI_EvalFunc_TIMEElapsed_C(v19->f_time_start, v19->f_speed_zoom))
        {
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x100, 0);
            *AI_gppst_CurrentJumpNode = (tAINode*)6;
            TIME_Wait_C(0.001f);
            return 0;
        }
        v21 = TIM_gf_MainClock - TIM_gf_LockTime;
        zoom = v19->f_zoom_init * (1.0f - (v21 - v19->f_time_start) / v19->f_speed_zoom);
        AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, zoom);
    }
    return 1000;
}

int SFX_Fragments_SFX_frag_init()
{
    double v1; // st7
    double v2; // st7
    tScriptStruct_SFXFragments* v3; // [esp+Ch] [ebp-188h]
    tGameObjectMain* v4; // [esp+14h] [ebp-180h]
    tGameObjectMain* v5; // [esp+18h] [ebp-17Ch]
    tGameObjectMain* v6; // [esp+1Ch] [ebp-178h]
    float v7; // [esp+5Ch] [ebp-138h]
    float v8; // [esp+60h] [ebp-134h]
    Vec3 v9; // [esp+68h] [ebp-12Ch] BYREF
    tMessage v10; // [esp+80h] [ebp-114h] BYREF
    int v11; // [esp+B4h] [ebp-E0h]
    int v12; // [esp+BCh] [ebp-D8h]
    Vec3 v13; // [esp+C0h] [ebp-D4h] BYREF
    Vec3 v14; // [esp+CCh] [ebp-C8h] BYREF
    void* v16; // [esp+DCh] [ebp-B8h]
    Vec3 v17; // [esp+E0h] [ebp-B4h] BYREF
    int v18; // [esp+ECh] [ebp-A8h]
    float v19; // [esp+F4h] [ebp-A0h]
    Vec3 v20; // [esp+F8h] [ebp-9Ch] BYREF
    int v21; // [esp+104h] [ebp-90h]
    void* Src; // [esp+108h] [ebp-8Ch]
    Vec3 v24; // [esp+110h] [ebp-84h] BYREF
    unsigned int v25; // [esp+11Ch] [ebp-78h]
    BOOL v26; // [esp+124h] [ebp-70h]
    Vec3 v27; // [esp+128h] [ebp-6Ch] BYREF
    Vec3 a4; // [esp+134h] [ebp-60h] BYREF
    Vec3 a2; // [esp+144h] [ebp-50h] BYREF
    float v30; // [esp+150h] [ebp-44h]
    Vec3 v31; // [esp+154h] [ebp-40h] BYREF
    Vec3 v32; // [esp+160h] [ebp-34h] BYREF
    tScriptStruct_SFXFragments* v33; // [esp+16Ch] [ebp-28h]
    Vec3 v34; // [esp+170h] [ebp-24h] BYREF
    void* v35; // [esp+17Ch] [ebp-18h]
    void* v36; // [esp+180h] [ebp-14h]
    int v37; // [esp+184h] [ebp-10h]
    int v38; // [esp+188h] [ebp-Ch]
    float a3; // [esp+18Ch] [ebp-8h]
    int v40; // [esp+190h] [ebp-4h]

    v13.x = 1.0;
    v13.y = 0.0;
    v13.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_SFXFragments*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v33 = v3;
    v21 = 0;
    v40 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_Fragments_SFX_frag_init_SFX_Fragments_SFX_frag_comportement = (int)AI_ul_RealResolveOneRefFunction(0x72000AFCu);
        fct_SFX_Fragments_SFX_frag_init_SFX_Fragments_SFX_frag_disparition = (int)AI_ul_RealResolveOneRefFunction(0x72000CB9u);
    }
    if (*AI_gppst_CurrentJumpNode && v33->f_delai_init != 0.0)
    {
        *AI_gppst_CurrentJumpNode = 0;
        TIME_Wait_C(v33->f_delai_init);
        return 0;
    }
    if (!AI_EvalFunc_MTHVecNullEpsilon_C(&v33->v_direction))
    {
        v31.x = 0.0;
        v31.y = 0.0;
        v31.z = 1.0;
        AI_EvalFunc_MATHVecNormalize_C(&v33->v_direction, &v27);
        AI_EvalFunc_MATHVecCrossProduct_C(&v31, &v27, &v33->v_axe);
    }
    if (v33->des_paf_sensitive)
    {
        AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0, 0x1000000);
        v21 = 0;
        Src = AI_EvalFunc_MSGGetByID_C(AI_gpst_CurrentGameObject, 37);
        memcpy(&v10, Src, sizeof(v10));
        while (v10.msgID)
        {
            v21 = 1;
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
            v35 = AI_EvalFunc_MSGGetByID_C(AI_gpst_CurrentGameObject, 37);
            memcpy(&v10, v35, sizeof(v10));
        }
        if (!v21)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 4;
        }
    }
    v33->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
    v1 = AI_EvalFunc_MATHRandFloat_C(v33->f_init_rot_speed_min, v33->f_init_rot_speed_max);
    v33->f_init_rotation_speed = (float)v1;
    v33->f_init_rotation_speed = v33->f_init_rotation_speed / 2.0f;
    v33->f_rotation_speed = v33->f_init_rotation_speed;
    v33->f_speed_zoom = 0.5;
    v40 = 1;
    v36 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v10, v36, sizeof(v10));
    while (v10.msgID)
    {
        switch (v10.msgID)
        {
        case 10:
            v33->i_life_time = v10.Int1;
            v33->i_nb_max_rebonds = v10.Int2;
            v33->f_rebound = v10.f2;
            v33->f_angle = v10.f1;
            v33->v_direction = v10.v1;
            break;
        case 11:
            v33->f_speed_min = v10.f2;
            v33->f_speed_max = v10.f1;
            v33->v_friction = v10.v1;
            v33->v_gravity = v10.v2;
            break;
        case 12:
            v33->f_speed_zoom = v10.f2;
            v40 = 0;
            break;
        }
        v16 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v10, v16, sizeof(v10));
    }
    v26 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
    v11 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x1029004, 0);
    AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v33->v_gravity);
    v38 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v33->v_friction);
    AI_EvalFunc_DYNReboundSet_C(AI_gpst_CurrentGameObject, v33->f_rebound);
    AI_EvalFunc_COLPrioritySet_C(AI_gpst_CurrentGameObject, -1);
    AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, 1);
    v25 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x1000000, 0);
    if (v33->direction_mode)
    {
        AI_EvalFunc_MATHVecLocalToGlobal_C(AI_gpst_CurrentGameObject, &v33->v_direction, &v14);
        v33->v_direction = v14;
    }
    v33->f_angle = v33->f_angle * 0.017453f;
    v33->f_angle = v33->f_angle / 2.0f;
    MATH_NormalizeEqualVector(&v33->v_direction);
    v7 = AI_EvalFunc_MATHRandFloat_C(v33->f_speed_min, v33->f_speed_max);
    v8 = AI_EvalFunc_MATHRandFloat_C(-1.0, 1.0);
    v19 = AI_EvalFunc_MATHRandFloat_C(-1.0, 1.0);
    v30 = AI_EvalFunc_MATHRandFloat_C(-1.0, 1.0);
    v24.x = v8;
    v24.y = v19;
    v24.z = v30;
    a3 = AI_EvalFunc_MATHRandFloat_C(0.0, v33->f_angle);
    AI_EvalFunc_MATHVecRotate_C(&v33->v_direction, &v24, a3, &a4);
    v13 = a4;
    v17.x = a4.x * v7;
    v17.y = a4.y * v7;
    v17.z = a4.z * v7;
    v18 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v17);
    AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&a2);
    v9.x = 0.0;
    v9.y = 0.0;
    v9.z = 1.0;
    AI_EvalFunc_MATHVecCrossProduct_C(&a2, &v9, &v34);
    v13 = v34;
    if (v33->des_i_oriente_a_linit)
    {
        if (AI_EvalFunc_MTHVecNullEpsilon_C(&v13))
        {
            v32.x = 0.0;
            v32.y = 1.0;
            v32.z = 0.0;
            AI_EvalFunc_OBJ_BankingSet_C(AI_gpst_CurrentGameObject, &v32);
        }
        else
        {
            AI_EvalFunc_MATHVecNormalize_C(&v13, &v20);
            AI_EvalFunc_OBJ_BankingSet_C(AI_gpst_CurrentGameObject, &v20);
        }
    }
    v6 = (tGameObjectMain*)v33->o_MDF;
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
    if (v5)
    {
        v4 = (tGameObjectMain*)v33->o_MDF;
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            AI_EvalFunc_SNDPlayModifier_C(v4, -1);
        }
        else
        {
            AI_EvalFunc_SNDPlayModifier_C(0, -1);
        }
    }
    v2 = AI_EvalFunc_OBJ_ZoomGet_C(AI_gpst_CurrentGameObject);
    v33->f_zoom_init = (float)v2;
    v33->i_nb_rebonds = 0;
    if (v40)
    {
        v37 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_Fragments_SFX_frag_init_SFX_Fragments_SFX_frag_comportement);
        if (v37 != 1000)
            return v37;
    }
    else
    {
        v12 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_Fragments_SFX_frag_init_SFX_Fragments_SFX_frag_disparition);
        if (v12 != 1000)
            return v12;
    }
    return 1000;
}

int SFX_Fragments_SFX_frag_comportement()
{
    float angle; // [esp+4h] [ebp-184h]
    float anglea; // [esp+4h] [ebp-184h]
    float angleb; // [esp+4h] [ebp-184h]
    float anglec; // [esp+4h] [ebp-184h]
    float v5; // [esp+8h] [ebp-180h]
    float v6; // [esp+Ch] [ebp-17Ch]
    float v7; // [esp+10h] [ebp-178h]
    float v8; // [esp+14h] [ebp-174h]
    float v9; // [esp+1Ch] [ebp-16Ch]
    tScriptStruct_SFXFragments* v10; // [esp+20h] [ebp-168h]
    float v11; // [esp+24h] [ebp-164h]
    tGameObjectMain* v12; // [esp+28h] [ebp-160h]
    tGameObjectMain* v13; // [esp+2Ch] [ebp-15Ch]
    tAI* v14; // [esp+34h] [ebp-154h]
    tGameObjectMain* v15; // [esp+38h] [ebp-150h]
    tGameObjectMain* v16; // [esp+3Ch] [ebp-14Ch]
    tGameObjectMain* v17; // [esp+44h] [ebp-144h]
    tGameObjectMain* v18; // [esp+4Ch] [ebp-13Ch]
    float v19; // [esp+5Ch] [ebp-12Ch]
    float v20; // [esp+60h] [ebp-128h]
    Vec3 v21; // [esp+74h] [ebp-114h] BYREF
    float v22; // [esp+80h] [ebp-108h]
    float v23; // [esp+84h] [ebp-104h]
    Vec3 v24; // [esp+88h] [ebp-100h] BYREF
    int v25; // [esp+94h] [ebp-F4h]
    Vec3 a3; // [esp+98h] [ebp-F0h] BYREF
    float v27; // [esp+A8h] [ebp-E0h]
    Vec3 pos; // [esp+ACh] [ebp-DCh] BYREF
    tMessage Src; // [esp+BCh] [ebp-CCh] BYREF
    Vec3 v30; // [esp+F4h] [ebp-94h] BYREF
    float v31; // [esp+104h] [ebp-84h]
    float v32; // [esp+108h] [ebp-80h]
    int v33; // [esp+10Ch] [ebp-7Ch]
    Vec3 a2; // [esp+110h] [ebp-78h] BYREF
    int v35; // [esp+120h] [ebp-68h]
    int v36; // [esp+124h] [ebp-64h]
    float v38; // [esp+12Ch] [ebp-5Ch]
    float v39; // [esp+134h] [ebp-54h]
    int v40; // [esp+13Ch] [ebp-4Ch]
    tGameObjectMain* v41; // [esp+140h] [ebp-48h]
    tAI* v42; // [esp+148h] [ebp-40h]
    float v43; // [esp+14Ch] [ebp-3Ch]
    tScriptStruct_SFXFragments* v44; // [esp+150h] [ebp-38h]
    int v45; // [esp+158h] [ebp-30h]
    Vec3 v46; // [esp+15Ch] [ebp-2Ch] BYREF
    int v48; // [esp+16Ch] [ebp-1Ch]
    float v49; // [esp+170h] [ebp-18h]
    Vec3 v50; // [esp+174h] [ebp-14h] BYREF

    pos.x = 1.0;
    pos.y = 0.0;
    pos.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v10 = (tScriptStruct_SFXFragments*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v10 = 0;
    v44 = v10;
    v40 = 0;
    v41 = 0;
    v45 = 0;
    v39 = 0.0;
    v42 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_Fragments_SFX_frag_comportement_SFX_Fragments_SFX_frag_disparition = (int)AI_ul_RealResolveOneRefFunction(0x72000CB9u);
        SFX_Fragments_SFX_frag_comportement_SFX_Fragments_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
    }
    v27 = TIM_gf_dt;
    v20 = 1.25f * TIM_gf_dt;
    if (v20 < 0.0)
        v20 = 0.0;
    if (v20 > 1.0)
        v20 = 1.0;
    v43 = 0.0f * v20 + v44->f_rotation_speed * (1.0f - v20);
    v44->f_rotation_speed = v43;
    if (v44->i_rotate_axis == 3)
    {
        v38 = TIM_gf_dt;
        v19 = TIM_gf_dt * 30.0f;
        v21.x = v19 * v44->v_axe.x;
        v21.y = v44->v_axe.y * v19;
        v21.z = v44->v_axe.z * v19;
        AI_EvalFunc_OBJRotateGlobal_C(AI_gpst_CurrentGameObject, &v21);
    }
    else if (v44->i_rotate_axis == 2)
    {
        angle = -v44->f_rotation_speed * TIM_gf_dt;
        AI_EvalFunc_OBJRotateLocalZ_c(AI_gpst_CurrentGameObject, angle);
    }
    else if (v44->i_rotate_axis)
    {
        v31 = TIM_gf_dt;
        anglea = -v44->f_rotation_speed * TIM_gf_dt;
        AI_EvalFunc_OBJRotateLocalY_C(AI_gpst_CurrentGameObject, anglea);
    }
    else
    {
        v49 = TIM_gf_dt;
        angleb = -v44->f_rotation_speed * TIM_gf_dt;
        AI_EvalFunc_OBJRotateLocalX_C(AI_gpst_CurrentGameObject, angleb);
    }
    v25 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x800000, 0);
    if (AI_EvalFunc_TIMEElapsed_C(v44->f_time_start, 0.1f))
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 1, 0);
    v9 = (float)v44->i_life_time;
    if (AI_EvalFunc_TIMEElapsed_C(v44->f_time_start, v9))
    {
        v44->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
        v36 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_Fragments_SFX_frag_comportement_SFX_Fragments_SFX_frag_disparition);
        if (v36 != 1000)
            return v36;
    }
    v23 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
    if (v23 > 1.0)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v44->f_time_play_collision, 0.1f))
            v44->i_flag_play_collision = 1;
    }
    if (v44->des_detect_water)
    {
        v40 = AI_EvalFunc_COLGMatReportGet_C(AI_gpst_CurrentGameObject, 0x10);
        if (v40 != -1)
        {
            AI_EvalFunc_COLPointGet_C(AI_gpst_CurrentGameObject, v40 + 0x80000000, &a3);
            pos = a3;
            if (v44->des_water_smoke)
            {
                v42 = (tAI*)AI_EvalFunc_OBJDuplicate_C(
                    (tGameObjectMain*)SFX_Fragments_SFX_frag_comportement_SFX_Fragments_SFX_explosion_gao,
                    &pos);
                Src.msgID = 1;
                Src.Int1 = 7;
                Src.f2 = v44->des_f_smoke_size;
                Src.f1 = v44->des_f_smoke_speed;
                Src.v1.x = v44->des_f_smoke_detail;
                Src.v1.y = 0.0;
                Src.v1.z = -1.0e14f;
                v18 = (tGameObjectMain*)v42;
                if (v42)
                {
                    if ((unsigned int)v42 <= 2)
                        v18 = AI_MainActor[(uint32_t)v42 - 1];
                    AI_EvalFunc_MSGSend_C(v18, &Src);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &Src);
                }
                v30.x = 0.001f;
                v30.y = 0.001f;
                v30.z = 0.99900001f;
                v17 = (tGameObjectMain*)v42;
                if (v42)
                {
                    if ((unsigned int)v42 <= 2)
                        v17 = AI_MainActor[(uint32_t)v42 - 1];
                    AI_EvalFunc_OBJ_SightSet_C(v17, &v30);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightSet_C(0, &v30);
                }
                if (v42 == WOR_gst_Universe.univAIInstance)
                {
                    v14 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v16 = (tGameObjectMain*)v42;
                    if (v42)
                    {
                        if ((unsigned int)v42 <= 2)
                            v16 = AI_MainActor[(uint32_t)v42 - 1];
                        v15 = v16;
                    }
                    else
                    {
                        v15 = 0;
                    }
                    v14 = v15->extended->AIInstance;
                }
                ((tScriptStruct_SFXExplosion*)(v14->instanceVarDataCurrent))->table_color[1] = 0xFFFFFF;
            }
            v44->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
            v35 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_Fragments_SFX_frag_comportement_SFX_Fragments_SFX_frag_disparition);
            if (v35 != 1000)
                return v35;
        }
    }
    v41 = 0;
    if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x40))
        v41 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 0x40);
    v13 = v41;
    if (v41)
    {
        if ((unsigned int)v41 <= 2)
            v13 = AI_MainActor[(uint32_t)v41 - 1];
        v12 = v13;
    }
    else
    {
        v12 = 0;
    }
    if (v12)
    {
        ++v44->i_nb_rebonds;
        v8 = (float)(2 * v44->i_nb_rebonds);
        v44->f_rotation_speed = v44->f_init_rotation_speed / v8;
        v7 = (float)v44->i_nb_rebonds;
        v11 = 1.0f - v7 * 0.25f;
        if (v11 < 0.0)
            v6 = 0.0;
        else
            v6 = 1.0f - v7 * 0.25f;
        v39 = v6;
        if (v44->i_flag_play_collision && v39 != 0.0f)
        {
            v44->f_time_play_collision = TIM_gf_MainClock - TIM_gf_LockTime;
            v44->i_flag_play_collision = 0;
            v45 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(0, -1.0);
            v22 = SND_f_GetVol(v45);
            anglec = v22 * v39;
            SND_SetVol(v45, anglec);
            v48 = SND_l_GetFreq(v45);
            v32 = AI_EvalFunc_MATHRandFloat_C(0.89999998f, 1.1f);
            v5 = (float)v48;
            SND_SetFreq(v45, (unsigned int)(v5 * v32));
        }
        if (v44->i_nb_rebonds > v44->i_nb_max_rebonds)
        {
            v44->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
            v33 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_Fragments_SFX_frag_comportement_SFX_Fragments_SFX_frag_disparition);
            if (v33 != 1000)
                return v33;
        }
        AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&a2);
        v46.x = 0.0;
        v46.y = 0.0;
        v46.z = 1.0;
        AI_EvalFunc_MATHVecCrossProduct_C(&a2, &v46, &v24);
        AI_EvalFunc_MATHVecNormalize_C(&v24, &v50);
        AI_EvalFunc_OBJ_BankingSet_C(AI_gpst_CurrentGameObject, &v50);
    }
    return 1000;
}