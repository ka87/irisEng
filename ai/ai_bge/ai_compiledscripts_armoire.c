#include "ai_compiledscripts_inc.h"

//armoire_arm_init
int armoire_arm_init_univVars;
int armoire_arm_init_univVars_Flag_speciaux_sauve;
int fct_armoire_arm_init_armoire_arm_go;

//armoire_arm_go
int armoire_arm_go_univVars;
int armoire_arm_go_univVars_Flag_speciaux_sauve;

int armoire_arm_init()
{
    int result; // eax
    tScriptStruct_Armoire* v1; // [esp+4h] [ebp-58h]
    tGameObjectMain* v2; // [esp+Ch] [ebp-50h]
    tGameObjectMain* v3; // [esp+10h] [ebp-4Ch]
    tGameObjectMain* v4; // [esp+14h] [ebp-48h]
    tGameObjectMain* v5; // [esp+1Ch] [ebp-40h]
    tGameObjectMain* v6; // [esp+20h] [ebp-3Ch]
    tGameObjectMain* v7; // [esp+24h] [ebp-38h]
    tGameObjectMain* v8; // [esp+2Ch] [ebp-30h]
    tGameObjectMain* v9; // [esp+30h] [ebp-2Ch]
    tGameObjectMain* v10; // [esp+34h] [ebp-28h]
    tGameObjectMain* v11; // [esp+3Ch] [ebp-20h]
    tGameObjectMain* v12; // [esp+40h] [ebp-1Ch]
    tGameObjectMain* v13; // [esp+44h] [ebp-18h]
    int v14; // [esp+50h] [ebp-Ch]
    BOOL v15; // [esp+58h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Armoire*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        armoire_arm_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&armoire_arm_init_univVars_Flag_speciaux_sauve, (void*)armoire_arm_init_univVars, 4488);
        fct_armoire_arm_init_armoire_arm_go = (int)AI_ul_RealResolveOneRefFunction(0x7100BD49u);
    }
    if (*AI_gppst_CurrentJumpNode)
    {
        *AI_gppst_CurrentJumpNode = 0;
        TIME_Wait_C(0.15000001f);
        result = 0;
    }
    else
    {
        if (v1->switch_SF_reste_ouverte <= 0)
            v15 = 0;
        else
            v15 = AI_EvalFunc_ARRBitTest_C(
                (int*)(armoire_arm_init_univVars_Flag_speciaux_sauve + 4),
                v1->switch_SF_reste_ouverte);
        if (v1->switch_etat_init || v15)
        {
            v13 = (tGameObjectMain*)v1->o_activer_si_ouvert;
            if (v13)
            {
                if ((unsigned int)v13 <= 2)
                    v13 = AI_MainActor[(uint32_t)v13 - 1];
                v12 = v13;
            }
            else
            {
                v12 = 0;
            }
            if (v12)
            {
                v11 = (tGameObjectMain*)v1->o_activer_si_ouvert;
                if (v11)
                {
                    if ((unsigned int)v11 <= 2)
                        v11 = AI_MainActor[(uint32_t)v11 - 1];
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(v11, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
                }
            }
            AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, 0);
            AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, 1);
            AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, 2);
            AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, 3);
            AI_EvalFunc_TRASetEnd_C(AI_gpst_CurrentGameObject, 0);
            AI_EvalFunc_TRASetEnd_C(AI_gpst_CurrentGameObject, 1);
            AI_EvalFunc_TRASetEnd_C(AI_gpst_CurrentGameObject, 2);
            AI_EvalFunc_TRASetEnd_C(AI_gpst_CurrentGameObject, 3);
            v1->switch_etat_init = 1;
        }
        else
        {
            AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, 4);
            AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, 5);
            AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, 6);
            AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, 7);
            AI_EvalFunc_TRASetEnd_C(AI_gpst_CurrentGameObject, 4);
            AI_EvalFunc_TRASetEnd_C(AI_gpst_CurrentGameObject, 5);
            AI_EvalFunc_TRASetEnd_C(AI_gpst_CurrentGameObject, 6);
            AI_EvalFunc_TRASetEnd_C(AI_gpst_CurrentGameObject, 7);
            v1->switch_etat_init = 0;
            v10 = (tGameObjectMain*)v1->a_activer_a_louverture1;
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
            if (v9)
            {
                v8 = (tGameObjectMain*)v1->a_activer_a_louverture1;
                if (v8)
                {
                    if ((unsigned int)v8 <= 2)
                        v8 = AI_MainActor[(uint32_t)v8 - 1];
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(v8, 1);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
                }
            }
            v7 = (tGameObjectMain*)v1->a_activer_a_louverture2;
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
            if (v6)
            {
                v5 = (tGameObjectMain*)v1->a_activer_a_louverture2;
                if (v5)
                {
                    if ((unsigned int)v5 <= 2)
                        v5 = AI_MainActor[(uint32_t)v5 - 1];
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(v5, 1);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
                }
            }
            v4 = (tGameObjectMain*)v1->a_activer_a_louverture3;
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
                v2 = (tGameObjectMain*)v1->a_activer_a_louverture3;
                if (v2)
                {
                    if ((unsigned int)v2 <= 2)
                        v2 = AI_MainActor[(uint32_t)v2 - 1];
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(v2, 1);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
                }
            }
        }
        AI_EvalFunc_OBJ_ExtraFlagsSet_C(AI_gpst_CurrentGameObject, 8, 0);
        AI_EvalFunc_OBJ_AILodSet_C(AI_gpst_CurrentGameObject, 400.0);
        v14 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_armoire_arm_init_armoire_arm_go);
        if (v14 == 1000)
            result = 1000;
        else
            result = v14;
    }
    return result;
}

int armoire_arm_go()
{
    BOOL v0; // eax
    tScriptStruct_Armoire* v2; // [esp+4h] [ebp-A4h]
    tGameObjectMain* v3; // [esp+10h] [ebp-98h]
    tGameObjectMain* v4; // [esp+14h] [ebp-94h]
    tGameObjectMain* v5; // [esp+18h] [ebp-90h]
    tGameObjectMain* v6; // [esp+20h] [ebp-88h]
    tGameObjectMain* v7; // [esp+24h] [ebp-84h]
    tGameObjectMain* v8; // [esp+28h] [ebp-80h]
    tGameObjectMain* v9; // [esp+30h] [ebp-78h]
    tGameObjectMain* v10; // [esp+34h] [ebp-74h]
    tGameObjectMain* v11; // [esp+38h] [ebp-70h]
    tGameObjectMain* v12; // [esp+40h] [ebp-68h]
    tGameObjectMain* v13; // [esp+44h] [ebp-64h]
    tGameObjectMain* v14; // [esp+48h] [ebp-60h]
    tGameObjectMain* v15; // [esp+50h] [ebp-58h]
    tGameObjectMain* v16; // [esp+54h] [ebp-54h]
    tGameObjectMain* v17; // [esp+58h] [ebp-50h]
    tGameObjectMain* v18; // [esp+60h] [ebp-48h]
    tGameObjectMain* v19; // [esp+64h] [ebp-44h]
    tGameObjectMain* v20; // [esp+68h] [ebp-40h]
    tGameObjectMain* v21; // [esp+70h] [ebp-38h]
    tGameObjectMain* v22; // [esp+74h] [ebp-34h]
    tGameObjectMain* v23; // [esp+78h] [ebp-30h]
    tGameObjectMain* v24; // [esp+84h] [ebp-24h]
    tGameObjectMain* v25; // [esp+88h] [ebp-20h]
    tGameObjectMain* v26; // [esp+8Ch] [ebp-1Ch]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_Armoire*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        armoire_arm_go_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&armoire_arm_go_univVars_Flag_speciaux_sauve, (void*)armoire_arm_go_univVars, 4488);
    }
    if (*AI_gppst_CurrentJumpNode != (tAINode*)8)
    {
        if (*AI_gppst_CurrentJumpNode != (tAINode*)13)
        {
            v26 = (tGameObjectMain*)v2->switch_condition_ouverture;
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
            if (!v25)
                return 1000;
            v24 = (tGameObjectMain*)v2->switch_condition_ouverture;
            if (v24)
            {
                if ((unsigned int)v24 <= 2)
                    v24 = AI_MainActor[(uint32_t)v24 - 1];
                v0 = AI_EvalFunc_OBJ_CapaTest_C(v24, 256);
            }
            else
            {
                v0 = AI_EvalFunc_OBJ_CapaTest_C(0, 256);
            }
            if (!v0)
                return 1000;
            if (!v2->switch_etat_init)
            {
                AI_EvalFunc_OBJ_CapaSet_C_CURRENT(256, 0);
                if (v2->switch_SF_reste_ouverte > 0)
                    AI_EvalFunc_ARRBitSet_C((int*)(armoire_arm_go_univVars_Flag_speciaux_sauve + 4), v2->switch_SF_reste_ouverte);
                v23 = (tGameObjectMain*)v2->a_activer_a_louverture1;
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
                    v21 = (tGameObjectMain*)v2->a_activer_a_louverture1;
                    if (v21)
                    {
                        if ((unsigned int)v21 <= 2)
                            v21 = AI_MainActor[(uint32_t)v21 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v21, 0);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
                    }
                }
                v20 = (tGameObjectMain*)v2->a_activer_a_louverture2;
                if (v20)
                {
                    if ((unsigned int)v20 <= 2)
                        v20 = AI_MainActor[(uint32_t)v20 - 1];
                    v19 = v20;
                }
                else
                {
                    v19 = 0;
                }
                if (v19)
                {
                    v18 = (tGameObjectMain*)v2->a_activer_a_louverture2;
                    if (v18)
                    {
                        if ((unsigned int)v18 <= 2)
                            v18 = AI_MainActor[(uint32_t)v18 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v18, 0);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
                    }
                }
                v17 = (tGameObjectMain*)v2->a_activer_a_louverture3;
                if (v17)
                {
                    if ((unsigned int)v17 <= 2)
                        v17 = AI_MainActor[(uint32_t)v17 - 1];
                    v16 = v17;
                }
                else
                {
                    v16 = 0;
                }
                if (v16)
                {
                    v15 = (tGameObjectMain*)v2->a_activer_a_louverture3;
                    if (v15)
                    {
                        if ((unsigned int)v15 <= 2)
                            v15 = AI_MainActor[(uint32_t)v15 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v15, 0);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
                    }
                }
                v14 = (tGameObjectMain*)v2->o_activer_si_ouvert;
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
                if (v13)
                {
                    v12 = (tGameObjectMain*)v2->o_activer_si_ouvert;
                    if (v12)
                    {
                        if ((unsigned int)v12 <= 2)
                            v12 = AI_MainActor[(uint32_t)v12 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v12, 0);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
                    }
                }
                AI_EvalFunc_SNDRequestPlay_C_CURRENT(0, -1.0);
                AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, 0);
                AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, 1);
                AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, 2);
                AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, 3);
                goto LABEL_71;
            }
            v11 = (tGameObjectMain*)v2->a_activer_a_louverture1;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v11 = AI_MainActor[(uint32_t)v11 - 1];
                v10 = v11;
            }
            else
            {
                v10 = 0;
            }
            if (v10)
            {
                v9 = (tGameObjectMain*)v2->a_activer_a_louverture1;
                if (v9)
                {
                    if ((unsigned int)v9 <= 2)
                        v9 = AI_MainActor[(uint32_t)v9 - 1];
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(v9, 1);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
                }
            }
            v8 = (tGameObjectMain*)v2->a_activer_a_louverture2;
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
            if (v7)
            {
                v6 = (tGameObjectMain*)v2->a_activer_a_louverture2;
                if (v6)
                {
                    if ((unsigned int)v6 <= 2)
                        v6 = AI_MainActor[(uint32_t)v6 - 1];
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(v6, 1);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
                }
            }
            v5 = (tGameObjectMain*)v2->a_activer_a_louverture3;
            if (v5)
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
                v3 = (tGameObjectMain*)v2->a_activer_a_louverture3;
                if (v3)
                {
                    if ((unsigned int)v3 <= 2)
                        v3 = AI_MainActor[(uint32_t)v3 - 1];
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(v3, 1);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
                }
            }
            AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
            AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, 4);
            AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, 5);
            AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, 6);
            AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, 7);
        }
        if (!AI_EvalFunc_TRAIsStopAll_C(AI_gpst_CurrentGameObject))
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)13;
            return 0;
        }
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
        if (v2->switch_SF_reste_ouverte > 0)
            AI_EvalFunc_ARRBitClear_C((int*)(armoire_arm_go_univVars_Flag_speciaux_sauve + 4), v2->switch_SF_reste_ouverte);
        goto LABEL_111;
    }
LABEL_71:
    if (!AI_EvalFunc_TRAIsStopAll_C(AI_gpst_CurrentGameObject))
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)8;
        return 0;
    }
LABEL_111:
    v2->switch_etat_init = 1 - v2->switch_etat_init;
    return 1000;
}