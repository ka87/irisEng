#include "ai_compiledscripts_inc.h"

//GAO_Shaker_GAOS_init
int fct_GAO_Shaker_GAOS_init_GAO_Shaker_GAOS_comportement;

int GAO_Shaker_GAOS_comportement()
{
    float X; // [esp+4h] [ebp-C8h]
    tScriptStruct_GaoShaker* v2; // [esp+8h] [ebp-C4h]
    Vec3* v3; // [esp+Ch] [ebp-C0h]
    Vec3* v4; // [esp+10h] [ebp-BCh]
    Vec3* v5; // [esp+14h] [ebp-B8h]
    Vec3* v6; // [esp+18h] [ebp-B4h]
    Vec3* v7; // [esp+1Ch] [ebp-B0h]
    Vec3* v8; // [esp+20h] [ebp-ACh]
    Vec3* v9; // [esp+24h] [ebp-A8h]
    float v10; // [esp+28h] [ebp-A4h]
    float v11; // [esp+34h] [ebp-98h]
    Vec3 a1; // [esp+3Ch] [ebp-90h] BYREF
    Vec3 v13; // [esp+4Ch] [ebp-80h] BYREF
    float v14; // [esp+58h] [ebp-74h]
    float v15; // [esp+5Ch] [ebp-70h]
    float v16; // [esp+60h] [ebp-6Ch]
    float v18; // [esp+68h] [ebp-64h]
    float v19; // [esp+6Ch] [ebp-60h]
    float v20; // [esp+70h] [ebp-5Ch]
    float v21; // [esp+74h] [ebp-58h]
    Vec3 a4; // [esp+84h] [ebp-48h] BYREF
    float v23; // [esp+90h] [ebp-3Ch]
    float v24; // [esp+94h] [ebp-38h]
    float v25; // [esp+9Ch] [ebp-30h]
    float a3; // [esp+A0h] [ebp-2Ch]
    float v27; // [esp+A4h] [ebp-28h]
    tScriptStruct_GaoShaker* v28; // [esp+ACh] [ebp-20h]
    Vec3 a2; // [esp+B0h] [ebp-1Ch] BYREF
    float v30; // [esp+C0h] [ebp-Ch]
    float v31; // [esp+C4h] [ebp-8h]
    float v32; // [esp+C8h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_GaoShaker*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v28 = v2;
    a3 = 0.0;
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
    {
        a3 = AI_EvalFunc_MATHRandFloat_C(1.0, 1.0);
        if (v28->des_je_suis_une_light)
        {
            v11 = AI_EvalFunc_MATHRandFloat_C(v28->des_light_near_min, v28->des_light_near_max);
            a3 = AI_EvalFunc_MATHRandFloat_C(v28->des_light_far_min, v28->des_light_far_max);
            AI_EvalFunc_LightSetNearFar_C(AI_gpst_CurrentGameObject, v11, a3);
        }
        if (!v28->i_flag_activated)
        {
            v28->i_flag_activated = 1;
            v28->f_time_phase = TIM_gf_MainClock - TIM_gf_LockTime;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v28->v_start_pos);
            if (v28->i_mode_phase)
            {
                v28->f_duree_phase = AI_EvalFunc_MATHRandFloat_C(v28->des_stop_delai_min, v28->des_stop_delai_max);
                v8 = &v28->v_dest_pos;
                v9 = &v28->v_start_pos;
                v28->v_dest_pos.x = v28->v_start_pos.x;
                v8->y = v9->y;
                v8->z = v9->z;
            }
            else
            {
                if (v28->des_move_delai_max == 0.0)
                    v28->f_duree_phase = 0.0;
                else
                    v28->f_duree_phase = AI_EvalFunc_MATHRandFloat_C(v28->des_move_delai_min, v28->des_move_delai_max);
                v6 = &v28->v_dest_pos;
                v7 = &v28->v_init_pos;
                v28->v_dest_pos.x = v28->v_init_pos.x;
                v6->y = v7->y;
                v6->z = v7->z;
                v25 = AI_EvalFunc_MATHRandFloat_C(v28->des_X_min, v28->des_X_max);
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&a2);
                v14 = a2.x * v25;
                v15 = a2.y * v25;
                v16 = a2.z * v25;
                v5 = &v28->v_dest_pos;
                v28->v_dest_pos.x = v14 + v28->v_dest_pos.x;
                v5->y = v15 + v5->y;
                v5->z = v16 + v5->z;
                v23 = AI_EvalFunc_MATHRandFloat_C(v28->des_Y_min, v28->des_Y_max);
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a1);
                v30 = a1.x * v23;
                v31 = a1.y * v23;
                v32 = a1.z * v23;
                v4 = &v28->v_dest_pos;
                v28->v_dest_pos.x = v30 + v28->v_dest_pos.x;
                v4->y = v31 + v4->y;
                v4->z = v32 + v4->z;
                v24 = AI_EvalFunc_MATHRandFloat_C(v28->des_Z_min, v28->des_Z_max);
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v13);
                v18 = v13.x * v24;
                v19 = v13.y * v24;
                v20 = v13.z * v24;
                v3 = &v28->v_dest_pos;
                v28->v_dest_pos.x = v18 + v28->v_dest_pos.x;
                v3->y = v19 + v3->y;
                v3->z = v20 + v3->z;
            }
        }
        if (v28->f_duree_phase == 0.0)
        {
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v28->v_dest_pos);
            v28->i_flag_activated = 0;
            if (v28->i_nb_phases)
            {
                if (v28->i_mode_phase)
                    v28->i_mode_phase = 0;
                else
                    v28->i_mode_phase = 1;
            }
        }
        else if (AI_EvalFunc_TIMEElapsed_C(v28->f_time_phase, v28->f_duree_phase))
        {
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v28->v_dest_pos);
            v28->i_flag_activated = 0;
            if (v28->i_nb_phases)
            {
                if (v28->i_mode_phase)
                    v28->i_mode_phase = 0;
                else
                    v28->i_mode_phase = 1;
            }
        }
        else if (!v28->i_mode_phase)
        {
            v21 = TIM_gf_MainClock - TIM_gf_LockTime;
            v10 = (v21 - v28->f_time_phase) / v28->f_duree_phase;
            if (v28->des_amorti)
            {
                X = v10 * 3.141593f;
                v27 = cosf(X);
                v10 = (1.0f - v27) * 0.5f;
            }
            AI_EvalFunc_MATHVecBlend_C(&v28->v_start_pos, &v28->v_dest_pos, v10, &a4);
            AI_EvalFunc_OBJSetPos_C_CURRENT(&a4);
        }
    }
    else
    {
        v28->i_flag_activated = 0;
        v28->i_mode_phase = 0;
    }
    return 1000;
}

int GAO_Shaker_GAOS_init()
{
    int result; // eax
    tScriptStruct_GaoShaker* v1; // [esp+0h] [ebp-30h]
    int v2; // [esp+4h] [ebp-2Ch]
    Vec3 a1; // [esp+8h] [ebp-28h] BYREF
    Vec3 a3; // [esp+20h] [ebp-10h] BYREF
    tScriptStruct_GaoShaker* v5; // [esp+2Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GaoShaker*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v5 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_GAO_Shaker_GAOS_init_GAO_Shaker_GAOS_comportement = (int)AI_ul_RealResolveOneRefFunction(0x72004F28u);
    }
    if (v5->des_coord)
    {
        a1.x = 0.0;
        a1.y = 1.0;
        a1.z = 0.0;
        a3.x = 0.0;
        a3.y = 0.0;
        a3.z = 1.0;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a1, &a3);
    }
    v5->i_nb_phases = v5->des_stop_delai_max != 0.0f;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v5->v_init_pos);
    v2 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_GAO_Shaker_GAOS_init_GAO_Shaker_GAOS_comportement);
    if (v2 == 1000)
        result = 1000;
    else
        result = v2;
    return result;
}