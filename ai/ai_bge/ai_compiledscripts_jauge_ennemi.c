#include "ai_compiledscripts_inc.h"

//SFX_JaugeEnnemi_jauge_init
int fct_SFX_JaugeEnnemi_jauge_init_SFX_JaugeEnnemi_jauge_comportement;

int SFX_JaugeEnnemi_jauge_init()
{
    int result; // eax
    float a3; // [esp+0h] [ebp-28h]
    tScriptStruct_JaugeEnnemi* v2; // [esp+4h] [ebp-24h]
    int v3; // [esp+8h] [ebp-20h]
    Vec3 a5; // [esp+Ch] [ebp-1Ch] BYREF
    BOOL v5; // [esp+18h] [ebp-10h]
    int v6; // [esp+20h] [ebp-8h]
    tScriptStruct_JaugeEnnemi* v7; // [esp+24h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_JaugeEnnemi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v7 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_JaugeEnnemi_jauge_init_SFX_JaugeEnnemi_jauge_comportement = (int)AI_ul_RealResolveOneRefFunction(0x720005D4u);
    }
    v5 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
    v6 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 4, 0);
    v7->f_time = TIM_gf_MainClock - TIM_gf_LockTime;
    a3 = (float)v7->i_life;
    STR_SetFloat(AI_gpst_CurrentGameObject, 1, a3, 0);
    a5.x = -1.0;
    a5.y = 0.0;
    a5.z = 0.0;
    STR_SetSize(AI_gpst_CurrentGameObject, 1, 0, -1, &a5);
    v3 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_JaugeEnnemi_jauge_init_SFX_JaugeEnnemi_jauge_comportement);
    if (v3 == 1000)
        result = 1000;
    else
        result = v3;
    return result;
}

int SFX_JaugeEnnemi_jauge_comportement()
{
    tScriptStruct_JaugeEnnemi* v1; // [esp+8h] [ebp-4Ch]
    double v2; // [esp+Ch] [ebp-48h]
    float v3; // [esp+14h] [ebp-40h]
    Vec3 v4; // [esp+1Ch] [ebp-38h] BYREF
    float v5; // [esp+28h] [ebp-2Ch]
    Vec3 a2; // [esp+2Ch] [ebp-28h] BYREF
    int v7; // [esp+38h] [ebp-1Ch]
    int v8; // [esp+3Ch] [ebp-18h]
    tScriptStruct_JaugeEnnemi* v10; // [esp+44h] [ebp-10h]
    Vec3 a3; // [esp+48h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_JaugeEnnemi*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v10 = v1;
    v8 = 0;
    v5 = TIM_gf_MainClock - TIM_gf_LockTime;
    v3 = 1.0f - (v5 - v1->f_time) / 2.0f;
    v2 = v3 * 255.0 + MATH_gd_magicDoubleForRound;
    v8 = (*(uint32_t*)&v2);
    if (AI_EvalFunc_TIMEElapsed_C(v1->f_time, 2.0))
    {
        STR_SetString(AI_gpst_CurrentGameObject, 1, (char*)AI_gpst_CurrentFunction->localStringConstsData);
        v7 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v7 != 1000)
            return v7;
    }
    v4.x = 0.0;
    v4.y = 0.0;
    v4.z = 1.0;
    AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v4);
    AI_EvalFunc_OBJ_TextColorSet_C(AI_gpst_CurrentGameObject, 1, 0, -1, 0x10101 * v8);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
    AI_EvalFunc_VIEW3dWorldTo2d_C(0, &a2, &a3);
    STR_SetPos(AI_gpst_CurrentGameObject, 1, 0, -1, &a3);
    return 1000;
}