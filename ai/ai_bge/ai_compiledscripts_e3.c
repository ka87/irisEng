#include "ai_compiledscripts_inc.h"

//e3_init
int main_E3_loadmap_me3_univVars;
int main_E3_loadmap_me3_init_Flag_speciaux_sauve;
int main_E3_loadmap_me3_init_world_ID;
int main_E3_loadmap_me3_init_world_entrance_ID;
int main_E3_loadmap_me3_init_i_force_newgame_reinit;
int main_E3_loadmap_me3_init_i_special_init;
int main_E3_loadmap_me3_INV_MatFade_gao;
int main_E3_loadmap_me3_init_load_world;

//e3_finit
int main_E3_loadmap_me3_finit_INV_MatFade_gao;
int main_E3_loadmap_me3_finit_univVars;
int main_E3_loadmap_me3_finit_i_special_init;
int main_E3_loadmap_me3_finit_i_INV_OptionsFlags;
int main_E3_loadmap_me3_finit_world_ID;
int main_E3_loadmap_me3_finit_load_world;
int fct_main_E3_loadmap_me3_finit_main_E3_loadmap_me3_waitload;
int main_E3_loadmap_me3_finit_Flag_speciaux_sauve;
int fct_main_E3_loadmap_me3_finit_main_E3_loadmap_me3_init;

//main_E3_loadmap_me3_waitload
int main_E3_loadmap_me3_waitload_univVars;
int main_E3_loadmap_me3_waitload_univVars_Flag_speciaux_sauve;
int main_E3_loadmap_me3_waitload_INV_MatFade_gao;
int main_E3_loadmap_me3_waitload_univVars_world_ID;
int main_E3_loadmap_me3_waitload_univVars_load_world;

// --------------------------- E3

int main_E3_loadmap_me3_init()
{
    int result;
    float v1;
    tScript_E3Struct* v2;
    tGameObjectMain* v3;
    tGameObjectMain* v4;
    tGameObjectMain* v5;
    tGameObjectMain* v6;
    tGameObjectMain* v7;
    tGameObjectMain* v8;
    tGameObjectMain* v9;
    tGameObjectMain* v10;
    tGameObjectMain* v11;
    tGameObjectMain* v12;
    tGameObjectMain* v13;
    tGameObjectMain* v14;
    tGameObjectMain* v15;
    double v16;
    float v17;
    Vec3 v18;
    float v19;
    float v20;
    float v21;
    float v22;
    Vec3 v23;
    Vec3 a3;
    Vec3 v25;
    int v26;
    int i;
    int v28;
    Vec3 a5;
    int v30;
    tScript_E3Struct* v31;
    BOOL v32;
    int v33;
    float v34;
    float v35;
    float v36;

    v25.x = 1.0;
    v25.y = 0.0;
    v25.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v2 = (tScript_E3Struct*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v31 = v2;
    v30 = 0;
    v26 = 0;
    i = 0;
    v28 = 0;
    v32 = 0;


    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        main_E3_loadmap_me3_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&main_E3_loadmap_me3_init_Flag_speciaux_sauve, (void*)main_E3_loadmap_me3_univVars, 4488);
        AI2C_ResolveBuffer(&main_E3_loadmap_me3_init_world_ID, (void*)main_E3_loadmap_me3_univVars, 4);
        AI2C_ResolveBuffer(&main_E3_loadmap_me3_init_world_entrance_ID, (void*)main_E3_loadmap_me3_univVars, 8);
        AI2C_ResolveBuffer(&main_E3_loadmap_me3_init_i_force_newgame_reinit, (void*)main_E3_loadmap_me3_univVars, 32);
        AI2C_ResolveBuffer(&main_E3_loadmap_me3_init_i_special_init, (void*)main_E3_loadmap_me3_univVars, 6780);
        main_E3_loadmap_me3_INV_MatFade_gao = AI_ul_RealResolveOneRefObject(0x5E006934u);
        AI2C_ResolveBuffer(&main_E3_loadmap_me3_init_load_world, (void*)main_E3_loadmap_me3_univVars, 0);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)41)
        goto LABEL_107;
    if (*AI_gppst_CurrentJumpNode == (tAINode*)44)
    {
        *(uint32_t*)main_E3_loadmap_me3_init_load_world = 1;
        return 1000;
    }
    i = 0;
    v30 = 0;
    v32 = AI_EvalFunc_ARRBitTest_C((int*)(main_E3_loadmap_me3_init_Flag_speciaux_sauve + 4), 3200);
    if (v32)
    {
        ++v30;
        if (AI_EvalFunc_ARRBitTest_C((int*)(main_E3_loadmap_me3_init_Flag_speciaux_sauve + 4), 3201))
        {
            ++v30;
            if (AI_EvalFunc_ARRBitTest_C((int*)(main_E3_loadmap_me3_init_Flag_speciaux_sauve + 4), 3202))
            {
                ++v30;
                if (AI_EvalFunc_ARRBitTest_C((int*)(main_E3_loadmap_me3_init_Flag_speciaux_sauve + 4), 3203))
                {
                    ++v30;
                    if (AI_EvalFunc_ARRBitTest_C((int*)(main_E3_loadmap_me3_init_Flag_speciaux_sauve + 4), 3204))
                        ++v30;
                }
            }
        }
    }
    v26 = 3;
    v32 = AI_EvalFunc_ARRBitTest_C((int*)(main_E3_loadmap_me3_init_Flag_speciaux_sauve + 4), 3252);
    if (v32)
        v26 = 5;
    if (v30 == v26)
    {
        AI_EvalFunc_ARRBitClear_C((int*)(main_E3_loadmap_me3_init_Flag_speciaux_sauve + 4), 3200);
        AI_EvalFunc_ARRBitClear_C((int*)(main_E3_loadmap_me3_init_Flag_speciaux_sauve + 4), 3201);
        AI_EvalFunc_ARRBitClear_C((int*)(main_E3_loadmap_me3_init_Flag_speciaux_sauve + 4), 3202);
        AI_EvalFunc_ARRBitClear_C((int*)(main_E3_loadmap_me3_init_Flag_speciaux_sauve + 4), 3203);
        AI_EvalFunc_ARRBitClear_C((int*)(main_E3_loadmap_me3_init_Flag_speciaux_sauve + 4), 3204);
        AI_EvalFunc_ARRBitClear_C((int*)(main_E3_loadmap_me3_init_Flag_speciaux_sauve + 4), 3205);
        v30 = 0;
    }
    if (v30 + 3200 > 0)
        AI_EvalFunc_ARRBitSet_C((int*)(main_E3_loadmap_me3_init_Flag_speciaux_sauve + 4), v30 + 3200);
    if (v30)
    {
        switch (v30)
        {
        case 1:
            v32 = AI_EvalFunc_ARRBitTest_C((int*)(main_E3_loadmap_me3_init_Flag_speciaux_sauve + 4), 3251);
            if (v32)
            {
                STR_SetString(AI_gpst_CurrentGameObject, 2, (char*)AI_gpst_CurrentFunction->localStringConstsData + 46);
                *(uint32_t*)main_E3_loadmap_me3_init_world_ID = 63;
                *(uint32_t*)main_E3_loadmap_me3_init_world_entrance_ID = 9;
            }
            else
            {
                STR_SetString(AI_gpst_CurrentGameObject, 2, (char*)AI_gpst_CurrentFunction->localStringConstsData + 22);
                *(uint32_t*)main_E3_loadmap_me3_init_world_ID = 150;
                *(uint32_t*)main_E3_loadmap_me3_init_world_entrance_ID = 10;
            }
            *(uint32_t*)main_E3_loadmap_me3_init_i_force_newgame_reinit = 2;
            *(uint32_t*)main_E3_loadmap_me3_init_i_special_init = 1;
            break;
        case 2:
            v32 = AI_EvalFunc_ARRBitTest_C((int*)(main_E3_loadmap_me3_init_Flag_speciaux_sauve + 4), 3251);
            if (v32)
            {
                STR_SetString(AI_gpst_CurrentGameObject, 2, (char*)AI_gpst_CurrentFunction->localStringConstsData + 100);
                *(uint32_t*)main_E3_loadmap_me3_init_world_ID = 150;
                *(uint32_t*)main_E3_loadmap_me3_init_world_entrance_ID = 10;
            }
            else
            {
                STR_SetString(AI_gpst_CurrentGameObject, 2, (char*)AI_gpst_CurrentFunction->localStringConstsData + 73);
                *(uint32_t*)main_E3_loadmap_me3_init_world_ID = 63;
                *(uint32_t*)main_E3_loadmap_me3_init_world_entrance_ID = 9;
            }
            *(uint32_t*)main_E3_loadmap_me3_init_i_force_newgame_reinit = 2;
            *(uint32_t*)main_E3_loadmap_me3_init_i_special_init = 1;
            break;
        case 3:
            STR_SetString(AI_gpst_CurrentGameObject, 2, (char*)AI_gpst_CurrentFunction->localStringConstsData + 124);
            *(uint32_t*)main_E3_loadmap_me3_init_world_ID = 120;
            *(uint32_t*)main_E3_loadmap_me3_init_world_entrance_ID = 1;
            *(uint32_t*)main_E3_loadmap_me3_init_i_force_newgame_reinit = 2;
            *(uint32_t*)main_E3_loadmap_me3_init_i_special_init = 1;
            break;
        case 4:
            STR_SetString(AI_gpst_CurrentGameObject, 2, (char*)AI_gpst_CurrentFunction->localStringConstsData + 155);
            *(uint32_t*)main_E3_loadmap_me3_init_world_ID = 133;
            *(uint32_t*)main_E3_loadmap_me3_init_world_entrance_ID = 3;
            *(uint32_t*)main_E3_loadmap_me3_init_i_force_newgame_reinit = 2;
            *(uint32_t*)main_E3_loadmap_me3_init_i_special_init = 1;
            break;
        }
    }
    else
    {
        STR_SetString(AI_gpst_CurrentGameObject, 2, (char*)AI_gpst_CurrentFunction->localStringConstsData);
        *(uint32_t*)main_E3_loadmap_me3_init_world_ID = 1002;
        *(uint32_t*)main_E3_loadmap_me3_init_world_entrance_ID = 0;
        *(uint32_t*)main_E3_loadmap_me3_init_i_force_newgame_reinit = 1;
        *(uint32_t*)main_E3_loadmap_me3_init_i_special_init = 1;
    }
    a5.x = -1.0;
    a5.y = 0.0;
    a5.z = 0.0;
    STR_SetSize(AI_gpst_CurrentGameObject, 2, 0, -1, &a5);
    v33 = AI_EvalFunc_AIGetCurSystem_C();
    v17 = (float)(v33 - 1);
    if (v17 < 0.0)
        v1 = 0.0;
    else
        v1 = (float)(v33 - 1);
    v22 = v1;
    v16 = v1 + MATH_gd_magicDoubleForRound;
    v28 = *(uint32_t*)(&v16);
    v15 = (tGameObjectMain*)v31->o_camera;
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
    v25 = a3;
    v14 = (tGameObjectMain*)v31->o_camera;
    if (v14)
    {
        if ((unsigned int)v14 <= 2)
            v14 = AI_MainActor[(uint32_t)v14 - 1];
        AI_EvalFunc_OBJ_SightGet_C(v14, &v18);
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C(0, &v18);
    }
    v34 = v18.x * 3.0f;
    v35 = v18.y * 3.0f;
    v36 = v18.z * 3.0f;
    v25.x = v25.x + v34;
    v25.y = v25.y + v35;
    v25.z = v25.z + v36;
    v13 = (tGameObjectMain*)v31->o_camera;
    if (v13)
    {
        if ((unsigned int)v13 <= 2)
            v13 = AI_MainActor[(uint32_t)v13 - 1];
        AI_EvalFunc_OBJ_BankingGet_C(v13, &v23);
    }
    else
    {
        AI_EvalFunc_OBJ_BankingGet_C(0, &v23);
    }
    v19 = v23.x * 0.2f;
    v20 = v23.y * 0.2f;
    v21 = v23.z * 0.2f;
    v25.x = v25.x + v19;
    v25.y = v25.y + v20;
    v25.z = v25.z + v21;
    v12 = (tGameObjectMain*)v31->o_pad[v28];
    if (v12)
    {
        if ((unsigned int)v12 <= 2)
            v12 = AI_MainActor[(uint32_t)v12 - 1];
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v12, 0);
    }
    else
    {
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
    }
    v11 = (tGameObjectMain*)v31->o_PadButtons[v28];
    if (v11)
    {
        if ((unsigned int)v11 <= 2)
            v11 = AI_MainActor[(uint32_t)v11 - 1];
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v11, 0);
    }
    else
    {
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
    }
    v10 = (tGameObjectMain*)v31->o_pad[v28];
    if (v10)
    {
        if ((unsigned int)v10 <= 2)
            v10 = AI_MainActor[(uint32_t)v10 - 1];
        AI_EvalFunc_OBJSetPos_C(v10, &v25);
    }
    else
    {
        AI_EvalFunc_OBJSetPos_C(0, &v25);
    }
    v9 = (tGameObjectMain*)v31->o_PadButtons[v28];
    if (v9)
    {
        if ((unsigned int)v9 <= 2)
            v9 = AI_MainActor[(uint32_t)v9 - 1];
        AI_EvalFunc_OBJSetPos_C(v9, &v25);
    }
    else
    {
        AI_EvalFunc_OBJSetPos_C(0, &v25);
    }
    for (i = 0; i < 5; ++i)
    {
        v8 = (tGameObjectMain*)v31->o_PadButtons[v28];
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v8 = AI_MainActor[(uint32_t)v8 - 1];
            AI_EvalFunc_OBJ_SetChannelProg_C(v8, i, 0.0);
        }
        else
        {
            AI_EvalFunc_OBJ_SetChannelProg_C(0, i, 0.0);
        }
    }
    v32 = AI_EvalFunc_ARRBitTest_C((int*)(main_E3_loadmap_me3_init_Flag_speciaux_sauve + 4), 3251);
    if (v32)
    {
        v4 = (tGameObjectMain*)v31->o_PadButtons[v28];
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            AI_EvalFunc_OBJ_SetChannelProg_C(v4, v30, 1.0);
            goto LABEL_107;
        }
    }
    else
    {
        if (v30 == 1)
        {
            v7 = (tGameObjectMain*)v31->o_PadButtons[v28];
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                AI_EvalFunc_OBJ_SetChannelProg_C(v7, 2, 1.0);
            }
            else
            {
                AI_EvalFunc_OBJ_SetChannelProg_C(0, 2, 1.0);
            }
            goto LABEL_107;
        }
        if (v30 == 2)
        {
            v6 = (tGameObjectMain*)v31->o_PadButtons[v28];
            if (v6)
            {
                if ((unsigned int)v6 <= 2)
                    v6 = AI_MainActor[(uint32_t)v6 - 1];
                AI_EvalFunc_OBJ_SetChannelProg_C(v6, 1, 1.0);
            }
            else
            {
                AI_EvalFunc_OBJ_SetChannelProg_C(0, 1, 1.0);
            }
            goto LABEL_107;
        }
        v5 = (tGameObjectMain*)v31->o_PadButtons[v28];
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            AI_EvalFunc_OBJ_SetChannelProg_C(v5, v30, 1.0);
            goto LABEL_107;
        }
    }
    AI_EvalFunc_OBJ_SetChannelProg_C(0, v30, 1.0);
LABEL_107:
    if (AI_EvalFunc_IoButtonJustPressed_C(0))
    {
        AI_EvalFunc_SNDPlayModifier_C(AI_gpst_CurrentGameObject, 0);
        SND_Fade(2, 1.0, 0.0, 0.5);
        AI_EvalFunc_GFXDel_C_CURRENT(10);
        i = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(i, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(i, 2, 1);
        v3 = (tGameObjectMain*)main_E3_loadmap_me3_INV_MatFade_gao;
        if (main_E3_loadmap_me3_INV_MatFade_gao)
        {
            if ((unsigned int)main_E3_loadmap_me3_INV_MatFade_gao <= 2)
                v3 = AI_MainActor[main_E3_loadmap_me3_INV_MatFade_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(i, v3, 9);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(i, 0, 9);
        }
        GFXSetCurGlobal(i);
        AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(11101, 0xFF000000);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 0.5);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 4.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
        *AI_gppst_CurrentJumpNode = (tAINode*)44;
        TIME_Wait_C(1.0);
        result = 0;
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)41;
        result = 0;
    }
    return result;
}

int main_E3_loadmap_me3_finit()
{
    tGameObjectMain* v0;
    int result;
    char* v2;
    int v3;
    tScript_E3Struct* v4;
    tGameObjectMain* v5;
    tGameObjectMain* v6;
    tGameObjectMain* v7;
    tGameObjectMain* v8;
    tGameObjectMain* v9;
    tGameObjectMain* v10;
    Vec3 v11;
    Vec3 a5;
    Vec3 a3;
    int v14;
    Vec3 v15;
    Vec3 v16;
    Vec3 v17;
    Vec3 v19;
    int v20;
    int v21;
    tGameObjectMain* v22;
    tScript_E3Struct* v23;
    int a1;
    Vec3 v25;

    if (AI_gpst_CurrentInstance)
        v4 = (tScript_E3Struct*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    v23 = v4;
    a1 = 0;

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        main_E3_loadmap_me3_finit_INV_MatFade_gao = AI_ul_RealResolveOneRefObject(0x5E006934u);
        main_E3_loadmap_me3_finit_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&main_E3_loadmap_me3_finit_i_special_init, (void*)main_E3_loadmap_me3_finit_univVars, 6780);
        AI2C_ResolveBuffer(&main_E3_loadmap_me3_finit_i_INV_OptionsFlags, (void*)main_E3_loadmap_me3_finit_univVars, 7656);
        AI2C_ResolveBuffer(&main_E3_loadmap_me3_finit_world_ID, (void*)main_E3_loadmap_me3_finit_univVars, 4);
        AI2C_ResolveBuffer(&main_E3_loadmap_me3_finit_load_world, (void*)main_E3_loadmap_me3_finit_univVars, 0);
        fct_main_E3_loadmap_me3_finit_main_E3_loadmap_me3_waitload = (int)AI_ul_RealResolveOneRefFunction(0x3D003E06u);
        AI2C_ResolveBuffer(&main_E3_loadmap_me3_finit_Flag_speciaux_sauve, (void*)main_E3_loadmap_me3_finit_univVars, 4488);
        fct_main_E3_loadmap_me3_finit_main_E3_loadmap_me3_init = AI_ul_RealResolveOneRefFunction(0x3D003DF7u);
    }
    switch ((unsigned int)*AI_gppst_CurrentJumpNode)
    {
    case 1u:
        INO_SetControlMode(4);
        INO_SetControlSubmode(8);
        v21 = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 17, 35);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(35, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(35, 2, 1);
        v2 = AI_EvalFunc_TEXTToString_C(&v23->www);
        STR_SetString(AI_gpst_CurrentGameObject, 0, v2);
        AI_EvalFunc_OBJ_TextFlagSet_C(AI_gpst_CurrentGameObject, 0, 21);
        a3.x = 0.5f;
        a3.y = 0.85000002f;
        a3.z = 0.0;
        AI_EvalFunc_OBJ_TextPivotSet_C(AI_gpst_CurrentGameObject, 0, &a3);
        a5.x = -1.0f;
        a5.y = 0.0;
        a5.z = 0.0;
        STR_SetSize(AI_gpst_CurrentGameObject, 0, 0, -1, &a5);
        STR_Align(AI_gpst_CurrentGameObject, 0, 0, -1, 1);
        AI_gpst_CurrentInstance->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -20.0f;
        goto LABEL_26;
    case 4u:
        *(uint32_t*)main_E3_loadmap_me3_finit_i_special_init = 0;
        *(uint32_t*)main_E3_loadmap_me3_finit_i_INV_OptionsFlags |= 0x10000u;
        *(uint32_t*)main_E3_loadmap_me3_finit_world_ID = 1000;
        *(uint32_t*)main_E3_loadmap_me3_finit_load_world = 1;
        *AI_gppst_CurrentJumpNode = (tAINode*)5;
        TIME_Wait_C(50.0);
        result = 0;
        break;
    case 5u:
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
        break;
    case 6u:
    LABEL_26:
        if (AI_EvalKeyword_During_C())
        {
            if (AI_EvalFunc_IoButtonJustPressed_C(0))
            {
                AI_EvalFunc_GFXDel_C_CURRENT(10);
                a1 = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 0, 1);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 2, 1);
                v6 = (tGameObjectMain*)main_E3_loadmap_me3_finit_INV_MatFade_gao;
                if (main_E3_loadmap_me3_finit_INV_MatFade_gao)
                {
                    if ((unsigned int)main_E3_loadmap_me3_finit_INV_MatFade_gao <= 2)
                        v6 = AI_MainActor[main_E3_loadmap_me3_finit_INV_MatFade_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, v6, 9);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, 0, 9);
                }
                GFXSetCurGlobal(a1);
                AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0);
                AI_EvalFunc_GFXSeti_C_CURGLOB(11101, 0xFF000000);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 1.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 12.0f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
                *AI_gppst_CurrentJumpNode = (tAINode*)4;
                TIME_Wait_C(2.0);
                result = 0;
            }
            else
            {
                *AI_gppst_CurrentJumpNode = (tAINode*)6;
                result = 0;
            }
        }
        else
        {
            AI_EvalFunc_GFXDel_C_CURRENT(10);
            a1 = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 2, 1);
            v5 = (tGameObjectMain*)main_E3_loadmap_me3_finit_INV_MatFade_gao;
            if (main_E3_loadmap_me3_finit_INV_MatFade_gao)
            {
                if ((unsigned int)main_E3_loadmap_me3_finit_INV_MatFade_gao <= 2)
                    v5 = AI_MainActor[main_E3_loadmap_me3_finit_INV_MatFade_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, v5, 9);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, 0, 9);
            }
            GFXSetCurGlobal(a1);
            AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(11101, 0xFF000000);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 1.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 12.0f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
            *AI_gppst_CurrentJumpNode = (tAINode*)7;
            TIME_Wait_C(2.0);
            result = 0;
        }
        break;
    case 7u:
        *(uint32_t*)main_E3_loadmap_me3_finit_i_special_init = 0;
        *(uint32_t*)main_E3_loadmap_me3_finit_i_INV_OptionsFlags |= 0x10000u;
        *(uint32_t*)main_E3_loadmap_me3_finit_world_ID = 1000;
        *(uint32_t*)main_E3_loadmap_me3_finit_load_world = 1;
        *AI_gppst_CurrentJumpNode = (tAINode*)8;
        TIME_Wait_C(50.0f);
        result = 0;
        break;
    case 8u:
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
        break;
    case 0xBu:
        AI_EvalFunc_ARRBitSet_C((int*)(main_E3_loadmap_me3_finit_Flag_speciaux_sauve + 4), 3250);
        v14 = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 17, 35);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(35, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(35, 2, 1);
        STR_SetString(AI_gpst_CurrentGameObject, 0, (char*)AI_gpst_CurrentFunction->localStringConstsData);
        AI_EvalFunc_OBJ_TextFlagSet_C(AI_gpst_CurrentGameObject, 0, 21);
        v25.x = 0.5f;
        v25.y = 0.85000002f;
        v25.z = 0.0;
        AI_EvalFunc_OBJ_TextPivotSet_C(AI_gpst_CurrentGameObject, 0, &v25);
        v19.x = -1.0f;
        v19.y = 0.0;
        v19.z = 0.0;
        STR_SetSize(AI_gpst_CurrentGameObject, 0, 0, -1, &v19);
        STR_Align(AI_gpst_CurrentGameObject, 0, 0, -1, 1);
        STR_SetString(AI_gpst_CurrentGameObject, 1, (char*)AI_gpst_CurrentFunction->localStringConstsData + 17);
        AI_EvalFunc_OBJ_TextFlagSet_C(AI_gpst_CurrentGameObject, 1, 21);
        v15.x = 0.5f;
        v15.y = 0.15000001f;
        v15.z = 0.0;
        AI_EvalFunc_OBJ_TextPivotSet_C(AI_gpst_CurrentGameObject, 1, &v15);
        v17.x = -1.0f;
        v17.y = 0.0;
        v17.z = 0.0;
        STR_SetSize(AI_gpst_CurrentGameObject, 1, 0, -1, &v17);
        STR_Align(AI_gpst_CurrentGameObject, 1, 0, -1, 1);
        STR_SetString(AI_gpst_CurrentGameObject, 2, (char*)AI_gpst_CurrentFunction->localStringConstsData + 29);
        AI_EvalFunc_OBJ_TextFlagSet_C(AI_gpst_CurrentGameObject, 2, 21);
        v11.x = 0.5f;
        v11.y = 0.22f;
        v11.z = 0.0;
        AI_EvalFunc_OBJ_TextPivotSet_C(AI_gpst_CurrentGameObject, 2, &v11);
        v16.x = -1.0f;
        v16.y = 0.0;
        v16.z = 0.0;
        STR_SetSize(AI_gpst_CurrentGameObject, 2, 0, -1, &v16);
        STR_Align(AI_gpst_CurrentGameObject, 2, 0, -1, 1);
        v3 = STR_i_EffectAdd(AI_gpst_CurrentGameObject, 2, 0, 0, -1);
        v23->i_effect = v3;
        STR_EffectSeti(AI_gpst_CurrentGameObject, v23->i_effect, 0, 1);
        STR_EffectSeti(AI_gpst_CurrentGameObject, v23->i_effect, 200, -1);
        STR_EffectSeti(AI_gpst_CurrentGameObject, v23->i_effect, 201, 0xFF4F4F4F);
        STR_EffectSetf(AI_gpst_CurrentGameObject, v23->i_effect, 250, 0.5f);
        v20 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_main_E3_loadmap_me3_finit_main_E3_loadmap_me3_init);
        if (v20 == 1000)
            result = 1000;
        else
            result = v20;
        break;
    default:
        AI_EvalFunc_GFXDel_C_CURRENT(10);
        a1 = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 2, 1);
        v10 = (tGameObjectMain*)main_E3_loadmap_me3_finit_INV_MatFade_gao;
        if (main_E3_loadmap_me3_finit_INV_MatFade_gao)
        {
            if ((unsigned int)main_E3_loadmap_me3_finit_INV_MatFade_gao <= 2)
                v10 = AI_MainActor[main_E3_loadmap_me3_finit_INV_MatFade_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, v10, 9);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, 0, 9);
        }
        GFXSetCurGlobal(a1);
        AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0xFF000000);
        AI_EvalFunc_GFXSeti_C_CURGLOB(11101, 0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 1.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 12.0f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
        v9 = (tGameObjectMain*)v23->o_camera;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            v8 = v9;
        }
        else
        {
            v8 = 0;
        }
        if (v8)
        {
            v7 = (tGameObjectMain*)v23->o_camera;
            if (v7)
            {
                if ((unsigned int)v7 <= 2)
                    v7 = AI_MainActor[(uint32_t)v7 - 1];
                v0 = AI_EvalFunc_VIEWAssignObject_C(v7, 0);
            }
            else
            {
                v0 = AI_EvalFunc_VIEWAssignObject_C(0, 0);
            }
            v22 = v0;
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)1;
        TIME_Wait_C(0.0099999998f);
        result = 0;
        break;
    }
    return result;
}

int main_E3_loadmap_me3_waitload()
{
    int result; // eax
    BOOL v1; // eax
    BOOL v2; // eax
    tScript_E3Struct* v3; // [esp+4h] [ebp-88h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-80h]
    tGameObjectMain* v5; // [esp+14h] [ebp-78h]
    tGameObjectMain* v6; // [esp+1Ch] [ebp-70h]
    tGameObjectMain* v7; // [esp+2Ch] [ebp-60h]
    tGameObjectMain* v8; // [esp+38h] [ebp-54h]
    tGameObjectMain* v9; // [esp+40h] [ebp-4Ch]
    int a1; // [esp+70h] [ebp-1Ch]
    int a1a; // [esp+70h] [ebp-1Ch]
    int a1b; // [esp+70h] [ebp-1Ch]
    int a1c; // [esp+70h] [ebp-1Ch]

    if (AI_gpst_CurrentInstance)
        v3 = (tScript_E3Struct*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        main_E3_loadmap_me3_waitload_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &main_E3_loadmap_me3_waitload_univVars_Flag_speciaux_sauve,
            (void*)main_E3_loadmap_me3_waitload_univVars,
            4488);
        main_E3_loadmap_me3_waitload_INV_MatFade_gao = AI_ul_RealResolveOneRefObject(0x5E006934u);
        AI2C_ResolveBuffer(
            &main_E3_loadmap_me3_waitload_univVars_world_ID,
            (void*)main_E3_loadmap_me3_waitload_univVars,
            4);
        AI2C_ResolveBuffer(
            &main_E3_loadmap_me3_waitload_univVars_load_world,
            (void*)main_E3_loadmap_me3_waitload_univVars,
            0);
    }
    switch ((unsigned int)*AI_gppst_CurrentJumpNode)
    {
    case 5u:
        *(uint32_t*)main_E3_loadmap_me3_waitload_univVars_world_ID = 1004;
        *(uint32_t*)main_E3_loadmap_me3_waitload_univVars_load_world = 1;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    case 0xAu:
        *(uint32_t*)main_E3_loadmap_me3_waitload_univVars_world_ID = 1004;
        *(uint32_t*)main_E3_loadmap_me3_waitload_univVars_load_world = 1;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    case 0x12u:
        *(uint32_t*)main_E3_loadmap_me3_waitload_univVars_world_ID = 1004;
        *(uint32_t*)main_E3_loadmap_me3_waitload_univVars_load_world = 1;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    case 0x15u:
        *(uint32_t*)main_E3_loadmap_me3_waitload_univVars_world_ID = 1004;
        *(uint32_t*)main_E3_loadmap_me3_waitload_univVars_load_world = 1;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    default:
        if (!AI_EvalFunc_ARRBitTest_C((int*)(main_E3_loadmap_me3_waitload_univVars_Flag_speciaux_sauve + 4), 3250))
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        if (v3->i_map)
            goto LABEL_68;
        v9 = (tGameObjectMain*)v3->o_capa;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            v1 = AI_EvalFunc_OBJ_CapaTest_C(v9, 256);
        }
        else
        {
            v1 = AI_EvalFunc_OBJ_CapaTest_C(0, 256);
        }
        if (v1)
            v3->time = TIM_gf_dt + v3->time;
        if (v3->time < 29.0)
        {
            if (v3->i_map != 1)
                goto LABEL_41;
        LABEL_68:
            if (AI_EvalFunc_ARRBitTest_C((int*)(main_E3_loadmap_me3_waitload_univVars_Flag_speciaux_sauve + 4), 3251))
            {
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                return 1000;
            }
            v3->time = TIM_gf_dt + v3->time;
            if (v3->time > 10.0)
            {
                SND_Fade(1, 1.0, 0.0, 2.0);
                SND_Fade(2, 1.0, 0.0, 2.0);
                SND_Fade(3, 1.0, 0.0, 2.0);
                SND_Fade(4, 1.0, 0.0, 2.0);
                AI_EvalFunc_GFXDel_C_CURRENT(10);
                a1a = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(a1a, 0, 1);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(a1a, 2, 1);
                v7 = (tGameObjectMain*)main_E3_loadmap_me3_waitload_INV_MatFade_gao;
                if (main_E3_loadmap_me3_waitload_INV_MatFade_gao)
                {
                    if ((unsigned int)main_E3_loadmap_me3_waitload_INV_MatFade_gao <= 2)
                        v7 = AI_MainActor[main_E3_loadmap_me3_waitload_INV_MatFade_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1a, v7, 9);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1a, 0, 9);
                }
                GFXSetCurGlobal(a1a);
                AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0);
                AI_EvalFunc_GFXSeti_C_CURGLOB(11101, -16777216);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 2.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 4.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
                *AI_gppst_CurrentJumpNode = (tAINode*)10;
                TIME_Wait_C(3.0);
                result = 0;
            }
            else
            {
                if (v3->i_map != 2)
                    goto LABEL_57;
            LABEL_41:
                if (AI_EvalFunc_ARRBitTest_C((int*)(main_E3_loadmap_me3_waitload_univVars_Flag_speciaux_sauve + 4), 3251))
                {
                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                    return 1000;
                }
                if (AI_EvalFunc_ARRBitTest_C((int*)(main_E3_loadmap_me3_waitload_univVars_Flag_speciaux_sauve + 4), 2901))
                    v3->time = TIM_gf_dt + v3->time;
                if (v3->time > 4.0)
                {
                    SND_Fade(1, 1.0, 0.0, 2.0);
                    SND_Fade(2, 1.0, 0.0, 2.0);
                    SND_Fade(3, 1.0, 0.0, 2.0);
                    SND_Fade(4, 1.0, 0.0, 2.0);
                    AI_EvalFunc_GFXDel_C_CURRENT(10);
                    a1b = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
                    AI_EvalFunc_GFXFlagSet_C_CURRENT(a1b, 0, 1);
                    AI_EvalFunc_GFXFlagSet_C_CURRENT(a1b, 2, 1);
                    v6 = (tGameObjectMain*)main_E3_loadmap_me3_waitload_INV_MatFade_gao;
                    if (main_E3_loadmap_me3_waitload_INV_MatFade_gao)
                    {
                        if ((unsigned int)main_E3_loadmap_me3_waitload_INV_MatFade_gao <= 2)
                            v6 = AI_MainActor[main_E3_loadmap_me3_waitload_INV_MatFade_gao - 1];
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1b, v6, 9);
                    }
                    else
                    {
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1b, 0, 9);
                    }
                    GFXSetCurGlobal(a1b);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(11101, -16777216);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 2.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 4.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
                    *AI_gppst_CurrentJumpNode = (tAINode*)18;
                    TIME_Wait_C(3.0);
                    result = 0;
                }
                else
                {
                    if (v3->i_map != 4)
                        goto LABEL_65;
                LABEL_57:
                    v5 = (tGameObjectMain*)v3->o_capa;
                    if (v5)
                    {
                        if ((unsigned int)v5 <= 2)
                            v5 = AI_MainActor[(uint32_t)v5 - 1];
                        v2 = AI_EvalFunc_OBJ_CapaTest_C(v5, 256);
                    }
                    else
                    {
                        v2 = AI_EvalFunc_OBJ_CapaTest_C(0, 256);
                    }
                    if (v2)
                        v3->time = TIM_gf_dt + v3->time;
                    if (v3->time >= 40.0)
                    {
                        SND_Fade(1, 1.0, 0.0, 2.0);
                        SND_Fade(2, 1.0, 0.0, 2.0);
                        SND_Fade(3, 1.0, 0.0, 2.0);
                        SND_Fade(4, 1.0, 0.0, 2.0);
                        AI_EvalFunc_GFXDel_C_CURRENT(10);
                        a1c = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
                        AI_EvalFunc_GFXFlagSet_C_CURRENT(a1c, 0, 1);
                        AI_EvalFunc_GFXFlagSet_C_CURRENT(a1c, 2, 1);
                        v4 = (tGameObjectMain*)main_E3_loadmap_me3_waitload_INV_MatFade_gao;
                        if (main_E3_loadmap_me3_waitload_INV_MatFade_gao)
                        {
                            if ((unsigned int)main_E3_loadmap_me3_waitload_INV_MatFade_gao <= 2)
                                v4 = AI_MainActor[main_E3_loadmap_me3_waitload_INV_MatFade_gao - 1];
                            AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1c, v4, 9);
                        }
                        else
                        {
                            AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1c, 0, 9);
                        }
                        GFXSetCurGlobal(a1c);
                        AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0);
                        AI_EvalFunc_GFXSeti_C_CURGLOB(11101, -16777216);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 2.0);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 4.0);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.0);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
                        *AI_gppst_CurrentJumpNode = (tAINode*)21;
                        TIME_Wait_C(3.0);
                        result = 0;
                    }
                    else
                    {
                    LABEL_65:
                        result = 1000;
                    }
                }
            }
        }
        else
        {
            SND_Fade(1, 1.0, 0.0, 2.0);
            SND_Fade(2, 1.0, 0.0, 2.0);
            SND_Fade(3, 1.0, 0.0, 2.0);
            SND_Fade(4, 1.0, 0.0, 2.0);
            AI_EvalFunc_GFXDel_C_CURRENT(10);
            a1 = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 2, 1);
            v8 = (tGameObjectMain*)main_E3_loadmap_me3_waitload_INV_MatFade_gao;
            if (main_E3_loadmap_me3_waitload_INV_MatFade_gao)
            {
                if ((unsigned int)main_E3_loadmap_me3_waitload_INV_MatFade_gao <= 2)
                    v8 = AI_MainActor[main_E3_loadmap_me3_waitload_INV_MatFade_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, v8, 9);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, 0, 9);
            }
            GFXSetCurGlobal(a1);
            AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(11101, 0xFF000000);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 2.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 4.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
            *AI_gppst_CurrentJumpNode = (tAINode*)5;
            TIME_Wait_C(3.0);
            result = 0;
        }
        return result;
    }
}