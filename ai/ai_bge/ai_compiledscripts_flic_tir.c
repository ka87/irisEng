#include "ai_compiledscripts_inc.h"

//Flic_Tir
int Flic_Tir_ft_init_SFX_light_smoke_gao;
int fct_Flic_Tir_ft_init_Flic_Tir_ft_loop;

//Flic_Tir_ft_explode
int Flic_Tir_ft_explode_SFX_explosion_gao;

//Flic_Tir_ft_loop
int fct_Flic_Tir_ft_loop_Flic_Tir_ft_explode;
int Flic_Tir_ft_loop_GST_Global_gao;
int Flic_Tir_ft_loop_GST_Global_gao_f_eau_hauteurZ;

//Flic_Tir_ft_afterr
int fct_Flic_Tir_ft_afterr_Flic_Tir_ft_explode;

int Flic_Tir_ft_init()
{
    int result; // eax
    tScriptStruct_FlicTir* v1; // [esp+0h] [ebp-50h]
    tGameObjectMain* v2; // [esp+8h] [ebp-48h]
    tGameObjectMain* v3; // [esp+10h] [ebp-40h]
    float v4; // [esp+20h] [ebp-30h]
    Vec3 a2; // [esp+34h] [ebp-1Ch] BYREF
    int v6; // [esp+40h] [ebp-10h]
    int v7; // [esp+44h] [ebp-Ch]
    tScriptStruct_FlicTir* v8; // [esp+48h] [ebp-8h]
    int v9; // [esp+4Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_FlicTir*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v8 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Flic_Tir_ft_init_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        fct_Flic_Tir_ft_init_Flic_Tir_ft_loop = (int)AI_ul_RealResolveOneRefFunction(0x3D00115Du);
    }
    AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
    v7 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x3029004, 0);
    a2.x = 0.0;
    a2.y = 0.0;
    a2.z = 0.0;
    AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &a2);
    AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, -1, 0);
    AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
    XMEN_SetSize(AI_gpst_CurrentGameObject, 0, -1, 15.0);
    v8->GFX_Halo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
    AI_EvalFunc_GFXFlagSet_C_CURRENT(v8->GFX_Halo, 4, 1);
    v3 = (tGameObjectMain*)Flic_Tir_ft_init_SFX_light_smoke_gao;
    if (Flic_Tir_ft_init_SFX_light_smoke_gao)
    {
        if ((unsigned int)Flic_Tir_ft_init_SFX_light_smoke_gao <= 2)
            v3 = AI_MainActor[Flic_Tir_ft_init_SFX_light_smoke_gao - 1];
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v8->GFX_Halo, v3, 0);
    }
    else
    {
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v8->GFX_Halo, 0, 0);
    }
    GFXSetCurGlobal(v8->GFX_Halo);
    AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 10.0);
    AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
    AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 255);
    AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
    v2 = *AI_MainActor;
    if (*AI_MainActor)
    {
        if ((unsigned int)v2 <= 2)
            v2 = AI_MainActor[(uint32_t)v2 - 1];
        AI_EvalFunc_DYNSpeedGetVector_C(v2, &v8->v_speed);
    }
    else
    {
        AI_EvalFunc_DYNSpeedGetVector_C(0, &v8->v_speed);
    }
    v9 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v8->v_speed);
    v4 = TIM_gf_MainClock - TIM_gf_LockTime;
    v8->f_time_live = v4;
    v6 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Flic_Tir_ft_init_Flic_Tir_ft_loop);
    if (v6 == 1000)
        result = 1000;
    else
        result = v6;
    return result;
}

int Flic_Tir_ft_explode()
{
    int result; // eax
    tScriptStruct_FlicTir* v1; // [esp+0h] [ebp-104h]
    tGameObjectMain* v2; // [esp+8h] [ebp-FCh]
    tGameObjectMain* v3; // [esp+10h] [ebp-F4h]
    tGameObjectMain* v4; // [esp+28h] [ebp-DCh]
    tGameObjectMain* v5; // [esp+2Ch] [ebp-D8h]
    tGameObjectMain* v6; // [esp+30h] [ebp-D4h]
    Vec3 a3; // [esp+5Ch] [ebp-A8h] BYREF
    Vec3 pos; // [esp+68h] [ebp-9Ch] BYREF
    Vec3 dest; // [esp+74h] [ebp-90h] BYREF
    int v10; // [esp+80h] [ebp-84h]
    tMessage v11; // [esp+84h] [ebp-80h] BYREF
    tMessage Src; // [esp+B8h] [ebp-4Ch] BYREF
    Vec3 v13; // [esp+ECh] [ebp-18h] BYREF
    tScriptStruct_FlicTir* v14; // [esp+F8h] [ebp-Ch]
    tGameObjectMain* v15; // [esp+FCh] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_FlicTir*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v14 = v1;
    v15 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Flic_Tir_ft_explode_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
    }
    v6 = (tGameObjectMain*)v14->o_send_paf;
    if (v6)
    {
        if ((unsigned int)v6 <= 2)
            v6 = AI_MainActor[(uint32_t)v6 - 1];
        v5 = v6;
    }
    else
    {
        v5 = 0;
    }
    if (v5)
    {
        v4 = (tGameObjectMain*)v14->o_send_paf;
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            AI_EvalFunc_OBJGetPos_C(v4, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v13);
        MATH_SubstractVec(&dest, &a3, &v13);
        Src.msgID = 37;
        Src.gao = AI_gpst_CurrentGameObject;
        Src.gao2 = 0;
        Src.Int1 = 101;
        Src.Int2 = 0;
        Src.v1 = dest;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&Src.v2);
        Src.f2 = 20.0;
        Src.f1 = 1000.0;
        v3 = (tGameObjectMain*)v14->o_send_paf;
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            AI_EvalFunc_MSGSend_C(v3, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
    v15 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)Flic_Tir_ft_explode_SFX_explosion_gao, &pos);
    v11.msgID = 1;
    v11.Int1 = 20;
    v11.f2 = 4.0;
    v11.f1 = 1.0;
    v11.v1.x = 1.0;
    v11.v1.y = 0.0;
    v11.v1.z = 1.0;
    v2 = v15;
    if (v15)
    {
        if ((unsigned int)v15 <= 2)
            v2 = AI_MainActor[(uint32_t)v15 - 1];
        AI_EvalFunc_MSGSend_C(v2, &v11);
    }
    else
    {
        AI_EvalFunc_MSGSend_C(0, &v11);
    }
    AI_EvalFunc_GFXDel_C_CURRENT(v14->GFX_Halo);
    v10 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
    if (v10 == 1000)
        result = 1000;
    else
        result = v10;
    return result;
}

int Flic_Tir_ft_loop()
{
    int result; // eax
    tGameObjectMain* v1; // eax
    tGameObjectMain* v2; // eax
    float v3; // [esp+0h] [ebp-F0h]
    tScriptStruct_FlicTir* v4; // [esp+Ch] [ebp-E4h]
    tGameObjectMain* v5; // [esp+10h] [ebp-E0h]
    tGameObjectMain* v6; // [esp+14h] [ebp-DCh]
    tGameObjectMain* v7; // [esp+18h] [ebp-D8h]
    tGameObjectMain* v8; // [esp+1Ch] [ebp-D4h]
    tGameObjectMain* v9; // [esp+20h] [ebp-D0h]
    tGameObjectMain* v10; // [esp+24h] [ebp-CCh]
    tGameObjectMain* v11; // [esp+28h] [ebp-C8h]
    tGameObjectMain* v12; // [esp+2Ch] [ebp-C4h]
    float v13; // [esp+38h] [ebp-B8h]
    tGameObjectMain* v14; // [esp+54h] [ebp-9Ch]
    Vec3 v15; // [esp+58h] [ebp-98h] BYREF
    Vec3 v16; // [esp+64h] [ebp-8Ch] BYREF
    Vec3 a3; // [esp+70h] [ebp-80h] BYREF
    tGameObjectMain* v18; // [esp+7Ch] [ebp-74h]
    int v19; // [esp+84h] [ebp-6Ch]
    Vec3 a4; // [esp+88h] [ebp-68h] BYREF
    int v22; // [esp+9Ch] [ebp-54h]
    int v23; // [esp+A0h] [ebp-50h]
    Vec3 dest; // [esp+A4h] [ebp-4Ch] BYREF
    Vec3 v27; // [esp+B8h] [ebp-38h] BYREF
    Vec3 a1; // [esp+C8h] [ebp-28h] BYREF
    int v29; // [esp+D4h] [ebp-1Ch]
    tScriptStruct_FlicTir* v30; // [esp+D8h] [ebp-18h]
    Vec3 v31; // [esp+DCh] [ebp-14h] BYREF
    float v32; // [esp+E8h] [ebp-8h]
    int v33; // [esp+ECh] [ebp-4h]

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_FlicTir*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v30 = v4;
    v18 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Flic_Tir_ft_loop_Flic_Tir_ft_explode = (int)AI_ul_RealResolveOneRefFunction(0x3D001159u);
        Flic_Tir_ft_loop_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(&Flic_Tir_ft_loop_GST_Global_gao_f_eau_hauteurZ, (void*)Flic_Tir_ft_loop_GST_Global_gao, 2772);
    }
    if (AI_EvalFunc_TIMEElapsed_C(v30->f_time_live, v30->f_time_max))
    {
        v29 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Flic_Tir_ft_loop_Flic_Tir_ft_explode);
        if (v29 == 1000)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            result = v29;
        }
    }
    else
    {
        v18 = *AI_MainActor;
        v14 = v18;
        if (v18)
        {
            if ((unsigned int)v18 <= 2)
                v14 = AI_MainActor[(uint32_t)v18 - 1];
            AI_EvalFunc_OBJGetPos_C(v14, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v16);
        MATH_SubstractVec(&dest, &a3, &v16);
        a1 = dest;
        MATH_NormalizeEqualVector(&a1);
        v13 = (float)(10 * v30->i_niveau_mechant);
        a1.x = a1.x * v13;
        a1.y = a1.y * v13;
        a1.z = a1.z * v13;
        MATH_VecAddVec(&v31, &v30->v_speed, &a1);
        v19 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v31);
        v27.x = 0.0;
        v27.y = 0.0;
        v27.z = 0.0;
        v32 = TIM_gf_dt;
        v3 = 0.30000001f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlend_C(&v30->v_speed, &v27, v3, &a4);
        v30->v_speed = a4;
        if (!AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x80))
            goto LABEL_29;
        v1 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 0x80);
        v30->o_send_paf = (int)v1;
        v12 = (tGameObjectMain*)v30->o_send_paf;
        if (v12)
        {
            if ((unsigned int)v12 <= 2)
                v12 = AI_MainActor[(uint32_t)v12 - 1];
            v11 = v12;
        }
        else
        {
            v11 = 0;
        }
        v10 = (tGameObjectMain*)v30->o_owner;
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
        if (v11 == v9
            || (v33 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Flic_Tir_ft_loop_Flic_Tir_ft_explode), v33 == 1000))
        {
        LABEL_29:
            if (!AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x40))
                goto LABEL_43;
            v2 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 0x40);
            v30->o_send_paf = (int)v2;
            v8 = (tGameObjectMain*)v30->o_send_paf;
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
            v6 = (tGameObjectMain*)v30->o_owner;
            if (v6)
            {
                if ((unsigned int)v6 <= 2)
                    v6 = AI_MainActor[(uint32_t)v6 - 1];
                v5 = v6;
            }
            else
            {
                v5 = 0;
            }
            if (v7 == v5
                || (v22 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Flic_Tir_ft_loop_Flic_Tir_ft_explode), v22 == 1000))
            {
            LABEL_43:
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v15);
                if (v15.z >= (double)*(float*)Flic_Tir_ft_loop_GST_Global_gao_f_eau_hauteurZ
                    || (v30->o_send_paf = 0,
                        v23 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Flic_Tir_ft_loop_Flic_Tir_ft_explode),
                        v23 == 1000))
                {
                    result = 1000;
                }
                else
                {
                    result = v23;
                }
            }
            else
            {
                result = v22;
            }
        }
        else
        {
            result = v33;
        }
    }
    return result;
}

int Flic_Tir_ft_afterr()
{
    double v0; // st7
    float v2; // [esp+0h] [ebp-34h]
    tScriptStruct_FlicTir* v3; // [esp+4h] [ebp-30h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-28h]
    float v5; // [esp+18h] [ebp-1Ch]
    Vec3 a1; // [esp+1Ch] [ebp-18h] BYREF
    tGameObjectMain* v7; // [esp+28h] [ebp-Ch]
    float v8; // [esp+2Ch] [ebp-8h]
    tScriptStruct_FlicTir* v9; // [esp+30h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_FlicTir*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v9 = v3;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Flic_Tir_ft_afterr_Flic_Tir_ft_explode = (int)AI_ul_RealResolveOneRefFunction(0x3D001159u);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
    AI_EvalFunc_GFXSetv_C_CURRENT(v9->GFX_Halo, 21200, &a1);
    v8 = TIM_gf_dt;
    v2 = TIM_gf_dt * 10.0f;
    AI_EvalFunc_OBJRotateLocalZ_c(AI_gpst_CurrentGameObject, v2);
    v7 = *AI_MainActor;
    v4 = v7;
    if (v7)
    {
        if ((unsigned int)v7 <= 2)
            v4 = AI_MainActor[(uint32_t)v7 - 1];
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v4);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v5 = (float)v0;
    if (v5 < 50.0)
    {
        v9->o_send_paf = (int)*AI_MainActor;
        AI_EvalFunc_AITrackChange_C(0, (tAIFunction*)fct_Flic_Tir_ft_afterr_Flic_Tir_ft_explode);
    }
    return 1000;
}