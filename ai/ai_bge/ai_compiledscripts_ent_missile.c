#include "ai_compiledscripts_inc.h"

//Ent_missile_EM_comportement
int fct_Ent_missile_EM_comportement_Ent_missile_EM_explosion;
int Ent_missile_EM_comportement_GST_Global_gao;
int Ent_missile_EM_comportement_GST_Global_gao_i_fade_priority;
int Ent_missile_EM_comportement_GST_Global_gao_f_fade_start_time;
int Ent_missile_EM_comportement_GST_Global_gao_f_fade_delay;
int Ent_missile_EM_comportement_univVars;
int Ent_missile_EM_comportement_univVars_i_fade_fx;
int Ent_missile_EM_comportement_INV_MatFade_gao;
int Ent_missile_EM_comportement_GST_Global_gao_f_eau_hauteurZ;

//Ent_missile_EM_init
int Ent_missile_EM_init_SFX_light_smoke_gao;
int fct_Ent_missile_EM_init_Ent_missile_EM_comportement;
int fct_Ent_missile_EM_init_Ent_missile_EM_reflex;

//Ent_missile_EM_reflex
int fct_Ent_missile_EM_reflex_Ent_missile_EM_disparition;

//Ent_missile_EM_explosion
int Ent_missile_EM_explosion_SFX_explosion_gao;

int Ent_missile_EM_disparition()
{
    float size; // [esp+4h] [ebp-4Ch]
    float v2; // [esp+8h] [ebp-48h]
    tScriptStruct_EntMissile* v3; // [esp+10h] [ebp-40h]
    float v4; // [esp+14h] [ebp-3Ch]
    float v5; // [esp+18h] [ebp-38h]
    int v6; // [esp+1Ch] [ebp-34h]
    float v7; // [esp+20h] [ebp-30h]
    float v8; // [esp+24h] [ebp-2Ch]
    int v9; // [esp+2Ch] [ebp-24h]
    float v10; // [esp+38h] [ebp-18h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_EntMissile*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    if (*AI_gppst_CurrentJumpNode != (tAINode*)2)
    {
        if (!AI_EvalFunc_TIMEElapsed_C(v3->f_time_start, 1.0))
        {
            v10 = TIM_gf_MainClock - TIM_gf_LockTime;
            v7 = 1.0f - (v10 - v3->f_time_start);
            v6 = AI_EvalFunc_COLORBlend_C(0, v3->c_color, v7);
            GFXSetCurGlobal(v3->GFX_Halo);
            AI_EvalFunc_GFXSeti_C_CURGLOB(4102, v6);
            size = 12.0f * v7;
            AI_EvalFunc_GFXSetf_C_CURGLOB(4000, size);
            return 1000;
        }
        AI_EvalFunc_AITrackStop_C(1);
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        v3->f_time_disp = TIM_gf_MainClock - TIM_gf_LockTime;
        v3->f_size_halo = 2.0;
    }
    if (AI_EvalFunc_TIMEElapsed_C(v3->f_time_disp, 2.0))
    {
        if (v3->GFX_Halo != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v3->GFX_Halo);
            v3->GFX_Halo = -1;
        }
        v9 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v9 != 1000)
            return v9;
        return 1000;
    }
    v5 = 4.0f * TIM_gf_dt;
    if (v5 < 0.0)
        v5 = 0.0;
    if (v5 > 1.0)
        v5 = 1.0;
    v8 = 0.0f * v5 + v3->f_largeur_X_men * (1.0f - v5);
    v3->f_largeur_X_men = v8;
    XMEN_SetSize(AI_gpst_CurrentGameObject, 0, -1, v3->f_largeur_X_men);
    v4 = v3->f_size_halo - TIM_gf_dt * 6.0f;
    if (v4 < 0.0)
        v2 = 0.0;
    else
        v2 = v3->f_size_halo - TIM_gf_dt * 6.0f;
    v3->f_size_halo = v2;
    AI_EvalFunc_GFXSetf_C_CURRENT(v3->GFX_Halo, 21000, v3->f_size_halo);
    *AI_gppst_CurrentJumpNode = (tAINode*)2;
    return 0;
}

int Ent_missile_EM_comportement()
{
    int result; // eax
    double v1; // st7
    double v2; // st7
    tGameObjectMain* v3; // eax
    tGameObjectMain* v4; // eax
    float a4; // [esp+0h] [ebp-258h]
    float v6; // [esp+8h] [ebp-250h]
    float v7; // [esp+Ch] [ebp-24Ch]
    tScriptStruct_EntMissile* v8; // [esp+10h] [ebp-248h]
    tGameObjectMain* v9; // [esp+18h] [ebp-240h]
    tGameObjectMain* v10; // [esp+30h] [ebp-228h]
    tGameObjectMain* v11; // [esp+38h] [ebp-220h]
    tGameObjectMain* v12; // [esp+50h] [ebp-208h]
    float v13; // [esp+5Ch] [ebp-1FCh]
    float v14; // [esp+60h] [ebp-1F8h]
    float v15; // [esp+64h] [ebp-1F4h]
    float v16; // [esp+70h] [ebp-1E8h]
    float v17; // [esp+88h] [ebp-1D0h]
    tGameObjectMain* v18; // [esp+98h] [ebp-1C0h]
    tGameObjectMain* v19; // [esp+A0h] [ebp-1B8h]
    tGameObjectMain* v20; // [esp+A4h] [ebp-1B4h]
    tGameObjectMain* v21; // [esp+A8h] [ebp-1B0h]
    tGameObjectMain* v22; // [esp+ACh] [ebp-1ACh]
    tGameObjectMain* v23; // [esp+B0h] [ebp-1A8h]
    tGameObjectMain* v24; // [esp+B8h] [ebp-1A0h]
    tGameObjectMain* v25; // [esp+C0h] [ebp-198h]
    tGameObjectMain* v26; // [esp+C4h] [ebp-194h]
    tGameObjectMain* v27; // [esp+C8h] [ebp-190h]
    Vec3 a5; // [esp+CCh] [ebp-18Ch] BYREF
    int v29; // [esp+D8h] [ebp-180h]
    tGameObjectMain* v30; // [esp+DCh] [ebp-17Ch]
    Vec3 v31; // [esp+E0h] [ebp-178h] BYREF
    Vec3 a3; // [esp+ECh] [ebp-16Ch] BYREF
    int v33; // [esp+F8h] [ebp-160h]
    Vec3 v34; // [esp+10Ch] [ebp-14Ch] BYREF
    int v36; // [esp+11Ch] [ebp-13Ch]
    float v37; // [esp+124h] [ebp-134h]
    float v38; // [esp+128h] [ebp-130h]
    float v39; // [esp+134h] [ebp-124h]
    Vec3 v40; // [esp+138h] [ebp-120h] BYREF
    float v41; // [esp+144h] [ebp-114h]
    float v42; // [esp+148h] [ebp-110h]
    float v43; // [esp+14Ch] [ebp-10Ch]
    Vec3 v44; // [esp+150h] [ebp-108h] BYREF
    float v45; // [esp+15Ch] [ebp-FCh]
    float v46; // [esp+160h] [ebp-F8h]
    Vec3 v47; // [esp+164h] [ebp-F4h] BYREF
    Vec3 a2; // [esp+170h] [ebp-E8h] BYREF
    int v49; // [esp+180h] [ebp-D8h]
    Vec3 v50; // [esp+184h] [ebp-D4h]
    int v51; // [esp+194h] [ebp-C4h]
    BOOL v52; // [esp+198h] [ebp-C0h]
    Vec3 v53; // [esp+19Ch] [ebp-BCh] BYREF
    Vec3 v54; // [esp+1A8h] [ebp-B0h] BYREF
    float v55; // [esp+1B4h] [ebp-A4h]
    float v56; // [esp+1B8h] [ebp-A0h]
    float v57; // [esp+1BCh] [ebp-9Ch]
    Vec3 dest; // [esp+1C0h] [ebp-98h] BYREF
    float v59; // [esp+1CCh] [ebp-8Ch]
    int v61; // [esp+1E4h] [ebp-74h]
    float v62; // [esp+1F8h] [ebp-60h]
    int v63; // [esp+200h] [ebp-58h]
    tScriptStruct_EntMissile* v64; // [esp+20Ch] [ebp-4Ch]
    Vec3 v66; // [esp+214h] [ebp-44h] BYREF
    Vec3 v67; // [esp+220h] [ebp-38h] BYREF
    float v68; // [esp+22Ch] [ebp-2Ch]
    BOOL v69; // [esp+234h] [ebp-24h]
    Vec3 v70; // [esp+238h] [ebp-20h] BYREF
    Vec3 v71; // [esp+244h] [ebp-14h] BYREF

    v55 = 1.0;
    v56 = 0.0;
    v57 = 0.0;
    v40.x = 1.0;
    v40.y = 0.0;
    v40.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    v50.x = 1.0;
    v50.y = 0.0;
    v50.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v8 = (tScriptStruct_EntMissile*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v8 = 0;
    v64 = v8;
    v38 = 0.0;
    v45 = 0.0;
    v36 = 0;
    v39 = 0.0;
    v46 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Ent_missile_EM_comportement_Ent_missile_EM_explosion = (int)AI_ul_RealResolveOneRefFunction(0x72001318u);
        Ent_missile_EM_comportement_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Ent_missile_EM_comportement_GST_Global_gao_i_fade_priority,
            (void*)Ent_missile_EM_comportement_GST_Global_gao,
            3268);
        AI2C_ResolveBuffer(
            &Ent_missile_EM_comportement_GST_Global_gao_f_fade_start_time,
            (void*)Ent_missile_EM_comportement_GST_Global_gao,
            3260);
        AI2C_ResolveBuffer(
            &Ent_missile_EM_comportement_GST_Global_gao_f_fade_delay,
            (void*)Ent_missile_EM_comportement_GST_Global_gao,
            3264);
        Ent_missile_EM_comportement_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Ent_missile_EM_comportement_univVars_i_fade_fx,
            (void*)Ent_missile_EM_comportement_univVars,
            20400);
        Ent_missile_EM_comportement_INV_MatFade_gao = AI_ul_RealResolveOneRefObject(0x5E006934u);
        AI2C_ResolveBuffer(
            &Ent_missile_EM_comportement_GST_Global_gao_f_eau_hauteurZ,
            (void*)Ent_missile_EM_comportement_GST_Global_gao,
            2772);
    }
    v27 = (tGameObjectMain*)v64->des_o_target;
    if (v64->des_o_target)
    {
        if ((unsigned int)v27 <= 2)
            v27 = AI_MainActor[(uint32_t)v27 - 1];
        v26 = v27;
    }
    else
    {
        v26 = 0;
    }
    if (v26)
    {
        v46 = TIM_gf_dt;
        v25 = (tGameObjectMain*)v64->des_o_target;
        if (v64->des_o_target)
        {
            if ((unsigned int)v25 <= 2)
                v25 = AI_MainActor[(uint32_t)v25 - 1];
            v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v25);
        }
        else
        {
            v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
        }
        v39 = (float)v1;
        v24 = (tGameObjectMain*)v64->des_o_target;
        if (v64->des_o_target)
        {
            if ((unsigned int)v24 <= 2)
                v24 = AI_MainActor[(uint32_t)v24 - 1];
            AI_EvalFunc_OBJGetPos_C(v24, &v64->v_target_pos);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v64->v_target_pos);
        }
        if (v39 >= 225.0
            || (v33 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Ent_missile_EM_comportement_Ent_missile_EM_explosion),
                v33 == 1000))
        {
            v38 = v64->des_f_traction;
            v30 = *AI_MainActor;
            v23 = (tGameObjectMain*)v64->des_o_target;
            if (v64->des_o_target)
            {
                if ((unsigned int)v23 <= 2)
                    v23 = AI_MainActor[(uint32_t)v23 - 1];
                v22 = v23;
            }
            else
            {
                v22 = 0;
            }
            v21 = v30;
            if (v30)
            {
                if ((unsigned int)v30 <= 2)
                    v21 = AI_MainActor[(uint32_t)v30 - 1];
                v20 = v21;
            }
            else
            {
                v20 = 0;
            }
            if (v22 == v20)
            {
                v52 = AI_EvalFunc_TIMEElapsed_C(v64->f_time_start_fade, 1.0);
                if (v52 && v39 < 160000.0)
                {
                    if (*(int*)Ent_missile_EM_comportement_GST_Global_gao_i_fade_priority <= 100
                        || (v69 = AI_EvalFunc_TIMEElapsed_C(
                            *(float*)Ent_missile_EM_comportement_GST_Global_gao_f_fade_start_time,
                            *(float*)Ent_missile_EM_comportement_GST_Global_gao_f_fade_delay)))
                    {
                        *(float*)Ent_missile_EM_comportement_GST_Global_gao_f_fade_start_time = TIM_gf_MainClock - TIM_gf_LockTime;
                        *(uint32_t*)Ent_missile_EM_comportement_GST_Global_gao_f_fade_delay = 0x3F028F5C;
                        *(uint32_t*)Ent_missile_EM_comportement_GST_Global_gao_i_fade_priority = 100;
                        AI_EvalFunc_GFXDel_C_CURRENT(10);
                        v36 = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
                        *(uint32_t*)Ent_missile_EM_comportement_univVars_i_fade_fx = v36;
                        AI_EvalFunc_GFXFlagSet_C_CURRENT(v36, 0, 1);
                        AI_EvalFunc_GFXFlagSet_C_CURRENT(v36, 2, 1);
                        v19 = (tGameObjectMain*)Ent_missile_EM_comportement_INV_MatFade_gao;
                        if (Ent_missile_EM_comportement_INV_MatFade_gao)
                        {
                            if ((unsigned int)Ent_missile_EM_comportement_INV_MatFade_gao <= 2)
                                v19 = AI_MainActor[Ent_missile_EM_comportement_INV_MatFade_gao - 1];
                            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v36, v19, 9);
                        }
                        else
                        {
                            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v36, 0, 9);
                        }
                        GFXSetCurGlobal(v36);
                        AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0);
                        AI_EvalFunc_GFXSeti_C_CURGLOB(11101, 0x400000FF);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 0.20999999f);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 0.1f);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.2f);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
                    }
                    else
                    {
                        v36 = -1;
                    }
                    if (v36 != -1)
                        v64->f_time_start_fade = TIM_gf_MainClock - TIM_gf_LockTime;
                }
                if (v39 < 2500.0)
                {
                    v18 = (tGameObjectMain*)v64->des_o_target;
                    if (v64->des_o_target)
                    {
                        if ((unsigned int)v18 <= 2)
                            v18 = AI_MainActor[(uint32_t)v18 - 1];
                        v2 = AI_EvalFunc_DYNSpeedGet_C(v18);
                    }
                    else
                    {
                        v2 = AI_EvalFunc_DYNSpeedGet_C(0);
                    }
                    v37 = (float)v2;
                    if (v37 > (double)v64->des_f_traction)
                        v7 = v64->des_f_traction;
                    else
                        v7 = v37;
                    v59 = v7;
                    v45 = v7;
                    v17 = v39 / 10.0f + v7;
                    if (v17 <= (double)v64->des_f_traction)
                    {
                        if (v17 >= (double)v45)
                            v38 = v39 / 10.0f + v7;
                        else
                            v38 = v45;
                    }
                    else
                    {
                        v38 = v64->des_f_traction;
                    }
                }
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v71);
            MATH_SubstractVec(&dest, &v64->v_target_pos, &v71);
            a3 = dest;
            MATH_NormalizeEqualVector(&a3);
            v16 = 0.5f * v46;
            if (v16 < 0.0)
                v16 = 0.0;
            if (v16 > 1.0)
                v16 = 1.0;
            v64->f_largeur_X_men = 15.0f * v16 + v64->f_largeur_X_men * (1.0f - v16);
            XMEN_SetSize(AI_gpst_CurrentGameObject, 0, -1, v64->f_largeur_X_men);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
            a4 = v64->des_f_rotation_speed * v46;
            AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
            v40 = a5;
            AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v40);
            v15 = v64->des_f_rot_coef_blend_speed * v46;
            if (v15 < 0.0)
                v15 = 0.0;
            if (v15 > 1.0)
                v15 = 1.0;
            v64->des_f_rotation_speed = v64->des_f_rotation_speed_max * v15 + v64->des_f_rotation_speed * (1.0f - v15);
            v14 = v64->des_f_friction_blend_speed * v46;
            if (v14 < 0.0)
                v14 = 0.0;
            if (v14 > 1.0)
                v14 = 1.0;
            v64->des_f_friction = v64->des_f_friction_max * v14 + v64->des_f_friction * (1.0f - v14);
            if (v39 < 10000.0 && !AI_EvalFunc_MATHFloatNullEps_C(v39))
            {
                v64->des_f_rotation_speed = 100000.0f / v39;
                v13 = 80000.0f / v39;
                if (v13 > 20.0)
                    v6 = 20.0;
                else
                    v6 = 80000.0f / v39;
                v64->des_f_friction = v6;
            }
            v61 = AI_EvalFunc_DYNFrictionSet_C(AI_gpst_CurrentGameObject, v64->des_f_friction);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v44);
            v41 = v44.x * v38;
            v42 = v44.y * v38;
            v43 = v44.z * v38;
            v55 = v41;
            v56 = v42;
            v57 = v43;
            v62 = AI_EvalFunc_DYNFrictionGet_C(AI_gpst_CurrentGameObject);
            v31.x = v55 * v62;
            v31.y = v56 * v62;
            v31.z = v57 * v62;
            AI_EvalFunc_DYNTractionSet_C_CURRENT(&v31);
            if (v64->GFX_Halo != -1)
            {
                v68 = AI_EvalFunc_MATHRandFloat_C(0.0, 1.0);
                v51 = AI_EvalFunc_COLORBlend_C(0x8000FF00, 0x80008000, v68);
                v64->c_color = v51;
                AI_EvalFunc_GFXSeti_C_CURRENT(v64->GFX_Halo, 21101, v64->c_color);
            }
            if (!AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x80))
                goto LABEL_97;
            v3 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 0x80);
            v64->o_collide_object = (int)v3;
            v12 = (tGameObjectMain*)v64->o_collide_object;
            if (v12)
            {
                if ((unsigned int)v12 <= 2)
                    v12 = AI_MainActor[(uint32_t)v12 - 1];
                AI_EvalFunc_OBJGetPos_C(v12, &v54);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v54);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v67);
            MATH_SubstractVec(&v34, &v54, &v67);
            v50 = v34;
            v64->m_msg.msgID = 37;
            v64->m_msg.gao = AI_gpst_CurrentGameObject;
            v64->m_msg.gao2 = 0;
            v64->m_msg.Int1 = v64->des_puissance + 96;
            v64->m_msg.Int2 = 0;
            v64->m_msg.v1 = v50;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v64->m_msg.v2);
            v64->m_msg.f2 = (float)v64->des_i_point_de_vie;
            v64->m_msg.f1 = 1000.0;
            v11 = (tGameObjectMain*)v64->o_collide_object;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v11 = AI_MainActor[(uint32_t)v11 - 1];
                AI_EvalFunc_MSGSend_C(v11, &v64->m_msg);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &v64->m_msg);
            }
            v29 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Ent_missile_EM_comportement_Ent_missile_EM_explosion);
            if (v29 == 1000)
            {
            LABEL_97:
                if (!AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x40))
                    goto LABEL_110;
                v4 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 0x40);
                v64->o_collide_object = (int)v4;
                v10 = (tGameObjectMain*)v64->o_collide_object;
                if (v10)
                {
                    if ((unsigned int)v10 <= 2)
                        v10 = AI_MainActor[(uint32_t)v10 - 1];
                    AI_EvalFunc_OBJGetPos_C(v10, &v66);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v66);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v53);
                MATH_SubstractVec(&v47, &v66, &v53);
                v50 = v47;
                v64->m_msg.msgID = 37;
                v64->m_msg.gao = AI_gpst_CurrentGameObject;
                v64->m_msg.gao2 = 0;
                v64->m_msg.Int1 = v64->des_puissance + 96;
                v64->m_msg.Int2 = 0;
                v64->m_msg.v1 = v50;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v64->m_msg.v2);
                v64->m_msg.f2 = (float)v64->des_i_point_de_vie;
                v64->m_msg.f1 = 1000.0;
                v9 = (tGameObjectMain*)v64->o_collide_object;
                if (v9)
                {
                    if ((unsigned int)v9 <= 2)
                        v9 = AI_MainActor[(uint32_t)v9 - 1];
                    AI_EvalFunc_MSGSend_C(v9, &v64->m_msg);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &v64->m_msg);
                }
                v49 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Ent_missile_EM_comportement_Ent_missile_EM_explosion);
                if (v49 == 1000)
                {
                LABEL_110:
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v70);
                    if (v70.z >= (double)*(float*)Ent_missile_EM_comportement_GST_Global_gao_f_eau_hauteurZ
                        || (v63 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Ent_missile_EM_comportement_Ent_missile_EM_explosion),
                            v63 == 1000))
                    {
                        result = 1000;
                    }
                    else
                    {
                        result = v63;
                    }
                }
                else
                {
                    result = v49;
                }
            }
            else
            {
                result = v29;
            }
        }
        else
        {
            result = v33;
        }
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int Ent_missile_EM_afterrec()
{
    tScriptStruct_EntMissile* v1; // [esp+0h] [ebp-14h]
    Vec3 a1; // [esp+4h] [ebp-10h] BYREF
    tScriptStruct_EntMissile* v3; // [esp+10h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_EntMissile*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
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

int Ent_missile_EM_init()
{
    int result; // eax
    tScriptStruct_EntMissile* v1; // [esp+4h] [ebp-B8h]
    tGameObjectMain* v2; // [esp+Ch] [ebp-B0h]
    float v3; // [esp+10h] [ebp-ACh]
    tGameObjectMain* v4; // [esp+18h] [ebp-A4h]
    tGameObjectMain* v5; // [esp+20h] [ebp-9Ch]
    tGameObjectMain* v6; // [esp+3Ch] [ebp-80h]
    tGameObjectMain* v7; // [esp+40h] [ebp-7Ch]
    BOOL v8; // [esp+44h] [ebp-78h]
    void* Src; // [esp+54h] [ebp-68h]
    Vec3 v10; // [esp+58h] [ebp-64h] BYREF
    Vec3 v11; // [esp+64h] [ebp-58h] BYREF
    int v12; // [esp+70h] [ebp-4Ch]
    BOOL v13; // [esp+78h] [ebp-44h]
    tGameObjectMain* v14; // [esp+7Ch] [ebp-40h]
    int v15; // [esp+80h] [ebp-3Ch]
    BOOL v16; // [esp+84h] [ebp-38h]
    int v17; // [esp+88h] [ebp-34h]
    int v18; // [esp+8Ch] [ebp-30h]
    Vec3 a2; // [esp+90h] [ebp-2Ch] BYREF
    int v20; // [esp+A0h] [ebp-1Ch]
    tScriptStruct_EntMissile* a3; // [esp+A4h] [ebp-18h]
    void* v22; // [esp+A8h] [ebp-14h]
    Vec3 v23; // [esp+B0h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_EntMissile*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    a3 = v1;
    v14 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Ent_missile_EM_init_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        fct_Ent_missile_EM_init_Ent_missile_EM_comportement = (int)AI_ul_RealResolveOneRefFunction(0x72001316u);
        fct_Ent_missile_EM_init_Ent_missile_EM_reflex = (int)AI_ul_RealResolveOneRefFunction(0x7200131Au);
    }
    v13 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
    v20 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x1000004, 0);
    v14 = AI_gpst_CurrentGameObject;
    Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&a3->m_msg, Src, sizeof(a3->m_msg));
    while (a3->m_msg.msgID)
    {
        v14 = a3->m_msg.gao;
        if (a3->m_msg.msgID == 1)
        {
            v7 = a3->m_msg.gao2;
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
                a3->des_o_target = (int)a3->m_msg.gao2;
            if (a3->m_msg.f2 >= 0.0)
                a3->des_f_rotation_speed_max = a3->m_msg.f2;
            if (a3->m_msg.f1 >= 0.0)
                a3->des_f_init_speed = a3->m_msg.f1;
            if (a3->m_msg.Int1 >= 0)
                a3->des_f_life_time = (float)a3->m_msg.Int1;
            if (a3->m_msg.Int2 >= 0)
                a3->des_f_traction = (float)a3->m_msg.Int2;
            if (fabs(a3->m_msg.v1.x) != 0.0f)
            {
                v8 = 0;
            }
            else if (fabs(a3->m_msg.v1.y) != 0.0f)
            {
                v8 = 0;
            }
            else
            {
                v8 = fabs(a3->m_msg.v1.z) == 0.0f;
            }
            if (!v8)
                a3->v_target_pos = a3->m_msg.v1;
            if (fabs(a3->m_msg.v2.x) != 0.0f)
            {
                v16 = 0;
            }
            else if (fabs(a3->m_msg.v2.y) != 0.0f)
            {
                v16 = 0;
            }
            else
            {
                v16 = fabs(a3->m_msg.v2.z) == 0.0f;
            }
            if (!v16)
                a3->des_f_friction = a3->m_msg.v2.x;
        }
        v22 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&a3->m_msg, v22, sizeof(a3->m_msg));
    }
    v5 = (tGameObjectMain*)a3->des_o_target;
    if (a3->des_o_target)
    {
        if ((unsigned int)v5 <= 2)
            v5 = AI_MainActor[(uint32_t)v5 - 1];
        AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, v5, &a3->des_o_target);
    }
    else
    {
        AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, 0, &a3->des_o_target);
    }
    v11.x = -400.0;
    v11.y = -400.0;
    v11.z = -400.0;
    AI_EvalFunc_BVMinSet_C(AI_gpst_CurrentGameObject, &v11);
    a2.x = 400.0;
    a2.y = 400.0;
    a2.z = 400.0;
    AI_EvalFunc_BVMaxSet_C(AI_gpst_CurrentGameObject, &a2);
    a3->m_msg.msgID = 77;
    a3->m_msg.gao = AI_gpst_CurrentGameObject;
    a3->m_msg.gao2 = v14;
    a3->m_msg.Int1 = 1;
    a3->m_msg.f2 = a3->des_f_life_time;
    v4 = (tGameObjectMain*)a3->des_o_target;
    if (a3->des_o_target)
    {
        if ((unsigned int)v4 <= 2)
            v4 = AI_MainActor[(uint32_t)v4 - 1];
        AI_EvalFunc_MSGSend_C(v4, &a3->m_msg);
    }
    else
    {
        AI_EvalFunc_MSGSend_C(0, &a3->m_msg);
    }
    a3->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
    AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &a3->des_v_gravity);
    v18 = AI_EvalFunc_DYNFrictionSet_C(AI_gpst_CurrentGameObject, a3->des_f_friction);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v10);
    v3 = a3->des_f_init_speed;
    v23.x = v10.x * v3;
    v23.y = v10.y * v3;
    v23.z = v10.z * v3;
    v17 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v23);
    v12 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
    a3->GFX_Halo = v12;
    AI_EvalFunc_GFXFlagSet_C_CURRENT(a3->GFX_Halo, 4, 1);
    v2 = (tGameObjectMain*)Ent_missile_EM_init_SFX_light_smoke_gao;
    if (Ent_missile_EM_init_SFX_light_smoke_gao)
    {
        if ((unsigned int)Ent_missile_EM_init_SFX_light_smoke_gao <= 2)
            v2 = AI_MainActor[Ent_missile_EM_init_SFX_light_smoke_gao - 1];
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(a3->GFX_Halo, v2, 0);
    }
    else
    {
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(a3->GFX_Halo, 0, 0);
    }
    GFXSetCurGlobal(a3->GFX_Halo);
    AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
    AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
    AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 2.0);
    AI_EvalFunc_AITrackChange_C(2, (tAIFunction*)fct_Ent_missile_EM_init_Ent_missile_EM_comportement);
    v15 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Ent_missile_EM_init_Ent_missile_EM_reflex);
    if (v15 == 1000)
        result = 1000;
    else
        result = v15;
    return result;
}

int Ent_missile_EM_reflex()
{
    int result; // eax
    tScriptStruct_EntMissile* v1; // [esp+8h] [ebp-28h]
    tGameObjectMain* v2; // [esp+Ch] [ebp-24h]
    tGameObjectMain* v3; // [esp+10h] [ebp-20h]
    void* v4; // [esp+14h] [ebp-1Ch]
    void* Src; // [esp+20h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_EntMissile*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Ent_missile_EM_reflex_Ent_missile_EM_disparition = (int)AI_ul_RealResolveOneRefFunction(0x72001317u);
    }
    v3 = (tGameObjectMain*)v1->des_o_target;
    if (v1->des_o_target)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        v2 = v3;
    }
    else
    {
        v2 = 0;
    }
    if (v2 && (v1->des_f_life_time <= 0.0 || !AI_EvalFunc_TIMEElapsed_C(v1->f_time_start, v1->des_f_life_time)))
    {
        Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v1->m_msg, Src, sizeof(v1->m_msg));
        while (v1->m_msg.msgID)
        {
            if (v1->m_msg.msgID == 37 && (v1->m_msg.Int1 & 7u) > 5)
                goto LABEL_19;
            v4 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
            memcpy(&v1->m_msg, v4, sizeof(v1->m_msg));
        }
        result = 1000;
    }
    else
    {
    LABEL_19:
        v1->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        AI_EvalFunc_AITrackChange_C(2, (tAIFunction*)fct_Ent_missile_EM_reflex_Ent_missile_EM_disparition);
        AI_EvalFunc_AITrackCurStop_C();
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int Ent_missile_EM_explosion()
{
    int result; // eax
    float v1; // [esp+0h] [ebp-1B4h]
    float size; // [esp+4h] [ebp-1B0h]
    float v3; // [esp+8h] [ebp-1ACh]
    float v4; // [esp+Ch] [ebp-1A8h]
    float v5; // [esp+10h] [ebp-1A4h]
    tScriptStruct_EntMissile* v6; // [esp+18h] [ebp-19Ch]
    float v7; // [esp+1Ch] [ebp-198h]
    float v8; // [esp+20h] [ebp-194h]
    tGameObjectMain* v9; // [esp+28h] [ebp-18Ch]
    tGameObjectMain* v10; // [esp+2Ch] [ebp-188h]
    tGameObjectMain* v11; // [esp+30h] [ebp-184h]
    tGameObjectMain* v12; // [esp+38h] [ebp-17Ch]
    tGameObjectMain* v13; // [esp+5Ch] [ebp-158h]
    tGameObjectMain* v14; // [esp+60h] [ebp-154h]
    tGameObjectMain* v15; // [esp+64h] [ebp-150h]
    tGameObjectMain* v16; // [esp+68h] [ebp-14Ch]
    tGameObjectMain* v17; // [esp+6Ch] [ebp-148h]
    float v18; // [esp+70h] [ebp-144h]
    tGameObjectMain* v19; // [esp+78h] [ebp-13Ch]
    tMessage Src; // [esp+90h] [ebp-124h] BYREF
    Vec3 v22; // [esp+C8h] [ebp-ECh]
    float v23; // [esp+D8h] [ebp-DCh]
    Vec3 a3; // [esp+E0h] [ebp-D4h] BYREF
    Vec3 v25; // [esp+ECh] [ebp-C8h] BYREF
    int v26; // [esp+F8h] [ebp-BCh]
    Vec3 v; // [esp+100h] [ebp-B4h] BYREF
    int v28; // [esp+10Ch] [ebp-A8h]
    float v29; // [esp+110h] [ebp-A4h]
    float v30; // [esp+114h] [ebp-A0h]
    Vec3 v31; // [esp+11Ch] [ebp-98h] BYREF
    float v32; // [esp+134h] [ebp-80h]
    Vec3 pos; // [esp+138h] [ebp-7Ch] BYREF
    Vec3 v34; // [esp+144h] [ebp-70h] BYREF
    unsigned int v35; // [esp+150h] [ebp-64h]
    tGameObjectMain* v36; // [esp+154h] [ebp-60h]
    int i; // [esp+158h] [ebp-5Ch]
    float v38; // [esp+15Ch] [ebp-58h]
    int v39; // [esp+160h] [ebp-54h]
    float v40; // [esp+164h] [ebp-50h]
    tScriptStruct_EntMissile* v41; // [esp+168h] [ebp-4Ch]
    Vec3 dest; // [esp+16Ch] [ebp-48h] BYREF
    float v43; // [esp+178h] [ebp-3Ch]
    Vec3 v44; // [esp+17Ch] [ebp-38h] BYREF
    float v45; // [esp+188h] [ebp-2Ch]
    tGameObjectMain* a7[10]; // [esp+18Ch] [ebp-28h] BYREF

    v.x = 1.0;
    v.y = 0.0;
    v.z = 0.0;
    v22.x = 1.0;
    v22.y = 0.0;
    v22.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_EntMissile*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v41 = v6;
    v36 = 0;
    i = 0;
    v23 = 0.0;
    v39 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Ent_missile_EM_explosion_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
    }
    if (*AI_gppst_CurrentJumpNode != (tAINode*)9)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v25);
        v22 = v25;
        if (v41->des_explosion)
        {
            if (v41->des_f_explosion_size_rand == 0.0)
            {
                v23 = 1.0;
            }
            else
            {
                size = v41->des_f_explosion_size_rand + 1.0f;
                v1 = 1.0f - v41->des_f_explosion_size_rand;
                v40 = AI_EvalFunc_MATHRandFloat_C(v1, size);
                v23 = v41->des_f_explosion_size * v40;
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
            v36 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)Ent_missile_EM_explosion_SFX_explosion_gao, &pos);
            Src.msgID = 1;
            Src.Int1 = v41->des_i_num_explosion;
            Src.f2 = v23;
            Src.f1 = v41->des_f_explosion_speed;
            Src.v1.x = v41->des_f_explosion_detail;
            Src.v1.y = 0.0;
            Src.v1.z = 1.0;
            v19 = v36;
            if (v36)
            {
                if ((unsigned int)v36 <= 2)
                    v19 = AI_MainActor[(uint32_t)v36 - 1];
                AI_EvalFunc_MSGSend_C(v19, &Src);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &Src);
            }
        }
        v39 = AI_EvalFunc_COLZDEZDEListGet_C(AI_gpst_CurrentGameObject, a7, 6, 1, 32800, 0, 0);
        for (i = 0; ; ++i)
        {
            v18 = (float)v39;
            v5 = v18 > 10.0f ? 10.0f : (float)v39;
            v43 = v5;
            v4 = (float)i;
            if (v4 >= (double)v5)
                break;
            v17 = (tGameObjectMain*)v41->o_collide_object;
            if (v17)
            {
                if ((unsigned int)v17 <= 2)
                    v17 = AI_MainActor[(uint32_t)v17 - 1];
                v16 = v17;
            }
            else
            {
                v16 = 0;
            }
            v15 = a7[i];
            if (v15)
            {
                if ((unsigned int)v15 <= 2)
                    v15 = AI_MainActor[(uint32_t)v15 - 1];
                v14 = v15;
            }
            else
            {
                v14 = 0;
            }
            if (v16 != v14)
            {
                v13 = a7[i];
                if (v13)
                {
                    if ((unsigned int)v13 <= 2)
                        v13 = AI_MainActor[(uint32_t)v13 - 1];
                    AI_EvalFunc_OBJGetPos_C(v13, &a3);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &a3);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v34);
                MATH_SubstractVec(&dest, &a3, &v34);
                v = dest;
                MATH_NormalizeEqualVector(&v);
                v41->m_msg.msgID = 37;
                v41->m_msg.gao = AI_gpst_CurrentGameObject;
                v41->m_msg.gao2 = 0;
                v41->m_msg.Int1 = v41->des_puissance + 96;
                v41->m_msg.Int2 = 0;
                v41->m_msg.v1 = v;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v41->m_msg.v2);
                v41->m_msg.f2 = (float)v41->des_i_point_de_vie;
                v41->m_msg.f1 = 1000.0;
                v12 = a7[i];
                if (v12)
                {
                    if ((unsigned int)v12 <= 2)
                        v12 = AI_MainActor[(uint32_t)v12 - 1];
                    AI_EvalFunc_MSGSend_C(v12, &v41->m_msg);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &v41->m_msg);
                }
            }
        }
        v11 = (tGameObjectMain*)v41->des_o_target;
        if (v41->des_o_target)
        {
            if ((unsigned int)v11 <= 2)
                v11 = AI_MainActor[(uint32_t)v11 - 1];
            v10 = v11;
        }
        else
        {
            v10 = 0;
        }
        if (v10)
        {
            v41->m_msg.msgID = 77;
            v41->m_msg.gao = AI_gpst_CurrentGameObject;
            v41->m_msg.Int1 = 0;
            v9 = (tGameObjectMain*)v41->des_o_target;
            if (v41->des_o_target)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                AI_EvalFunc_MSGSend_C(v9, &v41->m_msg);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &v41->m_msg);
            }
        }
        AI_EvalFunc_AITrackStop_C(1);
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        v44.x = 0.0;
        v44.y = 0.0;
        v44.z = 0.0;
        AI_EvalFunc_DYNTractionSet_C_CURRENT(&v44);
        v31.x = 0.0;
        v31.y = 0.0;
        v31.z = 0.0;
        v26 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v31);
        v29 = TIM_gf_MainClock - TIM_gf_LockTime;
        v41->f_time_disp = v29;
        v41->f_size_halo = 2.0;
    }
    if (AI_EvalFunc_TIMEElapsed_C(v41->f_time_disp, 2.0))
    {
        v35 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0, 0x10000000);
        v28 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v28 == 1000)
            result = 1000;
        else
            result = v28;
    }
    else
    {
        v30 = TIM_gf_dt;
        v8 = 4.0f * TIM_gf_dt;
        if (v8 < 0.0)
            v8 = 0.0;
        if (v8 > 1.0)
            v8 = 1.0;
        v38 = 0.0f * v8 + v41->f_largeur_X_men * (1.0f - v8);
        v41->f_largeur_X_men = v38;
        XMEN_SetSize(AI_gpst_CurrentGameObject, 0, -1, v41->f_largeur_X_men);
        v32 = TIM_gf_dt;
        v7 = v41->f_size_halo - TIM_gf_dt * 2.4000001f;
        if (v7 < 0.0)
            v3 = 0.0;
        else
            v3 = v41->f_size_halo - TIM_gf_dt * 2.4000001f;
        v45 = v3;
        v41->f_size_halo = v3;
        AI_EvalFunc_GFXSetf_C_CURRENT(v41->GFX_Halo, 21000, v41->f_size_halo);
        *AI_gppst_CurrentJumpNode = (tAINode*)9;
        result = 0;
    }
    return result;
}