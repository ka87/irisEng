#include "ai_compiledscripts_inc.h"

//Porte_Anti_Domz_PAD_mode_ouverture
int fct_Porte_Anti_Domz_PAD_mode_ouverture_Porte_Anti_Domz_PAD_mode_ouverte;

//Porte_Anti_Domz_PAD_init
int Porte_Anti_Domz_PAD_init_univVars;
int Porte_Anti_Domz_PAD_init_univVars_Flag_speciaux_sauve;
int fct_Porte_Anti_Domz_PAD_init_Porte_Anti_Domz_PAD_mode_fermee;
int fct_Porte_Anti_Domz_PAD_init_Porte_Anti_Domz_PAD_mode_ouverte;

//Porte_Anti_Domz_PAD_mode_fermeture
int fct_Porte_Anti_Domz_PAD_mode_fermeture_Porte_Anti_Domz_PAD_mode_fermee;

//Porte_Anti_Domz_PAD_mode_fermee
int Porte_Anti_Domz_PAD_mode_fermee_univVars;
int Porte_Anti_Domz_PAD_mode_fermee_univVars_Flag_speciaux_sauve;
int fct_Porte_Anti_Domz_PAD_mode_fermee_Porte_Anti_Domz_PAD_mode_ouverture;

//Porte_Anti_Domz_PAD_mode_ouverte
int Porte_Anti_Domz_PAD_mode_ouverte_univVars;
int Porte_Anti_Domz_PAD_mode_ouverte_univVars_Flag_speciaux_sauve;
int fct_Porte_Anti_Domz_PAD_mode_ouverte_Porte_Anti_Domz_PAD_mode_fermeture;

int Porte_Anti_Domz_PAD_mode_ouverture()
{
    tScriptStruct_PorteAntiDomz* v1; // [esp+0h] [ebp-A0h]
    tGameObjectMain* v2; // [esp+8h] [ebp-98h]
    tGameObjectMain* v3; // [esp+10h] [ebp-90h]
    float v4; // [esp+14h] [ebp-8Ch]
    Vec3 v5; // [esp+1Ch] [ebp-84h] BYREF
    Vec3 v6; // [esp+28h] [ebp-78h] BYREF
    Vec3 v7; // [esp+34h] [ebp-6Ch] BYREF
    Vec3 v8; // [esp+40h] [ebp-60h] BYREF
    float v9; // [esp+4Ch] [ebp-54h]
    int v10; // [esp+50h] [ebp-50h]
    Vec3 v11; // [esp+54h] [ebp-4Ch] BYREF
    Vec3 v12; // [esp+60h] [ebp-40h] BYREF
    Vec3 a4; // [esp+6Ch] [ebp-34h] BYREF
    Vec3 a1; // [esp+78h] [ebp-28h] BYREF
    tScriptStruct_PorteAntiDomz* v15; // [esp+84h] [ebp-1Ch]
    Vec3 v16; // [esp+88h] [ebp-18h] BYREF
    Vec3 a3; // [esp+94h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PorteAntiDomz*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v15 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Porte_Anti_Domz_PAD_mode_ouverture_Porte_Anti_Domz_PAD_mode_ouverte = (int)AI_ul_RealResolveOneRefFunction(0x72005583u);
    }
    v9 = TIM_gf_MainClock - TIM_gf_LockTime;
    v4 = (v9 - v15->f_time_changement_etat) / v15->des_delai_ouverture;
    if (v4 < 1.0)
    {
        v3 = (tGameObjectMain*)v15->des_wp_porte_ferme;
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
        AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &v7);
        AI_EvalFunc_MATHVecBlend_C(&a3, &v7, v4, &a4);
        AI_EvalFunc_OBJSetPos_C_CURRENT(&a4);
        v2 = (tGameObjectMain*)v15->des_wp_porte_ferme;
        if (v2)
        {
            if ((unsigned int)v2 <= 2)
                v2 = AI_MainActor[(uint32_t)v2 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v2, &v16);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &v16);
        }
        AI_EvalFunc_OBJ_SightInitGet_C(AI_gpst_CurrentGameObject, &v8);
        AI_EvalFunc_MATHVecBlend_C(&v16, &v8, v4, &v11);
        v6.x = 0.0;
        v6.y = 0.0;
        v6.z = 1.0;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v11, &v6);
    }
    else
    {
        AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &a1);
        AI_EvalFunc_OBJSetPos_C_CURRENT(&a1);
        AI_EvalFunc_OBJ_SightInitGet_C(AI_gpst_CurrentGameObject, &v12);
        v5.x = 0.0;
        v5.y = 0.0;
        v5.z = 1.0;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v12, &v5);
        if (v15->i_snd_instance != -1)
        {
            SND_Release(v15->i_snd_instance);
            v15->i_snd_instance = -1;
        }
        AI_EvalFunc_SNDRequestPlay_C_CURRENT(2, -1.0);
        v10 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Porte_Anti_Domz_PAD_mode_ouverture_Porte_Anti_Domz_PAD_mode_ouverte);
        if (v10 != 1000)
            return v10;
    }
    return 1000;
}

int Porte_Anti_Domz_PAD_init()
{
    tScriptStruct_PorteAntiDomz* v1; // [esp+0h] [ebp-5Ch]
    tGameObjectMain* v2; // [esp+8h] [ebp-54h]
    tGameObjectMain* v3; // [esp+10h] [ebp-4Ch]
    Vec3 v4; // [esp+1Ch] [ebp-40h] BYREF
    Vec3 a1; // [esp+28h] [ebp-34h] BYREF
    int v6; // [esp+38h] [ebp-24h]
    Vec3 a3; // [esp+3Ch] [ebp-20h] BYREF
    BOOL v8; // [esp+48h] [ebp-14h]
    tScriptStruct_PorteAntiDomz* v9; // [esp+50h] [ebp-Ch]
    BOOL v10; // [esp+54h] [ebp-8h]
    int v11; // [esp+58h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PorteAntiDomz*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v9 = v1;
    v8 = 0;
    v10 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Porte_Anti_Domz_PAD_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Porte_Anti_Domz_PAD_init_univVars_Flag_speciaux_sauve,
            (void*)Porte_Anti_Domz_PAD_init_univVars,
            4488);
        fct_Porte_Anti_Domz_PAD_init_Porte_Anti_Domz_PAD_mode_fermee = (int)AI_ul_RealResolveOneRefFunction(0x72005582u);
        fct_Porte_Anti_Domz_PAD_init_Porte_Anti_Domz_PAD_mode_ouverte = (int)AI_ul_RealResolveOneRefFunction(0x72005583u);
    }
    v10 = AI_EvalFunc_ARRBitTest_C((int*)(Porte_Anti_Domz_PAD_init_univVars_Flag_speciaux_sauve + 4), 2718);
    if (v9->des_SF_supplementaire <= 0)
        v8 = 0;
    else
        v8 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Porte_Anti_Domz_PAD_init_univVars_Flag_speciaux_sauve + 4),
            v9->des_SF_supplementaire);
    if (v10 || v8)
    {
        v3 = (tGameObjectMain*)v9->des_wp_porte_ferme;
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
        AI_EvalFunc_OBJSetPos_C_CURRENT(&a3);
        v2 = (tGameObjectMain*)v9->des_wp_porte_ferme;
        if (v2)
        {
            if ((unsigned int)v2 <= 2)
                v2 = AI_MainActor[(uint32_t)v2 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v2, &a1);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &a1);
        }
        v4.x = 0.0;
        v4.y = 0.0;
        v4.z = 1.0;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a1, &v4);
        v6 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Porte_Anti_Domz_PAD_init_Porte_Anti_Domz_PAD_mode_fermee);
        if (v6 != 1000)
            return v6;
    }
    else
    {
        v11 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Porte_Anti_Domz_PAD_init_Porte_Anti_Domz_PAD_mode_ouverte);
        if (v11 != 1000)
            return v11;
    }
    return 1000;
}

int Porte_Anti_Domz_PAD_mode_fermeture()
{
    tScriptStruct_PorteAntiDomz* v1; // [esp+0h] [ebp-B0h]
    tGameObjectMain* v2; // [esp+8h] [ebp-A8h]
    tGameObjectMain* v3; // [esp+10h] [ebp-A0h]
    tGameObjectMain* v4; // [esp+18h] [ebp-98h]
    tGameObjectMain* v5; // [esp+20h] [ebp-90h]
    float v6; // [esp+24h] [ebp-8Ch]
    Vec3 v7; // [esp+2Ch] [ebp-84h] BYREF
    Vec3 v8; // [esp+38h] [ebp-78h] BYREF
    Vec3 v9; // [esp+44h] [ebp-6Ch] BYREF
    Vec3 v10; // [esp+50h] [ebp-60h] BYREF
    float v11; // [esp+5Ch] [ebp-54h]
    int v12; // [esp+60h] [ebp-50h]
    Vec3 a4; // [esp+64h] [ebp-4Ch] BYREF
    Vec3 v14; // [esp+70h] [ebp-40h] BYREF
    Vec3 a1; // [esp+7Ch] [ebp-34h] BYREF
    Vec3 a3; // [esp+88h] [ebp-28h] BYREF
    tScriptStruct_PorteAntiDomz* v17; // [esp+94h] [ebp-1Ch]
    Vec3 v18; // [esp+98h] [ebp-18h] BYREF
    Vec3 v19; // [esp+A4h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PorteAntiDomz*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v17 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Porte_Anti_Domz_PAD_mode_fermeture_Porte_Anti_Domz_PAD_mode_fermee = (int)AI_ul_RealResolveOneRefFunction(0x72005582u);
    }
    v11 = TIM_gf_MainClock - TIM_gf_LockTime;
    v6 = (v11 - v17->f_time_changement_etat) / v17->des_delai_fermeture;
    if (v6 < 1.0)
    {
        AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &v19);
        v3 = (tGameObjectMain*)v17->des_wp_porte_ferme;
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            AI_EvalFunc_OBJGetPos_C(v3, &v9);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v9);
        }
        AI_EvalFunc_MATHVecBlend_C(&v19, &v9, v6, &a1);
        AI_EvalFunc_OBJSetPos_C_CURRENT(&a1);
        AI_EvalFunc_OBJ_SightInitGet_C(AI_gpst_CurrentGameObject, &v18);
        v2 = (tGameObjectMain*)v17->des_wp_porte_ferme;
        if (v2)
        {
            if ((unsigned int)v2 <= 2)
                v2 = AI_MainActor[(uint32_t)v2 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v2, &v10);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &v10);
        }
        AI_EvalFunc_MATHVecBlend_C(&v18, &v10, v6, &a4);
        v8.x = 0.0;
        v8.y = 0.0;
        v8.z = 1.0;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a4, &v8);
    }
    else
    {
        v5 = (tGameObjectMain*)v17->des_wp_porte_ferme;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            AI_EvalFunc_OBJGetPos_C(v5, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        AI_EvalFunc_OBJSetPos_C_CURRENT(&a3);
        v4 = (tGameObjectMain*)v17->des_wp_porte_ferme;
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v4, &v14);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &v14);
        }
        v7.x = 0.0;
        v7.y = 0.0;
        v7.z = 1.0;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v14, &v7);
        if (v17->i_snd_instance != -1)
        {
            SND_Release(v17->i_snd_instance);
            v17->i_snd_instance = -1;
        }
        AI_EvalFunc_SNDRequestPlay_C_CURRENT(3, -1.0);
        v12 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Porte_Anti_Domz_PAD_mode_fermeture_Porte_Anti_Domz_PAD_mode_fermee);
        if (v12 != 1000)
            return v12;
    }
    return 1000;
}

int Porte_Anti_Domz_PAD_mode_fermee()
{
    int result; // eax
    tScriptStruct_PorteAntiDomz* v1; // [esp+0h] [ebp-2Ch]
    int v2; // [esp+Ch] [ebp-20h]
    BOOL v3; // [esp+1Ch] [ebp-10h]
    BOOL v4; // [esp+28h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PorteAntiDomz*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Porte_Anti_Domz_PAD_mode_fermee_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Porte_Anti_Domz_PAD_mode_fermee_univVars_Flag_speciaux_sauve,
            (void*)Porte_Anti_Domz_PAD_mode_fermee_univVars,
            4488);
        fct_Porte_Anti_Domz_PAD_mode_fermee_Porte_Anti_Domz_PAD_mode_ouverture = (int)AI_ul_RealResolveOneRefFunction(0x72005584u);
    }
    v4 = AI_EvalFunc_ARRBitTest_C((int*)(Porte_Anti_Domz_PAD_mode_fermee_univVars_Flag_speciaux_sauve + 4), 2718);
    if (v1->des_SF_supplementaire <= 0)
        v3 = 0;
    else
        v3 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Porte_Anti_Domz_PAD_mode_fermee_univVars_Flag_speciaux_sauve + 4),
            v1->des_SF_supplementaire);
    if (v4
        || v3
        || (v1->i_snd_instance = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 1, 0x18000000),
            AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v1->i_snd_instance),
            v1->f_time_changement_etat = TIM_gf_MainClock - TIM_gf_LockTime,
            v2 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Porte_Anti_Domz_PAD_mode_fermee_Porte_Anti_Domz_PAD_mode_ouverture),
            v2 == 1000))
    {
        result = 1000;
    }
    else
    {
        result = v2;
    }
    return result;
}

int Porte_Anti_Domz_PAD_mode_ouverte()
{
    int result; // eax
    tScriptStruct_PorteAntiDomz* v1; // [esp+8h] [ebp-2Ch]
    BOOL v2; // [esp+24h] [ebp-10h]
    BOOL v3; // [esp+30h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PorteAntiDomz*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Porte_Anti_Domz_PAD_mode_ouverte_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Porte_Anti_Domz_PAD_mode_ouverte_univVars_Flag_speciaux_sauve,
            (void*)Porte_Anti_Domz_PAD_mode_ouverte_univVars,
            4488);
        fct_Porte_Anti_Domz_PAD_mode_ouverte_Porte_Anti_Domz_PAD_mode_fermeture = (int)AI_ul_RealResolveOneRefFunction(0x72005585u);
    }
    if (*AI_gppst_CurrentJumpNode != (tAINode*)6)
    {
        v3 = AI_EvalFunc_ARRBitTest_C((int*)(Porte_Anti_Domz_PAD_mode_ouverte_univVars_Flag_speciaux_sauve + 4), 2718);
        if (v1->des_SF_supplementaire <= 0)
            v2 = 0;
        else
            v2 = AI_EvalFunc_ARRBitTest_C(
                (int*)(Porte_Anti_Domz_PAD_mode_ouverte_univVars_Flag_speciaux_sauve + 4),
                v1->des_SF_supplementaire);
        if (v3 || v2)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)6;
            TIME_Wait_C(v1->des_delai_av_fermeture);
            return 0;
        }
        return 1000;
    }
    v1->i_snd_instance = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000000);
    AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v1->i_snd_instance);
    v1->f_time_changement_etat = TIM_gf_MainClock - TIM_gf_LockTime;
    result = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Porte_Anti_Domz_PAD_mode_ouverte_Porte_Anti_Domz_PAD_mode_fermeture);
    if (result == 1000)
        return 1000;
    return result;
}