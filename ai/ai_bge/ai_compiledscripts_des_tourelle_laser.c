#include "ai_compiledscripts_inc.h"

//Des_Tourelle_laser_Des_TL_mode_explosion
int Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao;
int Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_o_targets;
int Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_i_targets_nb;
int Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_v_targets_offset;
int Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_o_selected_target;
int Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_i_selected_target_nb;
int Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_v_selected_target_offset;
int Des_Tourelle_laser_Des_TL_mode_explosion_univVars;
int Des_Tourelle_laser_Des_TL_mode_explosion_SFX_explosion_gao;
int Des_Tourelle_laser_Des_TL_mode_explosion_univVars_Flag_speciaux_sauve;

//Des_Tourelle_laser_Des_TL_init
int Des_Tourelle_laser_Des_TL_init_univVars;
int Des_Tourelle_laser_Des_TL_init_univVars_Flag_speciaux_sauve;
int Des_Tourelle_laser_Des_TL_init_GST_Global_gao;
int Des_Tourelle_laser_Des_TL_init_GST_Global_gao_o_targets;
int Des_Tourelle_laser_Des_TL_init_GST_Global_gao_i_targets_nb;
int Des_Tourelle_laser_Des_TL_init_GST_Global_gao_v_targets_offset;
int fct_Des_Tourelle_laser_Des_TL_init_Des_Tourelle_laser_Des_TL_mode_attaque;
int fct_Des_Tourelle_laser_Des_TL_init_Des_Tourelle_laser_Des_TL_exec_afterrec;
int fct_Des_Tourelle_laser_Des_TL_init_Des_Tourelle_laser_Des_TL_reflex;

//Des_Tourelle_laser_Des_TL_reflex
int Des_Tourelle_laser_Des_TL_reflex__PJ_Aircraft_gao;
int Des_Tourelle_laser_Des_TL_reflex__PJ_Enterprise_gao;
int fct_Des_Tourelle_laser_Des_TL_reflex_Des_Tourelle_laser_Des_TL_mode_explosion;

int Des_Tourelle_laser_Des_TL_mode_explosion()
{
    tScriptStruct_DesTourelleLaser* v1; // [esp+4h] [ebp-E8h]
    tGameObjectMain* v2; // [esp+10h] [ebp-DCh]
    tGameObjectMain* v3; // [esp+14h] [ebp-D8h]
    tGameObjectMain* v4; // [esp+18h] [ebp-D4h]
    tGameObjectMain* v5; // [esp+20h] [ebp-CCh]
    Vec3* v6; // [esp+28h] [ebp-C4h]
    tGameObjectMain* v7; // [esp+38h] [ebp-B4h]
    tGameObjectMain* v8; // [esp+3Ch] [ebp-B0h]
    Vec3* v9; // [esp+44h] [ebp-A8h]
    tGameObjectMain* a3; // [esp+54h] [ebp-98h]
    tGameObjectMain* v11; // [esp+58h] [ebp-94h]
    tMessage Src; // [esp+74h] [ebp-78h] BYREF
    unsigned int v13; // [esp+B4h] [ebp-38h]
    int v14; // [esp+D0h] [ebp-1Ch]
    tScriptStruct_DesTourelleLaser* v15; // [esp+D4h] [ebp-18h]
    int a2; // [esp+D8h] [ebp-14h]
    Vec3 pos; // [esp+DCh] [ebp-10h] BYREF

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_DesTourelleLaser*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v15 = v1;
    a2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_o_targets,
            (void*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao,
            964);
        AI2C_ResolveBuffer(
            &Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_i_targets_nb,
            (void*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao,
            2572);
        AI2C_ResolveBuffer(
            &Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_v_targets_offset,
            (void*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao,
            1368);
        AI2C_ResolveBuffer(
            &Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_o_selected_target,
            (void*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao,
            2580);
        AI2C_ResolveBuffer(
            &Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_i_selected_target_nb,
            (void*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao,
            2748);
        AI2C_ResolveBuffer(
            &Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_v_selected_target_offset,
            (void*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao,
            2624);
        Des_Tourelle_laser_Des_TL_mode_explosion_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
        Des_Tourelle_laser_Des_TL_mode_explosion_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Des_Tourelle_laser_Des_TL_mode_explosion_univVars_Flag_speciaux_sauve,
            (void*)Des_Tourelle_laser_Des_TL_mode_explosion_univVars,
            4488);
    }
    if (v15->i_current_mode == 1)
        return 1000;
    v15->i_current_mode = 1;
    v11 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v11 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        a3 = v11;
    }
    else
    {
        a3 = 0;
    }
    v14 = AI_EvalFunc_ARRObjSearch_C(
        (tGameObjectMain**)(Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_o_targets + 4),
        100,
        a3);
    a2 = v14;
    if (v14 != -1)
    {
        *(uint32_t*)(Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_o_targets + 4 * a2 + 4) = *(uint32_t*)(Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_o_targets + 4 * *(uint32_t*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_i_targets_nb);
        v9 = (Vec3*)AI_EvalVarArray_C3(
            (char*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_v_targets_offset,
            *(uint32_t*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_i_targets_nb - 1);
        *(Vec3*)AI_EvalVarArray_C3((char*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_v_targets_offset, a2) = *v9;
        *(uint32_t*)(Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_o_targets
            + 4 * *(uint32_t*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_i_targets_nb) = 0;
        *(uint32_t*)AI_EvalVarArray_C3(
            (char*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_v_targets_offset,
            *(uint32_t*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_i_targets_nb - 1) = 0;
        *(uint32_t*)(AI_EvalVarArray_C3(
            (char*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_v_targets_offset,
            *(uint32_t*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_i_targets_nb - 1)
            + 4) = 0;
        *(uint32_t*)(AI_EvalVarArray_C3(
            (char*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_v_targets_offset,
            *(uint32_t*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_i_targets_nb - 1)
            + 8) = 0;
        --* (uint32_t*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_i_targets_nb;
    }
    v8 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v8 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        v7 = v8;
    }
    else
    {
        v7 = 0;
    }
    a2 = AI_EvalFunc_ARRObjSearch_C(
        (tGameObjectMain**)(Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_o_selected_target + 4),
        10,
        v7);
    if (a2 != -1)
    {
        *(uint32_t*)(Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_o_selected_target + 4 * a2 + 4) = *(uint32_t*)(Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_o_selected_target + 4 * *(uint32_t*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_i_selected_target_nb);
        v6 = (Vec3*)AI_EvalVarArray_C3(
            (char*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_v_selected_target_offset,
            *(uint32_t*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_i_selected_target_nb - 1);
        *(Vec3*)AI_EvalVarArray_C3(
            (char*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_v_selected_target_offset,
            a2) = *v6;
        *(uint32_t*)(Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_o_selected_target
            + 4 * *(uint32_t*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_i_selected_target_nb) = 0;
        *(uint32_t*)AI_EvalVarArray_C3(
            (char*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_v_selected_target_offset,
            *(uint32_t*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_i_selected_target_nb - 1) = 0;
        *(uint32_t*)(AI_EvalVarArray_C3(
            (char*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_v_selected_target_offset,
            *(uint32_t*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_i_selected_target_nb - 1)
            + 4) = 0;
        *(uint32_t*)(AI_EvalVarArray_C3(
            (char*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_v_selected_target_offset,
            *(uint32_t*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_i_selected_target_nb - 1)
            + 8) = 0;
        --* (uint32_t*)Des_Tourelle_laser_Des_TL_mode_explosion_GST_Global_gao_i_selected_target_nb;
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
    Src.msgID = 1;
    Src.Int1 = 20;
    Src.f2 = 20.0;
    Src.f1 = 1.0;
    Src.v1.x = 1.0;
    Src.v1.y = 0.0;
    Src.v1.z = 1.0;
    v5 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)Des_Tourelle_laser_Des_TL_mode_explosion_SFX_explosion_gao, &pos);
    if (v5)
    {
        if ((unsigned int)v5 <= 2)
            v5 = AI_MainActor[(uint32_t)v5 - 1];
        AI_EvalFunc_MSGSend_C(v5, &Src);
    }
    else
    {
        AI_EvalFunc_MSGSend_C(0, &Src);
    }
    AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
    AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
    v15->f_time_change_mode = TIM_gf_MainClock - TIM_gf_LockTime;
    v4 = (tGameObjectMain*)v15->des_o_actor_transfert;
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
        v15->m_msg.msgID = 43;
        v15->m_msg.gao = AI_gpst_CurrentGameObject;
        v2 = (tGameObjectMain*)v15->des_o_actor_transfert;
        if (v2)
        {
            if ((unsigned int)v2 <= 2)
                v2 = AI_MainActor[(uint32_t)v2 - 1];
            AI_EvalFunc_MSGSend_C(v2, &v15->m_msg);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &v15->m_msg);
        }
    }
    if (v15->des_SF_mort > 0)
        AI_EvalFunc_ARRBitSet_C(
            (int*)(Des_Tourelle_laser_Des_TL_mode_explosion_univVars_Flag_speciaux_sauve + 4),
            v15->des_SF_mort);
    v13 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
    return 1;
}

int Des_Tourelle_laser_Des_TL_init()
{
    int result; // eax
    tScriptStruct_DesTourelleLaser* v1; // [esp+4h] [ebp-80h]
    tGameObjectMain* v2; // [esp+Ch] [ebp-78h]
    tGameObjectMain* v3; // [esp+20h] [ebp-64h]
    tGameObjectMain* v4; // [esp+24h] [ebp-60h]
    int v5; // [esp+34h] [ebp-50h]
    Vec3 v6; // [esp+44h] [ebp-40h] BYREF
    tGameObjectMain* v7; // [esp+58h] [ebp-2Ch]
    int v8; // [esp+60h] [ebp-24h]
    int v9; // [esp+64h] [ebp-20h]
    unsigned int v10; // [esp+68h] [ebp-1Ch]
    tScriptStruct_DesTourelleLaser* v11; // [esp+6Ch] [ebp-18h]
    BOOL v12; // [esp+70h] [ebp-14h]
    Vec3 a3; // [esp+74h] [ebp-10h] BYREF

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_DesTourelleLaser*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v11 = v1;
    v9 = 0;
    v12 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Des_Tourelle_laser_Des_TL_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Des_Tourelle_laser_Des_TL_init_univVars_Flag_speciaux_sauve,
            (void*)Des_Tourelle_laser_Des_TL_init_univVars,
            4488);
        Des_Tourelle_laser_Des_TL_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Des_Tourelle_laser_Des_TL_init_GST_Global_gao_o_targets,
            (void*)Des_Tourelle_laser_Des_TL_init_GST_Global_gao,
            964);
        AI2C_ResolveBuffer(
            &Des_Tourelle_laser_Des_TL_init_GST_Global_gao_i_targets_nb,
            (void*)Des_Tourelle_laser_Des_TL_init_GST_Global_gao,
            2572);
        AI2C_ResolveBuffer(
            &Des_Tourelle_laser_Des_TL_init_GST_Global_gao_v_targets_offset,
            (void*)Des_Tourelle_laser_Des_TL_init_GST_Global_gao,
            1368);
        fct_Des_Tourelle_laser_Des_TL_init_Des_Tourelle_laser_Des_TL_mode_attaque = (int)AI_ul_RealResolveOneRefFunction(0x72001DDBu);
        fct_Des_Tourelle_laser_Des_TL_init_Des_Tourelle_laser_Des_TL_exec_afterrec = (int)AI_ul_RealResolveOneRefFunction(0x72001DDDu);
        fct_Des_Tourelle_laser_Des_TL_init_Des_Tourelle_laser_Des_TL_reflex = (int)AI_ul_RealResolveOneRefFunction(0x72001DDCu);
    }
    if (v11->des_SF_mort <= 0)
        v12 = 0;
    else
        v12 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Des_Tourelle_laser_Des_TL_init_univVars_Flag_speciaux_sauve + 4),
            v11->des_SF_mort);
    if (v12)
    {
        v10 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1;
    }
    else
    {
        AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x8000, 0);
        v8 = AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 8, 0);
        v4 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v4 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            v3 = v4;
        }
        else
        {
            v3 = 0;
        }
        v9 = AI_EvalFunc_ARRObjSearch_C(
            (tGameObjectMain**)(Des_Tourelle_laser_Des_TL_init_GST_Global_gao_o_targets + 4),
            *(uint32_t*)Des_Tourelle_laser_Des_TL_init_GST_Global_gao_i_targets_nb,
            v3);
        if (v9 == -1)
        {
            v9 = AI_EvalFunc_ARRObjSearch_C(
                (tGameObjectMain**)(Des_Tourelle_laser_Des_TL_init_GST_Global_gao_o_targets + 4),
                100,
                0);
            if (v9 != -1)
            {
                ++* (uint32_t*)Des_Tourelle_laser_Des_TL_init_GST_Global_gao_i_targets_nb;
                *(uint32_t*)(Des_Tourelle_laser_Des_TL_init_GST_Global_gao_o_targets + 4 * v9 + 4) = (uint32_t)AI_gpst_CurrentGameObject;
                *(uint32_t*)AI_EvalVarArray_C3((char*)Des_Tourelle_laser_Des_TL_init_GST_Global_gao_v_targets_offset, v9) = 0;
                *(uint32_t*)(AI_EvalVarArray_C3((char*)Des_Tourelle_laser_Des_TL_init_GST_Global_gao_v_targets_offset, v9) + 4) = 0;
                *(uint32_t*)(AI_EvalVarArray_C3((char*)Des_Tourelle_laser_Des_TL_init_GST_Global_gao_v_targets_offset, v9) + 8) = 0;
            }
        }
        v11->des_f_angle_vision = v11->des_f_angle_vision * 0.0087265f;
        v11->des_f_angle_vision = cosf(v11->des_f_angle_vision);
        a3.x = 0.0;
        a3.y = 0.0;
        a3.z = 10.0;
        AI_EvalFunc_COLSpecificPosSet_C(AI_gpst_CurrentGameObject, 1, &a3);
        v6.x = 20.0;
        v6.y = 20.0;
        v6.z = 20.0;
        AI_EvalFunc_COLSpecificSizeSet_C(AI_gpst_CurrentGameObject, 1u, &v6);
        AI_EvalFunc_AITrackChange_C(
            1,
            (tAIFunction*)fct_Des_Tourelle_laser_Des_TL_init_Des_Tourelle_laser_Des_TL_mode_attaque);
        v7 = AI_gpst_CurrentGameObject;
        v2 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v2 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v2,
                2,
                (tAIFunction*)fct_Des_Tourelle_laser_Des_TL_init_Des_Tourelle_laser_Des_TL_exec_afterrec);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_Des_Tourelle_laser_Des_TL_init_Des_Tourelle_laser_Des_TL_exec_afterrec);
        }
        v5 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Des_Tourelle_laser_Des_TL_init_Des_Tourelle_laser_Des_TL_reflex);
        if (v5 == 1000)
            result = 1000;
        else
            result = v5;
    }
    return result;
}

int Des_Tourelle_laser_Des_TL_reflex()
{
    tScriptStruct_DesTourelleLaser* v1; // [esp+0h] [ebp-6Ch]
    double v2; // [esp+4h] [ebp-68h]
    tGameObjectMain* v3; // [esp+24h] [ebp-48h]
    tGameObjectMain* v4; // [esp+28h] [ebp-44h]
    tGameObjectMain* v5; // [esp+2Ch] [ebp-40h]
    tGameObjectMain* v6; // [esp+30h] [ebp-3Ch]
    tGameObjectMain* v7; // [esp+34h] [ebp-38h]
    void* Src; // [esp+38h] [ebp-34h]
    Vec3 dest; // [esp+3Ch] [ebp-30h] BYREF
    void* v10; // [esp+4Ch] [ebp-20h]
    Vec3 a1; // [esp+50h] [ebp-1Ch] BYREF
    Vec3 a3; // [esp+5Ch] [ebp-10h] BYREF
    tScriptStruct_DesTourelleLaser* v13; // [esp+68h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_DesTourelleLaser*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v13 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Des_Tourelle_laser_Des_TL_reflex__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        Des_Tourelle_laser_Des_TL_reflex__PJ_Enterprise_gao = AI_ul_RealResolveOneRefObject(0x71003F3Eu);
        fct_Des_Tourelle_laser_Des_TL_reflex_Des_Tourelle_laser_Des_TL_mode_explosion = (int)AI_ul_RealResolveOneRefFunction(0x72001E0Au);
    }
    v13->o_target = (int)*AI_MainActor;
    v7 = (tGameObjectMain*)v13->o_target;
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
    if (v6 == (tGameObjectMain*)Des_Tourelle_laser_Des_TL_reflex__PJ_Aircraft_gao)
    {
        v13->i_target_type = 0;
    }
    else
    {
        v5 = (tGameObjectMain*)v13->o_target;
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
        if (v4 == (tGameObjectMain*)Des_Tourelle_laser_Des_TL_reflex__PJ_Enterprise_gao)
            v13->i_target_type = 1;
    }
    v3 = (tGameObjectMain*)v13->o_target;
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        AI_EvalFunc_OBJGetPos_C(v3, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
    MATH_SubstractVec(&dest, &a3, &a1);
    v13->v_me_to_target = dest;
    MATH_NormalizeEqualVector(&v13->v_me_to_target);
    Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v13->m_msg, Src, sizeof(v13->m_msg));
    while (v13->m_msg.msgID)
    {
        if (v13->m_msg.msgID == 37)
        {
            v2 = v13->m_msg.f2 + MATH_gd_magicDoubleForRound;
            v13->des_i_resistance -= (*(uint32_t*)&v2);
            if (v13->des_i_resistance <= 0)
                AI_EvalFunc_AITrackChange_C(
                    1,
                    (tAIFunction*)fct_Des_Tourelle_laser_Des_TL_reflex_Des_Tourelle_laser_Des_TL_mode_explosion);
        }
        v10 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v13->m_msg, v10, sizeof(v13->m_msg));
    }
    return 1000;
}

int Des_Tourelle_laser_Des_TL_exec_afterrec()
{
    tGameObjectMain* v0; // eax
    double v1; // st7
    float v3; // [esp+0h] [ebp-1A8h]
    float v4; // [esp+0h] [ebp-1A8h]
    float v5; // [esp+0h] [ebp-1A8h]
    tScriptStruct_DesTourelleLaser* v6; // [esp+8h] [ebp-1A0h]
    tGameObjectMain* v7; // [esp+10h] [ebp-198h]
    tGameObjectMain* v8; // [esp+18h] [ebp-190h]
    tGameObjectMain* v9; // [esp+28h] [ebp-180h]
    tGameObjectMain* v10; // [esp+30h] [ebp-178h]
    tGameObjectMain* v11; // [esp+38h] [ebp-170h]
    tGameObjectMain* v12; // [esp+40h] [ebp-168h]
    tGameObjectMain* v13; // [esp+48h] [ebp-160h]
    float v14; // [esp+4Ch] [ebp-15Ch]
    tGameObjectMain* v15; // [esp+60h] [ebp-148h]
    tGameObjectMain* v16; // [esp+84h] [ebp-124h]
    float v17; // [esp+94h] [ebp-114h]
    float v18; // [esp+98h] [ebp-110h]
    float v19; // [esp+98h] [ebp-110h]
    Vec3 v20; // [esp+9Ch] [ebp-10Ch] BYREF
    float v21; // [esp+ACh] [ebp-FCh]
    Vec3 v22; // [esp+B4h] [ebp-F4h] BYREF
    Vec3 dest; // [esp+C0h] [ebp-E8h] BYREF
    Vec3 v24; // [esp+D8h] [ebp-D0h]
    float v25; // [esp+E4h] [ebp-C4h]
    Vec3 v26; // [esp+E8h] [ebp-C0h] BYREF
    float v27; // [esp+F4h] [ebp-B4h]
    float v28; // [esp+F8h] [ebp-B0h]
    float v29; // [esp+FCh] [ebp-ACh]
    Vec3 v30; // [esp+100h] [ebp-A8h] BYREF
    Vec3 a3; // [esp+10Ch] [ebp-9Ch] BYREF
    Vec3 pos; // [esp+118h] [ebp-90h] BYREF
    Vec3 v33; // [esp+124h] [ebp-84h] BYREF
    Vec3 v; // [esp+130h] [ebp-78h] BYREF
    float v35; // [esp+13Ch] [ebp-6Ch]
    float v36; // [esp+144h] [ebp-64h]
    float v37; // [esp+148h] [ebp-60h]
    float v38; // [esp+14Ch] [ebp-5Ch]
    Vec3 v39; // [esp+150h] [ebp-58h] BYREF
    float v40; // [esp+160h] [ebp-48h]
    Vec3 v41; // [esp+164h] [ebp-44h] BYREF
    float v42; // [esp+170h] [ebp-38h]
    tScriptStruct_DesTourelleLaser* v43; // [esp+180h] [ebp-28h]
    int v44; // [esp+184h] [ebp-24h]
    float v45; // [esp+188h] [ebp-20h]
    float v46; // [esp+18Ch] [ebp-1Ch]
    float v47; // [esp+190h] [ebp-18h]
    Vec3 v48; // [esp+194h] [ebp-14h] BYREF
    float v49; // [esp+1A0h] [ebp-8h]

    v41.x = 1.0;
    v41.y = 0.0;
    v41.z = 0.0;
    pos.x = 1.0;
    pos.y = 0.0;
    pos.z = 0.0;
    v.x = 1.0;
    v.y = 0.0;
    v.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_DesTourelleLaser*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v43 = v6;
    v21 = 0.0;
    v44 = 0;
    v40 = 0.0;
    if (v6->i_flag_shoot)
    {
        v40 = v43->i_target_type ? 400.0f : 800.0f;
        v43->m_msg.msgID = 1;
        v43->m_msg.gao2 = 0;
        v43->m_msg.f2 = v40;
        v43->m_msg.f1 = 50.0;
        v43->m_msg.Int1 = -1;
        v43->m_msg.Int2 = 0xFFFFFF;
        v43->m_msg.v1.x = 0.0;
        v43->m_msg.v1.y = 0.0;
        v43->m_msg.v1.z = 0.0;
        v43->m_msg.v2.x = v43->des_f_point_de_vie;
        v43->m_msg.v2.y = 0.0;
        v43->m_msg.v2.z = v43->des_f_laser_life_time;
        v44 = AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)v43->ao_laser, v43->des_i_laser_max_nb, 0);
        if (v44 != -1)
        {
            v43->i_flag_shoot = 0;
            v43->f_time_laser_shoot = TIM_gf_MainClock - TIM_gf_LockTime;
            v43->f_delay_laser_shoot = AI_EvalFunc_MATHRandFloat_C(v43->des_f_laser_delay_min, v43->des_f_laser_delay_max);
            v16 = (tGameObjectMain*)v43->o_target;
            if (v16)
            {
                if ((unsigned int)v16 <= 2)
                    v16 = AI_MainActor[(uint32_t)v16 - 1];
                AI_EvalFunc_OBJGetPos_C(v16, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v48);
            MATH_SubstractVec(&dest, &a3, &v48);
            v = dest;
            MATH_NormalizeEqualVector(&v);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v39);
            v45 = v.x * 10.0f;
            v46 = v.y * 10.0f;
            v47 = v.z * 10.0f;
            pos.x = v39.x + v45;
            pos.y = v39.y + v46;
            pos.z = v39.z + v47;
            v24.x = 0.0;
            v24.y = 0.0;
            v24.z = 10.0;
            pos.x = pos.x + 0.0f;
            pos.y = pos.y + 0.0f;
            pos.z = pos.z + 10.0f;
            v15 = (tGameObjectMain*)v43->o_target;
            if (v15)
            {
                if ((unsigned int)v15 <= 2)
                    v15 = AI_MainActor[(uint32_t)v15 - 1];
                AI_EvalFunc_OBJGetPos_C(v15, &v33);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v33);
            }
            MATH_SubstractVec(&v22, &v33, &pos);
            v = v22;
            v21 = MATH_VecLen(&v);
            v14 = 1.0f / v21;
            v.x = v.x * v14;
            v.y = v.y * v14;
            v.z = v.z * v14;
            v13 = (tGameObjectMain*)v43->des_o_laser_model;
            if (v13)
            {
                if ((unsigned int)v13 <= 2)
                    v13 = AI_MainActor[(uint32_t)v13 - 1];
                v0 = AI_EvalFunc_OBJDuplicate_C(v13, &pos);
            }
            else
            {
                v0 = AI_EvalFunc_OBJDuplicate_C(0, &pos);
            }
            v43->ao_laser[v44] = (int)v0;
            v12 = (tGameObjectMain*)v43->ao_laser[v44];
            if (v12)
            {
                if ((unsigned int)v12 <= 2)
                    v12 = AI_MainActor[(uint32_t)v12 - 1];
                AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, v12, &v43->ao_laser[v44]);
            }
            else
            {
                AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, 0, &v43->ao_laser[v44]);
            }
            v11 = (tGameObjectMain*)v43->ao_laser[v44];
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v11 = AI_MainActor[(uint32_t)v11 - 1];
                AI_EvalFunc_MSGSend_C(v11, &v43->m_msg);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &v43->m_msg);
            }
            v17 = v21 / v40;
            v10 = (tGameObjectMain*)v43->o_target;
            if (v10)
            {
                if ((unsigned int)v10 <= 2)
                    v10 = AI_MainActor[(uint32_t)v10 - 1];
                v1 = AI_EvalFunc_DYNSpeedGet_C(v10);
            }
            else
            {
                v1 = AI_EvalFunc_DYNSpeedGet_C(0);
            }
            v25 = (float)v1;
            v18 = v17 * v25;
            v19 = v18 / 2.0f;
            v9 = (tGameObjectMain*)v43->o_target;
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                AI_EvalFunc_OBJGetPos_C(v9, &v30);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v30);
            }
            v41 = v30;
            v8 = (tGameObjectMain*)v43->o_target;
            if (v8)
            {
                if ((unsigned int)v8 <= 2)
                    v8 = AI_MainActor[(uint32_t)v8 - 1];
                AI_EvalFunc_DYNSpeedGetVector_C(v8, &v20);
            }
            else
            {
                AI_EvalFunc_DYNSpeedGetVector_C(0, &v20);
            }
            v27 = v20.x * v17;
            v28 = v20.y * v17;
            v29 = v20.z * v17;
            v41.x = v41.x + v27;
            v41.y = v41.y + v28;
            v41.z = v41.z + v29;
            v3 = -v19;
            v42 = AI_EvalFunc_MATHRandFloat_C(v3, v19);
            v4 = -v19;
            v35 = AI_EvalFunc_MATHRandFloat_C(v4, v19);
            v5 = -v19;
            v49 = AI_EvalFunc_MATHRandFloat_C(v5, v19);
            v36 = v42;
            v37 = v35;
            v38 = v49;
            v41.x = v41.x + v42;
            v41.y = v41.y + v35;
            v41.z = v41.z + v49;
            v41.x = v41.x - pos.x;
            v41.y = v41.y - pos.y;
            v41.z = v41.z - pos.z;
            AI_EvalFunc_MATHVecNormalize_C(&v41, &v26);
            v7 = (tGameObjectMain*)v43->ao_laser[v44];
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                AI_EvalFunc_OBJ_SightSet_C(v7, &v26);
            }
            else
            {
                AI_EvalFunc_OBJ_SightSet_C(0, &v26);
            }
        }
    }
    return 1000;
}

int Des_Tourelle_laser_Des_TL_mode_attaque()
{
    tScriptStruct_DesTourelleLaser* v1; // [esp+Ch] [ebp-2Ch]
    tGameObjectMain* v2; // [esp+14h] [ebp-24h]
    float v3; // [esp+18h] [ebp-20h]
    Vec3 a3; // [esp+1Ch] [ebp-1Ch] BYREF
    tScriptStruct_DesTourelleLaser* v7; // [esp+34h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_DesTourelleLaser*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v7 = v1;
    if (v1->i_current_mode)
    {
        v7->i_current_mode = 0;
        v7->f_time_change_mode = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
    {
        if (AI_EvalFunc_TIMEElapsed_C(v7->f_time_laser_shoot, v7->f_delay_laser_shoot))
        {
            v2 = (tGameObjectMain*)v7->o_target;
            if (v2)
            {
                if ((unsigned int)v2 <= 2)
                    v2 = AI_MainActor[(uint32_t)v2 - 1];
                AI_EvalFunc_OBJGetPos_C(v2, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            v3 = AI_EvalFunc_OBJIsInCone_C(AI_gpst_CurrentGameObject, &a3, v7->des_f_angle_vision, v7->des_f_dist_vision, 1);
            if (v3 != 0.0)
                v7->i_flag_shoot = 1;
        }
    }
    return 1000;
}