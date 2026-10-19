#include "ai_compiledscripts_inc.h"

//SFX_Disapear_dis_init
int fct_SFX_Disapear_dis_init_SFX_Disapear_dis_before_display;
int fct_SFX_Disapear_dis_init_SFX_Disapear_dis_after_display;
int fct_SFX_Disapear_dis_init_SFX_Disapear_dis_phases;

//SFX_Disapear_dis_phases
int fct_SFX_Disapear_dis_phases_SFX_Disapear_dis_before_display;
int fct_SFX_Disapear_dis_phases_SFX_Disapear_dis_after_display;

int SFX_Disapear_dis_init()
{
    int result; // eax
    tScriptStruct_SFXDisapear* v1; // [esp+8h] [ebp-38h]
    float v2; // [esp+10h] [ebp-30h]
    tGameObjectMain* v3; // [esp+18h] [ebp-28h]
    tGameObjectMain* v4; // [esp+20h] [ebp-20h]
    tGameObjectMain* v5; // [esp+24h] [ebp-1Ch]
    tGameObjectMain* v6; // [esp+28h] [ebp-18h]
    tGameObjectMain* v7; // [esp+30h] [ebp-10h]
    int v8; // [esp+38h] [ebp-8h]
    tScriptStruct_SFXDisapear* v9; // [esp+3Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXDisapear*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v9 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_Disapear_dis_init_SFX_Disapear_dis_before_display = (int)AI_ul_RealResolveOneRefFunction(0x7100BB9Eu);
        fct_SFX_Disapear_dis_init_SFX_Disapear_dis_after_display = (int)AI_ul_RealResolveOneRefFunction(0x7100BB9Fu);
        fct_SFX_Disapear_dis_init_SFX_Disapear_dis_phases = (int)AI_ul_RealResolveOneRefFunction(0x49001D82u);
    }
    v1->des_i_emetteur_fixe = 0;
    AI_EvalFunc_LightSetNearFar_C(AI_gpst_CurrentGameObject, v1->f_Light_Near, v1->f_Light_Far);
    AI_EvalFunc_LightSetFlag_C(AI_gpst_CurrentGameObject, 3, 1);
    v7 = (tGameObjectMain*)v1->des_o_target;
    if (v1->des_o_target)
    {
        if ((unsigned int)v7 <= 2)
            v7 = AI_MainActor[(uint32_t)v7 - 1];
        AI_EvalFunc_LightSetGO_C(AI_gpst_CurrentGameObject, v7);
    }
    else
    {
        AI_EvalFunc_LightSetGO_C(AI_gpst_CurrentGameObject, 0);
    }
    v6 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v6 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        v5 = v6;
    }
    else
    {
        v5 = 0;
    }
    v4 = (tGameObjectMain*)v1->des_o_target;
    if (v1->des_o_target)
    {
        if ((unsigned int)v4 <= 2)
            v4 = AI_MainActor[(uint32_t)v4 - 1];
        AI_EvalFunc_OBJ_AddMaterial_C(v4, v5, v1->des_i_mat_id);
    }
    else
    {
        AI_EvalFunc_OBJ_AddMaterial_C(0, v5, v1->des_i_mat_id);
    }
    v9->i_on_mat_flags = 0;
    v9->i_off_mat_flags = 0x1000;
    AI_EvalFunc_PAG_Setf_C(AI_gpst_CurrentGameObject, 1, v9->des_f_emetteur_diametre);
    AI_EvalFunc_PAG_Setf_C(AI_gpst_CurrentGameObject, 16, v9->des_f_particle_lifetimeMin);
    AI_EvalFunc_PAG_Setf_C(AI_gpst_CurrentGameObject, 17, v9->des_f_particle_lifetimeMax);
    v2 = -v9->f_Light_Far;
    AI_EvalFunc_PAG_Setf_C(AI_gpst_CurrentGameObject, 0, v2);
    AI_EvalFunc_PAG_SetGeneratorFreq_C(AI_gpst_CurrentGameObject, 1, 0.0);
    v9->f_LightStart = v9->f_Light_Near + (v9->f_Light_Far - v9->f_Light_Near) / 2.0f - 1.0f;
    v9->f_LightEnd = v9->f_Light_Far + v9->des_f_hauteur - (v9->f_Light_Far - v9->f_Light_Near) / 2.0f;
    v9->f_GenStart = v9->des_f_hauteur * 0.80000001f;
    v9->f_GenEnd = -v9->des_f_hauteur * 0.40000001f;
    v9->f_time1 = 0.0;
    v9->i_phase = 0;
    v3 = (tGameObjectMain*)v9->des_o_target;
    if (v9->des_o_target)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v3,
            0,
            (tAIFunction*)fct_SFX_Disapear_dis_init_SFX_Disapear_dis_before_display);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            0,
            (tAIFunction*)fct_SFX_Disapear_dis_init_SFX_Disapear_dis_before_display);
    }
    AI_EvalFunc_AICBAdd_C(
        AI_gpst_CurrentGameObject,
        (tGameObjectMain*)1,
        1,
        (tAIFunction*)fct_SFX_Disapear_dis_init_SFX_Disapear_dis_after_display);
    v8 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_Disapear_dis_init_SFX_Disapear_dis_phases);
    if (v8 == 1000)
        result = 1000;
    else
        result = v8;
    return result;
}

int SFX_Disapear_dis_after_display()
{
    tScriptStruct_SFXDisapear* v1; // [esp+0h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXDisapear*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    AI_EvalFunc_SetFlag_C(AI_gpst_CurrentGameObject, v1->des_i_mat_id, -1, v1->i_old_flag_value, ~(v1->i_old_flag_value & 0xffff));
    return 1000;
}

int SFX_Disapear_dis_phases()
{
    float v1; // [esp+8h] [ebp-F0h]
    tScriptStruct_SFXDisapear* v2; // [esp+10h] [ebp-E8h]
    float v3; // [esp+14h] [ebp-E4h]
    tGameObjectMain* v4; // [esp+24h] [ebp-D4h]
    float v5; // [esp+28h] [ebp-D0h]
    tGameObjectMain* v6; // [esp+30h] [ebp-C8h]
    tGameObjectMain* v7; // [esp+34h] [ebp-C4h]
    tGameObjectMain* v8; // [esp+38h] [ebp-C0h]
    tGameObjectMain* v9; // [esp+40h] [ebp-B8h]
    tGameObjectMain* v10; // [esp+48h] [ebp-B0h]
    tGameObjectMain* v11; // [esp+58h] [ebp-A0h]
    float v12; // [esp+5Ch] [ebp-9Ch]
    tGameObjectMain* v13; // [esp+6Ch] [ebp-8Ch]
    float v14; // [esp+70h] [ebp-88h]
    float v15; // [esp+74h] [ebp-84h]
    float v16; // [esp+78h] [ebp-80h]
    float v17; // [esp+7Ch] [ebp-7Ch]
    int v18; // [esp+80h] [ebp-78h]
    Vec3 a3; // [esp+88h] [ebp-70h] BYREF
    Vec3 v20; // [esp+94h] [ebp-64h] BYREF
    Vec3 v21; // [esp+A0h] [ebp-58h] BYREF
    Vec3 v22; // [esp+ACh] [ebp-4Ch]
    float v23; // [esp+BCh] [ebp-3Ch]
    tGameObjectMain* v24; // [esp+C4h] [ebp-34h]
    Vec3 v25; // [esp+C8h] [ebp-30h] BYREF
    float v26; // [esp+D8h] [ebp-20h]
    tScriptStruct_SFXDisapear* v27; // [esp+E0h] [ebp-18h]
    Vec3 v28; // [esp+E4h] [ebp-14h] BYREF

    v22.x = 1.0;
    v22.y = 0.0;
    v22.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SFXDisapear*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v27 = v2;
    v26 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_Disapear_dis_phases_SFX_Disapear_dis_before_display = (int)AI_ul_RealResolveOneRefFunction(0x7100BB9Eu);
        fct_SFX_Disapear_dis_phases_SFX_Disapear_dis_after_display = (int)AI_ul_RealResolveOneRefFunction(0x7100BB9Fu);
    }
    if (*AI_gppst_CurrentJumpNode != (tAINode*)5)
    {
        v13 = (tGameObjectMain*)v27->des_o_target;
        if (v27->des_o_target)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            AI_EvalFunc_OBJGetPos_C(v13, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        v27->v_pos_target = a3;
        v26 = v27->v_pos_target.z;
        if (!v27->i_phase)
        {
            if (v27->f_time_phase0 > (double)v27->f_time1)
            {
                v27->f_time1 = TIM_gf_dt + v27->f_time1;
                v12 = v27->f_time1 / v27->f_time_phase0;
                if (v12 < 0.0)
                    v12 = 0.0;
                if (v12 > 1.0)
                    v12 = 1.0;
                v27->v_pos_target.z = v27->f_LightEnd * v12 + v27->f_LightStart * (1.0f - v12) + v27->v_pos_target.z;
            }
            else
            {
                v27->i_on_mat_flags = 0x1000;
                v27->i_off_mat_flags = 0;
                v1 = (float)v27->des_i_particle_nbr;
                AI_EvalFunc_PAG_SetGeneratorFreq_C(AI_gpst_CurrentGameObject, 1, v1);
                ++v27->i_phase;
                v27->f_time1 = 0.0;
            }
        }
        if (v27->i_phase)
        {
            if (v27->f_time_phase1 < (double)v27->f_time1)
            {
                v17 = AI_EvalFunc_PAG_Getf_C(AI_gpst_CurrentGameObject, 17);
                v15 = AI_EvalFunc_PAG_Getf_C(AI_gpst_CurrentGameObject, 15);
                v23 = AI_EvalFunc_PAG_Getf_C(AI_gpst_CurrentGameObject, 13);
                v27->f_time1 = v17 + v15 + v23 - v27->f_time2;
                v27->f_time2 = TIM_gf_MainClock - TIM_gf_LockTime;
                goto LABEL_23;
            }
            v27->f_time1 = TIM_gf_dt + v27->f_time1;
            v5 = v27->f_time1 / v27->f_time_phase1;
            if (v5 < 0.0)
                v5 = 0.0;
            if (v5 > 1.0)
                v5 = 1.0;
            v27->v_pos_target.z = v27->f_LightStart * v5 + v27->f_LightEnd * (1.0f - v5) + v27->v_pos_target.z;
            v4 = (tGameObjectMain*)v27->des_o_target;
            if (v27->des_o_target)
            {
                if ((unsigned int)v4 <= 2)
                    v4 = AI_MainActor[(uint32_t)v4 - 1];
                AI_EvalFunc_OBJGetPos_C(v4, &v20);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v20);
            }
            v22 = v20;
            v14 = v20.z - v27->v_pos_target.z;
            if (!v27->des_i_emetteur_fixe)
            {
                v3 = v27->f_time1 / v27->f_time_phase1;
                if (v3 < 0.0)
                    v3 = 0.0;
                if (v3 > 1.0)
                    v3 = 1.0;
                v16 = v27->f_GenEnd * v3 + v27->f_GenStart * (1.0f - v3);
                if (v16 >= 0.0)
                {
                    v14 = v14 + v16;
                }
                else if (v27->f_time2 == 0.0)
                {
                    AI_EvalFunc_PAG_SetGeneratorFreq_C(AI_gpst_CurrentGameObject, 1, 0.0);
                    v27->f_time2 = v27->f_time_phase1 - v27->f_time1;
                }
            }
            AI_EvalFunc_PAG_Setf_C(AI_gpst_CurrentGameObject, 0, v14);
        }
    LABEL_72:
        AI_EvalFunc_OBJSetPos_C_CURRENT(&v27->v_pos_target);
        return 1000;
    }
LABEL_23:
    if (AI_EvalFunc_TIMEElapsed_C(v27->f_time2, v27->f_time1))
    {
        v10 = (tGameObjectMain*)v27->des_o_target;
        if (v27->des_o_target)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                v10,
                0,
                (tAIFunction*)fct_SFX_Disapear_dis_phases_SFX_Disapear_dis_before_display);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                0,
                0,
                (tAIFunction*)fct_SFX_Disapear_dis_phases_SFX_Disapear_dis_before_display);
        }
        v9 = (tGameObjectMain*)v27->des_o_target;
        if (v27->des_o_target)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                v9,
                1,
                (tAIFunction*)fct_SFX_Disapear_dis_phases_SFX_Disapear_dis_after_display);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                0,
                1,
                (tAIFunction*)fct_SFX_Disapear_dis_phases_SFX_Disapear_dis_after_display);
        }
        v24 = AI_gpst_CurrentGameObject;
        v8 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v8 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            v7 = v8;
        }
        else
        {
            v7 = 0;
        }
        v6 = (tGameObjectMain*)v27->des_o_target;
        if (v27->des_o_target)
        {
            if ((unsigned int)v6 <= 2)
                v6 = AI_MainActor[(uint32_t)v6 - 1];
            AI_EvalFunc_OBJ_AddMaterial_C(v6, v7, -1);
        }
        else
        {
            AI_EvalFunc_OBJ_AddMaterial_C(0, v7, -1);
        }
        v18 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v18 != 1000)
            return v18;
        goto LABEL_72;
    }
    v11 = (tGameObjectMain*)v27->des_o_target;
    if (v27->des_o_target)
    {
        if ((unsigned int)v11 <= 2)
            v11 = AI_MainActor[(uint32_t)v11 - 1];
        AI_EvalFunc_OBJGetPos_C(v11, &v25);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v25);
    }
    v28.x = 0.0;
    v28.y = 0.0;
    v28.z = v27->f_Light_Near;
    MATH_VecAddVec(&v21, &v25, &v28);
    v27->v_pos_target = v21;
    AI_EvalFunc_OBJSetPos_C_CURRENT(&v27->v_pos_target);
    *AI_gppst_CurrentJumpNode = (tAINode*)5;
    return 0;
}

int SFX_Disapear_dis_before_display()
{
    tScriptStruct_SFXDisapear* v1; // [esp+0h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXDisapear*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v1->i_old_flag_value = AI_EvalFunc_GetFlag_C(AI_gpst_CurrentGameObject, v1->des_i_mat_id, -1);
    AI_EvalFunc_SetFlag_C(AI_gpst_CurrentGameObject, v1->des_i_mat_id, -1, v1->i_on_mat_flags, v1->i_off_mat_flags);
    return 1000;
}