#include "ai_compiledscripts_inc.h"

//SFX_Arrose_SFX_Arrose_init
int SFX_Arrose_SFX_Arrose_init_univVars;
int SFX_Arrose_SFX_Arrose_init_univVars_Flag_speciaux_sauve;
int fct_SFX_Arrose_SFX_Arrose_init_SFX_Arrose_SFX_Arrose_comportement;
int fct_SFX_Arrose_SFX_Arrose_init_SFX_Arrose_SFX_Arrose_reflex;

int SFX_Arrose_SFX_Arrose_comportement()
{
    tScriptStruct_SFXArrose* v1; // [esp+4h] [ebp-168h]
    float v2; // [esp+20h] [ebp-14Ch]
    tGameObjectMain* v3; // [esp+28h] [ebp-144h]
    tGameObjectMain* v4; // [esp+30h] [ebp-13Ch]
    tGameObjectMain* v5; // [esp+38h] [ebp-134h]
    tGameObjectMain* v6; // [esp+48h] [ebp-124h]
    tGameObjectMain* v7; // [esp+50h] [ebp-11Ch]
    Vec3 v8; // [esp+54h] [ebp-118h] BYREF
    float v9; // [esp+60h] [ebp-10Ch]
    float v10; // [esp+64h] [ebp-108h]
    float v11; // [esp+68h] [ebp-104h]
    Vec3 v12; // [esp+6Ch] [ebp-100h] BYREF
    Vec3 dest; // [esp+78h] [ebp-F4h] BYREF
    float v14; // [esp+84h] [ebp-E8h]
    float v15; // [esp+88h] [ebp-E4h]
    float v16; // [esp+8Ch] [ebp-E0h]
    Vec3 v17; // [esp+90h] [ebp-DCh] BYREF
    float v18; // [esp+9Ch] [ebp-D0h]
    Vec3 v19; // [esp+A4h] [ebp-C8h] BYREF
    Vec3 v20; // [esp+B0h] [ebp-BCh] BYREF
    Vec3 v21; // [esp+BCh] [ebp-B0h] BYREF
    float v22; // [esp+C8h] [ebp-A4h]
    float v23; // [esp+CCh] [ebp-A0h]
    float v24; // [esp+D0h] [ebp-9Ch]
    Vec3 v25; // [esp+D4h] [ebp-98h] BYREF
    Vec3 v26; // [esp+E0h] [ebp-8Ch] BYREF
    Vec3 v27; // [esp+114h] [ebp-58h] BYREF
    Vec3 v28; // [esp+120h] [ebp-4Ch] BYREF
    Vec3 v29; // [esp+12Ch] [ebp-40h] BYREF
    Vec3 a3; // [esp+138h] [ebp-34h] BYREF
    tScriptStruct_SFXArrose* v31; // [esp+144h] [ebp-28h]
    Vec3 v32; // [esp+148h] [ebp-24h]
    Vec3 v33; // [esp+154h] [ebp-18h] BYREF
    Vec3 v34; // [esp+160h] [ebp-Ch] BYREF

    v19.x = 1.0;
    v19.y = 0.0;
    v19.z = 0.0;
    v29.x = 1.0;
    v29.y = 0.0;
    v29.z = 0.0;
    v12.x = 1.0;
    v12.y = 0.0;
    v12.z = 0.0;
    v21.x = 1.0;
    v21.y = 0.0;
    v21.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXArrose*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v31 = v1;
    if (v1->GFX == -1)
    {
        v31->GFX = AI_EvalFunc_GFXAdd_C_CURRENT(13);
        GFXSetCurGlobal(v31->GFX);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13100, v31->des_particules_buffer);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13106, 0xA43B7400);
        v7 = (tGameObjectMain*)v31->des_mat_gao;
        if (v7)
        {
            if ((unsigned int)v7 <= 2)
                v7 = AI_MainActor[(uint32_t)v7 - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v31->GFX, v7, -1);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v31->GFX, 0, -1);
        }
        AI_EvalFunc_GFXSeti_C_CURRENT(v31->GFX, 13101, v31->des_mat_1);
        if (v31->des_mat_2 != -1)
            AI_EvalFunc_GFXSeti_C_CURRENT(v31->GFX, 13110, v31->des_mat_2);
        if (v31->des_mat_3 != -1)
            AI_EvalFunc_GFXSeti_C_CURRENT(v31->GFX, 13111, v31->des_mat_2);
        if (v31->des_mat_4 != -1)
            AI_EvalFunc_GFXSeti_C_CURRENT(v31->GFX, 13112, v31->des_mat_4);
        GFXSetCurGlobal(v31->GFX);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13003, v31->des_f_time_phase_1);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13004, v31->des_f_time_phase_2);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13103, v31->des_c_1st_color);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13104, v31->des_c_2nd_color);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13105, v31->des_c_3rd_color);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13107, 0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13000, v31->des_f_growing_speed_min);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13001, v31->des_f_growing_speed_max);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13002, v31->des_f_growing_speed_friction);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13007, v31->des_f_gravity);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v31->des_v_friction);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v31->GFX, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v31->GFX, 2, 1);
        GFXSetCurGlobal(v31->GFX);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13012, v31->des_f_time_random);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13008, v31->des_f_gen_rate_ON);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13009, v31->des_f_ON_speed_min);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13010, v31->des_f_ON_speed_max);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13005, v31->des_f_ON_size_min);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13006, v31->des_f_ON_size_max);
    }
    v6 = (tGameObjectMain*)v31->o_bone;
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
    v29 = a3;
    v5 = (tGameObjectMain*)v31->o_bone;
    if (v5)
    {
        if ((unsigned int)v5 <= 2)
            v5 = AI_MainActor[(uint32_t)v5 - 1];
        AI_EvalFunc_OBJ_SightGet_C(v5, &v27);
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C(0, &v27);
    }
    v9 = v27.x * 0.40000001f;
    v10 = v27.y * 0.40000001f;
    v11 = v27.z * 0.40000001f;
    v29.x = v29.x + v9;
    v29.y = v29.y + v10;
    v29.z = v29.z + v11;
    v4 = (tGameObjectMain*)v31->o_bone;
    if (v4)
    {
        if ((unsigned int)v4 <= 2)
            v4 = AI_MainActor[(uint32_t)v4 - 1];
        AI_EvalFunc_OBJ_BankingGet_C(v4, &v8);
    }
    else
    {
        AI_EvalFunc_OBJ_BankingGet_C(0, &v8);
    }
    v22 = v8.x * 0.2f;
    v23 = v8.y * 0.2f;
    v24 = v8.z * 0.2f;
    v29.x = v29.x + v22;
    v29.y = v29.y + v23;
    v29.z = v29.z + v24;
    v3 = (tGameObjectMain*)v31->o_bone;
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        AI_EvalFunc_OBJ_HorizonGet_C(v3, &v33);
    }
    else
    {
        AI_EvalFunc_OBJ_HorizonGet_C(0, &v33);
    }
    v14 = v33.x * 0.2f;
    v15 = v33.y * 0.2f;
    v16 = v33.z * 0.2f;
    v29.x = v29.x - v14;
    v29.y = v29.y - v15;
    v29.z = v29.z - v16;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v28);
    v2 = v31->des_f_wind;
    v32.x = v28.x * v2;
    v32.y = v28.y * v2;
    v32.z = v28.z * v2;
    v21 = v32;
    GFXSetCurGlobal(v31->GFX);
    AI_EvalFunc_GFXSetv_C_CURGLOB(13204, &v21);
    v18 = TIM_gf_dt;
    v26.x = v21.x * TIM_gf_dt;
    v26.y = v21.y * TIM_gf_dt;
    v26.z = v21.z * TIM_gf_dt;
    MATH_SubstractVec(&dest, &v29, &v26);
    AI_EvalFunc_GFXSetv_C_CURGLOB(13200, &dest);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v34);
    v12 = v34;
    AI_EvalFunc_BVMinGet_C(AI_gpst_CurrentGameObject, &v25);
    v12.x = v12.x + v25.x;
    v12.y = v12.y + v25.y;
    v12.z = v12.z + v25.z;
    v12.x = v12.x - v29.x;
    v12.y = v12.y - v29.y;
    v12.z = v12.z - v29.z;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v17);
    v19 = v17;
    AI_EvalFunc_BVMaxGet_C(AI_gpst_CurrentGameObject, &v20);
    v19.x = v19.x + v20.x;
    v19.y = v19.y + v20.y;
    v19.z = v19.z + v20.z;
    v19.x = v19.x - v29.x;
    v19.y = v19.y - v29.y;
    v19.z = v19.z - v29.z;
    AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v12);
    AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v19);
    return 1000;
}

int SFX_Arrose_SFX_Arrose_init()
{
    int result; // eax
    tGameObjectMain* v1; // eax
    tScriptStruct_SFXArrose* v2; // [esp+0h] [ebp-40h]
    tGameObjectMain* v3; // [esp+8h] [ebp-38h]
    tGameObjectMain* v4; // [esp+10h] [ebp-30h]
    tGameObjectMain* v5; // [esp+14h] [ebp-2Ch]
    tGameObjectMain* v6; // [esp+18h] [ebp-28h]
    int v7; // [esp+2Ch] [ebp-14h]
    int v8; // [esp+34h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SFXArrose*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SFX_Arrose_SFX_Arrose_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &SFX_Arrose_SFX_Arrose_init_univVars_Flag_speciaux_sauve,
            (void*)SFX_Arrose_SFX_Arrose_init_univVars,
            4488);
        fct_SFX_Arrose_SFX_Arrose_init_SFX_Arrose_SFX_Arrose_comportement = (int)AI_ul_RealResolveOneRefFunction(0x72005776u);
        fct_SFX_Arrose_SFX_Arrose_init_SFX_Arrose_SFX_Arrose_reflex = (int)AI_ul_RealResolveOneRefFunction(0x72005775u);
    }
    if (v2->des_SF_desactivation)
    {
        if (v2->des_SF_desactivation > 0
            && AI_EvalFunc_ARRBitTest_C(
                (int*)(SFX_Arrose_SFX_Arrose_init_univVars_Flag_speciaux_sauve + 4),
                v2->des_SF_desactivation))
        {
            v8 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v8 != 1000)
                return v8;
        }
    }
    if (AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 0x2000000))
        AI_EvalFunc_TRAPlayAll_C(AI_gpst_CurrentGameObject);
    v6 = (tGameObjectMain*)v2->des_snap_actor;
    if (v6)
    {
        if ((unsigned int)v6 <= 2)
            v6 = AI_MainActor[(uint32_t)v6 - 1];
        v5 = v6;
    }
    else
    {
        v5 = 0;
    }
    if (!v5)
        v2->des_snap_actor = (int)AI_gpst_CurrentGameObject;
    if (v2->des_bone_index == -1)
    {
        v2->o_bone = v2->des_snap_actor;
    }
    else
    {
        v4 = (tGameObjectMain*)v2->des_snap_actor;
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            v1 = AI_EvalFunc_ANICanalGaoGet_C(v4, v2->des_bone_index);
        }
        else
        {
            v1 = AI_EvalFunc_ANICanalGaoGet_C(0, v2->des_bone_index);
        }
        v2->o_bone = (int)v1;
    }
    v3 = (tGameObjectMain*)v2->des_snap_actor;
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v3,
            3,
            (tAIFunction*)fct_SFX_Arrose_SFX_Arrose_init_SFX_Arrose_SFX_Arrose_comportement);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            3,
            (tAIFunction*)fct_SFX_Arrose_SFX_Arrose_init_SFX_Arrose_SFX_Arrose_comportement);
    }
    v7 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_Arrose_SFX_Arrose_init_SFX_Arrose_SFX_Arrose_reflex);
    if (v7 == 1000)
        result = 1000;
    else
        result = v7;
    return result;
}

int SFX_Arrose_SFX_Arrose_reflex()
{
    unsigned int v0; // eax
    tScriptStruct_SFXArrose* v2; // [esp+0h] [ebp-24h]
    tGameObjectMain* v3; // [esp+4h] [ebp-20h]
    tGameObjectMain* v4; // [esp+8h] [ebp-1Ch]
    tGameObjectMain* v5; // [esp+10h] [ebp-14h]
    int v6; // [esp+20h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SFXArrose*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v6 = 0;
    v5 = (tGameObjectMain*)v2->des_snap_actor;
    if (v5)
    {
        if ((unsigned int)v5 <= 2)
            v5 = AI_MainActor[(uint32_t)v5 - 1];
        v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(v5);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
    }
    if ((v0 & 0x620000) != 0)
    {
        v6 = 1;
    }
    else
    {
        v4 = (tGameObjectMain*)v2->des_snap_actor;
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            v3 = v4;
        }
        else
        {
            v3 = 0;
        }
        if ((v3->flags2 & 4) != 0)
            v6 = 1;
    }
    if (v6 && v2->GFX != -1)
    {
        AI_EvalFunc_GFXSeti_C_CURRENT(v2->GFX, 13106, 0);
        v2->GFX = -1;
    }
    return 1000;
}