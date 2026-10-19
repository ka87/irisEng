#include "ai_compiledscripts_inc.h"

//Fun_Mouette_mouette_init
int fct_Fun_Mouette_mouette_init_Fun_Mouette_mouette_move;
int fct_Fun_Mouette_mouette_init_Fun_Mouette_mouette_reflexe;
int Fun_Mouette_mouette_init_INV_manager_gao;
int Fun_Mouette_mouette_init_INV_manager_gao_i_Animals_Nb;
int Fun_Mouette_mouette_init_INV_manager_gao_ao_Animals;
int Fun_Mouette_mouette_init_INV_manager_gao_ai_AnimalsData;
int Fun_Mouette_mouette_init_GST_Global_gao;
int Fun_Mouette_mouette_init_GST_Global_gao_o_targets;
int Fun_Mouette_mouette_init_GST_Global_gao_i_targets_nb;
int Fun_Mouette_mouette_init_GST_Global_gao_v_targets_offset;
int Fun_Mouette_mouette_init_univVars;
int Fun_Mouette_mouette_init_univVars_Planetes_CurHour;
int fct_Fun_Mouette_mouette_init_Fun_Mouette_mouette_gotonext;

//Fun_Mouette_mouette_follow
int fct_Fun_Mouette_mouette_follow_Fun_Mouette_mouette_gotonext;

//Fun_Mouette_mouette_gotonext
int Fun_Mouette_mouette_gotonext_univVars;
int Fun_Mouette_mouette_gotonext_univVars_Planetes_CurHour;

//Fun_Mouette_mouette_reflexe
int Fun_Mouette_mouette_reflexe_univVars;
int Fun_Mouette_mouette_reflexe_univVars_Planetes_CurHour;

int Fun_Mouette_mouette_init()
{
    int result; // eax
    int v1; // eax
    tScriptStruct_ActorMouette* v2; // [esp+8h] [ebp-D4h]
    tGameObjectMain* v3; // [esp+10h] [ebp-CCh]
    tGameObjectMain* v4; // [esp+14h] [ebp-C8h]
    tGameObjectMain* v5; // [esp+18h] [ebp-C4h]
    tGameObjectMain* a3; // [esp+2Ch] [ebp-B0h]
    tGameObjectMain* v7; // [esp+30h] [ebp-ACh]
    tGameObjectMain* v8; // [esp+3Ch] [ebp-A0h]
    Vec3 v9; // [esp+50h] [ebp-8Ch] BYREF
    float v10; // [esp+70h] [ebp-6Ch]
    BOOL v11; // [esp+74h] [ebp-68h]
    int v12; // [esp+78h] [ebp-64h]
    Vec3 v13; // [esp+80h] [ebp-5Ch] BYREF
    float v14; // [esp+8Ch] [ebp-50h]
    int v15; // [esp+90h] [ebp-4Ch]
    Vec3 v16; // [esp+98h] [ebp-44h] BYREF
    float v17; // [esp+A4h] [ebp-38h]
    tGameObjectMain* v18; // [esp+A8h] [ebp-34h]
    int v19; // [esp+ACh] [ebp-30h]
    int v20; // [esp+B4h] [ebp-28h]
    tScriptStruct_ActorMouette* v21; // [esp+B8h] [ebp-24h]
    Vec3 a2; // [esp+BCh] [ebp-20h] BYREF
    float v23; // [esp+D4h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorMouette*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v21 = v2;
    v15 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Fun_Mouette_mouette_init_Fun_Mouette_mouette_move = (int)AI_ul_RealResolveOneRefFunction(0x5B0004E1u);
        fct_Fun_Mouette_mouette_init_Fun_Mouette_mouette_reflexe = (int)AI_ul_RealResolveOneRefFunction(0x3D00065Du);
        Fun_Mouette_mouette_init_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &Fun_Mouette_mouette_init_INV_manager_gao_i_Animals_Nb,
            (void*)Fun_Mouette_mouette_init_INV_manager_gao,
            23888);
        AI2C_ResolveBuffer(
            &Fun_Mouette_mouette_init_INV_manager_gao_ao_Animals,
            (void*)Fun_Mouette_mouette_init_INV_manager_gao,
            23368);
        AI2C_ResolveBuffer(
            &Fun_Mouette_mouette_init_INV_manager_gao_ai_AnimalsData,
            (void*)Fun_Mouette_mouette_init_INV_manager_gao,
            23628);
        Fun_Mouette_mouette_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Fun_Mouette_mouette_init_GST_Global_gao_o_targets,
            (void*)Fun_Mouette_mouette_init_GST_Global_gao,
            964);
        AI2C_ResolveBuffer(
            &Fun_Mouette_mouette_init_GST_Global_gao_i_targets_nb,
            (void*)Fun_Mouette_mouette_init_GST_Global_gao,
            2572);
        AI2C_ResolveBuffer(
            &Fun_Mouette_mouette_init_GST_Global_gao_v_targets_offset,
            (void*)Fun_Mouette_mouette_init_GST_Global_gao,
            1368);
        Fun_Mouette_mouette_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Fun_Mouette_mouette_init_univVars_Planetes_CurHour,
            (void*)Fun_Mouette_mouette_init_univVars,
            6116);
        fct_Fun_Mouette_mouette_init_Fun_Mouette_mouette_gotonext = (int)AI_ul_RealResolveOneRefFunction(0x5B0004DFu);
    }
    if (*AI_gppst_CurrentJumpNode)
    {
        *AI_gppst_CurrentJumpNode = 0;
        TIME_Wait_C(0.5);
        result = 0;
    }
    else
    {
        AI_EvalFunc_AITrackChange_C(3, (tAIFunction*)fct_Fun_Mouette_mouette_init_Fun_Mouette_mouette_move);
        AI_EvalFunc_AITrackChange_C(4, (tAIFunction*)fct_Fun_Mouette_mouette_init_Fun_Mouette_mouette_reflexe);
        if (*(int*)Fun_Mouette_mouette_init_INV_manager_gao_i_Animals_Nb < 64)
        {
            *(uint32_t*)(Fun_Mouette_mouette_init_INV_manager_gao_ao_Animals
                + 4 * *(uint32_t*)Fun_Mouette_mouette_init_INV_manager_gao_i_Animals_Nb
                + 4) = (uint32_t)AI_gpst_CurrentGameObject;
            *(uint32_t*)(Fun_Mouette_mouette_init_INV_manager_gao_ai_AnimalsData
                + 4 * (*(uint32_t*)Fun_Mouette_mouette_init_INV_manager_gao_i_Animals_Nb)++
                + 4) = (v21->i_MapEtage << 8) + 14;
        }
        v20 = AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 8, 0);
        v18 = AI_gpst_CurrentGameObject;
        v7 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v7 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            a3 = v7;
        }
        else
        {
            a3 = 0;
        }
        v15 = AI_EvalFunc_ARRObjSearch_C(
            (tGameObjectMain**)(Fun_Mouette_mouette_init_GST_Global_gao_o_targets + 4),
            *(uint32_t*)Fun_Mouette_mouette_init_GST_Global_gao_i_targets_nb,
            a3);
        if (v15 == -1)
        {
            v15 = AI_EvalFunc_ARRObjSearch_C(
                (tGameObjectMain**)(Fun_Mouette_mouette_init_GST_Global_gao_o_targets + 4),
                100,
                0);
            if (v15 != -1)
            {
                ++* (uint32_t*)Fun_Mouette_mouette_init_GST_Global_gao_i_targets_nb;
                *(uint32_t*)(Fun_Mouette_mouette_init_GST_Global_gao_o_targets + 4 * v15 + 4) = (uint32_t)AI_gpst_CurrentGameObject;
                *(uint32_t*)AI_EvalVarArray_C3((char*)Fun_Mouette_mouette_init_GST_Global_gao_v_targets_offset, v15) = 0;
                *((uint32_t*)AI_EvalVarArray_C3((char*)Fun_Mouette_mouette_init_GST_Global_gao_v_targets_offset, v15) + 1) = 0;
                *((uint32_t*)AI_EvalVarArray_C3((char*)Fun_Mouette_mouette_init_GST_Global_gao_v_targets_offset, v15) + 2) = 0;
            }
        }
        v21->des_f_speed = v21->des_f_speed * 0.60000002f;
        AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 64, 0);
        v8 = AI_gpst_CurrentGameObject;
        AI_EvalFunc_OBJ_InfoPhotoParamSet_C(AI_gpst_CurrentGameObject, 4, 14, -1, -1, 0.075000003f, 1.0, 0.69999999f, 1.2f);
        v16.x = 0.0;
        v16.y = -0.25;
        v16.z = 0.0;
        AI_EvalFunc_OBJ_InfoPhotoInfoOffsetSet_C(AI_gpst_CurrentGameObject, &v16);
        a2.x = 0.0;
        a2.y = -0.25;
        a2.z = 0.0;
        AI_EvalFunc_OBJ_InfoPhotoSphereOffsetSet_C(AI_gpst_CurrentGameObject, &a2);
        v5 = v8;
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v5 = AI_MainActor[(uint32_t)v8 - 1];
            v4 = v5;
        }
        else
        {
            v4 = 0;
        }
        v4->extended->fields->designInt3 = 0;
        AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, v21->des_f_zoom);
        v9.x = -v21->des_f_zoom;
        v9.y = -v21->des_f_zoom;
        v9.z = -v21->des_f_zoom;
        AI_EvalFunc_BVMinSet_C(AI_gpst_CurrentGameObject, &v9);
        v13.x = v21->des_f_zoom;
        v13.y = v21->des_f_zoom;
        v13.z = v21->des_f_zoom;
        AI_EvalFunc_BVMaxSet_C(AI_gpst_CurrentGameObject, &v13);
        v21->f_snd_delay = AI_EvalFunc_MATHRandFloat_C(1.0, 2.0);
        v21->f_snd_time = TIM_gf_MainClock - TIM_gf_LockTime;
        v21->f_time_leve = AI_EvalFunc_MATHRandFloat_C(5.5, 6.5);
        v21->f_time_couche = AI_EvalFunc_MATHRandFloat_C(16.5, 17.5);
        v21->i_forcehide = v21->f_time_leve >= (double)*(float*)Fun_Mouette_mouette_init_univVars_Planetes_CurHour
            || v21->f_time_couche <= (double)*(float*)Fun_Mouette_mouette_init_univVars_Planetes_CurHour;
        v11 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
        AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 4, 0);
        v12 = AI_EvalFunc_MATHRandInt_C(1, 20);
        AI_EvalFunc_ANICurrentFrameSet_C(AI_gpst_CurrentGameObject, 0, v12);
        v1 = (uint32_t)AI_EvalFunc_WAYNearestWPOfObj_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v21->des_network, -1, 0, 0);
        v21->cur_wp = v1;
        v14 = TIM_gf_MainClock - TIM_gf_LockTime;
        v23 = AI_EvalFunc_MATHRandFloat_C(0.0, 1000.0);
        v21->f_delaybeg = v14 + v23;
        v17 = TIM_gf_MainClock - TIM_gf_LockTime;
        v10 = AI_EvalFunc_MATHRandFloat_C(0.0, 4.0);
        v21->f_delay = v17 + v10;
        v21->f_speed = v21->des_f_speed;
        v3 = (tGameObjectMain*)v21->cur_wp;
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            AI_EvalFunc_BVRandomPosGet_C(v3, 0, &v21->v_postogo);
        }
        else
        {
            AI_EvalFunc_BVRandomPosGet_C(0, 0, &v21->v_postogo);
        }
        v19 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Fun_Mouette_mouette_init_Fun_Mouette_mouette_gotonext);
        if (v19 == 1000)
            result = 1000;
        else
            result = v19;
    }
    return result;
}

int Fun_Mouette_mouette_move()
{
    float a4; // [esp+0h] [ebp-118h]
    tScriptStruct_ActorMouette* v2; // [esp+8h] [ebp-110h]
    tGameObjectMain* v3; // [esp+10h] [ebp-108h]
    float v4; // [esp+24h] [ebp-F4h]
    Vec3 v5; // [esp+28h] [ebp-F0h] BYREF
    Vec3 a2; // [esp+34h] [ebp-E4h] BYREF
    Vec3 v7; // [esp+40h] [ebp-D8h] BYREF
    float v8; // [esp+4Ch] [ebp-CCh]
    float v9; // [esp+50h] [ebp-C8h]
    Vec3 a5[8]; // [esp+54h] [ebp-C4h] BYREF
    int v11; // [esp+B8h] [ebp-60h]
    Vec3 a3[2]; // [esp+C0h] [ebp-58h] BYREF
    Vec3 v14; // [esp+DCh] [ebp-3Ch] BYREF
    Vec3 v15; // [esp+E8h] [ebp-30h] BYREF
    Vec3 v16; // [esp+F4h] [ebp-24h] BYREF
    tScriptStruct_ActorMouette* v17; // [esp+100h] [ebp-18h]
    Vec3 v18; // [esp+104h] [ebp-14h] BYREF
    float v19; // [esp+110h] [ebp-8h]

    v7.x = 1.0;
    v7.y = 0.0;
    v7.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorMouette*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v17 = v2;
    v8 = 0.0;
    v11 = AI_EvalFunc_DYNFrictionSet_C(AI_gpst_CurrentGameObject, v2->des_f_friction);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v16);
    v4 = v17->f_speed;
    v14.x = v16.x * v4;
    v14.y = v16.y * v4;
    v14.z = v16.z * v4;
    AI_EvalFunc_DYNTractionSet_C_CURRENT(&v14);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v5);
    MATH_SubstractVec(a3, &v17->v_postogo, &v5);
    v19 = TIM_gf_dt;
    a4 = 2.0f * TIM_gf_dt;
    AI_EvalFunc_MATHVecBlendRotate_C(&a2, a3, a4, a5);
    v7 = a5[0];
    v15.x = 0.0;
    v15.y = 0.0;
    v15.z = 1.0;
    AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v7, &v15);
    AI_EvalFunc_ACTActionSet_C_CURRENT(v17->i_state);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v18);
    v7 = v18;
    AI_EvalFunc_OBJSetPos_C_CURRENT(&v7);
    if (v17->i_state)
        v8 = 2.0;
    else
        v8 = 1.0;
    if (AI_EvalFunc_TIMEElapsed_C(v17->f_delay, v8))
    {
        v3 = (tGameObjectMain*)v17->cur_wp;
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            AI_EvalFunc_BVRandomPosGet_C(v3, 0, &v17->v_postogo);
        }
        else
        {
            AI_EvalFunc_BVRandomPosGet_C(0, 0, &v17->v_postogo);
        }
        v9 = AI_EvalFunc_MATHRandFloat_C(-1.0, 1.0);
        v17->f_speed = v17->des_f_speed + v9;
        v17->f_delay = TIM_gf_MainClock - TIM_gf_LockTime;
        v17->i_state = AI_EvalFunc_MATHRandInt_C(0, 2);
        v17->f_delay = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    return 1000;
}

int Fun_Mouette_mouette_follow()
{
    tGameObjectMain* v0; // eax
    tScriptStruct_ActorMouette* v2; // [esp+0h] [ebp-ACh]
    tGameObjectMain* v3; // [esp+10h] [ebp-9Ch]
    tGameObjectMain* v4; // [esp+18h] [ebp-94h]
    tGameObjectMain* v5; // [esp+20h] [ebp-8Ch]
    tGameObjectMain* v6; // [esp+34h] [ebp-78h]
    float v7; // [esp+3Ch] [ebp-70h]
    int v8; // [esp+40h] [ebp-6Ch]
    Vec3 v9; // [esp+44h] [ebp-68h] BYREF
    Vec3 v10; // [esp+50h] [ebp-5Ch] BYREF
    Vec3 v11; // [esp+5Ch] [ebp-50h] BYREF
    Vec3 dest; // [esp+74h] [ebp-38h] BYREF
    Vec3 v13; // [esp+80h] [ebp-2Ch] BYREF
    Vec3 a3; // [esp+8Ch] [ebp-20h] BYREF
    tScriptStruct_ActorMouette* v15; // [esp+98h] [ebp-14h]
    Vec3 v16; // [esp+9Ch] [ebp-10h] BYREF

    v11.x = 1.0;
    v11.y = 0.0;
    v11.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorMouette*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v15 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Fun_Mouette_mouette_follow_Fun_Mouette_mouette_gotonext = (int)AI_ul_RealResolveOneRefFunction(0x5B0004DFu);
    }
    v15->o_follow = v15->des_o_follow;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v9);
    v6 = (tGameObjectMain*)v15->o_follow;
    if (v6)
    {
        if ((unsigned int)v6 <= 2)
            v6 = AI_MainActor[(uint32_t)v6 - 1];
        AI_EvalFunc_OBJGetPos_C(v6, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    MATH_SubstractVec(&dest, &v9, &a3);
    v11 = dest;
    v7 = MATH_VecLen(&v11);
    if (v15->des_f_maxfollow * 1.5 < v7)
    {
        v0 = AI_EvalFunc_WAYNearestWPOfObj_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v15->des_network, -1, 0, 0);
        v15->cur_wp = (int)v0;
        v5 = (tGameObjectMain*)v15->cur_wp;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            AI_EvalFunc_BVRandomPosGet_C(v5, 0, &v15->v_postogo);
        }
        else
        {
            AI_EvalFunc_BVRandomPosGet_C(0, 0, &v15->v_postogo);
        }
        v15->i_attwp = 1;
        v15->f_delayattwp = TIM_gf_MainClock - TIM_gf_LockTime;
        v15->o_follow = 0;
        v8 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Fun_Mouette_mouette_follow_Fun_Mouette_mouette_gotonext);
        if (v8 != 1000)
            return v8;
    }
    v4 = (tGameObjectMain*)v15->des_o_follow;
    if (v4)
    {
        if ((unsigned int)v4 <= 2)
            v4 = AI_MainActor[(uint32_t)v4 - 1];
        AI_EvalFunc_OBJGetPos_C(v4, &v13);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v13);
    }
    v3 = (tGameObjectMain*)v15->des_o_follow;
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        AI_EvalFunc_MATHVecLocalToGlobal_C(v3, &v15->v_posfollow, &v16);
    }
    else
    {
        AI_EvalFunc_MATHVecLocalToGlobal_C(0, &v15->v_posfollow, &v16);
    }
    MATH_VecAddVec(&v10, &v13, &v16);
    v15->v_postogo = v10;
    return 1000;
}

int Fun_Mouette_mouette_gotonext()
{
    BOOL v0; // eax
    tGameObjectMain* v1; // eax
    tGameObjectMain* v2; // eax
    float v4; // [esp+8h] [ebp-100h]
    float v5; // [esp+Ch] [ebp-FCh]
    tScriptStruct_ActorMouette* v6; // [esp+10h] [ebp-F8h]
    tGameObjectMain* v7; // [esp+28h] [ebp-E0h]
    tGameObjectMain* v8; // [esp+30h] [ebp-D8h]
    tGameObjectMain* v9; // [esp+38h] [ebp-D0h]
    tGameObjectMain* v10; // [esp+48h] [ebp-C0h]
    tGameObjectMain* v11; // [esp+50h] [ebp-B8h]
    tGameObjectMain* v12; // [esp+54h] [ebp-B4h]
    tGameObjectMain* v13; // [esp+58h] [ebp-B0h]
    tGameObjectMain* v14; // [esp+5Ch] [ebp-ACh]
    tGameObjectMain* v15; // [esp+60h] [ebp-A8h]
    tGameObjectMain* v16; // [esp+68h] [ebp-A0h]
    Vec3 v17; // [esp+70h] [ebp-98h] BYREF
    Vec3 a3; // [esp+7Ch] [ebp-8Ch] BYREF
    Vec3 v19; // [esp+88h] [ebp-80h]
    Vec3 v20; // [esp+94h] [ebp-74h] BYREF
    Vec3 v21; // [esp+A0h] [ebp-68h] BYREF
    float v22; // [esp+B8h] [ebp-50h]
    Vec3 dest; // [esp+C4h] [ebp-44h] BYREF
    Vec3 v25; // [esp+D0h] [ebp-38h] BYREF
    float v26; // [esp+E4h] [ebp-24h]
    tGameObjectMain* v27; // [esp+E8h] [ebp-20h]
    BOOL v28; // [esp+ECh] [ebp-1Ch]
    tScriptStruct_ActorMouette* v29; // [esp+F0h] [ebp-18h]
    Vec3 v30; // [esp+F4h] [ebp-14h] BYREF
    float v31; // [esp+104h] [ebp-4h]

    v19.x = 1.0;
    v19.y = 0.0;
    v19.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_ActorMouette*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v29 = v6;
    v27 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Fun_Mouette_mouette_gotonext_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Fun_Mouette_mouette_gotonext_univVars_Planetes_CurHour,
            (void*)Fun_Mouette_mouette_gotonext_univVars,
            6116);
    }
    if (v29->i_attwp)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v29->f_delayattwp, 1.0))
            v29->i_attwp = 0;
    }
    v16 = (tGameObjectMain*)v29->cur_wp;
    if (v16)
    {
        if ((unsigned int)v16 <= 2)
            v16 = AI_MainActor[(uint32_t)v16 - 1];
        v0 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, v16);
    }
    else
    {
        v0 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, 0);
    }
    v28 = v0;
    if (v0)
    {
        v15 = (tGameObjectMain*)v29->cur_wp;
        if (v15)
        {
            if ((unsigned int)v15 <= 2)
                v15 = AI_MainActor[(uint32_t)v15 - 1];
            v14 = v15;
        }
        else
        {
            v14 = 0;
        }
        v13 = (tGameObjectMain*)v29->des_o_coucher;
        if (v13)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            v12 = v13;
        }
        else
        {
            v12 = 0;
        }
        if (v14 == v12)
        {
            if (*(float*)Fun_Mouette_mouette_gotonext_univVars_Planetes_CurHour <= 17.0)
            {
                if (*(float*)Fun_Mouette_mouette_gotonext_univVars_Planetes_CurHour > 5.0)
                    v29->i_ishere = 1;
            }
            else
            {
                v29->i_ishere = 0;
            }
        }
        v11 = (tGameObjectMain*)v29->cur_wp;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v11 = AI_MainActor[(uint32_t)v11 - 1];
            v1 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v29->des_network, v11, 1, 0);
        }
        else
        {
            v1 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v29->des_network, 0, 1, 0);
        }
        v27 = v1;
        v10 = v1;
        if (v1)
        {
            if ((unsigned int)v1 <= 2)
                v10 = AI_MainActor[(uint32_t)v1 - 1];
            AI_EvalFunc_OBJGetPos_C(v10, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v17);
        MATH_SubstractVec(&dest, &a3, &v17);
        AI_EvalFunc_MATHVecNormalize_C(&dest, &v30);
        v19 = v30;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v21);
        v26 = v21.z * v19.z + v21.y * v19.y + v19.x * v21.x;
        if (v26 < 0.0)
        {
            v9 = (tGameObjectMain*)v29->cur_wp;
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                v2 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v29->des_network, v9, 3, 0);
            }
            else
            {
                v2 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v29->des_network, 0, 3, 0);
            }
            v27 = v2;
        }
        v29->cur_wp = (int)v27;
        v8 = (tGameObjectMain*)v29->cur_wp;
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v8 = AI_MainActor[(uint32_t)v8 - 1];
            AI_EvalFunc_BVRandomPosGet_C(v8, 0, &v29->v_postogo);
        }
        else
        {
            AI_EvalFunc_BVRandomPosGet_C(0, 0, &v29->v_postogo);
        }
        v7 = (tGameObjectMain*)v29->cur_wp;
        if (v7)
        {
            if ((unsigned int)v7 <= 2)
                v7 = AI_MainActor[(uint32_t)v7 - 1];
            AI_EvalFunc_OBJGetPos_C(v7, &v25);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v25);
        }
        v19 = v25;
        v29->v_postogo.z = v25.z;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v20);
        v19 = v20;
        if (v29->v_postogo.z <= (double)v20.z || (v5 = (float)fabs(v19.z - v29->v_postogo.z), v31 = v5, v5 <= 3.0))
        {
            if (v29->v_postogo.z > (double)v19.z)
            {
                v4 = (float)fabs(v19.z - v29->v_postogo.z);
                v22 = v4;
                if (v4 > 3.0)
                {
                    v29->f_delay = TIM_gf_MainClock - TIM_gf_LockTime;
                    v29->i_state = 0;
                }
            }
        }
        else
        {
            v29->f_delay = TIM_gf_MainClock - TIM_gf_LockTime;
            v29->i_state = 1;
        }
    }
    return 1000;
}

int Fun_Mouette_mouette_reflexe()
{
    int result; // eax
    tScriptStruct_ActorMouette* v1; // [esp+8h] [ebp-2Ch]
    float v2; // [esp+1Ch] [ebp-18h]
    int v3; // [esp+20h] [ebp-14h]
    float v4; // [esp+28h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ActorMouette*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Fun_Mouette_mouette_reflexe_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Fun_Mouette_mouette_reflexe_univVars_Planetes_CurHour,
            (void*)Fun_Mouette_mouette_reflexe_univVars,
            6116);
    }
    if (v1->des_i_always_visible)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        if (v1->f_time_couche >= (double)*(float*)Fun_Mouette_mouette_reflexe_univVars_Planetes_CurHour)
        {
            if (v1->f_time_leve < (double)*(float*)Fun_Mouette_mouette_reflexe_univVars_Planetes_CurHour)
            {
                v4 = AI_EvalFunc_OBJ_LodVisGet_C(AI_gpst_CurrentGameObject);
                if (v4 == 0.0)
                    v1->i_ishere = 1;
            }
        }
        else
        {
            v2 = AI_EvalFunc_OBJ_LodVisGet_C(AI_gpst_CurrentGameObject);
            if (v2 == 0.0)
                v1->i_ishere = 0;
        }
        if (v1->i_forcehide)
        {
            v1->i_ishere = 0;
            v1->i_forcehide = 0;
        }
        if (v1->i_ishere)
        {
            if (v1->i_washere != v1->i_ishere)
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
            if (AI_EvalFunc_TIMEElapsed_C(v1->f_snd_time, v1->f_snd_delay))
            {
                v1->f_snd_delay = AI_EvalFunc_MATHRandFloat_C(2.0, 4.0);
                v1->f_snd_time = TIM_gf_MainClock - TIM_gf_LockTime;
                v3 = AI_EvalFunc_MATHRandInt_C(0, 2);
                AI_EvalFunc_SNDRequestPlay_C_CURRENT(v3, -1.0);
            }
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
            v1->f_snd_time = TIM_gf_MainClock - TIM_gf_LockTime;
        }
        v1->i_washere = v1->i_ishere;
        result = 1000;
    }
    return result;
}