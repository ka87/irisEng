#include "ai_compiledscripts_inc.h"

//hyllis_meteor_hm_init
int hyllis_meteor_hm_init_univVars;
int hyllis_meteor_hm_init_univVars_Flag_speciaux_sauve;
int fct_hyllis_meteor_hm_init_hyllis_meteor_hm_loop;

//hyllis_meteor_hm_loop
int hyllis_meteor_hm_loop_univVars;
int hyllis_meteor_hm_loop_univVars_Flag_speciaux_sauve;

int hyllis_meteor_hm_init()
{
    int result; // eax
    tScriptStruct_HyllisMeteor* v1; // [esp+4h] [ebp-38h]
    int v2; // [esp+28h] [ebp-14h]
    int v3; // [esp+30h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_HyllisMeteor*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        hyllis_meteor_hm_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &hyllis_meteor_hm_init_univVars_Flag_speciaux_sauve,
            (void*)hyllis_meteor_hm_init_univVars,
            4488);
        fct_hyllis_meteor_hm_init_hyllis_meteor_hm_loop = (int)AI_ul_RealResolveOneRefFunction(0x3D001AF9u);
    }
    if (*AI_gppst_CurrentJumpNode != (tAINode*)5)
    {
        if (AI_EvalFunc_ARRBitTest_C((int*)(hyllis_meteor_hm_init_univVars_Flag_speciaux_sauve + 4), 2718))
        {
            v3 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v3 != 1000)
                return v3;
        }
        AI_EvalFunc_ARRBitSet_C((int*)(hyllis_meteor_hm_init_univVars_Flag_speciaux_sauve + 4), 2718);
        v1->i_jai_le_saut = AI_EvalFunc_ARRBitTest_C((int*)(hyllis_meteor_hm_init_univVars_Flag_speciaux_sauve + 4), 2789);
    }
    if (v1->i_jai_le_saut)
    {
        v1->i_jai_le_saut = AI_EvalFunc_ARRBitTest_C((int*)(hyllis_meteor_hm_init_univVars_Flag_speciaux_sauve + 4), 2197);
        v1->f_rand_gener = 0.0;
        v1->i_max_meteor = 35;
        v2 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_hyllis_meteor_hm_init_hyllis_meteor_hm_loop);
        if (v2 == 1000)
            result = 1000;
        else
            result = v2;
    }
    else
    {
        v1->i_jai_le_saut = AI_EvalFunc_ARRBitTest_C((int*)(hyllis_meteor_hm_init_univVars_Flag_speciaux_sauve + 4), 2789);
        *AI_gppst_CurrentJumpNode = (tAINode*)5;
        result = 0;
    }
    return result;
}

int hyllis_meteor_hm_loop()
{
    BOOL v0; // eax
    tGameObjectMain* v1; // eax
    BOOL v2; // eax
    tGameObjectMain* v3; // eax
    tGameObjectMain* v4; // eax
    tGameObjectMain* v5; // eax
    BOOL v6; // eax
    int v7; // eax
    int v9; // eax
    float v10; // [esp+Ch] [ebp-2FCh]
    float zoom; // [esp+10h] [ebp-2F8h]
    float v12; // [esp+14h] [ebp-2F4h]
    int v13; // [esp+18h] [ebp-2F0h]
    tScriptStruct_HyllisMeteor* v14; // [esp+24h] [ebp-2E4h]
    tGameObjectMain* v15; // [esp+30h] [ebp-2D8h]
    Vec3* v16; // [esp+3Ch] [ebp-2CCh]
    tGameObjectMain* v17; // [esp+4Ch] [ebp-2BCh]
    tGameObjectMain* v18; // [esp+54h] [ebp-2B4h]
    tGameObjectMain* v19; // [esp+5Ch] [ebp-2ACh]
    tGameObjectMain* v20; // [esp+64h] [ebp-2A4h]
    tGameObjectMain* v21; // [esp+68h] [ebp-2A0h]
    tGameObjectMain* v22; // [esp+6Ch] [ebp-29Ch]
    tGameObjectMain* v23; // [esp+74h] [ebp-294h]
    tGameObjectMain* v24; // [esp+7Ch] [ebp-28Ch]
    tGameObjectMain* v25; // [esp+84h] [ebp-284h]
    tGameObjectMain* v26; // [esp+88h] [ebp-280h]
    tGameObjectMain* v27; // [esp+8Ch] [ebp-27Ch]
    tGameObjectMain* v28; // [esp+94h] [ebp-274h]
    tGameObjectMain* v29; // [esp+9Ch] [ebp-26Ch]
    tAI* v30; // [esp+A0h] [ebp-268h]
    tGameObjectMain* v31; // [esp+A4h] [ebp-264h]
    tGameObjectMain* v32; // [esp+A8h] [ebp-260h]
    tGameObjectMain* v33; // [esp+B0h] [ebp-258h]
    tGameObjectMain* v34; // [esp+B8h] [ebp-250h]
    tGameObjectMain* v35; // [esp+C0h] [ebp-248h]
    tGameObjectMain* v36; // [esp+C8h] [ebp-240h]
    tGameObjectMain* v37; // [esp+D8h] [ebp-230h]
    float* v38; // [esp+E4h] [ebp-224h]
    tGameObjectMain* v39; // [esp+ECh] [ebp-21Ch]
    tGameObjectMain* v40; // [esp+F4h] [ebp-214h]
    int* v41; // [esp+F8h] [ebp-210h]
    Vec3* v42; // [esp+FCh] [ebp-20Ch]
    tGameObjectMain* v43; // [esp+10Ch] [ebp-1FCh]
    tGameObjectMain* v44; // [esp+114h] [ebp-1F4h]
    tGameObjectMain* v45; // [esp+11Ch] [ebp-1ECh]
    tGameObjectMain* v46; // [esp+13Ch] [ebp-1CCh]
    tGameObjectMain* v47; // [esp+144h] [ebp-1C4h]
    tGameObjectMain* v48; // [esp+160h] [ebp-1A8h]
    tGameObjectMain* v49; // [esp+168h] [ebp-1A0h]
    tGameObjectMain* v50; // [esp+170h] [ebp-198h]
    tGameObjectMain* v51; // [esp+178h] [ebp-190h]
    tAI* v52; // [esp+17Ch] [ebp-18Ch]
    tGameObjectMain* v53; // [esp+180h] [ebp-188h]
    tGameObjectMain* v54; // [esp+184h] [ebp-184h]
    Vec3 v55; // [esp+188h] [ebp-180h] BYREF
    float v56; // [esp+194h] [ebp-174h]
    unsigned int v57; // [esp+19Ch] [ebp-16Ch]
    BOOL v58; // [esp+1A4h] [ebp-164h]
    Vec3 v59; // [esp+1A8h] [ebp-160h] BYREF
    tGameObjectMain* v60; // [esp+1B8h] [ebp-150h]
    int v61; // [esp+1C8h] [ebp-140h]
    Vec3 v62; // [esp+1CCh] [ebp-13Ch] BYREF
    float v63; // [esp+1D8h] [ebp-130h]
    Vec3 a1; // [esp+1E0h] [ebp-128h] BYREF
    Vec3 v65; // [esp+1ECh] [ebp-11Ch]
    tAI* v66; // [esp+1FCh] [ebp-10Ch]
    Vec3 v67; // [esp+200h] [ebp-108h] BYREF
    Vec3 v68; // [esp+20Ch] [ebp-FCh] BYREF
    int v69; // [esp+218h] [ebp-F0h]
    int v70; // [esp+21Ch] [ebp-ECh]
    Vec3 v71; // [esp+220h] [ebp-E8h]
    Vec3 v72; // [esp+22Ch] [ebp-DCh] BYREF
    BOOL v73; // [esp+238h] [ebp-D0h]
    Vec3 v74; // [esp+240h] [ebp-C8h] BYREF
    Vec3 v75; // [esp+24Ch] [ebp-BCh] BYREF
    Vec3 a4; // [esp+258h] [ebp-B0h] BYREF
    float v77; // [esp+268h] [ebp-A0h]
    Vec3 a3; // [esp+26Ch] [ebp-9Ch] BYREF
    Vec3 v79; // [esp+278h] [ebp-90h] BYREF
    tGameObjectMain* v80; // [esp+284h] [ebp-84h]
    Vec3 v82; // [esp+28Ch] [ebp-7Ch] BYREF
    int v83; // [esp+298h] [ebp-70h]
    float v84; // [esp+2A0h] [ebp-68h]
    BOOL v85; // [esp+2A4h] [ebp-64h]
    tScriptStruct_HyllisMeteor* v86; // [esp+2B0h] [ebp-58h]
    float v87; // [esp+2B4h] [ebp-54h]
    float v88; // [esp+2B8h] [ebp-50h]
    float v89; // [esp+2BCh] [ebp-4Ch]
    Vec3 v90; // [esp+2C0h] [ebp-48h] BYREF
    Vec3 v91; // [esp+2CCh] [ebp-3Ch] BYREF
    Vec3 pos; // [esp+2D8h] [ebp-30h] BYREF
    int v93; // [esp+2E8h] [ebp-20h]
    Vec3 v94; // [esp+2ECh] [ebp-1Ch] BYREF
    float v95; // [esp+2F8h] [ebp-10h]
    int i; // [esp+2FCh] [ebp-Ch]
    int v97; // [esp+300h] [ebp-8h]

    v82.x = 1.0;
    v82.y = 0.0;
    v82.z = 0.0;
    v59.x = 1.0;
    v59.y = 0.0;
    v59.z = 0.0;
    v72.x = 1.0;
    v72.y = 0.0;
    v72.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v14 = (tScriptStruct_HyllisMeteor*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v14 = 0;
    v86 = v14;
    v57 = 0;
    i = 0;
    v87 = 0.0;
    v60 = 0;
    v97 = 0;
    v80 = 0;
    v66 = 0;
    v89 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        hyllis_meteor_hm_loop_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &hyllis_meteor_hm_loop_univVars_Flag_speciaux_sauve,
            (void*)hyllis_meteor_hm_loop_univVars,
            4488);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)13)
    {
        AI_EvalFunc_ARRBitClear_C((int*)(hyllis_meteor_hm_loop_univVars_Flag_speciaux_sauve + 4), 2718);
        v9 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        v70 = v9;
        if (v9 != 1000)
            return v70;
        return 1000;
    }
    v60 = *AI_MainActor;
    v80 = AI_EvalFunc_VIEWGetObject_C(0);
    if (v60 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
    {
        v52 = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        v54 = v60;
        if (v60)
        {
            if ((unsigned int)v60 <= 2)
                v54 = AI_MainActor[(uint32_t)v60 - 1];
            v53 = v54;
        }
        else
        {
            v53 = 0;
        }
        v52 = v53->extended->AIInstance;
    }
    v89 = ((tScriptStruct_PJSally*)(v52->instanceVarDataCurrent))->v_jump_zdm_reactivation_pos.x;
    if (AI_EvalFunc_TIMEElapsed_C(v86->f_time_gener, v86->f_rand_gener))
    {
        v51 = v60;
        if (v60)
        {
            if ((unsigned int)v60 <= 2)
                v51 = AI_MainActor[(uint32_t)v60 - 1];
            v0 = AI_EvalFunc_OBJ_CapaTest_C(v51, 0x2000);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x2000);
        }
        v58 = v0;
        if (v0)
        {
            v86->f_rand_gener = (float)(int)AI_EvalFunc_MATHRandInt_C(1, 3);
            if (v86->i_max_meteor)
            {
                --v86->i_max_meteor;
                v86->f_time_gener = TIM_gf_MainClock - TIM_gf_LockTime;
                if (v86->i_nummeteor != 20)
                {
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
                    v50 = (tGameObjectMain*)v86->des_o_meteor;
                    if (v86->des_o_meteor)
                    {
                        if ((unsigned int)v50 <= 2)
                            v50 = AI_MainActor[(uint32_t)v50 - 1];
                        v1 = AI_EvalFunc_OBJDuplicate_C(v50, &pos);
                    }
                    else
                    {
                        v1 = AI_EvalFunc_OBJDuplicate_C(0, &pos);
                    }
                    v57 = (unsigned int)v1;
                    v49 = v1;
                    if (v1)
                    {
                        if ((unsigned int)v1 <= 2)
                            v49 = AI_MainActor[(uint32_t)v1 - 1];
                        AI_EvalFunc_OBJ_CapaSet_C(v49, 0x100, 0);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_CapaSet_C(0, 0x100, 0);
                    }
                    v48 = v60;
                    if (v60)
                    {
                        if ((unsigned int)v60 <= 2)
                            v48 = AI_MainActor[(uint32_t)v60 - 1];
                        AI_EvalFunc_DYNSpeedGetVector_C(v48, &v79);
                    }
                    else
                    {
                        AI_EvalFunc_DYNSpeedGetVector_C(0, &v79);
                    }
                    a1.x = v79.x;
                    a1.y = v79.y;
                    a1.z = 0.0;
                    v90.x = 0.0;
                    v90.y = 0.0;
                    v90.z = 1.0;
                    v63 = AI_EvalFunc_MATHRandFloat_C(-0.80000001f, 0.80000001f);
                    AI_EvalFunc_MATHVecRotate_C(&a1, &v90, v63, &a4);
                    a1.x = a4.x * 2.3f;
                    a1.y = a4.y * 2.3f;
                    a1.z = a4.z * 2.3f;
                    v87 = MATH_VecLen(&a1);
                    if (v87 != 0.0)
                    {
                        if (v87 < 100.0)
                            goto LABEL_48;
                        v47 = v80;
                        if (v80)
                        {
                            if ((unsigned int)v80 <= 2)
                                v47 = AI_MainActor[(uint32_t)v80 - 1];
                            AI_EvalFunc_OBJ_SightGet_C(v47, &v94);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_SightGet_C(0, &v94);
                        }
                        v77 = v94.z * a1.z + v94.y * a1.y + a1.x * v94.x;
                        if (v77 >= 0.0)
                        {
                            if (v86->i_jai_le_saut)
                            {
                                MATH_VecMultNormScalar(&v67, &a1, v87);
                                v82 = v67;
                                v55.x = 0.0;
                                v55.y = 0.0;
                                v55.z = 1.0;
                                AI_EvalFunc_MATHVecCrossProduct_C(&v82, &v55, &v68);
                                v82 = v68;
                                v84 = AI_EvalFunc_MATHRandFloat_C(0.0, 50.0);
                                v82.x = v82.x * v84;
                                v82.y = v82.y * v84;
                                v82.z = v82.z * v84;
                                v45 = v80;
                                if (v80)
                                {
                                    if ((unsigned int)v80 <= 2)
                                        v45 = AI_MainActor[(uint32_t)v80 - 1];
                                    AI_EvalFunc_OBJ_SightGet_C(v45, &v75);
                                }
                                else
                                {
                                    AI_EvalFunc_OBJ_SightGet_C(0, &v75);
                                }
                                v95 = v82.z * v75.z + v82.y * v75.y + v75.x * v82.x;
                                if (v95 < 0.0)
                                {
                                    v82.x = v82.x * -1.0f;
                                    v82.y = v82.y * -1.0f;
                                    v82.z = v82.z * -1.0f;
                                }
                                a1.x = a1.x + v82.x;
                                a1.y = a1.y + v82.y;
                                a1.z = a1.z + v82.z;
                            }
                        }
                        else
                        {
                        LABEL_48:
                            v46 = v80;
                            if (v80)
                            {
                                if ((unsigned int)v80 <= 2)
                                    v46 = AI_MainActor[(uint32_t)v80 - 1];
                                AI_EvalFunc_OBJ_SightGet_C(v46, &v62);
                            }
                            else
                            {
                                AI_EvalFunc_OBJ_SightGet_C(0, &v62);
                            }
                            a1.x = v62.x * 150.0f;
                            a1.y = v62.y * 150.0f;
                            a1.z = v62.z * 150.0f;
                        }
                    }
                    v44 = v60;
                    if (v60)
                    {
                        if ((unsigned int)v60 <= 2)
                            v44 = AI_MainActor[(uint32_t)v60 - 1];
                        AI_EvalFunc_OBJGetPos_C(v44, &a3);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &a3);
                    }
                    a1.x = a1.x + a3.x;
                    a1.y = a1.y + a3.y;
                    a1.z = a1.z + a3.z;
                    v65.x = 0.0;
                    v65.y = 0.0;
                    v65.z = 200.0;
                    a1.x = a1.x + 0.0f;
                    a1.y = a1.y + 0.0f;
                    a1.z = a1.z + 200.0f;
                    v43 = (tGameObjectMain*)v57;
                    if (v57)
                    {
                        if (v57 <= 2)
                            v43 = AI_MainActor[v57 - 1];
                        AI_EvalFunc_OBJSetPos_C(v43, &a1);
                    }
                    else
                    {
                        AI_EvalFunc_OBJSetPos_C(0, &a1);
                    }
                    a1.x = AI_EvalFunc_MATHRandFloat_C(-50.0, 50.0);
                    a1.y = AI_EvalFunc_MATHRandFloat_C(-50.0, 50.0);
                    a1.z = -150.0;
                    v42 = (Vec3*)AI_EvalVarArray_C3((char*)&v86->__v_speed_Dimension1, v86->i_nummeteor);
                    *v42 = a1;
                    v86->o_lst_meteor[v86->i_nummeteor] = v57;
                    v41 = &v86->i_snd_loop[v86->i_nummeteor];
                    *v41 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18001000);
                    v13 = SND_l_GetFreq(v86->i_snd_loop[v86->i_nummeteor]);
                    v86->f_freq_loop = (float)v13;
                    v40 = (tGameObjectMain*)v57;
                    if (v57)
                    {
                        if (v57 <= 2)
                            v40 = AI_MainActor[v57 - 1];
                        AI_EvalFunc_SNDUltraPlaySoundLooped_C(v40, v86->i_snd_loop[v86->i_nummeteor]);
                    }
                    else
                    {
                        AI_EvalFunc_SNDUltraPlaySoundLooped_C(0, v86->i_snd_loop[v86->i_nummeteor]);
                    }
                    ++v86->i_nummeteor;
                }
            }
        }
    }
    for (i = 0; i < v86->i_nummeteor; ++i)
    {
        v57 = v86->o_lst_meteor[i];
        v39 = (tGameObjectMain*)v57;
        if (v57)
        {
            if (v57 <= 2)
                v39 = AI_MainActor[v57 - 1];
            v2 = AI_EvalFunc_OBJ_CapaTest_C(v39, 0x800);
        }
        else
        {
            v2 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x800);
        }
        v73 = v2;
        if (v2)
        {
            v18 = (tGameObjectMain*)v57;
            if (v57)
            {
                if (v57 <= 2)
                    v18 = AI_MainActor[v57 - 1];
                v6 = AI_EvalFunc_OBJ_CapaTest_C(v18, 0x1000);
            }
            else
            {
                v6 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x1000);
            }
            v85 = v6;
            if (v6)
            {
                if (v86->i_snd_loop[i] != -1)
                {
                    SND_Release(v86->i_snd_loop[i]);
                    v86->i_snd_loop[i] = -1;
                }
                v17 = (tGameObjectMain*)v57;
                if (v57)
                {
                    if (v57 <= 2)
                        v17 = AI_MainActor[v57 - 1];
                    v7 = AI_EvalFunc_OBJDestroy_C(v17);
                }
                else
                {
                    v7 = AI_EvalFunc_OBJDestroy_C(0);
                }
                v61 = v7;
                if (v7 != 1000)
                    return v61;
                v86->o_lst_meteor[i] = v86->o_lst_meteor[v86->i_nummeteor - 1]; // FIXME ?
                v16 = (Vec3*)AI_EvalVarArray_C3((char*)&v86->__v_speed_Dimension1, v86->i_nummeteor - 1);
                *(Vec3*)AI_EvalVarArray_C3((char*)&v86->__v_speed_Dimension1, i) = *v16;
                v86->i_snd_loop[i--] = v86->i_snd_loop[--v86->i_nummeteor]; // FIXME?
            }
        }
        else
        {
            v88 = TIM_gf_dt;
            v38 = (float*)AI_EvalVarArray_C3((char*)&v86->__v_speed_Dimension1, i);
            v71.x = v88 * *v38;
            v71.y = v38[1] * v88;
            v71.z = v38[2] * v88;
            v72 = v71;
            v37 = (tGameObjectMain*)v57;
            if (v57)
            {
                if (v57 <= 2)
                    v37 = AI_MainActor[v57 - 1];
                AI_EvalFunc_OBJGetPos_C(v37, &v74);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v74);
            }
            MATH_VecAddVec(&v91, &v74, &v72);
            v59 = v91;
            v36 = (tGameObjectMain*)v57;
            if (v57)
            {
                if (v57 <= 2)
                    v36 = AI_MainActor[v57 - 1];
                AI_EvalFunc_OBJSetPos_C(v36, &v59);
            }
            else
            {
                AI_EvalFunc_OBJSetPos_C(0, &v59);
            }
            if (v59.z < 5.0)
            {
                v35 = (tGameObjectMain*)v57;
                if (v57)
                {
                    if (v57 <= 2)
                        v35 = AI_MainActor[v57 - 1];
                    AI_EvalFunc_OBJ_CapaSet_C(v35, 0, 0x100);
                }
                else
                {
                    AI_EvalFunc_OBJ_CapaSet_C(0, 0, 0x100);
                }
                v34 = (tGameObjectMain*)v57;
                if (v57)
                {
                    if (v57 <= 2)
                        v34 = AI_MainActor[v57 - 1];
                    AI_EvalFunc_OBJ_CapaSet_C(v34, 0x800, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_CapaSet_C(0, 0x800, 0);
                }
                v59.z = 0.0;
                v33 = (tGameObjectMain*)v86->des_o_paf;
                if (v33)
                {
                    if ((unsigned int)v33 <= 2)
                        v33 = AI_MainActor[(uint32_t)v33 - 1];
                    v3 = AI_EvalFunc_OBJDuplicate_C(v33, &v59);
                }
                else
                {
                    v3 = AI_EvalFunc_OBJDuplicate_C(0, &v59);
                }
                v66 = (tAI*)v3;
                v97 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 1, 0x18000080);
                v83 = SND_l_GetFreq(v97);
                v56 = AI_EvalFunc_MATHRandFloat_C(0.94999999f, 1.05f);
                v12 = (float)v83;
                SND_SetFreq(v97, (unsigned int)(v12 * v56));
                AI_EvalFunc_SNDUltraPlaySound_C(AI_gpst_CurrentGameObject, v97);
                SND_Release(v86->i_snd_loop[i]);
                v86->i_snd_loop[i] = -1;
                if (v66 == WOR_gst_Universe.univAIInstance)
                {
                    v30 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v32 = (tGameObjectMain*)v66;
                    if (v66)
                    {
                        if ((unsigned int)v66 <= 2)
                            v32 = AI_MainActor[(uint32_t)v66 - 1];
                        v31 = v32;
                    }
                    else
                    {
                        v31 = 0;
                    }
                    v30 = v31->extended->AIInstance;
                }
                ((tScriptStruct_SFXOndeDeChoc*)(v30->instanceVarDataCurrent))->o_sound_bank = (int)AI_gpst_CurrentGameObject;
                v29 = (tGameObjectMain*)v86->des_o_plouf;
                if (v29)
                {
                    if ((unsigned int)v29 <= 2)
                        v29 = AI_MainActor[(uint32_t)v29 - 1];
                    v4 = AI_EvalFunc_OBJDuplicate_C(v29, &v59);
                }
                else
                {
                    v4 = AI_EvalFunc_OBJDuplicate_C(0, &v59);
                }
                v66 = (tAI*)v4;
                v28 = v4;
                if (v4)
                {
                    if ((unsigned int)v4 <= 2)
                        v28 = AI_MainActor[(uint32_t)v4 - 1];
                    AI_EvalFunc_OBJ_SetChannelProg_C(v28, 0, 1.0);
                }
                else
                {
                    AI_EvalFunc_OBJ_SetChannelProg_C(0, 0, 1.0);
                }
                v93 = AI_EvalFunc_MATHRandInt_C(2, 4);
                v27 = (tGameObjectMain*)v66;
                if (v66)
                {
                    if ((unsigned int)v66 <= 2)
                        v27 = AI_MainActor[(uint32_t)v66 - 1];
                    v26 = v27;
                }
                else
                {
                    v26 = 0;
                }
                zoom = (float)v93;
                AI_EvalFunc_OBJ_ZoomSet_C(v26, zoom);
                v59.z = -1.0;
                v25 = (tGameObjectMain*)v66;
                if (v66)
                {
                    if ((unsigned int)v66 <= 2)
                        v25 = AI_MainActor[(uint32_t)v66 - 1];
                    AI_EvalFunc_OBJSetPos_C(v25, &v59);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &v59);
                }
                v24 = (tGameObjectMain*)v86->des_o_plouf;
                if (v24)
                {
                    if ((unsigned int)v24 <= 2)
                        v24 = AI_MainActor[(uint32_t)v24 - 1];
                    v5 = AI_EvalFunc_OBJDuplicate_C(v24, &v59);
                }
                else
                {
                    v5 = AI_EvalFunc_OBJDuplicate_C(0, &v59);
                }
                v66 = (tAI*)v5;
                v23 = v5;
                if (v5)
                {
                    if ((unsigned int)v5 <= 2)
                        v23 = AI_MainActor[(uint32_t)v5 - 1];
                    AI_EvalFunc_OBJ_SetChannelProg_C(v23, 0, 1.0);
                }
                else
                {
                    AI_EvalFunc_OBJ_SetChannelProg_C(0, 0, 1.0);
                }
                v69 = AI_EvalFunc_MATHRandInt_C(1, 3);
                v22 = (tGameObjectMain*)v66;
                if (v66)
                {
                    if ((unsigned int)v66 <= 2)
                        v22 = AI_MainActor[(uint32_t)v66 - 1];
                    v21 = v22;
                }
                else
                {
                    v21 = 0;
                }
                v10 = (float)v69;
                AI_EvalFunc_OBJ_ZoomSet_C(v21, v10);
                v59.z = -1.0;
                v20 = (tGameObjectMain*)v66;
                if (v66)
                {
                    if ((unsigned int)v66 <= 2)
                        v20 = AI_MainActor[(uint32_t)v66 - 1];
                    AI_EvalFunc_OBJSetPos_C(v20, &v59);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &v59);
                }
                v59.z = 5.0;
                v19 = (tGameObjectMain*)v57;
                if (v57)
                {
                    if (v57 <= 2)
                        v19 = AI_MainActor[v57 - 1];
                    AI_EvalFunc_OBJSetPos_C(v19, &v59);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &v59);
                }
            }
        }
    }
    if (v86->i_max_meteor || v86->i_nummeteor)
        return 1000;
    AI_EvalFunc_ARRBitClear_C((int*)(hyllis_meteor_hm_loop_univVars_Flag_speciaux_sauve + 4), 2789);
    v15 = (tGameObjectMain*)v86->des_o_env;
    if (v15)
    {
        if ((unsigned int)v15 <= 2)
            v15 = AI_MainActor[(uint32_t)v15 - 1];
        AI_EvalFunc_OBJ_CapaSet_C(v15, 16, 0);
    }
    else
    {
        AI_EvalFunc_OBJ_CapaSet_C(0, 16, 0);
    }
    *AI_gppst_CurrentJumpNode = (tAINode*)13;
    TIME_Wait_C(10.0);
    return 0;
}