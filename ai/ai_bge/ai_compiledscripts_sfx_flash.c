#include "ai_compiledscripts_inc.h"

//SFX_Flash_SFX_Flash_init
int fct_SFX_Flash_SFX_Flash_init_SFX_Flash_SFX_Flash_comportement;

//SFX_Flash_SFX_Flash_comportement
int SFX_Flash_SFX_Flash_comportement_SFX_light_smoke_gao;

int SFX_Flash_SFX_Flash_init()
{
    tScriptStruct_SFXFlash* v1; // [esp+0h] [ebp-1Ch]
    tGameObjectMain* v2; // [esp+8h] [ebp-14h]
    tGameObjectMain* v3; // [esp+Ch] [ebp-10h]
    tGameObjectMain* v4; // [esp+10h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXFlash*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_Flash_SFX_Flash_init_SFX_Flash_SFX_Flash_comportement = (int)AI_ul_RealResolveOneRefFunction(0xBF002518);
    }
    v4 = (tGameObjectMain*)v1->des_o_snap_actor;
    if (v1->des_o_snap_actor)
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
        v1->des_o_snap_actor = (int)AI_gpst_CurrentGameObject;
    v2 = (tGameObjectMain*)v1->des_o_snap_actor;
    if (v1->des_o_snap_actor)
    {
        if ((unsigned int)v2 <= 2)
            v2 = AI_MainActor[(uint32_t)v2 - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v2,
            2,
            (tAIFunction*)fct_SFX_Flash_SFX_Flash_init_SFX_Flash_SFX_Flash_comportement);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            2,
            (tAIFunction*)fct_SFX_Flash_SFX_Flash_init_SFX_Flash_SFX_Flash_comportement);
    }
    AI_EvalFunc_AITrackCurStop_C();
    return 1000;
}

int SFX_Flash_SFX_Flash_comportement()
{
    double v0; // st7
    double v1; // st7
    float v3; // [esp+4h] [ebp-160h]
    float v4; // [esp+4h] [ebp-160h]
    tScriptStruct_SFXFlash* v5; // [esp+8h] [ebp-15Ch]
    float v6; // [esp+Ch] [ebp-158h]
    tGameObjectMain* v7; // [esp+20h] [ebp-144h]
    tGameObjectMain* v8; // [esp+28h] [ebp-13Ch]
    tGameObjectMain* v9; // [esp+30h] [ebp-134h]
    tGameObjectMain* v10; // [esp+40h] [ebp-124h]
    float v11; // [esp+44h] [ebp-120h]
    tGameObjectMain* v12; // [esp+58h] [ebp-10Ch]
    tGameObjectMain* v13; // [esp+60h] [ebp-104h]
    tGameObjectMain* v14; // [esp+68h] [ebp-FCh]
    tGameObjectMain* v15; // [esp+78h] [ebp-ECh]
    tGameObjectMain* v16; // [esp+80h] [ebp-E4h]
    Vec3 v17; // [esp+84h] [ebp-E0h] BYREF
    tGameObjectMain* v18; // [esp+94h] [ebp-D0h]
    float v19; // [esp+9Ch] [ebp-C8h]
    float v21; // [esp+A4h] [ebp-C0h]
    Vec3 v22; // [esp+A8h] [ebp-BCh] BYREF
    Vec3 dest; // [esp+B4h] [ebp-B0h] BYREF
    Vec3 a1; // [esp+C8h] [ebp-9Ch] BYREF
    float v26; // [esp+D4h] [ebp-90h]
    Vec3 v28; // [esp+DCh] [ebp-88h] BYREF
    float v29; // [esp+E8h] [ebp-7Ch]
    Vec3 a3; // [esp+ECh] [ebp-78h] BYREF
    Vec3 v31; // [esp+F8h] [ebp-6Ch] BYREF
    int v32; // [esp+104h] [ebp-60h]
    float v33; // [esp+10Ch] [ebp-58h]
    float v34; // [esp+110h] [ebp-54h]
    float v35; // [esp+114h] [ebp-50h]
    float v36; // [esp+120h] [ebp-44h]
    float v37; // [esp+124h] [ebp-40h]
    float v38; // [esp+128h] [ebp-3Ch]
    int v39; // [esp+12Ch] [ebp-38h]
    tScriptStruct_SFXFlash* v40; // [esp+134h] [ebp-30h]
    float v41; // [esp+138h] [ebp-2Ch]
    Vec3 v42; // [esp+13Ch] [ebp-28h] BYREF
    Vec3 v43; // [esp+148h] [ebp-1Ch] BYREF
    Vec3 v44; // [esp+154h] [ebp-10h] BYREF
    float v45; // [esp+160h] [ebp-4h]

    v22.x = 1.0;
    v22.y = 0.0;
    v22.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_SFXFlash*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v40 = v5;
    v18 = 0;
    v19 = 0.0;
    v41 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SFX_Flash_SFX_Flash_comportement_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
    {
        if (v40->GFX_Flash == -1)
        {
            v40->GFX_Flash = AI_EvalFunc_GFXAdd_C_CURRENT(18);
            v16 = (tGameObjectMain*)SFX_Flash_SFX_Flash_comportement_SFX_light_smoke_gao;
            if (SFX_Flash_SFX_Flash_comportement_SFX_light_smoke_gao)
            {
                if ((unsigned int)SFX_Flash_SFX_Flash_comportement_SFX_light_smoke_gao <= 2)
                    v16 = AI_MainActor[SFX_Flash_SFX_Flash_comportement_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v40->GFX_Flash, v16, 11);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v40->GFX_Flash, 0, 11);
            }
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v40->GFX_Flash, 2, 1);
            GFXSetCurGlobal(v40->GFX_Flash);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18010, v40->des_f_param1);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18012, v40->des_f_param2);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18000, v40->des_f_big_alpha);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18001, v40->des_f_big_length_min);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18002, v40->des_f_big_length_max);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18003, v40->des_f_big_width_min);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18004, v40->des_f_big_width_max);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18005, v40->des_f_small_alpha);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18006, v40->des_f_small_length_min);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18007, v40->des_f_small_length_max);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18008, v40->des_f_small_width_min);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18009, v40->des_f_small_width_max);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18011, 0.5);
            AI_EvalFunc_GFXSeti_C_CURGLOB(18101, v40->des_i_nb_rayons);
            AI_EvalFunc_GFXSeti_C_CURGLOB(18103, 4);
            AI_EvalFunc_GFXSeti_C_CURGLOB(18100, 1);
            v40->f_time_start_flash = TIM_gf_MainClock - TIM_gf_LockTime;
        }
        v40->f_time_stop_flash = TIM_gf_MainClock - TIM_gf_LockTime;
        v18 = AI_EvalFunc_VIEWGetObject_C(0);
        v15 = (tGameObjectMain*)v40->des_o_snap_actor;
        if (v40->des_o_snap_actor)
        {
            if ((unsigned int)v15 <= 2)
                v15 = AI_MainActor[(uint32_t)v15 - 1];
            AI_EvalFunc_OBJGetPos_C(v15, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        v22 = a3;
        v14 = (tGameObjectMain*)v40->des_o_snap_actor;
        if (v40->des_o_snap_actor)
        {
            if ((unsigned int)v14 <= 2)
                v14 = AI_MainActor[(uint32_t)v14 - 1];
            AI_EvalFunc_MATHVecLocalToGlobal_C(v14, &v40->des_v_offset, &v43);
        }
        else
        {
            AI_EvalFunc_MATHVecLocalToGlobal_C(0, &v40->des_v_offset, &v43);
        }
        v13 = (tGameObjectMain*)v40->des_o_snap_actor;
        if (v40->des_o_snap_actor)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            v0 = AI_EvalFunc_OBJ_ZoomGet_C(v13);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_ZoomGet_C(0);
        }
        v21 = (float)v0;
        v33 = v43.x * v21;
        v34 = v43.y * v21;
        v35 = v43.z * v21;
        v22.x = v22.x + v33;
        v22.y = v22.y + v34;
        v22.z = v22.z + v35;
        v12 = v18;
        if (v18)
        {
            if ((unsigned int)v18 <= 2)
                v12 = AI_MainActor[(uint32_t)v18 - 1];
            AI_EvalFunc_OBJGetPos_C(v12, &v42);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v42);
        }
        MATH_SubstractVec(&dest, &v42, &v22);
        a1 = dest;
        v41 = MATH_VecLen(&a1);
        v11 = 1.0f / v41;
        a1.x = a1.x * v11;
        a1.y = a1.y * v11;
        a1.z = a1.z * v11;
        GFXSetCurGlobal(v40->GFX_Flash);
        v3 = v41 - 1.0f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(18013, v3);
        AI_EvalFunc_GFXSetv_C_CURGLOB(18200, &v22);
        if (AI_EvalFunc_TIMEElapsed_C(v40->f_time_start_flash, v40->des_f_delai_apparition))
        {
            AI_EvalFunc_GFXSeti_C_CURRENT(v40->GFX_Flash, 18102, v40->des_c_color);
        }
        else
        {
            v45 = TIM_gf_MainClock - TIM_gf_LockTime;
            v19 = (v45 - v40->f_time_start_flash) / v40->des_f_delai_apparition;
            v32 = AI_EvalFunc_COLORBlend_C(0, v40->des_c_color, v19);
            AI_EvalFunc_GFXSeti_C_CURRENT(v40->GFX_Flash, 18102, v32);
        }
    }
    else if (v40->GFX_Flash != -1)
    {
        v18 = AI_EvalFunc_VIEWGetObject_C(0);
        v10 = (tGameObjectMain*)v40->des_o_snap_actor;
        if (v40->des_o_snap_actor)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            AI_EvalFunc_OBJGetPos_C(v10, &v28);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v28);
        }
        v22 = v28;
        v9 = (tGameObjectMain*)v40->des_o_snap_actor;
        if (v40->des_o_snap_actor)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            AI_EvalFunc_MATHVecLocalToGlobal_C(v9, &v40->des_v_offset, &v17);
        }
        else
        {
            AI_EvalFunc_MATHVecLocalToGlobal_C(0, &v40->des_v_offset, &v17);
        }
        v8 = (tGameObjectMain*)v40->des_o_snap_actor;
        if (v40->des_o_snap_actor)
        {
            if ((unsigned int)v8 <= 2)
                v8 = AI_MainActor[(uint32_t)v8 - 1];
            v1 = AI_EvalFunc_OBJ_ZoomGet_C(v8);
        }
        else
        {
            v1 = AI_EvalFunc_OBJ_ZoomGet_C(0);
        }
        v29 = (float)v1;
        v36 = v17.x * v29;
        v37 = v17.y * v29;
        v38 = v17.z * v29;
        v22.x = v22.x + v36;
        v22.y = v22.y + v37;
        v22.z = v22.z + v38;
        v7 = v18;
        if (v18)
        {
            if ((unsigned int)v18 <= 2)
                v7 = AI_MainActor[(uint32_t)v18 - 1];
            AI_EvalFunc_OBJGetPos_C(v7, &v31);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v31);
        }
        MATH_SubstractVec(&v44, &v31, &v22);
        a1 = v44;
        v41 = MATH_VecLen(&a1);
        v6 = 1.0f / v41;
        a1.x = a1.x * v6;
        a1.y = a1.y * v6;
        a1.z = a1.z * v6;
        GFXSetCurGlobal(v40->GFX_Flash);
        v4 = v41 - v40->des_f_extrude_dist;
        AI_EvalFunc_GFXSetf_C_CURGLOB(18013, v4);
        AI_EvalFunc_GFXSetv_C_CURGLOB(18200, &v22);
        if (AI_EvalFunc_TIMEElapsed_C(v40->f_time_stop_flash, v40->des_f_delai_disparition))
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v40->GFX_Flash);
            v40->GFX_Flash = -1;
        }
        else
        {
            v26 = TIM_gf_MainClock - TIM_gf_LockTime;
            v19 = (v26 - v40->f_time_stop_flash) / v40->des_f_delai_disparition;
            v39 = AI_EvalFunc_COLORBlend_C(v40->des_c_color, 0, v19);
            AI_EvalFunc_GFXSeti_C_CURRENT(v40->GFX_Flash, 18102, v39);
        }
    }
    return 1000;
}