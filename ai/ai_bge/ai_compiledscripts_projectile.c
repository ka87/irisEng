#include "ai_compiledscripts_inc.h"

//projectile_pr_init
int fct_projectile_pr_init_projectile_pr_go;

int projectile_pr_init()
{
    int result; // eax
    tScriptStruct_Projectile* v1; // [esp+0h] [ebp-10h]
    int v2; // [esp+8h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Projectile*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_projectile_pr_init_projectile_pr_go = (int)AI_ul_RealResolveOneRefFunction(0x7100AF34u);
    }
    v1->f_time = TIM_gf_MainClock - TIM_gf_LockTime;
    if (v1->i_type_projectile)
        v1->i_type_projectile = 2;
    else
        v1->i_type_projectile = 1;
    v2 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_projectile_pr_init_projectile_pr_go);
    if (v2 == 1000)
        result = 1000;
    else
        result = v2;
    return result;
}

int projectile_pr_go()
{
    tScriptStruct_Projectile* v1; // [esp+8h] [ebp-CCh]
    tGameObjectMain* v2; // [esp+10h] [ebp-C4h]
    tGameObjectMain* a7; // [esp+14h] [ebp-C0h] BYREF
    tMessage Src; // [esp+64h] [ebp-70h] BYREF
    int v6; // [esp+A0h] [ebp-34h]
    int v8; // [esp+B8h] [ebp-1Ch]
    int v10; // [esp+C0h] [ebp-14h]
    tScriptStruct_Projectile* v11; // [esp+C4h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Projectile*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v11 = v1;
    v6 = 0;
    if (AI_EvalFunc_TIMEElapsed_C(v1->f_time, 8.0))
    {
        v10 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v10 != 1000)
            return v10;
    }
    v6 = 0;
    if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 128))
    {
        v6 = 1;
        a7 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 128);
    }
    else if (AI_EvalFunc_COLZDEZDEListGet_C(AI_gpst_CurrentGameObject, &a7, 3, 1, v11->i_type_projectile, 0, 1))
    {
        v6 = 1;
    }
    if (!v6)
        return 1000;
    Src.msgID = 37;
    Src.gao = AI_gpst_CurrentGameObject;
    Src.gao2 = 0;
    Src.Int1 = 3;
    Src.Int2 = 0;
    AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&Src.v1);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&Src.v2);
    Src.f2 = 10.0;
    Src.f1 = 1000.0;
    v2 = a7;
    if (a7)
    {
        if ((unsigned int)a7 <= 2)
            v2 = AI_MainActor[(uint32_t)a7 - 1];
        AI_EvalFunc_MSGSend_C(v2, &Src);
    }
    else
    {
        AI_EvalFunc_MSGSend_C(0, &Src);
    }
    v8 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
    if (v8 != 1000)
        return v8;
    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
    return 1;
}