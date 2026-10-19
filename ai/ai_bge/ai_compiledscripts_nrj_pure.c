#include "ai_compiledscripts_inc.h"

//nrj_pure_np_timeout
int nrj_pure_np_timeout_SFX_light_smoke_gao;
int fct_nrj_pure_np_timeout_nrj_pure_np_afterrec;

//fct_nrj_pure_np_init
int fct_nrj_pure_np_init_nrj_pure_np_exec_before_display;
int fct_nrj_pure_np_init_nrj_pure_np_exec_after_display;
int fct_nrj_pure_np_init_univVars;
int fct_nrj_pure_np_init_univVars_Perso_ID_actor;
int nrj_pure_np_init_SFX_light_smoke_gao;
int fct_nrj_pure_np_init_nrj_pure_np_air;
int fct_nrj_pure_np_init_nrj_pure_np_reviens;
int fct_nrj_pure_np_init_univVars_Flag_speciaux_sauve;
int fct_nrj_pure_np_init_nrj_pure_np_sol;
int fct_nrj_pure_np_init_nrj_pure_np_afterrec;

//nrj_pure_np_air
int nrj_pure_np_air_univVars;
int nrj_pure_np_air_univVars_Perso_ID_actor;
int fct_nrj_pure_np_air_nrj_pure_np_sol;
int fct_nrj_pure_np_air_GST_Global_gao;
int fct_nrj_pure_np_air_GST_Global_gao_i_nb_materia_on;
int fct_nrj_pure_np_air_nrj_pure_np_afterrec;

//nrj_pure_np_sol
int nrj_pure_np_sol_GST_Global_gao;
int nrj_pure_np_sol_GST_Global_gao_i_nb_materia_on;
int nrj_pure_np_sol_GST_Global_gao_i_nb_materia_around_sally;
int fct_nrj_pure_np_sol_nrj_pure_np_afterrec;
int fct_nrj_pure_np_sol__PJ_Sally_gao;
int fct_nrj_pure_np_sol__PJ_Sally_gao_current_pm_mode;
int nrj_pure_np_sol_GST_Global_gao_o_acteur_cine_actif;
int fct_nrj_pure_np_sol_nrj_pure_np_timeout;
int nrj_pure_np_sol_GST_Global_gao_f_last_materia;
int nrj_pure_np_sol_univVars;
int nrj_pure_np_sol_univVars_Perso_ID_Energie_courante;
int nrj_pure_np_sol_univVars_Perso_ID_Energie_max;
int nrj_pure_np_sol_univVars_f_sally_fric;
int nrj_pure_np_sol_univVars_Flag_speciaux_sauve;

//nrj_pure_np_afterrec
int nrj_pure_np_afterrec_univVars;
int nrj_pure_np_afterrec_univVars_Perso_ID_actor;
int nrj_pure_np_afterrec__PJ_Sally_gao;
int nrj_pure_np_afterrec_GST_Global_gao;
int nrj_pure_np_afterrec_GST_Global_gao_i_nb_materia_around_sally;

//nrj_pure_np_reviens
int nrj_pure_np_reviens_GST_Global_gao;
int nrj_pure_np_reviens_GST_Global_gao_i_nb_materia_on;
int fct_nrj_pure_np_reviens_nrj_pure_np_afterrec;
int fct_nrj_pure_np_reviens_nrj_pure_np_sol;

int nrj_pure_np_timeout()
{
    int result; // eax
    tScriptStruct_NRJPure* v1; // [esp+0h] [ebp-74h]
    tGameObjectMain* v2; // [esp+8h] [ebp-6Ch]
    tGameObjectMain* v3; // [esp+10h] [ebp-64h]
    Vec3 v4; // [esp+14h] [ebp-60h] BYREF
    Vec3 v5; // [esp+20h] [ebp-54h] BYREF
    Vec3 v6; // [esp+2Ch] [ebp-48h] BYREF
    Vec3 v7; // [esp+3Ch] [ebp-38h] BYREF
    Vec3 v8; // [esp+48h] [ebp-2Ch] BYREF
    int v9; // [esp+54h] [ebp-20h]
    Vec3 a1; // [esp+58h] [ebp-1Ch] BYREF
    tScriptStruct_NRJPure* v11; // [esp+64h] [ebp-10h]
    Vec3 v12; // [esp+68h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_NRJPure*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v11 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        nrj_pure_np_timeout_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        fct_nrj_pure_np_timeout_nrj_pure_np_afterrec = AI_ul_RealResolveOneRefFunction(0x9E003F3D);
    }
    v11->GFX = AI_EvalFunc_GFXAdd_C_CURRENT(13);
    GFXSetCurGlobal(v11->GFX);
    AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 100);
    AI_EvalFunc_GFXSeti_C_CURGLOB(13106, 20);
    v3 = (tGameObjectMain*)nrj_pure_np_timeout_SFX_light_smoke_gao;
    if (nrj_pure_np_timeout_SFX_light_smoke_gao)
    {
        if ((unsigned int)nrj_pure_np_timeout_SFX_light_smoke_gao <= 2)
            v3 = AI_MainActor[nrj_pure_np_timeout_SFX_light_smoke_gao - 1];
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v11->GFX, v3, -1);
    }
    else
    {
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v11->GFX, 0, -1);
    }
    GFXSetCurGlobal(v11->GFX);
    AI_EvalFunc_GFXSeti_C_CURGLOB(13101, 0);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
    AI_EvalFunc_GFXSetv_C_CURGLOB(13200, &a1);
    AI_EvalFunc_GFXSeti_C_CURGLOB(13103, 0xFFC280BE);
    AI_EvalFunc_GFXSeti_C_CURGLOB(13104, 0xFFC280BE);
    AI_EvalFunc_GFXSeti_C_CURGLOB(13105, 0);
    AI_EvalFunc_GFXSeti_C_CURGLOB(13107, 0);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13000, -0.2f);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13001, -0.40000001f);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13002, 0.0099999998f);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13003, 2.0);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13004, 4.0);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13005, 0.30000001f);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13006, 0.60000002f);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13007, 0.1f);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.0);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13009, 2.0);
    AI_EvalFunc_GFXSetf_C_CURGLOB(13010, 8.0);
    v8.x = -0.30000001f;
    v8.y = -0.30000001f;
    v8.z = -0.30000001f;
    AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v8);
    v5.x = 0.30000001f;
    v5.y = 0.30000001f;
    v5.z = 0.30000001f;
    AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v5);
    v4.x = 0.090000004f;
    v4.y = 0.090000004f;
    v4.z = 0.090000004f;
    AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v4);
    v7.x = 0.0;
    v7.y = 0.0;
    v7.z = -1.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(13204, &v7);
    v12.x = 0.0;
    v12.y = 0.0;
    v12.z = 0.40000001f;
    AI_EvalFunc_GFXSetv_C_CURGLOB(13205, &v12);
    v6.x = 0.001f;
    v6.y = 0.001f;
    v6.z = 0.001f;
    AI_EvalFunc_GFXSetv_C_CURGLOB(13206, &v6);
    AI_EvalFunc_GFXFlagSet_C_CURRENT(v11->GFX, 0, 1);
    AI_EvalFunc_GFXFlagSet_C_CURRENT(v11->GFX, 2, 1);
    AI_EvalFunc_GFXSetf_C_CURRENT(v11->GFX, 13012, 4.0);
    if (v11->GFX_Halo != -1)
    {
        AI_EvalFunc_GFXDel_C_CURRENT(v11->GFX_Halo);
        v11->GFX_Halo = -1;
    }
    AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
    v2 = (tGameObjectMain*)v11->o_afterrec_gao;
    if (v2)
    {
        if ((unsigned int)v2 <= 2)
            v2 = AI_MainActor[(uint32_t)v2 - 1];
        AI_EvalFunc_AICBDel_C(AI_gpst_CurrentGameObject, v2, 2, (tAIFunction*)fct_nrj_pure_np_timeout_nrj_pure_np_afterrec);
    }
    else
    {
        AI_EvalFunc_AICBDel_C(AI_gpst_CurrentGameObject, 0, 2, (tAIFunction*)fct_nrj_pure_np_timeout_nrj_pure_np_afterrec);
    }
    v9 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
    if (v9 == 1000)
        result = 1000;
    else
        result = v9;
    return result;
}

int nrj_pure_np_exec_after_display()
{
    tScriptStruct_NRJPure* v1; // [esp+0h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_NRJPure*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v1->c_base_constant_1, 1);
    return 1000;
}

int nrj_pure_np_init()
{
    tGameObjectMain* v1; // eax
    float v2; // [esp+4h] [ebp-13Ch]
    tScriptStruct_NRJPure* v3; // [esp+8h] [ebp-138h]
    tGameObjectMain* v4; // [esp+10h] [ebp-130h]
    tGameObjectMain* v5; // [esp+2Ch] [ebp-114h]
    tGameObjectMain* v6; // [esp+30h] [ebp-110h]
    tGameObjectMain* v7; // [esp+38h] [ebp-108h]
    tGameObjectMain* v8; // [esp+40h] [ebp-100h]
    tGameObjectMain* v9; // [esp+48h] [ebp-F8h]
    tGameObjectMain* v10; // [esp+4Ch] [ebp-F4h]
    tGameObjectMain* v11; // [esp+54h] [ebp-ECh]
    tGameObjectMain* v12; // [esp+5Ch] [ebp-E4h]
    float v13; // [esp+60h] [ebp-E0h]
    float v14; // [esp+64h] [ebp-DCh]
    float v15; // [esp+68h] [ebp-D8h]
    float v16; // [esp+6Ch] [ebp-D4h]
    Vec3 v17; // [esp+70h] [ebp-D0h] BYREF
    BOOL v18; // [esp+80h] [ebp-C0h]
    Vec3 v19; // [esp+84h] [ebp-BCh] BYREF
    float v20; // [esp+90h] [ebp-B0h]
    tGameObjectMain* v22; // [esp+9Ch] [ebp-A4h]
    void* Src; // [esp+A0h] [ebp-A0h]
    int v24; // [esp+A4h] [ebp-9Ch]
    int v25; // [esp+ACh] [ebp-94h]
    int a1; // [esp+B0h] [ebp-90h]
    tGameObjectMain* v27; // [esp+B4h] [ebp-8Ch]
    float v28; // [esp+B8h] [ebp-88h]
    int v29; // [esp+C0h] [ebp-80h]
    int v30; // [esp+C4h] [ebp-7Ch]
    tMessage v31; // [esp+D0h] [ebp-70h] BYREF
    int v32; // [esp+104h] [ebp-3Ch]
    float v33; // [esp+108h] [ebp-38h]
    tScriptStruct_NRJPure* v35; // [esp+11Ch] [ebp-24h]
    int v36; // [esp+120h] [ebp-20h]
    tGameObjectMain* v37; // [esp+128h] [ebp-18h]
    Vec3 v38; // [esp+12Ch] [ebp-14h] BYREF
    BOOL v39; // [esp+13Ch] [ebp-4h]

    v17.x = 1.0;
    v17.y = 0.0;
    v17.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_NRJPure*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v35 = v3;
    v22 = 0;
    a1 = 0;
    v39 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_nrj_pure_np_init_nrj_pure_np_exec_before_display = (int)AI_ul_RealResolveOneRefFunction(0x7200564Bu);
        fct_nrj_pure_np_init_nrj_pure_np_exec_after_display = (int)AI_ul_RealResolveOneRefFunction(0x7200564Cu);
        fct_nrj_pure_np_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&fct_nrj_pure_np_init_univVars_Perso_ID_actor, (void*)fct_nrj_pure_np_init_univVars, 988);
        nrj_pure_np_init_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        fct_nrj_pure_np_init_nrj_pure_np_air = (int)AI_ul_RealResolveOneRefFunction(0x9E00079C);
        fct_nrj_pure_np_init_nrj_pure_np_reviens = (int)AI_ul_RealResolveOneRefFunction(0x9E0065B2);
        AI2C_ResolveBuffer(&fct_nrj_pure_np_init_univVars_Flag_speciaux_sauve, (void*)fct_nrj_pure_np_init_univVars, 4488);
        fct_nrj_pure_np_init_nrj_pure_np_sol = (int)AI_ul_RealResolveOneRefFunction(0x9E000793);
        fct_nrj_pure_np_init_nrj_pure_np_afterrec = (int)AI_ul_RealResolveOneRefFunction(0x9E003F3D);
    }
    AI_EvalFunc_AIPrioritySet_C(AI_gpst_CurrentGameObject, 150);
    AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, 1);
    AI_EvalFunc_COLPrioritySet_C(AI_gpst_CurrentGameObject, 1);
    if (v35->f_delay_before_activation != 0.0)
    {
        if (v35->f_time_activation == 0.0)
        {
            v35->f_time_activation = TIM_gf_MainClock - TIM_gf_LockTime;
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        }
        if (!AI_EvalFunc_TIMEElapsed_C(v35->f_time_activation, v35->f_delay_before_activation))
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 4;
        }
    }
    if (v35->i_sound_init)
    {
        a1 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 2, 0x18000080);
        AI_EvalFunc_SNDUltraPlaySound_C(AI_gpst_CurrentGameObject, a1);
        v15 = AI_EvalFunc_MATHRandFloat_C(0.80000001f, 1.0);
        v13 = SND_f_GetVol(a1);
        v2 = v15 * v13;
        SND_SetVol(a1, v2);
    }
    AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
    v27 = AI_gpst_CurrentGameObject;
    v12 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v12 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v12,
            0,
            (tAIFunction*)fct_nrj_pure_np_init_nrj_pure_np_exec_before_display);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            0,
            (tAIFunction*)fct_nrj_pure_np_init_nrj_pure_np_exec_before_display);
    }
    v37 = AI_gpst_CurrentGameObject;
    v11 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v11 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v11,
            1,
            (tAIFunction*)fct_nrj_pure_np_init_nrj_pure_np_exec_after_display);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            1,
            (tAIFunction*)fct_nrj_pure_np_init_nrj_pure_np_exec_after_display);
    }
    v18 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
    v30 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x1001004, 0);
    v36 = AI_EvalFunc_DYNFrictionSet_C(AI_gpst_CurrentGameObject, 0.0);
    v20 = AI_EvalFunc_MATHRandFloat_C(7.0, 9.0);
    v35->f_rot_speed = v20;
    v29 = AI_EvalFunc_MATHRandInt_C(0, 2);
    v35->i_rot_sens = 1 - 2 * v29;
    OBJ_FreezeModifier_C(AI_gpst_CurrentGameObject, 0, -1);
    XMEN_OnOff(AI_gpst_CurrentGameObject, -1, 0);
    v22 = *AI_MainActor;
    v10 = v22;
    if (v22)
    {
        if ((unsigned int)v22 <= 2)
            v10 = AI_MainActor[(uint32_t)v22 - 1];
        v9 = v10;
    }
    else
    {
        v9 = 0;
    }
    if (v9 == *(tGameObjectMain**)(fct_nrj_pure_np_init_univVars_Perso_ID_actor + 36))
    {
        v38.x = 0.0;
        v38.y = 0.0;
        v38.z = -10.0;
        AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v38);
        v8 = v22;
        if (v22)
        {
            if ((unsigned int)v22 <= 2)
                v8 = AI_MainActor[(uint32_t)v22 - 1];
            v1 = AI_EvalFunc_ANICanalGaoGet_C(v8, 0x64u);
        }
        else
        {
            v1 = AI_EvalFunc_ANICanalGaoGet_C(0, 0x64u);
        }
        v35->o_sally_sac = (int)v1;
        v35->f_dist_recupere = 4.0;
        AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, 1.5);
        XMEN_SetSize(AI_gpst_CurrentGameObject, -1, 0, 0.30000001f);
    }
    else
    {
        v19.x = 0.0;
        v19.y = 0.0;
        v19.z = -40.0;
        AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v19);
        v24 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x20000, 0);
        AI_EvalFunc_DYNReboundSet_C(AI_gpst_CurrentGameObject, 0.5);
        v35->o_sally_sac = (int)v22;
        v35->f_dist_recupere = 2500.0;
        AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, 10.0);
        XMEN_SetSize(AI_gpst_CurrentGameObject, -1, 0, 1.0);
    }
    v14 = AI_EvalFunc_MATHRandFloat_C(0.0, 0.5);
    v35->f_duree_add = v35->f_extern_duree_add + v14;
    v35->f_rotosol_speed = AI_EvalFunc_MATHRandFloat_C(0.07f, 0.14f);
    v33 = AI_EvalFunc_MATHRandFloat_C(-1.0, 1.0);
    v28 = AI_EvalFunc_MTHFloatSign_C(v33);
    v35->f_rotosol_speed = v28 * v35->f_rotosol_speed;
    v35->f_oscosol_speed = AI_EvalFunc_MATHRandFloat_C(4.7123895f, 6.283186f);
    v35->GFX_Halo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
    AI_EvalFunc_GFXFlagSet_C_CURRENT(v35->GFX_Halo, 4, 1);
    v7 = (tGameObjectMain*)nrj_pure_np_init_SFX_light_smoke_gao;
    if (nrj_pure_np_init_SFX_light_smoke_gao)
    {
        if ((unsigned int)nrj_pure_np_init_SFX_light_smoke_gao <= 2)
            v7 = AI_MainActor[nrj_pure_np_init_SFX_light_smoke_gao - 1];
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v35->GFX_Halo, v7, 0);
    }
    else
    {
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v35->GFX_Halo, 0, 0);
    }
    GFXSetCurGlobal(v35->GFX_Halo);
    AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
    AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
    AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 1.0);
    Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v31, Src, sizeof(v31));
    if (AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 0x20000))
    {
        AI_EvalFunc_COLPrioritySet_C(AI_gpst_CurrentGameObject, -1);
        if (v31.msgID)
        {
            v35->des_f_quantite_energie = v31.f2;
            v35->des_type = v31.Int1;
            v35->i_mode = v31.Int2;
            v6 = v22;
            if (v22)
            {
                if ((unsigned int)v22 <= 2)
                    v6 = AI_MainActor[(uint32_t)v22 - 1];
                v5 = v6;
            }
            else
            {
                v5 = 0;
            }
            if (v5 == *(tGameObjectMain**)(fct_nrj_pure_np_init_univVars_Perso_ID_actor + 44))
            {
                v35->f_dist_recupere = v31.f1;
                v35->f_dist_recupere = v35->f_dist_recupere * v35->f_dist_recupere;
            }
        }
        v35->i_mode = v31.Int2;
        v35->c_color = 0xFFC280BE;
        AI_EvalFunc_GFXSeti_C_CURRENT(v35->GFX_Halo, 21101, v35->c_color);
        if (v35->i_mode == 666)
        {
            v35->v_dest_pos = v31.v1;
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_nrj_pure_np_init_nrj_pure_np_reviens);
        }
        else
        {
            v17 = v31.v1;
            v32 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v17);
            v16 = TIM_gf_MainClock - TIM_gf_LockTime;
            v35->f_temps = v16;
            v35->i_tourne = 0;
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_nrj_pure_np_init_nrj_pure_np_air);
        }
    }
    else
    {
        if (v35->SF_Materia_prise <= 0)
            v39 = 0;
        else
            v39 = AI_EvalFunc_ARRBitTest_C(
                (int*)(fct_nrj_pure_np_init_univVars_Flag_speciaux_sauve + 4),
                v35->SF_Materia_prise);
        if (v39)
        {
            v25 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v25 != 1000)
                return v25;
        }
        else
        {
            v35->f_time_au_sol = 0.0;
            AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v35->v_pos_sol);
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_nrj_pure_np_init_nrj_pure_np_sol);
        }
    }
    v35->o_afterrec_gao = (int)v22;
    v4 = (tGameObjectMain*)v35->o_afterrec_gao;
    if (v4)
    {
        if ((unsigned int)v4 <= 2)
            v4 = AI_MainActor[(uint32_t)v4 - 1];
        AI_EvalFunc_AICBAdd_C(AI_gpst_CurrentGameObject, v4, 2, (tAIFunction*)fct_nrj_pure_np_init_nrj_pure_np_afterrec);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(AI_gpst_CurrentGameObject, 0, 2, (tAIFunction*)fct_nrj_pure_np_init_nrj_pure_np_afterrec);
    }
    return 1000;
}

int nrj_pure_np_air()
{
    double v0; // st7
    tScriptStruct_NRJPure* v2; // [esp+8h] [ebp-DCh]
    tGameObjectMain* v3; // [esp+10h] [ebp-D4h]
    tGameObjectMain* v4; // [esp+18h] [ebp-CCh]
    tGameObjectMain* v5; // [esp+20h] [ebp-C4h]
    tGameObjectMain* v6; // [esp+28h] [ebp-BCh]
    tGameObjectMain* v7; // [esp+2Ch] [ebp-B8h]
    Vec3 a2; // [esp+30h] [ebp-B4h] BYREF
    int v9; // [esp+3Ch] [ebp-A8h]
    int v10; // [esp+40h] [ebp-A4h]
    int v11; // [esp+44h] [ebp-A0h]
    int v12; // [esp+48h] [ebp-9Ch]
    Vec3 v13; // [esp+4Ch] [ebp-98h] BYREF
    Vec3 v14; // [esp+58h] [ebp-8Ch] BYREF
    float v15; // [esp+64h] [ebp-80h]
    Vec3 v16; // [esp+68h] [ebp-7Ch] BYREF
    int v17; // [esp+74h] [ebp-70h]
    int v18; // [esp+78h] [ebp-6Ch]
    Vec3 a1; // [esp+7Ch] [ebp-68h] BYREF
    int v20; // [esp+94h] [ebp-50h]
    Vec3 v21; // [esp+A4h] [ebp-40h] BYREF
    int v22; // [esp+B0h] [ebp-34h]
    tScriptStruct_NRJPure* v23; // [esp+BCh] [ebp-28h]
    int v24; // [esp+C0h] [ebp-24h]
    Vec3 v25; // [esp+C4h] [ebp-20h] BYREF
    Vec3 a3; // [esp+D0h] [ebp-14h] BYREF
    int v27; // [esp+DCh] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_NRJPure*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v23 = v2;
    v24 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        nrj_pure_np_air_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&nrj_pure_np_air_univVars_Perso_ID_actor, (void*)nrj_pure_np_air_univVars, 988);
        fct_nrj_pure_np_air_nrj_pure_np_sol = (int)AI_ul_RealResolveOneRefFunction(0x9E000793);
        fct_nrj_pure_np_air_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &fct_nrj_pure_np_air_GST_Global_gao_i_nb_materia_on,
            (void*)fct_nrj_pure_np_air_GST_Global_gao,
            4884);
        fct_nrj_pure_np_air_nrj_pure_np_afterrec = (int)AI_ul_RealResolveOneRefFunction(0x9E003F3D);
    }
    v7 = (tGameObjectMain*)v23->o_afterrec_gao;
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
    if (v6 == *(tGameObjectMain**)(nrj_pure_np_air_univVars_Perso_ID_actor + 36))
    {
        AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&a2);
        if (a2.z < 0.0)
            AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 1, 0);
    }
    else
    {
        v5 = (tGameObjectMain*)v23->o_sally_sac;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v5);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
        }
        v15 = (float)v0;
        if (v23->f_dist_recupere <= (double)v15)
        {
            AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 1, 0);
        }
        else
        {
            AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
            v23->f_time_au_sol = TIM_gf_MainClock - TIM_gf_LockTime;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v23->v_pos_sol);
            v10 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_nrj_pure_np_air_nrj_pure_np_sol);
            if (v10 != 1000)
                return v10;
        }
    }
    ++* (uint32_t*)fct_nrj_pure_np_air_GST_Global_gao_i_nb_materia_on;
    v24 = 0;
    v18 = AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x40);
    if (v18 && (AI_EvalFunc_COLNormalGet_C(AI_gpst_CurrentGameObject, 0x40, &a3), a3.z > 0.5))
    {
        v24 = 1;
    }
    else
    {
        v12 = AI_EvalFunc_COLGMatReportGet_C(AI_gpst_CurrentGameObject, 0x10);
        if (v12 == -1)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v21);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v25);
            AI_EvalFunc_WTR_GetDif_C(&v25, &v13);
            if (v21.z < (double)v13.z)
                v24 = 1;
        }
        else
        {
            v24 = 1;
        }
    }
    v20 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x800000, 0);
    if (v24)
    {
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, 1);
        v11 = AI_EvalFunc_COLGMFlagsGet_C(AI_gpst_CurrentGameObject, 0x40);
        if ((v11 & 0x40000000) != 0)
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
            if (v23->GFX_Halo != -1)
            {
                AI_EvalFunc_GFXDel_C_CURRENT(v23->GFX_Halo);
                v23->GFX_Halo = -1;
            }
            v4 = (tGameObjectMain*)v23->o_afterrec_gao;
            if (v4)
            {
                if ((unsigned int)v4 <= 2)
                    v4 = AI_MainActor[(uint32_t)v4 - 1];
                AI_EvalFunc_AICBDel_C(AI_gpst_CurrentGameObject, v4, 2, (tAIFunction*)fct_nrj_pure_np_air_nrj_pure_np_afterrec);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(AI_gpst_CurrentGameObject, 0, 2, (tAIFunction*)fct_nrj_pure_np_air_nrj_pure_np_afterrec);
            }
            v27 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v27 != 1000)
                return v27;
        }
        else
        {
            a1.x = 0.0;
            a1.y = 0.0;
            a1.z = 0.0;
            AI_EvalFunc_DYNTractionSet_C_CURRENT(&a1);
            v14.x = 0.0;
            v14.y = 0.0;
            v14.z = 0.0;
            v17 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v14);
            v9 = AI_EvalFunc_DYNFrictionSet_C(AI_gpst_CurrentGameObject, 0.0);
            v16.x = 0.0;
            v16.y = 0.0;
            v16.z = 0.0;
            AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v16);
        }
        v23->f_time_au_sol = TIM_gf_MainClock - TIM_gf_LockTime;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v23->v_pos_sol);
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_nrj_pure_np_air_nrj_pure_np_sol);
    }
    else if (AI_EvalFunc_TIMEElapsed_C(v23->f_temps, 4.0))
    {
        v3 = (tGameObjectMain*)v23->o_afterrec_gao;
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            AI_EvalFunc_AICBDel_C(AI_gpst_CurrentGameObject, v3, 2, (tAIFunction*)fct_nrj_pure_np_air_nrj_pure_np_afterrec);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(AI_gpst_CurrentGameObject, 0, 2, (tAIFunction*)fct_nrj_pure_np_air_nrj_pure_np_afterrec);
        }
        v22 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v22 != 1000)
            return v22;
    }
    return 1000;
}

int nrj_pure_np_exec_before_display()
{
    tScriptStruct_NRJPure* v1; // [esp+0h] [ebp-14h]
    int a2; // [esp+8h] [ebp-Ch]
    float v3; // [esp+Ch] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_NRJPure*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v1->c_base_constant_1 = AI_EvalFunc_MATGetConstant_C(AI_gpst_CurrentGameObject, 1u);
    v3 = AI_EvalFunc_OBJ_ZoomGet_C(AI_gpst_CurrentGameObject);
    a2 = AI_EvalFunc_COLORBlend_C(0, v1->c_base_constant_1, v3);
    AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, a2, 1);
    return 1000;
}

int nrj_pure_np_sol()
{
    double v1; // st7
    float v2; // [esp+4h] [ebp-A4h]
    float v3; // [esp+4h] [ebp-A4h]
    float v4; // [esp+4h] [ebp-A4h]
    float v5; // [esp+8h] [ebp-A0h]
    float v6; // [esp+Ch] [ebp-9Ch]
    float v7; // [esp+10h] [ebp-98h]
    char* v8; // [esp+14h] [ebp-94h]
    tScriptStruct_NRJPure* v9; // [esp+1Ch] [ebp-8Ch]
    tGameObjectMain* v10; // [esp+28h] [ebp-80h]
    tGameObjectMain* v11; // [esp+2Ch] [ebp-7Ch]
    tGameObjectMain* v12; // [esp+30h] [ebp-78h]
    tGameObjectMain* v13; // [esp+38h] [ebp-70h]
    int v14; // [esp+3Ch] [ebp-6Ch]
    int v15; // [esp+44h] [ebp-64h]
    float v16; // [esp+54h] [ebp-54h]
    float v17; // [esp+60h] [ebp-48h]
    Vec3 v18; // [esp+68h] [ebp-40h] BYREF
    float v19; // [esp+74h] [ebp-34h]
    int v20; // [esp+78h] [ebp-30h]
    int v22; // [esp+80h] [ebp-28h]
    unsigned int v23; // [esp+84h] [ebp-24h]
    float v24; // [esp+88h] [ebp-20h]
    tScriptStruct_NRJPure* v26; // [esp+90h] [ebp-18h]
    float v27; // [esp+98h] [ebp-10h]
    Vec3 a1; // [esp+9Ch] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v9 = (tScriptStruct_NRJPure*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v9 = 0;
    v26 = v9;
    v20 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        nrj_pure_np_sol_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(&nrj_pure_np_sol_GST_Global_gao_i_nb_materia_on, (void*)nrj_pure_np_sol_GST_Global_gao, 4884);
        AI2C_ResolveBuffer(
            &nrj_pure_np_sol_GST_Global_gao_i_nb_materia_around_sally,
            (void*)nrj_pure_np_sol_GST_Global_gao,
            4288);
        fct_nrj_pure_np_sol_nrj_pure_np_afterrec = (int)AI_ul_RealResolveOneRefFunction(0x9E003F3D);
        fct_nrj_pure_np_sol__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(
            &fct_nrj_pure_np_sol__PJ_Sally_gao_current_pm_mode,
            (void*)fct_nrj_pure_np_sol__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(&nrj_pure_np_sol_GST_Global_gao_o_acteur_cine_actif, (void*)nrj_pure_np_sol_GST_Global_gao, 264);
        fct_nrj_pure_np_sol_nrj_pure_np_timeout = (int)AI_ul_RealResolveOneRefFunction(0x26002F35u);
        AI2C_ResolveBuffer(&nrj_pure_np_sol_GST_Global_gao_f_last_materia, (void*)nrj_pure_np_sol_GST_Global_gao, 4292);
        nrj_pure_np_sol_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&nrj_pure_np_sol_univVars_Perso_ID_Energie_courante, (void*)nrj_pure_np_sol_univVars, 1148);
        AI2C_ResolveBuffer(&nrj_pure_np_sol_univVars_Perso_ID_Energie_max, (void*)nrj_pure_np_sol_univVars, 1096);
        AI2C_ResolveBuffer(&nrj_pure_np_sol_univVars_f_sally_fric, (void*)nrj_pure_np_sol_univVars, 1200);
        AI2C_ResolveBuffer(&nrj_pure_np_sol_univVars_Flag_speciaux_sauve, (void*)nrj_pure_np_sol_univVars, 4488);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)5)
    {
        v13 = (tGameObjectMain*)v26->o_afterrec_gao;
        if (v13)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            AI_EvalFunc_AICBDel_C(AI_gpst_CurrentGameObject, v13, 2, (tAIFunction*)fct_nrj_pure_np_sol_nrj_pure_np_afterrec);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(AI_gpst_CurrentGameObject, 0, 2, (tAIFunction*)fct_nrj_pure_np_sol_nrj_pure_np_afterrec);
        }
        v15 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v15 != 1000)
            return v15;
        goto LABEL_24;
    }
    ++* (uint32_t*)nrj_pure_np_sol_GST_Global_gao_i_nb_materia_on;
    if (v26->f_time_disp == 0.0 && v26->des_f_quantite_energie == -1.0)
    {
        if (v26->i_tourne)
        {
            --* (uint32_t*)nrj_pure_np_sol_GST_Global_gao_i_nb_materia_around_sally;
            v26->i_tourne = 0;
        }
        v26->f_time_disp = TIM_gf_MainClock - TIM_gf_LockTime;
        v26->f_size = 0.2f;
    }
    if (v26->f_time_disp != 0.0f)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v26->f_time_disp, 1.0))
        {
            v23 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x200000, 0);
            if (v26->GFX_Halo != -1)
            {
                AI_EvalFunc_GFXDel_C_CURRENT(v26->GFX_Halo);
                v26->GFX_Halo = -1;
            }
            *AI_gppst_CurrentJumpNode = (tAINode*)5;
            TIME_Wait_C(1.0);
            return 0;
        }
    LABEL_24:
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 4;
    }
    if (!AI_EvalFunc_TIMEElapsed_C(v26->f_time_au_sol, 0.30000001f) && !v26->i_face_de_mort)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    if (v26->f_time_au_sol != 0.0)
    {
        if (*(uint32_t*)fct_nrj_pure_np_sol__PJ_Sally_gao_current_pm_mode == 18)
            goto LABEL_37;
        v12 = *(tGameObjectMain**)nrj_pure_np_sol_GST_Global_gao_o_acteur_cine_actif;
        if (*(uint32_t*)nrj_pure_np_sol_GST_Global_gao_o_acteur_cine_actif)
        {
            if ((unsigned int)v12 <= 2)
                v12 = AI_MainActor[(uint32_t)v12 - 1];
            v11 = v12;
        }
        else
        {
            v11 = 0;
        }
        if (v11 || AI_EvalFunc_OBJ_CustomBitsTest_C((tGameObjectMain*)nrj_pure_np_sol_GST_Global_gao, 0x4000))
        {
        LABEL_37:
            v26->f_time_au_sol = TIM_gf_MainClock - TIM_gf_LockTime;
        }
        else
        {
            v2 = v26->f_duree_add + 10.0f;
            if (AI_EvalFunc_TIMEElapsed_C(v26->f_time_au_sol, v2))
            {
                v22 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_nrj_pure_np_sol_nrj_pure_np_timeout);
                if (v22 != 1000)
                    return v22;
            }
        }
    }
    v27 = TIM_gf_dt;
    v3 = v26->f_rotosol_speed * (TIM_gf_dt * 30.0f);
    AI_EvalFunc_OBJRotateLocalZ_c(AI_gpst_CurrentGameObject, v3);
    v10 = (tGameObjectMain*)v26->o_sally_sac;
    if (v10)
    {
        if ((unsigned int)v10 <= 2)
            v10 = AI_MainActor[(uint32_t)v10 - 1];
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v10);
    }
    else
    {
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v16 = (float)v1;
    if (v26->f_dist_recupere > (double)v16 && v26->f_time_init_tourne == 0.0f)
    {
        if (!AI_EvalFunc_TIMEElapsed_C(*(float*)nrj_pure_np_sol_GST_Global_gao_f_last_materia, 0.1f))
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        *(float*)nrj_pure_np_sol_GST_Global_gao_f_last_materia = TIM_gf_MainClock - TIM_gf_LockTime;
        v26->f_time_au_sol = 0.0f;
        if (v26->des_type)
        {
            if (v26->des_type == 1)
                *(uint32_t*)nrj_pure_np_sol_univVars_f_sally_fric += 5;
        }
        else
        {
            v8 = AI_EvalVarArray_C2((char*)nrj_pure_np_sol_univVars_Perso_ID_Energie_courante, 8);
            *(float*)v8 = *(float*)v8 + 20.0f;
            v7 = *(float*)AI_EvalVarArray_C2((char*)nrj_pure_np_sol_univVars_Perso_ID_Energie_courante, 8);
            if (*(float*)AI_EvalVarArray_C2((char*)nrj_pure_np_sol_univVars_Perso_ID_Energie_max, 8) <= (double)v7)
            {
                v6 = *(float*)AI_EvalVarArray_C2((char*)nrj_pure_np_sol_univVars_Perso_ID_Energie_max, 8);
                *(float*)AI_EvalVarArray_C2((char*)nrj_pure_np_sol_univVars_Perso_ID_Energie_courante, 8) = v6;
            }
        }
        if (v26->SF_Materia_prise > 0)
            AI_EvalFunc_ARRBitSet_C((int*)(nrj_pure_np_sol_univVars_Flag_speciaux_sauve + 4), v26->SF_Materia_prise);
        v26->i_face_de_mort = 0;
        a1.x = 0.0;
        a1.y = 0.0;
        a1.z = 0.0;
        AI_EvalFunc_DYNTractionSet_C_CURRENT(&a1);
        v18.x = 0.0;
        v18.y = 0.0;
        v18.z = 0.0;
        AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v18);
        v20 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 1, 0x18000080);
        v14 = SND_l_GetFreq(v20);
        v17 = AI_EvalFunc_MATHRandFloat_C(0.99000001f, 1.0);
        v5 = (float)v14;
        SND_SetFreq(v20, (unsigned __int64)(v5 * v17));
        v24 = SND_f_GetVol(v20);
        v19 = AI_EvalFunc_MATHRandFloat_C(0.80000001f, 1.0);
        v4 = v24 * v19;
        SND_SetVol(v20, v4);
        AI_EvalFunc_SNDUltraPlaySound_C(AI_gpst_CurrentGameObject, v20);
        v26->des_f_quantite_energie = 0.0;
        v26->i_tourne = 1;
        XMEN_OnOff(AI_gpst_CurrentGameObject, 0, -1);
        ++* (uint32_t*)nrj_pure_np_sol_GST_Global_gao_i_nb_materia_around_sally;
    }
    return 1000;
}

int nrj_pure_np_afterrec()
{
    double v0; // st7
    double v1; // st7
    double v2; // st7
    float a4; // [esp+0h] [ebp-314h]
    float a4a; // [esp+0h] [ebp-314h]
    float X; // [esp+4h] [ebp-310h]
    float Xa; // [esp+4h] [ebp-310h]
    float v8; // [esp+8h] [ebp-30Ch]
    float v9; // [esp+Ch] [ebp-308h]
    float v10; // [esp+10h] [ebp-304h]
    float v11; // [esp+14h] [ebp-300h]
    float v12; // [esp+18h] [ebp-2FCh]
    float v13; // [esp+1Ch] [ebp-2F8h]
    tScriptStruct_NRJPure* v14; // [esp+20h] [ebp-2F4h]
    tGameObjectMain* v15; // [esp+28h] [ebp-2ECh]
    tGameObjectMain* v16; // [esp+2Ch] [ebp-2E8h]
    float v17; // [esp+50h] [ebp-2C4h]
    tGameObjectMain* v18; // [esp+58h] [ebp-2BCh]
    tGameObjectMain* v19; // [esp+68h] [ebp-2ACh]
    tGameObjectMain* v20; // [esp+70h] [ebp-2A4h]
    float v21; // [esp+74h] [ebp-2A0h]
    float v22; // [esp+78h] [ebp-29Ch]
    tGameObjectMain* v23; // [esp+8Ch] [ebp-288h]
    tGameObjectMain* v24; // [esp+9Ch] [ebp-278h]
    float v25; // [esp+A0h] [ebp-274h]
    tGameObjectMain* v26; // [esp+B0h] [ebp-264h]
    float v27; // [esp+BCh] [ebp-258h]
    float v28; // [esp+C4h] [ebp-250h]
    tGameObjectMain* v29; // [esp+D8h] [ebp-23Ch]
    tGameObjectMain* v30; // [esp+E0h] [ebp-234h]
    tGameObjectMain* v31; // [esp+E4h] [ebp-230h]
    tGameObjectMain* v32; // [esp+E8h] [ebp-22Ch]
    tGameObjectMain* v33; // [esp+ECh] [ebp-228h]
    tGameObjectMain* v34; // [esp+FCh] [ebp-218h]
    tGameObjectMain* v35; // [esp+104h] [ebp-210h]
    tGameObjectMain* v36; // [esp+10Ch] [ebp-208h]
    tGameObjectMain* v37; // [esp+110h] [ebp-204h]
    float v38; // [esp+114h] [ebp-200h]
    Vec3 v39; // [esp+118h] [ebp-1FCh] BYREF
    float v40; // [esp+124h] [ebp-1F0h]
    float v41; // [esp+128h] [ebp-1ECh]
    Vec3 v42; // [esp+12Ch] [ebp-1E8h] BYREF
    Vec3 v43; // [esp+138h] [ebp-1DCh] BYREF
    float v44; // [esp+144h] [ebp-1D0h]
    Vec3 v45; // [esp+148h] [ebp-1CCh] BYREF
    Vec3 v46; // [esp+154h] [ebp-1C0h] BYREF
    int v47; // [esp+160h] [ebp-1B4h]
    float v48; // [esp+164h] [ebp-1B0h]
    Vec3 v49; // [esp+168h] [ebp-1ACh] BYREF
    Vec3 v50; // [esp+174h] [ebp-1A0h] BYREF
    Vec3 v51; // [esp+180h] [ebp-194h] BYREF
    float v52; // [esp+18Ch] [ebp-188h]
    float v53; // [esp+190h] [ebp-184h]
    float v54; // [esp+194h] [ebp-180h]
    Vec3 v55; // [esp+198h] [ebp-17Ch] BYREF
    float v56; // [esp+1A8h] [ebp-16Ch]
    float v57; // [esp+1ACh] [ebp-168h]
    Vec3 a3; // [esp+1B0h] [ebp-164h] BYREF
    Vec3 v59; // [esp+1BCh] [ebp-158h] BYREF
    float v60; // [esp+1C8h] [ebp-14Ch]
    Vec3 v61; // [esp+1CCh] [ebp-148h] BYREF
    Vec3 v62; // [esp+1D8h] [ebp-13Ch] BYREF
    Vec3 v63; // [esp+1E4h] [ebp-130h] BYREF
    float v64; // [esp+1F0h] [ebp-124h]
    float v65; // [esp+1F4h] [ebp-120h]
    Vec3 v66; // [esp+1F8h] [ebp-11Ch]
    float v67; // [esp+204h] [ebp-110h]
    float v68; // [esp+208h] [ebp-10Ch]
    float v69; // [esp+20Ch] [ebp-108h]
    Vec3 v70; // [esp+210h] [ebp-104h] BYREF
    float v71; // [esp+21Ch] [ebp-F8h]
    Vec3 v72; // [esp+220h] [ebp-F4h] BYREF
    Vec3 a1; // [esp+22Ch] [ebp-E8h] BYREF
    Vec3 v74; // [esp+238h] [ebp-DCh] BYREF
    Vec3 v75; // [esp+244h] [ebp-D0h] BYREF
    Vec3 v76; // [esp+250h] [ebp-C4h] BYREF
    float v77; // [esp+25Ch] [ebp-B8h]
    float v78; // [esp+260h] [ebp-B4h]
    Vec3 v79; // [esp+264h] [ebp-B0h] BYREF
    float v80; // [esp+270h] [ebp-A4h]
    float zoom; // [esp+274h] [ebp-A0h]
    Vec3 v82; // [esp+278h] [ebp-9Ch] BYREF
    Vec3 v83; // [esp+284h] [ebp-90h] BYREF
    float v84; // [esp+290h] [ebp-84h]
    float v85; // [esp+294h] [ebp-80h]
    Vec3 v86; // [esp+298h] [ebp-7Ch] BYREF
    Vec3 v87; // [esp+2A4h] [ebp-70h] BYREF
    float v88; // [esp+2B0h] [ebp-64h]
    float v89; // [esp+2B8h] [ebp-5Ch]
    tScriptStruct_NRJPure* v90; // [esp+2BCh] [ebp-58h]
    Vec3 v91; // [esp+2C4h] [ebp-50h] BYREF
    Vec3 dest; // [esp+2D0h] [ebp-44h] BYREF
    Vec3 v93; // [esp+2DCh] [ebp-38h] BYREF
    Vec3 v94; // [esp+2F0h] [ebp-24h] BYREF
    Vec3 a5; // [esp+2FCh] [ebp-18h] BYREF
    float v97; // [esp+30Ch] [ebp-8h]
    float v98; // [esp+310h] [ebp-4h]

    v55.x = 1.0;
    v55.y = 0.0;
    v55.z = 0.0;
    v75.x = 1.0;
    v75.y = 0.0;
    v75.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v14 = (tScriptStruct_NRJPure*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v14 = 0;
    v90 = v14;
    v41 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        nrj_pure_np_afterrec_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&nrj_pure_np_afterrec_univVars_Perso_ID_actor, (void*)nrj_pure_np_afterrec_univVars, 988);
        nrj_pure_np_afterrec__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        nrj_pure_np_afterrec_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &nrj_pure_np_afterrec_GST_Global_gao_i_nb_materia_around_sally,
            (void*)nrj_pure_np_afterrec_GST_Global_gao,
            4288);
    }
    if (v90->des_f_quantite_energie == -1.0)
    {
        v56 = AI_EvalFunc_OBJ_ZoomGet_C(AI_gpst_CurrentGameObject);
        v88 = TIM_gf_dt;
        v38 = v56 - TIM_gf_dt * 1.0f;
        if (v38 < 0.0000099999997)
            v13 = 0.0000099999997f;
        else
            v13 = v56 - TIM_gf_dt * 1.0f;
        zoom = v13;
        AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, v13);
        if (!v90->i_face_de_mort)
        {
            v37 = (tGameObjectMain*)v90->o_afterrec_gao;
            if (v37)
            {
                if ((unsigned int)v37 <= 2)
                    v37 = AI_MainActor[(uint32_t)v37 - 1];
                v36 = v37;
            }
            else
            {
                v36 = 0;
            }
            if (v36 == *(tGameObjectMain**)(nrj_pure_np_afterrec_univVars_Perso_ID_actor + 36))
            {
                v35 = (tGameObjectMain*)v90->o_sally_sac;
                if (v35)
                {
                    if ((unsigned int)v35 <= 2)
                        v35 = AI_MainActor[(uint32_t)v35 - 1];
                    AI_EvalFunc_OBJGetPos_C(v35, &v46);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v46);
                }
                AI_EvalFunc_OBJSetPos_C_CURRENT(&v46);
            }
            else
            {
                v34 = (tGameObjectMain*)v90->o_afterrec_gao;
                if (v34)
                {
                    if ((unsigned int)v34 <= 2)
                        v34 = AI_MainActor[(uint32_t)v34 - 1];
                    AI_EvalFunc_OBJGetPos_C(v34, &v43);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v43);
                }
                v66.x = 0.0;
                v66.y = 0.0;
                v66.z = 1.5;
                v55.x = v43.x + 0.0f;
                v55.y = v43.y + 0.0f;
                v55.z = v43.z + 1.5f;
                AI_EvalFunc_OBJSetPos_C_CURRENT(&v55);
            }
            v33 = (tGameObjectMain*)v90->o_afterrec_gao;
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
            if (v32 != (tGameObjectMain*)nrj_pure_np_afterrec__PJ_Sally_gao)
                XMEN_OnOff(AI_gpst_CurrentGameObject, -1, 0);
        }
    }
    else if (v90->i_tourne)
    {
        v31 = (tGameObjectMain*)v90->o_afterrec_gao;
        if (v31)
        {
            if ((unsigned int)v31 <= 2)
                v31 = AI_MainActor[(uint32_t)v31 - 1];
            v30 = v31;
        }
        else
        {
            v30 = 0;
        }
        if (v30 == *(tGameObjectMain**)(nrj_pure_np_afterrec_univVars_Perso_ID_actor + 36))
        {
            if (v90->f_time_init_tourne == 0.0)
            {
                v90->f_time_init_tourne = TIM_gf_MainClock - TIM_gf_LockTime;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v50);
                v29 = (tGameObjectMain*)v90->o_afterrec_gao;
                if (v29)
                {
                    if ((unsigned int)v29 <= 2)
                        v29 = AI_MainActor[(uint32_t)v29 - 1];
                    AI_EvalFunc_OBJGetPos_C(v29, &v79);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v79);
                }
                MATH_SubstractVec(&dest, &v50, &v79);
                v90->v_rot_vector = dest;
                v90->v_rot_vector.z = 0.0;
                v0 = MATH_VecLen(&v90->v_rot_vector);
                v90->f_rayon_tourne = (float)v0;
                v90->f_rayon_tourne_init = v90->f_rayon_tourne;
            }
            v77 = AI_EvalFunc_OBJ_ZoomGet_C(AI_gpst_CurrentGameObject);
            v48 = TIM_gf_dt;
            v28 = v77 - TIM_gf_dt * 4.0f;
            if (v28 < 0.5)
                v12 = 0.5;
            else
                v12 = v77 - TIM_gf_dt * 4.0f;
            v98 = v12;
            AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, v12);
            v78 = TIM_gf_dt;
            v27 = 15.0f * TIM_gf_dt + v90->f_blend_tourne;
            if (v27 > (double)v90->f_rot_speed)
                v11 = v90->f_rot_speed;
            else
                v11 = 15.0f * TIM_gf_dt + v90->f_blend_tourne;
            v57 = v11;
            v90->f_blend_tourne = v11;
            v65 = TIM_gf_dt;
            v10 = (float)v90->i_rot_sens;
            v41 = v90->f_blend_tourne * TIM_gf_dt * v10;
            v42.x = 0.0;
            v42.y = 0.0;
            v42.z = 1.0;
            AI_EvalFunc_MATHVecRotate_C(&v90->v_rot_vector, &v42, v41, &v63);
            v90->v_rot_vector = v63;
            v26 = (tGameObjectMain*)v90->o_sally_sac;
            if (v26)
            {
                if ((unsigned int)v26 <= 2)
                    v26 = AI_MainActor[(uint32_t)v26 - 1];
                AI_EvalFunc_OBJGetPos_C(v26, &v82);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v82);
            }
            MATH_VecAddVec(&a1, &v82, &v90->v_rot_vector);
            v55 = a1;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v94);
            v80 = TIM_gf_dt;
            a4 = 8.0f * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlend_C(&v94, &v55, a4, &v59);
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v59);
            v60 = TIM_gf_dt;
            v25 = v90->f_rayon_tourne - (v90->f_blend_tourne + 1.0f) / 3.0f * TIM_gf_dt;
            if (v25 < 0.1)
                v9 = 0.1f;
            else
                v9 = v90->f_rayon_tourne - (v90->f_blend_tourne + 1.0f) / 3.0f * TIM_gf_dt;
            v85 = v9;
            v90->f_rayon_tourne = v9;
            AI_EvalFunc_MATHVecSetNormVector_C(&v90->v_rot_vector, v90->f_rayon_tourne);
            AI_EvalFunc_GFXSetf_C_CURRENT(v90->GFX_Halo, 21000, v90->f_rayon_tourne);
            if (v90->f_rayon_tourne <= 0.1f)
                v90->des_f_quantite_energie = -1.0;
        }
        else
        {
            v24 = (tGameObjectMain*)v90->o_afterrec_gao;
            if (v24)
            {
                if ((unsigned int)v24 <= 2)
                    v24 = AI_MainActor[(uint32_t)v24 - 1];
                AI_EvalFunc_OBJGetPos_C(v24, &v45);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v45);
            }
            v75 = v45;
            v23 = (tGameObjectMain*)v90->o_afterrec_gao;
            if (v23)
            {
                if ((unsigned int)v23 <= 2)
                    v23 = AI_MainActor[(uint32_t)v23 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v23, &v91);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v91);
            }
            v67 = v91.x * 1.5f;
            v68 = v91.y * 1.5f;
            v69 = v91.z * 1.5f;
            v75.x = v75.x + v67;
            v75.y = v75.y + v68;
            v75.z = v75.z + v69;
            if (v90->f_time_init_tourne == 0.0f)
            {
                v90->f_time_init_tourne = TIM_gf_MainClock - TIM_gf_LockTime;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v70);
                MATH_SubstractVec(&v76, &v70, &v75);
                v90->v_rot_vector = v76;
                v1 = MATH_VecLen(&v90->v_rot_vector);
                v90->f_rayon_tourne = (float)v1;
                v90->f_rayon_tourne_init = v90->f_rayon_tourne;
                v90->f_blend_tourne = 0.0;
            }
            v52 = AI_EvalFunc_OBJ_ZoomGet_C(AI_gpst_CurrentGameObject);
            v97 = TIM_gf_dt;
            v22 = v52 - TIM_gf_dt * 3.0f;
            if (v22 < 0.5)
                v8 = 0.5;
            else
                v8 = v52 - TIM_gf_dt * 3.0f;
            v84 = v8;
            AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, v8);
            v44 = TIM_gf_dt;
            v21 = 0.5f * TIM_gf_dt;
            if (v21 < 0.0)
                v21 = 0.0;
            if (v21 > 1.0)
                v21 = 1.0;
            v53 = 10.0f * v21 + v90->f_blend_tourne * (1.0f - v21);
            v90->f_blend_tourne = v53;
            v20 = (tGameObjectMain*)v90->o_afterrec_gao;
            if (v20)
            {
                if ((unsigned int)v20 <= 2)
                    v20 = AI_MainActor[(uint32_t)v20 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v20, &v86);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v86);
            }
            v19 = (tGameObjectMain*)v90->o_afterrec_gao;
            if (v19)
            {
                if ((unsigned int)v19 <= 2)
                    v19 = AI_MainActor[(uint32_t)v19 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v19, &v93);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v93);
            }
            AI_EvalFunc_MATHVecBlend_C(&v86, &v93, 0.75, &v72);
            a3 = v72;
            v18 = (tGameObjectMain*)v90->o_afterrec_gao;
            if (v18)
            {
                if ((unsigned int)v18 <= 2)
                    v18 = AI_MainActor[(uint32_t)v18 - 1];
                AI_EvalFunc_OBJ_HorizonGet_C(v18, &v62);
            }
            else
            {
                AI_EvalFunc_OBJ_HorizonGet_C(0, &v62);
            }
            v17 = (float)v90->i_rot_sens;
            v83.x = v62.x * v17;
            v83.y = v62.y * v17;
            v83.z = v62.z * v17;
            AI_EvalFunc_MATHVecBlend_C(&a3, &v83, 0.5, &v51);
            a3 = v51;
            v64 = TIM_gf_dt;
            a4a = v90->f_blend_tourne * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlendRotate_C(&v90->v_rot_vector, &a3, a4a, &a5);
            v90->v_rot_vector = a5;
            v2 = MATH_VecLen(&v90->v_rot_vector);
            v90->f_rayon_tourne = (float)v2;
            v55 = v75;
            v55.x = v75.x + v90->v_rot_vector.x;
            v55.y = v75.y + v90->v_rot_vector.y;
            v55.z = v75.z + v90->v_rot_vector.z;
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v55);
            X = v90->f_rayon_tourne * 0.5f;
            AI_EvalFunc_GFXSetf_C_CURRENT(v90->GFX_Halo, 21000, X);
            if (v90->f_rayon_tourne <= 1.2f)
            {
                v90->des_f_quantite_energie = -1.0;
                v47 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(AI_gpst_CurrentGameObject, 1, -1.0, 0.99000001f, 1.01f);
            }
        }
    }
    else
    {
        v89 = TIM_gf_dt;
        Xa = 2.0f * TIM_gf_dt;
        AI_EvalFunc_OBJRotateGlobalY_C(AI_gpst_CurrentGameObject, Xa);
        if (!AI_EvalFunc_MTHVecNullEpsilon_C(&v90->v_pos_sol))
        {
            v54 = TIM_gf_dt;
            v90->f_oscosol_angle = v90->f_oscosol_speed * TIM_gf_dt + v90->f_oscosol_angle;
            if (v90->f_oscosol_angle >= 6.283185f)
                v90->f_oscosol_angle = v90->f_oscosol_angle - 6.283185f;
            v16 = (tGameObjectMain*)v90->o_afterrec_gao;
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
            if (v15 == *(tGameObjectMain**)(nrj_pure_np_afterrec_univVars_Perso_ID_actor + 36))
            {
                v71 = sinf(v90->f_oscosol_angle);
                v49.x = 0.0;
                v49.y = 0.0;
                v49.z = -0.125f * v71;
                MATH_VecAddVec(&v61, &v90->v_pos_sol, &v49);
                AI_EvalFunc_OBJSetPos_C_CURRENT(&v61);
            }
            else
            {
                v40 = sinf(v90->f_oscosol_angle);
                v87.x = 0.0;
                v87.y = 0.0;
                v87.z = -2.0f * v40;
                MATH_VecAddVec(&v39, &v90->v_pos_sol, &v87);
                AI_EvalFunc_OBJSetPos_C_CURRENT(&v39);
            }
        }
    }
    v90->i_nb_materia = *(uint32_t*)nrj_pure_np_afterrec_GST_Global_gao_i_nb_materia_around_sally;
    if (v90->GFX_Halo != -1)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v74);
        AI_EvalFunc_GFXSetv_C_CURRENT(v90->GFX_Halo, 21200, &v74);
    }
    return 1000;
}

int nrj_pure_np_reviens()
{
    tScriptStruct_NRJPure* v1; // [esp+0h] [ebp-68h]
    tGameObjectMain* v2; // [esp+8h] [ebp-60h]
    int v3; // [esp+Ch] [ebp-5Ch]
    float a4; // [esp+1Ch] [ebp-4Ch]
    Vec3 a1; // [esp+20h] [ebp-48h] BYREF
    int v6; // [esp+38h] [ebp-30h]
    Vec3 a2; // [esp+40h] [ebp-28h] BYREF
    tScriptStruct_NRJPure* v9; // [esp+4Ch] [ebp-1Ch]
    Vec3 v10; // [esp+50h] [ebp-18h] BYREF
    Vec3 v11; // [esp+5Ch] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_NRJPure*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v9 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        nrj_pure_np_reviens_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &nrj_pure_np_reviens_GST_Global_gao_i_nb_materia_on,
            (void*)nrj_pure_np_reviens_GST_Global_gao,
            4884);
        fct_nrj_pure_np_reviens_nrj_pure_np_afterrec = (int)AI_ul_RealResolveOneRefFunction(0x9E003F3D);
        fct_nrj_pure_np_reviens_nrj_pure_np_sol = (int)AI_ul_RealResolveOneRefFunction(0x9E000793);
    }
    ++* (uint32_t*)nrj_pure_np_reviens_GST_Global_gao_i_nb_materia_on;
    if (v9->i_mode == 666)
    {
        v9->i_mode = 1;
        a4 = AI_EvalFunc_MATHRandFloat_C(0.75, 1.1f);
        AI_EvalFunc_DYNImpulsion_C(AI_gpst_CurrentGameObject, &v9->v_dest_pos, a4);
    }
    AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&a2);
    if (a2.z < 0.0)
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 1, 0);
    if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x40))
    {
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, 1);
        if ((AI_EvalFunc_COLGMFlagsGet_C(AI_gpst_CurrentGameObject, 0x40) & 0x40000000) != 0)
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
            if (v9->GFX_Halo != -1)
            {
                AI_EvalFunc_GFXDel_C_CURRENT(v9->GFX_Halo);
                v9->GFX_Halo = -1;
            }
            v2 = (tGameObjectMain*)v9->o_afterrec_gao;
            if (v2)
            {
                if ((unsigned int)v2 <= 2)
                    v2 = AI_MainActor[(uint32_t)v2 - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v2,
                    2,
                    (tAIFunction*)fct_nrj_pure_np_reviens_nrj_pure_np_afterrec);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_nrj_pure_np_reviens_nrj_pure_np_afterrec);
            }
            v3 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v3 != 1000)
                return v3;
        }
        else
        {
            a1.x = 0.0;
            a1.y = 0.0;
            a1.z = 0.0;
            AI_EvalFunc_DYNTractionSet_C_CURRENT(&a1);
            v11.x = 0.0;
            v11.y = 0.0;
            v11.z = 0.0;
            AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v11);
            v6 = AI_EvalFunc_DYNFrictionSet_C(AI_gpst_CurrentGameObject, 0.0);
            v10.x = 0.0;
            v10.y = 0.0;
            v10.z = 0.0;
            AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v10);
        }
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        v9->f_time_au_sol = TIM_gf_MainClock - TIM_gf_LockTime;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v9->v_pos_sol);
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_nrj_pure_np_reviens_nrj_pure_np_sol);
    }
    return 1000;
}