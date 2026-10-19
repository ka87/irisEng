#include "ai_compiledscripts_inc.h"

//hyllis_gp_manager_hgm_init
int hyllis_gp_manager_hgm_init_univVars;
int hyllis_gp_manager_hgm_init_univVars_Flag_speciaux_sauve;
int hyllis_gp_manager_hgm_init_GST_Global_gao;
int hyllis_gp_manager_hgm_init_GST_Global_gao_o_joy_acteur;
int hyllis_gp_manager_hgm_init_univVars_i_Hyllis_Attaques;
int fct_hyllis_gp_manager_hgm_init_hyllis_gp_manager_hgm_loop;

//hyllis_gp_manager_hgm_loop
int hyllis_gp_manager_hgm_loop_univVars;
int hyllis_gp_manager_hgm_loop_univVars_Flag_speciaux_sauve;
int hyllis_gp_manager_hgm_loop_univVars_i_Hyllis_Attaques;
int hyllis_gp_manager_hgm_loop_univVars_Perso_ID_actor;
int fct_hyllis_gp_manager_hgm_loop_univVars_hyllis_gp_manager_hgm_attend;

//hyllis_gp_manager_hgm_attend
int hyllis_gp_manager_hgm_attend_univVars;
int hyllis_gp_manager_hgm_attend_univVars_Flag_speciaux_sauve;
int fct_hyllis_gp_manager_hgm_attend_hyllis_gp_manager_hgm_init;

int hyllis_gp_manager_hgm_init()
{
    int result; // eax
    tScriptStruct_GSTHyllisManager* v1; // [esp+4h] [ebp-38h]
    tGameObjectMain* v2; // [esp+Ch] [ebp-30h]
    tGameObjectMain* v3; // [esp+10h] [ebp-2Ch]
    tGameObjectMain* v4; // [esp+14h] [ebp-28h]
    tGameObjectMain* v5; // [esp+18h] [ebp-24h]
    int a2; // [esp+28h] [ebp-14h]
    int v7; // [esp+2Ch] [ebp-10h]
    int v8; // [esp+30h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GSTHyllisManager*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        hyllis_gp_manager_hgm_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &hyllis_gp_manager_hgm_init_univVars_Flag_speciaux_sauve,
            (void*)hyllis_gp_manager_hgm_init_univVars,
            4488);
        hyllis_gp_manager_hgm_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &hyllis_gp_manager_hgm_init_GST_Global_gao_o_joy_acteur,
            (void*)hyllis_gp_manager_hgm_init_GST_Global_gao,
            136);
        AI2C_ResolveBuffer(
            &hyllis_gp_manager_hgm_init_univVars_i_Hyllis_Attaques,
            (void*)hyllis_gp_manager_hgm_init_univVars,
            20244);
        fct_hyllis_gp_manager_hgm_init_hyllis_gp_manager_hgm_loop = (int)AI_ul_RealResolveOneRefFunction(0x3D001AE7u);
    }
    if (*AI_gppst_CurrentJumpNode)
    {
        *AI_gppst_CurrentJumpNode = 0;
        TIME_Wait_C(0.1f);
        return 0;
    }
    v1->i_with_time = 0;
    AI_EvalFunc_ARRBitClear_C((int*)(hyllis_gp_manager_hgm_init_univVars_Flag_speciaux_sauve + 4), 2718);
    AI_EvalFunc_ARRBitClear_C((int*)(hyllis_gp_manager_hgm_init_univVars_Flag_speciaux_sauve + 4), 2789);
    v5 = *(tGameObjectMain**)hyllis_gp_manager_hgm_init_GST_Global_gao_o_joy_acteur;
    if (*(uint32_t*)hyllis_gp_manager_hgm_init_GST_Global_gao_o_joy_acteur)
    {
        if ((unsigned int)v5 <= 2)
            v5 = AI_MainActor[(uint32_t)v5 - 1];
        v4 = v5;
    }
    else
    {
        v4 = 0;
    }
    if (v4)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    v7 = *(uint32_t*)hyllis_gp_manager_hgm_init_univVars_i_Hyllis_Attaques;
    if (*(uint32_t*)hyllis_gp_manager_hgm_init_univVars_i_Hyllis_Attaques)
    {
        switch (v7)
        {
        case 1:
            a2 = 126;
            break;
        case 2:
            a2 = 57;
            break;
        case 3:
            a2 = 2302;
            break;
        default:
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
    }
    else
    {
        a2 = 2721;
    }
    v3 = (tGameObjectMain*)v1->des_ao_allgp[v7];
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        v2 = v3;
    }
    else
    {
        v2 = 0;
    }
    if (v2)
    {
        if (!AI_EvalFunc_ARRBitTest_C((int*)(hyllis_gp_manager_hgm_init_univVars_Flag_speciaux_sauve + 4), a2)
            || (v8 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_hyllis_gp_manager_hgm_init_hyllis_gp_manager_hgm_loop),
                v8 == 1000))
        {
            //DBG_OutputDebugString("hyllis_gp_manager_hgm_init: SF %d, gp: %d\n", a2, v7);
            result = 1000;
        }
        else
        {
            result = v8;
        }
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int hyllis_gp_manager_hgm_loop()
{
    BOOL v0; // eax
    int result; // eax
    tScriptStruct_GSTHyllisManager* v2; // [esp+4h] [ebp-54h]
    tGameObjectMain* v3; // [esp+Ch] [ebp-4Ch]
    tGameObjectMain* v4; // [esp+14h] [ebp-44h]
    tGameObjectMain* a2; // [esp+18h] [ebp-40h]
    tGameObjectMain* v6; // [esp+1Ch] [ebp-3Ch]
    tGameObjectMain* v7; // [esp+20h] [ebp-38h]
    tGameObjectMain* v8; // [esp+24h] [ebp-34h]
    tGameObjectMain* v9; // [esp+3Ch] [ebp-1Ch]
    int v10; // [esp+40h] [ebp-18h]
    int v11; // [esp+48h] [ebp-10h]
    int i; // [esp+54h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_GSTHyllisManager*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        hyllis_gp_manager_hgm_loop_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &hyllis_gp_manager_hgm_loop_univVars_Flag_speciaux_sauve,
            (void*)hyllis_gp_manager_hgm_loop_univVars,
            4488);
        AI2C_ResolveBuffer(
            &hyllis_gp_manager_hgm_loop_univVars_i_Hyllis_Attaques,
            (void*)hyllis_gp_manager_hgm_loop_univVars,
            20244);
        AI2C_ResolveBuffer(
            &hyllis_gp_manager_hgm_loop_univVars_Perso_ID_actor,
            (void*)hyllis_gp_manager_hgm_loop_univVars,
            988);
        fct_hyllis_gp_manager_hgm_loop_univVars_hyllis_gp_manager_hgm_attend = (int)AI_ul_RealResolveOneRefFunction(0x3D002E66u);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)10)
    {
        result = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_hyllis_gp_manager_hgm_loop_univVars_hyllis_gp_manager_hgm_attend);
        if (result == 1000)
            result = 1000;
    }
    else if (AI_EvalFunc_ARRBitTest_C((int*)(hyllis_gp_manager_hgm_loop_univVars_Flag_speciaux_sauve + 4), 2718))
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        if (*(uint32_t*)hyllis_gp_manager_hgm_loop_univVars_i_Hyllis_Attaques != 3)
            goto LABEL_18;
        v8 = *AI_MainActor;
        if (*AI_MainActor)
        {
            if ((unsigned int)v8 <= 2)
                v8 = AI_MainActor[(uint32_t)v8 - 1];
            v7 = v8;
        }
        else
        {
            v7 = 0;
        }
        if (*(tGameObjectMain**)(hyllis_gp_manager_hgm_loop_univVars_Perso_ID_actor + 40) == v7)
        {
        LABEL_18:
            v11 = -1;
            v9 = *AI_MainActor;
            for (i = 0; i < v2->des_i_numzones; ++i)
            {
                v6 = (tGameObjectMain*)v2->des_ao_allzones[i];
                if (v6)
                {
                    if ((unsigned int)v6 <= 2)
                        v6 = AI_MainActor[(uint32_t)v6 - 1];
                    a2 = v6;
                }
                else
                {
                    a2 = 0;
                }
                v4 = v9;
                if (v9)
                {
                    if ((unsigned int)v9 <= 2)
                        v4 = AI_MainActor[(uint32_t)v9 - 1];
                    v0 = AI_EvalFunc_COLBVObjectObject_C(v4, a2);
                }
                else
                {
                    v0 = AI_EvalFunc_COLBVObjectObject_C(0, a2);
                }
                if (v0)
                {
                    v11 = i;
                    break;
                }
            }
            if (v11 == -1)
            {
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 1000;
            }
            else
            {
                v10 = *(uint32_t*)hyllis_gp_manager_hgm_loop_univVars_i_Hyllis_Attaques;
                v2->i_last_gp = *(uint32_t*)hyllis_gp_manager_hgm_loop_univVars_i_Hyllis_Attaques;
                v3 = (tGameObjectMain*)v2->des_ao_allgp[v10];
                if (v3)
                {
                    if ((unsigned int)v3 <= 2)
                        v3 = AI_MainActor[(uint32_t)v3 - 1];
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(v3, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
                }
                if (!v2->i_with_time)
                    ++* (uint32_t*)hyllis_gp_manager_hgm_loop_univVars_i_Hyllis_Attaques;
                *AI_gppst_CurrentJumpNode = (tAINode*)10;
                TIME_Wait_C(1.0);
                result = 0;
            }
        }
        else
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
    }
    return result;
}

int hyllis_gp_manager_hgm_attend()
{
    int result; // eax
    tScriptStruct_GSTHyllisManager* v1; // [esp+0h] [ebp-18h]
    int v2; // [esp+Ch] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GSTHyllisManager*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        hyllis_gp_manager_hgm_attend_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &hyllis_gp_manager_hgm_attend_univVars_Flag_speciaux_sauve,
            (void*)hyllis_gp_manager_hgm_attend_univVars,
            4488);
        fct_hyllis_gp_manager_hgm_attend_hyllis_gp_manager_hgm_init = (int)AI_ul_RealResolveOneRefFunction(0x3D001AE5u);
    }
    if (AI_EvalFunc_ARRBitTest_C((int*)(hyllis_gp_manager_hgm_attend_univVars_Flag_speciaux_sauve + 4), 2718))
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        v1->f_time = 0.0;
        v2 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_hyllis_gp_manager_hgm_attend_hyllis_gp_manager_hgm_init);
        if (v2 == 1000)
            result = 1000;
        else
            result = v2;
    }
    return result;
}