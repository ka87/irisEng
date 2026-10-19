#include "ai_compiledscripts_inc.h"

//Indice_Avancement_Jeu_iaj_init
int Indice_Avancement_Jeu_iaj_init_GST_Global_gao;
int Indice_Avancement_Jeu_iaj_init_GST_Global_gao_i_optim_nb_dialogue;
int Indice_Avancement_Jeu_iaj_init_GST_Global_gao_i_optim_nb_avancement;
int Indice_Avancement_Jeu_iaj_init_GST_Global_gao_ao_optim_dialogue;
int fct_Indice_Avancement_Jeu_iaj_init_Indice_Avancement_Jeu_iaj_change;

//Indice_Avancement_Jeu_iaj_change
int Indice_Avancement_Jeu_iaj_change_univVars;
int Indice_Avancement_Jeu_iaj_change_univVars_Perso_ID_actor;
int Indice_Avancement_Jeu_iaj_change_GST_Global_gao;
int Indice_Avancement_Jeu_iaj_change_GST_Global_gao_i_compagnon_qui_parle;
int Indice_Avancement_Jeu_iaj_change_GST_Global_gao_i_mise_a_jour_des_SF;
int Indice_Avancement_Jeu_iaj_change_univVars_Perso_ID_status;
int Indice_Avancement_Jeu_iaj_change_univVars_Perso_ID_world_ID;
int Indice_Avancement_Jeu_iaj_change_univVars_world_ID;
int Indice_Avancement_Jeu_iaj_change_univVars_Flag_speciaux_sauve;

// =======================================

//Dialogue_Reset_All_SF_sfr_init
int Dialogue_Reset_All_SF_sfr_init_GST_Global_gao;
int Dialogue_Reset_All_SF_sfr_init_GST_Global_gao_i_optim_nb_avancement;
int Dialogue_Reset_All_SF_sfr_init_GST_Global_gao_ao_optim_dialogue;
int fct_Dialogue_Reset_All_SF_sfr_init_Dialogue_Reset_All_SF_sfr_reset;

//Dialogue_Reset_All_SF
int Dialogue_Reset_All_SF_sfr_reset_univVars;
int Dialogue_Reset_All_SF_sfr_reset_univVars_Flag_speciaux_sauve;

int Indice_Avancement_Jeu_iaj_init()
{
    tScriptStruct_IndiceAvancementJeu* v1; // [esp+0h] [ebp-14h]
    int i; // [esp+10h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_IndiceAvancementJeu*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Indice_Avancement_Jeu_iaj_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Indice_Avancement_Jeu_iaj_init_GST_Global_gao_i_optim_nb_dialogue,
            (void*)Indice_Avancement_Jeu_iaj_init_GST_Global_gao,
            3048);
        AI2C_ResolveBuffer(
            &Indice_Avancement_Jeu_iaj_init_GST_Global_gao_i_optim_nb_avancement,
            (void*)Indice_Avancement_Jeu_iaj_init_GST_Global_gao,
            3052);
        AI2C_ResolveBuffer(
            &Indice_Avancement_Jeu_iaj_init_GST_Global_gao_ao_optim_dialogue,
            (void*)Indice_Avancement_Jeu_iaj_init_GST_Global_gao,
            3056);
        fct_Indice_Avancement_Jeu_iaj_init_Indice_Avancement_Jeu_iaj_change = (int)AI_ul_RealResolveOneRefFunction(0x9E002FC2);
    }
    for (i = 0; i < 20; ++i)
    {
        if (v1->ai_grisage[i])
            v1->ai_grisage[i] = 254;
        else
            v1->ai_grisage[i] = v1->ai_conditions[i] + 1;
    }
    AI_EvalFunc_AIPrioritySet_C(AI_gpst_CurrentGameObject, 201);
    *(uint32_t*)(Indice_Avancement_Jeu_iaj_init_GST_Global_gao_ao_optim_dialogue
        + 4
        * (*(uint32_t*)Indice_Avancement_Jeu_iaj_init_GST_Global_gao_i_optim_nb_avancement
            + (*(uint32_t*)Indice_Avancement_Jeu_iaj_init_GST_Global_gao_i_optim_nb_dialogue)++)
        + 4) = (uint32_t)AI_gpst_CurrentGameObject;
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Indice_Avancement_Jeu_iaj_init_Indice_Avancement_Jeu_iaj_change);
    AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
    return 1000;
}

int Indice_Avancement_Jeu_iaj_change()
{
    int result; // eax
    char* v1; // eax
    char* v2; // eax
    char* v3; // eax
    char* v4; // eax
    const void* v5; // [esp-8h] [ebp-100h]
    const void* v6; // [esp-8h] [ebp-100h]
    char* v7; // [esp-8h] [ebp-100h]
    char* v8; // [esp-8h] [ebp-100h]
    tScriptStruct_IndiceAvancementJeu* v9; // [esp+4h] [ebp-F4h]
    tAI* v10; // [esp+8h] [ebp-F0h]
    tGameObjectMain* v11; // [esp+Ch] [ebp-ECh]
    tGameObjectMain* v12; // [esp+10h] [ebp-E8h]
    tAI* v13; // [esp+18h] [ebp-E0h]
    tGameObjectMain* v14; // [esp+1Ch] [ebp-DCh]
    tGameObjectMain* v15; // [esp+20h] [ebp-D8h]
    tAI* v16; // [esp+24h] [ebp-D4h]
    tGameObjectMain* v17; // [esp+28h] [ebp-D0h]
    tGameObjectMain* v18; // [esp+2Ch] [ebp-CCh]
    tAI* v19; // [esp+30h] [ebp-C8h]
    tGameObjectMain* v20; // [esp+34h] [ebp-C4h]
    tGameObjectMain* v21; // [esp+38h] [ebp-C0h]
    tAI* v22; // [esp+40h] [ebp-B8h]
    tGameObjectMain* v23; // [esp+44h] [ebp-B4h]
    tGameObjectMain* v24; // [esp+48h] [ebp-B0h]
    tAI* v25; // [esp+4Ch] [ebp-ACh]
    tGameObjectMain* v26; // [esp+50h] [ebp-A8h]
    tGameObjectMain* v27; // [esp+54h] [ebp-A4h]
    tAI* v28; // [esp+58h] [ebp-A0h]
    tGameObjectMain* v29; // [esp+5Ch] [ebp-9Ch]
    tGameObjectMain* v30; // [esp+60h] [ebp-98h]
    tAI* v31; // [esp+64h] [ebp-94h]
    tGameObjectMain* v32; // [esp+68h] [ebp-90h]
    tGameObjectMain* v33; // [esp+6Ch] [ebp-8Ch]
    tAI* v34; // [esp+70h] [ebp-88h]
    tGameObjectMain* v35; // [esp+74h] [ebp-84h]
    tGameObjectMain* v36; // [esp+78h] [ebp-80h]
    tAI* v37; // [esp+7Ch] [ebp-7Ch]
    tGameObjectMain* v38; // [esp+80h] [ebp-78h]
    tGameObjectMain* v39; // [esp+84h] [ebp-74h]
    tAI* v40; // [esp+88h] [ebp-70h]
    tGameObjectMain* v41; // [esp+8Ch] [ebp-6Ch]
    tGameObjectMain* v42; // [esp+90h] [ebp-68h]
    tAI* v43; // [esp+94h] [ebp-64h]
    tGameObjectMain* v44; // [esp+98h] [ebp-60h]
    tGameObjectMain* v45; // [esp+9Ch] [ebp-5Ch]
    tAI* v46; // [esp+A0h] [ebp-58h]
    tGameObjectMain* v47; // [esp+A4h] [ebp-54h]
    tGameObjectMain* v48; // [esp+A8h] [ebp-50h]
    tAI* v49; // [esp+B4h] [ebp-44h]
    tGameObjectMain* v50; // [esp+B8h] [ebp-40h]
    tGameObjectMain* v51; // [esp+BCh] [ebp-3Ch]
    tAI* v52; // [esp+C4h] [ebp-34h]
    int i; // [esp+D4h] [ebp-24h]
    int a2; // [esp+D8h] [ebp-20h]
    BOOL v55; // [esp+DCh] [ebp-1Ch]
    BOOL v56; // [esp+DCh] [ebp-1Ch]
    int a4; // [esp+E4h] [ebp-14h]
    int num; // [esp+E8h] [ebp-10h]
    int v59; // [esp+ECh] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v9 = (tScriptStruct_IndiceAvancementJeu*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v9 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Indice_Avancement_Jeu_iaj_change_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Indice_Avancement_Jeu_iaj_change_univVars_Perso_ID_actor,
            (void*)Indice_Avancement_Jeu_iaj_change_univVars,
            988);
        Indice_Avancement_Jeu_iaj_change_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Indice_Avancement_Jeu_iaj_change_GST_Global_gao_i_compagnon_qui_parle,
            (void*)Indice_Avancement_Jeu_iaj_change_GST_Global_gao,
            2996);
        AI2C_ResolveBuffer(
            &Indice_Avancement_Jeu_iaj_change_GST_Global_gao_i_mise_a_jour_des_SF,
            (void*)Indice_Avancement_Jeu_iaj_change_GST_Global_gao,
            2992);
        AI2C_ResolveBuffer(
            &Indice_Avancement_Jeu_iaj_change_univVars_Perso_ID_status,
            (void*)Indice_Avancement_Jeu_iaj_change_univVars,
            484);
        AI2C_ResolveBuffer(
            &Indice_Avancement_Jeu_iaj_change_univVars_Perso_ID_world_ID,
            (void*)Indice_Avancement_Jeu_iaj_change_univVars,
            536);
        AI2C_ResolveBuffer(
            &Indice_Avancement_Jeu_iaj_change_univVars_world_ID,
            (void*)Indice_Avancement_Jeu_iaj_change_univVars,
            4);
        AI2C_ResolveBuffer(
            &Indice_Avancement_Jeu_iaj_change_univVars_Flag_speciaux_sauve,
            (void*)Indice_Avancement_Jeu_iaj_change_univVars,
            4488);
    }
    v52 = *(tAI**)(Indice_Avancement_Jeu_iaj_change_univVars_Perso_ID_actor + 4 * v9->des_i_companion + 4);
    if (v9->des_i_companion != *(uint32_t*)Indice_Avancement_Jeu_iaj_change_GST_Global_gao_i_compagnon_qui_parle
        || *(uint32_t*)(Indice_Avancement_Jeu_iaj_change_univVars_Perso_ID_status + 4 * v9->des_i_companion + 4)
        && *(uint32_t*)(Indice_Avancement_Jeu_iaj_change_univVars_Perso_ID_status + 4 * v9->des_i_companion + 4) != 1
        && *(uint32_t*)(Indice_Avancement_Jeu_iaj_change_univVars_Perso_ID_status + 4 * v9->des_i_companion + 4) != 5
        && (*(uint32_t*)(Indice_Avancement_Jeu_iaj_change_univVars_Perso_ID_status + 4 * v9->des_i_companion + 4) != 4
            || *(uint32_t*)(Indice_Avancement_Jeu_iaj_change_univVars_Perso_ID_world_ID + 4 * v9->des_i_companion + 4) != *(uint32_t*)Indice_Avancement_Jeu_iaj_change_univVars_world_ID))
    {
        goto LABEL_26;
    }
    for (i = 0; i < 5; ++i)
    {
        if (v52 == WOR_gst_Universe.univAIInstance)
        {
            v49 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v51 = *(tGameObjectMain**)(Indice_Avancement_Jeu_iaj_change_univVars_Perso_ID_actor + 4 * v9->des_i_companion + 4);
            if (v52)
            {
                if ((unsigned int)v52 <= 2)
                    v51 = AI_MainActor[(uint32_t)v52 - 1];
                v50 = v51;
            }
            else
            {
                v50 = 0;
            }
            v49 = v50->extended->AIInstance;
        }
        if (((tScriptStruct_Companion*)(v49->instanceVarDataCurrent))->ai_current_positions[i] == -1)
            break;
    }
    if (i != 5)
    {
        v59 = i;
        a4 = 0;
        num = v9->des_i_niveau;
        while (a4 < 20)
        {
            a2 = v9->ai_conditions[a4];
            if (a2 <= 10000)
            {
                v56 = a2 > 0
                    && AI_EvalFunc_ARRBitTest_C((int*)(Indice_Avancement_Jeu_iaj_change_univVars_Flag_speciaux_sauve + 4), a2);
            }
            else
            {
                v55 = a2 - 10000 > 0
                    && AI_EvalFunc_ARRBitTest_C(
                        (int*)(Indice_Avancement_Jeu_iaj_change_univVars_Flag_speciaux_sauve + 4),
                        a2 - 10000);
                v56 = !v55;
            }
            if (v56 && v59 < 4)
            {
                if (v52 == WOR_gst_Universe.univAIInstance)
                {
                    v46 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v48 = (tGameObjectMain*)v52;
                    if (v52)
                    {
                        if ((unsigned int)v52 <= 2)
                            v48 = AI_MainActor[(uint32_t)v52 - 1];
                        v47 = v48;
                    }
                    else
                    {
                        v47 = 0;
                    }
                    v46 = v47->extended->AIInstance;
                }
                ((tScriptStruct_Companion*)(v46->instanceVarDataCurrent))->ai_conditions[num] = v9->ai_conditions[a4];
                if (v52 == WOR_gst_Universe.univAIInstance)
                {
                    v43 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v45 = (tGameObjectMain*)v52;
                    if (v52)
                    {
                        if ((unsigned int)v52 <= 2)
                            v45 = AI_MainActor[(uint32_t)v52 - 1];
                        v44 = v45;
                    }
                    else
                    {
                        v44 = 0;
                    }
                    v43 = v44->extended->AIInstance;
                }
                v5 = (const void*)AI_EvalVarArray2_C(10, (char*)&v9->__at_questionsReponses_Dimension1, 0, a4);
                v1 = AI_EvalVarArray_C(10, (char*)&((tScriptStruct_Companion*)(v43->instanceVarDataCurrent))->__at_questions_Dimension1, num);
                memcpy(v1, v5, 8u);
                if (v52 == WOR_gst_Universe.univAIInstance)
                {
                    v40 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v42 = (tGameObjectMain*)v52;
                    if (v52)
                    {
                        if ((unsigned int)v52 <= 2)
                            v42 = AI_MainActor[(uint32_t)v52 - 1];
                        v41 = v42;
                    }
                    else
                    {
                        v41 = 0;
                    }
                    v40 = v41->extended->AIInstance;
                }
                v6 = (const void*)AI_EvalVarArray2_C(10, (char*)&v9->__at_questionsReponses_Dimension1, 1, a4);
                v2 = AI_EvalVarArray_C(10, (char*)&((tScriptStruct_Companion*)(v40->instanceVarDataCurrent))->__at_answers_Dimension1, num);
                memcpy(v2, v6, 8u);
                if (v52 == WOR_gst_Universe.univAIInstance)
                {
                    v37 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v39 = (tGameObjectMain*)v52;
                    if (v52)
                    {
                        if ((unsigned int)v52 <= 2)
                            v39 = AI_MainActor[(uint32_t)v52 - 1];
                        v38 = v39;
                    }
                    else
                    {
                        v38 = 0;
                    }
                    v37 = v38->extended->AIInstance;
                }
                ((tScriptStruct_Companion*)(v37->instanceVarDataCurrent))->ai_grisage[num] = v9->ai_grisage[a4];
                if (v9->ai_grisage[a4] == 254)
                {
                    if (v52 == WOR_gst_Universe.univAIInstance)
                    {
                        v31 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v33 = (tGameObjectMain*)v52;
                        if (v52)
                        {
                            if ((unsigned int)v52 <= 2)
                                v33 = AI_MainActor[(uint32_t)v52 - 1];
                            v32 = v33;
                        }
                        else
                        {
                            v32 = 0;
                        }
                        v31 = v32->extended->AIInstance;
                    }
                    ((tScriptStruct_Companion*)(v31->instanceVarDataCurrent))->ai_grisage[num] = v9->ai_conditions[a4] + 1;
                    if (v52 == WOR_gst_Universe.univAIInstance)
                    {
                        v28 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v30 = (tGameObjectMain*)v52;
                        if (v52)
                        {
                            if ((unsigned int)v52 <= 2)
                                v30 = AI_MainActor[(uint32_t)v52 - 1];
                            v29 = v30;
                        }
                        else
                        {
                            v29 = 0;
                        }
                        v28 = v29->extended->AIInstance;
                    }
                    ((tScriptStruct_Companion*)(v28->instanceVarDataCurrent))->ai_consequences[num] = 254;
                }
                else
                {
                    if (v52 == WOR_gst_Universe.univAIInstance)
                    {
                        v34 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v36 = (tGameObjectMain*)v52;
                        if (v52)
                        {
                            if ((unsigned int)v52 <= 2)
                                v36 = AI_MainActor[(uint32_t)v52 - 1];
                            v35 = v36;
                        }
                        else
                        {
                            v35 = 0;
                        }
                        v34 = v35->extended->AIInstance;
                    }
                    ((tScriptStruct_Companion*)(v34->instanceVarDataCurrent))->ai_consequences[num] = num;
                }
                if (v52 == WOR_gst_Universe.univAIInstance)
                {
                    v25 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v27 = (tGameObjectMain*)v52;
                    if (v52)
                    {
                        if ((unsigned int)v52 <= 2)
                            v27 = AI_MainActor[(uint32_t)v52 - 1];
                        v26 = v27;
                    }
                    else
                    {
                        v26 = 0;
                    }
                    v25 = v26->extended->AIInstance;
                }
                ((tScriptStruct_Companion*)(v25->instanceVarDataCurrent))->ai_current_positions[v59++] = num++;
                if (num - v9->des_i_niveau == 4 || v59 == 4)
                    break;
            }
            ++a4;
        }
        if (v52 == WOR_gst_Universe.univAIInstance)
        {
            v22 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v24 = (tGameObjectMain*)v52;
            if (v52)
            {
                if ((unsigned int)v52 <= 2)
                    v24 = AI_MainActor[(uint32_t)v52 - 1];
                v23 = v24;
            }
            else
            {
                v23 = 0;
            }
            v22 = v23->extended->AIInstance;
        }
        ((tScriptStruct_Companion*)(v22->instanceVarDataCurrent))->ai_conditions[20] = -1;
        if (v52 == WOR_gst_Universe.univAIInstance)
        {
            v19 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v21 = (tGameObjectMain*)v52;
            if (v52)
            {
                if ((unsigned int)v52 <= 2)
                    v21 = AI_MainActor[(uint32_t)v52 - 1];
                v20 = v21;
            }
            else
            {
                v20 = 0;
            }
            v19 = v20->extended->AIInstance;
        }
        v7 = AI_EvalVarArray_C(10, (char*)&v9->__at_question_reponse_OK_Dimension1, 0);
        v3 = AI_EvalVarArray_C(10, (char*)&((tScriptStruct_Companion*)(v19->instanceVarDataCurrent))->__at_questions_Dimension1, 20);
        memcpy(v3, v7, 8u);
        if (v52 == WOR_gst_Universe.univAIInstance)
        {
            v16 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v18 = (tGameObjectMain*)v52;
            if (v52)
            {
                if ((unsigned int)v52 <= 2)
                    v18 = AI_MainActor[(uint32_t)v52 - 1];
                v17 = v18;
            }
            else
            {
                v17 = 0;
            }
            v16 = v17->extended->AIInstance;
        }
        v8 = AI_EvalVarArray_C(10, (char*)&v9->__at_question_reponse_OK_Dimension1, 1);
        v4 = AI_EvalVarArray_C(10, (char*)&((tScriptStruct_Companion*)(v16->instanceVarDataCurrent))->__at_answers_Dimension1, 20);
        memcpy(v4, v8, 8u);
        if (v52 == WOR_gst_Universe.univAIInstance)
        {
            v13 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v15 = (tGameObjectMain*)v52;
            if (v52)
            {
                if ((unsigned int)v52 <= 2)
                    v15 = AI_MainActor[(uint32_t)v52 - 1];
                v14 = v15;
            }
            else
            {
                v14 = 0;
            }
            v13 = v14->extended->AIInstance;
        }
        ((tScriptStruct_Companion*)(v13->instanceVarDataCurrent))->ai_consequences[20] = 253;
        if (v52 == WOR_gst_Universe.univAIInstance)
        {
            v10 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v12 = (tGameObjectMain*)v52;
            if (v52)
            {
                if ((unsigned int)v52 <= 2)
                    v12 = AI_MainActor[(uint32_t)v52 - 1];
                v11 = v12;
            }
            else
            {
                v11 = 0;
            }
            v10 = v11->extended->AIInstance;
        }
        ((tScriptStruct_Companion*)(v10->instanceVarDataCurrent))->ai_current_positions[v59] = 20;
        *(uint32_t*)Indice_Avancement_Jeu_iaj_change_GST_Global_gao_i_mise_a_jour_des_SF = 0;
        AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
        result = 1000;
    }
    else
    {
    LABEL_26:
        *(uint32_t*)Indice_Avancement_Jeu_iaj_change_GST_Global_gao_i_mise_a_jour_des_SF = 0;
        AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

// ========================================

int Dialogue_Reset_All_SF_sfr_init()
{
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Dialogue_Reset_All_SF_sfr_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Dialogue_Reset_All_SF_sfr_init_GST_Global_gao_i_optim_nb_avancement,
            (void*)Dialogue_Reset_All_SF_sfr_init_GST_Global_gao,
            3052);
        AI2C_ResolveBuffer(
            &Dialogue_Reset_All_SF_sfr_init_GST_Global_gao_ao_optim_dialogue,
            (void*)Dialogue_Reset_All_SF_sfr_init_GST_Global_gao,
            3056);
        fct_Dialogue_Reset_All_SF_sfr_init_Dialogue_Reset_All_SF_sfr_reset = (int)AI_ul_RealResolveOneRefFunction(0x9E0035DE);
    }
    *(uint32_t*)(Dialogue_Reset_All_SF_sfr_init_GST_Global_gao_ao_optim_dialogue
        + 4 * (*(uint32_t*)Dialogue_Reset_All_SF_sfr_init_GST_Global_gao_i_optim_nb_avancement)++
        + 4) = (uint32_t)AI_gpst_CurrentGameObject;
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Dialogue_Reset_All_SF_sfr_init_Dialogue_Reset_All_SF_sfr_reset);
    AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
    return 1000;
}

int Dialogue_Reset_All_SF_sfr_reset()
{
    tScriptStruct_DialogueResetAllSF* v1; // [esp+0h] [ebp-14h]
    int i; // [esp+10h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_DialogueResetAllSF*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Dialogue_Reset_All_SF_sfr_reset_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Dialogue_Reset_All_SF_sfr_reset_univVars_Flag_speciaux_sauve,
            (void*)Dialogue_Reset_All_SF_sfr_reset_univVars,
            4488);
    }
    for (i = 0; i < 30; ++i)
    {
        if (v1->des_ai_SF_to_reset[i] > 0)
            AI_EvalFunc_ARRBitClear_C(
                (int*)(Dialogue_Reset_All_SF_sfr_reset_univVars_Flag_speciaux_sauve + 4),
                v1->des_ai_SF_to_reset[i]);
    }
    AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
    return 1000;
}