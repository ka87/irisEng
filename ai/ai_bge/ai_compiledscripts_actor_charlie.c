#include "ai_compiledscripts_inc.h"

//Scarabee_cine_sca_init
int fct_Scarabee_cine_sca_init_Scarabee_cine_sca_oriente;

// =============================

//charlie_ch_init
int charlie_ch_init_INV_manager_gao;
int charlie_ch_init_INV_manager_gao_i_Animals_Nb;
int charlie_ch_init_INV_manager_gao_ao_Animals;
int charlie_ch_init_INV_manager_gao_ai_AnimalsData;
int charlie_ch_init_univVars;
int charlie_ch_init_univVars_Flag_speciaux_sauve;
int fct_charlie_ch_init_charlie_ch_move;
int fct_charlie_ch_init_charlie_ch_poser;

//charlie_ch_poser
int charlie_ch_poser_univVars;
int charlie_ch_poser_univVars_Flag_speciaux_sauve;
int fct_charlie_ch_poser_charlie_ch_reflexe;
int fct_charlie_ch_poser_charlie_ch_move;

//charlie_ch_move
int charlie_ch_move_univVars;
int charlie_ch_move_univVars_Flag_speciaux_sauve;
int fct_charlie_ch_move_charlie_ch_poser;
int charlie_ch_move__PJ_Sally_gao;

int Scarabee_cine_sca_init()
{
    int result; // eax
    tScriptStruct_ActorScarabeeCine* v1; // [esp+0h] [ebp-50h]
    tGameObjectMain* v8; // [esp+34h] [ebp-1Ch]
    Vec3 v9; // [esp+38h] [ebp-18h] BYREF
    int v10; // [esp+44h] [ebp-Ch]
    tScriptStruct_ActorScarabeeCine* v11; // [esp+4Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ActorScarabeeCine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v11 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Scarabee_cine_sca_init_Scarabee_cine_sca_oriente = (int)AI_ul_RealResolveOneRefFunction(0x3500BE47u);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v9);
    v11->v_old_pos = v9;

    if (v11->des_i_morphing)
    {
        v8 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0);

        if (AI2C_CheckActorGao(v8))
        {
            AI_EvalFunc_OBJ_SetChannelProg_C(AI2C_CheckActorGao(v8), 0, 0.0);
            AI_EvalFunc_OBJ_SetChannelFactor_C(AI2C_CheckActorGao(v8), 0, 1.0);
            AI_EvalFunc_OBJ_SetChannelProg_C(AI2C_CheckActorGao(v8), 1, 0.0);
            AI_EvalFunc_OBJ_SetChannelFactor_C(AI2C_CheckActorGao(v8), 1, 1.0);           
        }
    }

    v10 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Scarabee_cine_sca_init_Scarabee_cine_sca_oriente);
    if (v10 == 1000)
        result = 1000;
    else
        result = v10;
    return result;
}

int Scarabee_cine_sca_oriente()
{
    float a3; // [esp+0h] [ebp-B4h]
    tScriptStruct_ActorScarabeeCine* v2; // [esp+8h] [ebp-ACh]
    Vec3 v9; // [esp+64h] [ebp-50h] BYREF
    Vec3 v11; // [esp+7Ch] [ebp-38h] BYREF
    Vec3 dest; // [esp+98h] [ebp-1Ch] BYREF
    tGameObjectMain* v15; // [esp+A4h] [ebp-10h]
    Vec3 a2, v12, v8;

    v12.x = 1.0;
    v12.y = 0.0;
    v12.z = 0.0;
    a2.x = 1.0;
    a2.y = 0.0;
    a2.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorScarabeeCine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;

    v15 = 0;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v11);
    a2 = v11;

    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(256) && !AI2C_DistOfTwoPointsIsZero(&a2, &v2->v_old_pos))
    {
        MATH_SubstractVec(&dest, &a2, &v2->v_old_pos);
        AI_EvalFunc_MATHVecNormalize_C(&dest, &v9);
        v12 = v9;
        v8.x = 0.0;
        v8.y = 0.0;
        v8.z = 1.0;
        AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v12, &v8);
    }

    v2->v_old_pos = a2;

    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(128))
    {
        v15 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0);

        if (AI2C_CheckActorGao(v15))
        {
            v2->i_prog_ailes++;
            v2->i_prog_ailes &= 0x1;

            a3 = (float)(v2->i_prog_ailes + 1);
            AI_EvalFunc_OBJ_SetChannelProg_C(AI2C_CheckActorGao(v15), 1, a3);
            AI_EvalFunc_OBJ_SetChannelFactor_C(AI2C_CheckActorGao(v15), 1, 1.0);            
        }
    }
    return 1000;
}

// ==============================

int charlie_ch_init()
{
    tScriptStruct_ActorCharlie* v1; // [esp+4h] [ebp-90h]
    int v9; // [esp+34h] [ebp-60h]
    int v10; // [esp+40h] [ebp-54h]
    int v12; // [esp+50h] [ebp-44h]
    int a3; // [esp+54h] [ebp-40h]
    int i; // [esp+58h] [ebp-3Ch]
    int v15; // [esp+60h] [ebp-34h]
    Vec3 v16; // [esp+64h] [ebp-30h] BYREF
    BOOL v17; // [esp+74h] [ebp-20h]
    tScriptStruct_ActorCharlie* v18; // [esp+7Ch] [ebp-18h]
    Vec3 a2; // [esp+80h] [ebp-14h] BYREF

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ActorCharlie*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v18 = v1;
    v17 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        charlie_ch_init_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(&charlie_ch_init_INV_manager_gao_i_Animals_Nb, (void*)charlie_ch_init_INV_manager_gao, 23888);
        AI2C_ResolveBuffer(&charlie_ch_init_INV_manager_gao_ao_Animals, (void*)charlie_ch_init_INV_manager_gao, 23368);
        AI2C_ResolveBuffer(&charlie_ch_init_INV_manager_gao_ai_AnimalsData, (void*)charlie_ch_init_INV_manager_gao, 23628);
        charlie_ch_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&charlie_ch_init_univVars_Flag_speciaux_sauve, (void*)charlie_ch_init_univVars, 4488);
        fct_charlie_ch_init_charlie_ch_move = (int)AI_ul_RealResolveOneRefFunction(0x3D00117Au);
        fct_charlie_ch_init_charlie_ch_poser = (int)AI_ul_RealResolveOneRefFunction(0x3D002B35u);
    }
    if (*AI_gppst_CurrentJumpNode)
    {
        *AI_gppst_CurrentJumpNode = 0;
        TIME_Wait_C(AI_EvalFunc_MATHRandFloat_C(0.0, 0.2f));
        return 0;
    }
    v18->f_rand_move = AI_EvalFunc_MATHRandFloat_C(2.0, 4.0);
    
    if (v18->i_mode_papillon)
    {
        AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, 1.2f);
        AI_EvalFunc_OBJ_SetChannelProg_C(AI2C_CheckActorGao(AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0)), 0, 0.0);        
    }
    else
    {
        if (!AI2C_CheckActorGao(AI_MainActor[0]))
        {
            v9 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v9 != 1000)
                return v9;
        }
    }

    AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 64, 0);
    AI2C_CheckActorGao(AI_gpst_CurrentGameObject)->extended->fields->designInt3 = 0;
    if (v18->i_mode_papillon)
    {
        a3 = 9;
        AI_EvalFunc_OBJ_InfoPhotoParamSet_C(AI_gpst_CurrentGameObject, 4, 9, 0, 0, 0.013f, 1.0, 0.89999998f, 0.1f);
    }
    else
    {
        a3 = 33;
        AI_EvalFunc_OBJ_InfoPhotoParamSet_C(AI_gpst_CurrentGameObject, 4, 33, 0, 0, 0.013f, 1.0, 0.89999998f, 0.1f);
    }
    if (*(int*)charlie_ch_init_INV_manager_gao_i_Animals_Nb < 64)
    {
        for (i = 0; i < *(int32_t*)charlie_ch_init_INV_manager_gao_i_Animals_Nb; i++)
        {           
            if (*(tGameObjectMain**)(charlie_ch_init_INV_manager_gao_ao_Animals + 4 * i + 4) == AI2C_CheckActorGao(AI_gpst_CurrentGameObject))
                break;
        }
        if (i == *(uint32_t*)charlie_ch_init_INV_manager_gao_i_Animals_Nb)
        {
            *(uint32_t*)(charlie_ch_init_INV_manager_gao_ao_Animals + 4 * *(uint32_t*)charlie_ch_init_INV_manager_gao_i_Animals_Nb + 4) = (uint32_t)AI_gpst_CurrentGameObject;
            *(uint32_t*)(charlie_ch_init_INV_manager_gao_ai_AnimalsData + 4 * (*(uint32_t*)charlie_ch_init_INV_manager_gao_i_Animals_Nb)++ + 4) = a3 + (v18->des_i_Etage << 8);
        }
    }
    v16.x = -0.2f;
    v16.y = -0.2f;
    v16.z = -0.2f;
    AI_EvalFunc_BVMinSet_C(AI_gpst_CurrentGameObject, &v16);
    a2.x = 0.2f;
    a2.y = 0.2f;
    a2.z = 0.2f;
    AI_EvalFunc_BVMaxSet_C(AI_gpst_CurrentGameObject, &a2);
    if (v18->i_mode_papillon)
    {
        v17 = AI_EvalFunc_ARRBitTest_C((int*)(charlie_ch_init_univVars_Flag_speciaux_sauve + 4), 809);
        if (!v17)
        {
            v18->f_rand_move = 0.0;
            v15 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_charlie_ch_init_charlie_ch_move);
            if (v15 != 1000)
                return v15;
        }
    }
    if (v18->i_mode_papillon)
    {
        v18->f_delay_poser = AI_EvalFunc_MATHRandFloat_C(6.0, 10.0);
        v18->f_time_poser = TIM_gf_MainClock - TIM_gf_LockTime;
        AI_EvalFunc_AITrackStop_C(1);
        v12 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_charlie_ch_init_charlie_ch_poser);
        if (v12 != 1000)
            return v12;
    }
    else
    {
        v10 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_charlie_ch_init_charlie_ch_move);
        if (v10 != 1000)
            return v10;
    }
    return 1000;
}

int charlie_ch_poser()
{   
    tScriptStruct_ActorCharlie* v5; // [esp+8h] [ebp-108h]
    Vec3 v15; // [esp+50h] [ebp-C0h] BYREF
    tGameObjectMain* v17; // [esp+70h] [ebp-A0h]
    float v18; // [esp+74h] [ebp-9Ch]
    float v20; // [esp+7Ch] [ebp-94h]
    int v21; // [esp+84h] [ebp-8Ch]
    Vec3 a5; // [esp+88h] [ebp-88h] BYREF
    float v25; // [esp+9Ch] [ebp-74h]
    Vec3 v26; // [esp+A0h] [ebp-70h] BYREF
    Vec3 a3; // [esp+B8h] [ebp-58h] BYREF
    Vec3 v29; // [esp+CCh] [ebp-44h] BYREF
    Vec3 v30; // [esp+D8h] [ebp-38h] BYREF
    BOOL v31; // [esp+E4h] [ebp-2Ch]
    BOOL v32; // [esp+E8h] [ebp-28h]
    tScriptStruct_ActorCharlie* v34; // [esp+F4h] [ebp-1Ch]
    Vec3 a2; // [esp+100h] [ebp-10h] BYREF
    int v36; // [esp+10Ch] [ebp-4h]

    v29.x = 1.0;
    v29.y = 0.0;
    v29.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_ActorCharlie*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v34 = v5;
    v31 = 0;
    v17 = 0;
    v32 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        charlie_ch_poser_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&charlie_ch_poser_univVars_Flag_speciaux_sauve, (void*)charlie_ch_poser_univVars, 4488);
        fct_charlie_ch_poser_charlie_ch_reflexe = (int)AI_ul_RealResolveOneRefFunction(0x3D001175u);
        fct_charlie_ch_poser_charlie_ch_move = (int)AI_ul_RealResolveOneRefFunction(0x3D00117Au);
    }
    v31 = 1;
    if (v34->i_mode_papillon)
    {
        v31 = AI_EvalFunc_ARRBitTest_C((int*)(charlie_ch_poser_univVars_Flag_speciaux_sauve + 4), 809);
        if (v31)
        {
            v34->i_first = 0;
        }
        else
        {
            v34->f_time_poser = TIM_gf_MainClock - TIM_gf_LockTime;
            v32 = AI_EvalFunc_ARRBitTest_C((int*)(charlie_ch_poser_univVars_Flag_speciaux_sauve + 4), 120);
            if (v32)
            {
                if (AI_EvalFunc_TIMEElapsed_C(v34->f_sf120, 2.0))
                {
                    AI_EvalFunc_OBJ_SightInitGet_C(AI_gpst_CurrentGameObject, &v15);
                    v26.x = 0.0;
                    v26.y = 0.0;
                    v26.z = 1.0;
                    AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v15, &v26);
                }
            }
            else
            {
                v34->f_sf120 = AI_EvalFunc_TIMEGet_C();
            }
        }
    }
    if (v31)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v34->f_time_poser, v34->f_delay_poser))
        {
            v34->f_time_vole = AI_EvalFunc_TIMEGet_C();
            v34->f_delay_vole = AI_EvalFunc_MATHRandFloat_C(4.0, 6.0);
            AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_charlie_ch_poser_charlie_ch_reflexe);
            v21 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_charlie_ch_poser_charlie_ch_move);
            if (v21 != 1000)
                return v21;
        }
    }
    if (v31)
    {
        v18 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, AI2C_CheckActorGao(AI_MainActor[0]));

        if (v34->f_dist_pos - 2.0 > v18)
        {
            AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_charlie_ch_poser_charlie_ch_reflexe);
            v36 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_charlie_ch_poser_charlie_ch_move);
            if (v36 != 1000)
                return v36;
        }
    }

    v17 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0);
    v25 = AI_EvalFunc_OBJ_GetChannelProg_C(AI2C_CheckActorGao(v17), 0);
    v20 = AI_EvalFunc_OBJ_GetChannelProg_C(AI2C_CheckActorGao(v17), 1);    

    AI_EvalFunc_OBJ_SetChannelProg_C(AI2C_CheckActorGao(v17), 1, AI_EvalFunc_MATHFloatBlend_C(1.0f, v20, 5.0f * TIM_gf_dt));
    AI_EvalFunc_OBJ_SetChannelProg_C(AI2C_CheckActorGao(v17), 0, AI_EvalFunc_MATHFloatBlend_C(3.0f, v25, 5.0f * TIM_gf_dt));
    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&a2);

    a3.x = 0.0;
    a3.y = 0.0;
    a3.z = 1.0;
    AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, 2.0f * TIM_gf_dt, &a5);
    v29 = a5;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v30);
    AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v30, &v29);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v34->v_pos_sur_place);
    return 1000;
}

int charlie_ch_reflexe()
{
    tScriptStruct_ActorCharlie* v6; // [esp+10h] [ebp-60h]
    float v13; // [esp+38h] [ebp-38h]
    tGameObjectMain* v14; // [esp+3Ch] [ebp-34h]
    tGameObjectMain* v15; // [esp+3Ch] [ebp-34h]

    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_ActorCharlie*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    if (v6->i_mode_papillon)
    {
        v14 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0);
        v6->f_prog = AI_EvalFunc_MATHAbsFloat_C(AI_EvalFunc_MATHSinus_C(20.0f * AI_EvalFunc_TIMEGet_C())) + 1.0f;
        AI_EvalFunc_OBJ_SetChannelProg_C(AI2C_CheckActorGao(v14), 1, v6->f_prog);        
        v13 = AI_EvalFunc_OBJ_GetChannelProg_C(AI2C_CheckActorGao(v14), 0);        
        AI_EvalFunc_OBJ_SetChannelProg_C(AI2C_CheckActorGao(v14), 0, AI_EvalFunc_MATHFloatBlend_C(1.0f, v13, 2.0f * TIM_gf_dt));
    }
    else
    {
        v15 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0);
        v6->f_prog = AI_EvalFunc_MATHAbsFloat_C(AI_EvalFunc_MATHSinus_C(20.0f * AI_EvalFunc_TIMEGet_C())) + 1.0f;
        AI_EvalFunc_OBJ_SetChannelProg_C(AI2C_CheckActorGao(v15), 0, v6->f_prog);
    }
    return 1000;
}

int charlie_ch_move()
{
    int result; // eax
    float v17; // [esp+8h] [ebp-31Ch]
    tScriptStruct_ActorCharlie* v18; // [esp+10h] [ebp-314h]
    float v20; // [esp+4Ch] [ebp-2D8h]    
    tGameObjectMain* v46; // [esp+150h] [ebp-1D4h]
    float v47; // [esp+154h] [ebp-1D0h]
    Vec3 v48; // [esp+15Ch] [ebp-1C8h] BYREF
    Vec3 v49; // [esp+168h] [ebp-1BCh] BYREF
    Vec3 v50; // [esp+178h] [ebp-1ACh] BYREF
    Vec3 v53; // [esp+18Ch] [ebp-198h] BYREF
    BOOL v54; // [esp+19Ch] [ebp-188h]
    float v56; // [esp+1A8h] [ebp-17Ch]
    Vec3 v57; // [esp+1B4h] [ebp-170h] BYREF
    Vec3 v58; // [esp+1C0h] [ebp-164h] BYREF
    float v59; // [esp+1CCh] [ebp-158h]
    float v60; // [esp+1D4h] [ebp-150h]
    Vec3 v61; // [esp+1D8h] [ebp-14Ch] BYREF
    Vec3 v62; // [esp+1E4h] [ebp-140h]
    float v63; // [esp+1F0h] [ebp-134h]]
    int v65; // [esp+204h] [ebp-120h]
    tGameObjectMain* v66; // [esp+208h] [ebp-11Ch]
    Vec3 v67; // [esp+20Ch] [ebp-118h]
    Vec3 v71; // [esp+228h] [ebp-FCh] BYREF    
    Vec3 v74; // [esp+24Ch] [ebp-D8h] BYREF
    Vec3 v75; // [esp+258h] [ebp-CCh] BYREF
    Vec3 a3; // [esp+268h] [ebp-BCh] BYREF
    Vec3 v77; // [esp+274h] [ebp-B0h] BYREF
    Vec3 v79; // [esp+284h] [ebp-A0h] BYREF    
    Vec3 v84; // [esp+2A4h] [ebp-80h] BYREF
    Vec3 a4; // [esp+2B0h] [ebp-74h] BYREF
    tScriptStruct_ActorCharlie* v89; // [esp+2C8h] [ebp-5Ch]
    Vec3 v91; // [esp+2D0h] [ebp-54h] BYREF
    Vec3 v92; // [esp+2DCh] [ebp-48h] BYREF
    float v94; // [esp+2F0h] [ebp-34h]
    Vec3 v95; // [esp+2F4h] [ebp-30h]
    Vec3 dest; // [esp+300h] [ebp-24h] BYREF
    Vec3 v97; // [esp+30Ch] [ebp-18h] BYREF
    BOOL v99; // [esp+31Ch] [ebp-8h]
    Vec3 a1;
    Vec3 v72;

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    v50.x = 1.0;
    v50.y = 0.0;
    v50.z = 0.0;
    v67.x = 1.0;
    v67.y = 0.0;
    v67.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v18 = (tScriptStruct_ActorCharlie*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v18 = 0;
    v89 = v18;
    v59 = 0.0;
    v99 = 0;
    v63 = 0.0;
    v60 = 0.0;
    v66 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        charlie_ch_move_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&charlie_ch_move_univVars_Flag_speciaux_sauve, (void*)charlie_ch_move_univVars, 4488);
        fct_charlie_ch_move_charlie_ch_poser = (int)AI_ul_RealResolveOneRefFunction(0x3D002B35u);
        charlie_ch_move__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
    }
    if (*AI_gppst_CurrentJumpNode && v89->f_first_wait != 0.0)
    {
        *AI_gppst_CurrentJumpNode = 0;
        TIME_Wait_C(v89->f_first_wait);
        return 0;
    }
    v89->f_first_wait = 0.0;
    v89->ti_pose_en_cours = 0;
    v66 = AI_MainActor[1];
    if (v89->i_mode_papillon)
        goto LABEL_164;
    v46 = v66;
    if (!AI2C_CheckActorGao(v66))
        goto LABEL_167;

    if (AI_EvalFunc_COLBVObjectObject_C(AI2C_CheckActorGao(v66), AI2C_CheckActorGao((tGameObjectMain*)v89->des_o_wp)))
    {
        AI_EvalFunc_OBJGetPos_C(AI2C_CheckActorGao(v66), &a3);
        v50.x = a3.x;
        v50.y = a3.y;
        v50.z = a3.z + 2.0f;
    }
    else
    {
    LABEL_167:
        if (v89->i_mode_papillon)
            goto LABEL_165;
    LABEL_164:
        if (!AI2C_CheckActorGao((tGameObjectMain*)v89->des_o_wp1))
            goto LABEL_168;

        if (!AI2C_CheckActorGao(v66))
            goto LABEL_168;
        
        if (AI_EvalFunc_COLBVObjectObject_C(AI2C_CheckActorGao(v66), AI2C_CheckActorGao((tGameObjectMain*)v89->des_o_wp1)))
        {
            AI_EvalFunc_OBJGetPos_C(AI2C_CheckActorGao(v66), &v49);            
            v50.x = v49.x;
            v50.y = v49.y;
            v50.z = v49.z + 2.0f;
        }
        else
        {
        LABEL_168:
            if (!v89->i_mode_papillon)
                goto LABEL_166;
        LABEL_165:
            v94 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, AI2C_CheckActorGao(AI_MainActor[0]));            
            if (v89->f_dist_pos < (double)v94 && (v54 = AI_EvalFunc_TIMEElapsed_C(v89->f_time_vole, v89->f_delay_vole)))
            {
                v89->ti_pose_en_cours = 1;
                AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &v77);
                v50 = v77;
            }
            else
            {
            LABEL_166:
                if (v89->i_first)
                {
                    v89->ti_pose_en_cours = 1;
                    AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &v91);
                    v50 = v91;
                }
                else
                {
                    if (!AI2C_CheckActorGao((tGameObjectMain*)v89->des_o_wp))
                    {
                        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                        return 1000;
                    }
                    AI_EvalFunc_BVRandomPosGet_C(AI2C_CheckActorGao((tGameObjectMain*)v89->des_o_wp), 0, &v57);                    
                    AI_EvalFunc_OBJGetPos_C(AI2C_CheckActorGao((tGameObjectMain*)v89->des_o_wp), &v75);
                    v50 = v57;                    
                    a1 = v75;
                    v50.z = v75.z;
                }
            }
        }
    }
    if (v89->i_mode_papillon)
    {
        if (AI2C_CheckActorGao((tGameObjectMain*)v89->des_o_pos_dep))
        {
            v99 = AI_EvalFunc_ARRBitTest_C((int*)(charlie_ch_move_univVars_Flag_speciaux_sauve + 4), 809);
            if (!v99 && !v89->i_first)
            {
                v89->i_first = 1;
                AI_EvalFunc_OBJGetPos_C(AI2C_CheckActorGao((tGameObjectMain*)v89->des_o_pos_dep), &v97);                
                v50 = v97;
                AI_EvalFunc_OBJSetPos_C_CURRENT(&v50);
                AI_EvalFunc_OBJ_SightGet_C(AI2C_CheckActorGao((tGameObjectMain*)v89->des_o_pos_dep), &v71);                
                v58.x = 0.0;
                v58.y = 0.0;
                v58.z = 1.0;
                AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v71, &v58);
                AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 1);
                v89->i_mode_move = 1.0;
                v89->f_time_move = TIM_gf_MainClock - TIM_gf_LockTime;
                v89->f_rand_move = 1.0;
                v89->ti_pose_en_cours = 1;
                v89->f_first_wait = 2.0;
            }
        }
    }
    if (v89->i_mode_papillon)
        v59 = 0.30000001f;
    else
        v59 = 0.5;
    if (v89->i_mode_move == 0.0f)
        v59 = v59 * 0.1f;
    if (!v89->ti_pose_en_cours)
    {
        v50.x = AI_EvalFunc_MATHRandFloat_C(-(v59 * 0.1f), v59 * 0.1f) + v50.x;
        v50.y = AI_EvalFunc_MATHRandFloat_C(-(v59 * 0.1f), v59 * 0.1f) + v50.y;
        v50.z = AI_EvalFunc_MATHRandFloat_C(-(v59 * 0.5f), v59 * 1.0f) + v50.z;
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v74);
    MATH_SubstractVec(&dest, &v74, &v50);
    a1 = dest;
    if (AI2C_VecEqNull(&a1))
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        MATH_NormalizeEqualVector(&a1);
        if (v89->i_mode_papillon)
        {
            AI2C_NegateVector(&v62, &a1);
            AI2C_NegateVector(&a1, &a1);
        }
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v61);
        v67 = v61;
        a1.x = AI_EvalFunc_MATHFloatBlend_C(v67.x, a1.x, v59 * 6.0f * TIM_gf_dt);
        a1.y = AI_EvalFunc_MATHFloatBlend_C(v67.y, a1.y, v59 * 6.0f * TIM_gf_dt);
        a1.z = AI_EvalFunc_MATHFloatBlend_C(v67.z, a1.z, v59 * 8.0f * TIM_gf_dt);        
        v72.x = 0.0;
        v72.y = 0.0;
        v72.z = 1.0;
        AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a1, &v72);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v92);
        v56 = TIM_gf_dt;
        v20 = v59 * 5.0f * TIM_gf_dt;
        a1.x = v92.x * v20;
        a1.y = v92.y * v20;
        a1.z = v92.z * v20;
        if (v89->i_mode_papillon)
        {
            AI2C_NegateVector(&v95, &a1);
            AI2C_NegateVector(&a1, &a1);
        }
        if (!v89->ti_pose_en_cours)
            goto LABEL_142;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v79);
        MATH_SubstractVec(&v48, &v50, &v79);

        if (MATH_VecLen(&a1) <= MATH_VecLen(&v48)
            || (v89->f_time_poser = AI_EvalFunc_TIMEGet_C(),
                v89->f_delay_poser = AI_EvalFunc_MATHRandFloat_C(6.0, 10.0),
                AI_EvalFunc_AITrackStop_C(1),
                v65 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_charlie_ch_move_charlie_ch_poser),
                v65 == 1000))
        {
        LABEL_142:
            if (v89->i_mode_move == 0.0f && !v89->i_first)
            {
                a1.x = 0.0f;
                a1.y = 0.0f;
                a1.z = 0.0f;
                a1.z = AI_EvalFunc_MATHSinus_C((v89->f_rand_move + AI_EvalFunc_TIMEGet_C()) * 7.0f) * 0.02f;
                AI_EvalFunc_MATHVecBlend_C(&v89->v_last_move, &a1, 4.0f * TIM_gf_dt, &a4);
                v89->v_last_move = a4;
            }
            else
            {
                v89->v_last_move = a1;
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v53);
            MATH_SubstractVec(&v84, &v53, &v89->v_last_move);
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v84);
            if (!v89->i_first)
            {
                if (AI_EvalFunc_TIMEElapsed_C(v89->f_time_move, v89->f_rand_move))
                {
                    v89->i_mode_move = v89->i_mode_move + 1.0f;
                    if (v89->i_mode_move == 2.0f)
                        v89->i_mode_move = 0.0f;
                    v89->f_time_move = TIM_gf_MainClock - TIM_gf_LockTime;
                    v89->f_rand_move = AI_EvalFunc_MATHRandFloat_C(2.0f, 3.0f);
                }
            }
            v60 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, AI2C_GetMainActor((tGameObjectMain*)charlie_ch_move__PJ_Sally_gao)); ;
            if (v89->i_snd_bzzz == -1 && v89->f_dist_snd >= (double)v60)
            {
                v89->i_snd_bzzz = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 402653184);;
                AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v89->i_snd_bzzz);
                v17 = (float)SND_l_GetFreq(v89->i_snd_bzzz);
                SND_SetFreq(v89->i_snd_bzzz, (unsigned int)(v89->f_freq_pitch * v17));
                v47 = SND_f_InstanceGetf(v89->i_snd_bzzz, 4107);
                v89->f_dist_snd = v47 + 1.0f;
                v89->f_dist_snd = v89->f_dist_snd * v89->f_dist_snd;
            }
            else if (v89->i_snd_bzzz != -1 && v89->f_dist_snd < (double)v60)
            {
                SND_Release(v89->i_snd_bzzz);
                v89->i_snd_bzzz = -1;
            }
            result = 1000;
        }
        else
        {
            result = v65;
        }
    }
    return result;
}
