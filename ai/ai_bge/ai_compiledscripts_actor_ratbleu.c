#include "ai_compiledscripts_inc.h"

//AR_RatBleu_rb_init
int AR_RatBleu_rb_init_INV_manager_gao;
int AR_RatBleu_rb_init_INV_manager_gao_i_Animals_Nb;
int AR_RatBleu_rb_init_INV_manager_gao_ao_Animals;
int AR_RatBleu_rb_init_INV_manager_gao_ai_AnimalsData;
int fct_AR_RatBleu_rb_init_AR_RatBleu_rb_loop;
int AR_RatBleu_rb_init_INV_manager_gao_i_Animals_Nb;

//AR_RatBleu_rb_loop
int fct_AR_RatBleu_rb_loop__PJ_Sally_gao;

int AR_RatBleu_rb_init()
{
    double v0; // st7
    int result; // eax
    tScriptStruct_ActorRatBleu* v2; // [esp+0h] [ebp-1Ch]
    int v3; // [esp+4h] [ebp-18h]
    int v4; // [esp+14h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorRatBleu*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        AR_RatBleu_rb_init_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &AR_RatBleu_rb_init_INV_manager_gao_i_Animals_Nb,
            (void*)AR_RatBleu_rb_init_INV_manager_gao,
            23888);
        AI2C_ResolveBuffer(
            &AR_RatBleu_rb_init_INV_manager_gao_ao_Animals,
            (void*)AR_RatBleu_rb_init_INV_manager_gao,
            23368);
        AI2C_ResolveBuffer(
            &AR_RatBleu_rb_init_INV_manager_gao_ai_AnimalsData,
            (void*)AR_RatBleu_rb_init_INV_manager_gao,
            23628);
        fct_AR_RatBleu_rb_init_AR_RatBleu_rb_loop = (int)AI_ul_RealResolveOneRefFunction(0x3D000711u);
    }
    if (*(int*)AR_RatBleu_rb_init_INV_manager_gao_i_Animals_Nb < 64)
    {
        *(uint32_t*)(AR_RatBleu_rb_init_INV_manager_gao_ao_Animals
            + 4 * *(uint32_t*)AR_RatBleu_rb_init_INV_manager_gao_i_Animals_Nb
            + 4) = (uint32_t)AI_gpst_CurrentGameObject;
        *(uint32_t*)(AR_RatBleu_rb_init_INV_manager_gao_ai_AnimalsData
            + 4 * (*(uint32_t*)AR_RatBleu_rb_init_INV_manager_gao_i_Animals_Nb)++
            + 4) = (v2->i_MapEtage << 8) + 8;
    }
    v4 = AI_EvalFunc_MATHRandInt_C(2, 4);
    AI_EvalFunc_ACTActionSet_C_CURRENT(v4);
    v0 = TIM_gf_MainClock - TIM_gf_LockTime;
    v2->time_anim = (float)v0;    
    v2->alea = AI_EvalFunc_MATHRandFloat_C(0.80000001f, 1.5);
    v3 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_AR_RatBleu_rb_init_AR_RatBleu_rb_loop);
    if (v3 == 1000)
        result = 1000;
    else
        result = v3;
    return result;
}

int AR_RatBleu_rb_loop()
{
    float v1; // [esp+4h] [ebp-6Ch]
    float v2; // [esp+8h] [ebp-68h]
    tScriptStruct_ActorRatBleu* v3; // [esp+Ch] [ebp-64h]
    float v4; // [esp+1Ch] [ebp-54h]
    Vec3 dest; // [esp+20h] [ebp-50h] BYREF
    Vec3 a1; // [esp+2Ch] [ebp-44h] BYREF
    int v9; // [esp+44h] [ebp-2Ch]
    Vec3 v10; // [esp+48h] [ebp-28h] BYREF
    Vec3 a3; // [esp+58h] [ebp-18h] BYREF
    tScriptStruct_ActorRatBleu* v12; // [esp+64h] [ebp-Ch]
    int v13; // [esp+68h] [ebp-8h]

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_ActorRatBleu*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v12 = v3;
    v9 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_AR_RatBleu_rb_loop__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
    }
    v9 = AI_EvalFunc_ACTActionGet_C_CURRENT();
    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)fct_AR_RatBleu_rb_loop__PJ_Sally_gao, &a3);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v10);
    MATH_SubstractVec(&dest, &a3, &v10);
    a1 = dest;
    v4 = MATH_VecLen(&a1);
    v2 = (float)v12->i_distance;
    if (v4 < (double)v2)
    {
        if (v9 == 5)
        {
            if (AI_EvalFunc_ACTActionFinished_C_CURRENT())
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        }
        else
        {
            AI_EvalFunc_ACTActionSet_C_CURRENT(5);
        }
        v12->time_anim = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    v1 = v12->alea + v12->alea;
    if (AI_EvalFunc_TIMEElapsed_C(v12->time_anim, v1))
    {
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);        
        v12->time_anim = TIM_gf_MainClock - TIM_gf_LockTime;
        if (v9 == 5)
        {
            AI_EvalFunc_ACTActionSet_C_CURRENT(6);
        }
        else
        {
            v13 = AI_EvalFunc_MATHRandInt_C(2, 4);
            AI_EvalFunc_ACTActionSet_C_CURRENT(v13);
        }
    }
    return 1000;
}