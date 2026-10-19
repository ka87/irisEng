#include "ai_compiledscripts_inc.h"

//ES_ciel_esc_enter_the_map
int ES_ciel_esc_enter_the_map_univVars;
int ES_ciel_esc_enter_the_map_univVars_world_entrance_ID;
int ES_ciel_esc_enter_the_map_univVars_world_entrance_angle;
int ES_ciel_esc_enter_the_map_univVars_world_entrance_percent;
int ES_ciel_esc_enter_the_map__PJ_Camera_gao;
int ES_ciel_esc_enter_the_map__PJ_Camera_Target_gao;
int fct_ES_ciel_esc_enter_the_map_ES_ciel_esc_exit_the_map;

//ES_ciel_esc_exit_the_map
int ES_ciel_esc_exit_the_map_GST_Global_gao;
int ES_ciel_esc_exit_the_map_GST_Global_gao_i_mort_en_cours;
int ES_ciel_esc_exit_the_map_univVars;
int ES_ciel_esc_exit_the_map_univVars_world_entrance_angle;
int ES_ciel_esc_exit_the_map_univVars_world_entrance_percent;
int ES_ciel_esc_exit_the_map_GST_Global_gao_o_joy_acteur;
int ES_ciel_esc_exit_the_map_GST_Global_gao_i_fade_priority;
int ES_ciel_esc_exit_the_map_GST_Global_gao_f_fade_start_time;
int ES_ciel_esc_exit_the_map_GST_Global_gao_f_fade_delay;
int ES_ciel_esc_exit_the_map_univVars_i_fade_fx;
int ES_ciel_esc_exit_the_map_INV_MatFade_gao;
int ES_ciel_esc_exit_the_map_univVars_i_special_init;
int ES_ciel_esc_exit_the_map_univVars_load_world;
int ES_ciel_esc_exit_the_map_univVars_world_ID;
int ES_ciel_esc_exit_the_map_univVars_world_entrance_ID;

//ES_ciel_esc_joy
int ES_ciel_esc_joy_GST_Global_gao;

int ES_ciel_esc_enter_the_map()
{
    int result; // eax
    float v1; // [esp+0h] [ebp-DCh]
    tScriptStruct_ESCiel* v2; // [esp+8h] [ebp-D4h]
    tGameObjectMain* v3; // [esp+10h] [ebp-CCh]
    tGameObjectMain* v4; // [esp+18h] [ebp-C4h]
    tGameObjectMain* v5; // [esp+20h] [ebp-BCh]
    tGameObjectMain* v6; // [esp+28h] [ebp-B4h]
    tGameObjectMain* v7; // [esp+30h] [ebp-ACh]
    tGameObjectMain* v8; // [esp+38h] [ebp-A4h]
    Vec3 v9; // [esp+44h] [ebp-98h] BYREF
    Vec3 a4; // [esp+50h] [ebp-8Ch] BYREF
    float v11; // [esp+5Ch] [ebp-80h]
    Vec3 v12; // [esp+60h] [ebp-7Ch] BYREF
    Vec3 v13; // [esp+6Ch] [ebp-70h] BYREF
    Vec3 v14; // [esp+7Ch] [ebp-60h] BYREF
    Vec3 v15; // [esp+88h] [ebp-54h] BYREF
    Vec3 v16; // [esp+94h] [ebp-48h] BYREF
    Vec3 v17; // [esp+A0h] [ebp-3Ch] BYREF
    float v18; // [esp+ACh] [ebp-30h]
    int v19; // [esp+B0h] [ebp-2Ch]
    Vec3 a1; // [esp+B4h] [ebp-28h] BYREF
    tScriptStruct_ESCiel* v21; // [esp+C0h] [ebp-1Ch]
    Vec3 v22; // [esp+C4h] [ebp-18h] BYREF
    Vec3 a3; // [esp+D0h] [ebp-Ch] BYREF

    v14.x = 1.0;
    v14.y = 0.0;
    v14.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ESCiel*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v21 = v2;
    v11 = 0.0;
    v19 = 0;
    v18 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        ES_ciel_esc_enter_the_map_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &ES_ciel_esc_enter_the_map_univVars_world_entrance_ID,
            (void*)ES_ciel_esc_enter_the_map_univVars,
            8);
        AI2C_ResolveBuffer(
            &ES_ciel_esc_enter_the_map_univVars_world_entrance_angle,
            (void*)ES_ciel_esc_enter_the_map_univVars,
            16);
        AI2C_ResolveBuffer(
            &ES_ciel_esc_enter_the_map_univVars_world_entrance_percent,
            (void*)ES_ciel_esc_enter_the_map_univVars,
            12);
        ES_ciel_esc_enter_the_map__PJ_Camera_gao = AI_ul_RealResolveOneRefObject(0x71003F56u);
        ES_ciel_esc_enter_the_map__PJ_Camera_Target_gao = AI_ul_RealResolveOneRefObject(0x35006499u);
        fct_ES_ciel_esc_enter_the_map_ES_ciel_esc_exit_the_map = (int)AI_ul_RealResolveOneRefFunction(0x7100BD16u);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)2)
    {
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_ES_ciel_esc_enter_the_map_ES_ciel_esc_exit_the_map);
        result = 1000;
    }
    else
    {
        v19 = AI_EvalFunc_AIPriorityGet_C(AI_gpst_CurrentGameObject);
        if (v21->des_numero_de_l_entree == *(uint32_t*)ES_ciel_esc_enter_the_map_univVars_world_entrance_ID)
        {
            v11 = *(float*)ES_ciel_esc_enter_the_map_univVars_world_entrance_angle;
            v18 = *(float*)ES_ciel_esc_enter_the_map_univVars_world_entrance_percent;
            a1.x = 1.0;
            a1.y = 0.0;
            a1.z = 0.0;
            v16.x = 0.0;
            v16.y = 0.0;
            v16.z = 1.0;
            AI_EvalFunc_MATHVecRotate_C(&a1, &v16, v11, &a4);
            v14 = a4;
            v1 = v18 * v21->des_diametre_surface;
            AI_EvalFunc_MATHVecSetNormVector_C(&v14, v1);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v9);
            v14.z = v9.z;
            v8 = (tGameObjectMain*)v21->des_perso_enterprise;
            if (v8)
            {
                if ((unsigned int)v8 <= 2)
                    v8 = AI_MainActor[(uint32_t)v8 - 1];
                AI_EvalFunc_OBJSetPos_C(v8, &v14);
            }
            else
            {
                AI_EvalFunc_OBJSetPos_C(0, &v14);
            }
            v15.x = 0.0;
            v15.y = 0.0;
            v15.z = 1.0;
            v7 = (tGameObjectMain*)v21->des_perso_enterprise;
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                AI_EvalFunc_OBJ_BankingSet_C(v7, &v15);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingSet_C(0, &v15);
            }
            v6 = (tGameObjectMain*)v21->des_perso_enterprise;
            if (v6)
            {
                if ((unsigned int)v6 <= 2)
                    v6 = AI_MainActor[(uint32_t)v6 - 1];
                AI_EvalFunc_OBJRotateLocalX_C(v6, 1.39624f);
            }
            else
            {
                AI_EvalFunc_OBJRotateLocalX_C(0, 1.39624f);
            }
            v5 = (tGameObjectMain*)v21->des_perso_enterprise;
            if (v5)
            {
                if ((unsigned int)v5 <= 2)
                    v5 = AI_MainActor[(uint32_t)v5 - 1];
                AI_EvalFunc_OBJGetPos_C(v5, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            v4 = (tGameObjectMain*)v21->des_perso_enterprise;
            if (v4)
            {
                if ((unsigned int)v4 <= 2)
                    v4 = AI_MainActor[(uint32_t)v4 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v4, &v12);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v12);
            }
            v17.x = v12.x * 20.0f;
            v17.y = v12.y * 20.0f;
            v17.z = v12.z * 20.0f;
            MATH_VecAddVec(&v22, &a3, &v17);
            AI_EvalFunc_OBJSetPos_C((tGameObjectMain*)ES_ciel_esc_enter_the_map__PJ_Camera_gao, &v22);
            v3 = (tGameObjectMain*)v21->des_perso_enterprise;
            if (v3)
            {
                if ((unsigned int)v3 <= 2)
                    v3 = AI_MainActor[(uint32_t)v3 - 1];
                AI_EvalFunc_OBJGetPos_C(v3, &v13);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v13);
            }
            AI_EvalFunc_OBJSetPos_C((tGameObjectMain*)ES_ciel_esc_enter_the_map__PJ_Camera_Target_gao, &v13);
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)2;
        TIME_Wait_C(2.5);
        result = 0;
    }
    return result;
}

int ES_ciel_esc_exit_the_map()
{
    int result; // eax
    tScriptStruct_ESCiel* v1; // [esp+4h] [ebp-100h]
    tGameObjectMain* v2; // [esp+Ch] [ebp-F8h]
    tGameObjectMain* v3; // [esp+10h] [ebp-F4h]
    tGameObjectMain* v4; // [esp+14h] [ebp-F0h]
    tGameObjectMain* v5; // [esp+88h] [ebp-7Ch]
    tGameObjectMain* v6; // [esp+8Ch] [ebp-78h]
    tGameObjectMain* v7; // [esp+90h] [ebp-74h]
    Vec3 v8; // [esp+94h] [ebp-70h] BYREF
    Vec3 v9; // [esp+A0h] [ebp-64h] BYREF
    float v10; // [esp+ACh] [ebp-58h]
    tGameObjectMain* v11; // [esp+B0h] [ebp-54h]
    int v12; // [esp+B4h] [ebp-50h]
    int v13; // [esp+B8h] [ebp-4Ch]
    Vec3 a3; // [esp+C8h] [ebp-3Ch] BYREF
    BOOL v15; // [esp+D4h] [ebp-30h]
    Vec3 a1; // [esp+D8h] [ebp-2Ch] BYREF
    Vec3 v17; // [esp+E4h] [ebp-20h] BYREF
    tScriptStruct_ESCiel* v18; // [esp+F0h] [ebp-14h]
    float v19; // [esp+F4h] [ebp-10h]
    float v20; // [esp+F8h] [ebp-Ch]
    float v21; // [esp+100h] [ebp-4h]

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ESCiel*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v18 = v1;
    v10 = 0.0;
    v11 = 0;
    v19 = 0.0;
    v13 = 0;
    v21 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        ES_ciel_esc_exit_the_map_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &ES_ciel_esc_exit_the_map_GST_Global_gao_i_mort_en_cours,
            (void*)ES_ciel_esc_exit_the_map_GST_Global_gao,
            3272);
        ES_ciel_esc_exit_the_map_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &ES_ciel_esc_exit_the_map_univVars_world_entrance_angle,
            (void*)ES_ciel_esc_exit_the_map_univVars,
            16);
        AI2C_ResolveBuffer(
            &ES_ciel_esc_exit_the_map_univVars_world_entrance_percent,
            (void*)ES_ciel_esc_exit_the_map_univVars,
            12);
        AI2C_ResolveBuffer(
            &ES_ciel_esc_exit_the_map_GST_Global_gao_o_joy_acteur,
            (void*)ES_ciel_esc_exit_the_map_GST_Global_gao,
            136);
        AI2C_ResolveBuffer(
            &ES_ciel_esc_exit_the_map_GST_Global_gao_i_fade_priority,
            (void*)ES_ciel_esc_exit_the_map_GST_Global_gao,
            3268);
        AI2C_ResolveBuffer(
            &ES_ciel_esc_exit_the_map_GST_Global_gao_f_fade_start_time,
            (void*)ES_ciel_esc_exit_the_map_GST_Global_gao,
            3260);
        AI2C_ResolveBuffer(
            &ES_ciel_esc_exit_the_map_GST_Global_gao_f_fade_delay,
            (void*)ES_ciel_esc_exit_the_map_GST_Global_gao,
            3264);
        AI2C_ResolveBuffer(&ES_ciel_esc_exit_the_map_univVars_i_fade_fx, (void*)ES_ciel_esc_exit_the_map_univVars, 20400);
        ES_ciel_esc_exit_the_map_INV_MatFade_gao = AI_ul_RealResolveOneRefObject(0x5E006934u);
        AI2C_ResolveBuffer(
            &ES_ciel_esc_exit_the_map_univVars_i_special_init,
            (void*)ES_ciel_esc_exit_the_map_univVars,
            6780);
        AI2C_ResolveBuffer(&ES_ciel_esc_exit_the_map_univVars_load_world, (void*)ES_ciel_esc_exit_the_map_univVars, 0);
        AI2C_ResolveBuffer(&ES_ciel_esc_exit_the_map_univVars_world_ID, (void*)ES_ciel_esc_exit_the_map_univVars, 4);
        AI2C_ResolveBuffer(
            &ES_ciel_esc_exit_the_map_univVars_world_entrance_ID,
            (void*)ES_ciel_esc_exit_the_map_univVars,
            8);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)6)
    {
        *(uint32_t*)ES_ciel_esc_exit_the_map_univVars_i_special_init = 0;
        *(uint32_t*)ES_ciel_esc_exit_the_map_univVars_load_world = 1;
        *(uint32_t*)ES_ciel_esc_exit_the_map_univVars_world_ID = v18->des_vers_map_numero;
        *(uint32_t*)ES_ciel_esc_exit_the_map_univVars_world_entrance_ID = v18->des_vers_entree_numero;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1;
    }
    else
    {
        v11 = *AI_MainActor;
        v7 = v11;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v7 = AI_MainActor[(uint32_t)v11 - 1];
            v6 = v7;
        }
        else
        {
            v6 = 0;
        }
        if (v6)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v17);
            v19 = v17.z;
            v5 = v11;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v5 = AI_MainActor[(uint32_t)v11 - 1];
                AI_EvalFunc_OBJGetPos_C(v5, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            a1 = a3;
            if (*(uint32_t*)ES_ciel_esc_exit_the_map_GST_Global_gao_i_mort_en_cours == -1 && a1.z > (double)v19)
            {
                a1.z = 0.0;
                v9.x = 1.0;
                v9.y = 0.0;
                v9.z = 0.0;
                v8.x = 0.0;
                v8.y = 0.0;
                v8.z = 1.0;
                v10 = MATH_f_VecAngle(&v9, &a1, &v8);
                *(float*)ES_ciel_esc_exit_the_map_univVars_world_entrance_angle = v10;
                v20 = MATH_VecLen(&a1);
                v21 = v20 / v18->des_diametre_surface;
                *(float*)ES_ciel_esc_exit_the_map_univVars_world_entrance_percent = v21;
                v4 = *(tGameObjectMain**)ES_ciel_esc_exit_the_map_GST_Global_gao_o_joy_acteur;
                if (*(uint32_t*)ES_ciel_esc_exit_the_map_GST_Global_gao_o_joy_acteur)
                {
                    if ((unsigned int)v4 <= 2)
                        v4 = AI_MainActor[(uint32_t)v4 - 1];
                    v3 = v4;
                }
                else
                {
                    v3 = 0;
                }
                if (!v3)
                    v18->i_must_freeze_joy = 1;
                v12 = SND_i_Seti(9, 1);
                if (*(int*)ES_ciel_esc_exit_the_map_GST_Global_gao_i_fade_priority <= 5000
                    || (v15 = AI_EvalFunc_TIMEElapsed_C(
                        *(float*)ES_ciel_esc_exit_the_map_GST_Global_gao_f_fade_start_time,
                        *(float*)ES_ciel_esc_exit_the_map_GST_Global_gao_f_fade_delay)))
                {
                    *(float*)ES_ciel_esc_exit_the_map_GST_Global_gao_f_fade_start_time = TIM_gf_MainClock - TIM_gf_LockTime;
                    *(uint32_t*)ES_ciel_esc_exit_the_map_GST_Global_gao_f_fade_delay = 0x3F99999A;
                    *(uint32_t*)ES_ciel_esc_exit_the_map_GST_Global_gao_i_fade_priority = 5000;
                    AI_EvalFunc_GFXDel_C_CURRENT(10);
                    v13 = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
                    *(uint32_t*)ES_ciel_esc_exit_the_map_univVars_i_fade_fx = v13;
                    AI_EvalFunc_GFXFlagSet_C_CURRENT(v13, 0, 1);
                    AI_EvalFunc_GFXFlagSet_C_CURRENT(v13, 2, 1);
                    v2 = (tGameObjectMain*)ES_ciel_esc_exit_the_map_INV_MatFade_gao;
                    if (ES_ciel_esc_exit_the_map_INV_MatFade_gao)
                    {
                        if ((unsigned int)ES_ciel_esc_exit_the_map_INV_MatFade_gao <= 2)
                            v2 = AI_MainActor[ES_ciel_esc_exit_the_map_INV_MatFade_gao - 1];
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v13, v2, 9);
                    }
                    else
                    {
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v13, 0, 9);
                    }
                    GFXSetCurGlobal(v13);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(11101, 0xFF000000);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 0.2f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 1.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
                }
                else
                {
                    v13 = -1;
                }
                *AI_gppst_CurrentJumpNode = (tAINode*)6;
                TIME_Wait_C(0.40000001f);
                result = 0;
            }
            else
            {
                result = 1000;
            }
        }
        else
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
    }
    return result;
}

int ES_ciel_esc_joy()
{
    tScriptStruct_ESCiel* v1; // [esp+0h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ESCiel*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        ES_ciel_esc_joy_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
    }
    if (v1->i_must_freeze_joy)
        AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)ES_ciel_esc_joy_GST_Global_gao, 0x8000, 0);
    return 1000;
}