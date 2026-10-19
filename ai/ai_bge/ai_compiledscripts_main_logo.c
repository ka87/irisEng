#include "ai_compiledscripts_inc.h"

// hack
int obj_INV_Manager_gao;
int obj_INV_reference_WP_gao;
int obj_INV_MatFade_gao;
int univVars2;
int i_INV_OptionsFlags;
int fct_main_logo_ml_init1;

// trailer
int main_logo_ml_trailer_univVars;
int main_logo_ml_trailer_world_ID;
int main_logo_ml_trailer_load_world;
int main_logo_ml_trailer_univVars_INV_MatFade_gao;

// init2
void* main_logo_ml_init2_INV_Manager_gao;
void* main_logo_ml_init2_univVars;
int main_logo_ml_init2_i_indice_de_map;
int main_logo_ml_init2_INV_MatFade_gao;
int fct_main_logo_ml_init2_main_logo_ml_load;
int main_logo_ml_init2_INV_Manager_gao_i_OpeningMode;
int main_logo_ml_init2_INV_Manager_gao_i_AskExit;
int main_logo_ml_init2_i_Exit;
int main_logo_ml_init2_i_show_credits;
int main_logo_ml_init2_i_INV_OptionsFlags;
int main_logo_ml_init2_world_ID;
int main_logo_ml_init2_load_world;

//init1
int fct_main_logo_ml_init1_main_logo_ml_init2;

//main_logo_ml_load
int main_logo_ml_load_univVars;
int main_logo_ml_load_univVars_i_special_init;
int main_logo_ml_load_univVars_i_INV_OptionsFlags;
int main_logo_ml_load_univVars_load_world;
int main_logo_ml_load_univVars_i_force_newgame_reinit;

// ===========================

//main_fix_mf_init
uint32_t world_ID;
uint32_t univVars;
uint32_t load_world;

int main_logo_ml_load()
{
    int result; // eax
    int v1; // [esp+4h] [ebp-Ch]
    int v2; // [esp+Ch] [ebp-4h]

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        main_logo_ml_load_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&main_logo_ml_load_univVars_i_special_init, (void*)main_logo_ml_load_univVars, 6780);
        AI2C_ResolveBuffer(&main_logo_ml_load_univVars_i_INV_OptionsFlags, (void*)main_logo_ml_load_univVars, 7656);
        AI2C_ResolveBuffer(&main_logo_ml_load_univVars_load_world, (void*)main_logo_ml_load_univVars, 0);
        AI2C_ResolveBuffer(&main_logo_ml_load_univVars_i_force_newgame_reinit, (void*)main_logo_ml_load_univVars, 32);
    }
    *(uint32_t*)main_logo_ml_load_univVars_i_special_init = 0;
    SAV_PhotoInit();
    *(uint32_t*)main_logo_ml_load_univVars_i_INV_OptionsFlags |= 0x10000u;
    *(uint32_t*)main_logo_ml_load_univVars_load_world = 1;
    *(uint32_t*)main_logo_ml_load_univVars_i_force_newgame_reinit = 1;
    v2 = *(uint32_t*)main_logo_ml_load_univVars_i_INV_OptionsFlags;
    AI_ReinitUniverse();
    *(uint32_t*)main_logo_ml_load_univVars_i_INV_OptionsFlags = v2;
    v1 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
    if (v1 == 1000)
        result = 1000;
    else
        result = v1;
    return result;
}

int main_logo_ml_init()
{
    tGameObjectMain* viewGaoObj;
    int gfxIdx;
    int result;
    tScript_mlStruct* instanceVars;
    tGameObjectMain* o_CurEvent;
    tGameObjectMain* texte;
    tGameObjectMain* o_MenuSel;
    tGameObjectMain* o_LangueSel;
    tGameObjectMain* o_LangueTitle2;
    tGameObjectMain* o_LangueTitle;
    tGameObjectMain* o_EventCamera;
    tGameObjectMain* sound;
    tTextGet Src;
    tGameObjectMain* vviewGaoObj;
    int ggfxIdx;
    int ioGameLang;
    int i;

    if (AI_gpst_CurrentInstance)
        instanceVars = (tScript_mlStruct*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        instanceVars = 0;
    ioGameLang = 0;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        obj_INV_Manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4);
        obj_INV_reference_WP_gao = AI_ul_RealResolveOneRefObject(0x620013B8);
        obj_INV_MatFade_gao = AI_ul_RealResolveOneRefObject(0x5E006934);
        univVars2 = AI_ul_RealResolveOneRefObject(0x71003FF9);
        AI2C_ResolveBuffer(&i_INV_OptionsFlags, (void*)univVars2, 7656);
        fct_main_logo_ml_init1 = AI_ul_RealResolveOneRefFunction(0x3D003A94);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)12)
    {
        if ((*(uint32_t*)i_INV_OptionsFlags & 0x10000) != 0)
        {
            instanceVars->o_CurEvent = instanceVars->o_End2Start;
            instanceVars->i_CurrentState = 11;
        }
        else
        {
            instanceVars->o_CurEvent = instanceVars->o_Logo2Langue;
            // test, main logo
            //instanceVars->o_CurEvent = instanceVars->o_Langue2Start;
            // test, old intro
            //instanceVars->o_CurEvent = instanceVars->o_Logo2Start;
        }
        OutputDebugStringA("ITTTTTTT\n");
        o_CurEvent = (tGameObjectMain*)instanceVars->o_CurEvent;
        if (o_CurEvent)
        {
            if ((unsigned int)o_CurEvent <= 2)
                o_CurEvent = AI_MainActor[(uint32_t)o_CurEvent - 1];
            AI_EvalFunc_OBJ_FlagInactiveSet_C(o_CurEvent, 0);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
        }
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_main_logo_ml_init1);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        sound = (tGameObjectMain*)instanceVars->sound;
        if (sound)
        {
            if ((unsigned int)sound <= 2)
                sound = AI_MainActor[(uint32_t)sound - 1];
            AI_EvalFunc_SNDPlayModifier_C(sound, 0);
        }
        else
        {
            AI_EvalFunc_SNDPlayModifier_C(0, 0);
        }


        for (i = 0; i < 20; ++i)
            AI_EvalFunc_Gsp_AE_MASTER_C(i, 0, 0, 0.0);
        AI_EvalFunc_Gsp_AE_MASTER_C(8, 0, 0, 0.5f);
        AI_EvalFunc_Gsp_AE_MASTER_C(9, 0, 0, 0.5f);


        AI_EvalFunc_VIEWFocaleSet_C(0, 1.2f);
        o_EventCamera = (tGameObjectMain*)instanceVars->o_EventCamera;
        if (o_EventCamera)
        {
            if ((unsigned int)o_EventCamera <= 2)
                o_EventCamera = AI_MainActor[(uint32_t)o_EventCamera - 1];
            viewGaoObj = AI_EvalFunc_VIEWAssignObject_C(o_EventCamera, 0);
        }
        else
        {
            viewGaoObj = AI_EvalFunc_VIEWAssignObject_C(0, 0);
        }
        vviewGaoObj = viewGaoObj;
        instanceVars->i_CurrentState = -1;
        o_LangueTitle = (tGameObjectMain*)instanceVars->o_LangueTitle;
        if (o_LangueTitle)
        {
            if ((unsigned int)o_LangueTitle <= 2)
                o_LangueTitle = AI_MainActor[(uint32_t)o_LangueTitle - 1];
            AI_EvalFunc_OBJGetPos_C(o_LangueTitle, &instanceVars->v_LangueTitle_Pos);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &instanceVars->v_LangueTitle_Pos);
        }
        o_LangueTitle2 = (tGameObjectMain*)instanceVars->o_LangueTitle;
        if (o_LangueTitle2)
        {
            if ((unsigned int)o_LangueTitle2 <= 2)
                o_LangueTitle2 = AI_MainActor[(uint32_t)o_LangueTitle2 - 1];
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(o_LangueTitle2, 1);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
        }
        o_LangueSel = (tGameObjectMain*)instanceVars->o_LangueSel;
        if (o_LangueSel)
        {
            if ((unsigned int)o_LangueSel <= 2)
                o_LangueSel = AI_MainActor[(uint32_t)o_LangueSel - 1];
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(o_LangueSel, 1);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
        }
        o_MenuSel = (tGameObjectMain*)instanceVars->o_MenuSel;
        if (o_MenuSel)
        {
            if ((unsigned int)o_MenuSel <= 2)
                o_MenuSel = AI_MainActor[(uint32_t)o_MenuSel - 1];
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(o_MenuSel, 1);
        }
        else
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
        }
        instanceVars->f_Time = 1.0;
        instanceVars->f_Chrono = TIM_gf_MainClock - TIM_gf_LockTime;
        instanceVars->i_ModePause = 0;
        instanceVars->i_PauseState = 0;
        AI_EvalFunc_OBJ_FlagInactiveSet_C((tGameObjectMain*)obj_INV_Manager_gao, 1);
        AI_EvalFunc_GRPInvisibleSet_C((tGameObjectMain*)obj_INV_reference_WP_gao, 1);
        AI_EvalFunc_TEXTGet_C(0x49023E66, 0, &Src);
        memcpy(&instanceVars->txt_MemCard, &Src, sizeof(instanceVars->txt_MemCard));
        instanceVars->MC_slot = 0;
        AI_EvalFunc_IoGetProtect_C(0);
        texte = (tGameObjectMain*)instanceVars->texte;
        if (texte)
        {
            if ((unsigned int)texte <= 2)
                texte = AI_MainActor[(uint32_t)texte - 1];
            gfxIdx = AI_EvalFunc_GFXAddSorted_C(texte, 17, 55);
        }
        else
        {
            gfxIdx = AI_EvalFunc_GFXAddSorted_C(0, 17, 55);
        }
        ggfxIdx = gfxIdx;
        AI_EvalFunc_GFXFlagSet_C_CURRENT(55, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(55, 2, 1);
        ioGameLang = AI_EvalFunc_IoGameLangGet_C();
        instanceVars->i_langnb = 0;
        if ((ioGameLang & 2) != 0)
        {
            instanceVars->i_langval[instanceVars->i_langnb++] = 1;
            ioGameLang &= ~2u;
        }
        i = 0;
        while (ioGameLang)
        {
            if ((ioGameLang & 1) != 0)
                instanceVars->i_langval[instanceVars->i_langnb++] = i;
            ++i;
            ioGameLang >>= 1;
        }
        ioGameLang = AI_EvalFunc_IoConsLangGet_C();
        instanceVars->i_langcur = 0;
        for (i = 0; i < instanceVars->i_langnb; ++i)
        {
            if (instanceVars->i_langval[i] == ioGameLang)
                instanceVars->i_langcur = i;
        }
        AI_EvalFunc_GFXDel_C_CURRENT(10);

        i = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);// fade
        AI_EvalFunc_GFXFlagSet_C_CURRENT(i, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(i, 2, 1);
        if (obj_INV_MatFade_gao)
        {
            if ((unsigned int)obj_INV_MatFade_gao <= 2)
                obj_INV_MatFade_gao = (int)AI_MainActor[(unsigned int)obj_INV_MatFade_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(i, (tGameObjectMain*)obj_INV_MatFade_gao, 9);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(i, 0, 9);
        }
        GFXSetCurGlobal(i);
        AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0xFF000000);
        AI_EvalFunc_GFXSeti_C_CURGLOB(11101, 0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 1.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);

        *AI_gppst_CurrentJumpNode = (tAINode*)12;
        TIME_Wait_C(0.000099999997f);
        result = 0;
    }
    return result;
}

int main_logo_ml_trailer()
{
    int result;
    tScript_mlStruct* v1;
    tGameObjectMain* v2;
    tGameObjectMain* v3;
    tGameObjectMain* v4;
    tGameObjectMain* v5;
    tTextGet Src;
    int v9;
    tScript_mlStruct* v10;
    int v11;
    tTextGet v12;

    if (AI_gpst_CurrentInstance)
        v1 = (tScript_mlStruct*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v10 = v1;
    v11 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        main_logo_ml_trailer_univVars_INV_MatFade_gao = AI_ul_RealResolveOneRefObject(0x5E006934u);
        main_logo_ml_trailer_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9);
        AI2C_ResolveBuffer(&main_logo_ml_trailer_world_ID, (void*)main_logo_ml_trailer_univVars, 4); //dword_994594
        AI2C_ResolveBuffer(&main_logo_ml_trailer_load_world, (void*)main_logo_ml_trailer_univVars, 0); //dword_99458C
    }
    switch ((unsigned int)*AI_gppst_CurrentJumpNode)
    {
    case 1u:
        *(uint32_t*)main_logo_ml_trailer_world_ID = 10;
        *(uint32_t*)main_logo_ml_trailer_load_world = 1;
        *AI_gppst_CurrentJumpNode = (tAINode*)2;
        TIME_Wait_C(50.0f);
        result = 0;
        break;
    case 2u:
        v9 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v9 == 1000)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            result = v9;
        }
        break;
    case 4u:
        v10->i_PauseState = 1;
        if (v10->i_PauseState == 1)
        {
            AI_EvalFunc_TEXTGet_C(0x4900FDC5, 0, &Src);
            memcpy(&v12, &Src, sizeof(v12));
            v4 = (tGameObjectMain*)v10->texte;
            if (v4)
            {
                if ((unsigned int)v4 <= 2)
                    v4 = AI_MainActor[(uint32_t)v4 - 1];
                STR_Reset(v4);
            }
            else
            {
                STR_Reset(0);
            }
            AI_EvalFunc_DbgTraceInt_C();
            v10->i_PauseState = 2;
        }
        if (v10->i_PauseState != 2)
            goto LABEL_47;
        if (!AI_EvalFunc_IoButtonJustPressed_C(0) && !AI_EvalFunc_IoButtonJustPressed_C(9) && !AI_EvalFunc_WORLoaded_C())
            goto LABEL_47;
        if (AI_EvalFunc_WORLoaded_C())
            AI_EvalFunc_DbgTraceInt_C();
        AI_EvalFunc_GFXDel_C_CURRENT(10);
        v11 = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v11, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v11, 2, 1);
        v3 = (tGameObjectMain*)main_logo_ml_trailer_univVars_INV_MatFade_gao;
        if (main_logo_ml_trailer_univVars_INV_MatFade_gao)
        {
            if ((unsigned int)main_logo_ml_trailer_univVars_INV_MatFade_gao <= 2)
                v3 = AI_MainActor[(uint32_t)main_logo_ml_trailer_univVars_INV_MatFade_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v11, v3, 9);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v11, 0, 9);
        }
        GFXSetCurGlobal(v11);
        AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(11101, 0xFF000000);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 1.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.5);
        AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
        *AI_gppst_CurrentJumpNode = (tAINode*)11;
        TIME_Wait_C(1.0);
        result = 0;
        break;
    case 0xBu:
        AI_EvalFunc_DbgTraceInt_C();
        v2 = (tGameObjectMain*)v10->sound;
        if (v2)
        {
            if ((unsigned int)v2 <= 2)
                v2 = AI_MainActor[(uint32_t)v2 - 1];
            AI_EvalFunc_SNDPlayModifier_C(v2, 0);
        }
        else
        {
            AI_EvalFunc_SNDPlayModifier_C(0, 0);
        }
        v10->f_Chrono = TIM_gf_MainClock - TIM_gf_LockTime;
        v10->i_CurrentState = 0;
        if (v10->i_ModePause == 4 || v10->i_ModePause == 5)
            v10->i_CurrentState = 402;
        v10->i_PauseState = 0;
        v10->i_ModePause = 0;
    LABEL_47:
        result = 1000;
        break;
    default:
        if (v10->i_ModePause)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(10);
            v11 = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v11, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v11, 2, 1);
            v5 = (tGameObjectMain*)main_logo_ml_trailer_univVars_INV_MatFade_gao;
            if (main_logo_ml_trailer_univVars_INV_MatFade_gao)
            {
                if ((unsigned int)main_logo_ml_trailer_univVars_INV_MatFade_gao <= 2)
                    v5 = AI_MainActor[(uint32_t)main_logo_ml_trailer_univVars_INV_MatFade_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v11, v5, 9);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v11, 0, 9);
            }
            GFXSetCurGlobal(v11);
            AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(11101, 0xFF000000);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 1.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 12.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
            *AI_gppst_CurrentJumpNode = (tAINode*)1;
            TIME_Wait_C(2.0f);
            result = 0;
        }
        else
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        break;
    }
    return result;
}

// actual menu
int main_logo_ml_init2()
{
    int result;
    char* v1;
    char* v2;
    char* v3;
    char* v4;
    int v5;
    char* v6;
    int v7;
    char* v8;
    int v9;
    tGameObjectMain* v10;
    char* v11;
    char* v12;
    char* v13;
    char* v14;
    int v15;
    char* v16;
    int v17;
    char* v18;
    int v19;
    float v20;
    float v21;
    float v22;
    float v23;
    float v24;
    float v25;
    float v26;
    float v27;
    float v28;
    float v29;
    float v30;
    float v31;
    float v32;
    float v33;
    float v34;
    float v35;
    float v36;
    float v37;
    float v38;
    float v39;
    float v40;
    float v41;
    float v42;
    float v43;
    float v44;
    float v45;
    float v46;
    float v47;
    float v48;
    float v49;
    float v50;
    float v51;
    float v52;
    float v53;
    float v54;
    float v55;
    float v56;
    float v57;
    float v58;
    float v59;
    float v60;
    float v61;
    float v62;
    float v63;
    float v64;
    float v65;
    float v66;
    float v67;
    float v68;
    float v69;
    float v70;
    tScript_mlStruct* v71;
    tGameObjectMain* v72;
    tGameObjectMain* v73;
    tGameObjectMain* v74;
    tGameObjectMain* v75;
    tGameObjectMain* v76;
    tGameObjectMain* v77;
    tGameObjectMain* v78;
    tGameObjectMain* v79;
    tGameObjectMain* v80;
    tGameObjectMain* v81;
    tGameObjectMain* v82;
    tGameObjectMain* v83;
    tGameObjectMain* v84;
    tGameObjectMain* v85;
    tGameObjectMain* v86;
    float v87;
    tGameObjectMain* v88;
    float v89;
    tGameObjectMain* v90;
    float v91;
    tGameObjectMain* v92;
    float v93;
    tGameObjectMain* v94;
    tGameObjectMain* v95;
    tGameObjectMain* v96;
    tGameObjectMain* v97;
    tGameObjectMain* v98;
    tGameObjectMain* v99;
    tGameObjectMain* v100;
    tGameObjectMain* v101;
    tGameObjectMain* v102;
    tGameObjectMain* v103;
    tGameObjectMain* v104;
    tGameObjectMain* v105;
    tGameObjectMain* v106;
    tGameObjectMain* v107;
    tGameObjectMain* v108;
    tGameObjectMain* v109;
    tGameObjectMain* v110;
    tGameObjectMain* v111;
    tGameObjectMain* v112;
    tGameObjectMain* v113;
    tGameObjectMain* v114;
    tGameObjectMain* v115;
    tGameObjectMain* v116;
    tGameObjectMain* v117;
    tGameObjectMain* v118;
    tGameObjectMain* v119;
    tGameObjectMain* v120;
    tGameObjectMain* v121;
    tGameObjectMain* v122;
    tGameObjectMain* v123;
    tGameObjectMain* v124;
    tGameObjectMain* v125;
    tGameObjectMain* v126;
    tGameObjectMain* v127;
    tGameObjectMain* v128;
    tGameObjectMain* v129;
    tGameObjectMain* v130;
    tGameObjectMain* v131;
    tGameObjectMain* v132;
    tGameObjectMain* v133;
    tGameObjectMain* v134;
    tGameObjectMain* v135;
    tGameObjectMain* v136;
    tGameObjectMain* v137;
    tGameObjectMain* v138;
    float v139;
    tGameObjectMain* v140;
    float v141;
    tGameObjectMain* v142;
    float v143;
    tGameObjectMain* v144;
    float v145;
    tGameObjectMain* v146;
    tGameObjectMain* v147;
    tGameObjectMain* v148;
    tGameObjectMain* v149;
    tGameObjectMain* v150;
    tGameObjectMain* v151;
    tGameObjectMain* v152;
    tGameObjectMain* v153;
    tGameObjectMain* v154;
    tGameObjectMain* v155;
    tGameObjectMain* v156;
    tGameObjectMain* v157;
    tGameObjectMain* v158;
    tGameObjectMain* v159;
    tGameObjectMain* v160;
    tGameObjectMain* v161;
    tGameObjectMain* v162;
    tGameObjectMain* v163;
    tGameObjectMain* v164;
    tGameObjectMain* v165;
    tGameObjectMain* v166;
    tGameObjectMain* v167;
    tGameObjectMain* v168;
    tGameObjectMain* v169;
    tGameObjectMain* v170;
    tGameObjectMain* v171;
    float v172;
    tGameObjectMain* v173;
    float v174;
    tGameObjectMain* v175;
    float v176;
    tGameObjectMain* v177;
    float v178;
    tGameObjectMain* v179;
    tGameObjectMain* v180;
    tGameObjectMain* v181;
    tGameObjectMain* v182;
    tGameObjectMain* v183;
    tGameObjectMain* v184;
    tGameObjectMain* v185;
    tGameObjectMain* v186;
    tGameObjectMain* v187;
    tGameObjectMain* v188;
    tGameObjectMain* v189;
    tGameObjectMain* v190;
    tGameObjectMain* v191;
    tGameObjectMain* v192;
    tGameObjectMain* v193;
    tGameObjectMain* v194;
    tGameObjectMain* v195;
    tGameObjectMain* v196;
    tGameObjectMain* v197;
    tGameObjectMain* v198;
    tGameObjectMain* v199;
    tGameObjectMain* v200;
    tGameObjectMain* v201;
    tGameObjectMain* v202;
    tGameObjectMain* v203;
    tGameObjectMain* v204;
    tGameObjectMain* v205;
    tGameObjectMain* v206;
    tGameObjectMain* v207;
    tGameObjectMain* v208;
    tGameObjectMain* v209;
    tGameObjectMain* v210;
    tGameObjectMain* v211;
    tGameObjectMain* v212;
    tGameObjectMain* v213;
    tGameObjectMain* v214;
    tGameObjectMain* v215;
    tGameObjectMain* v216;
    tGameObjectMain* v217;
    tGameObjectMain* v218;
    tGameObjectMain* v219;
    tGameObjectMain* v220;
    tGameObjectMain* v221;
    tGameObjectMain* v222;
    tGameObjectMain* v223;
    tGameObjectMain* v224;
    tGameObjectMain* v225;
    tGameObjectMain* v226;
    tGameObjectMain* v227;
    tGameObjectMain* v228;
    float v229;
    tGameObjectMain* v230;
    float v231;
    tGameObjectMain* v232;
    float v233;
    tGameObjectMain* v234;
    float v235;
    tGameObjectMain* v236;
    tGameObjectMain* v237;
    tGameObjectMain* v238;
    tGameObjectMain* v239;
    tGameObjectMain* v240;
    tGameObjectMain* v241;
    tGameObjectMain* v242;
    tGameObjectMain* v243;
    tGameObjectMain* v244;
    tGameObjectMain* v245;
    tGameObjectMain* v246;
    tGameObjectMain* v247;
    tGameObjectMain* v248;
    tGameObjectMain* v249;
    tGameObjectMain* v250;
    tGameObjectMain* v251;
    tGameObjectMain* v252;
    tGameObjectMain* v253;
    tGameObjectMain* v254;
    tGameObjectMain* v255;
    tGameObjectMain* v256;
    tGameObjectMain* v257;
    tGameObjectMain* v258;
    tGameObjectMain* v259;
    tGameObjectMain* v260;
    tGameObjectMain* v261;
    tGameObjectMain* v262;
    tGameObjectMain* v263;
    tGameObjectMain* v264;
    tGameObjectMain* v265;
    tGameObjectMain* v266;
    tGameObjectMain* v267;
    Vec3 v268;
    Vec3 v269;
    Vec3 v270;
    Vec3 v271;
    Vec3 v272;
    int v273;
    float v274;
    float v276;
    float v278;
    float v279;
    float v280;
    float v281;
    tGameObjectMain* v282;
    Vec3 v284;
    Vec3 v285;
    Vec3 v286;
    Vec3 v287;
    Vec3 v288;
    Vec3 v289;
    Vec3 v290;
    Vec3 v291;
    Vec3 v292;
    float v293;
    float v294;
    float v295;
    float v296;
    float v297;
    float v298;
    float v299;
    tTextGet v300;
    float v301;
    Vec3 v302;
    float v303;
    tTextGet v304;
    int v305;
    Vec3 v306;
    Vec3 v307;
    Vec3 v308;
    Vec3 v309;
    Vec3 dest;
    tTextGet v311;
    float v312;
    Vec3 v313;
    Vec3 v314;
    Vec3 v315;
    Vec3 v316;
    Vec3 v317;
    Vec3 v318;
    Vec3 v319;
    float v321;
    float v322;
    float v323;
    int v324;
    float v325;
    tTextGet v326;
    Vec3 v327;
    Vec3 a3;
    Vec3 v329;
    Vec3 v330;
    Vec3 v331;
    Vec3 v332;
    tTextGet Src;
    float v334;
    float v335;
    float v336;
    Vec3 v337;
    Vec3 v338;
    Vec3 v339;
    Vec3 v340;
    tTextGet v341;
    float v342;
    Vec3 v343;
    Vec3 v344;
    Vec3 v345;
    Vec3 v346;
    tTextGet v347;
    float v349;
    tTextGet v350;
    float v351;
    float v352;
    Vec3 v354;
    float v355;
    float v356;
    tTextGet v357;
    int v358;
    float v359;
    tTextGet v360;
    float v361;
    tTextGet v362;
    float v363;
    float v364;
    int v365;
    Vec3 v366;
    Vec3 v367;
    Vec3 v368;
    Vec3 v369;
    Vec3 v370;
    Vec3 v371;
    Vec3 v372;
    float v373;
    float v374;
    int v375;
    int v376;
    int idx;
    float v378;
    float v379;
    float v380;
    float v381;
    int a2;
    float v383;
    Vec3 v384;
    Vec3 v385;
    Vec3 v386;
    float v387;
    int v388;
    float v389;
    Vec3 v390;
    Vec3 v391;
    Vec3 v392;
    Vec3 v393;
    Vec3 v394;
    int v395;
    float v396;
    float v397;
    float v398;
    tGameObjectMain* v399;
    Vec3 v400;
    Vec3 v401;
    Vec3 v402;
    Vec3 v403;
    int v404;
    int v405;
    tTextGet v406;
    float v407;
    float v408;
    tTextGet v409;
    Vec3 v410;
    Vec3 v411;
    Vec3 v412;
    Vec3 v413;
    Vec3 v414;
    tScript_mlStruct* v415;
    float v416;
    float v417;
    float v418;
    float v419;
    float v420;
    float v421;
    tTextGet v422;
    float v423;
    Vec3 a5;
    Vec3 v425;
    Vec3 v426;
    Vec3 v427;
    Vec3 v428;
    Vec3 v429;
    Vec3 v430;
    float v431;
    float v432;
    Vec3 v433;
    Vec3 v434;
    Vec3 v435;
    Vec3 v436;
    tTextGet v437;
    int v438;
    float v439;
    float v440;

    v339.x = 1.0;
    v339.y = 0.0;
    v339.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v71 = (tScript_mlStruct*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v71 = 0;
    v415 = v71;
    v274 = 0.0;
    v376 = 0;
    idx = 0;
    a2 = 0;
    v399 = 0;
    v423 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        main_logo_ml_init2_INV_Manager_gao = (void*)AI_ul_RealResolveOneRefObject(0x4900FDB4);
        main_logo_ml_init2_univVars = (void*)AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&main_logo_ml_init2_i_indice_de_map, main_logo_ml_init2_univVars, 16156);
        main_logo_ml_init2_INV_MatFade_gao = AI_ul_RealResolveOneRefObject(0x5E006934);
        fct_main_logo_ml_init2_main_logo_ml_load = AI_ul_RealResolveOneRefFunction(0x3D003882);
        AI2C_ResolveBuffer(&main_logo_ml_init2_INV_Manager_gao_i_OpeningMode, main_logo_ml_init2_INV_Manager_gao, 100);
        AI2C_ResolveBuffer(&main_logo_ml_init2_INV_Manager_gao_i_AskExit, main_logo_ml_init2_INV_Manager_gao, 152);
        AI2C_ResolveBuffer(&main_logo_ml_init2_i_Exit, main_logo_ml_init2_INV_Manager_gao, 148);
        AI2C_ResolveBuffer(&main_logo_ml_init2_i_show_credits, main_logo_ml_init2_univVars, 20396);
        AI2C_ResolveBuffer(&main_logo_ml_init2_i_INV_OptionsFlags, main_logo_ml_init2_univVars, 7656);
        AI2C_ResolveBuffer(&main_logo_ml_init2_world_ID, main_logo_ml_init2_univVars, 4);
        AI2C_ResolveBuffer(&main_logo_ml_init2_load_world, main_logo_ml_init2_univVars, 0);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)29)
    {
        v9 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_main_logo_ml_init2_main_logo_ml_load);
        v305 = v9;
        if (v9 != 1000)
            return v305;
    }
    else
    {
        if (*AI_gppst_CurrentJumpNode == (tAINode*)69)
        {
            *(uint32_t*)main_logo_ml_init2_i_show_credits = 1;
            *(uint32_t*)main_logo_ml_init2_i_INV_OptionsFlags |= 0x10000u;
            *(uint32_t*)main_logo_ml_init2_world_ID = 1003;
            *(uint32_t*)main_logo_ml_init2_load_world = 1;
            v19 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            v375 = v19;
            if (v19 != 1000)
                return v375;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        INO_SetControlSubmode(4);
        if (v415->i_ModePause)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        if (!v415->i_CurrentState)
        {
            v267 = (tGameObjectMain*)v415->o_MenuSel;
            if (v267)
            {
                if ((unsigned int)v267 <= 2)
                    v267 = AI_MainActor[(uint32_t)v267 - 1];
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(v267, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
            }
            AI_EvalFunc_TEXTGet_C(0x4900FDC5, 0, &Src);
            memcpy(&v360, &Src, sizeof(v360));
            v266 = (tGameObjectMain*)v415->texte;
            if (v266)
            {
                if ((unsigned int)v266 <= 2)
                    v266 = AI_MainActor[(uint32_t)v266 - 1];
                STR_Reset(v266);
            }
            else
            {
                STR_Reset(0);
            }
            memcpy(&v406, &v360, sizeof(v406));
            v406.idx += 49;
            v384.x = 0.31f;
            v384.y = 0.60799998f;
            v384.z = 0.0f;
            v265 = (tGameObjectMain*)v415->texte;
            if (v265)
            {
                if ((unsigned int)v265 <= 2)
                    v265 = AI_MainActor[(uint32_t)v265 - 1];
                v264 = v265;
            }
            else
            {
                v264 = 0;
            }
            v1 = AI_EvalFunc_TEXTToString_C(&v406);
            STR_SetStringExt(v264, 0, 2031636, v1, &v384);
            memcpy(&v304, &v360, sizeof(v304));
            v304.idx += 51;
            v292.x = 0.55000001f;
            v292.y = 0.68800002f;
            v292.z = 0.0f;
            v263 = (tGameObjectMain*)v415->texte;
            if (v263)
            {
                if ((unsigned int)v263 <= 2)
                    v263 = AI_MainActor[(uint32_t)v263 - 1];
                v262 = v263;
            }
            else
            {
                v262 = 0;
            }
            v2 = AI_EvalFunc_TEXTToString_C(&v304);
            STR_SetStringExt(v262, 1, 0x1F0014, v2, &v292);
            memcpy(&v362, &v360, sizeof(v362));
            v362.idx += 53;
            v428.x = 0.31f;
            v428.y = 0.75999999f;
            v428.z = 0.0f;
            v261 = (tGameObjectMain*)v415->texte;
            if (v261)
            {
                if ((unsigned int)v261 <= 2)
                    v261 = AI_MainActor[(uint32_t)v261 - 1];
                v260 = v261;
            }
            else
            {
                v260 = 0;
            }
            v3 = AI_EvalFunc_TEXTToString_C(&v362);
            STR_SetStringExt(v260, 2, 2031636, v3, &v428);
            memcpy(&v326, &v360, sizeof(v326));
            v326.idx += 75;
            v385.x = 0.55000001f;
            v385.y = 0.83499998f;
            v385.z = 0.0f;
            v259 = (tGameObjectMain*)v415->texte;
            if (v259)
            {
                if ((unsigned int)v259 <= 2)
                    v259 = AI_MainActor[(uint32_t)v259 - 1];
                v258 = v259;
            }
            else
            {
                v258 = 0;
            }
            v4 = AI_EvalFunc_TEXTToString_C(&v326);
            STR_SetStringExt(v258, 3, 2031636, v4, &v385);
            for (idx = 0; idx < 4; ++idx)
            {
                v257 = (tGameObjectMain*)v415->texte;
                if (v257)
                {
                    if ((unsigned int)v257 <= 2)
                        v257 = AI_MainActor[(uint32_t)v257 - 1];
                    AI_EvalFunc_OBJ_TextPageSet_C(v257, idx, 0, -1, 2, -1);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextPageSet_C(0, idx, 0, -1, 2, -1);
                }
                a5.x = -1.0f;
                a5.y = 0.0f;
                a5.z = 0.0f;
                v256 = (tGameObjectMain*)v415->texte;
                if (v256)
                {
                    if ((unsigned int)v256 <= 2)
                        v256 = AI_MainActor[(uint32_t)v256 - 1];
                    STR_SetSize(v256, idx, 0, -1, &a5);
                }
                else
                {
                    STR_SetSize(0, idx, 0, -1, &a5);
                }
            }
            v255 = (tGameObjectMain*)v415->texte;
            if (v255)
            {
                if ((unsigned int)v255 <= 2)
                    v255 = AI_MainActor[(uint32_t)v255 - 1];
                AI_EvalFunc_OBJ_TextPivotGet_C(v255, 0, &a3);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotGet_C(0, 0, &a3);
            }
            v372.x = 0.02f;
            v372.y = 0.0f;
            v372.z = 0.0f;
            MATH_SubstractVec(&dest, &a3, &v372);
            v339 = dest;
            v254 = (tGameObjectMain*)v415->texte;
            if (v254)
            {
                if ((unsigned int)v254 <= 2)
                    v254 = AI_MainActor[(uint32_t)v254 - 1];
                STR_SetStringExt(v254, 20, 18808854, (char*)AI_gpst_CurrentFunction->localStringConstsData, &v339);
            }
            else
            {
                STR_SetStringExt(0, 20, 18808854, (char*)AI_gpst_CurrentFunction->localStringConstsData, &v339);
            }
            v253 = (tGameObjectMain*)v415->texte;
            if (v253)
            {
                if ((unsigned int)v253 <= 2)
                    v253 = AI_MainActor[(uint32_t)v253 - 1];
                AI_EvalFunc_OBJ_TextSizeGet_C(v253, 0, 0, -1, &v436);
            }
            else
            {
                AI_EvalFunc_OBJ_TextSizeGet_C(0, 0, 0, -1, &v436);
            }
            v339.x = (float)(v436.x + 0.039999999 + v339.x);
            v252 = (tGameObjectMain*)v415->texte;
            if (v252)
            {
                if ((unsigned int)v252 <= 2)
                    v252 = AI_MainActor[(uint32_t)v252 - 1];
                STR_SetStringExt(v252, 21, 18808852, (char*)AI_gpst_CurrentFunction->localStringConstsData + 16, &v339);
            }
            else
            {
                STR_SetStringExt(0, 21, 18808852, (char*)AI_gpst_CurrentFunction->localStringConstsData + 16, &v339);
            }
            if (-4.0 < 0.0)
                v70 = 0.0;
            else
                v70 = -4.0f;
            v380 = v70;
            v423 = 1.0f - v70;
            v330.x = v423 + 0.55000001f;
            v330.y = 0.73900002f;
            v330.z = 0.0f;
            v251 = (tGameObjectMain*)v415->texte;
            if (v251)
            {
                if ((unsigned int)v251 <= 2)
                    v251 = AI_MainActor[(uint32_t)v251 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v251, 3, &v330);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 3, &v330);
            }
            if (-2.0 < 0.0)
                v69 = 0.0;
            else
                v69 = -2.0f;
            v351 = v69;
            if (v69 > 1.0)
                v68 = 1.0;
            else
                v68 = v351;
            v299 = v68;
            v423 = 1.0f - v68;
            v346.x = v423 + 0.31f;
            v346.y = 0.66900003f;
            v346.z = 0.0f;
            v250 = (tGameObjectMain*)v415->texte;
            if (v250)
            {
                if ((unsigned int)v250 <= 2)
                    v250 = AI_MainActor[(uint32_t)v250 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v250, 2, &v346);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 2, &v346);
            }
            if (-1.0 < 0.0)
                v67 = 0.0;
            else
                v67 = -1.0;
            v397 = v67;
            if (v67 > 1.0)
                v66 = 1.0;
            else
                v66 = v397;
            v373 = v66;
            v423 = 1.0f - v66;
            v433.x = v423 + 0.31f;
            v433.y = 0.53399998f;
            v433.z = 0.0f;
            v249 = (tGameObjectMain*)v415->texte;
            if (v249)
            {
                if ((unsigned int)v249 <= 2)
                    v249 = AI_MainActor[(uint32_t)v249 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v249, 0, &v433);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 0, &v433);
            }
            v387 = 0.0;
            if (0.0 > 1.0)
                v65 = 1.0;
            else
                v65 = v387;
            v342 = v65;
            v423 = 1.0f - v65;
            v338.x = v423 + 0.55000001f;
            v338.y = 0.602f;
            v338.z = 0.0f;
            v248 = (tGameObjectMain*)v415->texte;
            if (v248)
            {
                if ((unsigned int)v248 <= 2)
                    v248 = AI_MainActor[(uint32_t)v248 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v248, 1, &v338);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 1, &v338);
            }
            v247 = (tGameObjectMain*)v415->o_MenuSel;
            if (v247)
            {
                if ((unsigned int)v247 <= 2)
                    v247 = AI_MainActor[(uint32_t)v247 - 1];
                AI_EvalFunc_OBJ_SetChannelProg_C(v247, 0, 1.0);
            }
            else
            {
                AI_EvalFunc_OBJ_SetChannelProg_C(0, 0, 1.0);
            }
            v246 = (tGameObjectMain*)v415->texte;
            if (v246)
            {
                if ((unsigned int)v246 <= 2)
                    v246 = AI_MainActor[(uint32_t)v246 - 1];
                AI_EvalFunc_OBJ_TextFlagSet_C(v246, 20, 0x400);
            }
            else
            {
                AI_EvalFunc_OBJ_TextFlagSet_C(0, 20, 0x400);
            }
            v245 = (tGameObjectMain*)v415->texte;
            if (v245)
            {
                if ((unsigned int)v245 <= 2)
                    v245 = AI_MainActor[(uint32_t)v245 - 1];
                AI_EvalFunc_OBJ_TextFlagSet_C(v245, 21, 0x400);
            }
            else
            {
                AI_EvalFunc_OBJ_TextFlagSet_C(0, 21, 0x400);
            }
            v244 = (tGameObjectMain*)v415->texte;
            if (v244)
            {
                if ((unsigned int)v244 <= 2)
                    v244 = AI_MainActor[(uint32_t)v244 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v244, 0, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 0, 0, -1, 0x7F7F7F7F);
            }
            v243 = (tGameObjectMain*)v415->texte;
            if (v243)
            {
                if ((unsigned int)v243 <= 2)
                    v243 = AI_MainActor[(uint32_t)v243 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v243, 1, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 1, 0, -1, 0x7F7F7F7F);
            }
            v242 = (tGameObjectMain*)v415->texte;
            if (v242)
            {
                if ((unsigned int)v242 <= 2)
                    v242 = AI_MainActor[(uint32_t)v242 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v242, 2, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 2, 0, -1, 0x7F7F7F7F);
            }
            v241 = (tGameObjectMain*)v415->texte;
            if (v241)
            {
                if ((unsigned int)v241 <= 2)
                    v241 = AI_MainActor[(uint32_t)v241 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v241, 3, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 3, 0, -1, 0x7F7F7F7F);
            }
            v240 = (tGameObjectMain*)v415->texte;
            if (v240)
            {
                if ((unsigned int)v240 <= 2)
                    v240 = AI_MainActor[(uint32_t)v240 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v240, v415->i_CurrentLine, 0, -1, -1);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, v415->i_CurrentLine, 0, -1, -1);
            }
            v239 = (tGameObjectMain*)v415->texte;
            if (v239)
            {
                if ((unsigned int)v239 <= 2)
                    v239 = AI_MainActor[(uint32_t)v239 - 1];
                AI_EvalFunc_OBJ_TextPivotGet_C(v239, v415->i_CurrentLine, &v393);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotGet_C(0, v415->i_CurrentLine, &v393);
            }
            v302.x = 0.02f;
            v302.y = 0.0f;
            v302.z = 0.0f;
            MATH_SubstractVec(&v425, &v393, &v302);
            v339 = v425;
            v238 = (tGameObjectMain*)v415->texte;
            if (v238)
            {
                if ((unsigned int)v238 <= 2)
                    v238 = AI_MainActor[(uint32_t)v238 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v238, 20, &v339);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 20, &v339);
            }
            v237 = (tGameObjectMain*)v415->texte;
            if (v237)
            {
                if ((unsigned int)v237 <= 2)
                    v237 = AI_MainActor[(uint32_t)v237 - 1];
                AI_EvalFunc_OBJ_TextSizeGet_C(v237, v415->i_CurrentLine, 0, -1, &v354);
            }
            else
            {
                AI_EvalFunc_OBJ_TextSizeGet_C(0, v415->i_CurrentLine, 0, -1, &v354);
            }
            v339.x = (float)(v354.x + 0.039999999 + v339.x);
            v236 = (tGameObjectMain*)v415->texte;
            if (v236)
            {
                if ((unsigned int)v236 <= 2)
                    v236 = AI_MainActor[(uint32_t)v236 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v236, 21, &v339);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 21, &v339);
            }
            v415->f_Time = 1.0;
            v415->i_CurrentState = 1;
            v415->i_CurrentLine = 0;
            v415->i_WaitZeroJoy = 1;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        if (v415->i_CurrentState == 1)
        {
            v420 = TIM_gf_dt;
            v415->f_Time = TIM_gf_dt * 8.0f + v415->f_Time;
            // test (slow down menu slide)
            //v415->f_Time = TIM_gf_dt * 1.0f + v415->f_Time;
            if (v415->f_Time > 4.0 && v415->f_Time < 5.0)
                v415->f_Time = v415->f_Time + 1.0f;
            if (v415->f_Time > 6.0)
                v415->f_Time = 6.0f;
            v235 = v415->f_Time - 5.0f;
            if (v235 < 0.0)
                v64 = 0.0f;
            else
                v64 = v415->f_Time - 5.0f;
            v356 = v64;
            v423 = 1.0f - v64;
            v368.x = v423 + 0.55000001f;
            v368.y = 0.73900002f;
            v368.z = 0.0f;
            v234 = (tGameObjectMain*)v415->texte;
            if (v234)
            {
                if ((unsigned int)v234 <= 2)
                    v234 = AI_MainActor[(uint32_t)v234 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v234, 3, &v368);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 3, &v368);
            }
            v233 = v415->f_Time - 3.0f;
            if (v233 < 0.0)
                v63 = 0.0;
            else
                v63 = v415->f_Time - 3.0f;
            v431 = v63;
            if (v63 > 1.0)
                v62 = 1.0f;
            else
                v62 = v431;
            v322 = v62;
            v423 = 1.0f - v62;
            v435.x = v423 + 0.31f;
            v435.y = 0.66900003f;
            v435.z = 0.0f;
            v232 = (tGameObjectMain*)v415->texte;
            if (v232)
            {
                if ((unsigned int)v232 <= 2)
                    v232 = AI_MainActor[(uint32_t)v232 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v232, 2, &v435);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 2, &v435);
            }
            v231 = v415->f_Time - 2.0f;
            if (v231 < 0.0)
                v61 = 0.0;
            else
                v61 = v415->f_Time - 2.0f;
            v396 = v61;
            if (v61 > 1.0)
                v60 = 1.0f;
            else
                v60 = v396;
            v296 = v60;
            v423 = 1.0f - v60;
            v317.x = v423 + 0.31f;
            v317.y = 0.53399998f;
            v317.z = 0.0f;
            v230 = (tGameObjectMain*)v415->texte;
            if (v230)
            {
                if ((unsigned int)v230 <= 2)
                    v230 = AI_MainActor[(uint32_t)v230 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v230, 0, &v317);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 0, &v317);
            }
            v229 = v415->f_Time - 1.0f;
            if (v229 < 0.0)
                v59 = 0.0;
            else
                v59 = v415->f_Time - 1.0f;
            v407 = v59;
            if (v59 > 1.0)
                v58 = 1.0f;
            else
                v58 = v407;
            v432 = v58;
            v423 = 1.0f - v58;
            v367.x = v423 + 0.55000001f;
            v367.y = 0.602f;
            v367.z = 0.0f;
            v228 = (tGameObjectMain*)v415->texte;
            if (v228)
            {
                if ((unsigned int)v228 <= 2)
                    v228 = AI_MainActor[(uint32_t)v228 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v228, 1, &v367);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 1, &v367);
            }
            v227 = (tGameObjectMain*)v415->o_MenuSel;
            if (v227)
            {
                if ((unsigned int)v227 <= 2)
                    v227 = AI_MainActor[(uint32_t)v227 - 1];
                AI_EvalFunc_OBJ_SetChannelProg_C(v227, 0, v415->f_Time);
            }
            else
            {
                AI_EvalFunc_OBJ_SetChannelProg_C(0, 0, v415->f_Time);
            }
            if (v415->f_Time == 6.0)
            {
                v226 = (tGameObjectMain*)v415->texte;
                if (v226)
                {
                    if ((unsigned int)v226 <= 2)
                        v226 = AI_MainActor[(uint32_t)v226 - 1];
                    AI_EvalFunc_OBJ_TextFlagSet_C(v226, 20, 0x16);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextFlagSet_C(0, 20, 0x16);
                }
                v225 = (tGameObjectMain*)v415->texte;
                if (v225)
                {
                    if ((unsigned int)v225 <= 2)
                        v225 = AI_MainActor[(uint32_t)v225 - 1];
                    AI_EvalFunc_OBJ_TextFlagSet_C(v225, 21, 0x14);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextFlagSet_C(0, 21, 0x14);
                }
            }
            else
            {
                v224 = (tGameObjectMain*)v415->texte;
                if (v224)
                {
                    if ((unsigned int)v224 <= 2)
                        v224 = AI_MainActor[(uint32_t)v224 - 1];
                    AI_EvalFunc_OBJ_TextFlagSet_C(v224, 20, 0x400);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextFlagSet_C(0, 20, 0x400);
                }
                v223 = (tGameObjectMain*)v415->texte;
                if (v223)
                {
                    if ((unsigned int)v223 <= 2)
                        v223 = AI_MainActor[(uint32_t)v223 - 1];
                    AI_EvalFunc_OBJ_TextFlagSet_C(v223, 21, 0x400);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextFlagSet_C(0, 21, 0x400);
                }
            }
            v222 = (tGameObjectMain*)v415->texte;
            if (v222)
            {
                if ((unsigned int)v222 <= 2)
                    v222 = AI_MainActor[(uint32_t)v222 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v222, 0, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 0, 0, -1, 0x7F7F7F7F);
            }
            v221 = (tGameObjectMain*)v415->texte;
            if (v221)
            {
                if ((unsigned int)v221 <= 2)
                    v221 = AI_MainActor[(uint32_t)v221 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v221, 1, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 1, 0, -1, 0x7F7F7F7F);
            }
            v220 = (tGameObjectMain*)v415->texte;
            if (v220)
            {
                if ((unsigned int)v220 <= 2)
                    v220 = AI_MainActor[(uint32_t)v220 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v220, 2, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 2, 0, -1, 0x7F7F7F7F);
            }
            v219 = (tGameObjectMain*)v415->texte;
            if (v219)
            {
                if ((unsigned int)v219 <= 2)
                    v219 = AI_MainActor[(uint32_t)v219 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v219, 3, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 3, 0, -1, 0x7F7F7F7F);
            }
            v218 = (tGameObjectMain*)v415->texte;
            if (v218)
            {
                if ((unsigned int)v218 <= 2)
                    v218 = AI_MainActor[(uint32_t)v218 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v218, v415->i_CurrentLine, 0, -1, -1);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, v415->i_CurrentLine, 0, -1, -1);
            }
            v217 = (tGameObjectMain*)v415->texte;
            if (v217)
            {
                if ((unsigned int)v217 <= 2)
                    v217 = AI_MainActor[(uint32_t)v217 - 1];
                AI_EvalFunc_OBJ_TextPivotGet_C(v217, v415->i_CurrentLine, &v344);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotGet_C(0, v415->i_CurrentLine, &v344);
            }
            v394.x = 0.02f;
            v394.y = 0.0f;
            v394.z = 0.0f;
            MATH_SubstractVec(&v319, &v344, &v394);
            v339 = v319;
            v216 = (tGameObjectMain*)v415->texte;
            if (v216)
            {
                if ((unsigned int)v216 <= 2)
                    v216 = AI_MainActor[(uint32_t)v216 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v216, 20, &v339);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 20, &v339);
            }
            v215 = (tGameObjectMain*)v415->texte;
            if (v215)
            {
                if ((unsigned int)v215 <= 2)
                    v215 = AI_MainActor[(uint32_t)v215 - 1];
                AI_EvalFunc_OBJ_TextSizeGet_C(v215, v415->i_CurrentLine, 0, -1, &v345);
            }
            else
            {
                AI_EvalFunc_OBJ_TextSizeGet_C(0, v415->i_CurrentLine, 0, -1, &v345);
            }
            v339.x = v345.x + 0.039999999f + v339.x;
            v214 = (tGameObjectMain*)v415->texte;
            if (v214)
            {
                if ((unsigned int)v214 <= 2)
                    v214 = AI_MainActor[(uint32_t)v214 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v214, 21, &v339);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 21, &v339);
            }
            if (v415->f_Time == 6.0)
            {
                v415->i_CurrentState = 2;
                v415->f_Chrono = TIM_gf_MainClock - TIM_gf_LockTime;
            }
        }
        if (v415->i_CurrentState == 2)
        {
            AI_EvalFunc_IoJoyGetMove_C(&v403);
            v274 = v403.y;
            if (v415->i_WaitZeroJoy)
            {
                v57 = (float)fabs(v274);
                v312 = v57;
                if (v57 < 0.5)
                    v415->i_WaitZeroJoy = 0;
            }
            else
            {
                idx = v415->i_CurrentLine;
                if (v274 > 0.69999999 && v415->i_CurrentLine > 0)
                    --v415->i_CurrentLine;
                if (v274 < -0.69999999 && v415->i_CurrentLine < 3)
                    ++v415->i_CurrentLine;
                if (idx != v415->i_CurrentLine)
                {
                    v404 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 5, 2176);
                    v439 = TIM_gf_MainClock - TIM_gf_LockTime;
                    v415->f_Chrono = v439;
                    v415->i_WaitZeroJoy = 1;
                    v316.x = 0.5f;
                    v56 = (float)v415->i_CurrentLine;
                    v316.y = 0.07f * v56 + 0.64999998f;
                    v316.z = 0.0f;
                    v213 = (tGameObjectMain*)v415->texte;
                    if (v213)
                    {
                        if ((unsigned int)v213 <= 2)
                            v213 = AI_MainActor[(uint32_t)v213 - 1];
                        AI_EvalFunc_OBJ_TextPivotSet_C(v213, 20, &v316);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_TextPivotSet_C(0, 20, &v316);
                    }
                }
            }
            if (1.0 < 0.0)
                v55 = 0.0;
            else
                v55 = 1.0f;
            v355 = v55;
            v423 = 1.0f - v55;
            v314.x = v423 + 0.55000001f;
            v314.y = 0.73900002f;
            v314.z = 0.0f;
            v212 = (tGameObjectMain*)v415->texte;
            if (v212)
            {
                if ((unsigned int)v212 <= 2)
                    v212 = AI_MainActor[(uint32_t)v212 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v212, 3, &v314);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 3, &v314);
            }
            if (3.0 < 0.0)
                v54 = 0.0;
            else
                v54 = 3.0f;
            v349 = v54;
            if (v54 > 1.0)
                v53 = 1.0f;
            else
                v53 = v349;
            v276 = v53;
            v423 = 1.0f - v53;
            v401.x = v423 + 0.31f;
            v401.y = 0.66900003f;
            v401.z = 0.0f;
            v211 = (tGameObjectMain*)v415->texte;
            if (v211)
            {
                if ((unsigned int)v211 <= 2)
                    v211 = AI_MainActor[(uint32_t)v211 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v211, 2, &v401);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 2, &v401);
            }
            if (4.0 < 0.0)
                v52 = 0.0;
            else
                v52 = 4.0f;
            v279 = v52;
            if (v52 > 1.0)
                v51 = 1.0f;
            else
                v51 = v279;
            v378 = v51;
            v423 = 1.0f - v51;
            v309.x = v423 + 0.31f;
            v309.y = 0.53399998f;
            v309.z = 0.0f;
            v210 = (tGameObjectMain*)v415->texte;
            if (v210)
            {
                if ((unsigned int)v210 <= 2)
                    v210 = AI_MainActor[(uint32_t)v210 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v210, 0, &v309);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 0, &v309);
            }
            if (5.0 < 0.0)
                v50 = 0.0;
            else
                v50 = 5.0;
            v381 = v50;
            if (v50 > 1.0)
                v49 = 1.0f;
            else
                v49 = v381;
            v281 = v49;
            v423 = 1.0f - v49;
            v318.x = v423 + 0.55000001f;
            v318.y = 0.602f;
            v318.z = 0.0f;
            v209 = (tGameObjectMain*)v415->texte;
            if (v209)
            {
                if ((unsigned int)v209 <= 2)
                    v209 = AI_MainActor[(uint32_t)v209 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v209, 1, &v318);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 1, &v318);
            }
            v208 = (tGameObjectMain*)v415->o_MenuSel;
            if (v208)
            {
                if ((unsigned int)v208 <= 2)
                    v208 = AI_MainActor[(uint32_t)v208 - 1];
                AI_EvalFunc_OBJ_SetChannelProg_C(v208, 0, 6.0);
            }
            else
            {
                AI_EvalFunc_OBJ_SetChannelProg_C(0, 0, 6.0);
            }
            v207 = (tGameObjectMain*)v415->texte;
            if (v207)
            {
                if ((unsigned int)v207 <= 2)
                    v207 = AI_MainActor[(uint32_t)v207 - 1];
                AI_EvalFunc_OBJ_TextFlagSet_C(v207, 20, 22);
            }
            else
            {
                AI_EvalFunc_OBJ_TextFlagSet_C(0, 20, 22);
            }
            v206 = (tGameObjectMain*)v415->texte;
            if (v206)
            {
                if ((unsigned int)v206 <= 2)
                    v206 = AI_MainActor[(uint32_t)v206 - 1];
                AI_EvalFunc_OBJ_TextFlagSet_C(v206, 21, 20);
            }
            else
            {
                AI_EvalFunc_OBJ_TextFlagSet_C(0, 21, 20);
            }
            v205 = (tGameObjectMain*)v415->texte;
            if (v205)
            {
                if ((unsigned int)v205 <= 2)
                    v205 = AI_MainActor[(uint32_t)v205 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v205, 0, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 0, 0, -1, 0x7F7F7F7F);
            }
            v204 = (tGameObjectMain*)v415->texte;
            if (v204)
            {
                if ((unsigned int)v204 <= 2)
                    v204 = AI_MainActor[(uint32_t)v204 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v204, 1, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 1, 0, -1, 0x7F7F7F7F);
            }
            v203 = (tGameObjectMain*)v415->texte;
            if (v203)
            {
                if ((unsigned int)v203 <= 2)
                    v203 = AI_MainActor[(uint32_t)v203 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v203, 2, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 2, 0, -1, 0x7F7F7F7F);
            }
            v202 = (tGameObjectMain*)v415->texte;
            if (v202)
            {
                if ((unsigned int)v202 <= 2)
                    v202 = AI_MainActor[(uint32_t)v202 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v202, 3, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 3, 0, -1, 0x7F7F7F7F);
            }
            v201 = (tGameObjectMain*)v415->texte;
            if (v201)
            {
                if ((unsigned int)v201 <= 2)
                    v201 = AI_MainActor[(uint32_t)v201 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v201, v415->i_CurrentLine, 0, -1, -1);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, v415->i_CurrentLine, 0, -1, -1);
            }
            v200 = (tGameObjectMain*)v415->texte;
            if (v200)
            {
                if ((unsigned int)v200 <= 2)
                    v200 = AI_MainActor[(uint32_t)v200 - 1];
                AI_EvalFunc_OBJ_TextPivotGet_C(v200, v415->i_CurrentLine, &v269);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotGet_C(0, v415->i_CurrentLine, &v269);
            }
            v427.x = 0.02f;
            v427.y = 0.0f;
            v427.z = 0.0f;
            MATH_SubstractVec(&v289, &v269, &v427);
            v339 = v289;
            v199 = (tGameObjectMain*)v415->texte;
            if (v199)
            {
                if ((unsigned int)v199 <= 2)
                    v199 = AI_MainActor[(uint32_t)v199 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v199, 20, &v339);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 20, &v339);
            }
            v198 = (tGameObjectMain*)v415->texte;
            if (v198)
            {
                if ((unsigned int)v198 <= 2)
                    v198 = AI_MainActor[(uint32_t)v198 - 1];
                AI_EvalFunc_OBJ_TextSizeGet_C(v198, v415->i_CurrentLine, 0, -1, &v271);
            }
            else
            {
                AI_EvalFunc_OBJ_TextSizeGet_C(0, v415->i_CurrentLine, 0, -1, &v271);
            }
            v339.x = v271.x + 0.039999999f + v339.x;
            v197 = (tGameObjectMain*)v415->texte;
            if (v197)
            {
                if ((unsigned int)v197 <= 2)
                    v197 = AI_MainActor[(uint32_t)v197 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v197, 21, &v339);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 21, &v339);
            }
            if (AI_EvalFunc_TIMEElapsed_C(v415->f_Chrono, 30.0f))
            {
                v415->i_ModePause = 2;
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 1000;
            }
            else
            {
                if (AI_EvalFunc_IoButtonJustPressed_C(0))
                {
                    v395 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 6, 2176);
                    v415->i_CurrentState = 100 * (v415->i_CurrentLine + 1);
                }
                if (AI_EvalFunc_IoButtonJustPressed_C(32))
                {
                    v388 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 6, 2176);
                    v415->i_CurrentState = 100;
                }
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 1000;
            }
            return result;
        }
        if (v415->i_CurrentState == 100)
        {
            v196 = (tGameObjectMain*)v415->sound;
            if (v196)
            {
                if ((unsigned int)v196 <= 2)
                    v196 = AI_MainActor[(uint32_t)v196 - 1];
                AI_EvalFunc_SNDStopModifier_C(v196, 0);
            }
            else
            {
                AI_EvalFunc_SNDStopModifier_C(0, 0);
            }
            v399 = main_logo_ml_init2_INV_Manager_gao;
            v195 = main_logo_ml_init2_INV_Manager_gao;
            if (main_logo_ml_init2_INV_Manager_gao)
            {
                if ((unsigned int)main_logo_ml_init2_INV_Manager_gao <= 2)
                    v195 = AI_MainActor[(uint32_t)main_logo_ml_init2_INV_Manager_gao - 1];
                v5 = AI_EvalFunc_GFXAddSorted_C(v195, 19, 5);
            }
            else
            {
                v5 = AI_EvalFunc_GFXAddSorted_C(0, 19, 5);
            }
            v376 = v5;
            v194 = v399;
            if (v399)
            {
                if ((unsigned int)v399 <= 2)
                    v194 = AI_MainActor[(uint32_t)v399 - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v376, v194, 1);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v376, 0, 1);
            }
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v376, 2, 1);
            for (a2 = 0; a2 < 110; ++a2)
            {
                if (*(uint32_t*)(main_logo_ml_init2_i_indice_de_map + 4 * a2 + 4) == -1) // FIXME, ez egy tömbb
                {
                    AI_EvalFunc_TEXTGet_C(0x2600063E, a2, &v409);
                    v193 = v399;
                    if (v399)
                    {
                        if ((unsigned int)v399 <= 2)
                            v193 = AI_MainActor[(uint32_t)v399 - 1];
                        v192 = v193;
                    }
                    else
                    {
                        v192 = 0;
                    }
                    v6 = AI_EvalFunc_TEXTToString_C(&v409);
                    STR_SetString(v192, 30, v6);
                    v191 = v399;
                    if (v399)
                    {
                        if ((unsigned int)v399 <= 2)
                            v191 = AI_MainActor[(uint32_t)v399 - 1];
                        STR_ToUpper(v191, 30, 0, -1);
                    }
                    else
                    {
                        STR_ToUpper(0, 30, 0, -1);
                    }
                    v190 = v399;
                    if (v399)
                    {
                        if ((unsigned int)v399 <= 2)
                            v190 = AI_MainActor[(uint32_t)v399 - 1];
                        AI_EvalFunc_OBJ_TextPageSet_C(v190, 30, 0, -1, 1, -1);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_TextPageSet_C(0, 30, 0, -1, 1, -1);
                    }
                    v284.x = 0.5f;
                    v284.y = 0.5f;
                    v284.z = 0.0f;
                    v189 = v399;
                    if (v399)
                    {
                        if ((unsigned int)v399 <= 2)
                            v189 = AI_MainActor[(uint32_t)v399 - 1];
                        STR_SetStringExt(v189, 30, 1441813, (char*)AI_gpst_CurrentFunction->localStringConstsData + 23, &v284);
                    }
                    else
                    {
                        STR_SetStringExt(0, 30, 1441813, (char*)AI_gpst_CurrentFunction->localStringConstsData + 23, &v284);
                    }
                    AI_EvalFunc_GFXSeti_C_CURRENT(v376, 19100, 30);
                    v188 = v399;
                    if (v399)
                    {
                        if ((unsigned int)v399 <= 2)
                            v188 = AI_MainActor[(uint32_t)v399 - 1];
                        STR_SetString(v188, 30, (char*)AI_gpst_CurrentFunction->localStringConstsData + 24);
                    }
                    else
                    {
                        STR_SetString(0, 30, (char*)AI_gpst_CurrentFunction->localStringConstsData + 24);
                    }
                    break;
                }
            }
            v187 = v399;
            if (v399)
            {
                if ((unsigned int)v399 <= 2)
                    v187 = AI_MainActor[(uint32_t)v399 - 1];
                v7 = AI_EvalFunc_GFXAddSorted_C(v187, 19, 6);
            }
            else
            {
                v7 = AI_EvalFunc_GFXAddSorted_C(0, 19, 6);
            }
            v376 = v7;
            v186 = v399;
            if (v399)
            {
                if ((unsigned int)v399 <= 2)
                    v186 = AI_MainActor[(uint32_t)v399 - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v376, v186, 0);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v376, 0, 0);
            }
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v376, 2, 1);
            AI_EvalFunc_TEXTGet_C(1224883814, 17, &v357);
            v400.x = 0.5f;
            v400.y = 0.80000001f;
            v400.z = 0.0f;
            v185 = v399;
            if (v399)
            {
                if ((unsigned int)v399 <= 2)
                    v185 = AI_MainActor[(uint32_t)v399 - 1];
                v184 = v185;
            }
            else
            {
                v184 = 0;
            }
            v8 = AI_EvalFunc_TEXTToString_C(&v357);
            STR_SetStringExt(v184, 31, 1507349, v8, &v400);
            AI_EvalFunc_GFXSeti_C_CURRENT(v376, 19100, 31);
            v183 = v399;
            if (v399)
            {
                if ((unsigned int)v399 <= 2)
                    v183 = AI_MainActor[(uint32_t)v399 - 1];
                STR_SetString(v183, 31, (char*)AI_gpst_CurrentFunction->localStringConstsData + 25);
            }
            else
            {
                STR_SetString(0, 31, (char*)AI_gpst_CurrentFunction->localStringConstsData + 25);
            }
            AI_EvalFunc_GFXDel_C_CURRENT(10);
            idx = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(idx, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(idx, 2, 1);
            v182 = (tGameObjectMain*)main_logo_ml_init2_INV_MatFade_gao;
            if (main_logo_ml_init2_INV_MatFade_gao)
            {
                if ((unsigned int)main_logo_ml_init2_INV_MatFade_gao <= 2)
                    v182 = AI_MainActor[main_logo_ml_init2_INV_MatFade_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(idx, v182, 9);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(idx, 0, 9);
            }
            GFXSetCurGlobal(idx);
            AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(11101, -16777216);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 0.2f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 1.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
            *AI_gppst_CurrentJumpNode = (tAINode*)29;
            TIME_Wait_C(0.40000001f);
            return 0;
        }
    }
    if (v415->i_CurrentState == 200)
    {
        v181 = (tGameObjectMain*)v415->texte;
        if (v181)
        {
            if ((unsigned int)v181 <= 2)
                v181 = AI_MainActor[(uint32_t)v181 - 1];
            STR_Reset(v181);
        }
        else
        {
            STR_Reset(0);
        }
        AI_EvalFunc_OBJ_FlagInactiveSet_C(main_logo_ml_init2_INV_Manager_gao, 0);
        *(uint32_t*)main_logo_ml_init2_INV_Manager_gao_i_OpeningMode = 13;
        v415->i_CurrentState = 201;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else if (v415->i_CurrentState == 201)
    {
        if (*(uint32_t*)main_logo_ml_init2_INV_Manager_gao_i_AskExit == 1)
        {
            *(uint32_t*)main_logo_ml_init2_INV_Manager_gao_i_AskExit = 0;
            *(uint32_t*)main_logo_ml_init2_i_Exit = 1;
            v415->i_CurrentState = 202;
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else if (v415->i_CurrentState == 202)
    {
        if (!*(uint32_t*)main_logo_ml_init2_i_Exit)
        {
            v273 = AI_EvalFunc_SNDRequest_C(main_logo_ml_init2_INV_Manager_gao, 7, 2176);
            v415->i_CurrentState = 0;
            AI_EvalFunc_VIEWFocaleSet_C(0, 1.2f);
            v180 = (tGameObjectMain*)v415->o_EventCamera;
            if (v180)
            {
                if ((unsigned int)v180 <= 2)
                    v180 = AI_MainActor[(uint32_t)v180 - 1];
                v10 = AI_EvalFunc_VIEWAssignObject_C(v180, 0);
            }
            else
            {
                v10 = AI_EvalFunc_VIEWAssignObject_C(0, 0);
            }
            v282 = v10;
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else if (v415->i_CurrentState == 300)
    {
        v179 = (tGameObjectMain*)v415->texte;
        if (v179)
        {
            if ((unsigned int)v179 <= 2)
                v179 = AI_MainActor[(uint32_t)v179 - 1];
            STR_Reset(v179);
        }
        else
        {
            STR_Reset(0);
        }
        AI_EvalFunc_OBJ_FlagInactiveSet_C(main_logo_ml_init2_INV_Manager_gao, 0);
        *(uint32_t*)main_logo_ml_init2_INV_Manager_gao_i_OpeningMode = 17;
        v415->i_CurrentState = 201;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        if (v415->i_CurrentState == 400)
        {
            v415->f_Time = 6.0;
            v438 = Demo_Seti(666);
        }
        if (v415->i_CurrentState == 401)
        {
            v301 = TIM_gf_dt;
            v415->f_Time = v415->f_Time - TIM_gf_dt * 8.0f;
            if (v415->f_Time < 5.0 && v415->f_Time > 4.0)
                v415->f_Time = v415->f_Time - 1.0f;
            if (v415->f_Time < 1.0)
                v415->f_Time = 1.0f;
            v178 = v415->f_Time - 5.0f;
            if (v178 < 0.0)
                v48 = 0.0;
            else
                v48 = v415->f_Time - 5.0f;
            v389 = v48;
            v423 = 1.0f - v48;
            v392.x = v423 + 0.55000001f;
            v392.y = 0.73900002f;
            v392.z = 0.0f;
            v177 = (tGameObjectMain*)v415->texte;
            if (v177)
            {
                if ((unsigned int)v177 <= 2)
                    v177 = AI_MainActor[(uint32_t)v177 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v177, 3, &v392);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 3, &v392);
            }
            v176 = v415->f_Time - 3.0f;
            if (v176 < 0.0)
                v47 = 0.0;
            else
                v47 = v415->f_Time - 3.0f;
            v294 = v47;
            if (v47 > 1.0)
                v46 = 1.0f;
            else
                v46 = v294;
            v303 = v46;
            v423 = 1.0f - v46;
            v340.x = v423 + 0.31f;
            v340.y = 0.66900003f;
            v340.z = 0.0f;
            v175 = (tGameObjectMain*)v415->texte;
            if (v175)
            {
                if ((unsigned int)v175 <= 2)
                    v175 = AI_MainActor[(uint32_t)v175 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v175, 2, &v340);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 2, &v340);
            }
            v174 = v415->f_Time - 2.0f;
            if (v174 < 0.0)
                v45 = 0.0f;
            else
                v45 = v415->f_Time - 2.0f;
            v359 = v45;
            if (v45 > 1.0)
                v44 = 1.0f;
            else
                v44 = v359;
            v278 = v44;
            v423 = 1.0f - v44;
            v268.x = v423 + 0.31f;
            v268.y = 0.53399998f;
            v268.z = 0.0f;
            v173 = (tGameObjectMain*)v415->texte;
            if (v173)
            {
                if ((unsigned int)v173 <= 2)
                    v173 = AI_MainActor[(uint32_t)v173 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v173, 0, &v268);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 0, &v268);
            }
            v172 = v415->f_Time - 1.0f;
            if (v172 < 0.0)
                v43 = 0.0f;
            else
                v43 = v415->f_Time - 1.0f;
            v364 = v43;
            if (v43 > 1.0)
                v42 = 1.0f;
            else
                v42 = v364;
            v352 = v42;
            v423 = 1.0f - v42;
            v434.x = v423 + 0.55000001f;
            v434.y = 0.602f;
            v434.z = 0.0f;
            v171 = (tGameObjectMain*)v415->texte;
            if (v171)
            {
                if ((unsigned int)v171 <= 2)
                    v171 = AI_MainActor[(uint32_t)v171 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v171, 1, &v434);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 1, &v434);
            }
            v170 = (tGameObjectMain*)v415->o_MenuSel;
            if (v170)
            {
                if ((unsigned int)v170 <= 2)
                    v170 = AI_MainActor[(uint32_t)v170 - 1];
                AI_EvalFunc_OBJ_SetChannelProg_C(v170, 0, v415->f_Time);
            }
            else
            {
                AI_EvalFunc_OBJ_SetChannelProg_C(0, 0, v415->f_Time);
            }
            if (v415->f_Time == 6.0)
            {
                v169 = (tGameObjectMain*)v415->texte;
                if (v169)
                {
                    if ((unsigned int)v169 <= 2)
                        v169 = AI_MainActor[(uint32_t)v169 - 1];
                    AI_EvalFunc_OBJ_TextFlagSet_C(v169, 20, 0x16);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextFlagSet_C(0, 20, 0x16);
                }
                v168 = (tGameObjectMain*)v415->texte;
                if (v168)
                {
                    if ((unsigned int)v168 <= 2)
                        v168 = AI_MainActor[(uint32_t)v168 - 1];
                    AI_EvalFunc_OBJ_TextFlagSet_C(v168, 21, 0x14);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextFlagSet_C(0, 21, 0x14);
                }
            }
            else
            {
                v167 = (tGameObjectMain*)v415->texte;
                if (v167)
                {
                    if ((unsigned int)v167 <= 2)
                        v167 = AI_MainActor[(uint32_t)v167 - 1];
                    AI_EvalFunc_OBJ_TextFlagSet_C(v167, 20, 0x400);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextFlagSet_C(0, 20, 0x400);
                }
                v166 = (tGameObjectMain*)v415->texte;
                if (v166)
                {
                    if ((unsigned int)v166 <= 2)
                        v166 = AI_MainActor[(uint32_t)v166 - 1];
                    AI_EvalFunc_OBJ_TextFlagSet_C(v166, 21, 0x400);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextFlagSet_C(0, 21, 0x400);
                }
            }
            v165 = (tGameObjectMain*)v415->texte;
            if (v165)
            {
                if ((unsigned int)v165 <= 2)
                    v165 = AI_MainActor[(uint32_t)v165 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v165, 0, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 0, 0, -1, 0x7F7F7F7F);
            }
            v164 = (tGameObjectMain*)v415->texte;
            if (v164)
            {
                if ((unsigned int)v164 <= 2)
                    v164 = AI_MainActor[(uint32_t)v164 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v164, 1, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 1, 0, -1, 0x7F7F7F7F);
            }
            v163 = (tGameObjectMain*)v415->texte;
            if (v163)
            {
                if ((unsigned int)v163 <= 2)
                    v163 = AI_MainActor[(uint32_t)v163 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v163, 2, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 2, 0, -1, 0x7F7F7F7F);
            }
            v162 = (tGameObjectMain*)v415->texte;
            if (v162)
            {
                if ((unsigned int)v162 <= 2)
                    v162 = AI_MainActor[(uint32_t)v162 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v162, 3, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 3, 0, -1, 0x7F7F7F7F);
            }
            v161 = (tGameObjectMain*)v415->texte;
            if (v161)
            {
                if ((unsigned int)v161 <= 2)
                    v161 = AI_MainActor[(uint32_t)v161 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v161, v415->i_CurrentLine, 0, -1, -1);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, v415->i_CurrentLine, 0, -1, -1);
            }
            v160 = (tGameObjectMain*)v415->texte;
            if (v160)
            {
                if ((unsigned int)v160 <= 2)
                    v160 = AI_MainActor[(uint32_t)v160 - 1];
                AI_EvalFunc_OBJ_TextPivotGet_C(v160, v415->i_CurrentLine, &v430);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotGet_C(0, v415->i_CurrentLine, &v430);
            }
            v308.x = 0.02f;
            v308.y = 0.0f;
            v308.z = 0.0f;
            MATH_SubstractVec(&v426, &v430, &v308);
            v339 = v426;
            v159 = (tGameObjectMain*)v415->texte;
            if (v159)
            {
                if ((unsigned int)v159 <= 2)
                    v159 = AI_MainActor[(uint32_t)v159 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v159, 20, &v339);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 20, &v339);
            }
            v158 = (tGameObjectMain*)v415->texte;
            if (v158)
            {
                if ((unsigned int)v158 <= 2)
                    v158 = AI_MainActor[(uint32_t)v158 - 1];
                AI_EvalFunc_OBJ_TextSizeGet_C(v158, v415->i_CurrentLine, 0, -1, &v315);
            }
            else
            {
                AI_EvalFunc_OBJ_TextSizeGet_C(0, v415->i_CurrentLine, 0, -1, &v315);
            }
            v339.x = v315.x + 0.039999999f + v339.x;
            v157 = (tGameObjectMain*)v415->texte;
            if (v157)
            {
                if ((unsigned int)v157 <= 2)
                    v157 = AI_MainActor[(uint32_t)v157 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v157, 21, &v339);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 21, &v339);
            }
            if (v415->f_Time == 1.0)
                v415->i_CurrentState = 402;
        }
        if (v415->i_CurrentState == 402)
        {
            AI_EvalFunc_TEXTGet_C(1224801733, 0, &v341);
            memcpy(&v360, &v341, sizeof(v360));
            v156 = (tGameObjectMain*)v415->texte;
            if (v156)
            {
                if ((unsigned int)v156 <= 2)
                    v156 = AI_MainActor[(uint32_t)v156 - 1];
                STR_Reset(v156);
            }
            else
            {
                STR_Reset(0);
            }
            memcpy(&v311, &v360, sizeof(v311));
            v311.idx += 78;
            v391.x = 0.31f;
            v391.y = 0.60799998f;
            v391.z = 0.0f;
            v155 = (tGameObjectMain*)v415->texte;
            if (v155)
            {
                if ((unsigned int)v155 <= 2)
                    v155 = AI_MainActor[(uint32_t)v155 - 1];
                v154 = v155;
            }
            else
            {
                v154 = 0;
            }
            v11 = AI_EvalFunc_TEXTToString_C(&v311);
            STR_SetStringExt(v154, 0, 2031636, v11, &v391);
            memcpy(&v347, &v360, sizeof(v347));
            v347.idx += 77;
            v307.x = 0.55000001f;
            v307.y = 0.68800002f;
            v307.z = 0.0f;
            v153 = (tGameObjectMain*)v415->texte;
            if (v153)
            {
                if ((unsigned int)v153 <= 2)
                    v153 = AI_MainActor[(uint32_t)v153 - 1];
                v152 = v153;
            }
            else
            {
                v152 = 0;
            }
            v12 = AI_EvalFunc_TEXTToString_C(&v347);
            STR_SetStringExt(v152, 1, 2031636, v12, &v307);
            memcpy(&v437, &v360, sizeof(v437));
            v437.idx += 76;
            v390.x = 0.31f;
            v390.y = 0.75999999f;
            v390.z = 0.0f;
            v151 = (tGameObjectMain*)v415->texte;
            if (v151)
            {
                if ((unsigned int)v151 <= 2)
                    v151 = AI_MainActor[(uint32_t)v151 - 1];
                v150 = v151;
            }
            else
            {
                v150 = 0;
            }
            v13 = AI_EvalFunc_TEXTToString_C(&v437);
            STR_SetStringExt(v150, 2, 2031636, v13, &v390);
            memcpy(&v350, &v360, sizeof(v350));
            v350.idx += 60;
            v306.x = 0.55000001f;
            v306.y = 0.83499998f;
            v306.z = 0.0f;
            v149 = (tGameObjectMain*)v415->texte;
            if (v149)
            {
                if ((unsigned int)v149 <= 2)
                    v149 = AI_MainActor[(uint32_t)v149 - 1];
                v148 = v149;
            }
            else
            {
                v148 = 0;
            }
            v14 = AI_EvalFunc_TEXTToString_C(&v350);
            STR_SetStringExt(v148, 3, 2031636, v14, &v306);
            v147 = (tGameObjectMain*)v415->texte;
            if (v147)
            {
                if ((unsigned int)v147 <= 2)
                    v147 = AI_MainActor[(uint32_t)v147 - 1];
                STR_SetStringExt(v147, 20, 18808854, (char*)AI_gpst_CurrentFunction->localStringConstsData + 26, &v339);
            }
            else
            {
                STR_SetStringExt(0, 20, 18808854, (char*)AI_gpst_CurrentFunction->localStringConstsData + 26, &v339);
            }
            v146 = (tGameObjectMain*)v415->texte;
            if (v146)
            {
                if ((unsigned int)v146 <= 2)
                    v146 = AI_MainActor[(uint32_t)v146 - 1];
                STR_SetStringExt(v146, 21, 18808852, (char*)AI_gpst_CurrentFunction->localStringConstsData + 42, &v339);
            }
            else
            {
                STR_SetStringExt(0, 21, 18808852, (char*)AI_gpst_CurrentFunction->localStringConstsData + 42, &v339);
            }
            v415->i_CurrentState = 403;
            v415->i_CurrentLine = 0;
            v415->i_WaitZeroJoy = 1;
            v415->f_Time = 1.0;
        }
        if (v415->i_CurrentState == 403)
        {
            v440 = TIM_gf_dt;
            v415->f_Time = TIM_gf_dt * 8.0f + v415->f_Time;
            if (v415->f_Time > 4.0 && v415->f_Time < 5.0)
                v415->f_Time = v415->f_Time + 1.0f;
            if (v415->f_Time > 6.0)
                v415->f_Time = 6.0f;
            v145 = v415->f_Time - 5.0f;
            if (v145 < 0.0)
                v41 = 0.0f;
            else
                v41 = v415->f_Time - 5.0f;
            v398 = v41;
            v423 = 1.0f - v41;
            v343.x = v423 + 0.55000001f;
            v343.y = 0.73900002f;
            v343.z = 0.0f;
            v144 = (tGameObjectMain*)v415->texte;
            if (v144)
            {
                if ((unsigned int)v144 <= 2)
                    v144 = AI_MainActor[(uint32_t)v144 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v144, 3, &v343);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 3, &v343);
            }
            v143 = v415->f_Time - 3.0f;
            if (v143 < 0.0)
                v40 = 0.0f;
            else
                v40 = v415->f_Time - 3.0f;
            v323 = v40;
            if (v40 > 1.0)
                v39 = 1.0f;
            else
                v39 = v323;
            v280 = v39;
            v423 = 1.0f - v39;
            v402.x = v423 + 0.31f;
            v402.y = 0.66900003f;
            v402.z = 0.0f;
            v142 = (tGameObjectMain*)v415->texte;
            if (v142)
            {
                if ((unsigned int)v142 <= 2)
                    v142 = AI_MainActor[(uint32_t)v142 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v142, 2, &v402);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 2, &v402);
            }
            v141 = v415->f_Time - 2.0f;
            if (v141 < 0.0)
                v38 = 0.0f;
            else
                v38 = v415->f_Time - 2.0f;
            v361 = v38;
            if (v38 > 1.0)
                v37 = 1.0f;
            else
                v37 = v361;
            v325 = v37;
            v423 = 1.0f - v37;
            v272.x = v423 + 0.31f;
            v272.y = 0.53399998f;
            v272.z = 0.0f;
            v140 = (tGameObjectMain*)v415->texte;
            if (v140)
            {
                if ((unsigned int)v140 <= 2)
                    v140 = AI_MainActor[(uint32_t)v140 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v140, 0, &v272);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 0, &v272);
            }
            v139 = v415->f_Time - 1.0f;
            if (v139 < 0.0)
                v36 = 0.0;
            else
                v36 = v415->f_Time - 1.0f;
            v408 = v36;
            if (v36 > 1.0)
                v35 = 1.0f;
            else
                v35 = v408;
            v363 = v35;
            v423 = 1.0f - v35;
            v313.x = v423 + 0.55000001f;
            v313.y = 0.602f;
            v313.z = 0.0f;
            v138 = (tGameObjectMain*)v415->texte;
            if (v138)
            {
                if ((unsigned int)v138 <= 2)
                    v138 = AI_MainActor[(uint32_t)v138 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v138, 1, &v313);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 1, &v313);
            }
            v137 = (tGameObjectMain*)v415->o_MenuSel;
            if (v137)
            {
                if ((unsigned int)v137 <= 2)
                    v137 = AI_MainActor[(uint32_t)v137 - 1];
                AI_EvalFunc_OBJ_SetChannelProg_C(v137, 0, v415->f_Time);
            }
            else
            {
                AI_EvalFunc_OBJ_SetChannelProg_C(0, 0, v415->f_Time);
            }
            if (v415->f_Time == 6.0)
            {
                v136 = (tGameObjectMain*)v415->texte;
                if (v136)
                {
                    if ((unsigned int)v136 <= 2)
                        v136 = AI_MainActor[(uint32_t)v136 - 1];
                    AI_EvalFunc_OBJ_TextFlagSet_C(v136, 20, 0x16);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextFlagSet_C(0, 20, 0x16);
                }
                v135 = (tGameObjectMain*)v415->texte;
                if (v135)
                {
                    if ((unsigned int)v135 <= 2)
                        v135 = AI_MainActor[(uint32_t)v135 - 1];
                    AI_EvalFunc_OBJ_TextFlagSet_C(v135, 21, 0x14);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextFlagSet_C(0, 21, 0x14);
                }
            }
            else
            {
                v134 = (tGameObjectMain*)v415->texte;
                if (v134)
                {
                    if ((unsigned int)v134 <= 2)
                        v134 = AI_MainActor[(uint32_t)v134 - 1];
                    AI_EvalFunc_OBJ_TextFlagSet_C(v134, 20, 0x400);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextFlagSet_C(0, 20, 0x400);
                }
                v133 = (tGameObjectMain*)v415->texte;
                if (v133)
                {
                    if ((unsigned int)v133 <= 2)
                        v133 = AI_MainActor[(uint32_t)v133 - 1];
                    AI_EvalFunc_OBJ_TextFlagSet_C(v133, 21, 0x400);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextFlagSet_C(0, 21, 0x400);
                }
            }
            v132 = (tGameObjectMain*)v415->texte;
            if (v132)
            {
                if ((unsigned int)v132 <= 2)
                    v132 = AI_MainActor[(uint32_t)v132 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v132, 0, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 0, 0, -1, 0x7F7F7F7F);
            }
            v131 = (tGameObjectMain*)v415->texte;
            if (v131)
            {
                if ((unsigned int)v131 <= 2)
                    v131 = AI_MainActor[(uint32_t)v131 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v131, 1, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 1, 0, -1, 0x7F7F7F7F);
            }
            v130 = (tGameObjectMain*)v415->texte;
            if (v130)
            {
                if ((unsigned int)v130 <= 2)
                    v130 = AI_MainActor[(uint32_t)v130 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v130, 2, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 2, 0, -1, 0x7F7F7F7F);
            }
            v129 = (tGameObjectMain*)v415->texte;
            if (v129)
            {
                if ((unsigned int)v129 <= 2)
                    v129 = AI_MainActor[(uint32_t)v129 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v129, 3, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 3, 0, -1, 0x7F7F7F7F);
            }
            v128 = (tGameObjectMain*)v415->texte;
            if (v128)
            {
                if ((unsigned int)v128 <= 2)
                    v128 = AI_MainActor[(uint32_t)v128 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v128, v415->i_CurrentLine, 0, -1, -1);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, v415->i_CurrentLine, 0, -1, -1);
            }
            v127 = (tGameObjectMain*)v415->texte;
            if (v127)
            {
                if ((unsigned int)v127 <= 2)
                    v127 = AI_MainActor[(uint32_t)v127 - 1];
                AI_EvalFunc_OBJ_TextPivotGet_C(v127, v415->i_CurrentLine, &v270);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotGet_C(0, v415->i_CurrentLine, &v270);
            }
            v290.x = 0.02f;
            v290.y = 0.0f;
            v290.z = 0.0f;
            MATH_SubstractVec(&v413, &v270, &v290);
            v339 = v413;
            v126 = (tGameObjectMain*)v415->texte;
            if (v126)
            {
                if ((unsigned int)v126 <= 2)
                    v126 = AI_MainActor[(uint32_t)v126 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v126, 20, &v339);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 20, &v339);
            }
            v125 = (tGameObjectMain*)v415->texte;
            if (v125)
            {
                if ((unsigned int)v125 <= 2)
                    v125 = AI_MainActor[(uint32_t)v125 - 1];
                AI_EvalFunc_OBJ_TextSizeGet_C(v125, v415->i_CurrentLine, 0, -1, &v371);
            }
            else
            {
                AI_EvalFunc_OBJ_TextSizeGet_C(0, v415->i_CurrentLine, 0, -1, &v371);
            }
            v339.x = v371.x + 0.039999999f + v339.x;
            v124 = (tGameObjectMain*)v415->texte;
            if (v124)
            {
                if ((unsigned int)v124 <= 2)
                    v124 = AI_MainActor[(uint32_t)v124 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v124, 21, &v339);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 21, &v339);
            }
            if (v415->f_Time == 6.0)
                v415->i_CurrentState = 404;
            v415->f_Chrono = TIM_gf_MainClock - TIM_gf_LockTime;
        }
        if (v415->i_CurrentState == 404)
        {
            if (1.0 < 0.0)
                v34 = 0.0f;
            else
                v34 = 1.0f;
            v297 = v34;
            v423 = 1.0f - v34;
            v410.x = v423 + 0.55000001f;
            v410.y = 0.73900002f;
            v410.z = 0.0f;
            v123 = (tGameObjectMain*)v415->texte;
            if (v123)
            {
                if ((unsigned int)v123 <= 2)
                    v123 = AI_MainActor[(uint32_t)v123 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v123, 3, &v410);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 3, &v410);
            }
            if (3.0 < 0.0)
                v33 = 0.0;
            else
                v33 = 3.0f;
            v383 = v33;
            if (v33 > 1.0)
                v32 = 1.0f;
            else
                v32 = v383;
            v336 = v32;
            v423 = 1.0f - v32;
            v286.x = v423 + 0.31f;
            v286.y = 0.66900003f;
            v286.z = 0.0f;
            v122 = (tGameObjectMain*)v415->texte;
            if (v122)
            {
                if ((unsigned int)v122 <= 2)
                    v122 = AI_MainActor[(uint32_t)v122 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v122, 2, &v286);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 2, &v286);
            }
            if (4.0 < 0.0)
                v31 = 0.0f;
            else
                v31 = 4.0f;
            v421 = v31;
            if (v31 > 1.0)
                v30 = 1.0f;
            else
                v30 = v421;
            v417 = v30;
            v423 = 1.0f - v30;
            v370.x = v423 + 0.31f;
            v370.y = 0.53399998f;
            v370.z = 0.0f;
            v121 = (tGameObjectMain*)v415->texte;
            if (v121)
            {
                if ((unsigned int)v121 <= 2)
                    v121 = AI_MainActor[(uint32_t)v121 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v121, 0, &v370);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 0, &v370);
            }
            if (5.0 < 0.0)
                v29 = 0.0f;
            else
                v29 = 5.0f;
            v334 = v29;
            if (v29 > 1.0)
                v28 = 1.0f;
            else
                v28 = v334;
            v295 = v28;
            v423 = 1.0f - v28;
            v414.x = v423 + 0.55000001f;
            v414.y = 0.602f;
            v414.z = 0.0f;
            v120 = (tGameObjectMain*)v415->texte;
            if (v120)
            {
                if ((unsigned int)v120 <= 2)
                    v120 = AI_MainActor[(uint32_t)v120 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v120, 1, &v414);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 1, &v414);
            }
            v119 = (tGameObjectMain*)v415->o_MenuSel;
            if (v119)
            {
                if ((unsigned int)v119 <= 2)
                    v119 = AI_MainActor[(uint32_t)v119 - 1];
                AI_EvalFunc_OBJ_SetChannelProg_C(v119, 0, 6.0f);
            }
            else
            {
                AI_EvalFunc_OBJ_SetChannelProg_C(0, 0, 6.0f);
            }
            v118 = (tGameObjectMain*)v415->texte;
            if (v118)
            {
                if ((unsigned int)v118 <= 2)
                    v118 = AI_MainActor[(uint32_t)v118 - 1];
                AI_EvalFunc_OBJ_TextFlagSet_C(v118, 20, 0x16);
            }
            else
            {
                AI_EvalFunc_OBJ_TextFlagSet_C(0, 20, 0x16);
            }
            v117 = (tGameObjectMain*)v415->texte;
            if (v117)
            {
                if ((unsigned int)v117 <= 2)
                    v117 = AI_MainActor[(uint32_t)v117 - 1];
                AI_EvalFunc_OBJ_TextFlagSet_C(v117, 21, 0x14);
            }
            else
            {
                AI_EvalFunc_OBJ_TextFlagSet_C(0, 21, 0x14);
            }
            v116 = (tGameObjectMain*)v415->texte;
            if (v116)
            {
                if ((unsigned int)v116 <= 2)
                    v116 = AI_MainActor[(uint32_t)v116 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v116, 0, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 0, 0, -1, 0x7F7F7F7F);
            }
            v115 = (tGameObjectMain*)v415->texte;
            if (v115)
            {
                if ((unsigned int)v115 <= 2)
                    v115 = AI_MainActor[(uint32_t)v115 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v115, 1, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 1, 0, -1, 0x7F7F7F7F);
            }
            v114 = (tGameObjectMain*)v415->texte;
            if (v114)
            {
                if ((unsigned int)v114 <= 2)
                    v114 = AI_MainActor[(uint32_t)v114 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v114, 2, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 2, 0, -1, 0x7F7F7F7F);
            }
            v113 = (tGameObjectMain*)v415->texte;
            if (v113)
            {
                if ((unsigned int)v113 <= 2)
                    v113 = AI_MainActor[(uint32_t)v113 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v113, 3, 0, -1, 0x7F7F7F7F);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, 3, 0, -1, 0x7F7F7F7F);
            }
            v112 = (tGameObjectMain*)v415->texte;
            if (v112)
            {
                if ((unsigned int)v112 <= 2)
                    v112 = AI_MainActor[(uint32_t)v112 - 1];
                AI_EvalFunc_OBJ_TextColorSet_C(v112, v415->i_CurrentLine, 0, -1, -1);
            }
            else
            {
                AI_EvalFunc_OBJ_TextColorSet_C(0, v415->i_CurrentLine, 0, -1, -1);
            }
            v111 = (tGameObjectMain*)v415->texte;
            if (v111)
            {
                if ((unsigned int)v111 <= 2)
                    v111 = AI_MainActor[(uint32_t)v111 - 1];
                AI_EvalFunc_OBJ_TextPivotGet_C(v111, v415->i_CurrentLine, &v369);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotGet_C(0, v415->i_CurrentLine, &v369);
            }
            v332.x = 0.02f;
            v332.y = 0.0f;
            v332.z = 0.0f;
            MATH_SubstractVec(&v287, &v369, &v332);
            v339 = v287;
            v110 = (tGameObjectMain*)v415->texte;
            if (v110)
            {
                if ((unsigned int)v110 <= 2)
                    v110 = AI_MainActor[(uint32_t)v110 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v110, 20, &v339);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 20, &v339);
            }
            v109 = (tGameObjectMain*)v415->texte;
            if (v109)
            {
                if ((unsigned int)v109 <= 2)
                    v109 = AI_MainActor[(uint32_t)v109 - 1];
                AI_EvalFunc_OBJ_TextSizeGet_C(v109, v415->i_CurrentLine, 0, -1, &v412);
            }
            else
            {
                AI_EvalFunc_OBJ_TextSizeGet_C(0, v415->i_CurrentLine, 0, -1, &v412);
            }
            v339.x = v412.x + 0.039999999f + v339.x;
            v108 = (tGameObjectMain*)v415->texte;
            if (v108)
            {
                if ((unsigned int)v108 <= 2)
                    v108 = AI_MainActor[(uint32_t)v108 - 1];
                AI_EvalFunc_OBJ_TextPivotSet_C(v108, 21, &v339);
            }
            else
            {
                AI_EvalFunc_OBJ_TextPivotSet_C(0, 21, &v339);
            }
            AI_EvalFunc_IoJoyGetMove_C(&v366);
            v274 = v366.y;
            if (v415->i_WaitZeroJoy)
            {
                v27 = (float)fabs(v274);
                v321 = v27;
                if (v27 < 0.5)
                    v415->i_WaitZeroJoy = 0;
            }
            else
            {
                idx = v415->i_CurrentLine;
                if (v274 > 0.69999999 && v415->i_CurrentLine > 0)
                    --v415->i_CurrentLine;
                if (v274 < -0.69999999 && v415->i_CurrentLine < 3)
                    ++v415->i_CurrentLine;
                if (idx != v415->i_CurrentLine)
                {
                    v365 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 5, 2176);
                    v415->i_WaitZeroJoy = 1;
                    v415->f_Chrono = TIM_gf_MainClock - TIM_gf_LockTime;
                }
            }
            if (AI_EvalFunc_TIMEElapsed_C(v415->f_Chrono, 30.0))
            {
                v415->i_ModePause = 3;
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 1000;
            }
            else
            {
                if (AI_EvalFunc_IoButtonJustPressed_C(0))
                {
                    if (v415->i_CurrentLine == 3)
                        v358 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 7, 2176);
                    else
                        v405 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 6, 2176);
                    v415->i_CurrentState = 100 * (v415->i_CurrentLine + 5);
                }
                if (AI_EvalFunc_IoButtonJustPressed_C(3))
                {
                    v324 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 7, 2176);
                    v415->i_CurrentState = 800;
                }
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 1000;
            }
        }
        else if (v415->i_CurrentState == 500)
        {
            v415->i_ModePause = 4;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else if (v415->i_CurrentState == 600)
        {
            v415->i_ModePause = 5;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else if (v415->i_CurrentState == 700)
        {
            v399 = main_logo_ml_init2_INV_Manager_gao;
            v107 = main_logo_ml_init2_INV_Manager_gao;
            if (main_logo_ml_init2_INV_Manager_gao)
            {
                if ((unsigned int)main_logo_ml_init2_INV_Manager_gao <= 2)
                    v107 = AI_MainActor[(uint32_t)main_logo_ml_init2_INV_Manager_gao - 1];
                v15 = AI_EvalFunc_GFXAddSorted_C(v107, 19, 5);
            }
            else
            {
                v15 = AI_EvalFunc_GFXAddSorted_C(0, 19, 5);
            }
            v376 = v15;
            v106 = v399;
            if (v399)
            {
                if ((unsigned int)v399 <= 2)
                    v106 = AI_MainActor[(uint32_t)v399 - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v376, v106, 1);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v376, 0, 1);
            }
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v376, 2, 1);
            for (a2 = 0; a2 < 110; ++a2)
            {
                if (*(uint32_t*)(main_logo_ml_init2_i_indice_de_map + 4 * a2 + 4) == -1)
                {
                    AI_EvalFunc_TEXTGet_C(0x2600063E, a2, &v300);
                    v105 = v399;
                    if (v399)
                    {
                        if ((unsigned int)v399 <= 2)
                            v105 = AI_MainActor[(uint32_t)v399 - 1];
                        v104 = v105;
                    }
                    else
                    {
                        v104 = 0;
                    }
                    v16 = AI_EvalFunc_TEXTToString_C(&v300);
                    STR_SetString(v104, 30, v16);
                    v103 = v399;
                    if (v399)
                    {
                        if ((unsigned int)v399 <= 2)
                            v103 = AI_MainActor[(uint32_t)v399 - 1];
                        STR_ToUpper(v103, 30, 0, -1);
                    }
                    else
                    {
                        STR_ToUpper(0, 30, 0, -1);
                    }
                    v102 = v399;
                    if (v399)
                    {
                        if ((unsigned int)v399 <= 2)
                            v102 = AI_MainActor[(uint32_t)v399 - 1];
                        AI_EvalFunc_OBJ_TextPageSet_C(v102, 30, 0, -1, 1, -1);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_TextPageSet_C(0, 30, 0, -1, 1, -1);
                    }
                    v327.x = 0.5f;
                    v327.y = 0.5f;
                    v327.z = 0.0f;
                    v101 = v399;
                    if (v399)
                    {
                        if ((unsigned int)v399 <= 2)
                            v101 = AI_MainActor[(uint32_t)v399 - 1];
                        STR_SetStringExt(v101, 30, 1441813, (char*)AI_gpst_CurrentFunction->localStringConstsData + 49, &v327);
                    }
                    else
                    {
                        STR_SetStringExt(0, 30, 1441813, (char*)AI_gpst_CurrentFunction->localStringConstsData + 49, &v327);
                    }
                    AI_EvalFunc_GFXSeti_C_CURRENT(v376, 19100, 30);
                    v100 = v399;
                    if (v399)
                    {
                        if ((unsigned int)v399 <= 2)
                            v100 = AI_MainActor[(uint32_t)v399 - 1];
                        STR_SetString(v100, 30, (char*)AI_gpst_CurrentFunction->localStringConstsData + 50);
                    }
                    else
                    {
                        STR_SetString(0, 30, (char*)AI_gpst_CurrentFunction->localStringConstsData + 50);
                    }
                    break;
                }
            }
            v99 = v399;
            if (v399)
            {
                if ((unsigned int)v399 <= 2)
                    v99 = AI_MainActor[(uint32_t)v399 - 1];
                v17 = AI_EvalFunc_GFXAddSorted_C(v99, 19, 6);
            }
            else
            {
                v17 = AI_EvalFunc_GFXAddSorted_C(0, 19, 6);
            }
            v376 = v17;
            v98 = v399;
            if (v399)
            {
                if ((unsigned int)v399 <= 2)
                    v98 = AI_MainActor[(uint32_t)v399 - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v376, v98, 0);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v376, 0, 0);
            }
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v376, 2, 1);
            AI_EvalFunc_TEXTGet_C(1224883814, 17, &v422);
            v285.x = 0.5f;
            v285.y = 0.80000001f;
            v285.z = 0.0f;
            v97 = v399;
            if (v399)
            {
                if ((unsigned int)v399 <= 2)
                    v97 = AI_MainActor[(uint32_t)v399 - 1];
                v96 = v97;
            }
            else
            {
                v96 = 0;
            }
            v18 = AI_EvalFunc_TEXTToString_C(&v422);
            STR_SetStringExt(v96, 31, 1507349, v18, &v285);
            AI_EvalFunc_GFXSeti_C_CURRENT(v376, 19100, 31);
            v95 = v399;
            if (v399)
            {
                if ((unsigned int)v399 <= 2)
                    v95 = AI_MainActor[(uint32_t)v399 - 1];
                STR_SetString(v95, 31, (char*)AI_gpst_CurrentFunction->localStringConstsData + 51);
            }
            else
            {
                STR_SetString(0, 31, (char*)AI_gpst_CurrentFunction->localStringConstsData + 51);
            }
            AI_EvalFunc_GFXDel_C_CURRENT(10);
            idx = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(idx, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(idx, 2, 1);
            v94 = (tGameObjectMain*)main_logo_ml_init2_INV_MatFade_gao;
            if (main_logo_ml_init2_INV_MatFade_gao)
            {
                if ((unsigned int)main_logo_ml_init2_INV_MatFade_gao <= 2)
                    v94 = AI_MainActor[main_logo_ml_init2_INV_MatFade_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(idx, v94, 9);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(idx, 0, 9);
            }
            GFXSetCurGlobal(idx);
            AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(11101, -16777216);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 0.2f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 1.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
            *AI_gppst_CurrentJumpNode = (tAINode*)69;
            TIME_Wait_C(0.40000001f);
            result = 0;
        }
        else
        {
            if (v415->i_CurrentState == 800)
            {
                v415->f_Time = 6.0f;
                v415->i_CurrentState = 801;
            }
            if (v415->i_CurrentState == 801)
            {
                v419 = TIM_gf_dt;
                v415->f_Time = v415->f_Time - TIM_gf_dt * 8.0f;
                if (v415->f_Time < 5.0 && v415->f_Time > 4.0)
                    v415->f_Time = v415->f_Time - 1.0f;
                if (v415->f_Time < 1.0)
                    v415->f_Time = 1.0f;
                v93 = v415->f_Time - 5.0f;
                if (v93 < 0.0)
                    v26 = 0.0;
                else
                    v26 = v415->f_Time - 5.0f;
                v298 = v26;
                v423 = 1.0f - v26;
                v329.x = v423 + 0.55000001f;
                v329.y = 0.73900002f;
                v329.z = 0.0f;
                v92 = (tGameObjectMain*)v415->texte;
                if (v92)
                {
                    if ((unsigned int)v92 <= 2)
                        v92 = AI_MainActor[(uint32_t)v92 - 1];
                    AI_EvalFunc_OBJ_TextPivotSet_C(v92, 3, &v329);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextPivotSet_C(0, 3, &v329);
                }
                v91 = v415->f_Time - 3.0f;
                if (v91 < 0.0)
                    v25 = 0.0f;
                else
                    v25 = v415->f_Time - 3.0f;
                v416 = v25;
                if (v25 > 1.0)
                    v24 = 1.0f;
                else
                    v24 = v416;
                v293 = v24;
                v423 = 1.0f - v24;
                v331.x = v423 + 0.31f;
                v331.y = 0.66900003f;
                v331.z = 0.0f;
                v90 = (tGameObjectMain*)v415->texte;
                if (v90)
                {
                    if ((unsigned int)v90 <= 2)
                        v90 = AI_MainActor[(uint32_t)v90 - 1];
                    AI_EvalFunc_OBJ_TextPivotSet_C(v90, 2, &v331);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextPivotSet_C(0, 2, &v331);
                }
                v89 = v415->f_Time - 2.0f;
                if (v89 < 0.0)
                    v23 = 0.0f;
                else
                    v23 = v415->f_Time - 2.0f;
                v374 = v23;
                if (v23 > 1.0)
                    v22 = 1.0f;
                else
                    v22 = v374;
                v418 = v22;
                v423 = 1.0f - v22;
                v291.x = v423 + 0.31f;
                v291.y = 0.53399998f;
                v291.z = 0.0f;
                v88 = (tGameObjectMain*)v415->texte;
                if (v88)
                {
                    if ((unsigned int)v88 <= 2)
                        v88 = AI_MainActor[(uint32_t)v88 - 1];
                    AI_EvalFunc_OBJ_TextPivotSet_C(v88, 0, &v291);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextPivotSet_C(0, 0, &v291);
                }
                v87 = v415->f_Time - 1.0f;
                if (v87 < 0.0)
                    v21 = 0.0f;
                else
                    v21 = v415->f_Time - 1.0f;
                v335 = v21;
                if (v21 > 1.0)
                    v20 = 1.0f;
                else
                    v20 = v335;
                v379 = v20;
                v423 = 1.0f - v20;
                v411.x = v423 + 0.55000001f;
                v411.y = 0.602f;
                v411.z = 0.0f;
                v86 = (tGameObjectMain*)v415->texte;
                if (v86)
                {
                    if ((unsigned int)v86 <= 2)
                        v86 = AI_MainActor[(uint32_t)v86 - 1];
                    AI_EvalFunc_OBJ_TextPivotSet_C(v86, 1, &v411);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextPivotSet_C(0, 1, &v411);
                }
                v85 = (tGameObjectMain*)v415->o_MenuSel;
                if (v85)
                {
                    if ((unsigned int)v85 <= 2)
                        v85 = AI_MainActor[(uint32_t)v85 - 1];
                    AI_EvalFunc_OBJ_SetChannelProg_C(v85, 0, v415->f_Time);
                }
                else
                {
                    AI_EvalFunc_OBJ_SetChannelProg_C(0, 0, v415->f_Time);
                }
                if (v415->f_Time == 6.0)
                {
                    v84 = (tGameObjectMain*)v415->texte;
                    if (v84)
                    {
                        if ((unsigned int)v84 <= 2)
                            v84 = AI_MainActor[(uint32_t)v84 - 1];
                        AI_EvalFunc_OBJ_TextFlagSet_C(v84, 20, 22);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_TextFlagSet_C(0, 20, 22);
                    }
                    v83 = (tGameObjectMain*)v415->texte;
                    if (v83)
                    {
                        if ((unsigned int)v83 <= 2)
                            v83 = AI_MainActor[(uint32_t)v83 - 1];
                        AI_EvalFunc_OBJ_TextFlagSet_C(v83, 21, 20);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_TextFlagSet_C(0, 21, 20);
                    }
                }
                else
                {
                    v82 = (tGameObjectMain*)v415->texte;
                    if (v82)
                    {
                        if ((unsigned int)v82 <= 2)
                            v82 = AI_MainActor[(uint32_t)v82 - 1];
                        AI_EvalFunc_OBJ_TextFlagSet_C(v82, 20, 1024);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_TextFlagSet_C(0, 20, 1024);
                    }
                    v81 = (tGameObjectMain*)v415->texte;
                    if (v81)
                    {
                        if ((unsigned int)v81 <= 2)
                            v81 = AI_MainActor[(uint32_t)v81 - 1];
                        AI_EvalFunc_OBJ_TextFlagSet_C(v81, 21, 1024);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_TextFlagSet_C(0, 21, 1024);
                    }
                }
                v80 = (tGameObjectMain*)v415->texte;
                if (v80)
                {
                    if ((unsigned int)v80 <= 2)
                        v80 = AI_MainActor[(uint32_t)v80 - 1];
                    AI_EvalFunc_OBJ_TextColorSet_C(v80, 0, 0, -1, 0x7F7F7F7F);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextColorSet_C(0, 0, 0, -1, 0x7F7F7F7F);
                }
                v79 = (tGameObjectMain*)v415->texte;
                if (v79)
                {
                    if ((unsigned int)v79 <= 2)
                        v79 = AI_MainActor[(uint32_t)v79 - 1];
                    AI_EvalFunc_OBJ_TextColorSet_C(v79, 1, 0, -1, 0x7F7F7F7F);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextColorSet_C(0, 1, 0, -1, 0x7F7F7F7F);
                }
                v78 = (tGameObjectMain*)v415->texte;
                if (v78)
                {
                    if ((unsigned int)v78 <= 2)
                        v78 = AI_MainActor[(uint32_t)v78 - 1];
                    AI_EvalFunc_OBJ_TextColorSet_C(v78, 2, 0, -1, 0x7F7F7F7F);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextColorSet_C(0, 2, 0, -1, 0x7F7F7F7F);
                }
                v77 = (tGameObjectMain*)v415->texte;
                if (v77)
                {
                    if ((unsigned int)v77 <= 2)
                        v77 = AI_MainActor[(uint32_t)v77 - 1];
                    AI_EvalFunc_OBJ_TextColorSet_C(v77, 3, 0, -1, 0x7F7F7F7F);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextColorSet_C(0, 3, 0, -1, 0x7F7F7F7F);
                }
                v76 = (tGameObjectMain*)v415->texte;
                if (v76)
                {
                    if ((unsigned int)v76 <= 2)
                        v76 = AI_MainActor[(uint32_t)v76 - 1];
                    AI_EvalFunc_OBJ_TextColorSet_C(v76, v415->i_CurrentLine, 0, -1, 0xFFFFFFFF);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextColorSet_C(0, v415->i_CurrentLine, 0, -1, 0xFFFFFFFF);
                }
                v75 = (tGameObjectMain*)v415->texte;
                if (v75)
                {
                    if ((unsigned int)v75 <= 2)
                        v75 = AI_MainActor[(uint32_t)v75 - 1];
                    AI_EvalFunc_OBJ_TextPivotGet_C(v75, v415->i_CurrentLine, &v288);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextPivotGet_C(0, v415->i_CurrentLine, &v288);
                }
                v337.x = 0.02f;
                v337.y = 0.0f;
                v337.z = 0.0f;
                MATH_SubstractVec(&v386, &v288, &v337);
                v339 = v386;
                v74 = (tGameObjectMain*)v415->texte;
                if (v74)
                {
                    if ((unsigned int)v74 <= 2)
                        v74 = AI_MainActor[(uint32_t)v74 - 1];
                    AI_EvalFunc_OBJ_TextPivotSet_C(v74, 20, &v339);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextPivotSet_C(0, 20, &v339);
                }
                v73 = (tGameObjectMain*)v415->texte;
                if (v73)
                {
                    if ((unsigned int)v73 <= 2)
                        v73 = AI_MainActor[(uint32_t)v73 - 1];
                    AI_EvalFunc_OBJ_TextSizeGet_C(v73, v415->i_CurrentLine, 0, -1, &v429);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextSizeGet_C(0, v415->i_CurrentLine, 0, -1, &v429);
                }
                v339.x = v429.x + 0.039999999f + v339.x;
                v72 = (tGameObjectMain*)v415->texte;
                if (v72)
                {
                    if ((unsigned int)v72 <= 2)
                        v72 = AI_MainActor[(uint32_t)v72 - 1];
                    AI_EvalFunc_OBJ_TextPivotSet_C(v72, 21, &v339);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextPivotSet_C(0, 21, &v339);
                }
                if (v415->f_Time == 1.0)
                    v415->i_CurrentState = 0;
            }
            result = 1000;
        }
    }
    return result;
}

// language selection
int main_logo_ml_init1()
{
    int result;
    BOOL v1;
    char* v2;
    int v3;
    char* v4;
    int v5;
    char* v6;
    char* v7;
    char* v8;
    char* v9;
    char* v10;
    char* v11;
    BOOL v12;
    float a3;
    Vec3* a3a;
    float a3b;
    float v16;
    float v17;
    float v18;
    tScript_mlStruct* v19;
    tGameObjectMain* v20;
    tGameObjectMain* v21;
    tGameObjectMain* v22;
    tGameObjectMain* v23;
    tGameObjectMain* v24;
    tGameObjectMain* v25;
    tGameObjectMain* v26;
    tGameObjectMain* v27;
    tGameObjectMain* v28;
    tGameObjectMain* v29;
    tGameObjectMain* v30;
    tGameObjectMain* v31;
    tGameObjectMain* v32;
    tGameObjectMain* v33;
    tGameObjectMain* v34;
    tGameObjectMain* v35;
    tGameObjectMain* v36;
    tGameObjectMain* v37;
    tGameObjectMain* v38;
    tGameObjectMain* v39;
    tGameObjectMain* v40;
    tGameObjectMain* v41;
    tGameObjectMain* v42;
    tGameObjectMain* v43;
    tGameObjectMain* v44;
    tGameObjectMain* v45;
    tGameObjectMain* v46;
    tGameObjectMain* v47;
    tGameObjectMain* v48;
    tGameObjectMain* v49;
    tGameObjectMain* v50;
    tGameObjectMain* v51;
    tGameObjectMain* v52;
    tGameObjectMain* v53;
    tGameObjectMain* v54;
    tGameObjectMain* v55;
    tGameObjectMain* v56;
    tGameObjectMain* v57;
    tGameObjectMain* v58;
    tGameObjectMain* v59;
    tGameObjectMain* v60;
    tGameObjectMain* v61;
    tGameObjectMain* v62;
    tGameObjectMain* v63;
    tGameObjectMain* v64;
    tGameObjectMain* v65;
    float v66;
    tGameObjectMain* v67;
    tGameObjectMain* v68;
    tGameObjectMain* v69;
    tGameObjectMain* v70;
    tGameObjectMain* v71;
    tGameObjectMain* v72;
    tGameObjectMain* v73;
    tGameObjectMain* v74;
    tGameObjectMain* v75;
    tGameObjectMain* v76;
    tGameObjectMain* v77;
    tGameObjectMain* v78;
    tGameObjectMain* v79;
    tGameObjectMain* v80;
    tGameObjectMain* v81;
    tGameObjectMain* v82;
    tGameObjectMain* v83;
    tGameObjectMain* v84;
    tGameObjectMain* v85;
    tGameObjectMain* v86;
    tGameObjectMain* v87;
    tGameObjectMain* v88;
    float v89;
    tGameObjectMain* v90;
    tGameObjectMain* v91;
    tGameObjectMain* v92;
    float v93;
    Vec3 v94;
    tTextGet v95;
    float v96;
    Vec3 v97;
    BOOL v98;
    float v99;
    tTextGet v100;
    tTextGet v101;
    tTextGet v102;
    int v103;
    BOOL v104;
    Vec3 v105;
    int v106;
    int v107;
    Vec3 v108;
    int v110;
    Vec3 v111;
    Vec3 v112;
    tTextGet v113;
    Vec3 v114;
    int v115;
    int v117;
    Vec3 v119;
    int v120;
    int v121;
    tTextGet v122;
    int v123;
    float v124;
    int j;
    Vec3 v127;
    Vec3 v128;
    Vec3 v129;
    tScript_mlStruct* v130;
    int i;
    Vec3 a5;
    Vec3 v133;
    Vec3 a2;
    Vec3 v135;
    tTextGet v136;
    tTextGet v137;

    v105.x = 1.0;
    v105.y = 0.0;
    v105.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v19 = (tScript_mlStruct*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v19 = 0;
    v130 = v19;
    v121 = 0;
    v115 = 0;
    j = 0;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_main_logo_ml_init1_main_logo_ml_init2 = AI_ul_RealResolveOneRefFunction(0x49027635);
    }
    if (v130->i_ModePause)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else if (v130->i_CurrentState == -1)
    {
        v92 = (tGameObjectMain*)v130->o_CurEvent;
        if (v92)
        {
            if ((unsigned int)v92 <= 2)
                v92 = AI_MainActor[(uint32_t)v92 - 1];
            v1 = AI_EvalFunc_TRAIsStop_C(v92, 0);
        }
        else
        {
            v1 = AI_EvalFunc_TRAIsStop_C(0, 0);
        }
        v104 = v1;
        if (v1)
        {
            v91 = (tGameObjectMain*)v130->o_CurEvent;
            if (v91)
            {
                if ((unsigned int)v91 <= 2)
                    v91 = AI_MainActor[(uint32_t)v91 - 1];
                AI_EvalFunc_OBJ_FlagInactiveSet_C(v91, 1);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
            }
            v130->i_CurrentState = 0;
            v130->f_Chrono = TIM_gf_MainClock - TIM_gf_LockTime;
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        if (!v130->i_CurrentState)
        {
            if (v130->i_langnb > 1)
            {
                v130->i_CurrentState = 1;
            }
            else
            {
                v120 = AI_EvalFunc_IoGameLangSet_C(v130->i_langval[0]);
                v130->i_CurrentState = 10;
            }
            v130->f_Chrono = TIM_gf_MainClock - TIM_gf_LockTime;
        }
        if (v130->i_CurrentState == 1)
        {
            v96 = TIM_gf_dt;
            v130->f_Time = v130->f_Time - TIM_gf_dt * 4.0f;
            if (v130->f_Time < 0.0)
            {
                v130->f_Time = 0.0;
                v130->i_CurrentState = 2;
            }
            v90 = (tGameObjectMain*)v130->o_LangueTitle;
            if (v90)
            {
                if ((unsigned int)v90 <= 2)
                    v90 = AI_MainActor[(uint32_t)v90 - 1];
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(v90, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
            }
            v89 = v130->f_Time;
            v112.x = v89 * v130->v_LangueTitle_Offset.x;
            v112.y = v130->v_LangueTitle_Offset.y * v89;
            v112.z = v130->v_LangueTitle_Offset.z * v89;
            MATH_VecAddVec(&a2, &v130->v_LangueTitle_Pos, &v112);
            v88 = (tGameObjectMain*)v130->o_LangueTitle;
            if (v88)
            {
                if ((unsigned int)v88 <= 2)
                    v88 = AI_MainActor[(uint32_t)v88 - 1];
                AI_EvalFunc_OBJSetPos_C(v88, &a2);
            }
            else
            {
                AI_EvalFunc_OBJSetPos_C(0, &a2);
            }
            v87 = (tGameObjectMain*)v130->o_LangueSel;
            if (v87)
            {
                if ((unsigned int)v87 <= 2)
                    v87 = AI_MainActor[(uint32_t)v87 - 1];
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(v87, 0);
            }
            else
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 0);
            }
            v86 = (tGameObjectMain*)v130->o_LangueSel;
            if (v86)
            {
                if ((unsigned int)v86 <= 2)
                    v86 = AI_MainActor[(uint32_t)v86 - 1];
                v85 = v86;
            }
            else
            {
                v85 = 0;
            }
            v18 = (float)v130->i_langnb;
            a3 = (1.0f - v130->f_Time) * v18 + 1.0f;
            AI_EvalFunc_OBJ_SetChannelProg_C(v85, 0, a3);
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            if (v130->i_CurrentState == 2)
            {
                v84 = (tGameObjectMain*)v130->texte;
                if (v84)
                {
                    if ((unsigned int)v84 <= 2)
                        v84 = AI_MainActor[(uint32_t)v84 - 1];
                    STR_Reset(v84);
                }
                else
                {
                    STR_Reset(0);
                }
                if (v130->i_langnb == 6)
                {
                    v121 = 0;
                }
                else if (v130->i_langnb < 4)
                {
                    v121 = 2;
                }
                else
                {
                    v121 = 1;
                }
                for (i = 0; i < v130->i_langnb; ++i)
                {
                    memcpy(&v102, &v130->txt_lang, sizeof(v102));
                    v102.idx += v130->i_langval[i];
                    v83 = (tGameObjectMain*)v130->texte;
                    if (v83)
                    {
                        if ((unsigned int)v83 <= 2)
                            v83 = AI_MainActor[(uint32_t)v83 - 1];
                        v82 = v83;
                    }
                    else
                    {
                        v82 = 0;
                    }
                    a3a = (Vec3*)AI_EvalVarArray_C3((char*)&v130->__v_LanguePos_Dimension, i + v121);
                    v2 = AI_EvalFunc_TEXTToString_C(&v102);
                    STR_SetStringExt(v82, i, 1507348, v2, a3a);
                    v81 = (tGameObjectMain*)v130->texte;
                    if (v81)
                    {
                        if ((unsigned int)v81 <= 2)
                            v81 = AI_MainActor[(uint32_t)v81 - 1];
                        AI_EvalFunc_OBJ_TextColorSet_C(v81, i, 0, -1, 0xFF7F7F7F);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_TextColorSet_C(0, i, 0, -1, 0xFF7F7F7F);
                    }
                }
                v80 = (tGameObjectMain*)v130->texte;
                if (v80)
                {
                    if ((unsigned int)v80 <= 2)
                        v80 = AI_MainActor[(uint32_t)v80 - 1];
                    AI_EvalFunc_OBJ_TextPivotGet_C(v80, v130->i_langcur, &v119);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextPivotGet_C(0, v130->i_langcur, &v119);
                }
                v105 = v119;
                v79 = (tGameObjectMain*)v130->texte;
                if (v79)
                {
                    if ((unsigned int)v79 <= 2)
                        v79 = AI_MainActor[(uint32_t)v79 - 1];
                    STR_SetStringExt(v79, 20, 18808854, (char*)AI_gpst_CurrentFunction->localStringConstsData, &v105);
                }
                else
                {
                    STR_SetStringExt(0, 20, 18808854, (char*)AI_gpst_CurrentFunction->localStringConstsData, &v105);
                }
                v78 = (tGameObjectMain*)v130->texte;
                if (v78)
                {
                    if ((unsigned int)v78 <= 2)
                        v78 = AI_MainActor[(uint32_t)v78 - 1];
                    AI_EvalFunc_OBJ_TextSizeGet_C(v78, v130->i_langcur, 0, -1, &a5);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextSizeGet_C(0, v130->i_langcur, 0, -1, &a5);
                }
                v105.x = a5.x + 0.02f + v105.x;
                v77 = (tGameObjectMain*)v130->texte;
                if (v77)
                {
                    if ((unsigned int)v77 <= 2)
                        v77 = AI_MainActor[(uint32_t)v77 - 1];
                    STR_SetStringExt(v77, 21, 18808852, (char*)AI_gpst_CurrentFunction->localStringConstsData + 7, &v105);
                }
                else
                {
                    STR_SetStringExt(0, 21, 18808852, (char*)AI_gpst_CurrentFunction->localStringConstsData + 7, &v105);
                }
                v76 = (tGameObjectMain*)v130->texte;
                if (v76)
                {
                    if ((unsigned int)v76 <= 2)
                        v76 = AI_MainActor[(uint32_t)v76 - 1];
                    AI_EvalFunc_OBJ_TextColorSet_C(v76, v130->i_langcur, 0, -1, -1);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextColorSet_C(0, v130->i_langcur, 0, -1, -1);
                }
                v130->f_Chrono = TIM_gf_MainClock - TIM_gf_LockTime;
                v130->i_CurrentState = 3;
            }
            if (v130->i_CurrentState == 3)
            {
                AI_EvalFunc_IoJoyGetMove_C(&v114);
                v93 = v114.y;
                v17 = (float)fabs(v114.y);
                v99 = v17;
                if (v17 <= 0.5)
                {
                    v130->f_Time = 0.0;
                }
                else
                {
                    v130->f_Chrono = TIM_gf_MainClock - TIM_gf_LockTime;
                    if (v130->f_Time <= 0.0)
                    {
                        if (v93 <= 0.0)
                            v3 = AI_EvalFunc_MTHModulo_C(v130->i_langcur + 1, v130->i_langnb);
                        else
                            v3 = AI_EvalFunc_MTHModulo_C(v130->i_langcur + v130->i_langnb - 1, v130->i_langnb);
                        v130->i_langcur = v3;
                        v75 = (tGameObjectMain*)v130->texte;
                        if (v75)
                        {
                            if ((unsigned int)v75 <= 2)
                                v75 = AI_MainActor[(uint32_t)v75 - 1];
                            AI_EvalFunc_OBJ_TextPivotGet_C(v75, v130->i_langcur, &v94);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_TextPivotGet_C(0, v130->i_langcur, &v94);
                        }
                        v105 = v94;
                        v74 = (tGameObjectMain*)v130->texte;
                        if (v74)
                        {
                            if ((unsigned int)v74 <= 2)
                                v74 = AI_MainActor[(uint32_t)v74 - 1];
                            AI_EvalFunc_OBJ_TextPivotSet_C(v74, 20, &v105);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_TextPivotSet_C(0, 20, &v105);
                        }
                        v73 = (tGameObjectMain*)v130->texte;
                        if (v73)
                        {
                            if ((unsigned int)v73 <= 2)
                                v73 = AI_MainActor[(uint32_t)v73 - 1];
                            AI_EvalFunc_OBJ_TextSizeGet_C(v73, v130->i_langcur, 0, -1, &v108);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_TextSizeGet_C(0, v130->i_langcur, 0, -1, &v108);
                        }
                        v105.x = v108.x + 0.02f + v105.x;
                        v72 = (tGameObjectMain*)v130->texte;
                        if (v72)
                        {
                            if ((unsigned int)v72 <= 2)
                                v72 = AI_MainActor[(uint32_t)v72 - 1];
                            AI_EvalFunc_OBJ_TextPivotSet_C(v72, 21, &v105);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_TextPivotSet_C(0, 21, &v105);
                        }
                        for (i = 0; i < v130->i_langnb; ++i)
                        {
                            v71 = (tGameObjectMain*)v130->texte;
                            if (v71)
                            {
                                if ((unsigned int)v71 <= 2)
                                    v71 = AI_MainActor[(uint32_t)v71 - 1];
                                AI_EvalFunc_OBJ_TextColorSet_C(v71, i, 0, -1, 0xFF7F7F7F);
                            }
                            else
                            {
                                AI_EvalFunc_OBJ_TextColorSet_C(0, i, 0, -1, 0xFF7F7F7F);
                            }
                        }
                        v70 = (tGameObjectMain*)v130->texte;
                        if (v70)
                        {
                            if ((unsigned int)v70 <= 2)
                                v70 = AI_MainActor[(uint32_t)v70 - 1];
                            AI_EvalFunc_OBJ_TextColorSet_C(v70, v130->i_langcur, 0, -1, -1);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_TextColorSet_C(0, v130->i_langcur, 0, -1, -1);
                        }
                        v123 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 5, 2176);
                        v130->f_Time = 0.2f;
                    }
                    else
                    {
                        v130->f_Time = v130->f_Time - TIM_gf_dt;
                    }
                }
                i = 0;
                if (AI_EvalFunc_IoButtonJustPressed_C(0))
                    i = 1;
                if (AI_EvalFunc_IoButtonJustPressed_C(32))
                    i = 1;
                if (i)
                {
                    v130->f_Chrono = TIM_gf_MainClock - TIM_gf_LockTime;
                    v107 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 6, 2176);
                    v106 = AI_EvalFunc_IoGameLangSet_C(v130->i_langval[v130->i_langcur]);
                    v130->f_Time = 1.0;
                    v130->i_CurrentState = 4;
                }
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 1000;
            }
            else if (v130->i_CurrentState == 4)
            {
                v124 = TIM_gf_dt;
                v130->f_Time = v130->f_Time - TIM_gf_dt * 4.0f;
                if (v130->f_Time < 0.0)
                {
                    v69 = (tGameObjectMain*)v130->o_LangueTitle;
                    if (v69)
                    {
                        if ((unsigned int)v69 <= 2)
                            v69 = AI_MainActor[(uint32_t)v69 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v69, 1);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
                    }
                    v68 = (tGameObjectMain*)v130->o_LangueSel;
                    if (v68)
                    {
                        if ((unsigned int)v68 <= 2)
                            v68 = AI_MainActor[(uint32_t)v68 - 1];
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(v68, 1);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(0, 1);
                    }
                    v130->f_Time = 1.0;
                    v130->i_CurrentState = 10;
                }
                v67 = (tGameObjectMain*)v130->texte;
                if (v67)
                {
                    if ((unsigned int)v67 <= 2)
                        v67 = AI_MainActor[(uint32_t)v67 - 1];
                    STR_Reset(v67);
                }
                else
                {
                    STR_Reset(0);
                }
                v66 = 1.0f - v130->f_Time;
                v97.x = v66 * v130->v_LangueTitle_Offset.x;
                v97.y = v130->v_LangueTitle_Offset.y * v66;
                v97.z = v130->v_LangueTitle_Offset.z * v66;
                MATH_VecAddVec(&v133, &v130->v_LangueTitle_Pos, &v97);
                v65 = (tGameObjectMain*)v130->o_LangueTitle;
                if (v65)
                {
                    if ((unsigned int)v65 <= 2)
                        v65 = AI_MainActor[(uint32_t)v65 - 1];
                    AI_EvalFunc_OBJSetPos_C(v65, &v133);
                }
                else
                {
                    AI_EvalFunc_OBJSetPos_C(0, &v133);
                }
                v64 = (tGameObjectMain*)v130->o_LangueSel;
                if (v64)
                {
                    if ((unsigned int)v64 <= 2)
                        v64 = AI_MainActor[(uint32_t)v64 - 1];
                    v63 = v64;
                }
                else
                {
                    v63 = 0;
                }
                v16 = (float)v130->i_langnb;
                a3b = v130->f_Time * v16 + 1.0f;
                AI_EvalFunc_OBJ_SetChannelProg_C(v63, 0, a3b);
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 1000;
            }
            else if (v130->i_CurrentState == 5)
            {
                v62 = (tGameObjectMain*)v130->texte;
                if (v62)
                {
                    if ((unsigned int)v62 <= 2)
                        v62 = AI_MainActor[(uint32_t)v62 - 1];
                    STR_SetString(v62, 20, (char*)AI_gpst_CurrentFunction->localStringConstsData + 23);
                }
                else
                {
                    STR_SetString(0, 20, (char*)AI_gpst_CurrentFunction->localStringConstsData + 23);
                }
                v61 = (tGameObjectMain*)v130->texte;
                if (v61)
                {
                    if ((unsigned int)v61 <= 2)
                        v61 = AI_MainActor[(uint32_t)v61 - 1];
                    STR_SetFrame(v61, 20, 9, 32);
                }
                else
                {
                    STR_SetFrame(0, 20, 9, 32);
                }
                v60 = (tGameObjectMain*)v130->texte;
                if (v60)
                {
                    if ((unsigned int)v60 <= 2)
                        v60 = AI_MainActor[(uint32_t)v60 - 1];
                    STR_SetFrameBorderSize(v60, 20, 0.0099999998f);
                }
                else
                {
                    STR_SetFrameBorderSize(0, 20, 0.0099999998f);
                }
                v59 = (tGameObjectMain*)v130->texte;
                if (v59)
                {
                    if ((unsigned int)v59 <= 2)
                        v59 = AI_MainActor[(uint32_t)v59 - 1];
                    STR_SetFrameColor(v59, 20, -1);
                }
                else
                {
                    STR_SetFrameColor(0, 20, -1);
                }
                memcpy(&v113, &v130->txt_MemCard, sizeof(v113));
                v113.idx += 21;
                v129.x = 0.5f;
                v129.y = 0.85000002f;
                v129.z = 0.0f;
                v58 = (tGameObjectMain*)v130->texte;
                if (v58)
                {
                    if ((unsigned int)v58 <= 2)
                        v58 = AI_MainActor[(uint32_t)v58 - 1];
                    v57 = v58;
                }
                else
                {
                    v57 = 0;
                }
                v4 = AI_EvalFunc_TEXTToString_C(&v113);
                STR_SetStringExt(v57, 20, 2031929, v4, &v129);
                v56 = (tGameObjectMain*)v130->texte;
                if (v56)
                {
                    if ((unsigned int)v56 <= 2)
                        v56 = AI_MainActor[(uint32_t)v56 - 1];
                    AI_EvalFunc_OBJ_TextFlagSet_C(v56, 11, 0x400);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextFlagSet_C(0, 11, 0x400);
                }
                v55 = (tGameObjectMain*)v130->texte;
                if (v55)
                {
                    if ((unsigned int)v55 <= 2)
                        v55 = AI_MainActor[(uint32_t)v55 - 1];
                    AI_EvalFunc_OBJ_TextFlagSet_C(v55, 12, 0x400);
                }
                else
                {
                    AI_EvalFunc_OBJ_TextFlagSet_C(0, 12, 0x400);
                }
                for (j = 1; j <= 5; ++j)
                {
                    v54 = (tGameObjectMain*)v130->texte;
                    if (v54)
                    {
                        if ((unsigned int)v54 <= 2)
                            v54 = AI_MainActor[(uint32_t)v54 - 1];
                        AI_EvalFunc_OBJ_TextColorSet_C(v54, j, 0, -1, 0xFF4F4F4F);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_TextColorSet_C(0, j, 0, -1, 0xFF4F4F4F);
                    }
                }
                v111.x = 0.5f;
                v111.y = 0.5f;
                v111.z = 0.0;
                v53 = (tGameObjectMain*)v130->texte;
                if (v53)
                {
                    if ((unsigned int)v53 <= 2)
                        v53 = AI_MainActor[(uint32_t)v53 - 1];
                    STR_SetStringExt(v53, 20, 1966101, (char*)AI_gpst_CurrentFunction->localStringConstsData + 24, &v111);
                }
                else
                {
                    STR_SetStringExt(0, 20, 1966101, (char*)AI_gpst_CurrentFunction->localStringConstsData + 24, &v111);
                }
                INO_SavSetCardStatus(v130->MC_slot, 30);
                v130->i_CurrentState = 6;
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 1000;
            }
            else if (v130->i_CurrentState == 6)
            {
                v117 = INO_i_SavGetCardStatus(v130->MC_slot, v130->MC_button);
                if (v117 != -1)
                {
                    v5 = INO_i_SavGetCardStatus(v130->MC_slot, v130->MC_button);
                    v130->MC_result = v5;
                    if (v130->MC_result)
                    {
                        if (v130->MC_result == 302)
                            v115 = 39;
                        else
                            v115 = 40;
                        v130->MC_button[0] = 3;
                        v130->MC_button[1] = 6;
                        v130->MC_button[2] = 0;
                        v130->MC_button[3] = 0;
                        memcpy(&v100, &v130->txt_MemCard, sizeof(v100));
                        v100.idx += v115;
                        v52 = (tGameObjectMain*)v130->texte;
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
                        v6 = AI_EvalFunc_TEXTToString_C(&v100);
                        STR_SetString(v51, 20, v6);
                        if (v130->MC_button[0] || v130->MC_button[1] || v130->MC_button[2] || v130->MC_button[3])
                        {
                            v50 = (tGameObjectMain*)v130->texte;
                            if (v50)
                            {
                                if ((unsigned int)v50 <= 2)
                                    v50 = AI_MainActor[(uint32_t)v50 - 1];
                                STR_InsertString(v50, 20, -1, (char*)AI_gpst_CurrentFunction->localStringConstsData + 25);
                            }
                            else
                            {
                                STR_InsertString(0, 20, -1, (char*)AI_gpst_CurrentFunction->localStringConstsData + 25);
                            }
                            if (v130->MC_button[1])
                            {
                                v49 = (tGameObjectMain*)v130->texte;
                                if (v49)
                                {
                                    if ((unsigned int)v49 <= 2)
                                        v49 = AI_MainActor[(uint32_t)v49 - 1];
                                    STR_InsertString(v49, 20, -1, (char*)AI_gpst_CurrentFunction->localStringConstsData + 32);
                                }
                                else
                                {
                                    STR_InsertString(0, 20, -1, (char*)AI_gpst_CurrentFunction->localStringConstsData + 32);
                                }
                                memcpy(&v137, &v130->txt_MemCard, sizeof(v137));
                                v137.idx += v130->MC_button[1] + 9;
                                v48 = (tGameObjectMain*)v130->texte;
                                if (v48)
                                {
                                    if ((unsigned int)v48 <= 2)
                                        v48 = AI_MainActor[(uint32_t)v48 - 1];
                                    v47 = v48;
                                }
                                else
                                {
                                    v47 = 0;
                                }
                                v7 = AI_EvalFunc_TEXTToString_C(&v137);
                                STR_InsertString(v47, 20, -1, v7);
                            }
                            if (v130->MC_button[0])
                            {
                                v46 = (tGameObjectMain*)v130->texte;
                                if (v46)
                                {
                                    if ((unsigned int)v46 <= 2)
                                        v46 = AI_MainActor[(uint32_t)v46 - 1];
                                    STR_InsertString(v46, 20, -1, (char*)AI_gpst_CurrentFunction->localStringConstsData + 41);
                                }
                                else
                                {
                                    STR_InsertString(0, 20, -1, (char*)AI_gpst_CurrentFunction->localStringConstsData + 41);
                                }
                                memcpy(&v122, &v130->txt_MemCard, sizeof(v122));
                                v122.idx += v130->MC_button[0] + 9;
                                v45 = (tGameObjectMain*)v130->texte;
                                if (v45)
                                {
                                    if ((unsigned int)v45 <= 2)
                                        v45 = AI_MainActor[(uint32_t)v45 - 1];
                                    v44 = v45;
                                }
                                else
                                {
                                    v44 = 0;
                                }
                                v8 = AI_EvalFunc_TEXTToString_C(&v122);
                                STR_InsertString(v44, 20, -1, v8);
                            }
                            if (v130->MC_button[2])
                            {
                                v43 = (tGameObjectMain*)v130->texte;
                                if (v43)
                                {
                                    if ((unsigned int)v43 <= 2)
                                        v43 = AI_MainActor[(uint32_t)v43 - 1];
                                    STR_InsertString(v43, 20, -1, (char*)AI_gpst_CurrentFunction->localStringConstsData + 50);
                                }
                                else
                                {
                                    STR_InsertString(0, 20, -1, (char*)AI_gpst_CurrentFunction->localStringConstsData + 50);
                                }
                                memcpy(&v95, &v130->txt_MemCard, sizeof(v95));
                                v95.idx += v130->MC_button[2] + 9;
                                v42 = (tGameObjectMain*)v130->texte;
                                if (v42)
                                {
                                    if ((unsigned int)v42 <= 2)
                                        v42 = AI_MainActor[(uint32_t)v42 - 1];
                                    v41 = v42;
                                }
                                else
                                {
                                    v41 = 0;
                                }
                                v9 = AI_EvalFunc_TEXTToString_C(&v95);
                                STR_InsertString(v41, 20, -1, v9);
                            }
                            if (v130->MC_button[3])
                            {
                                if (v130->MC_button[3] == 7)
                                {
                                    v40 = (tGameObjectMain*)v130->texte;
                                    if (v40)
                                    {
                                        if ((unsigned int)v40 <= 2)
                                            v40 = AI_MainActor[(uint32_t)v40 - 1];
                                        STR_InsertString(v40, 20, -1, (char*)AI_gpst_CurrentFunction->localStringConstsData + 59);
                                    }
                                    else
                                    {
                                        STR_InsertString(0, 20, -1, (char*)AI_gpst_CurrentFunction->localStringConstsData + 59);
                                    }
                                }
                                v39 = (tGameObjectMain*)v130->texte;
                                if (v39)
                                {
                                    if ((unsigned int)v39 <= 2)
                                        v39 = AI_MainActor[(uint32_t)v39 - 1];
                                    STR_InsertString(v39, 20, -1, (char*)AI_gpst_CurrentFunction->localStringConstsData + 63);
                                }
                                else
                                {
                                    STR_InsertString(0, 20, -1, (char*)AI_gpst_CurrentFunction->localStringConstsData + 63);
                                }
                                memcpy(&v101, &v130->txt_MemCard, sizeof(v101));
                                v101.idx += v130->MC_button[3] + 9;
                                v38 = (tGameObjectMain*)v130->texte;
                                if (v38)
                                {
                                    if ((unsigned int)v38 <= 2)
                                        v38 = AI_MainActor[(uint32_t)v38 - 1];
                                    v37 = v38;
                                }
                                else
                                {
                                    v37 = 0;
                                }
                                v10 = AI_EvalFunc_TEXTToString_C(&v101);
                                STR_InsertString(v37, 20, -1, v10);
                            }
                        }
                        v127.x = 0.5f;
                        v127.y = 0.85000002f;
                        v127.z = 0.0f;
                        v36 = (tGameObjectMain*)v130->texte;
                        if (v36)
                        {
                            if ((unsigned int)v36 <= 2)
                                v36 = AI_MainActor[(uint32_t)v36 - 1];
                            STR_SetStringExt(v36, 20, 1966393, (char*)AI_gpst_CurrentFunction->localStringConstsData + 72, &v127);
                        }
                        else
                        {
                            STR_SetStringExt(0, 20, 1966393, (char*)AI_gpst_CurrentFunction->localStringConstsData + 72, &v127);
                        }
                        v35 = (tGameObjectMain*)v130->texte;
                        if (v35)
                        {
                            if ((unsigned int)v35 <= 2)
                                v35 = AI_MainActor[(uint32_t)v35 - 1];
                            STR_Align(v35, 20, 0, -1, 1);
                        }
                        else
                        {
                            STR_Align(0, 20, 0, -1, 1);
                        }
                        for (j = 1; j <= 5; ++j)
                        {
                            v34 = (tGameObjectMain*)v130->texte;
                            if (v34)
                            {
                                if ((unsigned int)v34 <= 2)
                                    v34 = AI_MainActor[(uint32_t)v34 - 1];
                                AI_EvalFunc_OBJ_TextColorSet_C(v34, j, 0, -1, 0xFF4F4F4F);
                            }
                            else
                            {
                                AI_EvalFunc_OBJ_TextColorSet_C(0, j, 0, -1, 0xFF4F4F4F);
                            }
                        }
                        v33 = (tGameObjectMain*)v130->texte;
                        if (v33)
                        {
                            if ((unsigned int)v33 <= 2)
                                v33 = AI_MainActor[(uint32_t)v33 - 1];
                            AI_EvalFunc_OBJ_TextFlagSet_C(v33, 11, 0x400);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_TextFlagSet_C(0, 11, 0x400);
                        }
                        v32 = (tGameObjectMain*)v130->texte;
                        if (v32)
                        {
                            if ((unsigned int)v32 <= 2)
                                v32 = AI_MainActor[(uint32_t)v32 - 1];
                            AI_EvalFunc_OBJ_TextFlagSet_C(v32, 12, 0x400);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_TextFlagSet_C(0, 12, 0x400);
                        }
                        v135.x = 0.5;
                        v135.y = 0.5;
                        v135.z = 0.0;
                        v31 = (tGameObjectMain*)v130->texte;
                        if (v31)
                        {
                            if ((unsigned int)v31 <= 2)
                                v31 = AI_MainActor[(uint32_t)v31 - 1];
                            STR_SetStringExt(v31, 20, 1966101, (char*)AI_gpst_CurrentFunction->localStringConstsData + 73, &v135);
                        }
                        else
                        {
                            STR_SetStringExt(0, 20, 1966101, (char*)AI_gpst_CurrentFunction->localStringConstsData + 73, &v135);
                        }
                        v30 = (tGameObjectMain*)v130->texte;
                        if (v30)
                        {
                            if ((unsigned int)v30 <= 2)
                                v30 = AI_MainActor[(uint32_t)v30 - 1];
                            STR_Align(v30, 20, 0, -1, 1);
                        }
                        else
                        {
                            STR_Align(0, 20, 0, -1, 1);
                        }
                        v130->f_Time = 0.0;
                        v130->i_CurrentState = 7;
                    }
                    else
                    {
                        v130->i_CurrentState = 10;
                    }
                }
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                result = 1000;
            }
            else
            {
                if (v130->i_CurrentState == 7)
                {
                    v115 = 0;
                    if (AI_EvalFunc_IoButtonJustPressed_C(0))
                    {
                        v115 = v130->MC_button[0];
                    }
                    else if (AI_EvalFunc_IoButtonJustPressed_C(1))
                    {
                        v115 = v130->MC_button[2];
                    }
                    else if (AI_EvalFunc_IoButtonJustPressed_C(2))
                    {
                        v115 = v130->MC_button[1];
                    }
                    if (v115 == 3)
                    {
                        v103 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 6, 2176);
                        v130->i_CurrentState = 5;
                    }
                    else if (v115 == 6 || v115 == 4)
                    {
                        INO_SavSetCardStatus(v130->MC_slot, 0);
                        v110 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 7, 2176);
                        v130->i_CurrentState = 10;
                    }
                    else if (v115)
                    {
                        INO_SavSetCardStatus(v130->MC_slot, v115);
                        v130->i_CurrentState = 6;
                        if (v115 == 5)
                        {
                            memcpy(&v136, &v130->txt_MemCard, sizeof(v136));
                            v136.idx += 35;
                            v128.x = 0.5f;
                            v128.y = 0.85000002f;
                            v128.z = 0.0f;
                            v29 = (tGameObjectMain*)v130->texte;
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
                            v11 = AI_EvalFunc_TEXTToString_C(&v136);
                            STR_SetStringExt(v28, 20, 2031929, v11, &v128);
                            v27 = (tGameObjectMain*)v130->texte;
                            if (v27)
                            {
                                if ((unsigned int)v27 <= 2)
                                    v27 = AI_MainActor[(uint32_t)v27 - 1];
                                AI_EvalFunc_OBJ_TextFlagSet_C(v27, 11, 0x400);
                            }
                            else
                            {
                                AI_EvalFunc_OBJ_TextFlagSet_C(0, 11, 0x400);
                            }
                            v26 = (tGameObjectMain*)v130->texte;
                            if (v26)
                            {
                                if ((unsigned int)v26 <= 2)
                                    v26 = AI_MainActor[(uint32_t)v26 - 1];
                                AI_EvalFunc_OBJ_TextFlagSet_C(v26, 12, 0x400);
                            }
                            else
                            {
                                AI_EvalFunc_OBJ_TextFlagSet_C(0, 12, 0x400);
                            }
                            for (j = 1; j <= 5; ++j)
                            {
                                v25 = (tGameObjectMain*)v130->texte;
                                if (v25)
                                {
                                    if ((unsigned int)v25 <= 2)
                                        v25 = AI_MainActor[(uint32_t)v25 - 1];
                                    AI_EvalFunc_OBJ_TextColorSet_C(v25, j, 0, -1, 0xFF4F4F4F);
                                }
                                else
                                {
                                    AI_EvalFunc_OBJ_TextColorSet_C(0, j, 0, -1, 0xFF4F4F4F);
                                }
                            }
                        }
                    }
                }
                if (v130->i_CurrentState == 10)
                {
                    v24 = (tGameObjectMain*)v130->sound;
                    if (v24)
                    {
                        if ((unsigned int)v24 <= 2)
                            v24 = AI_MainActor[(uint32_t)v24 - 1];
                        AI_EvalFunc_SNDPlayModifier_C(v24, 1);
                    }
                    else
                    {
                        AI_EvalFunc_SNDPlayModifier_C(0, 1);
                    }
                    v23 = (tGameObjectMain*)v130->texte;
                    if (v23)
                    {
                        if ((unsigned int)v23 <= 2)
                            v23 = AI_MainActor[(uint32_t)v23 - 1];
                        STR_Reset(v23);
                    }
                    else
                    {
                        STR_Reset(0);
                    }
                    v130->o_CurEvent = v130->o_Langue2Start;
                    v22 = (tGameObjectMain*)v130->o_CurEvent;
                    if (v22)
                    {
                        if ((unsigned int)v22 <= 2)
                            v22 = AI_MainActor[(uint32_t)v22 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v22, 0);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
                    }
                    v130->i_CurrentState = 11;
                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                    result = 1000;
                }
                else if (v130->i_CurrentState == 11)
                {
                    v21 = (tGameObjectMain*)v130->o_CurEvent;
                    if (v21)
                    {
                        if ((unsigned int)v21 <= 2)
                            v21 = AI_MainActor[(uint32_t)v21 - 1];
                        v12 = AI_EvalFunc_TRAIsStop_C(v21, 2);
                    }
                    else
                    {
                        v12 = AI_EvalFunc_TRAIsStop_C(0, 2);
                    }
                    v98 = v12;
                    if (v12)
                    {
                        v20 = (tGameObjectMain*)v130->o_CurEvent;
                        if (v20)
                        {
                            if ((unsigned int)v20 <= 2)
                                v20 = AI_MainActor[(uint32_t)v20 - 1];
                            AI_EvalFunc_OBJ_FlagInactiveSet_C(v20, 1);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 1);
                        }
                        v130->i_CurrentState = 0;
                        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_main_logo_ml_init1_main_logo_ml_init2);
                    }
                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                    result = 1000;
                }
                else
                {
                    result = 1000;
                }
            }
        }
    }
    return result;
}

// ===================

int main_fix_mf_init()
{
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        univVars = AI_ul_RealResolveOneRefObject(0x71003FF9);
        AI2C_ResolveBufferByName(&world_ID, (void*)univVars, "world_ID");
        AI2C_ResolveBufferByName(&load_world, (void*)univVars, "load_world");
    }
    TEXT_ChangeLang(0);
    SND_i_Setf(11, 0.5f);
    SND_i_Setf(12, 0.14f);
    *(uint32_t*)world_ID = 1000;
    *(uint32_t*)load_world = 1;
    return 1000;
}