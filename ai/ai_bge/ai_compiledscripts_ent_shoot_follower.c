#include "ai_compiledscripts_inc.h"

//ent_shoot_follower_esf_init
int fct_ent_shoot_follower_esf_init_ent_shoot_follower_esf_follow;

//ent_shoot_follower_est_exec_mvt
int ent_shoot_follower_est_exec_mvt_PJ_Camera_gao;

//ent_shoot_follower_esf_follow
int fct_ent_shoot_follower_esf_follow_ent_shoot_follower_est_exec_mvt;

int ent_shoot_follower_esf_init()
{
    int result; // eax
    tScriptStruct_EntShootFollower* v1; // [esp+0h] [ebp-40h]
    Vec3 a1; // [esp+4h] [ebp-3Ch] BYREF
    Vec3 a3; // [esp+10h] [ebp-30h] BYREF
    int v4; // [esp+1Ch] [ebp-24h]
    BOOL v5; // [esp+20h] [ebp-20h]
    int v6; // [esp+24h] [ebp-1Ch]
    Vec3 a2; // [esp+28h] [ebp-18h] BYREF
    int v8; // [esp+34h] [ebp-Ch]
    tScriptStruct_EntShootFollower* v9; // [esp+38h] [ebp-8h]
    float v10; // [esp+3Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_EntShootFollower*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v9 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_ent_shoot_follower_esf_init_ent_shoot_follower_esf_follow = (int)AI_ul_RealResolveOneRefFunction(0x71003615u);
    }
    v5 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
    v8 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x204, 0);
    AI_EvalFunc_MATHVecNormalize_C(&v9->des_v_speed, &a2);
    a3.x = a2.x * 30.0f;
    a3.y = a2.y * 30.0f;
    a3.z = a2.z * 30.0f;
    MATH_VecAddVec(&a1, &v9->des_v_speed, &a3);
    v6 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &a1);
    v10 = TIM_gf_MainClock - TIM_gf_LockTime;
    v9->f_time = v10;
    v4 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_ent_shoot_follower_esf_init_ent_shoot_follower_esf_follow);
    if (v4 == 1000)
        result = 1000;
    else
        result = v4;
    return result;
}

int ent_shoot_follower_est_exec_mvt()
{
    float v1; // [esp+8h] [ebp-F8h]
    tScriptStruct_EntShootFollower* v2; // [esp+Ch] [ebp-F4h]
    float v3; // [esp+28h] [ebp-D8h]
    Vec3 v4; // [esp+40h] [ebp-C0h] BYREF
    Vec3 a1; // [esp+4Ch] [ebp-B4h] BYREF
    Vec3 v6; // [esp+58h] [ebp-A8h] BYREF
    Vec3 a2; // [esp+64h] [ebp-9Ch] BYREF
    Vec3 v8; // [esp+70h] [ebp-90h] BYREF
    Vec3 v9; // [esp+7Ch] [ebp-84h] BYREF
    Vec3 v10; // [esp+88h] [ebp-78h] BYREF
    Vec3 v11; // [esp+98h] [ebp-68h] BYREF
    Vec3 v12; // [esp+A4h] [ebp-5Ch] BYREF
    Vec3 v13; // [esp+B0h] [ebp-50h] BYREF
    Vec3 a4; // [esp+BCh] [ebp-44h] BYREF
    Vec3 dest; // [esp+C8h] [ebp-38h] BYREF
    tScriptStruct_EntShootFollower* v16; // [esp+D4h] [ebp-2Ch]
    float v17; // [esp+D8h] [ebp-28h]
    Vec3 v18; // [esp+DCh] [ebp-24h] BYREF
    Vec3 a3; // [esp+E8h] [ebp-18h] BYREF
    Vec3 v20; // [esp+F4h] [ebp-Ch] BYREF

    v10.x = 1.0;
    v10.y = 0.0;
    v10.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_EntShootFollower*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v16 = v2;
    v17 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        ent_shoot_follower_est_exec_mvt_PJ_Camera_gao = AI_ul_RealResolveOneRefObject(0x71003F56u);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v8);
    MATH_SubstractVec(&dest, &v16->v_mvt_target_pos, &v8);
    a1 = dest;
    v13.x = 0.0;
    v13.y = 0.0;
    v13.z = 1.0;
    AI_EvalFunc_OBJ_BankingSet_C(AI_gpst_CurrentGameObject, &v13);
    AI_EvalFunc_MATHVecGlobalToLocal_C(AI_gpst_CurrentGameObject, &a1, &v4);
    v10 = v4;
    if (v4.x <= 70.0)
    {
        if (v10.x >= -70.0)
            v3 = v10.x;
        else
            v3 = -70.0;
    }
    else
    {
        v3 = 70.0;
    }
    v10.x = v3;
    v1 = (float)fabs(v3 / 70.0);
    v17 = v1;
    if (v10.x <= 0.0)
    {
        AI_EvalFunc_OBJ_BankingGet_C((tGameObjectMain*)ent_shoot_follower_est_exec_mvt_PJ_Camera_gao, &v18);
        AI_EvalFunc_OBJ_HorizonGet_C((tGameObjectMain*)ent_shoot_follower_est_exec_mvt_PJ_Camera_gao, &v9);
        v12.x = -v9.x;
        v12.y = -v9.y;
        v12.z = -v9.z;
        AI_EvalFunc_MATHVecBlend_C(&v18, &v12, v17, &v6);
        v10 = v6;
    }
    else
    {
        AI_EvalFunc_OBJ_BankingGet_C((tGameObjectMain*)ent_shoot_follower_est_exec_mvt_PJ_Camera_gao, &a3);
        AI_EvalFunc_OBJ_HorizonGet_C((tGameObjectMain*)ent_shoot_follower_est_exec_mvt_PJ_Camera_gao, &a2);
        AI_EvalFunc_MATHVecBlend_C(&a3, &a2, v17, &a4);
        v10 = a4;
    }
    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v20);
    AI_EvalFunc_MATHVecBlend_C(&v20, &v10, 0.85000002f, &v11);
    AI_EvalFunc_OBJ_BankingSet_C(AI_gpst_CurrentGameObject, &v11);
    AI_EvalFunc_MATHVecSetNormVector_C(&a1, v16->f_mvt_proj_speed);
    AI_EvalFunc_DYNTractionSet_C_CURRENT(&a1);
    return 1000;
}

int ent_shoot_follower_esf_follow()
{
    int result; // eax
    tScriptStruct_EntShootFollower* v1; // [esp+8h] [ebp-28h]
    tGameObjectMain* v2; // [esp+10h] [ebp-20h]
    int v3; // [esp+14h] [ebp-1Ch]
    int v4; // [esp+28h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_EntShootFollower*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_ent_shoot_follower_esf_follow_ent_shoot_follower_est_exec_mvt = (int)AI_ul_RealResolveOneRefFunction(0x71004197u);
    }
    if (AI_EvalFunc_TIMEElapsed_C(v1->f_time, 60.0))
    {
        v4 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v4 == 1000)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            result = v4;
        }
    }
    else
    {
        v2 = (tGameObjectMain*)v1->des_o_lockedtarget;
        if (v2)
        {
            if ((unsigned int)v2 <= 2)
                v2 = AI_MainActor[(uint32_t)v2 - 1];
            AI_EvalFunc_OBJGetPos_C(v2, &v1->v_mvt_target_pos);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v1->v_mvt_target_pos);
        }
        v3 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_ent_shoot_follower_esf_follow_ent_shoot_follower_est_exec_mvt);
        if (v3 == 1000)
            result = 1000;
        else
            result = v3;
    }
    return result;
}