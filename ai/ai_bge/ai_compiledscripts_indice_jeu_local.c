#include "ai_compiledscripts_inc.h"

//Indice_Jeu_Local_ijl_init
int Indice_Jeu_Local_ijl_init_GST_Global_gao;
int Indice_Jeu_Local_ijl_init_GST_Global_gao_i_optim_nb_dialogue;
int Indice_Jeu_Local_ijl_init_GST_Global_gao_i_optim_nb_avancement;
int Indice_Jeu_Local_ijl_init_GST_Global_gao_ao_optim_dialogue;
int fct_Indice_Jeu_Local_ijl_init_Indice_Jeu_Local_ijl_wait;
int fct_Indice_Jeu_Local_ijl_init_Indice_Jeu_Local_iaj_change;

//Indice_Jeu_Local_iaj_change
int Indice_Jeu_Local_iaj_change_univVars;
int Indice_Jeu_Local_iaj_change_univVars_Perso_ID_actor;
int Indice_Jeu_Local_iaj_change_GST_Global_gao;
int Indice_Jeu_Local_iaj_change_GST_Global_gao_i_compagnon_qui_parle;
int fct_Indice_Jeu_Local_iaj_change_Indice_Jeu_Local_ijl_wait;
int Indice_Jeu_Local_iaj_change_univVars_Perso_ID_status;
int Indice_Jeu_Local_iaj_change_univVars_Perso_ID_world_ID;
int Indice_Jeu_Local_iaj_change_univVars_world_ID;
int Indice_Jeu_Local_iaj_change_univVars_Flag_speciaux_sauve;

//Indice_Jeu_Local_ijl_wait
int Indice_Jeu_Local_ijl_wait__PJ_Sally_gao;
int fct_Indice_Jeu_Local_ijl_wait_Indice_Jeu_Local_iaj_change;

int Indice_Jeu_Local_ijl_init()
{
    tScriptStruct_IndiceJeuLocal* v1; // [esp+0h] [ebp-18h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_IndiceJeuLocal*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Indice_Jeu_Local_ijl_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Indice_Jeu_Local_ijl_init_GST_Global_gao_i_optim_nb_dialogue,
            (void*)Indice_Jeu_Local_ijl_init_GST_Global_gao,
            3048);
        AI2C_ResolveBuffer(
            &Indice_Jeu_Local_ijl_init_GST_Global_gao_i_optim_nb_avancement,
            (void*)Indice_Jeu_Local_ijl_init_GST_Global_gao,
            3052);
        AI2C_ResolveBuffer(
            &Indice_Jeu_Local_ijl_init_GST_Global_gao_ao_optim_dialogue,
            (void*)Indice_Jeu_Local_ijl_init_GST_Global_gao,
            3056);
        fct_Indice_Jeu_Local_ijl_init_Indice_Jeu_Local_ijl_wait = (int)AI_ul_RealResolveOneRefFunction(0x9E00302D);
        fct_Indice_Jeu_Local_ijl_init_Indice_Jeu_Local_iaj_change = (int)AI_ul_RealResolveOneRefFunction(0x9E00302E);
    }
    AI_EvalFunc_AIPrioritySet_C(AI_gpst_CurrentGameObject, 200);
    *(uint32_t*)(Indice_Jeu_Local_ijl_init_GST_Global_gao_ao_optim_dialogue
        + 4
        * (*(uint32_t*)Indice_Jeu_Local_ijl_init_GST_Global_gao_i_optim_nb_avancement
            + (*(uint32_t*)Indice_Jeu_Local_ijl_init_GST_Global_gao_i_optim_nb_dialogue)++)
        + 4) = (uint32_t)AI_gpst_CurrentGameObject;
    if (v1->des_i_companion >= 3)
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Indice_Jeu_Local_ijl_init_Indice_Jeu_Local_iaj_change);
    else
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Indice_Jeu_Local_ijl_init_Indice_Jeu_Local_ijl_wait);
    AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
    return 1000;
}

int Indice_Jeu_Local_iaj_change()
{
    int* v1; // eax
    char* v2; // eax
    char* v3; // eax
    int v4; // [esp-Ch] [ebp-A4h]
    int v5; // [esp-8h] [ebp-A0h]
    const void* v6; // [esp-8h] [ebp-A0h]
    const void* v7; // [esp-8h] [ebp-A0h]
    int v8; // [esp-4h] [ebp-9Ch]
    tScriptStruct_IndiceJeuLocal* v9; // [esp+4h] [ebp-94h]
    tAI* v10; // [esp+8h] [ebp-90h]
    tGameObjectMain* v11; // [esp+Ch] [ebp-8Ch]
    tGameObjectMain* v12; // [esp+10h] [ebp-88h]
    tAI* v13; // [esp+14h] [ebp-84h]
    tGameObjectMain* v14; // [esp+18h] [ebp-80h]
    tGameObjectMain* v15; // [esp+1Ch] [ebp-7Ch]
    tAI* v16; // [esp+20h] [ebp-78h]
    tGameObjectMain* v17; // [esp+24h] [ebp-74h]
    tGameObjectMain* v18; // [esp+28h] [ebp-70h]
    tAI* v19; // [esp+2Ch] [ebp-6Ch]
    tGameObjectMain* v20; // [esp+30h] [ebp-68h]
    tGameObjectMain* v21; // [esp+34h] [ebp-64h]
    tAI* v22; // [esp+38h] [ebp-60h]
    tGameObjectMain* v23; // [esp+3Ch] [ebp-5Ch]
    tGameObjectMain* v24; // [esp+40h] [ebp-58h]
    int* a1; // [esp+44h] [ebp-54h]
    tAI* v26; // [esp+4Ch] [ebp-4Ch]
    tGameObjectMain* v27; // [esp+50h] [ebp-48h]
    tGameObjectMain* v28; // [esp+54h] [ebp-44h]
    tAI* v29; // [esp+58h] [ebp-40h]
    tGameObjectMain* v30; // [esp+5Ch] [ebp-3Ch]
    tGameObjectMain* v31; // [esp+60h] [ebp-38h]
    tAI* v32; // [esp+68h] [ebp-30h]
    int i; // [esp+78h] [ebp-20h]
    int j; // [esp+78h] [ebp-20h]
    int a4; // [esp+84h] [ebp-14h]
    int num; // [esp+88h] [ebp-10h]
    int v37; // [esp+8Ch] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v9 = (tScriptStruct_IndiceJeuLocal*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v9 = 0;
    num = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Indice_Jeu_Local_iaj_change_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Indice_Jeu_Local_iaj_change_univVars_Perso_ID_actor,
            (void*)Indice_Jeu_Local_iaj_change_univVars,
            988);
        Indice_Jeu_Local_iaj_change_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Indice_Jeu_Local_iaj_change_GST_Global_gao_i_compagnon_qui_parle,
            (void*)Indice_Jeu_Local_iaj_change_GST_Global_gao,
            2996);
        fct_Indice_Jeu_Local_iaj_change_Indice_Jeu_Local_ijl_wait = (int)AI_ul_RealResolveOneRefFunction(0x9E00302D);
        AI2C_ResolveBuffer(
            &Indice_Jeu_Local_iaj_change_univVars_Perso_ID_status,
            (void*)Indice_Jeu_Local_iaj_change_univVars,
            484);
        AI2C_ResolveBuffer(
            &Indice_Jeu_Local_iaj_change_univVars_Perso_ID_world_ID,
            (void*)Indice_Jeu_Local_iaj_change_univVars,
            536);
        AI2C_ResolveBuffer(&Indice_Jeu_Local_iaj_change_univVars_world_ID, (void*)Indice_Jeu_Local_iaj_change_univVars, 4);
        AI2C_ResolveBuffer(
            &Indice_Jeu_Local_iaj_change_univVars_Flag_speciaux_sauve,
            (void*)Indice_Jeu_Local_iaj_change_univVars,
            4488);
    }
    v32 = *(tAI**)(Indice_Jeu_Local_iaj_change_univVars_Perso_ID_actor + 4 * v9->des_i_companion + 4);
    if (v9->des_i_companion != *(uint32_t*)Indice_Jeu_Local_iaj_change_GST_Global_gao_i_compagnon_qui_parle)
    {
        if (v9->des_i_companion < 3)
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Indice_Jeu_Local_iaj_change_Indice_Jeu_Local_ijl_wait);
    LABEL_34:
        AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    if (*(uint32_t*)(Indice_Jeu_Local_iaj_change_univVars_Perso_ID_status + 4 * v9->des_i_companion + 4)
        && *(uint32_t*)(Indice_Jeu_Local_iaj_change_univVars_Perso_ID_status + 4 * v9->des_i_companion + 4) != 1
        && *(uint32_t*)(Indice_Jeu_Local_iaj_change_univVars_Perso_ID_status + 4 * v9->des_i_companion + 4) != 5
        && (*(uint32_t*)(Indice_Jeu_Local_iaj_change_univVars_Perso_ID_status + 4 * v9->des_i_companion + 4) != 4
            || *(uint32_t*)(Indice_Jeu_Local_iaj_change_univVars_Perso_ID_world_ID + 4 * v9->des_i_companion + 4) != *(uint32_t*)Indice_Jeu_Local_iaj_change_univVars_world_ID))
    {
        if (v9->des_i_companion < 3)
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Indice_Jeu_Local_iaj_change_Indice_Jeu_Local_ijl_wait);
        goto LABEL_34;
    }
    for (i = 0; i < 5; ++i)
    {
        if (v32 == WOR_gst_Universe.univAIInstance)
        {
            v29 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v31 = *(tGameObjectMain**)(Indice_Jeu_Local_iaj_change_univVars_Perso_ID_actor + 4 * v9->des_i_companion + 4);
            if (v32)
            {
                if ((unsigned int)v32 <= 2)
                    v31 = AI_MainActor[(uint32_t)v32 - 1];
                v30 = v31;
            }
            else
            {
                v30 = 0;
            }
            v29 = v30->extended->AIInstance;
        }
        if (((tScriptStruct_Companion*)(v29->instanceVarDataCurrent))->ai_current_positions[i] == -1)
            break;
    }
    if (i == 5)
    {
        if (v9->des_i_companion < 3)
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Indice_Jeu_Local_iaj_change_Indice_Jeu_Local_ijl_wait);
        goto LABEL_34;
    }
    v37 = i;
    for (j = v9->des_i_niveau; j < 10; ++j)
    {
        if (v32 == WOR_gst_Universe.univAIInstance)
        {
            v26 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v28 = *(tGameObjectMain**)(Indice_Jeu_Local_iaj_change_univVars_Perso_ID_actor + 4 * v9->des_i_companion + 4);
            if (v32)
            {
                if ((unsigned int)v32 <= 2)
                    v28 = AI_MainActor[(uint32_t)v32 - 1];
                v27 = v28;
            }
            else
            {
                v27 = 0;
            }
            v26 = v27->extended->AIInstance;
        }
        if (!((tScriptStruct_Companion*)(v26->instanceVarDataCurrent))->ai_consequences[j])
            break;
    }
    if (j == 10)
    {
        if (v9->des_i_companion < 3)
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Indice_Jeu_Local_iaj_change_Indice_Jeu_Local_ijl_wait);
        AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
    }
    else
    {
        num = j;
    }
    for (a4 = 0; a4 < 5 && v37 < 4; ++a4)
    {
        a1 = (int*)(Indice_Jeu_Local_iaj_change_univVars_Flag_speciaux_sauve + 4);
        v8 = *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v9->__ai_conditions_Dimension1, 3, a4);
        v5 = *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v9->__ai_conditions_Dimension1, 2, a4);
        v4 = *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v9->__ai_conditions_Dimension1, 1, a4);
        v1 = (int*)AI_EvalVarArray2_C(1, (char*)&v9->__ai_conditions_Dimension1, 0, a4);
        if (AI_EvalFunc_ARRBit4Test_C(a1, *v1, v4, v5, v8))
        {
            if (v9->ai_grisage[a4])
            {
                if (v32 == WOR_gst_Universe.univAIInstance)
                {
                    v22 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v24 = (tGameObjectMain*)v32;
                    if (v32)
                    {
                        if ((unsigned int)v32 <= 2)
                            v24 = AI_MainActor[(uint32_t)v32 - 1];
                        v23 = v24;
                    }
                    else
                    {
                        v23 = 0;
                    }
                    v22 = v23->extended->AIInstance;
                }
                v6 = (const void*)AI_EvalVarArray2_C(10, (char*)&v9->__at_questionsReponses_Dimension1, 0, a4);
                v2 = AI_EvalVarArray_C(10, (char*)&((tScriptStruct_Companion*)(v22->instanceVarDataCurrent))->__at_questions_Dimension1, num);
                memcpy(v2, v6, 8u);
                if (v32 == WOR_gst_Universe.univAIInstance)
                {
                    v19 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v21 = (tGameObjectMain*)v32;
                    if (v32)
                    {
                        if ((unsigned int)v32 <= 2)
                            v21 = AI_MainActor[(uint32_t)v32 - 1];
                        v20 = v21;
                    }
                    else
                    {
                        v20 = 0;
                    }
                    v19 = v20->extended->AIInstance;
                }
                v7 = (const void*)AI_EvalVarArray2_C(10, (char*)&v9->__at_questionsReponses_Dimension1, 1, a4);
                v3 = AI_EvalVarArray_C(10, (char*)&((tScriptStruct_Companion*)(v19->instanceVarDataCurrent))->__at_answers_Dimension1, num);
                memcpy(v3, v7, 8u);
                if (v32 == WOR_gst_Universe.univAIInstance)
                {
                    v16 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v18 = (tGameObjectMain*)v32;
                    if (v32)
                    {
                        if ((unsigned int)v32 <= 2)
                            v18 = AI_MainActor[(uint32_t)v32 - 1];
                        v17 = v18;
                    }
                    else
                    {
                        v17 = 0;
                    }
                    v16 = v17->extended->AIInstance;
                }
                ((tScriptStruct_Companion*)(v16->instanceVarDataCurrent))->ai_grisage[num] = v9->ai_grisage[a4];
                if (v32 == WOR_gst_Universe.univAIInstance)
                {
                    v13 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v15 = (tGameObjectMain*)v32;
                    if (v32)
                    {
                        if ((unsigned int)v32 <= 2)
                            v15 = AI_MainActor[(uint32_t)v32 - 1];
                        v14 = v15;
                    }
                    else
                    {
                        v14 = 0;
                    }
                    v13 = v14->extended->AIInstance;
                }
                ((tScriptStruct_Companion*)(v13->instanceVarDataCurrent))->ai_consequences[num] = num;
                if (v32 == WOR_gst_Universe.univAIInstance)
                {
                    v10 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v12 = (tGameObjectMain*)v32;
                    if (v32)
                    {
                        if ((unsigned int)v32 <= 2)
                            v12 = AI_MainActor[(uint32_t)v32 - 1];
                        v11 = v12;
                    }
                    else
                    {
                        v11 = 0;
                    }
                    v10 = v11->extended->AIInstance;
                }
                ((tScriptStruct_Companion*)(v10->instanceVarDataCurrent))->ai_current_positions[v37++] = num;
                if (++num - v9->des_i_niveau == 5 || v37 == 5)
                    break;
            }
        }
    }
    if (v9->des_i_companion < 3)
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Indice_Jeu_Local_iaj_change_Indice_Jeu_Local_ijl_wait);
    AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
    return 1000;
}

int Indice_Jeu_Local_ijl_wait()
{
    BOOL v0; // eax
    tGameObjectMain* v2; // [esp+8h] [ebp-14h]
    int v3; // [esp+14h] [ebp-8h]

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Indice_Jeu_Local_ijl_wait__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        fct_Indice_Jeu_Local_ijl_wait_Indice_Jeu_Local_iaj_change = (int)AI_ul_RealResolveOneRefFunction(0x9E00302E);
    }
    v2 = (tGameObjectMain*)Indice_Jeu_Local_ijl_wait__PJ_Sally_gao;
    if (Indice_Jeu_Local_ijl_wait__PJ_Sally_gao)
    {
        if ((unsigned int)Indice_Jeu_Local_ijl_wait__PJ_Sally_gao <= 2)
            v2 = AI_MainActor[Indice_Jeu_Local_ijl_wait__PJ_Sally_gao - 1];
        v0 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, v2);
    }
    else
    {
        v0 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, 0);
    }
    if (v0)
    {
        v3 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Indice_Jeu_Local_ijl_wait_Indice_Jeu_Local_iaj_change);
        if (v3 != 1000)
            return v3;
    }
    else
    {
        AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
    }
    return 1000;
}