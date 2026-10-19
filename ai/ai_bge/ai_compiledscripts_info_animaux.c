#include "ai_compiledscripts_inc.h"

//InfoAnimaux_ia_init
int InfoAnimaux_ia_init_univVars;
int InfoAnimaux_ia_init_univVars_Flag_speciaux_sauve;
int InfoAnimaux_ia_init_INV_manager_gao;
int InfoAnimaux_ia_init_INV_manager_gao_i_Animals_Nb;
int InfoAnimaux_ia_init_INV_manager_gao_ao_Animals;
int InfoAnimaux_ia_init_INV_manager_gao_ai_AnimalsData;

int InfoAnimaux_ia_init()
{
    int result; // eax
    tScriptStruct_InfoAnimaux* v1; // [esp+8h] [ebp-74h]
    tGameObjectMain* v2; // [esp+Ch] [ebp-70h]
    tGameObjectMain* v3; // [esp+10h] [ebp-6Ch]
    tGameObjectMain* v4; // [esp+14h] [ebp-68h]
    tGameObjectMain* v5; // [esp+18h] [ebp-64h]
    tGameObjectMain* v6; // [esp+20h] [ebp-5Ch]
    tGameObjectMain* v7; // [esp+28h] [ebp-54h]
    tGameObjectMain* v8; // [esp+2Ch] [ebp-50h]
    tGameObjectMain* v9; // [esp+30h] [ebp-4Ch]
    tGameObjectMain* v10; // [esp+38h] [ebp-44h]
    tGameObjectMain* v11; // [esp+3Ch] [ebp-40h]
    tGameObjectMain* v12; // [esp+40h] [ebp-3Ch]
    int v13; // [esp+64h] [ebp-18h]
    int i; // [esp+68h] [ebp-14h]
    int v15; // [esp+74h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_InfoAnimaux*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        InfoAnimaux_ia_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&InfoAnimaux_ia_init_univVars_Flag_speciaux_sauve, (void*)InfoAnimaux_ia_init_univVars, 4488);
        InfoAnimaux_ia_init_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &InfoAnimaux_ia_init_INV_manager_gao_i_Animals_Nb,
            (void*)InfoAnimaux_ia_init_INV_manager_gao,
            23888);
        AI2C_ResolveBuffer(
            &InfoAnimaux_ia_init_INV_manager_gao_ao_Animals,
            (void*)InfoAnimaux_ia_init_INV_manager_gao,
            23368);
        AI2C_ResolveBuffer(
            &InfoAnimaux_ia_init_INV_manager_gao_ai_AnimalsData,
            (void*)InfoAnimaux_ia_init_INV_manager_gao,
            23628);
    }
    if (v1->des_i_sf_jesuisla)
    {
        if (v1->des_i_sf_jesuisla <= 0)
            v15 = 0;
        else
            v15 = v1->des_i_sf_jesuisla < 10000 ? AI_EvalFunc_ARRBitTest_C(
                (int*)(InfoAnimaux_ia_init_univVars_Flag_speciaux_sauve + 4),
                v1->des_i_sf_jesuisla) : 1
            - AI_EvalFunc_ARRBitTest_C(
                (int*)(InfoAnimaux_ia_init_univVars_Flag_speciaux_sauve
                    + 4),
                v1->des_i_sf_jesuisla - 10000);
        if (!v15)
        {
            v13 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v13 != 1000)
                return v13;
        }
    }
    v12 = (tGameObjectMain*)v1->des_o_jesuisla_desactive;
    if (v12)
    {
        if ((unsigned int)v12 <= 2)
            v12 = AI_MainActor[(uint32_t)v12 - 1];
        v11 = v12;
    }
    else
    {
        v11 = 0;
    }
    if (!v11)
        goto LABEL_27;
    v10 = (tGameObjectMain*)v1->des_o_jesuisla_desactive;
    if (v10)
    {
        if ((unsigned int)v10 <= 2)
            v10 = AI_MainActor[(uint32_t)v10 - 1];
        result = AI_EvalFunc_OBJDestroy_C(v10);
    }
    else
    {
        result = AI_EvalFunc_OBJDestroy_C(0);
    }
    if (result == 1000)
    {
    LABEL_27:
        v9 = (tGameObjectMain*)v1->des_o_jesuisla_active;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            v8 = v9;
        }
        else
        {
            v8 = 0;
        }
        if (v8)
        {
            v7 = (tGameObjectMain*)v1->des_o_jesuisla_active;
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(v7, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
            }
            v6 = (tGameObjectMain*)v1->des_o_jesuisla_active;
            if (v6)
            {
                if ((unsigned int)v6 <= 2)
                    v6 = AI_MainActor[(uint32_t)v6 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v6, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
            }
        }
        if (v1->des_i_animaux != -1)
        {
            if (v1->des_i_MapEtage != -1 && *(int*)InfoAnimaux_ia_init_INV_manager_gao_i_Animals_Nb < 64)
            {
                for (i = 0; i < *(int32_t*)InfoAnimaux_ia_init_INV_manager_gao_i_Animals_Nb; ++i)
                {
                    v5 = AI_gpst_CurrentGameObject;
                    if (AI_gpst_CurrentGameObject)
                    {
                        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                            v5 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                        v4 = v5;
                    }
                    else
                    {
                        v4 = 0;
                    }
                    if (*(tGameObjectMain**)(InfoAnimaux_ia_init_INV_manager_gao_ao_Animals + 4 * i + 4) == v4)
                        break;
                }
                if (i == *(uint32_t*)InfoAnimaux_ia_init_INV_manager_gao_i_Animals_Nb)
                {
                    *(uint32_t*)(InfoAnimaux_ia_init_INV_manager_gao_ao_Animals
                        + 4 * *(uint32_t*)InfoAnimaux_ia_init_INV_manager_gao_i_Animals_Nb
                        + 4) = (uint32_t)AI_gpst_CurrentGameObject;
                    *(uint32_t*)(InfoAnimaux_ia_init_INV_manager_gao_ai_AnimalsData
                        + 4 * (*(uint32_t*)InfoAnimaux_ia_init_INV_manager_gao_i_Animals_Nb)++
                        + 4) = v1->des_i_animaux + (v1->des_i_MapEtage << 8);
                }
            }
            AI_EvalFunc_OBJ_InfoPhotoParamSet_C(
                AI_gpst_CurrentGameObject,
                4,
                v1->des_i_animaux,
                0,
                0,
                0.15000001f,
                1.0,
                v1->des_f_frame_min,
                v1->des_f_sphere_radius);
            if ((AI_EvalFunc_OBJ_FlagsIdentityGet_C(AI_gpst_CurrentGameObject) & 0x40) != 0)
            {
                v3 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                        v3 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                    v2 = v3;
                }
                else
                {
                    v2 = 0;
                }
                v2->extended->fields->designFlags = 0x40;
            }
        }
        v1->des_i_animaux = -1;
        AI_EvalFunc_AITrackCurStop_C();
        result = 1000;
    }
    return result;
}