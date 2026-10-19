#include "ai_compiledscripts_inc.h"

//Ascenseur_Nazh_Asc_Nazh_exec_comp_status
int Ascenseur_Nazh_Asc_Nazh_exec_comp_status_univVars;
int Ascenseur_Nazh_Asc_Nazh_exec_comp_status_univVars_Perso_ID_actor;
int Ascenseur_Nazh_Asc_Nazh_exec_comp_status_univVars_Perso_ID_world_ID;
int Ascenseur_Nazh_Asc_Nazh_exec_comp_status_univVars_world_ID;
int Ascenseur_Nazh_Asc_Nazh_exec_comp_status_univVars_Perso_ID_status;

//Ascenseur_Nazh_Asc_Nazh_init
int fct_Ascenseur_Nazh_Asc_Nazh_init_Ascenseur_Nazh_Asc_Nazh_exec_GFX;
int fct_Ascenseur_Nazh_Asc_Nazh_init_Ascenseur_Nazh_Asc_Nazh_exec_sound;
int fct_Ascenseur_Nazh_Asc_Nazh_init_Ascenseur_Nazh_Asc_Nazh_mode_wait;

//Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally
int fct_Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_Ascenseur_Nazh_Asc_Nazh_exec_comp_status;
int Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao;
int Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao_o_joy_acteur;
int Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao_f_joy_min;
int Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao__PJ_Sally_gao;
int fct_Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_Ascenseur_Nazh_Asc_Nazh_mode_transfert_companion;

//Ascenseur_Nazh_Asc_Nazh_exec_GFX
int Ascenseur_Nazh_Asc_Nazh_exec_GFX_SFX_light_smoke_gao;

//Ascenseur_Nazh_Asc_Nazh_mode_wait
int Ascenseur_Nazh_Asc_Nazh_mode_wait__PJ_Sally_gao;
int fct_Ascenseur_Nazh_Asc_Nazh_mode_wait_Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally;

//Ascenseur_Nazh_Asc_Nazh_mode_transfert_companion
int fct_Ascenseur_Nazh_Asc_Nazh_mode_transfert_companion_Ascenseur_Nazh_Asc_Nazh_exec_comp_status;
int Ascenseur_Nazh_Asc_Nazh_mode_transfert_companion_univVars;
int Ascenseur_Nazh_Asc_Nazh_mode_transfert_companion_univVars_Perso_ID_actor;   
int fct_Ascenseur_Nazh_Asc_Nazh_mode_transfert_companion_Ascenseur_Nazh_Asc_Nazh_mode_wait;

int Ascenseur_Nazh_Asc_Nazh_exec_comp_status()
{
    BOOL v0; // eax
    tScriptStruct_AscenseurNazh* v2; // [esp+0h] [ebp-B8h]
    tGameObjectMain* v3; // [esp+8h] [ebp-B0h]
    tGameObjectMain* v4; // [esp+10h] [ebp-A8h]
    tGameObjectMain* v5; // [esp+14h] [ebp-A4h]
    tGameObjectMain* v6; // [esp+18h] [ebp-A0h]
    tGameObjectMain* v7; // [esp+1Ch] [ebp-9Ch]
    tGameObjectMain* v8; // [esp+20h] [ebp-98h]
    tGameObjectMain* v9; // [esp+28h] [ebp-90h]
    tGameObjectMain* v10; // [esp+30h] [ebp-88h]
    tGameObjectMain* v11; // [esp+34h] [ebp-84h]
    tGameObjectMain* v12; // [esp+38h] [ebp-80h]
    unsigned int v13; // [esp+4Ch] [ebp-6Ch]
    tMessage Src; // [esp+54h] [ebp-64h] BYREF
    tScriptStruct_AscenseurNazh* v15; // [esp+ACh] [ebp-Ch]
    int i; // [esp+B0h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_AscenseurNazh*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v15 = v2;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Ascenseur_Nazh_Asc_Nazh_exec_comp_status_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Ascenseur_Nazh_Asc_Nazh_exec_comp_status_univVars_Perso_ID_actor,
            (void*)Ascenseur_Nazh_Asc_Nazh_exec_comp_status_univVars,
            988);
        AI2C_ResolveBuffer(
            &Ascenseur_Nazh_Asc_Nazh_exec_comp_status_univVars_Perso_ID_world_ID,
            (void*)Ascenseur_Nazh_Asc_Nazh_exec_comp_status_univVars,
            536);
        AI2C_ResolveBuffer(
            &Ascenseur_Nazh_Asc_Nazh_exec_comp_status_univVars_world_ID,
            (void*)Ascenseur_Nazh_Asc_Nazh_exec_comp_status_univVars,
            4);
        AI2C_ResolveBuffer(
            &Ascenseur_Nazh_Asc_Nazh_exec_comp_status_univVars_Perso_ID_status,
            (void*)Ascenseur_Nazh_Asc_Nazh_exec_comp_status_univVars,
            484);
    }
    for (i = 0; i <= 4; ++i)
    {
        v13 = *(uint32_t*)(Ascenseur_Nazh_Asc_Nazh_exec_comp_status_univVars_Perso_ID_actor + 4 * i + 4);
        v12 = (tGameObjectMain*)v13;
        if (v13)
        {
            if (v13 <= 2)
                v12 = AI_MainActor[v13 - 1];
            v11 = v12;
        }
        else
        {
            v11 = 0;
        }
        if (v11
            && *(uint32_t*)(Ascenseur_Nazh_Asc_Nazh_exec_comp_status_univVars_Perso_ID_world_ID + 4 * i + 4) == *(uint32_t*)Ascenseur_Nazh_Asc_Nazh_exec_comp_status_univVars_world_ID)
        {
            if (!v15->ai_companion_status[i])
            {
                v10 = *(tGameObjectMain**)(Ascenseur_Nazh_Asc_Nazh_exec_comp_status_univVars_Perso_ID_actor + 4 * i + 4);
                if (v13)
                {
                    if (v13 <= 2)
                        v10 = AI_MainActor[v13 - 1];
                    v0 = AI_EvalFunc_OBJ_CapaTest_C(v10, 2);
                }
                else
                {
                    v0 = AI_EvalFunc_OBJ_CapaTest_C(0, 2);
                }
                if (v0)
                {
                    v15->ai_companion_status[i] = 1;
                    continue;
                }
            }
            if (!*(uint32_t*)(Ascenseur_Nazh_Asc_Nazh_exec_comp_status_univVars_Perso_ID_status + 4 * i + 4)
                && v15->ai_companion_status[i] < 2)
            {
                v15->ai_companion_status[i] = 2;
                v9 = (tGameObjectMain*)v13;
                if (v13)
                {
                    if (v13 <= 2)
                        v9 = AI_MainActor[v13 - 1];
                    AI_EvalFunc_OBJ_CapaSet_C(v9, 2, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_CapaSet_C(0, 2, 0);
                }
                v8 = (tGameObjectMain*)v15->o_start_wp;
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
                v6 = (tGameObjectMain*)v15->o_wp_1;
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
                if (v7 == v5)
                {
                    Src.msgID = 6;
                    Src.Int1 = 1;
                    Src.Int2 = 0;
                    Src.f2 = 1000000.0;
                    Src.f1 = 20.0;
                    Src.v1.x = 0.0;
                    Src.v1.y = 0.0;
                    Src.v1.z = 0.0;
                    Src.v2.x = 0.0;
                    Src.v2.y = 0.0;
                    Src.v2.z = 0.0;
                    Src.gao = AI_gpst_CurrentGameObject;
                    Src.gao2 = (tGameObjectMain*)v15->o_wp_1_attente[i];
                    v4 = (tGameObjectMain*)v13;
                    if (!v13)
                        goto LABEL_44;
                    if (v13 <= 2)
                        v4 = AI_MainActor[v13 - 1];
                    AI_EvalFunc_MSGSend_C(v4, &Src);
                }
                else
                {
                    Src.msgID = 6;
                    Src.Int1 = 1;
                    Src.Int2 = 0;
                    Src.f2 = 1000000.0;
                    Src.f1 = 20.0;
                    Src.v1.x = 0.0;
                    Src.v1.y = 0.0;
                    Src.v1.z = 0.0;
                    Src.v2.x = 0.0;
                    Src.v2.y = 0.0;
                    Src.v2.z = 0.0;
                    Src.gao = AI_gpst_CurrentGameObject;
                    Src.gao2 = (tGameObjectMain*)v15->o_wp_2_attente[i];
                    v3 = (tGameObjectMain*)v13;
                    if (!v13)
                    {
                    LABEL_44:
                        AI_EvalFunc_MSGSend_C(0, &Src);
                        continue;
                    }
                    if (v13 <= 2)
                        v3 = AI_MainActor[v13 - 1];
                    AI_EvalFunc_MSGSend_C(v3, &Src);
                }
            }
        }
    }
    return 1000;
}

int Ascenseur_Nazh_Asc_Nazh_init()
{
    double v0; // st7
    int result; // eax
    tScriptStruct_AscenseurNazh* v2; // [esp+0h] [ebp-7Ch]
    tGameObjectMain* v3; // [esp+8h] [ebp-74h]
    tGameObjectMain* v4; // [esp+14h] [ebp-68h]
    tGameObjectMain* v5; // [esp+1Ch] [ebp-60h]
    tGameObjectMain* v6; // [esp+24h] [ebp-58h]
    tGameObjectMain* v7; // [esp+2Ch] [ebp-50h]
    Vec3 dest; // [esp+30h] [ebp-4Ch] BYREF
    Vec3 v9; // [esp+3Ch] [ebp-40h] BYREF
    int v10; // [esp+48h] [ebp-34h]
    Vec3 a3; // [esp+4Ch] [ebp-30h] BYREF
    Vec3 v12; // [esp+5Ch] [ebp-20h] BYREF
    Vec3 v13; // [esp+68h] [ebp-14h] BYREF
    tScriptStruct_AscenseurNazh* v14; // [esp+74h] [ebp-8h]
    tGameObjectMain* v15; // [esp+78h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_AscenseurNazh*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v14 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Ascenseur_Nazh_Asc_Nazh_init_Ascenseur_Nazh_Asc_Nazh_exec_GFX = (int)AI_ul_RealResolveOneRefFunction(0x72004381u);
        fct_Ascenseur_Nazh_Asc_Nazh_init_Ascenseur_Nazh_Asc_Nazh_exec_sound = (int)AI_ul_RealResolveOneRefFunction(0x870010AD);
        fct_Ascenseur_Nazh_Asc_Nazh_init_Ascenseur_Nazh_Asc_Nazh_mode_wait = (int)AI_ul_RealResolveOneRefFunction(0x72003816u);
    }
    v14->des_dist_sound_off = v14->des_dist_sound_off * v14->des_dist_sound_off;
    v7 = (tGameObjectMain*)v14->o_wp_1;
    if (v7)
    {
        if ((unsigned int)v7 <= 2)
            v7 = AI_MainActor[(uint32_t)v7 - 1];
        AI_EvalFunc_OBJGetPos_C(v7, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    v6 = (tGameObjectMain*)v14->o_wp_2;
    if (v6)
    {
        if ((unsigned int)v6 <= 2)
            v6 = AI_MainActor[(uint32_t)v6 - 1];
        AI_EvalFunc_OBJGetPos_C(v6, &v13);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v13);
    }
    if (a3.z <= (double)v13.z)
    {
        v14->o_up_wp = v14->o_wp_2;
        if (v14->des_WP_2_entree)
            v14->i_flag_particules_down = 1;
        v14->o_down_wp = v14->o_wp_1;
        if (v14->des_WP_1_entree)
            v14->i_flag_particules_up = 1;
    }
    else
    {
        v14->o_up_wp = v14->o_wp_1;
        if (v14->des_WP_1_entree)
            v14->i_flag_particules_down = 1;
        v14->o_down_wp = v14->o_wp_2;
        if (v14->des_WP_2_entree)
            v14->i_flag_particules_up = 1;
    }
    v14->f_GFX_wind_Z_speed = 20.0;
    v5 = (tGameObjectMain*)v14->o_up_wp;
    if (v5)
    {
        if ((unsigned int)v5 <= 2)
            v5 = AI_MainActor[(uint32_t)v5 - 1];
        AI_EvalFunc_OBJGetPos_C(v5, &v12);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v12);
    }
    v4 = (tGameObjectMain*)v14->o_down_wp;
    if (v4)
    {
        if ((unsigned int)v4 <= 2)
            v4 = AI_MainActor[(uint32_t)v4 - 1];
        AI_EvalFunc_OBJGetPos_C(v4, &v9);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v9);
    }
    MATH_SubstractVec(&dest, &v12, &v9);
    v0 = MATH_VecLen(&dest);
    v14->f_GFX_life_time = (float)v0;
    v14->f_GFX_life_time = v14->f_GFX_life_time / v14->f_GFX_wind_Z_speed;
    if (v14->des_GFX)
        AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_Ascenseur_Nazh_Asc_Nazh_init_Ascenseur_Nazh_Asc_Nazh_exec_GFX);
    v15 = AI_gpst_CurrentGameObject;
    v3 = AI_gpst_CurrentGameObject;
    if (AI_gpst_CurrentGameObject)
    {
        if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
            v3 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v3,
            2,
            (tAIFunction*)fct_Ascenseur_Nazh_Asc_Nazh_init_Ascenseur_Nazh_Asc_Nazh_exec_sound);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            2,
            (tAIFunction*)fct_Ascenseur_Nazh_Asc_Nazh_init_Ascenseur_Nazh_Asc_Nazh_exec_sound);
    }
    v10 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Ascenseur_Nazh_Asc_Nazh_init_Ascenseur_Nazh_Asc_Nazh_mode_wait);
    if (v10 == 1000)
        result = 1000;
    else
        result = v10;
    return result;
}

int Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally()
{
    double v1; // st7
    int v2; // eax
    int v3; // eax
    float X; // [esp+4h] [ebp-D8h]
    tScriptStruct_AscenseurNazh* v5; // [esp+8h] [ebp-D4h]
    tGameObjectMain* v6; // [esp+10h] [ebp-CCh]
    tGameObjectMain* v7; // [esp+14h] [ebp-C8h]
    tGameObjectMain* v8; // [esp+18h] [ebp-C4h]
    tGameObjectMain* v9; // [esp+20h] [ebp-BCh]
    tGameObjectMain* v10; // [esp+28h] [ebp-B4h]
    tGameObjectMain* v11; // [esp+38h] [ebp-A4h]
    tGameObjectMain* v12; // [esp+40h] [ebp-9Ch]
    tGameObjectMain* v13; // [esp+48h] [ebp-94h]
    tGameObjectMain* v14; // [esp+50h] [ebp-8Ch]
    float v15; // [esp+54h] [ebp-88h]
    float v16; // [esp+54h] [ebp-88h]
    float v17; // [esp+68h] [ebp-74h]
    Vec3 v18; // [esp+74h] [ebp-68h] BYREF
    Vec3 a3; // [esp+80h] [ebp-5Ch] BYREF
    Vec3 v20; // [esp+8Ch] [ebp-50h] BYREF
    Vec3 a4; // [esp+9Ch] [ebp-40h] BYREF
    int v23; // [esp+A8h] [ebp-34h]
    int v24; // [esp+ACh] [ebp-30h]
    int v25; // [esp+B4h] [ebp-28h]
    int v26; // [esp+B8h] [ebp-24h]
    int v27; // [esp+BCh] [ebp-20h]
    int v28; // [esp+C0h] [ebp-1Ch]
    int v29; // [esp+C4h] [ebp-18h]
    int v30; // [esp+C8h] [ebp-14h]
    tScriptStruct_AscenseurNazh* v31; // [esp+CCh] [ebp-10h]
    float v33; // [esp+D8h] [ebp-4h]

    v18.x = 1.0;
    v18.y = 0.0;
    v18.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_AscenseurNazh*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v31 = v5;
    v25 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_Ascenseur_Nazh_Asc_Nazh_exec_comp_status = (int)AI_ul_RealResolveOneRefFunction(0x720043F1u);
        Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao_o_joy_acteur,
            (void*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao,
            136);
        AI2C_ResolveBuffer(
            &Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao_f_joy_min,
            (void*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao,
            4888);
        Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        fct_Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_Ascenseur_Nazh_Asc_Nazh_mode_transfert_companion = (int)AI_ul_RealResolveOneRefFunction(0x72003819u);
    }
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(1))
    {
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 1);
        if (v31->i_snd_loop2 != -1)
        {
            SND_Release(v31->i_snd_loop2);
            v31->i_snd_loop2 = -1;
        }
        v30 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v30 != 1000)
            return v30;
    }
    if (v31->i_current_mode != 1)
    {
        v31->i_current_mode = 1;
        for (v31->i_companion_id = 0; v31->i_companion_id < 5; ++v31->i_companion_id)
            v31->ai_companion_status[v31->i_companion_id] = 0;
        v31->i_mode_phase = 0;
    }
    v28 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_Ascenseur_Nazh_Asc_Nazh_exec_comp_status);
    if (v28 != 1000)
        return v28;
    if (v31->i_mode_phase)
    {
        switch (v31->i_mode_phase)
        {
        case 1:
            v14 = (tGameObjectMain*)v31->o_start_wp;
            if (v14)
            {
                if ((unsigned int)v14 <= 2)
                    v14 = AI_MainActor[(uint32_t)v14 - 1];
                v1 = AI_EvalFunc_OBJ_SqrDistHorz_C(
                    (tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao__PJ_Sally_gao,
                    v14);
            }
            else
            {
                v1 = AI_EvalFunc_OBJ_SqrDistHorz_C(
                    (tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao__PJ_Sally_gao,
                    0);
            }
            v17 = (float)v1;
            if (v17 < 1.0)
            {
                *(uint32_t*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao_o_joy_acteur = 0;
                *(uint32_t*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao_f_joy_min = 0;
                AI_EvalFunc_OBJGetPos_C(
                    (tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao__PJ_Sally_gao,
                    &v31->v_transfert_init_pos);
                AI_EvalFunc_COLSetActivationSet_C(
                    (tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao__PJ_Sally_gao,
                    0,
                    1);
                v24 = AI_EvalFunc_DYNFlagsSet_C(
                    (tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao__PJ_Sally_gao,
                    0x40000,
                    0);
                ++v31->i_mode_phase;
                v31->f_time_phase = TIM_gf_MainClock - TIM_gf_LockTime;
                if (v31->i_snd_loop2 == -1)
                {
                    v31->i_snd_loop2 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 2, 0x18000000);
                    AI_EvalFunc_SNDUltraPlaySoundLooped_C(
                        (tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao__PJ_Sally_gao,
                        v31->i_snd_loop2);
                }
            }
            break;
        case 2:
            AI_EvalFunc_OBJ_CustomBitsSet_C(
                (tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao,
                0x8000,
                0);
            v27 = AI_EvalFunc_DYNFlagsSet_C(
                (tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao__PJ_Sally_gao,
                0x40000,
                0);
            AI_EvalFunc_COLSetActivationSet_C(
                (tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao__PJ_Sally_gao,
                0,
                1);
            if (AI_EvalFunc_TIMEElapsed_C(v31->f_time_phase, v31->des_f_time_transfert))
            {
                if (v31->i_flag_force_sortie)
                    *(uint32_t*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao_o_joy_acteur = v31->o_wp_sortie;
                v13 = (tGameObjectMain*)v31->o_dest_wp;
                if (v13)
                {
                    if ((unsigned int)v13 <= 2)
                        v13 = AI_MainActor[(uint32_t)v13 - 1];
                    AI_EvalFunc_OBJGetPos_C(v13, &a3);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &a3);
                }
                AI_EvalFunc_OBJSetPos_C(
                    (tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao__PJ_Sally_gao,
                    &a3);
                v26 = AI_EvalFunc_DYNFlagsSet_C(
                    (tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao__PJ_Sally_gao,
                    0,
                    0x40000);
                AI_EvalFunc_COLSetActivationSet_C(
                    (tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao__PJ_Sally_gao,
                    1,
                    0);
                ++v31->i_mode_phase;
                if (v31->i_snd_loop2 != -1)
                {
                    SND_Release(v31->i_snd_loop2);
                    v31->i_snd_loop2 = -1;
                }
                v25 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 3, 0x18000080);
                v12 = (tGameObjectMain*)v31->o_dest_wp;
                if (v12)
                {
                    if ((unsigned int)v12 <= 2)
                        v12 = AI_MainActor[(uint32_t)v12 - 1];
                    AI_EvalFunc_SNDUltraPlaySound_C(v12, v25);
                }
                else
                {
                    AI_EvalFunc_SNDUltraPlaySound_C(0, v25);
                }
            }
            else
            {
                v33 = TIM_gf_MainClock - TIM_gf_LockTime;
                v15 = (v33 - v31->f_time_phase) / v31->des_f_time_transfert;
                X = v15 * 1.570796f;
                v16 = sinf(X);
                v11 = (tGameObjectMain*)v31->o_dest_wp;
                if (v11)
                {
                    if ((unsigned int)v11 <= 2)
                        v11 = AI_MainActor[(uint32_t)v11 - 1];
                    AI_EvalFunc_OBJGetPos_C(v11, &v20);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v20);
                }
                AI_EvalFunc_MATHVecBlend_C(&v31->v_transfert_init_pos, &v20, v16, &a4);
                v18 = a4;
                AI_EvalFunc_OBJSetPos_C(
                    (tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao__PJ_Sally_gao,
                    &v18);
            }
            break;
        case 3:
            if (v31->i_mode_test_sortie)
            {
                v10 = (tGameObjectMain*)v31->o_wp_sortie;
                if (v10)
                {
                    if ((unsigned int)v10 <= 2)
                        v10 = AI_MainActor[(uint32_t)v10 - 1];
                    v2 = AI_EvalFunc_COLPivotBVCollide_C(
                        (tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao__PJ_Sally_gao,
                        v10);
                }
                else
                {
                    v2 = AI_EvalFunc_COLPivotBVCollide_C(
                        (tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao__PJ_Sally_gao,
                        0);
                }
                if (v2)
                    goto LABEL_66;
            }
            if (!v31->i_mode_test_sortie)
            {
                v9 = (tGameObjectMain*)v31->o_dest_wp;
                if (v9)
                {
                    if ((unsigned int)v9 <= 2)
                        v9 = AI_MainActor[(uint32_t)v9 - 1];
                    v3 = AI_EvalFunc_COLPivotBVCollide_C(
                        (tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao__PJ_Sally_gao,
                        v9);
                }
                else
                {
                    v3 = AI_EvalFunc_COLPivotBVCollide_C(
                        (tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao__PJ_Sally_gao,
                        0);
                }
                v23 = v3;
                if (!v3)
                {
                LABEL_66:
                    v8 = (tGameObjectMain*)v31->o_colmap_wp;
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
                        v6 = (tGameObjectMain*)v31->o_colmap_wp;
                        if (v6)
                        {
                            if ((unsigned int)v6 <= 2)
                                v6 = AI_MainActor[(uint32_t)v6 - 1];
                            AI_EvalFunc_COLMapActivationSet_C(v6, 0xFFu, 0);
                        }
                        else
                        {
                            AI_EvalFunc_COLMapActivationSet_C(0, 0xFFu, 0);
                        }
                    }
                    if (v31->i_flag_force_sortie)
                        *(uint32_t*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao_o_joy_acteur = 0;
                    v29 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_Ascenseur_Nazh_Asc_Nazh_mode_transfert_companion);
                    if (v29 != 1000)
                        return v29;
                }
            }
            break;
        }
    }
    else
    {
        *(uint32_t*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao_o_joy_acteur = v31->o_start_wp;
        *(uint32_t*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally_GST_Global_gao_f_joy_min = 0x3ECCCCCD;
        ++v31->i_mode_phase;
    }
    return 1000;
}

int Ascenseur_Nazh_Asc_Nazh_exec_GFX()
{
    double v0; // st7
    double v1; // st7
    double v3; // st7
    double v4; // st7
    tScriptStruct_AscenseurNazh* v5; // [esp+8h] [ebp-1B0h]
    tGameObjectMain* v6; // [esp+10h] [ebp-1A8h]
    tGameObjectMain* v7; // [esp+18h] [ebp-1A0h]
    tGameObjectMain* v8; // [esp+20h] [ebp-198h]
    tGameObjectMain* v9; // [esp+28h] [ebp-190h]
    tGameObjectMain* v10; // [esp+30h] [ebp-188h]
    tGameObjectMain* v11; // [esp+38h] [ebp-180h]
    tGameObjectMain* v12; // [esp+3Ch] [ebp-17Ch]
    tGameObjectMain* v13; // [esp+40h] [ebp-178h]
    tGameObjectMain* v14; // [esp+48h] [ebp-170h]
    tGameObjectMain* v15; // [esp+50h] [ebp-168h]
    tGameObjectMain* v16; // [esp+54h] [ebp-164h]
    tGameObjectMain* v17; // [esp+58h] [ebp-160h]
    tGameObjectMain* v18; // [esp+60h] [ebp-158h]
    tGameObjectMain* v19; // [esp+68h] [ebp-150h]
    tGameObjectMain* v20; // [esp+6Ch] [ebp-14Ch]
    tGameObjectMain* v21; // [esp+70h] [ebp-148h]
    tGameObjectMain* v22; // [esp+78h] [ebp-140h]
    tGameObjectMain* v23; // [esp+80h] [ebp-138h]
    tGameObjectMain* v24; // [esp+84h] [ebp-134h]
    tGameObjectMain* v25; // [esp+88h] [ebp-130h]
    Vec3 v26; // [esp+8Ch] [ebp-12Ch] BYREF
    float v27; // [esp+98h] [ebp-120h]
    Vec3 v28; // [esp+9Ch] [ebp-11Ch] BYREF
    float v29; // [esp+ACh] [ebp-10Ch]
    float v30; // [esp+B4h] [ebp-104h]
    Vec3 v31; // [esp+B8h] [ebp-100h] BYREF
    float v32; // [esp+C4h] [ebp-F4h]
    Vec3 dest; // [esp+CCh] [ebp-ECh] BYREF
    Vec3 v34; // [esp+D8h] [ebp-E0h] BYREF
    Vec3 v35; // [esp+E4h] [ebp-D4h] BYREF
    Vec3 v36; // [esp+F0h] [ebp-C8h] BYREF
    Vec3 v37; // [esp+FCh] [ebp-BCh] BYREF
    Vec3 v38; // [esp+108h] [ebp-B0h] BYREF
    Vec3 v39; // [esp+118h] [ebp-A0h] BYREF
    Vec3 v40; // [esp+124h] [ebp-94h] BYREF
    float v42; // [esp+13Ch] [ebp-7Ch]
    Vec3 v44; // [esp+144h] [ebp-74h] BYREF
    Vec3 a1; // [esp+150h] [ebp-68h] BYREF
    unsigned int v46; // [esp+160h] [ebp-58h]
    Vec3 v47; // [esp+164h] [ebp-54h] BYREF
    tScriptStruct_AscenseurNazh* v48; // [esp+170h] [ebp-48h]
    tGameObjectMain* v49; // [esp+174h] [ebp-44h]
    int v50; // [esp+178h] [ebp-40h]
    Vec3 v51; // [esp+17Ch] [ebp-3Ch] BYREF
    Vec3 v52; // [esp+188h] [ebp-30h] BYREF
    float v53; // [esp+194h] [ebp-24h]
    Vec3 a3; // [esp+19Ch] [ebp-1Ch] BYREF
    Vec3 v55; // [esp+1A8h] [ebp-10h] BYREF

    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_AscenseurNazh*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v48 = v5;
    v49 = 0;
    v50 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Ascenseur_Nazh_Asc_Nazh_exec_GFX_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    v49 = *AI_MainActor;
    v46 = AI_EvalFunc_OBJ_FlagsControlGet_C(AI_gpst_CurrentGameObject);
    if ((v46 & 0x220000) != 0)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v48->f_time_GFX, 2.0))
        {
            v48->f_time_GFX = TIM_gf_MainClock - TIM_gf_LockTime;
            v25 = v49;
            if (v49)
            {
                if ((unsigned int)v49 <= 2)
                    v25 = AI_MainActor[(uint32_t)v49 - 1];
                v24 = v25;
            }
            else
            {
                v24 = 0;
            }
            v23 = (tGameObjectMain*)v48->o_wp_1;
            if (v23)
            {
                if ((unsigned int)v23 <= 2)
                    v23 = AI_MainActor[(uint32_t)v23 - 1];
                v0 = AI_EvalFunc_OBJ_SqrDist_C(v23, v24);
            }
            else
            {
                v0 = AI_EvalFunc_OBJ_SqrDist_C(0, v24);
            }
            v27 = (float)v0;
            if (v48->des_dist_sound_off <= (double)v27)
            {
                v21 = v49;
                if (v49)
                {
                    if ((unsigned int)v49 <= 2)
                        v21 = AI_MainActor[(uint32_t)v49 - 1];
                    v20 = v21;
                }
                else
                {
                    v20 = 0;
                }
                v19 = (tGameObjectMain*)v48->o_wp_2;
                if (v19)
                {
                    if ((unsigned int)v19 <= 2)
                        v19 = AI_MainActor[(uint32_t)v19 - 1];
                    v1 = AI_EvalFunc_OBJ_SqrDist_C(v19, v20);
                }
                else
                {
                    v1 = AI_EvalFunc_OBJ_SqrDist_C(0, v20);
                }
                v53 = (float)v1;
                if (v48->des_dist_sound_off <= (double)v53)
                    goto LABEL_39;
                v50 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 1, 0x18000080);
                v18 = (tGameObjectMain*)v48->o_wp_2;
                if (v18)
                {
                    if ((unsigned int)v18 <= 2)
                        v18 = AI_MainActor[(uint32_t)v18 - 1];
                    AI_EvalFunc_SNDUltraPlaySound_C(v18, v50);
                    goto LABEL_39;
                }
            }
            else
            {
                v50 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 1, 0x18000080);
                v22 = (tGameObjectMain*)v48->o_wp_1;
                if (v22)
                {
                    if ((unsigned int)v22 <= 2)
                        v22 = AI_MainActor[(uint32_t)v22 - 1];
                    AI_EvalFunc_SNDUltraPlaySound_C(v22, v50);
                    goto LABEL_39;
                }
            }
            AI_EvalFunc_SNDUltraPlaySound_C(0, v50);
        }
    LABEL_39:
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    if (AI_EvalFunc_TIMEElapsed_C(v48->f_time_GFX, 2.0))
    {
        v48->f_time_GFX = TIM_gf_MainClock - TIM_gf_LockTime;
        v17 = v49;
        if (v49)
        {
            if ((unsigned int)v49 <= 2)
                v17 = AI_MainActor[(uint32_t)v49 - 1];
            v16 = v17;
        }
        else
        {
            v16 = 0;
        }
        v15 = (tGameObjectMain*)v48->o_wp_1;
        if (v15)
        {
            if ((unsigned int)v15 <= 2)
                v15 = AI_MainActor[(uint32_t)v15 - 1];
            v3 = AI_EvalFunc_OBJ_SqrDist_C(v15, v16);
        }
        else
        {
            v3 = AI_EvalFunc_OBJ_SqrDist_C(0, v16);
        }
        v32 = (float)v3;
        if (v48->des_dist_sound_off <= (double)v32)
        {
            v13 = v49;
            if (v49)
            {
                if ((unsigned int)v49 <= 2)
                    v13 = AI_MainActor[(uint32_t)v49 - 1];
                v12 = v13;
            }
            else
            {
                v12 = 0;
            }
            v11 = (tGameObjectMain*)v48->o_wp_2;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v11 = AI_MainActor[(uint32_t)v11 - 1];
                v4 = AI_EvalFunc_OBJ_SqrDist_C(v11, v12);
            }
            else
            {
                v4 = AI_EvalFunc_OBJ_SqrDist_C(0, v12);
            }
            v29 = (float)v4;
            if (v48->des_dist_sound_off <= (double)v29)
                goto LABEL_72;
            v50 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 1, 0x18000080);
            v10 = (tGameObjectMain*)v48->o_wp_2;
            if (v10)
            {
                if ((unsigned int)v10 <= 2)
                    v10 = AI_MainActor[(uint32_t)v10 - 1];
                AI_EvalFunc_SNDUltraPlaySound_C(v10, v50);
                goto LABEL_72;
            }
        }
        else
        {
            v50 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 1, 0x18000080);
            v14 = (tGameObjectMain*)v48->o_wp_1;
            if (v14)
            {
                if ((unsigned int)v14 <= 2)
                    v14 = AI_MainActor[(uint32_t)v14 - 1];
                AI_EvalFunc_SNDUltraPlaySound_C(v14, v50);
                goto LABEL_72;
            }
        }
        AI_EvalFunc_SNDUltraPlaySound_C(0, v50);
    LABEL_72:
        if (v48->i_flag_particules_up)
        {
            v48->GFX_Particules = AI_EvalFunc_GFXAdd_C_CURRENT(13);
            GFXSetCurGlobal(v48->GFX_Particules);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 100);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13106, 100);
            v9 = (tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_exec_GFX_SFX_light_smoke_gao;
            if (Ascenseur_Nazh_Asc_Nazh_exec_GFX_SFX_light_smoke_gao)
            {
                if ((unsigned int)Ascenseur_Nazh_Asc_Nazh_exec_GFX_SFX_light_smoke_gao <= 2)
                    v9 = AI_MainActor[Ascenseur_Nazh_Asc_Nazh_exec_GFX_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v48->GFX_Particules, v9, -1);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v48->GFX_Particules, 0, -1);
            }
            GFXSetCurGlobal(v48->GFX_Particules);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13101, 0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13103, 0x4080C040);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13104, 0x8080C040);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13005, 0.15000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13006, 0.25f);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13105, 0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13107, 0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13000, 0.0099999998f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13001, 0.1f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13002, 0.5f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13003, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13004, v48->f_GFX_life_time);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13007, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13009, 7.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13010, 7.0);
            v31.x = 0.050000001f;
            v31.y = 0.050000001f;
            v31.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v31);
            v36.x = 0.0;
            v36.y = 0.0;
            v36.z = v48->f_GFX_wind_Z_speed;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13204, &v36);
            v26.x = 0.0;
            v26.y = 0.0;
            v26.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13205, &v26);
            v35.x = 0.0;
            v35.y = 0.0;
            v35.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13206, &v35);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v48->GFX_Particules, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v48->GFX_Particules, 2, 1);
            GFXSetCurGlobal(v48->GFX_Particules);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13012, 0.0);
            v44.x = -1.0;
            v44.y = -1.0;
            v44.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v44);
            v40.x = 1.0;
            v40.y = 1.0;
            v40.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v40);
            v8 = (tGameObjectMain*)v48->o_down_wp;
            if (v8)
            {
                if ((unsigned int)v8 <= 2)
                    v8 = AI_MainActor[(uint32_t)v8 - 1];
                AI_EvalFunc_OBJGetPos_C(v8, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            v42 = TIM_gf_dt;
            v34.x = 0.0;
            v34.y = 0.0;
            v34.z = -v48->f_GFX_wind_Z_speed * TIM_gf_dt;
            MATH_SubstractVec(&dest, &a3, &v34);
            AI_EvalFunc_GFXSetv_C_CURGLOB(13200, &dest);
        }
        if (v48->i_flag_particules_down)
        {
            v48->GFX_Particules = AI_EvalFunc_GFXAdd_C_CURRENT(13);
            GFXSetCurGlobal(v48->GFX_Particules);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 100);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13106, 100);
            v7 = (tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_exec_GFX_SFX_light_smoke_gao;
            if (Ascenseur_Nazh_Asc_Nazh_exec_GFX_SFX_light_smoke_gao)
            {
                if ((unsigned int)Ascenseur_Nazh_Asc_Nazh_exec_GFX_SFX_light_smoke_gao <= 2)
                    v7 = AI_MainActor[Ascenseur_Nazh_Asc_Nazh_exec_GFX_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v48->GFX_Particules, v7, -1);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v48->GFX_Particules, 0, -1);
            }
            GFXSetCurGlobal(v48->GFX_Particules);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13101, 0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13103, 0x4080C040);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13104, 0x8080C040);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13005, 0.15000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13006, 0.25f);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13105, 0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13107, 0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13000, 0.0099999998f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13001, 0.1f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13002, 0.5f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13003, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13004, v48->f_GFX_life_time);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13007, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13009, 6.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13010, 6.0);
            v28.x = 0.050000001f;
            v28.y = 0.050000001f;
            v28.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v28);
            v51.x = 0.0;
            v51.y = 0.0;
            v51.z = -v48->f_GFX_wind_Z_speed;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13204, &v51);
            v37.x = 0.0;
            v37.y = 0.0;
            v37.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13205, &v37);
            v47.x = 0.0;
            v47.y = 0.0;
            v47.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13206, &v47);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v48->GFX_Particules, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v48->GFX_Particules, 2, 1);
            GFXSetCurGlobal(v48->GFX_Particules);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13012, 0.0);
            v38.x = -1.0;
            v38.y = -1.0;
            v38.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v38);
            v39.x = 1.0;
            v39.y = 1.0;
            v39.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v39);
            v6 = (tGameObjectMain*)v48->o_up_wp;
            if (v6)
            {
                if ((unsigned int)v6 <= 2)
                    v6 = AI_MainActor[(uint32_t)v6 - 1];
                AI_EvalFunc_OBJGetPos_C(v6, &v52);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v52);
            }
            v30 = TIM_gf_dt;
            v55.x = 0.0;
            v55.y = 0.0;
            v55.z = v48->f_GFX_wind_Z_speed * TIM_gf_dt;
            MATH_VecAddVec(&a1, &v52, &v55);
            AI_EvalFunc_GFXSetv_C_CURGLOB(13200, &a1);
        }
    }
    return 1000;
}

int Ascenseur_Nazh_Asc_Nazh_mode_wait()
{
    int v0; // eax
    int v2; // eax
    tScriptStruct_AscenseurNazh* v3; // [esp+0h] [ebp-58h]
    tGameObjectMain* v4; // [esp+8h] [ebp-50h]
    tGameObjectMain* v5; // [esp+10h] [ebp-48h]
    tGameObjectMain* v6; // [esp+18h] [ebp-40h]
    tGameObjectMain* v7; // [esp+20h] [ebp-38h]
    tGameObjectMain* v8; // [esp+24h] [ebp-34h]
    tGameObjectMain* v9; // [esp+28h] [ebp-30h]
    tGameObjectMain* v10; // [esp+30h] [ebp-28h]
    tGameObjectMain* v11; // [esp+38h] [ebp-20h]
    tGameObjectMain* v12; // [esp+3Ch] [ebp-1Ch]
    tGameObjectMain* v13; // [esp+40h] [ebp-18h]
    int v14; // [esp+44h] [ebp-14h]
    int v15; // [esp+50h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_AscenseurNazh*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Ascenseur_Nazh_Asc_Nazh_mode_wait__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        fct_Ascenseur_Nazh_Asc_Nazh_mode_wait_Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally = (int)AI_ul_RealResolveOneRefFunction(0x7200381Cu);
    }
    if (v3->i_current_mode)
    {
        v3->i_current_mode = 0;
        v13 = (tGameObjectMain*)v3->o_wp_1_colmap;
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
            if (v3->des_WP_1_entree)
            {
                v11 = (tGameObjectMain*)v3->o_wp_1_colmap;
                if (v11)
                {
                    if ((unsigned int)v11 <= 2)
                        v11 = AI_MainActor[(uint32_t)v11 - 1];
                    AI_EvalFunc_COLMapActivationSet_C(v11, 0, -1);
                }
                else
                {
                    AI_EvalFunc_COLMapActivationSet_C(0, 0, -1);
                }
            }
            else
            {
                v10 = (tGameObjectMain*)v3->o_wp_1_colmap;
                if (v10)
                {
                    if ((unsigned int)v10 <= 2)
                        v10 = AI_MainActor[(uint32_t)v10 - 1];
                    AI_EvalFunc_COLMapActivationSet_C(v10, 0xFFu, 0);
                }
                else
                {
                    AI_EvalFunc_COLMapActivationSet_C(0, 0xFFu, 0);
                }
            }
        }
        v9 = (tGameObjectMain*)v3->o_wp_2_colmap;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            v8 = v9;
        }
        else
        {
            v8 = 0;
        }
        if (v8)
        {
            if (v3->des_WP_2_entree)
            {
                v7 = (tGameObjectMain*)v3->o_wp_2_colmap;
                if (v7)
                {
                    if ((unsigned int)v7 <= 2)
                        v7 = AI_MainActor[(uint32_t)v7 - 1];
                    AI_EvalFunc_COLMapActivationSet_C(v7, 0, -1);
                }
                else
                {
                    AI_EvalFunc_COLMapActivationSet_C(0, 0, -1);
                }
            }
            else
            {
                v6 = (tGameObjectMain*)v3->o_wp_2_colmap;
                if (v6)
                {
                    if ((unsigned int)v6 <= 2)
                        v6 = AI_MainActor[(uint32_t)v6 - 1];
                    AI_EvalFunc_COLMapActivationSet_C(v6, 0xFFu, 0);
                }
                else
                {
                    AI_EvalFunc_COLMapActivationSet_C(0, 0xFFu, 0);
                }
            }
        }
    }
    if (v3->des_WP_1_entree)
    {
        v5 = (tGameObjectMain*)v3->o_wp_1;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            v0 = AI_EvalFunc_COLPivotBVCollide_C((tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_wait__PJ_Sally_gao, v5);
        }
        else
        {
            v0 = AI_EvalFunc_COLPivotBVCollide_C((tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_wait__PJ_Sally_gao, 0);
        }
        if (v0)
        {
            v3->o_start_wp = v3->o_wp_1;
            v3->o_dest_wp = v3->o_wp_2;
            v3->o_wp_sortie = v3->o_wp_2_sortie_sally;
            v3->o_colmap_wp = v3->o_wp_2_colmap;
            v3->i_flag_force_sortie = v3->des_wp_2_sortie_auto;
            v3->i_mode_test_sortie = v3->des_wp_2_mode_sortie;
            v15 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Ascenseur_Nazh_Asc_Nazh_mode_wait_Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally);
            if (v15 != 1000)
                return v15;
        }
    }
    if (v3->des_WP_2_entree)
    {
        v4 = (tGameObjectMain*)v3->o_wp_2;
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            v2 = AI_EvalFunc_COLPivotBVCollide_C((tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_wait__PJ_Sally_gao, v4);
        }
        else
        {
            v2 = AI_EvalFunc_COLPivotBVCollide_C((tGameObjectMain*)Ascenseur_Nazh_Asc_Nazh_mode_wait__PJ_Sally_gao, 0);
        }
        if (v2)
        {
            v3->o_start_wp = v3->o_wp_2;
            v3->o_dest_wp = v3->o_wp_1;
            v3->o_wp_sortie = v3->o_wp_1_sortie_sally;
            v3->o_colmap_wp = v3->o_wp_1_colmap;
            v3->i_flag_force_sortie = v3->des_wp_1_sortie_auto;
            v3->i_mode_test_sortie = v3->des_wp_1_mode_sortie;
            v14 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Ascenseur_Nazh_Asc_Nazh_mode_wait_Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally);
            if (v14 != 1000)
                return v14;
        }
    }
    return 1000;
}

int Ascenseur_Nazh_Asc_Nazh_mode_transfert_companion()
{
    double v1; // st7
    int v2; // eax
    double v3; // st7
    int v4; // eax
    int v5; // eax
    double v6; // st7
    float X; // [esp+4h] [ebp-1F8h]
    tScriptStruct_AscenseurNazh* v8; // [esp+8h] [ebp-1F4h]
    tGameObjectMain* v9; // [esp+10h] [ebp-1ECh]
    tGameObjectMain* v10; // [esp+18h] [ebp-1E4h]
    tGameObjectMain* v11; // [esp+28h] [ebp-1D4h]
    tGameObjectMain* v12; // [esp+30h] [ebp-1CCh]
    tGameObjectMain* v13; // [esp+38h] [ebp-1C4h]
    tGameObjectMain* v14; // [esp+3Ch] [ebp-1C0h]
    tGameObjectMain* v15; // [esp+40h] [ebp-1BCh]
    tGameObjectMain* v16; // [esp+48h] [ebp-1B4h]
    tGameObjectMain* v17; // [esp+50h] [ebp-1ACh]
    tGameObjectMain* v18; // [esp+54h] [ebp-1A8h]
    tGameObjectMain* v19; // [esp+58h] [ebp-1A4h]
    tGameObjectMain* v20; // [esp+5Ch] [ebp-1A0h]
    tGameObjectMain* v21; // [esp+60h] [ebp-19Ch]
    tGameObjectMain* v22; // [esp+68h] [ebp-194h]
    tGameObjectMain* v23; // [esp+70h] [ebp-18Ch]
    tGameObjectMain* v24; // [esp+78h] [ebp-184h]
    tGameObjectMain* v25; // [esp+80h] [ebp-17Ch]
    tGameObjectMain* v26; // [esp+88h] [ebp-174h]
    tGameObjectMain* v27; // [esp+90h] [ebp-16Ch]
    tGameObjectMain* v28; // [esp+98h] [ebp-164h]
    tGameObjectMain* v29; // [esp+A0h] [ebp-15Ch]
    tGameObjectMain* v30; // [esp+A4h] [ebp-158h]
    tGameObjectMain* v31; // [esp+A8h] [ebp-154h]
    tGameObjectMain* v32; // [esp+B0h] [ebp-14Ch]
    tGameObjectMain* v33; // [esp+B8h] [ebp-144h]
    tGameObjectMain* v34; // [esp+C0h] [ebp-13Ch]
    tGameObjectMain* v35; // [esp+C8h] [ebp-134h]
    tGameObjectMain* v36; // [esp+CCh] [ebp-130h]
    tGameObjectMain* v37; // [esp+D0h] [ebp-12Ch]
    tGameObjectMain* v38; // [esp+D8h] [ebp-124h]
    tGameObjectMain* v39; // [esp+E0h] [ebp-11Ch]
    Vec3 v40; // [esp+E4h] [ebp-118h] BYREF
    Vec3 v41; // [esp+F4h] [ebp-108h] BYREF
    float v42; // [esp+10Ch] [ebp-F0h]
    float v43; // [esp+114h] [ebp-E8h]
    int v44; // [esp+124h] [ebp-D8h]
    int v45; // [esp+128h] [ebp-D4h]
    tMessage Src; // [esp+12Ch] [ebp-D0h] BYREF
    float v47; // [esp+160h] [ebp-9Ch]
    int v49; // [esp+16Ch] [ebp-90h]
    float v50; // [esp+1A8h] [ebp-54h]
    Vec3 v51; // [esp+1B8h] [ebp-44h] BYREF
    tGameObjectMain* v52; // [esp+1C8h] [ebp-34h]
    int v53; // [esp+1CCh] [ebp-30h]
    tGameObjectMain* v54; // [esp+1D0h] [ebp-2Ch]
    tScriptStruct_AscenseurNazh* v55; // [esp+1D8h] [ebp-24h]
    int v56; // [esp+1DCh] [ebp-20h]
    Vec3 a4; // [esp+1E0h] [ebp-1Ch] BYREF
    int v58; // [esp+1ECh] [ebp-10h]
    int v59; // [esp+1F0h] [ebp-Ch]
    float v60; // [esp+1F8h] [ebp-4h]

    v51.x = 1.0;
    v51.y = 0.0;
    v51.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v8 = (tScriptStruct_AscenseurNazh*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v8 = 0;
    v55 = v8;
    v42 = 0.0;
    v52 = 0;
    v53 = 0;
    v56 = 0;
    v54 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Ascenseur_Nazh_Asc_Nazh_mode_transfert_companion_Ascenseur_Nazh_Asc_Nazh_exec_comp_status = (int)AI_ul_RealResolveOneRefFunction(0x720043F1u);
        Ascenseur_Nazh_Asc_Nazh_mode_transfert_companion_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Ascenseur_Nazh_Asc_Nazh_mode_transfert_companion_univVars_Perso_ID_actor,
            (void*)Ascenseur_Nazh_Asc_Nazh_mode_transfert_companion_univVars,
            988);
        fct_Ascenseur_Nazh_Asc_Nazh_mode_transfert_companion_Ascenseur_Nazh_Asc_Nazh_mode_wait = (int)AI_ul_RealResolveOneRefFunction(0x72003816u);
    }
    if (v55->i_current_mode != 2)
    {
        v55->i_current_mode = 2;
        v55->i_mode_phase = 0;
        v55->i_companion_id = 0;
    }
    v45 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Ascenseur_Nazh_Asc_Nazh_mode_transfert_companion_Ascenseur_Nazh_Asc_Nazh_exec_comp_status);
    if (v45 != 1000)
        return v45;
    v54 = *AI_MainActor;
    if (v55->i_mode_phase)
    {
        switch (v55->i_mode_phase)
        {
        case 1:
            v37 = (tGameObjectMain*)v55->o_start_wp;
            if (v37)
            {
                if ((unsigned int)v37 <= 2)
                    v37 = AI_MainActor[(uint32_t)v37 - 1];
                v36 = v37;
            }
            else
            {
                v36 = 0;
            }
            v35 = (tGameObjectMain*)v55->o_companion;
            if (v35)
            {
                if ((unsigned int)v35 <= 2)
                    v35 = AI_MainActor[(uint32_t)v35 - 1];
                v1 = AI_EvalFunc_OBJ_SqrDistHorz_C(v35, v36);
            }
            else
            {
                v1 = AI_EvalFunc_OBJ_SqrDistHorz_C(0, v36);
            }
            v43 = (float)v1;
            if (v43 < 1.0)
            {
                v34 = (tGameObjectMain*)v55->o_companion;
                if (v34)
                {
                    if ((unsigned int)v34 <= 2)
                        v34 = AI_MainActor[(uint32_t)v34 - 1];
                    AI_EvalFunc_OBJGetPos_C(v34, &v55->v_transfert_init_pos);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v55->v_transfert_init_pos);
                }
                v33 = (tGameObjectMain*)v55->o_companion;
                if (v33)
                {
                    if ((unsigned int)v33 <= 2)
                        v33 = AI_MainActor[(uint32_t)v33 - 1];
                    v2 = AI_EvalFunc_DYNFlagsSet_C(v33, 0x40000, 0);
                }
                else
                {
                    v2 = AI_EvalFunc_DYNFlagsSet_C(0, 0x40000, 0);
                }
                v58 = v2;
                v55->ai_companion_status[v55->i_companion_id] = 4;
                v55->m_msg.msgID = 49;
                v55->m_msg.gao2 = 0;
                v55->m_msg.Int1 = 4;
                v55->m_msg.Int2 = 0;
                v55->m_msg.f2 = 0.0;
                v55->m_msg.f1 = 0.0;
                v55->m_msg.v1.x = 0.0;
                v55->m_msg.v1.y = 0.0;
                v55->m_msg.v1.z = 0.0;
                v55->m_msg.v2.x = 0.0;
                v55->m_msg.v2.y = 0.0;
                v55->m_msg.v2.z = 0.0;
                v32 = (tGameObjectMain*)v55->o_companion;
                if (v32)
                {
                    if ((unsigned int)v32 <= 2)
                        v32 = AI_MainActor[(uint32_t)v32 - 1];
                    AI_EvalFunc_MSGSend_C(v32, &v55->m_msg);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &v55->m_msg);
                }
                ++v55->i_mode_phase;
                v55->f_time_phase = TIM_gf_MainClock - TIM_gf_LockTime;
                v31 = (tGameObjectMain*)v55->o_start_wp;
                if (v31)
                {
                    if ((unsigned int)v31 <= 2)
                        v31 = AI_MainActor[(uint32_t)v31 - 1];
                    v30 = v31;
                }
                else
                {
                    v30 = 0;
                }
                v29 = v54;
                if (v54)
                {
                    if ((unsigned int)v54 <= 2)
                        v29 = AI_MainActor[(uint32_t)v54 - 1];
                    v3 = AI_EvalFunc_OBJ_SqrDist_C(v29, v30);
                }
                else
                {
                    v3 = AI_EvalFunc_OBJ_SqrDist_C(0, v30);
                }
                v60 = (float)v3;
                if (v55->des_dist_sound_off > (double)v60)
                {
                    v53 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 2, 0x18000080);
                    v28 = (tGameObjectMain*)v55->o_companion;
                    if (v28)
                    {
                        if ((unsigned int)v28 <= 2)
                            v28 = AI_MainActor[(uint32_t)v28 - 1];
                        AI_EvalFunc_SNDUltraPlaySound_C(v28, v53);
                    }
                    else
                    {
                        AI_EvalFunc_SNDUltraPlaySound_C(0, v53);
                    }
                }
            }
            break;
        case 2:
            v27 = (tGameObjectMain*)v55->o_companion;
            if (v27)
            {
                if ((unsigned int)v27 <= 2)
                    v27 = AI_MainActor[(uint32_t)v27 - 1];
                v4 = AI_EvalFunc_DYNFlagsSet_C(v27, 0x40000, 0);
            }
            else
            {
                v4 = AI_EvalFunc_DYNFlagsSet_C(0, 0x40000, 0);
            }
            v44 = v4;
            v26 = (tGameObjectMain*)v55->o_companion;
            if (v26)
            {
                if ((unsigned int)v26 <= 2)
                    v26 = AI_MainActor[(uint32_t)v26 - 1];
                AI_EvalFunc_COLSetActivationSet_C(v26, 0, 1);
            }
            else
            {
                AI_EvalFunc_COLSetActivationSet_C(0, 0, 1);
            }
            if (AI_EvalFunc_TIMEElapsed_C(v55->f_time_phase, v55->des_f_time_transfert))
            {
                v25 = (tGameObjectMain*)v55->o_dest_wp;
                if (v25)
                {
                    if ((unsigned int)v25 <= 2)
                        v25 = AI_MainActor[(uint32_t)v25 - 1];
                    AI_EvalFunc_OBJGetPos_C(v25, &v40);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v40);
                }
                v24 = (tGameObjectMain*)v55->o_companion;
                if (v24)
                {
                    if ((unsigned int)v24 <= 2)
                        v24 = AI_MainActor[(uint32_t)v24 - 1];
                    AI_EvalFunc_OBJSetPos_C(v24, &v40);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &v40);
                }
                v23 = (tGameObjectMain*)v55->o_companion;
                if (v23)
                {
                    if ((unsigned int)v23 <= 2)
                        v23 = AI_MainActor[(uint32_t)v23 - 1];
                    v5 = AI_EvalFunc_DYNFlagsSet_C(v23, 0, 0x40000);
                }
                else
                {
                    v5 = AI_EvalFunc_DYNFlagsSet_C(0, 0, 0x40000);
                }
                v49 = v5;
                v22 = (tGameObjectMain*)v55->o_companion;
                if (v22)
                {
                    if ((unsigned int)v22 <= 2)
                        v22 = AI_MainActor[(uint32_t)v22 - 1];
                    AI_EvalFunc_COLSetActivationSet_C(v22, 1, 0);
                }
                else
                {
                    AI_EvalFunc_COLSetActivationSet_C(0, 1, 0);
                }
                v21 = (tGameObjectMain*)v55->o_dest_wp;
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
                v19 = (tGameObjectMain*)v55->o_wp_1;
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
                if (v20 == v18)
                    v52 = (tGameObjectMain*)v55->o_wp_1_sortie[v55->i_companion_id];
                else
                    v52 = (tGameObjectMain*)v55->o_wp_2_sortie[v55->i_companion_id];
                Src.msgID = 6;
                Src.Int1 = 1;
                Src.Int2 = 0;
                Src.f2 = 0.0;
                Src.f1 = 20.0;
                v17 = v52;
                if (v52)
                {
                    if ((unsigned int)v52 <= 2)
                        v17 = AI_MainActor[(uint32_t)v52 - 1];
                    AI_EvalFunc_OBJGetPos_C(v17, &Src.v1);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &Src.v1);
                }
                Src.v2.x = 0.0;
                Src.v2.y = 0.0;
                Src.v2.z = 0.0;
                Src.gao = AI_gpst_CurrentGameObject;
                Src.gao2 = v52;
                v16 = (tGameObjectMain*)v55->o_companion;
                if (v16)
                {
                    if ((unsigned int)v16 <= 2)
                        v16 = AI_MainActor[(uint32_t)v16 - 1];
                    AI_EvalFunc_MSGSend_C(v16, &Src);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &Src);
                }
                ++v55->i_mode_phase;
                v15 = (tGameObjectMain*)v55->o_dest_wp;
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
                v13 = v54;
                if (v54)
                {
                    if ((unsigned int)v54 <= 2)
                        v13 = AI_MainActor[(uint32_t)v54 - 1];
                    v6 = AI_EvalFunc_OBJ_SqrDist_C(v13, v14);
                }
                else
                {
                    v6 = AI_EvalFunc_OBJ_SqrDist_C(0, v14);
                }
                v50 = (float)v6;
                if (v55->des_dist_sound_off > (double)v50)
                {
                    v53 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 3, 0x18000080);
                    v12 = (tGameObjectMain*)v55->o_dest_wp;
                    if (v12)
                    {
                        if ((unsigned int)v12 <= 2)
                            v12 = AI_MainActor[(uint32_t)v12 - 1];
                        AI_EvalFunc_SNDUltraPlaySound_C(v12, v53);
                    }
                    else
                    {
                        AI_EvalFunc_SNDUltraPlaySound_C(0, v53);
                    }
                }
            }
            else
            {
                v47 = TIM_gf_MainClock - TIM_gf_LockTime;
                v42 = (v47 - v55->f_time_phase) / v55->des_f_time_transfert;
                X = v42 * 1.570796f;
                v42 = sinf(X);
                v11 = (tGameObjectMain*)v55->o_dest_wp;
                if (v11)
                {
                    if ((unsigned int)v11 <= 2)
                        v11 = AI_MainActor[(uint32_t)v11 - 1];
                    AI_EvalFunc_OBJGetPos_C(v11, &v41);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v41);
                }
                AI_EvalFunc_MATHVecBlend_C(&v55->v_transfert_init_pos, &v41, v42, &a4);
                v51 = a4;
                v10 = (tGameObjectMain*)v55->o_companion;
                if (v10)
                {
                    if ((unsigned int)v10 <= 2)
                        v10 = AI_MainActor[(uint32_t)v10 - 1];
                    AI_EvalFunc_OBJSetPos_C(v10, &v51);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &v51);
                }
            }
            break;
        case 3:
            ++v55->i_mode_phase;
            break;
        case 4:
            v9 = (tGameObjectMain*)v55->o_companion;
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                AI_EvalFunc_OBJ_CapaSet_C(v9, 0, 2);
            }
            else
            {
                AI_EvalFunc_OBJ_CapaSet_C(0, 0, 2);
            }
            v55->ai_companion_status[v55->i_companion_id] = 5;
            v55->i_mode_phase = 0;
            break;
        }
    }
    else
    {
        v56 = 0;
        for (v55->i_companion_id = 0; v55->i_companion_id < 5; ++v55->i_companion_id)
        {
            if (!v55->ai_companion_status[v55->i_companion_id] || v55->ai_companion_status[v55->i_companion_id] == 5)
                ++v56;
            if (v55->ai_companion_status[v55->i_companion_id] == 2)
            {
                v55->o_companion = *(uint32_t*)(Ascenseur_Nazh_Asc_Nazh_mode_transfert_companion_univVars_Perso_ID_actor
                    + 4 * v55->i_companion_id
                    + 4);
                v55->ai_companion_status[v55->i_companion_id] = 3;
                Src.msgID = 6;
                Src.Int1 = 1;
                Src.Int2 = 0;
                Src.f2 = 0.0;
                Src.f1 = 20.0;
                v39 = (tGameObjectMain*)v55->o_start_wp;
                if (v39)
                {
                    if ((unsigned int)v39 <= 2)
                        v39 = AI_MainActor[(uint32_t)v39 - 1];
                    AI_EvalFunc_OBJGetPos_C(v39, &Src.v1);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &Src.v1);
                }
                Src.v2.x = 0.0;
                Src.v2.y = 0.0;
                Src.v2.z = 0.0;
                Src.gao = AI_gpst_CurrentGameObject;
                Src.gao2 = AI_gpst_CurrentGameObject;
                v38 = (tGameObjectMain*)v55->o_companion;
                if (v38)
                {
                    if ((unsigned int)v38 <= 2)
                        v38 = AI_MainActor[(uint32_t)v38 - 1];
                    AI_EvalFunc_MSGSend_C(v38, &Src);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &Src);
                }
                ++v55->i_mode_phase;
                break;
            }
        }
        if (v56 == 5)
        {
            v59 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Ascenseur_Nazh_Asc_Nazh_mode_transfert_companion_Ascenseur_Nazh_Asc_Nazh_mode_wait);
            if (v59 != 1000)
                return v59;
        }
    }
    return 1000;
}

int Ascenseur_Nazh_Asc_Nazh_exec_sound()
{
    int v0; // eax
    int result; // eax
    tScriptStruct_AscenseurNazh* v2; // [esp+0h] [ebp-140h]
    tGameObjectMain* v3; // [esp+8h] [ebp-138h]
    tGameObjectMain* v4; // [esp+18h] [ebp-128h]
    tGameObjectMain* v5; // [esp+40h] [ebp-100h]
    float v6; // [esp+44h] [ebp-FCh]
    tGameObjectMain* v7; // [esp+60h] [ebp-E0h]
    tGameObjectMain* v8; // [esp+70h] [ebp-D0h]
    Vec3 dest; // [esp+78h] [ebp-C8h] BYREF
    Vec3 v10; // [esp+84h] [ebp-BCh] BYREF
    Vec3 v11; // [esp+90h] [ebp-B0h] BYREF
    Vec3 v12; // [esp+A0h] [ebp-A0h] BYREF
    Vec3 v13; // [esp+B0h] [ebp-90h]
    float v14; // [esp+BCh] [ebp-84h]
    float v15; // [esp+C0h] [ebp-80h]
    Vec3 v16; // [esp+C4h] [ebp-7Ch] BYREF
    Vec3 v17; // [esp+D0h] [ebp-70h] BYREF
    Vec3 v18; // [esp+DCh] [ebp-64h] BYREF
    Vec3 v19; // [esp+ECh] [ebp-54h] BYREF
    float v20; // [esp+F8h] [ebp-48h]
    float v21; // [esp+FCh] [ebp-44h]
    float v22; // [esp+100h] [ebp-40h]
    Vec3 a1; // [esp+104h] [ebp-3Ch] BYREF
    Vec3 a3; // [esp+110h] [ebp-30h] BYREF
    tScriptStruct_AscenseurNazh* v25; // [esp+11Ch] [ebp-24h]
    Vec3 v26; // [esp+120h] [ebp-20h] BYREF
    Vec3 v27; // [esp+12Ch] [ebp-14h] BYREF
    tGameObjectMain* v28; // [esp+138h] [ebp-8h]

    v18.x = 1.0;
    v18.y = 0.0;
    v18.z = 0.0;
    v13.x = 1.0;
    v13.y = 0.0;
    v13.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    v10.x = 1.0;
    v10.y = 0.0;
    v10.z = 0.0;
    v17.x = 1.0;
    v17.y = 0.0;
    v17.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_AscenseurNazh*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v25 = v2;
    v14 = 0.0;
    v15 = 0.0;
    v28 = 0;
    v28 = *AI_MainActor;
    v8 = (tGameObjectMain*)v2->o_wp_1;
    if (v8)
    {
        if ((unsigned int)v8 <= 2)
            v8 = AI_MainActor[(uint32_t)v8 - 1];
        AI_EvalFunc_OBJGetPos_C(v8, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    v18 = a3;
    v7 = (tGameObjectMain*)v25->o_wp_2;
    if (v7)
    {
        if ((unsigned int)v7 <= 2)
            v7 = AI_MainActor[(uint32_t)v7 - 1];
        AI_EvalFunc_OBJGetPos_C(v7, &v19);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v19);
    }
    v17 = v19;
    MATH_SubstractVec(&dest, &v17, &v18);
    a1 = dest;
    v15 = MATH_VecLen(&a1);
    v6 = 1.0f / v15;
    a1.x = a1.x * v6;
    a1.y = a1.y * v6;
    a1.z = a1.z * v6;
    v5 = v28;
    if (v28)
    {
        if ((unsigned int)v28 <= 2)
            v5 = AI_MainActor[(uint32_t)v28 - 1];
        AI_EvalFunc_OBJGetPos_C(v5, &v16);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v16);
    }
    MATH_SubstractVec(&v27, &v16, &v18);
    v13 = v27;
    v14 = a1.z * v27.z + a1.y * v27.y + v27.x * a1.x;
    if (v14 < 0.0 || v14 >(double)v15)
    {
        if (v14 >= 0.0)
            v10 = v17;
        else
            v10 = v18;
    }
    else
    {
        v20 = a1.x * v14;
        v21 = a1.y * v14;
        v22 = a1.z * v14;
        v10.x = v18.x + v20;
        v10.y = v18.y + v21;
        v10.z = v18.z + v22;
    }
    v4 = v28;
    if (v28)
    {
        if ((unsigned int)v28 <= 2)
            v4 = AI_MainActor[(uint32_t)v28 - 1];
        AI_EvalFunc_OBJGetPos_C(v4, &v26);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v26);
    }
    MATH_SubstractVec(&v12, &v10, &v26);
    v13 = v12;
    v14 = v13.z * v13.z + v13.y * v13.y + v13.x * v13.x;
    if (v25->des_dist_sound_off <= (double)v14)
    {
        if (v25->i_snd_loop != -1)
        {
            SND_Release(v25->i_snd_loop);
            v25->i_snd_loop = -1;
        }
        result = 1000;
    }
    else
    {
        v3 = v28;
        if (v28)
        {
            if ((unsigned int)v28 <= 2)
                v3 = AI_MainActor[(uint32_t)v28 - 1];
            AI_EvalFunc_OBJGetPos_C(v3, &v11);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v11);
        }
        if (v25->i_snd_loop == -1)
        {
            v0 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000000);
            v25->i_snd_loop = v0;
            AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v25->i_snd_loop);
        }
        AI_EvalFunc_SNDInstPosVSet_C(v25->i_snd_loop, &v10);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}