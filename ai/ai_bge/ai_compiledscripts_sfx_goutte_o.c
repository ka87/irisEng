#include "ai_compiledscripts_inc.h"

//SFX_GoutteO_g_init
int fct_SFX_GoutteO_g_init_SFX_GoutteO_g_ripple;
int fct_SFX_GoutteO_g_init_SFX_GoutteO_g_gener;

//SFX_GoutteO_g_gener
int SFX_GoutteO_g_gener_SFX_light_smoke_gao;

//SFX_GoutteO_g_move
int SFX_GoutteO_g_move_SFX_light_smoke_gao;
int SFX_GoutteO_g_move__PJ_Sally_gao;

//SFX_GoutteO_g_ripple
int SFX_GoutteO_g_ripple_SFX_light_smoke_gao;

int SFX_GoutteO_g_init()
{
    double v0; // st7
    tScriptStruct_SFXGoutteO* v2; // [esp+8h] [ebp-30h]
    int v3; // [esp+Ch] [ebp-2Ch]
    Vec3 a2; // [esp+14h] [ebp-24h] BYREF
    int v5; // [esp+20h] [ebp-18h]
    Vec3 a3; // [esp+28h] [ebp-10h] BYREF
    tScriptStruct_SFXGoutteO* v7; // [esp+34h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SFXGoutteO*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v7 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_GoutteO_g_init_SFX_GoutteO_g_ripple = (int)AI_ul_RealResolveOneRefFunction(0x3D0009E5u);
        fct_SFX_GoutteO_g_init_SFX_GoutteO_g_gener = (int)AI_ul_RealResolveOneRefFunction(0x3D00058Du);
    }
    a2.x = 0.0;
    a2.y = 0.0;
    a2.z = -1.0;
    a3.x = 0.0;
    a3.y = 1.0;
    a3.z = 0.0;
    AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a2, &a3);
    v7->f_last_gen = TIM_gf_MainClock - TIM_gf_LockTime;
    v0 = AI_EvalFunc_MATHRandFloat_C(v7->des_f_freq_min, v7->des_f_freq_max);
    v7->f_next_freq = (float)v0;
    if (v7->des_i_ripple)
    {
        v3 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_GoutteO_g_init_SFX_GoutteO_g_ripple);
        if (v3 != 1000)
            return v3;
    }
    else
    {
        v5 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_SFX_GoutteO_g_init_SFX_GoutteO_g_gener);
        if (v5 != 1000)
            return v5;
    }
    return 1000;
}

int SFX_GoutteO_g_gener()
{
    int result; // eax
    Vec3* v1; // eax
    float v2; // [esp+8h] [ebp-78h]
    tScriptStruct_SFXGoutteO* v3; // [esp+Ch] [ebp-74h]
    tGameObjectMain* v4; // [esp+14h] [ebp-6Ch]
    Vec3* v5; // [esp+18h] [ebp-68h]
    BOOL v6; // [esp+38h] [ebp-48h]
    float v7; // [esp+44h] [ebp-3Ch]
    float v8; // [esp+48h] [ebp-38h]
    float v9; // [esp+4Ch] [ebp-34h]
    Vec3 a3; // [esp+54h] [ebp-2Ch] BYREF
    unsigned int v11; // [esp+68h] [ebp-18h]
    tScriptStruct_SFXGoutteO* v12; // [esp+6Ch] [ebp-14h]
    int a2; // [esp+70h] [ebp-10h]
    Vec3 a1; // [esp+74h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_SFXGoutteO*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v12 = v3;
    a2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SFX_GoutteO_g_gener_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    if (AI_EvalFunc_TIMEElapsed_C(v12->f_last_gen, v12->f_next_freq))
    {
        v11 = AI_EvalFunc_OBJ_FlagsControlGet_C(AI_gpst_CurrentGameObject);
        if ((v11 & 0x20000) != 0)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            v12->f_last_gen = TIM_gf_MainClock - TIM_gf_LockTime;
            v12->f_next_freq = AI_EvalFunc_MATHRandFloat_C(v12->des_f_freq_min, v12->des_f_freq_max);
            if (v12->i_cur_gouttes < 50)
            {
                for (a2 = 0; a2 < 50; ++a2)
                {
                    v1 = (Vec3*)AI_EvalVarArray_C3((char*)&v12->__av_pos_gouttes_Dimension1, a2);
                    if (fabs(v1->x) != 0.0f)
                    {
                        v6 = 0;
                    }
                    else if (fabs(v1->y) != 0.0f)
                    {
                        v6 = 0;
                    }
                    else
                    {
                        v6 = fabs(v1->z) == 0.0f;
                    }
                    if (v6)
                    {
                        AI_EvalFunc_BVRandomPosGet_C(AI_gpst_CurrentGameObject, 0, &a3);
                        v7 = a3.x;
                        v8 = a3.y;
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
                        v9 = a1.z;
                        v5 = (Vec3*)AI_EvalVarArray_C3((char*)&v12->__av_pos_gouttes_Dimension1, a2);
                        v5->x = v7;
                        v5->y = v8;
                        v5->z = v9;
                        *(uint32_t*)AI_EvalVarArray_C2((char*)&v12->__af_speed_gouttes_Dimension1, a2) = 0;
                        v2 = AI_EvalFunc_MATHRandFloat_C(v12->des_f_gravity_min, v12->des_f_gravity_max);
                        *(float*)AI_EvalVarArray_C2((char*)&v12->__af_vit_gouttes_Dimension1, a2) = v2;
                        v12->ai_id_gouttes[a2] = AI_EvalFunc_GFXAdd_C_CURRENT(1);
                        GFXSetCurGlobal(v12->ai_id_gouttes[a2]);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(1000, v12->des_f_largeur_g);
                        AI_EvalFunc_GFXSeti_C_CURGLOB(1100, v12->des_c_goutte);
                        AI_EvalFunc_GFXFlagSet_C_CURRENT(v12->ai_id_gouttes[a2], 0, 1);
                        AI_EvalFunc_GFXFlagSet_C_CURRENT(v12->ai_id_gouttes[a2], 2, 1);
                        v4 = (tGameObjectMain*)SFX_GoutteO_g_gener_SFX_light_smoke_gao;
                        if (SFX_GoutteO_g_gener_SFX_light_smoke_gao)
                        {
                            if ((unsigned int)SFX_GoutteO_g_gener_SFX_light_smoke_gao <= 2)
                                v4 = AI_MainActor[SFX_GoutteO_g_gener_SFX_light_smoke_gao - 1];
                            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v12->ai_id_gouttes[a2], v4, 15);
                        }
                        else
                        {
                            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v12->ai_id_gouttes[a2], 0, 15);
                        }
                        if (a2 > v12->i_cur_gouttes)
                            v12->i_cur_gouttes = a2;
                        break;
                    }
                }
                result = 1000;
            }
            else
            {
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 1000;
            }
        }
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int SFX_GoutteO_g_move()
{
    BOOL v0; // eax
    char* v1; // eax
    char* v2; // eax
    float v4; // [esp+0h] [ebp-F8h]
    float v5; // [esp+0h] [ebp-F8h]
    float v6; // [esp+0h] [ebp-F8h]
    float v7; // [esp+4h] [ebp-F4h]
    float v8; // [esp+8h] [ebp-F0h]
    char* v9; // [esp+Ch] [ebp-ECh]
    tScriptStruct_SFXGoutteO* v10; // [esp+10h] [ebp-E8h]
    tGameObjectMain* v11; // [esp+24h] [ebp-D4h]
    tGameObjectMain* v12; // [esp+2Ch] [ebp-CCh]
    Vec3* v13; // [esp+30h] [ebp-C8h]
    Vec3 v14; // [esp+50h] [ebp-A8h] BYREF
    float v15; // [esp+5Ch] [ebp-9Ch]
    Vec3 v16; // [esp+64h] [ebp-94h] BYREF
    float v17; // [esp+70h] [ebp-88h]
    Vec3 dest; // [esp+74h] [ebp-84h] BYREF
    int v19; // [esp+80h] [ebp-78h]
    BOOL v20; // [esp+84h] [ebp-74h]
    int v21; // [esp+88h] [ebp-70h]
    float v22; // [esp+8Ch] [ebp-6Ch]
    Vec3 v23; // [esp+90h] [ebp-68h] BYREF
    Vec3 v24; // [esp+9Ch] [ebp-5Ch] BYREF
    int v25; // [esp+A8h] [ebp-50h]
    float v26; // [esp+B8h] [ebp-40h]
    Vec3 a2; // [esp+BCh] [ebp-3Ch] BYREF
    int v28; // [esp+C8h] [ebp-30h]
    int v29; // [esp+CCh] [ebp-2Ch]
    int a1; // [esp+D0h] [ebp-28h]
    float v31; // [esp+D4h] [ebp-24h]
    tScriptStruct_SFXGoutteO* v32; // [esp+D8h] [ebp-20h]
    float v33; // [esp+DCh] [ebp-1Ch]
    int i; // [esp+E0h] [ebp-18h]
    Vec3 a3; // [esp+E4h] [ebp-14h] BYREF
    float v36; // [esp+F4h] [ebp-4h]

    a2.x = 1.0;
    a2.y = 0.0;
    a2.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v10 = (tScriptStruct_SFXGoutteO*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v10 = 0;
    v32 = v10;
    v28 = 0;
    v33 = 0.0;
    v29 = 0;
    a1 = 0;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SFX_GoutteO_g_move_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        SFX_GoutteO_g_move__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
    }
    for (i = 0; i <= v32->i_cur_gouttes; ++i)
    {
        a2 = *(Vec3*)AI_EvalVarArray_C3((char*)&v32->__av_pos_gouttes_Dimension1, i);
        v0 = AI2C_VecEqNull(&a2);
        v20 = v0;
        if (!v0)
        {
            v31 = TIM_gf_dt;
            v9 = AI_EvalVarArray_C2((char*)&v32->__af_speed_gouttes_Dimension1, i);
            v1 = AI_EvalVarArray_C2((char*)&v32->__af_vit_gouttes_Dimension1, i);
            *(float*)v9 = *(float*)v1 * v31 + *(float*)v9;
            v2 = AI_EvalVarArray_C2((char*)&v32->__af_speed_gouttes_Dimension1, i);
            a2.z = a2.z - *(float*)v2;
            v13 = (Vec3*)AI_EvalVarArray_C3((char*)&v32->__av_pos_gouttes_Dimension1, i);
            *v13 = a2;
            v8 = v32->des_f_longueur_g;
            v33 = *(float*)AI_EvalVarArray_C2((char*)&v32->__af_speed_gouttes_Dimension1, i) * 3.0f + v8;
            if (v32->des_f_zsol < (double)a2.z)
            {
                a2 = *(Vec3*)AI_EvalVarArray_C3((char*)&v32->__av_pos_gouttes_Dimension1, i);
                if (v32->des_f_zsol > (double)a2.z)
                    a2.z = v32->des_f_zsol;
                GFXSetCurGlobal(v32->ai_id_gouttes[i]);
                AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &a2);
                a2.z = a2.z + v33;
                AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &a2);
            }
            else
            {
                *(uint32_t*)AI_EvalVarArray_C3((char*)&v32->__av_pos_gouttes_Dimension1, i) = 0;
                *(uint32_t*)(AI_EvalVarArray_C3((char*)&v32->__av_pos_gouttes_Dimension1, i) + 4) = 0;
                *(uint32_t*)(AI_EvalVarArray_C3((char*)&v32->__av_pos_gouttes_Dimension1, i) + 8) = 0;
                AI_EvalFunc_GFXDel_C_CURRENT(v32->ai_id_gouttes[i]);
                v29 = AI_EvalFunc_GFXAdd_C_CURRENT(9);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v29, 0, 1);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v29, 2, 1);
                v12 = (tGameObjectMain*)SFX_GoutteO_g_move_SFX_light_smoke_gao;
                if (SFX_GoutteO_g_move_SFX_light_smoke_gao)
                {
                    if ((unsigned int)SFX_GoutteO_g_move_SFX_light_smoke_gao <= 2)
                        v12 = AI_MainActor[SFX_GoutteO_g_move_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v29, v12, 5);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v29, 0, 5);
                }
                a2.z = v32->des_f_zsol;
                GFXSetCurGlobal(v29);
                AI_EvalFunc_GFXSetv_C_CURGLOB(9200, &a2);
                v14.x = 0.0;
                v14.y = 0.0;
                v14.z = 0.1f;
                AI_EvalFunc_GFXSetv_C_CURGLOB(9201, &v14);
                v23.x = 0.0;
                v23.y = 0.0;
                v23.z = -10.0;
                AI_EvalFunc_GFXSetv_C_CURGLOB(9202, &v23);
                AI_EvalFunc_GFXSeti_C_CURGLOB(9100, 10);
                if (v32->des_i_color_spark)
                    AI_EvalFunc_GFXSeti_C_CURRENT(v29, 9101, v32->des_c_spark);
                else
                    AI_EvalFunc_GFXSeti_C_CURRENT(v29, 9101, v32->des_c_ripple);
                GFXSetCurGlobal(v29);
                AI_EvalFunc_GFXSeti_C_CURGLOB(9102, 24);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9000, 0.1f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9001, 30.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9002, 40.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9003, 0.99000001f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9004, 0.039999999f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9005, 0.1f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9006, 0.2f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9007, 2.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9008, 0.0099999998f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9009, 0.2f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9010, 0.30000001f);
                v28 = AI_EvalFunc_GFXAdd_C_CURRENT(20);
                v11 = (tGameObjectMain*)SFX_GoutteO_g_move_SFX_light_smoke_gao;
                if (SFX_GoutteO_g_move_SFX_light_smoke_gao)
                {
                    if ((unsigned int)SFX_GoutteO_g_move_SFX_light_smoke_gao <= 2)
                        v11 = AI_MainActor[SFX_GoutteO_g_move_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v28, v11, 16);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v28, 0, 16);
                }
                v17 = AI_EvalFunc_MATHRandFloat_C(0.5, 1.0);
                GFXSetCurGlobal(v28);
                v4 = v17 + 1.0f;
                AI_EvalFunc_GFXSetf_C_CURGLOB(20000, v4);
                v26 = AI_EvalFunc_MATHRandFloat_C(0.80000001f, 1.2f);
                v5 = v26 + 1.5f;
                AI_EvalFunc_GFXSetf_C_CURGLOB(20001, v5);
                AI_EvalFunc_GFXSetf_C_CURGLOB(20002, 0.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(20003, 0.0);
                AI_EvalFunc_GFXSeti_C_CURGLOB(20100, 1);
                AI_EvalFunc_GFXSeti_C_CURGLOB(20100, -2);
                AI_EvalFunc_GFXSeti_C_CURGLOB(20101, v32->des_c_ripple);
                AI_EvalFunc_GFXSeti_C_CURGLOB(20102, 0);
                a2.z = v32->des_f_zsol;
                AI_EvalFunc_GFXSetv_C_CURGLOB(20200, &a2);
                AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)SFX_GoutteO_g_move__PJ_Sally_gao, &a3);
                MATH_SubstractVec(&dest, &a2, &a3);
                AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)SFX_GoutteO_g_move__PJ_Sally_gao, &v24);
                MATH_SubstractVec(&v16, &a2, &v24);
                v36 = v16.z * dest.z + v16.y * dest.y + dest.x * v16.x;
                if (v36 < 100.0)
                {
                    v25 = AI_EvalFunc_MATHRandInt_C(0, 2);
                    v19 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, v25, 128);
                    a1 = v19;
                    AI_EvalFunc_SNDUltraPlaySound_C(AI_gpst_CurrentGameObject, v19);
                    SND_SetFlags(AI_gpst_CurrentGameObject, a1, 0x18000000);
                    v21 = SND_l_GetFreq(a1);
                    v15 = AI_EvalFunc_MATHRandFloat_C(0.69999999f, 1.0);
                    v7 = (float)v21;
                    SND_SetFreq(a1, (unsigned int)(v7 * v15));
                    v22 = SND_f_GetVol(a1);
                    v6 = v32->f_coef_volume * v22;
                    SND_SetVol(a1, v6);
                    AI_EvalFunc_SNDInstPosVSet_C(a1, &a2);
                }
            }
        }
    }
    return 1000;
}

int SFX_GoutteO_g_ripple()
{
    float v1; // [esp+4h] [ebp-6Ch]
    tScriptStruct_SFXGoutteO* v2; // [esp+8h] [ebp-68h]
    tGameObjectMain* v3; // [esp+20h] [ebp-50h]
    float v4; // [esp+24h] [ebp-4Ch]
    float v5; // [esp+28h] [ebp-48h]
    float v6; // [esp+34h] [ebp-3Ch]
    Vec3 v7; // [esp+3Ch] [ebp-34h] BYREF
    Vec3 v8; // [esp+48h] [ebp-28h] BYREF
    tScriptStruct_SFXGoutteO* v9; // [esp+5Ch] [ebp-14h]
    int a1; // [esp+60h] [ebp-10h]
    Vec3 a3; // [esp+64h] [ebp-Ch] BYREF

    v7.x = 1.0;
    v7.y = 0.0;
    v7.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SFXGoutteO*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v9 = v2;
    a1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SFX_GoutteO_g_ripple_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    if (AI_EvalFunc_TIMEElapsed_C(v9->f_last_gen, v9->f_next_freq))
    {
        v9->f_last_gen = TIM_gf_MainClock - TIM_gf_LockTime;
        a1 = AI_EvalFunc_GFXAdd_C_CURRENT(20);
        v3 = (tGameObjectMain*)SFX_GoutteO_g_ripple_SFX_light_smoke_gao;
        if (SFX_GoutteO_g_ripple_SFX_light_smoke_gao)
        {
            if ((unsigned int)SFX_GoutteO_g_ripple_SFX_light_smoke_gao <= 2)
                v3 = AI_MainActor[SFX_GoutteO_g_ripple_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, v3, 16);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, 0, 16);
        }
        v5 = AI_EvalFunc_MATHRandFloat_C(v9->des_f_minlife, v9->des_f_maxlife);
        GFXSetCurGlobal(a1);
        AI_EvalFunc_GFXSetf_C_CURGLOB(20000, v5);
        v4 = AI_EvalFunc_MATHRandFloat_C(0.80000001f, 1.2f);
        v1 = v4 + 1.5f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(20001, v1);
        AI_EvalFunc_GFXSetf_C_CURGLOB(20002, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(20003, 0.0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(20100, 1);
        AI_EvalFunc_GFXSeti_C_CURGLOB(20100, -2);
        AI_EvalFunc_GFXSeti_C_CURGLOB(20101, v9->des_c_ripple);
        AI_EvalFunc_GFXSeti_C_CURGLOB(20102, 0);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v8);
        v6 = v8.z;
        AI_EvalFunc_BVRandomPosGet_C(AI_gpst_CurrentGameObject, 0, &a3);
        v7.x = a3.x;
        v7.y = a3.y;
        v7.z = v6;
        AI_EvalFunc_GFXSetv_C_CURGLOB(20200, &v7);
    }
    return 1000;
}