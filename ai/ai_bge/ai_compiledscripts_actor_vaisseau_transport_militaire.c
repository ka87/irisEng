#include "ai_compiledscripts_inc.h"

//Vaisseau_Transport_Militaire_VTM_exec_GFX
int Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao;

//Vaisseau_Transport_Militaire_VTM_mode_deplacement
int fct_Vaisseau_Transport_Militaire_VTM_mode_deplacement_Vaisseau_Transport_Militaire_VTM_exec_s_b;

//Vaisseau_Transport_Militaire_VTM_exec_afterrec
int Vaisseau_Transport_Militaire_VTM_exec_afterrec_SFX_light_smoke_gao;
int fct_Vaisseau_Transport_Militaire_VTM_exec_afterrec_Vaisseau_Transport_Militaire_VTM_exec_GFX;

//Vaisseau_Transport_Militaire_VTM_init
int fct_Vaisseau_Transport_Militaire_VTM_init_Vaisseau_Transport_Militaire_VTM_mode_deplacement;
int fct_Vaisseau_Transport_Militaire_VTM_init_Vaisseau_Transport_Militaire_VTM_reflex;

int Vaisseau_Transport_Militaire_VTM_exec_GFX()
{
    int result; // eax
    float v1; // [esp+0h] [ebp-330h]
    float v2; // [esp+0h] [ebp-330h]
    float v3; // [esp+0h] [ebp-330h]
    float v4; // [esp+0h] [ebp-330h]
    float v5; // [esp+0h] [ebp-330h]
    float v6; // [esp+0h] [ebp-330h]
    float v7; // [esp+0h] [ebp-330h]
    tScriptStruct_PNJVaisseauTransportMilitaire* v8; // [esp+4h] [ebp-32Ch]
    tGameObjectMain* v9; // [esp+14h] [ebp-31Ch]
    tGameObjectMain* v10; // [esp+1Ch] [ebp-314h]
    tGameObjectMain* v11; // [esp+24h] [ebp-30Ch]
    tGameObjectMain* v12; // [esp+34h] [ebp-2FCh]
    tGameObjectMain* v13; // [esp+3Ch] [ebp-2F4h]
    tGameObjectMain* v14; // [esp+4Ch] [ebp-2E4h]
    tGameObjectMain* v15; // [esp+54h] [ebp-2DCh]
    tGameObjectMain* v16; // [esp+5Ch] [ebp-2D4h]
    tGameObjectMain* v17; // [esp+6Ch] [ebp-2C4h]
    tGameObjectMain* v18; // [esp+74h] [ebp-2BCh]
    tGameObjectMain* v19; // [esp+7Ch] [ebp-2B4h]
    tGameObjectMain* v20; // [esp+84h] [ebp-2ACh]
    tGameObjectMain* v21; // [esp+8Ch] [ebp-2A4h]
    tGameObjectMain* v22; // [esp+94h] [ebp-29Ch]
    tGameObjectMain* v23; // [esp+98h] [ebp-298h]
    tGameObjectMain* v24; // [esp+9Ch] [ebp-294h]
    tGameObjectMain* v25; // [esp+ACh] [ebp-284h]
    tGameObjectMain* v26; // [esp+B4h] [ebp-27Ch]
    tGameObjectMain* v27; // [esp+C4h] [ebp-26Ch]
    tGameObjectMain* v28; // [esp+CCh] [ebp-264h]
    tGameObjectMain* v29; // [esp+DCh] [ebp-254h]
    tGameObjectMain* v30; // [esp+ECh] [ebp-244h]
    tGameObjectMain* v31; // [esp+F4h] [ebp-23Ch]
    tGameObjectMain* v32; // [esp+104h] [ebp-22Ch]
    tGameObjectMain* v33; // [esp+114h] [ebp-21Ch]
    tGameObjectMain* v34; // [esp+11Ch] [ebp-214h]
    tGameObjectMain* v35; // [esp+12Ch] [ebp-204h]
    tGameObjectMain* v36; // [esp+13Ch] [ebp-1F4h]
    tGameObjectMain* v37; // [esp+144h] [ebp-1ECh]
    tGameObjectMain* a1; // [esp+148h] [ebp-1E8h]
    tGameObjectMain* v39; // [esp+14Ch] [ebp-1E4h]
    Vec3 v40; // [esp+150h] [ebp-1E0h] BYREF
    float v41; // [esp+15Ch] [ebp-1D4h]
    float v42; // [esp+160h] [ebp-1D0h]
    float v43; // [esp+164h] [ebp-1CCh]
    float v44; // [esp+168h] [ebp-1C8h]
    float v45; // [esp+16Ch] [ebp-1C4h]
    float v46; // [esp+170h] [ebp-1C0h]
    Vec3 v47; // [esp+178h] [ebp-1B8h] BYREF
    float v48; // [esp+184h] [ebp-1ACh]
    Vec3 v49; // [esp+188h] [ebp-1A8h] BYREF
    Vec3 v50; // [esp+194h] [ebp-19Ch] BYREF
    Vec3 v51; // [esp+1A0h] [ebp-190h] BYREF
    float v52; // [esp+1ACh] [ebp-184h]
    float v53; // [esp+1B0h] [ebp-180h]
    float v54; // [esp+1B4h] [ebp-17Ch]
    float v55; // [esp+1B8h] [ebp-178h]
    float v56; // [esp+1BCh] [ebp-174h]
    float v57; // [esp+1C0h] [ebp-170h]
    float v58; // [esp+1C4h] [ebp-16Ch]
    float v59; // [esp+1CCh] [ebp-164h]
    int v60; // [esp+1D0h] [ebp-160h]
    Vec3 v61; // [esp+1D4h] [ebp-15Ch] BYREF
    Vec3 v62; // [esp+1E0h] [ebp-150h] BYREF
    float v63; // [esp+1ECh] [ebp-144h]
    Vec3 v64; // [esp+1F0h] [ebp-140h] BYREF
    Vec3 v65; // [esp+1FCh] [ebp-134h] BYREF
    Vec3 v66; // [esp+208h] [ebp-128h] BYREF
    Vec3 v67; // [esp+214h] [ebp-11Ch] BYREF
    float v68; // [esp+220h] [ebp-110h]
    Vec3 v69; // [esp+224h] [ebp-10Ch] BYREF
    Vec3 v70; // [esp+230h] [ebp-100h] BYREF
    float v71; // [esp+23Ch] [ebp-F4h]
    Vec3 v72; // [esp+244h] [ebp-ECh] BYREF
    Vec3 v73; // [esp+250h] [ebp-E0h] BYREF
    float v74; // [esp+25Ch] [ebp-D4h]
    float v75; // [esp+260h] [ebp-D0h]
    float v76; // [esp+264h] [ebp-CCh]
    Vec3 v77; // [esp+268h] [ebp-C8h] BYREF
    float v78; // [esp+274h] [ebp-BCh]
    Vec3 v79; // [esp+278h] [ebp-B8h] BYREF
    float v80; // [esp+284h] [ebp-ACh]
    Vec3 v81; // [esp+28Ch] [ebp-A4h] BYREF
    float v82; // [esp+298h] [ebp-98h]
    float v83; // [esp+29Ch] [ebp-94h]
    float v84; // [esp+2A0h] [ebp-90h]
    Vec3 v85; // [esp+2A4h] [ebp-8Ch] BYREF
    Vec3 v86; // [esp+2B4h] [ebp-7Ch] BYREF
    Vec3 v87; // [esp+2C0h] [ebp-70h] BYREF
    float v88; // [esp+2CCh] [ebp-64h]
    float v89; // [esp+2D0h] [ebp-60h]
    int v90; // [esp+2D4h] [ebp-5Ch]
    tScriptStruct_PNJVaisseauTransportMilitaire* v91; // [esp+2D8h] [ebp-58h]
    float v92; // [esp+2DCh] [ebp-54h]
    Vec3 v93; // [esp+2E0h] [ebp-50h] BYREF
    Vec3 a3; // [esp+2ECh] [ebp-44h] BYREF
    float v95[5]; // [esp+2F8h] [ebp-38h] BYREF
    Vec3 v96; // [esp+30Ch] [ebp-24h] BYREF
    Vec3 v97; // [esp+318h] [ebp-18h] BYREF

    v62.x = 1.0;
    v62.y = 0.0;
    v62.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v8 = (tScriptStruct_PNJVaisseauTransportMilitaire*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v8 = 0;
    v91 = v8;
    v90 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    v60 = (unsigned __int16)AI_gpst_CurrentGameObject->flags2;
    if ((v60 & 4) != 0)
    {
        if (v91->GFX_Halo_bleu != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v91->GFX_Halo_bleu);
            v91->GFX_Halo_bleu = -1;
        }
        if (v91->GFX_Halo_rouge != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v91->GFX_Halo_rouge);
            v91->GFX_Halo_rouge = -1;
        }
        if (v91->GFX_Reacteur_1 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v91->GFX_Reacteur_1);
            v91->GFX_Reacteur_1 = -1;
        }
        if (v91->GFX_Reacteur_2 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v91->GFX_Reacteur_2);
            v91->GFX_Reacteur_2 = -1;
        }
        if (v91->GFX_Reacteur_3 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v91->GFX_Reacteur_3);
            v91->GFX_Reacteur_3 = -1;
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        if (v91->des_patrouilleur)
        {
            if (v91->f_reacteur_coef == 0.0)
            {
                if (v91->GFX_Reacteur_1 != -1)
                {
                    AI_EvalFunc_GFXDel_C_CURRENT(v91->GFX_Reacteur_1);
                    v91->GFX_Reacteur_1 = -1;
                    AI_EvalFunc_GFXDel_C_CURRENT(v91->GFX_Reacteur_2);
                    v91->GFX_Reacteur_2 = -1;
                    AI_EvalFunc_GFXDel_C_CURRENT(v91->GFX_Reacteur_3);
                    v91->GFX_Reacteur_3 = -1;
                }
            }
            else
            {
                v88 = TIM_gf_dt;
                v39 = (tGameObjectMain*)v91->o_gyro;
                if (v39)
                {
                    if ((unsigned int)v39 <= 2)
                        v39 = AI_MainActor[(uint32_t)v39 - 1];
                    a1 = v39;
                }
                else
                {
                    a1 = 0;
                }
                v1 = 4.0f * v88;
                AI_EvalFunc_OBJRotateLocalZ_c(a1, v1);
                v90 = AI_EvalFunc_COLORBlend_C(0, 0xFF8080, v91->f_reacteur_coef);
                if (v91->GFX_Reacteur_1 == -1)
                {
                    v91->GFX_Reacteur_1 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                    AI_EvalFunc_GFXFlagSet_C_CURRENT(v91->GFX_Reacteur_1, 4, 1);
                    v37 = (tGameObjectMain*)Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao;
                    if (Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao)
                    {
                        if ((unsigned int)Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao <= 2)
                            v37 = AI_MainActor[Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao - 1];
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v91->GFX_Reacteur_1, v37, 29);
                    }
                    else
                    {
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v91->GFX_Reacteur_1, 0, 29);
                    }
                    GFXSetCurGlobal(v91->GFX_Reacteur_1);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(21104, 5);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0099999998f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 0.0099999998f);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(21100, 4);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 1.5);
                }
                GFXSetCurGlobal(v91->GFX_Reacteur_1);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v90);
                v42 = AI_EvalFunc_MATHRandFloat_C(8.0, 10.0);
                v2 = v91->f_reacteur_coef * v42;
                AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v2);
                v71 = AI_EvalFunc_MATHRandFloat_C(0.1f, 0.30000001f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21006, v71);
                v36 = (tGameObjectMain*)v91->o_motor_1;
                if (v36)
                {
                    if ((unsigned int)v36 <= 2)
                        v36 = AI_MainActor[(uint32_t)v36 - 1];
                    AI_EvalFunc_OBJGetPos_C(v36, (Vec3*)v95);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, (Vec3*)v95);
                }
                v62 = *(Vec3*)v95;
                AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v62);
                v35 = (tGameObjectMain*)v91->o_motor_1;
                if (v35)
                {
                    if ((unsigned int)v35 <= 2)
                        v35 = AI_MainActor[(uint32_t)v35 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v35, &v50);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v50);
                }
                v79.x = -v50.x;
                v79.y = -v50.y;
                v79.z = -v50.z;
                AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v79);
                if (v91->GFX_Reacteur_2 == -1)
                {
                    v91->GFX_Reacteur_2 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                    AI_EvalFunc_GFXFlagSet_C_CURRENT(v91->GFX_Reacteur_2, 4, 1);
                    v34 = (tGameObjectMain*)Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao;
                    if (Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao)
                    {
                        if ((unsigned int)Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao <= 2)
                            v34 = AI_MainActor[Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao - 1];
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v91->GFX_Reacteur_2, v34, 29);
                    }
                    else
                    {
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v91->GFX_Reacteur_2, 0, 29);
                    }
                    GFXSetCurGlobal(v91->GFX_Reacteur_2);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(21104, 5);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0099999998f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 0.0099999998f);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(21100, 4);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 1.5);
                }
                GFXSetCurGlobal(v91->GFX_Reacteur_2);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v90);
                v59 = AI_EvalFunc_MATHRandFloat_C(8.0, 10.0);
                v3 = v91->f_reacteur_coef * v59;
                AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v3);
                v78 = AI_EvalFunc_MATHRandFloat_C(0.1f, 0.30000001f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21006, v78);
                v33 = (tGameObjectMain*)v91->o_motor_2;
                if (v33)
                {
                    if ((unsigned int)v33 <= 2)
                        v33 = AI_MainActor[(uint32_t)v33 - 1];
                    AI_EvalFunc_OBJGetPos_C(v33, &v47);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v47);
                }
                v62 = v47;
                AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v62);
                v32 = (tGameObjectMain*)v91->o_motor_2;
                if (v32)
                {
                    if ((unsigned int)v32 <= 2)
                        v32 = AI_MainActor[(uint32_t)v32 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v32, &v96);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v96);
                }
                v77.x = -v96.x;
                v77.y = -v96.y;
                v77.z = -v96.z;
                AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v77);
                if (v91->GFX_Reacteur_3 == -1)
                {
                    v91->GFX_Reacteur_3 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                    AI_EvalFunc_GFXFlagSet_C_CURRENT(v91->GFX_Reacteur_3, 4, 1);
                    v31 = (tGameObjectMain*)Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao;
                    if (Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao)
                    {
                        if ((unsigned int)Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao <= 2)
                            v31 = AI_MainActor[Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao - 1];
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v91->GFX_Reacteur_3, v31, 29);
                    }
                    else
                    {
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v91->GFX_Reacteur_3, 0, 29);
                    }
                    GFXSetCurGlobal(v91->GFX_Reacteur_3);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(21104, 5);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0099999998f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 0.0099999998f);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(21100, 4);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 1.5);
                }
                GFXSetCurGlobal(v91->GFX_Reacteur_3);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v90);
                v68 = AI_EvalFunc_MATHRandFloat_C(8.0, 10.0);
                v4 = v91->f_reacteur_coef * v68;
                AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v4);
                v43 = AI_EvalFunc_MATHRandFloat_C(0.1f, 0.30000001f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21006, v43);
                v30 = (tGameObjectMain*)v91->o_motor_3;
                if (v30)
                {
                    if ((unsigned int)v30 <= 2)
                        v30 = AI_MainActor[(uint32_t)v30 - 1];
                    AI_EvalFunc_OBJGetPos_C(v30, &v67);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v67);
                }
                v62 = v67;
                AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v62);
                v29 = (tGameObjectMain*)v91->o_motor_3;
                if (v29)
                {
                    if ((unsigned int)v29 <= 2)
                        v29 = AI_MainActor[(uint32_t)v29 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v29, &v81);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v81);
                }
                v72.x = -v81.x;
                v72.y = -v81.y;
                v72.z = -v81.z;
                AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v72);
            }
            if (v91->GFX_Halo_bleu == -1)
            {
                v91->GFX_Halo_bleu = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v91->GFX_Halo_bleu, 4, 1);
                v28 = (tGameObjectMain*)Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao;
                if (Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao)
                {
                    if ((unsigned int)Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao <= 2)
                        v28 = AI_MainActor[Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v91->GFX_Halo_bleu, v28, 29);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v91->GFX_Halo_bleu, 0, 29);
                }
                GFXSetCurGlobal(v91->GFX_Halo_bleu);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21104, 5);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0099999998f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 0.0099999998f);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21100, 4);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 4.0);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 0xC0C0C0);
            }
            v80 = AI_EvalFunc_MATHRandFloat_C(15.0, 16.0);
            GFXSetCurGlobal(v91->GFX_Halo_bleu);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v80);
            v63 = AI_EvalFunc_MATHRandFloat_C(0.75, 0.85000002f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21006, v63);
            v27 = (tGameObjectMain*)v91->o_corps;
            if (v27)
            {
                if ((unsigned int)v27 <= 2)
                    v27 = AI_MainActor[(uint32_t)v27 - 1];
                AI_EvalFunc_OBJGetPos_C(v27, &v61);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v61);
            }
            v62 = v61;
            v26 = (tGameObjectMain*)v91->o_corps;
            if (v26)
            {
                if ((unsigned int)v26 <= 2)
                    v26 = AI_MainActor[(uint32_t)v26 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v26, &v85);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v85);
            }
            v44 = v85.x * 13.75f;
            v45 = v85.y * 13.75f;
            v46 = v85.z * 13.75f;
            v62.x = v62.x - v44;
            v62.y = v62.y - v45;
            v62.z = v62.z - v46;
            AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v62);
            v25 = (tGameObjectMain*)v91->o_corps;
            if (v25)
            {
                if ((unsigned int)v25 <= 2)
                    v25 = AI_MainActor[(uint32_t)v25 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v25, &v93);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v93);
            }
            v69.x = -v93.x;
            v69.y = -v93.y;
            v69.z = -v93.z;
            AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v69);
        }
        else
        {
            v92 = TIM_gf_dt;
            v24 = (tGameObjectMain*)v91->o_gyro;
            if (v24)
            {
                if ((unsigned int)v24 <= 2)
                    v24 = AI_MainActor[(uint32_t)v24 - 1];
                v23 = v24;
            }
            else
            {
                v23 = 0;
            }
            v5 = 2.0f * v92;
            AI_EvalFunc_OBJRotateLocalZ_c(v23, v5);
            if (v91->GFX_Halo_bleu == -1)
            {
                v91->GFX_Halo_bleu = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v91->GFX_Halo_bleu, 4, 1);
                v22 = (tGameObjectMain*)Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao;
                if (Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao)
                {
                    if ((unsigned int)Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao <= 2)
                        v22 = AI_MainActor[Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v91->GFX_Halo_bleu, v22, 29);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v91->GFX_Halo_bleu, 0, 29);
                }
                GFXSetCurGlobal(v91->GFX_Halo_bleu);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 1.0);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 16711680);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 2.0);
            }
            v21 = (tGameObjectMain*)v91->o_light_blue;
            if (v21)
            {
                if ((unsigned int)v21 <= 2)
                    v21 = AI_MainActor[(uint32_t)v21 - 1];
                AI_EvalFunc_OBJGetPos_C(v21, &v73);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v73);
            }
            AI_EvalFunc_GFXSetv_C_CURRENT(v91->GFX_Halo_bleu, 21200, &v73);
            if (v91->GFX_Halo_rouge == -1)
            {
                v91->GFX_Halo_rouge = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v91->GFX_Halo_rouge, 4, 1);
                v20 = (tGameObjectMain*)Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao;
                if (Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao)
                {
                    if ((unsigned int)Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao <= 2)
                        v20 = AI_MainActor[Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v91->GFX_Halo_rouge, v20, 29);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v91->GFX_Halo_rouge, 0, 29);
                }
                GFXSetCurGlobal(v91->GFX_Halo_rouge);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 1.0);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 255);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 2.0);
            }
            v19 = (tGameObjectMain*)v91->o_light_red;
            if (v19)
            {
                if ((unsigned int)v19 <= 2)
                    v19 = AI_MainActor[(uint32_t)v19 - 1];
                AI_EvalFunc_OBJGetPos_C(v19, &v49);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v49);
            }
            AI_EvalFunc_GFXSetv_C_CURRENT(v91->GFX_Halo_rouge, 21200, &v49);
            if (v91->f_reacteur_coef == 0.0)
            {
                if (v91->GFX_Reacteur_1 != -1)
                {
                    AI_EvalFunc_GFXDel_C_CURRENT(v91->GFX_Reacteur_1);
                    v91->GFX_Reacteur_1 = -1;
                    AI_EvalFunc_GFXDel_C_CURRENT(v91->GFX_Reacteur_2);
                    v91->GFX_Reacteur_2 = -1;
                }
            }
            else
            {
                v90 = AI_EvalFunc_COLORBlend_C(0, 16744576, v91->f_reacteur_coef);
                if (v91->GFX_Reacteur_1 == -1)
                {
                    v91->GFX_Reacteur_1 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                    AI_EvalFunc_GFXFlagSet_C_CURRENT(v91->GFX_Reacteur_1, 4, 1);
                    v18 = (tGameObjectMain*)Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao;
                    if (Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao)
                    {
                        if ((unsigned int)Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao <= 2)
                            v18 = AI_MainActor[Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao - 1];
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v91->GFX_Reacteur_1, v18, 29);
                    }
                    else
                    {
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v91->GFX_Reacteur_1, 0, 29);
                    }
                    GFXSetCurGlobal(v91->GFX_Reacteur_1);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(21104, 5);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0099999998f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 0.0099999998f);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(21100, 4);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 1.5);
                }
                GFXSetCurGlobal(v91->GFX_Reacteur_1);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v90);
                v41 = AI_EvalFunc_MATHRandFloat_C(5.0, 8.0);
                v6 = v91->f_reacteur_coef * v41;
                AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v6);
                v58 = AI_EvalFunc_MATHRandFloat_C(0.1f, 0.30000001f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21006, v58);
                v17 = (tGameObjectMain*)v91->o_motor_1;
                if (v17)
                {
                    if ((unsigned int)v17 <= 2)
                        v17 = AI_MainActor[(uint32_t)v17 - 1];
                    AI_EvalFunc_OBJGetPos_C(v17, &v86);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v86);
                }
                v62 = v86;
                v16 = (tGameObjectMain*)v91->o_motor_1;
                if (v16)
                {
                    if ((unsigned int)v16 <= 2)
                        v16 = AI_MainActor[(uint32_t)v16 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v16, &a3);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &a3);
                }
                v74 = a3.x * 0.80000001f;
                v75 = a3.y * 0.80000001f;
                v76 = a3.z * 0.80000001f;
                v62.x = v62.x + v74;
                v62.y = v62.y + v75;
                v62.z = v62.z + v76;
                v15 = (tGameObjectMain*)v91->o_motor_1;
                if (v15)
                {
                    if ((unsigned int)v15 <= 2)
                        v15 = AI_MainActor[(uint32_t)v15 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v15, &v65);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v65);
                }
                v82 = v65.x * 3.1500001f;
                v83 = v65.y * 3.1500001f;
                v84 = v65.z * 3.1500001f;
                v62.x = v62.x - v82;
                v62.y = v62.y - v83;
                v62.z = v62.z - v84;
                AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v62);
                v14 = (tGameObjectMain*)v91->o_motor_1;
                if (v14)
                {
                    if ((unsigned int)v14 <= 2)
                        v14 = AI_MainActor[(uint32_t)v14 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v14, &v51);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v51);
                }
                v64.x = -v51.x;
                v64.y = -v51.y;
                v64.z = -v51.z;
                AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v64);
                if (v91->GFX_Reacteur_2 == -1)
                {
                    v91->GFX_Reacteur_2 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                    AI_EvalFunc_GFXFlagSet_C_CURRENT(v91->GFX_Reacteur_2, 4, 1);
                    v13 = (tGameObjectMain*)Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao;
                    if (Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao)
                    {
                        if ((unsigned int)Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao <= 2)
                            v13 = AI_MainActor[Vaisseau_Transport_Militaire_VTM_exec_GFX_SFX_light_smoke_gao - 1];
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v91->GFX_Reacteur_2, v13, 29);
                    }
                    else
                    {
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v91->GFX_Reacteur_2, 0, 29);
                    }
                    GFXSetCurGlobal(v91->GFX_Reacteur_2);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(21104, 5);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0099999998f);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 0.0099999998f);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(21100, 4);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 1.5);
                }
                GFXSetCurGlobal(v91->GFX_Reacteur_2);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v90);
                v89 = AI_EvalFunc_MATHRandFloat_C(5.0, 8.0);
                v7 = v91->f_reacteur_coef * v89;
                AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v7);
                v48 = AI_EvalFunc_MATHRandFloat_C(0.1f, 0.30000001f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21006, v48);
                v12 = (tGameObjectMain*)v91->o_motor_2;
                if (v12)
                {
                    if ((unsigned int)v12 <= 2)
                        v12 = AI_MainActor[(uint32_t)v12 - 1];
                    AI_EvalFunc_OBJGetPos_C(v12, &v87);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v87);
                }
                v62 = v87;
                v11 = (tGameObjectMain*)v91->o_motor_2;
                if (v11)
                {
                    if ((unsigned int)v11 <= 2)
                        v11 = AI_MainActor[(uint32_t)v11 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v11, &v97);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v97);
                }
                v55 = v97.x * 0.80000001f;
                v56 = v97.y * 0.80000001f;
                v57 = v97.z * 0.80000001f;
                v62.x = v62.x + v55;
                v62.y = v62.y + v56;
                v62.z = v62.z + v57;
                v10 = (tGameObjectMain*)v91->o_motor_2;
                if (v10)
                {
                    if ((unsigned int)v10 <= 2)
                        v10 = AI_MainActor[(uint32_t)v10 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v10, &v70);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v70);
                }
                v52 = v70.x * 3.1500001f;
                v53 = v70.y * 3.1500001f;
                v54 = v70.z * 3.1500001f;
                v62.x = v62.x - v52;
                v62.y = v62.y - v53;
                v62.z = v62.z - v54;
                AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v62);
                v9 = (tGameObjectMain*)v91->o_motor_2;
                if (v9)
                {
                    if ((unsigned int)v9 <= 2)
                        v9 = AI_MainActor[(uint32_t)v9 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v9, &v66);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v66);
                }
                v40.x = -v66.x;
                v40.y = -v66.y;
                v40.z = -v66.z;
                AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v40);
            }
        }
        result = 1000;
    }
    return result;
}

int Vaisseau_Transport_Militaire_VTM_mode_deplacement()
{
    BOOL v0; // eax
    tGameObjectMain* v1; // eax
    int v2; // eax
    tGameObjectMain* v3; // eax
    float X_4; // [esp+4h] [ebp-A58h]
    float v6; // [esp+8h] [ebp-A54h]
    float v7; // [esp+Ch] [ebp-A50h]
    float v8; // [esp+10h] [ebp-A4Ch]
    float v9; // [esp+14h] [ebp-A48h]
    float v10; // [esp+18h] [ebp-A44h]
    float v11; // [esp+1Ch] [ebp-A40h]
    float v12; // [esp+20h] [ebp-A3Ch]
    float v13; // [esp+24h] [ebp-A38h]
    float v14; // [esp+28h] [ebp-A34h]
    float v15; // [esp+2Ch] [ebp-A30h]
    float v16; // [esp+30h] [ebp-A2Ch]
    float v17; // [esp+34h] [ebp-A28h]
    float v18; // [esp+38h] [ebp-A24h]
    tScriptStruct_PNJVaisseauTransportMilitaire* v19; // [esp+3Ch] [ebp-A20h]
    Vec3* v20; // [esp+40h] [ebp-A1Ch]
    Vec3* v21; // [esp+44h] [ebp-A18h]
    tGameObjectMain* v22; // [esp+4Ch] [ebp-A10h]
    tGameObjectMain* v23; // [esp+54h] [ebp-A08h]
    tGameObjectMain* v24; // [esp+5Ch] [ebp-A00h]
    tGameObjectMain* v25; // [esp+64h] [ebp-9F8h]
    tGameObjectMain* v26; // [esp+6Ch] [ebp-9F0h]
    tGameObjectMain* v27; // [esp+74h] [ebp-9E8h]
    tGameObjectMain* v28; // [esp+7Ch] [ebp-9E0h]
    tGameObjectMain* v29; // [esp+84h] [ebp-9D8h]
    tGameObjectMain* v30; // [esp+8Ch] [ebp-9D0h]
    tGameObjectMain* v31; // [esp+94h] [ebp-9C8h]
    tGameObjectMain* v32; // [esp+9Ch] [ebp-9C0h]
    tGameObjectMain* v33; // [esp+A4h] [ebp-9B8h]
    tGameObjectMain* v34; // [esp+ACh] [ebp-9B0h]
    tGameObjectMain* v35; // [esp+B4h] [ebp-9A8h]
    tGameObjectMain* v36; // [esp+BCh] [ebp-9A0h]
    tGameObjectMain* v37; // [esp+C4h] [ebp-998h]
    tGameObjectMain* v38; // [esp+CCh] [ebp-990h]
    tGameObjectMain* v39; // [esp+D4h] [ebp-988h]
    tGameObjectMain* v40; // [esp+DCh] [ebp-980h]
    float v41; // [esp+E0h] [ebp-97Ch]
    tGameObjectMain* v42; // [esp+E8h] [ebp-974h]
    tGameObjectMain* v43; // [esp+F0h] [ebp-96Ch]
    tGameObjectMain* v44; // [esp+F8h] [ebp-964h]
    tGameObjectMain* v45; // [esp+100h] [ebp-95Ch]
    float v46; // [esp+104h] [ebp-958h]
    tGameObjectMain* v47; // [esp+10Ch] [ebp-950h]
    tGameObjectMain* v48; // [esp+114h] [ebp-948h]
    tGameObjectMain* v49; // [esp+11Ch] [ebp-940h]
    tGameObjectMain* v50; // [esp+124h] [ebp-938h]
    float v51; // [esp+128h] [ebp-934h]
    tGameObjectMain* v52; // [esp+130h] [ebp-92Ch]
    tGameObjectMain* v53; // [esp+138h] [ebp-924h]
    tGameObjectMain* v54; // [esp+140h] [ebp-91Ch]
    tGameObjectMain* v55; // [esp+148h] [ebp-914h]
    tGameObjectMain* v56; // [esp+150h] [ebp-90Ch]
    tGameObjectMain* v57; // [esp+158h] [ebp-904h]
    tGameObjectMain* v58; // [esp+160h] [ebp-8FCh]
    tGameObjectMain* v59; // [esp+168h] [ebp-8F4h]
    tGameObjectMain* v60; // [esp+170h] [ebp-8ECh]
    tGameObjectMain* v61; // [esp+178h] [ebp-8E4h]
    tGameObjectMain* v62; // [esp+180h] [ebp-8DCh]
    tGameObjectMain* v63; // [esp+188h] [ebp-8D4h]
    tGameObjectMain* v64; // [esp+190h] [ebp-8CCh]
    tGameObjectMain* v65; // [esp+198h] [ebp-8C4h]
    tGameObjectMain* v66; // [esp+1A0h] [ebp-8BCh]
    tGameObjectMain* v67; // [esp+1A8h] [ebp-8B4h]
    tGameObjectMain* v68; // [esp+1B0h] [ebp-8ACh]
    tGameObjectMain* v69; // [esp+1B8h] [ebp-8A4h]
    float v70; // [esp+1BCh] [ebp-8A0h]
    float v71; // [esp+1C0h] [ebp-89Ch]
    float v72; // [esp+1C4h] [ebp-898h]
    tGameObjectMain* v73; // [esp+1E0h] [ebp-87Ch]
    tGameObjectMain* v74; // [esp+1E8h] [ebp-874h]
    tGameObjectMain* v75; // [esp+1F0h] [ebp-86Ch]
    tGameObjectMain* v76; // [esp+200h] [ebp-85Ch]
    float v77; // [esp+204h] [ebp-858h]
    float v78; // [esp+208h] [ebp-854h]
    tGameObjectMain* v79; // [esp+218h] [ebp-844h]
    float v80; // [esp+220h] [ebp-83Ch]
    tGameObjectMain* v81; // [esp+234h] [ebp-828h]
    tGameObjectMain* v82; // [esp+244h] [ebp-818h]
    tGameObjectMain* v83; // [esp+24Ch] [ebp-810h]
    tGameObjectMain* v84; // [esp+254h] [ebp-808h]
    tGameObjectMain* v85; // [esp+264h] [ebp-7F8h]
    float v86; // [esp+268h] [ebp-7F4h]
    tGameObjectMain* v87; // [esp+270h] [ebp-7ECh]
    tGameObjectMain* v88; // [esp+278h] [ebp-7E4h]
    tGameObjectMain* v89; // [esp+280h] [ebp-7DCh]
    tGameObjectMain* v90; // [esp+288h] [ebp-7D4h]
    float v91; // [esp+28Ch] [ebp-7D0h]
    tGameObjectMain* v92; // [esp+294h] [ebp-7C8h]
    tGameObjectMain* v93; // [esp+29Ch] [ebp-7C0h]
    tGameObjectMain* v94; // [esp+2A4h] [ebp-7B8h]
    tGameObjectMain* v95; // [esp+2ACh] [ebp-7B0h]
    float v96; // [esp+2B0h] [ebp-7ACh]
    tGameObjectMain* v97; // [esp+2B8h] [ebp-7A4h]
    tGameObjectMain* v98; // [esp+2C0h] [ebp-79Ch]
    tGameObjectMain* v99; // [esp+2C8h] [ebp-794h]
    tGameObjectMain* v100; // [esp+2D0h] [ebp-78Ch]
    tGameObjectMain* v101; // [esp+2D8h] [ebp-784h]
    tGameObjectMain* v102; // [esp+2E0h] [ebp-77Ch]
    tGameObjectMain* v103; // [esp+2E8h] [ebp-774h]
    tGameObjectMain* v104; // [esp+2F0h] [ebp-76Ch]
    tGameObjectMain* v105; // [esp+2F8h] [ebp-764h]
    tGameObjectMain* v106; // [esp+300h] [ebp-75Ch]
    tGameObjectMain* v107; // [esp+308h] [ebp-754h]
    tGameObjectMain* v108; // [esp+310h] [ebp-74Ch]
    tGameObjectMain* v109; // [esp+318h] [ebp-744h]
    tGameObjectMain* v110; // [esp+320h] [ebp-73Ch]
    tGameObjectMain* v111; // [esp+328h] [ebp-734h]
    tGameObjectMain* v112; // [esp+330h] [ebp-72Ch]
    tGameObjectMain* v113; // [esp+338h] [ebp-724h]
    tGameObjectMain* v114; // [esp+340h] [ebp-71Ch]
    tGameObjectMain* v115; // [esp+348h] [ebp-714h]
    tGameObjectMain* v116; // [esp+350h] [ebp-70Ch]
    tGameObjectMain* v117; // [esp+358h] [ebp-704h]
    tGameObjectMain* v118; // [esp+360h] [ebp-6FCh]
    tGameObjectMain* v119; // [esp+368h] [ebp-6F4h]
    tGameObjectMain* v120; // [esp+370h] [ebp-6ECh]
    tGameObjectMain* v121; // [esp+378h] [ebp-6E4h]
    tGameObjectMain* v122; // [esp+380h] [ebp-6DCh]
    tGameObjectMain* v123; // [esp+388h] [ebp-6D4h]
    tGameObjectMain* v124; // [esp+390h] [ebp-6CCh]
    tGameObjectMain* v125; // [esp+398h] [ebp-6C4h]
    tGameObjectMain* v126; // [esp+3A0h] [ebp-6BCh]
    tGameObjectMain* v127; // [esp+3A8h] [ebp-6B4h]
    tGameObjectMain* v128; // [esp+3B0h] [ebp-6ACh]
    tGameObjectMain* v129; // [esp+3B8h] [ebp-6A4h]
    float v130; // [esp+3BCh] [ebp-6A0h]
    tGameObjectMain* v131; // [esp+3C4h] [ebp-698h]
    tGameObjectMain* v132; // [esp+3CCh] [ebp-690h]
    Vec3 v133; // [esp+3D0h] [ebp-68Ch] BYREF
    Vec3 v134; // [esp+3DCh] [ebp-680h] BYREF
    Vec3 v135; // [esp+3E8h] [ebp-674h] BYREF
    Vec3 v136; // [esp+3F4h] [ebp-668h] BYREF
    Vec3 v137; // [esp+40Ch] [ebp-650h] BYREF
    Vec3 v138; // [esp+418h] [ebp-644h] BYREF
    Vec3 v139; // [esp+424h] [ebp-638h] BYREF
    Vec3 v140; // [esp+430h] [ebp-62Ch] BYREF
    float v141; // [esp+43Ch] [ebp-620h]
    float v142; // [esp+440h] [ebp-61Ch]
    float v143; // [esp+444h] [ebp-618h]
    int v144; // [esp+44Ch] [ebp-610h]
    float v146; // [esp+454h] [ebp-608h]
    float v147; // [esp+458h] [ebp-604h]
    float v148; // [esp+45Ch] [ebp-600h]
    Vec3 v149; // [esp+46Ch] [ebp-5F0h] BYREF
    Vec3 v150; // [esp+478h] [ebp-5E4h] BYREF
    Vec3 v151; // [esp+484h] [ebp-5D8h] BYREF
    Vec3 v152; // [esp+490h] [ebp-5CCh] BYREF
    Vec3 v153; // [esp+49Ch] [ebp-5C0h] BYREF
    Vec3 v154; // [esp+4A8h] [ebp-5B4h] BYREF
    Vec3 v155; // [esp+4B4h] [ebp-5A8h] BYREF
    Vec3 v156; // [esp+4C0h] [ebp-59Ch] BYREF
    Vec3 v157; // [esp+4CCh] [ebp-590h] BYREF
    float v158; // [esp+4D8h] [ebp-584h]
    float v159; // [esp+4DCh] [ebp-580h]
    float v160; // [esp+4E0h] [ebp-57Ch]
    float v161; // [esp+4E4h] [ebp-578h]
    float v162; // [esp+4E8h] [ebp-574h]
    float v163; // [esp+4ECh] [ebp-570h]
    int v164; // [esp+4F0h] [ebp-56Ch]
    Vec3 v; // [esp+4F4h] [ebp-568h] BYREF
    Vec3 v166; // [esp+500h] [ebp-55Ch] BYREF
    float v167; // [esp+50Ch] [ebp-550h]
    float v168; // [esp+510h] [ebp-54Ch]
    float v169; // [esp+514h] [ebp-548h]
    Vec3 v170; // [esp+51Ch] [ebp-540h] BYREF
    Vec3 v171; // [esp+528h] [ebp-534h] BYREF
    Vec3 a1; // [esp+534h] [ebp-528h] BYREF
    Vec3 v173; // [esp+540h] [ebp-51Ch] BYREF
    int v174; // [esp+54Ch] [ebp-510h]
    BOOL v175; // [esp+554h] [ebp-508h]
    Vec3 v176; // [esp+558h] [ebp-504h] BYREF
    Vec3 v177; // [esp+564h] [ebp-4F8h] BYREF
    Vec3 v178; // [esp+570h] [ebp-4ECh] BYREF
    Vec3 v179; // [esp+57Ch] [ebp-4E0h] BYREF
    Vec3 v180; // [esp+588h] [ebp-4D4h] BYREF
    Vec3 v181; // [esp+594h] [ebp-4C8h] BYREF
    Vec3 dest; // [esp+5A0h] [ebp-4BCh] BYREF
    Vec3 v183; // [esp+5ACh] [ebp-4B0h] BYREF
    Vec3 v184; // [esp+5B8h] [ebp-4A4h] BYREF
    Vec3 v185; // [esp+5C4h] [ebp-498h] BYREF
    Vec3 v186; // [esp+5D0h] [ebp-48Ch] BYREF
    float v188; // [esp+5E4h] [ebp-478h]
    float v189; // [esp+5E8h] [ebp-474h]
    float v190; // [esp+5ECh] [ebp-470h]
    float v191; // [esp+5F0h] [ebp-46Ch]
    Vec3 v192; // [esp+5F4h] [ebp-468h] BYREF
    Vec3 v193; // [esp+600h] [ebp-45Ch] BYREF
    Vec3 v194; // [esp+60Ch] [ebp-450h] BYREF
    Vec3 v195; // [esp+618h] [ebp-444h] BYREF
    Vec3 v196; // [esp+624h] [ebp-438h] BYREF
    Vec3 v197; // [esp+630h] [ebp-42Ch] BYREF
    Vec3 v198; // [esp+63Ch] [ebp-420h] BYREF
    float v199; // [esp+648h] [ebp-414h]
    float v200; // [esp+64Ch] [ebp-410h]
    float v201; // [esp+650h] [ebp-40Ch]
    BOOL v202; // [esp+654h] [ebp-408h]
    Vec3 v203; // [esp+658h] [ebp-404h] BYREF
    Vec3 v204; // [esp+664h] [ebp-3F8h] BYREF
    float v205; // [esp+670h] [ebp-3ECh]
    int v206; // [esp+674h] [ebp-3E8h]
    int v207; // [esp+678h] [ebp-3E4h]
    Vec3 v209; // [esp+680h] [ebp-3DCh] BYREF
    Vec3 v210; // [esp+68Ch] [ebp-3D0h] BYREF
    float v211; // [esp+698h] [ebp-3C4h]
    float v212; // [esp+69Ch] [ebp-3C0h]
    float v213; // [esp+6A0h] [ebp-3BCh]
    Vec3 v214; // [esp+6A4h] [ebp-3B8h] BYREF
    Vec3 v215; // [esp+6B0h] [ebp-3ACh] BYREF
    Vec3 v216; // [esp+6BCh] [ebp-3A0h] BYREF
    Vec3 v217; // [esp+6C8h] [ebp-394h] BYREF
    Vec3 v218; // [esp+6D4h] [ebp-388h] BYREF
    Vec3 v219; // [esp+6E0h] [ebp-37Ch] BYREF
    Vec3 v220; // [esp+6ECh] [ebp-370h] BYREF
    int v221; // [esp+6F8h] [ebp-364h]
    Vec3 v222; // [esp+6FCh] [ebp-360h] BYREF
    Vec3 v223; // [esp+708h] [ebp-354h] BYREF
    Vec3 v224; // [esp+714h] [ebp-348h] BYREF
    float v225; // [esp+720h] [ebp-33Ch]
    float v226; // [esp+724h] [ebp-338h]
    float v227; // [esp+728h] [ebp-334h]
    Vec3 v228; // [esp+72Ch] [ebp-330h] BYREF
    Vec3 v229; // [esp+738h] [ebp-324h] BYREF
    Vec3 v230; // [esp+744h] [ebp-318h] BYREF
    Vec3 v231; // [esp+750h] [ebp-30Ch] BYREF
    Vec3 v232; // [esp+75Ch] [ebp-300h] BYREF
    Vec3 v235; // [esp+774h] [ebp-2E8h] BYREF
    Vec3 v236; // [esp+780h] [ebp-2DCh] BYREF
    Vec3 v237; // [esp+78Ch] [ebp-2D0h] BYREF
    Vec3 v238; // [esp+798h] [ebp-2C4h] BYREF
    Vec3 a4; // [esp+7A4h] [ebp-2B8h] BYREF
    Vec3 v240; // [esp+7B0h] [ebp-2ACh] BYREF
    Vec3 v241; // [esp+7BCh] [ebp-2A0h]
    Vec3 v242; // [esp+7C8h] [ebp-294h] BYREF
    Vec3 v243; // [esp+7D4h] [ebp-288h] BYREF
    Vec3 v244; // [esp+7E0h] [ebp-27Ch] BYREF
    Vec3 v245; // [esp+7ECh] [ebp-270h] BYREF
    Vec3 v246; // [esp+7F8h] [ebp-264h] BYREF
    Vec3 v247; // [esp+804h] [ebp-258h] BYREF
    int v248; // [esp+810h] [ebp-24Ch]
    float v249; // [esp+814h] [ebp-248h]
    float v250; // [esp+818h] [ebp-244h]
    float v251; // [esp+81Ch] [ebp-240h]
    Vec3 a3; // [esp+824h] [ebp-238h] BYREF
    Vec3 v253; // [esp+830h] [ebp-22Ch] BYREF
    Vec3 v254; // [esp+83Ch] [ebp-220h] BYREF
    Vec3 v255; // [esp+848h] [ebp-214h] BYREF
    int v256; // [esp+854h] [ebp-208h]
    Vec3 v258; // [esp+85Ch] [ebp-200h] BYREF
    Vec3 v259; // [esp+868h] [ebp-1F4h] BYREF
    Vec3 v260; // [esp+874h] [ebp-1E8h] BYREF
    Vec3 v261; // [esp+880h] [ebp-1DCh] BYREF
    Vec3 v262; // [esp+88Ch] [ebp-1D0h] BYREF
    Vec3 v263; // [esp+898h] [ebp-1C4h] BYREF
    Vec3 v264; // [esp+8A4h] [ebp-1B8h] BYREF
    Vec3 v265; // [esp+8B0h] [ebp-1ACh] BYREF
    Vec3 v267; // [esp+8C4h] [ebp-198h] BYREF
    float v268; // [esp+8D0h] [ebp-18Ch]
    float v269; // [esp+8D4h] [ebp-188h]
    float v270; // [esp+8D8h] [ebp-184h]
    Vec3 v271; // [esp+8DCh] [ebp-180h] BYREF
    Vec3 v272; // [esp+8E8h] [ebp-174h] BYREF
    Vec3 v273; // [esp+8F4h] [ebp-168h] BYREF
    Vec3 v274; // [esp+900h] [ebp-15Ch] BYREF
    Vec3 v275; // [esp+90Ch] [ebp-150h] BYREF
    Vec3 v276; // [esp+918h] [ebp-144h] BYREF
    Vec3 v277; // [esp+924h] [ebp-138h] BYREF
    float v278; // [esp+930h] [ebp-12Ch]
    Vec3 v279; // [esp+938h] [ebp-124h] BYREF
    Vec3 v280; // [esp+944h] [ebp-118h] BYREF
    Vec3 v281; // [esp+950h] [ebp-10Ch] BYREF
    Vec3 v282; // [esp+95Ch] [ebp-100h] BYREF
    Vec3 v283; // [esp+968h] [ebp-F4h] BYREF
    Vec3 v284; // [esp+974h] [ebp-E8h] BYREF
    Vec3 v285; // [esp+980h] [ebp-DCh] BYREF
    Vec3 v286; // [esp+98Ch] [ebp-D0h] BYREF
    Vec3 v287; // [esp+998h] [ebp-C4h] BYREF
    tScriptStruct_PNJVaisseauTransportMilitaire* v288; // [esp+9A4h] [ebp-B8h]
    float v289; // [esp+9A8h] [ebp-B4h]
    float v290; // [esp+9ACh] [ebp-B0h]
    float v291; // [esp+9B0h] [ebp-ACh]
    Vec3 v292; // [esp+9B4h] [ebp-A8h] BYREF
    Vec3 v293; // [esp+9C0h] [ebp-9Ch] BYREF
    Vec3 v294; // [esp+9CCh] [ebp-90h] BYREF
    Vec3 v295; // [esp+9D8h] [ebp-84h] BYREF
    Vec3 v296; // [esp+9E4h] [ebp-78h] BYREF
    float v297; // [esp+9F0h] [ebp-6Ch]
    float v298; // [esp+9F4h] [ebp-68h]
    float v299; // [esp+9F8h] [ebp-64h]
    Vec3 v300; // [esp+9FCh] [ebp-60h] BYREF
    Vec3 v301; // [esp+A08h] [ebp-54h] BYREF
    Vec3 v302; // [esp+A14h] [ebp-48h] BYREF
    Vec3 v303; // [esp+A20h] [ebp-3Ch] BYREF
    Vec3 v304; // [esp+A2Ch] [ebp-30h] BYREF
    Vec3 v305; // [esp+A38h] [ebp-24h] BYREF
    Vec3 v306; // [esp+A44h] [ebp-18h] BYREF
    float v307; // [esp+A50h] [ebp-Ch]
    float v308; // [esp+A54h] [ebp-8h]
    float v309; // [esp+A58h] [ebp-4h]

    v241.x = 1.0;
    v241.y = 0.0;
    v241.z = 0.0;
    v203.x = 1.0;
    v203.y = 0.0;
    v203.z = 0.0;
    v.x = 1.0;
    v.y = 0.0;
    v.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v19 = (tScriptStruct_PNJVaisseauTransportMilitaire*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v19 = 0;
    v288 = v19;
    v167 = 0.0;
    v308 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Vaisseau_Transport_Militaire_VTM_mode_deplacement_Vaisseau_Transport_Militaire_VTM_exec_s_b = (int)AI_ul_RealResolveOneRefFunction(0x72000B51u);
    }
    v132 = (tGameObjectMain*)v288->o_next_wp;
    if (v132)
    {
        if ((unsigned int)v132 <= 2)
            v132 = AI_MainActor[(uint32_t)v132 - 1];
        v0 = AI_EvalFunc_OBJ_CapaTest_C(v132, 0x100);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x100);
    }
    v202 = v0;
    if (v0)
    {
        if (!v288->des_patrouilleur)
        {
            if (!v288->i_flag_pod_activation && !v288->i_flag_pod_out)
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v276);
                v131 = (tGameObjectMain*)v288->o_next_wp;
                if (v131)
                {
                    if ((unsigned int)v131 <= 2)
                        v131 = AI_MainActor[(uint32_t)v131 - 1];
                    AI_EvalFunc_OBJGetPos_C(v131, &a3);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &a3);
                }
                if (a3.z + 18.0 > v276.z)
                {
                    v288->f_time_pod_activation = TIM_gf_MainClock - TIM_gf_LockTime;
                    v288->i_flag_pod_activation = 1;
                    v164 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
                }
            }
            if (v288->i_flag_pod_activation)
            {
                if (AI_EvalFunc_TIMEElapsed_C(v288->f_time_pod_activation, 1.0))
                {
                    v288->f_pod_1_range = 0.75;
                    v288->f_pod_2_range = 0.75;
                    v288->f_pod_3_range = 0.75;
                    v102 = (tGameObjectMain*)v288->o_arceau_1;
                    if (v102)
                    {
                        if ((unsigned int)v102 <= 2)
                            v102 = AI_MainActor[(uint32_t)v102 - 1];
                        OBJ_RestoreInitialPos(v102);
                    }
                    else
                    {
                        OBJ_RestoreInitialPos(0);
                    }
                    v101 = (tGameObjectMain*)v288->o_arceau_2;
                    if (v101)
                    {
                        if ((unsigned int)v101 <= 2)
                            v101 = AI_MainActor[(uint32_t)v101 - 1];
                        OBJ_RestoreInitialPos(v101);
                    }
                    else
                    {
                        OBJ_RestoreInitialPos(0);
                    }
                    v100 = (tGameObjectMain*)v288->o_arceau_3;
                    if (v100)
                    {
                        if ((unsigned int)v100 <= 2)
                            v100 = AI_MainActor[(uint32_t)v100 - 1];
                        OBJ_RestoreInitialPos(v100);
                    }
                    else
                    {
                        OBJ_RestoreInitialPos(0);
                    }
                    v99 = (tGameObjectMain*)v288->o_pod_1;
                    if (v99)
                    {
                        if ((unsigned int)v99 <= 2)
                            v99 = AI_MainActor[(uint32_t)v99 - 1];
                        OBJ_RestoreInitialPos(v99);
                    }
                    else
                    {
                        OBJ_RestoreInitialPos(0);
                    }
                    v98 = (tGameObjectMain*)v288->o_pod_1;
                    if (v98)
                    {
                        if ((unsigned int)v98 <= 2)
                            v98 = AI_MainActor[(uint32_t)v98 - 1];
                        AI_EvalFunc_OBJGetPos_C(v98, &v217);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v217);
                    }
                    v97 = (tGameObjectMain*)v288->o_pod_1;
                    if (v97)
                    {
                        if ((unsigned int)v97 <= 2)
                            v97 = AI_MainActor[(uint32_t)v97 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v97, &v302);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v302);
                    }
                    v96 = v288->f_pod_1_range;
                    v271.x = v302.x * v96;
                    v271.y = v302.y * v96;
                    v271.z = v302.z * v96;
                    MATH_SubstractVec(&v171, &v217, &v271);
                    v95 = (tGameObjectMain*)v288->o_pod_1;
                    if (v95)
                    {
                        if ((unsigned int)v95 <= 2)
                            v95 = AI_MainActor[(uint32_t)v95 - 1];
                        AI_EvalFunc_OBJSetPos_C(v95, &v171);
                    }
                    else
                    {
                        AI_EvalFunc_OBJSetPos_C(0, &v171);
                    }
                    v94 = (tGameObjectMain*)v288->o_pod_2;
                    if (v94)
                    {
                        if ((unsigned int)v94 <= 2)
                            v94 = AI_MainActor[(uint32_t)v94 - 1];
                        OBJ_RestoreInitialPos(v94);
                    }
                    else
                    {
                        OBJ_RestoreInitialPos(0);
                    }
                    v93 = (tGameObjectMain*)v288->o_pod_2;
                    if (v93)
                    {
                        if ((unsigned int)v93 <= 2)
                            v93 = AI_MainActor[(uint32_t)v93 - 1];
                        AI_EvalFunc_OBJGetPos_C(v93, &v273);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v273);
                    }
                    v92 = (tGameObjectMain*)v288->o_pod_2;
                    if (v92)
                    {
                        if ((unsigned int)v92 <= 2)
                            v92 = AI_MainActor[(uint32_t)v92 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v92, &v306);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v306);
                    }
                    v91 = v288->f_pod_2_range;
                    v184.x = v306.x * v91;
                    v184.y = v306.y * v91;
                    v184.z = v306.z * v91;
                    MATH_SubstractVec(&v229, &v273, &v184);
                    v90 = (tGameObjectMain*)v288->o_pod_2;
                    if (v90)
                    {
                        if ((unsigned int)v90 <= 2)
                            v90 = AI_MainActor[(uint32_t)v90 - 1];
                        AI_EvalFunc_OBJSetPos_C(v90, &v229);
                    }
                    else
                    {
                        AI_EvalFunc_OBJSetPos_C(0, &v229);
                    }
                    v89 = (tGameObjectMain*)v288->o_pod_3;
                    if (v89)
                    {
                        if ((unsigned int)v89 <= 2)
                            v89 = AI_MainActor[(uint32_t)v89 - 1];
                        OBJ_RestoreInitialPos(v89);
                    }
                    else
                    {
                        OBJ_RestoreInitialPos(0);
                    }
                    v88 = (tGameObjectMain*)v288->o_pod_3;
                    if (v88)
                    {
                        if ((unsigned int)v88 <= 2)
                            v88 = AI_MainActor[(uint32_t)v88 - 1];
                        AI_EvalFunc_OBJGetPos_C(v88, &v178);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v178);
                    }
                    v87 = (tGameObjectMain*)v288->o_pod_3;
                    if (v87)
                    {
                        if ((unsigned int)v87 <= 2)
                            v87 = AI_MainActor[(uint32_t)v87 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v87, &v215);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v215);
                    }
                    v86 = v288->f_pod_3_range;
                    v136.x = v215.x * v86;
                    v136.y = v215.y * v86;
                    v136.z = v215.z * v86;
                    MATH_SubstractVec(&v267, &v178, &v136);
                    v85 = (tGameObjectMain*)v288->o_pod_3;
                    if (v85)
                    {
                        if ((unsigned int)v85 <= 2)
                            v85 = AI_MainActor[(uint32_t)v85 - 1];
                        AI_EvalFunc_OBJSetPos_C(v85, &v267);
                    }
                    else
                    {
                        AI_EvalFunc_OBJSetPos_C(0, &v267);
                    }
                    v288->i_flag_pod_activation = 0;
                    v288->i_flag_pod_out = 1;
                }
                else
                {
                    v299 = TIM_gf_MainClock - TIM_gf_LockTime;
                    v167 = v299 - v288->f_time_pod_activation;
                    v130 = v167;
                    if (v167 < 0.0)
                        v130 = 0.0;
                    if (v130 > 1.0)
                        v130 = 1.0;
                    v288->f_arceau_angle = 0.0f * v130 + (1.0f - v130) * -0.40000001f;
                    v129 = (tGameObjectMain*)v288->o_arceau_1;
                    if (v129)
                    {
                        if ((unsigned int)v129 <= 2)
                            v129 = AI_MainActor[(uint32_t)v129 - 1];
                        OBJ_RestoreInitialPos(v129);
                    }
                    else
                    {
                        OBJ_RestoreInitialPos(0);
                    }
                    v128 = (tGameObjectMain*)v288->o_arceau_1;
                    if (v128)
                    {
                        if ((unsigned int)v128 <= 2)
                            v128 = AI_MainActor[(uint32_t)v128 - 1];
                        AI_EvalFunc_OBJ_SightGet_C(v128, &v255);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_SightGet_C(0, &v255);
                    }
                    v127 = (tGameObjectMain*)v288->o_arceau_1;
                    if (v127)
                    {
                        if ((unsigned int)v127 <= 2)
                            v127 = AI_MainActor[(uint32_t)v127 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v127, &v292);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v292);
                    }
                    v200 = AI_EvalFunc_MTHFloatSign_C(v288->f_arceau_angle);
                    v126 = (tGameObjectMain*)v288->o_arceau_1;
                    if (v126)
                    {
                        if ((unsigned int)v126 <= 2)
                            v126 = AI_MainActor[(uint32_t)v126 - 1];
                        AI_EvalFunc_OBJ_HorizonGet_C(v126, &v246);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_HorizonGet_C(0, &v246);
                    }
                    MATH_VecMultScalar(&a1, &v246, v200);
                    v18 = (float)fabs(v288->f_arceau_angle);
                    v307 = v18;
                    AI_EvalFunc_MATHVecBlend_C(&v292, &a1, v18, &a4);
                    v125 = (tGameObjectMain*)v288->o_arceau_1;
                    if (v125)
                    {
                        if ((unsigned int)v125 <= 2)
                            v125 = AI_MainActor[(uint32_t)v125 - 1];
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(v125, &v255, &a4);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v255, &a4);
                    }
                    v124 = (tGameObjectMain*)v288->o_arceau_2;
                    if (v124)
                    {
                        if ((unsigned int)v124 <= 2)
                            v124 = AI_MainActor[(uint32_t)v124 - 1];
                        OBJ_RestoreInitialPos(v124);
                    }
                    else
                    {
                        OBJ_RestoreInitialPos(0);
                    }
                    v123 = (tGameObjectMain*)v288->o_arceau_2;
                    if (v123)
                    {
                        if ((unsigned int)v123 <= 2)
                            v123 = AI_MainActor[(uint32_t)v123 - 1];
                        AI_EvalFunc_OBJ_SightGet_C(v123, &v195);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_SightGet_C(0, &v195);
                    }
                    v122 = (tGameObjectMain*)v288->o_arceau_2;
                    if (v122)
                    {
                        if ((unsigned int)v122 <= 2)
                            v122 = AI_MainActor[(uint32_t)v122 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v122, &v220);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v220);
                    }
                    v163 = AI_EvalFunc_MTHFloatSign_C(v288->f_arceau_angle);
                    v121 = (tGameObjectMain*)v288->o_arceau_2;
                    if (v121)
                    {
                        if ((unsigned int)v121 <= 2)
                            v121 = AI_MainActor[(uint32_t)v121 - 1];
                        AI_EvalFunc_OBJ_HorizonGet_C(v121, &v218);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_HorizonGet_C(0, &v218);
                    }
                    MATH_VecMultScalar(&v264, &v218, v163);
                    v17 = (float)fabs(v288->f_arceau_angle);
                    v249 = v17;
                    AI_EvalFunc_MATHVecBlend_C(&v220, &v264, v17, &v301);
                    v120 = (tGameObjectMain*)v288->o_arceau_2;
                    if (v120)
                    {
                        if ((unsigned int)v120 <= 2)
                            v120 = AI_MainActor[(uint32_t)v120 - 1];
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(v120, &v195, &v301);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v195, &v301);
                    }
                    v119 = (tGameObjectMain*)v288->o_arceau_3;
                    if (v119)
                    {
                        if ((unsigned int)v119 <= 2)
                            v119 = AI_MainActor[(uint32_t)v119 - 1];
                        OBJ_RestoreInitialPos(v119);
                    }
                    else
                    {
                        OBJ_RestoreInitialPos(0);
                    }
                    v118 = (tGameObjectMain*)v288->o_arceau_3;
                    if (v118)
                    {
                        if ((unsigned int)v118 <= 2)
                            v118 = AI_MainActor[(uint32_t)v118 - 1];
                        AI_EvalFunc_OBJ_SightGet_C(v118, &v254);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_SightGet_C(0, &v254);
                    }
                    v117 = (tGameObjectMain*)v288->o_arceau_3;
                    if (v117)
                    {
                        if ((unsigned int)v117 <= 2)
                            v117 = AI_MainActor[(uint32_t)v117 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v117, &v204);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v204);
                    }
                    v205 = AI_EvalFunc_MTHFloatSign_C(v288->f_arceau_angle);
                    v116 = (tGameObjectMain*)v288->o_arceau_3;
                    if (v116)
                    {
                        if ((unsigned int)v116 <= 2)
                            v116 = AI_MainActor[(uint32_t)v116 - 1];
                        AI_EvalFunc_OBJ_HorizonGet_C(v116, &v263);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_HorizonGet_C(0, &v263);
                    }
                    MATH_VecMultScalar(&v166, &v263, v205);
                    v16 = (float)fabs(v288->f_arceau_angle);
                    v298 = v16;
                    AI_EvalFunc_MATHVecBlend_C(&v204, &v166, v16, &v224);
                    v115 = (tGameObjectMain*)v288->o_arceau_3;
                    if (v115)
                    {
                        if ((unsigned int)v115 <= 2)
                            v115 = AI_MainActor[(uint32_t)v115 - 1];
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(v115, &v254, &v224);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v254, &v224);
                    }
                    v114 = (tGameObjectMain*)v288->o_pod_1;
                    if (v114)
                    {
                        if ((unsigned int)v114 <= 2)
                            v114 = AI_MainActor[(uint32_t)v114 - 1];
                        OBJ_RestoreInitialPos(v114);
                    }
                    else
                    {
                        OBJ_RestoreInitialPos(0);
                    }
                    v113 = (tGameObjectMain*)v288->o_pod_1;
                    if (v113)
                    {
                        if ((unsigned int)v113 <= 2)
                            v113 = AI_MainActor[(uint32_t)v113 - 1];
                        AI_EvalFunc_OBJGetPos_C(v113, &v283);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v283);
                    }
                    v112 = (tGameObjectMain*)v288->o_pod_1;
                    if (v112)
                    {
                        if ((unsigned int)v112 <= 2)
                            v112 = AI_MainActor[(uint32_t)v112 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v112, &v232);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v232);
                    }
                    MATH_VecMultScalar(&v245, &v232, 0.75);
                    MATH_VecMultScalar(&v293, &v245, v167);
                    MATH_SubstractVec(&dest, &v283, &v293);
                    v111 = (tGameObjectMain*)v288->o_pod_1;
                    if (v111)
                    {
                        if ((unsigned int)v111 <= 2)
                            v111 = AI_MainActor[(uint32_t)v111 - 1];
                        AI_EvalFunc_OBJSetPos_C(v111, &dest);
                    }
                    else
                    {
                        AI_EvalFunc_OBJSetPos_C(0, &dest);
                    }
                    v110 = (tGameObjectMain*)v288->o_pod_2;
                    if (v110)
                    {
                        if ((unsigned int)v110 <= 2)
                            v110 = AI_MainActor[(uint32_t)v110 - 1];
                        OBJ_RestoreInitialPos(v110);
                    }
                    else
                    {
                        OBJ_RestoreInitialPos(0);
                    }
                    v109 = (tGameObjectMain*)v288->o_pod_2;
                    if (v109)
                    {
                        if ((unsigned int)v109 <= 2)
                            v109 = AI_MainActor[(uint32_t)v109 - 1];
                        AI_EvalFunc_OBJGetPos_C(v109, &v304);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v304);
                    }
                    v108 = (tGameObjectMain*)v288->o_pod_2;
                    if (v108)
                    {
                        if ((unsigned int)v108 <= 2)
                            v108 = AI_MainActor[(uint32_t)v108 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v108, &v258);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v258);
                    }
                    v146 = v258.x * 0.75f;
                    v147 = v258.y * 0.75f;
                    v148 = v258.z * 0.75f;
                    v181.x = v146 * v167;
                    v181.y = v147 * v167;
                    v181.z = v148 * v167;
                    MATH_SubstractVec(&v277, &v304, &v181);
                    v107 = (tGameObjectMain*)v288->o_pod_2;
                    if (v107)
                    {
                        if ((unsigned int)v107 <= 2)
                            v107 = AI_MainActor[(uint32_t)v107 - 1];
                        AI_EvalFunc_OBJSetPos_C(v107, &v277);
                    }
                    else
                    {
                        AI_EvalFunc_OBJSetPos_C(0, &v277);
                    }
                    v106 = (tGameObjectMain*)v288->o_pod_3;
                    if (v106)
                    {
                        if ((unsigned int)v106 <= 2)
                            v106 = AI_MainActor[(uint32_t)v106 - 1];
                        OBJ_RestoreInitialPos(v106);
                    }
                    else
                    {
                        OBJ_RestoreInitialPos(0);
                    }
                    v105 = (tGameObjectMain*)v288->o_pod_3;
                    if (v105)
                    {
                        if ((unsigned int)v105 <= 2)
                            v105 = AI_MainActor[(uint32_t)v105 - 1];
                        AI_EvalFunc_OBJGetPos_C(v105, &v295);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v295);
                    }
                    v104 = (tGameObjectMain*)v288->o_pod_3;
                    if (v104)
                    {
                        if ((unsigned int)v104 <= 2)
                            v104 = AI_MainActor[(uint32_t)v104 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v104, &v236);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v236);
                    }
                    v211 = v236.x * 0.75f;
                    v212 = v236.y * 0.75f;
                    v213 = v236.z * 0.75f;
                    v262.x = v211 * v167;
                    v262.y = v212 * v167;
                    v262.z = v213 * v167;
                    MATH_SubstractVec(&v186, &v295, &v262);
                    v103 = (tGameObjectMain*)v288->o_pod_3;
                    if (v103)
                    {
                        if ((unsigned int)v103 <= 2)
                            v103 = AI_MainActor[(uint32_t)v103 - 1];
                        AI_EvalFunc_OBJSetPos_C(v103, &v186);
                    }
                    else
                    {
                        AI_EvalFunc_OBJSetPos_C(0, &v186);
                    }
                }
            }
        }
        if (!v288->i_flag_on_ground)
        {
            AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 1, 0);
            if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 64))
            {
                v288->i_flag_on_ground = 1;
                v288->f_time_ground_col = TIM_gf_MainClock - TIM_gf_LockTime;
                v288->f_collision_speed_Z = v288->v_last_speed.z;
                v174 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v288->v_last_speed);
                v248 = AI_EvalFunc_DYNFrictionSet_C(AI_gpst_CurrentGameObject, 3.0);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v133);
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v185);
                v140.x = v185.x * 10.0f;
                v140.y = v185.y * 10.0f;
                v140.z = v185.z * 10.0f;
                MATH_SubstractVec(&v294, &v133, &v140);
                v288->v_grav_pos = v294;
                v288->v_grav_speed.x = 0.0;
                v288->v_grav_speed.y = 0.0;
                v288->v_grav_speed.z = 0.0;
                AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, 1);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v288->v_ground_col_pos);
                v84 = (tGameObjectMain*)v288->o_pod_1;
                if (v84)
                {
                    if ((unsigned int)v84 <= 2)
                        v84 = AI_MainActor[(uint32_t)v84 - 1];
                    AI_EvalFunc_OBJGetPos_C(v84, &v210);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v210);
                }
                v288->f_pod_1_Z = v210.z;
                v83 = (tGameObjectMain*)v288->o_pod_2;
                if (v83)
                {
                    if ((unsigned int)v83 <= 2)
                        v83 = AI_MainActor[(uint32_t)v83 - 1];
                    AI_EvalFunc_OBJGetPos_C(v83, &v134);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v134);
                }
                v288->f_pod_2_Z = v134.z;
                v82 = (tGameObjectMain*)v288->o_pod_3;
                if (v82)
                {
                    if ((unsigned int)v82 <= 2)
                        v82 = AI_MainActor[(uint32_t)v82 - 1];
                    AI_EvalFunc_OBJGetPos_C(v82, &v265);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v265);
                }
                v288->f_pod_3_Z = v265.z;
                AI_EvalFunc_COLPointGet_C(AI_gpst_CurrentGameObject, 64, &v214);
                v288->f_ground_Z = v214.z;
                v256 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(5, -1.0);
                SND_StopRq(v288->i_snd_moteur_vol);
                AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v288->i_snd_moteur_arret);
            }
        }
        if (v288->i_flag_position_ok)
        {
            v169 = TIM_gf_dt;
            v80 = 0.25f * TIM_gf_dt;
            if (v288->f_reacteur_coef > (double)v80)
                v14 = 0.25f * TIM_gf_dt;
            else
                v14 = v288->f_reacteur_coef;
            v142 = v14;
            v288->f_reacteur_coef = v288->f_reacteur_coef - v14;
            v79 = (tGameObjectMain*)v288->o_next_wp;
            if (v79)
            {
                if ((unsigned int)v79 <= 2)
                    v79 = AI_MainActor[(uint32_t)v79 - 1];
                AI_EvalFunc_OBJGetPos_C(v79, &v139);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v139);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v303);
            MATH_SubstractVec(&v151, &v139, &v303);
            v = v151;
        }
        else
        {
            v203.x = v288->v_grav_speed.x;
            v203.y = v288->v_grav_speed.y;
            v203.z = 0.0;
            v81 = (tGameObjectMain*)v288->o_next_wp;
            if (v81)
            {
                if ((unsigned int)v81 <= 2)
                    v81 = AI_MainActor[(uint32_t)v81 - 1];
                AI_EvalFunc_OBJGetPos_C(v81, &v259);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v259);
            }
            v275.x = 0.0;
            v275.y = 0.0;
            v275.z = 16.0;
            MATH_VecAddVec(&v154, &v259, &v275);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v253);
            MATH_SubstractVec(&v230, &v154, &v253);
            v = v230;
            v15 = (float)fabs(v230.z);
            v278 = v15;
            if (v15 < 1.0)
            {
                v250 = MATH_VecLen(&v203);
                if (v250 < 0.25)
                    v288->i_flag_position_ok = 1;
            }
        }
        if (v288->i_flag_on_ground)
        {
            X_4 = v288->des_f_wait_delay - 2.0f;
            if (AI_EvalFunc_TIMEElapsed_C(v288->f_time_ground_col, X_4))
            {
                if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
                {
                    SND_StopRq(v288->i_snd_moteur_arret);
                    AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v288->i_snd_moteur_vol);
                }
                AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
                v159 = TIM_gf_dt;
                v78 = 0.5f * TIM_gf_dt;
                v77 = 1.0f - v288->f_reacteur_coef;
                if (v77 > (double)v78)
                    v13 = 0.5f * TIM_gf_dt;
                else
                    v13 = 1.0f - v288->f_reacteur_coef;
                v168 = v13;
                v288->f_reacteur_coef = v13 + v288->f_reacteur_coef;
            }
            else if (AI_EvalFunc_TIMEElapsed_C(v288->f_time_ground_col, 1.0))
            {
                AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x100, 0);
            }
            if (AI_EvalFunc_TIMEElapsed_C(v288->f_time_ground_col, v288->des_f_wait_delay))
            {
                v144 = AI_EvalFunc_DYNFrictionSet_C(AI_gpst_CurrentGameObject, 2.0);
                v288->i_flag_decollage = 1;
                v288->i_flag_position_ok = 0;
                v288->f_rotation_coef = 0.0;
                v76 = (tGameObjectMain*)v288->o_next_wp;
                if (v76)
                {
                    if ((unsigned int)v76 <= 2)
                        v76 = AI_MainActor[(uint32_t)v76 - 1];
                    v1 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v288->n_net, v76, 0, 0);
                }
                else
                {
                    v1 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v288->n_net, 0, 0, 0);
                }
                v288->o_next_wp = (int)v1;
                v288->f_time_ground_col = TIM_gf_MainClock - TIM_gf_LockTime;
            }
            v209.x = 0.0;
            v209.y = 0.0;
            v209.z = 0.5;
            MATH_SubstractVec(&v300, &v288->v_ground_col_pos, &v209);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v296);
            MATH_SubstractVec(&v173, &v300, &v296);
            v241 = v173;
            v12 = (float)fabs(v288->f_collision_speed_Z * 5.0f);
            v297 = v12;
            v183.x = v241.x * v12;
            v183.y = v241.y * v12;
            v183.z = v241.z * v12;
            AI_EvalFunc_DYNTractionSet_C_CURRENT(&v183);
        }
        else
        {
            AI_EvalFunc_DYNTractionSet_C_CURRENT(&v);
        }
    }
    else
    {
        v288->f_reacteur_coef = 1.0;
        v75 = (tGameObjectMain*)v288->o_next_wp;
        if (v75)
        {
            if ((unsigned int)v75 <= 2)
                v75 = AI_MainActor[(uint32_t)v75 - 1];
            v2 = AI_EvalFunc_COLPivotBVCollide_C(AI_gpst_CurrentGameObject, v75);
        }
        else
        {
            v2 = AI_EvalFunc_COLPivotBVCollide_C(AI_gpst_CurrentGameObject, 0);
        }
        v207 = v2;
        if (v2)
        {
            v74 = (tGameObjectMain*)v288->o_next_wp;
            if (v74)
            {
                if ((unsigned int)v74 <= 2)
                    v74 = AI_MainActor[(uint32_t)v74 - 1];
                v3 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v288->n_net, v74, 0, 0);
            }
            else
            {
                v3 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v288->n_net, 0, 0, 0);
            }
            v288->o_next_wp = (int)v3;
        }
        v73 = (tGameObjectMain*)v288->o_next_wp;
        if (v73)
        {
            if ((unsigned int)v73 <= 2)
                v73 = AI_MainActor[(uint32_t)v73 - 1];
            AI_EvalFunc_OBJGetPos_C(v73, &v261);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v261);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v219);
        MATH_SubstractVec(&v170, &v261, &v219);
        v = v170;
        MATH_NormalizeEqualVector(&v);
        if (v288->i_flag_on_ground)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v305);
            if (v288->v_ground_col_pos.z < (double)v305.z)
            {
                v288->i_flag_on_ground = 0;
                v288->i_flag_pod_activation = 1;
                v288->f_time_pod_activation = TIM_gf_MainClock - TIM_gf_LockTime;
                v221 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
            }
        }
        if (!v288->i_flag_decollage || (v175 = AI_EvalFunc_TIMEElapsed_C(v288->f_time_ground_col, 1.0)))
        {
            v288->i_flag_decollage = 0;
            v71 = v288->des_f_traction;
            v216.x = v.x * v71;
            v216.y = v.y * v71;
            v216.z = v.z * v71;
            AI_EvalFunc_DYNTractionSet_C_CURRENT(&v216);
        }
        else
        {
            v309 = TIM_gf_MainClock - TIM_gf_LockTime;
            v167 = (v309 - v288->f_time_ground_col) / 3.0f;
            v72 = v288->des_f_traction * v167;
            v260.x = v.x * v72;
            v260.y = v.y * v72;
            v260.z = v.z * v72;
            AI_EvalFunc_DYNTractionSet_C_CURRENT(&v260);
        }
        if (v288->i_flag_pod_activation && !v288->des_patrouilleur)
        {
            if (AI_EvalFunc_TIMEElapsed_C(v288->f_time_pod_activation, 1.0))
            {
                v39 = (tGameObjectMain*)v288->o_arceau_1;
                if (v39)
                {
                    if ((unsigned int)v39 <= 2)
                        v39 = AI_MainActor[(uint32_t)v39 - 1];
                    OBJ_RestoreInitialPos(v39);
                }
                else
                {
                    OBJ_RestoreInitialPos(0);
                }
                v38 = (tGameObjectMain*)v288->o_arceau_1;
                if (v38)
                {
                    if ((unsigned int)v38 <= 2)
                        v38 = AI_MainActor[(uint32_t)v38 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v38, &v228);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v228);
                }
                v37 = (tGameObjectMain*)v288->o_arceau_1;
                if (v37)
                {
                    if ((unsigned int)v37 <= 2)
                        v37 = AI_MainActor[(uint32_t)v37 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v37, &v153);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v153);
                }
                v199 = AI_EvalFunc_MTHFloatSign_C(-0.40000001f);
                v36 = (tGameObjectMain*)v288->o_arceau_1;
                if (v36)
                {
                    if ((unsigned int)v36 <= 2)
                        v36 = AI_MainActor[(uint32_t)v36 - 1];
                    AI_EvalFunc_OBJ_HorizonGet_C(v36, &v243);
                }
                else
                {
                    AI_EvalFunc_OBJ_HorizonGet_C(0, &v243);
                }
                v281.x = v243.x * v199;
                v281.y = v243.y * v199;
                v281.z = v243.z * v199;
                v8 = (float)fabs(-0.4000000059604645);
                v161 = v8;
                AI_EvalFunc_MATHVecBlend_C(&v153, &v281, v8, &v197);
                v35 = (tGameObjectMain*)v288->o_arceau_1;
                if (v35)
                {
                    if ((unsigned int)v35 <= 2)
                        v35 = AI_MainActor[(uint32_t)v35 - 1];
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(v35, &v228, &v197);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v228, &v197);
                }
                v34 = (tGameObjectMain*)v288->o_arceau_2;
                if (v34)
                {
                    if ((unsigned int)v34 <= 2)
                        v34 = AI_MainActor[(uint32_t)v34 - 1];
                    OBJ_RestoreInitialPos(v34);
                }
                else
                {
                    OBJ_RestoreInitialPos(0);
                }
                v33 = (tGameObjectMain*)v288->o_arceau_2;
                if (v33)
                {
                    if ((unsigned int)v33 <= 2)
                        v33 = AI_MainActor[(uint32_t)v33 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v33, &v240);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v240);
                }
                v32 = (tGameObjectMain*)v288->o_arceau_2;
                if (v32)
                {
                    if ((unsigned int)v32 <= 2)
                        v32 = AI_MainActor[(uint32_t)v32 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v32, &v285);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v285);
                }
                v162 = AI_EvalFunc_MTHFloatSign_C(-0.40000001f);
                v31 = (tGameObjectMain*)v288->o_arceau_2;
                if (v31)
                {
                    if ((unsigned int)v31 <= 2)
                        v31 = AI_MainActor[(uint32_t)v31 - 1];
                    AI_EvalFunc_OBJ_HorizonGet_C(v31, &v194);
                }
                else
                {
                    AI_EvalFunc_OBJ_HorizonGet_C(0, &v194);
                }
                v279.x = v194.x * v162;
                v279.y = v194.y * v162;
                v279.z = v194.z * v162;
                v7 = (float)fabs(-0.4000000059604645);
                v158 = v7;
                AI_EvalFunc_MATHVecBlend_C(&v285, &v279, v7, &v196);
                v30 = (tGameObjectMain*)v288->o_arceau_2;
                if (v30)
                {
                    if ((unsigned int)v30 <= 2)
                        v30 = AI_MainActor[(uint32_t)v30 - 1];
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(v30, &v240, &v196);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v240, &v196);
                }
                v29 = (tGameObjectMain*)v288->o_arceau_3;
                if (v29)
                {
                    if ((unsigned int)v29 <= 2)
                        v29 = AI_MainActor[(uint32_t)v29 - 1];
                    OBJ_RestoreInitialPos(v29);
                }
                else
                {
                    OBJ_RestoreInitialPos(0);
                }
                v28 = (tGameObjectMain*)v288->o_arceau_3;
                if (v28)
                {
                    if ((unsigned int)v28 <= 2)
                        v28 = AI_MainActor[(uint32_t)v28 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v28, &v238);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v238);
                }
                v27 = (tGameObjectMain*)v288->o_arceau_3;
                if (v27)
                {
                    if ((unsigned int)v27 <= 2)
                        v27 = AI_MainActor[(uint32_t)v27 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v27, &v284);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v284);
                }
                v160 = AI_EvalFunc_MTHFloatSign_C(-0.40000001f);
                v26 = (tGameObjectMain*)v288->o_arceau_3;
                if (v26)
                {
                    if ((unsigned int)v26 <= 2)
                        v26 = AI_MainActor[(uint32_t)v26 - 1];
                    AI_EvalFunc_OBJ_HorizonGet_C(v26, &v193);
                }
                else
                {
                    AI_EvalFunc_OBJ_HorizonGet_C(0, &v193);
                }
                v237.x = v193.x * v160;
                v237.y = v193.y * v160;
                v237.z = v193.z * v160;
                v6 = (float)fabs(-0.4000000059604645);
                v289 = v6;
                AI_EvalFunc_MATHVecBlend_C(&v284, &v237, v6, &v155);
                v25 = (tGameObjectMain*)v288->o_arceau_3;
                if (v25)
                {
                    if ((unsigned int)v25 <= 2)
                        v25 = AI_MainActor[(uint32_t)v25 - 1];
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(v25, &v238, &v155);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v238, &v155);
                }
                v24 = (tGameObjectMain*)v288->o_pod_1;
                if (v24)
                {
                    if ((unsigned int)v24 <= 2)
                        v24 = AI_MainActor[(uint32_t)v24 - 1];
                    OBJ_RestoreInitialPos(v24);
                }
                else
                {
                    OBJ_RestoreInitialPos(0);
                }
                v23 = (tGameObjectMain*)v288->o_pod_2;
                if (v23)
                {
                    if ((unsigned int)v23 <= 2)
                        v23 = AI_MainActor[(uint32_t)v23 - 1];
                    OBJ_RestoreInitialPos(v23);
                }
                else
                {
                    OBJ_RestoreInitialPos(0);
                }
                v22 = (tGameObjectMain*)v288->o_pod_3;
                if (v22)
                {
                    if ((unsigned int)v22 <= 2)
                        v22 = AI_MainActor[(uint32_t)v22 - 1];
                    OBJ_RestoreInitialPos(v22);
                }
                else
                {
                    OBJ_RestoreInitialPos(0);
                }
                v288->i_flag_pod_activation = 0;
                v288->i_flag_pod_out = 0;
            }
            else
            {
                v141 = TIM_gf_MainClock - TIM_gf_LockTime;
                v167 = 1.0f - (v141 - v288->f_time_pod_activation);
                v70 = v167;
                if (v167 < 0.0)
                    v70 = 0.0;
                if (v70 > 1.0)
                    v70 = 1.0;
                v308 = v288->f_arceau_angle * v70 + (1.0f - v70) * -0.40000001f;
                v69 = (tGameObjectMain*)v288->o_arceau_1;
                if (v69)
                {
                    if ((unsigned int)v69 <= 2)
                        v69 = AI_MainActor[(uint32_t)v69 - 1];
                    OBJ_RestoreInitialPos(v69);
                }
                else
                {
                    OBJ_RestoreInitialPos(0);
                }
                v68 = (tGameObjectMain*)v288->o_arceau_1;
                if (v68)
                {
                    if ((unsigned int)v68 <= 2)
                        v68 = AI_MainActor[(uint32_t)v68 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v68, &v223);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v223);
                }
                v67 = (tGameObjectMain*)v288->o_arceau_1;
                if (v67)
                {
                    if ((unsigned int)v67 <= 2)
                        v67 = AI_MainActor[(uint32_t)v67 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v67, &v180);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v180);
                }
                v143 = AI_EvalFunc_MTHFloatSign_C(v308);
                v66 = (tGameObjectMain*)v288->o_arceau_1;
                if (v66)
                {
                    if ((unsigned int)v66 <= 2)
                        v66 = AI_MainActor[(uint32_t)v66 - 1];
                    AI_EvalFunc_OBJ_HorizonGet_C(v66, &v272);
                }
                else
                {
                    AI_EvalFunc_OBJ_HorizonGet_C(0, &v272);
                }
                v222.x = v272.x * v143;
                v222.y = v272.y * v143;
                v222.z = v272.z * v143;
                v11 = (float)fabs(v308);
                v188 = v11;
                AI_EvalFunc_MATHVecBlend_C(&v180, &v222, v11, &v135);
                v65 = (tGameObjectMain*)v288->o_arceau_1;
                if (v65)
                {
                    if ((unsigned int)v65 <= 2)
                        v65 = AI_MainActor[(uint32_t)v65 - 1];
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(v65, &v223, &v135);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v223, &v135);
                }
                v64 = (tGameObjectMain*)v288->o_arceau_2;
                if (v64)
                {
                    if ((unsigned int)v64 <= 2)
                        v64 = AI_MainActor[(uint32_t)v64 - 1];
                    OBJ_RestoreInitialPos(v64);
                }
                else
                {
                    OBJ_RestoreInitialPos(0);
                }
                v63 = (tGameObjectMain*)v288->o_arceau_2;
                if (v63)
                {
                    if ((unsigned int)v63 <= 2)
                        v63 = AI_MainActor[(uint32_t)v63 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v63, &v156);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v156);
                }
                v62 = (tGameObjectMain*)v288->o_arceau_2;
                if (v62)
                {
                    if ((unsigned int)v62 <= 2)
                        v62 = AI_MainActor[(uint32_t)v62 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v62, &v282);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v282);
                }
                v251 = AI_EvalFunc_MTHFloatSign_C(v308);
                v61 = (tGameObjectMain*)v288->o_arceau_2;
                if (v61)
                {
                    if ((unsigned int)v61 <= 2)
                        v61 = AI_MainActor[(uint32_t)v61 - 1];
                    AI_EvalFunc_OBJ_HorizonGet_C(v61, &v192);
                }
                else
                {
                    AI_EvalFunc_OBJ_HorizonGet_C(0, &v192);
                }
                v157.x = v192.x * v251;
                v157.y = v192.y * v251;
                v157.z = v192.z * v251;
                v10 = (float)fabs(v308);
                v290 = v10;
                AI_EvalFunc_MATHVecBlend_C(&v282, &v157, v10, &v244);
                v60 = (tGameObjectMain*)v288->o_arceau_2;
                if (v60)
                {
                    if ((unsigned int)v60 <= 2)
                        v60 = AI_MainActor[(uint32_t)v60 - 1];
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(v60, &v156, &v244);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v156, &v244);
                }
                v59 = (tGameObjectMain*)v288->o_arceau_3;
                if (v59)
                {
                    if ((unsigned int)v59 <= 2)
                        v59 = AI_MainActor[(uint32_t)v59 - 1];
                    OBJ_RestoreInitialPos(v59);
                }
                else
                {
                    OBJ_RestoreInitialPos(0);
                }
                v58 = (tGameObjectMain*)v288->o_arceau_3;
                if (v58)
                {
                    if ((unsigned int)v58 <= 2)
                        v58 = AI_MainActor[(uint32_t)v58 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v58, &v198);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v198);
                }
                v57 = (tGameObjectMain*)v288->o_arceau_3;
                if (v57)
                {
                    if ((unsigned int)v57 <= 2)
                        v57 = AI_MainActor[(uint32_t)v57 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v57, &v149);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v149);
                }
                v291 = AI_EvalFunc_MTHFloatSign_C(v308);
                v56 = (tGameObjectMain*)v288->o_arceau_3;
                if (v56)
                {
                    if ((unsigned int)v56 <= 2)
                        v56 = AI_MainActor[(uint32_t)v56 - 1];
                    AI_EvalFunc_OBJ_HorizonGet_C(v56, &v280);
                }
                else
                {
                    AI_EvalFunc_OBJ_HorizonGet_C(0, &v280);
                }
                v247.x = v280.x * v291;
                v247.y = v280.y * v291;
                v247.z = v280.z * v291;
                v9 = (float)fabs(v308);
                v201 = v9;
                AI_EvalFunc_MATHVecBlend_C(&v149, &v247, v9, &v150);
                v55 = (tGameObjectMain*)v288->o_arceau_3;
                if (v55)
                {
                    if ((unsigned int)v55 <= 2)
                        v55 = AI_MainActor[(uint32_t)v55 - 1];
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(v55, &v198, &v150);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v198, &v150);
                }
                v54 = (tGameObjectMain*)v288->o_pod_1;
                if (v54)
                {
                    if ((unsigned int)v54 <= 2)
                        v54 = AI_MainActor[(uint32_t)v54 - 1];
                    OBJ_RestoreInitialPos(v54);
                }
                else
                {
                    OBJ_RestoreInitialPos(0);
                }
                v53 = (tGameObjectMain*)v288->o_pod_1;
                if (v53)
                {
                    if ((unsigned int)v53 <= 2)
                        v53 = AI_MainActor[(uint32_t)v53 - 1];
                    AI_EvalFunc_OBJGetPos_C(v53, &v287);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v287);
                }
                v52 = (tGameObjectMain*)v288->o_pod_1;
                if (v52)
                {
                    if ((unsigned int)v52 <= 2)
                        v52 = AI_MainActor[(uint32_t)v52 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v52, &v242);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v242);
                }
                v51 = v288->f_pod_1_range;
                v189 = v242.x * v51;
                v190 = v242.y * v51;
                v191 = v242.z * v51;
                v152.x = v189 * v167;
                v152.y = v190 * v167;
                v152.z = v191 * v167;
                MATH_SubstractVec(&v286, &v287, &v152);
                v50 = (tGameObjectMain*)v288->o_pod_1;
                if (v50)
                {
                    if ((unsigned int)v50 <= 2)
                        v50 = AI_MainActor[(uint32_t)v50 - 1];
                    AI_EvalFunc_OBJSetPos_C(v50, &v286);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &v286);
                }
                v49 = (tGameObjectMain*)v288->o_pod_2;
                if (v49)
                {
                    if ((unsigned int)v49 <= 2)
                        v49 = AI_MainActor[(uint32_t)v49 - 1];
                    OBJ_RestoreInitialPos(v49);
                }
                else
                {
                    OBJ_RestoreInitialPos(0);
                }
                v48 = (tGameObjectMain*)v288->o_pod_2;
                if (v48)
                {
                    if ((unsigned int)v48 <= 2)
                        v48 = AI_MainActor[(uint32_t)v48 - 1];
                    AI_EvalFunc_OBJGetPos_C(v48, &v235);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v235);
                }
                v47 = (tGameObjectMain*)v288->o_pod_2;
                if (v47)
                {
                    if ((unsigned int)v47 <= 2)
                        v47 = AI_MainActor[(uint32_t)v47 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v47, &v179);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v179);
                }
                v46 = v288->f_pod_2_range;
                v225 = v179.x * v46;
                v226 = v179.y * v46;
                v227 = v179.z * v46;
                v274.x = v225 * v167;
                v274.y = v226 * v167;
                v274.z = v227 * v167;
                MATH_SubstractVec(&v138, &v235, &v274);
                v45 = (tGameObjectMain*)v288->o_pod_2;
                if (v45)
                {
                    if ((unsigned int)v45 <= 2)
                        v45 = AI_MainActor[(uint32_t)v45 - 1];
                    AI_EvalFunc_OBJSetPos_C(v45, &v138);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &v138);
                }
                v44 = (tGameObjectMain*)v288->o_pod_3;
                if (v44)
                {
                    if ((unsigned int)v44 <= 2)
                        v44 = AI_MainActor[(uint32_t)v44 - 1];
                    OBJ_RestoreInitialPos(v44);
                }
                else
                {
                    OBJ_RestoreInitialPos(0);
                }
                v43 = (tGameObjectMain*)v288->o_pod_3;
                if (v43)
                {
                    if ((unsigned int)v43 <= 2)
                        v43 = AI_MainActor[(uint32_t)v43 - 1];
                    AI_EvalFunc_OBJGetPos_C(v43, &v176);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v176);
                }
                v42 = (tGameObjectMain*)v288->o_pod_3;
                if (v42)
                {
                    if ((unsigned int)v42 <= 2)
                        v42 = AI_MainActor[(uint32_t)v42 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v42, &v231);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v231);
                }
                v41 = v288->f_pod_3_range;
                v268 = v231.x * v41;
                v269 = v231.y * v41;
                v270 = v231.z * v41;
                v137.x = v268 * v167;
                v137.y = v269 * v167;
                v137.z = v270 * v167;
                MATH_SubstractVec(&v177, &v176, &v137);
                v40 = (tGameObjectMain*)v288->o_pod_3;
                if (v40)
                {
                    if ((unsigned int)v40 <= 2)
                        v40 = AI_MainActor[(uint32_t)v40 - 1];
                    AI_EvalFunc_OBJSetPos_C(v40, &v177);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &v177);
                }
            }
        }
    }
    v206 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Vaisseau_Transport_Militaire_VTM_mode_deplacement_Vaisseau_Transport_Militaire_VTM_exec_s_b);
    if (v206 != 1000)
        return v206;
    v20 = &v288->v_last_speed;
    v21 = &v288->v_speed;
    v288->v_last_speed.x = v288->v_speed.x;
    v20->y = v21->y;
    v20->z = v21->z;
    return 1000;
}

int Vaisseau_Transport_Militaire_VTM_exec_afterrec()
{
    int result; // eax
    float X_4; // [esp+4h] [ebp-6BCh]
    float v2; // [esp+8h] [ebp-6B8h]
    float v3; // [esp+Ch] [ebp-6B4h]
    float v4; // [esp+10h] [ebp-6B0h]
    float v5; // [esp+14h] [ebp-6ACh]
    float v6; // [esp+18h] [ebp-6A8h]
    float v7; // [esp+1Ch] [ebp-6A4h]
    float v8; // [esp+20h] [ebp-6A0h]
    float v9; // [esp+24h] [ebp-69Ch]
    float v10; // [esp+28h] [ebp-698h]
    tScriptStruct_PNJVaisseauTransportMilitaire* v11; // [esp+2Ch] [ebp-694h]
    tGameObjectMain* v12; // [esp+54h] [ebp-66Ch]
    tGameObjectMain* v13; // [esp+64h] [ebp-65Ch]
    tGameObjectMain* v14; // [esp+6Ch] [ebp-654h]
    tGameObjectMain* v15; // [esp+7Ch] [ebp-644h]
    tGameObjectMain* v16; // [esp+84h] [ebp-63Ch]
    tGameObjectMain* v17; // [esp+94h] [ebp-62Ch]
    tGameObjectMain* v18; // [esp+9Ch] [ebp-624h]
    tGameObjectMain* v19; // [esp+A4h] [ebp-61Ch]
    tGameObjectMain* v20; // [esp+BCh] [ebp-604h]
    tGameObjectMain* v21; // [esp+CCh] [ebp-5F4h]
    tGameObjectMain* v22; // [esp+D4h] [ebp-5ECh]
    tGameObjectMain* v23; // [esp+ECh] [ebp-5D4h]
    tGameObjectMain* v24; // [esp+F4h] [ebp-5CCh]
    tGameObjectMain* v25; // [esp+FCh] [ebp-5C4h]
    tGameObjectMain* v26; // [esp+10Ch] [ebp-5B4h]
    tGameObjectMain* v27; // [esp+114h] [ebp-5ACh]
    tGameObjectMain* v28; // [esp+11Ch] [ebp-5A4h]
    float v29; // [esp+120h] [ebp-5A0h]
    tGameObjectMain* v30; // [esp+130h] [ebp-590h]
    tGameObjectMain* v31; // [esp+138h] [ebp-588h]
    float v32; // [esp+13Ch] [ebp-584h]
    tGameObjectMain* v33; // [esp+144h] [ebp-57Ch]
    float v34; // [esp+148h] [ebp-578h]
    tGameObjectMain* v35; // [esp+158h] [ebp-568h]
    tGameObjectMain* v36; // [esp+160h] [ebp-560h]
    float v37; // [esp+164h] [ebp-55Ch]
    tGameObjectMain* v38; // [esp+16Ch] [ebp-554h]
    float v39; // [esp+170h] [ebp-550h]
    tGameObjectMain* v40; // [esp+178h] [ebp-548h]
    tGameObjectMain* v41; // [esp+180h] [ebp-540h]
    tGameObjectMain* v42; // [esp+190h] [ebp-530h]
    tGameObjectMain* v43; // [esp+198h] [ebp-528h]
    tGameObjectMain* v44; // [esp+1A0h] [ebp-520h]
    float v45; // [esp+1A4h] [ebp-51Ch]
    tGameObjectMain* v46; // [esp+1ACh] [ebp-514h]
    tGameObjectMain* v47; // [esp+1B4h] [ebp-50Ch]
    tGameObjectMain* v48; // [esp+1C4h] [ebp-4FCh]
    tGameObjectMain* v49; // [esp+1CCh] [ebp-4F4h]
    tGameObjectMain* v50; // [esp+1D4h] [ebp-4ECh]
    float v51; // [esp+1D8h] [ebp-4E8h]
    tGameObjectMain* v52; // [esp+1E0h] [ebp-4E0h]
    tGameObjectMain* v53; // [esp+1E8h] [ebp-4D8h]
    tGameObjectMain* v54; // [esp+1F8h] [ebp-4C8h]
    tGameObjectMain* v55; // [esp+200h] [ebp-4C0h]
    tGameObjectMain* v56; // [esp+208h] [ebp-4B8h]
    tGameObjectMain* v57; // [esp+210h] [ebp-4B0h]
    tGameObjectMain* v58; // [esp+218h] [ebp-4A8h]
    tGameObjectMain* v59; // [esp+220h] [ebp-4A0h]
    tGameObjectMain* v60; // [esp+228h] [ebp-498h]
    tGameObjectMain* v61; // [esp+230h] [ebp-490h]
    tGameObjectMain* v62; // [esp+238h] [ebp-488h]
    tGameObjectMain* v63; // [esp+240h] [ebp-480h]
    tGameObjectMain* v64; // [esp+248h] [ebp-478h]
    tGameObjectMain* v65; // [esp+250h] [ebp-470h]
    tGameObjectMain* v66; // [esp+258h] [ebp-468h]
    tGameObjectMain* v67; // [esp+260h] [ebp-460h]
    tGameObjectMain* v68; // [esp+268h] [ebp-458h]
    tGameObjectMain* v69; // [esp+270h] [ebp-450h]
    tGameObjectMain* v70; // [esp+278h] [ebp-448h]
    float v71; // [esp+27Ch] [ebp-444h]
    Vec3 v72; // [esp+288h] [ebp-438h] BYREF
    float v73; // [esp+294h] [ebp-42Ch]
    float v74; // [esp+298h] [ebp-428h]
    float v75; // [esp+29Ch] [ebp-424h]
    Vec3 v76; // [esp+2A0h] [ebp-420h] BYREF
    Vec3 v77; // [esp+2ACh] [ebp-414h] BYREF
    Vec3 v78; // [esp+2B8h] [ebp-408h] BYREF
    Vec3 v79; // [esp+2C4h] [ebp-3FCh] BYREF
    int v80; // [esp+2DCh] [ebp-3E4h]
    Vec3 v81; // [esp+2E0h] [ebp-3E0h] BYREF
    Vec3 v82; // [esp+2ECh] [ebp-3D4h] BYREF
    Vec3 v83; // [esp+2F8h] [ebp-3C8h] BYREF
    Vec3 v84; // [esp+304h] [ebp-3BCh] BYREF
    Vec3 v85; // [esp+310h] [ebp-3B0h] BYREF
    float v86; // [esp+31Ch] [ebp-3A4h]
    Vec3 v87; // [esp+324h] [ebp-39Ch] BYREF
    Vec3 v88; // [esp+330h] [ebp-390h] BYREF
    Vec3 v89; // [esp+33Ch] [ebp-384h] BYREF
    float v90; // [esp+348h] [ebp-378h]
    float v91; // [esp+34Ch] [ebp-374h]
    Vec3 v92; // [esp+350h] [ebp-370h] BYREF
    Vec3 v93; // [esp+35Ch] [ebp-364h] BYREF
    Vec3 v94; // [esp+368h] [ebp-358h] BYREF
    Vec3 v95; // [esp+374h] [ebp-34Ch] BYREF
    float v96; // [esp+380h] [ebp-340h]
    float v98; // [esp+388h] [ebp-338h]
    Vec3 v99; // [esp+38Ch] [ebp-334h] BYREF
    Vec3 v100; // [esp+398h] [ebp-328h] BYREF
    Vec3 v101; // [esp+3A4h] [ebp-31Ch] BYREF
    int v102; // [esp+3B0h] [ebp-310h]
    int v103; // [esp+3B4h] [ebp-30Ch]
    float v104; // [esp+3B8h] [ebp-308h]
    float v105; // [esp+3BCh] [ebp-304h]
    float v106; // [esp+3C0h] [ebp-300h]
    int i; // [esp+3C4h] [ebp-2FCh]
    Vec3 v110; // [esp+3D0h] [ebp-2F0h] BYREF
    Vec3 v111; // [esp+3DCh] [ebp-2E4h] BYREF
    Vec3 a4; // [esp+3E8h] [ebp-2D8h] BYREF
    float v113; // [esp+3F4h] [ebp-2CCh]
    float v114; // [esp+3F8h] [ebp-2C8h]
    Vec3 v115; // [esp+3FCh] [ebp-2C4h] BYREF
    Vec3 v116; // [esp+408h] [ebp-2B8h] BYREF
    Vec3 v117; // [esp+414h] [ebp-2ACh] BYREF
    Vec3 v118; // [esp+420h] [ebp-2A0h] BYREF
    float v119; // [esp+42Ch] [ebp-294h]
    float v120; // [esp+430h] [ebp-290h]
    float v121; // [esp+434h] [ebp-28Ch]
    Vec3 v122; // [esp+438h] [ebp-288h] BYREF
    Vec3 v123; // [esp+444h] [ebp-27Ch] BYREF
    Vec3 v124; // [esp+450h] [ebp-270h] BYREF
    Vec3 v125; // [esp+45Ch] [ebp-264h] BYREF
    Vec3 v126; // [esp+468h] [ebp-258h] BYREF
    Vec3 v127; // [esp+474h] [ebp-24Ch] BYREF
    float v128; // [esp+480h] [ebp-240h]
    float v129; // [esp+484h] [ebp-23Ch]
    float v130; // [esp+48Ch] [ebp-234h]
    Vec3 v131; // [esp+490h] [ebp-230h] BYREF
    Vec3 v132; // [esp+49Ch] [ebp-224h] BYREF
    Vec3 v133; // [esp+4A8h] [ebp-218h] BYREF
    Vec3 v134; // [esp+4B4h] [ebp-20Ch] BYREF
    Vec3 v135; // [esp+4C0h] [ebp-200h] BYREF
    Vec3 v136; // [esp+4CCh] [ebp-1F4h]
    float v137; // [esp+4D8h] [ebp-1E8h]
    int v138; // [esp+4DCh] [ebp-1E4h]
    Vec3 v139; // [esp+4E0h] [ebp-1E0h] BYREF
    Vec3 v140; // [esp+4ECh] [ebp-1D4h] BYREF
    Vec3 v141; // [esp+4F8h] [ebp-1C8h] BYREF
    Vec3 v142; // [esp+504h] [ebp-1BCh] BYREF
    Vec3 v143; // [esp+510h] [ebp-1B0h] BYREF
    Vec3 v144; // [esp+51Ch] [ebp-1A4h] BYREF
    Vec3 v145; // [esp+528h] [ebp-198h] BYREF
    float v146; // [esp+534h] [ebp-18Ch]
    float v147; // [esp+538h] [ebp-188h]
    Vec3 v148; // [esp+53Ch] [ebp-184h] BYREF
    Vec3 a1; // [esp+548h] [ebp-178h] BYREF
    Vec3 v150; // [esp+554h] [ebp-16Ch] BYREF
    Vec3 v151; // [esp+560h] [ebp-160h] BYREF
    Vec3 v152; // [esp+56Ch] [ebp-154h] BYREF
    int v153; // [esp+57Ch] [ebp-144h]
    int v154; // [esp+580h] [ebp-140h]
    float v155; // [esp+584h] [ebp-13Ch]
    Vec3 v156; // [esp+588h] [ebp-138h] BYREF
    Vec3 v157; // [esp+594h] [ebp-12Ch] BYREF
    int v158; // [esp+5A0h] [ebp-120h]
    int v159; // [esp+5A4h] [ebp-11Ch]
    int v160; // [esp+5A8h] [ebp-118h]
    Vec3 v161; // [esp+5ACh] [ebp-114h] BYREF
    Vec3 a3; // [esp+5B8h] [ebp-108h] BYREF
    Vec3 v163; // [esp+5C4h] [ebp-FCh] BYREF
    float v164; // [esp+5D0h] [ebp-F0h]
    Vec3 v165; // [esp+5D8h] [ebp-E8h] BYREF
    Vec3 v166; // [esp+5E4h] [ebp-DCh] BYREF
    Vec3 dest; // [esp+5F0h] [ebp-D0h] BYREF
    Vec3 v168; // [esp+5FCh] [ebp-C4h] BYREF
    Vec3 v169; // [esp+608h] [ebp-B8h] BYREF
    Vec3 v170; // [esp+614h] [ebp-ACh] BYREF
    tScriptStruct_PNJVaisseauTransportMilitaire* v171; // [esp+624h] [ebp-9Ch]
    float v172; // [esp+628h] [ebp-98h]
    Vec3 v173; // [esp+62Ch] [ebp-94h] BYREF
    Vec3 v174; // [esp+638h] [ebp-88h] BYREF
    int v175; // [esp+644h] [ebp-7Ch]
    int v176; // [esp+648h] [ebp-78h]
    float v177; // [esp+64Ch] [ebp-74h]
    Vec3 v178; // [esp+650h] [ebp-70h] BYREF
    Vec3 v179; // [esp+65Ch] [ebp-64h] BYREF
    float v180; // [esp+66Ch] [ebp-54h]
    float v181; // [esp+670h] [ebp-50h]
    Vec3 v182; // [esp+674h] [ebp-4Ch] BYREF
    Vec3 v183; // [esp+680h] [ebp-40h] BYREF
    Vec3 v184; // [esp+68Ch] [ebp-34h] BYREF
    Vec3 v185; // [esp+698h] [ebp-28h] BYREF
    Vec3 v186; // [esp+6A4h] [ebp-1Ch] BYREF
    float v188; // [esp+6B4h] [ebp-Ch]
    float v189; // [esp+6B8h] [ebp-8h]
    int v190; // [esp+6BCh] [ebp-4h]

    v141.x = 1.0;
    v141.y = 0.0;
    v141.z = 0.0;
    v117.x = 1.0;
    v117.y = 0.0;
    v117.z = 0.0;
    v89.x = 1.0;
    v89.y = 0.0;
    v89.z = 0.0;
    v136.x = 1.0;
    v136.y = 0.0;
    v136.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v11 = (tScriptStruct_PNJVaisseauTransportMilitaire*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v11 = 0;
    v171 = v11;
    v113 = 0.0;
    v119 = 0.0;
    i = 0;
    v114 = 0.0;
    v121 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Vaisseau_Transport_Militaire_VTM_exec_afterrec_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        fct_Vaisseau_Transport_Militaire_VTM_exec_afterrec_Vaisseau_Transport_Militaire_VTM_exec_GFX = (int)AI_ul_RealResolveOneRefFunction(0x72000B55u);
    }
    if (v171->i_flag_on_ground)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v111);
        MATH_SubstractVec(&dest, &v171->v_ground_col_pos, &v111);
        v136 = dest;
        v71 = dest.z / 2.0f;
        if (v71 <= 1.0)
        {
            if (v71 >= 0.0)
                v113 = dest.z / 2.0f;
            else
                v113 = 0.0;
        }
        else
        {
            v113 = 1.0;
        }
        v10 = (float)fabs(v171->f_vert_coef - v113);
        v91 = v10;
        if (v10 > 0.0099999998)
        {
            v171->f_vert_coef = v113;
            v171->f_arceau_angle = v113 * 0.40000001f;
            v70 = (tGameObjectMain*)v171->o_arceau_1;
            if (v70)
            {
                if ((unsigned int)v70 <= 2)
                    v70 = AI_MainActor[(uint32_t)v70 - 1];
                OBJ_RestoreInitialPos(v70);
            }
            else
            {
                OBJ_RestoreInitialPos(0);
            }
            v69 = (tGameObjectMain*)v171->o_arceau_1;
            if (v69)
            {
                if ((unsigned int)v69 <= 2)
                    v69 = AI_MainActor[(uint32_t)v69 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v69, &v85);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v85);
            }
            v68 = (tGameObjectMain*)v171->o_arceau_1;
            if (v68)
            {
                if ((unsigned int)v68 <= 2)
                    v68 = AI_MainActor[(uint32_t)v68 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v68, &v132);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v132);
            }
            v180 = AI_EvalFunc_MTHFloatSign_C(v171->f_arceau_angle);
            v67 = (tGameObjectMain*)v171->o_arceau_1;
            if (v67)
            {
                if ((unsigned int)v67 <= 2)
                    v67 = AI_MainActor[(uint32_t)v67 - 1];
                AI_EvalFunc_OBJ_HorizonGet_C(v67, &v101);
            }
            else
            {
                AI_EvalFunc_OBJ_HorizonGet_C(0, &v101);
            }
            MATH_VecMultScalar(&a1, &v101, v180);
            v9 = (float)fabs(v171->f_arceau_angle);
            v181 = v9;
            AI_EvalFunc_MATHVecBlend_C(&v132, &a1, v9, &a4);
            v66 = (tGameObjectMain*)v171->o_arceau_1;
            if (v66)
            {
                if ((unsigned int)v66 <= 2)
                    v66 = AI_MainActor[(uint32_t)v66 - 1];
                AI_EvalFunc_OBJ_BankingGeneralSet_C(v66, &v85, &a4);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v85, &a4);
            }
            v65 = (tGameObjectMain*)v171->o_arceau_2;
            if (v65)
            {
                if ((unsigned int)v65 <= 2)
                    v65 = AI_MainActor[(uint32_t)v65 - 1];
                OBJ_RestoreInitialPos(v65);
            }
            else
            {
                OBJ_RestoreInitialPos(0);
            }
            v64 = (tGameObjectMain*)v171->o_arceau_2;
            if (v64)
            {
                if ((unsigned int)v64 <= 2)
                    v64 = AI_MainActor[(uint32_t)v64 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v64, &v145);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v145);
            }
            v63 = (tGameObjectMain*)v171->o_arceau_2;
            if (v63)
            {
                if ((unsigned int)v63 <= 2)
                    v63 = AI_MainActor[(uint32_t)v63 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v63, &v92);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v92);
            }
            v189 = AI_EvalFunc_MTHFloatSign_C(v171->f_arceau_angle);
            v62 = (tGameObjectMain*)v171->o_arceau_2;
            if (v62)
            {
                if ((unsigned int)v62 <= 2)
                    v62 = AI_MainActor[(uint32_t)v62 - 1];
                AI_EvalFunc_OBJ_HorizonGet_C(v62, &v139);
            }
            else
            {
                AI_EvalFunc_OBJ_HorizonGet_C(0, &v139);
            }
            v110.x = v139.x * v189;
            v110.y = v139.y * v189;
            v110.z = v139.z * v189;
            v8 = (float)fabs(v171->f_arceau_angle);
            v130 = v8;
            AI_EvalFunc_MATHVecBlend_C(&v92, &v110, v8, &v83);
            v61 = (tGameObjectMain*)v171->o_arceau_2;
            if (v61)
            {
                if ((unsigned int)v61 <= 2)
                    v61 = AI_MainActor[(uint32_t)v61 - 1];
                AI_EvalFunc_OBJ_BankingGeneralSet_C(v61, &v145, &v83);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v145, &v83);
            }
            v60 = (tGameObjectMain*)v171->o_arceau_3;
            if (v60)
            {
                if ((unsigned int)v60 <= 2)
                    v60 = AI_MainActor[(uint32_t)v60 - 1];
                OBJ_RestoreInitialPos(v60);
            }
            else
            {
                OBJ_RestoreInitialPos(0);
            }
            v59 = (tGameObjectMain*)v171->o_arceau_3;
            if (v59)
            {
                if ((unsigned int)v59 <= 2)
                    v59 = AI_MainActor[(uint32_t)v59 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v59, &v126);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v126);
            }
            v58 = (tGameObjectMain*)v171->o_arceau_3;
            if (v58)
            {
                if ((unsigned int)v58 <= 2)
                    v58 = AI_MainActor[(uint32_t)v58 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v58, &v157);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v157);
            }
            v146 = AI_EvalFunc_MTHFloatSign_C(v171->f_arceau_angle);
            v57 = (tGameObjectMain*)v171->o_arceau_3;
            if (v57)
            {
                if ((unsigned int)v57 <= 2)
                    v57 = AI_MainActor[(uint32_t)v57 - 1];
                AI_EvalFunc_OBJ_HorizonGet_C(v57, &v182);
            }
            else
            {
                AI_EvalFunc_OBJ_HorizonGet_C(0, &v182);
            }
            v148.x = v182.x * v146;
            v148.y = v182.y * v146;
            v148.z = v182.z * v146;
            v7 = (float)fabs(v171->f_arceau_angle);
            v120 = v7;
            AI_EvalFunc_MATHVecBlend_C(&v157, &v148, v7, &v115);
            v56 = (tGameObjectMain*)v171->o_arceau_3;
            if (v56)
            {
                if ((unsigned int)v56 <= 2)
                    v56 = AI_MainActor[(uint32_t)v56 - 1];
                AI_EvalFunc_OBJ_BankingGeneralSet_C(v56, &v126, &v115);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v126, &v115);
            }
            v55 = (tGameObjectMain*)v171->o_pod_1;
            if (v55)
            {
                if ((unsigned int)v55 <= 2)
                    v55 = AI_MainActor[(uint32_t)v55 - 1];
                OBJ_RestoreInitialPos(v55);
            }
            else
            {
                OBJ_RestoreInitialPos(0);
            }
            v54 = (tGameObjectMain*)v171->o_pod_1;
            if (v54)
            {
                if ((unsigned int)v54 <= 2)
                    v54 = AI_MainActor[(uint32_t)v54 - 1];
                AI_EvalFunc_OBJGetPos_C(v54, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            v89 = a3;
            v53 = (tGameObjectMain*)v171->o_pod_1;
            if (v53)
            {
                if ((unsigned int)v53 <= 2)
                    v53 = AI_MainActor[(uint32_t)v53 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v53, &v88);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v88);
            }
            v175 = 0;
            v176 = 0;
            v177 = v89.z - v171->f_pod_1_Z;
            v137 = v177 * v88.z + 0.0f * v88.y + v88.x * 0.0f;
            if (v137 > 0.75)
                v6 = 0.75;
            else
                v6 = v137;
            v172 = v6;
            v171->f_pod_1_range = v6;
            v52 = (tGameObjectMain*)v171->o_pod_1;
            if (v52)
            {
                if ((unsigned int)v52 <= 2)
                    v52 = AI_MainActor[(uint32_t)v52 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v52, &v135);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v135);
            }
            v51 = v171->f_pod_1_range;
            v144.x = v135.x * v51;
            v144.y = v135.y * v51;
            v144.z = v135.z * v51;
            MATH_SubstractVec(&v174, &v89, &v144);
            v50 = (tGameObjectMain*)v171->o_pod_1;
            if (v50)
            {
                if ((unsigned int)v50 <= 2)
                    v50 = AI_MainActor[(uint32_t)v50 - 1];
                AI_EvalFunc_OBJSetPos_C(v50, &v174);
            }
            else
            {
                AI_EvalFunc_OBJSetPos_C(0, &v174);
            }
            v49 = (tGameObjectMain*)v171->o_pod_2;
            if (v49)
            {
                if ((unsigned int)v49 <= 2)
                    v49 = AI_MainActor[(uint32_t)v49 - 1];
                OBJ_RestoreInitialPos(v49);
            }
            else
            {
                OBJ_RestoreInitialPos(0);
            }
            v48 = (tGameObjectMain*)v171->o_pod_2;
            if (v48)
            {
                if ((unsigned int)v48 <= 2)
                    v48 = AI_MainActor[(uint32_t)v48 - 1];
                AI_EvalFunc_OBJGetPos_C(v48, &v99);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v99);
            }
            v89 = v99;
            v47 = (tGameObjectMain*)v171->o_pod_2;
            if (v47)
            {
                if ((unsigned int)v47 <= 2)
                    v47 = AI_MainActor[(uint32_t)v47 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v47, &v184);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v184);
            }
            v153 = 0;
            v154 = 0;
            v155 = v89.z - v171->f_pod_2_Z;
            v86 = v155 * v184.z + 0.0f * v184.y + v184.x * 0.0f;
            if (v86 > 0.75)
                v5 = 0.75;
            else
                v5 = v86;
            v106 = v5;
            v171->f_pod_2_range = v5;
            v46 = (tGameObjectMain*)v171->o_pod_2;
            if (v46)
            {
                if ((unsigned int)v46 <= 2)
                    v46 = AI_MainActor[(uint32_t)v46 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v46, &v168);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v168);
            }
            v45 = v171->f_pod_2_range;
            v173.x = v168.x * v45;
            v173.y = v168.y * v45;
            v173.z = v168.z * v45;
            MATH_SubstractVec(&v143, &v89, &v173);
            v44 = (tGameObjectMain*)v171->o_pod_2;
            if (v44)
            {
                if ((unsigned int)v44 <= 2)
                    v44 = AI_MainActor[(uint32_t)v44 - 1];
                AI_EvalFunc_OBJSetPos_C(v44, &v143);
            }
            else
            {
                AI_EvalFunc_OBJSetPos_C(0, &v143);
            }
            v43 = (tGameObjectMain*)v171->o_pod_3;
            if (v43)
            {
                if ((unsigned int)v43 <= 2)
                    v43 = AI_MainActor[(uint32_t)v43 - 1];
                OBJ_RestoreInitialPos(v43);
            }
            else
            {
                OBJ_RestoreInitialPos(0);
            }
            v42 = (tGameObjectMain*)v171->o_pod_3;
            if (v42)
            {
                if ((unsigned int)v42 <= 2)
                    v42 = AI_MainActor[(uint32_t)v42 - 1];
                AI_EvalFunc_OBJGetPos_C(v42, &v125);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v125);
            }
            v89 = v125;
            v41 = (tGameObjectMain*)v171->o_pod_3;
            if (v41)
            {
                if ((unsigned int)v41 <= 2)
                    v41 = AI_MainActor[(uint32_t)v41 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v41, &v163);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v163);
            }
            v102 = 0;
            v103 = 0;
            v104 = v89.z - v171->f_pod_3_Z;
            v129 = v104 * v163.z + 0.0f * v163.y + v163.x * 0.0f;
            if (v129 > 0.75)
                v4 = 0.75;
            else
                v4 = v129;
            v188 = v4;
            v171->f_pod_3_range = v4;
            v40 = (tGameObjectMain*)v171->o_pod_3;
            if (v40)
            {
                if ((unsigned int)v40 <= 2)
                    v40 = AI_MainActor[(uint32_t)v40 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v40, &v169);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v169);
            }
            v39 = v171->f_pod_3_range;
            v95.x = v169.x * v39;
            v95.y = v169.y * v39;
            v95.z = v169.z * v39;
            MATH_SubstractVec(&v165, &v89, &v95);
            v38 = (tGameObjectMain*)v171->o_pod_3;
            if (v38)
            {
                if ((unsigned int)v38 <= 2)
                    v38 = AI_MainActor[(uint32_t)v38 - 1];
                AI_EvalFunc_OBJSetPos_C(v38, &v165);
            }
            else
            {
                AI_EvalFunc_OBJSetPos_C(0, &v165);
            }
        }
        if (AI_EvalFunc_TIMEElapsed_C(v171->f_time_ground_col, 1.0))
        {
            X_4 = v171->des_f_wait_delay - 2.0f;
            if (AI_EvalFunc_TIMEElapsed_C(v171->f_time_ground_col, X_4))
            {
                if (v171->i_flag_trappe_sortie)
                {
                    v171->i_flag_trappe_sortie = 0;
                    v138 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(6, -1.0);
                }
                v105 = TIM_gf_MainClock - TIM_gf_LockTime;
                v37 = v105 - (v171->des_f_wait_delay - 2.0f + v171->f_time_ground_col);
                if (v37 > 1.0)
                    v3 = 1.0;
                else
                    v3 = v105 - (v171->des_f_wait_delay - 2.0f + v171->f_time_ground_col);
                v128 = v3;
                v114 = v3;
                v36 = (tGameObjectMain*)v171->o_trappe;
                if (v36)
                {
                    if ((unsigned int)v36 <= 2)
                        v36 = AI_MainActor[(uint32_t)v36 - 1];
                    OBJ_RestoreInitialPos(v36);
                }
                else
                {
                    OBJ_RestoreInitialPos(0);
                }
                if (v114 < 1.0)
                {
                    v35 = (tGameObjectMain*)v171->o_trappe;
                    if (v35)
                    {
                        if ((unsigned int)v35 <= 2)
                            v35 = AI_MainActor[(uint32_t)v35 - 1];
                        AI_EvalFunc_OBJGetPos_C(v35, &v72);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v72);
                    }
                    v141 = v72;
                    v34 = v114;
                    if (v114 < 0.0)
                        v34 = 0.0;
                    if (v34 > 1.0)
                        v34 = 1.0;
                    v141.z = v141.z * v34 + v171->f_ground_Z * (1.0f - v34);
                    v33 = (tGameObjectMain*)v171->o_trappe;
                    if (v33)
                    {
                        if ((unsigned int)v33 <= 2)
                            v33 = AI_MainActor[(uint32_t)v33 - 1];
                        AI_EvalFunc_OBJSetPos_C(v33, &v141);
                    }
                    else
                    {
                        AI_EvalFunc_OBJSetPos_C(0, &v141);
                    }
                }
            }
            else
            {
                if (!v171->i_flag_trappe_sortie)
                {
                    v171->i_flag_trappe_sortie = 1;
                    v80 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(7, -1.0);
                }
                v147 = TIM_gf_MainClock - TIM_gf_LockTime;
                v32 = v147 - (v171->f_time_ground_col + 1.0f);
                if (v32 > 1.0)
                    v2 = 1.0;
                else
                    v2 = v147 - (v171->f_time_ground_col + 1.0f);
                v96 = v2;
                v114 = v2;
                v31 = (tGameObjectMain*)v171->o_trappe;
                if (v31)
                {
                    if ((unsigned int)v31 <= 2)
                        v31 = AI_MainActor[(uint32_t)v31 - 1];
                    OBJ_RestoreInitialPos(v31);
                }
                else
                {
                    OBJ_RestoreInitialPos(0);
                }
                v30 = (tGameObjectMain*)v171->o_trappe;
                if (v30)
                {
                    if ((unsigned int)v30 <= 2)
                        v30 = AI_MainActor[(uint32_t)v30 - 1];
                    AI_EvalFunc_OBJGetPos_C(v30, &v140);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v140);
                }
                v141 = v140;
                v29 = v114;
                if (v114 < 0.0)
                    v29 = 0.0;
                if (v29 > 1.0)
                    v29 = 1.0;
                v141.z = v171->f_ground_Z * v29 + (1.0f - v29) * v141.z;
                v28 = (tGameObjectMain*)v171->o_trappe;
                if (v28)
                {
                    if ((unsigned int)v28 <= 2)
                        v28 = AI_MainActor[(uint32_t)v28 - 1];
                    AI_EvalFunc_OBJSetPos_C(v28, &v141);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &v141);
                }
            }
        }
        if (v113 > 0.1)
        {
            if (AI_EvalFunc_TIMEElapsed_C(v171->f_time_sfx_gaz, v171->f_gfx_smoke_delay))
            {
                v171->f_time_sfx_gaz = TIM_gf_MainClock - TIM_gf_LockTime;
                v171->f_gfx_smoke_delay = AI_EvalFunc_MATHRandFloat_C(0.0099999998f, 0.050000001f);
                if (v171->GFX_smoke == -1)
                {
                    v171->GFX_smoke = AI_EvalFunc_GFXAdd_C_CURRENT(6);
                    AI_EvalFunc_GFXFlagSet_C_CURRENT(v171->GFX_smoke, 0, 1);
                    AI_EvalFunc_GFXFlagSet_C_CURRENT(v171->GFX_smoke, 2, 1);
                    v27 = (tGameObjectMain*)Vaisseau_Transport_Militaire_VTM_exec_afterrec_SFX_light_smoke_gao;
                    if (Vaisseau_Transport_Militaire_VTM_exec_afterrec_SFX_light_smoke_gao)
                    {
                        if ((unsigned int)Vaisseau_Transport_Militaire_VTM_exec_afterrec_SFX_light_smoke_gao <= 2)
                            v27 = AI_MainActor[Vaisseau_Transport_Militaire_VTM_exec_afterrec_SFX_light_smoke_gao - 1];
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v171->GFX_smoke, v27, 8);
                    }
                    else
                    {
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v171->GFX_smoke, 0, 8);
                    }
                    GFXSetCurGlobal(v171->GFX_smoke);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(6100, 300);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(6001, 1.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(6002, 0.25);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(6003, 2.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(6000, 1.0);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(6101, -1);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(6102, 0xFFFFFF);
                }
                for (i = 0; i < 3; ++i)
                {
                    if (i)
                    {
                        if (i == 1)
                            v171->o_sfx_reference_object = v171->o_plaque_2;
                        else
                            v171->o_sfx_reference_object = v171->o_plaque_3;
                    }
                    else
                    {
                        v171->o_sfx_reference_object = v171->o_plaque_1;
                    }
                    v26 = (tGameObjectMain*)v171->o_sfx_reference_object;
                    if (v26)
                    {
                        if ((unsigned int)v26 <= 2)
                            v26 = AI_MainActor[(uint32_t)v26 - 1];
                        AI_EvalFunc_OBJGetPos_C(v26, &v77);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v77);
                    }
                    v141 = v77;
                    v25 = (tGameObjectMain*)v171->o_sfx_reference_object;
                    if (v25)
                    {
                        if ((unsigned int)v25 <= 2)
                            v25 = AI_MainActor[(uint32_t)v25 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v25, &v123);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v123);
                    }
                    v73 = v123.x * 3.0f;
                    v74 = v123.y * 3.0f;
                    v75 = v123.z * 3.0f;
                    v141.x = v141.x - v73;
                    v141.y = v141.y - v74;
                    v141.z = v141.z - v75;
                    v164 = AI_EvalFunc_MATHRandFloat_C(1.0, 2.0);
                    v119 = v113 * 4.0f + v164;
                    v121 = AI_EvalFunc_MATHRandFloat_C(0.0, 0.60000002f);
                    v24 = (tGameObjectMain*)v171->o_sfx_reference_object;
                    if (v24)
                    {
                        if ((unsigned int)v24 <= 2)
                            v24 = AI_MainActor[(uint32_t)v24 - 1];
                        AI_EvalFunc_OBJ_SightGet_C(v24, &v151);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_SightGet_C(0, &v151);
                    }
                    v23 = (tGameObjectMain*)v171->o_sfx_reference_object;
                    if (v23)
                    {
                        if ((unsigned int)v23 <= 2)
                            v23 = AI_MainActor[(uint32_t)v23 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v23, &v156);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v156);
                    }
                    v166.x = -v156.x;
                    v166.y = -v156.y;
                    v166.z = -v156.z;
                    AI_EvalFunc_MATHVecBlend_C(&v151, &v166, v121, &v81);
                    v117 = v81;
                    v22 = (tGameObjectMain*)v171->o_sfx_reference_object;
                    if (v22)
                    {
                        if ((unsigned int)v22 <= 2)
                            v22 = AI_MainActor[(uint32_t)v22 - 1];
                        AI_EvalFunc_OBJ_SightGet_C(v22, &v150);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_SightGet_C(0, &v150);
                    }
                    v133.x = v150.x * 0.69999999f;
                    v133.y = v150.y * 0.69999999f;
                    v133.z = v150.z * 0.69999999f;
                    MATH_VecAddVec(&v170, &v141, &v133);
                    GFXSetCurGlobal(v171->GFX_smoke);
                    AI_EvalFunc_GFXSetv_C_CURGLOB(6200, &v170);
                    v142.x = v117.x * v119;
                    v142.y = v117.y * v119;
                    v142.z = v117.z * v119;
                    AI_EvalFunc_GFXSetv_C_CURGLOB(6201, &v142);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(6103, 0);
                    v90 = AI_EvalFunc_MATHRandFloat_C(1.0, 2.0);
                    v119 = v113 * 4.0f + v90;
                    v121 = AI_EvalFunc_MATHRandFloat_C(0.0, 0.60000002f);
                    v21 = (tGameObjectMain*)v171->o_sfx_reference_object;
                    if (v21)
                    {
                        if ((unsigned int)v21 <= 2)
                            v21 = AI_MainActor[(uint32_t)v21 - 1];
                        AI_EvalFunc_OBJ_SightGet_C(v21, &v79);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_SightGet_C(0, &v79);
                    }
                    v186.x = -v79.x;
                    v186.y = -v79.y;
                    v186.z = -v79.z;
                    v20 = (tGameObjectMain*)v171->o_sfx_reference_object;
                    if (v20)
                    {
                        if ((unsigned int)v20 <= 2)
                            v20 = AI_MainActor[(uint32_t)v20 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v20, &v84);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v84);
                    }
                    v152.x = -v84.x;
                    v152.y = -v84.y;
                    v152.z = -v84.z;
                    AI_EvalFunc_MATHVecBlend_C(&v186, &v152, v121, &v161);
                    v117 = v161;
                    v19 = (tGameObjectMain*)v171->o_sfx_reference_object;
                    if (v19)
                    {
                        if ((unsigned int)v19 <= 2)
                            v19 = AI_MainActor[(uint32_t)v19 - 1];
                        AI_EvalFunc_OBJ_SightGet_C(v19, &v82);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_SightGet_C(0, &v82);
                    }
                    v87.x = v82.x * 0.69999999f;
                    v87.y = v82.y * 0.69999999f;
                    v87.z = v82.z * 0.69999999f;
                    MATH_SubstractVec(&v116, &v141, &v87);
                    AI_EvalFunc_GFXSetv_C_CURGLOB(6200, &v116);
                    v131.x = v117.x * v119;
                    v131.y = v117.y * v119;
                    v131.z = v117.z * v119;
                    AI_EvalFunc_GFXSetv_C_CURGLOB(6201, &v131);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(6103, 0);
                }
                for (i = 0; i < 3; ++i)
                {
                    if (i)
                    {
                        if (i == 1)
                        {
                            v16 = (tGameObjectMain*)v171->o_arceau_2;
                            if (v16)
                            {
                                if ((unsigned int)v16 <= 2)
                                    v16 = AI_MainActor[(uint32_t)v16 - 1];
                                AI_EvalFunc_OBJGetPos_C(v16, &v122);
                            }
                            else
                            {
                                AI_EvalFunc_OBJGetPos_C(0, &v122);
                            }
                            v15 = (tGameObjectMain*)v171->o_arceau_3;
                            if (v15)
                            {
                                if ((unsigned int)v15 <= 2)
                                    v15 = AI_MainActor[(uint32_t)v15 - 1];
                                AI_EvalFunc_OBJGetPos_C(v15, &v183);
                            }
                            else
                            {
                                AI_EvalFunc_OBJGetPos_C(0, &v183);
                            }
                            AI_EvalFunc_MATHVecBlend_C(&v122, &v183, 0.5, &v179);
                            v141 = v179;
                        }
                        else
                        {
                            v14 = (tGameObjectMain*)v171->o_arceau_3;
                            if (v14)
                            {
                                if ((unsigned int)v14 <= 2)
                                    v14 = AI_MainActor[(uint32_t)v14 - 1];
                                AI_EvalFunc_OBJGetPos_C(v14, &v93);
                            }
                            else
                            {
                                AI_EvalFunc_OBJGetPos_C(0, &v93);
                            }
                            v13 = (tGameObjectMain*)v171->o_arceau_1;
                            if (v13)
                            {
                                if ((unsigned int)v13 <= 2)
                                    v13 = AI_MainActor[(uint32_t)v13 - 1];
                                AI_EvalFunc_OBJGetPos_C(v13, &v178);
                            }
                            else
                            {
                                AI_EvalFunc_OBJGetPos_C(0, &v178);
                            }
                            AI_EvalFunc_MATHVecBlend_C(&v93, &v178, 0.5, &v100);
                            v141 = v100;
                        }
                    }
                    else
                    {
                        v18 = (tGameObjectMain*)v171->o_arceau_1;
                        if (v18)
                        {
                            if ((unsigned int)v18 <= 2)
                                v18 = AI_MainActor[(uint32_t)v18 - 1];
                            AI_EvalFunc_OBJGetPos_C(v18, &v78);
                        }
                        else
                        {
                            AI_EvalFunc_OBJGetPos_C(0, &v78);
                        }
                        v17 = (tGameObjectMain*)v171->o_arceau_2;
                        if (v17)
                        {
                            if ((unsigned int)v17 <= 2)
                                v17 = AI_MainActor[(uint32_t)v17 - 1];
                            AI_EvalFunc_OBJGetPos_C(v17, &v76);
                        }
                        else
                        {
                            AI_EvalFunc_OBJGetPos_C(0, &v76);
                        }
                        AI_EvalFunc_MATHVecBlend_C(&v78, &v76, 0.5, &v134);
                        v141 = v134;
                    }
                    v141.z = v141.z - 1.3f;
                    v12 = (tGameObjectMain*)v171->o_corps;
                    if (v12)
                    {
                        if ((unsigned int)v12 <= 2)
                            v12 = AI_MainActor[(uint32_t)v12 - 1];
                        AI_EvalFunc_OBJGetPos_C(v12, &v118);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v118);
                    }
                    v117.x = v118.x;
                    v117.y = v118.y;
                    v117.z = v141.z;
                    MATH_SubstractVec(&v94, &v141, &v117);
                    v117 = v94;
                    v141.x = v141.x + v94.x;
                    v141.y = v141.y + v94.y;
                    v141.z = v141.z + v94.z;
                    v158 = 0;
                    v159 = 0;
                    v160 = 1065353216;
                    v124.x = -0.0;
                    v124.y = -0.0;
                    v124.z = -1.0;
                    v98 = AI_EvalFunc_MATHRandFloat_C(0.0, 0.30000001f);
                    AI_EvalFunc_MATHVecBlend_C(&v124, &v117, v98, &v185);
                    v117 = v185;
                    v119 = AI_EvalFunc_MATHRandFloat_C(2.0, 4.0);
                    GFXSetCurGlobal(v171->GFX_smoke);
                    AI_EvalFunc_GFXSetv_C_CURGLOB(6200, &v141);
                    v127.x = v117.x * v119;
                    v127.y = v117.y * v119;
                    v127.z = v117.z * v119;
                    AI_EvalFunc_GFXSetv_C_CURGLOB(6201, &v127);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(6103, 0);
                }
            }
        }
    }
    else if (v171->GFX_smoke != -1)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v171->f_time_sfx_gaz, 5.0))
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v171->GFX_smoke);
            v171->GFX_smoke = -1;
        }
    }
    v190 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Vaisseau_Transport_Militaire_VTM_exec_afterrec_Vaisseau_Transport_Militaire_VTM_exec_GFX);
    if (v190 == 1000)
        result = 1000;
    else
        result = v190;
    return result;
}

int Vaisseau_Transport_Militaire_VTM_init()
{
    int v0; // eax
    int result; // eax
    tScriptStruct_PNJVaisseauTransportMilitaire* v2; // [esp+0h] [ebp-DCh]
    tGameObjectMain* v3; // [esp+10h] [ebp-CCh]
    tGameObjectMain* v4; // [esp+18h] [ebp-C4h]
    Vec3 v5; // [esp+24h] [ebp-B8h] BYREF
    int v6; // [esp+3Ch] [ebp-A0h]
    Vec3 v7; // [esp+5Ch] [ebp-80h] BYREF
    BOOL v8; // [esp+84h] [ebp-58h]
    Vec3 a3; // [esp+94h] [ebp-48h] BYREF
    Vec3 dest; // [esp+A8h] [ebp-34h] BYREF
    tScriptStruct_PNJVaisseauTransportMilitaire* v11; // [esp+B4h] [ebp-28h]
    Vec3 v12; // [esp+B8h] [ebp-24h] BYREF
    int v13; // [esp+CCh] [ebp-10h]
    int v14; // [esp+D8h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJVaisseauTransportMilitaire*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v11 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Vaisseau_Transport_Militaire_VTM_init_Vaisseau_Transport_Militaire_VTM_mode_deplacement = (int)AI_ul_RealResolveOneRefFunction(0x72000B52u);
        fct_Vaisseau_Transport_Militaire_VTM_init_Vaisseau_Transport_Militaire_VTM_reflex = (int)AI_ul_RealResolveOneRefFunction(0x72000B54u);
    }
    v11->o_corps = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0);
    if (v11->des_patrouilleur)
    {
        v11->o_motor_1 = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(1u);
        v11->o_motor_2 = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(2u);
        v11->o_motor_3 = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(3u);
        v11->o_gyro = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(4u);
    }
    else
    {
        v11->o_arceau_1 = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(1u);
        v11->o_arceau_2 = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(2u);
        v11->o_arceau_3 = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(3u);
        v11->o_tete = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(4u);
        v11->o_gyro = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(5u);
        v11->o_motor_1 = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(7u);
        v11->o_motor_2 = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(8u);
        v11->o_plaque_1 = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(9u);
        v11->o_plaque_2 = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(10u);
        v11->o_plaque_3 = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(11u);
        v11->o_pod_1 = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(12u);
        v11->o_pod_2 = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(13u);
        v11->o_pod_3 = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(14u);
        v11->o_light_red = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(6u);
        v11->o_light_blue = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(15u);
        v11->o_trappe = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(16u);
    }
    v4 = (tGameObjectMain*)v11->des_o_start_wp;
    if (v4)
    {
        if ((unsigned int)v4 <= 2)
            v4 = AI_MainActor[(uint32_t)v4 - 1];
        v0 = (int)AI_EvalFunc_WAYNetOfObj_C(v4);
    }
    else
    {
        v0 = (int)AI_EvalFunc_WAYNetOfObj_C(0);
    }
    v11->n_net = v0;
    v11->o_next_wp = v11->des_o_start_wp;
    v11->i_snd_moteur_vol = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000000);
    v11->i_snd_moteur_vol_freq = SND_l_GetFreq(v11->i_snd_moteur_vol);
    v11->i_snd_moteur_arret = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 8, 0x18000000);
    AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v11->i_snd_moteur_vol);
    v3 = (tGameObjectMain*)v11->des_o_start_wp;
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        AI_EvalFunc_OBJGetPos_C(v3, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    AI_EvalFunc_OBJSetPos_C_CURRENT(&a3);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v5);
    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v12);
    v7.x = v12.x * 10.0f;
    v7.y = v12.y * 10.0f;
    v7.z = v12.z * 10.0f;
    MATH_SubstractVec(&dest, &v5, &v7);
    v11->v_grav_pos = dest;
    AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v11->v_grav_speed);
    v8 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
    v13 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 4, 0);
    v6 = AI_EvalFunc_DYNFrictionSet_C(AI_gpst_CurrentGameObject, 2.0);
    AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, 1);
    AI_EvalFunc_AITrackChange_C(
        2,
        (tAIFunction*)fct_Vaisseau_Transport_Militaire_VTM_init_Vaisseau_Transport_Militaire_VTM_mode_deplacement);
    v14 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Vaisseau_Transport_Militaire_VTM_init_Vaisseau_Transport_Militaire_VTM_reflex);
    if (v14 == 1000)
        result = 1000;
    else
        result = v14;
    return result;
}

int Vaisseau_Transport_Militaire_VTM_reflex()
{
    double v0; // st7
    tScriptStruct_PNJVaisseauTransportMilitaire* v2; // [esp+0h] [ebp-6Ch]
    Vec3* v3; // [esp+8h] [ebp-64h]
    Vec3* v4; // [esp+Ch] [ebp-60h]
    Vec3 a2; // [esp+44h] [ebp-28h] BYREF
    BOOL v6; // [esp+50h] [ebp-1Ch]
    BOOL v7; // [esp+54h] [ebp-18h]
    tScriptStruct_PNJVaisseauTransportMilitaire* v8; // [esp+5Ch] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJVaisseauTransportMilitaire*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v8 = v2;
    AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&a2);
    v8->v_speed = a2;
    v0 = MATH_VecLen(&v8->v_speed);
    v8->f_speed = (float)v0;
    if (fabs(v8->v_speed.x) != 0.0f)
    {
        v7 = 0;
    }
    else if (fabs(v8->v_speed.y) != 0.0f)
    {
        v7 = 0;
    }
    else
    {
        v7 = fabs(v8->v_speed.z) == 0.0f;
    }
    if (v7)
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v8->v_normalized_speed);
    else
        AI_EvalFunc_MATHVecNormalize_C(&v8->v_speed, &v8->v_normalized_speed);
    v3 = &v8->v_hor_speed;
    v4 = &v8->v_speed;
    v8->v_hor_speed.x = v8->v_speed.x;
    v3->y = v4->y;
    v3->z = v4->z;
    v8->v_hor_speed.z = 0.0;
    if (fabs(v8->v_hor_speed.x) != 0.0f)
    {
        v6 = 0;
    }
    else if (fabs(v8->v_hor_speed.y) != 0.0f)
    {
        v6 = 0;
    }
    else
    {
        v6 = fabs(v8->v_hor_speed.z) == 0.0f;
    }
    if (v6)
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v8->v_normalized_hor_speed);
    else
        AI_EvalFunc_MATHVecNormalize_C(&v8->v_hor_speed, &v8->v_normalized_hor_speed);
    return 1000;
}

int Vaisseau_Transport_Militaire_VTM_exec_s_b()
{
    int result; // eax
    BOOL v1; // eax
    float v2; // [esp+0h] [ebp-2B8h]
    float v3; // [esp+0h] [ebp-2B8h]
    float v4; // [esp+0h] [ebp-2B8h]
    tScriptStruct_PNJVaisseauTransportMilitaire* v5; // [esp+8h] [ebp-2B0h]
    tGameObjectMain* v6; // [esp+10h] [ebp-2A8h]
    tGameObjectMain* v7; // [esp+18h] [ebp-2A0h]
    tGameObjectMain* v8; // [esp+20h] [ebp-298h]
    tGameObjectMain* v9; // [esp+28h] [ebp-290h]
    tGameObjectMain* v10; // [esp+38h] [ebp-280h]
    tGameObjectMain* v11; // [esp+48h] [ebp-270h]
    tGameObjectMain* v12; // [esp+50h] [ebp-268h]
    tGameObjectMain* v13; // [esp+78h] [ebp-240h]
    tGameObjectMain* v14; // [esp+88h] [ebp-230h]
    float v15; // [esp+94h] [ebp-224h]
    Vec3* v16; // [esp+A0h] [ebp-218h]
    Vec3* v17; // [esp+A4h] [ebp-214h]
    tGameObjectMain* v18; // [esp+D0h] [ebp-1E8h]
    tGameObjectMain* v19; // [esp+D8h] [ebp-1E0h]
    float v20; // [esp+E4h] [ebp-1D4h]
    Vec3 v21; // [esp+E8h] [ebp-1D0h] BYREF
    Vec3 v22; // [esp+F4h] [ebp-1C4h] BYREF
    Vec3 v23; // [esp+100h] [ebp-1B8h] BYREF
    Vec3 v24; // [esp+10Ch] [ebp-1ACh] BYREF
    Vec3 v25; // [esp+118h] [ebp-1A0h] BYREF
    Vec3 v26; // [esp+124h] [ebp-194h] BYREF
    Vec3 v27; // [esp+13Ch] [ebp-17Ch] BYREF
    float v28; // [esp+154h] [ebp-164h]
    Vec3 v29; // [esp+158h] [ebp-160h] BYREF
    Vec3 v30; // [esp+164h] [ebp-154h] BYREF
    Vec3 v31; // [esp+170h] [ebp-148h] BYREF
    float v32; // [esp+17Ch] [ebp-13Ch]
    Vec3 v33; // [esp+180h] [ebp-138h] BYREF
    Vec3 v34; // [esp+18Ch] [ebp-12Ch] BYREF
    Vec3 a4; // [esp+198h] [ebp-120h] BYREF
    Vec3 v36; // [esp+1A4h] [ebp-114h] BYREF
    Vec3 v37; // [esp+1B4h] [ebp-104h] BYREF
    float v38; // [esp+1C0h] [ebp-F8h]
    float v39; // [esp+1C4h] [ebp-F4h]
    Vec3 v40; // [esp+1C8h] [ebp-F0h] BYREF
    Vec3 dest; // [esp+1D4h] [ebp-E4h] BYREF
    Vec3 v42; // [esp+1E0h] [ebp-D8h] BYREF
    Vec3 v43; // [esp+1ECh] [ebp-CCh] BYREF
    float v44; // [esp+1F8h] [ebp-C0h]
    Vec3 a3; // [esp+1FCh] [ebp-BCh] BYREF
    Vec3 v46; // [esp+208h] [ebp-B0h] BYREF
    float v47; // [esp+214h] [ebp-A4h]
    Vec3 v48; // [esp+218h] [ebp-A0h] BYREF
    Vec3 v49; // [esp+224h] [ebp-94h] BYREF
    Vec3 v50; // [esp+230h] [ebp-88h] BYREF
    float v51; // [esp+23Ch] [ebp-7Ch]
    Vec3 v52; // [esp+240h] [ebp-78h] BYREF
    BOOL v53; // [esp+24Ch] [ebp-6Ch]
    Vec3 v54; // [esp+250h] [ebp-68h] BYREF
    tScriptStruct_PNJVaisseauTransportMilitaire* v55; // [esp+25Ch] [ebp-5Ch]
    Vec3 a1; // [esp+260h] [ebp-58h] BYREF
    Vec3 v57; // [esp+26Ch] [ebp-4Ch] BYREF
    Vec3 v58; // [esp+278h] [ebp-40h] BYREF
    Vec3 v59; // [esp+284h] [ebp-34h] BYREF
    float v60; // [esp+290h] [ebp-28h]
    Vec3 v61; // [esp+294h] [ebp-24h] BYREF
    Vec3 v62; // [esp+2A0h] [ebp-18h] BYREF
    Vec3 v63; // [esp+2ACh] [ebp-Ch] BYREF

    v49.x = 1.0;
    v49.y = 0.0;
    v49.z = 0.0;
    v29.x = 1.0;
    v29.y = 0.0;
    v29.z = 0.0;
    v40.x = 1.0;
    v40.y = 0.0;
    v40.z = 0.0;
    v26.x = 1.0;
    v26.y = 0.0;
    v26.z = 0.0;
    v34.x = 1.0;
    v34.y = 0.0;
    v34.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_PNJVaisseauTransportMilitaire*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v55 = v5;
    if (!v5->i_flag_on_ground || v55->i_flag_decollage)
    {
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v30);
        v29 = v30;
        v19 = (tGameObjectMain*)v55->o_next_wp;
        if (v19)
        {
            if ((unsigned int)v19 <= 2)
                v19 = AI_MainActor[(uint32_t)v19 - 1];
            v1 = AI_EvalFunc_OBJ_CapaTest_C(v19, 0x100);
        }
        else
        {
            v1 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x100);
        }
        v53 = v1;
        if (v1)
        {
            v18 = (tGameObjectMain*)v55->o_next_wp;
            if (v18)
            {
                if ((unsigned int)v18 <= 2)
                    v18 = AI_MainActor[(uint32_t)v18 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v18, &a3);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &a3);
            }
            v40 = a3;
        }
        else if (v55->i_flag_decollage)
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v25);
            v40 = v25;
        }
        else
        {
            v40 = v55->v_normalized_hor_speed;
        }
        v26 = v55->v_grav_pos;
        v23.x = v55->v_grav_speed.x * 0.89999998f;
        v23.y = v55->v_grav_speed.y * 0.89999998f;
        v23.z = v55->v_grav_speed.z * 0.89999998f;
        v38 = TIM_gf_dt;
        MATH_VecMultScalar(&a1, &v23, TIM_gf_dt);
        MATH_VecAddVec(&v55->v_grav_pos, &v55->v_grav_pos, &a1);
        v46.x = 0.0;
        v46.y = 0.0;
        v46.z = -5.0;
        v60 = TIM_gf_dt;
        MATH_VecMultScalar(&v31, &v46, TIM_gf_dt);
        v17 = &v55->v_grav_pos;
        v55->v_grav_pos.x = v31.x + v55->v_grav_pos.x;
        v17->y = v31.y + v17->y;
        v17->z = v31.z + v17->z;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v43);
        MATH_SubstractVec(&v55->v_grav_pos, &v55->v_grav_pos, &v43);
        AI_EvalFunc_MATHVecSetNormVector_C(&v55->v_grav_pos, 10.0);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v63);
        v16 = &v55->v_grav_pos;
        v55->v_grav_pos.x = v63.x + v55->v_grav_pos.x;
        v16->y = v63.y + v16->y;
        v16->z = v63.z + v16->z;
        MATH_SubstractVec(&dest, &v55->v_grav_pos, &v26);
        v32 = TIM_gf_dt;
        MATH_VecMultNormScalar(&v36, &dest, TIM_gf_dt);
        v55->v_grav_speed = v36;
        if (!v55->i_flag_decollage)
        {
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v22);
            v51 = TIM_gf_dt;
            v15 = 5.0f * TIM_gf_dt;
            if (v15 < 0.0)
                v15 = 0.0;
            if (v15 > 1.0)
                v15 = 1.0;
            v20 = v40.z * v22.z + v40.y * v22.y + v22.x * v40.x;
            v44 = v20 * 3.0f * v15 + v55->f_rotation_coef * (1.0f - v15);
            v55->f_rotation_coef = v44;
            v61.x = 0.0;
            v61.y = 0.0;
            v61.z = 1.0;
            v47 = TIM_gf_dt;
            v2 = v55->f_rotation_coef * TIM_gf_dt;
            AI_EvalFunc_MATHVecRotate_C(&v29, &v61, v2, &a4);
            v40 = a4;
        }
        v33.x = 0.0;
        v33.y = 0.0;
        v33.z = 1.0;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v40, &v33);
        v14 = (tGameObjectMain*)v55->o_corps;
        if (v14)
        {
            if ((unsigned int)v14 <= 2)
                v14 = AI_MainActor[(uint32_t)v14 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v14, &v50);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &v50);
        }
        v29 = v50;
        v13 = (tGameObjectMain*)v55->o_corps;
        if (v13)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            AI_EvalFunc_OBJ_BankingGet_C(v13, &v24);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGet_C(0, &v24);
        }
        v34 = v24;
        if (v55->i_flag_position_ok)
        {
            v57.x = 0.0;
            v57.y = 0.0;
            v57.z = 1.0;
            v39 = TIM_gf_dt;
            v3 = 8.0f * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlend_C(&v34, &v57, v3, &v54);
            v49 = v54;
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v37);
            MATH_SubstractVec(&v42, &v37, &v55->v_grav_pos);
            AI_EvalFunc_MATHVecNormalize_C(&v42, &v59);
            v49 = v59;
            v28 = TIM_gf_dt;
            v4 = 8.0f * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlend_C(&v34, &v49, v4, &v62);
            v49 = v62;
        }
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v48);
        v12 = (tGameObjectMain*)v55->o_corps;
        if (v12)
        {
            if ((unsigned int)v12 <= 2)
                v12 = AI_MainActor[(uint32_t)v12 - 1];
            AI_EvalFunc_OBJ_BankingGeneralSet_C(v12, &v48, &v49);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v48, &v49);
        }
        if (!v55->des_patrouilleur)
        {
            v11 = (tGameObjectMain*)v55->o_corps;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v11 = AI_MainActor[(uint32_t)v11 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v11, &v21);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v21);
            }
            v29.x = v21.x;
            v29.y = v21.y;
            v29.z = 0.0;
            v10 = (tGameObjectMain*)v55->o_corps;
            if (v10)
            {
                if ((unsigned int)v10 <= 2)
                    v10 = AI_MainActor[(uint32_t)v10 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v10, &v27);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v27);
            }
            v49 = v27;
            v9 = (tGameObjectMain*)v55->o_motor_1;
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v9, &v52);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v52);
            }
            v8 = (tGameObjectMain*)v55->o_motor_1;
            if (v8)
            {
                if ((unsigned int)v8 <= 2)
                    v8 = AI_MainActor[(uint32_t)v8 - 1];
                AI_EvalFunc_OBJ_SightGeneralSet_C(v8, &v52, &v49);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGeneralSet_C(0, &v52, &v49);
            }
            v7 = (tGameObjectMain*)v55->o_motor_2;
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v7, &v58);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v58);
            }
            v6 = (tGameObjectMain*)v55->o_motor_2;
            if (v6)
            {
                if ((unsigned int)v6 <= 2)
                    v6 = AI_MainActor[(uint32_t)v6 - 1];
                AI_EvalFunc_OBJ_SightGeneralSet_C(v6, &v58, &v49);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGeneralSet_C(0, &v58, &v49);
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