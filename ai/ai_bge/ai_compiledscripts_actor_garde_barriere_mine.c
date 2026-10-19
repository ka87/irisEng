#include "ai_compiledscripts_inc.h"

//Garde_Barriere_Mine_GBM_afterrec
int Garde_Barriere_Mine_GBM_afterrec_PJ_Camera_gao;
int Garde_Barriere_Mine_GBM_afterrec_PJ_Camera_gao_i_usertomario;
int Garde_Barriere_Mine_GBM_afterrec_PJ_Camera_gao_f_ideal_dist;
int Garde_Barriere_Mine_GBM_afterrec__PJ_Aircraft_gao;
int Garde_Barriere_Mine_GBM_afterrec_PJ_Camera_gao_f_ideal_Z;

//Garde_Barriere_Mine_GBM_init
int Garde_Barriere_Mine_GBM_init_univVars;
int Garde_Barriere_Mine_GBM_init_univVars_Flag_speciaux_sauve;
int Garde_Barriere_Mine_GBM_init_GST_Global_gao;
int Garde_Barriere_Mine_GBM_init_GST_Global_gao_o_targets;
int Garde_Barriere_Mine_GBM_init_GST_Global_gao_i_targets_nb;
int Garde_Barriere_Mine_GBM_init_GST_Global_gao_v_targets_offset;
int fct_Garde_Barriere_Mine_GBM_init_Garde_Barriere_Mine_GBM_exec_GFX;
int Garde_Barriere_Mine_GBM_init_univVars_Planetes_CurHour;
int fct_Garde_Barriere_Mine_GBM_init_Garde_Barriere_Mine_GBM_attend;
int fct_Garde_Barriere_Mine_GBM_init_Garde_Barriere_Mine_GBM_depart;

//Garde_Barriere_Mine_GBM_reflex
int Garde_Barriere_Mine_GBM_reflex_SFX_explosion_gao;
int Garde_Barriere_Mine_GBM_reflex_univVars;
int Garde_Barriere_Mine_GBM_reflex_univVars_Planetes_CurHour;

//Garde_Barriere_Mine_GBM_attaque
int Garde_Barriere_Mine_GBM_attaque__PJ_Aircraft_gao;
int fct_Garde_Barriere_Mine_GBM_attaque_Garde_Barriere_Mine_GBM_attend;

//Garde_Barriere_Mine_GBM_exec_GFX
int Garde_Barriere_Mine_GBM_exec_GFX_SFX_light_smoke_gao;

//Garde_Barriere_Mine_GBM_attend
int Garde_Barriere_Mine_GBM_attend_univVars;
int Garde_Barriere_Mine_GBM_attend_univVars_Planetes_CurHour;
int Garde_Barriere_Mine_GBM_attend__PJ_Aircraft_gao;
int fct_Garde_Barriere_Mine_GBM_attend_Garde_Barriere_Mine_GBM_attaque;

//Garde_Barriere_Mine_GBM_depart
int Garde_Barriere_Mine_GBM_depart_univVars;
int Garde_Barriere_Mine_GBM_depart_univVars_Planetes_CurHour;
int fct_Garde_Barriere_Mine_GBM_depart_Garde_Barriere_Mine_GBM_attend;

//Garde_Barriere_Mine_GBM_retour
int fct_Garde_Barriere_Mine_GBM_retour_Garde_Barriere_Mine_GBM_depart;

int Garde_Barriere_Mine_GBM_afterrec()
{
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Garde_Barriere_Mine_GBM_afterrec_PJ_Camera_gao = AI_ul_RealResolveOneRefObject(0x71003F56u);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Mine_GBM_afterrec_PJ_Camera_gao_i_usertomario,
            (void*)Garde_Barriere_Mine_GBM_afterrec_PJ_Camera_gao,
            256);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Mine_GBM_afterrec_PJ_Camera_gao_f_ideal_dist,
            (void*)Garde_Barriere_Mine_GBM_afterrec_PJ_Camera_gao,
            132);
        Garde_Barriere_Mine_GBM_afterrec__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Mine_GBM_afterrec_PJ_Camera_gao_f_ideal_Z,
            (void*)Garde_Barriere_Mine_GBM_afterrec_PJ_Camera_gao,
            124);
    }
    return 1000;
}

int Garde_Barriere_Mine_GBM_init()
{
    int result; // eax
    tScriptStruct_PNJGardeBarriereMine* v1; // [esp+0h] [ebp-128h]
    tGameObjectMain* v2; // [esp+8h] [ebp-120h]
    tGameObjectMain* v3; // [esp+10h] [ebp-118h]
    float v4; // [esp+1Ch] [ebp-10Ch]
    tGameObjectMain* v5; // [esp+30h] [ebp-F8h]
    tGameObjectMain* v6; // [esp+34h] [ebp-F4h]
    tGameObjectMain* v7; // [esp+44h] [ebp-E4h]
    tGameObjectMain* v8; // [esp+4Ch] [ebp-DCh]
    Vec3 v9; // [esp+60h] [ebp-C8h] BYREF
    float v10; // [esp+6Ch] [ebp-BCh]
    int v11; // [esp+70h] [ebp-B8h]
    Vec3 v12; // [esp+74h] [ebp-B4h] BYREF
    Vec3 v13; // [esp+84h] [ebp-A4h] BYREF
    BOOL v14; // [esp+94h] [ebp-94h]
    tGameObjectMain* v15; // [esp+9Ch] [ebp-8Ch]
    Vec3 dest; // [esp+A0h] [ebp-88h] BYREF
    int v17; // [esp+B0h] [ebp-78h]
    tGameObjectMain* v18; // [esp+B4h] [ebp-74h]
    Vec3 v19; // [esp+B8h] [ebp-70h] BYREF
    int v20; // [esp+C8h] [ebp-60h]
    int v21; // [esp+CCh] [ebp-5Ch]
    Vec3 a3; // [esp+DCh] [ebp-4Ch] BYREF
    int v23; // [esp+ECh] [ebp-3Ch]
    tScriptStruct_PNJGardeBarriereMine* v24; // [esp+F0h] [ebp-38h]
    int v25; // [esp+F4h] [ebp-34h]
    BOOL v26; // [esp+F8h] [ebp-30h]
    Vec3 v27; // [esp+108h] [ebp-20h] BYREF
    Vec3 v28; // [esp+118h] [ebp-10h] BYREF

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJGardeBarriereMine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v24 = v1;
    v20 = 0;
    v10 = 0.0;
    v26 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Garde_Barriere_Mine_GBM_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Mine_GBM_init_univVars_Flag_speciaux_sauve,
            (void*)Garde_Barriere_Mine_GBM_init_univVars,
            4488);
        Garde_Barriere_Mine_GBM_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Mine_GBM_init_GST_Global_gao_o_targets,
            (void*)Garde_Barriere_Mine_GBM_init_GST_Global_gao,
            964);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Mine_GBM_init_GST_Global_gao_i_targets_nb,
            (void*)Garde_Barriere_Mine_GBM_init_GST_Global_gao,
            2572);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Mine_GBM_init_GST_Global_gao_v_targets_offset,
            (void*)Garde_Barriere_Mine_GBM_init_GST_Global_gao,
            1368);
        fct_Garde_Barriere_Mine_GBM_init_Garde_Barriere_Mine_GBM_exec_GFX = (int)AI_ul_RealResolveOneRefFunction(0x72006276u);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Mine_GBM_init_univVars_Planetes_CurHour,
            (void*)Garde_Barriere_Mine_GBM_init_univVars,
            6116);
        fct_Garde_Barriere_Mine_GBM_init_Garde_Barriere_Mine_GBM_attend = (int)AI_ul_RealResolveOneRefFunction(0x3D000DA3u);
        fct_Garde_Barriere_Mine_GBM_init_Garde_Barriere_Mine_GBM_depart = (int)AI_ul_RealResolveOneRefFunction(0x3D000DB0u);
    }
    v8 = (tGameObjectMain*)v24->des_o_mine_base;
    if (v24->des_o_mine_base)
    {
        if ((unsigned int)v8 <= 2)
            v8 = AI_MainActor[(uint32_t)v8 - 1];
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v8, 1);
    }
    else
    {
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
    }
    v7 = (tGameObjectMain*)v24->des_o_mine_base;
    if (v24->des_o_mine_base)
    {
        if ((unsigned int)v7 <= 2)
            v7 = AI_MainActor[(uint32_t)v7 - 1];
        AI_EvalFunc_OBJ_FlagInactiveSet_C(v7, 1);
    }
    else
    {
        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
    }
    if (v24->des_i_sf_mort
        && (v24->des_i_sf_mort <= 0 ? (v26 = 0) : (v26 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Garde_Barriere_Mine_GBM_init_univVars_Flag_speciaux_sauve
                + 4),
            v24->des_i_sf_mort)),
            v26))
    {
        v23 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v23 == 1000)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            result = v23;
        }
    }
    else if (!v24->des_i_special_flag
        || (v24->des_i_special_flag <= 0 ? (v26 = 0) : (v26 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Garde_Barriere_Mine_GBM_init_univVars_Flag_speciaux_sauve
                + 4),
            v24->des_i_special_flag)),
            v26))
    {
        AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 8, 0);
        v18 = AI_gpst_CurrentGameObject;
        v6 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v6 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            v5 = v6;
        }
        else
        {
            v5 = 0;
        }
        v20 = AI_EvalFunc_ARRObjSearch_C(
            (tGameObjectMain**)(Garde_Barriere_Mine_GBM_init_GST_Global_gao_o_targets + 4),
            *(uint32_t*)Garde_Barriere_Mine_GBM_init_GST_Global_gao_i_targets_nb,
            v5);
        if (v20 == -1)
        {
            v20 = AI_EvalFunc_ARRObjSearch_C(
                (tGameObjectMain**)(Garde_Barriere_Mine_GBM_init_GST_Global_gao_o_targets + 4),
                100,
                0);
            if (v20 != -1)
            {
                ++* (uint32_t*)Garde_Barriere_Mine_GBM_init_GST_Global_gao_i_targets_nb;
                *(uint32_t*)(Garde_Barriere_Mine_GBM_init_GST_Global_gao_o_targets + 4 * v20 + 4) = (uint32_t)AI_gpst_CurrentGameObject;
                *(uint32_t*)AI_EvalVarArray_C3((char*)Garde_Barriere_Mine_GBM_init_GST_Global_gao_v_targets_offset, v20) = 0;
                *(uint32_t*)(AI_EvalVarArray_C3((char*)Garde_Barriere_Mine_GBM_init_GST_Global_gao_v_targets_offset, v20) + 4) = 0;
                *(uint32_t*)(AI_EvalVarArray_C3((char*)Garde_Barriere_Mine_GBM_init_GST_Global_gao_v_targets_offset, v20) + 8) = 0;
            }
        }
        v14 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
        v21 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 50499588, 0);
        v12.x = 0.0;
        v12.y = 0.0;
        v12.z = 0.0;
        AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v12);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v28);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v19);
        v4 = v24->des_f_dist_centre_grav;
        v13.x = v19.x * v4;
        v13.y = v19.y * v4;
        v13.z = v19.z * v4;
        MATH_SubstractVec(&dest, &v28, &v13);
        v24->v_grav_pos = dest;
        v24->v_grav_speed.x = 0.0;
        v24->v_grav_speed.y = 0.0;
        v24->v_grav_speed.z = 0.0;
        v24->f_time_pos = TIM_gf_MainClock - TIM_gf_LockTime;
        v24->o_corps = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0);
        v24->o_motor_1 = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(1u);
        v24->o_motor_2 = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(2u);
        v24->o_motor_3 = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(3u);
        v24->o_gyro = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(4u);
        v15 = AI_gpst_CurrentGameObject;
        v3 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v3 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v3,
                2,
                (tAIFunction*)fct_Garde_Barriere_Mine_GBM_init_Garde_Barriere_Mine_GBM_exec_GFX);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_Garde_Barriere_Mine_GBM_init_Garde_Barriere_Mine_GBM_exec_GFX);
        }
        v10 = *(float*)Garde_Barriere_Mine_GBM_init_univVars_Planetes_CurHour;
        if (v24->des_f_hour_min >= (double)v10 && v24->des_f_hour_max <= (double)v10)
        {
            v25 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Garde_Barriere_Mine_GBM_init_Garde_Barriere_Mine_GBM_depart);
            if (v25 != 1000)
                return v25;
        }
        else
        {
            v2 = (tGameObjectMain*)v24->des_o_wp_de_base;
            if (v2)
            {
                if ((unsigned int)v2 <= 2)
                    v2 = AI_MainActor[(uint32_t)v2 - 1];
                AI_EvalFunc_OBJGetPos_C(v2, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            v9.x = 0.0;
            v9.y = 0.0;
            v9.z = 20.0;
            MATH_VecAddVec(&v27, &a3, &v9);
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v27);
            v17 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Garde_Barriere_Mine_GBM_init_Garde_Barriere_Mine_GBM_attend);
            if (v17 != 1000)
                return v17;
        }
        result = 1000;
    }
    else
    {
        v11 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v11 == 1000)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            result = v11;
        }
    }
    return result;
}

int Garde_Barriere_Mine_GBM_reflex()
{
    int v1; // eax
    float a4; // [esp+0h] [ebp-26Ch]
    float a4a; // [esp+0h] [ebp-26Ch]
    tScriptStruct_PNJGardeBarriereMine* v4; // [esp+Ch] [ebp-260h]
    tGameObjectMain* v5; // [esp+18h] [ebp-254h]
    tGameObjectMain* v6; // [esp+20h] [ebp-24Ch]
    Vec3* v7; // [esp+48h] [ebp-224h]
    Vec3* v8; // [esp+4Ch] [ebp-220h]
    Vec3* v9; // [esp+50h] [ebp-21Ch]
    float v10; // [esp+58h] [ebp-214h]
    Vec3 v11; // [esp+7Ch] [ebp-1F0h] BYREF
    float v12; // [esp+88h] [ebp-1E4h]
    Vec3 a2; // [esp+8Ch] [ebp-1E0h] BYREF
    tMessage v14; // [esp+98h] [ebp-1D4h] BYREF
    int v15; // [esp+D0h] [ebp-19Ch]
    float v16; // [esp+D4h] [ebp-198h]
    float v17; // [esp+D8h] [ebp-194h]
    float v18; // [esp+DCh] [ebp-190h]
    float v19; // [esp+E0h] [ebp-18Ch]
    float v20; // [esp+E4h] [ebp-188h]
    float v21; // [esp+E8h] [ebp-184h]
    tGameObjectMain* v22; // [esp+ECh] [ebp-180h]
    Vec3 a3; // [esp+F0h] [ebp-17Ch] BYREF
    Vec3 v24; // [esp+FCh] [ebp-170h] BYREF
    int v25; // [esp+108h] [ebp-164h]
    int v26; // [esp+10Ch] [ebp-160h]
    float v27; // [esp+110h] [ebp-15Ch]
    float v28; // [esp+114h] [ebp-158h]
    Vec3 v29; // [esp+118h] [ebp-154h] BYREF
    Vec3 v30; // [esp+124h] [ebp-148h] BYREF
    Vec3 v31; // [esp+130h] [ebp-13Ch] BYREF
    int v32; // [esp+13Ch] [ebp-130h]
    tMessage v33; // [esp+140h] [ebp-12Ch] BYREF
    Vec3 v34; // [esp+174h] [ebp-F8h] BYREF
    Vec3 v35; // [esp+180h] [ebp-ECh] BYREF
    Vec3 a5[2]; // [esp+18Ch] [ebp-E0h] BYREF
    Vec3 v37; // [esp+1A8h] [ebp-C4h] BYREF
    Vec3 v38; // [esp+1B4h] [ebp-B8h] BYREF
    float v39; // [esp+1C0h] [ebp-ACh]
    float v40; // [esp+1C4h] [ebp-A8h]
    int v41; // [esp+1C8h] [ebp-A4h]
    Vec3 v42; // [esp+1CCh] [ebp-A0h] BYREF
    int v43; // [esp+1D8h] [ebp-94h]
    float v44; // [esp+1DCh] [ebp-90h]
    void* Src; // [esp+1E0h] [ebp-8Ch]
    Vec3 v46; // [esp+1E4h] [ebp-88h] BYREF
    float v47; // [esp+1F0h] [ebp-7Ch]
    float v48; // [esp+1F4h] [ebp-78h]
    float v49; // [esp+1F8h] [ebp-74h]
    Vec3 v50; // [esp+1FCh] [ebp-70h] BYREF
    int v51; // [esp+208h] [ebp-64h]
    Vec3 dest; // [esp+20Ch] [ebp-60h] BYREF
    tScriptStruct_PNJGardeBarriereMine* v53; // [esp+218h] [ebp-54h]
    Vec3 pos; // [esp+220h] [ebp-4Ch] BYREF
    float v55; // [esp+238h] [ebp-34h]
    float v56; // [esp+23Ch] [ebp-30h]
    float v57; // [esp+240h] [ebp-2Ch]
    Vec3 v58; // [esp+244h] [ebp-28h] BYREF
    void* v59; // [esp+250h] [ebp-1Ch]
    Vec3 v60; // [esp+254h] [ebp-18h] BYREF

    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    v37.x = 1.0;
    v37.y = 0.0;
    v37.z = 0.0;
    pos.x = 1.0;
    pos.y = 0.0;
    pos.z = 0.0;
    v30.x = 1.0;
    v30.y = 0.0;
    v30.z = 0.0;
    v50.x = 1.0;
    v50.y = 0.0;
    v50.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_PNJGardeBarriereMine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v53 = v4;
    v32 = 0;
    v43 = 0;
    v15 = 0;
    v22 = 0;
    v51 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Garde_Barriere_Mine_GBM_reflex_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
        Garde_Barriere_Mine_GBM_reflex_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Mine_GBM_reflex_univVars_Planetes_CurHour,
            (void*)Garde_Barriere_Mine_GBM_reflex_univVars,
            4488);
    }
    if (*AI_gppst_CurrentJumpNode != (tAINode*)6)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v24);
        MATH_SubstractVec(&dest, &v53->v_pos_togo, &v24);
        AI_EvalFunc_MATHVecNormalize_C(&dest, &v42);
        a3 = v42;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
        v12 = TIM_gf_dt;
        a4 = 3.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, a5);
        a3 = a5[0];
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v58);
        v50 = v58;
        v30 = v53->v_grav_pos;
        v10 = v53->des_f_air_friction;
        v19 = v10 * v53->v_grav_speed.x;
        v20 = v53->v_grav_speed.y * v10;
        v21 = v53->v_grav_speed.z * v10;
        v44 = TIM_gf_dt;
        v55 = v19 * TIM_gf_dt;
        v56 = v20 * TIM_gf_dt;
        v57 = v21 * TIM_gf_dt;
        v9 = &v53->v_grav_pos;
        v53->v_grav_pos.x = v55 + v53->v_grav_pos.x;
        v9->y = v56 + v9->y;
        v9->z = v57 + v9->z;
        v25 = 0;
        v26 = 0;
        v27 = v53->des_f_air_gravity;
        v40 = TIM_gf_dt;
        v16 = 0.0f * TIM_gf_dt;
        v17 = v16;
        v18 = v27 * TIM_gf_dt;
        v8 = &v53->v_grav_pos;
        v53->v_grav_pos.x = v16 + v53->v_grav_pos.x;
        v8->y = v17 + v8->y;
        v8->z = v18 + v8->z;
        MATH_SubstractVec(&v53->v_grav_pos, &v53->v_grav_pos, &v50);
        AI_EvalFunc_MATHVecSetNormVector_C(&v53->v_grav_pos, v53->des_f_dist_centre_grav);
        v7 = &v53->v_grav_pos;
        v53->v_grav_pos.x = v50.x + v53->v_grav_pos.x;
        v7->y = v50.y + v7->y;
        v7->z = v50.z + v7->z;
        MATH_SubstractVec(&v38, &v53->v_grav_pos, &v30);
        v28 = TIM_gf_dt;
        MATH_VecMultNormScalar(&v34, &v38, TIM_gf_dt);
        v53->v_grav_speed = v34;
        MATH_SubstractVec(&v11, &v50, &v53->v_grav_pos);
        AI_EvalFunc_MATHVecNormalize_C(&v11, &v35);
        v37 = v35;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v46);
        v39 = TIM_gf_dt;
        a4a = 8.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&v46, &v37, a4a, &v60);
        v37 = v60;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a3, &v37);
        Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v14, Src, sizeof(v14));
        goto LABEL_9;
    }
    v1 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
    v41 = v1;
    if (v1 != 1000)
        return v41;
    while (1)
    {
        v59 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v14, v59, sizeof(v14));
    LABEL_9:
        if (!v14.msgID)
            return 1000;
        if (v14.msgID == 37)
        {
            v43 = v14.Int1 & 7;
            v51 = v14.Int1 & 0x60;
            v32 = v14.Int1 & 0x18;
            if ((v14.Int1 & 0x60) != 0 && v43 >= 4)
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v31);
                pos = v31;
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v29);
                v47 = v29.x * 8.0f;
                v48 = v29.y * 8.0f;
                v49 = v29.z * 8.0f;
                pos.x = pos.x - v47;
                pos.y = pos.y - v48;
                pos.z = pos.z - v49;
                v22 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)Garde_Barriere_Mine_GBM_reflex_SFX_explosion_gao, &pos);
                v33.msgID = 1;
                v33.Int1 = 37;
                v33.f2 = 10.0;
                v33.f1 = 0.0;
                v33.v1.x = 1.5;
                v33.v1.y = 0.0;
                v33.v1.z = 0.0;
                v6 = v22;
                if (v22)
                {
                    if ((unsigned int)v22 <= 2)
                        v6 = AI_MainActor[(uint32_t)v22 - 1];
                    AI_EvalFunc_MSGSend_C(v6, &v33);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &v33);
                }
                v15 = AI_EvalFunc_SNDRequest_C(
                    (tGameObjectMain*)Garde_Barriere_Mine_GBM_reflex_SFX_explosion_gao,
                    1,
                    402653184);
                AI_EvalFunc_SNDUltraPlaySound_C(AI_gpst_CurrentGameObject, v15);
                if (!--v53->des_i_life)
                    break;
            }
        }
    }
    v22 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)Garde_Barriere_Mine_GBM_reflex_SFX_explosion_gao, &pos);
    v33.msgID = 1;
    v33.Int1 = 37;
    v33.f2 = 10.0;
    v33.f1 = 0.0;
    v33.v1.x = 1.5;
    v33.v1.y = 0.0;
    v33.v1.z = 0.0;
    v5 = v22;
    if (v22)
    {
        if ((unsigned int)v22 <= 2)
            v5 = AI_MainActor[(uint32_t)v22 - 1];
        AI_EvalFunc_MSGSend_C(v5, &v33);
    }
    else
    {
        AI_EvalFunc_MSGSend_C(0, &v33);
    }
    if (v53->des_i_sf_mort && v53->des_i_sf_mort > 0)
        AI_EvalFunc_ARRBitSet_C((int*)(Garde_Barriere_Mine_GBM_reflex_univVars_Planetes_CurHour + 4), v53->des_i_sf_mort);
    if (v53->GFX_Reacteur_1 != -1)
    {
        AI_EvalFunc_GFXDel_C_CURRENT(v53->GFX_Reacteur_1);
        v53->GFX_Reacteur_1 = -1;
    }
    if (v53->GFX_Reacteur_2 != -1)
    {
        AI_EvalFunc_GFXDel_C_CURRENT(v53->GFX_Reacteur_2);
        v53->GFX_Reacteur_2 = -1;
    }
    if (v53->GFX_Reacteur_3 != -1)
    {
        AI_EvalFunc_GFXDel_C_CURRENT(v53->GFX_Reacteur_3);
        v53->GFX_Reacteur_3 = -1;
    }
    AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
    *AI_gppst_CurrentJumpNode = (tAINode*)6;
    TIME_Wait_C(0.1f);
    return 0;
}

int Garde_Barriere_Mine_GBM_attaque()
{
    int v1; // eax
    tGameObjectMain* v2; // eax
    float v3; // [esp+0h] [ebp-2C0h]
    tScriptStruct_PNJGardeBarriereMine* v4; // [esp+8h] [ebp-2B8h]
    tGameObjectMain* v5; // [esp+10h] [ebp-2B0h]
    tGameObjectMain* v6; // [esp+28h] [ebp-298h]
    tGameObjectMain* v7; // [esp+30h] [ebp-290h]
    tGameObjectMain* v8; // [esp+38h] [ebp-288h]
    tGameObjectMain* v9; // [esp+40h] [ebp-280h]
    tGameObjectMain* v10; // [esp+48h] [ebp-278h]
    float v11; // [esp+54h] [ebp-26Ch]
    float v12; // [esp+90h] [ebp-230h]
    tGameObjectMain* v13; // [esp+9Ch] [ebp-224h]
    tGameObjectMain* v14; // [esp+A0h] [ebp-220h]
    tGameObjectMain* v15; // [esp+B8h] [ebp-208h]
    Vec3 v16; // [esp+C0h] [ebp-200h] BYREF
    float v17; // [esp+D0h] [ebp-1F0h]
    Vec3 v18; // [esp+D4h] [ebp-1ECh] BYREF
    int v19; // [esp+E0h] [ebp-1E0h]
    Vec3 v21; // [esp+ECh] [ebp-1D4h] BYREF
    Vec3 v22; // [esp+F8h] [ebp-1C8h] BYREF
    Vec3 v23; // [esp+104h] [ebp-1BCh] BYREF
    Vec3 pos; // [esp+110h] [ebp-1B0h] BYREF
    Vec3 a3; // [esp+11Ch] [ebp-1A4h] BYREF
    float v26; // [esp+128h] [ebp-198h]
    float v27; // [esp+12Ch] [ebp-194h]
    float v28; // [esp+130h] [ebp-190h]
    int v29; // [esp+134h] [ebp-18Ch]
    float v30; // [esp+138h] [ebp-188h]
    Vec3 v31; // [esp+13Ch] [ebp-184h] BYREF
    float v32; // [esp+148h] [ebp-178h]
    float v33; // [esp+14Ch] [ebp-174h]
    Vec3 v34; // [esp+150h] [ebp-170h] BYREF
    Vec3 v35; // [esp+15Ch] [ebp-164h] BYREF
    Vec3 v36; // [esp+168h] [ebp-158h]
    Vec3 a4; // [esp+174h] [ebp-14Ch] BYREF
    Vec3 a1; // [esp+180h] [ebp-140h] BYREF
    tGameObjectMain* v39; // [esp+190h] [ebp-130h]
    Vec3 v40; // [esp+198h] [ebp-128h] BYREF
    Vec3 v41; // [esp+1A4h] [ebp-11Ch] BYREF
    Vec3 v42; // [esp+1B0h] [ebp-110h] BYREF
    Vec3 v43; // [esp+1BCh] [ebp-104h] BYREF
    Vec3 dest; // [esp+1D0h] [ebp-F0h] BYREF
    Vec3 v46; // [esp+1DCh] [ebp-E4h] BYREF
    tMessage Src; // [esp+1E8h] [ebp-D8h] BYREF
    float v48; // [esp+21Ch] [ebp-A4h]
    Vec3 v49; // [esp+220h] [ebp-A0h] BYREF
    Vec3 v50; // [esp+22Ch] [ebp-94h] BYREF
    Vec3 v51; // [esp+238h] [ebp-88h] BYREF
    float v52; // [esp+248h] [ebp-78h]
    Vec3 v53; // [esp+258h] [ebp-68h] BYREF
    tScriptStruct_PNJGardeBarriereMine* v54; // [esp+268h] [ebp-58h]
    Vec3 v55; // [esp+270h] [ebp-50h] BYREF
    Vec3 v56; // [esp+27Ch] [ebp-44h] BYREF
    unsigned int v58; // [esp+28Ch] [ebp-34h]
    float v59; // [esp+290h] [ebp-30h]
    int v60; // [esp+294h] [ebp-2Ch]
    Vec3 v61; // [esp+298h] [ebp-28h] BYREF
    float v62; // [esp+2A4h] [ebp-1Ch]
    float v63; // [esp+2A8h] [ebp-18h]
    float v64; // [esp+2ACh] [ebp-14h]
    Vec3 v65; // [esp+2B0h] [ebp-10h] BYREF

    v42.x = 1.0;
    v42.y = 0.0;
    v42.z = 0.0;
    v50.x = 1.0;
    v50.y = 0.0;
    v50.z = 0.0;
    v31.x = 1.0;
    v31.y = 0.0;
    v31.z = 0.0;
    v18.x = 1.0;
    v18.y = 0.0;
    v18.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_PNJGardeBarriereMine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v54 = v4;
    v58 = 0;
    v29 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Garde_Barriere_Mine_GBM_attaque__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        fct_Garde_Barriere_Mine_GBM_attaque_Garde_Barriere_Mine_GBM_attend = (int)AI_ul_RealResolveOneRefFunction(0x3D000DA3u);
    }
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(256, 0);
    v54->i_attaque = 1;
    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Garde_Barriere_Mine_GBM_attaque__PJ_Aircraft_gao, &a3);
    v15 = (tGameObjectMain*)v54->des_o_wp_de_base;
    if (v15)
    {
        if ((unsigned int)v15 <= 2)
            v15 = AI_MainActor[(uint32_t)v15 - 1];
        AI_EvalFunc_OBJGetPos_C(v15, &v53);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v53);
    }
    MATH_SubstractVec(&dest, &a3, &v53);
    a1 = dest;
    MATH_VecLen(&a1);
    v17 = MATH_VecLen(&a1);
    if (v54->des_f_dist_retour < (double)v17)
        goto LABEL_65;
    v39 = *AI_MainActor;
    v14 = v39;
    if (v39)
    {
        if ((unsigned int)v39 <= 2)
            v14 = AI_MainActor[(uint32_t)v39 - 1];
        v13 = v14;
    }
    else
    {
        v13 = 0;
    }
    if (v13 != (tGameObjectMain*)Garde_Barriere_Mine_GBM_attaque__PJ_Aircraft_gao)
    {
    LABEL_65:
        v54->f_time_mine = 0.0;
        v54->i_mode_arrdep = 0;
        v60 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Garde_Barriere_Mine_GBM_attaque_Garde_Barriere_Mine_GBM_attend);
        if (v60 != 1000)
            return v60;
    }
    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Garde_Barriere_Mine_GBM_attaque__PJ_Aircraft_gao, &v22);
    v18.x = v22.x;
    v18.y = v22.y;
    v18.z = v22.z + 30.0f;
    AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Garde_Barriere_Mine_GBM_attaque__PJ_Aircraft_gao, &v46);
    v59 = AI_EvalFunc_DYNSpeedGet_C((tGameObjectMain*)Garde_Barriere_Mine_GBM_attaque__PJ_Aircraft_gao);
    v12 = v59 * 3.0f;
    v26 = v46.x * v12;
    v27 = v46.y * v12;
    v28 = v46.z * v12;
    v31.x = v26;
    v31.y = v27;
    v31.z = 0.0;
    MATH_VecAddVec(&v43, &v18, &v31);
    v42 = v43;
    if (AI_EvalFunc_TIMEElapsed_C(v54->f_time_pos, 2.0))
    {
        v54->f_time_pos = TIM_gf_MainClock - TIM_gf_LockTime;
        v54->v_add_pos.x = AI_EvalFunc_MATHRandFloat_C(30.0, 50.0);
        if (v54->v_add_pos_blend.x >= 0.0)
            v54->v_add_pos.x = -v54->v_add_pos.x;
    }
    v30 = TIM_gf_dt;
    v3 = 6.0f * TIM_gf_dt;
    AI_EvalFunc_MATHVecBlend_C(&v54->v_add_pos_blend, &v54->v_add_pos, v3, &a4);
    v54->v_add_pos_blend = a4;
    AI_EvalFunc_MATHVecLocalToGlobal_C(
        (tGameObjectMain*)Garde_Barriere_Mine_GBM_attaque__PJ_Aircraft_gao,
        &v54->v_add_pos_blend,
        &v16);
    v42.x = v42.x + v16.x;
    v42.y = v42.y + v16.y;
    v42.z = v42.z + v16.z;
    v54->v_pos_togo = v42;
    v36.x = 0.0;
    v36.y = 0.0;
    v36.z = 10.0;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v49);
    MATH_SubstractVec(&v41, &v42, &v49);
    a1 = v41;
    AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v61);
    v50 = v61;
    v48 = MATH_VecLen(&v50);
    v32 = MATH_VecLen(&a1);
    v33 = TIM_gf_dt;
    v11 = 3.0f * TIM_gf_dt;
    if (v11 < 0.0)
        v11 = 0.0;
    if (v11 > 1.0)
        v11 = 1.0;
    v52 = v32 * v11 + (1.0f - v11) * v48;
    AI_EvalFunc_MATHVecSetNormVector_C(&a1, v52);
    v19 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &a1);
    if (AI_EvalFunc_TIMEElapsed_C(v54->f_time_mine1, v54->f_alea_mine))
    {
        v54->f_alea_mine = AI_EvalFunc_MATHRandFloat_C(3.0, 5.0);
        v54->i_num_mine = AI_EvalFunc_MATHRandInt_C(4, 6);
        v54->f_time_mine1 = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    if (AI_EvalFunc_TIMEElapsed_C(v54->f_time_mine, 0.5))
    {
        if (v54->i_num_mine)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v56);
            a1 = v56;
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v21);
            v62 = v21.x * 12.0f;
            v63 = v21.y * 12.0f;
            v64 = v21.z * 12.0f;
            a1.x = a1.x - v62;
            a1.y = a1.y - v63;
            a1.z = a1.z - v64;
            if (a1.z > 2.0)
            {
                --v54->i_num_mine;
                v54->f_time_mine = TIM_gf_MainClock - TIM_gf_LockTime;
                v10 = (tGameObjectMain*)v54->des_o_mine_base;
                if (v54->des_o_mine_base)
                {
                    if ((unsigned int)v10 <= 2)
                        v10 = AI_MainActor[(uint32_t)v10 - 1];
                    v1 = AI_EvalFunc_SNDRequest_C(v10, 2, 0x18000080);
                }
                else
                {
                    v1 = AI_EvalFunc_SNDRequest_C(0, 2, 0x18000080);
                }
                v29 = v1;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
                v9 = (tGameObjectMain*)v54->des_o_mine_base;
                if (v54->des_o_mine_base)
                {
                    if ((unsigned int)v9 <= 2)
                        v9 = AI_MainActor[(uint32_t)v9 - 1];
                    v2 = AI_EvalFunc_OBJDuplicate_C(v9, &pos);
                }
                else
                {
                    v2 = AI_EvalFunc_OBJDuplicate_C(0, &pos);
                }
                v58 = (unsigned int)v2;
                v8 = v2;
                if (v2)
                {
                    if ((unsigned int)v2 <= 2)
                        v8 = AI_MainActor[(uint32_t)v2 - 1];
                    AI_EvalFunc_SNDUltraPlaySound_C(v8, v29);
                }
                else
                {
                    AI_EvalFunc_SNDUltraPlaySound_C(0, v29);
                }
                v7 = (tGameObjectMain*)v58;
                if (v58)
                {
                    if (v58 <= 2)
                        v7 = AI_MainActor[v58 - 1];
                    AI_EvalFunc_COLStartMatrixSet_C(v7, &a1);
                }
                else
                {
                    AI_EvalFunc_COLStartMatrixSet_C(0, &a1);
                }
                v6 = (tGameObjectMain*)v58;
                if (v58)
                {
                    if (v58 <= 2)
                        v6 = AI_MainActor[v58 - 1];
                    AI_EvalFunc_OBJSetPos_C(v6, &a1);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &a1);
                }
                Src.msgID = 1;
                Src.gao2 = 0;
                Src.f2 = 0.0;
                Src.f1 = 0.0;
                AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Garde_Barriere_Mine_GBM_attaque__PJ_Aircraft_gao, &v55);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v40);
                MATH_SubstractVec(&v34, &v55, &v40);
                a1 = v34;
                v51.x = 0.0;
                v51.y = 0.0;
                v51.z = -100.0;
                AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v23);
                v35.x = v23.x * 0.5f;
                v35.y = v23.y * 0.5f;
                v35.z = v23.z * 0.5f;
                MATH_VecAddVec(&v65, &v51, &v35);
                Src.v1 = v65;
                Src.v2.x = 0.0;
                Src.v2.y = 0.0;
                Src.v2.z = 0.0;
                v5 = (tGameObjectMain*)v58;
                if (v58)
                {
                    if (v58 <= 2)
                        v5 = AI_MainActor[v58 - 1];
                    AI_EvalFunc_MSGSend_C(v5, &Src);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &Src);
                }
            }
        }
    }
    return 1000;
}

int Garde_Barriere_Mine_GBM_exec_GFX()
{
    int result; // eax
    float v1; // [esp+0h] [ebp-134h]
    tScriptStruct_PNJGardeBarriereMine* v2; // [esp+4h] [ebp-130h]
    tGameObjectMain* v3; // [esp+14h] [ebp-120h]
    tGameObjectMain* v4; // [esp+24h] [ebp-110h]
    tGameObjectMain* v5; // [esp+2Ch] [ebp-108h]
    tGameObjectMain* v6; // [esp+3Ch] [ebp-F8h]
    tGameObjectMain* v7; // [esp+4Ch] [ebp-E8h]
    tGameObjectMain* v8; // [esp+54h] [ebp-E0h]
    tGameObjectMain* v9; // [esp+64h] [ebp-D0h]
    tGameObjectMain* v10; // [esp+74h] [ebp-C0h]
    tGameObjectMain* v11; // [esp+7Ch] [ebp-B8h]
    tGameObjectMain* a1; // [esp+80h] [ebp-B4h]
    tGameObjectMain* v13; // [esp+84h] [ebp-B0h]
    Vec3 v14; // [esp+88h] [ebp-ACh] BYREF
    float v15; // [esp+94h] [ebp-A0h]
    float v16; // [esp+98h] [ebp-9Ch]
    Vec3 v17; // [esp+9Ch] [ebp-98h] BYREF
    Vec3 v18; // [esp+A8h] [ebp-8Ch] BYREF
    float v19; // [esp+B4h] [ebp-80h]
    float v20; // [esp+B8h] [ebp-7Ch]
    int v21; // [esp+BCh] [ebp-78h]
    Vec3 v22; // [esp+C0h] [ebp-74h] BYREF
    Vec3 v23; // [esp+CCh] [ebp-68h] BYREF
    float v24; // [esp+D8h] [ebp-5Ch]
    Vec3 a3; // [esp+DCh] [ebp-58h] BYREF
    Vec3 v26; // [esp+E8h] [ebp-4Ch] BYREF
    Vec3 v27; // [esp+100h] [ebp-34h] BYREF
    float v28; // [esp+10Ch] [ebp-28h]
    int v29; // [esp+110h] [ebp-24h]
    tScriptStruct_PNJGardeBarriereMine* v30; // [esp+114h] [ebp-20h]
    Vec3 v31; // [esp+118h] [ebp-1Ch] BYREF
    float v32; // [esp+124h] [ebp-10h]
    Vec3 v33; // [esp+128h] [ebp-Ch] BYREF

    v22.x = 1.0;
    v22.y = 0.0;
    v22.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJGardeBarriereMine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v30 = v2;
    v29 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Garde_Barriere_Mine_GBM_exec_GFX_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    v21 = (unsigned __int16)AI_gpst_CurrentGameObject->flags2;
    if ((v21 & 4) != 0)
    {
        if (v30->GFX_Reacteur_1 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v30->GFX_Reacteur_1);
            v30->GFX_Reacteur_1 = -1;
        }
        if (v30->GFX_Reacteur_2 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v30->GFX_Reacteur_2);
            v30->GFX_Reacteur_2 = -1;
        }
        if (v30->GFX_Reacteur_3 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v30->GFX_Reacteur_3);
            v30->GFX_Reacteur_3 = -1;
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        v28 = TIM_gf_dt;
        v13 = (tGameObjectMain*)v30->o_gyro;
        if (v13)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            a1 = v13;
        }
        else
        {
            a1 = 0;
        }
        v1 = 4.0f * v28;
        AI_EvalFunc_OBJRotateLocalZ_c(a1, v1);
        v29 = 0xFF8080;
        if (v30->GFX_Reacteur_1 == -1)
        {
            v30->GFX_Reacteur_1 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v30->GFX_Reacteur_1, 4, 1);
            v11 = (tGameObjectMain*)Garde_Barriere_Mine_GBM_exec_GFX_SFX_light_smoke_gao;
            if (Garde_Barriere_Mine_GBM_exec_GFX_SFX_light_smoke_gao)
            {
                if ((unsigned int)Garde_Barriere_Mine_GBM_exec_GFX_SFX_light_smoke_gao <= 2)
                    v11 = AI_MainActor[Garde_Barriere_Mine_GBM_exec_GFX_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v30->GFX_Reacteur_1, v11, 29);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v30->GFX_Reacteur_1, 0, 29);
            }
            GFXSetCurGlobal(v30->GFX_Reacteur_1);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21104, 5);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0099999998f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 0.0099999998f);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21100, 4);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 1.5);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v29);
        }
        v16 = AI_EvalFunc_MATHRandFloat_C(8.0, 10.0);
        GFXSetCurGlobal(v30->GFX_Reacteur_1);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v16);
        v15 = AI_EvalFunc_MATHRandFloat_C(0.1f, 0.30000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21006, v15);
        v10 = (tGameObjectMain*)v30->o_motor_1;
        if (v10)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            AI_EvalFunc_OBJGetPos_C(v10, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        v22 = a3;
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v22);
        v9 = (tGameObjectMain*)v30->o_motor_1;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            AI_EvalFunc_OBJ_BankingGet_C(v9, &v31);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGet_C(0, &v31);
        }
        v18.x = -v31.x;
        v18.y = -v31.y;
        v18.z = -v31.z;
        AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v18);
        if (v30->GFX_Reacteur_2 == -1)
        {
            v30->GFX_Reacteur_2 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v30->GFX_Reacteur_2, 4, 1);
            v8 = (tGameObjectMain*)Garde_Barriere_Mine_GBM_exec_GFX_SFX_light_smoke_gao;
            if (Garde_Barriere_Mine_GBM_exec_GFX_SFX_light_smoke_gao)
            {
                if ((unsigned int)Garde_Barriere_Mine_GBM_exec_GFX_SFX_light_smoke_gao <= 2)
                    v8 = AI_MainActor[Garde_Barriere_Mine_GBM_exec_GFX_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v30->GFX_Reacteur_2, v8, 29);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v30->GFX_Reacteur_2, 0, 29);
            }
            GFXSetCurGlobal(v30->GFX_Reacteur_2);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21104, 5);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0099999998f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 0.0099999998f);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21100, 4);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 1.5);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v29);
        }
        v32 = AI_EvalFunc_MATHRandFloat_C(8.0, 10.0);
        GFXSetCurGlobal(v30->GFX_Reacteur_2);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v32);
        v19 = AI_EvalFunc_MATHRandFloat_C(0.1f, 0.30000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21006, v19);
        v7 = (tGameObjectMain*)v30->o_motor_2;
        if (v7)
        {
            if ((unsigned int)v7 <= 2)
                v7 = AI_MainActor[(uint32_t)v7 - 1];
            AI_EvalFunc_OBJGetPos_C(v7, &v26);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v26);
        }
        v22 = v26;
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v22);
        v6 = (tGameObjectMain*)v30->o_motor_2;
        if (v6)
        {
            if ((unsigned int)v6 <= 2)
                v6 = AI_MainActor[(uint32_t)v6 - 1];
            AI_EvalFunc_OBJ_BankingGet_C(v6, &v17);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGet_C(0, &v17);
        }
        v33.x = -v17.x;
        v33.y = -v17.y;
        v33.z = -v17.z;
        AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v33);
        if (v30->GFX_Reacteur_3 == -1)
        {
            v30->GFX_Reacteur_3 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v30->GFX_Reacteur_3, 4, 1);
            v5 = (tGameObjectMain*)Garde_Barriere_Mine_GBM_exec_GFX_SFX_light_smoke_gao;
            if (Garde_Barriere_Mine_GBM_exec_GFX_SFX_light_smoke_gao)
            {
                if ((unsigned int)Garde_Barriere_Mine_GBM_exec_GFX_SFX_light_smoke_gao <= 2)
                    v5 = AI_MainActor[Garde_Barriere_Mine_GBM_exec_GFX_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v30->GFX_Reacteur_3, v5, 29);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v30->GFX_Reacteur_3, 0, 29);
            }
            GFXSetCurGlobal(v30->GFX_Reacteur_3);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21104, 5);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0099999998f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 0.0099999998f);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21100, 4);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 1.5);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v29);
        }
        v20 = AI_EvalFunc_MATHRandFloat_C(8.0, 10.0);
        GFXSetCurGlobal(v30->GFX_Reacteur_3);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v20);
        v24 = AI_EvalFunc_MATHRandFloat_C(0.1f, 0.30000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21006, v24);
        v4 = (tGameObjectMain*)v30->o_motor_3;
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            AI_EvalFunc_OBJGetPos_C(v4, &v14);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v14);
        }
        v22 = v14;
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v22);
        v3 = (tGameObjectMain*)v30->o_motor_3;
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            AI_EvalFunc_OBJ_BankingGet_C(v3, &v23);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGet_C(0, &v23);
        }
        v27.x = -v23.x;
        v27.y = -v23.y;
        v27.z = -v23.z;
        AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v27);
        result = 1000;
    }
    return result;
}

int Garde_Barriere_Mine_GBM_attend()
{
    float v1; // [esp+0h] [ebp-F8h]
    tScriptStruct_PNJGardeBarriereMine* v2; // [esp+8h] [ebp-F0h]
    tGameObjectMain* v3; // [esp+1Ch] [ebp-DCh]
    tGameObjectMain* v4; // [esp+20h] [ebp-D8h]
    tGameObjectMain* v5; // [esp+24h] [ebp-D4h]
    tGameObjectMain* v6; // [esp+40h] [ebp-B8h]
    Vec3 v7; // [esp+4Ch] [ebp-ACh] BYREF
    float v8; // [esp+5Ch] [ebp-9Ch]
    float v9; // [esp+60h] [ebp-98h]
    Vec3 v10; // [esp+64h] [ebp-94h] BYREF
    Vec3 v11; // [esp+70h] [ebp-88h] BYREF
    Vec3 a3; // [esp+7Ch] [ebp-7Ch] BYREF
    float v13; // [esp+88h] [ebp-70h]
    BOOL v14; // [esp+8Ch] [ebp-6Ch]
    Vec3 v15; // [esp+90h] [ebp-68h] BYREF
    float v16; // [esp+9Ch] [ebp-5Ch]
    Vec3 a4; // [esp+ACh] [ebp-4Ch] BYREF
    tGameObjectMain* v18; // [esp+B8h] [ebp-40h]
    Vec3 v19; // [esp+BCh] [ebp-3Ch] BYREF
    Vec3 dest; // [esp+C8h] [ebp-30h] BYREF
    Vec3 v21; // [esp+D4h] [ebp-24h] BYREF
    tScriptStruct_PNJGardeBarriereMine* v22; // [esp+E0h] [ebp-18h]
    Vec3 v23; // [esp+E4h] [ebp-14h] BYREF
    int v24; // [esp+F4h] [ebp-4h]

    v11.x = 1.0;
    v11.y = 0.0;
    v11.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJGardeBarriereMine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v22 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Garde_Barriere_Mine_GBM_attend_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Mine_GBM_attend_univVars_Planetes_CurHour,
            (void*)Garde_Barriere_Mine_GBM_attend_univVars,
            6116);
        Garde_Barriere_Mine_GBM_attend__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        fct_Garde_Barriere_Mine_GBM_attend_Garde_Barriere_Mine_GBM_attaque = (int)AI_ul_RealResolveOneRefFunction(0x3D000DA5u);
    }
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 256);
    v14 = AI_EvalFunc_TIMEElapsed_C(v22->f_time_mine, 4.0);
    if (v14
        || (AI_EvalFunc_OBJGetPos_C_CURRENT(&v21),
            MATH_SubstractVec(&dest, &v21, &v22->v_pos_togo),
            v8 = MATH_VecLen(&dest),
            v8 < 30.0))
    {
        v22->f_time_mine = TIM_gf_MainClock - TIM_gf_LockTime;
        v6 = (tGameObjectMain*)v22->des_o_wp_de_base;
        if (v6)
        {
            if ((unsigned int)v6 <= 2)
                v6 = AI_MainActor[(uint32_t)v6 - 1];
            AI_EvalFunc_BVRandomPosGet_C(v6, 0, &v22->v_pos_togo);
        }
        else
        {
            AI_EvalFunc_BVRandomPosGet_C(0, 0, &v22->v_pos_togo);
        }
        v22->v_pos_togo.z = AI_EvalFunc_MATHRandFloat_C(30.0, 40.0);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v10);
    MATH_SubstractVec(&v19, &v22->v_pos_togo, &v10);
    v11 = v19;
    AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v23);
    v13 = TIM_gf_dt;
    v1 = 4.0f * TIM_gf_dt;
    AI_EvalFunc_MATHVecBlend_C(&v23, &v11, v1, &a4);
    v11 = a4;
    v9 = MATH_VecLen(&v11);
    if (v9 > 40.0)
        AI_EvalFunc_MATHVecSetNormVector_C(&v11, 40.0);
    v24 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v11);
    v18 = *AI_MainActor;
    v5 = v18;
    if (v18)
    {
        if ((unsigned int)v18 <= 2)
            v5 = AI_MainActor[(uint32_t)v18 - 1];
        v4 = v5;
    }
    else
    {
        v4 = 0;
    }
    if (v4 == (tGameObjectMain*)Garde_Barriere_Mine_GBM_attend__PJ_Aircraft_gao)
    {
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Garde_Barriere_Mine_GBM_attend__PJ_Aircraft_gao, &a3);
        v3 = (tGameObjectMain*)v22->des_o_wp_de_base;
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            AI_EvalFunc_OBJGetPos_C(v3, &v15);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v15);
        }
        MATH_SubstractVec(&v7, &a3, &v15);
        v11 = v7;
        v16 = MATH_VecLen(&v11);
        if (v22->des_f_dist_att > (double)v16)
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Garde_Barriere_Mine_GBM_attend_Garde_Barriere_Mine_GBM_attaque);
    }
    return 1000;
}

int Garde_Barriere_Mine_GBM_depart()
{
    tScriptStruct_PNJGardeBarriereMine* v1; // [esp+0h] [ebp-7Ch]
    tGameObjectMain* v2; // [esp+Ch] [ebp-70h]
    Vec3 dest; // [esp+14h] [ebp-68h] BYREF
    Vec3 a3; // [esp+20h] [ebp-5Ch] BYREF
    Vec3 v5; // [esp+2Ch] [ebp-50h] BYREF
    int v6; // [esp+44h] [ebp-38h]
    int v7; // [esp+48h] [ebp-34h]
    Vec3 v8; // [esp+4Ch] [ebp-30h] BYREF
    Vec3 a1; // [esp+58h] [ebp-24h] BYREF
    tScriptStruct_PNJGardeBarriereMine* v10; // [esp+70h] [ebp-Ch]
    float v11; // [esp+74h] [ebp-8h]
    int v12; // [esp+78h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJGardeBarriereMine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v10 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Garde_Barriere_Mine_GBM_depart_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Garde_Barriere_Mine_GBM_depart_univVars_Planetes_CurHour,
            (void*)Garde_Barriere_Mine_GBM_depart_univVars,
            6116);
        fct_Garde_Barriere_Mine_GBM_depart_Garde_Barriere_Mine_GBM_attend = (int)AI_ul_RealResolveOneRefFunction(0x3D000DA3u);
    }
    if (v10->des_f_hour_min >= (double)*(float*)Garde_Barriere_Mine_GBM_depart_univVars_Planetes_CurHour
        && v10->des_f_hour_max <= (double)*(float*)Garde_Barriere_Mine_GBM_depart_univVars_Planetes_CurHour)
    {
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
    }
    else
    {
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
        if (v10->i_mode_arrdep)
        {
            v2 = (tGameObjectMain*)v10->des_o_wp_de_base;
            if (v2)
            {
                if ((unsigned int)v2 <= 2)
                    v2 = AI_MainActor[(uint32_t)v2 - 1];
                AI_EvalFunc_OBJGetPos_C(v2, &v10->v_pos_togo);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v10->v_pos_togo);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v8);
            v10->v_pos_togo.z = v8.z;
        }
        else
        {
            AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &v10->v_pos_togo);
            v10->v_pos_togo.z = 300.0;
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a3);
        MATH_SubstractVec(&dest, &v10->v_pos_togo, &a3);
        v7 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &dest);
        v12 = AI_EvalFunc_DYNFrictionSet_C(AI_gpst_CurrentGameObject, 100.0);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v5);
        MATH_SubstractVec(&a1, &v10->v_pos_togo, &v5);
        v11 = MATH_VecLen(&a1);
        if (v11 < 5.0 && ++v10->i_mode_arrdep == 2)
        {
            v6 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Garde_Barriere_Mine_GBM_depart_Garde_Barriere_Mine_GBM_attend);
            if (v6 != 1000)
                return v6;
        }
    }
    return 1000;
}

int Garde_Barriere_Mine_GBM_retour()
{
    int result; // eax
    float v1; // [esp+0h] [ebp-B0h]
    tScriptStruct_PNJGardeBarriereMine* v2; // [esp+8h] [ebp-A8h]
    Vec3 dest; // [esp+20h] [ebp-90h] BYREF
    Vec3 a3; // [esp+2Ch] [ebp-84h] BYREF
    Vec3 v5; // [esp+38h] [ebp-78h] BYREF
    float v6; // [esp+44h] [ebp-6Ch]
    Vec3 a4; // [esp+48h] [ebp-68h] BYREF
    Vec3 v8; // [esp+60h] [ebp-50h] BYREF
    Vec3 a2; // [esp+6Ch] [ebp-44h] BYREF
    Vec3 a1; // [esp+78h] [ebp-38h] BYREF
    int v11; // [esp+90h] [ebp-20h]
    tScriptStruct_PNJGardeBarriereMine* v12; // [esp+A0h] [ebp-10h]
    int v13; // [esp+A4h] [ebp-Ch]
    float v14; // [esp+A8h] [ebp-8h]
    int v15; // [esp+ACh] [ebp-4h]

    v5.x = 1.0;
    v5.y = 0.0;
    v5.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJGardeBarriereMine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v12 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Garde_Barriere_Mine_GBM_retour_Garde_Barriere_Mine_GBM_depart = (int)AI_ul_RealResolveOneRefFunction(0x3D000DB0u);
    }
    if (v12->i_mode_arrdep)
    {
        if (v12->i_mode_arrdep == 1)
        {
            AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &v12->v_pos_togo);
            v12->v_pos_togo.z = 300.0;
        }
        else
        {
            AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &v12->v_pos_togo);
        }
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v12->v_pos_togo);
        v12->v_pos_togo.z = 300.0;
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a3);
    MATH_SubstractVec(&dest, &v12->v_pos_togo, &a3);
    v5 = dest;
    AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&a2);
    v14 = TIM_gf_dt;
    v1 = 4.0f * TIM_gf_dt;
    AI_EvalFunc_MATHVecBlend_C(&a2, &v5, v1, &a4);
    v5 = a4;
    v11 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v5);
    v13 = AI_EvalFunc_DYNFrictionSet_C(AI_gpst_CurrentGameObject, 100.0);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v8);
    MATH_SubstractVec(&a1, &v12->v_pos_togo, &v8);
    v6 = MATH_VecLen(&a1);
    if (v6 >= 5.0
        || (++v12->i_mode_arrdep, v12->i_mode_arrdep != 3)
        || (v15 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Garde_Barriere_Mine_GBM_retour_Garde_Barriere_Mine_GBM_depart),
            v15 == 1000))
    {
        result = 1000;
    }
    else
    {
        result = v15;
    }
    return result;
}