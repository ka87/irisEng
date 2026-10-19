#include "ai_compiledscripts_inc.h"

//Lecteur_Code_lc_init
int fct_Lecteur_Code_lc_init_Lecteur_Code_lc_reflex;

//Lecteur_Code_lc_reflex
int Lecteur_Code_lc_reflex__PJ_Sally_gao;
int Lecteur_Code_lc_reflex__PJ_Sally_gao_current_pm_mode;
int Lecteur_Code_lc_reflex__PJ_Sally_gao_current_pm_priority;
int Lecteur_Code_lc_reflex_GST_Board_panel_gao;
int Lecteur_Code_lc_reflex_GST_Board_panel_gao_i_sally_priority;
int Lecteur_Code_lc_reflex_GST_Board_panel_gao_o_sally_contextuel;
int Lecteur_Code_lc_reflex_GST_Board_panel_gao_i_sally_action;
int Lecteur_Code_lc_reflex_GST_Board_panel_gao_v_sally_offset;
int Lecteur_Code_lc_reflex_GST_Board_panel_gao_i_force_interface;
int Lecteur_Code_lc_reflex_INV_manager_gao_i_CodeUsableFirst;
int Lecteur_Code_lc_reflex_INV_manager_gao;
int Lecteur_Code_lc_reflex_INV_manager_gao_i_CodeUsableLast;
int Lecteur_Code_lc_reflex_INV_manager_gao_i_OpeningMode;
int fct_Lecteur_Code_lc_reflex_Lecteur_Code_lc_attente_code;

//Lecteur_Code_lc_attente_code
int Lecteur_Code_lc_attente_code_INV_manager_gao;
int Lecteur_Code_lc_attente_code_INV_manager_gao_i_OpeningMode;
int Lecteur_Code_lc_attente_code_univVars;
int Lecteur_Code_lc_attente_code_univVars_Flag_speciaux_sauve;
int Lecteur_Code_lc_attente_code_univVars_f_sally_fric;
int Lecteur_Code_lc_attente_code_INV_manager_gao_i_Code;
int Lecteur_Code_lc_attente_code_INV_manager_gao_i_Allocated;
int fct_Lecteur_Code_lc_attente_code_Lecteur_Code_lc_reflex;

int Lecteur_Code_lc_init()
{
    tScriptStruct_LecteurCode* v1; // [esp+0h] [ebp-18h]
    tGameObjectMain* v2; // [esp+8h] [ebp-10h]
    tGameObjectMain* v3; // [esp+Ch] [ebp-Ch]
    tGameObjectMain* v4; // [esp+10h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_LecteurCode*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Lecteur_Code_lc_init_Lecteur_Code_lc_reflex = (int)AI_ul_RealResolveOneRefFunction(0x9E0035B8);
    }
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
    v4 = (tGameObjectMain*)v1->o_other_capa;
    if (v4)
    {
        if ((unsigned int)v4 <= 2)
            v4 = AI_MainActor[(uint32_t)v4 - 1];
        v3 = v4;
    }
    else
    {
        v3 = 0;
    }
    if (v3)
    {
        v2 = (tGameObjectMain*)v1->o_other_capa;
        if (v2)
        {
            if ((unsigned int)v2 <= 2)
                v2 = AI_MainActor[(uint32_t)v2 - 1];
            AI_EvalFunc_OBJ_CapaSet_C(v2, 0, 0x100);
        }
        else
        {
            AI_EvalFunc_OBJ_CapaSet_C(0, 0, 0x100);
        }
    }
    AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x8000, 0);
    v1->des_i_distance = v1->des_i_distance * v1->des_i_distance;
    v1->angle_de_proposition = v1->angle_de_proposition * 0.017453f;
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Lecteur_Code_lc_init_Lecteur_Code_lc_reflex);
    return 1000;
}

int Lecteur_Code_lc_reflex()
{
    double v0; // st7
    double v1; // st7
    tScriptStruct_LecteurCode* v3; // [esp+4h] [ebp-100h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-F8h]
    tGameObjectMain* v5; // [esp+10h] [ebp-F4h]
    tGameObjectMain* v6; // [esp+14h] [ebp-F0h]
    tGameObjectMain* v7; // [esp+24h] [ebp-E0h]
    tGameObjectMain* v8; // [esp+2Ch] [ebp-D8h]
    tGameObjectMain* v9; // [esp+34h] [ebp-D0h]
    tGameObjectMain* v10; // [esp+3Ch] [ebp-C8h]
    tGameObjectMain* v11; // [esp+40h] [ebp-C4h]
    tGameObjectMain* v12; // [esp+44h] [ebp-C0h]
    Vec3 v13; // [esp+48h] [ebp-BCh] BYREF
    Vec3 a3; // [esp+54h] [ebp-B0h] BYREF
    tGameObjectMain* v15; // [esp+60h] [ebp-A4h]
    float v16; // [esp+64h] [ebp-A0h]
    tMessage v17; // [esp+68h] [ebp-9Ch] BYREF
    float v19; // [esp+A0h] [ebp-64h]
    Vec3 v20; // [esp+ACh] [ebp-58h] BYREF
    Vec3 dest; // [esp+B8h] [ebp-4Ch] BYREF
    BOOL v22; // [esp+D0h] [ebp-34h]
    float v23; // [esp+DCh] [ebp-28h]
    float v24; // [esp+E0h] [ebp-24h]
    tScriptStruct_LecteurCode* v25; // [esp+E8h] [ebp-1Ch]
    Vec3 v26; // [esp+ECh] [ebp-18h] BYREF
    int v27; // [esp+F8h] [ebp-Ch]
    int v28; // [esp+FCh] [ebp-8h]
    void* Src; // [esp+100h] [ebp-4h]

    v20.x = 1.0;
    v20.y = 0.0;
    v20.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_LecteurCode*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v25 = v3;
    v15 = 0;
    v16 = 0.0;
    v22 = 0;
    v28 = 0;
    v24 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Lecteur_Code_lc_reflex__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(
            &Lecteur_Code_lc_reflex__PJ_Sally_gao_current_pm_mode,
            (void*)Lecteur_Code_lc_reflex__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(
            &Lecteur_Code_lc_reflex__PJ_Sally_gao_current_pm_priority,
            (void*)Lecteur_Code_lc_reflex__PJ_Sally_gao,
            576);
        Lecteur_Code_lc_reflex_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &Lecteur_Code_lc_reflex_GST_Board_panel_gao_i_sally_priority,
            (void*)Lecteur_Code_lc_reflex_GST_Board_panel_gao,
            396);
        AI2C_ResolveBuffer(
            &Lecteur_Code_lc_reflex_GST_Board_panel_gao_o_sally_contextuel,
            (void*)Lecteur_Code_lc_reflex_GST_Board_panel_gao,
            388);
        AI2C_ResolveBuffer(
            &Lecteur_Code_lc_reflex_GST_Board_panel_gao_i_sally_action,
            (void*)Lecteur_Code_lc_reflex_GST_Board_panel_gao,
            412);
        AI2C_ResolveBuffer(
            &Lecteur_Code_lc_reflex_GST_Board_panel_gao_v_sally_offset,
            (void*)Lecteur_Code_lc_reflex_GST_Board_panel_gao,
            400);
        AI2C_ResolveBuffer(
            &Lecteur_Code_lc_reflex_GST_Board_panel_gao_i_force_interface,
            (void*)Lecteur_Code_lc_reflex_GST_Board_panel_gao,
            3292);
        Lecteur_Code_lc_reflex_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &Lecteur_Code_lc_reflex_INV_manager_gao_i_CodeUsableFirst,
            (void*)Lecteur_Code_lc_reflex_INV_manager_gao,
            22564);
        AI2C_ResolveBuffer(
            &Lecteur_Code_lc_reflex_INV_manager_gao_i_CodeUsableLast,
            (void*)Lecteur_Code_lc_reflex_INV_manager_gao,
            22568);
        AI2C_ResolveBuffer(
            &Lecteur_Code_lc_reflex_INV_manager_gao_i_OpeningMode,
            (void*)Lecteur_Code_lc_reflex_INV_manager_gao,
            100);
        fct_Lecteur_Code_lc_reflex_Lecteur_Code_lc_attente_code = (int)AI_ul_RealResolveOneRefFunction(0x9E0035BD);
    }
    if (!v25->i_pas_tout_le_temps)
    {
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
        v12 = (tGameObjectMain*)v25->o_other_capa;
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
            v10 = (tGameObjectMain*)v25->o_other_capa;
            if (v10)
            {
                if ((unsigned int)v10 <= 2)
                    v10 = AI_MainActor[(uint32_t)v10 - 1];
                AI_EvalFunc_OBJ_CapaSet_C(v10, 0, 0x100);
            }
            else
            {
                AI_EvalFunc_OBJ_CapaSet_C(0, 0, 0x100);
            }
        }
    }
    v15 = *AI_MainActor;
    v9 = v15;
    if (v15)
    {
        if ((unsigned int)v15 <= 2)
            v9 = AI_MainActor[(uint32_t)v15 - 1];
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v9);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v16 = (float)v0;
    v8 = v15;
    if (v15)
    {
        if ((unsigned int)v15 <= 2)
            v8 = AI_MainActor[(uint32_t)v15 - 1];
        v1 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v8);
    }
    else
    {
        v1 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
    }
    v24 = (float)v1;
    v7 = v15;
    if (v15)
    {
        if ((unsigned int)v15 <= 2)
            v7 = AI_MainActor[(uint32_t)v15 - 1];
        AI_EvalFunc_OBJGetPos_C(v7, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v13);
    MATH_SubstractVec(&dest, &a3, &v13);
    v20 = dest;
    AI_EvalFunc_MATHVecSetHorzNormalize_C(&v20);
    if (v25->des_i_distance + v25->des_i_distance > v16 && v25->des_i_distance > (double)v24)
    {
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v26);
        v19 = v26.z * v20.z + v26.y * v20.y + v20.x * v26.x;
        v23 = cosf(v25->angle_de_proposition);
        if (v19 > (double)v23)
        {
            v27 = (unsigned __int16)*(uint32_t*)(Lecteur_Code_lc_reflex__PJ_Sally_gao + 4);
            if ((v27 & 1) != 0)
                v22 = !*(uint32_t*)Lecteur_Code_lc_reflex__PJ_Sally_gao_current_pm_mode
                || *(int*)Lecteur_Code_lc_reflex__PJ_Sally_gao_current_pm_priority < 20;
            else
                v22 = 0;
            if (v22 && *(float*)Lecteur_Code_lc_reflex_GST_Board_panel_gao_i_sally_priority < 5.0)
            {
                *(uint32_t*)Lecteur_Code_lc_reflex_GST_Board_panel_gao_o_sally_contextuel = (uint32_t)AI_gpst_CurrentGameObject;
                *(uint32_t*)Lecteur_Code_lc_reflex_GST_Board_panel_gao_i_sally_priority = 0x40A00000;
                *(uint32_t*)Lecteur_Code_lc_reflex_GST_Board_panel_gao_i_sally_action = 4;
                *(uint32_t*)Lecteur_Code_lc_reflex_GST_Board_panel_gao_v_sally_offset = 0;
                *(uint32_t*)(Lecteur_Code_lc_reflex_GST_Board_panel_gao_v_sally_offset + 4) = 0;
                *(float*)(Lecteur_Code_lc_reflex_GST_Board_panel_gao_v_sally_offset + 8) = v25->f_offset_Z_regard;
                *(uint32_t*)Lecteur_Code_lc_reflex_GST_Board_panel_gao_i_force_interface = 1;
            }
        }
    }
    v28 = 0;
    if (AI_EvalFunc_MSGGetCount_C(AI_gpst_CurrentGameObject))
    {
        Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v17, Src, sizeof(v17));
        //DBG_OutputDebugString("Lecteur_code: msgid: %d\n", v17.msgID);
        // hack
        // v1 jo, de nincs cutscene
        //Lecteur_Code_lc_attente_code_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        //AI2C_ResolveBuffer(
            //&Lecteur_Code_lc_attente_code_univVars_Flag_speciaux_sauve,
            //(void*)Lecteur_Code_lc_attente_code_univVars,
            //4488);
        //AI_EvalFunc_ARRBitSet_C((int*)(Lecteur_Code_lc_attente_code_univVars_Flag_speciaux_sauve + 4), 2044); // open hangar door event test
        //AI_EvalFunc_ARRBitSet_C((int*)(Lecteur_Code_lc_attente_code_univVars_Flag_speciaux_sauve + 4), 2506); // no gyro test, plays double H advice 
        // v3, nem jo, mert nem aktivalja a palya tobbi reszet
        //tGameObjectMain* obj1 = (tGameObjectMain*)AI_ul_RealResolveOneRefObject(0x1f000119);
        //tGameObjectMain* obj2 = (tGameObjectMain*)AI_ul_RealResolveOneRefObject(0x1f00011a);
        //tGameObjectMain* obj3 = (tGameObjectMain*)AI_ul_RealResolveOneRefObject(0x1f00011b);       
        //AI_EvalFunc_OBJ_FlagInvisibleSet_C(obj1, 1);
        //AI_EvalFunc_OBJ_FlagInvisibleSet_C(obj2, 1);
        //AI_EvalFunc_OBJ_FlagInvisibleSet_C(obj3, 1);
        // v3 nem jo
        //tGameObjectMain* keyb = (tGameObjectMain*)AI_ul_RealResolveOneRefObject(0xd90071fb);
        //AI_EvalFunc_OBJ_CapaSet_C(keyb, 0x400, 0);
        // hack
        if (v17.msgID == 10 && (*(uint32_t *)&v17.f2) == 0x40A00000)
        {
            v6 = v15;
            if (v15)
            {
                if ((unsigned int)v15 <= 2)
                    v6 = AI_MainActor[(uint32_t)v15 - 1];
                v5 = v6;
            }
            else
            {
                v5 = 0;
            }
            if (v5 == (tGameObjectMain*)Lecteur_Code_lc_reflex__PJ_Sally_gao)
            {
                v17.msgID = 57;
                v17.gao = AI_gpst_CurrentGameObject;
                v4 = v15;
                if (v15)
                {
                    if ((unsigned int)v15 <= 2)
                        v4 = AI_MainActor[(uint32_t)v15 - 1];
                    AI_EvalFunc_MSGSend_C(v4, &v17);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &v17);
                }
            }
            else
            {
                v28 = 1;
            }
        }
        else if (v17.msgID == 57)
        {
            v28 = 1;
        }
    }
    if (!v28)
        return 1000;
    *(uint32_t*)Lecteur_Code_lc_reflex_INV_manager_gao_i_CodeUsableFirst = v25->des_i_code;
    *(uint32_t*)Lecteur_Code_lc_reflex_INV_manager_gao_i_CodeUsableLast = v25->des_i_code;
    if (v25->des_i_code == 11)
        *(uint32_t*)Lecteur_Code_lc_reflex_INV_manager_gao_i_OpeningMode = 8;
    else
        *(uint32_t*)Lecteur_Code_lc_reflex_INV_manager_gao_i_OpeningMode = 1;
    AI_EvalFunc_OBJ_CapaSet_C((tGameObjectMain*)Lecteur_Code_lc_reflex_GST_Board_panel_gao, 0x1000, 0);
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Lecteur_Code_lc_reflex_Lecteur_Code_lc_attente_code);
    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
    return 1000;
}

int Lecteur_Code_lc_attente_code()
{
    int result; // eax
    tScriptStruct_LecteurCode* v1; // [esp+0h] [ebp-40h]
    tGameObjectMain* v2; // [esp+10h] [ebp-30h]
    tGameObjectMain* v3; // [esp+14h] [ebp-2Ch]
    tGameObjectMain* v4; // [esp+18h] [ebp-28h]
    int v5; // [esp+30h] [ebp-10h]
    BOOL v6; // [esp+34h] [ebp-Ch]
    int i; // [esp+3Ch] [ebp-4h]
    int j; // [esp+3Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_LecteurCode*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Lecteur_Code_lc_attente_code_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &Lecteur_Code_lc_attente_code_INV_manager_gao_i_OpeningMode,
            (void*)Lecteur_Code_lc_attente_code_INV_manager_gao,
            100);
        Lecteur_Code_lc_attente_code_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Lecteur_Code_lc_attente_code_univVars_Flag_speciaux_sauve,
            (void*)Lecteur_Code_lc_attente_code_univVars,
            4488);
        AI2C_ResolveBuffer(
            &Lecteur_Code_lc_attente_code_univVars_f_sally_fric,
            (void*)Lecteur_Code_lc_attente_code_univVars,
            1200);
        AI2C_ResolveBuffer(
            &Lecteur_Code_lc_attente_code_INV_manager_gao_i_Code,
            (void*)Lecteur_Code_lc_attente_code_INV_manager_gao,
            22820);
        AI2C_ResolveBuffer(
            &Lecteur_Code_lc_attente_code_INV_manager_gao_i_Allocated,
            (void*)Lecteur_Code_lc_attente_code_INV_manager_gao,
            156);
        fct_Lecteur_Code_lc_attente_code_Lecteur_Code_lc_reflex = (int)AI_ul_RealResolveOneRefFunction(0x9E0035B8);
    }
    if (*(uint32_t*)Lecteur_Code_lc_attente_code_INV_manager_gao_i_OpeningMode)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        v5 = 0;
        if (v1->des_i_code == 11)
        {
            for (i = 0; i < 5; ++i)
            {
                if (i + v1->des_SF_Indice_1er_Journal <= 0)
                    v6 = 0;
                else
                    v6 = AI_EvalFunc_ARRBitTest_C(
                        (int*)(Lecteur_Code_lc_attente_code_univVars_Flag_speciaux_sauve + 4),
                        i + v1->des_SF_Indice_1er_Journal);
                if (v6)
                {
                    if (i + 2272 > 0)
                        AI_EvalFunc_ARRBitSet_C((int*)(Lecteur_Code_lc_attente_code_univVars_Flag_speciaux_sauve + 4), i + 2272);
                    *(uint32_t*)Lecteur_Code_lc_attente_code_univVars_f_sally_fric -= v1->prix_abonnement;
                }
            }
            for (j = 0; j < 5; ++j)
            {
                if (j + v1->des_SF_Indice_1er_Journal > 0)
                    AI_EvalFunc_ARRBitClear_C(
                        (int*)(Lecteur_Code_lc_attente_code_univVars_Flag_speciaux_sauve + 4),
                        j + v1->des_SF_Indice_1er_Journal);
            }
        }
        if (*(uint32_t*)Lecteur_Code_lc_attente_code_INV_manager_gao_i_Code == v1->des_i_code)
            v5 = 1;
        if (v5)
        {
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x100, 0);
            v4 = (tGameObjectMain*)v1->o_other_capa;
            if (v4)
            {
                if ((unsigned int)v4 <= 2)
                    v4 = AI_MainActor[(uint32_t)v4 - 1];
                v3 = v4;
            }
            else
            {
                v3 = 0;
            }
            if (v3)
            {
                v2 = (tGameObjectMain*)v1->o_other_capa;
                if (v2)
                {
                    if ((unsigned int)v2 <= 2)
                        v2 = AI_MainActor[(uint32_t)v2 - 1];
                    AI_EvalFunc_OBJ_CapaSet_C(v2, 0x100, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_CapaSet_C(0, 0x100, 0);
                }
            }
            if (v1->des_SF_valide >= 10000)
            {
                v1->des_SF_valide -= 10000;
                if (v1->des_SF_valide > 0)
                    AI_EvalFunc_ARRBitClear_C(
                        (int*)(Lecteur_Code_lc_attente_code_univVars_Flag_speciaux_sauve + 4),
                        v1->des_SF_valide);
            }
            else if (v1->des_SF_valide > 0)
            {
                AI_EvalFunc_ARRBitSet_C(
                    (int*)(Lecteur_Code_lc_attente_code_univVars_Flag_speciaux_sauve + 4),
                    v1->des_SF_valide);
            }
        }
        *(uint32_t*)Lecteur_Code_lc_attente_code_INV_manager_gao_i_Code = -1;
        *(uint32_t*)Lecteur_Code_lc_attente_code_INV_manager_gao_i_Allocated = 0;
        AI_EvalFunc_MSGClear_C(AI_gpst_CurrentGameObject);
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Lecteur_Code_lc_attente_code_Lecteur_Code_lc_reflex);
        result = 1000;
    }
    return result;
}