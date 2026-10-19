#include "ai_compiledscripts_inc.h"

//CV_aircraft_CV_exec_test_collision
int fctvar_CV_aircraft_CV_exec_test_collision_objet_flottant;
int CV_aircraft_CV_exec_test_collision_univVars;
int CV_aircraft_CV_exec_test_collision_univVars_Perso_ID_passengers_list;

//CV_aircraft_CV_exec_GFX_Smoke
int CV_aircraft_CV_exec_GFX_Smoke_SFX_explosion_gao;

//CV_aircraft_CV_exec_GFX_Flammes
int CV_aircraft_CV_exec_GFX_Flammes_SFX_explosion_gao;

//CV_aircraft_CV_exec_GFX_Halo
int CV_aircraft_CV_exec_GFX_Halo_SFX_light_smoke_gao;

//CV_aircraft_CV_init
int CV_aircraft_CV_init_univVars;
int CV_aircraft_CV_init_univVars_Flag_speciaux_sauve;
int CV_aircraft_CV_init_univVars_Perso_ID_actor;
int CV_aircraft_CV_init_GST_Global_gao;
int CV_aircraft_CV_init_GST_Global_gao_i_map_just_reset;
int CV_aircraft_CV_init_univVars_world_ID;
int CV_aircraft_CV_init_SFX_light_smoke_gao;
int fct_CV_aircraft_CV_init_CV_aircraft_CV_exec_afterrec;
int CV_aircraft_CV_init_GST_Global_gao_o_targets;
int CV_aircraft_CV_init_GST_Global_gao_i_targets_nb;
int CV_aircraft_CV_init_GST_Global_gao_v_targets_offset;
int fct_CV_aircraft_CV_init_CV_aircraft_CV_mode_deplacement;

//CV_aircraft_CV_mode_deplacement
int CV_aircraft_CV_mode_deplacement_GST_Global_gao;
int CV_aircraft_CV_mode_deplacement_GST_Global_gao_i_Domz_Attack_Boat_Shoot_NB;
int CV_aircraft_CV_mode_deplacement__SFX_explosion_gao;
int CV_aircraft_CV_mode_deplacement_univVars;
int CV_aircraft_CV_mode_deplacement_univVars_Perso_ID_passengers_list;
int CV_aircraft_CV_mode_deplacement_univVars_Perso_ID_actor;
int CV_aircraft_CV_mode_deplacement_univVars_Flag_speciaux_sauve;
int fct_CV_aircraft_CV_mode_deplacement_CV_aircraft_CV_exec_waterforce;
int CV_aircraft_CV_mode_deplacement_univVars_world_ID;
int fct_CV_aircraft_CV_mode_deplacement_CV_aircraft_CV_exec_s_b;
int fct_CV_aircraft_CV_mode_deplacement_CV_aircraft_CV_exec_check_pos;
int fct_CV_aircraft_CV_mode_deplacement_CV_aircraft_CV_exec_test_collision;
int fctvar_CV_aircraft_CV_mode_deplacement_CV_aircraft;

//CV_aircraft_CV_exec_afterrec
int fct_CV_aircraft_CV_exec_afterrec_CV_aircraft_CV_exec_trace;
int fct_CV_aircraft_CV_exec_afterrec_CV_aircraft_CV_exec_GFX_Halo;
int fct_CV_aircraft_CV_exec_afterrec_CV_aircraft_CV_exec_GFX_Smoke;
int fct_CV_aircraft_CV_exec_afterrec_CV_aircraft_CV_exec_GFX_Flammes;

int CV_aircraft_CV_exec_test_collision()
{
    float v1; // [esp+0h] [ebp-250h]
    float v2; // [esp+4h] [ebp-24Ch]
    tScriptStruct_CVAirCraft* v3; // [esp+8h] [ebp-248h]
    tGameObjectMain* v4; // [esp+10h] [ebp-240h]
    tAI* v5; // [esp+14h] [ebp-23Ch]
    unsigned int v6; // [esp+18h] [ebp-238h]
    tGameObjectMain* v7; // [esp+1Ch] [ebp-234h]
    tGameObjectMain* v8; // [esp+20h] [ebp-230h]
    tAI* v9; // [esp+24h] [ebp-22Ch]
    unsigned int v10; // [esp+28h] [ebp-228h]
    tGameObjectMain* v11; // [esp+2Ch] [ebp-224h]
    tGameObjectMain* v12; // [esp+30h] [ebp-220h]
    tGameObjectMain* v13; // [esp+38h] [ebp-218h]
    tAI* v14; // [esp+3Ch] [ebp-214h]
    unsigned int v15; // [esp+40h] [ebp-210h]
    tGameObjectMain* v16; // [esp+44h] [ebp-20Ch]
    tGameObjectMain* v17; // [esp+48h] [ebp-208h]
    tAI* v18; // [esp+4Ch] [ebp-204h]
    tAI* v19; // [esp+50h] [ebp-200h]
    tGameObjectMain* v20; // [esp+54h] [ebp-1FCh]
    tGameObjectMain* v21; // [esp+58h] [ebp-1F8h]
    tGameObjectMain* v22; // [esp+60h] [ebp-1F0h]
    tGameObjectMain* v23; // [esp+64h] [ebp-1ECh]
    tGameObjectMain* v24; // [esp+80h] [ebp-1D0h]
    Vec3 v25; // [esp+88h] [ebp-1C8h] BYREF
    Vec3* v26; // [esp+94h] [ebp-1BCh]
    Vec3* v27; // [esp+98h] [ebp-1B8h]
    tGameObjectMain* v28; // [esp+9Ch] [ebp-1B4h]
    tGameObjectMain* v29; // [esp+A0h] [ebp-1B0h]
    tGameObjectMain* v30; // [esp+A4h] [ebp-1ACh]
    tGameObjectMain* v31; // [esp+A8h] [ebp-1A8h]
    tGameObjectMain* a1; // [esp+ACh] [ebp-1A4h]
    tGameObjectMain* v33; // [esp+B0h] [ebp-1A0h]
    Vec3* v34; // [esp+B4h] [ebp-19Ch]
    Vec3* v35; // [esp+B8h] [ebp-198h]
    uint32_t* a2; // [esp+BCh] [ebp-194h]
    tGameObjectMain* v37; // [esp+C0h] [ebp-190h]
    tGameObjectMain* v38; // [esp+C4h] [ebp-18Ch]
    tGameObjectMain* v39; // [esp+C8h] [ebp-188h]
    tGameObjectMain* v40; // [esp+CCh] [ebp-184h]
    tGameObjectMain* v41; // [esp+D0h] [ebp-180h]
    tGameObjectMain* v42; // [esp+D4h] [ebp-17Ch]
    tGameObjectMain* v43; // [esp+D8h] [ebp-178h]
    tGameObjectMain* v44; // [esp+DCh] [ebp-174h]
    tGameObjectMain* v45; // [esp+E0h] [ebp-170h]
    float v48; // [esp+F0h] [ebp-160h]
    Vec3 v49; // [esp+F8h] [ebp-158h] BYREF
    Vec3 a3; // [esp+104h] [ebp-14Ch] BYREF
    int a5; // [esp+110h] [ebp-140h] BYREF
    int v52; // [esp+114h] [ebp-13Ch]
    Vec3 v53; // [esp+118h] [ebp-138h] BYREF
    Vec3 v54; // [esp+134h] [ebp-11Ch] BYREF
    Vec3 a4; // [esp+140h] [ebp-110h] BYREF
    float v56; // [esp+14Ch] [ebp-104h]
    tGameObjectMain* v57; // [esp+150h] [ebp-100h]
    Vec3 v58; // [esp+154h] [ebp-FCh] BYREF
    Vec3 v59; // [esp+160h] [ebp-F0h] BYREF
    tGameObjectMain* v60; // [esp+16Ch] [ebp-E4h]
    Vec3 v62; // [esp+184h] [ebp-CCh] BYREF
    Vec3 v63; // [esp+19Ch] [ebp-B4h] BYREF
    Vec3 v64; // [esp+1ACh] [ebp-A4h] BYREF
    float v65; // [esp+1B8h] [ebp-98h]
    tGameObjectMain* v66; // [esp+1BCh] [ebp-94h]
    Vec3 v67; // [esp+1C0h] [ebp-90h] BYREF
    Vec3 v69; // [esp+1D0h] [ebp-80h] BYREF
    tScriptStruct_CVAirCraft* v70; // [esp+1E8h] [ebp-68h]
    Vec3 dest; // [esp+1ECh] [ebp-64h] BYREF
    Vec3 v72; // [esp+20Ch] [ebp-44h] BYREF
    tMessage Src; // [esp+218h] [ebp-38h] BYREF
    float v74; // [esp+24Ch] [ebp-4h]

    v54.x = 1.0;
    v54.y = 0.0;
    v54.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_CVAirCraft*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v70 = v3;
    a5 = 0;
    v52 = 0;
    v48 = 0.0;
    v57 = 0;
    v60 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fctvar_CV_aircraft_CV_exec_test_collision_objet_flottant = AI_ul_RealResolveOneRef(42, 0x72000E6Fu);
        CV_aircraft_CV_exec_test_collision_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &CV_aircraft_CV_exec_test_collision_univVars_Perso_ID_passengers_list,
            (void*)CV_aircraft_CV_exec_test_collision_univVars,
            936);
    }
    v60 = AI_EvalFunc_VIEWGetObject_C(0);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v69);
    v66 = AI_EvalFunc_COLBestWallGaoGet_C(AI_gpst_CurrentGameObject, &v69, &a5);
    v45 = v66;
    if (v66)
    {
        if ((unsigned int)v45 <= 2)
            v45 = AI_MainActor[(uint32_t)v45 - 1];
        v44 = v45;
    }
    else
    {
        v44 = 0;
    }
    if (v44)
    {
        v57 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, a5 + 0x80000000);
        v43 = v57;
        if (v57)
        {
            if ((unsigned int)v43 <= 2)
                v43 = AI_MainActor[(uint32_t)v43 - 1];
            v42 = v43;
        }
        else
        {
            v42 = 0;
        }
        if (v42)
        {
            v41 = v57;
            if (v57)
            {
                if ((unsigned int)v41 <= 2)
                    v41 = AI_MainActor[(uint32_t)v41 - 1];
                v40 = v41;
            }
            else
            {
                v40 = 0;
            }
            if (AI_EvalFunc_OBJ_FlagsIdentityTest_C(v40, 0x20))
            {
                v39 = v57;
                if (v57)
                {
                    if ((unsigned int)v39 <= 2)
                        v39 = AI_MainActor[(uint32_t)v39 - 1];
                    v38 = v39;
                }
                else
                {
                    v38 = 0;
                }
                if (AI_EvalFunc_AIIsModel_C(v38, (tAIModel*)fctvar_CV_aircraft_CV_exec_test_collision_objet_flottant))
                {
                    v70->m_msg.msgID = 37;
                    v70->m_msg.gao = AI_gpst_CurrentGameObject;
                    v70->m_msg.gao2 = 0;
                    v70->m_msg.Int1 = 103;
                    v70->m_msg.Int2 = 0;
                    v37 = v57;
                    if (v57)
                    {
                        if ((unsigned int)v37 <= 2)
                            v37 = AI_MainActor[(uint32_t)v37 - 1];
                        a2 = &v37->flags;
                    }
                    else
                    {
                        a2 = 0;
                    }
                    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)a2, &a3);
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v64);
                    MATH_SubstractVec(&dest, &a3, &v64);
                    v34 = &v70->m_msg.v1;
                    v35 = &dest;
                    v70->m_msg.v1.x = dest.x;
                    v34->y = v35->y;
                    v34->z = v35->z;
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v70->m_msg.v2);
                    v70->m_msg.f2 = 1000.0;
                    v70->m_msg.f1 = 1000.0;
                    v33 = v57;
                    if (v57)
                    {
                        if ((unsigned int)v33 <= 2)
                            v33 = AI_MainActor[(uint32_t)v33 - 1];
                        a1 = v33;
                    }
                    else
                    {
                        a1 = 0;
                    }
                    AI_EvalFunc_MSGSend_C(a1, &v70->m_msg);
                }
            }
        }
        v60 = *AI_MainActor;
        v31 = v57;
        if (v57)
        {
            if ((unsigned int)v31 <= 2)
                v31 = AI_MainActor[(uint32_t)v31 - 1];
            v30 = v31;
        }
        else
        {
            v30 = 0;
        }
        v29 = v60;
        if (v60)
        {
            if ((unsigned int)v29 <= 2)
                v29 = AI_MainActor[(uint32_t)v29 - 1];
            v28 = v29;
        }
        else
        {
            v28 = 0;
        }
        if (v30 == v28)
        {
            v49.x = 0.0;
            v49.y = 0.0;
            v49.z = 0.0;
            v27 = &v70->v_last_speed;
            v26 = &v49;
            MATH_SubstractVec(&v25, &v70->v_last_speed, &v49);
            if (!MATH_b_NulVector(&v25))
            {
                v24 = v57;
                if (v57)
                {
                    if ((unsigned int)v57 <= 2)
                        v24 = AI_MainActor[(uint32_t)v57 - 1];
                    AI_EvalFunc_OBJGetPos_C(v24, &v72);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v72);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v62);
                MATH_SubstractVec(&v53, &v72, &v62);
                v54 = v53;
                AI_EvalFunc_MATHVecSetHorzNormalize_C(&v54);
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v59);
                v48 = v54.z * v59.z + v54.y * v59.y + v59.x * v54.x;
                if (v48 > 0.86602497)
                {
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v58);
                    v67.x = 0.0;
                    v67.y = 0.0;
                    v67.z = 1.0;
                    AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v63);
                    v74 = v63.z * v54.z + v63.y * v54.y + v54.x * v63.x;
                    v65 = AI_EvalFunc_MTHFloatSign_C(v74);
                    v1 = 0.52358997f * v65;
                    AI_EvalFunc_MATHVecRotate_C(&v58, &v67, v1, &a4);
                    v54 = a4;
                }
                v56 = MATH_VecLen(&v70->v_last_speed);
                v2 = v56 * v48;
                AI_EvalFunc_MATHVecSetNormVector_C(&v54, v2);
                v23 = (tGameObjectMain*)v70->des_o_acteur_texte;
                if (v23)
                {
                    if ((unsigned int)v23 <= 2)
                        v23 = AI_MainActor[(uint32_t)v23 - 1];
                    v22 = v23;
                }
                else
                {
                    v22 = 0;
                }
                if (v22)
                {
                    if (AI_EvalFunc_TIMEElapsed_C(v70->f_time_affiche_texte, 5.0))
                    {
                        if (!SND_i_IsTrackPlaying(1040))
                        {
                            v52 = -1;
                            if ((*(uint32_t*)(CV_aircraft_CV_exec_test_collision_univVars_Perso_ID_passengers_list + 44) & 1) != 0
                                && !v70->i_last_txt_is_comp)
                            {
                                v70->i_last_txt_is_comp = 1;
                                v19 = (tAI*)v70->des_o_bison_fute;
                                if ((tAI*)v70->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
                                {
                                    v18 = WOR_gst_Universe.univAIInstance;
                                }
                                else
                                {
                                    v21 = (tGameObjectMain*)v70->des_o_bison_fute;
                                    if (v19)
                                    {
                                        if ((unsigned int)v19 <= 2)
                                            v21 = AI_MainActor[(uint32_t)v19 - 1];
                                        v20 = v21;
                                    }
                                    else
                                    {
                                        v20 = 0;
                                    }
                                    v18 = v20->extended->AIInstance;
                                }
                                v52 = ((tScriptStruct_BisonFute*)(v18->instanceVarDataCurrent))->i_last_txt_comp;
                                v52 = AI_EvalFunc_MTHModulo_C(v52 + 1, v70->des_peyj_col_nb);
                                v15 = v70->des_o_bison_fute;
                                if ((tAI*)v70->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
                                {
                                    v14 = WOR_gst_Universe.univAIInstance;
                                }
                                else
                                {
                                    v17 = (tGameObjectMain*)v70->des_o_bison_fute;
                                    if (v15)
                                    {
                                        if (v15 <= 2)
                                            v17 = AI_MainActor[v15 - 1];
                                        v16 = v17;
                                    }
                                    else
                                    {
                                        v16 = 0;
                                    }
                                    v14 = v16->extended->AIInstance;
                                }
                                ((tScriptStruct_BisonFute*)(v14->instanceVarDataCurrent))->i_last_txt_comp = v52;
                                v52 += v70->des_peyj_col_index;
                                Src.msgID = 30;
                                Src.Int1 = v52;
                                Src.Int2 = 100;
                                Src.f2 = 2.0;
                                Src.v1.x = 0.0;
                                Src.v1.y = 0.0;
                                Src.v1.z = 0.0;
                                Src.v2.x = 0.0;
                                Src.v2.y = 0.0;
                                Src.v2.z = 0.0;
                                Src.gao = 0;
                                Src.gao2 = (tGameObjectMain*)v70->o_aircraft;
                                v13 = (tGameObjectMain*)v70->des_o_acteur_texte;
                                if (v13)
                                {
                                    if ((unsigned int)v13 <= 2)
                                        v13 = AI_MainActor[(uint32_t)v13 - 1];
                                    AI_EvalFunc_MSGSend_C(v13, &Src);
                                }
                                else
                                {
                                    AI_EvalFunc_MSGSend_C(0, &Src);
                                }
                            }
                            if (v52 == -1)
                            {
                                v70->i_last_txt_is_comp = 0;
                                v10 = v70->des_o_bison_fute;
                                if ((tAI*)v70->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
                                {
                                    v9 = WOR_gst_Universe.univAIInstance;
                                }
                                else
                                {
                                    v12 = (tGameObjectMain*)v70->des_o_bison_fute;
                                    if (v10)
                                    {
                                        if (v10 <= 2)
                                            v12 = AI_MainActor[v10 - 1];
                                        v11 = v12;
                                    }
                                    else
                                    {
                                        v11 = 0;
                                    }
                                    v9 = v11->extended->AIInstance;
                                }
                                v52 = ((tScriptStruct_BisonFute*)(v9->instanceVarDataCurrent))->i_last_txt_sally;
                                v52 = AI_EvalFunc_MTHModulo_C(v52 + 1, v70->des_peyj_col_nb);
                                v6 = v70->des_o_bison_fute;
                                if ((tAI*)v70->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
                                {
                                    v5 = WOR_gst_Universe.univAIInstance;
                                }
                                else
                                {
                                    v8 = (tGameObjectMain*)v70->des_o_bison_fute;
                                    if (v6)
                                    {
                                        if (v6 <= 2)
                                            v8 = AI_MainActor[v6 - 1];
                                        v7 = v8;
                                    }
                                    else
                                    {
                                        v7 = 0;
                                    }
                                    v5 = v7->extended->AIInstance;
                                }
                                ((tScriptStruct_BisonFute*)(v5->instanceVarDataCurrent))->i_last_txt_sally = v52;
                                v52 += v70->des_sally_col_index;
                                Src.msgID = 30;
                                Src.Int1 = v52;
                                Src.Int2 = 100;
                                Src.f2 = 1.0;
                                Src.v1.x = 0.0;
                                Src.v1.y = 0.0;
                                Src.v1.z = 0.0;
                                Src.v2.x = 0.0;
                                Src.v2.y = 0.0;
                                Src.v2.z = 0.0;
                                Src.gao = 0;
                                Src.gao2 = (tGameObjectMain*)v70->o_aircraft;
                                v4 = (tGameObjectMain*)v70->des_o_acteur_texte;
                                if (v4)
                                {
                                    if ((unsigned int)v4 <= 2)
                                        v4 = AI_MainActor[(uint32_t)v4 - 1];
                                    AI_EvalFunc_MSGSend_C(v4, &Src);
                                }
                                else
                                {
                                    AI_EvalFunc_MSGSend_C(0, &Src);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 1000;
}

int CV_aircraft_CV_exec_GFX_Smoke()
{
    int result; // eax
    float v1; // [esp+0h] [ebp-D0h]
    float v2; // [esp+4h] [ebp-CCh]
    float v3; // [esp+8h] [ebp-C8h]
    tScriptStruct_CVAirCraft* v4; // [esp+Ch] [ebp-C4h]
    tGameObjectMain* v5; // [esp+1Ch] [ebp-B4h]
    float v6; // [esp+20h] [ebp-B0h]
    Vec3 v7; // [esp+24h] [ebp-ACh] BYREF
    Vec3 v8; // [esp+30h] [ebp-A0h] BYREF
    Vec3 v9; // [esp+3Ch] [ebp-94h] BYREF
    int v10; // [esp+48h] [ebp-88h]
    Vec3 v11; // [esp+50h] [ebp-80h] BYREF
    Vec3 v12; // [esp+5Ch] [ebp-74h] BYREF
    int v13; // [esp+68h] [ebp-68h]
    float v14; // [esp+6Ch] [ebp-64h]
    float v15; // [esp+70h] [ebp-60h]
    float v16; // [esp+74h] [ebp-5Ch]
    float v17; // [esp+78h] [ebp-58h]
    Vec3 v18; // [esp+7Ch] [ebp-54h] BYREF
    Vec3 v19; // [esp+88h] [ebp-48h] BYREF
    Vec3 v20; // [esp+94h] [ebp-3Ch] BYREF
    int v21; // [esp+A0h] [ebp-30h]
    float v22; // [esp+A4h] [ebp-2Ch]
    tScriptStruct_CVAirCraft* v24; // [esp+B0h] [ebp-20h]
    Vec3 a1; // [esp+B4h] [ebp-1Ch] BYREF
    Vec3 a2; // [esp+C4h] [ebp-Ch] BYREF

    v19.x = 1.0;
    v19.y = 0.0;
    v19.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_CVAirCraft*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v24 = v4;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        CV_aircraft_CV_exec_GFX_Smoke_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
    }
    if (v24->f_camera_dist > 1000000.0 || (v13 = (unsigned __int16)AI_gpst_CurrentGameObject->flags2, (v13 & 4) != 0))
    {
        if (v24->GFX_Fire_Smoke != -1)
        {
            AI_EvalFunc_GFXSeti_C_CURRENT(v24->GFX_Fire_Smoke, 13106, 50);
            v24->GFX_Fire_Smoke = -1;
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x400))
        {
            if (v24->GFX_Fire_Smoke == -1)
            {
                v24->GFX_Fire_Smoke = AI_EvalFunc_GFXAdd_C_CURRENT(13);
                if (v24->i_flag_low_res)
                {
                    GFXSetCurGlobal(v24->GFX_Fire_Smoke);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 10);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.15000001f);
                }
                else
                {
                    GFXSetCurGlobal(v24->GFX_Fire_Smoke);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 50);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.029999999f);
                }
                AI_EvalFunc_GFXSeti_C_CURRENT(v24->GFX_Fire_Smoke, 13106, 0xA43B7400);
                v5 = (tGameObjectMain*)CV_aircraft_CV_exec_GFX_Smoke_SFX_explosion_gao;
                if (CV_aircraft_CV_exec_GFX_Smoke_SFX_explosion_gao)
                {
                    if ((unsigned int)CV_aircraft_CV_exec_GFX_Smoke_SFX_explosion_gao <= 2)
                        v5 = AI_MainActor[CV_aircraft_CV_exec_GFX_Smoke_SFX_explosion_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v24->GFX_Fire_Smoke, v5, -1);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v24->GFX_Fire_Smoke, 0, -1);
                }
                GFXSetCurGlobal(v24->GFX_Fire_Smoke);
                AI_EvalFunc_GFXSeti_C_CURGLOB(13101, 7);
                AI_EvalFunc_GFXSetf_C_CURGLOB(13003, 0.5);
                AI_EvalFunc_GFXSetf_C_CURGLOB(13004, 1.5);
                AI_EvalFunc_GFXSeti_C_CURGLOB(13107, 1);
                AI_EvalFunc_GFXSetf_C_CURGLOB(13000, 0.1f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(13001, 0.40000001f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(13002, 0.0049999999f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(13005, 2.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(13006, 4.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(13009, 10.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(13010, 20.0);
                v9.x = -1.0;
                v9.y = -1.0;
                v9.z = 1.0;
                AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v9);
                v8.x = 1.0;
                v8.y = 1.0;
                v8.z = 1.0;
                AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v8);
                v18.x = 0.1f;
                v18.y = 0.1f;
                v18.z = 0.0;
                AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v18);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v24->GFX_Fire_Smoke, 0, 1);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v24->GFX_Fire_Smoke, 2, 1);
                AI_EvalFunc_GFXSetf_C_CURRENT(v24->GFX_Fire_Smoke, 13012, 0.75);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
            if (v24->f_water_Z - 5.0 >= a1.z)
            {
                v22 = TIM_gf_MainClock - TIM_gf_LockTime;
                v6 = v22 - v24->f_time_black_smoke;
                if (v6 > 1.0)
                    v3 = 1.0;
                else
                    v3 = v22 - v24->f_time_black_smoke;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v11);
                if (v24->f_water_Z - 35.0 > v11.z)
                    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 1024);
                v21 = AI_EvalFunc_COLORBlend_C(4800, 0x20FFFFFF, v3);
                GFXSetCurGlobal(v24->GFX_Fire_Smoke);
                AI_EvalFunc_GFXSeti_C_CURGLOB(13103, v21);
                v10 = AI_EvalFunc_COLORBlend_C(0x80000020, 0x20FFFFFF, v3);
                AI_EvalFunc_GFXSeti_C_CURGLOB(13104, v10);
                AI_EvalFunc_GFXSeti_C_CURGLOB(13105, 0xFFFFFF);
            }
            else
            {
                v24->f_time_black_smoke = TIM_gf_MainClock - TIM_gf_LockTime;
                GFXSetCurGlobal(v24->GFX_Fire_Smoke);
                AI_EvalFunc_GFXSeti_C_CURGLOB(13103, 0x400012C0);
                AI_EvalFunc_GFXSeti_C_CURGLOB(13104, 0x80000020);
                AI_EvalFunc_GFXSeti_C_CURGLOB(13105, 0);
            }
            AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&a2);
            v20.x = a2.x * 0.75f;
            v20.y = a2.y * 0.75f;
            v20.z = a2.z * 0.75f;
            GFXSetCurGlobal(v24->GFX_Fire_Smoke);
            AI_EvalFunc_GFXSetv_C_CURGLOB(13204, &v20);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v12);
            v19 = v12;
            v2 = v24->des_f_Y_max_offset * 0.75f;
            v1 = v24->des_f_Y_min_offset * 0.25f;
            v17 = AI_EvalFunc_MATHRandFloat_C(v1, v2);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v7);
            v14 = v7.x * v17;
            v15 = v7.y * v17;
            v16 = v7.z * v17;
            v19.x = v19.x + v14;
            v19.y = v19.y + v15;
            v19.z = v19.z + v16;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13200, &v19);
        }
        else if (v24->GFX_Fire_Smoke != -1)
        {
            AI_EvalFunc_GFXSeti_C_CURRENT(v24->GFX_Fire_Smoke, 13106, 50);
            v24->GFX_Fire_Smoke = -1;
        }
        result = 1000;
    }
    return result;
}

int CV_aircraft_CV_exec_check_pos()
{
    int result; // eax
    double v1; // st7
    int v2; // eax
    int v3; // eax
    double v4; // st7
    float v5; // [esp+8h] [ebp-38Ch]
    float v6; // [esp+10h] [ebp-384h]
    float v7; // [esp+14h] [ebp-380h]
    float v8; // [esp+18h] [ebp-37Ch]
    float v9; // [esp+1Ch] [ebp-378h]
    float v10; // [esp+20h] [ebp-374h]
    float v11; // [esp+24h] [ebp-370h]
    float v12; // [esp+28h] [ebp-36Ch]
    float v13; // [esp+2Ch] [ebp-368h]
    float v14; // [esp+30h] [ebp-364h]
    tScriptStruct_CVAirCraft* v15; // [esp+34h] [ebp-360h]
    float v16; // [esp+38h] [ebp-35Ch]
    float v17; // [esp+3Ch] [ebp-358h]
    tGameObjectMain* v18; // [esp+44h] [ebp-350h]
    tAI* v19; // [esp+50h] [ebp-344h]
    tGameObjectMain* v20; // [esp+54h] [ebp-340h]
    tGameObjectMain* v21; // [esp+58h] [ebp-33Ch]
    tGameObjectMain* v22; // [esp+60h] [ebp-334h]
    float v23; // [esp+68h] [ebp-32Ch]
    tGameObjectMain* v24; // [esp+78h] [ebp-31Ch]
    tGameObjectMain* v25; // [esp+8Ch] [ebp-308h]
    tAI* v26; // [esp+98h] [ebp-2FCh]
    tGameObjectMain* v27; // [esp+9Ch] [ebp-2F8h]
    tGameObjectMain* v28; // [esp+A0h] [ebp-2F4h]
    float v29; // [esp+A4h] [ebp-2F0h]
    tAI* v30; // [esp+B0h] [ebp-2E4h]
    tGameObjectMain* v31; // [esp+B4h] [ebp-2E0h]
    tGameObjectMain* v32; // [esp+B8h] [ebp-2DCh]
    tAI* v33; // [esp+CCh] [ebp-2C8h]
    tGameObjectMain* v34; // [esp+D0h] [ebp-2C4h]
    tGameObjectMain* v35; // [esp+D4h] [ebp-2C0h]
    tGameObjectMain* v36; // [esp+E4h] [ebp-2B0h]
    tGameObjectMain* v37; // [esp+ECh] [ebp-2A8h]
    tGameObjectMain* v38; // [esp+FCh] [ebp-298h]
    tAI* v39; // [esp+110h] [ebp-284h]
    tGameObjectMain* v40; // [esp+114h] [ebp-280h]
    tGameObjectMain* v41; // [esp+118h] [ebp-27Ch]
    float v42; // [esp+11Ch] [ebp-278h]
    tGameObjectMain* v43; // [esp+124h] [ebp-270h]
    tGameObjectMain* v44; // [esp+134h] [ebp-260h]
    tAI* v45; // [esp+140h] [ebp-254h]
    tGameObjectMain* v46; // [esp+144h] [ebp-250h]
    tGameObjectMain* v47; // [esp+148h] [ebp-24Ch]
    float v48; // [esp+150h] [ebp-244h]
    tGameObjectMain* v49; // [esp+160h] [ebp-234h]
    tGameObjectMain* v50; // [esp+164h] [ebp-230h]
    tGameObjectMain* v51; // [esp+168h] [ebp-22Ch]
    tGameObjectMain* v52; // [esp+16Ch] [ebp-228h]
    tGameObjectMain* v53; // [esp+170h] [ebp-224h]
    tAI* v54; // [esp+174h] [ebp-220h]
    tGameObjectMain* v55; // [esp+178h] [ebp-21Ch]
    tGameObjectMain* v56; // [esp+17Ch] [ebp-218h]
    tAI* v57; // [esp+180h] [ebp-214h]
    tGameObjectMain* v58; // [esp+184h] [ebp-210h]
    tGameObjectMain* v59; // [esp+188h] [ebp-20Ch]
    tAI* v60; // [esp+18Ch] [ebp-208h]
    unsigned int v61; // [esp+190h] [ebp-204h]
    tGameObjectMain* v62; // [esp+194h] [ebp-200h]
    tGameObjectMain* v63; // [esp+198h] [ebp-1FCh]
    tAI* v64; // [esp+19Ch] [ebp-1F8h]
    unsigned int v65; // [esp+1A0h] [ebp-1F4h]
    tGameObjectMain* v66; // [esp+1A4h] [ebp-1F0h]
    tGameObjectMain* v67; // [esp+1A8h] [ebp-1ECh]
    float v68; // [esp+1ACh] [ebp-1E8h]
    float v69; // [esp+1B0h] [ebp-1E4h]
    Vec3 v70; // [esp+1B4h] [ebp-1E0h] BYREF
    Vec3 v71; // [esp+1C0h] [ebp-1D4h] BYREF
    Vec3 v72; // [esp+1D0h] [ebp-1C4h] BYREF
    tGameObjectMain* v73; // [esp+1DCh] [ebp-1B8h]
    Vec3 v74; // [esp+1E4h] [ebp-1B0h] BYREF
    Vec3 v75; // [esp+1F4h] [ebp-1A0h] BYREF
    Vec3 v76; // [esp+200h] [ebp-194h] BYREF
    float v77; // [esp+20Ch] [ebp-188h]
    int v78; // [esp+214h] [ebp-180h]
    float v79; // [esp+21Ch] [ebp-178h]
    Vec3 v80; // [esp+220h] [ebp-174h] BYREF
    Vec3 v81; // [esp+22Ch] [ebp-168h] BYREF
    float v82; // [esp+238h] [ebp-15Ch]
    int v83; // [esp+240h] [ebp-154h]
    float v84; // [esp+244h] [ebp-150h]
    float v85; // [esp+24Ch] [ebp-148h]
    float v86; // [esp+250h] [ebp-144h]
    float v87; // [esp+254h] [ebp-140h]
    float v88; // [esp+258h] [ebp-13Ch]
    Vec3 v89; // [esp+25Ch] [ebp-138h]
    Vec3 v90; // [esp+268h] [ebp-12Ch] BYREF
    float v91; // [esp+27Ch] [ebp-118h]
    Vec3 v92; // [esp+280h] [ebp-114h] BYREF
    Vec3 v93; // [esp+28Ch] [ebp-108h] BYREF
    float v94; // [esp+298h] [ebp-FCh]
    float v95; // [esp+2A0h] [ebp-F4h]
    Vec3 v96; // [esp+2A4h] [ebp-F0h] BYREF
    float v97; // [esp+2B0h] [ebp-E4h]
    float v98; // [esp+2BCh] [ebp-D8h]
    Vec3 a3; // [esp+2C0h] [ebp-D4h] BYREF
    Vec3 v100; // [esp+2D4h] [ebp-C0h]
    Vec3 v101; // [esp+2E0h] [ebp-B4h] BYREF
    float v102; // [esp+2F0h] [ebp-A4h]
    float v103; // [esp+2F4h] [ebp-A0h]
    tGameObjectMain* v104; // [esp+2F8h] [ebp-9Ch]
    tScriptStruct_CVAirCraft* v105; // [esp+330h] [ebp-64h]
    float v106; // [esp+334h] [ebp-60h]
    Vec3 v107; // [esp+338h] [ebp-5Ch] BYREF
    Vec3 a1; // [esp+344h] [ebp-50h] BYREF
    Vec3 v109; // [esp+350h] [ebp-44h] BYREF
    Vec3 v110; // [esp+35Ch] [ebp-38h] BYREF
    float v111; // [esp+36Ch] [ebp-28h]
    Vec3 dest; // [esp+370h] [ebp-24h] BYREF
    Vec3 v113; // [esp+37Ch] [ebp-18h] BYREF
    float v114; // [esp+38Ch] [ebp-8h]
    float v115; // [esp+390h] [ebp-4h]

    v92.x = 1.0;
    v92.y = 0.0;
    v92.z = 0.0;
    v89.x = 1.0;
    v89.y = 0.0;
    v89.z = 0.0;
    v100.x = 1.0;
    v100.y = 0.0;
    v100.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v15 = (tScriptStruct_CVAirCraft*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v15 = 0;
    v105 = v15;
    v73 = 0;
    v85 = 0.0;
    v82 = 0.0;
    v87 = 0.0;
    v111 = 0.0;
    v83 = 0;
    v78 = 0;
    v84 = 0.0;
    v88 = 0.0;
    v15->f_speed_target = v15->des_f_speed;
    AI_EvalFunc_DYNSpeedLimitSet_C(AI_gpst_CurrentGameObject, 1.0e14f);
    if (v105->des_i_quartier_pietons)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        v68 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
        v78 = v105->i_last_check++;
        v65 = v105->des_o_bison_fute;
        if ((tAI*)v105->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
        {
            v64 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v67 = (tGameObjectMain*)v105->des_o_bison_fute;
            if (v65)
            {
                if (v65 <= 2)
                    v67 = AI_MainActor[v65 - 1];
                v66 = v67;
            }
            else
            {
                v66 = 0;
            }
            v64 = v66->extended->AIInstance;
        }
        if (v105->i_last_check == ((tScriptStruct_BisonFute*)(v64->instanceVarDataCurrent))->i_nb_aircraft)
            v105->i_last_check = 0;
        v61 = v105->des_o_bison_fute;
        if ((tAI*)v105->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
        {
            v60 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v63 = (tGameObjectMain*)v105->des_o_bison_fute;
            if (v61)
            {
                if (v61 <= 2)
                    v63 = AI_MainActor[v61 - 1];
                v62 = v63;
            }
            else
            {
                v62 = 0;
            }
            v60 = v62->extended->AIInstance;
        }
        v73 = (tGameObjectMain*)((tScriptStruct_BisonFute*)(v60->instanceVarDataCurrent))->ao_aircraft[v78];
        v83 = 1;
        if (v105->des_mode == 1)
        {
            if (v73 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
            {
                v57 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v59 = v73;
                if (v73)
                {
                    if ((unsigned int)v73 <= 2)
                        v59 = AI_MainActor[(uint32_t)v73 - 1];
                    v58 = v59;
                }
                else
                {
                    v58 = 0;
                }
                v57 = v58->extended->AIInstance;
            }
            if (v105->des_mode == ((tScriptStruct_CVAirCraft*)(v57->instanceVarDataCurrent))->des_mode)
            {
                if (v73 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v54 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v56 = v73;
                    if (v73)
                    {
                        if ((unsigned int)v73 <= 2)
                            v56 = AI_MainActor[(uint32_t)v73 - 1];
                        v55 = v56;
                    }
                    else
                    {
                        v55 = 0;
                    }
                    v54 = v55->extended->AIInstance;
                }
                if (v105->i_limo_categ != ((tScriptStruct_CVAirCraft*)(v54->instanceVarDataCurrent))->i_limo_categ)
                    v83 = 0;
            }
        }
        if (v83)
        {
            v104 = AI_gpst_CurrentGameObject;
            v53 = v73;
            if (v73)
            {
                if ((unsigned int)v73 <= 2)
                    v53 = AI_MainActor[(uint32_t)v73 - 1];
                v52 = v53;
            }
            else
            {
                v52 = 0;
            }
            v51 = v104;
            if (v104)
            {
                if ((unsigned int)v104 <= 2)
                    v51 = AI_MainActor[(uint32_t)v104 - 1];
                v50 = v51;
            }
            else
            {
                v50 = 0;
            }
            if (v52 != v50)
            {
                v49 = v73;
                if (v73)
                {
                    if ((unsigned int)v73 <= 2)
                        v49 = AI_MainActor[(uint32_t)v73 - 1];
                    AI_EvalFunc_OBJGetPos_C(v49, &a3);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &a3);
                }
                v92 = a3;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v72);
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v71);
                v48 = v105->des_f_Y_max_offset;
                v93.x = v71.x * v48;
                v93.y = v71.y * v48;
                v93.z = v71.z * v48;
                MATH_VecAddVec(&a1, &v72, &v93);
                v92.x = v92.x - a1.x;
                v92.y = v92.y - a1.y;
                v92.z = v92.z - a1.z;
                v88 = MATH_VecLen(&v92);
                if (v73 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v45 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v47 = v73;
                    if (v73)
                    {
                        if ((unsigned int)v73 <= 2)
                            v47 = AI_MainActor[(uint32_t)v73 - 1];
                        v46 = v47;
                    }
                    else
                    {
                        v46 = 0;
                    }
                    v45 = v46->extended->AIInstance;
                }
                if (v105->f_largeur_test < (double)((tScriptStruct_CVAirCraft*)(v45->instanceVarDataCurrent))->f_largeur_test)
                    v14 = ((tScriptStruct_CVAirCraft*)(v45->instanceVarDataCurrent))->f_largeur_test;
                else
                    v14 = v105->f_largeur_test;
                v69 = v105->f_brake_dist + v14;
                if (v105->des_f_Y_max_offset + v69 > v88)
                {
                    v44 = v73;
                    if (v73)
                    {
                        if ((unsigned int)v73 <= 2)
                            v44 = AI_MainActor[(uint32_t)v73 - 1];
                        AI_EvalFunc_OBJ_SightGet_C(v44, &v107);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_SightGet_C(0, &v107);
                    }
                    v100 = v107;
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v80);
                    v82 = v92.z * v80.z + v92.y * v80.y + v80.x * v92.x;
                    if (v82 > 0.0)
                    {
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&v74);
                        v43 = v73;
                        if (v73)
                        {
                            if ((unsigned int)v73 <= 2)
                                v43 = AI_MainActor[(uint32_t)v73 - 1];
                            AI_EvalFunc_OBJGetPos_C(v43, &v113);
                        }
                        else
                        {
                            AI_EvalFunc_OBJGetPos_C(0, &v113);
                        }
                        v13 = (float)fabs(v74.z - v113.z);
                        v98 = v13;
                        if (v13 < 4.0)
                        {
                            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v81);
                            v91 = v81.z * v92.z + v81.y * v92.y + v92.x * v81.x;
                            v87 = -v91;
                            v42 = 1.0f / v88;
                            v92.x = v92.x * v42;
                            v92.y = v92.y * v42;
                            v92.z = v92.z * v42;
                            if (v73 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                            {
                                v39 = WOR_gst_Universe.univAIInstance;
                            }
                            else
                            {
                                v41 = v73;
                                if (v73)
                                {
                                    if ((unsigned int)v73 <= 2)
                                        v41 = AI_MainActor[(uint32_t)v73 - 1];
                                    v40 = v41;
                                }
                                else
                                {
                                    v40 = 0;
                                }
                                v39 = v40->extended->AIInstance;
                            }
                            v100 = ((tScriptStruct_CVAirCraft*)(v39->instanceVarDataCurrent))->v_new_sight;
                            v84 = v100.z * v105->v_new_sight.z + v100.y * v105->v_new_sight.y + v100.x * v105->v_new_sight.x;
                            v38 = v73;
                            if (v73)
                            {
                                if ((unsigned int)v73 <= 2)
                                    v38 = AI_MainActor[(uint32_t)v73 - 1];
                                AI_EvalFunc_DYNSpeedGetVector_C(v38, &v90);
                            }
                            else
                            {
                                AI_EvalFunc_DYNSpeedGetVector_C(0, &v90);
                            }
                            v89 = v90;
                            v37 = v73;
                            if (v73)
                            {
                                if ((unsigned int)v73 <= 2)
                                    v37 = AI_MainActor[(uint32_t)v73 - 1];
                                v1 = AI_EvalFunc_DYNSpeedGet_C(v37);
                            }
                            else
                            {
                                v1 = AI_EvalFunc_DYNSpeedGet_C(0);
                            }
                            v103 = (float)v1;
                            if (v103 < 1.0)
                            {
                                v36 = v73;
                                if (v73)
                                {
                                    if ((unsigned int)v73 <= 2)
                                        v36 = AI_MainActor[(uint32_t)v73 - 1];
                                    AI_EvalFunc_OBJ_SightGet_C(v36, &v96);
                                }
                                else
                                {
                                    AI_EvalFunc_OBJ_SightGet_C(0, &v96);
                                }
                                v89 = v96;
                            }
                            if (v73 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                            {
                                v33 = WOR_gst_Universe.univAIInstance;
                            }
                            else
                            {
                                v35 = v73;
                                if (v73)
                                {
                                    if ((unsigned int)v73 <= 2)
                                        v35 = AI_MainActor[(uint32_t)v73 - 1];
                                    v34 = v35;
                                }
                                else
                                {
                                    v34 = 0;
                                }
                                v33 = v34->extended->AIInstance;
                            }
                            if (v105->f_largeur_test < (double)((tScriptStruct_CVAirCraft*)(v33->instanceVarDataCurrent))->f_largeur_test)
                                v12 = ((tScriptStruct_CVAirCraft*)(v33->instanceVarDataCurrent))->f_largeur_test;
                            else
                                v12 = v105->f_largeur_test;
                            v85 = v92.z * v89.z + v92.y * v89.y + v89.x * v92.x;
                            if (v85 <= 10.0
                                || (v105->f_vitesse < (double)v105->f_brake_dist ? (v11 = v105->f_brake_dist) : (v11 = v105->f_vitesse),
                                    (v86 = v11, v82 >= (double)v11) || v84 <= 0.0 || v87 <= -100.0 || v87 >= 0.0))
                            {
                                v10 = (float)fabs(v87);
                                v102 = v10;
                                if (v10 < (double)v12)
                                {
                                    if (v73 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                                    {
                                        v30 = WOR_gst_Universe.univAIInstance;
                                    }
                                    else
                                    {
                                        v32 = v73;
                                        if (v73)
                                        {
                                            if ((unsigned int)v73 <= 2)
                                                v32 = AI_MainActor[(uint32_t)v73 - 1];
                                            v31 = v32;
                                        }
                                        else
                                        {
                                            v31 = 0;
                                        }
                                        v30 = v31->extended->AIInstance;
                                    }
                                    if (v105->f_brake_dist < (double)((tScriptStruct_CVAirCraft*)(v30->instanceVarDataCurrent))->f_brake_dist)
                                        v9 = ((tScriptStruct_CVAirCraft*)(v30->instanceVarDataCurrent))->f_brake_dist;
                                    else
                                        v9 = v105->f_brake_dist;
                                    v111 = v9;
                                    if (v82 >= (double)v9)
                                    {
                                        if (v84 > 0.70710701 && v105->f_vitesse > (double)v82)
                                        {
                                            if (v73 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                                            {
                                                v26 = WOR_gst_Universe.univAIInstance;
                                            }
                                            else
                                            {
                                                v28 = v73;
                                                if (v73)
                                                {
                                                    if ((unsigned int)v73 <= 2)
                                                        v28 = AI_MainActor[(uint32_t)v73 - 1];
                                                    v27 = v28;
                                                }
                                                else
                                                {
                                                    v27 = 0;
                                                }
                                                v26 = v27->extended->AIInstance;
                                            }
                                            if (v105->f_vitesse > (double)((tScriptStruct_CVAirCraft*)(v26->instanceVarDataCurrent))->f_speed_target)
                                                v8 = ((tScriptStruct_CVAirCraft*)(v26->instanceVarDataCurrent))->f_speed_target;
                                            else
                                                v8 = v105->f_vitesse;
                                            v105->f_speed_target = v8;
                                        }
                                    }
                                    else
                                    {
                                        v105->f_speed_target = 0.0;
                                        v3 = AI_EvalFunc_MTHModulo_C(v105->i_limo_categ + 1, 2);
                                        v105->i_limo_categ = v3;
                                        v95 = AI_EvalFunc_DYNSpeedLimitGet_C(AI_gpst_CurrentGameObject);
                                        v29 = v82 - 10.0f;
                                        if (v29 <= (double)v95)
                                        {
                                            if (v29 >= 0.0)
                                                v106 = v82 - 10.0f;
                                            else
                                                v106 = 0.0;
                                        }
                                        else
                                        {
                                            v106 = v95;
                                        }
                                        AI_EvalFunc_DYNSpeedLimitSet_C(AI_gpst_CurrentGameObject, v106);
                                    }
                                }
                            }
                            else
                            {
                                v105->f_speed_target = 0.0;
                                v2 = AI_EvalFunc_MTHModulo_C(v105->i_limo_categ + 1, 2);
                                v105->i_limo_categ = v2;
                            }
                        }
                    }
                }
            }
        }
        if (!v105->des_ignore_player_aircraft)
        {
            v73 = *AI_MainActor;
            v25 = v73;
            if (v73)
            {
                if ((unsigned int)v73 <= 2)
                    v25 = AI_MainActor[(uint32_t)v73 - 1];
                AI_EvalFunc_OBJGetPos_C(v25, &v110);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v110);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v75);
            MATH_SubstractVec(&dest, &v110, &v75);
            v92 = dest;
            v88 = MATH_VecLen(&v92);
            if (v88 / v68 < 0.5)
            {
                v24 = v73;
                if (v73)
                {
                    if ((unsigned int)v73 <= 2)
                        v24 = AI_MainActor[(uint32_t)v73 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v24, &v70);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v70);
                }
                v100 = v70;
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v76);
                v82 = v92.z * v76.z + v92.y * v76.y + v76.x * v92.x;
                if (v82 > 5.0)
                {
                    AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v109);
                    v97 = v109.z * v92.z + v109.y * v92.y + v92.x * v109.x;
                    v87 = -v97;
                    v23 = 1.0f / v88;
                    v92.x = v92.x * v23;
                    v92.y = v92.y * v23;
                    v92.z = v92.z * v23;
                    v84 = v100.z * v105->v_new_sight.z + v100.y * v105->v_new_sight.y + v100.x * v105->v_new_sight.x;
                    v22 = v73;
                    if (v73)
                    {
                        if ((unsigned int)v73 <= 2)
                            v22 = AI_MainActor[(uint32_t)v73 - 1];
                        AI_EvalFunc_DYNSpeedGetVector_C(v22, &v101);
                    }
                    else
                    {
                        AI_EvalFunc_DYNSpeedGetVector_C(0, &v101);
                    }
                    v85 = v92.z * v101.z + v92.y * v101.y + v101.x * v92.x;
                    if (v85 <= 20.0 || v105->f_vitesse <= (double)v82 || v84 <= 0.0 || v87 <= -100.0 || v87 >= 0.0)
                    {
                        v7 = (float)fabs(v87);
                        v114 = v7;
                        if (v105->f_largeur_test > (double)v7)
                        {
                            if (v73 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                            {
                                v19 = WOR_gst_Universe.univAIInstance;
                            }
                            else
                            {
                                v21 = v73;
                                if (v73)
                                {
                                    if ((unsigned int)v73 <= 2)
                                        v21 = AI_MainActor[(uint32_t)v73 - 1];
                                    v20 = v21;
                                }
                                else
                                {
                                    v20 = 0;
                                }
                                v19 = v20->extended->AIInstance;
                            }
                            if (v105->f_brake_dist < (double)((tScriptStruct_CVAirCraft*)(v19->instanceVarDataCurrent))->f_brake_dist)
                                v6 = ((tScriptStruct_CVAirCraft*)(v19->instanceVarDataCurrent))->f_brake_dist;
                            else
                                v6 = v105->f_brake_dist;
                            v111 = v6;
                            if (v82 >= (double)v6)
                            {
                                if (v84 > 0.70710701 && v105->f_vitesse > (double)v82)
                                {
                                    v18 = v73;
                                    if (v73)
                                    {
                                        if ((unsigned int)v73 <= 2)
                                            v18 = AI_MainActor[(uint32_t)v73 - 1];
                                        v4 = AI_EvalFunc_DYNSpeedGet_C(v18);
                                    }
                                    else
                                    {
                                        v4 = AI_EvalFunc_DYNSpeedGet_C(0);
                                    }
                                    v105->f_speed_target = (float)v4;
                                }
                            }
                            else
                            {
                                v105->f_speed_target = 0.0;
                                if (!v105->i_snd_klaxon)
                                {
                                    v105->i_snd_klaxon = 1;
                                    v105->f_snd_time_bip = TIM_gf_MainClock - TIM_gf_LockTime;
                                }
                            }
                        }
                    }
                    else
                    {
                        v105->f_speed_target = 0.0;
                        if (!v105->i_snd_klaxon)
                        {
                            v105->i_snd_klaxon = 1;
                            v105->f_snd_time_bip = TIM_gf_MainClock - TIM_gf_LockTime;
                        }
                    }
                }
            }
        }
        if (v105->f_vitesse < (double)v105->f_speed_target)
        {
            v94 = TIM_gf_dt;
            v5 = (float)(4 - v105->des_type);
            v16 = v5 * TIM_gf_dt;
            if (v16 < 0.0)
                v16 = 0.0;
            if (v16 > 1.0)
                v16 = 1.0;
            v77 = v105->f_speed_target * v16 + v105->f_vitesse * (1.0f - v16);
            v105->f_vitesse = v77;
        }
        else
        {
            v115 = TIM_gf_dt;
            v17 = 3.0f * TIM_gf_dt;
            if (v17 < 0.0)
                v17 = 0.0;
            if (v17 > 1.0)
                v17 = 1.0;
            v79 = v105->f_speed_target * v17 + v105->f_vitesse * (1.0f - v17);
            v105->f_vitesse = v79;
        }
        result = 1000;
    }
    return result;
}

int CV_aircraft_CV_exec_GFX_Flammes()
{
    int result; // eax
    BOOL v1; // eax
    float v2; // [esp+0h] [ebp-19Ch]
    float v3; // [esp+4h] [ebp-198h]
    tScriptStruct_CVAirCraft* v4; // [esp+8h] [ebp-194h]
    tGameObjectMain* v5; // [esp+20h] [ebp-17Ch]
    tGameObjectMain* v6; // [esp+28h] [ebp-174h]
    tAI* v7; // [esp+2Ch] [ebp-170h]
    unsigned int v8; // [esp+30h] [ebp-16Ch]
    tGameObjectMain* v9; // [esp+34h] [ebp-168h]
    tGameObjectMain* v10; // [esp+38h] [ebp-164h]
    tAI* v11; // [esp+3Ch] [ebp-160h]
    tAI* v12; // [esp+40h] [ebp-15Ch]
    tGameObjectMain* v13; // [esp+44h] [ebp-158h]
    tGameObjectMain* v14; // [esp+48h] [ebp-154h]
    tGameObjectMain* v15; // [esp+4Ch] [ebp-150h]
    tGameObjectMain* v16; // [esp+50h] [ebp-14Ch]
    tGameObjectMain* v17; // [esp+58h] [ebp-144h]
    tGameObjectMain* v18; // [esp+5Ch] [ebp-140h]
    tGameObjectMain* v19; // [esp+60h] [ebp-13Ch]
    Vec3 v20; // [esp+64h] [ebp-138h] BYREF
    float v21; // [esp+70h] [ebp-12Ch]
    Vec3 a3; // [esp+74h] [ebp-128h] BYREF
    BOOL v23; // [esp+80h] [ebp-11Ch]
    Vec3 v24; // [esp+84h] [ebp-118h] BYREF
    Vec3 a2; // [esp+A0h] [ebp-FCh] BYREF
    int v27; // [esp+ACh] [ebp-F0h]
    float v28; // [esp+B0h] [ebp-ECh]
    float v29; // [esp+B4h] [ebp-E8h]
    float v30; // [esp+B8h] [ebp-E4h]
    Vec3 v31; // [esp+BCh] [ebp-E0h] BYREF
    Vec3 v32; // [esp+C8h] [ebp-D4h]
    float v33; // [esp+D4h] [ebp-C8h]
    BOOL v34; // [esp+DCh] [ebp-C0h]
    Vec3 v35; // [esp+E0h] [ebp-BCh] BYREF
    Vec3 v36; // [esp+ECh] [ebp-B0h] BYREF
    float v37; // [esp+F8h] [ebp-A4h]
    float v38; // [esp+FCh] [ebp-A0h]
    float v39; // [esp+100h] [ebp-9Ch]
    float v40; // [esp+10Ch] [ebp-90h]
    Vec3 v41; // [esp+114h] [ebp-88h] BYREF
    float v42; // [esp+120h] [ebp-7Ch]
    float v43; // [esp+124h] [ebp-78h]
    tScriptStruct_CVAirCraft* v44; // [esp+12Ch] [ebp-70h]
    int v45; // [esp+130h] [ebp-6Ch]
    Vec3 dest; // [esp+140h] [ebp-5Ch] BYREF
    Vec3 v47; // [esp+150h] [ebp-4Ch] BYREF
    Vec3 v48; // [esp+15Ch] [ebp-40h] BYREF
    tMessage Src; // [esp+168h] [ebp-34h] BYREF

    v41.x = 1.0;
    v41.y = 0.0;
    v41.z = 0.0;
    v36.x = 1.0;
    v36.y = 0.0;
    v36.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_CVAirCraft*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v44 = v4;
    v45 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        CV_aircraft_CV_exec_GFX_Flammes_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
    }
    if (v44->f_camera_dist > 1000000.0 || (v27 = (unsigned __int16)AI_gpst_CurrentGameObject->flags2, (v27 & 4) != 0))
    {
        if (v44->GFX_Flammes != -1)
        {
            AI_EvalFunc_GFXSeti_C_CURRENT(v44->GFX_Flammes, 13106, 50);
            v44->GFX_Flammes = -1;
        }
        if (!AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x800))
            v44->f_time_no_fire = TIM_gf_MainClock - TIM_gf_LockTime;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        v23 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x800);
        if (v23 && (AI_EvalFunc_OBJGetPos_C_CURRENT(&v20), v44->f_water_Z - 5.0 < v20.z))
        {
            if (v44->f_camera_dist < 40000.0)
            {
                v19 = (tGameObjectMain*)v44->des_o_acteur_texte;
                if (v19)
                {
                    if ((unsigned int)v19 <= 2)
                        v19 = AI_MainActor[(uint32_t)v19 - 1];
                    v18 = v19;
                }
                else
                {
                    v18 = 0;
                }
                if (v18)
                {
                    v17 = (tGameObjectMain*)v44->des_o_acteur_texte;
                    if (v17)
                    {
                        if ((unsigned int)v17 <= 2)
                            v17 = AI_MainActor[(uint32_t)v17 - 1];
                        v1 = AI_EvalFunc_OBJ_CapaTest_C(v17, 256);
                    }
                    else
                    {
                        v1 = AI_EvalFunc_OBJ_CapaTest_C(0, 256);
                    }
                    v34 = v1;
                    if (v1)
                        v44->f_time_affiche_texte = TIM_gf_MainClock - TIM_gf_LockTime;
                }
                v16 = (tGameObjectMain*)v44->des_o_acteur_texte;
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
                    if (AI_EvalFunc_TIMEElapsed_C(v44->f_time_affiche_texte, 4.0))
                    {
                        v12 = (tAI*)v44->des_o_bison_fute;
                        if ((tAI*)v44->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
                        {
                            v11 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v14 = (tGameObjectMain*)v44->des_o_bison_fute;
                            if (v12)
                            {
                                if ((unsigned int)v12 <= 2)
                                    v14 = AI_MainActor[(uint32_t)v12 - 1];
                                v13 = v14;
                            }
                            else
                            {
                                v13 = 0;
                            }
                            v11 = v13->extended->AIInstance;
                        }
                        v45 = ((tScriptStruct_BisonFute*)(v11->instanceVarDataCurrent))->i_last_sos_txt;
                        v45 = AI_EvalFunc_MTHModulo_C(v45 + 1, v44->des_SOS_nb);
                        v8 = v44->des_o_bison_fute;
                        if ((tAI*)v44->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
                        {
                            v7 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v10 = (tGameObjectMain*)v44->des_o_bison_fute;
                            if (v8)
                            {
                                if (v8 <= 2)
                                    v10 = AI_MainActor[v8 - 1];
                                v9 = v10;
                            }
                            else
                            {
                                v9 = 0;
                            }
                            v7 = v9->extended->AIInstance;
                        }
                        ((tScriptStruct_BisonFute*)(v7->instanceVarDataCurrent))->i_last_sos_txt = v45;
                        v45 += v44->des_SOS_index;
                        Src.msgID = 30;
                        Src.Int1 = v45;
                        Src.Int2 = 100;
                        Src.f2 = 2.0;
                        Src.v1.x = 0.0;
                        Src.v1.y = 0.0;
                        Src.v1.z = 0.0;
                        Src.v2.x = 0.0;
                        Src.v2.y = 0.0;
                        Src.v2.z = 0.0;
                        Src.gao = 0;
                        Src.gao2 = 0;
                        v6 = (tGameObjectMain*)v44->des_o_acteur_texte;
                        if (v6)
                        {
                            if ((unsigned int)v6 <= 2)
                                v6 = AI_MainActor[(uint32_t)v6 - 1];
                            AI_EvalFunc_MSGSend_C(v6, &Src);
                        }
                        else
                        {
                            AI_EvalFunc_MSGSend_C(0, &Src);
                        }
                    }
                }
            }
            if (v44->GFX_Flammes == -1)
            {
                v44->GFX_Flammes = AI_EvalFunc_GFXAdd_C_CURRENT(13);
                if (v44->i_flag_low_res)
                {
                    GFXSetCurGlobal(v44->GFX_Flammes);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 10);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.050000001f);
                }
                else
                {
                    GFXSetCurGlobal(v44->GFX_Flammes);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 50);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.0099999998f);
                }
                AI_EvalFunc_GFXSeti_C_CURRENT(v44->GFX_Flammes, 13106, 0xA43B7400);
                v5 = (tGameObjectMain*)CV_aircraft_CV_exec_GFX_Flammes_SFX_explosion_gao;
                if (CV_aircraft_CV_exec_GFX_Flammes_SFX_explosion_gao)
                {
                    if ((unsigned int)CV_aircraft_CV_exec_GFX_Flammes_SFX_explosion_gao <= 2)
                        v5 = AI_MainActor[CV_aircraft_CV_exec_GFX_Flammes_SFX_explosion_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v44->GFX_Flammes, v5, -1);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v44->GFX_Flammes, 0, -1);
                }
                GFXSetCurGlobal(v44->GFX_Flammes);
                AI_EvalFunc_GFXSeti_C_CURGLOB(13101, 8);
                AI_EvalFunc_GFXSeti_C_CURGLOB(13110, 9);
                AI_EvalFunc_GFXSeti_C_CURGLOB(13111, 10);
                AI_EvalFunc_GFXSeti_C_CURGLOB(13112, 8);
                AI_EvalFunc_GFXSetf_C_CURGLOB(13003, 0.15000001f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(13004, 0.44999999f);
                AI_EvalFunc_GFXSeti_C_CURGLOB(13105, 0);
                AI_EvalFunc_GFXSeti_C_CURGLOB(13107, 1);
                AI_EvalFunc_GFXSetf_C_CURGLOB(13000, -0.2f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(13001, -0.80000001f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(13002, 0.0099999998f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(13009, 10.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(13010, 20.0);
                AI_EvalFunc_GFXSeti_C_CURGLOB(13103, 0xFF000000);
                AI_EvalFunc_GFXSeti_C_CURGLOB(13104, 0x800037B2);
                v24.x = -1.0;
                v24.y = -1.0;
                v24.z = 0.0;
                AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v24);
                v48.x = 1.0;
                v48.y = 1.0;
                v48.z = 0.0;
                AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v48);
                AI_EvalFunc_GFXSetf_C_CURGLOB(13005, 1.5);
                AI_EvalFunc_GFXSetf_C_CURGLOB(13006, 3.0);
                v35.x = 0.15000001f;
                v35.y = 0.15000001f;
                v35.z = 0.0;
                AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v35);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v44->GFX_Flammes, 0, 1);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v44->GFX_Flammes, 2, 1);
                AI_EvalFunc_GFXSetf_C_CURRENT(v44->GFX_Flammes, 13012, 0.25);
            }
            AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&a2);
            v32.x = a2.x * 0.75f;
            v32.y = a2.y * 0.75f;
            v32.z = a2.z * 0.75f;
            v41 = v32;
            v21 = AI_EvalFunc_MATHRandFloat_C(-2.0, 2.0);
            v33 = AI_EvalFunc_MATHRandFloat_C(-2.0, 2.0);
            v43 = AI_EvalFunc_MATHRandFloat_C(10.0, 20.0);
            v37 = v21;
            v38 = v33;
            v39 = v43;
            v41.x = v41.x + v21;
            v41.y = v41.y + v33;
            v41.z = v41.z + v43;
            GFXSetCurGlobal(v44->GFX_Flammes);
            AI_EvalFunc_GFXSetv_C_CURGLOB(13204, &v41);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v47);
            v36 = v47;
            v3 = v44->des_f_Y_max_offset * 0.75f;
            v2 = v44->des_f_Y_min_offset * 0.75f;
            v40 = AI_EvalFunc_MATHRandFloat_C(v2, v3);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v31);
            v28 = v31.x * v40;
            v29 = v31.y * v40;
            v30 = v31.z * v40;
            v36.x = v36.x + v28;
            v36.y = v36.y + v29;
            v36.z = v36.z + v30;
            v42 = TIM_gf_dt;
            a3.x = v41.x * TIM_gf_dt;
            a3.y = v41.y * TIM_gf_dt;
            a3.z = v41.z * TIM_gf_dt;
            MATH_SubstractVec(&dest, &v36, &a3);
            AI_EvalFunc_GFXSetv_C_CURGLOB(13200, &dest);
        }
        else
        {
            v44->f_time_no_fire = TIM_gf_MainClock - TIM_gf_LockTime;
            if (v44->GFX_Flammes != -1)
            {
                AI_EvalFunc_GFXSeti_C_CURRENT(v44->GFX_Flammes, 13106, 50);
                v44->GFX_Flammes = -1;
            }
        }
        result = 1000;
    }
    return result;
}

int CV_aircraft_CV_exec_after_display()
{
    AI_EvalFunc_SetLocalAlpha_C(AI_gpst_CurrentGameObject, 0, 0, 255);
    return 1000;
}

int CV_aircraft_CV_exec_GFX_Halo()
{
    int result; // eax
    float v1; // [esp+0h] [ebp-1DCh]
    float v2; // [esp+0h] [ebp-1DCh]
    tScriptStruct_CVAirCraft* v3; // [esp+4h] [ebp-1D8h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-1D0h]
    tGameObjectMain* v5; // [esp+1Ch] [ebp-1C0h]
    float v6; // [esp+20h] [ebp-1BCh]
    float v7; // [esp+24h] [ebp-1B8h]
    float v8; // [esp+28h] [ebp-1B4h]
    float v9; // [esp+2Ch] [ebp-1B0h]
    float v10; // [esp+30h] [ebp-1ACh]
    float v11; // [esp+34h] [ebp-1A8h]
    Vec3 v12; // [esp+3Ch] [ebp-1A0h] BYREF
    Vec3 a3; // [esp+48h] [ebp-194h] BYREF
    int v14; // [esp+54h] [ebp-188h]
    Vec3 a2; // [esp+58h] [ebp-184h] BYREF
    Vec3 v16; // [esp+64h] [ebp-178h] BYREF
    Vec3 a1; // [esp+70h] [ebp-16Ch] BYREF
    Vec3 v18; // [esp+7Ch] [ebp-160h] BYREF
    float v19; // [esp+88h] [ebp-154h]
    float v20; // [esp+8Ch] [ebp-150h]
    float v21; // [esp+90h] [ebp-14Ch]
    Vec3 v22; // [esp+94h] [ebp-148h] BYREF
    int v23; // [esp+A0h] [ebp-13Ch]
    Vec3 v24; // [esp+A4h] [ebp-138h] BYREF
    float v25; // [esp+B0h] [ebp-12Ch]
    float v26; // [esp+B4h] [ebp-128h]
    float v27; // [esp+B8h] [ebp-124h]
    Vec3 v28; // [esp+BCh] [ebp-120h] BYREF
    Vec3 v29; // [esp+C8h] [ebp-114h] BYREF
    Vec3 v30; // [esp+D4h] [ebp-108h] BYREF
    Vec3 v31; // [esp+E0h] [ebp-FCh] BYREF
    int v32; // [esp+ECh] [ebp-F0h]
    Vec3 v33; // [esp+F0h] [ebp-ECh] BYREF
    float v34; // [esp+FCh] [ebp-E0h]
    float v35; // [esp+100h] [ebp-DCh]
    float v36; // [esp+104h] [ebp-D8h]
    Vec3 v37; // [esp+108h] [ebp-D4h] BYREF
    float v38; // [esp+114h] [ebp-C8h]
    float v39; // [esp+118h] [ebp-C4h]
    float v40; // [esp+11Ch] [ebp-C0h]
    float v41; // [esp+120h] [ebp-BCh]
    float v42; // [esp+124h] [ebp-B8h]
    float v43; // [esp+128h] [ebp-B4h]
    Vec3 v44; // [esp+12Ch] [ebp-B0h] BYREF
    Vec3 v45; // [esp+138h] [ebp-A4h] BYREF
    float v46; // [esp+144h] [ebp-98h]
    Vec3 v47; // [esp+148h] [ebp-94h] BYREF
    float v48; // [esp+154h] [ebp-88h]
    float v49; // [esp+158h] [ebp-84h]
    float v50; // [esp+15Ch] [ebp-80h]
    float v51; // [esp+160h] [ebp-7Ch]
    float v52; // [esp+164h] [ebp-78h]
    float v53; // [esp+168h] [ebp-74h]
    float v54; // [esp+16Ch] [ebp-70h]
    float v55; // [esp+170h] [ebp-6Ch]
    float v56; // [esp+174h] [ebp-68h]
    float v57; // [esp+178h] [ebp-64h]
    float v58; // [esp+17Ch] [ebp-60h]
    float v59; // [esp+180h] [ebp-5Ch]
    float v60; // [esp+184h] [ebp-58h]
    tScriptStruct_CVAirCraft* v61; // [esp+188h] [ebp-54h]
    float v62; // [esp+18Ch] [ebp-50h]
    float v63; // [esp+190h] [ebp-4Ch]
    float v64; // [esp+194h] [ebp-48h]
    float v65; // [esp+198h] [ebp-44h]
    float v66; // [esp+19Ch] [ebp-40h]
    float v67; // [esp+1A0h] [ebp-3Ch]
    Vec3 v68; // [esp+1A4h] [ebp-38h] BYREF
    float v69; // [esp+1B8h] [ebp-24h]
    float v70; // [esp+1BCh] [ebp-20h]
    float v71; // [esp+1C0h] [ebp-1Ch]
    float v72; // [esp+1C4h] [ebp-18h]
    float v73; // [esp+1C8h] [ebp-14h]
    float v74; // [esp+1CCh] [ebp-10h]
    float v75; // [esp+1D0h] [ebp-Ch]
    float v76; // [esp+1D4h] [ebp-8h]
    float v77; // [esp+1D8h] [ebp-4h]

    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_CVAirCraft*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v61 = v3;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        CV_aircraft_CV_exec_GFX_Halo_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    if (v61->des_bateau)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else if (v61->f_camera_dist > 1000000.0
        || (v23 = (unsigned __int16)AI_gpst_CurrentGameObject->flags2, (v23 & 4) != 0))
    {
        if (v61->GFX_Halo_G != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v61->GFX_Halo_G);
            v61->GFX_Halo_G = -1;
        }
        if (v61->GFX_Halo_D != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v61->GFX_Halo_D);
            v61->GFX_Halo_D = -1;
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        v57 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
        if (v57 * 0.89999998 <= v61->f_speed_target)
        {
            v2 = 6.0f * TIM_gf_dt;
            v32 = AI_EvalFunc_COLORBlend_C(v61->c_phares, 0x40000040, v2);
            v61->c_phares = v32;
        }
        else
        {
            v46 = TIM_gf_dt;
            v1 = 6.0f * TIM_gf_dt;
            v14 = AI_EvalFunc_COLORBlend_C(v61->c_phares, 0xFF0000FF, v1);
            v61->c_phares = v14;
        }
        if (v61->GFX_Halo_G == -1)
        {
            v61->GFX_Halo_G = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v61->GFX_Halo_G, 4, 1);
            v5 = (tGameObjectMain*)CV_aircraft_CV_exec_GFX_Halo_SFX_light_smoke_gao;
            if (CV_aircraft_CV_exec_GFX_Halo_SFX_light_smoke_gao)
            {
                if ((unsigned int)CV_aircraft_CV_exec_GFX_Halo_SFX_light_smoke_gao <= 2)
                    v5 = AI_MainActor[CV_aircraft_CV_exec_GFX_Halo_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v61->GFX_Halo_G, v5, 0);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v61->GFX_Halo_G, 0, 0);
            }
            GFXSetCurGlobal(v61->GFX_Halo_G);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.40000001f);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 5.0);
        }
        AI_EvalFunc_GFXSeti_C_CURRENT(v61->GFX_Halo_G, 21101, v61->c_phares);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
        a3 = a1;
        if (v61->des_type)
        {
            switch (v61->des_type)
            {
            case 1:
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v33);
                v19 = v33.x * 7.0f;
                v20 = v33.y * 7.0f;
                v21 = v33.z * 7.0f;
                a3.x = a3.x - v19;
                a3.y = a3.y - v20;
                a3.z = a3.z - v21;
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v29);
                v9 = v29.x * 3.0f;
                v10 = v29.y * 3.0f;
                v11 = v29.z * 3.0f;
                a3.x = a3.x + v9;
                a3.y = a3.y + v10;
                a3.z = a3.z + v11;
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v28);
                v51 = v28.x * 3.0f;
                v52 = v28.y * 3.0f;
                v53 = v28.z * 3.0f;
                a3.x = a3.x - v51;
                a3.y = a3.y - v52;
                a3.z = a3.z - v53;
                break;
            case 2:
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v37);
                v75 = v37.x * 10.0f;
                v76 = v37.y * 10.0f;
                v77 = v37.z * 10.0f;
                a3.x = a3.x - v75;
                a3.y = a3.y - v76;
                a3.z = a3.z - v77;
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v44);
                v25 = v44.x * 5.0f;
                v26 = v44.y * 5.0f;
                v27 = v44.z * 5.0f;
                a3.x = a3.x + v25;
                a3.y = a3.y + v26;
                a3.z = a3.z + v27;
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v24);
                v48 = v24.x * 4.0f;
                v49 = v24.y * 4.0f;
                v50 = v24.z * 4.0f;
                a3.x = a3.x - v48;
                a3.y = a3.y - v49;
                a3.z = a3.z - v50;
                break;
            case 3:
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v12);
                v62 = v12.x * 12.0f;
                v63 = v12.y * 12.0f;
                v64 = v12.z * 12.0f;
                a3.x = a3.x - v62;
                a3.y = a3.y - v63;
                a3.z = a3.z - v64;
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v30);
                v58 = v30.x * 6.0f;
                v59 = v30.y * 6.0f;
                v60 = v30.z * 6.0f;
                a3.x = a3.x + v58;
                a3.y = a3.y + v59;
                a3.z = a3.z + v60;
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v31);
                v41 = v31.x * 5.0f;
                v42 = v31.y * 5.0f;
                v43 = v31.z * 5.0f;
                a3.x = a3.x - v41;
                a3.y = a3.y - v42;
                a3.z = a3.z - v43;
                break;
            }
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v45);
            v65 = v45.x * 5.0f;
            v66 = v45.y * 5.0f;
            v67 = v45.z * 5.0f;
            a3.x = a3.x - v65;
            a3.y = a3.y - v66;
            a3.z = a3.z - v67;
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v22);
            v34 = v22.x + v22.x;
            v35 = v22.y + v22.y;
            v36 = v22.z + v22.z;
            a3.x = a3.x + v34;
            a3.y = a3.y + v35;
            a3.z = a3.z + v36;
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&a2);
            v72 = a2.x + a2.x;
            v73 = a2.y + a2.y;
            v74 = a2.z + a2.z;
            a3.x = a3.x - v72;
            a3.y = a3.y - v73;
            a3.z = a3.z - v74;
        }
        AI_EvalFunc_GFXSetv_C_CURRENT(v61->GFX_Halo_G, 21200, &a3);
        if (v61->GFX_Halo_D == -1)
        {
            v61->GFX_Halo_D = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v61->GFX_Halo_D, 4, 1);
            v4 = (tGameObjectMain*)CV_aircraft_CV_exec_GFX_Halo_SFX_light_smoke_gao;
            if (CV_aircraft_CV_exec_GFX_Halo_SFX_light_smoke_gao)
            {
                if ((unsigned int)CV_aircraft_CV_exec_GFX_Halo_SFX_light_smoke_gao <= 2)
                    v4 = AI_MainActor[CV_aircraft_CV_exec_GFX_Halo_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v61->GFX_Halo_D, v4, 0);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v61->GFX_Halo_D, 0, 0);
            }
            GFXSetCurGlobal(v61->GFX_Halo_D);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.40000001f);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 5.0);
        }
        AI_EvalFunc_GFXSeti_C_CURRENT(v61->GFX_Halo_D, 21101, v61->c_phares);
        if (v61->des_type)
        {
            switch (v61->des_type)
            {
            case 1:
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v47);
                v6 = v47.x * 6.0f;
                v7 = v47.y * 6.0f;
                v8 = v47.z * 6.0f;
                a3.x = a3.x + v6;
                a3.y = a3.y + v7;
                a3.z = a3.z + v8;
                break;
            case 2:
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v18);
                v54 = v18.x * 8.0f;
                v55 = v18.y * 8.0f;
                v56 = v18.z * 8.0f;
                a3.x = a3.x + v54;
                a3.y = a3.y + v55;
                a3.z = a3.z + v56;
                break;
            case 3:
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v68);
                v38 = v68.x * 10.0f;
                v39 = v68.y * 10.0f;
                v40 = v68.z * 10.0f;
                a3.x = a3.x + v38;
                a3.y = a3.y + v39;
                a3.z = a3.z + v40;
                break;
            }
        }
        else
        {
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v16);
            v69 = v16.x * 4.0f;
            v70 = v16.y * 4.0f;
            v71 = v16.z * 4.0f;
            a3.x = a3.x + v69;
            a3.y = a3.y + v70;
            a3.z = a3.z + v71;
        }
        AI_EvalFunc_GFXSetv_C_CURRENT(v61->GFX_Halo_D, 21200, &a3);
        result = 1000;
    }
    return result;
}

int CV_aircraft_CV_init()
{
    int result; // eax
    int v1; // eax
    tGameObjectMain* v2; // eax
    int v3; // eax
    float v4; // [esp+0h] [ebp-3C4h]
    tScriptStruct_CVAirCraft* v5; // [esp+Ch] [ebp-3B8h]
    Vec3* v6; // [esp+10h] [ebp-3B4h]
    tGameObjectMain* v7; // [esp+2Ch] [ebp-398h]
    tGameObjectMain* v8; // [esp+30h] [ebp-394h]
    tGameObjectMain* v9; // [esp+3Ch] [ebp-388h]
    tGameObjectMain* v10; // [esp+44h] [ebp-380h]
    tAI* v11; // [esp+4Ch] [ebp-378h]
    unsigned int v12; // [esp+50h] [ebp-374h]
    tGameObjectMain* v13; // [esp+54h] [ebp-370h]
    tGameObjectMain* v14; // [esp+58h] [ebp-36Ch]
    tAI* v15; // [esp+5Ch] [ebp-368h]
    unsigned int v16; // [esp+60h] [ebp-364h]
    tGameObjectMain* v17; // [esp+64h] [ebp-360h]
    tGameObjectMain* v18; // [esp+68h] [ebp-35Ch]
    tAI* v19; // [esp+6Ch] [ebp-358h]
    unsigned int v20; // [esp+70h] [ebp-354h]
    tGameObjectMain* v21; // [esp+74h] [ebp-350h]
    tGameObjectMain* v22; // [esp+78h] [ebp-34Ch]
    tAI* v23; // [esp+80h] [ebp-344h]
    unsigned int v24; // [esp+84h] [ebp-340h]
    tGameObjectMain* v25; // [esp+88h] [ebp-33Ch]
    tGameObjectMain* v26; // [esp+8Ch] [ebp-338h]
    tAI* v27; // [esp+90h] [ebp-334h]
    unsigned int v28; // [esp+94h] [ebp-330h]
    tGameObjectMain* v29; // [esp+98h] [ebp-32Ch]
    tGameObjectMain* v30; // [esp+9Ch] [ebp-328h]
    tGameObjectMain* v31; // [esp+A0h] [ebp-324h]
    tGameObjectMain* v32; // [esp+A4h] [ebp-320h]
    tGameObjectMain* v33; // [esp+ACh] [ebp-318h]
    tGameObjectMain* v34; // [esp+B4h] [ebp-310h]
    tAI* v35; // [esp+BCh] [ebp-308h]
    unsigned int v36; // [esp+C0h] [ebp-304h]
    tGameObjectMain* v37; // [esp+C4h] [ebp-300h]
    tGameObjectMain* v38; // [esp+C8h] [ebp-2FCh]
    tGameObjectMain* v39; // [esp+CCh] [ebp-2F8h]
    tGameObjectMain* v40; // [esp+D0h] [ebp-2F4h]
    tGameObjectMain* v41; // [esp+D4h] [ebp-2F0h]
    tGameObjectMain* v42; // [esp+D8h] [ebp-2ECh]
    tAI* v43; // [esp+ECh] [ebp-2D8h]
    unsigned int v44; // [esp+F0h] [ebp-2D4h]
    tGameObjectMain* v45; // [esp+F4h] [ebp-2D0h]
    tGameObjectMain* v46; // [esp+F8h] [ebp-2CCh]
    tAI* v47; // [esp+FCh] [ebp-2C8h]
    tAI* v48; // [esp+100h] [ebp-2C4h]
    tGameObjectMain* v49; // [esp+118h] [ebp-2ACh]
    tGameObjectMain* v50; // [esp+120h] [ebp-2A4h]
    tGameObjectMain* v51; // [esp+128h] [ebp-29Ch]
    tGameObjectMain* v52; // [esp+130h] [ebp-294h]
    tGameObjectMain* v53; // [esp+138h] [ebp-28Ch]
    tGameObjectMain* v54; // [esp+140h] [ebp-284h]
    tGameObjectMain* v55; // [esp+144h] [ebp-280h]
    Vec3 v56; // [esp+154h] [ebp-270h] BYREF
    Vec3 v57; // [esp+160h] [ebp-264h] BYREF
    float v58; // [esp+178h] [ebp-24Ch]
    Vec3 v59; // [esp+17Ch] [ebp-248h] BYREF
    int v60; // [esp+188h] [ebp-23Ch]
    float v61; // [esp+18Ch] [ebp-238h]
    int v62; // [esp+190h] [ebp-234h]
    int v63; // [esp+194h] [ebp-230h]
    Vec3 v64; // [esp+198h] [ebp-22Ch] BYREF
    int v65; // [esp+1A4h] [ebp-220h]
    int v66; // [esp+1A8h] [ebp-21Ch]
    Vec3 v67; // [esp+1ACh] [ebp-218h] BYREF
    int v68; // [esp+1B8h] [ebp-20Ch]
    tGameObjectMain* v69; // [esp+1C0h] [ebp-204h]
    Vec3 v70; // [esp+1C4h] [ebp-200h] BYREF
    Vec3 v71; // [esp+1D0h] [ebp-1F4h] BYREF
    Vec3 v72; // [esp+1DCh] [ebp-1E8h] BYREF
    tGameObjectMain* v73; // [esp+1ECh] [ebp-1D8h]
    BOOL v74; // [esp+1F4h] [ebp-1D0h]
    Vec3 a3; // [esp+1F8h] [ebp-1CCh] BYREF
    BOOL v76; // [esp+208h] [ebp-1BCh]
    Vec3 v77; // [esp+20Ch] [ebp-1B8h]
    Vec3 a1; // [esp+21Ch] [ebp-1A8h] BYREF
    Vec3 v79; // [esp+228h] [ebp-19Ch] BYREF
    Vec3 v80; // [esp+234h] [ebp-190h] BYREF
    Vec3 v81; // [esp+240h] [ebp-184h] BYREF
    Vec3 dest; // [esp+258h] [ebp-16Ch] BYREF
    Vec3 v83; // [esp+264h] [ebp-160h] BYREF
    float v84; // [esp+270h] [ebp-154h]
    int v85; // [esp+278h] [ebp-14Ch]
    Vec3 v86; // [esp+27Ch] [ebp-148h] BYREF
    Vec3 v87; // [esp+294h] [ebp-130h] BYREF
    Vec3 v88; // [esp+2A0h] [ebp-124h] BYREF
    Vec3 v89; // [esp+2ACh] [ebp-118h] BYREF
    int v90; // [esp+2B8h] [ebp-10Ch]
    tGameObjectMain* v91; // [esp+2BCh] [ebp-108h]
    int v92; // [esp+2C4h] [ebp-100h]
    Vec3 v93; // [esp+2CCh] [ebp-F8h] BYREF
    tGameObjectMain* v94; // [esp+2DCh] [ebp-E8h]
    BOOL v95; // [esp+2E0h] [ebp-E4h]
    int v96; // [esp+2E4h] [ebp-E0h]
    int v97; // [esp+2F4h] [ebp-D0h]
    int v98; // [esp+2F8h] [ebp-CCh]
    int v99; // [esp+2FCh] [ebp-C8h]
    Vec3 v100; // [esp+300h] [ebp-C4h] BYREF
    Vec3 v101; // [esp+30Ch] [ebp-B8h] BYREF
    int v102; // [esp+31Ch] [ebp-A8h]
    int v103; // [esp+320h] [ebp-A4h]
    Vec3 v104; // [esp+328h] [ebp-9Ch] BYREF
    Vec3 v105; // [esp+334h] [ebp-90h] BYREF
    Vec3 v106; // [esp+34Ch] [ebp-78h] BYREF
    int v107; // [esp+358h] [ebp-6Ch]
    Vec3 v108; // [esp+364h] [ebp-60h] BYREF
    tScriptStruct_CVAirCraft* v109; // [esp+370h] [ebp-54h]
    int v110; // [esp+374h] [ebp-50h]
    Vec3 v111; // [esp+378h] [ebp-4Ch] BYREF
    Vec3 v112; // [esp+384h] [ebp-40h] BYREF
    unsigned int v113; // [esp+390h] [ebp-34h]
    int v114; // [esp+398h] [ebp-2Ch]
    int v115; // [esp+39Ch] [ebp-28h]
    Vec3 v116; // [esp+3ACh] [ebp-18h] BYREF
    unsigned int v117; // [esp+3B8h] [ebp-Ch]
    unsigned int v118; // [esp+3BCh] [ebp-8h]
    BOOL v119; // [esp+3C0h] [ebp-4h]

    v100.x = 1.0;
    v100.y = 0.0;
    v100.z = 0.0;
    v89.x = 1.0;
    v89.y = 0.0;
    v89.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_CVAirCraft*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v109 = v5;
    v103 = 0;
    v92 = 0;
    v69 = 0;
    v110 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        CV_aircraft_CV_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&CV_aircraft_CV_init_univVars_Flag_speciaux_sauve, (void*)CV_aircraft_CV_init_univVars, 4488);
        AI2C_ResolveBuffer(&CV_aircraft_CV_init_univVars_Perso_ID_actor, (void*)CV_aircraft_CV_init_univVars, 988);
        CV_aircraft_CV_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &CV_aircraft_CV_init_GST_Global_gao_i_map_just_reset,
            (void*)CV_aircraft_CV_init_GST_Global_gao,
            3300);
        AI2C_ResolveBuffer(&CV_aircraft_CV_init_univVars_world_ID, (void*)CV_aircraft_CV_init_univVars, 4);
        CV_aircraft_CV_init_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        fct_CV_aircraft_CV_init_CV_aircraft_CV_exec_afterrec = (int)AI_ul_RealResolveOneRefFunction(0x72004737u);
        AI2C_ResolveBuffer(&CV_aircraft_CV_init_GST_Global_gao_o_targets, (void*)CV_aircraft_CV_init_GST_Global_gao, 964);
        AI2C_ResolveBuffer(
            &CV_aircraft_CV_init_GST_Global_gao_i_targets_nb,
            (void*)CV_aircraft_CV_init_GST_Global_gao,
            2572);
        AI2C_ResolveBuffer(
            &CV_aircraft_CV_init_GST_Global_gao_v_targets_offset,
            (void*)CV_aircraft_CV_init_GST_Global_gao,
            1368);
        fct_CV_aircraft_CV_init_CV_aircraft_CV_mode_deplacement = (int)AI_ul_RealResolveOneRefFunction(0x7200198Du);
    }
    if (v109->des_i_sf_jesuisla)
    {
        if (v109->des_i_sf_jesuisla <= 0)
        {
            v110 = 0;
        }
        else if (v109->des_i_sf_jesuisla < 10000)
        {
            v110 = AI_EvalFunc_ARRBitTest_C(
                (int*)(CV_aircraft_CV_init_univVars_Flag_speciaux_sauve + 4),
                v109->des_i_sf_jesuisla);
        }
        else
        {
            v76 = AI_EvalFunc_ARRBitTest_C(
                (int*)(CV_aircraft_CV_init_univVars_Flag_speciaux_sauve + 4),
                v109->des_i_sf_jesuisla - 10000);
            v110 = 1 - v76;
        }
        if (!v110)
        {
            v96 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v96 != 1000)
                return v96;
        }
    }
    v65 = AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 1, 2);
    if (v109->des_mode == 1)
    {
        v63 = AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 0, 32);
        v85 = AI_EvalFunc_MATHRandInt_C(0, 4);
        v109->i_num_mat = v85;
        if (v109->des_i_quartier_pietons)
        {
            AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, 0.80000001f);
        }
        else
        {
            AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, 1.5);
            v109->des_f_speed = AI_EvalFunc_MATHRandFloat_C(50.0, 70.0);
        }
        v109->des_type = 1;
    }
    v109->i_limo_categ = AI_EvalFunc_MATHRandInt_C(0, 2);
    AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
    AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
    AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
    if (v109->des_SF_desactivation <= 0)
        v110 = 0;
    else
        v110 = AI_EvalFunc_ARRBitTest_C(
            (int*)(CV_aircraft_CV_init_univVars_Flag_speciaux_sauve + 4),
            v109->des_SF_desactivation);
    if (v110)
    {
        v113 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 6291456, 0);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1;
    }
    else
    {
        v69 = *AI_MainActor;
        v55 = v69;
        if (v69)
        {
            if ((unsigned int)v69 <= 2)
                v55 = AI_MainActor[(uint32_t)v69 - 1];
            v54 = v55;
        }
        else
        {
            v54 = 0;
        }
        if (v54)
        {
            v109->o_aircraft = *(uint32_t*)(CV_aircraft_CV_init_univVars_Perso_ID_actor + 44);
            v53 = (tGameObjectMain*)v109->des_o_wp_of_net;
            if (v53)
            {
                if ((unsigned int)v53 <= 2)
                    v53 = AI_MainActor[(uint32_t)v53 - 1];
                v1 = (int)AI_EvalFunc_WAYNetOfObj_C(v53);
            }
            else
            {
                v1 = (int)AI_EvalFunc_WAYNetOfObj_C(0);
            }
            v109->n_net = v1;
            v109->o_current_wp = v109->des_o_wp_of_net;
            v52 = (tGameObjectMain*)v109->o_current_wp;
            if (v52)
            {
                if ((unsigned int)v52 <= 2)
                    v52 = AI_MainActor[(uint32_t)v52 - 1];
                v2 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v109->n_net, v52, 1, 0);
            }
            else
            {
                v2 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v109->n_net, 0, 1, 0);
            }
            v109->o_next_wp = (int)v2;
            if (v109->des_rand_bv_pos)
            {
                v51 = (tGameObjectMain*)v109->o_next_wp;
                if (v51)
                {
                    if ((unsigned int)v51 <= 2)
                        v51 = AI_MainActor[(uint32_t)v51 - 1];
                    AI_EvalFunc_BVRandomPosGet_C(v51, 0, &v109->v_dest_pos);
                }
                else
                {
                    AI_EvalFunc_BVRandomPosGet_C(0, 0, &v109->v_dest_pos);
                }
            }
            else
            {
                v50 = (tGameObjectMain*)v109->o_next_wp;
                if (v50)
                {
                    if ((unsigned int)v50 <= 2)
                        v50 = AI_MainActor[(uint32_t)v50 - 1];
                    AI_EvalFunc_OBJGetPos_C(v50, &v109->v_dest_pos);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v109->v_dest_pos);
                }
            }
            v109->f_water_Z = -1000000000.0;
            v49 = (tGameObjectMain*)v109->o_current_wp;
            if (v49)
            {
                if ((unsigned int)v49 <= 2)
                    v49 = AI_MainActor[(uint32_t)v49 - 1];
                AI_EvalFunc_OBJGetPos_C(v49, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            v100 = a3;
            AI_EvalFunc_WTR_GetDif_C(&v100, &v81);
            v89 = v81;
            if (v81.z <= -1000000000.0)
            {
                v48 = (tAI*)v109->des_o_bison_fute;
                if (v109->des_o_bison_fute)
                {
                    if ((unsigned int)v48 <= 2)
                        v48 = (tAI*)AI_MainActor[(uint32_t)v48 - 1];
                    v47 = v48;
                }
                else
                {
                    v47 = 0;
                }
                if (v47)
                {
                    v44 = v109->des_o_bison_fute;
                    if ((tAI*)v109->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
                    {
                        v43 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v46 = (tGameObjectMain*)v109->des_o_bison_fute;
                        if (v44)
                        {
                            if (v44 <= 2)
                                v46 = AI_MainActor[v44 - 1];
                            v45 = v46;
                        }
                        else
                        {
                            v45 = 0;
                        }
                        v43 = v45->extended->AIInstance;
                    }
                    v109->f_water_Z = ((tScriptStruct_BisonFute*)(v43->instanceVarDataCurrent))->f_water_Z;
                }
                if (v109->f_water_Z == -1000000000.0f)
                {
                    v59.x = 0.0;
                    v59.y = 0.0;
                    v59.z = 500.0;
                    MATH_VecAddVec(&a1, &v100, &v59);
                    v89 = a1;
                    v97 = 0;
                    v98 = 0;
                    v99 = 1065353216;
                    v88.x = -0.0;
                    v88.y = -0.0;
                    v88.z = -1.0;
                    v117 = (int)AI_EvalFunc_COLRayObjectDist_C(AI_gpst_CurrentGameObject, &v89, &v88, 1000.0, -1, 0, 0, 2);
                    v42 = (tGameObjectMain*)v117;
                    if (v117)
                    {
                        if (v117 <= 2)
                            v42 = AI_MainActor[v117 - 1];
                        v41 = v42;
                    }
                    else
                    {
                        v41 = 0;
                    }
                    if (v41)
                    {
                        AI_EvalFunc_COLRayPosGet_C(AI_gpst_CurrentGameObject, &v93);
                        v109->f_water_Z = v93.z;
                        v40 = (tGameObjectMain*)v109->des_o_bison_fute;
                        if (v109->des_o_bison_fute)
                        {
                            if ((unsigned int)v40 <= 2)
                                v40 = AI_MainActor[(uint32_t)v40 - 1];
                            v39 = v40;
                        }
                        else
                        {
                            v39 = 0;
                        }
                        if (v39)
                        {
                            v36 = v109->des_o_bison_fute;
                            if ((tAI*)v109->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
                            {
                                v35 = WOR_gst_Universe.univAIInstance;
                            }
                            else
                            {
                                v38 = (tGameObjectMain*)v109->des_o_bison_fute;
                                if (v36)
                                {
                                    if (v36 <= 2)
                                        v38 = AI_MainActor[v36 - 1];
                                    v37 = v38;
                                }
                                else
                                {
                                    v37 = 0;
                                }
                                v35 = v37->extended->AIInstance;
                            }
                            ((tScriptStruct_BisonFute*)(v35->instanceVarDataCurrent))->f_water_Z = v109->f_water_Z;
                        }
                    }
                }
                v100.z = v109->f_water_Z;
            }
            else
            {
                v100.z = v89.z;
            }
            v77.x = 0.0;
            v77.y = 0.0;
            v77.z = -1.0;
            v100.x = v100.x + 0.0f;
            v100.y = v100.y + 0.0f;
            v100.z = v100.z + -1.0f;
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v100);
            if (*(uint32_t*)CV_aircraft_CV_init_GST_Global_gao_i_map_just_reset)
                goto LABEL_113;
            v69 = AI_EvalFunc_VIEWGetObject_C(0);
            v110 = AI_EvalFunc_ARRBitTest_C((int*)(CV_aircraft_CV_init_univVars_Flag_speciaux_sauve + 4), 2718);
            if (v110)
                goto LABEL_112;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v64);
            v34 = v69;
            if (v69)
            {
                if ((unsigned int)v69 <= 2)
                    v34 = AI_MainActor[(uint32_t)v69 - 1];
                AI_EvalFunc_OBJGetPos_C(v34, &v111);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v111);
            }
            MATH_SubstractVec(&dest, &v64, &v111);
            v33 = v69;
            if (v69)
            {
                if ((unsigned int)v69 <= 2)
                    v33 = AI_MainActor[(uint32_t)v69 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v33, &v108);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v108);
            }
            v84 = v108.z * dest.z + v108.y * dest.y + dest.x * v108.x;
            if (v84 <= -100.0)
            {
            LABEL_113:
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
                v109->des_v_friction.y = (float)(6 * (4 - v109->des_type) + 6);
                v109->des_v_friction.x = v109->des_v_friction.y + v109->des_v_friction.y;
                if (!v109->des_type_speed)
                {
                    if (v109->des_type)
                    {
                        switch (v109->des_type)
                        {
                        case 1:
                            v109->des_f_speed = 70.0;
                            break;
                        case 2:
                            v109->des_f_speed = 60.0;
                            break;
                        case 3:
                            v109->des_f_speed = 50.0;
                            break;
                        }
                    }
                    else
                    {
                        v109->des_f_speed = 90.0;
                    }
                }
                v109->f_brake_dist = v109->des_f_Y_max_offset - v109->des_f_Y_min_offset;
                v109->f_vitesse = v109->des_f_speed;
                v91 = AI_gpst_CurrentGameObject;
                v32 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                        v32 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                    v31 = v32;
                }
                else
                {
                    v31 = 0;
                }
                v28 = v109->des_o_bison_fute;
                if ((tAI*)v109->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
                {
                    v27 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v30 = (tGameObjectMain*)v109->des_o_bison_fute;
                    if (v28)
                    {
                        if (v28 <= 2)
                            v30 = AI_MainActor[v28 - 1];
                        v29 = v30;
                    }
                    else
                    {
                        v29 = 0;
                    }
                    v27 = v29->extended->AIInstance;
                }
                v24 = v109->des_o_bison_fute;
                if ((tAI*)v109->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
                {
                    v23 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v26 = (tGameObjectMain*)v109->des_o_bison_fute;
                    if (v24)
                    {
                        if (v24 <= 2)
                            v26 = AI_MainActor[v24 - 1];
                        v25 = v26;
                    }
                    else
                    {
                        v25 = 0;
                    }
                    v23 = v25->extended->AIInstance;
                }
                v115 = AI_EvalFunc_ARRObjSearch_C(
                    (tGameObjectMain**)((tScriptStruct_BisonFute*)(v23->instanceVarDataCurrent))->ao_aircraft,
                    ((tScriptStruct_BisonFute*)(v27->instanceVarDataCurrent))->i_nb_aircraft,
                    v31);
                v103 = v115;
                if (v115 == -1)
                {
                    v20 = v109->des_o_bison_fute;
                    if ((tAI*)v109->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
                    {
                        v19 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v22 = (tGameObjectMain*)v109->des_o_bison_fute;
                        if (v20)
                        {
                            if (v20 <= 2)
                                v22 = AI_MainActor[v20 - 1];
                            v21 = v22;
                        }
                        else
                        {
                            v21 = 0;
                        }
                        v19 = v21->extended->AIInstance;
                    }
                    v16 = v109->des_o_bison_fute;
                    if ((tAI*)v109->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
                    {
                        v15 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v18 = (tGameObjectMain*)v109->des_o_bison_fute;
                        if (v16)
                        {
                            if (v16 <= 2)
                                v18 = AI_MainActor[v16 - 1];
                            v17 = v18;
                        }
                        else
                        {
                            v17 = 0;
                        }
                        v15 = v17->extended->AIInstance;
                    }
                    ((tScriptStruct_BisonFute*)(v15->instanceVarDataCurrent))->ao_aircraft[((tScriptStruct_BisonFute*)(v19->instanceVarDataCurrent))->i_nb_aircraft] = (int)AI_gpst_CurrentGameObject;
                    v12 = v109->des_o_bison_fute;
                    if ((tAI*)v109->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
                    {
                        v11 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v14 = (tGameObjectMain*)v109->des_o_bison_fute;
                        if (v12)
                        {
                            if (v12 <= 2)
                                v14 = AI_MainActor[v12 - 1];
                            v13 = v14;
                        }
                        else
                        {
                            v13 = 0;
                        }
                        v11 = v13->extended->AIInstance;
                    }
                    ++((tScriptStruct_BisonFute*)(v11->instanceVarDataCurrent))->i_nb_aircraft;
                }
                v119 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
                if (*(uint32_t*)CV_aircraft_CV_init_univVars_world_ID == 90)
                {
                    AI_EvalFunc_COLPrioritySet_C(AI_gpst_CurrentGameObject, 1);
                    v102 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x1039004, 0);
                }
                else
                {
                    v60 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x3039004, 0);
                }
                v72.x = 0.0;
                v72.y = 0.0;
                v72.z = 0.0;
                v107 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v72);
                v112.x = 0.0;
                v112.y = 0.0;
                v112.z = -20.0;
                AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v112);
                v86.x = 0.0;
                v86.y = 0.0;
                v86.z = 20.0;
                AI_EvalFunc_DYNTractionSet_C_CURRENT(&v86);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v79);
                MATH_SubstractVec(&v101, &v109->v_dest_pos, &v79);
                v71.x = 0.0;
                v71.y = 0.0;
                v71.z = 1.0;
                AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v101, &v71);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v80);
                AI_EvalFunc_COLStartMatrixSet_C(AI_gpst_CurrentGameObject, &v80);
                AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x8000, 0);
                v118 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0, 0x1000000);
                if (v109->des_type)
                {
                    if (v109->des_type == 1)
                    {
                        v109->f_largeur_test = 12.0;
                        v105.x = -8.0;
                        v105.y = -8.0;
                        v105.z = -3.0;
                        AI_EvalFunc_BVMinSet_C(AI_gpst_CurrentGameObject, &v105);
                        v116.x = 8.0;
                        v116.y = 8.0;
                        v116.z = 10.0;
                        AI_EvalFunc_BVMaxSet_C(AI_gpst_CurrentGameObject, &v116);
                    }
                    else if (v109->des_type == 2)
                    {
                        v109->f_largeur_test = 15.0;
                        v70.x = -8.0;
                        v70.y = -8.0;
                        v70.z = -3.0;
                        AI_EvalFunc_BVMinSet_C(AI_gpst_CurrentGameObject, &v70);
                        v83.x = 8.0;
                        v83.y = 8.0;
                        v83.z = 10.0;
                        AI_EvalFunc_BVMaxSet_C(AI_gpst_CurrentGameObject, &v83);
                    }
                    else
                    {
                        v109->f_largeur_test = 50.0;
                    }
                }
                else
                {
                    v109->f_largeur_test = 8.0;
                    v106.x = -6.0;
                    v106.y = -6.0;
                    v106.z = -3.0;
                    AI_EvalFunc_BVMinSet_C(AI_gpst_CurrentGameObject, &v106);
                    v67.x = 6.0;
                    v67.y = 6.0;
                    v67.z = 6.0;
                    AI_EvalFunc_BVMaxSet_C(AI_gpst_CurrentGameObject, &v67);
                }
                if (v109->des_mode == 1)
                {
                    AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
                    AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
                }
                else
                {
                    AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 1, 16);
                    AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0xFFu, 0);
                }
                if (fabs(v109->des_v_pos_fumee.x) != 0.0f)
                {
                    v74 = 0;
                }
                else if (fabs(v109->des_v_pos_fumee.y) != 0.0f)
                {
                    v74 = 0;
                }
                else
                {
                    v74 = fabs(v109->des_v_pos_fumee.z) == 0.0f;
                }
                if (!v74)
                {
                    v109->GFX_Smoke = AI_EvalFunc_GFXAdd_C_CURRENT(13);
                    GFXSetCurGlobal(v109->GFX_Smoke);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 100);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13106, 0x4876E800);
                    v10 = (tGameObjectMain*)CV_aircraft_CV_init_SFX_light_smoke_gao;
                    if (CV_aircraft_CV_init_SFX_light_smoke_gao)
                    {
                        if ((unsigned int)CV_aircraft_CV_init_SFX_light_smoke_gao <= 2)
                            v10 = AI_MainActor[CV_aircraft_CV_init_SFX_light_smoke_gao - 1];
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v109->GFX_Smoke, v10, -1);
                    }
                    else
                    {
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v109->GFX_Smoke, 0, -1);
                    }
                    GFXSetCurGlobal(v109->GFX_Smoke);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13101, 8);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13003, 2.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13004, 2.0);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13103, 0x1FFFFFFF);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13104, 0xFFFFFFF);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13105, 0xFFFF);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(13107, 0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13000, 5.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13001, 25.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13002, 0.050000001f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.1f);
                    v56.x = 0.0;
                    v56.y = 0.0;
                    v56.z = 10.0;
                    AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v56);
                    v104.x = 0.0;
                    v104.y = 0.0;
                    v104.z = 20.0;
                    AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v104);
                    v57.x = 0.0;
                    v57.y = 0.0;
                    v57.z = 0.1f;
                    AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v57);
                    v90 = AI_EvalFunc_MATHRandInt_C(1, 3);
                    v68 = AI_EvalFunc_MATHRandInt_C(8, 11);
                    v87.x = 0.0;
                    v87.y = (float)v90;
                    v87.z = (float)v68;
                    AI_EvalFunc_GFXSetv_C_CURGLOB(13204, &v87);
                    AI_EvalFunc_GFXFlagSet_C_CURRENT(v109->GFX_Smoke, 0, 1);
                    AI_EvalFunc_GFXFlagSet_C_CURRENT(v109->GFX_Smoke, 2, 1);
                    v58 = AI_EvalFunc_MATHRandFloat_C(1.5, 2.5);
                    AI_EvalFunc_GFXSetf_C_CURRENT(v109->GFX_Smoke, 13012, v58);
                }
                v73 = AI_gpst_CurrentGameObject;
                v9 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                        v9 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                    AI_EvalFunc_AICBAdd_C(
                        AI_gpst_CurrentGameObject,
                        v9,
                        2,
                        (tAIFunction*)fct_CV_aircraft_CV_init_CV_aircraft_CV_exec_afterrec);
                }
                else
                {
                    AI_EvalFunc_AICBAdd_C(
                        AI_gpst_CurrentGameObject,
                        0,
                        2,
                        (tAIFunction*)fct_CV_aircraft_CV_init_CV_aircraft_CV_exec_afterrec);
                }
                v3 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000000);
                v109->i_snd_loop_motor = v3;
                AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v109->i_snd_loop_motor);
                if (v109->des_type != 3)
                {
                    v114 = SND_l_GetFreq(v109->i_snd_loop_motor);
                    v61 = AI_EvalFunc_MATHRandFloat_C(0.94999999f, 1.05f);
                    v4 = (float)v114;
                    SND_SetFreq(v109->i_snd_loop_motor, (unsigned int)(v4 * v61));
                }
                v109->f_snd_delai_bip = AI_EvalFunc_MATHRandFloat_C(0.1f, 0.30000001f);
                v109->f_snd_freq_bip = AI_EvalFunc_MATHRandFloat_C(0.89999998f, 1.1f);
                v109->f_snd_freq_tut = AI_EvalFunc_MATHRandFloat_C(0.89999998f, 1.1f);
                v109->i_snd_mode = AI_EvalFunc_MATHRandInt_C(0, 4);
                v109->i_last_txt_is_comp = AI_EvalFunc_MATHRandInt_C(0, 2);
                if (fabs(v109->v_lock_offset.x) != 0.0f)
                {
                    v95 = 0;
                }
                else if (fabs(v109->v_lock_offset.y) != 0.0f)
                {
                    v95 = 0;
                }
                else
                {
                    v95 = fabs(v109->v_lock_offset.z) == 0.0f;
                }
                if (v95)
                {
                    if (v109->des_mode)
                    {
                        v109->v_lock_offset.x = 0.0;
                        v109->v_lock_offset.y = 0.0;
                        v109->v_lock_offset.z = 3.0;
                    }
                    else
                    {
                        v109->v_lock_offset.x = 0.0;
                        v109->v_lock_offset.y = 0.0;
                        v109->v_lock_offset.z = 2.3f;
                    }
                }
                v62 = AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 8, 0);
                v94 = AI_gpst_CurrentGameObject;
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
                v92 = AI_EvalFunc_ARRObjSearch_C(
                    (tGameObjectMain**)(CV_aircraft_CV_init_GST_Global_gao_o_targets + 4),
                    *(uint32_t*)CV_aircraft_CV_init_GST_Global_gao_i_targets_nb,
                    v7);
                if (v92 == -1)
                {
                    v92 = AI_EvalFunc_ARRObjSearch_C(
                        (tGameObjectMain**)(CV_aircraft_CV_init_GST_Global_gao_o_targets + 4),
                        100,
                        0);
                    if (v92 != -1)
                    {
                        ++* (uint32_t*)CV_aircraft_CV_init_GST_Global_gao_i_targets_nb;
                        *(uint32_t*)(CV_aircraft_CV_init_GST_Global_gao_o_targets + 4 * v92 + 4) = (int)AI_gpst_CurrentGameObject;
                        v6 = (Vec3*)AI_EvalVarArray_C3((char*)CV_aircraft_CV_init_GST_Global_gao_v_targets_offset, v92);
                        *v6 = v109->v_lock_offset;
                    }
                }
                v66 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_CV_aircraft_CV_init_CV_aircraft_CV_mode_deplacement);
                if (v66 == 1000)
                    result = 1000;
                else
                    result = v66;
            }
            else
            {
            LABEL_112:
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 4;
            }
        }
        else
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
    }
    return result;
}

int CV_aircraft_CV_exec_before_display()
{
    tScriptStruct_CVAirCraft* v1; // [esp+0h] [ebp-1Ch]
    float v2; // [esp+4h] [ebp-18h]
    float v3; // [esp+4h] [ebp-18h]
    float v4; // [esp+4h] [ebp-18h]
    Vec3 a1; // [esp+8h] [ebp-14h] BYREF
    tScriptStruct_CVAirCraft* v6; // [esp+18h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_CVAirCraft*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v6 = v1;
    if (v1->i_flag_on_coule)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
        v2 = v6->f_water_Z - 5.0f - a1.z;
        v3 = v2 / 50.0f;
        if (v3 <= 1.0)
        {
            if (v3 < 0.0)
                v3 = 0.0;
        }
        else
        {
            v3 = 1.0;
        }
        v4 = 1.0f - v3;
        AI_EvalFunc_SetLocalAlpha_C(AI_gpst_CurrentGameObject, 0, 0, (unsigned int)(255.0f * v4));
    }
    return 1000;
}

int CV_aircraft_CV_bdisplay()
{
    tScriptStruct_CVAirCraft* v1; // [esp+0h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_CVAirCraft*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    AI_EvalFunc_MATEnableMltxtLvl_C(AI_gpst_CurrentGameObject, 0x16003F9Eu, 0, 0);
    AI_EvalFunc_MATEnableMltxtLvl_C(AI_gpst_CurrentGameObject, 0x16003F9Eu, 1, 0);
    AI_EvalFunc_MATEnableMltxtLvl_C(AI_gpst_CurrentGameObject, 0x16003F9Eu, 2, 0);
    AI_EvalFunc_MATEnableMltxtLvl_C(AI_gpst_CurrentGameObject, 0x16003F9Eu, 3, 0);
    AI_EvalFunc_MATEnableMltxtLvl_C(AI_gpst_CurrentGameObject, 0x16003F9Eu, 4, 0);
    AI_EvalFunc_MATEnableMltxtLvl_C(AI_gpst_CurrentGameObject, 0x16003F9Eu, v1->i_num_mat, 1);
    return 1000;
}

int CV_aircraft_CV_exec_s_b()
{
    BOOL v1; // eax
    float X; // [esp+0h] [ebp-3FCh]
    float Xa; // [esp+0h] [ebp-3FCh]
    float Xb; // [esp+0h] [ebp-3FCh]
    float Xc; // [esp+0h] [ebp-3FCh]
    float Xd; // [esp+0h] [ebp-3FCh]
    float Xe; // [esp+0h] [ebp-3FCh]
    float Xf; // [esp+0h] [ebp-3FCh]
    float v9; // [esp+8h] [ebp-3F4h]
    float v10; // [esp+Ch] [ebp-3F0h]
    float v11; // [esp+10h] [ebp-3ECh]
    float v12; // [esp+14h] [ebp-3E8h]
    float v13; // [esp+18h] [ebp-3E4h]
    float v14; // [esp+1Ch] [ebp-3E0h]
    float v15; // [esp+20h] [ebp-3DCh]
    float v16; // [esp+24h] [ebp-3D8h]
    tScriptStruct_CVAirCraft* v17; // [esp+28h] [ebp-3D4h]
    float v18; // [esp+44h] [ebp-3B8h]
    float v19; // [esp+48h] [ebp-3B4h]
    float v20; // [esp+4Ch] [ebp-3B0h]
    tGameObjectMain* v21; // [esp+98h] [ebp-364h]
    float v22; // [esp+A4h] [ebp-358h]
    float v23; // [esp+A8h] [ebp-354h]
    float v24; // [esp+C0h] [ebp-33Ch]
    Vec3 v25; // [esp+11Ch] [ebp-2E0h] BYREF
    Vec3 v26; // [esp+128h] [ebp-2D4h]
    Vec3 v27; // [esp+134h] [ebp-2C8h] BYREF
    Vec3 v28; // [esp+140h] [ebp-2BCh] BYREF
    float v29; // [esp+14Ch] [ebp-2B0h]
    float v30; // [esp+150h] [ebp-2ACh]
    float v31; // [esp+154h] [ebp-2A8h]
    float v32; // [esp+158h] [ebp-2A4h]
    Vec3 v33; // [esp+15Ch] [ebp-2A0h] BYREF
    Vec3 v34; // [esp+168h] [ebp-294h] BYREF
    Vec3 v35; // [esp+174h] [ebp-288h] BYREF
    float v36; // [esp+180h] [ebp-27Ch]
    float v37; // [esp+184h] [ebp-278h]
    float v38; // [esp+188h] [ebp-274h]
    Vec3 v39; // [esp+18Ch] [ebp-270h] BYREF
    Vec3 a3; // [esp+198h] [ebp-264h] BYREF
    float v41; // [esp+1A8h] [ebp-254h]
    Vec3 v42; // [esp+1ACh] [ebp-250h] BYREF
    float v44; // [esp+1BCh] [ebp-240h]
    Vec3 v45; // [esp+1C0h] [ebp-23Ch] BYREF
    float v46; // [esp+1CCh] [ebp-230h]
    float v47; // [esp+1D8h] [ebp-224h]
    float v48; // [esp+1DCh] [ebp-220h]
    Vec3 v49; // [esp+1E4h] [ebp-218h] BYREF
    Vec3 a2; // [esp+1F0h] [ebp-20Ch] BYREF
    float v51; // [esp+1FCh] [ebp-200h]
    float v52; // [esp+200h] [ebp-1FCh]
    float v53; // [esp+204h] [ebp-1F8h]
    Vec3 v54; // [esp+208h] [ebp-1F4h]
    Vec3 v55; // [esp+214h] [ebp-1E8h] BYREF
    Vec3 v56; // [esp+220h] [ebp-1DCh] BYREF
    float v57; // [esp+22Ch] [ebp-1D0h]
    tGameObjectMain* v58; // [esp+230h] [ebp-1CCh]
    Vec3 v59; // [esp+234h] [ebp-1C8h] BYREF
    Vec3 v60; // [esp+240h] [ebp-1BCh] BYREF
    Vec3 v61; // [esp+24Ch] [ebp-1B0h] BYREF
    Vec3 a5; // [esp+258h] [ebp-1A4h] BYREF
    float v63; // [esp+264h] [ebp-198h]
    float v64; // [esp+268h] [ebp-194h]
    BOOL v65; // [esp+26Ch] [ebp-190h]
    Vec3 v66; // [esp+270h] [ebp-18Ch] BYREF
    Vec3 v67; // [esp+27Ch] [ebp-180h] BYREF
    Vec3 v68; // [esp+288h] [ebp-174h] BYREF
    Vec3 v69; // [esp+294h] [ebp-168h] BYREF
    Vec3 a1; // [esp+2A0h] [ebp-15Ch] BYREF
    float v71; // [esp+2ACh] [ebp-150h]
    Vec3 v72; // [esp+2B0h] [ebp-14Ch] BYREF
    Vec3 a4; // [esp+2BCh] [ebp-140h] BYREF
    Vec3 v74; // [esp+2C8h] [ebp-134h] BYREF
    Vec3 v75; // [esp+2D4h] [ebp-128h] BYREF
    Vec3 v76; // [esp+2E0h] [ebp-11Ch] BYREF
    float v77; // [esp+2ECh] [ebp-110h]
    float v78; // [esp+2F0h] [ebp-10Ch]
    float v79; // [esp+2F4h] [ebp-108h]
    Vec3 v80; // [esp+2F8h] [ebp-104h] BYREF
    Vec3 v81; // [esp+310h] [ebp-ECh] BYREF
    float v82; // [esp+31Ch] [ebp-E0h]
    float v83; // [esp+320h] [ebp-DCh]
    Vec3 v84; // [esp+324h] [ebp-D8h] BYREF
    Vec3 v85; // [esp+330h] [ebp-CCh] BYREF
    float v86; // [esp+33Ch] [ebp-C0h]
    float v87; // [esp+340h] [ebp-BCh]
    float v88; // [esp+344h] [ebp-B8h]
    Vec3 v90; // [esp+34Ch] [ebp-B0h] BYREF
    Vec3 v91; // [esp+358h] [ebp-A4h] BYREF
    Vec3 v92; // [esp+370h] [ebp-8Ch] BYREF
    float v93; // [esp+37Ch] [ebp-80h]
    float v94; // [esp+380h] [ebp-7Ch]
    float v95; // [esp+384h] [ebp-78h]
    Vec3 v96; // [esp+388h] [ebp-74h] BYREF
    tScriptStruct_CVAirCraft* v97; // [esp+394h] [ebp-68h]
    float v98; // [esp+398h] [ebp-64h]
    float v99; // [esp+39Ch] [ebp-60h]
    float v100; // [esp+3A0h] [ebp-5Ch]
    Vec3 v101; // [esp+3A4h] [ebp-58h] BYREF
    Vec3 v102; // [esp+3B0h] [ebp-4Ch] BYREF
    Vec3 v103; // [esp+3BCh] [ebp-40h] BYREF
    float v104; // [esp+3C8h] [ebp-34h]
    Vec3 v105; // [esp+3D0h] [ebp-2Ch] BYREF
    Vec3 v106; // [esp+3DCh] [ebp-20h] BYREF
    float v107; // [esp+3F0h] [ebp-Ch]
    float v108; // [esp+3F4h] [ebp-8h]
    float v109; // [esp+3F8h] [ebp-4h]

    v55.x = 1.0;
    v55.y = 0.0;
    v55.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    v69.x = 1.0;
    v69.y = 0.0;
    v69.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v17 = (tScriptStruct_CVAirCraft*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v17 = 0;
    v97 = v17;
    v57 = 0.0;
    v51 = 0.0;
    v58 = 0;
    v107 = 0.0;
    v53 = 0.0;
    if (v17->f_collision_rot_speed != 0.0f)
    {
        v16 = (float)fabs(v97->f_collision_rot_speed);
        v52 = v16;
        if (v16 < 0.2)
            v97->f_collision_rot_speed = 0.0;
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v92);
    MATH_SubstractVec(&v97->v_new_sight, &v97->v_dest_pos, &v92);
    MATH_NormalizeEqualVector(&v97->v_new_sight);
    if (v97->i_flag_on_coule)
    {
        v57 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
        if (v57 == 0.0)
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v72);
            a3 = v72;
            AI_EvalFunc_MATHVecSetHorzNormalize_C(&a3);
            a2.x = 0.0;
            a2.y = 0.0;
            a2.z = 1.0;
            AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, 0.80000001f, &a5);
            a3 = a5;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v33);
            v64 = TIM_gf_dt;
            AI_EvalFunc_MATHVecBlend_C(&v33, &a3, TIM_gf_dt, &v85);
            a3 = v85;
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v75);
            v55 = v75;
        }
        else
        {
            AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v35);
            MATH_VecMultNormScalar(&a1, &v35, v57);
            a3 = a1;
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v103);
            v48 = v103.z * a3.z + v103.y * a3.y + a3.x * v103.x;
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v81);
            v98 = v81.x * v48;
            v99 = v81.y * v48;
            v100 = v81.z * v48;
            a3.x = a3.x - v98;
            a3.y = a3.y - v99;
            a3.z = a3.z - v100;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v49);
            v78 = TIM_gf_dt;
            AI_EvalFunc_MATHVecBlend_C(&v49, &a3, TIM_gf_dt, &v42);
            a3 = v42;
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v106);
            v55 = v106;
        }
        AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a3, &v55);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    if (v97->f_camera_dist > 160000.0)
    {
        if (v97->des_f_rot_speed == 0.0)
            v97->f_rot_speed = 4.0;
        else
            v97->f_rot_speed = v97->des_f_rot_speed;
        v97->f_virage_coef = 1.0;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v105);
        v82 = TIM_gf_dt;
        X = v97->f_rot_speed * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlend_C(&v105, &v97->v_new_sight, X, &v56);
        a3 = v56;
        v54.x = 0.0;
        v54.y = 0.0;
        v54.z = 1.0;
        v55.x = 0.0;
        v55.y = 0.0;
        v55.z = 1.0;
        goto LABEL_71;
    }
    if (v97->f_collision_rot_speed == 0.0)
    {
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v102);
        v97->f_virage_coef = v97->v_new_sight.z * v102.z + v97->v_new_sight.y * v102.y + v102.x * v97->v_new_sight.x;
        if (v97->f_virage_coef < 0.5)
            v14 = 0.5;
        else
            v14 = v97->f_virage_coef;
        v97->f_virage_coef = v14;
        if (v97->des_f_rot_speed == 0.0f)
        {
            v47 = TIM_gf_dt;
            v22 = 8.0f * TIM_gf_dt;
            if (v22 < 0.0)
                v22 = 0.0;
            if (v22 > 1.0)
                v22 = 1.0;
            v94 = v97->f_vitesse / v97->des_f_speed * 4.0f * v22 + v97->f_rot_speed * (1.0f - v22);
            v97->f_rot_speed = v94;
        }
        else
        {
            v87 = TIM_gf_dt;
            v23 = 8.0f * TIM_gf_dt;
            if (v23 < 0.0)
                v23 = 0.0;
            if (v23 > 1.0)
                v23 = 1.0;
            v38 = v97->des_f_rot_speed * v23 + v97->f_rot_speed * (1.0f - v23);
            v97->f_rot_speed = v38;
        }
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v101);
        v77 = TIM_gf_dt;
        Xb = v97->f_rot_speed * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlend_C(&v101, &v97->v_new_sight, Xb, &v60);
        a3 = v60;
    }
    else
    {
        v88 = AI_EvalFunc_MTHFloatSign_C(v97->f_collision_rot_speed);
        v41 = TIM_gf_dt;
        v24 = v88 * TIM_gf_dt;
        if (v97->f_collision_rot_speed > (double)v24)
            v15 = v88 * TIM_gf_dt;
        else
            v15 = v97->f_collision_rot_speed;
        v97->f_collision_rot_speed = v97->f_collision_rot_speed - v15;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v66);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v96);
        v71 = TIM_gf_dt;
        Xa = v97->f_collision_rot_speed * TIM_gf_dt;
        AI_EvalFunc_MATHVecRotate_C(&v66, &v96, Xa, &a4);
        a3 = a4;
    }
    if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x40))
    {
        v58 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 0x40);
        v21 = v58;
        if (v58)
        {
            if ((unsigned int)v58 <= 2)
                v21 = AI_MainActor[(uint32_t)v58 - 1];
            v1 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v21, 4);
        }
        else
        {
            v1 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 4);
        }
        v65 = v1;
        if (!v1)
        {
            v97->f_time_ground_col = TIM_gf_MainClock - TIM_gf_LockTime;
            AI_EvalFunc_COLNormalGet_C(AI_gpst_CurrentGameObject, 64, &v97->v_ground_normal);
        }
    }
    if (!v97->des_mode)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v97->f_time_ground_col, 0.30000001f))
        {
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v91);
            v108 = TIM_gf_dt;
            Xc = 4.0f * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlend_C(&v91, &v97->v_water_normal, Xc, &v45);
            v55 = v45;
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v67);
            v46 = TIM_gf_dt;
            Xd = 6.0f * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlend_C(&v67, &v97->v_ground_normal, Xd, &v61);
            v55 = v61;
        }
    LABEL_71:
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a3, &v55);
        return 1000;
    }
    v53 = v97->f_banking_coef;
    a3 = v97->v_new_sight;
    AI_EvalFunc_MATHVecSetHorzNormalize_C(&a3);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v25);
    v69 = v25;
    AI_EvalFunc_MATHVecSetHorzNormalize_C(&v69);
    v90.x = 0.0;
    v90.y = 0.0;
    v90.z = 1.0;
    AI_EvalFunc_MATHVecCrossProduct_C(&v90, &v69, &v28);
    v69 = v28;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v74);
    v44 = a3.z * v74.z + a3.y * v74.y + v74.x * a3.x;
    if (v44 < 0.0)
    {
        v79 = v69.z * a3.z + v69.y * a3.y + a3.x * v69.x;
        if (v79 <= 0.0)
        {
            v26.x = -v69.x;
            v26.y = -v69.y;
            v26.z = -v69.z;
            a3.x = -v69.x;
            a3.y = -v69.y;
            a3.z = -v69.z;
        }
        else
        {
            a3 = v69;
        }
    }
    v107 = v69.z * v97->v_new_sight.z + v69.y * v97->v_new_sight.y + v69.x * v97->v_new_sight.x;
    v29 = AI_EvalFunc_MTHFloatSign_C(v107);
    v13 = (float)fabs(v107);
    v104 = v13;
    v20 = v13 * 4.0f;
    if (v20 > 0.44999999)
        v12 = 0.44999999f;
    else
        v12 = v13 * 4.0f;
    v37 = v12;
    v107 = v29 * v12;
    if (v97->des_i_quartier_pietons)
    {
        v30 = TIM_gf_dt;
        v19 = 2.0f * TIM_gf_dt;
        if (v19 < 0.0)
            v19 = 0.0;
        if (v19 > 1.0)
            v19 = 1.0;
        v63 = v107 * 0.5f * v19 + v97->f_banking_coef * (1.0f - v19);
        v97->f_banking_coef = v63;
    }
    else
    {
        v31 = TIM_gf_dt;
        v18 = 2.0f * TIM_gf_dt;
        if (v18 < 0.0)
            v18 = 0.0;
        if (v18 > 1.0)
            v18 = 1.0;
        v86 = v107 * v18 + v97->f_banking_coef * (1.0f - v18);
        v97->f_banking_coef = v86;
    }
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v59);
    v83 = TIM_gf_dt;
    Xe = v97->f_rot_speed * TIM_gf_dt;
    AI_EvalFunc_MATHVecBlend_C(&v59, &a3, Xe, &v84);
    a3 = v84;
    v51 = v97->f_banking_coef;
    v11 = (float)fabs(v51);
    v95 = v11;
    v10 = (float)fabs(v53);
    v36 = v10;
    if (v95 >= (double)v10)
    {
        v39.x = 0.0;
        v39.y = 0.0;
        v39.z = 1.0;
        v32 = AI_EvalFunc_MTHFloatSign_C(v51);
        v27.x = v69.x * v32;
        v27.y = v69.y * v32;
        v27.z = v69.z * v32;
        v9 = (float)fabs(v51);
        v109 = v9;
        AI_EvalFunc_MATHVecBlend_C(&v39, &v27, v9, &v34);
        v55 = v34;
    }
    else
    {
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v80);
        v68.x = 0.0;
        v68.y = 0.0;
        v68.z = 1.0;
        v93 = TIM_gf_dt;
        Xf = 4.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlend_C(&v80, &v68, Xf, &v76);
        v55 = v76;
    }
    AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a3, &v55);
    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
    return 1000;
}

int CV_aircraft_CV_mode_deplacement()
{
    double v0; // st7
    double v1; // st7
    int v2; // eax
    BOOL v4; // eax
    tGameObjectMain* v5; // eax
    int v6; // eax
    tGameObjectMain* v7; // eax
    tGameObjectMain* v8; // eax
    tGameObjectMain* v9; // eax
    int v10; // eax
    tGameObjectMain* v11; // eax
    int v12; // eax
    tGameObjectMain* v13; // eax
    tGameObjectMain* v14; // eax
    int v15; // eax
    BOOL v16; // eax
    BOOL v17; // eax
    tGameObjectMain* v18; // eax
    tGameObjectMain* v19; // eax
    tGameObjectMain* v20; // eax
    int v21; // eax
    tGameObjectMain* v22; // eax
    int v23; // eax
    tGameObjectMain* v24; // eax
    BOOL v25; // eax
    tGameObjectMain* v26; // eax
    tGameObjectMain* v27; // eax
    int v28; // eax
    tGameObjectMain* v29; // eax
    int v30; // eax
    tGameObjectMain* v31; // eax
    BOOL v32; // eax
    int v33; // eax
    float v34; // [esp+8h] [ebp-A28h]
    float v35; // [esp+Ch] [ebp-A24h]
    float v36; // [esp+10h] [ebp-A20h]
    float v37; // [esp+14h] [ebp-A1Ch]
    float v38; // [esp+1Ch] [ebp-A14h]
    float v39; // [esp+20h] [ebp-A10h]
    tScriptStruct_CVAirCraft* v40; // [esp+24h] [ebp-A0Ch]
    Vec3* v41; // [esp+38h] [ebp-9F8h]
    float v42; // [esp+3Ch] [ebp-9F4h]
    tGameObjectMain* v43; // [esp+48h] [ebp-9E8h]
    tGameObjectMain* v44; // [esp+50h] [ebp-9E0h]
    tAI* v45; // [esp+54h] [ebp-9DCh]
    tGameObjectMain* v46; // [esp+58h] [ebp-9D8h]
    tGameObjectMain* v47; // [esp+5Ch] [ebp-9D4h]
    tAI* v48; // [esp+60h] [ebp-9D0h]
    tGameObjectMain* v49; // [esp+64h] [ebp-9CCh]
    tGameObjectMain* v50; // [esp+68h] [ebp-9C8h]
    tAI* v51; // [esp+6Ch] [ebp-9C4h]
    tGameObjectMain* v52; // [esp+70h] [ebp-9C0h]
    tGameObjectMain* v53; // [esp+74h] [ebp-9BCh]
    tGameObjectMain* v54; // [esp+7Ch] [ebp-9B4h]
    tGameObjectMain* v55; // [esp+84h] [ebp-9ACh]
    tGameObjectMain* v56; // [esp+8Ch] [ebp-9A4h]
    tGameObjectMain* v57; // [esp+94h] [ebp-99Ch]
    tGameObjectMain* v58; // [esp+9Ch] [ebp-994h]
    tGameObjectMain* v59; // [esp+A4h] [ebp-98Ch]
    tGameObjectMain* v60; // [esp+ACh] [ebp-984h]
    tGameObjectMain* v61; // [esp+B4h] [ebp-97Ch]
    tGameObjectMain* v62; // [esp+BCh] [ebp-974h]
    tGameObjectMain* v63; // [esp+C4h] [ebp-96Ch]
    tGameObjectMain* v64; // [esp+CCh] [ebp-964h]
    tGameObjectMain* v65; // [esp+D4h] [ebp-95Ch]
    tGameObjectMain* v66; // [esp+DCh] [ebp-954h]
    tGameObjectMain* v67; // [esp+E4h] [ebp-94Ch]
    tGameObjectMain* v68; // [esp+ECh] [ebp-944h]
    tGameObjectMain* v69; // [esp+F4h] [ebp-93Ch]
    tGameObjectMain* v70; // [esp+10Ch] [ebp-924h]
    tGameObjectMain* v71; // [esp+114h] [ebp-91Ch]
    tGameObjectMain* v72; // [esp+11Ch] [ebp-914h]
    tGameObjectMain* v73; // [esp+124h] [ebp-90Ch]
    tGameObjectMain* v74; // [esp+12Ch] [ebp-904h]
    tGameObjectMain* v75; // [esp+134h] [ebp-8FCh]
    tGameObjectMain* v76; // [esp+13Ch] [ebp-8F4h]
    tGameObjectMain* v77; // [esp+144h] [ebp-8ECh]
    tGameObjectMain* v78; // [esp+14Ch] [ebp-8E4h]
    tGameObjectMain* v79; // [esp+154h] [ebp-8DCh]
    tGameObjectMain* v80; // [esp+16Ch] [ebp-8C4h]
    tGameObjectMain* v81; // [esp+174h] [ebp-8BCh]
    tGameObjectMain* v82; // [esp+17Ch] [ebp-8B4h]
    tGameObjectMain* v83; // [esp+184h] [ebp-8ACh]
    tGameObjectMain* v84; // [esp+18Ch] [ebp-8A4h]
    tGameObjectMain* v85; // [esp+194h] [ebp-89Ch]
    tGameObjectMain* v86; // [esp+19Ch] [ebp-894h]
    tGameObjectMain* v87; // [esp+1A4h] [ebp-88Ch]
    tGameObjectMain* v88; // [esp+1B4h] [ebp-87Ch]
    tGameObjectMain* v89; // [esp+1C4h] [ebp-86Ch]
    tGameObjectMain* v90; // [esp+1ECh] [ebp-844h]
    tGameObjectMain* v91; // [esp+1FCh] [ebp-834h]
    tGameObjectMain* v92; // [esp+200h] [ebp-830h]
    tGameObjectMain* v93; // [esp+204h] [ebp-82Ch]
    tGameObjectMain* v94; // [esp+20Ch] [ebp-824h]
    tGameObjectMain* v95; // [esp+214h] [ebp-81Ch]
    tGameObjectMain* v96; // [esp+218h] [ebp-818h]
    tGameObjectMain* v97; // [esp+21Ch] [ebp-814h]
    tGameObjectMain* v98; // [esp+220h] [ebp-810h]
    tGameObjectMain* v99; // [esp+224h] [ebp-80Ch]
    tAI* v100; // [esp+228h] [ebp-808h]
    unsigned int v101; // [esp+22Ch] [ebp-804h]
    tGameObjectMain* v102; // [esp+230h] [ebp-800h]
    tGameObjectMain* v103; // [esp+234h] [ebp-7FCh]
    Vec3* v104; // [esp+244h] [ebp-7ECh]
    float v105; // [esp+26Ch] [ebp-7C4h]
    float v106; // [esp+280h] [ebp-7B0h]
    tGameObjectMain* v107; // [esp+288h] [ebp-7A8h]
    tGameObjectMain* v108; // [esp+290h] [ebp-7A0h]
    float v109; // [esp+294h] [ebp-79Ch]
    tGameObjectMain* v110; // [esp+2A4h] [ebp-78Ch]
    tGameObjectMain* v111; // [esp+2A8h] [ebp-788h]
    tGameObjectMain* v112; // [esp+2ACh] [ebp-784h]
    tGameObjectMain* v113; // [esp+2B4h] [ebp-77Ch]
    tGameObjectMain* v114; // [esp+2BCh] [ebp-774h]
    tGameObjectMain* v115; // [esp+2C0h] [ebp-770h]
    tGameObjectMain* v116; // [esp+2C4h] [ebp-76Ch]
    tGameObjectMain* v117; // [esp+2C8h] [ebp-768h]
    tGameObjectMain* v118; // [esp+2D0h] [ebp-760h]
    tGameObjectMain* v119; // [esp+2D8h] [ebp-758h]
    tGameObjectMain* v120; // [esp+2DCh] [ebp-754h]
    tGameObjectMain* v121; // [esp+2E0h] [ebp-750h]
    tGameObjectMain* v122; // [esp+2E4h] [ebp-74Ch]
    tGameObjectMain* v123; // [esp+2ECh] [ebp-744h]
    tAI* v124; // [esp+2F0h] [ebp-740h]
    unsigned int v125; // [esp+2F4h] [ebp-73Ch]
    tGameObjectMain* v126; // [esp+2F8h] [ebp-738h]
    tGameObjectMain* v127; // [esp+2FCh] [ebp-734h]
    tAI* v128; // [esp+300h] [ebp-730h]
    unsigned int v129; // [esp+304h] [ebp-72Ch]
    tGameObjectMain* v130; // [esp+308h] [ebp-728h]
    tGameObjectMain* v131; // [esp+30Ch] [ebp-724h]
    tGameObjectMain* v132; // [esp+314h] [ebp-71Ch]
    tAI* v133; // [esp+318h] [ebp-718h]
    unsigned int v134; // [esp+31Ch] [ebp-714h]
    tGameObjectMain* v135; // [esp+320h] [ebp-710h]
    tGameObjectMain* v136; // [esp+324h] [ebp-70Ch]
    tAI* v137; // [esp+328h] [ebp-708h]
    unsigned int v138; // [esp+32Ch] [ebp-704h]
    tGameObjectMain* v139; // [esp+330h] [ebp-700h]
    tGameObjectMain* v140; // [esp+334h] [ebp-6FCh]
    tGameObjectMain* v141; // [esp+33Ch] [ebp-6F4h]
    tGameObjectMain* v142; // [esp+340h] [ebp-6F0h]
    tAI* v143; // [esp+344h] [ebp-6ECh]
    tGameObjectMain* v144; // [esp+348h] [ebp-6E8h]
    tGameObjectMain* v145; // [esp+34Ch] [ebp-6E4h]
    tAI* v146; // [esp+350h] [ebp-6E0h]
    tGameObjectMain* v147; // [esp+354h] [ebp-6DCh]
    tGameObjectMain* v148; // [esp+358h] [ebp-6D8h]
    tAI* v149; // [esp+35Ch] [ebp-6D4h]
    tGameObjectMain* v150; // [esp+360h] [ebp-6D0h]
    tGameObjectMain* v151; // [esp+364h] [ebp-6CCh]
    tGameObjectMain* v152; // [esp+36Ch] [ebp-6C4h]
    tGameObjectMain* v153; // [esp+37Ch] [ebp-6B4h]
    tGameObjectMain* v154; // [esp+384h] [ebp-6ACh]
    tGameObjectMain* v155; // [esp+38Ch] [ebp-6A4h]
    Vec3 v156; // [esp+390h] [ebp-6A0h] BYREF
    Vec3 v157; // [esp+39Ch] [ebp-694h] BYREF
    float v158; // [esp+3B4h] [ebp-67Ch]
    float v159; // [esp+3B8h] [ebp-678h]
    float v160; // [esp+3BCh] [ebp-674h]
    Vec3 v161; // [esp+3C0h] [ebp-670h] BYREF
    float v162; // [esp+3D8h] [ebp-658h]
    float v163; // [esp+3DCh] [ebp-654h]
    float v164; // [esp+3E0h] [ebp-650h]
    Vec3 v165; // [esp+3E4h] [ebp-64Ch] BYREF
    Vec3 v166; // [esp+3F0h] [ebp-640h] BYREF
    Vec3 v167; // [esp+3FCh] [ebp-634h] BYREF
    Vec3 v168; // [esp+408h] [ebp-628h] BYREF
    int v169; // [esp+414h] [ebp-61Ch]
    int v170; // [esp+418h] [ebp-618h]
    BOOL v172; // [esp+420h] [ebp-610h]
    tAI* v173; // [esp+424h] [ebp-60Ch]
    tMessage v174; // [esp+428h] [ebp-608h] BYREF
    float v175; // [esp+45Ch] [ebp-5D4h]
    Vec3 a3[3]; // [esp+460h] [ebp-5D0h] BYREF
    Vec3 v177; // [esp+484h] [ebp-5ACh] BYREF
    Vec3 v178; // [esp+490h] [ebp-5A0h] BYREF
    Vec3 v179; // [esp+49Ch] [ebp-594h] BYREF
    Vec3 v180; // [esp+4A8h] [ebp-588h] BYREF
    Vec3 v181; // [esp+4B4h] [ebp-57Ch] BYREF
    float v182; // [esp+4C8h] [ebp-568h]
    int v186; // [esp+4E4h] [ebp-54Ch]
    Vec3 v187; // [esp+4ECh] [ebp-544h] BYREF
    Vec3 v188; // [esp+4F8h] [ebp-538h] BYREF
    Vec3 v189; // [esp+504h] [ebp-52Ch] BYREF
    void* v190; // [esp+510h] [ebp-520h]
    Vec3 v191; // [esp+52Ch] [ebp-504h] BYREF
    Vec3 v192; // [esp+544h] [ebp-4ECh]
    Vec3 v193; // [esp+55Ch] [ebp-4D4h] BYREF
    Vec3 v194; // [esp+574h] [ebp-4BCh] BYREF
    float v195; // [esp+580h] [ebp-4B0h]
    float v196; // [esp+584h] [ebp-4ACh]
    float v197; // [esp+588h] [ebp-4A8h]
    float v198; // [esp+58Ch] [ebp-4A4h]
    float v199; // [esp+590h] [ebp-4A0h]
    float v200; // [esp+594h] [ebp-49Ch]
    Vec3 v201; // [esp+5A4h] [ebp-48Ch] BYREF
    int v202; // [esp+5B0h] [ebp-480h]
    int v203; // [esp+5B4h] [ebp-47Ch]
    int v205; // [esp+5C4h] [ebp-46Ch]
    int v206; // [esp+5C8h] [ebp-468h]
    tMessage v207; // [esp+5CCh] [ebp-464h] BYREF
    int v208; // [esp+600h] [ebp-430h]
    Vec3 dest; // [esp+604h] [ebp-42Ch] BYREF
    Vec3 v210; // [esp+610h] [ebp-420h]
    Vec3 v211; // [esp+61Ch] [ebp-414h] BYREF
    float v212; // [esp+640h] [ebp-3F0h]
    float v213; // [esp+644h] [ebp-3ECh]
    float v214; // [esp+648h] [ebp-3E8h]
    int v215; // [esp+650h] [ebp-3E0h]
    int v216; // [esp+658h] [ebp-3D8h]
    int v217; // [esp+660h] [ebp-3D0h]
    BOOL v218; // [esp+664h] [ebp-3CCh]
    Vec3 v219; // [esp+670h] [ebp-3C0h]
    Vec3 v220; // [esp+688h] [ebp-3A8h] BYREF
    Vec3 v221; // [esp+694h] [ebp-39Ch]
    int v222; // [esp+6ACh] [ebp-384h]
    tGameObjectMain* v223; // [esp+6B0h] [ebp-380h]
    int v224; // [esp+6B8h] [ebp-378h]
    Vec3 v225; // [esp+6BCh] [ebp-374h] BYREF
    Vec3 v226; // [esp+6C8h] [ebp-368h] BYREF
    Vec3 v227; // [esp+6D4h] [ebp-35Ch] BYREF
    Vec3 v228; // [esp+6E0h] [ebp-350h] BYREF
    Vec3 v229; // [esp+6ECh] [ebp-344h] BYREF
    Vec3 v230; // [esp+6F8h] [ebp-338h] BYREF
    int v233; // [esp+71Ch] [ebp-314h]
    int v234; // [esp+720h] [ebp-310h]
    Vec3 v235; // [esp+724h] [ebp-30Ch] BYREF
    Vec3 v236; // [esp+730h] [ebp-300h] BYREF
    Vec3 v237; // [esp+73Ch] [ebp-2F4h] BYREF
    Vec3 v238; // [esp+748h] [ebp-2E8h] BYREF
    Vec3 pos; // [esp+754h] [ebp-2DCh] BYREF
    Vec3 v240; // [esp+760h] [ebp-2D0h] BYREF
    void* Src; // [esp+76Ch] [ebp-2C4h]
    BOOL v242; // [esp+770h] [ebp-2C0h]
    float v243; // [esp+778h] [ebp-2B8h]
    Vec3 v246; // [esp+78Ch] [ebp-2A4h] BYREF
    Vec3 v247; // [esp+798h] [ebp-298h] BYREF
    Vec3 v248; // [esp+7B0h] [ebp-280h] BYREF
    Vec3 v249; // [esp+7BCh] [ebp-274h]
    Vec3 v250; // [esp+7C8h] [ebp-268h] BYREF
    Vec3 v251; // [esp+7E0h] [ebp-250h]
    Vec3 v252; // [esp+7ECh] [ebp-244h] BYREF
    Vec3 v253; // [esp+804h] [ebp-22Ch] BYREF
    Vec3 v254; // [esp+810h] [ebp-220h] BYREF
    BOOL v255; // [esp+82Ch] [ebp-204h]
    Vec3 v256; // [esp+834h] [ebp-1FCh] BYREF
    Vec3 v257; // [esp+840h] [ebp-1F0h] BYREF
    Vec3 v258; // [esp+84Ch] [ebp-1E4h] BYREF
    tAI* v259; // [esp+85Ch] [ebp-1D4h]
    float v261; // [esp+864h] [ebp-1CCh]
    float v262; // [esp+86Ch] [ebp-1C4h]
    BOOL v263; // [esp+870h] [ebp-1C0h]
    float v264; // [esp+878h] [ebp-1B8h]
    Vec3 v265; // [esp+87Ch] [ebp-1B4h] BYREF
    Vec3 v; // [esp+894h] [ebp-19Ch] BYREF
    float v267; // [esp+8A0h] [ebp-190h]
    float v268; // [esp+8A4h] [ebp-18Ch]
    float v269; // [esp+8A8h] [ebp-188h]
    Vec3 v270; // [esp+8B8h] [ebp-178h] BYREF
    float v271; // [esp+8D0h] [ebp-160h]
    int v273; // [esp+8DCh] [ebp-154h]
    int v274; // [esp+8E0h] [ebp-150h]
    Vec3 v275; // [esp+8E4h] [ebp-14Ch] BYREF
    Vec3 v276; // [esp+8FCh] [ebp-134h] BYREF
    Vec3 v277; // [esp+908h] [ebp-128h] BYREF
    int v278; // [esp+914h] [ebp-11Ch]
    int v279; // [esp+91Ch] [ebp-114h]
    BOOL v280; // [esp+920h] [ebp-110h]
    Vec3 v281; // [esp+930h] [ebp-100h] BYREF
    Vec3 v282; // [esp+93Ch] [ebp-F4h] BYREF
    Vec3 v283; // [esp+948h] [ebp-E8h] BYREF
    tScriptStruct_CVAirCraft* v284; // [esp+96Ch] [ebp-C4h]
    float v285; // [esp+978h] [ebp-B8h]
    BOOL v287; // [esp+980h] [ebp-B0h]
    float v288; // [esp+98Ch] [ebp-A4h]
    BOOL v289; // [esp+990h] [ebp-A0h]
    Vec3 v290; // [esp+998h] [ebp-98h] BYREF
    BOOL v291; // [esp+9BCh] [ebp-74h]
    BOOL v292; // [esp+9C4h] [ebp-6Ch]
    float v294; // [esp+9D4h] [ebp-5Ch]
    Vec3 v295; // [esp+9E4h] [ebp-4Ch] BYREF
    Vec3 v296; // [esp+9F0h] [ebp-40h] BYREF
    Vec3 v297; // [esp+A08h] [ebp-28h] BYREF
    Vec3 v298; // [esp+A14h] [ebp-1Ch] BYREF
    tGameObjectMain* v299; // [esp+A24h] [ebp-Ch]

    v210.x = 1.0;
    v210.y = 0.0;
    v210.z = 0.0;
    v249.x = 1.0;
    v249.y = 0.0;
    v249.z = 0.0;
    v221.x = 1.0;
    v221.y = 0.0;
    v221.z = 0.0;
    v.x = 1.0;
    v.y = 0.0;
    v.z = 0.0;
    v189.x = 1.0;
    v189.y = 0.0;
    v189.z = 0.0;
    pos.x = 1.0;
    pos.y = 0.0;
    pos.z = 0.0;
    v240.x = 1.0;
    v240.y = 0.0;
    v240.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v40 = (tScriptStruct_CVAirCraft*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v40 = 0;
    v284 = v40;
    v169 = 0;
    v259 = 0;
    v299 = 0;
    v285 = 0.0;
    v223 = 0;
    v173 = 0;
    v262 = 0.0;
    v218 = 0;
    v233 = 0;
    v263 = 0;
    v288 = 0.0;
    v205 = 0;
    v274 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        CV_aircraft_CV_mode_deplacement_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &CV_aircraft_CV_mode_deplacement_GST_Global_gao_i_Domz_Attack_Boat_Shoot_NB,
            (void*)CV_aircraft_CV_mode_deplacement_GST_Global_gao,
            4644);
        CV_aircraft_CV_mode_deplacement__SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
        CV_aircraft_CV_mode_deplacement_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &CV_aircraft_CV_mode_deplacement_univVars_Perso_ID_passengers_list,
            (void*)CV_aircraft_CV_mode_deplacement_univVars,
            936);
        AI2C_ResolveBuffer(
            &CV_aircraft_CV_mode_deplacement_univVars_Perso_ID_actor,
            (void*)CV_aircraft_CV_mode_deplacement_univVars,
            988);
        AI2C_ResolveBuffer(
            &CV_aircraft_CV_mode_deplacement_univVars_Flag_speciaux_sauve,
            (void*)CV_aircraft_CV_mode_deplacement_univVars,
            4488);
        fct_CV_aircraft_CV_mode_deplacement_CV_aircraft_CV_exec_waterforce = (int)AI_ul_RealResolveOneRefFunction(0x7200198Eu);
        AI2C_ResolveBuffer(
            &CV_aircraft_CV_mode_deplacement_univVars_world_ID,
            (void*)CV_aircraft_CV_mode_deplacement_univVars,
            4);
        fct_CV_aircraft_CV_mode_deplacement_CV_aircraft_CV_exec_s_b = (int)AI_ul_RealResolveOneRefFunction(0x7200198Fu);
        fct_CV_aircraft_CV_mode_deplacement_CV_aircraft_CV_exec_check_pos = (int)AI_ul_RealResolveOneRefFunction(0x720019E7u);
        fct_CV_aircraft_CV_mode_deplacement_CV_aircraft_CV_exec_test_collision = (int)AI_ul_RealResolveOneRefFunction(0x72001A8Eu);
        fctvar_CV_aircraft_CV_mode_deplacement_CV_aircraft = AI_ul_RealResolveOneRef(42, 0x72001991u);
    }
    if (v284->i_current_mode)
    {
        v284->i_current_mode = 0;
        v284->f_time = TIM_gf_MainClock - TIM_gf_LockTime;
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0xC00);
    }
    v299 = AI_EvalFunc_VIEWGetObject_C(0);
    v155 = v299;
    if (v299)
    {
        if ((unsigned int)v299 <= 2)
            v155 = AI_MainActor[(uint32_t)v299 - 1];
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v155);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v284->f_camera_dist = (float)v0;
    v299 = *AI_MainActor;
    v154 = v299;
    if (v299)
    {
        if ((unsigned int)v299 <= 2)
            v154 = AI_MainActor[(uint32_t)v299 - 1];
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v154);
    }
    else
    {
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v284->f_aircraft_dist = (float)v1;
    Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v284->m_msg, Src, sizeof(v284->m_msg));
    while (v284->m_msg.msgID)
    {
        v223 = v284->m_msg.gao;
        if (v284->m_msg.msgID == 37)
        {
            if ((v284->m_msg.Int1 & 0x60) == 0)
                goto LABEL_165;
            if (!v284->i_snd_klaxon)
            {
                v284->i_snd_klaxon = 1;
                v284->f_snd_time_bip = TIM_gf_MainClock - TIM_gf_LockTime;
            }
            v153 = v223;
            if (v223)
            {
                if ((unsigned int)v223 <= 2)
                    v153 = AI_MainActor[(uint32_t)v223 - 1];
                v2 = AI_EvalFunc_OBJ_FlagsDesignGet_C(v153);
            }
            else
            {
                v2 = AI_EvalFunc_OBJ_FlagsDesignGet_C(0);
            }
            v202 = v2;
            if ((v2 & 1) != 0)
            {
                if (v284->m_msg.Int2)
                {
                    ++* (uint32_t*)CV_aircraft_CV_mode_deplacement_GST_Global_gao_i_Domz_Attack_Boat_Shoot_NB;
                    pos = v284->m_msg.v2;
                    AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v257);
                    v294 = TIM_gf_dt;
                    v212 = v257.x * TIM_gf_dt;
                    v213 = v257.y * TIM_gf_dt;
                    v214 = v257.z * TIM_gf_dt;
                    pos.x = pos.x + v212;
                    pos.y = pos.y + v213;
                    pos.z = pos.z + v214;
                    v259 = (tAI*)AI_EvalFunc_OBJDuplicate_C(
                        (tGameObjectMain*)CV_aircraft_CV_mode_deplacement__SFX_explosion_gao,
                        &pos);
                    v207.msgID = 1;
                    v207.Int1 = 30;
                    v207.f2 = 3.0;
                    v207.f1 = 1.0;
                    v207.v1.x = 1.5;
                    v207.v1.y = 0.0;
                    v207.v1.z = 1.0;
                    v152 = (tGameObjectMain*)v259;
                    if (v259)
                    {
                        if ((unsigned int)v259 <= 2)
                            v152 = AI_MainActor[(uint32_t)v259 - 1];
                        AI_EvalFunc_MSGSend_C(v152, &v207);
                    }
                    else
                    {
                        AI_EvalFunc_MSGSend_C(0, &v207);
                    }
                    if (v259 == WOR_gst_Universe.univAIInstance)
                    {
                        v149 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v151 = (tGameObjectMain*)v259;
                        if (v259)
                        {
                            if ((unsigned int)v259 <= 2)
                                v151 = AI_MainActor[(uint32_t)v259 - 1];
                            v150 = v151;
                        }
                        else
                        {
                            v150 = 0;
                        }
                        v149 = v150->extended->AIInstance;
                    }
                    ((tScriptStruct_SFXExplosion*)(v149->instanceVarDataCurrent))->des_perso_a_suivre = (int)AI_gpst_CurrentGameObject;
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v247);
                    MATH_SubstractVec(&dest, &pos, &v247);
                    if (v259 == WOR_gst_Universe.univAIInstance)
                    {
                        v146 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v148 = (tGameObjectMain*)v259;
                        if (v259)
                        {
                            if ((unsigned int)v259 <= 2)
                                v148 = AI_MainActor[(uint32_t)v259 - 1];
                            v147 = v148;
                        }
                        else
                        {
                            v147 = 0;
                        }
                        v146 = v147->extended->AIInstance;
                    }
                    AI_EvalFunc_MATHVecGlobalToLocal_C(
                        AI_gpst_CurrentGameObject,
                        &dest,
                        &((tScriptStruct_SFXExplosion*)(v146->instanceVarDataCurrent))->des_v_offset);
                    if (v259 == WOR_gst_Universe.univAIInstance)
                    {
                        v143 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v145 = (tGameObjectMain*)v259;
                        if (v259)
                        {
                            if ((unsigned int)v259 <= 2)
                                v145 = AI_MainActor[(uint32_t)v259 - 1];
                            v144 = v145;
                        }
                        else
                        {
                            v144 = 0;
                        }
                        v143 = v144->extended->AIInstance;
                    }
                    ((tScriptStruct_SFXExplosion*)(v143->instanceVarDataCurrent))->des_bone_a_suivre = 0;
                    v142 = (tGameObjectMain*)v284->des_o_acteur_texte;
                    if (v142)
                    {
                        if ((unsigned int)v142 <= 2)
                            v142 = AI_MainActor[(uint32_t)v142 - 1];
                        v141 = v142;
                    }
                    else
                    {
                        v141 = 0;
                    }
                    if (v141)
                    {
                        if (AI_EvalFunc_TIMEElapsed_C(v284->f_time_affiche_texte, 10.0))
                        {
                            if (!SND_i_IsTrackPlaying(1040))
                            {
                                v274 = -1;
                                if ((*(uint32_t*)(CV_aircraft_CV_mode_deplacement_univVars_Perso_ID_passengers_list + 44) & 1) != 0
                                    && !v284->i_last_txt_is_comp
                                    && v284->i_nb_laser_pris)
                                {
                                    v284->i_last_txt_is_comp = 1;
                                    v138 = v284->des_o_bison_fute;
                                    if ((tAI*)v284->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
                                    {
                                        v137 = WOR_gst_Universe.univAIInstance;
                                    }
                                    else
                                    {
                                        v140 = (tGameObjectMain*)v284->des_o_bison_fute;
                                        if (v138)
                                        {
                                            if (v138 <= 2)
                                                v140 = AI_MainActor[v138 - 1];
                                            v139 = v140;
                                        }
                                        else
                                        {
                                            v139 = 0;
                                        }
                                        v137 = v139->extended->AIInstance;
                                    }
                                    v274 = ((tScriptStruct_BisonFute*)(v137->instanceVarDataCurrent))->i_last_paf_txt_comp;
                                    v274 = AI_EvalFunc_MTHModulo_C(v274 + 1, v284->des_peyj_tir_nb);
                                    v134 = v284->des_o_bison_fute;
                                    if ((tAI*)v284->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
                                    {
                                        v133 = WOR_gst_Universe.univAIInstance;
                                    }
                                    else
                                    {
                                        v136 = (tGameObjectMain*)v284->des_o_bison_fute;
                                        if (v134)
                                        {
                                            if (v134 <= 2)
                                                v136 = AI_MainActor[v134 - 1];
                                            v135 = v136;
                                        }
                                        else
                                        {
                                            v135 = 0;
                                        }
                                        v133 = v135->extended->AIInstance;
                                    }
                                    ((tScriptStruct_BisonFute*)(v133->instanceVarDataCurrent))->i_last_paf_txt_comp = v274;
                                    v274 += v284->des_peyj_tir_index;
                                    v174.msgID = 30;
                                    v174.Int1 = v274;
                                    v174.Int2 = 100;
                                    v174.f2 = 3.0;
                                    v174.v1.x = 0.0;
                                    v174.v1.y = 0.0;
                                    v174.v1.z = 0.0;
                                    v174.v2.x = 0.0;
                                    v174.v2.y = 0.0;
                                    v174.v2.z = 0.0;
                                    v174.gao = 0;
                                    v174.gao2 = (tGameObjectMain*)v284->o_aircraft;
                                    v132 = (tGameObjectMain*)v284->des_o_acteur_texte;
                                    if (v132)
                                    {
                                        if ((unsigned int)v132 <= 2)
                                            v132 = AI_MainActor[(uint32_t)v132 - 1];
                                        AI_EvalFunc_MSGSend_C(v132, &v174);
                                    }
                                    else
                                    {
                                        AI_EvalFunc_MSGSend_C(0, &v174);
                                    }
                                }
                                if (v274 == -1)
                                {
                                    v284->i_last_txt_is_comp = 0;
                                    v129 = v284->des_o_bison_fute;
                                    if ((tAI*)v284->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
                                    {
                                        v128 = WOR_gst_Universe.univAIInstance;
                                    }
                                    else
                                    {
                                        v131 = (tGameObjectMain*)v284->des_o_bison_fute;
                                        if (v129)
                                        {
                                            if (v129 <= 2)
                                                v131 = AI_MainActor[v129 - 1];
                                            v130 = v131;
                                        }
                                        else
                                        {
                                            v130 = 0;
                                        }
                                        v128 = v130->extended->AIInstance;
                                    }
                                    v274 = ((tScriptStruct_BisonFute*)(v128->instanceVarDataCurrent))->i_last_paf_txt_sally;
                                    v274 = AI_EvalFunc_MTHModulo_C(v274 + 1, v284->des_tir_nb);
                                    v125 = v284->des_o_bison_fute;
                                    if ((tAI*)v284->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
                                    {
                                        v124 = WOR_gst_Universe.univAIInstance;
                                    }
                                    else
                                    {
                                        v127 = (tGameObjectMain*)v284->des_o_bison_fute;
                                        if (v125)
                                        {
                                            if (v125 <= 2)
                                                v127 = AI_MainActor[v125 - 1];
                                            v126 = v127;
                                        }
                                        else
                                        {
                                            v126 = 0;
                                        }
                                        v124 = v126->extended->AIInstance;
                                    }
                                    ((tScriptStruct_BisonFute*)(v124->instanceVarDataCurrent))->i_last_paf_txt_sally = v274;
                                    v274 += v284->des_tir_index;
                                    v174.msgID = 30;
                                    v174.Int1 = v274;
                                    v174.Int2 = 100;
                                    v174.f2 = 3.0;
                                    v174.v1.x = 0.0;
                                    v174.v1.y = 0.0;
                                    v174.v1.z = 0.0;
                                    v174.v2.x = 0.0;
                                    v174.v2.y = 0.0;
                                    v174.v2.z = 0.0;
                                    v174.gao = 0;
                                    v174.gao2 = (tGameObjectMain*)v284->o_aircraft;
                                    v123 = (tGameObjectMain*)v284->des_o_acteur_texte;
                                    if (v123)
                                    {
                                        if ((unsigned int)v123 <= 2)
                                            v123 = AI_MainActor[(uint32_t)v123 - 1];
                                        AI_EvalFunc_MSGSend_C(v123, &v174);
                                    }
                                    else
                                    {
                                        AI_EvalFunc_MSGSend_C(0, &v174);
                                    }
                                }
                            }
                        }
                    }
          ++v284->i_nb_laser_pris;
        }
        if (v284->i_nb_laser_pris >= 3)
        {
            v284->i_nb_laser_pris = 0;
            v122 = (tGameObjectMain*)v284->des_police;
            if (v122)
            {
                if ((unsigned int)v122 <= 2)
                    v122 = AI_MainActor[(uint32_t)v122 - 1];
                v121 = v122;
            }
            else
            {
                v121 = 0;
            }
            if (!v121)
                goto LABEL_133;
            v120 = v299;
            if (v299)
            {
                if ((unsigned int)v299 <= 2)
                    v120 = AI_MainActor[(uint32_t)v299 - 1];
                v119 = v120;
            }
            else
            {
                v119 = 0;
            }
            if (v119 == *(tGameObjectMain**)(CV_aircraft_CV_mode_deplacement_univVars_Perso_ID_actor + 44))
            {
                v118 = (tGameObjectMain*)v284->des_police;
                if (!v118)
                    goto LABEL_129;
                if ((unsigned int)v118 <= 2)
                    v118 = AI_MainActor[(uint32_t)v118 - 1];
                AI_EvalFunc_OBJ_CapaSet_C(v118, 0x100, 0);
            }
            else
            {
            LABEL_133:
                v117 = (tGameObjectMain*)v284->des_vaisseau_police;
                if (v117)
                {
                    if ((unsigned int)v117 <= 2)
                        v117 = AI_MainActor[(uint32_t)v117 - 1];
                    v116 = v117;
                }
                else
                {
                    v116 = 0;
                }
                if (v116)
                {
                    v115 = v299;
                    if (v299)
                    {
                        if ((unsigned int)v299 <= 2)
                            v115 = AI_MainActor[(uint32_t)v299 - 1];
                        v114 = v115;
                    }
                    else
                    {
                        v114 = 0;
                    }
                    if (v114 == *(tGameObjectMain**)(CV_aircraft_CV_mode_deplacement_univVars_Perso_ID_actor + 40))
                    {
                        v113 = (tGameObjectMain*)v284->des_vaisseau_police;
                        if (!v113)
                        {
                        LABEL_129:
                            AI_EvalFunc_OBJ_CapaSet_C(0, 0x100, 0);
                            goto LABEL_165;
                        }
                        if ((unsigned int)v113 <= 2)
                            v113 = AI_MainActor[(uint32_t)v113 - 1];
                        AI_EvalFunc_OBJ_CapaSet_C(v113, 0x100, 0);
                    }
                }
            }
        }
      }
      else if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x400))
      {
      AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x800, 0);
      }
      else
      {
      AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x400, 0);
      }
    }
    else if (v284->m_msg.msgID == 43)
    {
    if (v284->GFX_Halo_G != -1)
        AI_EvalFunc_GFXDel_C_CURRENT(v284->GFX_Halo_G);
    if (v284->GFX_Halo_D != -1)
        AI_EvalFunc_GFXDel_C_CURRENT(v284->GFX_Halo_D);
    if (v284->GFX_Smoke != -1)
        AI_EvalFunc_GFXDel_C_CURRENT(v284->GFX_Smoke);
    if (v284->GFX_Fire_Smoke != -1)
        AI_EvalFunc_GFXDel_C_CURRENT(v284->GFX_Fire_Smoke);
    if (v284->GFX_Flammes != -1)
        AI_EvalFunc_GFXDel_C_CURRENT(v284->GFX_Flammes);
    if (v284->m_msg.Int1)
    {
        AI_EvalFunc_OBJReinit_C(AI_gpst_CurrentGameObject, 1);
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 4;
    }
    }
LABEL_165:
v190 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
memcpy(&v284->m_msg, v190, sizeof(v284->m_msg));
  }
  v218 = 0;
  v112 = (tGameObjectMain*)v284->des_o_acteur_texte;
  if (v112)
  {
      if ((unsigned int)v112 <= 2)
          v112 = AI_MainActor[(uint32_t)v112 - 1];
      v111 = v112;
  }
  else
  {
      v111 = 0;
  }
  if (v111)
  {
      v110 = (tGameObjectMain*)v284->des_o_acteur_texte;
      if (v110)
      {
          if ((unsigned int)v110 <= 2)
              v110 = AI_MainActor[(uint32_t)v110 - 1];
          v4 = AI_EvalFunc_OBJ_CapaTest_C(v110, 0x100);
      }
      else
      {
          v4 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x100);
      }
      v292 = v4;
      if (v4)
          v284->f_time_affiche_texte = TIM_gf_MainClock - TIM_gf_LockTime;
  }
  if (!v284->i_flag_on_coule)
  {
      if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x800))
      {
          if (AI_EvalFunc_TIMEElapsed_C(v284->f_time_no_fire, 20.0))
          {
              AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x1000, 0);
              v284->i_flag_on_coule = 1;
              AI_EvalFunc_OBJGetPos_C_CURRENT(&v250);
              pos = v250;
              AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v290);
              v109 = (v284->des_f_Y_max_offset - v284->des_f_Y_min_offset) * 0.5f;
              v198 = v290.x * v109;
              v199 = v290.y * v109;
              v200 = v290.z * v109;
              pos.x = pos.x + v198;
              pos.y = pos.y + v199;
              pos.z = pos.z + v200;
              AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v298);
              v267 = v298.x * 3.0f;
              v268 = v298.y * 3.0f;
              v269 = v298.z * 3.0f;
              pos.x = pos.x + v267;
              pos.y = pos.y + v268;
              pos.z = pos.z + v269;
              v259 = (tAI*)AI_EvalFunc_OBJDuplicate_C(
                  (tGameObjectMain*)CV_aircraft_CV_mode_deplacement__SFX_explosion_gao,
                  &pos);
              v207.msgID = 1;
              v207.Int1 = 36;
              v207.f2 = 4.0;
              v207.f1 = 0.0;
              v207.v1.x = 0.5;
              v207.v1.y = 0.0;
              v207.v1.z = 0.0;
              v108 = (tGameObjectMain*)v259;
              if (v259)
              {
                  if ((unsigned int)v259 <= 2)
                      v108 = AI_MainActor[(uint32_t)v259 - 1];
                  AI_EvalFunc_MSGSend_C(v108, &v207);
              }
              else
              {
                  AI_EvalFunc_MSGSend_C(0, &v207);
              }
              v284->o_sfx_explosion = (int)v259;
              v107 = (tGameObjectMain*)v284->o_sfx_explosion;
              if (v107)
              {
                  if ((unsigned int)v107 <= 2)
                      v107 = AI_MainActor[(uint32_t)v107 - 1];
                  AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, v107, &v284->o_sfx_explosion);
              }
              else
              {
                  AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, 0, &v284->o_sfx_explosion);
              }
          }
      }
  }
  if (v284->i_flag_on_coule && v284->f_camera_dist > 1000000.0)
  {
      v280 = AI_EvalFunc_DYNOff_C(AI_gpst_CurrentGameObject);
      *AI_gppst_CurrentJumpNode = (tAINode*)-1;
      return 1000;
  }
  if (!AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 4))
  {
      v255 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
      v234 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x1039004, 0);
      v270.x = 0.0;
      v270.y = 0.0;
      v270.z = 0.0;
      v206 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v270);
      v229.x = 0.0;
      v229.y = 0.0;
      v229.z = -20.0;
      AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v229);
      v295.x = 0.0;
      v295.y = 0.0;
      v295.z = 20.0;
      AI_EvalFunc_DYNTractionSet_C_CURRENT(&v295);
  }
  if (v284->f_camera_dist >= 160000.0)
  {
      if (v284->f_water_Z == -1000000000.0f)
      {
          AI_EvalFunc_OBJGetPos_C_CURRENT(&v226);
          AI_EvalFunc_WTR_GetDif_C(&v226, &v161);
          v284->f_water_Z = v161.z;
          v284->v_water_normal.x = 0.0;
          v284->v_water_normal.y = 0.0;
          v284->v_water_normal.z = 1.0;
      }
  }
  else
  {
      v262 = v284->des_f_Y_max_offset - v284->des_f_Y_min_offset;
      v262 = v262 / 5.0f;
      AI_EvalFunc_OBJGetPos_C_CURRENT(&v277);
      AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v191);
      v106 = v284->des_f_Y_min_offset;
      v275.x = v191.x * v106;
      v275.y = v191.y * v106;
      v275.z = v191.z * v106;
      MATH_VecAddVec(&v297, &v277, &v275);
      v240 = v297;
      AI_EvalFunc_OBJGetPos_C_CURRENT(&v194);
      v = v194;
      AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v238);
      v105 = v284->des_f_Y_max_offset;
      v195 = v238.x * v105;
      v196 = v238.y * v105;
      v197 = v238.z * v105;
      v.x = v.x + v195;
      v.y = v.y + v196;
      v.z = v.z + v197;
      if (v284->i_flag_water_picking_side)
      {
          v284->i_flag_water_picking_side = 0;
          AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v228);
          v162 = v228.x * v262;
          v163 = v228.y * v262;
          v164 = v228.z * v262;
          v.x = v.x + v162;
          v.y = v.y + v163;
          v.z = v.z + v164;
      }
      else
      {
          v284->i_flag_water_picking_side = 1;
          AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v276);
          v158 = v276.x * v262;
          v159 = v276.y * v262;
          v160 = v276.z * v262;
          v.x = v.x - v158;
          v.y = v.y - v159;
          v.z = v.z - v160;
      }
      v251.x = 0.0;
      v251.y = 0.0;
      v251.z = 50.0;
      v192.x = 0.0;
      v192.y = 0.0;
      v192.z = 50.0;
      AI_EvalFunc_WTR_GetDif_C(&v240, &v248);
      v240 = v248;
      if ((*(uint32_t*)&v248.z) == 0xCE6E6B28)
      {
          v284->v_water_normal.x = 0.0;
          v284->v_water_normal.y = 0.0;
          v284->v_water_normal.z = 1.0;
      }
      else
      {
          AI_EvalFunc_WTR_GetDif_C(&v, &v201);
          v = v201;
          MATH_VecAddVec(&v168, &v240, &v);
          v210 = v168;
          v240.x = v240.x - v.x;
          v240.y = v240.y - v.y;
          v240.z = v240.z - v.z;
          if (v284->i_flag_water_picking_side)
              AI_EvalFunc_MATHVecCrossProduct_C(&v240, &v284->v_water_last_diff, &v284->v_water_normal);
          else
              AI_EvalFunc_MATHVecCrossProduct_C(&v284->v_water_last_diff, &v240, &v284->v_water_normal);
          MATH_NormalizeEqualVector(&v284->v_water_normal);
          if (v284->v_water_normal.z < 0.0)
          {
              v104 = &v284->v_water_normal;
              v284->v_water_normal.x = v284->v_water_normal.x * -1.0f;
              v104->y = v104->y * -1.0f;
              v104->z = v104->z * -1.0f;
          }
          v284->v_water_last_diff = v240;
          v284->f_water_Z = v210.z / 2.0f;
      }
      if (v284->des_mode == 1)
      {
          if (!v284->i_limo_categ)
              v284->f_water_Z = (float)v284->i_water_z_categ0;
          if (v284->i_limo_categ == 1)
              v284->f_water_Z = (float)v284->i_water_z_categ1;
      }
      AI_EvalFunc_OBJGetPos_C_CURRENT(&v165);
      if (v284->f_water_Z + 0.5 > v165.z)
          AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
  }
  v218 = AI_EvalFunc_ARRBitTest_C((int*)(CV_aircraft_CV_mode_deplacement_univVars_Flag_speciaux_sauve + 4), 2718);
  if (v284->n_fuite)
  {
      v101 = v284->des_o_bison_fute;
      if ((tAI*)v284->des_o_bison_fute == WOR_gst_Universe.univAIInstance)
      {
          v100 = WOR_gst_Universe.univAIInstance;
      }
      else
      {
          v103 = (tGameObjectMain*)v284->des_o_bison_fute;
          if (v101)
          {
              if (v101 <= 2)
                  v103 = AI_MainActor[v101 - 1];
              v102 = v103;
          }
          else
          {
              v102 = 0;
          }
          v100 = v102->extended->AIInstance;
      }
      v284->o_monstros = ((tScriptStruct_BisonFute*)(v100->instanceVarDataCurrent))->o_monstros;
      if (!v218)
          goto LABEL_291;
      v99 = (tGameObjectMain*)v284->o_monstros;
      if (v99)
      {
          if ((unsigned int)v99 <= 2)
              v99 = AI_MainActor[(uint32_t)v99 - 1];
          v98 = v99;
      }
      else
      {
          v98 = 0;
      }
      if (v98)
      {
          v284->i_flag_fuite = 1;
          v97 = (tGameObjectMain*)v284->o_fuite_wp;
          if (v97)
          {
              if ((unsigned int)v97 <= 2)
                  v97 = AI_MainActor[(uint32_t)v97 - 1];
              v96 = v97;
          }
          else
          {
              v96 = 0;
          }
          if (!v96)
          {
              v5 = AI_EvalFunc_WAYNearestWPOfObj_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_fuite, 128, 0, 0);
              v284->o_fuite_wp = (int)v5;
          }
          v95 = (tGameObjectMain*)v284->o_fuite_wp;
          if (v95)
          {
              if ((unsigned int)v95 <= 2)
                  v95 = AI_MainActor[(uint32_t)v95 - 1];
              v6 = AI_EvalFunc_COLPivotBVCollide_C(AI_gpst_CurrentGameObject, v95);
          }
          else
          {
              v6 = AI_EvalFunc_COLPivotBVCollide_C(AI_gpst_CurrentGameObject, 0);
          }
          v273 = v6;
          if (v6)
          {
              v94 = (tGameObjectMain*)v284->o_fuite_wp;
              if (v94)
              {
                  if ((unsigned int)v94 <= 2)
                      v94 = AI_MainActor[(uint32_t)v94 - 1];
                  v7 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_fuite, v94, 1, 0);
              }
              else
              {
                  v7 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_fuite, 0, 1, 0);
              }
              v284->o_fuite_wp = (int)v7;
              v93 = (tGameObjectMain*)v284->o_fuite_wp;
              if (v93)
              {
                  if ((unsigned int)v93 <= 2)
                      v93 = AI_MainActor[(uint32_t)v93 - 1];
                  v92 = v93;
              }
              else
              {
                  v92 = 0;
              }
              if (!v92)
              {
                  AI_EvalFunc_OBJReinit_C(AI_gpst_CurrentGameObject, 1);
                  AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
                  AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
                  AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
                  *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                  return 4;
              }
          }
          v91 = (tGameObjectMain*)v284->o_monstros;
          if (v91)
          {
              if ((unsigned int)v91 <= 2)
                  v91 = AI_MainActor[(uint32_t)v91 - 1];
              AI_EvalFunc_OBJGetPos_C(v91, a3);
          }
          else
          {
              AI_EvalFunc_OBJGetPos_C(0, a3);
          }
          AI_EvalFunc_OBJGetPos_C_CURRENT(&v256);
          MATH_SubstractVec(&v237, a3, &v256);
          v240.x = v237.x;
          v240.y = v237.y;
          v240.z = 0.0;
          v90 = (tGameObjectMain*)v284->o_fuite_wp;
          if (v90)
          {
              if ((unsigned int)v90 <= 2)
                  v90 = AI_MainActor[(uint32_t)v90 - 1];
              AI_EvalFunc_OBJGetPos_C(v90, &v284->v_dest_pos);
          }
          else
          {
              AI_EvalFunc_OBJGetPos_C(0, &v284->v_dest_pos);
          }
          v262 = MATH_VecLen(&v240);
          if (v262 != 0.0)
          {
              AI_EvalFunc_OBJGetPos_C_CURRENT(&v187);
              MATH_SubstractVec(&v157, &v284->v_dest_pos, &v187);
              v = v157;
              v175 = v157.z * v240.z + v157.y * v240.y + v240.x * v157.x;
              if (v175 > 0.0)
              {
                  v296.x = 0.0;
                  v296.y = 0.0;
                  v296.z = 1.0;
                  AI_EvalFunc_MATHVecCrossProduct_C(&v, &v296, &v180);
                  v = v180;
                  MATH_NormalizeEqualVector(&v);
                  v264 = v.z * v240.z + v.y * v240.y + v240.x * v.x;
                  v39 = (float)fabs(v264);
                  v271 = v39;
                  if (v39 < 400.0)
                  {
                      v182 = v.z * v240.z + v.y * v240.y + v240.x * v.x;
                      if (v182 <= 0.0)
                      {
                          v88 = (tGameObjectMain*)v284->o_monstros;
                          if (v88)
                          {
                              if ((unsigned int)v88 <= 2)
                                  v88 = AI_MainActor[(uint32_t)v88 - 1];
                              AI_EvalFunc_OBJGetPos_C(v88, &v167);
                          }
                          else
                          {
                              AI_EvalFunc_OBJGetPos_C(0, &v167);
                          }
                          v166.x = v.x * 400.0f;
                          v166.y = v.y * 400.0f;
                          v166.z = v.z * 400.0f;
                          MATH_VecAddVec(&v236, &v167, &v166);
                          v284->v_dest_pos = v236;
                      }
                      else
                      {
                          v89 = (tGameObjectMain*)v284->o_monstros;
                          if (v89)
                          {
                              if ((unsigned int)v89 <= 2)
                                  v89 = AI_MainActor[(uint32_t)v89 - 1];
                              AI_EvalFunc_OBJGetPos_C(v89, &v188);
                          }
                          else
                          {
                              AI_EvalFunc_OBJGetPos_C(0, &v188);
                          }
                          v220.x = v.x * 400.0f;
                          v220.y = v.y * 400.0f;
                          v220.z = v.z * 400.0f;
                          MATH_SubstractVec(&v235, &v188, &v220);
                          v284->v_dest_pos = v235;
                      }
                      v87 = (tGameObjectMain*)v284->o_fuite_wp;
                      if (v87)
                      {
                          if ((unsigned int)v87 <= 2)
                              v87 = AI_MainActor[(uint32_t)v87 - 1];
                          AI_EvalFunc_OBJGetPos_C(v87, &v225);
                      }
                      else
                      {
                          AI_EvalFunc_OBJGetPos_C(0, &v225);
                      }
                      v284->v_dest_pos.z = v225.z;
                  }
              }
          }
      }
      else
      {
      LABEL_291:
          v284->o_fuite_wp = 0;
          if (v284->i_flag_fuite)
          {
              v284->i_flag_fuite = 0;
              v8 = AI_EvalFunc_WAYNearestWPOfObj_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, 128, 0, 0);
              v284->o_current_wp = (int)v8;
              v86 = (tGameObjectMain*)v284->o_current_wp;
              if (v86)
              {
                  if ((unsigned int)v86 <= 2)
                      v86 = AI_MainActor[(uint32_t)v86 - 1];
                  v9 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, v86, 3, 0);
              }
              else
              {
                  v9 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, 0, 3, 0);
              }
              v284->o_next_wp = (int)v9;
              v85 = (tGameObjectMain*)v284->o_next_wp;
              if (v85)
              {
                  if ((unsigned int)v85 <= 2)
                      v85 = AI_MainActor[(uint32_t)v85 - 1];
                  v10 = AI_EvalFunc_OBJ_CapaGet_C(v85);
              }
              else
              {
                  v10 = AI_EvalFunc_OBJ_CapaGet_C(0);
              }
              v169 = v10;
              if (v284->des_type)
              {
                  v233 = 0;
                  while ((v169 & 0x400) != 0 && v233 < 5)
                  {
                      ++v233;
                      v84 = (tGameObjectMain*)v284->o_current_wp;
                      if (v84)
                      {
                          if ((unsigned int)v84 <= 2)
                              v84 = AI_MainActor[(uint32_t)v84 - 1];
                          v11 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, v84, 1, 0);
                      }
                      else
                      {
                          v11 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, 0, 1, 0);
                      }
                      v284->o_next_wp = (int)v11;
                      v83 = (tGameObjectMain*)v284->o_next_wp;
                      if (v83)
                      {
                          if ((unsigned int)v83 <= 2)
                              v83 = AI_MainActor[(uint32_t)v83 - 1];
                          v12 = AI_EvalFunc_OBJ_CapaGet_C(v83);
                      }
                      else
                      {
                          v12 = AI_EvalFunc_OBJ_CapaGet_C(0);
                      }
                      v169 = v12;
                  }
                  if (v233 == 5 && (v169 & 0x400) != 0)
                  {
                      v82 = (tGameObjectMain*)v284->o_current_wp;
                      if (v82)
                      {
                          if ((unsigned int)v82 <= 2)
                              v82 = AI_MainActor[(uint32_t)v82 - 1];
                          v13 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, v82, 3, 0);
                      }
                      else
                      {
                          v13 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, 0, 3, 0);
                      }
                      v284->o_next_wp = (int)v13;
                  }
              }
              if (v284->des_rand_bv_pos)
              {
                  v81 = (tGameObjectMain*)v284->o_next_wp;
                  if (v81)
                  {
                      if ((unsigned int)v81 <= 2)
                          v81 = AI_MainActor[(uint32_t)v81 - 1];
                      AI_EvalFunc_BVRandomPosGet_C(v81, 0, &v284->v_dest_pos);
                  }
                  else
                  {
                      AI_EvalFunc_BVRandomPosGet_C(0, 0, &v284->v_dest_pos);
                  }
              }
              else
              {
                  v80 = (tGameObjectMain*)v284->o_next_wp;
                  if (v80)
                  {
                      if ((unsigned int)v80 <= 2)
                          v80 = AI_MainActor[(uint32_t)v80 - 1];
                      AI_EvalFunc_OBJGetPos_C(v80, &v284->v_dest_pos);
                  }
                  else
                  {
                      AI_EvalFunc_OBJGetPos_C(0, &v284->v_dest_pos);
                  }
              }
          }
      }
  }
  if (v284->f_water_Z == -1000000000.0)
  {
      AI_EvalFunc_COLPosGet_C(AI_gpst_CurrentGameObject, 0, &v230);
      v221 = v230;
      v288 = AI_EvalFunc_COLSizeGet_C(AI_gpst_CurrentGameObject, 0);
      v205 = AI_EvalFunc_COLGMatReportGet_C(AI_gpst_CurrentGameObject, 16);
      if (v205 != -1)
      {
          AI_EvalFunc_COLPointGet_C(AI_gpst_CurrentGameObject, v205 + 0x80000000, &v265);
          v249 = v265;
          v284->f_water_Z = v265.z;
          AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 256);
      }
  }
  if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(512))
  {
      AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 512);
      v14 = AI_EvalFunc_WAYNearestWPOfObj_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, 0x80, 0, 0);
      v284->o_next_wp = (int)v14;
      if (v284->des_rand_bv_pos)
      {
          v79 = (tGameObjectMain*)v284->o_next_wp;
          if (v79)
          {
              if ((unsigned int)v79 <= 2)
                  v79 = AI_MainActor[(uint32_t)v79 - 1];
              AI_EvalFunc_BVRandomPosGet_C(v79, 0, &v284->v_dest_pos);
          }
          else
          {
              AI_EvalFunc_BVRandomPosGet_C(0, 0, &v284->v_dest_pos);
          }
      }
      else
      {
          v78 = (tGameObjectMain*)v284->o_next_wp;
          if (v78)
          {
              if ((unsigned int)v78 <= 2)
                  v78 = AI_MainActor[(uint32_t)v78 - 1];
              AI_EvalFunc_OBJGetPos_C(v78, &v284->v_dest_pos);
          }
          else
          {
              AI_EvalFunc_OBJGetPos_C(0, &v284->v_dest_pos);
          }
      }
      AI_EvalFunc_OBJGetPos_C_CURRENT(&v227);
      MATH_SubstractVec(&v193, &v284->v_dest_pos, &v227);
      AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v156);
      AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v193, &v156);
  }
  if (!v218 || v284->des_i_v_hyllis)
  {
      v77 = (tGameObjectMain*)v284->o_next_wp;
      if (v77)
      {
          if ((unsigned int)v77 <= 2)
              v77 = AI_MainActor[(uint32_t)v77 - 1];
          v15 = AI_EvalFunc_COLPivotBVCollide_C(AI_gpst_CurrentGameObject, v77);
      }
      else
      {
          v15 = AI_EvalFunc_COLPivotBVCollide_C(AI_gpst_CurrentGameObject, 0);
      }
      v279 = v15;
      if (v15)
      {
          v76 = (tGameObjectMain*)v284->o_next_wp;
          if (v76)
          {
              if ((unsigned int)v76 <= 2)
                  v76 = AI_MainActor[(uint32_t)v76 - 1];
              v16 = AI_EvalFunc_OBJ_CapaTest_C(v76, 1);
          }
          else
          {
              v16 = AI_EvalFunc_OBJ_CapaTest_C(0, 1);
          }
          v242 = v16;
          if (v16)
              v284->i_num_mat = AI_EvalFunc_MATHRandInt_C(0, 4);
          v284->f_rot_speed = 0.0;
          v75 = (tGameObjectMain*)v284->o_next_wp;
          if (v75)
          {
              if ((unsigned int)v75 <= 2)
                  v75 = AI_MainActor[(uint32_t)v75 - 1];
              v17 = AI_EvalFunc_OBJ_CapaTest_C(v75, 0x100);
          }
          else
          {
              v17 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x100);
          }
          v172 = v17;
          if (v17)
          {
              AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0xC00);
              v74 = (tGameObjectMain*)v284->o_next_wp;
              if (v74)
              {
                  if ((unsigned int)v74 <= 2)
                      v74 = AI_MainActor[(uint32_t)v74 - 1];
                  v18 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, v74, 1, 0);
              }
              else
              {
                  v18 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, 0, 1, 0);
              }
              v284->o_current_wp = (int)v18;
              v73 = (tGameObjectMain*)v284->o_current_wp;
              if (v73)
              {
                  if ((unsigned int)v73 <= 2)
                      v73 = AI_MainActor[(uint32_t)v73 - 1];
                  v19 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, v73, 1, 0);
              }
              else
              {
                  v19 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, 0, 1, 0);
              }
              v284->o_next_wp = (int)v19;
              if (v284->des_rand_bv_pos)
              {
                  v72 = (tGameObjectMain*)v284->o_next_wp;
                  if (v72)
                  {
                      if ((unsigned int)v72 <= 2)
                          v72 = AI_MainActor[(uint32_t)v72 - 1];
                      AI_EvalFunc_BVRandomPosGet_C(v72, 0, &v284->v_dest_pos);
                  }
                  else
                  {
                      AI_EvalFunc_BVRandomPosGet_C(0, 0, &v284->v_dest_pos);
                  }
              }
              else
              {
                  v71 = (tGameObjectMain*)v284->o_next_wp;
                  if (v71)
                  {
                      if ((unsigned int)v71 <= 2)
                          v71 = AI_MainActor[(uint32_t)v71 - 1];
                      AI_EvalFunc_OBJGetPos_C(v71, &v284->v_dest_pos);
                  }
                  else
                  {
                      AI_EvalFunc_OBJGetPos_C(0, &v284->v_dest_pos);
                  }
              }
              v70 = (tGameObjectMain*)v284->o_current_wp;
              if (v70)
              {
                  if ((unsigned int)v70 <= 2)
                      v70 = AI_MainActor[(uint32_t)v70 - 1];
                  AI_EvalFunc_OBJGetPos_C(v70, &v181);
              }
              else
              {
                  AI_EvalFunc_OBJGetPos_C(0, &v181);
              }
              v240 = v181;
              AI_EvalFunc_WTR_GetDif_C(&v240, &v282);
              pos = v282;
              if (v282.z > -1000000000.0)
                  v284->f_water_Z = pos.z;
              v240.z = v284->f_water_Z - 1.0f;
              AI_EvalFunc_OBJSetPos_C_CURRENT(&v240);
              AI_EvalFunc_OBJGetPos_C_CURRENT(&v253);
              MATH_SubstractVec(&v211, &v284->v_dest_pos, &v253);
              v178.x = 0.0;
              v178.y = 0.0;
              v178.z = 1.0;
              AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v211, &v178);
              AI_EvalFunc_OBJGetPos_C_CURRENT(&v281);
              AI_EvalFunc_COLStartMatrixSet_C(AI_gpst_CurrentGameObject, &v281);
              v252.x = 0.0;
              v252.y = 0.0;
              v252.z = 0.0;
              v217 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v252);
              v177.x = 0.0;
              v177.y = 0.0;
              v177.z = 0.0;
              AI_EvalFunc_DYNTractionSet_C_CURRENT(&v177);
              if (v284->GFX_Airtrace != -1)
              {
                  AI_EvalFunc_GFXDel_C_CURRENT(v284->GFX_Airtrace);
                  v284->GFX_Airtrace = -1;
              }
              *AI_gppst_CurrentJumpNode = (tAINode*)-1;
              return 1000;
          }
          v284->o_current_wp = v284->o_next_wp;
          v69 = (tGameObjectMain*)v284->o_current_wp;
          if (v69)
          {
              if ((unsigned int)v69 <= 2)
                  v69 = AI_MainActor[(uint32_t)v69 - 1];
              v20 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, v69, 1, 0);
          }
          else
          {
              v20 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, 0, 1, 0);
          }
          v284->o_next_wp = (int)v20;
          v68 = (tGameObjectMain*)v284->o_next_wp;
          if (v68)
          {
              if ((unsigned int)v68 <= 2)
                  v68 = AI_MainActor[(uint32_t)v68 - 1];
              v21 = AI_EvalFunc_OBJ_CapaGet_C(v68);
          }
          else
          {
              v21 = AI_EvalFunc_OBJ_CapaGet_C(0);
          }
          v169 = v21;
          if (v284->des_type)
          {
              v233 = 0;
              while ((v169 & 0x400) != 0 && v233 < 5)
              {
                  ++v233;
                  v67 = (tGameObjectMain*)v284->o_current_wp;
                  if (v67)
                  {
                      if ((unsigned int)v67 <= 2)
                          v67 = AI_MainActor[(uint32_t)v67 - 1];
                      v22 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, v67, 1, 0);
                  }
                  else
                  {
                      v22 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, 0, 1, 0);
                  }
                  v284->o_next_wp = (int)v22;
                  v66 = (tGameObjectMain*)v284->o_next_wp;
                  if (v66)
                  {
                      if ((unsigned int)v66 <= 2)
                          v66 = AI_MainActor[(uint32_t)v66 - 1];
                      v23 = AI_EvalFunc_OBJ_CapaGet_C(v66);
                  }
                  else
                  {
                      v23 = AI_EvalFunc_OBJ_CapaGet_C(0);
                  }
                  v169 = v23;
              }
              if (v233 == 5 && (v169 & 0x400) != 0)
              {
                  v65 = (tGameObjectMain*)v284->o_current_wp;
                  if (v65)
                  {
                      if ((unsigned int)v65 <= 2)
                          v65 = AI_MainActor[(uint32_t)v65 - 1];
                      v24 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, v65, 3, 0);
                  }
                  else
                  {
                      v24 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, 0, 3, 0);
                  }
                  v284->o_next_wp = (int)v24;
              }
          }
          if (v284->des_rand_bv_pos)
          {
              v64 = (tGameObjectMain*)v284->o_next_wp;
              if (v64)
              {
                  if ((unsigned int)v64 <= 2)
                      v64 = AI_MainActor[(uint32_t)v64 - 1];
                  AI_EvalFunc_BVRandomPosGet_C(v64, 0, &v284->v_dest_pos);
              }
              else
              {
                  AI_EvalFunc_BVRandomPosGet_C(0, 0, &v284->v_dest_pos);
              }
          }
          else
          {
              v63 = (tGameObjectMain*)v284->o_next_wp;
              if (v63)
              {
                  if ((unsigned int)v63 <= 2)
                      v63 = AI_MainActor[(uint32_t)v63 - 1];
                  AI_EvalFunc_OBJGetPos_C(v63, &v284->v_dest_pos);
              }
              else
              {
                  AI_EvalFunc_OBJGetPos_C(0, &v284->v_dest_pos);
              }
          }
      }
  }
  v216 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_CV_aircraft_CV_mode_deplacement_CV_aircraft_CV_exec_waterforce);
  if (v216 != 1000)
      return v216;
  if (v284->des_mode)
  {
      if (v284->f_aircraft_dist >= 2500.0)
          AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
      else
          AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0xFFu, 0);
  }
  else if (v284->i_flag_on_coule)
  {
      AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
  }
  else
  {
      if (v284->f_aircraft_dist < 10000.0)
          goto LABEL_464;
      if (!AI_EvalFunc_TIMEElapsed_C(v284->f_blocking_time, 4.0))
          goto LABEL_464;
      v62 = (tGameObjectMain*)v284->o_next_wp;
      if (v62)
      {
          if ((unsigned int)v62 <= 2)
              v62 = AI_MainActor[(uint32_t)v62 - 1];
          v25 = AI_EvalFunc_OBJ_CapaTest_C(v62, 1);
      }
      else
      {
          v25 = AI_EvalFunc_OBJ_CapaTest_C(0, 1);
      }
      v287 = v25;
      if (!v25)
      {
          if (v284->f_water_Z != -1000000000.0f)
              AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, 17);
      }
      else
      {
      LABEL_464:
          if (*(uint32_t*)CV_aircraft_CV_mode_deplacement_univVars_world_ID != 90)
          {
              v246.x = 2.0;
              v246.y = 2.0;
              v246.z = 8.0;
              v208 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v246);
          }
          if (v284->f_aircraft_dist >= 225.0)
              AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 1, 16);
          else
              AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 17, 0);
      }
  }
  if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
      AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 1, 16);
  v278 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_CV_aircraft_CV_mode_deplacement_CV_aircraft_CV_exec_s_b);
  if (v278 != 1000)
      return v278;
  v170 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_CV_aircraft_CV_mode_deplacement_CV_aircraft_CV_exec_check_pos);
  if (v170 != 1000)
      return v170;
  v203 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_CV_aircraft_CV_mode_deplacement_CV_aircraft_CV_exec_test_collision);
  if (v203 != 1000)
      return v203;
  if (v284->i_flag_fuite || (v243 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject), v243 > 10.0))
      v284->f_time = TIM_gf_MainClock - TIM_gf_LockTime;
  v38 = (float)v284->des_i_temps_echec;
  if (AI_EvalFunc_TIMEElapsed_C(v284->f_time, v38))
  {
      v37 = (float)(v284->des_i_temps_echec + 1);
      if (AI_EvalFunc_TIMEElapsed_C(v284->f_time, v37))
      {
          v26 = AI_EvalFunc_WAYNearestWPOfObj_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, 128, 0, 0);
          v284->o_current_wp = (int)v26;
          v61 = (tGameObjectMain*)v284->o_current_wp;
          if (v61)
          {
              if ((unsigned int)v61 <= 2)
                  v61 = AI_MainActor[(uint32_t)v61 - 1];
              v27 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, v61, 3, 0);
          }
          else
          {
              v27 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, 0, 3, 0);
          }
          v284->o_next_wp = (int)v27;
          v60 = (tGameObjectMain*)v284->o_next_wp;
          if (v60)
          {
              if ((unsigned int)v60 <= 2)
                  v60 = AI_MainActor[(uint32_t)v60 - 1];
              v28 = AI_EvalFunc_OBJ_CapaGet_C(v60);
          }
          else
          {
              v28 = AI_EvalFunc_OBJ_CapaGet_C(0);
          }
          v169 = v28;
          if (v284->des_type)
          {
              v233 = 0;
              while ((v169 & 0x400) != 0 && v233 < 5)
              {
                  ++v233;
                  v59 = (tGameObjectMain*)v284->o_current_wp;
                  if (v59)
                  {
                      if ((unsigned int)v59 <= 2)
                          v59 = AI_MainActor[(uint32_t)v59 - 1];
                      v29 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, v59, 1, 0);
                  }
                  else
                  {
                      v29 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, 0, 1, 0);
                  }
                  v284->o_next_wp = (int)v29;
                  v58 = (tGameObjectMain*)v284->o_next_wp;
                  if (v58)
                  {
                      if ((unsigned int)v58 <= 2)
                          v58 = AI_MainActor[(uint32_t)v58 - 1];
                      v30 = AI_EvalFunc_OBJ_CapaGet_C(v58);
                  }
                  else
                  {
                      v30 = AI_EvalFunc_OBJ_CapaGet_C(0);
                  }
                  v169 = v30;
              }
              if (v233 == 5 && (v169 & 0x400) != 0)
              {
                  v57 = (tGameObjectMain*)v284->o_current_wp;
                  if (v57)
                  {
                      if ((unsigned int)v57 <= 2)
                          v57 = AI_MainActor[(uint32_t)v57 - 1];
                      v31 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, v57, 3, 0);
                  }
                  else
                  {
                      v31 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v284->n_net, 0, 3, 0);
                  }
                  v284->o_next_wp = (int)v31;
              }
          }
          if (v284->des_rand_bv_pos)
          {
              v56 = (tGameObjectMain*)v284->o_next_wp;
              if (v56)
              {
                  if ((unsigned int)v56 <= 2)
                      v56 = AI_MainActor[(uint32_t)v56 - 1];
                  AI_EvalFunc_BVRandomPosGet_C(v56, 0, &v284->v_dest_pos);
              }
              else
              {
                  AI_EvalFunc_BVRandomPosGet_C(0, 0, &v284->v_dest_pos);
              }
          }
          else
          {
              v55 = (tGameObjectMain*)v284->o_next_wp;
              if (v55)
              {
                  if ((unsigned int)v55 <= 2)
                      v55 = AI_MainActor[(uint32_t)v55 - 1];
                  AI_EvalFunc_OBJGetPos_C(v55, &v284->v_dest_pos);
              }
              else
              {
                  AI_EvalFunc_OBJGetPos_C(0, &v284->v_dest_pos);
              }
          }
          v284->f_time = TIM_gf_MainClock - TIM_gf_LockTime;
          v284->f_blocking_time = v284->f_time;
          v284->f_vitesse = v284->des_f_speed;
      }
      else if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x80))
      {
          v173 = (tAI*)AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 0x80);
          v54 = (tGameObjectMain*)v173;
          if (v173)
          {
              if ((unsigned int)v173 <= 2)
                  v54 = AI_MainActor[(uint32_t)v173 - 1];
              v32 = AI_EvalFunc_AIIsModel_C(v54, (tAIModel*)fctvar_CV_aircraft_CV_mode_deplacement_CV_aircraft);
          }
          else
          {
              v32 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_CV_aircraft_CV_mode_deplacement_CV_aircraft);
          }
          v289 = v32;
          if (v32)
          {
              if (v173 == WOR_gst_Universe.univAIInstance)
              {
                  v51 = WOR_gst_Universe.univAIInstance;
              }
              else
              {
                  v53 = (tGameObjectMain*)v173;
                  if (v173)
                  {
                      if ((unsigned int)v173 <= 2)
                          v53 = AI_MainActor[(uint32_t)v173 - 1];
                      v52 = v53;
                  }
                  else
                  {
                      v52 = 0;
                  }
                  v51 = v52->extended->AIInstance;
              }
              if (v284->n_net == ((tScriptStruct_CVAirCraft*)(v51->instanceVarDataCurrent))->n_net)
              {
                  if (v173 == WOR_gst_Universe.univAIInstance)
                  {
                      v48 = WOR_gst_Universe.univAIInstance;
                  }
                  else
                  {
                      v50 = (tGameObjectMain*)v173;
                      if (v173)
                      {
                          if ((unsigned int)v173 <= 2)
                              v50 = AI_MainActor[(uint32_t)v173 - 1];
                          v49 = v50;
                      }
                      else
                      {
                          v49 = 0;
                      }
                      v48 = v49->extended->AIInstance;
                  }
                  v284->o_current_wp = ((tScriptStruct_CVAirCraft*)(v48->instanceVarDataCurrent))->o_current_wp;
                  if (v173 == WOR_gst_Universe.univAIInstance)
                  {
                      v45 = WOR_gst_Universe.univAIInstance;
                  }
                  else
                  {
                      v47 = (tGameObjectMain*)v173;
                      if (v173)
                      {
                          if ((unsigned int)v173 <= 2)
                              v47 = AI_MainActor[(uint32_t)v173 - 1];
                          v46 = v47;
                      }
                      else
                      {
                          v46 = 0;
                      }
                      v45 = v46->extended->AIInstance;
                  }
                  v284->o_next_wp = ((tScriptStruct_CVAirCraft*)(v45->instanceVarDataCurrent))->o_next_wp;
                  v284->f_vitesse = v284->des_f_speed;
              }
          }
          if (v284->des_rand_bv_pos)
          {
              v44 = (tGameObjectMain*)v284->o_next_wp;
              if (v44)
              {
                  if ((unsigned int)v44 <= 2)
                      v44 = AI_MainActor[(uint32_t)v44 - 1];
                  AI_EvalFunc_BVRandomPosGet_C(v44, 0, &v284->v_dest_pos);
              }
              else
              {
                  AI_EvalFunc_BVRandomPosGet_C(0, 0, &v284->v_dest_pos);
              }
          }
          else
          {
              v43 = (tGameObjectMain*)v284->o_next_wp;
              if (v43)
              {
                  if ((unsigned int)v43 <= 2)
                      v43 = AI_MainActor[(uint32_t)v43 - 1];
                  AI_EvalFunc_OBJGetPos_C(v43, &v284->v_dest_pos);
              }
              else
              {
                  AI_EvalFunc_OBJGetPos_C(0, &v284->v_dest_pos);
              }
          }
          v284->f_time = TIM_gf_MainClock - TIM_gf_LockTime;
          v284->f_blocking_time = v284->f_time;
    }
  }
  v285 = v284->f_virage_coef * v284->f_vitesse;
  AI_EvalFunc_DYNFrictionVectorGet_C(AI_gpst_CurrentGameObject, &v283);
  v285 = v285 * v283.y;
  v263 = AI_EvalFunc_ARRBitTest_C((int*)(CV_aircraft_CV_mode_deplacement_univVars_Flag_speciaux_sauve + 4), 2784);
  if (v263)
  {
      OBJ_RestoreInitialPos(AI_gpst_CurrentGameObject);
      v285 = 0.0;
  }
  v215 = AI_EvalFunc_OBJ_CapaGet_C(AI_gpst_CurrentGameObject);
  if ((v215 & 0x880) != 0)
      v285 = 0.0;
  if (v284->i_flag_on_coule)
  {
      AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v254);
      v42 = v284->des_v_friction.y * 10.0f;
      v41 = &v284->v_engine_force;
      v284->v_engine_force.x = v254.x * v42;
      v41->y = v254.y * v42;
      v41->z = v254.z * v42;
  }
  else
  {
      AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v179);
      v219.x = v179.x * v285;
      v219.y = v179.y * v285;
      v219.z = v179.z * v285;
      v284->v_engine_force = v219;
  }
  MATH_VecAddVec(&v258, &v284->v_engine_force, &v284->v_water_force);
  v189 = v258;
  AI_EvalFunc_DYNTractionSet_C_CURRENT(&v189);
  AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v284->v_last_speed);
  if (v284->i_snd_loop_motor == -1 || v284->f_aircraft_dist <= 14400.0)
  {
      if (v284->i_snd_loop_motor == -1 && v284->f_aircraft_dist < 14400.0)
      {
          v33 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000000);
          v284->i_snd_loop_motor = v33;
          AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v284->i_snd_loop_motor);
          if (v284->des_type != 3)
          {
              v224 = SND_l_GetFreq(v284->i_snd_loop_motor);
              v261 = AI_EvalFunc_MATHRandFloat_C(0.94999999f, 1.05f);
              v36 = (float)v224;
              SND_SetFreq(v284->i_snd_loop_motor, (unsigned __int64)(v36 * v261));
          }
      }
  }
  else
  {
      SND_Release(v284->i_snd_loop_motor);
      v284->i_snd_loop_motor = -1;
  }
  if (v284->f_aircraft_dist < 100000.0 && v284->des_type == 3)
  {
      if (v284->i_snd_tut == -1 && (v291 = AI_EvalFunc_TIMEElapsed_C(v284->f_snd_tut, 45.0)))
      {
          v284->i_snd_tut = AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
          SND_SetDoppler(v284->i_snd_tut, 1);
          v222 = SND_l_GetFreq(v284->i_snd_tut);
          v35 = (float)v222;
          SND_SetFreq(v284->i_snd_tut, (unsigned __int64)(v284->f_snd_freq_tut * v35));
      }
      else if (AI_EvalFunc_TIMEElapsed_C(v284->f_snd_tut, 48.0))
      {
          v284->i_snd_tut = AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
          SND_SetDoppler(v284->i_snd_tut, 1);
          v186 = SND_l_GetFreq(v284->i_snd_tut);
          v34 = (float)v186;
          SND_SetFreq(v284->i_snd_tut, (unsigned __int64)(v284->f_snd_freq_tut * v34));
          v284->i_snd_tut = -1;
          v284->f_snd_tut = TIM_gf_MainClock - TIM_gf_LockTime;
      }
  }
  return 1000;
}

int CV_aircraft_CV_exec_waterforce()
{
    int result; // eax
    float v1; // [esp+0h] [ebp-B8h]
    float v2; // [esp+8h] [ebp-B0h]
    tScriptStruct_CVAirCraft* v3; // [esp+Ch] [ebp-ACh]
    float v4; // [esp+10h] [ebp-A8h]
    Vec3 v5; // [esp+30h] [ebp-88h] BYREF
    Vec3 a4; // [esp+3Ch] [ebp-7Ch] BYREF
    Vec3 v7; // [esp+48h] [ebp-70h]
    int v9; // [esp+58h] [ebp-60h]
    Vec3 a1; // [esp+5Ch] [ebp-5Ch] BYREF
    float v11; // [esp+6Ch] [ebp-4Ch]
    float a3; // [esp+88h] [ebp-30h]
    Vec3 a2; // [esp+8Ch] [ebp-2Ch] BYREF
    tScriptStruct_CVAirCraft* v14; // [esp+98h] [ebp-20h]
    Vec3 v15; // [esp+9Ch] [ebp-1Ch] BYREF
    Vec3 v16; // [esp+A8h] [ebp-10h] BYREF

    v7.x = 1.0;
    v7.y = 0.0;
    v7.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_CVAirCraft*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v14 = v3;
    if (v3->i_flag_on_coule)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
        if (v14->f_water_Z - 100.0 > a1.z)
        {
            v14->m_msg.msgID = 43;
            v14->m_msg.Int1 = 1;
            AI_EvalFunc_MSGSend_C(AI_gpst_CurrentGameObject, &v14->m_msg);
        }
        a2.x = 0.0;
        a2.y = 0.0;
        a2.z = 0.0;
        a3 = TIM_gf_dt;
        AI_EvalFunc_MATHVecBlend_C(&v14->v_water_force, &a2, TIM_gf_dt, &a4);
        v14->v_water_force = a4;
        v5.x = 1.0;
        v5.y = 2.0;
        v5.z = 1.0;
        v11 = TIM_gf_dt;
        v1 = 4.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlend_C(&v14->des_v_friction, &v5, v1, &v16);
        v14->des_v_friction = v16;
        v9 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v14->des_v_friction);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        if (v14->f_water_Z == -1000000000.0f)
        {
            v14->v_water_force.x = 0.0;
            v14->v_water_force.y = 0.0;
            v14->v_water_force.z = 0.0;
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v15);
            v7 = v15;
            v4 = v14->f_water_Z - (v14->des_f_Z_water_offset + v15.z);
            if (v4 < 0.0)
                v2 = 0.0;
            else
                v2 = v14->f_water_Z - (v14->des_f_Z_water_offset + v15.z);
            v14->v_water_force.x = 0.0;
            v14->v_water_force.y = 0.0;
            v14->v_water_force.z = v2 * 20.0f;
        }
        if (AI_EvalFunc_TIMEElapsed_C(v14->f_time_ground_col, 0.30000001f))
        {
            if (v14->v_water_force.z == 0.0f)
                v14->des_v_friction.z = 0.0;
            else
                v14->des_v_friction.z = 8.0;
        }
        else
        {
            v14->des_v_friction.z = 0.0;
        }
        AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v14->des_v_friction);
        result = 1000;
    }
    return result;
}

int CV_aircraft_CV_exec_afterrec()
{
    float v1; // [esp+4h] [ebp-158h]
    float v2; // [esp+8h] [ebp-154h]
    float v3; // [esp+Ch] [ebp-150h]
    float v4; // [esp+10h] [ebp-14Ch]
    float v5; // [esp+14h] [ebp-148h]
    float v6; // [esp+18h] [ebp-144h]
    float v7; // [esp+1Ch] [ebp-140h]
    float v8; // [esp+20h] [ebp-13Ch]
    float v9; // [esp+24h] [ebp-138h]
    tScriptStruct_CVAirCraft* v10; // [esp+28h] [ebp-134h]
    tGameObjectMain* v11; // [esp+30h] [ebp-12Ch]
    float v12; // [esp+34h] [ebp-128h]
    tGameObjectMain* v13; // [esp+40h] [ebp-11Ch]
    tGameObjectMain* v14; // [esp+44h] [ebp-118h]
    Vec3 v15; // [esp+48h] [ebp-114h] BYREF
    int v16; // [esp+54h] [ebp-108h]
    int v18; // [esp+60h] [ebp-FCh]
    Vec3 v19; // [esp+64h] [ebp-F8h] BYREF
    Vec3 a3; // [esp+70h] [ebp-ECh] BYREF
    Vec3 a2; // [esp+84h] [ebp-D8h] BYREF
    Vec3 v23; // [esp+90h] [ebp-CCh] BYREF
    Vec3 v24; // [esp+9Ch] [ebp-C0h] BYREF
    int v25; // [esp+A8h] [ebp-B4h]
    int v26; // [esp+ACh] [ebp-B0h]
    float v27; // [esp+B4h] [ebp-A8h]
    float v28; // [esp+B8h] [ebp-A4h]
    float v29; // [esp+BCh] [ebp-A0h]
    int v31; // [esp+CCh] [ebp-90h]
    Vec3 v32; // [esp+D0h] [ebp-8Ch] BYREF
    float v33; // [esp+DCh] [ebp-80h]
    float v34; // [esp+E0h] [ebp-7Ch]
    float v35; // [esp+E4h] [ebp-78h]
    Vec3 v36; // [esp+F0h] [ebp-6Ch] BYREF
    int v37; // [esp+FCh] [ebp-60h]
    BOOL v38; // [esp+100h] [ebp-5Ch]
    int v40; // [esp+114h] [ebp-48h]
    tScriptStruct_CVAirCraft* v41; // [esp+118h] [ebp-44h]
    int v42; // [esp+11Ch] [ebp-40h]
    int v43; // [esp+120h] [ebp-3Ch]
    Vec3 v44; // [esp+124h] [ebp-38h] BYREF
    Vec3 v45; // [esp+130h] [ebp-2Ch] BYREF
    int v46; // [esp+13Ch] [ebp-20h]
    int v47; // [esp+140h] [ebp-1Ch]
    int v48; // [esp+144h] [ebp-18h]
    Vec3 v49; // [esp+148h] [ebp-14h] BYREF
    int v50; // [esp+154h] [ebp-8h]
    int v51; // [esp+158h] [ebp-4h]

    a2.x = 1.0;
    a2.y = 0.0;
    a2.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v10 = (tScriptStruct_CVAirCraft*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v10 = 0;
    v41 = v10;
    v43 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_CV_aircraft_CV_exec_afterrec_CV_aircraft_CV_exec_trace = (int)AI_ul_RealResolveOneRefFunction(0x72002FE7u);
        fct_CV_aircraft_CV_exec_afterrec_CV_aircraft_CV_exec_GFX_Halo = (int)AI_ul_RealResolveOneRefFunction(0x72004736u);
        fct_CV_aircraft_CV_exec_afterrec_CV_aircraft_CV_exec_GFX_Smoke = (int)AI_ul_RealResolveOneRefFunction(0x72004D67u);
        fct_CV_aircraft_CV_exec_afterrec_CV_aircraft_CV_exec_GFX_Flammes = (int)AI_ul_RealResolveOneRefFunction(0x72004D66u);
    }
    if (v41->i_flag_low_res)
    {
        if (v41->f_camera_dist < 10000.0)
        {
            v41->i_flag_low_res = 0;
            if (v41->GFX_Flammes != -1)
            {
                AI_EvalFunc_GFXSeti_C_CURRENT(v41->GFX_Flammes, 13106, 0);
                v41->GFX_Flammes = -1;
            }
            if (v41->GFX_Smoke != -1)
            {
            LABEL_16:
                AI_EvalFunc_GFXSeti_C_CURRENT(v41->GFX_Smoke, 13106, 0);
                v41->GFX_Smoke = -1;
                goto LABEL_17;
            }
        }
    }
    else if (v41->f_camera_dist > 40000.0)
    {
        v41->i_flag_low_res = 1;
        if (v41->GFX_Flammes != -1)
        {
            AI_EvalFunc_GFXSeti_C_CURRENT(v41->GFX_Flammes, 13106, 0);
            v41->GFX_Flammes = -1;
        }
        if (v41->GFX_Smoke != -1)
            goto LABEL_16;
    }
LABEL_17:
    if (v41->des_water_trace)
    {
        v25 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_CV_aircraft_CV_exec_afterrec_CV_aircraft_CV_exec_trace);
        if (v25 != 1000)
            return v25;
    }
    v40 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_CV_aircraft_CV_exec_afterrec_CV_aircraft_CV_exec_GFX_Halo);
    if (v40 != 1000)
        return v40;
    v37 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_CV_aircraft_CV_exec_afterrec_CV_aircraft_CV_exec_GFX_Smoke);
    if (v37 != 1000)
        return v37;
    v18 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_CV_aircraft_CV_exec_afterrec_CV_aircraft_CV_exec_GFX_Flammes);
    if (v18 != 1000)
        return v18;
    if (v41->GFX_Smoke != -1)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v15);
        if (v41->f_water_Z - 5.0 <= v15.z)
        {
            if (v41->f_camera_dist > 1000000.0 || (v31 = (unsigned __int16)AI_gpst_CurrentGameObject->flags2, (v31 & 4) != 0))
            {
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v41->GFX_Smoke, 0, 0);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v45);
                AI_EvalFunc_MATHVecLocalToGlobal_C(AI_gpst_CurrentGameObject, &v41->des_v_pos_fumee, &a3);
                MATH_VecAddVec(&v36, &v45, &a3);
                AI_EvalFunc_GFXSetv_C_CURRENT(v41->GFX_Smoke, 13200, &v36);
            }
            else
            {
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v41->GFX_Smoke, 0, 1);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v44);
                AI_EvalFunc_MATHVecLocalToGlobal_C(AI_gpst_CurrentGameObject, &v41->des_v_pos_fumee, &v23);
                MATH_VecAddVec(&v32, &v44, &v23);
                AI_EvalFunc_GFXSetv_C_CURRENT(v41->GFX_Smoke, 13200, &v32);
            }
        }
        else
        {
            AI_EvalFunc_GFXSeti_C_CURRENT(v41->GFX_Smoke, 13106, 10);
            v41->GFX_Smoke = -1;
        }
    }
    v14 = (tGameObjectMain*)v41->o_sfx_explosion;
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
    if (v13)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v19);
        a2 = v19;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v49);
        v12 = (v41->des_f_Y_max_offset - v41->des_f_Y_min_offset) * 0.5f;
        v33 = v49.x * v12;
        v34 = v49.y * v12;
        v35 = v49.z * v12;
        a2.x = a2.x + v33;
        a2.y = a2.y + v34;
        a2.z = a2.z + v35;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v24);
        v27 = v24.x * 3.0f;
        v28 = v24.y * 3.0f;
        v29 = v24.z * 3.0f;
        a2.x = a2.x + v27;
        a2.y = a2.y + v28;
        a2.z = a2.z + v29;
        v11 = (tGameObjectMain*)v41->o_sfx_explosion;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v11 = AI_MainActor[(uint32_t)v11 - 1];
            AI_EvalFunc_OBJSetPos_C(v11, &a2);
        }
        else
        {
            AI_EvalFunc_OBJSetPos_C(0, &a2);
        }
    }
    if (v41->i_snd_klaxon)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v41->f_snd_time_bip, 8.0))
        {
            v41->f_snd_delai_bip = AI_EvalFunc_MATHRandFloat_C(0.1f, 0.30000001f);
            v41->i_snd_mode = AI_EvalFunc_MATHRandInt_C(0, 4);
            v41->i_snd_klaxon = 0;
        }
        else if (v41->i_snd_mode)
        {
            switch (v41->i_snd_mode)
            {
            case 1:
                if (v41->i_snd_klaxon == 1)
                {
                    v43 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
                    SND_SetDoppler(v43, 1);
                    v42 = SND_l_GetFreq(v43);
                    v6 = (float)v42;
                    SND_SetFreq(v43, (unsigned int)(v41->f_snd_freq_bip * v6));
                    ++v41->i_snd_klaxon;
                }
                else if (v41->i_snd_klaxon == 2)
                {
                    if (AI_EvalFunc_TIMEElapsed_C(v41->f_snd_time_bip, v41->f_snd_delai_bip))
                    {
                        v43 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
                        SND_SetDoppler(v43, 1);
                        v48 = SND_l_GetFreq(v43);
                        v5 = (float)v48;
                        SND_SetFreq(v43, (unsigned int)(v41->f_snd_freq_bip * v5));
                        ++v41->i_snd_klaxon;
                    }
                }
                break;
            case 2:
                if (v41->i_snd_klaxon == 1)
                {
                    v43 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(2, -1.0);
                    SND_SetDoppler(v43, 1);
                    v51 = SND_l_GetFreq(v43);
                    v4 = (float)v51;
                    SND_SetFreq(v43, (unsigned int)(v41->f_snd_freq_bip * v4));
                    ++v41->i_snd_klaxon;
                }
                break;
            case 3:
                if (v41->i_snd_klaxon == 1)
                {
                    v43 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
                    SND_SetDoppler(v43, 1);
                    v16 = SND_l_GetFreq(v43);
                    v3 = (float)v16;
                    SND_SetFreq(v43, (unsigned int)(v41->f_snd_freq_bip * v3));
                    ++v41->i_snd_klaxon;
                }
                else if (v41->i_snd_klaxon == 2)
                {
                    if (AI_EvalFunc_TIMEElapsed_C(v41->f_snd_time_bip, v41->f_snd_delai_bip))
                    {
                        v43 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(2, -1.0);
                        SND_SetDoppler(v43, 1);
                        v47 = SND_l_GetFreq(v43);
                        v2 = (float)v47;
                        SND_SetFreq(v43, (unsigned int)(v41->f_snd_freq_bip * v2));
                        ++v41->i_snd_klaxon;
                    }
                }
                break;
            }
        }
        else if (v41->i_snd_klaxon == 1)
        {
            v43 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
            SND_SetDoppler(v43, 1);
            v50 = SND_l_GetFreq(v43);
            v9 = (float)v50;
            SND_SetFreq(v43, (unsigned int)(v41->f_snd_freq_bip * v9));
            ++v41->i_snd_klaxon;
        }
        else if (v41->i_snd_klaxon == 2 && (v38 = AI_EvalFunc_TIMEElapsed_C(v41->f_snd_time_bip, v41->f_snd_delai_bip)))
        {
            v43 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
            SND_SetDoppler(v43, 1);
            v26 = SND_l_GetFreq(v43);
            v8 = (float)v26;
            SND_SetFreq(v43, (unsigned int)(v41->f_snd_freq_bip * v8));
            ++v41->i_snd_klaxon;
        }
        else if (v41->i_snd_klaxon == 3)
        {
            v1 = v41->f_snd_delai_bip + v41->f_snd_delai_bip;
            if (AI_EvalFunc_TIMEElapsed_C(v41->f_snd_time_bip, v1))
            {
                v43 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(2, -1.0);
                SND_SetDoppler(v43, 1);
                v46 = SND_l_GetFreq(v43);
                v7 = (float)v46;
                SND_SetFreq(v43, (unsigned int)(v41->f_snd_freq_bip * v7));
                ++v41->i_snd_klaxon;
            }
        }
    }
    return 1000;
}