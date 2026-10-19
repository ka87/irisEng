#include "ai_compiledscripts_inc.h"

//Barrique_Barrique_init
int fct_Barrique_Barrique_init_Barrique_Barrique_mode_balance;
int fct_Barrique_Barrique_init_Barrique_Barrique_reflex;

//Barrique_Barrique_reflex
int Barrique_Barrique_reflex__PJ_Sally_gao;
int Barrique_Barrique_reflex__PJ_Sally_gao_current_pm_mode;
int Barrique_Barrique_reflex__PJ_Sally_gao_current_pm_priority;
int Barrique_Barrique_reflex_GST_Board_panel_gao;
int Barrique_Barrique_reflex_GST_Board_panel_gao_i_sally_priority;
int Barrique_Barrique_reflex_GST_Board_panel_gao_o_sally_contextuel;
int Barrique_Barrique_reflex_GST_Board_panel_gao_i_sally_action;
int Barrique_Barrique_reflex_GST_Board_panel_gao_v_sally_offset;
int Barrique_Barrique_reflex_GST_Board_panel_gao_i_force_interface;

int Barrique_Barrique_init()
{
    int result; // eax
    tScriptStruct_Barrique* v1; // [esp+0h] [ebp-10h]
    int v2; // [esp+8h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Barrique*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Barrique_Barrique_init_Barrique_Barrique_mode_balance = (int)AI_ul_RealResolveOneRefFunction(0x72004BC6u);
        fct_Barrique_Barrique_init_Barrique_Barrique_reflex = (int)AI_ul_RealResolveOneRefFunction(0x72004BC7u);
    }
    AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x8000, 0);
    AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 2, 0);
    v1->des_f_dist_propose_kick = v1->des_f_dist_propose_kick * v1->des_f_dist_propose_kick;
    AI_EvalFunc_AITrackChange_C(1, (tAIFunction*)fct_Barrique_Barrique_init_Barrique_Barrique_mode_balance);
    v2 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Barrique_Barrique_init_Barrique_Barrique_reflex);
    if (v2 == 1000)
        result = 1000;
    else
        result = v2;
    return result;
}

int Barrique_Barrique_mode_balance()
{
    double v0; // st7
    double v1; // st7
    double v2; // st7
    float v4; // [esp+8h] [ebp-558h]
    float v5; // [esp+Ch] [ebp-554h]
    float v6; // [esp+10h] [ebp-550h]
    float v7; // [esp+14h] [ebp-54Ch]
    float v8; // [esp+18h] [ebp-548h]
    float v9; // [esp+1Ch] [ebp-544h]
    tScriptStruct_Barrique* v10; // [esp+20h] [ebp-540h]
    Vec3* v11; // [esp+30h] [ebp-530h]
    Vec3* v12; // [esp+34h] [ebp-52Ch]
    float v13; // [esp+38h] [ebp-528h]
    Vec3* v14; // [esp+68h] [ebp-4F8h]
    Vec3* v15; // [esp+6Ch] [ebp-4F4h]
    float v16; // [esp+70h] [ebp-4F0h]
    float v17; // [esp+78h] [ebp-4E8h]
    float v18; // [esp+7Ch] [ebp-4E4h]
    float v19; // [esp+80h] [ebp-4E0h]
    float v20; // [esp+A4h] [ebp-4BCh]
    Vec3* v21; // [esp+BCh] [ebp-4A4h]
    Vec3* v22; // [esp+C0h] [ebp-4A0h]
    float v23; // [esp+C4h] [ebp-49Ch]
    float v24; // [esp+CCh] [ebp-494h]
    Vec3* v25; // [esp+D4h] [ebp-48Ch]
    float v26; // [esp+E0h] [ebp-480h]
    Vec3* v27; // [esp+114h] [ebp-44Ch]
    Vec3* v28; // [esp+118h] [ebp-448h]
    float v29; // [esp+124h] [ebp-43Ch]
    float v30; // [esp+128h] [ebp-438h]
    Vec3 v31; // [esp+150h] [ebp-410h] BYREF
    float v32; // [esp+15Ch] [ebp-404h]
    float v33; // [esp+160h] [ebp-400h]
    float v34; // [esp+164h] [ebp-3FCh]
    Vec3 v35; // [esp+168h] [ebp-3F8h] BYREF
    Vec3 v36; // [esp+174h] [ebp-3ECh] BYREF
    float v37; // [esp+18Ch] [ebp-3D4h]
    float v38; // [esp+190h] [ebp-3D0h]
    float v39; // [esp+194h] [ebp-3CCh]
    Vec3 v40; // [esp+198h] [ebp-3C8h] BYREF
    Vec3 v41; // [esp+1A4h] [ebp-3BCh]
    Vec3 v42; // [esp+1B0h] [ebp-3B0h] BYREF
    float v43; // [esp+1BCh] [ebp-3A4h]
    float v44; // [esp+1C0h] [ebp-3A0h]
    float v45; // [esp+1C4h] [ebp-39Ch]
    float v46; // [esp+1C8h] [ebp-398h]
    Vec3 v47; // [esp+1CCh] [ebp-394h] BYREF
    Vec3 v48; // [esp+1D8h] [ebp-388h] BYREF
    Vec3 v49; // [esp+1E4h] [ebp-37Ch]
    Vec3 v50; // [esp+1F0h] [ebp-370h] BYREF
    int v52; // [esp+200h] [ebp-360h]
    float v53; // [esp+204h] [ebp-35Ch]
    Vec3 v54; // [esp+214h] [ebp-34Ch]
    Vec3 v55; // [esp+220h] [ebp-340h] BYREF
    Vec3 v56; // [esp+22Ch] [ebp-334h]
    float v57; // [esp+238h] [ebp-328h]
    Vec3 v58; // [esp+23Ch] [ebp-324h]
    Vec3 v59; // [esp+248h] [ebp-318h] BYREF
    Vec3 v60; // [esp+254h] [ebp-30Ch]
    Vec3 v61; // [esp+260h] [ebp-300h] BYREF
    Vec3 v62; // [esp+270h] [ebp-2F0h] BYREF
    Vec3 v63; // [esp+27Ch] [ebp-2E4h] BYREF
    Vec3 v64; // [esp+288h] [ebp-2D8h] BYREF
    Vec3 dest; // [esp+294h] [ebp-2CCh] BYREF
    Vec3 v66; // [esp+2A0h] [ebp-2C0h] BYREF
    Vec3 v67; // [esp+2ACh] [ebp-2B4h] BYREF
    float v68; // [esp+2B8h] [ebp-2A8h]
    int v69; // [esp+2BCh] [ebp-2A4h]
    Vec3 v70; // [esp+2C0h] [ebp-2A0h] BYREF
    Vec3 v71; // [esp+2CCh] [ebp-294h] BYREF
    Vec3 v72; // [esp+2E4h] [ebp-27Ch] BYREF
    int v73; // [esp+2F0h] [ebp-270h]
    Vec3 v74; // [esp+2F4h] [ebp-26Ch] BYREF
    Vec3 v75; // [esp+300h] [ebp-260h] BYREF
    Vec3 v76; // [esp+30Ch] [ebp-254h] BYREF
    float v77; // [esp+318h] [ebp-248h]
    float v78; // [esp+31Ch] [ebp-244h]
    Vec3 v79; // [esp+320h] [ebp-240h] BYREF
    float v80; // [esp+32Ch] [ebp-234h]
    float v81; // [esp+330h] [ebp-230h]
    float v82; // [esp+334h] [ebp-22Ch]
    Vec3 v83; // [esp+338h] [ebp-228h]
    Vec3 v84; // [esp+344h] [ebp-21Ch]
    int v87; // [esp+374h] [ebp-1ECh]
    float v88; // [esp+378h] [ebp-1E8h]
    float v89; // [esp+37Ch] [ebp-1E4h]
    Vec3 v90; // [esp+380h] [ebp-1E0h] BYREF
    Vec3 v91; // [esp+38Ch] [ebp-1D4h]
    Vec3 a1; // [esp+398h] [ebp-1C8h] BYREF
    Vec3 v93; // [esp+3A4h] [ebp-1BCh] BYREF
    Vec3 v94; // [esp+3B0h] [ebp-1B0h] BYREF
    float v95; // [esp+3BCh] [ebp-1A4h]
    float v96; // [esp+3C0h] [ebp-1A0h]
    Vec3 v97; // [esp+3C4h] [ebp-19Ch] BYREF
    float v98; // [esp+3D0h] [ebp-190h]
    float v99; // [esp+3D4h] [ebp-18Ch]
    float v100; // [esp+3D8h] [ebp-188h]
    Vec3 v101; // [esp+3DCh] [ebp-184h] BYREF
    Vec3 v102; // [esp+3E8h] [ebp-178h] BYREF
    float v103; // [esp+400h] [ebp-160h]
    float v104; // [esp+408h] [ebp-158h]
    Vec3 v105; // [esp+410h] [ebp-150h]
    Vec3 v106; // [esp+41Ch] [ebp-144h] BYREF
    Vec3 v107; // [esp+428h] [ebp-138h] BYREF
    float v108; // [esp+434h] [ebp-12Ch]
    float v109; // [esp+440h] [ebp-120h]
    Vec3 v110; // [esp+444h] [ebp-11Ch] BYREF
    float v111; // [esp+450h] [ebp-110h]
    float v112; // [esp+454h] [ebp-10Ch]
    float v113; // [esp+458h] [ebp-108h]
    Vec3 v114; // [esp+45Ch] [ebp-104h] BYREF
    Vec3 v115; // [esp+468h] [ebp-F8h] BYREF
    Vec3 a3; // [esp+474h] [ebp-ECh] BYREF
    float v117; // [esp+480h] [ebp-E0h]
    float v118; // [esp+484h] [ebp-DCh]
    Vec3 v119; // [esp+490h] [ebp-D0h]
    Vec3 v120; // [esp+49Ch] [ebp-C4h]
    Vec3 a2; // [esp+4A8h] [ebp-B8h] BYREF
    Vec3 v122; // [esp+4B4h] [ebp-ACh] BYREF
    Vec3 v124; // [esp+4CCh] [ebp-94h] BYREF
    tScriptStruct_Barrique* v125; // [esp+4D8h] [ebp-88h]
    Vec3 v126; // [esp+4DCh] [ebp-84h] BYREF
    Vec3 v127; // [esp+4E8h] [ebp-78h] BYREF
    Vec3 v128; // [esp+4F4h] [ebp-6Ch] BYREF
    Vec3 v129; // [esp+500h] [ebp-60h] BYREF
    Vec3 v130; // [esp+50Ch] [ebp-54h] BYREF
    float v131; // [esp+518h] [ebp-48h]
    float v132; // [esp+51Ch] [ebp-44h]
    float v133; // [esp+520h] [ebp-40h]
    Vec3 v134; // [esp+530h] [ebp-30h] BYREF
    Vec3 v135; // [esp+53Ch] [ebp-24h]
    Vec3 v136; // [esp+548h] [ebp-18h]
    float v137; // [esp+558h] [ebp-8h]
    int v138; // [esp+55Ch] [ebp-4h]

    v58.x = 1.0;
    v58.y = 0.0;
    v58.z = 0.0;
    v71.x = 1.0;
    v71.y = 0.0;
    v71.z = 0.0;
    v97.x = 1.0;
    v97.y = 0.0;
    v97.z = 0.0;
    v114.x = 1.0;
    v114.y = 0.0;
    v114.z = 0.0;
    v75.x = 1.0;
    v75.y = 0.0;
    v75.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    v49.x = 1.0;
    v49.y = 0.0;
    v49.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    v93.x = 1.0;
    v93.y = 0.0;
    v93.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v10 = (tScriptStruct_Barrique*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v10 = 0;
    v125 = v10;
    v57 = 0.0;
    v73 = 0;
    v109 = 0.0;
    if (v10->i_current_mode)
    {
        v125->i_current_mode = 0;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v125->v_init_pos);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&a2);
        MATH_VecMultScalar(&v106, &a2, 0.30000001f);
        MATH_VecAddVec(&v125->v_grav_pos, &v125->v_init_pos, &v106);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v50);
        MATH_VecMultScalar(&v94, &v50, 0.5);
        MATH_VecAddVec(&v128, &v125->v_init_pos, &v94);
        a3 = v128;
        MATH_SubstractVec(&dest, &v125->v_grav_pos, &a3);
        v0 = MATH_VecLen(&dest);
        v125->f_norm_OG = (float)v0;
        MATH_SubstractVec(&v129, &v125->v_grav_pos, &a3);
        AI_EvalFunc_MATHVecNormalize_C(&v129, &v72);
        v71 = v72;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v102);
        AI_EvalFunc_MATHVecCrossProduct_C(&v71, &v102, &v59);
        v114 = v59;
        MATH_SubstractVec(&v134, &v125->v_init_pos, &a3);
        v1 = AI_EvalFunc_MATHVecDotProduct_C(&v134, &v71);
        v125->f_norm_OH = (float)v1;
        MATH_SubstractVec(&v70, &v125->v_init_pos, &a3);
        v2 = AI_EvalFunc_MATHVecDotProduct_C(&v70, &v114);
        v125->f_norm_HI = (float)v2;
        v93 = a3;
        v30 = v125->f_norm_OH;
        v44 = v71.x * v30;
        v45 = v71.y * v30;
        v46 = v71.z * v30;
        v93.x = a3.x + v44;
        v93.y = a3.y + v45;
        v93.z = a3.z + v46;
        if (AI_EvalFunc_TIMEElapsed_C(v125->f_time_bing, 0.2f))
        {
            v125->f_time_bing = TIM_gf_MainClock - TIM_gf_LockTime;
            v73 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(0, 0.40000001f);
            v138 = SND_l_GetFreq(v73);
            v9 = (float)v138;
            SND_SetFreq(v73, (unsigned int)(v9 * 0.5f));
        }
    }
    v108 = TIM_gf_dt;
    v109 = v125->f_rotation_speed * TIM_gf_dt;
    AI_EvalFunc_OBJRotateGlobalZ_C(AI_gpst_CurrentGameObject, v109);
    v77 = TIM_gf_dt;
    v125->f_rotation_speed = v125->f_rotation_speed - v125->f_rotation_friction * TIM_gf_dt;
    v78 = AI_EvalFunc_MTHFloatSign_C(v125->f_rotation_friction);
    v29 = v125->f_rotation_speed * v78;
    if (v29 < 0.0)
        v8 = 0.0;
    else
        v8 = v125->f_rotation_speed * v78;
    v118 = v8;
    v125->f_rotation_speed = v8;
    if (v125->i_flag_stop_dyn)
    {
        AI_EvalFunc_OBJSetPos_C_CURRENT(&v125->v_init_pos);
        v76.x = 0.0;
        v76.y = 0.0;
        v76.z = 1.0;
        AI_EvalFunc_OBJ_BankingSet_C(AI_gpst_CurrentGameObject, &v76);
    }
    else
    {
        v55.x = 0.0;
        v55.y = 0.0;
        v55.z = -10.0;
        v132 = TIM_gf_dt;
        MATH_VecMultScalar(&v90, &v55, TIM_gf_dt);
        v49 = v90;
        v28 = &v125->v_grav_speed;
        v125->v_grav_speed.x = v90.x + v125->v_grav_speed.x;
        v28->y = v49.y + v28->y;
        v28->z = v49.z + v28->z;
        v124.x = v125->v_grav_speed.x * 0.1f;
        v124.y = v125->v_grav_speed.y * 0.1f;
        v124.z = v125->v_grav_speed.z * 0.050000001f;
        v95 = TIM_gf_dt;
        MATH_VecMultScalar(&v101, &v124, TIM_gf_dt);
        v27 = &v125->v_grav_speed;
        v125->v_grav_speed.x = v125->v_grav_speed.x - v101.x;
        v27->y = v27->y - v101.y;
        v27->z = v27->z - v101.z;
        v97 = v125->v_grav_pos;
        v131 = TIM_gf_dt;
        v64.x = TIM_gf_dt * v125->v_grav_speed.x;
        v64.y = v125->v_grav_speed.y * TIM_gf_dt;
        v64.z = v125->v_grav_speed.z * TIM_gf_dt;
        MATH_VecAddVec(&v125->v_grav_pos, &v125->v_grav_pos, &v64);
        MATH_SubstractVec(&v115, &v125->v_grav_pos, &v125->v_init_pos);
        a1.x = v115.x;
        a1.y = v115.y;
        a1.z = 0.0;
        v58.x = v115.x;
        v58.y = v115.y;
        v58.z = 0.0;
        if (MATH_b_NulVector(&a1))
        {
            v93 = v125->v_init_pos;
            v56.x = 0.0;
            v56.y = 0.0;
            v56.z = 1.0;
            v74.x = 0.0 * 0.30000001f;
            v74.y = v74.x;
            v74.z = 1.0 * 0.30000001f;
            MATH_VecAddVec(&v125->v_grav_pos, &v125->v_init_pos, &v74);
            v41.x = 0.0;
            v41.y = 0.0;
            v41.z = 1.0;
            v43 = 1.0f * v125->v_grav_speed.z + 0.0f * v125->v_grav_speed.y + 0.0f * v125->v_grav_speed.x;
            v87 = 0;
            v88 = 0.0;
            v89 = 1.0;
            v13 = 2.0f * v43;
            v12 = &v125->v_grav_speed;
            v125->v_grav_speed.x = 0.0f * v13;
            v12->y = v88 * v13;
            v12->z = v89 * v13;
            v11 = &v125->v_grav_speed;
            v125->v_grav_speed.x = v125->v_grav_speed.x * 0.89999998f;
            v11->y = v11->y * 0.89999998f;
            v11->z = v11->z * 0.89999998f;
            v136.x = 0.0;
            v136.y = 0.0;
            v136.z = 1.0;
            v133 = 1.0f * v125->v_grav_speed.z + 0.0f * v125->v_grav_speed.y + 0.0f * v125->v_grav_speed.x;
            if (v133 < (double)v49.z)
                v125->i_flag_stop_dyn = 1;
        }
        else
        {
            MATH_NormalizeEqualVector(&a1);
            v63.x = a1.x * 0.5f;
            v63.y = a1.y * 0.5f;
            v63.z = a1.z * 0.5f;
            MATH_VecAddVec(&v122, &v125->v_init_pos, &v63);
            a3 = v122;
            MATH_SubstractVec(&v126, &v125->v_grav_pos, &a3);
            v103 = v126.z * a1.z + v126.y * a1.y + a1.x * v126.x;
            if (v103 > 0.0)
            {
                v83.x = 0.0;
                v83.y = 0.0;
                v83.z = 1.0;
                v26 = v125->f_norm_OG;
                v110.x = 0.0f * v26;
                v110.y = v110.x;
                v110.z = 1.0f * v26;
                MATH_VecAddVec(&v67, &a3, &v110);
                v125->v_grav_pos = v67;
                v80 = a1.x * 0.0099999998f;
                v81 = a1.y * 0.0099999998f;
                v82 = a1.z * 0.0099999998f;
                v25 = &v125->v_grav_pos;
                v125->v_grav_pos.x = v125->v_grav_pos.x - v80;
                v25->y = v25->y - v81;
                v25->z = v25->z - v82;
            }
            v137 = a1.z * v125->v_IO.z + a1.y * v125->v_IO.y + a1.x * v125->v_IO.x;
            if (v137 >= 0.0)
            {
                MATH_SubstractVec(&v31, &v125->v_grav_pos, &a3);
                AI_EvalFunc_MATHVecNormalize_C(&v31, &v66);
                v71 = v66;
                v20 = v125->f_norm_OG;
                v42.x = v66.x * v20;
                v42.y = v66.y * v20;
                v42.z = v66.z * v20;
                MATH_VecAddVec(&v127, &a3, &v42);
                v125->v_grav_pos = v127;
                v47.x = 0.0;
                v47.y = 0.0;
                v47.z = 1.0;
                AI_EvalFunc_MATHVecCrossProduct_C(&a1, &v47, &v40);
                v114 = v40;
                AI_EvalFunc_MATHVecCrossProduct_C(&v114, &v71, &v79);
                v114 = v79;
                v93 = a3;
                v19 = v125->f_norm_OH;
                v32 = v71.x * v19;
                v33 = v71.y * v19;
                v34 = v71.z * v19;
                v93.x = a3.x + v32;
                v93.y = a3.y + v33;
                v93.z = a3.z + v34;
                v18 = v125->f_norm_HI;
                v111 = v79.x * v18;
                v112 = v79.y * v18;
                v113 = v79.z * v18;
                v93.x = v93.x + v111;
                v93.y = v93.y + v112;
                v93.z = v93.z + v113;
                if (v125->v_init_pos.z > (double)v93.z)
                {
                    if (AI_EvalFunc_TIMEElapsed_C(v125->f_time_bing, 0.2f))
                    {
                        v125->f_time_bing = TIM_gf_MainClock - TIM_gf_LockTime;
                        v17 = -v125->v_grav_speed.z / 2.0f;
                        if (v17 < 0.0)
                            v5 = 0.0;
                        else
                            v5 = -v125->v_grav_speed.z / 2.0f;
                        v57 = v5 * 0.25f;
                        v73 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(0, v57);
                        v52 = SND_l_GetFreq(v73);
                        v4 = (float)v52;
                        SND_SetFreq(v73, (unsigned int)(v4 * 0.5f));
                    }
                    v105.x = 0.0;
                    v105.y = 0.0;
                    v105.z = 1.0;
                    v96 = 1.0f * v125->v_grav_speed.z + 0.0f * v125->v_grav_speed.y + 0.0f * v125->v_grav_speed.x;
                    v119.x = 0.0;
                    v119.y = 0.0;
                    v119.z = 1.0;
                    v16 = 2.0f * v96;
                    v98 = 0.0f * v16;
                    v99 = v98;
                    v100 = 1.0f * v16;
                    v15 = &v125->v_grav_speed;
                    v125->v_grav_speed.x = v125->v_grav_speed.x - v98;
                    v15->y = v15->y - v99;
                    v15->z = v15->z - v100;
                    v14 = &v125->v_grav_speed;
                    v125->v_grav_speed.x = v125->v_grav_speed.x * 0.89999998f;
                    v14->y = v14->y * 0.89999998f;
                    v14->z = v14->z * 0.89999998f;
                    v54.x = 0.0;
                    v54.y = 0.0;
                    v54.z = 1.0;
                    v35.x = 0.0 * 0.30000001;
                    v35.y = v35.x;
                    v35.z = 1.0 * 0.30000001f;
                    MATH_VecAddVec(&v125->v_grav_pos, &v125->v_init_pos, &v35);
                    v93 = v125->v_init_pos;
                    v135.x = 0.0;
                    v135.y = 0.0;
                    v135.z = 1.0;
                    v53 = 1.0f * v125->v_grav_speed.z + 0.0f * v125->v_grav_speed.y + 0.0f * v125->v_grav_speed.x;
                    if (v53 < (double)v49.z)
                        v125->i_flag_stop_dyn = 1;
                }
                MATH_SubstractVec(&v107, &v125->v_grav_pos, &v97);
                v117 = TIM_gf_dt;
                MATH_VecMultNormScalar(&v48, &v107, TIM_gf_dt);
                v125->v_grav_speed = v48;
            }
            else
            {
                if (AI_EvalFunc_TIMEElapsed_C(v125->f_time_bing, 0.2f))
                {
                    v125->f_time_bing = TIM_gf_MainClock - TIM_gf_LockTime;
                    v24 = -v125->v_grav_speed.z / 2.0f;
                    if (v24 < 0.0)
                        v7 = 0.0;
                    else
                        v7 = -v125->v_grav_speed.z / 2.0f;
                    v57 = v7 * 0.25f;
                    v73 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(0, v57);
                    v69 = SND_l_GetFreq(v73);
                    v6 = (float)v69;
                    SND_SetFreq(v73, (unsigned int)(v6 * 0.5f));
                }
                v91.x = 0.0;
                v91.y = 0.0;
                v91.z = 1.0;
                v68 = 1.0f * v125->v_grav_speed.z + 0.0f * v125->v_grav_speed.y + 0.0f * v125->v_grav_speed.x;
                v84.x = 0.0;
                v84.y = 0.0;
                v84.z = 1.0;
                v23 = 2.0f * v68;
                v37 = 0.0f * v23;
                v38 = v37;
                v39 = 1.0f * v23;
                v22 = &v125->v_grav_speed;
                v125->v_grav_speed.x = v125->v_grav_speed.x - v37;
                v22->y = v22->y - v38;
                v22->z = v22->z - v39;
                v21 = &v125->v_grav_speed;
                v125->v_grav_speed.x = v125->v_grav_speed.x * 0.89999998f;
                v21->y = v21->y * 0.89999998f;
                v21->z = v21->z * 0.89999998f;
                v120.x = 0.0;
                v120.y = 0.0;
                v120.z = 1.0;
                v36.x = 0.0 * 0.30000001;
                v36.y = v36.x;
                v36.z = 1.0 * 0.30000001f;
                MATH_VecAddVec(&v125->v_grav_pos, &v125->v_init_pos, &v36);
                v93 = v125->v_init_pos;
                v60.x = 0.0;
                v60.y = 0.0;
                v60.z = 1.0;
                v104 = 1.0f * v125->v_grav_speed.z + 0.0f * v125->v_grav_speed.y + 0.0f * v125->v_grav_speed.x;
                if (v104 < (double)v49.z)
                    v125->i_flag_stop_dyn = 1;
            }
            v125->v_IO = a1;
        }
        MATH_SubstractVec(&v61, &v125->v_grav_pos, &v93);
        AI_EvalFunc_MATHVecNormalize_C(&v61, &v130);
        v75 = v130;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v62);
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v62, &v75);
        AI_EvalFunc_OBJSetPos_C_CURRENT(&v93);
    }
    return 1000;
}

int Barrique_Barrique_reflex()
{
    double v0; // st7
    float v2; // [esp+8h] [ebp-148h]
    float v3; // [esp+Ch] [ebp-144h]
    float v4; // [esp+10h] [ebp-140h]
    float v5; // [esp+14h] [ebp-13Ch]
    tScriptStruct_Barrique* v6; // [esp+18h] [ebp-138h]
    Vec3* v7; // [esp+1Ch] [ebp-134h]
    Vec3* v8; // [esp+20h] [ebp-130h]
    Vec3* v9; // [esp+24h] [ebp-12Ch]
    Vec3* v10; // [esp+28h] [ebp-128h]
    tGameObjectMain* v11; // [esp+2Ch] [ebp-124h]
    tGameObjectMain* v12; // [esp+30h] [ebp-120h]
    Vec3* v13; // [esp+34h] [ebp-11Ch]
    Vec3* v14; // [esp+38h] [ebp-118h]
    Vec3* v15; // [esp+3Ch] [ebp-114h]
    tGameObjectMain* v16; // [esp+44h] [ebp-10Ch]
    tGameObjectMain* v17; // [esp+4Ch] [ebp-104h]
    tGameObjectMain* v18; // [esp+50h] [ebp-100h]
    tGameObjectMain* v19; // [esp+54h] [ebp-FCh]
    tGameObjectMain* v20; // [esp+5Ch] [ebp-F4h]
    Vec3 v21; // [esp+60h] [ebp-F0h] BYREF
    float v22; // [esp+6Ch] [ebp-E4h]
    float v23; // [esp+70h] [ebp-E0h]
    void* Src; // [esp+78h] [ebp-D8h]
    Vec3 dest; // [esp+7Ch] [ebp-D4h] BYREF
    float v26; // [esp+94h] [ebp-BCh]
    float v27; // [esp+98h] [ebp-B8h]
    tGameObjectMain* v28; // [esp+9Ch] [ebp-B4h]
    float v29; // [esp+A0h] [ebp-B0h]
    float v30; // [esp+A4h] [ebp-ACh]
    Vec3 a2; // [esp+B4h] [ebp-9Ch] BYREF
    void* v32; // [esp+C0h] [ebp-90h]
    int v33; // [esp+C4h] [ebp-8Ch]
    float v34; // [esp+C8h] [ebp-88h]
    float v35; // [esp+CCh] [ebp-84h]
    float v36; // [esp+DCh] [ebp-74h]
    Vec3 v37; // [esp+E0h] [ebp-70h] BYREF
    float v38; // [esp+ECh] [ebp-64h]
    float v39; // [esp+F0h] [ebp-60h]
    float v40; // [esp+F4h] [ebp-5Ch]
    int v41; // [esp+104h] [ebp-4Ch]
    float v42; // [esp+108h] [ebp-48h]
    Vec3 a3; // [esp+10Ch] [ebp-44h] BYREF
    tScriptStruct_Barrique* v44; // [esp+124h] [ebp-2Ch]
    BOOL v45; // [esp+128h] [ebp-28h]
    Vec3 v46; // [esp+138h] [ebp-18h] BYREF
    int v47; // [esp+144h] [ebp-Ch]
    int v48; // [esp+148h] [ebp-8h]
    float v49; // [esp+14Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_Barrique*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v44 = v6;
    v41 = 0;
    v28 = 0;
    v40 = 0.0;
    v45 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Barrique_Barrique_reflex__PJ_Sally_gao = AI_ul_RealResolveOneRefObject(0x71003F5Au);
        AI2C_ResolveBuffer(
            &Barrique_Barrique_reflex__PJ_Sally_gao_current_pm_mode,
            (void*)Barrique_Barrique_reflex__PJ_Sally_gao,
            572);
        AI2C_ResolveBuffer(
            &Barrique_Barrique_reflex__PJ_Sally_gao_current_pm_priority,
            (void*)Barrique_Barrique_reflex__PJ_Sally_gao,
            576);
        Barrique_Barrique_reflex_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        AI2C_ResolveBuffer(
            &Barrique_Barrique_reflex_GST_Board_panel_gao_i_sally_priority,
            (void*)Barrique_Barrique_reflex_GST_Board_panel_gao,
            396);
        AI2C_ResolveBuffer(
            &Barrique_Barrique_reflex_GST_Board_panel_gao_o_sally_contextuel,
            (void*)Barrique_Barrique_reflex_GST_Board_panel_gao,
            388);
        AI2C_ResolveBuffer(
            &Barrique_Barrique_reflex_GST_Board_panel_gao_i_sally_action,
            (void*)Barrique_Barrique_reflex_GST_Board_panel_gao,
            412);
        AI2C_ResolveBuffer(
            &Barrique_Barrique_reflex_GST_Board_panel_gao_v_sally_offset,
            (void*)Barrique_Barrique_reflex_GST_Board_panel_gao,
            400);
        AI2C_ResolveBuffer(
            &Barrique_Barrique_reflex_GST_Board_panel_gao_i_force_interface,
            (void*)Barrique_Barrique_reflex_GST_Board_panel_gao,
            3292);
    }
    v28 = AI_gpst_CurrentGameObject;
    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Barrique_Barrique_reflex__PJ_Sally_gao, &a3);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v37);
    if (v37.z + 1.0 > a3.z)
    {
        v20 = (tGameObjectMain*)Barrique_Barrique_reflex__PJ_Sally_gao;
        if (Barrique_Barrique_reflex__PJ_Sally_gao)
        {
            if ((unsigned int)Barrique_Barrique_reflex__PJ_Sally_gao <= 2)
                v20 = AI_MainActor[Barrique_Barrique_reflex__PJ_Sally_gao - 1];
            v0 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v20);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
        }
        v23 = (float)v0;
        if (v23 < (double)v44->des_f_dist_propose_kick)
        {
            AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Barrique_Barrique_reflex__PJ_Sally_gao, &v21);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
            AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Barrique_Barrique_reflex__PJ_Sally_gao, &v46);
            MATH_SubstractVec(&dest, &a2, &v46);
            v39 = dest.z * v21.z + dest.y * v21.y + v21.x * dest.x;
            if (v39 > 0.0)
            {
                v47 = (unsigned __int16)*(uint32_t*)(Barrique_Barrique_reflex__PJ_Sally_gao + 4);
                if ((v47 & 1) != 0)
                    v45 = *(uint32_t*)Barrique_Barrique_reflex__PJ_Sally_gao_current_pm_mode == 7
                    || *(int*)Barrique_Barrique_reflex__PJ_Sally_gao_current_pm_priority < 20;
                else
                    v45 = 0;
                if (v45 && *(float*)Barrique_Barrique_reflex_GST_Board_panel_gao_i_sally_priority < 50.0)
                {
                    *(uint32_t*)Barrique_Barrique_reflex_GST_Board_panel_gao_o_sally_contextuel = (uint32_t)AI_gpst_CurrentGameObject;
                    *(uint32_t*)Barrique_Barrique_reflex_GST_Board_panel_gao_i_sally_priority = 0x42480000;
                    *(uint32_t*)Barrique_Barrique_reflex_GST_Board_panel_gao_i_sally_action = 15;
                    *(uint32_t*)Barrique_Barrique_reflex_GST_Board_panel_gao_v_sally_offset = 0;
                    *(uint32_t*)(Barrique_Barrique_reflex_GST_Board_panel_gao_v_sally_offset + 4) = 0;
                    *(uint32_t*)(Barrique_Barrique_reflex_GST_Board_panel_gao_v_sally_offset + 8) = 0;
                    *(uint32_t*)Barrique_Barrique_reflex_GST_Board_panel_gao_i_force_interface = 1;
                }
            }
        }
    }
    Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v44->m_msg, Src, sizeof(v44->m_msg));
    while (v44->m_msg.msgID)
    {
        if (v44->m_msg.msgID == 37)
        {
            v41 = v44->m_msg.Int1 & 7;
            if (v41)
            {
                v19 = (tGameObjectMain*)v44->des_o_active;
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
                    v17 = (tGameObjectMain*)v44->des_o_active;
                    if (v17)
                    {
                        if ((unsigned int)v17 <= 2)
                            v17 = AI_MainActor[(uint32_t)v17 - 1];
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(v17, 0);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_FlagInactiveSet_C(0, 0);
                    }
                    v16 = (tGameObjectMain*)v44->des_o_active;
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
                }
                v48 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(0, -1.0);
                v14 = &v44->v_paf_dir;
                v15 = &v44->m_msg.v1;
                v44->v_paf_dir.x = v44->m_msg.v1.x;
                v14->y = v15->y;
                v14->z = v15->z;
                v33 = 897988541;
                v34 = 0.0;
                v35 = 0.0;
                v13 = &v44->v_paf_dir;
                v44->v_paf_dir.x = v44->v_paf_dir.x + 0.000001f;
                v13->y = v34 + v13->y;
                v13->z = v35 + v13->z;
                AI_EvalFunc_MATHVecSetHorzNormalize_C(&v44->v_paf_dir);
                v44->i_flag_stop_dyn = 0;
                v12 = v44->m_msg.gao;
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
                if (v11 == (tGameObjectMain*)Barrique_Barrique_reflex__PJ_Sally_gao)
                {
                    v40 = AI_EvalFunc_MATHRandFloat_C(2.0, 3.0);
                    v29 = AI_EvalFunc_MATHRandFloat_C(-1.0, 1.0);
                    v22 = AI_EvalFunc_MTHFloatSign_C(v29);
                    v44->f_rotation_speed = v40 * 0.75f * v22 + v44->f_rotation_speed;
                    v5 = (float)fabs(v44->f_rotation_speed);
                    v30 = v5;
                    if (v5 > 5.0)
                        v4 = 5.0;
                    else
                        v4 = v30;
                    v42 = v4;
                    v36 = AI_EvalFunc_MTHFloatSign_C(v44->f_rotation_speed);
                    v44->f_rotation_speed = v42 * v36;
                    v49 = AI_EvalFunc_MTHFloatSign_C(v44->f_rotation_speed);
                    v44->f_rotation_friction = v49 + v49;
                    v3 = (float)fabs(v44->f_rotation_speed);
                    v38 = v3;
                    if (v3 > 5.0)
                        v2 = 5.0;
                    else
                        v2 = v38;
                    v26 = v2;
                    v27 = AI_EvalFunc_MTHFloatSign_C(v44->f_rotation_speed);
                    v44->f_rotation_speed = v26 * v27;
                    v10 = &v44->v_paf_dir;
                    v9 = &v44->v_grav_speed;
                    v44->v_grav_speed.x = v40 * v44->v_paf_dir.x;
                    v9->y = v10->y * v40;
                    v9->z = v10->z * v40;
                }
                else
                {
                    v40 = AI_EvalFunc_MATHRandFloat_C(0.75, 1.5);
                    v8 = &v44->v_paf_dir;
                    v7 = &v44->v_grav_speed;
                    v44->v_grav_speed.x = v40 * v44->v_paf_dir.x;
                    v7->y = v8->y * v40;
                    v7->z = v8->z * v40;
                }
            }
        }
        else if (v44->m_msg.msgID == 10)
        {
            v44->m_msg.msgID = 21;
            v44->m_msg.gao = v28;
            v44->m_msg.gao2 = v28;
            v44->m_msg.Int1 = 1;
            v44->m_msg.Int2 = 0;
            v44->m_msg.f2 = 1.0;
            v44->m_msg.f1 = 0.0;
            v44->m_msg.v1.x = 0.0;
            v44->m_msg.v1.y = 0.0;
            v44->m_msg.v1.z = 0.0;
            v44->m_msg.v2.x = 0.0;
            v44->m_msg.v2.y = 0.0;
            v44->m_msg.v2.z = 0.0;
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)Barrique_Barrique_reflex__PJ_Sally_gao, &v44->m_msg);
        }
        v32 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v44->m_msg, v32, sizeof(v44->m_msg));
    }
    return 1000;
}