#include "ai_compiledscripts_inc.h"

//vaisseau_hyllis_flic_vhf_move
int fct_vaisseau_hyllis_flic_vhf_move_vaisseau_hyllis_flic_vhf_part;

//vaisseau_hyllis_flic_vhf_part
int fct_vaisseau_hyllis_flic_vhf_part_vaisseau_hyllis_flic_vhf_init;

//vaisseau_hyllis_flic_vhf_reflexe
int fctvar_vaisseau_hyllis_flic_vhf_reflexe_laser;
int vaisseau_hyllis_flic_vhf_reflexe_univVars;
int vaisseau_hyllis_flic_vhf_reflexe_univVars_f_sally_fric;
int vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao;
int vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao_i_Fric2Add;
int vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao_i_FricWithPerso;
int vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao_f_PogneAppearTimeInit;
int vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao_f_PogneAppearTimeFixed;
int vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao_f_PogneDelay;
int vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao_i_OpeningMode;
int vaisseau_hyllis_flic_vhf_reflexe_GST_Board_panel_gao;

//vaisseau_hyllis_flic_vhf_init
int vaisseau_hyllis_flic_vhf_init_univVars;
int vaisseau_hyllis_flic_vhf_init_univVars_i_num_amendes;
int fct_vaisseau_hyllis_flic_vhf_init_vaisseau_hyllis_flic_vhf_move;

int vaisseau_hyllis_flic_vhf_move()
{
    double v1; // st7
    float a4; // [esp+0h] [ebp-180h]
    tScriptStruct_VaisseauHyllisFlic* v3; // [esp+8h] [ebp-178h]
    float v4; // [esp+Ch] [ebp-174h]
    tGameObjectMain* v5; // [esp+30h] [ebp-150h]
    tGameObjectMain* v6; // [esp+40h] [ebp-140h]
    float v7; // [esp+4Ch] [ebp-134h]
    tGameObjectMain* v8; // [esp+54h] [ebp-12Ch]
    tGameObjectMain* v9; // [esp+5Ch] [ebp-124h]
    tGameObjectMain* v10; // [esp+6Ch] [ebp-114h]
    Vec3 v11; // [esp+70h] [ebp-110h] BYREF
    Vec3 v12; // [esp+7Ch] [ebp-104h] BYREF
    Vec3 v13; // [esp+88h] [ebp-F8h] BYREF
    tGameObjectMain* v14; // [esp+94h] [ebp-ECh]
    Vec3 v15; // [esp+9Ch] [ebp-E4h]
    Vec3 v16; // [esp+ACh] [ebp-D4h] BYREF
    Vec3 v17; // [esp+B8h] [ebp-C8h] BYREF
    Vec3 v19; // [esp+D0h] [ebp-B0h] BYREF
    float v20; // [esp+DCh] [ebp-A4h]
    float v21; // [esp+E0h] [ebp-A0h]
    Vec3 a1; // [esp+E4h] [ebp-9Ch] BYREF
    float v23; // [esp+F0h] [ebp-90h]
    float v24; // [esp+F4h] [ebp-8Ch]
    Vec3 a3; // [esp+F8h] [ebp-88h] BYREF
    Vec3 v26; // [esp+104h] [ebp-7Ch] BYREF
    float v27; // [esp+120h] [ebp-60h]
    float v28; // [esp+124h] [ebp-5Ch]
    float v29; // [esp+128h] [ebp-58h]
    Vec3 v30; // [esp+12Ch] [ebp-54h] BYREF
    Vec3 v31; // [esp+13Ch] [ebp-44h] BYREF
    Vec3 a5; // [esp+148h] [ebp-38h] BYREF
    int v33; // [esp+158h] [ebp-28h]
    tScriptStruct_VaisseauHyllisFlic* v34; // [esp+15Ch] [ebp-24h]
    Vec3 v35; // [esp+160h] [ebp-20h]
    Vec3 dest; // [esp+170h] [ebp-10h] BYREF

    v31.x = 1.0;
    v31.y = 0.0;
    v31.z = 0.0;
    v17.x = 1.0;
    v17.y = 0.0;
    v17.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    v15.x = 1.0;
    v15.y = 0.0;
    v15.z = 0.0;
    v19.x = 1.0;
    v19.y = 0.0;
    v19.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_VaisseauHyllisFlic*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v34 = v3;
    v14 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_vaisseau_hyllis_flic_vhf_move_vaisseau_hyllis_flic_vhf_part = (int)AI_ul_RealResolveOneRefFunction(0x3D001165u);
    }
    if (v34->i_je_me_barre)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v34->f_temps_part, 1.0))
        {
            v33 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_vaisseau_hyllis_flic_vhf_move_vaisseau_hyllis_flic_vhf_part);
            if (v33 != 1000)
                return v33;
        }
    }
    v14 = *AI_MainActor;
    v10 = v14;
    if (v14)
    {
        if ((unsigned int)v14 <= 2)
            v10 = AI_MainActor[(uint32_t)v14 - 1];
        AI_EvalFunc_OBJGetPos_C(v10, &v13);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v13);
    }
    v17.x = v13.x;
    v17.y = v13.y;
    v17.z = v13.z + 5.0f;
    v9 = v14;
    if (v14)
    {
        if ((unsigned int)v14 <= 2)
            v9 = AI_MainActor[(uint32_t)v14 - 1];
        AI_EvalFunc_OBJ_SightGet_C(v9, &v12);
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C(0, &v12);
    }
    v8 = v14;
    if (v14)
    {
        if ((unsigned int)v14 <= 2)
            v8 = AI_MainActor[(uint32_t)v14 - 1];
        v1 = AI_EvalFunc_DYNSpeedGet_C(v8);
    }
    else
    {
        v1 = AI_EvalFunc_DYNSpeedGet_C(0);
    }
    v24 = (float)v1;
    v7 = v24 * 1.2f;
    v35.x = v12.x * v7;
    v35.y = v12.y * v7;
    v35.z = v12.z * v7;
    v19 = v35;
    v6 = v14;
    if (v14)
    {
        if ((unsigned int)v14 <= 2)
            v6 = AI_MainActor[(uint32_t)v14 - 1];
        AI_EvalFunc_OBJ_SightGet_C(v6, &v16);
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C(0, &v16);
    }
    v27 = v16.x * 40.0f;
    v28 = v16.y * 40.0f;
    v29 = v16.z * 40.0f;
    v19.x = v19.x + v27;
    v19.y = v19.y + v28;
    v19.z = v19.z + v29;
    if (AI_EvalFunc_TIMEElapsed_C(v34->f_time_change_pos, 1.0))
    {
        v34->f_time_change_pos = TIM_gf_MainClock - TIM_gf_LockTime;
        if (++v34->i_etat_change == 4)
            v34->i_etat_change = 0;
        v34->v_pos_decal.x = 0.0;
        v34->v_pos_decal.y = 0.0;
        v34->v_pos_decal.z = 0.0;
        if (v34->i_etat_change)
        {
            switch (v34->i_etat_change)
            {
            case 1:
                v34->v_pos_decal.x = AI_EvalFunc_MATHRandFloat_C(0.0, 40.0);
                break;
            case 2:
                v34->v_pos_decal.z = AI_EvalFunc_MATHRandFloat_C(0.0, 20.0);
                break;
            case 3:
                v34->v_pos_decal.z = AI_EvalFunc_MATHRandFloat_C(0.0, -20.0);
                break;
            }
        }
        else
        {
            v34->v_pos_decal.x = AI_EvalFunc_MATHRandFloat_C(-40.0, 0.0);
        }
    }
    MATH_VecAddVec(&a1, &v17, &v19);
    a3 = a1;
    v5 = v14;
    if (v14)
    {
        if ((unsigned int)v14 <= 2)
            v5 = AI_MainActor[(uint32_t)v14 - 1];
        AI_EvalFunc_MATHVecLocalToGlobal_C(v5, &v34->v_pos_decal, &v11);
    }
    else
    {
        AI_EvalFunc_MATHVecLocalToGlobal_C(0, &v34->v_pos_decal, &v11);
    }
    a3.x = a3.x + v11.x;
    a3.y = a3.y + v11.y;
    a3.z = a3.z + v11.z;
    if (a3.z < 10.0)
        a3.z = 10.0;
    if (a3.z > 2000.0)
        a3.z = 2000.0;
    v23 = TIM_gf_dt;
    a4 = 4.0f * TIM_gf_dt;
    AI_EvalFunc_MATHVecBlendRotate_C(&v34->v_pos_togo, &a3, a4, &a5);
    v34->v_pos_togo = a5;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v26);
    MATH_SubstractVec(&dest, &v34->v_pos_togo, &v26);
    v31 = dest;
    AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v30);
    v15 = v30;
    v20 = MATH_VecLen(&v31);
    v21 = TIM_gf_dt;
    v4 = 10.0f * TIM_gf_dt;
    if (v4 < 0.0)
        v4 = 0.0;
    if (v4 > 1.0)
        v4 = 1.0;
    v34->f_speed = v20 * v4 + v34->f_speed * (1.0f - v4);
    AI_EvalFunc_MATHVecSetNormVector_C(&v31, v34->f_speed);
    AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v31);
    return 1000;
}

int vaisseau_hyllis_flic_vhf_part()
{
    int result; // eax
    float a4; // [esp+0h] [ebp-CCh]
    tScriptStruct_VaisseauHyllisFlic* v2; // [esp+8h] [ebp-C4h]
    float v3; // [esp+Ch] [ebp-C0h]
    Vec3 a5; // [esp+34h] [ebp-98h] BYREF
    Vec3 a3; // [esp+40h] [ebp-8Ch] BYREF
    float v6; // [esp+4Ch] [ebp-80h]
    BOOL v7; // [esp+50h] [ebp-7Ch]
    Vec3 a2; // [esp+54h] [ebp-78h] BYREF
    Vec3 v9; // [esp+60h] [ebp-6Ch] BYREF
    Vec3 a1; // [esp+70h] [ebp-5Ch] BYREF
    Vec3 v11; // [esp+7Ch] [ebp-50h] BYREF
    Vec3 v12; // [esp+88h] [ebp-44h]
    int v13; // [esp+94h] [ebp-38h]
    Vec3 v14; // [esp+98h] [ebp-34h] BYREF
    float v15; // [esp+A4h] [ebp-28h]
    Vec3 v16; // [esp+A8h] [ebp-24h] BYREF
    tScriptStruct_VaisseauHyllisFlic* v17; // [esp+B4h] [ebp-18h]
    Vec3 dest; // [esp+B8h] [ebp-14h] BYREF
    float v19; // [esp+C4h] [ebp-8h]
    int v20; // [esp+C8h] [ebp-4h]

    v12.x = 1.0;
    v12.y = 0.0;
    v12.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_VaisseauHyllisFlic*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v17 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_vaisseau_hyllis_flic_vhf_part_vaisseau_hyllis_flic_vhf_init = (int)AI_ul_RealResolveOneRefFunction(0x3D000FF8u);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v9);
    v16.x = 0.0;
    v16.y = 0.0;
    v16.z = 100.0;
    MATH_VecAddVec(&v14, &v9, &v16);
    a3 = v14;
    v6 = TIM_gf_dt;
    a4 = 4.0f * TIM_gf_dt;
    AI_EvalFunc_MATHVecBlendRotate_C(&v17->v_pos_togo, &a3, a4, &a5);
    v17->v_pos_togo = a5;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v11);
    MATH_SubstractVec(&dest, &v17->v_pos_togo, &v11);
    a1 = dest;
    AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&a2);
    v12 = a2;
    v15 = MATH_VecLen(&a1);
    v19 = TIM_gf_dt;
    v3 = 10.0f * TIM_gf_dt;
    if (v3 < 0.0)
        v3 = 0.0;
    if (v3 > 1.0)
        v3 = 1.0;
    v17->f_speed = v15 * v3 + v17->f_speed * (1.0f - v3);
    AI_EvalFunc_MATHVecSetNormVector_C(&a1, v17->f_speed);
    v13 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &a1);
    v7 = AI_EvalFunc_TIMEElapsed_C(v17->f_time_vu, 5.0);
    if (!v7
        || (v20 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_vaisseau_hyllis_flic_vhf_part_vaisseau_hyllis_flic_vhf_init),
            v20 == 1000))
    {
        result = 1000;
    }
    else
    {
        result = v20;
    }
    return result;
}

int vaisseau_hyllis_flic_vhf_reflexe()
{
    int result; // eax
    BOOL v1; // eax
    float a4; // [esp+0h] [ebp-27Ch]
    float a4a; // [esp+0h] [ebp-27Ch]
    float v4; // [esp+4h] [ebp-278h]
    float v5; // [esp+8h] [ebp-274h]
    float v6; // [esp+Ch] [ebp-270h]
    tScriptStruct_VaisseauHyllisFlic* v7; // [esp+10h] [ebp-26Ch]
    double v8; // [esp+14h] [ebp-268h]
    float v9; // [esp+20h] [ebp-25Ch]
    float v10; // [esp+24h] [ebp-258h]
    tGameObjectMain* v11; // [esp+2Ch] [ebp-250h]
    tGameObjectMain* v12; // [esp+4Ch] [ebp-230h]
    Vec3* v13; // [esp+6Ch] [ebp-210h]
    Vec3* v14; // [esp+70h] [ebp-20Ch]
    Vec3* v15; // [esp+74h] [ebp-208h]
    float v16; // [esp+7Ch] [ebp-200h]
    tGameObjectMain* v17; // [esp+A0h] [ebp-1DCh]
    tGameObjectMain* v18; // [esp+A4h] [ebp-1D8h]
    tGameObjectMain* v19; // [esp+ACh] [ebp-1D0h]
    tGameObjectMain* v20; // [esp+B0h] [ebp-1CCh]
    tGameObjectMain* v21; // [esp+B4h] [ebp-1C8h]
    float v22; // [esp+B8h] [ebp-1C4h]
    float v23; // [esp+C0h] [ebp-1BCh]
    Vec3 v24; // [esp+CCh] [ebp-1B0h] BYREF
    tMessage v25; // [esp+D8h] [ebp-1A4h] BYREF
    float v26; // [esp+110h] [ebp-16Ch]
    float v27; // [esp+114h] [ebp-168h]
    float v28; // [esp+118h] [ebp-164h]
    Vec3 v29; // [esp+11Ch] [ebp-160h] BYREF
    float v30; // [esp+128h] [ebp-154h]
    Vec3 a3; // [esp+130h] [ebp-14Ch] BYREF
    Vec3 a5; // [esp+13Ch] [ebp-140h] BYREF
    int v33; // [esp+148h] [ebp-134h]
    int v34; // [esp+14Ch] [ebp-130h]
    float v35; // [esp+150h] [ebp-12Ch]
    void* Src; // [esp+154h] [ebp-128h]
    Vec3 v37; // [esp+158h] [ebp-124h] BYREF
    Vec3 a1; // [esp+164h] [ebp-118h] BYREF
    Vec3 v39; // [esp+170h] [ebp-10Ch] BYREF
    tGameObjectMain* v40; // [esp+17Ch] [ebp-100h]
    float v41; // [esp+190h] [ebp-ECh]
    float v42; // [esp+194h] [ebp-E8h]
    Vec3 v43; // [esp+198h] [ebp-E4h] BYREF
    Vec3 v44; // [esp+1A4h] [ebp-D8h] BYREF
    Vec3 v45; // [esp+1B0h] [ebp-CCh] BYREF
    Vec3 v46; // [esp+1BCh] [ebp-C0h] BYREF
    float v47; // [esp+1C8h] [ebp-B4h]
    float v48; // [esp+1CCh] [ebp-B0h]
    float v49; // [esp+1D0h] [ebp-ACh]
    Vec3 v50; // [esp+1D4h] [ebp-A8h] BYREF
    float v51; // [esp+1E0h] [ebp-9Ch]
    float v52; // [esp+1E8h] [ebp-94h]
    Vec3 v53; // [esp+1ECh] [ebp-90h] BYREF
    Vec3 a2; // [esp+1F8h] [ebp-84h] BYREF
    Vec3 v55; // [esp+208h] [ebp-74h] BYREF
    Vec3 v56; // [esp+214h] [ebp-68h] BYREF
    float v58; // [esp+228h] [ebp-54h]
    BOOL v59; // [esp+230h] [ebp-4Ch]
    Vec3 v60; // [esp+234h] [ebp-48h] BYREF
    tScriptStruct_VaisseauHyllisFlic* v61; // [esp+240h] [ebp-3Ch]
    Vec3 dest; // [esp+244h] [ebp-38h] BYREF
    Vec3 v63; // [esp+250h] [ebp-2Ch] BYREF
    float v64; // [esp+264h] [ebp-18h]
    Vec3 v65; // [esp+268h] [ebp-14h] BYREF
    float v66; // [esp+278h] [ebp-4h]

    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    v46.x = 1.0;
    v46.y = 0.0;
    v46.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    v55.x = 1.0;
    v55.y = 0.0;
    v55.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_VaisseauHyllisFlic*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    v61 = v7;
    v40 = 0;
    v41 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fctvar_vaisseau_hyllis_flic_vhf_reflexe_laser = AI_ul_RealResolveOneRef(42, 0x720008BDu);
        vaisseau_hyllis_flic_vhf_reflexe_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &vaisseau_hyllis_flic_vhf_reflexe_univVars_f_sally_fric,
            (void*)vaisseau_hyllis_flic_vhf_reflexe_univVars,
            1200);
        vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao_i_Fric2Add,
            (void*)vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao,
            21476);
        AI2C_ResolveBuffer(
            &vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao_i_FricWithPerso,
            (void*)vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao,
            21484);
        AI2C_ResolveBuffer(
            &vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao_f_PogneAppearTimeInit,
            (void*)vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao,
            21504);
        AI2C_ResolveBuffer(
            &vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao_f_PogneAppearTimeFixed,
            (void*)vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao,
            21496);
        AI2C_ResolveBuffer(
            &vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao_f_PogneDelay,
            (void*)vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao,
            21500);
        AI2C_ResolveBuffer(
            &vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao_i_OpeningMode,
            (void*)vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao,
            100);
        vaisseau_hyllis_flic_vhf_reflexe_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
    }
    if (v61->i_can_move)
    {
        Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v25, Src, sizeof(v25));
        if (v25.msgID == 37)
        {
            v40 = v25.gao;
            v21 = v25.gao;
            if (v25.gao)
            {
                if (v25.gao <= (tGameObjectMain*)2)
                    v21 = AI_MainActor[(int)v25.gao - 1];
                v20 = v21;
            }
            else
            {
                v20 = 0;
            }
            if (v20)
            {
                v19 = v40;
                if (v40)
                {
                    if ((unsigned int)v40 <= 2)
                        v19 = AI_MainActor[(uint32_t)v40 - 1];
                    v1 = AI_EvalFunc_AIIsModel_C(v19, (tAIModel*)fctvar_vaisseau_hyllis_flic_vhf_reflexe_laser);
                }
                else
                {
                    v1 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_vaisseau_hyllis_flic_vhf_reflexe_laser);
                }
                v59 = v1;
                if (v1)
                {
                    v61->f_time_last_paf = TIM_gf_MainClock - TIM_gf_LockTime;
                    ++v61->i_nb_vu;
                    v61->i_je_me_barre = 0;
                    ++v61->i_numpaf;
                    ++v61->i_mode_attaque;
                }
            }
        }
        v40 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0);
        v18 = v40;
        if (v40)
        {
            if ((unsigned int)v40 <= 2)
                v18 = AI_MainActor[(uint32_t)v40 - 1];
            v17 = v18;
        }
        else
        {
            v17 = 0;
        }
        v4 = 15.0f * TIM_gf_dt;
        AI_EvalFunc_OBJRotateLocalZ_c(v17, v4);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v45);
        MATH_SubstractVec(&dest, &v61->v_pos_togo, &v45);
        AI_EvalFunc_MATHVecNormalize_C(&dest, &v29);
        a3 = v29;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
        v64 = TIM_gf_dt;
        a4 = 3.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
        a3 = a5;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v50);
        v55 = v50;
        a1 = v61->v_grav_pos;
        v16 = v61->des_f_air_friction;
        v26 = v16 * v61->v_grav_speed.x;
        v27 = v61->v_grav_speed.y * v16;
        v28 = v61->v_grav_speed.z * v16;
        v66 = TIM_gf_dt;
        v47 = v26 * TIM_gf_dt;
        v48 = v27 * TIM_gf_dt;
        v49 = v28 * TIM_gf_dt;
        v15 = &v61->v_grav_pos;
        v61->v_grav_pos.x = v47 + v61->v_grav_pos.x;
        v15->y = v48 + v15->y;
        v15->z = v49 + v15->z;
        v33 = 0;
        v34 = 0;
        v35 = v61->des_f_air_gravity;
        v42 = TIM_gf_dt;
        v22 = 0.0f * TIM_gf_dt;
        v23 = v35 * TIM_gf_dt;
        v14 = &v61->v_grav_pos;
        v61->v_grav_pos.x = v22 + v61->v_grav_pos.x;
        v14->y = v22 + v14->y;
        v14->z = v23 + v14->z;
        MATH_SubstractVec(&v61->v_grav_pos, &v61->v_grav_pos, &v55);
        AI_EvalFunc_MATHVecSetNormVector_C(&v61->v_grav_pos, v61->des_f_dist_centre_grav);
        v13 = &v61->v_grav_pos;
        v61->v_grav_pos.x = v55.x + v61->v_grav_pos.x;
        v13->y = v55.y + v13->y;
        v13->z = v55.z + v13->z;
        MATH_SubstractVec(&v56, &v61->v_grav_pos, &a1);
        v52 = TIM_gf_dt;
        MATH_VecMultNormScalar(&v65, &v56, TIM_gf_dt);
        v61->v_grav_speed = v65;
        MATH_SubstractVec(&v53, &v55, &v61->v_grav_pos);
        AI_EvalFunc_MATHVecNormalize_C(&v53, &v39);
        v46 = v39;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v37);
        v58 = TIM_gf_dt;
        a4a = 3.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&v37, &v46, a4a, &v24);
        v46 = v24;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a3, &v46);
        v40 = *AI_MainActor;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v43);
        v12 = v40;
        if (v40)
        {
            if ((unsigned int)v40 <= 2)
                v12 = AI_MainActor[(uint32_t)v40 - 1];
            AI_EvalFunc_OBJGetPos_C(v12, &v60);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v60);
        }
        MATH_SubstractVec(&v44, &v43, &v60);
        a1 = v44;
        v41 = MATH_VecLen(&a1);
        MATH_NormalizeEqualVector(&a1);
        if (v41 >= 100.0)
            goto LABEL_40;
        v11 = v40;
        if (v40)
        {
            if ((unsigned int)v40 <= 2)
                v11 = AI_MainActor[(uint32_t)v40 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v11, &v63);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &v63);
        }
        v30 = v63.z * a1.z + v63.y * a1.y + a1.x * v63.x;
        if (v30 <= 0.40000001)
        {
        LABEL_40:
            v61->i_devant = 0;
            v61->f_temps_devant = 0.0;
        }
        else
        {
            v61->i_devant = 1;
            v61->f_temps_devant = TIM_gf_dt + v61->f_temps_devant;
        }
        if (!v61->i_je_me_barre)
        {
            if (AI_EvalFunc_TIMEElapsed_C(v61->f_time_last_paf, 40.0))
            {
                v61->f_time_last_paf = TIM_gf_MainClock - TIM_gf_LockTime;
                v61->i_je_me_barre = 1;
                v61->f_temps_part = TIM_gf_MainClock - TIM_gf_LockTime;
                v61->f_time_vu = TIM_gf_MainClock - TIM_gf_LockTime;
            }
        }
        if (v61->i_devant && !v61->i_je_me_barre && v61->f_temps_devant > 3.0)
        {
            v10 = (float)*(int*)vaisseau_hyllis_flic_vhf_reflexe_univVars_f_sally_fric;
            if (v10 < 250.0)
                v6 = (float)*(int*)vaisseau_hyllis_flic_vhf_reflexe_univVars_f_sally_fric;
            else
                v6 = 250.0;
            v41 = v6;
            v9 = (float)*(int*)vaisseau_hyllis_flic_vhf_reflexe_univVars_f_sally_fric;
            if (v9 > 100.0)
                v5 = 100.0;
            else
                v5 = (float)*(int*)vaisseau_hyllis_flic_vhf_reflexe_univVars_f_sally_fric;
            v51 = v5;
            v8 = MATH_gd_magicDoubleForRound - v5;
            *(uint32_t*)vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao_i_Fric2Add = (*(uint32_t *)&v8);
            *(uint32_t*)vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao_i_FricWithPerso = 12;
            *(uint32_t*)vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao_f_PogneAppearTimeInit = 0x3F800000;
            *(uint32_t*)vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao_f_PogneAppearTimeFixed = 0x3E4CCCCD;
            *(uint32_t*)vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao_f_PogneDelay = 0x3E4CCCCD;
            *(uint32_t*)vaisseau_hyllis_flic_vhf_reflexe_INV_manager_gao_i_OpeningMode = 19;
            AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)vaisseau_hyllis_flic_vhf_reflexe_GST_Board_panel_gao, 0, 0x100);
            AI_EvalFunc_OBJ_CapaSet_C((tGameObjectMain*)vaisseau_hyllis_flic_vhf_reflexe_GST_Board_panel_gao, 0x1000, 0);
            v61->i_je_me_barre = 1;
            v61->f_temps_part = TIM_gf_MainClock - TIM_gf_LockTime;
            v61->f_time_vu = TIM_gf_MainClock - TIM_gf_LockTime;
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

int vaisseau_hyllis_flic_vhf_shoot()
{
    int result; // eax
    tGameObjectMain* v1; // eax
    tScriptStruct_VaisseauHyllisFlic* v2; // [esp+0h] [ebp-A0h]
    tAI* v3; // [esp+4h] [ebp-9Ch]
    tGameObjectMain* v4; // [esp+8h] [ebp-98h]
    tGameObjectMain* v5; // [esp+Ch] [ebp-94h]
    tAI* v6; // [esp+10h] [ebp-90h]
    tGameObjectMain* v7; // [esp+14h] [ebp-8Ch]
    tGameObjectMain* v8; // [esp+18h] [ebp-88h]
    tGameObjectMain* v9; // [esp+20h] [ebp-80h]
    tGameObjectMain* v10; // [esp+28h] [ebp-78h]
    tGameObjectMain* v11; // [esp+3Ch] [ebp-64h]
    float v12; // [esp+40h] [ebp-60h]
    Vec3 dest; // [esp+44h] [ebp-5Ch] BYREF
    tGameObjectMain* v14; // [esp+50h] [ebp-50h]
    Vec3 v15; // [esp+54h] [ebp-4Ch] BYREF
    Vec3 pos; // [esp+64h] [ebp-3Ch] BYREF
    Vec3 v17; // [esp+70h] [ebp-30h] BYREF
    Vec3 v18; // [esp+80h] [ebp-20h] BYREF
    Vec3 a3; // [esp+8Ch] [ebp-14h] BYREF
    tScriptStruct_VaisseauHyllisFlic* v20; // [esp+98h] [ebp-8h]

    v18.x = 1.0;
    v18.y = 0.0;
    v18.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_VaisseauHyllisFlic*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v20 = v2;
    v14 = 0;
    v14 = *AI_MainActor;
    v11 = v14;
    if (v14)
    {
        if ((unsigned int)v14 <= 2)
            v11 = AI_MainActor[(uint32_t)v14 - 1];
        AI_EvalFunc_OBJGetPos_C(v11, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v17);
    MATH_SubstractVec(&dest, &a3, &v17);
    v18 = dest;
    v12 = MATH_VecLen(&v18);
    if (v12 <= 300.0)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
        v10 = (tGameObjectMain*)v20->des_o_missile;
        if (v10)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            v1 = AI_EvalFunc_OBJDuplicate_C(v10, &pos);
        }
        else
        {
            v1 = AI_EvalFunc_OBJDuplicate_C(0, &pos);
        }
        v14 = v1;
        AI_EvalFunc_MATHVecSetNormVector_C(&v18, 10.0);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v15);
        v18.x = v18.x + v15.x;
        v18.y = v18.y + v15.y;
        v18.z = v18.z + v15.z;
        v9 = v14;
        if (v14)
        {
            if ((unsigned int)v14 <= 2)
                v9 = AI_MainActor[(uint32_t)v14 - 1];
            AI_EvalFunc_OBJSetPos_C(v9, &v18);
        }
        else
        {
            AI_EvalFunc_OBJSetPos_C(0, &v18);
        }
        if (v14 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
        {
            v6 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v8 = v14;
            if (v14)
            {
                if ((unsigned int)v14 <= 2)
                    v8 = AI_MainActor[(uint32_t)v14 - 1];
                v7 = v8;
            }
            else
            {
                v7 = 0;
            }
            v6 = v7->extended->AIInstance;
        }
        ((tScriptStruct_FlicTir*)(v6->instanceVarDataCurrent))->i_niveau_mechant = v20->i_mode_attaque;
        if (v14 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
        {
            v3 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v5 = v14;
            if (v14)
            {
                if ((unsigned int)v14 <= 2)
                    v5 = AI_MainActor[(uint32_t)v14 - 1];
                v4 = v5;
            }
            else
            {
                v4 = 0;
            }
            v3 = v4->extended->AIInstance;
        }
        ((tScriptStruct_FlicTir*)(v3->instanceVarDataCurrent))->o_owner = (int)AI_gpst_CurrentGameObject;
        result = 1000;
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int vaisseau_hyllis_flic_vhf_init()
{
    int result; // eax
    tScriptStruct_VaisseauHyllisFlic* v1; // [esp+4h] [ebp-A4h]
    tGameObjectMain* v2; // [esp+14h] [ebp-94h]
    float v3; // [esp+20h] [ebp-88h]
    Vec3 v4; // [esp+24h] [ebp-84h] BYREF
    Vec3 a1; // [esp+30h] [ebp-78h] BYREF
    Vec3 v6; // [esp+3Ch] [ebp-6Ch] BYREF
    Vec3 a3; // [esp+58h] [ebp-50h] BYREF
    BOOL v8; // [esp+64h] [ebp-44h]
    tGameObjectMain* v9; // [esp+68h] [ebp-40h]
    Vec3 v10; // [esp+6Ch] [ebp-3Ch] BYREF
    Vec3 v11; // [esp+7Ch] [ebp-2Ch] BYREF
    int v13; // [esp+8Ch] [ebp-1Ch]
    tScriptStruct_VaisseauHyllisFlic* v14; // [esp+90h] [ebp-18h]
    Vec3 dest; // [esp+94h] [ebp-14h] BYREF
    int v16; // [esp+A4h] [ebp-4h]

    v6.x = 1.0;
    v6.y = 0.0;
    v6.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_VaisseauHyllisFlic*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v14 = v1;
    v9 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        vaisseau_hyllis_flic_vhf_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &vaisseau_hyllis_flic_vhf_init_univVars_i_num_amendes,
            (void*)vaisseau_hyllis_flic_vhf_init_univVars,
            8508);
        fct_vaisseau_hyllis_flic_vhf_init_vaisseau_hyllis_flic_vhf_move = (int)AI_ul_RealResolveOneRefFunction(0x3D000FF9u);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)1)
        goto LABEL_11;
    if (*AI_gppst_CurrentJumpNode != (tAINode*)2)
    {
        AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, 0.2f);
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        v8 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
        v13 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x3029004, 0);
        v11.x = 0.0;
        v11.y = 0.0;
        v11.z = 0.0;
        AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v11);
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v4);
        v3 = v14->des_f_dist_centre_grav;
        v10.x = v4.x * v14->des_f_dist_centre_grav;
        v10.y = v4.y * v3;
        v10.z = v4.z * v3;
        MATH_SubstractVec(&dest, &a1, &v10);
        v14->v_grav_pos = dest;
        v14->v_grav_speed.x = 0.0;
        v14->v_grav_speed.y = 0.0;
        v14->v_grav_speed.z = 0.0;
        v14->i_can_move = 0;
        AI_EvalFunc_GRPInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
    LABEL_11:
        if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
        {
            ++* (uint32_t*)vaisseau_hyllis_flic_vhf_init_univVars_i_num_amendes;
            *AI_gppst_CurrentJumpNode = (tAINode*)2;
            TIME_Wait_C(1.0);
        }
        else
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)1;
        }
        return 0;
    }
    AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
    v9 = *AI_MainActor;
    v2 = v9;
    if (v9)
    {
        if ((unsigned int)v9 <= 2)
            v2 = AI_MainActor[(uint32_t)v9 - 1];
        AI_EvalFunc_OBJGetPos_C(v2, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    v6.x = a3.x;
    v6.y = a3.y;
    v6.z = a3.z + 300.0f;
    AI_EvalFunc_OBJSetPos_C_CURRENT(&v6);
    AI_EvalFunc_COLStartMatrixSet_C(AI_gpst_CurrentGameObject, &v6);
    v14->f_time_last_paf = TIM_gf_MainClock - TIM_gf_LockTime;
    v14->i_je_me_barre = 0;
    ++v14->i_nb_vu;
    v14->f_time_vu = TIM_gf_MainClock - TIM_gf_LockTime;
    v14->i_can_move = 1;
    v16 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_vaisseau_hyllis_flic_vhf_init_vaisseau_hyllis_flic_vhf_move);
    if (v16 == 1000)
        result = 1000;
    else
        result = v16;
    return result;
}