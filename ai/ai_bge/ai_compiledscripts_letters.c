#include "ai_compiledscripts_inc.h"

//Letters_init
int Letters_init_INV_manager_gao;
int Letters_init_univVars;
int Letters_init_univVars_ai_codes;
int fct_Letters_init_Letters_WaitRestart;

//Letters_WaitRestart
int Letters_WaitRestart_Letters_init;

int Letters_init()
{
    int result; // eax
    char* v1; // eax
    float v2; // [esp+0h] [ebp-50h]
    tScriptStruct_Letters* v3; // [esp+4h] [ebp-4Ch]
    tGameObjectMain* v4; // [esp+Ch] [ebp-44h]
    tGameObjectMain* a1; // [esp+10h] [ebp-40h]
    tGameObjectMain* v6; // [esp+14h] [ebp-3Ch]
    tTextGet v7; // [esp+1Ch] [ebp-34h] BYREF
    tTextGet a5; // [esp+28h] [ebp-28h] BYREF
    tGameObjectMain* v10; // [esp+30h] [ebp-20h]
    int a3; // [esp+34h] [ebp-1Ch]
    int v12; // [esp+38h] [ebp-18h]
    tTextGet v13; // [esp+3Ch] [ebp-14h] BYREF
    tTextGet Src; // [esp+44h] [ebp-Ch] BYREF
    tScriptStruct_Letters* v15; // [esp+4Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_Letters*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v15 = v3;
    a3 = 0;
    v12 = 0;

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Letters_init_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        Letters_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&Letters_init_univVars_ai_codes, (void*)Letters_init_univVars, 6788);
        fct_Letters_init_Letters_WaitRestart = (int)AI_ul_RealResolveOneRefFunction(0x49018B57u);
    }
    v15->des_i_Restart = 0;
    if (AI_EvalFunc_OBJ_TextRequest_C(0))
    {
        if (v15->des_i_Mode)
        {
            if (v15->des_i_Mode == 1)
            {
                STR_SetString(
                    (tGameObjectMain*)Letters_init_INV_manager_gao,
                    22,
                    (char*)AI_gpst_CurrentFunction->localStringConstsData);
                for (a3 = 0; ; ++a3)
                {
                    v12 = *(uint32_t*)AI_EvalVarArray2_C(1, (char*)Letters_init_univVars_ai_codes, a3, v15->des_i_IndexOrCode);
                    if (!v12)
                        break;
                    if (v12 >= 27)
                    {
                        if (v12 >= 37)
                            AI_EvalFunc_OBJ_TextCharSet_C((tGameObjectMain*)Letters_init_INV_manager_gao, 22, a3, 32);
                        else
                            AI_EvalFunc_OBJ_TextCharSet_C((tGameObjectMain*)Letters_init_INV_manager_gao, 22, a3, v12 + 21);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_TextCharSet_C((tGameObjectMain*)Letters_init_INV_manager_gao, 22, a3, v12 + 64);
                    }
                }
                AI_EvalFunc_TEXTGet_C(0x4900FDC5, 132, &v13);
                memcpy(&a5, &v13, sizeof(a5));
                AI_EvalFunc_OBJ_Text2Text_C((tGameObjectMain*)Letters_init_INV_manager_gao, 22, 0, -1, &a5);
                STR_SetString(
                    (tGameObjectMain*)Letters_init_INV_manager_gao,
                    22,
                    (char*)AI_gpst_CurrentFunction->localStringConstsData + 11);
            }
            else
            {
                v2 = (float)v15->des_i_IndexOrCode;
                STR_SetFloat((tGameObjectMain*)Letters_init_INV_manager_gao, 22, v2, 0);
                STR_InsertString(
                    (tGameObjectMain*)Letters_init_INV_manager_gao,
                    22,
                    -1,
                    (char*)AI_gpst_CurrentFunction->localStringConstsData + 12);
                AI_EvalFunc_TEXTGet_C(0x4900FDC5, 132, &v7);
                memcpy(&a5, &v7, sizeof(a5));
                AI_EvalFunc_OBJ_Text2Text_C((tGameObjectMain*)Letters_init_INV_manager_gao, 22, 0, -1, &a5);
                STR_SetString(
                    (tGameObjectMain*)Letters_init_INV_manager_gao,
                    22,
                    (char*)AI_gpst_CurrentFunction->localStringConstsData + 15);
            }
        }
        else
        {
            AI_EvalFunc_TEXTGet_C(v15->des_txt_File, v15->des_i_IndexOrCode, &Src);
            memcpy(&a5, &Src, sizeof(a5));
        }
        v6 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v6 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            a1 = v6;
        }
        else
        {
            a1 = 0;
        }
        v1 = AI_EvalFunc_TEXTToString_C(&a5);
        STR_Init3DString(a1, v1);
        v10 = AI_gpst_CurrentGameObject;
        v4 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v4 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            STR_3DStringList_Add(v4);
        }
        else
        {
            STR_3DStringList_Add(0);
        }
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Letters_init_Letters_WaitRestart);
        result = 1000;
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int Letters_WaitRestart()
{
    tScriptStruct_Letters* v1; // [esp+0h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Letters*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Letters_WaitRestart_Letters_init = AI_ul_RealResolveOneRefFunction(0x4901880Du);
    }
    if (v1->des_i_Restart)
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)Letters_WaitRestart_Letters_init);
    return 1000;
}