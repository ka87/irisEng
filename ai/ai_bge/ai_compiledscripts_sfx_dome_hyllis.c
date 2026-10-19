#include "ai_compiledscripts_inc.h"

//SFX_Dome_Hyllis_sdh_init
int SFX_Dome_Hyllis_sdh_init_univVars;
int SFX_Dome_Hyllis_sdh_init_univVars_Flag_speciaux_sauve;
int fct_SFX_Dome_Hyllis_sdh_init_SFX_Dome_Hyllis_sdh_bdisplay;
int fct_SFX_Dome_Hyllis_sdh_init_SFX_Dome_Hyllis_sdh_adisplay;
int fct_SFX_Dome_Hyllis_sdh_init_SFX_Dome_Hyllis_sdh_reflex;

//SFX_Dome_Hyllis_sdh_reflex
int SFX_Dome_Hyllis_sdh_reflex_univVars;
int SFX_Dome_Hyllis_sdh_reflex_univVars_Flag_speciaux_sauve;

int SFX_Dome_Hyllis_sdh_init()
{
    int result; // eax
    tScriptStruct_SFXDomeHyllis* v1; // [esp+0h] [ebp-34h]
    tGameObjectMain* v2; // [esp+8h] [ebp-2Ch]
    tGameObjectMain* v3; // [esp+10h] [ebp-24h]
    int v4; // [esp+18h] [ebp-1Ch]
    int v5; // [esp+28h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXDomeHyllis*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SFX_Dome_Hyllis_sdh_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &SFX_Dome_Hyllis_sdh_init_univVars_Flag_speciaux_sauve,
            (void*)SFX_Dome_Hyllis_sdh_init_univVars,
            4488);
        fct_SFX_Dome_Hyllis_sdh_init_SFX_Dome_Hyllis_sdh_bdisplay = (int)AI_ul_RealResolveOneRefFunction(0x3D002DFEu);
        fct_SFX_Dome_Hyllis_sdh_init_SFX_Dome_Hyllis_sdh_adisplay = (int)AI_ul_RealResolveOneRefFunction(0x3D002DFDu);
        fct_SFX_Dome_Hyllis_sdh_init_SFX_Dome_Hyllis_sdh_reflex = (int)AI_ul_RealResolveOneRefFunction(0x3D002DFCu);
    }
    if (v1->des_i_sf_jesuis_plus_la)
    {
        if (v1->des_i_sf_jesuis_plus_la > 0
            && AI_EvalFunc_ARRBitTest_C(
                (int*)(SFX_Dome_Hyllis_sdh_init_univVars_Flag_speciaux_sauve + 4),
                v1->des_i_sf_jesuis_plus_la))
        {
            v5 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v5 != 1000)
                return v5;
        }
    }
    v3 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v3 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v3,
            0,
            (tAIFunction*)fct_SFX_Dome_Hyllis_sdh_init_SFX_Dome_Hyllis_sdh_bdisplay);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            0,
            (tAIFunction*)fct_SFX_Dome_Hyllis_sdh_init_SFX_Dome_Hyllis_sdh_bdisplay);
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
            (tAIFunction*)fct_SFX_Dome_Hyllis_sdh_init_SFX_Dome_Hyllis_sdh_adisplay);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            1,
            (tAIFunction*)fct_SFX_Dome_Hyllis_sdh_init_SFX_Dome_Hyllis_sdh_adisplay);
    }
    v4 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_Dome_Hyllis_sdh_init_SFX_Dome_Hyllis_sdh_reflex);
    if (v4 == 1000)
        result = 1000;
    else
        result = v4;
    return result;
}

int SFX_Dome_Hyllis_sdh_adisplay()
{
    tScriptStruct_SFXDomeHyllis* v1; // [esp+4h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXDomeHyllis*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    AI_EvalFunc_LightRLIFactorUsed_C(0);
    AI_EvalFunc_LightSetRLIFactor_C(v1->old_factor);
    AI_EvalFunc_LightSetRLIDestColor_C(v1->old_color);
    return 1000;
}

int SFX_Dome_Hyllis_sdh_bdisplay()
{
    tScriptStruct_SFXDomeHyllis* v1; // [esp+0h] [ebp-14h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXDomeHyllis*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v1->old_factor = AI_EvalFunc_LightGetRLIFactor_C();
    v1->old_color = AI_EvalFunc_LightGetRLIDestColor_C();
    AI_EvalFunc_LightRLIFactorUsed_C(1);
    AI_EvalFunc_LightSetRLIFactor_C(v1->f_transparency);
    AI_EvalFunc_LightSetRLIDestColor_C(-1);
    return 1000;
}

int SFX_Dome_Hyllis_sdh_reflex()
{
    tScriptStruct_SFXDomeHyllis* v1; // [esp+8h] [ebp-2Ch]
    float v2; // [esp+Ch] [ebp-28h]
    float v3; // [esp+10h] [ebp-24h]
    float v4; // [esp+18h] [ebp-1Ch]
    float v5; // [esp+24h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXDomeHyllis*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SFX_Dome_Hyllis_sdh_reflex_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &SFX_Dome_Hyllis_sdh_reflex_univVars_Flag_speciaux_sauve,
            (void*)SFX_Dome_Hyllis_sdh_reflex_univVars,
            4488);
    }
    if (*AI_gppst_CurrentJumpNode != (tAINode*)3)
    {
        v1->i_after = 0;
        if (!AI_EvalFunc_ARRBitTest_C((int*)(SFX_Dome_Hyllis_sdh_reflex_univVars_Flag_speciaux_sauve + 4), 2718))
        {
            v2 = TIM_gf_dt;
            if (TIM_gf_dt < 0.0)
                v2 = 0.0;
            if (v2 > 1.0)
                v2 = 1.0;
            v4 = 1.0f * v2 + (1.0f - v2) * v1->f_transparency;
            v1->f_transparency = v4;
            AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
            if (v1->f_transparency >= 0.94999999)
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
            return 1000;
        }
        if (!v1->i_wait)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)3;
            TIME_Wait_C(v1->f_wait_app);
            return 0;
        }
    }
    v1->i_wait = 1;
    v3 = TIM_gf_dt;
    if (TIM_gf_dt < 0.0)
        v3 = 0.0;
    if (v3 > 1.0)
        v3 = 1.0;
    v5 = 0.0f * v3 + (1.0f - v3) * v1->f_transparency;
    v1->f_transparency = v5;
    AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0xFFu, 0);
    AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
    return 1000;
}