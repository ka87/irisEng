#include "ai_compiledscripts_inc.h"

//Cine_lipsynch_auto_Lips_init
int Cine_lipsynch_auto_Lips_init_Cine_lipsynch_auto_Lips_morphing;
int Cine_lipsynch_auto_Lips_init_Cine_lipsynch_auto_Lips_lecture_messages;

int Cine_lipsynch_auto_Lips_init()
{
    int result; // eax
    tScriptStruct_LipsyncAuto* v1; // [esp+0h] [ebp-418h]
    tGameObjectMain* v2; // [esp+404h] [ebp-14h]
    tGameObjectMain* v3; // [esp+408h] [ebp-10h]
    int v4; // [esp+410h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_LipsyncAuto*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Cine_lipsynch_auto_Lips_init_Cine_lipsynch_auto_Lips_morphing = (int)AI_ul_RealResolveOneRefFunction(0x3500B77Eu);
        Cine_lipsynch_auto_Lips_init_Cine_lipsynch_auto_Lips_lecture_messages = (int)AI_ul_RealResolveOneRefFunction(0x3500B773u);
    }
    v3 = (tGameObjectMain*)v1->des_o_acteur;
    if (v1->des_o_acteur)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        v2 = v3;
    }
    else
    {
        v2 = 0;
    }
    if (!v2)
        v1->des_o_acteur = (int)AI_gpst_CurrentGameObject;
    switch (v1->des_i_type_acteur)
    {
    case 1:
        v1->ai_expr_bouche_blabla_liste[0] = 7;
        v1->ai_expr_bouche_blabla_liste[1] = 6;
        v1->ai_expr_bouche_blabla_liste[2] = 7;
        v1->ai_expr_bouche_blabla_liste[3] = 6;
        v1->ai_expr_bouche_blabla_liste[4] = 6;
        v1->ai_expr_bouche_blabla_liste[5] = 8;
        v1->ai_expr_bouche_blabla_liste[6] = 6;
        v1->ai_expr_bouche_blabla_liste[7] = 7;
        v1->ai_expr_bouche_blabla_liste[8] = 8;
        v1->ai_expr_bouche_blabla_liste[9] = 6;
        v1->ai_expr_bouche_blabla_liste[10] = 9;
        v1->ai_expr_bouche_blabla_liste[11] = 6;
        v1->ai_expr_bouche_blabla_liste[12] = 7;
        v1->ai_expr_bouche_blabla_liste[13] = 8;
        v1->ai_expr_bouche_blabla_liste[14] = 6;
        v1->ai_expr_bouche_blabla_liste[15] = 9;
        v1->ai_expr_bouche_blabla_liste[16] = 7;
        v1->ai_expr_bouche_blabla_liste[17] = 7;
        v1->ai_expr_bouche_blabla_liste[18] = 6;
        v1->ai_expr_bouche_blabla_liste[19] = 6;
        v1->ai_expr_bouche_blabla_liste[20] = 8;
        v1->ai_expr_bouche_blabla_liste[21] = 9;
        v1->ai_expr_bouche_blabla_liste[22] = 6;
        v1->ai_expr_bouche_blabla_liste[23] = 7;
        v1->ai_expr_bouche_blabla_liste[24] = 6;
        v1->ai_expr_bouche_blabla_liste[25] = 8;
        v1->ai_expr_bouche_blabla_liste[26] = 6;
        v1->ai_expr_bouche_blabla_liste[27] = 9;
        v1->ai_expr_bouche_blabla_liste[28] = 6;
        v1->ai_expr_bouche_blabla_liste[29] = 7;
        v1->ai_expr_bouche_blabla_liste[30] = 8;
        v1->ai_expr_bouche_blabla_liste[31] = 6;
        v1->i_canal_tete_ouverture_bouche = 4;
        break;
    case 2:
        v1->ai_expr_bouche_blabla_liste[0] = 7;
        v1->ai_expr_bouche_blabla_liste[1] = 6;
        v1->ai_expr_bouche_blabla_liste[2] = 7;
        v1->ai_expr_bouche_blabla_liste[3] = 6;
        v1->ai_expr_bouche_blabla_liste[4] = 6;
        v1->ai_expr_bouche_blabla_liste[5] = 8;
        v1->ai_expr_bouche_blabla_liste[6] = 6;
        v1->ai_expr_bouche_blabla_liste[7] = 7;
        v1->ai_expr_bouche_blabla_liste[8] = 8;
        v1->ai_expr_bouche_blabla_liste[9] = 6;
        v1->ai_expr_bouche_blabla_liste[10] = 9;
        v1->ai_expr_bouche_blabla_liste[11] = 6;
        v1->ai_expr_bouche_blabla_liste[12] = 7;
        v1->ai_expr_bouche_blabla_liste[13] = 8;
        v1->ai_expr_bouche_blabla_liste[14] = 6;
        v1->ai_expr_bouche_blabla_liste[15] = 9;
        v1->ai_expr_bouche_blabla_liste[16] = 7;
        v1->ai_expr_bouche_blabla_liste[17] = 7;
        v1->ai_expr_bouche_blabla_liste[18] = 6;
        v1->ai_expr_bouche_blabla_liste[19] = 6;
        v1->ai_expr_bouche_blabla_liste[20] = 8;
        v1->ai_expr_bouche_blabla_liste[21] = 9;
        v1->ai_expr_bouche_blabla_liste[22] = 6;
        v1->ai_expr_bouche_blabla_liste[23] = 7;
        v1->ai_expr_bouche_blabla_liste[24] = 6;
        v1->ai_expr_bouche_blabla_liste[25] = 9;
        v1->ai_expr_bouche_blabla_liste[26] = 6;
        v1->ai_expr_bouche_blabla_liste[27] = 6;
        v1->ai_expr_bouche_blabla_liste[28] = 8;
        v1->ai_expr_bouche_blabla_liste[29] = 7;
        v1->ai_expr_bouche_blabla_liste[30] = 8;
        v1->ai_expr_bouche_blabla_liste[31] = 6;
        v1->i_canal_tete_ouverture_bouche = 3;
        break;
    case 3:
        v1->ai_expr_bouche_blabla_liste[0] = 5;
        v1->ai_expr_bouche_blabla_liste[1] = 0;
        v1->ai_expr_bouche_blabla_liste[2] = 5;
        v1->ai_expr_bouche_blabla_liste[3] = 0;
        v1->ai_expr_bouche_blabla_liste[4] = 0;
        v1->ai_expr_bouche_blabla_liste[5] = 3;
        v1->ai_expr_bouche_blabla_liste[6] = 3;
        v1->ai_expr_bouche_blabla_liste[7] = 0;
        v1->ai_expr_bouche_blabla_liste[8] = 5;
        v1->ai_expr_bouche_blabla_liste[9] = 3;
        v1->ai_expr_bouche_blabla_liste[10] = 0;
        v1->ai_expr_bouche_blabla_liste[11] = 4;
        v1->ai_expr_bouche_blabla_liste[12] = 0;
        v1->ai_expr_bouche_blabla_liste[13] = 5;
        v1->ai_expr_bouche_blabla_liste[14] = 3;
        v1->ai_expr_bouche_blabla_liste[15] = 3;
        v1->ai_expr_bouche_blabla_liste[16] = 0;
        v1->ai_expr_bouche_blabla_liste[17] = 4;
        v1->ai_expr_bouche_blabla_liste[18] = 5;
        v1->ai_expr_bouche_blabla_liste[19] = 5;
        v1->ai_expr_bouche_blabla_liste[20] = 0;
        v1->ai_expr_bouche_blabla_liste[21] = 0;
        v1->ai_expr_bouche_blabla_liste[22] = 3;
        v1->ai_expr_bouche_blabla_liste[23] = 3;
        v1->ai_expr_bouche_blabla_liste[24] = 4;
        v1->ai_expr_bouche_blabla_liste[25] = 0;
        v1->ai_expr_bouche_blabla_liste[26] = 5;
        v1->ai_expr_bouche_blabla_liste[27] = 0;
        v1->ai_expr_bouche_blabla_liste[28] = 3;
        v1->ai_expr_bouche_blabla_liste[29] = 3;
        v1->ai_expr_bouche_blabla_liste[30] = 4;
        v1->ai_expr_bouche_blabla_liste[31] = 0;
        v1->i_canal_tete_ouverture_bouche = 2;
        break;
    case 4:
        v1->ai_expr_bouche_blabla_liste[0] = 1;
        v1->ai_expr_bouche_blabla_liste[1] = 0;
        v1->ai_expr_bouche_blabla_liste[2] = 1;
        v1->ai_expr_bouche_blabla_liste[3] = 0;
        v1->ai_expr_bouche_blabla_liste[4] = 0;
        v1->ai_expr_bouche_blabla_liste[5] = 3;
        v1->ai_expr_bouche_blabla_liste[6] = 0;
        v1->ai_expr_bouche_blabla_liste[7] = 1;
        v1->ai_expr_bouche_blabla_liste[8] = 3;
        v1->ai_expr_bouche_blabla_liste[9] = 0;
        v1->ai_expr_bouche_blabla_liste[10] = 2;
        v1->ai_expr_bouche_blabla_liste[11] = 0;
        v1->ai_expr_bouche_blabla_liste[12] = 1;
        v1->ai_expr_bouche_blabla_liste[13] = 3;
        v1->ai_expr_bouche_blabla_liste[14] = 0;
        v1->ai_expr_bouche_blabla_liste[15] = 2;
        v1->ai_expr_bouche_blabla_liste[16] = 1;
        v1->ai_expr_bouche_blabla_liste[17] = 1;
        v1->ai_expr_bouche_blabla_liste[18] = 0;
        v1->ai_expr_bouche_blabla_liste[19] = 0;
        v1->ai_expr_bouche_blabla_liste[20] = 3;
        v1->ai_expr_bouche_blabla_liste[21] = 2;
        v1->ai_expr_bouche_blabla_liste[22] = 0;
        v1->ai_expr_bouche_blabla_liste[23] = 1;
        v1->ai_expr_bouche_blabla_liste[24] = 0;
        v1->ai_expr_bouche_blabla_liste[25] = 3;
        v1->ai_expr_bouche_blabla_liste[26] = 2;
        v1->ai_expr_bouche_blabla_liste[27] = 0;
        v1->ai_expr_bouche_blabla_liste[28] = 0;
        v1->ai_expr_bouche_blabla_liste[29] = 1;
        v1->ai_expr_bouche_blabla_liste[30] = 3;
        v1->ai_expr_bouche_blabla_liste[31] = 0;
        v1->i_canal_tete_ouverture_bouche = 1;
        v1->i_shape_canal_tete = 3;
        break;
    case 5:
        v1->ai_expr_bouche_blabla_liste[0] = 7;
        v1->ai_expr_bouche_blabla_liste[1] = 6;
        v1->ai_expr_bouche_blabla_liste[2] = 7;
        v1->ai_expr_bouche_blabla_liste[3] = 6;
        v1->ai_expr_bouche_blabla_liste[4] = 6;
        v1->ai_expr_bouche_blabla_liste[5] = 8;
        v1->ai_expr_bouche_blabla_liste[6] = 6;
        v1->ai_expr_bouche_blabla_liste[7] = 7;
        v1->ai_expr_bouche_blabla_liste[8] = 8;
        v1->ai_expr_bouche_blabla_liste[9] = 6;
        v1->ai_expr_bouche_blabla_liste[10] = 9;
        v1->ai_expr_bouche_blabla_liste[11] = 6;
        v1->ai_expr_bouche_blabla_liste[12] = 7;
        v1->ai_expr_bouche_blabla_liste[13] = 8;
        v1->ai_expr_bouche_blabla_liste[14] = 6;
        v1->ai_expr_bouche_blabla_liste[15] = 9;
        v1->ai_expr_bouche_blabla_liste[16] = 7;
        v1->ai_expr_bouche_blabla_liste[17] = 7;
        v1->ai_expr_bouche_blabla_liste[18] = 6;
        v1->ai_expr_bouche_blabla_liste[19] = 6;
        v1->ai_expr_bouche_blabla_liste[20] = 8;
        v1->ai_expr_bouche_blabla_liste[21] = 9;
        v1->ai_expr_bouche_blabla_liste[22] = 6;
        v1->ai_expr_bouche_blabla_liste[23] = 7;
        v1->ai_expr_bouche_blabla_liste[24] = 6;
        v1->ai_expr_bouche_blabla_liste[25] = 9;
        v1->ai_expr_bouche_blabla_liste[26] = 6;
        v1->ai_expr_bouche_blabla_liste[27] = 6;
        v1->ai_expr_bouche_blabla_liste[28] = 8;
        v1->ai_expr_bouche_blabla_liste[29] = 8;
        v1->ai_expr_bouche_blabla_liste[30] = 7;
        v1->ai_expr_bouche_blabla_liste[31] = 6;
        v1->i_canal_tete_ouverture_bouche = 4;
        v1->i_shape_canal_tete = 4;
        break;
    case 6:
        v1->ai_expr_bouche_blabla_liste[0] = 9;
        v1->ai_expr_bouche_blabla_liste[1] = 0;
        v1->ai_expr_bouche_blabla_liste[2] = 9;
        v1->ai_expr_bouche_blabla_liste[3] = 0;
        v1->ai_expr_bouche_blabla_liste[4] = 0;
        v1->ai_expr_bouche_blabla_liste[5] = 10;
        v1->ai_expr_bouche_blabla_liste[6] = 0;
        v1->ai_expr_bouche_blabla_liste[7] = 9;
        v1->ai_expr_bouche_blabla_liste[8] = 10;
        v1->ai_expr_bouche_blabla_liste[9] = 0;
        v1->ai_expr_bouche_blabla_liste[10] = 11;
        v1->ai_expr_bouche_blabla_liste[11] = 0;
        v1->ai_expr_bouche_blabla_liste[12] = 9;
        v1->ai_expr_bouche_blabla_liste[13] = 10;
        v1->ai_expr_bouche_blabla_liste[14] = 0;
        v1->ai_expr_bouche_blabla_liste[15] = 11;
        v1->ai_expr_bouche_blabla_liste[16] = 9;
        v1->ai_expr_bouche_blabla_liste[17] = 9;
        v1->ai_expr_bouche_blabla_liste[18] = 0;
        v1->ai_expr_bouche_blabla_liste[19] = 0;
        v1->ai_expr_bouche_blabla_liste[20] = 10;
        v1->ai_expr_bouche_blabla_liste[21] = 11;
        v1->ai_expr_bouche_blabla_liste[22] = 0;
        v1->ai_expr_bouche_blabla_liste[23] = 11;
        v1->ai_expr_bouche_blabla_liste[24] = 0;
        v1->ai_expr_bouche_blabla_liste[25] = 9;
        v1->ai_expr_bouche_blabla_liste[26] = 0;
        v1->ai_expr_bouche_blabla_liste[27] = 0;
        v1->ai_expr_bouche_blabla_liste[28] = 10;
        v1->ai_expr_bouche_blabla_liste[29] = 10;
        v1->ai_expr_bouche_blabla_liste[30] = 11;
        v1->ai_expr_bouche_blabla_liste[31] = 0;
        v1->i_canal_tete_ouverture_bouche = 4;
        break;
    case 7:
        v1->ai_expr_bouche_blabla_liste[0] = 5;
        v1->ai_expr_bouche_blabla_liste[1] = 0;
        v1->ai_expr_bouche_blabla_liste[2] = 5;
        v1->ai_expr_bouche_blabla_liste[3] = 0;
        v1->ai_expr_bouche_blabla_liste[4] = 0;
        v1->ai_expr_bouche_blabla_liste[5] = 4;
        v1->ai_expr_bouche_blabla_liste[6] = 0;
        v1->ai_expr_bouche_blabla_liste[7] = 5;
        v1->ai_expr_bouche_blabla_liste[8] = 4;
        v1->ai_expr_bouche_blabla_liste[9] = 0;
        v1->ai_expr_bouche_blabla_liste[10] = 6;
        v1->ai_expr_bouche_blabla_liste[11] = 0;
        v1->ai_expr_bouche_blabla_liste[12] = 5;
        v1->ai_expr_bouche_blabla_liste[13] = 4;
        v1->ai_expr_bouche_blabla_liste[14] = 0;
        v1->ai_expr_bouche_blabla_liste[15] = 6;
        v1->ai_expr_bouche_blabla_liste[16] = 5;
        v1->ai_expr_bouche_blabla_liste[17] = 5;
        v1->ai_expr_bouche_blabla_liste[18] = 0;
        v1->ai_expr_bouche_blabla_liste[19] = 0;
        v1->ai_expr_bouche_blabla_liste[20] = 4;
        v1->ai_expr_bouche_blabla_liste[21] = 6;
        v1->ai_expr_bouche_blabla_liste[22] = 0;
        v1->ai_expr_bouche_blabla_liste[23] = 6;
        v1->ai_expr_bouche_blabla_liste[24] = 0;
        v1->ai_expr_bouche_blabla_liste[25] = 5;
        v1->ai_expr_bouche_blabla_liste[26] = 0;
        v1->ai_expr_bouche_blabla_liste[27] = 0;
        v1->ai_expr_bouche_blabla_liste[28] = 4;
        v1->ai_expr_bouche_blabla_liste[29] = 4;
        v1->ai_expr_bouche_blabla_liste[30] = 6;
        v1->ai_expr_bouche_blabla_liste[31] = 0;
        v1->i_canal_tete_ouverture_bouche = 4;
        break;
    default:
        v1->ai_expr_bouche_blabla_liste[0] = 6;
        v1->ai_expr_bouche_blabla_liste[1] = 4;
        v1->ai_expr_bouche_blabla_liste[2] = 4;
        v1->ai_expr_bouche_blabla_liste[3] = 4;
        v1->ai_expr_bouche_blabla_liste[4] = 1;
        v1->ai_expr_bouche_blabla_liste[5] = 4;
        v1->ai_expr_bouche_blabla_liste[6] = 5;
        v1->ai_expr_bouche_blabla_liste[7] = 5;
        v1->ai_expr_bouche_blabla_liste[8] = 5;
        v1->ai_expr_bouche_blabla_liste[9] = 3;
        v1->ai_expr_bouche_blabla_liste[10] = 5;
        v1->ai_expr_bouche_blabla_liste[11] = 1;
        v1->ai_expr_bouche_blabla_liste[12] = 1;
        v1->ai_expr_bouche_blabla_liste[13] = 4;
        v1->ai_expr_bouche_blabla_liste[14] = 4;
        v1->ai_expr_bouche_blabla_liste[15] = 3;
        v1->ai_expr_bouche_blabla_liste[16] = 4;
        v1->ai_expr_bouche_blabla_liste[17] = 4;
        v1->ai_expr_bouche_blabla_liste[18] = 1;
        v1->ai_expr_bouche_blabla_liste[19] = 5;
        v1->ai_expr_bouche_blabla_liste[20] = 5;
        v1->ai_expr_bouche_blabla_liste[21] = 4;
        v1->ai_expr_bouche_blabla_liste[22] = 1;
        v1->ai_expr_bouche_blabla_liste[23] = 4;
        v1->ai_expr_bouche_blabla_liste[24] = 5;
        v1->ai_expr_bouche_blabla_liste[25] = 5;
        v1->ai_expr_bouche_blabla_liste[26] = 1;
        v1->ai_expr_bouche_blabla_liste[27] = 6;
        v1->ai_expr_bouche_blabla_liste[28] = 4;
        v1->ai_expr_bouche_blabla_liste[29] = 3;
        v1->ai_expr_bouche_blabla_liste[30] = 4;
        v1->ai_expr_bouche_blabla_liste[31] = 1;
        break;
    }
    *(uint32_t*)AI_EvalVarArray_C2((char*)&v1->__f_expr_flag_Dimension1, v1->i_canal_tete_ouverture_bouche) = 0x40000000;
    AI_EvalFunc_AITrackChange_C(3, (tAIFunction*)Cine_lipsynch_auto_Lips_init_Cine_lipsynch_auto_Lips_morphing);
    v4 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)Cine_lipsynch_auto_Lips_init_Cine_lipsynch_auto_Lips_lecture_messages);
    if (v4 == 1000)
        result = 1000;
    else
        result = v4;
    return result;
}

int Cine_lipsynch_auto_Lips_morphing()
{
    tGameObjectMain* v0; // eax
    int v1; // esi
    int v3; // eax
    unsigned int v4; // rax
    float* v5; // eax
    float* v6; // eax
    float v7; // [esp+8h] [ebp-70h]
    float v8; // [esp+Ch] [ebp-6Ch]
    tScriptStruct_LipsyncAuto* v9; // [esp+10h] [ebp-68h]
    tGameObjectMain* v10; // [esp+14h] [ebp-64h]
    tGameObjectMain* v11; // [esp+18h] [ebp-60h]
    tGameObjectMain* a1; // [esp+1Ch] [ebp-5Ch]
    tGameObjectMain* v13; // [esp+20h] [ebp-58h]
    float v14; // [esp+24h] [ebp-54h]
    float v15; // [esp+28h] [ebp-50h]
    float v16; // [esp+2Ch] [ebp-4Ch]
    float v17; // [esp+30h] [ebp-48h]
    tGameObjectMain* v18; // [esp+34h] [ebp-44h]
    tGameObjectMain* v19; // [esp+38h] [ebp-40h]
    tGameObjectMain* v20; // [esp+40h] [ebp-38h]
    tGameObjectMain* v21; // [esp+48h] [ebp-30h]
    tGameObjectMain* v22; // [esp+50h] [ebp-28h]
    float v23; // [esp+54h] [ebp-24h]
    unsigned int v24; // [esp+58h] [ebp-20h]
    float v25; // [esp+5Ch] [ebp-1Ch]
    float v26; // [esp+64h] [ebp-14h]

    if (AI_gpst_CurrentInstance)
        v9 = (tScriptStruct_LipsyncAuto*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v9 = 0;
    v22 = (tGameObjectMain*)v9->des_o_acteur;
    if (v9->des_o_acteur)
    {
        if ((unsigned int)v22 <= 2)
            v22 = AI_MainActor[(uint32_t)v22 - 1];
        v0 = AI_EvalFunc_ANICanalGaoGet_C(v22, v9->i_shape_canal_tete);
    }
    else
    {
        v0 = AI_EvalFunc_ANICanalGaoGet_C(0, v9->i_shape_canal_tete);
    }
    v24 = (unsigned int)v0;
    if (*(float*)AI_EvalVarArray_C2((char*)&v9->__f_expr_flag_Dimension1, v9->i_canal_tete_ouverture_bouche) == 2.0f)
    {
        if (!v9->i_expr_bouche_auto)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v1 = v9->i_expr_bouche_blabla_id;
        *(float*)AI_EvalVarArray_C2((char*)&v9->__f_expr_forme_Dimension1, v9->i_canal_tete_ouverture_bouche) = (float)v9->ai_expr_bouche_blabla_liste[v1];
        v9->i_expr_bouche_blabla_id = AI_EvalFunc_MTHModulo_C(v9->i_expr_bouche_blabla_id + 1, 32);
        *(uint32_t*)AI_EvalVarArray_C2((char*)&v9->__f_expr_flag_Dimension1, v9->i_canal_tete_ouverture_bouche) = 0x3F800000;
    }
    if (*(float*)AI_EvalVarArray_C2((char*)&v9->__f_expr_flag_Dimension1, v9->i_canal_tete_ouverture_bouche) != 2.0f)
    {
        if (*(float*)AI_EvalVarArray_C2((char*)&v9->__f_expr_flag_Dimension1, v9->i_canal_tete_ouverture_bouche) == 1.0f)
        {
            v21 = (tGameObjectMain*)v24;
            if (v24)
            {
                if (v24 <= 2)
                    v21 = AI_MainActor[v24 - 1];
                v3 = AI_EvalFunc_OBJ_GetChannelData_C(v21, v9->i_canal_tete_ouverture_bouche, 1);
            }
            else
            {
                v3 = AI_EvalFunc_OBJ_GetChannelData_C(0, v9->i_canal_tete_ouverture_bouche, 1);
            }
            v20 = (tGameObjectMain*)v24;
            if (v24)
            {
                if (v24 <= 2)
                    v20 = AI_MainActor[v24 - 1];
                AI_EvalFunc_OBJ_SetChannelData_C(v20, v9->i_canal_tete_ouverture_bouche, v3, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_SetChannelData_C(0, v9->i_canal_tete_ouverture_bouche, v3, 0);
            }
            v19 = (tGameObjectMain*)v24;
            if (v24)
            {
                if (v24 <= 2)
                    v19 = AI_MainActor[v24 - 1];
                v18 = v19;
            }
            else
            {
                v18 = 0;
            }
            v4 = (unsigned int)*(float*)AI_EvalVarArray_C2(
                (char*)&v9->__f_expr_forme_Dimension1,
                v9->i_canal_tete_ouverture_bouche);
            AI_EvalFunc_OBJ_SetChannelData_C(v18, v9->i_canal_tete_ouverture_bouche, v4, 1);
            *(uint32_t*)AI_EvalVarArray_C2((char*)&v9->__f_expr_prog_current_Dimension1, v9->i_canal_tete_ouverture_bouche) = 0x3F800000;
            *(uint32_t*)AI_EvalVarArray_C2((char*)&v9->__f_expr_flag_Dimension1, v9->i_canal_tete_ouverture_bouche) = 0;
        }
        v25 = TIM_gf_dt;
        v17 = *(float*)AI_EvalVarArray_C2((char*)&v9->__f_expr_speed_Dimension1, v9->i_canal_tete_ouverture_bouche) * v25;
        v16 = *(float*)AI_EvalVarArray_C2(
            (char*)&v9->__f_expr_factor_target_Dimension1,
            v9->i_canal_tete_ouverture_bouche);
        v15 = *(float*)AI_EvalVarArray_C2(
            (char*)&v9->__f_expr_factor_current_Dimension1,
            v9->i_canal_tete_ouverture_bouche);
        if (v17 < 0.0)
            v17 = 0.0;
        if (v17 > 1.0)
            v17 = 1.0;
        v23 = v16 * v17 + (1.0f - v17) * v15;
        *(float*)AI_EvalVarArray_C2((char*)&v9->__f_expr_factor_current_Dimension1, v9->i_canal_tete_ouverture_bouche) = v23;
        v26 = TIM_gf_dt;
        v8 = *(float*)AI_EvalVarArray_C2((char*)&v9->__f_expr_speed_Dimension1, v9->i_canal_tete_ouverture_bouche) * v26;
        v14 = *(float*)AI_EvalVarArray_C2((char*)&v9->__f_expr_prog_current_Dimension1, v9->i_canal_tete_ouverture_bouche)
            + v8;
        if (v14 < 2.0)
            v7 = v14;
        else
            v7 = 2.0;
        *(float*)AI_EvalVarArray_C2((char*)&v9->__f_expr_prog_current_Dimension1, v9->i_canal_tete_ouverture_bouche) = v7;
        v13 = (tGameObjectMain*)v24;
        if (v24)
        {
            if (v24 <= 2)
                v13 = AI_MainActor[v24 - 1];
            a1 = v13;
        }
        else
        {
            a1 = 0;
        }
        v5 = (float*)AI_EvalVarArray_C2((char*)&v9->__f_expr_factor_current_Dimension1, v9->i_canal_tete_ouverture_bouche);
        AI_EvalFunc_OBJ_SetChannelFactor_C(a1, v9->i_canal_tete_ouverture_bouche, *v5);
        v11 = (tGameObjectMain*)v24;
        if (v24)
        {
            if (v24 <= 2)
                v11 = AI_MainActor[v24 - 1];
            v10 = v11;
        }
        else
        {
            v10 = 0;
        }
        v6 = (float*)AI_EvalVarArray_C2((char*)&v9->__f_expr_prog_current_Dimension1, v9->i_canal_tete_ouverture_bouche);
        AI_EvalFunc_OBJ_SetChannelProg_C(v10, v9->i_canal_tete_ouverture_bouche, *v6);
        if (*(float*)AI_EvalVarArray_C2((char*)&v9->__f_expr_prog_current_Dimension1, v9->i_canal_tete_ouverture_bouche)
            - 1.99f >= 0.0f)
            *(uint32_t*)AI_EvalVarArray_C2((char*)&v9->__f_expr_flag_Dimension1, v9->i_canal_tete_ouverture_bouche) = 0x40000000;
    }
    return 1000;
}

int Cine_lipsynch_auto_Lips_lecture_messages()
{
    float v1; // [esp+8h] [ebp-4Ch]
    float v2; // [esp+Ch] [ebp-48h]
    float v3; // [esp+10h] [ebp-44h]
    float v4; // [esp+14h] [ebp-40h]
    tScriptStruct_LipsyncAuto* v5; // [esp+18h] [ebp-3Ch]
    double v6; // [esp+1Ch] [ebp-38h]
    tGameObjectMain* v7; // [esp+2Ch] [ebp-28h]
    tGameObjectMain* v8; // [esp+30h] [ebp-24h]
    float v9; // [esp+34h] [ebp-20h]
    float v10; // [esp+34h] [ebp-20h]
    void* Src; // [esp+38h] [ebp-1Ch]
    float v12; // [esp+40h] [ebp-14h]

    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_LipsyncAuto*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v9 = 0.0;
    Src = AI_EvalFunc_MSGGetByID_C(AI_gpst_CurrentGameObject, 53);
    memcpy(&v5->m_blink, Src, sizeof(v5->m_blink));
    if (v5->m_blink.msgID == 53)
    {
        v8 = v5->m_blink.gao;
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
            v5->des_o_acteur = (int)v5->m_blink.gao;
        v12 = v5->m_blink.v2.y;
        v6 = v12 + MATH_gd_magicDoubleForRound;
        if ((*(uint32_t*)&v6))
        {
            *(float*)AI_EvalVarArray_C2((char*)&v5->__f_expr_speed_Dimension1, v5->i_canal_tete_ouverture_bouche) = (float)v5->m_blink.Int1;
            v4 = (float)(*(int32_t*)&v6);
            v10 = v4 / 1000.0f;
            v3 = (float)((*(int32_t*)&v6) / 1000);
            v9 = v10 - v3;
            v2 = (float)fabs(v9);
            v1 = (v2 - 0.5f) * 10.0f;
            *(float*)AI_EvalVarArray_C2((char*)&v5->__f_expr_factor_target_Dimension1, v5->i_canal_tete_ouverture_bouche) = v1;
            *(float*)AI_EvalVarArray_C2((char*)&v5->__f_expr_forme_Dimension1, v5->i_canal_tete_ouverture_bouche) = v3;
            *(uint32_t*)AI_EvalVarArray_C2((char*)&v5->__f_expr_flag_Dimension1, v5->i_canal_tete_ouverture_bouche) = 0x3F800000;
        }
        if (v12 != 0.0f)
        {
            if (v9 >= 0.0f)
            {
                v5->i_expr_bouche_auto = 0;
            }
            else
            {
                v5->i_expr_bouche_auto = 1;
                *(uint32_t*)AI_EvalVarArray_C2((char*)&v5->__f_expr_flag_Dimension1, v5->i_canal_tete_ouverture_bouche) = 0x40000000;
            }
        }
    }
    return 1000;
}