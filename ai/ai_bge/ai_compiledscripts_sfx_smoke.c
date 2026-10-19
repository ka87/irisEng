#include "ai_compiledscripts_inc.h"

//SFX_Smoke_SFX_Smoke_init
int SFX_Smoke_SFX_Smoke_init_univVars;
int SFX_Smoke_SFX_Smoke_init_univVars_Flag_speciaux_sauve;
int fct_SFX_Smoke_SFX_Smoke_init_SFX_Smoke_SFX_Smoke_comportement;

int SFX_Smoke_SFX_Smoke_init()
{
    int result; // eax
    tScriptStruct_SFXSmoke* v1; // [esp+0h] [ebp-48h]
    tGameObjectMain* v2; // [esp+Ch] [ebp-3Ch]
    tGameObjectMain* v3; // [esp+10h] [ebp-38h]
    tGameObjectMain* v4; // [esp+1Ch] [ebp-2Ch]
    tGameObjectMain* v5; // [esp+20h] [ebp-28h]
    int v6; // [esp+2Ch] [ebp-1Ch]
    int v7; // [esp+38h] [ebp-10h]
    int v8; // [esp+44h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXSmoke*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SFX_Smoke_SFX_Smoke_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &SFX_Smoke_SFX_Smoke_init_univVars_Flag_speciaux_sauve,
            (void*)SFX_Smoke_SFX_Smoke_init_univVars,
            4488);
        fct_SFX_Smoke_SFX_Smoke_init_SFX_Smoke_SFX_Smoke_comportement = (int)AI_ul_RealResolveOneRefFunction(0x72003FEBu);
    }
    if (v1->des_i_sf_jesuisla)
    {
        if (v1->des_i_sf_jesuisla <= 0)
            v8 = 0;
        else
            v8 = v1->des_i_sf_jesuisla < 10000 ? AI_EvalFunc_ARRBitTest_C(
                (int*)(SFX_Smoke_SFX_Smoke_init_univVars_Flag_speciaux_sauve + 4),
                v1->des_i_sf_jesuisla) : 1
            - AI_EvalFunc_ARRBitTest_C(
                (int*)(SFX_Smoke_SFX_Smoke_init_univVars_Flag_speciaux_sauve
                    + 4),
                v1->des_i_sf_jesuisla - 10000);
        if (!v8)
        {
            v7 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v7 != 1000)
                return v7;
        }
    }
    v1->o_me = (int)AI_gpst_CurrentGameObject;
    v5 = (tGameObjectMain*)v1->des_o_BV;
    if (v1->des_o_BV)
    {
        if ((unsigned int)v5 <= 2)
            v5 = AI_MainActor[(uint32_t)v5 - 1];
        v4 = v5;
    }
    else
    {
        v4 = 0;
    }
    if (!v4)
        v1->des_o_BV = v1->o_me;
    v3 = (tGameObjectMain*)v1->des_mat_gao;
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        v2 = v3;
    }
    else
    {
        v2 = 0;
    }
    if (!v2)
        v1->des_mat_gao = v1->o_me;
    v6 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_Smoke_SFX_Smoke_init_SFX_Smoke_SFX_Smoke_comportement);
    if (v6 == 1000)
        result = 1000;
    else
        result = v6;
    return result;
}

int SFX_Smoke_SFX_Smoke_comportement()
{
    tScriptStruct_SFXSmoke* v1; // [esp+4h] [ebp-1E4h]
    tGameObjectMain* v2; // [esp+Ch] [ebp-1DCh]
    tGameObjectMain* v3; // [esp+1Ch] [ebp-1CCh]
    tGameObjectMain* v4; // [esp+24h] [ebp-1C4h]
    tGameObjectMain* v5; // [esp+34h] [ebp-1B4h]
    tGameObjectMain* v6; // [esp+38h] [ebp-1B0h]
    tGameObjectMain* v7; // [esp+3Ch] [ebp-1ACh]
    float v8; // [esp+40h] [ebp-1A8h]
    tGameObjectMain* v9; // [esp+48h] [ebp-1A0h]
    tGameObjectMain* v10; // [esp+58h] [ebp-190h]
    Vec3 v11; // [esp+6Ch] [ebp-17Ch] BYREF
    Vec3 dest; // [esp+7Ch] [ebp-16Ch] BYREF
    Vec3 v13; // [esp+88h] [ebp-160h] BYREF
    Vec3 v14; // [esp+94h] [ebp-154h] BYREF
    Vec3 v15; // [esp+C8h] [ebp-120h] BYREF
    float v16; // [esp+D4h] [ebp-114h]
    Vec3 v17; // [esp+D8h] [ebp-110h] BYREF
    float v18; // [esp+E4h] [ebp-104h]
    Vec3 v19; // [esp+ECh] [ebp-FCh] BYREF
    Vec3 v20; // [esp+F8h] [ebp-F0h] BYREF
    Vec3 v21; // [esp+104h] [ebp-E4h] BYREF
    Vec3 v22; // [esp+110h] [ebp-D8h] BYREF
    Vec3 v24; // [esp+120h] [ebp-C8h] BYREF
    Vec3 v25; // [esp+12Ch] [ebp-BCh] BYREF
    Vec3 v26; // [esp+138h] [ebp-B0h] BYREF
    Vec3 v27; // [esp+144h] [ebp-A4h] BYREF
    float v28; // [esp+150h] [ebp-98h]
    float v29; // [esp+154h] [ebp-94h]
    float v30; // [esp+158h] [ebp-90h]
    float v31; // [esp+15Ch] [ebp-8Ch]
    Vec3 v32; // [esp+160h] [ebp-88h] BYREF
    Vec3 v33; // [esp+16Ch] [ebp-7Ch] BYREF
    Vec3 v34; // [esp+178h] [ebp-70h] BYREF
    Vec3 v35; // [esp+184h] [ebp-64h] BYREF
    Vec3 v36; // [esp+190h] [ebp-58h] BYREF
    Vec3 v37; // [esp+19Ch] [ebp-4Ch] BYREF
    tScriptStruct_SFXSmoke* v38; // [esp+1A8h] [ebp-40h]
    Vec3 v39; // [esp+1ACh] [ebp-3Ch] BYREF
    Vec3 v40; // [esp+1B8h] [ebp-30h] BYREF
    Vec3 v41; // [esp+1C4h] [ebp-24h] BYREF
    Vec3 a3; // [esp+1D0h] [ebp-18h] BYREF
    Vec3 v43; // [esp+1DCh] [ebp-Ch] BYREF

    v28 = 1.0;
    v29 = 0.0;
    v30 = 0.0;
    v20.x = 1.0;
    v20.y = 0.0;
    v20.z = 0.0;
    v14.x = 1.0;
    v14.y = 0.0;
    v14.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXSmoke*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v38 = v1;
    v1->table_color[0] = 0x800037B2;
    v38->table_color[1] = 0x80003151;
    v38->table_color[2] = 0x2006F6F0;
    v38->table_color[3] = 0x80012865;
    if (v38->GFX == -1)
    {
        v38->GFX = AI_EvalFunc_GFXAdd_C_CURRENT(13);
        GFXSetCurGlobal(v38->GFX);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13100, v38->des_particules_buffer);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13106, 0xA43B7400);
        v10 = (tGameObjectMain*)v38->des_mat_gao;
        if (v10)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v38->GFX, v10, -1);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v38->GFX, 0, -1);
        }
        AI_EvalFunc_GFXSeti_C_CURRENT(v38->GFX, 13101, v38->des_mat_1);
        if (v38->des_mat_2 != -1)
            AI_EvalFunc_GFXSeti_C_CURRENT(v38->GFX, 13110, v38->des_mat_2);
        if (v38->des_mat_3 != -1)
            AI_EvalFunc_GFXSeti_C_CURRENT(v38->GFX, 13111, v38->des_mat_2);
        if (v38->des_mat_4 != -1)
            AI_EvalFunc_GFXSeti_C_CURRENT(v38->GFX, 13112, v38->des_mat_4);
        GFXSetCurGlobal(v38->GFX);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13003, v38->des_f_time_phase_1);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13004, v38->des_f_time_phase_2);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13103, v38->des_c_1st_color);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13104, v38->des_c_2nd_color);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13105, v38->des_c_3rd_color);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13107, 0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13000, v38->des_f_growing_speed_min);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13001, v38->des_f_growing_speed_max);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13002, v38->des_f_growing_speed_friction);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13007, v38->des_f_gravity);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v38->des_v_friction);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v38->GFX, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v38->GFX, 2, 1);
        GFXSetCurGlobal(v38->GFX);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13012, v38->des_f_time_random);
        AI_EvalFunc_BVMaxGet_C(AI_gpst_CurrentGameObject, &v37);
        AI_EvalFunc_BVMinGet_C(AI_gpst_CurrentGameObject, &v32);
        MATH_SubstractVec(&dest, &v37, &v32);
        v28 = dest.x * 0.5f;
        v29 = dest.y * 0.5f;
        v30 = dest.z * 0.5f;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v11);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13200, &v11);
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v25);
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v41);
        v16 = v41.z * v30 + v41.y * v29 + v28 * v41.x;
        v33.x = v25.x * v16;
        v33.y = v25.y * v16;
        v33.z = v25.z * v16;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13207, &v33);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v39);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v17);
        v31 = v17.z * v30 + v17.y * v29 + v28 * v17.x;
        v15.x = v39.x * v31;
        v15.y = v39.y * v31;
        v15.z = v39.z * v31;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13208, &v15);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v43);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v26);
        v18 = v26.z * v30 + v26.y * v29 + v28 * v26.x;
        v22.x = v43.x * v18;
        v22.y = v43.y * v18;
        v22.z = v43.z * v18;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13209, &v22);
        v38->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(256))
    {
        GFXSetCurGlobal(v38->GFX);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13008, v38->des_f_gen_rate_ON);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13009, v38->des_f_ON_speed_min);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13010, v38->des_f_ON_speed_max);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13005, v38->des_f_ON_size_min);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13006, v38->des_f_ON_size_max);
    }
    else
    {
        GFXSetCurGlobal(v38->GFX);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13008, v38->des_f_gen_rate_OFF);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13009, v38->des_f_OFF_speed_min);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13010, v38->des_f_OFF_speed_max);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13005, v38->des_f_OFF_size_min);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13006, v38->des_f_OFF_size_max);
    }
    v9 = (tGameObjectMain*)v38->des_o_BV;
    if (v38->des_o_BV)
    {
        if ((unsigned int)v9 <= 2)
            v9 = AI_MainActor[(uint32_t)v9 - 1];
        AI_EvalFunc_OBJ_SightGet_C(v9, &v35);
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C(0, &v35);
    }
    v8 = v38->des_f_wind;
    v27.x = v35.x * v8;
    v27.y = v35.y * v8;
    v27.z = v35.z * v8;
    AI_EvalFunc_GFXSetv_C_CURRENT(v38->GFX, 13204, &v27);
    v7 = (tGameObjectMain*)v38->des_o_BV;
    if (v38->des_o_BV)
    {
        if ((unsigned int)v7 <= 2)
            v7 = AI_MainActor[(uint32_t)v7 - 1];
        v6 = v7;
    }
    else
    {
        v6 = 0;
    }
    if (v6)
    {
        v5 = (tGameObjectMain*)v38->des_o_BV;
        if (v38->des_o_BV)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            AI_EvalFunc_OBJGetPos_C(v5, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        v14 = a3;
        v4 = (tGameObjectMain*)v38->des_o_BV;
        if (v38->des_o_BV)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            AI_EvalFunc_BVMinGet_C(v4, &v34);
        }
        else
        {
            AI_EvalFunc_BVMinGet_C(0, &v34);
        }
        v14.x = v14.x + v34.x;
        v14.y = v14.y + v34.y;
        v14.z = v14.z + v34.z;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v21);
        v14.x = v14.x - v21.x;
        v14.y = v14.y - v21.y;
        v14.z = v14.z - v21.z;
        v3 = (tGameObjectMain*)v38->des_o_BV;
        if (v38->des_o_BV)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            AI_EvalFunc_OBJGetPos_C(v3, &v19);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v19);
        }
        v20 = v19;
        v2 = (tGameObjectMain*)v38->des_o_BV;
        if (v38->des_o_BV)
        {
            if ((unsigned int)v2 <= 2)
                v2 = AI_MainActor[(uint32_t)v2 - 1];
            AI_EvalFunc_BVMaxGet_C(v2, &v36);
        }
        else
        {
            AI_EvalFunc_BVMaxGet_C(0, &v36);
        }
        v20.x = v20.x + v36.x;
        v20.y = v20.y + v36.y;
        v20.z = v20.z + v36.z;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v13);
        v20.x = v20.x - v13.x;
        v20.y = v20.y - v13.y;
        v20.z = v20.z - v13.z;
        GFXSetCurGlobal(v38->GFX);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v14);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v20);
    }
    else
    {
        AI_EvalFunc_BVMinGet_C(AI_gpst_CurrentGameObject, &v40);
        GFXSetCurGlobal(v38->GFX);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v40);
        AI_EvalFunc_BVMaxGet_C(AI_gpst_CurrentGameObject, &v24);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v24);
    }
    return 1000;
}