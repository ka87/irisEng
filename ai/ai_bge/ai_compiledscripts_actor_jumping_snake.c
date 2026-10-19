#include "ai_compiledscripts_inc.h"

//Jumping_Snake_JS_init
int fct_Jumping_Snake_JS_init_Jumping_Snake_JS_comportement;

//Jumping_Snake_JS_firstinit
int Jumping_Snake_JS_firstinit_univVars;
int Jumping_Snake_JS_firstinit_univVars_Flag_speciaux_sauve;
int Jumping_Snake_JS_firstinit_INV_manager_gao;
int Jumping_Snake_JS_firstinit_INV_manager_gao_i_Animals_Nb;
int Jumping_Snake_JS_firstinit_INV_manager_gao_ao_Animals;
int Jumping_Snake_JS_firstinit_INV_manager_gao_ai_AnimalsData;
int fct_Jumping_Snake_JS_firstinit_Jumping_Snake_JS_init;

//Jumping_Snake_JS_comportement
int Jumping_Snake_JS_comportement_univVars;
int Jumping_Snake_JS_comportement_univVars_Flag_speciaux_sauve;

int Jumping_Snake_JS_init()
{
    int v0; // eax
    tGameObjectMain* v1; // eax
    int result; // eax
    tScriptStruct_PNJJumpingSnake* v3; // [esp+0h] [ebp-48h]
    tGameObjectMain* v4; // [esp+8h] [ebp-40h]
    tGameObjectMain* v5; // [esp+10h] [ebp-38h]
    int v6; // [esp+2Ch] [ebp-1Ch]
    Vec3 a2; // [esp+3Ch] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_PNJJumpingSnake*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Jumping_Snake_JS_init_Jumping_Snake_JS_comportement = (int)AI_ul_RealResolveOneRefFunction(0x72002CF7u);
    }
    v5 = (tGameObjectMain*)v3->des_o_wp_of_net;
    if (v5)
    {
        if ((unsigned int)v5 <= 2)
            v5 = AI_MainActor[(uint32_t)v5 - 1];
        v0 = (int)AI_EvalFunc_WAYNetOfObj_C(v5);
    }
    else
    {
        v0 = (int)AI_EvalFunc_WAYNetOfObj_C(0);
    }
    v3->n_net = v0;
    v3->o_current_wp = v3->des_o_wp_of_net;
    v4 = (tGameObjectMain*)v3->o_current_wp;
    if (v4)
    {
        if ((unsigned int)v4 <= 2)
            v4 = AI_MainActor[(uint32_t)v4 - 1];
        v1 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v3->n_net, v4, 3, 0);
    }
    else
    {
        v1 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v3->n_net, 0, 3, 0);
    }
    v3->o_next_wp = (int)v1;
    v3->f_oscillation_speed = AI_EvalFunc_MATHRandFloat_C(2.0, 3.0);
    AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
    AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 16814084, 0);
    a2.x = v3->des_f_friction;
    a2.y = 1.0;
    a2.z = v3->des_f_friction;
    AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &a2);
    v6 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Jumping_Snake_JS_init_Jumping_Snake_JS_comportement);
    if (v6 == 1000)
        result = 1000;
    else
        result = v6;
    return result;
}

int Jumping_Snake_JS_firstinit()
{
    int result; // eax
    tScriptStruct_PNJJumpingSnake* v1; // [esp+0h] [ebp-38h]
    tGameObjectMain* v2; // [esp+4h] [ebp-34h]
    tGameObjectMain* v3; // [esp+8h] [ebp-30h]
    int v4; // [esp+1Ch] [ebp-1Ch]
    int i; // [esp+28h] [ebp-10h]
    int v6; // [esp+2Ch] [ebp-Ch]
    int v7; // [esp+34h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJJumpingSnake*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Jumping_Snake_JS_firstinit_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Jumping_Snake_JS_firstinit_univVars_Flag_speciaux_sauve,
            (void*)Jumping_Snake_JS_firstinit_univVars,
            4488);
        Jumping_Snake_JS_firstinit_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &Jumping_Snake_JS_firstinit_INV_manager_gao_i_Animals_Nb,
            (void*)Jumping_Snake_JS_firstinit_INV_manager_gao,
            23888);
        AI2C_ResolveBuffer(
            &Jumping_Snake_JS_firstinit_INV_manager_gao_ao_Animals,
            (void*)Jumping_Snake_JS_firstinit_INV_manager_gao,
            23368);
        AI2C_ResolveBuffer(
            &Jumping_Snake_JS_firstinit_INV_manager_gao_ai_AnimalsData,
            (void*)Jumping_Snake_JS_firstinit_INV_manager_gao,
            23628);
        fct_Jumping_Snake_JS_firstinit_Jumping_Snake_JS_init = (int)AI_ul_RealResolveOneRefFunction(0x72002CF6u);
    }
    if (!AI_EvalFunc_ARRBitTest_C((int*)(Jumping_Snake_JS_firstinit_univVars_Flag_speciaux_sauve + 4), 126))
    {
        v6 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v6 != 1000)
            return v6;
    }
    if (v1->des_i_etage != -1)
    {
        v7 = AI_EvalFunc_OBJ_InfoPhotoInfoGet_C(AI_gpst_CurrentGameObject);
        if (v7 != -1 && *(int*)Jumping_Snake_JS_firstinit_INV_manager_gao_i_Animals_Nb < 64)
        {
            for (i = 0; i < *(int32_t*)Jumping_Snake_JS_firstinit_INV_manager_gao_i_Animals_Nb; ++i)
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
                if (*(tGameObjectMain**)(Jumping_Snake_JS_firstinit_INV_manager_gao_ao_Animals + 4 * i + 4) == v2)
                    break;
            }
            if (i == *(uint32_t*)Jumping_Snake_JS_firstinit_INV_manager_gao_i_Animals_Nb)
            {
                *(uint32_t*)(Jumping_Snake_JS_firstinit_INV_manager_gao_ao_Animals
                    + 4 * *(uint32_t*)Jumping_Snake_JS_firstinit_INV_manager_gao_i_Animals_Nb
                    + 4) = (uint32_t)AI_gpst_CurrentGameObject;
                *(uint32_t*)(Jumping_Snake_JS_firstinit_INV_manager_gao_ai_AnimalsData
                    + 4 * (*(uint32_t*)Jumping_Snake_JS_firstinit_INV_manager_gao_i_Animals_Nb)++
                    + 4) = (v1->des_i_etage << 8) + v7;
            }
        }
    }
    v4 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Jumping_Snake_JS_firstinit_Jumping_Snake_JS_init);
    if (v4 == 1000)
        result = 1000;
    else
        result = v4;
    return result;
}

int Jumping_Snake_JS_comportement()
{
    int v0; // eax
    tGameObjectMain* v1; // eax
    int result; // eax
    float X; // [esp+0h] [ebp-20Ch]
    float Xa; // [esp+0h] [ebp-20Ch]
    float Xb; // [esp+0h] [ebp-20Ch]
    float v6; // [esp+8h] [ebp-204h]
    tScriptStruct_PNJJumpingSnake* v7; // [esp+Ch] [ebp-200h]
    tGameObjectMain* v8; // [esp+1Ch] [ebp-1F0h]
    float v9; // [esp+20h] [ebp-1ECh]
    tGameObjectMain* v10; // [esp+30h] [ebp-1DCh]
    tGameObjectMain* v11; // [esp+48h] [ebp-1C4h]
    float v12; // [esp+54h] [ebp-1B8h]
    float v13; // [esp+58h] [ebp-1B4h]
    tGameObjectMain* v14; // [esp+74h] [ebp-198h]
    tGameObjectMain* v15; // [esp+78h] [ebp-194h]
    tGameObjectMain* v16; // [esp+7Ch] [ebp-190h]
    tGameObjectMain* v17; // [esp+84h] [ebp-188h]
    tGameObjectMain* v18; // [esp+8Ch] [ebp-180h]
    tGameObjectMain* v19; // [esp+90h] [ebp-17Ch]
    tGameObjectMain* v20; // [esp+94h] [ebp-178h]
    Vec3 a4; // [esp+9Ch] [ebp-170h] BYREF
    Vec3 v22; // [esp+ACh] [ebp-160h] BYREF
    Vec3 v23; // [esp+B8h] [ebp-154h] BYREF
    float v24; // [esp+C4h] [ebp-148h]
    float v25; // [esp+C8h] [ebp-144h]
    Vec3 v26; // [esp+CCh] [ebp-140h] BYREF
    Vec3 v27; // [esp+DCh] [ebp-130h] BYREF
    Vec3 dest; // [esp+E8h] [ebp-124h] BYREF
    Vec3 v29; // [esp+F8h] [ebp-114h] BYREF
    Vec3 v30; // [esp+104h] [ebp-108h] BYREF
    float v31; // [esp+110h] [ebp-FCh]
    tGameObjectMain* v32; // [esp+114h] [ebp-F8h]
    float v33; // [esp+118h] [ebp-F4h]
    float v34; // [esp+11Ch] [ebp-F0h]
    float v35; // [esp+120h] [ebp-ECh]
    Vec3 v36; // [esp+124h] [ebp-E8h] BYREF
    float v37; // [esp+130h] [ebp-DCh]
    Vec3 v38; // [esp+134h] [ebp-D8h] BYREF
    Vec3 v39; // [esp+140h] [ebp-CCh] BYREF
    Vec3 v40; // [esp+14Ch] [ebp-C0h] BYREF
    Vec3 v41; // [esp+15Ch] [ebp-B0h] BYREF
    Vec3 v; // [esp+168h] [ebp-A4h] BYREF
    float v43; // [esp+174h] [ebp-98h]
    float v44; // [esp+178h] [ebp-94h]
    float v45; // [esp+17Ch] [ebp-90h]
    Vec3 v46; // [esp+180h] [ebp-8Ch] BYREF
    Vec3 v47; // [esp+194h] [ebp-78h] BYREF
    Vec3 v48; // [esp+1A0h] [ebp-6Ch] BYREF
    Vec3 v49; // [esp+1ACh] [ebp-60h] BYREF
    BOOL v50; // [esp+1B8h] [ebp-54h]
    int v51; // [esp+1BCh] [ebp-50h]
    Vec3 v52; // [esp+1C0h] [ebp-4Ch] BYREF
    tScriptStruct_PNJJumpingSnake* v53; // [esp+1CCh] [ebp-40h]
    Vec3 a3; // [esp+1D0h] [ebp-3Ch] BYREF
    float v55; // [esp+1DCh] [ebp-30h]
    float v56; // [esp+1E0h] [ebp-2Ch]
    float v57; // [esp+1E4h] [ebp-28h]
    float v58; // [esp+1E8h] [ebp-24h]
    float v59; // [esp+1ECh] [ebp-20h]
    Vec3 v60; // [esp+1F0h] [ebp-1Ch] BYREF
    Vec3 v61; // [esp+1FCh] [ebp-10h]
    float v62; // [esp+208h] [ebp-4h]

    v.x = 1.0;
    v.y = 0.0;
    v.z = 0.0;
    v48.x = 1.0;
    v48.y = 0.0;
    v48.z = 0.0;
    v29.x = 1.0;
    v29.y = 0.0;
    v29.z = 0.0;
    v22.x = 1.0;
    v22.y = 0.0;
    v22.z = 0.0;
    v40.x = 1.0;
    v40.y = 0.0;
    v40.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_PNJJumpingSnake*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    v53 = v7;
    v31 = 0.0;
    v32 = 0;
    v33 = 0.0;
    v35 = 0.0;
    v50 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Jumping_Snake_JS_comportement_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Jumping_Snake_JS_comportement_univVars_Flag_speciaux_sauve,
            (void*)Jumping_Snake_JS_comportement_univVars,
            4488);
    }
    v50 = AI_EvalFunc_ARRBitTest_C((int*)(Jumping_Snake_JS_comportement_univVars_Flag_speciaux_sauve + 4), 2718);
    if (v50)
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
    else
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
    v20 = (tGameObjectMain*)v53->o_next_wp;
    if (v20)
    {
        if ((unsigned int)v20 <= 2)
            v20 = AI_MainActor[(uint32_t)v20 - 1];
        v19 = v20;
    }
    else
    {
        v19 = 0;
    }
    if (v19)
    {
        v18 = (tGameObjectMain*)v53->o_next_wp;
        if (v18)
        {
            if ((unsigned int)v18 <= 2)
                v18 = AI_MainActor[(uint32_t)v18 - 1];
            v0 = AI_EvalFunc_COLPivotBVCollide_C(AI_gpst_CurrentGameObject, v18);
        }
        else
        {
            v0 = AI_EvalFunc_COLPivotBVCollide_C(AI_gpst_CurrentGameObject, 0);
        }
        v51 = v0;
        if (v0)
        {
            v53->o_current_wp = v53->o_next_wp;
            v17 = (tGameObjectMain*)v53->o_current_wp;
            if (v17)
            {
                if ((unsigned int)v17 <= 2)
                    v17 = AI_MainActor[(uint32_t)v17 - 1];
                v1 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v53->n_net, v17, 3, 0);
            }
            else
            {
                v1 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v53->n_net, 0, 3, 0);
            }
            v53->o_next_wp = (int)v1;
            v53->f_sight_blend_coef = 0.0;
            v53->f_banking_blend_coef = 0.0;
        }
    }
    v24 = TIM_gf_dt;
    v53->f_angle = v53->f_oscillation_speed * TIM_gf_dt + v53->f_angle;
    if (v53->f_angle >= 6.283185)
        v53->f_angle = v53->f_angle - 6.283185f;
    v33 = sinf(v53->f_angle);
    v31 = cosf(v53->f_angle);
    v59 = TIM_gf_dt;
    v53->f_angle_2 = v53->f_oscillation_speed * TIM_gf_dt * 6.0f + v53->f_angle_2;
    if (v53->f_angle_2 >= 6.283185)
        v53->f_angle_2 = v53->f_angle_2 - 6.283185f;
    v35 = sinf(v53->f_angle_2);
    v16 = (tGameObjectMain*)v53->o_next_wp;
    if (v16)
    {
        if ((unsigned int)v16 <= 2)
            v16 = AI_MainActor[(uint32_t)v16 - 1];
        v15 = v16;
    }
    else
    {
        v15 = 0;
    }
    if (v15)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v46);
        v14 = (tGameObjectMain*)v53->o_next_wp;
        if (v14)
        {
            if ((unsigned int)v14 <= 2)
                v14 = AI_MainActor[(uint32_t)v14 - 1];
            AI_EvalFunc_OBJGetPos_C(v14, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        MATH_SubstractVec(&dest, &v46, &a3);
        v = dest;
        MATH_NormalizeEqualVector(&v);
        v44 = TIM_gf_dt;
        v13 = 2.0f * TIM_gf_dt;
        if (v13 < 0.0)
            v13 = 0.0;
        if (v13 > 1.0)
            v13 = 1.0;
        v25 = v53->des_f_sight_blend_speed * v13 + v53->f_sight_blend_coef * (1.0f - v13);
        v53->f_sight_blend_coef = v25;
        v62 = TIM_gf_dt;
        v12 = 2.0f * TIM_gf_dt;
        if (v12 < 0.0)
            v12 = 0.0;
        if (v12 > 1.0)
            v12 = 1.0;
        v45 = v53->des_f_banking_blend_speed * v12 + v53->f_banking_blend_coef * (1.0f - v12);
        v53->f_banking_blend_coef = v45;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v27);
        v37 = TIM_gf_dt;
        X = v53->f_sight_blend_coef * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlend_C(&v27, &v, X, &a4);
        v29 = a4;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v36);
        v11 = (tGameObjectMain*)v53->o_current_wp;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v11 = AI_MainActor[(uint32_t)v11 - 1];
            AI_EvalFunc_OBJ_BankingGet_C(v11, &v47);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGet_C(0, &v47);
        }
        v43 = TIM_gf_dt;
        Xa = v53->f_banking_blend_coef * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlend_C(&v36, &v47, Xa, &v60);
        v48 = v60;
        AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v29, &v48);
        v32 = AI_gpst_CurrentGameObject;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v30);
        v34 = AI_EvalFunc_MTHFloatSign_C(v33);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v49);
        v23.x = v49.x * v34;
        v23.y = v49.y * v34;
        v23.z = v49.z * v34;
        v6 = (float)fabs(v33);
        v58 = v6;
        Xb = v6 * 0.050000001f;
        AI_EvalFunc_MATHVecBlend_C(&v30, &v23, Xb, &v38);
        v48 = v38;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v52);
        v10 = v32;
        if (v32)
        {
            if ((unsigned int)v32 <= 2)
                v10 = AI_MainActor[(uint32_t)v32 - 1];
            AI_EvalFunc_OBJ_BankingGeneralSet_C(v10, &v52, &v48);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v52, &v48);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v39);
        v22 = v39;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v41);
        v9 = v31 * 3.0f;
        v55 = v41.x * v9;
        v56 = v41.y * v9;
        v57 = v41.z * v9;
        v22.x = v22.x + v55;
        v22.y = v22.y + v56;
        v22.z = v22.z + v57;
        v8 = v32;
        if (v32)
        {
            if ((unsigned int)v32 <= 2)
                v8 = AI_MainActor[(uint32_t)v32 - 1];
            AI_EvalFunc_OBJSetPos_C(v8, &v22);
        }
        else
        {
            AI_EvalFunc_OBJSetPos_C(0, &v22);
        }
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v26);
        v61.x = v26.x * -50.0f;
        v61.y = v26.y * -50.0f;
        v61.z = v26.z * -50.0f;
        v40 = v61;
        AI_EvalFunc_DYNTractionSet_C_CURRENT(&v40);
        result = 1000;
    }
    else
    {
        AI_EvalFunc_OBJReinit_C(AI_gpst_CurrentGameObject, 1);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}