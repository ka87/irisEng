#include "ai_compiledscripts_inc.h"

//Porte_Armoire_Porte_Armoire_init
int Porte_Armoire_Porte_Armoire_init_univVars;
int Porte_Armoire_Porte_Armoire_init_univVars_Flag_speciaux_sauve;
int fct_Porte_Armoire_Porte_Armoire_init_Porte_Armoire_Porte_Armoire_mode_open;
int fct_Porte_Armoire_Porte_Armoire_init_Porte_Armoire_Porte_Armoire_mode_close;
int fct_Porte_Armoire_Porte_Armoire_init_Porte_Armoire_Porte_Armoire_mode_wait;

//Porte_Armoire_Porte_Armoire_mode_open
int Porte_Armoire_Porte_Armoire_mode_open_univVars;
int Porte_Armoire_Porte_Armoire_mode_open_univVars_Flag_speciaux_sauve;
int Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao;
int Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_f_event_sound_time;
int Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_i_event_sound_nbr;
int Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_v_event_sound_pos;
int Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_f_event_sound_size;
int Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_o_event_sound_actor;
int Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_i_event_sound_id;
int Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_i_event_sound_next_id;
int fct_Porte_Armoire_Porte_Armoire_mode_open_Porte_Armoire_Porte_Armoire_mode_wait;

//Porte_Armoire_Porte_Armoire_mode_close
int Porte_Armoire_Porte_Armoire_mode_close_univVars;
int Porte_Armoire_Porte_Armoire_mode_close_univVars_Flag_speciaux_sauve;
int Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao;
int Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_f_event_sound_time;
int Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_i_event_sound_nbr;
int Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_v_event_sound_pos;
int Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_f_event_sound_size;
int Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_o_event_sound_actor;
int Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_i_event_sound_id;
int Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_i_event_sound_next_id;
int fct_Porte_Armoire_Porte_Armoire_mode_close_Porte_Armoire_Porte_Armoire_mode_wait;

//Porte_Armoire_Porte_Armoire_mode_wait
int fct_Porte_Armoire_Porte_Armoire_mode_wait_Porte_Armoire_Porte_Armoire_mode_close;
int Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao;
int Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao_current_pm_mode;
int Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao_current_pm_priority;
int Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao;
int Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_i_sally_priority;
int Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_o_sally_contextuel;
int Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_i_sally_action;
int Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_v_sally_offset;
int Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_i_force_interface;
int fct_Porte_Armoire_Porte_Armoire_mode_wait_Porte_Armoire_Porte_Armoire_mode_open;

int Porte_Armoire_Porte_Armoire_init()
{
    float X; // [esp+0h] [ebp-90h]
    float Xa; // [esp+0h] [ebp-90h]
    float Xb; // [esp+0h] [ebp-90h]
    float v4; // [esp+4h] [ebp-8Ch]
    float v5; // [esp+8h] [ebp-88h]
    float v6; // [esp+Ch] [ebp-84h]
    tScriptStruct_PorteArmoire* v7; // [esp+14h] [ebp-7Ch]
    tGameObjectMain* v8; // [esp+1Ch] [ebp-74h]
    tGameObjectMain* v9; // [esp+20h] [ebp-70h]
    tGameObjectMain* v10; // [esp+24h] [ebp-6Ch]
    tGameObjectMain* v11; // [esp+2Ch] [ebp-64h]
    tGameObjectMain* v12; // [esp+30h] [ebp-60h]
    tGameObjectMain* v13; // [esp+34h] [ebp-5Ch]
    tGameObjectMain* v14; // [esp+3Ch] [ebp-54h]
    tGameObjectMain* v15; // [esp+40h] [ebp-50h]
    tGameObjectMain* v16; // [esp+44h] [ebp-4Ch]
    tGameObjectMain* v17; // [esp+4Ch] [ebp-44h]
    tGameObjectMain* v18; // [esp+50h] [ebp-40h]
    tGameObjectMain* v19; // [esp+54h] [ebp-3Ch]
    tGameObjectMain* v20; // [esp+5Ch] [ebp-34h]
    tGameObjectMain* v21; // [esp+60h] [ebp-30h]
    tGameObjectMain* v22; // [esp+64h] [ebp-2Ch]
    tGameObjectMain* v23; // [esp+68h] [ebp-28h]
    int v24; // [esp+74h] [ebp-1Ch]
    int v25; // [esp+7Ch] [ebp-14h]
    int v26; // [esp+8Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_PorteArmoire*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Porte_Armoire_Porte_Armoire_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_init_univVars_Flag_speciaux_sauve,
            (void*)Porte_Armoire_Porte_Armoire_init_univVars,
            4488);
        fct_Porte_Armoire_Porte_Armoire_init_Porte_Armoire_Porte_Armoire_mode_open = (int)AI_ul_RealResolveOneRefFunction(0x72004002u);
        fct_Porte_Armoire_Porte_Armoire_init_Porte_Armoire_Porte_Armoire_mode_close = (int)AI_ul_RealResolveOneRefFunction(0x72004021u);
        fct_Porte_Armoire_Porte_Armoire_init_Porte_Armoire_Porte_Armoire_mode_wait = (int)AI_ul_RealResolveOneRefFunction(0x72004001u);
    }
    if (*AI_gppst_CurrentJumpNode != (tAINode*)14)
    {
        v7->des_f_cone = v7->des_f_cone / 2.0f;
        v7->des_f_cone = v7->des_f_cone * 0.017453f;
        v7->des_f_cone = cosf(v7->des_f_cone);
        AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x8000, 0);
        AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 2, 0);
        v7->des_f_dist_open = v7->des_f_dist_open * v7->des_f_dist_open;
        v23 = (tGameObjectMain*)v7->des_sound_event_open_pos;
        if (v23)
        {
            if ((unsigned int)v23 <= 2)
                v23 = AI_MainActor[(uint32_t)v23 - 1];
            v22 = v23;
        }
        else
        {
            v22 = 0;
        }
        if (!v22)
            v7->des_sound_event_open_pos = (int)AI_gpst_CurrentGameObject;
        v21 = (tGameObjectMain*)v7->des_sound_event_close_pos;
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
        if (!v20)
            v7->des_sound_event_close_pos = (int)AI_gpst_CurrentGameObject;
        v7->i_flag_porte_ouverte = v7->des_SF_porte_ouverte > 0
            && AI_EvalFunc_ARRBitTest_C(
                (int*)(Porte_Armoire_Porte_Armoire_init_univVars_Flag_speciaux_sauve + 4),
                v7->des_SF_porte_ouverte);
        if (v7->i_flag_porte_ouverte || v7->des_test == 2)
        {
            v7->f_angle = v7->des_f_angle_max;
            v19 = (tGameObjectMain*)v7->des_o_contenu;
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
            if (v18)
            {
                v17 = (tGameObjectMain*)v7->des_o_contenu;
                if (v17)
                {
                    if ((unsigned int)v17 <= 2)
                        v17 = AI_MainActor[(uint32_t)v17 - 1];
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(v17, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
                }
            }
            v16 = (tGameObjectMain*)v7->des_o_contenu1;
            if (v16)
            {
                if ((unsigned int)v16 <= 2)
                    v16 = AI_MainActor[(uint32_t)v16 - 1];
                v15 = v16;
            }
            else
            {
                v15 = 0;
            }
            if (v15)
            {
                v14 = (tGameObjectMain*)v7->des_o_contenu1;
                if (v14)
                {
                    if ((unsigned int)v14 <= 2)
                        v14 = AI_MainActor[(uint32_t)v14 - 1];
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(v14, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
                }
            }
            v13 = (tGameObjectMain*)v7->des_o_contenu2;
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
                v11 = (tGameObjectMain*)v7->des_o_contenu2;
                if (v11)
                {
                    if ((unsigned int)v11 <= 2)
                        v11 = AI_MainActor[(uint32_t)v11 - 1];
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(v11, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
                }
            }
            v10 = (tGameObjectMain*)v7->des_o_contenu3;
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
                v8 = (tGameObjectMain*)v7->des_o_contenu3;
                if (v8)
                {
                    if ((unsigned int)v8 <= 2)
                        v8 = AI_MainActor[(uint32_t)v8 - 1];
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(v8, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
                }
            }
            if (v7->des_axe)
            {
                if (v7->des_axe == 1)
                {
                    v5 = (float)v7->des_angle_sign;
                    Xa = v7->f_angle * v5;
                    AI_EvalFunc_OBJRotateLocalY_C(AI_gpst_CurrentGameObject, Xa);
                }
                else
                {
                    v4 = (float)v7->des_angle_sign;
                    Xb = v7->f_angle * v4;
                    AI_EvalFunc_OBJRotateLocalZ_c(AI_gpst_CurrentGameObject, Xb);
                }
            }
            else
            {
                v6 = (float)v7->des_angle_sign;
                X = v7->f_angle * v6;
                AI_EvalFunc_OBJRotateLocalX_C(AI_gpst_CurrentGameObject, X);
            }
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(256, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 256);
        }
        if (v7->des_test)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)14;
            TIME_Wait_C(2.0);
            return 0;
        }
    }
    if (v7->des_test == 1)
    {
        v7->f_angle_speed = v7->des_f_open_speed;
        v25 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Porte_Armoire_Porte_Armoire_init_Porte_Armoire_Porte_Armoire_mode_open);
        if (v25 != 1000)
            return v25;
    }
    else if (v7->des_test == 2)
    {
        v7->f_angle_speed = v7->des_f_close_speed;
        v26 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Porte_Armoire_Porte_Armoire_init_Porte_Armoire_Porte_Armoire_mode_close);
        if (v26 != 1000)
            return v26;
    }
    else
    {
        v24 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Porte_Armoire_Porte_Armoire_init_Porte_Armoire_Porte_Armoire_mode_wait);
        if (v24 != 1000)
            return v24;
    }
    return 1000;
}

int Porte_Armoire_Porte_Armoire_mode_open()
{
    Vec3* v0; // eax
    float X_4; // [esp+4h] [ebp-F8h]
    float X_4a; // [esp+4h] [ebp-F8h]
    float X_4b; // [esp+4h] [ebp-F8h]
    float X_4c; // [esp+4h] [ebp-F8h]
    float X_4d; // [esp+4h] [ebp-F8h]
    float v7; // [esp+8h] [ebp-F4h]
    float v8; // [esp+Ch] [ebp-F0h]
    float v9; // [esp+10h] [ebp-ECh]
    float v10; // [esp+14h] [ebp-E8h]
    float v11; // [esp+18h] [ebp-E4h]
    float v12; // [esp+1Ch] [ebp-E0h]
    float v13; // [esp+20h] [ebp-DCh]
    float v14; // [esp+24h] [ebp-D8h]
    float v15; // [esp+28h] [ebp-D4h]
    float v16; // [esp+2Ch] [ebp-D0h]
    float v17; // [esp+30h] [ebp-CCh]
    tScriptStruct_PorteArmoire* v18; // [esp+34h] [ebp-C8h]
    float v19; // [esp+3Ch] [ebp-C0h]
    float v20; // [esp+44h] [ebp-B8h]
    tGameObjectMain* v21; // [esp+4Ch] [ebp-B0h]
    tGameObjectMain* v22; // [esp+50h] [ebp-ACh]
    tGameObjectMain* v23; // [esp+54h] [ebp-A8h]
    tGameObjectMain* v24; // [esp+5Ch] [ebp-A0h]
    tGameObjectMain* v25; // [esp+60h] [ebp-9Ch]
    tGameObjectMain* v26; // [esp+64h] [ebp-98h]
    tGameObjectMain* v27; // [esp+6Ch] [ebp-90h]
    tGameObjectMain* v28; // [esp+70h] [ebp-8Ch]
    tGameObjectMain* v29; // [esp+74h] [ebp-88h]
    tGameObjectMain* v30; // [esp+7Ch] [ebp-80h]
    tGameObjectMain* v31; // [esp+80h] [ebp-7Ch]
    tGameObjectMain* v32; // [esp+84h] [ebp-78h]
    tGameObjectMain* a2; // [esp+88h] [ebp-74h]
    tGameObjectMain* v34; // [esp+8Ch] [ebp-70h]
    tGameObjectMain* v35; // [esp+94h] [ebp-68h]
    tGameObjectMain* v36; // [esp+98h] [ebp-64h]
    tGameObjectMain* v37; // [esp+9Ch] [ebp-60h]
    int v38; // [esp+C0h] [ebp-3Ch]
    float v39; // [esp+D4h] [ebp-28h]
    int v40; // [esp+D8h] [ebp-24h]
    int v41; // [esp+D8h] [ebp-24h]
    int i; // [esp+ECh] [ebp-10h]
    float v43; // [esp+F8h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v18 = (tScriptStruct_PorteArmoire*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v18 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Porte_Armoire_Porte_Armoire_mode_open_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_open_univVars_Flag_speciaux_sauve,
            (void*)Porte_Armoire_Porte_Armoire_mode_open_univVars,
            4488);
        Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_f_event_sound_time,
            (void*)Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao,
            704);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_i_event_sound_nbr,
            (void*)Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao,
            872);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_v_event_sound_pos,
            (void*)Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao,
            376);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_f_event_sound_size,
            (void*)Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao,
            620);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_o_event_sound_actor,
            (void*)Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao,
            788);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_i_event_sound_id,
            (void*)Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao,
            876);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_i_event_sound_next_id,
            (void*)Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao,
            960);
        fct_Porte_Armoire_Porte_Armoire_mode_open_Porte_Armoire_Porte_Armoire_mode_wait = (int)AI_ul_RealResolveOneRefFunction(0x72004001u);
    }
    if (v18->i_current_mode != 1)
    {
        v18->f_time_open = TIM_gf_MainClock - TIM_gf_LockTime;
        v18->i_current_mode = 1;
        v18->i_flag_porte_ouverte = 1;
        v18->f_angle_speed = v18->des_f_open_speed;
        if (v18->des_SF_porte_ouverte > 0)
            AI_EvalFunc_ARRBitSet_C(
                (int*)(Porte_Armoire_Porte_Armoire_mode_open_univVars_Flag_speciaux_sauve + 4),
                v18->des_SF_porte_ouverte);
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x100, 0);
        v37 = (tGameObjectMain*)v18->des_o_battant;
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
        if (v36)
        {
            v35 = (tGameObjectMain*)v18->des_o_battant;
            if (v35)
            {
                if ((unsigned int)v35 <= 2)
                    v35 = AI_MainActor[(uint32_t)v35 - 1];
                AI_EvalFunc_OBJ_CapaSet_C(v35, 0x100, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_CapaSet_C(0, 0x100, 0);
            }
        }
        if (v18->des_sound_event == 1 || v18->des_sound_event == 2)
        {
            for (i = 0;
                i < 20
                && *(float*)AI_EvalVarArray_C2(
                    (char*)Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_f_event_sound_time,
                    i) != 0.0;
                ++i)
            {
                ;
            }
            if (i != 20)
            {
                ++* (uint32_t*)Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_i_event_sound_nbr;
                v34 = (tGameObjectMain*)v18->des_sound_event_open_pos;
                if (v34)
                {
                    if ((unsigned int)v34 <= 2)
                        v34 = AI_MainActor[(uint32_t)v34 - 1];
                    a2 = v34;
                }
                else
                {
                    a2 = 0;
                }
                v0 = (Vec3*)AI_EvalVarArray_C3(
                    (char*)Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_v_event_sound_pos,
                    i);
                AI_EvalFunc_OBJGetPos_C(a2, v0);
                v17 = v18->des_sound_event_portee;
                *(float*)AI_EvalVarArray_C2((char*)Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_f_event_sound_size, i) = v17;
                v16 = v18->des_sound_event_duree;
                *(float*)AI_EvalVarArray_C2((char*)Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_f_event_sound_time, i) = v16;
                *(uint32_t*)(Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_o_event_sound_actor + 4 * i + 4) = (uint32_t)AI_gpst_CurrentGameObject;
                *(uint32_t*)(Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_i_event_sound_id + 4 * i + 4) = (*(uint32_t*)Porte_Armoire_Porte_Armoire_mode_open_GST_Global_gao_i_event_sound_next_id)++;
            }
        }
        v18->i_open_once = 0;
    }
    if ((v18->des_f_time_active_contenu == 0.0
        || AI_EvalFunc_TIMEElapsed_C(v18->f_time_open, v18->des_f_time_active_contenu))
        && !v18->i_open_once)
    {
        v18->i_open_once = 1;
        v32 = (tGameObjectMain*)v18->des_o_contenu;
        if (v32)
        {
            if ((unsigned int)v32 <= 2)
                v32 = AI_MainActor[(uint32_t)v32 - 1];
            v31 = v32;
        }
        else
        {
            v31 = 0;
        }
        if (v31)
        {
            v30 = (tGameObjectMain*)v18->des_o_contenu;
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
        }
        v29 = (tGameObjectMain*)v18->des_o_contenu1;
        if (v29)
        {
            if ((unsigned int)v29 <= 2)
                v29 = AI_MainActor[(uint32_t)v29 - 1];
            v28 = v29;
        }
        else
        {
            v28 = 0;
        }
        if (v28)
        {
            v27 = (tGameObjectMain*)v18->des_o_contenu1;
            if (v27)
            {
                if ((unsigned int)v27 <= 2)
                    v27 = AI_MainActor[(uint32_t)v27 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v27, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
            }
        }
        v26 = (tGameObjectMain*)v18->des_o_contenu2;
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
            v24 = (tGameObjectMain*)v18->des_o_contenu2;
            if (v24)
            {
                if ((unsigned int)v24 <= 2)
                    v24 = AI_MainActor[(uint32_t)v24 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v24, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
            }
        }
        v23 = (tGameObjectMain*)v18->des_o_contenu3;
        if (v23)
        {
            if ((unsigned int)v23 <= 2)
                v23 = AI_MainActor[(uint32_t)v23 - 1];
            v22 = v23;
        }
        else
        {
            v22 = 0;
        }
        if (v22)
        {
            v21 = (tGameObjectMain*)v18->des_o_contenu3;
            if (v21)
            {
                if ((unsigned int)v21 <= 2)
                    v21 = AI_MainActor[(uint32_t)v21 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v21, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
            }
        }
    }
    v18->f_angle_speed = v18->des_f_angle_acceleration * TIM_gf_dt + v18->f_angle_speed;
    v18->f_angle = v18->f_angle_speed * TIM_gf_dt + v18->f_angle;
    if (v18->f_angle >= 6.283185f)
        v18->f_angle = v18->f_angle - 6.283185f;
    if (v18->des_f_angle_min <= (double)v18->f_angle)
    {
        if (v18->des_f_angle_max >= (double)v18->f_angle)
        {
            v18->i_flag_2nd_trame = 0;
        }
        else
        {
            v12 = (float)fabs(v18->f_angle_speed);
            if (v12 > 0.5)
            {
                v41 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
                v43 = SND_f_GetVol(v41);
                v11 = (float)fabs(v18->f_angle_speed / 10.0f);
                X_4a = v43 * v11;
                SND_SetVol(v41, X_4a);
            }
            v18->f_angle = v18->des_f_angle_max;
            v19 = v18->des_f_rebound_max_friction * v18->f_angle_speed;
            if (v18->des_f_rebound_speed > (double)v19)
                v10 = v18->des_f_rebound_max_friction * v18->f_angle_speed;
            else
                v10 = v18->des_f_rebound_speed;
            v18->f_angle_speed = -v10;
            if (v18->i_flag_2nd_trame)
            {
                if (v18->des_propose_close)
                {
                    v38 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Porte_Armoire_Porte_Armoire_mode_open_Porte_Armoire_Porte_Armoire_mode_wait);
                    if (v38 != 1000)
                        return v38;
                }
                else
                {
                    AI_EvalFunc_AITrackCurStop_C();
                }
            }
            v18->i_flag_2nd_trame = 1;
        }
    }
    else
    {
        v15 = (float)fabs(v18->f_angle_speed);
        if (v15 > 0.5)
        {
            v40 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
            v39 = SND_f_GetVol(v40);
            v14 = (float)fabs(v18->f_angle_speed / 10.0f);
            X_4 = v39 * v14;
            SND_SetVol(v40, X_4);
        }
        v18->f_angle = v18->des_f_angle_min;
        v20 = v18->des_f_rebound_min_friction * v18->f_angle_speed;
        if (v18->des_f_rebound_speed > (double)v20)
            v13 = v18->des_f_rebound_min_friction * v18->f_angle_speed;
        else
            v13 = v18->des_f_rebound_speed;
        v18->f_angle_speed = -v13;
        v18->i_flag_2nd_trame = 0;
    }
    OBJ_RestoreInitialPos(AI_gpst_CurrentGameObject);
    if (v18->des_axe)
    {
        if (v18->des_axe == 1)
        {
            v8 = (float)v18->des_angle_sign;
            X_4c = v18->f_angle * v8;
            AI_EvalFunc_OBJRotateLocalY_C(AI_gpst_CurrentGameObject, X_4c);
        }
        else
        {
            v7 = (float)v18->des_angle_sign;
            X_4d = v18->f_angle * v7;
            AI_EvalFunc_OBJRotateLocalZ_c(AI_gpst_CurrentGameObject, X_4d);
        }
    }
    else
    {
        v9 = (float)v18->des_angle_sign;
        X_4b = v18->f_angle * v9;
        AI_EvalFunc_OBJRotateLocalX_C(AI_gpst_CurrentGameObject, X_4b);
    }
    return 1000;
}

int Porte_Armoire_Porte_Armoire_mode_close()
{
    Vec3* v0; // eax
    float X_4; // [esp+4h] [ebp-F4h]
    float X_4a; // [esp+4h] [ebp-F4h]
    float X_4b; // [esp+4h] [ebp-F4h]
    float X_4c; // [esp+4h] [ebp-F4h]
    float X_4d; // [esp+4h] [ebp-F4h]
    float v7; // [esp+8h] [ebp-F0h]
    float v8; // [esp+Ch] [ebp-ECh]
    float v9; // [esp+10h] [ebp-E8h]
    float v10; // [esp+14h] [ebp-E4h]
    float v11; // [esp+18h] [ebp-E0h]
    float v12; // [esp+1Ch] [ebp-DCh]
    float v13; // [esp+20h] [ebp-D8h]
    float v14; // [esp+24h] [ebp-D4h]
    float v15; // [esp+28h] [ebp-D0h]
    float v16; // [esp+2Ch] [ebp-CCh]
    float v17; // [esp+30h] [ebp-C8h]
    float v18; // [esp+34h] [ebp-C4h]
    tScriptStruct_PorteArmoire* v19; // [esp+38h] [ebp-C0h]
    float v20; // [esp+40h] [ebp-B8h]
    float v21; // [esp+48h] [ebp-B0h]
    tGameObjectMain* a2; // [esp+4Ch] [ebp-ACh]
    tGameObjectMain* v23; // [esp+50h] [ebp-A8h]
    tGameObjectMain* v24; // [esp+58h] [ebp-A0h]
    tGameObjectMain* v25; // [esp+5Ch] [ebp-9Ch]
    tGameObjectMain* v26; // [esp+60h] [ebp-98h]
    tGameObjectMain* v27; // [esp+68h] [ebp-90h]
    tGameObjectMain* v28; // [esp+6Ch] [ebp-8Ch]
    tGameObjectMain* v29; // [esp+70h] [ebp-88h]
    tGameObjectMain* v30; // [esp+78h] [ebp-80h]
    tGameObjectMain* v31; // [esp+7Ch] [ebp-7Ch]
    tGameObjectMain* v32; // [esp+80h] [ebp-78h]
    tGameObjectMain* v33; // [esp+88h] [ebp-70h]
    tGameObjectMain* v34; // [esp+8Ch] [ebp-6Ch]
    tGameObjectMain* v35; // [esp+90h] [ebp-68h]
    int v36; // [esp+ACh] [ebp-4Ch]
    float v37; // [esp+B8h] [ebp-40h]
    int v38; // [esp+D0h] [ebp-28h]
    int v39; // [esp+D0h] [ebp-28h]
    float v40; // [esp+D8h] [ebp-20h]
    int i; // [esp+DCh] [ebp-1Ch]
    int v42; // [esp+E0h] [ebp-18h]
    int v43; // [esp+E8h] [ebp-10h]
    float v44; // [esp+F0h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v19 = (tScriptStruct_PorteArmoire*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v19 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Porte_Armoire_Porte_Armoire_mode_close_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_close_univVars_Flag_speciaux_sauve,
            (void*)Porte_Armoire_Porte_Armoire_mode_close_univVars,
            4488);
        Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_f_event_sound_time,
            (void*)Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao,
            704);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_i_event_sound_nbr,
            (void*)Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao,
            872);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_v_event_sound_pos,
            (void*)Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao,
            376);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_f_event_sound_size,
            (void*)Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao,
            620);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_o_event_sound_actor,
            (void*)Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao,
            788);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_i_event_sound_id,
            (void*)Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao,
            876);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_i_event_sound_next_id,
            (void*)Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao,
            960);
        fct_Porte_Armoire_Porte_Armoire_mode_close_Porte_Armoire_Porte_Armoire_mode_wait = (int)AI_ul_RealResolveOneRefFunction(0x72004001u);
    }
    if (v19->i_current_mode != 2)
    {
        v19->i_current_mode = 2;
        v19->i_flag_porte_ouverte = 0;
        v19->f_angle_speed = v19->des_f_close_speed;
        if (v19->des_SF_porte_ouverte > 0)
            AI_EvalFunc_ARRBitClear_C(
                (int*)(Porte_Armoire_Porte_Armoire_mode_close_univVars_Flag_speciaux_sauve + 4),
                v19->des_SF_porte_ouverte);
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
        v43 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(2, -1.0);
        v42 = SND_l_GetFreq(v43);
        v40 = AI_EvalFunc_MATHRandFloat_C(0.80000001f, 1.0);
        v18 = (float)v42;
        SND_SetFreq(v43, (unsigned __int64)(v18 * v40));
        v35 = (tGameObjectMain*)v19->des_o_contenu;
        if (v35)
        {
            if ((unsigned int)v35 <= 2)
                v35 = AI_MainActor[(uint32_t)v35 - 1];
            v34 = v35;
        }
        else
        {
            v34 = 0;
        }
        if (v34 && v19->i_desactive_qd_ferme_contenu)
        {
            v33 = (tGameObjectMain*)v19->des_o_contenu;
            if (v33)
            {
                if ((unsigned int)v33 <= 2)
                    v33 = AI_MainActor[(uint32_t)v33 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v33, 1);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
            }
        }
        v32 = (tGameObjectMain*)v19->des_o_contenu1;
        if (v32)
        {
            if ((unsigned int)v32 <= 2)
                v32 = AI_MainActor[(uint32_t)v32 - 1];
            v31 = v32;
        }
        else
        {
            v31 = 0;
        }
        if (v31 && v19->i_desactive_qd_ferme_contenu1)
        {
            v30 = (tGameObjectMain*)v19->des_o_contenu1;
            if (v30)
            {
                if ((unsigned int)v30 <= 2)
                    v30 = AI_MainActor[(uint32_t)v30 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v30, 1);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
            }
        }
        v29 = (tGameObjectMain*)v19->des_o_contenu2;
        if (v29)
        {
            if ((unsigned int)v29 <= 2)
                v29 = AI_MainActor[(uint32_t)v29 - 1];
            v28 = v29;
        }
        else
        {
            v28 = 0;
        }
        if (v28 && v19->i_desactive_qd_ferme_contenu2)
        {
            v27 = (tGameObjectMain*)v19->des_o_contenu2;
            if (v27)
            {
                if ((unsigned int)v27 <= 2)
                    v27 = AI_MainActor[(uint32_t)v27 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v27, 1);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
            }
        }
        v26 = (tGameObjectMain*)v19->des_o_contenu3;
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
        if (v25 && v19->i_desactive_qd_ferme_contenu3)
        {
            v24 = (tGameObjectMain*)v19->des_o_contenu3;
            if (v24)
            {
                if ((unsigned int)v24 <= 2)
                    v24 = AI_MainActor[(uint32_t)v24 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v24, 1);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
            }
        }
        if (v19->des_sound_event == 1 || v19->des_sound_event == 3)
        {
            for (i = 0;
                i < 20
                && *(float*)AI_EvalVarArray_C2(
                    (char*)Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_f_event_sound_time,
                    i) != 0.0;
                ++i)
            {
                ;
            }
            if (i != 20)
            {
                ++* (uint32_t*)Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_i_event_sound_nbr;
                v23 = (tGameObjectMain*)v19->des_sound_event_close_pos;
                if (v23)
                {
                    if ((unsigned int)v23 <= 2)
                        v23 = AI_MainActor[(uint32_t)v23 - 1];
                    a2 = v23;
                }
                else
                {
                    a2 = 0;
                }
                v0 = (Vec3*)AI_EvalVarArray_C3(
                    (char*)Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_v_event_sound_pos,
                    i);
                AI_EvalFunc_OBJGetPos_C(a2, v0);
                v17 = v19->des_sound_event_portee;
                *(float*)AI_EvalVarArray_C2(
                    (char*)Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_f_event_sound_size,
                    i) = v17;
                v16 = v19->des_sound_event_duree;
                *(float*)AI_EvalVarArray_C2(
                    (char*)Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_f_event_sound_time,
                    i) = v16;
                *(uint32_t*)(Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_o_event_sound_actor + 4 * i + 4) = (uint32_t)AI_gpst_CurrentGameObject;
                *(uint32_t*)(Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_i_event_sound_id + 4 * i + 4) = (*(uint32_t*)Porte_Armoire_Porte_Armoire_mode_close_GST_Global_gao_i_event_sound_next_id)++;
            }
        }
    }
    v19->f_angle_speed = v19->f_angle_speed - v19->des_f_angle_acceleration * TIM_gf_dt;
    v19->f_angle = v19->f_angle_speed * TIM_gf_dt + v19->f_angle;
    if (v19->f_angle >= 6.283185f)
        v19->f_angle = v19->f_angle - 6.283185f;
    if (v19->f_angle >= 0.0f)
    {
        if (v19->des_f_angle_max >= (double)v19->f_angle)
        {
            v19->i_flag_2nd_trame = 0;
        }
        else
        {
            v12 = (float)fabs(v19->f_angle_speed);
            if (v12 > 0.5)
            {
                v39 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
                v37 = SND_f_GetVol(v39);
                v11 = (float)fabs(v19->f_angle_speed / 10.0f);
                X_4a = v37 * v11;
                SND_SetVol(v39, X_4a);
            }
            v19->f_angle = v19->des_f_angle_max;
            v20 = v19->des_f_rebound_max_friction * v19->f_angle_speed;
            if (v19->des_f_rebound_speed > (double)v20)
                v10 = v19->des_f_rebound_max_friction * v19->f_angle_speed;
            else
                v10 = v19->des_f_rebound_speed;
            v19->f_angle_speed = -v10;
            v19->i_flag_2nd_trame = 0;
        }
    }
    else
    {
        v15 = (float)fabs(v19->f_angle_speed);
        if (v15 > 0.5)
        {
            v38 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
            v44 = SND_f_GetVol(v38);
            v14 = (float)fabs(v19->f_angle_speed / 10.0f);
            X_4 = v44 * v14;
            SND_SetVol(v38, X_4);
        }
        v19->f_angle = 0.0;
        v21 = v19->des_f_rebound_min_friction * v19->f_angle_speed;
        if (v19->des_f_rebound_speed > (double)v21)
            v13 = v19->des_f_rebound_min_friction * v19->f_angle_speed;
        else
            v13 = v19->des_f_rebound_speed;
        v19->f_angle_speed = -v13;
        if (v19->i_flag_2nd_trame)
        {
            v36 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Porte_Armoire_Porte_Armoire_mode_close_Porte_Armoire_Porte_Armoire_mode_wait);
            if (v36 != 1000)
                return v36;
        }
        v19->i_flag_2nd_trame = 1;
    }
    OBJ_RestoreInitialPos(AI_gpst_CurrentGameObject);
    if (v19->des_axe)
    {
        if (v19->des_axe == 1)
        {
            v8 = (float)v19->des_angle_sign;
            X_4c = v19->f_angle * v8;
            AI_EvalFunc_OBJRotateLocalY_C(AI_gpst_CurrentGameObject, X_4c);
        }
        else
        {
            v7 = (float)v19->des_angle_sign;
            X_4d = v19->f_angle * v7;
            AI_EvalFunc_OBJRotateLocalZ_c(AI_gpst_CurrentGameObject, X_4d);
        }
    }
    else
    {
        v9 = (float)v19->des_angle_sign;
        X_4b = v19->f_angle * v9;
        AI_EvalFunc_OBJRotateLocalX_C(AI_gpst_CurrentGameObject, X_4b);
    }
    return 1000;
}


int Porte_Armoire_Porte_Armoire_mode_wait()
{
    float v1; // [esp+8h] [ebp-310h]
    float v2; // [esp+Ch] [ebp-30Ch]
    float v3; // [esp+10h] [ebp-308h]
    float v4; // [esp+14h] [ebp-304h]
    float v5; // [esp+18h] [ebp-300h]
    float v6; // [esp+1Ch] [ebp-2FCh]
    tScriptStruct_PorteArmoire* v7; // [esp+20h] [ebp-2F8h]
    float v8; // [esp+2Ch] [ebp-2ECh]
    float v9; // [esp+38h] [ebp-2E0h]
    tGameObjectMain* v10; // [esp+5Ch] [ebp-2BCh]
    tGameObjectMain* v11; // [esp+60h] [ebp-2B8h]
    tGameObjectMain* v12; // [esp+64h] [ebp-2B4h]
    float v13; // [esp+80h] [ebp-298h]
    float v14; // [esp+8Ch] [ebp-28Ch]
    float v15; // [esp+90h] [ebp-288h]
    float v16; // [esp+B0h] [ebp-268h]
    Vec3 v17; // [esp+BCh] [ebp-25Ch] BYREF
    Vec3 v18; // [esp+D4h] [ebp-244h] BYREF
    float v19; // [esp+E0h] [ebp-238h]
    float v20; // [esp+E8h] [ebp-230h]
    float v21; // [esp+ECh] [ebp-22Ch]
    float v22; // [esp+F0h] [ebp-228h]
    float v23; // [esp+F4h] [ebp-224h]
    float v24; // [esp+F8h] [ebp-220h]
    float v25; // [esp+FCh] [ebp-21Ch]
    float v26; // [esp+100h] [ebp-218h]
    Vec3 v27; // [esp+104h] [ebp-214h] BYREF
    int v28; // [esp+110h] [ebp-208h]
    Vec3 v29; // [esp+114h] [ebp-204h] BYREF
    Vec3 v30; // [esp+120h] [ebp-1F8h] BYREF
    int v31; // [esp+12Ch] [ebp-1ECh]
    Vec3 v32; // [esp+130h] [ebp-1E8h] BYREF
    Vec3 v33; // [esp+13Ch] [ebp-1DCh] BYREF
    Vec3 v34; // [esp+148h] [ebp-1D0h] BYREF
    Vec3 v35; // [esp+154h] [ebp-1C4h] BYREF
    Vec3 v36; // [esp+160h] [ebp-1B8h] BYREF
    Vec3 v37; // [esp+174h] [ebp-1A4h] BYREF
    Vec3 v38; // [esp+180h] [ebp-198h]
    Vec3 a1; // [esp+19Ch] [ebp-17Ch] BYREF
    Vec3 v41; // [esp+1ACh] [ebp-16Ch] BYREF
    Vec3 dest; // [esp+1B8h] [ebp-160h] BYREF
    int v43; // [esp+1C4h] [ebp-154h]
    int v44; // [esp+1C8h] [ebp-150h]
    int v45; // [esp+1CCh] [ebp-14Ch]
    Vec3 v46; // [esp+1D0h] [ebp-148h] BYREF
    float v47; // [esp+1DCh] [ebp-13Ch]
    Vec3 v48; // [esp+1E0h] [ebp-138h] BYREF
    Vec3 a3; // [esp+1ECh] [ebp-12Ch] BYREF
    Vec3 v50; // [esp+1F8h] [ebp-120h] BYREF
    Vec3 v51; // [esp+204h] [ebp-114h] BYREF
    Vec3 v52; // [esp+210h] [ebp-108h] BYREF
    float v53; // [esp+21Ch] [ebp-FCh]
    float v54; // [esp+220h] [ebp-F8h]
    float v55; // [esp+224h] [ebp-F4h]
    Vec3 v56; // [esp+228h] [ebp-F0h]
    float v57; // [esp+234h] [ebp-E4h]
    float v58; // [esp+238h] [ebp-E0h]
    int v59; // [esp+23Ch] [ebp-DCh]
    float v60; // [esp+240h] [ebp-D8h]
    int v61; // [esp+248h] [ebp-D0h]
    Vec3 v62; // [esp+24Ch] [ebp-CCh] BYREF
    Vec3 v63; // [esp+258h] [ebp-C0h] BYREF
    int v64; // [esp+264h] [ebp-B4h]
    Vec3 v65; // [esp+268h] [ebp-B0h] BYREF
    Vec3 v66; // [esp+274h] [ebp-A4h]
    Vec3 v67; // [esp+280h] [ebp-98h] BYREF
    int v68; // [esp+28Ch] [ebp-8Ch]
    Vec3 v70; // [esp+2A0h] [ebp-78h]
    int v71; // [esp+2ACh] [ebp-6Ch]
    int v72; // [esp+2B0h] [ebp-68h]
    void* Src; // [esp+2B4h] [ebp-64h]
    float v74; // [esp+2B8h] [ebp-60h]
    float v75; // [esp+2BCh] [ebp-5Ch]
    float v76; // [esp+2C0h] [ebp-58h]
    tScriptStruct_PorteArmoire* v77; // [esp+2C4h] [ebp-54h]
    BOOL v78; // [esp+2C8h] [ebp-50h]
    Vec3 v79; // [esp+2CCh] [ebp-4Ch] BYREF
    Vec3 v80; // [esp+2D8h] [ebp-40h] BYREF
    void* v81; // [esp+2E8h] [ebp-30h]
    float v82; // [esp+2ECh] [ebp-2Ch]
    float v83; // [esp+2F0h] [ebp-28h]
    Vec3 v84; // [esp+2F4h] [ebp-24h]
    Vec3 v85; // [esp+300h] [ebp-18h] BYREF
    int v86; // [esp+30Ch] [ebp-Ch]
    float v87; // [esp+310h] [ebp-8h]

    v56.x = 1.0;
    v56.y = 0.0;
    v56.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_PorteArmoire*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    v77 = v7;
    v19 = 0.0;
    v68 = 0;
    v59 = 0;
    v78 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Porte_Armoire_Porte_Armoire_mode_wait_Porte_Armoire_Porte_Armoire_mode_close = (int)AI_ul_RealResolveOneRefFunction(0x72004021u);
        Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao_current_pm_mode,
            (void*)Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao_current_pm_priority,
            (void*)Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao,
            576);
        Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_i_sally_priority,
            (void*)Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao,
            396);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_o_sally_contextuel,
            (void*)Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao,
            388);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_i_sally_action,
            (void*)Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao,
            412);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_v_sally_offset,
            (void*)Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao,
            400);
        AI2C_ResolveBuffer(
            &Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_i_force_interface,
            (void*)Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao,
            3292);
        fct_Porte_Armoire_Porte_Armoire_mode_wait_Porte_Armoire_Porte_Armoire_mode_open = (int)AI_ul_RealResolveOneRefFunction(0x72004002u);
    }
    if (v77->i_current_mode)
    {
        v77->i_current_mode = 0;
        AI_EvalFunc_MSGClear_C(AI_gpst_CurrentGameObject);
    }
    if (v77->i_flag_porte_ouverte)
    {
        if (!AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
        {
            v72 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Porte_Armoire_Porte_Armoire_mode_wait_Porte_Armoire_Porte_Armoire_mode_close);
            if (v72 != 1000)
                return v72;
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v62);
        a1 = v62;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v27);
        v6 = (float)v77->des_angle_sign;
        v16 = v77->des_f_porte_largeur * v6;
        v23 = v27.x * v16;
        v24 = v27.y * v16;
        v25 = v27.z * v16;
        a1.x = a1.x - v23;
        a1.y = a1.y - v24;
        a1.z = a1.z - v25;
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao, &a3);
        a1.x = a1.x - a3.x;
        a1.y = a1.y - a3.y;
        a1.z = a1.z - a3.z;
        v5 = (float)fabs(a1.z);
        v83 = v5;
        if (v5 < 2.0)
        {
            a1.z = 0.0;
            v19 = 0.0f * 0.0f + a1.y * a1.y + a1.x * a1.x;
            if (v19 < 0.36000001f)
            {
                v59 = 0;
                MATH_NormalizeEqualVector(&a1);
                AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao, &v63);
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v79);
                v38.x = -v79.x;
                v38.y = -v79.y;
                v38.z = -v79.z;
                v58 = -v79.z * v63.z
                    + -v79.y * v63.y
                    + v63.x * -v79.x;
                if (v58 <= 0.0
                    || (AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v29),
                        v84.x = -v29.x,
                        v84.y = -v29.y,
                        v84.z = -v29.z,
                        v60 = -v29.z * a1.z + -v29.y * a1.y + a1.x * -v29.x,
                        v60 <= 0.0))
                {
                    AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao, &v37);
                    AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v46);
                    v15 = (float)v77->des_angle_sign;
                    v20 = v46.x * v15;
                    v21 = v46.y * v15;
                    v22 = v46.z * v15;
                    v47 = v22 * v37.z + v21 * v37.y + v37.x * v20;
                    if (v77->des_f_cone < (double)v47)
                    {
                        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v65);
                        v14 = (float)v77->des_angle_sign;
                        v53 = v65.x * v14;
                        v54 = v65.y * v14;
                        v55 = v65.z * v14;
                        v87 = v55 * a1.z + v54 * a1.y + a1.x * v53;
                        if (v77->des_f_cone < (double)v87)
                            v59 = 2;
                    }
                }
                else
                {
                    v59 = 1;
                }
                if (v59)
                {
                    v64 = (unsigned __int16)*(uint32_t*)(Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao + 4);
                    if ((v64 & 1) != 0)
                        v78 = *(uint32_t*)Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao_current_pm_mode == 7
                        || *(int*)Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao_current_pm_priority < 20;
                    else
                        v78 = 0;
                    if (v78 && *(float*)Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_i_sally_priority < 20.0)
                    {
                        *(uint32_t*)Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_o_sally_contextuel = (uint32_t)AI_gpst_CurrentGameObject;
                        *(uint32_t*)Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_i_sally_priority = 1101004800;
                        *(uint32_t*)Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_i_sally_action = 13;
                        *(uint32_t*)Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_v_sally_offset = 0;
                        *(uint32_t*)(Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_v_sally_offset + 4) = 0;
                        *(uint32_t*)(Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_v_sally_offset + 8) = 0;
                        *(uint32_t*)Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_i_force_interface = 1;
                    }
                }
            }
        }
    }
    else
    {
        if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(256))
        {
            v28 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Porte_Armoire_Porte_Armoire_mode_wait_Porte_Armoire_Porte_Armoire_mode_open);
            if (v28 != 1000)
                return v28;
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v80);
        a1 = v80;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v48);
        v4 = (float)v77->des_angle_sign;
        v13 = v77->des_f_porte_largeur * v4;
        v74 = v48.x * v13;
        v75 = v48.y * v13;
        v76 = v48.z * v13;
        a1.x = a1.x - v74;
        a1.y = a1.y - v75;
        a1.z = a1.z - v76;
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao, &v51);
        a1.x = a1.x - v51.x;
        a1.y = a1.y - v51.y;
        a1.z = a1.z - v51.z;
        v3 = (float)fabs(a1.z);
        v57 = v3;
        if (v3 < 2.0f)
        {
            a1.z = 0.0;
            v19 = 0.0f * 0.0f + a1.y * a1.y + a1.x * a1.x;
            if (v77->des_f_dist_open > (double)v19)
            {
                AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao, &v33);
                AI_EvalFunc_OBJ_SightInitGet_C(AI_gpst_CurrentGameObject, &v85);
                v66.x = -v85.x;
                v66.y = -v85.y;
                v66.z = -v85.z;
                v26 = -v85.z * v33.z + -v85.y * v33.y + v33.x * -v85.x;
                if (v77->des_f_cone < (double)v26)
                {
                    AI_EvalFunc_MATHVecSetHorzNormalize_C(&a1);
                    AI_EvalFunc_OBJ_SightInitGet_C(AI_gpst_CurrentGameObject, &v36);
                    v70.x = -v36.x;
                    v70.y = -v36.y;
                    v70.z = -v36.z;
                    v82 = -v36.z * a1.z + -v36.y * a1.y + a1.x * -v36.x;
                    if (v77->des_f_cone < (double)v82)
                    {
                        v61 = (unsigned __int16)*(uint32_t*)(Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao + 4);
                        if ((v61 & 1) != 0)
                            v78 = *(uint32_t*)Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao_current_pm_mode == 7
                            || *(int*)Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao_current_pm_priority < 20;
                        else
                            v78 = 0;
                        v43 = 0;
                        v44 = 0;
                        v45 = 0;
                        v56.x = 0.0;
                        v56.y = 0.0;
                        v56.z = 0.0;
                        v12 = (tGameObjectMain*)v77->o_regarde_la;
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
                        if (v11)
                        {
                            v10 = (tGameObjectMain*)v77->o_regarde_la;
                            if (v10)
                            {
                                if ((unsigned int)v10 <= 2)
                                    v10 = AI_MainActor[(uint32_t)v10 - 1];
                                AI_EvalFunc_OBJGetPos_C(v10, &v67);
                            }
                            else
                            {
                                AI_EvalFunc_OBJGetPos_C(0, &v67);
                            }
                            AI_EvalFunc_OBJGetPos_C_CURRENT(&v34);
                            MATH_SubstractVec(&dest, &v67, &v34);
                            v56 = dest;
                        }
                        if (v78 && *(float*)Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_i_sally_priority < 10.0)
                        {
                            *(uint32_t*)Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_o_sally_contextuel = (uint32_t)AI_gpst_CurrentGameObject;
                            *(uint32_t*)Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_i_sally_priority = 0x41200000;
                            *(uint32_t*)Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_i_sally_action = 15;
                            *(Vec3*)Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_v_sally_offset = v56;
                            *(uint32_t*)Porte_Armoire_Porte_Armoire_mode_wait_GST_Board_panel_gao_i_force_interface = 1;
                        }
                    }
                }
            }
        }
    }
    Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v77->m_msg, Src, sizeof(v77->m_msg));
    while (v77->m_msg.msgID)
    {
        if (v77->m_msg.msgID == 37)
        {
            v68 = v77->m_msg.Int1 & 7;
            a1 = v77->m_msg.v1;
            if (v68 > 2)
                v31 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(AI_gpst_CurrentGameObject, 0, -1.0, 0.69999999f, 1.0);
            if (v77->i_flag_porte_ouverte)
            {
                v71 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Porte_Armoire_Porte_Armoire_mode_wait_Porte_Armoire_Porte_Armoire_mode_close);
                if (v71 != 1000)
                    return v71;
            }
            else
            {
                v86 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Porte_Armoire_Porte_Armoire_mode_wait_Porte_Armoire_Porte_Armoire_mode_open);
                if (v86 != 1000)
                    return v86;
            }
        }
        else if (v77->m_msg.msgID == 10)
        {
            if (v77->i_flag_porte_ouverte)
            {
                v77->m_msg.msgID = 7;
                v77->m_msg.gao = AI_gpst_CurrentGameObject;
                v77->m_msg.gao2 = 0;
                v77->m_msg.Int1 = 0;
                v77->m_msg.Int2 = 0;
                v77->m_msg.f2 = 0.0;
                v77->m_msg.f1 = 0.0;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v50);
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v32);
                v2 = (float)v77->des_angle_sign;
                v9 = v77->des_f_porte_largeur * v2;
                v41.x = v32.x * v9;
                v41.y = v32.y * v9;
                v41.z = v32.z * v9;
                MATH_SubstractVec(&v18, &v50, &v41);
                v77->m_msg.v1 = v18;
            }
            else
            {
                v77->m_msg.msgID = 21;
                v77->m_msg.gao = AI_gpst_CurrentGameObject;
                v77->m_msg.gao2 = AI_gpst_CurrentGameObject;
                v77->m_msg.Int1 = 1;
                v77->m_msg.Int2 = 0;
                v77->m_msg.f2 = 0.0;
                v77->m_msg.f1 = 0.0;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v30);
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v52);
                v1 = (float)v77->des_angle_sign;
                v8 = v77->des_f_porte_largeur * v1;
                v17.x = v52.x * v8;
                v17.y = v52.y * v8;
                v17.z = v52.z * v8;
                MATH_SubstractVec(&v35, &v30, &v17);
                v77->m_msg.v1 = v35;
            }
            v77->m_msg.v2.x = 0.0;
            v77->m_msg.v2.y = 0.0;
            v77->m_msg.v2.z = 0.0;
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)Porte_Armoire_Porte_Armoire_mode_wait__PJ_Sally_gao, &v77->m_msg);
        }
        v81 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v77->m_msg, v81, sizeof(v77->m_msg));
    }
    return 1000;
}