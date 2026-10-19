#include "ai_compiledscripts_inc.h"

//Hyllis_Meteor_Manager_HMM_mode_aircraft
int Hyllis_Meteor_Manager_HMM_mode_aircraft_SFX_explosion_gao;
int fct_Hyllis_Meteor_Manager_HMM_mode_aircraft_Hyllis_Meteor_Manager_HMM_mode_aircraft;
int Hyllis_Meteor_Manager_HMM_mode_aircraft_univVars;
int Hyllis_Meteor_Manager_HMM_mode_aircraft_univVars_Flag_speciaux_sauve;
int fct_Hyllis_Meteor_Manager_HMM_mode_aircraft_Hyllis_Meteor_Manager_HMM_mode_reinit;
int fct_Hyllis_Meteor_Manager_HMM_mode_aircraft_Hyllis_Meteor_Manager_HMM_init;

//Hyllis_Meteor_Manager_HMM_mode_reinit
int Hyllis_Meteor_Manager_HMM_mode_reinit_univVars;
int Hyllis_Meteor_Manager_HMM_mode_reinit_univVars_Flag_speciaux_sauve;

//Hyllis_Meteor_Manager_HMM_init
int fct_Hyllis_Meteor_Manager_HMM_init_Hyllis_Meteor_Manager_HMM_mode_wait;

//Hyllis_Meteor_Manager_HMM_mode_wait
int Hyllis_Meteor_Manager_HMM_mode_wait_univVars;
int Hyllis_Meteor_Manager_HMM_mode_wait_univVars_Flag_speciaux_sauve;
int fctvar_Hyllis_Meteor_Manager_HMM_mode_wait_Domz_Sansue;
int fct_Hyllis_Meteor_Manager_HMM_mode_wait_Hyllis_Meteor_Manager_HMM_mode_wait;
int fct_Hyllis_Meteor_Manager_HMM_mode_wait_Hyllis_Meteor_Manager_HMM_mode_aircraft;
int fct_Hyllis_Meteor_Manager_HMM_mode_wait_Hyllis_Meteor_Manager_HMM_mode_vaisseau;

//Hyllis_Meteor_Manager_HMM_mode_vaisseau
int fct_Hyllis_Meteor_Manager_HMM_mode_vaisseau_Hyllis_Meteor_Manager_HMM_mode_vaisseau;
int Hyllis_Meteor_Manager_HMM_mode_vaisseau_univVars;
int Hyllis_Meteor_Manager_HMM_mode_vaisseau_univVars_Flag_speciaux_sauve;
int fct_Hyllis_Meteor_Manager_HMM_mode_vaisseau_Hyllis_Meteor_Manager_HMM_mode_reinit;
int fct_Hyllis_Meteor_Manager_HMM_mode_vaisseau_Hyllis_Meteor_Manager_HMM_init;

int Hyllis_Meteor_Manager_HMM_mode_aircraft()
{
    Vec3* v0; // eax
    char* v1; // eax
    Vec3* v2; // eax
    tGameObjectMain* v3; // eax
    int result; // eax
    Vec3* v5; // eax
    char* v6; // eax
    float v7; // [esp+4h] [ebp-2CCh]
    char* v8; // [esp+8h] [ebp-2C8h]
    int v9; // [esp+Ch] [ebp-2C4h]
    float v10; // [esp+10h] [ebp-2C0h]
    float v11; // [esp+14h] [ebp-2BCh]
    char* v12; // [esp+18h] [ebp-2B8h]
    tScriptStruct_HyllisMeteorManager* v13; // [esp+1Ch] [ebp-2B4h]
    tGameObjectMain* v14; // [esp+28h] [ebp-2A8h]
    tGameObjectMain* v15; // [esp+30h] [ebp-2A0h]
    Vec3* v16; // [esp+34h] [ebp-29Ch]
    tGameObjectMain* v17; // [esp+38h] [ebp-298h]
    tGameObjectMain* v18; // [esp+3Ch] [ebp-294h]
    tGameObjectMain* v19; // [esp+44h] [ebp-28Ch]
    tGameObjectMain* v20; // [esp+4Ch] [ebp-284h]
    tGameObjectMain* v21; // [esp+50h] [ebp-280h]
    tGameObjectMain* v22; // [esp+54h] [ebp-27Ch]
    tGameObjectMain* v23; // [esp+5Ch] [ebp-274h]
    tGameObjectMain* v24; // [esp+64h] [ebp-26Ch]
    tGameObjectMain* v25; // [esp+6Ch] [ebp-264h]
    tGameObjectMain* v26; // [esp+74h] [ebp-25Ch]
    tGameObjectMain* v27; // [esp+7Ch] [ebp-254h]
    tGameObjectMain* v28; // [esp+8Ch] [ebp-244h]
    tGameObjectMain* v29; // [esp+94h] [ebp-23Ch]
    tGameObjectMain* v30; // [esp+9Ch] [ebp-234h]
    tGameObjectMain* v31; // [esp+A4h] [ebp-22Ch]
    float v32; // [esp+ACh] [ebp-224h]
    tGameObjectMain* v33; // [esp+C0h] [ebp-210h]
    tGameObjectMain* v34; // [esp+D0h] [ebp-200h]
    tGameObjectMain* v35; // [esp+E4h] [ebp-1ECh]
    tGameObjectMain* v36; // [esp+FCh] [ebp-1D4h]
    Vec3 v37; // [esp+100h] [ebp-1D0h] BYREF
    float v38; // [esp+10Ch] [ebp-1C4h]
    float v39; // [esp+110h] [ebp-1C0h]
    float v40; // [esp+114h] [ebp-1BCh]
    int v41; // [esp+118h] [ebp-1B8h]
    int v42; // [esp+11Ch] [ebp-1B4h]
    float v43; // [esp+120h] [ebp-1B0h]
    Vec3 v44; // [esp+124h] [ebp-1ACh] BYREF
    float v45; // [esp+134h] [ebp-19Ch]
    Vec3 v46; // [esp+138h] [ebp-198h] BYREF
    float v47; // [esp+144h] [ebp-18Ch]
    Vec3 a3; // [esp+14Ch] [ebp-184h] BYREF
    Vec3 v49; // [esp+158h] [ebp-178h] BYREF
    float v50; // [esp+164h] [ebp-16Ch]
    Vec3 pos; // [esp+168h] [ebp-168h] BYREF
    Vec3 v52; // [esp+174h] [ebp-15Ch] BYREF
    float v53; // [esp+190h] [ebp-140h]
    float v54; // [esp+194h] [ebp-13Ch]
    float v55; // [esp+198h] [ebp-138h]
    float v56; // [esp+19Ch] [ebp-134h]
    Vec3 v57; // [esp+1A0h] [ebp-130h] BYREF
    float v58; // [esp+1ACh] [ebp-124h]
    float v59; // [esp+1B0h] [ebp-120h]
    float v60; // [esp+1B4h] [ebp-11Ch]
    Vec3 a1; // [esp+1B8h] [ebp-118h] BYREF
    float v62; // [esp+1C4h] [ebp-10Ch]
    float v63; // [esp+1C8h] [ebp-108h]
    float v64; // [esp+1CCh] [ebp-104h]
    BOOL v65; // [esp+1D0h] [ebp-100h]
    int v66; // [esp+1D8h] [ebp-F8h]
    float v67; // [esp+1DCh] [ebp-F4h]
    int v68; // [esp+1E4h] [ebp-ECh]
    Vec3 v69; // [esp+1E8h] [ebp-E8h] BYREF
    tMessage Src; // [esp+1F4h] [ebp-DCh] BYREF
    float v71; // [esp+228h] [ebp-A8h]
    Vec3 v72; // [esp+230h] [ebp-A0h] BYREF
    int v73; // [esp+23Ch] [ebp-94h]
    float v74; // [esp+240h] [ebp-90h]
    float v75; // [esp+244h] [ebp-8Ch]
    Vec3 v76; // [esp+248h] [ebp-88h] BYREF
    Vec3 v77; // [esp+258h] [ebp-78h] BYREF
    tScriptStruct_HyllisMeteorManager* v78; // [esp+264h] [ebp-6Ch]
    tGameObjectMain* v79; // [esp+268h] [ebp-68h]
    tGameObjectMain* v80; // [esp+26Ch] [ebp-64h]
    int i; // [esp+270h] [ebp-60h]
    float v82; // [esp+274h] [ebp-5Ch]
    float v83; // [esp+278h] [ebp-58h]
    float v84; // [esp+27Ch] [ebp-54h]
    Vec3 v85; // [esp+280h] [ebp-50h] BYREF
    Vec3 v86; // [esp+298h] [ebp-38h] BYREF
    float v87; // [esp+2A4h] [ebp-2Ch]
    Vec3 v88; // [esp+2A8h] [ebp-28h] BYREF
    Vec3 v89; // [esp+2B4h] [ebp-1Ch] BYREF
    float X; // [esp+2C0h] [ebp-10h]
    float v91; // [esp+2C4h] [ebp-Ch]
    float v92; // [esp+2C8h] [ebp-8h]
    float v93; // [esp+2CCh] [ebp-4h]

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    v52.x = 1.0;
    v52.y = 0.0;
    v52.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v13 = (tScriptStruct_HyllisMeteorManager*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v13 = 0;
    v78 = v13;
    v73 = 0;
    X = 0.0;
    v65 = 0;
    v66 = 0;
    v79 = 0;
    v92 = 0.0;
    v80 = 0;
    v93 = 0.0;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Hyllis_Meteor_Manager_HMM_mode_aircraft_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
        fct_Hyllis_Meteor_Manager_HMM_mode_aircraft_Hyllis_Meteor_Manager_HMM_mode_aircraft = (int)AI_ul_RealResolveOneRefFunction(0x72004D7Cu);
        Hyllis_Meteor_Manager_HMM_mode_aircraft_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Hyllis_Meteor_Manager_HMM_mode_aircraft_univVars_Flag_speciaux_sauve,
            (void*)Hyllis_Meteor_Manager_HMM_mode_aircraft_univVars,
            4488);
        fct_Hyllis_Meteor_Manager_HMM_mode_aircraft_Hyllis_Meteor_Manager_HMM_mode_reinit = (int)AI_ul_RealResolveOneRefFunction(0x72004D90u);
        fct_Hyllis_Meteor_Manager_HMM_mode_aircraft_Hyllis_Meteor_Manager_HMM_init = (int)AI_ul_RealResolveOneRefFunction(0x72004D7Au);
    }
    if (v78->i_current_mode == 1)
    {
        v28 = (tGameObjectMain*)v78->o_aircraft;
        if (v28)
        {
            if ((unsigned int)v28 <= 2)
                v28 = AI_MainActor[(uint32_t)v28 - 1];
            AI_EvalFunc_OBJGetPos_C(v28, &v72);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v72);
        }
        v52 = v72;
        v27 = (tGameObjectMain*)v78->o_aircraft;
        if (v27)
        {
            if ((unsigned int)v27 <= 2)
                v27 = AI_MainActor[(uint32_t)v27 - 1];
            AI_EvalFunc_DYNSpeedGetVector_C(v27, &v44);
        }
        else
        {
            AI_EvalFunc_DYNSpeedGetVector_C(0, &v44);
        }
        v87 = TIM_gf_dt;
        v58 = v44.x * TIM_gf_dt;
        v59 = v44.y * TIM_gf_dt;
        v60 = v44.z * TIM_gf_dt;
        v52.x = v52.x + v58;
        v52.y = v52.y + v59;
        v52.z = v52.z + v60;
        v26 = (tGameObjectMain*)v78->o_aircraft;
        if (v26)
        {
            if ((unsigned int)v26 <= 2)
                v26 = AI_MainActor[(uint32_t)v26 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v26, &v77);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &v77);
        }
        v54 = v77.x + v77.x;
        v55 = v77.y + v77.y;
        v56 = v77.z + v77.z;
        v52.x = v52.x + v54;
        v52.y = v52.y + v55;
        v52.z = v52.z + v56;
        v73 = 0;
        for (i = 0; i < v78->i_nb_meteor_aircraft; ++i)
        {
            if (*(float*)AI_EvalVarArray_C2((char*)&v78->__f_Z_hors_champ_Dimension1, i) > 0.0)
            {
                v74 = TIM_gf_dt;
                v8 = AI_EvalVarArray_C2((char*)&v78->__f_Z_hors_champ_Dimension1, i);
                v6 = AI_EvalVarArray_C2((char*)&v78->__f_speed_Dimension1, i);
                *(float*)v8 = *(float*)v8 - *(float*)v6 * v74;
                ++v73;
                v7 = *(float*)AI_EvalVarArray_C2((char*)&v78->__f_Z_hors_champ_Dimension1, i);
                v93 = 1.0f - v7 / *(float*)AI_EvalVarArray_C2((char*)&v78->__f_Z_hors_champ_save_Dimension1, i);
                v93 = v93 * 0.75f;
                v93 = v93 + 0.25f;
                v18 = (tGameObjectMain*)v78->o_meteor[i];
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
                if (v17)
                {
                    v16 = (Vec3*)AI_EvalVarArray_C3((char*)&v78->__v_offset_Dimension1, i);
                    v37.x = v93 * v16->x;
                    v37.y = v16->y * v93;
                    v37.z = v16->z * v93;
                    v46.x = 0.0;
                    v46.y = 0.0;
                    v46.z = *(float*)AI_EvalVarArray_C2((char*)&v78->__f_Z_hors_champ_Dimension1, i);
                    MATH_VecAddVec(&v76, &v37, &v46);
                    MATH_VecAddVec(&v85, &v52, &v76);
                    v15 = (tGameObjectMain*)v78->o_meteor[i];
                    if (v15)
                    {
                        if ((unsigned int)v15 <= 2)
                            v15 = AI_MainActor[(uint32_t)v15 - 1];
                        AI_EvalFunc_OBJSetPos_C(v15, &v85);
                    }
                    else
                    {
                        AI_EvalFunc_OBJSetPos_C(0, &v85);
                    }
                }
            }
            else
            {
                *(uint32_t*)AI_EvalVarArray_C2((char*)&v78->__f_Z_hors_champ_Dimension1, i) = 0;
                if (!i && v78->i_flag_explode)
                {
                    v78->i_flag_explode = 0;
                    v25 = (tGameObjectMain*)v78->o_aircraft;
                    if (v25)
                    {
                        if ((unsigned int)v25 <= 2)
                            v25 = AI_MainActor[(uint32_t)v25 - 1];
                        AI_EvalFunc_OBJ_CapaSet_C(v25, 3072, 0);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_CapaSet_C(0, 3072, 0);
                    }
                    v80 = AI_EvalFunc_OBJDuplicate_C(
                        (tGameObjectMain*)Hyllis_Meteor_Manager_HMM_mode_aircraft_SFX_explosion_gao,
                        &v52);
                    Src.msgID = 1;
                    Src.Int1 = 37;
                    Src.f2 = 15.0;
                    Src.f1 = 0.0;
                    Src.v1.x = 1.0;
                    Src.v1.y = 0.0;
                    Src.v1.z = 0.0;
                    v24 = v80;
                    if (v80)
                    {
                        if ((unsigned int)v80 <= 2)
                            v24 = AI_MainActor[(uint32_t)v80 - 1];
                        AI_EvalFunc_MSGSend_C(v24, &Src);
                    }
                    else
                    {
                        AI_EvalFunc_MSGSend_C(0, &Src);
                    }
                    v66 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 3, 0x18000080);
                    v23 = (tGameObjectMain*)v78->o_aircraft;
                    if (v23)
                    {
                        if ((unsigned int)v23 <= 2)
                            v23 = AI_MainActor[(uint32_t)v23 - 1];
                        AI_EvalFunc_SNDUltraPlaySound_C(v23, v66);
                    }
                    else
                    {
                        AI_EvalFunc_SNDUltraPlaySound_C(0, v66);
                    }
                }
                v22 = (tGameObjectMain*)v78->o_meteor[i];
                if (v22)
                {
                    if ((unsigned int)v22 <= 2)
                        v22 = AI_MainActor[(uint32_t)v22 - 1];
                    v21 = v22;
                }
                else
                {
                    v21 = 0;
                }
                if (v21)
                {
                    ++v73;
                    v5 = (Vec3*)AI_EvalVarArray_C3((char*)&v78->__v_offset_Dimension1, i);
                    MATH_VecAddVec(&v89, &v52, v5);
                    v20 = (tGameObjectMain*)v78->o_meteor[i];
                    if (v20)
                    {
                        if ((unsigned int)v20 <= 2)
                            v20 = AI_MainActor[(uint32_t)v20 - 1];
                        AI_EvalFunc_OBJSetPos_C(v20, &v89);
                    }
                    else
                    {
                        AI_EvalFunc_OBJSetPos_C(0, &v89);
                    }
                    v19 = (tGameObjectMain*)v78->o_meteor[i];
                    if (v19)
                    {
                        if ((unsigned int)v19 <= 2)
                            v19 = AI_MainActor[(uint32_t)v19 - 1];
                        AI_EvalFunc_OBJ_CapaSet_C(v19, 0, 0x100);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_CapaSet_C(0, 0, 0x100);
                    }
                }
            }
        }
        if (!v73)
        {
            v14 = (tGameObjectMain*)v78->o_aircraft;
            if (v14)
            {
                if ((unsigned int)v14 <= 2)
                    v14 = AI_MainActor[(uint32_t)v14 - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v14,
                    2,
                    (tAIFunction*)fct_Hyllis_Meteor_Manager_HMM_mode_aircraft_Hyllis_Meteor_Manager_HMM_mode_aircraft);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_Hyllis_Meteor_Manager_HMM_mode_aircraft_Hyllis_Meteor_Manager_HMM_mode_aircraft);
            }
            v65 = AI_EvalFunc_ARRBitTest_C(
                (int*)(Hyllis_Meteor_Manager_HMM_mode_aircraft_univVars_Flag_speciaux_sauve + 4),
                2789);
            if (v65)
                AI_EvalFunc_AITrackChange_C(
                    0,
                    (tAIFunction*)fct_Hyllis_Meteor_Manager_HMM_mode_aircraft_Hyllis_Meteor_Manager_HMM_init);
            else
                AI_EvalFunc_AITrackChange_C(
                    0,
                    (tAIFunction*)fct_Hyllis_Meteor_Manager_HMM_mode_aircraft_Hyllis_Meteor_Manager_HMM_mode_reinit);
        }
        result = 1000;
    }
    else
    {
        v78->i_current_mode = 1;
        v36 = (tGameObjectMain*)v78->o_aircraft;
        if (v36)
        {
            if ((unsigned int)v36 <= 2)
                v36 = AI_MainActor[(uint32_t)v36 - 1];
            AI_EvalFunc_OBJGetPos_C(v36, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        v52 = a3;
        v79 = AI_EvalFunc_VIEWGetObject_C(0);
        v71 = AI_EvalFunc_VIEWFocaleGet_C(0);
        X = v71 * 0.5f;
        v92 = cosf(X);
        *(uint32_t*)AI_EvalVarArray_C2((char*)&v78->__f_Z_hors_champ_Dimension1, 0) = 0x43C80000;
        a1 = v52;
        v41 = 0;
        v42 = 0;
        v43 = *(float*)AI_EvalVarArray_C2((char*)&v78->__f_Z_hors_champ_Dimension1, 0);
        a1.x = a1.x + 0.0f;
        a1.y = a1.y + 0.0f;
        a1.z = a1.z + v43;
        v35 = v79;
        if (v79)
        {
            if ((unsigned int)v79 <= 2)
                v35 = AI_MainActor[(uint32_t)v79 - 1];
            AI_EvalFunc_OBJGetPos_C(v35, &v57);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v57);
        }
        a1.x = a1.x - v57.x;
        a1.y = a1.y - v57.y;
        a1.z = a1.z - v57.z;
        MATH_NormalizeEqualVector(&a1);
        while (1)
        {
            v34 = v79;
            if (v79)
            {
                if ((unsigned int)v79 <= 2)
                    v34 = AI_MainActor[(uint32_t)v79 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v34, &v86);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v86);
            }
            v47 = a1.z * v86.z + a1.y * v86.y + v86.x * a1.x;
            if (v47 <= (double)v92)
                break;
            v12 = AI_EvalVarArray_C2((char*)&v78->__f_Z_hors_champ_Dimension1, 0);
            *(float*)v12 = AI_EvalFunc_MATHRandFloat_C(100.0, 200.0) + *(float*)v12;
            a1 = v52;
            v82 = 0.0;
            v83 = 0.0;
            v84 = *(float*)AI_EvalVarArray_C2((char*)&v78->__f_Z_hors_champ_Dimension1, 0);
            a1.x = a1.x + v82;
            a1.y = a1.y + v83;
            a1.z = a1.z + v84;
            v33 = v79;
            if (v79)
            {
                if ((unsigned int)v79 <= 2)
                    v33 = AI_MainActor[(uint32_t)v79 - 1];
                AI_EvalFunc_OBJGetPos_C(v33, &v49);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v49);
            }
            a1.x = a1.x - v49.x;
            a1.y = a1.y - v49.y;
            a1.z = a1.z - v49.z;
            MATH_NormalizeEqualVector(&a1);
        }
        v62 = 0.0;
        v63 = 0.0;
        v64 = *(float*)AI_EvalVarArray_C2((char*)&v78->__f_Z_hors_champ_Dimension1, 0);
        v52.x = v52.x + v62;
        v52.y = v52.y + v63;
        v52.z = v52.z + v64;
        for (i = 0; i < v78->i_nb_meteor_aircraft; ++i)
        {
            if (i)
            {
                v45 = AI_EvalFunc_MATHRandFloat_C(100.0, 250.0);
                v91 = AI_EvalFunc_MATHRandFloat_C(-1.0, 1.0);
                v67 = AI_EvalFunc_MTHFloatSign_C(v91);
                v50 = AI_EvalFunc_MATHRandFloat_C(0.5, 1.0);
                v53 = AI_EvalFunc_MATHRandFloat_C(0.5, 1.0);
                v38 = v50;
                v39 = v53;
                v40 = 0.0;
                v32 = v45 * v67;
                v0 = (Vec3*)AI_EvalVarArray_C3((char*)&v78->__v_offset_Dimension1, i);
                v0->x = v38 * v32;
                v0->y = v39 * v32;
                v0->z = v40 * v32;
                v75 = AI_EvalFunc_MATHRandFloat_C(50.0, 200.0);
                v1 = AI_EvalVarArray_C2((char*)&v78->__f_Z_hors_champ_Dimension1, 0);
                v11 = *(float*)v1 + v75;
                *(float*)AI_EvalVarArray_C2((char*)&v78->__f_Z_hors_champ_Dimension1, i) = v11;
            }
            v10 = *(float*)AI_EvalVarArray_C2((char*)&v78->__f_Z_hors_champ_Dimension1, i);
            *(float*)AI_EvalVarArray_C2((char*)&v78->__f_Z_hors_champ_save_Dimension1, i) = v10;
            v68 = AI_EvalFunc_MATHRandInt_C(20, 30);
            v9 = 10 * v68;
            *(float*)AI_EvalVarArray_C2((char*)&v78->__f_speed_Dimension1, i) = (float)v9;
            v88.x = 0.0;
            v88.y = 0.0;
            v88.z = *(float*)AI_EvalVarArray_C2((char*)&v78->__f_Z_hors_champ_Dimension1, i);
            v2 = (Vec3*)AI_EvalVarArray_C3((char*)&v78->__v_offset_Dimension1, i);
            MATH_VecAddVec(&v69, v2, &v88);
            MATH_VecAddVec(&pos, &v52, &v69);
            v31 = (tGameObjectMain*)v78->des_meteor;
            if (v31)
            {
                if ((unsigned int)v31 <= 2)
                    v31 = AI_MainActor[(uint32_t)v31 - 1];
                v3 = AI_EvalFunc_OBJDuplicate_C(v31, &pos);
            }
            else
            {
                v3 = AI_EvalFunc_OBJDuplicate_C(0, &pos);
            }
            v78->o_meteor[i] = (int)v3;
            v30 = (tGameObjectMain*)v78->o_meteor[i];
            if (v30)
            {
                if ((unsigned int)v30 <= 2)
                    v30 = AI_MainActor[(uint32_t)v30 - 1];
                AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, v30, &v78->o_meteor[i]);
            }
            else
            {
                AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, 0, &v78->o_meteor[i]);
            }
            v29 = (tGameObjectMain*)v78->o_meteor[i];
            if (v29)
            {
                if ((unsigned int)v29 <= 2)
                    v29 = AI_MainActor[(uint32_t)v29 - 1];
                AI_EvalFunc_OBJ_CapaSet_C(v29, 0x100, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_CapaSet_C(0, 0x100, 0);
            }
        }
        v78->i_flag_explode = 1;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 4;
    }
    return result;
}

int Hyllis_Meteor_Manager_HMM_mode_reinit()
{
    tScriptStruct_HyllisMeteorManager* v1; // [esp+0h] [ebp-1Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_HyllisMeteorManager*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Hyllis_Meteor_Manager_HMM_mode_reinit_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Hyllis_Meteor_Manager_HMM_mode_reinit_univVars_Flag_speciaux_sauve,
            (void*)Hyllis_Meteor_Manager_HMM_mode_reinit_univVars,
            4488);
    }
    if (v1->i_current_mode != 3)
    {
        v1->i_current_mode = 3;
        v1->f_time_start_wait = TIM_gf_MainClock - TIM_gf_LockTime;
        v1->f_delai_before_reinit = AI_EvalFunc_MATHRandFloat_C(120.0, 240.0);
    }
    if (AI_EvalFunc_ARRBitTest_C((int*)(Hyllis_Meteor_Manager_HMM_mode_reinit_univVars_Flag_speciaux_sauve + 4), 2718))
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
    else
        AI_EvalFunc_OBJReinit_C(AI_gpst_CurrentGameObject, 1);
    return 1000;
}

int Hyllis_Meteor_Manager_HMM_init()
{
    tGameObjectMain* v1; // [esp+8h] [ebp-Ch]

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Hyllis_Meteor_Manager_HMM_init_Hyllis_Meteor_Manager_HMM_mode_wait = (int)AI_ul_RealResolveOneRefFunction(0x72004D7Bu);
    }
    v1 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v1 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v1,
            2,
            (tAIFunction*)fct_Hyllis_Meteor_Manager_HMM_init_Hyllis_Meteor_Manager_HMM_mode_wait);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            2,
            (tAIFunction*)fct_Hyllis_Meteor_Manager_HMM_init_Hyllis_Meteor_Manager_HMM_mode_wait);
    }
    AI_EvalFunc_AITrackCurStop_C();
    return 1000;
}

int Hyllis_Meteor_Manager_HMM_mode_wait()
{
    int result; // eax
    BOOL v1; // eax
    BOOL v2; // eax
    BOOL v3; // eax
    double v4; // st7
    float X; // [esp+0h] [ebp-220h]
    tScriptStruct_HyllisMeteorManager* v6; // [esp+4h] [ebp-21Ch]
    tGameObjectMain* v7; // [esp+Ch] [ebp-214h]
    tGameObjectMain* v8; // [esp+14h] [ebp-20Ch]
    tGameObjectMain* v9; // [esp+1Ch] [ebp-204h]
    tGameObjectMain* v10; // [esp+38h] [ebp-1E8h]
    tGameObjectMain* v11; // [esp+40h] [ebp-1E0h]
    tGameObjectMain* v12; // [esp+48h] [ebp-1D8h]
    tGameObjectMain* v13; // [esp+4Ch] [ebp-1D4h]
    tGameObjectMain* v14; // [esp+50h] [ebp-1D0h]
    tGameObjectMain* v15; // [esp+58h] [ebp-1C8h]
    tGameObjectMain* v16; // [esp+5Ch] [ebp-1C4h]
    tGameObjectMain* v17; // [esp+60h] [ebp-1C0h]
    tGameObjectMain* v18; // [esp+64h] [ebp-1BCh]
    tGameObjectMain* v19; // [esp+68h] [ebp-1B8h]
    tGameObjectMain* v20; // [esp+70h] [ebp-1B0h]
    tGameObjectMain* v21; // [esp+78h] [ebp-1A8h]
    tGameObjectMain* v22; // [esp+80h] [ebp-1A0h]
    tGameObjectMain* v23; // [esp+90h] [ebp-190h]
    tGameObjectMain* v24; // [esp+98h] [ebp-188h]
    float v25; // [esp+9Ch] [ebp-184h]
    tGameObjectMain* v26; // [esp+B0h] [ebp-170h]
    tGameObjectMain* v27; // [esp+B8h] [ebp-168h]
    tGameObjectMain* v28; // [esp+C0h] [ebp-160h]
    tGameObjectMain* v29; // [esp+C4h] [ebp-15Ch]
    tGameObjectMain* v30; // [esp+C8h] [ebp-158h]
    tGameObjectMain* v31; // [esp+CCh] [ebp-154h]
    tGameObjectMain* v32; // [esp+D0h] [ebp-150h]
    tGameObjectMain* v33; // [esp+D8h] [ebp-148h]
    tAI* v34; // [esp+DCh] [ebp-144h]
    tAI* v35; // [esp+E0h] [ebp-140h]
    tGameObjectMain* v36; // [esp+E4h] [ebp-13Ch]
    tGameObjectMain* v37; // [esp+E8h] [ebp-138h]
    tAI* v38; // [esp+ECh] [ebp-134h]
    tAI* v39; // [esp+F0h] [ebp-130h]
    tGameObjectMain* v40; // [esp+F4h] [ebp-12Ch]
    tGameObjectMain* v41; // [esp+F8h] [ebp-128h]
    tGameObjectMain* v42; // [esp+FCh] [ebp-124h]
    tGameObjectMain* v43; // [esp+100h] [ebp-120h]
    tGameObjectMain* v44; // [esp+108h] [ebp-118h]
    tGameObjectMain* v45; // [esp+10Ch] [ebp-114h]
    float v46; // [esp+110h] [ebp-110h]
    float v47; // [esp+114h] [ebp-10Ch]
    Vec3 v48; // [esp+120h] [ebp-100h] BYREF
    BOOL v49; // [esp+12Ch] [ebp-F4h]
    Vec3 v50; // [esp+130h] [ebp-F0h] BYREF
    BOOL v51; // [esp+13Ch] [ebp-E4h]
    Vec3 v52; // [esp+140h] [ebp-E0h] BYREF
    Vec3 dest; // [esp+14Ch] [ebp-D4h] BYREF
    Vec3 v54; // [esp+15Ch] [ebp-C4h] BYREF
    float v55; // [esp+168h] [ebp-B8h]
    float v56; // [esp+16Ch] [ebp-B4h]
    tGameObjectMain* v57; // [esp+170h] [ebp-B0h]
    float v58; // [esp+174h] [ebp-ACh]
    Vec3 a1; // [esp+17Ch] [ebp-A4h] BYREF
    Vec3 v60; // [esp+188h] [ebp-98h] BYREF
    Vec3 a3; // [esp+19Ch] [ebp-84h] BYREF
    float v62; // [esp+1ACh] [ebp-74h]
    BOOL v63; // [esp+1B0h] [ebp-70h]
    Vec3 v; // [esp+1B4h] [ebp-6Ch] BYREF
    Vec3 v65; // [esp+1C0h] [ebp-60h] BYREF
    BOOL v66; // [esp+1CCh] [ebp-54h]
    tScriptStruct_HyllisMeteorManager* v67; // [esp+1D8h] [ebp-48h]
    float v68; // [esp+1DCh] [ebp-44h]
    tGameObjectMain* v69; // [esp+1E0h] [ebp-40h]
    float v70; // [esp+1E4h] [ebp-3Ch]
    unsigned int v71; // [esp+1E8h] [ebp-38h]
    tGameObjectMain* v72; // [esp+1ECh] [ebp-34h]
    int i; // [esp+1F0h] [ebp-30h]
    Vec3 v74; // [esp+1F4h] [ebp-2Ch] BYREF
    Vec3 v75; // [esp+200h] [ebp-20h] BYREF
    int v76; // [esp+20Ch] [ebp-14h]
    Vec3 v77; // [esp+210h] [ebp-10h] BYREF
    int v78; // [esp+21Ch] [ebp-4h]

    v.x = 1.0;
    v.y = 0.0;
    v.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_HyllisMeteorManager*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v67 = v6;
    v66 = 0;
    v68 = 0.0;
    v69 = 0;
    v70 = 0.0;
    v71 = 0;
    v62 = 0.0;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Hyllis_Meteor_Manager_HMM_mode_wait_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Hyllis_Meteor_Manager_HMM_mode_wait_univVars_Flag_speciaux_sauve,
            (void*)Hyllis_Meteor_Manager_HMM_mode_wait_univVars,
            4488);
        fctvar_Hyllis_Meteor_Manager_HMM_mode_wait_Domz_Sansue = AI_ul_RealResolveOneRef(42, 0x72004C60u);
        fct_Hyllis_Meteor_Manager_HMM_mode_wait_Hyllis_Meteor_Manager_HMM_mode_wait = (int)AI_ul_RealResolveOneRefFunction(0x72004D7Bu);
        fct_Hyllis_Meteor_Manager_HMM_mode_wait_Hyllis_Meteor_Manager_HMM_mode_aircraft = (int)AI_ul_RealResolveOneRefFunction(0x72004D7Cu);
        fct_Hyllis_Meteor_Manager_HMM_mode_wait_Hyllis_Meteor_Manager_HMM_mode_vaisseau = (int)AI_ul_RealResolveOneRefFunction(0x72004D8Cu);
    }
    if (v67->i_current_mode)
    {
        v67->i_current_mode = 0;
        v67->f_time_start_wait = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    v67->o_main_actor = (int)*AI_MainActor;
    v45 = (tGameObjectMain*)v67->o_main_actor;
    if (v45)
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
        v66 = AI_EvalFunc_ARRBitTest_C((int*)(Hyllis_Meteor_Manager_HMM_mode_wait_univVars_Flag_speciaux_sauve + 4), 2789);
        if (v66)
        {
            v43 = (tGameObjectMain*)v67->des_gameplay_manager;
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
            if (!v42)
                goto LABEL_46;
            v39 = (tAI*)v67->des_gameplay_manager;
            if (v39 == WOR_gst_Universe.univAIInstance)
            {
                v38 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v41 = (tGameObjectMain*)v67->des_gameplay_manager;
                if (v39)
                {
                    if ((unsigned int)v39 <= 2)
                        v41 = AI_MainActor[(uint32_t)v39 - 1];
                    v40 = v41;
                }
                else
                {
                    v40 = 0;
                }
                v38 = v40->extended->AIInstance;
            }
            i = ((tScriptStruct_GSTHyllisManager*)(v38->instanceVarDataCurrent))->i_last_gp;
            v35 = (tAI*)v67->des_gameplay_manager;
            if (v35 == WOR_gst_Universe.univAIInstance)
            {
                v34 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v37 = (tGameObjectMain*)v67->des_gameplay_manager;
                if (v35)
                {
                    if ((unsigned int)v35 <= 2)
                        v37 = AI_MainActor[(uint32_t)v35 - 1];
                    v36 = v37;
                }
                else
                {
                    v36 = 0;
                }
                v34 = v36->extended->AIInstance;
            }
            v71 = ((tScriptStruct_GSTHyllisManager*)(v34->instanceVarDataCurrent))->des_ao_allgp[i];
            v33 = (tGameObjectMain*)v71;
            if (v71)
            {
                if (v71 <= 2)
                    v33 = AI_MainActor[v71 - 1];
                v1 = AI_EvalFunc_AIIsModel_C(v33, (tAIModel*)fctvar_Hyllis_Meteor_Manager_HMM_mode_wait_Domz_Sansue);
            }
            else
            {
                v1 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_Hyllis_Meteor_Manager_HMM_mode_wait_Domz_Sansue);
            }
            v49 = v1;
            if (v1)
            {
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 1000;
            }
            else
            {
            LABEL_46:
                v69 = AI_EvalFunc_VIEWGetObject_C(0);
                v58 = AI_EvalFunc_MATHRandFloat_C(0.0, 1.0);
                if (v58 >= 0.60000002)
                {
                    v47 = AI_EvalFunc_VIEWFocaleGet_C(0);
                    X = v47 * 0.5f;
                    v68 = cosf(X);
                    for (i = 0; i < 20; ++i)
                    {
                        v19 = (tGameObjectMain*)v67->des_vaisseau[i];
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
                        if (!v18)
                            break;
                        v17 = (tGameObjectMain*)v67->des_vaisseau[i];
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
                        v78 = (unsigned __int16)v16->flags2;
                        if ((v78 & 4) == 0)
                        {
                            v15 = (tGameObjectMain*)v67->des_vaisseau[i];
                            if (v15)
                            {
                                if ((unsigned int)v15 <= 2)
                                    v15 = AI_MainActor[(uint32_t)v15 - 1];
                                v3 = AI_EvalFunc_OBJ_CapaTest_C(v15, 0x800);
                            }
                            else
                            {
                                v3 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x800);
                            }
                            v63 = v3;
                            if (!v3)
                            {
                                v14 = (tGameObjectMain*)v67->des_vaisseau[i];
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
                                v12 = (tGameObjectMain*)v67->o_main_actor;
                                if (v12)
                                {
                                    if ((unsigned int)v12 <= 2)
                                        v12 = AI_MainActor[(uint32_t)v12 - 1];
                                    v4 = AI_EvalFunc_OBJ_SqrDist_C(v12, v13);
                                }
                                else
                                {
                                    v4 = AI_EvalFunc_OBJ_SqrDist_C(0, v13);
                                }
                                v55 = (float)v4;
                                if (v55 < 4000000.0)
                                {
                                    v11 = (tGameObjectMain*)v67->des_vaisseau[i];
                                    if (v11)
                                    {
                                        if ((unsigned int)v11 <= 2)
                                            v11 = AI_MainActor[(uint32_t)v11 - 1];
                                        AI_EvalFunc_OBJGetPos_C(v11, &v54);
                                    }
                                    else
                                    {
                                        AI_EvalFunc_OBJGetPos_C(0, &v54);
                                    }
                                    v10 = v69;
                                    if (v69)
                                    {
                                        if ((unsigned int)v69 <= 2)
                                            v10 = AI_MainActor[(uint32_t)v69 - 1];
                                        AI_EvalFunc_OBJGetPos_C(v10, &v65);
                                    }
                                    else
                                    {
                                        AI_EvalFunc_OBJGetPos_C(0, &v65);
                                    }
                                    MATH_SubstractVec(&v48, &v54, &v65);
                                    v = v48;
                                    MATH_NormalizeEqualVector(&v);
                                    v9 = v69;
                                    if (v69)
                                    {
                                        if ((unsigned int)v69 <= 2)
                                            v9 = AI_MainActor[(uint32_t)v69 - 1];
                                        AI_EvalFunc_OBJ_SightGet_C(v9, &v75);
                                    }
                                    else
                                    {
                                        AI_EvalFunc_OBJ_SightGet_C(0, &v75);
                                    }
                                    v70 = v75.z * v.z + v75.y * v.y + v.x * v75.x;
                                    if (v70 > (double)v68)
                                    {
                                        v67->o_aircraft = v67->des_vaisseau[i];
                                        v72 = AI_gpst_CurrentGameObject;
                                        v8 = AI_gpst_CurrentGameObject;
                                        if (AI_gpst_CurrentGameObject)
                                        {
                                            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                                                v8 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                                            AI_EvalFunc_AICBDel_C(
                                                AI_gpst_CurrentGameObject,
                                                v8,
                                                2,
                                                (tAIFunction*)fct_Hyllis_Meteor_Manager_HMM_mode_wait_Hyllis_Meteor_Manager_HMM_mode_wait);
                                        }
                                        else
                                        {
                                            AI_EvalFunc_AICBDel_C(
                                                AI_gpst_CurrentGameObject,
                                                0,
                                                2,
                                                (tAIFunction*)fct_Hyllis_Meteor_Manager_HMM_mode_wait_Hyllis_Meteor_Manager_HMM_mode_wait);
                                        }
                                        v7 = (tGameObjectMain*)v67->o_aircraft;
                                        if (v7)
                                        {
                                            if ((unsigned int)v7 <= 2)
                                                v7 = AI_MainActor[(uint32_t)v7 - 1];
                                            AI_EvalFunc_AICBAdd_C(
                                                AI_gpst_CurrentGameObject,
                                                v7,
                                                2,
                                                (tAIFunction*)fct_Hyllis_Meteor_Manager_HMM_mode_wait_Hyllis_Meteor_Manager_HMM_mode_vaisseau);
                                        }
                                        else
                                        {
                                            AI_EvalFunc_AICBAdd_C(
                                                AI_gpst_CurrentGameObject,
                                                0,
                                                2,
                                                (tAIFunction*)fct_Hyllis_Meteor_Manager_HMM_mode_wait_Hyllis_Meteor_Manager_HMM_mode_vaisseau);
                                        }
                                        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                                        return 4;
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    for (i = 0; i < 20; ++i)
                    {
                        v32 = (tGameObjectMain*)v67->des_aircraft[i];
                        if (v32)
                        {
                            if ((unsigned int)v32 <= 2)
                                v32 = AI_MainActor[(uint32_t)v32 - 1];
                            v31 = v32;
                        }
                        else
                        {
                            v31 = 0;
                        }
                        if (!v31)
                            break;
                        v30 = (tGameObjectMain*)v67->des_aircraft[i];
                        if (v30)
                        {
                            if ((unsigned int)v30 <= 2)
                                v30 = AI_MainActor[(uint32_t)v30 - 1];
                            v29 = v30;
                        }
                        else
                        {
                            v29 = 0;
                        }
                        v76 = (unsigned __int16)v29->flags2;
                        if ((v76 & 4) == 0)
                        {
                            v28 = (tGameObjectMain*)v67->des_aircraft[i];
                            if (v28)
                            {
                                if ((unsigned int)v28 <= 2)
                                    v28 = AI_MainActor[(uint32_t)v28 - 1];
                                v2 = AI_EvalFunc_OBJ_CapaTest_C(v28, 0x800);
                            }
                            else
                            {
                                v2 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x800);
                            }
                            v51 = v2;
                            if (!v2)
                            {
                                v27 = (tGameObjectMain*)v67->des_aircraft[i];
                                if (v27)
                                {
                                    if ((unsigned int)v27 <= 2)
                                        v27 = AI_MainActor[(uint32_t)v27 - 1];
                                    AI_EvalFunc_OBJGetPos_C(v27, &a3);
                                }
                                else
                                {
                                    AI_EvalFunc_OBJGetPos_C(0, &a3);
                                }
                                v26 = (tGameObjectMain*)v67->o_main_actor;
                                if (v26)
                                {
                                    if ((unsigned int)v26 <= 2)
                                        v26 = AI_MainActor[(uint32_t)v26 - 1];
                                    AI_EvalFunc_OBJGetPos_C(v26, &v74);
                                }
                                else
                                {
                                    AI_EvalFunc_OBJGetPos_C(0, &v74);
                                }
                                MATH_SubstractVec(&dest, &a3, &v74);
                                a1.x = dest.x;
                                a1.y = dest.y;
                                a1.z = 0.0;
                                v62 = MATH_VecLen(&a1);
                                v25 = 1.0f / v62;
                                a1.x = a1.x * v25;
                                a1.y = a1.y * v25;
                                a1.z = a1.z * v25;
                                v46 = v62 / 120.0f;
                                if (v46 > 6.0 && v46 < 12.0)
                                {
                                    v24 = (tGameObjectMain*)v67->des_aircraft[i];
                                    if (v24)
                                    {
                                        if ((unsigned int)v24 <= 2)
                                            v24 = AI_MainActor[(uint32_t)v24 - 1];
                                        AI_EvalFunc_OBJGetPos_C(v24, &v50);
                                    }
                                    else
                                    {
                                        AI_EvalFunc_OBJGetPos_C(0, &v50);
                                    }
                                    v23 = v69;
                                    if (v69)
                                    {
                                        if ((unsigned int)v69 <= 2)
                                            v23 = AI_MainActor[(uint32_t)v69 - 1];
                                        AI_EvalFunc_OBJGetPos_C(v23, &v77);
                                    }
                                    else
                                    {
                                        AI_EvalFunc_OBJGetPos_C(0, &v77);
                                    }
                                    MATH_SubstractVec(&v60, &v50, &v77);
                                    v = v60;
                                    v22 = v69;
                                    if (v69)
                                    {
                                        if ((unsigned int)v69 <= 2)
                                            v22 = AI_MainActor[(uint32_t)v69 - 1];
                                        AI_EvalFunc_OBJ_SightGet_C(v22, &v52);
                                    }
                                    else
                                    {
                                        AI_EvalFunc_OBJ_SightGet_C(0, &v52);
                                    }
                                    v56 = v52.z * v.z + v52.y * v.y + v.x * v52.x;
                                    if (v56 > 0.0)
                                    {
                                        v67->i_nb_meteor_aircraft = AI_EvalFunc_MATHRandInt_C(2, 6);
                                        v67->o_aircraft = v67->des_aircraft[i];
                                        v57 = AI_gpst_CurrentGameObject;
                                        v21 = AI_gpst_CurrentGameObject;
                                        if (AI_gpst_CurrentGameObject)
                                        {
                                            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                                                v21 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                                            AI_EvalFunc_AICBDel_C(
                                                AI_gpst_CurrentGameObject,
                                                v21,
                                                2,
                                                (tAIFunction*)fct_Hyllis_Meteor_Manager_HMM_mode_wait_Hyllis_Meteor_Manager_HMM_mode_wait);
                                        }
                                        else
                                        {
                                            AI_EvalFunc_AICBDel_C(
                                                AI_gpst_CurrentGameObject,
                                                0,
                                                2,
                                                (tAIFunction*)fct_Hyllis_Meteor_Manager_HMM_mode_wait_Hyllis_Meteor_Manager_HMM_mode_wait);
                                        }
                                        v20 = (tGameObjectMain*)v67->o_aircraft;
                                        if (v20)
                                        {
                                            if ((unsigned int)v20 <= 2)
                                                v20 = AI_MainActor[(uint32_t)v20 - 1];
                                            AI_EvalFunc_AICBAdd_C(
                                                AI_gpst_CurrentGameObject,
                                                v20,
                                                2,
                                                (tAIFunction*)fct_Hyllis_Meteor_Manager_HMM_mode_wait_Hyllis_Meteor_Manager_HMM_mode_aircraft);
                                        }
                                        else
                                        {
                                            AI_EvalFunc_AICBAdd_C(
                                                AI_gpst_CurrentGameObject,
                                                0,
                                                2,
                                                (tAIFunction*)fct_Hyllis_Meteor_Manager_HMM_mode_wait_Hyllis_Meteor_Manager_HMM_mode_aircraft);
                                        }
                                        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                                        return 4;
                                    }
                                }
                            }
                        }
                    }
                }
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

int Hyllis_Meteor_Manager_HMM_mode_vaisseau()
{
    Vec3* v0; // eax
    tGameObjectMain* v1; // eax
    int result; // eax
    Vec3* v3; // eax
    float v4; // [esp+0h] [ebp-2E8h]
    char* v5; // [esp+4h] [ebp-2E4h]
    float v6; // [esp+8h] [ebp-2E0h]
    char* v7; // [esp+14h] [ebp-2D4h]
    tScriptStruct_HyllisMeteorManager* v8; // [esp+18h] [ebp-2D0h]
    tGameObjectMain* v9; // [esp+24h] [ebp-2C4h]
    tGameObjectMain* v10; // [esp+2Ch] [ebp-2BCh]
    Vec3* v11; // [esp+30h] [ebp-2B8h]
    tGameObjectMain* v12; // [esp+38h] [ebp-2B0h]
    tGameObjectMain* v13; // [esp+40h] [ebp-2A8h]
    tGameObjectMain* v14; // [esp+44h] [ebp-2A4h]
    tGameObjectMain* v15; // [esp+48h] [ebp-2A0h]
    tGameObjectMain* v16; // [esp+50h] [ebp-298h]
    tGameObjectMain* v17; // [esp+68h] [ebp-280h]
    tGameObjectMain* v18; // [esp+70h] [ebp-278h]
    tGameObjectMain* v19; // [esp+80h] [ebp-268h]
    tGameObjectMain* v20; // [esp+88h] [ebp-260h]
    tGameObjectMain* v21; // [esp+90h] [ebp-258h]
    tGameObjectMain* v22; // [esp+98h] [ebp-250h]
    tGameObjectMain* v23; // [esp+BCh] [ebp-22Ch]
    tGameObjectMain* v24; // [esp+CCh] [ebp-21Ch]
    tGameObjectMain* v25; // [esp+E0h] [ebp-208h]
    tGameObjectMain* v26; // [esp+F8h] [ebp-1F0h]
    float v27; // [esp+110h] [ebp-1D8h]
    Vec3 pos; // [esp+118h] [ebp-1D0h] BYREF
    float v29; // [esp+124h] [ebp-1C4h]
    Vec3 v30; // [esp+130h] [ebp-1B8h] BYREF
    float v31; // [esp+13Ch] [ebp-1ACh]
    float v32; // [esp+140h] [ebp-1A8h]
    float v33; // [esp+144h] [ebp-1A4h]
    float v34; // [esp+148h] [ebp-1A0h]
    Vec3 a3; // [esp+150h] [ebp-198h] BYREF
    Vec3 v36; // [esp+15Ch] [ebp-18Ch] BYREF
    Vec3 v37; // [esp+174h] [ebp-174h] BYREF
    Vec3 v38; // [esp+180h] [ebp-168h] BYREF
    Vec3 v39; // [esp+190h] [ebp-158h] BYREF
    tGameObjectMain* v40; // [esp+1ACh] [ebp-13Ch]
    Vec3 v41; // [esp+1B0h] [ebp-138h] BYREF
    Vec3 v42; // [esp+1BCh] [ebp-12Ch] BYREF
    float v43; // [esp+1C8h] [ebp-120h]
    Vec3 v44; // [esp+1CCh] [ebp-11Ch] BYREF
    Vec3 a1; // [esp+1D8h] [ebp-110h] BYREF
    float v46; // [esp+1E4h] [ebp-104h]
    float v47; // [esp+1E8h] [ebp-100h]
    float v48; // [esp+1ECh] [ebp-FCh]
    float v49; // [esp+1F0h] [ebp-F8h]
    float v50; // [esp+1F4h] [ebp-F4h]
    float v51; // [esp+1F8h] [ebp-F0h]
    BOOL v52; // [esp+200h] [ebp-E8h]
    float v53; // [esp+214h] [ebp-D4h]
    Vec3 v54; // [esp+21Ch] [ebp-CCh] BYREF
    Vec3 v55; // [esp+228h] [ebp-C0h] BYREF
    int v56; // [esp+234h] [ebp-B4h]
    float v57; // [esp+244h] [ebp-A4h]
    Vec3 v58; // [esp+248h] [ebp-A0h] BYREF
    tScriptStruct_HyllisMeteorManager* v59; // [esp+254h] [ebp-94h]
    int i; // [esp+258h] [ebp-90h]
    float v61; // [esp+25Ch] [ebp-8Ch]
    float v62; // [esp+260h] [ebp-88h]
    float v63; // [esp+264h] [ebp-84h]
    Vec3 v64; // [esp+268h] [ebp-80h] BYREF
    Vec3 v65; // [esp+274h] [ebp-74h] BYREF
    Vec3 v66; // [esp+280h] [ebp-68h] BYREF
    Vec3 v67; // [esp+29Ch] [ebp-4Ch]
    float v68; // [esp+2A8h] [ebp-40h]
    tMessage Src; // [esp+2ACh] [ebp-3Ch] BYREF
    float X; // [esp+2E4h] [ebp-4h]

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    v38.x = 1.0;
    v38.y = 0.0;
    v38.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v8 = (tScriptStruct_HyllisMeteorManager*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v8 = 0;
    v59 = v8;
    v29 = 0.0;
    v56 = 0;
    v68 = 0.0;
    v52 = 0;
    X = 0.0;
    i = 0;
    v40 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Hyllis_Meteor_Manager_HMM_mode_vaisseau_Hyllis_Meteor_Manager_HMM_mode_vaisseau = (int)AI_ul_RealResolveOneRefFunction(0x72004D8Cu);
        Hyllis_Meteor_Manager_HMM_mode_vaisseau_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Hyllis_Meteor_Manager_HMM_mode_vaisseau_univVars_Flag_speciaux_sauve,
            (void*)Hyllis_Meteor_Manager_HMM_mode_vaisseau_univVars,
            4488);
        fct_Hyllis_Meteor_Manager_HMM_mode_vaisseau_Hyllis_Meteor_Manager_HMM_mode_reinit = (int)AI_ul_RealResolveOneRefFunction(0x72004D90u);
        fct_Hyllis_Meteor_Manager_HMM_mode_vaisseau_Hyllis_Meteor_Manager_HMM_init = (int)AI_ul_RealResolveOneRefFunction(0x72004D7Au);
    }
    if (v59->i_current_mode == 2)
    {
        v19 = (tGameObjectMain*)v59->o_aircraft;
        if (v19)
        {
            if ((unsigned int)v19 <= 2)
                v19 = AI_MainActor[(uint32_t)v19 - 1];
            AI_EvalFunc_OBJGetPos_C(v19, &v41);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v41);
        }
        v38 = v41;
        v18 = (tGameObjectMain*)v59->o_aircraft;
        if (v18)
        {
            if ((unsigned int)v18 <= 2)
                v18 = AI_MainActor[(uint32_t)v18 - 1];
            AI_EvalFunc_DYNSpeedGetVector_C(v18, &v58);
        }
        else
        {
            AI_EvalFunc_DYNSpeedGetVector_C(0, &v58);
        }
        v43 = TIM_gf_dt;
        v46 = v58.x * TIM_gf_dt;
        v47 = v58.y * TIM_gf_dt;
        v48 = v58.z * TIM_gf_dt;
        v38.x = v38.x + v46;
        v38.y = v38.y + v47;
        v38.z = v38.z + v48;
        v17 = (tGameObjectMain*)v59->o_aircraft;
        if (v17)
        {
            if ((unsigned int)v17 <= 2)
                v17 = AI_MainActor[(uint32_t)v17 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v17, &v65);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &v65);
        }
        v31 = v65.x + v65.x;
        v32 = v65.y + v65.y;
        v33 = v65.z + v65.z;
        v38.x = v38.x + v31;
        v38.y = v38.y + v32;
        v38.z = v38.z + v33;
        v56 = 0;
        for (i = 0; i < 1; ++i)
        {
            if (*(float*)AI_EvalVarArray_C2((char*)&v59->__f_Z_hors_champ_Dimension1, i) > 15.0)
            {
                v57 = TIM_gf_dt;
                v5 = AI_EvalVarArray_C2((char*)&v59->__f_Z_hors_champ_Dimension1, i);
                *(float*)v5 = *(float*)v5 - 250.0f * v57;
                ++v56;
                v4 = *(float*)AI_EvalVarArray_C2((char*)&v59->__f_Z_hors_champ_Dimension1, i);
                v29 = 1.0f - v4 / *(float*)AI_EvalVarArray_C2((char*)&v59->__f_Z_hors_champ_save_Dimension1, i);
                v29 = v29 * 0.5f;
                v29 = v29 + 0.5f;
                v11 = (Vec3*)AI_EvalVarArray_C3((char*)&v59->__v_offset_Dimension1, i);
                v64.x = v29 * v11->x;
                v64.y = v11->y * v29;
                v64.z = v11->z * v29;
                v44.x = 0.0;
                v44.y = 0.0;
                v44.z = *(float*)AI_EvalVarArray_C2((char*)&v59->__f_Z_hors_champ_Dimension1, i);
                MATH_VecAddVec(&v39, &v64, &v44);
                MATH_VecAddVec(&v55, &v38, &v39);
                v10 = (tGameObjectMain*)v59->o_meteor[i];
                if (v10)
                {
                    if ((unsigned int)v10 <= 2)
                        v10 = AI_MainActor[(uint32_t)v10 - 1];
                    AI_EvalFunc_OBJSetPos_C(v10, &v55);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &v55);
                }
            }
            else
            {
                *(uint32_t*)AI_EvalVarArray_C2((char*)&v59->__f_Z_hors_champ_Dimension1, i) = 0;
                if (!i)
                {
                    v67.x = 0.0;
                    v67.y = 0.0;
                    v67.z = -1.0;
                    a1.x = 0.0;
                    a1.y = 0.0;
                    a1.z = -1.0;
                    Src.msgID = 37;
                    Src.gao = AI_gpst_CurrentGameObject;
                    Src.gao2 = 0;
                    Src.Int1 = 3;
                    Src.Int2 = 0;
                    Src.v1.x = 0.0;
                    Src.v1.y = 0.0;
                    Src.v1.z = -1.0;
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&Src.v2);
                    Src.f2 = 0.0;
                    Src.f1 = 1000.0;
                    v16 = (tGameObjectMain*)v59->o_aircraft;
                    if (v16)
                    {
                        if ((unsigned int)v16 <= 2)
                            v16 = AI_MainActor[(uint32_t)v16 - 1];
                        AI_EvalFunc_MSGSend_C(v16, &Src);
                    }
                    else
                    {
                        AI_EvalFunc_MSGSend_C(0, &Src);
                    }
                }
                v15 = (tGameObjectMain*)v59->o_meteor[i];
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
                    ++v56;
                    v3 = (Vec3*)AI_EvalVarArray_C3((char*)&v59->__v_offset_Dimension1, i);
                    MATH_VecAddVec(&v30, &v38, v3);
                    v13 = (tGameObjectMain*)v59->o_meteor[i];
                    if (v13)
                    {
                        if ((unsigned int)v13 <= 2)
                            v13 = AI_MainActor[(uint32_t)v13 - 1];
                        AI_EvalFunc_OBJSetPos_C(v13, &v30);
                    }
                    else
                    {
                        AI_EvalFunc_OBJSetPos_C(0, &v30);
                    }
                    v12 = (tGameObjectMain*)v59->o_meteor[i];
                    if (v12)
                    {
                        if ((unsigned int)v12 <= 2)
                            v12 = AI_MainActor[(uint32_t)v12 - 1];
                        AI_EvalFunc_OBJ_CapaSet_C(v12, 0, 0x100);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_CapaSet_C(0, 0, 0x100);
                    }
                }
            }
        }
        if (!v56)
        {
            v9 = (tGameObjectMain*)v59->o_aircraft;
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v9,
                    2,
                    (tAIFunction*)fct_Hyllis_Meteor_Manager_HMM_mode_vaisseau_Hyllis_Meteor_Manager_HMM_mode_vaisseau);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_Hyllis_Meteor_Manager_HMM_mode_vaisseau_Hyllis_Meteor_Manager_HMM_mode_vaisseau);
            }
            v52 = AI_EvalFunc_ARRBitTest_C(
                (int*)(Hyllis_Meteor_Manager_HMM_mode_vaisseau_univVars_Flag_speciaux_sauve + 4),
                2789);
            if (v52)
                AI_EvalFunc_AITrackChange_C(
                    0,
                    (tAIFunction*)fct_Hyllis_Meteor_Manager_HMM_mode_vaisseau_Hyllis_Meteor_Manager_HMM_init);
            else
                AI_EvalFunc_AITrackChange_C(
                    0,
                    (tAIFunction*)fct_Hyllis_Meteor_Manager_HMM_mode_vaisseau_Hyllis_Meteor_Manager_HMM_mode_reinit);
        }
        result = 1000;
    }
    else
    {
        v59->i_current_mode = 2;
        v26 = (tGameObjectMain*)v59->o_aircraft;
        if (v26)
        {
            if ((unsigned int)v26 <= 2)
                v26 = AI_MainActor[(uint32_t)v26 - 1];
            AI_EvalFunc_OBJGetPos_C(v26, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        v38 = a3;
        v40 = AI_EvalFunc_VIEWGetObject_C(0);
        v53 = AI_EvalFunc_VIEWFocaleGet_C(0);
        X = v53 * 0.5f;
        v68 = cosf(X);
        *(uint32_t*)AI_EvalVarArray_C2((char*)&v59->__f_Z_hors_champ_Dimension1, 0) = 1128792064;
        a1 = v38;
        v27 = *(float*)AI_EvalVarArray_C2((char*)&v59->__f_Z_hors_champ_Dimension1, 0);
        a1.x = a1.x + 0.0f;
        a1.y = a1.y + 0.0f;
        a1.z = a1.z + v27;
        v25 = v40;
        if (v40)
        {
            if ((unsigned int)v40 <= 2)
                v25 = AI_MainActor[(uint32_t)v40 - 1];
            AI_EvalFunc_OBJGetPos_C(v25, &v42);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v42);
        }
        a1.x = a1.x - v42.x;
        a1.y = a1.y - v42.y;
        a1.z = a1.z - v42.z;
        MATH_NormalizeEqualVector(&a1);
        while (1)
        {
            v24 = v40;
            if (v40)
            {
                if ((unsigned int)v40 <= 2)
                    v24 = AI_MainActor[(uint32_t)v40 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v24, &v66);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v66);
            }
            v34 = a1.z * v66.z + a1.y * v66.y + v66.x * a1.x;
            if (v34 <= (double)v68)
                break;
            v7 = AI_EvalVarArray_C2((char*)&v59->__f_Z_hors_champ_Dimension1, 0);
            *(float*)v7 = AI_EvalFunc_MATHRandFloat_C(100.0, 200.0) + *(float*)v7;
            a1 = v38;
            v61 = 0.0;
            v62 = 0.0;
            v63 = *(float*)AI_EvalVarArray_C2((char*)&v59->__f_Z_hors_champ_Dimension1, 0);
            a1.x = a1.x + v61;
            a1.y = a1.y + v62;
            a1.z = a1.z + v63;
            v23 = v40;
            if (v40)
            {
                if ((unsigned int)v40 <= 2)
                    v23 = AI_MainActor[(uint32_t)v40 - 1];
                AI_EvalFunc_OBJGetPos_C(v23, &v36);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v36);
            }
            a1.x = a1.x - v36.x;
            a1.y = a1.y - v36.y;
            a1.z = a1.z - v36.z;
            MATH_NormalizeEqualVector(&a1);
        }
        v49 = 0.0;
        v50 = 0.0;
        v51 = *(float*)AI_EvalVarArray_C2((char*)&v59->__f_Z_hors_champ_Dimension1, 0);
        v38.x = v38.x + v49;
        v38.y = v38.y + v50;
        v38.z = v38.z + v51;
        for (i = 0; i < 1; ++i)
        {
            v6 = *(float*)AI_EvalVarArray_C2((char*)&v59->__f_Z_hors_champ_Dimension1, i);
            *(float*)AI_EvalVarArray_C2((char*)&v59->__f_Z_hors_champ_save_Dimension1, i) = v6;
            v37.x = 0.0;
            v37.y = 0.0;
            v37.z = *(float*)AI_EvalVarArray_C2((char*)&v59->__f_Z_hors_champ_Dimension1, i);
            v0 = (Vec3*)AI_EvalVarArray_C3((char*)&v59->__v_offset_Dimension1, i);
            MATH_VecAddVec(&v54, v0, &v37);
            MATH_VecAddVec(&pos, &v38, &v54);
            v22 = (tGameObjectMain*)v59->des_meteor;
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
            v59->o_meteor[i] = (int)v1;
            v21 = (tGameObjectMain*)v59->o_meteor[i];
            if (v21)
            {
                if ((unsigned int)v21 <= 2)
                    v21 = AI_MainActor[(uint32_t)v21 - 1];
                AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, v21, &v59->o_meteor[i]);
            }
            else
            {
                AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, 0, &v59->o_meteor[i]);
            }
            v20 = (tGameObjectMain*)v59->o_meteor[i];
            if (v20)
            {
                if ((unsigned int)v20 <= 2)
                    v20 = AI_MainActor[(uint32_t)v20 - 1];
                AI_EvalFunc_OBJ_CapaSet_C(v20, 0x100, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_CapaSet_C(0, 0x100, 0);
            }
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 4;
    }
    return result;
}