#include "ai_compiledscripts_inc.h"

//Onde_de_Choc_ODC_comportement
int Onde_de_Choc_ODC_comportement_univVars;
int Onde_de_Choc_ODC_comportement_univVars_Perso_ID_actor;
int Onde_de_Choc_ODC_comportement_univVars_Flag_speciaux_sauve;
int fct_Onde_de_Choc_ODC_comportement_Onde_de_Choc_ODC_bdisplay;
int fct_Onde_de_Choc_ODC_comportement_Onde_de_Choc_ODC_adisplay;
int fct_Onde_de_Choc_ODC_comportement_Onde_de_Choc_ODC_destroy;
int Onde_de_Choc_ODC_comportement__PJ_Sally_gao;

//Onde_de_Choc_ODC_init
int fct_Onde_de_Choc_ODC_init_Onde_de_Choc_ODC_bdisplay;
int fct_Onde_de_Choc_ODC_init_Onde_de_Choc_ODC_adisplay;
int fct_Onde_de_Choc_ODC_init_Onde_de_Choc_ODC_comportement;

int Onde_de_Choc_ODC_adisplay()
{
    tScriptStruct_SFXOndeDeChoc* v1; // [esp+4h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXOndeDeChoc*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    AI_EvalFunc_LightRLIFactorUsed_C(0);
    AI_EvalFunc_LightSetRLIFactor_C(v1->old_factor);
    AI_EvalFunc_LightSetRLIDestColor_C(v1->old_color);
    AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v1->old_constantcolor0, 0);
    AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v1->old_constantcolor1, 1);
    v1->i_after = 0;
    return 1000;
}

int Onde_de_Choc_ODC_bdisplay()
{
    tScriptStruct_SFXOndeDeChoc* v1; // [esp+4h] [ebp-24h]
    int a2; // [esp+14h] [ebp-14h]
    tScriptStruct_SFXOndeDeChoc* v3; // [esp+20h] [ebp-8h]
    int v4; // [esp+24h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXOndeDeChoc*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v3 = v1;
    v1->old_factor = AI_EvalFunc_LightGetRLIFactor_C();
    v1->old_color = AI_EvalFunc_LightGetRLIDestColor_C();
    v1->old_constantcolor0 = AI_EvalFunc_MATGetConstant_C(AI_gpst_CurrentGameObject, 0);
    v1->old_constantcolor1 = AI_EvalFunc_MATGetConstant_C(AI_gpst_CurrentGameObject, 1u);
    AI_EvalFunc_LightRLIFactorUsed_C(1);
    AI_EvalFunc_LightSetRLIFactor_C(v1->f_transparency_coef);
    AI_EvalFunc_LightSetRLIDestColor_C(-1);
    a2 = AI_EvalFunc_COLORBlend_C(v3->old_constantcolor0, 0, v3->f_transparency_coef);
    AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, a2, 0);
    v4 = AI_EvalFunc_COLORBlend_C(v3->old_constantcolor1, 0, v3->f_transparency_coef);
    AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v4, 1);
    return 1000;
}

int Onde_de_Choc_ODC_comportement()
{
    BOOL v1; // eax
    tScriptStruct_SFXOndeDeChoc* v2; // [esp+0h] [ebp-1CCh]
    tGameObjectMain* v3; // [esp+8h] [ebp-1C4h]
    tGameObjectMain* v4; // [esp+20h] [ebp-1ACh]
    tGameObjectMain* v5; // [esp+28h] [ebp-1A4h]
    tGameObjectMain* v6; // [esp+2Ch] [ebp-1A0h]
    tGameObjectMain* v7; // [esp+34h] [ebp-198h]
    tGameObjectMain* v8; // [esp+4Ch] [ebp-180h]
    tGameObjectMain* v9; // [esp+54h] [ebp-178h]
    tGameObjectMain* v10; // [esp+5Ch] [ebp-170h]
    tGameObjectMain* v11; // [esp+64h] [ebp-168h]
    tGameObjectMain* v12; // [esp+68h] [ebp-164h]
    tGameObjectMain* v13; // [esp+70h] [ebp-15Ch]
    tGameObjectMain* v14; // [esp+78h] [ebp-154h]
    float v15; // [esp+7Ch] [ebp-150h]
    tGameObjectMain* v16; // [esp+88h] [ebp-144h]
    tGameObjectMain* v17; // [esp+8Ch] [ebp-140h]
    tGameObjectMain* v18; // [esp+A0h] [ebp-12Ch]
    float v19; // [esp+A4h] [ebp-128h]
    float v20; // [esp+A4h] [ebp-128h]
    Vec3 v21; // [esp+A8h] [ebp-124h] BYREF
    Vec3 dest; // [esp+B8h] [ebp-114h] BYREF
    tGameObjectMain* v23; // [esp+C4h] [ebp-108h]
    float zoom; // [esp+C8h] [ebp-104h]
    tMessage Src; // [esp+D0h] [ebp-FCh] BYREF
    BOOL v26; // [esp+104h] [ebp-C8h]
    float v27; // [esp+108h] [ebp-C4h]
    Vec3 v28; // [esp+110h] [ebp-BCh] BYREF
    Vec3 v29; // [esp+11Ch] [ebp-B0h] BYREF
    Vec3 v30; // [esp+130h] [ebp-9Ch] BYREF
    Vec3 v31; // [esp+13Ch] [ebp-90h] BYREF
    Vec3 v32; // [esp+148h] [ebp-84h] BYREF
    Vec3 v33; // [esp+154h] [ebp-78h] BYREF
    BOOL v34; // [esp+170h] [ebp-5Ch]
    int v35; // [esp+174h] [ebp-58h]
    Vec3 a3; // [esp+178h] [ebp-54h] BYREF
    tGameObjectMain* v37; // [esp+184h] [ebp-48h]
    float v38; // [esp+188h] [ebp-44h]
    BOOL v39; // [esp+18Ch] [ebp-40h]
    Vec3 v40; // [esp+190h] [ebp-3Ch] BYREF
    Vec3 v41; // [esp+1A0h] [ebp-2Ch] BYREF
    tScriptStruct_SFXOndeDeChoc* v42; // [esp+1ACh] [ebp-20h]
    tGameObjectMain* v43; // [esp+1BCh] [ebp-10h]
    int v44; // [esp+1C8h] [ebp-4h]

    v29.x = 1.0;
    v29.y = 0.0;
    v29.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SFXOndeDeChoc*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v42 = v2;
    v23 = 0;
    zoom = 0.0;
    v38 = 0.0;
    v26 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Onde_de_Choc_ODC_comportement_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Onde_de_Choc_ODC_comportement_univVars_Perso_ID_actor,
            (void*)Onde_de_Choc_ODC_comportement_univVars,
            988);
        AI2C_ResolveBuffer(
            &Onde_de_Choc_ODC_comportement_univVars_Flag_speciaux_sauve,
            (void*)Onde_de_Choc_ODC_comportement_univVars,
            4488);
        fct_Onde_de_Choc_ODC_comportement_Onde_de_Choc_ODC_bdisplay = (int)AI_ul_RealResolveOneRefFunction(0x3D002CDFu);
        fct_Onde_de_Choc_ODC_comportement_Onde_de_Choc_ODC_adisplay = (int)AI_ul_RealResolveOneRefFunction(0x3D002CE0u);
        fct_Onde_de_Choc_ODC_comportement_Onde_de_Choc_ODC_destroy = (int)AI_ul_RealResolveOneRefFunction(0x870003EC);
        Onde_de_Choc_ODC_comportement__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
    }
    v23 = *AI_MainActor;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v41);
    v18 = v23;
    if (v23)
    {
        if ((unsigned int)v23 <= 2)
            v18 = AI_MainActor[(uint32_t)v23 - 1];
        AI_EvalFunc_OBJGetPos_C(v18, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    MATH_SubstractVec(&dest, &v41, &a3);
    v29 = dest;
    v38 = MATH_VecLen(&v29);
    v23 = *AI_MainActor;
    v17 = v23;
    if (v23)
    {
        if ((unsigned int)v23 <= 2)
            v17 = AI_MainActor[(uint32_t)v23 - 1];
        v16 = v17;
    }
    else
    {
        v16 = 0;
    }
    if (v16 == *(tGameObjectMain**)(Onde_de_Choc_ODC_comportement_univVars_Perso_ID_actor + 44))
    {
        v26 = AI_EvalFunc_ARRBitTest_C((int*)(Onde_de_Choc_ODC_comportement_univVars_Flag_speciaux_sauve + 4), 2197);
        if (v26)
            v42->des_f_rayon_max = 40.0;
        else
            v42->des_f_rayon_max = 25.0;
    }
    v42->i_after = 0;
    v15 = v42->f_blend;
    if (v15 < 0.0)
        v15 = 0.0;
    if (v15 > 1.0)
        v15 = 1.0;
    zoom = v15 * v42->des_f_rayon_max + (1.0f - v15) * 0.0f;
    AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, zoom);
    if (v42->des_f_rayon_max - 0.1 <= zoom)
    {
        v37 = AI_gpst_CurrentGameObject;
        v14 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v14 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                v14,
                0,
                (tAIFunction*)fct_Onde_de_Choc_ODC_comportement_Onde_de_Choc_ODC_bdisplay);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                0,
                0,
                (tAIFunction*)fct_Onde_de_Choc_ODC_comportement_Onde_de_Choc_ODC_bdisplay);
        }
        v43 = AI_gpst_CurrentGameObject;
        v13 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v13 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                v13,
                1,
                (tAIFunction*)fct_Onde_de_Choc_ODC_comportement_Onde_de_Choc_ODC_adisplay);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                0,
                1,
                (tAIFunction*)fct_Onde_de_Choc_ODC_comportement_Onde_de_Choc_ODC_adisplay);
        }
        v42->f_blend = TIM_gf_MainClock - TIM_gf_LockTime;
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        v35 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Onde_de_Choc_ODC_comportement_Onde_de_Choc_ODC_destroy);
        if (v35 != 1000)
            return v35;
    }
    v27 = TIM_gf_dt;
    v42->f_blend = v42->des_f_speed * TIM_gf_dt + v42->f_blend;
    v19 = zoom / v42->des_f_rayon_max;
    v20 = v19 + 1.0f;
    SND_SetFreq(v42->i_snd_loop, (unsigned __int64)(v42->f_snd_freqref * v20));
    v42->f_transparency_coef = 0.0;
    if (v42->des_f_rayon_max - 10.0 < zoom)
        v42->f_transparency_coef = (zoom - (v42->des_f_rayon_max - 10.0f)) / 10.0f;
    v23 = *AI_MainActor;
    if (v42->i_canpaf)
    {
        v12 = v23;
        if (v23)
        {
            if ((unsigned int)v23 <= 2)
                v12 = AI_MainActor[(uint32_t)v23 - 1];
            v11 = v12;
        }
        else
        {
            v11 = 0;
        }
        if (v11 == *(tGameObjectMain**)(Onde_de_Choc_ODC_comportement_univVars_Perso_ID_actor + 44))
        {
            if (v38 < (double)zoom && zoom - 8.0 < v38)
            {
                v10 = v23;
                if (v23)
                {
                    if ((unsigned int)v23 <= 2)
                        v10 = AI_MainActor[(uint32_t)v23 - 1];
                    v1 = AI_EvalFunc_OBJ_CapaTest_C(v10, 0x2000);
                }
                else
                {
                    v1 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x2000);
                }
                v34 = v1;
                if (v1)
                {
                    v9 = v23;
                    if (v23)
                    {
                        if ((unsigned int)v23 <= 2)
                            v9 = AI_MainActor[(uint32_t)v23 - 1];
                        AI_EvalFunc_DYNSpeedGetVector_C(v9, &v28);
                    }
                    else
                    {
                        AI_EvalFunc_DYNSpeedGetVector_C(0, &v28);
                    }
                    if (v28.z < 2.0)
                    {
                        v42->i_canpaf = 0;
                        v8 = v23;
                        if (v23)
                        {
                            if ((unsigned int)v23 <= 2)
                                v8 = AI_MainActor[(uint32_t)v23 - 1];
                            AI_EvalFunc_OBJGetPos_C(v8, &v33);
                        }
                        else
                        {
                            AI_EvalFunc_OBJGetPos_C(0, &v33);
                        }
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&v21);
                        MATH_SubstractVec(&v32, &v33, &v21);
                        v29 = v32;
                        Src.msgID = 37;
                        Src.gao = AI_gpst_CurrentGameObject;
                        Src.gao2 = 0;
                        Src.Int1 = 0x60065;
                        Src.Int2 = 0;
                        Src.v1 = v32;
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&Src.v2);
                        Src.f2 = 20.0;
                        Src.f1 = 1000.0;
                        v7 = v23;
                        if (v23)
                        {
                            if ((unsigned int)v23 <= 2)
                                v7 = AI_MainActor[(uint32_t)v23 - 1];
                            AI_EvalFunc_MSGSend_C(v7, &Src);
                        }
                        else
                        {
                            AI_EvalFunc_MSGSend_C(0, &Src);
                        }
                    }
                }
            }
        }
        else
        {
            v6 = v23;
            if (v23)
            {
                if ((unsigned int)v23 <= 2)
                    v6 = AI_MainActor[(uint32_t)v23 - 1];
                v5 = v6;
            }
            else
            {
                v5 = 0;
            }
            if (v5 == *(tGameObjectMain**)(Onde_de_Choc_ODC_comportement_univVars_Perso_ID_actor + 36))
            {
                v44 = AI_EvalFunc_ACTActionGet_C((tGameObjectMain*)Onde_de_Choc_ODC_comportement__PJ_Sally_gao);
                if (v44 != 158)
                {
                    v39 = AI_EvalFunc_OBJ_CapaTest_C((tGameObjectMain*)Onde_de_Choc_ODC_comportement__PJ_Sally_gao, 0x200);
                    if (!v39 && v38 < (double)zoom && zoom - 8.0 < v38)
                    {
                        v42->i_canpaf = 0;
                        v4 = v23;
                        if (v23)
                        {
                            if ((unsigned int)v23 <= 2)
                                v4 = AI_MainActor[(uint32_t)v23 - 1];
                            AI_EvalFunc_OBJGetPos_C(v4, &v31);
                        }
                        else
                        {
                            AI_EvalFunc_OBJGetPos_C(0, &v31);
                        }
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&v30);
                        MATH_SubstractVec(&v40, &v31, &v30);
                        v29 = v40;
                        Src.msgID = 37;
                        Src.gao = AI_gpst_CurrentGameObject;
                        Src.gao2 = 0;
                        Src.Int1 = 0x60065;
                        Src.Int2 = 0;
                        Src.v1 = v40;
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&Src.v2);
                        Src.f2 = 20.0;
                        Src.f1 = 1000.0;
                        v3 = v23;
                        if (v23)
                        {
                            if ((unsigned int)v23 <= 2)
                                v3 = AI_MainActor[(uint32_t)v23 - 1];
                            AI_EvalFunc_MSGSend_C(v3, &Src);
                        }
                        else
                        {
                            AI_EvalFunc_MSGSend_C(0, &Src);
                        }
                    }
                }
            }
        }
    }
    return 1000;
}

int Onde_de_Choc_ODC_destroy()
{
    float v1; // [esp+4h] [ebp-1Ch]
    tScriptStruct_SFXOndeDeChoc* v2; // [esp+8h] [ebp-18h]
    float v3; // [esp+Ch] [ebp-14h]
    float v4; // [esp+Ch] [ebp-14h]
    float v5; // [esp+14h] [ebp-Ch]
    int v6; // [esp+18h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_SFXOndeDeChoc*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (AI_EvalFunc_TIMEElapsed_C(v2->f_blend, 0.5))
    {
        if (v2->i_snd_loop != -1)
        {
            SND_Release(v2->i_snd_loop);
            v2->i_snd_loop = -1;
        }
        v6 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v6 != 1000)
            return v6;
    }
    else
    {
        v5 = TIM_gf_MainClock - TIM_gf_LockTime;
        v3 = (v5 - v2->f_blend) / 0.5f;
        v4 = 1.0f - v3;
        v1 = v2->f_snd_volref * v4;
        SND_SetVol(v2->i_snd_loop, v1);
    }
    return 1000;
}

int Onde_de_Choc_ODC_init()
{
    int v0; // eax
    double v1; // st7
    int v2; // eax
    int result; // eax
    float v4; // [esp+0h] [ebp-70h]
    int v5; // [esp+4h] [ebp-6Ch]
    tScriptStruct_SFXOndeDeChoc* v6; // [esp+8h] [ebp-68h]
    tGameObjectMain* v7; // [esp+10h] [ebp-60h]
    tGameObjectMain* v8; // [esp+18h] [ebp-58h]
    tGameObjectMain* v9; // [esp+20h] [ebp-50h]
    tGameObjectMain* v10; // [esp+28h] [ebp-48h]
    Vec3 a2; // [esp+30h] [ebp-40h] BYREF
    tGameObjectMain* v12; // [esp+40h] [ebp-30h]
    int v13; // [esp+44h] [ebp-2Ch]
    Vec3 v14; // [esp+4Ch] [ebp-24h] BYREF
    int v15; // [esp+58h] [ebp-18h]
    int a1; // [esp+5Ch] [ebp-14h]
    tGameObjectMain* v17; // [esp+60h] [ebp-10h]
    tScriptStruct_SFXOndeDeChoc* v18; // [esp+64h] [ebp-Ch]
    float v19; // [esp+6Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_SFXOndeDeChoc*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v18 = v6;
    a1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Onde_de_Choc_ODC_init_Onde_de_Choc_ODC_bdisplay = (int)AI_ul_RealResolveOneRefFunction(0x3D002CDFu);
        fct_Onde_de_Choc_ODC_init_Onde_de_Choc_ODC_adisplay = (int)AI_ul_RealResolveOneRefFunction(0x3D002CE0u);
        fct_Onde_de_Choc_ODC_init_Onde_de_Choc_ODC_comportement = (int)AI_ul_RealResolveOneRefFunction(0x72004A8Fu);
    }
    v12 = AI_gpst_CurrentGameObject;
    v10 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v10 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v10,
            0,
            (tAIFunction*)fct_Onde_de_Choc_ODC_init_Onde_de_Choc_ODC_bdisplay);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            0,
            (tAIFunction*)fct_Onde_de_Choc_ODC_init_Onde_de_Choc_ODC_bdisplay);
    }
    v17 = AI_gpst_CurrentGameObject;
    v9 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v9 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v9,
            1,
            (tAIFunction*)fct_Onde_de_Choc_ODC_init_Onde_de_Choc_ODC_adisplay);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            1,
            (tAIFunction*)fct_Onde_de_Choc_ODC_init_Onde_de_Choc_ODC_adisplay);
    }
    AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, 0.1f);
    v18->i_canpaf = 1;
    v14.x = -v18->des_f_rayon_max;
    v14.y = -v18->des_f_rayon_max;
    v14.z = -v18->des_f_hauteur;
    AI_EvalFunc_BVMinSet_C(AI_gpst_CurrentGameObject, &v14);
    a2.x = v18->des_f_rayon_max;
    a2.y = v18->des_f_rayon_max;
    a2.z = v18->des_f_hauteur;
    AI_EvalFunc_BVMaxSet_C(AI_gpst_CurrentGameObject, &a2);
    v8 = (tGameObjectMain*)v18->o_sound_bank;
    if (v8)
    {
        if ((unsigned int)v8 <= 2)
            v8 = AI_MainActor[(uint32_t)v8 - 1];
        v0 = AI_EvalFunc_SNDRequest_C(v8, 2, 0x18001000);
    }
    else
    {
        v0 = AI_EvalFunc_SNDRequest_C(0, 2, 0x18001000);
    }
    v18->i_snd_loop = v0;
    v5 = SND_l_GetFreq(v18->i_snd_loop);
    v18->f_snd_freqref = (float)v5;
    v1 = SND_f_GetVol(v18->i_snd_loop);
    v18->f_snd_volref = (float)v1;
    AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v18->i_snd_loop);
    v7 = (tGameObjectMain*)v18->o_sound_bank;
    if (v7)
    {
        if ((unsigned int)v7 <= 2)
            v7 = AI_MainActor[(uint32_t)v7 - 1];
        v2 = AI_EvalFunc_SNDRequest_C(v7, 1, 0x18001080);
    }
    else
    {
        v2 = AI_EvalFunc_SNDRequest_C(0, 1, 0x18001080);
    }
    a1 = v2;
    AI_EvalFunc_SNDUltraPlaySound_C(AI_gpst_CurrentGameObject, v2);
    v15 = SND_l_GetFreq(a1);
    v19 = AI_EvalFunc_MATHRandFloat_C(0.89999998f, 1.1f);
    v4 = (float)v15;
    SND_SetFreq(a1, (unsigned int)(v4 * v19));
    v13 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Onde_de_Choc_ODC_init_Onde_de_Choc_ODC_comportement);
    if (v13 == 1000)
        result = 1000;
    else
        result = v13;
    return result;
}