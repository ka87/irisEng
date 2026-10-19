#include "ai_compiledscripts_inc.h"

//ent_shoot_simple_ess_init
int fct_ent_shoot_simple_ess_init_ent_shoot_simple_ess_tete_chercheuse;
int fct_ent_shoot_simple_ess_init_ent_shoot_simple_ess_go;

int ent_shoot_simple_ess_init()
{
    tScriptStruct_EntShootSimple* v1; // [esp+0h] [ebp-58h]
    tGameObjectMain* v2; // [esp+4h] [ebp-54h]
    tGameObjectMain* v3; // [esp+8h] [ebp-50h]
    Vec3 a3; // [esp+10h] [ebp-48h] BYREF
    Vec3 v5; // [esp+1Ch] [ebp-3Ch] BYREF
    BOOL v6; // [esp+28h] [ebp-30h]
    int v7; // [esp+2Ch] [ebp-2Ch]
    Vec3 a2; // [esp+30h] [ebp-28h] BYREF
    int v9; // [esp+3Ch] [ebp-1Ch]
    float v10; // [esp+40h] [ebp-18h]
    tScriptStruct_EntShootSimple* v11; // [esp+44h] [ebp-14h]
    Vec3 v12; // [esp+48h] [ebp-10h] BYREF
    int v13; // [esp+54h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_EntShootSimple*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v11 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_ent_shoot_simple_ess_init_ent_shoot_simple_ess_tete_chercheuse = (int)AI_ul_RealResolveOneRefFunction(0x4000818u);
        fct_ent_shoot_simple_ess_init_ent_shoot_simple_ess_go = (int)AI_ul_RealResolveOneRefFunction(0x7100328Fu);
    }
    v6 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
    v10 = TIM_gf_MainClock - TIM_gf_LockTime;
    v11->f_time = v10;
    v3 = (tGameObjectMain*)v11->o_locked_perso;
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
    if (v2)
    {
        a2.x = 0.0;
        a2.y = -v11->f_speed;
        a2.z = 0.0;
        AI_EvalFunc_MATHVecLocalToGlobal_C(AI_gpst_CurrentGameObject, &a2, &a3);
        AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &a3);
        v7 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_ent_shoot_simple_ess_init_ent_shoot_simple_ess_tete_chercheuse);
        if (v7 != 1000)
            return v7;
    }
    else
    {
        v12.x = 0.0;
        v12.y = -v11->f_speed - 100.0f;
        v12.z = 0.0;
        AI_EvalFunc_MATHVecLocalToGlobal_C(AI_gpst_CurrentGameObject, &v12, &v5);
        v9 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v5);
        v13 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_ent_shoot_simple_ess_init_ent_shoot_simple_ess_go);
        if (v13 != 1000)
            return v13;
    }
    return 1000;
}

int ent_shoot_simple_ess_go()
{
    int result; // eax
    tScriptStruct_EntShootSimple* v1; // [esp+8h] [ebp-60h]
    tGameObjectMain* v2; // [esp+10h] [ebp-58h]
    tMessage Src; // [esp+1Ch] [ebp-4Ch] BYREF
    int v4; // [esp+50h] [ebp-18h]
    int v6; // [esp+5Ch] [ebp-Ch]
    tScriptStruct_EntShootSimple* v7; // [esp+60h] [ebp-8h]
    int v8; // [esp+64h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_EntShootSimple*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v7 = v1;
    v8 = 0;
    if (AI_EvalFunc_TIMEElapsed_C(v1->f_time, 5.0))
    {
        v6 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v6 == 1000)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            result = v6;
        }
    }
    else
    {
        v8 = AI_EvalFunc_COLBVGetAllOverlaps_C(AI_gpst_CurrentGameObject, (tGameObjectMain**)v7->o_list, 2, 0, 1);
        if (!v8)
            goto LABEL_16;
        AI_EvalFunc_DYNSpeedSet_C(AI_gpst_CurrentGameObject, 0.0);
        v7->o_perso_cible = v7->o_list[0];
        Src.msgID = 12;
        v2 = (tGameObjectMain*)v7->o_perso_cible;
        if (v2)
        {
            if ((unsigned int)v2 <= 2)
                v2 = AI_MainActor[(uint32_t)v2 - 1];
            AI_EvalFunc_MSGSend_C(v2, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
        v4 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v4 == 1000)
            LABEL_16:
        result = 1000;
        else
            result = v4;
    }
    return result;
}

int ent_shoot_simple_ess_tete_chercheuse()
{
    float v1; // [esp+0h] [ebp-104h]
    tScriptStruct_EntShootSimple* v2; // [esp+8h] [ebp-FCh]
    tGameObjectMain* v3; // [esp+18h] [ebp-ECh]
    tGameObjectMain* v4; // [esp+28h] [ebp-DCh]
    tMessage Src; // [esp+34h] [ebp-D0h] BYREF
    Vec3 v7; // [esp+6Ch] [ebp-98h] BYREF
    Vec3 v8; // [esp+78h] [ebp-8Ch] BYREF
    Vec3 a3; // [esp+84h] [ebp-80h] BYREF
    Vec3 v10; // [esp+90h] [ebp-74h] BYREF
    Vec3 v11; // [esp+9Ch] [ebp-68h] BYREF
    float v12; // [esp+A8h] [ebp-5Ch]
    Vec3 a4; // [esp+ACh] [ebp-58h] BYREF
    Vec3 v14; // [esp+B8h] [ebp-4Ch] BYREF
    int v15; // [esp+C4h] [ebp-40h]
    int v16; // [esp+C8h] [ebp-3Ch]
    int v17; // [esp+CCh] [ebp-38h]
    Vec3 dest; // [esp+D0h] [ebp-34h] BYREF
    float v19; // [esp+DCh] [ebp-28h]
    Vec3 v20; // [esp+E0h] [ebp-24h] BYREF
    tScriptStruct_EntShootSimple* v21; // [esp+ECh] [ebp-18h]
    float v22; // [esp+F0h] [ebp-14h]
    float v23; // [esp+F4h] [ebp-10h]
    Vec3 v24; // [esp+F8h] [ebp-Ch] BYREF

    v10.x = 1.0;
    v10.y = 0.0;
    v10.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_EntShootSimple*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v21 = v2;
    v4 = (tGameObjectMain*)v2->o_locked_perso;
    if (v4)
    {
        if ((unsigned int)v4 <= 2)
            v4 = AI_MainActor[(uint32_t)v4 - 1];
        AI_EvalFunc_OBJGetPos_C(v4, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v20);
    MATH_SubstractVec(&dest, &a3, &v20);
    v10 = dest;
    if (AI_EvalFunc_TIMEElapsed_C(v21->f_time, 1.5))
    {
        if (AI_EvalFunc_TIMEElapsed_C(v21->f_time, 6.0))
        {
            v15 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v15 != 1000)
                return v15;
        }
        else
        {
            v22 = v10.z * v10.z + v10.y * v10.y + v10.x * v10.x;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v8);
            v19 = v8.z * v10.z + v8.y * v10.y + v10.x * v8.x;
            v23 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
            v12 = TIM_gf_dt;
            if (v23 * TIM_gf_dt * v19 <= v22)
            {
                AI_EvalFunc_MATHVecNormalize_C(&v10, &v7);
                v10 = v7;
                v24.x = v7.x * 400.0f;
                v24.y = v7.y * 400.0f;
                v24.z = v7.z * 400.0f;
                v16 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v24);
                AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v11);
                AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v11);
            }
            else
            {
                AI_EvalFunc_DYNSpeedSet_C(AI_gpst_CurrentGameObject, 0.0);
                Src.msgID = 12;
                v3 = (tGameObjectMain*)v21->o_locked_perso;
                if (v3)
                {
                    if ((unsigned int)v3 <= 2)
                        v3 = AI_MainActor[(uint32_t)v3 - 1];
                    AI_EvalFunc_MSGSend_C(v3, &Src);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &Src);
                }
                v17 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
                if (v17 != 1000)
                    return v17;
            }
        }
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v14);
        v1 = 10.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlend_C(&v14, &v10, v1, &a4);
        AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &a4);
    }
    return 1000;
}