#include "ai_compiledscripts_inc.h"

//Mobile_Mobile_init
int fct_Mobile_Mobile_init_Mobile_Mobile_comportement;

//Mobile_Mobile_comportement
int Mobile_Mobile_comportement_PJ_Sally_gao;

int Mobile_Mobile_init()
{
    int result; // eax
    tScriptStruct_Mobile* v1; // [esp+0h] [ebp-44h]
    int v2; // [esp+Ch] [ebp-38h]
    Vec3 dest; // [esp+10h] [ebp-34h] BYREF
    Vec3 a2; // [esp+1Ch] [ebp-28h] BYREF
    Vec3 a3; // [esp+28h] [ebp-1Ch] BYREF
    Vec3 a1; // [esp+34h] [ebp-10h] BYREF
    tScriptStruct_Mobile* v7; // [esp+40h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Mobile*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v7 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Mobile_Mobile_init_Mobile_Mobile_comportement = (int)AI_ul_RealResolveOneRefFunction(0x72004BD3u);
    }
    v7->i_flag_reinit_done = 1;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&a1);
    a3.x = a1.x + a1.x;
    a3.y = a1.y + a1.y;
    a3.z = a1.z + a1.z;
    MATH_SubstractVec(&dest, &a2, &a3);
    v7->v_grav_pos = dest;
    v2 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Mobile_Mobile_init_Mobile_Mobile_comportement);
    if (v2 == 1000)
        result = 1000;
    else
        result = v2;
    return result;
}

int Mobile_Mobile_comportement()
{
    int result; // eax
    int v1; // eax
    float v2; // [esp+4h] [ebp-180h]
    tScriptStruct_Mobile* v3; // [esp+8h] [ebp-17Ch]
    float v4; // [esp+Ch] [ebp-178h]
    Vec3* v5; // [esp+1Ch] [ebp-168h]
    Vec3* v6; // [esp+20h] [ebp-164h]
    Vec3* v7; // [esp+24h] [ebp-160h]
    Vec3* v8; // [esp+28h] [ebp-15Ch]
    float v9; // [esp+30h] [ebp-154h]
    Vec3* v10; // [esp+3Ch] [ebp-148h]
    tGameObjectMain* v11; // [esp+4Ch] [ebp-138h]
    Vec3 v12; // [esp+58h] [ebp-12Ch] BYREF
    float v13; // [esp+64h] [ebp-120h]
    Vec3 v14; // [esp+68h] [ebp-11Ch] BYREF
    Vec3 v15; // [esp+74h] [ebp-110h] BYREF
    Vec3 v16; // [esp+80h] [ebp-104h]
    float v17; // [esp+8Ch] [ebp-F8h]
    float v18; // [esp+90h] [ebp-F4h]
    float v19; // [esp+94h] [ebp-F0h]
    Vec3 a3; // [esp+98h] [ebp-ECh] BYREF
    float v21; // [esp+A4h] [ebp-E0h]
    float v22; // [esp+A8h] [ebp-DCh]
    float v23; // [esp+ACh] [ebp-D8h]
    int v24; // [esp+B0h] [ebp-D4h]
    int v25; // [esp+B4h] [ebp-D0h]
    Vec3 v26; // [esp+B8h] [ebp-CCh] BYREF
    Vec3 a1; // [esp+C4h] [ebp-C0h] BYREF
    Vec3 v28; // [esp+D0h] [ebp-B4h] BYREF
    float v29; // [esp+DCh] [ebp-A8h]
    float v30; // [esp+E0h] [ebp-A4h]
    float v31; // [esp+E4h] [ebp-A0h]
    Vec3 v32; // [esp+E8h] [ebp-9Ch]
    Vec3 v33; // [esp+F4h] [ebp-90h] BYREF
    float v34; // [esp+100h] [ebp-84h]
    Vec3 v35; // [esp+104h] [ebp-80h] BYREF
    Vec3 a2; // [esp+110h] [ebp-74h] BYREF
    float v37; // [esp+11Ch] [ebp-68h]
    Vec3 dest; // [esp+120h] [ebp-64h] BYREF
    float v39; // [esp+12Ch] [ebp-58h]
    Vec3 v41; // [esp+134h] [ebp-50h] BYREF
    Vec3 v42; // [esp+140h] [ebp-44h] BYREF
    tScriptStruct_Mobile* v43; // [esp+150h] [ebp-34h]
    float v44; // [esp+154h] [ebp-30h]
    Vec3 v45; // [esp+164h] [ebp-20h] BYREF
    float v46; // [esp+170h] [ebp-14h]
    Vec3 v47; // [esp+174h] [ebp-10h] BYREF

    v28.x = 1.0;
    v28.y = 0.0;
    v28.z = 0.0;
    v16.x = 1.0;
    v16.y = 0.0;
    v16.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_Mobile*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v43 = v3;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Mobile_Mobile_comportement_PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
    }
    v24 = (unsigned __int16)AI_gpst_CurrentGameObject->flags2;
    if ((v24 & 4) == 0)
        v43->f_time_culled = TIM_gf_MainClock - TIM_gf_LockTime;
    if (AI_EvalFunc_TIMEElapsed_C(v43->f_time_culled, 5.0))
    {
        if (!v43->i_flag_reinit_done)
        {
            v43->i_flag_reinit_done = 0;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v14);
            v12.x = 0.0;
            v12.y = 0.0;
            v12.z = 1.0;
            AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v14, &v12);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v33);
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v45);
            a3.x = v45.x + v45.x;
            a3.y = v45.y + v45.y;
            a3.z = v45.z + v45.z;
            MATH_SubstractVec(&dest, &v33, &a3);
            v43->v_grav_pos = dest;
            v43->v_grav_speed.x = 0.0;
            v43->v_grav_speed.y = 0.0;
            v43->v_grav_speed.z = 0.0;
            v43->f_speed_rotation = 0.0;
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        v11 = (tGameObjectMain*)Mobile_Mobile_comportement_PJ_Sally_gao;
        if (Mobile_Mobile_comportement_PJ_Sally_gao)
        {
            if ((unsigned int)Mobile_Mobile_comportement_PJ_Sally_gao <= 2)
                v11 = AI_MainActor[Mobile_Mobile_comportement_PJ_Sally_gao - 1];
            v1 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, v11);
        }
        else
        {
            v1 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, 0);
        }
        v25 = v1;
        if (v1)
        {
            if (v43->i_flag_sally_out_BV)
            {
                v43->i_flag_sally_out_BV = 0;
                v43->i_flag_reinit_done = 0;
                AI_EvalFunc_DYNSpeedGetVector_C((tGameObjectMain*)Mobile_Mobile_comportement_PJ_Sally_gao, &a2);
                v16 = a2;
                v17 = a2.x * 0.5f;
                v18 = a2.y * 0.5f;
                v19 = a2.z * 0.5f;
                v10 = &v43->v_grav_speed;
                v43->v_grav_speed.x = v17 + v43->v_grav_speed.x;
                v10->y = v18 + v10->y;
                v10->z = v19 + v10->z;
                v43->f_speed_rotation = AI_EvalFunc_MATHRandFloat_C(0.5, 1.0) + v43->f_speed_rotation;
            }
        }
        else
        {
            v43->i_flag_sally_out_BV = 1;
        }
        v28 = v43->v_grav_pos;
        v37 = TIM_gf_dt;
        v9 = 0.98000002f * TIM_gf_dt;
        v21 = v9 * v43->v_grav_speed.x;
        v22 = v43->v_grav_speed.y * v9;
        v23 = v43->v_grav_speed.z * v9;
        v8 = &v43->v_grav_pos;
        v43->v_grav_pos.x = v21 + v43->v_grav_pos.x;
        v8->y = v22 + v8->y;
        v8->z = v23 + v8->z;
        v32.x = 0.0;
        v32.y = 0.0;
        v32.z = -0.5;
        v13 = TIM_gf_dt;
        v29 = 0.0f * TIM_gf_dt;
        v30 = v29;
        v31 = -0.5f * TIM_gf_dt;
        v7 = &v43->v_grav_pos;
        v43->v_grav_pos.x = v29 + v43->v_grav_pos.x;
        v7->y = v30 + v7->y;
        v7->z = v31 + v7->z;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v41);
        v6 = &v43->v_grav_pos;
        v43->v_grav_pos.x = v43->v_grav_pos.x - v41.x;
        v6->y = v6->y - v41.y;
        v6->z = v6->z - v41.z;
        AI_EvalFunc_MATHVecSetNormVector_C(&v43->v_grav_pos, 2.0);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v35);
        v5 = &v43->v_grav_pos;
        v43->v_grav_pos.x = v35.x + v43->v_grav_pos.x;
        v5->y = v35.y + v5->y;
        v5->z = v35.z + v5->z;
        MATH_SubstractVec(&v47, &v43->v_grav_pos, &v28);
        v39 = TIM_gf_dt;
        MATH_VecMultNormScalar(&a1, &v47, TIM_gf_dt);
        v43->v_grav_speed = a1;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v26);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v42);
        MATH_SubstractVec(&v15, &v42, &v43->v_grav_pos);
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v26, &v15);
        v46 = TIM_gf_dt;
        v2 = v43->f_speed_rotation * TIM_gf_dt;
        AI_EvalFunc_OBJRotateLocalZ_c(AI_gpst_CurrentGameObject, v2);
        v34 = TIM_gf_dt;
        v4 = 0.5f * TIM_gf_dt;
        if (v4 < 0.0)
            v4 = 0.0;
        if (v4 > 1.0)
            v4 = 1.0;
        v44 = 0.0f * v4 + v43->f_speed_rotation * (1.0f - v4);
        v43->f_speed_rotation = v44;
        result = 1000;
    }
    return result;
}