#include "ai_compiledscripts_inc.h"

//LecteurCarte_lcrt_wait
int LecteurCarte_lcrt_wait__PJ_Sally_gao;
int LecteurCarte_lcrt_wait_univVars;
int LecteurCarte_lcrt_wait_univVars_Perso_ID_equipement;
int LecteurCarte_lcrt_wait__PJ_Sally_gao_current_pm_mode;
int LecteurCarte_lcrt_wait__PJ_Sally_gao_current_pm_priority;
int LecteurCarte_lcrt_wait_GST_Board_panel_gao;
int LecteurCarte_lcrt_wait_GST_Board_panel_gao_i_use_prio;
int LecteurCarte_lcrt_wait_GST_Board_panel_gao_o_use_object;
int LecteurCarte_lcrt_wait_GST_Board_panel_gao_i_use_action;
int LecteurCarte_lcrt_wait_GST_Board_panel_gao_f_use_offset;
int LecteurCarte_lcrt_wait_univVars_Flag_speciaux_sauve;
int LecteurCarte_lcrt_wait_univVars_objet_inventaire;
int LecteurCarte_lcrt_wait_TXT_Sally_gao;
int LecteurCarte_lcrt_wait_GST_Board_panel_gao_i_commerce_tune;

//LecteurCarte_lcrt_init
int LecteurCarte_lcrt_init_LecteurCarte_lcrt_wait;

int LecteurCarte_lcrt_wait()
{
    double v0; // st7
    int v1; // eax
    int v2; // eax
    tScriptStruct_LecteurCarte* v4; // [esp+0h] [ebp-F4h]
    tGameObjectMain* v5; // [esp+Ch] [ebp-E8h]
    tGameObjectMain* v6; // [esp+20h] [ebp-D4h]
    tGameObjectMain* v7; // [esp+2Ch] [ebp-C8h]
    tMessage v8; // [esp+34h] [ebp-C0h] BYREF
    float v9; // [esp+74h] [ebp-80h]
    int v11; // [esp+80h] [ebp-74h]
    tMessage Src; // [esp+98h] [ebp-5Ch] BYREF
    int v13; // [esp+CCh] [ebp-28h]
    tScriptStruct_LecteurCarte* v14; // [esp+D0h] [ebp-24h]
    void* v15; // [esp+E0h] [ebp-14h]
    BOOL v17; // [esp+E8h] [ebp-Ch]
    BOOL v18; // [esp+ECh] [ebp-8h]
    int v19; // [esp+F0h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_LecteurCarte*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v14 = v4;
    v17 = 0;
    v18 = 0;
    v19 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        LecteurCarte_lcrt_wait__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        LecteurCarte_lcrt_wait_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &LecteurCarte_lcrt_wait_univVars_Perso_ID_equipement,
            (void*)LecteurCarte_lcrt_wait_univVars,
            40);
        AI2C_ResolveBuffer(
            &LecteurCarte_lcrt_wait__PJ_Sally_gao_current_pm_mode,
            (void*)LecteurCarte_lcrt_wait__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(
            &LecteurCarte_lcrt_wait__PJ_Sally_gao_current_pm_priority,
            (void*)LecteurCarte_lcrt_wait__PJ_Sally_gao,
            576);
        LecteurCarte_lcrt_wait_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &LecteurCarte_lcrt_wait_GST_Board_panel_gao_i_use_prio,
            (void*)LecteurCarte_lcrt_wait_GST_Board_panel_gao,
            496);
        AI2C_ResolveBuffer(
            &LecteurCarte_lcrt_wait_GST_Board_panel_gao_o_use_object,
            (void*)LecteurCarte_lcrt_wait_GST_Board_panel_gao,
            488);
        AI2C_ResolveBuffer(
            &LecteurCarte_lcrt_wait_GST_Board_panel_gao_i_use_action,
            (void*)LecteurCarte_lcrt_wait_GST_Board_panel_gao,
            500);
        AI2C_ResolveBuffer(
            &LecteurCarte_lcrt_wait_GST_Board_panel_gao_f_use_offset,
            (void*)LecteurCarte_lcrt_wait_GST_Board_panel_gao,
            504);
        AI2C_ResolveBuffer(
            &LecteurCarte_lcrt_wait_univVars_Flag_speciaux_sauve,
            (void*)LecteurCarte_lcrt_wait_univVars,
            4488);
        AI2C_ResolveBuffer(&LecteurCarte_lcrt_wait_univVars_objet_inventaire, (void*)LecteurCarte_lcrt_wait_univVars, 1480);
        LecteurCarte_lcrt_wait_TXT_Sally_gao = AI_ul_RealResolveOneRefObject(0x7200064Eu);
        AI2C_ResolveBuffer(
            &LecteurCarte_lcrt_wait_GST_Board_panel_gao_i_commerce_tune,
            (void*)LecteurCarte_lcrt_wait_GST_Board_panel_gao,
            2212);
    }
    v7 = (tGameObjectMain*)LecteurCarte_lcrt_wait__PJ_Sally_gao;
    if (LecteurCarte_lcrt_wait__PJ_Sally_gao)
    {
        if ((unsigned int)LecteurCarte_lcrt_wait__PJ_Sally_gao <= 2)
            v7 = AI_MainActor[LecteurCarte_lcrt_wait__PJ_Sally_gao - 1];
        v0 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v7);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
    }
    v9 = (float)v0;
    if (v9 < 1.0
        && *(uint32_t*)AI_EvalVarArray2_C(1, (char*)LecteurCarte_lcrt_wait_univVars_Perso_ID_equipement, 0, 8) == v14->objet_lecteur)
    {
        v13 = (unsigned __int16)*(uint32_t*)(LecteurCarte_lcrt_wait__PJ_Sally_gao + 4);
        if ((v13 & 1) != 0)
            v17 = !*(uint32_t*)LecteurCarte_lcrt_wait__PJ_Sally_gao_current_pm_mode
            || *(int*)LecteurCarte_lcrt_wait__PJ_Sally_gao_current_pm_priority < 20;
        else
            v17 = 0;
        if (v17 && *(int*)LecteurCarte_lcrt_wait_GST_Board_panel_gao_i_use_prio < 5)
        {
            *(uint32_t*)LecteurCarte_lcrt_wait_GST_Board_panel_gao_o_use_object = (uint32_t)AI_gpst_CurrentGameObject;
            *(uint32_t*)LecteurCarte_lcrt_wait_GST_Board_panel_gao_i_use_prio = 5;
            *(uint32_t*)LecteurCarte_lcrt_wait_GST_Board_panel_gao_i_use_action = 4;
            *(uint32_t*)LecteurCarte_lcrt_wait_GST_Board_panel_gao_f_use_offset = 0;
        }
    }
    v18 = AI_EvalFunc_ARRBitTest_C((int*)(LecteurCarte_lcrt_wait_univVars_Flag_speciaux_sauve + 4), 2349);
    if (v18)
    {
        v6 = (tGameObjectMain*)LecteurCarte_lcrt_wait__PJ_Sally_gao;
        if (LecteurCarte_lcrt_wait__PJ_Sally_gao)
        {
            if ((unsigned int)LecteurCarte_lcrt_wait__PJ_Sally_gao <= 2)
                v6 = AI_MainActor[LecteurCarte_lcrt_wait__PJ_Sally_gao - 1];
            v1 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, v6);
        }
        else
        {
            v1 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, 0);
        }
        if (v1)
        {
            AI_EvalFunc_ARRBitClear_C((int*)(LecteurCarte_lcrt_wait_univVars_Flag_speciaux_sauve + 4), 2349);
            if (*(uint32_t*)AI_EvalVarArray2_C(1, (char*)LecteurCarte_lcrt_wait_univVars_objet_inventaire, 34, 8))
            {
                AI_EvalFunc_ARRBitClear_C((int*)(LecteurCarte_lcrt_wait_univVars_Flag_speciaux_sauve + 4), 2349);
                Src.msgID = 9;
                Src.f2 = 5.0;
                AI_EvalFunc_MSGSend_C(AI_gpst_CurrentGameObject, &Src);
            }
        }
    }
    if (AI_EvalFunc_MSGGetCount_C(AI_gpst_CurrentGameObject))
    {
        v15 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&Src, v15, sizeof(Src));
        if (Src.msgID == 9 && (*(uint32_t *)&Src.f2) == 0x40A00000)
        {
            Src.msgID = 64;
            Src.Int1 = 34;
            Src.gao = AI_gpst_CurrentGameObject;
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)LecteurCarte_lcrt_wait__PJ_Sally_gao, &Src);
        }
        else if (Src.msgID == 37 && (Src.Int1 & 7) != 0)
        {
            if (v14->SF_Mode_Retrait_Pognes > 0)
                AI_EvalFunc_ARRBitSet_C(
                    (int*)(LecteurCarte_lcrt_wait_univVars_Flag_speciaux_sauve + 4),
                    v14->SF_Mode_Retrait_Pognes);
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x100, 0);
            v14->i_afficher_tune = 1;
            v8.msgID = 30;
            v8.Int1 = 29;
            v8.Int2 = 0;
            v8.f2 = 2.0;
            v8.v1.x = 0.0;
            v8.v1.y = 0.0;
            v8.v1.z = 0.0;
            v8.v2.x = 0.0;
            v8.v2.y = 0.0;
            v8.v2.z = 0.0;
            v8.gao = 0;
            v8.gao2 = 0;
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)LecteurCarte_lcrt_wait_TXT_Sally_gao, &v8);
            v19 = AI_EvalFunc_SNDRequest_C((tGameObjectMain*)LecteurCarte_lcrt_wait_GST_Board_panel_gao, 7, 0x18000080);
            AI_EvalFunc_SNDUltraPlaySound_C(AI_gpst_CurrentGameObject, v19);
            v19 = AI_EvalFunc_SNDRequest_C((tGameObjectMain*)LecteurCarte_lcrt_wait_GST_Board_panel_gao, 8, 0x18000080);
            AI_EvalFunc_SNDUltraPlaySound_C(AI_gpst_CurrentGameObject, v19);
            v19 = AI_EvalFunc_SNDRequest_C((tGameObjectMain*)LecteurCarte_lcrt_wait_GST_Board_panel_gao, 9, 0x18000080);
            AI_EvalFunc_SNDUltraPlaySound_C(AI_gpst_CurrentGameObject, v19);
        }
    }
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(256))
    {
        v5 = (tGameObjectMain*)LecteurCarte_lcrt_wait__PJ_Sally_gao;
        if (LecteurCarte_lcrt_wait__PJ_Sally_gao)
        {
            if ((unsigned int)LecteurCarte_lcrt_wait__PJ_Sally_gao <= 2)
                v5 = AI_MainActor[LecteurCarte_lcrt_wait__PJ_Sally_gao - 1];
            v2 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, v5);
        }
        else
        {
            v2 = AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, 0);
        }
        v11 = v2;
        if (!v2)
        {
            if (v14->SF_Mode_Retrait_Pognes > 0)
                AI_EvalFunc_ARRBitClear_C(
                    (int*)(LecteurCarte_lcrt_wait_univVars_Flag_speciaux_sauve + 4),
                    v14->SF_Mode_Retrait_Pognes);
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
            v14->i_afficher_tune = 0;
        }
    }
    if (v14->i_afficher_tune)
        *(uint32_t*)LecteurCarte_lcrt_wait_GST_Board_panel_gao_i_commerce_tune = -(v14->des_i_vendeur_ID + 1);
    return 1000;
}

int LecteurCarte_lcrt_init()
{
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        LecteurCarte_lcrt_init_LecteurCarte_lcrt_wait = (int)AI_ul_RealResolveOneRefFunction(0x9E003B31);
    }
    AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x8000, 0);
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)LecteurCarte_lcrt_init_LecteurCarte_lcrt_wait);
    return 1000;
}