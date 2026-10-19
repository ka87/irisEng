#include "ai_compiledscripts_inc.h"

//mirror_mirror_init
int fct_mirror_mirror_init_mirror_mirror_beforedisplay;
int fct_mirror_mirror_init_mirror_mirror_afterdisplay;
int mirror_mirror_init_univVars;
int mirror_mirror_init_univVars_Flag_speciaux_sauve;
int fct_mirror_mirror_init_mirror_mirror_exec_set_snd_pos;
int fct_mirror_mirror_init_mirror_mirror_loop;

//mirror_mirror_loop
int fct_mirror_mirror_loop_mirror_mirror_lightning_off;
int mirror_mirror_loop_univVars;
int mirror_mirror_loop_univVars_Flag_speciaux_sauve;
int mirror_mirror_loop__PJ_Sally_gao;
int mirror_mirror_loop_GST_Global_gao;
int mirror_mirror_loop_GST_Global_gao_i_map_just_reset;
int fct_mirror_mirror_loop_mirror_mirror_lightning_on;
int fct_mirror_mirror_loop_mirror_mirror_lightning_update;
int mirror_mirror_loop_GST_Global_gao_f_reinit_time;
int mirror_mirror_loop_univVars_Perso_ID_actor;
int fctvar_mirror_mirror_loop_Arc_generateur_gao;
int mirror_mirror_loop_SFX_light_smoke_gao;

//mirror_mirror_lightning_update
int fct_mirror_mirror_lightning_update_mirror_mirror_lightning_off;

//mirror_mirror_lightning_on
int fct_mirror_mirror_lightning_on_mirror_mirror_lightning_off;

//mirror_mirror_exec_set_snd_pos
int mirror_mirror_exec_set_snd_pos_PJ_Sally_gao;

int mirror_mirror_init()
{
    int result; // eax
    tScriptStruct_Mirror* v1; // [esp+8h] [ebp-B0h]
    tGameObjectMain* v2; // [esp+10h] [ebp-A8h]
    tGameObjectMain* v3; // [esp+28h] [ebp-90h]
    tGameObjectMain* v4; // [esp+2Ch] [ebp-8Ch]
    tGameObjectMain* v5; // [esp+30h] [ebp-88h]
    tGameObjectMain* v6; // [esp+38h] [ebp-80h]
    tGameObjectMain* v7; // [esp+3Ch] [ebp-7Ch]
    tGameObjectMain* v8; // [esp+40h] [ebp-78h]
    tGameObjectMain* v9; // [esp+48h] [ebp-70h]
    tGameObjectMain* v10; // [esp+4Ch] [ebp-6Ch]
    tGameObjectMain* v11; // [esp+50h] [ebp-68h]
    tGameObjectMain* v12; // [esp+5Ch] [ebp-5Ch]
    tGameObjectMain* v13; // [esp+64h] [ebp-54h]
    tGameObjectMain* v14; // [esp+68h] [ebp-50h]
    tGameObjectMain* v15; // [esp+6Ch] [ebp-4Ch]
    tGameObjectMain* v16; // [esp+74h] [ebp-44h]
    tGameObjectMain* v17; // [esp+7Ch] [ebp-3Ch]
    tGameObjectMain* v18; // [esp+80h] [ebp-38h]
    tGameObjectMain* v19; // [esp+84h] [ebp-34h]
    int v20; // [esp+90h] [ebp-28h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Mirror*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_mirror_mirror_init_mirror_mirror_beforedisplay = (int)AI_ul_RealResolveOneRefFunction(0x3D0005BEu);
        fct_mirror_mirror_init_mirror_mirror_afterdisplay = (int)AI_ul_RealResolveOneRefFunction(0x3D0005BFu);
        mirror_mirror_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&mirror_mirror_init_univVars_Flag_speciaux_sauve, (void*)mirror_mirror_init_univVars, 4488);
        fct_mirror_mirror_init_mirror_mirror_exec_set_snd_pos = (int)AI_ul_RealResolveOneRefFunction(0x87000DE7);
        fct_mirror_mirror_init_mirror_mirror_loop = (int)AI_ul_RealResolveOneRefFunction(0x5B0002B1u);
    }
    if (*AI_gppst_CurrentJumpNode != (tAINode*)22)
    {
        v1->t_time_spark = TIM_gf_MainClock - TIM_gf_LockTime;
        v1->t_delay_spark = AI_EvalFunc_MATHRandFloat_C(0.2f, 1.2f);
        v19 = (tGameObjectMain*)v1->des_o_const_gao;
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
            v17 = (tGameObjectMain*)v1->des_o_const_gao;
            if (v17)
            {
                if ((unsigned int)v17 <= 2)
                    v17 = AI_MainActor[(uint32_t)v17 - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v17,
                    0,
                    (tAIFunction*)fct_mirror_mirror_init_mirror_mirror_beforedisplay);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    0,
                    (tAIFunction*)fct_mirror_mirror_init_mirror_mirror_beforedisplay);
            }
            v16 = (tGameObjectMain*)v1->des_o_const_gao;
            if (v16)
            {
                if ((unsigned int)v16 <= 2)
                    v16 = AI_MainActor[(uint32_t)v16 - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v16,
                    1,
                    (tAIFunction*)fct_mirror_mirror_init_mirror_mirror_afterdisplay);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    1,
                    (tAIFunction*)fct_mirror_mirror_init_mirror_mirror_afterdisplay);
            }
        }
        v15 = (tGameObjectMain*)v1->des_o_const_gao1;
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
            v13 = (tGameObjectMain*)v1->des_o_const_gao1;
            if (v13)
            {
                if ((unsigned int)v13 <= 2)
                    v13 = AI_MainActor[(uint32_t)v13 - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v13,
                    0,
                    (tAIFunction*)fct_mirror_mirror_init_mirror_mirror_beforedisplay);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    0,
                    (tAIFunction*)fct_mirror_mirror_init_mirror_mirror_beforedisplay);
            }
            v12 = (tGameObjectMain*)v1->des_o_const_gao1;
            if (v12)
            {
                if ((unsigned int)v12 <= 2)
                    v12 = AI_MainActor[(uint32_t)v12 - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v12,
                    1,
                    (tAIFunction*)fct_mirror_mirror_init_mirror_mirror_afterdisplay);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    1,
                    (tAIFunction*)fct_mirror_mirror_init_mirror_mirror_afterdisplay);
            }
        }
        if (v1->des_i_specialflag)
        {
            if (v1->des_i_specialflag > 0
                && AI_EvalFunc_ARRBitTest_C((int*)(mirror_mirror_init_univVars_Flag_speciaux_sauve + 4), v1->des_i_specialflag))
            {
                v11 = (tGameObjectMain*)v1->des_o_Wall_GAO;
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
                    v9 = (tGameObjectMain*)v1->des_o_Wall_GAO;
                    if (v9)
                    {
                        if ((unsigned int)v9 <= 2)
                            v9 = AI_MainActor[(uint32_t)v9 - 1];
                        AI_EvalFunc_COLMapActivationSet_C(v9, 0, -1);
                    }
                    else
                    {
                        AI_EvalFunc_COLMapActivationSet_C(0, 0, -1);
                    }
                }
                AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 256);
                v1->detruit_moi = 1;
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                return 1000;
            }
        }
        if (v1->detruit_moi)
        {
            v8 = (tGameObjectMain*)v1->des_o_Wall_GAO;
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
                v6 = (tGameObjectMain*)v1->des_o_Wall_GAO;
                if (v6)
                {
                    if ((unsigned int)v6 <= 2)
                        v6 = AI_MainActor[(uint32_t)v6 - 1];
                    AI_EvalFunc_COLMapActivationSet_C(v6, 0, -1);
                }
                else
                {
                    AI_EvalFunc_COLMapActivationSet_C(0, 0, -1);
                }
            }
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v5 = (tGameObjectMain*)v1->des_o_Wall_GAO;
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
            v3 = (tGameObjectMain*)v1->des_o_Wall_GAO;
            if (v3)
            {
                if ((unsigned int)v3 <= 2)
                    v3 = AI_MainActor[(uint32_t)v3 - 1];
                AI_EvalFunc_COLMapActivationSet_C(v3, 0xFFu, 0);
            }
            else
            {
                AI_EvalFunc_COLMapActivationSet_C(0, 0xFFu, 0);
            }
        }
        v1->f_paf_time = TIM_gf_MainClock - TIM_gf_LockTime;
        v1->f_snd_dist_nosound = v1->f_snd_dist_nosound * v1->f_snd_dist_nosound;
        v1->msg.msgID = 37;
        v1->msg.gao = AI_gpst_CurrentGameObject;
        v1->msg.f1 = 1000.0;
        v1->msg.Int1 = 0;
        switch (v1->des_i_forcelum)
        {
        case 1:
            v1->msg.Int1 |= 0x8000u;
            break;
        case 2:
            v1->msg.Int1 |= 0x10000u;
            break;
        case 3:
            v1->msg.Int1 |= 0x18000u;
            break;
        }
        switch (v1->des_i_forceelec)
        {
        case 1:
            v1->msg.Int1 |= 8u;
            break;
        case 2:
            v1->msg.Int1 |= 0x10u;
            break;
        case 3:
            v1->msg.Int1 |= 0x18u;
            break;
        }
        if (v1->des_i_forceinerte)
            v1->msg.Int1 |= v1->des_i_forceinerte + 1;
        switch (v1->des_i_forcechaleur)
        {
        case 1:
            v1->msg.Int1 |= 0x20u;
            break;
        case 2:
            v1->msg.Int1 |= 0x40u;
            break;
        case 3:
            v1->msg.Int1 |= 0x60u;
            break;
        }
        v1->i_idspark[0] = -1;
        v1->i_idspark[1] = -1;
        v1->i_idhalo[0] = -1;
        v1->i_idhalo[1] = -1;
        v1->i_baseGFX = 16;
        if ((v1->des_i_spark & 1) != 0)
        {
            if (v1->des_i_generator)
                v1->i_baseGFX += 2;
            else
                v1->i_baseGFX += 3;
        }
        if ((v1->des_i_spark & 2) != 0)
            v1->i_baseGFX += 12;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v1->v_init_pos);
        if (v1->f_first_wait != 0.0)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)22;
            TIME_Wait_C(v1->f_first_wait);
            return 0;
        }
    }
    if (!v1->des_i_type)
    {
        v2 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v2 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v2,
                2,
                (tAIFunction*)fct_mirror_mirror_init_mirror_mirror_exec_set_snd_pos);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_mirror_mirror_init_mirror_mirror_exec_set_snd_pos);
        }
    }
    v20 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_mirror_mirror_init_mirror_mirror_loop);
    if (v20 == 1000)
        result = 1000;
    else
        result = v20;
    return result;
}

int mirror_mirror_loop()
{
    int result; // eax
    double v1; // st7
    BOOL v2; // eax
    double v3; // st7
    int v4; // eax
    int v5; // eax
    double v6; // st7
    int v7; // eax
    double v8; // st7
    int v9; // eax
    BOOL v10; // eax
    float v11; // [esp+0h] [ebp-928h]
    float v12; // [esp+4h] [ebp-924h]
    float v13; // [esp+4h] [ebp-924h]
    float v14; // [esp+4h] [ebp-924h]
    float v15; // [esp+4h] [ebp-924h]
    float v16; // [esp+Ch] [ebp-91Ch]
    tScriptStruct_Mirror* v17; // [esp+10h] [ebp-918h]
    float v18; // [esp+14h] [ebp-914h]
    tGameObjectMain* v19; // [esp+1Ch] [ebp-90Ch]
    tGameObjectMain* v20; // [esp+24h] [ebp-904h]
    tAI* v21; // [esp+2Ch] [ebp-8FCh]
    tGameObjectMain* v22; // [esp+30h] [ebp-8F8h]
    tGameObjectMain* v23; // [esp+34h] [ebp-8F4h]
    tGameObjectMain* v24; // [esp+40h] [ebp-8E8h]
    tGameObjectMain* v25; // [esp+44h] [ebp-8E4h]
    tAI* v26; // [esp+48h] [ebp-8E0h]
    tGameObjectMain* v27; // [esp+4Ch] [ebp-8DCh]
    tGameObjectMain* v28; // [esp+50h] [ebp-8D8h]
    tAI* v29; // [esp+54h] [ebp-8D4h]
    tGameObjectMain* v30; // [esp+58h] [ebp-8D0h]
    tGameObjectMain* v31; // [esp+5Ch] [ebp-8CCh]
    tAI* v32; // [esp+60h] [ebp-8C8h]
    tGameObjectMain* v33; // [esp+64h] [ebp-8C4h]
    tGameObjectMain* v34; // [esp+68h] [ebp-8C0h]
    tAI* v35; // [esp+6Ch] [ebp-8BCh]
    tGameObjectMain* v36; // [esp+70h] [ebp-8B8h]
    tGameObjectMain* v37; // [esp+74h] [ebp-8B4h]
    tAI* v38; // [esp+78h] [ebp-8B0h]
    tGameObjectMain* v39; // [esp+7Ch] [ebp-8ACh]
    tGameObjectMain* v40; // [esp+80h] [ebp-8A8h]
    tAI* v41; // [esp+84h] [ebp-8A4h]
    tGameObjectMain* v42; // [esp+88h] [ebp-8A0h]
    tGameObjectMain* v43; // [esp+8Ch] [ebp-89Ch]
    tAI* v44; // [esp+90h] [ebp-898h]
    tGameObjectMain* v45; // [esp+94h] [ebp-894h]
    tGameObjectMain* v46; // [esp+98h] [ebp-890h]
    tAI* v47; // [esp+9Ch] [ebp-88Ch]
    tGameObjectMain* v48; // [esp+A0h] [ebp-888h]
    tGameObjectMain* v49; // [esp+A4h] [ebp-884h]
    tAI* v50; // [esp+A8h] [ebp-880h]
    tGameObjectMain* v51; // [esp+ACh] [ebp-87Ch]
    tGameObjectMain* v52; // [esp+B0h] [ebp-878h]
    tAI* v53; // [esp+B4h] [ebp-874h]
    tGameObjectMain* v54; // [esp+B8h] [ebp-870h]
    tGameObjectMain* v55; // [esp+BCh] [ebp-86Ch]
    tAI* v56; // [esp+C0h] [ebp-868h]
    tGameObjectMain* v57; // [esp+C4h] [ebp-864h]
    tGameObjectMain* v58; // [esp+C8h] [ebp-860h]
    tAI* v59; // [esp+CCh] [ebp-85Ch]
    tGameObjectMain* v60; // [esp+D0h] [ebp-858h]
    tGameObjectMain* v61; // [esp+D4h] [ebp-854h]
    tAI* v62; // [esp+E4h] [ebp-844h]
    tGameObjectMain* v63; // [esp+E8h] [ebp-840h]
    tGameObjectMain* v64; // [esp+ECh] [ebp-83Ch]
    tGameObjectMain* v65; // [esp+F4h] [ebp-834h]
    tAI* v66; // [esp+F8h] [ebp-830h]
    tGameObjectMain* v67; // [esp+FCh] [ebp-82Ch]
    tGameObjectMain* v68; // [esp+100h] [ebp-828h]
    tAI* v69; // [esp+104h] [ebp-824h]
    tGameObjectMain* v70; // [esp+108h] [ebp-820h]
    tGameObjectMain* v71; // [esp+10Ch] [ebp-81Ch]
    tGameObjectMain* v72; // [esp+114h] [ebp-814h]
    tGameObjectMain* v73; // [esp+11Ch] [ebp-80Ch]
    tGameObjectMain* v74; // [esp+120h] [ebp-808h]
    tGameObjectMain* v75; // [esp+124h] [ebp-804h]
    tGameObjectMain* v76; // [esp+12Ch] [ebp-7FCh]
    tGameObjectMain* v77; // [esp+13Ch] [ebp-7ECh]
    tGameObjectMain* v78; // [esp+144h] [ebp-7E4h]
    tGameObjectMain* v79; // [esp+148h] [ebp-7E0h]
    tGameObjectMain* v80; // [esp+14Ch] [ebp-7DCh]
    tGameObjectMain* v81; // [esp+150h] [ebp-7D8h]
    tGameObjectMain* v82; // [esp+154h] [ebp-7D4h]
    tGameObjectMain* v83; // [esp+15Ch] [ebp-7CCh]
    tGameObjectMain* v84; // [esp+164h] [ebp-7C4h]
    tGameObjectMain* v85; // [esp+188h] [ebp-7A0h]
    tGameObjectMain* v86; // [esp+190h] [ebp-798h]
    tGameObjectMain* v87; // [esp+198h] [ebp-790h]
    Vec3* v88; // [esp+19Ch] [ebp-78Ch]
    tGameObjectMain* v89; // [esp+1E0h] [ebp-748h]
    Vec3* v90; // [esp+1E4h] [ebp-744h]
    tGameObjectMain* v91; // [esp+1F8h] [ebp-730h]
    tGameObjectMain* v92; // [esp+1FCh] [ebp-72Ch]
    tGameObjectMain* v93; // [esp+200h] [ebp-728h]
    tGameObjectMain* v94; // [esp+204h] [ebp-724h]
    tGameObjectMain* v95; // [esp+208h] [ebp-720h]
    tGameObjectMain* v96; // [esp+20Ch] [ebp-71Ch]
    tGameObjectMain* v97; // [esp+220h] [ebp-708h]
    tGameObjectMain* v98; // [esp+224h] [ebp-704h]
    tGameObjectMain* v99; // [esp+228h] [ebp-700h]
    tGameObjectMain* v100; // [esp+22Ch] [ebp-6FCh]
    tGameObjectMain* v101; // [esp+230h] [ebp-6F8h]
    tGameObjectMain* v102; // [esp+234h] [ebp-6F4h]
    tGameObjectMain* v103; // [esp+268h] [ebp-6C0h]
    tGameObjectMain* v104; // [esp+270h] [ebp-6B8h]
    tGameObjectMain* v105; // [esp+284h] [ebp-6A4h]
    tGameObjectMain* v106; // [esp+28Ch] [ebp-69Ch]
    tGameObjectMain* v107; // [esp+290h] [ebp-698h]
    tGameObjectMain* v108; // [esp+294h] [ebp-694h]
    tGameObjectMain* v109; // [esp+298h] [ebp-690h]
    tGameObjectMain* v110; // [esp+29Ch] [ebp-68Ch]
    tGameObjectMain* v111; // [esp+2A4h] [ebp-684h]
    tGameObjectMain* v112; // [esp+2A8h] [ebp-680h]
    tGameObjectMain* v113; // [esp+2ACh] [ebp-67Ch]
    tGameObjectMain* v114; // [esp+2B0h] [ebp-678h]
    tGameObjectMain* v115; // [esp+2B4h] [ebp-674h]
    tGameObjectMain* v116; // [esp+2C0h] [ebp-668h]
    tGameObjectMain* v117; // [esp+2C8h] [ebp-660h]
    tGameObjectMain* v118; // [esp+2D8h] [ebp-650h]
    tGameObjectMain* v119; // [esp+2E8h] [ebp-640h]
    tGameObjectMain* v120; // [esp+2F4h] [ebp-634h]
    tGameObjectMain* v121; // [esp+2FCh] [ebp-62Ch]
    tGameObjectMain* v122; // [esp+314h] [ebp-614h]
    float v123; // [esp+324h] [ebp-604h]
    float v124; // [esp+34Ch] [ebp-5DCh]
    float v125; // [esp+350h] [ebp-5D8h]
    tGameObjectMain* v126; // [esp+358h] [ebp-5D0h]
    tGameObjectMain* v127; // [esp+35Ch] [ebp-5CCh]
    tGameObjectMain* v128; // [esp+360h] [ebp-5C8h]
    tGameObjectMain* v129; // [esp+368h] [ebp-5C0h]
    tGameObjectMain* v130; // [esp+36Ch] [ebp-5BCh]
    tGameObjectMain* v131; // [esp+370h] [ebp-5B8h]
    tGameObjectMain* v132; // [esp+378h] [ebp-5B0h]
    tGameObjectMain* v133; // [esp+37Ch] [ebp-5ACh]
    tGameObjectMain* v134; // [esp+380h] [ebp-5A8h]
    tGameObjectMain* v135; // [esp+388h] [ebp-5A0h]
    tGameObjectMain* v136; // [esp+38Ch] [ebp-59Ch]
    tGameObjectMain* v137; // [esp+390h] [ebp-598h]
    Vec3 v138; // [esp+39Ch] [ebp-58Ch] BYREF
    Vec3 v139; // [esp+3A8h] [ebp-580h] BYREF
    Vec3 v140; // [esp+3B4h] [ebp-574h] BYREF
    Vec3 v141; // [esp+3C0h] [ebp-568h] BYREF
    Vec3 a5; // [esp+3CCh] [ebp-55Ch] BYREF
    Vec3 v143; // [esp+3D8h] [ebp-550h] BYREF
    Vec3 v144; // [esp+3E4h] [ebp-544h] BYREF
    float v145; // [esp+3F0h] [ebp-538h]
    float v146; // [esp+3F4h] [ebp-534h]
    float v147; // [esp+3F8h] [ebp-530h]
    float v149; // [esp+40Ch] [ebp-51Ch]
    Vec3 v150; // [esp+410h] [ebp-518h] BYREF
    int v151; // [esp+41Ch] [ebp-50Ch]
    int v152; // [esp+420h] [ebp-508h]
    int v153; // [esp+424h] [ebp-504h]
    Vec3 v154; // [esp+428h] [ebp-500h] BYREF
    Vec3 v155; // [esp+434h] [ebp-4F4h] BYREF
    Vec3 v156; // [esp+440h] [ebp-4E8h] BYREF
    Vec3 v157; // [esp+458h] [ebp-4D0h] BYREF
    float v158; // [esp+464h] [ebp-4C4h]
    int v160; // [esp+46Ch] [ebp-4BCh]
    int v161; // [esp+470h] [ebp-4B8h]
    float v162; // [esp+474h] [ebp-4B4h]
    Vec3 v163; // [esp+478h] [ebp-4B0h] BYREF
    Vec3 v164; // [esp+484h] [ebp-4A4h] BYREF
    Vec3 v165; // [esp+498h] [ebp-490h] BYREF
    Vec3 v166; // [esp+4A4h] [ebp-484h] BYREF
    Vec3 v167; // [esp+4B0h] [ebp-478h] BYREF
    Vec3 v168; // [esp+4BCh] [ebp-46Ch] BYREF
    int v169; // [esp+4C8h] [ebp-460h]
    int v170; // [esp+4D0h] [ebp-458h]
    Vec3 v171; // [esp+4D4h] [ebp-454h] BYREF
    Vec3 v172; // [esp+4E0h] [ebp-448h] BYREF
    Vec3 v173; // [esp+4ECh] [ebp-43Ch] BYREF
    Vec3 v174; // [esp+4F8h] [ebp-430h] BYREF
    Vec3 v175; // [esp+504h] [ebp-424h] BYREF
    Vec3 v176; // [esp+510h] [ebp-418h] BYREF
    Vec3 v177; // [esp+51Ch] [ebp-40Ch] BYREF
    float v178; // [esp+528h] [ebp-400h]
    float v179; // [esp+530h] [ebp-3F8h]
    int v180; // [esp+534h] [ebp-3F4h]
    float a4; // [esp+538h] [ebp-3F0h]
    BOOL v182; // [esp+53Ch] [ebp-3ECh]
    Vec3 v183; // [esp+544h] [ebp-3E4h] BYREF
    Vec3 v184; // [esp+550h] [ebp-3D8h] BYREF
    Vec3 v185; // [esp+55Ch] [ebp-3CCh] BYREF
    float v186; // [esp+568h] [ebp-3C0h]
    float v187; // [esp+56Ch] [ebp-3BCh]
    float v188; // [esp+574h] [ebp-3B4h]
    int v190; // [esp+57Ch] [ebp-3ACh]
    float v191; // [esp+580h] [ebp-3A8h]
    Vec3 v192; // [esp+584h] [ebp-3A4h] BYREF
    float v193; // [esp+590h] [ebp-398h]
    float v194; // [esp+594h] [ebp-394h]
    float v195; // [esp+598h] [ebp-390h]
    Vec3 v196; // [esp+59Ch] [ebp-38Ch] BYREF
    Vec3 v197; // [esp+5A8h] [ebp-380h] BYREF
    tAI* v198; // [esp+5B4h] [ebp-374h]
    tGameObjectMain* v199; // [esp+5B8h] [ebp-370h]
    Vec3 v200; // [esp+5C8h] [ebp-360h] BYREF
    Vec3 v201; // [esp+5D4h] [ebp-354h] BYREF
    Vec3 a1; // [esp+5E0h] [ebp-348h] BYREF
    Vec3 v203; // [esp+5ECh] [ebp-33Ch] BYREF
    Vec3 v204; // [esp+5F8h] [ebp-330h] BYREF
    Vec3 v205; // [esp+604h] [ebp-324h] BYREF
    Vec3 v206; // [esp+610h] [ebp-318h] BYREF
    int v207; // [esp+61Ch] [ebp-30Ch]
    int v209; // [esp+624h] [ebp-304h]
    Vec3 v210; // [esp+638h] [ebp-2F0h] BYREF
    Vec3 v211; // [esp+644h] [ebp-2E4h] BYREF
    Vec3 v212; // [esp+650h] [ebp-2D8h] BYREF
    Vec3 a2; // [esp+65Ch] [ebp-2CCh] BYREF
    Vec3 v214; // [esp+668h] [ebp-2C0h] BYREF
    Vec3 v215; // [esp+674h] [ebp-2B4h] BYREF
    void* v216; // [esp+680h] [ebp-2A8h]
    int v217; // [esp+684h] [ebp-2A4h]
    float v218; // [esp+688h] [ebp-2A0h]
    BOOL v219; // [esp+68Ch] [ebp-29Ch]
    Vec3 v220; // [esp+694h] [ebp-294h] BYREF
    Vec3 v221; // [esp+6A0h] [ebp-288h] BYREF
    Vec3 v222; // [esp+6ACh] [ebp-27Ch] BYREF
    Vec3 a3; // [esp+6B8h] [ebp-270h] BYREF
    Vec3 v; // [esp+6C4h] [ebp-264h] BYREF
    Vec3 v225; // [esp+6D0h] [ebp-258h] BYREF
    Vec3 v226; // [esp+6DCh] [ebp-24Ch] BYREF
    Vec3 v227; // [esp+6E8h] [ebp-240h] BYREF
    Vec3 v228; // [esp+6F4h] [ebp-234h] BYREF
    int v229; // [esp+704h] [ebp-224h]
    int v230; // [esp+708h] [ebp-220h]
    int v231; // [esp+70Ch] [ebp-21Ch]
    int v232; // [esp+710h] [ebp-218h]
    int v233; // [esp+714h] [ebp-214h]
    Vec3 dest; // [esp+718h] [ebp-210h] BYREF
    tMessage v235; // [esp+728h] [ebp-200h] BYREF
    int v236; // [esp+75Ch] [ebp-1CCh]
    int v237; // [esp+760h] [ebp-1C8h]
    float v238; // [esp+764h] [ebp-1C4h]
    int v239; // [esp+768h] [ebp-1C0h]
    float v240; // [esp+76Ch] [ebp-1BCh]
    Vec3 v241; // [esp+770h] [ebp-1B8h] BYREF
    Vec3 v242; // [esp+788h] [ebp-1A0h] BYREF
    Vec3 v243; // [esp+794h] [ebp-194h] BYREF
    Vec3 v244; // [esp+7A0h] [ebp-188h] BYREF
    Vec3 v245; // [esp+7ACh] [ebp-17Ch] BYREF
    float v246; // [esp+7B8h] [ebp-170h]
    float v247; // [esp+7BCh] [ebp-16Ch]
    float v248; // [esp+7C0h] [ebp-168h]
    int v249; // [esp+7C8h] [ebp-160h]
    int v250; // [esp+7CCh] [ebp-15Ch]
    int v251; // [esp+7D0h] [ebp-158h]
    unsigned int v252; // [esp+7D4h] [ebp-154h]
    float v253; // [esp+7D8h] [ebp-150h]
    Vec3 v254; // [esp+7E0h] [ebp-148h] BYREF
    Vec3 v255; // [esp+7F8h] [ebp-130h] BYREF
    Vec3 v256; // [esp+804h] [ebp-124h] BYREF
    Vec3 v257; // [esp+810h] [ebp-118h] BYREF
    int v258; // [esp+81Ch] [ebp-10Ch]
    float v259; // [esp+820h] [ebp-108h]
    float v260; // [esp+828h] [ebp-100h]
    float v261; // [esp+82Ch] [ebp-FCh]
    int v262; // [esp+830h] [ebp-F8h]
    void* Src; // [esp+834h] [ebp-F4h]
    Vec3 v264; // [esp+838h] [ebp-F0h] BYREF
    Vec3 v265; // [esp+844h] [ebp-E4h] BYREF
    tScriptStruct_Mirror* v266; // [esp+850h] [ebp-D8h]
    BOOL v267; // [esp+854h] [ebp-D4h]
    int v268; // [esp+85Ch] [ebp-CCh]
    float v269; // [esp+864h] [ebp-C4h]
    Vec3 v270; // [esp+86Ch] [ebp-BCh] BYREF
    float v271; // [esp+878h] [ebp-B0h]
    float v272; // [esp+87Ch] [ebp-ACh]
    float v273; // [esp+880h] [ebp-A8h]
    Vec3 v274; // [esp+884h] [ebp-A4h] BYREF
    Vec3 v275; // [esp+89Ch] [ebp-8Ch] BYREF
    Vec3 v276; // [esp+8A8h] [ebp-80h] BYREF
    Vec3 v277; // [esp+8B4h] [ebp-74h] BYREF
    Vec3 v278; // [esp+8C0h] [ebp-68h] BYREF
    float v279; // [esp+8CCh] [ebp-5Ch]
    Vec3 v280; // [esp+8D0h] [ebp-58h] BYREF
    Vec3 v281; // [esp+8DCh] [ebp-4Ch] BYREF
    Vec3 v282; // [esp+8E8h] [ebp-40h] BYREF
    Vec3 v283; // [esp+8F4h] [ebp-34h] BYREF
    Vec3 v284; // [esp+900h] [ebp-28h] BYREF
    Vec3 v285; // [esp+90Ch] [ebp-1Ch] BYREF
    int v286; // [esp+918h] [ebp-10h]
    float v287; // [esp+91Ch] [ebp-Ch]
    float v288; // [esp+920h] [ebp-8h]
    float v289; // [esp+924h] [ebp-4h]

    v.x = 1.0;
    v.y = 0.0;
    v.z = 0.0;
    v270.x = 1.0;
    v270.y = 0.0;
    v270.z = 0.0;
    v243.x = 1.0;
    v243.y = 0.0;
    v243.z = 0.0;
    v265.x = 1.0;
    v265.y = 0.0;
    v265.z = 0.0;
    v215.x = 1.0;
    v215.y = 0.0;
    v215.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v17 = (tScriptStruct_Mirror*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v17 = 0;
    v266 = v17;
    v249 = 0;
    v186 = 0.0;
    v231 = 0;
    v251 = 0;
    v190 = 0;
    v252 = 0;
    v261 = 0.0;
    v288 = 0.0;
    v209 = 0;
    v191 = 0.0;
    v198 = 0;
    v236 = 0;
    v182 = 0;
    v199 = 0;
    v149 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_mirror_mirror_loop_mirror_mirror_lightning_off = (int)AI_ul_RealResolveOneRefFunction(0x35009A5Cu);
        mirror_mirror_loop_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&mirror_mirror_loop_univVars_Flag_speciaux_sauve, (void*)mirror_mirror_loop_univVars, 4488);
        mirror_mirror_loop__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        mirror_mirror_loop_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &mirror_mirror_loop_GST_Global_gao_i_map_just_reset,
            (void*)mirror_mirror_loop_GST_Global_gao,
            3300);
        fct_mirror_mirror_loop_mirror_mirror_lightning_on = (int)AI_ul_RealResolveOneRefFunction(0x35009A5Au);
        fct_mirror_mirror_loop_mirror_mirror_lightning_update = (int)AI_ul_RealResolveOneRefFunction(0x35009A5Bu);
        AI2C_ResolveBuffer(&mirror_mirror_loop_GST_Global_gao_f_reinit_time, (void*)mirror_mirror_loop_GST_Global_gao, 12);
        AI2C_ResolveBuffer(&mirror_mirror_loop_univVars_Perso_ID_actor, (void*)mirror_mirror_loop_univVars, 988);
        fctvar_mirror_mirror_loop_Arc_generateur_gao = AI_ul_RealResolveOneRef(42, 0x5B0002AFu);
        mirror_mirror_loop_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    v266->i_flag_ON = 0;
    if (v266->i_flag_1st_trame)
        --v266->i_flag_1st_trame;
    if (v266->i_paf_one_time)
        v266->des_f_sf_time = v266->des_f_sf_time - TIM_gf_dt;
    Src = AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v235, Src, sizeof(v235));
    while (v235.msgID)
    {
        if (v235.msgID == 43)
        {
            v239 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_mirror_mirror_loop_mirror_mirror_lightning_off);
            if (v239 != 1000)
                return v239;
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 256);
            v266->detruit_moi = 1;
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        if (v235.msgID == 44)
            v266->detruit_moi = 0;
        if (v266->detruit_moi)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        if (v266->des_i_specialflag)
        {
            if (v235.msgID == 37)
            {
                v190 = v235.Int1 & 7;
                if (v190 >= v266->des_puissance)
                {
                    v266->i_paf_one_time = 1;
                    v236 = 1;
                    if (v266->des_i_sf_peut_exploser)
                    {
                        v182 = v266->des_i_sf_peut_exploser > 0
                            && AI_EvalFunc_ARRBitTest_C(
                                (int*)(mirror_mirror_loop_univVars_Flag_speciaux_sauve + 4),
                                v266->des_i_sf_peut_exploser);
                        if (!v182 || v266->des_f_sf_time > 0.0)
                            v236 = 0;
                    }
                    if (v236)
                    {
                        if (v266->des_i_specialflag > 0)
                            AI_EvalFunc_ARRBitSet_C(
                                (int*)(mirror_mirror_loop_univVars_Flag_speciaux_sauve + 4),
                                v266->des_i_specialflag);
                        v161 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_mirror_mirror_loop_mirror_mirror_lightning_off);
                        if (v161 != 1000)
                            return v161;
                        v137 = (tGameObjectMain*)v266->des_o_Wall_GAO;
                        if (v137)
                        {
                            if ((unsigned int)v137 <= 2)
                                v137 = AI_MainActor[(uint32_t)v137 - 1];
                            v136 = v137;
                        }
                        else
                        {
                            v136 = 0;
                        }
                        if (v136)
                        {
                            v135 = (tGameObjectMain*)v266->des_o_Wall_GAO;
                            if (v135)
                            {
                                if ((unsigned int)v135 <= 2)
                                    v135 = AI_MainActor[(uint32_t)v135 - 1];
                                AI_EvalFunc_COLMapActivationSet_C(v135, 0, -1);
                            }
                            else
                            {
                                AI_EvalFunc_COLMapActivationSet_C(0, 0, -1);
                            }
                        }
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(AI_gpst_CurrentGameObject, 1);
                        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                        return 1000;
                    }
                }
            }
        }
        v216 = AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v235, v216, sizeof(v235));
    }
    if (v266->detruit_moi)
    {
        v134 = (tGameObjectMain*)v266->des_o_Wall_GAO;
        if (v134)
        {
            if ((unsigned int)v134 <= 2)
                v134 = AI_MainActor[(uint32_t)v134 - 1];
            v133 = v134;
        }
        else
        {
            v133 = 0;
        }
        if (v133)
        {
            v132 = (tGameObjectMain*)v266->des_o_Wall_GAO;
            if (v132)
            {
                if ((unsigned int)v132 <= 2)
                    v132 = AI_MainActor[(uint32_t)v132 - 1];
                AI_EvalFunc_COLMapActivationSet_C(v132, 0, -1);
            }
            else
            {
                AI_EvalFunc_COLMapActivationSet_C(0, 0, -1);
            }
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    v131 = (tGameObjectMain*)v266->des_o_Wall_GAO;
    if (v131)
    {
        if ((unsigned int)v131 <= 2)
            v131 = AI_MainActor[(uint32_t)v131 - 1];
        v130 = v131;
    }
    else
    {
        v130 = 0;
    }
    if (v130)
    {
        v129 = (tGameObjectMain*)v266->des_o_Wall_GAO;
        if (v129)
        {
            if ((unsigned int)v129 <= 2)
                v129 = AI_MainActor[(uint32_t)v129 - 1];
            AI_EvalFunc_COLMapActivationSet_C(v129, 0xFFu, 0);
        }
        else
        {
            AI_EvalFunc_COLMapActivationSet_C(0, 0xFFu, 0);
        }
    }
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 256);
    if (!v266->i_islighted && !v266->des_i_generator)
    {
        v266->f_ray_lenght = 0.0;
        v128 = (tGameObjectMain*)v266->des_o_follow_dest;
        if (v128)
        {
            if ((unsigned int)v128 <= 2)
                v128 = AI_MainActor[(uint32_t)v128 - 1];
            v127 = v128;
        }
        else
        {
            v127 = 0;
        }
        if (v127)
        {
            v126 = (tGameObjectMain*)v266->des_o_follow_dest;
            if (v126)
            {
                if ((unsigned int)v126 <= 2)
                    v126 = AI_MainActor[(uint32_t)v126 - 1];
                OBJ_RestoreInitialPos(v126);
            }
            else
            {
                OBJ_RestoreInitialPos(0);
            }
        }
        if (v266->GFX_Halo != -1)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v278);
            GFXSetCurGlobal(v266->GFX_Halo);
            AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v278);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v173);
            AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v173);
            v238 = TIM_gf_dt;
            v125 = 3.0f * TIM_gf_dt;
            if (v125 < 0.0)
                v125 = 0.0;
            if (v125 > 1.0)
                v125 = 1.0;
            v279 = 0.0f * v125 + v266->GFX_Halo_Size * (1.0f - v125);
            v266->GFX_Halo_Size = v279;
            v187 = AI_EvalFunc_MATHRandFloat_C(-0.1f, 0.1f);
            v12 = v266->GFX_Halo_Size / 5.0f + v187;
            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, v12);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v266->GFX_Halo_Size);
            if (v266->GFX_Halo_Size < 0.2)
            {
                AI_EvalFunc_GFXDel_C_CURRENT(v266->GFX_Halo);
                v266->GFX_Halo = -1;
                v266->GFX_Halo_Size = 0.0;
            }
        }
        v233 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_mirror_mirror_loop_mirror_mirror_lightning_off);
        if (v233 != 1000)
            return v233;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    v266->i_islighted = 0;
    AI_EvalFunc_OBJ_CapaSet_C_CURRENT(256, 0);
    if (!v266->des_i_canreflect)
    {
        v170 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_mirror_mirror_loop_mirror_mirror_lightning_off);
        if (v170 != 1000)
            return v170;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    v198 = 0;
    if (v266->des_i_type)
    {
        if (v266->des_mouvement)
        {
            v243 = v266->v_init_pos;
            v13 = v266->des_f_amplitude / 2.0f;
            v11 = -(v266->des_f_amplitude / 2.0f);
            v1 = AI_EvalFunc_MATHRandFloat_C(v11, v13);
            v243.z = (float)v1 + v243.z;
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v243);
        }
        v231 = 0;
        v191 = 100000.0;
        v249 = -1;
        while (v231 < v266->des_i_numdest)
        {
            v117 = (tGameObjectMain*)v266->des_ao_dest[v231];
            if (v117)
            {
                if ((unsigned int)v117 <= 2)
                    v117 = AI_MainActor[(uint32_t)v117 - 1];
                v2 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, v117);
            }
            else
            {
                v2 = AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, 0);
            }
            v219 = v2;
            if (v2)
            {
                v116 = (tGameObjectMain*)v266->des_ao_dest[v231];
                if (v116)
                {
                    if ((unsigned int)v116 <= 2)
                        v116 = AI_MainActor[(uint32_t)v116 - 1];
                    AI_EvalFunc_OBJGetPos_C(v116, &v172);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v172);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v204);
                MATH_SubstractVec(&v143, &v172, &v204);
                v186 = MATH_VecLen(&v143);
                if (v186 < (double)v191)
                {
                    v249 = v231;
                    v191 = v186;
                }
            }
            ++v231;
        }
        if (v249 == -1)
        {
            if (v266->des_i_alone)
            {
                v251 = 6;
                if (v266->i_snd_cur_mat != 6 || !v266->i_snd_flag)
                {
                    v103 = (tGameObjectMain*)mirror_mirror_loop__PJ_Sally_gao;
                    if (mirror_mirror_loop__PJ_Sally_gao)
                    {
                        if ((unsigned int)mirror_mirror_loop__PJ_Sally_gao <= 2)
                            v103 = AI_MainActor[mirror_mirror_loop__PJ_Sally_gao - 1];
                        v6 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v103);
                    }
                    else
                    {
                        v6 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
                    }
                    v146 = (float)v6;
                    v266->i_snd_flag = v266->f_snd_dist_nosound >= (double)v146;
                    if (v266->i_snd_flag)
                    {
                        v266->i_snd_cur_mat = v251;
                        if (v266->i_request_play != -1)
                            SND_Release(v266->i_request_play);
                        v7 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, v266->i_snd_cur_mat, 0x18000000);
                        v266->i_request_play = v7;
                        v207 = SND_l_GetFreq(v266->i_request_play);
                        SND_SetFreq(v266->i_request_play, v266->f_freq_additionnelle_pour_dep + v207);
                        AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v266->i_request_play);
                    }
                    else if (v266->i_request_play != -1)
                    {
                        SND_Release(v266->i_request_play);
                        v266->i_request_play = -1;
                    }
                }
                v266->o_snd_old_target = (int)AI_gpst_CurrentGameObject;
                v266->i_GFX = v266->des_i_alone;
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v266->v_dir);
                v262 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_mirror_mirror_loop_mirror_mirror_lightning_on);
                if (v262 != 1000)
                    return v262;
                v160 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_mirror_mirror_loop_mirror_mirror_lightning_update);
                if (v160 != 1000)
                    return v160;
            }
            else
            {
                v237 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_mirror_mirror_loop_mirror_mirror_lightning_off);
                if (v237 != 1000)
                    return v237;
            }
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            return 1000;
        }
        v266->o_snd_cur_target = v266->des_ao_dest[v249];
        v115 = (tGameObjectMain*)v266->o_snd_cur_target;
        if (v115)
        {
            if ((unsigned int)v115 <= 2)
                v115 = AI_MainActor[(uint32_t)v115 - 1];
            v114 = v115;
        }
        else
        {
            v114 = 0;
        }
        v113 = (tGameObjectMain*)v266->o_snd_old_target;
        if (v113)
        {
            if ((unsigned int)v113 <= 2)
                v113 = AI_MainActor[(uint32_t)v113 - 1];
            v112 = v113;
        }
        else
        {
            v112 = 0;
        }
        if (v114 != v112 || !v266->i_snd_flag)
        {
            v111 = (tGameObjectMain*)mirror_mirror_loop__PJ_Sally_gao;
            if (mirror_mirror_loop__PJ_Sally_gao)
            {
                if ((unsigned int)mirror_mirror_loop__PJ_Sally_gao <= 2)
                    v111 = AI_MainActor[mirror_mirror_loop__PJ_Sally_gao - 1];
                v3 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v111);
            }
            else
            {
                v3 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
            }
            v147 = (float)v3;
            v266->i_snd_flag = v266->f_snd_dist_nosound >= (double)v147;
            if (v266->i_snd_flag)
            {
                if (!*(uint32_t*)mirror_mirror_loop_GST_Global_gao_i_map_just_reset)
                {
                    v110 = (tGameObjectMain*)v266->o_snd_cur_target;
                    if (v110)
                    {
                        if ((unsigned int)v110 <= 2)
                            v110 = AI_MainActor[(uint32_t)v110 - 1];
                        v109 = v110;
                    }
                    else
                    {
                        v109 = 0;
                    }
                    v108 = (tGameObjectMain*)v266->o_snd_old_target;
                    if (v108)
                    {
                        if ((unsigned int)v108 <= 2)
                            v108 = AI_MainActor[(uint32_t)v108 - 1];
                        v107 = v108;
                    }
                    else
                    {
                        v107 = 0;
                    }
                    if (v109 != v107)
                    {
                        v229 = AI_EvalFunc_MATHRandInt_C(8, 11);
                        v169 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(v229, -1.0);
                    }
                }
                v106 = (tGameObjectMain*)v266->o_snd_cur_target;
                if (v106)
                {
                    if ((unsigned int)v106 <= 2)
                        v106 = AI_MainActor[(uint32_t)v106 - 1];
                    v4 = AI_EvalFunc_OBJ_CustomBitsGet_C(v106);
                }
                else
                {
                    v4 = AI_EvalFunc_OBJ_CustomBitsGet_C(0);
                }
                v230 = v4;
                v251 = v4 >> 8;
                if (v4 >> 8 != v266->i_snd_cur_mat || v266->i_request_play == -1)
                {
                    v266->i_snd_cur_mat = v251;
                    if (v266->i_request_play != -1)
                        SND_Release(v266->i_request_play);
                    v5 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, v266->i_snd_cur_mat, 0x18000000);
                    v266->i_request_play = v5;
                    v180 = SND_l_GetFreq(v266->i_request_play);
                    SND_SetFreq(v266->i_request_play, v266->f_freq_additionnelle_pour_dep + v180);
                    AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v266->i_request_play);
                }
            }
            else if (v266->i_request_play != -1)
            {
                SND_Release(v266->i_request_play);
                v266->i_request_play = -1;
            }
        }
        v266->o_snd_old_target = v266->o_snd_cur_target;
        if (v266->des_i_spark)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v141);
            v105 = (tGameObjectMain*)v266->des_ao_dest[v249];
            if (v105)
            {
                if ((unsigned int)v105 <= 2)
                    v105 = AI_MainActor[(uint32_t)v105 - 1];
                AI_EvalFunc_OBJGetPos_C(v105, &v277);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v277);
            }
            MATH_SubstractVec(&v156, &v141, &v277);
            MATH_VecMultNormScalar(&v140, &v156, v186);
            v266->v_dir = v140;
        }
        v104 = (tGameObjectMain*)v266->des_ao_dest[v249];
        if (v104)
        {
            if ((unsigned int)v104 <= 2)
                v104 = AI_MainActor[(uint32_t)v104 - 1];
            AI_EvalFunc_OBJGetPos_C(v104, &v266->v_destpos);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v266->v_destpos);
        }
        v198 = (tAI*)v266->des_ao_dest[v249];
    }
    else
    {
        if (v266->des_i_generator)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v284);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v222);
            v124 = v266->des_f_maxlen;
            v184.x = v222.x * v124;
            v184.y = v222.y * v124;
            v184.z = v222.z * v124;
            MATH_VecAddVec(&a1, &v284, &v184);
            v266->v_destpos = a1;
        }
        else
        {
            AI_EvalFunc_MATHVecNormalize_C(&v266->v_entering, &v155);
            v266->v_entering = v155;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v206);
            v253 = v206.z * v266->v_entering.z + v206.y * v266->v_entering.y + v206.x * v266->v_entering.x;
            v288 = 2.0f * v253;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v227);
            v283.x = v227.x * v288;
            v283.y = v227.y * v288;
            v283.z = v227.z * v288;
            MATH_SubstractVec(&dest, &v266->v_entering, &v283);
            v = dest;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v196);
            v193 = -v.x;
            v194 = -v.y;
            v195 = -v.z;
            v123 = v266->des_f_maxlen;
            v244.x = -v.x * v123;
            v244.y = -v.y * v123;
            v244.z = -v.z * v123;
            MATH_VecAddVec(&v164, &v196, &v244);
            v266->v_destpos = v164;
        }
        if (v266->des_i_numdest)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v274);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v210);
            MATH_SubstractVec(&v264, &v266->v_destpos, &v210);
            v249 = AI_EvalFunc_ARRObjBestSight_C(
                (tGameObjectMain**)v266->des_ao_dest,
                v266->des_i_numdest,
                &v274,
                &v264,
                v266->i_coef_de_best_sight);
            v198 = (tAI*)v266->des_ao_dest[v249];
            v122 = (tGameObjectMain*)v198;
            if (v198)
            {
                if ((unsigned int)v198 <= 2)
                    v122 = AI_MainActor[(uint32_t)v198 - 1];
                AI_EvalFunc_OBJGetPos_C(v122, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v275);
            MATH_SubstractVec(&v175, &a3, &v275);
            AI_EvalFunc_MATHVecNormalize_C(&v175, &v282);
            v = v282;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v241);
            MATH_SubstractVec(&v150, &v266->v_destpos, &v241);
            AI_EvalFunc_MATHVecNormalize_C(&v150, &v177);
            v215 = v177;
            if (v266->des_f_angletoler == -1.0)
                v261 = *(float*)AI_EvalVarArray_C2((char*)&v266->__des_af_angletoler_Dimension1, v249);
            else
                v261 = v266->des_f_angletoler;
            v260 = v.z * v215.z + v.y * v215.y + v215.x * v.x;
            if (1.0 - v261 > v260)
            {
                v198 = 0;
            }
            else
            {
                v121 = (tGameObjectMain*)v198;
                if (v198)
                {
                    if ((unsigned int)v198 <= 2)
                        v121 = AI_MainActor[(uint32_t)v198 - 1];
                    AI_EvalFunc_OBJGetPos_C(v121, &v266->v_destpos);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v266->v_destpos);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v221);
                v120 = (tGameObjectMain*)v198;
                if (v198)
                {
                    if ((unsigned int)v198 <= 2)
                        v120 = AI_MainActor[(uint32_t)v198 - 1];
                    AI_EvalFunc_OBJGetPos_C(v120, &v201);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v201);
                }
                MATH_SubstractVec(&v245, &v221, &v201);
                v178 = MATH_VecLen(&v245);
                if (v266->f_ray_lenght <= (double)v178)
                {
                    v198 = 0;
                }
                else
                {
                    v119 = (tGameObjectMain*)v198;
                    if (v198)
                    {
                        if ((unsigned int)v198 <= 2)
                            v119 = AI_MainActor[(uint32_t)v198 - 1];
                        AI_EvalFunc_OBJGetPos_C(v119, &v205);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v205);
                    }
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v280);
                    MATH_SubstractVec(&v254, &v205, &v280);
                    AI_EvalFunc_MATHVecNormalize_C(&v254, &v167);
                    v = v167;
                    if (v266->des_i_backface)
                    {
                        v118 = (tGameObjectMain*)v198;
                        if (v198)
                        {
                            if ((unsigned int)v198 <= 2)
                                v118 = AI_MainActor[(uint32_t)v198 - 1];
                            AI_EvalFunc_OBJ_SightGet_C(v118, &v255);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_SightGet_C(0, &v255);
                        }
                        v287 = v255.z * v.z + v255.y * v.y + v.x * v255.x;
                        if (v287 > -0.1)
                            v198 = 0;
                    }
                }
            }
        }
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v214);
    MATH_VecAddVec(&v257, &v214, &v266->des_v_offset_rayon);
    v = v257;
    MATH_VecAddVec(&v226, &v266->v_destpos, &v266->des_v_offset_rayon);
    MATH_SubstractVec(&v163, &v226, &v);
    v215 = v163;
    v149 = MATH_VecLen(&v215);
    if (AI_EvalFunc_TIMEElapsed_C(*(float*)mirror_mirror_loop_GST_Global_gao_f_reinit_time, 1.0))
    {
        v162 = TIM_gf_dt;
        v266->f_ray_lenght = 120.0f * TIM_gf_dt + v266->f_ray_lenght;
    }
    else
    {
        v289 = TIM_gf_dt;
        v266->f_ray_lenght = 120000.0f * TIM_gf_dt + v266->f_ray_lenght;
    }
    if (v266->f_ray_lenght > (double)v149)
        v266->f_ray_lenght = v149 + 0.1f;
    if (v266->f_ray_lenght > (double)v149)
        v16 = v149;
    else
        v16 = v266->f_ray_lenght;
    v240 = v16;
    AI_EvalFunc_MATHVecSetNormVector_C(&v215, v16);
    MATH_VecAddVec(&v242, &v, &v215);
    v266->v_destpos = v242;
    v199 = 0;
    v151 = 0;
    v152 = 0;
    v153 = 0;
    v265.x = 0.0;
    v265.y = 0.0;
    v265.z = 0.0;
    if (!v266->des_i_decors)
    {
        v209 = 0;
        if (v266->des_i_detect_all)
        {
            v209 = -1;
        }
        else
        {
            if (v266->des_i_detect_ai)
                v209 |= 0x20u;
            if (v266->des_i_detect_meca)
                v209 |= 4u;
            if (v266->des_i_detect_anim)
                v209 |= 2u;
        }
        v199 = AI_EvalFunc_COLRayObjectVector_C(AI_gpst_CurrentGameObject, &v, &v215, v209, 0, 0, 1);
        if (!v266->i_je_paf_le_milit)
        {
            v252 = *(uint32_t*)(mirror_mirror_loop_univVars_Perso_ID_actor + 8);
            v102 = (tGameObjectMain*)v252;
            if (v252)
            {
                if (v252 <= 2)
                    v102 = AI_MainActor[v252 - 1];
                v101 = v102;
            }
            else
            {
                v101 = 0;
            }
            v100 = v199;
            if (v199)
            {
                if ((unsigned int)v199 <= 2)
                    v100 = AI_MainActor[(uint32_t)v199 - 1];
                v99 = v100;
            }
            else
            {
                v99 = 0;
            }
            if (v101 == v99)
                v199 = 0;
        }
        v98 = v199;
        if (v199)
        {
            if ((unsigned int)v199 <= 2)
                v98 = AI_MainActor[(uint32_t)v199 - 1];
            v97 = v98;
        }
        else
        {
            v97 = 0;
        }
        if (v97)
        {
            AI_EvalFunc_COLRayPosGet_C(AI_gpst_CurrentGameObject, &v197);
            v265 = v197;
            AI_EvalFunc_COLRayNormalGet_C(AI_gpst_CurrentGameObject, &v212);
            v270 = v212;
        }
        v96 = v199;
        if (v199)
        {
            if ((unsigned int)v199 <= 2)
                v96 = AI_MainActor[(uint32_t)v199 - 1];
            v95 = v96;
        }
        else
        {
            v95 = 0;
        }
        if (v95)
        {
            v94 = v199;
            if (v199)
            {
                if ((unsigned int)v199 <= 2)
                    v94 = AI_MainActor[(uint32_t)v199 - 1];
                v93 = v94;
            }
            else
            {
                v93 = 0;
            }
            v92 = (tGameObjectMain*)v198;
            if (v198)
            {
                if ((unsigned int)v198 <= 2)
                    v92 = AI_MainActor[(uint32_t)v198 - 1];
                v91 = v92;
            }
            else
            {
                v91 = 0;
            }
            if (v93 != v91)
            {
                AI_EvalFunc_COLRayPosGet_C(AI_gpst_CurrentGameObject, &v144);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v139);
                MATH_SubstractVec(&v211, &v144, &v139);
                v8 = MATH_VecLen(&v211);
                v266->f_ray_lenght = (float)v8;
                if (v266->des_i_force_axe_recul)
                {
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v281);
                    v271 = -v281.x;
                    v272 = -v281.y;
                    v273 = -v281.z;
                    v90 = &v266->msg.v1;
                    v266->msg.v1.x = -v281.x;
                    v90->y = v272;
                    v90->z = v273;
                }
                else
                {
                    v89 = v199;
                    if (v199)
                    {
                        if ((unsigned int)v199 <= 2)
                            v89 = AI_MainActor[(uint32_t)v199 - 1];
                        AI_EvalFunc_OBJGetPos_C(v89, &v168);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v168);
                    }
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v276);
                    MATH_SubstractVec(&v176, &v168, &v276);
                    v.x = v176.x;
                    v.y = v176.y;
                    v.z = 0.0;
                    MATH_NormalizeEqualVector(&v);
                    v215.z = 0.0;
                    MATH_NormalizeEqualVector(&v215);
                    AI_EvalFunc_MATHVecCrossProduct_C(&v, &v215, &v192);
                    v = v192;
                    AI_EvalFunc_MATHVecCrossProduct_C(&v, &v215, &v166);
                    v = v166;
                    v246 = -v166.x;
                    v247 = -v166.y;
                    v248 = -v166.z;
                    v88 = &v266->msg.v1;
                    v266->msg.v1.x = -v166.x;
                    v88->y = v247;
                    v88->z = v248;
                }
                v266->msg.f2 = 20.0;
                v87 = v199;
                if (v199)
                {
                    if ((unsigned int)v199 <= 2)
                        v87 = AI_MainActor[(uint32_t)v199 - 1];
                    AI_EvalFunc_MSGSend_C(v87, &v266->msg);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &v266->msg);
                }
                if (AI_EvalFunc_TIMEElapsed_C(v266->f_paf_time, 0.5))
                {
                    v266->f_paf_time = TIM_gf_MainClock - TIM_gf_LockTime;
                    v286 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(7, -1.0);
                }
                if (v266->des_i_type == 1 && !v266->des_i_generator)
                {
                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                    return 1000;
                }
                v86 = v199;
                if (v199)
                {
                    if ((unsigned int)v199 <= 2)
                        v86 = AI_MainActor[(uint32_t)v199 - 1];
                    v9 = AI_EvalFunc_OBJ_FlagsIdentityGet_C(v86);
                }
                else
                {
                    v9 = AI_EvalFunc_OBJ_FlagsIdentityGet_C(0);
                }
                v250 = v9;
                if ((v9 & 2) != 0)
                {
                    v85 = v199;
                    if (v199)
                    {
                        if ((unsigned int)v199 <= 2)
                            v85 = AI_MainActor[(uint32_t)v199 - 1];
                        AI_EvalFunc_OBJGetPos_C(v85, &v203);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v203);
                    }
                    v = v203;
                    v.z = v266->v_destpos.z;
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v165);
                    MATH_SubstractVec(&v285, &v, &v165);
                    v186 = MATH_VecLen(&v285);
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v200);
                    MATH_SubstractVec(&v171, &v266->v_destpos, &v200);
                    v = v171;
                    AI_EvalFunc_MATHVecSetNormVector_C(&v, v186);
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v138);
                    MATH_VecAddVec(&v256, &v, &v138);
                    v266->v_destpos = v256;
                }
                else
                {
                    AI_EvalFunc_COLRayPosGet_C(AI_gpst_CurrentGameObject, &v266->v_destpos);
                }
                v198 = 0;
            }
        }
        v249 = AI_EvalFunc_COLCrossObjectNumber_C();
        if (!v266->des_i_type && v249)
        {
            v266->msg.msgID = 41;
            --v249;
            while (v249 >= 0)
            {
                v199 = AI_EvalFunc_COLCrossObjectGet_C(AI_gpst_CurrentGameObject, v249);
                v84 = v199;
                if (v199)
                {
                    if ((unsigned int)v199 <= 2)
                        v84 = AI_MainActor[(uint32_t)v199 - 1];
                    AI_EvalFunc_MSGSend_C(v84, &v266->msg);
                }
                else
                {
                    AI_EvalFunc_MSGSend_C(0, &v266->msg);
                }
                v83 = v199;
                if (v199)
                {
                    if ((unsigned int)v199 <= 2)
                        v83 = AI_MainActor[(uint32_t)v199 - 1];
                    AI_EvalFunc_OBJ_CapaSet_C(v83, 0x100, 0);
                }
                else
                {
                    AI_EvalFunc_OBJ_CapaSet_C(0, 0x100, 0);
                }
                --v249;
            }
            v266->msg.msgID = 37;
        }
        v82 = (tGameObjectMain*)v266->des_o_follow_dest;
        if (v82)
        {
            if ((unsigned int)v82 <= 2)
                v82 = AI_MainActor[(uint32_t)v82 - 1];
            v81 = v82;
        }
        else
        {
            v81 = 0;
        }
        if (v81)
        {
            v80 = (tGameObjectMain*)v198;
            if (v198)
            {
                if ((unsigned int)v198 <= 2)
                    v80 = AI_MainActor[(uint32_t)v198 - 1];
                v79 = v80;
            }
            else
            {
                v79 = 0;
            }
            if (v79)
            {
                v78 = (tGameObjectMain*)v198;
                if (v198)
                {
                    if ((unsigned int)v198 <= 2)
                        v78 = AI_MainActor[(uint32_t)v198 - 1];
                    AI_EvalFunc_OBJGetPos_C(v78, &v266->v_destpos);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v266->v_destpos);
                }
            }
            v77 = (tGameObjectMain*)v266->des_o_follow_dest;
            if (v77)
            {
                if ((unsigned int)v77 <= 2)
                    v77 = AI_MainActor[(uint32_t)v77 - 1];
                AI_EvalFunc_OBJGetPos_C(v77, &a2);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a2);
            }
            a4 = TIM_gf_dt;
            AI_EvalFunc_MATHVecBlendRotate_C(&a2, &v266->v_destpos, TIM_gf_dt, &a5);
            v = a5;
            v76 = (tGameObjectMain*)v266->des_o_follow_dest;
            if (v76)
            {
                if ((unsigned int)v76 <= 2)
                    v76 = AI_MainActor[(uint32_t)v76 - 1];
                AI_EvalFunc_OBJSetPos_C(v76, &v);
            }
            else
            {
                AI_EvalFunc_OBJSetPos_C(0, &v);
            }
        }
    }
    v266->i_GFX = v266->i_baseGFX;
    v75 = (tGameObjectMain*)v198;
    if (v198)
    {
        if ((unsigned int)v198 <= 2)
            v75 = AI_MainActor[(uint32_t)v198 - 1];
        v74 = v75;
    }
    else
    {
        v74 = 0;
    }
    if (v74)
    {
        v73 = (tGameObjectMain*)v198;
        if (v198)
        {
            if ((unsigned int)v198 <= 2)
                v73 = AI_MainActor[(uint32_t)v198 - 1];
            AI_EvalFunc_OBJGetPos_C(v73, &v266->v_destpos);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v266->v_destpos);
        }
        v72 = (tGameObjectMain*)v198;
        if (v198)
        {
            if ((unsigned int)v198 <= 2)
                v72 = AI_MainActor[(uint32_t)v198 - 1];
            v10 = AI_EvalFunc_AIIsModel_C(v72, (tAIModel*)fctvar_mirror_mirror_loop_Arc_generateur_gao);
        }
        else
        {
            v10 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_mirror_mirror_loop_Arc_generateur_gao);
        }
        v267 = v10;
        if (!v10)
            goto LABEL_529;
        if (v198 == WOR_gst_Universe.univAIInstance)
        {
            v69 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v71 = (tGameObjectMain*)v198;
            if (v198)
            {
                if ((unsigned int)v198 <= 2)
                    v71 = AI_MainActor[(uint32_t)v198 - 1];
                v70 = v71;
            }
            else
            {
                v70 = 0;
            }
            v69 = v70->extended->AIInstance;
        }
        if (((tScriptStruct_Mirror*)(v69->instanceVarDataCurrent))->des_i_generator)// TODO FIXME ez mi? =>d9005d4d mirror_mirror
        {
        LABEL_529:
            v266->i_GFX &= ~5u;
        }
        else
        {
            if (v198 == WOR_gst_Universe.univAIInstance)
            {
                v66 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v68 = (tGameObjectMain*)v198;
                if (v198)
                {
                    if ((unsigned int)v198 <= 2)
                        v68 = AI_MainActor[(uint32_t)v198 - 1];
                    v67 = v68;
                }
                else
                {
                    v67 = 0;
                }
                v66 = v67->extended->AIInstance;
            }
            ((tScriptStruct_Mirror*)(v66->instanceVarDataCurrent))->i_islighted = 1;   // TODO FIXME meg ez is
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v228);
            v65 = (tGameObjectMain*)v198;
            if (v198)
            {
                if ((unsigned int)v198 <= 2)
                    v65 = AI_MainActor[(uint32_t)v198 - 1];
                AI_EvalFunc_OBJGetPos_C(v65, &v185);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v185);
            }
            MATH_SubstractVec(&v157, &v228, &v185);
            if (v198 == WOR_gst_Universe.univAIInstance)
            {
                v62 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v64 = (tGameObjectMain*)v198;
                if (v198)
                {
                    if ((unsigned int)v198 <= 2)
                        v64 = AI_MainActor[(uint32_t)v198 - 1];
                    v63 = v64;
                }
                else
                {
                    v63 = 0;
                }
                v62 = v63->extended->AIInstance;
            }
            ((tScriptStruct_Mirror*)(v62->instanceVarDataCurrent))->v_entering = v157;
            if (v198 == WOR_gst_Universe.univAIInstance)
            {
                v59 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v61 = (tGameObjectMain*)v198;
                if (v198)
                {
                    if ((unsigned int)v198 <= 2)
                        v61 = AI_MainActor[(uint32_t)v198 - 1];
                    v60 = v61;
                }
                else
                {
                    v60 = 0;
                }
                v59 = v60->extended->AIInstance;
            }
            ((tScriptStruct_Mirror*)(v59->instanceVarDataCurrent))->des_i_forceelec = v266->des_i_forceelec;
            if (v198 == WOR_gst_Universe.univAIInstance)
            {
                v56 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v58 = (tGameObjectMain*)v198;
                if (v198)
                {
                    if ((unsigned int)v198 <= 2)
                        v58 = AI_MainActor[(uint32_t)v198 - 1];
                    v57 = v58;
                }
                else
                {
                    v57 = 0;
                }
                v56 = v57->extended->AIInstance;
            }
            ((tScriptStruct_Mirror*)(v56->instanceVarDataCurrent))->des_i_forceinerte = v266->des_i_forceinerte;
            if (v198 == WOR_gst_Universe.univAIInstance)
            {
                v53 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v55 = (tGameObjectMain*)v198;
                if (v198)
                {
                    if ((unsigned int)v198 <= 2)
                        v55 = AI_MainActor[(uint32_t)v198 - 1];
                    v54 = v55;
                }
                else
                {
                    v54 = 0;
                }
                v53 = v54->extended->AIInstance;
            }
            ((tScriptStruct_Mirror*)(v53->instanceVarDataCurrent))->des_i_forcelum = v266->des_i_forcelum;
            if (v198 == WOR_gst_Universe.univAIInstance)
            {
                v50 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v52 = (tGameObjectMain*)v198;
                if (v198)
                {
                    if ((unsigned int)v198 <= 2)
                        v52 = AI_MainActor[(uint32_t)v198 - 1];
                    v51 = v52;
                }
                else
                {
                    v51 = 0;
                }
                v50 = v51->extended->AIInstance;
            }
            ((tScriptStruct_Mirror*)(v50->instanceVarDataCurrent))->des_i_forcechaleur = v266->des_i_forcechaleur;
            if (v198 == WOR_gst_Universe.univAIInstance)
            {
                v47 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v49 = (tGameObjectMain*)v198;
                if (v198)
                {
                    if ((unsigned int)v198 <= 2)
                        v49 = AI_MainActor[(uint32_t)v198 - 1];
                    v48 = v49;
                }
                else
                {
                    v48 = 0;
                }
                v47 = v48->extended->AIInstance;
            }
            ((tScriptStruct_Mirror*)(v47->instanceVarDataCurrent))->des_f_epaisseur_faisceau = v266->des_f_epaisseur_faisceau;
            if (v198 == WOR_gst_Universe.univAIInstance)
            {
                v44 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v46 = (tGameObjectMain*)v198;
                if (v198)
                {
                    if ((unsigned int)v198 <= 2)
                        v46 = AI_MainActor[(uint32_t)v198 - 1];
                    v45 = v46;
                }
                else
                {
                    v45 = 0;
                }
                v44 = v45->extended->AIInstance;
            }
            ((tScriptStruct_Mirror*)(v44->instanceVarDataCurrent))->des_f_radius = v266->des_f_radius;
            if (v198 == WOR_gst_Universe.univAIInstance)
            {
                v41 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v43 = (tGameObjectMain*)v198;
                if (v198)
                {
                    if ((unsigned int)v198 <= 2)
                        v43 = AI_MainActor[(uint32_t)v198 - 1];
                    v42 = v43;
                }
                else
                {
                    v42 = 0;
                }
                v41 = v42->extended->AIInstance;
            }
            ((tScriptStruct_Mirror*)(v41->instanceVarDataCurrent))->des_o_gaomat = v266->des_o_gaomat;
            if (v198 == WOR_gst_Universe.univAIInstance)
            {
                v38 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v40 = (tGameObjectMain*)v198;
                if (v198)
                {
                    if ((unsigned int)v198 <= 2)
                        v40 = AI_MainActor[(uint32_t)v198 - 1];
                    v39 = v40;
                }
                else
                {
                    v39 = 0;
                }
                v38 = v39->extended->AIInstance;
            }
            ((tScriptStruct_Mirror*)(v38->instanceVarDataCurrent))->des_i_submat = v266->des_i_submat;
            if (v198 == WOR_gst_Universe.univAIInstance)
            {
                v35 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v37 = (tGameObjectMain*)v198;
                if (v198)
                {
                    if ((unsigned int)v198 <= 2)
                        v37 = AI_MainActor[(uint32_t)v198 - 1];
                    v36 = v37;
                }
                else
                {
                    v36 = 0;
                }
                v35 = v36->extended->AIInstance;
            }
            ((tScriptStruct_Mirror*)(v35->instanceVarDataCurrent))->des_color_1 = v266->des_color_1;
            if (v198 == WOR_gst_Universe.univAIInstance)
            {
                v32 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v34 = (tGameObjectMain*)v198;
                if (v198)
                {
                    if ((unsigned int)v198 <= 2)
                        v34 = AI_MainActor[(uint32_t)v198 - 1];
                    v33 = v34;
                }
                else
                {
                    v33 = 0;
                }
                v32 = v33->extended->AIInstance;
            }
            ((tScriptStruct_Mirror*)(v32->instanceVarDataCurrent))->des_color_2 = v266->des_color_2;
            if (v198 == WOR_gst_Universe.univAIInstance)
            {
                v29 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v31 = (tGameObjectMain*)v198;
                if (v198)
                {
                    if ((unsigned int)v198 <= 2)
                        v31 = AI_MainActor[(uint32_t)v198 - 1];
                    v30 = v31;
                }
                else
                {
                    v30 = 0;
                }
                v29 = v30->extended->AIInstance;
            }
            if (((tScriptStruct_Mirror*)(v29->instanceVarDataCurrent))->des_i_transfert_auto)
            {
                if (v198 == WOR_gst_Universe.univAIInstance)
                {
                    v26 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v28 = (tGameObjectMain*)v198;
                    if (v198)
                    {
                        if ((unsigned int)v198 <= 2)
                            v28 = AI_MainActor[(uint32_t)v198 - 1];
                        v27 = v28;
                    }
                    else
                    {
                        v27 = 0;
                    }
                    v26 = v27->extended->AIInstance;
                }
                ((tScriptStruct_Mirror*)(v26->instanceVarDataCurrent))->des_i_numdest = v266->des_i_numdest - 1;
                v25 = (tGameObjectMain*)v198;
                if (v198)
                {
                    if ((unsigned int)v198 <= 2)
                        v25 = AI_MainActor[(uint32_t)v198 - 1];
                    v24 = v25;
                }
                else
                {
                    v24 = 0;
                }
                v268 = AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)v266->des_ao_dest, v266->des_i_numdest, v24);
                v266->des_ao_dest[v268] = *(&v266->__des_ao_dest_Dimension1 + v266->des_i_numdest--);
                if (v198 == WOR_gst_Universe.univAIInstance)
                {
                    v21 = WOR_gst_Universe.univAIInstance;
                }
                else
                {
                    v23 = (tGameObjectMain*)v198;
                    if (v198)
                    {
                        if ((unsigned int)v198 <= 2)
                            v23 = AI_MainActor[(uint32_t)v198 - 1];
                        v22 = v23;
                    }
                    else
                    {
                        v22 = 0;
                    }
                    v21 = v22->extended->AIInstance;
                }
                AI_EvalFunc_ARRObjCopy_C(((tScriptStruct_Mirror*)(v21->instanceVarDataCurrent))->des_ao_dest, v266->des_ao_dest, v266->des_i_numdest);
                v266->des_ao_dest[v266->des_i_numdest++] = (int)v198;
            }
        }
    }
    v266->i_flag_ON = 1;
    if (!v266->des_i_type && !v266->i_flag_1st_trame && (v266->i_GFX & 0x10) != 0 && v266->i_GFX != v266->i_sndGFX)
        v232 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(2, -1.0);
    if (!v266->des_i_type && !AI_EvalFunc_MTHVecNullEpsilon_C(&v265))
    {
        if (AI_EvalFunc_TIMEElapsed_C(v266->t_time_spark, v266->t_delay_spark))
        {
            v266->t_time_spark = TIM_gf_MainClock - TIM_gf_LockTime;
            v266->t_delay_spark = AI_EvalFunc_MATHRandFloat_C(0.050000001f, 0.1f);
            v266->spark = AI_EvalFunc_GFXAdd_C_CURRENT(9);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v266->spark, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v266->spark, 2, 1);
            v20 = (tGameObjectMain*)mirror_mirror_loop_SFX_light_smoke_gao;
            if (mirror_mirror_loop_SFX_light_smoke_gao)
            {
                if ((unsigned int)mirror_mirror_loop_SFX_light_smoke_gao <= 2)
                    v20 = AI_MainActor[mirror_mirror_loop_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v266->spark, v20, 5);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v266->spark, 0, 5);
            }
            GFXSetCurGlobal(v266->spark);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9000, 1.570796f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9001, 2.0f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9002, 5.0f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9003, 0.98000002f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9004, 0.2f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9005, 0.2f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9006, 0.40000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9007, 3.0f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9008, 0.0049999999f);
            AI_EvalFunc_GFXSeti_C_CURGLOB(9100, 3);
            AI_EvalFunc_GFXSeti_C_CURGLOB(9102, 24);
            v188 = AI_EvalFunc_MATHRandFloat_C(-0.15000001f, 0.15000001f);
            v158 = AI_EvalFunc_MATHRandFloat_C(-0.15000001f, 0.15000001f);
            v269 = AI_EvalFunc_MATHRandFloat_C(-0.15000001f, 0.15000001f);
            v225.x = v188;
            v225.y = v158;
            v225.z = v269;
            MATH_VecAddVec(&v183, &v265, &v225);
            AI_EvalFunc_GFXSetv_C_CURGLOB(9200, &v183);
            AI_EvalFunc_GFXSetv_C_CURGLOB(9201, &v270);
            v154.x = 0.0;
            v154.y = 0.0;
            v154.z = -3.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(9202, &v154);
            AI_EvalFunc_GFXSeti_C_CURGLOB(9101, v266->c_current_color);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9009, 0.2f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9010, 0.40000001f);
        }
    }
    if (v266->GFX_Halo == -1 && !v266->des_i_type)
    {
        v266->GFX_Halo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v266->GFX_Halo, 4, 1);
        v19 = (tGameObjectMain*)mirror_mirror_loop_SFX_light_smoke_gao;
        if (mirror_mirror_loop_SFX_light_smoke_gao)
        {
            if ((unsigned int)mirror_mirror_loop_SFX_light_smoke_gao <= 2)
                v19 = AI_MainActor[mirror_mirror_loop_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v266->GFX_Halo, v19, 0);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v266->GFX_Halo, 0, 0);
        }
        GFXSetCurGlobal(v266->GFX_Halo);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21100, 4);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21006, 0.75);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21104, 5);
    }
    if (v266->GFX_Halo != -1)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v220);
        GFXSetCurGlobal(v266->GFX_Halo);
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v220);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v174);
        AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v174);
        v218 = TIM_gf_dt;
        v18 = 10.0f * TIM_gf_dt;
        if (v18 < 0.0)
            v18 = 0.0;
        if (v18 > 1.0)
            v18 = 1.0;
        v259 = 5.0f * v18 + v266->GFX_Halo_Size * (1.0f - v18);
        v266->GFX_Halo_Size = v259;
        v145 = AI_EvalFunc_MATHRandFloat_C(-0.80000001f, 0.80000001f);
        v14 = v266->GFX_Halo_Size + v145;
        AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v14);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v266->c_current_color);
        v179 = AI_EvalFunc_MATHRandFloat_C(-0.1f, 0.1f);
        v15 = v266->GFX_Halo_Size / 5.0f + v179;
        AI_EvalFunc_GFXSetf_C_CURGLOB(21000, v15);
    }
    v217 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_mirror_mirror_loop_mirror_mirror_lightning_on);
    if (v217 != 1000)
        return v217;
    v258 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_mirror_mirror_loop_mirror_mirror_lightning_update);
    if (v258 == 1000)
        result = 1000;
    else
        result = v258;
    return result;
}

int mirror_mirror_lightning_update()
{
    int result; // eax
    tScriptStruct_Mirror* v1; // [esp+0h] [ebp-90h]
    Vec3 v2; // [esp+2Ch] [ebp-64h] BYREF
    int v3; // [esp+38h] [ebp-58h]
    Vec3 v4; // [esp+3Ch] [ebp-54h] BYREF
    Vec3 a1; // [esp+48h] [ebp-48h] BYREF
    float v6; // [esp+54h] [ebp-3Ch]
    Vec3 v7; // [esp+58h] [ebp-38h] BYREF
    float v8; // [esp+64h] [ebp-2Ch]
    float v9; // [esp+68h] [ebp-28h]
    float v10; // [esp+6Ch] [ebp-24h]
    int v11; // [esp+70h] [ebp-20h]
    tScriptStruct_Mirror* v12; // [esp+74h] [ebp-1Ch]
    Vec3 a2; // [esp+78h] [ebp-18h] BYREF
    Vec3 v14; // [esp+84h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Mirror*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v12 = v1;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_mirror_mirror_lightning_update_mirror_mirror_lightning_off = (int)AI_ul_RealResolveOneRefFunction(0x35009A5Cu);
    }
    v6 = AI_EvalFunc_OBJ_LodVisGet_C(AI_gpst_CurrentGameObject);
    if (v6 == 0.0)
    {
        v12->i_do_not_destroy_sound = 1;
        v11 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_mirror_mirror_lightning_update_mirror_mirror_lightning_off);
        if (v11 == 1000)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            result = v11;
        }
    }
    else
    {
        v9 = AI_EvalFunc_MATHRandFloat_C(0.0, 1.0);
        v3 = AI_EvalFunc_COLORBlend_C(v12->des_color_1, v12->des_color_2, v9);
        v12->c_current_color = v3;
        if (v12->i_ideclaire != -1)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v2);
            GFXSetCurGlobal(v12->i_ideclaire);
            AI_EvalFunc_GFXSetv_C_CURGLOB(2201, &v2);
            AI_EvalFunc_GFXSetv_C_CURGLOB(2200, &v12->v_destpos);
            AI_EvalFunc_GFXSeti_C_CURGLOB(2102, v12->c_current_color);
        }
        if (v12->i_idspark[0] != -1)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v7);
            GFXSetCurGlobal(v12->i_idspark[0]);
            AI_EvalFunc_GFXSetv_C_CURGLOB(9200, &v7);
            v14.x = -v12->v_dir.x;
            v14.y = -v12->v_dir.y;
            v14.z = -v12->v_dir.z;
            AI_EvalFunc_GFXSetv_C_CURGLOB(9201, &v14);
        }
        if (v12->i_idspark[1] != -1)
        {
            GFXSetCurGlobal(v12->i_idspark[1]);
            AI_EvalFunc_GFXSetv_C_CURGLOB(9200, &v12->v_destpos);
            AI_EvalFunc_GFXSetv_C_CURGLOB(9201, &v12->v_dir);
        }
        if (v12->i_idhalo[0] != -1)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v4);
            GFXSetCurGlobal(v12->i_idhalo[0]);
            AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v4);
            v10 = AI_EvalFunc_MATHRandFloat_C(0.80000001f, 1.2f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, v10);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v12->c_current_color);
        }
        if (v12->i_idhalo[1] != -1)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
            MATH_VecAddVec(&a1, &a2, &v12->des_v_offset);
            GFXSetCurGlobal(v12->i_idhalo[1]);
            AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &a1);
            v8 = AI_EvalFunc_MATHRandFloat_C(0.80000001f, 1.2f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, v8);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v12->c_current_color);
        }
        result = 1000;
    }
    return result;
}

int mirror_mirror_lightning_on()
{
    int result; // eax
    tScriptStruct_Mirror* v1; // [esp+4h] [ebp-58h]
    tGameObjectMain* v2; // [esp+Ch] [ebp-50h]
    tGameObjectMain* v3; // [esp+14h] [ebp-48h]
    tGameObjectMain* v4; // [esp+1Ch] [ebp-40h]
    Vec3 a3; // [esp+24h] [ebp-38h] BYREF
    float v6; // [esp+30h] [ebp-2Ch]
    Vec3 v7; // [esp+34h] [ebp-28h] BYREF
    int v8; // [esp+40h] [ebp-1Ch]
    int a1; // [esp+48h] [ebp-14h]
    int v10; // [esp+4Ch] [ebp-10h]
    tScriptStruct_Mirror* v11; // [esp+50h] [ebp-Ch]
    int v12; // [esp+54h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Mirror*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v11 = v1;
    a1 = 0;
    v8 = 0;
    v12 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_mirror_mirror_lightning_on_mirror_mirror_lightning_off = (int)AI_ul_RealResolveOneRefFunction(0x35009A5Cu);
    }
    v11->i_sndGFX = v11->i_GFX;
    v6 = AI_EvalFunc_OBJ_LodVisGet_C(AI_gpst_CurrentGameObject);
    if (v6 == 0.0)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else if (v11->i_GFX == v11->i_oldGFX)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else if (v11->i_oldGFX == -1
        || (v10 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_mirror_mirror_lightning_on_mirror_mirror_lightning_off),
            v10 == 1000))
    {
        v11->i_oldGFX = v11->i_GFX;
        v11->i_sndGFX = v11->i_GFX;
        if ((v11->i_GFX & 0x10) != 0)
        {
            v11->i_ideclaire = AI_EvalFunc_GFXAdd_C_CURRENT(2);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v11->i_ideclaire, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v11->i_ideclaire, 2, 1);
            v4 = (tGameObjectMain*)v11->des_o_gaomat;
            if (v4)
            {
                if ((unsigned int)v4 <= 2)
                    v4 = AI_MainActor[(uint32_t)v4 - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v11->i_ideclaire, v4, v11->des_i_submat);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v11->i_ideclaire, 0, v11->des_i_submat);
            }
            GFXSetCurGlobal(v11->i_ideclaire);
            AI_EvalFunc_GFXSeti_C_CURGLOB(2100, 5);
            AI_EvalFunc_GFXSeti_C_CURGLOB(2101, 2);
            AI_EvalFunc_GFXSetf_C_CURGLOB(2000, v11->des_f_epaisseur_faisceau);
            AI_EvalFunc_GFXSetf_C_CURGLOB(2001, v11->des_f_radius);
            if (v11->des_i_orientation == 1)
            {
                a3.x = 1.0;
                a3.y = 0.0;
                a3.z = 0.0;
                AI_EvalFunc_GFXSetv_C_CURRENT(v11->i_ideclaire, 2202, &a3);
            }
        }
        else
        {
            v11->i_ideclaire = -1;
        }
        v12 = 0;
        v8 = 1;
        while (v12 < 2)
        {
            if ((v8 & v11->i_GFX) != 0)
            {
                v11->i_idspark[v12] = AI_EvalFunc_GFXAdd_C_CURRENT(9);
                a1 = v11->i_idspark[v12];
                AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 0, 1);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 2, 1);
                v3 = (tGameObjectMain*)v11->des_o_gaomat;
                if (v3)
                {
                    if ((unsigned int)v3 <= 2)
                        v3 = AI_MainActor[(uint32_t)v3 - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, v3, 5);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, 0, 5);
                }
                v7.x = 0.0;
                v7.y = 0.0;
                v7.z = -10.0;
                GFXSetCurGlobal(a1);
                AI_EvalFunc_GFXSetv_C_CURGLOB(9202, &v7);
                AI_EvalFunc_GFXSeti_C_CURGLOB(9100, 30);
                AI_EvalFunc_GFXSeti_C_CURGLOB(9102, 1);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9000, 0.80000001f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9001, 10.0f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9002, 20.0f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9003, 0.69999999f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9004, 1.0f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9005, 0.1f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9006, 0.60000002f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9007, 1.5f);
                AI_EvalFunc_GFXSetf_C_CURGLOB(9008, 0.02f);
            }
            else
            {
                v11->i_idspark[v12] = -1;
            }
            v8 *= 2;
            if (v11->des_i_halo && (v8 & v11->i_GFX) != 0)
            {
                v11->i_idhalo[v12] = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                a1 = v11->i_idhalo[v12];
                AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 4, 1);
                v2 = (tGameObjectMain*)v11->des_o_gaomat;
                if (v2)
                {
                    if ((unsigned int)v2 <= 2)
                        v2 = AI_MainActor[(uint32_t)v2 - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, v2, v11->des_i_submat_halo);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, 0, v11->des_i_submat_halo);
                }
                GFXSetCurGlobal(a1);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 1.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.5);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 0xFFFFFF);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
            }
            else
            {
                v11->i_idhalo[v12] = -1;
            }
            v8 *= 2;
            ++v12;
        }
        result = 1000;
    }
    else
    {
        result = v10;
    }
    return result;
}

int mirror_mirror_lightning_off()
{
    int result; // eax
    tScriptStruct_Mirror* v1; // [esp+0h] [ebp-28h]
    int v2; // [esp+1Ch] [ebp-Ch]
    int v3; // [esp+20h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Mirror*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (v1->i_do_not_destroy_sound)
    {
        v1->i_do_not_destroy_sound = 0;
    }
    else
    {
        if (v1->i_request_play != -1)
        {
            SND_Release(v1->i_request_play);
            v1->i_request_play = -1;
            v1->i_snd_cur_mat = -1;
            v1->o_snd_old_target = 0;
        }
        if (v1->i_sndGFX != -1)
            v1->i_sndGFX = -1;
    }
    if (v1->i_oldGFX == -1)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        if ((v1->i_oldGFX & 0x10) != 0)
            AI_EvalFunc_GFXDel_C_CURRENT(v1->i_ideclaire);
        if ((v1->i_oldGFX & 1) != 0)
        {
            v2 = AI_EvalFunc_GFXGeti_C(AI_gpst_CurrentGameObject, v1->i_idspark[0], 9102);
            AI_EvalFunc_GFXSeti_C_CURRENT(v1->i_idspark[0], 9102, v2 | 4);
        }
        if ((v1->i_oldGFX & 4) != 0)
        {
            v3 = AI_EvalFunc_GFXGeti_C(AI_gpst_CurrentGameObject, v1->i_idspark[1], 9102);
            AI_EvalFunc_GFXSeti_C_CURRENT(v1->i_idspark[1], 9102, v3 | 4);
        }
        if ((v1->i_oldGFX & 2) != 0)
            AI_EvalFunc_GFXDel_C_CURRENT(v1->i_idhalo[0]);
        if ((v1->i_oldGFX & 8) != 0)
            AI_EvalFunc_GFXDel_C_CURRENT(v1->i_idhalo[1]);
        v1->i_oldGFX = -1;
        result = 1000;
    }
    return result;
}

int mirror_mirror_beforedisplay()
{
    int result; // eax
    tScriptStruct_Mirror* v1; // [esp+0h] [ebp-1Ch]
    tGameObjectMain* v2; // [esp+4h] [ebp-18h]
    tGameObjectMain* v3; // [esp+8h] [ebp-14h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-10h]
    tGameObjectMain* v5; // [esp+10h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Mirror*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v5 = (tGameObjectMain*)v1->des_o_const_gao;
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
        goto LABEL_23;
    v3 = (tGameObjectMain*)v1->des_o_const_gao1;
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
    LABEL_23:
        if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(256))
        {
            AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v1->des_c_const_allum, 0x620022C9);
            AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v1->des_c_const_allum, 0x62002182);
        }
        else
        {
            AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v1->des_c_const_base, 0x620022C9);
            AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v1->des_c_const_base, 0x62002182);
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

int mirror_mirror_afterdisplay()
{
    int result; // eax
    tScriptStruct_Mirror* v1; // [esp+0h] [ebp-18h]
    tGameObjectMain* v2; // [esp+4h] [ebp-14h]
    tGameObjectMain* v3; // [esp+8h] [ebp-10h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-Ch]
    tGameObjectMain* v5; // [esp+10h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Mirror*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v5 = (tGameObjectMain*)v1->des_o_const_gao;
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
        goto LABEL_17;
    v3 = (tGameObjectMain*)v1->des_o_const_gao1;
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
    LABEL_17:
        AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v1->des_c_const_base, 0x620022C9);
        AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v1->des_c_const_base, 0x62002182);
        result = 1000;
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int mirror_mirror_exec_set_snd_pos()
{
    int v0; // eax
    int v1; // eax
    int result; // eax
    tScriptStruct_Mirror* v3; // [esp+0h] [ebp-108h]
    float v4; // [esp+24h] [ebp-E4h]
    Vec3 a2; // [esp+40h] [ebp-C8h] BYREF
    float v6; // [esp+4Ch] [ebp-BCh]
    float v7; // [esp+50h] [ebp-B8h]
    Vec3 v8; // [esp+54h] [ebp-B4h] BYREF
    Vec3 a3; // [esp+64h] [ebp-A4h] BYREF
    Vec3 v10; // [esp+70h] [ebp-98h] BYREF
    float v11; // [esp+7Ch] [ebp-8Ch]
    float v12; // [esp+80h] [ebp-88h]
    float v13; // [esp+84h] [ebp-84h]
    Vec3 v14; // [esp+88h] [ebp-80h]
    Vec3 v15; // [esp+98h] [ebp-70h] BYREF
    Vec3 a1; // [esp+A4h] [ebp-64h] BYREF
    Vec3 v17; // [esp+B0h] [ebp-58h] BYREF
    Vec3 dest; // [esp+C0h] [ebp-48h] BYREF
    Vec3 v19; // [esp+D0h] [ebp-38h] BYREF
    tScriptStruct_Mirror* v20; // [esp+DCh] [ebp-2Ch]
    Vec3 v21; // [esp+E0h] [ebp-28h] BYREF
    Vec3 v22; // [esp+ECh] [ebp-1Ch] BYREF
    Vec3 v23; // [esp+F8h] [ebp-10h] BYREF
    float v24; // [esp+104h] [ebp-4h]

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    v14.x = 1.0;
    v14.y = 0.0;
    v14.z = 0.0;
    a2.x = 1.0;
    a2.y = 0.0;
    a2.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_Mirror*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v20 = v3;
    v6 = 0.0;
    v24 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        mirror_mirror_exec_set_snd_pos_PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
    }
    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)mirror_mirror_exec_set_snd_pos_PJ_Sally_gao, &a3);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v19);
    MATH_SubstractVec(&dest, &a3, &v19);
    v14 = dest;
    if (v20->i_flag_ON && (v7 = v14.z * v14.z + v14.y * v14.y + v14.x * v14.x, v7 < 400.0))
    {
        if (v20->i_snd_prisme_loop == -1)
        {
            v0 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 1, 0x18000000);
            v20->i_snd_prisme_loop = v0;
            AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v20->i_snd_prisme_loop);
        }
    }
    else if (v20->i_snd_prisme_loop != -1)
    {
        SND_Release(v20->i_snd_prisme_loop);
        v20->i_snd_prisme_loop = -1;
    }
    if (!v20->i_flag_ON)
        goto LABEL_23;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v15);
    MATH_SubstractVec(&v22, &v20->v_destpos, &v15);
    a1 = v22;
    v24 = MATH_VecLen(&a1);
    v4 = 1.0f / v24;
    a1.x = a1.x * v4;
    a1.y = a1.y * v4;
    a1.z = a1.z * v4;
    v6 = a1.z * v14.z + a1.y * v14.y + v14.x * a1.x;
    if (v6 < 0.0 || v6 >(double)v24)
    {
        if (v6 >= 0.0)
        {
            a2 = v20->v_destpos;
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v17);
            a2 = v17;
        }
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v21);
        v11 = a1.x * v6;
        v12 = a1.y * v6;
        v13 = a1.z * v6;
        a2.x = v21.x + v11;
        a2.y = v21.y + v12;
        a2.z = v21.z + v13;
    }
    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)mirror_mirror_exec_set_snd_pos_PJ_Sally_gao, &v8);
    MATH_SubstractVec(&v23, &a2, &v8);
    v14 = v23;
    v6 = v14.z * v14.z + v14.y * v14.y + v14.x * v14.x;
    if (v6 < 400.0)
    {
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)mirror_mirror_exec_set_snd_pos_PJ_Sally_gao, &v10);
        if (v20->i_snd_lumiere_loop == -1)
        {
            v1 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000000);
            v20->i_snd_lumiere_loop = v1;
            AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v20->i_snd_lumiere_loop);
        }
        AI_EvalFunc_SNDInstPosVSet_C(v20->i_snd_lumiere_loop, &a2);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
    LABEL_23:
        if (v20->i_snd_lumiere_loop != -1)
        {
            SND_Release(v20->i_snd_lumiere_loop);
            v20->i_snd_lumiere_loop = -1;
        }
        result = 1000;
    }
    return result;
}