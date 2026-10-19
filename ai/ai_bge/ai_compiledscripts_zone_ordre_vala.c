#include "ai_compiledscripts_inc.h"

//Zone_OrdreVala_zov_ini
int fct_Zone_OrdreVala_zov_ini_Zone_OrdreVala_zov_init;

//Zone_OrdreVala_zov_wait
int Zone_OrdreVala_zov_wait_univVars;
int Zone_OrdreVala_zov_wait_univVars_Perso_ID_actor;
int Zone_OrdreVala_zov_wait_univVars_Perso_ID_status;
int Zone_OrdreVala_zov_wait_TXT_Sally_gao;
int Zone_OrdreVala_zov_wait_GST_Global_gao;
int Zone_OrdreVala_zov_wait_GST_Global_gao_o_actor_texte;

//Zone_OrdreVala_zov_init
int fct_Zone_OrdreVala_zov_init_Zone_OrdreVala_zov_wait;

int Zone_OrdreVala_zov_ini()
{
    int result; // eax
    tScriptStruct_ZoneOrdreVala* v1; // [esp+0h] [ebp-2Ch]
    int v2; // [esp+24h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ZoneOrdreVala*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Zone_OrdreVala_zov_ini_Zone_OrdreVala_zov_init = (int)AI_ul_RealResolveOneRefFunction(0x9E003DCD);
    }
    v1->ai_compagnon_qui_agit[0] = v1->i_Peyj_vala;
    v1->ai_compagnon_qui_agit[1] = v1->i_Mili_vala;
    v1->ai_compagnon_qui_agit[2] = v1->i_Domz_vala;
    v1->ai_compagnon_qui_agit[3] = v1->i_XB25_vala;
    v1->ai_compagnon_qui_agit[4] = v1->i_XB26_vala;
    v1->i_replique_limite = AI_EvalFunc_MATHRandInt_C(0, 5);
    if (v1->i_cancel_vala)
        v1->f_time_attente = -1.0;
    v2 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Zone_OrdreVala_zov_ini_Zone_OrdreVala_zov_init);
    if (v2 == 1000)
        result = 1000;
    else
        result = v2;
    return result;
}

int Zone_OrdreVala_zov_wait()
{
    int v0; // eax
    int v1; // eax
    int result; // eax
    int v3; // eax
    tScriptStruct_ZoneOrdreVala* v4; // [esp+8h] [ebp-12Ch]
    tGameObjectMain* v5; // [esp+10h] [ebp-124h]
    tGameObjectMain* v6; // [esp+14h] [ebp-120h]
    tGameObjectMain* v7; // [esp+18h] [ebp-11Ch]
    tGameObjectMain* v8; // [esp+20h] [ebp-114h]
    tGameObjectMain* v9; // [esp+28h] [ebp-10Ch]
    tGameObjectMain* v10; // [esp+30h] [ebp-104h]
    tGameObjectMain* v11; // [esp+34h] [ebp-100h]
    tGameObjectMain* v12; // [esp+38h] [ebp-FCh]
    tGameObjectMain* v13; // [esp+40h] [ebp-F4h]
    tGameObjectMain* v14; // [esp+48h] [ebp-ECh]
    tMessage v15; // [esp+64h] [ebp-D0h] BYREF
    unsigned int v16; // [esp+98h] [ebp-9Ch]
    int v17; // [esp+B4h] [ebp-80h]
    unsigned int v18; // [esp+B8h] [ebp-7Ch]
    tMessage Src; // [esp+D4h] [ebp-60h] BYREF
    tGameObjectMain* v20; // [esp+108h] [ebp-2Ch]
    int v21; // [esp+110h] [ebp-24h]
    tScriptStruct_ZoneOrdreVala* v22; // [esp+114h] [ebp-20h]
    int i; // [esp+118h] [ebp-1Ch]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_ZoneOrdreVala*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v22 = v4;
    v16 = 0;
    v20 = 0;
    v18 = 0;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Zone_OrdreVala_zov_wait_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&Zone_OrdreVala_zov_wait_univVars_Perso_ID_actor, (void*)Zone_OrdreVala_zov_wait_univVars, 988);
        AI2C_ResolveBuffer(&Zone_OrdreVala_zov_wait_univVars_Perso_ID_status, (void*)Zone_OrdreVala_zov_wait_univVars, 484);
        Zone_OrdreVala_zov_wait_TXT_Sally_gao = AI_ul_RealResolveOneRefObject(0x7200064Eu);
        Zone_OrdreVala_zov_wait_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Zone_OrdreVala_zov_wait_GST_Global_gao_o_actor_texte,
            (void*)Zone_OrdreVala_zov_wait_GST_Global_gao,
            3616);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)4)
    {
        for (i = 0; ; ++i)
        {
            if (i >= 5)
            {
                if (v22->i_sedesactive_apres_ordre)
                {
                    v22->i_done = 0;
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(AI_gpst_CurrentGameObject, 1);
                }
                goto LABEL_80;
            }
            v16 = *(uint32_t*)(Zone_OrdreVala_zov_wait_univVars_Perso_ID_actor + 4 * i + 4);
            v12 = (tGameObjectMain*)v16;
            if (v16)
            {
                if (v16 <= 2)
                    v12 = AI_MainActor[v16 - 1];
                v11 = v12;
            }
            else
            {
                v11 = 0;
            }
            if (v11
                && v22->ai_compagnon_qui_agit[i]
                && (!*(uint32_t*)(Zone_OrdreVala_zov_wait_univVars_Perso_ID_status + 4 * i + 4)
                    || *(uint32_t*)(Zone_OrdreVala_zov_wait_univVars_Perso_ID_status + 4 * i + 4) == 4))
            {
                if (v22->i_replique != -1)
                {
                    v18 = v22->i_Perso_ID_replique_acteur == 8 ? Zone_OrdreVala_zov_wait_TXT_Sally_gao : *(uint32_t*)(Zone_OrdreVala_zov_wait_GST_Global_gao_o_actor_texte + 4 * v22->i_Perso_ID_replique_acteur + 4);
                    v20 = *(tGameObjectMain**)(Zone_OrdreVala_zov_wait_univVars_Perso_ID_actor
                        + 4 * v22->i_Perso_ID_replique_acteur
                        + 4);
                    v3 = AI_EvalFunc_MTHModulo_C(v22->i_replique_limite + 1, 5);
                    v22->i_replique_limite = v3;
                    if (!v22->i_replique_limite)
                    {
                        Src.msgID = 30;
                        Src.Int1 = v22->i_replique;
                        Src.Int2 = 0;
                        Src.f2 = 2.0;
                        Src.v1.x = 0.0;
                        Src.v1.y = 0.0;
                        Src.v1.z = 0.0;
                        Src.v2.x = 0.0;
                        Src.v2.y = 0.0;
                        Src.v2.z = 0.0;
                        Src.gao = 0;
                        Src.gao2 = v20;
                        v10 = (tGameObjectMain*)v18;
                        if (v18)
                        {
                            if (v18 <= 2)
                                v10 = AI_MainActor[v18 - 1];
                            AI_EvalFunc_MSGSend_C(v10, &Src);
                        }
                        else
                        {
                            AI_EvalFunc_MSGSend_C(0, &Src);
                        }
                    }
                }
                if (v22->i_mode_de_deplacement)
                {
                    v15.msgID = 6;
                    v15.Int1 = 6;
                    v15.Int2 = 0;
                    v15.f2 = v22->f_time_attente;
                    v15.f1 = 110.0;
                    v15.v1.x = 0.0;
                    v15.v1.y = 0.0;
                    v15.v1.z = 0.0;
                    v15.v2.x = 0.0;
                    v15.v2.y = 0.0;
                    v15.v2.z = 0.0;
                    v15.gao = (tGameObjectMain*)v22->o_destinationVala[i];
                    v15.gao2 = 0;
                    v8 = (tGameObjectMain*)v16;
                    if (!v16)
                    {
                    LABEL_48:
                        AI_EvalFunc_MSGSend_C(0, &v15);
                        continue;
                    }
                    if (v16 <= 2)
                        v8 = AI_MainActor[v16 - 1];
                    AI_EvalFunc_MSGSend_C(v8, &v15);
                }
                else
                {
                    v15.msgID = 6;
                    v15.Int1 = 1;
                    v15.Int2 = 0;
                    v15.f2 = v22->f_time_attente;
                    v15.f1 = 100.0;
                    v15.v1.x = 0.0;
                    v15.v1.y = 0.0;
                    v15.v1.z = 0.0;
                    v15.v2.x = 0.0;
                    v15.v2.y = 0.0;
                    v15.v2.z = 0.0;
                    v15.gao = (tGameObjectMain*)v22->o_destinationVala[i];
                    v15.gao2 = 0;
                    v9 = (tGameObjectMain*)v16;
                    if (!v16)
                        goto LABEL_48;
                    if (v16 <= 2)
                        v9 = AI_MainActor[v16 - 1];
                    AI_EvalFunc_MSGSend_C(v9, &v15);
                }
            }
        }
    }
    if (!v22->i_Detect)
    {
        v14 = (tGameObjectMain*)v22->o_actor_a_detecter;
        if (v22->o_actor_a_detecter)
        {
            if ((unsigned int)v14 <= 2)
                v14 = AI_MainActor[(uint32_t)v14 - 1];
            v0 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, v14);
        }
        else
        {
            v0 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, 0);
        }
        v17 = v0;
        if (v0)
            goto LABEL_83;
    }
    if (!v22->i_Detect)
        goto LABEL_84;
    v13 = (tGameObjectMain*)v22->o_actor_a_detecter;
    if (v22->o_actor_a_detecter)
    {
        if ((unsigned int)v13 <= 2)
            v13 = AI_MainActor[(uint32_t)v13 - 1];
        v1 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, v13);
    }
    else
    {
        v1 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, 0);
    }
    v21 = v1;
    if (v1)
    {
    LABEL_84:
        if (v22->i_done)
        {
            if (v22->i_cancel_vala)
            {
                for (i = 0; i < 5; ++i)
                {
                    v16 = *(uint32_t*)(Zone_OrdreVala_zov_wait_univVars_Perso_ID_actor + 4 * i + 4);
                    v7 = (tGameObjectMain*)v16;
                    if (v16)
                    {
                        if (v16 <= 2)
                            v7 = AI_MainActor[v16 - 1];
                        v6 = v7;
                    }
                    else
                    {
                        v6 = 0;
                    }
                    if (v6
                        && v22->ai_compagnon_qui_agit[i]
                        && (!*(uint32_t*)(Zone_OrdreVala_zov_wait_univVars_Perso_ID_status + 4 * i + 4)
                            || *(uint32_t*)(Zone_OrdreVala_zov_wait_univVars_Perso_ID_status + 4 * i + 4) == 4))
                    {
                        v15.msgID = 6;
                        v15.Int1 = 0;
                        v15.Int2 = 0;
                        v15.f2 = 0.0;
                        v15.f1 = 110.0;
                        v15.v1.x = 0.0;
                        v15.v1.y = 0.0;
                        v15.v1.z = 0.0;
                        v15.v2.x = 0.0;
                        v15.v2.y = 0.0;
                        v15.v2.z = 0.0;
                        v15.gao = 0;
                        v15.gao2 = 0;
                        v5 = (tGameObjectMain*)v16;
                        if (v16)
                        {
                            if (v16 <= 2)
                                v5 = AI_MainActor[v16 - 1];
                            AI_EvalFunc_MSGSend_C(v5, &v15);
                        }
                        else
                        {
                            AI_EvalFunc_MSGSend_C(0, &v15);
                        }
                    }
                }
            }
            v22->i_done = 0;
        }
        else
        {
            v22->i_done = 0;
        }
    LABEL_80:
        result = 1000;
    }
    else
    {
    LABEL_83:
        if (v22->i_done)
            goto LABEL_80;
        v22->i_done = 1;
        *AI_gppst_CurrentJumpNode = (tAINode*)4;
        TIME_Wait_C(v22->f_delai_avant_envoi_vala);
        result = 0;
    }
    return result;
}

int Zone_OrdreVala_zov_init()
{
    int v0; // eax
    int result; // eax
    tScriptStruct_ZoneOrdreVala* v2; // [esp+0h] [ebp-18h]
    tGameObjectMain* v3; // [esp+8h] [ebp-10h]
    int v4; // [esp+10h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ZoneOrdreVala*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Zone_OrdreVala_zov_init_Zone_OrdreVala_zov_wait = (int)AI_ul_RealResolveOneRefFunction(0x9E003DCF);
    }
    v2->i_done = 0;
    if (v2->i_Detect)
    {
        v3 = (tGameObjectMain*)v2->o_actor_a_detecter;
        if (v2->o_actor_a_detecter)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            v0 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, v3);
        }
        else
        {
            v0 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, 0);
        }
        if (!v0)
            goto LABEL_15;
    }
    v4 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Zone_OrdreVala_zov_init_Zone_OrdreVala_zov_wait);
    if (v4 == 1000)
        LABEL_15:
    result = 1000;
    else
        result = v4;
    return result;
}