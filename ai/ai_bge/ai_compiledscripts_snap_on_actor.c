#include "ai_compiledscripts_inc.h"

//snap_on_actor_soa_init
int fct_snap_on_actor_soa_init_snap_on_actor_soa_go;

int snap_on_actor_soa_init()
{
    tScriptStruct_SnapOnActor* v1; // [esp+0h] [ebp-7Ch]
    tGameObjectMain* v2; // [esp+8h] [ebp-74h]
    tGameObjectMain* v3; // [esp+18h] [ebp-64h]
    Vec3 dest; // [esp+44h] [ebp-38h] BYREF
    Vec3 v5; // [esp+50h] [ebp-2Ch] BYREF
    tGameObjectMain* v6; // [esp+5Ch] [ebp-20h]
    Vec3 a3; // [esp+60h] [ebp-1Ch] BYREF
    Vec3 a1; // [esp+6Ch] [ebp-10h] BYREF
    tScriptStruct_SnapOnActor* v9; // [esp+78h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SnapOnActor*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v9 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_snap_on_actor_soa_init_snap_on_actor_soa_go = (int)AI_ul_RealResolveOneRefFunction(0x71005C6Bu);
    }
    if (v9->des_use_initial_offset)
    {
        v5.x = 0.0;
        v5.y = 0.0;
        v5.z = 0.0;
        if (AI2C_DistOfTwoPointsIsZero(&v9->v_offset, &v5))
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
            v3 = (tGameObjectMain*)v9->des_acteur;
            if (v3)
            {
                if ((unsigned int)v3 <= 2)
                    v3 = AI_MainActor[(uint32_t)v3 - 1];
                AI_EvalFunc_OBJGetPos_C(v3, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            MATH_SubstractVec(&dest, &a1, &a3);
            v9->v_offset = dest;
        }
    }
    else
    {
        v9->v_offset.x = 0.0;
        v9->v_offset.y = 0.0;
        v9->v_offset.z = 0.0;
    }
    v6 = AI_gpst_CurrentGameObject;
    v2 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v2 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v2,
            2,
            (tAIFunction*)fct_snap_on_actor_soa_init_snap_on_actor_soa_go);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            2,
            (tAIFunction*)fct_snap_on_actor_soa_init_snap_on_actor_soa_go);
    }
    AI_EvalFunc_AITrackCurStop_C();
    return 1000;
}

int snap_on_actor_soa_go()
{
    tScriptStruct_SnapOnActor* v1; // [esp+0h] [ebp-30h]
    tGameObjectMain* v2; // [esp+8h] [ebp-28h]
    tGameObjectMain* v3; // [esp+Ch] [ebp-24h]
    tGameObjectMain* v4; // [esp+10h] [ebp-20h]
    Vec3 a3; // [esp+14h] [ebp-1Ch] BYREF
    Vec3 a1; // [esp+20h] [ebp-10h] BYREF
    tScriptStruct_SnapOnActor* v7; // [esp+2Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SnapOnActor*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v7 = v1;
    v4 = (tGameObjectMain*)v1->des_acteur;
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
    if (v3)
    {
        v2 = (tGameObjectMain*)v7->des_acteur;
        if (v2)
        {
            if ((unsigned int)v2 <= 2)
                v2 = AI_MainActor[(uint32_t)v2 - 1];
            AI_EvalFunc_OBJGetPos_C(v2, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        MATH_VecAddVec(&a1, &a3, &v7->v_offset);
        AI_EvalFunc_OBJSetPos_C_CURRENT(&a1);
    }
    return 1000;
}