#include "ai_compiledscripts_inc.h"

//Generateur_lanceur_gela_init
int fct_Generateur_lanceur_gela_init_Generateur_lanceur_gela_normal;

int Generateur_lanceur_gela_normal()
{
    tGameObjectMain* v0; // eax
    float v2; // [esp+0h] [ebp-6Ch]
    float v3; // [esp+0h] [ebp-6Ch]
    tScriptStruct_GenerateurLanceur* v4; // [esp+8h] [ebp-64h]
    tGameObjectMain* v5; // [esp+10h] [ebp-5Ch]
    tGameObjectMain* v6; // [esp+18h] [ebp-54h]
    tGameObjectMain* v7; // [esp+20h] [ebp-4Ch]
    tGameObjectMain* v8; // [esp+28h] [ebp-44h]
    tGameObjectMain* v9; // [esp+2Ch] [ebp-40h]
    tGameObjectMain* v10; // [esp+30h] [ebp-3Ch]
    Vec3 a1; // [esp+38h] [ebp-34h] BYREF
    unsigned int v12; // [esp+44h] [ebp-28h]
    float v13; // [esp+48h] [ebp-24h]
    Vec3 a2; // [esp+50h] [ebp-1Ch] BYREF
    tScriptStruct_GenerateurLanceur* v16; // [esp+64h] [ebp-8h]
    float v17; // [esp+68h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_GenerateurLanceur*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v16 = v4;
    v12 = 0;
    if (AI_EvalFunc_TIMEElapsed_C(v4->f_temps, v4->f_delai))
    {
        v16->f_temps = TIM_gf_MainClock - TIM_gf_LockTime;
        v16->f_delai = AI_EvalFunc_MATHRandFloat_C(v16->des_f_frequence_min, v16->des_f_frequence_max);
        if (v16->i_indice >= 5)
            goto LABEL_33;
        v10 = (tGameObjectMain*)v16->des_o_objet_lance[v16->i_indice];
        if (v10)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            v9 = v10;
        }
        else
        {
            v9 = 0;
        }
        if (v9)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
            v8 = (tGameObjectMain*)v16->des_o_objet_lance[v16->i_indice];
            if (v8)
            {
                if ((unsigned int)v8 <= 2)
                    v8 = AI_MainActor[(uint32_t)v8 - 1];
                v0 = AI_EvalFunc_OBJDuplicate_C(v8, &a1);
            }
            else
            {
                v0 = AI_EvalFunc_OBJDuplicate_C(0, &a1);
            }
            v12 = (unsigned int)v0;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
            v7 = (tGameObjectMain*)v12;
            if (v12)
            {
                if (v12 <= 2)
                    v7 = AI_MainActor[v12 - 1];
                AI_EvalFunc_OBJ_SightSet_C(v7, &a2);
            }
            else
            {
                AI_EvalFunc_OBJ_SightSet_C(0, &a2);
            }
            v2 = -v16->des_f_angle_max;
            v17 = AI_EvalFunc_MATHRandFloat_C(v2, v16->des_f_angle_max);
            v6 = (tGameObjectMain*)v12;
            if (v12)
            {
                if (v12 <= 2)
                    v6 = AI_MainActor[v12 - 1];
                AI_EvalFunc_OBJRotateLocalZ_c(v6, v17);
            }
            else
            {
                AI_EvalFunc_OBJRotateLocalZ_c(0, v17);
            }
            v3 = -v16->des_f_angle_max;
            v13 = AI_EvalFunc_MATHRandFloat_C(v3, v16->des_f_angle_max);
            v5 = (tGameObjectMain*)v12;
            if (v12)
            {
                if (v12 <= 2)
                    v5 = AI_MainActor[v12 - 1];
                AI_EvalFunc_OBJRotateLocalX_C(v5, v13);
            }
            else
            {
                AI_EvalFunc_OBJRotateLocalX_C(0, v13);
            }
            ++v16->i_indice;
        }
        else
        {
        LABEL_33:
            v16->f_delai = 0.0;
            v16->i_indice = 0;
        }
    }
    return 1000;
}

int Generateur_lanceur_gela_init()
{
    tScriptStruct_GenerateurLanceur* v1; // [esp+8h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GenerateurLanceur*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Generateur_lanceur_gela_init_Generateur_lanceur_gela_normal = (int)AI_ul_RealResolveOneRefFunction(0x35009FB9u);
    }
    v1->f_temps = TIM_gf_MainClock - TIM_gf_LockTime;
    v1->f_delai = AI_EvalFunc_MATHRandFloat_C(v1->des_f_frequence_min, v1->des_f_frequence_max);
    v1->des_f_angle_max = v1->des_f_angle_max * 0.017453f;
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Generateur_lanceur_gela_init_Generateur_lanceur_gela_normal);
    return 1000;
}