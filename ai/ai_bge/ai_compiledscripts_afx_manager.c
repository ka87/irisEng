#include "ai_compiledscripts_inc.h"

int afx_manager_afx_loop()
{
    float v1; // [esp+8h] [ebp-3Ch]
    float v2; // [esp+Ch] [ebp-38h]
    float v3; // [esp+10h] [ebp-34h]
    float v4; // [esp+14h] [ebp-30h]
    float v5; // [esp+18h] [ebp-2Ch]
    float v6; // [esp+1Ch] [ebp-28h]
    tScriptStruct_AFXManager* v7; // [esp+20h] [ebp-24h]
    float v8; // [esp+24h] [ebp-20h]
    float v9; // [esp+24h] [ebp-20h]
    float v10; // [esp+24h] [ebp-20h]
    float v11; // [esp+24h] [ebp-20h]
    float v12; // [esp+24h] [ebp-20h]
    float v13; // [esp+24h] [ebp-20h]
    float v14; // [esp+28h] [ebp-1Ch]
    float v15; // [esp+30h] [ebp-14h]
    float v16; // [esp+38h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_AFXManager*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    if (v7->i_remanence_on)
    {
        v15 = TIM_gf_dt;
        v6 = (float)fabs(v7->f_remanence_fin - v7->f_remanence_deb);
        v8 = v15 * v6;
        v5 = (float)v7->i_remanence_delay;
        v9 = v8 / (v5 / 1000.0f);
        if (v7->f_remanence_deb >= (double)v7->f_remanence_fin)
        {
            v7->f_remanence_cur = v7->f_remanence_cur - v9;
            if (v7->f_remanence_fin > (double)v7->f_remanence_cur)
                v7->f_remanence_cur = v7->f_remanence_fin;
        }
        else
        {
            v7->f_remanence_cur = v9 + v7->f_remanence_cur;
            if (v7->f_remanence_fin < (double)v7->f_remanence_cur)
                v7->f_remanence_cur = v7->f_remanence_fin;
        }
        AI_EvalFunc_Gsp_AE_MASTER_C(7, 1, (unsigned __int64)v7->f_remanence_cur, v7->f_remanence_cur);
    }
    else
    {
        AI_EvalFunc_Gsp_AE_MASTER_C(7, 0, 0, 0.0);
    }
    if (v7->i_Motion_blur_on)
    {
        v16 = TIM_gf_dt;
        v4 = (float)fabs(v7->f_Motion_blur_fin - v7->f_Motion_blur_deb);
        v10 = v16 * v4;
        v3 = (float)v7->i_Motion_blur_delay;
        v11 = v10 / (v3 / 1000.0f);
        if (v7->f_Motion_blur_deb >= (double)v7->f_Motion_blur_fin)
        {
            v7->f_Motion_blur_cur = v7->f_Motion_blur_cur - v11;
            if (v7->f_Motion_blur_fin > (double)v7->f_Motion_blur_cur)
                v7->f_Motion_blur_cur = v7->f_Motion_blur_fin;
        }
        else
        {
            v7->f_Motion_blur_cur = v11 + v7->f_Motion_blur_cur;
            if (v7->f_Motion_blur_fin < (double)v7->f_Motion_blur_cur)
                v7->f_Motion_blur_cur = v7->f_Motion_blur_fin;
        }
        AI_EvalFunc_Gsp_AE_MASTER_C(1, 1, (unsigned __int64)v7->f_Motion_blur_cur, v7->f_Motion_blur_cur);
    }
    else
    {
        AI_EvalFunc_Gsp_AE_MASTER_C(1, 0, 0, 0.0);
    }
    if (v7->i_Depth_Blur_on)
    {
        if (v7->i_Depth_Blur_type)
        {
            AI_EvalFunc_Gsp_AE_MASTER_C(14, 1, 0, v7->f_Depth_Blur_cur_near);
        }
        else
        {
            AI_EvalFunc_Gsp_AE_MASTER_C(0, 1, 0, v7->f_Depth_Blur_cur_near);
            AI_EvalFunc_Gsp_AE_MASTER_C(0, 1, 1, v7->f_Depth_Blur_cur_far);
        }
    }
    else
    {
        AI_EvalFunc_Gsp_AE_MASTER_C(0, 0, 0, 0.0);
        AI_EvalFunc_Gsp_AE_MASTER_C(0, 0, 1, 0.0);
        AI_EvalFunc_Gsp_AE_MASTER_C(14, 0, 0, v7->f_Depth_Blur_cur_near);
    }
    if (v7->i_Screen_Smooth_on)
    {
        v14 = TIM_gf_dt;
        v2 = (float)fabs(v7->f_Screen_Smooth_fin - v7->f_Screen_Smooth_deb);
        v12 = v14 * v2;
        v1 = (float)v7->i_Screen_Smooth_delay;
        v13 = v12 / (v1 / 1000.0f);
        if (v7->f_Screen_Smooth_deb >= (double)v7->f_Screen_Smooth_fin)
        {
            v7->f_Screen_Smooth_cur = v7->f_Screen_Smooth_cur - v13;
            if (v7->f_Screen_Smooth_fin > (double)v7->f_Screen_Smooth_cur)
                v7->f_Screen_Smooth_cur = v7->f_Screen_Smooth_fin;
        }
        else
        {
            v7->f_Screen_Smooth_cur = v13 + v7->f_Screen_Smooth_cur;
            if (v7->f_Screen_Smooth_fin < (double)v7->f_Screen_Smooth_cur)
                v7->f_Screen_Smooth_cur = v7->f_Screen_Smooth_fin;
        }
        AI_EvalFunc_Gsp_AE_MASTER_C(2, 1, (unsigned __int64)v7->f_Screen_Smooth_cur, v7->f_Screen_Smooth_cur);
    }
    else
    {
        AI_EvalFunc_Gsp_AE_MASTER_C(2, 0, 0, 0.0);
    }
    return 1000;
}

int afx_manager_afx_msg()
{
    int result; // eax
    tScriptStruct_AFXManager* v1; // [esp+0h] [ebp-44h]
    tMessage v2; // [esp+8h] [ebp-3Ch] BYREF
    void* Src; // [esp+3Ch] [ebp-8h]
    tScriptStruct_AFXManager* v4; // [esp+40h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_AFXManager*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v4 = v1;
    if (AI_EvalFunc_MSGGetCount_C(AI_gpst_CurrentGameObject))
    {
        Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v2, Src, sizeof(v2));
        if (v2.msgID == 1)
        {
            if (v2.Int1 <= 0)
            {
                v4->i_remanence_on = 0;
            }
            else
            {
                v4->i_remanence_on = 1;
                v4->f_remanence_deb = v2.f2;
                v4->f_remanence_fin = v2.f1;
                v4->i_remanence_delay = v2.Int1;
                v4->f_remanence_cur = v4->f_remanence_deb;
            }
        }
        if (v2.msgID == 2)
        {
            if (v2.Int1 <= 0)
            {
                v4->i_Motion_blur_on = 0;
            }
            else
            {
                v4->i_Motion_blur_on = 1;
                v4->f_Motion_blur_deb = v2.f2;
                v4->f_Motion_blur_fin = v2.f1;
                v4->i_Motion_blur_delay = v2.Int1;
                v4->f_Motion_blur_cur = v4->f_Motion_blur_deb;
            }
        }
        if (v2.msgID == 3)
        {
            if (v2.f2 <= 0.0 && v2.f1 <= 0.0)
            {
                v4->i_Depth_Blur_on = 0;
            }
            else
            {
                v4->i_Depth_Blur_on = 1;
                v4->f_Depth_Blur_cur_near = v2.f2;
                v4->f_Depth_Blur_cur_far = v2.f1;
                v4->i_Depth_Blur_type = v2.Int1;
            }
        }
        if (v2.msgID == 3)
        {
            if (v2.Int1 <= 0)
            {
                v4->i_Screen_Smooth_on = 0;
            }
            else
            {
                v4->i_Screen_Smooth_on = 1;
                v4->f_Screen_Smooth_deb = v2.f2;
                v4->f_Screen_Smooth_fin = v2.f1;
                v4->i_Screen_Smooth_delay = v2.Int1;
                v4->f_Screen_Smooth_cur = v4->f_Screen_Smooth_deb;
            }
        }
        result = 1000;
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}