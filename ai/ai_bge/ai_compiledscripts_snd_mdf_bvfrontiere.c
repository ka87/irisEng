#include "ai_compiledscripts_inc.h"

//Snd_mdf_BVFrontiere_BVF_init
int fct_Snd_mdf_BVFrontiere_BVF_init_Snd_mdf_BVFrontiere_BVF_out_BV;

//Snd_mdf_BVFrontiere_BVF_in_BV_cotevert
int Snd_mdf_BVFrontiere_BVF_in_BV_cotevert__PJ_Sally_gao;
int fct_Snd_mdf_BVFrontiere_BVF_in_BV_cotevert_Snd_mdf_BVFrontiere_BVF_out_BV;
int fct_Snd_mdf_BVFrontiere_BVF_in_BV_cotevert_Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert;

//Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert
int Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert__PJ_Sally_gao;
int fct_Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert_Snd_mdf_BVFrontiere_BVF_out_BV;
int fct_Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert_Snd_mdf_BVFrontiere_BVF_in_BV_cotevert;

//Snd_mdf_BVFrontiere_BVF_out_BV
int Snd_mdf_BVFrontiere_BVF_out_BV__PJ_Sally_gao;
int fct_Snd_mdf_BVFrontiere_BVF_out_BV_Snd_mdf_BVFrontiere_BVF_in_BV_cotevert;
int fct_Snd_mdf_BVFrontiere_BVF_out_BV_Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert;

int Snd_mdf_BVFrontiere_BVF_init()
{
    tScriptStruct_SNDMDFBVFrontiere* v1; // [esp+0h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SNDMDFBVFrontiere*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Snd_mdf_BVFrontiere_BVF_init_Snd_mdf_BVFrontiere_BVF_out_BV = (int)AI_ul_RealResolveOneRefFunction(0x26000240u);
    }
    v1->i_play = 1;
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Snd_mdf_BVFrontiere_BVF_init_Snd_mdf_BVFrontiere_BVF_out_BV);
    return 1000;
}

int Snd_mdf_BVFrontiere_BVF_in_BV_cotevert()
{
    BOOL v0; // eax
    tScriptStruct_SNDMDFBVFrontiere* v2; // [esp+0h] [ebp-D8h]
    tGameObjectMain* v3; // [esp+8h] [ebp-D0h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-CCh]
    tGameObjectMain* v5; // [esp+10h] [ebp-C8h]
    tGameObjectMain* v6; // [esp+18h] [ebp-C0h]
    tGameObjectMain* v7; // [esp+1Ch] [ebp-BCh]
    tGameObjectMain* v8; // [esp+20h] [ebp-B8h]
    tGameObjectMain* v9; // [esp+28h] [ebp-B0h]
    tGameObjectMain* v10; // [esp+2Ch] [ebp-ACh]
    tGameObjectMain* v11; // [esp+30h] [ebp-A8h]
    float v12; // [esp+34h] [ebp-A4h]
    tGameObjectMain* v13; // [esp+3Ch] [ebp-9Ch]
    tGameObjectMain* v14; // [esp+40h] [ebp-98h]
    tGameObjectMain* v15; // [esp+44h] [ebp-94h]
    tGameObjectMain* v16; // [esp+4Ch] [ebp-8Ch]
    tGameObjectMain* v17; // [esp+50h] [ebp-88h]
    tGameObjectMain* v18; // [esp+54h] [ebp-84h]
    tGameObjectMain* v19; // [esp+5Ch] [ebp-7Ch]
    tGameObjectMain* v20; // [esp+60h] [ebp-78h]
    tGameObjectMain* v21; // [esp+64h] [ebp-74h]
    tGameObjectMain* v22; // [esp+6Ch] [ebp-6Ch]
    Vec3 a3; // [esp+70h] [ebp-68h] BYREF
    Vec3 v24; // [esp+7Ch] [ebp-5Ch] BYREF
    Vec3 v25; // [esp+88h] [ebp-50h] BYREF
    float v26; // [esp+94h] [ebp-44h]
    BOOL v27; // [esp+98h] [ebp-40h]
    Vec3 a2; // [esp+9Ch] [ebp-3Ch] BYREF
    Vec3 v29; // [esp+A8h] [ebp-30h] BYREF
    int v31; // [esp+B8h] [ebp-20h]
    tScriptStruct_SNDMDFBVFrontiere* v32; // [esp+BCh] [ebp-1Ch]
    Vec3 dest; // [esp+C0h] [ebp-18h] BYREF
    Vec3 v34; // [esp+CCh] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SNDMDFBVFrontiere*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v32 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Snd_mdf_BVFrontiere_BVF_in_BV_cotevert__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        fct_Snd_mdf_BVFrontiere_BVF_in_BV_cotevert_Snd_mdf_BVFrontiere_BVF_out_BV = (int)AI_ul_RealResolveOneRefFunction(0x26000240u);
        fct_Snd_mdf_BVFrontiere_BVF_in_BV_cotevert_Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert = (int)AI_ul_RealResolveOneRefFunction(0x26000242u);
    }
    v22 = (tGameObjectMain*)Snd_mdf_BVFrontiere_BVF_in_BV_cotevert__PJ_Sally_gao;
    if (Snd_mdf_BVFrontiere_BVF_in_BV_cotevert__PJ_Sally_gao)
    {
        if ((unsigned int)Snd_mdf_BVFrontiere_BVF_in_BV_cotevert__PJ_Sally_gao <= 2)
            v22 = AI_MainActor[Snd_mdf_BVFrontiere_BVF_in_BV_cotevert__PJ_Sally_gao - 1];
        v0 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, v22);
    }
    else
    {
        v0 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, 0);
    }
    v27 = v0;
    if (!v0)
    {
        if (v32->i_play)
        {
            v31 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Snd_mdf_BVFrontiere_BVF_in_BV_cotevert_Snd_mdf_BVFrontiere_BVF_out_BV);
            if (v31 != 1000)
                return v31;
        }
    }
    if (!SND_i_IsTrackPlaying(v32->i_track))
    {
        v32->i_play = 1;
        v21 = (tGameObjectMain*)v32->o_mdf_son1_V;
        if (v21)
        {
            if ((unsigned int)v21 <= 2)
                v21 = AI_MainActor[(uint32_t)v21 - 1];
            v20 = v21;
        }
        else
        {
            v20 = 0;
        }
        if (v20)
        {
            v19 = (tGameObjectMain*)v32->o_mdf_son1_V;
            if (v19)
            {
                if ((unsigned int)v19 <= 2)
                    v19 = AI_MainActor[(uint32_t)v19 - 1];
                AI_EvalFunc_SNDPlayModifier_C(v19, -1);
            }
            else
            {
                AI_EvalFunc_SNDPlayModifier_C(0, -1);
            }
        }
        v18 = (tGameObjectMain*)v32->o_mdf_son2_V;
        if (v18)
        {
            if ((unsigned int)v18 <= 2)
                v18 = AI_MainActor[(uint32_t)v18 - 1];
            v17 = v18;
        }
        else
        {
            v17 = 0;
        }
        if (v17)
        {
            v16 = (tGameObjectMain*)v32->o_mdf_son2_V;
            if (v16)
            {
                if ((unsigned int)v16 <= 2)
                    v16 = AI_MainActor[(uint32_t)v16 - 1];
                AI_EvalFunc_SNDPlayModifier_C(v16, -1);
            }
            else
            {
                AI_EvalFunc_SNDPlayModifier_C(0, -1);
            }
        }
        v15 = (tGameObjectMain*)v32->o_mdf_son3_V;
        if (v15)
        {
            if ((unsigned int)v15 <= 2)
                v15 = AI_MainActor[(uint32_t)v15 - 1];
            v14 = v15;
        }
        else
        {
            v14 = 0;
        }
        if (v14)
        {
            v13 = (tGameObjectMain*)v32->o_mdf_son3_V;
            if (v13)
            {
                if ((unsigned int)v13 <= 2)
                    v13 = AI_MainActor[(uint32_t)v13 - 1];
                AI_EvalFunc_SNDPlayModifier_C(v13, -1);
            }
            else
            {
                AI_EvalFunc_SNDPlayModifier_C(0, -1);
            }
        }
    }
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v24);
    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Snd_mdf_BVFrontiere_BVF_in_BV_cotevert__PJ_Sally_gao, &a3);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v34);
    v12 = v32->f_taille_hysteresis;
    v25.x = v34.x * v32->f_taille_hysteresis;
    v25.y = v34.y * v12;
    v25.z = v34.z * v12;
    MATH_VecAddVec(&v29, &a2, &v25);
    MATH_SubstractVec(&dest, &a3, &v29);
    v26 = dest.z * v24.z + dest.y * v24.y + v24.x * dest.x;
    if (v26 > 0.0)
    {
        v32->i_play = 0;
        v11 = (tGameObjectMain*)v32->o_mdf_son1_V;
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
            v9 = (tGameObjectMain*)v32->o_mdf_son1_V;
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                AI_EvalFunc_SNDStopModifier_C(v9, -1);
            }
            else
            {
                AI_EvalFunc_SNDStopModifier_C(0, -1);
            }
        }
        v8 = (tGameObjectMain*)v32->o_mdf_son2_V;
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
            v6 = (tGameObjectMain*)v32->o_mdf_son2_V;
            if (v6)
            {
                if ((unsigned int)v6 <= 2)
                    v6 = AI_MainActor[(uint32_t)v6 - 1];
                AI_EvalFunc_SNDStopModifier_C(v6, -1);
            }
            else
            {
                AI_EvalFunc_SNDStopModifier_C(0, -1);
            }
        }
        v5 = (tGameObjectMain*)v32->o_mdf_son3_V;
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
            v3 = (tGameObjectMain*)v32->o_mdf_son3_V;
            if (v3)
            {
                if ((unsigned int)v3 <= 2)
                    v3 = AI_MainActor[(uint32_t)v3 - 1];
                AI_EvalFunc_SNDStopModifier_C(v3, -1);
            }
            else
            {
                AI_EvalFunc_SNDStopModifier_C(0, -1);
            }
        }
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Snd_mdf_BVFrontiere_BVF_in_BV_cotevert_Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert);
    }
    return 1000;
}

int Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert()
{
    BOOL v0; // eax
    tScriptStruct_SNDMDFBVFrontiere* v2; // [esp+0h] [ebp-D8h]
    tGameObjectMain* v3; // [esp+8h] [ebp-D0h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-CCh]
    tGameObjectMain* v5; // [esp+10h] [ebp-C8h]
    tGameObjectMain* v6; // [esp+18h] [ebp-C0h]
    tGameObjectMain* v7; // [esp+1Ch] [ebp-BCh]
    tGameObjectMain* v8; // [esp+20h] [ebp-B8h]
    tGameObjectMain* v9; // [esp+28h] [ebp-B0h]
    tGameObjectMain* v10; // [esp+2Ch] [ebp-ACh]
    tGameObjectMain* v11; // [esp+30h] [ebp-A8h]
    float v12; // [esp+34h] [ebp-A4h]
    tGameObjectMain* v13; // [esp+3Ch] [ebp-9Ch]
    tGameObjectMain* v14; // [esp+40h] [ebp-98h]
    tGameObjectMain* v15; // [esp+44h] [ebp-94h]
    tGameObjectMain* v16; // [esp+4Ch] [ebp-8Ch]
    tGameObjectMain* v17; // [esp+50h] [ebp-88h]
    tGameObjectMain* v18; // [esp+54h] [ebp-84h]
    tGameObjectMain* v19; // [esp+5Ch] [ebp-7Ch]
    tGameObjectMain* v20; // [esp+60h] [ebp-78h]
    tGameObjectMain* v21; // [esp+64h] [ebp-74h]
    tGameObjectMain* v22; // [esp+6Ch] [ebp-6Ch]
    Vec3 a3; // [esp+70h] [ebp-68h] BYREF
    Vec3 v24; // [esp+7Ch] [ebp-5Ch] BYREF
    Vec3 v25; // [esp+88h] [ebp-50h] BYREF
    float v26; // [esp+94h] [ebp-44h]
    BOOL v27; // [esp+98h] [ebp-40h]
    Vec3 a2; // [esp+9Ch] [ebp-3Ch] BYREF
    Vec3 dest; // [esp+A8h] [ebp-30h] BYREF
    int v31; // [esp+B8h] [ebp-20h]
    tScriptStruct_SNDMDFBVFrontiere* v32; // [esp+BCh] [ebp-1Ch]
    Vec3 v33; // [esp+C0h] [ebp-18h] BYREF
    Vec3 v34; // [esp+CCh] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SNDMDFBVFrontiere*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v32 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        fct_Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert_Snd_mdf_BVFrontiere_BVF_out_BV = (int)AI_ul_RealResolveOneRefFunction(0x26000240u);
        fct_Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert_Snd_mdf_BVFrontiere_BVF_in_BV_cotevert = (int)AI_ul_RealResolveOneRefFunction(0x26000241u);
    }
    v22 = (tGameObjectMain*)Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert__PJ_Sally_gao;
    if (Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert__PJ_Sally_gao)
    {
        if ((unsigned int)Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert__PJ_Sally_gao <= 2)
            v22 = AI_MainActor[Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert__PJ_Sally_gao - 1];
        v0 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, v22);
    }
    else
    {
        v0 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, 0);
    }
    v27 = v0;
    if (!v0)
    {
        if (v32->i_play)
        {
            v31 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert_Snd_mdf_BVFrontiere_BVF_out_BV);
            if (v31 != 1000)
                return v31;
        }
    }
    if (!SND_i_IsTrackPlaying(v32->i_track))
    {
        v32->i_play = 1;
        v21 = (tGameObjectMain*)v32->o_mdf_son1;
        if (v21)
        {
            if ((unsigned int)v21 <= 2)
                v21 = AI_MainActor[(uint32_t)v21 - 1];
            v20 = v21;
        }
        else
        {
            v20 = 0;
        }
        if (v20)
        {
            v19 = (tGameObjectMain*)v32->o_mdf_son1;
            if (v19)
            {
                if ((unsigned int)v19 <= 2)
                    v19 = AI_MainActor[(uint32_t)v19 - 1];
                AI_EvalFunc_SNDPlayModifier_C(v19, -1);
            }
            else
            {
                AI_EvalFunc_SNDPlayModifier_C(0, -1);
            }
        }
        v18 = (tGameObjectMain*)v32->o_mdf_son2;
        if (v18)
        {
            if ((unsigned int)v18 <= 2)
                v18 = AI_MainActor[(uint32_t)v18 - 1];
            v17 = v18;
        }
        else
        {
            v17 = 0;
        }
        if (v17)
        {
            v16 = (tGameObjectMain*)v32->o_mdf_son2;
            if (v16)
            {
                if ((unsigned int)v16 <= 2)
                    v16 = AI_MainActor[(uint32_t)v16 - 1];
                AI_EvalFunc_SNDPlayModifier_C(v16, -1);
            }
            else
            {
                AI_EvalFunc_SNDPlayModifier_C(0, -1);
            }
        }
        v15 = (tGameObjectMain*)v32->o_mdf_son3;
        if (v15)
        {
            if ((unsigned int)v15 <= 2)
                v15 = AI_MainActor[(uint32_t)v15 - 1];
            v14 = v15;
        }
        else
        {
            v14 = 0;
        }
        if (v14)
        {
            v13 = (tGameObjectMain*)v32->o_mdf_son3;
            if (v13)
            {
                if ((unsigned int)v13 <= 2)
                    v13 = AI_MainActor[(uint32_t)v13 - 1];
                AI_EvalFunc_SNDPlayModifier_C(v13, -1);
            }
            else
            {
                AI_EvalFunc_SNDPlayModifier_C(0, -1);
            }
        }
    }
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v24);
    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert__PJ_Sally_gao, &a3);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v34);
    v12 = v32->f_taille_hysteresis;
    v25.x = v34.x * v32->f_taille_hysteresis;
    v25.y = v34.y * v12;
    v25.z = v34.z * v12;
    MATH_SubstractVec(&dest, &a2, &v25);
    MATH_SubstractVec(&v33, &a3, &dest);
    v26 = v33.z * v24.z + v33.y * v24.y + v24.x * v33.x;
    if (v26 < 0.0)
    {
        v32->i_play = 0;
        v11 = (tGameObjectMain*)v32->o_mdf_son1;
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
            v9 = (tGameObjectMain*)v32->o_mdf_son1;
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                AI_EvalFunc_SNDStopModifier_C(v9, -1);
            }
            else
            {
                AI_EvalFunc_SNDStopModifier_C(0, -1);
            }
        }
        v8 = (tGameObjectMain*)v32->o_mdf_son2;
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
            v6 = (tGameObjectMain*)v32->o_mdf_son2;
            if (v6)
            {
                if ((unsigned int)v6 <= 2)
                    v6 = AI_MainActor[(uint32_t)v6 - 1];
                AI_EvalFunc_SNDStopModifier_C(v6, -1);
            }
            else
            {
                AI_EvalFunc_SNDStopModifier_C(0, -1);
            }
        }
        v5 = (tGameObjectMain*)v32->o_mdf_son3;
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
            v3 = (tGameObjectMain*)v32->o_mdf_son3;
            if (v3)
            {
                if ((unsigned int)v3 <= 2)
                    v3 = AI_MainActor[(uint32_t)v3 - 1];
                AI_EvalFunc_SNDStopModifier_C(v3, -1);
            }
            else
            {
                AI_EvalFunc_SNDStopModifier_C(0, -1);
            }
        }
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert_Snd_mdf_BVFrontiere_BVF_in_BV_cotevert);
    }
    return 1000;
}

int Snd_mdf_BVFrontiere_BVF_out_BV()
{
    BOOL v0; // eax
    tScriptStruct_SNDMDFBVFrontiere* v2; // [esp+0h] [ebp-48h]
    tGameObjectMain* v3; // [esp+8h] [ebp-40h]
    Vec3 dest; // [esp+Ch] [ebp-3Ch] BYREF
    Vec3 v5; // [esp+18h] [ebp-30h] BYREF
    BOOL v6; // [esp+24h] [ebp-24h]
    float v7; // [esp+28h] [ebp-20h]
    Vec3 a3; // [esp+2Ch] [ebp-1Ch] BYREF
    Vec3 a1; // [esp+38h] [ebp-10h] BYREF
    tScriptStruct_SNDMDFBVFrontiere* v10; // [esp+44h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SNDMDFBVFrontiere*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v10 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Snd_mdf_BVFrontiere_BVF_out_BV__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        fct_Snd_mdf_BVFrontiere_BVF_out_BV_Snd_mdf_BVFrontiere_BVF_in_BV_cotevert = (int)AI_ul_RealResolveOneRefFunction(0x26000241u);
        fct_Snd_mdf_BVFrontiere_BVF_out_BV_Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert = (int)AI_ul_RealResolveOneRefFunction(0x26000242u);
    }
    v3 = (tGameObjectMain*)Snd_mdf_BVFrontiere_BVF_out_BV__PJ_Sally_gao;
    if (Snd_mdf_BVFrontiere_BVF_out_BV__PJ_Sally_gao)
    {
        if ((unsigned int)Snd_mdf_BVFrontiere_BVF_out_BV__PJ_Sally_gao <= 2)
            v3 = AI_MainActor[Snd_mdf_BVFrontiere_BVF_out_BV__PJ_Sally_gao - 1];
        v0 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, v3);
    }
    else
    {
        v0 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, 0);
    }
    v6 = v0;
    if (v0)
    {
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a1);
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Snd_mdf_BVFrontiere_BVF_out_BV__PJ_Sally_gao, &a3);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v5);
        MATH_SubstractVec(&dest, &a3, &v5);
        v7 = dest.z * a1.z + dest.y * a1.y + a1.x * dest.x;
        if (v7 >= 0.0)
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Snd_mdf_BVFrontiere_BVF_out_BV_Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert);
        else
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Snd_mdf_BVFrontiere_BVF_out_BV_Snd_mdf_BVFrontiere_BVF_in_BV_cotevert);
    }
    return 1000;
}