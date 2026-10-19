#include "ai_compiledscripts_inc.h"

//AR_luciole_luciole_init
int AR_luciole_luciole_init_univVars;
int AR_luciole_luciole_init_univVars_Planetes_CurHour;
int fct_AR_luciole_luciole_init_AR_luciole_luciole_move;

//AR_luciole_luciole_move
int fct_AR_luciole_luciole_move_AR_luciole_luciole_loop;

//AR_luciole_luciole_reflexe
int AR_luciole_luciole_reflexe_univVars;
int AR_luciole_luciole_reflexe_univVars_Planetes_CurHour;

//AR_luciole_luciole_loop
int AR_luciole_luciole_loop_SFX_light_smoke_gao;
int fct_AR_luciole_luciole_loop_AR_luciole_luciole_move;

int AR_luciole_luciole_init()
{
    int v0; // eax
    int result; // eax
    tScriptStruct_ActorLuciole* v2; // [esp+0h] [ebp-44h]
    tGameObjectMain* v3; // [esp+4h] [ebp-40h]
    tGameObjectMain* v4; // [esp+8h] [ebp-3Ch]
    Vec3 v5; // [esp+1Ch] [ebp-28h] BYREF
    Vec3 a2; // [esp+30h] [ebp-14h] BYREF
    tScriptStruct_ActorLuciole* v7; // [esp+3Ch] [ebp-8h]
    int v8; // [esp+40h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorLuciole*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v7 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        AR_luciole_luciole_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &AR_luciole_luciole_init_univVars_Planetes_CurHour,
            (void*)AR_luciole_luciole_init_univVars,
            6116);
        fct_AR_luciole_luciole_init_AR_luciole_luciole_move = (int)AI_ul_RealResolveOneRefFunction(0x3D000630u);
    }
    v5.x = -0.2f;
    v5.y = -0.2f;
    v5.z = -0.2f;
    AI_EvalFunc_BVMinSet_C(AI_gpst_CurrentGameObject, &v5);
    a2.x = 0.2f;
    a2.y = 0.2f;
    a2.z = 0.2f;
    AI_EvalFunc_BVMaxSet_C(AI_gpst_CurrentGameObject, &a2);
    AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x40, 0);
    v4 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v4 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        v3 = v4;
    }
    else
    {
        v3 = 0;
    }
    v3->extended->fields->designInt3 = 0;
    AI_EvalFunc_OBJ_InfoPhotoParamSet_C(AI_gpst_CurrentGameObject, 4, 16, 0, 0, 0.050000001f, 1.0, 0.5, 0.25);
    if (v7->des_i_test_cycle)
        v7->i_forcehide = *(float*)AR_luciole_luciole_init_univVars_Planetes_CurHour > 5.0
        && *(float*)AR_luciole_luciole_init_univVars_Planetes_CurHour < 19.0;
    v7->canmove = 0;
    v7->delay = TIM_gf_MainClock - TIM_gf_LockTime;
    v7->time_att = AI_EvalFunc_MATHRandFloat_C(0.0, 1.0);
    v0 = AI_EvalFunc_MATHRandInt_C(0, v7->des_i_num_points);
    v7->des_i_first_wp = v0;
    v7->wp_current = v7->des_ao_allpoints[v7->des_i_first_wp];
    v7->i_cur_wp = v7->des_i_first_wp;
    v8 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_AR_luciole_luciole_init_AR_luciole_luciole_move);
    if (v8 == 1000)
        result = 1000;
    else
        result = v8;
    return result;
}

int AR_luciole_luciole_move()
{
    int result; // eax
    float a4; // [esp+0h] [ebp-10Ch]
    float a4a; // [esp+0h] [ebp-10Ch]
    float a4b; // [esp+0h] [ebp-10Ch]
    float a4c; // [esp+0h] [ebp-10Ch]
    tScriptStruct_ActorLuciole* v5; // [esp+8h] [ebp-104h]
    Vec3* v6; // [esp+14h] [ebp-F8h]
    Vec3* v7; // [esp+18h] [ebp-F4h]
    tGameObjectMain* v8; // [esp+40h] [ebp-CCh]
    float v9; // [esp+48h] [ebp-C4h]
    Vec3 dest; // [esp+4Ch] [ebp-C0h] BYREF
    Vec3 a3; // [esp+58h] [ebp-B4h] BYREF
    float v12; // [esp+64h] [ebp-A8h]
    int v13; // [esp+68h] [ebp-A4h]
    Vec3 a5; // [esp+6Ch] [ebp-A0h] BYREF
    float v15[4]; // [esp+78h] [ebp-94h] BYREF
    float v16; // [esp+88h] [ebp-84h]
    float v17; // [esp+8Ch] [ebp-80h]
    float v18; // [esp+90h] [ebp-7Ch]
    Vec3 v19; // [esp+94h] [ebp-78h] BYREF
    Vec3 v20; // [esp+A0h] [ebp-6Ch] BYREF
    Vec3 v21; // [esp+B4h] [ebp-58h] BYREF
    Vec3 v22; // [esp+C0h] [ebp-4Ch] BYREF
    Vec3 v23; // [esp+CCh] [ebp-40h] BYREF
    float v24; // [esp+D8h] [ebp-34h]
    Vec3 a1; // [esp+DCh] [ebp-30h] BYREF
    tScriptStruct_ActorLuciole* v26; // [esp+E8h] [ebp-24h]
    float v27; // [esp+ECh] [ebp-20h]
    float v28; // [esp+F0h] [ebp-1Ch]
    float v29; // [esp+F4h] [ebp-18h]
    Vec3 v30; // [esp+FCh] [ebp-10h] BYREF

    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_ActorLuciole*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v26 = v5;
    v27 = 0.0;
    v12 = 0.0;
    v28 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_AR_luciole_luciole_move_AR_luciole_luciole_loop = (int)AI_ul_RealResolveOneRefFunction(0x3D000629u);
    }
    if (v26->canmove)
    {
        v8 = (tGameObjectMain*)v26->des_ao_allpoints[v26->i_cur_wp];
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v8 = AI_MainActor[(uint32_t)v8 - 1];
            AI_EvalFunc_OBJGetPos_C(v8, (Vec3*)v15);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, (Vec3*)v15);
        }
        MATH_VecAddVec(&a1, (Vec3*)v15, &v26->v_rand_pos);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v21);
        MATH_SubstractVec(&dest, &a1, &v21);
        a3 = dest;
        v27 = MATH_VecLen(&a3);
        MATH_NormalizeEqualVector(&a3);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v19);
        v29 = TIM_gf_dt;
        a4 = 4.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&v19, &a3, a4, &a5);
        a3 = a5;
        v23.x = 0.0;
        v23.y = 0.0;
        v23.z = 1.0;
        AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a3, &v23);
        AI_EvalFunc_MATHVecSetNormVector_C(&a3, v27);
        v28 = TIM_gf_dt;
        if (v27 > 0.40000001
            || (v26->time_att = AI_EvalFunc_MATHRandFloat_C(1.0, 3.0),
                v26->delay = TIM_gf_MainClock - TIM_gf_LockTime,
                v13 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_AR_luciole_luciole_move_AR_luciole_luciole_loop),
                v13 == 1000))
        {
            AI_EvalFunc_MATHVecSetNormVector_C(&a3, v28);
            v12 = v27 / 20.0f;
            if (AI_EvalFunc_TIMEElapsed_C(v26->delay, 0.5))
            {
                v6 = &v26->add_vitesse1;
                v7 = &v26->add_vitesse;
                v26->add_vitesse1.x = v26->add_vitesse.x;
                v6->y = v7->y;
                v6->z = v7->z;
                v26->delay = TIM_gf_MainClock - TIM_gf_LockTime;
                a4a = -v12;
                v16 = AI_EvalFunc_MATHRandFloat_C(a4a, v12);
                v17 = TIM_gf_dt;
                v26->add_vitesse.x = 4.0f * TIM_gf_dt * v16;
                a4b = -v12;
                v9 = AI_EvalFunc_MATHRandFloat_C(a4b, v12);
                v18 = TIM_gf_dt;
                v26->add_vitesse.y = 4.0f * TIM_gf_dt * v9;
            }
            v24 = TIM_gf_dt;
            a4c = TIM_gf_dt + TIM_gf_dt;
            AI_EvalFunc_MATHVecBlendRotate_C(&v26->add_vitesse1, &v26->add_vitesse, a4c, &v20);
            v26->add_vitesse1 = v20;
            MATH_VecAddVec(&v30, &a3, &v26->add_vitesse1);
            AI_EvalFunc_OBJMove_C(AI_gpst_CurrentGameObject, &v30);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v22);
            AI_EvalFunc_GFXSetv_C_CURRENT(v26->i_idhalo, 21200, &v22);
            result = 1000;
        }
        else
        {
            result = v13;
        }
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int AR_luciole_luciole_reflexe()
{
    int result; // eax
    float v1; // [esp+8h] [ebp-14h]
    tScriptStruct_ActorLuciole* v2; // [esp+Ch] [ebp-10h]
    float v3; // [esp+10h] [ebp-Ch]
    float v4; // [esp+10h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorLuciole*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        AR_luciole_luciole_reflexe_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &AR_luciole_luciole_reflexe_univVars_Planetes_CurHour,
            (void*)AR_luciole_luciole_reflexe_univVars,
            6116);
    }
    if (v2->des_i_test_cycle)
    {
        v3 = *(float*)AR_luciole_luciole_reflexe_univVars_Planetes_CurHour;
        if (*(float*)AR_luciole_luciole_reflexe_univVars_Planetes_CurHour <= 5.0 || v3 >= 19.0)
        {
            v2->canmove = 1;
            v1 = (float)fabs(v3 - 19.0f);
            v4 = v1;
            if (v1 > 0.40000001f)
                v4 = 0.40000001f;
        }
        else
        {
            v4 = 0.40000001f - (v3 - 5.0f);
            if (v4 < 0.0f)
            {
                v4 = 0.0f;
                v2->canmove = 0;
            }
        }
        AI_EvalFunc_GFXSetf_C_CURRENT(v2->i_idhalo, 21000, v4);
        if (v2->i_forcehide)
        {
            v2->canmove = 0;
            v2->i_forcehide = 0;
        }
        if (v2->canmove)
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
        else
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        result = 1000;
    }
    else
    {
        v2->canmove = 1;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int AR_luciole_luciole_loop()
{
    int result; // eax
    tScriptStruct_ActorLuciole* v1; // [esp+0h] [ebp-3Ch]
    tGameObjectMain* v2; // [esp+8h] [ebp-34h]
    int v3; // [esp+14h] [ebp-28h]
    Vec3 a1; // [esp+24h] [ebp-18h] BYREF
    tScriptStruct_ActorLuciole* v5; // [esp+30h] [ebp-Ch]
    int v6; // [esp+34h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ActorLuciole*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v5 = v1;
    v6 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        AR_luciole_luciole_loop_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        fct_AR_luciole_luciole_loop_AR_luciole_luciole_move = (int)AI_ul_RealResolveOneRefFunction(0x3D000630u);
    }
    if (v5->canmove)
    {
        v5->wp_current = v5->des_ao_allpoints[v5->i_cur_wp];
        if (v5->i_idhalo == -1)
        {
            v5->i_idhalo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v5->i_idhalo, 4, 1);
            v2 = (tGameObjectMain*)AR_luciole_luciole_loop_SFX_light_smoke_gao;
            if (AR_luciole_luciole_loop_SFX_light_smoke_gao)
            {
                if ((unsigned int)AR_luciole_luciole_loop_SFX_light_smoke_gao <= 2)
                    v2 = AI_MainActor[AR_luciole_luciole_loop_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v5->i_idhalo, v2, 0);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v5->i_idhalo, 0, 0);
            }
            GFXSetCurGlobal(v5->i_idhalo);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 0.40000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.40000001f);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v5->des_c_couleur);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
        AI_EvalFunc_GFXSetv_C_CURRENT(v5->i_idhalo, 21200, &a1);
        v5->delay = TIM_gf_MainClock - TIM_gf_LockTime;
        v5->time_att = AI_EvalFunc_MATHRandFloat_C(0.0, 1.0);
        v6 = AI_EvalFunc_MATHRandInt_C(0, v5->des_i_num_points);
        if (v6 == v5->i_cur_wp)
        {
            if (++v5->i_cur_wp == v5->des_i_num_points)
                v5->i_cur_wp = 0;
        }
        else
        {
            v5->i_cur_wp = v6;
        }
        v5->v_rand_pos.x = AI_EvalFunc_MATHRandFloat_C(-0.30000001f, 0.30000001f);
        v5->v_rand_pos.y = AI_EvalFunc_MATHRandFloat_C(-0.30000001f, 0.30000001f);
        v3 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_AR_luciole_luciole_loop_AR_luciole_luciole_move);
        if (v3 == 1000)
            result = 1000;
        else
            result = v3;
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}