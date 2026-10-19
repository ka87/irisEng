#include "ai_compiledscripts_inc.h"

int Bison_Fute_Bison_Fute_init_GST_Global_gao;
int Bison_Fute_Bison_Fute_init_GST_Global_gao_f_eau_hauteurZ;
int fct_Bison_Fute_Bison_Fute_init_Bison_Fute_Bison_Fute_comportement;

int Bison_Fute_Bison_Fute_init()
{
    int result; // eax
    tScriptStruct_BisonFute* v1; // [esp+4h] [ebp-48h]
    tGameObjectMain* v2; // [esp+Ch] [ebp-40h]
    tGameObjectMain* v3; // [esp+1Ch] [ebp-30h]
    tGameObjectMain* v4; // [esp+20h] [ebp-2Ch]
    tGameObjectMain* v5; // [esp+24h] [ebp-28h]
    Vec3 v6; // [esp+28h] [ebp-24h] BYREF
    Vec3 a3; // [esp+34h] [ebp-18h] BYREF
    int v8; // [esp+40h] [ebp-Ch]
    tScriptStruct_BisonFute* v9; // [esp+44h] [ebp-8h]
    int i; // [esp+48h] [ebp-4h]

    v6.x = 1.0;
    v6.y = 0.0;
    v6.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_BisonFute*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v9 = v1;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Bison_Fute_Bison_Fute_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Bison_Fute_Bison_Fute_init_GST_Global_gao_f_eau_hauteurZ,
            (void*)Bison_Fute_Bison_Fute_init_GST_Global_gao,
            2772);
        fct_Bison_Fute_Bison_Fute_init_Bison_Fute_Bison_Fute_comportement = (int)AI_ul_RealResolveOneRefFunction(0x7200472Du);
    }
    if (*AI_gppst_CurrentJumpNode)
    {
        *AI_gppst_CurrentJumpNode = 0;
        TIME_Wait_C(0.1f);
        result = 0;
    }
    else
    {
        if (*(float*)Bison_Fute_Bison_Fute_init_GST_Global_gao_f_eau_hauteurZ != -1.0e14f)
        {
            for (i = 0; ; ++i)
            {
                v5 = (tGameObjectMain*)v9->ao_aircraft[i];
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
                if (!v4)
                    break;
                v3 = (tGameObjectMain*)v9->ao_aircraft[i];
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
                v6 = a3;
                v6.z = *(float*)Bison_Fute_Bison_Fute_init_GST_Global_gao_f_eau_hauteurZ;
                v2 = (tGameObjectMain*)v9->ao_aircraft[i];
                if (v2)
                {
                    if ((unsigned int)v2 <= 2)
                        v2 = AI_MainActor[(uint32_t)v2 - 1];
                    AI_EvalFunc_OBJSetPos_C(v2, &v6);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &v6);
                }
            }
        }
        v8 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Bison_Fute_Bison_Fute_init_Bison_Fute_Bison_Fute_comportement);
        if (v8 == 1000)
            result = 1000;
        else
            result = v8;
    }
    return result;
}

int Bison_Fute_Bison_Fute_comportement()
{
    AI_EvalFunc_AITrackCurStop_C();
    return 1000;
}

int Empty_IA_Empty_IA_init() {
    AI_EvalFunc_AITrackCurStop_C();
    return 1000;
}