#include "ai_compiledscripts_inc.h"

//GST_Carte3D_Init
int GST_Carte3D_Init_univVars;
int GST_Carte3D_Init_univVars_Flag_speciaux_sauve;
int GST_Carte3D_Init_univVars_Carte3D_o_Object;
int GST_Carte3D_Init_univVars_Carte3D_i_CurCarte;
int GST_Carte3D_Init_univVars_Carte3D_i_CurEtage;
int GST_Carte3D_Init_univVars_Perso_ID_CarteEtage;
int GST_Carte3D_Init_univVars_world_ID;
int fct_GST_Carte3D_Init_GST_Carte3D_Close;

//GST_Carte3D_Close
int GST_Carte3D_Close_INV_manager_gao;
int fct_GST_Carte3D_Close_GST_Carte3D_Open;

//GST_Carte3D_Open
int GST_Carte3D_Open_univVars;
int GST_Carte3D_Open_univVars_Carte3D_i_CurEtage;
int fct_GST_Carte3D_Open_GST_Carte3D_exec_UpdatePiece;
int GST_Carte3D_Open_univVars_Carte_ao_Object;
int GST_Carte3D_Open_univVars_Perso_ID_CarteEtage;
int GST_Carte3D_Open_univVars_Flag_speciaux_sauve;
int GST_Carte3D_Open_INV_manager_gao;
int GST_Carte3D_Open_INV_manager_gao_i_Animals_Nb;
int GST_Carte3D_Open_INV_manager_gao_ao_Animals;
int GST_Carte3D_Open_INV_manager_gao_ai_AnimalsData;
int fct_GST_Carte3D_Open_GST_Carte3D_Loop;

//GST_Carte3D_exec_UpdatePiece
int GST_Carte3D_exec_UpdatePiece_univVars;
int GST_Carte3D_exec_UpdatePiece_univVars_Flag_speciaux_sauve;

//GST_Carte3D_Loop
int fct_GST_Carte3D_Loop_GST_Carte3D_Close;
int fct_GST_Carte3D_Loop_GST_Carte3D_exec_UpdatePiece;
int GST_Carte3D_Loop_univVars;
int GST_Carte3D_Loop_univVars_Carte_i_NbSpot;
int GST_Carte3D_Loop_univVars_Carte_ao_Object;
int fct_GST_Carte3D_Loop_GST_Carte3D_exec_Spot;

//GST_Carte3D_exec_Spot
int GST_Carte3D_exec_Spot_INV_manager_gao;
int GST_Carte3D_exec_Spot_univVars;
int GST_Carte3D_exec_Spot_univVars_Perso_ID_actor;
int GST_Carte3D_exec_Spot_univVars_Perso_ID_passengers_list;
int GST_Carte3D_exec_Spot_univVars_Perso_ID_CarteEtage;
int GST_Carte3D_exec_Spot_univVars_Flag_speciaux_sauve;
int GST_Carte3D_exec_Spot_univVars_Perso_ID_world_ID;
int GST_Carte3D_exec_Spot_univVars_Perso_ID_pos;
int fct_GST_Carte3D_exec_Spot_GST_Carte3D_exec_SpotInsert;
int GST_Carte3D_exec_Spot_INV_manager_gao_i_MailCur;
int GST_Carte3D_exec_Spot_INV_manager_gao_ai_Note;
int GST_Carte3D_exec_Spot_Carte_ao_Object;

//GST_Carte3D_exec_SpotInsert
int GST_Carte3D_exec_SpotInsert_INV_manager_gao;

int GST_Carte3D_Init()
{
    int result; // eax
    tScriptStruct_GSTCarte3D* v1; // [esp+0h] [ebp-58h]
    int v2; // [esp+48h] [ebp-10h]
    int a2; // [esp+54h] [ebp-4h]
    int a2a; // [esp+54h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GSTCarte3D*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        GST_Carte3D_Init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&GST_Carte3D_Init_univVars_Flag_speciaux_sauve, (void*)GST_Carte3D_Init_univVars, 4488);
        AI2C_ResolveBuffer(&GST_Carte3D_Init_univVars_Carte3D_o_Object, (void*)GST_Carte3D_Init_univVars, 4476);
        AI2C_ResolveBuffer(&GST_Carte3D_Init_univVars_Carte3D_i_CurCarte, (void*)GST_Carte3D_Init_univVars, 4480);
        AI2C_ResolveBuffer(&GST_Carte3D_Init_univVars_Carte3D_i_CurEtage, (void*)GST_Carte3D_Init_univVars, 4484);
        AI2C_ResolveBuffer(&GST_Carte3D_Init_univVars_Perso_ID_CarteEtage, (void*)GST_Carte3D_Init_univVars, 1044);
        AI2C_ResolveBuffer(&GST_Carte3D_Init_univVars_world_ID, (void*)GST_Carte3D_Init_univVars, 4);
        fct_GST_Carte3D_Init_GST_Carte3D_Close = (int)AI_ul_RealResolveOneRefFunction(0x4901E6E7u);
    }
    if (v1->des_i_CarteNum == 15
        && AI_EvalFunc_ARRBitTest_C((int*)(GST_Carte3D_Init_univVars_Flag_speciaux_sauve + 4), 2452))
    {
        v1->des_i_NbEtages = 3;
    }
    v1->f_FlecheTimer = 0.0;
    if (v1->des_i_ForceTaken && v1->des_i_CarteNum + 79 > 0)
        AI_EvalFunc_ARRBitSet_C((int*)(GST_Carte3D_Init_univVars_Flag_speciaux_sauve + 4), v1->des_i_CarteNum + 79);
    *(uint32_t*)GST_Carte3D_Init_univVars_Carte3D_o_Object = (uint32_t)AI_gpst_CurrentGameObject;
    *(uint32_t*)GST_Carte3D_Init_univVars_Carte3D_i_CurCarte = v1->des_i_CarteNum;
    if (*(int*)GST_Carte3D_Init_univVars_Carte3D_i_CurEtage < 0
        || *(int32_t*)GST_Carte3D_Init_univVars_Carte3D_i_CurEtage >= v1->des_i_NbEtages)
    {
        *(uint32_t*)GST_Carte3D_Init_univVars_Carte3D_i_CurEtage = v1->des_i_DefaultEtage;
    }
    for (a2 = 0; a2 < 12; ++a2)
    {
        if (*(int*)(GST_Carte3D_Init_univVars_Perso_ID_CarteEtage + 4 * a2 + 4) < 0
            || *(int32_t*)(GST_Carte3D_Init_univVars_Perso_ID_CarteEtage + 4 * a2 + 4) >= v1->des_i_NbEtages)
        {
            *(uint32_t*)(GST_Carte3D_Init_univVars_Perso_ID_CarteEtage + 4 * a2 + 4) = v1->des_i_DefaultEtage;
        }
    }
    for (a2a = 0; a2a < v1->des_i_MapNb; ++a2a)
    {
        if (v1->des_ai_MapIndex[a2a] == *(uint32_t*)GST_Carte3D_Init_univVars_world_ID)
        {
            v1->i_CurMap = a2a;
            v1->v_Offset = *(Vec3*)AI_EvalVarArray_C3((char*)&v1->__des_av_MapOffset_Dimension1, a2a);
            v1->i_Rotation = v1->des_ai_MapRotation[a2a];
            v1->i_FixedSpot = v1->des_ai_FixedSpot[a2a];
        }
    }
    v1->ai_CompState[0] = 0xE210011;
    v1->ai_CompState[1] = 0xE201011;
    v1->ai_CompState[2] = 0xE2B2011;
    v1->ai_CompState[3] = 0xE263011;
    v1->ai_CompState[4] = 0xE264011;
    v1->ai_CompState[5] = 0xE265011;
    v1->ai_CompState[6] = 0xE266011;
    v1->ai_CompState[7] = 0xE267011;
    v1->ai_CompState[8] = 0xE228011;
    v1->ai_CompState[9] = 0xE279011;
    v1->ai_CompState[10] = 0xE23A011;
    v1->i_Edit_CurPage = 0;
    v1->i_Edit_Page0_CurLine = 0;
    v1->i_Edit_Page1_CurLine = 0;
    v1->f_Edit_Delay = 0.0;
    v1->i_Edit_ShowAnimalsStatic = 1;
    v1->i_Edit_ShowAnimalsDyn = 1;
    v2 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_GST_Carte3D_Init_GST_Carte3D_Close);
    if (v2 == 1000)
        result = 1000;
    else
        result = v2;
    return result;
}

int GST_Carte3D_Close()
{
    tScriptStruct_GSTCarte3D* v1; // [esp+0h] [ebp-2Ch]
    tGameObjectMain* v2; // [esp+8h] [ebp-24h]
    tGameObjectMain* v3; // [esp+10h] [ebp-1Ch]
    tGameObjectMain* v4; // [esp+1Ch] [ebp-10h]
    tGameObjectMain* v5; // [esp+20h] [ebp-Ch]
    int i; // [esp+28h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_GSTCarte3D*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        GST_Carte3D_Close_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        fct_GST_Carte3D_Close_GST_Carte3D_Open = (int)AI_ul_RealResolveOneRefFunction(0x4901E6E8u);
    }
    for (i = 0; i < v1->des_i_NbEtages; ++i)
    {
        v5 = (tGameObjectMain*)v1->des_ao_Etage[i];
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
            v3 = (tGameObjectMain*)v1->des_ao_Etage[i];
            if (v3)
            {
                if ((unsigned int)v3 <= 2)
                    v3 = AI_MainActor[(uint32_t)v3 - 1];
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(v3, 1);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
            }
        }
    }
    v2 = (tGameObjectMain*)v1->des_o_Spot;
    if (v2)
    {
        if ((unsigned int)v2 <= 2)
            v2 = AI_MainActor[(uint32_t)v2 - 1];
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v2, 1);
    }
    else
    {
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
    }
    STR_SetString(
        (tGameObjectMain*)GST_Carte3D_Close_INV_manager_gao,
        20,
        (char*)AI_gpst_CurrentFunction->localStringConstsData);
    STR_SetString(
        (tGameObjectMain*)GST_Carte3D_Close_INV_manager_gao,
        19,
        (char*)AI_gpst_CurrentFunction->localStringConstsData + 1);
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_GST_Carte3D_Close_GST_Carte3D_Open);
    AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
    AI_EvalFunc_OBJ_FlagInactiveSet_C(AI_gpst_CurrentGameObject, 1);
    return 1000;
}

int GST_Carte3D_Open()
{
    int result; // eax
    int v1; // eax
    double v2; // st7
    char* v3; // eax
    char* v4; // eax
    int v5; // eax
    float X; // [esp+0h] [ebp-348h]
    tScriptStruct_GSTCarte3D* v7; // [esp+1Ch] [ebp-32Ch]
    tGameObjectMain* v8; // [esp+20h] [ebp-328h]
    tGameObjectMain* v9; // [esp+24h] [ebp-324h]
    tGameObjectMain* v10; // [esp+2Ch] [ebp-31Ch]
    tGameObjectMain* v11; // [esp+30h] [ebp-318h]
    tGameObjectMain* v12; // [esp+34h] [ebp-314h]
    Vec3* v13; // [esp+5Ch] [ebp-2ECh]
    Vec3* v14; // [esp+60h] [ebp-2E8h]
    tGameObjectMain* v15; // [esp+68h] [ebp-2E0h]
    tGameObjectMain* v16; // [esp+78h] [ebp-2D0h]
    Vec3 v17; // [esp+7Ch] [ebp-2CCh] BYREF
    Vec3 a3; // [esp+98h] [ebp-2B0h] BYREF
    int v19; // [esp+A4h] [ebp-2A4h]
    float v20; // [esp+ACh] [ebp-29Ch]
    float v21; // [esp+B0h] [ebp-298h]
    int v22; // [esp+B4h] [ebp-294h]
    int v23[66]; // [esp+B8h] [ebp-290h]
    int v24; // [esp+1C0h] [ebp-188h]
    int v25; // [esp+1C4h] [ebp-184h]
    int v26; // [esp+1C8h] [ebp-180h]
    int v27; // [esp+1D0h] [ebp-178h]
    float v28; // [esp+1E4h] [ebp-164h]
    int v29; // [esp+1ECh] [ebp-15Ch]
    BOOL v30; // [esp+1F0h] [ebp-158h]
    tGameObjectMain* v31; // [esp+1F4h] [ebp-154h]
    BOOL v32; // [esp+1FCh] [ebp-14Ch]
    float v33[66]; // [esp+200h] [ebp-148h]
    Vec3 v34; // [esp+308h] [ebp-40h] BYREF
    Vec3 a5; // [esp+318h] [ebp-30h] BYREF
    int j; // [esp+324h] [ebp-24h]
    tScriptStruct_GSTCarte3D* v37; // [esp+32Ch] [ebp-1Ch]
    float v38; // [esp+330h] [ebp-18h]
    int i; // [esp+334h] [ebp-14h]
    float v40; // [esp+33Ch] [ebp-Ch]
    BOOL v41; // [esp+340h] [ebp-8h]
    int v42; // [esp+344h] [ebp-4h]

    v17.x = 1.0;
    v17.y = 0.0;
    v17.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_GSTCarte3D*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    v37 = v7;
    j = 0;
    v19 = 0;
    v29 = 0;
    v38 = 0.0;
    v24 = 0;
    v30 = 0;
    v27 = 0;
    v31 = 0;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        GST_Carte3D_Open_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&GST_Carte3D_Open_univVars_Carte3D_i_CurEtage, (void*)GST_Carte3D_Open_univVars, 4484);
        fct_GST_Carte3D_Open_GST_Carte3D_exec_UpdatePiece = (int)AI_ul_RealResolveOneRefFunction(0x4901F0ECu);
        AI2C_ResolveBuffer(&GST_Carte3D_Open_univVars_Carte_ao_Object, (void*)GST_Carte3D_Open_univVars, 4304);
        AI2C_ResolveBuffer(&GST_Carte3D_Open_univVars_Perso_ID_CarteEtage, (void*)GST_Carte3D_Open_univVars, 1044);
        AI2C_ResolveBuffer(&GST_Carte3D_Open_univVars_Flag_speciaux_sauve, (void*)GST_Carte3D_Open_univVars, 4488);
        GST_Carte3D_Open_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(&GST_Carte3D_Open_INV_manager_gao_i_Animals_Nb, (void*)GST_Carte3D_Open_INV_manager_gao, 23888);
        AI2C_ResolveBuffer(&GST_Carte3D_Open_INV_manager_gao_ao_Animals, (void*)GST_Carte3D_Open_INV_manager_gao, 23368);
        AI2C_ResolveBuffer(
            &GST_Carte3D_Open_INV_manager_gao_ai_AnimalsData,
            (void*)GST_Carte3D_Open_INV_manager_gao,
            23628);
        fct_GST_Carte3D_Open_GST_Carte3D_Loop = (int)AI_ul_RealResolveOneRefFunction(0x4901FDD2u);
    }
    if (v37->des_i_NbEtages)
    {
        v37->i_EtageCur = *(uint32_t*)GST_Carte3D_Open_univVars_Carte3D_i_CurEtage;
        v37->i_EtageRequired = v37->i_EtageCur;
        for (i = 0; i < v37->des_i_NbEtages; ++i)
            *(uint32_t*)AI_EvalVarArray_C2((char*)&v37->__f_EtageState_Dimension1, i) = 0;
        *(uint32_t*)AI_EvalVarArray_C2((char*)&v37->__f_EtageState_Dimension1, v37->i_EtageCur) = 1065353216;
        if (v37->i_EtageCur > 0)
            *(uint32_t*)AI_EvalVarArray_C2((char*)&v37->__f_EtageState_Dimension1, v37->i_EtageCur - 1) = 1041865114;
        v37->des_i_Close = 0;
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
        v16 = (tGameObjectMain*)v37->des_o_Spot;
        if (v16)
        {
            if ((unsigned int)v16 <= 2)
                v16 = AI_MainActor[(uint32_t)v16 - 1];
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(v16, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
        }
        v26 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Carte3D_Open_GST_Carte3D_exec_UpdatePiece);
        if (v26 == 1000)
        {
            *(uint32_t*)(GST_Carte3D_Open_univVars_Carte_ao_Object + 4) = (uint32_t)*AI_MainActor;
            if (v37->des_i_CarteNum == 4)
                *(uint32_t*)(GST_Carte3D_Open_univVars_Perso_ID_CarteEtage + 44) = 1;
            v37->des_i_CanZoom = 1;
            AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, v37->des_f_Zoom);
            v31 = AI_EvalFunc_VIEWGetObject_C(0);
            v15 = v31;
            if (v31)
            {
                if ((unsigned int)v31 <= 2)
                    v15 = AI_MainActor[(uint32_t)v31 - 1];
                AI_EvalFunc_OBJGetPos_C(v15, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            MATH_VecAddVec(&v37->v_SavePos, &a3, &v37->des_v_Pos);
            v13 = &v37->v_ZoomPos;
            v14 = &v37->v_SavePos;
            v37->v_ZoomPos.x = v37->v_SavePos.x;
            v13->y = v14->y;
            v13->z = v14->z;
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v37->v_SavePos);
            v28 = sinf(v37->des_f_Alpha);
            v40 = cosf(v37->des_f_Alpha);
            v20 = v28;
            v21 = -v40;
            v22 = 0;
            v17.x = v28;
            v17.y = v21;
            v17.z = 0.0;
            v34.x = 0.0;
            v34.y = 0.0;
            v34.z = 1.0;
            AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v17, &v34);
            AI_EvalFunc_OBJRotateGlobalX_C(AI_gpst_CurrentGameObject, v37->des_f_Beta);
            v37->f_ZoomScale = 0.0099999998f;
            v37->i_Appear = 1;
            v37->f_ZoomMinTime = 0.0;
            if (v37->des_i_PearlNbGroup)
            {
                for (i = 0; i < v37->des_i_PearlNbGroup; ++i)
                {
                    v37->des_i_PearlGroupNb[i] = 0;
                    for (j = v37->des_i_PearlGroupFirst[i]; j < v37->des_i_PearlGroupFirst[i + 1]; ++j)
                    {
                        v1 = AI_EvalFunc_MTHModulo_C(v37->des_ai_Pearl[j], 100);
                        v27 = v1;
                        if (v1 + 2900 <= 0)
                            v30 = 0;
                        else
                            v30 = AI_EvalFunc_ARRBitTest_C((int*)(GST_Carte3D_Open_univVars_Flag_speciaux_sauve + 4), v27 + 2900);
                        if (!v30)
                        {
                            v19 = 1;
                            v24 = 1;
                            if (v37->des_ai_PearlSF[j])
                            {
                                if (v37->des_ai_PearlSF[j] <= 0)
                                {
                                    v19 = 0;
                                }
                                else if (v37->des_ai_PearlSF[j] < 10000)
                                {
                                    v19 = AI_EvalFunc_ARRBitTest_C(
                                        (int*)(GST_Carte3D_Open_univVars_Flag_speciaux_sauve + 4),
                                        v37->des_ai_PearlSF[j]);
                                }
                                else
                                {
                                    v32 = AI_EvalFunc_ARRBitTest_C(
                                        (int*)(GST_Carte3D_Open_univVars_Flag_speciaux_sauve + 4),
                                        v37->des_ai_PearlSF[j] - 10000);
                                    v19 = 1 - v32;
                                }
                            }
                            if (v37->des_ai_PearlSF2[j])
                            {
                                if (v37->des_ai_PearlSF2[j] <= 0)
                                {
                                    v24 = 0;
                                }
                                else if (v37->des_ai_PearlSF2[j] < 10000)
                                {
                                    v24 = AI_EvalFunc_ARRBitTest_C(
                                        (int*)(GST_Carte3D_Open_univVars_Flag_speciaux_sauve + 4),
                                        v37->des_ai_PearlSF2[j]);
                                }
                                else
                                {
                                    v41 = AI_EvalFunc_ARRBitTest_C(
                                        (int*)(GST_Carte3D_Open_univVars_Flag_speciaux_sauve + 4),
                                        v37->des_ai_PearlSF2[j] - 10000);
                                    v24 = 1 - v41;
                                }
                            }
                            if (v19)
                            {
                                if (v24)
                                {
                                    v25 = AI_EvalFunc_MTHModulo_C(v37->des_ai_Pearl[j], 100);
                                    v27 = v37->des_ai_Pearl[j] - v25;
                                    v27 /= 100;
                                    v37->des_i_PearlGroupNb[i] += v27 + 1;
                                }
                            }
                        }
                    }
                }
            }
            for (i = 0; i < 65; ++i)
                v23[i] = -1;
            for (i = 0; i < *(int32_t*)GST_Carte3D_Open_INV_manager_gao_i_Animals_Nb; ++i)
            {
                v31 = *(tGameObjectMain**)(GST_Carte3D_Open_INV_manager_gao_ao_Animals + 4 * i + 4);
                v29 = (unsigned __int8)*(uint32_t*)(GST_Carte3D_Open_INV_manager_gao_ai_AnimalsData + 4 * i + 4);
                if (v29 + 800 <= 0)
                    v30 = 0;
                else
                    v30 = AI_EvalFunc_ARRBitTest_C((int*)(GST_Carte3D_Open_univVars_Flag_speciaux_sauve + 4), v29 + 800);
                if (!v30)
                {
                    v12 = *(tGameObjectMain**)(GST_Carte3D_Open_univVars_Carte_ao_Object + 4);
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
                    v10 = v31;
                    if (v31)
                    {
                        if ((unsigned int)v31 <= 2)
                            v10 = AI_MainActor[(uint32_t)v31 - 1];
                        v2 = AI_EvalFunc_OBJ_SqrDist_C(v10, v11);
                    }
                    else
                    {
                        v2 = AI_EvalFunc_OBJ_SqrDist_C(0, v11);
                    }
                    v38 = (float)v2;
                    if (v23[v29] == -1 || v33[v29] > (double)v38)
                    {
                        v23[v29] = i;
                        v33[v29] = v38;
                    }
                }
            }
            v37->i_AnimalDynNb = 0;
            for (i = 1; i < 65; ++i)
            {
                if (v23[i] != -1)
                {
                    v31 = *(tGameObjectMain**)(GST_Carte3D_Open_INV_manager_gao_ao_Animals + 4 * v23[i] + 4);
                    v9 = v31;
                    if (v31)
                    {
                        if ((unsigned int)v31 <= 2)
                            v9 = AI_MainActor[(uint32_t)v31 - 1];
                        v8 = v9;
                    }
                    else
                    {
                        v8 = 0;
                    }
                    v3 = AI_EvalVarArray_C3((char*)&v37->__av_AnimalDynPos_Dimension1, v37->i_AnimalDynNb);
                    AI_EvalFunc_OBJGetPos_C(v8, (Vec3*)v3);
                    v37->ai_AnimalDynData[v37->i_AnimalDynNb] = *(uint32_t*)(GST_Carte3D_Open_INV_manager_gao_ai_AnimalsData
                        + 4 * v23[i]
                        + 4);
                    if (v37->ai_AnimalDynData[v37->i_AnimalDynNb] >> 8 == 100)
                    {
                        v38 = *((float*)AI_EvalVarArray_C3((char*)&v37->__av_AnimalDynPos_Dimension1, v37->i_AnimalDynNb) + 2);
                        for (j = 0; j < v37->des_i_NbEtages - 1; ++j)
                        {
                            v4 = AI_EvalVarArray_C2((char*)&v37->__des_af_EtageZ_Dimension1, j);
                            if (*(float*)v4 > (double)v38)
                                break;
                        }
                        v37->ai_AnimalDynData[v37->i_AnimalDynNb] = (j << 8)
                            + (unsigned __int8)v37->ai_AnimalDynData[v37->i_AnimalDynNb];
                    }
                    ++v37->i_AnimalDynNb;
                }
            }
            for (i = 0; i < v37->des_i_NbAnimals; ++i)
            {
                v37->des_ai_AnimalState[i] &= 0xFFFFFFFB;
                if ((v37->des_ai_AnimalState[i] & 2) != 0)
                {
                    if (v37->des_ai_AnimalMap[i] != v37->i_CurMap || v23[v37->des_ai_Animal[i]] == -1)
                        v37->des_ai_AnimalState[i] &= 0xFFFFFFFE;
                    else
                        v37->des_ai_AnimalState[i] |= 1u;
                }
                else if (v23[v37->des_ai_Animal[i]] != -1)
                {
                    v37->des_ai_AnimalState[i] |= 4u;
                }
            }
            if (!v37->i_FixedSpot)
            {
                a5.x = -5.0;
                a5.y = -5.0;
                a5.z = 0.0;
                STR_SetStringExt(
                    (tGameObjectMain*)GST_Carte3D_Open_INV_manager_gao,
                    19,
                    1507349,
                    (char*)AI_gpst_CurrentFunction->localStringConstsData,
                    &a5);
                STR_EffectSeti((tGameObjectMain*)GST_Carte3D_Open_INV_manager_gao, 1, -1, 19);
                v5 = STR_i_EffectAdd((void*)GST_Carte3D_Open_INV_manager_gao, 15, 19, 0, -1);
                v37->i_EffectRot = v5;
            }
            X = v37->des_f_Zoom * v37->f_ZoomScale;
            AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, X);
            v42 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_GST_Carte3D_Open_GST_Carte3D_Loop);
            if (v42 == 1000)
                result = 1000;
            else
                result = v42;
        }
        else
        {
            result = v26;
        }
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int GST_Carte3D_exec_UpdatePiece()
{
    float* v0; // eax
    float* v1; // eax
    float v3; // [esp+4h] [ebp-B4h]
    float v4; // [esp+8h] [ebp-B0h]
    float v5; // [esp+Ch] [ebp-ACh]
    char* v6; // [esp+10h] [ebp-A8h]
    float v7; // [esp+14h] [ebp-A4h]
    float v8; // [esp+18h] [ebp-A0h]
    char* v9; // [esp+1Ch] [ebp-9Ch]
    float v10; // [esp+20h] [ebp-98h]
    float v11; // [esp+24h] [ebp-94h]
    tScriptStruct_GSTCarte3D* v12; // [esp+28h] [ebp-90h]
    tGameObjectMain* v13; // [esp+30h] [ebp-88h]
    float v14; // [esp+34h] [ebp-84h]
    double v15; // [esp+38h] [ebp-80h]
    tGameObjectMain* v16; // [esp+48h] [ebp-70h]
    tGameObjectMain* v17; // [esp+4Ch] [ebp-6Ch]
    tGameObjectMain* v18; // [esp+58h] [ebp-60h]
    tGameObjectMain* v19; // [esp+60h] [ebp-58h]
    tGameObjectMain* v20; // [esp+68h] [ebp-50h]
    tGameObjectMain* v21; // [esp+70h] [ebp-48h]
    tGameObjectMain* v22; // [esp+74h] [ebp-44h]
    tGameObjectMain* v23; // [esp+78h] [ebp-40h]
    float v24; // [esp+84h] [ebp-34h]
    int v25; // [esp+8Ch] [ebp-2Ch]
    int i; // [esp+8Ch] [ebp-2Ch]
    int v27; // [esp+94h] [ebp-24h]
    BOOL v28; // [esp+9Ch] [ebp-1Ch]
    int v29; // [esp+A0h] [ebp-18h]
    int a3; // [esp+A4h] [ebp-14h]
    int a3a; // [esp+A4h] [ebp-14h]
    int a3b; // [esp+A4h] [ebp-14h]
    unsigned int v33; // [esp+A8h] [ebp-10h]
    float v34; // [esp+ACh] [ebp-Ch]
    int a2; // [esp+B4h] [ebp-4h]
    int a2a; // [esp+B4h] [ebp-4h]
    int a2b; // [esp+B4h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v12 = (tScriptStruct_GSTCarte3D*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v12 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        GST_Carte3D_exec_UpdatePiece_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &GST_Carte3D_exec_UpdatePiece_univVars_Flag_speciaux_sauve,
            (void*)GST_Carte3D_exec_UpdatePiece_univVars,
            4488);
    }
    if (v12->i_EtageTransition)
    {
        v12->i_EtageTransition = 0;
        for (a2 = 0; a2 < v12->des_i_NbEtages; ++a2)
        {
            v11 = *(float*)AI_EvalVarArray_C2((char*)&v12->__f_EtageState_Dimension1, a2);
            if (*(float*)AI_EvalVarArray_C2((char*)&v12->__f_EtageStateNext_Dimension1, a2) != v11)
            {
                v12->i_EtageTransition = 1;
                v10 = *(float*)AI_EvalVarArray_C2((char*)&v12->__f_EtageState_Dimension1, a2);
                if (*(float*)AI_EvalVarArray_C2((char*)&v12->__f_EtageStateNext_Dimension1, a2) <= (double)v10)
                {
                    v34 = TIM_gf_dt;
                    v6 = AI_EvalVarArray_C2((char*)&v12->__f_EtageState_Dimension1, a2);
                    *(float*)v6 = *(float*)v6 - v34 * 5.0f;
                    v5 = *(float*)AI_EvalVarArray_C2((char*)&v12->__f_EtageState_Dimension1, a2);
                    if (*(float*)AI_EvalVarArray_C2((char*)&v12->__f_EtageStateNext_Dimension1, a2) > (double)v5)
                    {
                        v4 = *(float*)AI_EvalVarArray_C2((char*)&v12->__f_EtageStateNext_Dimension1, a2);
                        *(float*)AI_EvalVarArray_C2((char*)&v12->__f_EtageState_Dimension1, a2) = v4;
                    }
                }
                else
                {
                    v24 = TIM_gf_dt;
                    v9 = AI_EvalVarArray_C2((char*)&v12->__f_EtageState_Dimension1, a2);
                    *(float*)v9 = v24 * 5.0f + *(float*)v9;
                    v8 = *(float*)AI_EvalVarArray_C2((char*)&v12->__f_EtageState_Dimension1, a2);
                    if (*(float*)AI_EvalVarArray_C2((char*)&v12->__f_EtageStateNext_Dimension1, a2) < (double)v8)
                    {
                        v7 = *(float*)AI_EvalVarArray_C2((char*)&v12->__f_EtageStateNext_Dimension1, a2);
                        *(float*)AI_EvalVarArray_C2((char*)&v12->__f_EtageState_Dimension1, a2) = v7;
                    }
                }
            }
        }
    }
    v3 = (float)v12->i_EtageCur;
    v12->des_f_Z = v12->des_f_EtageEcart * v3;
    for (a2a = 0; a2a < v12->des_i_NbEtages; ++a2a)
    {
        v23 = (tGameObjectMain*)v12->des_ao_Etage[a2a];
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
            if (*(float*)AI_EvalVarArray_C2((char*)&v12->__f_EtageState_Dimension1, a2a) == 0.0)
            {
                v21 = (tGameObjectMain*)v12->des_ao_Etage[a2a];
                if (v21)
                {
                    if ((unsigned int)v21 <= 2)
                        v21 = AI_MainActor[(uint32_t)v21 - 1];
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(v21, 1);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
                }
            }
            else
            {
                v20 = (tGameObjectMain*)v12->des_ao_Etage[a2a];
                if (v20)
                {
                    if ((unsigned int)v20 <= 2)
                        v20 = AI_MainActor[(uint32_t)v20 - 1];
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(v20, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
                }
                v19 = (tGameObjectMain*)v12->des_ao_Etage[a2a];
                if (v19)
                {
                    if ((unsigned int)v19 <= 2)
                        v19 = AI_MainActor[(uint32_t)v19 - 1];
                    AI_EvalFunc_OBJ_RLICarteSet_C(v19, 0xFFFFFFFE, 3);
                }
                else
                {
                    AI_EvalFunc_OBJ_RLICarteSet_C(0, 0xFFFFFFFE, 3);
                }
                v0 = (float*)AI_EvalVarArray_C2((char*)&v12->__f_EtageState_Dimension1, a2a);
                a3 = AI_EvalFunc_COLORBlend_C(0xFF000000, 0, *v0);
                v18 = (tGameObjectMain*)v12->des_ao_Etage[a2a];
                if (v18)
                {
                    if ((unsigned int)v18 <= 2)
                        v18 = AI_MainActor[(uint32_t)v18 - 1];
                    AI_EvalFunc_OBJ_RLICarteSet_C(v18, 0, a3);
                }
                else
                {
                    AI_EvalFunc_OBJ_RLICarteSet_C(0, 0, a3);
                }
            }
        }
    }
    if (v12->des_i_CarteNum + 79 <= 0)
        v28 = 0;
    else
        v28 = AI_EvalFunc_ARRBitTest_C(
            (int*)(GST_Carte3D_exec_UpdatePiece_univVars_Flag_speciaux_sauve + 4),
            v12->des_i_CarteNum + 79);
    for (a2b = 0; a2b < v12->des_i_NbPieces; ++a2b)
    {
        v25 = (unsigned __int8)v12->des_ai_PieceData[a2b];
        if (v25 != 100)
        {
            if (v25 == 255)
            {
                v27 = 0;
                v29 = v12->des_i_NbEtages - 1;
            }
            else
            {
                v27 = (unsigned __int8)v12->des_ai_PieceData[a2b];
                v29 = v27;
            }
            for (i = v27; i <= v29; ++i)
            {
                v33 = v12->des_ao_Etage[i];
                v17 = (tGameObjectMain*)v33;
                if (v33)
                {
                    if (v33 <= 2)
                        v17 = AI_MainActor[v33 - 1];
                    v16 = v17;
                }
                else
                {
                    v16 = 0;
                }
                if (!v16 || *(float*)AI_EvalVarArray_C2((char*)&v12->__f_EtageState_Dimension1, i) == 0.0)
                {
                    v12->des_ai_PieceColor[a2b] = -1;
                }
                else
                {
                    if (v12->des_ai_PieceState[a2b] != 2)
                    {
                        if (a2b + v12->des_i_SFPiece <= 0)
                            v12->des_ai_PieceState[a2b] = 0;
                        else
                            v12->des_ai_PieceState[a2b] = AI_EvalFunc_ARRBitTest_C(
                                (int*)(GST_Carte3D_exec_UpdatePiece_univVars_Flag_speciaux_sauve + 4),
                                a2b + v12->des_i_SFPiece);
                    }
                    if (v12->des_ai_PieceState[a2b] || v28)
                    {
                        a3a = 0x747474;
                        if (v12->des_ai_PieceState[a2b])
                        {
                            v1 = (float*)AI_EvalVarArray_C2((char*)&v12->__f_EtageState_Dimension1, i);
                            a3a = AI_EvalFunc_COLORBlend_C(0x747474, 0, *v1);
                        }
                        v14 = (1.0f - *(float*)AI_EvalVarArray_C2((char*)&v12->__f_EtageState_Dimension1, i)) * 255.0f;
                        v15 = v14 + MATH_gd_magicDoubleForRound;
                        a3b = ((*(uint32_t *)&v15) << 24) | a3a;
                    }
                    else
                    {
                        a3b = -1;
                    }
                    v12->des_ai_PieceColor[a2b] = a3b;
                    v13 = (tGameObjectMain*)v33;
                    if (v33)
                    {
                        if (v33 <= 2)
                            v13 = AI_MainActor[v33 - 1];
                        AI_EvalFunc_OBJ_RLICarteSet_C(v13, a2b + 1, a3b);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_RLICarteSet_C(0, a2b + 1, a3b);
                    }
                }
            }
        }
    }
    return 1000;
}

int GST_Carte3D_Loop()
{
    int result; // eax
    float zoom; // [esp+0h] [ebp-174h]
    float zooma; // [esp+0h] [ebp-174h]
    tScriptStruct_GSTCarte3D* v3; // [esp+4h] [ebp-170h]
    Vec3* v4; // [esp+10h] [ebp-164h]
    Vec3* v5; // [esp+14h] [ebp-160h]
    float v6; // [esp+18h] [ebp-15Ch]
    float v7; // [esp+24h] [ebp-150h]
    float v8; // [esp+48h] [ebp-12Ch]
    tGameObjectMain* v9; // [esp+58h] [ebp-11Ch]
    tGameObjectMain* v10; // [esp+70h] [ebp-104h]
    tGameObjectMain* v11; // [esp+74h] [ebp-100h]
    float v12; // [esp+78h] [ebp-FCh]
    float v13; // [esp+7Ch] [ebp-F8h]
    float v14; // [esp+80h] [ebp-F4h]
    float v15; // [esp+8Ch] [ebp-E8h]
    float v16; // [esp+90h] [ebp-E4h]
    float v17; // [esp+94h] [ebp-E0h]
    Vec3 a3; // [esp+98h] [ebp-DCh] BYREF
    Vec3 v19; // [esp+A4h] [ebp-D0h] BYREF
    float v20; // [esp+B0h] [ebp-C4h]
    float v21; // [esp+B4h] [ebp-C0h]
    float v22; // [esp+B8h] [ebp-BCh]
    float v23; // [esp+BCh] [ebp-B8h]
    int v24; // [esp+C0h] [ebp-B4h]
    float v25; // [esp+C4h] [ebp-B0h]
    Vec3 v26; // [esp+C8h] [ebp-ACh] BYREF
    float v27; // [esp+D4h] [ebp-A0h]
    unsigned int v28; // [esp+D8h] [ebp-9Ch]
    Vec3 v29; // [esp+DCh] [ebp-98h] BYREF
    float v30; // [esp+F4h] [ebp-80h]
    float v31; // [esp+F8h] [ebp-7Ch]
    float v32; // [esp+FCh] [ebp-78h]
    float v33; // [esp+100h] [ebp-74h]
    Vec3 v34; // [esp+104h] [ebp-70h] BYREF
    Vec3 a1; // [esp+110h] [ebp-64h] BYREF
    float v36; // [esp+11Ch] [ebp-58h]
    Vec3 dest; // [esp+120h] [ebp-54h] BYREF
    float v38; // [esp+12Ch] [ebp-48h]
    Vec3 v39; // [esp+130h] [ebp-44h]
    Vec3 v40; // [esp+13Ch] [ebp-38h] BYREF
    int v41; // [esp+148h] [ebp-2Ch]
    tScriptStruct_GSTCarte3D* v42; // [esp+14Ch] [ebp-28h]
    int i; // [esp+150h] [ebp-24h]
    Vec3 v44; // [esp+154h] [ebp-20h] BYREF
    float v45; // [esp+160h] [ebp-14h]
    Vec3 v46; // [esp+164h] [ebp-10h] BYREF
    int v47; // [esp+170h] [ebp-4h]

    v39.x = 1.0;
    v39.y = 0.0;
    v39.z = 0.0;
    v31 = 1.0;
    v32 = 0.0;
    v33 = 0.0;
    v26.x = 1.0;
    v26.y = 0.0;
    v26.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_GSTCarte3D*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v42 = v3;
    v23 = 0.0;
    v27 = 0.0;
    v28 = 0;
    v25 = 0.0;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_GST_Carte3D_Loop_GST_Carte3D_Close = (int)AI_ul_RealResolveOneRefFunction(0x4901E6E7u);
        fct_GST_Carte3D_Loop_GST_Carte3D_exec_UpdatePiece = (int)AI_ul_RealResolveOneRefFunction(0x4901F0ECu);
        GST_Carte3D_Loop_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&GST_Carte3D_Loop_univVars_Carte_i_NbSpot, (void*)GST_Carte3D_Loop_univVars, 4472);
        AI2C_ResolveBuffer(&GST_Carte3D_Loop_univVars_Carte_ao_Object, (void*)GST_Carte3D_Loop_univVars, 4304);
        fct_GST_Carte3D_Loop_GST_Carte3D_exec_Spot = (int)AI_ul_RealResolveOneRefFunction(0x4901F480u);
    }
    if (v42->des_i_Close)
    {
        v24 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Carte3D_Loop_GST_Carte3D_Close);
        if (v24 == 1000)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            result = v24;
        }
    }
    else
    {
        if (v42->i_EtageRequired != v42->i_EtageCur)
        {
            if (v42->i_EtageRequired < 0)
                v42->i_EtageRequired = 0;
            if (v42->i_EtageRequired >= v42->des_i_NbEtages)
                v42->i_EtageRequired = v42->des_i_NbEtages - 1;
            if (v42->i_EtageRequired != v42->i_EtageCur)
            {
                for (i = 0; i < v42->des_i_NbEtages; ++i)
                    *(uint32_t*)AI_EvalVarArray_C2((char*)&v42->__f_EtageStateNext_Dimension1, i) = 0;
                *(uint32_t*)AI_EvalVarArray_C2((char*)&v42->__f_EtageStateNext_Dimension1, v42->i_EtageRequired) = 1065353216;
                if (v42->i_EtageRequired > 0)
                    *(uint32_t*)AI_EvalVarArray_C2((char*)&v42->__f_EtageStateNext_Dimension1, v42->i_EtageRequired - 1) = 1041865114;
                v42->i_EtageCur = v42->i_EtageRequired;
                v42->i_EtageTransition = 1;
            }
        }
        if (!v42->i_EtageTransition
            || (v41 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Carte3D_Loop_GST_Carte3D_exec_UpdatePiece), v41 == 1000))
        {
            v11 = (tGameObjectMain*)v42->des_o_Spot;
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
                if (*(int*)GST_Carte3D_Loop_univVars_Carte_i_NbSpot > 0)
                {
                    if (v42->i_Appear == 1)
                    {
                        if (v42->f_ZoomScale < 1.0)
                        {
                            v38 = TIM_gf_dt;
                            v42->f_ZoomScale = TIM_gf_dt * 4.0f + v42->f_ZoomScale;
                            if (v42->f_ZoomScale > 1.0)
                            {
                                v42->f_ZoomScale = 1.0;
                                v42->i_Appear = 0;
                            }
                            zoom = v42->des_f_Zoom * v42->f_ZoomScale;
                            AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, zoom);
                        }
                        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                        result = 1000;
                    }
                    else
                    {
                        i = INO_b_IoButtonPressed(5);
                        if (v42->i_ZoomWait1)
                            i = 0;
                        if (v42->f_ZoomMinTime > 0.0)
                        {
                            v42->f_ZoomMinTime = v42->f_ZoomMinTime - TIM_gf_dt;
                            i = 1;
                        }
                        v25 = v42->f_ZoomScale;
                        if (i && v42->des_i_CanZoom)
                        {
                            if (v42->f_ZoomScale == 1.0f)
                                v42->f_ZoomMinTime = 0.1f;
                            if (v42->f_ZoomScale != 3.0f)
                            {
                                v30 = TIM_gf_dt;
                                v42->f_ZoomScale = 10.0f * TIM_gf_dt + v42->f_ZoomScale;
                                if (v42->f_ZoomScale > 3.0f)
                                    v42->f_ZoomScale = 3.0f;
                            }
                        }
                        else if (v42->f_ZoomScale == 1.0f)
                        {
                            v42->i_ZoomWait1 = 0;
                        }
                        else
                        {
                            v45 = TIM_gf_dt;
                            v42->f_ZoomScale = v42->f_ZoomScale - 10.0f * TIM_gf_dt;
                            if (v42->f_ZoomScale < 1.0f)
                                v42->f_ZoomScale = 1.0f;
                            v42->i_ZoomWait1 = 1;
                        }
                        if (v42->f_ZoomScale != v25)
                        {
                            zooma = v42->des_f_Zoom * v42->f_ZoomScale;
                            AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, zooma);
                            if (v42->i_FixedSpot)
                            {
                                v26 = v42->v_Offset;
                            }
                            else
                            {
                                v28 = *(uint32_t*)(GST_Carte3D_Loop_univVars_Carte_ao_Object + 4);
                                v9 = (tGameObjectMain*)v28;
                                if (v28)
                                {
                                    if (v28 <= 2)
                                        v9 = AI_MainActor[v28 - 1];
                                    AI_EvalFunc_OBJGetPos_C(v9, &a3);
                                }
                                else
                                {
                                    AI_EvalFunc_OBJGetPos_C(0, &a3);
                                }
                                MATH_SubstractVec(&dest, &a3, &v42->v_Offset);
                                v26 = dest;
                                if (v42->i_Rotation == 1)
                                {
                                    v23 = v26.x;
                                    v26.x = v26.y;
                                    v26.y = -v23;
                                }
                                else if (v42->i_Rotation == 2)
                                {
                                    v23 = v26.x;
                                    v26.x = -v26.y;
                                    v26.y = v23;
                                }
                            }
                            v8 = v42->des_f_Zoom;
                            v26.x = v26.x * v8;
                            v26.y = v26.y * v8;
                            v26.z = 0.0;
                            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v44);
                            v39 = v44;
                            v20 = v44.x * v26.x;
                            v21 = v44.y * v26.x;
                            v22 = v44.z * v26.x;
                            v31 = v20;
                            v32 = v21;
                            v33 = v22;
                            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a1);
                            v39 = a1;
                            v15 = a1.x * v26.y;
                            v16 = a1.y * v26.y;
                            v17 = a1.z * v26.y;
                            v31 = v31 - v15;
                            v32 = v32 - v16;
                            v33 = v33 - v17;
                            v7 = v42->f_ZoomScale - 1.0f;
                            v46.x = v31 * v7;
                            v46.y = v32 * v7;
                            v46.z = v33 * v7;
                            MATH_SubstractVec(&v34, &v42->v_SavePos, &v46);
                            AI_EvalFunc_OBJSetPos_C_CURRENT(&v34);
                            if (v42->f_ZoomScale >= (double)v25)
                            {
                                AI_EvalFunc_OBJGetPos_C_CURRENT(&v42->v_ZoomPos);
                                v4 = &v42->v_ZoomPosInit;
                                v5 = &v42->v_ZoomPos;
                                v42->v_ZoomPosInit.x = v42->v_ZoomPos.x;
                                v4->y = v5->y;
                                v4->z = v5->z;
                            }
                            else
                            {
                                AI_EvalFunc_OBJGetPos_C_CURRENT(&v19);
                                v26 = v19;
                                MATH_SubstractVec(&v29, &v42->v_ZoomPos, &v42->v_ZoomPosInit);
                                v6 = (v42->f_ZoomScale - 1.0f) / 2.0f;
                                v12 = v29.x * v6;
                                v13 = v29.y * v6;
                                v14 = v29.z * v6;
                                v26.x = v26.x + v12;
                                v26.y = v26.y + v13;
                                v26.z = v26.z + v14;
                                AI_EvalFunc_OBJSetPos_C_CURRENT(&v26);
                            }
                        }
                        if (v42->des_i_CanPane && v42->f_ZoomScale == 3.0f)
                        {
                            AI_EvalFunc_IoJoyGetMove_C(&v40);
                            v26 = v40;
                            v36 = TIM_gf_dt;
                            v27 = 20.0f * TIM_gf_dt;
                            if (v40.x <= 0.5f)
                            {
                                if (v26.x < -0.5)
                                    v42->v_ZoomPos.x = (-0.5f - v26.x) * v27 + v42->v_ZoomPos.x;
                            }
                            else
                            {
                                v42->v_ZoomPos.x = v42->v_ZoomPos.x - (v26.x - 0.5f) * v27;
                            }
                            if (v26.y <= 0.5f)
                            {
                                if (v26.y < -0.5)
                                    v42->v_ZoomPos.z = (-0.5f - v26.y) * v27 + v42->v_ZoomPos.z;
                            }
                            else
                            {
                                v42->v_ZoomPos.z = v42->v_ZoomPos.z - (v26.y - 0.5f) * v27;
                            }
                            if (v42->f_ZoomMinZ > (double)v42->v_ZoomPos.z)
                                v42->v_ZoomPos.z = v42->f_ZoomMinZ;
                            if (v42->f_ZoomMaxZ < (double)v42->v_ZoomPos.z)
                                v42->v_ZoomPos.z = v42->f_ZoomMaxZ;
                            if (v42->f_ZoomMinX > (double)v42->v_ZoomPos.x)
                                v42->v_ZoomPos.x = v42->f_ZoomMinX;
                            if (v42->f_ZoomMaxX < (double)v42->v_ZoomPos.x)
                                v42->v_ZoomPos.x = v42->f_ZoomMaxX;
                            AI_EvalFunc_OBJSetPos_C_CURRENT(&v42->v_ZoomPos);
                        }
                        v47 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Carte3D_Loop_GST_Carte3D_exec_Spot);
                        if (v47 == 1000)
                            result = 1000;
                        else
                            result = v47;
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
        }
        else
        {
            result = v41;
        }
    }
    return result;
}

int GST_Carte3D_exec_Spot()
{
    char* v0; // eax
    char* v2; // eax
    char* v3; // eax
    char* v4; // eax
    char* v5; // eax
    float* v6; // eax
    char* v7; // [esp-4h] [ebp-318h]
    char* v8; // [esp-4h] [ebp-318h]
    char* v9; // [esp-4h] [ebp-318h]
    char* v10; // [esp-4h] [ebp-318h]
    Vec3* v11; // [esp-4h] [ebp-318h]
    float v12; // [esp+4h] [ebp-310h]
    float v13; // [esp+8h] [ebp-30Ch]
    tScriptStruct_GSTCarte3D* v14; // [esp+Ch] [ebp-308h]
    tGameObjectMain* v15; // [esp+14h] [ebp-300h]
    tGameObjectMain* v16; // [esp+1Ch] [ebp-2F8h]
    tGameObjectMain* v17; // [esp+20h] [ebp-2F4h]
    tGameObjectMain* v18; // [esp+24h] [ebp-2F0h]
    tGameObjectMain* v19; // [esp+30h] [ebp-2E4h]
    Vec3* v20; // [esp+34h] [ebp-2E0h]
    Vec3* v21; // [esp+38h] [ebp-2DCh]
    Vec3* v22; // [esp+3Ch] [ebp-2D8h]
    char* v23; // [esp+44h] [ebp-2D0h]
    Vec3* v24; // [esp+48h] [ebp-2CCh]
    tGameObjectMain* v25; // [esp+54h] [ebp-2C0h]
    tGameObjectMain* v26; // [esp+60h] [ebp-2B4h]
    tGameObjectMain* v27; // [esp+64h] [ebp-2B0h]
    char* v28; // [esp+98h] [ebp-27Ch]
    Vec3* v29; // [esp+9Ch] [ebp-278h]
    char* v30; // [esp+A4h] [ebp-270h]
    Vec3* v31; // [esp+A8h] [ebp-26Ch]
    Vec3* v32; // [esp+11Ch] [ebp-1F8h]
    tGameObjectMain* v33; // [esp+124h] [ebp-1F0h]
    tGameObjectMain* v34; // [esp+12Ch] [ebp-1E8h]
    tGameObjectMain* v35; // [esp+138h] [ebp-1DCh]
    tGameObjectMain* v36; // [esp+13Ch] [ebp-1D8h]
    tGameObjectMain* v37; // [esp+144h] [ebp-1D0h]
    tGameObjectMain* v38; // [esp+148h] [ebp-1CCh]
    tGameObjectMain* v39; // [esp+14Ch] [ebp-1C8h]
    Vec3* v40; // [esp+150h] [ebp-1C4h]
    Vec3* v41; // [esp+154h] [ebp-1C0h]
    tGameObjectMain* v42; // [esp+164h] [ebp-1B0h]
    tGameObjectMain* v43; // [esp+168h] [ebp-1ACh]
    Vec3 v44; // [esp+178h] [ebp-19Ch] BYREF
    Vec3 a3; // [esp+184h] [ebp-190h] BYREF
    float v46; // [esp+198h] [ebp-17Ch]
    float v47; // [esp+19Ch] [ebp-178h]
    float v48; // [esp+1A0h] [ebp-174h]
    Vec3 v49; // [esp+1A4h] [ebp-170h] BYREF
    int v50; // [esp+1B0h] [ebp-164h]
    int v51; // [esp+1B4h] [ebp-160h]
    int v52[4]; // [esp+1B8h] [ebp-15Ch]
    Vec3 a5; // [esp+1C8h] [ebp-14Ch] BYREF
    Vec3 v54; // [esp+1D4h] [ebp-140h] BYREF
    float v56; // [esp+1FCh] [ebp-118h]
    float a4; // [esp+200h] [ebp-114h]
    int v58; // [esp+204h] [ebp-110h]
    Vec3 v59; // [esp+220h] [ebp-F4h] BYREF
    Vec3 dest; // [esp+244h] [ebp-D0h] BYREF
    Vec3 v61; // [esp+25Ch] [ebp-B8h] BYREF
    int v62; // [esp+268h] [ebp-ACh]
    int i; // [esp+26Ch] [ebp-A8h]
    int l; // [esp+270h] [ebp-A4h]
    int v65; // [esp+274h] [ebp-A0h]
    int v66; // [esp+27Ch] [ebp-98h]
    tGameObjectMain* v67; // [esp+284h] [ebp-90h]
    tGameObjectMain* v68; // [esp+288h] [ebp-8Ch]
    int v69; // [esp+28Ch] [ebp-88h]
    Vec3 v70; // [esp+290h] [ebp-84h] BYREF
    int v71; // [esp+2A0h] [ebp-74h]
    int j; // [esp+2A4h] [ebp-70h]
    BOOL v73; // [esp+2A8h] [ebp-6Ch]
    BOOL v74; // [esp+2B0h] [ebp-64h]
    int v75; // [esp+2B4h] [ebp-60h]
    int v76; // [esp+2B8h] [ebp-5Ch]
    Vec3 v77; // [esp+2BCh] [ebp-58h] BYREF
    tScriptStruct_GSTCarte3D* v78; // [esp+2D4h] [ebp-40h]
    BOOL v79; // [esp+2D8h] [ebp-3Ch]
    unsigned int v80; // [esp+2DCh] [ebp-38h]
    int k; // [esp+2E0h] [ebp-34h]
    Vec3 v82; // [esp+2E4h] [ebp-30h] BYREF
    BOOL v83; // [esp+2FCh] [ebp-18h]
    int v84; // [esp+300h] [ebp-14h]
    int v85; // [esp+304h] [ebp-10h]
    int v86; // [esp+308h] [ebp-Ch]
    BOOL v87; // [esp+310h] [ebp-4h]

    v70.x = 1.0;
    v70.y = 0.0;
    v70.z = 0.0;
    v54.x = 1.0;
    v54.y = 0.0;
    v54.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v14 = (tScriptStruct_GSTCarte3D*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v14 = 0;
    v78 = v14;
    j = 0;
    v73 = 0;
    v56 = 0.0;
    v62 = 0;
    i = 0;
    l = 0;
    v74 = 0;
    v65 = 0;
    v66 = 0;
    v80 = 0;
    v75 = 0;
    a4 = 0.0;
    k = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        GST_Carte3D_exec_Spot_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        GST_Carte3D_exec_Spot_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&GST_Carte3D_exec_Spot_univVars_Perso_ID_actor, (void*)GST_Carte3D_exec_Spot_univVars, 988);
        AI2C_ResolveBuffer(
            &GST_Carte3D_exec_Spot_univVars_Perso_ID_passengers_list,
            (void*)GST_Carte3D_exec_Spot_univVars,
            936);
        AI2C_ResolveBuffer(
            &GST_Carte3D_exec_Spot_univVars_Perso_ID_CarteEtage,
            (void*)GST_Carte3D_exec_Spot_univVars,
            1044);
        AI2C_ResolveBuffer(
            &GST_Carte3D_exec_Spot_univVars_Flag_speciaux_sauve,
            (void*)GST_Carte3D_exec_Spot_univVars,
            4488);
        AI2C_ResolveBuffer(&GST_Carte3D_exec_Spot_univVars_Perso_ID_world_ID, (void*)GST_Carte3D_exec_Spot_univVars, 536);
        AI2C_ResolveBuffer(&GST_Carte3D_exec_Spot_univVars_Perso_ID_pos, (void*)GST_Carte3D_exec_Spot_univVars, 588);
        fct_GST_Carte3D_exec_Spot_GST_Carte3D_exec_SpotInsert = (int)AI_ul_RealResolveOneRefFunction(0x4901F0EEu);
        AI2C_ResolveBuffer(
            &GST_Carte3D_exec_Spot_INV_manager_gao_i_MailCur,
            (void*)GST_Carte3D_exec_Spot_INV_manager_gao,
            22872);
        AI2C_ResolveBuffer(
            &GST_Carte3D_exec_Spot_INV_manager_gao_ai_Note,
            (void*)GST_Carte3D_exec_Spot_INV_manager_gao,
            22896);
        AI2C_ResolveBuffer(&GST_Carte3D_exec_Spot_Carte_ao_Object, (void*)GST_Carte3D_exec_Spot_univVars, 4304);
    }
    a5.x = 0.0;
    a5.y = 0.0;
    a5.z = 0.0;
    STR_SetStringExt(
        (tGameObjectMain*)GST_Carte3D_exec_Spot_INV_manager_gao,
        20,
        1507840,
        (char*)AI_gpst_CurrentFunction->localStringConstsData,
        &a5);
    v78->i_SpotIndex = 0;
    v78->i_SpotOnMap = 0;
    v78->i_SpotOnFloor = 0;
    v52[0] = 8;
    v52[1] = 10;
    v52[2] = 9;
    v77.x = -10.0;
    v77.y = -10.0;
    v77.z = 0.0;
    AI_EvalFunc_OBJ_TextPivotSet_C((tGameObjectMain*)GST_Carte3D_exec_Spot_INV_manager_gao, 19, &v77);
    for (i = 0; i < 3; ++i)
    {
        k = v52[i];
        v73 = 0;
        if (v78->i_FixedSpot)
        {
            v67 = *AI_MainActor;
            v43 = v67;
            if (v67)
            {
                if ((unsigned int)v67 <= 2)
                    v43 = AI_MainActor[(uint32_t)v67 - 1];
                v42 = v43;
            }
            else
            {
                v42 = 0;
            }
            if (*(tGameObjectMain**)(GST_Carte3D_exec_Spot_univVars_Perso_ID_actor + 4 * k + 4) == v42)
                v73 = 1;
        }
        else if (((1 << k) & *(uint32_t*)(GST_Carte3D_exec_Spot_univVars_Perso_ID_passengers_list + 44)) == 0
            && ((1 << k) & *(uint32_t*)(GST_Carte3D_exec_Spot_univVars_Perso_ID_passengers_list + 40)) == 0)
        {
            if (*(uint32_t*)(GST_Carte3D_exec_Spot_univVars_Perso_ID_CarteEtage + 4 * k + 4) == v78->i_EtageCur)
            {
                if (k == 8)
                {
                    v73 = 1;
                }
                else if (k + 1500 <= 0)
                {
                    v73 = 0;
                }
                else
                {
                    v73 = AI_EvalFunc_ARRBitTest_C((int*)(GST_Carte3D_exec_Spot_univVars_Flag_speciaux_sauve + 4), k + 1500);
                }
            }
            else
            {
                v73 = 0;
            }
        }
        if (v73)
        {
            if (v78->i_FixedSpot)
            {
                v40 = &v78->v_exec_3DPos;
                v41 = &v78->v_Offset;
                v78->v_exec_3DPos.x = v78->v_Offset.x;
                v40->y = v41->y;
                v40->z = v41->z;
                v78->i_exec_Rotate = 0;
            }
            else
            {
                v39 = *(tGameObjectMain**)(GST_Carte3D_exec_Spot_univVars_Perso_ID_actor + 4 * k + 4);
                if (v39)
                {
                    if ((unsigned int)v39 <= 2)
                        v39 = AI_MainActor[(uint32_t)v39 - 1];
                    v38 = v39;
                }
                else
                {
                    v38 = 0;
                }
                if (v38)
                {
                    v80 = *(uint32_t*)(GST_Carte3D_exec_Spot_univVars_Perso_ID_actor + 4 * k + 4);
                    v37 = (tGameObjectMain*)v80;
                    if (v80)
                    {
                        if (v80 <= 2)
                            v37 = AI_MainActor[v80 - 1];
                        AI_EvalFunc_OBJGetPos_C(v37, &a3);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &a3);
                    }
                    MATH_SubstractVec(&v78->v_exec_3DPos, &a3, &v78->v_Offset);
                    v78->i_exec_Rotate = v78->i_Rotation;
                }
                else
                {
                    for (j = 0;
                        j < v78->des_i_MapNb
                        && *(uint32_t*)(GST_Carte3D_exec_Spot_univVars_Perso_ID_world_ID + 4 * k + 4) != v78->des_ai_MapIndex[j];
                        ++j)
                    {
                        ;
                    }
                    if (j == v78->des_i_MapNb)
                    {
                        v73 = 0;
                    }
                    else
                    {
                        v7 = AI_EvalVarArray_C3((char*)&v78->__des_av_MapOffset_Dimension1, j);
                        v0 = AI_EvalVarArray_C3((char*)GST_Carte3D_exec_Spot_univVars_Perso_ID_pos, k);
                        MATH_SubstractVec(&v78->v_exec_3DPos, (Vec3*)v0, (Vec3*)v7);
                        v78->i_exec_Rotate = v78->des_ai_MapRotation[j];
                    }
                }
            }
            if (v73)
            {
                v78->i_exec_Icone = v78->ai_CompState[k] >> 16;
                v78->i_exec_Resize = 1;
                v78->i_exec_State = (k << 16) | (unsigned __int16)v78->ai_CompState[k];
                v50 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Carte3D_exec_Spot_GST_Carte3D_exec_SpotInsert);
                if (v50 != 1000)
                    return v50;
                v78->ai_CompState[k] = (v78->i_exec_Icone << 16) | (unsigned __int16)v78->i_exec_State;
                v68 = *AI_MainActor;
                v36 = v68;
                if (v68)
                {
                    if ((unsigned int)v68 <= 2)
                        v36 = AI_MainActor[(uint32_t)v68 - 1];
                    v35 = v36;
                }
                else
                {
                    v35 = 0;
                }
                if (*(tGameObjectMain**)(GST_Carte3D_exec_Spot_univVars_Perso_ID_actor + 4 * k + 4) == v35)
                {
                    v80 = *(uint32_t*)(GST_Carte3D_exec_Spot_univVars_Perso_ID_actor + 4 * k + 4);
                    v54 = v78->v_exec_2DPos;
                    AI_EvalFunc_OBJ_TextPivotSet_C((tGameObjectMain*)GST_Carte3D_exec_Spot_INV_manager_gao, 19, &v54);
                    AI_EvalFunc_OBJ_TextColorSet_C(
                        (tGameObjectMain*)GST_Carte3D_exec_Spot_INV_manager_gao,
                        19,
                        0,
                        1,
                        v78->c_exec_Color);
                    v34 = (tGameObjectMain*)v80;
                    if (v80)
                    {
                        if (v80 <= 2)
                            v34 = AI_MainActor[v80 - 1];
                        AI_EvalFunc_OBJGetPos_C(v34, &v82);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v82);
                    }
                    MATH_SubstractVec(&v78->v_exec_3DPos, &v82, &v78->v_Offset);
                    v33 = (tGameObjectMain*)v80;
                    if (v80)
                    {
                        if (v80 <= 2)
                            v33 = AI_MainActor[v80 - 1];
                        AI_EvalFunc_OBJ_SightGet_C(v33, &v61);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_SightGet_C(0, &v61);
                    }
                    v46 = v61.x * 1000.0f;
                    v47 = v61.y * 1000.0f;
                    v48 = v61.z * 1000.0f;
                    v32 = &v78->v_exec_3DPos;
                    v78->v_exec_3DPos.x = v46 + v78->v_exec_3DPos.x;
                    v32->y = v47 + v32->y;
                    v32->z = v48 + v32->z;
                    v78->i_exec_Icone = -1;
                    v86 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Carte3D_exec_Spot_GST_Carte3D_exec_SpotInsert);
                    if (v86 != 1000)
                        return v86;
                    MATH_SubstractVec(&dest, &v54, &v78->v_exec_2DPos);
                    v70 = dest;
                    if (!AI2C_VecEqNull(&v70))
                    {
                        v59.x = 0.0;
                        v59.y = 1.0;
                        v59.z = 0.0;
                        v44.x = 0.0;
                        v44.y = 0.0;
                        v44.z = 1.0;
                        a4 = MATH_f_VecAngle(&v70, &v59, &v44);
                        STR_EffectSetf((tGameObjectMain*)GST_Carte3D_exec_Spot_INV_manager_gao, v78->i_EffectRot, 1550, a4);
                    }
                }
            }
        }
    }
    if (v78->i_SpotType)
    {
        switch (v78->i_SpotType)
        {
        case 3:
            for (k = 0; k < v78->des_i_NbConsole; ++k)
            {
                ++v78->i_SpotOnMap;
                v12 = (float)v78->i_EtageCur;
                if (*((float*)AI_EvalVarArray_C3((char*)&v78->__des_av_ConsolePos_Dimension1, k) + 2) == v12)
                {
                    ++v78->i_SpotOnFloor;
                    v78->i_exec_Icone = 3684;
                    v78->i_exec_Resize = 2;
                    v78->i_exec_State = 0;
                    v28 = AI_EvalVarArray_C3((char*)&v78->__des_av_ConsolePos_Dimension1, k);
                    v29 = &v78->v_exec_3DPos;
                    v78->v_exec_3DPos.x = *(float*)v28;
                    v29->y = *((float*)v28 + 1);
                    v29->z = *((float*)v28 + 2);
                    v78->i_exec_Rotate = 0;
                    v58 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Carte3D_exec_Spot_GST_Carte3D_exec_SpotInsert);
                    if (v58 != 1000)
                        return v58;
                }
            }
            break;
        case 1:
            if (v78->des_i_PearlNbGroup)
            {
                for (k = 0; k < v78->des_i_PearlNbGroup; ++k)
                {
                    v78->i_SpotOnMap += v78->des_i_PearlGroupNb[k];
                    v65 = v78->des_i_PearlGroupFirst[k];
                    if (v78->des_i_PearlGroupNb[k] && v78->i_EtageCur == v78->des_ai_PearlEtage[v65])
                    {
                        v78->i_SpotOnFloor += v78->des_i_PearlGroupNb[k];
                        v78->i_exec_Icone = v78->des_i_PearlGroupNb[k] == 1 ? 3685 : 3686;
                        v78->i_exec_Resize = 2;
                        v78->i_exec_State = 0;
                        v8 = AI_EvalVarArray_C3((char*)&v78->__des_av_MapOffset_Dimension1, v78->des_ai_PearlMap[v65]);
                        v2 = AI_EvalVarArray_C3((char*)&v78->__des_av_PearlPos_Dimension1, v65);
                        MATH_SubstractVec(&v78->v_exec_3DPos, (Vec3*)v2, (Vec3*)v8);
                        v78->i_exec_Rotate = v78->des_ai_MapRotation[v78->des_ai_PearlMap[v65]];
                        v71 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Carte3D_exec_Spot_GST_Carte3D_exec_SpotInsert);
                        if (v71 != 1000)
                            return v71;
                    }
                }
            }
            else
            {
                for (k = 0; k < v78->des_i_NbPearls; ++k)
                {
                    if (v78->des_ai_Pearl[k] + 2900 <= 0)
                        v74 = 0;
                    else
                        v74 = AI_EvalFunc_ARRBitTest_C(
                            (int*)(GST_Carte3D_exec_Spot_univVars_Flag_speciaux_sauve + 4),
                            v78->des_ai_Pearl[k] + 2900);
                    v75 = 1;
                    v62 = 1;
                    if (v78->des_ai_PearlSF[k])
                    {
                        if (v78->des_ai_PearlSF[k] <= 0)
                        {
                            v75 = 0;
                        }
                        else if (v78->des_ai_PearlSF[k] < 10000)
                        {
                            v75 = AI_EvalFunc_ARRBitTest_C(
                                (int*)(GST_Carte3D_exec_Spot_univVars_Flag_speciaux_sauve + 4),
                                v78->des_ai_PearlSF[k]);
                        }
                        else
                        {
                            v83 = AI_EvalFunc_ARRBitTest_C(
                                (int*)(GST_Carte3D_exec_Spot_univVars_Flag_speciaux_sauve + 4),
                                v78->des_ai_PearlSF[k] - 10000);
                            v75 = 1 - v83;
                        }
                    }
                    if (v78->des_ai_PearlSF2[k])
                    {
                        if (v78->des_ai_PearlSF2[k] <= 0)
                        {
                            v62 = 0;
                        }
                        else if (v78->des_ai_PearlSF2[k] < 10000)
                        {
                            v62 = AI_EvalFunc_ARRBitTest_C(
                                (int*)(GST_Carte3D_exec_Spot_univVars_Flag_speciaux_sauve + 4),
                                v78->des_ai_PearlSF2[k]);
                        }
                        else
                        {
                            v79 = AI_EvalFunc_ARRBitTest_C(
                                (int*)(GST_Carte3D_exec_Spot_univVars_Flag_speciaux_sauve + 4),
                                v78->des_ai_PearlSF2[k] - 10000);
                            v62 = 1 - v79;
                        }
                    }
                    if (!v74)
                    {
                        if (v75)
                        {
                            if (v62)
                            {
                                ++v78->i_SpotOnMap;
                                if (v78->i_EtageCur == v78->des_ai_PearlEtage[k])
                                {
                                    ++v78->i_SpotOnFloor;
                                    v78->i_exec_Icone = 3685;
                                    v78->i_exec_Resize = 2;
                                    v78->i_exec_State = 0;
                                    v9 = AI_EvalVarArray_C3((char*)&v78->__des_av_MapOffset_Dimension1, v78->des_ai_PearlMap[k]);
                                    v3 = AI_EvalVarArray_C3((char*)&v78->__des_av_PearlPos_Dimension1, k);
                                    MATH_SubstractVec(&v78->v_exec_3DPos, (Vec3*)v3, (Vec3*)v9);
                                    v78->i_exec_Rotate = v78->des_ai_MapRotation[v78->des_ai_PearlMap[k]];
                                    v85 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Carte3D_exec_Spot_GST_Carte3D_exec_SpotInsert);
                                    if (v85 != 1000)
                                        return v85;
                                }
                            }
                        }
                    }
                }
            }
            break;
        case 2:
            if (v78->i_Edit_ShowAnimalsStatic)
            {
                for (k = 0; k < v78->des_i_NbAnimals; ++k)
                {
                    if ((v78->des_ai_AnimalState[k] & 5) == 0 || v78->des_ai_AnimalMap[k] != v78->i_CurMap)
                    {
                        v74 = v78->des_ai_Animal[k] + 800 > 0
                            && AI_EvalFunc_ARRBitTest_C(
                                (int*)(GST_Carte3D_exec_Spot_univVars_Flag_speciaux_sauve + 4),
                                v78->des_ai_Animal[k] + 800);
                        if (v78->des_ai_AnimalSF[k])
                        {
                            if (v78->des_ai_AnimalSF[k] <= 0)
                            {
                                v75 = 0;
                            }
                            else if (v78->des_ai_AnimalSF[k] < 10000)
                            {
                                v75 = AI_EvalFunc_ARRBitTest_C(
                                    (int*)(GST_Carte3D_exec_Spot_univVars_Flag_speciaux_sauve + 4),
                                    v78->des_ai_AnimalSF[k]);
                            }
                            else
                            {
                                v87 = AI_EvalFunc_ARRBitTest_C(
                                    (int*)(GST_Carte3D_exec_Spot_univVars_Flag_speciaux_sauve + 4),
                                    v78->des_ai_AnimalSF[k] - 10000);
                                v75 = 1 - v87;
                            }
                        }
                        else
                        {
                            v75 = 0;
                        }
                        if (!v74 && !v75)
                        {
                            ++v78->i_SpotOnMap;
                            if (v78->i_EtageCur == v78->des_ai_AnimalEtage[k])
                            {
                                ++v78->i_SpotOnFloor;
                                v78->i_exec_Icone = 3682;
                                v78->i_exec_Resize = 2;
                                v78->i_exec_State = 4;
                                v10 = AI_EvalVarArray_C3((char*)&v78->__des_av_MapOffset_Dimension1, v78->des_ai_AnimalMap[k]);
                                v4 = AI_EvalVarArray_C3((char*)&v78->__des_av_AnimalPos_Dimension1, k);
                                MATH_SubstractVec(&v78->v_exec_3DPos, (Vec3*)v4, (Vec3*)v10);
                                v78->i_exec_Rotate = v78->des_ai_MapRotation[v78->des_ai_AnimalMap[k]];
                                v51 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Carte3D_exec_Spot_GST_Carte3D_exec_SpotInsert);
                                if (v51 != 1000)
                                    return v51;
                            }
                        }
                    }
                }
            }
            if (v78->i_Edit_ShowAnimalsDyn && !v78->i_FixedSpot)
            {
                for (k = 0; k < v78->i_AnimalDynNb; ++k)
                {
                    ++v78->i_SpotOnMap;
                    v66 = v78->ai_AnimalDynData[k] >> 8;
                    if (v78->i_EtageCur == v66)
                    {
                        ++v78->i_SpotOnFloor;
                        v78->i_exec_Icone = 3682;
                        v78->i_exec_Resize = 2;
                        v78->i_exec_State = 4;
                        v11 = &v78->v_Offset;
                        v5 = AI_EvalVarArray_C3((char*)&v78->__av_AnimalDynPos_Dimension1, k);
                        MATH_SubstractVec(&v78->v_exec_3DPos, (Vec3*)v5, v11);
                        v78->i_exec_Rotate = v78->i_Rotation;
                        v84 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Carte3D_exec_Spot_GST_Carte3D_exec_SpotInsert);
                        if (v84 != 1000)
                            return v84;
                    }
                }
            }
            v74 = AI_EvalFunc_ARRBitTest_C((int*)(GST_Carte3D_exec_Spot_univVars_Flag_speciaux_sauve + 4), 849);
            if (!v74 && *(uint32_t*)(GST_Carte3D_exec_Spot_univVars_Perso_ID_CarteEtage + 4) == v78->i_EtageCur)
            {
                for (l = 0; l < v78->des_i_MapNb; ++l)
                {
                    if (v78->des_ai_MapIndex[l] == *(uint32_t*)(GST_Carte3D_exec_Spot_univVars_Perso_ID_world_ID + 4))
                    {
                        v73 = 1;
                        if (l == v78->i_CurMap)
                        {
                            v80 = *(uint32_t*)(GST_Carte3D_exec_Spot_univVars_Perso_ID_actor + 4);
                            v27 = (tGameObjectMain*)v80;
                            if (v80)
                            {
                                if (v80 <= 2)
                                    v27 = AI_MainActor[v80 - 1];
                                v26 = v27;
                            }
                            else
                            {
                                v26 = 0;
                            }
                            if (v26)
                            {
                                v25 = (tGameObjectMain*)v80;
                                if (v80)
                                {
                                    if (v80 <= 2)
                                        v25 = AI_MainActor[v80 - 1];
                                    AI_EvalFunc_OBJGetPos_C(v25, &v78->v_exec_3DPos);
                                }
                                else
                                {
                                    AI_EvalFunc_OBJGetPos_C(0, &v78->v_exec_3DPos);
                                }
                            }
                            else
                            {
                                v73 = 0;
                            }
                        }
                        else
                        {
                            v23 = AI_EvalVarArray_C3((char*)GST_Carte3D_exec_Spot_univVars_Perso_ID_pos, 0);
                            v24 = &v78->v_exec_3DPos;
                            v78->v_exec_3DPos.x = *(float*)v23;
                            v24->y = *((float*)v23 + 1);
                            v24->z = *((float*)v23 + 2);
                        }
                        if (v73)
                        {
                            ++v78->i_SpotOnMap;
                            ++v78->i_SpotOnFloor;
                            v78->i_exec_Icone = 3682;
                            v78->i_exec_Resize = 2;
                            v78->i_exec_State = 4;
                            v6 = (float*)AI_EvalVarArray_C3((char*)&v78->__des_av_MapOffset_Dimension1, l);
                            v22 = &v78->v_exec_3DPos;
                            v78->v_exec_3DPos.x = v78->v_exec_3DPos.x - *v6;
                            v22->y = v22->y - v6[1];
                            v22->z = v22->z - v6[2];
                            v78->i_exec_Rotate = v78->des_ai_MapRotation[l];
                            v76 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Carte3D_exec_Spot_GST_Carte3D_exec_SpotInsert);
                            if (v76 != 1000)
                                return v76;
                        }
                    }
                }
            }
            break;
        }
    }
    else
    {
        for (k = 0; k < v78->des_i_NbNotes; ++k)
        {
            if (v78->des_ai_Note[k] + 1700 <= 0)
                v74 = 0;
            else
                v74 = AI_EvalFunc_ARRBitTest_C(
                    (int*)(GST_Carte3D_exec_Spot_univVars_Flag_speciaux_sauve + 4),
                    v78->des_ai_Note[k] + 1700);
            if (v78->des_ai_Note[k] + 1750 <= 0)
                v75 = 0;
            else
                v75 = AI_EvalFunc_ARRBitTest_C(
                    (int*)(GST_Carte3D_exec_Spot_univVars_Flag_speciaux_sauve + 4),
                    v78->des_ai_Note[k] + 1750);
            if (v78->des_ai_NoteSF[k])
            {
                if (v78->des_ai_NoteSF[k] <= 0)
                    v62 = 0;
                else
                    v62 = AI_EvalFunc_ARRBitTest_C(
                        (int*)(GST_Carte3D_exec_Spot_univVars_Flag_speciaux_sauve + 4),
                        v78->des_ai_NoteSF[k]);
            }
            else
            {
                v62 = 0;
            }
            if (v74)
            {
                if (!v75
                    && !v62
                    && v78->des_ai_Note[k] == *(uint32_t*)(GST_Carte3D_exec_Spot_INV_manager_gao_ai_Note
                        + 4 * *(uint32_t*)GST_Carte3D_exec_Spot_INV_manager_gao_i_MailCur
                        + 4))
                {
                    v13 = (float)v78->i_EtageCur;
                    if (*((float*)AI_EvalVarArray_C3((char*)&v78->__des_av_NotePos_Dimension1, k) + 2) == v13)
                    {
                        v78->i_exec_Icone = 3681;
                        v78->i_exec_Resize = 0;
                        v78->i_exec_State = 0;
                        v30 = AI_EvalVarArray_C3((char*)&v78->__des_av_NotePos_Dimension1, k);
                        v31 = &v78->v_exec_3DPos;
                        v78->v_exec_3DPos.x = *(float*)v30;
                        v31->y = *((float*)v30 + 1);
                        v31->z = *((float*)v30 + 2);
                        v78->i_exec_Rotate = 0;
                        v69 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_GST_Carte3D_exec_Spot_GST_Carte3D_exec_SpotInsert);
                        if (v69 != 1000)
                            return v69;
                    }
                }
            }
        }
    }
    STR_SetLetterFlag((tGameObjectMain*)GST_Carte3D_exec_Spot_INV_manager_gao, 20, 0, -1, 3);
    if (v78->i_FixedSpot)
    {
        v20 = &v78->v_exec_3DPos;
        v21 = &v78->v_Offset;
        v78->v_exec_3DPos.x = v78->v_Offset.x;
        v20->y = v21->y;
        v20->z = v21->z;
        v78->v_exec_3DPos.z = v78->des_f_Z;
        v19 = (tGameObjectMain*)v78->des_o_Spot;
        if (v19)
        {
            if ((unsigned int)v19 <= 2)
                v19 = AI_MainActor[(uint32_t)v19 - 1];
            AI_EvalFunc_OBJSetLocalPos_C(v19, &v78->v_exec_3DPos);
            return 1000;
        }
    }
    else
    {
        v80 = *(uint32_t*)(GST_Carte3D_exec_Spot_Carte_ao_Object + 4);
        v18 = (tGameObjectMain*)v80;
        if (v80)
        {
            if (v80 <= 2)
                v18 = AI_MainActor[v80 - 1];
            v17 = v18;
        }
        else
        {
            v17 = 0;
        }
        if (v17)
        {
            v16 = (tGameObjectMain*)v80;
            if (v80)
            {
                if (v80 <= 2)
                    v16 = AI_MainActor[v80 - 1];
                AI_EvalFunc_OBJGetPos_C(v16, &v49);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v49);
            }
            MATH_SubstractVec(&v78->v_exec_3DPos, &v49, &v78->v_Offset);
            if (v78->i_Rotation == 1)
            {
                v56 = v78->v_exec_3DPos.x;
                v78->v_exec_3DPos.x = v78->v_exec_3DPos.y;
                v78->v_exec_3DPos.y = -v56;
            }
            else if (v78->i_Rotation == 2)
            {
                v56 = v78->v_exec_3DPos.x;
                v78->v_exec_3DPos.x = -v78->v_exec_3DPos.y;
                v78->v_exec_3DPos.y = v56;
            }
        }
        v78->v_exec_3DPos.z = v78->des_f_Z;
        v15 = (tGameObjectMain*)v78->des_o_Spot;
        if (v15)
        {
            if ((unsigned int)v15 <= 2)
                v15 = AI_MainActor[(uint32_t)v15 - 1];
            AI_EvalFunc_OBJSetLocalPos_C(v15, &v78->v_exec_3DPos);
            return 1000;
        }
    }
    AI_EvalFunc_OBJSetLocalPos_C(0, &v78->v_exec_3DPos);
    return 1000;
}

int GST_Carte3D_exec_SpotInsert()
{
    int result; // eax
    int v1; // eax
    float v2; // [esp+0h] [ebp-94h]
    tScriptStruct_GSTCarte3D* v3; // [esp+4h] [ebp-90h]
    float v4; // [esp+8h] [ebp-8Ch]
    double v5; // [esp+14h] [ebp-80h]
    tGameObjectMain* v6; // [esp+20h] [ebp-74h]
    tGameObjectMain* v7; // [esp+28h] [ebp-6Ch]
    float v8; // [esp+2Ch] [ebp-68h]
    float v9; // [esp+2Ch] [ebp-68h]
    float v10; // [esp+2Ch] [ebp-68h]
    float v11; // [esp+2Ch] [ebp-68h]
    float v12; // [esp+2Ch] [ebp-68h]
    float v13; // [esp+30h] [ebp-64h]
    float v14; // [esp+30h] [ebp-64h]
    float v15; // [esp+30h] [ebp-64h]
    Vec3 a5; // [esp+34h] [ebp-60h] BYREF
    Vec3 a3; // [esp+44h] [ebp-50h] BYREF
    Vec3 v18; // [esp+50h] [ebp-44h] BYREF
    Vec3 v19; // [esp+5Ch] [ebp-38h] BYREF
    int a4; // [esp+68h] [ebp-2Ch]
    int v21; // [esp+70h] [ebp-24h]
    tScriptStruct_GSTCarte3D* v22; // [esp+80h] [ebp-14h]
    int v23; // [esp+84h] [ebp-10h]
    Vec3 v24; // [esp+88h] [ebp-Ch] BYREF

    v19.x = 1.0;
    v19.y = 0.0;
    v19.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_GSTCarte3D*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v22 = v3;
    v21 = 0;
    a4 = 0;
    v23 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        GST_Carte3D_exec_SpotInsert_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
    }
    ++v22->i_SpotIndex;
    if (v22->i_exec_Rotate == 1)
    {
        v13 = v22->v_exec_3DPos.x;
        v22->v_exec_3DPos.x = v22->v_exec_3DPos.y;
        v22->v_exec_3DPos.y = -v13;
    }
    else if (v22->i_exec_Rotate == 2)
    {
        v14 = v22->v_exec_3DPos.x;
        v22->v_exec_3DPos.x = -v22->v_exec_3DPos.y;
        v22->v_exec_3DPos.y = v14;
    }
    v22->v_exec_3DPos.z = v22->des_f_Z;
    v7 = (tGameObjectMain*)v22->des_o_Spot;
    if (v7)
    {
        if ((unsigned int)v7 <= 2)
            v7 = AI_MainActor[(uint32_t)v7 - 1];
        AI_EvalFunc_OBJSetLocalPos_C(v7, &v22->v_exec_3DPos);
    }
    else
    {
        AI_EvalFunc_OBJSetLocalPos_C(0, &v22->v_exec_3DPos);
    }
    v6 = (tGameObjectMain*)v22->des_o_Spot;
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
    AI_EvalFunc_VIEW3dWorldTo2d_C(0, &a3, &v22->v_exec_2DPos);
    if (v22->i_exec_Icone == -1)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else if (v22->v_exec_2DPos.y >= 0.12 && v22->v_exec_2DPos.y <= 0.88)
    {
        v23 = v22->i_exec_State & 0xF;
        if ((v22->i_exec_State & 0x10) != 0)
        {
            v21 = (v22->i_exec_State & 0xFF00) >> 8;
            v8 = (float)v21;
            v9 = v8 / 128.0f;
            v15 = 1.0f - v9;
            v2 = v15 * v15;
            v1 = AI_EvalFunc_COLORBlend_C(v22->des_c_SpotColor[v23], 0, v2);
            v22->c_exec_Color = v1;
            v10 = v9 + TIM_gf_dt;
            if (v10 > 2.0)
                v10 = v10 - 2.0f;
            v11 = v10 / 2.0f;
            v12 = v11 * 255.0f;
            v5 = v12 + MATH_gd_magicDoubleForRound;
            v21 = (*(uint32_t *)&v5);
            v22->i_exec_State &= 0xFFFF00FF;
            v22->i_exec_State |= v21 << 8;
        }
        else
        {
            v22->c_exec_Color = -1;
        }
        STR_InsertString(
            (tGameObjectMain*)GST_Carte3D_exec_SpotInsert_INV_manager_gao,
            20,
            0,
            (char*)AI_gpst_CurrentFunction->localStringConstsData);
        a4 = (unsigned __int8)v22->i_exec_Icone | (16 * (v22->i_exec_Icone & 0xF00));
        AI_EvalFunc_OBJ_TextCharSet_C((tGameObjectMain*)GST_Carte3D_exec_SpotInsert_INV_manager_gao, 20, 0, a4);
        AI_EvalFunc_OBJ_TextColorSet_C(
            (tGameObjectMain*)GST_Carte3D_exec_SpotInsert_INV_manager_gao,
            20,
            0,
            1,
            v22->c_exec_Color);
        STR_SetPos((tGameObjectMain*)GST_Carte3D_exec_SpotInsert_INV_manager_gao, 20, 0, 1, &v22->v_exec_2DPos);
        a5.x = -1.0;
        a5.y = 0.0;
        a5.z = 0.0;
        STR_SetSize((tGameObjectMain*)GST_Carte3D_exec_SpotInsert_INV_manager_gao, 20, 0, 1, &a5);
        AI_EvalFunc_OBJ_TextSizeGet_C((tGameObjectMain*)GST_Carte3D_exec_SpotInsert_INV_manager_gao, 20, 0, 1, &v18);
        v19 = v18;
        if (v22->i_exec_Resize == 2)
        {
            v4 = (v22->f_ZoomScale + 3.0f) / 6.0f;
            v19.x = v19.x * v4;
            v19.y = v19.y * v4;
            v19.z = v19.z * v4;
            STR_SetSize((tGameObjectMain*)GST_Carte3D_exec_SpotInsert_INV_manager_gao, 20, 0, 1, &v19);
        }
        else if (v22->i_exec_Resize)
        {
            v24.x = v19.x * 0.5f;
            v24.y = v19.y * 0.5f;
            v24.z = v19.z * 0.5f;
            STR_SetSize((tGameObjectMain*)GST_Carte3D_exec_SpotInsert_INV_manager_gao, 20, 0, 1, &v24);
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