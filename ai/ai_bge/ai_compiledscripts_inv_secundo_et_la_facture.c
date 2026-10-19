#include "ai_compiledscripts_inc.h"

//INV_SecundoEtLaFacture_firstinit
int INV_SecundoEtLaFacture_firstinit_INV_manager_gao;
int INV_SecundoEtLaFacture_firstinit_INV_manager_gao_o_ModeEmpty_Obj;
int INV_SecundoEtLaFacture_firstinit_INV_manager_gao_i_ModeEmpty_Flags;
int fct_INV_SecundoEtLaFacture_firstinit_INV_manager_gao_INV_SecundoEtLaFacture_init;

//INV_SecundoEtLaFacture_init
int INV_SecundoEtLaFacture_init_INV_manager_gao;
int INV_SecundoEtLaFacture_init_INV_manager_gao_o_PhotoText;
int fct_INV_SecundoEtLaFacture_init_INV_SecundoEtLaFacture_loop;

//INV_SecundoEtLaFacture_loop
int INV_SecundoEtLaFacture_loop_INV_manager_gao;
int INV_SecundoEtLaFacture_loop_INV_manager_gao_i_SmallTransfert;
int INV_SecundoEtLaFacture_loop_INV_manager_gao_i_Fric2Add;
int INV_SecundoEtLaFacture_loop_INV_manager_gao_i_FricWithPerso;
int INV_SecundoEtLaFacture_loop_INV_manager_gao_f_PogneAppearTimeInit;
int INV_SecundoEtLaFacture_loop_INV_manager_gao_f_PogneAppearTimeFixed;
int INV_SecundoEtLaFacture_loop_INV_manager_gao_f_PogneDelay;
int INV_SecundoEtLaFacture_loop_INV_manager_gao_f_Timer;
int INV_SecundoEtLaFacture_loop_INV_manager_gao_f_PogneAppearTime;
int INV_SecundoEtLaFacture_loop_INV_manager_gao_i_SndPognes;
int INV_SecundoEtLaFacture_loop_univVars;
int INV_SecundoEtLaFacture_loop_univVars_f_sally_fric;
int INV_SecundoEtLaFacture_loop_INV_manager_gao_i_ModeEmpty_Flags;
int INV_SecundoEtLaFacture_loop_INV_manager_gao_i_Locked;
int INV_SecundoEtLaFacture_loop_INV_manager_gao_i_AskExit;
int INV_SecundoEtLaFacture_loop_univVars_Flag_speciaux_sauve;

int INV_SecundoEtLaFacture_firstinit()
{
    tScriptStruct_INVSecundoEtLaFacture* v1; // [esp+0h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_INVSecundoEtLaFacture*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        INV_SecundoEtLaFacture_firstinit_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &INV_SecundoEtLaFacture_firstinit_INV_manager_gao_o_ModeEmpty_Obj,
            (void*)INV_SecundoEtLaFacture_firstinit_INV_manager_gao,
            23352);
        AI2C_ResolveBuffer(
            &INV_SecundoEtLaFacture_firstinit_INV_manager_gao_i_ModeEmpty_Flags,
            (void*)INV_SecundoEtLaFacture_firstinit_INV_manager_gao,
            23356);
        fct_INV_SecundoEtLaFacture_firstinit_INV_manager_gao_INV_SecundoEtLaFacture_init = (int)AI_ul_RealResolveOneRefFunction(0x4901FC14u);
    }
    v1->i_State = 0;
    *(uint32_t*)INV_SecundoEtLaFacture_firstinit_INV_manager_gao_o_ModeEmpty_Obj = (uint32_t)AI_gpst_CurrentGameObject;
    *(uint32_t*)INV_SecundoEtLaFacture_firstinit_INV_manager_gao_i_ModeEmpty_Flags = 1;
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_INV_SecundoEtLaFacture_firstinit_INV_manager_gao_INV_SecundoEtLaFacture_init);
    AI_EvalFunc_OBJ_FlagInactiveSet_C(AI_gpst_CurrentGameObject, 1);

    //FIXME TEST
    int invmanager = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
    int phototext;
    AI2C_ResolveBuffer(&phototext, (void*)invmanager, 48);
    DBG_OutputDebugString("INV_SecundoEtLaFacture_firstinit: manager: %p, phototext: %p\n", invmanager, phototext);

    return 1000;
}

int INV_SecundoEtLaFacture_init()
{
    int result; // eax
    unsigned int v1; // eax
    char* v2; // eax
    int v3; // eax
    float v4; // [esp+0h] [ebp-1A0h]
    float v5; // [esp+4h] [ebp-19Ch]
    tScriptStruct_INVSecundoEtLaFacture* v6; // [esp+Ch] [ebp-194h]
    tGameObjectMain* v7; // [esp+14h] [ebp-18Ch]
    tGameObjectMain* v8; // [esp+1Ch] [ebp-184h]
    tGameObjectMain* v9; // [esp+24h] [ebp-17Ch]
    tGameObjectMain* v10; // [esp+2Ch] [ebp-174h]
    tGameObjectMain* v11; // [esp+34h] [ebp-16Ch]
    tGameObjectMain* v12; // [esp+3Ch] [ebp-164h]
    tGameObjectMain* v13; // [esp+44h] [ebp-15Ch]
    tGameObjectMain* v14; // [esp+48h] [ebp-158h]
    tGameObjectMain* v15; // [esp+4Ch] [ebp-154h]
    tGameObjectMain* v16; // [esp+54h] [ebp-14Ch]
    tGameObjectMain* v17; // [esp+5Ch] [ebp-144h]
    tGameObjectMain* v18; // [esp+64h] [ebp-13Ch]
    tGameObjectMain* v19; // [esp+6Ch] [ebp-134h]
    tGameObjectMain* v20; // [esp+74h] [ebp-12Ch]
    tGameObjectMain* v21; // [esp+7Ch] [ebp-124h]
    tGameObjectMain* v22; // [esp+8Ch] [ebp-114h]
    tGameObjectMain* v23; // [esp+94h] [ebp-10Ch]
    tGameObjectMain* v24; // [esp+9Ch] [ebp-104h]
    tGameObjectMain* v25; // [esp+A0h] [ebp-100h]
    tGameObjectMain* v26; // [esp+A4h] [ebp-FCh]
    tGameObjectMain* v27; // [esp+ACh] [ebp-F4h]
    tGameObjectMain* v28; // [esp+B4h] [ebp-ECh]
    tGameObjectMain* v29; // [esp+BCh] [ebp-E4h]
    tGameObjectMain* v30; // [esp+C4h] [ebp-DCh]
    tGameObjectMain* v31; // [esp+CCh] [ebp-D4h]
    tGameObjectMain* v32; // [esp+D0h] [ebp-D0h]
    tGameObjectMain* v33; // [esp+D4h] [ebp-CCh]
    Vec3 v34; // [esp+D8h] [ebp-C8h] BYREF
    Vec3 v35; // [esp+E4h] [ebp-BCh] BYREF
    Vec3 a3; // [esp+F0h] [ebp-B0h] BYREF
    float a4; // [esp+FCh] [ebp-A4h]
    float v38; // [esp+100h] [ebp-A0h]
    tTextGet v39; // [esp+104h] [ebp-9Ch] BYREF
    float v40; // [esp+10Ch] [ebp-94h]
    float v41; // [esp+110h] [ebp-90h]
    float v42; // [esp+114h] [ebp-8Ch]
    Vec3 v43; // [esp+118h] [ebp-88h] BYREF
    tTextGet Src; // [esp+124h] [ebp-7Ch] BYREF
    Vec3 v45; // [esp+130h] [ebp-70h] BYREF
    Vec3 a5; // [esp+13Ch] [ebp-64h] BYREF
    float v47; // [esp+14Ch] [ebp-54h]
    float v48; // [esp+150h] [ebp-50h]
    float v49; // [esp+154h] [ebp-4Ch]
    float v50; // [esp+158h] [ebp-48h]
    int v51; // [esp+15Ch] [ebp-44h]
    Vec3 v52; // [esp+160h] [ebp-40h] BYREF
    int soundInstanceID; // [esp+170h] [ebp-30h]
    tGameObjectMain* v54; // [esp+174h] [ebp-2Ch]
    unsigned int v55; // [esp+178h] [ebp-28h]
    tScriptStruct_INVSecundoEtLaFacture* v56; // [esp+17Ch] [ebp-24h]
    int v57; // [esp+180h] [ebp-20h]
    float v58; // [esp+184h] [ebp-1Ch]
    float v59; // [esp+188h] [ebp-18h]
    float v60; // [esp+18Ch] [ebp-14h]
    Vec3 v61; // [esp+190h] [ebp-10h] BYREF
    float v62; // [esp+19Ch] [ebp-4h]

    v45.x = 1.0;
    v45.y = 0.0;
    v45.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_INVSecundoEtLaFacture*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v56 = v6;
    soundInstanceID = 0;
    a4 = 0.0;
    v57 = 0;
    v54 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        INV_SecundoEtLaFacture_init_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &INV_SecundoEtLaFacture_init_INV_manager_gao_o_PhotoText,
            (void*)INV_SecundoEtLaFacture_init_INV_manager_gao,
            48);
        fct_INV_SecundoEtLaFacture_init_INV_SecundoEtLaFacture_loop = (int)AI_ul_RealResolveOneRefFunction(0x4901FC1Cu);
    }
    if (*AI_gppst_CurrentJumpNode)
    {
        AI_EvalFunc_SNDPrefetchModifier_C(AI_gpst_CurrentGameObject, 0);
        *AI_gppst_CurrentJumpNode = 0;
        TIME_Wait_C(0.5);
        result = 0;
    }
    else
    {
        v33 = (tGameObjectMain*)v56->o_Secundo;
        if (v33)
        {
            if ((unsigned int)v33 <= 2)
                v33 = AI_MainActor[(uint32_t)v33 - 1];
            v32 = v33;
        }
        else
        {
            v32 = 0;
        }
        if (v32)
        {
            v31 = (tGameObjectMain*)v56->o_Secundo;
            if (v31)
            {
                if ((unsigned int)v31 <= 2)
                    v31 = AI_MainActor[(uint32_t)v31 - 1];
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(v31, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
            }
            v30 = (tGameObjectMain*)v56->o_Secundo;
            if (v30)
            {
                if ((unsigned int)v30 <= 2)
                    v30 = AI_MainActor[(uint32_t)v30 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v30, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
            }
            v29 = (tGameObjectMain*)v56->o_Secundo;
            if (v29)
            {
                if ((unsigned int)v29 <= 2)
                    v29 = AI_MainActor[(uint32_t)v29 - 1];
                AI_EvalFunc_OBJ_ZoomSet_C(v29, 1.0);
            }
            else
            {
                AI_EvalFunc_OBJ_ZoomSet_C(0, 1.0);
            }
            v43.x = 0.0;
            v43.y = -1.0;
            v43.z = 0.0;
            v28 = (tGameObjectMain*)v56->o_Secundo;
            if (v28)
            {
                if ((unsigned int)v28 <= 2)
                    v28 = AI_MainActor[(uint32_t)v28 - 1];
                AI_EvalFunc_OBJ_SightSet_C(v28, &v43);
            }
            else
            {
                AI_EvalFunc_OBJ_SightSet_C(0, &v43);
            }
            v27 = (tGameObjectMain*)v56->o_Secundo;
            if (v27)
            {
                if ((unsigned int)v27 <= 2)
                    v27 = AI_MainActor[(uint32_t)v27 - 1];
                AI_EvalFunc_ACTActionSet_C(v27, 0);
            }
            else
            {
                AI_EvalFunc_ACTActionSet_C(0, 0);
            }
            v26 = (tGameObjectMain*)v56->o_SecundoFX;
            if (v26)
            {
                if ((unsigned int)v26 <= 2)
                    v26 = AI_MainActor[(uint32_t)v26 - 1];
                v25 = v26;
            }
            else
            {
                v25 = 0;
            }
            if (v25)
            {
                v24 = (tGameObjectMain*)v56->o_SecundoFX;
                if (v24)
                {
                    if ((unsigned int)v24 <= 2)
                        v24 = AI_MainActor[(uint32_t)v24 - 1];
                    v1 = AI_EvalFunc_OBJ_FlagsControlSet_C(v24, 0, 0x4000000);
                }
                else
                {
                    v1 = AI_EvalFunc_OBJ_FlagsControlSet_C(0, 0, 0x4000000);
                }
                v55 = v1;
                v23 = (tGameObjectMain*)v56->o_SecundoFX;
                if (v23)
                {
                    if ((unsigned int)v23 <= 2)
                        v23 = AI_MainActor[(uint32_t)v23 - 1];
                    AI_EvalFunc_OBJ_CapaSet_C(v23, 0, 0x100);
                }
                else
                {
                    AI_EvalFunc_OBJ_CapaSet_C(0, 0, 0x100);
                }
            }
            v54 = AI_EvalFunc_VIEWGetObject_C(0);
            v22 = v54;
            if (v54)
            {
                if ((unsigned int)v54 <= 2)
                    v22 = AI_MainActor[(uint32_t)v54 - 1];
                AI_EvalFunc_OBJGetPos_C(v22, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            v45 = a3;
            v21 = v54;
            if (v54)
            {
                if ((unsigned int)v54 <= 2)
                    v21 = AI_MainActor[(uint32_t)v54 - 1];
                AI_EvalFunc_OBJ_HorizonGet_C(v21, &v35);
            }
            else
            {
                AI_EvalFunc_OBJ_HorizonGet_C(0, &v35);
            }
            v47 = v35.x * 0.2f;
            v48 = v35.y * 0.2f;
            v49 = v35.z * 0.2f;
            v45.x = v45.x + v47;
            v45.y = v45.y + v48;
            v45.z = v45.z + v49;
            v20 = v54;
            if (v54)
            {
                if ((unsigned int)v54 <= 2)
                    v20 = AI_MainActor[(uint32_t)v54 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v20, &v61);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v61);
            }
            v40 = v61.x * 1.0f;
            v41 = v61.y * 1.0f;
            v42 = v61.z * 1.0f;
            v45.x = v45.x + v40;
            v45.y = v45.y + v41;
            v45.z = v45.z + v42;
            v19 = v54;
            if (v54)
            {
                if ((unsigned int)v54 <= 2)
                    v19 = AI_MainActor[(uint32_t)v54 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v19, &v52);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v52);
            }
            v58 = v52.x * -1.4f;
            v59 = v52.y * -1.4f;
            v60 = v52.z * -1.4f;
            v45.x = v45.x + v58;
            v45.y = v45.y + v59;
            v45.z = v45.z + v60;
            v18 = (tGameObjectMain*)v56->o_Secundo;
            if (v18)
            {
                if ((unsigned int)v18 <= 2)
                    v18 = AI_MainActor[(uint32_t)v18 - 1];
                AI_EvalFunc_OBJSetPos_C(v18, &v45);
            }
            else
            {
                AI_EvalFunc_OBJSetPos_C(0, &v45);
            }
        }
        soundInstanceID = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x80);
        v51 = SND_i_InstanceSetf(soundInstanceID, 4114, 0.5);
        AI_EvalFunc_SNDUltraPlaySound_C(AI_gpst_CurrentGameObject, soundInstanceID);
        v38 = SND_f_GetVol(soundInstanceID);
        v4 = 0.5f * v38;
        SND_SetVol(soundInstanceID, v4);
        v62 = SND_f_InstanceGetf(soundInstanceID, 4116);
        if (v62 > 1.0)
            v5 = v62;
        else
            v5 = 1.0;
        v50 = v5;
        a4 = v5;
        SND_Release(soundInstanceID);
        soundInstanceID = -1;
        v56->o_Text = *(uint32_t*)INV_SecundoEtLaFacture_init_INV_manager_gao_o_PhotoText;
        v17 = (tGameObjectMain*)v56->o_Text;
        if (v56->o_Text)
        {
            if ((unsigned int)v17 <= 2)
                v17 = AI_MainActor[(uint32_t)v17 - 1];
            STR_Reset(v17);
        }
        else
        {
            STR_Reset(0);
        }
        v16 = (tGameObjectMain*)v56->o_Text;
        if (v56->o_Text)
        {
            if ((unsigned int)v16 <= 2)
                v16 = AI_MainActor[(uint32_t)v16 - 1];
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(v16, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
        }
        AI_EvalFunc_TEXTGet_C(0xD0002F90, 86, &Src);
        memcpy(&v39, &Src, sizeof(v39));
        v15 = (tGameObjectMain*)v56->o_Text;
        if (v56->o_Text)
        {
            if ((unsigned int)v15 <= 2)
                v15 = AI_MainActor[(uint32_t)v15 - 1];
            v14 = v15;
        }
        else
        {
            v14 = 0;
        }
        v2 = AI_EvalFunc_TEXTToString_C(&v39);
        STR_SetString(v14, 0, v2);
        v13 = (tGameObjectMain*)v56->o_Text;
        if (v56->o_Text)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            AI_EvalFunc_OBJ_TextFlagSet_C(v13, 0, 0x10);
        }
        else
        {
            AI_EvalFunc_OBJ_TextFlagSet_C(0, 0, 0x10);
        }
        a5.x = -1.0;
        a5.y = 0.0;
        a5.z = 0.0;
        v12 = (tGameObjectMain*)v56->o_Text;
        if (v56->o_Text)
        {
            if ((unsigned int)v12 <= 2)
                v12 = AI_MainActor[(uint32_t)v12 - 1];
            STR_SetSize(v12, 0, 0, -1, &a5);
        }
        else
        {
            STR_SetSize(0, 0, 0, -1, &a5);
        }
        v34.x = 0.40000001f;
        v34.y = 0.40000001f;
        v34.z = 0.0f;
        v11 = (tGameObjectMain*)v56->o_Text;
        if (v56->o_Text)
        {
            if ((unsigned int)v11 <= 2)
                v11 = AI_MainActor[(uint32_t)v11 - 1];
            AI_EvalFunc_OBJ_TextPivotSet_C(v11, 0, &v34);
        }
        else
        {
            AI_EvalFunc_OBJ_TextPivotSet_C(0, 0, &v34);
        }
        v10 = (tGameObjectMain*)v56->o_Text;
        if (v56->o_Text)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            v3 = STR_i_EffectAdd(v10, 8, 0, 0, -1);
        }
        else
        {
            v3 = STR_i_EffectAdd(0, 8, 0, 0, -1);
        }
        v57 = v3;
        v9 = (tGameObjectMain*)v56->o_Text;
        if (v56->o_Text)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            STR_EffectSetf(v9, v57, 750, a4);
        }
        else
        {
            STR_EffectSetf(0, v57, 750, a4);
        }
        v8 = (tGameObjectMain*)v56->o_Text;
        if (v56->o_Text)
        {
            if ((unsigned int)v8 <= 2)
                v8 = AI_MainActor[(uint32_t)v8 - 1];
            STR_EffectSetf(v8, v57, 751, 0.02f);
        }
        else
        {
            STR_EffectSetf(0, v57, 751, 0.02f);
        }
        v7 = (tGameObjectMain*)v56->o_Text;
        if (v56->o_Text)
        {
            if ((unsigned int)v7 <= 2)
                v7 = AI_MainActor[(uint32_t)v7 - 1];
            STR_EffectSetf(v7, v57, 752, 0.2f);
        }
        else
        {
            STR_EffectSetf(0, v57, 752, 0.2f);
        }
        v56->f_Timer = a4 + 0.5f;
        AI_EvalFunc_TRAPlayAll_C(AI_gpst_CurrentGameObject);
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_INV_SecundoEtLaFacture_init_INV_SecundoEtLaFacture_loop);
        result = 1000;
    }
    return result;
}

int INV_SecundoEtLaFacture_loop()
{
    int v0; // eax
    int result; // eax
    float v2; // [esp+0h] [ebp-C4h]
    tScriptStruct_INVSecundoEtLaFacture* v3; // [esp+4h] [ebp-C0h]
    tGameObjectMain* v4; // [esp+10h] [ebp-B4h]
    tGameObjectMain* v5; // [esp+18h] [ebp-ACh]
    double v6; // [esp+1Ch] [ebp-A8h]
    tGameObjectMain* v7; // [esp+28h] [ebp-9Ch]
    tGameObjectMain* v8; // [esp+30h] [ebp-94h]
    tGameObjectMain* v9; // [esp+34h] [ebp-90h]
    tGameObjectMain* v10; // [esp+38h] [ebp-8Ch]
    tGameObjectMain* v11; // [esp+40h] [ebp-84h]
    tGameObjectMain* v12; // [esp+48h] [ebp-7Ch]
    tGameObjectMain* v13; // [esp+4Ch] [ebp-78h]
    tGameObjectMain* v14; // [esp+50h] [ebp-74h]
    tGameObjectMain* v15; // [esp+58h] [ebp-6Ch]
    tGameObjectMain* v16; // [esp+60h] [ebp-64h]
    tGameObjectMain* v17; // [esp+68h] [ebp-5Ch]
    float v18; // [esp+6Ch] [ebp-58h]
    Vec3 v19; // [esp+70h] [ebp-54h] BYREF
    Vec3 a3; // [esp+7Ch] [ebp-48h] BYREF
    Vec3 v22; // [esp+94h] [ebp-30h] BYREF
    int v23; // [esp+A0h] [ebp-24h]
    int v24; // [esp+A4h] [ebp-20h]
    float v25; // [esp+ACh] [ebp-18h]
    int v26; // [esp+B0h] [ebp-14h]
    tScriptStruct_INVSecundoEtLaFacture* v27; // [esp+B8h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_INVSecundoEtLaFacture*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v27 = v3;
    v26 = 0;
    v23 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        INV_SecundoEtLaFacture_loop_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &INV_SecundoEtLaFacture_loop_INV_manager_gao_i_SmallTransfert,
            (void*)INV_SecundoEtLaFacture_loop_INV_manager_gao,
            21488);
        AI2C_ResolveBuffer(
            &INV_SecundoEtLaFacture_loop_INV_manager_gao_i_Fric2Add,
            (void*)INV_SecundoEtLaFacture_loop_INV_manager_gao,
            21476);
        AI2C_ResolveBuffer(
            &INV_SecundoEtLaFacture_loop_INV_manager_gao_i_FricWithPerso,
            (void*)INV_SecundoEtLaFacture_loop_INV_manager_gao,
            21484);
        AI2C_ResolveBuffer(
            &INV_SecundoEtLaFacture_loop_INV_manager_gao_f_PogneAppearTimeInit,
            (void*)INV_SecundoEtLaFacture_loop_INV_manager_gao,
            21504);
        AI2C_ResolveBuffer(
            &INV_SecundoEtLaFacture_loop_INV_manager_gao_f_PogneAppearTimeFixed,
            (void*)INV_SecundoEtLaFacture_loop_INV_manager_gao,
            21496);
        AI2C_ResolveBuffer(
            &INV_SecundoEtLaFacture_loop_INV_manager_gao_f_PogneDelay,
            (void*)INV_SecundoEtLaFacture_loop_INV_manager_gao,
            21500);
        AI2C_ResolveBuffer(
            &INV_SecundoEtLaFacture_loop_INV_manager_gao_f_Timer,
            (void*)INV_SecundoEtLaFacture_loop_INV_manager_gao,
            176);
        AI2C_ResolveBuffer(
            &INV_SecundoEtLaFacture_loop_INV_manager_gao_f_PogneAppearTime,
            (void*)INV_SecundoEtLaFacture_loop_INV_manager_gao,
            21492);
        AI2C_ResolveBuffer(
            &INV_SecundoEtLaFacture_loop_INV_manager_gao_i_SndPognes,
            (void*)INV_SecundoEtLaFacture_loop_INV_manager_gao,
            23064);
        INV_SecundoEtLaFacture_loop_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &INV_SecundoEtLaFacture_loop_univVars_f_sally_fric,
            (void*)INV_SecundoEtLaFacture_loop_univVars,
            1200);
        AI2C_ResolveBuffer(
            &INV_SecundoEtLaFacture_loop_INV_manager_gao_i_ModeEmpty_Flags,
            (void*)INV_SecundoEtLaFacture_loop_INV_manager_gao,
            23356);
        AI2C_ResolveBuffer(
            &INV_SecundoEtLaFacture_loop_INV_manager_gao_i_Locked,
            (void*)INV_SecundoEtLaFacture_loop_INV_manager_gao,
            172);
        AI2C_ResolveBuffer(
            &INV_SecundoEtLaFacture_loop_INV_manager_gao_i_AskExit,
            (void*)INV_SecundoEtLaFacture_loop_INV_manager_gao,
            152);
        AI2C_ResolveBuffer(
            &INV_SecundoEtLaFacture_loop_univVars_Flag_speciaux_sauve,
            (void*)INV_SecundoEtLaFacture_loop_univVars,
            4488);
    }
    if (v27->i_State)
    {
        if (v27->i_State == 1)
        {
            v27->f_Timer = v27->f_Timer - TIM_gf_dt;
            v14 = (tGameObjectMain*)v27->o_Secundo;
            if (v14)
            {
                if ((unsigned int)v14 <= 2)
                    v14 = AI_MainActor[(uint32_t)v14 - 1];
                v13 = v14;
            }
            else
            {
                v13 = 0;
            }
            if (v13)
            {
                v12 = (tGameObjectMain*)v27->o_Secundo;
                if (v12)
                {
                    if ((unsigned int)v12 <= 2)
                        v12 = AI_MainActor[(uint32_t)v12 - 1];
                    AI_EvalFunc_OBJGetPos_C(v12, &a3);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &a3);
                }
                v19.x = -0.050000001f;
                v19.y = 0.0;
                v19.z = 0.0;
                MATH_VecAddVec(&v22, &a3, &v19);
                v11 = (tGameObjectMain*)v27->o_Secundo;
                if (v11)
                {
                    if ((unsigned int)v11 <= 2)
                        v11 = AI_MainActor[(uint32_t)v11 - 1];
                    AI_EvalFunc_OBJSetPos_C(v11, &v22);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &v22);
                }
            }
            if (v27->f_Timer < 0.0)
            {
                v10 = (tGameObjectMain*)v27->o_Secundo;
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
                if (v9)
                {
                    v8 = (tGameObjectMain*)v27->o_Secundo;
                    if (v8)
                    {
                        if ((unsigned int)v8 <= 2)
                            v8 = AI_MainActor[(uint32_t)v8 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v8, 1);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
                    }
                    v7 = (tGameObjectMain*)v27->o_Secundo;
                    if (v7)
                    {
                        if ((unsigned int)v7 <= 2)
                            v7 = AI_MainActor[(uint32_t)v7 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v7, 1);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
                    }
                }
                *(uint32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_i_SmallTransfert = 0;
                *(int32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_i_Fric2Add = -350;
                *(uint32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_i_FricWithPerso = 15;
                *(uint32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_f_PogneAppearTimeInit = 0x3F800000;
                *(uint32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_f_PogneAppearTimeFixed = 0x3F000000;
                *(uint32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_f_PogneDelay = 0;
                *(uint32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_f_Timer = 0x3F800000;
                v27->i_State = 2;
            }
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else if (*(float*)INV_SecundoEtLaFacture_loop_INV_manager_gao_f_PogneAppearTime <= 0.0)
        {
            *(float*)INV_SecundoEtLaFacture_loop_INV_manager_gao_f_Timer = *(float*)INV_SecundoEtLaFacture_loop_INV_manager_gao_f_Timer
                - TIM_gf_dt;
            if (*(uint32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_i_Fric2Add)
            {
                if (*(uint32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_i_SndPognes == -1)
                {
                    *(uint32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_i_SndPognes = AI_EvalFunc_SNDRequest_C(
                        (tGameObjectMain*)INV_SecundoEtLaFacture_loop_INV_manager_gao,
                        4,
                        0);
                    AI_EvalFunc_SNDUltraPlaySoundLooped_C(
                        (tGameObjectMain*)INV_SecundoEtLaFacture_loop_INV_manager_gao,
                        *(uint32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_i_SndPognes);
                }
                if (*(float*)INV_SecundoEtLaFacture_loop_INV_manager_gao_f_Timer > 0.0)
                {
                    v25 = TIM_gf_dt;
                    v18 = TIM_gf_dt / (*(float*)INV_SecundoEtLaFacture_loop_INV_manager_gao_f_Timer * 0.80000001f);
                    v2 = (float)*(int*)INV_SecundoEtLaFacture_loop_INV_manager_gao_i_Fric2Add;
                    v6 = v18 * v2 + MATH_gd_magicDoubleForRound;
                    v26 = (*(uint32_t*)&v6);
                }
                else
                {
                    v26 = *(uint32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_i_Fric2Add;
                }
                *(uint32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_i_Fric2Add -= v26;
                *(uint32_t*)INV_SecundoEtLaFacture_loop_univVars_f_sally_fric += v26;
                if (!*(uint32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_i_Fric2Add)
                    *(uint32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_i_ModeEmpty_Flags = 0;
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 1000;
            }
            else
            {
                if (*(uint32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_i_SndPognes != -1)
                {
                    SND_Release(*(uint32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_i_SndPognes);
                    *(uint32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_i_SndPognes = -1;
                }
                if (!AI_EvalFunc_IoButtonJustPressed_C(0) && !AI_EvalFunc_IoButtonJustPressed_C(3))
                    goto LABEL_88;
                v5 = (tGameObjectMain*)v27->o_Text;
                if (v27->o_Text)
                {
                    if ((unsigned int)v5 <= 2)
                        v5 = AI_MainActor[(uint32_t)v5 - 1];
                    STR_Reset(v5);
                }
                else
                {
                    STR_Reset(0);
                }
                v4 = (tGameObjectMain*)v27->o_Text;
                if (v27->o_Text)
                {
                    if ((unsigned int)v4 <= 2)
                        v4 = AI_MainActor[(uint32_t)v4 - 1];
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(v4, 1);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
                }
                *(uint32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_i_Locked = 0;
                *(uint32_t*)INV_SecundoEtLaFacture_loop_INV_manager_gao_i_AskExit = 1;
                AI_EvalFunc_ARRBitSet_C((int*)(INV_SecundoEtLaFacture_loop_univVars_Flag_speciaux_sauve + 4), 2709);
                v24 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
                if (v24 != 1000)
                    result = v24;
                else
                    LABEL_88:
                result = 1000;
            }
        }
        else
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
    }
    else
    {
        v27->f_Timer = v27->f_Timer - TIM_gf_dt;
        if (v27->f_Timer < 0.0)
        {
            v27->f_Timer = 1.0;
            v17 = (tGameObjectMain*)v27->o_Text;
            if (v27->o_Text)
            {
                if ((unsigned int)v17 <= 2)
                    v17 = AI_MainActor[(uint32_t)v17 - 1];
                v0 = STR_i_EffectAdd(v17, 1, 0, 0, -1);
            }
            else
            {
                v0 = STR_i_EffectAdd(0, 1, 0, 0, -1);
            }
            v23 = v0;
            v16 = (tGameObjectMain*)v27->o_Text;
            if (v27->o_Text)
            {
                if ((unsigned int)v16 <= 2)
                    v16 = AI_MainActor[(uint32_t)v16 - 1];
                STR_EffectSetf(v16, v23, 150, 0.5);
            }
            else
            {
                STR_EffectSetf(0, v23, 150, 0.5);
            }
            v15 = (tGameObjectMain*)v27->o_Text;
            if (v27->o_Text)
            {
                if ((unsigned int)v15 <= 2)
                    v15 = AI_MainActor[(uint32_t)v15 - 1];
                STR_EffectSetf(v15, v23, 752, 0.2f);
            }
            else
            {
                STR_EffectSetf(0, v23, 752, 0.2f);
            }
            v27->i_State = 1;
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}