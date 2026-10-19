#include "ai_compiledscripts_inc.h"

//vaisseau_hyllis_vh_init
int vaisseau_hyllis_vh_init_univVars;
int vaisseau_hyllis_vh_init_univVars_Flag_speciaux_sauve;
int vaisseau_hyllis_vh_init_GST_Global_gao;
int vaisseau_hyllis_vh_init_GST_Global_gao_i_map_just_reset;
int vaisseau_hyllis_vh_init_GST_Global_gao_o_targets;
int vaisseau_hyllis_vh_init_GST_Global_gao_i_targets_nb;
int vaisseau_hyllis_vh_init_GST_Global_gao_v_targets_offset;
int vaisseau_hyllis_vh_init_univVars_Planetes_CurHour;
int fct_vaisseau_hyllis_vh_init_vaisseau_hyllis_vh_go;

//vaisseau_hyllis_vh_go
int fct_vaisseau_hyllis_vh_go_vaisseau_hyllis_vh_go_vrille;
int fct_vaisseau_hyllis_vh_go_vaisseau_hyllis_vh_go_move;

//vaisseau_hyllis_vh_go_vrille
int fct_vaisseau_hyllis_vh_go_vrille_vaisseau_hyllis_vh_explosion;

//vaisseau_hyllis_vh_explosion
int vaisseau_hyllis_vh_explosion_SFX_explosion_gao;

//vaisseau_hyllis_vh_go_move
int fctvar_vaisseau_hyllis_vh_go_move_vaisseau_hyllis_vh;

//
int vaisseau_hyllis_vh_reflexe_SFX_light_smoke_gao;
int vaisseau_hyllis_vh_reflexe_univVars;
int vaisseau_hyllis_vh_reflexe_univVars_Planetes_CurHour;
int vaisseau_hyllis_vh_reflexe_SFX_explosion_gao;

int vaisseau_hyllis_vh_init()
{
    int result; // eax
    float v1; // [esp+4h] [ebp-1A4h]
    tScriptStruct_ActorVaisseauHyllis* v2; // [esp+Ch] [ebp-19Ch]
    double v3; // [esp+10h] [ebp-198h]
    tGameObjectMain* v4; // [esp+1Ch] [ebp-18Ch]
    tGameObjectMain* v5; // [esp+24h] [ebp-184h]
    tGameObjectMain* v6; // [esp+2Ch] [ebp-17Ch]
    tGameObjectMain* v7; // [esp+3Ch] [ebp-16Ch]
    Vec3* v8; // [esp+48h] [ebp-160h]
    tGameObjectMain* v9; // [esp+64h] [ebp-144h]
    tGameObjectMain* v10; // [esp+68h] [ebp-140h]
    tGameObjectMain* v11; // [esp+78h] [ebp-130h]
    tGameObjectMain* v12; // [esp+80h] [ebp-128h]
    Vec3 v13; // [esp+88h] [ebp-120h] BYREF
    Vec3 a3; // [esp+94h] [ebp-114h] BYREF
    Vec3 v15; // [esp+A0h] [ebp-108h] BYREF
    int v16; // [esp+ACh] [ebp-FCh]
    float v17; // [esp+B4h] [ebp-F4h]
    Vec3 v18; // [esp+B8h] [ebp-F0h] BYREF
    int v19; // [esp+C4h] [ebp-E4h]
    tGameObjectMain* v20; // [esp+C8h] [ebp-E0h]
    Vec3 v21; // [esp+DCh] [ebp-CCh] BYREF
    tGameObjectMain* v22; // [esp+E8h] [ebp-C0h]
    Vec3 v23; // [esp+ECh] [ebp-BCh] BYREF
    Vec3 v24; // [esp+F8h] [ebp-B0h] BYREF
    Vec3 dest; // [esp+104h] [ebp-A4h] BYREF
    Vec3 a1; // [esp+118h] [ebp-90h] BYREF
    Vec3 v27; // [esp+124h] [ebp-84h]
    BOOL v28; // [esp+130h] [ebp-78h]
    int v29; // [esp+134h] [ebp-74h]
    int v30; // [esp+138h] [ebp-70h]
    Vec3 v31; // [esp+140h] [ebp-68h] BYREF
    Vec3 v32; // [esp+14Ch] [ebp-5Ch]
    Vec3 v33; // [esp+168h] [ebp-40h] BYREF
    int v34; // [esp+174h] [ebp-34h]
    tScriptStruct_ActorVaisseauHyllis* v35; // [esp+178h] [ebp-30h]
    float v36; // [esp+17Ch] [ebp-2Ch]
    int v37; // [esp+180h] [ebp-28h]
    Vec3 v38; // [esp+184h] [ebp-24h] BYREF
    int v39; // [esp+190h] [ebp-18h]
    float v40; // [esp+194h] [ebp-14h]
    Vec3 v41; // [esp+198h] [ebp-10h] BYREF

    v18.x = 1.0;
    v18.y = 0.0;
    v18.z = 0.0;
    v27.x = 1.0;
    v27.y = 0.0;
    v27.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorVaisseauHyllis*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v35 = v2;
    v29 = 0;
    v37 = 0;
    v22 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        vaisseau_hyllis_vh_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &vaisseau_hyllis_vh_init_univVars_Flag_speciaux_sauve,
            (void*)vaisseau_hyllis_vh_init_univVars,
            4488);
        vaisseau_hyllis_vh_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &vaisseau_hyllis_vh_init_GST_Global_gao_i_map_just_reset,
            (void*)vaisseau_hyllis_vh_init_GST_Global_gao,
            3300);
        AI2C_ResolveBuffer(
            &vaisseau_hyllis_vh_init_GST_Global_gao_o_targets,
            (void*)vaisseau_hyllis_vh_init_GST_Global_gao,
            964);
        AI2C_ResolveBuffer(
            &vaisseau_hyllis_vh_init_GST_Global_gao_i_targets_nb,
            (void*)vaisseau_hyllis_vh_init_GST_Global_gao,
            2572);
        AI2C_ResolveBuffer(
            &vaisseau_hyllis_vh_init_GST_Global_gao_v_targets_offset,
            (void*)vaisseau_hyllis_vh_init_GST_Global_gao,
            1368);
        AI2C_ResolveBuffer(
            &vaisseau_hyllis_vh_init_univVars_Planetes_CurHour,
            (void*)vaisseau_hyllis_vh_init_univVars,
            6116);
        fct_vaisseau_hyllis_vh_init_vaisseau_hyllis_vh_go = (int)AI_ul_RealResolveOneRefFunction(0x3D000E51u);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)3)
    {
        if (*(uint32_t*)vaisseau_hyllis_vh_init_GST_Global_gao_i_map_just_reset)
            goto LABEL_28;
        v22 = AI_EvalFunc_VIEWGetObject_C(0);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v15);
        v12 = v22;
        if (v22)
        {
            if ((unsigned int)v22 <= 2)
                v12 = AI_MainActor[(uint32_t)v22 - 1];
            AI_EvalFunc_OBJGetPos_C(v12, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        MATH_SubstractVec(&dest, &v15, &a3);
        v11 = v22;
        if (v22)
        {
            if ((unsigned int)v22 <= 2)
                v11 = AI_MainActor[(uint32_t)v22 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v11, &v38);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &v38);
        }
        v17 = v38.z * dest.z + v38.y * dest.y + dest.x * v38.x;
        if (v17 <= -100.0)
        {
        LABEL_28:
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x800);
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
            v32.x = 0.0;
            v32.y = 0.0;
            v32.z = 19.0;
            v27.x = 0.0;
            v27.y = 0.0;
            v27.z = 19.0;
            v39 = AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 8, 0);
            v20 = AI_gpst_CurrentGameObject;
            v10 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v10 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                v9 = v10;
            }
            else
            {
                v9 = 0;
            }
            v29 = AI_EvalFunc_ARRObjSearch_C(
                (tGameObjectMain**)(vaisseau_hyllis_vh_init_GST_Global_gao_o_targets + 4),
                *(uint32_t*)vaisseau_hyllis_vh_init_GST_Global_gao_i_targets_nb,
                v9);
            if (v29 == -1)
            {
                v29 = AI_EvalFunc_ARRObjSearch_C(
                    (tGameObjectMain**)(vaisseau_hyllis_vh_init_GST_Global_gao_o_targets + 4),
                    100,
                    0);
                if (v29 != -1)
                {
                    ++* (uint32_t*)vaisseau_hyllis_vh_init_GST_Global_gao_i_targets_nb;
                    *(uint32_t*)(vaisseau_hyllis_vh_init_GST_Global_gao_o_targets + 4 * v29 + 4) = (uint32_t)AI_gpst_CurrentGameObject;
                    v8 = (Vec3*)AI_EvalVarArray_C3((char*)vaisseau_hyllis_vh_init_GST_Global_gao_v_targets_offset, v29);
                    *v8 = v27;
                }
            }
            v28 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
            v19 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 50499588, 0);
            v24.x = 0.0;
            v24.y = 0.0;
            v24.z = 0.0;
            AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v24);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v13);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v23);
            v33.x = v23.x * 1000.0f;
            v33.y = v23.y * 1000.0f;
            v33.z = v23.z * 1000.0f;
            MATH_VecAddVec(&a1, &v13, &v33);
            v35->v_postogo = a1;
            v7 = (tGameObjectMain*)v35->des_o_init;
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                AI_EvalFunc_OBJGetPos_C(v7, &v41);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v41);
            }
            v18 = v41;
            v1 = (float)v35->des_i_id;
            v18.z = v35->f_rank_z * v1 + v41.z;
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v18);
            v6 = (tGameObjectMain*)v35->des_o_init;
            if (v6)
            {
                if ((unsigned int)v6 <= 2)
                    v6 = AI_MainActor[(uint32_t)v6 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v6, &v31);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v31);
            }
            v5 = (tGameObjectMain*)v35->des_o_init;
            if (v5)
            {
                if ((unsigned int)v5 <= 2)
                    v5 = AI_MainActor[(uint32_t)v5 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v5, &v21);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v21);
            }
            AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v31, &v21);
            v35->o_next_wp = v35->des_o_init;
            v4 = (tGameObjectMain*)v35->o_next_wp;
            if (v4)
            {
                if ((unsigned int)v4 <= 2)
                    v4 = AI_MainActor[(uint32_t)v4 - 1];
                AI_EvalFunc_OBJGetPos_C(v4, &v35->v_postogo);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v35->v_postogo);
            }
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v35->v_cur_sight);
            v16 = AI_EvalFunc_MATHRandInt_C(1, 20);
            v35->f_alea_halo = (float)(1000 * v16);
            v40 = AI_EvalFunc_MATHRandFloat_C(0.0, 5.0);
            v35->f_alea_speed = v40 * 10.0f;
            v35->f_speed_pal = AI_EvalFunc_MATHRandFloat_C(15.0, 25.0);
            v35->i_cur_replique = v35->des_i_replique + 1;
            v3 = *(float*)vaisseau_hyllis_vh_init_univVars_Planetes_CurHour + MATH_gd_magicDoubleForRound;
            v37 = (*(uint32_t*)&v3);
            if ((*(int32_t*)&v3) > 17 || v37 < 6)
                v35->i_ok_halo = 1;
            v36 = TIM_gf_MainClock - TIM_gf_LockTime;
            v35->f_time_smoke = v36 - 10.0f;
            v35->f_freq = AI_EvalFunc_MATHRandFloat_C(0.94999999f, 1.05f);
            v30 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_vaisseau_hyllis_vh_init_vaisseau_hyllis_vh_go);
            if (v30 == 1000)
                result = 1000;
            else
                result = v30;
        }
        else
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 4;
        }
    }
    else if (!v35->des_i_sf_je_suis_la
        || (v35->des_i_sf_je_suis_la <= 0 ? (v37 = 0) : (v37 = AI_EvalFunc_ARRBitTest_C(
            (int*)(vaisseau_hyllis_vh_init_univVars_Flag_speciaux_sauve
                + 4),
            v35->des_i_sf_je_suis_la)),
            v37 || (v34 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject), v34 == 1000)))
    {
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        *AI_gppst_CurrentJumpNode = (tAINode*)3;
        TIME_Wait_C(0.1f);
        result = 0;
    }
    else
    {
        result = v34;
    }
    return result;
}

int vaisseau_hyllis_vh_go()
{
    double v0; // st7
    int result; // eax
    BOOL v2; // eax
    BOOL v3; // eax
    BOOL v4; // eax
    BOOL v5; // eax
    float v6; // [esp+0h] [ebp-14Ch]
    tScriptStruct_ActorVaisseauHyllis* v7; // [esp+4h] [ebp-148h]
    float v8; // [esp+8h] [ebp-144h]
    Vec3* v9; // [esp+Ch] [ebp-140h]
    tAI* v10; // [esp+14h] [ebp-138h]
    tAI* v11; // [esp+18h] [ebp-134h]
    tGameObjectMain* v12; // [esp+1Ch] [ebp-130h]
    tGameObjectMain* v13; // [esp+20h] [ebp-12Ch]
    Vec3* v14; // [esp+24h] [ebp-128h]
    tGameObjectMain* v15; // [esp+34h] [ebp-118h]
    float v16; // [esp+40h] [ebp-10Ch]
    tGameObjectMain* v17; // [esp+48h] [ebp-104h]
    tGameObjectMain* v18; // [esp+50h] [ebp-FCh]
    tGameObjectMain* v19; // [esp+58h] [ebp-F4h]
    tGameObjectMain* v20; // [esp+60h] [ebp-ECh]
    tGameObjectMain* v21; // [esp+68h] [ebp-E4h]
    tGameObjectMain* v22; // [esp+70h] [ebp-DCh]
    tGameObjectMain* v23; // [esp+74h] [ebp-D8h]
    tGameObjectMain* v24; // [esp+78h] [ebp-D4h]
    tAI* v25; // [esp+7Ch] [ebp-D0h]
    tAI* v26; // [esp+80h] [ebp-CCh]
    tGameObjectMain* v27; // [esp+84h] [ebp-C8h]
    tGameObjectMain* v28; // [esp+88h] [ebp-C4h]
    tGameObjectMain* v29; // [esp+8Ch] [ebp-C0h]
    tGameObjectMain* v30; // [esp+90h] [ebp-BCh]
    tGameObjectMain* v31; // [esp+98h] [ebp-B4h]
    float v32; // [esp+9Ch] [ebp-B0h]
    float v33; // [esp+9Ch] [ebp-B0h]
    float v34; // [esp+A0h] [ebp-ACh]
    float v35; // [esp+A0h] [ebp-ACh]
    Vec3 v36; // [esp+A4h] [ebp-A8h] BYREF
    int v37; // [esp+B0h] [ebp-9Ch]
    int v38; // [esp+B4h] [ebp-98h]
    Vec3 dest; // [esp+B8h] [ebp-94h] BYREF
    BOOL v40; // [esp+C4h] [ebp-88h]
    Vec3 v41; // [esp+C8h] [ebp-84h]
    Vec3 a1; // [esp+D4h] [ebp-78h] BYREF
    Vec3 v43; // [esp+E0h] [ebp-6Ch] BYREF
    tGameObjectMain* v44; // [esp+ECh] [ebp-60h]
    float v45; // [esp+F4h] [ebp-58h]
    BOOL v46; // [esp+F8h] [ebp-54h]
    Vec3 v47; // [esp+FCh] [ebp-50h] BYREF
    Vec3 v48; // [esp+108h] [ebp-44h] BYREF
    int v49; // [esp+114h] [ebp-38h]
    BOOL v50; // [esp+118h] [ebp-34h]
    unsigned int v51; // [esp+120h] [ebp-2Ch]
    float v52; // [esp+124h] [ebp-28h]
    float v53; // [esp+128h] [ebp-24h]
    tScriptStruct_ActorVaisseauHyllis* v54; // [esp+12Ch] [ebp-20h]
    Vec3 a3; // [esp+130h] [ebp-1Ch] BYREF
    BOOL v56; // [esp+13Ch] [ebp-10h]
    Vec3 v57; // [esp+140h] [ebp-Ch] BYREF

    v41.x = 1.0;
    v41.y = 0.0;
    v41.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_ActorVaisseauHyllis*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    v54 = v7;
    v51 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_vaisseau_hyllis_vh_go_vaisseau_hyllis_vh_go_vrille = (int)AI_ul_RealResolveOneRefFunction(0x72004D96u);
        fct_vaisseau_hyllis_vh_go_vaisseau_hyllis_vh_go_move = (int)AI_ul_RealResolveOneRefFunction(0x72004D0Fu);
    }
    v44 = *AI_MainActor;
    v31 = v44;
    if (v44)
    {
        if ((unsigned int)v44 <= 2)
            v31 = AI_MainActor[(uint32_t)v44 - 1];
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v31);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v53 = (float)v0;
    if (v53 >= 640000.0)
    {
        if (v54->i_snd_indice_loop != -1)
        {
            SND_Release(v54->i_snd_indice_loop);
            v54->i_snd_indice_loop = -1;
        }
    }
    else if (v54->i_snd_indice_loop == -1)
    {
        v54->i_snd_indice_loop = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000000);
        AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v54->i_snd_indice_loop);
        v38 = SND_l_GetFreq(v54->i_snd_indice_loop);
        v6 = (float)v38;
        SND_SetFreq(v54->i_snd_indice_loop, (unsigned __int64)(v54->f_freq * v6));
    }
    if (v54->i_flag_gameover)
    {
        v37 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_vaisseau_hyllis_vh_go_vaisseau_hyllis_vh_go_vrille);
        if (v37 != 1000)
            return v37;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 4;
    }
    v30 = (tGameObjectMain*)v54->o_leader;
    if (v30)
    {
        if ((unsigned int)v30 <= 2)
            v30 = AI_MainActor[(uint32_t)v30 - 1];
        v29 = v30;
    }
    else
    {
        v29 = 0;
    }
    if (v29)
    {
        if (v54->i_force_leader)
            goto LABEL_65;
        v26 = (tAI*)v54->o_leader;
        if (v26 == WOR_gst_Universe.univAIInstance)
        {
            v25 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v28 = (tGameObjectMain*)v54->o_leader;
            if (v26)
            {
                if ((unsigned int)v26 <= 2)
                    v28 = AI_MainActor[(uint32_t)v26 - 1];
                v27 = v28;
            }
            else
            {
                v27 = 0;
            }
            v25 = v27->extended->AIInstance;
        }
        v51 = *((uint32_t*)v25->instanceVarDataCurrent + 6);// TODO FIXME: what is it?
        v24 = (tGameObjectMain*)v51;
        if (v51)
        {
            if (v51 <= 2)
                v24 = AI_MainActor[v51 - 1];
            v23 = v24;
        }
        else
        {
            v23 = 0;
        }
        if (v23)
        {
            v22 = (tGameObjectMain*)v54->o_next_wp;
            if (v22)
            {
                if ((unsigned int)v22 <= 2)
                    v22 = AI_MainActor[(uint32_t)v22 - 1];
                v2 = AI_EvalFunc_OBJ_CapaTest_C(v22, 1);
            }
            else
            {
                v2 = AI_EvalFunc_OBJ_CapaTest_C(0, 1);
            }
            v46 = v2;
            if (v2)
                goto LABEL_106;
            v21 = (tGameObjectMain*)v51;
            if (v51)
            {
                if (v51 <= 2)
                    v21 = AI_MainActor[v51 - 1];
                v3 = AI_EvalFunc_OBJ_CapaTest_C(v21, 1);
            }
            else
            {
                v3 = AI_EvalFunc_OBJ_CapaTest_C(0, 1);
            }
            v56 = v3;
            if (!v3)
            {
            LABEL_106:
                v20 = (tGameObjectMain*)v54->o_next_wp;
                if (v20)
                {
                    if ((unsigned int)v20 <= 2)
                        v20 = AI_MainActor[(uint32_t)v20 - 1];
                    v4 = AI_EvalFunc_OBJ_CapaTest_C(v20, 1);
                }
                else
                {
                    v4 = AI_EvalFunc_OBJ_CapaTest_C(0, 1);
                }
                v40 = v4;
                if (!v4)
                    goto LABEL_65;
                v19 = (tGameObjectMain*)v51;
                if (v51)
                {
                    if (v51 <= 2)
                        v19 = AI_MainActor[v51 - 1];
                    v5 = AI_EvalFunc_OBJ_CapaTest_C(v19, 1);
                }
                else
                {
                    v5 = AI_EvalFunc_OBJ_CapaTest_C(0, 1);
                }
                v50 = v5;
                if (v5)
                {
                LABEL_65:
                    v54->i_force_leader = 1;
                    v18 = (tGameObjectMain*)v54->o_leader;
                    if (v18)
                    {
                        if ((unsigned int)v18 <= 2)
                            v18 = AI_MainActor[(uint32_t)v18 - 1];
                        AI_EvalFunc_OBJGetPos_C(v18, &a3);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &a3);
                    }
                    v17 = (tGameObjectMain*)v54->o_leader;
                    if (v17)
                    {
                        if ((unsigned int)v17 <= 2)
                            v17 = AI_MainActor[(uint32_t)v17 - 1];
                        AI_EvalFunc_OBJ_SightGet_C(v17, &v43);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_SightGet_C(0, &v43);
                    }
                    v16 = v54->f_forcepostogo;
                    v48.x = v43.x * v16;
                    v48.y = v43.y * v16;
                    v48.z = v43.z * v16;
                    MATH_SubstractVec(&dest, &a3, &v48);
                    v54->v_postogo = dest;
                    v15 = (tGameObjectMain*)v54->o_leader;
                    if (v15)
                    {
                        if ((unsigned int)v15 <= 2)
                            v15 = AI_MainActor[(uint32_t)v15 - 1];
                        AI_EvalFunc_MATHVecLocalToGlobal_C(v15, &v54->v_forcepostogo, &v57);
                    }
                    else
                    {
                        AI_EvalFunc_MATHVecLocalToGlobal_C(0, &v54->v_forcepostogo, &v57);
                    }
                    v41 = v57;
                    v14 = &v54->v_postogo;
                    v54->v_postogo.x = v57.x + v54->v_postogo.x;
                    v14->y = v41.y + v14->y;
                    v14->z = v41.z + v14->z;
                    v11 = (tAI*)v54->o_leader;
                    if (v11 == WOR_gst_Universe.univAIInstance)
                    {
                        v10 = WOR_gst_Universe.univAIInstance;
                    }
                    else
                    {
                        v13 = (tGameObjectMain*)v54->o_leader;
                        if (v11)
                        {
                            if ((unsigned int)v11 <= 2)
                                v13 = AI_MainActor[(uint32_t)v11 - 1];
                            v12 = v13;
                        }
                        else
                        {
                            v12 = 0;
                        }
                        v10 = v12->extended->AIInstance;
                    }
                    v32 = *((float*)v10->instanceVarDataCurrent + 39);
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v47);
                    MATH_SubstractVec(&a1, &v54->v_postogo, &v47);
                    v34 = MATH_VecLen(&a1);
                    if (v34 < 5.0)
                    {
                        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v36);
                        v9 = &v54->v_postogo;
                        v54->v_postogo.x = v36.x + v54->v_postogo.x;
                        v9->y = v36.y + v9->y;
                        v9->z = v36.z + v9->z;
                    }
                    v35 = v34 / 200.0f;
                    if (v35 > 1.5)
                        v35 = 1.5;
                    if (v35 < 0.80000001)
                        v35 = 0.80000001f;
                    v33 = v32 * v35;
                    goto LABEL_97;
                }
            }
        }
    }
    else
    {
        v54->i_force_leader = 0;
    }
    v33 = v54->des_f_max_speed;
LABEL_97:
    v45 = TIM_gf_dt;
    v8 = v54->des_f_blend_speed * TIM_gf_dt;
    if (v8 < 0.0)
        v8 = 0.0;
    if (v8 > 1.0)
        v8 = 1.0;
    v52 = v33 * v8 + v54->f_speed * (1.0f - v8);
    v54->f_speed = v52;
    v49 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_vaisseau_hyllis_vh_go_vaisseau_hyllis_vh_go_move);
    if (v49 == 1000)
        result = 1000;
    else
        result = v49;
    return result;
}

int vaisseau_hyllis_vh_go_vrille()
{
    float a4; // [esp+0h] [ebp-E8h]
    tScriptStruct_ActorVaisseauHyllis* v2; // [esp+8h] [ebp-E0h]
    tGameObjectMain* v3; // [esp+10h] [ebp-D8h]
    tGameObjectMain* v4; // [esp+1Ch] [ebp-CCh]
    tGameObjectMain* v5; // [esp+20h] [ebp-C8h]
    tGameObjectMain* v6; // [esp+24h] [ebp-C4h]
    tGameObjectMain* v7; // [esp+28h] [ebp-C0h]
    Vec3 a2; // [esp+40h] [ebp-A8h] BYREF
    Vec3 v9; // [esp+4Ch] [ebp-9Ch] BYREF
    Vec3 a3; // [esp+58h] [ebp-90h] BYREF
    Vec3 v11; // [esp+64h] [ebp-84h] BYREF
    Vec3 v12; // [esp+70h] [ebp-78h] BYREF
    Vec3 v13; // [esp+7Ch] [ebp-6Ch] BYREF
    Vec3 v14; // [esp+88h] [ebp-60h] BYREF
    Vec3 v15; // [esp+94h] [ebp-54h] BYREF
    float v16; // [esp+A0h] [ebp-48h]
    tGameObjectMain* v17; // [esp+A4h] [ebp-44h]
    Vec3 v18; // [esp+A8h] [ebp-40h] BYREF
    tScriptStruct_ActorVaisseauHyllis* v19; // [esp+BCh] [ebp-2Ch]
    float v20; // [esp+C0h] [ebp-28h]
    Vec3 v21; // [esp+C4h] [ebp-24h] BYREF
    Vec3 a5; // [esp+D0h] [ebp-18h] BYREF
    Vec3 v23; // [esp+DCh] [ebp-Ch]

    v15.x = 1.0;
    v15.y = 0.0;
    v15.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorVaisseauHyllis*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v19 = v2;
    v20 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_vaisseau_hyllis_vh_go_vrille_vaisseau_hyllis_vh_explosion = (int)AI_ul_RealResolveOneRefFunction(0x72004D98u);
    }
    v13.x = 0.0;
    v13.y = 0.0;
    v13.z = -40.0;
    AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v13);
    v20 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
    if (v20 != 0.0)
    {
        AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v18);
        MATH_VecMultNormScalar(&v9, &v18, v20);
        a3 = v9;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
        v16 = TIM_gf_dt;
        a4 = 2.0f * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
        a3 = a5;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v12);
        AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a3, &v12);
        v20 = v20 * TIM_gf_dt;
        v20 = v20 + 25.0f;
        AI_EvalFunc_COL_SpecificCrossableSet_C(0);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v21);
        if (v21.z < 10.0)
            goto LABEL_14;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v14);
        v17 = AI_EvalFunc_COLRayObjectDist_C(AI_gpst_CurrentGameObject, &v14, &a3, v20, -1, 0, 0, -2147483647);
        v7 = v17;
        if (v17)
        {
            if ((int)v17 <= 2)
                v7 = AI_MainActor[(int)v17 - 1];
            v6 = v7;
        }
        else
        {
            v6 = 0;
        }
        if (v6)
            LABEL_14:
        AI_EvalFunc_AITrackChange_C(0, (tAIFunction*)fct_vaisseau_hyllis_vh_go_vrille_vaisseau_hyllis_vh_explosion);
    }
    v5 = (tGameObjectMain*)v19->o_gameover_explosion;
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
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v11);
        v23.x = 0.0;
        v23.y = 0.0;
        v23.z = 8.0;
        v15.x = v11.x + 0.0f;
        v15.y = v11.y + 0.0f;
        v15.z = v11.z + 8.0f;
        v3 = (tGameObjectMain*)v19->o_gameover_explosion;
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            AI_EvalFunc_OBJSetPos_C(v3, &v15);
        }
        else
        {
            AI_EvalFunc_OBJSetPos_C(0, &v15);
        }
    }
    return 1000;
}

int vaisseau_hyllis_vh_explosion()
{
    int v0; // eax
    tScriptStruct_ActorVaisseauHyllis* v2; // [esp+8h] [ebp-C8h]
    tGameObjectMain* v3; // [esp+10h] [ebp-C0h]
    tAI* v4; // [esp+14h] [ebp-BCh]
    tAI* v5; // [esp+18h] [ebp-B8h]
    tGameObjectMain* v6; // [esp+1Ch] [ebp-B4h]
    tGameObjectMain* v7; // [esp+20h] [ebp-B0h]
    tAI* v8; // [esp+24h] [ebp-ACh]
    tAI* v9; // [esp+28h] [ebp-A8h]
    tGameObjectMain* v10; // [esp+2Ch] [ebp-A4h]
    tGameObjectMain* v11; // [esp+30h] [ebp-A0h]
    tGameObjectMain* v12; // [esp+34h] [ebp-9Ch]
    tGameObjectMain* v13; // [esp+38h] [ebp-98h]
    tGameObjectMain* v14; // [esp+40h] [ebp-90h]
    tMessage Src; // [esp+4Ch] [ebp-84h] BYREF
    Vec3 v16; // [esp+84h] [ebp-4Ch]
    Vec3 pos; // [esp+90h] [ebp-40h] BYREF
    int v18; // [esp+9Ch] [ebp-34h]
    tGameObjectMain* v19; // [esp+A4h] [ebp-2Ch]
    int v20; // [esp+B4h] [ebp-1Ch]
    Vec3 v21; // [esp+B8h] [ebp-18h] BYREF
    BOOL v22; // [esp+C4h] [ebp-Ch]
    tScriptStruct_ActorVaisseauHyllis* v23; // [esp+C8h] [ebp-8h]

    pos.x = 1.0;
    pos.y = 0.0;
    pos.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_ActorVaisseauHyllis*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v23 = v2;
    v20 = 0;
    v19 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        vaisseau_hyllis_vh_explosion_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
    }
    if (v23->f_time_crash == 0.0f)
    {
        v23->f_time_crash = TIM_gf_MainClock - TIM_gf_LockTime;
        v22 = AI_EvalFunc_DYNOff_C(AI_gpst_CurrentGameObject);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v21);
        v16.x = 0.0;
        v16.y = 0.0;
        v16.z = -10.0;
        pos.x = v21.x + 0.0f;
        pos.y = v21.y + 0.0f;
        pos.z = v21.z + -10.0f;
        v19 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)vaisseau_hyllis_vh_explosion_SFX_explosion_gao, &pos);
        Src.msgID = 1;
        Src.Int1 = 37;
        Src.f2 = 35.0;
        Src.f1 = 0.0;
        Src.v1.x = 1.5;
        Src.v1.y = 0.0;
        Src.v1.z = 0.0;
        v14 = v19;
        if (v19)
        {
            if ((unsigned int)v19 <= 2)
                v14 = AI_MainActor[(uint32_t)v19 - 1];
            AI_EvalFunc_MSGSend_C(v14, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
    }
    if (AI_EvalFunc_TIMEElapsed_C(v23->f_time_crash, 0.30000001f))
    {
        v13 = (tGameObjectMain*)v23->GFX_Flamme;
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
            v9 = (tAI*)v23->GFX_Flamme;
            if (v9 == WOR_gst_Universe.univAIInstance)
            {
                v8 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v11 = (tGameObjectMain*)v23->GFX_Flamme;
                if (v9)
                {
                    if ((unsigned int)v9 <= 2)
                        v11 = AI_MainActor[(uint32_t)v9 - 1];
                    v10 = v11;
                }
                else
                {
                    v10 = 0;
                }
                v8 = v10->extended->AIInstance;
            }
            v20 = ((tScriptStruct_SFXExplosion*)(v8->instanceVarDataCurrent))->GFX;
            if (v20 != -1)
                AI_EvalFunc_GFXDel_C_CURRENT(v20);
            v5 = (tAI*)v23->GFX_Flamme;
            if (v5 == WOR_gst_Universe.univAIInstance)
            {
                v4 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v7 = (tGameObjectMain*)v23->GFX_Flamme;
                if (v5)
                {
                    if ((unsigned int)v5 <= 2)
                        v7 = AI_MainActor[(uint32_t)v5 - 1];
                    v6 = v7;
                }
                else
                {
                    v6 = 0;
                }
                v4 = v6->extended->AIInstance;
            }
            v20 = ((tScriptStruct_SFXExplosion*)(v4->instanceVarDataCurrent))->GFX_3;
            if (v20 != -1)
                AI_EvalFunc_GFXDel_C_CURRENT(v20);
            v3 = (tGameObjectMain*)v23->GFX_Flamme;
            if (v3)
            {
                if ((unsigned int)v3 <= 2)
                    v3 = AI_MainActor[(uint32_t)v3 - 1];
                v0 = AI_EvalFunc_OBJDestroy_C(v3);
            }
            else
            {
                v0 = AI_EvalFunc_OBJDestroy_C(0);
            }
            v18 = v0;
            if (v0 != 1000)
                return v18;
        }
        if (v23->i_idhaloL != -1)
            AI_EvalFunc_GFXDel_C_CURRENT(v23->i_idhaloL);
        if (v23->i_idhaloR != -1)
            AI_EvalFunc_GFXDel_C_CURRENT(v23->i_idhaloR);
        AI_EvalFunc_OBJReinit_C(AI_gpst_CurrentGameObject, 1);
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
    }
    return 1000;
}

int vaisseau_hyllis_vh_go_move()
{
    tGameObjectMain* v0; // eax
    tGameObjectMain* v1; // eax
    tGameObjectMain* v2; // eax
    BOOL v3; // eax
    float X; // [esp+0h] [ebp-4E4h]
    float X_4; // [esp+4h] [ebp-4E0h]
    float v7; // [esp+8h] [ebp-4DCh]
    float v8; // [esp+Ch] [ebp-4D8h]
    tScriptStruct_ActorVaisseauHyllis* v9; // [esp+10h] [ebp-4D4h]
    float v10; // [esp+14h] [ebp-4D0h]
    float v11; // [esp+20h] [ebp-4C4h]
    float v12; // [esp+2Ch] [ebp-4B8h]
    tGameObjectMain* v13; // [esp+68h] [ebp-47Ch]
    tGameObjectMain* v14; // [esp+70h] [ebp-474h]
    float v15; // [esp+7Ch] [ebp-468h]
    float v16; // [esp+80h] [ebp-464h]
    tGameObjectMain* v17; // [esp+94h] [ebp-450h]
    tGameObjectMain* v18; // [esp+9Ch] [ebp-448h]
    float v19; // [esp+A0h] [ebp-444h]
    tGameObjectMain* v20; // [esp+B4h] [ebp-430h]
    tGameObjectMain* v21; // [esp+BCh] [ebp-428h]
    tGameObjectMain* v22; // [esp+C4h] [ebp-420h]
    tGameObjectMain* v23; // [esp+D4h] [ebp-410h]
    tGameObjectMain* v24; // [esp+DCh] [ebp-408h]
    tGameObjectMain* v25; // [esp+E4h] [ebp-400h]
    tGameObjectMain* v26; // [esp+ECh] [ebp-3F8h]
    tGameObjectMain* v27; // [esp+F4h] [ebp-3F0h]
    tGameObjectMain* v28; // [esp+F8h] [ebp-3ECh]
    tGameObjectMain* v29; // [esp+FCh] [ebp-3E8h]
    tGameObjectMain* v30; // [esp+104h] [ebp-3E0h]
    tGameObjectMain* v31; // [esp+108h] [ebp-3DCh]
    tGameObjectMain* v32; // [esp+10Ch] [ebp-3D8h]
    float v33; // [esp+11Ch] [ebp-3C8h]
    Vec3 v34; // [esp+120h] [ebp-3C4h] BYREF
    Vec3 v35; // [esp+12Ch] [ebp-3B8h] BYREF
    Vec3 v36; // [esp+138h] [ebp-3ACh] BYREF
    Vec3 dest; // [esp+154h] [ebp-390h] BYREF
    Vec3 v38; // [esp+160h] [ebp-384h] BYREF
    Vec3 a1; // [esp+16Ch] [ebp-378h] BYREF
    Vec3 v40; // [esp+178h] [ebp-36Ch] BYREF
    Vec3 v41; // [esp+184h] [ebp-360h] BYREF
    float v42; // [esp+190h] [ebp-354h]
    Vec3 v43; // [esp+194h] [ebp-350h] BYREF
    Vec3 v44; // [esp+1A0h] [ebp-344h] BYREF
    Vec3 v45; // [esp+1B8h] [ebp-32Ch] BYREF
    float v46; // [esp+1C4h] [ebp-320h]
    float v47; // [esp+1C8h] [ebp-31Ch]
    float v48; // [esp+1CCh] [ebp-318h]
    float v49; // [esp+1D0h] [ebp-314h]
    tGameObjectMain* v50[102]; // [esp+1D4h] [ebp-310h] BYREF
    float v51; // [esp+36Ch] [ebp-178h]
    float v52; // [esp+370h] [ebp-174h]
    Vec3 v53; // [esp+374h] [ebp-170h] BYREF
    Vec3 v54; // [esp+380h] [ebp-164h] BYREF
    BOOL v55; // [esp+38Ch] [ebp-158h]
    Vec3 v56; // [esp+390h] [ebp-154h] BYREF
    Vec3 v57; // [esp+39Ch] [ebp-148h] BYREF
    float v58; // [esp+3ACh] [ebp-138h]
    Vec3 v59; // [esp+3B4h] [ebp-130h] BYREF
    float v60; // [esp+3C0h] [ebp-124h]
    float v61; // [esp+3C4h] [ebp-120h]
    Vec3 v62; // [esp+3D0h] [ebp-114h] BYREF
    Vec3 v63; // [esp+3DCh] [ebp-108h] BYREF
    Vec3 v64; // [esp+3E8h] [ebp-FCh] BYREF
    Vec3 v65; // [esp+3F4h] [ebp-F0h] BYREF
    Vec3 v66; // [esp+400h] [ebp-E4h] BYREF
    Vec3 v67; // [esp+40Ch] [ebp-D8h]
    Vec3 a3; // [esp+418h] [ebp-CCh] BYREF
    Vec3 v69; // [esp+428h] [ebp-BCh] BYREF
    Vec3 v70; // [esp+434h] [ebp-B0h] BYREF
    Vec3 v71; // [esp+444h] [ebp-A0h] BYREF
    Vec3 v72; // [esp+450h] [ebp-94h] BYREF
    Vec3 v73; // [esp+45Ch] [ebp-88h] BYREF
    float v74; // [esp+468h] [ebp-7Ch]
    Vec3 a4; // [esp+46Ch] [ebp-78h] BYREF
    float v76; // [esp+478h] [ebp-6Ch]
    float v77; // [esp+47Ch] [ebp-68h]
    float v78; // [esp+480h] [ebp-64h]
    float v79; // [esp+484h] [ebp-60h]
    tScriptStruct_ActorVaisseauHyllis* v80; // [esp+488h] [ebp-5Ch]
    float v81; // [esp+48Ch] [ebp-58h]
    Vec3 v82; // [esp+490h] [ebp-54h] BYREF
    Vec3 v83; // [esp+49Ch] [ebp-48h] BYREF
    tGameObjectMain* v84; // [esp+4A8h] [ebp-3Ch]
    float v85; // [esp+4B0h] [ebp-34h]
    Vec3 v86; // [esp+4B8h] [ebp-2Ch] BYREF
    Vec3 v87; // [esp+4C4h] [ebp-20h] BYREF
    int v88; // [esp+4D0h] [ebp-14h]
    float v90; // [esp+4DCh] [ebp-8h]
    float v91; // [esp+4E0h] [ebp-4h]

    v65.x = 1.0;
    v65.y = 0.0;
    v65.z = 0.0;
    v54.x = 1.0;
    v54.y = 0.0;
    v54.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    v59.x = 1.0;
    v59.y = 0.0;
    v59.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v9 = (tScriptStruct_ActorVaisseauHyllis*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v9 = 0;
    v80 = v9;
    v88 = 0;
    v48 = 0.0;
    v84 = 0;
    v91 = 0.0;
    v52 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fctvar_vaisseau_hyllis_vh_go_move_vaisseau_hyllis_vh = AI_ul_RealResolveOneRef(42, 0x3D000E47u);
    }
    v49 = TIM_gf_dt;
    v33 = 0.5f * TIM_gf_dt;
    if (v33 < 0.0f)
        v33 = 0.0f;
    if (v33 > 1.0f)
        v33 = 1.0f;
    v77 = 1.0f * v33 + v80->f_rot_blend_speed * (1.0f - v33);
    v80->f_rot_blend_speed = v77;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v69);
    MATH_SubstractVec(&dest, &v80->v_postogo, &v69);
    a1 = dest;
    v91 = MATH_VecLen(&a1);
    v32 = (tGameObjectMain*)v80->o_leader;
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
    if (!v31 && v91 < 100.0)
    {
        v80->f_rot_blend_speed = 0.0;
        v30 = (tGameObjectMain*)v80->o_next_wp;
        if (v30)
        {
            if ((unsigned int)v30 <= 2)
                v30 = AI_MainActor[(uint32_t)v30 - 1];
            v0 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v80->des_o_network, v30, 1, 0);
        }
        else
        {
            v0 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v80->des_o_network, 0, 1, 0);
        }
        v84 = v0;
        v29 = v0;
        if (v0)
        {
            if ((unsigned int)v0 <= 2)
                v29 = AI_MainActor[(uint32_t)v0 - 1];
            v28 = v29;
        }
        else
        {
            v28 = 0;
        }
        if (!v28)
        {
            v84 = AI_EvalFunc_WAYRootGet_C((tWayNetwork*)v80->des_o_network);
            v27 = v84;
            if (v84)
            {
                if ((unsigned int)v84 <= 2)
                    v27 = AI_MainActor[(uint32_t)v84 - 1];
                AI_EvalFunc_OBJGetPos_C(v27, &v80->v_postogo);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v80->v_postogo);
            }
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v80->v_postogo);
            AI_EvalFunc_COLStartMatrixSet_C(AI_gpst_CurrentGameObject, &v80->v_postogo);
            v26 = v84;
            if (v84)
            {
                if ((unsigned int)v84 <= 2)
                    v26 = AI_MainActor[(uint32_t)v84 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v26, &v70);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v70);
            }
            v25 = v84;
            if (v84)
            {
                if ((unsigned int)v84 <= 2)
                    v25 = AI_MainActor[(uint32_t)v84 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v25, &v82);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v82);
            }
            AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v70, &v82);
            v24 = v84;
            if (v84)
            {
                if ((unsigned int)v84 <= 2)
                    v24 = AI_MainActor[(uint32_t)v84 - 1];
                v1 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v80->des_o_network, v24, 1, 0);
            }
            else
            {
                v1 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v80->des_o_network, 0, 1, 0);
            }
            v84 = v1;
        }
        v23 = v84;
        if (v84)
        {
            if ((unsigned int)v84 <= 2)
                v23 = AI_MainActor[(uint32_t)v84 - 1];
            AI_EvalFunc_OBJGetPos_C(v23, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v40);
        MATH_SubstractVec(&v86, &a3, &v40);
        a1 = v86;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v62);
        v51 = v62.z * a1.z + v62.y * a1.y + a1.x * v62.x;
        if (v51 < 0.0)
        {
            v22 = (tGameObjectMain*)v80->o_next_wp;
            if (v22)
            {
                if ((unsigned int)v22 <= 2)
                    v22 = AI_MainActor[(uint32_t)v22 - 1];
                v2 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v80->des_o_network, v22, 3, 0);
            }
            else
            {
                v2 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v80->des_o_network, 0, 3, 0);
            }
            v84 = v2;
        }
        v80->o_next_wp = (int)v84;
        v21 = (tGameObjectMain*)v80->o_next_wp;
        if (v21)
        {
            if ((unsigned int)v21 <= 2)
                v21 = AI_MainActor[(uint32_t)v21 - 1];
            AI_EvalFunc_BVRandomPosGet_C(v21, 0, &v80->v_postogo);
        }
        else
        {
            AI_EvalFunc_BVRandomPosGet_C(0, 0, &v80->v_postogo);
        }
        v20 = (tGameObjectMain*)v80->o_next_wp;
        if (v20)
        {
            if ((unsigned int)v20 <= 2)
                v20 = AI_MainActor[(uint32_t)v20 - 1];
            AI_EvalFunc_OBJGetPos_C(v20, &v57);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v57);
        }
        v8 = (float)v80->des_i_id;
        v80->v_postogo.z = v80->f_rank_z * v8 + v57.z;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v72);
        MATH_SubstractVec(&v63, &v80->v_postogo, &v72);
        a1 = v63;
        v91 = MATH_VecLen(&a1);
    }
    v19 = 1.0f / v91;
    a1.x = a1.x * v19;
    a1.y = a1.y * v19;
    a1.z = a1.z * v19;
    v88 = AI_EvalFunc_COLBVGetAllOverlaps_C(AI_gpst_CurrentGameObject, v50, 32, 0, 0);
    while (v88)
    {
        v18 = v50[--v88];
        if (v18)
        {
            if ((unsigned int)v18 <= 2)
                v18 = AI_MainActor[(uint32_t)v18 - 1];
            v3 = AI_EvalFunc_AIIsModel_C(v18, (tAIModel*)fctvar_vaisseau_hyllis_vh_go_move_vaisseau_hyllis_vh);
        }
        else
        {
            v3 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_vaisseau_hyllis_vh_go_move_vaisseau_hyllis_vh);
        }
        v55 = v3;
        if (v3)
        {
            v17 = v50[v88];
            if (v17)
            {
                if ((unsigned int)v17 <= 2)
                    v17 = AI_MainActor[(uint32_t)v17 - 1];
                AI_EvalFunc_OBJGetPos_C(v17, &v53);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v53);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v73);
            MATH_SubstractVec(&v38, &v53, &v73);
            v65 = v38;
            v91 = MATH_VecLen(&v65);
            if (v91 != 0.0f && v91 < 200.0f)
            {
                v16 = 1.0f / v91;
                v65.x = v65.x * v16;
                v65.y = v65.y * v16;
                v65.z = v65.z * v16;
                v48 = v65.z * a1.z + v65.y * a1.y + a1.x * v65.x;
                if (v48 > 0.0)
                {
                    v81 = TIM_gf_dt;
                    v15 = 2.0f * TIM_gf_dt;
                    if (v15 < 0.0f)
                        v15 = 0.0f;
                    if (v15 > 1.0f)
                        v15 = 1.0f;
                    v61 = 2.0f * v15 + v80->f_rot_blend_speed * (1.0f - v15);
                    v80->f_rot_blend_speed = v61;
                    a1 = v80->v_postogo;
                    if (v65.z <= 0.0)
                    {
                        v13 = v50[v88];
                        if (v13)
                        {
                            if ((unsigned int)v13 <= 2)
                                v13 = AI_MainActor[(uint32_t)v13 - 1];
                            AI_EvalFunc_OBJGetPos_C(v13, &v83);
                        }
                        else
                        {
                            AI_EvalFunc_OBJGetPos_C(0, &v83);
                        }
                        a1.z = v83.z + 200.0f + a1.z;
                    }
                    else
                    {
                        v14 = v50[v88];
                        if (v14)
                        {
                            if ((unsigned int)v14 <= 2)
                                v14 = AI_MainActor[(uint32_t)v14 - 1];
                            AI_EvalFunc_OBJGetPos_C(v14, &v64);
                        }
                        else
                        {
                            AI_EvalFunc_OBJGetPos_C(0, &v64);
                        }
                        a1.z = v64.z - 200.0f;
                    }
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v44);
                    a1.x = a1.x - v44.x;
                    a1.y = a1.y - v44.y;
                    a1.z = a1.z - v44.z;
                    MATH_NormalizeEqualVector(&a1);
                }
            }
        }
    }
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v87);
    v59 = v87;
    AI_EvalFunc_MATHVecSetHorzNormalize_C(&v59);
    v71.x = 0.0;
    v71.y = 0.0;
    v71.z = 1.0;
    AI_EvalFunc_MATHVecCrossProduct_C(&v71, &v59, &v36);
    v59 = v36;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v45);
    v79 = a1.z * v45.z + a1.y * v45.y + v45.x * a1.x;
    if (v79 < 0.0)
    {
        v85 = v59.z * a1.z + v59.y * a1.y + a1.x * v59.x;
        if (v85 <= 0.0)
        {
            v67.x = -v59.x;
            v67.y = -v59.y;
            v67.z = -v59.z;
            a1.x = -v59.x;
            a1.y = -v59.y;
            a1.z = -v59.z;
        }
        else
        {
            a1 = v59;
        }
    }
    v48 = v59.z * a1.z + v59.y * a1.y + a1.x * v59.x;
    v74 = TIM_gf_dt;
    v12 = TIM_gf_dt;
    if (TIM_gf_dt < 0.0f)
        v12 = 0.0f;
    if (v12 > 1.0f)
        v12 = 1.0f;
    v46 = v48 * 0.5f * v12 + v80->f_banking_coef * (1.0f - v12);
    v80->f_banking_coef = v46;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v56);
    v90 = TIM_gf_dt;
    X = v80->f_rot_blend_speed * TIM_gf_dt;
    AI_EvalFunc_MATHVecBlend_C(&v56, &a1, X, &a4);
    a1 = a4;
    v52 = v80->f_banking_coef;
    v42 = TIM_gf_dt;
    v11 = TIM_gf_dt;
    if (TIM_gf_dt < 0.0)
        v11 = 0.0;
    if (v11 > 1.0)
        v11 = 1.0;
    v78 = 0.0f * v11 + v80->f_blend_paf * (1.0f - v11);
    v80->f_blend_paf = v78;
    if (!AI_EvalFunc_MATHFloatNullEps_C(v80->f_blend_paf))
    {
        v47 = TIM_gf_MainClock - TIM_gf_LockTime;
        X_4 = v47 * 10.0f;
        v60 = sinf(X_4);
        v52 = v80->f_blend_paf * 0.25f * v60 + v52;
    }
    v43.x = 0.0;
    v43.y = 0.0;
    v43.z = 1.0;
    v58 = AI_EvalFunc_MTHFloatSign_C(v52);
    v34.x = v59.x * v58;
    v34.y = v59.y * v58;
    v34.z = v59.z * v58;
    v7 = (float)fabs(v52);
    v76 = v7;
    AI_EvalFunc_MATHVecBlend_C(&v43, &v34, v7, &v35);
    v54 = v35;
    AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a1, &v54);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v66);
    v10 = v80->f_speed;
    v41.x = v66.x * v10;
    v41.y = v66.y * v10;
    v41.z = v66.z * v10;
    AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v41);
    return 1000;
}

int vaisseau_hyllis_vh_reflexe()
{
    double v0; // st7
    int v1; // eax
    float X_4; // [esp+4h] [ebp-418h]
    float X_4a; // [esp+4h] [ebp-418h]
    float X_4b; // [esp+4h] [ebp-418h]
    float X_4c; // [esp+4h] [ebp-418h]
    float X_4d; // [esp+4h] [ebp-418h]
    float v8; // [esp+8h] [ebp-414h]
    float v9; // [esp+Ch] [ebp-410h]
    tScriptStruct_ActorVaisseauHyllis* v10; // [esp+10h] [ebp-40Ch]
    tAI* v11; // [esp+1Ch] [ebp-400h]
    tAI* v12; // [esp+20h] [ebp-3FCh]
    tGameObjectMain* v13; // [esp+24h] [ebp-3F8h]
    tGameObjectMain* v14; // [esp+28h] [ebp-3F4h]
    tAI* v15; // [esp+34h] [ebp-3E8h]
    tAI* v16; // [esp+38h] [ebp-3E4h]
    tGameObjectMain* v17; // [esp+3Ch] [ebp-3E0h]
    tGameObjectMain* v18; // [esp+40h] [ebp-3DCh]
    tGameObjectMain* v19; // [esp+48h] [ebp-3D4h]
    tGameObjectMain* v20; // [esp+4Ch] [ebp-3D0h]
    tGameObjectMain* v21; // [esp+50h] [ebp-3CCh]
    float v22; // [esp+54h] [ebp-3C8h]
    tGameObjectMain* v23; // [esp+5Ch] [ebp-3C0h]
    tAI* v24; // [esp+60h] [ebp-3BCh]
    tGameObjectMain* v25; // [esp+64h] [ebp-3B8h]
    tGameObjectMain* v26; // [esp+68h] [ebp-3B4h]
    tGameObjectMain* v27; // [esp+70h] [ebp-3ACh]
    tGameObjectMain* v28; // [esp+74h] [ebp-3A8h]
    tGameObjectMain* v29; // [esp+78h] [ebp-3A4h]
    tGameObjectMain* v30; // [esp+80h] [ebp-39Ch]
    tGameObjectMain* v31; // [esp+84h] [ebp-398h]
    tGameObjectMain* v32; // [esp+88h] [ebp-394h]
    tGameObjectMain* v33; // [esp+90h] [ebp-38Ch]
    tGameObjectMain* v34; // [esp+98h] [ebp-384h]
    tGameObjectMain* v35; // [esp+A4h] [ebp-378h]
    tGameObjectMain* v36; // [esp+A8h] [ebp-374h]
    tGameObjectMain* v37; // [esp+B0h] [ebp-36Ch]
    tGameObjectMain* v38; // [esp+B8h] [ebp-364h]
    tGameObjectMain* v39; // [esp+C0h] [ebp-35Ch]
    tGameObjectMain* v40; // [esp+C4h] [ebp-358h]
    tGameObjectMain* v41; // [esp+C8h] [ebp-354h]
    tGameObjectMain* v42; // [esp+D0h] [ebp-34Ch]
    tAI* v43; // [esp+D4h] [ebp-348h]
    tAI* v44; // [esp+D8h] [ebp-344h]
    tGameObjectMain* v45; // [esp+DCh] [ebp-340h]
    tGameObjectMain* v46; // [esp+E0h] [ebp-33Ch]
    tAI* v47; // [esp+E4h] [ebp-338h]
    tAI* v48; // [esp+E8h] [ebp-334h]
    tGameObjectMain* v49; // [esp+ECh] [ebp-330h]
    tGameObjectMain* v50; // [esp+F0h] [ebp-32Ch]
    tGameObjectMain* v51; // [esp+F4h] [ebp-328h]
    tGameObjectMain* v52; // [esp+F8h] [ebp-324h]
    double v53; // [esp+FCh] [ebp-320h]
    float v54; // [esp+104h] [ebp-318h]
    tGameObjectMain* v55; // [esp+10Ch] [ebp-310h]
    tGameObjectMain* v56; // [esp+114h] [ebp-308h]
    tGameObjectMain* v57; // [esp+11Ch] [ebp-300h]
    tGameObjectMain* v58; // [esp+120h] [ebp-2FCh]
    tGameObjectMain* v59; // [esp+124h] [ebp-2F8h]
    tGameObjectMain* v60; // [esp+128h] [ebp-2F4h]
    tGameObjectMain* v61; // [esp+12Ch] [ebp-2F0h]
    float v62; // [esp+138h] [ebp-2E4h]
    tGameObjectMain* v63; // [esp+140h] [ebp-2DCh]
    tGameObjectMain* v64; // [esp+148h] [ebp-2D4h]
    tGameObjectMain* v65; // [esp+150h] [ebp-2CCh]
    tGameObjectMain* v66; // [esp+154h] [ebp-2C8h]
    tGameObjectMain* v67; // [esp+158h] [ebp-2C4h]
    tGameObjectMain* v68; // [esp+15Ch] [ebp-2C0h]
    tGameObjectMain* v69; // [esp+160h] [ebp-2BCh]
    float v70; // [esp+164h] [ebp-2B8h]
    float v71; // [esp+168h] [ebp-2B4h]
    float v72; // [esp+16Ch] [ebp-2B0h]
    Vec3 v73; // [esp+170h] [ebp-2ACh] BYREF
    float v74; // [esp+17Ch] [ebp-2A0h]
    Vec3 v76; // [esp+184h] [ebp-298h] BYREF
    tMessage v78; // [esp+198h] [ebp-284h] BYREF
    tGameObjectMain* v79; // [esp+1D8h] [ebp-244h]
    float v80; // [esp+1DCh] [ebp-240h]
    Vec3 v81; // [esp+1E0h] [ebp-23Ch] BYREF
    Vec3 v82; // [esp+1ECh] [ebp-230h] BYREF
    tGameObjectMain* v84; // [esp+1FCh] [ebp-220h]
    tGameObjectMain* v85; // [esp+200h] [ebp-21Ch]
    Vec3 v86; // [esp+208h] [ebp-214h] BYREF
    float v87; // [esp+214h] [ebp-208h]
    float v88; // [esp+218h] [ebp-204h]
    float v89; // [esp+21Ch] [ebp-200h]
    Vec3 v90; // [esp+220h] [ebp-1FCh] BYREF
    float v92; // [esp+234h] [ebp-1E8h]
    Vec3 v93; // [esp+238h] [ebp-1E4h] BYREF
    Vec3 v95; // [esp+25Ch] [ebp-1C0h] BYREF
    int v96; // [esp+268h] [ebp-1B4h]
    int v97; // [esp+26Ch] [ebp-1B0h]
    int v98; // [esp+270h] [ebp-1ACh]
    void* Src; // [esp+274h] [ebp-1A8h]
    tMessage v100; // [esp+280h] [ebp-19Ch] BYREF
    Vec3 v101; // [esp+2B4h] [ebp-168h] BYREF
    Vec3 v102; // [esp+2C0h] [ebp-15Ch] BYREF
    Vec3 v103; // [esp+2D4h] [ebp-148h] BYREF
    Vec3 v104; // [esp+2E0h] [ebp-13Ch] BYREF
    Vec3 v105; // [esp+2F8h] [ebp-124h]
    float v106; // [esp+308h] [ebp-114h]
    int v107; // [esp+30Ch] [ebp-110h]
    Vec3 v108; // [esp+310h] [ebp-10Ch] BYREF
    float v109; // [esp+31Ch] [ebp-100h]
    float v110; // [esp+320h] [ebp-FCh]
    int v111; // [esp+324h] [ebp-F8h]
    Vec3 pos; // [esp+328h] [ebp-F4h] BYREF
    int v113; // [esp+350h] [ebp-CCh]
    int v115; // [esp+358h] [ebp-C4h]
    Vec3 v116; // [esp+35Ch] [ebp-C0h] BYREF
    Vec3 v117; // [esp+368h] [ebp-B4h] BYREF
    Vec3 v118; // [esp+374h] [ebp-A8h] BYREF
    float v119; // [esp+380h] [ebp-9Ch]
    tMessage v120; // [esp+388h] [ebp-94h] BYREF
    Vec3 v121; // [esp+3BCh] [ebp-60h]
    tScriptStruct_ActorVaisseauHyllis* v122; // [esp+3C8h] [ebp-54h]
    Vec3 a1; // [esp+3D8h] [ebp-44h] BYREF
    Vec3 v124; // [esp+3E4h] [ebp-38h] BYREF
    Vec3 a3; // [esp+3F0h] [ebp-2Ch] BYREF
    void* v126; // [esp+3FCh] [ebp-20h]
    Vec3 v127; // [esp+400h] [ebp-1Ch] BYREF
    float v128; // [esp+414h] [ebp-8h]

    pos.x = 1.0;
    pos.y = 0.0;
    pos.z = 0.0;
    v73.x = 1.0;
    v73.y = 0.0;
    v73.z = 0.0;
    v82.x = 1.0;
    v82.y = 0.0;
    v82.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v10 = (tScriptStruct_ActorVaisseauHyllis*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v10 = 0;
    v122 = v10;
    v74 = 0.0;
    v79 = 0;
    v113 = 0;
    v84 = 0;
    v85 = 0;
    v115 = 0;
    v107 = 0;
    v110 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        vaisseau_hyllis_vh_reflexe_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        vaisseau_hyllis_vh_reflexe_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &vaisseau_hyllis_vh_reflexe_univVars_Planetes_CurHour,
            (void*)vaisseau_hyllis_vh_reflexe_univVars,
            6116);
        vaisseau_hyllis_vh_reflexe_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
    }
    v79 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0);
    v69 = v79;
    if (v79)
    {
        if ((unsigned int)v79 <= 2)
            v69 = AI_MainActor[(uint32_t)v79 - 1];
        v68 = v69;
    }
    else
    {
        v68 = 0;
    }
    if (v68)
    {
        v119 = TIM_gf_dt;
        v67 = v79;
        if (v79)
        {
            if ((unsigned int)v79 <= 2)
                v67 = AI_MainActor[(uint32_t)v79 - 1];
            v66 = v67;
        }
        else
        {
            v66 = 0;
        }
        X_4 = v122->f_speed_pal * v119;
        AI_EvalFunc_OBJRotateLocalY_C(v66, X_4);
    }
    v109 = TIM_gf_MainClock - TIM_gf_LockTime;
    v0 = v122->f_alea_halo + v109;
    X_4a = (float)(v0 + v0);
    v80 = sinf(X_4a);
    v9 = (float)fabs(v80 * 40.0f);
    v110 = v9;
    if (v122->i_idhaloL == -1)
    {
        v122->i_idhaloL = AI_EvalFunc_GFXAdd_C_CURRENT(21);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v122->i_idhaloL, 4, 1);
        v65 = (tGameObjectMain*)vaisseau_hyllis_vh_reflexe_SFX_light_smoke_gao;
        if (vaisseau_hyllis_vh_reflexe_SFX_light_smoke_gao)
        {
            if ((unsigned int)vaisseau_hyllis_vh_reflexe_SFX_light_smoke_gao <= 2)
                v65 = AI_MainActor[vaisseau_hyllis_vh_reflexe_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v122->i_idhaloL, v65, 0);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v122->i_idhaloL, 0, 0);
        }
        GFXSetCurGlobal(v122->i_idhaloL);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 15.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 10.0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v122->des_c_colorhalo);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
    }
    v64 = v79;
    if (v79)
    {
        if ((unsigned int)v79 <= 2)
            v64 = AI_MainActor[(uint32_t)v79 - 1];
        AI_EvalFunc_OBJGetPos_C(v64, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    v63 = v79;
    if (v79)
    {
        if ((unsigned int)v79 <= 2)
            v63 = AI_MainActor[(uint32_t)v79 - 1];
        AI_EvalFunc_OBJ_SightGet_C(v63, &v86);
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C(0, &v86);
    }
    v62 = v122->des_f_dechalo;
    v108.x = v86.x * v62;
    v108.y = v86.y * v62;
    v108.z = v86.z * v62;
    MATH_VecAddVec(&a1, &a3, &v108);
    v73 = a1;
    AI_EvalFunc_GFXSetv_C_CURRENT(v122->i_idhaloL, 21200, &v73);
    v79 = AI_EvalFunc_ANICanalGaoGet_C_CURRENT(1u);
    v61 = v79;
    if (v79)
    {
        if ((unsigned int)v79 <= 2)
            v61 = AI_MainActor[(uint32_t)v79 - 1];
        v60 = v61;
    }
    else
    {
        v60 = 0;
    }
    if (v60)
    {
        v106 = TIM_gf_dt;
        v59 = v79;
        if (v79)
        {
            if ((unsigned int)v79 <= 2)
                v59 = AI_MainActor[(uint32_t)v79 - 1];
            v58 = v59;
        }
        else
        {
            v58 = 0;
        }
        X_4b = v122->f_speed_pal * v106;
        AI_EvalFunc_OBJRotateLocalY_C(v58, X_4b);
    }
    if (v122->i_idhaloR == -1)
    {
        v122->i_idhaloR = AI_EvalFunc_GFXAdd_C_CURRENT(21);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v122->i_idhaloR, 4, 1);
        v57 = (tGameObjectMain*)vaisseau_hyllis_vh_reflexe_SFX_light_smoke_gao;
        if (vaisseau_hyllis_vh_reflexe_SFX_light_smoke_gao)
        {
            if ((unsigned int)vaisseau_hyllis_vh_reflexe_SFX_light_smoke_gao <= 2)
                v57 = AI_MainActor[vaisseau_hyllis_vh_reflexe_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v122->i_idhaloR, v57, 0);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v122->i_idhaloR, 0, 0);
        }
        GFXSetCurGlobal(v122->i_idhaloR);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 15.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 10.0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v122->des_c_colorhalo);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
    }
    v56 = v79;
    if (v79)
    {
        if ((unsigned int)v79 <= 2)
            v56 = AI_MainActor[(uint32_t)v79 - 1];
        AI_EvalFunc_OBJGetPos_C(v56, &v127);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v127);
    }
    v55 = v79;
    if (v79)
    {
        if ((unsigned int)v79 <= 2)
            v55 = AI_MainActor[(uint32_t)v79 - 1];
        AI_EvalFunc_OBJ_SightGet_C(v55, &v104);
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C(0, &v104);
    }
    v54 = v122->des_f_dechalo;
    v93.x = v104.x * v54;
    v93.y = v104.y * v54;
    v93.z = v104.z * v54;
    MATH_VecAddVec(&v95, &v127, &v93);
    AI_EvalFunc_GFXSetv_C_CURRENT(v122->i_idhaloR, 21200, &v95);
    v53 = *(float*)vaisseau_hyllis_vh_reflexe_univVars_Planetes_CurHour + MATH_gd_magicDoubleForRound;
    v107 = (*(uint32_t *)&v53);
    if ((*(int32_t*)&v53) <= 17 && v107 >= 6)
    {
        v122->i_ok_halo = 0;
        AI_EvalFunc_GFXSetf_C_CURRENT(v122->i_idhaloL, 21000, 0.0);
        AI_EvalFunc_GFXSetf_C_CURRENT(v122->i_idhaloR, 21000, 0.0);
    }
    else
    {
        if (v122->i_ok_halo)
        {
            AI_EvalFunc_GFXSetf_C_CURRENT(v122->i_idhaloL, 21000, v110);
            AI_EvalFunc_GFXSetf_C_CURRENT(v122->i_idhaloR, 21000, v110);
        }
        if (!AI_EvalFunc_MATHRandInt_C(0, 4))
            v122->i_ok_halo = 1;
    }
    Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v78, Src, sizeof(v78));
    while (v78.msgID)
    {
        v84 = v78.gao;
        if (v78.msgID == 37)
        {
            v115 = v78.Int1 & 7;
            if ((v78.Int1 & 7) != 0)
            {
                if (AI_EvalFunc_TIMEElapsed_C(v122->f_time_smoke, 4.0))
                {
                    v122->f_blend_paf = 1.0;
                    v122->f_time_smoke = TIM_gf_MainClock - TIM_gf_LockTime;
                    v52 = (tGameObjectMain*)v122->GFX_Flamme;
                    if (v52)
                    {
                        if ((unsigned int)v52 <= 2)
                            v52 = AI_MainActor[(uint32_t)v52 - 1];
                        v51 = v52;
                    }
                    else
                    {
                        v51 = 0;
                    }
                    if (v51)
                    {
                        v48 = (tAI*)v122->GFX_Flamme;
                        if (v48 == WOR_gst_Universe.univAIInstance)
                        {
                            v47 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v50 = (tGameObjectMain*)v122->GFX_Flamme;
                            if (v48)
                            {
                                if ((unsigned int)v48 <= 2)
                                    v50 = AI_MainActor[(uint32_t)v48 - 1];
                                v49 = v50;
                            }
                            else
                            {
                                v49 = 0;
                            }
                            v47 = v49->extended->AIInstance;
                        }
                        ((tScriptStruct_SFXExplosion*)(v47->instanceVarDataCurrent))->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
                        v44 = (tAI*)v122->GFX_Flamme;
                        if (v44 == WOR_gst_Universe.univAIInstance)
                        {
                            v43 = WOR_gst_Universe.univAIInstance;
                        }
                        else
                        {
                            v46 = (tGameObjectMain*)v122->GFX_Flamme;
                            if (v44)
                            {
                                if ((unsigned int)v44 <= 2)
                                    v46 = AI_MainActor[(uint32_t)v44 - 1];
                                v45 = v46;
                            }
                            else
                            {
                                v45 = 0;
                            }
                            v43 = v45->extended->AIInstance;
                        }
                        v122->f_time_gen = ((tScriptStruct_SFXExplosion*)(v43->instanceVarDataCurrent))->f_time_start;
                    }
                    v42 = v84;
                    if (v84)
                    {
                        if ((unsigned int)v84 <= 2)
                            v42 = AI_MainActor[(uint32_t)v84 - 1];
                        v1 = AI_EvalFunc_OBJ_FlagsDesignGet_C(v42);
                    }
                    else
                    {
                        v1 = AI_EvalFunc_OBJ_FlagsDesignGet_C(0);
                    }
                    v111 = v1;
                    if ((v1 & 1) != 0)
                    {
                        if (!SND_i_IsTrackPlaying(1040))
                        {
                            if (v122->i_nb_paf <= 3)
                            {
                                v41 = (tGameObjectMain*)v122->des_o_text;
                                if (v41)
                                {
                                    if ((unsigned int)v41 <= 2)
                                        v41 = AI_MainActor[(uint32_t)v41 - 1];
                                    v40 = v41;
                                }
                                else
                                {
                                    v40 = 0;
                                }
                                if (v40)
                                {
                                    if (v122->i_nb_paf < 3)
                                    {
                                        v100.msgID = 30;
                                        v100.Int1 = v122->i_cur_replique;
                                        v100.Int2 = 4502;
                                        v100.f2 = 10.0;
                                        v100.v1.x = 0.0;
                                        v100.v1.y = 0.0;
                                        v100.v1.z = 0.0;
                                        v100.v2.x = 0.0;
                                        v100.v2.y = 0.0;
                                        v100.v2.z = 0.0;
                                        v100.gao = 0;
                                        v100.gao2 = 0;
                                        v37 = (tGameObjectMain*)v122->des_o_text;
                                        if (v37)
                                        {
                                            if ((unsigned int)v37 <= 2)
                                                v37 = AI_MainActor[(uint32_t)v37 - 1];
                                            AI_EvalFunc_MSGSend_C(v37, &v100);
                                        }
                                        else
                                        {
                                            AI_EvalFunc_MSGSend_C(0, &v100);
                                        }
                                        if (++v122->i_cur_replique > v122->des_i_replique + 3)
                                            v122->i_cur_replique = v122->des_i_replique + 1;
                                    }
                                    else
                                    {
                                        v39 = (tGameObjectMain*)v122->des_o_flic;
                                        if (v39)
                                        {
                                            if ((unsigned int)v39 <= 2)
                                                v39 = AI_MainActor[(uint32_t)v39 - 1];
                                            AI_EvalFunc_OBJ_CapaSet_C(v39, 256, 0);
                                        }
                                        else
                                        {
                                            AI_EvalFunc_OBJ_CapaSet_C(0, 256, 0);
                                        }
                                        v100.msgID = 30;
                                        v100.Int1 = v122->des_i_replique;
                                        v100.Int2 = 4502;
                                        v100.f2 = 10.0;
                                        v100.v1.x = 0.0;
                                        v100.v1.y = 0.0;
                                        v100.v1.z = 0.0;
                                        v100.v2.x = 0.0;
                                        v100.v2.y = 0.0;
                                        v100.v2.z = 0.0;
                                        v100.gao = 0;
                                        v100.gao2 = 0;
                                        v38 = (tGameObjectMain*)v122->des_o_text;
                                        if (v38)
                                        {
                                            if ((unsigned int)v38 <= 2)
                                                v38 = AI_MainActor[(uint32_t)v38 - 1];
                                            AI_EvalFunc_MSGSend_C(v38, &v100);
                                        }
                                        else
                                        {
                                            AI_EvalFunc_MSGSend_C(0, &v100);
                                        }
                                    }
                                }
                            }
                            ++v122->i_nb_paf;
                        }
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(2048, 0);
                        v122->i_nb_paf = 4;
                        v122->f_rot_blend_speed = 0.0;
                        v122->i_flag_gameover = 1;
                        v36 = (tGameObjectMain*)v122->o_gameover_explosion;
                        if (v36)
                        {
                            if ((unsigned int)v36 <= 2)
                                v36 = AI_MainActor[(uint32_t)v36 - 1];
                            v35 = v36;
                        }
                        else
                        {
                            v35 = 0;
                        }
                        if (!v35)
                        {
                            AI_EvalFunc_OBJGetPos_C_CURRENT(&v101);
                            v121.x = 0.0;
                            v121.y = 0.0;
                            v121.z = 8.0;
                            pos.x = v101.x + 0.0f;
                            pos.y = v101.y + 0.0f;
                            pos.z = v101.z + 8.0f;
                            v85 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)vaisseau_hyllis_vh_reflexe_SFX_explosion_gao, &pos);
                            v120.msgID = 1;
                            v120.Int1 = 36;
                            v120.f2 = 10.0;
                            v120.f1 = 0.0;
                            v120.v1.x = 2.0;
                            v120.v1.y = 0.0;
                            v120.v1.z = 0.0;
                            v34 = v85;
                            if (v85)
                            {
                                if ((unsigned int)v85 <= 2)
                                    v34 = AI_MainActor[(uint32_t)v85 - 1];
                                AI_EvalFunc_MSGSend_C(v34, &v120);
                            }
                            else
                            {
                                AI_EvalFunc_MSGSend_C(0, &v120);
                            }
                            v122->o_gameover_explosion = (int)v85;
                            v33 = (tGameObjectMain*)v122->o_gameover_explosion;
                            if (v33)
                            {
                                if ((unsigned int)v33 <= 2)
                                    v33 = AI_MainActor[(uint32_t)v33 - 1];
                                AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, v33, &v122->o_gameover_explosion);
                            }
                            else
                            {
                                AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, 0, &v122->o_gameover_explosion);
                            }
                        }
                    }
                }
            }
        }
        v126 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v78, v126, sizeof(v78));
    }
    if (AI_EvalFunc_TIMEElapsed_C(v122->f_time_smoke, 20.0))
    {
        v122->i_nb_paf = 0;
        v122->i_cur_replique = v122->des_i_replique + 1;
        AI_EvalFunc_GFXSetf_C_CURRENT(v122->GFX_Smoke, 13008, 10000.0);
    }
    if (AI_EvalFunc_TIMEElapsed_C(v122->f_time_smoke, 22.0))
    {
        v32 = (tGameObjectMain*)v122->GFX_Flamme;
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
        if (!v31)
        {
            v122->GFX_Smoke = -1;
            AI_EvalFunc_GFXDel_C_CURRENT(v122->GFX_Smoke);
        }
    }
    if (v122->i_nb_paf == 2 && v122->GFX_Smoke == -1)
    {
        v122->GFX_Smoke = AI_EvalFunc_GFXAdd_C_CURRENT(13);
        GFXSetCurGlobal(v122->GFX_Smoke);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 200);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13106, 0x4876E800);
        v30 = (tGameObjectMain*)vaisseau_hyllis_vh_reflexe_SFX_light_smoke_gao;
        if (vaisseau_hyllis_vh_reflexe_SFX_light_smoke_gao)
        {
            if ((unsigned int)vaisseau_hyllis_vh_reflexe_SFX_light_smoke_gao <= 2)
                v30 = AI_MainActor[vaisseau_hyllis_vh_reflexe_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v122->GFX_Smoke, v30, -1);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v122->GFX_Smoke, 0, -1);
        }
        GFXSetCurGlobal(v122->GFX_Smoke);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13101, 8);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13003, 1.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13004, 2.0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13103, 0xFFFFFFF);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13104, 0x4FFFFFFF);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13105, 0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(13107, 0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13000, 20.0f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13001, 25.0f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13002, 0.0099999998f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.1f);
        v76.x = -1.0;
        v76.y = -1.0;
        v76.z = 0.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v76);
        v90.x = 1.0;
        v90.y = 1.0;
        v90.z = 0.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v90);
        v117.x = 0.1f;
        v117.y = 0.1f;
        v117.z = 0.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v117);
        v124.x = 1.0;
        v124.y = 2.0;
        v124.z = 3.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13204, &v124);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v122->GFX_Smoke, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v122->GFX_Smoke, 2, 1);
        AI_EvalFunc_GFXSetf_C_CURRENT(v122->GFX_Smoke, 13012, 0.0);
    }
    if (v122->i_nb_paf == 4)
    {
        v29 = (tGameObjectMain*)v122->GFX_Flamme;
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
        if (!v28)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v103);
            v85 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)vaisseau_hyllis_vh_reflexe_SFX_explosion_gao, &v103);
            v120.msgID = 1;
            v120.Int1 = 30;
            v120.f2 = 25.0;
            v120.f1 = 0.0;
            v120.v1.x = 5.0;
            v120.v1.y = 0.0;
            v120.v1.z = 0.0;
            v27 = v85;
            if (v85)
            {
                if ((unsigned int)v85 <= 2)
                    v27 = AI_MainActor[(uint32_t)v85 - 1];
                AI_EvalFunc_MSGSend_C(v27, &v120);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &v120);
            }
            if (v85 == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
            {
                v24 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v26 = v85;
                if (v85)
                {
                    if ((unsigned int)v85 <= 2)
                        v26 = AI_MainActor[(uint32_t)v85 - 1];
                    v25 = v26;
                }
                else
                {
                    v25 = 0;
                }
                v24 = v25->extended->AIInstance;
            }
            ((tScriptStruct_SFXExplosion*)(v24->instanceVarDataCurrent))->f_speed_coef = 20.0;
            v122->GFX_Flamme = (int)v85;
            v23 = (tGameObjectMain*)v122->GFX_Flamme;
            if (v23)
            {
                if ((unsigned int)v23 <= 2)
                    v23 = AI_MainActor[(uint32_t)v23 - 1];
                AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, v23, &v122->GFX_Flamme);
            }
            else
            {
                AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, 0, &v122->GFX_Flamme);
            }
            v122->f_time_gen = TIM_gf_MainClock - TIM_gf_LockTime;
            v122->i_nb_paf = 0;
        }
    }
    v74 = 1.0;
    if (v122->GFX_Smoke != -1)
    {
        v82.x = AI_EvalFunc_MATHRandFloat_C(-4.0, 4.0);
        v82.y = AI_EvalFunc_MATHRandFloat_C(-4.0, 4.0);
        v82.z = AI_EvalFunc_MATHRandFloat_C(-4.0, 4.0);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v81);
        MATH_VecAddVec(&v116, &v81, &v82);
        AI_EvalFunc_GFXSetv_C_CURRENT(v122->GFX_Smoke, 13200, &v116);
        v128 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
        v22 = v128 / 300.0f;
        if (v22 > 1.0)
            v8 = 1.0;
        else
            v8 = v128 / 300.0f;
        v92 = v8;
        v74 = v8 * 4.0f + 1.0f;
    }
    v21 = (tGameObjectMain*)v122->GFX_Flamme;
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
    if (v20)
    {
        v19 = (tGameObjectMain*)v122->GFX_Flamme;
        if (v19)
        {
            if ((unsigned int)v19 <= 2)
                v19 = AI_MainActor[(uint32_t)v19 - 1];
            AI_EvalFunc_OBJSetPos_C(v19, &v73);
        }
        else
        {
            AI_EvalFunc_OBJSetPos_C(0, &v73);
        }
        v16 = (tAI*)v122->GFX_Flamme;
        if (v16 == WOR_gst_Universe.univAIInstance)
        {
            v15 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v18 = (tGameObjectMain*)v122->GFX_Flamme;
            if (v16)
            {
                if ((unsigned int)v16 <= 2)
                    v18 = AI_MainActor[(uint32_t)v16 - 1];
                v17 = v18;
            }
            else
            {
                v17 = 0;
            }
            v15 = v17->extended->AIInstance;
        }
        v113 = ((tScriptStruct_SFXExplosion*)(v15->instanceVarDataCurrent))->GFX;
        if (v113 != -1)
        {
            AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v102);
            v87 = v102.x * 0.5f;
            v88 = v102.y * 0.5f;
            v89 = v102.z * 0.5f;
            v96 = 0;
            v97 = 0;
            v98 = -1054867456;
            v82.x = v87 + 0.0f;
            v82.y = v88 + 0.0f;
            v82.z = v89 + -10.0f;
            AI_EvalFunc_GFXSetv_C_CURRENT(v113, 13204, &v82);
            if (!AI_EvalFunc_TIMEElapsed_C(v122->f_time_gen, 20.0))
            {
                X_4c = 0.0099999998f / v74;
                AI_EvalFunc_GFXSetf_C_CURRENT(v113, 13008, X_4c);
            }
            AI_EvalFunc_GFXSetv_C_CURRENT(v113, 13200, &v73);
        }
        v12 = (tAI*)v122->GFX_Flamme;
        if (v12 == WOR_gst_Universe.univAIInstance)
        {
            v11 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v14 = (tGameObjectMain*)v122->GFX_Flamme;
            if (v12)
            {
                if ((unsigned int)v12 <= 2)
                    v14 = AI_MainActor[(uint32_t)v12 - 1];
                v13 = v14;
            }
            else
            {
                v13 = 0;
            }
            v11 = v13->extended->AIInstance;
        }
        v113 = ((tScriptStruct_SFXExplosion*)(v11->instanceVarDataCurrent))->GFX_3;
        if (v113 != -1)
        {
            AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v118);
            v70 = v118.x * 0.34999999f;
            v71 = v118.y * 0.34999999f;
            v72 = v118.z * 0.34999999f;
            v105.x = 0.0;
            v105.y = 0.0;
            v105.z = -10.0;
            v82.x = v70 + 0.0f;
            v82.y = v71 + 0.0f;
            v82.z = v72 + -10.0f;
            AI_EvalFunc_GFXSetv_C_CURRENT(v113, 13204, &v82);
            if (!AI_EvalFunc_TIMEElapsed_C(v122->f_time_gen, 20.0))
            {
                X_4d = 0.02f / v74;
                AI_EvalFunc_GFXSetf_C_CURRENT(v113, 13008, X_4d);
            }
            GFXSetCurGlobal(v113);
            AI_EvalFunc_GFXSetv_C_CURGLOB(13200, &v73);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13104, 0x20000020);
        }
    }
    return 1000;
}