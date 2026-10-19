#include "ai_compiledscripts_inc.h"

//SND_MergeKit_SND_Merge
int fct_SND_MergeKit_SND_Merge_SND_MergeKit_SND_MergeUnmerge;
int SND_MergeKit_SND_Merge_univVars;
int SND_MergeKit_SND_Merge_univVars_Flag_speciaux_sauve;

//SND_MergeKit_SND_MergeUnmerge
int SND_MergeKit_SND_MergeUnmerge_univVars;
int SND_MergeKit_SND_MergeUnmerge_univVars_Flag_speciaux_sauve;


int SND_MergeKit_SND_Merge()
{
    int result; // eax
    tScriptStruct_SNDMerge* v1; // [esp+0h] [ebp-2Ch]
    tGameObjectMain* v2; // [esp+8h] [ebp-24h]
    int v3; // [esp+14h] [ebp-18h]
    int v4; // [esp+18h] [ebp-14h]
    int v5; // [esp+28h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SNDMerge*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SND_MergeKit_SND_Merge_SND_MergeKit_SND_MergeUnmerge = (int)AI_ul_RealResolveOneRefFunction(0x26003173u);
        SND_MergeKit_SND_Merge_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &SND_MergeKit_SND_Merge_univVars_Flag_speciaux_sauve,
            (void*)SND_MergeKit_SND_Merge_univVars,
            4488);
    }
    if (v1->i_mode)
    {
        v4 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SND_MergeKit_SND_Merge_SND_MergeKit_SND_MergeUnmerge);
        if (v4 != 1000)
            return v4;
    }
    if (v1->SF_actif <= 0)
    {
        v5 = 0;
    }
    else if (v1->SF_actif < 10000)
    {
        v5 = AI_EvalFunc_ARRBitTest_C((int*)(SND_MergeKit_SND_Merge_univVars_Flag_speciaux_sauve + 4), v1->SF_actif);
    }
    else
    {
        v5 = 1
            - AI_EvalFunc_ARRBitTest_C(
                (int*)(SND_MergeKit_SND_Merge_univVars_Flag_speciaux_sauve + 4),
                v1->SF_actif - 10000);
    }
    if (v5)
        goto LABEL_22;
    v2 = (tGameObjectMain*)v1->o_dest_gao;
    if (v2)
    {
        if ((unsigned int)v2 <= 2)
            v2 = AI_MainActor[(uint32_t)v2 - 1];
        SND_MergeBankOver(AI_gpst_CurrentGameObject, v2);
    }
    else
    {
        SND_MergeBankOver(AI_gpst_CurrentGameObject, 0);
    }
    v3 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
    if (v3 == 1000)
        LABEL_22:
    result = 1000;
    else
        result = v3;
    return result;
}

int SND_MergeKit_SND_MergeUnmerge()
{
    tScriptStruct_SNDMerge* v1; // [esp+0h] [ebp-3Ch]
    tGameObjectMain* v2; // [esp+8h] [ebp-34h]
    tGameObjectMain* v3; // [esp+Ch] [ebp-30h]
    tGameObjectMain* v4; // [esp+10h] [ebp-2Ch]
    tGameObjectMain* v5; // [esp+18h] [ebp-24h]
    tGameObjectMain* v6; // [esp+1Ch] [ebp-20h]
    tGameObjectMain* v7; // [esp+20h] [ebp-1Ch]
    int v8; // [esp+38h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SNDMerge*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SND_MergeKit_SND_MergeUnmerge_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &SND_MergeKit_SND_MergeUnmerge_univVars_Flag_speciaux_sauve,
            (void*)SND_MergeKit_SND_MergeUnmerge_univVars,
            4488);
    }
    if (v1->SF_actif <= 0)
    {
        v8 = 0;
    }
    else if (v1->SF_actif < 10000)
    {
        v8 = AI_EvalFunc_ARRBitTest_C((int*)(SND_MergeKit_SND_MergeUnmerge_univVars_Flag_speciaux_sauve + 4), v1->SF_actif);
    }
    else
    {
        v8 = 1
            - AI_EvalFunc_ARRBitTest_C(
                (int*)(SND_MergeKit_SND_MergeUnmerge_univVars_Flag_speciaux_sauve + 4),
                v1->SF_actif - 10000);
    }
    if (v8)
    {
        if (!v1->i_flag_merged)
        {
            v7 = (tGameObjectMain*)v1->o_dest_gao;
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                v6 = v7;
            }
            else
            {
                v6 = 0;
            }
            v5 = (tGameObjectMain*)v1->o_gao_merge;
            if (v5)
            {
                if ((unsigned int)v5 <= 2)
                    v5 = AI_MainActor[(uint32_t)v5 - 1];
                SND_MergeBankOver(v5, v6);
            }
            else
            {
                SND_MergeBankOver(0, v6);
            }
            v1->i_flag_merged = 1;
        }
    }
    else if (v1->i_flag_merged)
    {
        v4 = (tGameObjectMain*)v1->o_dest_gao;
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
        v2 = (tGameObjectMain*)v1->o_gao_unmerge;
        if (v2)
        {
            if ((unsigned int)v2 <= 2)
                v2 = AI_MainActor[(uint32_t)v2 - 1];
            SND_MergeBankOver(v2, v3);
        }
        else
        {
            SND_MergeBankOver(0, v3);
        }
        v1->i_flag_merged = 0;
    }
    return 1000;
}