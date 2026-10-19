#include "ai_compiledscripts_inc.h"

//SFX_FlashTexture_Fla_Flash_constant
int fct_SFX_FlashTexture_Fla_Flash_constant_SFX_FlashTexture_Fla_BeDisplay;
int fct_SFX_FlashTexture_Fla_Flash_constant_SFX_FlashTexture_Fla_AfDisplay;

int SFX_FlashTexture_Fla_Flash_constant()
{
    tScriptStruct_SFXFlashTexture* v1; // [esp+4h] [ebp-9Ch]
    float v2; // [esp+8h] [ebp-98h]
    float v3; // [esp+Ch] [ebp-94h]
    tGameObjectMain* v4; // [esp+18h] [ebp-88h]
    tGameObjectMain* v5; // [esp+1Ch] [ebp-84h]
    tGameObjectMain* v6; // [esp+20h] [ebp-80h]
    tGameObjectMain* v7; // [esp+28h] [ebp-78h]
    tGameObjectMain* v8; // [esp+30h] [ebp-70h]
    tGameObjectMain* v9; // [esp+38h] [ebp-68h]
    tGameObjectMain* v10; // [esp+3Ch] [ebp-64h]
    tGameObjectMain* v11; // [esp+40h] [ebp-60h]
    tGameObjectMain* v12; // [esp+48h] [ebp-58h]
    tGameObjectMain* v13; // [esp+50h] [ebp-50h]
    tGameObjectMain* v14; // [esp+58h] [ebp-48h]
    tGameObjectMain* a2; // [esp+5Ch] [ebp-44h]
    tGameObjectMain* v16; // [esp+60h] [ebp-40h]
    tGameObjectMain* v17; // [esp+68h] [ebp-38h]
    tGameObjectMain* v18; // [esp+70h] [ebp-30h]
    int v19; // [esp+78h] [ebp-28h]
    unsigned int v20; // [esp+88h] [ebp-18h]
    int v21; // [esp+9Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXFlashTexture*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_FlashTexture_Fla_Flash_constant_SFX_FlashTexture_Fla_BeDisplay = (int)AI_ul_RealResolveOneRefFunction(0x35009407u);
        fct_SFX_FlashTexture_Fla_Flash_constant_SFX_FlashTexture_Fla_AfDisplay = (int)AI_ul_RealResolveOneRefFunction(0x35009408u);
    }
    if (v1->i_flash_phase1)
    {
        if (!v1->i_flash_encours)
        {
            v18 = (tGameObjectMain*)v1->o_objet_cible;
            if (v18)
            {
                if ((unsigned int)v18 <= 2)
                    v18 = AI_MainActor[(uint32_t)v18 - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v18,
                    0,
                    (tAIFunction*)fct_SFX_FlashTexture_Fla_Flash_constant_SFX_FlashTexture_Fla_BeDisplay);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    0,
                    (tAIFunction*)fct_SFX_FlashTexture_Fla_Flash_constant_SFX_FlashTexture_Fla_BeDisplay);
            }
            v17 = (tGameObjectMain*)v1->o_objet_cible;
            if (v17)
            {
                if ((unsigned int)v17 <= 2)
                    v17 = AI_MainActor[(uint32_t)v17 - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v17,
                    1,
                    (tAIFunction*)fct_SFX_FlashTexture_Fla_Flash_constant_SFX_FlashTexture_Fla_AfDisplay);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    1,
                    (tAIFunction*)fct_SFX_FlashTexture_Fla_Flash_constant_SFX_FlashTexture_Fla_AfDisplay);
            }
            v16 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v16 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                a2 = v16;
            }
            else
            {
                a2 = 0;
            }
            v14 = (tGameObjectMain*)v1->o_objet_cible;
            if (v14)
            {
                if ((unsigned int)v14 <= 2)
                    v14 = AI_MainActor[(uint32_t)v14 - 1];
                AI_EvalFunc_OBJ_AddMaterial_C(v14, a2, v1->des_i_submat);
            }
            else
            {
                AI_EvalFunc_OBJ_AddMaterial_C(0, a2, v1->des_i_submat);
            }
            v1->i_flash_encours = 1;
        }
        if (v1->des_f_temps_deb > 0.0 && v1->des_f_opacite > (double)v1->f_coeff_courant)
        {
            v1->f_coeff_courant = v1->des_f_opacite * TIM_gf_dt / v1->des_f_temps_deb + v1->f_coeff_courant;
        }
        else
        {
            v1->i_flash_phase1 = 0;
            if (v1->des_f_temps_deb == 0.0)
                v1->f_coeff_courant = v1->des_f_opacite;
        }
    }
    if (!v1->i_flash_phase1)
    {
        if (v1->des_i_type_effet)
        {
            v1->f_coeff_courant = v1->des_f_opacite;
            if (v1->des_f_opacite <= 0.0)
            {
                v8 = (tGameObjectMain*)v1->o_objet_cible;
                if (v8)
                {
                    if ((unsigned int)v8 <= 2)
                        v8 = AI_MainActor[(uint32_t)v8 - 1];
                    AI_EvalFunc_AICBDel_C(
                        AI_gpst_CurrentGameObject,
                        v8,
                        0,
                        (tAIFunction*)fct_SFX_FlashTexture_Fla_Flash_constant_SFX_FlashTexture_Fla_BeDisplay);
                }
                else
                {
                    AI_EvalFunc_AICBDel_C(
                        AI_gpst_CurrentGameObject,
                        0,
                        0,
                        (tAIFunction*)fct_SFX_FlashTexture_Fla_Flash_constant_SFX_FlashTexture_Fla_BeDisplay);
                }
                v7 = (tGameObjectMain*)v1->o_objet_cible;
                if (v7)
                {
                    if ((unsigned int)v7 <= 2)
                        v7 = AI_MainActor[(uint32_t)v7 - 1];
                    AI_EvalFunc_AICBDel_C(
                        AI_gpst_CurrentGameObject,
                        v7,
                        1,
                        (tAIFunction*)fct_SFX_FlashTexture_Fla_Flash_constant_SFX_FlashTexture_Fla_AfDisplay);
                }
                else
                {
                    AI_EvalFunc_AICBDel_C(
                        AI_gpst_CurrentGameObject,
                        0,
                        1,
                        (tAIFunction*)fct_SFX_FlashTexture_Fla_Flash_constant_SFX_FlashTexture_Fla_AfDisplay);
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
                v4 = (tGameObjectMain*)v1->o_objet_cible;
                if (v4)
                {
                    if ((unsigned int)v4 <= 2)
                        v4 = AI_MainActor[(uint32_t)v4 - 1];
                    AI_EvalFunc_OBJ_AddMaterial_C(v4, v5, -1);
                }
                else
                {
                    AI_EvalFunc_OBJ_AddMaterial_C(0, v5, -1);
                }
                v21 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
                if (v21 != 1000)
                    return v21;
            }
        }
        else if (v1->des_f_temps_fin == 0.0 || v1->f_coeff_courant <= 0.0)
        {
            v13 = (tGameObjectMain*)v1->o_objet_cible;
            if (v13)
            {
                if ((unsigned int)v13 <= 2)
                    v13 = AI_MainActor[(uint32_t)v13 - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v13,
                    0,
                    (tAIFunction*)fct_SFX_FlashTexture_Fla_Flash_constant_SFX_FlashTexture_Fla_BeDisplay);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    0,
                    (tAIFunction*)fct_SFX_FlashTexture_Fla_Flash_constant_SFX_FlashTexture_Fla_BeDisplay);
            }
            v12 = (tGameObjectMain*)v1->o_objet_cible;
            if (v12)
            {
                if ((unsigned int)v12 <= 2)
                    v12 = AI_MainActor[(uint32_t)v12 - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v12,
                    1,
                    (tAIFunction*)fct_SFX_FlashTexture_Fla_Flash_constant_SFX_FlashTexture_Fla_AfDisplay);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    1,
                    (tAIFunction*)fct_SFX_FlashTexture_Fla_Flash_constant_SFX_FlashTexture_Fla_AfDisplay);
            }
            v11 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v11 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v10 = v11;
            }
            else
            {
                v10 = 0;
            }
            v9 = (tGameObjectMain*)v1->o_objet_cible;
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                AI_EvalFunc_OBJ_AddMaterial_C(v9, v10, -1);
            }
            else
            {
                AI_EvalFunc_OBJ_AddMaterial_C(0, v10, -1);
            }
            v19 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v19 != 1000)
                return v19;
        }
        else
        {
            v1->f_coeff_courant = v1->f_coeff_courant - v1->des_f_opacite * TIM_gf_dt / v1->des_f_temps_fin;
        }
    }
    v3 = v1->f_coeff_courant;
    if (v3 <= (double)v1->des_f_opacite)
    {
        if (v3 >= 0.0)
            v2 = v1->f_coeff_courant;
        else
            v2 = 0.0;
    }
    else
    {
        v2 = v1->des_f_opacite;
    }
    v1->f_coeff_courant = v2;
    v20 = AI_EvalFunc_COLORBlack_C();
    v1->c_couleur_courante = AI_EvalFunc_COLORBlend_C(v20, v1->des_c_couleur, v1->f_coeff_courant);
    return 1000;
}

int SFX_FlashTexture_Fla_BeDisplay()
{
    tScriptStruct_SFXFlashTexture* v1;

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXFlashTexture*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v1->c_constant_backup = AI_EvalFunc_MATGetConstant_C(AI_gpst_CurrentGameObject, v1->des_i_submat);
    AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v1->c_couleur_courante, v1->des_i_submat);
    return 1000;
}

int SFX_FlashTexture_Fla_AfDisplay()
{
    tScriptStruct_SFXFlashTexture* v1;

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXFlashTexture*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v1->c_constant_backup, v1->des_i_submat);
    return 1000;
}