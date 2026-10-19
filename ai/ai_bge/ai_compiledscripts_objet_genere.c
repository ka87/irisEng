#include "ai_compiledscripts_inc.h"

//fct_Objet_genere_obge_init
int fct_Objet_genere_obge_init_Objet_genere_obge_normal;

int Objet_genere_obge_init()
{
    float v1; // [esp+0h] [ebp-64h]
    float v2; // [esp+4h] [ebp-60h]
    tScriptStruct_ObjetGenere* v3; // [esp+8h] [ebp-5Ch]
    float v4; // [esp+14h] [ebp-50h]
    Vec3 a1; // [esp+18h] [ebp-4Ch] BYREF
    int v6; // [esp+24h] [ebp-40h]
    Vec3 v7; // [esp+28h] [ebp-3Ch] BYREF
    BOOL v8; // [esp+34h] [ebp-30h]
    Vec3 v9; // [esp+38h] [ebp-2Ch]
    Vec3 a2; // [esp+44h] [ebp-20h] BYREF
    int v11; // [esp+50h] [ebp-14h]
    int v12; // [esp+54h] [ebp-10h]
    tScriptStruct_ObjetGenere* v13; // [esp+58h] [ebp-Ch]
    int v14; // [esp+5Ch] [ebp-8h]
    float v15; // [esp+60h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_ObjetGenere*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v13 = v3;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Objet_genere_obge_init_Objet_genere_obge_normal = (int)AI_ul_RealResolveOneRefFunction(0x35009FC5u);
    }
    v8 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
    v12 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 131076, 0);
    a2.x = 0.0;
    a2.y = 0.0;
    a2.z = -15.0;
    AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &a2);
    v6 = AI_EvalFunc_DYNFrictionSet_C(AI_gpst_CurrentGameObject, 1.0);
    AI_EvalFunc_DYNReboundSet_C(AI_gpst_CurrentGameObject, 0.75);
    v4 = v13->f_vitesse_dep * 0.25f;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a1);
    v9.x = -a1.x;
    v9.y = -a1.y;
    v9.z = -a1.z;
    v2 = v13->f_vitesse_dep + v4;
    v1 = v13->f_vitesse_dep - v4;
    v15 = AI_EvalFunc_MATHRandFloat_C(v1, v2);
    v7.x = v9.x * v15;
    v7.y = v9.y * v15;
    v7.z = v9.z * v15;
    v11 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v7);
    v14 = AI_EvalFunc_MATHRandInt_C(0, 3);
    v13->i_snd = v14;
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Objet_genere_obge_init_Objet_genere_obge_normal);
    return 1000;
}

int Objet_genere_obge_normal()
{
    tScriptStruct_ObjetGenere* v1; // [esp+4h] [ebp-28h]
    float v2; // [esp+Ch] [ebp-20h]
    float v3; // [esp+14h] [ebp-18h]
    float v4; // [esp+14h] [ebp-18h]
    int a1; // [esp+24h] [ebp-8h]
    int v6; // [esp+28h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_ObjetGenere*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    AI_EvalFunc_OBJRotateGlobalX_C(AI_gpst_CurrentGameObject, -0.44999999f);
    if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 64))
    {
        if (v1->f_duree_vie == 0.0)
            v1->f_duree_vie = TIM_gf_MainClock - TIM_gf_LockTime;
        a1 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(AI_gpst_CurrentGameObject, v1->i_snd, -1.0, 0.94999999f, 1.05f);
        v2 = SND_f_GetVol(a1);
        v3 = v1->f_snd_vol * v2;
        SND_SetVol(a1, v3);
        v1->f_snd_vol = v1->f_snd_blend * v1->f_snd_vol;
    }
    else if (v1->f_duree_vie != 0.0f && AI_EvalFunc_TIMEElapsed_C(v1->f_duree_vie, 1.75f)
        || (v4 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject), v4 < 0.2f))
    {
        v6 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v6 != 1000)
            return v6;
    }
    return 1000;
}