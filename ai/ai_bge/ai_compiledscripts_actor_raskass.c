#include "ai_compiledscripts_inc.h"

//Raskass_r_afterrec
int fct_Raskass_r_afterrec_Raskass_r_reflexe;
int fct_Raskass_r_afterrec_Raskass_r_exec_testgens;
int fct_Raskass_r_afterrec_Raskass_r_tracepos;

//Raskass_r_ripple
int Raskass_r_ripple_SFX_light_smoke_gao;

//Raskass_r_enter_zone
int Raskass_r_enter_zone_INV_manager_gao;
int Raskass_r_enter_zone_INV_manager_gao_i_Animals_Nb;
int Raskass_r_enter_zone_INV_manager_gao_ao_Animals;
int Raskass_r_enter_zone_INV_manager_gao_ai_AnimalsData;
int fct_Raskass_r_enter_zone_Raskass_r_move;

//Raskass_r_sortie_zone
int fct_Raskass_r_sortie_zone_Raskass_r_init;

//Raskass_r_attend_zone
int fct_Raskass_r_attend_zone_Raskass_r_enter_zone;

//Raskass_r_move
int Raskass_r_move_univVars;
int Raskass_r_move_univVars_Flag_speciaux_sauve;
int fct_Raskass_r_move_Raskass_r_sortie_zone;
int fct_Raskass_r_move_Raskass_r_plouf;
int fct_Raskass_r_move_Raskass_r_saute;

//Raskass_r_init
int Raskass_r_init_INV_manager_gao;
int Raskass_r_init_INV_manager_gao_i_Animals_Nb;
int Raskass_r_init_INV_manager_gao_ao_Animals;
int Raskass_r_init_INV_manager_gao_ai_AnimalsData;
int Raskass_r_init_univVars;
int Raskass_r_init_univVars_Flag_speciaux_sauve;
int fct_Raskass_r_init_Raskass_r_attend_zone;

//Raskass_r_saute
int fct_Raskass_r_saute_Raskass_r_plouf;
int fct_Raskass_r_saute_Raskass_r_move;

//Raskass_r_reflexe
int fct_Raskass_r_reflexe_Raskass_r_trace;

int Raskass_r_plouf_SFX_explosion_gao;
int Raskass_r_plouf_Raskass_r_ripple;

int Raskass_r_afterrec()
{
    tScriptStruct_PNJRaskass* v1; // [esp+0h] [ebp-FCh]
    tGameObjectMain* v2; // [esp+4h] [ebp-F8h]
    tGameObjectMain* v3; // [esp+8h] [ebp-F4h]
    int v4; // [esp+Ch] [ebp-F0h]
    tGameObjectMain* v5; // [esp+10h] [ebp-ECh]
    int v6; // [esp+18h] [ebp-E4h]
    tGameObjectMain* a2[51]; // [esp+1Ch] [ebp-E0h] BYREF
    int v8; // [esp+E8h] [ebp-14h]
    int v9; // [esp+ECh] [ebp-10h]
    int v10; // [esp+F0h] [ebp-Ch]
    tScriptStruct_PNJRaskass* v11; // [esp+F8h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJRaskass*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v11 = v1;
    v8 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Raskass_r_afterrec_Raskass_r_reflexe = (int)AI_ul_RealResolveOneRefFunction(0x3D000CDDu);
        fct_Raskass_r_afterrec_Raskass_r_exec_testgens = (int)AI_ul_RealResolveOneRefFunction(0x3D001D8Bu);
        fct_Raskass_r_afterrec_Raskass_r_tracepos = (int)AI_ul_RealResolveOneRefFunction(0x3D000CE4u);
    }
    v6 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Raskass_r_afterrec_Raskass_r_reflexe);
    if (v6 != 1000)
        return v6;
    if (!v11->des_i_constraint && v11->i_etat <= 1)
    {
        v5 = (tGameObjectMain*)v11->des_o_followed;
        v3 = v5;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v3 = AI_MainActor[(uint32_t)v5 - 1];
            v2 = v3;
        }
        else
        {
            v2 = 0;
        }
        if (!v2)
            v5 = *AI_MainActor;
        v11->o_test_gen = (int)v5;
        v10 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Raskass_r_afterrec_Raskass_r_exec_testgens);
        if (v10 != 1000)
            return v10;
        v8 = AI_EvalFunc_COLBVGetAllOverlaps_C(AI_gpst_CurrentGameObject, a2, 2, 0, 1);
        while (v8)
        {
            v11->o_test_gen = (int)a2[--v8];
            v4 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Raskass_r_afterrec_Raskass_r_exec_testgens);
            if (v4 != 1000)
                return v4;
        }
    }
    v9 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Raskass_r_afterrec_Raskass_r_tracepos);
    if (v9 != 1000)
        return v9;
    v11->v_pos_trace.z = v11->f_water_Z;
    AI_EvalFunc_GFXSetv_C_CURRENT(v11->GFX_Airtrace, 12206, &v11->v_pos_trace);
    return 1000;
}

int Raskass_r_ripple()
{
    float v1; // [esp+0h] [ebp-5Ch]
    tScriptStruct_PNJRaskass* v2; // [esp+4h] [ebp-58h]
    tGameObjectMain* v3; // [esp+Ch] [ebp-50h]
    Vec3 v4; // [esp+18h] [ebp-44h] BYREF
    float v5; // [esp+24h] [ebp-38h]
    Vec3 v6; // [esp+28h] [ebp-34h] BYREF
    Vec3 v7; // [esp+38h] [ebp-24h] BYREF
    float v8; // [esp+44h] [ebp-18h]
    Vec3 v9; // [esp+48h] [ebp-14h] BYREF
    tScriptStruct_PNJRaskass* v10; // [esp+54h] [ebp-8h]
    int a1; // [esp+58h] [ebp-4h]

    v6.x = 1.0;
    v6.y = 0.0;
    v6.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJRaskass*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v10 = v2;
    a1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Raskass_r_ripple_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    a1 = AI_EvalFunc_GFXAdd_C_CURRENT(20);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v9);
    v6 = v9;
    v6.z = v10->f_water_Z;
    v3 = (tGameObjectMain*)Raskass_r_ripple_SFX_light_smoke_gao;
    if (Raskass_r_ripple_SFX_light_smoke_gao)
    {
        if ((unsigned int)Raskass_r_ripple_SFX_light_smoke_gao <= 2)
            v3 = AI_MainActor[Raskass_r_ripple_SFX_light_smoke_gao - 1];
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, v3, 16);
    }
    else
    {
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, 0, 16);
    }
    v8 = AI_EvalFunc_MATHRandFloat_C(1.5, 2.0);
    GFXSetCurGlobal(a1);
    AI_EvalFunc_GFXSetf_C_CURGLOB(20000, v8);
    v5 = AI_EvalFunc_MATHRandFloat_C(2.0, 3.0);
    v1 = v5 + 8.0f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(20001, v1);
    AI_EvalFunc_GFXSetf_C_CURGLOB(20002, 0.30000001f);
    AI_EvalFunc_GFXSetf_C_CURGLOB(20003, 0.0);
    AI_EvalFunc_GFXSetf_C_CURGLOB(20004, v10->des_f_hauteur);
    AI_EvalFunc_GFXSeti_C_CURGLOB(20100, 3);
    AI_EvalFunc_GFXSeti_C_CURGLOB(20100, -2);
    AI_EvalFunc_GFXSeti_C_CURGLOB(20101, 0x808080);
    AI_EvalFunc_GFXSeti_C_CURGLOB(20102, 0);
    AI_EvalFunc_GFXSetv_C_CURGLOB(20200, &v6);
    v4.x = 0.0;
    v4.y = 0.0;
    v4.z = 1.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(20201, &v4);
    v7.x = 0.0;
    v7.y = 1.0;
    v7.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(20202, &v7);
    return 1000;
}

int Raskass_r_enter_zone()
{
    int result; // eax
    tScriptStruct_PNJRaskass* v1; // [esp+4h] [ebp-A4h]
    tGameObjectMain* v2; // [esp+Ch] [ebp-9Ch]
    tGameObjectMain* v3; // [esp+14h] [ebp-94h]
    tGameObjectMain* v4; // [esp+24h] [ebp-84h]
    tGameObjectMain* v5; // [esp+28h] [ebp-80h]
    tGameObjectMain* v6; // [esp+2Ch] [ebp-7Ch]
    tGameObjectMain* v7; // [esp+30h] [ebp-78h]
    tGameObjectMain* v8; // [esp+34h] [ebp-74h]
    Vec3 a3; // [esp+38h] [ebp-70h] BYREF
    tGameObjectMain* v10; // [esp+44h] [ebp-64h]
    float v11; // [esp+4Ch] [ebp-5Ch]
    float v12; // [esp+50h] [ebp-58h]
    float v13; // [esp+54h] [ebp-54h]
    Vec3 a1; // [esp+58h] [ebp-50h] BYREF
    float v15; // [esp+64h] [ebp-44h]
    float v16; // [esp+68h] [ebp-40h]
    float v17; // [esp+6Ch] [ebp-3Ch]
    Vec3 v19; // [esp+74h] [ebp-34h] BYREF
    int i; // [esp+80h] [ebp-28h]
    int v21; // [esp+84h] [ebp-24h]
    Vec3 v22; // [esp+88h] [ebp-20h] BYREF
    int v23; // [esp+94h] [ebp-14h]
    tGameObjectMain* v24; // [esp+98h] [ebp-10h]
    tScriptStruct_PNJRaskass* v25; // [esp+9Ch] [ebp-Ch]
    float v26; // [esp+A0h] [ebp-8h]
    float v27; // [esp+A4h] [ebp-4h]

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJRaskass*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v25 = v1;
    v10 = 0;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Raskass_r_enter_zone_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &Raskass_r_enter_zone_INV_manager_gao_i_Animals_Nb,
            (void*)Raskass_r_enter_zone_INV_manager_gao,
            23888);
        AI2C_ResolveBuffer(
            &Raskass_r_enter_zone_INV_manager_gao_ao_Animals,
            (void*)Raskass_r_enter_zone_INV_manager_gao,
            23368);
        AI2C_ResolveBuffer(
            &Raskass_r_enter_zone_INV_manager_gao_ai_AnimalsData,
            (void*)Raskass_r_enter_zone_INV_manager_gao,
            23628);
        fct_Raskass_r_enter_zone_Raskass_r_move = (int)AI_ul_RealResolveOneRefFunction(0x3D000CDBu);
    }
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(256))
    {
        for (i = 0; i < *(int32_t*)Raskass_r_enter_zone_INV_manager_gao_i_Animals_Nb; ++i)
        {
            v24 = AI_gpst_CurrentGameObject;
            v8 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v8 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v7 = v8;
            }
            else
            {
                v7 = 0;
            }
            if (*(tGameObjectMain**)(Raskass_r_enter_zone_INV_manager_gao_ao_Animals + 4 * i + 4) == v7)
            {
                --* (uint32_t*)Raskass_r_enter_zone_INV_manager_gao_i_Animals_Nb;
                *(uint32_t*)(Raskass_r_enter_zone_INV_manager_gao_ao_Animals + 4 * i + 4) = *(uint32_t*)(Raskass_r_enter_zone_INV_manager_gao_ao_Animals
                    + 4
                    * *(uint32_t*)Raskass_r_enter_zone_INV_manager_gao_i_Animals_Nb
                    + 4);
                *(uint32_t*)(Raskass_r_enter_zone_INV_manager_gao_ai_AnimalsData + 4 * i + 4) = *(uint32_t*)(Raskass_r_enter_zone_INV_manager_gao_ai_AnimalsData
                    + 4
                    * *(uint32_t*)Raskass_r_enter_zone_INV_manager_gao_i_Animals_Nb
                    + 4);
                break;
            }
        }
        v23 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v23 != 1000)
            return v23;
    }
    v25->i_dans_la_place = 1;
    v10 = (tGameObjectMain*)v25->des_o_followed;
    v6 = v10;
    if (v10)
    {
        if ((unsigned int)v10 <= 2)
            v6 = AI_MainActor[(uint32_t)v10 - 1];
        v5 = v6;
    }
    else
    {
        v5 = 0;
    }
    if (!v5)
        v10 = *AI_MainActor;
    v4 = v10;
    if (v10)
    {
        if ((unsigned int)v10 <= 2)
            v4 = AI_MainActor[(uint32_t)v10 - 1];
        AI_EvalFunc_OBJGetPos_C(v4, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    a1 = a3;
    if (v25->des_i_mode != 1)
    {
        a1.z = a1.z - 50.0f;
        v3 = v10;
        if (v10)
        {
            if ((unsigned int)v10 <= 2)
                v3 = AI_MainActor[(uint32_t)v10 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v3, &v19);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &v19);
        }
        v26 = AI_EvalFunc_MATHRandFloat_C(100.0, 200.0);
        v11 = v19.x * v26;
        v12 = v19.y * v26;
        v13 = v19.z * v26;
        a1.x = a1.x + v11;
        a1.y = a1.y + v12;
        a1.z = a1.z + v13;
        v2 = v10;
        if (v10)
        {
            if ((unsigned int)v10 <= 2)
                v2 = AI_MainActor[(uint32_t)v10 - 1];
            AI_EvalFunc_OBJ_HorizonGet_C(v2, &v22);
        }
        else
        {
            AI_EvalFunc_OBJ_HorizonGet_C(0, &v22);
        }
        v27 = AI_EvalFunc_MATHRandFloat_C(-50.0, 50.0);
        v15 = v22.x * v27;
        v16 = v22.y * v27;
        v17 = v22.z * v27;
        a1.x = a1.x + v15;
        a1.y = a1.y + v16;
        a1.z = a1.z + v17;
    }
    AI_EvalFunc_OBJSetPos_C_CURRENT(&a1);
    AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
    v25->f_time_inside_etat = 0.0;
    v25->f_time_change_etat = 0.0;
    v21 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Raskass_r_enter_zone_Raskass_r_move);
    if (v21 == 1000)
        result = 1000;
    else
        result = v21;
    return result;
}

int Raskass_r_sortie_zone()
{
    int result; // eax
    float a4; // [esp+0h] [ebp-D8h]
    float v2; // [esp+8h] [ebp-D0h]
    tScriptStruct_PNJRaskass* v3; // [esp+Ch] [ebp-CCh]
    float v4; // [esp+18h] [ebp-C0h]
    Vec3 v5; // [esp+38h] [ebp-A0h] BYREF
    Vec3 a1; // [esp+44h] [ebp-94h] BYREF
    Vec3 a3; // [esp+50h] [ebp-88h] BYREF
    Vec3 v8; // [esp+5Ch] [ebp-7Ch] BYREF
    Vec3 a5; // [esp+68h] [ebp-70h] BYREF
    Vec3 v10; // [esp+74h] [ebp-64h] BYREF
    float v11; // [esp+80h] [ebp-58h]
    Vec3 a2; // [esp+84h] [ebp-54h] BYREF
    Vec3 v13; // [esp+90h] [ebp-48h] BYREF
    Vec3 v14; // [esp+9Ch] [ebp-3Ch] BYREF
    Vec3 v15; // [esp+A8h] [ebp-30h] BYREF
    Vec3 v16; // [esp+B4h] [ebp-24h] BYREF
    tScriptStruct_PNJRaskass* v17; // [esp+C0h] [ebp-18h]
    Vec3 v18; // [esp+C4h] [ebp-14h] BYREF
    float v19; // [esp+D0h] [ebp-8h]
    int v20; // [esp+D4h] [ebp-4h]

    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_PNJRaskass*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v17 = v3;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Raskass_r_sortie_zone_Raskass_r_init = (int)AI_ul_RealResolveOneRefFunction(0x3D000CD0u);
    }
    v17->i_dans_la_place = 0;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v10);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v16);
    v14.x = v16.x * 20.0f;
    v14.y = v16.y * 20.0f;
    v14.z = v16.z * 20.0f;
    MATH_VecAddVec(&a1, &v10, &v14);
    a3.x = a1.x;
    a3.y = a1.y;
    a3.z = -200.0;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v5);
    a3.x = a3.x - v5.x;
    a3.y = a3.y - v5.y;
    a3.z = a3.z - v5.z;
    MATH_NormalizeEqualVector(&a3);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
    v19 = TIM_gf_dt;
    a4 = TIM_gf_dt * 8.0f;
    AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
    a3 = a5;
    v15.x = 0.0;
    v15.y = 0.0;
    v15.z = 1.0;
    AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a3, &v15);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v18);
    v11 = TIM_gf_dt;
    v2 = (float)v17->f_vitesse;
    v4 = v2 * TIM_gf_dt;
    v13.x = v18.x * v4;
    v13.y = v18.y * v4;
    v13.z = v18.z * v4;
    AI_EvalFunc_OBJMove_C(AI_gpst_CurrentGameObject, &v13);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v8);
    a3 = v8;
    if (v8.z >= -80.0
        || (v20 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Raskass_r_sortie_zone_Raskass_r_init), v20 == 1000))
    {
        result = 1000;
    }
    else
    {
        result = v20;
    }
    return result;
}

int Raskass_r_attend_zone()
{
    BOOL v1; // eax
    tScriptStruct_PNJRaskass* v2; // [esp+0h] [ebp-3Ch]
    tGameObjectMain* v3; // [esp+8h] [ebp-34h]
    tGameObjectMain* a2; // [esp+Ch] [ebp-30h]
    tGameObjectMain* v5; // [esp+10h] [ebp-2Ch]
    tGameObjectMain* v6; // [esp+14h] [ebp-28h]
    tGameObjectMain* v7; // [esp+18h] [ebp-24h]
    int v8; // [esp+1Ch] [ebp-20h]
    tGameObjectMain* v9; // [esp+24h] [ebp-18h]
    int v10; // [esp+2Ch] [ebp-10h]
    int v11; // [esp+30h] [ebp-Ch]
    int i; // [esp+38h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJRaskass*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Raskass_r_attend_zone_Raskass_r_enter_zone = (int)AI_ul_RealResolveOneRefFunction(0x3D000CD8u);
    }
    v2->i_dans_la_place = 0;
    v9 = (tGameObjectMain*)v2->des_o_followed;
    v7 = v9;
    if (v9)
    {
        if ((unsigned int)v9 <= 2)
            v7 = AI_MainActor[(uint32_t)v9 - 1];
        v6 = v7;
    }
    else
    {
        v6 = 0;
    }
    if (!v6)
        v9 = *AI_MainActor;
    if (v2->des_i_mode == 1)
    {
        v11 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Raskass_r_attend_zone_Raskass_r_enter_zone);
        if (v11 != 1000)
            return v11;
    }
    if (!v2->des_i_numzones)
    {
        v10 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Raskass_r_attend_zone_Raskass_r_enter_zone);
        if (v10 != 1000)
            return v10;
    }
    for (i = 0; i < v2->des_i_numzones; ++i)
    {
        v2->o_cur_zone = v2->des_o_allzones[i];
        v5 = (tGameObjectMain*)v2->o_cur_zone;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            a2 = v5;
        }
        else
        {
            a2 = 0;
        }
        v3 = v9;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v3 = AI_MainActor[(uint32_t)v9 - 1];
            v1 = AI_EvalFunc_COLBVObjectObject_C(v3, a2);
        }
        else
        {
            v1 = AI_EvalFunc_COLBVObjectObject_C(0, a2);
        }
        if (v1)
        {
            v8 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Raskass_r_attend_zone_Raskass_r_enter_zone);
            if (v8 != 1000)
                return v8;
        }
    }
    return 1000;
}

int Raskass_r_move()
{
    BOOL v0; // eax
    double v2; // st7
    double v3; // st7
    double v4; // st7
    int v5; // eax
    float a4; // [esp+8h] [ebp-424h]
    float v7; // [esp+Ch] [ebp-420h]
    float v8; // [esp+10h] [ebp-41Ch]
    float v9; // [esp+14h] [ebp-418h]
    float v10; // [esp+18h] [ebp-414h]
    float v11; // [esp+1Ch] [ebp-410h]
    float v12; // [esp+20h] [ebp-40Ch]
    float v13; // [esp+24h] [ebp-408h]
    tScriptStruct_PNJRaskass* v14; // [esp+28h] [ebp-404h]
    float v15; // [esp+28h] [ebp-404h]
    float v16; // [esp+28h] [ebp-404h]
    float v17; // [esp+2Ch] [ebp-400h]
    double v18; // [esp+34h] [ebp-3F8h]
    double v19; // [esp+3Ch] [ebp-3F0h]
    double v20; // [esp+44h] [ebp-3E8h]
    float v21; // [esp+50h] [ebp-3DCh]
    tGameObjectMain* v22; // [esp+6Ch] [ebp-3C0h]
    Vec3* v23; // [esp+74h] [ebp-3B8h]
    float v24; // [esp+78h] [ebp-3B4h]
    Vec3* v25; // [esp+7Ch] [ebp-3B0h]
    float v26; // [esp+80h] [ebp-3ACh]
    tGameObjectMain* v27; // [esp+A0h] [ebp-38Ch]
    tGameObjectMain* v28; // [esp+A8h] [ebp-384h]
    tGameObjectMain* v29; // [esp+C8h] [ebp-364h]
    Vec3* v30; // [esp+CCh] [ebp-360h]
    Vec3* v31; // [esp+D0h] [ebp-35Ch]
    tGameObjectMain* v32; // [esp+E0h] [ebp-34Ch]
    tGameObjectMain* v33; // [esp+E8h] [ebp-344h]
    Vec3* v34; // [esp+ECh] [ebp-340h]
    Vec3* v35; // [esp+F0h] [ebp-33Ch]
    tGameObjectMain* v36; // [esp+100h] [ebp-32Ch]
    Vec3* v37; // [esp+104h] [ebp-328h]
    Vec3* v38; // [esp+108h] [ebp-324h]
    Vec3* v39; // [esp+10Ch] [ebp-320h]
    Vec3* v40; // [esp+114h] [ebp-318h]
    tGameObjectMain* v41; // [esp+128h] [ebp-304h]
    Vec3* v42; // [esp+12Ch] [ebp-300h]
    float v43; // [esp+130h] [ebp-2FCh]
    double v44; // [esp+134h] [ebp-2F8h]
    tGameObjectMain* v45; // [esp+154h] [ebp-2D8h]
    tGameObjectMain* v46; // [esp+164h] [ebp-2C8h]
    tGameObjectMain* v47; // [esp+174h] [ebp-2B8h]
    tGameObjectMain* v48; // [esp+17Ch] [ebp-2B0h]
    tGameObjectMain* v49; // [esp+180h] [ebp-2ACh]
    tGameObjectMain* v50; // [esp+184h] [ebp-2A8h]
    tGameObjectMain* v51; // [esp+188h] [ebp-2A4h]
    tGameObjectMain* v52; // [esp+18Ch] [ebp-2A0h]
    tGameObjectMain* v53; // [esp+190h] [ebp-29Ch]
    tGameObjectMain* v54; // [esp+194h] [ebp-298h]
    Vec3 v55; // [esp+1A4h] [ebp-288h] BYREF
    Vec3 v56; // [esp+1B0h] [ebp-27Ch] BYREF
    float v57; // [esp+1C4h] [ebp-268h]
    BOOL v58; // [esp+1C8h] [ebp-264h]
    float v59; // [esp+1CCh] [ebp-260h]
    float v60; // [esp+1D0h] [ebp-25Ch]
    Vec3 v61; // [esp+1D4h] [ebp-258h] BYREF
    float v62; // [esp+1E0h] [ebp-24Ch]
    int v63; // [esp+1E4h] [ebp-248h]
    int v64; // [esp+1E8h] [ebp-244h]
    int v65; // [esp+1ECh] [ebp-240h]
    tGameObjectMain* v66; // [esp+1F0h] [ebp-23Ch]
    BOOL v67; // [esp+1F8h] [ebp-234h]
    Vec3 v68; // [esp+1FCh] [ebp-230h]
    Vec3 v69; // [esp+208h] [ebp-224h] BYREF
    Vec3 v70; // [esp+21Ch] [ebp-210h] BYREF
    Vec3 a5; // [esp+228h] [ebp-204h] BYREF
    float v72; // [esp+234h] [ebp-1F8h]
    float v73; // [esp+238h] [ebp-1F4h]
    float v74; // [esp+23Ch] [ebp-1F0h]
    Vec3 dest; // [esp+240h] [ebp-1ECh] BYREF
    float v76; // [esp+24Ch] [ebp-1E0h]
    Vec3 v77; // [esp+258h] [ebp-1D4h] BYREF
    int v78; // [esp+268h] [ebp-1C4h]
    float v79; // [esp+26Ch] [ebp-1C0h]
    Vec3 v80; // [esp+278h] [ebp-1B4h] BYREF
    Vec3 v81; // [esp+284h] [ebp-1A8h] BYREF
    Vec3 v82; // [esp+290h] [ebp-19Ch] BYREF
    Vec3 v83; // [esp+29Ch] [ebp-190h] BYREF
    int v84; // [esp+2ACh] [ebp-180h]
    Vec3 a3; // [esp+2B4h] [ebp-178h] BYREF
    Vec3 v86; // [esp+2C0h] [ebp-16Ch] BYREF
    Vec3 v87; // [esp+2CCh] [ebp-160h] BYREF
    Vec3 v88; // [esp+2D8h] [ebp-154h] BYREF
    float v89; // [esp+2E4h] [ebp-148h]
    float v90; // [esp+2ECh] [ebp-140h]
    float v91; // [esp+2F0h] [ebp-13Ch]
    float v92; // [esp+2F4h] [ebp-138h]
    Vec3 v93; // [esp+2F8h] [ebp-134h] BYREF
    float v94; // [esp+304h] [ebp-128h]
    float v95; // [esp+308h] [ebp-124h]
    float v96; // [esp+30Ch] [ebp-120h]
    Vec3 v97; // [esp+31Ch] [ebp-110h]
    int v98; // [esp+330h] [ebp-FCh]
    Vec3 v99; // [esp+334h] [ebp-F8h] BYREF
    float v100; // [esp+340h] [ebp-ECh]
    float v101; // [esp+348h] [ebp-E4h]
    float v102; // [esp+34Ch] [ebp-E0h]
    BOOL v103; // [esp+350h] [ebp-DCh]
    Vec3 v104; // [esp+354h] [ebp-D8h] BYREF
    Vec3 v105; // [esp+360h] [ebp-CCh]
    Vec3 v106; // [esp+378h] [ebp-B4h] BYREF
    Vec3 v107; // [esp+384h] [ebp-A8h] BYREF
    int v108; // [esp+390h] [ebp-9Ch]
    float v109; // [esp+398h] [ebp-94h]
    Vec3 v110; // [esp+39Ch] [ebp-90h] BYREF
    float v111; // [esp+3A8h] [ebp-84h]
    float v112; // [esp+3ACh] [ebp-80h]
    float v113; // [esp+3B0h] [ebp-7Ch]
    float v114; // [esp+3B4h] [ebp-78h]
    Vec3 v115; // [esp+3C8h] [ebp-64h] BYREF
    tScriptStruct_PNJRaskass* v116; // [esp+3D4h] [ebp-58h]
    float v117; // [esp+3D8h] [ebp-54h]
    float v118; // [esp+3DCh] [ebp-50h]
    Vec3 v119; // [esp+3E0h] [ebp-4Ch] BYREF
    Vec3 v120; // [esp+3ECh] [ebp-40h] BYREF
    Vec3 v121; // [esp+3F8h] [ebp-34h] BYREF
    float v122; // [esp+404h] [ebp-28h]
    float v123; // [esp+40Ch] [ebp-20h]
    float v124; // [esp+420h] [ebp-Ch]

    v68.x = 1.0;
    v68.y = 0.0;
    v68.z = 0.0;
    v104.x = 1.0;
    v104.y = 0.0;
    v104.z = 0.0;
    v105.x = 1.0;
    v105.y = 0.0;
    v105.z = 0.0;
    v86.x = 1.0;
    v86.y = 0.0;
    v86.z = 0.0;
    v97.x = 1.0;
    v97.y = 0.0;
    v97.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v14 = (tScriptStruct_PNJRaskass*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v14 = 0;
    v116 = v14;
    v66 = 0;
    v79 = 0.0;
    v117 = 0.0;
    v58 = 0;
    v118 = 0.0;
    v59 = 0.0;
    v102 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Raskass_r_move_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&Raskass_r_move_univVars_Flag_speciaux_sauve, (void*)Raskass_r_move_univVars, 4488);
        fct_Raskass_r_move_Raskass_r_sortie_zone = (int)AI_ul_RealResolveOneRefFunction(0x3D000CEFu);
        fct_Raskass_r_move_Raskass_r_plouf = (int)AI_ul_RealResolveOneRefFunction(0x3D000CE8u);
        fct_Raskass_r_move_Raskass_r_saute = (int)AI_ul_RealResolveOneRefFunction(0x3D000CE6u);
    }
    v116->i_nage_init = 0;
    if (v116->des_i_mode != 1)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v77);
        v86 = v77;
        if (v77.z >= -5.0)
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
        else
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
    }
    v58 = AI_EvalFunc_ARRBitTest_C((int*)(Raskass_r_move_univVars_Flag_speciaux_sauve + 4), 2718);
    if (v116->des_i_mode != 1)
        AI_EvalFunc_OBJ_InfoPhotoFlagsSet_C(AI_gpst_CurrentGameObject, 1, 0);
    v66 = (tGameObjectMain*)v116->des_o_followed;
    v54 = v66;
    if (v66)
    {
        if ((unsigned int)v66 <= 2)
            v54 = AI_MainActor[(uint32_t)v66 - 1];
        v53 = v54;
    }
    else
    {
        v53 = 0;
    }
    if (!v53)
        v66 = *AI_MainActor;
    if (v58)
        goto LABEL_181;
    v52 = (tGameObjectMain*)v116->o_cur_zone;
    if (v52)
    {
        if ((unsigned int)v52 <= 2)
            v52 = AI_MainActor[(uint32_t)v52 - 1];
        v51 = v52;
    }
    else
    {
        v51 = 0;
    }
    if (v51)
    {
        v50 = (tGameObjectMain*)v116->o_cur_zone;
        if (v50)
        {
            if ((unsigned int)v50 <= 2)
                v50 = AI_MainActor[(uint32_t)v50 - 1];
            v49 = v50;
        }
        else
        {
            v49 = 0;
        }
        v48 = v66;
        if (v66)
        {
            if ((unsigned int)v66 <= 2)
                v48 = AI_MainActor[(uint32_t)v66 - 1];
            v0 = AI_EvalFunc_COLBVObjectObject_C(v48, v49);
        }
        else
        {
            v0 = AI_EvalFunc_COLBVObjectObject_C(0, v49);
        }
        v67 = v0;
        if (!v0)
        {
        LABEL_181:
            v65 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Raskass_r_move_Raskass_r_sortie_zone);
            if (v65 != 1000)
                return v65;
        }
    }
    v47 = v66;
    if (v66)
    {
        if ((unsigned int)v66 <= 2)
            v47 = AI_MainActor[(uint32_t)v66 - 1];
        AI_EvalFunc_OBJGetPos_C(v47, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    v97 = a3;
    v46 = v66;
    if (v66)
    {
        if ((unsigned int)v66 <= 2)
            v46 = AI_MainActor[(uint32_t)v66 - 1];
        AI_EvalFunc_OBJ_SightGet_C(v46, &v121);
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C(0, &v121);
    }
    v105 = v121;
    v45 = v66;
    if (v66)
    {
        if ((unsigned int)v66 <= 2)
            v45 = AI_MainActor[(uint32_t)v66 - 1];
        v2 = AI_EvalFunc_DYNSpeedGet_C(v45);
    }
    else
    {
        v2 = AI_EvalFunc_DYNSpeedGet_C(0);
    }
    v117 = (float)v2;
    v58 = 0;
    if (!v116->i_can_jump)
        goto LABEL_70;
    if (v116->i_etat > 3)
        goto LABEL_70;
    if (v116->f_vitesse <= 15)
        goto LABEL_70;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v99);
    v86 = v99;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v119);
    v68 = v119;
    if (v119.z <= 0.40000001 || -v116->des_f_hauteur_peut_saut >= v86.z)
        goto LABEL_70;
    v78 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Raskass_r_move_Raskass_r_plouf);
    if (v78 != 1000)
        return v78;
    v116->f_max_z = 0.0;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v116->v_speed_saut);
    if (v58)
        v116->v_speed_saut.z = AI_EvalFunc_MATHRandFloat_C(0.0, 0.02f);
    else
        v116->v_speed_saut.z = AI_EvalFunc_MATHRandFloat_C(0.0, 0.2f) + v116->v_speed_saut.z;
    v116->i_can_jump_cote = AI_EvalFunc_MATHRandInt_C(0, 2);
    if (v116->f_lime_va_vite < (double)v117)
    {
        AI_EvalFunc_SNAKE_SetInertia_C(AI_gpst_CurrentGameObject, 10.0);
        v118 = AI_EvalFunc_MATHRandFloat_C(90.0, 100.0);
        v13 = (float)v116->f_vitesse;
        if (v13 > (double)v118)
        {
            v44 = v118 + MATH_gd_magicDoubleForRound;
            v116->f_vitesse = (*(uint32_t *)&v44);
        }
        v116->i_can_jump_cote = 0;
    }
    v43 = (float)v116->f_vitesse;
    v42 = &v116->v_speed_saut;
    v116->v_speed_saut.x = v43 * v116->v_speed_saut.x;
    v42->y = v43 * v42->y;
    v42->z = v43 * v42->z;
    v116->f_time_change_etat = TIM_gf_MainClock - TIM_gf_LockTime;
    v116->i_etat = 20;
    v63 = AI_EvalFunc_MATHRandInt_C(0, 3);
    v84 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(AI_gpst_CurrentGameObject, v63, -1.0, 0.94999999f, 1.05f);
    v108 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(AI_gpst_CurrentGameObject, 3, -1.0, 0.94999999f, 1.05f);
    v98 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Raskass_r_move_Raskass_r_saute);
    if (v98 != 1000)
        return v98;
LABEL_70:
    v17 = v116->f_time_alea_etat * 1.5f;
    if (AI_EvalFunc_TIMEElapsed_C(v116->f_time_jump, v17))
        v116->i_can_jump = 1;
    v12 = (float)v116->f_vitesse;
    if (v116->des_f_hauteur + v116->des_f_hauteur > v12
        || (v103 = AI_EvalFunc_TIMEElapsed_C(v116->f_time_change_etat, v116->f_time_alea_etat)))
    {
        v116->i_can_jump = 1;
        if (v116->des_i_mode == 1)
        {
            if (v116->des_i_constraint)
                v116->f_time_alea_etat = AI_EvalFunc_MATHRandFloat_C(0.5, 1.0);
            else
                v116->f_time_alea_etat = AI_EvalFunc_MATHRandFloat_C(1.0, 2.0);
        }
        else
        {
            v116->f_time_alea_etat = AI_EvalFunc_MATHRandFloat_C(2.0, 4.0);
        }
        if (++v116->i_etat > 1)
            v116->i_etat = 0;
        v116->f_time_change_etat = TIM_gf_MainClock - TIM_gf_LockTime;
        v116->f_time_inside_etat = TIM_gf_MainClock - TIM_gf_LockTime;
        if (v116->i_etat <= 1)
        {
            if (v116->des_i_mode != 1)
                v66 = AI_EvalFunc_VIEWGetObject_C(0);
            v41 = v66;
            if (v66)
            {
                if ((unsigned int)v66 <= 2)
                    v41 = AI_MainActor[(uint32_t)v66 - 1];
                AI_EvalFunc_OBJ_HorizonGet_C(v41, &v115);
            }
            else
            {
                AI_EvalFunc_OBJ_HorizonGet_C(0, &v115);
            }
            v86 = v115;
            if (v116->des_i_mode == 1)
            {
                v33 = v66;
                if (v66)
                {
                    if ((unsigned int)v66 <= 2)
                        v33 = AI_MainActor[(uint32_t)v66 - 1];
                    AI_EvalFunc_BVRandomPosGet_C(v33, 0, &v82);
                }
                else
                {
                    AI_EvalFunc_BVRandomPosGet_C(0, 0, &v82);
                }
                v32 = v66;
                if (v66)
                {
                    if ((unsigned int)v66 <= 2)
                        v32 = AI_MainActor[(uint32_t)v66 - 1];
                    AI_EvalFunc_OBJGetPos_C(v32, &v106);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v106);
                }
                MATH_SubstractVec(&dest, &v82, &v106);
                v116->v_offset_pos = dest;
                v116->v_offset_pos.z = v116->f_water_Z - v116->des_f_hauteur;
                v30 = &v116->v_last_offset_pos;
                v31 = &v116->v_offset_pos;
                v116->v_last_offset_pos.x = v116->v_offset_pos.x;
                v30->y = v31->y;
                v30->z = v31->z;
            }
            else
            {
                if (v116->f_lime_va_vite >= (double)v117)
                {
                    v122 = AI_EvalFunc_MATHRandFloat_C(5.0, 10.0);
                    v72 = v86.x * v122;
                    v73 = v86.y * v122;
                    v74 = v86.z * v122;
                    v39 = &v116->v_offset_pos;
                    v116->v_offset_pos.x = v72;
                    v39->y = v73;
                    v39->z = v74;
                }
                else
                {
                    v89 = AI_EvalFunc_MATHRandFloat_C(0.0, 15.0);
                    v94 = v86.x * v89;
                    v95 = v86.y * v89;
                    v96 = v86.z * v89;
                    v40 = &v116->v_offset_pos;
                    v116->v_offset_pos.x = v94;
                    v40->y = v95;
                    v40->z = v96;
                }
                v116->v_offset_pos.x = 0.0;
                v116->v_offset_pos.y = 0.0;
                v116->v_offset_pos.z = 0.0;
                if (v116->i_etat == 1)
                {
                    v38 = &v116->v_offset_pos;
                    v37 = &v116->v_offset_pos;
                    v116->v_offset_pos.x = -v116->v_offset_pos.x;
                    v37->y = -v38->y;
                    v37->z = -v38->z;
                }
                v36 = v66;
                if (v66)
                {
                    if ((unsigned int)v66 <= 2)
                        v36 = AI_MainActor[(uint32_t)v66 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v36, &v61);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v61);
                }
                v86 = v61;
                if (v116->f_lime_va_vite >= (double)v117)
                {
                    v123 = AI_EvalFunc_MATHRandFloat_C(50.0, 60.0);
                    v90 = v86.x * v123;
                    v91 = v86.y * v123;
                    v92 = v86.z * v123;
                    v34 = &v116->v_offset_pos;
                    v116->v_offset_pos.x = v90 + v116->v_offset_pos.x;
                    v34->y = v91 + v34->y;
                    v34->z = v92 + v34->z;
                }
                else
                {
                    v76 = AI_EvalFunc_MATHRandFloat_C(80.0, 85.0);
                    v111 = v86.x * v76;
                    v112 = v86.y * v76;
                    v113 = v86.z * v76;
                    v35 = &v116->v_offset_pos;
                    v116->v_offset_pos.x = v111 + v116->v_offset_pos.x;
                    v35->y = v112 + v35->y;
                    v35->z = v113 + v35->z;
                }
                v116->v_offset_pos.z = -3.0;
            }
            v29 = v66;
            if (v66)
            {
                if ((unsigned int)v66 <= 2)
                    v29 = AI_MainActor[(uint32_t)v66 - 1];
                AI_EvalFunc_MATHVecGlobalToLocal_C(v29, &v116->v_offset_pos, &v83);
            }
            else
            {
                AI_EvalFunc_MATHVecGlobalToLocal_C(0, &v116->v_offset_pos, &v83);
            }
            v116->v_offset_pos = v83;
        }
    }
    if (v116->i_etat <= 3)
    {
        v11 = (float)v116->f_vitesse;
        v59 = v11 * 0.5f;
        if (v59 == 0.0)
            v59 = 1.0;
        if (v116->i_etat <= 1)
        {
            if (AI_EvalFunc_TIMEElapsed_C(v116->f_time_inside_etat, 0.5))
            {
                if (-v116->des_f_hauteur <= v116->v_offset_pos.z)
                    v15 = -(v116->des_f_hauteur * 3.0f);
                else
                    v15 = -v116->des_f_hauteur;
                v3 = AI_EvalFunc_MATHRandFloat_C(v15, v116->f_water_Z);
                v116->v_offset_pos.z = (float)v3;
                v116->f_time_inside_etat = TIM_gf_MainClock - TIM_gf_LockTime;
            }
        }
        v124 = TIM_gf_dt;
        v16 = v59 * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&v116->v_last_offset_pos, &v116->v_offset_pos, v16, &a5);
        v116->v_last_offset_pos = a5;
        v104 = v116->v_last_offset_pos;
        v28 = v66;
        if (v66)
        {
            if ((unsigned int)v66 <= 2)
                v28 = AI_MainActor[(uint32_t)v66 - 1];
            AI_EvalFunc_OBJGetPos_C(v28, &v87);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v87);
        }
        v27 = v66;
        if (v66)
        {
            if ((unsigned int)v66 <= 2)
                v27 = AI_MainActor[(uint32_t)v66 - 1];
            AI_EvalFunc_MATHVecLocalToGlobal_C(v27, &v104, &v70);
        }
        else
        {
            AI_EvalFunc_MATHVecLocalToGlobal_C(0, &v104, &v70);
        }
        MATH_VecAddVec(&v80, &v87, &v70);
        v116->v_pos_to_go = v80;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v55);
        MATH_SubstractVec(&v110, &v116->v_pos_to_go, &v55);
        v86.x = v110.x;
        v86.y = v110.y;
        v86.z = 0.0;
        v57 = MATH_VecLen(&v86);
        if (v57 < 4.0)
        {
            v102 = MATH_VecLen(&v116->v_last_offset_pos);
            if (v102 != 0.0f)
            {
                v25 = &v116->v_last_offset_pos;
                v26 = 1.0f / v102;
                v116->v_last_offset_pos.x = v26 * v116->v_last_offset_pos.x;
                v25->y = v26 * v25->y;
                v25->z = v26 * v25->z;
            }
            v24 = v116->des_f_hauteur + v102;
            v23 = &v116->v_last_offset_pos;
            v116->v_last_offset_pos.x = v24 * v116->v_last_offset_pos.x;
            v23->y = v24 * v23->y;
            v23->z = v24 * v23->z;
        }
        if (v116->des_i_mode == 1)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v56);
            MATH_SubstractVec(&v120, &v116->v_pos_to_go, &v56);
            v62 = MATH_VecLen(&v120);
            v79 = v62 * 0.2f;
            v116->f_inertie_rot = v79;
            if (v116->des_i_constraint)
            {
                v79 = 3.0;
                v116->f_inertie_rot = 3.0;
            }
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v69);
            MATH_SubstractVec(&v93, &v116->v_pos_to_go, &v69);
            v60 = MATH_VecLen(&v93);
            v79 = v60 * 0.5f;
            v22 = v66;
            if (v66)
            {
                if ((unsigned int)v66 <= 2)
                    v22 = AI_MainActor[(uint32_t)v66 - 1];
                v4 = AI_EvalFunc_DYNSpeedGet_C(v22);
            }
            else
            {
                v4 = AI_EvalFunc_DYNSpeedGet_C(0);
            }
            v79 = (float)v4 + v79;
            if (v116->v_pos_to_go.z < -3.0)
                v79 = v79 * 3.0f;
            v116->f_inertie_rot = v79 / 40.0f;
        }
        if (v116->f_inertie_rot > 1.0)
        {
            if (v116->f_inertie_rot > 2.0)
            {
                AI_EvalFunc_SNAKE_SetInertia_C(AI_gpst_CurrentGameObject, 1.0);
                v116->f_factor_sinus = 0.0;
                v116->f_inertie_sinus = 5.0;
                v116->f_friction = 0.89999998f;
                v116->f_gravity = AI_EvalFunc_MATHRandFloat_C(9.0, 10.0);
            }
            else
            {
                AI_EvalFunc_SNAKE_SetInertia_C(AI_gpst_CurrentGameObject, 5.0);
                v116->f_factor_sinus = 0.1f;
                v116->f_inertie_sinus = 5.0;
                v116->f_friction = 0.94999999f;
                v116->f_gravity = AI_EvalFunc_MATHRandFloat_C(6.0, 8.0);
            }
        }
        else
        {
            AI_EvalFunc_SNAKE_SetInertia_C(AI_gpst_CurrentGameObject, 10.0);
            v116->f_factor_sinus = 0.1f;
            v116->f_inertie_sinus = 5.0;
            v116->f_friction = 0.98000002f;
            v116->f_gravity = AI_EvalFunc_MATHRandFloat_C(6.0, 8.0);
        }
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v107);
    v86 = v107;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v81);
    v68 = v81;
    if (v81.z > 0.40000001 && v116->i_can_jump)
        v79 = v116->des_f_hauteur * 10.0f + v79;
    if (v116->i_etat == 3)
    {
        v116->f_inertie_rot = 10.0;
        v79 = v116->des_f_hauteur + v116->des_f_hauteur + v79;
    }
    v101 = TIM_gf_dt;
    v21 = 4.0f * TIM_gf_dt;
    if (v21 < 0.0)
        v21 = 0.0;
    if (v21 > 1.0)
        v21 = 1.0;
    v10 = (float)v116->f_vitesse;
    v109 = v79 * v21 + (1.0f - v21) * v10;
    v20 = v109 + MATH_gd_magicDoubleForRound;
    v116->f_vitesse = (*(uint32_t *)&v20);
    v9 = (float)v116->f_vitesse;
    if (v116->des_f_min_speed > (double)v9)
    {
        v19 = v116->des_f_min_speed + MATH_gd_magicDoubleForRound;
        v116->f_vitesse = (*(uint32_t*)&v19);
    }
    v8 = (float)v116->f_vitesse;
    if (v116->des_f_max_speed < (double)v8)
    {
        v18 = v116->des_f_max_speed + MATH_gd_magicDoubleForRound;
        v116->f_vitesse = (*(uint32_t*)&v18);
    }
    if (v116->i_sound_nage == -1)
    {
        v5 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 5, 0x18000000);
        v116->i_sound_nage = v5;
        AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v116->i_sound_nage);
        v64 = SND_l_GetFreq(v116->i_sound_nage);
        v100 = AI_EvalFunc_MATHRandFloat_C(0.80000001f, 1.1f);
        v7 = (float)v64;
        SND_SetFreq(v116->i_sound_nage, (unsigned int)(v7 * v100));
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v88);
        v86 = v88;
        if (v88.z > -1.0)
            v86.z = -1.0;
        a4 = (float)fabs(v86.z);
        v114 = a4;
        v86.z = 1.0f / (3.0f * a4);
        SND_SetVol(v116->i_sound_nage, v86.z);
    }
    return 1000;
}

int Raskass_r_init()
{
    int result; // eax
    BOOL v1; // eax
    tScriptStruct_PNJRaskass* v2; // [esp+Ch] [ebp-B0h]
    tGameObjectMain* v3; // [esp+10h] [ebp-ACh]
    tGameObjectMain* v4; // [esp+14h] [ebp-A8h]
    tGameObjectMain* v5; // [esp+18h] [ebp-A4h]
    tGameObjectMain* v6; // [esp+1Ch] [ebp-A0h]
    tGameObjectMain* v7; // [esp+20h] [ebp-9Ch]
    tGameObjectMain* v8; // [esp+24h] [ebp-98h]
    tGameObjectMain* v9; // [esp+28h] [ebp-94h]
    tGameObjectMain* v10; // [esp+2Ch] [ebp-90h]
    tGameObjectMain* v11; // [esp+34h] [ebp-88h]
    tGameObjectMain* v12; // [esp+38h] [ebp-84h]
    tGameObjectMain* v13; // [esp+3Ch] [ebp-80h]
    tGameObjectMain* v14; // [esp+48h] [ebp-74h]
    tGameObjectMain* v15; // [esp+4Ch] [ebp-70h]
    tGameObjectMain* v16; // [esp+54h] [ebp-68h]
    tGameObjectMain* v17; // [esp+58h] [ebp-64h]
    tGameObjectMain* v18; // [esp+60h] [ebp-5Ch]
    float v19; // [esp+64h] [ebp-58h]
    int v20; // [esp+70h] [ebp-4Ch]
    Vec3 a2; // [esp+74h] [ebp-48h] BYREF
    tGameObjectMain* v22; // [esp+80h] [ebp-3Ch]
    BOOL v23; // [esp+84h] [ebp-38h]
    int v24; // [esp+8Ch] [ebp-30h]
    int i; // [esp+90h] [ebp-2Ch]
    int v26; // [esp+94h] [ebp-28h]
    tScriptStruct_PNJRaskass* v27; // [esp+A4h] [ebp-18h]
    Vec3 v28; // [esp+A8h] [ebp-14h] BYREF
    tGameObjectMain* v29; // [esp+B4h] [ebp-8h]
    int v30; // [esp+B8h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJRaskass*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v27 = v2;
    v24 = 0;
    i = 0;
    v26 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Raskass_r_init_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(&Raskass_r_init_INV_manager_gao_i_Animals_Nb, (void*)Raskass_r_init_INV_manager_gao, 23888);
        AI2C_ResolveBuffer(&Raskass_r_init_INV_manager_gao_ao_Animals, (void*)Raskass_r_init_INV_manager_gao, 23368);
        AI2C_ResolveBuffer(&Raskass_r_init_INV_manager_gao_ai_AnimalsData, (void*)Raskass_r_init_INV_manager_gao, 23628);
        Raskass_r_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&Raskass_r_init_univVars_Flag_speciaux_sauve, (void*)Raskass_r_init_univVars, 4488);
        fct_Raskass_r_init_Raskass_r_attend_zone = (int)AI_ul_RealResolveOneRefFunction(0x3D000CD6u);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)12)
    {
        v1 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(256);
        v23 = v1;
        if (v1)
        {
            for (i = 0; i < *(int32_t*)Raskass_r_init_INV_manager_gao_i_Animals_Nb; ++i)
            {
                v29 = AI_gpst_CurrentGameObject;
                v13 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                        v13 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                    v12 = v13;
                }
                else
                {
                    v12 = 0;
                }
                if (*(tGameObjectMain**)(Raskass_r_init_INV_manager_gao_ao_Animals + 4 * i + 4) == v12)
                {
                    --* (uint32_t*)Raskass_r_init_INV_manager_gao_i_Animals_Nb;
                    *(uint32_t*)(Raskass_r_init_INV_manager_gao_ao_Animals + 4 * i + 4) = *(uint32_t*)(Raskass_r_init_INV_manager_gao_ao_Animals
                        + 4
                        * *(uint32_t*)Raskass_r_init_INV_manager_gao_i_Animals_Nb
                        + 4);
                    *(uint32_t*)(Raskass_r_init_INV_manager_gao_ai_AnimalsData + 4 * i + 4) = *(uint32_t*)(Raskass_r_init_INV_manager_gao_ai_AnimalsData
                        + 4
                        * *(uint32_t*)Raskass_r_init_INV_manager_gao_i_Animals_Nb
                        + 4);
                    break;
                }
            }
            v20 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v20 != 1000)
                return v20;
        }
        if (v27->des_i_mode == 1)
        {
            AI_EvalFunc_MATEnableMltxtLvl_C(AI_gpst_CurrentGameObject, 0x16003889u, 0, 0);
            AI_EvalFunc_MATEnableMltxtLvl_C(AI_gpst_CurrentGameObject, 0x16003889u, 1, 1);
        }
        else
        {
            v18 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0);
            v11 = v18;
            if (v18)
            {
                if ((unsigned int)v18 <= 2)
                    v11 = AI_MainActor[(uint32_t)v18 - 1];
                AI_EvalFunc_OBJ_FlagsIdentitySet_C(v11, 0x40, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagsIdentitySet_C(0, 0x40, 0);
            }
            v10 = v18;
            if (v18)
            {
                if ((unsigned int)v18 <= 2)
                    v10 = AI_MainActor[(uint32_t)v18 - 1];
                v9 = v10;
            }
            else
            {
                v9 = 0;
            }
            v9->extended->fields->designInt3 = 0x400001F;
            v8 = v18;
            if (v18)
            {
                if ((unsigned int)v18 <= 2)
                    v8 = AI_MainActor[(uint32_t)v18 - 1];
                v7 = v8;
            }
            else
            {
                v7 = 0;
            }
            v7->extended->fields->designFlags |= 0x40u;
            v6 = v18;
            if (v18)
            {
                if ((unsigned int)v18 <= 2)
                    v6 = AI_MainActor[(uint32_t)v18 - 1];
                v5 = v6;
            }
            else
            {
                v5 = 0;
            }
            v5->extended->fields->designFloat1 = 0.0;
            v4 = v18;
            if (v18)
            {
                if ((unsigned int)v18 <= 2)
                    v4 = AI_MainActor[(uint32_t)v18 - 1];
                v3 = v4;
            }
            else
            {
                v3 = 0;
            }
            v3->extended->fields->designFloat2 = 1.0;
            AI_EvalFunc_MATEnableMltxtLvl_C(AI_gpst_CurrentGameObject, 0x16003889u, 0, 1);
            AI_EvalFunc_MATEnableMltxtLvl_C(AI_gpst_CurrentGameObject, 0x16003889u, 1, 0);
            AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, 0.80000001f);
        }
        if (v27->des_i_mode != 1)
        {
            v19 = v27->des_f_hauteur * 3.0f;
            v28.x = -v19;
            v28.y = v28.x;
            v28.z = v28.x;
            AI_EvalFunc_BVMinSet_C(AI_gpst_CurrentGameObject, &v28);
            a2.x = v19;
            a2.y = v19;
            a2.z = v19;
            AI_EvalFunc_BVMaxSet_C(AI_gpst_CurrentGameObject, &a2);
        }
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        v27->i_etat = 0;
        v27->i_can_jump = 1;
        v27->f_time_alea_etat = AI_EvalFunc_MATHRandFloat_C(1.0, 3.0);
        v27->f_time_alea_init = AI_EvalFunc_MATHRandFloat_C(0.0, 100.0);
        if (v27->des_i_mode != 1)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)21;
            TIME_Wait_C(5.0);
            return 0;
        }
        goto LABEL_83;
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)21)
    {
    LABEL_83:
        v30 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Raskass_r_init_Raskass_r_attend_zone);
        if (v30 == 1000)
            result = 1000;
        else
            result = v30;
        return result;
    }
    if (v27->des_i_animal == -1)
        v24 = v27->des_i_mode + 31;
    else
        v24 = v27->des_i_animal;
    v26 = 0;
    if (*(int*)Raskass_r_init_INV_manager_gao_i_Animals_Nb < 64)
    {
        for (i = 0; i < *(int32_t*)Raskass_r_init_INV_manager_gao_i_Animals_Nb; ++i)
        {
            v22 = AI_gpst_CurrentGameObject;
            v17 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v17 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v16 = v17;
            }
            else
            {
                v16 = 0;
            }
            if (*(tGameObjectMain**)(Raskass_r_init_INV_manager_gao_ao_Animals + 4 * i + 4) == v16)
                break;
        }
        if (i == *(uint32_t*)Raskass_r_init_INV_manager_gao_i_Animals_Nb)
        {
            *(uint32_t*)(Raskass_r_init_INV_manager_gao_ao_Animals
                + 4 * *(uint32_t*)Raskass_r_init_INV_manager_gao_i_Animals_Nb
                + 4) = (uint32_t)AI_gpst_CurrentGameObject;
            *(uint32_t*)(Raskass_r_init_INV_manager_gao_ai_AnimalsData
                + 4 * (*(uint32_t*)Raskass_r_init_INV_manager_gao_i_Animals_Nb)++
                + 4) = v24 + (v27->des_i_etage << 8);
        }
    }
    if (v27->i_sound_nage != -1)
    {
        SND_Release(v27->i_sound_nage);
        v27->i_sound_nage = -1;
    }
    v24 = AI_EvalFunc_ARRBitTest_C((int*)(Raskass_r_init_univVars_Flag_speciaux_sauve + 4), 2718);
    if (v24)
    {
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 4;
    }
    else
    {
        OBJ_FreezeModifier_C(AI_gpst_CurrentGameObject, 20, 0);
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
        v15 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v15 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            v14 = v15;
        }
        else
        {
            v14 = 0;
        }
        v14->extended->fields->designFlags = 64;
        *AI_gppst_CurrentJumpNode = (tAINode*)12;
        TIME_Wait_C(0.5);
        result = 0;
    }
    return result;
}

int Raskass_r_saute()
{
    double v0; // st7
    float a4; // [esp+0h] [ebp-2DCh]
    float a4a; // [esp+0h] [ebp-2DCh]
    float a4b; // [esp+0h] [ebp-2DCh]
    float a4c; // [esp+0h] [ebp-2DCh]
    float a4d; // [esp+0h] [ebp-2DCh]
    float v7; // [esp+8h] [ebp-2D4h]
    tScriptStruct_PNJRaskass* v8; // [esp+Ch] [ebp-2D0h]
    Vec3* v9; // [esp+10h] [ebp-2CCh]
    double v10; // [esp+14h] [ebp-2C8h]
    Vec3* v11; // [esp+38h] [ebp-2A4h]
    Vec3* v12; // [esp+74h] [ebp-268h]
    float v13; // [esp+78h] [ebp-264h]
    tGameObjectMain* v14; // [esp+84h] [ebp-258h]
    float v15; // [esp+A0h] [ebp-23Ch]
    tGameObjectMain* v16; // [esp+B4h] [ebp-228h]
    tGameObjectMain* v17; // [esp+BCh] [ebp-220h]
    tGameObjectMain* v18; // [esp+C0h] [ebp-21Ch]
    tGameObjectMain* v19; // [esp+C4h] [ebp-218h]
    float v20; // [esp+D0h] [ebp-20Ch]
    float v21; // [esp+D4h] [ebp-208h]
    float v22; // [esp+D8h] [ebp-204h]
    float v23; // [esp+DCh] [ebp-200h]
    float v24; // [esp+E0h] [ebp-1FCh]
    float v25; // [esp+E4h] [ebp-1F8h]
    Vec3 v26; // [esp+E8h] [ebp-1F4h] BYREF
    Vec3 a5; // [esp+F4h] [ebp-1E8h] BYREF
    Vec3 v28; // [esp+100h] [ebp-1DCh] BYREF
    tGameObjectMain* v29; // [esp+10Ch] [ebp-1D0h]
    tGameObjectMain* v30; // [esp+110h] [ebp-1CCh]
    float v31; // [esp+114h] [ebp-1C8h]
    Vec3 v32; // [esp+118h] [ebp-1C4h] BYREF
    float v33; // [esp+124h] [ebp-1B8h]
    Vec3 v34; // [esp+128h] [ebp-1B4h] BYREF
    Vec3 v35; // [esp+134h] [ebp-1A8h] BYREF
    float v36; // [esp+144h] [ebp-198h]
    tMessage Src; // [esp+148h] [ebp-194h] BYREF
    float v38; // [esp+17Ch] [ebp-160h]
    Vec3 v39; // [esp+180h] [ebp-15Ch] BYREF
    float v40; // [esp+194h] [ebp-148h]
    Vec3 v41; // [esp+19Ch] [ebp-140h] BYREF
    Vec3 v42; // [esp+1A8h] [ebp-134h] BYREF
    Vec3 v43; // [esp+1B4h] [ebp-128h] BYREF
    float v44; // [esp+1C4h] [ebp-118h]
    float v45; // [esp+1C8h] [ebp-114h]
    int v46; // [esp+1CCh] [ebp-110h]
    Vec3 v47; // [esp+1D0h] [ebp-10Ch] BYREF
    Vec3 v48; // [esp+1DCh] [ebp-100h] BYREF
    Vec3 a3; // [esp+1F8h] [ebp-E4h] BYREF
    Vec3 v50; // [esp+204h] [ebp-D8h] BYREF
    Vec3 v51; // [esp+210h] [ebp-CCh] BYREF
    float v52; // [esp+21Ch] [ebp-C0h]
    Vec3 v53; // [esp+224h] [ebp-B8h] BYREF
    float v54; // [esp+230h] [ebp-ACh]
    float v55; // [esp+238h] [ebp-A4h]
    Vec3 v56; // [esp+23Ch] [ebp-A0h] BYREF
    Vec3 v57; // [esp+248h] [ebp-94h] BYREF
    Vec3 v58; // [esp+254h] [ebp-88h] BYREF
    Vec3 v59; // [esp+264h] [ebp-78h] BYREF
    int v60; // [esp+270h] [ebp-6Ch]
    float v61; // [esp+274h] [ebp-68h]
    float v62; // [esp+278h] [ebp-64h]
    int v63; // [esp+27Ch] [ebp-60h]
    tScriptStruct_PNJRaskass* v64; // [esp+284h] [ebp-58h]
    float v65; // [esp+288h] [ebp-54h]
    Vec3 v66; // [esp+28Ch] [ebp-50h] BYREF
    Vec3 dest; // [esp+298h] [ebp-44h] BYREF
    Vec3 v68; // [esp+2A4h] [ebp-38h] BYREF
    float v70; // [esp+2B8h] [ebp-24h]
    Vec3 v71; // [esp+2BCh] [ebp-20h] BYREF
    Vec3 v72; // [esp+2C8h] [ebp-14h]
    int v73; // [esp+2D8h] [ebp-4h]

    v39.x = 1.0;
    v39.y = 0.0;
    v39.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    v42.x = 1.0;
    v42.y = 0.0;
    v42.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v8 = (tScriptStruct_PNJRaskass*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v8 = 0;
    v64 = v8;
    v29 = 0;
    v40 = 0.0;
    v38 = 0.0;
    v55 = 0.0;
    v70 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Raskass_r_saute_Raskass_r_plouf = (int)AI_ul_RealResolveOneRefFunction(0x3D000CE8u);
        fct_Raskass_r_saute_Raskass_r_move = (int)AI_ul_RealResolveOneRefFunction(0x3D000CDBu);
    }
    if (!v64->i_nage_init)
    {
        v64->i_nage_init = 1;
        v25 = v64->v_speed_saut.x;
        if (v25 <= 15.0)
        {
            if (v25 >= -15.0)
                v24 = v64->v_speed_saut.x;
            else
                v24 = -15.0;
        }
        else
        {
            v24 = 15.0;
        }
        v64->v_speed_saut.x = v24;
        v23 = v64->v_speed_saut.y;
        if (v23 <= 15.0)
        {
            if (v23 >= -15.0)
                v22 = v64->v_speed_saut.y;
            else
                v22 = -15.0;
        }
        else
        {
            v22 = 15.0;
        }
        v64->v_speed_saut.y = v22;
        v21 = v64->v_speed_saut.z;
        if (v21 <= 80.0)
        {
            if (v21 >= 60.0)
                v20 = v64->v_speed_saut.z;
            else
                v20 = 60.0;
        }
        else
        {
            v20 = 80.0;
        }
        v64->v_speed_saut.z = v20;
        v30 = AI_gpst_CurrentGameObject;
    }
    if (v64->i_sound_nage != -1)
    {
        SND_Release(v64->i_sound_nage);
        v64->i_sound_nage = -1;
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v28);
    v39 = v28;
    if (v28.z > 2.0)
        AI_EvalFunc_OBJ_InfoPhotoFlagsSet_C(AI_gpst_CurrentGameObject, 0, 1);
    v29 = (tGameObjectMain*)v64->des_o_followed;
    v19 = v29;
    if (v29)
    {
        if ((unsigned int)v29 <= 2)
            v19 = AI_MainActor[(uint32_t)v29 - 1];
        v18 = v19;
    }
    else
    {
        v18 = 0;
    }
    if (!v18)
        v29 = *AI_MainActor;
    v17 = v29;
    if (v29)
    {
        if ((unsigned int)v29 <= 2)
            v17 = AI_MainActor[(uint32_t)v29 - 1];
        v0 = AI_EvalFunc_DYNSpeedGet_C(v17);
    }
    else
    {
        v0 = AI_EvalFunc_DYNSpeedGet_C(0);
    }
    v70 = (float)v0;
    v16 = v29;
    if (v29)
    {
        if ((unsigned int)v29 <= 2)
            v16 = AI_MainActor[(uint32_t)v29 - 1];
        AI_EvalFunc_OBJGetPos_C(v16, &v34);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v34);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v53);
    MATH_SubstractVec(&dest, &v34, &v53);
    a3 = dest;
    v40 = MATH_VecLen(&a3);
    if (v40 == 0.0)
    {
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v51);
        a3 = v51;
    }
    else
    {
        v15 = 1.0f / v40;
        a3.x = a3.x * v15;
        a3.y = a3.y * v15;
        a3.z = a3.z * v15;
    }
    if (v64->i_can_send_paf && v40 < 9.0)
    {
        v64->i_can_send_paf = 0;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v32);
        v39 = v32;
        AI_EvalFunc_MATHVecSetHorzNormalize_C(&v39);
        Src.msgID = 37;
        Src.gao = AI_gpst_CurrentGameObject;
        Src.gao2 = 0;
        Src.Int1 = 5;
        Src.Int2 = 0;
        Src.v1 = v39;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&Src.v2);
        Src.f2 = 5.0;
        Src.f1 = 1000.0;
        v14 = v29;
        if (v29)
        {
            if ((unsigned int)v29 <= 2)
                v14 = AI_MainActor[(uint32_t)v29 - 1];
            AI_EvalFunc_MSGSend_C(v14, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
    }
    v38 = MATH_VecLen(&v64->v_speed_saut);
    v12 = &v64->v_speed_saut;
    v13 = 1.0f / v38;
    v64->v_speed_saut.x = v13 * v64->v_speed_saut.x;
    v12->y = v13 * v12->y;
    v12->z = v13 * v12->z;
    v38 = v64->f_friction * v38;
    if (v64->i_can_send_paf)
    {
        if (v64->f_lime_va_vite >= (double)v70)
        {
            v45 = TIM_gf_dt;
            a4a = 4.0f * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlendRotate_C(&v64->v_speed_saut, &a3, a4a, &v57);
            v64->v_speed_saut = v57;
        }
        else
        {
            v44 = TIM_gf_dt;
            a4 = 0.40000001f * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlendRotate_C(&v64->v_speed_saut, &a3, a4, &a5);
            v64->v_speed_saut = a5;
        }
    }
    if (v64->i_can_jump_cote)
    {
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v50);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v71);
        v54 = TIM_gf_dt;
        a4b = 2.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecRotate_C(&v50, &v71, a4b, &v43);
        v39 = v43;
    }
    else
    {
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v41);
        v58.x = 0.0;
        v58.y = 0.0;
        v58.z = 1.0;
        v31 = TIM_gf_dt;
        a4c = 2.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlend_C(&v41, &v58, a4c, &v66);
        v39 = v66;
    }
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v47);
    v65 = TIM_gf_dt;
    a4d = 5.0f * TIM_gf_dt;
    AI_EvalFunc_MATHVecBlend_C(&v47, &v64->v_speed_saut, a4d, &v48);
    v42 = v48;
    AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v42, &v39);
    v39 = v64->v_speed_saut;
    AI_EvalFunc_MATHVecSetHorzNormalize_C(&v39);
    v52 = TIM_gf_dt;
    AI_EvalFunc_MATHVecBlendRotate_C(&v64->v_speed_saut, &v39, TIM_gf_dt, &v68);
    v64->v_speed_saut = v68;
    v11 = &v64->v_speed_saut;
    v64->v_speed_saut.x = v38 * v64->v_speed_saut.x;
    v11->y = v38 * v11->y;
    v11->z = v38 * v11->z;
    v36 = TIM_gf_dt;
    v72.x = TIM_gf_dt * v64->v_speed_saut.x;
    v72.y = v64->v_speed_saut.y * TIM_gf_dt;
    v72.z = v64->v_speed_saut.z * TIM_gf_dt;
    v39 = v72;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v56);
    MATH_VecAddVec(&v26, &v56, &v39);
    AI_EvalFunc_OBJSetPos_C_CURRENT(&v26);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v35);
    v39 = v35;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v59);
    v42 = v59;
    if (v64->f_max_z == 0.0 && v42.z < 0.0)
        v64->f_max_z = v39.z - v64->f_water_Z;
    if (v64->f_water_Z <= (double)v39.z || v42.z >= 0.0)
        goto LABEL_73;
    if (AI_EvalFunc_MATHRandInt_C(0, 2))
    {
        v64->i_can_jump = 0;
    }
    else
    {
        v64->i_can_jump = 1;
        v7 = (float)v64->f_vitesse;
        v64->v_offset_pos.z = -(v7 * 0.5f);
    }
    v64->i_etat = AI_EvalFunc_MATHRandInt_C(0, 2);
    v10 = v38 + MATH_gd_magicDoubleForRound;
    v64->f_vitesse = (*(uint32_t *)&v10);
    v64->f_time_jump = TIM_gf_MainClock - TIM_gf_LockTime;
    v64->f_time_change_etat = TIM_gf_MainClock - TIM_gf_LockTime;
    v64->f_time_inside_etat = TIM_gf_MainClock - TIM_gf_LockTime;
    v46 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(AI_gpst_CurrentGameObject, 4, -1.0, 0.94999999f, 1.05f);
    v73 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Raskass_r_saute_Raskass_r_plouf);
    if (v73 != 1000)
        return v73;
    v63 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Raskass_r_saute_Raskass_r_move);
    if (v63 != 1000)
        return v63;
LABEL_73:
    v55 = v64->f_gravity * 0.60000002f;
    v55 = v64->des_f_factor_gravity * v55;
    v33 = TIM_gf_dt;
    v55 = TIM_gf_dt * 20.0f * v55;
    v60 = 0;
    v61 = 0.0;
    v62 = -v55;
    v9 = &v64->v_speed_saut;
    v64->v_speed_saut.x = v64->v_speed_saut.x + 0.0f;
    v9->y = v61 + v9->y;
    v9->z = v62 + v9->z;
    return 1000;
}

int Raskass_r_reflexe()
{
    int result; // eax
    float a4; // [esp+0h] [ebp-160h]
    float a4a; // [esp+0h] [ebp-160h]
    float a4b; // [esp+0h] [ebp-160h]
    float X; // [esp+4h] [ebp-15Ch]
    float v5; // [esp+8h] [ebp-158h]
    tScriptStruct_PNJRaskass* v6; // [esp+Ch] [ebp-154h]
    float v7; // [esp+10h] [ebp-150h]
    float v8; // [esp+1Ch] [ebp-144h]
    float v9; // [esp+58h] [ebp-108h]
    Vec3 v10; // [esp+5Ch] [ebp-104h] BYREF
    float v11; // [esp+68h] [ebp-F8h]
    Vec3 v12; // [esp+6Ch] [ebp-F4h] BYREF
    Vec3 v13; // [esp+78h] [ebp-E8h] BYREF
    float v14; // [esp+84h] [ebp-DCh]
    Vec3 a1; // [esp+88h] [ebp-D8h] BYREF
    Vec3 v16; // [esp+94h] [ebp-CCh] BYREF
    Vec3 v17; // [esp+A0h] [ebp-C0h] BYREF
    int v18; // [esp+B0h] [ebp-B0h]
    float v19; // [esp+B4h] [ebp-ACh]
    Vec3 v20; // [esp+B8h] [ebp-A8h] BYREF
    Vec3 v21; // [esp+C4h] [ebp-9Ch] BYREF
    Vec3 v22; // [esp+D0h] [ebp-90h] BYREF
    Vec3 v23; // [esp+DCh] [ebp-84h] BYREF
    Vec3 v24; // [esp+E8h] [ebp-78h]
    Vec3 a3; // [esp+F4h] [ebp-6Ch] BYREF
    float v26; // [esp+100h] [ebp-60h]
    Vec3 a5; // [esp+104h] [ebp-5Ch] BYREF
    float v28; // [esp+110h] [ebp-50h]
    Vec3 v30; // [esp+118h] [ebp-48h] BYREF
    float v31; // [esp+124h] [ebp-3Ch]
    Vec3 dest; // [esp+128h] [ebp-38h] BYREF
    tScriptStruct_PNJRaskass* v33; // [esp+134h] [ebp-2Ch]
    Vec3 v34; // [esp+138h] [ebp-28h] BYREF
    Vec3 a2; // [esp+144h] [ebp-1Ch] BYREF
    Vec3 v36; // [esp+150h] [ebp-10h] BYREF
    float v37; // [esp+15Ch] [ebp-4h]

    v30.x = 1.0;
    v30.y = 0.0;
    v30.z = 0.0;
    v17.x = 1.0;
    v17.y = 0.0;
    v17.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_PNJRaskass*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v33 = v6;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Raskass_r_reflexe_Raskass_r_trace = (int)AI_ul_RealResolveOneRefFunction(0x3D000CE1u);
    }
    if (!v33->i_dans_la_place)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    if (v33->i_etat <= 3)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
        MATH_SubstractVec(&dest, &v33->v_pos_to_go, &a1);
        if (AI2C_VecEqNull(&dest))
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v12);
        MATH_SubstractVec(&v10, &v33->v_pos_to_go, &v12);
        AI_EvalFunc_MATHVecNormalize_C(&v10, &v22);
        a3 = v22;
        v30 = v22;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
        v14 = TIM_gf_dt;
        a4 = v33->f_inertie_rot * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
        a3 = a5;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v34);
        v33->f_factor_sinus = (v34.z * v30.z + v34.y * v30.y + v30.x * v34.x) * v33->f_factor_sinus;
        v26 = TIM_gf_MainClock - TIM_gf_LockTime;
        v9 = v33->f_time_alea_init + v26;
        v13.x = 0.0;
        v13.y = 0.0;
        v13.z = 1.0;
        X = v33->f_inertie_sinus * v9;
        v37 = sinf(X);
        a4a = v33->f_factor_sinus * v37;
        AI_EvalFunc_MATHVecRotate_C(&a3, &v13, a4a, &v23);
        a3 = v23;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v16);
        v21.x = 0.0;
        v21.y = 0.0;
        v21.z = 1.0;
        v11 = TIM_gf_dt;
        a4b = 4.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&v16, &v21, a4b, &v20);
        v30 = v20;
        AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a3, &v30);
        v17 = v30;
        v31 = TIM_gf_dt;
        v5 = (float)v33->f_vitesse;
        v8 = v5 * TIM_gf_dt;
        v24.x = a3.x * v8;
        v24.y = a3.y * v8;
        v24.z = a3.z * v8;
        v30 = v24;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v36);
        v30.x = v30.x + v36.x;
        v30.y = v30.y + v36.y;
        v30.z = v30.z + v36.z;
        if (v33->f_water_Z < (double)v30.z)
        {
            if (v33->i_etat == 3)
                v33->f_time_change_etat = 0.0;
            v28 = TIM_gf_dt;
            v7 = 4.0f * TIM_gf_dt;
            if (v7 < 0.0)
                v7 = 0.0;
            if (v7 > 1.0)
                v7 = 1.0;
            v19 = 0.0f * v7 + (1.0f - v7) * a3.z;
            a3.z = v19;
            AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a3, &v17);
            v30.z = v33->f_water_Z;
        }
        AI_EvalFunc_OBJSetPos_C_CURRENT(&v30);
    }
    v18 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Raskass_r_reflexe_Raskass_r_trace);
    if (v18 == 1000)
        result = 1000;
    else
        result = v18;
    return result;
}

int Raskass_r_tracepos()
{
    tScriptStruct_PNJRaskass* v1; // [esp+0h] [ebp-34h]
    Vec3* v2; // [esp+4h] [ebp-30h]
    float v3; // [esp+8h] [ebp-2Ch]
    float v4; // [esp+18h] [ebp-1Ch]
    float v5; // [esp+1Ch] [ebp-18h]
    float v6; // [esp+20h] [ebp-14h]
    Vec3 a1; // [esp+24h] [ebp-10h] BYREF
    tScriptStruct_PNJRaskass* v8; // [esp+30h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJRaskass*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v8 = v1;
    v3 = v1->des_f_dist_aileron - (v1->des_f_hauteur + v1->WaterZDelta) * 0.5f;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v1->v_pos_trace);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a1);
    v4 = a1.x * v3;
    v5 = a1.y * v3;
    v6 = a1.z * v3;
    v2 = &v8->v_pos_trace;
    v8->v_pos_trace.x = v8->v_pos_trace.x - v4;
    v2->y = v2->y - v5;
    v2->z = v2->z - v6;
    return 1000;
}

int Raskass_r_plouf()
{
    int result; // eax
    tScriptStruct_PNJRaskass* v1; // [esp+0h] [ebp-7Ch]
    tGameObjectMain* v2; // [esp+8h] [ebp-74h]
    tMessage Src; // [esp+14h] [ebp-68h] BYREF
    Vec3 pos; // [esp+48h] [ebp-34h] BYREF
    int v5; // [esp+54h] [ebp-28h]
    Vec3 v6; // [esp+58h] [ebp-24h] BYREF
    tGameObjectMain* v7; // [esp+70h] [ebp-Ch]
    tScriptStruct_PNJRaskass* v8; // [esp+78h] [ebp-4h]

    pos.x = 1.0;
    pos.y = 0.0;
    pos.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJRaskass*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v8 = v1;
    v7 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Raskass_r_plouf_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
        Raskass_r_plouf_Raskass_r_ripple = (int)AI_ul_RealResolveOneRefFunction(0x3D000CEAu);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v6);
    pos = v6;
    pos.z = v8->f_water_Z;
    v7 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)Raskass_r_plouf_SFX_explosion_gao, &pos);
    Src.msgID = 1;
    Src.Int1 = 38;
    Src.f2 = 4.0;
    Src.f1 = 0.0;
    Src.v1.x = 0.0;
    Src.v1.y = 0.0;
    Src.v1.z = 0.0;
    v2 = v7;
    if (v7)
    {
        if ((unsigned int)v7 <= 2)
            v2 = AI_MainActor[(uint32_t)v7 - 1];
        AI_EvalFunc_MSGSend_C(v2, &Src);
    }
    else
    {
        AI_EvalFunc_MSGSend_C(0, &Src);
    }
    v5 = AI_EvalFunc_AIExecute_C((tAIFunction*)Raskass_r_plouf_Raskass_r_ripple);
    if (v5 == 1000)
        result = 1000;
    else
        result = v5;
    return result;
}

int Raskass_r_exec_testgens()
{
    tScriptStruct_PNJRaskass* v1; // [esp+0h] [ebp-C8h]
    tGameObjectMain* v2; // [esp+14h] [ebp-B4h]
    tGameObjectMain* v3; // [esp+1Ch] [ebp-ACh]
    tGameObjectMain* v4; // [esp+30h] [ebp-98h]
    Vec3 dest; // [esp+40h] [ebp-88h] BYREF
    Vec3 v6; // [esp+4Ch] [ebp-7Ch] BYREF
    float v7; // [esp+58h] [ebp-70h]
    tGameObjectMain* v8; // [esp+5Ch] [ebp-6Ch]
    Vec3 v9; // [esp+60h] [ebp-68h] BYREF
    Vec3 v10; // [esp+6Ch] [ebp-5Ch] BYREF
    Vec3 a1; // [esp+78h] [ebp-50h] BYREF
    Vec3 v12; // [esp+88h] [ebp-40h] BYREF
    Vec3 v13; // [esp+94h] [ebp-34h] BYREF
    Vec3 a3; // [esp+A0h] [ebp-28h] BYREF
    tScriptStruct_PNJRaskass* v15; // [esp+ACh] [ebp-1Ch]
    float v16; // [esp+B0h] [ebp-18h]
    float v17; // [esp+B4h] [ebp-14h]
    Vec3 v18; // [esp+B8h] [ebp-10h] BYREF

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    v6.x = 1.0;
    v6.y = 0.0;
    v6.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJRaskass*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v15 = v1;
    v7 = 0.0;
    v8 = 0;
    v16 = 0.0;
    v17 = 0.0;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v10);
    v6 = v10;
    v4 = (tGameObjectMain*)v15->o_test_gen;
    if (v4)
    {
        if ((unsigned int)v4 <= 2)
            v4 = AI_MainActor[(uint32_t)v4 - 1];
        AI_EvalFunc_OBJGetPos_C(v4, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v12);
    MATH_SubstractVec(&dest, &a3, &v12);
    a1.x = dest.x;
    a1.y = dest.y;
    a1.z = 0.0;
    v16 = MATH_VecLen(&a1);
    v8 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(7u);
    v3 = (tGameObjectMain*)v15->o_test_gen;
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        AI_EvalFunc_OBJGetPos_C(v3, &v18);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v18);
    }
    v2 = v8;
    if (v8)
    {
        if ((unsigned int)v8 <= 2)
            v2 = AI_MainActor[(uint32_t)v8 - 1];
        AI_EvalFunc_OBJGetPos_C(v2, &v9);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v9);
    }
    MATH_SubstractVec(&v13, &v18, &v9);
    a1.x = v13.x;
    a1.y = v13.y;
    a1.z = 0.0;
    v17 = MATH_VecLen(&a1);
    if (v17 < (double)v16)
        v16 = v17;
    v7 = v15->des_f_hauteur * 4.0f;
    v17 = v7 / (v7 + v7);
    v16 = v16 * v17 - v7;
    if (v16 > 0.0)
        v16 = 0.0;
    if (v6.z > (double)v16)
    {
        v6.z = v16;
        AI_EvalFunc_OBJSetPos_C_CURRENT(&v6);
    }
    return 1000;
}