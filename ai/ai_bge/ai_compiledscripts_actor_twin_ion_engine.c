#include "ai_compiledscripts_inc.h"

//Twin_Ion_Engine_TIE_exec_afterrec
int Twin_Ion_Engine_TIE_exec_afterrec_SFX_light_smoke_gao;

//Twin_Ion_Engine_TIE_mode_wait_for_target
int fct_Twin_Ion_Engine_TIE_mode_wait_for_target_Twin_Ion_Engine_TIE_mode_flying;
int fct_Twin_Ion_Engine_TIE_mode_wait_for_target_Twin_Ion_Engine_TIE_exec_afterrec;
int fct_Twin_Ion_Engine_TIE_mode_wait_for_target_Twin_Ion_Engine_TIE_reflex;
int Twin_Ion_Engine_TIE_mode_wait_for_target_univVars;
int Twin_Ion_Engine_TIE_mode_wait_for_target_univVars_Flag_speciaux_sauve;

//Twin_Ion_Engine_TIE_mode_flying
int Twin_Ion_Engine_TIE_mode_flying_SFX_light_smoke_gao;
int Twin_Ion_Engine_TIE_mode_flying_GST_Global_gao;
int Twin_Ion_Engine_TIE_mode_flying_GST_Global_gao_o_targets;
int Twin_Ion_Engine_TIE_mode_flying_GST_Global_gao_i_targets_nb;
int Twin_Ion_Engine_TIE_mode_flying_GST_Global_gao_v_targets_offset;
int Twin_Ion_Engine_TIE_mode_flying_univVars;
int Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_i_NbSpot;
int Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_ao_Object;
int Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_ai_Flags;
int fctvar_Twin_Ion_Engine_TIE_mode_flying_Twin_Ion_Engine;
int fct_Twin_Ion_Engine_TIE_mode_flying_Twin_Ion_Engine_TIE_exec_s_b;

//Twin_Ion_Engine_TIE_exec_s_b
int fct_Twin_Ion_Engine_TIE_exec_s_b_Twin_Ion_Engine_TIE_exec_constraint_Z;
int fct_Twin_Ion_Engine_TIE_exec_s_b_Twin_Ion_Engine_TIE_exec_constraint_collision;

//Twin_Ion_Engine_TIE_exec_gestion_paf
int fct_Twin_Ion_Engine_TIE_exec_gestion_paf_Twin_Ion_Engine_TIE_mode_meurt;
int Twin_Ion_Engine_TIE_exec_gestion_paf_SFX_explosion_gao;

//Twin_Ion_Engine_TIE_mode_meurt
int Twin_Ion_Engine_TIE_mode_meurt_SFX_explosion_gao;
int Twin_Ion_Engine_TIE_mode_meurt_SFX_fragment_carlingue_gao;

//Twin_Ion_Engine_TIE_init
int fct_Twin_Ion_Engine_TIE_init_Twin_Ion_Engine_TIE_mode_wait_for_target;

//Twin_Ion_Engine_TIE_reflex
int Twin_Ion_Engine_TIE_reflex_univVars;
int Twin_Ion_Engine_TIE_reflex_univVars_Flag_speciaux_sauve;
int Twin_Ion_Engine_TIE_reflex_GST_Global_gao;
int Twin_Ion_Engine_TIE_reflex_GST_Global_gao_o_targets;
int Twin_Ion_Engine_TIE_reflex_GST_Global_gao_i_targets_nb;
int Twin_Ion_Engine_TIE_reflex_GST_Global_gao_v_targets_offset;
int Twin_Ion_Engine_TIE_reflex_GST_Global_gao_o_selected_target;
int Twin_Ion_Engine_TIE_reflex_GST_Global_gao_i_selected_target_nb;
int Twin_Ion_Engine_TIE_reflex_GST_Global_gao_v_selected_target_offset;
int Twin_Ion_Engine_TIE_reflex_univVars_Carte_i_NbSpot;
int Twin_Ion_Engine_TIE_reflex_univVars_Carte_ao_Object;
int Twin_Ion_Engine_TIE_reflex_univVars_Carte_ai_Flags;
int fct_Twin_Ion_Engine_TIE_reflex_Twin_Ion_Engine_TIE_exec_afterrec;
int fct_Twin_Ion_Engine_TIE_reflex_Twin_Ion_Engine_TIE_exec_gestion_paf;
int fctvar_Twin_Ion_Engine_TIE_reflex_Twin_Ion_Engine;
int fctvar_Twin_Ion_Engine_TIE_reflex_CV_aircraft;
int fctvar_Twin_Ion_Engine_TIE_reflex_vaisseau_hyllis;

int Twin_Ion_Engine_TIE_exec_afterrec()
{
    double v0; // st7
    tGameObjectMain* v1; // eax
    int v2; // eax
    tGameObjectMain* v3; // eax
    float v5; // [esp+4h] [ebp-3A0h]
    float v6; // [esp+4h] [ebp-3A0h]
    float v7; // [esp+4h] [ebp-3A0h]
    float v8; // [esp+4h] [ebp-3A0h]
    tScriptStruct_PNJTwinIonEngine* v9; // [esp+8h] [ebp-39Ch]
    tGameObjectMain* v10; // [esp+10h] [ebp-394h]
    tGameObjectMain* v11; // [esp+18h] [ebp-38Ch]
    tGameObjectMain* v12; // [esp+30h] [ebp-374h]
    tGameObjectMain* v13; // [esp+38h] [ebp-36Ch]
    tGameObjectMain* v14; // [esp+40h] [ebp-364h]
    tGameObjectMain* v15; // [esp+48h] [ebp-35Ch]
    float v16; // [esp+4Ch] [ebp-358h]
    tGameObjectMain* v17; // [esp+68h] [ebp-33Ch]
    tGameObjectMain* v18; // [esp+70h] [ebp-334h]
    tGameObjectMain* v19; // [esp+78h] [ebp-32Ch]
    tGameObjectMain* v20; // [esp+80h] [ebp-324h]
    tGameObjectMain* v21; // [esp+88h] [ebp-31Ch]
    tGameObjectMain* v22; // [esp+90h] [ebp-314h]
    float v23; // [esp+94h] [ebp-310h]
    Vec3* v24; // [esp+ACh] [ebp-2F8h]
    Vec3* v25; // [esp+B0h] [ebp-2F4h]
    tGameObjectMain* v26; // [esp+B8h] [ebp-2ECh]
    Vec3* v27; // [esp+BCh] [ebp-2E8h]
    Vec3* v28; // [esp+C0h] [ebp-2E4h]
    tGameObjectMain* v29; // [esp+D8h] [ebp-2CCh]
    Vec3 v30; // [esp+E4h] [ebp-2C0h] BYREF
    Vec3 v31; // [esp+F0h] [ebp-2B4h] BYREF
    float v32; // [esp+FCh] [ebp-2A8h]
    float v33; // [esp+100h] [ebp-2A4h]
    float v34; // [esp+104h] [ebp-2A0h]
    float v35; // [esp+110h] [ebp-294h]
    Vec3 v36; // [esp+114h] [ebp-290h] BYREF
    float v37; // [esp+120h] [ebp-284h]
    float v38; // [esp+124h] [ebp-280h]
    float v39; // [esp+128h] [ebp-27Ch]
    Vec3 v40; // [esp+12Ch] [ebp-278h] BYREF
    Vec3 v41; // [esp+138h] [ebp-26Ch] BYREF
    Vec3 dest; // [esp+148h] [ebp-25Ch] BYREF
    int v43; // [esp+154h] [ebp-250h]
    float v44; // [esp+158h] [ebp-24Ch]
    Vec3 v45; // [esp+15Ch] [ebp-248h] BYREF
    Vec3 v46; // [esp+168h] [ebp-23Ch] BYREF
    float v47; // [esp+174h] [ebp-230h]
    Vec3 v48; // [esp+178h] [ebp-22Ch] BYREF
    Vec3 v49; // [esp+184h] [ebp-220h] BYREF
    Vec3 v50; // [esp+190h] [ebp-214h] BYREF
    Vec3 v51; // [esp+19Ch] [ebp-208h] BYREF
    Vec3 v52; // [esp+1A8h] [ebp-1FCh] BYREF
    Vec3 v53; // [esp+1B4h] [ebp-1F0h] BYREF
    Vec3 v54; // [esp+1C4h] [ebp-1E0h] BYREF
    float v55; // [esp+1D8h] [ebp-1CCh]
    float v56; // [esp+1DCh] [ebp-1C8h]
    float v57; // [esp+1E0h] [ebp-1C4h]
    float v58; // [esp+1E4h] [ebp-1C0h]
    float v59; // [esp+1E8h] [ebp-1BCh]
    float v60; // [esp+1ECh] [ebp-1B8h]
    float v61; // [esp+1F0h] [ebp-1B4h]
    float v62; // [esp+1F4h] [ebp-1B0h]
    float v63; // [esp+1F8h] [ebp-1ACh]
    float v64; // [esp+1FCh] [ebp-1A8h]
    Vec3 v65; // [esp+200h] [ebp-1A4h] BYREF
    float v66; // [esp+20Ch] [ebp-198h]
    float v67; // [esp+210h] [ebp-194h]
    float v68; // [esp+214h] [ebp-190h]
    Vec3 v69; // [esp+218h] [ebp-18Ch] BYREF
    Vec3 v71; // [esp+22Ch] [ebp-178h] BYREF
    Vec3 v72; // [esp+238h] [ebp-16Ch] BYREF
    Vec3 v73; // [esp+258h] [ebp-14Ch] BYREF
    Vec3 pos; // [esp+264h] [ebp-140h] BYREF
    Vec3 v75; // [esp+270h] [ebp-134h] BYREF
    int v76; // [esp+27Ch] [ebp-128h]
    Vec3 v77; // [esp+28Ch] [ebp-118h] BYREF
    Vec3 v78; // [esp+298h] [ebp-10Ch] BYREF
    float v79; // [esp+2B4h] [ebp-F0h]
    Vec3 a3; // [esp+2B8h] [ebp-ECh] BYREF
    Vec3 v81; // [esp+2C4h] [ebp-E0h] BYREF
    float v82; // [esp+2D0h] [ebp-D4h]
    float v83; // [esp+2D4h] [ebp-D0h]
    float v84; // [esp+2D8h] [ebp-CCh]
    Vec3 v85; // [esp+2DCh] [ebp-C8h] BYREF
    float v86; // [esp+2E8h] [ebp-BCh]
    Vec3 v87; // [esp+2F4h] [ebp-B0h] BYREF
    float v88; // [esp+300h] [ebp-A4h]
    float v89; // [esp+304h] [ebp-A0h]
    float v90; // [esp+308h] [ebp-9Ch]
    Vec3 v91; // [esp+30Ch] [ebp-98h] BYREF
    tScriptStruct_PNJTwinIonEngine* v92; // [esp+32Ch] [ebp-78h]
    int v93; // [esp+330h] [ebp-74h]
    int v94; // [esp+334h] [ebp-70h]
    int v95; // [esp+33Ch] [ebp-68h]
    float v96; // [esp+34Ch] [ebp-58h]
    float v97; // [esp+350h] [ebp-54h]
    float v98; // [esp+354h] [ebp-50h]
    float v99; // [esp+358h] [ebp-4Ch]
    float v100; // [esp+35Ch] [ebp-48h]
    float v101; // [esp+360h] [ebp-44h]
    Vec3 v104; // [esp+378h] [ebp-2Ch] BYREF
    Vec3 v105; // [esp+384h] [ebp-20h] BYREF
    float v106; // [esp+390h] [ebp-14h]
    float v107; // [esp+394h] [ebp-10h]
    float v108; // [esp+398h] [ebp-Ch]
    float v109; // [esp+39Ch] [ebp-8h]
    float v110; // [esp+3A0h] [ebp-4h]

    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    pos.x = 1.0;
    pos.y = 0.0;
    pos.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v9 = (tScriptStruct_PNJTwinIonEngine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v9 = 0;
    v92 = v9;
    v33 = 0.0;
    v93 = 0;
    v94 = 0;
    v44 = 0.0;
    v35 = 0.0;
    v79 = 0.0;
    v95 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Twin_Ion_Engine_TIE_exec_afterrec_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    if (!v92->i_current_mode)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v54);
        a3 = v54;
        v29 = (tGameObjectMain*)v92->o_target;
        if (v29)
        {
            if ((unsigned int)v29 <= 2)
                v29 = AI_MainActor[(uint32_t)v29 - 1];
            AI_EvalFunc_OBJGetPos_C(v29, &v92->v_target_pos);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v92->v_target_pos);
        }
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v77);
        MATH_SubstractVec(&dest, &v92->v_target_pos, &a3);
        AI_EvalFunc_MATHVecNormalize_C(&dest, &v41);
        v44 = v41.z * v77.z + v41.y * v77.y + v77.x * v41.x;
        if (v44 <= 0.98480803)
        {
            v92->f_time_target_locked = TIM_gf_MainClock - TIM_gf_LockTime;
        }
        else
        {
            if (AI_EvalFunc_TIMEElapsed_C(v92->f_time_shoot, v92->f_delay_shoot))
            {
                v95 = AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)v92->ao_laser, v92->des_i_laser_max_nb, 0);
                if (v95 != -1)
                {
                    v92->m_msg.msgID = 1;
                    v92->m_msg.gao2 = 0;
                    v92->m_msg.f2 = 400.0;
                    v92->m_msg.f1 = 50.0;
                    v92->m_msg.Int1 = -1;
                    v92->m_msg.Int2 = v92->i_laser_color;
                    v92->m_msg.v1.x = 0.0;
                    v92->m_msg.v1.y = 0.0;
                    v92->m_msg.v1.z = 0.0;
                    v92->m_msg.v2.x = 0.0;
                    v92->m_msg.v2.y = 0.0;
                    v92->m_msg.v2.z = 0.0;
                    v92->f_time_shoot = TIM_gf_MainClock - TIM_gf_LockTime;
                    v0 = AI_EvalFunc_MATHRandFloat_C(v92->des_f_laser_delay_min, v92->des_f_laser_delay_max);
                    v92->f_delay_shoot = (float)v0;
                    MATH_SubstractVec(&v46, &v92->v_target_pos, &a3);
                    v110 = MATH_VecLen(&v46);
                    v79 = v110 / 600.0f;
                    if (v79 <= 1.0)
                    {
                        if (v79 < 0.0)
                            v79 = 0.0;
                    }
                    else
                    {
                        v79 = 1.0;
                    }
                    v79 = AI_EvalFunc_MATHRandFloat_C(0.0, 2.0) + v79;
                    v27 = &v92->v_me_to_target;
                    v28 = &v92->v_target_pos;
                    v92->v_me_to_target.x = v92->v_target_pos.x;
                    v27->y = v28->y;
                    v27->z = v28->z;
                    v26 = (tGameObjectMain*)v92->o_target;
                    if (v26)
                    {
                        if ((unsigned int)v26 <= 2)
                            v26 = AI_MainActor[(uint32_t)v26 - 1];
                        AI_EvalFunc_DYNSpeedGetVector_C(v26, &v69);
                    }
                    else
                    {
                        AI_EvalFunc_DYNSpeedGetVector_C(0, &v69);
                    }
                    v37 = v69.x * v79;
                    v38 = v69.y * v79;
                    v39 = v69.z * v79;
                    v25 = &v92->v_me_to_target;
                    v92->v_me_to_target.x = v37 + v92->v_me_to_target.x;
                    v25->y = v38 + v25->y;
                    v25->z = v39 + v25->z;
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v65);
                    v24 = &v92->v_me_to_target;
                    v92->v_me_to_target.x = v92->v_me_to_target.x - v65.x;
                    v24->y = v24->y - v65.y;
                    v24->z = v24->z - v65.z;
                    MATH_NormalizeEqualVector(&v92->v_me_to_target);
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v81);
                    pos = v81;
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v75);
                    v106 = v75.x * 20.0f;
                    v107 = v75.y * 20.0f;
                    v108 = v75.z * 20.0f;
                    pos.x = pos.x + v106;
                    pos.y = pos.y + v107;
                    pos.z = pos.z + v108;
                    AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v78);
                    v58 = TIM_gf_dt;
                    v23 = 2.0f * TIM_gf_dt;
                    v55 = v78.x * v23;
                    v56 = v78.y * v23;
                    v57 = v78.z * v23;
                    pos.x = pos.x + v55;
                    pos.y = pos.y + v56;
                    pos.z = pos.z + v57;
                    v22 = (tGameObjectMain*)v92->des_o_laser;
                    if (v22)
                    {
                        if ((unsigned int)v22 <= 2)
                            v22 = AI_MainActor[(uint32_t)v22 - 1];
                        v1 = AI_EvalFunc_OBJDuplicate_C(v22, &pos);
                    }
                    else
                    {
                        v1 = AI_EvalFunc_OBJDuplicate_C(0, &pos);
                    }
                    v92->ao_laser[v95] = (int)v1;
                    v21 = (tGameObjectMain*)v92->ao_laser[v95];
                    if (v21)
                    {
                        if ((unsigned int)v21 <= 2)
                            v21 = AI_MainActor[(uint32_t)v21 - 1];
                        AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, v21, &v92->ao_laser[v95]);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, 0, &v92->ao_laser[v95]);
                    }
                    v20 = (tGameObjectMain*)v92->ao_laser[v95];
                    if (v20)
                    {
                        if ((unsigned int)v20 <= 2)
                            v20 = AI_MainActor[(uint32_t)v20 - 1];
                        AI_EvalFunc_OBJ_CapaSet_C(v20, 2, 0);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_CapaSet_C(0, 2, 0);
                    }
                    v19 = (tGameObjectMain*)v92->ao_laser[v95];
                    if (v19)
                    {
                        if ((unsigned int)v19 <= 2)
                            v19 = AI_MainActor[(uint32_t)v19 - 1];
                        AI_EvalFunc_OBJ_SightSet_C(v19, &v92->v_me_to_target);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_SightSet_C(0, &v92->v_me_to_target);
                    }
                    v18 = (tGameObjectMain*)v92->ao_laser[v95];
                    if (v18)
                    {
                        if ((unsigned int)v18 <= 2)
                            v18 = AI_MainActor[(uint32_t)v18 - 1];
                        AI_EvalFunc_MSGSend_C(v18, &v92->m_msg);
                    }
                    else
                    {
                        AI_EvalFunc_MSGSend_C(0, &v92->m_msg);
                    }
                    v17 = (tGameObjectMain*)v92->ao_laser[v95];
                    if (v17)
                    {
                        if ((unsigned int)v17 <= 2)
                            v17 = AI_MainActor[(uint32_t)v17 - 1];
                        v2 = AI_EvalFunc_OBJ_FlagsDesignSet_C(v17, 6, 0);
                    }
                    else
                    {
                        v2 = AI_EvalFunc_OBJ_FlagsDesignSet_C(0, 6, 0);
                    }
                    v43 = v2;
                }
            }
            if (v92->i_flag_near_death)
            {
                if (AI_EvalFunc_TIMEElapsed_C(v92->f_time_missile_shoot, 20.0))
                {
                    v95 = AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)v92->ao_missile, v92->des_i_missile_max_nb, 0);
                    if (v95 != -1)
                    {
                        v92->f_time_target_locked = TIM_gf_MainClock - TIM_gf_LockTime;
                        v92->f_time_shoot = v92->f_time_target_locked;
                        v92->f_time_missile_shoot = v92->f_time_target_locked;
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&v72);
                        pos = v72;
                        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v73);
                        v96 = v73.x * 20.0f;
                        v97 = v73.y * 20.0f;
                        v98 = v73.z * 20.0f;
                        pos.x = pos.x + v96;
                        pos.y = pos.y + v97;
                        pos.z = pos.z + v98;
                        AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v48);
                        v109 = TIM_gf_dt;
                        v16 = 2.0f * TIM_gf_dt;
                        v82 = v48.x * v16;
                        v83 = v48.y * v16;
                        v84 = v48.z * v16;
                        pos.x = pos.x + v82;
                        pos.y = pos.y + v83;
                        pos.z = pos.z + v84;
                        v15 = (tGameObjectMain*)v92->des_o_missile;
                        if (v15)
                        {
                            if ((unsigned int)v15 <= 2)
                                v15 = AI_MainActor[(uint32_t)v15 - 1];
                            v3 = AI_EvalFunc_OBJDuplicate_C(v15, &pos);
                        }
                        else
                        {
                            v3 = AI_EvalFunc_OBJDuplicate_C(0, &pos);
                        }
                        v92->ao_missile[v95] = (int)v3;
                        v14 = (tGameObjectMain*)v92->ao_missile[v95];
                        if (v14)
                        {
                            if ((unsigned int)v14 <= 2)
                                v14 = AI_MainActor[(uint32_t)v14 - 1];
                            AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, v14, &v92->ao_missile[v95]);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, 0, &v92->ao_missile[v95]);
                        }
                        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v52);
                        v13 = (tGameObjectMain*)v92->ao_missile[v95];
                        if (v13)
                        {
                            if ((unsigned int)v13 <= 2)
                                v13 = AI_MainActor[(uint32_t)v13 - 1];
                            AI_EvalFunc_OBJ_SightSet_C(v13, &v52);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_SightSet_C(0, &v52);
                        }
                        v92->m_msg.msgID = 1;
                        v92->m_msg.gao = AI_gpst_CurrentGameObject;
                        v92->m_msg.gao2 = (tGameObjectMain*)v92->o_target;
                        v92->m_msg.f2 = 6.0;
                        v92->m_msg.f1 = 100.0;
                        v92->m_msg.Int1 = 10;
                        v92->m_msg.Int2 = 350;
                        v92->m_msg.v1.x = 0.0;
                        v92->m_msg.v1.y = 0.0;
                        v92->m_msg.v1.z = 0.0;
                        v92->m_msg.v2.x = 0.0;
                        v92->m_msg.v2.y = 0.0;
                        v92->m_msg.v2.z = 0.0;
                        v12 = (tGameObjectMain*)v92->ao_missile[v95];
                        if (v12)
                        {
                            if ((unsigned int)v12 <= 2)
                                v12 = AI_MainActor[(uint32_t)v12 - 1];
                            AI_EvalFunc_MSGSend_C(v12, &v92->m_msg);
                        }
                        else
                        {
                            AI_EvalFunc_MSGSend_C(0, &v92->m_msg);
                        }
                    }
                }
            }
        }
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v85);
    pos = v85;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v50);
    v59 = v50.x * 5.0f;
    v60 = v50.y * 5.0f;
    v61 = v50.z * 5.0f;
    pos.x = pos.x - v59;
    pos.y = pos.y - v60;
    pos.z = pos.z - v61;
    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v104);
    v88 = v104.x * 3.0f;
    v89 = v104.y * 3.0f;
    v90 = v104.z * 3.0f;
    pos.x = pos.x + v88;
    pos.y = pos.y + v89;
    pos.z = pos.z + v90;
    AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v45);
    v87.x = v45.x * 4.0f;
    v87.y = v45.y * 4.0f;
    v87.z = v45.z * 4.0f;
    MATH_VecAddVec(&v105, &pos, &v87);
    AI_EvalFunc_GFXSetv_C_CURRENT(v92->GFX_Halo_G, 21200, &v105);
    AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v49);
    v71.x = v49.x * 4.0f;
    v71.y = v49.y * 4.0f;
    v71.z = v49.z * 4.0f;
    MATH_SubstractVec(&v51, &pos, &v71);
    AI_EvalFunc_GFXSetv_C_CURRENT(v92->GFX_Halo_D, 21200, &v51);
    if (v92->i_flag_near_death)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v92->f_time_sfx_gaz, v92->f_delay_sfx_gaz))
        {
            v92->f_time_sfx_gaz = TIM_gf_MainClock - TIM_gf_LockTime;
            v92->f_delay_sfx_gaz = AI_EvalFunc_MATHRandFloat_C(0.0099999998f, 0.050000001f);
            v33 = AI_EvalFunc_MATHRandFloat_C(1.0, 3.0);
            v35 = AI_EvalFunc_MATHRandFloat_C(5.0, 10.0);
            v47 = AI_EvalFunc_MATHRandFloat_C(0.0, 1.0);
            v76 = AI_EvalFunc_COLORBlend_C(-8355712, -1, v47);
            v93 = v76;
            v94 = -8355712;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v30);
            pos = v30;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v53);
            v32 = AI_EvalFunc_MATHRandFloat_C(6.0, 8.0);
            v99 = v53.x * v32;
            v100 = v53.y * v32;
            v101 = v53.z * v32;
            pos.x = pos.x - v99;
            pos.y = pos.y - v100;
            pos.z = pos.z - v101;
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v40);
            v34 = AI_EvalFunc_MATHRandFloat_C(-2.0, 2.0);
            v62 = v40.x * v34;
            v63 = v40.y * v34;
            v64 = v40.z * v34;
            pos.x = pos.x + v62;
            pos.y = pos.y + v63;
            pos.z = pos.z + v64;
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v31);
            v86 = AI_EvalFunc_MATHRandFloat_C(-2.0, 2.0);
            v66 = v31.x * v86;
            v67 = v31.y * v86;
            v68 = v31.z * v86;
            pos.x = pos.x + v66;
            pos.y = pos.y + v67;
            pos.z = pos.z + v68;
            if (v92->GFX_smoke == -1)
            {
                v92->GFX_smoke = AI_EvalFunc_GFXAdd_C_CURRENT(6);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v92->GFX_smoke, 0, 1);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v92->GFX_smoke, 2, 1);
                v11 = (tGameObjectMain*)Twin_Ion_Engine_TIE_exec_afterrec_SFX_light_smoke_gao;
                if (Twin_Ion_Engine_TIE_exec_afterrec_SFX_light_smoke_gao)
                {
                    if ((unsigned int)Twin_Ion_Engine_TIE_exec_afterrec_SFX_light_smoke_gao <= 2)
                        v11 = AI_MainActor[Twin_Ion_Engine_TIE_exec_afterrec_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v92->GFX_smoke, v11, 8);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v92->GFX_smoke, 0, 8);
                }
                GFXSetCurGlobal(v92->GFX_smoke);
                AI_EvalFunc_GFXSeti_C_CURGLOB(6100, 100);
                AI_EvalFunc_GFXSetf_C_CURGLOB(6001, 0.97000003f);
                v5 = v35 * 0.5f;
                AI_EvalFunc_GFXSetf_C_CURGLOB(6002, v5);
                v6 = v35 + v35;
                AI_EvalFunc_GFXSetf_C_CURGLOB(6003, v6);
                v7 = v33 / 2.0f;
                AI_EvalFunc_GFXSetf_C_CURGLOB(6000, v7);
                v92->GFX_smoke2 = AI_EvalFunc_GFXAdd_C_CURRENT(6);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v92->GFX_smoke2, 0, 1);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v92->GFX_smoke2, 2, 1);
                v10 = (tGameObjectMain*)Twin_Ion_Engine_TIE_exec_afterrec_SFX_light_smoke_gao;
                if (Twin_Ion_Engine_TIE_exec_afterrec_SFX_light_smoke_gao)
                {
                    if ((unsigned int)Twin_Ion_Engine_TIE_exec_afterrec_SFX_light_smoke_gao <= 2)
                        v10 = AI_MainActor[Twin_Ion_Engine_TIE_exec_afterrec_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v92->GFX_smoke2, v10, 8);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v92->GFX_smoke2, 0, 8);
                }
                GFXSetCurGlobal(v92->GFX_smoke2);
                AI_EvalFunc_GFXSeti_C_CURGLOB(6100, 100);
                AI_EvalFunc_GFXSetf_C_CURGLOB(6001, 0.97000003f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(6002, v35);
                v8 = v35 * 4.0f;
                AI_EvalFunc_GFXSetf_C_CURGLOB(6003, v8);
                AI_EvalFunc_GFXSetf_C_CURGLOB(6000, v33);
            }
            GFXSetCurGlobal(v92->GFX_smoke);
            AI_EvalFunc_GFXSetv_C_CURGLOB(6200, &pos);
            AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v91);
            AI_EvalFunc_GFXSetv_C_CURGLOB(6201, &v91);
            AI_EvalFunc_GFXSeti_C_CURGLOB(6103, 0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(6101, v93);
            AI_EvalFunc_GFXSeti_C_CURGLOB(6102, 0);
            AI_EvalFunc_GFXSetv_C_CURRENT(v92->GFX_smoke2, 6200, &pos);
            AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v36);
            GFXSetCurGlobal(v92->GFX_smoke2);
            AI_EvalFunc_GFXSetv_C_CURGLOB(6201, &v36);
            AI_EvalFunc_GFXSeti_C_CURGLOB(6103, 0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(6101, v94);
            AI_EvalFunc_GFXSeti_C_CURGLOB(6102, 0);
        }
    }
    return 1000;
}

int Twin_Ion_Engine_TIE_mode_wait_for_target()
{
    int result; // eax
    tScriptStruct_PNJTwinIonEngine* v1; // [esp+8h] [ebp-1D4h]
    tGameObjectMain* v2; // [esp+10h] [ebp-1CCh]
    tGameObjectMain* v3; // [esp+1Ch] [ebp-1C0h]
    tGameObjectMain* v4; // [esp+24h] [ebp-1B8h]
    tGameObjectMain* v5; // [esp+2Ch] [ebp-1B0h]
    tGameObjectMain* v6; // [esp+34h] [ebp-1A8h]
    tGameObjectMain* v7; // [esp+44h] [ebp-198h]
    tGameObjectMain* v8; // [esp+54h] [ebp-188h]
    tGameObjectMain* v9; // [esp+5Ch] [ebp-180h]
    tGameObjectMain* v10; // [esp+64h] [ebp-178h]
    tGameObjectMain* v11; // [esp+6Ch] [ebp-170h]
    tGameObjectMain* v12; // [esp+7Ch] [ebp-160h]
    tGameObjectMain* v13; // [esp+80h] [ebp-15Ch]
    tGameObjectMain* v14; // [esp+84h] [ebp-158h]
    tGameObjectMain* v15; // [esp+88h] [ebp-154h]
    tGameObjectMain* v16; // [esp+8Ch] [ebp-150h]
    tGameObjectMain* v17; // [esp+94h] [ebp-148h]
    Vec3 v18; // [esp+98h] [ebp-144h] BYREF
    Vec3 v19; // [esp+A4h] [ebp-138h] BYREF
    void* v20; // [esp+B0h] [ebp-12Ch]
    Vec3 a3; // [esp+B4h] [ebp-128h] BYREF
    Vec3 dest; // [esp+C0h] [ebp-11Ch] BYREF
    float v23; // [esp+CCh] [ebp-110h]
    float v24; // [esp+D0h] [ebp-10Ch]
    float v25; // [esp+D4h] [ebp-108h]
    tGameObjectMain* v26; // [esp+D8h] [ebp-104h]
    Vec3 v27; // [esp+DCh] [ebp-100h] BYREF
    Vec3 v28; // [esp+ECh] [ebp-F0h] BYREF
    Vec3 v29; // [esp+F8h] [ebp-E4h] BYREF
    Vec3 v30; // [esp+108h] [ebp-D4h] BYREF
    Vec3 v31; // [esp+118h] [ebp-C4h] BYREF
    float v32; // [esp+124h] [ebp-B8h]
    float v33; // [esp+128h] [ebp-B4h]
    tGameObjectMain* v34; // [esp+130h] [ebp-ACh]
    Vec3 v35; // [esp+134h] [ebp-A8h] BYREF
    Vec3 v36; // [esp+140h] [ebp-9Ch] BYREF
    float v37; // [esp+150h] [ebp-8Ch]
    float v38; // [esp+154h] [ebp-88h]
    float v39; // [esp+158h] [ebp-84h]
    float v40; // [esp+15Ch] [ebp-80h]
    float v41; // [esp+164h] [ebp-78h]
    Vec3 a1; // [esp+168h] [ebp-74h] BYREF
    Vec3 v43; // [esp+174h] [ebp-68h] BYREF
    float v44; // [esp+180h] [ebp-5Ch]
    float v45; // [esp+184h] [ebp-58h]
    float v46; // [esp+188h] [ebp-54h]
    void* Src; // [esp+18Ch] [ebp-50h]
    tScriptStruct_PNJTwinIonEngine* v48; // [esp+194h] [ebp-48h]
    int v49; // [esp+198h] [ebp-44h]
    BOOL v50; // [esp+19Ch] [ebp-40h]
    float v51; // [esp+1A0h] [ebp-3Ch]
    float v52; // [esp+1A4h] [ebp-38h]
    float v53; // [esp+1A8h] [ebp-34h]
    Vec3 v54; // [esp+1ACh] [ebp-30h] BYREF
    float v55; // [esp+1B8h] [ebp-24h]
    float v56; // [esp+1BCh] [ebp-20h]
    float v57; // [esp+1C0h] [ebp-1Ch]
    Vec3 v59; // [esp+1C8h] [ebp-14h] BYREF
    float v60; // [esp+1D4h] [ebp-8h]

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJTwinIonEngine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v48 = v1;
    v26 = 0;
    v50 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Twin_Ion_Engine_TIE_mode_wait_for_target_Twin_Ion_Engine_TIE_mode_flying = (int)AI_ul_RealResolveOneRefFunction(0x7200205Fu);
        fct_Twin_Ion_Engine_TIE_mode_wait_for_target_Twin_Ion_Engine_TIE_exec_afterrec = (int)AI_ul_RealResolveOneRefFunction(0x72002069u);
        fct_Twin_Ion_Engine_TIE_mode_wait_for_target_Twin_Ion_Engine_TIE_reflex = (int)AI_ul_RealResolveOneRefFunction(0x72002060u);
        Twin_Ion_Engine_TIE_mode_wait_for_target_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Twin_Ion_Engine_TIE_mode_wait_for_target_univVars_Flag_speciaux_sauve,
            (void*)Twin_Ion_Engine_TIE_mode_wait_for_target_univVars,
            4488);
    }
    if (v48->i_current_mode != 2)
    {
        v48->i_current_mode = 2;
        v48->f_time_start_mode = TIM_gf_MainClock - TIM_gf_LockTime;
        v48->o_target = 0;
        v48->m_msg.msgID = 16;
        v48->m_msg.gao = AI_gpst_CurrentGameObject;
        v17 = (tGameObjectMain*)v48->des_o_dogfight_manager;
        if (v17)
        {
            if ((unsigned int)v17 <= 2)
                v17 = AI_MainActor[(uint32_t)v17 - 1];
            AI_EvalFunc_MSGSend_C(v17, &v48->m_msg);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &v48->m_msg);
        }
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
    }
    v16 = (tGameObjectMain*)v48->o_target;
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
    if (v15)
    {
        v26 = AI_EvalFunc_VIEWGetObject_C(0);
        v14 = (tGameObjectMain*)v48->des_tourbillon;
        if (v14)
        {
            if ((unsigned int)v14 <= 2)
                v14 = AI_MainActor[(uint32_t)v14 - 1];
            v13 = v14;
        }
        else
        {
            v13 = 0;
        }
        if (v13 && v48->i_flag_je_suis_un_ennemi)
        {
            v12 = (tGameObjectMain*)v48->des_tourbillon;
            if (v12)
            {
                if ((unsigned int)v12 <= 2)
                    v12 = AI_MainActor[(uint32_t)v12 - 1];
                AI_EvalFunc_OBJGetPos_C(v12, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            a1 = a3;
            v11 = (tGameObjectMain*)v48->des_tourbillon;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v11 = AI_MainActor[(uint32_t)v11 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v11, &v19);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v19);
            }
            v33 = AI_EvalFunc_MATHRandFloat_C(0.0, 100.0);
            v51 = v19.x * v33;
            v52 = v19.y * v33;
            v53 = v19.z * v33;
            a1.x = a1.x - v51;
            a1.y = a1.y - v52;
            a1.z = a1.z - v53;
            v10 = (tGameObjectMain*)v48->des_tourbillon;
            if (v10)
            {
                if ((unsigned int)v10 <= 2)
                    v10 = AI_MainActor[(uint32_t)v10 - 1];
                AI_EvalFunc_OBJ_HorizonGet_C(v10, &v27);
            }
            else
            {
                AI_EvalFunc_OBJ_HorizonGet_C(0, &v27);
            }
            v41 = AI_EvalFunc_MATHRandFloat_C(0.0, 100.0);
            v55 = v27.x * v41;
            v56 = v27.y * v41;
            v57 = v27.z * v41;
            a1.x = a1.x - v55;
            a1.y = a1.y - v56;
            a1.z = a1.z - v57;
            v9 = (tGameObjectMain*)v48->des_tourbillon;
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v9, &v28);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v28);
            }
            v37 = AI_EvalFunc_MATHRandFloat_C(150.0, 300.0);
            v23 = v28.x * v37;
            v24 = v28.y * v37;
            v25 = v28.z * v37;
            a1.x = a1.x + v23;
            a1.y = a1.y + v24;
            a1.z = a1.z + v25;
            AI_EvalFunc_OBJSetPos_C_CURRENT(&a1);
            v8 = (tGameObjectMain*)v48->des_tourbillon;
            if (v8)
            {
                if ((unsigned int)v8 <= 2)
                    v8 = AI_MainActor[(uint32_t)v8 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v8, &v59);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v59);
            }
            v36.x = -v59.x;
            v36.y = -v59.y;
            v36.z = -v59.z;
            v29.x = 0.0;
            v29.y = 0.0;
            v29.z = 1.0;
            AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v36, &v29);
        }
        else
        {
            v7 = v26;
            if (v26)
            {
                if ((unsigned int)v26 <= 2)
                    v7 = AI_MainActor[(uint32_t)v26 - 1];
                AI_EvalFunc_OBJGetPos_C(v7, &v31);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v31);
            }
            a1 = v31;
            v6 = v26;
            if (v26)
            {
                if ((unsigned int)v26 <= 2)
                    v6 = AI_MainActor[(uint32_t)v26 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v6, &v18);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v18);
            }
            v32 = AI_EvalFunc_MATHRandFloat_C(100.0, 200.0);
            v44 = v18.x * v32;
            v45 = v18.y * v32;
            v46 = v18.z * v32;
            a1.x = a1.x - v44;
            a1.y = a1.y - v45;
            a1.z = a1.z - v46;
            v5 = v26;
            if (v26)
            {
                if ((unsigned int)v26 <= 2)
                    v5 = AI_MainActor[(uint32_t)v26 - 1];
                AI_EvalFunc_OBJ_HorizonGet_C(v5, &v35);
            }
            else
            {
                AI_EvalFunc_OBJ_HorizonGet_C(0, &v35);
            }
            v60 = AI_EvalFunc_MATHRandFloat_C(-200.0, 200.0);
            v38 = v35.x * v60;
            v39 = v35.y * v60;
            v40 = v35.z * v60;
            a1.x = a1.x + v38;
            a1.y = a1.y + v39;
            a1.z = a1.z + v40;
            a1.z = AI_EvalFunc_MATHRandFloat_C(v48->des_f_Z_min, v48->des_f_Z_max);
            AI_EvalFunc_OBJSetPos_C_CURRENT(&a1);
            v4 = v26;
            if (v26)
            {
                if ((unsigned int)v26 <= 2)
                    v4 = AI_MainActor[(uint32_t)v26 - 1];
                AI_EvalFunc_OBJGetPos_C(v4, &v30);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v30);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v43);
            MATH_SubstractVec(&dest, &v30, &v43);
            v54.x = 0.0;
            v54.y = 0.0;
            v54.z = 1.0;
            AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&dest, &v54);
        }
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
        AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0xFFu, 0);
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, -1, 0);
        AI_EvalFunc_AITrackChange_C(
            1,
            (tAIFunction*)fct_Twin_Ion_Engine_TIE_mode_wait_for_target_Twin_Ion_Engine_TIE_mode_flying);
        v34 = AI_gpst_CurrentGameObject;
        v3 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v3 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v3,
                2,
                (tAIFunction*)fct_Twin_Ion_Engine_TIE_mode_wait_for_target_Twin_Ion_Engine_TIE_exec_afterrec);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_Twin_Ion_Engine_TIE_mode_wait_for_target_Twin_Ion_Engine_TIE_exec_afterrec);
        }
        v49 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Twin_Ion_Engine_TIE_mode_wait_for_target_Twin_Ion_Engine_TIE_reflex);
        if (v49 == 1000)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 4;
        }
        else
        {
            result = v49;
        }
    }
    else
    {
        v50 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Twin_Ion_Engine_TIE_mode_wait_for_target_univVars_Flag_speciaux_sauve + 4),
            2789);
        if (!v50)
            v48->f_time_start_mode = TIM_gf_MainClock - TIM_gf_LockTime;
        if (AI_EvalFunc_TIMEElapsed_C(v48->f_time_start_mode, 0.2f))
        {
            v48->f_time_start_mode = TIM_gf_MainClock - TIM_gf_LockTime;
            v48->m_msg.msgID = 16;
            v48->m_msg.gao = AI_gpst_CurrentGameObject;
            v2 = (tGameObjectMain*)v48->des_o_dogfight_manager;
            if (v2)
            {
                if ((unsigned int)v2 <= 2)
                    v2 = AI_MainActor[(uint32_t)v2 - 1];
                AI_EvalFunc_MSGSend_C(v2, &v48->m_msg);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &v48->m_msg);
            }
        }
        Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v48->m_msg, Src, sizeof(v48->m_msg));
        while (v48->m_msg.msgID)
        {
            if (v48->m_msg.msgID == 16)
            {
                v48->o_target = (int)v48->m_msg.gao2;
                v48->i_laser_color = v48->m_msg.Int1;
                v48->i_flag_je_suis_un_ennemi = v48->m_msg.Int2;
            }
            v20 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
            memcpy(&v48->m_msg, v20, sizeof(v48->m_msg));
        }
        result = 1000;
    }
    return result;
}

int Twin_Ion_Engine_TIE_mode_flying()
{
    BOOL v0; // eax
    BOOL v1; // eax
    double v2; // st7
    float v4; // [esp+8h] [ebp-380h]
    int v5; // [esp+Ch] [ebp-37Ch]
    float v6; // [esp+10h] [ebp-378h]
    float v7; // [esp+14h] [ebp-374h]
    float v8; // [esp+18h] [ebp-370h]
    float v9; // [esp+1Ch] [ebp-36Ch]
    float v10; // [esp+20h] [ebp-368h]
    tScriptStruct_PNJTwinIonEngine* v11; // [esp+24h] [ebp-364h]
    float v12; // [esp+28h] [ebp-360h]
    float v13; // [esp+2Ch] [ebp-35Ch]
    float v14; // [esp+30h] [ebp-358h]
    float v15; // [esp+34h] [ebp-354h]
    float v16; // [esp+38h] [ebp-350h]
    float v17; // [esp+3Ch] [ebp-34Ch]
    float v18; // [esp+58h] [ebp-330h]
    float v19; // [esp+64h] [ebp-324h]
    tGameObjectMain* v20; // [esp+6Ch] [ebp-31Ch]
    float v21; // [esp+70h] [ebp-318h]
    float v22; // [esp+74h] [ebp-314h]
    float v23; // [esp+78h] [ebp-310h]
    float v24; // [esp+7Ch] [ebp-30Ch]
    float v25; // [esp+80h] [ebp-308h]
    float v26; // [esp+84h] [ebp-304h]
    float v27; // [esp+A0h] [ebp-2E8h]
    float v28; // [esp+A4h] [ebp-2E4h]
    tGameObjectMain* v29; // [esp+ACh] [ebp-2DCh]
    tGameObjectMain* v30; // [esp+B4h] [ebp-2D4h]
    tGameObjectMain* v31; // [esp+BCh] [ebp-2CCh]
    float v32; // [esp+C0h] [ebp-2C8h]
    tGameObjectMain* v33; // [esp+CCh] [ebp-2BCh]
    float v34; // [esp+D0h] [ebp-2B8h]
    tGameObjectMain* v35; // [esp+DCh] [ebp-2ACh]
    tGameObjectMain* v36; // [esp+F0h] [ebp-298h]
    tGameObjectMain* v37; // [esp+F4h] [ebp-294h]
    tGameObjectMain* v38; // [esp+FCh] [ebp-28Ch]
    tGameObjectMain* v39; // [esp+104h] [ebp-284h]
    Vec3 v40; // [esp+114h] [ebp-274h] BYREF
    BOOL v41; // [esp+120h] [ebp-268h]
    float v42; // [esp+128h] [ebp-260h]
    float v43; // [esp+12Ch] [ebp-25Ch]
    float v44; // [esp+130h] [ebp-258h]
    float v45; // [esp+134h] [ebp-254h]
    float v46; // [esp+138h] [ebp-250h]
    float v47; // [esp+13Ch] [ebp-24Ch]
    float v48; // [esp+140h] [ebp-248h]
    Vec3 v49; // [esp+144h] [ebp-244h] BYREF
    Vec3 v50; // [esp+150h] [ebp-238h] BYREF
    float v51; // [esp+15Ch] [ebp-22Ch]
    float v52; // [esp+160h] [ebp-228h]
    float v54; // [esp+170h] [ebp-218h]
    float v55; // [esp+178h] [ebp-210h]
    int v56; // [esp+180h] [ebp-208h]
    Vec3 v57; // [esp+184h] [ebp-204h] BYREF
    float v58; // [esp+190h] [ebp-1F8h]
    float v59; // [esp+194h] [ebp-1F4h]
    float v60; // [esp+19Ch] [ebp-1ECh]
    float v61; // [esp+1A0h] [ebp-1E8h]
    float v62; // [esp+1A8h] [ebp-1E0h]
    float v63; // [esp+1ACh] [ebp-1DCh]
    float v64; // [esp+1B8h] [ebp-1D0h]
    tGameObjectMain* v65; // [esp+1BCh] [ebp-1CCh]
    float v66; // [esp+1C0h] [ebp-1C8h]
    float v67; // [esp+1C4h] [ebp-1C4h]
    BOOL v68; // [esp+1CCh] [ebp-1BCh]
    float v69; // [esp+1D4h] [ebp-1B4h]
    float v70; // [esp+1D8h] [ebp-1B0h]
    tGameObjectMain* v71; // [esp+1E0h] [ebp-1A8h]
    Vec3 v73; // [esp+1E8h] [ebp-1A0h] BYREF
    Vec3 v74; // [esp+1F4h] [ebp-194h] BYREF
    Vec3 v75; // [esp+200h] [ebp-188h] BYREF
    Vec3 a3; // [esp+20Ch] [ebp-17Ch] BYREF
    float v77; // [esp+218h] [ebp-170h]
    float v78; // [esp+21Ch] [ebp-16Ch]
    int v79; // [esp+220h] [ebp-168h]
    float v80; // [esp+224h] [ebp-164h]
    float v82; // [esp+22Ch] [ebp-15Ch]
    Vec3 v83; // [esp+230h] [ebp-158h] BYREF
    float v84; // [esp+23Ch] [ebp-14Ch]
    float v85; // [esp+244h] [ebp-144h]
    float v86; // [esp+248h] [ebp-140h]
    int v87; // [esp+250h] [ebp-138h]
    Vec3 v88; // [esp+254h] [ebp-134h] BYREF
    float v90; // [esp+270h] [ebp-118h]
    int v91; // [esp+280h] [ebp-108h]
    BOOL v92; // [esp+284h] [ebp-104h]
    Vec3 v93; // [esp+288h] [ebp-100h] BYREF
    Vec3 v94; // [esp+294h] [ebp-F4h] BYREF
    int v95; // [esp+2A0h] [ebp-E8h]
    float v96; // [esp+2A4h] [ebp-E4h]
    tGameObjectMain* v97; // [esp+2A8h] [ebp-E0h]
    int v98; // [esp+2ACh] [ebp-DCh]
    Vec3 v99; // [esp+2B4h] [ebp-D4h] BYREF
    Vec3 v100; // [esp+2C0h] [ebp-C8h] BYREF
    float v101; // [esp+2CCh] [ebp-BCh]
    BOOL v102; // [esp+2D0h] [ebp-B8h]
    float v103; // [esp+2D4h] [ebp-B4h]
    float v104; // [esp+2D8h] [ebp-B0h]
    float v105; // [esp+2DCh] [ebp-ACh]
    BOOL v106; // [esp+2E4h] [ebp-A4h]
    Vec3 v107; // [esp+2E8h] [ebp-A0h] BYREF
    int v108; // [esp+2F4h] [ebp-94h]
    int v109; // [esp+2F8h] [ebp-90h]
    int v110; // [esp+2FCh] [ebp-8Ch]
    float v111; // [esp+300h] [ebp-88h]
    float v112; // [esp+304h] [ebp-84h]
    float v113; // [esp+308h] [ebp-80h]
    int v114; // [esp+30Ch] [ebp-7Ch]
    tScriptStruct_PNJTwinIonEngine* v115; // [esp+314h] [ebp-74h]
    BOOL v116; // [esp+318h] [ebp-70h]
    Vec3 v117; // [esp+31Ch] [ebp-6Ch] BYREF
    float v118; // [esp+328h] [ebp-60h]
    float v121; // [esp+33Ch] [ebp-4Ch]
    int v122; // [esp+340h] [ebp-48h]
    Vec3 v123; // [esp+344h] [ebp-44h] BYREF
    int v124; // [esp+350h] [ebp-38h]
    int v125; // [esp+354h] [ebp-34h]
    int v126; // [esp+358h] [ebp-30h]
    Vec3 v127; // [esp+35Ch] [ebp-2Ch] BYREF
    Vec3 dest; // [esp+368h] [ebp-20h] BYREF
    int v129; // [esp+374h] [ebp-14h]
    float v130; // [esp+378h] [ebp-10h]
    float v131; // [esp+37Ch] [ebp-Ch]
    float v132; // [esp+380h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v11 = (tScriptStruct_PNJTwinIonEngine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v11 = 0;
    v115 = v11;
    v55 = 0.0;
    v97 = 0;
    v71 = 0;
    v91 = 0;
    v48 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Twin_Ion_Engine_TIE_mode_flying_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        Twin_Ion_Engine_TIE_mode_flying_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Twin_Ion_Engine_TIE_mode_flying_GST_Global_gao_o_targets,
            (void*)Twin_Ion_Engine_TIE_mode_flying_GST_Global_gao,
            964);
        AI2C_ResolveBuffer(
            &Twin_Ion_Engine_TIE_mode_flying_GST_Global_gao_i_targets_nb,
            (void*)Twin_Ion_Engine_TIE_mode_flying_GST_Global_gao,
            2572);
        AI2C_ResolveBuffer(
            &Twin_Ion_Engine_TIE_mode_flying_GST_Global_gao_v_targets_offset,
            (void*)Twin_Ion_Engine_TIE_mode_flying_GST_Global_gao,
            1368);
        Twin_Ion_Engine_TIE_mode_flying_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_i_NbSpot,
            (void*)Twin_Ion_Engine_TIE_mode_flying_univVars,
            4472);
        AI2C_ResolveBuffer(
            &Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_ao_Object,
            (void*)Twin_Ion_Engine_TIE_mode_flying_univVars,
            4304);
        AI2C_ResolveBuffer(
            &Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_ai_Flags,
            (void*)Twin_Ion_Engine_TIE_mode_flying_univVars,
            4388);
        fctvar_Twin_Ion_Engine_TIE_mode_flying_Twin_Ion_Engine = AI_ul_RealResolveOneRef(42, 0x72002063u);
        fct_Twin_Ion_Engine_TIE_mode_flying_Twin_Ion_Engine_TIE_exec_s_b = (int)AI_ul_RealResolveOneRefFunction(0x7200205Eu);
    }
    if (v115->i_current_mode)
    {
        v115->i_current_mode = 0;
        v68 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
        v114 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x1228004, 0);
        v98 = AI_EvalFunc_DYNFrictionSet_C(AI_gpst_CurrentGameObject, v115->f_friction);
        v56 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
        v115->GFX_Halo_G = v56;
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v115->GFX_Halo_G, 4, 1);
        v39 = (tGameObjectMain*)Twin_Ion_Engine_TIE_mode_flying_SFX_light_smoke_gao;
        if (Twin_Ion_Engine_TIE_mode_flying_SFX_light_smoke_gao)
        {
            if ((unsigned int)Twin_Ion_Engine_TIE_mode_flying_SFX_light_smoke_gao <= 2)
                v39 = AI_MainActor[Twin_Ion_Engine_TIE_mode_flying_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v115->GFX_Halo_G, v39, 29);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v115->GFX_Halo_G, 0, 29);
        }
        GFXSetCurGlobal(v115->GFX_Halo_G);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v115->i_laser_color);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 4.0);
        v115->GFX_Halo_D = AI_EvalFunc_GFXAdd_C_CURRENT(21);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v115->GFX_Halo_D, 4, 1);
        v38 = (tGameObjectMain*)Twin_Ion_Engine_TIE_mode_flying_SFX_light_smoke_gao;
        if (Twin_Ion_Engine_TIE_mode_flying_SFX_light_smoke_gao)
        {
            if ((unsigned int)Twin_Ion_Engine_TIE_mode_flying_SFX_light_smoke_gao <= 2)
                v38 = AI_MainActor[Twin_Ion_Engine_TIE_mode_flying_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v115->GFX_Halo_D, v38, 29);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v115->GFX_Halo_D, 0, 29);
        }
        GFXSetCurGlobal(v115->GFX_Halo_D);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v115->i_laser_color);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 4.0);
        if (v115->i_flag_je_suis_un_ennemi)
        {
            v87 = AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 2, 1);
            v122 = AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 8, 0);
            v65 = AI_gpst_CurrentGameObject;
            v37 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v37 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v36 = v37;
            }
            else
            {
                v36 = 0;
            }
            v95 = AI_EvalFunc_ARRObjSearch_C(
                (tGameObjectMain**)(Twin_Ion_Engine_TIE_mode_flying_GST_Global_gao_o_targets + 4),
                *(uint32_t*)Twin_Ion_Engine_TIE_mode_flying_GST_Global_gao_i_targets_nb,
                v36);
            v91 = v95;
            if (v95 == -1)
            {
                v91 = AI_EvalFunc_ARRObjSearch_C(
                    (tGameObjectMain**)(Twin_Ion_Engine_TIE_mode_flying_GST_Global_gao_o_targets + 4),
                    100,
                    0);
                if (v91 != -1)
                {
                    ++* (uint32_t*)Twin_Ion_Engine_TIE_mode_flying_GST_Global_gao_i_targets_nb;
                    *(uint32_t*)(Twin_Ion_Engine_TIE_mode_flying_GST_Global_gao_o_targets + 4 * v91 + 4) = (uint32_t)AI_gpst_CurrentGameObject;
                    *(uint32_t*)AI_EvalVarArray_C3((char*)Twin_Ion_Engine_TIE_mode_flying_GST_Global_gao_v_targets_offset, v91) = 0;
                    *(uint32_t*)(AI_EvalVarArray_C3((char*)Twin_Ion_Engine_TIE_mode_flying_GST_Global_gao_v_targets_offset, v91)
                        + 4) = 0;
                    *(uint32_t*)(AI_EvalVarArray_C3((char*)Twin_Ion_Engine_TIE_mode_flying_GST_Global_gao_v_targets_offset, v91)
                        + 8) = 0;
                }
            }
            if (*(int*)Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_i_NbSpot < 20)
            {
                if (!*(uint32_t*)Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_i_NbSpot)
                    *(uint32_t*)Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_i_NbSpot = 1;
                *(uint32_t*)(Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_ao_Object
                    + 4 * *(uint32_t*)Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_i_NbSpot
                    + 4) = (uint32_t)AI_gpst_CurrentGameObject;
                *(uint32_t*)(Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_ai_Flags
                    + 4 * (*(uint32_t*)Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_i_NbSpot)++
                    + 4) = 18;
            }
        }
        else
        {
            v129 = AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 1, 2);
            if (*(int*)Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_i_NbSpot < 20)
            {
                if (!*(uint32_t*)Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_i_NbSpot)
                    *(uint32_t*)Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_i_NbSpot = 1;
                *(uint32_t*)(Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_ao_Object
                    + 4 * *(uint32_t*)Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_i_NbSpot
                    + 4) = (uint32_t)AI_gpst_CurrentGameObject;
                *(uint32_t*)(Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_ai_Flags
                    + 4 * (*(uint32_t*)Twin_Ion_Engine_TIE_mode_flying_univVars_Carte_i_NbSpot)++
                    + 4) = 1;
            }
        }
    }
    v71 = *AI_MainActor;
    if (v115->i_flag_lock_altitude)
    {
        v35 = v71;
        if (v71)
        {
            if ((unsigned int)v71 <= 2)
                v35 = AI_MainActor[(uint32_t)v71 - 1];
            AI_EvalFunc_OBJGetPos_C(v35, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        v34 = a3.z - 200.0f;
        if (v34 < (double)v115->des_f_Z_min)
            v10 = v115->des_f_Z_min;
        else
            v10 = a3.z - 200.0f;
        v115->f_Z_min = v10;
        v33 = v71;
        if (v71)
        {
            if ((unsigned int)v71 <= 2)
                v33 = AI_MainActor[(uint32_t)v71 - 1];
            AI_EvalFunc_OBJGetPos_C(v33, &v75);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v75);
        }
        v32 = v75.z + 200.0f;
        if (v32 > (double)v115->des_f_Z_max)
            v9 = v115->des_f_Z_max;
        else
            v9 = v75.z + 200.0f;
        v115->f_Z_max = v9;
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v88);
        v31 = v71;
        if (v71)
        {
            if ((unsigned int)v71 <= 2)
                v31 = AI_MainActor[(uint32_t)v71 - 1];
            AI_EvalFunc_OBJGetPos_C(v31, &v123);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v123);
        }
        v8 = (float)fabs(v88.z - v123.z);
        v96 = v8;
        if (v8 < 200.0)
            v115->i_flag_lock_altitude = 1;
        if (v115->des_mode)
        {
            v115->f_Z_min = 30.0;
            v115->f_Z_max = 1.0e14f;
        }
        else
        {
            v115->f_Z_min = v115->des_f_Z_min;
            v115->f_Z_max = v115->des_f_Z_max;
        }
    }
    if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 64))
    {
        v97 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 64);
        v30 = v97;
        if (v97)
        {
            if ((unsigned int)v97 <= 2)
                v30 = AI_MainActor[(uint32_t)v97 - 1];
            v0 = AI_EvalFunc_AIIsModel_C(v30, (tAIModel*)fctvar_Twin_Ion_Engine_TIE_mode_flying_Twin_Ion_Engine);
        }
        else
        {
            v0 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_Twin_Ion_Engine_TIE_mode_flying_Twin_Ion_Engine);
        }
        v102 = v0;
        if (v0)
            v115->f_time_paf = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 128))
    {
        v97 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 128);
        v29 = v97;
        if (v97)
        {
            if ((unsigned int)v97 <= 2)
                v29 = AI_MainActor[(uint32_t)v97 - 1];
            v1 = AI_EvalFunc_AIIsModel_C(v29, (tAIModel*)fctvar_Twin_Ion_Engine_TIE_mode_flying_Twin_Ion_Engine);
        }
        else
        {
            v1 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_Twin_Ion_Engine_TIE_mode_flying_Twin_Ion_Engine);
        }
        v116 = v1;
        if (v1)
            v115->f_time_paf = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    v92 = AI_EvalFunc_TIMEElapsed_C(v115->f_time_paf, 1.0);
    if (v92 || v115->i_flag_near_death)
    {
        if (v115->i_flag_esquive)
        {
            if (AI_EvalFunc_TIMEElapsed_C(v115->f_time_esquive, 2.0))
            {
                v115->f_time_esquive = TIM_gf_MainClock - TIM_gf_LockTime;
                v130 = AI_EvalFunc_MTHFloatSign_C(v115->f_coef_lacet_cible);
                v115->f_coef_lacet_cible = -v130 * 0.40000001f;
                if (v115->f_coef_tangage_cible >= 0.0)
                    v115->f_coef_tangage_cible = -0.60000002f;
                else
                    v115->f_coef_tangage_cible = 0.60000002f;
                v104 = TIM_gf_dt;
                v28 = 4.0f * TIM_gf_dt;
                if (v28 < 0.0)
                    v28 = 0.0;
                if (v28 > 1.0)
                    v28 = 1.0;
                v52 = v115->des_f_speed_max * v28 + v115->f_engine_traction * (1.0f - v28);
                v115->f_engine_traction = v52;
            }
        }
        else if (v115->i_flag_retour)
        {
            v64 = TIM_gf_dt;
            v27 = 4.0f * TIM_gf_dt;
            if (v27 < 0.0)
                v27 = 0.0;
            if (v27 > 1.0)
                v27 = 1.0;
            v113 = v115->des_f_speed_retour * v27 + v115->f_engine_traction * (1.0f - v27);
            v115->f_engine_traction = v113;
            v115->f_coef_lacet_cible = v115->v_hor_horizon.z * v115->v_hor_me_to_retour.z
                + v115->v_hor_horizon.y * v115->v_hor_me_to_retour.y
                + v115->v_hor_me_to_retour.x * v115->v_hor_horizon.x;
            v115->f_coef_lacet_cible = v115->f_coef_lacet_cible * 0.40000001f;
            v90 = v115->v_vert_banking.z * v115->v_me_to_retour.z
                + v115->v_vert_banking.y * v115->v_me_to_retour.y
                + v115->v_me_to_retour.x * v115->v_vert_banking.x;
            v115->f_coef_tangage_cible = -v90;
            v115->f_coef_tangage_cible = v115->f_coef_tangage_cible * 0.60000002f;
        }
        else if (AI_EvalFunc_TIMEElapsed_C(v115->f_time_target_seen, 3.0))
        {
            v106 = AI_EvalFunc_TIMEElapsed_C(v115->f_time_target_seen, 15.0);
            if (v106 || v115->i_flag_near_death)
            {
                v115->f_coef_lacet_cible = v115->v_hor_horizon.z * v115->v_hor_me_to_target.z
                    + v115->v_hor_horizon.y * v115->v_hor_me_to_target.y
                    + v115->v_hor_me_to_target.x * v115->v_hor_horizon.x;
                v115->f_coef_lacet_cible = v115->f_coef_lacet_cible * 0.40000001f;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v74);
                MATH_SubstractVec(&dest, &v115->v_target_pos, &v74);
                v108 = 0;
                v109 = 0;
                v110 = 1065353216;
                v60 = 1.0f * dest.z + 0.0f * dest.y + dest.x * 0.0f;
                v26 = v60 / 100.0f;
                if (v26 <= 0.25)
                {
                    if (v26 >= -0.25)
                        v112 = v60 / 100.0f;
                    else
                        v112 = -0.25;
                }
                else
                {
                    v112 = 0.25;
                }
                v115->f_coef_tangage_cible = -v112;
                v115->f_coef_tangage_cible = v115->f_coef_tangage_cible * 0.60000002f;
                v7 = (float)v115->i_energie;
                v6 = (float)v115->des_i_energie;
                if (v6 * 0.5 < v7)
                {
                    if (v115->f_dist_to_target <= 40000.0)
                    {
                        v77 = TIM_gf_dt;
                        v23 = 4.0f * TIM_gf_dt;
                        if (v23 < 0.0)
                            v23 = 0.0;
                        if (v23 > 1.0)
                            v23 = 1.0;
                        v42 = v115->f_target_speed * v23 + v115->f_engine_traction * (1.0f - v23);
                        v115->f_engine_traction = v42;
                    }
                    else
                    {
                        v84 = TIM_gf_dt;
                        v24 = 4.0f * TIM_gf_dt;
                        if (v24 < 0.0)
                            v24 = 0.0;
                        if (v24 > 1.0)
                            v24 = 1.0;
                        v62 = v115->des_f_speed_max * v24 + v115->f_engine_traction * (1.0f - v24);
                        v115->f_engine_traction = v62;
                    }
                }
                else
                {
                    v132 = TIM_gf_dt;
                    v25 = 4.0f * TIM_gf_dt;
                    if (v25 < 0.0)
                        v25 = 0.0;
                    if (v25 > 1.0)
                        v25 = 1.0;
                    v67 = v115->des_f_speed_retour * v25 + v115->f_engine_traction * (1.0f - v25);
                    v115->f_engine_traction = v67;
                }
            }
            else
            {
                v111 = TIM_gf_dt;
                v22 = 4.0f * TIM_gf_dt;
                if (v22 < 0.0)
                    v22 = 0.0;
                if (v22 > 1.0)
                    v22 = 1.0;
                v45 = v115->f_enfine_traction_cible * v22 + v115->f_engine_traction * (1.0f - v22);
                v115->f_engine_traction = v45;
                if (AI_EvalFunc_TIMEElapsed_C(v115->f_time_choose_rot, v115->f_delay_choose_rot))
                {
                    v115->f_time_choose_rot = TIM_gf_MainClock - TIM_gf_LockTime;
                    v115->f_delay_choose_rot = AI_EvalFunc_MATHRandFloat_C(2.0, 8.0);
                    v47 = AI_EvalFunc_MTHFloatSign_C(v115->f_coef_lacet_cible);
                    v66 = AI_EvalFunc_MATHRandFloat_C(0.0, 0.2f);
                    v115->f_coef_lacet_cible = -v47 * v66;
                    v2 = AI_EvalFunc_MATHRandFloat_C(v115->des_f_speed_min, v115->des_f_speed_max);
                    v115->f_enfine_traction_cible = (float)v2;
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v117);
                MATH_SubstractVec(&v50, &v115->v_target_pos, &v117);
                v78 = 1.0f * v50.z + 0.0f * v50.y + v50.x * 0.0f;
                v21 = v78 / 100.0f;
                if (v21 <= 0.25)
                {
                    if (v21 >= -0.25)
                        v44 = v78 / 100.0f;
                    else
                        v44 = -0.25;
                }
                else
                {
                    v44 = 0.25;
                }
                v115->f_coef_tangage_cible = -v44;
                v115->f_coef_tangage_cible = v115->f_coef_tangage_cible * 0.60000002f;
            }
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v100);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v73);
            MATH_SubstractVec(&v93, &v115->v_target_pos, &v73);
            AI_EvalFunc_MATHVecNormalize_C(&v93, &v99);
            v48 = v99.z * v100.z + v99.y * v100.y + v100.x * v99.x;
            AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v49);
            v20 = (tGameObjectMain*)v115->o_target;
            if (v20)
            {
                if ((unsigned int)v20 <= 2)
                    v20 = AI_MainActor[(uint32_t)v20 - 1];
                AI_EvalFunc_DYNSpeedGetVector_C(v20, &v94);
            }
            else
            {
                AI_EvalFunc_DYNSpeedGetVector_C(0, &v94);
            }
            MATH_SubstractVec(&v83, &v49, &v94);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v107);
            v55 = v107.z * v83.z + v107.y * v83.y + v83.x * v107.x;
            if (v55 < 60.0)
                *(float*)&v5 = 60.0;
            else
                *(float*)&v5 = v55;
            v55 = *(float*)&v5;
            v41 = AI_EvalFunc_TIMEElapsed_C(v115->f_time_esquive, 1.0);
            if (v41
                && v115->f_target_size <= (double)v115->f_dist_to_target
                && (v48 <= 0.98480803 || 0.5 * v55 * v55 <= v115->f_dist_to_target))
            {
                if (v48 >= 0.5)
                {
                    v115->f_coef_lacet_cible = v115->v_hor_horizon.z * v115->v_hor_me_to_target.z
                        + v115->v_hor_horizon.y * v115->v_hor_me_to_target.y
                        + v115->v_hor_me_to_target.x * v115->v_hor_horizon.x;
                    v115->f_coef_lacet_cible = v115->f_coef_lacet_cible * 0.40000001f;
                    v121 = v115->v_vert_banking.z * v115->v_me_to_target.z
                        + v115->v_vert_banking.y * v115->v_me_to_target.y
                        + v115->v_me_to_target.x * v115->v_vert_banking.x;
                    v115->f_coef_tangage_cible = -v121;
                    v115->f_coef_tangage_cible = v115->f_coef_tangage_cible * 0.60000002f;
                    if (v115->f_dist_to_target <= 90000.0)
                    {
                        if (v115->f_dist_to_target <= 10000.0)
                        {
                            v59 = TIM_gf_dt;
                            v15 = 4.0f * TIM_gf_dt;
                            if (v15 < 0.0)
                                v15 = 0.0;
                            if (v15 > 1.0)
                                v15 = 1.0;
                            v103 = v115->f_target_speed * v15 + v115->f_engine_traction * (1.0f - v15);
                            v115->f_engine_traction = v103;
                        }
                        else
                        {
                            v63 = TIM_gf_dt;
                            v16 = 4.0f * TIM_gf_dt;
                            if (v16 < 0.0)
                                v16 = 0.0;
                            if (v16 > 1.0)
                                v16 = 1.0;
                            v69 = v115->des_f_speed_max * v16 + v115->f_engine_traction * (1.0f - v16);
                            v115->f_engine_traction = v69;
                        }
                    }
                    else
                    {
                        v58 = TIM_gf_dt;
                        v17 = 4.0f * TIM_gf_dt;
                        if (v17 < 0.0)
                            v17 = 0.0;
                        if (v17 > 1.0)
                            v17 = 1.0;
                        v118 = v115->des_f_speed_retour * v17 + v115->f_engine_traction * (1.0f - v17);
                        v115->f_engine_traction = v118;
                    }
                }
                else
                {
                    v46 = TIM_gf_dt;
                    v18 = 4.0f * TIM_gf_dt;
                    if (v18 < 0.0)
                        v18 = 0.0;
                    if (v18 > 1.0)
                        v18 = 1.0;
                    v86 = v115->des_f_speed_max * v18 + v115->f_engine_traction * (1.0f - v18);
                    v115->f_engine_traction = v86;
                    v115->f_coef_lacet_cible = 0.0;
                    v82 = v115->v_vert_banking.z * v115->v_hor_sight.z
                        + v115->v_vert_banking.y * v115->v_hor_sight.y
                        + v115->v_hor_sight.x * v115->v_vert_banking.x;
                    v115->f_coef_tangage_cible = -v82;
                    v115->f_coef_tangage_cible = v115->f_coef_tangage_cible * 0.60000002f;
                }
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v40);
                v124 = 0;
                v125 = 0;
                v126 = -1027080192;
                if (AI_EvalFunc_TIMEElapsed_C(v115->f_time_esquive, 1.0))
                    v115->f_time_esquive = TIM_gf_MainClock - TIM_gf_LockTime;
                v101 = TIM_gf_dt;
                v19 = 4.0f * TIM_gf_dt;
                if (v19 < 0.0)
                    v19 = 0.0;
                if (v19 > 1.0)
                    v19 = 1.0;
                v51 = v115->des_f_speed_max * v19 + v115->f_engine_traction * (1.0f - v19);
                v115->f_engine_traction = v51;
                v115->f_coef_lacet_cible = 0.0;
                v54 = v115->v_vert_banking.z * v115->v_me_to_target.z
                    + v115->v_vert_banking.y * v115->v_me_to_target.y
                    + v115->v_me_to_target.x * v115->v_vert_banking.x;
                v70 = AI_EvalFunc_MTHFloatSign_C(v54);
                v85 = AI_EvalFunc_MTHFloatSign_C(v48);
                v115->f_coef_tangage_cible = v70 * v85;
                v4 = (float)fabs(v48);
                v43 = v4;
                v115->f_coef_tangage_cible = (v4 * 0.30000001f + 0.30000001f) * v115->f_coef_tangage_cible;
            }
        }
    }
    v80 = TIM_gf_dt;
    v14 = 6.0f * TIM_gf_dt;
    if (v14 < 0.0)
        v14 = 0.0;
    if (v14 > 1.0)
        v14 = 1.0;
    v61 = v115->f_coef_lacet_cible * v14 + v115->f_coef_lacet * (1.0f - v14);
    v115->f_coef_lacet = v61;
    v131 = TIM_gf_dt;
    v13 = 6.0f * TIM_gf_dt;
    if (v13 < 0.0)
        v13 = 0.0;
    if (v13 > 1.0)
        v13 = 1.0;
    v105 = v115->f_coef_tangage_cible * v13 + v115->f_coef_tangage * (1.0f - v13);
    v115->f_coef_tangage = v105;
    v79 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Twin_Ion_Engine_TIE_mode_flying_Twin_Ion_Engine_TIE_exec_s_b);
    if (v79 != 1000)
        return v79;
    if (!v115->i_flag_lock_altitude)
    {
        if (v115->des_mode)
            v115->f_engine_traction = 500.0;
    }
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v57);
    v12 = v115->f_friction * v115->f_engine_traction;
    v127.x = v57.x * v12;
    v127.y = v57.y * v12;
    v127.z = v57.z * v12;
    AI_EvalFunc_DYNTractionSet_C_CURRENT(&v127);
    return 1000;
}

int Twin_Ion_Engine_TIE_exec_s_b()
{
    double v0; // st7
    float X; // [esp+0h] [ebp-1A4h]
    float Xa; // [esp+0h] [ebp-1A4h]
    float Xb; // [esp+0h] [ebp-1A4h]
    float X_4; // [esp+4h] [ebp-1A0h]
    float v6; // [esp+8h] [ebp-19Ch]
    float v7; // [esp+Ch] [ebp-198h]
    tScriptStruct_PNJTwinIonEngine* v8; // [esp+10h] [ebp-194h]
    float v9; // [esp+20h] [ebp-184h]
    float v10; // [esp+24h] [ebp-180h]
    float v11; // [esp+28h] [ebp-17Ch]
    float a4; // [esp+70h] [ebp-134h]
    float v13; // [esp+74h] [ebp-130h]
    float v14; // [esp+74h] [ebp-130h]
    float v15; // [esp+74h] [ebp-130h]
    float v16; // [esp+74h] [ebp-130h]
    float v17; // [esp+74h] [ebp-130h]
    Vec3 v18; // [esp+78h] [ebp-12Ch] BYREF
    Vec3 v19; // [esp+84h] [ebp-120h] BYREF
    Vec3 a2; // [esp+A0h] [ebp-104h] BYREF
    float v21; // [esp+B0h] [ebp-F4h]
    float v22; // [esp+C0h] [ebp-E4h]
    Vec3 a1; // [esp+C4h] [ebp-E0h] BYREF
    Vec3 v24; // [esp+D0h] [ebp-D4h] BYREF
    float v25; // [esp+DCh] [ebp-C8h]
    int v26; // [esp+E0h] [ebp-C4h]
    int v27; // [esp+E4h] [ebp-C0h]
    Vec3 v28; // [esp+F8h] [ebp-ACh] BYREF
    float v29; // [esp+104h] [ebp-A0h]
    float v30; // [esp+108h] [ebp-9Ch]
    Vec3 dest; // [esp+10Ch] [ebp-98h] BYREF
    float v32; // [esp+124h] [ebp-80h]
    float v33; // [esp+128h] [ebp-7Ch]
    Vec3 v34; // [esp+138h] [ebp-6Ch] BYREF
    float v35; // [esp+144h] [ebp-60h]
    float v37; // [esp+14Ch] [ebp-58h]
    Vec3 v38; // [esp+154h] [ebp-50h] BYREF
    tScriptStruct_PNJTwinIonEngine* v39; // [esp+160h] [ebp-44h]
    float v40; // [esp+164h] [ebp-40h]
    float v41; // [esp+174h] [ebp-30h]
    float v42; // [esp+178h] [ebp-2Ch]
    float v43; // [esp+17Ch] [ebp-28h]
    Vec3 a5; // [esp+180h] [ebp-24h] BYREF
    Vec3 a3; // [esp+18Ch] [ebp-18h] BYREF
    Vec3 v46; // [esp+198h] [ebp-Ch] BYREF

    a2.x = 1.0;
    a2.y = 0.0;
    a2.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v8 = (tScriptStruct_PNJTwinIonEngine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v8 = 0;
    v39 = v8;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Twin_Ion_Engine_TIE_exec_s_b_Twin_Ion_Engine_TIE_exec_constraint_Z = (int)AI_ul_RealResolveOneRefFunction(0x72002067u);
        fct_Twin_Ion_Engine_TIE_exec_s_b_Twin_Ion_Engine_TIE_exec_constraint_collision = (int)AI_ul_RealResolveOneRefFunction(0x72002078u);
    }
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a1);
    v38.x = 0.0;
    v38.y = 0.0;
    v38.z = 1.0;
    v35 = TIM_gf_dt;
    X = v39->f_coef_lacet * 4.0f * TIM_gf_dt;
    AI_EvalFunc_MATHVecRotate_C(&a1, &v38, X, &v39->v_new_sight);
    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v19);
    v28.x = 0.0;
    v28.y = 0.0;
    v28.z = 1.0;
    v43 = TIM_gf_dt;
    Xa = v39->f_coef_lacet * 4.0f * TIM_gf_dt;
    AI_EvalFunc_MATHVecRotate_C(&v19, &v28, Xa, &v39->v_new_banking);
    v34.x = 0.0;
    v34.y = 0.0;
    v34.z = 1.0;
    AI_EvalFunc_MATHVecCrossProduct_C(&v34, &v39->v_new_sight, &v39->v_hor_horizon);
    MATH_NormalizeEqualVector(&v39->v_hor_horizon);
    v24.x = 0.0;
    v24.y = 0.0;
    v24.z = 1.0;
    AI_EvalFunc_MATHVecCrossProduct_C(&v39->v_hor_horizon, &v24, &v39->v_hor_sight);
    v21 = v39->v_new_banking.z * v39->v_hor_horizon.z
        + v39->v_new_banking.y * v39->v_hor_horizon.y
        + v39->v_hor_horizon.x * v39->v_new_banking.x;
    v46.x = v21 * v39->v_hor_horizon.x;
    v46.y = v39->v_hor_horizon.y * v21;
    v46.z = v39->v_hor_horizon.z * v21;
    MATH_SubstractVec(&dest, &v39->v_new_banking, &v46);
    a2 = dest;
    MATH_NormalizeEqualVector(&a2);
    v25 = TIM_gf_dt;
    Xb = (v39->f_coef_tangage + v39->f_coef_tangage) * TIM_gf_dt;
    AI_EvalFunc_MATHVecRotate_C(&a2, &v39->v_hor_horizon, Xb, &v39->v_new_banking);
    v39->f_coef_vert = v39->v_hor_sight.z * v39->v_new_banking.z
        + v39->v_hor_sight.y * v39->v_new_banking.y
        + v39->v_new_banking.x * v39->v_hor_sight.x;
    v0 = AI_EvalFunc_MTHFloatSign_C(v39->f_coef_vert);
    v39->f_coef_vert_sign = (float)v0;
    if (v39->v_new_banking.z >= 0.0)
    {
        v7 = (float)fabs(v39->f_coef_vert);
        v39->f_coef_vert = v7;
    }
    else
    {
        v39->f_coef_vert = 1.0;
    }
    v33 = AI_EvalFunc_MTHFloatSign_C(v39->f_coef_tangage);
    if (v39->f_coef_vert_sign == v33 && v39->f_coef_vert > 0.86602497)
    {
        if (v39->f_coef_vert <= 0.93969297)
        {
            a4 = 1.0f - (v39->f_coef_vert - 0.86602497f) / 0.073668003f;
            AI_EvalFunc_MATHVecBlendRotate_C(&a2, &v39->v_new_banking, a4, &a5);
        }
        else
        {
            AI_EvalFunc_MATHVecBlendRotate_C(&a2, &v39->v_new_banking, 0.0, &a5);
        }
        v39->v_new_banking = a5;
    }
    AI_EvalFunc_MATHVecCrossProduct_C(&v39->v_hor_horizon, &v39->v_new_banking, &v39->v_new_sight);
    v27 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Twin_Ion_Engine_TIE_exec_s_b_Twin_Ion_Engine_TIE_exec_constraint_Z);
    if (v27 != 1000)
        return v27;
    v26 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Twin_Ion_Engine_TIE_exec_s_b_Twin_Ion_Engine_TIE_exec_constraint_collision);
    if (v26 != 1000)
        return v26;
    v39->v_vert_banking = v39->v_new_banking;
    if (AI_EvalFunc_TIMEElapsed_C(v39->f_time_paf, 1.0))
    {
        v11 = v39->f_coef_lacet + v39->f_coef_lacet;
        if (v11 <= 0.75)
        {
            if (v11 >= -0.75)
                v13 = v39->f_coef_lacet + v39->f_coef_lacet;
            else
                v13 = -0.75;
        }
        else
        {
            v13 = 0.75;
        }
        v42 = TIM_gf_dt;
        v10 = 2.0f * TIM_gf_dt;
        if (v10 < 0.0)
            v10 = 0.0;
        if (v10 > 1.0)
            v10 = 1.0;
        v30 = v13 * v10 + v39->f_coef_roulis * (1.0f - v10);
        v39->f_coef_roulis = v30;
    }
    else
    {
        v40 = TIM_gf_MainClock - TIM_gf_LockTime;
        v14 = v40 - v39->f_time_paf;
        X_4 = v14 * 6.283185f + v14 * 6.283185f;
        v29 = (float)sinf(X_4);
        v15 = (1.0f - v14) * v29;
        v16 = v39->f_paf_roulis_sens * v15;
        v17 = v16 * 0.89999998f;
        v32 = TIM_gf_dt;
        v9 = 8.0f * TIM_gf_dt;
        if (v9 < 0.0)
            v9 = 0.0;
        if (v9 > 1.0)
            v9 = 1.0;
        v41 = v17 * v9 + v39->f_coef_roulis * (1.0f - v9);
        v39->f_coef_roulis = v41;
    }
    v22 = AI_EvalFunc_MTHFloatSign_C(v39->f_coef_roulis);
    a3.x = v22 * v39->v_hor_horizon.x;
    a3.y = v39->v_hor_horizon.y * v22;
    a3.z = v39->v_hor_horizon.z * v22;
    v6 = (float)fabs(v39->f_coef_roulis);
    v37 = v6;
    AI_EvalFunc_MATHVecBlendRotate_C(&v39->v_new_banking, &a3, v6, &v18);
    v39->v_new_banking = v18;
    AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v39->v_new_sight, &v39->v_new_banking);
    return 1000;
}

int Twin_Ion_Engine_TIE_exec_constraint_Z()
{
    double v0; // st7
    double v1; // st7
    float v3; // [esp+8h] [ebp-120h]
    float v4; // [esp+Ch] [ebp-11Ch]
    tScriptStruct_PNJTwinIonEngine* v5; // [esp+10h] [ebp-118h]
    Vec3 v6; // [esp+60h] [ebp-C8h] BYREF
    Vec3 a5; // [esp+78h] [ebp-B0h] BYREF
    float a4; // [esp+84h] [ebp-A4h]
    Vec3 a1; // [esp+98h] [ebp-90h] BYREF
    Vec3 v10; // [esp+A4h] [ebp-84h] BYREF
    Vec3 v11; // [esp+C4h] [ebp-64h] BYREF
    Vec3 v12; // [esp+D0h] [ebp-58h]
    Vec3 a2; // [esp+E0h] [ebp-48h] BYREF
    Vec3 v14; // [esp+F8h] [ebp-30h] BYREF
    Vec3 v15; // [esp+104h] [ebp-24h]
    tScriptStruct_PNJTwinIonEngine* v16; // [esp+110h] [ebp-18h]
    Vec3 v17; // [esp+114h] [ebp-14h] BYREF

    v14.x = 1.0;
    v14.y = 0.0;
    v14.z = 0.0;
    v12.x = 1.0;
    v12.y = 0.0;
    v12.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_PNJTwinIonEngine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v16 = v5;
    a4 = 0.0;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
    if (v16->f_Z_min + v16->f_Z_Offset <= a1.z)
    {
        if (v16->f_Z_max - v16->f_Z_Offset < a1.z)
        {
            a4 = 1.0f - (v16->f_Z_max - a1.z) / v16->f_Z_Offset;
            v10.x = 0.0;
            v10.y = 0.0;
            v10.z = 1.0;
            AI_EvalFunc_MATHVecBlendRotate_C(&v10, &v16->v_hor_sight, a4, &v11);
            v12 = v11;
            if (v16->v_new_sight.z > (double)v11.z)
            {
                v16->v_new_sight = v12;
                AI_EvalFunc_MATHVecCrossProduct_C(&v16->v_new_sight, &v16->v_hor_horizon, &v16->v_new_banking);
                v16->f_coef_vert = v16->v_hor_sight.z * v16->v_new_banking.z
                    + v16->v_hor_sight.y * v16->v_new_banking.y
                    + v16->v_new_banking.x * v16->v_hor_sight.x;
                v1 = AI_EvalFunc_MTHFloatSign_C(v16->f_coef_vert);
                v16->f_coef_vert_sign = (float)v1;
                v3 = (float)fabs(v16->f_coef_vert);
                v16->f_coef_vert = v3;
                AI_EvalFunc_MATHVecCrossProduct_C(&v16->v_hor_horizon, &v16->v_new_banking, &v16->v_new_sight);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v6);
            v14 = v6;
            if (v16->f_Z_max < (double)v6.z)
            {
                v14.z = v16->f_Z_max;
                AI_EvalFunc_OBJSetPos_C_CURRENT(&v14);
            }
        }
    }
    else
    {
        a4 = 1.0f - (a1.z - v16->f_Z_min) / v16->f_Z_Offset;
        v15.x = 0.0;
        v15.y = 0.0;
        v15.z = 1.0;
        a2.x = -0.0;
        a2.y = -0.0;
        a2.z = -1.0;
        AI_EvalFunc_MATHVecBlendRotate_C(&a2, &v16->v_hor_sight, a4, &a5);
        v12 = a5;
        if (v16->v_new_sight.z < (double)a5.z)
        {
            v16->v_new_sight = v12;
            AI_EvalFunc_MATHVecCrossProduct_C(&v16->v_new_sight, &v16->v_hor_horizon, &v16->v_new_banking);
            v16->f_coef_vert = v16->v_hor_sight.z * v16->v_new_banking.z
                + v16->v_hor_sight.y * v16->v_new_banking.y
                + v16->v_new_banking.x * v16->v_hor_sight.x;
            v0 = AI_EvalFunc_MTHFloatSign_C(v16->f_coef_vert);
            v16->f_coef_vert_sign = (float)v0;
            v4 = (float)fabs(v16->f_coef_vert);
            v16->f_coef_vert = v4;
            AI_EvalFunc_MATHVecCrossProduct_C(&v16->v_hor_horizon, &v16->v_new_banking, &v16->v_new_sight);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v17);
        v14 = v17;
        if (v16->f_Z_min > (double)v17.z)
        {
            v14.z = v16->f_Z_min;
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v14);
        }
    }
    return 1000;
}

int Twin_Ion_Engine_TIE_exec_gestion_paf()
{
    double v0; // st7
    float v2; // [esp+0h] [ebp-10Ch]
    tScriptStruct_PNJTwinIonEngine* v3; // [esp+4h] [ebp-108h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-100h]
    float v5; // [esp+14h] [ebp-F8h]
    double v6; // [esp+34h] [ebp-D8h]
    Vec3 v7; // [esp+44h] [ebp-C8h] BYREF
    tGameObjectMain* v8; // [esp+50h] [ebp-BCh]
    int v9; // [esp+60h] [ebp-ACh]
    tMessage Src; // [esp+64h] [ebp-A8h] BYREF
    Vec3 v11; // [esp+9Ch] [ebp-70h] BYREF
    Vec3 pos; // [esp+A8h] [ebp-64h] BYREF
    Vec3 v; // [esp+B8h] [ebp-54h] BYREF
    tGameObjectMain* v14; // [esp+C4h] [ebp-48h]
    Vec3 v15; // [esp+C8h] [ebp-44h] BYREF
    int v16; // [esp+D4h] [ebp-38h]
    int v17; // [esp+DCh] [ebp-30h]
    Vec3 a3; // [esp+E0h] [ebp-2Ch] BYREF
    float v19; // [esp+ECh] [ebp-20h]
    Vec3 a2; // [esp+F0h] [ebp-1Ch] BYREF
    tScriptStruct_PNJTwinIonEngine* v21; // [esp+FCh] [ebp-10h]
    float v22; // [esp+100h] [ebp-Ch]
    float v23; // [esp+108h] [ebp-4h]

    v11.x = 1.0;
    v11.y = 0.0;
    v11.z = 0.0;
    v.x = 1.0;
    v.y = 0.0;
    v.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_PNJTwinIonEngine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v21 = v3;
    v8 = 0;
    v9 = 0;
    v16 = 0;
    v17 = 0;
    v14 = 0;
    v22 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Twin_Ion_Engine_TIE_exec_gestion_paf_Twin_Ion_Engine_TIE_mode_meurt = (int)AI_ul_RealResolveOneRefFunction(0x7200207Au);
        Twin_Ion_Engine_TIE_exec_gestion_paf_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
    }
    v8 = v21->m_msg.gao;
    v16 = v21->m_msg.Int1 & 7;
    v9 = v21->m_msg.Int1 & 0x60;
    v17 = v21->m_msg.Int1 & 0x18;
    v21->f_time_paf = TIM_gf_MainClock - TIM_gf_LockTime;
    v21->i_flag_retour = 0;
    v6 = v21->m_msg.f2 + MATH_gd_magicDoubleForRound;
    v21->i_energie -= (*(uint32_t *)&v6);
    if (v21->i_energie <= 0)
        v21->i_energie = 0;
    if (v21->des_type)
        AI_EvalFunc_AITrackChange_C(
            1,
            (tAIFunction*)fct_Twin_Ion_Engine_TIE_exec_gestion_paf_Twin_Ion_Engine_TIE_mode_meurt);
    v = v21->m_msg.v1;
    MATH_NormalizeEqualVector(&v);
    v.x = v.x * 100.0f;
    v.y = v.y * 100.0f;
    v.z = v.z * 100.0f;
    if (v16)
    {
        AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&a2);
        v11 = a2;
        v5 = (float)v16;
        a3.x = v.x * v5;
        a3.y = v.y * v5;
        a3.z = v.z * v5;
        MATH_VecAddVec(&v7, &v11, &a3);
        AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v7);
    }
    AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v15);
    v22 = v15.z * v.z + v15.y * v.y + v.x * v15.x;
    v0 = AI_EvalFunc_MTHFloatSign_C(v22);
    v21->f_paf_roulis_sens = (float)v0;
    v2 = (float)v16;
    v21->f_coef_lacet = v2 * 0.2f * v21->f_paf_roulis_sens + v21->f_coef_lacet;
    v19 = v21->v_vert_banking.z * v.z + v21->v_vert_banking.y * v.y + v.x * v21->v_vert_banking.x;
    v23 = AI_EvalFunc_MTHFloatSign_C(v19);
    v21->f_coef_tangage_cible = v23 * 0.60000002f;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
    v14 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)Twin_Ion_Engine_TIE_exec_gestion_paf_SFX_explosion_gao, &pos);
    Src.msgID = 1;
    Src.Int1 = 22;
    Src.f2 = 5.0;
    Src.f1 = 1.0;
    Src.v1.x = 1.0;
    Src.v1.y = 0.0;
    Src.v1.z = 1.0;
    v4 = v14;
    if (v14)
    {
        if ((unsigned int)v14 <= 2)
            v4 = AI_MainActor[(uint32_t)v14 - 1];
        AI_EvalFunc_MSGSend_C(v4, &Src);
    }
    else
    {
        AI_EvalFunc_MSGSend_C(0, &Src);
    }
    return 1000;
}

int Twin_Ion_Engine_TIE_mode_meurt()
{
    float a3; // [esp+0h] [ebp-2A8h]
    float a3a; // [esp+0h] [ebp-2A8h]
    tScriptStruct_PNJTwinIonEngine* v3; // [esp+Ch] [ebp-29Ch]
    tGameObjectMain* v4; // [esp+14h] [ebp-294h]
    tGameObjectMain* v5; // [esp+1Ch] [ebp-28Ch]
    tGameObjectMain* v6; // [esp+4Ch] [ebp-25Ch]
    tGameObjectMain* v7; // [esp+54h] [ebp-254h]
    tGameObjectMain* v8; // [esp+58h] [ebp-250h]
    tGameObjectMain* v9; // [esp+5Ch] [ebp-24Ch]
    float v10; // [esp+78h] [ebp-230h]
    tGameObjectMain* v11; // [esp+80h] [ebp-228h]
    tGameObjectMain* v12; // [esp+88h] [ebp-220h]
    tGameObjectMain* v13; // [esp+90h] [ebp-218h]
    Vec3 v14; // [esp+94h] [ebp-214h] BYREF
    Vec3 pos; // [esp+B0h] [ebp-1F8h] BYREF
    Vec3 v16; // [esp+BCh] [ebp-1ECh] BYREF
    float v17; // [esp+C8h] [ebp-1E0h]
    tMessage v18; // [esp+CCh] [ebp-1DCh] BYREF
    float v19; // [esp+100h] [ebp-1A8h]
    Vec3 v20; // [esp+104h] [ebp-1A4h] BYREF
    Vec3 v21; // [esp+110h] [ebp-198h]
    Vec3 v22; // [esp+11Ch] [ebp-18Ch] BYREF
    int i; // [esp+128h] [ebp-180h]
    Vec3 v24; // [esp+12Ch] [ebp-17Ch] BYREF
    Vec3 v25; // [esp+138h] [ebp-170h] BYREF
    Vec3 a2; // [esp+148h] [ebp-160h] BYREF
    float v28; // [esp+154h] [ebp-154h]
    Vec3 v29; // [esp+158h] [ebp-150h]
    Vec3 v30; // [esp+17Ch] [ebp-12Ch]
    Vec3 v31; // [esp+188h] [ebp-120h] BYREF
    Vec3 v32; // [esp+194h] [ebp-114h] BYREF
    Vec3 v33; // [esp+1A0h] [ebp-108h] BYREF
    Vec3 v34; // [esp+1ACh] [ebp-FCh] BYREF
    tGameObjectMain* v35; // [esp+1B8h] [ebp-F0h]
    Vec3 v36; // [esp+1C0h] [ebp-E8h] BYREF
    tMessage v37; // [esp+1CCh] [ebp-DCh] BYREF
    tMessage Src; // [esp+200h] [ebp-A8h] BYREF
    float v39; // [esp+234h] [ebp-74h]
    Vec3 v41; // [esp+23Ch] [ebp-6Ch] BYREF
    Vec3 v42; // [esp+248h] [ebp-60h]
    int v43; // [esp+258h] [ebp-50h]
    Vec3 v44; // [esp+25Ch] [ebp-4Ch]
    tScriptStruct_PNJTwinIonEngine* v45; // [esp+268h] [ebp-40h]
    Vec3 v46; // [esp+26Ch] [ebp-3Ch] BYREF
    Vec3 v47; // [esp+278h] [ebp-30h] BYREF
    tGameObjectMain* v48; // [esp+284h] [ebp-24h]
    unsigned int v49; // [esp+28Ch] [ebp-1Ch]
    Vec3 v50; // [esp+290h] [ebp-18h] BYREF
    Vec3 a4; // [esp+29Ch] [ebp-Ch] BYREF

    v32.x = 1.0;
    v32.y = 0.0;
    v32.z = 0.0;
    v42.x = 1.0;
    v42.y = 0.0;
    v42.z = 0.0;
    v25.x = 1.0;
    v25.y = 0.0;
    v25.z = 0.0;
    v16.x = 1.0;
    v16.y = 0.0;
    v16.z = 0.0;
    v30.x = 1.0;
    v30.y = 0.0;
    v30.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_PNJTwinIonEngine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v45 = v3;
    v35 = 0;
    v48 = 0;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Twin_Ion_Engine_TIE_mode_meurt_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
        Twin_Ion_Engine_TIE_mode_meurt_SFX_fragment_carlingue_gao = AI_ul_RealResolveOneRefObject(0x720037D6u);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)9)
    {
        if (!v45->des_mode)
        {
            v49 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1;
        }
        AI_EvalFunc_OBJReinit_C(AI_gpst_CurrentGameObject, 1);
        return 1000;
    }
    if (v45->i_current_mode != 1)
    {
        v45->i_current_mode = 1;
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(2048, 0);
        v20.x = 0.0;
        v20.y = 0.0;
        v20.z = 0.0;
        AI_EvalFunc_DYNTractionSet_C_CURRENT(&v20);
        v43 = AI_EvalFunc_DYNFrictionSet_C(AI_gpst_CurrentGameObject, 2.0);
        v36.x = 0.0;
        v36.y = 0.0;
        v36.z = -200.0;
        AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v36);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
        v48 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)Twin_Ion_Engine_TIE_mode_meurt_SFX_explosion_gao, &pos);
        Src.msgID = 1;
        Src.Int1 = 20;
        Src.f2 = 5.0;
        Src.f1 = 1.0;
        Src.v1.x = 1.0;
        Src.v1.y = 0.0;
        Src.v1.z = 1.0;
        v13 = v48;
        if (v48)
        {
            if ((unsigned int)v48 <= 2)
                v13 = AI_MainActor[(uint32_t)v48 - 1];
            AI_EvalFunc_MSGSend_C(v13, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
        v45->o_sfx_explode = (int)v48;
        v12 = (tGameObjectMain*)v45->o_sfx_explode;
        if (v12)
        {
            if ((unsigned int)v12 <= 2)
                v12 = AI_MainActor[(uint32_t)v12 - 1];
            AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, v12, &v45->o_sfx_explode);
        }
        else
        {
            AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, 0, &v45->o_sfx_explode);
        }
        v45->m_msg.gao = AI_gpst_CurrentGameObject;
        v45->m_msg.msgID = 43;
        v11 = (tGameObjectMain*)v45->des_o_dogfight_manager;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v11 = AI_MainActor[(uint32_t)v11 - 1];
            AI_EvalFunc_MSGSend_C(v11, &v45->m_msg);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &v45->m_msg);
        }
        v45->f_coef_roulis = 0.0;
    }
    v19 = TIM_gf_dt;
    v10 = 4.0f * TIM_gf_dt;
    if (v10 < 0.0)
        v10 = 0.0;
    if (v10 > 1.0)
        v10 = 1.0;
    v39 = 5.0f * v10 + v45->f_coef_roulis * (1.0f - v10);
    v45->f_coef_roulis = v39;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v46);
    v21.x = 0.0;
    v21.y = 0.0;
    v21.z = 1.0;
    v33.x = -0.0;
    v33.y = -0.0;
    v33.z = -1.0;
    v17 = TIM_gf_dt;
    a3 = 0.5f * TIM_gf_dt;
    AI_EvalFunc_MATHVecBlend_C(&v46, &v33, a3, &a4);
    v16 = a4;
    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v31);
    AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v22);
    v28 = TIM_gf_dt;
    a3a = v45->f_coef_roulis * TIM_gf_dt;
    AI_EvalFunc_MATHVecBlend_C(&v31, &v22, a3a, &v14);
    v25 = v14;
    AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v16, &v25);
    v9 = (tGameObjectMain*)v45->o_sfx_explode;
    if (v9)
    {
        if ((unsigned int)v9 <= 2)
            v9 = AI_MainActor[(uint32_t)v9 - 1];
        v8 = v9;
    }
    else
    {
        v8 = 0;
    }
    if (v8)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
        v7 = (tGameObjectMain*)v45->o_sfx_explode;
        if (v7)
        {
            if ((unsigned int)v7 <= 2)
                v7 = AI_MainActor[(uint32_t)v7 - 1];
            AI_EvalFunc_OBJSetPos_C(v7, &a2);
        }
        else
        {
            AI_EvalFunc_OBJSetPos_C(0, &a2);
        }
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v41);
    AI_EvalFunc_WTR_GetDif_C(&v41, &v34);
    v45->f_Z_min = v34.z;
    if (v45->f_Z_min == -1000000000.0)
        v45->f_Z_min = 0.0;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v50);
    if (v45->f_Z_min <= (double)v50.z
        && !AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 64)
        && !AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 128))
    {
        return 1000;
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v24);
    v48 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)Twin_Ion_Engine_TIE_mode_meurt_SFX_explosion_gao, &v24);
    Src.msgID = 1;
    Src.Int1 = 20;
    Src.f2 = 20.0;
    Src.f1 = 1.0;
    Src.v1.x = 1.0;
    Src.v1.y = 0.0;
    Src.v1.z = 1.0;
    v6 = v48;
    if (v48)
    {
        if ((unsigned int)v48 <= 2)
            v6 = AI_MainActor[(uint32_t)v48 - 1];
        AI_EvalFunc_MSGSend_C(v6, &Src);
    }
    else
    {
        AI_EvalFunc_MSGSend_C(0, &Src);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v47);
    v32 = v47;
    v29.x = 1.0;
    v29.y = 1.0;
    v29.z = 1.0;
    v30.x = 1.0;
    v30.y = 1.0;
    v30.z = 1.0;
    v44.x = 0.0;
    v44.y = 0.0;
    v44.z = -80.0;
    v42.x = 0.0;
    v42.y = 0.0;
    v42.z = -80.0;
    v37.msgID = 10;
    v37.Int1 = 5;
    v37.Int2 = 4;
    v37.f2 = 0.5;
    v37.f1 = 160.0;
    v37.v1.x = 0.0;
    v37.v1.y = 0.0;
    v37.v1.z = 1.0;
    v18.msgID = 11;
    v18.f2 = 50.0;
    v18.f1 = 150.0;
    v18.v1.x = 1.0;
    v18.v1.y = 1.0;
    v18.v1.z = 1.0;
    v18.v2.x = 0.0;
    v18.v2.y = 0.0;
    v18.v2.z = -80.0;
    for (i = 0; i < 10; ++i)
    {
        v35 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)Twin_Ion_Engine_TIE_mode_meurt_SFX_fragment_carlingue_gao, &v32);
        v5 = v35;
        if (v35)
        {
            if ((unsigned int)v35 <= 2)
                v5 = AI_MainActor[(uint32_t)v35 - 1];
            AI_EvalFunc_MSGSend_C(v5, &v37);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &v37);
        }
        v4 = v35;
        if (v35)
        {
            if ((unsigned int)v35 <= 2)
                v4 = AI_MainActor[(uint32_t)v35 - 1];
            AI_EvalFunc_MSGSend_C(v4, &v18);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &v18);
        }
    }
    AI_EvalFunc_GFXDel_C_CURRENT(v45->GFX_Halo_G);
    AI_EvalFunc_GFXDel_C_CURRENT(v45->GFX_Halo_D);
    AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
    *AI_gppst_CurrentJumpNode = (tAINode*)9;
    TIME_Wait_C(0.1f);
    return 0;
}

int Twin_Ion_Engine_TIE_init()
{
    int result; // eax
    tScriptStruct_PNJTwinIonEngine* v1; // [esp+0h] [ebp-50h]
    tGameObjectMain* v2; // [esp+8h] [ebp-48h]
    Vec3 v3; // [esp+Ch] [ebp-44h] BYREF
    Vec3 v4; // [esp+18h] [ebp-38h] BYREF
    Vec3 v5; // [esp+24h] [ebp-2Ch] BYREF
    int v6; // [esp+30h] [ebp-20h]
    Vec3 a3[2]; // [esp+34h] [ebp-1Ch] BYREF
    tScriptStruct_PNJTwinIonEngine* v8; // [esp+4Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJTwinIonEngine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v8 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Twin_Ion_Engine_TIE_init_Twin_Ion_Engine_TIE_mode_wait_for_target = (int)AI_ul_RealResolveOneRefFunction(0x720020A8u);
    }
    if (v8->des_mode)
    {
        v8->des_f_zone_max_dist = 500.0;
        v8->des_f_speed_min = 75.0;
        v8->des_f_speed_max = 150.0;
        v8->des_f_speed_retour = 200.0;
        v8->des_f_Z_min = 30.0;
        v8->des_f_Z_max = 400.0;
        v8->des_f_laser_delay_min = 0.2f;
        v8->des_f_laser_delay_max = 0.5;
    }
    AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x8000, 0);
    v5.x = 0.0;
    v5.y = 0.0;
    v5.z = 1.0;
    AI_EvalFunc_OBJ_BankingSet_C(AI_gpst_CurrentGameObject, &v5);
    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v8->v_vert_banking);
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x800);
    v8->i_energie = v8->des_i_energie;
    v8->f_target_size = 90.0;
    if (v8->des_f_Z_max == 0.0f)
    {
        v2 = (tGameObjectMain*)v8->des_o_zone;
        if (v2)
        {
            if ((unsigned int)v2 <= 2)
                v2 = AI_MainActor[(uint32_t)v2 - 1];
            AI_EvalFunc_OBJGetPos_C(v2, a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, a3);
        }
        v8->des_f_Z_max = v8->des_f_zone_max_dist + a3[0].z;
    }
    v8->des_f_zone_max_dist = v8->des_f_zone_max_dist * v8->des_f_zone_max_dist;
    v4.x = -100.0;
    v4.y = -100.0;
    v4.z = -100.0;
    AI_EvalFunc_BVMinSet_C(AI_gpst_CurrentGameObject, &v4);
    v3.x = 100.0;
    v3.y = 100.0;
    v3.z = 100.0;
    AI_EvalFunc_BVMaxSet_C(AI_gpst_CurrentGameObject, &v3);
    XMEN_SetSize(AI_gpst_CurrentGameObject, 0, -1, 10.0);
    v6 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Twin_Ion_Engine_TIE_init_Twin_Ion_Engine_TIE_mode_wait_for_target);
    if (v6 == 1000)
        result = 1000;
    else
        result = v6;
    return result;
}

int Twin_Ion_Engine_TIE_reflex()
{
    int result; // eax
    double v1; // st7
    double v2; // st7
    BOOL v3; // eax
    BOOL v4; // eax
    BOOL v5; // eax
    BOOL v6; // eax
    BOOL v7; // eax
    BOOL v8; // eax
    unsigned int v9; // eax
    float v10; // [esp+Ch] [ebp-348h]
    float v11; // [esp+10h] [ebp-344h]
    float v12; // [esp+14h] [ebp-340h]
    tScriptStruct_PNJTwinIonEngine* v13; // [esp+1Ch] [ebp-338h]
    tGameObjectMain* v14; // [esp+20h] [ebp-334h]
    tGameObjectMain* v15; // [esp+24h] [ebp-330h]
    tGameObjectMain* v16; // [esp+2Ch] [ebp-328h]
    tGameObjectMain* v17; // [esp+34h] [ebp-320h]
    tGameObjectMain* v18; // [esp+3Ch] [ebp-318h]
    tGameObjectMain* v19; // [esp+44h] [ebp-310h]
    tGameObjectMain* v20; // [esp+4Ch] [ebp-308h]
    tGameObjectMain* v21; // [esp+54h] [ebp-300h]
    tGameObjectMain* v22; // [esp+5Ch] [ebp-2F8h]
    tGameObjectMain* v23; // [esp+64h] [ebp-2F0h]
    tGameObjectMain* v24; // [esp+6Ch] [ebp-2E8h]
    tGameObjectMain* v25; // [esp+70h] [ebp-2E4h]
    tGameObjectMain* v26; // [esp+74h] [ebp-2E0h]
    tGameObjectMain* v27; // [esp+78h] [ebp-2DCh]
    tGameObjectMain* v28; // [esp+7Ch] [ebp-2D8h]
    tGameObjectMain* v29; // [esp+8Ch] [ebp-2C8h]
    Vec3* v30; // [esp+9Ch] [ebp-2B8h]
    Vec3* v31; // [esp+A0h] [ebp-2B4h]
    Vec3* v32; // [esp+A4h] [ebp-2B0h]
    Vec3* v33; // [esp+A8h] [ebp-2ACh]
    float v34; // [esp+ACh] [ebp-2A8h]
    tGameObjectMain* v35; // [esp+B4h] [ebp-2A0h]
    Vec3* v36; // [esp+B8h] [ebp-29Ch]
    Vec3* v37; // [esp+BCh] [ebp-298h]
    tGameObjectMain* v38; // [esp+C4h] [ebp-290h]
    tGameObjectMain* v39; // [esp+CCh] [ebp-288h]
    Vec3* v40; // [esp+DCh] [ebp-278h]
    Vec3* v41; // [esp+E0h] [ebp-274h]
    Vec3* v42; // [esp+E4h] [ebp-270h]
    Vec3* v43; // [esp+E8h] [ebp-26Ch]
    Vec3* v44; // [esp+ECh] [ebp-268h]
    Vec3 a1; // [esp+F4h] [ebp-260h] BYREF
    tGameObjectMain* v46; // [esp+100h] [ebp-254h]
    tGameObjectMain* v47; // [esp+104h] [ebp-250h]
    Vec3* v48; // [esp+108h] [ebp-24Ch]
    tGameObjectMain* v49; // [esp+10Ch] [ebp-248h]
    tGameObjectMain* v50; // [esp+110h] [ebp-244h]
    tGameObjectMain* v51; // [esp+114h] [ebp-240h]
    tGameObjectMain* v52; // [esp+118h] [ebp-23Ch]
    tGameObjectMain* v53; // [esp+11Ch] [ebp-238h]
    tGameObjectMain* v54; // [esp+120h] [ebp-234h]
    tGameObjectMain* v55; // [esp+124h] [ebp-230h]
    tGameObjectMain* v56; // [esp+128h] [ebp-22Ch]
    tGameObjectMain* v57; // [esp+12Ch] [ebp-228h]
    tGameObjectMain* v58; // [esp+130h] [ebp-224h]
    tGameObjectMain* v59; // [esp+134h] [ebp-220h]
    tGameObjectMain* v60; // [esp+138h] [ebp-21Ch]
    Vec3* v61; // [esp+13Ch] [ebp-218h]
    Vec3* v62; // [esp+140h] [ebp-214h]
    Vec3* v63; // [esp+144h] [ebp-210h]
    Vec3* v64; // [esp+148h] [ebp-20Ch]
    int v65; // [esp+14Ch] [ebp-208h]
    tGameObjectMain* v66; // [esp+150h] [ebp-204h]
    tGameObjectMain* v67; // [esp+154h] [ebp-200h]
    Vec3* v68; // [esp+158h] [ebp-1FCh]
    Vec3* v69; // [esp+15Ch] [ebp-1F8h]
    Vec3* v70; // [esp+160h] [ebp-1F4h]
    Vec3* v71; // [esp+164h] [ebp-1F0h]
    int v72; // [esp+168h] [ebp-1ECh]
    tGameObjectMain* v73; // [esp+16Ch] [ebp-1E8h]
    tGameObjectMain* v74; // [esp+170h] [ebp-1E4h]
    int v75; // [esp+174h] [ebp-1E0h]
    tGameObjectMain* v76; // [esp+178h] [ebp-1DCh]
    tGameObjectMain* v77; // [esp+17Ch] [ebp-1D8h]
    uint32_t* a2; // [esp+180h] [ebp-1D4h]
    tGameObjectMain* v79; // [esp+184h] [ebp-1D0h]
    float v80; // [esp+188h] [ebp-1CCh]
    float v81; // [esp+190h] [ebp-1C4h]
    BOOL v82; // [esp+194h] [ebp-1C0h]
    Vec3 dest; // [esp+1A4h] [ebp-1B0h] BYREF
    Vec3 a3; // [esp+1C0h] [ebp-194h] BYREF
    Vec3 v85; // [esp+1CCh] [ebp-188h] BYREF
    int v86; // [esp+1D8h] [ebp-17Ch]
    float v87; // [esp+1E0h] [ebp-174h]
    BOOL v88; // [esp+1E8h] [ebp-16Ch]
    BOOL v89; // [esp+1ECh] [ebp-168h]
    void* v90; // [esp+1F0h] [ebp-164h]
    tGameObjectMain* v91; // [esp+204h] [ebp-150h]
    Vec3 v92; // [esp+208h] [ebp-14Ch] BYREF
    float v93; // [esp+214h] [ebp-140h]
    Vec3 v94; // [esp+218h] [ebp-13Ch] BYREF
    BOOL v95; // [esp+238h] [ebp-11Ch]
    Vec3 v96; // [esp+23Ch] [ebp-118h] BYREF
    Vec3 v97; // [esp+248h] [ebp-10Ch] BYREF
    float v98; // [esp+254h] [ebp-100h]
    BOOL v99; // [esp+258h] [ebp-FCh]
    float v100; // [esp+25Ch] [ebp-F8h]
    float v101; // [esp+260h] [ebp-F4h]
    float v102; // [esp+264h] [ebp-F0h]
    Vec3 v103; // [esp+268h] [ebp-ECh] BYREF
    int i; // [esp+278h] [ebp-DCh]
    tGameObjectMain* v106; // [esp+27Ch] [ebp-D8h]
    tGameObjectMain* v107; // [esp+280h] [ebp-D4h]
    int v108; // [esp+284h] [ebp-D0h]
    Vec3 v109; // [esp+288h] [ebp-CCh] BYREF
    Vec3 v110; // [esp+294h] [ebp-C0h] BYREF
    tGameObjectMain* v111; // [esp+2A0h] [ebp-B4h]
    float v112; // [esp+2A4h] [ebp-B0h]
    Vec3 v113; // [esp+2A8h] [ebp-ACh] BYREF
    Vec3 v114; // [esp+2B4h] [ebp-A0h] BYREF
    tGameObjectMain* v115; // [esp+2C8h] [ebp-8Ch]
    tGameObjectMain* v116; // [esp+2CCh] [ebp-88h]
    Vec3 v117; // [esp+2D0h] [ebp-84h] BYREF
    unsigned int v118; // [esp+2E8h] [ebp-6Ch]
    float v119; // [esp+2ECh] [ebp-68h]
    float v120; // [esp+2F0h] [ebp-64h]
    float v121; // [esp+2F4h] [ebp-60h]
    tScriptStruct_PNJTwinIonEngine* v122; // [esp+2F8h] [ebp-5Ch]
    float v123; // [esp+2FCh] [ebp-58h]
    BOOL v124; // [esp+300h] [ebp-54h]
    Vec3 v125; // [esp+304h] [ebp-50h] BYREF
    Vec3 v126; // [esp+310h] [ebp-44h] BYREF
    void* Src; // [esp+31Ch] [ebp-38h]
    int v128; // [esp+320h] [ebp-34h]
    Vec3 v129; // [esp+328h] [ebp-2Ch] BYREF
    Vec3 v130; // [esp+334h] [ebp-20h] BYREF
    BOOL v131; // [esp+34Ch] [ebp-8h]
    float v132; // [esp+350h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v13 = (tScriptStruct_PNJTwinIonEngine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v13 = 0;
    v122 = v13;
    v80 = 0.0;
    v123 = 0.0;
    i = 0;
    v81 = 0.0;
    v115 = 0;
    v112 = 0.0;
    v124 = 0;
    v116 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Twin_Ion_Engine_TIE_reflex_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Twin_Ion_Engine_TIE_reflex_univVars_Flag_speciaux_sauve,
            (void*)Twin_Ion_Engine_TIE_reflex_univVars,
            4488);
        Twin_Ion_Engine_TIE_reflex_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Twin_Ion_Engine_TIE_reflex_GST_Global_gao_o_targets,
            (void*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao,
            964);
        AI2C_ResolveBuffer(
            &Twin_Ion_Engine_TIE_reflex_GST_Global_gao_i_targets_nb,
            (void*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao,
            2572);
        AI2C_ResolveBuffer(
            &Twin_Ion_Engine_TIE_reflex_GST_Global_gao_v_targets_offset,
            (void*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao,
            1368);
        AI2C_ResolveBuffer(
            &Twin_Ion_Engine_TIE_reflex_GST_Global_gao_o_selected_target,
            (void*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao,
            2580);
        AI2C_ResolveBuffer(
            &Twin_Ion_Engine_TIE_reflex_GST_Global_gao_i_selected_target_nb,
            (void*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao,
            2748);
        AI2C_ResolveBuffer(
            &Twin_Ion_Engine_TIE_reflex_GST_Global_gao_v_selected_target_offset,
            (void*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao,
            2624);
        AI2C_ResolveBuffer(
            &Twin_Ion_Engine_TIE_reflex_univVars_Carte_i_NbSpot,
            (void*)Twin_Ion_Engine_TIE_reflex_univVars,
            4472);
        AI2C_ResolveBuffer(
            &Twin_Ion_Engine_TIE_reflex_univVars_Carte_ao_Object,
            (void*)Twin_Ion_Engine_TIE_reflex_univVars,
            4304);
        AI2C_ResolveBuffer(
            &Twin_Ion_Engine_TIE_reflex_univVars_Carte_ai_Flags,
            (void*)Twin_Ion_Engine_TIE_reflex_univVars,
            4388);
        fct_Twin_Ion_Engine_TIE_reflex_Twin_Ion_Engine_TIE_exec_afterrec = (int)AI_ul_RealResolveOneRefFunction(0x72002069u);
        fct_Twin_Ion_Engine_TIE_reflex_Twin_Ion_Engine_TIE_exec_gestion_paf = (int)AI_ul_RealResolveOneRefFunction(0x72002061u);
        fctvar_Twin_Ion_Engine_TIE_reflex_Twin_Ion_Engine = AI_ul_RealResolveOneRef(42, 0x72002063u);
        fctvar_Twin_Ion_Engine_TIE_reflex_CV_aircraft = AI_ul_RealResolveOneRef(42, 0x72001991u);
        fctvar_Twin_Ion_Engine_TIE_reflex_vaisseau_hyllis = AI_ul_RealResolveOneRef(42, 0x3D000E47u);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)11)
    {
        AI_EvalFunc_OBJReinit_C(AI_gpst_CurrentGameObject, 1);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 4;
    }
    else
    {
        v115 = *AI_MainActor;
        v116 = AI_EvalFunc_VIEWGetObject_C(0);
        v80 = 0.0;
        if (!v122->des_mode)
            goto LABEL_57;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v109);
        v79 = v116;
        if (v116)
        {
            if ((unsigned int)v79 <= 2)
                v79 = AI_MainActor[(uint32_t)v79 - 1];
            a2 = &v79->flags;
        }
        else
        {
            a2 = 0;
        }
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)a2, &a3);
        MATH_SubstractVec(&dest, &v109, &a3);
        v77 = v116;
        if (v116)
        {
            if ((unsigned int)v77 <= 2)
                v77 = AI_MainActor[(uint32_t)v77 - 1];
            v76 = v77;
        }
        else
        {
            v76 = 0;
        }
        AI_EvalFunc_OBJ_SightGet_C(v76, &v96);
        v80 = v96.z * dest.z + v96.y * dest.y + dest.x * v96.x;
        v122->des_type = v80 > 100.0;
        if (v80 >= -100.0
            || (v75 = 0,
                v124 = AI_EvalFunc_ARRBitTest_C((int*)(Twin_Ion_Engine_TIE_reflex_univVars_Flag_speciaux_sauve + 4), 2789)))
        {
        LABEL_57:
            v56 = (tGameObjectMain*)v122->des_o_zone;
            if (v56)
            {
                if ((unsigned int)v56 <= 2)
                    v56 = AI_MainActor[(uint32_t)v56 - 1];
                v55 = v56;
            }
            else
            {
                v55 = 0;
            }
            v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v55);
            v122->f_zone_sqr_dist = (float)v1;
            if (v122->des_mode && (v80 > 1000.0 || v80 < -300.0))
                v122->i_flag_retour = 1;
            if (v122->des_f_zone_max_dist < (double)v122->f_zone_sqr_dist && !v122->i_flag_retour)
            {
                v122->i_flag_retour = 1;
                v54 = (tGameObjectMain*)v122->des_o_zone;
                if (v54)
                {
                    if ((unsigned int)v54 <= 2)
                        v54 = AI_MainActor[(uint32_t)v54 - 1];
                    v53 = v54;
                }
                else
                {
                    v53 = 0;
                }
                AI_EvalFunc_BVRandomPosGet_C(v53, 1, &v122->v_retour_pos);
            }
            if (v122->i_flag_retour)
            {
                if (v122->des_mode)
                {
                    v52 = v115;
                    if (v115)
                    {
                        if ((unsigned int)v52 <= 2)
                            v52 = AI_MainActor[(uint32_t)v52 - 1];
                        v51 = v52;
                    }
                    else
                    {
                        v51 = 0;
                    }
                    AI_EvalFunc_OBJGetPos_C(v51, &v122->v_retour_pos);
                    v50 = v116;
                    if (v116)
                    {
                        if ((unsigned int)v50 <= 2)
                            v50 = AI_MainActor[(uint32_t)v50 - 1];
                        v49 = v50;
                    }
                    else
                    {
                        v49 = 0;
                    }
                    AI_EvalFunc_OBJ_SightGet_C(v49, &v114);
                    v100 = v114.x * 200.0f;
                    v101 = v114.y * 200.0f;
                    v102 = v114.z * 200.0f;
                    v48 = &v122->v_retour_pos;
                    v122->v_retour_pos.x = v100 + v122->v_retour_pos.x;
                    v48->y = v101 + v48->y;
                    v48->z = v102 + v48->z;
                    v47 = v115;
                    if (v115)
                    {
                        if ((unsigned int)v47 <= 2)
                            v47 = AI_MainActor[(uint32_t)v47 - 1];
                        v46 = v47;
                    }
                    else
                    {
                        v46 = 0;
                    }
                    AI_EvalFunc_OBJGetPos_C(v46, &v129);
                    v122->v_retour_pos.z = v129.z + 60.0f;
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v110);
                MATH_SubstractVec(&a1, &v110, &v122->v_retour_pos);
                v93 = MATH_VecLen(&a1);
                if (v93 >= 200.0)
                {
                    v43 = &v122->v_me_to_retour;
                    v44 = &v122->v_retour_pos;
                    v122->v_me_to_retour.x = v122->v_retour_pos.x;
                    v43->y = v44->y;
                    v43->z = v44->z;
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v92);
                    v42 = &v122->v_me_to_retour;
                    v122->v_me_to_retour.x = v122->v_me_to_retour.x - v92.x;
                    v42->y = v42->y - v92.y;
                    v42->z = v42->z - v92.z;
                    v40 = &v122->v_hor_me_to_retour;
                    v41 = &v122->v_me_to_retour;
                    v122->v_hor_me_to_retour.x = v122->v_me_to_retour.x;
                    v40->y = v41->y;
                    v40->z = v41->z;
                    MATH_NormalizeEqualVector(&v122->v_me_to_retour);
                    AI_EvalFunc_MATHVecSetHorzNormalize_C(&v122->v_hor_me_to_retour);
                }
                else
                {
                    v122->i_flag_retour = 0;
                }
            }
            v39 = (tGameObjectMain*)v122->o_target;
            if (v39)
            {
                if ((unsigned int)v39 <= 2)
                    v39 = AI_MainActor[(uint32_t)v39 - 1];
                v2 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v39);
            }
            else
            {
                v2 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
            }
            v122->f_dist_to_target = (float)v2;
            v38 = (tGameObjectMain*)v122->o_target;
            if (v38)
            {
                if ((unsigned int)v38 <= 2)
                    v38 = AI_MainActor[(uint32_t)v38 - 1];
                AI_EvalFunc_OBJGetPos_C(v38, &v122->v_target_pos);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v122->v_target_pos);
            }
            v36 = &v122->v_me_to_target;
            v37 = &v122->v_target_pos;
            v122->v_me_to_target.x = v122->v_target_pos.x;
            v36->y = v37->y;
            v36->z = v37->z;
            v35 = (tGameObjectMain*)v122->o_target;
            if (v35)
            {
                if ((unsigned int)v35 <= 2)
                    v35 = AI_MainActor[(uint32_t)v35 - 1];
                AI_EvalFunc_DYNSpeedGetVector_C(v35, &v125);
            }
            else
            {
                AI_EvalFunc_DYNSpeedGetVector_C(0, &v125);
            }
            v98 = TIM_gf_dt;
            v34 = TIM_gf_dt * 10.0f;
            v119 = v125.x * v34;
            v120 = v125.y * v34;
            v121 = v125.z * v34;
            v33 = &v122->v_me_to_target;
            v122->v_me_to_target.x = v119 + v122->v_me_to_target.x;
            v33->y = v120 + v33->y;
            v33->z = v121 + v33->z;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v97);
            v32 = &v122->v_me_to_target;
            v122->v_me_to_target.x = v122->v_me_to_target.x - v97.x;
            v32->y = v32->y - v97.y;
            v32->z = v32->z - v97.z;
            v30 = &v122->v_hor_me_to_target;
            v31 = &v122->v_me_to_target;
            v122->v_hor_me_to_target.x = v122->v_me_to_target.x;
            v30->y = v31->y;
            v30->z = v31->z;
            MATH_NormalizeEqualVector(&v122->v_me_to_target);
            AI_EvalFunc_MATHVecSetHorzNormalize_C(&v122->v_hor_me_to_target);
            v29 = (tGameObjectMain*)v122->o_target;
            if (v29)
            {
                if ((unsigned int)v29 <= 2)
                    v29 = AI_MainActor[(uint32_t)v29 - 1];
                AI_EvalFunc_DYNSpeedGetVector_C(v29, &v103);
            }
            else
            {
                AI_EvalFunc_DYNSpeedGetVector_C(0, &v103);
            }
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v126);
            v87 = v126.z * v103.z + v126.y * v103.y + v103.x * v126.x;
            if (v87 < (double)v122->des_f_speed_min)
                v12 = v122->des_f_speed_min;
            else
                v12 = v87;
            v132 = v12;
            v122->f_target_speed = v12;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v113);
            v112 = v113.z * v122->v_me_to_target.z + v113.y * v122->v_me_to_target.y + v113.x * v122->v_me_to_target.x;
            if (v112 > 0.5)
            {
                v122->f_time_target_seen = TIM_gf_MainClock - TIM_gf_LockTime;
                v122->f_time_choose_rot = TIM_gf_MainClock - TIM_gf_LockTime;
            }
            Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
            memcpy(&v122->m_msg, Src, sizeof(v122->m_msg));
            while (v122->m_msg.msgID)
            {
                switch (v122->m_msg.msgID)
                {
                case 0x25:
                    v108 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Twin_Ion_Engine_TIE_reflex_Twin_Ion_Engine_TIE_exec_gestion_paf);
                    if (v108 != 1000)
                        return v108;
                    break;
                case 0x4D:
                    if (v122->m_msg.Int1)
                    {
                        v122->i_flag_esquive = 1;
                        v122->f_time_lock = TIM_gf_MainClock - TIM_gf_LockTime;
                        v122->f_time_esquive = TIM_gf_MainClock - TIM_gf_LockTime;
                        v122->o_missile_lock_on_me = (int)v122->m_msg.gao;
                        v122->o_ennemy_lock_on_me = (int)v122->m_msg.gao2;
                        v122->f_missile_life_time = v122->m_msg.f2;
                    }
                    else
                    {
                        v28 = (tGameObjectMain*)v122->o_missile_lock_on_me;
                        if (v28)
                        {
                            if ((unsigned int)v28 <= 2)
                                v28 = AI_MainActor[(uint32_t)v28 - 1];
                            v27 = v28;
                        }
                        else
                        {
                            v27 = 0;
                        }
                        v26 = v122->m_msg.gao;
                        if (v26)
                        {
                            if ((unsigned int)v26 <= 2)
                                v26 = AI_MainActor[(uint32_t)v26 - 1];
                            v25 = v26;
                        }
                        else
                        {
                            v25 = 0;
                        }
                        if (v27 == v25)
                        {
                            v122->i_flag_esquive = 0;
                            v122->o_missile_lock_on_me = 0;
                            v122->f_missile_life_time = 0.0;
                        }
                    }
                    break;
                case 0x10:
                    v122->o_target = (int)v122->m_msg.gao2;
                    break;
                }
                v90 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
                memcpy(&v122->m_msg, v90, sizeof(v122->m_msg));
            }
            if (v122->des_mode)
            {
                v24 = (tGameObjectMain*)v122->o_target;
                if (v24)
                {
                    if ((unsigned int)v24 <= 2)
                        v24 = AI_MainActor[(uint32_t)v24 - 1];
                    AI_EvalFunc_OBJGetPos_C(v24, &v94);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v94);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v130);
                MATH_SubstractVec(&v117, &v94, &v130);
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v85);
                v112 = v85.z * v117.z + v85.y * v117.y + v117.x * v85.x;
                v81 = 100.0;
                v123 = 500.0;
                v23 = (tGameObjectMain*)v122->o_target;
                if (v23)
                {
                    if ((unsigned int)v23 <= 2)
                        v23 = AI_MainActor[(uint32_t)v23 - 1];
                    v3 = AI_EvalFunc_AIIsModel_C(v23, (tAIModel*)fctvar_Twin_Ion_Engine_TIE_reflex_Twin_Ion_Engine);
                }
                else
                {
                    v3 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_Twin_Ion_Engine_TIE_reflex_Twin_Ion_Engine);
                }
                v131 = v3;
                if (v3)
                {
                    v22 = (tGameObjectMain*)v122->o_target;
                    if (v22)
                    {
                        if ((unsigned int)v22 <= 2)
                            v22 = AI_MainActor[(uint32_t)v22 - 1];
                        v4 = AI_EvalFunc_OBJ_CapaTest_C(v22, 0x800);
                    }
                    else
                    {
                        v4 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x800);
                    }
                    v88 = v4;
                    if (v4)
                        v112 = 1.0e14f;
                    v122->f_target_size = 90.0;
                    v81 = 100.0;
                    v123 = 1500.0;
                }
                else
                {
                    v21 = (tGameObjectMain*)v122->o_target;
                    if (v21)
                    {
                        if ((unsigned int)v21 <= 2)
                            v21 = AI_MainActor[(uint32_t)v21 - 1];
                        v5 = AI_EvalFunc_AIIsModel_C(v21, (tAIModel*)fctvar_Twin_Ion_Engine_TIE_reflex_CV_aircraft);
                    }
                    else
                    {
                        v5 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_Twin_Ion_Engine_TIE_reflex_CV_aircraft);
                    }
                    v99 = v5;
                    if (v5)
                    {
                        v20 = (tGameObjectMain*)v122->o_target;
                        if (v20)
                        {
                            if ((unsigned int)v20 <= 2)
                                v20 = AI_MainActor[(uint32_t)v20 - 1];
                            v6 = AI_EvalFunc_OBJ_CapaTest_C(v20, 0x800);
                        }
                        else
                        {
                            v6 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x800);
                        }
                        v89 = v6;
                        if (v6)
                            v112 = 1.0e14f;
                        v122->f_target_size = 10000.0;
                        v81 = 200.0;
                        v123 = 600.0;
                    }
                    else
                    {
                        v19 = (tGameObjectMain*)v122->o_target;
                        if (v19)
                        {
                            if ((unsigned int)v19 <= 2)
                                v19 = AI_MainActor[(uint32_t)v19 - 1];
                            v7 = AI_EvalFunc_AIIsModel_C(v19, (tAIModel*)fctvar_Twin_Ion_Engine_TIE_reflex_vaisseau_hyllis);
                        }
                        else
                        {
                            v7 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_Twin_Ion_Engine_TIE_reflex_vaisseau_hyllis);
                        }
                        v95 = v7;
                        if (v7)
                        {
                            v18 = (tGameObjectMain*)v122->o_target;
                            if (v18)
                            {
                                if ((unsigned int)v18 <= 2)
                                    v18 = AI_MainActor[(uint32_t)v18 - 1];
                                v8 = AI_EvalFunc_OBJ_CapaTest_C(v18, 0x800);
                            }
                            else
                            {
                                v8 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x800);
                            }
                            v82 = v8;
                            if (v8)
                                v112 = 1.0e14f;
                            v122->f_target_size = 10000.0;
                            v81 = 200.0;
                            v123 = 2000.0;
                        }
                    }
                }
                v17 = (tGameObjectMain*)v122->o_target;
                if (v17)
                {
                    if ((unsigned int)v17 <= 2)
                        v17 = AI_MainActor[(uint32_t)v17 - 1];
                    v9 = AI_EvalFunc_OBJ_FlagsControlGet_C(v17);
                }
                else
                {
                    v9 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
                }
                v118 = v9;
                if ((v9 & 0x200000) != 0 || v112 < (double)v81 || v112 >(double)v123)
                {
                    v122->m_msg.msgID = 16;
                    v122->m_msg.gao = AI_gpst_CurrentGameObject;
                    v16 = (tGameObjectMain*)v122->des_o_dogfight_manager;
                    if (v16)
                    {
                        if ((unsigned int)v16 <= 2)
                            v16 = AI_MainActor[(uint32_t)v16 - 1];
                        AI_EvalFunc_MSGSend_C(v16, &v122->m_msg);
                    }
                    else
                    {
                        AI_EvalFunc_MSGSend_C(0, &v122->m_msg);
                    }
                }
            }
            v11 = (float)v122->i_energie;
            v10 = (float)v122->des_i_energie;
            v122->i_flag_near_death = v10 * 0.5 > v11;
            v15 = (tGameObjectMain*)v122->o_missile_lock_on_me;
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
            if (v14)
            {
                if (AI_EvalFunc_TIMEElapsed_C(v122->f_time_lock, v122->f_missile_life_time))
                {
                    v122->i_flag_esquive = 0;
                    v122->o_missile_lock_on_me = 0;
                    v122->f_missile_life_time = 0.0;
                }
            }
            result = 1000;
        }
        else
        {
            if (v122->i_flag_je_suis_un_ennemi)
            {
                v111 = AI_gpst_CurrentGameObject;
                v74 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)v74 <= 2)
                        v74 = AI_MainActor[(uint32_t)v74 - 1];
                    v73 = v74;
                }
                else
                {
                    v73 = 0;
                }
                v72 = 0;
                v128 = AI_EvalFunc_ARRObjSearch_C(
                    (tGameObjectMain**)(Twin_Ion_Engine_TIE_reflex_GST_Global_gao_o_targets + 4),
                    100,
                    v73);
                i = v128;
                if (v128 != -1)
                {
                    *(uint32_t*)(Twin_Ion_Engine_TIE_reflex_GST_Global_gao_o_targets + 4 * i + 4) = *(uint32_t*)(Twin_Ion_Engine_TIE_reflex_GST_Global_gao_o_targets + 4 * *(uint32_t*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_i_targets_nb);
                    v69 = (Vec3*)AI_EvalVarArray_C3(
                        (char*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_v_targets_offset,
                        *(uint32_t*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_i_targets_nb - 1);
                    v68 = (Vec3*)AI_EvalVarArray_C3((char*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_v_targets_offset, i);
                    v70 = v68;
                    v71 = v69;
                    v68->x = v69->x;
                    v70->y = v71->y;
                    v70->z = v71->z;
                    *(uint32_t*)(Twin_Ion_Engine_TIE_reflex_GST_Global_gao_o_targets
                        + 4 * *(uint32_t*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_i_targets_nb) = 0;
                    *(uint32_t*)AI_EvalVarArray_C3(
                        (char*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_v_targets_offset,
                        *(uint32_t*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_i_targets_nb - 1) = 0;
                    *(uint32_t*)(AI_EvalVarArray_C3(
                        (char*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_v_targets_offset,
                        *(uint32_t*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_i_targets_nb - 1)
                        + 4) = 0;
                    *(uint32_t*)(AI_EvalVarArray_C3(
                        (char*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_v_targets_offset,
                        *(uint32_t*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_i_targets_nb - 1)
                        + 8) = 0;
                    --* (uint32_t*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_i_targets_nb;
                }
                v106 = AI_gpst_CurrentGameObject;
                v67 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)v67 <= 2)
                        v67 = AI_MainActor[(uint32_t)v67 - 1];
                    v66 = v67;
                }
                else
                {
                    v66 = 0;
                }
                v65 = 0;
                v86 = AI_EvalFunc_ARRObjSearch_C(
                    (tGameObjectMain**)(Twin_Ion_Engine_TIE_reflex_GST_Global_gao_o_selected_target + 4),
                    10,
                    v66);
                i = v86;
                if (v86 != -1)
                {
                    *(uint32_t*)(Twin_Ion_Engine_TIE_reflex_GST_Global_gao_o_selected_target + 4 * i + 4) = *(uint32_t*)(Twin_Ion_Engine_TIE_reflex_GST_Global_gao_o_selected_target + 4 * *(uint32_t*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_i_selected_target_nb);
                    v62 = (Vec3*)AI_EvalVarArray_C3(
                        (char*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_v_selected_target_offset,
                        *(uint32_t*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_i_selected_target_nb - 1);
                    v61 = (Vec3*)AI_EvalVarArray_C3(
                        (char*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_v_selected_target_offset,
                        i);
                    v63 = v61;
                    v64 = v62;
                    v61->x = v62->x;
                    v63->y = v64->y;
                    v63->z = v64->z;
                    *(uint32_t*)(Twin_Ion_Engine_TIE_reflex_GST_Global_gao_o_selected_target
                        + 4 * *(uint32_t*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_i_selected_target_nb) = 0;
                    *(uint32_t*)AI_EvalVarArray_C3(
                        (char*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_v_selected_target_offset,
                        *(uint32_t*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_i_selected_target_nb - 1) = 0;
                    *(uint32_t*)(AI_EvalVarArray_C3(
                        (char*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_v_selected_target_offset,
                        *(uint32_t*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_i_selected_target_nb - 1)
                        + 4) = 0;
                    *(uint32_t*)(AI_EvalVarArray_C3(
                        (char*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_v_selected_target_offset,
                        *(uint32_t*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_i_selected_target_nb - 1)
                        + 8) = 0;
                    --* (uint32_t*)Twin_Ion_Engine_TIE_reflex_GST_Global_gao_i_selected_target_nb;
                }
                for (i = 0; i < *(int32_t*)Twin_Ion_Engine_TIE_reflex_univVars_Carte_i_NbSpot; ++i)
                {
                    v107 = AI_gpst_CurrentGameObject;
                    v60 = AI_gpst_CurrentGameObject;
                    if (AI_gpst_CurrentGameObject)
                    {
                        if ((unsigned int)v60 <= 2)
                            v60 = AI_MainActor[(uint32_t)v60 - 1];
                        v59 = v60;
                    }
                    else
                    {
                        v59 = 0;
                    }
                    if (*(tGameObjectMain**)(Twin_Ion_Engine_TIE_reflex_univVars_Carte_ao_Object + 4 * i + 4) == v59
                        && i < -- * (int32_t*)Twin_Ion_Engine_TIE_reflex_univVars_Carte_i_NbSpot)
                    {
                        *(uint32_t*)(Twin_Ion_Engine_TIE_reflex_univVars_Carte_ao_Object + 4 * i + 4) = *(uint32_t*)(Twin_Ion_Engine_TIE_reflex_univVars_Carte_ao_Object + 4 * *(uint32_t*)Twin_Ion_Engine_TIE_reflex_univVars_Carte_i_NbSpot + 4);
                        *(uint32_t*)(Twin_Ion_Engine_TIE_reflex_univVars_Carte_ai_Flags + 4 * i + 4) = *(uint32_t*)(Twin_Ion_Engine_TIE_reflex_univVars_Carte_ai_Flags + 4 * *(uint32_t*)Twin_Ion_Engine_TIE_reflex_univVars_Carte_i_NbSpot + 4);
                    }
                }
            }
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
            AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
            AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
            AI_EvalFunc_GFXDel_C_CURRENT(v122->GFX_Halo_G);
            AI_EvalFunc_GFXDel_C_CURRENT(v122->GFX_Halo_D);
            v91 = AI_gpst_CurrentGameObject;
            v58 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)v58 <= 2)
                    v58 = AI_MainActor[(uint32_t)v58 - 1];
                v57 = v58;
            }
            else
            {
                v57 = 0;
            }
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                v57,
                2,
                (tAIFunction*)fct_Twin_Ion_Engine_TIE_reflex_Twin_Ion_Engine_TIE_exec_afterrec);
            AI_EvalFunc_AITrackStop_C(1);
            *AI_gppst_CurrentJumpNode = (tAINode*)11;
            TIME_Wait_C(0.1f);
            result = 0;
        }
    }
    return result;
}

int Twin_Ion_Engine_TIE_exec_constraint_collision()
{
    double v0; // st7
    float X; // [esp+0h] [ebp-88h]
    float Xa; // [esp+0h] [ebp-88h]
    float v4; // [esp+8h] [ebp-80h]
    tScriptStruct_PNJTwinIonEngine* v5; // [esp+Ch] [ebp-7Ch]
    Vec3 a4; // [esp+40h] [ebp-48h] BYREF
    Vec3 v7; // [esp+4Ch] [ebp-3Ch] BYREF
    tScriptStruct_PNJTwinIonEngine* v8; // [esp+78h] [ebp-10h]
    float v10; // [esp+84h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_PNJTwinIonEngine*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v8 = v5;
    if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 64))
    {
        v8->f_time_ground_col = TIM_gf_MainClock - TIM_gf_LockTime;
        AI_EvalFunc_COLNormalGet_C(AI_gpst_CurrentGameObject, 64, &v8->v_normale_sol);
    }
    if (!AI_EvalFunc_TIMEElapsed_C(v8->f_time_ground_col, 0.2f))
    {
        X = 2.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlend_C(&v8->v_new_banking, &v8->v_normale_sol, X, &a4);
        v8->v_new_banking = a4;
        v8->f_coef_vert = v8->v_hor_sight.z * v8->v_new_banking.z
            + v8->v_hor_sight.y * v8->v_new_banking.y
            + v8->v_new_banking.x * v8->v_hor_sight.x;
        v0 = AI_EvalFunc_MTHFloatSign_C(v8->f_coef_vert);
        v8->f_coef_vert_sign = (float)v0;
        v4 = (float)fabs(v8->f_coef_vert);
        v8->f_coef_vert = v4;
    }
    if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 128))
    {
        v8->f_time_wall_col = TIM_gf_MainClock - TIM_gf_LockTime;
        AI_EvalFunc_COLNormalGet_C(AI_gpst_CurrentGameObject, 128, &v8->v_normale_wall);
    }
    if (!AI_EvalFunc_TIMEElapsed_C(v8->f_time_wall_col, 0.2f))
    {
        v10 = TIM_gf_dt;
        Xa = 2.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlend_C(&v8->v_new_sight, &v8->v_normale_wall, Xa, &v7);
        v8->v_new_sight = v7;
    }
    return 1000;
}