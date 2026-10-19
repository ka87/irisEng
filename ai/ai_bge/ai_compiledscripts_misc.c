#include "ai_compiledscripts_inc.h"

//mpegplayer_init
int mpegplayer_init_univVars;
int mpegplayer_init_univVars_Flag_speciaux_sauve;
int fct_mpegplayer_init_mpegplayer_loop;

//mpegplayer_loop
int mpegplayer_loop_univVars;
int mpegplayer_loop_univVars_Flag_speciaux_sauve;

// ============================

//Unknown_DEMO_ES_select
int Unknown_DEMO_ES_select_GST_Board_panel_gao;
int Unknown_DEMO_ES_select_GST_Board_panel_gao_i_Obj_On_Radar_Nb;
int Unknown_DEMO_ES_select_GST_Board_panel_gao_ao_Obj_On_Radar;
int Unknown_DEMO_ES_select_GST_Board_panel_gao_at_Obj_On_Radar_Desc;

// =====================================

//vehicule_speed_max_vsm_test
int fctvar_vehicule_speed_max_vsm_test__PJ_Aircraft_gao;
int fctvar_vehicule_speed_max_vsm_test__PJ_Enterprise_gao;

int mpegplayer_init()
{
    int result; // eax
    int v1; // eax
    float v2; // [esp+4h] [ebp-94h]
    tAINode* v3; // [esp+8h] [ebp-90h]
    tScriptStruct_MpegPlayer* v4; // [esp+Ch] [ebp-8Ch]
    tGameObjectMain* v5; // [esp+14h] [ebp-84h]
    tGameObjectMain* v6; // [esp+24h] [ebp-74h]
    tGameObjectMain* v7; // [esp+28h] [ebp-70h]
    tGameObjectMain* v8; // [esp+30h] [ebp-68h]
    tGameObjectMain* v9; // [esp+34h] [ebp-64h]
    tGameObjectMain* v10; // [esp+38h] [ebp-60h]
    double v11; // [esp+40h] [ebp-58h]
    tGameObjectMain* v12; // [esp+4Ch] [ebp-4Ch]
    tGameObjectMain* v13; // [esp+50h] [ebp-48h]
    tGameObjectMain* v14; // [esp+54h] [ebp-44h]
    float v15; // [esp+60h] [ebp-38h]
    BOOL v16; // [esp+80h] [ebp-18h]
    tTexProceduralStruct* a1c; // [esp+8Ch] [ebp-Ch]
    tTexProceduralStruct* a1d; // [esp+8Ch] [ebp-Ch]
    int a1a; // [esp+8Ch] [ebp-Ch]
    tTexProceduralStruct* a1e; // [esp+8Ch] [ebp-Ch]
    tTexProceduralStruct* a1f; // [esp+8Ch] [ebp-Ch]
    tTexProceduralStruct* a1b; // [esp+8Ch] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_MpegPlayer*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        mpegplayer_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&mpegplayer_init_univVars_Flag_speciaux_sauve, (void*)mpegplayer_init_univVars, 4488);
        fct_mpegplayer_init_mpegplayer_loop = (int)AI_ul_RealResolveOneRefFunction(0x4901BEE4u);
    }
    v3 = *AI_gppst_CurrentJumpNode;
    if (*AI_gppst_CurrentJumpNode == (tAINode*)6)
    {
        if (v4->des_i_sf_modifier_vrai > 0)
            AI_EvalFunc_ARRBitSet_C((int*)(mpegplayer_init_univVars_Flag_speciaux_sauve + 4), v4->des_i_sf_modifier_vrai);
        v14 = (tGameObjectMain*)v4->des_o_snd_stop;
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
            v12 = (tGameObjectMain*)v4->des_o_snd_stop;
            if (v12)
            {
                if ((unsigned int)v12 <= 2)
                    v12 = AI_MainActor[(uint32_t)v12 - 1];
                AI_EvalFunc_SNDPlayModifier_C(v12, 0);
            }
            else
            {
                AI_EvalFunc_SNDPlayModifier_C(0, 0);
            }
        }
        AI_EvalFunc_AITrackCurStop_C();
    LABEL_37:
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    if (v3 == (tAINode*)14)
    {
        AI_EvalFunc_SNDPlayModifier_C(AI_gpst_CurrentGameObject, 0);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    if (v3 == (tAINode*)15)
    {
        v7 = (tGameObjectMain*)v4->des_o_Interrupteur;
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
        if (!v6)
            AI_EvalFunc_AITrackCurStop_C();
        a1b = AI_EvalFunc_MATGetPro_C(AI_gpst_CurrentGameObject, v4->des_i_SubMat, v4->des_i_SubMatLevel);
        if (!a1b)
            AI_EvalFunc_AITrackCurStop_C();
        if (v4->des_i_StopAtEnd)
            AI_EvalFunc_MATProSeti_C(a1b, 1, 1);
        v16 = 0;
        if (v4->des_i_sf_on_off)
        {
            if (v4->des_i_sf_on_off <= 0)
                v16 = 0;
            else
                v16 = AI_EvalFunc_ARRBitTest_C((int*)(mpegplayer_init_univVars_Flag_speciaux_sauve + 4), v4->des_i_sf_on_off);
        }
        v5 = (tGameObjectMain*)v4->des_o_Interrupteur;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            v1 = AI_EvalFunc_OBJ_CapaTest_C(v5, 0x100);
        }
        else
        {
            v1 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x100);
        }
        v4->i_Play = v1;
        if (v4->i_Play || v16)
        {
            AI_EvalFunc_MATProSeti_C(a1b, 10, 2);
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(2, 0);
        }
        else
        {
            AI_EvalFunc_MATProSeti_C(a1b, 10, 1);
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        }
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_mpegplayer_init_mpegplayer_loop);
        result = 1000;
    }
    else
    {
        if (v4->des_i_sf_modifier_vrai)
        {
            if (v4->des_i_sf_modifier_vrai > 0
                && AI_EvalFunc_ARRBitTest_C(
                    (int*)(mpegplayer_init_univVars_Flag_speciaux_sauve + 4),
                    v4->des_i_sf_modifier_vrai))
            {
                a1c = AI_EvalFunc_MATGetPro_C(AI_gpst_CurrentGameObject, v4->des_i_SubMat, v4->des_i_SubMatLevel);
                AI_EvalFunc_MATProSeti_C(a1c, 10, 1);
                AI_EvalFunc_MATEnableMltxtLvlMT_C(
                    AI_gpst_CurrentGameObject,
                    v4->des_i_mat,
                    v4->des_i_SubMatLevel + 1,
                    v4->des_i_SubMat,
                    1);
                AI_EvalFunc_AITrackCurStop_C();
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                return 1000;
            }
        }
        if (v4->des_i_sf_jesuisla)
        {
            if (v4->des_f_init_la)
            {
                v2 = (float)v4->des_f_init_la;
                if (AI_EvalFunc_TIMEElapsed_C(v2, v4->des_f_duree_here))
                {
                    a1d = AI_EvalFunc_MATGetPro_C(AI_gpst_CurrentGameObject, v4->des_i_SubMat, v4->des_i_SubMatLevel);
                    AI_EvalFunc_MATProSeti_C(a1d, 10, 1);
                    AI_EvalFunc_MATEnableMltxtLvlMT_C(
                        AI_gpst_CurrentGameObject,
                        v4->des_i_mat,
                        v4->des_i_SubMatLevel + 1,
                        v4->des_i_SubMat,
                        1);
                    AI_EvalFunc_SNDStopModifier_C(AI_gpst_CurrentGameObject, 0);
                    *AI_gppst_CurrentJumpNode = (tAINode*)6;
                    TIME_Wait_C(2.0);
                    return 0;
                }
                goto LABEL_37;
            }
            v15 = TIM_gf_MainClock - TIM_gf_LockTime;
            v11 = v15 + MATH_gd_magicDoubleForRound;
            v4->des_f_init_la = (*(uint32_t *)&v11);
            if (v4->des_i_sf_jesuisla <= 0)
                a1a = 0;
            else
                a1a = AI_EvalFunc_ARRBitTest_C((int*)(mpegplayer_init_univVars_Flag_speciaux_sauve + 4), v4->des_i_sf_jesuisla);
            if (a1a)
            {
                a1f = AI_EvalFunc_MATGetPro_C(AI_gpst_CurrentGameObject, v4->des_i_SubMat, v4->des_i_SubMatLevel);
                AI_EvalFunc_MATProSeti_C(a1f, 10, 2);
                AI_EvalFunc_MATEnableMltxtLvlMT_C(
                    AI_gpst_CurrentGameObject,
                    v4->des_i_mat,
                    v4->des_i_SubMatLevel + 1,
                    v4->des_i_SubMat,
                    0);
                v10 = (tGameObjectMain*)v4->des_o_snd_stop;
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
                    v8 = (tGameObjectMain*)v4->des_o_snd_stop;
                    if (v8)
                    {
                        if ((unsigned int)v8 <= 2)
                            v8 = AI_MainActor[(uint32_t)v8 - 1];
                        AI_EvalFunc_SNDStopModifier_C(v8, 0);
                    }
                    else
                    {
                        AI_EvalFunc_SNDStopModifier_C(0, 0);
                    }
                }
                *AI_gppst_CurrentJumpNode = (tAINode*)14;
                TIME_Wait_C(0.1f);
                result = 0;
            }
            else
            {
                a1e = AI_EvalFunc_MATGetPro_C(AI_gpst_CurrentGameObject, v4->des_i_SubMat, v4->des_i_SubMatLevel);
                AI_EvalFunc_MATProSeti_C(a1e, 10, 1);
                AI_EvalFunc_MATEnableMltxtLvlMT_C(
                    AI_gpst_CurrentGameObject,
                    v4->des_i_mat,
                    v4->des_i_SubMatLevel + 1,
                    v4->des_i_SubMat,
                    1);
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 1000;
            }
        }
        else
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)15;
            TIME_Wait_C(0.0099999998f);
            result = 0;
        }
    }
    return result;
}

int mpegplayer_loop()
{
    int result; // eax
    BOOL v1; // eax
    tScriptStruct_MpegPlayer* v2; // [esp+0h] [ebp-64h]
    tGameObjectMain* v3; // [esp+Ch] [ebp-58h]
    tGameObjectMain* v4; // [esp+14h] [ebp-50h]
    tGameObjectMain* v5; // [esp+18h] [ebp-4Ch]
    tGameObjectMain* v6; // [esp+1Ch] [ebp-48h]
    tGameObjectMain* v7; // [esp+28h] [ebp-3Ch]
    tTexProceduralStruct* v8; // [esp+58h] [ebp-Ch]
    tTexProceduralStruct* v9; // [esp+58h] [ebp-Ch]
    tTexProceduralStruct* v10; // [esp+58h] [ebp-Ch]
    tTexProceduralStruct* v11; // [esp+58h] [ebp-Ch]
    tTexProceduralStruct* v12; // [esp+58h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_MpegPlayer*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        mpegplayer_loop_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&mpegplayer_loop_univVars_Flag_speciaux_sauve, (void*)mpegplayer_loop_univVars, 4488);
    }
    if (v2->i_Play
        && v2->des_i_SND_modifier
        && (AI_EvalFunc_SNDGetStatusModifier_C(AI_gpst_CurrentGameObject, 0) & 4) == 0)
    {
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        v2->i_Play = 0;
        v8 = AI_EvalFunc_MATGetPro_C(AI_gpst_CurrentGameObject, v2->des_i_SubMat, v2->des_i_SubMatLevel);
        AI_EvalFunc_MATProSeti_C(v8, 10, 1);
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
        if (v2->des_i_sf_on_off)
        {
            if (v2->des_i_sf_on_off > 0)
                AI_EvalFunc_ARRBitClear_C((int*)(mpegplayer_loop_univVars_Flag_speciaux_sauve + 4), v2->des_i_sf_on_off);
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        if (v2->i_Play && AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x2000))
        {
            v2->i_Play = 0;
            v9 = AI_EvalFunc_MATGetPro_C(AI_gpst_CurrentGameObject, v2->des_i_SubMat, v2->des_i_SubMatLevel);
            AI_EvalFunc_MATProSeti_C(v9, 10, 1);
        }
        if (!v2->i_Play && AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
        {
            v2->i_Play = 1;
            v10 = AI_EvalFunc_MATGetPro_C(AI_gpst_CurrentGameObject, v2->des_i_SubMat, v2->des_i_SubMatLevel);
            AI_EvalFunc_MATProSeti_C(v10, 10, 2);
        }
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x2000);
        v7 = (tGameObjectMain*)v2->des_o_Interrupteur;
        if (v7)
        {
            if ((unsigned int)v7 <= 2)
                v7 = AI_MainActor[(uint32_t)v7 - 1];
            v1 = AI_EvalFunc_OBJ_CapaTest_C(v7, 0x100);
        }
        else
        {
            v1 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x100);
        }
        if (v1 || AI_EvalFunc_OBJ_CapaTest_C_CURRENT(2))
        {
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 2);
            if (v2->i_Play)
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
                v2->i_Play = 0;
                v11 = AI_EvalFunc_MATGetPro_C(AI_gpst_CurrentGameObject, v2->des_i_SubMat, v2->des_i_SubMatLevel);
                AI_EvalFunc_MATProSeti_C(v11, 10, 1);
                AI_EvalFunc_SNDStopModifier_C(AI_gpst_CurrentGameObject, 0);
                if (v2->des_i_sf_on_off && v2->des_i_sf_on_off > 0)
                    AI_EvalFunc_ARRBitClear_C((int*)(mpegplayer_loop_univVars_Flag_speciaux_sauve + 4), v2->des_i_sf_on_off);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
                v6 = (tGameObjectMain*)v2->des_o_stopsound;
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
                if (v5)
                {
                    v4 = (tGameObjectMain*)v2->des_o_stopsound;
                    if (v4)
                    {
                        if ((unsigned int)v4 <= 2)
                            v4 = AI_MainActor[(uint32_t)v4 - 1];
                        AI_EvalFunc_SNDStopModifier_C(v4, -1);
                    }
                    else
                    {
                        AI_EvalFunc_SNDStopModifier_C(0, -1);
                    }
                    v3 = (tGameObjectMain*)v2->des_o_stopsound;
                    if (v3)
                    {
                        if ((unsigned int)v3 <= 2)
                            v3 = AI_MainActor[(uint32_t)v3 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v3, 1);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
                    }
                }
                AI_EvalFunc_SNDPlayModifier_C(AI_gpst_CurrentGameObject, 0);
                v2->i_Play = 1;
                v12 = AI_EvalFunc_MATGetPro_C(AI_gpst_CurrentGameObject, v2->des_i_SubMat, v2->des_i_SubMatLevel);
                AI_EvalFunc_MATProSeti_C(v12, 10, 2);
                if (v2->des_i_sf_on_off && v2->des_i_sf_on_off > 0)
                    AI_EvalFunc_ARRBitSet_C((int*)(mpegplayer_loop_univVars_Flag_speciaux_sauve + 4), v2->des_i_sf_on_off);
            }
        }
        result = 1000;
    }
    return result;
}

int Move_Grid_Move_Grid_init()
{
    double v0; // st7
    tScriptStruct_MoveGrid* v2; // [esp+0h] [ebp-34h]
    tGameObjectMain* v3; // [esp+8h] [ebp-2Ch]
    Vec3 a1; // [esp+Ch] [ebp-28h] BYREF
    tGameObjectMain* v5; // [esp+18h] [ebp-1Ch]
    Vec3 a3; // [esp+20h] [ebp-14h] BYREF
    float v7; // [esp+2Ch] [ebp-8h]
    tScriptStruct_MoveGrid* v8; // [esp+30h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_MoveGrid*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v8 = v2;
    v5 = 0;
    v5 = *AI_MainActor;
    v3 = v5;
    if (v5)
    {
        if ((unsigned int)v5 <= 2)
            v3 = AI_MainActor[(uint32_t)v5 - 1];
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v3);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v7 = (float)v0;
    if (v7 < (double)v8->des_f_distance)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a3);
        AI_EvalFunc_GRIDSetPosAll_C((int)AI_gpst_CurrentGameObject, 0, &a3);
        if (v8->des_i_grid1)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
            AI_EvalFunc_GRIDSetPosAll_C((int)AI_gpst_CurrentGameObject, 1, &a1);
        }
    }
    return 1000;
}

// =============================

int Unknown_DEMO_ES_select()
{
    double v0; // st7
    char* v1; // eax
    char* v2; // eax
    char* v4; // [esp-8h] [ebp-5Ch]
    tScriptStruct_DEMOESSelect* v5; // [esp+4h] [ebp-50h]
    tGameObjectMain* a3; // [esp+Ch] [ebp-48h]
    tGameObjectMain* v7; // [esp+10h] [ebp-44h]
    tGameObjectMain* v8; // [esp+18h] [ebp-3Ch]
    tGameObjectMain* a2; // [esp+1Ch] [ebp-38h]
    tGameObjectMain* v10; // [esp+20h] [ebp-34h]
    tGameObjectMain* v11; // [esp+24h] [ebp-30h]
    tGameObjectMain* v12; // [esp+28h] [ebp-2Ch]
    float v13; // [esp+2Ch] [ebp-28h]
    int v14; // [esp+38h] [ebp-1Ch]
    int i; // [esp+3Ch] [ebp-18h]
    int v16; // [esp+40h] [ebp-14h]
    int v17; // [esp+40h] [ebp-14h]
    int num; // [esp+48h] [ebp-Ch]
    float v19; // [esp+4Ch] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_DEMOESSelect*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v14 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Unknown_DEMO_ES_select_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &Unknown_DEMO_ES_select_GST_Board_panel_gao_i_Obj_On_Radar_Nb,
            (void*)Unknown_DEMO_ES_select_GST_Board_panel_gao,
            2284);
        AI2C_ResolveBuffer(
            &Unknown_DEMO_ES_select_GST_Board_panel_gao_ao_Obj_On_Radar,
            (void*)Unknown_DEMO_ES_select_GST_Board_panel_gao,
            2288);
        AI2C_ResolveBuffer(
            &Unknown_DEMO_ES_select_GST_Board_panel_gao_at_Obj_On_Radar_Desc,
            (void*)Unknown_DEMO_ES_select_GST_Board_panel_gao,
            2372);
    }
    v13 = 1.0e14f;
    for (i = 0; i < 5; ++i)
    {
        v12 = (tGameObjectMain*)v5->ao_ES[i];
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
        if (!v11)
            break;
        v10 = *AI_MainActor;
        if (*AI_MainActor)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            a2 = v10;
        }
        else
        {
            a2 = 0;
        }
        v8 = (tGameObjectMain*)v5->ao_ES[i];
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v8 = AI_MainActor[(uint32_t)v8 - 1];
            v0 = AI_EvalFunc_OBJ_SqrDistHorz_C(v8, a2);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_SqrDistHorz_C(0, a2);
        }
        v19 = (float)v0;
        if (v19 < (double)v13)
        {
            v13 = (float)v0;
            v14 = i;
        }
    }
    if (v14 != v5->i_ES_valide)
    {
        if (v5->i_ES_valide != -1)
        {
            v16 = *(uint32_t*)Unknown_DEMO_ES_select_GST_Board_panel_gao_i_Obj_On_Radar_Nb;
            v7 = (tGameObjectMain*)v5->ao_ES[v5->i_ES_valide];
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                a3 = v7;
            }
            else
            {
                a3 = 0;
            }
            for (num = AI_EvalFunc_ARRObjSearch_C(
                (tGameObjectMain**)(Unknown_DEMO_ES_select_GST_Board_panel_gao_ao_Obj_On_Radar + 4),
                v16,
                a3); num < v16 - 1; ++num)
            {
                *(uint32_t*)(Unknown_DEMO_ES_select_GST_Board_panel_gao_ao_Obj_On_Radar + 4 * num + 4) = *(uint32_t*)(Unknown_DEMO_ES_select_GST_Board_panel_gao_ao_Obj_On_Radar + 4 * num + 8);
                v4 = AI_EvalVarArray_C(10, (char*)Unknown_DEMO_ES_select_GST_Board_panel_gao_at_Obj_On_Radar_Desc, num + 1);
                v1 = AI_EvalVarArray_C(10, (char*)Unknown_DEMO_ES_select_GST_Board_panel_gao_at_Obj_On_Radar_Desc, num);
                memcpy(v1, v4, 8u);
            }
            --* (uint32_t*)Unknown_DEMO_ES_select_GST_Board_panel_gao_i_Obj_On_Radar_Nb;
        }
        v5->i_ES_valide = v14;
        DBG_OutputDebugString("Unknown_DEMO_ES_select: i_ES_valide set: %d\n", v5->i_ES_valide);
        v17 = *(uint32_t*)Unknown_DEMO_ES_select_GST_Board_panel_gao_i_Obj_On_Radar_Nb;
        *(uint32_t*)(Unknown_DEMO_ES_select_GST_Board_panel_gao_ao_Obj_On_Radar + 4 * v17 + 4) = v5->ao_ES[v5->i_ES_valide];
        v2 = AI_EvalVarArray_C(10, (char*)Unknown_DEMO_ES_select_GST_Board_panel_gao_at_Obj_On_Radar_Desc, v17);
        memcpy(v2, &v5->Nom_De_L_Entree_Radar, 8u);
        ++* (uint32_t*)Unknown_DEMO_ES_select_GST_Board_panel_gao_i_Obj_On_Radar_Nb;
    }
    return 1000;
}

// ===============================

int vehicule_speed_max_vsm_test()
{
    int result; // eax
    BOOL v1; // eax
    BOOL v2; // eax
    BOOL v3; // eax
    tScriptStruct_VehiculeMaxSpeedTest* v4; // [esp+0h] [ebp-84h]
    tGameObjectMain* v5; // [esp+8h] [ebp-7Ch]
    tGameObjectMain* v6; // [esp+10h] [ebp-74h]
    tGameObjectMain* v7; // [esp+18h] [ebp-6Ch]
    tGameObjectMain* v8; // [esp+20h] [ebp-64h]
    tGameObjectMain* v9; // [esp+24h] [ebp-60h]
    tGameObjectMain* v10; // [esp+28h] [ebp-5Ch]
    tGameObjectMain* v11; // [esp+38h] [ebp-4Ch]
    tMessage Src; // [esp+44h] [ebp-40h] BYREF
    BOOL v13; // [esp+78h] [ebp-Ch]
    BOOL v14; // [esp+7Ch] [ebp-8h]
    tScriptStruct_VehiculeMaxSpeedTest* v15; // [esp+80h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_VehiculeMaxSpeedTest*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v15 = v4;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fctvar_vehicule_speed_max_vsm_test__PJ_Aircraft_gao = AI_ul_RealResolveOneRef(42, 0x7100BBCCu);
        fctvar_vehicule_speed_max_vsm_test__PJ_Enterprise_gao = AI_ul_RealResolveOneRef(42, 0x72001EC9u);
    }
    v11 = *AI_MainActor;
    v10 = *AI_MainActor;
    if (*AI_MainActor)
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
        if (v15->des_actor)
            goto LABEL_43;
        v8 = *AI_MainActor;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v8 = AI_MainActor[(uint32_t)v11 - 1];
            v1 = AI_EvalFunc_AIIsModel_C(v8, (tAIModel*)fctvar_vehicule_speed_max_vsm_test__PJ_Aircraft_gao);
        }
        else
        {
            v1 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_vehicule_speed_max_vsm_test__PJ_Aircraft_gao);
        }
        v14 = v1;
        if (v1)
        {
        LABEL_43:
            if (!v15->des_actor)
                goto LABEL_29;
            v7 = v11;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v7 = AI_MainActor[(uint32_t)v11 - 1];
                v2 = AI_EvalFunc_AIIsModel_C(v7, (tAIModel*)fctvar_vehicule_speed_max_vsm_test__PJ_Enterprise_gao);
            }
            else
            {
                v2 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_vehicule_speed_max_vsm_test__PJ_Enterprise_gao);
            }
            v13 = v2;
            if (v2)
            {
            LABEL_29:
                v6 = v11;
                if (v11)
                {
                    if ((unsigned int)v11 <= 2)
                        v6 = AI_MainActor[(uint32_t)v11 - 1];
                    v3 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, v6);
                }
                else
                {
                    v3 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, 0);
                }
                if (v3)
                {
                    Src.msgID = 56;
                    Src.f2 = v15->des_speed_max;
                    Src.f1 = v15->des_speed_min;
                    Src.Int1 = v15->des_saut_autorise;
                    Src.Int2 = v15->des_boost_autorise;
                    Src.v1.x = (float)v15->des_shoot_autorise;
                    Src.v1.y = 0.0;
                    Src.v1.z = 0.0;
                    v5 = v11;
                    if (v11)
                    {
                        if ((unsigned int)v11 <= 2)
                            v5 = AI_MainActor[(uint32_t)v11 - 1];
                        AI_EvalFunc_MSGSend_C(v5, &Src);
                    }
                    else
                    {
                        AI_EvalFunc_MSGSend_C(0, &Src);
                    }
                }
                result = 1000;
            }
            else
            {
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
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
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}