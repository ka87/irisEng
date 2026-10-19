#include "ai_compiledscripts_inc.h"

//Fun_EtoileFilante_ef_loop
int Fun_EtoileFilante_ef_loop_univVars;
int Fun_EtoileFilante_ef_loop_univVars_Planetes_CurHour;
int fct_Fun_EtoileFilante_ef_loop_Fun_EtoileFilante_ef_move;

//Fun_EtoileFilante_ef_move
int fct_Fun_EtoileFilante_ef_move_Fun_EtoileFilante_ef_loop;

//Fun_EtoileFilante_ef_init
int fct_Fun_EtoileFilante_ef_init_Fun_EtoileFilante_ef_loop;

int Fun_EtoileFilante_ef_loop()
{
    int result; // eax
    float v1; // [esp+0h] [ebp-C4h]
    tScriptStruct_FunEtoileFilante* v2; // [esp+4h] [ebp-C0h]
    float v3; // [esp+10h] [ebp-B4h]
    tGameObjectMain* v4; // [esp+18h] [ebp-ACh]
    tGameObjectMain* v5; // [esp+20h] [ebp-A4h]
    tGameObjectMain* v6; // [esp+30h] [ebp-94h]
    tGameObjectMain* v7; // [esp+38h] [ebp-8Ch]
    Vec3 a3; // [esp+40h] [ebp-84h] BYREF
    float v9; // [esp+50h] [ebp-74h]
    Vec3 a1; // [esp+54h] [ebp-70h] BYREF
    Vec3 v11; // [esp+60h] [ebp-64h] BYREF
    tGameObjectMain* v13; // [esp+70h] [ebp-54h]
    Vec3 v14; // [esp+74h] [ebp-50h] BYREF
    Vec3 v15; // [esp+80h] [ebp-44h] BYREF
    int v16; // [esp+8Ch] [ebp-38h]
    Vec3 v17; // [esp+90h] [ebp-34h] BYREF
    unsigned int v18; // [esp+A0h] [ebp-24h]
    tScriptStruct_FunEtoileFilante* v19; // [esp+A4h] [ebp-20h]
    Vec3 v20; // [esp+A8h] [ebp-1Ch] BYREF
    Vec3 v21; // [esp+B4h] [ebp-10h] BYREF
    float v22; // [esp+C0h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_FunEtoileFilante*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v19 = v2;
    v13 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Fun_EtoileFilante_ef_loop_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Fun_EtoileFilante_ef_loop_univVars_Planetes_CurHour,
            (void*)Fun_EtoileFilante_ef_loop_univVars,
            6116);
        fct_Fun_EtoileFilante_ef_loop_Fun_EtoileFilante_ef_move = (int)AI_ul_RealResolveOneRefFunction(0x3D000703u);
    }
    if (v19->espace
        || *(float*)Fun_EtoileFilante_ef_loop_univVars_Planetes_CurHour <= 5.0
        || *(float*)Fun_EtoileFilante_ef_loop_univVars_Planetes_CurHour >= 20.0)
    {
        v1 = v19->alea + v19->alea;
        if (!AI_EvalFunc_TIMEElapsed_C(v19->time, v1))
            goto LABEL_36;
        v18 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x10000000, 0);
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
        v19->time = TIM_gf_MainClock - TIM_gf_LockTime;
        v13 = *AI_MainActor;
        if (v19->espace)
        {
            v7 = v13;
            if (v13)
            {
                if ((unsigned int)v13 <= 2)
                    v7 = AI_MainActor[(uint32_t)v13 - 1];
                AI_EvalFunc_OBJGetPos_C(v7, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            v6 = v13;
            if (v13)
            {
                if ((unsigned int)v13 <= 2)
                    v6 = AI_MainActor[(uint32_t)v13 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v6, &v14);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v14);
            }
            v21.x = v14.x * 200.0f;
            v21.y = v14.y * 200.0f;
            v21.z = v14.z * 200.0f;
            MATH_VecAddVec(&a1, &a3, &v21);
            v19->speed = a1;
        }
        else
        {
            v5 = v13;
            if (v13)
            {
                if ((unsigned int)v13 <= 2)
                    v5 = AI_MainActor[(uint32_t)v13 - 1];
                AI_EvalFunc_OBJGetPos_C(v5, &v17);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v17);
            }
            v4 = v13;
            if (v13)
            {
                if ((unsigned int)v13 <= 2)
                    v4 = AI_MainActor[(uint32_t)v13 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v4, &v20);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v20);
            }
            v3 = v19->des_f_z;
            v11.x = v20.x * v3;
            v11.y = v20.y * v3;
            v11.z = v20.z * v3;
            MATH_VecAddVec(&v15, &v17, &v11);
            v19->speed = v15;
            v19->speed.z = v19->des_f_z;
        }
        AI_EvalFunc_OBJSetPos_C_CURRENT(&v19->speed);
        v9 = AI_EvalFunc_MATHRandFloat_C(-1.0, 1.0);
        v19->speed.x = v9 * 100.0f;
        v22 = AI_EvalFunc_MATHRandFloat_C(-1.0, 1.0);
        v19->speed.y = v22 * 100.0f;
        v19->speed.z = 0.0;
        v16 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Fun_EtoileFilante_ef_loop_Fun_EtoileFilante_ef_move);
        if (v16 == 1000)
            LABEL_36:
        result = 1000;
        else
            result = v16;
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int Fun_EtoileFilante_ef_move()
{
    float size; // [esp+0h] [ebp-54h]
    tScriptStruct_FunEtoileFilante* v2; // [esp+4h] [ebp-50h]
    float v3; // [esp+10h] [ebp-44h]
    Vec3 a2; // [esp+14h] [ebp-40h] BYREF
    Vec3 a1; // [esp+20h] [ebp-34h] BYREF
    int v7; // [esp+30h] [ebp-24h]
    float v8; // [esp+34h] [ebp-20h]
    float v9; // [esp+38h] [ebp-1Ch]
    unsigned int v10; // [esp+3Ch] [ebp-18h]
    tScriptStruct_FunEtoileFilante* v11; // [esp+40h] [ebp-14h]
    Vec3 a3; // [esp+44h] [ebp-10h] BYREF
    float v13; // [esp+50h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_FunEtoileFilante*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v11 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Fun_EtoileFilante_ef_move_Fun_EtoileFilante_ef_loop = (int)AI_ul_RealResolveOneRefFunction(0x3D0006FAu);
    }
    size = v11->alea + v11->alea;
    if (AI_EvalFunc_TIMEElapsed_C(v11->time, size))
    {
        v11->size = v11->size * 0.80000001f;
        if (v11->size < 0.1)
        {
            v10 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0, 0x10000000);
            v8 = AI_EvalFunc_MATHRandFloat_C(-1.0, 1.0);
            v11->speed.x = v8 * 100.0f;
            v3 = AI_EvalFunc_MATHRandFloat_C(-1.0, 1.0);
            v11->speed.y = v3 * 100.0f;
            v11->speed.z = 0.0;
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
            v11->time = TIM_gf_MainClock - TIM_gf_LockTime;
            v7 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Fun_EtoileFilante_ef_move_Fun_EtoileFilante_ef_loop);
            if (v7 != 1000)
                return v7;
        }
    }
    else
    {
        v13 = TIM_gf_MainClock - TIM_gf_LockTime;
        v11->size = v13 - v11->time;
        if (v11->espace)
        {
            if (v11->size > 50.0)
                v11->size = 50.0;
        }
        else if (v11->size > 5.0)
        {
            v11->size = 5.0;
        }
    }
    XMEN_SetSize(AI_gpst_CurrentGameObject, 0, -1, v11->size);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
    v9 = TIM_gf_dt;
    a3.x = TIM_gf_dt * v11->speed.x;
    a3.y = v11->speed.y * TIM_gf_dt;
    a3.z = v11->speed.z * TIM_gf_dt;
    MATH_VecAddVec(&a1, &a2, &a3);
    AI_EvalFunc_OBJSetPos_C_CURRENT(&a1);
    return 1000;
}

int Fun_EtoileFilante_ef_init()
{
    int result; // eax
    tScriptStruct_FunEtoileFilante* v1; // [esp+0h] [ebp-1Ch]
    int v2; // [esp+4h] [ebp-18h]
    float v3; // [esp+Ch] [ebp-10h]
    float v4; // [esp+14h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_FunEtoileFilante*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Fun_EtoileFilante_ef_init_Fun_EtoileFilante_ef_loop = (int)AI_ul_RealResolveOneRefFunction(0x3D0006FAu);
    }
    v3 = AI_EvalFunc_MATHRandFloat_C(-1.0, 1.0);
    v1->speed.x = v3 * 100.0f;
    v4 = AI_EvalFunc_MATHRandFloat_C(-1.5, 1.5);
    v1->speed.y = v4 * 100.0f;
    v1->speed.z = 0.0;
    v1->alea = AI_EvalFunc_MATHRandFloat_C(0.5, 1.5);
    AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
    v1->time = TIM_gf_MainClock - TIM_gf_LockTime;
    v2 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Fun_EtoileFilante_ef_init_Fun_EtoileFilante_ef_loop);
    if (v2 == 1000)
        result = 1000;
    else
        result = v2;
    return result;
}