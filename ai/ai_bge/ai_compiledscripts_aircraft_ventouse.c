#include "ai_compiledscripts_inc.h"

//Ventouse_Ventouse_init
int Ventouse_Ventouse_init_SFX_light_smoke_gao;
int Ventouse_Ventouse_init__PJ_Aircraft_gao;
int fct_Ventouse_Ventouse_init_Ventouse_Ventouse_afterrec;
int fct_Ventouse_Ventouse_init_Ventouse_Ventouse_avec_cible;

//Ventouse_Ventouse_explosion
int Ventouse_Ventouse_explosion__PJ_Aircraft_gao;

//Ventouse_Ventouse_avec_cible
int Ventouse_Ventouse_avec_cible__PJ_Aircraft_gao;
int Ventouse_Ventouse_avec_cible_GST_Global_gao;
int Ventouse_Ventouse_avec_cible_GST_Global_gao_f_eau_hauteurZ;
int fct_Ventouse_Ventouse_avec_cible_Ventouse_Ventouse_explosion;

int Ventouse_Ventouse_init()
{
    int v0; // eax
    int result; // eax
    int v2; // [esp+4h] [ebp-ACh]
    tScriptStruct_AirCraftVentouse* v3; // [esp+8h] [ebp-A8h]
    tGameObjectMain* v4; // [esp+10h] [ebp-A0h]
    tGameObjectMain* v5; // [esp+18h] [ebp-98h]
    float v6; // [esp+1Ch] [ebp-94h]
    Vec3* v7; // [esp+2Ch] [ebp-84h]
    Vec3* v8; // [esp+30h] [ebp-80h]
    void* v9; // [esp+34h] [ebp-7Ch]
    int v10; // [esp+38h] [ebp-78h]
    BOOL v11; // [esp+48h] [ebp-68h]
    Vec3 v12; // [esp+54h] [ebp-5Ch] BYREF
    BOOL v13; // [esp+64h] [ebp-4Ch]
    tGameObjectMain* v14; // [esp+68h] [ebp-48h]
    Vec3 v15; // [esp+6Ch] [ebp-44h] BYREF
    int v16; // [esp+7Ch] [ebp-34h]
    int v17; // [esp+80h] [ebp-30h]
    int v18; // [esp+84h] [ebp-2Ch]
    void* Src; // [esp+88h] [ebp-28h]
    int v20; // [esp+8Ch] [ebp-24h]
    tScriptStruct_AirCraftVentouse* v21; // [esp+90h] [ebp-20h]
    Vec3 a1; // [esp+94h] [ebp-1Ch] BYREF
    Vec3 a2; // [esp+A0h] [ebp-10h] BYREF

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_AirCraftVentouse*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v21 = v3;
    v16 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Ventouse_Ventouse_init_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        Ventouse_Ventouse_init__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        fct_Ventouse_Ventouse_init_Ventouse_Ventouse_afterrec = (int)AI_ul_RealResolveOneRefFunction(0x72004C19u);
        fct_Ventouse_Ventouse_init_Ventouse_Ventouse_avec_cible = (int)AI_ul_RealResolveOneRefFunction(0x72004C18u);
    }
    v13 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
    v20 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x1000004, 0);
    Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v21->m_msg, Src, sizeof(v21->m_msg));
    while (v21->m_msg.msgID)
    {
        v21->o_sender = (int)v21->m_msg.gao;
        if (v21->m_msg.msgID == 1)
        {
            if (v21->m_msg.f2 >= 0.0)
                v21->des_f_rotation_speed_max = v21->m_msg.f2;
            if (v21->m_msg.f1 >= 0.0)
                v21->des_f_init_speed = v21->m_msg.f1;
            if (v21->m_msg.Int1 >= 0)
                v21->des_f_life_time = (float)v21->m_msg.Int1;
            if (v21->m_msg.Int2 >= 0)
                v21->des_f_traction = (float)v21->m_msg.Int2;
            v7 = &v21->v_offset;
            v8 = &v21->m_msg.v1;
            v21->v_offset.x = v21->m_msg.v1.x;
            v7->y = v8->y;
            v7->z = v8->z;
            if (fabs(v21->m_msg.v2.x) != 0.0f)
            {
                v11 = 0;
            }
            else if (fabs(v21->m_msg.v2.y) != 0.0f)
            {
                v11 = 0;
            }
            else
            {
                v11 = fabs(v21->m_msg.v2.z) == 0.0f;
            }
            if (!v11)
                v21->des_f_friction = v21->m_msg.v2.x;
        }
        v9 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v21->m_msg, v9, sizeof(v21->m_msg));
    }
    v15.x = -400.0;
    v15.y = -400.0;
    v15.z = -400.0;
    AI_EvalFunc_BVMinSet_C(AI_gpst_CurrentGameObject, &v15);
    a2.x = 400.0;
    a2.y = 400.0;
    a2.z = 400.0;
    AI_EvalFunc_BVMaxSet_C(AI_gpst_CurrentGameObject, &a2);
    v21->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
    AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v21->des_v_gravity);
    v18 = AI_EvalFunc_DYNFrictionSet_C(AI_gpst_CurrentGameObject, v21->des_f_friction);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a1);
    v6 = v21->des_f_init_speed;
    v12.x = a1.x * v6;
    v12.y = a1.y * v6;
    v12.z = a1.z * v6;
    v17 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v12);
    v21->GFX_Halo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
    AI_EvalFunc_GFXFlagSet_C_CURRENT(v21->GFX_Halo, 4, 1);
    v5 = (tGameObjectMain*)Ventouse_Ventouse_init_SFX_light_smoke_gao;
    if (Ventouse_Ventouse_init_SFX_light_smoke_gao)
    {
        if ((unsigned int)Ventouse_Ventouse_init_SFX_light_smoke_gao <= 2)
            v5 = AI_MainActor[Ventouse_Ventouse_init_SFX_light_smoke_gao - 1];
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v21->GFX_Halo, v5, 0);
    }
    else
    {
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v21->GFX_Halo, 0, 0);
    }
    GFXSetCurGlobal(v21->GFX_Halo);
    AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
    AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
    AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 2.0);
    v16 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 2, 0x18000080);
    AI_EvalFunc_SNDUltraPlaySound_C((tGameObjectMain*)Ventouse_Ventouse_init__PJ_Aircraft_gao, v16);
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(256, 0);
    v21->f_snd_vol = 0.0;
    v0 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 3, 0x18000000);
    v21->i_snd_loop = v0;
    AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v21->i_snd_loop);
    v2 = SND_l_GetFreq(v21->i_snd_loop);
    v21->f_snd_freq = (float)v2;
    v14 = AI_gpst_CurrentGameObject;
    v4 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v4 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v4,
            2,
            (tAIFunction*)fct_Ventouse_Ventouse_init_Ventouse_Ventouse_afterrec);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            2,
            (tAIFunction*)fct_Ventouse_Ventouse_init_Ventouse_Ventouse_afterrec);
    }
    v10 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Ventouse_Ventouse_init_Ventouse_Ventouse_avec_cible);
    if (v10 == 1000)
        result = 1000;
    else
        result = v10;
    return result;
}

int Ventouse_Ventouse_afterrec()
{
    tScriptStruct_AirCraftVentouse* v1; // [esp+0h] [ebp-14h]
    Vec3 a1; // [esp+4h] [ebp-10h] BYREF
    tScriptStruct_AirCraftVentouse* v3; // [esp+10h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_AirCraftVentouse*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v3 = v1;
    if (v1->GFX_Halo != -1)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
        AI_EvalFunc_GFXSetv_C_CURRENT(v3->GFX_Halo, 21200, &a1);
    }
    return 1000;
}

int Ventouse_Ventouse_explosion()
{
    int result; // eax
    float v1; // [esp+8h] [ebp-7Ch]
    tScriptStruct_AirCraftVentouse* v2; // [esp+10h] [ebp-74h]
    float v3; // [esp+14h] [ebp-70h]
    float v4; // [esp+18h] [ebp-6Ch]
    Vec3* v5; // [esp+1Ch] [ebp-68h]
    Vec3* v6; // [esp+20h] [ebp-64h]
    Vec3 a1; // [esp+48h] [ebp-3Ch] BYREF
    unsigned int v8; // [esp+54h] [ebp-30h]
    float v9; // [esp+58h] [ebp-2Ch]
    float v10; // [esp+5Ch] [ebp-28h]
    tScriptStruct_AirCraftVentouse* v11; // [esp+68h] [ebp-1Ch]
    int a2; // [esp+6Ch] [ebp-18h]
    Vec3 v13; // [esp+70h] [ebp-14h] BYREF
    float v15; // [esp+80h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_AirCraftVentouse*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v11 = v2;
    a2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Ventouse_Ventouse_explosion__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)5)
        goto LABEL_19;
    if (*AI_gppst_CurrentJumpNode != (tAINode*)6)
    {
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 256);
        SND_StopRq(v11->i_snd_loop);
        v11->i_snd_loop = -1;
        if (v11->i_snd_col == -1)
            a2 = 1;
        else
            a2 = AI_EvalFunc_COLGMSoundGet_C((tGameObjectMain*)Ventouse_Ventouse_explosion__PJ_Aircraft_gao, v11->i_snd_col);
        if (a2 == 1 || a2 == 2)
            a2 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000080);
        else
            a2 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 1, 0x18000080);
        AI_EvalFunc_SNDUltraPlaySound_C((tGameObjectMain*)Ventouse_Ventouse_explosion__PJ_Aircraft_gao, a2);
        v11->m_msg.msgID = 89;
        v11->m_msg.gao = AI_gpst_CurrentGameObject;
        v11->m_msg.gao2 = (tGameObjectMain*)v11->o_sender;
        v11->m_msg.Int1 = 0;
        v11->m_msg.Int2 = 0;
        v11->m_msg.f2 = 0.0;
        v11->m_msg.f1 = 0.0;
        v5 = &v11->m_msg.v1;
        v6 = &v11->v_offset;
        v11->m_msg.v1.x = v11->v_offset.x;
        v5->y = v6->y;
        v5->z = v6->z;
        v11->m_msg.v2.x = 0.0;
        v11->m_msg.v2.y = 0.0;
        v11->m_msg.v2.z = 0.0;
        AI_EvalFunc_MSGSend_C((tGameObjectMain*)Ventouse_Ventouse_explosion__PJ_Aircraft_gao, &v11->m_msg);
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        a1.x = 0.0;
        a1.y = 0.0;
        a1.z = 0.0;
        AI_EvalFunc_DYNTractionSet_C_CURRENT(&a1);
        v13.x = 0.0;
        v13.y = 0.0;
        v13.z = 0.0;
        AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v13);
        v10 = TIM_gf_MainClock - TIM_gf_LockTime;
        v11->f_time_disp = v10;
        v11->f_size_halo = 2.0;
    LABEL_19:
        if (AI_EvalFunc_TIMEElapsed_C(v11->f_time_disp, 2.0))
        {
            v8 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0, 0x10000000);
            *AI_gppst_CurrentJumpNode = (tAINode*)6;
            TIME_Wait_C(0.1f);
            result = 0;
        }
        else
        {
            v4 = 4.0f * TIM_gf_dt;
            if (v4 < 0.0)
                v4 = 0.0;
            if (v4 > 1.0)
                v4 = 1.0;
            v9 = 0.0f * v4 + v11->f_largeur_X_men * (1.0f - v4);
            v11->f_largeur_X_men = v9;
            XMEN_SetSize(AI_gpst_CurrentGameObject, 0, -1, v11->f_largeur_X_men);
            v3 = v11->f_size_halo - TIM_gf_dt * 2.4000001f;
            if (v3 < 0.0)
                v1 = 0.0;
            else
                v1 = v11->f_size_halo - TIM_gf_dt * 2.4000001f;
            v15 = v1;
            v11->f_size_halo = v1;
            AI_EvalFunc_GFXSetf_C_CURRENT(v11->GFX_Halo, 21000, v11->f_size_halo);
            *AI_gppst_CurrentJumpNode = (tAINode*)5;
            result = 0;
        }
        return result;
    }
    result = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
    if (result == 1000)
        result = 1000;
    return result;
}

int Ventouse_Ventouse_avec_cible()
{
    float a4; // [esp+0h] [ebp-158h]
    float v2; // [esp+8h] [ebp-150h]
    tScriptStruct_AirCraftVentouse* v3; // [esp+Ch] [ebp-14Ch]
    tGameObjectMain* v4; // [esp+10h] [ebp-148h]
    tGameObjectMain* v5; // [esp+14h] [ebp-144h]
    tGameObjectMain* v6; // [esp+18h] [ebp-140h]
    tGameObjectMain* v7; // [esp+1Ch] [ebp-13Ch]
    float v8; // [esp+30h] [ebp-128h]
    float v9; // [esp+34h] [ebp-124h]
    float v10; // [esp+38h] [ebp-120h]
    float v11; // [esp+3Ch] [ebp-11Ch]
    float v12; // [esp+40h] [ebp-118h]
    float v13; // [esp+44h] [ebp-114h]
    float v14; // [esp+54h] [ebp-104h]
    float v15; // [esp+60h] [ebp-F8h]
    Vec3 dest; // [esp+64h] [ebp-F4h] BYREF
    float v17; // [esp+70h] [ebp-E8h]
    Vec3 v18; // [esp+74h] [ebp-E4h] BYREF
    Vec3 v19; // [esp+80h] [ebp-D8h] BYREF
    tGameObjectMain* v20; // [esp+8Ch] [ebp-CCh]
    Vec3 a2; // [esp+90h] [ebp-C8h] BYREF
    float v22; // [esp+9Ch] [ebp-BCh]
    Vec3 v23; // [esp+A0h] [ebp-B8h] BYREF
    Vec3 a3; // [esp+B4h] [ebp-A4h] BYREF
    int v25; // [esp+C0h] [ebp-98h]
    float v26; // [esp+C4h] [ebp-94h]
    tGameObjectMain* v27; // [esp+C8h] [ebp-90h]
    float v28; // [esp+CCh] [ebp-8Ch]
    Vec3 a1; // [esp+D0h] [ebp-88h] BYREF
    float v30; // [esp+DCh] [ebp-7Ch]
    float v31; // [esp+E0h] [ebp-78h]
    float v32; // [esp+E4h] [ebp-74h]
    float v33; // [esp+E8h] [ebp-70h]
    float v34; // [esp+ECh] [ebp-6Ch]
    float v35; // [esp+F0h] [ebp-68h]
    int v36; // [esp+F4h] [ebp-64h]
    Vec3 v37; // [esp+FCh] [ebp-5Ch] BYREF
    int v38; // [esp+108h] [ebp-50h]
    int v39; // [esp+110h] [ebp-48h]
    float v42; // [esp+124h] [ebp-34h]
    Vec3 v43; // [esp+128h] [ebp-30h] BYREF
    tScriptStruct_AirCraftVentouse* v44; // [esp+134h] [ebp-24h]
    float v45; // [esp+138h] [ebp-20h]
    int v46; // [esp+13Ch] [ebp-1Ch]
    Vec3 a5; // [esp+148h] [ebp-10h] BYREF
    int v48; // [esp+154h] [ebp-4h]

    v37.x = 1.0;
    v37.y = 0.0;
    v37.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    v19.x = 1.0;
    v19.y = 0.0;
    v19.z = 0.0;
    v33 = 1.0;
    v34 = 0.0;
    v35 = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_AirCraftVentouse*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v44 = v3;
    v26 = 0.0;
    v28 = 0.0;
    v45 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Ventouse_Ventouse_avec_cible__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        Ventouse_Ventouse_avec_cible_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Ventouse_Ventouse_avec_cible_GST_Global_gao_f_eau_hauteurZ,
            (void*)Ventouse_Ventouse_avec_cible_GST_Global_gao,
            2772);
        fct_Ventouse_Ventouse_avec_cible_Ventouse_Ventouse_explosion = (int)AI_ul_RealResolveOneRefFunction(0x72004C16u);
    }
    v45 = TIM_gf_dt;
    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Ventouse_Ventouse_avec_cible__PJ_Aircraft_gao, &v43);
    v19 = v43;
    v14 = *(float*)Ventouse_Ventouse_avec_cible_GST_Global_gao_f_eau_hauteurZ + 2.0f;
    if (v14 < (double)v43.z)
        v2 = v19.z;
    else
        v2 = *(float*)Ventouse_Ventouse_avec_cible_GST_Global_gao_f_eau_hauteurZ + 2.0f;
    v19.z = v2;
    v26 = v44->des_f_traction;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v18);
    MATH_SubstractVec(&dest, &v19, &v18);
    a3 = dest;
    v15 = MATH_VecLen(&a3);
    if (v15 == 0.0f)
    {
        v36 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Ventouse_Ventouse_avec_cible_Ventouse_Ventouse_explosion);
        if (v36 != 1000)
            return v36;
    }
    else
    {
        v13 = 1.0f / v15;
        a3.x = a3.x * v13;
        a3.y = a3.y * v13;
        a3.z = a3.z * v13;
        v12 = 0.5f * v45;
        if (v12 < 0.0)
            v12 = 0.0;
        if (v12 > 1.0)
            v12 = 1.0;
        v44->f_largeur_X_men = 15.0f * v12 + v44->f_largeur_X_men * (1.0f - v12);
        XMEN_SetSize(AI_gpst_CurrentGameObject, 0, -1, v44->f_largeur_X_men);
        if (v15 >= 20.0f)
        {
            v9 = v44->des_f_rot_coef_blend_speed * v45;
            if (v9 < 0.0)
                v9 = 0.0;
            if (v9 > 1.0)
                v9 = 1.0;
            v44->des_f_rotation_speed = v44->des_f_rotation_speed_max * v9 + v44->des_f_rotation_speed * (1.0f - v9);
            v8 = v44->des_f_friction_blend_speed * v45;
            if (v8 < 0.0)
                v8 = 0.0;
            if (v8 > 1.0)
                v8 = 1.0;
            v44->des_f_friction = v44->des_f_friction_max * v8 + v44->des_f_friction * (1.0f - v8);
        }
        else
        {
            v28 = v15 / 20.0f;
            v22 = TIM_gf_dt;
            if (v26 * TIM_gf_dt > v15)
                v26 = v15;
            v11 = v28;
            if (v28 < 0.0)
                v11 = 0.0;
            if (v11 > 1.0)
                v11 = 1.0;
            v44->des_f_rotation_speed = v44->des_f_rotation_speed_max * v11 + (1.0f - v11) * 60.0f;
            v10 = v28;
            if (v28 < 0.0)
                v10 = 0.0;
            if (v10 > 1.0)
                v10 = 1.0;
            v44->des_f_friction = v44->des_f_friction_max * v10 + (1.0f - v10) * 60.0f;
        }
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
        a4 = v44->des_f_rotation_speed * v45;
        AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
        v37 = a5;
        AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v37);
        v38 = AI_EvalFunc_DYNFrictionSet_C(AI_gpst_CurrentGameObject, v44->des_f_friction);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v23);
        v30 = v23.x * v26;
        v31 = v23.y * v26;
        v32 = v23.z * v26;
        v33 = v30;
        v34 = v31;
        v35 = v32;
        v17 = AI_EvalFunc_DYNFrictionGet_C(AI_gpst_CurrentGameObject);
        a1.x = v33 * v17;
        a1.y = v34 * v17;
        a1.z = v35 * v17;
        AI_EvalFunc_DYNTractionSet_C_CURRENT(&a1);
        if (v44->GFX_Halo != -1)
        {
            v42 = AI_EvalFunc_MATHRandFloat_C(0.0, 1.0);
            v39 = AI_EvalFunc_COLORBlend_C(0x8000FF00, 0x80008000, v42);
            v44->c_color = v39;
            AI_EvalFunc_GFXSeti_C_CURRENT(v44->GFX_Halo, 21101, v44->c_color);
        }
        if (v15 < 5.0)
        {
            v48 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Ventouse_Ventouse_avec_cible_Ventouse_Ventouse_explosion);
            if (v48 != 1000)
                return v48;
        }
        if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x80))
        {
            v27 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 0x80);
            v7 = v27;
            if (v27)
            {
                if ((unsigned int)v27 <= 2)
                    v7 = AI_MainActor[(uint32_t)v27 - 1];
                v6 = v7;
            }
            else
            {
                v6 = 0;
            }
            if (v6 == (tGameObjectMain*)Ventouse_Ventouse_avec_cible__PJ_Aircraft_gao)
            {
                v44->i_snd_col = 0x80;
                v25 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Ventouse_Ventouse_avec_cible_Ventouse_Ventouse_explosion);
                if (v25 != 1000)
                    return v25;
            }
        }
        if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x40))
        {
            v20 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 0x40);
            v5 = v20;
            if (v20)
            {
                if ((unsigned int)v20 <= 2)
                    v5 = AI_MainActor[(uint32_t)v20 - 1];
                v4 = v5;
            }
            else
            {
                v4 = 0;
            }
            if (v4 == (tGameObjectMain*)Ventouse_Ventouse_avec_cible__PJ_Aircraft_gao)
            {
                v44->i_snd_col = 0x40;
                v46 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Ventouse_Ventouse_avec_cible_Ventouse_Ventouse_explosion);
                if (v46 != 1000)
                    return v46;
            }
        }
    }
    return 1000;
}