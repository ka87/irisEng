#include "ai_compiledscripts_inc.h"

//BV_Active_Desactive_BV_CBits_Out
int fct_BV_Active_Desactive_BV_CBits_Out_BV_Active_Desactive_BV_CBits_In;

//BV_Active_Desactive_BV_In
int fct_BV_Active_Desactive_BV_In_BV_Active_Desactive_BV_Out;

//BV_Active_Desactive_BV_AD
int fct_BV_Active_Desactive_BV_AD_BV_Active_Desactive_BV_CBits_Out;
int fct_BV_Active_Desactive_BV_AD_BV_Active_Desactive_BV_Out;

//BV_Active_Desactive_BV_Out
int fct_BV_Active_Desactive_BV_Out_BV_Active_Desactive_BV_In;

//BV_Active_Desactive_BV_CBits_In
int fct_BV_Active_Desactive_BV_CBits_In_BV_Active_Desactive_BV_CBits_Out;

int BV_Active_Desactive_BV_CBits_Out()
{
    tGameObjectMain* v0; // eax
    BOOL v1; // eax
    BOOL v2; // eax
    tScriptStruct_BVActiveDesactive* v4; // [esp+0h] [ebp-C4h]
    tGameObjectMain* v5; // [esp+8h] [ebp-BCh]
    tGameObjectMain* v6; // [esp+Ch] [ebp-B8h]
    tGameObjectMain* v7; // [esp+10h] [ebp-B4h]
    tGameObjectMain* v8; // [esp+18h] [ebp-ACh]
    tGameObjectMain* v9; // [esp+1Ch] [ebp-A8h]
    tGameObjectMain* v10; // [esp+20h] [ebp-A4h]
    tGameObjectMain* v11; // [esp+28h] [ebp-9Ch]
    tGameObjectMain* v12; // [esp+30h] [ebp-94h]
    tGameObjectMain* v13; // [esp+38h] [ebp-8Ch]
    tGameObjectMain* v14; // [esp+3Ch] [ebp-88h]
    tGameObjectMain* v15; // [esp+40h] [ebp-84h]
    tGameObjectMain* v16; // [esp+48h] [ebp-7Ch]
    tGameObjectMain* v17; // [esp+50h] [ebp-74h]
    tGameObjectMain* v18; // [esp+54h] [ebp-70h]
    tGameObjectMain* v19; // [esp+58h] [ebp-6Ch]
    tGameObjectMain* v20; // [esp+60h] [ebp-64h]
    tGameObjectMain* v21; // [esp+68h] [ebp-5Ch]
    tGameObjectMain* v22; // [esp+6Ch] [ebp-58h]
    tGameObjectMain* v23; // [esp+70h] [ebp-54h]
    tGameObjectMain* v24; // [esp+78h] [ebp-4Ch]
    tGameObjectMain* v25; // [esp+80h] [ebp-44h]
    tGameObjectMain* v26; // [esp+88h] [ebp-3Ch]
    tGameObjectMain* v27; // [esp+90h] [ebp-34h]
    tGameObjectMain* v28; // [esp+94h] [ebp-30h]
    tGameObjectMain* v29; // [esp+98h] [ebp-2Ch]
    tGameObjectMain* v30; // [esp+9Ch] [ebp-28h]
    tGameObjectMain* v31; // [esp+A0h] [ebp-24h]
    tGameObjectMain* v32; // [esp+A8h] [ebp-1Ch]
    int v33; // [esp+ACh] [ebp-18h]
    BOOL a2; // [esp+C0h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_BVActiveDesactive*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_BV_Active_Desactive_BV_CBits_Out_BV_Active_Desactive_BV_CBits_In = (int)AI_ul_RealResolveOneRefFunction(0x7100F73Cu);
    }
    v32 = (tGameObjectMain*)v4->o_BV_a_qui;
    if (v32)
    {
        if ((unsigned int)v32 <= 2)
            v32 = AI_MainActor[(uint32_t)v32 - 1];
        v0 = AI_EvalFunc_COLBVGetFirstOverlap_C(v32, v4->acteur_avec_CBit, 0, 5);
    }
    else
    {
        v0 = AI_EvalFunc_COLBVGetFirstOverlap_C(0, v4->acteur_avec_CBit, 0, 5);
    }
    v31 = v0;
    if (v0)
    {
        if ((unsigned int)v0 <= 2)
            v31 = AI_MainActor[(uint32_t)v0 - 1];
        v30 = v31;
    }
    else
    {
        v30 = 0;
    }
    if (v30)
    {
        if (!v4->a_active)
        {
            a2 = v4->active_dans_bv == 0;
            v29 = (tGameObjectMain*)v4->object_a_activer1;
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
                if (v4->ia_reinit1)
                {
                    v27 = (tGameObjectMain*)v4->object_a_activer1;
                    if (v27)
                    {
                        if ((unsigned int)v27 <= 2)
                            v27 = AI_MainActor[(uint32_t)v27 - 1];
                        AI_EvalFunc_AIReinitVars_C(v27);
                    }
                    else
                    {
                        AI_EvalFunc_AIReinitVars_C(0);
                    }
                }
                if (!v4->ne_fait_pas_l_activation)
                {
                    v26 = (tGameObjectMain*)v4->object_a_activer1;
                    if (v26)
                    {
                        if ((unsigned int)v26 <= 2)
                            v26 = AI_MainActor[(uint32_t)v26 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v26, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, a2);
                    }
                    v25 = (tGameObjectMain*)v4->object_a_activer1;
                    if (v25)
                    {
                        if ((unsigned int)v25 <= 2)
                            v25 = AI_MainActor[(uint32_t)v25 - 1];
                        v1 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v25, 0x800000);
                    }
                    else
                    {
                        v1 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x800000);
                    }
                    if (v1)
                    {
                        v24 = (tGameObjectMain*)v4->object_a_activer1;
                        if (v24)
                        {
                            if ((unsigned int)v24 <= 2)
                                v24 = AI_MainActor[(uint32_t)v24 - 1];
                            AI_EvalFunc_GRPInactiveSet_C(v24, a2);
                        }
                        else
                        {
                            AI_EvalFunc_GRPInactiveSet_C(0, a2);
                        }
                    }
                }
            }
            v23 = (tGameObjectMain*)v4->object_a_activer2;
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
                if (v4->ia_reinit2)
                {
                    v21 = (tGameObjectMain*)v4->object_a_activer2;
                    if (v21)
                    {
                        if ((unsigned int)v21 <= 2)
                            v21 = AI_MainActor[(uint32_t)v21 - 1];
                        AI_EvalFunc_AIReinitVars_C(v21);
                    }
                    else
                    {
                        AI_EvalFunc_AIReinitVars_C(0);
                    }
                }
                if (!v4->ne_fait_pas_l_activation)
                {
                    v20 = (tGameObjectMain*)v4->object_a_activer2;
                    if (v20)
                    {
                        if ((unsigned int)v20 <= 2)
                            v20 = AI_MainActor[(uint32_t)v20 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v20, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, a2);
                    }
                }
            }
            v19 = (tGameObjectMain*)v4->object_a_activer3;
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
                if (v4->ia_reinit3)
                {
                    v17 = (tGameObjectMain*)v4->object_a_activer3;
                    if (v17)
                    {
                        if ((unsigned int)v17 <= 2)
                            v17 = AI_MainActor[(uint32_t)v17 - 1];
                        AI_EvalFunc_AIReinitVars_C(v17);
                    }
                    else
                    {
                        AI_EvalFunc_AIReinitVars_C(0);
                    }
                }
                if (!v4->ne_fait_pas_l_activation)
                {
                    v16 = (tGameObjectMain*)v4->object_a_activer3;
                    if (v16)
                    {
                        if ((unsigned int)v16 <= 2)
                            v16 = AI_MainActor[(uint32_t)v16 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v16, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, a2);
                    }
                }
            }
            if (v4->rend_invisible_aussi)
            {
                v15 = (tGameObjectMain*)v4->object_a_activer1;
                if (v15)
                {
                    if ((unsigned int)v15 <= 2)
                        v15 = AI_MainActor[(uint32_t)v15 - 1];
                    v14 = v15;
                }
                else
                {
                    v14 = 0;
                }
                if (v14)
                {
                    v13 = (tGameObjectMain*)v4->object_a_activer1;
                    if (v13)
                    {
                        if ((unsigned int)v13 <= 2)
                            v13 = AI_MainActor[(uint32_t)v13 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v13, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, a2);
                    }
                    v12 = (tGameObjectMain*)v4->object_a_activer1;
                    if (v12)
                    {
                        if ((unsigned int)v12 <= 2)
                            v12 = AI_MainActor[(uint32_t)v12 - 1];
                        v2 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v12, 0x800000);
                    }
                    else
                    {
                        v2 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x800000);
                    }
                    if (v2)
                    {
                        v11 = (tGameObjectMain*)v4->object_a_activer1;
                        if (v11)
                        {
                            if ((unsigned int)v11 <= 2)
                                v11 = AI_MainActor[(uint32_t)v11 - 1];
                            AI_EvalFunc_GRPInvisibleSet_C(v11, a2);
                        }
                        else
                        {
                            AI_EvalFunc_GRPInvisibleSet_C(0, a2);
                        }
                    }
                }
                v10 = (tGameObjectMain*)v4->object_a_activer2;
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
                    v8 = (tGameObjectMain*)v4->object_a_activer2;
                    if (v8)
                    {
                        if ((unsigned int)v8 <= 2)
                            v8 = AI_MainActor[(uint32_t)v8 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v8, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, a2);
                    }
                }
                v7 = (tGameObjectMain*)v4->object_a_activer3;
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
                    v5 = (tGameObjectMain*)v4->object_a_activer3;
                    if (v5)
                    {
                        if ((unsigned int)v5 <= 2)
                            v5 = AI_MainActor[(uint32_t)v5 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v5, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, a2);
                    }
                }
            }
            if (!v4->ne_fait_qu_activer)
            {
                v33 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_BV_Active_Desactive_BV_CBits_Out_BV_Active_Desactive_BV_CBits_In);
                if (v33 != 1000)
                    return v33;
            }
            v4->a_active = 1;
        }
        if (v4->se_desactive_apres)
        {
            v4->a_active = 0;
            AI_EvalFunc_OBJ_FlagInactiveSet_C(AI_gpst_CurrentGameObject, 1);
        }
    }
    else
    {
        v4->a_active = 0;
    }
    return 1000;
}

int BV_Active_Desactive_BV_In()
{
    BOOL v0; // eax
    int v1; // eax
    BOOL v2; // eax
    BOOL v3; // eax
    tScriptStruct_BVActiveDesactive* v5; // [esp+0h] [ebp-D8h]
    tGameObjectMain* v6; // [esp+8h] [ebp-D0h]
    tGameObjectMain* v7; // [esp+Ch] [ebp-CCh]
    tGameObjectMain* v8; // [esp+10h] [ebp-C8h]
    tGameObjectMain* v9; // [esp+18h] [ebp-C0h]
    tGameObjectMain* v10; // [esp+1Ch] [ebp-BCh]
    tGameObjectMain* v11; // [esp+20h] [ebp-B8h]
    tGameObjectMain* v12; // [esp+28h] [ebp-B0h]
    tGameObjectMain* v13; // [esp+30h] [ebp-A8h]
    tGameObjectMain* v14; // [esp+38h] [ebp-A0h]
    tGameObjectMain* v15; // [esp+3Ch] [ebp-9Ch]
    tGameObjectMain* v16; // [esp+40h] [ebp-98h]
    tGameObjectMain* v17; // [esp+48h] [ebp-90h]
    tGameObjectMain* v18; // [esp+50h] [ebp-88h]
    tGameObjectMain* v19; // [esp+54h] [ebp-84h]
    tGameObjectMain* v20; // [esp+58h] [ebp-80h]
    tGameObjectMain* v21; // [esp+60h] [ebp-78h]
    tGameObjectMain* v22; // [esp+68h] [ebp-70h]
    tGameObjectMain* v23; // [esp+6Ch] [ebp-6Ch]
    tGameObjectMain* v24; // [esp+70h] [ebp-68h]
    tGameObjectMain* v25; // [esp+78h] [ebp-60h]
    tGameObjectMain* v26; // [esp+80h] [ebp-58h]
    tGameObjectMain* v27; // [esp+88h] [ebp-50h]
    tGameObjectMain* v28; // [esp+90h] [ebp-48h]
    tGameObjectMain* v29; // [esp+94h] [ebp-44h]
    tGameObjectMain* v30; // [esp+98h] [ebp-40h]
    tGameObjectMain* v31; // [esp+A0h] [ebp-38h]
    tGameObjectMain* v32; // [esp+A4h] [ebp-34h]
    tGameObjectMain* v33; // [esp+A8h] [ebp-30h]
    tGameObjectMain* v34; // [esp+B0h] [ebp-28h]
    tGameObjectMain* v35; // [esp+B4h] [ebp-24h]
    tGameObjectMain* v36; // [esp+B8h] [ebp-20h]
    int v37; // [esp+BCh] [ebp-1Ch]
    BOOL a2; // [esp+D4h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_BVActiveDesactive*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_BV_Active_Desactive_BV_In_BV_Active_Desactive_BV_Out = (int)AI_ul_RealResolveOneRefFunction(0x7100F571u);
    }
    if (!v5->i_bv_pivot)
    {
        v36 = (tGameObjectMain*)v5->acteur_a_detecter;
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
        v34 = (tGameObjectMain*)v5->o_BV_a_qui;
        if (v34)
        {
            if ((unsigned int)v34 <= 2)
                v34 = AI_MainActor[(uint32_t)v34 - 1];
            v0 = AI_EvalFunc_COLBVObjectObject_C(v34, v35);
        }
        else
        {
            v0 = AI_EvalFunc_COLBVObjectObject_C(0, v35);
        }
        if (!v0)
            goto LABEL_151;
    }
    if (!v5->i_bv_pivot)
        goto LABEL_147;
    v33 = (tGameObjectMain*)v5->acteur_a_detecter;
    if (v33)
    {
        if ((unsigned int)v33 <= 2)
            v33 = AI_MainActor[(uint32_t)v33 - 1];
        v32 = v33;
    }
    else
    {
        v32 = 0;
    }
    v31 = (tGameObjectMain*)v5->o_BV_a_qui;
    if (v31)
    {
        if ((unsigned int)v31 <= 2)
            v31 = AI_MainActor[(uint32_t)v31 - 1];
        v1 = AI_EvalFunc_COLBVPivotCollide_C(v31, v32);
    }
    else
    {
        v1 = AI_EvalFunc_COLBVPivotCollide_C(0, v32);
    }
    if (v1)
    {
    LABEL_147:
        v5->a_active = 0;
    }
    else
    {
    LABEL_151:
        if (!v5->a_active)
        {
            a2 = v5->active_dans_bv != 0;
            v30 = (tGameObjectMain*)v5->object_a_activer1;
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
                if (v5->ia_reinit1)
                {
                    v28 = (tGameObjectMain*)v5->object_a_activer1;
                    if (v28)
                    {
                        if ((unsigned int)v28 <= 2)
                            v28 = AI_MainActor[(uint32_t)v28 - 1];
                        AI_EvalFunc_AIReinitVars_C(v28);
                    }
                    else
                    {
                        AI_EvalFunc_AIReinitVars_C(0);
                    }
                }
                if (!v5->ne_fait_pas_l_activation)
                {
                    v27 = (tGameObjectMain*)v5->object_a_activer1;
                    if (v27)
                    {
                        if ((unsigned int)v27 <= 2)
                            v27 = AI_MainActor[(uint32_t)v27 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v27, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, a2);
                    }
                    v26 = (tGameObjectMain*)v5->object_a_activer1;
                    if (v26)
                    {
                        if ((unsigned int)v26 <= 2)
                            v26 = AI_MainActor[(uint32_t)v26 - 1];
                        v2 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v26, 0x800000);
                    }
                    else
                    {
                        v2 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x800000);
                    }
                    if (v2)
                    {
                        v25 = (tGameObjectMain*)v5->object_a_activer1;
                        if (v25)
                        {
                            if ((unsigned int)v25 <= 2)
                                v25 = AI_MainActor[(uint32_t)v25 - 1];
                            AI_EvalFunc_GRPInactiveSet_C(v25, a2);
                        }
                        else
                        {
                            AI_EvalFunc_GRPInactiveSet_C(0, a2);
                        }
                    }
                }
            }
            v24 = (tGameObjectMain*)v5->object_a_activer2;
            if (v24)
            {
                if ((unsigned int)v24 <= 2)
                    v24 = AI_MainActor[(uint32_t)v24 - 1];
                v23 = v24;
            }
            else
            {
                v23 = 0;
            }
            if (v23)
            {
                if (v5->ia_reinit2)
                {
                    v22 = (tGameObjectMain*)v5->object_a_activer2;
                    if (v22)
                    {
                        if ((unsigned int)v22 <= 2)
                            v22 = AI_MainActor[(uint32_t)v22 - 1];
                        AI_EvalFunc_AIReinitVars_C(v22);
                    }
                    else
                    {
                        AI_EvalFunc_AIReinitVars_C(0);
                    }
                }
                if (!v5->ne_fait_pas_l_activation)
                {
                    v21 = (tGameObjectMain*)v5->object_a_activer2;
                    if (v21)
                    {
                        if ((unsigned int)v21 <= 2)
                            v21 = AI_MainActor[(uint32_t)v21 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v21, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, a2);
                    }
                }
            }
            v20 = (tGameObjectMain*)v5->object_a_activer3;
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
                if (v5->ia_reinit3)
                {
                    v18 = (tGameObjectMain*)v5->object_a_activer3;
                    if (v18)
                    {
                        if ((unsigned int)v18 <= 2)
                            v18 = AI_MainActor[(uint32_t)v18 - 1];
                        AI_EvalFunc_AIReinitVars_C(v18);
                    }
                    else
                    {
                        AI_EvalFunc_AIReinitVars_C(0);
                    }
                }
                if (!v5->ne_fait_pas_l_activation)
                {
                    v17 = (tGameObjectMain*)v5->object_a_activer3;
                    if (v17)
                    {
                        if ((unsigned int)v17 <= 2)
                            v17 = AI_MainActor[(uint32_t)v17 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v17, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, a2);
                    }
                }
            }
            if (v5->rend_invisible_aussi)
            {
                v16 = (tGameObjectMain*)v5->object_a_activer1;
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
                    v14 = (tGameObjectMain*)v5->object_a_activer1;
                    if (v14)
                    {
                        if ((unsigned int)v14 <= 2)
                            v14 = AI_MainActor[(uint32_t)v14 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v14, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, a2);
                    }
                    v13 = (tGameObjectMain*)v5->object_a_activer1;
                    if (v13)
                    {
                        if ((unsigned int)v13 <= 2)
                            v13 = AI_MainActor[(uint32_t)v13 - 1];
                        v3 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v13, 0x800000);
                    }
                    else
                    {
                        v3 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x800000);
                    }
                    if (v3)
                    {
                        v12 = (tGameObjectMain*)v5->object_a_activer1;
                        if (v12)
                        {
                            if ((unsigned int)v12 <= 2)
                                v12 = AI_MainActor[(uint32_t)v12 - 1];
                            AI_EvalFunc_GRPInvisibleSet_C(v12, a2);
                        }
                        else
                        {
                            AI_EvalFunc_GRPInvisibleSet_C(0, a2);
                        }
                    }
                }
                v11 = (tGameObjectMain*)v5->object_a_activer2;
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
                    v9 = (tGameObjectMain*)v5->object_a_activer2;
                    if (v9)
                    {
                        if ((unsigned int)v9 <= 2)
                            v9 = AI_MainActor[(uint32_t)v9 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v9, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, a2);
                    }
                }
                v8 = (tGameObjectMain*)v5->object_a_activer3;
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
                    v6 = (tGameObjectMain*)v5->object_a_activer3;
                    if (v6)
                    {
                        if ((unsigned int)v6 <= 2)
                            v6 = AI_MainActor[(uint32_t)v6 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v6, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, a2);
                    }
                }
            }
            if (!v5->ne_fait_qu_activer)
            {
                v37 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_BV_Active_Desactive_BV_In_BV_Active_Desactive_BV_Out);
                if (v37 != 1000)
                    return v37;
            }
            v5->a_active = 1;
        }
        if (v5->se_desactive_apres)
        {
            v5->a_active = 0;
            AI_EvalFunc_OBJ_FlagInactiveSet_C(AI_gpst_CurrentGameObject, 1);
        }
    }
    return 1000;
}

int BV_Active_Desactive_BV_AD()
{
    tScriptStruct_BVActiveDesactive* v1; // [esp+0h] [ebp-24h]
    tGameObjectMain* v2; // [esp+4h] [ebp-20h]
    tGameObjectMain* v3; // [esp+8h] [ebp-1Ch]
    tGameObjectMain* v4; // [esp+Ch] [ebp-18h]
    tGameObjectMain* v5; // [esp+10h] [ebp-14h]
    int v6; // [esp+18h] [ebp-Ch]
    int v7; // [esp+1Ch] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_BVActiveDesactive*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_BV_Active_Desactive_BV_AD_BV_Active_Desactive_BV_CBits_Out = (int)AI_ul_RealResolveOneRefFunction(0x7100F73Bu);
        fct_BV_Active_Desactive_BV_AD_BV_Active_Desactive_BV_Out = (int)AI_ul_RealResolveOneRefFunction(0x7100F571u);
    }
    v5 = (tGameObjectMain*)v1->o_BV_a_qui;
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
    if (!v4)
        v1->o_BV_a_qui = (int)AI_gpst_CurrentGameObject;
    if (v1->i_detection_type)
    {
        v7 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_BV_Active_Desactive_BV_AD_BV_Active_Desactive_BV_CBits_Out);
        if (v7 != 1000)
            return v7;
    }
    else
    {
        v3 = (tGameObjectMain*)v1->acteur_a_detecter;
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            v2 = v3;
        }
        else
        {
            v2 = 0;
        }
        if (v2)
        {
            v6 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_BV_Active_Desactive_BV_AD_BV_Active_Desactive_BV_Out);
            if (v6 != 1000)
                return v6;
        }
    }
    return 1000;
}

int BV_Active_Desactive_BV_Out()
{
    BOOL v0; // eax
    int v1; // eax
    BOOL v2; // eax
    BOOL v3; // eax
    tScriptStruct_BVActiveDesactive* v5; // [esp+0h] [ebp-D8h]
    tGameObjectMain* v6; // [esp+8h] [ebp-D0h]
    tGameObjectMain* v7; // [esp+Ch] [ebp-CCh]
    tGameObjectMain* v8; // [esp+10h] [ebp-C8h]
    tGameObjectMain* v9; // [esp+18h] [ebp-C0h]
    tGameObjectMain* v10; // [esp+1Ch] [ebp-BCh]
    tGameObjectMain* v11; // [esp+20h] [ebp-B8h]
    tGameObjectMain* v12; // [esp+28h] [ebp-B0h]
    tGameObjectMain* v13; // [esp+30h] [ebp-A8h]
    tGameObjectMain* v14; // [esp+38h] [ebp-A0h]
    tGameObjectMain* v15; // [esp+3Ch] [ebp-9Ch]
    tGameObjectMain* v16; // [esp+40h] [ebp-98h]
    tGameObjectMain* v17; // [esp+48h] [ebp-90h]
    tGameObjectMain* v18; // [esp+50h] [ebp-88h]
    tGameObjectMain* v19; // [esp+54h] [ebp-84h]
    tGameObjectMain* v20; // [esp+58h] [ebp-80h]
    tGameObjectMain* v21; // [esp+60h] [ebp-78h]
    tGameObjectMain* v22; // [esp+68h] [ebp-70h]
    tGameObjectMain* v23; // [esp+6Ch] [ebp-6Ch]
    tGameObjectMain* v24; // [esp+70h] [ebp-68h]
    tGameObjectMain* v25; // [esp+78h] [ebp-60h]
    tGameObjectMain* v26; // [esp+80h] [ebp-58h]
    tGameObjectMain* v27; // [esp+88h] [ebp-50h]
    tGameObjectMain* v28; // [esp+90h] [ebp-48h]
    tGameObjectMain* v29; // [esp+94h] [ebp-44h]
    tGameObjectMain* v30; // [esp+98h] [ebp-40h]
    tGameObjectMain* v31; // [esp+A0h] [ebp-38h]
    tGameObjectMain* v32; // [esp+A4h] [ebp-34h]
    tGameObjectMain* v33; // [esp+A8h] [ebp-30h]
    tGameObjectMain* v34; // [esp+B0h] [ebp-28h]
    tGameObjectMain* v35; // [esp+B4h] [ebp-24h]
    tGameObjectMain* v36; // [esp+B8h] [ebp-20h]
    int v37; // [esp+BCh] [ebp-1Ch]
    BOOL a2; // [esp+D4h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_BVActiveDesactive*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_BV_Active_Desactive_BV_Out_BV_Active_Desactive_BV_In = (int)AI_ul_RealResolveOneRefFunction(0x7100F56Du);
    }
    if (!v5->i_bv_pivot)
    {
        v36 = (tGameObjectMain*)v5->acteur_a_detecter;
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
        v34 = (tGameObjectMain*)v5->o_BV_a_qui;
        if (v34)
        {
            if ((unsigned int)v34 <= 2)
                v34 = AI_MainActor[(uint32_t)v34 - 1];
            v0 = AI_EvalFunc_COLBVObjectObject_C(v34, v35);
        }
        else
        {
            v0 = AI_EvalFunc_COLBVObjectObject_C(0, v35);
        }
        if (v0)
            goto LABEL_151;
    }
    if (!v5->i_bv_pivot)
        goto LABEL_147;
    v33 = (tGameObjectMain*)v5->acteur_a_detecter;
    if (v33)
    {
        if ((unsigned int)v33 <= 2)
            v33 = AI_MainActor[(uint32_t)v33 - 1];
        v32 = v33;
    }
    else
    {
        v32 = 0;
    }
    v31 = (tGameObjectMain*)v5->o_BV_a_qui;
    if (v31)
    {
        if ((unsigned int)v31 <= 2)
            v31 = AI_MainActor[(uint32_t)v31 - 1];
        v1 = AI_EvalFunc_COLBVPivotCollide_C(v31, v32);
    }
    else
    {
        v1 = AI_EvalFunc_COLBVPivotCollide_C(0, v32);
    }
    if (v1)
    {
    LABEL_151:
        if (!v5->a_active)
        {
            a2 = v5->active_dans_bv == 0;
            v30 = (tGameObjectMain*)v5->object_a_activer1;
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
                if (v5->ia_reinit1)
                {
                    v28 = (tGameObjectMain*)v5->object_a_activer1;
                    if (v28)
                    {
                        if ((unsigned int)v28 <= 2)
                            v28 = AI_MainActor[(uint32_t)v28 - 1];
                        AI_EvalFunc_AIReinitVars_C(v28);
                    }
                    else
                    {
                        AI_EvalFunc_AIReinitVars_C(0);
                    }
                }
                if (!v5->ne_fait_pas_l_activation)
                {
                    v27 = (tGameObjectMain*)v5->object_a_activer1;
                    if (v27)
                    {
                        if ((unsigned int)v27 <= 2)
                            v27 = AI_MainActor[(uint32_t)v27 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v27, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, a2);
                    }
                    v26 = (tGameObjectMain*)v5->object_a_activer1;
                    if (v26)
                    {
                        if ((unsigned int)v26 <= 2)
                            v26 = AI_MainActor[(uint32_t)v26 - 1];
                        v2 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v26, 0x800000);
                    }
                    else
                    {
                        v2 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x800000);
                    }
                    if (v2)
                    {
                        v25 = (tGameObjectMain*)v5->object_a_activer1;
                        if (v25)
                        {
                            if ((unsigned int)v25 <= 2)
                                v25 = AI_MainActor[(uint32_t)v25 - 1];
                            AI_EvalFunc_GRPInactiveSet_C(v25, a2);
                        }
                        else
                        {
                            AI_EvalFunc_GRPInactiveSet_C(0, a2);
                        }
                    }
                }
            }
            v24 = (tGameObjectMain*)v5->object_a_activer2;
            if (v24)
            {
                if ((unsigned int)v24 <= 2)
                    v24 = AI_MainActor[(uint32_t)v24 - 1];
                v23 = v24;
            }
            else
            {
                v23 = 0;
            }
            if (v23)
            {
                if (v5->ia_reinit2)
                {
                    v22 = (tGameObjectMain*)v5->object_a_activer2;
                    if (v22)
                    {
                        if ((unsigned int)v22 <= 2)
                            v22 = AI_MainActor[(uint32_t)v22 - 1];
                        AI_EvalFunc_AIReinitVars_C(v22);
                    }
                    else
                    {
                        AI_EvalFunc_AIReinitVars_C(0);
                    }
                }
                if (!v5->ne_fait_pas_l_activation)
                {
                    v21 = (tGameObjectMain*)v5->object_a_activer2;
                    if (v21)
                    {
                        if ((unsigned int)v21 <= 2)
                            v21 = AI_MainActor[(uint32_t)v21 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v21, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, a2);
                    }
                }
            }
            v20 = (tGameObjectMain*)v5->object_a_activer3;
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
                if (v5->ia_reinit3)
                {
                    v18 = (tGameObjectMain*)v5->object_a_activer3;
                    if (v18)
                    {
                        if ((unsigned int)v18 <= 2)
                            v18 = AI_MainActor[(uint32_t)v18 - 1];
                        AI_EvalFunc_AIReinitVars_C(v18);
                    }
                    else
                    {
                        AI_EvalFunc_AIReinitVars_C(0);
                    }
                }
                if (!v5->ne_fait_pas_l_activation)
                {
                    v17 = (tGameObjectMain*)v5->object_a_activer3;
                    if (v17)
                    {
                        if ((unsigned int)v17 <= 2)
                            v17 = AI_MainActor[(uint32_t)v17 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v17, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, a2);
                    }
                }
            }
            if (v5->rend_invisible_aussi)
            {
                v16 = (tGameObjectMain*)v5->object_a_activer1;
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
                    v14 = (tGameObjectMain*)v5->object_a_activer1;
                    if (v14)
                    {
                        if ((unsigned int)v14 <= 2)
                            v14 = AI_MainActor[(uint32_t)v14 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v14, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, a2);
                    }
                    v13 = (tGameObjectMain*)v5->object_a_activer1;
                    if (v13)
                    {
                        if ((unsigned int)v13 <= 2)
                            v13 = AI_MainActor[(uint32_t)v13 - 1];
                        v3 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v13, 0x800000);
                    }
                    else
                    {
                        v3 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x800000);
                    }
                    if (v3)
                    {
                        v12 = (tGameObjectMain*)v5->object_a_activer1;
                        if (v12)
                        {
                            if ((unsigned int)v12 <= 2)
                                v12 = AI_MainActor[(uint32_t)v12 - 1];
                            AI_EvalFunc_GRPInvisibleSet_C(v12, a2);
                        }
                        else
                        {
                            AI_EvalFunc_GRPInvisibleSet_C(0, a2);
                        }
                    }
                }
                v11 = (tGameObjectMain*)v5->object_a_activer2;
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
                    v9 = (tGameObjectMain*)v5->object_a_activer2;
                    if (v9)
                    {
                        if ((unsigned int)v9 <= 2)
                            v9 = AI_MainActor[(uint32_t)v9 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v9, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, a2);
                    }
                }
                v8 = (tGameObjectMain*)v5->object_a_activer3;
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
                    v6 = (tGameObjectMain*)v5->object_a_activer3;
                    if (v6)
                    {
                        if ((unsigned int)v6 <= 2)
                            v6 = AI_MainActor[(uint32_t)v6 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v6, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, a2);
                    }
                }
            }
            if (!v5->ne_fait_qu_activer)
            {
                v37 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_BV_Active_Desactive_BV_Out_BV_Active_Desactive_BV_In);
                if (v37 != 1000)
                    return v37;
            }
            v5->a_active = 1;
        }
        if (v5->se_desactive_apres)
        {
            v5->a_active = 0;
            AI_EvalFunc_OBJ_FlagInactiveSet_C(AI_gpst_CurrentGameObject, 1);
        }
    }
    else
    {
    LABEL_147:
        v5->a_active = 0;
    }
    return 1000;
}

int BV_Active_Desactive_BV_CBits_In()
{
    tGameObjectMain* v0; // eax
    BOOL v1; // eax
    BOOL v2; // eax
    tScriptStruct_BVActiveDesactive* v4; // [esp+0h] [ebp-C4h]
    tGameObjectMain* v5; // [esp+8h] [ebp-BCh]
    tGameObjectMain* v6; // [esp+Ch] [ebp-B8h]
    tGameObjectMain* v7; // [esp+10h] [ebp-B4h]
    tGameObjectMain* v8; // [esp+18h] [ebp-ACh]
    tGameObjectMain* v9; // [esp+1Ch] [ebp-A8h]
    tGameObjectMain* v10; // [esp+20h] [ebp-A4h]
    tGameObjectMain* v11; // [esp+28h] [ebp-9Ch]
    tGameObjectMain* v12; // [esp+30h] [ebp-94h]
    tGameObjectMain* v13; // [esp+38h] [ebp-8Ch]
    tGameObjectMain* v14; // [esp+3Ch] [ebp-88h]
    tGameObjectMain* v15; // [esp+40h] [ebp-84h]
    tGameObjectMain* v16; // [esp+48h] [ebp-7Ch]
    tGameObjectMain* v17; // [esp+50h] [ebp-74h]
    tGameObjectMain* v18; // [esp+54h] [ebp-70h]
    tGameObjectMain* v19; // [esp+58h] [ebp-6Ch]
    tGameObjectMain* v20; // [esp+60h] [ebp-64h]
    tGameObjectMain* v21; // [esp+68h] [ebp-5Ch]
    tGameObjectMain* v22; // [esp+6Ch] [ebp-58h]
    tGameObjectMain* v23; // [esp+70h] [ebp-54h]
    tGameObjectMain* v24; // [esp+78h] [ebp-4Ch]
    tGameObjectMain* v25; // [esp+80h] [ebp-44h]
    tGameObjectMain* v26; // [esp+88h] [ebp-3Ch]
    tGameObjectMain* v27; // [esp+90h] [ebp-34h]
    tGameObjectMain* v28; // [esp+94h] [ebp-30h]
    tGameObjectMain* v29; // [esp+98h] [ebp-2Ch]
    tGameObjectMain* v30; // [esp+9Ch] [ebp-28h]
    tGameObjectMain* v31; // [esp+A0h] [ebp-24h]
    tGameObjectMain* v32; // [esp+A8h] [ebp-1Ch]
    int v33; // [esp+ACh] [ebp-18h]
    BOOL a2; // [esp+C0h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_BVActiveDesactive*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_BV_Active_Desactive_BV_CBits_In_BV_Active_Desactive_BV_CBits_Out = (int)AI_ul_RealResolveOneRefFunction(0x7100F73Bu);
    }
    v32 = (tGameObjectMain*)v4->o_BV_a_qui;
    if (v32)
    {
        if ((unsigned int)v32 <= 2)
            v32 = AI_MainActor[(uint32_t)v32 - 1];
        v0 = AI_EvalFunc_COLBVGetFirstOverlap_C(v32, v4->acteur_avec_CBit, 0, 5);
    }
    else
    {
        v0 = AI_EvalFunc_COLBVGetFirstOverlap_C(0, v4->acteur_avec_CBit, 0, 5);
    }
    v31 = v0;
    if (v0)
    {
        if ((unsigned int)v0 <= 2)
            v31 = AI_MainActor[(uint32_t)v0 - 1];
        v30 = v31;
    }
    else
    {
        v30 = 0;
    }
    if (v30)
    {
        v4->a_active = 0;
    }
    else
    {
        if (!v4->a_active)
        {
            a2 = v4->active_dans_bv != 0;
            v29 = (tGameObjectMain*)v4->object_a_activer1;
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
                if (v4->ia_reinit1)
                {
                    v27 = (tGameObjectMain*)v4->object_a_activer1;
                    if (v27)
                    {
                        if ((unsigned int)v27 <= 2)
                            v27 = AI_MainActor[(uint32_t)v27 - 1];
                        AI_EvalFunc_AIReinitVars_C(v27);
                    }
                    else
                    {
                        AI_EvalFunc_AIReinitVars_C(0);
                    }
                }
                if (!v4->ne_fait_pas_l_activation)
                {
                    v26 = (tGameObjectMain*)v4->object_a_activer1;
                    if (v26)
                    {
                        if ((unsigned int)v26 <= 2)
                            v26 = AI_MainActor[(uint32_t)v26 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v26, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, a2);
                    }
                    v25 = (tGameObjectMain*)v4->object_a_activer1;
                    if (v25)
                    {
                        if ((unsigned int)v25 <= 2)
                            v25 = AI_MainActor[(uint32_t)v25 - 1];
                        v1 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v25, 0x800000);
                    }
                    else
                    {
                        v1 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x800000);
                    }
                    if (v1)
                    {
                        v24 = (tGameObjectMain*)v4->object_a_activer1;
                        if (v24)
                        {
                            if ((unsigned int)v24 <= 2)
                                v24 = AI_MainActor[(uint32_t)v24 - 1];
                            AI_EvalFunc_GRPInactiveSet_C(v24, a2);
                        }
                        else
                        {
                            AI_EvalFunc_GRPInactiveSet_C(0, a2);
                        }
                    }
                }
            }
            v23 = (tGameObjectMain*)v4->object_a_activer2;
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
                if (v4->ia_reinit2)
                {
                    v21 = (tGameObjectMain*)v4->object_a_activer2;
                    if (v21)
                    {
                        if ((unsigned int)v21 <= 2)
                            v21 = AI_MainActor[(uint32_t)v21 - 1];
                        AI_EvalFunc_AIReinitVars_C(v21);
                    }
                    else
                    {
                        AI_EvalFunc_AIReinitVars_C(0);
                    }
                }
                if (!v4->ne_fait_pas_l_activation)
                {
                    v20 = (tGameObjectMain*)v4->object_a_activer2;
                    if (v20)
                    {
                        if ((unsigned int)v20 <= 2)
                            v20 = AI_MainActor[(uint32_t)v20 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v20, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, a2);
                    }
                }
            }
            v19 = (tGameObjectMain*)v4->object_a_activer3;
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
                if (v4->ia_reinit3)
                {
                    v17 = (tGameObjectMain*)v4->object_a_activer3;
                    if (v17)
                    {
                        if ((unsigned int)v17 <= 2)
                            v17 = AI_MainActor[(uint32_t)v17 - 1];
                        AI_EvalFunc_AIReinitVars_C(v17);
                    }
                    else
                    {
                        AI_EvalFunc_AIReinitVars_C(0);
                    }
                }
                if (!v4->ne_fait_pas_l_activation)
                {
                    v16 = (tGameObjectMain*)v4->object_a_activer3;
                    if (v16)
                    {
                        if ((unsigned int)v16 <= 2)
                            v16 = AI_MainActor[(uint32_t)v16 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v16, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, a2);
                    }
                }
            }
            if (v4->rend_invisible_aussi)
            {
                v15 = (tGameObjectMain*)v4->object_a_activer1;
                if (v15)
                {
                    if ((unsigned int)v15 <= 2)
                        v15 = AI_MainActor[(uint32_t)v15 - 1];
                    v14 = v15;
                }
                else
                {
                    v14 = 0;
                }
                if (v14)
                {
                    v13 = (tGameObjectMain*)v4->object_a_activer1;
                    if (v13)
                    {
                        if ((unsigned int)v13 <= 2)
                            v13 = AI_MainActor[(uint32_t)v13 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v13, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, a2);
                    }
                    v12 = (tGameObjectMain*)v4->object_a_activer1;
                    if (v12)
                    {
                        if ((unsigned int)v12 <= 2)
                            v12 = AI_MainActor[(uint32_t)v12 - 1];
                        v2 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v12, 0x800000);
                    }
                    else
                    {
                        v2 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x800000);
                    }
                    if (v2)
                    {
                        v11 = (tGameObjectMain*)v4->object_a_activer1;
                        if (v11)
                        {
                            if ((unsigned int)v11 <= 2)
                                v11 = AI_MainActor[(uint32_t)v11 - 1];
                            AI_EvalFunc_GRPInvisibleSet_C(v11, a2);
                        }
                        else
                        {
                            AI_EvalFunc_GRPInvisibleSet_C(0, a2);
                        }
                    }
                }
                v10 = (tGameObjectMain*)v4->object_a_activer2;
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
                    v8 = (tGameObjectMain*)v4->object_a_activer2;
                    if (v8)
                    {
                        if ((unsigned int)v8 <= 2)
                            v8 = AI_MainActor[(uint32_t)v8 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v8, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, a2);
                    }
                }
                v7 = (tGameObjectMain*)v4->object_a_activer3;
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
                    v5 = (tGameObjectMain*)v4->object_a_activer3;
                    if (v5)
                    {
                        if ((unsigned int)v5 <= 2)
                            v5 = AI_MainActor[(uint32_t)v5 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v5, a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, a2);
                    }
                }
            }
            if (!v4->ne_fait_qu_activer)
            {
                v33 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_BV_Active_Desactive_BV_CBits_In_BV_Active_Desactive_BV_CBits_Out);
                if (v33 != 1000)
                    return v33;
            }
            v4->a_active = 1;
        }
        if (v4->se_desactive_apres)
        {
            v4->a_active = 0;
            AI_EvalFunc_OBJ_FlagInactiveSet_C(AI_gpst_CurrentGameObject, 1);
        }
    }
    return 1000;
}