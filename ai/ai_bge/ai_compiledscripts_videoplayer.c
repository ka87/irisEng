#include "ai_compiledscripts_inc.h"

//Video_Player_VP_int
int fct_Video_Player_VP_int_Video_Player_VP_go;

//Video_Player_VP_go
int Video_Player_VP_go_INV_manager_gao;

int Video_Player_VP_int()
{
    int result; // eax
    tScriptStruct_VideoPlayer* v1; // [esp+0h] [ebp-10h]
    int v2; // [esp+8h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_VideoPlayer*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Video_Player_VP_int_Video_Player_VP_go = (int)AI_ul_RealResolveOneRefFunction(0x26000D75u);
    }
    v1->f_time_wait = TIM_gf_MainClock - TIM_gf_LockTime;
    v2 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Video_Player_VP_int_Video_Player_VP_go);
    if (v2 == 1000)
        result = 1000;
    else
        result = v2;
    return result;
}

int Video_Player_VP_go()
{
    int v0; // eax
    tScriptStruct_VideoPlayer* v2; // [esp+8h] [ebp-68h]
    Vec3 a2; // [esp+2Ch] [ebp-44h] BYREF
    Vec3 v6; // [esp+38h] [ebp-38h] BYREF
    Vec3 v7; // [esp+44h] [ebp-2Ch] BYREF
    int v8; // [esp+50h] [ebp-20h]
    int i; // [esp+54h] [ebp-1Ch]    
    int v12; // [esp+60h] [ebp-10h]
    Vec3 a3; // [esp+64h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_VideoPlayer*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;

    i = 0;
    v12 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Video_Player_VP_go_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
    }
    v12 = 0;
    AI_EvalFunc_IoJoyGetMove_C(&v6);
    if (AI2C_VecEqNull(&v6))
    {
        for (i = 0; i < 15; ++i)
        {
            v0 = INO_b_IoButtonPressed(v12);
            v8 = v0;
            if (v0)
            {
                v12 = 1;
                break;
            }
        }
    }
    else
    {
        v12 = 1;
    }
    if (v12)
        v2->f_time_wait = AI_EvalFunc_TIMEGet_C();
    if (AI_EvalFunc_TIMEElapsed_C(v2->f_time_wait, v2->f_delai))
    {
        if (!v2->VideoIsPlaying)
        {
            v2->VideoIsPlaying = 1;
            AI_EvalFunc_DbgTraceInt_C();
            AI_EvalFunc_Gsp_AE_MASTER_C(15, 0, 0, 0.80000001f);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v7);
            a3.x = 100000.0;
            a3.y = 100000.0;
            a3.z = 100000.0;
            MATH_VecAddVec(&a2, &v7, &a3);
            AI_EvalFunc_OBJSetPos_C(AI2C_CheckActorGao((tGameObjectMain*)v2->o_camera), &a2);            
            AI_EvalFunc_OBJ_FlagInvisibleSet_C((tGameObjectMain*)Video_Player_VP_go_INV_manager_gao, 1);
        }
    }
    else if (v2->VideoIsPlaying)
    {
        v2->VideoIsPlaying = 0;
        AI_EvalFunc_DbgTraceInt_C();
        AI_EvalFunc_Gsp_AE_MASTER_C(15, 0, 0, 0.80000001f);
        AI_EvalFunc_OBJReinit_C(AI2C_CheckActorGao((tGameObjectMain*)v2->o_camera), 1);        
        AI_EvalFunc_OBJ_FlagInvisibleSet_C((tGameObjectMain*)Video_Player_VP_go_INV_manager_gao, 0);
    }
    return 1000;
}