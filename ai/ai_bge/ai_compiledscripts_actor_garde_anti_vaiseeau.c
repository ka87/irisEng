#include "ai_compiledscripts_inc.h"

//Garde_anti_vaisseau_gav_init
int Garde_anti_vaisseau_gav_init_univVars;
int Garde_anti_vaisseau_gav_init_univVars_Flag_speciaux_sauve;
int Garde_anti_vaisseau_gav_init_univVars_Perso_ID_actor;
int Garde_anti_vaisseau_gav_init_GST_Global_gao;
int Garde_anti_vaisseau_gav_init_GST_Global_gao_o_ciel_acteur;
int fct_Garde_anti_vaisseau_gav_init_Garde_anti_vaisseau_gav_interception;
int fct_Garde_anti_vaisseau_gav_init_Garde_anti_vaisseau_gav_test;

//Garde_anti_vaisseau_gav_test
int Garde_anti_vaisseau_gav_test__PJ_Sally_gao;
int Garde_anti_vaisseau_gav_test_TXT_Sally_gao;
int Garde_anti_vaisseau_gav_test_GST_Board_panel_gao;
int Garde_anti_vaisseau_gav_test_univVars;
int Garde_anti_vaisseau_gav_test_univVars_Flag_speciaux_sauve;
int Garde_anti_vaisseau_gav_test_GST_Global_gao;
int Garde_anti_vaisseau_gav_test_GST_Global_gao_o_joy_acteur;
int Garde_anti_vaisseau_gav_test_DUP_WP_simple_de_base_gao;

int Garde_anti_vaisseau_gav_interception()
{
    float v1; // [esp+4h] [ebp-94h]
    float v2; // [esp+8h] [ebp-90h]
    tScriptStruct_PNJGardeAntiVaisseau* v3; // [esp+Ch] [ebp-8Ch]
    tGameObjectMain* v4; // [esp+14h] [ebp-84h]
    float v5; // [esp+18h] [ebp-80h]
    float v6; // [esp+1Ch] [ebp-7Ch]
    tGameObjectMain* v7; // [esp+2Ch] [ebp-6Ch]
    tGameObjectMain* v8; // [esp+3Ch] [ebp-5Ch]
    float v9; // [esp+40h] [ebp-58h]
    Vec3 v10; // [esp+44h] [ebp-54h] BYREF
    unsigned int v11; // [esp+50h] [ebp-48h]
    float v12; // [esp+58h] [ebp-40h]
    Vec3 v13; // [esp+5Ch] [ebp-3Ch] BYREF
    Vec3 v14; // [esp+68h] [ebp-30h] BYREF
    Vec3 a3; // [esp+78h] [ebp-20h] BYREF
    tScriptStruct_PNJGardeAntiVaisseau* v17; // [esp+84h] [ebp-14h]
    float v18; // [esp+88h] [ebp-10h]
    float v19; // [esp+8Ch] [ebp-Ch]
    float v20; // [esp+90h] [ebp-8h]
    float v21; // [esp+94h] [ebp-4h]

    v13.x = 1.0;
    v13.y = 0.0;
    v13.z = 0.0;
    v10.x = 1.0;
    v10.y = 0.0;
    v10.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_PNJGardeAntiVaisseau*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v17 = v3;
    v18 = 0.0;
    if (v3->des_f_rotation != 0.0)
    {
        v12 = TIM_gf_dt;
        v1 = v17->des_f_rotation * TIM_gf_dt;
        AI_EvalFunc_OBJRotateLocalZ_c(AI_gpst_CurrentGameObject, v1);
    }
    v8 = (tGameObjectMain*)v17->o_beluga;
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
    v10 = a3;
    if (v17->i_ok)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v17->f_time_ok, 5.0))
        {
            v11 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 6291456, 0);
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1;
        }
        v9 = TIM_gf_MainClock - TIM_gf_LockTime;
        v18 = (v9 - v17->f_time_ok) / 5.0f;
        v7 = (tGameObjectMain*)v17->o_beluga;
        if (v7)
        {
            if ((unsigned int)v7 <= 2)
                v7 = AI_MainActor[(uint32_t)v7 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v7, &v14);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &v14);
        }
        v13 = v14;
        if (v14.z < 0.0)
            AI_EvalFunc_MATHVecSetHorzNormalize_C(&v13);
        v6 = v18 * 5000.0f;
        v19 = v13.x * v6;
        v20 = v13.y * v6;
        v21 = v13.z * v6;
        v10.x = v10.x + v19;
        v10.y = v10.y + v20;
        v10.z = v10.z + v21;
        v5 = v17->des_f_vert_offset + v17->des_f_altitude_stop;
        if (v10.z < (double)v5)
            v2 = v17->des_f_vert_offset + v17->des_f_altitude_stop;
        else
            v2 = v10.z;
        v10.z = v2;
    }
    else
    {
        if (v10.z > (double)v17->des_f_altitude_stop)
        {
            v10.z = v17->des_f_altitude_stop;
            v4 = (tGameObjectMain*)v17->o_beluga;
            if (v4)
            {
                if ((unsigned int)v4 <= 2)
                    v4 = AI_MainActor[(uint32_t)v4 - 1];
                AI_EvalFunc_OBJSetPos_C(v4, &v10);
            }
            else
            {
                AI_EvalFunc_OBJSetPos_C(0, &v10);
            }
        }
        v10.z = v17->des_f_vert_offset + v17->des_f_altitude_stop;
    }
    AI_EvalFunc_OBJSetPos_C_CURRENT(&v10);
    return 1000;
}

int Garde_anti_vaisseau_gav_init()
{
    int result; // eax
    int v1; // eax
    tScriptStruct_PNJGardeAntiVaisseau* v2; // [esp+0h] [ebp-68h]
    tGameObjectMain* v3; // [esp+8h] [ebp-60h]
    tGameObjectMain* v4; // [esp+18h] [ebp-50h]
    tGameObjectMain* v5; // [esp+20h] [ebp-48h]
    tGameObjectMain* v6; // [esp+24h] [ebp-44h]
    tGameObjectMain* v7; // [esp+28h] [ebp-40h]
    Vec3 a3; // [esp+38h] [ebp-30h] BYREF
    Vec3 v9; // [esp+44h] [ebp-24h]
    int v10; // [esp+54h] [ebp-14h]
    BOOL v11; // [esp+58h] [ebp-10h]
    int v12; // [esp+5Ch] [ebp-Ch]
    unsigned int v13; // [esp+60h] [ebp-8h]
    tScriptStruct_PNJGardeAntiVaisseau* v14; // [esp+64h] [ebp-4h]

    v9.x = 1.0;
    v9.y = 0.0;
    v9.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJGardeAntiVaisseau*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v14 = v2;
    v11 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        int Garde_anti_vaisseau_gav_init_univVars;
        int Garde_anti_vaisseau_gav_init_univVars_Flag_speciaux_sauve;
        int Garde_anti_vaisseau_gav_init_univVars_Perso_ID_actor;
        int Garde_anti_vaisseau_gav_init_GST_Global_gao;
        int Garde_anti_vaisseau_gav_init_GST_Global_gao_o_ciel_acteur;
        int fct_Garde_anti_vaisseau_gav_init_Garde_anti_vaisseau_gav_interception;
        int fct_Garde_anti_vaisseau_gav_init_Garde_anti_vaisseau_gav_test;
        Garde_anti_vaisseau_gav_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Garde_anti_vaisseau_gav_init_univVars_Flag_speciaux_sauve,
            (void*)Garde_anti_vaisseau_gav_init_univVars,
            4488);
        AI2C_ResolveBuffer(
            &Garde_anti_vaisseau_gav_init_univVars_Perso_ID_actor,
            (void*)Garde_anti_vaisseau_gav_init_univVars,
            988);
        Garde_anti_vaisseau_gav_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Garde_anti_vaisseau_gav_init_GST_Global_gao_o_ciel_acteur,
            (void*)Garde_anti_vaisseau_gav_init_GST_Global_gao,
            2776);
        fct_Garde_anti_vaisseau_gav_init_Garde_anti_vaisseau_gav_interception = (int)AI_ul_RealResolveOneRefFunction(0x720025EDu);
        fct_Garde_anti_vaisseau_gav_init_Garde_anti_vaisseau_gav_test = (int)AI_ul_RealResolveOneRefFunction(0x720025EBu);
    }
    if (v14->des_i_test_SF
        && (v14->des_i_test_SF <= 0 ? (v11 = 0) : (v11 = AI_EvalFunc_ARRBitTest_C(
            (int*)(Garde_anti_vaisseau_gav_init_univVars_Flag_speciaux_sauve
                + 4),
            v14->des_i_test_SF)),
            v11))
    {
        v13 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x600000, 0);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1;
    }
    else
    {
        v14->o_beluga = *(uint32_t*)(Garde_anti_vaisseau_gav_init_univVars_Perso_ID_actor + 40);
        v7 = (tGameObjectMain*)v14->o_beluga;
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
            goto LABEL_35;
        v5 = (tGameObjectMain*)v14->o_beluga;
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            v1 = AI_EvalFunc_AIPriorityGet_C(v5);
        }
        else
        {
            v1 = AI_EvalFunc_AIPriorityGet_C(0);
        }
        v12 = v1;
        AI_EvalFunc_AIPrioritySet_C(AI_gpst_CurrentGameObject, v1 - 1);
        v14->o_ciel_actor = *(uint32_t*)Garde_anti_vaisseau_gav_init_GST_Global_gao_o_ciel_acteur;
        v4 = (tGameObjectMain*)v14->o_ciel_actor;
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            AI_EvalFunc_OBJGetPos_C(v4, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        v9 = a3;
        v14->des_f_altitude_stop = a3.z + v14->des_f_altitude_stop;
        v14->des_f_altitude_test = v9.z + v14->des_f_altitude_test;
        v3 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v3 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v3,
                2,
                (tAIFunction*)fct_Garde_anti_vaisseau_gav_init_Garde_anti_vaisseau_gav_interception);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_Garde_anti_vaisseau_gav_init_Garde_anti_vaisseau_gav_interception);
        }
        v10 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Garde_anti_vaisseau_gav_init_Garde_anti_vaisseau_gav_test);
        if (v10 == 1000)
            LABEL_35:
        result = 1000;
        else
            result = v10;
    }
    return result;
}

int Garde_anti_vaisseau_gav_test()
{
    int v1; // eax
    tGameObjectMain* v2; // eax
    tScriptStruct_PNJGardeAntiVaisseau* v3; // [esp+Ch] [ebp-1D0h]
    tGameObjectMain* v4; // [esp+14h] [ebp-1C8h]
    tGameObjectMain* v5; // [esp+18h] [ebp-1C4h]
    tGameObjectMain* v6; // [esp+1Ch] [ebp-1C0h]
    tGameObjectMain* v7; // [esp+2Ch] [ebp-1B0h]
    tGameObjectMain* v8; // [esp+58h] [ebp-184h]
    tGameObjectMain* v9; // [esp+60h] [ebp-17Ch]
    tGameObjectMain* v10; // [esp+64h] [ebp-178h]
    tGameObjectMain* v11; // [esp+68h] [ebp-174h]
    tGameObjectMain* v12; // [esp+70h] [ebp-16Ch]
    tGameObjectMain* v13; // [esp+78h] [ebp-164h]
    tGameObjectMain* v14; // [esp+84h] [ebp-158h]
    tGameObjectMain* v15; // [esp+8Ch] [ebp-150h]
    tGameObjectMain* v16; // [esp+90h] [ebp-14Ch]
    tGameObjectMain* v17; // [esp+94h] [ebp-148h]
    tGameObjectMain* v18; // [esp+98h] [ebp-144h]
    tGameObjectMain* v19; // [esp+9Ch] [ebp-140h]
    Vec3 v20; // [esp+A0h] [ebp-13Ch] BYREF
    tMessage v21; // [esp+B8h] [ebp-124h] BYREF
    tMessage Src; // [esp+ECh] [ebp-F0h] BYREF
    tGameObjectMain* v23; // [esp+12Ch] [ebp-B0h]
    Vec3 v24; // [esp+130h] [ebp-ACh] BYREF
    Vec3 pos; // [esp+140h] [ebp-9Ch] BYREF
    Vec3 a1; // [esp+160h] [ebp-7Ch] BYREF
    float v28; // [esp+16Ch] [ebp-70h]
    float v29; // [esp+170h] [ebp-6Ch]
    float v30; // [esp+174h] [ebp-68h]
    int v31; // [esp+178h] [ebp-64h]
    int v32; // [esp+17Ch] [ebp-60h]
    int v33; // [esp+180h] [ebp-5Ch]
    Vec3 v35; // [esp+188h] [ebp-54h] BYREF
    Vec3 v36; // [esp+194h] [ebp-48h] BYREF
    int v37; // [esp+1ACh] [ebp-30h]
    int v38; // [esp+1B0h] [ebp-2Ch]
    int v39; // [esp+1B4h] [ebp-28h]
    Vec3 a3; // [esp+1B8h] [ebp-24h] BYREF
    tScriptStruct_PNJGardeAntiVaisseau* v41; // [esp+1C4h] [ebp-18h]
    int v42; // [esp+1D8h] [ebp-4h]

    pos.x = 1.0;
    pos.y = 0.0;
    pos.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_PNJGardeAntiVaisseau*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v41 = v3;
    v23 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Garde_anti_vaisseau_gav_test__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        Garde_anti_vaisseau_gav_test_TXT_Sally_gao = AI_ul_RealResolveOneRefObject(0x7200064Eu);
        Garde_anti_vaisseau_gav_test_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        Garde_anti_vaisseau_gav_test_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Garde_anti_vaisseau_gav_test_univVars_Flag_speciaux_sauve,
            (void*)Garde_anti_vaisseau_gav_test_univVars,
            4488);
        Garde_anti_vaisseau_gav_test_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Garde_anti_vaisseau_gav_test_GST_Global_gao_o_joy_acteur,
            (void*)Garde_anti_vaisseau_gav_test_GST_Global_gao,
            136);
        Garde_anti_vaisseau_gav_test_DUP_WP_simple_de_base_gao = AI_ul_RealResolveOneRefObject(0x7100BEC4u);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)5)
        return 1000;
    if (*AI_gppst_CurrentJumpNode == (tAINode*)6)
    {
        if (v41->des_i_test_SF <= 0)
            v41->i_ok = 0;
        else
            v41->i_ok = AI_EvalFunc_ARRBitTest_C(
                (int*)(Garde_anti_vaisseau_gav_test_univVars_Flag_speciaux_sauve + 4),
                v41->des_i_test_SF);
        if (v41->i_ok)
        {
            v41->f_time_ok = TIM_gf_MainClock - TIM_gf_LockTime;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        if (v41->des_i_texte_echec_code != -1)
        {
            Src.msgID = 30;
            Src.Int1 = v41->des_i_texte_echec_code;
            Src.Int2 = 4900;
            Src.f2 = 3.0;
            Src.v1.x = 0.0;
            Src.v1.y = 0.0;
            Src.v1.z = 0.0;
            Src.v2.x = 0.0;
            Src.v2.y = 0.0;
            Src.v2.z = 0.0;
            Src.gao = 0;
            Src.gao2 = (tGameObjectMain*)Garde_anti_vaisseau_gav_test__PJ_Sally_gao;
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)Garde_anti_vaisseau_gav_test_TXT_Sally_gao, &Src);
        }
        v13 = (tGameObjectMain*)v41->o_beluga;
        if (v13)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            AI_EvalFunc_OBJGetPos_C(v13, &v36);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v36);
        }
        v12 = (tGameObjectMain*)v41->o_ciel_actor;
        if (v12)
        {
            if ((unsigned int)v12 <= 2)
                v12 = AI_MainActor[(uint32_t)v12 - 1];
            AI_EvalFunc_OBJGetPos_C(v12, &v24);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v24);
        }
        if (v24.z - 10.0 <= v36.z)
        {
            v8 = (tGameObjectMain*)v41->o_beluga;
            if (v8)
            {
                if ((unsigned int)v8 <= 2)
                    v8 = AI_MainActor[(uint32_t)v8 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v8, &v35);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v35);
            }
            a1.x = v35.x;
            a1.y = v35.y;
            a1.z = 0.0;
            if (AI2C_VecEqNull(&a1))
            {
                v31 = 0;
                v32 = 0;
                v33 = 0;
                a1.x = 0.0;
                a1.y = 0.0;
                a1.z = 0.0;
            }
            else
            {
                MATH_NormalizeEqualVector(&a1);
            }
            v7 = (tGameObjectMain*)v41->o_beluga;
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                AI_EvalFunc_OBJGetPos_C(v7, &v20);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v20);
            }
            v28 = a1.x * 50.0f;
            v29 = a1.y * 50.0f;
            v30 = a1.z * 50.0f;
            pos.x = v20.x + v28;
            pos.y = v20.y + v29;
            pos.z = v20.z + v30;
            v37 = 0;
            v38 = 0;
            v39 = -1027080192;
            pos.x = pos.x + 0.0f;
            pos.y = pos.y + 0.0f;
            pos.z = pos.z + -100.0f;
            v6 = (tGameObjectMain*)v41->o_wp_de_base;
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
                v4 = (tGameObjectMain*)v41->o_wp_de_base;
                if (v4)
                {
                    if ((unsigned int)v4 <= 2)
                        v4 = AI_MainActor[(uint32_t)v4 - 1];
                    AI_EvalFunc_OBJSetPos_C(v4, &pos);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &pos);
                }
            }
            else
            {
                v2 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)Garde_anti_vaisseau_gav_test_DUP_WP_simple_de_base_gao, &pos);
                v41->o_wp_de_base = (int)v2;
            }
            *(uint32_t*)Garde_anti_vaisseau_gav_test_GST_Global_gao_o_joy_acteur = v41->o_wp_de_base;
        }
        else
        {
            v41->i_flag_show_lecteur_code = 1;
            v41->f_time_demande_code = 0.0;
            v11 = (tGameObjectMain*)v41->o_wp_de_base;
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
                v9 = (tGameObjectMain*)v41->o_wp_de_base;
                if (v9)
                {
                    if ((unsigned int)v9 <= 2)
                        v9 = AI_MainActor[(uint32_t)v9 - 1];
                    v1 = AI_EvalFunc_OBJDestroy_C(v9);
                }
                else
                {
                    v1 = AI_EvalFunc_OBJDestroy_C(0);
                }
                v42 = v1;
                if (v1 != 1000)
                    return v42;
                v41->o_wp_de_base = 0;
                *(uint32_t*)Garde_anti_vaisseau_gav_test_GST_Global_gao_o_joy_acteur = 0;
            }
        }
        return 1000;
    }
    v23 = *AI_MainActor;
    if (v41->i_ok)
        goto LABEL_23;
    v19 = v23;
    if (v23)
    {
        if ((unsigned int)v23 <= 2)
            v19 = AI_MainActor[(uint32_t)v23 - 1];
        v18 = v19;
    }
    else
    {
        v18 = 0;
    }
    v17 = (tGameObjectMain*)v41->o_beluga;
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
    if (v18 != v16)
    {
    LABEL_23:
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    if (!v41->i_flag_show_lecteur_code)
    {
        if (!AI_EvalFunc_OBJ_CapaTest_C((tGameObjectMain*)Garde_anti_vaisseau_gav_test_GST_Board_panel_gao, 256))
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)6;
            TIME_Wait_C(0.1f);
            return 0;
        }
        return 1000;
    }
    v15 = (tGameObjectMain*)v41->o_beluga;
    if (v15)
    {
        if ((unsigned int)v15 <= 2)
            v15 = AI_MainActor[(uint32_t)v15 - 1];
        AI_EvalFunc_OBJGetPos_C(v15, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    if (v41->des_f_altitude_test >= (double)a3.z)
        return 1000;
    if (v41->f_time_demande_code == 0.0)
    {
        v41->f_time_demande_code = TIM_gf_MainClock - TIM_gf_LockTime;
        Src.msgID = 30;
        Src.Int1 = v41->des_i_texte_demande_code;
        Src.Int2 = 4900;
        Src.f2 = 3.0;
        Src.v1.x = 0.0;
        Src.v1.y = 0.0;
        Src.v1.z = 0.0;
        Src.v2.x = 0.0;
        Src.v2.y = 0.0;
        Src.v2.z = 0.0;
        Src.gao = 0;
        Src.gao2 = (tGameObjectMain*)Garde_anti_vaisseau_gav_test__PJ_Sally_gao;
        AI_EvalFunc_MSGSend_C((tGameObjectMain*)Garde_anti_vaisseau_gav_test_TXT_Sally_gao, &Src);
        return 1000;
    }
    if (!AI_EvalFunc_TIMEElapsed_C(v41->f_time_demande_code, 4.0))
        return 1000;
    v41->i_flag_show_lecteur_code = 0;
    v21.msgID = 57;
    v14 = (tGameObjectMain*)v41->des_o_lecteur_code;
    if (v14)
    {
        if ((unsigned int)v14 <= 2)
            v14 = AI_MainActor[(uint32_t)v14 - 1];
        AI_EvalFunc_MSGSend_C(v14, &v21);
    }
    else
    {
        AI_EvalFunc_MSGSend_C(0, &v21);
    }
    *AI_gppst_CurrentJumpNode = (tAINode*)5;
    TIME_Wait_C(0.1f);
    return 0;
}