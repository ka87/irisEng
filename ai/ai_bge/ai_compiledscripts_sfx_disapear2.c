#include "ai_compiledscripts_inc.h"

//SFX_Disapear2_loop
int SFX_Disapear2_loop_SFX_WPDisappear_gao;
int SFX_Disapear2_loop_SFX_WPDisappear_gao_i_NbObject;
int SFX_Disapear2_loop_SFX_WPDisappear_gao_ao_Object;
int SFX_Disapear2_loop_SFX_WPDisappear_gao_ai_Flags;
int SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Min;
int SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Max;
int SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Time1;
int SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Time2;
int SFX_Disapear2_loop_SFX_WPDisappear_gao_af_TimeCur;
int SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Scale;
int fct_SFX_Disapear2_loop_SFX_Disapear2_ComputeMatParams;

//SFX_Disapear2_init;
int fct_SFX_Disapear2_init_SFX_Disapear2_loop;

int SFX_Disapear2_loop()
{
    char* v0; // eax
    char* v1; // eax
    char* v2; // eax
    char* v3; // eax
    float v5; // [esp+4h] [ebp-110h]
    float v6; // [esp+8h] [ebp-10Ch]
    float v7; // [esp+Ch] [ebp-108h]
    float v8; // [esp+10h] [ebp-104h]
    float v9; // [esp+14h] [ebp-100h]
    float v10; // [esp+18h] [ebp-FCh]
    char* v11; // [esp+1Ch] [ebp-F8h]
    int* v12; // [esp+20h] [ebp-F4h]
    float v13; // [esp+24h] [ebp-F0h]
    float v14; // [esp+28h] [ebp-ECh]
    float v15; // [esp+2Ch] [ebp-E8h]
    float v16; // [esp+30h] [ebp-E4h]
    tScriptStruct_SFXDisappear2* v17; // [esp+34h] [ebp-E0h]
    tGameObjectMain* v18; // [esp+38h] [ebp-DCh]
    tGameObjectMain* v19; // [esp+3Ch] [ebp-D8h]
    tGameObjectMain* v20; // [esp+44h] [ebp-D0h]
    tGameObjectMain* v21; // [esp+4Ch] [ebp-C8h]
    tGameObjectMain* v22; // [esp+50h] [ebp-C4h]
    tGameObjectMain* v23; // [esp+54h] [ebp-C0h]
    tGameObjectMain* v24; // [esp+5Ch] [ebp-B8h]
    tGameObjectMain* v25; // [esp+64h] [ebp-B0h]
    tGameObjectMain* a2; // [esp+68h] [ebp-ACh]
    tGameObjectMain* v27; // [esp+6Ch] [ebp-A8h]
    tGameObjectMain* v28; // [esp+74h] [ebp-A0h]
    tGameObjectMain* v29; // [esp+7Ch] [ebp-98h]
    tGameObjectMain* v30; // [esp+90h] [ebp-84h]
    tGameObjectMain* v31; // [esp+94h] [ebp-80h]
    int v32; // [esp+98h] [ebp-7Ch]
    int v33; // [esp+A0h] [ebp-74h]
    Vec3 v34; // [esp+A4h] [ebp-70h]
    void* Src; // [esp+B0h] [ebp-64h]
    int v36; // [esp+B4h] [ebp-60h]
    int i; // [esp+B4h] [ebp-60h]
    Vec3 v38; // [esp+BCh] [ebp-58h] BYREF
    tMessage v39; // [esp+C8h] [ebp-4Ch] BYREF
    int j; // [esp+FCh] [ebp-18h]
    Vec3 v41; // [esp+100h] [ebp-14h] BYREF
    tScriptStruct_SFXDisappear2* v42; // [esp+10Ch] [ebp-8h]
    tGameObjectMain* v43; // [esp+110h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v17 = (tScriptStruct_SFXDisappear2*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v17 = 0;
    v42 = v17;
    j = 0;

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SFX_Disapear2_loop_SFX_WPDisappear_gao = AI_ul_RealResolveOneRefObject(0x4902121Eu);
        AI2C_ResolveBuffer(
            &SFX_Disapear2_loop_SFX_WPDisappear_gao_i_NbObject,
            (void*)SFX_Disapear2_loop_SFX_WPDisappear_gao,
            0);
        AI2C_ResolveBuffer(
            &SFX_Disapear2_loop_SFX_WPDisappear_gao_ao_Object,
            (void*)SFX_Disapear2_loop_SFX_WPDisappear_gao,
            8);
        AI2C_ResolveBuffer(
            &SFX_Disapear2_loop_SFX_WPDisappear_gao_ai_Flags,
            (void*)SFX_Disapear2_loop_SFX_WPDisappear_gao,
            44);
        AI2C_ResolveBuffer(
            &SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Min,
            (void*)SFX_Disapear2_loop_SFX_WPDisappear_gao,
            80);
        AI2C_ResolveBuffer(
            &SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Max,
            (void*)SFX_Disapear2_loop_SFX_WPDisappear_gao,
            116);
        AI2C_ResolveBuffer(
            &SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Time1,
            (void*)SFX_Disapear2_loop_SFX_WPDisappear_gao,
            152);
        AI2C_ResolveBuffer(
            &SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Time2,
            (void*)SFX_Disapear2_loop_SFX_WPDisappear_gao,
            188);
        AI2C_ResolveBuffer(
            &SFX_Disapear2_loop_SFX_WPDisappear_gao_af_TimeCur,
            (void*)SFX_Disapear2_loop_SFX_WPDisappear_gao,
            224);
        AI2C_ResolveBuffer(
            &SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Scale,
            (void*)SFX_Disapear2_loop_SFX_WPDisappear_gao,
            296);
        fct_SFX_Disapear2_loop_SFX_Disapear2_ComputeMatParams = (int)AI_ul_RealResolveOneRefFunction(0x4902122Eu);
    }
    Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v39, Src, sizeof(v39));
    if (v39.msgID)
    {
        v31 = (tGameObjectMain*)v39.gao2;
        if (v39.gao2)
        {
            if ((unsigned int)v39.gao2 <= 2)
                v31 = AI_MainActor[(unsigned int)v39.gao2 - 1];
            v30 = v31;
        }
        else
        {
            v30 = 0;
        }
        if (v30 && (v39.msgID == 1 || v39.msgID == 2))
        {
            v34 = v39.v2;
            v36 = *(uint32_t*)SFX_Disapear2_loop_SFX_WPDisappear_gao_i_NbObject;
            if (*(int*)SFX_Disapear2_loop_SFX_WPDisappear_gao_i_NbObject < 8)
            {
                *(uint32_t*)(SFX_Disapear2_loop_SFX_WPDisappear_gao_ao_Object + 4 * v36 + 4) = (uint32_t)v39.gao2;
                *(uint32_t*)(SFX_Disapear2_loop_SFX_WPDisappear_gao_ai_Flags + 4 * v36 + 4) = v39.Int1;
                v0 = AI_EvalVarArray_C2((char*)SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Min, v36);
                *(float*)v0 = v39.f2;
                v1 = AI_EvalVarArray_C2((char*)SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Max, v36);
                *(float*)v1 = v39.f1;
                *(float*)AI_EvalVarArray_C2((char*)SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Time1, v36) = v34.x;
                *(float*)AI_EvalVarArray_C2((char*)SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Time2, v36) = v34.y;
                v16 = v34.x + v34.y;
                *(float*)AI_EvalVarArray_C2((char*)SFX_Disapear2_loop_SFX_WPDisappear_gao_af_TimeCur, v36) = v16;
                if (v34.z == 0.0)
                    *(uint32_t*)AI_EvalVarArray_C2((char*)SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Scale, v36) = 0x3F800000;
                else
                    *(float*)AI_EvalVarArray_C2((char*)SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Scale, v36) = v34.z;
                ++* (uint32_t*)SFX_Disapear2_loop_SFX_WPDisappear_gao_i_NbObject;
            }
        }
    }
    v42->i_CurObj = 0;
LABEL_21:
    while (v42->i_CurObj < v42->i_NbObject)
    {
        if ((v42->ai_Flags[v42->i_CurObj] & 1) != 0)
        {
            v11 = AI_EvalVarArray_C2((char*)&v42->__af_TimeCur_Dimension1, v42->i_CurObj);
            *(float*)v11 = *(float*)v11 - TIM_gf_dt;
            if (*(float*)AI_EvalVarArray_C2((char*)&v42->__af_TimeCur_Dimension1, v42->i_CurObj) > 0.0)
            {
                v33 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_SFX_Disapear2_loop_SFX_Disapear2_ComputeMatParams);
                if (v33 != 1000)
                    return v33;
                ++v42->i_CurObj;
            }
            else
            {
                if ((v42->ai_Flags[v42->i_CurObj] & 2) == 0)
                {
                    v24 = (tGameObjectMain*)v42->ao_Object[v42->i_CurObj];
                    if (v24)
                    {
                        if ((unsigned int)v24 <= 2)
                            v24 = AI_MainActor[(uint32_t)v24 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v24, 1);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
                    }
                }
                v43 = AI_gpst_CurrentGameObject;
                v23 = AI_gpst_CurrentGameObject;
                if (AI_gpst_CurrentGameObject)
                {
                    if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                        v23 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                    v22 = v23;
                }
                else
                {
                    v22 = 0;
                }
                v21 = (tGameObjectMain*)v42->ao_Object[v42->i_CurObj];
                if (v21)
                {
                    if ((unsigned int)v21 <= 2)
                        v21 = AI_MainActor[(uint32_t)v21 - 1];
                    AI_EvalFunc_OBJ_AddMaterial_C(v21, v22, -1);
                }
                else
                {
                    AI_EvalFunc_OBJ_AddMaterial_C(0, v22, -1);
                }
                v20 = (tGameObjectMain*)v42->ao_Object[v42->i_CurObj];
                if (v20)
                {
                    if ((unsigned int)v20 <= 2)
                        v20 = AI_MainActor[(uint32_t)v20 - 1];
                    AI_EvalFunc_OBJDrawMaskSet_C(v20, 0x20100000, 0);
                }
                else
                {
                    AI_EvalFunc_OBJDrawMaskSet_C(0, 0x20100000, 0);
                }
                for (i = 0; i < *(int32_t*)SFX_Disapear2_loop_SFX_WPDisappear_gao_i_NbObject; ++i)
                {
                    v19 = (tGameObjectMain*)v42->ao_Object[v42->i_CurObj];
                    if (v19)
                    {
                        if ((unsigned int)v19 <= 2)
                            v19 = AI_MainActor[(uint32_t)v19 - 1];
                        v18 = v19;
                    }
                    else
                    {
                        v18 = 0;
                    }
                    if (*(tGameObjectMain**)(SFX_Disapear2_loop_SFX_WPDisappear_gao_ao_Object + 4 * i + 4) == v18)
                    {
                        for (j = i + 1; j < *(int32_t*)SFX_Disapear2_loop_SFX_WPDisappear_gao_i_NbObject; ++j)
                        {
                            *(uint32_t*)(SFX_Disapear2_loop_SFX_WPDisappear_gao_ao_Object + 4 * j) = *(uint32_t*)(SFX_Disapear2_loop_SFX_WPDisappear_gao_ao_Object
                                + 4 * j
                                + 4);
                            *(uint32_t*)(SFX_Disapear2_loop_SFX_WPDisappear_gao_ai_Flags + 4 * j) = *(uint32_t*)(SFX_Disapear2_loop_SFX_WPDisappear_gao_ai_Flags
                                + 4 * j
                                + 4);
                            v10 = *(float*)AI_EvalVarArray_C2((char*)SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Min, j);
                            *(float*)AI_EvalVarArray_C2((char*)SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Min, j - 1) = v10;
                            v9 = *(float*)AI_EvalVarArray_C2((char*)SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Max, j);
                            *(float*)AI_EvalVarArray_C2((char*)SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Max, j - 1) = v9;
                            v8 = *(float*)AI_EvalVarArray_C2((char*)SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Time1, j);
                            *(float*)AI_EvalVarArray_C2((char*)SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Time1, j - 1) = v8;
                            v7 = *(float*)AI_EvalVarArray_C2((char*)SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Time2, j);
                            *(float*)AI_EvalVarArray_C2((char*)SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Time2, j - 1) = v7;
                            v6 = *(float*)AI_EvalVarArray_C2((char*)SFX_Disapear2_loop_SFX_WPDisappear_gao_af_TimeCur, j);
                            *(float*)AI_EvalVarArray_C2((char*)SFX_Disapear2_loop_SFX_WPDisappear_gao_af_TimeCur, j - 1) = v6;
                            v5 = *(float*)AI_EvalVarArray_C2((char*)SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Scale, j);
                            *(float*)AI_EvalVarArray_C2((char*)SFX_Disapear2_loop_SFX_WPDisappear_gao_af_Scale, j - 1) = v5;
                        }
                        --* (uint32_t*)SFX_Disapear2_loop_SFX_WPDisappear_gao_i_NbObject;
                        *(uint32_t*)(SFX_Disapear2_loop_SFX_WPDisappear_gao_ao_Object
                            + 4 * *(uint32_t*)SFX_Disapear2_loop_SFX_WPDisappear_gao_i_NbObject
                            + 4) = 0;
                        *(uint32_t*)(SFX_Disapear2_loop_SFX_WPDisappear_gao_ai_Flags
                            + 4 * *(uint32_t*)SFX_Disapear2_loop_SFX_WPDisappear_gao_i_NbObject
                            + 4) = 0;
                        goto LABEL_21;
                    }
                }
            }
        }
        else
        {
            v15 = *(float*)AI_EvalVarArray_C2((char*)&v42->__af_Min_Dimension1, v42->i_CurObj);
            if (*(float*)AI_EvalVarArray_C2((char*)&v42->__af_Max_Dimension1, v42->i_CurObj) == v15)
            {
                v29 = (tGameObjectMain*)v42->ao_Object[v42->i_CurObj];
                if (v29)
                {
                    if ((unsigned int)v29 <= 2)
                        v29 = AI_MainActor[(uint32_t)v29 - 1];
                    AI_EvalFunc_BVMinGet_C(v29, &v41);
                }
                else
                {
                    AI_EvalFunc_BVMinGet_C(0, &v41);
                }
                v2 = AI_EvalVarArray_C2((char*)&v42->__af_Min_Dimension1, v42->i_CurObj);
                *(float*)v2 = v41.z;
                v28 = (tGameObjectMain*)v42->ao_Object[v42->i_CurObj];
                if (v28)
                {
                    if ((unsigned int)v28 <= 2)
                        v28 = AI_MainActor[(uint32_t)v28 - 1];
                    AI_EvalFunc_BVMaxGet_C(v28, &v38);
                }
                else
                {
                    AI_EvalFunc_BVMaxGet_C(0, &v38);
                }
                v3 = AI_EvalVarArray_C2((char*)&v42->__af_Max_Dimension1, v42->i_CurObj);
                *(float*)v3 = v38.z;
            }
            v14 = *(float*)AI_EvalVarArray_C2((char*)&v42->__af_Time1_Dimension1, v42->i_CurObj);
            v13 = *(float*)AI_EvalVarArray_C2((char*)&v42->__af_Time2_Dimension1, v42->i_CurObj) + v14;
            *(float*)AI_EvalVarArray_C2((char*)&v42->__af_TimeCur_Dimension1, v42->i_CurObj) = v13;
            v12 = &v42->ai_Flags[v42->i_CurObj];
            *v12 |= 1u;
            v27 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v27 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                a2 = v27;
            }
            else
            {
                a2 = 0;
            }
            v25 = (tGameObjectMain*)v42->ao_Object[v42->i_CurObj];
            if (v25)
            {
                if ((unsigned int)v25 <= 2)
                    v25 = AI_MainActor[(uint32_t)v25 - 1];
                AI_EvalFunc_OBJ_AddMaterial_C(v25, a2, v42->i_CurObj);
            }
            else
            {
                AI_EvalFunc_OBJ_AddMaterial_C(0, a2, v42->i_CurObj);
            }
            v32 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_SFX_Disapear2_loop_SFX_Disapear2_ComputeMatParams);
            if (v32 != 1000)
                return v32;
            ++v42->i_CurObj;
        }
    }
    return 1000;
}

int fct_SFX_Disapear2_init_SFX_Disapear2_loop;

int SFX_Disapear2_init()
{
    tScriptStruct_SFXDisappear2* v1; // [esp+0h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXDisappear2*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_SFX_Disapear2_init_SFX_Disapear2_loop = AI_ul_RealResolveOneRefFunction(0x4902121Cu);
    }
    v1->i_NbObject = 0;
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_SFX_Disapear2_init_SFX_Disapear2_loop);
    return 1000;
}

int SFX_Disapear2_ComputeMatParams()
{
    float X; // [esp+0h] [ebp-74h]
    float Xa; // [esp+0h] [ebp-74h]
    float Xb; // [esp+0h] [ebp-74h]
    float Xc; // [esp+0h] [ebp-74h]
    float Xd; // [esp+0h] [ebp-74h]
    float v6; // [esp+4h] [ebp-70h]
    float v7; // [esp+8h] [ebp-6Ch]
    tScriptStruct_SFXDisappear2* v8; // [esp+Ch] [ebp-68h]
    tGameObjectMain* v9; // [esp+14h] [ebp-60h]
    tGameObjectMain* v10; // [esp+1Ch] [ebp-58h]
    tGameObjectMain* v11; // [esp+24h] [ebp-50h]
    tGameObjectMain* v12; // [esp+2Ch] [ebp-48h]
    tGameObjectMain* v13; // [esp+34h] [ebp-40h]
    tGameObjectMain* v14; // [esp+3Ch] [ebp-38h]
    float v15; // [esp+40h] [ebp-34h]
    float v16; // [esp+40h] [ebp-34h]
    float v17; // [esp+40h] [ebp-34h]
    float v18; // [esp+40h] [ebp-34h]
    float a5; // [esp+44h] [ebp-30h]
    float v20; // [esp+48h] [ebp-2Ch]
    float v21; // [esp+48h] [ebp-2Ch]
    Vec3 a3; // [esp+4Ch] [ebp-28h] BYREF
    float v23; // [esp+58h] [ebp-1Ch]
    float v24; // [esp+5Ch] [ebp-18h]
    float v25; // [esp+60h] [ebp-14h]
    tScriptStruct_SFXDisappear2* v26; // [esp+64h] [ebp-10h]
    float v27; // [esp+68h] [ebp-Ch]
    float v28; // [esp+6Ch] [ebp-8h]
    int v29; // [esp+70h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v8 = (tScriptStruct_SFXDisappear2*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v8 = 0;
    v26 = v8;
    v27 = 0.0;
    v28 = 0.0;
    v29 = 0;
    v24 = 0.0;
    if ((v8->ai_Flags[v8->i_CurObj] & 0x10) != 0)
    {
        AI_EvalFunc_SetFlag_C(AI_gpst_CurrentGameObject, v26->i_CurObj, 1000, 0, 64);
        v20 = 0.0;
    }
    else
    {
        AI_EvalFunc_SetFlag_C(AI_gpst_CurrentGameObject, v26->i_CurObj, 1000, 64, 0);
        v14 = (tGameObjectMain*)v26->ao_Object[v26->i_CurObj];
        if (v14)
        {
            if ((unsigned int)v14 <= 2)
                v14 = AI_MainActor[(uint32_t)v14 - 1];
            AI_EvalFunc_OBJGetPos_C(v14, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        v20 = a3.z;
    }
    v24 = *(float*)AI_EvalVarArray_C2((char*)&v26->__af_Max_Dimension1, v26->i_CurObj) + v20;
    v21 = *(float*)AI_EvalVarArray_C2((char*)&v26->__af_Min_Dimension1, v26->i_CurObj) + v20;
    v29 = 0;
    a5 = 0.0;
    v7 = *(float*)AI_EvalVarArray_C2((char*)&v26->__af_TimeCur_Dimension1, v26->i_CurObj);
    if (*(float*)AI_EvalVarArray_C2((char*)&v26->__af_Time2_Dimension1, v26->i_CurObj) >= (double)v7)
    {
        v17 = *(float*)AI_EvalVarArray_C2((char*)&v26->__af_TimeCur_Dimension1, v26->i_CurObj);
        v16 = v17 / *(float*)AI_EvalVarArray_C2((char*)&v26->__af_Time2_Dimension1, v26->i_CurObj);
        if ((v26->ai_Flags[v26->i_CurObj] & 0x8000) != 0)
        {
            a5 = 0.5f;
            v16 = 1.0f - v16;
        }
        if ((v26->ai_Flags[v26->i_CurObj] & 2) != 0)
        {
            v11 = (tGameObjectMain*)v26->ao_Object[v26->i_CurObj];
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v11 = AI_MainActor[(uint32_t)v11 - 1];
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(v11, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
            }
            v10 = (tGameObjectMain*)v26->ao_Object[v26->i_CurObj];
            if (v10)
            {
                if ((unsigned int)v10 <= 2)
                    v10 = AI_MainActor[(uint32_t)v10 - 1];
                AI_EvalFunc_OBJDrawMaskSet_C(v10, 0x20100000, 0);
            }
            else
            {
                AI_EvalFunc_OBJDrawMaskSet_C(0, 0x20100000, 0);
            }
        }
        else
        {
            v9 = (tGameObjectMain*)v26->ao_Object[v26->i_CurObj];
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                AI_EvalFunc_OBJDrawMaskSet_C(v9, 0, 0x20100000);
            }
            else
            {
                AI_EvalFunc_OBJDrawMaskSet_C(0, 0, 0x20100000);
            }
            v29 = 1;
        }
    }
    else
    {
        v6 = *(float*)AI_EvalVarArray_C2((char*)&v26->__af_TimeCur_Dimension1, v26->i_CurObj);
        v15 = v6 - *(float*)AI_EvalVarArray_C2((char*)&v26->__af_Time2_Dimension1, v26->i_CurObj);
        v16 = v15 / *(float*)AI_EvalVarArray_C2((char*)&v26->__af_Time1_Dimension1, v26->i_CurObj);
        if ((v26->ai_Flags[v26->i_CurObj] & 0x800) != 0)
            a5 = 0.5f;
        else
            v16 = 1.0f - v16;
        if ((v26->ai_Flags[v26->i_CurObj] & 2) != 0)
        {
            v13 = (tGameObjectMain*)v26->ao_Object[v26->i_CurObj];
            if (v13)
            {
                if ((unsigned int)v13 <= 2)
                    v13 = AI_MainActor[(uint32_t)v13 - 1];
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(v13, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
            }
            v12 = (tGameObjectMain*)v26->ao_Object[v26->i_CurObj];
            if (v12)
            {
                if ((unsigned int)v12 <= 2)
                    v12 = AI_MainActor[(uint32_t)v12 - 1];
                AI_EvalFunc_OBJDrawMaskSet_C(v12, 0, 0x20100000);
            }
            else
            {
                AI_EvalFunc_OBJDrawMaskSet_C(0, 0, 0x20100000);
            }
            v29 = 1;
        }
    }
    v18 = (v21 - v24) * v16 + v24;
    *(float*)AI_EvalVarArray_C2((char*)&v26->__af_ZCur_Dimension1, v26->i_CurObj) = v18;
    if (a5 == 0.0)
        v18 = -v18;
    AI_EvalFunc_SetUV_C(AI_gpst_CurrentGameObject, v26->i_CurObj, 0, 4, a5);
    AI_EvalFunc_SetUV_C(AI_gpst_CurrentGameObject, v26->i_CurObj, 0, 1, v18);
    X = *(float*)AI_EvalVarArray_C2((char*)&v26->__af_TimeCur_Dimension1, v26->i_CurObj) * 3.0f;
    v25 = sinf(X);
    v27 = 0.2f * v25;
    Xa = *(float*)AI_EvalVarArray_C2((char*)&v26->__af_TimeCur_Dimension1, v26->i_CurObj) * 3.0f;
    v23 = cosf(Xa);
    v28 = 0.2f * v23;
    AI_EvalFunc_SetUV_C(AI_gpst_CurrentGameObject, v26->i_CurObj, 2, 0, v27);
    AI_EvalFunc_SetUV_C(AI_gpst_CurrentGameObject, v26->i_CurObj, 2, 1, v28);
    Xb = v27 / 3.0f;
    AI_EvalFunc_SetUV_C(AI_gpst_CurrentGameObject, v26->i_CurObj, 3, 0, Xb);
    Xc = v28 / 3.0f;
    AI_EvalFunc_SetUV_C(AI_gpst_CurrentGameObject, v26->i_CurObj, 3, 1, Xc);
    Xd = *(float*)AI_EvalVarArray_C2((char*)&v26->__af_Scale_Dimension1, v26->i_CurObj) * 3.5999999f;
    AI_EvalFunc_SetUV_C(AI_gpst_CurrentGameObject, v26->i_CurObj, 1, 3, Xd);
    v27 = *(float*)AI_EvalVarArray_C2((char*)&v26->__af_Scale_Dimension1, v26->i_CurObj) * 1.1799999f;
    AI_EvalFunc_SetUV_C(AI_gpst_CurrentGameObject, v26->i_CurObj, 2, 2, v27);
    AI_EvalFunc_SetUV_C(AI_gpst_CurrentGameObject, v26->i_CurObj, 2, 3, v27);
    v27 = *(float*)AI_EvalVarArray_C2((char*)&v26->__af_Scale_Dimension1, v26->i_CurObj) * 1.29f;
    AI_EvalFunc_SetUV_C(AI_gpst_CurrentGameObject, v26->i_CurObj, 3, 2, v27);
    AI_EvalFunc_SetUV_C(AI_gpst_CurrentGameObject, v26->i_CurObj, 3, 3, v27);
    return 1000;
}