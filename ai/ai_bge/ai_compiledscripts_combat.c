#include "ai_compiledscripts_inc.h"

//GST_Script_Combat_GST_Init
int GST_Script_Combat_GST_Init_univVars;
int GST_Script_Combat_GST_Init_univVars_Flag_speciaux_sauve;
int GST_Script_Combat_GST_Init__PJ_Sally_gao;
int GST_Script_Combat_GST_Init__PJ_Sally_gao_i_Fight_dymmy;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_AChoixPerso;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_AForceComportement;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_TestContinue;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_Pause;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_Label;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_Goto;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_ActionAlternee;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_ActiveCP;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_ActionEnchaine;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_ExecMacroSimul;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_ExecMacroAlterne;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_DesactiveCP;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_ComportementSpeciaux;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_SendMessage;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_CP_Minumum;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_CP_AttendPaf;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_CP_FightSallyOn;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_CP_RefreshActors;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_Sarko;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_Sauterelle;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_Crevette;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_DireWorm;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_Tikha;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_CaveCrawler;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_Spectre;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_SpectreBas;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_SpectrePlus;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_SpectreProche;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_SpectreContinu;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_ConditionPermanente;
int fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_main;

//GST_Script_Combat
int GST_Script_Combat_GST_main_univVars;
int GST_Script_Combat_GST_main_univVars_Flag_speciaux_sauve;
int GST_Script_Combat_GST_main__PJ_Sally_gao;
int GST_Script_Combat_GST_main_univVars_Flag_speciaux_sauve_i_Fight_dymmy;

//GST_Script_Combat_GST_ComportementSpeciaux
int fct_GST_Script_Combat_GST_ComportementSpeciaux__PJ_Sally_gao;
int fct_GST_Script_Combat_GST_ComportementSpeciaux_GST_Script_Combat_GST_Macro_ExecMacroAlterne;

//GST_Script_Combat_GST_Macro_AChoixPerso
int GST_Script_Combat_GST_Macro_AChoixPerso__PJ_Sally_gao;
int GST_Script_Combat_GST_Macro_AChoixPerso__PJ_Camera_gao;

//GST_Script_Combat_GST_CP_Minumum
int GST_Script_Combat_GST_CP_Minumum_GST_Fight_Manager_gao;
int GST_Script_Combat_GST_CP_Minumum_GST_Fight_Manager_gao_i_Evil_NbActor;

//GST_Script_Combat_GST_CP_FightSallyOn
int GST_Script_Combat_GST_CP_FightSallyOn__PJ_Sally_gao;
int GST_Script_Combat_GST_CP_FightSallyOn__PJ_Sally_gao_i_Fight_dymmy;

// =================================

//Monstroplante_Gamin_MPG_init
int Monstroplante_Gamin_MPG_init__PJ_Sally_gao;
int fct_Monstroplante_Gamin_MPG_init_Monstroplante_Gamin_MPG_Jump;

//Monstroplante_Gamin_MPG_Jump
int fct_Monstroplante_Gamin_MPG_Jump_Monstroplante_Gamin_MPG_run;

int GST_Script_Combat_GST_Init()
{
    int v0; // eax
    tScriptStruct_GSTCombat* v2; // [esp+Ch] [ebp-B8h]
    tGameObjectMain* v3; // [esp+18h] [ebp-ACh]
    tGameObjectMain* v4; // [esp+1Ch] [ebp-A8h]
    tGameObjectMain* v5; // [esp+20h] [ebp-A4h]
    tGameObjectMain* v6; // [esp+24h] [ebp-A0h]
    tGameObjectMain* v7; // [esp+2Ch] [ebp-98h]
    tGameObjectMain* v8; // [esp+34h] [ebp-90h]
    tGameObjectMain* v9; // [esp+3Ch] [ebp-88h]
    tGameObjectMain* v10; // [esp+44h] [ebp-80h]
    tGameObjectMain* v11; // [esp+48h] [ebp-7Ch]
    tGameObjectMain* v12; // [esp+4Ch] [ebp-78h]
    tGameObjectMain* v13; // [esp+54h] [ebp-70h]
    tGameObjectMain* a2; // [esp+58h] [ebp-6Ch]
    tGameObjectMain* v15; // [esp+5Ch] [ebp-68h]
    tGameObjectMain* v16; // [esp+60h] [ebp-64h]
    tGameObjectMain* v17; // [esp+64h] [ebp-60h]
    int v18; // [esp+70h] [ebp-54h]
    int v19; // [esp+7Ch] [ebp-48h]
    int v20; // [esp+84h] [ebp-40h]
    int v21; // [esp+8Ch] [ebp-38h]
    int v22; // [esp+90h] [ebp-34h]
    int v23; // [esp+94h] [ebp-30h]
    int v24; // [esp+9Ch] [ebp-28h]
    int v25; // [esp+A0h] [ebp-24h]
    int v26; // [esp+A4h] [ebp-20h]
    BOOL v28; // [esp+B4h] [ebp-10h]
    int i; // [esp+B4h] [ebp-10h]
    int v30; // [esp+B8h] [ebp-Ch]
    int v31; // [esp+C0h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        GST_Script_Combat_GST_Init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &GST_Script_Combat_GST_Init_univVars_Flag_speciaux_sauve,
            (void*)GST_Script_Combat_GST_Init_univVars,
            4488);
        GST_Script_Combat_GST_Init__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(
            &GST_Script_Combat_GST_Init__PJ_Sally_gao_i_Fight_dymmy,
            (void*)GST_Script_Combat_GST_Init__PJ_Sally_gao,
            4240);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_AChoixPerso = (int)AI_ul_RealResolveOneRefFunction(0x32000037u);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_AForceComportement = (int)AI_ul_RealResolveOneRefFunction(0x3200003Cu);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_TestContinue = (int)AI_ul_RealResolveOneRefFunction(0x3200003Du);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_Pause = (int)AI_ul_RealResolveOneRefFunction(0x9E0060C4);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_Label = (int)AI_ul_RealResolveOneRefFunction(0x9E0060C6);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_Goto = (int)AI_ul_RealResolveOneRefFunction(0x9E0060C8);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_ActionAlternee = (int)AI_ul_RealResolveOneRefFunction(0x9E0060CA);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_ActiveCP = (int)AI_ul_RealResolveOneRefFunction(0x9E0060D9);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_ActionEnchaine = (int)AI_ul_RealResolveOneRefFunction(0x9E0060DB);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_ExecMacroSimul = (int)AI_ul_RealResolveOneRefFunction(0x9E006135);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_ExecMacroAlterne = (int)AI_ul_RealResolveOneRefFunction(0x9E00614C);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_DesactiveCP = (int)AI_ul_RealResolveOneRefFunction(0x9E006150);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_ComportementSpeciaux = (int)AI_ul_RealResolveOneRefFunction(0x9E006183);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_SendMessage = (int)AI_ul_RealResolveOneRefFunction(0x9E006465);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_CP_Minumum = (int)AI_ul_RealResolveOneRefFunction(0x9E0060CC);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_CP_AttendPaf = (int)AI_ul_RealResolveOneRefFunction(0x9E0060D7);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_CP_FightSallyOn = (int)AI_ul_RealResolveOneRefFunction(0x9E00645C);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_CP_RefreshActors = (int)AI_ul_RealResolveOneRefFunction(0x9E006BD7);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_Sarko = (int)AI_ul_RealResolveOneRefFunction(0x9E006159);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_Sauterelle = (int)AI_ul_RealResolveOneRefFunction(0x9E0063C3);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_Crevette = (int)AI_ul_RealResolveOneRefFunction(0x9E0063C2);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_DireWorm = (int)AI_ul_RealResolveOneRefFunction(0x9E006443);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_Tikha = (int)AI_ul_RealResolveOneRefFunction(0x9E006555);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_CaveCrawler = (int)AI_ul_RealResolveOneRefFunction(0x9E006629);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_Spectre = (int)AI_ul_RealResolveOneRefFunction(0x9E0066CA);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_SpectreBas = (int)AI_ul_RealResolveOneRefFunction(0x9E006818);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_SpectrePlus = (int)AI_ul_RealResolveOneRefFunction(0x9E006859);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_SpectreProche = (int)AI_ul_RealResolveOneRefFunction(0x9E006890);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_SpectreContinu = (int)AI_ul_RealResolveOneRefFunction(0x9E006CAB);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_ConditionPermanente = (int)AI_ul_RealResolveOneRefFunction(0x9E0060C1);
        fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_main = (int)AI_ul_RealResolveOneRefFunction(0x9E0060C2);
    }
    if (*AI_gppst_CurrentJumpNode != (tAINode*)10)
    {
        if (v2->SF_END)
        {
            if (v2->SF_END > 0
                && AI_EvalFunc_ARRBitTest_C((int*)(GST_Script_Combat_GST_Init_univVars_Flag_speciaux_sauve + 4), v2->SF_END))
            {
                DBG_OutputDebugString("SF end hit: %x\n", v2->SF_END);
                AI_EvalFunc_OBJ_FlagInactiveSet_C(AI_gpst_CurrentGameObject, 1);
            }
        }
        if (v2->SF_INIT)
        {
            if (v2->SF_INIT <= 0)
                v28 = 0;
            else
                v28 = AI_EvalFunc_ARRBitTest_C(
                    (int*)(GST_Script_Combat_GST_Init_univVars_Flag_speciaux_sauve + 4),
                    v2->SF_INIT);
        }
        else
        {
            v28 = 1;
        }
        v17 = (tGameObjectMain*)v2->o_ZONE_Fight;
        if (v2->o_ZONE_Fight)
        {
            if ((unsigned int)v17 <= 2)
                v17 = AI_MainActor[(uint32_t)v17 - 1];
            v16 = v17;
        }
        else
        {
            v16 = 0;
        }
        if (!v16)
            v2->o_ZONE_Fight = (int)AI_gpst_CurrentGameObject;
        if (!v28)
            goto LABEL_38;
        DBG_OutputDebugString("SF init hit: %x\n", v2->SF_INIT);
        v15 = (tGameObjectMain*)GST_Script_Combat_GST_Init__PJ_Sally_gao;
        if (GST_Script_Combat_GST_Init__PJ_Sally_gao)
        {
            if ((unsigned int)GST_Script_Combat_GST_Init__PJ_Sally_gao <= 2)
                v15 = AI_MainActor[GST_Script_Combat_GST_Init__PJ_Sally_gao - 1];
            a2 = v15;
        }
        else
        {
            a2 = 0;
        }
        v13 = (tGameObjectMain*)v2->o_ZONE_Fight;
        if (v2->o_ZONE_Fight)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            v0 = AI_EvalFunc_COLBVPivotCollide_C(v13, a2);
        }
        else
        {
            v0 = AI_EvalFunc_COLBVPivotCollide_C(0, a2);
        }
        if (!v0 || AI_EvalFunc_OBJ_CapaTest_C_CURRENT(1))
        {
        LABEL_38:
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v2->f_time_wait = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    if (!AI_EvalFunc_TIMEElapsed_C(v2->f_time_wait, v2->f_time_delai_avant_debut))
    {
        *(uint32_t*)GST_Script_Combat_GST_Init__PJ_Sally_gao_i_Fight_dymmy = 1;
        *AI_gppst_CurrentJumpNode = (tAINode*)10;
        return 0;
    }
    *(uint32_t*)GST_Script_Combat_GST_Init__PJ_Sally_gao_i_Fight_dymmy = 1;
    for (i = 0; i < 30; ++i)
    {
        v12 = (tGameObjectMain*)v2->Actors[i];
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
        if (v2->i_active_actor_on_init)
        {
            v10 = (tGameObjectMain*)v2->Actors[i];
            if (v10)
            {
                if ((unsigned int)v10 <= 2)
                    v10 = AI_MainActor[(uint32_t)v10 - 1];
                AI_EvalFunc_OBJReinit_C(v10, 1);
            }
            else
            {
                AI_EvalFunc_OBJReinit_C(0, 1);
            }
            v9 = (tGameObjectMain*)v2->Actors[i];
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v9, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
            }
            if (v2->i_invisible_actor_on_init)
            {
                v8 = (tGameObjectMain*)v2->Actors[i];
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
            }
            else
            {
                v7 = (tGameObjectMain*)v2->Actors[i];
                if (v7)
                {
                    if ((unsigned int)v7 <= 2)
                        v7 = AI_MainActor[(uint32_t)v7 - 1];
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(v7, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
                }
            }
        }
        v6 = (tGameObjectMain*)v2->Actors[i];
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
        v5->extended->fields->designInt2 = 0;
        v4 = (tGameObjectMain*)v2->Actors[i];
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
        v3->extended->fields->designGO2 = (int)AI_gpst_CurrentGameObject;
    }
    *(uint32_t*)AI_EvalVarArray_C(5, (char*)&v2->__Scr_Function_Dimension1, 1) = fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_AChoixPerso;
    *(uint32_t*)AI_EvalVarArray_C(5, (char*)&v2->__Scr_Function_Dimension1, 2) = fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_AForceComportement;
    *(uint32_t*)AI_EvalVarArray_C(5, (char*)&v2->__Scr_Function_Dimension1, 3) = fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_TestContinue;
    *(uint32_t*)AI_EvalVarArray_C(5, (char*)&v2->__Scr_Function_Dimension1, 4) = fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_Pause;
    *(uint32_t*)AI_EvalVarArray_C(5, (char*)&v2->__Scr_Function_Dimension1, 5) = fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_Label;
    *(uint32_t*)AI_EvalVarArray_C(5, (char*)&v2->__Scr_Function_Dimension1, 6) = fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_Goto;
    *(uint32_t*)AI_EvalVarArray_C(5, (char*)&v2->__Scr_Function_Dimension1, 7) = fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_ActionAlternee;
    *(uint32_t*)AI_EvalVarArray_C(5, (char*)&v2->__Scr_Function_Dimension1, 8) = fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_ActiveCP;
    *(uint32_t*)AI_EvalVarArray_C(5, (char*)&v2->__Scr_Function_Dimension1, 9) = fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_ActionEnchaine;
    *(uint32_t*)AI_EvalVarArray_C(5, (char*)&v2->__Scr_Function_Dimension1, 10) = fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_ExecMacroSimul;
    *(uint32_t*)AI_EvalVarArray_C(5, (char*)&v2->__Scr_Function_Dimension1, 11) = fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_ExecMacroAlterne;
    *(uint32_t*)AI_EvalVarArray_C(5, (char*)&v2->__Scr_Function_Dimension1, 12) = fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_DesactiveCP;
    *(uint32_t*)AI_EvalVarArray_C(5, (char*)&v2->__Scr_Function_Dimension1, 13) = fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_ComportementSpeciaux;
    *(uint32_t*)AI_EvalVarArray_C(5, (char*)&v2->__Scr_Function_Dimension1, 14) = fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Macro_SendMessage;
    *(uint32_t*)AI_EvalVarArray_C(5, (char*)&v2->__Scr_FunctionCondition_Dimension1, 1) = fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_CP_Minumum;
    *(uint32_t*)AI_EvalVarArray_C(5, (char*)&v2->__Scr_FunctionCondition_Dimension1, 2) = fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_CP_AttendPaf;
    *(uint32_t*)AI_EvalVarArray_C(5, (char*)&v2->__Scr_FunctionCondition_Dimension1, 3) = fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_CP_FightSallyOn;
    *(uint32_t*)AI_EvalVarArray_C(5, (char*)&v2->__Scr_FunctionCondition_Dimension1, 4) = fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_CP_RefreshActors;
    v2->i_Macro_Ind[0] = -1;
    v2->Scr_Labels[0] = -1;
    v2->Scr_Label_Indice = 0;
    switch (v2->i_script_indice)
    {
    case 1:
        v26 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_Sarko);
        if (v26 != 1000)
            return v26;
        break;
    case 2:
        v30 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_Sauterelle);
        if (v30 != 1000)
            return v30;
        break;
    case 3:
        v21 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_Crevette);
        if (v21 != 1000)
            return v21;
        break;
    case 4:
        v25 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_DireWorm);
        if (v25 != 1000)
            return v25;
        break;
    case 5:
        v19 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_Tikha);
        if (v19 != 1000)
            return v19;
        break;
    case 6:
        v31 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_CaveCrawler);
        if (v31 != 1000)
            return v31;
        break;
    case 7:
        v24 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_Spectre);
        if (v24 != 1000)
            return v24;
        break;
    case 8:
        v20 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_SpectreBas);
        if (v20 != 1000)
            return v20;
        break;
    case 9:
        v22 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_SpectrePlus);
        if (v22 != 1000)
            return v22;
        break;
    case 10:
        v18 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_SpectreProche);
        if (v18 != 1000)
            return v18;
        break;
    case 11:
        v23 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_Script_Order_SpectreContinu);
        if (v23 != 1000)
            return v23;
        break;
    }
    AI_EvalFunc_AITrackChange_C(
        1,
        (tAIFunction*)fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_ConditionPermanente);
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_GST_Script_Combat_GST_Init_GST_Script_Combat_GST_main);
    return 1000;
}

int GST_Script_Combat_GST_main()
{
    int result; // eax
    tAIFunction** v1; // eax
    tScriptStruct_GSTCombat* v2; // [esp+0h] [ebp-3Ch]
    tGameObjectMain* v3; // [esp+8h] [ebp-34h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-30h]
    tGameObjectMain* v5; // [esp+10h] [ebp-2Ch]
    tGameObjectMain* v6; // [esp+1Ch] [ebp-20h]
    tGameObjectMain* v7; // [esp+20h] [ebp-1Ch]
    tGameObjectMain* v8; // [esp+24h] [ebp-18h]
    int v9; // [esp+30h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        GST_Script_Combat_GST_main_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &GST_Script_Combat_GST_main_univVars_Flag_speciaux_sauve,
            (void*)GST_Script_Combat_GST_main_univVars,
            4488);
        GST_Script_Combat_GST_main__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(
            &GST_Script_Combat_GST_main_univVars_Flag_speciaux_sauve_i_Fight_dymmy,
            (void*)GST_Script_Combat_GST_main__PJ_Sally_gao,
            4240);
    }
    if (v2->SF_END)
    {
        if (v2->SF_END > 0
            && AI_EvalFunc_ARRBitTest_C((int*)(GST_Script_Combat_GST_main_univVars_Flag_speciaux_sauve + 4), v2->SF_END))
        {
            DBG_OutputDebugString("SF end hit (main): %x\n", v2->SF_END);
            AI_EvalFunc_OBJ_FlagInactiveSet_C(AI_gpst_CurrentGameObject, 1);
        }
    }
    if (v2->Script_nb_de_refresh && v2->Scr_nb_mort > v2->Script_nb_de_refresh)
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(2, 0);
    if (v2->i_nb_NMI_restant_pour_activer != -1
        && v2->i_choix_perso_done
        && v2->Scr_Actor_Nbr <= v2->i_nb_NMI_restant_pour_activer)
    {
        v8 = (tGameObjectMain*)v2->o_chain_script;
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
            v6 = (tGameObjectMain*)v2->o_chain_script;
            if (v6)
            {
                if ((unsigned int)v6 <= 2)
                    v6 = AI_MainActor[(uint32_t)v6 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v6, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
            }
        }
        v2->o_chain_script = 0;
    }
    if (v2->Scr_Indice < v2->Scr_Labels[19])
    {
        if (v2->Scr_Indice >= 50
            || (v1 = (tAIFunction**)AI_EvalVarArray_C(
                5,
                (char*)&v2->__Scr_Function_Dimension1,
                v2->Scr_ScriptFunction[v2->Scr_Indice]),
                v9 = AI_EvalFunc_AIExecute_C(*v1),
                v9 == 1000))
        {
            result = 1000;
        }
        else
        {
            result = v9;
        }
    }
    else
    {
        *(uint32_t*)GST_Script_Combat_GST_main_univVars_Flag_speciaux_sauve_i_Fight_dymmy = 1;
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0x100, 0);
        v5 = (tGameObjectMain*)v2->o_chain_script;
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
            v3 = (tGameObjectMain*)v2->o_chain_script;
            if (v3)
            {
                if ((unsigned int)v3 <= 2)
                    v3 = AI_MainActor[(uint32_t)v3 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v3, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
            }
        }
        AI_EvalFunc_OBJ_FlagInactiveSet_C(AI_gpst_CurrentGameObject, 1);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int GST_Script_Combat_GST_ConditionPermanente()
{
    tAIFunction** v0; // eax
    tScriptStruct_GSTCombat* v2; // [esp+0h] [ebp-10h]
    int v3; // [esp+4h] [ebp-Ch]
    int i; // [esp+Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    for (v2->Scr_Condition_Indice = 0; v2->Scr_Condition_Indice < v2->Scr_Condition_Nb; ++v2->Scr_Condition_Indice)
    {
        if (v2->Scr_ConditionOn[v2->Scr_Condition_Indice])
        {
            v0 = (tAIFunction**)AI_EvalVarArray_C(
                5,
                (char*)&v2->__Scr_FunctionCondition_Dimension1,
                v2->Scr_Conditions[v2->Scr_Condition_Indice]);
            v3 = AI_EvalFunc_AIExecute_C(*v0);
            if (v3 != 1000)
                return v3;
        }
    }
    v2->Scr_Indice_Old = v2->Scr_Indice;
    for (i = 0; i < 20 && v2->Scr_Indice >= v2->Scr_Labels[i]; ++i)
        v2->Scr_Label_Indice = i;
    return 1000;
}

int GST_Script_Combat_GST_Macro_Pause()
{
    float* v0; // eax
    float v2; // [esp+0h] [ebp-48h]
    float v3; // [esp+4h] [ebp-44h]
    float v4; // [esp+8h] [ebp-40h]
    float v5; // [esp+Ch] [ebp-3Ch]
    float v6; // [esp+10h] [ebp-38h]
    float v7; // [esp+14h] [ebp-34h]
    tScriptStruct_GSTCombat* v8; // [esp+18h] [ebp-30h]
    double v9; // [esp+28h] [ebp-20h]
    float v10; // [esp+38h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v8 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v8 = 0;
    if (!v8->Scr_TimeWait[0])
    {
        v10 = TIM_gf_MainClock - TIM_gf_LockTime;
        v9 = v10 + MATH_gd_magicDoubleForRound;
        v8->Scr_TimeWait[0] = (*(uint32_t *)&v9);
        v7 = *(float*)AI_EvalVarArray2_C(2, (char*)&v8->__Scr_ScriptParam_Dimension1, 0, v8->Scr_Indice);
        v3 = *(float*)AI_EvalVarArray2_C(2, (char*)&v8->__Scr_ScriptParam_Dimension1, 1, v8->Scr_Indice) + v7;
        v6 = *(float*)AI_EvalVarArray2_C(2, (char*)&v8->__Scr_ScriptParam_Dimension1, 0, v8->Scr_Indice);
        v2 = v6 - *(float*)AI_EvalVarArray2_C(2, (char*)&v8->__Scr_ScriptParam_Dimension1, 1, v8->Scr_Indice);
        v5 = AI_EvalFunc_MATHRandFloat_C(v2, v3);
        *(float*)AI_EvalVarArray2_C(2, (char*)&v8->__Scr_ScriptParam_Dimension1, 2, v8->Scr_Indice) = v5;
    }
    v0 = (float*)AI_EvalVarArray2_C(2, (char*)&v8->__Scr_ScriptParam_Dimension1, 2, v8->Scr_Indice);
    v4 = (float)v8->Scr_TimeWait[0];
    if (AI_EvalFunc_TIMEElapsed_C(v4, *v0))
    {
        v8->Scr_TimeWait[0] = 0;
        ++v8->Scr_Indice;
    }
    return 1000;
}

int GST_Script_Combat_GST_Macro_Label()
{
    tScriptStruct_GSTCombat* v1; // [esp+0h] [ebp-18h]
    double v2; // [esp+8h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v2 = *(float*)AI_EvalVarArray2_C(2, (char*)&v1->__Scr_ScriptParam_Dimension1, 0, v1->Scr_Indice) + MATH_gd_magicDoubleForRound;
    v1->Scr_Labels[(*(uint32_t *)&v2)] = ++v1->Scr_Indice;
    return 1000;
}

int GST_Script_Combat_GST_Script_Order_Sauterelle()
{
    int v1; // [esp+4h] [ebp-18h]
    int v2; // [esp+8h] [ebp-14h]
    tScriptStruct_GSTCombat* v3; // [esp+Ch] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v3->__Scr_Macro_Dimension1, 0, 0) = 0;
    v3->Scr_ScriptFunction[0] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 0) = 0x41000000;
    v3->Scr_ScriptFunction[1] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 1) = 0x42800000;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v3->__Scr_Macro_Dimension1, 1, 0) = 2;
    v3->Scr_Indice = 2;
    v3->i_label = 0;
    v3->Scr_Labels[++v3->i_label] = 2;
    v3->Scr_Conditions[v3->Scr_Condition_Nb] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 0, v3->Scr_Condition_Nb) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 1, v3->Scr_Condition_Nb) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 2, v3->Scr_Condition_Nb) = 0x41980000;
    v3->Scr_ConditionOn[v3->Scr_Condition_Nb++] = 0;
    v3->Scr_ScriptFunction[2] = 8;
    v2 = v3->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 2) = (float)v2;
    v3->Scr_Conditions[v3->Scr_Condition_Nb] = 3;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 0, v3->Scr_Condition_Nb) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 1, v3->Scr_Condition_Nb) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 2, v3->Scr_Condition_Nb) = 0;
    v3->Scr_ConditionOn[v3->Scr_Condition_Nb++] = 0;
    v3->Scr_ScriptFunction[3] = 8;
    v1 = v3->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 3) = (float)v1;
    v3->Scr_ScriptFunction[4] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 4) = 0x41200000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 1, 4) = 0x40800000;
    v3->Scr_Labels[++v3->i_label] = 5;
    v3->Scr_ScriptFunction[5] = 2;
    v3->Scr_ScriptFunction[5] = 10;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 5) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 1, 5) = 0;
    v3->Scr_ScriptFunction[6] = 6;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 6) = 0x40000000;
    v3->Scr_Labels[19] = 7;
    return 1000;
}

int GST_Script_Combat_GST_Script_Order_Spectre()
{
    int v1; // [esp+4h] [ebp-24h]
    int v2; // [esp+8h] [ebp-20h]
    int v3; // [esp+Ch] [ebp-1Ch]
    int v4; // [esp+10h] [ebp-18h]
    int v5; // [esp+14h] [ebp-14h]
    tScriptStruct_GSTCombat* v6; // [esp+18h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v6->__Scr_Macro_Dimension1, 0, 0) = 0;
    v6->Scr_ScriptFunction[0] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 0) = 0x41000000;
    v6->Scr_ScriptFunction[1] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 1) = 0x40000000;
    v6->Scr_ScriptFunction[2] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 2) = 0x40000000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 2) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v6->__Scr_Macro_Dimension1, 1, 0) = 3;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v6->__Scr_Macro_Dimension1, 0, 1) = 3;
    v6->Scr_ScriptFunction[3] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 3) = 0x41000000;
    v6->Scr_ScriptFunction[4] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 4) = 0x40000000;
    v6->Scr_ScriptFunction[5] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 5) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 5) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v6->__Scr_Macro_Dimension1, 1, 1) = 6;
    v6->Scr_Indice = 6;
    v6->i_label = 0;
    v6->Scr_Labels[++v6->i_label] = 6;
    v6->Scr_Conditions[v6->Scr_Condition_Nb] = 3;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ConditionParam_Dimension1, 0, v6->Scr_Condition_Nb) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ConditionParam_Dimension1, 1, v6->Scr_Condition_Nb) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ConditionParam_Dimension1, 2, v6->Scr_Condition_Nb) = 0;
    v6->Scr_ConditionOn[v6->Scr_Condition_Nb++] = 0;
    v6->Scr_ScriptFunction[6] = 8;
    v5 = v6->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 6) = (float)v5;
    v6->Scr_ScriptFunction[7] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 7) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 7) = 0;
    v6->Scr_ScriptFunction[8] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 8) = 0x40000000;
    v6->Scr_Conditions[v6->Scr_Condition_Nb] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ConditionParam_Dimension1, 0, v6->Scr_Condition_Nb) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ConditionParam_Dimension1, 1, v6->Scr_Condition_Nb) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ConditionParam_Dimension1, 2, v6->Scr_Condition_Nb) = 0x40400000;
    v6->Scr_ConditionOn[v6->Scr_Condition_Nb++] = 0;
    v6->Scr_ScriptFunction[9] = 8;
    v4 = v6->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 9) = (float)v4;
    v6->Scr_ScriptFunction[10] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 10) = 1065353216;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 10) = 0;
    v6->Scr_Labels[++v6->i_label] = 11;
    v6->Scr_ScriptFunction[11] = 7;
    v6->Scr_ScriptFunction[11] = 11;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 11) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 11) = 0;
    v6->Scr_ScriptFunction[12] = 6;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 12) = 0x40000000;
    v6->Scr_Labels[++v6->i_label] = 13;
    v6->Scr_ScriptFunction[13] = 12;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 13) = 0x3F800000;
    v6->Scr_ScriptFunction[14] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 14) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 14) = 0;
    v6->Scr_ScriptFunction[15] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 15) = 0x40000000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 15) = 0;
    v6->Scr_ScriptFunction[16] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 16) = 0x40000000;
    v6->Scr_Conditions[v6->Scr_Condition_Nb] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ConditionParam_Dimension1, 0, v6->Scr_Condition_Nb) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ConditionParam_Dimension1, 1, v6->Scr_Condition_Nb) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ConditionParam_Dimension1, 2, v6->Scr_Condition_Nb) = 0x40A00000;
    v6->Scr_ConditionOn[v6->Scr_Condition_Nb++] = 0;
    v6->Scr_ScriptFunction[17] = 8;
    v3 = v6->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 17) = (float)v3;
    v6->Scr_ScriptFunction[18] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 18) = 0x3FC00000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 18) = 0;
    v6->Scr_Labels[++v6->i_label] = 19;
    v6->Scr_ScriptFunction[19] = 7;
    v6->Scr_ScriptFunction[19] = 11;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 19) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 19) = 0;
    v6->Scr_ScriptFunction[20] = 6;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 20) = 0x40800000;
    v6->Scr_Labels[++v6->i_label] = 21;
    v6->Scr_ScriptFunction[21] = 12;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 21) = 0x40000000;
    v6->Scr_ScriptFunction[22] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 22) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 22) = 0;
    v6->Scr_ScriptFunction[23] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 23) = 0x40000000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 23) = 0;
    v6->Scr_ScriptFunction[24] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 24) = 0x40000000;
    v6->Scr_Conditions[v6->Scr_Condition_Nb] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ConditionParam_Dimension1, 0, v6->Scr_Condition_Nb) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ConditionParam_Dimension1, 1, v6->Scr_Condition_Nb) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ConditionParam_Dimension1, 2, v6->Scr_Condition_Nb) = 0x40E00000;
    v6->Scr_ConditionOn[v6->Scr_Condition_Nb++] = 0;
    v6->Scr_ScriptFunction[25] = 8;
    v2 = v6->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 25) = (float)v2;
    v6->Scr_ScriptFunction[26] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 26) = 0x3FC00000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 26) = 0;
    v6->Scr_Labels[++v6->i_label] = 27;
    v6->Scr_ScriptFunction[27] = 7;
    v6->Scr_ScriptFunction[27] = 11;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 27) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 27) = 0;
    v6->Scr_ScriptFunction[28] = 6;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 28) = 0x40C00000;
    v6->Scr_Labels[++v6->i_label] = 29;
    v6->Scr_ScriptFunction[29] = 12;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 29) = 0x40400000;
    v6->Scr_ScriptFunction[30] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 30) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 30) = 0;
    v6->Scr_ScriptFunction[31] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 31) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 31) = 0;
    v6->Scr_Conditions[v6->Scr_Condition_Nb] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ConditionParam_Dimension1, 0, v6->Scr_Condition_Nb) = 0x40800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ConditionParam_Dimension1, 1, v6->Scr_Condition_Nb) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ConditionParam_Dimension1, 2, v6->Scr_Condition_Nb) = 0x41980000;
    v6->Scr_ConditionOn[v6->Scr_Condition_Nb++] = 0;
    v6->Scr_ScriptFunction[32] = 8;
    v1 = v6->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 32) = (float)v1;
    v6->Scr_ScriptFunction[33] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 33) = 0x40000000;
    v6->Scr_ScriptFunction[34] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 34) = 0x3FC00000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 34) = 0;
    v6->Scr_Labels[++v6->i_label] = 35;
    v6->Scr_ScriptFunction[35] = 7;
    v6->Scr_ScriptFunction[35] = 11;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 35) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 35) = 0;
    v6->Scr_ScriptFunction[36] = 6;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 36) = 0x40C00000;
    v6->Scr_Labels[19] = 37;
    return 1000;
}

int GST_Script_Combat_GST_Script_Order_SpectrePlus()
{
    int v1; // [esp+4h] [ebp-1Ch]
    int v2; // [esp+8h] [ebp-18h]
    float v3; // [esp+Ch] [ebp-14h]
    tScriptStruct_GSTCombat* v4; // [esp+10h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v4->__Scr_Macro_Dimension1, 0, 0) = 0;
    v4->Scr_ScriptFunction[0] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 0) = 0x41000000;
    v4->Scr_ScriptFunction[1] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 1) = 0x40000000;
    v4->Scr_ScriptFunction[2] = 4;
    v3 = v4->Script_9_delai_entre_attaque;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 2) = v3;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 2) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v4->__Scr_Macro_Dimension1, 1, 0) = 3;
    v4->Scr_Indice = 3;
    v4->i_label = 0;
    v4->Scr_Labels[++v4->i_label] = 3;
    v4->Scr_Conditions[v4->Scr_Condition_Nb] = 3;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 0, v4->Scr_Condition_Nb) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 1, v4->Scr_Condition_Nb) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 2, v4->Scr_Condition_Nb) = 0;
    v4->Scr_ConditionOn[v4->Scr_Condition_Nb++] = 0;
    v4->Scr_ScriptFunction[3] = 8;
    v2 = v4->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 3) = (float)v2;
    v4->Scr_Labels[++v4->i_label] = 4;
    v4->Scr_ScriptFunction[4] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 4) = 0x41200000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 4) = 0x40800000;
    v4->Scr_ScriptFunction[5] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 5) = 0x40000000;
    v4->Scr_Conditions[v4->Scr_Condition_Nb] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 0, v4->Scr_Condition_Nb) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 1, v4->Scr_Condition_Nb) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 2, v4->Scr_Condition_Nb) = 0x41980000;
    v4->Scr_ConditionOn[v4->Scr_Condition_Nb++] = 0;
    v4->Scr_ScriptFunction[6] = 8;
    v1 = v4->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 6) = (float)v1;
    v4->Scr_Labels[++v4->i_label] = 7;
    v4->Scr_ScriptFunction[7] = 7;
    v4->Scr_ScriptFunction[7] = 11;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 7) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 7) = 0;
    v4->Scr_ScriptFunction[8] = 6;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 8) = 0x40400000;
    v4->Scr_Labels[19] = 9;
    return 1000;
}

int GST_Script_Combat_GST_ComportementSpeciaux()
{
    double v0; // st7
    int result; // eax
    double v2; // st7
    tScriptStruct_GSTCombat* v3; // [esp+0h] [ebp-B0h]
    tGameObjectMain* v4; // [esp+4h] [ebp-ACh]
    tGameObjectMain* v5; // [esp+8h] [ebp-A8h]
    tGameObjectMain* a3; // [esp+Ch] [ebp-A4h]
    tGameObjectMain* v7; // [esp+10h] [ebp-A0h]
    double v8; // [esp+18h] [ebp-98h]
    tGameObjectMain* v9; // [esp+24h] [ebp-8Ch]
    tGameObjectMain* a2; // [esp+28h] [ebp-88h]
    tGameObjectMain* v11; // [esp+2Ch] [ebp-84h]
    tGameObjectMain* v12; // [esp+30h] [ebp-80h]
    tGameObjectMain* v13; // [esp+34h] [ebp-7Ch]
    tGameObjectMain* v14; // [esp+38h] [ebp-78h]
    tGameObjectMain* v15; // [esp+3Ch] [ebp-74h]
    tGameObjectMain* v16[7]; // [esp+40h] [ebp-70h] BYREF
    float v17; // [esp+5Ch] [ebp-54h]
    tGameObjectMain* v18; // [esp+60h] [ebp-50h]
    int v19; // [esp+64h] [ebp-4Ch]
    int v20; // [esp+68h] [ebp-48h]
    int i; // [esp+6Ch] [ebp-44h]
    int v22; // [esp+70h] [ebp-40h]
    int v23; // [esp+74h] [ebp-3Ch]
    int v24; // [esp+78h] [ebp-38h]
    tScriptStruct_GSTCombat* v25; // [esp+7Ch] [ebp-34h]
    int v26; // [esp+80h] [ebp-30h]
    int v27[5]; // [esp+84h] [ebp-2Ch]
    float v28[6]; // [esp+98h] [ebp-18h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v25 = v3;
    i = 0;
    v22 = 0;
    v17 = 0.0;
    v18 = 0;
    v19 = 0;
    v26 = 0;
    v23 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_GST_Script_Combat_GST_ComportementSpeciaux__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        fct_GST_Script_Combat_GST_ComportementSpeciaux_GST_Script_Combat_GST_Macro_ExecMacroAlterne = (int)AI_ul_RealResolveOneRefFunction(0x9E00614C);
    }
    v26 = 0;
    v19 = 0;
    v28[0] = 1000000.0;
    v16[0] = 0;
    while (v26 < v25->Scr_Actor_Nbr)
    {
        v18 = (tGameObjectMain*)v25->Scr_Slot[v26];
        v15 = v18;
        if (v18)
        {
            if ((unsigned int)v18 <= 2)
                v15 = AI_MainActor[(uint32_t)v18 - 1];
            v14 = v15;
        }
        else
        {
            v14 = 0;
        }
        if (v14)
        {
            v13 = v18;
            if (v18)
            {
                if ((unsigned int)v18 <= 2)
                    v13 = AI_MainActor[(uint32_t)v18 - 1];
                v12 = v13;
            }
            else
            {
                v12 = 0;
            }
            if ((v12->extended->fields->designInt2 & 0xA000) == 0)
            {
                for (i = v19; i >= 0; --i)
                {
                    v11 = (tGameObjectMain*)fct_GST_Script_Combat_GST_ComportementSpeciaux__PJ_Sally_gao;
                    if (fct_GST_Script_Combat_GST_ComportementSpeciaux__PJ_Sally_gao)
                    {
                        if ((unsigned int)fct_GST_Script_Combat_GST_ComportementSpeciaux__PJ_Sally_gao <= 2)
                            v11 = AI_MainActor[fct_GST_Script_Combat_GST_ComportementSpeciaux__PJ_Sally_gao - 1];
                        a2 = v11;
                    }
                    else
                    {
                        a2 = 0;
                    }
                    v9 = v18;
                    if (v18)
                    {
                        if ((unsigned int)v18 <= 2)
                            v9 = AI_MainActor[(uint32_t)v18 - 1];
                        v0 = AI_EvalFunc_OBJ_SqrDistHorz_C(v9, a2);
                    }
                    else
                    {
                        v0 = AI_EvalFunc_OBJ_SqrDistHorz_C(0, a2);
                    }
                    v17 = (float)v0;
                    if (v28[i] <= (double)v17)
                        break;
                    v28[i + 1] = v28[i];
                    v16[i + 1] = v16[i];
                    v28[i] = v17;
                    v16[i] = v18;
                }
                ++v19;
            }
        }
        ++v26;
    }
    if (v19)
    {
        for (i = 0; i < v25->Scr_Actor_Nbr; ++i)
        {
            v27[i] = v25->Scr_Slot[i];
            v25->Scr_Slot[i] = (int)v16[i];
        }
        v23 = v25->Scr_Actor_Nbr;
        v8 = *(float*)AI_EvalVarArray2_C(2, (char*)&v25->__Scr_ScriptParam_Dimension1, 0, v25->Scr_Indice)
            + MATH_gd_magicDoubleForRound;
        v25->Scr_Actor_Nbr = (*(uint32_t*)&v8);
        v22 = v25->Scr_Indice++;
        v24 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Script_Combat_GST_ComportementSpeciaux_GST_Script_Combat_GST_Macro_ExecMacroAlterne);
        if (v24 == 1000)
        {
            v25->Scr_Indice = v22;
            v25->Scr_Actor_Nbr = v23;
            for (i = 0; i < v25->Scr_Actor_Nbr; ++i)
                v25->Scr_Slot[i] = v27[i];
            for (i = 0; i < v25->Scr_Actor_Nbr; ++i)
            {
                v18 = (tGameObjectMain*)v25->Scr_Slot[i];
                v7 = v18;
                if (v18)
                {
                    if ((unsigned int)v18 <= 2)
                        v7 = AI_MainActor[(uint32_t)v18 - 1];
                    a3 = v7;
                }
                else
                {
                    a3 = 0;
                }
                v2 = *(float*)AI_EvalVarArray2_C(2, (char*)&v25->__Scr_ScriptParam_Dimension1, 0, v25->Scr_Indice);
                v20 = AI_EvalFunc_ARRObjSearch_C(v16, (unsigned int)v2, a3);
                if (v20 == -1)
                {
                    v5 = v18;
                    if (v18)
                    {
                        if ((unsigned int)v18 <= 2)
                            v5 = AI_MainActor[(uint32_t)v18 - 1];
                        v4 = v5;
                    }
                    else
                    {
                        v4 = 0;
                    }
                    v4->extended->fields->designInt1 = 64;
                }
            }
            result = 1000;
        }
        else
        {
            result = v24;
        }
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int GST_Script_Combat_GST_Script_Order_Crevette()
{
    int v1; // [esp+4h] [ebp-14h]
    tScriptStruct_GSTCombat* v2; // [esp+8h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v2->__Scr_Macro_Dimension1, 0, 0) = 0;
    v2->Scr_ScriptFunction[0] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ScriptParam_Dimension1, 0, 0) = 0x3F000000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ScriptParam_Dimension1, 1, 0) = 0;
    v2->Scr_ScriptFunction[1] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ScriptParam_Dimension1, 0, 1) = 0x3F800000;
    v2->Scr_ScriptFunction[2] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ScriptParam_Dimension1, 0, 2) = 0x41000000;
    v2->Scr_ScriptFunction[3] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ScriptParam_Dimension1, 0, 3) = 0x42800000;
    v2->Scr_ScriptFunction[4] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ScriptParam_Dimension1, 0, 4) = 0;
    v2->Scr_ScriptFunction[5] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ScriptParam_Dimension1, 0, 5) = 0x3FC00000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ScriptParam_Dimension1, 1, 5) = 0x3E99999A;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v2->__Scr_Macro_Dimension1, 1, 0) = 6;
    v2->Scr_Indice = 6;
    v2->i_label = 0;
    v2->Scr_ScriptFunction[6] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ScriptParam_Dimension1, 0, 6) = 0x40000000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ScriptParam_Dimension1, 1, 6) = 0;
    v2->Scr_Labels[++v2->i_label] = 7;
    v2->Scr_ScriptFunction[7] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ScriptParam_Dimension1, 0, 7) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ScriptParam_Dimension1, 1, 7) = 0;
    v2->Scr_Conditions[v2->Scr_Condition_Nb] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ConditionParam_Dimension1, 0, v2->Scr_Condition_Nb) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ConditionParam_Dimension1, 1, v2->Scr_Condition_Nb) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ConditionParam_Dimension1, 2, v2->Scr_Condition_Nb) = 0x41980000;
    v2->Scr_ConditionOn[v2->Scr_Condition_Nb++] = 0;
    v2->Scr_ScriptFunction[8] = 8;
    v1 = v2->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ScriptParam_Dimension1, 0, 8) = (float)v1;
    v2->Scr_Labels[++v2->i_label] = 9;
    v2->Scr_ScriptFunction[9] = 7;
    v2->Scr_ScriptFunction[9] = 11;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ScriptParam_Dimension1, 0, 9) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ScriptParam_Dimension1, 1, 9) = 0;
    v2->Scr_ScriptFunction[10] = 6;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ScriptParam_Dimension1, 0, 10) = 0x40000000;
    v2->Scr_Labels[19] = 11;
    return 1000;
}

int GST_Script_Combat_GST_Script_Order_SpectreBas()
{
    int v1; // [esp+4h] [ebp-18h]
    int v2; // [esp+8h] [ebp-14h]
    tScriptStruct_GSTCombat* v3; // [esp+Ch] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v3->__Scr_Macro_Dimension1, 0, 0) = 0;
    v3->Scr_ScriptFunction[0] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 0) = 0x41000000;
    v3->Scr_ScriptFunction[1] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 1) = 0x40000000;
    v3->Scr_ScriptFunction[2] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 2) = 0x40000000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 1, 2) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v3->__Scr_Macro_Dimension1, 1, 0) = 3;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v3->__Scr_Macro_Dimension1, 0, 1) = 3;
    v3->Scr_ScriptFunction[3] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 3) = 0x41000000;
    v3->Scr_ScriptFunction[4] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 4) = 0x40000000;
    v3->Scr_ScriptFunction[5] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 5) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 1, 5) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v3->__Scr_Macro_Dimension1, 1, 1) = 6;
    v3->Scr_Indice = 6;
    v3->i_label = 0;
    v3->Scr_Labels[++v3->i_label] = 6;
    v3->Scr_Conditions[v3->Scr_Condition_Nb] = 3;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 0, v3->Scr_Condition_Nb) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 1, v3->Scr_Condition_Nb) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 2, v3->Scr_Condition_Nb) = 0;
    v3->Scr_ConditionOn[v3->Scr_Condition_Nb++] = 0;
    v3->Scr_ScriptFunction[6] = 8;
    v2 = v3->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 6) = (float)v2;
    v3->Scr_Labels[++v3->i_label] = 7;
    v3->Scr_ScriptFunction[7] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 7) = 0x40800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 1, 7) = 0x40800000;
    v3->Scr_ScriptFunction[8] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 8) = 0x40000000;
    v3->Scr_Conditions[v3->Scr_Condition_Nb] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 0, v3->Scr_Condition_Nb) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 1, v3->Scr_Condition_Nb) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 2, v3->Scr_Condition_Nb) = 0x40000000;
    v3->Scr_ConditionOn[v3->Scr_Condition_Nb++] = 0;
    v3->Scr_ScriptFunction[9] = 8;
    v1 = v3->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 9) = (float)v1;
    v3->Scr_ScriptFunction[10] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 10) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 1, 10) = 0;
    v3->Scr_Labels[++v3->i_label] = 11;
    v3->Scr_ScriptFunction[11] = 7;
    v3->Scr_ScriptFunction[11] = 11;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 11) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 1, 11) = 0;
    v3->Scr_ScriptFunction[12] = 6;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 12) = 0x40400000;
    v3->Scr_Labels[19] = 13;
    return 1000;
}

int GST_Script_Combat_GST_CP_RefreshActors()
{
    int result; // eax
    float v1; // [esp+0h] [ebp-58h]
    float v2; // [esp+4h] [ebp-54h]
    tScriptStruct_GSTCombat* v3; // [esp+8h] [ebp-50h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-4Ch]
    tGameObjectMain* v5; // [esp+10h] [ebp-48h]
    tGameObjectMain* v6; // [esp+14h] [ebp-44h]
    tGameObjectMain* v7; // [esp+18h] [ebp-40h]
    tGameObjectMain* a3; // [esp+20h] [ebp-38h]
    tGameObjectMain* v9; // [esp+24h] [ebp-34h]
    tGameObjectMain* v10; // [esp+28h] [ebp-30h]
    tGameObjectMain* v11; // [esp+2Ch] [ebp-2Ch]
    tGameObjectMain* v12; // [esp+30h] [ebp-28h]
    tGameObjectMain* v13; // [esp+34h] [ebp-24h]
    tGameObjectMain* v14; // [esp+38h] [ebp-20h]
    tGameObjectMain* v15; // [esp+3Ch] [ebp-1Ch]
    unsigned int v16; // [esp+44h] [ebp-14h]
    unsigned int v17; // [esp+44h] [ebp-14h]
    int j; // [esp+48h] [ebp-10h]
    int i; // [esp+54h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v2 = (float)v3->Scr_nb_refresh;
    if (*(float*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 1, v3->Scr_Condition_Indice) > (double)v2)
    {
        for (i = 0; ; ++i)
        {
            v1 = (float)i;
            if (*(float*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 0, v3->Scr_Condition_Indice) <= (double)v1)
                break;
            v16 = v3->Scr_Slot[i];
            v15 = (tGameObjectMain*)v16;
            if (v16)
            {
                if (v16 <= 2)
                    v15 = AI_MainActor[v16 - 1];
                v14 = v15;
            }
            else
            {
                v14 = 0;
            }
            if (v14)
            {
                v13 = (tGameObjectMain*)v3->Scr_Slot[i];
                if (v16)
                {
                    if (v16 <= 2)
                        v13 = AI_MainActor[v16 - 1];
                    v12 = v13;
                }
                else
                {
                    v12 = 0;
                }
                if ((v12->extended->fields->designInt2 & 0x8000) != 0)
                {
                    for (j = 0; j < 30; ++j)
                    {
                        v17 = v3->Actors[j];
                        v11 = (tGameObjectMain*)v17;
                        if (v17)
                        {
                            if (v17 <= 2)
                                v11 = AI_MainActor[v17 - 1];
                            v10 = v11;
                        }
                        else
                        {
                            v10 = 0;
                        }
                        if (v10)
                        {
                            v9 = (tGameObjectMain*)v3->Actors[j];
                            if (v17)
                            {
                                if (v17 <= 2)
                                    v9 = AI_MainActor[v17 - 1];
                                a3 = v9;
                            }
                            else
                            {
                                a3 = 0;
                            }
                            if (AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)v3->Scr_Slot, v3->Scr_Actor_Nbr, a3) == -1)
                            {
                                v7 = (tGameObjectMain*)v17;
                                if (v17)
                                {
                                    if (v17 <= 2)
                                        v7 = AI_MainActor[v17 - 1];
                                    v6 = v7;
                                }
                                else
                                {
                                    v6 = 0;
                                }
                                if ((v6->flags2 & 1) != 0)
                                {
                                    v5 = (tGameObjectMain*)v17;
                                    if (v17)
                                    {
                                        if (v17 <= 2)
                                            v5 = AI_MainActor[v17 - 1];
                                        v4 = v5;
                                    }
                                    else
                                    {
                                        v4 = 0;
                                    }
                                    if ((v4->extended->fields->designInt2 & 0x8000) == 0)
                                    {
                                        ++v3->Scr_nb_refresh;
                                        ++v3->Scr_Actor_Nbr;
                                        v3->Scr_Slot[i] = v17;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        result = 1000;
    }
    else
    {
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(2, 0);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int GST_Script_Combat_GST_Macro_AForceComportement()
{
    tScriptStruct_GSTCombat* v1; // [esp+0h] [ebp-28h]
    tGameObjectMain* v2; // [esp+4h] [ebp-24h]
    tGameObjectMain* v3; // [esp+8h] [ebp-20h]
    double v4; // [esp+10h] [ebp-18h]
    unsigned int v5; // [esp+1Ch] [ebp-Ch]
    int i; // [esp+24h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    for (i = 0; i < v1->Scr_Actor_Nbr; ++i)
    {
        v5 = v1->Scr_Slot[i];
        v4 = *(float*)AI_EvalVarArray2_C(2, (char*)&v1->__Scr_ScriptParam_Dimension1, 0, v1->Scr_Indice)
            + MATH_gd_magicDoubleForRound;
        v3 = (tGameObjectMain*)v5;
        if (v5)
        {
            if (v5 <= 2)
                v3 = AI_MainActor[v5 - 1];
            v2 = v3;
        }
        else
        {
            v2 = 0;
        }
        v2->extended->fields->designInt1 = (*(uint32_t *)&v4);
    }
    ++v1->Scr_Indice;
    return 1000;
}

int GST_Script_Combat_GST_Macro_DesactiveCP()
{
    tScriptStruct_GSTCombat* v1; // [esp+0h] [ebp-18h]
    double v2; // [esp+8h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v1->i_test = 0;
    v2 = *(float*)AI_EvalVarArray2_C(2, (char*)&v1->__Scr_ScriptParam_Dimension1, 0, v1->Scr_Indice)
        + MATH_gd_magicDoubleForRound;
    v1->Scr_Condition_Indice = (*(uint32_t *)&v2);
    v1->Scr_ConditionOn[v1->Scr_Condition_Indice] = 0;
    ++v1->Scr_Indice;
    return 1000;
}

int GST_Script_Combat_GST_Script_Order_CaveCrawler()
{
    int v1; // [esp+4h] [ebp-28h]
    int v2; // [esp+8h] [ebp-24h]
    int v3; // [esp+Ch] [ebp-20h]
    tScriptStruct_GSTCombat* v4; // [esp+10h] [ebp-1Ch]
    tGameObjectMain* v5; // [esp+18h] [ebp-14h]
    tGameObjectMain* v6; // [esp+1Ch] [ebp-10h]
    int a4; // [esp+28h] [ebp-4h]
    int a4a; // [esp+28h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v4->__Scr_Macro_Dimension1, 0, 0) = 0;
    v4->Scr_ScriptFunction[0] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 0) = 0x41000000;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v4->__Scr_Macro_Dimension1, 1, 0) = 1;
    v4->Scr_Indice = 1;
    v4->i_label = 0;
    v4->Scr_Conditions[v4->Scr_Condition_Nb] = 3;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 0, v4->Scr_Condition_Nb) = 0x40C00000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 1, v4->Scr_Condition_Nb) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 2, v4->Scr_Condition_Nb) = 0;
    v4->Scr_ConditionOn[v4->Scr_Condition_Nb++] = 0;
    v4->Scr_ScriptFunction[1] = 8;
    v3 = v4->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 1) = (float)v3;
    v4->Scr_Conditions[v4->Scr_Condition_Nb] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 0, v4->Scr_Condition_Nb) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 1, v4->Scr_Condition_Nb) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 2, v4->Scr_Condition_Nb) = 0x40400000;
    v4->Scr_ConditionOn[v4->Scr_Condition_Nb++] = 0;
    v4->Scr_ScriptFunction[2] = 8;
    v2 = v4->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 2) = (float)v2;
    v4->Scr_Conditions[v4->Scr_Condition_Nb] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 0, v4->Scr_Condition_Nb) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 1, v4->Scr_Condition_Nb) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 2, v4->Scr_Condition_Nb) = 0x40A00000;
    v4->Scr_ConditionOn[v4->Scr_Condition_Nb++] = 0;
    v4->Scr_ScriptFunction[3] = 8;
    v1 = v4->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 3) = (float)v1;
    v4->Scr_ScriptFunction[4] = 12;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 4) = 0x3F800000;
    v4->Scr_ScriptFunction[5] = 12;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 5) = 0x40000000;
    v6 = (tGameObjectMain*)v4->Actors[0];
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
        v4->Scr_Labels[++v4->i_label] = 6;
        v4->Scr_ScriptFunction[6] = 1;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 6) = 0x40400000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 6) = 0;
        v4->Scr_ScriptFunction[7] = 8;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 7) = 0x3F800000;
        v4->Scr_ScriptFunction[8] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 8) = 0x461C4400;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 8) = 0x40400000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 8) = 0;
        v4->Scr_ScriptFunction[9] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 9) = 0x469C4200;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 9) = 0x40400000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 9) = 0x3FC00000;
        v4->Scr_ScriptFunction[10] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 10) = 0x46EA6400;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 10) = 0x40400000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 10) = 0x40400000;
        v4->Scr_ScriptFunction[11] = 2;
        v4->Scr_ScriptFunction[11] = 10;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 11) = 0;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 11) = 0;
        v4->Scr_Labels[++v4->i_label] = 12;
        v4->Scr_ScriptFunction[12] = 6;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 12) = 0x40000000;
        v4->Scr_Labels[++v4->i_label] = 13;
        v4->Scr_ScriptFunction[13] = 12;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 13) = 0x3F800000;
        v4->Scr_ScriptFunction[14] = 4;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 14) = 0x3FC00000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 14) = 0;
        v4->Scr_ScriptFunction[15] = 1;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 15) = 0x40800000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 15) = 0;
        v4->Scr_ScriptFunction[16] = 8;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 16) = 0x40000000;
        v4->Scr_ScriptFunction[17] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 17) = 0x461C4400;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 17) = 0x40400000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 17) = 0;
        v4->Scr_ScriptFunction[18] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 18) = 0x469C4200;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 18) = 0x40400000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 18) = 0x3FC00000;
        v4->Scr_ScriptFunction[19] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 19) = 0x46EA6400;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 19) = 0x40400000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 19) = 0x40400000;
        v4->Scr_ScriptFunction[20] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 20) = 0x471C4100;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 20) = 0x40400000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 20) = 0x40600000;
        v4->Scr_ScriptFunction[21] = 2;
        v4->Scr_ScriptFunction[21] = 10;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 21) = 0;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 21) = 0;
        v4->Scr_Labels[++v4->i_label] = 22;
        v4->Scr_ScriptFunction[22] = 6;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 22) = 0x40800000;
        v4->Scr_Labels[++v4->i_label] = 23;
        v4->Scr_ScriptFunction[23] = 12;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 23) = 0x40000000;
        v4->Scr_ScriptFunction[24] = 4;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 24) = 0x3FC00000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 24) = 0;
        v4->Scr_ScriptFunction[25] = 1;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 25) = 0x3F800000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 25) = 0;
        v4->Scr_ScriptFunction[26] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 26) = 0x461C4400;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 26) = 0x41200000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 26) = 0;
        v4->Scr_ScriptFunction[27] = 2;
        v4->Scr_ScriptFunction[27] = 10;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 27) = 0;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 27) = 0;
        v4->Scr_ScriptFunction[28] = 4;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 28) = 0x3F800000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 28) = 0;
        v4->Scr_ScriptFunction[29] = 1;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 29) = 0x3F800000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 29) = 0;
        v4->Scr_ScriptFunction[30] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 30) = 0x461C4400;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 30) = 0x41200000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 30) = 0;
        v4->Scr_ScriptFunction[31] = 2;
        v4->Scr_ScriptFunction[31] = 10;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 31) = 0;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 31) = 0;
        v4->Scr_ScriptFunction[32] = 4;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 32) = 0x3FC00000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 32) = 0;
        v4->Scr_ScriptFunction[33] = 1;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 33) = 0x40800000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 33) = 0;
        v4->Scr_ScriptFunction[34] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 34) = 0x3F800000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 34) = 0x40400000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 34) = 0x3F99999A;
        v4->Scr_ScriptFunction[35] = 2;
        v4->Scr_ScriptFunction[35] = 10;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 35) = 0;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 35) = 0;
        v4->Scr_ScriptFunction[36] = 4;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 36) = 0x3FC00000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 36) = 0;
        v4->Scr_ScriptFunction[37] = 1;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 37) = 0x40C00000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 37) = 0;
        a4 = 38;
        v4->Scr_ScriptFunction[38] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 38) = 0x3F800000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 38) = 0x40400000;
    }
    else
    {
        v4->Scr_ScriptFunction[6] = 4;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 6) = 0x3F800000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 6) = 0;
        v4->Scr_Labels[++v4->i_label] = 7;
        v4->Scr_ScriptFunction[7] = 1;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 7) = 0x40400000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 7) = 0;
        v4->Scr_ScriptFunction[8] = 8;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 8) = 0x3F800000;
        v4->Scr_ScriptFunction[9] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 9) = 0x461C4400;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 9) = 0xC0C00000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 9) = 0;
        v4->Scr_ScriptFunction[10] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 10) = 0x469C4200;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 10) = 0xC0C00000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 10) = 0x3FC00000;
        v4->Scr_ScriptFunction[11] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 11) = 0x46EA6400;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 11) = 0xC0C00000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 11) = 0x40400000;
        v4->Scr_ScriptFunction[12] = 2;
        v4->Scr_ScriptFunction[12] = 10;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 12) = 0;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 12) = 0;
        v4->Scr_Labels[++v4->i_label] = 13;
        v4->Scr_ScriptFunction[13] = 6;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 13) = 0x40000000;
        v4->Scr_Labels[++v4->i_label] = 14;
        v4->Scr_ScriptFunction[14] = 12;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 14) = 0x3F800000;
        v4->Scr_ScriptFunction[15] = 4;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 15) = 0x3FC00000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 15) = 0;
        v4->Scr_ScriptFunction[16] = 1;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 16) = 0x40800000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 16) = 0;
        v4->Scr_ScriptFunction[17] = 8;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 17) = 0x40000000;
        v4->Scr_ScriptFunction[18] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 18) = 0x461C4400;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 18) = 0xC0C00000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 18) = 0;
        v4->Scr_ScriptFunction[19] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 19) = 0x469C4200;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 19) = 0xC0C00000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 19) = 0x3FC00000;
        v4->Scr_ScriptFunction[20] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 20) = 0x46EA6400;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 20) = 0xC0C00000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 20) = 0x40400000;
        v4->Scr_ScriptFunction[21] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 21) = 0x471C4100;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 21) = 0xC0C00000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 21) = 0x40600000;
        v4->Scr_ScriptFunction[22] = 2;
        v4->Scr_ScriptFunction[22] = 10;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 22) = 0;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 22) = 0;
        v4->Scr_Labels[++v4->i_label] = 23;
        v4->Scr_ScriptFunction[23] = 6;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 23) = 0x40800000;
        v4->Scr_Labels[++v4->i_label] = 24;
        v4->Scr_ScriptFunction[24] = 12;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 24) = 0x40000000;
        v4->Scr_ScriptFunction[25] = 4;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 25) = 0x3FC00000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 25) = 0;
        v4->Scr_ScriptFunction[26] = 1;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 26) = 0x3F800000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 26) = 0;
        v4->Scr_ScriptFunction[27] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 27) = 0x461C4400;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 27) = 0xC1200000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 27) = 0;
        v4->Scr_ScriptFunction[28] = 2;
        v4->Scr_ScriptFunction[28] = 10;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 28) = 0;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 28) = 0;
        v4->Scr_ScriptFunction[29] = 4;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 29) = 0x3F800000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 29) = 0;
        v4->Scr_ScriptFunction[30] = 1;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 30) = 0x3F800000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 30) = 0;
        v4->Scr_ScriptFunction[31] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 31) = 0x461C4400;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 31) = 0xC1200000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 31) = 0;
        v4->Scr_ScriptFunction[32] = 2;
        v4->Scr_ScriptFunction[32] = 10;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 32) = 0;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 32) = 0;
        v4->Scr_ScriptFunction[33] = 4;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 33) = 0x3FC00000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 33) = 0;
        v4->Scr_ScriptFunction[34] = 1;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 34) = 0x40800000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 34) = 0;
        v4->Scr_ScriptFunction[35] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 35) = 0x3F800000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 35) = 0xC0C00000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 35) = 0x3F99999A;
        v4->Scr_ScriptFunction[36] = 2;
        v4->Scr_ScriptFunction[36] = 10;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 36) = 0;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 36) = 0;
        v4->Scr_ScriptFunction[37] = 4;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 37) = 0x3FC00000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 37) = 0;
        v4->Scr_ScriptFunction[38] = 1;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 38) = 0x40C00000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 38) = 0;
        a4 = 39;
        v4->Scr_ScriptFunction[39] = 14;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 39) = 0x3F800000;
        *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 39) = 0xC0C00000;
    }
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, a4) = 0x3F99999A;
    a4a = a4 + 1;
    v4->Scr_ScriptFunction[a4a] = 2;
    v4->Scr_ScriptFunction[a4a] = 10;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, a4a) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, a4a++) = 0;
    v4->Scr_ScriptFunction[a4a] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, a4a) = 0x3FC00000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, a4a++) = 0;
    v4->Scr_ScriptFunction[a4a] = 6;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, a4a) = 0x3F800000;
    v4->Scr_Labels[19] = a4a + 1;
    return 1000;
}

int GST_Script_Combat_GST_Macro_ActionAlternee()
{
    tScriptStruct_GSTCombat* v1; // [esp+0h] [ebp-50h]
    tGameObjectMain* v2; // [esp+4h] [ebp-4Ch]
    tGameObjectMain* v3; // [esp+8h] [ebp-48h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-44h]
    tGameObjectMain* v5; // [esp+10h] [ebp-40h]
    tGameObjectMain* v6; // [esp+14h] [ebp-3Ch]
    tGameObjectMain* v7; // [esp+18h] [ebp-38h]
    tGameObjectMain* v8; // [esp+1Ch] [ebp-34h]
    tGameObjectMain* v9; // [esp+20h] [ebp-30h]
    tGameObjectMain* v10; // [esp+24h] [ebp-2Ch]
    tGameObjectMain* v11; // [esp+28h] [ebp-28h]
    tGameObjectMain* v12; // [esp+2Ch] [ebp-24h]
    tGameObjectMain* v13; // [esp+30h] [ebp-20h]
    double v14; // [esp+38h] [ebp-18h]
    tGameObjectMain* v15; // [esp+40h] [ebp-10h]
    tGameObjectMain* v16; // [esp+44h] [ebp-Ch]
    unsigned int v17; // [esp+48h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (v1->Scr_Actor_Nbr)
    {
        v17 = v1->Scr_Slot[v1->AA_Indice];
        if (v1->AA_ActionPrecedente == -1)
        {
            v16 = (tGameObjectMain*)v1->Scr_Slot[v1->AA_Indice];
            if (v17)
            {
                if (v17 <= 2)
                    v16 = AI_MainActor[v17 - 1];
                v15 = v16;
            }
            else
            {
                v15 = 0;
            }
            v1->AA_ActionPrecedente = v15->extended->fields->designInt1;
            v14 = *(float*)AI_EvalVarArray2_C(2, (char*)&v1->__Scr_ScriptParam_Dimension1, 0, v1->Scr_Indice)
                + MATH_gd_magicDoubleForRound;
            v13 = (tGameObjectMain*)v17;
            if (v17)
            {
                if (v17 <= 2)
                    v13 = AI_MainActor[v17 - 1];
                v12 = v13;
            }
            else
            {
                v12 = 0;
            }
            v12->extended->fields->designInt1 = (*(uint32_t *)&v14);
        }
        v11 = (tGameObjectMain*)v17;
        if (v17)
        {
            if (v17 <= 2)
                v11 = AI_MainActor[v17 - 1];
            v10 = v11;
        }
        else
        {
            v10 = 0;
        }
        v9 = (tGameObjectMain*)v17;
        if (v17)
        {
            if (v17 <= 2)
                v9 = AI_MainActor[v17 - 1];
            v8 = v9;
        }
        else
        {
            v8 = 0;
        }
        v7 = (tGameObjectMain*)v17;
        if (v17)
        {
            if (v17 <= 2)
                v7 = AI_MainActor[v17 - 1];
            v6 = v7;
        }
        else
        {
            v6 = 0;
        }
        if (v10->extended->fields->designInt1 == (v6->extended->fields->designInt1 & v8->extended->fields->designInt2))
            goto LABEL_39;
        v5 = (tGameObjectMain*)v17;
        if (v17)
        {
            if (v17 <= 2)
                v5 = AI_MainActor[v17 - 1];
            v4 = v5;
        }
        else
        {
            v4 = 0;
        }
        if ((v4->extended->fields->designInt2 & 0x8000) != 0)
        {
        LABEL_39:
            ++v1->AA_Indice;
            v3 = (tGameObjectMain*)v17;
            if (v17)
            {
                if (v17 <= 2)
                    v3 = AI_MainActor[v17 - 1];
                v2 = v3;
            }
            else
            {
                v2 = 0;
            }
            v2->extended->fields->designInt1 = v1->AA_ActionPrecedente;
            v1->AA_ActionPrecedente = -1;
        }
    }
    else
    {
        v1->AA_Indice = 0;
    }
    if (v1->AA_Indice == v1->Scr_Actor_Nbr)
    {
        v1->AA_Indice = 0;
        ++v1->Scr_Indice;
    }
    return 1000;
}

int GST_Script_Combat_GST_Macro_ExecMacroAlterne()
{
    double v1; // st7
    float* v2; // eax
    float v3; // [esp+0h] [ebp-ECh]
    float v4; // [esp+4h] [ebp-E8h]
    float v5; // [esp+Ch] [ebp-E0h]
    float v6; // [esp+10h] [ebp-DCh]
    float v7; // [esp+14h] [ebp-D8h]
    float v8; // [esp+18h] [ebp-D4h]
    float v9; // [esp+20h] [ebp-CCh]
    tScriptStruct_GSTCombat* v10; // [esp+24h] [ebp-C8h]
    double v11; // [esp+34h] [ebp-B8h]
    tGameObjectMain* v12; // [esp+40h] [ebp-ACh]
    tGameObjectMain* v13; // [esp+44h] [ebp-A8h]
    tGameObjectMain* v14; // [esp+48h] [ebp-A4h]
    tGameObjectMain* v15; // [esp+4Ch] [ebp-A0h]
    tGameObjectMain* v16; // [esp+50h] [ebp-9Ch]
    tGameObjectMain* v17; // [esp+54h] [ebp-98h]
    tGameObjectMain* v18; // [esp+58h] [ebp-94h]
    tGameObjectMain* v19; // [esp+5Ch] [ebp-90h]
    tGameObjectMain* v20; // [esp+60h] [ebp-8Ch]
    tGameObjectMain* v21; // [esp+64h] [ebp-88h]
    tGameObjectMain* v22; // [esp+68h] [ebp-84h]
    tGameObjectMain* v23; // [esp+6Ch] [ebp-80h]
    double v24; // [esp+74h] [ebp-78h]
    double v25; // [esp+94h] [ebp-58h]
    float v26; // [esp+C8h] [ebp-24h]
    unsigned int v27; // [esp+CCh] [ebp-20h]
    unsigned int v28; // [esp+CCh] [ebp-20h]
    int i; // [esp+E0h] [ebp-Ch]
    int v30; // [esp+E0h] [ebp-Ch]
    int v31; // [esp+E4h] [ebp-8h]
    int v32; // [esp+E8h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v10 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v10 = 0;
    if (!v10->Scr_Actor_Nbr)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    if (v10->i_Macro_Ind[0] == -1)
    {
        v10->EMA_indice = 0;
        for (i = 0; i < v10->Scr_Actor_Nbr; ++i)
        {
            v25 = *(float*)AI_EvalVarArray2_C(2, (char*)&v10->__Scr_ScriptParam_Dimension1, 0, v10->Scr_Indice)
                + MATH_gd_magicDoubleForRound;
            v10->i_Macro_Ind[i] = *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v10->__Scr_Macro_Dimension1, 0, (*(int32_t *)&v25));
        }
    }
    if (v10->EMA_indice >= v10->Scr_Actor_Nbr)
        v30 = 0;
    else
        v30 = v10->EMA_indice;
    v1 = *(float*)AI_EvalVarArray2_C(2, (char*)&v10->__Scr_ScriptParam_Dimension1, 0, v10->Scr_Indice)
        + MATH_gd_magicDoubleForRound;
    if (v10->i_Macro_Ind[v30] < *(int32_t*)AI_EvalVarArray2_C(1, (char*)&v10->__Scr_Macro_Dimension1, 1, (*(int32_t *)&v1)))
    {
        v31 = v10->i_Macro_Ind[v30];
        v32 = v10->Scr_ScriptFunction[v31];
        if (v32 == 2)
        {
            v27 = v10->Scr_Slot[v30];
            v24 = *(float*)AI_EvalVarArray2_C(2, (char*)&v10->__Scr_ScriptParam_Dimension1, 0, v31)
                + MATH_gd_magicDoubleForRound;
            v23 = (tGameObjectMain*)v27;
            if (v27)
            {
                if (v27 <= 2)
                    v23 = AI_MainActor[v27 - 1];
                v22 = v23;
            }
            else
            {
                v22 = 0;
            }
            v22->extended->fields->designInt1 = (*(uint32_t *)&v24);
            if (*(float*)AI_EvalVarArray2_C(2, (char*)&v10->__Scr_ScriptParam_Dimension1, 1, v10->Scr_Indice) != 0.0)
            {
                v21 = (tGameObjectMain*)v27;
                if (v27)
                {
                    if (v27 <= 2)
                        v21 = AI_MainActor[v27 - 1];
                    v20 = v21;
                }
                else
                {
                    v20 = 0;
                }
                v9 = (float)v30;
                v20->extended->fields->designFloat1 = *(float*)AI_EvalVarArray2_C(
                    2,
                    (char*)&v10->__Scr_ScriptParam_Dimension1,
                    1,
                    v10->Scr_Indice)
                    * v9;
            }
            v28 = v10->Scr_Slot[v30];
            v19 = (tGameObjectMain*)v28;
            if (v28)
            {
                if (v28 <= 2)
                    v19 = AI_MainActor[v28 - 1];
                v18 = v19;
            }
            else
            {
                v18 = 0;
            }
            v17 = (tGameObjectMain*)v10->Scr_Slot[v30];
            if (v28)
            {
                if (v28 <= 2)
                    v17 = AI_MainActor[v28 - 1];
                v16 = v17;
            }
            else
            {
                v16 = 0;
            }
            v15 = (tGameObjectMain*)v10->Scr_Slot[v30];
            if (v28)
            {
                if (v28 <= 2)
                    v15 = AI_MainActor[v28 - 1];
                v14 = v15;
            }
            else
            {
                v14 = 0;
            }
            if (v18->extended->fields->designInt1 == (v14->extended->fields->designInt1 & v16->extended->fields->designInt2))
                goto LABEL_51;
            v13 = (tGameObjectMain*)v10->Scr_Slot[v30];
            if (v28)
            {
                if (v28 <= 2)
                    v13 = AI_MainActor[v28 - 1];
                v12 = v13;
            }
            else
            {
                v12 = 0;
            }
            if ((v12->extended->fields->designInt2 & 0x8000) != 0)
                LABEL_51:
            ++v10->i_Macro_Ind[v30];
        }
        else if (v32 == 4)
        {
            if (!v10->Scr_TimeWait[0])
            {
                v26 = TIM_gf_MainClock - TIM_gf_LockTime;
                v11 = v26 + MATH_gd_magicDoubleForRound;
                v10->Scr_TimeWait[0] = (*(uint32_t *)&v11);
                v8 = *(float*)AI_EvalVarArray2_C(2, (char*)&v10->__Scr_ScriptParam_Dimension1, 0, v31);
                v4 = *(float*)AI_EvalVarArray2_C(2, (char*)&v10->__Scr_ScriptParam_Dimension1, 1, v31) + v8;
                v7 = *(float*)AI_EvalVarArray2_C(2, (char*)&v10->__Scr_ScriptParam_Dimension1, 0, v31);
                v3 = v7 - *(float*)AI_EvalVarArray2_C(2, (char*)&v10->__Scr_ScriptParam_Dimension1, 1, v31);
                v6 = AI_EvalFunc_MATHRandFloat_C(v3, v4);
                *(float*)AI_EvalVarArray2_C(2, (char*)&v10->__Scr_ScriptParam_Dimension1, 2, v31) = v6;
            }
            v2 = (float*)AI_EvalVarArray2_C(2, (char*)&v10->__Scr_ScriptParam_Dimension1, 2, v31);
            v5 = (float)v10->Scr_TimeWait[0];
            if (AI_EvalFunc_TIMEElapsed_C(v5, *v2))
            {
                v10->Scr_TimeWait[0] = 0;
                ++v10->i_Macro_Ind[v30];
            }
        }
    }
    else if (++v10->EMA_indice >= v10->Scr_Actor_Nbr)
    {
        ++v10->Scr_Indice;
        v10->i_Macro_Ind[0] = -1;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    return 1000;
}

int GST_Script_Combat_GST_Macro_AChoixPerso()
{
    BOOL v0; // eax
    double v1; // st7
    float v3; // [esp+0h] [ebp-2E8h]
    float v4; // [esp+4h] [ebp-2E4h]
    float v5; // [esp+8h] [ebp-2E0h]
    float v6; // [esp+Ch] [ebp-2DCh]
    float v7; // [esp+10h] [ebp-2D8h]
    float v8; // [esp+14h] [ebp-2D4h]
    float v9; // [esp+18h] [ebp-2D0h]
    tScriptStruct_GSTCombat* v10; // [esp+1Ch] [ebp-2CCh]
    tGameObjectMain* v11; // [esp+24h] [ebp-2C4h]
    tGameObjectMain* v12; // [esp+28h] [ebp-2C0h]
    tGameObjectMain* v13; // [esp+2Ch] [ebp-2BCh]
    tGameObjectMain* v14; // [esp+30h] [ebp-2B8h]
    tGameObjectMain* v15; // [esp+34h] [ebp-2B4h]
    tGameObjectMain* v16; // [esp+38h] [ebp-2B0h]
    tGameObjectMain* v17; // [esp+3Ch] [ebp-2ACh]
    float v18; // [esp+58h] [ebp-290h]
    tGameObjectMain* v19; // [esp+6Ch] [ebp-27Ch]
    tGameObjectMain* v20; // [esp+74h] [ebp-274h]
    tGameObjectMain* v21; // [esp+78h] [ebp-270h]
    tGameObjectMain* v22; // [esp+7Ch] [ebp-26Ch]
    tGameObjectMain* v23; // [esp+80h] [ebp-268h]
    tGameObjectMain* v24; // [esp+84h] [ebp-264h]
    tGameObjectMain* v25; // [esp+88h] [ebp-260h]
    tGameObjectMain* v26; // [esp+8Ch] [ebp-25Ch]
    tGameObjectMain* v27; // [esp+B0h] [ebp-238h]
    tGameObjectMain* v28; // [esp+B4h] [ebp-234h]
    tGameObjectMain* v29; // [esp+B8h] [ebp-230h]
    tGameObjectMain* v30; // [esp+BCh] [ebp-22Ch]
    tGameObjectMain* v31; // [esp+C0h] [ebp-228h]
    tGameObjectMain* v32; // [esp+C4h] [ebp-224h]
    tGameObjectMain* v33; // [esp+C8h] [ebp-220h]
    tGameObjectMain* v34; // [esp+CCh] [ebp-21Ch]
    tGameObjectMain* v35; // [esp+D0h] [ebp-218h]
    tGameObjectMain* v36; // [esp+D4h] [ebp-214h]
    tGameObjectMain* v37; // [esp+D8h] [ebp-210h]
    tGameObjectMain* v38; // [esp+DCh] [ebp-20Ch]
    tGameObjectMain* v39; // [esp+E0h] [ebp-208h]
    tGameObjectMain* v40; // [esp+E4h] [ebp-204h]
    tGameObjectMain* v41; // [esp+E8h] [ebp-200h]
    tGameObjectMain* v42; // [esp+ECh] [ebp-1FCh]
    tGameObjectMain* v43; // [esp+F0h] [ebp-1F8h]
    tGameObjectMain* v44; // [esp+F4h] [ebp-1F4h]
    Vec3 v45; // [esp+F8h] [ebp-1F0h] BYREF
    Vec3 v46; // [esp+104h] [ebp-1E4h] BYREF
    float v47; // [esp+110h] [ebp-1D8h]
    Vec3 v48; // [esp+114h] [ebp-1D4h]
    float v49[34]; // [esp+120h] [ebp-1C8h]
    int v50; // [esp+1A8h] [ebp-140h]
    tGameObjectMain* v51[34]; // [esp+1ACh] [ebp-13Ch] BYREF
    Vec3 v52; // [esp+234h] [ebp-B4h] BYREF
    Vec3 v53; // [esp+240h] [ebp-A8h] BYREF
    int v54; // [esp+24Ch] [ebp-9Ch]
    Vec3 a1; // [esp+250h] [ebp-98h] BYREF
    Vec3 v56; // [esp+25Ch] [ebp-8Ch] BYREF
    float v57; // [esp+268h] [ebp-80h]
    Vec3 v58; // [esp+26Ch] [ebp-7Ch] BYREF
    Vec3 dest; // [esp+278h] [ebp-70h] BYREF
    Vec3 v60; // [esp+284h] [ebp-64h]
    int v61; // [esp+290h] [ebp-58h]
    int v62; // [esp+294h] [ebp-54h]
    Vec3 v63; // [esp+298h] [ebp-50h] BYREF
    BOOL v64; // [esp+2A4h] [ebp-44h]
    tGameObjectMain* v65; // [esp+2A8h] [ebp-40h]
    int j; // [esp+2ACh] [ebp-3Ch]
    int v67; // [esp+2B4h] [ebp-34h]
    tScriptStruct_GSTCombat* v68; // [esp+2B8h] [ebp-30h]
    int i; // [esp+2BCh] [ebp-2Ch]
    Vec3 v70; // [esp+2C0h] [ebp-28h] BYREF
    Vec3 a3; // [esp+2CCh] [ebp-1Ch] BYREF
    Vec3 v72; // [esp+2D8h] [ebp-10h] BYREF

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v10 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v10 = 0;
    v68 = v10;
    j = 0;
    v50 = 0;
    v61 = 0;
    v65 = 0;
    i = 0;
    v47 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        GST_Script_Combat_GST_Macro_AChoixPerso__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        GST_Script_Combat_GST_Macro_AChoixPerso__PJ_Camera_gao = AI_ul_RealResolveOneRefObject(0x71003F56u);
    }
    v68->Scr_Actor_ID = 0;
    v50 = 0;
    if (*(float*)AI_EvalVarArray2_C(2, (char*)&v68->__Scr_ScriptParam_Dimension1, 1, v68->Scr_Indice) == 0.0)
    {
        v68->Scr_Actor_Nbr = 0;
        do
        {
            v9 = (float)v68->Scr_Actor_Nbr;
            if (*(float*)AI_EvalVarArray2_C(2, (char*)&v68->__Scr_ScriptParam_Dimension1, 0, v68->Scr_Indice) <= (double)v9)
                break;
            v65 = (tGameObjectMain*)v68->Actors[v68->Scr_Actor_ID];
            v44 = v65;
            if (v65)
            {
                if ((unsigned int)v65 <= 2)
                    v44 = AI_MainActor[(uint32_t)v65 - 1];
                v43 = v44;
            }
            else
            {
                v43 = 0;
            }
            if (v43)
            {
                v42 = v65;
                if (v65)
                {
                    if ((unsigned int)v65 <= 2)
                        v42 = AI_MainActor[(uint32_t)v65 - 1];
                    v41 = v42;
                }
                else
                {
                    v41 = 0;
                }
                v54 = (unsigned __int16)v41->flags2;
                if ((v54 & 1) != 0)
                {
                    v40 = v65;
                    if (v65)
                    {
                        if ((unsigned int)v65 <= 2)
                            v40 = AI_MainActor[(uint32_t)v65 - 1];
                        v39 = v40;
                    }
                    else
                    {
                        v39 = 0;
                    }
                    if ((v39->extended->fields->designInt2 & 0x9000) == 0)
                        v68->Scr_Slot[v68->Scr_Actor_Nbr++] = (int)v65;
                }
            }
            if (++v68->Scr_Actor_ID >= 30)
                v68->Scr_Actor_ID = 0;
        } while (v50 != v68->Scr_Actor_ID);
        v8 = (float)v68->Scr_Actor_Nbr;
        if (*(float*)AI_EvalVarArray2_C(2, (char*)&v68->__Scr_ScriptParam_Dimension1, 0, v68->Scr_Indice) <= (double)v8)
        {
            ++v68->Scr_Indice;
            v68->i_choix_perso_done = 1;
        }
    }
    else if (*(float*)AI_EvalVarArray2_C(2, (char*)&v68->__Scr_ScriptParam_Dimension1, 1, v68->Scr_Indice) == 4.0)
    {
        v68->Scr_Actor_Nbr = 0;
        do
        {
            v7 = (float)v68->Scr_Actor_Nbr;
            if (*(float*)AI_EvalVarArray2_C(2, (char*)&v68->__Scr_ScriptParam_Dimension1, 0, v68->Scr_Indice) <= (double)v7)
                break;
            v65 = (tGameObjectMain*)v68->Actors[v68->Scr_Actor_ID];
            v38 = v65;
            if (v65)
            {
                if ((unsigned int)v65 <= 2)
                    v38 = AI_MainActor[(uint32_t)v65 - 1];
                v37 = v38;
            }
            else
            {
                v37 = 0;
            }
            if (v37)
            {
                v36 = v65;
                if (v65)
                {
                    if ((unsigned int)v65 <= 2)
                        v36 = AI_MainActor[(uint32_t)v65 - 1];
                    v35 = v36;
                }
                else
                {
                    v35 = 0;
                }
                v67 = (unsigned __int16)v35->flags2;
                if ((v67 & 1) != 0)
                {
                    v34 = v65;
                    if (v65)
                    {
                        if ((unsigned int)v65 <= 2)
                            v34 = AI_MainActor[(uint32_t)v65 - 1];
                        v33 = v34;
                    }
                    else
                    {
                        v33 = 0;
                    }
                    if ((v33->extended->fields->designInt2 & 0x9000) == 0)
                        v68->Scr_Slot[v68->Scr_Actor_Nbr++] = (int)v65;
                }
            }
            if (++v68->Scr_Actor_ID >= 30)
                v68->Scr_Actor_ID = 0;
        } while (v50 != v68->Scr_Actor_ID);
        if (v68->Scr_Actor_Nbr >= 1)
        {
            ++v68->Scr_Indice;
            v68->i_choix_perso_done = 1;
        }
    }
    else if (*(float*)AI_EvalVarArray2_C(2, (char*)&v68->__Scr_ScriptParam_Dimension1, 1, v68->Scr_Indice) == 1.0
        || *(float*)AI_EvalVarArray2_C(2, (char*)&v68->__Scr_ScriptParam_Dimension1, 1, v68->Scr_Indice) == 5.0
        || *(float*)AI_EvalVarArray2_C(2, (char*)&v68->__Scr_ScriptParam_Dimension1, 1, v68->Scr_Indice) == 6.0)
    {
        v68->Scr_Actor_ID = 0;
        v68->Scr_Actor_Nbr = 0;
        for (i = 0; i < 30; ++i)
        {
            v65 = (tGameObjectMain*)v68->Actors[i];
            v32 = v65;
            if (v65)
            {
                if ((unsigned int)v65 <= 2)
                    v32 = AI_MainActor[(uint32_t)v65 - 1];
                v31 = v32;
            }
            else
            {
                v31 = 0;
            }
            if (v31)
            {
                v30 = v65;
                if (v65)
                {
                    if ((unsigned int)v65 <= 2)
                        v30 = AI_MainActor[(uint32_t)v65 - 1];
                    v29 = v30;
                }
                else
                {
                    v29 = 0;
                }
                if ((v29->extended->fields->designInt2 & 0x9000) == 0)
                {
                    v28 = v65;
                    if (v65)
                    {
                        if ((unsigned int)v65 <= 2)
                            v28 = AI_MainActor[(uint32_t)v65 - 1];
                        v27 = v28;
                    }
                    else
                    {
                        v27 = 0;
                    }
                    if ((v27->extended->fields->designInt1 & 8) == 0)
                        v51[++v50] = v65;
                }
            }
        }
        if (*(float*)AI_EvalVarArray2_C(2, (char*)&v68->__Scr_ScriptParam_Dimension1, 1, v68->Scr_Indice) == 1.0)
        {
            AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)GST_Script_Combat_GST_Macro_AChoixPerso__PJ_Sally_gao, &v63);
            v48.x = -v63.x;
            v48.y = -v63.y;
            v48.z = -v63.z;
            a1.x = -v63.x;
            a1.y = -v63.y;
            a1.z = -v63.z;
        }
        else if (*(float*)AI_EvalVarArray2_C(2, (char*)&v68->__Scr_ScriptParam_Dimension1, 1, v68->Scr_Indice) == 5.0)
        {
            AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)GST_Script_Combat_GST_Macro_AChoixPerso__PJ_Sally_gao, &v46);
            a1 = v46;
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGet_C((tGameObjectMain*)GST_Script_Combat_GST_Macro_AChoixPerso__PJ_Sally_gao, &v58);
            a1 = v58;
        }
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)GST_Script_Combat_GST_Macro_AChoixPerso__PJ_Sally_gao, &a3);
        v61 = AI_EvalFunc_ARRObjBestSight_C(&v51[1], v50, &a3, &a1, 0.0);
        if (v61 == -1)
            v68->Scr_Slot[v68->Scr_Actor_Nbr] = 0;
        else
            v68->Scr_Slot[v68->Scr_Actor_Nbr++] = (int)v51[v61 + 1];
        ++v68->Scr_Indice;
        v68->i_choix_perso_done = 1;
    }
    else if (*(float*)AI_EvalVarArray2_C(2, (char*)&v68->__Scr_ScriptParam_Dimension1, 1, v68->Scr_Indice) == 7.0)
    {
        v68->Scr_Actor_ID = 0;
        v68->Scr_Actor_Nbr = 0;
        for (i = 0; i < 30; ++i)
        {
            v65 = (tGameObjectMain*)v68->Actors[i];
            v26 = v65;
            if (v65)
            {
                if ((unsigned int)v65 <= 2)
                    v26 = AI_MainActor[(uint32_t)v65 - 1];
                v25 = v26;
            }
            else
            {
                v25 = 0;
            }
            if (v25)
            {
                v24 = v65;
                if (v65)
                {
                    if ((unsigned int)v65 <= 2)
                        v24 = AI_MainActor[(uint32_t)v65 - 1];
                    v23 = v24;
                }
                else
                {
                    v23 = 0;
                }
                if ((v23->extended->fields->designInt2 & 0x9000) == 0)
                {
                    v22 = v65;
                    if (v65)
                    {
                        if ((unsigned int)v65 <= 2)
                            v22 = AI_MainActor[(uint32_t)v65 - 1];
                        v21 = v22;
                    }
                    else
                    {
                        v21 = 0;
                    }
                    if ((v21->extended->fields->designInt1 & 8) == 0)
                    {
                        v20 = v65;
                        if (v65)
                        {
                            if ((unsigned int)v65 <= 2)
                                v20 = AI_MainActor[(uint32_t)v65 - 1];
                            v0 = AI_EvalFunc_VIEWCulled_C(v20, 0);
                        }
                        else
                        {
                            v0 = AI_EvalFunc_VIEWCulled_C(0, 0);
                        }
                        v64 = v0;
                        if (!v0)
                        {
                            v19 = v65;
                            if (v65)
                            {
                                if ((unsigned int)v65 <= 2)
                                    v19 = AI_MainActor[(uint32_t)v65 - 1];
                                AI_EvalFunc_OBJGetPos_C(v19, &v70);
                            }
                            else
                            {
                                AI_EvalFunc_OBJGetPos_C(0, &v70);
                            }
                            AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)GST_Script_Combat_GST_Macro_AChoixPerso__PJ_Camera_gao, &v52);
                            MATH_SubstractVec(&dest, &v70, &v52);
                            a1.x = dest.x;
                            a1.y = dest.y;
                            a1.z = 0.0;
                            v47 = MATH_VecLen(&a1);
                            if (v47 == 0.0)
                            {
                                AI_EvalFunc_OBJ_SightGet_C(
                                    (tGameObjectMain*)GST_Script_Combat_GST_Macro_AChoixPerso__PJ_Camera_gao,
                                    &v72);
                                v60.x = -v72.x;
                                v60.y = -v72.y;
                                v60.z = -v72.z;
                                a1.x = -v72.x;
                                a1.y = -v72.y;
                                a1.z = -v72.z;
                            }
                            else
                            {
                                v18 = 1.0f / v47;
                                a1.x = a1.x * v18;
                                a1.y = a1.y * v18;
                                a1.z = a1.z * v18;
                            }
                            AI_EvalFunc_OBJ_SightGet_C(
                                (tGameObjectMain*)GST_Script_Combat_GST_Macro_AChoixPerso__PJ_Camera_gao,
                                &v53);
                            v57 = a1.z * v53.z + a1.y * v53.y + v53.x * a1.x;
                            if (v57 > 0.70710701)
                                v51[++v50] = v65;
                        }
                    }
                }
            }
        }
        while (1)
        {
            v6 = (float)v68->Scr_Actor_Nbr;
            if (*(float*)AI_EvalVarArray2_C(2, (char*)&v68->__Scr_ScriptParam_Dimension1, 0, v68->Scr_Indice) <= (double)v6)
                break;
            AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)GST_Script_Combat_GST_Macro_AChoixPerso__PJ_Camera_gao, &v45);
            a1 = v45;
            AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)GST_Script_Combat_GST_Macro_AChoixPerso__PJ_Sally_gao, &v56);
            v61 = AI_EvalFunc_ARRObjBestSight_C(&v51[1], v50, &v56, &a1, 0.0);
            if (v61 == -1)
            {
                v68->Scr_Slot[v68->Scr_Actor_Nbr] = 0;
                break;
            }
            v68->Scr_Slot[v68->Scr_Actor_Nbr] = (int)v51[v61 + 1];
            v51[v61 + 1] = v51[v50--];
            ++v68->Scr_Actor_Nbr;
        }
        ++v68->Scr_Indice;
        v68->i_choix_perso_done = 1;
    }
    else if (*(float*)AI_EvalVarArray2_C(2, (char*)&v68->__Scr_ScriptParam_Dimension1, 1, v68->Scr_Indice) == 2.0
        || *(float*)AI_EvalVarArray2_C(2, (char*)&v68->__Scr_ScriptParam_Dimension1, 1, v68->Scr_Indice) == 3.0)
    {
        v68->Scr_Actor_ID = 0;
        v68->Scr_Actor_Nbr = 0;
        v49[0] = 1.0e14f;
        v51[1] = 0;
        for (i = 0; i < 30; ++i)
        {
            v65 = (tGameObjectMain*)v68->Actors[i];
            v17 = v65;
            if (v65)
            {
                if ((unsigned int)v65 <= 2)
                    v17 = AI_MainActor[(uint32_t)v65 - 1];
                v16 = v17;
            }
            else
            {
                v16 = 0;
            }
            if (v16)
            {
                v15 = v65;
                if (v65)
                {
                    if ((unsigned int)v65 <= 2)
                        v15 = AI_MainActor[(uint32_t)v65 - 1];
                    v14 = v15;
                }
                else
                {
                    v14 = 0;
                }
                v62 = (unsigned __int16)v14->flags2;
                if ((v62 & 1) != 0)
                {
                    v13 = v65;
                    if (v65)
                    {
                        if ((unsigned int)v65 <= 2)
                            v13 = AI_MainActor[(uint32_t)v65 - 1];
                        v12 = v13;
                    }
                    else
                    {
                        v12 = 0;
                    }
                    if ((v12->extended->fields->designInt2 & 0x8000) == 0)
                    {
                        v11 = v65;
                        if (v65)
                        {
                            if ((unsigned int)v65 <= 2)
                                v11 = AI_MainActor[(uint32_t)v65 - 1];
                            v1 = AI_EvalFunc_OBJ_SqrDist_C(
                                (tGameObjectMain*)GST_Script_Combat_GST_Macro_AChoixPerso__PJ_Sally_gao,
                                v11);
                        }
                        else
                        {
                            v1 = AI_EvalFunc_OBJ_SqrDist_C(
                                (tGameObjectMain*)GST_Script_Combat_GST_Macro_AChoixPerso__PJ_Sally_gao,
                                0);
                        }
                        v47 = (float)v1;
                        for (j = i; j >= 0; --j)
                        {
                            if (v49[j] > (double)v47)
                            {
                                v51[j + 2] = v51[j + 1];
                                v49[j + 1] = v49[j];
                                v51[j + 1] = v65;
                                v49[j] = v47;
                                ++v68->Scr_Actor_ID;
                            }
                        }
                    }
                }
            }
        }
        if (*(float*)AI_EvalVarArray2_C(2, (char*)&v68->__Scr_ScriptParam_Dimension1, 1, v68->Scr_Indice) == 2.0)
        {
            for (i = 0; ; ++i)
            {
                v5 = (float)i;
                if (*(float*)AI_EvalVarArray2_C(2, (char*)&v68->__Scr_ScriptParam_Dimension1, 0, v68->Scr_Indice) <= (double)v5)
                    break;
                v68->Scr_Slot[v68->Scr_Actor_Nbr++] = (int)v51[i + 1];
            }
        }
        else if (*(float*)AI_EvalVarArray2_C(2, (char*)&v68->__Scr_ScriptParam_Dimension1, 1, v68->Scr_Indice) == 3.0)
        {
            for (i = v68->Scr_Actor_ID - 1; i >= 0; --i)
            {
                v68->Scr_Slot[v68->Scr_Actor_Nbr++] = (int)v51[i + 1];
                v4 = (float)v68->Scr_Actor_Nbr;
                if (*(float*)AI_EvalVarArray2_C(2, (char*)&v68->__Scr_ScriptParam_Dimension1, 0, v68->Scr_Indice) <= (double)v4)
                    break;
            }
        }
        if (v68->Scr_Actor_ID)
        {
            v3 = (float)v68->Scr_Actor_Nbr;
            if (*(float*)AI_EvalVarArray2_C(2, (char*)&v68->__Scr_ScriptParam_Dimension1, 0, v68->Scr_Indice) <= (double)v3)
            {
                ++v68->Scr_Indice;
                v68->i_choix_perso_done = 1;
            }
        }
    }
    return 1000;
}

int GST_Script_Combat_GST_Macro_TestContinue()
{
    tScriptStruct_GSTCombat* v1; // [esp+0h] [ebp-34h]
    tGameObjectMain* v2; // [esp+4h] [ebp-30h]
    tGameObjectMain* v3; // [esp+8h] [ebp-2Ch]
    tGameObjectMain* v4; // [esp+Ch] [ebp-28h]
    tGameObjectMain* v5; // [esp+10h] [ebp-24h]
    tGameObjectMain* v6; // [esp+14h] [ebp-20h]
    tGameObjectMain* v7; // [esp+18h] [ebp-1Ch]
    tGameObjectMain* v8; // [esp+1Ch] [ebp-18h]
    tGameObjectMain* v9; // [esp+20h] [ebp-14h]
    unsigned int v10; // [esp+24h] [ebp-10h]
    int v11; // [esp+28h] [ebp-Ch]
    int i; // [esp+30h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v11 = 0;
    for (i = 0; i < v1->Scr_Actor_Nbr; ++i)
    {
        v10 = v1->Scr_Slot[i];
        v9 = (tGameObjectMain*)v10;
        if (v10)
        {
            if (v10 <= 2)
                v9 = AI_MainActor[v10 - 1];
            v8 = v9;
        }
        else
        {
            v8 = 0;
        }
        v7 = (tGameObjectMain*)v1->Scr_Slot[i];
        if (v10)
        {
            if (v10 <= 2)
                v7 = AI_MainActor[v10 - 1];
            v6 = v7;
        }
        else
        {
            v6 = 0;
        }
        v5 = (tGameObjectMain*)v1->Scr_Slot[i];
        if (v10)
        {
            if (v10 <= 2)
                v5 = AI_MainActor[v10 - 1];
            v4 = v5;
        }
        else
        {
            v4 = 0;
        }
        if (v8->extended->fields->designInt1 == (v4->extended->fields->designInt1 & v6->extended->fields->designInt2))
            goto LABEL_30;
        v3 = (tGameObjectMain*)v1->Scr_Slot[i];
        if (v10)
        {
            if (v10 <= 2)
                v3 = AI_MainActor[v10 - 1];
            v2 = v3;
        }
        else
        {
            v2 = 0;
        }
        if ((v2->extended->fields->designInt2 & 0x8000) != 0)
            LABEL_30:
        ++v11;
    }
    if (v11 == v1->Scr_Actor_Nbr)
        ++v1->Scr_Indice;
    return 1000;
}

int GST_Script_Combat_GST_CP_Minumum()
{
    int result; // eax
    float v1; // [esp+4h] [ebp-A4h]
    float v2; // [esp+8h] [ebp-A0h]
    float v3; // [esp+Ch] [ebp-9Ch]
    tScriptStruct_GSTCombat* v4; // [esp+10h] [ebp-98h]
    double v5; // [esp+18h] [ebp-90h]
    tGameObjectMain* v6; // [esp+24h] [ebp-84h]
    tGameObjectMain* v7; // [esp+28h] [ebp-80h]
    tGameObjectMain* v8; // [esp+2Ch] [ebp-7Ch]
    tGameObjectMain* v9; // [esp+30h] [ebp-78h]
    double v10; // [esp+38h] [ebp-70h]
    tGameObjectMain* a3; // [esp+4Ch] [ebp-5Ch]
    tGameObjectMain* v12; // [esp+50h] [ebp-58h]
    tGameObjectMain* v13; // [esp+54h] [ebp-54h]
    tGameObjectMain* v14; // [esp+58h] [ebp-50h]
    tGameObjectMain* v15; // [esp+5Ch] [ebp-4Ch]
    tGameObjectMain* v16; // [esp+60h] [ebp-48h]
    tGameObjectMain* v17; // [esp+64h] [ebp-44h]
    tGameObjectMain* v18; // [esp+68h] [ebp-40h]
    tGameObjectMain* v19; // [esp+6Ch] [ebp-3Ch]
    tGameObjectMain* v20; // [esp+70h] [ebp-38h]
    tGameObjectMain* v21; // [esp+74h] [ebp-34h]
    tGameObjectMain* v22; // [esp+78h] [ebp-30h]
    double v23; // [esp+80h] [ebp-28h]
    unsigned int v24; // [esp+90h] [ebp-18h]
    unsigned int v25; // [esp+90h] [ebp-18h]
    int v26; // [esp+94h] [ebp-14h]
    int i; // [esp+A4h] [ebp-4h]
    int j; // [esp+A4h] [ebp-4h]
    int k; // [esp+A4h] [ebp-4h]
    int l; // [esp+A4h] [ebp-4h]
    int m; // [esp+A4h] [ebp-4h]
    int n; // [esp+A4h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        GST_Script_Combat_GST_CP_Minumum_GST_Fight_Manager_gao = AI_ul_RealResolveOneRefObject(0x9E002FFA);
        AI2C_ResolveBuffer(
            &GST_Script_Combat_GST_CP_Minumum_GST_Fight_Manager_gao_i_Evil_NbActor,
            (void*)GST_Script_Combat_GST_CP_Minumum_GST_Fight_Manager_gao,
            2624);
    }
    v26 = 0;
    if (*(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 0, v4->Scr_Condition_Indice) == 5.0)
    {
        v3 = (float)*(int*)GST_Script_Combat_GST_CP_Minumum_GST_Fight_Manager_gao_i_Evil_NbActor;
        if (*(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 1, v4->Scr_Condition_Indice) > (double)v3)
        {
            for (i = 0; i < 30; ++i)
                v4->Scr_TimeWait[i] = 0;
            v4->i_Macro_Ind[0] = -1;
            if (*(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 2, v4->Scr_Condition_Indice) == -1.0)
            {
                v4->Scr_Indice = v4->Scr_Labels[v4->Scr_Label_Indice + 1];
            }
            else
            {
                v23 = *(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 2, v4->Scr_Condition_Indice)
                    + MATH_gd_magicDoubleForRound;
                v4->Scr_Indice = v4->Scr_Labels[(*(uint32_t *)&v23)];
            }
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else if (v4->Scr_Actor_Nbr)
    {
        for (j = 0; j < v4->Scr_Actor_Nbr; ++j)
        {
            v22 = (tGameObjectMain*)v4->Scr_Slot[j];
            if (v22)
            {
                if ((unsigned int)v22 <= 2)
                    v22 = AI_MainActor[(uint32_t)v22 - 1];
                v21 = v22;
            }
            else
            {
                v21 = 0;
            }
            if (!v21)
                goto LABEL_33;
            v20 = (tGameObjectMain*)v4->Scr_Slot[j];
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
            if ((v19->extended->fields->designInt2 & 0x8000) != 0)
            {
            LABEL_33:
                if (!--v4->Scr_Actor_Nbr)
                    break;
                v4->Scr_Slot[j] = v4->Scr_Slot[v4->Scr_Actor_Nbr];
                v4->Scr_Slot[v4->Scr_Actor_Nbr] = 0;
                --j;
            }
        }
        v2 = (float)v4->Scr_Actor_Nbr;
        if (*(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 1, v4->Scr_Condition_Indice) > (double)v2)
            v26 = 1;
        if (v26)
        {
            if (*(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 0, v4->Scr_Condition_Indice) == 2.0)
            {
                for (k = 0; ; ++k)
                {
                    v1 = (float)v4->Scr_Actor_Nbr;
                    if (*(float*)AI_EvalVarArray2_C(
                        2,
                        (char*)&v4->__Scr_ConditionParam_Dimension1,
                        2,
                        v4->Scr_Condition_Indice) <= (double)v1
                        || k >= 30)
                    {
                        break;
                    }
                    v24 = v4->Actors[k];
                    v18 = (tGameObjectMain*)v24;
                    if (v24)
                    {
                        if (v24 <= 2)
                            v18 = AI_MainActor[v24 - 1];
                        v17 = v18;
                    }
                    else
                    {
                        v17 = 0;
                    }
                    if (!v17)
                        break;
                    v16 = (tGameObjectMain*)v4->Actors[k];
                    if (v24)
                    {
                        if (v24 <= 2)
                            v16 = AI_MainActor[v24 - 1];
                        v15 = v16;
                    }
                    else
                    {
                        v15 = 0;
                    }
                    if ((v15->extended->fields->designInt2 & 0x8000) == 0)
                    {
                        v14 = (tGameObjectMain*)v4->Actors[k];
                        if (v24)
                        {
                            if (v24 <= 2)
                                v14 = AI_MainActor[v24 - 1];
                            v13 = v14;
                        }
                        else
                        {
                            v13 = 0;
                        }
                        if ((v13->flags2 & 1) != 0)
                        {
                            v12 = (tGameObjectMain*)v4->Actors[k];
                            if (v24)
                            {
                                if (v24 <= 2)
                                    v12 = AI_MainActor[v24 - 1];
                                a3 = v12;
                            }
                            else
                            {
                                a3 = 0;
                            }
                            if (AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)v4->Scr_Slot, v4->Scr_Actor_Nbr, a3) == -1)
                                v4->Scr_Slot[v4->Scr_Actor_Nbr++] = v24;
                        }
                    }
                }
            }
            else if (*(float*)AI_EvalVarArray2_C(
                2,
                (char*)&v4->__Scr_ConditionParam_Dimension1,
                0,
                v4->Scr_Condition_Indice) == 3.0)
            {
                for (l = 0; l < 30; ++l)
                    v4->Scr_TimeWait[l] = 0;
                v4->i_Macro_Ind[0] = -1;
                if (*(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 2, v4->Scr_Condition_Indice) == -1.0)
                {
                    v4->Scr_Indice = v4->Scr_Labels[v4->Scr_Label_Indice + 1];
                }
                else
                {
                    v10 = *(float*)AI_EvalVarArray2_C(
                        2,
                        (char*)&v4->__Scr_ConditionParam_Dimension1,
                        2,
                        v4->Scr_Condition_Indice)
                        + MATH_gd_magicDoubleForRound;
                    v4->Scr_Indice = v4->Scr_Labels[(*(uint32_t *)&v10)];
                }
            }
            else if (*(float*)AI_EvalVarArray2_C(
                2,
                (char*)&v4->__Scr_ConditionParam_Dimension1,
                0,
                v4->Scr_Condition_Indice) == 4.0)
            {
                for (m = 0; m < 30; ++m)
                {
                    v25 = v4->Actors[m];
                    v9 = (tGameObjectMain*)v25;
                    if (v25)
                    {
                        if (v25 <= 2)
                            v9 = AI_MainActor[v25 - 1];
                        v8 = v9;
                    }
                    else
                    {
                        v8 = 0;
                    }
                    if (v8)
                    {
                        v7 = (tGameObjectMain*)v4->Actors[m];
                        if (v25)
                        {
                            if (v25 <= 2)
                                v7 = AI_MainActor[v25 - 1];
                            v6 = v7;
                        }
                        else
                        {
                            v6 = 0;
                        }
                        if ((v6->extended->fields->designInt2 & 0x8000) == 0)
                        {
                            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                            return 1000;
                        }
                    }
                }
                for (n = 0; n < 30; ++n)
                    v4->Scr_TimeWait[n] = 0;
                v4->i_Macro_Ind[0] = -1;
                v5 = *(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 2, v4->Scr_Condition_Indice)
                    + MATH_gd_magicDoubleForRound;
                v4->Scr_Indice = v4->Scr_Labels[(*(uint32_t *)&v5)];
            }
        }
        result = 1000;
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int GST_Script_Combat_GST_Script_Order_DireWorm()
{
    float v1; // [esp+4h] [ebp-24h]
    float v2; // [esp+8h] [ebp-20h]
    float v3; // [esp+Ch] [ebp-1Ch]
    float v4; // [esp+10h] [ebp-18h]
    int v5; // [esp+14h] [ebp-14h]
    tScriptStruct_GSTCombat* v6; // [esp+18h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v6->__Scr_Macro_Dimension1, 0, 0) = 0;
    v6->Scr_ScriptFunction[0] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 0) = 0x41000000;
    v6->Scr_ScriptFunction[1] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 1) = 0x41800000;
    v6->Scr_ScriptFunction[2] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 2) = 0x3F0CCCCD;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 2) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v6->__Scr_Macro_Dimension1, 1, 0) = 3;
    v6->Scr_Indice = 3;
    v6->i_label = 0;
    v6->Scr_ScriptFunction[3] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 3) = 0x41200000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 3) = 0x40800000;
    v6->Scr_Conditions[v6->Scr_Condition_Nb] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ConditionParam_Dimension1, 0, v6->Scr_Condition_Nb) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ConditionParam_Dimension1, 1, v6->Scr_Condition_Nb) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ConditionParam_Dimension1, 2, v6->Scr_Condition_Nb) = 0x41980000;
    v6->Scr_ConditionOn[v6->Scr_Condition_Nb++] = 0;
    v6->Scr_ScriptFunction[4] = 8;
    v5 = v6->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 4) = (float)v5;
    v6->Scr_ScriptFunction[5] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 5) = 0x3E99999A;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 5) = 0;
    v6->Scr_Labels[++v6->i_label] = 6;
    v6->Scr_ScriptFunction[6] = 14;
    v4 = v6->Script_4_distance_a_Sally + 0.0f;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 6) = v4;
    v3 = v6->Script_4_vitesse_de_deplaceme;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 6) = v3;
    v2 = v6->Script_4_delai_entre_attaque;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 2, 6) = v2;
    v6->Scr_ScriptFunction[7] = 2;
    v6->Scr_ScriptFunction[7] = 10;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 7) = 0;
    v1 = v6->Script_4_angle_entre_Worms;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 1, 7) = v1;
    v6->Scr_ScriptFunction[8] = 6;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v6->__Scr_ScriptParam_Dimension1, 0, 8) = (float)v6->i_label;
    v6->Scr_Labels[19] = 9;
    return 1000;
}

int GST_Script_Combat_GST_CP_FightSallyOn()
{
    int v0; // eax
    tScriptStruct_GSTCombat* v2; // [esp+0h] [ebp-1Ch]
    tGameObjectMain* v3; // [esp+8h] [ebp-14h]
    tGameObjectMain* a2; // [esp+Ch] [ebp-10h]
    tGameObjectMain* v5; // [esp+10h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        GST_Script_Combat_GST_CP_FightSallyOn__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(
            &GST_Script_Combat_GST_CP_FightSallyOn__PJ_Sally_gao_i_Fight_dymmy,
            (void*)GST_Script_Combat_GST_CP_FightSallyOn__PJ_Sally_gao,
            4240);
    }
    *(uint32_t*)GST_Script_Combat_GST_CP_FightSallyOn__PJ_Sally_gao_i_Fight_dymmy = 1;
    if (*(float*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ConditionParam_Dimension1, 0, v2->Scr_Condition_Indice) == 6.0)
    {
        v5 = (tGameObjectMain*)GST_Script_Combat_GST_CP_FightSallyOn__PJ_Sally_gao;
        if (GST_Script_Combat_GST_CP_FightSallyOn__PJ_Sally_gao)
        {
            if ((unsigned int)GST_Script_Combat_GST_CP_FightSallyOn__PJ_Sally_gao <= 2)
                v5 = AI_MainActor[GST_Script_Combat_GST_CP_FightSallyOn__PJ_Sally_gao - 1];
            a2 = v5;
        }
        else
        {
            a2 = 0;
        }
        v3 = (tGameObjectMain*)v2->o_ZONE_Fight;
        if (v2->o_ZONE_Fight)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            v0 = AI_EvalFunc_COLBVPivotCollide_C(v3, a2);
        }
        else
        {
            v0 = AI_EvalFunc_COLBVPivotCollide_C(0, a2);
        }
        if (!v0)
            AI_EvalFunc_OBJReinit_C(AI_gpst_CurrentGameObject, 1);
    }
    return 1000;
}

int GST_Script_Combat_GST_Macro_SendMessage()
{
    float v1; // [esp+0h] [ebp-94h]
    float v2; // [esp+8h] [ebp-8Ch]
    tScriptStruct_GSTCombat* v3; // [esp+Ch] [ebp-88h]
    tGameObjectMain* v4; // [esp+14h] [ebp-80h]
    tGameObjectMain* v5; // [esp+20h] [ebp-74h]
    tGameObjectMain* v6; // [esp+24h] [ebp-70h]
    tGameObjectMain* v7; // [esp+2Ch] [ebp-68h]
    float v8; // [esp+38h] [ebp-5Ch]
    double v9; // [esp+3Ch] [ebp-58h]
    float v10; // [esp+44h] [ebp-50h]
    int v11; // [esp+50h] [ebp-44h]
    tMessage Src; // [esp+54h] [ebp-40h] BYREF
    int i; // [esp+88h] [ebp-Ch]
    tScriptStruct_GSTCombat* v14; // [esp+8Ch] [ebp-8h]
    unsigned int v15; // [esp+90h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v14 = v3;
    v15 = 0;
    i = 0;
    Src.msgID = 16;
    v8 = *(float*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, v3->Scr_Indice) / 10000.0f;
    v9 = v8 + MATH_gd_magicDoubleForRound;
    i = (*(uint32_t *)&v9);
    if ((*(uint32_t *)&v9))
    {
        v1 = (float)(10000 * i);
        Src.f2 = *(float*)AI_EvalVarArray2_C(2, (char*)&v14->__Scr_ScriptParam_Dimension1, 0, v14->Scr_Indice) - v1;
        Src.f1 = *(float*)AI_EvalVarArray2_C(2, (char*)&v14->__Scr_ScriptParam_Dimension1, 1, v14->Scr_Indice);
        Src.v1.x = *(float*)AI_EvalVarArray2_C(2, (char*)&v14->__Scr_ScriptParam_Dimension1, 2, v14->Scr_Indice);
        Src.v1.y = 0.0;
        Src.v1.z = 0.0;
        v15 = *(&v14->__Scr_Slot_Dimension1 + i);
        v4 = (tGameObjectMain*)v15;
        if (v15)
        {
            if (v15 <= 2)
                v4 = AI_MainActor[v15 - 1];
            AI_EvalFunc_MSGSend_C(v4, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
    }
    else
    {
        Src.f2 = *(float*)AI_EvalVarArray2_C(2, (char*)&v14->__Scr_ScriptParam_Dimension1, 0, v14->Scr_Indice);
        Src.f1 = *(float*)AI_EvalVarArray2_C(2, (char*)&v14->__Scr_ScriptParam_Dimension1, 1, v14->Scr_Indice);
        v11 = 0;
        for (i = 0; i < v14->Scr_Actor_Nbr; ++i)
        {
            if (v14->Scr_Actor_Nbr <= 1)
            {
                v10 = *(float*)AI_EvalVarArray2_C(2, (char*)&v14->__Scr_ScriptParam_Dimension1, 2, v14->Scr_Indice);
            }
            else
            {
                v2 = (float)v11;
                v10 = *(float*)AI_EvalVarArray2_C(2, (char*)&v14->__Scr_ScriptParam_Dimension1, 2, v14->Scr_Indice) * v2;
            }
            Src.v1.x = v10;
            Src.v1.y = 0.0;
            Src.v1.z = 0.0;
            v15 = v14->Scr_Slot[i];
            v7 = (tGameObjectMain*)v15;
            if (v15)
            {
                if (v15 <= 2)
                    v7 = AI_MainActor[v15 - 1];
                AI_EvalFunc_MSGSend_C(v7, &Src);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &Src);
            }
            v6 = (tGameObjectMain*)v15;
            if (v15)
            {
                if (v15 <= 2)
                    v6 = AI_MainActor[v15 - 1];
                v5 = v6;
            }
            else
            {
                v5 = 0;
            }
            if ((v5->extended->fields->designInt2 & 0x8000) == 0)
                ++v11;
        }
    }
    ++v14->Scr_Indice;
    return 1000;
}

int GST_Script_Combat_GST_Macro_ActiveCP()
{
    tScriptStruct_GSTCombat* v1; // [esp+0h] [ebp-18h]
    double v2; // [esp+8h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v2 = *(float*)AI_EvalVarArray2_C(2, (char*)&v1->__Scr_ScriptParam_Dimension1, 0, v1->Scr_Indice)
        + MATH_gd_magicDoubleForRound;
    v1->Scr_Condition_Indice = (*(uint32_t *)&v2);
    v1->Scr_ConditionOn[v1->Scr_Condition_Indice] = 1;
    ++v1->Scr_Indice;
    return 1000;
}

int GST_Script_Combat_GST_Macro_ActionEnchaine()
{
    float v1; // [esp+0h] [ebp-5Ch]
    tScriptStruct_GSTCombat* v2; // [esp+4h] [ebp-58h]
    tGameObjectMain* v3; // [esp+8h] [ebp-54h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-50h]
    tGameObjectMain* v5; // [esp+10h] [ebp-4Ch]
    tGameObjectMain* v6; // [esp+14h] [ebp-48h]
    tGameObjectMain* v7; // [esp+18h] [ebp-44h]
    tGameObjectMain* v8; // [esp+1Ch] [ebp-40h]
    tGameObjectMain* v9; // [esp+20h] [ebp-3Ch]
    tGameObjectMain* v10; // [esp+24h] [ebp-38h]
    tGameObjectMain* v11; // [esp+28h] [ebp-34h]
    tGameObjectMain* v12; // [esp+2Ch] [ebp-30h]
    tGameObjectMain* v13; // [esp+30h] [ebp-2Ch]
    tGameObjectMain* v14; // [esp+34h] [ebp-28h]
    double v15; // [esp+3Ch] [ebp-20h]
    tGameObjectMain* v16; // [esp+44h] [ebp-18h]
    tGameObjectMain* v17; // [esp+48h] [ebp-14h]
    unsigned int v18; // [esp+4Ch] [ebp-10h]
    int v19; // [esp+50h] [ebp-Ch]
    int i; // [esp+58h] [ebp-4h]
    int j; // [esp+58h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v19 = 0;
    for (i = 0; i < v2->Scr_Actor_Nbr; ++i)
    {
        v18 = v2->Scr_Slot[i];
        if (v2->AA_ActionPrecedente == -1)
        {
            v17 = (tGameObjectMain*)v2->Scr_Slot[i];
            if (v18)
            {
                if (v18 <= 2)
                    v17 = AI_MainActor[v18 - 1];
                v16 = v17;
            }
            else
            {
                v16 = 0;
            }
            v2->AA_ActionPrecedente = v16->extended->fields->designInt1;
            v15 = *(float*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ScriptParam_Dimension1, 0, v2->Scr_Indice)
                + MATH_gd_magicDoubleForRound;
            v14 = (tGameObjectMain*)v18;
            if (v18)
            {
                if (v18 <= 2)
                    v14 = AI_MainActor[v18 - 1];
                v13 = v14;
            }
            else
            {
                v13 = 0;
            }
            v13->extended->fields->designInt1 = (*(uint32_t *)&v15);
        }
        v12 = (tGameObjectMain*)v18;
        if (v18)
        {
            if (v18 <= 2)
                v12 = AI_MainActor[v18 - 1];
            v11 = v12;
        }
        else
        {
            v11 = 0;
        }
        v10 = (tGameObjectMain*)v18;
        if (v18)
        {
            if (v18 <= 2)
                v10 = AI_MainActor[v18 - 1];
            v9 = v10;
        }
        else
        {
            v9 = 0;
        }
        v8 = (tGameObjectMain*)v18;
        if (v18)
        {
            if (v18 <= 2)
                v8 = AI_MainActor[v18 - 1];
            v7 = v8;
        }
        else
        {
            v7 = 0;
        }
        if (v11->extended->fields->designInt1 == (v7->extended->fields->designInt1 & v9->extended->fields->designInt2))
        {
            if (v2->AE_Action1_effectue[i])
            {
                ++v19;
            }
            else
            {
                v2->AE_Action1_effectue[i] = 1;
                v6 = (tGameObjectMain*)v18;
                if (v18)
                {
                    if (v18 <= 2)
                        v6 = AI_MainActor[v18 - 1];
                    v5 = v6;
                }
                else
                {
                    v5 = 0;
                }
                v5->extended->fields->designInt1 = v2->AA_ActionPrecedente;
            }
        }
        else
        {
            v4 = (tGameObjectMain*)v18;
            if (v18)
            {
                if (v18 <= 2)
                    v4 = AI_MainActor[v18 - 1];
                v3 = v4;
            }
            else
            {
                v3 = 0;
            }
            if ((v3->extended->fields->designInt2 & 0x8000) != 0)
                ++v19;
        }
    }
    if (v19 == v2->Scr_Actor_Nbr)
    {
        v2->AA_ActionPrecedente = -1;
        for (j = 0; j < v2->Scr_Actor_Nbr; ++j)
            v2->AE_Action1_effectue[j] = 0;
        v1 = (float)v2->AE_nb;
        if (*(float*)AI_EvalVarArray2_C(2, (char*)&v2->__Scr_ScriptParam_Dimension1, 1, v2->Scr_Indice) == v1)
        {
            ++v2->Scr_Indice;
            v2->AE_nb = 0;
        }
        else
        {
            ++v2->AE_nb;
        }
    }
    return 1000;
}

int GST_Script_Combat_GST_Script_Order_Tikha()
{
    int v1; // [esp+4h] [ebp-1Ch]
    int v2; // [esp+8h] [ebp-18h]
    int v3; // [esp+Ch] [ebp-14h]
    tScriptStruct_GSTCombat* v4; // [esp+10h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v4->__Scr_Macro_Dimension1, 0, 0) = 0;
    v4->Scr_ScriptFunction[0] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 0) = 0x41000000;
    v4->Scr_ScriptFunction[1] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 1) = 0x41800000;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v4->__Scr_Macro_Dimension1, 1, 0) = 2;
    v4->Scr_Indice = 2;
    v4->i_label = 0;
    v4->Scr_Labels[++v4->i_label] = 2;
    v4->Scr_ScriptFunction[2] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 2) = 0x40000000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 2) = 0;
    v4->Scr_Conditions[v4->Scr_Condition_Nb] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 0, v4->Scr_Condition_Nb) = 0x40A00000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 1, v4->Scr_Condition_Nb) = 0x40000000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 2, v4->Scr_Condition_Nb) = 0x40400000;
    v4->Scr_ConditionOn[v4->Scr_Condition_Nb++] = 0;
    v4->Scr_ScriptFunction[3] = 8;
    v3 = v4->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 3) = (float)v3;
    v4->Scr_Conditions[v4->Scr_Condition_Nb] = 3;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 0, v4->Scr_Condition_Nb) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 1, v4->Scr_Condition_Nb) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 2, v4->Scr_Condition_Nb) = 0;
    v4->Scr_ConditionOn[v4->Scr_Condition_Nb++] = 0;
    v4->Scr_ScriptFunction[4] = 8;
    v2 = v4->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 4) = (float)v2;
    v4->Scr_Labels[++v4->i_label] = 5;
    v4->Scr_ScriptFunction[5] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 5) = 0x40000000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 5) = 0x40E00000;
    v4->Scr_ScriptFunction[6] = 14;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 6) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 6) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 2, 6) = 0x3FACCCCD;
    v4->Scr_ScriptFunction[7] = 2;
    v4->Scr_ScriptFunction[7] = 10;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 7) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 7) = 0;
    v4->Scr_ScriptFunction[8] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 8) = 0x3FACCCCD;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 8) = 0;
    v4->Scr_ScriptFunction[9] = 6;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 9) = 0x40000000;
    v4->Scr_Labels[++v4->i_label] = 10;
    v4->Scr_ScriptFunction[10] = 12;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 10) = 0;
    v4->Scr_Conditions[v4->Scr_Condition_Nb] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 0, v4->Scr_Condition_Nb) = 0x40A00000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 1, v4->Scr_Condition_Nb) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 2, v4->Scr_Condition_Nb) = 0x41980000;
    v4->Scr_ConditionOn[v4->Scr_Condition_Nb++] = 0;
    v4->Scr_ScriptFunction[11] = 8;
    v1 = v4->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 11) = (float)v1;
    v4->Scr_Labels[++v4->i_label] = 12;
    v4->Scr_ScriptFunction[12] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 12) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 12) = 0x40E00000;
    v4->Scr_ScriptFunction[13] = 2;
    v4->Scr_ScriptFunction[13] = 10;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 13) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 13) = 0;
    v4->Scr_ScriptFunction[14] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 14) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 14) = 0;
    v4->Scr_ScriptFunction[15] = 6;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 15) = 0x40800000;
    v4->Scr_Labels[19] = 16;
    return 1000;
}

int GST_Script_Combat_GST_CP_AttendPaf()
{
    tScriptStruct_GSTCombat* v1; // [esp+0h] [ebp-2Ch]
    double v2; // [esp+Ch] [ebp-20h]
    tGameObjectMain* v3; // [esp+14h] [ebp-18h]
    tGameObjectMain* v4; // [esp+18h] [ebp-14h]
    int v5; // [esp+20h] [ebp-Ch]
    int i; // [esp+28h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v5 = 0;
    for (i = 0; i < v1->Scr_Actor_Nbr; ++i)
    {
        v4 = (tGameObjectMain*)v1->Scr_Slot[i];
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
        if ((v3->extended->fields->designInt2 & 0x1000) != 0)
        {
            v5 = 1;
            break;
        }
    }
    if (v5
        && *(float*)AI_EvalVarArray2_C(2, (char*)&v1->__Scr_ConditionParam_Dimension1, 0, v1->Scr_Condition_Indice) == 0.0)
    {
        v2 = *(float*)AI_EvalVarArray2_C(2, (char*)&v1->__Scr_ConditionParam_Dimension1, 1, v1->Scr_Condition_Indice)
            + MATH_gd_magicDoubleForRound;
        v1->Scr_Indice = v1->Scr_Labels[(*(uint32_t*)&v2)];
        v1->AA_ActionPrecedente = -1;
        v1->i_Macro_Ind[0] = -1;
    }
    return 1000;
}

int GST_Script_Combat_GST_Script_Order_Sarko()
{
    int v1; // [esp+4h] [ebp-18h]
    int v2; // [esp+8h] [ebp-14h]
    tScriptStruct_GSTCombat* v3; // [esp+Ch] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v3->__Scr_Macro_Dimension1, 0, 0) = 0;
    v3->Scr_ScriptFunction[0] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 0) = 0x42900000;
    v3->Scr_ScriptFunction[1] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 1) = 0x42800000;
    v3->Scr_ScriptFunction[2] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 2) = 0x40C00000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 1, 2) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v3->__Scr_Macro_Dimension1, 1, 0) = 3;
    v3->Scr_Indice = 3;
    v3->i_label = 0;
    v3->Scr_Labels[++v3->i_label] = 3;
    v3->Scr_ScriptFunction[3] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 3) = 0x3F000000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 1, 3) = 0;
    v3->Scr_ScriptFunction[4] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 4) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 1, 4) = 0;
    v3->Scr_Conditions[v3->Scr_Condition_Nb] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 0, v3->Scr_Condition_Nb) = 0x40000000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 1, v3->Scr_Condition_Nb) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 2, v3->Scr_Condition_Nb) = 0x40400000;
    v3->Scr_ConditionOn[v3->Scr_Condition_Nb++] = 0;
    v3->Scr_ScriptFunction[5] = 8;
    v2 = v3->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 5) = (float)v2;
    v3->Scr_Conditions[v3->Scr_Condition_Nb] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 0, v3->Scr_Condition_Nb) = 0x40800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 1, v3->Scr_Condition_Nb) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 2, v3->Scr_Condition_Nb) = 0x41980000;
    v3->Scr_ConditionOn[v3->Scr_Condition_Nb++] = 0;
    v3->Scr_ScriptFunction[6] = 8;
    v1 = v3->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 6) = (float)v1;
    v3->Scr_ScriptFunction[7] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 7) = 0x42800000;
    v3->Scr_Labels[++v3->i_label] = 8;
    v3->Scr_ScriptFunction[8] = 7;
    v3->Scr_ScriptFunction[8] = 11;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 8) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 1, 8) = 0;
    v3->Scr_ScriptFunction[9] = 6;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 9) = 0x40000000;
    v3->Scr_Labels[19] = 10;
    return 1000;
}

int GST_Script_Combat_GST_Script_Order_Memorial()
{
    int v1; // [esp+4h] [ebp-18h]
    int v2; // [esp+8h] [ebp-14h]
    tScriptStruct_GSTCombat* v3; // [esp+Ch] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v3->__Scr_Macro_Dimension1, 0, 0) = 0;
    v3->Scr_ScriptFunction[0] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 0) = 0x3F4CCCCD;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 1, 0) = 0x3F000000;
    v3->Scr_ScriptFunction[1] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 1) = 0x42900000;
    v3->Scr_ScriptFunction[2] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 2) = 0x42800000;
    v3->Scr_ScriptFunction[3] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 3) = 0x41200000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 1, 3) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v3->__Scr_Macro_Dimension1, 1, 0) = 4;
    v3->Scr_Indice = 4;
    v3->i_label = 0;
    v3->Scr_ScriptFunction[4] = 4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 4) = 0x40000000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 1, 4) = 0;
    v3->Scr_Labels[++v3->i_label] = 5;
    v3->Scr_ScriptFunction[5] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 5) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 1, 5) = 0;
    v3->Scr_Conditions[v3->Scr_Condition_Nb] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 0, v3->Scr_Condition_Nb) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 1, v3->Scr_Condition_Nb) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 2, v3->Scr_Condition_Nb) = 0x41980000;
    v3->Scr_ConditionOn[v3->Scr_Condition_Nb++] = 0;
    v3->Scr_ScriptFunction[6] = 8;
    v2 = v3->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 6) = (float)v2;
    v3->Scr_Conditions[v3->Scr_Condition_Nb] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 0, v3->Scr_Condition_Nb) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 1, v3->Scr_Condition_Nb) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ConditionParam_Dimension1, 2, v3->Scr_Condition_Nb) = 0;
    v3->Scr_ConditionOn[v3->Scr_Condition_Nb++] = 0;
    v3->Scr_ScriptFunction[7] = 8;
    v1 = v3->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 7) = (float)v1;
    v3->Scr_ScriptFunction[8] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 8) = 0x42800000;
    v3->Scr_Labels[++v3->i_label] = 9;
    v3->Scr_ScriptFunction[9] = 7;
    v3->Scr_ScriptFunction[9] = 11;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 9) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 1, 9) = 0;
    v3->Scr_ScriptFunction[10] = 6;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 10) = 0x40000000;
    v3->Scr_Labels[++v3->i_label] = 11;
    v3->Scr_ScriptFunction[11] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 11) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 1, 11) = 0x3F800000;
    v3->Scr_ScriptFunction[12] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 12) = 0x41100000;
    v3->Scr_ScriptFunction[13] = 3;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 13) = 0;
    v3->Scr_ScriptFunction[14] = 6;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v3->__Scr_ScriptParam_Dimension1, 0, 14) = 0x3F800000;
    v3->Scr_Labels[19] = 15;
    return 1000;
}

int GST_Script_Combat_GST_Script_Order_SpectreContinu()
{
    int v1; // [esp+4h] [ebp-1Ch]
    int v2; // [esp+8h] [ebp-18h]
    float v3; // [esp+Ch] [ebp-14h]
    tScriptStruct_GSTCombat* v4; // [esp+10h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v4->__Scr_Macro_Dimension1, 0, 0) = 0;
    v4->Scr_ScriptFunction[0] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 0) = 0x41000000;
    v4->Scr_ScriptFunction[1] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 1) = 0x40000000;
    v4->Scr_ScriptFunction[2] = 4;
    v3 = v4->Script_9_delai_entre_attaque;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 2) = v3;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 2) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v4->__Scr_Macro_Dimension1, 1, 0) = 3;
    v4->Scr_Indice = 3;
    v4->i_label = 0;
    v4->Scr_Labels[++v4->i_label] = 3;
    v4->Scr_Conditions[v4->Scr_Condition_Nb] = 3;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 0, v4->Scr_Condition_Nb) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 1, v4->Scr_Condition_Nb) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 2, v4->Scr_Condition_Nb) = 0;
    v4->Scr_ConditionOn[v4->Scr_Condition_Nb++] = 0;
    v4->Scr_ScriptFunction[3] = 8;
    v2 = v4->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 3) = (float)v2;
    v4->Scr_Conditions[v4->Scr_Condition_Nb] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 0, v4->Scr_Condition_Nb) = 0x40A00000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 1, v4->Scr_Condition_Nb) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ConditionParam_Dimension1, 2, v4->Scr_Condition_Nb) = 0x41980000;
    v4->Scr_ConditionOn[v4->Scr_Condition_Nb++] = 0;
    v4->Scr_ScriptFunction[4] = 8;
    v1 = v4->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 4) = (float)v1;
    v4->Scr_Labels[++v4->i_label] = 5;
    v4->Scr_ScriptFunction[5] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 5) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 5) = 0x40000000;
    v4->Scr_ScriptFunction[6] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 6) = 0x40000000;
    v4->Scr_Labels[++v4->i_label] = 7;
    v4->Scr_ScriptFunction[7] = 7;
    v4->Scr_ScriptFunction[7] = 11;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 7) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 1, 7) = 0;
    v4->Scr_ScriptFunction[8] = 6;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v4->__Scr_ScriptParam_Dimension1, 0, 8) = 0x40000000;
    v4->Scr_Labels[19] = 9;
    return 1000;
}

int GST_Script_Combat_GST_Macro_Goto()
{
    tScriptStruct_GSTCombat* v1; // [esp+0h] [ebp-18h]
    double v2; // [esp+8h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (*(float*)AI_EvalVarArray2_C(2, (char*)&v1->__Scr_ScriptParam_Dimension1, 0, v1->Scr_Indice) == -1.0)
    {
        v1->Scr_Indice = v1->Scr_Labels[++v1->Scr_Label_Indice];
    }
    else
    {
        v2 = *(float*)AI_EvalVarArray2_C(2, (char*)&v1->__Scr_ScriptParam_Dimension1, 0, v1->Scr_Indice)
            + MATH_gd_magicDoubleForRound;
        v1->Scr_Indice = v1->Scr_Labels[(*(uint32_t *)&v2)];
    }
    return 1000;
}

int GST_Script_Combat_GST_Macro_ExecMacroSimul()
{
    int result; // eax
    double v1; // st7
    unsigned int v2; // rax
    char* v3; // eax
    float* v4; // eax
    float v5; // [esp+0h] [ebp-D0h]
    float v6; // [esp+4h] [ebp-CCh]
    float v7; // [esp+Ch] [ebp-C4h]
    float v8; // [esp+10h] [ebp-C0h]
    float v9; // [esp+14h] [ebp-BCh]
    float v10; // [esp+18h] [ebp-B8h]
    float v11; // [esp+20h] [ebp-B0h]
    tScriptStruct_GSTCombat* v12; // [esp+24h] [ebp-ACh]
    tGameObjectMain* v13; // [esp+28h] [ebp-A8h]
    tGameObjectMain* v14; // [esp+2Ch] [ebp-A4h]
    double v15; // [esp+30h] [ebp-A0h]
    tGameObjectMain* v16; // [esp+3Ch] [ebp-94h]
    tGameObjectMain* v17; // [esp+40h] [ebp-90h]
    tGameObjectMain* v18; // [esp+44h] [ebp-8Ch]
    tGameObjectMain* v19; // [esp+48h] [ebp-88h]
    tGameObjectMain* v20; // [esp+4Ch] [ebp-84h]
    tGameObjectMain* v21; // [esp+50h] [ebp-80h]
    tGameObjectMain* v22; // [esp+54h] [ebp-7Ch]
    tGameObjectMain* v23; // [esp+58h] [ebp-78h]
    tGameObjectMain* v24; // [esp+5Ch] [ebp-74h]
    tGameObjectMain* v25; // [esp+60h] [ebp-70h]
    tGameObjectMain* v26; // [esp+64h] [ebp-6Ch]
    tGameObjectMain* v27; // [esp+68h] [ebp-68h]
    double v28; // [esp+70h] [ebp-60h]
    int v29[10]; // [esp+84h] [ebp-4Ch]
    float v30; // [esp+ACh] [ebp-24h]
    unsigned int v31; // [esp+B0h] [ebp-20h]
    int v33; // [esp+BCh] [ebp-14h]
    tScriptStruct_GSTCombat* v34; // [esp+C0h] [ebp-10h]
    int i; // [esp+C4h] [ebp-Ch]
    int a4; // [esp+C8h] [ebp-8h]
    int v37; // [esp+CCh] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v12 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v12 = 0;
    v34 = v12;
    a4 = 0;
    v37 = 0;
    v33 = 0;
    v31 = 0;
    i = 0;
    if (v12->Scr_Actor_Nbr)
    {
        if (v34->i_Macro_Ind[0] == -1)
        {
            for (i = 0; i < v34->Scr_Actor_Nbr; ++i)
            {
                v1 = *(float*)AI_EvalVarArray2_C(2, (char*)&v34->__Scr_ScriptParam_Dimension1, 0, v34->Scr_Indice);
                v34->i_Macro_Ind[i] = *(uint32_t*)AI_EvalVarArray2_C(
                    1,
                    (char*)&v34->__Scr_Macro_Dimension1,
                    0,
                    (unsigned int)v1);
            }
        }
        v33 = 0;
        for (i = 0; i < v34->Scr_Actor_Nbr; ++i)
        {
            v29[i] = 0;
            v2 = (unsigned int)*(float*)AI_EvalVarArray2_C(
                2,
                (char*)&v34->__Scr_ScriptParam_Dimension1,
                0,
                v34->Scr_Indice);
            v3 = AI_EvalVarArray2_C(1, (char*)&v34->__Scr_Macro_Dimension1, 1, v2);
            if (v34->i_Macro_Ind[i] < *(int32_t*)v3)
            {
                a4 = v34->i_Macro_Ind[i];
                v37 = v34->Scr_ScriptFunction[a4];
                if (v37 == 2)
                {
                    v31 = v34->Scr_Slot[i];
                    v28 = *(float*)AI_EvalVarArray2_C(2, (char*)&v34->__Scr_ScriptParam_Dimension1, 0, a4)
                        + MATH_gd_magicDoubleForRound;
                    v27 = (tGameObjectMain*)v31;
                    if (v31)
                    {
                        if (v31 <= 2)
                            v27 = AI_MainActor[v31 - 1];
                        v26 = v27;
                    }
                    else
                    {
                        v26 = 0;
                    }
                    v26->extended->fields->designInt1 = (*(uint32_t *)&v28);
                    if (*(float*)AI_EvalVarArray2_C(2, (char*)&v34->__Scr_ScriptParam_Dimension1, 1, v34->Scr_Indice) != 0.0)
                    {
                        v25 = (tGameObjectMain*)v31;
                        if (v31)
                        {
                            if (v31 <= 2)
                                v25 = AI_MainActor[v31 - 1];
                            v24 = v25;
                        }
                        else
                        {
                            v24 = 0;
                        }
                        v11 = (float)i;
                        v24->extended->fields->designFloat1 = *(float*)AI_EvalVarArray2_C(
                            2,
                            (char*)&v34->__Scr_ScriptParam_Dimension1,
                            1,
                            v34->Scr_Indice)
                            * v11;
                    }
                    v31 = v34->Scr_Slot[i];
                    v23 = (tGameObjectMain*)v31;
                    if (v31)
                    {
                        if (v31 <= 2)
                            v23 = AI_MainActor[v31 - 1];
                        v22 = v23;
                    }
                    else
                    {
                        v22 = 0;
                    }
                    v21 = (tGameObjectMain*)v31;
                    if (v31)
                    {
                        if (v31 <= 2)
                            v21 = AI_MainActor[v31 - 1];
                        v20 = v21;
                    }
                    else
                    {
                        v20 = 0;
                    }
                    v19 = (tGameObjectMain*)v31;
                    if (v31)
                    {
                        if (v31 <= 2)
                            v19 = AI_MainActor[v31 - 1];
                        v18 = v19;
                    }
                    else
                    {
                        v18 = 0;
                    }
                    if (v22->extended->fields->designInt1 == (v18->extended->fields->designInt1 & v20->extended->fields->designInt2))
                        goto LABEL_52;
                    v17 = (tGameObjectMain*)v31;
                    if (v31)
                    {
                        if (v31 <= 2)
                            v17 = AI_MainActor[v31 - 1];
                        v16 = v17;
                    }
                    else
                    {
                        v16 = 0;
                    }
                    if ((v16->extended->fields->designInt2 & 0x8000) != 0)
                        LABEL_52:
                    ++v34->i_Macro_Ind[i];
                }
                else if (v37 == 4)
                {
                    if (!v34->Scr_TimeWait[i])
                    {
                        v30 = TIM_gf_MainClock - TIM_gf_LockTime;
                        v15 = v30 + MATH_gd_magicDoubleForRound;
                        v34->Scr_TimeWait[i] = (*(uint32_t *)&v15);
                        v10 = *(float*)AI_EvalVarArray2_C(2, (char*)&v34->__Scr_ScriptParam_Dimension1, 0, a4);
                        v6 = *(float*)AI_EvalVarArray2_C(2, (char*)&v34->__Scr_ScriptParam_Dimension1, 1, a4) + v10;
                        v9 = *(float*)AI_EvalVarArray2_C(2, (char*)&v34->__Scr_ScriptParam_Dimension1, 0, a4);
                        v5 = v9 - *(float*)AI_EvalVarArray2_C(2, (char*)&v34->__Scr_ScriptParam_Dimension1, 1, a4);
                        v8 = AI_EvalFunc_MATHRandFloat_C(v5, v6);
                        *(float*)AI_EvalVarArray2_C(2, (char*)&v34->__Scr_ScriptParam_Dimension1, 2, a4) = v8;
                    }
                    v4 = (float*)AI_EvalVarArray2_C(2, (char*)&v34->__Scr_ScriptParam_Dimension1, 2, a4);
                    v7 = (float)v34->Scr_TimeWait[i];
                    if (AI_EvalFunc_TIMEElapsed_C(v7, *v4))
                        goto LABEL_63;
                    v14 = (tGameObjectMain*)v34->Scr_Slot[i];
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
                    if ((v13->extended->fields->designInt2 & 0x8000) != 0)
                    {
                    LABEL_63:
                        v34->Scr_TimeWait[i] = 0;
                        ++v34->i_Macro_Ind[i];
                    }
                }
            }
            else
            {
                v29[i] = 1;
                v33 += v29[i];
                if (v33 == v34->Scr_Actor_Nbr)
                {
                    ++v34->Scr_Indice;
                    v34->i_Macro_Ind[0] = -1;
                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                    return 1000;
                }
            }
        }
        result = 1000;
    }
    else
    {
        ++v34->Scr_Indice;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int GST_Script_Combat_GST_Script_Order_SpectreProche()
{
    int v1; // [esp+4h] [ebp-20h]
    int v2; // [esp+8h] [ebp-1Ch]
    int v3; // [esp+Ch] [ebp-18h]
    float v4; // [esp+10h] [ebp-14h]
    tScriptStruct_GSTCombat* v5; // [esp+14h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_GSTCombat*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v5->__Scr_Macro_Dimension1, 0, 0) = 0;
    v5->Scr_ScriptFunction[0] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ScriptParam_Dimension1, 0, 0) = 0x41000000;
    v5->Scr_ScriptFunction[1] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ScriptParam_Dimension1, 0, 1) = 0x40000000;
    v5->Scr_ScriptFunction[2] = 4;
    v4 = v5->Script_9_delai_entre_attaque;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ScriptParam_Dimension1, 0, 2) = v4;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ScriptParam_Dimension1, 1, 2) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(1, (char*)&v5->__Scr_Macro_Dimension1, 1, 0) = 3;
    v5->Scr_Indice = 3;
    v5->i_label = 0;
    v5->Scr_Labels[++v5->i_label] = 3;
    v5->Scr_Conditions[v5->Scr_Condition_Nb] = 3;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ConditionParam_Dimension1, 0, v5->Scr_Condition_Nb) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ConditionParam_Dimension1, 1, v5->Scr_Condition_Nb) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ConditionParam_Dimension1, 2, v5->Scr_Condition_Nb) = 0;
    v5->Scr_ConditionOn[v5->Scr_Condition_Nb++] = 0;
    v5->Scr_ScriptFunction[3] = 8;
    v3 = v5->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ScriptParam_Dimension1, 0, 3) = (float)v3;
    v5->Scr_Conditions[v5->Scr_Condition_Nb] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ConditionParam_Dimension1, 0, v5->Scr_Condition_Nb) = 0x40400000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ConditionParam_Dimension1, 1, v5->Scr_Condition_Nb) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ConditionParam_Dimension1, 2, v5->Scr_Condition_Nb) = 0x40000000;
    v5->Scr_ConditionOn[v5->Scr_Condition_Nb++] = 0;
    v5->Scr_ScriptFunction[4] = 8;
    v2 = v5->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ScriptParam_Dimension1, 0, 4) = (float)v2;
    v5->Scr_Conditions[v5->Scr_Condition_Nb] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ConditionParam_Dimension1, 0, v5->Scr_Condition_Nb) = 0x40A00000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ConditionParam_Dimension1, 1, v5->Scr_Condition_Nb) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ConditionParam_Dimension1, 2, v5->Scr_Condition_Nb) = 0x41980000;
    v5->Scr_ConditionOn[v5->Scr_Condition_Nb++] = 0;
    v5->Scr_ScriptFunction[5] = 8;
    v1 = v5->Scr_Condition_Nb - 1;
    *(float*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ScriptParam_Dimension1, 0, 5) = (float)v1;
    v5->Scr_Labels[++v5->i_label] = 6;
    v5->Scr_ScriptFunction[6] = 1;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ScriptParam_Dimension1, 0, 6) = 0x3F800000;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ScriptParam_Dimension1, 1, 6) = 0x40000000;
    v5->Scr_ScriptFunction[7] = 2;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ScriptParam_Dimension1, 0, 7) = 0x40000000;
    v5->Scr_Labels[++v5->i_label] = 8;
    v5->Scr_ScriptFunction[8] = 7;
    v5->Scr_ScriptFunction[8] = 11;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ScriptParam_Dimension1, 0, 8) = 0;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ScriptParam_Dimension1, 1, 8) = 0;
    v5->Scr_ScriptFunction[9] = 6;
    *(uint32_t*)AI_EvalVarArray2_C(2, (char*)&v5->__Scr_ScriptParam_Dimension1, 0, 9) = 0x40000000;
    v5->Scr_Labels[19] = 10;
    return 1000;
}

// ==================================

int Monstroplante_Gamin_MPG_run()
{
    int result; // eax
    float a4; // [esp+0h] [ebp-6Ch]
    tScriptStruct_MonstroplanteGamin* v2; // [esp+8h] [ebp-64h]
    tGameObjectMain* v3; // [esp+10h] [ebp-5Ch]
    Vec3 a3; // [esp+18h] [ebp-54h] BYREF
    int v5; // [esp+24h] [ebp-48h]
    Vec3 a2; // [esp+28h] [ebp-44h] BYREF
    Vec3 a5; // [esp+34h] [ebp-38h] BYREF
    Vec3 a1; // [esp+40h] [ebp-2Ch] BYREF
    int v9; // [esp+4Ch] [ebp-20h]
    Vec3 v10; // [esp+50h] [ebp-1Ch] BYREF
    tScriptStruct_MonstroplanteGamin* v11; // [esp+5Ch] [ebp-10h]
    Vec3 v12; // [esp+60h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_MonstroplanteGamin*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v11 = v2;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
    v3 = (tGameObjectMain*)v11->o_target;
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        AI_EvalFunc_OBJGetPos_C(v3, &v10);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v10);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
    MATH_SubstractVec(&a3, &v10, &a1);
    a4 = 5.0f * TIM_gf_dt;
    AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
    v12.x = 0.0;
    v12.y = 0.0;
    v12.z = 1.0;
    AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a5, &v12);
    v5 = (unsigned __int16)AI_gpst_CurrentGameObject->flags2;
    if ((v5 & 4) == 0 || (v9 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject), v9 == 1000))
        result = 1000;
    else
        result = v9;
    return result;
}

int Monstroplante_Gamin_MPG_init()
{
    int result; // eax
    tScriptStruct_MonstroplanteGamin* v1; // [esp+4h] [ebp-C4h]
    tGameObjectMain* v2; // [esp+Ch] [ebp-BCh]
    Vec3 v3; // [esp+2Ch] [ebp-9Ch] BYREF
    Vec3 v4; // [esp+38h] [ebp-90h] BYREF
    Vec3 a1; // [esp+44h] [ebp-84h] BYREF
    Vec3 v6; // [esp+50h] [ebp-78h] BYREF
    Vec3 v7; // [esp+5Ch] [ebp-6Ch] BYREF
    Vec3 v8; // [esp+68h] [ebp-60h] BYREF
    int v10; // [esp+78h] [ebp-50h]
    Vec3 v11; // [esp+7Ch] [ebp-4Ch] BYREF
    int v12; // [esp+88h] [ebp-40h]
    Vec3 dest; // [esp+8Ch] [ebp-3Ch] BYREF
    int v14; // [esp+98h] [ebp-30h]
    BOOL v15; // [esp+9Ch] [ebp-2Ch]
    tScriptStruct_MonstroplanteGamin* v16; // [esp+A0h] [ebp-28h]
    Vec3 v17; // [esp+A4h] [ebp-24h] BYREF
    Vec3 a3; // [esp+B0h] [ebp-18h] BYREF
    Vec3 v19; // [esp+BCh] [ebp-Ch]

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_MonstroplanteGamin*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v16 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Monstroplante_Gamin_MPG_init__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        fct_Monstroplante_Gamin_MPG_init_Monstroplante_Gamin_MPG_Jump = (int)AI_ul_RealResolveOneRefFunction(0x260028FAu);
    }
    if (*AI_gppst_CurrentJumpNode != (tAINode*)1)
    {
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        AI_EvalFunc_ACTActionSet_C_CURRENT(0);
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
    }
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(256))
    {
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 1, 0);
        AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0xFFu, 0);
        v15 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
        v14 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 4100, 0);
        v4.x = 0.0;
        v4.y = 0.0;
        v4.z = -9.0;
        AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v4);
        v3.x = 10.0;
        v3.y = 5.0;
        v3.z = 0.0;
        v10 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v3);
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Monstroplante_Gamin_MPG_init__PJ_Sally_gao, &a3);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v7);
        MATH_SubstractVec(&dest, &a3, &v7);
        a1.x = dest.x;
        a1.y = dest.y;
        a1.z = 0.0;
        MATH_NormalizeEqualVector(&a1);
        v17.x = 0.0;
        v17.y = 0.0;
        v17.z = 1.0;
        AI_EvalFunc_MATHVecCrossProduct_C(&a1, &v17, &v8);
        a1 = v8;
        a1.x = a1.x + a1.x;
        a1.y = a1.y + a1.y;
        a1.z = a1.z + a1.z;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v11);
        a1.x = a1.x + v11.x;
        a1.y = a1.y + v11.y;
        a1.z = a1.z + v11.z;
        v2 = (tGameObjectMain*)v16->o_z_ground;
        if (v16->o_z_ground)
        {
            if ((unsigned int)v2 <= 2)
                v2 = AI_MainActor[(uint32_t)v2 - 1];
            AI_EvalFunc_OBJGetPos_C(v2, &v6);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v6);
        }
        a1.z = v6.z;
        v19.x = 0.0;
        v19.y = 0.0;
        v19.z = 0.0099999998f;
        AI_EvalFunc_ACTActionSet_C_CURRENT(5);
        AI_EvalFunc_DYNImpulsion_C(AI_gpst_CurrentGameObject, &a1, 0.0099999998f);
        v12 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Monstroplante_Gamin_MPG_init_Monstroplante_Gamin_MPG_Jump);
        if (v12 == 1000)
            result = 1000;
        else
            result = v12;
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)1;
        result = 0;
    }
    return result;
}

int Monstroplante_Gamin_MPG_Jump()
{
    int result; // eax
    float a4; // [esp+0h] [ebp-40h]
    tScriptStruct_MonstroplanteGamin* v2; // [esp+8h] [ebp-38h]
    Vec3 a5; // [esp+10h] [ebp-30h] BYREF
    Vec3 a2; // [esp+1Ch] [ebp-24h] BYREF
    int v5; // [esp+28h] [ebp-18h]
    float v6; // [esp+2Ch] [ebp-14h]
    Vec3 a3; // [esp+30h] [ebp-10h] BYREF
    tScriptStruct_MonstroplanteGamin* v8; // [esp+3Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_MonstroplanteGamin*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v8 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Monstroplante_Gamin_MPG_Jump_Monstroplante_Gamin_MPG_run = (int)AI_ul_RealResolveOneRefFunction(0x26002913u);
    }
    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&a2);
    a3.x = 0.0;
    a3.y = 0.0;
    a3.z = 1.0;
    v6 = TIM_gf_dt;
    a4 = 10.0f * TIM_gf_dt;
    AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
    AI_EvalFunc_OBJ_BankingSet_C(AI_gpst_CurrentGameObject, &a5);
    if (!AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 64)
        || (AI_EvalFunc_ACTActionSet_C_CURRENT(2),
            AI_EvalFunc_ANIFrequencySet_C(AI_gpst_CurrentGameObject, 0, v8->i_freq),
            v5 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Monstroplante_Gamin_MPG_Jump_Monstroplante_Gamin_MPG_run),
            v5 == 1000))
    {
        result = 1000;
    }
    else
    {
        result = v5;
    }
    return result;
}