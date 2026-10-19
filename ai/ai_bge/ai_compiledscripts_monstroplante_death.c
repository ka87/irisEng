#include "ai_compiledscripts_inc.h"

//monstroplante_death_mpd_loop
int monstroplante_death_mpd_loop_INV_manager_gao;
int monstroplante_death_mpd_loop_INV_manager_gao_f_Timer;
int monstroplante_death_mpd_loop_SFX_explosion_gao;

//monstroplante_death_mpd_waitfordeath
int fct_monstroplante_death_mpd_waitfordeath_monstroplante_death_mpd_beforedisplay;
int fct_monstroplante_death_mpd_waitfordeath_monstroplante_death_mpd_afterdisplay;
int fct_monstroplante_death_mpd_waitfordeath_monstroplante_death_mpd_loop;

//monstroplante_death_mpd_beforedisplay
int monstroplante_death_mpd_beforedisplay_INV_manager_gao;
int monstroplante_death_mpd_beforedisplay_INV_manager_gao_f_Timer;

//monstroplante_death_mpd_init
int fct_monstroplante_death_mpd_init_monstroplante_death_mpd_waitfordeath;

int monstroplante_death_mpd_loop()
{
    char* v0; // eax Cine_MonstroPlante_0003_intro.gao, Key (hex): bf002299
    Vec3* v1; // eax
    tGameObjectMain* v2; // eax
    float* v3; // eax
    int v4; // eax
    int result; // eax
    char* v6; // eax
    float X; // [esp+0h] [ebp-234h]
    float Xa; // [esp+0h] [ebp-234h]
    float Xb; // [esp+0h] [ebp-234h]
    float Xc; // [esp+0h] [ebp-234h]
    float* v11; // [esp+4h] [ebp-230h]
    float v12; // [esp+8h] [ebp-22Ch]
    float* v13; // [esp+Ch] [ebp-228h]
    float* v14; // [esp+10h] [ebp-224h]
    tScriptStruct_MonstroplanteDeath* v15; // [esp+14h] [ebp-220h]
    tGameObjectMain* v16; // [esp+1Ch] [ebp-218h]
    tGameObjectMain* v17; // [esp+20h] [ebp-214h]
    tGameObjectMain* v18; // [esp+24h] [ebp-210h]
    tGameObjectMain* v19; // [esp+2Ch] [ebp-208h]
    tGameObjectMain* v20; // [esp+34h] [ebp-200h]
    tGameObjectMain* v21; // [esp+3Ch] [ebp-1F8h]
    tGameObjectMain* v22; // [esp+40h] [ebp-1F4h]
    tGameObjectMain* v23; // [esp+44h] [ebp-1F0h]
    tGameObjectMain* v24; // [esp+4Ch] [ebp-1E8h]
    tGameObjectMain* v25; // [esp+50h] [ebp-1E4h]
    tGameObjectMain* v26; // [esp+54h] [ebp-1E0h]
    tGameObjectMain* v27; // [esp+5Ch] [ebp-1D8h]
    tGameObjectMain* v28; // [esp+60h] [ebp-1D4h]
    tGameObjectMain* v29; // [esp+64h] [ebp-1D0h]
    tAI* v30; // [esp+68h] [ebp-1CCh]
    tGameObjectMain* v31; // [esp+6Ch] [ebp-1C8h]
    tGameObjectMain* v32; // [esp+70h] [ebp-1C4h]
    tGameObjectMain* v33; // [esp+7Ch] [ebp-1B8h]
    tGameObjectMain* v34; // [esp+80h] [ebp-1B4h]
    tAI* v35; // [esp+84h] [ebp-1B0h]
    tGameObjectMain* v36; // [esp+88h] [ebp-1ACh]
    tGameObjectMain* v37; // [esp+8Ch] [ebp-1A8h]
    tGameObjectMain* v38; // [esp+94h] [ebp-1A0h]
    tGameObjectMain* v39; // [esp+9Ch] [ebp-198h]
    tGameObjectMain* v40; // [esp+A4h] [ebp-190h]
    tGameObjectMain* v41; // [esp+ACh] [ebp-188h]
    tGameObjectMain* v42; // [esp+B0h] [ebp-184h]
    tGameObjectMain* v43; // [esp+B4h] [ebp-180h]
    tGameObjectMain* v44; // [esp+BCh] [ebp-178h]
    tAI* v45; // [esp+C4h] [ebp-170h]
    tGameObjectMain* v46; // [esp+C8h] [ebp-16Ch]
    tGameObjectMain* v47; // [esp+CCh] [ebp-168h]
    tAI* v48; // [esp+D4h] [ebp-160h]
    tGameObjectMain* v49; // [esp+D8h] [ebp-15Ch]
    tGameObjectMain* v50; // [esp+DCh] [ebp-158h]
    tAI* v51; // [esp+E4h] [ebp-150h]
    tGameObjectMain* v52; // [esp+E8h] [ebp-14Ch]
    tGameObjectMain* v53; // [esp+ECh] [ebp-148h]
    tAI* v54; // [esp+F4h] [ebp-140h]
    tGameObjectMain* v55; // [esp+F8h] [ebp-13Ch]
    tGameObjectMain* v56; // [esp+FCh] [ebp-138h]
    tGameObjectMain* v57; // [esp+10Ch] [ebp-128h]
    tGameObjectMain* v58; // [esp+110h] [ebp-124h]
    tGameObjectMain* v59; // [esp+114h] [ebp-120h]
    tGameObjectMain* v60; // [esp+11Ch] [ebp-118h]
    tGameObjectMain* v61; // [esp+124h] [ebp-110h]
    tGameObjectMain* v62; // [esp+12Ch] [ebp-108h]
    tGameObjectMain* v63; // [esp+134h] [ebp-100h]
    tGameObjectMain* v64; // [esp+13Ch] [ebp-F8h]
    tGameObjectMain* v65; // [esp+144h] [ebp-F0h]
    tGameObjectMain* v66; // [esp+148h] [ebp-ECh]
    tGameObjectMain* v67; // [esp+14Ch] [ebp-E8h]
    tGameObjectMain* v68; // [esp+154h] [ebp-E0h]
    tGameObjectMain* v69; // [esp+158h] [ebp-DCh]
    tGameObjectMain* v70; // [esp+15Ch] [ebp-D8h]
    tGameObjectMain* v71; // [esp+160h] [ebp-D4h]
    tGameObjectMain* v72; // [esp+164h] [ebp-D0h]
    tGameObjectMain* v73; // [esp+168h] [ebp-CCh]
    tGameObjectMain* v74; // [esp+16Ch] [ebp-C8h]
    Vec3 a2; // [esp+170h] [ebp-C4h] BYREF
    tMessage Src; // [esp+17Ch] [ebp-B8h] BYREF
    tGameObjectMain* v77; // [esp+1B4h] [ebp-80h]
    float a3; // [esp+1B8h] [ebp-7Ch]
    int v79; // [esp+1BCh] [ebp-78h]
    float zoom; // [esp+1C0h] [ebp-74h]
    float v81[7]; // [esp+1C4h] [ebp-70h] BYREF
    int v82; // [esp+1E0h] [ebp-54h]
    Vec3 pos; // [esp+1E4h] [ebp-50h] BYREF
    int v84; // [esp+1F0h] [ebp-44h]
    int i; // [esp+1F8h] [ebp-3Ch]
    float v86; // [esp+1FCh] [ebp-38h]
    Vec3 v87; // [esp+204h] [ebp-30h]
    tGameObjectMain* v88; // [esp+210h] [ebp-24h]
    tAI* v89; // [esp+214h] [ebp-20h]
    tScriptStruct_MonstroplanteDeath* v90; // [esp+21Ch] [ebp-18h]
    float v91[5]; // [esp+220h] [ebp-14h] BYREF

    pos.x = 1.0;
    pos.y = 0.0;
    pos.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v15 = (tScriptStruct_MonstroplanteDeath*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v15 = 0;
    v90 = v15;
    v77 = 0;
    i = 0;
    a3 = 0.0;
    v89 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        monstroplante_death_mpd_loop_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(

            &monstroplante_death_mpd_loop_INV_manager_gao_f_Timer,
            (void*)monstroplante_death_mpd_loop_INV_manager_gao,
            176);
        monstroplante_death_mpd_loop_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
    }
    if (v90->i_CutCam)
    {
        if (v90->i_CutCam == 1)
        {
            v79 = AI_EvalFunc_OBJ_CapaGet_C(AI_gpst_CurrentGameObject);
            if ((v79 & 0x200) != 0)
            {
                v90->i_CutCam = 2;
                v59 = (tGameObjectMain*)v90->o_Particles;
                if (v59)
                {
                    if ((unsigned int)v59 <= 2)
                        v59 = AI_MainActor[(uint32_t)v59 - 1];
                    v58 = v59;
                }
                else
                {
                    v58 = 0;
                }
                if (v58)
                {
                    a2.x = -9.0;
                    a2.y = -1.0;
                    a2.z = 0.0;
                    v57 = (tGameObjectMain*)v90->o_Particles;
                    if (v57)
                    {
                        if ((unsigned int)v57 <= 2)
                            v57 = AI_MainActor[(uint32_t)v57 - 1];
                        AI_EvalFunc_PAG_ShiftParticles_C(v57, &a2);
                    }
                    else
                    {
                        AI_EvalFunc_PAG_ShiftParticles_C(0, &a2);
                    }
                }
            }
        }
    }
    else
    {
        v82 = AI_EvalFunc_OBJ_CapaGet_C(AI_gpst_CurrentGameObject);
        if ((v82 & 0x100) != 0)
        {
            for (i = 0; i < 20; ++i)
            {
                if (*(float*)AI_EvalVarArray_C2((char*)&v90->__af_ExplodeTime_Dimension1, i) >= 1000.0)
                {
                    v14 = (float*)AI_EvalVarArray_C2((char*)&v90->__af_ExplodeTime_Dimension1, i);
                    *v14 = *v14 - (1000.0f - v90->f_Time);
                }
                else
                {
                    *(uint32_t*)AI_EvalVarArray_C2((char*)&v90->__af_ExplodeTime_Dimension1, i) = 0xBF800000;
                }
            }
            for (i = 0; i < 7; ++i)
            {
                v13 = (float*)AI_EvalVarArray_C2((char*)&v90->__af_MorphStartTime_Dimension1, i);
                *v13 = *v13 + v90->f_Time;
            }
            v90->f_ParticleZMaxStart = v90->f_ParticleZMaxStart + v90->f_Time;
            v77 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0);
            v74 = v77;
            if (v77)
            {
                if ((unsigned int)v77 <= 2)
                    v74 = AI_MainActor[(uint32_t)v77 - 1];
                v73 = v74;
            }
            else
            {
                v73 = 0;
            }
            if (v73)
            {
                v72 = (tGameObjectMain*)v90->o_Particles;
                if (v72)
                {
                    if ((unsigned int)v72 <= 2)
                        v72 = AI_MainActor[(uint32_t)v72 - 1];
                    v71 = v72;
                }
                else
                {
                    v71 = 0;
                }
                if (v71)
                {
                    v70 = v77;
                    if (v77)
                    {
                        if ((unsigned int)v77 <= 2)
                            v70 = AI_MainActor[(uint32_t)v77 - 1];
                        v69 = v70;
                    }
                    else
                    {
                        v69 = 0;
                    }
                    v68 = (tGameObjectMain*)v90->o_Particles;
                    if (v68)
                    {
                        if ((unsigned int)v68 <= 2)
                            v68 = AI_MainActor[(uint32_t)v68 - 1];
                        AI_EvalFunc_PAG_SetObject_C(v68, v69);
                    }
                    else
                    {
                        AI_EvalFunc_PAG_SetObject_C(0, v69);
                    }
                    v88 = AI_gpst_CurrentGameObject;
                    v67 = AI_gpst_CurrentGameObject;
                    if (AI_gpst_CurrentGameObject)
                    {
                        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                            v67 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                        v66 = v67;
                    }
                    else
                    {
                        v66 = 0;
                    }
                    v65 = (tGameObjectMain*)v90->o_Particles;
                    if (v65)
                    {
                        if ((unsigned int)v65 <= 2)
                            v65 = AI_MainActor[(uint32_t)v65 - 1];
                        AI_EvalFunc_PAG_SetMatrixRef_C(v65, v66);
                    }
                    else
                    {
                        AI_EvalFunc_PAG_SetMatrixRef_C(0, v66);
                    }
                    v64 = (tGameObjectMain*)v90->o_Particles;
                    if (v64)
                    {
                        if ((unsigned int)v64 <= 2)
                            v64 = AI_MainActor[(uint32_t)v64 - 1];
                        AI_EvalFunc_PAG_Setf_C(v64, 23, v90->f_ParticleZMin);
                    }
                    else
                    {
                        AI_EvalFunc_PAG_Setf_C(0, 23, v90->f_ParticleZMin);
                    }
                    v63 = (tGameObjectMain*)v90->o_Particles;
                    if (v63)
                    {
                        if ((unsigned int)v63 <= 2)
                            v63 = AI_MainActor[(uint32_t)v63 - 1];
                        AI_EvalFunc_PAG_Setf_C(v63, 25, v90->f_ParticleZMaxStart);
                    }
                    else
                    {
                        AI_EvalFunc_PAG_Setf_C(0, 25, v90->f_ParticleZMaxStart);
                    }
                    v62 = (tGameObjectMain*)v90->o_Particles;
                    if (v62)
                    {
                        if ((unsigned int)v62 <= 2)
                            v62 = AI_MainActor[(uint32_t)v62 - 1];
                        AI_EvalFunc_PAG_Setf_C(v62, 21, 0.0);
                    }
                    else
                    {
                        AI_EvalFunc_PAG_Setf_C(0, 21, 0.0);
                    }
                }
                v61 = v77;
                if (v77)
                {
                    if ((unsigned int)v77 <= 2)
                        v61 = AI_MainActor[(uint32_t)v77 - 1];
                    OBJ_FreezeModifier_C(v61, 21, 1);
                }
                else
                {
                    OBJ_FreezeModifier_C(0, 21, 1);
                }
                v60 = v77;
                if (v77)
                {
                    if ((unsigned int)v77 <= 2)
                        v60 = AI_MainActor[(uint32_t)v77 - 1];
                    AI_EvalFunc_OBJ_SetChannelProg_C(v60, 0, 1.0);
                }
                else
                {
                    AI_EvalFunc_OBJ_SetChannelProg_C(0, 0, 1.0);
                }
            }
            v90->i_CutCam = 1;
        }
    }
    v90->f_Time = TIM_gf_dt + v90->f_Time;
    if (v90->f_Time > 14.0)
        AI_EvalFunc_AITrackCurStop_C();
    *(float*)monstroplante_death_mpd_loop_INV_manager_gao_f_Timer = v90->f_Time;
    if (v90->i_CutCam)
        *(float*)monstroplante_death_mpd_loop_INV_manager_gao_f_Timer = *(float*)monstroplante_death_mpd_loop_INV_manager_gao_f_Timer
        - 2.0f;
    for (i = 0; i < 20; ++i)
    {
        if (*(float*)AI_EvalVarArray_C2((char*)&v90->__af_ExplodeTime_Dimension1, i) != -1.0)
        {
            v0 = AI_EvalVarArray_C2((char*)&v90->__af_ExplodeTime_Dimension1, i);
            if (*(float*)v0 < (double)v90->f_Time)
            {
                *(uint32_t*)AI_EvalVarArray_C2((char*)&v90->__af_ExplodeTime_Dimension1, i) = 0xBF800000;
                AI_EvalFunc_OBJGetPos_C_CURRENT((Vec3*)v91);
                v1 = (Vec3*)AI_EvalVarArray_C3((char*)&v90->__av_ExplodePos_Dimension1, i);
                MATH_VecAddVec((Vec3*)v81, (Vec3*)v91, v1);
                pos = *(Vec3*)v81;
                if (i < 8)
                {
                    v87.x = 0.30000001f;
                    v87.y = -0.30000001f;
                    v87.z = 0.40000001f;
                    pos.x = pos.x + 0.30000001f;
                    pos.y = pos.y + -0.30000001f;
                    pos.z = pos.z + 0.40000001f;
                }
                v89 = (tAI*)AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)monstroplante_death_mpd_loop_SFX_explosion_gao, &pos);
                if (v89 == WOR_gst_Universe.univAIInstance)
                {
                    v54 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v56 = (tGameObjectMain*)v89;
                    if (v89)
                    {
                        if ((unsigned int)v89 <= 2)
                            v56 = AI_MainActor[(uint32_t)v89 - 1];
                        v55 = v56;
                    }
                    else
                    {
                        v55 = 0;
                    }
                    v54 = v55->extended->AIInstance;
                }
                ((tScriptStruct_SFXExplosion*)(v54->instanceVarDataCurrent))->table_color[0] = 0x101010;
                if (v89 == WOR_gst_Universe.univAIInstance)
                {
                    v51 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v53 = (tGameObjectMain*)v89;
                    if (v89)
                    {
                        if ((unsigned int)v89 <= 2)
                            v53 = AI_MainActor[(uint32_t)v89 - 1];
                        v52 = v53;
                    }
                    else
                    {
                        v52 = 0;
                    }
                    v51 = v52->extended->AIInstance;
                }
                ((tScriptStruct_SFXExplosion*)(v51->instanceVarDataCurrent))->table_color[1] = 0x80101808;
                if (v89 == WOR_gst_Universe.univAIInstance)
                {
                    v48 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v50 = (tGameObjectMain*)v89;
                    if (v89)
                    {
                        if ((unsigned int)v89 <= 2)
                            v50 = AI_MainActor[(uint32_t)v89 - 1];
                        v49 = v50;
                    }
                    else
                    {
                        v49 = 0;
                    }
                    v48 = v49->extended->AIInstance;
                }
                ((tScriptStruct_SFXExplosion*)(v48->instanceVarDataCurrent))->table_color[2] = 0x80000000;
                if (v89 == WOR_gst_Universe.univAIInstance)
                {
                    v45 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v47 = (tGameObjectMain*)v89;
                    if (v89)
                    {
                        if ((unsigned int)v89 <= 2)
                            v47 = AI_MainActor[(uint32_t)v89 - 1];
                        v46 = v47;
                    }
                    else
                    {
                        v46 = 0;
                    }
                    v45 = v46->extended->AIInstance;
                }
                ((tScriptStruct_SFXExplosion*)(v45->instanceVarDataCurrent))->table_color[3] = 0x80101008;
                Src.msgID = 1;
                Src.Int1 = 31;
                if (i >= 8)
                {
                    Src.f2 = 0.1f;
                }
                else
                {
                    v12 = (float)i;
                    Src.f2 = 0.30000001f - v12 * 0.0099999998f;
                }
                Src.f1 = 0.050000001f;
                Src.v1.x = 1.0;
                Src.v1.y = 0.0;
                Src.v1.z = 1.0;
                v44 = (tGameObjectMain*)v89;
                if (v89)
                {
                    if ((unsigned int)v89 <= 2)
                        v44 = AI_MainActor[(uint32_t)v89 - 1];
                    AI_EvalFunc_MSGSend_C(v44, &Src);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &Src);
                }
                v43 = (tGameObjectMain*)v90->o_ExplodeCircleDup;
                if (v43)
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
                    v41 = (tGameObjectMain*)v90->o_ExplodeCircleDup;
                    if (v41)
                    {
                        if ((unsigned int)v41 <= 2)
                            v41 = AI_MainActor[(uint32_t)v41 - 1];
                        v2 = AI_EvalFunc_OBJDuplicate_C(v41, &pos);
                    }
                    else
                    {
                        v2 = AI_EvalFunc_OBJDuplicate_C(0, &pos);
                    }
                    v77 = v2;
                    zoom = AI_EvalFunc_MATHRandFloat_C(0.80000001f, 1.2f);
                    v40 = v77;
                    if (v77)
                    {
                        if ((unsigned int)v77 <= 2)
                            v40 = AI_MainActor[(uint32_t)v77 - 1];
                        AI_EvalFunc_OBJ_ZoomSet_C(v40, zoom);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_ZoomSet_C(0, zoom);
                    }
                    v39 = v77;
                    if (v77)
                    {
                        if ((unsigned int)v77 <= 2)
                            v39 = AI_MainActor[(uint32_t)v77 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v39, 0);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
                    }
                    v38 = v77;
                    if (v77)
                    {
                        if ((unsigned int)v77 <= 2)
                            v38 = AI_MainActor[(uint32_t)v77 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v38, 0);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
                    }
                    if (v77 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                    {
                        v35 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v37 = v77;
                        if (v77)
                        {
                            if ((unsigned int)v77 <= 2)
                                v37 = AI_MainActor[(uint32_t)v77 - 1];
                            v36 = v37;
                        }
                        else
                        {
                            v36 = 0;
                        }
                        v35 = v36->extended->AIInstance;
                    }
                    ((tScriptStruct_SFXChangeColor*)(v35->instanceVarDataCurrent))->c_Color = 0x434B21;
                    v90->ao_ExplodeCircle[i] = (int)v77;
                    *(uint32_t*)AI_EvalVarArray_C2((char*)&v90->__af_ExplodeCircleTime_Dimension1, i) = 0;
                }
            }
        }
        v34 = (tGameObjectMain*)v90->ao_ExplodeCircle[i];
        if (v34)
        {
            if ((unsigned int)v34 <= 2)
                v34 = AI_MainActor[(uint32_t)v34 - 1];
            v33 = v34;
        }
        else
        {
            v33 = 0;
        }
        if (v33)
        {
            v11 = (float*)AI_EvalVarArray_C2((char*)&v90->__af_ExplodeCircleTime_Dimension1, i);
            *v11 = TIM_gf_dt + *v11;
            v77 = (tGameObjectMain*)v90->ao_ExplodeCircle[i];
            if (*(float*)AI_EvalVarArray_C2((char*)&v90->__af_ExplodeCircleTime_Dimension1, i) > 1.0)
            {
                a3 = 3.0;
                if (v77 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
                {
                    v30 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v32 = v77;
                    if (v77)
                    {
                        if ((unsigned int)v77 <= 2)
                            v32 = AI_MainActor[(uint32_t)v77 - 1];
                        v31 = v32;
                    }
                    else
                    {
                        v31 = 0;
                    }
                    v30 = v31->extended->AIInstance;
                }
                v3 = (float*)AI_EvalVarArray_C2((char*)&v90->__af_ExplodeCircleTime_Dimension1, i);
                Xa = 1.5f - *v3 + 1.5f - *v3;
                ((tScriptStruct_SFXChangeColor*)(v30->instanceVarDataCurrent))->c_Color = AI_EvalFunc_COLORBlend_C(0, 0x434B21, Xa);
            }
            else
            {
                X = *(float*)AI_EvalVarArray_C2((char*)&v90->__af_ExplodeCircleTime_Dimension1, i) * 1.570796f;
                v86 = sinf(X);
                a3 = 3.0f * v86;
            }
            v29 = v77;
            if (v77)
            {
                if ((unsigned int)v77 <= 2)
                    v29 = AI_MainActor[(uint32_t)v77 - 1];
                v28 = v29;
            }
            else
            {
                v28 = 0;
            }
            Xb = a3 + -0.99000001f;
            AI_EvalFunc_OBJ_SetChannelFactor_C(v28, 0, Xb);
            if (*(float*)AI_EvalVarArray_C2((char*)&v90->__af_ExplodeCircleTime_Dimension1, i) > 1.5)
            {
                v27 = v77;
                if (v77)
                {
                    if ((unsigned int)v77 <= 2)
                        v27 = AI_MainActor[(uint32_t)v77 - 1];
                    v4 = AI_EvalFunc_OBJDestroy_C(v27);
                }
                else
                {
                    v4 = AI_EvalFunc_OBJDestroy_C(0);
                }
                v84 = v4;
                if (v4 != 1000)
                    return v84;
                v90->ao_ExplodeCircle[i] = 0;
            }
        }
    }
    if (v90->i_CutCam)
    {
        v77 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0);
        v26 = v77;
        if (v77)
        {
            if ((unsigned int)v77 <= 2)
                v26 = AI_MainActor[(uint32_t)v77 - 1];
            v25 = v26;
        }
        else
        {
            v25 = 0;
        }
        if (v25)
        {
            for (i = 0; i < 7; ++i)
            {
                v6 = AI_EvalVarArray_C2((char*)&v90->__af_MorphStartTime_Dimension1, i);
                if (*(float*)v6 <= (double)v90->f_Time)
                    a3 = 1.0;
                else
                    a3 = 0.0;
                v24 = v77;
                if (v77)
                {
                    if ((unsigned int)v77 <= 2)
                        v24 = AI_MainActor[(uint32_t)v77 - 1];
                    AI_EvalFunc_OBJ_SetChannelProg_C(v24, i + 1, a3);
                }
                else
                {
                    AI_EvalFunc_OBJ_SetChannelProg_C(0, i + 1, a3);
                }
            }
        }
        v23 = (tGameObjectMain*)v90->o_Particles;
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
            if (v90->f_ParticleZMaxStart <= (double)v90->f_Time)
            {
                if (v90->f_ParticleZMaxLength >= v90->f_Time - v90->f_ParticleZMaxStart)
                {
                    v19 = (tGameObjectMain*)v90->o_Particles;
                    if (v19)
                    {
                        if ((unsigned int)v19 <= 2)
                            v19 = AI_MainActor[(uint32_t)v19 - 1];
                        AI_EvalFunc_PAG_SetGeneratorFreq_C(v19, 1, 10000.0);
                    }
                    else
                    {
                        AI_EvalFunc_PAG_SetGeneratorFreq_C(0, 1, 10000.0);
                    }
                    a3 = (v90->f_ParticleZMaxLow + v90->f_ParticleZMaxHigh - v90->f_ParticleZMaxLow)
                        * ((v90->f_Time - v90->f_ParticleZMaxStart)
                            / v90->f_ParticleZMaxLength);
                }
                else
                {
                    a3 = v90->f_ParticleZMaxHigh;
                    if (v90->f_ParticleStop < v90->f_Time - v90->f_ParticleZMaxStart)
                    {
                        v21 = (tGameObjectMain*)v90->o_Particles;
                        if (v21)
                        {
                            if ((unsigned int)v21 <= 2)
                                v21 = AI_MainActor[(uint32_t)v21 - 1];
                            AI_EvalFunc_PAG_Setf_C(v21, 21, -0.1f);
                        }
                        else
                        {
                            AI_EvalFunc_PAG_Setf_C(0, 21, -0.1f);
                        }
                        v20 = (tGameObjectMain*)v90->o_Particles;
                        if (v20)
                        {
                            if ((unsigned int)v20 <= 2)
                                v20 = AI_MainActor[(uint32_t)v20 - 1];
                            AI_EvalFunc_PAG_SetGeneratorFreq_C(v20, 1, 0.0);
                        }
                        else
                        {
                            AI_EvalFunc_PAG_SetGeneratorFreq_C(0, 1, 0.0);
                        }
                    }
                }
            }
            else
            {
                a3 = v90->f_ParticleZMaxLow;
            }
            v18 = (tGameObjectMain*)v90->o_Particles;
            if (v18)
            {
                if ((unsigned int)v18 <= 2)
                    v18 = AI_MainActor[(uint32_t)v18 - 1];
                v17 = v18;
            }
            else
            {
                v17 = 0;
            }
            Xc = a3 - 2.0f;
            AI_EvalFunc_PAG_Setf_C(v17, 23, Xc);
            v16 = (tGameObjectMain*)v90->o_Particles;
            if (v16)
            {
                if ((unsigned int)v16 <= 2)
                    v16 = AI_MainActor[(uint32_t)v16 - 1];
                AI_EvalFunc_PAG_Setf_C(v16, 25, a3);
            }
            else
            {
                AI_EvalFunc_PAG_Setf_C(0, 25, a3);
            }
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

int monstroplante_death_mpd_waitfordeath()
{
    int result; // eax
    tScriptStruct_MonstroplanteDeath* v1; // [esp+0h] [ebp-50h]
    tGameObjectMain* v2; // [esp+8h] [ebp-48h]
    tGameObjectMain* v3; // [esp+10h] [ebp-40h]
    tGameObjectMain* v4; // [esp+14h] [ebp-3Ch]
    tGameObjectMain* v5; // [esp+18h] [ebp-38h]
    tGameObjectMain* v6; // [esp+20h] [ebp-30h]
    tGameObjectMain* v7; // [esp+24h] [ebp-2Ch]
    tGameObjectMain* v8; // [esp+28h] [ebp-28h]
    tGameObjectMain* v9; // [esp+2Ch] [ebp-24h]
    tGameObjectMain* v10; // [esp+30h] [ebp-20h]
    tGameObjectMain* v11; // [esp+34h] [ebp-1Ch]
    int a2; // [esp+44h] [ebp-Ch]
    int a2a; // [esp+44h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_MonstroplanteDeath*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_monstroplante_death_mpd_waitfordeath_monstroplante_death_mpd_beforedisplay = (int)AI_ul_RealResolveOneRefFunction(0x49020BECu);
        fct_monstroplante_death_mpd_waitfordeath_monstroplante_death_mpd_afterdisplay = (int)AI_ul_RealResolveOneRefFunction(0x49020BEDu);
        fct_monstroplante_death_mpd_waitfordeath_monstroplante_death_mpd_loop = (int)AI_ul_RealResolveOneRefFunction(0x49020BEEu);
    }
    if ((AI_EvalFunc_OBJ_CapaGet_C(AI_gpst_CurrentGameObject) & 1) != 0)
    {
        v11 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0);
        v10 = v11;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v10 = AI_MainActor[(uint32_t)v11 - 1];
            v9 = v10;
        }
        else
        {
            v9 = 0;
        }
        if (v9)
        {
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
            v6 = v11;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v6 = AI_MainActor[(uint32_t)v11 - 1];
                AI_EvalFunc_AICBAdd_C(
                    v6,
                    v7,
                    0,
                    (tAIFunction*)fct_monstroplante_death_mpd_waitfordeath_monstroplante_death_mpd_beforedisplay);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    0,
                    v7,
                    0,
                    (tAIFunction*)fct_monstroplante_death_mpd_waitfordeath_monstroplante_death_mpd_beforedisplay);
            }
            v5 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v5 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v4 = v5;
            }
            else
            {
                v4 = 0;
            }
            v3 = v11;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v3 = AI_MainActor[(uint32_t)v11 - 1];
                AI_EvalFunc_AICBAdd_C(
                    v3,
                    v4,
                    1,
                    (tAIFunction*)fct_monstroplante_death_mpd_waitfordeath_monstroplante_death_mpd_afterdisplay);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    0,
                    v4,
                    1,
                    (tAIFunction*)fct_monstroplante_death_mpd_waitfordeath_monstroplante_death_mpd_afterdisplay);
            }
            for (a2 = 0; a2 < 5; ++a2)
            {
                v2 = v11;
                if (v11)
                {
                    if ((unsigned int)v11 <= 2)
                        v2 = AI_MainActor[(uint32_t)v11 - 1];
                    AI_EvalFunc_OBJ_SetChannelProg_C(v2, a2, 0.0);
                }
                else
                {
                    AI_EvalFunc_OBJ_SetChannelProg_C(0, a2, 0.0);
                }
            }
        }
        for (a2a = 0; a2a < 20; ++a2a)
            v1->ao_ExplodeCircle[a2a] = 0;
        v1->i_CutCam = 0;
        v1->f_Time = 0.0;
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_monstroplante_death_mpd_waitfordeath_monstroplante_death_mpd_loop);
        result = 1000;
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int monstroplante_death_mpd_beforedisplay()
{
    int result; // eax
    double v1; // [esp+4h] [ebp-20h]
    float v2; // [esp+10h] [ebp-14h]
    float v3; // [esp+14h] [ebp-10h]
    float v4; // [esp+14h] [ebp-10h]
    int a4; // [esp+18h] [ebp-Ch]
    float a5; // [esp+1Ch] [ebp-8h]

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        monstroplante_death_mpd_beforedisplay_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &monstroplante_death_mpd_beforedisplay_INV_manager_gao_f_Timer,
            (void*)monstroplante_death_mpd_beforedisplay_INV_manager_gao,
            176);
    }
    v2 = *(float*)monstroplante_death_mpd_beforedisplay_INV_manager_gao_f_Timer;
    if (*(float*)monstroplante_death_mpd_beforedisplay_INV_manager_gao_f_Timer >= 1.8)
    {
        if (v2 <= 4.3000002)
        {
            if (v2 >= 2.5999999)
            {
                a4 = 200;
                if (v2 >= 2.5)
                {
                    if (v2 >= 3.3)
                    {
                        a5 = 9.1999998f;
                    }
                    else
                    {
                        v4 = (v2 - 2.5f) / 0.79999995f;
                        a5 = v4 * -2.8000002f + 12.0f;
                    }
                }
                else
                {
                    a5 = 12.0;
                }
            }
            else
            {
                v3 = (v2 - 1.8f) / 0.79999995f;
                a5 = v3 * -3.0f + 15.0f;
                v1 = v3 * 200.0 + MATH_gd_magicDoubleForRound;
                a4 = (*(uint32_t *)&v1);
            }
            AI_EvalFunc_MATEnableMltxtLvl_C(AI_gpst_CurrentGameObject, 1u, 1, 1);
            AI_EvalFunc_SetLocalAlpha_C(AI_gpst_CurrentGameObject, 1u, 1, a4);
            AI_EvalFunc_SetUV_C(AI_gpst_CurrentGameObject, 1, 1, 1, a5);
            result = 1000;
        }
        else
        {
            AI_EvalFunc_MATEnableMltxtLvl_C(AI_gpst_CurrentGameObject, 1u, 0, 0);
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

int monstroplante_death_mpd_afterdisplay()
{
    AI_EvalFunc_MATEnableMltxtLvl_C(AI_gpst_CurrentGameObject, 1u, 0, 1);
    AI_EvalFunc_MATEnableMltxtLvl_C(AI_gpst_CurrentGameObject, 1u, 1, 0);
    return 1000;
}

int monstroplante_death_mpd_init()
{
    tGameObjectMain* v1; // [esp+8h] [ebp-20h]
    tGameObjectMain* v2; // [esp+10h] [ebp-18h]
    tGameObjectMain* v3; // [esp+14h] [ebp-14h]
    tGameObjectMain* v4; // [esp+18h] [ebp-10h]
    tGameObjectMain* v5; // [esp+1Ch] [ebp-Ch]

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_monstroplante_death_mpd_init_monstroplante_death_mpd_waitfordeath = (int)AI_ul_RealResolveOneRefFunction(0x49020E9Bu);
    }
    v5 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0);
    v4 = v5;
    if (v5)
    {
        if ((unsigned int)v5 <= 2)
            v4 = AI_MainActor[(uint32_t)v5 - 1];
        v3 = v4;
    }
    else
    {
        v3 = 0;
    }
    if (v3)
    {
        v2 = v5;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v2 = AI_MainActor[(uint32_t)v5 - 1];
            AI_EvalFunc_OBJ_SetChannelProg_C(v2, 0, 0.0);
        }
        else
        {
            AI_EvalFunc_OBJ_SetChannelProg_C(0, 0, 0.0);
        }
        v1 = v5;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v1 = AI_MainActor[(uint32_t)v5 - 1];
            AI_EvalFunc_OBJ_SetChannelFactor_C(v1, 0, 1.0);
        }
        else
        {
            AI_EvalFunc_OBJ_SetChannelFactor_C(0, 0, 1.0);
        }
    }
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_monstroplante_death_mpd_init_monstroplante_death_mpd_waitfordeath);
    return 1000;
}