#include "ai_compiledscripts_inc.h"

//SFX_eclaire_se_go
int fct_SFX_eclaire_se_go_SFX_eclaire_se_meurt;

int SFX_eclaire_se_adisplay()
{
    tScriptStruct_SFXEclaire* v1; // [esp+4h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXEclaire*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    AI_EvalFunc_LightSetRLIFactor_C(v1->old_factor);
    AI_EvalFunc_LightSetRLIDestColor_C(v1->old_color);
    return 1000;
}

int SFX_eclaire_se_meurt()
{
    int result; // eax
    tScriptStruct_SFXEclaire* v1; // [esp+0h] [ebp-10h]
    int v2; // [esp+8h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXEclaire*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v1->i_meurt = 1;
    v1->f_transparency_coef = TIM_gf_dt + v1->f_transparency_coef;
    if (v1->f_transparency_coef <= 1.0
        || (v1->f_transparency_coef = 1.0, v2 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject), v2 == 1000))
    {
        result = 1000;
    }
    else
    {
        result = v2;
    }
    return result;
}

int SFX_eclaire_se_orient()
{
    tScriptStruct_SFXEclaire* v1; // [esp+0h] [ebp-44h]
    tGameObjectMain* v2; // [esp+10h] [ebp-34h]
    Vec3 v3; // [esp+14h] [ebp-30h] BYREF
    tGameObjectMain* v4; // [esp+20h] [ebp-24h]
    Vec3 a3; // [esp+28h] [ebp-1Ch] BYREF
    Vec3 a1; // [esp+34h] [ebp-10h] BYREF
    tScriptStruct_SFXEclaire* v7; // [esp+40h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXEclaire*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v7 = v1;
    v4 = AI_EvalFunc_VIEWGetObject_C(0);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a1);
    v2 = v4;
    if (v4)
    {
        if ((unsigned int)v4 <= 2)
            v2 = AI_MainActor[(uint32_t)v4 - 1];
        AI_EvalFunc_OBJ_SightGet_C(v2, &a3);
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C(0, &a3);
    }
    v3.x = -a3.x;
    v3.y = -a3.y;
    v3.z = -a3.z;
    AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a1, &v3);
    return 1000;
}

int SFX_eclaire_se_bdisplay()
{
    tScriptStruct_SFXEclaire* v1; // [esp+4h] [ebp-50h]
    tGameObjectMain* v2; // [esp+14h] [ebp-40h]
    Vec3 v3; // [esp+1Ch] [ebp-38h] BYREF
    tGameObjectMain* v4; // [esp+28h] [ebp-2Ch]
    Vec3 a3; // [esp+34h] [ebp-20h] BYREF
    Vec3 v6; // [esp+40h] [ebp-14h] BYREF
    tScriptStruct_SFXEclaire* v7; // [esp+4Ch] [ebp-8h]
    int a1; // [esp+50h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXEclaire*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v7 = v1;
    v4 = AI_EvalFunc_VIEWGetObject_C(0);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v6);
    v2 = v4;
    if (v4)
    {
        if ((unsigned int)v4 <= 2)
            v2 = AI_MainActor[(uint32_t)v4 - 1];
        AI_EvalFunc_OBJ_SightGet_C(v2, &a3);
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C(0, &a3);
    }
    v3.x = -a3.x;
    v3.y = -a3.y;
    v3.z = -a3.z;
    AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v6, &v3);
    v7->old_factor = AI_EvalFunc_LightGetRLIFactor_C();
    v7->old_color = AI_EvalFunc_LightGetRLIDestColor_C();
    if (v7->i_meurt)
    {
        AI_EvalFunc_LightSetRLIFactor_C(1.0);
        a1 = AI_EvalFunc_COLORBlend_C(0, -1, v7->f_transparency_coef);
        AI_EvalFunc_LightSetRLIDestColor_C(a1);
    }
    return 1000;
}

int SFX_eclaire_se_go()
{
    int result; // eax
    float a3; // [esp+0h] [ebp-44h]
    float a3a; // [esp+0h] [ebp-44h]
    float a3b; // [esp+0h] [ebp-44h]
    float a3c; // [esp+0h] [ebp-44h]
    float a3d; // [esp+0h] [ebp-44h]
    tScriptStruct_SFXEclaire* v6; // [esp+8h] [ebp-3Ch]
    float v7; // [esp+Ch] [ebp-38h]
    float v9; // [esp+14h] [ebp-30h]
    float v10; // [esp+14h] [ebp-30h]
    float v11; // [esp+18h] [ebp-2Ch]
    int v12; // [esp+24h] [ebp-20h]
    float v13; // [esp+28h] [ebp-1Ch]
    int a1; // [esp+30h] [ebp-14h]
    int a2; // [esp+34h] [ebp-10h]
    float v16; // [esp+3Ch] [ebp-8h]
    float v17; // [esp+40h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_SFXEclaire*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_eclaire_se_go_SFX_eclaire_se_meurt = (int)AI_ul_RealResolveOneRefFunction(0x3D002D37u);
    }
    switch ((unsigned int)*AI_gppst_CurrentJumpNode)
    {
    case 2u:
        goto LABEL_10;
    case 4u:
        goto LABEL_13;
    case 6u:
        goto LABEL_16;
    case 8u:
        goto LABEL_19;
    case 10u:
        goto LABEL_22;
    default:
        if (!AI_EvalFunc_OBJ_CustomBitsTest_C(AI_gpst_CurrentGameObject, 256))
        {
            a2 = AI_EvalFunc_MATHRandInt_C(0, 3);
            a1 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(AI_gpst_CurrentGameObject, a2, -1.0, 0.89999998f, 1.3f);            
            v7 = AI_EvalFunc_MATHRandFloat_C(0.5, 1.0);
            v9 = SND_f_GetVol(a1) * v7;
            SND_SetVol(a1, v9);
        }
        v6->angle = AI_EvalFunc_MATHRandFloat_C(-0.5, 0.5);
        AI_EvalFunc_OBJRotateLocalZ_c(AI_gpst_CurrentGameObject, v6->angle);
        AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, 70.0);
        AI_gpst_CurrentInstance->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -0.079999998f;
    LABEL_10:
        if (AI_EvalKeyword_During_C())
        {
            v16 = AI_EvalFunc_MATHRandFloat_C(-0.1f, 0.1f);
            a3 = v16 + 1.0f;
            AI_EvalFunc_OBJ_SetChannelProg_C(AI_gpst_CurrentGameObject, 0, a3);
            *AI_gppst_CurrentJumpNode = (tAINode*)2;
            result = 0;
        }
        else
        {
            AI_gpst_CurrentInstance->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -0.079999998f;
        LABEL_13:
            if (AI_EvalKeyword_During_C())
            {
                v10 = AI_EvalFunc_MATHRandFloat_C(-0.1f, 0.1f);
                a3a = v10 + 2.0f;
                AI_EvalFunc_OBJ_SetChannelProg_C(AI_gpst_CurrentGameObject, 0, a3a);
                *AI_gppst_CurrentJumpNode = (tAINode*)4;
                result = 0;
            }
            else
            {
                AI_gpst_CurrentInstance->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -0.079999998f;
            LABEL_16:
                if (AI_EvalKeyword_During_C())
                {
                    v13 = AI_EvalFunc_MATHRandFloat_C(-0.1f, 0.1f);
                    a3b = v13 + 3.0f;
                    AI_EvalFunc_OBJ_SetChannelProg_C(AI_gpst_CurrentGameObject, 0, a3b);
                    *AI_gppst_CurrentJumpNode = (tAINode*)6;
                    result = 0;
                }
                else
                {
                    AI_gpst_CurrentInstance->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -0.079999998f;
                LABEL_19:
                    if (AI_EvalKeyword_During_C())
                    {
                        v17 = AI_EvalFunc_MATHRandFloat_C(-0.1f, 0.1f);
                        a3c = v17 + 4.0f;
                        AI_EvalFunc_OBJ_SetChannelProg_C(AI_gpst_CurrentGameObject, 0, a3c);
                        *AI_gppst_CurrentJumpNode = (tAINode*)8;
                        result = 0;
                    }
                    else
                    {
                        AI_gpst_CurrentInstance->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -0.16f;
                    LABEL_22:
                        if (AI_EvalKeyword_During_C())
                        {
                            v11 = AI_EvalFunc_MATHRandFloat_C(-0.2f, 0.2f);
                            a3d = v11 + 5.0f;
                            AI_EvalFunc_OBJ_SetChannelProg_C(AI_gpst_CurrentGameObject, 0, a3d);
                            *AI_gppst_CurrentJumpNode = (tAINode*)10;
                            result = 0;
                        }
                        else
                        {
                            v12 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_eclaire_se_go_SFX_eclaire_se_meurt);
                            if (v12 == 1000)
                                result = 1000;
                            else
                                result = v12;
                        }
                    }
                }
            }
        }
        return result;
    }
}