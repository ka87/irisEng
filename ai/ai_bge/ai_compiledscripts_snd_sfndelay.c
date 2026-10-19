#include "ai_compiledscripts_inc.h"

//SND_SFnDELAY_sfd_init
int SND_SFnDELAY_sfd_init_univVars;
int SND_SFnDELAY_sfd_init_univVars_Flag_speciaux_sauve;
int fct_SND_SFnDELAY_sfd_init_SND_SFnDELAY_sfd_go;

//SND_SFnDELAY_sfd_go
int SND_SFnDELAY_sfd_go_univVars;
int SND_SFnDELAY_sfd_go_univVars_Flag_speciaux_sauve;

int SND_SFnDELAY_sfd_init()
{
    BOOL v0; // eax
    int result; // eax
    tScriptStruct_SNDSfnDelay* v2; // [esp+4h] [ebp-4Ch]
    tGameObjectMain* v3; // [esp+14h] [ebp-3Ch]
    tGameObjectMain* v4; // [esp+18h] [ebp-38h]
    tGameObjectMain* v5; // [esp+1Ch] [ebp-34h]
    int v6; // [esp+30h] [ebp-20h]
    int v7; // [esp+48h] [ebp-8h]
    int v8; // [esp+48h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SNDSfnDelay*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SND_SFnDELAY_sfd_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &SND_SFnDELAY_sfd_init_univVars_Flag_speciaux_sauve,
            (void*)SND_SFnDELAY_sfd_init_univVars,
            4488);
        fct_SND_SFnDELAY_sfd_init_SND_SFnDELAY_sfd_go = (int)AI_ul_RealResolveOneRefFunction(0x320000BBu);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)5)
    {
        result = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (result != 1000)
            return result;
    }
    else
    {
        if (v2->i_SF_Stop <= 0)
        {
            v7 = 0;
        }
        else if (v2->i_SF_Stop < 10000)
        {
            v7 = AI_EvalFunc_ARRBitTest_C((int*)(SND_SFnDELAY_sfd_init_univVars_Flag_speciaux_sauve + 4), v2->i_SF_Stop);
        }
        else
        {
            v7 = 1
                - AI_EvalFunc_ARRBitTest_C(
                    (int*)(SND_SFnDELAY_sfd_init_univVars_Flag_speciaux_sauve + 4),
                    v2->i_SF_Stop - 10000);
        }
        if (v7)
            goto LABEL_26;
        v5 = (tGameObjectMain*)v2->o_capa;
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
            v3 = (tGameObjectMain*)v2->o_capa;
            if (v3)
            {
                if ((unsigned int)v3 <= 2)
                    v3 = AI_MainActor[(uint32_t)v3 - 1];
                v0 = AI_EvalFunc_OBJ_CapaTest_C(v3, 0x100);
            }
            else
            {
                v0 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x100);
            }
            if (v0)
            {
            LABEL_26:
                AI_EvalFunc_SNDStopModifier_C(AI_gpst_CurrentGameObject, 0);
                *AI_gppst_CurrentJumpNode = (tAINode*)5;
                TIME_Wait_C(15.0);
                return 0;
            }
        }
    }
    if (v2->i_SF <= 0)
    {
        v8 = 0;
    }
    else if (v2->i_SF < 10000)
    {
        v8 = AI_EvalFunc_ARRBitTest_C((int*)(SND_SFnDELAY_sfd_init_univVars_Flag_speciaux_sauve + 4), v2->i_SF);
    }
    else
    {
        v8 = 1 - AI_EvalFunc_ARRBitTest_C((int*)(SND_SFnDELAY_sfd_init_univVars_Flag_speciaux_sauve + 4), v2->i_SF - 10000);
    }
    if (!v8 && v2->i_SF
        || (v2->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime,
            v6 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SND_SFnDELAY_sfd_init_SND_SFnDELAY_sfd_go),
            v6 == 1000))
    {
        result = 1000;
    }
    else
    {
        result = v6;
    }
    return result;
}

int SND_SFnDELAY_sfd_go()
{
    int v0; // eax
    int v1; // eax
    BOOL v2; // eax
    int result; // eax
    tScriptStruct_SNDSfnDelay* v4; // [esp+Ch] [ebp-A4h]
    tGameObjectMain* v5; // [esp+14h] [ebp-9Ch]
    tGameObjectMain* v6; // [esp+18h] [ebp-98h]
    tGameObjectMain* v7; // [esp+1Ch] [ebp-94h]
    tGameObjectMain* v8; // [esp+2Ch] [ebp-84h]
    tGameObjectMain* v9; // [esp+30h] [ebp-80h]
    tGameObjectMain* v10; // [esp+34h] [ebp-7Ch]
    tGameObjectMain* v11; // [esp+3Ch] [ebp-74h]
    tGameObjectMain* a2; // [esp+40h] [ebp-70h]
    tGameObjectMain* v13; // [esp+44h] [ebp-6Ch]
    tGameObjectMain* v14; // [esp+48h] [ebp-68h]
    tGameObjectMain* v15; // [esp+4Ch] [ebp-64h]
    tGameObjectMain* v16; // [esp+50h] [ebp-60h]
    tGameObjectMain* v17; // [esp+54h] [ebp-5Ch]
    tGameObjectMain* v18; // [esp+88h] [ebp-28h]
    float v19; // [esp+8Ch] [ebp-24h]
    int v20; // [esp+94h] [ebp-1Ch]
    int v21; // [esp+A0h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_SNDSfnDelay*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SND_SFnDELAY_sfd_go_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&SND_SFnDELAY_sfd_go_univVars_Flag_speciaux_sauve, (void*)SND_SFnDELAY_sfd_go_univVars, 4488);
    }
    if (*AI_gppst_CurrentJumpNode != (tAINode*)12)
    {
        v18 = *AI_MainActor;
        v17 = (tGameObjectMain*)v4->o_BV;
        if (v4->o_BV)
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
            goto LABEL_76;
        v15 = (tGameObjectMain*)v4->o_BV;
        if (v4->o_BV)
        {
            if ((unsigned int)v15 <= 2)
                v15 = AI_MainActor[(uint32_t)v15 - 1];
            v14 = v15;
        }
        else
        {
            v14 = 0;
        }
        if (!v14)
            goto LABEL_52;
        if (v4->i_inout)
        {
            v13 = *AI_MainActor;
            if (v18)
            {
                if ((unsigned int)v18 <= 2)
                    v13 = AI_MainActor[(uint32_t)v18 - 1];
                a2 = v13;
            }
            else
            {
                a2 = 0;
            }
            v11 = (tGameObjectMain*)v4->o_BV;
            if (v4->o_BV)
            {
                if ((unsigned int)v11 <= 2)
                    v11 = AI_MainActor[(uint32_t)v11 - 1];
                v0 = AI_EvalFunc_COLBVPivotCollide_C(v11, a2);
            }
            else
            {
                v0 = AI_EvalFunc_COLBVPivotCollide_C(0, a2);
            }
            if (v0)
                goto LABEL_76;
        }
        if (v4->i_inout)
            goto LABEL_52;
        v10 = v18;
        if (v18)
        {
            if ((unsigned int)v18 <= 2)
                v10 = AI_MainActor[(uint32_t)v18 - 1];
            v9 = v10;
        }
        else
        {
            v9 = 0;
        }
        v8 = (tGameObjectMain*)v4->o_BV;
        if (v4->o_BV)
        {
            if ((unsigned int)v8 <= 2)
                v8 = AI_MainActor[(uint32_t)v8 - 1];
            v1 = AI_EvalFunc_COLBVPivotCollide_C(v8, v9);
        }
        else
        {
            v1 = AI_EvalFunc_COLBVPivotCollide_C(0, v9);
        }
        if (v1)
        {
        LABEL_52:
            AI_EvalFunc_SNDStopModifier_C(AI_gpst_CurrentGameObject, 0);
            v4->f_delay = AI_EvalFunc_MATHRandFloat_C(v4->f_delay_init_min, v4->f_delay_init_max);
            v4->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
        }
        else
        {
        LABEL_76:
            if (AI_EvalFunc_TIMEElapsed_C(v4->f_time_start, v4->f_delay))
            {
                if (SND_i_IsTrackPlaying(1020))
                {
                    v4->f_delay = AI_EvalFunc_MATHRandFloat_C(v4->f_delay_add_min, v4->f_delay_add_max) + v4->f_delay;
                }
                else
                {
                    v20 = AI_EvalFunc_SNDGetInstanceModifier_C(AI_gpst_CurrentGameObject, 0);
                    if (!v4->i_rand_play || v4->f_rand_pos == -1.0)
                    {
                        SND_i_InstanceSetf(v20, 4117, 0.0);
                        v19 = AI_EvalFunc_MATHRandFloat_C(0.0, 30.0);
                        v4->f_rand_pos = v19;
                    }
                    else
                    {
                        SND_i_InstanceSetf(v20, 4117, v4->f_rand_pos);
                    }
                    AI_EvalFunc_SNDPlayModifier_C(AI_gpst_CurrentGameObject, 0);
                    v4->f_delay = AI_EvalFunc_MATHRandFloat_C(v4->f_delay_min, v4->f_delay_max);
                    v4->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
                }
            }
        }
        if (v4->i_SF_Stop <= 0)
        {
            v21 = 0;
        }
        else if (v4->i_SF_Stop < 10000)
        {
            v21 = AI_EvalFunc_ARRBitTest_C((int*)(SND_SFnDELAY_sfd_go_univVars_Flag_speciaux_sauve + 4), v4->i_SF_Stop);
        }
        else
        {
            v21 = 1
                - AI_EvalFunc_ARRBitTest_C(
                    (int*)(SND_SFnDELAY_sfd_go_univVars_Flag_speciaux_sauve + 4),
                    v4->i_SF_Stop - 10000);
        }
        if (v21)
            goto LABEL_71;
        v7 = (tGameObjectMain*)v4->o_capa;
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
            v5 = (tGameObjectMain*)v4->o_capa;
            if (v5)
            {
                if ((unsigned int)v5 <= 2)
                    v5 = AI_MainActor[(uint32_t)v5 - 1];
                v2 = AI_EvalFunc_OBJ_CapaTest_C(v5, 0x100);
            }
            else
            {
                v2 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x100);
            }
            if (v2)
            {
            LABEL_71:
                AI_EvalFunc_SNDStopModifier_C(AI_gpst_CurrentGameObject, 0);
                *AI_gppst_CurrentJumpNode = (tAINode*)12;
                TIME_Wait_C(15.0);
                return 0;
            }
        }
        return 1000;
    }
    result = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
    if (result == 1000)
        return 1000;
    return result;
}