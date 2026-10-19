#include "ai_compiledscripts_inc.h"

//Pilot_Wing_Ring_PWR_init
int fct_Pilot_Wing_Ring_PWR_init_Pilot_Wing_Ring_PWR_exec_before_display;
int fct_Pilot_Wing_Ring_PWR_init_Pilot_Wing_Ring_PWR_exec_after_display;
int fct_Pilot_Wing_Ring_PWR_init_Pilot_Wing_Ring_PWR_comportement;

int Pilot_Wing_Ring_PWR_comportement()
{
    float v1; // [esp+4h] [ebp-164h]
    tScriptStruct_PilotWingRing* v2; // [esp+8h] [ebp-160h]
    tGameObjectMain* v3; // [esp+10h] [ebp-158h]
    tGameObjectMain* v4; // [esp+28h] [ebp-140h]
    float v5; // [esp+30h] [ebp-138h]
    tGameObjectMain* v6; // [esp+40h] [ebp-128h]
    float v7; // [esp+44h] [ebp-124h]
    float v8; // [esp+48h] [ebp-120h]
    float v9; // [esp+4Ch] [ebp-11Ch]
    Vec3 v10; // [esp+50h] [ebp-118h] BYREF
    Vec3 v11; // [esp+5Ch] [ebp-10Ch] BYREF
    Vec3 dest; // [esp+68h] [ebp-100h] BYREF
    float v13; // [esp+74h] [ebp-F4h]
    tGameObjectMain* v14; // [esp+7Ch] [ebp-ECh]
    float v15; // [esp+80h] [ebp-E8h]
    Vec3 v16; // [esp+84h] [ebp-E4h] BYREF
    Vec3 v17; // [esp+90h] [ebp-D8h] BYREF
    tMessage Src; // [esp+A0h] [ebp-C8h] BYREF
    Vec3 v19; // [esp+D4h] [ebp-94h] BYREF
    Vec3 v20; // [esp+E0h] [ebp-88h] BYREF
    Vec3 v21; // [esp+F0h] [ebp-78h] BYREF
    float v22; // [esp+FCh] [ebp-6Ch]
    float v23; // [esp+100h] [ebp-68h]
    float v24; // [esp+104h] [ebp-64h]
    Vec3 v25; // [esp+108h] [ebp-60h] BYREF
    Vec3 v26; // [esp+118h] [ebp-50h] BYREF
    float v27; // [esp+128h] [ebp-40h]
    Vec3 v28; // [esp+12Ch] [ebp-3Ch] BYREF
    Vec3 a3; // [esp+138h] [ebp-30h] BYREF
    tScriptStruct_PilotWingRing* v30; // [esp+144h] [ebp-24h]
    int v31; // [esp+148h] [ebp-20h]
    Vec3 v32; // [esp+15Ch] [ebp-Ch] BYREF

    v25.x = 1.0;
    v25.y = 0.0;
    v25.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PilotWingRing*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v30 = v2;
    v13 = 0.0;
    v14 = 0;
    v27 = 0.0;
    v14 = *AI_MainActor;
    v6 = v14;
    if (v14)
    {
        if ((unsigned int)v14 <= 2)
            v6 = AI_MainActor[(uint32_t)v14 - 1];
        AI_EvalFunc_OBJGetPos_C(v6, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v26);
    MATH_SubstractVec(&dest, &a3, &v26);
    v25 = dest;
    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v11);
    v8 = v11.z * v25.z + v11.y * v25.y + v25.x * v11.x;
    v13 = AI_EvalFunc_MTHFloatSign_C(v8);
    if (v30->f_side == 0.0f)
        v30->f_side = v13;
    v15 = TIM_gf_dt;
    v30->f_zoom = TIM_gf_dt * 160.0f + v30->f_zoom;
    AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, v30->f_zoom);
    v5 = v30->f_zoom - 180.0f;
    if (v5 < 0.0)
        v1 = 0.0;
    else
        v1 = v30->f_zoom - 180.0f;
    v30->f_transparency_coef = v1;
    v30->f_transparency_coef = v30->f_transparency_coef / 20.0f;
    if (v30->f_zoom > 200.0)
    {
        v31 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v31 != 1000)
            return v31;
    }
    if (v30->i_flag_paf)
    {
        if (v30->f_side != v13)
        {
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v16);
            v22 = v16.x * v8;
            v23 = v16.y * v8;
            v24 = v16.z * v8;
            v25.x = v25.x - v22;
            v25.y = v25.y - v23;
            v25.z = v25.z - v24;
            v9 = MATH_VecLen(&v25);
            v27 = v30->f_zoom * 1.1f;
            v7 = v30->f_zoom * 0.55000001f;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v32);
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v21);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v17);
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v20);
            if (v9 > (double)v7 && v9 < (double)v27)
            {
                v30->i_flag_paf = 0;
                v4 = v14;
                if (v14)
                {
                    if ((unsigned int)v14 <= 2)
                        v4 = AI_MainActor[(uint32_t)v14 - 1];
                    AI_EvalFunc_OBJGetPos_C(v4, &v10);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v10);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v19);
                MATH_SubstractVec(&v28, &v10, &v19);
                v25 = v28;
                Src.msgID = 37;
                Src.gao = AI_gpst_CurrentGameObject;
                Src.gao2 = 0;
                Src.Int1 = 393317;
                Src.Int2 = 0;
                Src.v1 = v28;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&Src.v2);
                Src.f2 = 20.0;
                Src.f1 = 1000.0;
                v3 = v14;
                if (v14)
                {
                    if ((unsigned int)v14 <= 2)
                        v3 = AI_MainActor[(uint32_t)v14 - 1];
                    AI_EvalFunc_MSGSend_C(v3, &Src);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &Src);
                }
            }
        }
    }
    v30->f_side = v13;
    return 1000;
}

int Pilot_Wing_Ring_PWR_exec_after_display()
{
    tScriptStruct_PilotWingRing* v1; // [esp+4h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PilotWingRing*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    AI_EvalFunc_LightRLIFactorUsed_C(0);
    AI_EvalFunc_LightSetRLIFactor_C(v1->old_factor);
    AI_EvalFunc_LightSetRLIDestColor_C(v1->old_color);
    AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v1->old_constantcolor0, 0);
    AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v1->old_constantcolor1, 1);
    return 1000;
}

int Pilot_Wing_Ring_PWR_init()
{
    int result; // eax
    tScriptStruct_PilotWingRing* v1; // [esp+0h] [ebp-24h]
    tGameObjectMain* v2; // [esp+8h] [ebp-1Ch]
    tGameObjectMain* v3; // [esp+10h] [ebp-14h]
    int v4; // [esp+18h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PilotWingRing*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Pilot_Wing_Ring_PWR_init_Pilot_Wing_Ring_PWR_exec_before_display = (int)AI_ul_RealResolveOneRefFunction(0x72005FAEu);
        fct_Pilot_Wing_Ring_PWR_init_Pilot_Wing_Ring_PWR_exec_after_display = (int)AI_ul_RealResolveOneRefFunction(0x72005FAFu);
        fct_Pilot_Wing_Ring_PWR_init_Pilot_Wing_Ring_PWR_comportement = (int)AI_ul_RealResolveOneRefFunction(0x72005FA7u);
    }
    AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, 0.1f);
    v1->i_flag_paf = 1;
    v3 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v3 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v3,
            0,
            (tAIFunction*)fct_Pilot_Wing_Ring_PWR_init_Pilot_Wing_Ring_PWR_exec_before_display);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            0,
            (tAIFunction*)fct_Pilot_Wing_Ring_PWR_init_Pilot_Wing_Ring_PWR_exec_before_display);
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
            (tAIFunction*)fct_Pilot_Wing_Ring_PWR_init_Pilot_Wing_Ring_PWR_exec_after_display);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            1,
            (tAIFunction*)fct_Pilot_Wing_Ring_PWR_init_Pilot_Wing_Ring_PWR_exec_after_display);
    }
    v4 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Pilot_Wing_Ring_PWR_init_Pilot_Wing_Ring_PWR_comportement);
    if (v4 == 1000)
        result = 1000;
    else
        result = v4;
    return result;
}

int Pilot_Wing_Ring_PWR_exec_before_display()
{
    tScriptStruct_PilotWingRing* v1; // [esp+4h] [ebp-24h]
    int a2; // [esp+14h] [ebp-14h]
    tScriptStruct_PilotWingRing* v3; // [esp+20h] [ebp-8h]
    int v4; // [esp+24h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PilotWingRing*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v3 = v1;
    v1->old_factor = AI_EvalFunc_LightGetRLIFactor_C();
    v1->old_color = AI_EvalFunc_LightGetRLIDestColor_C();
    v1->old_constantcolor0 = AI_EvalFunc_MATGetConstant_C(AI_gpst_CurrentGameObject, 0);
    v1->old_constantcolor1 = AI_EvalFunc_MATGetConstant_C(AI_gpst_CurrentGameObject, 1u);
    AI_EvalFunc_LightRLIFactorUsed_C(1);
    AI_EvalFunc_LightSetRLIFactor_C(v1->f_transparency_coef);
    AI_EvalFunc_LightSetRLIDestColor_C(-1);
    a2 = AI_EvalFunc_COLORBlend_C(v3->old_constantcolor0, 0, v3->f_transparency_coef);
    AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, a2, 0);
    v4 = AI_EvalFunc_COLORBlend_C(v3->old_constantcolor1, 0, v3->f_transparency_coef);
    AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v4, 1);
    return 1000;
}