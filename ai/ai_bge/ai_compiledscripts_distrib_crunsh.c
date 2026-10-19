#include "ai_compiledscripts_inc.h"

//distrib_crunsh_dc_init
int fct_distrib_crunsh_dc_init_distrib_crunsh_dc_reflex;

//distrib_crunsh_dc_deroule
int distrib_crunsh_dc_deroule_GST_Global_gao;
int distrib_crunsh_dc_deroule_GST_Global_gao_i_freeze_joy;
int distrib_crunsh_dc_deroule_GST_Board_panel_gao;
int distrib_crunsh_dc_deroule_GST_Board_panel_gao_i_sally_priority;
int distrib_crunsh_dc_deroule_GST_Board_panel_gao_o_sally_contextuel;
int distrib_crunsh_dc_deroule_GST_Board_panel_gao_i_sally_action;
int distrib_crunsh_dc_deroule_GST_Board_panel_gao_v_sally_offset;
int distrib_crunsh_dc_deroule_GST_Board_panel_gao_i_force_interface;
int fct_distrib_crunsh_dc_deroule_distrib_crunsh_dc_exec_cam;
int distrib_crunsh_dc_deroule__PJ_Sally_gao;
int distrib_crunsh_dc_deroule__PJ_Sally_gao_current_pm_mode;
int distrib_crunsh_dc_deroule__PJ_Sally_gao_current_pm_priority;
int distrib_crunsh_dc_deroule_univVars;
int distrib_crunsh_dc_deroule_univVars_f_sally_fric;
int distrib_crunsh_dc_deroule_TXT_Sally_gao;
int fct_distrib_crunsh_dc_deroule_distrib_crunsh_dc_bouge;
int distrib_crunsh_dc_deroule__PJ_Camera_gao;
int distrib_crunsh_dc_deroule__PJ_Camera_Target_gao;
int distrib_crunsh_dc_deroule__PJ_Camera_Derriere_gao;
int fct_distrib_crunsh_dc_deroule_distrib_crunsh_dc_reflex;

//distrib_crunsh_dc_texte
int distrib_crunsh_dc_texte_GST_Board_panel_gao;
int distrib_crunsh_dc_texte_GST_Board_panel_gao_i_commerce_tune;
int distrib_crunsh_dc_texte_GST_AffichageTexte_gao;
int distrib_crunsh_dc_texte_GST_AffichageTexte_gao_texte_message_a_traiter;
int distrib_crunsh_dc_texte_GST_AffichageTexte_gao_i_Sound_ID;
int distrib_crunsh_dc_texte_GST_AffichageTexte_gao_o_Sound_Localisation3D;
int distrib_crunsh_dc_texte_GST_AffichageTexte_gao_f_Sound_Volume;
int distrib_crunsh_dc_texte_GST_AffichageTexte_gao_f_near;
int distrib_crunsh_dc_texte_GST_AffichageTexte_gao_f_far;
int distrib_crunsh_dc_texte_GST_AffichageTexte_gao_message_a_traiter;
int distrib_crunsh_dc_texte_GST_AffichageTexte_gao_prio_message_a_traiter;

//distrib_crunsh_dc_exec_cam
int distrib_crunsh_dc_exec_cam__PJ_Sally_gao;
int distrib_crunsh_dc_exec_cam__PJ_Camera_gao;
int distrib_crunsh_dc_exec_cam__PJ_Camera_Derriere_gao;
int distrib_crunsh_dc_exec_cam__PJ_Camera_Target_gao;

//distrib_crunsh_dc_reflex
int distrib_crunsh_dc_reflex__PJ_Sally_gao;
int distrib_crunsh_dc_reflex__PJ_Sally_gao_i_fight_on;
int distrib_crunsh_dc_reflex_univVars;
int distrib_crunsh_dc_reflex_univVars_Perso_ID_equipement;
int distrib_crunsh_dc_reflex__PJ_Sally_gao_current_pm_mode;
int distrib_crunsh_dc_reflex__PJ_Sally_gao_current_pm_priority;
int distrib_crunsh_dc_reflex_GST_Board_panel_gao;
int distrib_crunsh_dc_reflex_GST_Board_panel_gao_i_use_prio;
int distrib_crunsh_dc_reflex_GST_Board_panel_gao_o_use_object;
int distrib_crunsh_dc_reflex_GST_Board_panel_gao_i_use_action;
int distrib_crunsh_dc_reflex_GST_Board_panel_gao_f_use_offset;
int distrib_crunsh_dc_reflex_GST_Board_panel_gao_i_sally_priority;
int distrib_crunsh_dc_reflex_GST_Board_panel_gao_o_sally_contextuel;
int distrib_crunsh_dc_reflex_GST_Board_panel_gao_i_sally_action;
int distrib_crunsh_dc_reflex_GST_Board_panel_gao_v_sally_offset;
int distrib_crunsh_dc_reflex_GST_Board_panel_gao_i_force_interface;
int distrib_crunsh_dc_reflex_TXT_Sally_gao;
int distrib_crunsh_dc_reflex__PJ_Camera_gao;
int fct_distrib_crunsh_dc_reflex_distrib_crunsh_dc_deroule;

int distrib_crunsh_dc_init()
{
    int result; // eax
    char* v1; // eax
    char* v2; // eax
    char* v3; // eax
    char* v4; // eax
    char* v5; // eax
    char* v6; // eax
    char* v7; // eax
    char* v8; // eax
    char* v9; // eax
    char* v10; // eax
    char* v11; // eax
    char* v12; // eax
    char* v13; // eax
    char* v14; // eax
    char* v15; // eax
    char* v16; // eax
    char* v17; // eax
    char* v18; // eax
    char* v19; // eax
    char* v20; // eax
    char* v21; // eax
    tGameObjectMain* v22; // eax
    char* v23; // eax
    float v24; // [esp+0h] [ebp-230h]
    float v25; // [esp+8h] [ebp-228h]
    tScriptStruct_DistribCrunsh* v26; // [esp+14h] [ebp-21Ch]
    tGameObjectMain* v27; // [esp+1Ch] [ebp-214h]
    tGameObjectMain* v28; // [esp+34h] [ebp-1FCh]
    tGameObjectMain* v29; // [esp+44h] [ebp-1ECh]
    tGameObjectMain* v30; // [esp+4Ch] [ebp-1E4h]
    tGameObjectMain* v31; // [esp+54h] [ebp-1DCh]
    tGameObjectMain* v32; // [esp+5Ch] [ebp-1D4h]
    tGameObjectMain* v33; // [esp+64h] [ebp-1CCh]
    tGameObjectMain* v34; // [esp+6Ch] [ebp-1C4h]
    tGameObjectMain* v35; // [esp+74h] [ebp-1BCh]
    tAI* v36; // [esp+78h] [ebp-1B8h]
    tGameObjectMain* v37; // [esp+7Ch] [ebp-1B4h]
    tGameObjectMain* v38; // [esp+80h] [ebp-1B0h]
    tGameObjectMain* v39; // [esp+84h] [ebp-1ACh]
    tGameObjectMain* v40; // [esp+88h] [ebp-1A8h]
    Vec3 v41; // [esp+8Ch] [ebp-1A4h] BYREF
    tTextGet v42; // [esp+98h] [ebp-198h] BYREF
    tTextGet v43; // [esp+A0h] [ebp-190h] BYREF
    Vec3 v44; // [esp+A8h] [ebp-188h] BYREF
    tTextGet v45; // [esp+B4h] [ebp-17Ch] BYREF
    tTextGet v46; // [esp+BCh] [ebp-174h] BYREF
    tAI* v47; // [esp+C4h] [ebp-16Ch]
    unsigned int v48; // [esp+C8h] [ebp-168h]
    Vec3 v49; // [esp+CCh] [ebp-164h] BYREF
    Vec3 v50; // [esp+D8h] [ebp-158h] BYREF
    tTextGet v51; // [esp+E4h] [ebp-14Ch] BYREF
    Vec3 v52; // [esp+ECh] [ebp-144h] BYREF
    Vec3 v53; // [esp+F8h] [ebp-138h] BYREF
    Vec3 v54; // [esp+104h] [ebp-12Ch] BYREF
    tTextGet v55; // [esp+110h] [ebp-120h] BYREF
    tTextGet v56; // [esp+118h] [ebp-118h] BYREF
    Vec3 v57; // [esp+120h] [ebp-110h] BYREF
    tTextGet v58; // [esp+12Ch] [ebp-104h] BYREF
    Vec3 v59; // [esp+134h] [ebp-FCh] BYREF
    tTextGet v60; // [esp+140h] [ebp-F0h] BYREF
    tTextGet v61; // [esp+148h] [ebp-E8h] BYREF
    Vec3 v62; // [esp+150h] [ebp-E0h] BYREF
    Vec3 v63; // [esp+15Ch] [ebp-D4h] BYREF
    tTextGet v64; // [esp+168h] [ebp-C8h] BYREF
    tTextGet v65; // [esp+174h] [ebp-BCh] BYREF
    tTextGet v66; // [esp+17Ch] [ebp-B4h] BYREF
    tTextGet Src; // [esp+184h] [ebp-ACh] BYREF
    tTextGet v68; // [esp+18Ch] [ebp-A4h] BYREF
    Vec3 a3; // [esp+194h] [ebp-9Ch] BYREF
    Vec3 v70; // [esp+1A0h] [ebp-90h] BYREF
    int v71; // [esp+1ACh] [ebp-84h]
    tTextGet v72; // [esp+1B0h] [ebp-80h] BYREF
    tTextGet v73; // [esp+1B8h] [ebp-78h] BYREF
    Vec3 a4; // [esp+1C0h] [ebp-70h] BYREF
    Vec3 v75; // [esp+1CCh] [ebp-64h] BYREF
    tScriptStruct_DistribCrunsh* v76; // [esp+1D8h] [ebp-58h]
    tTextGet v77; // [esp+1DCh] [ebp-54h] BYREF
    int num; // [esp+1E4h] [ebp-4Ch]
    Vec3 v79; // [esp+1E8h] [ebp-48h] BYREF
    Vec3 v80; // [esp+1F4h] [ebp-3Ch] BYREF
    Vec3 v81; // [esp+200h] [ebp-30h] BYREF
    tTextGet v82; // [esp+20Ch] [ebp-24h] BYREF
    Vec3 v83; // [esp+214h] [ebp-1Ch] BYREF
    tTextGet v84; // [esp+220h] [ebp-10h] BYREF
    tTextGet v85; // [esp+228h] [ebp-8h] BYREF

    v70.x = 1.0;
    v70.y = 0.0;
    v70.z = 0.0;
    v63.x = 1.0;
    v63.y = 0.0;
    v63.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v26 = (tScriptStruct_DistribCrunsh*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v26 = 0;
    v76 = v26;
    v71 = 0;
    v47 = 0;
    v48 = 0;
    num = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_distrib_crunsh_dc_init_distrib_crunsh_dc_reflex = (int)AI_ul_RealResolveOneRefFunction(0x3D001D4Du);
    }
    if (*AI_gppst_CurrentJumpNode)
    {
        *AI_gppst_CurrentJumpNode = 0;
        TIME_Wait_C(0.5);
        result = 0;
    }
    else
    {
        for (num = 0; num < 6; ++num)
        {
            v40 = (tGameObjectMain*)v76->des_ao_contenu[num];
            if (v40)
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
                v47 = (tAI*)v76->des_ao_contenu[num];
                if (v47 == WOR_gst_Universe.univAIInstance)
                {
                    v36 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v38 = (tGameObjectMain*)v47;
                    if (v47)
                    {
                        if ((unsigned int)v47 <= 2)
                            v38 = AI_MainActor[(uint32_t)v47 - 1];
                        v37 = v38;
                    }
                    else
                    {
                        v37 = 0;
                    }
                    v36 = v37->extended->AIInstance;
                }
                v71 = ((tScriptStruct_GetObject*)(v36->instanceVarDataCurrent))->des_i_object;
                v52.x = 0.0;
                v52.y = 0.0;
                v52.z = 1.0;
                v75.x = 0.0;
                v75.y = -1.0;
                v75.z = 0.0;
                v35 = (tGameObjectMain*)v47;
                if (v47)
                {
                    if ((unsigned int)v47 <= 2)
                        v35 = AI_MainActor[(uint32_t)v47 - 1];
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(v35, &v52, &v75);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v52, &v75);
                }
                switch (v71)
                {
                case 30:
                    v76->des_ai_prix[num] = 150;
                    v76->des_ai_son[num] = 0;
                    AI_EvalFunc_TEXTGet_C(0xD0002C9E, 43, &Src);
                    v1 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_prix_Dimension1, num);
                    memcpy(v1, &Src, 8u);
                    AI_EvalFunc_TEXTGet_C(0xD0002C9E, 50, &v45);
                    v2 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_noms_Dimension1, num);
                    memcpy(v2, &v45, 8u);
                    AI_EvalFunc_TEXTGet_C(0x72000655, 119, &v42);
                    v3 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_explication_Dimension1, num);
                    memcpy(v3, &v42, 8u);
                    v59.x = 0.0;
                    v59.y = 1.0;
                    v59.z = 0.0;
                    v81.x = 0.0;
                    v81.y = 0.0;
                    v81.z = 1.0;
                    v34 = (tGameObjectMain*)v47;
                    if (v47)
                    {
                        if ((unsigned int)v47 <= 2)
                            v34 = AI_MainActor[(uint32_t)v47 - 1];
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(v34, &v59, &v81);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v59, &v81);
                    }
                    *(uint32_t*)AI_EvalVarArray_C2((char*)&v76->__des_f_offset_Z_Dimension1, num) = 0x3CA3D70A;
                    break;
                case 23:
                    v76->des_ai_prix[num] = 300;
                    v76->des_ai_son[num] = 1;
                    AI_EvalFunc_TEXTGet_C(0xD0002C9E, 45, &v51);
                    v4 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_prix_Dimension1, num);
                    memcpy(v4, &v51, 8u);
                    AI_EvalFunc_TEXTGet_C(0xD0002C9E, 49, &v68);
                    v5 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_noms_Dimension1, num);
                    memcpy(v5, &v68, 8u);
                    AI_EvalFunc_TEXTGet_C(0x72000655, 120, &v82);
                    v6 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_explication_Dimension1, num);
                    memcpy(v6, &v82, 8u);
                    v53.x = 0.0;
                    v53.y = 1.0;
                    v53.z = 0.0;
                    v62.x = 0.0;
                    v62.y = 0.0;
                    v62.z = 1.0;
                    v33 = (tGameObjectMain*)v47;
                    if (v47)
                    {
                        if ((unsigned int)v47 <= 2)
                            v33 = AI_MainActor[(uint32_t)v47 - 1];
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(v33, &v53, &v62);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v53, &v62);
                    }
                    break;
                case 28:
                    v76->des_ai_prix[num] = 1500;
                    v76->des_ai_son[num] = 3;
                    AI_EvalFunc_TEXTGet_C(0xD0002C9E, 47, &v46);
                    v7 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_prix_Dimension1, num);
                    memcpy(v7, &v46, 8u);
                    AI_EvalFunc_TEXTGet_C(0xD0002C9E, 51, &v85);
                    v8 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_noms_Dimension1, num);
                    memcpy(v8, &v85, 8u);
                    AI_EvalFunc_TEXTGet_C(0x72000655, 122, &v65);
                    v9 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_explication_Dimension1, num);
                    memcpy(v9, &v65, 8u);
                    v54.x = 0.0;
                    v54.y = 1.0;
                    v54.z = 0.0;
                    v57.x = 0.0;
                    v57.y = 0.0;
                    v57.z = 1.0;
                    v32 = (tGameObjectMain*)v47;
                    if (v47)
                    {
                        if ((unsigned int)v47 <= 2)
                            v32 = AI_MainActor[(uint32_t)v47 - 1];
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(v32, &v54, &v57);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v54, &v57);
                    }
                    break;
                case 15:
                    v76->des_ai_prix[num] = 150;
                    v76->des_ai_son[num] = 4;
                    AI_EvalFunc_TEXTGet_C(0xD0002C9E, 43, &v43);
                    v10 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_prix_Dimension1, num);
                    memcpy(v10, &v43, 8u);
                    AI_EvalFunc_TEXTGet_C(0xD0002C9E, 59, &v58);
                    v11 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_noms_Dimension1, num);
                    memcpy(v11, &v58, 8u);
                    AI_EvalFunc_TEXTGet_C(0x72000655, 123, &v73);
                    v12 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_explication_Dimension1, num);
                    memcpy(v12, &v73, 8u);
                    *(uint32_t*)AI_EvalVarArray_C2((char*)&v76->__des_f_offset_Z_Dimension1, num) = 0x3DA3D70A;
                    break;
                case 16:
                    v76->des_ai_prix[num] = 300;
                    v76->des_ai_son[num] = 5;
                    AI_EvalFunc_TEXTGet_C(0xD0002C9E, 45, &v64);
                    v13 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_prix_Dimension1, num);
                    memcpy(v13, &v64, 8u);
                    AI_EvalFunc_TEXTGet_C(0xD0002C9E, 60, &v84);
                    v14 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_noms_Dimension1, num);
                    memcpy(v14, &v84, 8u);
                    AI_EvalFunc_TEXTGet_C(0x72000655, 124, &v66);
                    v15 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_explication_Dimension1, num);
                    memcpy(v15, &v66, 8u);
                    *(uint32_t*)AI_EvalVarArray_C2((char*)&v76->__des_f_offset_Z_Dimension1, num) = 0x3DA3D70A;
                    break;
                case 14:
                    v76->des_ai_prix[num] = 1500;
                    v76->des_ai_son[num] = 7;
                    AI_EvalFunc_TEXTGet_C(0xD0002C9E, 47, &v55);
                    v16 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_prix_Dimension1, num);
                    memcpy(v16, &v55, 8u);
                    AI_EvalFunc_TEXTGet_C(0xD0002C9E, 62, &v56);
                    v17 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_noms_Dimension1, num);
                    memcpy(v17, &v56, 8u);
                    AI_EvalFunc_TEXTGet_C(0x72000655, 165, &v72);
                    v18 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_explication_Dimension1, num);
                    memcpy(v18, &v72, 8u);
                    *(uint32_t*)AI_EvalVarArray_C2((char*)&v76->__des_f_offset_Z_Dimension1, num) = 0x3D8F5C29;
                    v44.x = 0.0;
                    v44.y = 1.0;
                    v44.z = 0.0;
                    v79.x = 0.0;
                    v79.y = -0.89999998f;
                    v79.z = 0.5;
                    v31 = (tGameObjectMain*)v47;
                    if (v47)
                    {
                        if ((unsigned int)v47 <= 2)
                            v31 = AI_MainActor[(uint32_t)v47 - 1];
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(v31, &v44, &v79);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v44, &v79);
                    }
                    break;
                case 37:
                    v76->des_ai_prix[num] = 150;
                    v76->des_ai_son[num] = 6;
                    AI_EvalFunc_TEXTGet_C(0xD0002C9E, 43, &v61);
                    v19 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_prix_Dimension1, num);
                    memcpy(v19, &v61, 8u);
                    AI_EvalFunc_TEXTGet_C(0xD0002C9E, 52, &v77);
                    v20 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_noms_Dimension1, num);
                    memcpy(v20, &v77, 8u);
                    AI_EvalFunc_TEXTGet_C(0x72000655, 125, &v60);
                    v21 = AI_EvalVarArray_C(10, (char*)&v76->__des_at_explication_Dimension1, num);
                    memcpy(v21, &v60, 8u);
                    *(uint32_t*)AI_EvalVarArray_C2((char*)&v76->__des_f_offset_Z_Dimension1, num) = 0x3D23D70A;
                    break;
                }
                v30 = (tGameObjectMain*)v76->des_ao_contenu[num];
                if (v30)
                {
                    if ((unsigned int)v30 <= 2)
                        v30 = AI_MainActor[(uint32_t)v30 - 1];
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(v30, 1);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
                }
                v29 = (tGameObjectMain*)v76->des_o_distrib;
                if (v29)
                {
                    if ((unsigned int)v29 <= 2)
                        v29 = AI_MainActor[(uint32_t)v29 - 1];
                    v22 = AI_EvalFunc_ANICanalGaoGet_C(v29, 1u);
                }
                else
                {
                    v22 = AI_EvalFunc_ANICanalGaoGet_C(0, 1u);
                }
                v48 = (unsigned int)v22;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v80);
                v49.x = 0.0;
                v49.y = 0.0;
                v49.z = v76->f_hauteur_disque;
                MATH_VecAddVec(&v83, &v80, &v49);
                v70 = v83;
                v28 = (tGameObjectMain*)v48;
                if (v48)
                {
                    if (v48 <= 2)
                        v28 = AI_MainActor[v48 - 1];
                    AI_EvalFunc_OBJGetPos_C(v28, &a3);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &a3);
                }
                v23 = AI_EvalVarArray_C2((char*)&v76->__des_f_offset_Z_Dimension1, num);
                v70.z = *(float*)v23 + a3.z;
                v70.z = v70.z + 0.1f;
                v76->v_centre_disque = v70;
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v41);
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v50);
                v25 = (float)(2 * num);
                v24 = v25 * 0.52359903f;
                AI_EvalFunc_MATHVecRotate_C(&v41, &v50, v24, &a4);
                v63.x = a4.x;
                v63.y = a4.y;
                v63.z = 0.0;
                AI_EvalFunc_MATHVecSetNormVector_C(&v63, v76->f_rayon_disque);
                v70.x = v70.x + v63.x;
                v70.y = v70.y + v63.y;
                v70.z = v70.z + v63.z;
                v27 = (tGameObjectMain*)v76->des_ao_contenu[num];
                if (v27)
                {
                    if ((unsigned int)v27 <= 2)
                        v27 = AI_MainActor[(uint32_t)v27 - 1];
                    AI_EvalFunc_OBJSetPos_C(v27, &v70);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &v70);
                }
            }
        }
        AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x8000, 0);
        v76->des_f_distance = v76->des_f_distance * v76->des_f_distance;
        v76->angle_de_proposition = v76->angle_de_proposition * 0.017453f;
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_distrib_crunsh_dc_init_distrib_crunsh_dc_reflex);
        result = 1000;
    }
    return result;
}

int distrib_crunsh_dc_bouge()
{
    tGameObjectMain* v0; // eax
    int v1; // eax
    Vec3* v2; // eax
    Vec3* v3; // eax
    Vec3* v4; // eax
    Vec3* v5; // eax
    float a4; // [esp+0h] [ebp-1CCh]
    float a4a; // [esp+0h] [ebp-1CCh]
    float v9; // [esp+8h] [ebp-1C4h]
    float v10; // [esp+Ch] [ebp-1C0h]
    tScriptStruct_DistribCrunsh* v11; // [esp+10h] [ebp-1BCh]
    float v12; // [esp+14h] [ebp-1B8h]
    float v13; // [esp+14h] [ebp-1B8h]
    float v14; // [esp+18h] [ebp-1B4h]
    tGameObjectMain* v15; // [esp+18h] [ebp-1B4h]
    tGameObjectMain* v16; // [esp+20h] [ebp-1ACh]
    tGameObjectMain* v17; // [esp+30h] [ebp-19Ch]
    tGameObjectMain* v18; // [esp+38h] [ebp-194h]
    tGameObjectMain* v19; // [esp+40h] [ebp-18Ch]
    tGameObjectMain* v20; // [esp+48h] [ebp-184h]
    tGameObjectMain* v21; // [esp+90h] [ebp-13Ch]
    tGameObjectMain* v22; // [esp+94h] [ebp-138h]
    tGameObjectMain* v23; // [esp+98h] [ebp-134h]
    tGameObjectMain* v24; // [esp+A0h] [ebp-12Ch]
    tGameObjectMain* v25; // [esp+B4h] [ebp-118h]
    tGameObjectMain* v26; // [esp+B8h] [ebp-114h]
    tGameObjectMain* v27; // [esp+BCh] [ebp-110h]
    tGameObjectMain* v28; // [esp+C4h] [ebp-108h]
    tGameObjectMain* v29; // [esp+CCh] [ebp-100h]
    Vec3 v30; // [esp+D0h] [ebp-FCh] BYREF
    float v31; // [esp+DCh] [ebp-F0h]
    Vec3 a3; // [esp+E0h] [ebp-ECh] BYREF
    Vec3 a2; // [esp+ECh] [ebp-E0h] BYREF
    Vec3 v34; // [esp+FCh] [ebp-D0h] BYREF
    int v35; // [esp+108h] [ebp-C4h]
    Vec3 v36; // [esp+10Ch] [ebp-C0h] BYREF
    Vec3 v37; // [esp+118h] [ebp-B4h] BYREF
    Vec3 v38; // [esp+128h] [ebp-A4h]
    float v39; // [esp+134h] [ebp-98h]
    Vec3 v40; // [esp+138h] [ebp-94h] BYREF
    Vec3 v41; // [esp+144h] [ebp-88h] BYREF
    unsigned int v42; // [esp+150h] [ebp-7Ch]
    Vec3 a5; // [esp+160h] [ebp-6Ch] BYREF
    Vec3 v44; // [esp+16Ch] [ebp-60h]
    Vec3 v45; // [esp+178h] [ebp-54h] BYREF
    int i; // [esp+184h] [ebp-48h]
    Vec3 v47; // [esp+198h] [ebp-34h] BYREF
    int v48; // [esp+1A4h] [ebp-28h]
    tScriptStruct_DistribCrunsh* v49; // [esp+1A8h] [ebp-24h]
    Vec3 v50; // [esp+1ACh] [ebp-20h] BYREF
    Vec3 a1; // [esp+1B8h] [ebp-14h] BYREF
    float v52; // [esp+1C4h] [ebp-8h]
    float v53; // [esp+1C8h] [ebp-4h]

    v44.x = 1.0;
    v44.y = 0.0;
    v44.z = 0.0;
    v38.x = 1.0;
    v38.y = 0.0;
    v38.z = 0.0;
    a2.x = 1.0;
    a2.y = 0.0;
    a2.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v11 = (tScriptStruct_DistribCrunsh*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v11 = 0;
    v49 = v11;
    v52 = 0.0;
    v39 = 0.0;
    v35 = 0;
    i = 0;
    v42 = 0;
    v29 = (tGameObjectMain*)v11->des_o_distrib;
    if (v29)
    {
        if ((unsigned int)v29 <= 2)
            v29 = AI_MainActor[(uint32_t)v29 - 1];
        v0 = AI_EvalFunc_ANICanalGaoGet_C(v29, 1u);
    }
    else
    {
        v0 = AI_EvalFunc_ANICanalGaoGet_C(0, 1u);
    }
    v42 = (unsigned int)v0;
    v35 = 0;
    if (v49->i_init_bouge)
    {
        v49->i_last_snd = -1;
        v28 = (tGameObjectMain*)v49->des_o_distrib;
        if (v28)
        {
            if ((unsigned int)v28 <= 2)
                v28 = AI_MainActor[(uint32_t)v28 - 1];
            v1 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(v28, 2, -1.0, 0.94999999f, 1.05f);
        }
        else
        {
            v1 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(0, 2, -1.0, 0.94999999f, 1.05f);
        }
        v48 = v1;
        v49->i_force_expl = 1;
        v49->i_mode_texte = 0;
        v49->f_time_texte = TIM_gf_MainClock - TIM_gf_LockTime;
        for (i = 0; i < 6; ++i)
        {
            v27 = (tGameObjectMain*)v49->des_ao_contenu[i];
            if (v27)
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
                v25 = (tGameObjectMain*)v49->des_ao_contenu[i];
                if (v25)
                {
                    if ((unsigned int)v25 <= 2)
                        v25 = AI_MainActor[(uint32_t)v25 - 1];
                    AI_EvalFunc_OBJGetPos_C(v25, &a3);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &a3);
                }
                a2 = a3;
                a2.x = a3.x - v49->v_centre_disque.x;
                a2.y = a3.y - v49->v_centre_disque.y;
                a2.z = a3.z - v49->v_centre_disque.z;
                AI_EvalFunc_MATHVecSetHorzNormalize_C(&a2);
                AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v30);
                v2 = (Vec3*)AI_EvalVarArray_C3((char*)&v49->__av_dest_Dimension1, i);
                v10 = (float)v49->i_bouge_sens;
                v12 = 1.0471981f * v10;
                AI_EvalFunc_MATHVecRotate_C(&a2, &v30, v12, v2);
                v14 = v49->f_rayon_disque;
                v3 = (Vec3*)AI_EvalVarArray_C3((char*)&v49->__av_dest_Dimension1, i);
                AI_EvalFunc_MATHVecSetNormVector_C(v3, v14);
            }
        }
        v49->i_current -= v49->i_bouge_sens;
        if (v49->i_current < 0)
            v49->i_current = 5;
        if (v49->i_current == 6)
            v49->i_current = 0;
        v49->i_init_bouge = 0;
        v24 = (tGameObjectMain*)v42;
        if (v42)
        {
            if (v42 <= 2)
                v24 = AI_MainActor[v42 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v24, &a1);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &a1);
        }
        v34.x = 0.0;
        v34.y = 0.0;
        v34.z = 1.0;
        v9 = (float)v49->i_bouge_sens;
        v13 = 1.0471981f * v9;
        AI_EvalFunc_MATHVecRotate_C(&a1, &v34, v13, &v49->v_rot_plateau);
    }
    for (i = 0; i < 6; ++i)
    {
        v23 = (tGameObjectMain*)v49->des_ao_contenu[i];
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
            v21 = (tGameObjectMain*)v49->des_ao_contenu[i];
            if (v21)
            {
                if ((unsigned int)v21 <= 2)
                    v21 = AI_MainActor[(uint32_t)v21 - 1];
                AI_EvalFunc_OBJGetPos_C(v21, &v50);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v50);
            }
            a2 = v50;
            v39 = v50.z;
            a2.x = v50.x - v49->v_centre_disque.x;
            a2.y = v50.y - v49->v_centre_disque.y;
            a2.z = v50.z - v49->v_centre_disque.z;
            if (v35)
            {
                a2 = *(Vec3*)AI_EvalVarArray_C3((char*)&v49->__av_dest_Dimension1, i);
            }
            else
            {
                AI_EvalFunc_MATHVecNormalize_C(&a2, &v36);
                v38 = v36;
                v4 = (Vec3*)AI_EvalVarArray_C3((char*)&v49->__av_dest_Dimension1, i);
                AI_EvalFunc_MATHVecNormalize_C(v4, &v45);
                v44 = v45;
                v52 = v45.z * v38.z + v45.y * v38.y + v38.x * v45.x;
                if (v52 <= 0.99900001)
                {
                    v53 = TIM_gf_dt;
                    a4 = 6.0f * TIM_gf_dt;
                    v5 = (Vec3*)AI_EvalVarArray_C3((char*)&v49->__av_dest_Dimension1, i);
                    AI_EvalFunc_MATHVecBlendRotate_C(&a2, v5, a4, &a5);
                    a2 = a5;
                }
                else
                {
                    a2 = *(Vec3*)AI_EvalVarArray_C3((char*)&v49->__av_dest_Dimension1, i);
                    v35 = 1;
                }
            }
            AI_EvalFunc_MATHVecSetNormVector_C(&a2, v49->f_rayon_disque);
            a2.x = a2.x + v49->v_centre_disque.x;
            a2.y = a2.y + v49->v_centre_disque.y;
            a2.z = v39;
            v20 = (tGameObjectMain*)v49->des_ao_contenu[i];
            if (v20)
            {
                if ((unsigned int)v20 <= 2)
                    v20 = AI_MainActor[(uint32_t)v20 - 1];
                AI_EvalFunc_OBJSetPos_C(v20, &a2);
            }
            else
            {
                AI_EvalFunc_OBJSetPos_C(0, &a2);
            }
        }
    }
    if (v35)
    {
        v19 = (tGameObjectMain*)v42;
        if (v42)
        {
            if (v42 <= 2)
                v19 = AI_MainActor[v42 - 1];
            AI_EvalFunc_OBJ_BankingGet_C(v19, &v37);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGet_C(0, &v37);
        }
        v18 = (tGameObjectMain*)v42;
        if (v42)
        {
            if (v42 <= 2)
                v18 = AI_MainActor[v42 - 1];
            AI_EvalFunc_OBJ_BankingGeneralSet_C(v18, &v49->v_rot_plateau, &v37);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &v49->v_rot_plateau, &v37);
        }
    }
    else
    {
        v17 = (tGameObjectMain*)v42;
        if (v42)
        {
            if (v42 <= 2)
                v17 = AI_MainActor[v42 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v17, &v41);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &v41);
        }
        v31 = TIM_gf_dt;
        a4a = 6.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&v41, &v49->v_rot_plateau, a4a, &v40);
        a2 = v40;
        v16 = (tGameObjectMain*)v42;
        if (v42)
        {
            if (v42 <= 2)
                v16 = AI_MainActor[v42 - 1];
            AI_EvalFunc_OBJ_BankingGet_C(v16, &v47);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGet_C(0, &v47);
        }
        v15 = (tGameObjectMain*)v42;
        if (v42)
        {
            if (v42 <= 2)
                v15 = AI_MainActor[v42 - 1];
            AI_EvalFunc_OBJ_BankingGeneralSet_C(v15, &a2, &v47);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGeneralSet_C(0, &a2, &v47);
        }
    }
    if (v35)
    {
        v49->i_bouge_sens = 0;
        AI_EvalFunc_AITrackCurStop_C();
    }
    return 1000;
}

int distrib_crunsh_dc_deroule()
{
    int result; // eax
    unsigned int v1; // eax
    int v2; // eax
    tGameObjectMain* v3; // eax
    int v4; // eax
    tGameObjectMain* v5; // eax
    tGameObjectMain* v6; // eax
    unsigned int v7; // eax
    int v8; // eax
    int v9; // eax
    tAINode* v10; // [esp+0h] [ebp-228h]
    tScriptStruct_DistribCrunsh* v11; // [esp+4h] [ebp-224h]
    tGameObjectMain* v12; // [esp+Ch] [ebp-21Ch]
    tGameObjectMain* v13; // [esp+1Ch] [ebp-20Ch]
    tGameObjectMain* v14; // [esp+24h] [ebp-204h]
    tGameObjectMain* v15; // [esp+3Ch] [ebp-1ECh]
    tGameObjectMain* v16; // [esp+44h] [ebp-1E4h]
    tGameObjectMain* v17; // [esp+4Ch] [ebp-1DCh]
    tGameObjectMain* v18; // [esp+54h] [ebp-1D4h]
    tGameObjectMain* v19; // [esp+64h] [ebp-1C4h]
    tGameObjectMain* v20; // [esp+6Ch] [ebp-1BCh]
    tGameObjectMain* v21; // [esp+74h] [ebp-1B4h]
    tGameObjectMain* v22; // [esp+7Ch] [ebp-1ACh]
    tGameObjectMain* v23; // [esp+8Ch] [ebp-19Ch]
    tGameObjectMain* v24; // [esp+94h] [ebp-194h]
    tGameObjectMain* v25; // [esp+9Ch] [ebp-18Ch]
    tGameObjectMain* v26; // [esp+A4h] [ebp-184h]
    tGameObjectMain* v27; // [esp+ACh] [ebp-17Ch]
    tGameObjectMain* v28; // [esp+B4h] [ebp-174h]
    tGameObjectMain* v29; // [esp+B8h] [ebp-170h]
    tGameObjectMain* v30; // [esp+BCh] [ebp-16Ch]
    void* Src; // [esp+C0h] [ebp-168h]
    tMessage v32; // [esp+D4h] [ebp-154h] BYREF
    Vec3 a3; // [esp+10Ch] [ebp-11Ch] BYREF
    tMessage v35; // [esp+118h] [ebp-110h] BYREF
    int v36; // [esp+158h] [ebp-D0h]
    int v37; // [esp+168h] [ebp-C0h]
    unsigned int v38; // [esp+16Ch] [ebp-BCh]
    Vec3 v39; // [esp+170h] [ebp-B8h] BYREF
    int v40; // [esp+17Ch] [ebp-ACh]
    Vec3 v41; // [esp+180h] [ebp-A8h] BYREF
    Vec3 v42; // [esp+18Ch] [ebp-9Ch]
    int v43; // [esp+198h] [ebp-90h]
    unsigned int v44; // [esp+19Ch] [ebp-8Ch]
    Vec3 v45; // [esp+1A0h] [ebp-88h] BYREF
    int v46; // [esp+1ACh] [ebp-7Ch]
    BOOL v47; // [esp+1B4h] [ebp-74h]
    float v48; // [esp+1BCh] [ebp-6Ch]
    int v49; // [esp+1C0h] [ebp-68h]
    int v50; // [esp+1C4h] [ebp-64h]
    int v51; // [esp+1D4h] [ebp-54h]
    Vec3 v52; // [esp+1D8h] [ebp-50h] BYREF
    int v53; // [esp+1E4h] [ebp-44h]
    int v54; // [esp+1E8h] [ebp-40h]
    tScriptStruct_DistribCrunsh* v55; // [esp+1FCh] [ebp-2Ch]
    int v57; // [esp+214h] [ebp-14h]
    int v58; // [esp+218h] [ebp-10h]
    unsigned int v59; // [esp+21Ch] [ebp-Ch]
    unsigned int v60; // [esp+224h] [ebp-4h]

    v42.x = 1.0;
    v42.y = 0.0;
    v42.z = 0.0;
    v52.x = 1.0;
    v52.y = 0.0;
    v52.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v11 = (tScriptStruct_DistribCrunsh*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v11 = 0;
    v55 = v11;
    v54 = 0;
    v59 = 0;
    v47 = 0;
    v38 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;        
        distrib_crunsh_dc_deroule_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_deroule_GST_Global_gao_i_freeze_joy,
            (void*)distrib_crunsh_dc_deroule_GST_Global_gao,
            4864);
        distrib_crunsh_dc_deroule_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_deroule_GST_Board_panel_gao_i_sally_priority,
            (void*)distrib_crunsh_dc_deroule_GST_Board_panel_gao,
            396);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_deroule_GST_Board_panel_gao_o_sally_contextuel,
            (void*)distrib_crunsh_dc_deroule_GST_Board_panel_gao,
            388);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_deroule_GST_Board_panel_gao_i_sally_action,
            (void*)distrib_crunsh_dc_deroule_GST_Board_panel_gao,
            412);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_deroule_GST_Board_panel_gao_v_sally_offset,
            (void*)distrib_crunsh_dc_deroule_GST_Board_panel_gao,
            400);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_deroule_GST_Board_panel_gao_i_force_interface,
            (void*)distrib_crunsh_dc_deroule_GST_Board_panel_gao,
            3292);
        fct_distrib_crunsh_dc_deroule_distrib_crunsh_dc_exec_cam = (int)AI_ul_RealResolveOneRefFunction(0x3D001D5Du);
        distrib_crunsh_dc_deroule__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_deroule__PJ_Sally_gao_current_pm_mode,
            (void*)distrib_crunsh_dc_deroule__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_deroule__PJ_Sally_gao_current_pm_priority,
            (void*)distrib_crunsh_dc_deroule__PJ_Sally_gao,
            576);
        distrib_crunsh_dc_deroule_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_deroule_univVars_f_sally_fric,
            (void*)distrib_crunsh_dc_deroule_univVars,
            1200);
        distrib_crunsh_dc_deroule_TXT_Sally_gao = AI_ul_RealResolveOneRefObject(0x7200064Eu);
        fct_distrib_crunsh_dc_deroule_distrib_crunsh_dc_bouge = (int)AI_ul_RealResolveOneRefFunction(0x3D001D54u);
        distrib_crunsh_dc_deroule__PJ_Camera_gao = AI_ul_RealResolveOneRefObject(0x71003F56u);
        distrib_crunsh_dc_deroule__PJ_Camera_Target_gao = AI_ul_RealResolveOneRefObject(0x35006499u);
        distrib_crunsh_dc_deroule__PJ_Camera_Derriere_gao = AI_ul_RealResolveOneRefObject(0x5E005FA2u);
        fct_distrib_crunsh_dc_deroule_distrib_crunsh_dc_reflex = (int)AI_ul_RealResolveOneRefFunction(0x3D001D4Du);
    }
    v10 = *AI_gppst_CurrentJumpNode;
    if (*AI_gppst_CurrentJumpNode == (tAINode*)11)
    {
    LABEL_61:
        if (AI_EvalKeyword_During_C())
        {
            v36 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_distrib_crunsh_dc_deroule_distrib_crunsh_dc_exec_cam);
            if (v36 != 1000)
                return v36;
            AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)distrib_crunsh_dc_deroule_GST_Global_gao, 0x8000, 0);
            AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)distrib_crunsh_dc_deroule_GST_Board_panel_gao, 0x200, 0);
            v24 = (tGameObjectMain*)v55->des_o_distrib;
            if (v24)
            {
                if ((unsigned int)v24 <= 2)
                    v24 = AI_MainActor[(uint32_t)v24 - 1];
                v3 = AI_EvalFunc_ANICanalGaoGet_C(v24, 2u);
            }
            else
            {
                v3 = AI_EvalFunc_ANICanalGaoGet_C(0, 2u);
            }
            v59 = (unsigned int)v3;
            v23 = v3;
            if (v3)
            {
                if ((unsigned int)v3 <= 2)
                    v23 = AI_MainActor[(uint32_t)v3 - 1];
                AI_EvalFunc_OBJGetPos_C(v23, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            v52.x = a3.x;
            v52.y = a3.y;
            v52.z = a3.z - TIM_gf_dt;
            v22 = (tGameObjectMain*)v59;
            if (v59)
            {
                if (v59 <= 2)
                    v22 = AI_MainActor[v59 - 1];
                AI_EvalFunc_OBJSetPos_C(v22, &v52);
            }
            else
            {
                AI_EvalFunc_OBJSetPos_C(0, &v52);
            }
            *AI_gppst_CurrentJumpNode = (tAINode*)11;
            return 0;
        }
        SND_Release(v55->i_snd);
        v55->i_snd = -1;
        AI_gpst_CurrentInstance->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -0.5;
        goto LABEL_81;
    }
    if (v10 == (tAINode*)13)
    {
    LABEL_81:
        if (AI_EvalKeyword_During_C())
        {
            v46 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_distrib_crunsh_dc_deroule_distrib_crunsh_dc_exec_cam);
            if (v46 != 1000)
                return v46;
            AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)distrib_crunsh_dc_deroule_GST_Global_gao, 0x8000, 0);
            AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)distrib_crunsh_dc_deroule_GST_Board_panel_gao, 0x200, 0);
            *AI_gppst_CurrentJumpNode = (tAINode*)13;
            return 0;
        }
        v21 = (tGameObjectMain*)v55->des_o_distrib;
        if (v21)
        {
            if ((unsigned int)v21 <= 2)
                v21 = AI_MainActor[(uint32_t)v21 - 1];
            v4 = AI_EvalFunc_SNDRequest_C(v21, 1, 0x18000000);
        }
        else
        {
            v4 = AI_EvalFunc_SNDRequest_C(0, 1, 0x18000000);
        }
        v55->i_snd = v4;
        AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v55->i_snd);
        AI_gpst_CurrentInstance->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -0.5;
        goto LABEL_91;
    }
    if (v10 != (tAINode*)15)
    {
        *(uint32_t*)distrib_crunsh_dc_deroule_GST_Global_gao_i_freeze_joy = 1;
        AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)distrib_crunsh_dc_deroule_GST_Board_panel_gao, 0x200, 0);
        AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)distrib_crunsh_dc_deroule_GST_Global_gao, 0x8000, 0);
        if (*(float*)distrib_crunsh_dc_deroule_GST_Board_panel_gao_i_sally_priority < 20.0)
        {
            *(uint32_t*)distrib_crunsh_dc_deroule_GST_Board_panel_gao_o_sally_contextuel = (uint32_t)AI_gpst_CurrentGameObject;
            *(uint32_t*)distrib_crunsh_dc_deroule_GST_Board_panel_gao_i_sally_priority = 0x41A00000;
            *(uint32_t*)distrib_crunsh_dc_deroule_GST_Board_panel_gao_i_sally_action = -10;
            *(uint32_t*)distrib_crunsh_dc_deroule_GST_Board_panel_gao_v_sally_offset = 0;
            *(uint32_t*)(distrib_crunsh_dc_deroule_GST_Board_panel_gao_v_sally_offset + 4) = 0;
            *(uint32_t*)(distrib_crunsh_dc_deroule_GST_Board_panel_gao_v_sally_offset + 8) = 0x3F800000;
            *(uint32_t*)distrib_crunsh_dc_deroule_GST_Board_panel_gao_i_force_interface = 1;
        }
        v51 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_distrib_crunsh_dc_deroule_distrib_crunsh_dc_exec_cam);
        if (v51 != 1000)
            return v51;
        if (v55->i_bouge_sens)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v32.msgID = 0;
        if (AI_EvalFunc_MSGGetCount_C(AI_gpst_CurrentGameObject))
        {
            Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
            memcpy(&v32, Src, sizeof(v32));
        }
        v38 = v55->des_ao_contenu[v55->i_current];
        v30 = (tGameObjectMain*)v38;
        if (v38)
        {
            if (v38 <= 2)
                v30 = AI_MainActor[v38 - 1];
            v29 = v30;
        }
        else
        {
            v29 = 0;
        }
        if (!v29)
            goto LABEL_138;
        v28 = (tGameObjectMain*)v38;
        if (v38)
        {
            if (v38 <= 2)
                v28 = AI_MainActor[v38 - 1];
            v1 = AI_EvalFunc_OBJ_FlagsControlGet_C(v28);
        }
        else
        {
            v1 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
        }
        v44 = v1;
        if ((v1 & 0x200000) != 0)
            goto LABEL_138;
        v58 = (unsigned __int16)*(uint32_t*)(distrib_crunsh_dc_deroule__PJ_Sally_gao + 4);
        if ((v58 & 1) != 0)
            v47 = !*(uint32_t*)distrib_crunsh_dc_deroule__PJ_Sally_gao_current_pm_mode
            || *(int*)distrib_crunsh_dc_deroule__PJ_Sally_gao_current_pm_priority < 20;
        else
            v47 = 0;
        if (v47 && *(float*)distrib_crunsh_dc_deroule_GST_Board_panel_gao_i_sally_priority < 25.0)
        {
            *(uint32_t*)distrib_crunsh_dc_deroule_GST_Board_panel_gao_o_sally_contextuel = (uint32_t)AI_gpst_CurrentGameObject;
            *(uint32_t*)distrib_crunsh_dc_deroule_GST_Board_panel_gao_i_sally_priority = 0x41C80000;
            *(uint32_t*)distrib_crunsh_dc_deroule_GST_Board_panel_gao_i_sally_action = 6;
            *(uint32_t*)distrib_crunsh_dc_deroule_GST_Board_panel_gao_v_sally_offset = 0;
            *(uint32_t*)(distrib_crunsh_dc_deroule_GST_Board_panel_gao_v_sally_offset + 4) = 0;
            *(uint32_t*)(distrib_crunsh_dc_deroule_GST_Board_panel_gao_v_sally_offset + 8) = 0;
            *(uint32_t*)distrib_crunsh_dc_deroule_GST_Board_panel_gao_i_force_interface = 1;
        }
        if (v32.msgID != 10 || v32.f2 != 25.0f)
            goto LABEL_138;
        if (*(int32_t*)distrib_crunsh_dc_deroule_univVars_f_sally_fric < v55->des_ai_prix[v55->i_current])
        {
            v13 = (tGameObjectMain*)v55->des_o_distrib;
            if (v13)
            {
                if ((unsigned int)v13 <= 2)
                    v13 = AI_MainActor[(uint32_t)v13 - 1];
                v8 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(v13, 4, -1.0, 0.94999999f, 1.05f);
            }
            else
            {
                v8 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(0, 4, -1.0, 0.94999999f, 1.05f);
            }
            v37 = v8;
            v53 = AI_EvalFunc_MATHRandInt_C(0, 3);
            v54 = v53 + 61;
            v35.msgID = 30;
            v35.Int1 = v53 + 61;
            v35.Int2 = 0;
            v35.f2 = 1.0;
            v35.v1.x = 0.0;
            v35.v1.y = 0.0;
            v35.v1.z = 0.0;
            v35.v2.x = 0.0;
            v35.v2.y = 0.0;
            v35.v2.z = 0.0;
            v35.gao = 0;
            v35.gao2 = 0;
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)distrib_crunsh_dc_deroule_TXT_Sally_gao, &v35);
            goto LABEL_138;
        }
        v27 = (tGameObjectMain*)v55->des_ao_contenu[v55->i_current];
        if (v27)
        {
            if ((unsigned int)v27 <= 2)
                v27 = AI_MainActor[(uint32_t)v27 - 1];
            AI_EvalFunc_OBJ_FlagInactiveSet_C(v27, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
        }
        v32.msgID = 10;
        v26 = (tGameObjectMain*)v55->des_ao_contenu[v55->i_current];
        if (v26)
        {
            if ((unsigned int)v26 <= 2)
                v26 = AI_MainActor[(uint32_t)v26 - 1];
            AI_EvalFunc_MSGSend_C(v26, &v32);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &v32);
        }
        v25 = (tGameObjectMain*)v55->des_o_distrib;
        if (v25)
        {
            if ((unsigned int)v25 <= 2)
                v25 = AI_MainActor[(uint32_t)v25 - 1];
            v2 = AI_EvalFunc_SNDRequest_C(v25, 1, 0x18000000);
        }
        else
        {
            v2 = AI_EvalFunc_SNDRequest_C(0, 1, 0x18000000);
        }
        v55->i_snd = v2;
        AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v55->i_snd);
        AI_gpst_CurrentInstance->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -0.5;
        goto LABEL_61;
    }
LABEL_91:
    if (AI_EvalKeyword_During_C())
    {
        v40 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_distrib_crunsh_dc_deroule_distrib_crunsh_dc_exec_cam);
        if (v40 != 1000)
            return v40;
        AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)distrib_crunsh_dc_deroule_GST_Global_gao, 0x8000, 0);
        AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)distrib_crunsh_dc_deroule_GST_Board_panel_gao, 0x200, 0);
        v20 = (tGameObjectMain*)v55->des_o_distrib;
        if (v20)
        {
            if ((unsigned int)v20 <= 2)
                v20 = AI_MainActor[(uint32_t)v20 - 1];
            v5 = AI_EvalFunc_ANICanalGaoGet_C(v20, 2u);
        }
        else
        {
            v5 = AI_EvalFunc_ANICanalGaoGet_C(0, 2u);
        }
        v59 = (unsigned int)v5;
        v19 = v5;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v19 = AI_MainActor[(uint32_t)v5 - 1];
            AI_EvalFunc_OBJGetPos_C(v19, &v39);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v39);
        }
        v52.x = v39.x;
        v52.y = v39.y;
        v52.z = v39.z + TIM_gf_dt;
        v18 = (tGameObjectMain*)v59;
        if (v59)
        {
            if (v59 <= 2)
                v18 = AI_MainActor[v59 - 1];
            AI_EvalFunc_OBJSetPos_C(v18, &v52);
        }
        else
        {
            AI_EvalFunc_OBJSetPos_C(0, &v52);
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)15;
        return 0;
    }
    SND_Release(v55->i_snd);
    v55->i_snd = -1;
    v17 = (tGameObjectMain*)v55->des_o_distrib;
    if (v17)
    {
        if ((unsigned int)v17 <= 2)
            v17 = AI_MainActor[(uint32_t)v17 - 1];
        v6 = AI_EvalFunc_ANICanalGaoGet_C(v17, 2u);
    }
    else
    {
        v6 = AI_EvalFunc_ANICanalGaoGet_C(0, 2u);
    }
    v59 = (unsigned int)v6;
    v16 = v6;
    if (v6)
    {
        if ((unsigned int)v6 <= 2)
            v16 = AI_MainActor[(uint32_t)v6 - 1];
        OBJ_RestoreInitialPos(v16);
    }
    else
    {
        OBJ_RestoreInitialPos(0);
    }
    v15 = (tGameObjectMain*)v55->des_ao_contenu[v55->i_current];
    if (v15)
    {
        if ((unsigned int)v15 <= 2)
            v15 = AI_MainActor[(uint32_t)v15 - 1];
        v7 = AI_EvalFunc_OBJ_FlagsControlGet_C(v15);
    }
    else
    {
        v7 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
    }
    v60 = v7;
    if ((v7 & 0x200000) != 0)
    {
        *(uint32_t*)distrib_crunsh_dc_deroule_univVars_f_sally_fric -= v55->des_ai_prix[v55->i_current];
        v48 = (float)v55->des_ai_prix[v55->i_current];
        v49 = 0;
        v50 = 0;
        v52.x = v48;
        v52.y = 0.0;
        v52.z = 0.0;
        v35.msgID = 30;
        v35.Int1 = 49;
        v35.Int2 = 0;
        v35.f2 = 1.5;
        v35.v1.x = 0.0;
        v35.v1.y = 0.0;
        v35.v1.z = 0.0;
        v35.v2.x = v48;
        v35.v2.y = 0.0;
        v35.v2.z = 0.0;
        v35.gao = 0;
        v35.gao2 = 0;
        AI_EvalFunc_MSGSend_C((tGameObjectMain*)distrib_crunsh_dc_deroule_TXT_Sally_gao, &v35);
    }
    else
    {
        v14 = (tGameObjectMain*)v55->des_ao_contenu[v55->i_current];
        if (v14)
        {
            if ((unsigned int)v14 <= 2)
                v14 = AI_MainActor[(uint32_t)v14 - 1];
            AI_EvalFunc_OBJ_FlagInactiveSet_C(v14, 1);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
        }
    }
LABEL_138:
    AI_EvalFunc_IoJoyGetMove_C(&v41);
    v42 = v41;
    if (v41.x < -0.5)
    {
        v55->i_init_bouge = 1;
        v55->i_bouge_sens = -1;
        AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_distrib_crunsh_dc_deroule_distrib_crunsh_dc_bouge);
    }
    if (v42.x > 0.5)
    {
        v55->i_init_bouge = 1;
        v55->i_bouge_sens = 1;
        AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_distrib_crunsh_dc_deroule_distrib_crunsh_dc_bouge);
    }
    if (!AI_EvalFunc_IoButtonJustPressed_C(3))
        goto LABEL_150;
    v12 = (tGameObjectMain*)v55->des_o_distrib;
    if (v12)
    {
        if ((unsigned int)v12 <= 2)
            v12 = AI_MainActor[(uint32_t)v12 - 1];
        v9 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(v12, 5, -1.0, 0.94999999f, 1.05f);
    }
    else
    {
        v9 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(0, 5, -1.0, 0.94999999f, 1.05f);
    }
    v43 = v9;
    AI_EvalFunc_OBJSetPos_C((tGameObjectMain*)distrib_crunsh_dc_deroule__PJ_Camera_gao, &v55->v_last_pos_cam);
    v45.x = 0.0;
    v45.y = 0.0;
    v45.z = 1.0;
    AI_EvalFunc_OBJ_SightGeneralSet_C(
        (tGameObjectMain*)distrib_crunsh_dc_deroule__PJ_Camera_gao,
        &v55->v_last_sight_cam,
        &v45);
    AI_EvalFunc_OBJ_FlagInactiveSet_C((tGameObjectMain*)distrib_crunsh_dc_deroule__PJ_Camera_gao, 0);
    AI_EvalFunc_OBJ_FlagInactiveSet_C((tGameObjectMain*)distrib_crunsh_dc_deroule__PJ_Camera_Target_gao, 0);
    AI_EvalFunc_OBJ_FlagInactiveSet_C((tGameObjectMain*)distrib_crunsh_dc_deroule__PJ_Camera_Derriere_gao, 0);
    STR_DelChar((tGameObjectMain*)distrib_crunsh_dc_deroule_GST_Board_panel_gao, 1, 0, -1);
    *(uint32_t*)distrib_crunsh_dc_deroule_GST_Global_gao_i_freeze_joy = 0;
    v57 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_distrib_crunsh_dc_deroule_distrib_crunsh_dc_reflex);
    if (v57 == 1000)
        LABEL_150:
    result = 1000;
    else
        result = v57;
    return result;
}

int distrib_crunsh_dc_texte()
{
    double v0; // st7
    tGameObjectMain* v2; // eax
    unsigned int v3; // eax
    tTextGet* v4; // eax
    char* v5; // eax
    int v6; // eax
    tTextGet* v7; // eax
    char* v8; // eax
    char* v9; // eax
    tScriptStruct_DistribCrunsh* v10; // [esp+8h] [ebp-FCh]
    tGameObjectMain* v11; // [esp+10h] [ebp-F4h]
    tGameObjectMain* v12; // [esp+18h] [ebp-ECh]
    tGameObjectMain* v13; // [esp+1Ch] [ebp-E8h]
    tGameObjectMain* v14; // [esp+20h] [ebp-E4h]
    tGameObjectMain* v15; // [esp+28h] [ebp-DCh]
    tGameObjectMain* v16; // [esp+30h] [ebp-D4h]
    tGameObjectMain* v17; // [esp+34h] [ebp-D0h]
    tGameObjectMain* v18; // [esp+38h] [ebp-CCh]
    tGameObjectMain* v19; // [esp+40h] [ebp-C4h]
    tGameObjectMain* v20; // [esp+48h] [ebp-BCh]
    tGameObjectMain* v21; // [esp+50h] [ebp-B4h]
    tGameObjectMain* v22; // [esp+54h] [ebp-B0h]
    tGameObjectMain* v23; // [esp+58h] [ebp-ACh]
    tGameObjectMain* v24; // [esp+60h] [ebp-A4h]
    tGameObjectMain* v25; // [esp+68h] [ebp-9Ch]
    unsigned int v26; // [esp+70h] [ebp-94h]
    tMessage Src; // [esp+74h] [ebp-90h] BYREF
    unsigned int v28; // [esp+A8h] [ebp-5Ch]
    unsigned int v29; // [esp+ACh] [ebp-58h]
    tGameObjectMain* v31; // [esp+C4h] [ebp-40h]
    float v32; // [esp+CCh] [ebp-38h]
    int v33; // [esp+D0h] [ebp-34h]
    int v35; // [esp+D8h] [ebp-2Ch]
    float v36; // [esp+E8h] [ebp-1Ch]
    tScriptStruct_DistribCrunsh* v37; // [esp+ECh] [ebp-18h]

    if (AI_gpst_CurrentInstance)
        v10 = (tScriptStruct_DistribCrunsh*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v10 = 0;
    v37 = v10;
    v35 = 0;
    v29 = 0;
    v32 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        distrib_crunsh_dc_texte_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_texte_GST_Board_panel_gao_i_commerce_tune,
            (void*)distrib_crunsh_dc_texte_GST_Board_panel_gao,
            2212);
        distrib_crunsh_dc_texte_GST_AffichageTexte_gao = AI_ul_RealResolveOneRefObject(0x7100E7ACu);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_texte_GST_AffichageTexte_gao_texte_message_a_traiter,
            (void*)distrib_crunsh_dc_texte_GST_AffichageTexte_gao,
            52);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_texte_GST_AffichageTexte_gao_i_Sound_ID,
            (void*)distrib_crunsh_dc_texte_GST_AffichageTexte_gao,
            64);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_texte_GST_AffichageTexte_gao_o_Sound_Localisation3D,
            (void*)distrib_crunsh_dc_texte_GST_AffichageTexte_gao,
            68);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_texte_GST_AffichageTexte_gao_f_Sound_Volume,
            (void*)distrib_crunsh_dc_texte_GST_AffichageTexte_gao,
            72);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_texte_GST_AffichageTexte_gao_f_near,
            (void*)distrib_crunsh_dc_texte_GST_AffichageTexte_gao,
            220);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_texte_GST_AffichageTexte_gao_f_far,
            (void*)distrib_crunsh_dc_texte_GST_AffichageTexte_gao,
            224);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_texte_GST_AffichageTexte_gao_message_a_traiter,
            (void*)distrib_crunsh_dc_texte_GST_AffichageTexte_gao,
            0);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_texte_GST_AffichageTexte_gao_prio_message_a_traiter,
            (void*)distrib_crunsh_dc_texte_GST_AffichageTexte_gao,
            60);
    }
    v31 = *AI_MainActor;
    v25 = v31;
    if (v31)
    {
        if ((unsigned int)v31 <= 2)
            v25 = AI_MainActor[(uint32_t)v31 - 1];
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v25);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v36 = (float)v0;
    v32 = v36;
    if (v36 < 9.0)
        *(uint32_t*)distrib_crunsh_dc_texte_GST_Board_panel_gao_i_commerce_tune = 1;
    if (v32 > 100.0)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    v24 = (tGameObjectMain*)v37->des_o_distrib;
    if (v24)
    {
        if ((unsigned int)v24 <= 2)
            v24 = AI_MainActor[(uint32_t)v24 - 1];
        v2 = AI_EvalFunc_ANICanalGaoGet_C(v24, 3u);
    }
    else
    {
        v2 = AI_EvalFunc_ANICanalGaoGet_C(0, 3u);
    }
    v26 = (unsigned int)v2;
    v29 = v37->des_ao_contenu[v37->i_current];
    v23 = (tGameObjectMain*)v29;
    if (v29)
    {
        if (v29 <= 2)
            v23 = AI_MainActor[v29 - 1];
        v22 = v23;
    }
    else
    {
        v22 = 0;
    }
    if (!v22)
        goto LABEL_80;
    v21 = (tGameObjectMain*)v29;
    if (v29)
    {
        if (v29 <= 2)
            v21 = AI_MainActor[v29 - 1];
        v3 = AI_EvalFunc_OBJ_FlagsControlGet_C(v21);
    }
    else
    {
        v3 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
    }
    v28 = v3;
    if ((v3 & 0x200000) != 0)
    {
    LABEL_80:
        v11 = (tGameObjectMain*)v26;
        if (v26)
        {
            if (v26 <= 2)
                v11 = AI_MainActor[v26 - 1];
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(v11, 1);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
        }
        goto LABEL_85;
    }
    v20 = (tGameObjectMain*)v26;
    if (v26)
    {
        if (v26 <= 2)
            v20 = AI_MainActor[v26 - 1];
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v20, 0);
    }
    else
    {
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
    }
    if (v37->i_mode_texte)
    {
        if (v37->i_mode_texte == 1)
        {
            v16 = (tGameObjectMain*)v26;
            if (v26)
            {
                if (v26 <= 2)
                    v16 = AI_MainActor[v26 - 1];
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(v16, 1);
                goto LABEL_75;
            }
        }
        else
        {
            if (v37->i_mode_texte == 2)
            {
                if (v37->i_last_snd != v37->i_mode_texte)
                {
                    v15 = (tGameObjectMain*)v37->des_o_distrib;
                    if (v15)
                    {
                        if ((unsigned int)v15 <= 2)
                            v15 = AI_MainActor[(uint32_t)v15 - 1];
                        v6 = AI_EvalFunc_SNDRequestPlay_C(v15, 0, -1.0);
                    }
                    else
                    {
                        v6 = AI_EvalFunc_SNDRequestPlay_C(0, 0, -1.0);
                    }
                    v33 = v6;
                    v37->i_last_snd = v37->i_mode_texte;
                }
                v14 = (tGameObjectMain*)v26;
                if (v26)
                {
                    if (v26 <= 2)
                        v14 = AI_MainActor[v26 - 1];
                    v13 = v14;
                }
                else
                {
                    v13 = 0;
                }
                v7 = (tTextGet*)AI_EvalVarArray_C(10, (char*)&v37->__des_at_prix_Dimension1, v37->i_current);
                v8 = AI_EvalFunc_TEXTToString_C(v7);
                DBG_OutputDebugString("distrib_crunsh_dc_texte: %s\n", v8);
                STR_Init3DString(v13, v8);
                goto LABEL_75;
            }
            v12 = (tGameObjectMain*)v26;
            if (v26)
            {
                if (v26 <= 2)
                    v12 = AI_MainActor[v26 - 1];
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(v12, 1);
                goto LABEL_75;
            }
        }
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
    }
    else
    {
        if (v37->i_last_snd != v37->i_mode_texte)
        {
            v19 = (tGameObjectMain*)v37->des_o_distrib;
            if (v19)
            {
                if ((unsigned int)v19 <= 2)
                    v19 = AI_MainActor[(uint32_t)v19 - 1];
                AI_EvalFunc_SNDRequestPlay_C(v19, 0, -1.0);
            }
            else
            {
                AI_EvalFunc_SNDRequestPlay_C(0, 0, -1.0);
            }
            v37->i_last_snd = v37->i_mode_texte;
        }
        v18 = (tGameObjectMain*)v26;
        if (v26)
        {
            if (v26 <= 2)
                v18 = AI_MainActor[v26 - 1];
            v17 = v18;
        }
        else
        {
            v17 = 0;
        }
        v4 = (tTextGet*)AI_EvalVarArray_C(10, (char*)&v37->__des_at_noms_Dimension1, v37->i_current);
        v5 = AI_EvalFunc_TEXTToString_C(v4);
        STR_Init3DString(v17, v5);
    }
LABEL_75:
    if (v37->i_mode_texte <= 2)
    {
        if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(256))
        {
            if (v37->i_force_expl)
            {
                if (AI_EvalFunc_TIMEElapsed_C(v37->f_time_debut, 3.0))
                {
                    v37->i_force_expl = 0;
                    v9 = AI_EvalVarArray_C(10, (char*)&v37->__des_at_explication_Dimension1, v37->i_current);
                    memcpy((void*)distrib_crunsh_dc_texte_GST_AffichageTexte_gao_texte_message_a_traiter, v9, 8u);
                    *(uint32_t*)distrib_crunsh_dc_texte_GST_AffichageTexte_gao_i_Sound_ID = v37->des_ai_son[v37->i_current];
                    *(uint32_t*)distrib_crunsh_dc_texte_GST_AffichageTexte_gao_o_Sound_Localisation3D = (uint32_t)AI_gpst_CurrentGameObject;
                    *(uint32_t*)distrib_crunsh_dc_texte_GST_AffichageTexte_gao_f_Sound_Volume = 0x3F4CCCCD;
                    *(uint32_t*)distrib_crunsh_dc_texte_GST_AffichageTexte_gao_f_near = 0x48F42400;
                    *(uint32_t*)distrib_crunsh_dc_texte_GST_AffichageTexte_gao_f_far = 0x48F42400;
                    Src.msgID = 30;
                    Src.Int1 = 0;
                    Src.f2 = 5.0;
                    Src.f1 = -1.0;
                    Src.v1.x = 0.5;
                    Src.v1.y = 0.0;
                    Src.v1.z = 0.2f;
                    Src.v2.x = 0.0;
                    Src.v2.y = 0.0;
                    Src.v2.z = 0.0;
                    Src.gao2 = AI_gpst_CurrentGameObject;
                    Src.gao = AI_gpst_CurrentGameObject;
                    memcpy((void*)distrib_crunsh_dc_texte_GST_AffichageTexte_gao_message_a_traiter, &Src, 0x34u);
                    *(uint32_t*)distrib_crunsh_dc_texte_GST_AffichageTexte_gao_prio_message_a_traiter = 10;
                }
            }
        }
    }
LABEL_85:
    v35 = 0;
    if ((v37->i_mode_texte & 1) != 0)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v37->f_time_texte, 0.30000001f))
            v35 = 1;
    }
    if ((v37->i_mode_texte & 1) == 0)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v37->f_time_texte, 1.5))
            v35 = 1;
    }
    if (v35)
    {
        v37->f_time_texte = TIM_gf_MainClock - TIM_gf_LockTime;
        if (++v37->i_mode_texte == 4)
            v37->i_mode_texte = 0;
    }
    return 1000;
}

int distrib_crunsh_dc_exec_cam()
{
    tScriptStruct_DistribCrunsh* v1; // [esp+0h] [ebp-134h]
    float v2; // [esp+30h] [ebp-104h]
    Vec3 v3; // [esp+34h] [ebp-100h] BYREF
    Vec3 v4; // [esp+40h] [ebp-F4h] BYREF
    Vec3 v5; // [esp+4Ch] [ebp-E8h] BYREF
    Vec3 a2; // [esp+58h] [ebp-DCh] BYREF
    float v7; // [esp+64h] [ebp-D0h]
    float v8; // [esp+68h] [ebp-CCh]
    float v9; // [esp+6Ch] [ebp-C8h]
    BOOL v10; // [esp+70h] [ebp-C4h]
    Vec3 v11; // [esp+74h] [ebp-C0h] BYREF
    Vec3 v12; // [esp+80h] [ebp-B4h] BYREF
    Vec3 a1; // [esp+8Ch] [ebp-A8h] BYREF
    Vec3 v14; // [esp+98h] [ebp-9Ch] BYREF
    Vec3 v15; // [esp+A4h] [ebp-90h] BYREF
    Vec3 v16; // [esp+B4h] [ebp-80h] BYREF
    float v17; // [esp+C0h] [ebp-74h]
    float v18; // [esp+C4h] [ebp-70h]
    float v19; // [esp+C8h] [ebp-6Ch]
    Vec3 v20; // [esp+CCh] [ebp-68h] BYREF
    Vec3 dest; // [esp+D8h] [ebp-5Ch] BYREF
    Vec3 v22; // [esp+E4h] [ebp-50h] BYREF
    float v23; // [esp+F0h] [ebp-44h]
    float v24; // [esp+F4h] [ebp-40h]
    float v25; // [esp+F8h] [ebp-3Ch]
    Vec3 a3; // [esp+FCh] [ebp-38h] BYREF
    tScriptStruct_DistribCrunsh* v27; // [esp+108h] [ebp-2Ch]
    Vec3 v28; // [esp+10Ch] [ebp-28h] BYREF
    tGameObjectMain* v29; // [esp+118h] [ebp-1Ch]
    Vec3 v30; // [esp+11Ch] [ebp-18h] BYREF
    Vec3 v31; // [esp+128h] [ebp-Ch] BYREF

    a2.x = 1.0;
    a2.y = 0.0;
    a2.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_DistribCrunsh*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v27 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        distrib_crunsh_dc_exec_cam__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        distrib_crunsh_dc_exec_cam__PJ_Camera_gao = AI_ul_RealResolveOneRefObject(0x71003F56u);
        distrib_crunsh_dc_exec_cam__PJ_Camera_Derriere_gao = AI_ul_RealResolveOneRefObject(0x5E005FA2u);
        distrib_crunsh_dc_exec_cam__PJ_Camera_Target_gao = AI_ul_RealResolveOneRefObject(0x35006499u);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v11);
    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)distrib_crunsh_dc_exec_cam__PJ_Sally_gao, &a3);
    MATH_SubstractVec(&dest, &v11, &a3);
    a2 = dest;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v5);
    AI_EvalFunc_MATHVecCrossProduct_C(&v5, &a2, &v4);
    a2 = v4;
    v2 = AI_EvalFunc_MTHFloatSign_C(v4.z);
    AI_EvalFunc_OBJ_FlagInactiveSet_C((tGameObjectMain*)distrib_crunsh_dc_exec_cam__PJ_Camera_gao, 1);
    AI_EvalFunc_OBJ_FlagInactiveSet_C((tGameObjectMain*)distrib_crunsh_dc_exec_cam__PJ_Camera_Derriere_gao, 1);
    AI_EvalFunc_OBJ_FlagInactiveSet_C((tGameObjectMain*)distrib_crunsh_dc_exec_cam__PJ_Camera_Target_gao, 1);
    v29 = AI_EvalFunc_VIEWAssignObject_C((tGameObjectMain*)distrib_crunsh_dc_exec_cam__PJ_Camera_gao, 0);
    if (fabs(v27->v_pos_cam.x) != 0.0f)
    {
        v10 = 0;
    }
    else if (fabs(v27->v_pos_cam.y) != 0.0f)
    {
        v10 = 0;
    }
    else
    {
        v10 = fabs(v27->v_pos_cam.z) == 0.0f;
    }
    if (v10)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v22);
        v28.x = 0.0;
        v28.y = 0.0;
        v28.z = 2.0;
        MATH_VecAddVec(&a1, &v22, &v28);
        a2 = a1;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v20);
        v7 = v20.x * 1.5f;
        v8 = v20.y * 1.5f;
        v9 = v20.z * 1.5f;
        a2.x = a2.x + v7;
        a2.y = a2.y + v8;
        a2.z = a2.z + v9;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v31);
        v17 = v31.x * v2;
        v18 = v31.y * v2;
        v19 = v31.z * v2;
        a2.x = a2.x + v17;
        a2.y = a2.y + v18;
        a2.z = a2.z + v19;
        AI_EvalFunc_OBJSetPos_C((tGameObjectMain*)distrib_crunsh_dc_exec_cam__PJ_Camera_gao, &a2);
        v27->v_pos_cam = a2;
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v12);
    v15.x = 0.0;
    v15.y = 0.0;
    v15.z = 1.2f;
    MATH_VecAddVec(&v3, &v12, &v15);
    a2 = v3;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v14);
    v23 = v14.x * 0.30000001f;
    v24 = v14.y * 0.30000001f;
    v25 = v14.z * 0.30000001f;
    a2.x = a2.x + v23;
    a2.y = a2.y + v24;
    a2.z = a2.z + v25;
    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)distrib_crunsh_dc_exec_cam__PJ_Camera_gao, &v16);
    a2.x = a2.x - v16.x;
    a2.y = a2.y - v16.y;
    a2.z = a2.z - v16.z;
    v30.x = 0.0;
    v30.y = 0.0;
    v30.z = 1.0;
    AI_EvalFunc_OBJ_SightGeneralSet_C((tGameObjectMain*)distrib_crunsh_dc_exec_cam__PJ_Camera_gao, &a2, &v30);
    return 1000;
}

int distrib_crunsh_dc_reflex()
{
    double v0; // st7
    int v1; // eax
    int v2; // eax
    int v3; // eax
    char* v4; // eax
    tScriptStruct_DistribCrunsh* v6; // [esp+4h] [ebp-198h]
    tGameObjectMain* v7; // [esp+Ch] [ebp-190h]
    tGameObjectMain* v8; // [esp+14h] [ebp-188h]
    tGameObjectMain* v9; // [esp+1Ch] [ebp-180h]
    tGameObjectMain* v10; // [esp+2Ch] [ebp-170h]
    tGameObjectMain* v11; // [esp+34h] [ebp-168h]
    Vec3 a1; // [esp+38h] [ebp-164h] BYREF
    int v13; // [esp+44h] [ebp-158h]
    tMessage v14; // [esp+48h] [ebp-154h] BYREF
    tMessage v15; // [esp+7Ch] [ebp-120h] BYREF
    Vec3 a3; // [esp+BCh] [ebp-E0h] BYREF
    tGameObjectMain* v17; // [esp+C8h] [ebp-D4h]
    float v18; // [esp+CCh] [ebp-D0h]
    float v19; // [esp+DCh] [ebp-C0h]
    Vec3 v20; // [esp+F4h] [ebp-A8h] BYREF
    tTextGet v21; // [esp+100h] [ebp-9Ch] BYREF
    int v22; // [esp+108h] [ebp-94h]
    int v23; // [esp+10Ch] [ebp-90h]
    Vec3 dest; // [esp+110h] [ebp-8Ch] BYREF
    int v25; // [esp+11Ch] [ebp-80h]
    void* Src; // [esp+130h] [ebp-6Ch]
    float v27; // [esp+13Ch] [ebp-60h]
    Vec3 v28; // [esp+14Ch] [ebp-50h] BYREF
    Vec3 a5; // [esp+158h] [ebp-44h] BYREF
    BOOL v30; // [esp+164h] [ebp-38h]
    tTextGet v31; // [esp+168h] [ebp-34h] BYREF
    tScriptStruct_DistribCrunsh* v32; // [esp+174h] [ebp-28h]
    Vec3 v33; // [esp+184h] [ebp-18h] BYREF
    int v34; // [esp+190h] [ebp-Ch]

    v28.x = 1.0;
    v28.y = 0.0;
    v28.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_DistribCrunsh*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v32 = v6;
    v17 = 0;
    v30 = 0;
    v18 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        distrib_crunsh_dc_reflex__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_reflex__PJ_Sally_gao_i_fight_on,
            (void*)distrib_crunsh_dc_reflex__PJ_Sally_gao,
            1216);
        distrib_crunsh_dc_reflex_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_reflex_univVars_Perso_ID_equipement,
            (void*)distrib_crunsh_dc_reflex_univVars,
            40);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_reflex__PJ_Sally_gao_current_pm_mode,
            (void*)distrib_crunsh_dc_reflex__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_reflex__PJ_Sally_gao_current_pm_priority,
            (void*)distrib_crunsh_dc_reflex__PJ_Sally_gao,
            576);
        distrib_crunsh_dc_reflex_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_reflex_GST_Board_panel_gao_i_use_prio,
            (void*)distrib_crunsh_dc_reflex_GST_Board_panel_gao,
            496);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_reflex_GST_Board_panel_gao_o_use_object,
            (void*)distrib_crunsh_dc_reflex_GST_Board_panel_gao,
            488);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_reflex_GST_Board_panel_gao_i_use_action,
            (void*)distrib_crunsh_dc_reflex_GST_Board_panel_gao,
            500);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_reflex_GST_Board_panel_gao_f_use_offset,
            (void*)distrib_crunsh_dc_reflex_GST_Board_panel_gao,
            504);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_reflex_GST_Board_panel_gao_i_sally_priority,
            (void*)distrib_crunsh_dc_reflex_GST_Board_panel_gao,
            396);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_reflex_GST_Board_panel_gao_o_sally_contextuel,
            (void*)distrib_crunsh_dc_reflex_GST_Board_panel_gao,
            388);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_reflex_GST_Board_panel_gao_i_sally_action,
            (void*)distrib_crunsh_dc_reflex_GST_Board_panel_gao,
            412);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_reflex_GST_Board_panel_gao_v_sally_offset,
            (void*)distrib_crunsh_dc_reflex_GST_Board_panel_gao,
            400);
        AI2C_ResolveBuffer(
            &distrib_crunsh_dc_reflex_GST_Board_panel_gao_i_force_interface,
            (void*)distrib_crunsh_dc_reflex_GST_Board_panel_gao,
            3292);
        distrib_crunsh_dc_reflex_TXT_Sally_gao = AI_ul_RealResolveOneRefObject(0x7200064Eu);
        distrib_crunsh_dc_reflex__PJ_Camera_gao = AI_ul_RealResolveOneRefObject(0x71003F56u);
        fct_distrib_crunsh_dc_reflex_distrib_crunsh_dc_deroule = (int)AI_ul_RealResolveOneRefFunction(0x3D001D52u);
    }
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
    v32->v_pos_cam.x = 0.0;
    v32->v_pos_cam.y = 0.0;
    v32->v_pos_cam.z = 0.0;
    v17 = *AI_MainActor;
    v11 = v17;
    if (v17)
    {
        if ((unsigned int)v17 <= 2)
            v11 = AI_MainActor[(uint32_t)v17 - 1];
        v0 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v11);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
    }
    v18 = (float)v0;
    v10 = v17;
    if (v17)
    {
        if ((unsigned int)v17 <= 2)
            v10 = AI_MainActor[(uint32_t)v17 - 1];
        AI_EvalFunc_OBJGetPos_C(v10, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
    MATH_SubstractVec(&dest, &a3, &a1);
    v28 = dest;
    if (*(uint32_t*)distrib_crunsh_dc_reflex__PJ_Sally_gao_i_fight_on)
    {
        v32->i_swap_force = 0;
    }
    else
    {
        AI_EvalFunc_MATHVecSetHorzNormalize_C(&v28);
        if (v18 >= (double)v32->des_f_distance
            || (AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v33),
                v19 = v33.z * v28.z + v33.y * v28.y + v28.x * v33.x,
                v27 = cosf(v32->angle_de_proposition),
                v19 <= (double)v27))
        {
            v32->i_swap_force = 0;
        }
        else if (*(uint32_t*)AI_EvalVarArray2_C(1, (char*)distrib_crunsh_dc_reflex_univVars_Perso_ID_equipement, 0, 8) == 34)
        {
            v34 = (unsigned __int16)*(uint32_t*)(distrib_crunsh_dc_reflex__PJ_Sally_gao + 4);
            if ((v34 & 1) != 0)
                v30 = !*(uint32_t*)distrib_crunsh_dc_reflex__PJ_Sally_gao_current_pm_mode
                || *(int*)distrib_crunsh_dc_reflex__PJ_Sally_gao_current_pm_priority < 20;
            else
                v30 = 0;
            if (v30 && *(int*)distrib_crunsh_dc_reflex_GST_Board_panel_gao_i_use_prio < 5)
            {
                *(uint32_t*)distrib_crunsh_dc_reflex_GST_Board_panel_gao_o_use_object = (uint32_t)AI_gpst_CurrentGameObject;
                *(uint32_t*)distrib_crunsh_dc_reflex_GST_Board_panel_gao_i_use_prio = 5;
                *(uint32_t*)distrib_crunsh_dc_reflex_GST_Board_panel_gao_i_use_action = 4;
                *(uint32_t*)distrib_crunsh_dc_reflex_GST_Board_panel_gao_f_use_offset = 0;
            }
            if (v30 && *(float*)distrib_crunsh_dc_reflex_GST_Board_panel_gao_i_sally_priority < 5.0)
            {
                *(uint32_t*)distrib_crunsh_dc_reflex_GST_Board_panel_gao_o_sally_contextuel = (uint32_t)AI_gpst_CurrentGameObject;
                *(uint32_t*)distrib_crunsh_dc_reflex_GST_Board_panel_gao_i_sally_priority = 0x40A00000;
                *(uint32_t*)distrib_crunsh_dc_reflex_GST_Board_panel_gao_i_sally_action = 4;
                *(uint32_t*)distrib_crunsh_dc_reflex_GST_Board_panel_gao_v_sally_offset = 0;
                *(uint32_t*)(distrib_crunsh_dc_reflex_GST_Board_panel_gao_v_sally_offset + 4) = 0;
                *(uint32_t*)(distrib_crunsh_dc_reflex_GST_Board_panel_gao_v_sally_offset + 8) = 0;
                *(uint32_t*)distrib_crunsh_dc_reflex_GST_Board_panel_gao_i_force_interface = 1;
            }
        }
        else if (!v32->i_swap_force)
        {
            v32->i_swap_force = 1;
            *(uint32_t*)AI_EvalVarArray2_C(1, (char*)distrib_crunsh_dc_reflex_univVars_Perso_ID_equipement, 0, 8) = 34;
        }
    }
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
    if (AI_EvalFunc_MSGGetCount_C(AI_gpst_CurrentGameObject))
    {
        Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v15, Src, sizeof(v15));
        if (v15.msgID == 9 && v15.f2 == 5.0f || v15.msgID == 10 && v15.f2 == 5.0f)
        {
            v15.msgID = 64;
            v15.Int1 = 34;
            v15.gao = AI_gpst_CurrentGameObject;
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)distrib_crunsh_dc_reflex__PJ_Sally_gao, &v15);
        }
        else if (v15.msgID == 37 && (v15.Int1 & 7) != 0)
        {
            v9 = (tGameObjectMain*)v32->des_o_distrib;
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                v1 = AI_EvalFunc_SNDRequestPlay_C(v9, 6, -1.0);
            }
            else
            {
                v1 = AI_EvalFunc_SNDRequestPlay_C(0, 6, -1.0);
            }
            v22 = v1;
            v8 = (tGameObjectMain*)v32->des_o_distrib;
            if (v8)
            {
                if ((unsigned int)v8 <= 2)
                    v8 = AI_MainActor[(uint32_t)v8 - 1];
                v2 = AI_EvalFunc_SNDRequestPlay_C(v8, 7, -1.0);
            }
            else
            {
                v2 = AI_EvalFunc_SNDRequestPlay_C(0, 7, -1.0);
            }
            v13 = v2;
            v7 = (tGameObjectMain*)v32->des_o_distrib;
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                v3 = AI_EvalFunc_SNDRequestPlay_C(v7, 8, -1.0);
            }
            else
            {
                v3 = AI_EvalFunc_SNDRequestPlay_C(0, 8, -1.0);
            }
            v23 = v3;
            v14.msgID = 30;
            v14.Int1 = 29;
            v14.Int2 = 10;
            v14.f2 = 2.0;
            v14.v1.x = 0.0;
            v14.v1.y = 0.0;
            v14.v1.z = 0.0;
            v14.v2.x = 0.0;
            v14.v2.y = 0.0;
            v14.v2.z = 0.0;
            v14.gao = 0;
            v14.gao2 = AI_gpst_CurrentGameObject;
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)distrib_crunsh_dc_reflex_TXT_Sally_gao, &v14);
            v32->f_time_debut = TIM_gf_MainClock - TIM_gf_LockTime;
            AI_EvalFunc_TEXTGet_C(0x4900FDC5, 60, &v21);
            memcpy(&v31, &v21, sizeof(v31));
            v4 = AI_EvalFunc_TEXTToString_C(&v31);
            STR_SetString((tGameObjectMain*)distrib_crunsh_dc_reflex_GST_Board_panel_gao, 1, v4);
            AI_EvalFunc_OBJ_TextFlagSet_C((tGameObjectMain*)distrib_crunsh_dc_reflex_GST_Board_panel_gao, 1, 5);
            v20.x = 0.5;
            v20.y = 0.75;
            v20.z = 1.0;
            AI_EvalFunc_OBJ_TextPivotSet_C((tGameObjectMain*)distrib_crunsh_dc_reflex_GST_Board_panel_gao, 1, &v20);
            a5.x = -1.0;
            a5.y = 0.0;
            a5.z = 0.0;
            STR_SetSize((tGameObjectMain*)distrib_crunsh_dc_reflex_GST_Board_panel_gao, 1, 0, -1, &a5);
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(256, 0);
            AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)distrib_crunsh_dc_reflex__PJ_Camera_gao, &v32->v_last_pos_cam);
            AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)distrib_crunsh_dc_reflex__PJ_Camera_gao, &v32->v_last_sight_cam);
            v32->i_force_expl = 1;
            v25 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_distrib_crunsh_dc_reflex_distrib_crunsh_dc_deroule);
            if (v25 != 1000)
                return v25;
        }
    }
    return 1000;
}

int distrib_crunsh_dc_tourne()
{
    tScriptStruct_DistribCrunsh* v1; // [esp+0h] [ebp-20h]
    tGameObjectMain* v2; // [esp+8h] [ebp-18h]
    tGameObjectMain* v3; // [esp+Ch] [ebp-14h]
    tGameObjectMain* v4; // [esp+10h] [ebp-10h]
    int i; // [esp+1Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_DistribCrunsh*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    for (i = 0; i < 6; ++i)
    {
        v4 = (tGameObjectMain*)v1->des_ao_contenu[i];
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            v3 = v4;
        }
        else
        {
            v3 = 0;
        }
        if (v3)
        {
            v2 = (tGameObjectMain*)v1->des_ao_contenu[i];
            if (v2)
            {
                if ((unsigned int)v2 <= 2)
                    v2 = AI_MainActor[(uint32_t)v2 - 1];
                AI_EvalFunc_OBJRotateGlobalZ_C(v2, TIM_gf_dt);
            }
            else
            {
                AI_EvalFunc_OBJRotateGlobalZ_C(0, TIM_gf_dt);
            }
        }
    }
    return 1000;
}