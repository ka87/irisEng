#include "ai_compiledscripts_inc.h"

//Dog_Fight_Manager_DFM_init
int Dog_Fight_Manager_DFM_init_univVars;
int Dog_Fight_Manager_DFM_init_univVars_Flag_speciaux_sauve;
int fctvar_Dog_Fight_Manager_DFM_init_Domz_Sansue;
int fctvar_Dog_Fight_Manager_DFM_init_Twin_Ion_Engine;
int fct_Dog_Fight_Manager_DFM_init_Dog_Fight_Manager_DFM_comportement;

//Dog_Fight_Manager_DFM_comportement
int Dog_Fight_Manager_DFM_comportement_univVars;
int Dog_Fight_Manager_DFM_comportement_univVars_Flag_speciaux_sauve;

int Dog_Fight_Manager_DFM_init()
{
    int result; // eax
    BOOL v1; // eax
    BOOL v2; // eax
    BOOL v3; // eax
    tScriptStruct_DogFightManager* v4; // [esp+4h] [ebp-BCh]
    tGameObjectMain* v5; // [esp+Ch] [ebp-B4h]
    tGameObjectMain* v6; // [esp+14h] [ebp-ACh]
    tGameObjectMain* v7; // [esp+1Ch] [ebp-A4h]
    tGameObjectMain* v8; // [esp+24h] [ebp-9Ch]
    tGameObjectMain* v9; // [esp+28h] [ebp-98h]
    tGameObjectMain* v10; // [esp+2Ch] [ebp-94h]
    tGameObjectMain* v11; // [esp+3Ch] [ebp-84h]
    tGameObjectMain* v12; // [esp+40h] [ebp-80h]
    tGameObjectMain* v13; // [esp+44h] [ebp-7Ch]
    tGameObjectMain* v14; // [esp+48h] [ebp-78h]
    tGameObjectMain* v15; // [esp+50h] [ebp-70h]
    tGameObjectMain* v16; // [esp+54h] [ebp-6Ch]
    tGameObjectMain* v17; // [esp+58h] [ebp-68h]
    tAI* v18; // [esp+5Ch] [ebp-64h]
    unsigned int v19; // [esp+60h] [ebp-60h]
    tGameObjectMain* v20; // [esp+64h] [ebp-5Ch]
    tGameObjectMain* v21; // [esp+68h] [ebp-58h]
    tAI* v22; // [esp+6Ch] [ebp-54h]
    unsigned int v23; // [esp+70h] [ebp-50h]
    tGameObjectMain* v24; // [esp+74h] [ebp-4Ch]
    tGameObjectMain* v25; // [esp+78h] [ebp-48h]
    tGameObjectMain* v26; // [esp+80h] [ebp-40h]
    tGameObjectMain* v27; // [esp+84h] [ebp-3Ch]
    unsigned int v28; // [esp+A4h] [ebp-1Ch]
    int v29; // [esp+B4h] [ebp-Ch]
    int i; // [esp+B4h] [ebp-Ch]
    int j; // [esp+B4h] [ebp-Ch]
    int k; // [esp+B4h] [ebp-Ch]
    int l; // [esp+B4h] [ebp-Ch]
    int v34; // [esp+B8h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_DogFightManager*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Dog_Fight_Manager_DFM_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Dog_Fight_Manager_DFM_init_univVars_Flag_speciaux_sauve,
            (void*)Dog_Fight_Manager_DFM_init_univVars,
            4488);
        fctvar_Dog_Fight_Manager_DFM_init_Domz_Sansue = AI_ul_RealResolveOneRef(42, 0x72004C60u);
        fctvar_Dog_Fight_Manager_DFM_init_Twin_Ion_Engine = AI_ul_RealResolveOneRef(42, 0x72002063u);
        fct_Dog_Fight_Manager_DFM_init_Dog_Fight_Manager_DFM_comportement = (int)AI_ul_RealResolveOneRefFunction(0x720020A1u);
    }
    v27 = (tGameObjectMain*)v4->des_gameplay_manager;
    if (v4->des_gameplay_manager)
    {
        if ((unsigned int)v27 <= 2)
            v27 = AI_MainActor[(uint32_t)v27 - 1];
        v26 = v27;
    }
    else
    {
        v26 = 0;
    }
    if (!v26)
        goto LABEL_43;
    if (!AI_EvalFunc_ARRBitTest_C((int*)(Dog_Fight_Manager_DFM_init_univVars_Flag_speciaux_sauve + 4), 2789))
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 4;
    }
    v23 = v4->des_gameplay_manager;
    if ((tAI*)v4->des_gameplay_manager == WOR_gst_Universe.univAIInstance)
    {
        v22 = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        v25 = (tGameObjectMain*)v4->des_gameplay_manager;
        if (v23)
        {
            if (v23 <= 2)
                v25 = AI_MainActor[v23 - 1];
            v24 = v25;
        }
        else
        {
            v24 = 0;
        }
        v22 = v24->extended->AIInstance;
    }
    v19 = v4->des_gameplay_manager;
    if ((tAI*)v4->des_gameplay_manager == WOR_gst_Universe.univAIInstance)
    {
        v18 = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        v21 = (tGameObjectMain*)v4->des_gameplay_manager;
        if (v19)
        {
            if (v19 <= 2)
                v21 = AI_MainActor[v19 - 1];
            v20 = v21;
        }
        else
        {
            v20 = 0;
        }
        v18 = v20->extended->AIInstance;
    }
    v29 = ((tScriptStruct_GSTHyllisManager*)(v22->instanceVarDataCurrent))->i_last_gp;
    v28 = ((tScriptStruct_GSTHyllisManager*)(v18->instanceVarDataCurrent))->des_ao_allgp[v29];
    v17 = (tGameObjectMain*)v28;
    if (v28)
    {
        if (v28 <= 2)
            v17 = AI_MainActor[v28 - 1];
        v16 = v17;
    }
    else
    {
        v16 = 0;
    }
    if (!v16)
        goto LABEL_42;
    v15 = (tGameObjectMain*)((tScriptStruct_GSTHyllisManager*)(v18->instanceVarDataCurrent))->des_ao_allgp[v29];
    if (v28)
    {
        if (v28 <= 2)
            v15 = AI_MainActor[v28 - 1];
        v1 = AI_EvalFunc_AIIsModel_C(v15, (tAIModel*)fctvar_Dog_Fight_Manager_DFM_init_Domz_Sansue);
    }
    else
    {
        v1 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_Dog_Fight_Manager_DFM_init_Domz_Sansue);
    }
    if (v1)
    {
    LABEL_42:
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 4;
    }
    else
    {
    LABEL_43:
        AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x8000, 0);
        for (i = 0; i < 10; ++i)
        {
            v14 = (tGameObjectMain*)v4->des_o_ennemi[i];
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
            if (!v13)
                break;
            v4->ao_ennemi[v4->i_ennemi_nb++] = v4->des_o_ennemi[i];
        }
        v12 = (tGameObjectMain*)v4->des_gameplay_manager;
        if (v4->des_gameplay_manager)
        {
            if ((unsigned int)v12 <= 2)
                v12 = AI_MainActor[(uint32_t)v12 - 1];
            v11 = v12;
        }
        else
        {
            v11 = 0;
        }
        if (!v11)
        {
            v4->ao_ami[0] = (int)*AI_MainActor;
            v4->i_ami_nb = 1;
        }
        for (j = 0; j < 30; ++j)
        {
            v10 = (tGameObjectMain*)v4->des_o_ami[j];
            if (v10)
            {
                if ((unsigned int)v10 <= 2)
                    v10 = AI_MainActor[(uint32_t)v10 - 1];
                v9 = v10;
            }
            else
            {
                v9 = 0;
            }
            if (!v9)
                break;
            v4->ao_ami[v4->i_ami_nb++] = v4->des_o_ami[j];
        }
        for (k = 0; k < v4->i_ennemi_nb; ++k)
        {
            v8 = (tGameObjectMain*)v4->ao_ennemi[k];
            if (v8)
            {
                if ((unsigned int)v8 <= 2)
                    v8 = AI_MainActor[(uint32_t)v8 - 1];
                v2 = AI_EvalFunc_AIIsModel_C(v8, (tAIModel*)fctvar_Dog_Fight_Manager_DFM_init_Twin_Ion_Engine);
            }
            else
            {
                v2 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_Dog_Fight_Manager_DFM_init_Twin_Ion_Engine);
            }
            if (v2)
            {
                v4->m_msg.msgID = 16;
                v4->m_msg.gao = AI_gpst_CurrentGameObject;
                v4->m_msg.gao2 = (tGameObjectMain*)v4->ao_ami[v4->i_next_friend_index];
                v4->m_msg.Int1 = 65280;
                v4->m_msg.Int2 = 1;
                v7 = (tGameObjectMain*)v4->ao_ennemi[k];
                if (v7)
                {
                    if ((unsigned int)v7 <= 2)
                        v7 = AI_MainActor[(uint32_t)v7 - 1];
                    AI_EvalFunc_MSGSend_C(v7, &v4->m_msg);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &v4->m_msg);
                }
                v4->ao_ennemi_cible[k] = v4->ao_ami[v4->i_next_friend_index];
                v4->i_next_friend_index = AI_EvalFunc_MTHModulo_C(v4->i_next_friend_index + 1, v4->i_ami_nb);
            }
        }
        for (l = 0; l < v4->i_ami_nb; ++l)
        {
            v6 = (tGameObjectMain*)v4->ao_ami[l];
            if (v6)
            {
                if ((unsigned int)v6 <= 2)
                    v6 = AI_MainActor[(uint32_t)v6 - 1];
                v3 = AI_EvalFunc_AIIsModel_C(v6, (tAIModel*)fctvar_Dog_Fight_Manager_DFM_init_Twin_Ion_Engine);
            }
            else
            {
                v3 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_Dog_Fight_Manager_DFM_init_Twin_Ion_Engine);
            }
            if (v3)
            {
                v4->m_msg.msgID = 16;
                v4->m_msg.gao = AI_gpst_CurrentGameObject;
                v4->m_msg.gao2 = (tGameObjectMain*)v4->ao_ennemi[v4->i_next_enemy_index];
                v4->m_msg.Int1 = 16639;
                v4->m_msg.Int2 = 0;
                v5 = (tGameObjectMain*)v4->ao_ami[l];
                if (v5)
                {
                    if ((unsigned int)v5 <= 2)
                        v5 = AI_MainActor[(uint32_t)v5 - 1];
                    AI_EvalFunc_MSGSend_C(v5, &v4->m_msg);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &v4->m_msg);
                }
                v4->ao_ami_cible[l] = v4->ao_ennemi[v4->i_next_enemy_index];
                v4->i_next_enemy_index = AI_EvalFunc_MTHModulo_C(v4->i_next_enemy_index + 1, v4->i_ennemi_nb);
            }
        }
        v34 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Dog_Fight_Manager_DFM_init_Dog_Fight_Manager_DFM_comportement);
        if (v34 == 1000)
            result = 1000;
        else
            result = v34;
    }
    return result;
}

int Dog_Fight_Manager_DFM_comportement()
{
    tScriptStruct_DogFightManager* v1; // [esp+4h] [ebp-130h]
    tGameObjectMain* v2; // [esp+Ch] [ebp-128h]
    tGameObjectMain* v3; // [esp+14h] [ebp-120h]
    tGameObjectMain* v4; // [esp+1Ch] [ebp-118h]
    tGameObjectMain* v5; // [esp+20h] [ebp-114h]
    tGameObjectMain* v6; // [esp+24h] [ebp-110h]
    tGameObjectMain* v7; // [esp+28h] [ebp-10Ch]
    tGameObjectMain* v8; // [esp+2Ch] [ebp-108h]
    tGameObjectMain* v9; // [esp+38h] [ebp-FCh]
    tGameObjectMain* v10; // [esp+3Ch] [ebp-F8h]
    tGameObjectMain* v11; // [esp+48h] [ebp-ECh]
    tGameObjectMain* v12; // [esp+50h] [ebp-E4h]
    tGameObjectMain* v13; // [esp+54h] [ebp-E0h]
    tGameObjectMain* v14; // [esp+58h] [ebp-DCh]
    tGameObjectMain* v15; // [esp+5Ch] [ebp-D8h]
    tGameObjectMain* v16; // [esp+68h] [ebp-CCh]
    tGameObjectMain* v17; // [esp+6Ch] [ebp-C8h]
    tGameObjectMain* v18; // [esp+74h] [ebp-C0h]
    tGameObjectMain* v19; // [esp+78h] [ebp-BCh]
    tGameObjectMain* a3; // [esp+80h] [ebp-B4h]
    tGameObjectMain* v21; // [esp+84h] [ebp-B0h]
    tGameObjectMain* v22; // [esp+90h] [ebp-A4h]
    tGameObjectMain* v23; // [esp+98h] [ebp-9Ch]
    tGameObjectMain* v24; // [esp+9Ch] [ebp-98h]
    tGameObjectMain* v25; // [esp+A0h] [ebp-94h]
    tGameObjectMain* v26; // [esp+A4h] [ebp-90h]
    tGameObjectMain* v27; // [esp+B0h] [ebp-84h]
    tGameObjectMain* v28; // [esp+B4h] [ebp-80h]
    tGameObjectMain* v29; // [esp+B8h] [ebp-7Ch]
    tGameObjectMain* v30; // [esp+BCh] [ebp-78h]
    tGameObjectMain* v31; // [esp+CCh] [ebp-68h]
    tGameObjectMain* v32; // [esp+D0h] [ebp-64h]
    tGameObjectMain* v33; // [esp+D8h] [ebp-5Ch]
    tGameObjectMain* v34; // [esp+DCh] [ebp-58h]
    void* Src; // [esp+E0h] [ebp-54h]
    tGameObjectMain* v36; // [esp+F8h] [ebp-3Ch]
    int v37; // [esp+100h] [ebp-34h]
    int v38; // [esp+108h] [ebp-2Ch]
    unsigned int v39; // [esp+118h] [ebp-1Ch]
    tGameObjectMain* v40; // [esp+11Ch] [ebp-18h]
    int i; // [esp+124h] [ebp-10h]
    int k; // [esp+124h] [ebp-10h]
    int j; // [esp+124h] [ebp-10h]
    void* v44; // [esp+130h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_DogFightManager*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Dog_Fight_Manager_DFM_comportement_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Dog_Fight_Manager_DFM_comportement_univVars_Flag_speciaux_sauve,
            (void*)Dog_Fight_Manager_DFM_comportement_univVars,
            4488);
    }
    v34 = (tGameObjectMain*)v1->des_gameplay_manager;
    if (v1->des_gameplay_manager)
    {
        if ((unsigned int)v34 <= 2)
            v34 = AI_MainActor[(uint32_t)v34 - 1];
        v33 = v34;
    }
    else
    {
        v33 = 0;
    }
    if (v33
        && !AI_EvalFunc_ARRBitTest_C((int*)(Dog_Fight_Manager_DFM_comportement_univVars_Flag_speciaux_sauve + 4), 2789))
    {
        AI_EvalFunc_OBJReinit_C(AI_gpst_CurrentGameObject, 1);
    }
    v32 = (tGameObjectMain*)v1->des_gameplay_manager;
    if (v1->des_gameplay_manager)
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
    {
        v40 = *AI_MainActor;
        v30 = (tGameObjectMain*)v1->ao_ami[0];
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
        v28 = *AI_MainActor;
        if (v40)
        {
            if ((unsigned int)v40 <= 2)
                v28 = AI_MainActor[(uint32_t)v40 - 1];
            v27 = v28;
        }
        else
        {
            v27 = 0;
        }
        if (v29 != v27)
        {
            v39 = v1->ao_ami[0];
            v1->ao_ami[0] = (int)*AI_MainActor;
            for (i = 0; i < v1->i_ennemi_nb; ++i)
            {
                v26 = (tGameObjectMain*)v1->ao_ennemi_cible[i];
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
                v24 = (tGameObjectMain*)v39;
                if (v39)
                {
                    if (v39 <= 2)
                        v24 = AI_MainActor[v39 - 1];
                    v23 = v24;
                }
                else
                {
                    v23 = 0;
                }
                if (v25 == v23)
                {
                    v1->m_msg.msgID = 16;
                    v1->m_msg.gao = AI_gpst_CurrentGameObject;
                    v1->m_msg.gao2 = (tGameObjectMain*)v1->ao_ami[0];
                    v22 = (tGameObjectMain*)v1->ao_ennemi[i];
                    if (v22)
                    {
                        if ((unsigned int)v22 <= 2)
                            v22 = AI_MainActor[(uint32_t)v22 - 1];
                        AI_EvalFunc_MSGSend_C(v22, &v1->m_msg);
                    }
                    else
                    {
                        AI_EvalFunc_MSGSend_C(0, &v1->m_msg);
                    }
                    v1->ao_ennemi_cible[i] = v1->ao_ami[0];
                }
            }
        }
    }
    Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v1->m_msg, Src, sizeof(v1->m_msg));
    while (v1->m_msg.msgID)
    {
        v36 = v1->m_msg.gao;
        v21 = v36;
        if (v36)
        {
            if ((unsigned int)v36 <= 2)
                v21 = AI_MainActor[(uint32_t)v36 - 1];
            a3 = v21;
        }
        else
        {
            a3 = 0;
        }
        v37 = AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)v1->ao_ami, v1->i_ami_nb, a3);
        v19 = v36;
        if (v36)
        {
            if ((unsigned int)v36 <= 2)
                v19 = AI_MainActor[(uint32_t)v36 - 1];
            v18 = v19;
        }
        else
        {
            v18 = 0;
        }
        v38 = AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)v1->ao_ennemi, v1->i_ennemi_nb, v18);
        if (v1->m_msg.msgID == 43)
        {
            if (v37 == -1 || !v37)
            {
                if (v38 != -1)
                {
                    v10 = (tGameObjectMain*)v1->des_gameplay_manager;
                    if (v1->des_gameplay_manager)
                    {
                        if ((unsigned int)v10 <= 2)
                            v10 = AI_MainActor[(uint32_t)v10 - 1];
                        v9 = v10;
                    }
                    else
                    {
                        v9 = 0;
                    }
                    if (!v9)
                    {
                        v1->ao_ennemi[v38] = v1->ao_ennemi[--v1->i_ennemi_nb];
                        v1->ao_ennemi_cible[v38] = v1->ao_ennemi_cible[v1->i_ennemi_nb];
                        v1->ao_ennemi[v1->i_ennemi_nb] = 0;
                        v1->ao_ennemi_cible[v1->i_ennemi_nb] = 0;
                    }
                    for (j = 0; j < v1->i_ami_nb; ++j)
                    {
                        if (v1->i_ennemi_nb)
                        {
                            v8 = (tGameObjectMain*)v1->ao_ami_cible[j];
                            if (v8)
                            {
                                if ((unsigned int)v8 <= 2)
                                    v8 = AI_MainActor[(uint32_t)v8 - 1];
                                v7 = v8;
                            }
                            else
                            {
                                v7 = 0;
                            }
                            v6 = v36;
                            if (v36)
                            {
                                if ((unsigned int)v36 <= 2)
                                    v6 = AI_MainActor[(uint32_t)v36 - 1];
                                v5 = v6;
                            }
                            else
                            {
                                v5 = 0;
                            }
                            if (v7 == v5)
                            {
                                v1->m_msg.msgID = 16;
                                v1->m_msg.gao = AI_gpst_CurrentGameObject;
                                v1->m_msg.gao2 = (tGameObjectMain*)v1->ao_ami_cible[v1->i_ennemi_nb + 31];
                                v4 = (tGameObjectMain*)v1->ao_ami[j];
                                if (v4)
                                {
                                    if ((unsigned int)v4 <= 2)
                                        v4 = AI_MainActor[(uint32_t)v4 - 1];
                                    AI_EvalFunc_MSGSend_C(v4, &v1->m_msg);
                                }
                                else
                                {
                                    AI_EvalFunc_MSGSend_C(0, &v1->m_msg);
                                }
                                v1->ao_ami_cible[j] = v1->ao_ami_cible[v1->i_ennemi_nb + 31];
                            }
                        }
                    }
                }
            }
            else
            {
                v17 = (tGameObjectMain*)v1->des_gameplay_manager;
                if (v1->des_gameplay_manager)
                {
                    if ((unsigned int)v17 <= 2)
                        v17 = AI_MainActor[(uint32_t)v17 - 1];
                    v16 = v17;
                }
                else
                {
                    v16 = 0;
                }
                if (!v16)
                {
                    v1->ao_ami[v37] = v1->ao_ami[--v1->i_ami_nb];
                    v1->ao_ami_cible[v37] = v1->ao_ami_cible[v1->i_ami_nb];
                    v1->ao_ami[v1->i_ami_nb] = 0;
                    v1->ao_ami_cible[v1->i_ami_nb] = 0;
                }
                for (k = 0; k < v1->i_ennemi_nb; ++k)
                {
                    v15 = (tGameObjectMain*)v1->ao_ennemi_cible[k];
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
                    v13 = v36;
                    if (v36)
                    {
                        if ((unsigned int)v36 <= 2)
                            v13 = AI_MainActor[(uint32_t)v36 - 1];
                        v12 = v13;
                    }
                    else
                    {
                        v12 = 0;
                    }
                    if (v14 == v12)
                    {
                        v1->m_msg.msgID = 16;
                        v1->m_msg.gao = AI_gpst_CurrentGameObject;
                        v1->m_msg.gao2 = (tGameObjectMain*)v1->ao_ami[0];
                        v11 = (tGameObjectMain*)v1->ao_ennemi[k];
                        if (v11)
                        {
                            if ((unsigned int)v11 <= 2)
                                v11 = AI_MainActor[(uint32_t)v11 - 1];
                            AI_EvalFunc_MSGSend_C(v11, &v1->m_msg);
                        }
                        else
                        {
                            AI_EvalFunc_MSGSend_C(0, &v1->m_msg);
                        }
                        v1->ao_ennemi_cible[k] = v1->ao_ami[0];
                    }
                }
            }
        }
        else if (v1->m_msg.msgID == 16)
        {
            if (v37 == -1)
            {
                if (v38 != -1)
                {
                    v1->m_msg.gao = AI_gpst_CurrentGameObject;
                    v1->m_msg.gao2 = (tGameObjectMain*)v1->ao_ami[v1->i_next_friend_index];
                    v1->m_msg.Int1 = 65280;
                    v1->m_msg.Int2 = 1;
                    v2 = v36;
                    if (v36)
                    {
                        if ((unsigned int)v36 <= 2)
                            v2 = AI_MainActor[(uint32_t)v36 - 1];
                        AI_EvalFunc_MSGSend_C(v2, &v1->m_msg);
                    }
                    else
                    {
                        AI_EvalFunc_MSGSend_C(0, &v1->m_msg);
                    }
                    v1->ao_ennemi_cible[v38] = v1->ao_ami[v1->i_next_friend_index];
                    v1->i_next_friend_index = AI_EvalFunc_MTHModulo_C(v1->i_next_friend_index + 1, v1->i_ami_nb);
                }
            }
            else
            {
                v1->m_msg.gao = AI_gpst_CurrentGameObject;
                v1->m_msg.gao2 = (tGameObjectMain*)v1->ao_ennemi[v1->i_next_enemy_index];
                v1->m_msg.Int1 = 16639;
                v1->m_msg.Int2 = 0;
                v3 = v36;
                if (v36)
                {
                    if ((unsigned int)v36 <= 2)
                        v3 = AI_MainActor[(uint32_t)v36 - 1];
                    AI_EvalFunc_MSGSend_C(v3, &v1->m_msg);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &v1->m_msg);
                }
                v1->ao_ami_cible[v37] = v1->ao_ennemi[v1->i_next_enemy_index];
                v1->i_next_enemy_index = AI_EvalFunc_MTHModulo_C(v1->i_next_enemy_index + 1, v1->i_ennemi_nb);
            }
        }
        v44 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v1->m_msg, v44, sizeof(v1->m_msg));
    }
    return 1000;
}