#include "ai_compiledscripts_inc.h"

//sfx_pas_poussiere_spp_bv
int fct_sfx_pas_poussiere_spp_bv_sfx_pas_poussiere_spp_exec_test;

//sfx_pas_poussiere_spp_go
int fct_sfx_pas_poussiere_spp_go_sfx_pas_poussiere_spp_bv;

int sfx_pas_poussiere_spp_bv()
{
    int result; // eax
    tScriptStruct_SFXPasPoussiere* v1; // [esp+0h] [ebp-28h]
    tGameObjectMain* v2; // [esp+4h] [ebp-24h]
    tGameObjectMain* v3; // [esp+8h] [ebp-20h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-1Ch]
    tGameObjectMain* v5; // [esp+10h] [ebp-18h]
    int v6; // [esp+14h] [ebp-14h]
    int v7; // [esp+20h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXPasPoussiere*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_sfx_pas_poussiere_spp_bv_sfx_pas_poussiere_spp_exec_test = (int)AI_ul_RealResolveOneRefFunction(0x3D002999u);
    }
    v1->i_actor = 0;
    v1->o_test_obj = (int)*AI_MainActor;
    v5 = (tGameObjectMain*)v1->o_test_obj;
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
        v7 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_sfx_pas_poussiere_spp_bv_sfx_pas_poussiere_spp_exec_test);
        if (v7 != 1000)
            return v7;
    }
    v1->i_actor = 1;
    v1->o_test_obj = (int)AI_MainActor[1];
    v3 = (tGameObjectMain*)v1->o_test_obj;
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        v2 = v3;
    }
    else
    {
        v2 = 0;
    }
    if (!v2
        || (v6 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_sfx_pas_poussiere_spp_bv_sfx_pas_poussiere_spp_exec_test),
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

int sfx_pas_poussiere_spp_ar()
{
    float v1; // [esp+8h] [ebp-64h]
    tScriptStruct_SFXPasPoussiere* v2; // [esp+10h] [ebp-5Ch]
    double v3; // [esp+14h] [ebp-58h]
    float v4; // [esp+1Ch] [ebp-50h]
    tGameObjectMain* v5; // [esp+24h] [ebp-48h]
    tGameObjectMain* v6; // [esp+28h] [ebp-44h]
    tGameObjectMain* v7; // [esp+2Ch] [ebp-40h]
    float v8; // [esp+30h] [ebp-3Ch]
    tGameObjectMain* v9; // [esp+34h] [ebp-38h]
    int v10; // [esp+44h] [ebp-28h]
    int color; // [esp+4Ch] [ebp-20h]
    Vec3 a3; // [esp+54h] [ebp-18h] BYREF
    tScriptStruct_SFXPasPoussiere* v13; // [esp+64h] [ebp-8h]
    int v14; // [esp+68h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SFXPasPoussiere*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v13 = v2;
    if (v2->i_generated)
    {
        if (!AI_EvalFunc_TIMEElapsed_C(v13->f_time_gen, 0.30000001f))
        {
            v9 = AI_MainActor[v13->i_actor];
            v7 = v9;
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v7 = AI_MainActor[(uint32_t)v9 - 1];
                v6 = v7;
            }
            else
            {
                v6 = 0;
            }
            if (v6)
            {
                v5 = AI_MainActor[v13->i_actor];
                if (v9)
                {
                    if ((unsigned int)v9 <= 2)
                        v5 = AI_MainActor[(uint32_t)v9 - 1];
                    AI_EvalFunc_OBJGetPos_C(v5, &a3);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &a3);
                }
                AI_EvalFunc_OBJSetPos_C_CURRENT(&a3);
            }
            AI_EvalFunc_PAG_SetGeneratorFreq_C(AI_gpst_CurrentGameObject, 1, 5.0);
            v13->done = 1;
        }
        v4 = 2.0f * TIM_gf_dt;
        if (v4 < 0.0)
            v4 = 0.0;
        if (v4 > 1.0)
            v4 = 1.0;
        v8 = 0.0f * v4 + v13->des_f_alpha * (1.0f - v4);
        v13->des_f_alpha = v8;
        if (v13->des_f_alpha < 0.0099999998f)
        {
            v10 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v10 != 1000)
                return v10;
        }
        v1 = (float)(int)((unsigned int)(v13->des_f_alpha * 255.0) << 24);
        v3 = v1 + MATH_gd_magicDoubleForRound;
        v14 = AI_EvalFunc_WORColorAmbiantGet_C(0);
        color = (*(uint32_t *)&v3) + (AI_EvalFunc_COLORBlend_C(v14, -1, 0.2f) & 0xFFFFFF);
        AI_EvalFunc_PAG_SetColor_C(AI_gpst_CurrentGameObject, color);
    }
    return 1000;
}

int sfx_pas_poussiere_spp_go()
{
    tScriptStruct_SFXPasPoussiere* v1; // [esp+0h] [ebp-10h]
    int v2; // [esp+4h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXPasPoussiere*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_sfx_pas_poussiere_spp_go_sfx_pas_poussiere_spp_bv = (int)AI_ul_RealResolveOneRefFunction(0x3D00298Bu);
    }
    if (v1->i_generated)
    {
        v1->f_time_gen = TIM_gf_MainClock - TIM_gf_LockTime;
        AI_EvalFunc_AITrackCurStop_C();
    }
    else
    {
        v2 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_sfx_pas_poussiere_spp_go_sfx_pas_poussiere_spp_bv);
        if (v2 != 1000)
            return v2;
    }
    return 1000;
}

int sfx_pas_poussiere_spp_exec_test()
{
    BOOL v0; // eax
    int v1; // eax
    double v2; // st7
    tScriptStruct_SFXPasPoussiere* v4; // [esp+8h] [ebp-64h]
    tAI* v5; // [esp+Ch] [ebp-60h]
    tGameObjectMain* v6; // [esp+10h] [ebp-5Ch]
    tGameObjectMain* v7; // [esp+14h] [ebp-58h]
    tAI* v8; // [esp+18h] [ebp-54h]
    tGameObjectMain* v9; // [esp+1Ch] [ebp-50h]
    tGameObjectMain* v10; // [esp+20h] [ebp-4Ch]
    tGameObjectMain* v11; // [esp+28h] [ebp-44h]
    tGameObjectMain* v12; // [esp+30h] [ebp-3Ch]
    tGameObjectMain* v13; // [esp+38h] [ebp-34h]
    float v14; // [esp+40h] [ebp-2Ch]
    tAI* v15; // [esp+44h] [ebp-28h]
    Vec3 pos; // [esp+4Ch] [ebp-20h] BYREF
    tScriptStruct_SFXPasPoussiere* v18; // [esp+60h] [ebp-Ch]
    int v19; // [esp+64h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_SFXPasPoussiere*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v18 = v4;
    v19 = 0;
    v4->i_ready = 0;
    if (v18->des_i_on_bv)
    {
        v13 = (tGameObjectMain*)v18->o_test_obj;
        if (v13)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            v0 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, v13);
        }
        else
        {
            v0 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, 0);
        }
        if (v0)
            v18->i_ready = 1;
    }
    v12 = (tGameObjectMain*)v18->o_test_obj;
    if (v12)
    {
        if ((unsigned int)v12 <= 2)
            v12 = AI_MainActor[(uint32_t)v12 - 1];
        v1 = AI_EvalFunc_COLGMSoundGet_C(v12, 0x40);
    }
    else
    {
        v1 = AI_EvalFunc_COLGMSoundGet_C(0, 0x40);
    }
    v19 = v1;
    if (v18->des_i_on_cbc && !v19)
        v18->i_ready = 1;
    if (v18->des_i_on_terre && v19 == 5)
        v18->i_ready = 1;
    if (v18->i_ready)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v18->f_time_gen, 0.5))
        {
            v11 = (tGameObjectMain*)v18->o_test_obj;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v11 = AI_MainActor[(uint32_t)v11 - 1];
                v2 = AI_EvalFunc_DYNSpeedGet_C(v11);
            }
            else
            {
                v2 = AI_EvalFunc_DYNSpeedGet_C(0);
            }
            v14 = (float)v2;
            if (v14 > 2.0f)
            {
                v18->f_time_gen = TIM_gf_MainClock - TIM_gf_LockTime;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
                v15 = (tAI*)AI_EvalFunc_OBJDuplicate_C(AI_gpst_CurrentGameObject, &pos);
                if (v15 == WOR_gst_Universe.univAIInstance)
                {
                    v8 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v10 = (tGameObjectMain*)v15;
                    if (v15)
                    {
                        if ((unsigned int)v15 <= 2)
                            v10 = AI_MainActor[(uint32_t)v15 - 1];
                        v9 = v10;
                    }
                    else
                    {
                        v9 = 0;
                    }
                    v8 = v9->extended->AIInstance;
                }
                ((tScriptStruct_SFXPasPoussiere*)(v8->instanceVarDataCurrent))->i_generated = 1;
                if (v15 == WOR_gst_Universe.univAIInstance)
                {
                    v5 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v7 = (tGameObjectMain*)v15;
                    if (v15)
                    {
                        if ((unsigned int)v15 <= 2)
                            v7 = AI_MainActor[(uint32_t)v15 - 1];
                        v6 = v7;
                    }
                    else
                    {
                        v6 = 0;
                    }
                    v5 = v6->extended->AIInstance;
                }
                ((tScriptStruct_SFXPasPoussiere*)(v5->instanceVarDataCurrent))->i_actor = v18->i_actor;
            }
        }
    }
    return 1000;
}