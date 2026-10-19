#include "ai_compiledscripts_inc.h"

//AR_tickpoil_art_marche
int fct_AR_tickpoil_art_marche_AR_tickpoil_art_att;

//AR_tickpoil_art_att
int fct_AR_tickpoil_art_att_AR_tickpoil_art_marche;
int fct_AR_tickpoil_art_att_AR_tickpoil_art_init;

//AR_tickpoil_art_init
int AR_tickpoil_art_init_INV_manager_gao;
int AR_tickpoil_art_init_INV_manager_gao_i_Animals_Nb;
int AR_tickpoil_art_init_INV_manager_gao_ao_Animals;
int AR_tickpoil_art_init_INV_manager_gao_ai_AnimalsData;
int fct_AR_tickpoil_art_init_AR_tickpoil_art_marche;

//AR_tickpoil_art_reflex
int AR_tickpoil_art_reflex_univVars;
int AR_tickpoil_art_reflex_univVars_objet_liste_gao;
int fct_AR_tickpoil_art_reflex_AR_tickpoil_art_meurt;

//AR_tickpoil_art_firstinit
int fct_AR_tickpoil_art_firstinit_AR_tickpoil_art_init;

//AR_tickpoil_art_meurt
int AR_tickpoil_art_meurt_SFX_explosion_gao;
int AR_tickpoil_art_meurt_INV_manager_gao;
int AR_tickpoil_art_meurt_INV_manager_gao_i_Animals_Nb;
int AR_tickpoil_art_meurt_INV_manager_gao_ao_Animals;
int AR_tickpoil_art_meurt_INV_manager_gao_ai_AnimalsData;

int AR_tickpoil_art_marche()
{
    float v1; // [esp+0h] [ebp-80h]
    tScriptStruct_PNJARTicpoil* v2; // [esp+4h] [ebp-7Ch]
    float v3; // [esp+8h] [ebp-78h]
    int v4; // [esp+1Ch] [ebp-64h]
    Vec3 a1; // [esp+20h] [ebp-60h] BYREF
    float v6; // [esp+2Ch] [ebp-54h]
    float v7; // [esp+30h] [ebp-50h]
    Vec3 a3; // [esp+34h] [ebp-4Ch] BYREF
    float v9; // [esp+40h] [ebp-40h]
    float v10; // [esp+44h] [ebp-3Ch]
    Vec3 v11; // [esp+48h] [ebp-38h] BYREF
    Vec3 a2; // [esp+54h] [ebp-2Ch] BYREF
    Vec3 dest; // [esp+60h] [ebp-20h] BYREF
    tScriptStruct_PNJARTicpoil* v14; // [esp+6Ch] [ebp-14h]
    Vec3 v15; // [esp+70h] [ebp-10h] BYREF
    float v16; // [esp+7Ch] [ebp-4h]

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJARTicpoil*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v14 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_AR_tickpoil_art_marche_AR_tickpoil_art_att = (int)AI_ul_RealResolveOneRefFunction(0x3D0030F4u);
    }
    if (v14->i_snd_marche != -1)
        AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v14->i_snd_marche);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a3);
    MATH_SubstractVec(&dest, &v14->v_dest_pos, &a3);
    a1 = dest;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v11);
    v6 = MATH_VecLen(&a1);
    if (v6 < 0.1)
    {
        v4 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_AR_tickpoil_art_marche_AR_tickpoil_art_att);
        if (v4 != 1000)
            return v4;
    }
    v10 = MATH_VecLen(&a1);
    v16 = TIM_gf_dt;
    v3 = v14->f_speed * TIM_gf_dt;
    if (v10 > (double)v3)
        v1 = v14->f_speed * TIM_gf_dt;
    else
        v1 = v10;
    v7 = v1;
    AI_EvalFunc_MATHVecSetNormVector_C(&a1, v1);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
    MATH_VecAddVec(&v15, &a2, &a1);
    AI_EvalFunc_OBJSetPos_C_CURRENT(&v15);
    v9 = TIM_gf_dt;
    v14->f_morph = 12.0f * TIM_gf_dt + v14->f_morph;
    if (v14->f_morph > 6.0)
        v14->f_morph = 1.0;
    AI_EvalFunc_OBJ_SetChannelProg_C(AI_gpst_CurrentGameObject, 0, v14->f_morph);
    return 1000;
}

int AR_tickpoil_art_att()
{
    int result; // eax
    int v1; // eax
    tScriptStruct_PNJARTicpoil* v2; // [esp+4h] [ebp-70h]
    tGameObjectMain* v3; // [esp+18h] [ebp-5Ch]
    float v4; // [esp+1Ch] [ebp-58h]
    Vec3 dest; // [esp+20h] [ebp-54h] BYREF
    tGameObjectMain* v6; // [esp+2Ch] [ebp-48h]
    Vec3 a1; // [esp+34h] [ebp-40h] BYREF
    Vec3 a3; // [esp+50h] [ebp-24h] BYREF
    int v9; // [esp+5Ch] [ebp-18h]
    Vec3 v10; // [esp+60h] [ebp-14h] BYREF
    tScriptStruct_PNJARTicpoil* v11; // [esp+6Ch] [ebp-8h]
    int v12; // [esp+70h] [ebp-4h]

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJARTicpoil*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v11 = v2;
    v6 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_AR_tickpoil_art_att_AR_tickpoil_art_marche = (int)AI_ul_RealResolveOneRefFunction(0x3D0030F2u);
        fct_AR_tickpoil_art_att_AR_tickpoil_art_init = (int)AI_ul_RealResolveOneRefFunction(0x3D0030EBu);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)3)
    {
        v11->i_fuite = 0;
        AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &v11->v_dest_pos);
        v1 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_AR_tickpoil_art_att_AR_tickpoil_art_marche);
        v12 = v1;
        if (v1 != 1000)
            return v12;
        goto LABEL_20;
    }
    if (v11->i_snd_marche != -1)
        SND_StopRq(v11->i_snd_marche);
    if (v11->i_fuite)
    {
        v6 = *AI_MainActor;
        AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &v10);
        v3 = v6;
        if (v6)
        {
            if ((unsigned int)v6 <= 2)
                v3 = AI_MainActor[(uint32_t)v6 - 1];
            AI_EvalFunc_OBJGetPos_C(v3, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        MATH_SubstractVec(&dest, &v10, &a3);
        a1 = dest;
        v4 = MATH_VecLen(&a1);
        if (v11->f_dist < (double)v4)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)3;
            TIME_Wait_C(2.0);
            return 0;
        }
    LABEL_20:
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    v11->f_arrivee = TIM_gf_MainClock - TIM_gf_LockTime;
    v9 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_AR_tickpoil_art_att_AR_tickpoil_art_init);
    if (v9 == 1000)
        result = 1000;
    else
        result = v9;
    return result;
}

int AR_tickpoil_art_init()
{
    int result; // eax
    float v1; // [esp+0h] [ebp-D8h]
    float v2; // [esp+0h] [ebp-D8h]
    tScriptStruct_PNJARTicpoil* v3; // [esp+8h] [ebp-D0h]
    tGameObjectMain* v4; // [esp+10h] [ebp-C8h]
    tGameObjectMain* v5; // [esp+24h] [ebp-B4h]
    tGameObjectMain* v6; // [esp+30h] [ebp-A8h]
    tGameObjectMain* v7; // [esp+34h] [ebp-A4h]
    tGameObjectMain* v8; // [esp+48h] [ebp-90h]
    Vec3 a1; // [esp+58h] [ebp-80h] BYREF
    Vec3 dest; // [esp+64h] [ebp-74h] BYREF
    Vec3 v11; // [esp+70h] [ebp-68h] BYREF
    tGameObjectMain* v12; // [esp+7Ch] [ebp-5Ch]
    int v13; // [esp+80h] [ebp-58h]
    float v14; // [esp+84h] [ebp-54h]
    Vec3 a3; // [esp+8Ch] [ebp-4Ch] BYREF
    int i; // [esp+98h] [ebp-40h]
    int v17; // [esp+9Ch] [ebp-3Ch]
    int v18; // [esp+A8h] [ebp-30h]
    tScriptStruct_PNJARTicpoil* v19; // [esp+B0h] [ebp-28h]
    Vec3 v20; // [esp+B4h] [ebp-24h] BYREF
    Vec3 v21; // [esp+CCh] [ebp-Ch] BYREF

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_PNJARTicpoil*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v19 = v3;
    v18 = 0;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        AR_tickpoil_art_init_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &AR_tickpoil_art_init_INV_manager_gao_i_Animals_Nb,
            (void*)AR_tickpoil_art_init_INV_manager_gao,
            23888);
        AI2C_ResolveBuffer(
            &AR_tickpoil_art_init_INV_manager_gao_ao_Animals,
            (void*)AR_tickpoil_art_init_INV_manager_gao,
            23368);
        AI2C_ResolveBuffer(
            &AR_tickpoil_art_init_INV_manager_gao_ai_AnimalsData,
            (void*)AR_tickpoil_art_init_INV_manager_gao,
            23628);
        fct_AR_tickpoil_art_init_AR_tickpoil_art_marche = (int)AI_ul_RealResolveOneRefFunction(0x3D0030F2u);
    }
    v18 = 0;
    if (*(int*)AR_tickpoil_art_init_INV_manager_gao_i_Animals_Nb < 64)
    {
        for (i = 0; i < *(int32_t*)AR_tickpoil_art_init_INV_manager_gao_i_Animals_Nb; ++i)
        {
            v12 = AI_gpst_CurrentGameObject;
            v7 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v7 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v6 = v7;
            }
            else
            {
                v6 = 0;
            }
            if (*(tGameObjectMain**)(AR_tickpoil_art_init_INV_manager_gao_ao_Animals + 4 * i + 4) == v6)
                break;
        }
        if (i == *(uint32_t*)AR_tickpoil_art_init_INV_manager_gao_i_Animals_Nb)
        {
            *(uint32_t*)(AR_tickpoil_art_init_INV_manager_gao_ao_Animals
                + 4 * *(uint32_t*)AR_tickpoil_art_init_INV_manager_gao_i_Animals_Nb
                + 4) = (uint32_t)AI_gpst_CurrentGameObject;
            *(uint32_t*)(AR_tickpoil_art_init_INV_manager_gao_ai_AnimalsData
                + 4 * (*(uint32_t*)AR_tickpoil_art_init_INV_manager_gao_i_Animals_Nb)++
                + 4) = (v19->i_MapEtage << 8) + 29;
        }
    }
    if (v19->i_snd_marche != -1)
        SND_Release(v19->i_snd_marche);
    v19->i_snd_marche = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000000);
    if (AI_EvalFunc_TIMEElapsed_C(v19->f_arrivee, v19->f_rand_bouge))
    {
        v19->f_rand_bouge = AI_EvalFunc_MATHRandFloat_C(1.0, 2.0);
        ++v19->i_cpt_bouge;
        v19->f_arrivee = TIM_gf_MainClock - TIM_gf_LockTime;
        if (v19->i_cpt_bouge == 3)
        {
            AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &v19->v_dest_pos);
            v19->i_cpt_bouge = 0;
        }
        else
        {
            v1 = -v19->f_max_debat;
            a1.x = AI_EvalFunc_MATHRandFloat_C(v1, v19->f_max_debat);
            v2 = -v19->f_max_debat;
            a1.y = AI_EvalFunc_MATHRandFloat_C(v2, v19->f_max_debat);
            a1.z = 0.0;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v20);
            MATH_VecAddVec(&v11, &v20, &a1);
            v19->v_dest_pos = v11;
        }
        v17 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_AR_tickpoil_art_init_AR_tickpoil_art_marche);
        if (v17 != 1000)
            return v17;
    }
    v8 = *AI_MainActor;
    AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &v21);
    v5 = v8;
    if (v8)
    {
        if ((unsigned int)v8 <= 2)
            v5 = AI_MainActor[(uint32_t)v8 - 1];
        AI_EvalFunc_OBJGetPos_C(v5, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    MATH_SubstractVec(&dest, &v21, &a3);
    a1 = dest;
    v14 = MATH_VecLen(&a1);
    if (v19->f_dist - 2.0 <= v14)
        goto LABEL_40;
    v19->i_fuite = 1;
    v4 = (tGameObjectMain*)v19->o_fuite;
    if (v4)
    {
        if ((unsigned int)v4 <= 2)
            v4 = AI_MainActor[(uint32_t)v4 - 1];
        AI_EvalFunc_OBJGetPos_C(v4, &v19->v_dest_pos);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v19->v_dest_pos);
    }
    v13 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_AR_tickpoil_art_init_AR_tickpoil_art_marche);
    if (v13 == 1000)
        LABEL_40:
    result = 1000;
    else
        result = v13;
    return result;
}

int AR_tickpoil_art_reflex()
{
    tGameObjectMain* v0; // eax
    tScriptStruct_PNJARTicpoil* v2; // [esp+0h] [ebp-E0h]
    tGameObjectMain* v3; // [esp+8h] [ebp-D8h]
    tGameObjectMain* v4; // [esp+10h] [ebp-D0h]
    tGameObjectMain* v5; // [esp+20h] [ebp-C0h]
    Vec3 pos; // [esp+30h] [ebp-B0h] BYREF
    float v7; // [esp+3Ch] [ebp-A4h]
    float v8; // [esp+40h] [ebp-A0h]
    int v9; // [esp+44h] [ebp-9Ch]
    tMessage v10; // [esp+48h] [ebp-98h] BYREF
    Vec3 v11; // [esp+7Ch] [ebp-64h]
    void* Src; // [esp+88h] [ebp-58h]
    unsigned int v13; // [esp+8Ch] [ebp-54h]
    float v14; // [esp+94h] [ebp-4Ch]
    tMessage v15; // [esp+98h] [ebp-48h] BYREF
    float v16; // [esp+CCh] [ebp-14h]
    tScriptStruct_PNJARTicpoil* v17; // [esp+D0h] [ebp-10h]
    unsigned int v18; // [esp+D4h] [ebp-Ch]
    int v19; // [esp+D8h] [ebp-8h]
    int i; // [esp+DCh] [ebp-4h]

    v11.x = 1.0;
    v11.y = 0.0;
    v11.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJARTicpoil*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v17 = v2;
    i = 0;
    v13 = 0;
    v18 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        AR_tickpoil_art_reflex_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&AR_tickpoil_art_reflex_univVars_objet_liste_gao, (void*)AR_tickpoil_art_reflex_univVars, 1276);
        fct_AR_tickpoil_art_reflex_AR_tickpoil_art_meurt = (int)AI_ul_RealResolveOneRefFunction(0x3D003D9Au);
    }
    Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v15, Src, sizeof(v15));
    if (v15.msgID != 37)
        return 1000;
    for (i = 0; i < 3; ++i)
    {
        v16 = AI_EvalFunc_MATHRandFloat_C(-2.0, 2.0);
        v14 = AI_EvalFunc_MATHRandFloat_C(-1.0, 1.0);
        v7 = v16;
        v8 = v14;
        v9 = 1077936128;
        v11.x = v16;
        v11.y = v14;
        v11.z = 3.0;
        v18 = *(uint32_t*)(AR_tickpoil_art_reflex_univVars_objet_liste_gao + 16);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
        v5 = (tGameObjectMain*)v18;
        if (v18)
        {
            if (v18 <= 2)
                v5 = AI_MainActor[v18 - 1];
            v0 = AI_EvalFunc_OBJDuplicate_C(v5, &pos);
        }
        else
        {
            v0 = AI_EvalFunc_OBJDuplicate_C(0, &pos);
        }
        v13 = (unsigned int)v0;
        v10.msgID = 1;
        v10.Int1 = 1;
        v10.Int2 = 1;
        v10.f2 = 5.0;
        v10.f1 = 1.5;
        v10.v1 = v11;
        v4 = v0;
        if (v0)
        {
            if ((unsigned int)v0 <= 2)
                v4 = AI_MainActor[(uint32_t)v0 - 1];
            AI_EvalFunc_MSGSend_C(v4, &v10);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &v10);
        }
        v3 = (tGameObjectMain*)v13;
        if (v13)
        {
            if (v13 <= 2)
                v3 = AI_MainActor[v13 - 1];
            AI_EvalFunc_COLSetActivationSet_C(v3, 0, 1);
        }
        else
        {
            AI_EvalFunc_COLSetActivationSet_C(0, 0, 1);
        }
    }
    AI_EvalFunc_AITrackStop_C(0);
    v19 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_AR_tickpoil_art_reflex_AR_tickpoil_art_meurt);
    if (v19 != 1000)
        return v19;
    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
    return 1000;
}

int AR_tickpoil_art_firstinit()
{
    int result; // eax
    int v1; // [esp+8h] [ebp-8h]

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_AR_tickpoil_art_firstinit_AR_tickpoil_art_init = (int)AI_ul_RealResolveOneRefFunction(0x3D0030EBu);
    }
    AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 2, 0);
    AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x8000, 0);
    v1 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_AR_tickpoil_art_firstinit_AR_tickpoil_art_init);
    if (v1 == 1000)
        result = 1000;
    else
        result = v1;
    return result;
}

int AR_tickpoil_art_meurt()
{
    int result; // eax
    tScriptStruct_PNJARTicpoil* v1; // [esp+8h] [ebp-80h]
    tGameObjectMain* v2; // [esp+Ch] [ebp-7Ch]
    tGameObjectMain* v3; // [esp+10h] [ebp-78h]
    tGameObjectMain* v4; // [esp+18h] [ebp-70h]
    Vec3 v5; // [esp+1Ch] [ebp-6Ch] BYREF
    Vec3 v6; // [esp+28h] [ebp-60h] BYREF
    Vec3 a1; // [esp+34h] [ebp-54h] BYREF
    int v8; // [esp+40h] [ebp-48h]
    int v9; // [esp+44h] [ebp-44h]
    Vec3 v10; // [esp+48h] [ebp-40h] BYREF
    Vec3 v11; // [esp+54h] [ebp-34h] BYREF
    Vec3 v12; // [esp+60h] [ebp-28h] BYREF
    int i; // [esp+6Ch] [ebp-1Ch]
    int v14; // [esp+70h] [ebp-18h]
    tScriptStruct_PNJARTicpoil* v15; // [esp+74h] [ebp-14h]
    Vec3 v16; // [esp+78h] [ebp-10h] BYREF
    tGameObjectMain* v17; // [esp+84h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJARTicpoil*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v15 = v1;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        AR_tickpoil_art_meurt_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
        AR_tickpoil_art_meurt_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &AR_tickpoil_art_meurt_INV_manager_gao_i_Animals_Nb,
            (void*)AR_tickpoil_art_meurt_INV_manager_gao,
            23888);
        AI2C_ResolveBuffer(
            &AR_tickpoil_art_meurt_INV_manager_gao_ao_Animals,
            (void*)AR_tickpoil_art_meurt_INV_manager_gao,
            23368);
        AI2C_ResolveBuffer(
            &AR_tickpoil_art_meurt_INV_manager_gao_ai_AnimalsData,
            (void*)AR_tickpoil_art_meurt_INV_manager_gao,
            23628);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)1)
    {
        for (i = 0; i < *(int32_t*)AR_tickpoil_art_meurt_INV_manager_gao_i_Animals_Nb; ++i)
        {
            v17 = AI_gpst_CurrentGameObject;
            v3 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v3 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v2 = v3;
            }
            else
            {
                v2 = 0;
            }
            if (*(tGameObjectMain**)(AR_tickpoil_art_meurt_INV_manager_gao_ao_Animals + 4 * i + 4) == v2)
            {
                --* (uint32_t*)AR_tickpoil_art_meurt_INV_manager_gao_i_Animals_Nb;
                *(uint32_t*)(AR_tickpoil_art_meurt_INV_manager_gao_ao_Animals + 4 * i + 4) = *(uint32_t*)(AR_tickpoil_art_meurt_INV_manager_gao_ao_Animals
                    + 4
                    * *(uint32_t*)AR_tickpoil_art_meurt_INV_manager_gao_i_Animals_Nb
                    + 4);
                *(uint32_t*)(AR_tickpoil_art_meurt_INV_manager_gao_ai_AnimalsData + 4 * i + 4) = *(uint32_t*)(AR_tickpoil_art_meurt_INV_manager_gao_ai_AnimalsData
                    + 4 * *(uint32_t*)AR_tickpoil_art_meurt_INV_manager_gao_i_Animals_Nb
                    + 4);
                break;
            }
        }
        v9 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v9 == 1000)
            result = 1000;
        else
            result = v9;
    }
    else
    {
        if (v15->i_snd_marche != -1)
            SND_Release(v15->i_snd_marche);
        v15->i_snd_marche = -1;
        v8 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(AI_gpst_CurrentGameObject, 4, -1.0, 0.94999999f, 1.05f);
        v14 = AI_EvalFunc_GFXAdd_C_CURRENT(13);
        v15->ti_GFX_Boom = v14;
        GFXSetCurGlobal(v15->ti_GFX_Boom);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 50);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13106, 50);
        v4 = (tGameObjectMain*)AR_tickpoil_art_meurt_SFX_explosion_gao;
        if (AR_tickpoil_art_meurt_SFX_explosion_gao)
        {
            if ((unsigned int)AR_tickpoil_art_meurt_SFX_explosion_gao <= 2)
                v4 = AI_MainActor[AR_tickpoil_art_meurt_SFX_explosion_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v15->ti_GFX_Boom, v4, -1);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v15->ti_GFX_Boom, 0, -1);
        }
        GFXSetCurGlobal(v15->ti_GFX_Boom);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13101, 7);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13103, 0x2066B6DD);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13104, 0x3066B6DD);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13105, 0x66B6DD);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13005, 0.2f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13006, 0.5);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13107, 0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13000, 0.5);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13001, 1.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13002, 0.001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13003, 0.5);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13004, 1.5);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13007, 1.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13009, 2.5);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13010, 5.0);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v15->ti_GFX_Boom, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v15->ti_GFX_Boom, 2, 1);
        GFXSetCurGlobal(v15->ti_GFX_Boom);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13012, 0.75);
        v11.x = -1.0;
        v11.y = -1.0;
        v11.z = -0.5;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v11);
        v6.x = 1.0;
        v6.y = 1.0;
        v6.z = 1.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v6);
        v5.x = 0.029999999f;
        v5.y = 0.029999999f;
        v5.z = 0.029999999f;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v5);
        v10.x = 0.0;
        v10.y = 0.0;
        v10.z = 0.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13205, &v10);
        v16.x = 0.0;
        v16.y = 0.0;
        v16.z = 0.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13206, &v16);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13200, &a1);
        v12.x = 0.0;
        v12.y = 0.0;
        v12.z = 0.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13204, &v12);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.001f);
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        *AI_gppst_CurrentJumpNode = (tAINode*)1;
        TIME_Wait_C(1.0);
        result = 0;
    }
    return result;
}