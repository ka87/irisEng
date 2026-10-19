#include "ai_compiledscripts_inc.h"

//Object_Scalator_osc_scale
int fct_Object_Scalator_osc_scale_Object_Scalator_osc_wait;

//Object_Scalator_osc_wait
int fct_Object_Scalator_osc_wait_Object_Scalator_osc_scale_objet;
int fct_Object_Scalator_osc_wait_Object_Scalator_osc_scale;

//Object_Scalator_osc_init
int fct_Object_Scalator_osc_init_Object_Scalator_osc_scale_objet;
int fct_Object_Scalator_osc_init_Object_Scalator_osc_wait;

int Object_Scalator_osc_scale()
{
    tScriptStruct_ObjectScalator* v1; // [esp+0h] [ebp-50h]
    Vec3* v2; // [esp+4h] [ebp-4Ch]
    Vec3* v3; // [esp+8h] [ebp-48h]
    float v4; // [esp+10h] [ebp-40h]
    float v5; // [esp+14h] [ebp-3Ch]
    float v6; // [esp+18h] [ebp-38h]
    float v7; // [esp+1Ch] [ebp-34h]
    float v8; // [esp+20h] [ebp-30h]
    float v9; // [esp+20h] [ebp-30h]
    float v10; // [esp+24h] [ebp-2Ch]
    Vec3 a3; // [esp+2Ch] [ebp-24h] BYREF
    float v12; // [esp+38h] [ebp-18h]
    float X; // [esp+3Ch] [ebp-14h]
    tScriptStruct_ObjectScalator* v14; // [esp+40h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ObjectScalator*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v14 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Object_Scalator_osc_scale_Object_Scalator_osc_wait = (int)AI_ul_RealResolveOneRefFunction(0x3500BE11u);
    }
    v14->f_cur_coeff = v14->f_delai * TIM_gf_dt + v14->f_cur_coeff;
    if (v14->f_cur_coeff >= 1.0)
    {
        v2 = &v14->v_cur_scale;
        v3 = &v14->v_next_scale;
        v14->v_cur_scale.x = v14->v_next_scale.x;
        v2->y = v3->y;
        v2->z = v3->z;
        OBJ_ScaleSet(AI_gpst_CurrentGameObject, &v14->v_cur_scale);
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Object_Scalator_osc_scale_Object_Scalator_osc_wait);
    }
    else
    {
        v5 = v14->f_cur_coeff;
        if (v5 < 0.0)
            v5 = 0.0;
        if (v5 > 1.0)
            v5 = 1.0;
        X = 0.0f * v5 + (1.0f - v5) * 1.570796f;
        v12 = cosf(X);
        v4 = v14->f_cur_coeff;
        if (v4 < 0.0)
            v4 = 0.0;
        if (v4 > 1.0)
            v4 = 1.0;
        v10 = 3.141593f * v4 + (1.0f - v4) * 0.0f;
        v7 = sinf(v10);
        v8 = v12 * v7;
        v9 = v14->f_squash * v8;
        v6 = v14->f_cur_coeff + v9;
        a3.x = v6 * v14->v_delta_scale.x;
        a3.y = v14->v_delta_scale.y * v6;
        a3.z = v14->v_delta_scale.z * v6;
        MATH_VecAddVec(&v14->v_cur_scale, &v14->v_old_scale, &a3);
        OBJ_ScaleSet(AI_gpst_CurrentGameObject, &v14->v_cur_scale);
    }
    return 1000;
}

int Object_Scalator_osc_wait()
{
    tScriptStruct_ObjectScalator* v1; // [esp+0h] [ebp-80h]
    Vec3* v2; // [esp+4h] [ebp-7Ch]
    Vec3* v3; // [esp+8h] [ebp-78h]
    tGameObjectMain* v4; // [esp+44h] [ebp-3Ch]
    tGameObjectMain* v5; // [esp+48h] [ebp-38h]
    void* Src; // [esp+64h] [ebp-1Ch]
    int v7; // [esp+68h] [ebp-18h]
    Vec3 v8; // [esp+6Ch] [ebp-14h] BYREF
    int v9; // [esp+78h] [ebp-8h]
    tScriptStruct_ObjectScalator* v10; // [esp+7Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ObjectScalator*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v10 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Object_Scalator_osc_wait_Object_Scalator_osc_scale_objet = (int)AI_ul_RealResolveOneRefFunction(0x3500BE30u);
        fct_Object_Scalator_osc_wait_Object_Scalator_osc_scale = (int)AI_ul_RealResolveOneRefFunction(0x3500BE12u);
    }
    Src = AI_EvalFunc_MSGGetByID_C(AI_gpst_CurrentGameObject, 1);
    memcpy(&v10->m_msg, Src, sizeof(v10->m_msg));
    v5 = v10->m_msg.gao;
    if (v5)
    {
        if ((unsigned int)v5 <= 2)
            v5 = AI_MainActor[(uint32_t)v5 - 1];
        v4 = v5;
    }
    else
    {
        v4 = 0;
    }
    if (v4)
    {
        v10->des_o_objet_scale = (int)v10->m_msg.gao;
        v9 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Object_Scalator_osc_wait_Object_Scalator_osc_scale_objet);
        if (v9 != 1000)
            return v9;
    }
    else
    {
        v8.x = 0.0;
        v8.y = 0.0;
        v8.z = 0.0;
        if (!AI2C_DistOfTwoPointsIsZero(&v10->m_msg.v1, &v8)
            && !AI2C_DistOfTwoPointsIsZero(&v10->m_msg.v1, &v10->v_cur_scale))
        {
            v2 = &v10->v_next_scale;
            v3 = &v10->m_msg.v1;
            v10->v_next_scale.x = v10->m_msg.v1.x;
            v2->y = v3->y;
            v2->z = v3->z;
            AI_EvalFunc_OBJ_ScaleGet_C(AI_gpst_CurrentGameObject, &v10->v_old_scale);
            v10->f_delai = 1.0f / v10->m_msg.f2;
            v10->f_squash = v10->m_msg.f1;
            MATH_SubstractVec(&v10->v_delta_scale, &v10->v_next_scale, &v10->v_old_scale);
            v10->f_cur_coeff = 0.0;
            v7 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Object_Scalator_osc_wait_Object_Scalator_osc_scale);
            if (v7 != 1000)
                return v7;
        }
    }
    return 1000;
}

int Object_Scalator_osc_scale_objet()
{
    tScriptStruct_ObjectScalator* v1; // [esp+0h] [ebp-20h]
    tGameObjectMain* v2; // [esp+8h] [ebp-18h]
    Vec3 a3; // [esp+10h] [ebp-10h] BYREF
    tScriptStruct_ObjectScalator* v4; // [esp+1Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ObjectScalator*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v4 = v1;
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
    {
        v2 = (tGameObjectMain*)v4->des_o_objet_scale;
        if (v4->des_o_objet_scale)
        {
            if ((unsigned int)v2 <= 2)
                v2 = AI_MainActor[(uint32_t)v2 - 1];
            AI_EvalFunc_OBJGetPos_C(v2, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        OBJ_ScaleSet(AI_gpst_CurrentGameObject, &a3);
    }
    return 1000;
}

int Object_Scalator_osc_init()
{
    tScriptStruct_ObjectScalator* v1; // [esp+0h] [ebp-2Ch]
    tGameObjectMain* v2; // [esp+4h] [ebp-28h]
    tGameObjectMain* v3; // [esp+8h] [ebp-24h]
    Vec3 a2; // [esp+14h] [ebp-18h] BYREF
    int v5; // [esp+20h] [ebp-Ch]
    int v6; // [esp+24h] [ebp-8h]
    tScriptStruct_ObjectScalator* v7; // [esp+28h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ObjectScalator*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v7 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Object_Scalator_osc_init_Object_Scalator_osc_scale_objet = (int)AI_ul_RealResolveOneRefFunction(0x3500BE30u);
        fct_Object_Scalator_osc_init_Object_Scalator_osc_wait = (int)AI_ul_RealResolveOneRefFunction(0x3500BE11u);
    }
    AI_EvalFunc_OBJ_ScaleGet_C(AI_gpst_CurrentGameObject, &a2);
    v7->v_cur_scale = a2;
    v3 = (tGameObjectMain*)v7->des_o_objet_scale;
    if (v7->des_o_objet_scale)
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
        v6 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Object_Scalator_osc_init_Object_Scalator_osc_scale_objet);
        if (v6 != 1000)
            return v6;
    }
    else
    {
        v5 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Object_Scalator_osc_init_Object_Scalator_osc_wait);
        if (v5 != 1000)
            return v5;
    }
    return 1000;
}