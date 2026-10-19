#include "ai_compiledscripts_inc.h"

//porte_po_init
int fct_porte_po_init_porte_po_reflex;
int porte_po_init__PJ_Sally_gao;
int fct_porte_po_init_porte_po_mode_indy_ARec;
int fct_porte_po_init_porte_po_mode_indy;
int porte_po_init_univVars;
int porte_po_init_univVars_Flag_speciaux_sauve;
int fct_porte_po_init_porte_po_ferme_toi;
int fct_porte_po_init_porte_po_ouvre_toi;
int fct_porte_po_init_porte_po_etat_ferme;

//porte_po_ferme_toi
int fct_porte_po_ferme_toi_porte_po_paf;
int porte_po_ferme_toi_GST_Global_gao;
int porte_po_ferme_toi_GST_Global_gao_i_map_just_reset;
int porte_po_ferme_toi_univVars;
int porte_po_ferme_toi_univVars_Flag_speciaux_sauve;
int fct_porte_po_ferme_toi_porte_po_etat_ferme;

//porte_po_mode_indy_ARec
int porte_po_mode_indy_ARec_PJ_Sally_gao;
int porte_po_mode_indy_ARec_PJ_Sally_gao_i_stealth_accroupi;

//porte_po_etat_ouverte
int fct_porte_po_etat_ouverte_porte_po_check_separation;
int porte_po_etat_ouverte_GST_Global_gao;
int porte_po_etat_ouverte_GST_Global_gao_f_event_sound_time;
int porte_po_etat_ouverte_GST_Global_gao_i_event_sound_nbr;
int porte_po_etat_ouverte_GST_Global_gao_v_event_sound_pos;
int porte_po_etat_ouverte_GST_Global_gao_f_event_sound_size;
int porte_po_etat_ouverte_GST_Global_gao_o_event_sound_actor;
int porte_po_etat_ouverte_GST_Global_gao_i_event_sound_id;
int porte_po_etat_ouverte_GST_Global_gao_i_event_sound_next_id;
int fct_porte_po_etat_ouverte_porte_po_ferme_toi;

//porte_po_check_separation
int porte_po_check_separation__PJ_Sally_gao;
int porte_po_check_separation_univVars;
int porte_po_check_separation_univVars_Perso_ID_status;
int porte_po_check_separation_univVars_Perso_ID_actor;

//porte_po_etat_ferme
int porte_po_etat_ferme_GST_Global_gao;
int porte_po_etat_ferme_GST_Global_gao_f_event_sound_time;
int porte_po_etat_ferme_GST_Global_gao_i_event_sound_nbr;
int porte_po_etat_ferme_GST_Global_gao_v_event_sound_pos;
int porte_po_etat_ferme_GST_Global_gao_f_event_sound_size;
int porte_po_etat_ferme_GST_Global_gao_o_event_sound_actor;
int porte_po_etat_ferme_GST_Global_gao_i_event_sound_id;
int porte_po_etat_ferme_GST_Global_gao_i_event_sound_next_id;
int fct_porte_po_etat_ferme_porte_po_ouvre_toi;

//porte_po_ouvre_toi
int porte_po_ouvre_toi_GST_Global_gao;
int porte_po_ouvre_toi_GST_Global_gao_i_map_just_reset;
int porte_po_ouvre_toi_univVars;
int porte_po_ouvre_toi_univVars_Flag_speciaux_sauve;
int fct_porte_po_ouvre_toi_porte_po_etat_ouverte;

int porte_po_init()
{
    tScriptStruct_Porte* v1; // [esp+0h] [ebp-7Ch]
    tGameObjectMain* v2; // [esp+8h] [ebp-74h]
    tGameObjectMain* v3; // [esp+10h] [ebp-6Ch]
    tGameObjectMain* v4; // [esp+14h] [ebp-68h]
    tGameObjectMain* v5; // [esp+18h] [ebp-64h]
    tGameObjectMain* v6; // [esp+24h] [ebp-58h]
    tGameObjectMain* v7; // [esp+28h] [ebp-54h]
    tGameObjectMain* v8; // [esp+2Ch] [ebp-50h]
    tGameObjectMain* v9; // [esp+30h] [ebp-4Ch]
    tGameObjectMain* v10; // [esp+34h] [ebp-48h]
    int v11; // [esp+40h] [ebp-3Ch]
    Vec3 v12; // [esp+48h] [ebp-34h] BYREF
    int v13; // [esp+58h] [ebp-24h]
    int v14; // [esp+5Ch] [ebp-20h]
    int v15; // [esp+60h] [ebp-1Ch]
    tScriptStruct_Porte* v16; // [esp+68h] [ebp-14h]
    Vec3 a3; // [esp+6Ch] [ebp-10h] BYREF
    int v18; // [esp+78h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Porte*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v16 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_porte_po_init_porte_po_reflex = (int)AI_ul_RealResolveOneRefFunction(0x26000748u);
        porte_po_init__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        fct_porte_po_init_porte_po_mode_indy_ARec = (int)AI_ul_RealResolveOneRefFunction(0x260035B5u);
        fct_porte_po_init_porte_po_mode_indy = (int)AI_ul_RealResolveOneRefFunction(0x260035B1u);
        porte_po_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&porte_po_init_univVars_Flag_speciaux_sauve, (void*)porte_po_init_univVars, 4488);
        fct_porte_po_init_porte_po_ouvre_toi = (int)AI_ul_RealResolveOneRefFunction(0x7100BF42u);
        fct_porte_po_init_porte_po_ferme_toi = (int)AI_ul_RealResolveOneRefFunction(0x7100BF43u);
        fct_porte_po_init_porte_po_etat_ferme = (int)AI_ul_RealResolveOneRefFunction(0x7100BF02u);
    }
    v10 = (tGameObjectMain*)v16->des_sound_event_open_pos;
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
    if (!v9)
        v16->des_sound_event_open_pos = (int)AI_gpst_CurrentGameObject;
    v8 = (tGameObjectMain*)v16->des_sound_event_close_pos;
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
    if (!v7)
        v16->des_sound_event_close_pos = (int)AI_gpst_CurrentGameObject;
    if (v16->i_sound_BR)
        AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_porte_po_init_porte_po_reflex);
    if (v16->type_de_fonctionnement == 2)
    {
        v16->f_epaisseur_porte = v16->f_epaisseur_porte * 0.5f;
        v6 = (tGameObjectMain*)porte_po_init__PJ_Sally_gao;
        if (porte_po_init__PJ_Sally_gao)
        {
            if ((unsigned int)porte_po_init__PJ_Sally_gao <= 2)
                v6 = AI_MainActor[porte_po_init__PJ_Sally_gao - 1];
            AI_EvalFunc_AICBAdd_C(AI_gpst_CurrentGameObject, v6, 2, (tAIFunction*)fct_porte_po_init_porte_po_mode_indy_ARec);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(AI_gpst_CurrentGameObject, 0, 2, (tAIFunction*)fct_porte_po_init_porte_po_mode_indy_ARec);
        }
        v15 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_porte_po_init_porte_po_mode_indy);
        if (v15 != 1000)
            return v15;
    }
    if (v16->SF_qui_me_commande <= 0)
        v16->i_bit = 0;
    else
        v16->i_bit = AI_EvalFunc_ARRBitTest_C(
            (int*)(porte_po_init_univVars_Flag_speciaux_sauve + 4),
            v16->SF_qui_me_commande);
    if (v16->i_bit)
    {
        v16->i_instantane = 1;
        v11 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_porte_po_init_porte_po_ouvre_toi);
        if (v11 != 1000)
            return v11;
    }
    else if (v16->play_close_on_init)
    {
        v16->i_instantane = 1;
        v13 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_porte_po_init_porte_po_ferme_toi);
        if (v13 != 1000)
            return v13;
    }
    else
    {
        if (!v16->ne_touche_pas_aux_colmaps)
            AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0xFFu, 0);
        v5 = (tGameObjectMain*)v16->TAG_WP_extremite_1;
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
        if (v4)
        {
            AI_EvalFunc_GRIDCurrentSet_C(v16->TAG_etage);
            v3 = (tGameObjectMain*)v16->TAG_WP_extremite_1;
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
            v2 = (tGameObjectMain*)v16->TAG_WP_extremite_2;
            if (v2)
            {
                if ((unsigned int)v2 <= 2)
                    v2 = AI_MainActor[(uint32_t)v2 - 1];
                AI_EvalFunc_OBJGetPos_C(v2, &v12);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v12);
            }
            v14 = AI_EvalFunc_GRIDTagBox_C(AI_gpst_CurrentGameObject, &a3, &v12, 64);
        }
        v18 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_porte_po_init_porte_po_etat_ferme);
        if (v18 != 1000)
            return v18;
    }
    return 1000;
}

int porte_po_reflex()
{
    tScriptStruct_Porte* v1; // [esp+0h] [ebp-48h]
    tMessage* Src; // [esp+4h] [ebp-44h]
    tMessage v3; // [esp+8h] [ebp-40h] BYREF
    unsigned int v4; // [esp+3Ch] [ebp-Ch]
    int v5; // [esp+40h] [ebp-8h]
    tScriptStruct_Porte* v6; // [esp+44h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Porte*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v6 = v1;
    v4 = 0;
    Src = AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v3, Src, sizeof(v3));
    if (v3.msgID == 37)
    {
        v4 = v3.Int1 & 7;
        if (v4 >= 6)
            v5 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(5, -1.0);
    }
    return 1000;
}

int porte_po_ferme_toi()
{
    int v0; // eax
    float v2; // [esp+0h] [ebp-D4h]
    float v3; // [esp+0h] [ebp-D4h]
    tAINode* v4; // [esp+4h] [ebp-D0h]
    tScriptStruct_Porte* v5; // [esp+8h] [ebp-CCh]
    tGameObjectMain* v6; // [esp+10h] [ebp-C4h]
    tGameObjectMain* v7; // [esp+18h] [ebp-BCh]
    tGameObjectMain* v8; // [esp+24h] [ebp-B0h]
    tGameObjectMain* v9; // [esp+2Ch] [ebp-A8h]
    tGameObjectMain* v10; // [esp+30h] [ebp-A4h]
    tGameObjectMain* v11; // [esp+34h] [ebp-A0h]
    tGameObjectMain* v12; // [esp+3Ch] [ebp-98h]
    tGameObjectMain* v13; // [esp+40h] [ebp-94h]
    tGameObjectMain* v14; // [esp+44h] [ebp-90h]
    tGameObjectMain* v15; // [esp+4Ch] [ebp-88h]
    tGameObjectMain* v16; // [esp+50h] [ebp-84h]
    tGameObjectMain* v17; // [esp+54h] [ebp-80h]
    tGameObjectMain* v18; // [esp+5Ch] [ebp-78h]
    tGameObjectMain* v19; // [esp+60h] [ebp-74h]
    tGameObjectMain* v20; // [esp+64h] [ebp-70h]
    tGameObjectMain* v21; // [esp+6Ch] [ebp-68h]
    tGameObjectMain* v22; // [esp+70h] [ebp-64h]
    tGameObjectMain* v23; // [esp+74h] [ebp-60h]
    tGameObjectMain* v24; // [esp+7Ch] [ebp-58h]
    tGameObjectMain* v25; // [esp+80h] [ebp-54h]
    tGameObjectMain* v26; // [esp+84h] [ebp-50h]
    float v27; // [esp+88h] [ebp-4Ch]
    Vec3 v28; // [esp+98h] [ebp-3Ch] BYREF
    int v29; // [esp+A4h] [ebp-30h]
    int v31; // [esp+ACh] [ebp-28h]
    float v32; // [esp+B0h] [ebp-24h]
    tScriptStruct_Porte* v34; // [esp+BCh] [ebp-18h]
    int i; // [esp+C0h] [ebp-14h]
    Vec3 a3; // [esp+C4h] [ebp-10h] BYREF
    int v37; // [esp+D0h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_Porte*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v34 = v5;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;

        fct_porte_po_ferme_toi_porte_po_paf = (int)AI_ul_RealResolveOneRefFunction(0x7100BF46u);
        porte_po_ferme_toi_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &porte_po_ferme_toi_GST_Global_gao_i_map_just_reset,
            (void*)porte_po_ferme_toi_GST_Global_gao,
            3300);
        porte_po_ferme_toi_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&porte_po_ferme_toi_univVars_Flag_speciaux_sauve, (void*)porte_po_ferme_toi_univVars, 4488);
        fct_porte_po_ferme_toi_porte_po_etat_ferme = (int)AI_ul_RealResolveOneRefFunction(0x7100BF02u);
    }
    v4 = *AI_gppst_CurrentJumpNode;
    if (*AI_gppst_CurrentJumpNode == (tAINode*)12)
    {
        for (i = v34->i_nbr_track_ouverture; i < 2 * v34->i_nbr_track_ouverture; ++i)
            AI_EvalFunc_TRASetEnd_C(AI_gpst_CurrentGameObject, i);
    }
    else if (v4 != (tAINode*)13)
    {
        if (v4 == (tAINode*)26)
        {
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_porte_po_ferme_toi_porte_po_etat_ferme);
            return 1000;
        }
        v26 = (tGameObjectMain*)v34->o_desactive_a_la_fermeture0;
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
            v24 = (tGameObjectMain*)v34->o_desactive_a_la_fermeture0;
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
        v23 = (tGameObjectMain*)v34->o_desactive_a_la_fermeture1;
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
            v21 = (tGameObjectMain*)v34->o_desactive_a_la_fermeture1;
            if (v21)
            {
                if ((unsigned int)v21 <= 2)
                    v21 = AI_MainActor[(uint32_t)v21 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v21, 1);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
            }
        }
        v20 = (tGameObjectMain*)v34->o_active_a_la_fermeture0;
        if (v20)
        {
            if ((unsigned int)v20 <= 2)
                v20 = AI_MainActor[(uint32_t)v20 - 1];
            v19 = v20;
        }
        else
        {
            v19 = 0;
        }
        if (v19)
        {
            v18 = (tGameObjectMain*)v34->o_active_a_la_fermeture0;
            if (v18)
            {
                if ((unsigned int)v18 <= 2)
                    v18 = AI_MainActor[(uint32_t)v18 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v18, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
            }
        }
        v17 = (tGameObjectMain*)v34->o_active_a_la_fermeture1;
        if (v17)
        {
            if ((unsigned int)v17 <= 2)
                v17 = AI_MainActor[(uint32_t)v17 - 1];
            v16 = v17;
        }
        else
        {
            v16 = 0;
        }
        if (v16)
        {
            v15 = (tGameObjectMain*)v34->o_active_a_la_fermeture1;
            if (v15)
            {
                if ((unsigned int)v15 <= 2)
                    v15 = AI_MainActor[(uint32_t)v15 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v15, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
            }
        }
        v14 = (tGameObjectMain*)v34->o_evt_fermeture;
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
            v12 = (tGameObjectMain*)v34->o_evt_fermeture;
            if (v12)
            {
                if ((unsigned int)v12 <= 2)
                    v12 = AI_MainActor[(uint32_t)v12 - 1];
                AI_EvalFunc_TRAPlayAll_C(v12);
            }
            else
            {
                AI_EvalFunc_TRAPlayAll_C(0);
            }
        }
        v29 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_porte_po_ferme_toi_porte_po_paf);
        if (v29 != 1000)
            return v29;
        if (!v34->ne_touche_pas_aux_colmaps)
            AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0xFFu, 0);
        if (!AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 0x2000000))
            goto LABEL_112;
        for (i = v34->i_nbr_track_ouverture; i < 2 * v34->i_nbr_track_ouverture; ++i)
            AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, i);
        if (v34->i_instantane)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)12;
            TIME_Wait_C(0.1f);
            return 0;
        }
        if (!*(uint32_t*)porte_po_ferme_toi_GST_Global_gao_i_map_just_reset)
        {
            if (v34->snd_le_son_douverture_boucle)
            {
                v34->i_snd_instance = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 1, 0x18000000);
                AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v34->i_snd_instance);
            }
            else
            {
                v34->i_snd_instance = AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
            }
            v27 = SND_f_GetVol(v34->i_snd_instance);
            v2 = v34->f_coef_volume * v27;
            SND_SetVol(v34->i_snd_instance, v2);
        }
    }
    if (!AI_EvalFunc_TRAIsStopAll_C(AI_gpst_CurrentGameObject))
    {
        if (v34->paf_pendant_fermeture)
        {
            v37 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_porte_po_ferme_toi_porte_po_paf);
            if (v37 != 1000)
                return v37;
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)13;
        return 0;
    }
    if (!*(uint32_t*)porte_po_ferme_toi_GST_Global_gao_i_map_just_reset)
    {
        if (v34->snd_le_son_douverture_boucle)
            SND_Release(v34->i_snd_instance);
        v34->i_snd_instance = AI_EvalFunc_SNDRequestPlay_C_CURRENT(3, -1.0);
        v32 = SND_f_GetVol(v34->i_snd_instance);
        v3 = v34->f_coef_volume * v32;
        SND_SetVol(v34->i_snd_instance, v3);
    }
    v11 = (tGameObjectMain*)v34->TAG_WP_extremite_1;
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
    if (v10)
    {
        AI_EvalFunc_GRIDCurrentSet_C(v34->TAG_etage);
        v9 = (tGameObjectMain*)v34->TAG_WP_extremite_1;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            AI_EvalFunc_OBJGetPos_C(v9, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        v8 = (tGameObjectMain*)v34->TAG_WP_extremite_2;
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v8 = AI_MainActor[(uint32_t)v8 - 1];
            AI_EvalFunc_OBJGetPos_C(v8, &v28);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v28);
        }
        v31 = AI_EvalFunc_GRIDTagBox_C(AI_gpst_CurrentGameObject, &a3, &v28, 64);
    }
    v34->i_instantane = 0;
LABEL_112:
    if (v34->SF_qui_me_commande > 0)
        AI_EvalFunc_ARRBitClear_C((int*)(porte_po_ferme_toi_univVars_Flag_speciaux_sauve + 4), v34->SF_qui_me_commande);
    v7 = (tGameObjectMain*)v34->Capa_qui_me_commande1;
    if (v7)
    {
        if ((unsigned int)v7 <= 2)
            v7 = AI_MainActor[(uint32_t)v7 - 1];
        v0 = AI_EvalFunc_OBJ_CustomBitsGet_C(v7);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_CustomBitsGet_C(0);
    }
    if (v0 == 21760)
    {
        v6 = (tGameObjectMain*)v34->Capa_qui_me_commande1;
        if (v6)
        {
            if ((unsigned int)v6 <= 2)
                v6 = AI_MainActor[(uint32_t)v6 - 1];
            AI_EvalFunc_OBJ_CapaSet_C(v6, 1, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_CapaSet_C(0, 1, 0);
        }
    }
    *AI_gppst_CurrentJumpNode = (tAINode*)26;
    TIME_Wait_C(v34->attente_sup_apres_mvt);
    return 0;
}

int porte_po_mode_indy_ARec()
{
    BOOL v0; // eax
    float v2; // [esp+0h] [ebp-1A0h]
    tScriptStruct_Porte* v3; // [esp+4h] [ebp-19Ch]
    float v4; // [esp+24h] [ebp-17Ch]
    float v5; // [esp+28h] [ebp-178h]
    Vec3* v6; // [esp+40h] [ebp-160h]
    float v7; // [esp+44h] [ebp-15Ch]
    tGameObjectMain* v8; // [esp+5Ch] [ebp-144h]
    Vec3 v9; // [esp+60h] [ebp-140h] BYREF
    Vec3 v10; // [esp+6Ch] [ebp-134h] BYREF
    Vec3 v11; // [esp+78h] [ebp-128h] BYREF
    Vec3 v12; // [esp+84h] [ebp-11Ch] BYREF
    Vec3 v13; // [esp+90h] [ebp-110h] BYREF
    int v14; // [esp+9Ch] [ebp-104h]
    float v15; // [esp+A0h] [ebp-100h]
    Vec3 v16; // [esp+A4h] [ebp-FCh] BYREF
    float v17; // [esp+B0h] [ebp-F0h]
    BOOL v18; // [esp+B4h] [ebp-ECh]
    int v19; // [esp+B8h] [ebp-E8h]
    Vec3 a1; // [esp+BCh] [ebp-E4h] BYREF
    Vec3 v21; // [esp+C8h] [ebp-D8h] BYREF
    Vec3 v22; // [esp+D4h] [ebp-CCh] BYREF
    Vec3 v23; // [esp+E0h] [ebp-C0h] BYREF
    Vec3 v24; // [esp+ECh] [ebp-B4h] BYREF
    Vec3 v25; // [esp+104h] [ebp-9Ch] BYREF
    Vec3 dest; // [esp+110h] [ebp-90h] BYREF
    Vec3 v27; // [esp+11Ch] [ebp-84h] BYREF
    Vec3 v28; // [esp+128h] [ebp-78h] BYREF
    BOOL v29; // [esp+134h] [ebp-6Ch]
    int v30; // [esp+138h] [ebp-68h]
    Vec3 v31; // [esp+13Ch] [ebp-64h] BYREF
    Vec3 a3[2]; // [esp+148h] [ebp-58h] BYREF
    Vec3 v33; // [esp+168h] [ebp-38h] BYREF
    tScriptStruct_Porte* v34; // [esp+174h] [ebp-2Ch]
    Vec3 a2; // [esp+178h] [ebp-28h] BYREF
    int v36; // [esp+184h] [ebp-1Ch]
    float v37; // [esp+188h] [ebp-18h]
    Vec3 v38; // [esp+18Ch] [ebp-14h] BYREF
    float v39; // [esp+198h] [ebp-8h]
    float v40; // [esp+19Ch] [ebp-4h]

    v28.x = 1.0;
    v28.y = 0.0;
    v28.z = 0.0;
    v21.x = 1.0;
    v21.y = 0.0;
    v21.z = 0.0;
    v13.x = 1.0;
    v13.y = 0.0;
    v13.z = 0.0;
    v25.x = 1.0;
    v25.y = 0.0;
    v25.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_Porte*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v34 = v3;
    v17 = 0.0;
    v40 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        porte_po_mode_indy_ARec_PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(
            &porte_po_mode_indy_ARec_PJ_Sally_gao_i_stealth_accroupi,
            (void*)porte_po_mode_indy_ARec_PJ_Sally_gao,
            4224);
    }
    v8 = (tGameObjectMain*)porte_po_mode_indy_ARec_PJ_Sally_gao;
    if (porte_po_mode_indy_ARec_PJ_Sally_gao)
    {
        if ((unsigned int)porte_po_mode_indy_ARec_PJ_Sally_gao <= 2)
            v8 = AI_MainActor[porte_po_mode_indy_ARec_PJ_Sally_gao - 1];
        v0 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, v8);
    }
    else
    {
        v0 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, 0);
    }
    v18 = v0;
    if (v0)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v33);
        v13 = v33;
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)porte_po_mode_indy_ARec_PJ_Sally_gao, a3);
        v21 = a3[0];
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v11);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v10);
        MATH_SubstractVec(&dest, &v13, &v21);
        v37 = dest.z * v10.z + dest.y * v10.y + v10.x * dest.x;
        v15 = AI_EvalFunc_MTHFloatSign_C(v37);
        v7 = -v15;
        v6 = &v34->v_recal_axis;
        v34->v_recal_axis.x = v11.x * v7;
        v6->y = v11.y * v7;
        v6->z = v11.z * v7;
        v17 = 1.5;
        if (*(uint32_t*)porte_po_mode_indy_ARec_PJ_Sally_gao_i_stealth_accroupi)
        {
            v17 = 0.89999998f;
        }
        else
        {
            v36 = AI_EvalFunc_ACTActionGet_C((tGameObjectMain*)porte_po_mode_indy_ARec_PJ_Sally_gao);
            if (v36 == 158)
            {
                v19 = AI_EvalFunc_ACTActionItemGet_C((tGameObjectMain*)porte_po_mode_indy_ARec_PJ_Sally_gao);
                if (v19
                    || (v30 = AI_EvalFunc_ACTActionItemGet_C((tGameObjectMain*)porte_po_mode_indy_ARec_PJ_Sally_gao)) == 0
                    && (v14 = AI_EvalFunc_ANICurrentFrameGet_C((tGameObjectMain*)porte_po_mode_indy_ARec_PJ_Sally_gao, 0),
                        v14 > 24))
                {
                    v17 = 0.89999998f;
                }
            }
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v38);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v27);
        v16.x = v27.x * v17;
        v16.y = v27.y * v17;
        v16.z = v27.z * v17;
        MATH_SubstractVec(&v23, &v38, &v16);
        v25 = v23;
        MATH_SubstractVec(&v9, &v25, &v21);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v24);
        v40 = v24.z * v9.z + v24.y * v9.y + v9.x * v24.x;
        if (v40 >= 0.0)
        {
            v34->v_memo_indy_sens.x = 0.0;
            v34->v_memo_indy_sens.y = 0.0;
            v34->v_memo_indy_sens.z = 0.0;
        }
        else
        {
            if (fabs(v34->v_memo_indy_sens.x) != 0.0)
            {
                v29 = 0;
            }
            else if (fabs(v34->v_memo_indy_sens.y) != 0.0)
            {
                v29 = 0;
            }
            else
            {
                v29 = fabs(v34->v_memo_indy_sens.z) == 0.0;
            }
            if (v29)
                v34->v_memo_indy_sens = v34->v_recal_axis;
            v5 = -v40;
            if (v5 > 0.1)
                v2 = 0.1f;
            else
                v2 = -v40;
            v39 = v2;
            v4 = v2 / 0.1f * v34->f_epaisseur_porte;
            v31.x = v4 * v34->v_memo_indy_sens.x;
            v31.y = v34->v_memo_indy_sens.y * v4;
            v31.z = v34->v_memo_indy_sens.z * v4;
            MATH_VecAddVec(&a1, &v13, &v31);
            v25 = a1;
            v28.x = v21.x;
            v28.y = v21.y;
            v28.z = a1.z;
            MATH_SubstractVec(&v22, &v28, &v25);
            v40 = v34->v_memo_indy_sens.z * v22.z + v34->v_memo_indy_sens.y * v22.y + v22.x * v34->v_memo_indy_sens.x;
            if (v40 < 0.0)
            {
                v12.x = v40 * v34->v_memo_indy_sens.x;
                v12.y = v34->v_memo_indy_sens.y * v40;
                v12.z = v34->v_memo_indy_sens.z * v40;
                MATH_SubstractVec(&a2, &v21, &v12);
                AI_EvalFunc_OBJSetPos_C((tGameObjectMain*)porte_po_mode_indy_ARec_PJ_Sally_gao, &a2);
            }
        }
    }
    return 1000;
}

int porte_po_paf()
{
    tScriptStruct_Porte* v1; // [esp+0h] [ebp-114h]
    tGameObjectMain* v2; // [esp+8h] [ebp-10Ch]
    tGameObjectMain* v3; // [esp+28h] [ebp-ECh]
    Vec3 v4; // [esp+34h] [ebp-E0h] BYREF
    Vec3 a3; // [esp+40h] [ebp-D4h] BYREF
    Vec3 v6; // [esp+4Ch] [ebp-C8h]
    tMessage Src; // [esp+58h] [ebp-BCh] BYREF
    Vec3 v8; // [esp+8Ch] [ebp-88h] BYREF
    tGameObjectMain* a7[10]; // [esp+A8h] [ebp-6Ch] BYREF
    Vec3 dest; // [esp+D0h] [ebp-44h] BYREF
    tGameObjectMain* v11; // [esp+DCh] [ebp-38h]
    int v12; // [esp+E0h] [ebp-34h]
    Vec3 v13; // [esp+E4h] [ebp-30h] BYREF
    Vec3 v14; // [esp+F0h] [ebp-24h]
    Vec3 v15; // [esp+FCh] [ebp-18h] BYREF
    tScriptStruct_Porte* v16; // [esp+108h] [ebp-Ch]
    float v17; // [esp+10Ch] [ebp-8h]

    v6.x = 1.0;
    v6.y = 0.0;
    v6.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Porte*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v16 = v1;
    v11 = 0;
    v12 = 0;
    v12 = AI_EvalFunc_COLZDEPivotListGet_C(AI_gpst_CurrentGameObject, a7, 6, 2, 0, 0);
    if (v12)
    {
        --v12;
        while (v12 >= 0)
        {
            v11 = a7[v12];
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v15);
            v6 = v15;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v13);
            v3 = v11;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v3 = AI_MainActor[(uint32_t)v11 - 1];
                AI_EvalFunc_OBJGetPos_C(v3, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v4);
            MATH_SubstractVec(&dest, &a3, &v4);
            v17 = dest.z * v13.z + dest.y * v13.y + v13.x * dest.x;
            if (v17 < 0.0)
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v8);
                v14.x = -v8.x;
                v14.y = -v8.y;
                v14.z = -v8.z;
                v6.x = -v8.x;
                v6.y = -v8.y;
                v6.z = -v8.z;
            }
            Src.msgID = 37;
            Src.gao = AI_gpst_CurrentGameObject;
            Src.gao2 = 0;
            Src.Int1 = 3;
            Src.Int2 = 0;
            Src.v1 = v6;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&Src.v2);
            Src.f2 = 0.0;
            Src.f1 = 1000.0;
            v2 = v11;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v2 = AI_MainActor[(uint32_t)v11 - 1];
                AI_EvalFunc_MSGSend_C(v2, &Src);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &Src);
            }
            --v12;
        }
    }
    return 1000;
}

int porte_po_etat_ouverte()
{
    __int16 v0; // ax
    int v2; // eax
    char* v3; // eax
    char* v4; // eax
    float v5; // [esp+0h] [ebp-90h]
    float v6; // [esp+4h] [ebp-8Ch]
    float v7; // [esp+8h] [ebp-88h]
    float v8; // [esp+Ch] [ebp-84h]
    tScriptStruct_Porte* v9; // [esp+10h] [ebp-80h]
    tGameObjectMain* v10; // [esp+14h] [ebp-7Ch]
    tGameObjectMain* v11; // [esp+18h] [ebp-78h]
    tGameObjectMain* v12; // [esp+1Ch] [ebp-74h]
    tGameObjectMain* v13; // [esp+20h] [ebp-70h]
    tGameObjectMain* v14; // [esp+28h] [ebp-68h]
    tGameObjectMain* v15; // [esp+30h] [ebp-60h]
    tGameObjectMain* v16; // [esp+38h] [ebp-58h]
    tGameObjectMain* v17; // [esp+40h] [ebp-50h]
    int v18; // [esp+48h] [ebp-48h]
    int v19; // [esp+4Ch] [ebp-44h]
    int v20; // [esp+54h] [ebp-3Ch]
    Vec3 a3; // [esp+74h] [ebp-1Ch] BYREF
    tScriptStruct_Porte* v22; // [esp+80h] [ebp-10h]
    int i; // [esp+84h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v9 = (tScriptStruct_Porte*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v9 = 0;
    v22 = v9;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_porte_po_etat_ouverte_porte_po_check_separation = (int)AI_ul_RealResolveOneRefFunction(0x7100C57Fu);
        porte_po_etat_ouverte_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &porte_po_etat_ouverte_GST_Global_gao_f_event_sound_time,
            (void*)porte_po_etat_ouverte_GST_Global_gao,
            704);
        AI2C_ResolveBuffer(
            &porte_po_etat_ouverte_GST_Global_gao_i_event_sound_nbr,
            (void*)porte_po_etat_ouverte_GST_Global_gao,
            872);
        AI2C_ResolveBuffer(
            &porte_po_etat_ouverte_GST_Global_gao_v_event_sound_pos,
            (void*)porte_po_etat_ouverte_GST_Global_gao,
            376);
        AI2C_ResolveBuffer(
            &porte_po_etat_ouverte_GST_Global_gao_f_event_sound_size,
            (void*)porte_po_etat_ouverte_GST_Global_gao,
            620);
        AI2C_ResolveBuffer(
            &porte_po_etat_ouverte_GST_Global_gao_o_event_sound_actor,
            (void*)porte_po_etat_ouverte_GST_Global_gao,
            788);
        AI2C_ResolveBuffer(
            &porte_po_etat_ouverte_GST_Global_gao_i_event_sound_id,
            (void*)porte_po_etat_ouverte_GST_Global_gao,
            876);
        AI2C_ResolveBuffer(
            &porte_po_etat_ouverte_GST_Global_gao_i_event_sound_next_id,
            (void*)porte_po_etat_ouverte_GST_Global_gao,
            960);
        fct_porte_po_etat_ouverte_porte_po_ferme_toi = (int)AI_ul_RealResolveOneRefFunction(0x7100BF43u);
    }
    if (!AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 0x2000000))
    {
        v17 = (tGameObjectMain*)v22->pos_haute;
        if (v17)
        {
            if ((unsigned int)v17 <= 2)
                v17 = AI_MainActor[(uint32_t)v17 - 1];
            AI_EvalFunc_OBJGetPos_C(v17, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        AI_EvalFunc_OBJSetPos_C_CURRENT(&a3);
    }
    v16 = (tGameObjectMain*)v22->Capa_qui_me_commande1;
    if (v16)
    {
        if ((unsigned int)v16 <= 2)
            v16 = AI_MainActor[(uint32_t)v16 - 1];
        v0 = AI_EvalFunc_OBJ_CapaGet_C(v16);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_CapaGet_C(0);
    }
    v22->i_bit = v0 & 0x100;
    if (v22->i_bit == 256)
        v22->i_bit = 1;
    if (!v22->etat_de_declanchement)
        v22->i_bit = 1 - v22->i_bit;
    if (v22->i_bit && !v22->type_de_fonctionnement || !v22->i_bit && v22->type_de_fonctionnement)
    {
        if (v22->interdire_separation_equipe)
        {
            v18 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_porte_po_etat_ouverte_porte_po_check_separation);
            if (v18 != 1000)
                return v18;
            if (v22->i_separation)
            {
                v15 = (tGameObjectMain*)v22->Capa_qui_me_commande1;
                if (v15)
                {
                    if ((unsigned int)v15 <= 2)
                        v15 = AI_MainActor[(uint32_t)v15 - 1];
                    v2 = AI_EvalFunc_OBJ_CustomBitsGet_C(v15);
                }
                else
                {
                    v2 = AI_EvalFunc_OBJ_CustomBitsGet_C(0);
                }
                if (v2 == 21760)
                {
                    v14 = (tGameObjectMain*)v22->Capa_qui_me_commande1;
                    if (v14)
                    {
                        if ((unsigned int)v14 <= 2)
                            v14 = AI_MainActor[(uint32_t)v14 - 1];
                        AI_EvalFunc_OBJ_CapaSet_C(v14, 258, 0);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_CapaSet_C(0, 258, 0);
                    }
                }
            }
            else
            {
                if (v22->des_sound_event == 1 || v22->des_sound_event == 3)
                {
                    for (i = 0;
                        i < 20
                        && *(float*)AI_EvalVarArray_C2((char*)porte_po_etat_ouverte_GST_Global_gao_f_event_sound_time, i) != 0.0;
                        ++i)
                    {
                        ;
                    }
                    if (i != 20)
                    {
                        ++* (uint32_t*)porte_po_etat_ouverte_GST_Global_gao_i_event_sound_nbr;
                        v13 = (tGameObjectMain*)v22->des_sound_event_close_pos;
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
                        v3 = AI_EvalVarArray_C3((char*)porte_po_etat_ouverte_GST_Global_gao_v_event_sound_pos, i);
                        AI_EvalFunc_OBJGetPos_C(v12, (Vec3*)v3);
                        v8 = v22->des_sound_event_portee;
                        *(float*)AI_EvalVarArray_C2((char*)porte_po_etat_ouverte_GST_Global_gao_f_event_sound_size, i) = v8;
                        v7 = v22->des_sound_event_duree;
                        *(float*)AI_EvalVarArray_C2((char*)porte_po_etat_ouverte_GST_Global_gao_f_event_sound_time, i) = v7;
                        *(uint32_t*)(porte_po_etat_ouverte_GST_Global_gao_o_event_sound_actor + 4 * i + 4) = (uint32_t)AI_gpst_CurrentGameObject;
                        *(uint32_t*)(porte_po_etat_ouverte_GST_Global_gao_i_event_sound_id + 4 * i + 4) = (*(uint32_t*)porte_po_etat_ouverte_GST_Global_gao_i_event_sound_next_id)++;
                    }
                }
                v19 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_porte_po_etat_ouverte_porte_po_ferme_toi);
                if (v19 != 1000)
                    return v19;
            }
        }
        else
        {
            if (v22->des_sound_event == 1 || v22->des_sound_event == 3)
            {
                for (i = 0;
                    i < 20
                    && *(float*)AI_EvalVarArray_C2((char*)porte_po_etat_ouverte_GST_Global_gao_f_event_sound_time, i) != 0.0;
                    ++i)
                {
                    ;
                }
                if (i != 20)
                {
                    ++* (uint32_t*)porte_po_etat_ouverte_GST_Global_gao_i_event_sound_nbr;
                    v11 = (tGameObjectMain*)v22->des_sound_event_close_pos;
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
                    v4 = AI_EvalVarArray_C3((char*)porte_po_etat_ouverte_GST_Global_gao_v_event_sound_pos, i);
                    AI_EvalFunc_OBJGetPos_C(v10, (Vec3*)v4);
                    v6 = v22->des_sound_event_portee;
                    *(float*)AI_EvalVarArray_C2((char*)porte_po_etat_ouverte_GST_Global_gao_f_event_sound_size, i) = v6;
                    v5 = v22->des_sound_event_duree;
                    *(float*)AI_EvalVarArray_C2((char*)porte_po_etat_ouverte_GST_Global_gao_f_event_sound_time, i) = v5;
                    *(uint32_t*)(porte_po_etat_ouverte_GST_Global_gao_o_event_sound_actor + 4 * i + 4) = (uint32_t)AI_gpst_CurrentGameObject;
                    *(uint32_t*)(porte_po_etat_ouverte_GST_Global_gao_i_event_sound_id + 4 * i + 4) = (*(uint32_t*)porte_po_etat_ouverte_GST_Global_gao_i_event_sound_next_id)++;
                }
            }
            v20 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_porte_po_etat_ouverte_porte_po_ferme_toi);
            if (v20 != 1000)
                return v20;
        }
    }
    return 1000;
}

int porte_po_check_separation()
{
    int v0; // eax
    int v1; // eax
    int v2; // eax
    tScriptStruct_Porte* v4; // [esp+8h] [ebp-E0h]
    tGameObjectMain* v5; // [esp+10h] [ebp-D8h]
    tAI* v6; // [esp+14h] [ebp-D4h]
    tGameObjectMain* v7; // [esp+18h] [ebp-D0h]
    tGameObjectMain* v8; // [esp+1Ch] [ebp-CCh]
    tGameObjectMain* v9; // [esp+24h] [ebp-C4h]
    tGameObjectMain* v10; // [esp+28h] [ebp-C0h]
    tGameObjectMain* v11; // [esp+2Ch] [ebp-BCh]
    tGameObjectMain* v12; // [esp+34h] [ebp-B4h]
    tGameObjectMain* v13; // [esp+38h] [ebp-B0h]
    tGameObjectMain* v14; // [esp+3Ch] [ebp-ACh]
    tGameObjectMain* v15; // [esp+44h] [ebp-A4h]
    tGameObjectMain* a2; // [esp+48h] [ebp-A0h]
    tGameObjectMain* v17; // [esp+4Ch] [ebp-9Ch]
    tGameObjectMain* v18; // [esp+50h] [ebp-98h]
    tGameObjectMain* v19; // [esp+54h] [ebp-94h]
    tGameObjectMain* v20; // [esp+60h] [ebp-88h]
    tGameObjectMain* v21; // [esp+64h] [ebp-84h]
    tMessage Src; // [esp+68h] [ebp-80h] BYREF
    int v23; // [esp+9Ch] [ebp-4Ch]
    int v24; // [esp+A0h] [ebp-48h]
    unsigned int v25; // [esp+A4h] [ebp-44h]
    tAI* v27; // [esp+B8h] [ebp-30h]
    unsigned int v28; // [esp+BCh] [ebp-2Ch]
    int i; // [esp+C4h] [ebp-24h]
    int v30; // [esp+CCh] [ebp-1Ch]
    tScriptStruct_Porte* v31; // [esp+D4h] [ebp-14h]
    int v32; // [esp+D8h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_Porte*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v31 = v4;
    v25 = 0;
    i = 0;
    v27 = 0;
    v32 = 0;
    v28 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        porte_po_check_separation__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        porte_po_check_separation_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &porte_po_check_separation_univVars_Perso_ID_status,
            (void*)porte_po_check_separation_univVars,
            484);
        AI2C_ResolveBuffer(
            &porte_po_check_separation_univVars_Perso_ID_actor,
            (void*)porte_po_check_separation_univVars,
            988);
    }
    v27 = 0;
    if (AI_EvalFunc_TIMEElapsed_C(v31->f_last_separation_time, 1.0))
    {
        v28 = porte_po_check_separation__PJ_Sally_gao;
        for (i = 0; i < 5 && *(uint32_t*)(porte_po_check_separation_univVars_Perso_ID_status + 4 * i + 4); ++i)
            ;
        v31->i_separation = i != 5;
        if (v31->i_separation)
        {
            v21 = (tGameObjectMain*)v31->o_BV_test_separation;
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
                v31->o_BV_test_separation = (int)AI_gpst_CurrentGameObject;
            v31->i_separation = 0;
            for (i = 0; i < 5; ++i)
            {
                v27 = *(tAI**)(porte_po_check_separation_univVars_Perso_ID_actor + 4 * i + 4);
                v19 = (tGameObjectMain*)v27;
                if (v27)
                {
                    if ((unsigned int)v27 <= 2)
                        v19 = AI_MainActor[(uint32_t)v27 - 1];
                    v18 = v19;
                }
                else
                {
                    v18 = 0;
                }
                if (v18 && !*(uint32_t*)(porte_po_check_separation_univVars_Perso_ID_status + 4 * i + 4))
                {
                    v17 = (tGameObjectMain*)v28;
                    if (v28)
                    {
                        if (v28 <= 2)
                            v17 = AI_MainActor[v28 - 1];
                        a2 = v17;
                    }
                    else
                    {
                        a2 = 0;
                    }
                    v15 = (tGameObjectMain*)v31->o_BV_test_separation;
                    if (v15)
                    {
                        if ((unsigned int)v15 <= 2)
                            v15 = AI_MainActor[(uint32_t)v15 - 1];
                        v0 = AI_EvalFunc_COLBVPivotCollide_C(v15, a2);
                    }
                    else
                    {
                        v0 = AI_EvalFunc_COLBVPivotCollide_C(0, a2);
                    }
                    v30 = v0;
                    if (v0)
                    {
                        v14 = (tGameObjectMain*)v27;
                        if (v27)
                        {
                            if ((unsigned int)v27 <= 2)
                                v14 = AI_MainActor[(uint32_t)v27 - 1];
                            v13 = v14;
                        }
                        else
                        {
                            v13 = 0;
                        }
                        v12 = (tGameObjectMain*)v31->o_BV_test_separation;
                        if (v12)
                        {
                            if ((unsigned int)v12 <= 2)
                                v12 = AI_MainActor[(uint32_t)v12 - 1];
                            v1 = AI_EvalFunc_COLBVPivotCollide_C(v12, v13);
                        }
                        else
                        {
                            v1 = AI_EvalFunc_COLBVPivotCollide_C(0, v13);
                        }
                        v24 = v1;
                        if (!v1)
                        {
                            v31->i_separation = 1;
                            break;
                        }
                    }
                    else
                    {
                        v11 = (tGameObjectMain*)v27;
                        if (v27)
                        {
                            if ((unsigned int)v27 <= 2)
                                v11 = AI_MainActor[(uint32_t)v27 - 1];
                            v10 = v11;
                        }
                        else
                        {
                            v10 = 0;
                        }
                        v9 = (tGameObjectMain*)v31->o_BV_test_separation;
                        if (v9)
                        {
                            if ((unsigned int)v9 <= 2)
                                v9 = AI_MainActor[(uint32_t)v9 - 1];
                            v2 = AI_EvalFunc_COLBVPivotCollide_C(v9, v10);
                        }
                        else
                        {
                            v2 = AI_EvalFunc_COLBVPivotCollide_C(0, v10);
                        }
                        v23 = v2;
                        if (v2)
                        {
                            v31->i_separation = 1;
                            break;
                        }
                    }
                }
            }
        }
        if (v31->i_separation)
        {
            v32 = AI_EvalFunc_MATHRandInt_C(0, 2);
            if (v27 == WOR_gst_Universe.univAIInstance)
            {
                v6 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v8 = (tGameObjectMain*)v27;
                if (v27)
                {
                    if ((unsigned int)v27 <= 2)
                        v8 = AI_MainActor[(uint32_t)v27 - 1];
                    v7 = v8;
                }
                else
                {
                    v7 = 0;
                }
                v6 = v7->extended->AIInstance;
            }
            v25 = *((uint32_t*)v6->instanceVarDataCurrent + 7);// TODO FIXME: ez mi?
            Src.msgID = 30;
            Src.Int1 = v32 + 11;
            Src.Int2 = 0;
            Src.f2 = 2.0;
            Src.v1.x = 0.0;
            Src.v1.y = 0.0;
            Src.v1.z = 0.0;
            Src.v2.x = 0.0;
            Src.v2.y = 0.0;
            Src.v2.z = 0.0;
            Src.gao = 0;
            Src.gao2 = 0;
            v5 = (tGameObjectMain*)v25;
            if (v25)
            {
                if (v25 <= 2)
                    v5 = AI_MainActor[v25 - 1];
                AI_EvalFunc_MSGSend_C(v5, &Src);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &Src);
            }
            v31->f_last_separation_time = TIM_gf_MainClock - TIM_gf_LockTime;
        }
    }
    else
    {
        v31->i_separation = 1;
    }
    return 1000;
}

int porte_po_etat_ferme()
{
    __int16 v0; // ax
    char* v1; // eax
    int result; // eax
    float v3; // [esp+0h] [ebp-54h]
    float v4; // [esp+4h] [ebp-50h]
    tScriptStruct_Porte* v5; // [esp+8h] [ebp-4Ch]
    tGameObjectMain* v6; // [esp+Ch] [ebp-48h]
    tGameObjectMain* v7; // [esp+10h] [ebp-44h]
    tGameObjectMain* v8; // [esp+18h] [ebp-3Ch]
    tGameObjectMain* v9; // [esp+20h] [ebp-34h]
    int v10; // [esp+38h] [ebp-1Ch]
    Vec3 a3; // [esp+40h] [ebp-14h] BYREF
    tScriptStruct_Porte* v12; // [esp+4Ch] [ebp-8h]
    int i; // [esp+50h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_Porte*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v12 = v5;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        porte_po_etat_ferme_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &porte_po_etat_ferme_GST_Global_gao_f_event_sound_time,
            (void*)porte_po_etat_ferme_GST_Global_gao,
            704);
        AI2C_ResolveBuffer(
            &porte_po_etat_ferme_GST_Global_gao_i_event_sound_nbr,
            (void*)porte_po_etat_ferme_GST_Global_gao,
            872);
        AI2C_ResolveBuffer(
            &porte_po_etat_ferme_GST_Global_gao_v_event_sound_pos,
            (void*)porte_po_etat_ferme_GST_Global_gao,
            376);
        AI2C_ResolveBuffer(
            &porte_po_etat_ferme_GST_Global_gao_f_event_sound_size,
            (void*)porte_po_etat_ferme_GST_Global_gao,
            620);
        AI2C_ResolveBuffer(
            &porte_po_etat_ferme_GST_Global_gao_o_event_sound_actor,
            (void*)porte_po_etat_ferme_GST_Global_gao,
            788);
        AI2C_ResolveBuffer(
            &porte_po_etat_ferme_GST_Global_gao_i_event_sound_id,
            (void*)porte_po_etat_ferme_GST_Global_gao,
            876);
        AI2C_ResolveBuffer(
            &porte_po_etat_ferme_GST_Global_gao_i_event_sound_next_id,
            (void*)porte_po_etat_ferme_GST_Global_gao,
            960);
        fct_porte_po_etat_ferme_porte_po_ouvre_toi = (int)AI_ul_RealResolveOneRefFunction(0x7100BF42u);
    }
    if (!AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 0x2000000))
    {
        v9 = (tGameObjectMain*)v12->pos_basse;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            AI_EvalFunc_OBJGetPos_C(v9, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        AI_EvalFunc_OBJSetPos_C_CURRENT(&a3);
    }
    v8 = (tGameObjectMain*)v12->Capa_qui_me_commande1;
    if (v8)
    {
        if ((unsigned int)v8 <= 2)
            v8 = AI_MainActor[(uint32_t)v8 - 1];
        v0 = AI_EvalFunc_OBJ_CapaGet_C(v8);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_CapaGet_C(0);
    }
    v12->i_bit = v0 & 0x100;
    if (v12->i_bit == 256)
        v12->i_bit = 1;
    if (!v12->etat_de_declanchement)
        v12->i_bit = 1 - v12->i_bit;
    if ((!v12->i_bit || v12->type_de_fonctionnement) && (!v12->i_bit || !v12->type_de_fonctionnement))
        goto LABEL_41;
    if (v12->des_sound_event == 1 || v12->des_sound_event == 2)
    {
        for (i = 0;
            i < 20
            && *(float*)AI_EvalVarArray_C2((char*)porte_po_etat_ferme_GST_Global_gao_f_event_sound_time, i) != 0.0;
            ++i)
        {
            ;
        }
        if (i != 20)
        {
            ++* (uint32_t*)porte_po_etat_ferme_GST_Global_gao_i_event_sound_nbr;
            v7 = (tGameObjectMain*)v12->des_sound_event_open_pos;
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                v6 = v7;
            }
            else
            {
                v6 = 0;
            }
            v1 = AI_EvalVarArray_C3((char*)porte_po_etat_ferme_GST_Global_gao_v_event_sound_pos, i);
            AI_EvalFunc_OBJGetPos_C(v6, (Vec3*)v1);
            v4 = v12->des_sound_event_portee;
            *(float*)AI_EvalVarArray_C2((char*)porte_po_etat_ferme_GST_Global_gao_f_event_sound_size, i) = v4;
            v3 = v12->des_sound_event_duree;
            *(float*)AI_EvalVarArray_C2((char*)porte_po_etat_ferme_GST_Global_gao_f_event_sound_time, i) = v3;
            *(uint32_t*)(porte_po_etat_ferme_GST_Global_gao_o_event_sound_actor + 4 * i + 4) = (uint32_t)AI_gpst_CurrentGameObject;
            *(uint32_t*)(porte_po_etat_ferme_GST_Global_gao_i_event_sound_id + 4 * i + 4) = (*(uint32_t*)porte_po_etat_ferme_GST_Global_gao_i_event_sound_next_id)++;
        }
    }
    v10 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_porte_po_etat_ferme_porte_po_ouvre_toi);
    if (v10 != 1000)
        result = v10;
    else
        LABEL_41:
    result = 1000;
    return result;
}

int porte_po_mode_indy()
{
    BOOL v0; // eax
    int v1; // eax
    BOOL v2; // eax
    tScriptStruct_Porte* v4; // [esp+0h] [ebp-1B8h]
    tGameObjectMain* v5; // [esp+8h] [ebp-1B0h]
    tGameObjectMain* v6; // [esp+Ch] [ebp-1ACh]
    tGameObjectMain* v7; // [esp+10h] [ebp-1A8h]
    tGameObjectMain* v8; // [esp+18h] [ebp-1A0h]
    tGameObjectMain* v9; // [esp+1Ch] [ebp-19Ch]
    tGameObjectMain* v10; // [esp+20h] [ebp-198h]
    tGameObjectMain* v11; // [esp+28h] [ebp-190h]
    tGameObjectMain* v12; // [esp+2Ch] [ebp-18Ch]
    tGameObjectMain* v13; // [esp+30h] [ebp-188h]
    tGameObjectMain* v14; // [esp+38h] [ebp-180h]
    tGameObjectMain* v15; // [esp+3Ch] [ebp-17Ch]
    tGameObjectMain* v16; // [esp+40h] [ebp-178h]
    float v17; // [esp+44h] [ebp-174h]
    float v18; // [esp+48h] [ebp-170h]
    tGameObjectMain* v19; // [esp+50h] [ebp-168h]
    float v20; // [esp+54h] [ebp-164h]
    tGameObjectMain* v21; // [esp+5Ch] [ebp-15Ch]
    tGameObjectMain* v22; // [esp+60h] [ebp-158h]
    tGameObjectMain* v23; // [esp+64h] [ebp-154h]
    tGameObjectMain* v24; // [esp+6Ch] [ebp-14Ch]
    tGameObjectMain* v25; // [esp+70h] [ebp-148h]
    tGameObjectMain* v26; // [esp+74h] [ebp-144h]
    tGameObjectMain* v27; // [esp+8Ch] [ebp-12Ch]
    tGameObjectMain* v28; // [esp+94h] [ebp-124h]
    tGameObjectMain* v29; // [esp+9Ch] [ebp-11Ch]
    Vec3 v30; // [esp+A0h] [ebp-118h] BYREF
    float v31; // [esp+ACh] [ebp-10Ch]
    Vec3 dest; // [esp+B0h] [ebp-108h] BYREF
    Vec3 v33; // [esp+BCh] [ebp-FCh] BYREF
    float v34; // [esp+C8h] [ebp-F0h]
    float v35; // [esp+CCh] [ebp-ECh]
    float v36; // [esp+D0h] [ebp-E8h]
    Vec3 v37; // [esp+D4h] [ebp-E4h] BYREF
    Vec3 v38; // [esp+E0h] [ebp-D8h] BYREF
    float v39; // [esp+ECh] [ebp-CCh]
    BOOL v40; // [esp+F0h] [ebp-C8h]
    Vec3 v41; // [esp+F4h] [ebp-C4h] BYREF
    float v42; // [esp+100h] [ebp-B8h]
    float v43; // [esp+104h] [ebp-B4h]
    float v44; // [esp+108h] [ebp-B0h]
    int v45; // [esp+10Ch] [ebp-ACh]
    float v46; // [esp+110h] [ebp-A8h]
    float v47; // [esp+114h] [ebp-A4h]
    float v48; // [esp+118h] [ebp-A0h]
    float v49; // [esp+11Ch] [ebp-9Ch]
    float v50; // [esp+120h] [ebp-98h]
    float v51; // [esp+124h] [ebp-94h]
    Vec3 v52; // [esp+128h] [ebp-90h] BYREF
    Vec3 v53; // [esp+134h] [ebp-84h] BYREF
    float v54; // [esp+140h] [ebp-78h]
    float v55; // [esp+144h] [ebp-74h]
    float v56; // [esp+148h] [ebp-70h]
    Vec3 v57; // [esp+14Ch] [ebp-6Ch]
    float v58; // [esp+158h] [ebp-60h]
    float v59; // [esp+15Ch] [ebp-5Ch]
    float v60; // [esp+160h] [ebp-58h]
    Vec3 v61; // [esp+168h] [ebp-50h] BYREF
    float v62; // [esp+178h] [ebp-40h]
    float v63; // [esp+17Ch] [ebp-3Ch]
    Vec3 a3; // [esp+180h] [ebp-38h] BYREF
    int i; // [esp+18Ch] [ebp-2Ch]
    tScriptStruct_Porte* v66; // [esp+190h] [ebp-28h]
    Vec3 v67; // [esp+194h] [ebp-24h] BYREF
    Vec3 v68; // [esp+1A4h] [ebp-14h] BYREF
    BOOL v69; // [esp+1B0h] [ebp-8h]
    float v70; // [esp+1B4h] [ebp-4h]

    v57.x = 1.0;
    v57.y = 0.0;
    v57.z = 0.0;
    v41.x = 1.0;
    v41.y = 0.0;
    v41.z = 0.0;
    v33.x = 1.0;
    v33.y = 0.0;
    v33.z = 0.0;
    v53.x = 1.0;
    v53.y = 0.0;
    v53.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_Porte*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v66 = v4;
    i = 0;
    v70 = 0.0;
    v29 = (tGameObjectMain*)v4->Capa_qui_me_commande1;
    if (v29)
    {
        if ((unsigned int)v29 <= 2)
            v29 = AI_MainActor[(uint32_t)v29 - 1];
        v0 = AI_EvalFunc_OBJ_CapaTest_C(v29, 0x100);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x100);
    }
    v40 = v0;
    if (v0)
        v66->i_must_open = 1;
    v28 = (tGameObjectMain*)v66->o_open_max;
    if (v28)
    {
        if ((unsigned int)v28 <= 2)
            v28 = AI_MainActor[(uint32_t)v28 - 1];
        AI_EvalFunc_OBJGetPos_C(v28, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    MATH_VecSet(&v33, &a3);
    v27 = (tGameObjectMain*)v66->o_close_max;
    if (v27)
    {
        if ((unsigned int)v27 <= 2)
            v27 = AI_MainActor[(uint32_t)v27 - 1];
        AI_EvalFunc_OBJGetPos_C(v27, &v61);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v61);
    }
    MATH_VecSet(&v41, &v61);
    MATH_SubstractVec(&dest, &v33, &v41);
    AI_EvalFunc_MATHVecNormalize_C(&dest, &v30);
    v57 = v30;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v52);
    v53 = v52;
    if (v66->i_must_open)
    {
        if (v66->i_snd_instance == -1)
        {
            v1 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000000);
            v66->i_snd_instance = v1;
            AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v66->i_snd_instance);
        }
        v26 = (tGameObjectMain*)v66->o_halo_simple;
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
            v24 = (tGameObjectMain*)v66->o_halo_simple;
            if (v24)
            {
                if ((unsigned int)v24 <= 2)
                    v24 = AI_MainActor[(uint32_t)v24 - 1];
                AI_EvalFunc_OBJ_CapaSet_C(v24, 3, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_CapaSet_C(0, 3, 0);
            }
        }
        for (i = 0; i < 3; ++i)
        {
            v23 = (tGameObjectMain*)v66->o_halo_simple_supl[i];
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
                v21 = (tGameObjectMain*)v66->o_halo_simple_supl[i];
                if (v21)
                {
                    if ((unsigned int)v21 <= 2)
                        v21 = AI_MainActor[(uint32_t)v21 - 1];
                    AI_EvalFunc_OBJ_CapaSet_C(v21, 3, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_CapaSet_C(0, 3, 0);
                }
            }
        }
        v20 = v66->f_open_speed;
        v37.x = v57.x * v20;
        v37.y = v57.y * v20;
        v37.z = v57.z * v20;
        v62 = TIM_gf_dt;
        MATH_VecMultScalar(&v67, &v37, TIM_gf_dt);
        v53.x = v53.x + v67.x;
        v53.y = v53.y + v67.y;
        v53.z = v53.z + v67.z;
        MATH_SubstractVec(&v38, &v33, &v53);
        v70 = v57.z * v38.z + v57.y * v38.y + v38.x * v57.x;
        if (v70 < 0.0)
        {
            v34 = v57.x * v70;
            v35 = v57.y * v70;
            v36 = v57.z * v70;
            v53.x = v53.x + v34;
            v53.y = v53.y + v35;
            v53.z = v53.z + v36;
            v66->f_grav = 0.0;
            v19 = (tGameObjectMain*)v66->Capa_qui_me_commande1;
            if (v19)
            {
                if ((unsigned int)v19 <= 2)
                    v19 = AI_MainActor[(uint32_t)v19 - 1];
                v2 = AI_EvalFunc_OBJ_CapaTest_C(v19, 0x100);
            }
            else
            {
                v2 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x100);
            }
            v69 = v2;
            if (!v2)
                v66->i_must_open = 0;
        }
    }
    else
    {
        if (v66->f_gravity_close_max >= (double)v66->f_grav)
        {
            v31 = TIM_gf_dt;
            v66->f_grav = v66->f_gravity * TIM_gf_dt + v66->f_grav;
            v17 = v66->f_grav;
            v46 = v57.x * v17;
            v47 = v57.y * v17;
            v48 = v57.z * v17;
            v63 = TIM_gf_dt;
            v54 = v46 * TIM_gf_dt;
            v55 = v47 * TIM_gf_dt;
            v56 = v48 * TIM_gf_dt;
            v53.x = v53.x - v54;
            v53.y = v53.y - v55;
            v53.z = v53.z - v56;
        }
        else
        {
            v18 = v66->f_close_speed;
            v58 = v57.x * v18;
            v59 = v57.y * v18;
            v60 = v57.z * v18;
            v39 = TIM_gf_dt;
            v49 = v58 * TIM_gf_dt;
            v50 = v59 * TIM_gf_dt;
            v51 = v60 * TIM_gf_dt;
            v53.x = v53.x - v49;
            v53.y = v53.y - v50;
            v53.z = v53.z - v51;
        }
        MATH_SubstractVec(&v68, &v41, &v53);
        v70 = v57.z * v68.z + v57.y * v68.y + v68.x * v57.x;
        if (v70 <= 0.0)
        {
            v10 = (tGameObjectMain*)v66->o_halo_simple;
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
                v8 = (tGameObjectMain*)v66->o_halo_simple;
                if (v8)
                {
                    if ((unsigned int)v8 <= 2)
                        v8 = AI_MainActor[(uint32_t)v8 - 1];
                    AI_EvalFunc_OBJ_CapaSet_C(v8, 0, 3);
                }
                else
                {
                    AI_EvalFunc_OBJ_CapaSet_C(0, 0, 3);
                }
            }
            for (i = 0; i < 3; ++i)
            {
                v7 = (tGameObjectMain*)v66->o_halo_simple_supl[i];
                if (v7)
                {
                    if ((unsigned int)v7 <= 2)
                        v7 = AI_MainActor[(uint32_t)v7 - 1];
                    v6 = v7;
                }
                else
                {
                    v6 = 0;
                }
                if (v6)
                {
                    v5 = (tGameObjectMain*)v66->o_halo_simple_supl[i];
                    if (v5)
                    {
                        if ((unsigned int)v5 <= 2)
                            v5 = AI_MainActor[(uint32_t)v5 - 1];
                        AI_EvalFunc_OBJ_CapaSet_C(v5, 0, 3);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_CapaSet_C(0, 0, 3);
                    }
                }
            }
        }
        else
        {
            if (v66->i_snd_instance != -1)
            {
                SND_Release(v66->i_snd_instance);
                v66->i_snd_instance = -1;
                v45 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(2, -1.0);
            }
            v42 = v57.x * v70;
            v43 = v57.y * v70;
            v44 = v57.z * v70;
            v53.x = v53.x + v42;
            v53.y = v53.y + v43;
            v53.z = v53.z + v44;
            v16 = (tGameObjectMain*)v66->o_halo_simple;
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
                v14 = (tGameObjectMain*)v66->o_halo_simple;
                if (v14)
                {
                    if ((unsigned int)v14 <= 2)
                        v14 = AI_MainActor[(uint32_t)v14 - 1];
                    AI_EvalFunc_OBJ_CapaSet_C(v14, 1, 2);
                }
                else
                {
                    AI_EvalFunc_OBJ_CapaSet_C(0, 1, 2);
                }
            }
            for (i = 0; i < 3; ++i)
            {
                v13 = (tGameObjectMain*)v66->o_halo_simple_supl[i];
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
                    v11 = (tGameObjectMain*)v66->o_halo_simple_supl[i];
                    if (v11)
                    {
                        if ((unsigned int)v11 <= 2)
                            v11 = AI_MainActor[(uint32_t)v11 - 1];
                        AI_EvalFunc_OBJ_CapaSet_C(v11, 1, 2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_CapaSet_C(0, 1, 2);
                    }
                }
            }
        }
    }
    AI_EvalFunc_OBJSetPos_C_CURRENT(&v53);
    return 1000;
}

int porte_po_ouvre_toi()
{
    float v1; // [esp+0h] [ebp-B8h]
    float v2; // [esp+0h] [ebp-B8h]
    tAINode* v3; // [esp+4h] [ebp-B4h]
    tScriptStruct_Porte* v4; // [esp+8h] [ebp-B0h]
    tGameObjectMain* v5; // [esp+14h] [ebp-A4h]
    tGameObjectMain* v6; // [esp+1Ch] [ebp-9Ch]
    tGameObjectMain* v7; // [esp+20h] [ebp-98h]
    tGameObjectMain* v8; // [esp+24h] [ebp-94h]
    tGameObjectMain* v9; // [esp+2Ch] [ebp-8Ch]
    tGameObjectMain* v10; // [esp+30h] [ebp-88h]
    tGameObjectMain* v11; // [esp+34h] [ebp-84h]
    tGameObjectMain* v12; // [esp+3Ch] [ebp-7Ch]
    tGameObjectMain* v13; // [esp+40h] [ebp-78h]
    tGameObjectMain* v14; // [esp+44h] [ebp-74h]
    tGameObjectMain* v15; // [esp+4Ch] [ebp-6Ch]
    tGameObjectMain* v16; // [esp+50h] [ebp-68h]
    tGameObjectMain* v17; // [esp+54h] [ebp-64h]
    tGameObjectMain* v18; // [esp+5Ch] [ebp-5Ch]
    tGameObjectMain* v19; // [esp+60h] [ebp-58h]
    tGameObjectMain* v20; // [esp+64h] [ebp-54h]
    tGameObjectMain* v21; // [esp+6Ch] [ebp-4Ch]
    tGameObjectMain* v22; // [esp+70h] [ebp-48h]
    tGameObjectMain* v23; // [esp+74h] [ebp-44h]
    float v24; // [esp+7Ch] [ebp-3Ch]
    Vec3 a3; // [esp+80h] [ebp-38h] BYREF
    Vec3 v27; // [esp+94h] [ebp-24h] BYREF
    tScriptStruct_Porte* v28; // [esp+A8h] [ebp-10h]
    int i; // [esp+ACh] [ebp-Ch]
    int v30; // [esp+B0h] [ebp-8h]
    float v31; // [esp+B4h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_Porte*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v28 = v4;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        porte_po_ouvre_toi_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &porte_po_ouvre_toi_GST_Global_gao_i_map_just_reset,
            (void*)porte_po_ouvre_toi_GST_Global_gao,
            3300);
        porte_po_ouvre_toi_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&porte_po_ouvre_toi_univVars_Flag_speciaux_sauve, (void*)porte_po_ouvre_toi_univVars, 4488);
        fct_porte_po_ouvre_toi_porte_po_etat_ouverte = (int)AI_ul_RealResolveOneRefFunction(0x7100BF01u);
    }
    v3 = *AI_gppst_CurrentJumpNode;
    if (*AI_gppst_CurrentJumpNode == (tAINode*)11)
    {
        for (i = 0; i < v28->i_nbr_track_ouverture; ++i)
            AI_EvalFunc_TRASetEnd_C(AI_gpst_CurrentGameObject, i);
    }
    else if (v3 != (tAINode*)12)
    {
        if (v3 == (tAINode*)24)
        {
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_porte_po_ouvre_toi_porte_po_etat_ouverte);
            return 1000;
        }
        v23 = (tGameObjectMain*)v28->o_desactive_a_louverture0;
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
            v21 = (tGameObjectMain*)v28->o_desactive_a_louverture0;
            if (v21)
            {
                if ((unsigned int)v21 <= 2)
                    v21 = AI_MainActor[(uint32_t)v21 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v21, 1);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
            }
        }
        v20 = (tGameObjectMain*)v28->o_desactive_a_louverture1;
        if (v20)
        {
            if ((unsigned int)v20 <= 2)
                v20 = AI_MainActor[(uint32_t)v20 - 1];
            v19 = v20;
        }
        else
        {
            v19 = 0;
        }
        if (v19)
        {
            v18 = (tGameObjectMain*)v28->o_desactive_a_louverture1;
            if (v18)
            {
                if ((unsigned int)v18 <= 2)
                    v18 = AI_MainActor[(uint32_t)v18 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v18, 1);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
            }
        }
        v17 = (tGameObjectMain*)v28->o_active_a_louverture0;
        if (v17)
        {
            if ((unsigned int)v17 <= 2)
                v17 = AI_MainActor[(uint32_t)v17 - 1];
            v16 = v17;
        }
        else
        {
            v16 = 0;
        }
        if (v16)
        {
            v15 = (tGameObjectMain*)v28->o_active_a_louverture0;
            if (v15)
            {
                if ((unsigned int)v15 <= 2)
                    v15 = AI_MainActor[(uint32_t)v15 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v15, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
            }
        }
        v14 = (tGameObjectMain*)v28->o_active_a_louverture1;
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
            v12 = (tGameObjectMain*)v28->o_active_a_louverture1;
            if (v12)
            {
                if ((unsigned int)v12 <= 2)
                    v12 = AI_MainActor[(uint32_t)v12 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v12, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
            }
        }
        v11 = (tGameObjectMain*)v28->o_evt_ouverture;
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
        if (v10)
        {
            v9 = (tGameObjectMain*)v28->o_evt_ouverture;
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                AI_EvalFunc_TRAPlayAll_C(v9);
            }
            else
            {
                AI_EvalFunc_TRAPlayAll_C(0);
            }
        }
        if (!AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 0x2000000))
            goto LABEL_106;
        for (i = 0; i < v28->i_nbr_track_ouverture; ++i)
            AI_EvalFunc_TRAPlay_C(AI_gpst_CurrentGameObject, i);
        if (v28->i_instantane)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)11;
            TIME_Wait_C(0.1f);
            return 0;
        }
        if (!*(uint32_t*)porte_po_ouvre_toi_GST_Global_gao_i_map_just_reset)
        {
            if (v28->snd_le_son_douverture_boucle)
            {
                v28->i_snd_instance = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000000);
                AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v28->i_snd_instance);
            }
            else
            {
                v28->i_snd_instance = AI_EvalFunc_SNDRequestPlay_C_CURRENT(0, -1.0);
            }
            v24 = SND_f_GetVol(v28->i_snd_instance);
            v1 = v28->f_coef_volume * v24;
            SND_SetVol(v28->i_snd_instance, v1);
        }
    }
    if (!AI_EvalFunc_TRAIsStopAll_C(AI_gpst_CurrentGameObject))
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)12;
        return 0;
    }
    if (!*(uint32_t*)porte_po_ouvre_toi_GST_Global_gao_i_map_just_reset)
    {
        if (v28->snd_le_son_douverture_boucle)
            SND_Release(v28->i_snd_instance);
        v28->i_snd_instance = AI_EvalFunc_SNDRequestPlay_C_CURRENT(2, -1.0);
        v31 = SND_f_GetVol(v28->i_snd_instance);
        v2 = v28->f_coef_volume * v31;
        SND_SetVol(v28->i_snd_instance, v2);
    }
    if (!v28->i_instantane)
    {
        v8 = (tGameObjectMain*)v28->TAG_WP_extremite_1;
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
            AI_EvalFunc_GRIDCurrentSet_C(v28->TAG_etage);
            v6 = (tGameObjectMain*)v28->TAG_WP_extremite_1;
            if (v6)
            {
                if ((unsigned int)v6 <= 2)
                    v6 = AI_MainActor[(uint32_t)v6 - 1];
                AI_EvalFunc_OBJGetPos_C(v6, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            v5 = (tGameObjectMain*)v28->TAG_WP_extremite_2;
            if (v5)
            {
                if ((unsigned int)v5 <= 2)
                    v5 = AI_MainActor[(uint32_t)v5 - 1];
                AI_EvalFunc_OBJGetPos_C(v5, &v27);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v27);
            }
            v30 = AI_EvalFunc_GRIDTagBox_C(AI_gpst_CurrentGameObject, &a3, &v27, -64);
        }
    }
    v28->i_instantane = 0;
LABEL_106:
    if (!v28->ne_touche_pas_aux_colmaps)
        AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
    if (v28->SF_qui_me_commande > 0)
        AI_EvalFunc_ARRBitSet_C((int*)(porte_po_ouvre_toi_univVars_Flag_speciaux_sauve + 4), v28->SF_qui_me_commande);
    *AI_gppst_CurrentJumpNode = (tAINode*)24;
    TIME_Wait_C(v28->attente_sup_apres_mvt);
    return 0;
}