#include "ai_compiledscripts_inc.h"

//Light_PAF_lp_init
int fct_Light_PAF_lp_init_Light_PAF_lp_FarUp_ColorDown;
int fct_Light_PAF_lp_init_Light_PAF_lp_FarUp_FarDown;
int fct_Light_PAF_lp_init_Light_PAF_C_Light_mode_FarDown;

//Light_PAF_lp_FarUp_FarDown
int fct_Light_PAF_lp_FarUp_FarDown_Light_PAF_C_Light_mode_FarDown;

//===========================================

//RLI_Variator_RLI_Variator_init
int fct_RLI_Variator_RLI_Variator_init_RLI_Variator_RLI_Variator_comportement;
int fct_RLI_Variator_RLI_Variator_init_RLI_Variator_RLI_Variator_exec_before_display;
int fct_RLI_Variator_RLI_Variator_init_RLI_Variator_RLI_Variator_exec_after_display;

//RLI_Variator_RLI_Variator_comportement
int fct_RLI_Variator_RLI_Variator_comportement_RLI_Variator_RLI_Variator_exec_mode_stealth;
int fct_RLI_Variator_RLI_Variator_comportement_RLI_Variator_RLI_Variator_exec_mode_standard;

int Light_PAF_lp_init()
{
    int result; // eax
    tScriptStruct_LightPAF* v1; // [esp+4h] [ebp-1Ch]
    int v2; // [esp+8h] [ebp-18h]
    int v3; // [esp+10h] [ebp-10h]
    int v4; // [esp+14h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_LightPAF*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Light_PAF_lp_init_Light_PAF_lp_FarUp_ColorDown = (int)AI_ul_RealResolveOneRefFunction(0x9E006105);
        fct_Light_PAF_lp_init_Light_PAF_lp_FarUp_FarDown = (int)AI_ul_RealResolveOneRefFunction(0x9E006103);
        fct_Light_PAF_lp_init_Light_PAF_C_Light_mode_FarDown = (int)AI_ul_RealResolveOneRefFunction(0x9E005BD9);
    }
    AI_EvalFunc_OBJ_CapaGet_C(AI_gpst_CurrentGameObject);
    if (v1->i_type)
    {
        if (v1->i_type == 2)
        {
            v4 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Light_PAF_lp_init_Light_PAF_lp_FarUp_ColorDown);
            if (v4 != 1000)
                return v4;
        }
        else if (v1->i_type == 1)
        {
            v3 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Light_PAF_lp_init_Light_PAF_lp_FarUp_FarDown);
            if (v3 != 1000)
                return v3;
        }
    }
    AI_EvalFunc_LightSetNearFar_C(AI_gpst_CurrentGameObject, v1->f_near, v1->f_far);
    AI_EvalFunc_LightSetColor_C(AI_gpst_CurrentGameObject, v1->i_color);
    AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
    v2 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Light_PAF_lp_init_Light_PAF_C_Light_mode_FarDown);
    if (v2 == 1000)
        result = 1000;
    else
        result = v2;
    return result;
}

int Light_PAF_lp_FarUp_FarDown()
{
    float v1; // [esp+8h] [ebp-30h]
    float v2; // [esp+Ch] [ebp-2Ch]
    tScriptStruct_LightPAF* v3; // [esp+10h] [ebp-28h]
    float v4; // [esp+18h] [ebp-20h]
    float v5; // [esp+20h] [ebp-18h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_LightPAF*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Light_PAF_lp_FarUp_FarDown_Light_PAF_C_Light_mode_FarDown = (int)AI_ul_RealResolveOneRefFunction(0x9E005BD9);
    }
    v5 = v3->f_blend * v3->f_near * TIM_gf_dt + v3->f_near_current;
    if (v3->f_near > (double)v5)
        v2 = v3->f_blend * v3->f_near * TIM_gf_dt + v3->f_near_current;
    else
        v2 = v3->f_near;
    v3->f_near_current = v2;
    v4 = v3->f_far * v3->f_blend * TIM_gf_dt + v3->f_far_current;
    if (v3->f_far > (double)v4)
        v1 = v3->f_far * v3->f_blend * TIM_gf_dt + v3->f_far_current;
    else
        v1 = v3->f_far;
    v3->f_far_current = v1;
    AI_EvalFunc_LightSetNearFar_C(AI_gpst_CurrentGameObject, v3->f_near_current, v3->f_far_current);
    AI_EvalFunc_LightSetColor_C(AI_gpst_CurrentGameObject, v3->i_color);
    if (v3->f_far == v3->f_far_current)
    {
        AI_EvalFunc_LightSetNearFar_C(AI_gpst_CurrentGameObject, v3->f_near, v3->f_far);
        AI_EvalFunc_LightSetColor_C(AI_gpst_CurrentGameObject, v3->i_color);
        v3->f_blend = v3->f_blend2;
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Light_PAF_lp_FarUp_FarDown_Light_PAF_C_Light_mode_FarDown);
    }
    return 1000;
}

int Light_PAF_lp_FarUp_ColorDown()
{
    int result; // eax
    float a3; // [esp+4h] [ebp-40h]
    float v2; // [esp+8h] [ebp-3Ch]
    float v3; // [esp+Ch] [ebp-38h]
    tScriptStruct_LightPAF* v4; // [esp+10h] [ebp-34h]
    float v5; // [esp+18h] [ebp-2Ch]
    float v6; // [esp+20h] [ebp-24h]
    int v7; // [esp+40h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_LightPAF*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v6 = v4->f_blend * v4->f_near * TIM_gf_dt + v4->f_near_current;
    if (v4->f_near > (double)v6)
        v3 = v4->f_blend * v4->f_near * TIM_gf_dt + v4->f_near_current;
    else
        v3 = v4->f_near;
    v4->f_near_current = v3;
    v5 = v4->f_far * v4->f_blend * TIM_gf_dt + v4->f_far_current;
    if (v4->f_far > (double)v5)
        v2 = v4->f_far * v4->f_blend * TIM_gf_dt + v4->f_far_current;
    else
        v2 = v4->f_far;
    v4->f_far_current = v2;
    AI_EvalFunc_LightSetNearFar_C(AI_gpst_CurrentGameObject, v4->f_near_current, v4->f_far_current);
    AI_EvalFunc_LightSetColor_C(AI_gpst_CurrentGameObject, v4->i_color);
    if (v4->f_far != v4->f_far_current
        || (a3 = v4->f_blend2 * TIM_gf_dt,
            v4->i_color = AI_EvalFunc_COLORBlend_C(v4->i_color, 0, a3),
            AI_EvalFunc_LightSetColor_C(AI_gpst_CurrentGameObject, v4->i_color),
            v4->i_color)
        || (AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1),
            v7 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject),
            v7 == 1000))
    {
        result = 1000;
    }
    else
    {
        result = v7;
    }
    return result;
}

int Light_PAF_C_Light_mode_FarDown()
{
    int result; // eax
    float v1; // [esp+4h] [ebp-30h]
    float v2; // [esp+8h] [ebp-2Ch]
    tScriptStruct_LightPAF* v3; // [esp+Ch] [ebp-28h]
    float v4; // [esp+10h] [ebp-24h]
    float v5; // [esp+14h] [ebp-20h]
    int v6; // [esp+24h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_LightPAF*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v5 = v3->f_near - v3->f_blend * TIM_gf_dt;
    if (v5 > 0.0)
        v2 = v3->f_near - v3->f_blend * TIM_gf_dt;
    else
        v2 = 0.0;
    v3->f_near = v2;
    v4 = v3->f_far - v3->f_blend * TIM_gf_dt;
    if (v4 > 0.0)
        v1 = v3->f_far - v3->f_blend * TIM_gf_dt;
    else
        v1 = 0.0;
    v3->f_far = v1;
    AI_EvalFunc_LightSetNearFar_C(AI_gpst_CurrentGameObject, v3->f_near, v3->f_far);
    AI_EvalFunc_LightSetColor_C(AI_gpst_CurrentGameObject, v3->i_color);
    if (!AI_EvalFunc_MATHFloatNullEps_C(v3->f_far)
        || (AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1),
            v6 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject),
            v6 == 1000))
    {
        result = 1000;
    }
    else
    {
        result = v6;
    }
    return result;
}

//=======================================

int RLI_Variator_RLI_Variator_init()
{
    tScriptStruct_RLIVariator* v1; // [esp+0h] [ebp-4Ch]
    tGameObjectMain* v2; // [esp+8h] [ebp-44h]
    tGameObjectMain* v3; // [esp+10h] [ebp-3Ch]
    tGameObjectMain* v4; // [esp+20h] [ebp-2Ch]
    tGameObjectMain* v5; // [esp+24h] [ebp-28h]
    int v6; // [esp+38h] [ebp-14h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_RLIVariator*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_RLI_Variator_RLI_Variator_init_RLI_Variator_RLI_Variator_comportement = (int)AI_ul_RealResolveOneRefFunction(0x720040AAu);
        fct_RLI_Variator_RLI_Variator_init_RLI_Variator_RLI_Variator_exec_before_display = (int)AI_ul_RealResolveOneRefFunction(0x720055BAu);
        fct_RLI_Variator_RLI_Variator_init_RLI_Variator_RLI_Variator_exec_after_display = (int)AI_ul_RealResolveOneRefFunction(0x720055BBu);
    }
    if (v1->des_paf_sensitive)
        AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x8000, 0);
    AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 2, 0);
    v5 = (tGameObjectMain*)v1->des_sound_location;
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
    if (!v4)
        v1->des_sound_location = (int)AI_gpst_CurrentGameObject;
    v1->i_flag_ON = 1;
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x100, 0);
    v1->c_ambiant_off = AI_EvalFunc_COLORBlend_C(v1->c_ambiant_on, 0, 0.60000002f);
    v1->c_ambiant2_off = AI_EvalFunc_COLORBlend_C(v1->c_ambiant2_on, 0, 0.60000002f);
    v1->des_f_light_trans_vib = v1->des_f_light_trans_vib / 2.0f;
    v1->i_nb_halo_simple = AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)v1->des_o_halo_simple, 10, 0);
    if (v1->i_nb_halo_simple == -1)
        v1->i_nb_halo_simple = 10;
    if (v1->des_global_RLI)
    {
        v6 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_RLI_Variator_RLI_Variator_init_RLI_Variator_RLI_Variator_comportement);
        if (v6 != 1000)
            return v6;
    }
    else
    {
        v3 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v3 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v3,
                0,
                (tAIFunction*)fct_RLI_Variator_RLI_Variator_init_RLI_Variator_RLI_Variator_exec_before_display);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                0,
                (tAIFunction*)fct_RLI_Variator_RLI_Variator_init_RLI_Variator_RLI_Variator_exec_before_display);
        }
        v2 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v2 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v2,
                1,
                (tAIFunction*)fct_RLI_Variator_RLI_Variator_init_RLI_Variator_RLI_Variator_exec_after_display);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                1,
                (tAIFunction*)fct_RLI_Variator_RLI_Variator_init_RLI_Variator_RLI_Variator_exec_after_display);
        }
        AI_EvalFunc_AITrackCurStop_C();
    }
    return 1000;
}

int RLI_Variator_RLI_Variator_comportement()
{
    tScriptStruct_RLIVariator* v1; // [esp+0h] [ebp-24h]
    tMessage* Src; // [esp+Ch] [ebp-18h]
    int v3; // [esp+10h] [ebp-14h]
    tMessage* v4; // [esp+18h] [ebp-Ch]
    int v5; // [esp+20h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_RLIVariator*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_RLI_Variator_RLI_Variator_comportement_RLI_Variator_RLI_Variator_exec_mode_stealth = (int)AI_ul_RealResolveOneRefFunction(0x72005F2Eu);
        fct_RLI_Variator_RLI_Variator_comportement_RLI_Variator_RLI_Variator_exec_mode_standard = (int)AI_ul_RealResolveOneRefFunction(0x72005F2Du);
    }
    if (v1->des_paf_sensitive)
    {
        Src = AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v1->m_msg, Src, sizeof(v1->m_msg));
        while (v1->m_msg.msgID)
        {
            if (v1->m_msg.msgID == 37)
                AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
            v4 = AI_EvalFunc_MSGGet_C_CURRENT();
            memcpy(&v1->m_msg, v4, sizeof(v1->m_msg));
        }
    }
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
    {
        if (!v1->i_flag_ON)
        {
            v1->f_time_activation = TIM_gf_MainClock - TIM_gf_LockTime;
            v1->i_flag_ON = 1;
        }
    }
    else if (v1->i_flag_ON)
    {
        v1->f_time_activation = TIM_gf_MainClock - TIM_gf_LockTime;
        v1->i_flag_ON = 0;
    }
    if (v1->des_stealth_mode)
    {
        v3 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_RLI_Variator_RLI_Variator_comportement_RLI_Variator_RLI_Variator_exec_mode_stealth);
        if (v3 != 1000)
            return v3;
    }
    else
    {
        v5 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_RLI_Variator_RLI_Variator_comportement_RLI_Variator_RLI_Variator_exec_mode_standard);
        if (v5 != 1000)
            return v5;
    }
    return 1000;
}

int RLI_Variator_RLI_Variator_exec_after_display()
{
    tScriptStruct_RLIVariator* v1; // [esp+4h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_RLIVariator*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    AI_EvalFunc_WORColorAmbiantSet_C(0, v1->i_backup_ambiant);
    AI_EvalFunc_WORColorAmbiant2Set_C(0, v1->i_backup_ambiant2);
    AI_EvalFunc_LightSetRLIFactor_C(v1->f_backup_RLI_Factor);
    AI_EvalFunc_LightSetRLIDestColor_C(v1->c_backup_RLI_DestColor);
    return 1000;
}