#include "ai_compiledscripts_inc.h"


//SFX_XMEN_SFX_XMEN_init
int fct_SFX_XMEN_SFX_XMEN_init_SFX_XMEN_SFX_XMEN_afterrec_init;
int fct_SFX_XMEN_SFX_XMEN_init_SFX_XMEN_SFX_XMEN_afterblend_init;
int fct_SFX_XMEN_SFX_XMEN_init_SFX_XMEN_SFX_XMEN_reflex;

//SFX_XMEN_SFX_XMEN_afterrec_init
int fct_SFX_XMEN_SFX_XMEN_afterrec_init_SFX_XMEN_SFX_XMEN_afterblend_init;
int fct_SFX_XMEN_SFX_XMEN_afterrec_init_SFX_XMEN_SFX_XMEN_afterrec_init;
int fct_SFX_XMEN_SFX_XMEN_afterrec_init_SFX_XMEN_SFX_XMEN_set_moving_function;
int fct_SFX_XMEN_SFX_XMEN_afterrec_init_SFX_XMEN_SFX_XMEN_exec_GFX_Halo;

//SFX_XMEN_SFX_XMEN_afterblend_init
int fct_SFX_XMEN_SFX_XMEN_afterblend_init_SFX_XMEN_SFX_XMEN_afterblend_init;
int fct_SFX_XMEN_SFX_XMEN_afterblend_init_SFX_XMEN_SFX_XMEN_afterrec_init;
int fct_SFX_XMEN_SFX_XMEN_afterblend_init_SFX_XMEN_SFX_XMEN_set_moving_function;
int fct_SFX_XMEN_SFX_XMEN_afterblend_init_SFX_XMEN_SFX_XMEN_exec_GFX_Halo;

//SFX_XMEN_SFX_XMEN_set_moving_function
int fct_SFX_XMEN_SFX_XMEN_set_moving_function_SFX_XMEN_SFX_XMEN_afterrec_move;
int fct_SFX_XMEN_SFX_XMEN_set_moving_function_SFX_XMEN_SFX_XMEN_afterblend_move;

//SFX_XMEN_SFX_XMEN_afterrec_move
int fct_SFX_XMEN_SFX_XMEN_afterrec_move_SFX_XMEN_SFX_XMEN_afterblend_move;

//SFX_XMEN_SFX_XMEN_afterblend_move
int fct_SFX_XMEN_SFX_XMEN_afterblend_move_SFX_XMEN_SFX_XMEN_afterrec_move;
int fct_SFX_XMEN_SFX_XMEN_afterblend_move_SFX_XMEN_SFX_XMEN_afterblend_move;
int fct_SFX_XMEN_SFX_XMEN_afterblend_move_SFX_XMEN_SFX_XMEN_Destroy;
int fct_SFX_XMEN_SFX_XMEN_afterblend_move_SFX_XMEN_SFX_XMEN_exec_GFX_Halo;

//SFX_XMEN_SFX_XMEN_reflex
int fct_SFX_XMEN_SFX_XMEN_reflex_SFX_XMEN_SFX_XMEN_afterrec_init;
int fct_SFX_XMEN_SFX_XMEN_reflex_SFX_XMEN_SFX_XMEN_afterrec_move;

//SFX_XMEN_SFX_XMEN_exec_GFX_Halo
int SFX_XMEN_SFX_XMEN_exec_GFX_Halo_SFX_light_smoke_gao;

int SFX_XMEN_SFX_XMEN_init()
{
    tGameObjectMain* v0; // eax
    BOOL v1; // eax
    BOOL v2; // eax
    float v4; // [esp+8h] [ebp-C8h]
    tScriptStruct_SFXXMEN* v5; // [esp+Ch] [ebp-C4h]
    tAI* v6; // [esp+10h] [ebp-C0h]
    tGameObjectMain* v7; // [esp+14h] [ebp-BCh]
    tGameObjectMain* v8; // [esp+18h] [ebp-B8h]
    tGameObjectMain* v9; // [esp+20h] [ebp-B0h]
    float v10; // [esp+24h] [ebp-ACh]
    tGameObjectMain* v11; // [esp+2Ch] [ebp-A4h]
    tGameObjectMain* v12; // [esp+34h] [ebp-9Ch]
    tGameObjectMain* v13; // [esp+3Ch] [ebp-94h]
    tGameObjectMain* v14; // [esp+44h] [ebp-8Ch]
    tGameObjectMain* v15; // [esp+4Ch] [ebp-84h]
    tGameObjectMain* v16; // [esp+58h] [ebp-78h]
    tGameObjectMain* v17; // [esp+5Ch] [ebp-74h]
    tGameObjectMain* v18; // [esp+64h] [ebp-6Ch]
    tGameObjectMain* v19; // [esp+68h] [ebp-68h]
    tGameObjectMain* v20; // [esp+6Ch] [ebp-64h]
    tGameObjectMain* v21; // [esp+78h] [ebp-58h]
    int v22; // [esp+90h] [ebp-40h]
    Vec3 pos; // [esp+94h] [ebp-3Ch] BYREF
    Vec3 a3; // [esp+A4h] [ebp-2Ch] BYREF
    float v26; // [esp+B8h] [ebp-18h]
    tScriptStruct_SFXXMEN* v27; // [esp+C0h] [ebp-10h]
    float v28; // [esp+C4h] [ebp-Ch]
    int v29; // [esp+C8h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_SFXXMEN*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v27 = v5;

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_XMEN_SFX_XMEN_init_SFX_XMEN_SFX_XMEN_afterrec_init = (int)AI_ul_RealResolveOneRefFunction(0x72005558u);
        fct_SFX_XMEN_SFX_XMEN_init_SFX_XMEN_SFX_XMEN_afterblend_init = (int)AI_ul_RealResolveOneRefFunction(0x72005559u);
        fct_SFX_XMEN_SFX_XMEN_init_SFX_XMEN_SFX_XMEN_reflex = (int)AI_ul_RealResolveOneRefFunction(0x72005563u);
    }
    XMEN_OnOff(AI_gpst_CurrentGameObject, 0, 0);
    AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
    if (AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 0x20000))
    {
        v20 = (tGameObjectMain*)v27->des_start_actor;
        if (v27->des_start_actor)
        {
            if ((unsigned int)v20 <= 2)
                v20 = AI_MainActor[(uint32_t)v20 - 1];
            v19 = v20;
        }
        else
        {
            v19 = 0;
        }
        if (!v19)
            v27->des_start_actor = (int)AI_gpst_CurrentGameObject;
        if (v27->des_start_bone_num == -1)
        {
            v27->o_bone = v27->des_start_actor;
        }
        else
        {
            v18 = (tGameObjectMain*)v27->des_start_actor;
            if (v27->des_start_actor)
            {
                if ((unsigned int)v18 <= 2)
                    v18 = AI_MainActor[(uint32_t)v18 - 1];
                v0 = AI_EvalFunc_ANICanalGaoGet_C(v18, v27->des_start_bone_num);
            }
            else
            {
                v0 = AI_EvalFunc_ANICanalGaoGet_C(0, v27->des_start_bone_num);
            }
            v27->o_bone = (int)v0;
            v17 = (tGameObjectMain*)v27->o_bone;
            if (v17)
            {
                if ((unsigned int)v17 <= 2)
                    v17 = AI_MainActor[(uint32_t)v17 - 1];
                v16 = v17;
            }
            else
            {
                v16 = 0;
            }
            if (!v16)
                v27->o_bone = v27->des_start_actor;
        }
        v15 = (tGameObjectMain*)v27->des_start_actor;
        if (v27->des_start_actor)
        {
            if ((unsigned int)v15 <= 2)
                v15 = AI_MainActor[(uint32_t)v15 - 1];
            v1 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v15, 32);
        }
        else
        {
            v1 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 32);
        }
        if (v1)
        {
            v27->o_afterrec_gao = v27->des_start_actor;
            v14 = (tGameObjectMain*)v27->o_afterrec_gao;
            if (v14)
            {
                if ((unsigned int)v14 <= 2)
                    v14 = AI_MainActor[(uint32_t)v14 - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v14,
                    2,
                    (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_init_SFX_XMEN_SFX_XMEN_afterrec_init);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_init_SFX_XMEN_SFX_XMEN_afterrec_init);
            }
            v13 = (tGameObjectMain*)v27->des_start_actor;
            if (v27->des_start_actor)
            {
                if ((unsigned int)v13 <= 2)
                    v13 = AI_MainActor[(uint32_t)v13 - 1];
                v2 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v13, 2);
            }
            else
            {
                v2 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 2);
            }
            if (v2)
            {
                v27->o_afterblend_gao = v27->des_start_actor;
                v12 = (tGameObjectMain*)v27->o_afterblend_gao;
                if (v12)
                {
                    if ((unsigned int)v12 <= 2)
                        v12 = AI_MainActor[(uint32_t)v12 - 1];
                    AI_EvalFunc_AICBAdd_C(
                        AI_gpst_CurrentGameObject,
                        v12,
                        3,
                        (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_init_SFX_XMEN_SFX_XMEN_afterblend_init);
                }
                else
                {
                    AI_EvalFunc_AICBAdd_C(
                        AI_gpst_CurrentGameObject,
                        0,
                        3,
                        (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_init_SFX_XMEN_SFX_XMEN_afterblend_init);
                }
            }
        }
        else
        {
            v27->o_afterrec_gao = (int)AI_gpst_CurrentGameObject;
            v11 = (tGameObjectMain*)v27->o_afterrec_gao;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v11 = AI_MainActor[(uint32_t)v11 - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v11,
                    2,
                    (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_init_SFX_XMEN_SFX_XMEN_afterrec_init);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_init_SFX_XMEN_SFX_XMEN_afterrec_init);
            }
        }
        v29 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_XMEN_SFX_XMEN_init_SFX_XMEN_SFX_XMEN_reflex);
        if (v29 != 1000)
            return v29;
    }
    else
    {
        if (v27->des_digitalisation_delay != 0.0)
        {
            if (v27->f_time_start == 0.0)
                v27->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
            v26 = TIM_gf_MainClock - TIM_gf_LockTime;
            v10 = (v26 - v27->f_time_start) / v27->des_digitalisation_delay;
            if (v10 > 1.0)
                v4 = 1.0;
            else
                v4 = (v26 - v27->f_time_start) / v27->des_digitalisation_delay;
            v28 = v4;
            v27->f_Z_coef = v4;
        }
        if (v27->f_Z_coef == 1.0)
        {
            v22 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v22 != 1000)
                return v22;
        }
        else if (AI_EvalFunc_TIMEElapsed_C(v27->f_time_gen, v27->f_delay_gen))
        {
            v27->f_time_gen = TIM_gf_MainClock - TIM_gf_LockTime;
            v27->f_delay_gen = AI_EvalFunc_MATHRandFloat_C(v27->des_delay_min, v27->des_delay_max);
            if (v27->des_rand_BV_start_pos)
            {
                AI_EvalFunc_BVRandomPosGet_C(AI_gpst_CurrentGameObject, 0, &a3);
                v21 = AI_EvalFunc_OBJDuplicate_C(AI_gpst_CurrentGameObject, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
                v21 = AI_EvalFunc_OBJDuplicate_C(AI_gpst_CurrentGameObject, &pos);
            }
            v9 = v21;
            if (v21)
            {
                if ((unsigned int)v21 <= 2)
                    v9 = AI_MainActor[(uint32_t)v21 - 1];
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(v9, 1);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
            }
            if (v21 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
            {
                v6 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v8 = v21;
                if (v21)
                {
                    if ((unsigned int)v21 <= 2)
                        v8 = AI_MainActor[(uint32_t)v21 - 1];
                    v7 = v8;
                }
                else
                {
                    v7 = 0;
                }
                v6 = v7->extended->AIInstance;
            }
            ((tScriptStruct_SFXXMEN*)(v6->instanceVarDataCurrent))->f_Z_coef = v27->f_Z_coef;
        }
    }
    return 1000;
}

int SFX_XMEN_SFX_XMEN_afterrec_init()
{
    unsigned int v0; // eax
    int result; // eax
    tScriptStruct_SFXXMEN* v2; // [esp+4h] [ebp-E4h]
    tGameObjectMain* v3; // [esp+Ch] [ebp-DCh]
    tGameObjectMain* v4; // [esp+1Ch] [ebp-CCh]
    float v5; // [esp+20h] [ebp-C8h]
    tGameObjectMain* v6; // [esp+28h] [ebp-C0h]
    tGameObjectMain* v7; // [esp+30h] [ebp-B8h]
    tGameObjectMain* v8; // [esp+38h] [ebp-B0h]
    tGameObjectMain* v9; // [esp+48h] [ebp-A0h]
    tGameObjectMain* v10; // [esp+50h] [ebp-98h]
    tGameObjectMain* v11; // [esp+58h] [ebp-90h]
    tGameObjectMain* v12; // [esp+5Ch] [ebp-8Ch]
    tGameObjectMain* v13; // [esp+60h] [ebp-88h]
    tGameObjectMain* v14; // [esp+68h] [ebp-80h]
    tGameObjectMain* v15; // [esp+6Ch] [ebp-7Ch]
    tGameObjectMain* v16; // [esp+70h] [ebp-78h]
    float v17; // [esp+74h] [ebp-74h]
    Vec3 v18; // [esp+78h] [ebp-70h] BYREF
    Vec3 v19; // [esp+84h] [ebp-64h] BYREF
    float v20; // [esp+90h] [ebp-58h]
    Vec3 v21; // [esp+94h] [ebp-54h] BYREF
    unsigned int v22; // [esp+A0h] [ebp-48h]
    int v23; // [esp+A4h] [ebp-44h]
    Vec3 a3; // [esp+A8h] [ebp-40h] BYREF
    Vec3 v25; // [esp+B4h] [ebp-34h] BYREF
    Vec3 v26; // [esp+C0h] [ebp-28h] BYREF
    Vec3 v27; // [esp+CCh] [ebp-1Ch] BYREF
    int v28; // [esp+D8h] [ebp-10h]
    tScriptStruct_SFXXMEN* v29; // [esp+DCh] [ebp-Ch]

    v27.x = 1.0;
    v27.y = 0.0;
    v27.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SFXXMEN*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v29 = v2;
    v20 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_XMEN_SFX_XMEN_afterrec_init_SFX_XMEN_SFX_XMEN_afterblend_init = (int)AI_ul_RealResolveOneRefFunction(0x72005559u);
        fct_SFX_XMEN_SFX_XMEN_afterrec_init_SFX_XMEN_SFX_XMEN_afterrec_init = (int)AI_ul_RealResolveOneRefFunction(0x72005558u);
        fct_SFX_XMEN_SFX_XMEN_afterrec_init_SFX_XMEN_SFX_XMEN_set_moving_function = (int)AI_ul_RealResolveOneRefFunction(0x7200555Au);
        fct_SFX_XMEN_SFX_XMEN_afterrec_init_SFX_XMEN_SFX_XMEN_exec_GFX_Halo = (int)AI_ul_RealResolveOneRefFunction(0x7200556Eu);
    }
    v16 = (tGameObjectMain*)v29->o_afterblend_gao;
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
        v14 = (tGameObjectMain*)v29->o_afterblend_gao;
        if (v14)
        {
            if ((unsigned int)v14 <= 2)
                v14 = AI_MainActor[(uint32_t)v14 - 1];
            v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(v14);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
        }
        v22 = v0;
        if ((v0 & 0x220000) == 0)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 4;
        }
    }
    if (v29->f_time_start == 0.0)
    {
        if (v29->des_rand_BV_start_pos)
        {
            v9 = (tGameObjectMain*)v29->o_bone;
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                AI_EvalFunc_BVRandomPosGet_C(v9, 0, &v25);
            }
            else
            {
                AI_EvalFunc_BVRandomPosGet_C(0, 0, &v25);
            }
            v27 = v25;
            if (v29->des_digitalisation_delay != 0.0)
            {
                v8 = (tGameObjectMain*)v29->o_bone;
                if (v8)
                {
                    if ((unsigned int)v8 <= 2)
                        v8 = AI_MainActor[(uint32_t)v8 - 1];
                    AI_EvalFunc_BVMinGet_C(v8, &v19);
                }
                else
                {
                    AI_EvalFunc_BVMinGet_C(0, &v19);
                }
                v17 = v19.z;
                v7 = (tGameObjectMain*)v29->o_bone;
                if (v7)
                {
                    if ((unsigned int)v7 <= 2)
                        v7 = AI_MainActor[(uint32_t)v7 - 1];
                    AI_EvalFunc_BVMaxGet_C(v7, &v18);
                }
                else
                {
                    AI_EvalFunc_BVMaxGet_C(0, &v18);
                }
                v20 = v18.z;
                v6 = (tGameObjectMain*)v29->o_bone;
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
                v27.z = a3.z;
                v5 = v29->f_Z_coef;
                if (v5 < 0.0)
                    v5 = 0.0;
                if (v5 > 1.0)
                    v5 = 1.0;
                v27.z = v17 * v5 + (1.0f - v5) * v20 + v27.z;
            }
        }
        else
        {
            v4 = (tGameObjectMain*)v29->o_bone;
            if (v4)
            {
                if ((unsigned int)v4 <= 2)
                    v4 = AI_MainActor[(uint32_t)v4 - 1];
                AI_EvalFunc_OBJGetPos_C(v4, &v21);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v21);
            }
            v27 = v21;
            v3 = (tGameObjectMain*)v29->o_bone;
            if (v3)
            {
                if ((unsigned int)v3 <= 2)
                    v3 = AI_MainActor[(uint32_t)v3 - 1];
                AI_EvalFunc_MATHVecLocalToGlobal_C(v3, &v29->des_start_bone_offset, &v26);
            }
            else
            {
                AI_EvalFunc_MATHVecLocalToGlobal_C(0, &v29->des_start_bone_offset, &v26);
            }
            v27.x = v27.x + v26.x;
            v27.y = v27.y + v26.y;
            v27.z = v27.z + v26.z;
        }
        AI_EvalFunc_OBJSetPos_C_CURRENT(&v27);
        v29->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
        XMEN_SetSize(AI_gpst_CurrentGameObject, 0, -1, 0.0);
        XMEN_OnOff(AI_gpst_CurrentGameObject, 0, 1);
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
    }
    else
    {
        XMEN_SetSize(AI_gpst_CurrentGameObject, 0, -1, v29->des_XMEN_width);
        v13 = (tGameObjectMain*)v29->o_afterblend_gao;
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
        if (v12)
        {
            v11 = (tGameObjectMain*)v29->o_afterblend_gao;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v11 = AI_MainActor[(uint32_t)v11 - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v11,
                    3,
                    (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_afterrec_init_SFX_XMEN_SFX_XMEN_afterblend_init);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    3,
                    (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_afterrec_init_SFX_XMEN_SFX_XMEN_afterblend_init);
            }
            v29->o_afterblend_gao = 0;
        }
        v10 = (tGameObjectMain*)v29->o_afterrec_gao;
        if (v10)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                v10,
                2,
                (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_afterrec_init_SFX_XMEN_SFX_XMEN_afterrec_init);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_afterrec_init_SFX_XMEN_SFX_XMEN_afterrec_init);
        }
        v29->o_afterrec_gao = 0;
        v28 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_SFX_XMEN_SFX_XMEN_afterrec_init_SFX_XMEN_SFX_XMEN_set_moving_function);
        if (v28 != 1000)
            return v28;
    }
    v23 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_SFX_XMEN_SFX_XMEN_afterrec_init_SFX_XMEN_SFX_XMEN_exec_GFX_Halo);
    if (v23 == 1000)
        result = 1000;
    else
        result = v23;
    return result;
}

int SFX_XMEN_SFX_XMEN_afterblend_init()
{
    int result; // eax
    tScriptStruct_SFXXMEN* v1; // [esp+4h] [ebp-C8h]
    tGameObjectMain* v2; // [esp+Ch] [ebp-C0h]
    tGameObjectMain* v3; // [esp+1Ch] [ebp-B0h]
    float v4; // [esp+20h] [ebp-ACh]
    tGameObjectMain* v5; // [esp+28h] [ebp-A4h]
    tGameObjectMain* v6; // [esp+30h] [ebp-9Ch]
    tGameObjectMain* v7; // [esp+38h] [ebp-94h]
    tGameObjectMain* v8; // [esp+48h] [ebp-84h]
    tGameObjectMain* v9; // [esp+50h] [ebp-7Ch]
    tGameObjectMain* v10; // [esp+58h] [ebp-74h]
    float v11; // [esp+5Ch] [ebp-70h]
    Vec3 a3; // [esp+60h] [ebp-6Ch] BYREF
    Vec3 v13; // [esp+6Ch] [ebp-60h] BYREF
    float v14; // [esp+78h] [ebp-54h]
    Vec3 v15; // [esp+7Ch] [ebp-50h] BYREF
    int v16; // [esp+88h] [ebp-44h]
    Vec3 v17; // [esp+90h] [ebp-3Ch] BYREF
    Vec3 v18; // [esp+A0h] [ebp-2Ch] BYREF
    Vec3 v19; // [esp+ACh] [ebp-20h] BYREF
    tScriptStruct_SFXXMEN* v20; // [esp+B8h] [ebp-14h]
    Vec3 v21; // [esp+BCh] [ebp-10h] BYREF
    int v22; // [esp+C8h] [ebp-4h]

    v18.x = 1.0;
    v18.y = 0.0;
    v18.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXXMEN*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v20 = v1;
    v14 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_XMEN_SFX_XMEN_afterblend_init_SFX_XMEN_SFX_XMEN_afterblend_init = (int)AI_ul_RealResolveOneRefFunction(0x72005559u);
        fct_SFX_XMEN_SFX_XMEN_afterblend_init_SFX_XMEN_SFX_XMEN_afterrec_init = (int)AI_ul_RealResolveOneRefFunction(0x72005558u);
        fct_SFX_XMEN_SFX_XMEN_afterblend_init_SFX_XMEN_SFX_XMEN_set_moving_function = (int)AI_ul_RealResolveOneRefFunction(0x7200555Au);
        fct_SFX_XMEN_SFX_XMEN_afterblend_init_SFX_XMEN_SFX_XMEN_exec_GFX_Halo = (int)AI_ul_RealResolveOneRefFunction(0x7200556Eu);
    }
    if (v20->f_time_start == 0.0)
    {
        if (v20->des_rand_BV_start_pos)
        {
            v8 = (tGameObjectMain*)v20->o_bone;
            if (v8)
            {
                if ((unsigned int)v8 <= 2)
                    v8 = AI_MainActor[(uint32_t)v8 - 1];
                AI_EvalFunc_BVRandomPosGet_C(v8, 0, &v19);
            }
            else
            {
                AI_EvalFunc_BVRandomPosGet_C(0, 0, &v19);
            }
            v18 = v19;
            if (v20->des_digitalisation_delay != 0.0)
            {
                v7 = (tGameObjectMain*)v20->o_bone;
                if (v7)
                {
                    if ((unsigned int)v7 <= 2)
                        v7 = AI_MainActor[(uint32_t)v7 - 1];
                    AI_EvalFunc_BVMinGet_C(v7, &v17);
                }
                else
                {
                    AI_EvalFunc_BVMinGet_C(0, &v17);
                }
                v11 = v17.z;
                v6 = (tGameObjectMain*)v20->o_bone;
                if (v6)
                {
                    if ((unsigned int)v6 <= 2)
                        v6 = AI_MainActor[(uint32_t)v6 - 1];
                    AI_EvalFunc_BVMaxGet_C(v6, &v13);
                }
                else
                {
                    AI_EvalFunc_BVMaxGet_C(0, &v13);
                }
                v14 = v13.z;
                v5 = (tGameObjectMain*)v20->o_bone;
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
                v18.z = a3.z;
                v4 = v20->f_Z_coef;
                if (v4 < 0.0)
                    v4 = 0.0;
                if (v4 > 1.0)
                    v4 = 1.0;
                v18.z = v11 * v4 + (1.0f - v4) * v14 + v18.z;
            }
        }
        else
        {
            v3 = (tGameObjectMain*)v20->o_bone;
            if (v3)
            {
                if ((unsigned int)v3 <= 2)
                    v3 = AI_MainActor[(uint32_t)v3 - 1];
                AI_EvalFunc_OBJGetPos_C(v3, &v21);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v21);
            }
            v18 = v21;
            v2 = (tGameObjectMain*)v20->o_bone;
            if (v2)
            {
                if ((unsigned int)v2 <= 2)
                    v2 = AI_MainActor[(uint32_t)v2 - 1];
                AI_EvalFunc_MATHVecLocalToGlobal_C(v2, &v20->des_start_bone_offset, &v15);
            }
            else
            {
                AI_EvalFunc_MATHVecLocalToGlobal_C(0, &v20->des_start_bone_offset, &v15);
            }
            v18.x = v18.x + v15.x;
            v18.y = v18.y + v15.y;
            v18.z = v18.z + v15.z;
        }
        AI_EvalFunc_OBJSetPos_C_CURRENT(&v18);
        v20->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
        XMEN_SetSize(AI_gpst_CurrentGameObject, 0, -1, 0.0);
        XMEN_OnOff(AI_gpst_CurrentGameObject, 0, 1);
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
    }
    else
    {
        XMEN_SetSize(AI_gpst_CurrentGameObject, 0, -1, v20->des_XMEN_width);
        v10 = (tGameObjectMain*)v20->o_afterblend_gao;
        if (v10)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                v10,
                3,
                (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_afterblend_init_SFX_XMEN_SFX_XMEN_afterblend_init);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                0,
                3,
                (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_afterblend_init_SFX_XMEN_SFX_XMEN_afterblend_init);
        }
        v20->o_afterblend_gao = 0;
        v9 = (tGameObjectMain*)v20->o_afterrec_gao;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                v9,
                2,
                (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_afterblend_init_SFX_XMEN_SFX_XMEN_afterrec_init);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_afterblend_init_SFX_XMEN_SFX_XMEN_afterrec_init);
        }
        v20->o_afterrec_gao = 0;
        v16 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_SFX_XMEN_SFX_XMEN_afterblend_init_SFX_XMEN_SFX_XMEN_set_moving_function);
        if (v16 != 1000)
            return v16;
    }
    v22 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_SFX_XMEN_SFX_XMEN_afterblend_init_SFX_XMEN_SFX_XMEN_exec_GFX_Halo);
    if (v22 == 1000)
        result = 1000;
    else
        result = v22;
    return result;
}

int SFX_XMEN_SFX_XMEN_set_moving_function()
{
    tGameObjectMain* v0; // eax
    BOOL v1; // eax
    BOOL v2; // eax
    tScriptStruct_SFXXMEN* v4; // [esp+0h] [ebp-58h]
    tGameObjectMain* v5; // [esp+8h] [ebp-50h]
    tGameObjectMain* v6; // [esp+10h] [ebp-48h]
    tGameObjectMain* v7; // [esp+18h] [ebp-40h]
    tGameObjectMain* v8; // [esp+20h] [ebp-38h]
    tGameObjectMain* v9; // [esp+28h] [ebp-30h]
    tGameObjectMain* v10; // [esp+34h] [ebp-24h]
    tGameObjectMain* v11; // [esp+38h] [ebp-20h]
    tGameObjectMain* v12; // [esp+40h] [ebp-18h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_SFXXMEN*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_XMEN_SFX_XMEN_set_moving_function_SFX_XMEN_SFX_XMEN_afterrec_move = (int)AI_ul_RealResolveOneRefFunction(0x7200555Bu);
        fct_SFX_XMEN_SFX_XMEN_set_moving_function_SFX_XMEN_SFX_XMEN_afterblend_move = (int)AI_ul_RealResolveOneRefFunction(0x7200555Cu);
    }
    if (v4->des_dest_bone_num == -1)
    {
        v4->o_bone = v4->des_dest_actor;
    }
    else
    {
        v12 = (tGameObjectMain*)v4->des_dest_actor;
        if (v12)
        {
            if ((unsigned int)v12 <= 2)
                v12 = AI_MainActor[(uint32_t)v12 - 1];
            v0 = AI_EvalFunc_ANICanalGaoGet_C(v12, v4->des_dest_bone_num);
        }
        else
        {
            v0 = AI_EvalFunc_ANICanalGaoGet_C(0, v4->des_dest_bone_num);
        }
        v4->o_bone = (int)v0;
        v11 = (tGameObjectMain*)v4->o_bone;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v11 = AI_MainActor[(uint32_t)v11 - 1];
            v10 = v11;
        }
        else
        {
            v10 = 0;
        }
        if (!v10)
            v4->o_bone = v4->des_dest_actor;
    }
    v9 = (tGameObjectMain*)v4->des_dest_actor;
    if (v9)
    {
        if ((unsigned int)v9 <= 2)
            v9 = AI_MainActor[(uint32_t)v9 - 1];
        v1 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v9, 32);
    }
    else
    {
        v1 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 32);
    }
    if (v1)
    {
        v4->o_afterrec_gao = v4->des_dest_actor;
        v8 = (tGameObjectMain*)v4->o_afterrec_gao;
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v8 = AI_MainActor[(uint32_t)v8 - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v8,
                2,
                (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_set_moving_function_SFX_XMEN_SFX_XMEN_afterrec_move);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_set_moving_function_SFX_XMEN_SFX_XMEN_afterrec_move);
        }
        v7 = (tGameObjectMain*)v4->des_dest_actor;
        if (v7)
        {
            if ((unsigned int)v7 <= 2)
                v7 = AI_MainActor[(uint32_t)v7 - 1];
            v2 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v7, 2);
        }
        else
        {
            v2 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 2);
        }
        if (v2)
        {
            v4->o_afterblend_gao = v4->des_dest_actor;
            v6 = (tGameObjectMain*)v4->o_afterblend_gao;
            if (v6)
            {
                if ((unsigned int)v6 <= 2)
                    v6 = AI_MainActor[(uint32_t)v6 - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v6,
                    3,
                    (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_set_moving_function_SFX_XMEN_SFX_XMEN_afterblend_move);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    3,
                    (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_set_moving_function_SFX_XMEN_SFX_XMEN_afterblend_move);
            }
        }
    }
    else
    {
        v4->o_afterrec_gao = (int)AI_gpst_CurrentGameObject;
        v5 = (tGameObjectMain*)v4->o_afterrec_gao;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v5,
                2,
                (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_set_moving_function_SFX_XMEN_SFX_XMEN_afterrec_move);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_set_moving_function_SFX_XMEN_SFX_XMEN_afterrec_move);
        }
    }
    return 1000;
}

int SFX_XMEN_SFX_XMEN_afterrec_move()
{
    unsigned int v0; // eax
    int result; // eax
    tScriptStruct_SFXXMEN* v2; // [esp+0h] [ebp-20h]
    tGameObjectMain* v3; // [esp+8h] [ebp-18h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-14h]
    tGameObjectMain* v5; // [esp+10h] [ebp-10h]
    int v6; // [esp+18h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SFXXMEN*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_XMEN_SFX_XMEN_afterrec_move_SFX_XMEN_SFX_XMEN_afterblend_move = (int)AI_ul_RealResolveOneRefFunction(0x7200555Cu);
    }
    v2->i_flag_move_done = 0;
    v5 = (tGameObjectMain*)v2->o_afterblend_gao;
    if (v5)
    {
        if ((unsigned int)v5 <= 2)
            v5 = AI_MainActor[(uint32_t)v5 - 1];
        v4 = v5;
    }
    else
    {
        v4 = 0;
    }
    if (v4)
    {
        v3 = (tGameObjectMain*)v2->o_afterblend_gao;
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(v3);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
        }
        if ((v0 & 0x220000) == 0)
            goto LABEL_20;
    }
    v6 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_SFX_XMEN_SFX_XMEN_afterrec_move_SFX_XMEN_SFX_XMEN_afterblend_move);
    if (v6 == 1000)
        LABEL_20:
    result = 1000;
    else
        result = v6;
    return result;
}

int SFX_XMEN_SFX_XMEN_afterblend_move()
{
    int result; // eax
    float v1; // [esp+8h] [ebp-104h]
    tScriptStruct_SFXXMEN* v2; // [esp+Ch] [ebp-100h]
    tGameObjectMain* v3; // [esp+2Ch] [ebp-E0h]
    tGameObjectMain* v4; // [esp+30h] [ebp-DCh]
    tGameObjectMain* v5; // [esp+34h] [ebp-D8h]
    tGameObjectMain* v6; // [esp+3Ch] [ebp-D0h]
    tGameObjectMain* v7; // [esp+40h] [ebp-CCh]
    tGameObjectMain* v8; // [esp+44h] [ebp-C8h]
    tGameObjectMain* v9; // [esp+4Ch] [ebp-C0h]
    tGameObjectMain* v10; // [esp+5Ch] [ebp-B0h]
    tGameObjectMain* v11; // [esp+6Ch] [ebp-A0h]
    float v12; // [esp+78h] [ebp-94h]
    Vec3 v13; // [esp+7Ch] [ebp-90h] BYREF
    Vec3 v14; // [esp+88h] [ebp-84h] BYREF
    float a4; // [esp+98h] [ebp-74h]
    Vec3 v16; // [esp+9Ch] [ebp-70h] BYREF
    Vec3 v17; // [esp+A8h] [ebp-64h] BYREF
    Vec3 a2; // [esp+B4h] [ebp-58h] BYREF
    int v19; // [esp+C0h] [ebp-4Ch]
    Vec3 v20; // [esp+C4h] [ebp-48h] BYREF
    Vec3 a3; // [esp+D4h] [ebp-38h] BYREF
    Vec3 v22; // [esp+E0h] [ebp-2Ch] BYREF
    tScriptStruct_SFXXMEN* v23; // [esp+ECh] [ebp-20h]
    int v24; // [esp+F0h] [ebp-1Ch]
    Vec3 a5; // [esp+F4h] [ebp-18h] BYREF
    Vec3 dest; // [esp+100h] [ebp-Ch] BYREF

    v13.x = 1.0;
    v13.y = 0.0;
    v13.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    a2.x = 1.0;
    a2.y = 0.0;
    a2.z = 0.0;
    v16.x = 1.0;
    v16.y = 0.0;
    v16.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SFXXMEN*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v23 = v2;
    a4 = 0.0;
    v24 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_XMEN_SFX_XMEN_afterblend_move_SFX_XMEN_SFX_XMEN_afterrec_move = (int)AI_ul_RealResolveOneRefFunction(0x7200555Bu);
        fct_SFX_XMEN_SFX_XMEN_afterblend_move_SFX_XMEN_SFX_XMEN_afterblend_move = (int)AI_ul_RealResolveOneRefFunction(0x7200555Cu);
        fct_SFX_XMEN_SFX_XMEN_afterblend_move_SFX_XMEN_SFX_XMEN_Destroy = (int)AI_ul_RealResolveOneRefFunction(0x2600345Eu);
        fct_SFX_XMEN_SFX_XMEN_afterblend_move_SFX_XMEN_SFX_XMEN_exec_GFX_Halo = (int)AI_ul_RealResolveOneRefFunction(0x7200556Eu);
    }
    if (v23->i_flag_move_done)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        XMEN_SetSize(AI_gpst_CurrentGameObject, 0, -1, v23->des_XMEN_width);
        v23->i_flag_move_done = 1;
        v11 = (tGameObjectMain*)v23->o_bone;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v11 = AI_MainActor[(uint32_t)v11 - 1];
            AI_EvalFunc_MATHVecLocalToGlobal_C(v11, &v23->des_dest_bone_dir, &v14);
        }
        else
        {
            AI_EvalFunc_MATHVecLocalToGlobal_C(0, &v23->des_dest_bone_dir, &v14);
        }
        a3 = v14;
        v10 = (tGameObjectMain*)v23->o_bone;
        if (v10)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            AI_EvalFunc_OBJGetPos_C(v10, &v22);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v22);
        }
        v16 = v22;
        v9 = (tGameObjectMain*)v23->o_bone;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            AI_EvalFunc_MATHVecLocalToGlobal_C(v9, &v23->des_dest_bone_offset, &v20);
        }
        else
        {
            AI_EvalFunc_MATHVecLocalToGlobal_C(0, &v23->des_dest_bone_offset, &v20);
        }
        v16.x = v16.x + v20.x;
        v16.y = v16.y + v20.y;
        v16.z = v16.z + v20.z;
        v16.x = v16.x - a3.x;
        v16.y = v16.y - a3.y;
        v16.z = v16.z - a3.z;
        if (v23->f_time_death == 0.0)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v17);
            MATH_SubstractVec(&dest, &v17, &v16);
            a2 = dest;
            v13 = v16;
            v12 = TIM_gf_MainClock - TIM_gf_LockTime;
            a4 = (v12 - (v23->f_time_start + 0.1f)) / v23->des_f_move_delay;
            if (a4 > 1.0)
                v1 = 1.0;
            else
                v1 = a4;
            a4 = v1;
            if (v23->des_accelere)
                a4 = a4 * a4;
            AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
            v13.x = v13.x + a5.x;
            v13.y = v13.y + a5.y;
            v13.z = v13.z + a5.z;
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v13);
            if (a4 == 1.0)
                v23->f_time_death = TIM_gf_MainClock - TIM_gf_LockTime;
            v19 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_SFX_XMEN_SFX_XMEN_afterblend_move_SFX_XMEN_SFX_XMEN_exec_GFX_Halo);
            if (v19 == 1000)
                result = 1000;
            else
                result = v19;
        }
        else
        {
            v24 = 0;
            if (AI_EvalFunc_TIMEElapsed_C(v23->f_time_death, v23->des_f_death_delay))
            {
                v24 = 1;
                XMEN_OnOff(AI_gpst_CurrentGameObject, 0, 0);
            }
            if (v24 && AI_EvalFunc_TIMEElapsed_C(v23->f_time_start, 0.5))
            {
                if (v23->GFX_Halo != -1)
                {
                    AI_EvalFunc_GFXDel_C_CURRENT(v23->GFX_Halo);
                    v23->GFX_Halo = -1;
                }
                v8 = (tGameObjectMain*)v23->o_afterrec_gao;
                if (v8)
                {
                    if ((unsigned int)v8 <= 2)
                        v8 = AI_MainActor[(uint32_t)v8 - 1];
                    v7 = v8;
                }
                else
                {
                    v7 = 0;
                }
                if (v7)
                {
                    v6 = (tGameObjectMain*)v23->o_afterrec_gao;
                    if (v6)
                    {
                        if ((unsigned int)v6 <= 2)
                            v6 = AI_MainActor[(uint32_t)v6 - 1];
                        AI_EvalFunc_AICBDel_C(
                            AI_gpst_CurrentGameObject,
                            v6,
                            2,
                            (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_afterblend_move_SFX_XMEN_SFX_XMEN_afterrec_move);
                    }
                    else
                    {
                        AI_EvalFunc_AICBDel_C(
                            AI_gpst_CurrentGameObject,
                            0,
                            2,
                            (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_afterblend_move_SFX_XMEN_SFX_XMEN_afterrec_move);
                    }
                }
                v5 = (tGameObjectMain*)v23->o_afterblend_gao;
                if (v5)
                {
                    if ((unsigned int)v5 <= 2)
                        v5 = AI_MainActor[(uint32_t)v5 - 1];
                    v4 = v5;
                }
                else
                {
                    v4 = 0;
                }
                if (v4)
                {
                    v3 = (tGameObjectMain*)v23->o_afterblend_gao;
                    if (v3)
                    {
                        if ((unsigned int)v3 <= 2)
                            v3 = AI_MainActor[(uint32_t)v3 - 1];
                        AI_EvalFunc_AICBDel_C(
                            AI_gpst_CurrentGameObject,
                            v3,
                            3,
                            (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_afterblend_move_SFX_XMEN_SFX_XMEN_afterblend_move);
                    }
                    else
                    {
                        AI_EvalFunc_AICBDel_C(
                            AI_gpst_CurrentGameObject,
                            0,
                            3,
                            (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_afterblend_move_SFX_XMEN_SFX_XMEN_afterblend_move);
                    }
                }
                AI_EvalFunc_AITrackChange_C(0, (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_afterblend_move_SFX_XMEN_SFX_XMEN_Destroy);
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 4;
            }
            else
            {
                v13.x = v16.x + a3.x;
                v13.y = v16.y + a3.y;
                v13.z = v16.z + a3.z;
                AI_EvalFunc_OBJSetPos_C_CURRENT(&v13);
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 4;
            }
        }
    }
    return result;
}

int SFX_XMEN_SFX_XMEN_reflex()
{
    unsigned int v0; // eax
    tScriptStruct_SFXXMEN* v2; // [esp+0h] [ebp-6Ch]
    tGameObjectMain* v3; // [esp+8h] [ebp-64h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-60h]
    tGameObjectMain* v5; // [esp+10h] [ebp-5Ch]
    tGameObjectMain* v6; // [esp+18h] [ebp-54h]
    tGameObjectMain* v7; // [esp+20h] [ebp-4Ch]
    tGameObjectMain* v8; // [esp+28h] [ebp-44h]
    tGameObjectMain* v9; // [esp+2Ch] [ebp-40h]
    tGameObjectMain* v10; // [esp+30h] [ebp-3Ch]
    tGameObjectMain* v11; // [esp+38h] [ebp-34h]
    tGameObjectMain* v12; // [esp+40h] [ebp-2Ch]
    tGameObjectMain* v13; // [esp+44h] [ebp-28h]
    tGameObjectMain* v14; // [esp+48h] [ebp-24h]
    tGameObjectMain* v15; // [esp+4Ch] [ebp-20h]
    tGameObjectMain* v16; // [esp+50h] [ebp-1Ch]
    tGameObjectMain* v17; // [esp+58h] [ebp-14h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SFXXMEN*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_XMEN_SFX_XMEN_reflex_SFX_XMEN_SFX_XMEN_afterrec_init = (int)AI_ul_RealResolveOneRefFunction(0x72005558u);
        fct_SFX_XMEN_SFX_XMEN_reflex_SFX_XMEN_SFX_XMEN_afterrec_move = (int)AI_ul_RealResolveOneRefFunction(0x7200555Bu);
    }
    v17 = (tGameObjectMain*)v2->o_afterrec_gao;
    if (v17)
    {
        if ((unsigned int)v17 <= 2)
            v17 = AI_MainActor[(uint32_t)v17 - 1];
        v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(v17);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
    }
    if ((v0 & 0x400000) != 0)
    {
        v16 = (tGameObjectMain*)v2->o_afterrec_gao;
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
        v14 = (tGameObjectMain*)v2->des_start_actor;
        if (v2->des_start_actor)
        {
            if ((unsigned int)v14 <= 2)
                v14 = AI_MainActor[(uint32_t)v14 - 1];
            v13 = v14;
        }
        else
        {
            v13 = 0;
        }
        if (v15 == v13)
        {
            v12 = (tGameObjectMain*)v2->o_afterrec_gao;
            if (v12)
            {
                if ((unsigned int)v12 <= 2)
                    v12 = AI_MainActor[(uint32_t)v12 - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v12,
                    2,
                    (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_reflex_SFX_XMEN_SFX_XMEN_afterrec_init);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_reflex_SFX_XMEN_SFX_XMEN_afterrec_init);
            }
            v2->o_afterrec_gao = (int)AI_gpst_CurrentGameObject;
            v11 = (tGameObjectMain*)v2->o_afterrec_gao;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v11 = AI_MainActor[(uint32_t)v11 - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v11,
                    2,
                    (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_reflex_SFX_XMEN_SFX_XMEN_afterrec_init);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_reflex_SFX_XMEN_SFX_XMEN_afterrec_init);
            }
            v10 = (tGameObjectMain*)v2->o_afterblend_gao;
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
                v8 = (tGameObjectMain*)v2->o_afterblend_gao;
                if (v8)
                {
                    if ((unsigned int)v8 <= 2)
                        v8 = AI_MainActor[(uint32_t)v8 - 1];
                    AI_EvalFunc_AICBDel_C(
                        AI_gpst_CurrentGameObject,
                        v8,
                        3,
                        (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_reflex_SFX_XMEN_SFX_XMEN_afterrec_init);
                }
                else
                {
                    AI_EvalFunc_AICBDel_C(
                        AI_gpst_CurrentGameObject,
                        0,
                        3,
                        (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_reflex_SFX_XMEN_SFX_XMEN_afterrec_init);
                }
                v2->o_afterblend_gao = 0;
            }
        }
        else
        {
            v7 = (tGameObjectMain*)v2->o_afterrec_gao;
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v7,
                    2,
                    (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_reflex_SFX_XMEN_SFX_XMEN_afterrec_move);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_reflex_SFX_XMEN_SFX_XMEN_afterrec_move);
            }
            v2->o_afterrec_gao = (int)AI_gpst_CurrentGameObject;
            v6 = (tGameObjectMain*)v2->o_afterrec_gao;
            if (v6)
            {
                if ((unsigned int)v6 <= 2)
                    v6 = AI_MainActor[(uint32_t)v6 - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v6,
                    2,
                    (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_reflex_SFX_XMEN_SFX_XMEN_afterrec_move);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_reflex_SFX_XMEN_SFX_XMEN_afterrec_move);
            }
            v5 = (tGameObjectMain*)v2->o_afterblend_gao;
            if (v5)
            {
                if ((unsigned int)v5 <= 2)
                    v5 = AI_MainActor[(uint32_t)v5 - 1];
                v4 = v5;
            }
            else
            {
                v4 = 0;
            }
            if (v4)
            {
                v3 = (tGameObjectMain*)v2->o_afterblend_gao;
                if (v3)
                {
                    if ((unsigned int)v3 <= 2)
                        v3 = AI_MainActor[(uint32_t)v3 - 1];
                    AI_EvalFunc_AICBDel_C(
                        AI_gpst_CurrentGameObject,
                        v3,
                        3,
                        (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_reflex_SFX_XMEN_SFX_XMEN_afterrec_move);
                }
                else
                {
                    AI_EvalFunc_AICBDel_C(
                        AI_gpst_CurrentGameObject,
                        0,
                        3,
                        (tAIFunction*)fct_SFX_XMEN_SFX_XMEN_reflex_SFX_XMEN_SFX_XMEN_afterrec_move);
                }
                v2->o_afterblend_gao = 0;
            }
        }
    }
    return 1000;
}

int SFX_XMEN_SFX_XMEN_exec_GFX_Halo()
{
    int result; // eax
    float v1; // [esp+4h] [ebp-34h]
    tScriptStruct_SFXXMEN* v2; // [esp+8h] [ebp-30h]
    tGameObjectMain* v3; // [esp+10h] [ebp-28h]
    float v4; // [esp+14h] [ebp-24h]
    float v5; // [esp+14h] [ebp-24h]
    int v6; // [esp+18h] [ebp-20h]
    float v7; // [esp+1Ch] [ebp-1Ch]
    Vec3 v8; // [esp+24h] [ebp-14h] BYREF
    tScriptStruct_SFXXMEN* v9; // [esp+34h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SFXXMEN*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v9 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SFX_XMEN_SFX_XMEN_exec_GFX_Halo_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    if (v9->des_sfx_halo)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v9->f_time_start, 0.5))
        {
            if (v9->GFX_Halo != -1)
            {
                AI_EvalFunc_GFXDel_C_CURRENT(v9->GFX_Halo);
                v9->GFX_Halo = -1;
            }
        }
        else
        {
            if (v9->GFX_Halo == -1)
            {
                v9->GFX_Halo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v9->GFX_Halo, 4, 1);
                v3 = (tGameObjectMain*)SFX_XMEN_SFX_XMEN_exec_GFX_Halo_SFX_light_smoke_gao;
                if (SFX_XMEN_SFX_XMEN_exec_GFX_Halo_SFX_light_smoke_gao)
                {
                    if ((unsigned int)SFX_XMEN_SFX_XMEN_exec_GFX_Halo_SFX_light_smoke_gao <= 2)
                        v3 = AI_MainActor[SFX_XMEN_SFX_XMEN_exec_GFX_Halo_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v9->GFX_Halo, v3, 0);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v9->GFX_Halo, 0, 0);
                }
                GFXSetCurGlobal(v9->GFX_Halo);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.5f);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
                v1 = v9->des_XMEN_width + v9->des_XMEN_width;
                AI_EvalFunc_GFXSetf_C_CURGLOB(21000, v1);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v8);
                AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v8);
            }
            v7 = TIM_gf_MainClock - TIM_gf_LockTime;
            v4 = (v7 - v9->f_time_start) / 0.5f;
            v5 = v4 * v4;
            v6 = AI_EvalFunc_COLORBlend_C(0xFF80FF, 0, v5);
            AI_EvalFunc_GFXSeti_C_CURRENT(v9->GFX_Halo, 21101, v6);
        }
        result = 1000;
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int SFX_XMEN_SFX_XMEN_Destroy()
{
    int result; // eax
    int v1; // [esp+4h] [ebp-8h]

    v1 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
    if (v1 == 1000)
        result = 1000;
    else
        result = v1;
    return result;
}