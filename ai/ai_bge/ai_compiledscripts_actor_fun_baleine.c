#include "ai_compiledscripts_inc.h"

//Fun_baleine_bal_move
int fct_Fun_baleine_bal_move_Fun_baleine_bal_plonge;
int fct_Fun_baleine_bal_move_Fun_baleine_bal_saute;

//Fun_baleine_bal_queue
int Fun_baleine_bal_queue_SFX_light_smoke_gao;

//Fun_baleine_bal_init
int Fun_baleine_bal_init_univVars;
int Fun_baleine_bal_init_univVars_Flag_speciaux_sauve;
int Fun_baleine_bal_init_INV_manager_gao;
int Fun_baleine_bal_init_INV_manager_gao_i_Animals_Nb;
int Fun_baleine_bal_init_INV_manager_gao_ao_Animals;
int Fun_baleine_bal_init_INV_manager_gao_ai_AnimalsData;
int fct_Fun_baleine_bal_init_Fun_baleine_bal_exec_snd;
int fct_Fun_baleine_bal_init_Fun_baleine_bal_move;

//Fun_baleine_bal_saute
int fct_Fun_baleine_bal_saute_Fun_baleine_bal_plouf;
int fct_Fun_baleine_bal_saute_Fun_baleine_bal_move;

//Fun_baleine_bal_plonge
int fct_Fun_baleine_bal_plonge_Fun_baleine_bal_plouf;
int fct_Fun_baleine_bal_plonge_Fun_baleine_bal_move;

//Fun_baleine_bal_afterrec
int fct_Fun_baleine_bal_afterrec_Fun_baleine_bal_queue;
int Fun_baleine_bal_afterrec_SFX_light_smoke_gao;

//Fun_baleine_bal_plouf
int Fun_baleine_bal_plouf_SFX_light_smoke_gao;

int Fun_baleine_bal_exec_snd()
{
    double v0; // st7
    tScriptStruct_PNJBaleine* v2; // [esp+8h] [ebp-24h]
    tGameObjectMain* v3; // [esp+10h] [ebp-1Ch]
    float v4; // [esp+24h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJBaleine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v3 = *AI_MainActor;
    if (*AI_MainActor)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v3);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v4 = (float)v0;
    if (v4 >= 640000.0)
    {
        if (v2->i_snd_loop != -1)
        {
            SND_Release(v2->i_snd_loop);
            v2->i_snd_loop = -1;
        }
    }
    else
    {
        if (v2->i_snd_loop == -1)
            v2->i_snd_loop = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 2, 0x18000000);
        if (AI_EvalFunc_TIMEElapsed_C(v2->f_time_ripple, 2.0))
            SND_StopRq(v2->i_snd_loop);
        else
            AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v2->i_snd_loop);
    }
    return 1000;
}

int Fun_baleine_bal_move()
{
    double v0; // st7
    int result; // eax
    tGameObjectMain* v2; // eax
    float v3; // [esp+0h] [ebp-23Ch]
    float v4; // [esp+0h] [ebp-23Ch]
    float v5; // [esp+0h] [ebp-23Ch]
    tScriptStruct_PNJBaleine* v6; // [esp+8h] [ebp-234h]
    float v7; // [esp+24h] [ebp-218h]
    float v8; // [esp+30h] [ebp-20Ch]
    tGameObjectMain* v9; // [esp+6Ch] [ebp-1D0h]
    tGameObjectMain* v10; // [esp+74h] [ebp-1C8h]
    tGameObjectMain* v11; // [esp+88h] [ebp-1B4h]
    tGameObjectMain* v12; // [esp+98h] [ebp-1A4h]
    Vec3 v13; // [esp+9Ch] [ebp-1A0h] BYREF
    float v14; // [esp+ACh] [ebp-190h]
    Vec3 v15; // [esp+B0h] [ebp-18Ch] BYREF
    Vec3 a3; // [esp+BCh] [ebp-180h] BYREF
    float v17; // [esp+C8h] [ebp-174h]
    float v18; // [esp+CCh] [ebp-170h]
    Vec3 v19; // [esp+DCh] [ebp-160h] BYREF
    Vec3 v20; // [esp+E8h] [ebp-154h] BYREF
    Vec3 v21; // [esp+F4h] [ebp-148h] BYREF
    Vec3 dest; // [esp+100h] [ebp-13Ch] BYREF
    Vec3 v24; // [esp+110h] [ebp-12Ch] BYREF
    Vec3 v25; // [esp+11Ch] [ebp-120h] BYREF
    tGameObjectMain* v26; // [esp+128h] [ebp-114h]
    Vec3 v27; // [esp+12Ch] [ebp-110h] BYREF
    Vec3 v28; // [esp+138h] [ebp-104h] BYREF
    Vec3 v29; // [esp+144h] [ebp-F8h] BYREF
    Vec3 v30; // [esp+150h] [ebp-ECh] BYREF
    float v31; // [esp+15Ch] [ebp-E0h]
    int v32; // [esp+160h] [ebp-DCh]
    Vec3 v34; // [esp+168h] [ebp-D4h] BYREF
    Vec3 v35; // [esp+174h] [ebp-C8h] BYREF
    float v36; // [esp+180h] [ebp-BCh]
    Vec3 a2; // [esp+184h] [ebp-B8h] BYREF
    Vec3 v38; // [esp+190h] [ebp-ACh] BYREF
    float v39; // [esp+1A0h] [ebp-9Ch]
    float v40; // [esp+1A4h] [ebp-98h]
    Vec3 v41; // [esp+1A8h] [ebp-94h] BYREF
    Vec3 a5; // [esp+1B4h] [ebp-88h] BYREF
    float v43; // [esp+1C0h] [ebp-7Ch]
    Vec3 v44; // [esp+1C4h] [ebp-78h] BYREF
    Vec3 v45; // [esp+1D0h] [ebp-6Ch] BYREF
    float v46; // [esp+1E0h] [ebp-5Ch]
    int v47; // [esp+1E4h] [ebp-58h]
    int v48; // [esp+1E8h] [ebp-54h]
    BOOL v49; // [esp+1ECh] [ebp-50h]
    float v50; // [esp+1F0h] [ebp-4Ch]
    Vec3 v51; // [esp+1F8h] [ebp-44h]
    tScriptStruct_PNJBaleine* v52; // [esp+204h] [ebp-38h]
    Vec3 v53; // [esp+208h] [ebp-34h] BYREF
    Vec3 a1; // [esp+214h] [ebp-28h] BYREF
    float v55; // [esp+220h] [ebp-1Ch]
    float a4; // [esp+22Ch] [ebp-10h]
    float X; // [esp+234h] [ebp-8h]

    v28.x = 1.0;
    v28.y = 0.0;
    v28.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_PNJBaleine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v52 = v6;
    v47 = 0;
    v18 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Fun_baleine_bal_move_Fun_baleine_bal_plonge = (int)AI_ul_RealResolveOneRefFunction(0x3D000D36u);
        fct_Fun_baleine_bal_move_Fun_baleine_bal_saute = (int)AI_ul_RealResolveOneRefFunction(0x3D000D38u);
    }
    v26 = *AI_MainActor;
    v12 = v26;
    if (v26)
    {
        if ((unsigned int)v26 <= 2)
            v12 = AI_MainActor[(uint32_t)v26 - 1];
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v12);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v50 = (float)v0;
    v18 = v50;
    if (v50 <= 16000000.0)
    {
        v52->i_count_plouf = 0;
        v52->i_etat = 0;
        v52->f_reqinertia = 3.0;
        v41.x = 0.0;
        v41.y = 0.0;
        v41.z = 0.0;
        v17 = TIM_gf_dt;
        v3 = 3.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlend_C(&v52->v_speed_saut, &v41, v3, &v13);
        v52->v_speed_saut = v13;
        v11 = (tGameObjectMain*)v52->o_curwp;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v11 = AI_MainActor[(uint32_t)v11 - 1];
            AI_EvalFunc_OBJGetPos_C(v11, &v34);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v34);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
        MATH_SubstractVec(&dest, &v34, &a1);
        a3.x = dest.x;
        a3.y = dest.y;
        a3.z = 0.0;
        v43 = MATH_VecLen(&a3);
        if (v43 < 50.0)
        {
            v10 = (tGameObjectMain*)v52->o_curwp;
            if (v10)
            {
                if ((unsigned int)v10 <= 2)
                    v10 = AI_MainActor[(uint32_t)v10 - 1];
                v2 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v52->des_o_network, v10, 1, 0);
            }
            else
            {
                v2 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v52->des_o_network, 0, 1, 0);
            }
            v52->o_curwp = (int)v2;
        }
        v9 = (tGameObjectMain*)v52->o_curwp;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            AI_EvalFunc_OBJGetPos_C(v9, &v24);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v24);
        }
        a3 = v24;
        a3.z = v52->z_pos;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v38);
        a3.x = a3.x - v38.x;
        a3.y = a3.y - v38.y;
        a3.z = a3.z - v38.z;
        MATH_NormalizeEqualVector(&a3);
        v19.x = 0.0;
        v19.y = 0.0;
        v19.z = 1.0;
        X = TIM_gf_MainClock - TIM_gf_LockTime;
        v40 = sinf(X);
        v4 = 0.5f * v40;
        AI_EvalFunc_MATHVecRotate_C(&a3, &v19, v4, &v25);
        a3 = v25;
        v30.x = 1.0;
        v30.y = 0.0;
        v30.z = 0.0;
        v14 = TIM_gf_MainClock - TIM_gf_LockTime;
        v31 = sinf(v14);
        v5 = 0.5f * v31;
        AI_EvalFunc_MATHVecRotate_C(&a3, &v30, v5, &v44);
        a3 = v44;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
        a4 = TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, TIM_gf_dt, &a5);
        a3 = a5;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v29);
        v27.x = 0.0;
        v27.y = 0.0;
        v27.z = 1.0;
        v46 = TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&v29, &v27, TIM_gf_dt, &v15);
        v28 = v15;
        AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a3, &v28);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v53);
        v36 = TIM_gf_dt;
        v8 = v52->f_speed * TIM_gf_dt;
        v51.x = v53.x * v8;
        v51.y = v53.y * v8;
        v51.z = v53.z * v8;
        a3 = v51;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v35);
        a3.x = a3.x + v35.x;
        a3.y = a3.y + v35.y;
        a3.z = a3.z + v35.z;
        if (a3.z > -5.0)
            a3.z = -5.0;
        AI_EvalFunc_OBJSetPos_C_CURRENT(&a3);
        v39 = TIM_gf_dt;
        v7 = TIM_gf_dt;
        if (TIM_gf_dt < 0.0)
            v7 = 0.0;
        if (v7 > 1.0)
            v7 = 1.0;
        v55 = 40.0f * v7 + v52->f_speed * (1.0f - v7);
        v52->f_speed = v55;
        if (AI_EvalFunc_TIMEElapsed_C(v52->f_time_etat, v52->f_alea_etat))
        {
            v52->f_alea_etat = AI_EvalFunc_MATHRandFloat_C(3.0, 5.0);
            v52->f_time_etat = TIM_gf_MainClock - TIM_gf_LockTime;
            v52->z_pos = AI_EvalFunc_MATHRandFloat_C(-10.0, -50.0);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v21);
        a3 = v21;
        if (v21.z > -25.0
            && (v49 = AI_EvalFunc_TIMEElapsed_C(v52->f_time_plonge, 15.0))
            && (v47 = AI_EvalFunc_MATHRandInt_C(0, 2)) != 0
            && (v52->f_time_etat = TIM_gf_MainClock - TIM_gf_LockTime,
                v52->i_can_queue = 1,
                v52->i_can_plonge = 1,
                v32 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Fun_baleine_bal_move_Fun_baleine_bal_plonge),
                v32 != 1000))
        {
            result = v32;
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v45);
            a3 = v45;
            if (v45.z >= -30.0)
                goto LABEL_49;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v20);
            a3 = v20;
            if (v20.z <= 0.2)
                goto LABEL_49;
            if (!AI_EvalFunc_TIMEElapsed_C(v52->f_time_saut, 15.0))
                goto LABEL_49;
            v47 = AI_EvalFunc_MATHRandInt_C(0, 2);
            if (v47
                && (v52->f_time_etat = TIM_gf_MainClock - TIM_gf_LockTime,
                    v52->i_can_queue = 1,
                    v52->v_speed_saut.x = 0.0,
                    v52->v_speed_saut.y = 0.0,
                    v52->v_speed_saut.z = 0.0,
                    v48 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Fun_baleine_bal_move_Fun_baleine_bal_saute),
                    v48 != 1000))
            {
                result = v48;
            }
            else
            {
            LABEL_49:
                result = 1000;
            }
        }
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int Fun_baleine_bal_queue()
{
    int result; // eax
    float v1; // [esp+0h] [ebp-50h]
    tScriptStruct_PNJBaleine* v2; // [esp+4h] [ebp-4Ch]
    tGameObjectMain* v3; // [esp+Ch] [ebp-44h]
    Vec3 v4; // [esp+18h] [ebp-38h] BYREF
    Vec3 v5; // [esp+24h] [ebp-2Ch] BYREF
    Vec3 v6; // [esp+30h] [ebp-20h] BYREF
    float v7; // [esp+40h] [ebp-10h]
    float v8; // [esp+44h] [ebp-Ch]
    tScriptStruct_PNJBaleine* v9; // [esp+48h] [ebp-8h]
    int a1; // [esp+4Ch] [ebp-4h]

    v6.x = 1.0;
    v6.y = 0.0;
    v6.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJBaleine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v9 = v2;
    a1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Fun_baleine_bal_queue_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    if (v9->i_can_queue)
    {
        v9->i_can_queue = 0;
        v6 = v9->v_queue;
        v6.z = v9->des_f_water_Z;
        a1 = AI_EvalFunc_GFXAdd_C_CURRENT(20);
        v3 = (tGameObjectMain*)Fun_baleine_bal_queue_SFX_light_smoke_gao;
        if (Fun_baleine_bal_queue_SFX_light_smoke_gao)
        {
            if ((unsigned int)Fun_baleine_bal_queue_SFX_light_smoke_gao <= 2)
                v3 = AI_MainActor[Fun_baleine_bal_queue_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, v3, 16);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, 0, 16);
        }
        v8 = AI_EvalFunc_MATHRandFloat_C(1.5, 2.0);
        GFXSetCurGlobal(a1);
        AI_EvalFunc_GFXSetf_C_CURGLOB(20000, v8);
        v7 = AI_EvalFunc_MATHRandFloat_C(100.0, 200.0);
        v1 = v7 + 7.0f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(20001, v1);
        AI_EvalFunc_GFXSetf_C_CURGLOB(20002, 0.30000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(20003, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(20004, 20.0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(20100, 1);
        AI_EvalFunc_GFXSeti_C_CURGLOB(20100, -2);
        AI_EvalFunc_GFXSeti_C_CURGLOB(20101, 0xA0A0A0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(20102, 0);
        AI_EvalFunc_GFXSetv_C_CURGLOB(20200, &v6);
        v5.x = 0.0;
        v5.y = 0.0;
        v5.z = 1.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(20201, &v5);
        v4.x = 0.0;
        v4.y = 1.0;
        v4.z = 0.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(20202, &v4);
        result = 1000;
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int Fun_baleine_bal_init()
{
    int result; // eax
    int v1; // eax
    tGameObjectMain* v2; // eax
    tScriptStruct_PNJBaleine* v3; // [esp+4h] [ebp-3Ch]
    Vec3 a2; // [esp+Ch] [ebp-34h] BYREF
    int v5; // [esp+1Ch] [ebp-24h]
    int v6; // [esp+2Ch] [ebp-14h]
    int v7; // [esp+30h] [ebp-10h]
    tScriptStruct_PNJBaleine* v8; // [esp+34h] [ebp-Ch]
    int v9; // [esp+38h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_PNJBaleine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v8 = v3;
    v6 = 0;
    v9 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Fun_baleine_bal_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&Fun_baleine_bal_init_univVars_Flag_speciaux_sauve, (void*)Fun_baleine_bal_init_univVars, 4488);
        Fun_baleine_bal_init_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &Fun_baleine_bal_init_INV_manager_gao_i_Animals_Nb,
            (void*)Fun_baleine_bal_init_INV_manager_gao,
            23888);
        AI2C_ResolveBuffer(
            &Fun_baleine_bal_init_INV_manager_gao_ao_Animals,
            (void*)Fun_baleine_bal_init_INV_manager_gao,
            23368);
        AI2C_ResolveBuffer(
            &Fun_baleine_bal_init_INV_manager_gao_ai_AnimalsData,
            (void*)Fun_baleine_bal_init_INV_manager_gao,
            23628);
        fct_Fun_baleine_bal_init_Fun_baleine_bal_exec_snd = (int)AI_ul_RealResolveOneRefFunction(0x87000D31);
        fct_Fun_baleine_bal_init_Fun_baleine_bal_move = (int)AI_ul_RealResolveOneRefFunction(0x3D000D2Du);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)3)
    {
        v1 = AI_EvalFunc_OBJ_InfoPhotoInfoGet_C(AI_gpst_CurrentGameObject);
        v9 = v1;
        if (v1 != -1 && *(int*)Fun_baleine_bal_init_INV_manager_gao_i_Animals_Nb < 64)
        {
            *(uint32_t*)(Fun_baleine_bal_init_INV_manager_gao_ao_Animals
                + 4 * *(uint32_t*)Fun_baleine_bal_init_INV_manager_gao_i_Animals_Nb
                + 4) = (uint32_t)AI_gpst_CurrentGameObject;
            *(uint32_t*)(Fun_baleine_bal_init_INV_manager_gao_ai_AnimalsData
                + 4 * (*(uint32_t*)Fun_baleine_bal_init_INV_manager_gao_i_Animals_Nb)++
                + 4) = v9;
        }
        AI_EvalFunc_SNAKE_SetInertia_C(AI_gpst_CurrentGameObject, 2.0);
        v8->f_inertia = 2.0;
        v8->f_reqinertia = 2.0;
        a2.x = 0.0;
        a2.y = 0.0;
        a2.z = 0.0;
        AI_EvalFunc_OBJ_InfoPhotoSphereOffsetSet_C(AI_gpst_CurrentGameObject, &a2);
        v8->i_can_queue = 1;
        v8->i_can_plonge = 1;
        v8->f_alea_etat = 4.0;
        v8->f_speed = 20.0;
        v2 = AI_EvalFunc_WAYNearestWPOfObj_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v8->des_o_network, -1, 0, 0);
        v8->o_curwp = (int)v2;
        v8->i_etat = 0;
        v8->f_time_etat = TIM_gf_MainClock - TIM_gf_LockTime;
        AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_Fun_baleine_bal_init_Fun_baleine_bal_exec_snd);
        v5 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Fun_baleine_bal_init_Fun_baleine_bal_move);
        if (v5 == 1000)
            result = 1000;
        else
            result = v5;
    }
    else if (AI_EvalFunc_ARRBitTest_C((int*)(Fun_baleine_bal_init_univVars_Flag_speciaux_sauve + 4), 126)
        || (v7 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject), v7 == 1000))
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)3;
        TIME_Wait_C(1.0);
        result = 0;
    }
    else
    {
        result = v7;
    }
    return result;
}

int Fun_baleine_bal_saute()
{
    float a4; // [esp+0h] [ebp-1CCh]
    tScriptStruct_PNJBaleine* v2; // [esp+8h] [ebp-1C4h]
    Vec3* v3; // [esp+Ch] [ebp-1C0h]
    float v4; // [esp+7Ch] [ebp-150h]
    Vec3 v5; // [esp+8Ch] [ebp-140h] BYREF
    Vec3 v6; // [esp+98h] [ebp-134h] BYREF
    float v7; // [esp+A4h] [ebp-128h]
    float v8; // [esp+A8h] [ebp-124h]
    Vec3 a2; // [esp+B0h] [ebp-11Ch] BYREF
    float v10; // [esp+BCh] [ebp-110h]
    Vec3 v11; // [esp+C0h] [ebp-10Ch] BYREF
    Vec3 v12; // [esp+CCh] [ebp-100h] BYREF
    Vec3 v13; // [esp+D8h] [ebp-F4h] BYREF
    Vec3 v14; // [esp+E4h] [ebp-E8h] BYREF
    Vec3 a3; // [esp+F0h] [ebp-DCh] BYREF
    Vec3 v16; // [esp+FCh] [ebp-D0h] BYREF
    Vec3 v17; // [esp+108h] [ebp-C4h] BYREF
    int v18; // [esp+114h] [ebp-B8h]
    Vec3 v19; // [esp+118h] [ebp-B4h] BYREF
    float v20; // [esp+124h] [ebp-A8h]
    int v21; // [esp+128h] [ebp-A4h]
    Vec3 a5; // [esp+12Ch] [ebp-A0h] BYREF
    int v23; // [esp+138h] [ebp-94h]
    float v24; // [esp+13Ch] [ebp-90h]
    float v25; // [esp+140h] [ebp-8Ch]
    Vec3 v26; // [esp+144h] [ebp-88h]
    Vec3 v27; // [esp+150h] [ebp-7Ch]
    float v28; // [esp+15Ch] [ebp-70h]
    Vec3 a1; // [esp+160h] [ebp-6Ch] BYREF
    Vec3 v30; // [esp+16Ch] [ebp-60h] BYREF
    float v31; // [esp+178h] [ebp-54h]
    Vec3 v32; // [esp+17Ch] [ebp-50h] BYREF
    Vec3 v33; // [esp+190h] [ebp-3Ch] BYREF
    tScriptStruct_PNJBaleine* v34; // [esp+19Ch] [ebp-30h]
    Vec3 v35; // [esp+1A0h] [ebp-2Ch] BYREF
    Vec3 v36; // [esp+1ACh] [ebp-20h] BYREF
    Vec3 v37; // [esp+1BCh] [ebp-10h] BYREF
    float v38; // [esp+1C8h] [ebp-4h]

    v26.x = 1.0;
    v26.y = 0.0;
    v26.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    v6.x = 1.0;
    v6.y = 0.0;
    v6.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJBaleine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v34 = v2;
    v7 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Fun_baleine_bal_saute_Fun_baleine_bal_plouf = (int)AI_ul_RealResolveOneRefFunction(0x3D000D42u);
        fct_Fun_baleine_bal_saute_Fun_baleine_bal_move = (int)AI_ul_RealResolveOneRefFunction(0x3D000D2Du);
    }
    v34->f_reqinertia = 5.0;
    v34->i_etat = 1;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v11);
    if (v11.z < 35.0)
        AI_EvalFunc_OBJ_InfoPhotoFlagsSet_C(AI_gpst_CurrentGameObject, 1, 0);
    else
        AI_EvalFunc_OBJ_InfoPhotoFlagsSet_C(AI_gpst_CurrentGameObject, 0, 1);
    v34->f_time_jet = TIM_gf_MainClock - TIM_gf_LockTime;
    v31 = TIM_gf_dt;
    v4 = TIM_gf_dt;
    if (TIM_gf_dt < 0.0)
        v4 = 0.0;
    if (v4 > 1.0)
        v4 = 1.0;
    v8 = 170.0f * v4 + v34->f_speed * (1.0f - v4);
    v34->f_speed = v8;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v5);
    v6 = v5;
    if (v5.z <= 5.0)
    {
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v35);
        v6 = v35;
    }
    else
    {
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v19);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v36);
        v10 = TIM_gf_dt;
        AI_EvalFunc_MATHVecRotate_C(&v19, &v36, TIM_gf_dt, &v30);
        v6 = v30;
    }
    if (v34->v_speed_saut.z == 0.0)
    {
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v12);
        v27.x = 0.2f;
        v27.y = 0.2f;
        v27.z = 1.0;
        a3.x = 0.2f;
        a3.y = 0.2f;
        a3.z = 1.0;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
        v38 = TIM_gf_dt;
        a4 = 3.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
        a3 = a5;
    }
    else
    {
        v34->f_reqinertia = 10.0;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v13);
        a3 = v13;
    }
    AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a3, &v6);
    if (v34->v_speed_saut.z < 0.0)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v17);
        v6 = v17;
        if (v17.z < 2.0)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v34->v_queue);
            v18 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Fun_baleine_bal_saute_Fun_baleine_bal_plouf);
            if (v18 != 1000)
                return v18;
        }
    }
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v32);
    v6 = v32;
    AI_EvalFunc_MATHVecSetNormVector_C(&v6, v34->f_speed);
    v6.x = v6.x + v34->v_speed_saut.x;
    v6.y = v6.y + v34->v_speed_saut.y;
    v6.z = v6.z + v34->v_speed_saut.z;
    v26 = v6;
    v28 = TIM_gf_dt;
    v6.x = v6.x * TIM_gf_dt;
    v6.y = v6.y * TIM_gf_dt;
    v6.z = v6.z * TIM_gf_dt;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v37);
    MATH_VecAddVec(&a1, &v37, &v6);
    AI_EvalFunc_OBJSetPos_C_CURRENT(&a1);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v16);
    v6 = v16;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v14);
    a3 = v14;
    if (v6.z < -100.0 && v34->v_speed_saut.z < -20.0)
    {
        v34->f_speed = 0.0;
        v34->f_time_etat = TIM_gf_MainClock - TIM_gf_LockTime;
        v34->f_time_saut = TIM_gf_MainClock - TIM_gf_LockTime;
        v34->f_time_plonge = TIM_gf_MainClock - TIM_gf_LockTime;
        v21 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Fun_baleine_bal_saute_Fun_baleine_bal_move);
        if (v21 != 1000)
            return v21;
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v33);
    v6 = v33;
    if (v33.z > 0.0)
    {
        v20 = TIM_gf_dt;
        v7 = TIM_gf_dt * 20.0f * 6.0f;
        v23 = 0;
        v24 = 0.0;
        v25 = -v7;
        v3 = &v34->v_speed_saut;
        v34->v_speed_saut.x = v34->v_speed_saut.x + 0.0f;
        v3->y = v24 + v3->y;
        v3->z = v25 + v3->z;
    }
    return 1000;
}

int Fun_baleine_bal_plonge()
{
    int result; // eax
    tScriptStruct_PNJBaleine* v1; // [esp+8h] [ebp-124h]
    Vec3* v2; // [esp+Ch] [ebp-120h]
    Vec3* v3; // [esp+10h] [ebp-11Ch]
    tGameObjectMain* v4; // [esp+18h] [ebp-114h]
    tGameObjectMain* v5; // [esp+20h] [ebp-10Ch]
    float v6; // [esp+2Ch] [ebp-100h]
    float v7; // [esp+30h] [ebp-FCh]
    Vec3 a2; // [esp+54h] [ebp-D8h] BYREF
    Vec3 a3; // [esp+60h] [ebp-CCh] BYREF
    int v10; // [esp+6Ch] [ebp-C0h]
    float v11; // [esp+70h] [ebp-BCh]
    Vec3 v12; // [esp+74h] [ebp-B8h] BYREF
    Vec3 v13; // [esp+80h] [ebp-ACh]
    Vec3 v14; // [esp+8Ch] [ebp-A0h] BYREF
    tGameObjectMain* v15; // [esp+A0h] [ebp-8Ch]
    BOOL v16; // [esp+A4h] [ebp-88h]
    Vec3 v17; // [esp+A8h] [ebp-84h] BYREF
    Vec3 a5; // [esp+B8h] [ebp-74h] BYREF
    float v19; // [esp+C4h] [ebp-68h]
    float v20; // [esp+CCh] [ebp-60h]
    float v21; // [esp+D0h] [ebp-5Ch]
    int v23; // [esp+D8h] [ebp-54h]
    Vec3 a1; // [esp+ECh] [ebp-40h] BYREF
    tScriptStruct_PNJBaleine* v25; // [esp+F8h] [ebp-34h]
    Vec3 v26; // [esp+FCh] [ebp-30h] BYREF
    Vec3 v27; // [esp+108h] [ebp-24h] BYREF
    Vec3 v28; // [esp+114h] [ebp-18h] BYREF
    Vec3 v29; // [esp+120h] [ebp-Ch] BYREF

    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJBaleine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v25 = v1;
    v15 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Fun_baleine_bal_plonge_Fun_baleine_bal_plouf = (int)AI_ul_RealResolveOneRefFunction(0x3D000D42u);
        fct_Fun_baleine_bal_plonge_Fun_baleine_bal_move = (int)AI_ul_RealResolveOneRefFunction(0x3D000D2Du);
    }
    v25->f_time_jet = TIM_gf_MainClock - TIM_gf_LockTime;
    v25->i_etat = 2;
    AI_EvalFunc_OBJ_InfoPhotoFlagsSet_C(AI_gpst_CurrentGameObject, 1, 0);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a1);
    a3 = a1;
    if (AI_EvalFunc_TIMEElapsed_C(v25->f_time_etat, 3.0))
    {
        v25->f_reqinertia = 10.0;
        a3.z = 0.0;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v12);
        v21 = TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&v12, &a3, TIM_gf_dt, &v26);
        a3 = v26;
        v14.x = 0.0;
        v14.y = 0.0;
        v14.z = 1.0;
        AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a3, &v14);
    }
    else
    {
        v25->f_reqinertia = 1.0;
        a3.z = a3.z - 1.0f;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
        AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, TIM_gf_dt, &a5);
        a3 = a5;
        v27.x = 0.0;
        v27.y = 0.0;
        v27.z = 1.0;
        AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a3, &v27);
    }
    v19 = TIM_gf_dt;
    v7 = 2.0f * TIM_gf_dt;
    if (v7 < 0.0)
        v7 = 0.0;
    if (v7 > 1.0)
        v7 = 1.0;
    v11 = 50.0f * v7 + v25->f_speed * (1.0f - v7);
    v25->f_speed = v11;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v29);
    v20 = TIM_gf_dt;
    v6 = v25->f_speed * TIM_gf_dt;
    v13.x = v29.x * v6;
    v13.y = v29.y * v6;
    v13.z = v29.z * v6;
    a3 = v13;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v17);
    a3.x = a3.x + v17.x;
    a3.y = a3.y + v17.y;
    a3.z = a3.z + v17.z;
    AI_EvalFunc_OBJSetPos_C_CURRENT(&a3);
    if (AI_EvalFunc_TIMEElapsed_C(v25->f_time_etat, 4.0))
    {
        v15 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(8u);
        v5 = v15;
        if (v15)
        {
            if ((unsigned int)v15 <= 2)
                v5 = AI_MainActor[(uint32_t)v15 - 1];
            AI_EvalFunc_OBJGetPos_C(v5, &v25->v_queue);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v25->v_queue);
        }
        v15 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(9u);
        v4 = v15;
        if (v15)
        {
            if ((unsigned int)v15 <= 2)
                v4 = AI_MainActor[(uint32_t)v15 - 1];
            AI_EvalFunc_OBJGetPos_C(v4, &v28);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v28);
        }
        v3 = &v25->v_queue;
        v25->v_queue.x = v28.x + v25->v_queue.x;
        v3->y = v28.y + v3->y;
        v3->z = v28.z + v3->z;
        v2 = &v25->v_queue;
        v25->v_queue.x = v25->v_queue.x * 0.5f;
        v2->y = v2->y * 0.5f;
        v2->z = v2->z * 0.5f;
        v23 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Fun_baleine_bal_plonge_Fun_baleine_bal_plouf);
        if (v23 != 1000)
            return v23;
    }
    v16 = AI_EvalFunc_TIMEElapsed_C(v25->f_time_etat, 5.0);
    if (!v16
        || (v25->f_time_plonge = TIM_gf_MainClock - TIM_gf_LockTime,
            v25->f_time_etat = TIM_gf_MainClock - TIM_gf_LockTime,
            v10 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Fun_baleine_bal_plonge_Fun_baleine_bal_move),
            v10 == 1000))
    {
        result = 1000;
    }
    else
    {
        result = v10;
    }
    return result;
}

int Fun_baleine_bal_afterrec()
{
    int result; // eax
    tGameObjectMain* v1; // eax
    tScriptStruct_PNJBaleine* v2; // [esp+8h] [ebp-24Ch]
    tGameObjectMain* v3; // [esp+10h] [ebp-244h]
    tGameObjectMain* v4; // [esp+28h] [ebp-22Ch]
    tGameObjectMain* v5; // [esp+38h] [ebp-21Ch]
    float v6; // [esp+3Ch] [ebp-218h]
    tGameObjectMain* v7; // [esp+4Ch] [ebp-208h]
    tGameObjectMain* v8; // [esp+5Ch] [ebp-1F8h]
    tGameObjectMain* v9; // [esp+6Ch] [ebp-1E8h]
    tGameObjectMain* v10; // [esp+7Ch] [ebp-1D8h]
    tGameObjectMain* v11; // [esp+8Ch] [ebp-1C8h]
    tGameObjectMain* v12; // [esp+90h] [ebp-1C4h]
    tGameObjectMain* v13; // [esp+94h] [ebp-1C0h]
    Vec3 v14; // [esp+98h] [ebp-1BCh]
    Vec3 v15; // [esp+A4h] [ebp-1B0h] BYREF
    Vec3 dest; // [esp+B0h] [ebp-1A4h] BYREF
    Vec3 a1; // [esp+BCh] [ebp-198h] BYREF
    Vec3 v18; // [esp+C8h] [ebp-18Ch] BYREF
    float v19; // [esp+D4h] [ebp-180h]
    Vec3 v20; // [esp+D8h] [ebp-17Ch] BYREF
    Vec3 v21; // [esp+E4h] [ebp-170h] BYREF
    Vec3 v22; // [esp+F0h] [ebp-164h] BYREF
    Vec3 v23; // [esp+FCh] [ebp-158h] BYREF
    int v24; // [esp+108h] [ebp-14Ch]
    Vec3 v25; // [esp+10Ch] [ebp-148h] BYREF
    Vec3 v26; // [esp+118h] [ebp-13Ch] BYREF
    tGameObjectMain* v27; // [esp+124h] [ebp-130h]
    Vec3 v28; // [esp+128h] [ebp-12Ch] BYREF
    Vec3 v29; // [esp+134h] [ebp-120h] BYREF
    Vec3 v30; // [esp+140h] [ebp-114h] BYREF
    Vec3 v31; // [esp+14Ch] [ebp-108h] BYREF
    Vec3 v32; // [esp+160h] [ebp-F4h] BYREF
    Vec3 v33; // [esp+16Ch] [ebp-E8h] BYREF
    Vec3 v35; // [esp+17Ch] [ebp-D8h] BYREF
    Vec3 v36; // [esp+188h] [ebp-CCh] BYREF
    Vec3 a3; // [esp+1A0h] [ebp-B4h] BYREF
    Vec3 v39; // [esp+1ACh] [ebp-A8h] BYREF
    Vec3 v40; // [esp+1B8h] [ebp-9Ch] BYREF
    Vec3 v41; // [esp+1C4h] [ebp-90h]
    Vec3 v42; // [esp+1D0h] [ebp-84h] BYREF
    Vec3 v43; // [esp+1DCh] [ebp-78h] BYREF
    int i; // [esp+1E8h] [ebp-6Ch]
    tGameObjectMain* v45; // [esp+1F0h] [ebp-64h]
    float v46; // [esp+1F4h] [ebp-60h]
    float v47; // [esp+1F8h] [ebp-5Ch]
    float v48; // [esp+1FCh] [ebp-58h]
    tScriptStruct_PNJBaleine* v49; // [esp+204h] [ebp-50h]
    Vec3 v50; // [esp+20Ch] [ebp-48h] BYREF
    Vec3 v51; // [esp+218h] [ebp-3Ch] BYREF
    float v52; // [esp+230h] [ebp-24h]
    Vec3 v53; // [esp+238h] [ebp-1Ch] BYREF
    Vec3 v54; // [esp+244h] [ebp-10h] BYREF

    v18.x = 1.0;
    v18.y = 0.0;
    v18.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJBaleine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v49 = v2;
    i = 0;
    v45 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Fun_baleine_bal_afterrec_Fun_baleine_bal_queue = (int)AI_ul_RealResolveOneRefFunction(0x3D000D3Eu);
        Fun_baleine_bal_afterrec_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    v27 = *AI_MainActor;
    v13 = v27;
    if (v27)
    {
        if ((unsigned int)v27 <= 2)
            v13 = AI_MainActor[(uint32_t)v27 - 1];
        v12 = v13;
    }
    else
    {
        v12 = 0;
    }
    if (v12)
    {
        v45 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(1u);
        v11 = v45;
        if (v45)
        {
            if ((unsigned int)v45 <= 2)
                v11 = AI_MainActor[(uint32_t)v45 - 1];
            AI_EvalFunc_OBJGetPos_C(v11, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
        MATH_SubstractVec(&dest, &a3, &a1);
        AI_EvalFunc_MATHVecGlobalToLocal_C(AI_gpst_CurrentGameObject, &dest, &v32);
        v18 = v32;
        AI_EvalFunc_COLGeoColMapPosSet_C(AI_gpst_CurrentGameObject, 0, &v18);
        v45 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(2u);
        v10 = v45;
        if (v45)
        {
            if ((unsigned int)v45 <= 2)
                v10 = AI_MainActor[(uint32_t)v45 - 1];
            AI_EvalFunc_OBJGetPos_C(v10, &v21);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v21);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v40);
        MATH_SubstractVec(&v50, &v21, &v40);
        AI_EvalFunc_MATHVecGlobalToLocal_C(AI_gpst_CurrentGameObject, &v50, &v25);
        v18 = v25;
        AI_EvalFunc_COLGeoColMapPosSet_C(AI_gpst_CurrentGameObject, 1u, &v18);
        v45 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(3u);
        v9 = v45;
        if (v45)
        {
            if ((unsigned int)v45 <= 2)
                v9 = AI_MainActor[(uint32_t)v45 - 1];
            AI_EvalFunc_OBJGetPos_C(v9, &v20);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v20);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v54);
        MATH_SubstractVec(&v36, &v20, &v54);
        AI_EvalFunc_MATHVecGlobalToLocal_C(AI_gpst_CurrentGameObject, &v36, &v26);
        v18 = v26;
        AI_EvalFunc_COLGeoColMapPosSet_C(AI_gpst_CurrentGameObject, 2u, &v18);
        v45 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(4u);
        v8 = v45;
        if (v45)
        {
            if ((unsigned int)v45 <= 2)
                v8 = AI_MainActor[(uint32_t)v45 - 1];
            AI_EvalFunc_OBJGetPos_C(v8, &v15);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v15);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v31);
        MATH_SubstractVec(&v43, &v15, &v31);
        AI_EvalFunc_MATHVecGlobalToLocal_C(AI_gpst_CurrentGameObject, &v43, &v35);
        v18 = v35;
        AI_EvalFunc_COLGeoColMapPosSet_C(AI_gpst_CurrentGameObject, 3u, &v18);
        v45 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(5u);
        v7 = v45;
        if (v45)
        {
            if ((unsigned int)v45 <= 2)
                v7 = AI_MainActor[(uint32_t)v45 - 1];
            AI_EvalFunc_OBJGetPos_C(v7, &v39);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v39);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v29);
        MATH_SubstractVec(&v28, &v39, &v29);
        AI_EvalFunc_MATHVecGlobalToLocal_C(AI_gpst_CurrentGameObject, &v28, &v42);
        v18 = v42;
        AI_EvalFunc_COLGeoColMapPosSet_C(AI_gpst_CurrentGameObject, 4u, &v18);
        v19 = TIM_gf_dt;
        v6 = 2.0f * TIM_gf_dt;
        if (v6 < 0.0)
            v6 = 0.0;
        if (v6 > 1.0)
            v6 = 1.0;
        v52 = v49->f_reqinertia * v6 + v49->f_inertia * (1.0f - v6);
        v49->f_inertia = v52;
        AI_EvalFunc_SNAKE_SetInertia_C(AI_gpst_CurrentGameObject, v49->f_inertia);
        if (AI_EvalFunc_TIMEElapsed_C(v49->f_time_ripple, 0.5))
        {
            for (i = 0; i < 10; ++i)
            {
                v1 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(i);
                v45 = v1;
                v5 = v1;
                if (v1)
                {
                    if ((unsigned int)v1 <= 2)
                        v5 = AI_MainActor[(uint32_t)v1 - 1];
                    AI_EvalFunc_OBJGetPos_C(v5, &v33);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v33);
                }
                v18 = v33;
                if (v33.z > -25.0 && v18.z < 5.0)
                {
                    v49->f_time_ripple = TIM_gf_MainClock - TIM_gf_LockTime;
                    v49->i_can_queue = 1;
                    v4 = v45;
                    if (v45)
                    {
                        if ((unsigned int)v45 <= 2)
                            v4 = AI_MainActor[(uint32_t)v45 - 1];
                        AI_EvalFunc_OBJGetPos_C(v4, &v49->v_queue);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v49->v_queue);
                    }
                    v24 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Fun_baleine_bal_afterrec_Fun_baleine_bal_queue);
                    if (v24 != 1000)
                        return v24;
                }
            }
        }
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v53);
        v41.x = -v53.x;
        v41.y = -v53.y;
        v41.z = -v53.z;
        v14.x = -v53.x * 22.0f;
        v14.y = -v53.y * 22.0f;
        v14.z = -v53.z * 22.0f;
        v18 = v14;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v23);
        v46 = v23.x * 25.0f;
        v47 = v23.y * 25.0f;
        v48 = v23.z * 25.0f;
        v18.x = v18.x + v46;
        v18.y = v18.y + v47;
        v18.z = v18.z + v48;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v51);
        v18.x = v18.x + v51.x;
        v18.y = v18.y + v51.y;
        v18.z = v18.z + v51.z;
        if (v18.z >= -2.0)
        {
            if (AI_EvalFunc_TIMEElapsed_C(v49->f_time_jet, 8.0))
            {
                v49->f_time_jet = TIM_gf_MainClock - TIM_gf_LockTime;
                v49->i_Gfx = AI_EvalFunc_GFXAdd_C_CURRENT(9);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v49->i_Gfx, 0, 1);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v49->i_Gfx, 2, 1);
                v3 = (tGameObjectMain*)Fun_baleine_bal_afterrec_SFX_light_smoke_gao;
                if (Fun_baleine_bal_afterrec_SFX_light_smoke_gao)
                {
                    if ((unsigned int)Fun_baleine_bal_afterrec_SFX_light_smoke_gao <= 2)
                        v3 = AI_MainActor[Fun_baleine_bal_afterrec_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v49->i_Gfx, v3, 5);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v49->i_Gfx, 0, 5);
                }
                GFXSetCurGlobal(v49->i_Gfx);
                AI_EvalFunc_GFXSetv_C_CURGLOB(9200, &v18);
                v22.x = 0.0;
                v22.y = 0.0;
                v22.z = 1.0;
                AI_EvalFunc_GFXSetv_C_CURGLOB(9201, &v22);
                v30.x = 0.0;
                v30.y = 0.0;
                v30.z = -40.0;
                AI_EvalFunc_GFXSetv_C_CURGLOB(9202, &v30);
                AI_EvalFunc_GFXSeti_C_CURGLOB(9100, 150);
                AI_EvalFunc_GFXSeti_C_CURGLOB(9101, 0x201010);
                AI_EvalFunc_GFXSeti_C_CURGLOB(9102, 24);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9000, 0.050000001f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9001, 80.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9002, 200.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9003, 0.995f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9004, 2.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9005, 0.5);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9006, 1.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9007, 6.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9008, 0.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9009, 0.2f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9010, 0.30000001f);
                result = 1000;
            }
            else
            {
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 1000;
            }
        }
        else
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int Fun_baleine_bal_plouf()
{
    int result; // eax
    tGameObjectMain* v1; // eax
    tScriptStruct_PNJBaleine* v2; // [esp+0h] [ebp-88h]
    tGameObjectMain* v3; // [esp+18h] [ebp-70h]
    tGameObjectMain* v4; // [esp+20h] [ebp-68h]
    tGameObjectMain* v5; // [esp+30h] [ebp-58h]
    tGameObjectMain* v6; // [esp+34h] [ebp-54h]
    tGameObjectMain* v7; // [esp+38h] [ebp-50h]
    float zoom; // [esp+40h] [ebp-48h]
    Vec3 a2; // [esp+44h] [ebp-44h] BYREF
    int v10; // [esp+50h] [ebp-38h]
    unsigned int v11; // [esp+54h] [ebp-34h]
    Vec3 pos; // [esp+68h] [ebp-20h] BYREF
    tScriptStruct_PNJBaleine* v13; // [esp+74h] [ebp-14h]
    int v14; // [esp+78h] [ebp-10h]

    a2.x = 1.0;
    a2.y = 0.0;
    a2.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJBaleine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v13 = v2;
    v14 = 0;
    v11 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Fun_baleine_bal_plouf_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    if (v13->i_can_plonge)
    {
        v7 = (tGameObjectMain*)v13->o_explo_com;
        if (v7)
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
            if (v13->i_count_plouf != 3)
            {
                v10 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(AI_gpst_CurrentGameObject, 1, -1.0, 0.85000002f, 1.25);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
                v5 = (tGameObjectMain*)v13->o_explo_com;
                if (v5)
                {
                    if ((unsigned int)v5 <= 2)
                        v5 = AI_MainActor[(uint32_t)v5 - 1];
                    v1 = AI_EvalFunc_OBJDuplicate_C(v5, &pos);
                }
                else
                {
                    v1 = AI_EvalFunc_OBJDuplicate_C(0, &pos);
                }
                v11 = (unsigned int)v1;
                a2 = v13->v_queue;
                a2.z = v13->des_f_water_Z;
                v4 = v1;
                if (v1)
                {
                    if ((unsigned int)v1 <= 2)
                        v4 = AI_MainActor[(uint32_t)v1 - 1];
                    AI_EvalFunc_OBJSetPos_C(v4, &a2);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &a2);
                }
                zoom = AI_EvalFunc_MATHRandFloat_C(6.0, 8.0);
                v3 = (tGameObjectMain*)v11;
                if (v11)
                {
                    if (v11 <= 2)
                        v3 = AI_MainActor[v11 - 1];
                    AI_EvalFunc_OBJ_ZoomSet_C(v3, zoom);
                }
                else
                {
                    AI_EvalFunc_OBJ_ZoomSet_C(0, zoom);
                }
                ++v13->i_count_plouf;
            }
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}