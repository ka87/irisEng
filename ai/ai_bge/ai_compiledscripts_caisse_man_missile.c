#include "ai_compiledscripts_inc.h"

//Caisse_Man_Missile_CMM_explosion
int Caisse_Man_Missile_CMM_explosion_SFX_explosion_gao;
int Caisse_Man_Missile_CMM_explosion__PJ_Sally_gao;
int Caisse_Man_Missile_CMM_explosion_univVars;
int Caisse_Man_Missile_CMM_explosion_univVars_objet_liste_gao;
int Caisse_Man_Missile_CMM_explosion_GST_Global_gao;
int Caisse_Man_Missile_CMM_explosion_GST_Global_gao_i_nb_SFX_Explosion_Fire_SND;

//Caisse_Man_Missile_CMM_disparition
int Caisse_Man_Missile_CMM_disparition_GST_Global_gao;
int Caisse_Man_Missile_CMM_disparition_GST_Global_gao_i_nb_SFX_Explosion_Fire_SND;

//Caisse_Man_Missile_CMM_comportement
int fct_Caisse_Man_Missile_CMM_comportement_Caisse_Man_Missile_CMM_disparition;
int Caisse_Man_Missile_CMM_comportement__PJ_Sally_gao;
int Caisse_Man_Missile_CMM_comportement_univVars;
int Caisse_Man_Missile_CMM_comportement_univVars_Perso_ID_actor;
int fct_Caisse_Man_Missile_CMM_comportement_Caisse_Man_Missile_CMM_explosion;

//Caisse_Man_Missile_CMM_init
int Caisse_Man_Missile_CMM_init_GST_Global_gao;
int Caisse_Man_Missile_CMM_init_GST_Global_gao_i_nb_SFX_Explosion_Fire_SND;
int fctvar_Caisse_Man_Missile_CMM_init_UNKNOWN;
int Caisse_Man_Missile_CMM_init_univVars;
int Caisse_Man_Missile_CMM_init_univVars_Perso_ID_actor;
int Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao;
int fct_Caisse_Man_Missile_CMM_init_Caisse_Man_Missile_CMM_afterrec;
int fct_Caisse_Man_Missile_CMM_init_Caisse_Man_Missile_CMM_comportement;
int fct_Caisse_Man_Missile_CMM_init_Caisse_Man_Missile_CMM_reflex;

//Caisse_Man_Missile_CMM_reflex
int Caisse_Man_Missile_CMM_reflex_GST_Global_gao;
int Caisse_Man_Missile_CMM_reflex_GST_Global_gao_i_nb_SFX_Explosion_Fire_SND;
int fct_Caisse_Man_Missile_CMM_reflex_Caisse_Man_Missile_CMM_disparition;

int Caisse_Man_Missile_CMM_explosion()
{
    BOOL v0; // eax
    int v1; // eax
    tGameObjectMain* v2; // eax
    int result; // eax
    float v4; // [esp+0h] [ebp-2E8h]
    float size; // [esp+4h] [ebp-2E4h]
    float v6; // [esp+8h] [ebp-2E0h]
    tScriptStruct_CaisseManMissile* v7; // [esp+10h] [ebp-2D8h]
    float v8; // [esp+14h] [ebp-2D4h]
    float v9; // [esp+18h] [ebp-2D0h]
    tGameObjectMain* v10; // [esp+20h] [ebp-2C8h]
    tGameObjectMain* v11; // [esp+28h] [ebp-2C0h]
    tGameObjectMain* v12; // [esp+30h] [ebp-2B8h]
    tGameObjectMain* v13; // [esp+3Ch] [ebp-2ACh]
    tGameObjectMain* v14; // [esp+5Ch] [ebp-28Ch]
    tGameObjectMain* v15; // [esp+64h] [ebp-284h]
    tGameObjectMain* v16; // [esp+84h] [ebp-264h]
    tGameObjectMain* v17; // [esp+88h] [ebp-260h]
    tGameObjectMain* v18; // [esp+8Ch] [ebp-25Ch]
    tGameObjectMain* v19; // [esp+94h] [ebp-254h]
    tAI* v20; // [esp+98h] [ebp-250h]
    unsigned int v21; // [esp+9Ch] [ebp-24Ch]
    tGameObjectMain* v22; // [esp+A0h] [ebp-248h]
    tGameObjectMain* v23; // [esp+A4h] [ebp-244h]
    tGameObjectMain* v24; // [esp+ACh] [ebp-23Ch]
    tAI* v25; // [esp+B0h] [ebp-238h]
    tAI* v26; // [esp+B4h] [ebp-234h]
    tGameObjectMain* v27; // [esp+BCh] [ebp-22Ch]
    float v28; // [esp+104h] [ebp-1E4h]
    tMessage Src; // [esp+108h] [ebp-1E0h] BYREF
    tGameObjectMain* v30; // [esp+13Ch] [ebp-1ACh]
    Vec3 v31; // [esp+140h] [ebp-1A8h] BYREF
    int v32; // [esp+14Ch] [ebp-19Ch]
    int v33; // [esp+150h] [ebp-198h]
    float v34; // [esp+154h] [ebp-194h]
    int v35; // [esp+158h] [ebp-190h]
    Vec3 v36; // [esp+15Ch] [ebp-18Ch] BYREF
    Vec3 v37; // [esp+168h] [ebp-180h] BYREF
    int v38; // [esp+174h] [ebp-174h]
    Vec3 v39; // [esp+178h] [ebp-170h] BYREF
    Vec3 v40; // [esp+184h] [ebp-164h] BYREF
    int v42; // [esp+198h] [ebp-150h]
    int v43; // [esp+1A0h] [ebp-148h]
    Vec3 v45; // [esp+1ACh] [ebp-13Ch] BYREF
    Vec3 v46; // [esp+1B8h] [ebp-130h] BYREF
    Vec3 a3; // [esp+1C4h] [ebp-124h] BYREF
    unsigned int v49; // [esp+1D4h] [ebp-114h]
    Vec3 v50; // [esp+1D8h] [ebp-110h] BYREF
    Vec3 pos; // [esp+1E4h] [ebp-104h] BYREF
    BOOL v52; // [esp+1F0h] [ebp-F8h]
    int i; // [esp+1FCh] [ebp-ECh]
    int v55; // [esp+204h] [ebp-E4h]
    float v56; // [esp+208h] [ebp-E0h]
    float v57; // [esp+20Ch] [ebp-DCh]
    tScriptStruct_CaisseManMissile* v58; // [esp+21Ch] [ebp-CCh]
    Vec3 v59; // [esp+220h] [ebp-C8h] BYREF
    Vec3 v60; // [esp+22Ch] [ebp-BCh] BYREF
    float v62; // [esp+23Ch] [ebp-ACh]
    Vec3 v63; // [esp+244h] [ebp-A4h] BYREF
    Vec3 dest; // [esp+250h] [ebp-98h] BYREF
    unsigned int v65; // [esp+25Ch] [ebp-8Ch]
    tMessage v66; // [esp+260h] [ebp-88h] BYREF
    tGameObjectMain* a7[20]; // [esp+298h] [ebp-50h] BYREF

    v40.x = 1.0;
    v40.y = 0.0;
    v40.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_CaisseManMissile*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    v58 = v7;
    v65 = 0;
    v33 = 0;
    i = 0;
    v55 = 0;
    v30 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Caisse_Man_Missile_CMM_explosion_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
        Caisse_Man_Missile_CMM_explosion__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Caisse_Man_Missile_CMM_explosion_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Caisse_Man_Missile_CMM_explosion_univVars_objet_liste_gao,
            (void*)Caisse_Man_Missile_CMM_explosion_univVars,
            1276);
        Caisse_Man_Missile_CMM_explosion_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Caisse_Man_Missile_CMM_explosion_GST_Global_gao_i_nb_SFX_Explosion_Fire_SND,
            (void*)Caisse_Man_Missile_CMM_explosion_GST_Global_gao,
            4640);
    }
    if (*AI_gppst_CurrentJumpNode != (tAINode*)21)
    {
        if (v58->GFX_Lightning_1 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v58->GFX_Lightning_1);
            v58->GFX_Lightning_1 = -1;
        }
        if (v58->GFX_Lightning_2 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v58->GFX_Lightning_2);
            v58->GFX_Lightning_2 = -1;
        }
        if (v58->GFX_Lightning_3 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v58->GFX_Lightning_3);
            v58->GFX_Lightning_3 = -1;
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v36);
        size = v58->des_f_explosion_size_rand + 1.0f;
        v4 = 1.0f - v58->des_f_explosion_size_rand;
        v56 = AI_EvalFunc_MATHRandFloat_C(v4, size);
        v28 = v58->des_f_explosion_size * v56;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
        v30 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)Caisse_Man_Missile_CMM_explosion_SFX_explosion_gao, &pos);
        Src.msgID = 1;
        Src.Int1 = v58->des_i_num_explosion;
        Src.f2 = v28;
        Src.f1 = 1.0;
        Src.v1.x = v58->des_f_explosion_detail;
        Src.v1.y = 0.0;
        Src.v1.z = 1.0;
        v27 = v30;
        if (v30)
        {
            if ((unsigned int)v30 <= 2)
                v27 = AI_MainActor[(uint32_t)v30 - 1];
            AI_EvalFunc_MSGSend_C(v27, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
        if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x80))
            AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 0x80);
        v26 = (tAI*)v58->des_o_target;
        if (v58->des_o_target)
        {
            if ((unsigned int)v26 <= 2)
                v26 = (tAI*)AI_MainActor[(uint32_t)v26 - 1];
            v25 = v26;
        }
        else
        {
            v25 = 0;
        }
        if (v25 == (tAI*)Caisse_Man_Missile_CMM_explosion__PJ_Sally_gao
            && (v32 = AI_EvalFunc_ACTDesignFlagsGet_C((tGameObjectMain*)Caisse_Man_Missile_CMM_explosion__PJ_Sally_gao),
                (v32 & 0x2000) != 0))
        {
            v58->i_plantage_missile = 1;
        }
        else
        {
            v24 = (tGameObjectMain*)v58->des_o_target;
            if (v58->des_o_target)
            {
                if ((unsigned int)v24 <= 2)
                    v24 = AI_MainActor[(uint32_t)v24 - 1];
                v0 = AI_EvalFunc_OBJ_CapaTest_C(v24, 0x200);
            }
            else
            {
                v0 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x200);
            }
            v52 = v0;
            if (v0)
            {
                v58->i_plantage_missile = 1;
            }
            else if (v58->i_beluga)
            {
                v21 = v58->des_o_target;
                if ((tAI*)v58->des_o_target == WOR_gst_Universe.univAIInstance)
                {
                    v20 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v23 = (tGameObjectMain*)v58->des_o_target;
                    if (v21)
                    {
                        if (v21 <= 2)
                            v23 = AI_MainActor[v21 - 1];
                        v22 = v23;
                    }
                    else
                    {
                        v22 = 0;
                    }
                    v20 = v22->extended->AIInstance;
                }
                if (!AI_EvalFunc_TIMEElapsed_C(*((float*)v20->instanceVarDataCurrent + 122), 1.0)) // TODO FIXME: kideríteni hogy mi ez
                    goto LABEL_51;
                v19 = (tGameObjectMain*)v58->des_o_target;
                if (v58->des_o_target)
                {
                    if ((unsigned int)v19 <= 2)
                        v19 = AI_MainActor[(uint32_t)v19 - 1];
                    v1 = AI_EvalFunc_MSGGetCount_C(v19);
                }
                else
                {
                    v1 = AI_EvalFunc_MSGGetCount_C(0);
                }
                v38 = v1;
                if (v1)
                    LABEL_51:
                v58->i_plantage_missile = 1;
            }
        }
        if (v58->i_plantage_missile)
        {
            if (v58->des_drop != 3)
            {
                v30 = *(tGameObjectMain**)(Caisse_Man_Missile_CMM_explosion_univVars_objet_liste_gao + 16);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v59);
                v12 = v30;
                if (v30)
                {
                    if ((unsigned int)v30 <= 2)
                        v12 = AI_MainActor[(uint32_t)v30 - 1];
                    v2 = AI_EvalFunc_OBJDuplicate_C(v12, &v59);
                }
                else
                {
                    v2 = AI_EvalFunc_OBJDuplicate_C(0, &v59);
                }
                v65 = (unsigned int)v2;
                v66.msgID = 1;
                v66.Int1 = 1;
                v66.Int2 = 1;
                v66.f2 = 5.0;
                v66.f1 = 1.5;
                v66.v1.x = 0.0;
                v66.v1.y = 0.0;
                v66.v1.z = 0.0;
                v11 = v2;
                if (v2)
                {
                    if ((unsigned int)v2 <= 2)
                        v11 = AI_MainActor[(uint32_t)v2 - 1];
                    AI_EvalFunc_MSGSend_C(v11, &v66);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &v66);
                }
                v10 = (tGameObjectMain*)v65;
                if (v65)
                {
                    if (v65 <= 2)
                        v10 = AI_MainActor[v65 - 1];
                    AI_EvalFunc_COLSetActivationSet_C(v10, 0, 1);
                }
                else
                {
                    AI_EvalFunc_COLSetActivationSet_C(0, 0, 1);
                }
            }
        }
        else
        {
            v18 = (tGameObjectMain*)v58->o_paf;
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
                v33 = 20;
                v16 = (tGameObjectMain*)v58->o_paf;
                if (v16)
                {
                    if ((unsigned int)v16 <= 2)
                        v16 = AI_MainActor[(uint32_t)v16 - 1];
                    AI_EvalFunc_OBJGetPos_C(v16, &a3);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &a3);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v31);
                MATH_SubstractVec(&dest, &a3, &v31);
                v40 = dest;
                if (AI_EvalFunc_MTHVecNullEpsilon_C(&v40))
                {
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v37);
                    v40 = v37;
                }
                v58->m_msg.msgID = 37;
                v58->m_msg.gao = AI_gpst_CurrentGameObject;
                v58->m_msg.gao2 = 0;
                v58->m_msg.Int1 = v58->des_puissance + 1;
                v58->m_msg.Int2 = 0;
                v58->m_msg.v1 = v40;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v58->m_msg.v2);
                v58->m_msg.f2 = (float)v33;
                v58->m_msg.f1 = 1000.0;
                v15 = (tGameObjectMain*)v58->o_paf;
                if (v15)
                {
                    if ((unsigned int)v15 <= 2)
                        v15 = AI_MainActor[(uint32_t)v15 - 1];
                    AI_EvalFunc_MSGSend_C(v15, &v58->m_msg);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &v58->m_msg);
                }
            }
            else
            {
                if (v58->i_mode_test_PAF)
                    v55 = AI_EvalFunc_COLZDEZDEListGet_C(AI_gpst_CurrentGameObject, a7, 6, 1, 1, 0, 1);
                else
                    v55 = AI_EvalFunc_COLZDEZDEListGet_C(AI_gpst_CurrentGameObject, a7, 6, 1, 32800, 0, 0);
                for (i = 0; i < v55; ++i)
                {
                    v14 = a7[i];
                    if (v14)
                    {
                        if ((unsigned int)v14 <= 2)
                            v14 = AI_MainActor[(uint32_t)v14 - 1];
                        AI_EvalFunc_OBJGetPos_C(v14, &v46);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v46);
                    }
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v63);
                    MATH_SubstractVec(&v50, &v46, &v63);
                    v40 = v50;
                    if (AI_EvalFunc_MTHVecNullEpsilon_C(&v40))
                    {
                        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v39);
                        v40 = v39;
                    }
                    else
                    {
                        AI_EvalFunc_MATHVecSetHorzNormalize_C(&v40);
                    }
                    v33 = 20;
                    v58->m_msg.msgID = 37;
                    v58->m_msg.gao = AI_gpst_CurrentGameObject;
                    v58->m_msg.gao2 = 0;
                    v58->m_msg.Int1 = v58->des_puissance + 1;
                    v58->m_msg.Int2 = 0;
                    v58->m_msg.v1 = v40;
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v58->m_msg.v2);
                    v58->m_msg.f2 = (float)v33;
                    v58->m_msg.f1 = 1000.0;
                    v13 = a7[i];
                    if (v13)
                    {
                        if ((unsigned int)v13 <= 2)
                            v13 = AI_MainActor[(uint32_t)v13 - 1];
                        AI_EvalFunc_MSGSend_C(v13, &v58->m_msg);
                    }
                    else
                    {
                        AI_EvalFunc_MSGSend_C(0, &v58->m_msg);
                    }
                }
            }
        }
        SND_Release(v58->i_sound);
        v58->i_sound = -1;
        v43 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(2, -1.0);
        AI_EvalFunc_AITrackStop_C(1);
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        v45.x = 0.0;
        v45.y = 0.0;
        v45.z = 0.0;
        AI_EvalFunc_DYNTractionSet_C_CURRENT(&v45);
        v60.x = 0.0;
        v60.y = 0.0;
        v60.z = 0.0;
        v35 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v60);
        if (v58->i_flag_play_generation_snd)
        {
            v58->i_flag_play_generation_snd = 0;
            --* (uint32_t*)Caisse_Man_Missile_CMM_explosion_GST_Global_gao_i_nb_SFX_Explosion_Fire_SND;
        }
        v58->f_time_disp = TIM_gf_MainClock - TIM_gf_LockTime;
        v58->f_size = 0.2f;
        v58->f_size_halo = 2.0;
    }
    if (AI_EvalFunc_TIMEElapsed_C(v58->f_time_disp, 2.0))
    {
        if (v58->GFX_Halo != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v58->GFX_Halo);
            v58->GFX_Halo = -1;
        }
        v49 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0, 0x10000000);
        v42 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v42 == 1000)
            result = 1000;
        else
            result = v42;
    }
    else
    {
        v9 = 12.0f * TIM_gf_dt;
        if (v9 < 0.0)
            v9 = 0.0;
        if (v9 > 1.0)
            v9 = 1.0;
        v34 = 0.0f * v9 + v58->f_size * (1.0f - v9);
        v58->f_size = v34;
        XMEN_SetSize(AI_gpst_CurrentGameObject, 0, -1, v58->f_size);
        v57 = TIM_gf_dt;
        v8 = v58->f_size_halo - TIM_gf_dt * 2.4000001f;
        if (v8 < 0.0)
            v6 = 0.0;
        else
            v6 = v58->f_size_halo - TIM_gf_dt * 2.4000001f;
        v62 = v6;
        v58->f_size_halo = v6;
        AI_EvalFunc_GFXSetf_C_CURRENT(v58->GFX_Halo, 21000, v58->f_size_halo);
        *AI_gppst_CurrentJumpNode = (tAINode*)21;
        result = 0;
    }
    return result;
}

int Caisse_Man_Missile_CMM_disparition()
{
    float size; // [esp+4h] [ebp-58h]
    float sizea; // [esp+4h] [ebp-58h]
    float v3; // [esp+8h] [ebp-54h]
    tScriptStruct_CaisseManMissile* v4; // [esp+10h] [ebp-4Ch]
    float v5; // [esp+14h] [ebp-48h]
    float v6; // [esp+18h] [ebp-44h]
    uint32_t a2; // [esp+1Ch] [ebp-40h]
    uint32_t a2a; // [esp+1Ch] [ebp-40h]
    uint32_t a2b; // [esp+1Ch] [ebp-40h]
    float v10; // [esp+20h] [ebp-3Ch]
    float v11; // [esp+24h] [ebp-38h]
    int v12; // [esp+44h] [ebp-18h]
    float v13; // [esp+58h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_CaisseManMissile*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Caisse_Man_Missile_CMM_disparition_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Caisse_Man_Missile_CMM_disparition_GST_Global_gao_i_nb_SFX_Explosion_Fire_SND,
            (void*)Caisse_Man_Missile_CMM_disparition_GST_Global_gao,
            4640);
    }
    if (*AI_gppst_CurrentJumpNode != (tAINode*)4)
    {
        if (!AI_EvalFunc_TIMEElapsed_C(v4->f_time_start, 1.0))
        {
            v13 = TIM_gf_MainClock - TIM_gf_LockTime;
            v10 = 1.0f - (v13 - v4->f_time_start);
            v4->f_amplitude_min = v10 + v10;
            v4->f_amplitude_max = v10 * 5.0f;
            a2 = (uint32_t)AI_EvalFunc_COLORBlend_C(0, v4->c_color, v10);
            GFXSetCurGlobal(v4->GFX_Halo);
            AI_EvalFunc_GFXSeti_C_CURGLOB(4102, a2);
            sizea = 12.0f * v10;
            AI_EvalFunc_GFXSetf_C_CURGLOB(4000, sizea);
            a2a = (uint32_t)AI_EvalFunc_COLORBlend_C(0, 0x28002800, v10);
            AI_EvalFunc_GFXSeti_C_CURRENT(v4->GFX_Lightning_1, 1100, a2a);
            AI_EvalFunc_GFXSeti_C_CURRENT(v4->GFX_Lightning_2, 1100, a2a);
            AI_EvalFunc_GFXSeti_C_CURRENT(v4->GFX_Lightning_3, 1100, a2a);
            a2b = (uint32_t)AI_EvalFunc_COLORBlend_C(0, 0x80A4E794, v10);
            AI_EvalFunc_LightSetColor_C(AI_gpst_CurrentGameObject, (int)a2b);
            return 1000;
        }
        if (v4->GFX_Lightning_1 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v4->GFX_Lightning_1);
            v4->GFX_Lightning_1 = -1;
        }
        if (v4->GFX_Lightning_2 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v4->GFX_Lightning_2);
            v4->GFX_Lightning_2 = -1;
        }
        if (v4->GFX_Lightning_3 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v4->GFX_Lightning_3);
            v4->GFX_Lightning_3 = -1;
        }
        SND_Release(v4->i_sound);
        v4->i_sound = -1;
        AI_EvalFunc_AITrackStop_C(1);
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        v4->f_time_disp = TIM_gf_MainClock - TIM_gf_LockTime;
        v4->f_size = 0.2f;
        v4->f_size_halo = 2.0;
        if (v4->i_flag_play_generation_snd)
        {
            v4->i_flag_play_generation_snd = 0;
            --* (uint32_t*)Caisse_Man_Missile_CMM_disparition_GST_Global_gao_i_nb_SFX_Explosion_Fire_SND;
        }
    }
    if (AI_EvalFunc_TIMEElapsed_C(v4->f_time_disp, 2.0))
    {
        if (v4->GFX_Halo != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v4->GFX_Halo);
            v4->GFX_Halo = -1;
        }
        AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0, 0x10000000);
        v12 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v12 != 1000)
            return v12;
        return 1000;
    }
    v6 = 4.0f * TIM_gf_dt;
    if (v6 < 0.0)
        v6 = 0.0;
    if (v6 > 1.0)
        v6 = 1.0;
    v11 = 0.0f * v6 + v4->f_size * (1.0f - v6);
    v4->f_size = v11;
    XMEN_SetSize(AI_gpst_CurrentGameObject, 0, -1, v4->f_size);
    v5 = v4->f_size_halo - TIM_gf_dt * 6.0f;
    if (v5 < 0.0)
        v3 = 0.0;
    else
        v3 = v4->f_size_halo - TIM_gf_dt * 6.0f;
    v4->f_size_halo = v3;
    if (v4->i_beluga)
    {
        size = v4->f_size / 5.0f;
        AI_EvalFunc_GFXSetf_C_CURRENT(v4->GFX_Halo, 21000, size);
    }
    else
    {
        AI_EvalFunc_GFXSetf_C_CURRENT(v4->GFX_Halo, 21000, v4->f_size_halo);
    }
    *AI_gppst_CurrentJumpNode = (tAINode*)4;
    return 0;
}

int Caisse_Man_Missile_CMM_comportement()
{
    double v0; // st7
    BOOL v1; // eax
    BOOL v3; // eax
    tGameObjectMain* v4; // eax
    float a4; // [esp+0h] [ebp-24Ch]
    float a4a; // [esp+0h] [ebp-24Ch]
    tScriptStruct_CaisseManMissile* v7; // [esp+8h] [ebp-244h]
    float v8; // [esp+Ch] [ebp-240h]
    tGameObjectMain* v9; // [esp+14h] [ebp-238h]
    tGameObjectMain* v10; // [esp+1Ch] [ebp-230h]
    tGameObjectMain* v11; // [esp+20h] [ebp-22Ch]
    tGameObjectMain* v12; // [esp+24h] [ebp-228h]
    tGameObjectMain* v13; // [esp+28h] [ebp-224h]
    float v14; // [esp+34h] [ebp-218h]
    float v15; // [esp+38h] [ebp-214h]
    tGameObjectMain* v16; // [esp+50h] [ebp-1FCh]
    tGameObjectMain* v17; // [esp+54h] [ebp-1F8h]
    tGameObjectMain* v18; // [esp+58h] [ebp-1F4h]
    float v19; // [esp+5Ch] [ebp-1F0h]
    tGameObjectMain* v20; // [esp+68h] [ebp-1E4h]
    tGameObjectMain* v21; // [esp+70h] [ebp-1DCh]
    tGameObjectMain* v22; // [esp+78h] [ebp-1D4h]
    tGameObjectMain* v23; // [esp+88h] [ebp-1C4h]
    tGameObjectMain* v24; // [esp+90h] [ebp-1BCh]
    Vec3 v25; // [esp+94h] [ebp-1B8h] BYREF
    Vec3 dest; // [esp+A0h] [ebp-1ACh] BYREF
    BOOL v27; // [esp+ACh] [ebp-1A0h]
    Vec3 v28; // [esp+B0h] [ebp-19Ch] BYREF
    Vec3 a3; // [esp+BCh] [ebp-190h] BYREF
    float v30; // [esp+C8h] [ebp-184h]
    float v31; // [esp+CCh] [ebp-180h]
    float v33; // [esp+D4h] [ebp-178h]
    Vec3 v34; // [esp+D8h] [ebp-174h] BYREF
    float v36; // [esp+E8h] [ebp-164h]
    float v37; // [esp+ECh] [ebp-160h]
    int v38; // [esp+F0h] [ebp-15Ch]
    Vec3 v39; // [esp+F4h] [ebp-158h] BYREF
    float v40; // [esp+100h] [ebp-14Ch]
    Vec3 v41; // [esp+108h] [ebp-144h] BYREF
    Vec3 v42; // [esp+114h] [ebp-138h] BYREF
    float v43; // [esp+120h] [ebp-12Ch]
    Vec3 a1; // [esp+124h] [ebp-128h] BYREF
    Vec3 v45; // [esp+130h] [ebp-11Ch] BYREF
    int v46; // [esp+13Ch] [ebp-110h]
    BOOL v47; // [esp+140h] [ebp-10Ch]
    Vec3 v48; // [esp+144h] [ebp-108h] BYREF
    Vec3 v49; // [esp+150h] [ebp-FCh] BYREF
    int v50; // [esp+15Ch] [ebp-F0h]
    int v51; // [esp+160h] [ebp-ECh]
    Vec3 v52; // [esp+164h] [ebp-E8h] BYREF
    Vec3 v53; // [esp+170h] [ebp-DCh] BYREF
    Vec3 v54; // [esp+17Ch] [ebp-D0h] BYREF
    int v55; // [esp+188h] [ebp-C4h]
    float v56; // [esp+18Ch] [ebp-C0h]
    float v57; // [esp+190h] [ebp-BCh]
    Vec3 v58; // [esp+194h] [ebp-B8h] BYREF
    Vec3 v59; // [esp+1A0h] [ebp-ACh] BYREF
    float v60; // [esp+1ACh] [ebp-A0h]
    Vec3 v61; // [esp+1B0h] [ebp-9Ch] BYREF
    Vec3 a2; // [esp+1BCh] [ebp-90h] BYREF
    Vec3 v63; // [esp+1C8h] [ebp-84h] BYREF
    float v64; // [esp+1D4h] [ebp-78h]
    Vec3 v65; // [esp+1E4h] [ebp-68h] BYREF
    int v66; // [esp+1F0h] [ebp-5Ch]
    Vec3 v68; // [esp+1F8h] [ebp-54h]
    tScriptStruct_CaisseManMissile* v69; // [esp+204h] [ebp-48h]
    Vec3 v70; // [esp+208h] [ebp-44h] BYREF
    float v71; // [esp+214h] [ebp-38h]
    int v72; // [esp+21Ch] [ebp-30h]
    BOOL v73; // [esp+220h] [ebp-2Ch]
    Vec3 a5; // [esp+224h] [ebp-28h] BYREF
    Vec3 v75; // [esp+230h] [ebp-1Ch] BYREF
    int v76; // [esp+23Ch] [ebp-10h]
    float v77; // [esp+244h] [ebp-8h]
    int v78; // [esp+248h] [ebp-4h]

    v53.x = 1.0;
    v53.y = 0.0;
    v53.z = 0.0;
    v41.x = 1.0;
    v41.y = 0.0;
    v41.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_CaisseManMissile*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    v69 = v7;
    v30 = 0.0;
    v77 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Caisse_Man_Missile_CMM_comportement_Caisse_Man_Missile_CMM_disparition = (int)AI_ul_RealResolveOneRefFunction(0x72000B2Au);
        Caisse_Man_Missile_CMM_comportement__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Caisse_Man_Missile_CMM_comportement_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Caisse_Man_Missile_CMM_comportement_univVars_Perso_ID_actor,
            (void*)Caisse_Man_Missile_CMM_comportement_univVars,
            988);
        fct_Caisse_Man_Missile_CMM_comportement_Caisse_Man_Missile_CMM_explosion = (int)AI_ul_RealResolveOneRefFunction(0x72000B1Fu);
    }
    if (v69->i_mode_suivi)
    {
        v24 = (tGameObjectMain*)v69->o_target;
        if (v24)
        {
            if ((unsigned int)v24 <= 2)
                v24 = AI_MainActor[(uint32_t)v24 - 1];
            v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v24);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
        }
        v40 = (float)v0;
        if (v40 <= 6.0)
        {
            v60 = TIM_gf_dt;
            v69->v_dest_pos.z = v69->v_dest_pos.z - 2.0f * TIM_gf_dt;
        }
        else
        {
            v23 = (tGameObjectMain*)v69->o_target;
            if (v23)
            {
                if ((unsigned int)v23 <= 2)
                    v23 = AI_MainActor[(uint32_t)v23 - 1];
                AI_EvalFunc_OBJGetPos_C(v23, &v69->v_dest_pos);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v69->v_dest_pos);
            }
        }
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v28);
    MATH_SubstractVec(&dest, &v69->v_dest_pos, &v28);
    a3 = dest;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v49);
    if (v69->i_beluga)
    {
        v22 = (tGameObjectMain*)v69->o_mon_papa;
        if (v22)
        {
            if ((unsigned int)v22 <= 2)
                v22 = AI_MainActor[(uint32_t)v22 - 1];
            v1 = AI_EvalFunc_OBJ_CapaTest_C(v22, 0x100);
        }
        else
        {
            v1 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x100);
        }
        v73 = v1;
        if (v1)
            goto LABEL_108;
        if (!AI_EvalFunc_OBJ_CapaTest_C_CURRENT(1))
        {
            v21 = (tGameObjectMain*)v69->o_target;
            if (v21)
            {
                if ((unsigned int)v21 <= 2)
                    v21 = AI_MainActor[(uint32_t)v21 - 1];
                AI_EvalFunc_OBJGetPos_C(v21, &v59);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v59);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v70);
            MATH_SubstractVec(&v39, &v59, &v70);
            v20 = (tGameObjectMain*)v69->o_target;
            if (v20)
            {
                if ((unsigned int)v20 <= 2)
                    v20 = AI_MainActor[(uint32_t)v20 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v20, &v54);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v54);
            }
            v33 = v54.z * v39.z + v54.y * v39.y + v39.x * v54.x;
            if (v33 > 0.0)
            {
            LABEL_108:
                v69->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
                AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
                AI_EvalFunc_AITrackStop_C(1);
                v55 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Caisse_Man_Missile_CMM_comportement_Caisse_Man_Missile_CMM_disparition);
                if (v55 != 1000)
                    return v55;
            }
        }
    }
    else if (a3.z > -0.5)
    {
        a3.z = -0.5;
    }
    v77 = MATH_VecLen(&a3);
    v19 = 1.0f / v77;
    a3.x = a3.x * v19;
    a3.y = a3.y * v19;
    a3.z = a3.z * v19;
    if (v77 < 2.5)
    {
        v18 = (tGameObjectMain*)v69->des_o_target;
        if (v69->des_o_target)
        {
            if ((unsigned int)v18 <= 2)
                v18 = AI_MainActor[(uint32_t)v18 - 1];
            v17 = v18;
        }
        else
        {
            v17 = 0;
        }
        if (v17 == (tGameObjectMain*)Caisse_Man_Missile_CMM_comportement__PJ_Sally_gao
            && (v46 = AI_EvalFunc_ACTDesignFlagsGet_C((tGameObjectMain*)Caisse_Man_Missile_CMM_comportement__PJ_Sally_gao),
                (v46 & 0x2000) != 0))
        {
            v69->i_plantage_missile = 1;
        }
        else
        {
            v16 = (tGameObjectMain*)v69->des_o_target;
            if (v69->des_o_target)
            {
                if ((unsigned int)v16 <= 2)
                    v16 = AI_MainActor[(uint32_t)v16 - 1];
                v3 = AI_EvalFunc_OBJ_CapaTest_C(v16, 0x200);
            }
            else
            {
                v3 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x200);
            }
            v27 = v3;
            if (v3)
                v69->i_plantage_missile = 1;
        }
    }
    if (v69->i_flag_preset)
    {
        v30 = 6.0;
        v69->des_f_rotation_speed = 3.0;
    }
    else
    {
        v30 = v69->des_f_traction;
    }
    if (!v69->i_beluga || !(v47 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(1)))
    {
        if (v69->i_plantage_missile)
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v58);
            v42.x = 0.0;
            v42.y = 0.0;
            v42.z = -1.0;
            v43 = TIM_gf_dt;
            a4a = v69->des_f_rotation_speed * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlendRotate_C(&v58, &v42, a4a, &v63);
            v41 = v63;
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
            v57 = TIM_gf_dt;
            a4 = v69->des_f_rotation_speed * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
            v41 = a5;
        }
        AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v41);
    }
    v31 = TIM_gf_dt;
    v15 = 2.0f * TIM_gf_dt;
    if (v15 < 0.0)
        v15 = 0.0;
    if (v15 > 1.0)
        v15 = 1.0;
    v71 = v30 * v15 + v69->des_f_init_speed * (1.0f - v15);
    v69->des_f_init_speed = v71;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v48);
    v14 = v69->des_f_init_speed;
    v68.x = v48.x * v14;
    v68.y = v48.y * v14;
    v68.z = v48.z * v14;
    v53 = v68;
    v51 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v53);
    if (v69->GFX_Halo != -1)
    {
        if (v69->i_beluga)
        {
            v56 = AI_EvalFunc_MATHRandFloat_C(0.0, 1.0);
            v72 = AI_EvalFunc_COLORBlend_C(0xC072A2FA, 0xC053F8E3, v56);
            v69->c_color = v72;
        }
        else
        {
            v36 = AI_EvalFunc_MATHRandFloat_C(0.0, 1.0);
            v78 = AI_EvalFunc_COLORBlend_C(0x8080FF80, 0x80408040, v36);
            v69->c_color = v78;
        }
        AI_EvalFunc_GFXSeti_C_CURRENT(v69->GFX_Halo, 21101, v69->c_color);
    }
    if (v69->i_beluga)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v61);
        AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v25);
        v37 = TIM_gf_dt;
        v65.x = v25.x * TIM_gf_dt;
        v65.y = v25.y * TIM_gf_dt;
        v65.z = v25.z * TIM_gf_dt;
        v4 = AI_EvalFunc_COLRayObjectVector_C(AI_gpst_CurrentGameObject, &v61, &v65, -1, 0, 0, 1);
        v69->o_paf = (int)v4;
        v13 = (tGameObjectMain*)v69->o_paf;
        if (v13)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            v12 = v13;
        }
        else
        {
            v12 = 0;
        }
        if (v12)
        {
            v11 = (tGameObjectMain*)v69->o_paf;
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
            if (v10 == *(tGameObjectMain**)(Caisse_Man_Missile_CMM_comportement_univVars_Perso_ID_actor + 40))
            {
                v9 = (tGameObjectMain*)v69->o_paf;
                if (v9)
                {
                    if ((unsigned int)v9 <= 2)
                        v9 = AI_MainActor[(uint32_t)v9 - 1];
                    AI_EvalFunc_OBJGetPos_C(v9, &v52);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v52);
                }
                AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v45);
                v64 = TIM_gf_dt;
                v8 = 15.0f * TIM_gf_dt;
                v34.x = v45.x * v8;
                v34.y = v45.y * v8;
                v34.z = v45.z * v8;
                MATH_VecAddVec(&a1, &v52, &v34);
                AI_EvalFunc_OBJSetPos_C_CURRENT(&a1);
            }
            AI_EvalFunc_COLRayPosGet_C(AI_gpst_CurrentGameObject, &v75);
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v75);
            v66 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Caisse_Man_Missile_CMM_comportement_Caisse_Man_Missile_CMM_explosion);
            if (v66 != 1000)
                return v66;
        }
    }
    else
    {
        if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(2))
        {
            if (!AI_EvalFunc_TIMEElapsed_C(v69->f_time_start, 0.40000001f))
            {
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                return 1000;
            }
        }
        v76 = AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x80);
        if (v76 || (v38 = AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x40)) != 0)
        {
            v50 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Caisse_Man_Missile_CMM_comportement_Caisse_Man_Missile_CMM_explosion);
            if (v50 != 1000)
                return v50;
        }
    }
    return 1000;
}

int Caisse_Man_Missile_CMM_init()
{
    BOOL v0; // eax
    tGameObjectMain* v1; // eax
    int result; // eax
    float v3; // [esp+0h] [ebp-144h]
    tScriptStruct_CaisseManMissile* v4; // [esp+4h] [ebp-140h]
    tGameObjectMain* v5; // [esp+Ch] [ebp-138h]
    tGameObjectMain* v6; // [esp+14h] [ebp-130h]
    tGameObjectMain* v7; // [esp+1Ch] [ebp-128h]
    tGameObjectMain* v8; // [esp+24h] [ebp-120h]
    tGameObjectMain* v9; // [esp+2Ch] [ebp-118h]
    tGameObjectMain* v10; // [esp+34h] [ebp-110h]
    float v11; // [esp+38h] [ebp-10Ch]
    tGameObjectMain* v12; // [esp+40h] [ebp-104h]
    tGameObjectMain* v13; // [esp+44h] [ebp-100h]
    tGameObjectMain* v14; // [esp+54h] [ebp-F0h]
    tGameObjectMain* v15; // [esp+58h] [ebp-ECh]
    tGameObjectMain* v16; // [esp+5Ch] [ebp-E8h]
    tGameObjectMain* v17; // [esp+64h] [ebp-E0h]
    tGameObjectMain* v18; // [esp+80h] [ebp-C4h]
    tGameObjectMain* v19; // [esp+84h] [ebp-C0h]
    tGameObjectMain* v20; // [esp+8Ch] [ebp-B8h]
    BOOL v21; // [esp+A0h] [ebp-A4h]
    Vec3 v22; // [esp+A4h] [ebp-A0h] BYREF
    BOOL v23; // [esp+BCh] [ebp-88h]
    int v24; // [esp+C0h] [ebp-84h]
    void* Src; // [esp+C8h] [ebp-7Ch]
    Vec3 v26; // [esp+CCh] [ebp-78h] BYREF
    void* v27; // [esp+DCh] [ebp-68h]
    BOOL v28; // [esp+E0h] [ebp-64h]
    unsigned int v29; // [esp+E4h] [ebp-60h]
    int v30; // [esp+E8h] [ebp-5Ch]
    tMessage v31; // [esp+ECh] [ebp-58h] BYREF
    int v32; // [esp+124h] [ebp-20h]
    tScriptStruct_CaisseManMissile* v33; // [esp+128h] [ebp-1Ch]
    tGameObjectMain* v34; // [esp+12Ch] [ebp-18h]
    tGameObjectMain* v35; // [esp+130h] [ebp-14h]
    BOOL v36; // [esp+138h] [ebp-Ch]
    int v37; // [esp+13Ch] [ebp-8h]
    tGameObjectMain* v38; // [esp+140h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_CaisseManMissile*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v33 = v4;
    v34 = 0;
    v35 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Caisse_Man_Missile_CMM_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Caisse_Man_Missile_CMM_init_GST_Global_gao_i_nb_SFX_Explosion_Fire_SND,
            (void*)Caisse_Man_Missile_CMM_init_GST_Global_gao,
            4640);
        fctvar_Caisse_Man_Missile_CMM_init_UNKNOWN = AI_ul_RealResolveOneRef(42, 0x5E000320u);
        Caisse_Man_Missile_CMM_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Caisse_Man_Missile_CMM_init_univVars_Perso_ID_actor,
            (void*)Caisse_Man_Missile_CMM_init_univVars,
            988);
        Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        fct_Caisse_Man_Missile_CMM_init_Caisse_Man_Missile_CMM_afterrec = (int)AI_ul_RealResolveOneRefFunction(0x72000B28u);
        fct_Caisse_Man_Missile_CMM_init_Caisse_Man_Missile_CMM_comportement = (int)AI_ul_RealResolveOneRefFunction(0x72000B1Du);
        fct_Caisse_Man_Missile_CMM_init_Caisse_Man_Missile_CMM_reflex = (int)AI_ul_RealResolveOneRefFunction(0x72000B1Eu);
    }
    v23 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
    v32 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 16781316, 0);
    v29 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x10000000, 0);
    if (*(int*)Caisse_Man_Missile_CMM_init_GST_Global_gao_i_nb_SFX_Explosion_Fire_SND < 5)
    {
        v33->i_flag_play_generation_snd = 1;
        ++* (uint32_t*)Caisse_Man_Missile_CMM_init_GST_Global_gao_i_nb_SFX_Explosion_Fire_SND;
        AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
    }
    v33->i_sound = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18001000);
    AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v33->i_sound);
    Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v31, Src, sizeof(v31));
    while (v31.msgID)
    {
        v35 = v31.gao;
        v20 = v31.gao;
        if (v31.gao)
        {
            if (v31.gao <= (tGameObjectMain*)2)
                v20 = AI_MainActor[(int)v31.gao - 1];
            v0 = AI_EvalFunc_AIIsModel_C(v20, (tAIModel*)fctvar_Caisse_Man_Missile_CMM_init_UNKNOWN);
        }
        else
        {
            v0 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_Caisse_Man_Missile_CMM_init_UNKNOWN);
        }
        v36 = v0;
        v33->i_flag_preset = v0;
        if (v31.msgID == 1)
        {
            v33->i_mode_suivi = 1;
            v33->i_mode_test_PAF = 0;
            v19 = v31.gao2;
            if (v31.gao2)
            {
                if (v31.gao2 <= (tGameObjectMain*)2)
                    v19 = AI_MainActor[(int)v31.gao2 - 1];
                v18 = v19;
            }
            else
            {
                v18 = 0;
            }
            if (v18)
                v33->des_o_target = (int)v31.gao2;
            if (v31.f2 >= 0.0)
                v33->des_f_rotation_speed = v31.f2;
            if (v31.f1 >= 0.0)
                v33->des_f_init_speed = v31.f1;
            if (v31.Int1 >= 0)
                v33->des_f_life_time = (float)v31.Int1;
            if (v31.Int2 >= 0)
                v33->des_f_traction = (float)v31.Int2;
            if (fabs(v31.v1.x) != 0.0f)
            {
                v21 = 0;
            }
            else if (fabs(v31.v1.y) != 0.0f)
            {
                v21 = 0;
            }
            else
            {
                v21 = fabs(v31.v1.z) == 0.0f;
            }
            if (!v21)
                v33->des_v_gravity = v31.v1;
            if (fabs(v31.v2.x) != 0.0f)
            {
                v28 = 0;
            }
            else if (fabs(v31.v2.y) != 0.0f)
            {
                v28 = 0;
            }
            else
            {
                v28 = fabs(v31.v2.z) == 0.0f;
            }
            if (!v28)
                v33->des_v_friction = v31.v2;
            v33->o_mon_papa = (int)v31.gao;
            v17 = (tGameObjectMain*)v33->des_o_target;
            if (v33->des_o_target)
            {
                if ((unsigned int)v17 <= 2)
                    v17 = AI_MainActor[(uint32_t)v17 - 1];
                v1 = AI_EvalFunc_ANICanalGaoGet_C(v17, 0);
            }
            else
            {
                v1 = AI_EvalFunc_ANICanalGaoGet_C(0, 0);
            }
            v34 = v1;
            v16 = v1;
            if (v1)
            {
                if ((unsigned int)v1 <= 2)
                    v16 = AI_MainActor[(uint32_t)v1 - 1];
                v15 = v16;
            }
            else
            {
                v15 = 0;
            }
            if (v15)
                v33->o_target = (int)v34;
            else
                v33->o_target = v33->des_o_target;
            v14 = (tGameObjectMain*)v33->o_target;
            if (v14)
            {
                if ((unsigned int)v14 <= 2)
                    v14 = AI_MainActor[(uint32_t)v14 - 1];
                AI_EvalFunc_OBJGetPos_C(v14, &v33->v_dest_pos);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v33->v_dest_pos);
            }
        }
        else if (v31.msgID == 2)
        {
            v33->i_mode_suivi = 0;
            v33->i_mode_test_PAF = v31.Int1;
            v33->v_dest_pos = v31.v1;
            v33->des_drop = 3;
        }
        v27 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v31, v27, sizeof(v31));
    }
    v33->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
    v13 = (tGameObjectMain*)v33->des_o_target;
    if (v33->des_o_target)
    {
        if ((unsigned int)v13 <= 2)
            v13 = AI_MainActor[(uint32_t)v13 - 1];
        v12 = v13;
    }
    else
    {
        v12 = 0;
    }
    if (v12 == *(tGameObjectMain**)(Caisse_Man_Missile_CMM_init_univVars_Perso_ID_actor + 40))
        v33->i_beluga = 1;
    AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v33->des_v_gravity);
    v37 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v33->des_v_friction);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v26);
    v11 = v33->des_f_init_speed;
    v22.x = v26.x * v11;
    v22.y = v26.y * v11;
    v22.z = v26.z * v11;
    v24 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v22);
    v33->GFX_Halo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
    AI_EvalFunc_GFXFlagSet_C_CURRENT(v33->GFX_Halo, 4, 1);
    GFXSetCurGlobal(v33->GFX_Halo);
    AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
    AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
    if (v33->i_beluga)
    {
        v10 = (tGameObjectMain*)Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao;
        if (Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao)
        {
            if ((unsigned int)Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao <= 2)
                v10 = AI_MainActor[Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v33->GFX_Halo, v10, 29);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v33->GFX_Halo, 0, 29);
        }
        v3 = v33->f_size / 5.0f;
        AI_EvalFunc_GFXSetf_C_CURRENT(v33->GFX_Halo, 21000, v3);
    }
    else
    {
        v9 = (tGameObjectMain*)Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao;
        if (Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao)
        {
            if ((unsigned int)Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao <= 2)
                v9 = AI_MainActor[Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v33->GFX_Halo, v9, 0);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v33->GFX_Halo, 0, 0);
        }
        AI_EvalFunc_GFXSetf_C_CURRENT(v33->GFX_Halo, 21000, 2.0);
    }
    if (!v33->i_beluga)
    {
        v33->GFX_Lightning_1 = AI_EvalFunc_GFXAdd_C_CURRENT(1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v33->GFX_Lightning_1, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v33->GFX_Lightning_1, 2, 1);
        v8 = (tGameObjectMain*)Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao;
        if (Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao)
        {
            if ((unsigned int)Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao <= 2)
                v8 = AI_MainActor[Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v33->GFX_Lightning_1, v8, 15);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v33->GFX_Lightning_1, 0, 15);
        }
        GFXSetCurGlobal(v33->GFX_Lightning_1);
        AI_EvalFunc_GFXSetf_C_CURGLOB(1000, 0.1f);
        AI_EvalFunc_GFXSeti_C_CURGLOB(1100, 671098880);
        v33->GFX_Lightning_2 = AI_EvalFunc_GFXAdd_C_CURRENT(1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v33->GFX_Lightning_2, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v33->GFX_Lightning_2, 2, 1);
        v7 = (tGameObjectMain*)Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao;
        if (Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao)
        {
            if ((unsigned int)Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao <= 2)
                v7 = AI_MainActor[Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v33->GFX_Lightning_2, v7, 15);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v33->GFX_Lightning_2, 0, 15);
        }
        GFXSetCurGlobal(v33->GFX_Lightning_2);
        AI_EvalFunc_GFXSetf_C_CURGLOB(1000, 0.1f);
        AI_EvalFunc_GFXSeti_C_CURGLOB(1100, 0x28002800);
        v33->GFX_Lightning_3 = AI_EvalFunc_GFXAdd_C_CURRENT(1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v33->GFX_Lightning_3, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v33->GFX_Lightning_3, 2, 1);
        v6 = (tGameObjectMain*)Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao;
        if (Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao)
        {
            if ((unsigned int)Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao <= 2)
                v6 = AI_MainActor[Caisse_Man_Missile_CMM_init_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v33->GFX_Lightning_3, v6, 15);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v33->GFX_Lightning_3, 0, 15);
        }
        GFXSetCurGlobal(v33->GFX_Lightning_3);
        AI_EvalFunc_GFXSetf_C_CURGLOB(1000, 0.1f);
        AI_EvalFunc_GFXSeti_C_CURGLOB(1100, 0x28002800);
    }
    v38 = AI_gpst_CurrentGameObject;
    v5 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v5 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v5,
            2,
            (tAIFunction*)fct_Caisse_Man_Missile_CMM_init_Caisse_Man_Missile_CMM_afterrec);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            2,
            (tAIFunction*)fct_Caisse_Man_Missile_CMM_init_Caisse_Man_Missile_CMM_afterrec);
    }
    AI_EvalFunc_AITrackChange_C(2, (tAIFunction*)fct_Caisse_Man_Missile_CMM_init_Caisse_Man_Missile_CMM_comportement);
    v30 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Caisse_Man_Missile_CMM_init_Caisse_Man_Missile_CMM_reflex);
    if (v30 == 1000)
        result = 1000;
    else
        result = v30;
    return result;
}

int Caisse_Man_Missile_CMM_reflex()
{
    BOOL v0; // eax
    tScriptStruct_CaisseManMissile* v2; // [esp+8h] [ebp-24h]
    tGameObjectMain* v3; // [esp+10h] [ebp-1Ch]
    float v4; // [esp+14h] [ebp-18h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_CaisseManMissile*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Caisse_Man_Missile_CMM_reflex_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Caisse_Man_Missile_CMM_reflex_GST_Global_gao_i_nb_SFX_Explosion_Fire_SND,
            (void*)Caisse_Man_Missile_CMM_reflex_GST_Global_gao,
            4640);
        fct_Caisse_Man_Missile_CMM_reflex_Caisse_Man_Missile_CMM_disparition = (int)AI_ul_RealResolveOneRefFunction(0x72000B2Au);
    }
    if (v2->i_flag_play_generation_snd && AI_EvalFunc_TIMEElapsed_C(v2->f_time_start, 0.40000001f))
    {
        v2->i_flag_play_generation_snd = 0;
        --* (uint32_t*)Caisse_Man_Missile_CMM_reflex_GST_Global_gao_i_nb_SFX_Explosion_Fire_SND;
    }
    if (v2->des_f_life_time > 0.0 && AI_EvalFunc_TIMEElapsed_C(v2->f_time_start, v2->des_f_life_time))
    {
        v2->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        AI_EvalFunc_AITrackChange_C(2, (tAIFunction*)fct_Caisse_Man_Missile_CMM_reflex_Caisse_Man_Missile_CMM_disparition);
        AI_EvalFunc_AITrackCurStop_C();
    }
    v3 = (tGameObjectMain*)v2->o_mon_papa;
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        v0 = AI_EvalFunc_OBJ_CapaTest_C(v3, 1);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_CapaTest_C(0, 1);
    }
    if (v0)
    {
        v4 = TIM_gf_MainClock - TIM_gf_LockTime;
        v2->f_time_start = v4 - 10.0f;
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        AI_EvalFunc_AITrackChange_C(2, (tAIFunction*)fct_Caisse_Man_Missile_CMM_reflex_Caisse_Man_Missile_CMM_disparition);
        AI_EvalFunc_AITrackCurStop_C();
    }
    return 1000;
}

int Caisse_Man_Missile_CMM_afterrec()
{
    float v1; // [esp+0h] [ebp-154h]
    float v2; // [esp+0h] [ebp-154h]
    float v3; // [esp+0h] [ebp-154h]
    float v4; // [esp+4h] [ebp-150h]
    float v5; // [esp+4h] [ebp-150h]
    float v6; // [esp+4h] [ebp-150h]
    tScriptStruct_CaisseManMissile* v7; // [esp+8h] [ebp-14Ch]
    Vec3 a3; // [esp+Ch] [ebp-148h] BYREF
    float v9; // [esp+18h] [ebp-13Ch]
    Vec3 v10; // [esp+1Ch] [ebp-138h] BYREF
    Vec3 v11; // [esp+28h] [ebp-12Ch] BYREF
    float v12; // [esp+34h] [ebp-120h]
    float v13; // [esp+38h] [ebp-11Ch]
    Vec3 v14; // [esp+3Ch] [ebp-118h] BYREF
    Vec3 v15; // [esp+48h] [ebp-10Ch] BYREF
    Vec3 v16; // [esp+54h] [ebp-100h] BYREF
    Vec3 v17; // [esp+60h] [ebp-F4h] BYREF
    Vec3 v18; // [esp+6Ch] [ebp-E8h] BYREF
    Vec3 v19; // [esp+78h] [ebp-DCh] BYREF
    Vec3 v20; // [esp+84h] [ebp-D0h] BYREF
    Vec3 v21; // [esp+90h] [ebp-C4h] BYREF
    Vec3 v22; // [esp+9Ch] [ebp-B8h] BYREF
    Vec3 a1; // [esp+A8h] [ebp-ACh] BYREF
    Vec3 v24; // [esp+B4h] [ebp-A0h] BYREF
    Vec3 v25; // [esp+C0h] [ebp-94h] BYREF
    Vec3 v26; // [esp+CCh] [ebp-88h] BYREF
    Vec3 v27; // [esp+D8h] [ebp-7Ch] BYREF
    float v28; // [esp+E4h] [ebp-70h]
    float v29; // [esp+E8h] [ebp-6Ch]
    Vec3 v30; // [esp+ECh] [ebp-68h] BYREF
    Vec3 v31; // [esp+F8h] [ebp-5Ch] BYREF
    Vec3 a2; // [esp+104h] [ebp-50h] BYREF
    Vec3 v33; // [esp+110h] [ebp-44h] BYREF
    tScriptStruct_CaisseManMissile* v34; // [esp+11Ch] [ebp-38h]
    Vec3 v35; // [esp+120h] [ebp-34h] BYREF
    Vec3 v36; // [esp+12Ch] [ebp-28h] BYREF
    float v37; // [esp+138h] [ebp-1Ch]
    Vec3 v38; // [esp+13Ch] [ebp-18h] BYREF
    Vec3 v39; // [esp+148h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_CaisseManMissile*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    v34 = v7;
    if (v7->GFX_Halo != -1)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v15);
        AI_EvalFunc_GFXSetv_C_CURRENT(v34->GFX_Halo, 21200, &v15);
    }
    if (v34->GFX_Lightning_1 != -1)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
        v29 = AI_EvalFunc_MATHRandFloat_C(v34->f_amplitude_min, v34->f_amplitude_max);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v11);
        a3.x = v11.x * v29;
        a3.y = v11.y * v29;
        a3.z = v11.z * v29;
        MATH_VecAddVec(&a1, &a2, &a3);
        GFXSetCurGlobal(v34->GFX_Lightning_1);
        AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &a1);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v36);
        v4 = -v34->f_amplitude_min;
        v1 = -v34->f_amplitude_max;
        v13 = AI_EvalFunc_MATHRandFloat_C(v1, v4);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v27);
        v35.x = v27.x * v13;
        v35.y = v27.y * v13;
        v35.z = v27.z * v13;
        MATH_VecAddVec(&v16, &v36, &v35);
        AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &v16);
    }
    if (v34->GFX_Lightning_2 != -1)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v26);
        v12 = AI_EvalFunc_MATHRandFloat_C(v34->f_amplitude_min, v34->f_amplitude_max);
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v39);
        v25.x = v39.x * v12;
        v25.y = v39.y * v12;
        v25.z = v39.z * v12;
        MATH_VecAddVec(&v14, &v26, &v25);
        GFXSetCurGlobal(v34->GFX_Lightning_2);
        AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &v14);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v20);
        v5 = -v34->f_amplitude_min;
        v2 = -v34->f_amplitude_max;
        v9 = AI_EvalFunc_MATHRandFloat_C(v2, v5);
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v19);
        v31.x = v19.x * v9;
        v31.y = v19.y * v9;
        v31.z = v19.z * v9;
        MATH_VecAddVec(&v24, &v20, &v31);
        AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &v24);
    }
    if (v34->GFX_Lightning_3 != -1)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v38);
        v28 = AI_EvalFunc_MATHRandFloat_C(v34->f_amplitude_min, v34->f_amplitude_max);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v18);
        v17.x = v18.x * v28;
        v17.y = v18.y * v28;
        v17.z = v18.z * v28;
        MATH_VecAddVec(&v30, &v38, &v17);
        GFXSetCurGlobal(v34->GFX_Lightning_3);
        AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &v30);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v10);
        v6 = -v34->f_amplitude_min;
        v3 = -v34->f_amplitude_max;
        v37 = AI_EvalFunc_MATHRandFloat_C(v3, v6);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v21);
        v33.x = v21.x * v37;
        v33.y = v21.y * v37;
        v33.z = v21.z * v37;
        MATH_VecAddVec(&v22, &v10, &v33);
        AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &v22);
    }
    return 1000;
}