#include "ai_compiledscripts_inc.h"

//frisbee_fr_rebound
int fct_frisbee_fr_rebound_frisbee_fr_impact;
int frisbee_fr_rebound_SFX_light_smoke_gao;

//frisbee_fr_init
int frisbee_fr_init__PJ_Sally_gao;
int frisbee_fr_init_GST_Global_gao;
int frisbee_fr_init_GST_Global_gao_i_projectile_ami_nbr;
int frisbee_fr_init_GST_Global_gao_oa_projectile_ami;
int frisbee_fr_init_SFX_light_smoke_gao;
int frisbee_fr_init__PJ_Camera_gao;
int fct_frisbee_fr_init_frisbee_fr_before_display;
int fct_frisbee_fr_init_frisbee_fr_jet;

//frisbee_fr_destroy
int frisbee_fr_destroy_SFX_light_smoke_gao;

//frisbee_fr_impact
int frisbee_fr_impact_SFX_light_smoke_gao;

//frisbee_fr_exec_GFX_Plouf
int frisbee_fr_exec_GFX_Plouf_SFX_light_smoke_gao;
int frisbee_fr_exec_GFX_Plouf_SFX_explosion_gao;

//frisbee_fr_jet
int fctvar_frisbee_fr_jet_UNKNOWN;
int frisbee_fr_jet__PJ_Sally_gao;
int fct_frisbee_fr_jet_frisbee_fr_exec_GFX_Plouf;
int frisbee_fr_jet_GST_Global_gao;
int frisbee_fr_jet_GST_Global_gao_f_eau_hauteurZ;
int frisbee_fr_jet_GST_Global_gao_i_projectile_ami_nbr;
int frisbee_fr_jet_GST_Global_gao_oa_projectile_ami;
int frisbee_fr_jet_GST_Global_gao_f_event_sound_time;
int frisbee_fr_jet_GST_Global_gao_i_event_sound_nbr;
int frisbee_fr_jet_GST_Global_gao_v_event_sound_pos;
int frisbee_fr_jet_GST_Global_gao_f_event_sound_size;
int frisbee_fr_jet_GST_Global_gao_o_event_sound_actor;
int frisbee_fr_jet_GST_Global_gao_i_event_sound_id;
int frisbee_fr_jet_GST_Global_gao_i_event_sound_next_id;
int fct_frisbee_fr_jet_frisbee_fr_before_display;
int fct_frisbee_fr_jet_frisbee_fr_rebound;
int fct_frisbee_fr_jet_frisbee_fr_impact;
int fct_frisbee_fr_jet_frisbee_fr_disparition;

int frisbee_fr_rebound()
{
    int result; // eax
    int v1; // eax
    float zoom; // [esp+0h] [ebp-88h]
    float zooma; // [esp+0h] [ebp-88h]
    tAINode* v4; // [esp+4h] [ebp-84h]
    tScriptStruct_Frisbee* v5; // [esp+8h] [ebp-80h]
    tGameObjectMain* v6; // [esp+10h] [ebp-78h]
    tGameObjectMain* v7; // [esp+18h] [ebp-70h]
    Vec3 v8; // [esp+1Ch] [ebp-6Ch] BYREF
    int v9; // [esp+2Ch] [ebp-5Ch]
    Vec3 v10; // [esp+30h] [ebp-58h] BYREF
    Vec3 v11; // [esp+3Ch] [ebp-4Ch] BYREF
    float v13; // [esp+50h] [ebp-38h]
    Vec3 a3; // [esp+54h] [ebp-34h] BYREF
    int v15; // [esp+60h] [ebp-28h]
    tScriptStruct_Frisbee* v16; // [esp+70h] [ebp-18h]
    int a1; // [esp+74h] [ebp-14h]
    Vec3 v18; // [esp+78h] [ebp-10h] BYREF
    float v19; // [esp+84h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_Frisbee*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v16 = v5;
    a1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_frisbee_fr_rebound_frisbee_fr_impact = (int)AI_ul_RealResolveOneRefFunction(0x260015A8u);
        frisbee_fr_rebound_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    v4 = *AI_gppst_CurrentJumpNode;
    if (*AI_gppst_CurrentJumpNode == (tAINode*)3)
    {
    LABEL_29:
        if (AI_EvalKeyword_During_C())
        {
            AI_EvalFunc_MATHVecGlobalToLocal_C(AI_gpst_CurrentGameObject, &v16->v_axe, &a3);
            AI_EvalFunc_OBJRotateAround_C(AI_gpst_CurrentGameObject, &a3, 0.75);
            v19 = TIM_gf_dt;
            zoom = 0.75f * TIM_gf_dt;
            AI_EvalFunc_OBJRotateLocalX_C(AI_gpst_CurrentGameObject, zoom);
            *AI_gppst_CurrentJumpNode = (tAINode*)3;
            return 0;
        }
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(1, 0);
        AI_gpst_CurrentInstance->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -1.0;
        goto LABEL_32;
    }
    if (v4 != (tAINode*)5)
    {
        if (v4 == (tAINode*)6)
        {
            v1 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            v9 = v1;
            if (v1 == 1000)
                result = 1000;
            else
                result = v9;
            return result;
        }
        if (v16->GFX_Halo != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v16->GFX_Halo);
            v16->GFX_Halo = -1;
        }
        if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v16->v_impact);
            v15 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_frisbee_fr_rebound_frisbee_fr_impact);
            if (v15 != 1000)
                return v15;
        }
        v16->halo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v16->halo, 4, 1);
        v7 = (tGameObjectMain*)frisbee_fr_rebound_SFX_light_smoke_gao;
        if (frisbee_fr_rebound_SFX_light_smoke_gao)
        {
            if ((unsigned int)frisbee_fr_rebound_SFX_light_smoke_gao <= 2)
                v7 = AI_MainActor[frisbee_fr_rebound_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v16->halo, v7, 29);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v16->halo, 0, 29);
        }
        GFXSetCurGlobal(v16->halo);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.5);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 0xFF962BDD);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 0.85000002f);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v8);
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v8);
        AI_EvalFunc_GFXLifeTimeSet_C(AI_gpst_CurrentGameObject, v16->halo, 0.1f);
        a1 = AI_EvalFunc_GFXAdd_C_CURRENT(9);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 2, 1);
        v6 = (tGameObjectMain*)frisbee_fr_rebound_SFX_light_smoke_gao;
        if (frisbee_fr_rebound_SFX_light_smoke_gao)
        {
            if ((unsigned int)frisbee_fr_rebound_SFX_light_smoke_gao <= 2)
                v6 = AI_MainActor[frisbee_fr_rebound_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, v6, 5);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, 0, 5);
        }
        GFXSetCurGlobal(a1);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9000, 1.570796f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9001, 3.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9002, 6.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9003, 0.94999999f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9004, 0.2f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9005, 0.40000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9006, 0.80000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9007, 2.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9008, 0.0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9101, 0xFFF68BF0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9100, 20);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9102, 24);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v18);
        AI_EvalFunc_GFXSetv_C_CURGLOB(9200, &v18);
        AI_EvalFunc_GFXSetv_C_CURGLOB(9201, &v16->v_direction);
        v10.x = 0.0;
        v10.y = 0.0;
        v10.z = -2.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(9202, &v10);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9009, 0.30000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9010, 0.60000002f);
        AI_EvalFunc_OBJRotateLocalX_C(AI_gpst_CurrentGameObject, 0.1f);
        AI_gpst_CurrentInstance->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -0.5;
        goto LABEL_29;
    }
LABEL_32:
    if (AI_EvalKeyword_During_C())
    {
        AI_EvalFunc_MATHVecGlobalToLocal_C(AI_gpst_CurrentGameObject, &v16->v_axe, &v11);
        AI_EvalFunc_OBJRotateAround_C(AI_gpst_CurrentGameObject, &v11, 0.75);
        v13 = AI_EvalFunc_OBJ_ZoomGet_C(AI_gpst_CurrentGameObject);
        zooma = v13 * 0.92000002f;
        AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, zooma);
        *AI_gppst_CurrentJumpNode = (tAINode*)5;
    }
    else
    {
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        *AI_gppst_CurrentJumpNode = (tAINode*)6;
        TIME_Wait_C(0.1f);
    }
    return 0;
}

int frisbee_fr_init()
{
    int result; // eax
    tScriptStruct_Frisbee* v1; // [esp+0h] [ebp-100h]
    tGameObjectMain* v2; // [esp+8h] [ebp-F8h]
    tGameObjectMain* v3; // [esp+10h] [ebp-F0h]
    Vec3 a3; // [esp+18h] [ebp-E8h] BYREF
    void* Src; // [esp+24h] [ebp-DCh]
    int v6; // [esp+28h] [ebp-D8h]
    int v7; // [esp+2Ch] [ebp-D4h]
    int v8; // [esp+30h] [ebp-D0h]
    Vec3 v9; // [esp+34h] [ebp-CCh] BYREF
    Vec3 a1; // [esp+50h] [ebp-B0h] BYREF
    Vec3 v11; // [esp+5Ch] [ebp-A4h] BYREF
    Vec3 a2; // [esp+78h] [ebp-88h] BYREF
    tGameObjectMain* v13; // [esp+84h] [ebp-7Ch]
    Vec3 v14; // [esp+94h] [ebp-6Ch] BYREF
    tMessage v15; // [esp+A0h] [ebp-60h] BYREF
    int v16; // [esp+D8h] [ebp-28h]
    tScriptStruct_Frisbee* v17; // [esp+DCh] [ebp-24h]
    Vec3 dest; // [esp+E0h] [ebp-20h] BYREF
    BOOL v19; // [esp+ECh] [ebp-14h]
    Vec3 v20; // [esp+F0h] [ebp-10h] BYREF
    int v21; // [esp+FCh] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Frisbee*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v17 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        frisbee_fr_init__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        frisbee_fr_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &frisbee_fr_init_GST_Global_gao_i_projectile_ami_nbr,
            (void*)frisbee_fr_init_GST_Global_gao,
            3564);
        AI2C_ResolveBuffer(&frisbee_fr_init_GST_Global_gao_oa_projectile_ami, (void*)frisbee_fr_init_GST_Global_gao, 3568);
        frisbee_fr_init_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        frisbee_fr_init__PJ_Camera_gao = AI_ul_RealResolveOneRefObject(0x71003F56u);
        fct_frisbee_fr_init_frisbee_fr_before_display = (int)AI_ul_RealResolveOneRefFunction(0x9E0076FD);
        fct_frisbee_fr_init_frisbee_fr_jet = (int)AI_ul_RealResolveOneRefFunction(0x9E001C09);
    }
    AI_EvalFunc_OBJDrawMaskSet_C(AI_gpst_CurrentGameObject, 8, 0);
    AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0x40, 0);
    v9.x = 0.0;
    v9.y = 0.0;
    v9.z = 1.0;
    AI_EvalFunc_OBJ_BankingSet_C(AI_gpst_CurrentGameObject, &v9);
    AI_EvalFunc_COLCrossableSet_C(AI_gpst_CurrentGameObject, 0x502B, 0x230);
    v16 = AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 5, 0);
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 1);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v17->v_start_pos);
    v6 = AI_EvalFunc_OBJ_CapaGet_C((tGameObjectMain*)frisbee_fr_init__PJ_Sally_gao);
    v17->i_start_etage = v6 & 0x4000;
    Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v15, Src, sizeof(v15));
    if (v15.msgID != 9)
        goto LABEL_19;
    *(uint32_t*)(frisbee_fr_init_GST_Global_gao_oa_projectile_ami
        + 4 * (*(uint32_t*)frisbee_fr_init_GST_Global_gao_i_projectile_ami_nbr)++
        + 4) = (uint32_t)AI_gpst_CurrentGameObject;
    v19 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
    v8 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(0, -1.0);
    v17->o_jeter_surki = (int)v15.gao2;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v14);
    MATH_SubstractVec(&dest, &v15.v1, &v14);
    AI_EvalFunc_MATHVecNormalize_C(&dest, &v17->v_sens);
    a2.x = v17->v_sens.x * 20.0f;
    a2.y = v17->v_sens.y * 20.0f;
    a2.z = v17->v_sens.z * 20.0f;
    v7 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &a2);
    v20.x = 0.0;
    v20.y = 0.0;
    v20.z = 1.0;
    AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v17->v_sens, &v20);
    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v17->v_axe);
    AI_EvalFunc_OBJRotateLocalX_C(AI_gpst_CurrentGameObject, 0.1f);
    v17->GFX_Halo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
    AI_EvalFunc_GFXFlagSet_C_CURRENT(v17->GFX_Halo, 4, 1);
    v3 = (tGameObjectMain*)frisbee_fr_init_SFX_light_smoke_gao;
    if (frisbee_fr_init_SFX_light_smoke_gao)
    {
        if ((unsigned int)frisbee_fr_init_SFX_light_smoke_gao <= 2)
            v3 = AI_MainActor[frisbee_fr_init_SFX_light_smoke_gao - 1];
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v17->GFX_Halo, v3, 23);
    }
    else
    {
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v17->GFX_Halo, 0, 23);
    }
    GFXSetCurGlobal(v17->GFX_Halo);
    AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 2.0);
    AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
    AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 0.0);
    AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 0x11EBB6EF);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v11);
    AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)frisbee_fr_init__PJ_Camera_gao, &a3);
    MATH_VecAddVec(&a1, &v11, &a3);
    AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &a1);
    v17->f_time_life = TIM_gf_MainClock - TIM_gf_LockTime;
    v13 = AI_gpst_CurrentGameObject;
    v2 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v2 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v2,
            0,
            (tAIFunction*)fct_frisbee_fr_init_frisbee_fr_before_display);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(AI_gpst_CurrentGameObject, 0, 0, (tAIFunction*)fct_frisbee_fr_init_frisbee_fr_before_display);
    }
    v21 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_frisbee_fr_init_frisbee_fr_jet);
    if (v21 == 1000)
        LABEL_19:
    result = 1000;
    else
        result = v21;
    return result;
}

int frisbee_fr_destroy()
{
    int result; // eax
    int v1; // eax
    tScriptStruct_Frisbee* v2; // [esp+4h] [ebp-50h]
    tGameObjectMain* v3; // [esp+Ch] [ebp-48h]
    tGameObjectMain* v4; // [esp+14h] [ebp-40h]
    Vec3 v5; // [esp+18h] [ebp-3Ch] BYREF
    BOOL v6; // [esp+28h] [ebp-2Ch]
    Vec3 v7; // [esp+2Ch] [ebp-28h] BYREF
    Vec3 v8; // [esp+38h] [ebp-1Ch] BYREF
    tScriptStruct_Frisbee* v9; // [esp+48h] [ebp-Ch]
    int a1; // [esp+4Ch] [ebp-8h]
    int v11; // [esp+50h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_Frisbee*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v9 = v2;
    a1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        frisbee_fr_destroy_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    if (*AI_gppst_CurrentJumpNode)
    {
        if (*AI_gppst_CurrentJumpNode == (tAINode*)1)
        {
            v1 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            v11 = v1;
            if (v1 == 1000)
                result = 1000;
            else
                result = v11;
        }
        else
        {
            v6 = AI_EvalFunc_DYNOff_C(AI_gpst_CurrentGameObject);
            AI_EvalFunc_OBJDrawMaskSet_C(AI_gpst_CurrentGameObject, 0, 8);
            v9->halo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v9->halo, 4, 1);
            v4 = (tGameObjectMain*)frisbee_fr_destroy_SFX_light_smoke_gao;
            if (frisbee_fr_destroy_SFX_light_smoke_gao)
            {
                if ((unsigned int)frisbee_fr_destroy_SFX_light_smoke_gao <= 2)
                    v4 = AI_MainActor[frisbee_fr_destroy_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v9->halo, v4, 0);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v9->halo, 0, 0);
            }
            GFXSetCurGlobal(v9->halo);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.1f);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21101, -1);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 0.5);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v8);
            AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v8);
            a1 = AI_EvalFunc_GFXAdd_C_CURRENT(9);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 2, 1);
            v3 = (tGameObjectMain*)frisbee_fr_destroy_SFX_light_smoke_gao;
            if (frisbee_fr_destroy_SFX_light_smoke_gao)
            {
                if ((unsigned int)frisbee_fr_destroy_SFX_light_smoke_gao <= 2)
                    v3 = AI_MainActor[frisbee_fr_destroy_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, v3, 5);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, 0, 5);
            }
            GFXSetCurGlobal(a1);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9000, 1.570796f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9001, 2.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9002, 4.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9003, 0.94999999f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9004, 0.1f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9005, 0.40000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9006, 0.80000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9007, 2.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9008, 0.0049999999f);
            AI_EvalFunc_GFXSeti_C_CURGLOB(9100, 10);
            AI_EvalFunc_GFXSeti_C_CURGLOB(9102, 8);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v5);
            AI_EvalFunc_GFXSetv_C_CURGLOB(9200, &v5);
            AI_EvalFunc_GFXSetv_C_CURGLOB(9201, &v9->v_direction);
            v7.x = 0.0;
            v7.y = 0.0;
            v7.z = -5.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(9202, &v7);
            *AI_gppst_CurrentJumpNode = 0;
            TIME_Wait_C(0.1f);
            result = 0;
        }
    }
    else
    {
        AI_EvalFunc_GFXDel_C_CURRENT(v9->halo);
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        *AI_gppst_CurrentJumpNode = (tAINode*)1;
        TIME_Wait_C(1.2f);
        result = 0;
    }
    return result;
}

int frisbee_fr_impact()
{
    int result; // eax
    float v1; // [esp+0h] [ebp-78h]
    tScriptStruct_Frisbee* v2; // [esp+8h] [ebp-70h]
    tGameObjectMain* v3; // [esp+10h] [ebp-68h]
    tGameObjectMain* v4; // [esp+18h] [ebp-60h]
    Vec3 v5; // [esp+20h] [ebp-58h] BYREF
    float v6; // [esp+2Ch] [ebp-4Ch]
    float v7; // [esp+30h] [ebp-48h]
    Vec3 a3; // [esp+34h] [ebp-44h] BYREF
    BOOL v9; // [esp+40h] [ebp-38h]
    Vec3 a1; // [esp+48h] [ebp-30h] BYREF
    Vec3 v11; // [esp+54h] [ebp-24h] BYREF
    float v12; // [esp+60h] [ebp-18h]
    tScriptStruct_Frisbee* v13; // [esp+68h] [ebp-10h]
    int v14; // [esp+6Ch] [ebp-Ch]
    float v15; // [esp+70h] [ebp-8h]
    float v16; // [esp+74h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_Frisbee*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v13 = v2;
    v14 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        frisbee_fr_impact_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)3)
    {
        result = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (result != 1000)
            return result;
    }
    else
    {
        if (*AI_gppst_CurrentJumpNode != (tAINode*)4)
        {
            if (v13->GFX_Halo != -1)
            {
                AI_EvalFunc_GFXDel_C_CURRENT(v13->GFX_Halo);
                v13->GFX_Halo = -1;
            }
            v9 = AI_EvalFunc_DYNOff_C(AI_gpst_CurrentGameObject);
            AI_EvalFunc_OBJDrawMaskSet_C(AI_gpst_CurrentGameObject, 0, 8);
            v14 = AI_EvalFunc_GFXAdd_C_CURRENT(9);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v14, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v14, 2, 1);
            v4 = (tGameObjectMain*)frisbee_fr_impact_SFX_light_smoke_gao;
            if (frisbee_fr_impact_SFX_light_smoke_gao)
            {
                if ((unsigned int)frisbee_fr_impact_SFX_light_smoke_gao <= 2)
                    v4 = AI_MainActor[frisbee_fr_impact_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v14, v4, 5);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v14, 0, 5);
            }
            GFXSetCurGlobal(v14);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9000, 1.570796f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9001, 3.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9002, 6.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9003, 0.94999999f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9004, 0.2f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9005, 0.40000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9006, 0.80000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9007, 2.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9008, 0.0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(9101, 0xFFF68BF0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(9100, 20);
            AI_EvalFunc_GFXSeti_C_CURGLOB(9102, 24);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v11);
            AI_EvalFunc_GFXSetv_C_CURGLOB(9200, &v11);
            AI_EvalFunc_GFXSetv_C_CURGLOB(9201, &v13->v_direction);
            v5.x = 0.0;
            v5.y = 0.0;
            v5.z = -2.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(9202, &v5);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9009, 0.30000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9010, 0.60000002f);
        }
        if (v13->f_coef < 0.1)
        {
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(1, 0);
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
            *AI_gppst_CurrentJumpNode = (tAINode*)3;
            TIME_Wait_C(0.5);
            return 0;
        }
    }
    v13->halo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
    AI_EvalFunc_GFXFlagSet_C_CURRENT(v13->halo, 4, 1);
    v3 = (tGameObjectMain*)frisbee_fr_impact_SFX_light_smoke_gao;
    if (frisbee_fr_impact_SFX_light_smoke_gao)
    {
        if ((unsigned int)frisbee_fr_impact_SFX_light_smoke_gao <= 2)
            v3 = AI_MainActor[frisbee_fr_impact_SFX_light_smoke_gao - 1];
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v13->halo, v3, 0);
    }
    else
    {
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v13->halo, 0, 0);
    }
    GFXSetCurGlobal(v13->halo);
    AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.1f);
    AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
    AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 0xFFF68BF0);
    v15 = AI_EvalFunc_MATHRandFloat_C(0.2f, 0.60000002f);
    v1 = v13->f_coef * v15;
    AI_EvalFunc_GFXSetf_C_CURGLOB(21000, v1);
    v7 = AI_EvalFunc_MATHRandFloat_C(-0.15000001f, 0.15000001f);
    v12 = AI_EvalFunc_MATHRandFloat_C(-0.15000001f, 0.15000001f);
    v16 = AI_EvalFunc_MATHRandFloat_C(-0.15000001f, 0.15000001f);
    a3.x = v7;
    a3.y = v12;
    a3.z = v16;
    MATH_VecAddVec(&a1, &v13->v_impact, &a3);
    AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &a1);
    AI_EvalFunc_GFXLifeTimeSet_C(AI_gpst_CurrentGameObject, v13->halo, 0.1f);
    v6 = TIM_gf_dt;
    v13->f_coef = v13->f_coef - TIM_gf_dt * 3.0f;
    *AI_gppst_CurrentJumpNode = (tAINode*)4;
    return 0;
}

int frisbee_fr_exec_GFX_Plouf()
{
    float v1; // [esp+0h] [ebp-ECh]
    tScriptStruct_Frisbee* v2; // [esp+4h] [ebp-E8h]
    tGameObjectMain* v3; // [esp+Ch] [ebp-E0h]
    tGameObjectMain* v4; // [esp+14h] [ebp-D8h]
    Vec3 a3; // [esp+30h] [ebp-BCh] BYREF
    Vec3 a2; // [esp+3Ch] [ebp-B0h] BYREF
    tMessage Src; // [esp+48h] [ebp-A4h] BYREF
    float v8; // [esp+7Ch] [ebp-70h]
    Vec3 pos; // [esp+80h] [ebp-6Ch] BYREF
    Vec3 v10; // [esp+8Ch] [ebp-60h] BYREF
    Vec3 v11; // [esp+98h] [ebp-54h] BYREF
    tGameObjectMain* v12; // [esp+A4h] [ebp-48h]
    Vec3 v13; // [esp+ACh] [ebp-40h] BYREF
    Vec3 v14; // [esp+B8h] [ebp-34h] BYREF
    Vec3 v15; // [esp+C8h] [ebp-24h] BYREF
    tScriptStruct_Frisbee* v16; // [esp+D4h] [ebp-18h]
    int v17; // [esp+D8h] [ebp-14h]
    float v18; // [esp+E8h] [ebp-4h]

    pos.x = 1.0;
    pos.y = 0.0;
    pos.z = 0.0;
    v13.x = 1.0;
    v13.y = 0.0;
    v13.z = 0.0;
    v11.x = 1.0;
    v11.y = 0.0;
    v11.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_Frisbee*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v16 = v2;
    v12 = 0;
    v17 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        frisbee_fr_exec_GFX_Plouf_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        frisbee_fr_exec_GFX_Plouf_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
    }
    v16->i_sound = 666;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v10);
    pos.x = v10.x;
    pos.y = v10.y;
    pos.z = v10.z + 0.050000001f;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v15);
    v11 = v15;
    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v14);
    v13 = v14;
    a2.x = 0.0;
    a2.y = 0.0;
    a2.z = -1.0;
    a3.x = 0.0;
    a3.y = 1.0;
    a3.z = 0.0;
    AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a2, &a3);
    v17 = AI_EvalFunc_GFXAdd_C_CURRENT(20);
    v4 = (tGameObjectMain*)frisbee_fr_exec_GFX_Plouf_SFX_light_smoke_gao;
    if (frisbee_fr_exec_GFX_Plouf_SFX_light_smoke_gao)
    {
        if ((unsigned int)frisbee_fr_exec_GFX_Plouf_SFX_light_smoke_gao <= 2)
            v4 = AI_MainActor[frisbee_fr_exec_GFX_Plouf_SFX_light_smoke_gao - 1];
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v17, v4, 16);
    }
    else
    {
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v17, 0, 16);
    }
    v18 = AI_EvalFunc_MATHRandFloat_C(1.0, 2.0);
    GFXSetCurGlobal(v17);
    AI_EvalFunc_GFXSetf_C_CURGLOB(20000, v18);
    v8 = AI_EvalFunc_MATHRandFloat_C(0.80000001f, 1.2f);
    v1 = v8 + 1.5f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(20001, v1);
    AI_EvalFunc_GFXSetf_C_CURGLOB(20002, 0.0);
    AI_EvalFunc_GFXSetf_C_CURGLOB(20003, 0.0);
    AI_EvalFunc_GFXSeti_C_CURGLOB(20100, 1);
    AI_EvalFunc_GFXSeti_C_CURGLOB(20100, -2);
    AI_EvalFunc_GFXSeti_C_CURGLOB(20101, 0x808080);
    AI_EvalFunc_GFXSeti_C_CURGLOB(20102, 0);
    AI_EvalFunc_GFXSetv_C_CURGLOB(20200, &pos);
    AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v11, &v13);
    v12 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)frisbee_fr_exec_GFX_Plouf_SFX_explosion_gao, &pos);
    Src.msgID = 1;
    Src.Int1 = 38;
    Src.f2 = 0.25;
    Src.f1 = 0.0;
    Src.v1.x = 0.0;
    Src.v1.y = 0.0;
    Src.v1.z = 0.0;
    v3 = v12;
    if (v12)
    {
        if ((unsigned int)v12 <= 2)
            v3 = AI_MainActor[(uint32_t)v12 - 1];
        AI_EvalFunc_MSGSend_C(v3, &Src);
    }
    else
    {
        AI_EvalFunc_MSGSend_C(0, &Src);
    }
    return 1000;
}

int frisbee_fr_disparition()
{
    int result; // eax
    tScriptStruct_Frisbee* v1; // [esp+4h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Frisbee*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (*AI_gppst_CurrentJumpNode == (tAINode*)1)
    {
        result = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (result == 1000)
            result = 1000;
    }
    else
    {
        if (v1->GFX_Halo != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v1->GFX_Halo);
            v1->GFX_Halo = -1;
        }
        AI_EvalFunc_GFXDel_C_CURRENT(v1->halo);
        AI_EvalFunc_GFXDel_C_CURRENT(v1->GFX_Lightning_1);
        AI_EvalFunc_GFXDel_C_CURRENT(v1->GFX_Lightning_2);
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        *AI_gppst_CurrentJumpNode = (tAINode*)1;
        TIME_Wait_C(0.1f);
        result = 0;
    }
    return result;
}

int frisbee_fr_before_display()
{
    tScriptStruct_Frisbee* v1; // [esp+8h] [ebp-30h]
    float v2; // [esp+Ch] [ebp-2Ch]
    float v3; // [esp+10h] [ebp-28h]
    Vec3 a1; // [esp+1Ch] [ebp-1Ch] BYREF
    Vec3 a2; // [esp+28h] [ebp-10h] BYREF
    tScriptStruct_Frisbee* v6; // [esp+34h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Frisbee*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v6 = v1;
    if (AI_EvalFunc_TIMEElapsed_C(v1->f_time_life, 0.02f))
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
        MATH_VecAddVec(&a1, &a2, &v6->v_sens);
        GFXSetCurGlobal(v6->GFX_Halo);
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &a1);
        v2 = 5.0f * TIM_gf_dt;
        if (v2 < 0.0)
            v2 = 0.0;
        if (v2 > 1.0)
            v2 = 1.0;
        v3 = 0.15000001f * v2 + v6->f_size * (1.0f - v2);
        v6->f_size = v3;
        AI_EvalFunc_GFXSetf_C_CURGLOB(21000, v6->f_size);
    }
    else
    {
        v6->f_size = 0.0;
    }
    return 1000;
}

int frisbee_fr_jet()
{
    BOOL v0; // eax
    tGameObjectMain* v1; // eax
    int v2; // eax
    tGameObjectMain* v3; // eax
    int v4; // eax
    int result; // eax
    int v6; // eax
    int v7; // eax
    Vec3* v8; // eax
    int v9; // eax
    float v10; // [esp+Ch] [ebp-410h]
    float v11; // [esp+10h] [ebp-40Ch]
    float v12; // [esp+14h] [ebp-408h]
    float v13; // [esp+18h] [ebp-404h]
    float v14; // [esp+1Ch] [ebp-400h]
    tScriptStruct_Frisbee* v15; // [esp+20h] [ebp-3FCh]
    tGameObjectMain* v16; // [esp+2Ch] [ebp-3F0h]
    tGameObjectMain* v17; // [esp+30h] [ebp-3ECh]
    tGameObjectMain* v18; // [esp+38h] [ebp-3E4h]
    tGameObjectMain* v19; // [esp+40h] [ebp-3DCh]
    Vec3* v20; // [esp+50h] [ebp-3CCh]
    float v21; // [esp+60h] [ebp-3BCh]
    tGameObjectMain* v22; // [esp+70h] [ebp-3ACh]
    tGameObjectMain* v23; // [esp+74h] [ebp-3A8h]
    tGameObjectMain* v24; // [esp+78h] [ebp-3A4h]
    tGameObjectMain* v25; // [esp+84h] [ebp-398h]
    tGameObjectMain* v26; // [esp+88h] [ebp-394h]
    tGameObjectMain* v27; // [esp+90h] [ebp-38Ch]
    tGameObjectMain* v28; // [esp+9Ch] [ebp-380h]
    tGameObjectMain* v29; // [esp+A0h] [ebp-37Ch]
    tGameObjectMain* v30; // [esp+B0h] [ebp-36Ch]
    tGameObjectMain* v31; // [esp+B8h] [ebp-364h]
    tGameObjectMain* v32; // [esp+BCh] [ebp-360h]
    tGameObjectMain* v33; // [esp+C0h] [ebp-35Ch]
    float v34; // [esp+C4h] [ebp-358h]
    tGameObjectMain* v35; // [esp+ECh] [ebp-330h]
    tGameObjectMain* v36; // [esp+F0h] [ebp-32Ch]
    tGameObjectMain* v37; // [esp+F8h] [ebp-324h]
    tGameObjectMain* v38; // [esp+100h] [ebp-31Ch]
    tGameObjectMain* v39; // [esp+108h] [ebp-314h]
    tGameObjectMain* v40; // [esp+110h] [ebp-30Ch]
    float v41; // [esp+114h] [ebp-308h]
    float v42; // [esp+118h] [ebp-304h]
    float v43; // [esp+11Ch] [ebp-300h]
    Vec3 v44; // [esp+120h] [ebp-2FCh] BYREF
    Vec3 v45; // [esp+12Ch] [ebp-2F0h] BYREF
    tGameObjectMain* v46; // [esp+138h] [ebp-2E4h]
    int v47; // [esp+140h] [ebp-2DCh]
    Vec3 v48; // [esp+144h] [ebp-2D8h] BYREF
    tGameObjectMain* v49; // [esp+150h] [ebp-2CCh]
    unsigned int v50; // [esp+154h] [ebp-2C8h]
    int v51; // [esp+158h] [ebp-2C4h]
    float v52; // [esp+15Ch] [ebp-2C0h]
    tGameObjectMain* a7[16]; // [esp+160h] [ebp-2BCh] BYREF
    Vec3 v54; // [esp+1A0h] [ebp-27Ch]
    int v55; // [esp+1ACh] [ebp-270h]
    BOOL v56; // [esp+1B0h] [ebp-26Ch]
    int v57; // [esp+1B4h] [ebp-268h]
    Vec3 v58; // [esp+1B8h] [ebp-264h] BYREF
    float v59; // [esp+1C4h] [ebp-258h]
    Vec3 v60; // [esp+1CCh] [ebp-250h] BYREF
    Vec3 v61; // [esp+1D8h] [ebp-244h] BYREF
    Vec3 dest; // [esp+1E4h] [ebp-238h] BYREF
    int v63; // [esp+1F4h] [ebp-228h]
    int i; // [esp+1F8h] [ebp-224h]
    int v65; // [esp+1FCh] [ebp-220h]
    Vec3 v66; // [esp+204h] [ebp-218h] BYREF
    Vec3 v67; // [esp+210h] [ebp-20Ch] BYREF
    unsigned int v68; // [esp+21Ch] [ebp-200h]
    Vec3 v69; // [esp+220h] [ebp-1FCh] BYREF
    Vec3 v70; // [esp+22Ch] [ebp-1F0h] BYREF
    Vec3 v71; // [esp+238h] [ebp-1E4h] BYREF
    Vec3 v72; // [esp+244h] [ebp-1D8h] BYREF
    float v73; // [esp+250h] [ebp-1CCh]
    unsigned int v74; // [esp+258h] [ebp-1C4h]
    tGameObjectMain* v75; // [esp+25Ch] [ebp-1C0h]
    Vec3 v76; // [esp+268h] [ebp-1B4h] BYREF
    Vec3 v77; // [esp+274h] [ebp-1A8h] BYREF
    int v78; // [esp+284h] [ebp-198h]
    int v79; // [esp+288h] [ebp-194h]
    float v80; // [esp+29Ch] [ebp-180h]
    int v81; // [esp+2A0h] [ebp-17Ch]
    float v82; // [esp+2A4h] [ebp-178h]
    float v83; // [esp+2ACh] [ebp-170h]
    float v84; // [esp+2B0h] [ebp-16Ch]
    unsigned int v85; // [esp+2B4h] [ebp-168h]
    unsigned int v86; // [esp+2B8h] [ebp-164h]
    Vec3 v87; // [esp+2BCh] [ebp-160h]
    int v88; // [esp+2D4h] [ebp-148h]
    int v89; // [esp+2D8h] [ebp-144h]
    int v90; // [esp+2E0h] [ebp-13Ch]
    Vec3 v91; // [esp+2E4h] [ebp-138h] BYREF
    Vec3 v92; // [esp+2F0h] [ebp-12Ch] BYREF
    Vec3 v93; // [esp+2FCh] [ebp-120h] BYREF
    Vec3 v94; // [esp+308h] [ebp-114h] BYREF
    tMessage Src; // [esp+320h] [ebp-FCh] BYREF
    unsigned int v96; // [esp+354h] [ebp-C8h]
    int v97; // [esp+358h] [ebp-C4h]
    float v98; // [esp+35Ch] [ebp-C0h]
    tScriptStruct_Frisbee* v101; // [esp+3A0h] [ebp-7Ch]
    Vec3 v102; // [esp+3A4h] [ebp-78h] BYREF
    Vec3 v103; // [esp+3B0h] [ebp-6Ch] BYREF
    Vec3 v104; // [esp+3BCh] [ebp-60h] BYREF
    Vec3 v105; // [esp+3C8h] [ebp-54h] BYREF
    Vec3 a3; // [esp+3D4h] [ebp-48h] BYREF
    int v107; // [esp+3E0h] [ebp-3Ch]
    float v108; // [esp+3E4h] [ebp-38h]
    int v109; // [esp+3E8h] [ebp-34h]
    int v110; // [esp+3ECh] [ebp-30h]
    float v111; // [esp+3F0h] [ebp-2Ch]
    float v112; // [esp+3F4h] [ebp-28h]
    float v113; // [esp+3F8h] [ebp-24h]
    float v114; // [esp+3FCh] [ebp-20h]
    Vec3 v115; // [esp+400h] [ebp-1Ch] BYREF
    tGameObjectMain* v116; // [esp+40Ch] [ebp-10h]
    int v117; // [esp+410h] [ebp-Ch]
    int v118; // [esp+414h] [ebp-8h]

    v87.x = 1.0;
    v87.y = 0.0;
    v87.z = 0.0;
    v112 = 1.0;
    v113 = 0.0;
    v114 = 0.0;
    v102.x = 1.0;
    v102.y = 0.0;
    v102.z = 0.0;
    v103.x = 1.0;
    v103.y = 0.0;
    v103.z = 0.0;
    v105.x = 1.0;
    v105.y = 0.0;
    v105.z = 0.0;
    v94.x = 1.0;
    v94.y = 0.0;
    v94.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v15 = (tScriptStruct_Frisbee*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v15 = 0;
    v101 = v15;
    v80 = 0.0;
    v73 = 0.0;
    v96 = 0;
    v78 = 0;
    v97 = 0;
    v85 = 0;
    v111 = 0.0;
    v55 = 0;
    v52 = 0.0;
    i = 0;
    v57 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fctvar_frisbee_fr_jet_UNKNOWN = AI_ul_RealResolveOneRef(42, 0x72002647u);
        frisbee_fr_jet__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        fct_frisbee_fr_jet_frisbee_fr_exec_GFX_Plouf = (int)AI_ul_RealResolveOneRefFunction(0x720060FAu);
        frisbee_fr_jet_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(&frisbee_fr_jet_GST_Global_gao_f_eau_hauteurZ, (void*)frisbee_fr_jet_GST_Global_gao, 2772);
        AI2C_ResolveBuffer(&frisbee_fr_jet_GST_Global_gao_i_projectile_ami_nbr, (void*)frisbee_fr_jet_GST_Global_gao, 3564);
        AI2C_ResolveBuffer(&frisbee_fr_jet_GST_Global_gao_oa_projectile_ami, (void*)frisbee_fr_jet_GST_Global_gao, 3568);
        AI2C_ResolveBuffer(&frisbee_fr_jet_GST_Global_gao_f_event_sound_time, (void*)frisbee_fr_jet_GST_Global_gao, 704);
        AI2C_ResolveBuffer(&frisbee_fr_jet_GST_Global_gao_i_event_sound_nbr, (void*)frisbee_fr_jet_GST_Global_gao, 872);
        AI2C_ResolveBuffer(&frisbee_fr_jet_GST_Global_gao_v_event_sound_pos, (void*)frisbee_fr_jet_GST_Global_gao, 376);
        AI2C_ResolveBuffer(&frisbee_fr_jet_GST_Global_gao_f_event_sound_size, (void*)frisbee_fr_jet_GST_Global_gao, 620);
        AI2C_ResolveBuffer(&frisbee_fr_jet_GST_Global_gao_o_event_sound_actor, (void*)frisbee_fr_jet_GST_Global_gao, 788);
        AI2C_ResolveBuffer(&frisbee_fr_jet_GST_Global_gao_i_event_sound_id, (void*)frisbee_fr_jet_GST_Global_gao, 876);
        AI2C_ResolveBuffer(&frisbee_fr_jet_GST_Global_gao_i_event_sound_next_id, (void*)frisbee_fr_jet_GST_Global_gao, 960);
        fct_frisbee_fr_jet_frisbee_fr_before_display = (int)AI_ul_RealResolveOneRefFunction(0x9E0076FD);
        fct_frisbee_fr_jet_frisbee_fr_rebound = (int)AI_ul_RealResolveOneRefFunction(0x260015A6u);
        fct_frisbee_fr_jet_frisbee_fr_impact = (int)AI_ul_RealResolveOneRefFunction(0x260015A8u);
        fct_frisbee_fr_jet_frisbee_fr_disparition = (int)AI_ul_RealResolveOneRefFunction(0x72000C9Eu);
    }
    AI_EvalFunc_MATHVecGlobalToLocal_C(AI_gpst_CurrentGameObject, &v101->v_axe, &v67);
    AI_EvalFunc_OBJRotateAround_C(AI_gpst_CurrentGameObject, &v67, 1.0);
    v55 = 0;
    if (AI_EvalFunc_TIMEElapsed_C(v101->f_time_life, 0.02f))
        v78 = AI_EvalFunc_COLZDEZDEListGet_C(AI_gpst_CurrentGameObject, a7, 6, 1, 2, 0, 1);
    else
        v78 = 0;
    while (v78)
    {
        v101->o_PAF = (int)a7[--v78];
        v40 = (tGameObjectMain*)v101->o_PAF;
        if (v40)
        {
            if ((unsigned int)v40 <= 2)
                v40 = AI_MainActor[(uint32_t)v40 - 1];
            v0 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v40, 0x20);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x20);
        }
        v56 = v0;
        if (!v0)
            goto LABEL_23;
        v39 = (tGameObjectMain*)v101->o_PAF;
        if (v39)
        {
            if ((unsigned int)v39 <= 2)
                v39 = AI_MainActor[(uint32_t)v39 - 1];
            v1 = (tGameObjectMain*)AI_EvalFunc_AIIsModel_C(v39, (tAIModel*)fctvar_frisbee_fr_jet_UNKNOWN);
        }
        else
        {
            v1 = (tGameObjectMain*)AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_frisbee_fr_jet_UNKNOWN);
        }
        a7[15] = v1;
        if (v1)
        {
            v101->o_PAF = 0;
        }
        else
        {
        LABEL_23:
            v78 = 0;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v76);
            v38 = (tGameObjectMain*)v101->o_PAF;
            if (v38)
            {
                if ((unsigned int)v38 <= 2)
                    v38 = AI_MainActor[(uint32_t)v38 - 1];
                AI_EvalFunc_OBJGetPos_C(v38, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            MATH_SubstractVec(&dest, &v76, &a3);
            AI_EvalFunc_MATHVecNormalize_C(&dest, &v101->v_direction);
            v101->i_sound = -1;
            v55 = 1;
            v37 = (tGameObjectMain*)v101->o_PAF;
            if (v37)
            {
                if ((unsigned int)v37 <= 2)
                    v37 = AI_MainActor[(uint32_t)v37 - 1];
                v2 = AI_EvalFunc_OBJ_CapaGet_C(v37);
            }
            else
            {
                v2 = AI_EvalFunc_OBJ_CapaGet_C(0);
            }
            v107 = v2;
            if ((v2 & 0x4000) != 0)
                AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x4000, 0);
        }
    }
    if (!v55)
    {
        v68 = AI_EvalFunc_OBJ_FlagsControlGet_C((tGameObjectMain*)frisbee_fr_jet__PJ_Sally_gao);
        v85 = v68 & 0x1000000;
        v86 = AI_EvalFunc_OBJ_FlagsControlSet_C((tGameObjectMain*)frisbee_fr_jet__PJ_Sally_gao, 0x1000000, 0);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v58);
        AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v115);
        v83 = TIM_gf_dt;
        v66.x = v115.x * TIM_gf_dt;
        v66.y = v115.y * TIM_gf_dt;
        v66.z = v115.z * TIM_gf_dt;
        v3 = AI_EvalFunc_COLRayObjectVector_C(AI_gpst_CurrentGameObject, &v58, &v66, -1, 0, 0, 1);
        v101->o_PAF = (int)v3;
        if (!v85)
            v50 = AI_EvalFunc_OBJ_FlagsControlSet_C((tGameObjectMain*)frisbee_fr_jet__PJ_Sally_gao, 0, 0x1000000);
        v36 = (tGameObjectMain*)v101->o_PAF;
        if (v36)
        {
            if ((unsigned int)v36 <= 2)
                v36 = AI_MainActor[(uint32_t)v36 - 1];
            v35 = v36;
        }
        else
        {
            v35 = 0;
        }
        if (v35)
        {
            v4 = AI_EvalFunc_COLGMSoundGet_C(AI_gpst_CurrentGameObject, 0xFFFF);
            v101->i_sound = v4;
            AI_EvalFunc_COLRayPosGet_C(AI_gpst_CurrentGameObject, &v92);
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v92);
            AI_EvalFunc_COLRayNormalGet_C(AI_gpst_CurrentGameObject, &v101->v_direction);
            v117 = AI_EvalFunc_COLGMFlagsGet_C(AI_gpst_CurrentGameObject, 0xFFFF);
            if ((v117 & 0x10) != 0)
            {
                v90 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_frisbee_fr_jet_frisbee_fr_exec_GFX_Plouf);
                if (v90 != 1000)
                    return v90;
            }
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v70);
            v87 = v70;
            AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v69);
            v98 = TIM_gf_dt;
            v54.x = v69.x * TIM_gf_dt;
            v54.y = v69.y * TIM_gf_dt;
            v54.z = v69.z * TIM_gf_dt;
            v94 = v54;
            v105.x = v87.x + v54.x;
            v105.y = v87.y + v54.y;
            v105.z = v87.z + v54.z;
            if (v54.z < 0.0 && *(float*)frisbee_fr_jet_GST_Global_gao_f_eau_hauteurZ + 10.0 > v105.z)
            {
                AI_EvalFunc_WTR_GetDif_C(&v105, &v104);
                v73 = v104.z;
                if (v105.z < (double)v104.z)
                {
                    v101->o_PAF = (int)AI_gpst_CurrentGameObject;
                    v101->v_direction.x = 0.0;
                    v101->v_direction.y = 0.0;
                    v101->v_direction.z = 1.0;
                    v103.x = v87.x;
                    v103.y = v87.y;
                    v103.z = v73;
                    v14 = (float)fabs(v94.z);
                    v111 = v14;
                    v94.z = 0.0;
                    v52 = MATH_VecLen(&v94);
                    if (v52 != 0.0f)
                    {
                        v13 = (float)fabs(v87.z - v73);
                        v108 = v13;
                        v34 = v13 / v111;
                        v94.x = v94.x * v34;
                        v94.y = v94.y * v34;
                        v94.z = v94.z * v34;
                        v103.x = v103.x + v94.x;
                        v103.y = v103.y + v94.y;
                        v103.z = v103.z + v94.z;
                    }
                    AI_EvalFunc_OBJSetPos_C_CURRENT(&v103);
                    v65 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_frisbee_fr_jet_frisbee_fr_exec_GFX_Plouf);
                    if (v65 != 1000)
                        return v65;
                }
            }
        }
        v33 = (tGameObjectMain*)v101->o_PAF;
        if (v33)
        {
            if ((unsigned int)v33 <= 2)
                v33 = AI_MainActor[(uint32_t)v33 - 1];
            v32 = v33;
        }
        else
        {
            v32 = 0;
        }
        if (v32)
        {
            if (AI_EvalFunc_COLZDEZDEListGet_C(AI_gpst_CurrentGameObject, a7, 6, 1, 2, 0, 1))
            {
                v101->o_PAF = (int)a7[0];
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v91);
                v31 = (tGameObjectMain*)v101->o_PAF;
                if (v31)
                {
                    if ((unsigned int)v31 <= 2)
                        v31 = AI_MainActor[(uint32_t)v31 - 1];
                    AI_EvalFunc_OBJGetPos_C(v31, &v48);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v48);
                }
                MATH_SubstractVec(&v61, &v91, &v48);
                AI_EvalFunc_MATHVecNormalize_C(&v61, &v101->v_direction);
                v101->i_sound = -1;
                v55 = 1;
                v30 = (tGameObjectMain*)v101->o_PAF;
                if (v30)
                {
                    if ((unsigned int)v30 <= 2)
                        v30 = AI_MainActor[(uint32_t)v30 - 1];
                    v6 = AI_EvalFunc_OBJ_CapaGet_C(v30);
                }
                else
                {
                    v6 = AI_EvalFunc_OBJ_CapaGet_C(0);
                }
                v109 = v6;
                if ((v6 & 0x4000) != 0)
                    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x4000, 0);
                goto LABEL_87;
            }
            v12 = (float)fabs(v101->v_direction.z);
            v82 = v12;
            if (v12 >= 0.5)
            {
                v97 = AI_EvalFunc_COLGMFlagsGet_C(AI_gpst_CurrentGameObject, 0xFFFF);
                if ((v97 & 0x800000) != 0)
                {
                LABEL_82:
                    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x4000, 0);
                LABEL_87:
                    v55 = 1;
                    goto LABEL_88;
                }
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v71);
                MATH_VecAddVec(&v93, &v71, &v101->v_direction);
                v102 = v93;
                v60.x = 0.0;
                v60.y = 0.0;
                v60.z = -1.0;
                v74 = (uint32_t)AI_EvalFunc_COLRayObjectDist_C(AI_gpst_CurrentGameObject, &v102, &v60, 10.0, -1, 32, 0, 1);
                v29 = (tGameObjectMain*)v74;
                if (v74)
                {
                    if (v74 <= 2)
                        v29 = AI_MainActor[v74 - 1];
                    v28 = v29;
                }
                else
                {
                    v28 = 0;
                }
                if (v28)
                {
                    v118 = AI_EvalFunc_COLGMFlagsGet_C(AI_gpst_CurrentGameObject, 0xFFFF);
                    if ((v118 & 0x800000) != 0)
                        goto LABEL_82;
                }
            }
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x4000);
            goto LABEL_87;
        }
    }
LABEL_88:
    if (v55)
    {
        Src.msgID = 37;
        Src.gao = AI_gpst_CurrentGameObject;
        Src.gao2 = 0;
        Src.Int1 = 1;
        Src.Int2 = 1;
        Src.v1 = v101->v_sens;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&Src.v2);
        Src.f2 = 0.0;
        Src.f1 = 1000.0;
        v27 = (tGameObjectMain*)v101->o_PAF;
        if (v27)
        {
            if ((unsigned int)v27 <= 2)
                v27 = AI_MainActor[(uint32_t)v27 - 1];
            AI_EvalFunc_MSGSend_C(v27, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
        if (-- * (uint32_t*)frisbee_fr_jet_GST_Global_gao_i_projectile_ami_nbr)
        {
            v116 = AI_gpst_CurrentGameObject;
            v26 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v26 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v25 = v26;
            }
            else
            {
                v25 = 0;
            }
            v63 = AI_EvalFunc_ARRObjSearch_C(
                (tGameObjectMain**)(frisbee_fr_jet_GST_Global_gao_oa_projectile_ami + 4),
                10,
                v25);
            v57 = v63;
            *(uint32_t*)(frisbee_fr_jet_GST_Global_gao_oa_projectile_ami + 4 * v63 + 4) = *(uint32_t*)(frisbee_fr_jet_GST_Global_gao_oa_projectile_ami
                + 4
                * *(uint32_t*)frisbee_fr_jet_GST_Global_gao_i_projectile_ami_nbr
                + 4);
            *(uint32_t*)(frisbee_fr_jet_GST_Global_gao_oa_projectile_ami
                + 4 * *(uint32_t*)frisbee_fr_jet_GST_Global_gao_i_projectile_ami_nbr
                + 4) = 0;
        }
        else
        {
            *(uint32_t*)(frisbee_fr_jet_GST_Global_gao_oa_projectile_ami + 4) = 0;
        }
        if (v101->i_sound == -1)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v77);
            v96 = (uint32_t)AI_EvalFunc_COLRayObjectDist_C(AI_gpst_CurrentGameObject, &v77, &v101->v_sens, 4.0, -1, 0, 0, 1);
            v24 = (tGameObjectMain*)v96;
            if (v96)
            {
                if (v96 <= 2)
                    v24 = AI_MainActor[v96 - 1];
                v23 = v24;
            }
            else
            {
                v23 = 0;
            }
            if (v23)
            {
                v7 = AI_EvalFunc_COLGMSoundGet_C(AI_gpst_CurrentGameObject, 0xFFFF);
                v101->i_sound = v7;
            }
            if (v101->i_sound == -1)
                v101->i_sound = 0;
        }
        if (v101->i_sound != -1 && v101->i_sound < 10)
            v47 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(v101->i_sound + 5, -1.0);
        for (i = 0;
            i < 20 && *(float*)AI_EvalVarArray_C2((char*)frisbee_fr_jet_GST_Global_gao_f_event_sound_time, i) != 0.0;
            ++i)
        {
            ;
        }
        if (i != 20)
        {
            ++* (uint32_t*)frisbee_fr_jet_GST_Global_gao_i_event_sound_nbr;
            v8 = (Vec3*)AI_EvalVarArray_C3((char*)frisbee_fr_jet_GST_Global_gao_v_event_sound_pos, i);
            AI_EvalFunc_OBJGetPos_C_CURRENT(v8);
            *(uint32_t*)AI_EvalVarArray_C2((char*)frisbee_fr_jet_GST_Global_gao_f_event_sound_size, i) = 0x41C80000;
            *(uint32_t*)AI_EvalVarArray_C2((char*)frisbee_fr_jet_GST_Global_gao_f_event_sound_time, i) = 0x3D75C28F;
            *(uint32_t*)(frisbee_fr_jet_GST_Global_gao_o_event_sound_actor + 4 * i + 4) = (uint32_t)AI_gpst_CurrentGameObject;
            *(uint32_t*)(frisbee_fr_jet_GST_Global_gao_i_event_sound_id + 4 * i + 4) = (*(uint32_t*)frisbee_fr_jet_GST_Global_gao_i_event_sound_next_id)++;
        }
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, 1);
        v22 = (tGameObjectMain*)v101->o_PAF;
        if (v22)
        {
            if ((unsigned int)v22 <= 2)
                v22 = AI_MainActor[(uint32_t)v22 - 1];
            v9 = AI_EvalFunc_OBJ_FlagsDesignGet_C(v22);
        }
        else
        {
            v9 = AI_EvalFunc_OBJ_FlagsDesignGet_C(0);
        }
        v81 = v9;
        if ((v9 & 8) == 0)
        {
            v59 = v101->v_sens.z * v101->v_direction.z
                + v101->v_sens.y * v101->v_direction.y
                + v101->v_direction.x * v101->v_sens.x;
            v11 = (float)fabs(v59);
            v84 = v11;
            v80 = v11;
            v21 = 2.0f * v11;
            v41 = v21 * v101->v_direction.x;
            v42 = v101->v_direction.y * v21;
            v43 = v101->v_direction.z * v21;
            v112 = v41;
            v113 = v42;
            v114 = v43;
            v20 = &v101->v_sens;
            v101->v_sens.x = v41 + v101->v_sens.x;
            v20->y = v113 + v20->y;
            v20->z = v114 + v20->z;
            if (v101->v_sens.z < 1.2)
                v10 = 1.2f;
            else
                v10 = v101->v_sens.z;
            v101->v_sens.z = v10;
            v45.x = v101->v_sens.x + v101->v_sens.x;
            v45.y = v101->v_sens.y + v101->v_sens.y;
            v45.z = v101->v_sens.z + v101->v_sens.z;
            v110 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v45);
            v51 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 4, 0);
            v44.x = 0.0;
            v44.y = 0.0;
            v44.z = -20.0;
            MATH_VecMultNormScalar(&v72, &v44, 3.0);
            AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v72);
            v46 = AI_gpst_CurrentGameObject;
            v19 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v19 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v19,
                    0,
                    (tAIFunction*)fct_frisbee_fr_jet_frisbee_fr_before_display);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    0,
                    (tAIFunction*)fct_frisbee_fr_jet_frisbee_fr_before_display);
            }
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_frisbee_fr_jet_frisbee_fr_rebound);
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v101->v_impact);
        v75 = AI_gpst_CurrentGameObject;
        v18 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v18 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                v18,
                0,
                (tAIFunction*)fct_frisbee_fr_jet_frisbee_fr_before_display);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                0,
                0,
                (tAIFunction*)fct_frisbee_fr_jet_frisbee_fr_before_display);
        }
        v88 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_frisbee_fr_jet_frisbee_fr_impact);
        if (v88 != 1000)
            return v88;
    }
    if (!AI_EvalFunc_TIMEElapsed_C(v101->f_time_life, 1.5))
        goto LABEL_154;
    AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
    v101->f_time = TIM_gf_MainClock - TIM_gf_LockTime;
    if (-- * (uint32_t*)frisbee_fr_jet_GST_Global_gao_i_projectile_ami_nbr)
    {
        v49 = AI_gpst_CurrentGameObject;
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
        v89 = AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)(frisbee_fr_jet_GST_Global_gao_oa_projectile_ami + 4), 10, v16);
        v57 = v89;
        *(uint32_t*)(frisbee_fr_jet_GST_Global_gao_oa_projectile_ami + 4 * v89 + 4) = *(uint32_t*)(frisbee_fr_jet_GST_Global_gao_oa_projectile_ami
            + 4
            * *(uint32_t*)frisbee_fr_jet_GST_Global_gao_i_projectile_ami_nbr
            + 4);
        *(uint32_t*)(frisbee_fr_jet_GST_Global_gao_oa_projectile_ami
            + 4 * *(uint32_t*)frisbee_fr_jet_GST_Global_gao_i_projectile_ami_nbr
            + 4) = 0;
    }
    else
    {
        *(uint32_t*)(frisbee_fr_jet_GST_Global_gao_oa_projectile_ami + 4) = 0;
    }
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(1, 0);
    v79 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_frisbee_fr_jet_frisbee_fr_disparition);
    if (v79 == 1000)
        LABEL_154:
    result = 1000;
    else
        result = v79;
    return result;
}