#include "ai_compiledscripts_inc.h"

//SFX_Phare_SFX_Phare_init
int SFX_Phare_SFX_Phare_init_univVars;
int SFX_Phare_SFX_Phare_init_univVars_Flag_speciaux_sauve;
int fct_SFX_Phare_SFX_Phare_init_SFX_Phare_SFX_Phare_comportement;

//SFX_Phare_SFX_Phare_comportement
int SFX_Phare_SFX_Phare_comportement_univVars;
int SFX_Phare_SFX_Phare_comportement_univVars_Flag_speciaux_sauve;
int SFX_Phare_SFX_Phare_comportement_univVars_Planetes_CurHour;
int SFX_Phare_SFX_Phare_comportement_SFX_light_smoke_gao;
int SFX_Phare_SFX_Phare_comportement__PJ_Sally_gao;

int SFX_Phare_SFX_Phare_init()
{
    int result; // eax
    char* v1; // [esp+0h] [ebp-14Ch]
    float v2; // [esp+4h] [ebp-148h]
    tScriptStruct_SFXPhare* v3; // [esp+8h] [ebp-144h]
    tGameObjectMain* v4; // [esp+A8h] [ebp-A4h]
    Vec3 v5; // [esp+C4h] [ebp-88h] BYREF
    Vec3 v6; // [esp+D0h] [ebp-7Ch] BYREF
    Vec3 v7; // [esp+DCh] [ebp-70h] BYREF
    Vec3 v8; // [esp+E8h] [ebp-64h] BYREF
    Vec3 v9; // [esp+FCh] [ebp-50h] BYREF
    Vec3 a1; // [esp+108h] [ebp-44h] BYREF
    int v11; // [esp+114h] [ebp-38h]
    Vec3 dest; // [esp+118h] [ebp-34h] BYREF
    int v13; // [esp+128h] [ebp-24h]
    tScriptStruct_SFXPhare* v14; // [esp+12Ch] [ebp-20h]
    int i; // [esp+130h] [ebp-1Ch]
    Vec3 v16; // [esp+134h] [ebp-18h] BYREF
    Vec3 a3; // [esp+140h] [ebp-Ch] BYREF

    v8.x = 1.0;
    v8.y = 0.0;
    v8.z = 0.0;
    v9.x = 1.0;
    v9.y = 0.0;
    v9.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_SFXPhare*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v14 = v3;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SFX_Phare_SFX_Phare_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &SFX_Phare_SFX_Phare_init_univVars_Flag_speciaux_sauve,
            (void*)SFX_Phare_SFX_Phare_init_univVars,
            4488);
        fct_SFX_Phare_SFX_Phare_init_SFX_Phare_SFX_Phare_comportement = (int)AI_ul_RealResolveOneRefFunction(0x72004209u);
    }
    if (v14->des_SF_desactivation <= 0)
        i = 0;
    else
        i = AI_EvalFunc_ARRBitTest_C(
            (int*)(SFX_Phare_SFX_Phare_init_univVars_Flag_speciaux_sauve + 4),
            v14->des_SF_desactivation);
    if (i)
    {
        v13 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v13 != 1000)
            return v13;
    }
    v14->i_nb_wp = AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)v14->des_o_wp, 5, 0);
    if (v14->i_nb_wp == -1)
        v14->i_nb_wp = 5;
    v6.x = -v14->des_f_light_length;
    v6.y = -v14->des_f_light_length;
    v6.z = -5.0;
    AI_EvalFunc_BVMinSet_C(AI_gpst_CurrentGameObject, &v6);
    v5.x = v14->des_f_light_length;
    v5.y = v14->des_f_light_length;
    v5.z = 5.0;
    AI_EvalFunc_BVMaxSet_C(AI_gpst_CurrentGameObject, &v5);
    OBJ_RestoreInitialPos(AI_gpst_CurrentGameObject);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a1);
    v9 = a1;
    AI_EvalFunc_MATHVecSetHorzNormalize_C(&v9);
    for (i = 0; i < v14->i_nb_wp; ++i)
    {
        v4 = (tGameObjectMain*)v14->des_o_wp[i];
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            AI_EvalFunc_OBJGetPos_C(v4, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v7);
        MATH_SubstractVec(&dest, &a3, &v7);
        v8 = dest;
        AI_EvalFunc_MATHVecSetHorzNormalize_C(&v8);
        v16.x = 0.0;
        v16.y = 0.0;
        v16.z = 1.0;
        v2 = MATH_f_VecAngle(&v9, &v8, &v16);
        *(float*)AI_EvalVarArray_C2((char*)&v14->__af_angle_Dimension1, i) = v2;
        if (*(float*)AI_EvalVarArray_C2((char*)&v14->__af_angle_Dimension1, i) < 0.0)
        {
            v1 = AI_EvalVarArray_C2((char*)&v14->__af_angle_Dimension1, i);
            *(float*)v1 = *(float*)v1 + 6.283185f;
        }
    }
    v11 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_Phare_SFX_Phare_init_SFX_Phare_SFX_Phare_comportement);
    if (v11 == 1000)
        result = 1000;
    else
        result = v11;
    return result;
}

int SFX_Phare_SFX_Phare_comportement()
{
    int result; // eax
    double v1; // st7
    int v2; // eax
    float v3; // [esp+4h] [ebp-330h]
    float v4; // [esp+8h] [ebp-32Ch]
    float v5; // [esp+Ch] [ebp-328h]
    float v6; // [esp+10h] [ebp-324h]
    float v7; // [esp+14h] [ebp-320h]
    tScriptStruct_SFXPhare* v8; // [esp+18h] [ebp-31Ch]
    tGameObjectMain* v9; // [esp+1Ch] [ebp-318h]
    tGameObjectMain* v10; // [esp+20h] [ebp-314h]
    float v11; // [esp+24h] [ebp-310h]
    float v12; // [esp+28h] [ebp-30Ch]
    float v13; // [esp+2Ch] [ebp-308h]
    float v14; // [esp+30h] [ebp-304h]
    float v15; // [esp+34h] [ebp-300h]
    tGameObjectMain* v16; // [esp+38h] [ebp-2FCh]
    tGameObjectMain* v17; // [esp+3Ch] [ebp-2F8h]
    tGameObjectMain* v18; // [esp+40h] [ebp-2F4h]
    tGameObjectMain* v19; // [esp+44h] [ebp-2F0h]
    tGameObjectMain* v20; // [esp+48h] [ebp-2ECh]
    tGameObjectMain* v21; // [esp+4Ch] [ebp-2E8h]
    tGameObjectMain* v22; // [esp+50h] [ebp-2E4h]
    tGameObjectMain* v23; // [esp+54h] [ebp-2E0h]
    tGameObjectMain* v24; // [esp+5Ch] [ebp-2D8h]
    float v25; // [esp+60h] [ebp-2D4h]
    float v26; // [esp+64h] [ebp-2D0h]
    float v27; // [esp+68h] [ebp-2CCh]
    float v28; // [esp+6Ch] [ebp-2C8h]
    float v29; // [esp+70h] [ebp-2C4h]
    tGameObjectMain* v30; // [esp+74h] [ebp-2C0h]
    tGameObjectMain* v31; // [esp+78h] [ebp-2BCh]
    tGameObjectMain* v32; // [esp+88h] [ebp-2ACh]
    float v33; // [esp+90h] [ebp-2A4h]
    tGameObjectMain* v34; // [esp+A4h] [ebp-290h]
    float v35; // [esp+A8h] [ebp-28Ch]
    tGameObjectMain* v36; // [esp+B8h] [ebp-27Ch]
    tGameObjectMain* v37; // [esp+C0h] [ebp-274h]
    tGameObjectMain* v38; // [esp+C8h] [ebp-26Ch]
    float v39; // [esp+CCh] [ebp-268h]
    float v40; // [esp+D0h] [ebp-264h]
    float v41; // [esp+D4h] [ebp-260h]
    tGameObjectMain* v42; // [esp+DCh] [ebp-258h]
    tGameObjectMain* v43; // [esp+E0h] [ebp-254h]
    tGameObjectMain* v44; // [esp+E4h] [ebp-250h]
    float v45; // [esp+E8h] [ebp-24Ch]
    tGameObjectMain* v46; // [esp+F0h] [ebp-244h]
    tGameObjectMain* v47; // [esp+F4h] [ebp-240h]
    tGameObjectMain* v48; // [esp+F8h] [ebp-23Ch]
    tAI* v49; // [esp+FCh] [ebp-238h]
    tAI* v50; // [esp+100h] [ebp-234h]
    tGameObjectMain* v51; // [esp+104h] [ebp-230h]
    tGameObjectMain* v52; // [esp+108h] [ebp-22Ch]
    tGameObjectMain* v53; // [esp+10Ch] [ebp-228h]
    tGameObjectMain* v54; // [esp+110h] [ebp-224h]
    int v55; // [esp+12Ch] [ebp-208h]
    Vec3 v56; // [esp+130h] [ebp-204h] BYREF
    float v57; // [esp+13Ch] [ebp-1F8h]
    float v58; // [esp+140h] [ebp-1F4h]
    float v59; // [esp+144h] [ebp-1F0h]
    Vec3 v60; // [esp+148h] [ebp-1ECh] BYREF
    float v62; // [esp+158h] [ebp-1DCh]
    float v63; // [esp+15Ch] [ebp-1D8h]
    Vec3 v64; // [esp+164h] [ebp-1D0h] BYREF
    Vec3 v65; // [esp+170h] [ebp-1C4h] BYREF
    Vec3 v66; // [esp+17Ch] [ebp-1B8h] BYREF
    Vec3 v67; // [esp+188h] [ebp-1ACh]
    float v68; // [esp+194h] [ebp-1A0h]
    float v69; // [esp+198h] [ebp-19Ch]
    float v70; // [esp+19Ch] [ebp-198h]
    int v71; // [esp+1A4h] [ebp-190h]
    Vec3 v72; // [esp+1A8h] [ebp-18Ch] BYREF
    Vec3 v73; // [esp+1B4h] [ebp-180h] BYREF
    float v74; // [esp+1C0h] [ebp-174h]
    Vec3 v75; // [esp+1C8h] [ebp-16Ch] BYREF
    Vec3 v76; // [esp+1D4h] [ebp-160h] BYREF
    Vec3 v77; // [esp+1E0h] [ebp-154h] BYREF
    int v78; // [esp+1ECh] [ebp-148h]
    int v79; // [esp+1F0h] [ebp-144h]
    float v80; // [esp+1F4h] [ebp-140h]
    tGameObjectMain* v81; // [esp+1FCh] [ebp-138h]
    tGameObjectMain* v82; // [esp+200h] [ebp-134h]
    float v83; // [esp+204h] [ebp-130h]
    float v84; // [esp+208h] [ebp-12Ch]
    Vec3 v85; // [esp+20Ch] [ebp-128h] BYREF
    Vec3 v86; // [esp+218h] [ebp-11Ch] BYREF
    float v87; // [esp+224h] [ebp-110h]
    Vec3 v88; // [esp+22Ch] [ebp-108h] BYREF
    Vec3 v89; // [esp+238h] [ebp-FCh] BYREF
    float v90; // [esp+248h] [ebp-ECh]
    float v91; // [esp+250h] [ebp-E4h]
    tGameObjectMain* v92; // [esp+254h] [ebp-E0h]
    int v93; // [esp+258h] [ebp-DCh]
    Vec3 v94; // [esp+25Ch] [ebp-D8h] BYREF
    int v95; // [esp+268h] [ebp-CCh]
    int v96; // [esp+26Ch] [ebp-C8h]
    Vec3 v97; // [esp+270h] [ebp-C4h] BYREF
    Vec3 v98; // [esp+27Ch] [ebp-B8h] BYREF
    Vec3 v99; // [esp+288h] [ebp-ACh] BYREF
    float v100; // [esp+298h] [ebp-9Ch]
    Vec3 dest; // [esp+29Ch] [ebp-98h] BYREF
    Vec3 v102; // [esp+2A8h] [ebp-8Ch] BYREF
    Vec3 v103; // [esp+2B4h] [ebp-80h] BYREF
    float v104; // [esp+2C0h] [ebp-74h]
    Vec3 v106; // [esp+2C8h] [ebp-6Ch] BYREF
    tScriptStruct_SFXPhare* v107; // [esp+2D4h] [ebp-60h]
    BOOL v108; // [esp+2D8h] [ebp-5Ch]
    Vec3 a1; // [esp+2DCh] [ebp-58h] BYREF
    Vec3 v110; // [esp+2E8h] [ebp-4Ch] BYREF
    Vec3 v111; // [esp+2F4h] [ebp-40h] BYREF
    int v112; // [esp+300h] [ebp-34h]
    tGameObjectMain* v113; // [esp+304h] [ebp-30h]
    int v114; // [esp+308h] [ebp-2Ch]
    Vec3 a3; // [esp+30Ch] [ebp-28h] BYREF
    Vec3 v116; // [esp+318h] [ebp-1Ch] BYREF
    float v117; // [esp+328h] [ebp-Ch]
    int v118; // [esp+32Ch] [ebp-8h]

    v72.x = 1.0;
    v72.y = 0.0;
    v72.z = 0.0;
    v88.x = 1.0;
    v88.y = 0.0;
    v88.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v8 = (tScriptStruct_SFXPhare*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v8 = 0;
    v107 = v8;
    v117 = 0.0;
    v81 = 0;
    v108 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SFX_Phare_SFX_Phare_comportement_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &SFX_Phare_SFX_Phare_comportement_univVars_Flag_speciaux_sauve,
            (void*)SFX_Phare_SFX_Phare_comportement_univVars,
            4488);
        AI2C_ResolveBuffer(
            &SFX_Phare_SFX_Phare_comportement_univVars_Planetes_CurHour,
            (void*)SFX_Phare_SFX_Phare_comportement_univVars,
            6116);
        SFX_Phare_SFX_Phare_comportement_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        SFX_Phare_SFX_Phare_comportement__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
    }
    v108 = 0;
    if (v107->des_SF_activation)
    {
        if (v107->des_SF_activation <= 0)
            v108 = 0;
        else
            v108 = AI_EvalFunc_ARRBitTest_C(
                (int*)(SFX_Phare_SFX_Phare_comportement_univVars_Flag_speciaux_sauve + 4),
                v107->des_SF_activation);
        v108 = !v108;
    }
    if (!v108)
    {
        v54 = (tGameObjectMain*)v107->des_SFX_Sun;
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
        if (!v53)
            goto LABEL_28;
        v50 = (tAI*)v107->des_SFX_Sun;
        if (v50 == WOR_gst_Universe.univAIInstance)
        {
            v49 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v52 = (tGameObjectMain*)v107->des_SFX_Sun;
            if (v50)
            {
                if ((unsigned int)v50 <= 2)
                    v52 = AI_MainActor[(uint32_t)v50 - 1];
                v51 = v52;
            }
            else
            {
                v51 = 0;
            }
            v49 = v51->extended->AIInstance;
        }
        if (((tScriptStruct_SFXSun*)(v49->instanceVarDataCurrent))->i_GFX == -1)
        {
        LABEL_28:
            if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
            {
                v108 = 1;
            }
            else if (*(float*)SFX_Phare_SFX_Phare_comportement_univVars_Planetes_CurHour > 8.0
                && *(float*)SFX_Phare_SFX_Phare_comportement_univVars_Planetes_CurHour < 17.0)
            {
                v108 = 1;
            }
        }
        else
        {
            v108 = 1;
        }
    }
    if (v108)
    {
        if (v107->GFX_Halo != -1)
        {
            v95 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
            SND_StopRq(v107->i_snd_loop);
            v107->i_snd_loop = -1;
            SND_StopRq(v107->i_snd_loop2);
            v107->i_snd_loop2 = -1;
            v48 = (tGameObjectMain*)v107->o_son_lumiere;
            if (v48)
            {
                if ((unsigned int)v48 <= 2)
                    v48 = AI_MainActor[(uint32_t)v48 - 1];
                v47 = v48;
            }
            else
            {
                v47 = 0;
            }
            if (v47)
            {
                v46 = (tGameObjectMain*)v107->o_son_lumiere;
                if (v46)
                {
                    if ((unsigned int)v46 <= 2)
                        v46 = AI_MainActor[(uint32_t)v46 - 1];
                    AI_EvalFunc_SNDStopModifier_C(v46, 0);
                }
                else
                {
                    AI_EvalFunc_SNDStopModifier_C(0, 0);
                }
            }
            AI_EvalFunc_GFXDel_C_CURRENT(v107->GFX_Halo);
            v107->GFX_Halo = -1;
        }
        if (v107->GFX_Halo_2 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v107->GFX_Halo_2);
            v107->GFX_Halo_2 = -1;
        }
        if (v107->GFX_Halo_3 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v107->GFX_Halo_3);
            v107->GFX_Halo_3 = -1;
        }
        if (v107->GFX_Flash != -1)
        {
            if (AI_EvalFunc_TIMEElapsed_C(v107->f_time_light_ON, 0.2f))
            {
                AI_EvalFunc_GFXDel_C_CURRENT(v107->GFX_Flash);
                v107->GFX_Flash = -1;
            }
            else
            {
                v58 = TIM_gf_MainClock - TIM_gf_LockTime;
                v117 = (v58 - v107->f_time_light_ON) / 0.2f;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v85);
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v111);
                v45 = v107->des_f_flash_extrude;
                v66.x = v111.x * v45;
                v66.y = v111.y * v45;
                v66.z = v111.z * v45;
                v94.x = 0.0;
                v94.y = 0.0;
                v94.z = v107->des_Z_offset;
                MATH_VecAddVec(&a1, &v66, &v94);
                MATH_VecAddVec(&v73, &v85, &a1);
                GFXSetCurGlobal(v107->GFX_Flash);
                AI_EvalFunc_GFXSetv_C_CURGLOB(18200, &v73);
                v93 = AI_EvalFunc_COLORBlend_C(v107->c_flash_color, 0, v117);
                AI_EvalFunc_GFXSeti_C_CURGLOB(18102, v93);
            }
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        v62 = TIM_gf_dt;
        v107->f_angle = v107->des_f_rot_speed * TIM_gf_dt + v107->f_angle;
        if (v107->f_angle >= 6.283185)
        {
            if (v107->i_snd_loop == -1)
            {
                v107->i_snd_loop = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 3, 0x18000000);
                AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v107->i_snd_loop);
                v107->i_snd_loop2 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 4, 0x18000000);
                AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v107->i_snd_loop2);
                v44 = (tGameObjectMain*)v107->o_son_lumiere;
                if (v44)
                {
                    if ((unsigned int)v44 <= 2)
                        v44 = AI_MainActor[(uint32_t)v44 - 1];
                    v43 = v44;
                }
                else
                {
                    v43 = 0;
                }
                if (v43)
                {
                    v42 = (tGameObjectMain*)v107->o_son_lumiere;
                    if (v42)
                    {
                        if ((unsigned int)v42 <= 2)
                            v42 = AI_MainActor[(uint32_t)v42 - 1];
                        AI_EvalFunc_SNDPlayModifier_C(v42, 0);
                    }
                    else
                    {
                        AI_EvalFunc_SNDPlayModifier_C(0, 0);
                    }
                }
            }
            v107->i_wp_index = 0;
            v107->f_angle = v107->f_angle - 6.283185f;
        }
        OBJ_RestoreInitialPos(AI_gpst_CurrentGameObject);
        AI_EvalFunc_OBJRotateLocalZ_c(AI_gpst_CurrentGameObject, v107->f_angle);
        if (v107->i_wp_index >= v107->i_nb_wp
            || (v7 = v107->f_angle,
                *(float*)AI_EvalVarArray_C2((char*)&v107->__af_angle_Dimension1, v107->i_wp_index) >= (double)v7))
        {
            v100 = TIM_gf_dt;
            v39 = 6.0f * TIM_gf_dt;
            if (v39 < 0.0)
                v39 = 0.0;
            if (v39 > 1.0)
                v39 = 1.0;
            v90 = 1.0f * v39 + v107->f_Halo_color * (1.0f - v39);
            v107->f_Halo_color = v90;
        }
        else
        {
            v6 = v107->f_angle;
            if (*(float*)AI_EvalVarArray_C2((char*)&v107->__af_angle_Dimension1, v107->i_wp_index)
                + v107->des_f_angle_obscure >= v6)
            {
                v59 = TIM_gf_dt;
                v40 = 6.0f * TIM_gf_dt;
                if (v40 < 0.0)
                    v40 = 0.0;
                if (v40 > 1.0)
                    v40 = 1.0;
                v84 = 0.0f * v40 + v107->f_Halo_color * (1.0f - v40);
                v107->f_Halo_color = v84;
            }
            else
            {
                ++v107->i_wp_index;
                v74 = TIM_gf_dt;
                v41 = 6.0f * TIM_gf_dt;
                if (v41 < 0.0)
                    v41 = 0.0;
                if (v41 > 1.0)
                    v41 = 1.0;
                v83 = 1.0f * v41 + v107->f_Halo_color * (1.0f - v41);
                v107->f_Halo_color = v83;
            }
        }
        if (v107->GFX_Halo == -1)
        {
            v118 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(0, -1.0);
            v96 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            v107->GFX_Halo = v96;
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v107->GFX_Halo, 4, 1);
            v38 = (tGameObjectMain*)SFX_Phare_SFX_Phare_comportement_SFX_light_smoke_gao;
            if (SFX_Phare_SFX_Phare_comportement_SFX_light_smoke_gao)
            {
                if ((unsigned int)SFX_Phare_SFX_Phare_comportement_SFX_light_smoke_gao <= 2)
                    v38 = AI_MainActor[SFX_Phare_SFX_Phare_comportement_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v107->GFX_Halo, v38, 0);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v107->GFX_Halo, 0, 0);
            }
            GFXSetCurGlobal(v107->GFX_Halo);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 3.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 5.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21006, v107->des_f_halo_angle);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21104, v107->des_halo_detail);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v107->des_f_light_length);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v76);
        v75.x = 0.0;
        v75.y = 0.0;
        v75.z = 2.0;
        MATH_VecAddVec(&v98, &v76, &v75);
        GFXSetCurGlobal(v107->GFX_Halo);
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v98);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v60);
        AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v60);
        v112 = AI_EvalFunc_COLORBlend_C(0, v107->des_c_halo, v107->f_Halo_color);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v112);
        if (v107->des_halo_2)
        {
            if (v107->GFX_Halo_2 == -1)
            {
                v107->GFX_Halo_2 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v107->GFX_Halo_2, 4, 1);
                v37 = (tGameObjectMain*)SFX_Phare_SFX_Phare_comportement_SFX_light_smoke_gao;
                if (SFX_Phare_SFX_Phare_comportement_SFX_light_smoke_gao)
                {
                    if ((unsigned int)SFX_Phare_SFX_Phare_comportement_SFX_light_smoke_gao <= 2)
                        v37 = AI_MainActor[SFX_Phare_SFX_Phare_comportement_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v107->GFX_Halo_2, v37, 0);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v107->GFX_Halo_2, 0, 0);
                }
                GFXSetCurGlobal(v107->GFX_Halo_2);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 3.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 5.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21006, v107->des_f_halo_angle);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21104, v107->des_halo_detail);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v107->des_f_light_length);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v106);
            v86.x = 0.0;
            v86.y = 0.0;
            v86.z = 1.5;
            MATH_VecAddVec(&v89, &v106, &v86);
            GFXSetCurGlobal(v107->GFX_Halo_2);
            AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v89);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v110);
            AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v110);
            v71 = AI_EvalFunc_COLORBlend_C(0, v107->des_c_halo, v107->f_Halo_color);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v71);
        }
        if (v107->des_halo_3)
        {
            if (v107->GFX_Halo_3 == -1)
            {
                v107->GFX_Halo_3 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v107->GFX_Halo_2, 4, 1);
                v36 = (tGameObjectMain*)SFX_Phare_SFX_Phare_comportement_SFX_light_smoke_gao;
                if (SFX_Phare_SFX_Phare_comportement_SFX_light_smoke_gao)
                {
                    if ((unsigned int)SFX_Phare_SFX_Phare_comportement_SFX_light_smoke_gao <= 2)
                        v36 = AI_MainActor[SFX_Phare_SFX_Phare_comportement_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v107->GFX_Halo_2, v36, 0);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v107->GFX_Halo_2, 0, 0);
                }
                GFXSetCurGlobal(v107->GFX_Halo_3);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 3.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 5.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21006, v107->des_f_halo_angle);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21104, v107->des_halo_detail);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v107->des_f_light_length);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v97);
            v56.x = 0.0;
            v56.y = 0.0;
            v56.z = 2.5;
            MATH_VecAddVec(&v65, &v97, &v56);
            GFXSetCurGlobal(v107->GFX_Halo_3);
            AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v65);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v102);
            AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v102);
            v114 = AI_EvalFunc_COLORBlend_C(0, v107->des_c_halo, v107->f_Halo_color);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v114);
        }
        v81 = AI_EvalFunc_VIEWGetObject_C(0);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v77);
        v88 = v77;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v99);
        v35 = v107->des_f_flash_extrude;
        v68 = v99.x * v35;
        v69 = v99.y * v35;
        v70 = v99.z * v35;
        v88.x = v88.x + v68;
        v88.y = v88.y + v69;
        v88.z = v88.z + v70;
        v78 = 0;
        v79 = 0;
        v80 = v107->des_Z_offset;
        v88.x = v88.x + 0.0f;
        v88.y = v88.y + 0.0f;
        v88.z = v88.z + v80;
        v34 = v81;
        if (v81)
        {
            if ((unsigned int)v81 <= 2)
                v34 = AI_MainActor[(uint32_t)v81 - 1];
            AI_EvalFunc_OBJGetPos_C(v34, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        MATH_SubstractVec(&dest, &a3, &v88);
        v72 = dest;
        v1 = MATH_VecLen(&v72);
        v107->f_dist_to_cam = (float)v1;
        v33 = 1.0f / v107->f_dist_to_cam;
        v72.x = v72.x * v33;
        v72.y = v72.y * v33;
        v72.z = v72.z * v33;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v103);
        v32 = v81;
        if (v81)
        {
            if ((unsigned int)v81 <= 2)
                v32 = AI_MainActor[(uint32_t)v81 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v32, &v116);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &v116);
        }
        v67.x = -v116.x;
        v67.y = -v116.y;
        v67.z = -v116.z;
        v87 = -v116.z * v103.z + -v116.y * v103.y + v103.x * -v116.x;
        if (v87 <= 0.0)
        {
            v117 = 0.0;
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v64);
            v117 = v72.z * v64.z + v72.y * v64.y + v64.x * v72.x;
        }
        if (v117 > 0.0)
        {
            v107->f_time_light_ON = TIM_gf_MainClock - TIM_gf_LockTime;
            if (v107->GFX_Flash == -1)
            {
                v107->o_camera = (int)v81;
                v107->GFX_Flash = AI_EvalFunc_GFXAdd_C_CURRENT(18);
                v24 = (tGameObjectMain*)SFX_Phare_SFX_Phare_comportement_SFX_light_smoke_gao;
                if (SFX_Phare_SFX_Phare_comportement_SFX_light_smoke_gao)
                {
                    if ((unsigned int)SFX_Phare_SFX_Phare_comportement_SFX_light_smoke_gao <= 2)
                        v24 = AI_MainActor[SFX_Phare_SFX_Phare_comportement_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v107->GFX_Flash, v24, 11);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v107->GFX_Flash, 0, 11);
                }
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v107->GFX_Flash, 2, 1);
                GFXSetCurGlobal(v107->GFX_Flash);
                AI_EvalFunc_GFXSetf_C_CURGLOB(18010, 3.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(18012, 0.5);
                v23 = *AI_MainActor;
                if (*AI_MainActor)
                {
                    if ((unsigned int)v23 <= 2)
                        v23 = AI_MainActor[(uint32_t)v23 - 1];
                    v22 = v23;
                }
                else
                {
                    v22 = 0;
                }
                if (v22 == (tGameObjectMain*)SFX_Phare_SFX_Phare_comportement__PJ_Sally_gao)
                {
                    GFXSetCurGlobal(v107->GFX_Flash);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18000, 0.2f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18001, 10.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18002, 15.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18003, 2.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18004, 4.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18005, 0.2f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18006, 5.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18007, 10.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18008, 3.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18009, 6.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18011, 2.0);
                }
                else
                {
                    GFXSetCurGlobal(v107->GFX_Flash);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18000, 0.2f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18001, 20.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18002, 35.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18003, 10.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18004, 15.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18005, 0.2f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18006, 15.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18007, 20.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18008, 6.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18009, 12.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(18011, 10.0);
                }
                GFXSetCurGlobal(v107->GFX_Flash);
                AI_EvalFunc_GFXSeti_C_CURGLOB(18101, 32);
                AI_EvalFunc_GFXSeti_C_CURGLOB(18103, 8);
                AI_EvalFunc_GFXSeti_C_CURGLOB(18100, 1);
            }
            v21 = (tGameObjectMain*)v107->o_camera;
            if (v21)
            {
                if ((unsigned int)v21 <= 2)
                    v21 = AI_MainActor[(uint32_t)v21 - 1];
                v20 = v21;
            }
            else
            {
                v20 = 0;
            }
            v19 = v81;
            if (v81)
            {
                if ((unsigned int)v81 <= 2)
                    v19 = AI_MainActor[(uint32_t)v81 - 1];
                v18 = v19;
            }
            else
            {
                v18 = 0;
            }
            if (v20 != v18)
            {
                AI_EvalFunc_GFXDel_C_CURRENT(v107->GFX_Flash);
                v107->GFX_Flash = -1;
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                return 1000;
            }
            v113 = *AI_MainActor;
            v17 = v113;
            if (v113)
            {
                if ((unsigned int)v113 <= 2)
                    v17 = AI_MainActor[(uint32_t)v113 - 1];
                v16 = v17;
            }
            else
            {
                v16 = 0;
            }
            if (v16 == (tGameObjectMain*)SFX_Phare_SFX_Phare_comportement__PJ_Sally_gao)
            {
                v15 = v107->f_dist_to_cam - 2.0f;
                v13 = v107->f_dist_to_cam / 2.0f;
                if (v13 <= (double)v15)
                {
                    v14 = v107->f_dist_to_cam - 100.0f;
                    if (v13 >= (double)v14)
                        v57 = v107->f_dist_to_cam / 2.0f;
                    else
                        v57 = v107->f_dist_to_cam - 100.0f;
                }
                else
                {
                    v57 = v107->f_dist_to_cam - 2.0f;
                }
                AI_EvalFunc_GFXSetf_C_CURRENT(v107->GFX_Flash, 18013, v57);
            }
            else
            {
                v12 = v107->f_dist_to_cam - 8.0f;
                v11 = v107->f_dist_to_cam / 2.0f;
                if (v11 > (double)v12)
                    v4 = v107->f_dist_to_cam - 8.0f;
                else
                    v4 = v107->f_dist_to_cam / 2.0f;
                AI_EvalFunc_GFXSetf_C_CURRENT(v107->GFX_Flash, 18013, v4);
            }
            AI_EvalFunc_GFXSetv_C_CURRENT(v107->GFX_Flash, 18200, &v88);
            v82 = *AI_MainActor;
            v10 = v82;
            if (v82)
            {
                if ((unsigned int)v82 <= 2)
                    v10 = AI_MainActor[(uint32_t)v82 - 1];
                v9 = v10;
            }
            else
            {
                v9 = 0;
            }
            if (v9 == (tGameObjectMain*)SFX_Phare_SFX_Phare_comportement__PJ_Sally_gao)
            {
                v2 = AI_EvalFunc_COLORBlend_C(0, 0xFF80C0C0, v117);
            }
            else
            {
                v3 = v107->f_Halo_color * v117;
                v2 = AI_EvalFunc_COLORBlend_C(0, 0xFF80C0C0, v3);
            }
            v107->c_flash_color = v2;
            AI_EvalFunc_GFXSeti_C_CURRENT(v107->GFX_Flash, 18102, v107->c_flash_color);
        }
        else if (AI_EvalFunc_TIMEElapsed_C(v107->f_time_light_ON, 0.2f))
        {
            if (v107->GFX_Flash != -1)
            {
                AI_EvalFunc_GFXDel_C_CURRENT(v107->GFX_Flash);
                v107->GFX_Flash = -1;
            }
        }
        else
        {
            v104 = TIM_gf_MainClock - TIM_gf_LockTime;
            v117 = (v104 - v107->f_time_light_ON) / 0.2f;
            GFXSetCurGlobal(v107->GFX_Flash);
            AI_EvalFunc_GFXSetv_C_CURGLOB(18200, &v88);
            v55 = AI_EvalFunc_COLORBlend_C(v107->c_flash_color, 0, v117);
            AI_EvalFunc_GFXSeti_C_CURGLOB(18102, v55);
            v92 = *AI_MainActor;
            v31 = v92;
            if (v92)
            {
                if ((unsigned int)v92 <= 2)
                    v31 = AI_MainActor[(uint32_t)v92 - 1];
                v30 = v31;
            }
            else
            {
                v30 = 0;
            }
            if (v30 == (tGameObjectMain*)SFX_Phare_SFX_Phare_comportement__PJ_Sally_gao)
            {
                v29 = v107->f_dist_to_cam - 2.0f;
                v27 = v107->f_dist_to_cam / 2.0f;
                if (v27 <= (double)v29)
                {
                    v28 = v107->f_dist_to_cam - 20.0f;
                    if (v27 >= (double)v28)
                        v63 = v107->f_dist_to_cam / 2.0f;
                    else
                        v63 = v107->f_dist_to_cam - 20.0f;
                }
                else
                {
                    v63 = v107->f_dist_to_cam - 2.0f;
                }
                AI_EvalFunc_GFXSetf_C_CURRENT(v107->GFX_Flash, 18013, v63);
            }
            else
            {
                v26 = v107->f_dist_to_cam - 8.0f;
                v25 = v107->f_dist_to_cam / 2.0f;
                if (v25 > (double)v26)
                    v5 = v107->f_dist_to_cam - 8.0f;
                else
                    v5 = v107->f_dist_to_cam / 2.0f;
                v91 = v5;
                AI_EvalFunc_GFXSetf_C_CURRENT(v107->GFX_Flash, 18013, v5);
            }
        }
        result = 1000;
    }
    return result;
}