#include "ai_compiledscripts_inc.h"

//INV_Photo_Loop
int fct_INV_Photo_Loop_INV_Photo_LoopMode1;
int INV_Photo_Loop_univVars;
int INV_Photo_Loop_univVars_i_INV_PhotoDisplayed;

//INV_Photo_LoopMode1
int INV_Photo_LoopMode1_univVars;
int INV_Photo_LoopMode1_univVars_i_INV_PhotoDisplayed;
int fct_INV_Photo_LoopMode1_INV_Photo_Loop;
int fct_INV_Photo_LoopMode1_Global_Library_photo_del;

int INV_Photo_Loop()
{
    int result; // eax
    tScriptStruct_INVPhoto* v1; // [esp+0h] [ebp-2Ch]
    Vec3 v2; // [esp+4h] [ebp-28h] BYREF
    Vec3 a2; // [esp+10h] [ebp-1Ch] BYREF
    Vec3 v4; // [esp+1Ch] [ebp-10h] BYREF
    tScriptStruct_INVPhoto* v5; // [esp+28h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_INVPhoto*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v5 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_INV_Photo_Loop_INV_Photo_LoopMode1 = (int)AI_ul_RealResolveOneRefFunction(0x49018735u);
        INV_Photo_Loop_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&INV_Photo_Loop_univVars_i_INV_PhotoDisplayed, (void*)INV_Photo_Loop_univVars, 7652);
    }
    if (v5->i_Mode)
    {
        if (v5->i_Mode == 1)
        {
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_INV_Photo_Loop_INV_Photo_LoopMode1);
            *(uint32_t*)INV_Photo_Loop_univVars_i_INV_PhotoDisplayed = 1;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            if (v5->i_Mode == 2)
            {
                AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, 1.0);
                v2.x = 0.0;
                v2.y = -1.0;
                v2.z = 0.0;
                v4.x = 0.0;
                v4.y = 0.0;
                v4.z = 1.0;
                AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v2, &v4);
                AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &a2);
                AI_EvalFunc_OBJSetPos_C_CURRENT(&a2);
                v5->i_Mode = 0;
            }
            result = 1000;
        }
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int INV_Photo_LoopMode1()
{
    tScriptStruct_INVPhoto* v1; // [esp+0h] [ebp-80h]
    tGameObjectMain* v2; // [esp+8h] [ebp-78h]
    tGameObjectMain* v3; // [esp+10h] [ebp-70h]
    tGameObjectMain* v4; // [esp+18h] [ebp-68h]
    tGameObjectMain* v5; // [esp+20h] [ebp-60h]
    tGameObjectMain* v6; // [esp+28h] [ebp-58h]
    Vec3 v7; // [esp+2Ch] [ebp-54h] BYREF
    int v8; // [esp+38h] [ebp-48h]
    float v9; // [esp+3Ch] [ebp-44h]
    int v10; // [esp+40h] [ebp-40h]
    int v11; // [esp+44h] [ebp-3Ch]
    float v12; // [esp+48h] [ebp-38h]
    Vec3 a3; // [esp+4Ch] [ebp-34h] BYREF
    Vec3 v14; // [esp+58h] [ebp-28h] BYREF
    Vec3 a2; // [esp+68h] [ebp-18h] BYREF
    tScriptStruct_INVPhoto* v17; // [esp+74h] [ebp-Ch]
    int v18; // [esp+78h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_INVPhoto*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v17 = v1;

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        INV_Photo_LoopMode1_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&INV_Photo_LoopMode1_univVars_i_INV_PhotoDisplayed, (void*)INV_Photo_LoopMode1_univVars, 7652);
        fct_INV_Photo_LoopMode1_INV_Photo_Loop = (int)AI_ul_RealResolveOneRefFunction(0x49018734u);
        fct_INV_Photo_LoopMode1_Global_Library_photo_del = (int)AI_ul_RealResolveOneRefFunction(0x49018A17u);
    }
    if (v17->i_Mode == 3)
    {
        v7.x = 0.04789f - v17->f_offset;
        v7.y = -0.1f;
        v7.z = -0.033f;
        AI_EvalFunc_OBJSetPos_C_CURRENT(&v7);
        AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, 0.030786f);
    }
    else if (v17->i_Mode == 1)
    {
        v10 = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 17, 40);
        AI_EvalFunc_GFXSetf_C_CURRENT(40, 17000, 1.2f);
        a2.x = 0.0;
        a2.y = 1.0;
        a2.z = 0.0;
        v14.x = 0.0;
        v14.y = 0.0;
        v14.z = 1.0;
        AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a2, &v14);
        v17->i_Status = 0;
        v17->f_time = 1.1f;
        v17->i_Mode = 3;
    }
    else
    {
        AI_EvalFunc_GFXDel_C_CURRENT(40);
        v6 = (tGameObjectMain*)v17->o_StringObj;
        if (v6)
        {
            if ((unsigned int)v6 <= 2)
                v6 = AI_MainActor[(uint32_t)v6 - 1];
            STR_SetString(v6, v17->i_String, (char*)AI_gpst_CurrentFunction->localStringConstsData);
        }
        else
        {
            STR_SetString(0, v17->i_String, (char*)AI_gpst_CurrentFunction->localStringConstsData);
        }
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        v5 = (tGameObjectMain*)v17->o_StringObj;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(v5, 1);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
        }
        v17->o_StringObj = 0;
        *(uint32_t*)INV_Photo_LoopMode1_univVars_i_INV_PhotoDisplayed = 0;
        v8 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_INV_Photo_LoopMode1_INV_Photo_Loop);
        if (v8 != 1000)
            return v8;
    }
    if (v17->i_Status)
    {
        if (v17->i_Status == 1)
        {
            if (v17->f_time <= 0.0)
            {
                v17->i_Status = 2;
                v17->f_time = 1.0;
            }
            else
            {
                v17->f_time = v17->f_time - TIM_gf_dt;
            }
        }
        else
        {
            v9 = TIM_gf_dt;
            v17->f_time = v17->f_time - 2.0f * TIM_gf_dt;
            if (v17->f_time <= 0.0)
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
                v17->f_offset = 1.0;
                v17->i_Mode = 0;
            }
            else
            {
                v17->f_offset = 1.0f - v17->f_time;
            }
        }
    }
    else if (v17->f_time <= 0.0)
    {
        v17->f_offset = 0.0;
        v17->i_Status = 1;
        v17->f_time = 5.0f;
    }
    else
    {
        v17->f_offset = v17->f_time;
        v12 = TIM_gf_dt;
        v17->f_time = v17->f_time - 4.0f * TIM_gf_dt;
    }
    if (v17->i_Status == 1 && v17->i_EffacePhoto != -1)
    {
        if (AI_EvalFunc_IoButtonJustPressed_C(7))
        {
            AI2C_PushInt(v17->i_EffacePhoto);
            v18 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_INV_Photo_LoopMode1_Global_Library_photo_del);
            if (v18 != 1000)
                return v18;
            v17->i_EffacePhoto = -1;
            v11 = AI_EvalFunc_OBJ_TextFlagGet_C(AI_gpst_CurrentGameObject, v17->i_String);
            v4 = (tGameObjectMain*)v17->o_StringObj;
            if (v4)
            {
                if ((unsigned int)v4 <= 2)
                    v4 = AI_MainActor[(uint32_t)v4 - 1];
                AI_EvalFunc_OBJ_TextFlagSet_C(v4, v17->i_String, v11 | 0x400);
            }
            else
            {
                AI_EvalFunc_OBJ_TextFlagSet_C(0, v17->i_String, v11 | 0x400);
            }
            v17->i_String = v17->i_StringEfface;
            v3 = (tGameObjectMain*)v17->o_StringObj;
            if (v3)
            {
                if ((unsigned int)v3 <= 2)
                    v3 = AI_MainActor[(uint32_t)v3 - 1];
                AI_EvalFunc_OBJ_TextFlagSet_C(v3, v17->i_String, 16);
            }
            else
            {
                AI_EvalFunc_OBJ_TextFlagSet_C(0, v17->i_String, 16);
            }
            v17->f_time = 2.0;
        }
    }
    a3.x = 0.30000001f;
    a3.y = v17->f_offset * 0.30000001f + 0.80000001f;
    a3.z = 0.0;
    v2 = (tGameObjectMain*)v17->o_StringObj;
    if (v2)
    {
        if ((unsigned int)v2 <= 2)
            v2 = AI_MainActor[(uint32_t)v2 - 1];
        AI_EvalFunc_OBJ_TextPivotSet_C(v2, v17->i_String, &a3);
    }
    else
    {
        AI_EvalFunc_OBJ_TextPivotSet_C(0, v17->i_String, &a3);
    }
    v17->f_offset = -v17->f_offset * v17->des_f_width;
    return 1000;
}