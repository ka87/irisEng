#include "ai_compiledscripts_inc.h"

//torpille_tor_mode_mer
int fct_torpille_tor_mode_mer_torpille_tor_mode_air;
int fct_torpille_tor_mode_mer_torpille_tor_mode_mer;
int fct_torpille_tor_mode_mer_torpille_tor_mode_explosion;
int fctvar_torpille_tor_mode_mer_aircraft;
int fct_torpille_tor_mode_mer_aircraft_torpille_tor_exec_trace;
int torpille_tor_mode_mer_SFX_light_smoke_gao;
int fct_torpille_tor_mode_mer_torpille_tor_exec_test_collision;

//torpille_tor_exec_visuel
int torpille_tor_exec_visuel_SFX_light_smoke_gao;

//torpille_tor_mode_air
int fctvar_torpille_tor_mode_air_aircraft;
int fct_torpille_tor_mode_air_torpille_tor_mode_air;
int fct_torpille_tor_mode_air_torpille_tor_mode_explosion;
int fct_torpille_tor_mode_air_torpille_tor_exec_test_collision;

//torpille_tor_exec_trace
int torpille_tor_exec_trace_PJ_aircraft_material_gao;

//torpille_tor_exec_test_collision
int fct_torpille_tor_exec_test_collision_torpille_tor_exec_send_paf;
int fct_torpille_tor_exec_test_collision_torpille_tor_mode_air;
int fct_torpille_tor_exec_test_collision_torpille_tor_mode_mer;
int fct_torpille_tor_exec_test_collision_torpille_tor_mode_explosion;
int fctvar_torpille_tor_exec_test_collision_camera;
int fctvar_torpille_tor_exec_test_collision_torpille;

//torpille_tor_init
int torpille_tor_init_univVars;
int torpille_tor_init_univVars_Flag_speciaux_sauve;
int torpille_tor_init_GST_Global_gao;
int torpille_tor_init_GST_Global_gao_i_map_just_reset;
int fct_torpille_tor_init_torpille_tor_exec_visuel;
int fct_torpille_tor_init_torpille_tor_exec_before_display;
int fct_torpille_tor_init_torpille_tor_exec_after_display;
int torpille_tor_init_GST_Global_gao_o_targets;
int torpille_tor_init_GST_Global_gao_i_targets_nb;
int torpille_tor_init_GST_Global_gao_v_targets_offset;
int fct_torpille_tor_init_torpille_tor_mode_mer;
int torpille_tor_init_univVars_world_ID;
int fct_torpille_tor_init_torpille_tor_reflex;

//torpille_tor_reflex
int torpille_tor_reflex_GST_Global_gao;
int torpille_tor_reflex_GST_Global_gao_f_dist_mine;
int fctvar_torpille_tor_reflex_torpille;
int fctvar_torpille_tor_reflex_Hovercraft_shoot;
int fct_torpille_tor_reflex_torpille_tor_exec_send_paf;
int fct_torpille_tor_reflex_torpille_tor_mode_mer;
int fct_torpille_tor_reflex_torpille_tor_mode_air;
int fct_torpille_tor_reflex_torpille_tor_mode_explosion;
int torpille_tor_reflex__PJ_Aircraft_gao;
int torpille_tor_reflex_SFX_light_smoke_gao;

//torpille_tor_mode_explosion
int torpille_tor_mode_explosion_univVars;
int torpille_tor_mode_explosion_univVars_Flag_speciaux_sauve;
int torpille_tor_mode_explosion_GST_Global_gao;
int torpille_tor_mode_explosion_GST_Global_gao_o_targets;
int torpille_tor_mode_explosion_GST_Global_gao_i_targets_nb;
int torpille_tor_mode_explosion_GST_Global_gao_v_targets_offset;
int torpille_tor_mode_explosion_GST_Global_gao_o_selected_target;
int torpille_tor_mode_explosion_GST_Global_gao_i_selected_target_nb;
int torpille_tor_mode_explosion_GST_Global_gao_v_selected_target_offset;
int fct_torpille_tor_mode_explosion_torpille_tor_exec_visuel;
int fct_torpille_tor_mode_explosion_torpille_tor_exec_send_paf;
int torpille_tor_mode_explosion_SFX_explosion_gao;
int fct_torpille_tor_mode_explosion_torpille_tor_mode_explosion;

int torpille_tor_mode_mer()
{
    unsigned int v0; // eax
    int result; // eax
    unsigned int v2; // eax
    BOOL v3; // eax
    BOOL v4; // eax
    unsigned int v5; // eax
    BOOL v6; // eax
    int v7; // eax
    BOOL v8; // eax
    double v9; // st7
    double v10; // st7
    float v11; // [esp+0h] [ebp-504h]
    float v12; // [esp+4h] [ebp-500h]
    float v13; // [esp+4h] [ebp-500h]
    float v14; // [esp+8h] [ebp-4FCh]
    float v15; // [esp+Ch] [ebp-4F8h]
    float v16; // [esp+10h] [ebp-4F4h]
    float v17; // [esp+14h] [ebp-4F0h]
    tScriptStruct_PNJTorpille* v18; // [esp+18h] [ebp-4ECh]
    tGameObjectMain* v19; // [esp+28h] [ebp-4DCh]
    float v20; // [esp+3Ch] [ebp-4C8h]
    float v21; // [esp+48h] [ebp-4BCh]
    tGameObjectMain* v22; // [esp+58h] [ebp-4ACh]
    float v23; // [esp+64h] [ebp-4A0h]
    tGameObjectMain* v24; // [esp+70h] [ebp-494h]
    tGameObjectMain* v25; // [esp+78h] [ebp-48Ch]
    tGameObjectMain* v26; // [esp+80h] [ebp-484h]
    float v27; // [esp+8Ch] [ebp-478h]
    float v28; // [esp+90h] [ebp-474h]
    tGameObjectMain* v29; // [esp+94h] [ebp-470h]
    tGameObjectMain* v30; // [esp+98h] [ebp-46Ch]
    float v31; // [esp+9Ch] [ebp-468h]
    tGameObjectMain* v32; // [esp+ACh] [ebp-458h]
    tGameObjectMain* v33; // [esp+B0h] [ebp-454h]
    tGameObjectMain* v34; // [esp+B4h] [ebp-450h]
    tGameObjectMain* v35; // [esp+BCh] [ebp-448h]
    tGameObjectMain* v36; // [esp+C4h] [ebp-440h]
    tGameObjectMain* v37; // [esp+C8h] [ebp-43Ch]
    tGameObjectMain* v38; // [esp+CCh] [ebp-438h]
    tGameObjectMain* v39; // [esp+D0h] [ebp-434h]
    tGameObjectMain* v40; // [esp+D4h] [ebp-430h]
    tGameObjectMain* v41; // [esp+DCh] [ebp-428h]
    tGameObjectMain* v42; // [esp+E4h] [ebp-420h]
    tGameObjectMain* v43; // [esp+ECh] [ebp-418h]
    float v44; // [esp+F0h] [ebp-414h]
    float v45; // [esp+FCh] [ebp-408h]
    tGameObjectMain* v46; // [esp+110h] [ebp-3F4h]
    tGameObjectMain* v47; // [esp+118h] [ebp-3ECh]
    tGameObjectMain* v48; // [esp+128h] [ebp-3DCh]
    tGameObjectMain* v49; // [esp+130h] [ebp-3D4h]
    tGameObjectMain* v50; // [esp+134h] [ebp-3D0h]
    tGameObjectMain* v51; // [esp+138h] [ebp-3CCh]
    tGameObjectMain* v52; // [esp+13Ch] [ebp-3C8h]
    tGameObjectMain* v53; // [esp+140h] [ebp-3C4h]
    tGameObjectMain* v54; // [esp+144h] [ebp-3C0h]
    tGameObjectMain* v55; // [esp+148h] [ebp-3BCh]
    float v56; // [esp+14Ch] [ebp-3B8h]
    tGameObjectMain* v57; // [esp+160h] [ebp-3A4h]
    tGameObjectMain* v58; // [esp+174h] [ebp-390h]
    tGameObjectMain* v59; // [esp+17Ch] [ebp-388h]
    tGameObjectMain* v60; // [esp+184h] [ebp-380h]
    tGameObjectMain* v61; // [esp+188h] [ebp-37Ch]
    tGameObjectMain* v62; // [esp+18Ch] [ebp-378h]
    tGameObjectMain* v63; // [esp+190h] [ebp-374h]
    tGameObjectMain* v64; // [esp+194h] [ebp-370h]
    tGameObjectMain* v65; // [esp+19Ch] [ebp-368h]
    tGameObjectMain* v66; // [esp+1A0h] [ebp-364h]
    tGameObjectMain* v67; // [esp+1A4h] [ebp-360h]
    Vec3 v68; // [esp+1A8h] [ebp-35Ch] BYREF
    Vec3 v69; // [esp+1B4h] [ebp-350h] BYREF
    Vec3 v70; // [esp+1C0h] [ebp-344h] BYREF
    Vec3 v71; // [esp+1CCh] [ebp-338h] BYREF
    Vec3 v72; // [esp+1D8h] [ebp-32Ch] BYREF
    float v73; // [esp+1E4h] [ebp-320h]
    int i; // [esp+1E8h] [ebp-31Ch]
    float X; // [esp+1ECh] [ebp-318h]
    float v76; // [esp+1F0h] [ebp-314h]
    float v77; // [esp+1F4h] [ebp-310h]
    float v78; // [esp+1F8h] [ebp-30Ch]
    float v79; // [esp+1FCh] [ebp-308h]
    float v80; // [esp+200h] [ebp-304h]
    Vec3 v81; // [esp+204h] [ebp-300h] BYREF
    Vec3 v82; // [esp+210h] [ebp-2F4h] BYREF
    Vec3 v83; // [esp+21Ch] [ebp-2E8h]
    float v84; // [esp+228h] [ebp-2DCh]
    int v85; // [esp+22Ch] [ebp-2D8h]
    tGameObjectMain* v86; // [esp+230h] [ebp-2D4h]
    unsigned int v87; // [esp+234h] [ebp-2D0h]
    tGameObjectMain* v88; // [esp+238h] [ebp-2CCh]
    Vec3 v89; // [esp+23Ch] [ebp-2C8h] BYREF
    Vec3 v90; // [esp+248h] [ebp-2BCh] BYREF
    float v91; // [esp+254h] [ebp-2B0h]
    float v92; // [esp+258h] [ebp-2ACh]
    Vec3 v93; // [esp+260h] [ebp-2A4h] BYREF
    float v94; // [esp+26Ch] [ebp-298h]
    float v95; // [esp+270h] [ebp-294h]
    int v96; // [esp+274h] [ebp-290h]
    Vec3 v97; // [esp+278h] [ebp-28Ch] BYREF
    Vec3 v98; // [esp+284h] [ebp-280h] BYREF
    Vec3 v99; // [esp+290h] [ebp-274h] BYREF
    BOOL v100; // [esp+29Ch] [ebp-268h]
    float v101; // [esp+2A0h] [ebp-264h]
    BOOL v102; // [esp+2A4h] [ebp-260h]
    float v103; // [esp+2A8h] [ebp-25Ch]
    Vec3 v104; // [esp+2ACh] [ebp-258h] BYREF
    Vec3 v105; // [esp+2B8h] [ebp-24Ch] BYREF
    int v106[4]; // [esp+2C4h] [ebp-240h] BYREF
    Vec3 v107; // [esp+2D4h] [ebp-230h] BYREF
    Vec3 v108; // [esp+2E0h] [ebp-224h] BYREF
    Vec3 v109; // [esp+2F0h] [ebp-214h] BYREF
    Vec3 v110; // [esp+2FCh] [ebp-208h] BYREF
    Vec3 v111; // [esp+308h] [ebp-1FCh] BYREF
    Vec3 v112; // [esp+314h] [ebp-1F0h] BYREF
    float v113; // [esp+320h] [ebp-1E4h]
    float v114; // [esp+324h] [ebp-1E0h]
    BOOL v115; // [esp+32Ch] [ebp-1D8h]
    float v116; // [esp+330h] [ebp-1D4h]
    Vec3 v117; // [esp+334h] [ebp-1D0h] BYREF
    Vec3 v118; // [esp+340h] [ebp-1C4h] BYREF
    float v119; // [esp+34Ch] [ebp-1B8h]
    tGameObjectMain* v120; // [esp+354h] [ebp-1B0h]
    unsigned int v121; // [esp+35Ch] [ebp-1A8h]
    Vec3 v122; // [esp+364h] [ebp-1A0h] BYREF
    Vec3 v123; // [esp+370h] [ebp-194h] BYREF
    Vec3 v124; // [esp+37Ch] [ebp-188h] BYREF
    Vec3 v125; // [esp+388h] [ebp-17Ch] BYREF
    Vec3 v126; // [esp+394h] [ebp-170h] BYREF
    Vec3 v127; // [esp+3ACh] [ebp-158h] BYREF
    BOOL v128; // [esp+3B8h] [ebp-14Ch]
    float v129; // [esp+3BCh] [ebp-148h]
    Vec3 v130; // [esp+3C0h] [ebp-144h] BYREF
    Vec3 v131; // [esp+3CCh] [ebp-138h] BYREF
    Vec3 dest; // [esp+3D8h] [ebp-12Ch] BYREF
    Vec3 v133; // [esp+3E4h] [ebp-120h]
    float v134; // [esp+3F0h] [ebp-114h]
    unsigned int v135; // [esp+3F4h] [ebp-110h]
    int v136; // [esp+3F8h] [ebp-10Ch]
    float v137; // [esp+3FCh] [ebp-108h]
    Vec3 v138; // [esp+400h] [ebp-104h] BYREF
    Vec3 v139; // [esp+40Ch] [ebp-F8h] BYREF
    Vec3 v140; // [esp+418h] [ebp-ECh] BYREF
    Vec3 v141; // [esp+424h] [ebp-E0h] BYREF
    Vec3 v142; // [esp+430h] [ebp-D4h] BYREF
    float v143; // [esp+43Ch] [ebp-C8h]
    unsigned int v144; // [esp+440h] [ebp-C4h]
    float v145; // [esp+444h] [ebp-C0h]
    unsigned int v146; // [esp+448h] [ebp-BCh]
    Vec3 v147; // [esp+44Ch] [ebp-B8h]
    Vec3 v148; // [esp+458h] [ebp-ACh] BYREF
    Vec3 a4; // [esp+464h] [ebp-A0h] BYREF
    Vec3 v150; // [esp+470h] [ebp-94h] BYREF
    int v151; // [esp+47Ch] [ebp-88h]
    float v152; // [esp+484h] [ebp-80h]
    tScriptStruct_PNJTorpille* v153; // [esp+488h] [ebp-7Ch]
    int v154; // [esp+490h] [ebp-74h]
    int v155; // [esp+494h] [ebp-70h]
    Vec3 v156; // [esp+498h] [ebp-6Ch]
    Vec3 v157; // [esp+4A4h] [ebp-60h] BYREF
    float v158; // [esp+4B0h] [ebp-54h]
    float v159; // [esp+4B4h] [ebp-50h]
    int v160; // [esp+4B8h] [ebp-4Ch]
    float v161; // [esp+4BCh] [ebp-48h]
    float v162; // [esp+4C0h] [ebp-44h]
    Vec3 v164; // [esp+4C8h] [ebp-3Ch] BYREF
    Vec3 v165; // [esp+4E0h] [ebp-24h] BYREF
    Vec3 a3; // [esp+4ECh] [ebp-18h] BYREF
    float v167; // [esp+4FCh] [ebp-8h]
    BOOL v168; // [esp+500h] [ebp-4h]

    v93.x = 1.0;
    v93.y = 0.0;
    v93.z = 0.0;
    v139.x = 1.0;
    v139.y = 0.0;
    v139.z = 0.0;
    v140.x = 1.0;
    v140.y = 0.0;
    v140.z = 0.0;
    v118.x = 1.0;
    v118.y = 0.0;
    v118.z = 0.0;
    v105.x = 1.0;
    v105.y = 0.0;
    v105.z = 0.0;
    v127.x = 1.0;
    v127.y = 0.0;
    v127.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v18 = (tScriptStruct_PNJTorpille*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v18 = 0;
    v153 = v18;
    i = 0;
    X = 0.0;
    v92 = 0.0;
    v134 = 0.0;
    v79 = 0.0;
    v115 = 0;
    v154 = 0;
    v137 = 0.0;
    v155 = 0;
    v162 = 0.0;
    v88 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_torpille_tor_mode_mer_torpille_tor_mode_air = (int)AI_ul_RealResolveOneRefFunction(0x72000722u);
        fct_torpille_tor_mode_mer_torpille_tor_mode_mer = (int)AI_ul_RealResolveOneRefFunction(0x72000680u);
        fct_torpille_tor_mode_mer_torpille_tor_mode_explosion = (int)AI_ul_RealResolveOneRefFunction(0x7200069Bu);
        fctvar_torpille_tor_mode_mer_aircraft = AI_ul_RealResolveOneRef(42, 0x7100BBCCu);
        fct_torpille_tor_mode_mer_aircraft_torpille_tor_exec_trace = (int)AI_ul_RealResolveOneRefFunction(0x7200067Eu);
        torpille_tor_mode_mer_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        fct_torpille_tor_mode_mer_torpille_tor_exec_test_collision = (int)AI_ul_RealResolveOneRefFunction(0x72001C27u);
    }
    v88 = *AI_MainActor;
    if (v153->i_current_mode && v153->i_current_mode != 2)
    {
        v153->i_current_mode = 0;
        v153->f_time_change_mode = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    if (v153->i_flag_real_water)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v131);
        AI_EvalFunc_WTR_GetDif_C(&v131, &v90);
        v153->f_water_Z = v90.z;
        v153->f_last_valid_water_Z = v153->f_water_Z;
    }
    v155 = 0;
    v67 = (tGameObjectMain*)v153->des_o_target;
    if (v67)
    {
        if ((unsigned int)v67 <= 2)
            v67 = AI_MainActor[(uint32_t)v67 - 1];
        v66 = v67;
    }
    else
    {
        v66 = 0;
    }
    if (!v66)
        goto LABEL_262;
    v65 = (tGameObjectMain*)v153->des_o_target;
    if (v65)
    {
        if ((unsigned int)v65 <= 2)
            v65 = AI_MainActor[(uint32_t)v65 - 1];
        v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(v65);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
    }
    v87 = v0;
    if ((v0 & 0x600000) != 0)
    {
        v120 = *AI_MainActor;
        v64 = (tGameObjectMain*)v153->des_o_target;
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
        v62 = v120;
        if (v120)
        {
            if ((unsigned int)v120 <= 2)
                v62 = AI_MainActor[(uint32_t)v120 - 1];
            v61 = v62;
        }
        else
        {
            v61 = 0;
        }
        if (v63 != v61)
            v155 = 1;
    }
    else
    {
    LABEL_262:
        if (v153->des_f_mine_time != 0.0)
        {
            if (AI_EvalFunc_TIMEElapsed_C(v153->f_time_start, v153->des_f_mine_time))
                v155 = 1;
        }
    }
    if (v155)
    {
        if (v153->i_flag_mode_air)
        {
            v153->i_flag_mode_air = 0;
            v60 = (tGameObjectMain*)v153->o_me;
            if (v60)
            {
                if ((unsigned int)v60 <= 2)
                    v60 = AI_MainActor[(uint32_t)v60 - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v60,
                    2,
                    (tAIFunction*)fct_torpille_tor_mode_mer_torpille_tor_mode_air);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_torpille_tor_mode_mer_torpille_tor_mode_air);
            }
        }
        if (v153->i_flag_mode_mer)
        {
            v153->i_flag_mode_mer = 0;
            v59 = (tGameObjectMain*)v153->o_me;
            if (v59)
            {
                if ((unsigned int)v59 <= 2)
                    v59 = AI_MainActor[(uint32_t)v59 - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v59,
                    2,
                    (tAIFunction*)fct_torpille_tor_mode_mer_torpille_tor_mode_mer);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_torpille_tor_mode_mer_torpille_tor_mode_mer);
            }
        }
        if (!v153->i_flag_mode_explosion)
        {
            v153->i_flag_mode_explosion = 1;
            v58 = (tGameObjectMain*)v153->o_me;
            if (v58)
            {
                if ((unsigned int)v58 <= 2)
                    v58 = AI_MainActor[(uint32_t)v58 - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v58,
                    2,
                    (tAIFunction*)fct_torpille_tor_mode_mer_torpille_tor_mode_explosion);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_torpille_tor_mode_mer_torpille_tor_mode_explosion);
            }
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        if (v153->des_mode == 2)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v98);
            MATH_SubstractVec(&dest, &v98, &v153->v_last_pos);
            v161 = TIM_gf_dt;
            MATH_VecMultNormScalar((Vec3*)v106, &dest, TIM_gf_dt);
            v105 = *(Vec3*)v106;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v153->v_last_pos);
            v96 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v105);
        }
        else if (v153->des_mode == 1)
        {
            v57 = (tGameObjectMain*)v153->des_o_target;
            if (v57)
            {
                if ((unsigned int)v57 <= 2)
                    v57 = AI_MainActor[(uint32_t)v57 - 1];
                AI_EvalFunc_OBJGetPos_C(v57, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v123);
            MATH_SubstractVec(&v104, &a3, &v123);
            v127 = v104;
            X = MATH_VecLen(&v127);
            v56 = 1.0f / X;
            v127.x = v127.x * v56;
            v127.y = v127.y * v56;
            v127.z = v127.z * v56;
            v55 = (tGameObjectMain*)v153->des_o_target;
            if (v55)
            {
                if ((unsigned int)v55 <= 2)
                    v55 = AI_MainActor[(uint32_t)v55 - 1];
                v54 = v55;
            }
            else
            {
                v54 = 0;
            }
            v53 = v88;
            if (v88)
            {
                if ((unsigned int)v88 <= 2)
                    v53 = AI_MainActor[(uint32_t)v88 - 1];
                v52 = v53;
            }
            else
            {
                v52 = 0;
            }
            if (v54 == v52 && v153->des_f_target_lock_dist > (double)X)
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v81);
                v116 = v127.z * v81.z + v127.y * v81.y + v81.x * v127.x;
                if (v153->des_angle_lock < (double)v116)
                {
                    for (i = 0; i < 50; ++i)
                    {
                        v51 = (tGameObjectMain*)v153->des_o_target_lock[i];
                        if (v51)
                        {
                            if ((unsigned int)v51 <= 2)
                                v51 = AI_MainActor[(uint32_t)v51 - 1];
                            v50 = v51;
                        }
                        else
                        {
                            v50 = 0;
                        }
                        if (!v50)
                            break;
                        v49 = (tGameObjectMain*)v153->des_o_target_lock[i];
                        if (v49)
                        {
                            if ((unsigned int)v49 <= 2)
                                v49 = AI_MainActor[(uint32_t)v49 - 1];
                            v2 = AI_EvalFunc_OBJ_FlagsControlGet_C(v49);
                        }
                        else
                        {
                            v2 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
                        }
                        v146 = v2;
                        if ((v2 & 0x400000) == 0)
                        {
                            v48 = (tGameObjectMain*)v153->des_o_target_lock[i];
                            if (v48)
                            {
                                if ((unsigned int)v48 <= 2)
                                    v48 = AI_MainActor[(uint32_t)v48 - 1];
                                AI_EvalFunc_OBJGetPos_C(v48, &v126);
                            }
                            else
                            {
                                AI_EvalFunc_OBJGetPos_C(0, &v126);
                            }
                            AI_EvalFunc_OBJGetPos_C_CURRENT(&v164);
                            MATH_SubstractVec(&v130, &v126, &v164);
                            v127 = v130;
                            v47 = (tGameObjectMain*)v153->des_o_target;
                            if (v47)
                            {
                                if ((unsigned int)v47 <= 2)
                                    v47 = AI_MainActor[(uint32_t)v47 - 1];
                                AI_EvalFunc_OBJGetPos_C(v47, &v108);
                            }
                            else
                            {
                                AI_EvalFunc_OBJGetPos_C(0, &v108);
                            }
                            v46 = (tGameObjectMain*)v153->des_o_target_lock[i];
                            if (v46)
                            {
                                if ((unsigned int)v46 <= 2)
                                    v46 = AI_MainActor[(uint32_t)v46 - 1];
                                AI_EvalFunc_OBJGetPos_C(v46, &v107);
                            }
                            else
                            {
                                AI_EvalFunc_OBJGetPos_C(0, &v107);
                            }
                            MATH_SubstractVec(&v141, &v108, &v107);
                            v139 = v141;
                            X = MATH_VecLen(&v139);
                            v45 = 1.0f / X;
                            v139.x = v139.x * v45;
                            v139.y = v139.y * v45;
                            v139.z = v139.z * v45;
                            v156.x = -v127.x;
                            v156.y = -v127.y;
                            v156.z = -v127.z;
                            v162 = v139.z * -v127.z
                                + v139.y * -v127.y
                                + -v127.x * v139.x;
                            if (v162 < 0.0 || v162 >(double)X)
                            {
                                X = v127.z * v127.z + v127.y * v127.y + v127.x * v127.x;
                                if (v153->f_target_lock_sqr_dist > (double)X)
                                {
                                    X = sqrtf(X);
                                    v44 = 1.0f / X;
                                    v127.x = v127.x * v44;
                                    v127.y = v127.y * v44;
                                    v127.z = v127.z * v44;
                                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v125);
                                    v159 = v127.z * v125.z + v127.y * v125.y + v125.x * v127.x;
                                    if (v153->des_angle_lock < (double)v159)
                                    {
                                        v153->o_last_target = v153->des_o_target;
                                        v153->des_o_target = v153->des_o_target_lock[i];
                                        goto LABEL_115;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (!v153->des_lock_target_mode)
                goto LABEL_128;
        LABEL_115:
            v43 = (tGameObjectMain*)v153->des_o_target;
            if (v43)
            {
                if ((unsigned int)v43 <= 2)
                    v43 = AI_MainActor[(uint32_t)v43 - 1];
                v3 = AI_EvalFunc_AIIsModel_C(v43, (tAIModel*)fctvar_torpille_tor_mode_mer_aircraft);
            }
            else
            {
                v3 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_torpille_tor_mode_mer_aircraft);
            }
            v102 = v3;
            if (v3)
            {
                v42 = (tGameObjectMain*)v153->des_o_target;
                if (v42)
                {
                    if ((unsigned int)v42 <= 2)
                        v42 = AI_MainActor[(uint32_t)v42 - 1];
                    v4 = AI_EvalFunc_OBJ_CapaTest_C(v42, 0x2000);
                }
                else
                {
                    v4 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x2000);
                }
                v168 = v4;
                v115 = v4;
            }
            else
            {
            LABEL_128:
                v155 = 0;
                v41 = (tGameObjectMain*)v153->des_o_target;
                if (v41)
                {
                    if ((unsigned int)v41 <= 2)
                        v41 = AI_MainActor[(uint32_t)v41 - 1];
                    v5 = AI_EvalFunc_OBJ_FlagsControlGet_C(v41);
                }
                else
                {
                    v5 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
                }
                v144 = v5;
                if ((v5 & 0x600000) != 0)
                {
                    v86 = *AI_MainActor;
                    v40 = (tGameObjectMain*)v153->des_o_target;
                    if (v40)
                    {
                        if ((unsigned int)v40 <= 2)
                            v40 = AI_MainActor[(uint32_t)v40 - 1];
                        v39 = v40;
                    }
                    else
                    {
                        v39 = 0;
                    }
                    v38 = v86;
                    if (v86)
                    {
                        if ((unsigned int)v86 <= 2)
                            v38 = AI_MainActor[(uint32_t)v86 - 1];
                        v37 = v38;
                    }
                    else
                    {
                        v37 = 0;
                    }
                    if (v39 != v37)
                        v155 = 1;
                }
                else
                {
                    v36 = (tGameObjectMain*)v153->des_o_target;
                    if (v36)
                    {
                        if ((unsigned int)v36 <= 2)
                            v36 = AI_MainActor[(uint32_t)v36 - 1];
                        v6 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v36, 0x4000);
                    }
                    else
                    {
                        v6 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x4000);
                    }
                    v100 = v6;
                    if (v6)
                    {
                        v35 = (tGameObjectMain*)v153->des_o_target;
                        if (v35)
                        {
                            if ((unsigned int)v35 <= 2)
                                v35 = AI_MainActor[(uint32_t)v35 - 1];
                            v7 = AI_EvalFunc_OBJDrawMaskGet_C(v35);
                        }
                        else
                        {
                            v7 = AI_EvalFunc_OBJDrawMaskGet_C(0);
                        }
                        v151 = v7;
                        if ((v7 & 8) == 0)
                            v155 = 1;
                    }
                }
                if (v155)
                {
                    v34 = (tGameObjectMain*)v153->o_last_target;
                    if (v34)
                    {
                        if ((unsigned int)v34 <= 2)
                            v34 = AI_MainActor[(uint32_t)v34 - 1];
                        v33 = v34;
                    }
                    else
                    {
                        v33 = 0;
                    }
                    if (v33)
                    {
                        v153->des_o_target = v153->o_last_target;
                        v153->o_last_target = 0;
                    }
                    v115 = 0;
                }
                else
                {
                    v115 = 1;
                }
            }
            if (v115)
            {
                v32 = (tGameObjectMain*)v153->des_o_target;
                if (v32)
                {
                    if ((unsigned int)v32 <= 2)
                        v32 = AI_MainActor[(uint32_t)v32 - 1];
                    AI_EvalFunc_OBJGetPos_C(v32, &v157);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v157);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v122);
                MATH_SubstractVec(&v111, &v157, &v122);
                v127 = v111;
                AI_EvalFunc_MATHVecSetHorzNormalize_C(&v127);
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v142);
                v101 = v127.z * v142.z + v127.y * v142.y + v142.x * v127.x;
                if (v101 < 0.0)
                    v17 = 0.0;
                else
                    v17 = v101;
                v113 = v17;
                v31 = 1.0f - v17;
                if (v31 > 0.1)
                    v16 = 1.0f - v17;
                else
                    v16 = 0.1f;
                v92 = v16;
                v30 = (tGameObjectMain*)v153->o_last_target;
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
                    v79 = 60.0;
                else
                    v79 = 12.0;
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v150);
                v95 = v127.z * v150.z + v127.y * v150.y + v150.x * v127.x;
                if (v95 <= 0.0)
                {
                    v103 = TIM_gf_dt;
                    v27 = v79 * TIM_gf_dt;
                    if (v27 < 0.0)
                        v27 = 0.0;
                    if (v27 > 1.0)
                        v27 = 1.0;
                    v119 = -v153->des_f_rotation_speed * v92 * v27 + v153->f_sight_rotspeed * (1.0f - v27);
                    v153->f_sight_rotspeed = v119;
                }
                else
                {
                    v152 = TIM_gf_dt;
                    v28 = v79 * TIM_gf_dt;
                    if (v28 < 0.0)
                        v28 = 0.0;
                    if (v28 > 1.0)
                        v28 = 1.0;
                    v167 = v153->des_f_rotation_speed * v92 * v28 + v153->f_sight_rotspeed * (1.0f - v28);
                    v153->f_sight_rotspeed = v167;
                }
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v97);
                v112.x = 0.0;
                v112.y = 0.0;
                v112.z = 1.0;
                v76 = TIM_gf_dt;
                v11 = v153->f_sight_rotspeed * TIM_gf_dt;
                AI_EvalFunc_MATHVecRotate_C(&v97, &v112, v11, &a4);
                v118 = a4;
                v71.x = 0.0;
                v71.y = 0.0;
                v71.z = 1.0;
                AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v118, &v71);
            }
            if (!v153->des_seb_mode)
                goto LABEL_232;
            v26 = (tGameObjectMain*)v153->des_o_target;
            if (v26)
            {
                if ((unsigned int)v26 <= 2)
                    v26 = AI_MainActor[(uint32_t)v26 - 1];
                v8 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v26, 4);
            }
            else
            {
                v8 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 4);
            }
            v128 = v8;
            if (v8)
            {
                v25 = (tGameObjectMain*)v153->des_o_target;
                if (v25)
                {
                    if ((unsigned int)v25 <= 2)
                        v25 = AI_MainActor[(uint32_t)v25 - 1];
                    v9 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v25);
                }
                else
                {
                    v9 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
                }
                v94 = (float)v9;
                if (v94 <= 400.0)
                {
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v70);
                    v22 = (tGameObjectMain*)v153->des_o_target;
                    if (v22)
                    {
                        if ((unsigned int)v22 <= 2)
                            v22 = AI_MainActor[(uint32_t)v22 - 1];
                        AI_EvalFunc_DYNSpeedGetVector_C(v22, &v110);
                    }
                    else
                    {
                        AI_EvalFunc_DYNSpeedGetVector_C(0, &v110);
                    }
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v69);
                    v145 = v69.z * v110.z + v69.y * v110.y + v110.x * v69.x;
                    if (v145 < 60.0)
                        v14 = 60.0;
                    else
                        v14 = v145;
                    v114 = v14;
                    v133.x = v70.x * v14;
                    v133.y = v70.y * v14;
                    v133.z = v70.z * v14;
                    v140 = v133;
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v124);
                    v24 = (tGameObjectMain*)v153->des_o_target;
                    if (v24)
                    {
                        if ((unsigned int)v24 <= 2)
                            v24 = AI_MainActor[(uint32_t)v24 - 1];
                        AI_EvalFunc_DYNSpeedGetVector_C(v24, &v68);
                    }
                    else
                    {
                        AI_EvalFunc_DYNSpeedGetVector_C(0, &v68);
                    }
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v99);
                    v73 = v99.z * v68.z + v99.y * v68.y + v68.x * v99.x;
                    v23 = v73 + 10.0f;
                    if (v23 < (double)v153->des_f_speed)
                        v15 = v153->des_f_speed;
                    else
                        v15 = v73 + 10.0f;
                    v158 = v15;
                    v83.x = v124.x * v15;
                    v83.y = v124.y * v15;
                    v83.z = v124.z * v15;
                    v140 = v83;
                }
            }
            else
            {
            LABEL_232:
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v138);
                v21 = v153->des_f_speed;
                v147.x = v138.x * v21;
                v147.y = v138.y * v21;
                v147.z = v138.z * v21;
                v140 = v147;
            }
            v20 = v153->des_v_water_friction.y;
            v140.x = v140.x * v20;
            v140.y = v140.y * v20;
            v140.z = v140.z * v20;
            MATH_VecAddVec(&v82, &v140, &v153->v_water_force);
            AI_EvalFunc_DYNTractionSet_C_CURRENT(&v82);
            v136 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_torpille_tor_mode_mer_aircraft_torpille_tor_exec_trace);
            if (v136 != 1000)
                return v136;
        }
        else if (!v153->des_mode && v153->f_water_Z != -1.0e14)
        {
            if (v153->des_mine_mode)
            {
                v121 = AI_EvalFunc_OBJ_FlagsControlGet_C(AI_gpst_CurrentGameObject);
                if ((v121 & 0x20000) != 0)
                {
                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                    return 1000;
                }
                if (v153->i_flag_real_water)
                {
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v148);
                    v93 = v148;
                    v93.z = v153->f_water_Z;
                    AI_EvalFunc_OBJSetPos_C_CURRENT(&v93);
                }
                else
                {
                    v129 = TIM_gf_dt;
                    v153->f_angle = v153->f_oscillation_speed * TIM_gf_dt + v153->f_angle;
                    if (v153->f_angle > 6.283185)
                        v153->f_angle = v153->f_angle - 6.283185f;
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v89);
                    v93 = v89;
                    v78 = sinf(v153->f_angle);
                    v134 = v153->f_water_Z - 1.0f + v78 * 0.5f;
                    v137 = v93.z - v134;
                    v93.z = v134;
                    AI_EvalFunc_OBJSetPos_C_CURRENT(&v93);
                }
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v72);
                v153->v_water_force.x = 0.0;
                v153->v_water_force.y = 0.0;
                v153->v_water_force.z = (v153->f_water_Z - v72.z) * v153->des_f_archimede;
                v85 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v153->des_v_water_friction);
                AI_EvalFunc_DYNTractionSet_C_CURRENT(&v153->v_water_force);
            }
            v135 = AI_EvalFunc_OBJ_FlagsControlGet_C(AI_gpst_CurrentGameObject);
            if ((v135 & 0x20000) == 0)
            {
                v143 = TIM_gf_dt;
                v12 = v153->f_rotation_speed * TIM_gf_dt;
                AI_EvalFunc_OBJRotateGlobalZ_C(AI_gpst_CurrentGameObject, v12);
                if (!v153->i_flag_real_water)
                {
                    v84 = AI_EvalFunc_MTHFloatSign_C(v137);
                    v91 = AI_EvalFunc_MTHFloatSign_C(v153->f_ripple_sign);
                    if (v84 != v91)
                    {
                        v10 = AI_EvalFunc_MTHFloatSign_C(v137);
                        v153->f_ripple_sign = (float)v10;
                        if (v153->des_i_ripple)
                        {
                            v154 = AI_EvalFunc_GFXAdd_C_CURRENT(20);
                            v19 = (tGameObjectMain*)torpille_tor_mode_mer_SFX_light_smoke_gao;
                            if (torpille_tor_mode_mer_SFX_light_smoke_gao)
                            {
                                if ((unsigned int)torpille_tor_mode_mer_SFX_light_smoke_gao <= 2)
                                    v19 = AI_MainActor[torpille_tor_mode_mer_SFX_light_smoke_gao - 1];
                                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v154, v19, 16);
                            }
                            else
                            {
                                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v154, 0, 16);
                            }
                            v77 = AI_EvalFunc_MATHRandFloat_C(4.0, 6.0);
                            GFXSetCurGlobal(v154);
                            AI_EvalFunc_GFXSetf_C_CURGLOB(20000, v77);
                            v80 = AI_EvalFunc_MATHRandFloat_C(0.0, 2.0);
                            v13 = v80 + 3.0f;
                            AI_EvalFunc_GFXSetf_C_CURGLOB(20001, v13);
                            AI_EvalFunc_GFXSetf_C_CURGLOB(20002, 0.0);
                            AI_EvalFunc_GFXSetf_C_CURGLOB(20003, 0.0);
                            AI_EvalFunc_GFXSetf_C_CURGLOB(20004, 2.0);
                            AI_EvalFunc_GFXSeti_C_CURGLOB(20100, 1);
                            AI_EvalFunc_GFXSeti_C_CURGLOB(20100, -2);
                            AI_EvalFunc_GFXSeti_C_CURGLOB(20101, 0x777777);
                            AI_EvalFunc_GFXSeti_C_CURGLOB(20102, 0);
                            AI_EvalFunc_OBJGetPos_C_CURRENT(&v117);
                            v139 = v117;
                            v139.z = v153->f_water_Z + 0.1f;
                            AI_EvalFunc_GFXSetv_C_CURGLOB(20200, &v139);
                            v109.x = 0.0;
                            v109.y = 0.0;
                            v109.z = 1.0;
                            AI_EvalFunc_GFXSetv_C_CURGLOB(20201, &v109);
                            v165.x = 0.0;
                            v165.y = -1.0;
                            v165.z = 0.0;
                            AI_EvalFunc_GFXSetv_C_CURGLOB(20202, &v165);
                        }
                    }
                }
            }
        }
        v160 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_torpille_tor_mode_mer_torpille_tor_exec_test_collision);
        if (v160 == 1000)
            result = 1000;
        else
            result = v160;
    }
    return result;
}

int torpille_tor_exec_visuel()
{
    int result; // eax
    int v1; // eax
    double v2; // st7
    int v3; // eax
    float X; // [esp+4h] [ebp-268h]
    float Xa; // [esp+4h] [ebp-268h]
    float Xb; // [esp+4h] [ebp-268h]
    float Xc; // [esp+4h] [ebp-268h]
    float Xd; // [esp+4h] [ebp-268h]
    float Xe; // [esp+4h] [ebp-268h]
    float Xf; // [esp+4h] [ebp-268h]
    float Xg; // [esp+4h] [ebp-268h]
    float Xh; // [esp+4h] [ebp-268h]
    float v13; // [esp+Ch] [ebp-260h]
    float v14; // [esp+10h] [ebp-25Ch]
    float v15; // [esp+14h] [ebp-258h]
    float v16; // [esp+18h] [ebp-254h]
    tScriptStruct_PNJTorpille* v17; // [esp+1Ch] [ebp-250h]
    float v18; // [esp+20h] [ebp-24Ch]
    tGameObjectMain* v19; // [esp+28h] [ebp-244h]
    float v20; // [esp+2Ch] [ebp-240h]
    tGameObjectMain* v21; // [esp+3Ch] [ebp-230h]
    float v22; // [esp+40h] [ebp-22Ch]
    tGameObjectMain* v23; // [esp+48h] [ebp-224h]
    tGameObjectMain* v24; // [esp+50h] [ebp-21Ch]
    tGameObjectMain* v25; // [esp+58h] [ebp-214h]
    float v26; // [esp+5Ch] [ebp-210h]
    tGameObjectMain* v27; // [esp+64h] [ebp-208h]
    float v28; // [esp+68h] [ebp-204h]
    tGameObjectMain* v29; // [esp+70h] [ebp-1FCh]
    tGameObjectMain* v30; // [esp+80h] [ebp-1ECh]
    tGameObjectMain* v31; // [esp+88h] [ebp-1E4h]
    tGameObjectMain* v32; // [esp+90h] [ebp-1DCh]
    tGameObjectMain* v33; // [esp+98h] [ebp-1D4h]
    tGameObjectMain* v34; // [esp+A0h] [ebp-1CCh]
    tGameObjectMain* v35; // [esp+A8h] [ebp-1C4h]
    tGameObjectMain* v36; // [esp+B0h] [ebp-1BCh]
    tGameObjectMain* v37; // [esp+B4h] [ebp-1B8h]
    tGameObjectMain* v38; // [esp+B8h] [ebp-1B4h]
    float v39; // [esp+BCh] [ebp-1B0h]
    tGameObjectMain* v40; // [esp+C4h] [ebp-1A8h]
    float v41; // [esp+C8h] [ebp-1A4h]
    float v42; // [esp+C8h] [ebp-1A4h]
    float v43; // [esp+C8h] [ebp-1A4h]
    float v44; // [esp+C8h] [ebp-1A4h]
    Vec3 a3; // [esp+CCh] [ebp-1A0h] BYREF
    float v46; // [esp+D8h] [ebp-194h]
    Vec3 v47; // [esp+E0h] [ebp-18Ch] BYREF
    Vec3 v48; // [esp+ECh] [ebp-180h] BYREF
    Vec3 v49; // [esp+FCh] [ebp-170h] BYREF
    Vec3 v50; // [esp+108h] [ebp-164h] BYREF
    int v51; // [esp+114h] [ebp-158h]
    Vec3 v52; // [esp+120h] [ebp-14Ch] BYREF
    float v53; // [esp+12Ch] [ebp-140h]
    unsigned int v54; // [esp+130h] [ebp-13Ch]
    Vec3 v55; // [esp+134h] [ebp-138h] BYREF
    float v56; // [esp+140h] [ebp-12Ch]
    Vec3 v57; // [esp+144h] [ebp-128h] BYREF
    Vec3 v58; // [esp+150h] [ebp-11Ch] BYREF
    float v59; // [esp+160h] [ebp-10Ch]
    Vec3 v60; // [esp+164h] [ebp-108h] BYREF
    float v61; // [esp+170h] [ebp-FCh]
    float v62; // [esp+174h] [ebp-F8h]
    float v63; // [esp+178h] [ebp-F4h]
    Vec3 v64; // [esp+180h] [ebp-ECh] BYREF
    Vec3 v65; // [esp+18Ch] [ebp-E0h] BYREF
    float v66; // [esp+198h] [ebp-D4h]
    float v67; // [esp+19Ch] [ebp-D0h]
    float v68; // [esp+1A0h] [ebp-CCh]
    BOOL v69; // [esp+1ACh] [ebp-C0h]
    Vec3 v70; // [esp+1B0h] [ebp-BCh] BYREF
    Vec3 v71; // [esp+1BCh] [ebp-B0h] BYREF
    float v72; // [esp+1C8h] [ebp-A4h]
    Vec3 v73; // [esp+1CCh] [ebp-A0h] BYREF
    Vec3 v74; // [esp+1D8h] [ebp-94h] BYREF
    Vec3 v75; // [esp+1E4h] [ebp-88h] BYREF
    Vec3 v76; // [esp+1F0h] [ebp-7Ch] BYREF
    Vec3 v77; // [esp+1FCh] [ebp-70h] BYREF
    float v78; // [esp+208h] [ebp-64h]
    Vec3 v80; // [esp+210h] [ebp-5Ch] BYREF
    tScriptStruct_PNJTorpille* v81; // [esp+21Ch] [ebp-50h]
    Vec3 v82; // [esp+220h] [ebp-4Ch] BYREF
    Vec3 v83; // [esp+22Ch] [ebp-40h] BYREF
    Vec3 v84; // [esp+238h] [ebp-34h] BYREF
    float v85; // [esp+244h] [ebp-28h]
    Vec3 v86; // [esp+24Ch] [ebp-20h] BYREF
    Vec3 v87; // [esp+258h] [ebp-14h] BYREF
    float v88; // [esp+264h] [ebp-8h]

    v48.x = 1.0;
    v48.y = 0.0;
    v48.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v17 = (tScriptStruct_PNJTorpille*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v17 = 0;
    v81 = v17;
    v41 = 0.0;
    v69 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        torpille_tor_exec_visuel_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    v54 = AI_EvalFunc_OBJ_FlagsControlGet_C(AI_gpst_CurrentGameObject);
    v69 = (v54 & 0x220000) != 0;
    if (v81->des_mine_infinie)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v81->f_time_send_paf, 0.25))
        {
            AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, 1.0);
        }
        else
        {
            v72 = TIM_gf_MainClock - TIM_gf_LockTime;
            v42 = (v72 - v81->f_time_send_paf) / 0.25f;
            X = v42 * 3.141593f;
            v41 = sinf(X);
            Xa = v41 * 0.5f + 1.0f;
            AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, Xa);
        }
    }
    if (v81->des_visuel)
    {
        if (v81->des_visuel == 1)
        {
            if (v81->des_mode == 1)
            {
                if (v69)
                {
                    if (v81->GFX_Halo != -1)
                    {
                        AI_EvalFunc_GFXDel_C_CURRENT(v81->GFX_Halo);
                        v81->GFX_Halo = -1;
                    }
                }
                else
                {
                    v88 = TIM_gf_dt;
                    v38 = (tGameObjectMain*)v81->o_visuel;
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
                    Xd = v81->des_f_visuel_rotation_speed * v88;
                    AI_EvalFunc_OBJRotateLocalZ_c(v37, Xd);
                    if (v81->GFX_Halo == -1)
                    {
                        v81->GFX_Halo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                        AI_EvalFunc_GFXFlagSet_C_CURRENT(v81->GFX_Halo, 4, 1);
                        v36 = (tGameObjectMain*)torpille_tor_exec_visuel_SFX_light_smoke_gao;
                        if (torpille_tor_exec_visuel_SFX_light_smoke_gao)
                        {
                            if ((unsigned int)torpille_tor_exec_visuel_SFX_light_smoke_gao <= 2)
                                v36 = AI_MainActor[torpille_tor_exec_visuel_SFX_light_smoke_gao - 1];
                            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v81->GFX_Halo, v36, 0);
                        }
                        else
                        {
                            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v81->GFX_Halo, 0, 0);
                        }
                        GFXSetCurGlobal(v81->GFX_Halo);
                        AI_EvalFunc_GFXSeti_C_CURGLOB(21104, 3);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(21006, 0.5);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
                        AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 0.0);
                        AI_EvalFunc_GFXSeti_C_CURGLOB(21100, 4);
                        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 0xFF0000FF);
                    }
                    if (v81->i_flag_activation)
                    {
                        v53 = TIM_gf_MainClock - TIM_gf_LockTime;
                        Xe = v53 * 16.0f;
                        v59 = sinf(Xe);
                        v44 = (v59 + 1.0f) * 0.5f;
                        AI_EvalFunc_GFXSeti_C_CURRENT(v81->GFX_Halo, 21101, 0xFF0000FF);
                    }
                    else
                    {
                        v44 = 0.0;
                        AI_EvalFunc_GFXSeti_C_CURRENT(v81->GFX_Halo, 21101, 0xFF2020D0);
                    }
                    GFXSetCurGlobal(v81->GFX_Halo);
                    Xf = 5.0f * v44 + 5.0f;
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21004, Xf);
                    Xg = 1.5f * v44 + 1.5f;
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21000, Xg);
                    v35 = (tGameObjectMain*)v81->o_visuel;
                    if (v35)
                    {
                        if ((unsigned int)v35 <= 2)
                            v35 = AI_MainActor[(uint32_t)v35 - 1];
                        AI_EvalFunc_OBJGetPos_C(v35, &a3);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &a3);
                    }
                    v34 = (tGameObjectMain*)v81->o_visuel;
                    if (v34)
                    {
                        if ((unsigned int)v34 <= 2)
                            v34 = AI_MainActor[(uint32_t)v34 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v34, &v58);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v58);
                    }
                    v76.x = v58.x + v58.x;
                    v76.y = v58.y + v58.y;
                    v76.z = v58.z + v58.z;
                    MATH_VecAddVec(&v64, &a3, &v76);
                    AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v64);
                    v33 = (tGameObjectMain*)v81->o_visuel;
                    if (v33)
                    {
                        if ((unsigned int)v33 <= 2)
                            v33 = AI_MainActor[(uint32_t)v33 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v33, &v86);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v86);
                    }
                    AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v86);
                }
            }
        }
        else if (v81->des_visuel == 2)
        {
            if (v81->f_main_actor_dist >= 2500.0)
            {
                if (v81->f_radar_activation_coef != 0.0)
                {
                    v24 = (tGameObjectMain*)v81->o_radar;
                    if (v24)
                    {
                        if ((unsigned int)v24 <= 2)
                            v24 = AI_MainActor[(uint32_t)v24 - 1];
                        OBJ_RestoreInitialPos(v24);
                    }
                    else
                    {
                        OBJ_RestoreInitialPos(0);
                    }
                    v23 = (tGameObjectMain*)v81->o_radar;
                    if (v23)
                    {
                        if ((unsigned int)v23 <= 2)
                            v23 = AI_MainActor[(uint32_t)v23 - 1];
                        AI_EvalFunc_OBJRotateLocalZ_c(v23, v81->f_radar_rot_angle);
                    }
                    else
                    {
                        AI_EvalFunc_OBJRotateLocalZ_c(0, v81->f_radar_rot_angle);
                    }
                    v46 = TIM_gf_dt;
                    v81->f_radar_activation_coef = v81->f_radar_activation_coef - 2.0f * TIM_gf_dt;
                    v22 = 1.0f - v81->f_main_actor_dist / 2500.0f;
                    if (v22 > 1.0)
                        v14 = 1.0;
                    else
                        v14 = 1.0f - v81->f_main_actor_dist / 2500.0f;
                    v41 = v81->f_radar_activation_coef * v14;
                    if (v81->f_radar_activation_coef >= 0.0)
                    {
                        v21 = (tGameObjectMain*)v81->o_radar;
                        if (v21)
                        {
                            if ((unsigned int)v21 <= 2)
                                v21 = AI_MainActor[(uint32_t)v21 - 1];
                            AI_EvalFunc_OBJGetPos_C(v21, &v77);
                        }
                        else
                        {
                            AI_EvalFunc_OBJGetPos_C(0, &v77);
                        }
                        v48 = v77;
                        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v84);
                        v66 = v84.x * 1.3f;
                        v67 = v84.y * 1.3f;
                        v68 = v84.z * 1.3f;
                        v20 = v81->f_radar_activation_coef;
                        v57.x = v66 * v20;
                        v57.y = v67 * v20;
                        v57.z = v68 * v20;
                        MATH_VecAddVec(&v74, &v48, &v57);
                        v19 = (tGameObjectMain*)v81->o_radar;
                        if (v19)
                        {
                            if ((unsigned int)v19 <= 2)
                                v19 = AI_MainActor[(uint32_t)v19 - 1];
                            AI_EvalFunc_OBJSetPos_C(v19, &v74);
                        }
                        else
                        {
                            AI_EvalFunc_OBJSetPos_C(0, &v74);
                        }
                    }
                    else
                    {
                        v81->f_radar_activation_coef = 0.0;
                    }
                }
            }
            else
            {
                v32 = (tGameObjectMain*)v81->o_radar;
                if (v32)
                {
                    if ((unsigned int)v32 <= 2)
                        v32 = AI_MainActor[(uint32_t)v32 - 1];
                    OBJ_RestoreInitialPos(v32);
                }
                else
                {
                    OBJ_RestoreInitialPos(0);
                }
                v31 = (tGameObjectMain*)v81->o_radar;
                if (v31)
                {
                    if ((unsigned int)v31 <= 2)
                        v31 = AI_MainActor[(uint32_t)v31 - 1];
                    AI_EvalFunc_OBJRotateLocalZ_c(v31, v81->f_radar_rot_angle);
                }
                else
                {
                    AI_EvalFunc_OBJRotateLocalZ_c(0, v81->f_radar_rot_angle);
                }
                v30 = (tGameObjectMain*)v81->o_radar;
                if (v30)
                {
                    if ((unsigned int)v30 <= 2)
                        v30 = AI_MainActor[(uint32_t)v30 - 1];
                    AI_EvalFunc_OBJGetPos_C(v30, &v70);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v70);
                }
                v48 = v70;
                if (v81->f_radar_activation_coef >= 1.0)
                {
                    v26 = 1.0f - v81->f_main_actor_dist / 2500.0f;
                    if (v26 > 1.0)
                        v15 = 1.0;
                    else
                        v15 = 1.0f - v81->f_main_actor_dist / 2500.0f;
                    v41 = v15;
                    v85 = TIM_gf_dt;
                    v81->f_radar_rot_angle = v81->f_radar_rot_angle - 20.0f * TIM_gf_dt * v15;
                    if (v81->f_radar_rot_angle < 0.0)
                        v81->f_radar_rot_angle = v81->f_radar_rot_angle + 6.283185f;
                    AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v50);
                    v87.x = v50.x * 1.3f;
                    v87.y = v50.y * 1.3f;
                    v87.z = v50.z * 1.3f;
                    MATH_VecAddVec(&v75, &v48, &v87);
                    v25 = (tGameObjectMain*)v81->o_radar;
                    if (v25)
                    {
                        if ((unsigned int)v25 <= 2)
                            v25 = AI_MainActor[(uint32_t)v25 - 1];
                        AI_EvalFunc_OBJSetPos_C(v25, &v75);
                    }
                    else
                    {
                        AI_EvalFunc_OBJSetPos_C(0, &v75);
                    }
                }
                else
                {
                    v41 = 0.0;
                    v56 = TIM_gf_dt;
                    v81->f_radar_activation_coef = 2.0f * TIM_gf_dt + v81->f_radar_activation_coef;
                    if (v81->f_radar_activation_coef <= 1.0)
                    {
                        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v83);
                        v61 = v83.x * 1.3f;
                        v62 = v83.y * 1.3f;
                        v63 = v83.z * 1.3f;
                        v28 = v81->f_radar_activation_coef;
                        v80.x = v61 * v28;
                        v80.y = v62 * v28;
                        v80.z = v63 * v28;
                        MATH_VecAddVec(&v60, &v48, &v80);
                        v27 = (tGameObjectMain*)v81->o_radar;
                        if (v27)
                        {
                            if ((unsigned int)v27 <= 2)
                                v27 = AI_MainActor[(uint32_t)v27 - 1];
                            AI_EvalFunc_OBJSetPos_C(v27, &v60);
                        }
                        else
                        {
                            AI_EvalFunc_OBJSetPos_C(0, &v60);
                        }
                    }
                    else
                    {
                        v81->f_radar_activation_coef = 1.0;
                        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v55);
                        v73.x = v55.x * 1.3f;
                        v73.y = v55.y * 1.3f;
                        v73.z = v55.z * 1.3f;
                        MATH_VecAddVec(&v47, &v48, &v73);
                        v29 = (tGameObjectMain*)v81->o_radar;
                        if (v29)
                        {
                            if ((unsigned int)v29 <= 2)
                                v29 = AI_MainActor[(uint32_t)v29 - 1];
                            AI_EvalFunc_OBJSetPos_C(v29, &v47);
                        }
                        else
                        {
                            AI_EvalFunc_OBJSetPos_C(0, &v47);
                        }
                    }
                }
            }
            if (v81->f_radar_activation_coef == 0.0)
            {
                if (v81->i_snd_rotor != -1)
                {
                    SND_Release(v81->i_snd_rotor);
                    v81->i_snd_rotor = -1;
                }
            }
            else
            {
                if (v81->i_snd_rotor == -1)
                {
                    v1 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 2, 0x18000000);
                    v81->i_snd_rotor = v1;
                    AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v81->i_snd_rotor);
                    v2 = SND_f_GetVol(v81->i_snd_rotor);
                    v81->f_snd_rotor_volref = (float)v2;
                    v3 = SND_l_GetFreq(v81->i_snd_rotor);
                    v81->i_snd_rotor_freqref = v3;
                }
                Xh = v81->f_snd_rotor_volref * v41;
                SND_SetVol(v81->i_snd_rotor, Xh);
                v18 = v41;
                if (v41 < 0.0)
                    v18 = 0.0;
                if (v18 > 1.0)
                    v18 = 1.0;
                v13 = (float)v81->i_snd_rotor_freqref;
                v78 = v13 * 1.5f * v18 + v13 * 0.2f * (1.0f - v18);
                SND_SetFreq(v81->i_snd_rotor, (unsigned int)v78);
            }
        }
        result = 1000;
    }
    else
    {
        if (v69)
        {
            if (v81->GFX_Halo != -1)
            {
                AI_EvalFunc_GFXDel_C_CURRENT(v81->GFX_Halo);
                v81->GFX_Halo = -1;
            }
        }
        else
        {
            if (v81->GFX_Halo == -1)
            {
                v81->GFX_Halo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v81->GFX_Halo, 4, 1);
                v40 = (tGameObjectMain*)torpille_tor_exec_visuel_SFX_light_smoke_gao;
                if (torpille_tor_exec_visuel_SFX_light_smoke_gao)
                {
                    if ((unsigned int)torpille_tor_exec_visuel_SFX_light_smoke_gao <= 2)
                        v40 = AI_MainActor[torpille_tor_exec_visuel_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v81->GFX_Halo, v40, 0);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v81->GFX_Halo, 0, 0);
                }
                GFXSetCurGlobal(v81->GFX_Halo);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21104, 3);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21006, 0.5);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 0.0);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21100, 4);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 0xFF0000FF);
            }
            v43 = AI_EvalFunc_OBJ_ZoomGet_C(AI_gpst_CurrentGameObject);
            if (v43 > 1.0)
                v16 = 1.0;
            else
                v16 = v43;
            v51 = AI_EvalFunc_COLORBlend_C(0, 0xFF0000FF, v16);
            GFXSetCurGlobal(v81->GFX_Halo);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v51);
            Xb = 10.0f * v16;
            AI_EvalFunc_GFXSetf_C_CURGLOB(21004, Xb);
            Xc = 1.5f * v16;
            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, Xc);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v71);
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v82);
            v39 = v16 * 1.5f;
            v52.x = v82.x * v39;
            v52.y = v82.y * v39;
            v52.z = v82.z * v39;
            MATH_VecAddVec(&v65, &v71, &v52);
            AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v65);
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v49);
            AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v49);
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int torpille_tor_exec_send_paf()
{
    BOOL v0; // eax
    tScriptStruct_PNJTorpille* v2; // [esp+0h] [ebp-114h]
    tGameObjectMain* v3; // [esp+8h] [ebp-10Ch]
    tGameObjectMain* v4; // [esp+18h] [ebp-FCh]
    tGameObjectMain* v5; // [esp+1Ch] [ebp-F8h]
    tGameObjectMain* v6; // [esp+20h] [ebp-F4h]
    tGameObjectMain* v7; // [esp+24h] [ebp-F0h]
    tGameObjectMain* v8; // [esp+28h] [ebp-ECh]
    tGameObjectMain* v9; // [esp+34h] [ebp-E0h]
    tGameObjectMain* v10; // [esp+44h] [ebp-D0h]
    tGameObjectMain* v11; // [esp+4Ch] [ebp-C8h]
    tGameObjectMain* v12; // [esp+50h] [ebp-C4h]
    tGameObjectMain* v13; // [esp+54h] [ebp-C0h]
    Vec3 v14; // [esp+60h] [ebp-B4h] BYREF
    Vec3 a3[2]; // [esp+6Ch] [ebp-A8h] BYREF
    Vec3 v16; // [esp+88h] [ebp-8Ch] BYREF
    tMessage Src; // [esp+98h] [ebp-7Ch] BYREF
    Vec3 dest; // [esp+CCh] [ebp-48h] BYREF
    Vec3 v19; // [esp+D8h] [ebp-3Ch] BYREF
    BOOL v20; // [esp+ECh] [ebp-28h]
    tScriptStruct_PNJTorpille* v21; // [esp+F0h] [ebp-24h]
    unsigned int v22; // [esp+F4h] [ebp-20h]
    int v23; // [esp+F8h] [ebp-1Ch]
    Vec3 v24; // [esp+FCh] [ebp-18h] BYREF

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJTorpille*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v21 = v2;
    v22 = 0;
    v23 = 0;
    v2->f_time_send_paf = TIM_gf_MainClock - TIM_gf_LockTime;
    v13 = (tGameObjectMain*)v21->o_collide_object;
    if (v13)
    {
        if ((unsigned int)v13 <= 2)
            v13 = AI_MainActor[(uint32_t)v13 - 1];
        v12 = v13;
    }
    else
    {
        v12 = 0;
    }
    if (v12)
    {
        v11 = (tGameObjectMain*)v21->o_collide_object;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v11 = AI_MainActor[(uint32_t)v11 - 1];
            v0 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v11, 32800);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 32800);
        }
        v20 = v0;
        if (v0)
        {
            Src.msgID = 37;
            Src.gao = AI_gpst_CurrentGameObject;
            Src.gao2 = 0;
            Src.Int1 = v21->des_puissance;
            Src.Int2 = 0;
            v10 = (tGameObjectMain*)v21->o_collide_object;
            if (v10)
            {
                if ((unsigned int)v10 <= 2)
                    v10 = AI_MainActor[(uint32_t)v10 - 1];
                AI_EvalFunc_OBJGetPos_C(v10, a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, a3);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v14);
            MATH_SubstractVec(&dest, a3, &v14);
            Src.v1 = dest;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&Src.v2);
            Src.f2 = (float)v21->des_i_point_de_vie;
            Src.f1 = 1000.0;
            v9 = (tGameObjectMain*)v21->o_collide_object;
            if (v9)
            {
                if ((unsigned int)v9 <= 2)
                    v9 = AI_MainActor[(uint32_t)v9 - 1];
                AI_EvalFunc_MSGSend_C(v9, &Src);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &Src);
            }
        }
    }
    v23 = AI_EvalFunc_COLZDEZDEListGet_C(AI_gpst_CurrentGameObject, (tGameObjectMain**)v21->ao_obj, 1, 1, 0x8020, 0, 0);
    while (v23)
    {
        v22 = v21->ao_obj[--v23];
        v8 = (tGameObjectMain*)v22;
        if (v22)
        {
            if (v22 <= 2)
                v8 = AI_MainActor[v22 - 1];
            v7 = v8;
        }
        else
        {
            v7 = 0;
        }
        v6 = (tGameObjectMain*)v21->o_collide_object;
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
        if (v7 != v5)
        {
            Src.msgID = 37;
            Src.gao = AI_gpst_CurrentGameObject;
            Src.gao2 = 0;
            Src.Int1 = v21->des_puissance;
            Src.Int2 = 0;
            v4 = (tGameObjectMain*)v22;
            if (v22)
            {
                if (v22 <= 2)
                    v4 = AI_MainActor[v22 - 1];
                AI_EvalFunc_OBJGetPos_C(v4, &v24);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v24);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v16);
            MATH_SubstractVec(&v19, &v24, &v16);
            Src.v1 = v19;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&Src.v2);
            Src.f2 = (float)v21->des_i_point_de_vie;
            Src.f1 = 1000.0;
            v3 = (tGameObjectMain*)v22;
            if (v22)
            {
                if (v22 <= 2)
                    v3 = AI_MainActor[v22 - 1];
                AI_EvalFunc_MSGSend_C(v3, &Src);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &Src);
            }
        }
    }
    return 1000;
}

int torpille_tor_mode_air()
{
    unsigned int v0; // eax
    BOOL v1; // eax
    BOOL v2; // eax
    unsigned int v3; // eax
    BOOL v4; // eax
    int v5; // eax
    BOOL v6; // eax
    double v7; // st7
    unsigned int v8; // eax
    int result; // eax
    float v10; // [esp+0h] [ebp-3A0h]
    float v11; // [esp+8h] [ebp-398h]
    float v12; // [esp+Ch] [ebp-394h]
    float v13; // [esp+10h] [ebp-390h]
    float v14; // [esp+14h] [ebp-38Ch]
    tScriptStruct_PNJTorpille* v15; // [esp+18h] [ebp-388h]
    tGameObjectMain* v16; // [esp+20h] [ebp-380h]
    tGameObjectMain* v17; // [esp+28h] [ebp-378h]
    tGameObjectMain* v18; // [esp+2Ch] [ebp-374h]
    tGameObjectMain* v19; // [esp+30h] [ebp-370h]
    tGameObjectMain* v20; // [esp+34h] [ebp-36Ch]
    tGameObjectMain* v21; // [esp+38h] [ebp-368h]
    tGameObjectMain* v22; // [esp+40h] [ebp-360h]
    tGameObjectMain* v23; // [esp+44h] [ebp-35Ch]
    tGameObjectMain* v24; // [esp+48h] [ebp-358h]
    float v25; // [esp+4Ch] [ebp-354h]
    float v26; // [esp+58h] [ebp-348h]
    tGameObjectMain* v27; // [esp+68h] [ebp-338h]
    float v28; // [esp+74h] [ebp-32Ch]
    tGameObjectMain* v29; // [esp+80h] [ebp-320h]
    tGameObjectMain* v30; // [esp+88h] [ebp-318h]
    tGameObjectMain* v31; // [esp+90h] [ebp-310h]
    float v32; // [esp+9Ch] [ebp-304h]
    float v33; // [esp+A0h] [ebp-300h]
    tGameObjectMain* v34; // [esp+A4h] [ebp-2FCh]
    tGameObjectMain* v35; // [esp+A8h] [ebp-2F8h]
    float v36; // [esp+ACh] [ebp-2F4h]
    tGameObjectMain* v37; // [esp+B0h] [ebp-2F0h]
    tGameObjectMain* v38; // [esp+B4h] [ebp-2ECh]
    tGameObjectMain* v39; // [esp+BCh] [ebp-2E4h]
    tGameObjectMain* v40; // [esp+C4h] [ebp-2DCh]
    tGameObjectMain* v41; // [esp+C8h] [ebp-2D8h]
    tGameObjectMain* v42; // [esp+CCh] [ebp-2D4h]
    tGameObjectMain* v43; // [esp+D0h] [ebp-2D0h]
    tGameObjectMain* v44; // [esp+D4h] [ebp-2CCh]
    tGameObjectMain* v45; // [esp+DCh] [ebp-2C4h]
    tGameObjectMain* v46; // [esp+E4h] [ebp-2BCh]
    tGameObjectMain* v47; // [esp+ECh] [ebp-2B4h]
    float v48; // [esp+F0h] [ebp-2B0h]
    float v49; // [esp+FCh] [ebp-2A4h]
    tGameObjectMain* v50; // [esp+110h] [ebp-290h]
    tGameObjectMain* v51; // [esp+118h] [ebp-288h]
    tGameObjectMain* v52; // [esp+128h] [ebp-278h]
    tGameObjectMain* v53; // [esp+130h] [ebp-270h]
    tGameObjectMain* v54; // [esp+134h] [ebp-26Ch]
    tGameObjectMain* v55; // [esp+138h] [ebp-268h]
    tGameObjectMain* v56; // [esp+13Ch] [ebp-264h]
    tGameObjectMain* v57; // [esp+140h] [ebp-260h]
    tGameObjectMain* v58; // [esp+144h] [ebp-25Ch]
    tGameObjectMain* v59; // [esp+148h] [ebp-258h]
    float v60; // [esp+14Ch] [ebp-254h]
    tGameObjectMain* v61; // [esp+160h] [ebp-240h]
    Vec3 v62; // [esp+164h] [ebp-23Ch] BYREF
    float v63; // [esp+170h] [ebp-230h]
    unsigned int v64; // [esp+174h] [ebp-22Ch]
    Vec3 v66; // [esp+17Ch] [ebp-224h]
    Vec3 v67; // [esp+188h] [ebp-218h] BYREF
    float v68; // [esp+194h] [ebp-20Ch]
    Vec3 a3; // [esp+198h] [ebp-208h] BYREF
    tGameObjectMain* v70; // [esp+1A4h] [ebp-1FCh]
    Vec3 v71; // [esp+1A8h] [ebp-1F8h] BYREF
    Vec3 v72; // [esp+1B4h] [ebp-1ECh] BYREF
    Vec3 v73; // [esp+1C0h] [ebp-1E0h] BYREF
    Vec3 v74; // [esp+1CCh] [ebp-1D4h] BYREF
    Vec3 v75; // [esp+1D8h] [ebp-1C8h] BYREF
    Vec3 v76; // [esp+1E4h] [ebp-1BCh] BYREF
    Vec3 v77; // [esp+1F0h] [ebp-1B0h] BYREF
    tGameObjectMain* v78; // [esp+1FCh] [ebp-1A4h]
    float v79; // [esp+200h] [ebp-1A0h]
    float v80; // [esp+204h] [ebp-19Ch]
    Vec3 v81; // [esp+208h] [ebp-198h] BYREF
    Vec3 v82; // [esp+214h] [ebp-18Ch] BYREF
    Vec3 v83; // [esp+220h] [ebp-180h] BYREF
    Vec3 v84; // [esp+22Ch] [ebp-174h] BYREF
    float v85; // [esp+238h] [ebp-168h]
    float X; // [esp+23Ch] [ebp-164h]
    Vec3 v87; // [esp+240h] [ebp-160h] BYREF
    BOOL v88; // [esp+24Ch] [ebp-154h]
    float v89; // [esp+250h] [ebp-150h]
    tGameObjectMain* v90; // [esp+254h] [ebp-14Ch]
    float v91; // [esp+258h] [ebp-148h]
    BOOL v92; // [esp+25Ch] [ebp-144h]
    Vec3 v93; // [esp+260h] [ebp-140h] BYREF
    float v94; // [esp+26Ch] [ebp-134h]
    BOOL v95; // [esp+278h] [ebp-128h]
    Vec3 v96; // [esp+27Ch] [ebp-124h]
    Vec3 dest; // [esp+288h] [ebp-118h] BYREF
    float v98; // [esp+294h] [ebp-10Ch]
    BOOL v99; // [esp+298h] [ebp-108h]
    int v100; // [esp+29Ch] [ebp-104h]
    Vec3 v101; // [esp+2A0h] [ebp-100h] BYREF
    Vec3 v102; // [esp+2ACh] [ebp-F4h] BYREF
    Vec3 v103; // [esp+2B8h] [ebp-E8h] BYREF
    Vec3 v104; // [esp+2C4h] [ebp-DCh]
    Vec3 v105; // [esp+2D0h] [ebp-D0h] BYREF
    Vec3 v106; // [esp+2DCh] [ebp-C4h]
    float v107; // [esp+2E8h] [ebp-B8h]
    float v108; // [esp+2F0h] [ebp-B0h]
    float v109; // [esp+2F8h] [ebp-A8h]
    Vec3 v110; // [esp+2FCh] [ebp-A4h] BYREF
    Vec3 a4; // [esp+308h] [ebp-98h] BYREF
    unsigned int v112; // [esp+314h] [ebp-8Ch]
    float v113; // [esp+318h] [ebp-88h]
    Vec3 v114; // [esp+320h] [ebp-80h] BYREF
    Vec3 v115; // [esp+32Ch] [ebp-74h] BYREF
    float v116; // [esp+338h] [ebp-68h]
    float v117; // [esp+340h] [ebp-60h]
    Vec3 v118; // [esp+344h] [ebp-5Ch] BYREF
    tScriptStruct_PNJTorpille* v119; // [esp+350h] [ebp-50h]
    int v120; // [esp+354h] [ebp-4Ch]
    Vec3 v121; // [esp+358h] [ebp-48h] BYREF
    unsigned int v122; // [esp+364h] [ebp-3Ch]
    BOOL v123; // [esp+36Ch] [ebp-34h]
    int v124; // [esp+374h] [ebp-2Ch]
    Vec3 v125; // [esp+378h] [ebp-28h] BYREF
    Vec3 v126; // [esp+384h] [ebp-1Ch] BYREF
    float v127; // [esp+390h] [ebp-10h]
    float v128; // [esp+394h] [ebp-Ch]
    int i; // [esp+398h] [ebp-8h]
    float v130; // [esp+39Ch] [ebp-4h]

    v81.x = 1.0;
    v81.y = 0.0;
    v81.z = 0.0;
    v110.x = 1.0;
    v110.y = 0.0;
    v110.z = 0.0;
    v103.x = 1.0;
    v103.y = 0.0;
    v103.z = 0.0;
    v87.x = 1.0;
    v87.y = 0.0;
    v87.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v15 = (tScriptStruct_PNJTorpille*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v15 = 0;
    v119 = v15;
    v99 = 0;
    v85 = 0.0;
    v89 = 0.0;
    i = 0;
    v90 = 0;
    X = 0.0;
    v91 = 0.0;
    v120 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fctvar_torpille_tor_mode_air_aircraft = AI_ul_RealResolveOneRef(42, 0x7100BBCCu);
        fct_torpille_tor_mode_air_torpille_tor_mode_air = (int)AI_ul_RealResolveOneRefFunction(0x72000722u);
        fct_torpille_tor_mode_air_torpille_tor_mode_explosion = (int)AI_ul_RealResolveOneRefFunction(0x7200069Bu);
        fct_torpille_tor_mode_air_torpille_tor_exec_test_collision = (int)AI_ul_RealResolveOneRefFunction(0x72001C27u);
    }
    if (v119->i_current_mode != 1)
    {
        v119->i_current_mode = 1;
        v83.x = 0.0;
        v83.y = 0.0;
        v83.z = 0.0;
        AI_EvalFunc_DYNTractionSet_C_CURRENT(&v83);
        v119->f_time_change_mode = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    v90 = *AI_MainActor;
    if (v119->des_propulsion && v119->i_flag_engine_on)
    {
        v61 = (tGameObjectMain*)v119->des_o_target;
        if (v61)
        {
            if ((unsigned int)v61 <= 2)
                v61 = AI_MainActor[(uint32_t)v61 - 1];
            AI_EvalFunc_OBJGetPos_C(v61, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v67);
        MATH_SubstractVec(&dest, &a3, &v67);
        v87 = dest;
        X = MATH_VecLen(&v87);
        v60 = 1.0f / X;
        v87.x = v87.x * v60;
        v87.y = v87.y * v60;
        v87.z = v87.z * v60;
        v59 = (tGameObjectMain*)v119->des_o_target;
        if (v59)
        {
            if ((unsigned int)v59 <= 2)
                v59 = AI_MainActor[(uint32_t)v59 - 1];
            v58 = v59;
        }
        else
        {
            v58 = 0;
        }
        v57 = v90;
        if (v90)
        {
            if ((unsigned int)v90 <= 2)
                v57 = AI_MainActor[(uint32_t)v90 - 1];
            v56 = v57;
        }
        else
        {
            v56 = 0;
        }
        if (v58 == v56 && v119->des_f_target_lock_dist > (double)X)
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v76);
            v109 = v87.z * v76.z + v87.y * v76.y + v76.x * v87.x;
            if (v119->des_angle_lock < (double)v109)
            {
                for (i = 0; i < 50; ++i)
                {
                    v55 = (tGameObjectMain*)v119->des_o_target_lock[i];
                    if (v55)
                    {
                        if ((unsigned int)v55 <= 2)
                            v55 = AI_MainActor[(uint32_t)v55 - 1];
                        v54 = v55;
                    }
                    else
                    {
                        v54 = 0;
                    }
                    if (!v54)
                        break;
                    v53 = (tGameObjectMain*)v119->des_o_target_lock[i];
                    if (v53)
                    {
                        if ((unsigned int)v53 <= 2)
                            v53 = AI_MainActor[(uint32_t)v53 - 1];
                        v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(v53);
                    }
                    else
                    {
                        v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
                    }
                    v122 = v0;
                    if ((v0 & 0x400000) == 0)
                    {
                        v52 = (tGameObjectMain*)v119->des_o_target_lock[i];
                        if (v52)
                        {
                            if ((unsigned int)v52 <= 2)
                                v52 = AI_MainActor[(uint32_t)v52 - 1];
                            AI_EvalFunc_OBJGetPos_C(v52, &v84);
                        }
                        else
                        {
                            AI_EvalFunc_OBJGetPos_C(0, &v84);
                        }
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&v105);
                        MATH_SubstractVec(&v73, &v84, &v105);
                        v87 = v73;
                        v51 = (tGameObjectMain*)v119->des_o_target;
                        if (v51)
                        {
                            if ((unsigned int)v51 <= 2)
                                v51 = AI_MainActor[(uint32_t)v51 - 1];
                            AI_EvalFunc_OBJGetPos_C(v51, &v126);
                        }
                        else
                        {
                            AI_EvalFunc_OBJGetPos_C(0, &v126);
                        }
                        v50 = (tGameObjectMain*)v119->des_o_target_lock[i];
                        if (v50)
                        {
                            if ((unsigned int)v50 <= 2)
                                v50 = AI_MainActor[(uint32_t)v50 - 1];
                            AI_EvalFunc_OBJGetPos_C(v50, &v102);
                        }
                        else
                        {
                            AI_EvalFunc_OBJGetPos_C(0, &v102);
                        }
                        MATH_SubstractVec(&v82, &v126, &v102);
                        v110 = v82;
                        X = MATH_VecLen(&v110);
                        v49 = 1.0f / X;
                        v110.x = v110.x * v49;
                        v110.y = v110.y * v49;
                        v110.z = v110.z * v49;
                        v66.x = -v87.x;
                        v66.y = -v87.y;
                        v66.z = -v87.z;
                        v91 = v110.z * -v87.z
                            + v110.y * -v87.y
                            + -v87.x * v110.x;
                        if (v91 < 0.0 || v91 >(double)X)
                        {
                            X = v87.z * v87.z + v87.y * v87.y + v87.x * v87.x;
                            if (v119->f_target_lock_sqr_dist > (double)X)
                            {
                                X = sqrtf(X);
                                v48 = 1.0f / X;
                                v87.x = v87.x * v48;
                                v87.y = v87.y * v48;
                                v87.z = v87.z * v48;
                                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v125);
                                v108 = v87.z * v125.z + v87.y * v125.y + v125.x * v87.x;
                                if (v119->des_angle_lock < (double)v108)
                                {
                                    v119->o_last_target = v119->des_o_target;
                                    v119->des_o_target = v119->des_o_target_lock[i];
                                    goto LABEL_64;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (!v119->des_lock_target_mode)
            goto LABEL_77;
    LABEL_64:
        v47 = (tGameObjectMain*)v119->des_o_target;
        if (v47)
        {
            if ((unsigned int)v47 <= 2)
                v47 = AI_MainActor[(uint32_t)v47 - 1];
            v1 = AI_EvalFunc_AIIsModel_C(v47, (tAIModel*)fctvar_torpille_tor_mode_air_aircraft);
        }
        else
        {
            v1 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_torpille_tor_mode_air_aircraft);
        }
        v92 = v1;
        if (v1)
        {
            v46 = (tGameObjectMain*)v119->des_o_target;
            if (v46)
            {
                if ((unsigned int)v46 <= 2)
                    v46 = AI_MainActor[(uint32_t)v46 - 1];
                v2 = AI_EvalFunc_OBJ_CapaTest_C(v46, 0x2000);
            }
            else
            {
                v2 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x2000);
            }
            v88 = v2;
            v99 = v2;
        }
        else
        {
        LABEL_77:
            v120 = 0;
            v45 = (tGameObjectMain*)v119->des_o_target;
            if (v45)
            {
                if ((unsigned int)v45 <= 2)
                    v45 = AI_MainActor[(uint32_t)v45 - 1];
                v3 = AI_EvalFunc_OBJ_FlagsControlGet_C(v45);
            }
            else
            {
                v3 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
            }
            v112 = v3;
            if ((v3 & 0x600000) != 0)
            {
                v70 = *AI_MainActor;
                v44 = (tGameObjectMain*)v119->des_o_target;
                if (v44)
                {
                    if ((unsigned int)v44 <= 2)
                        v44 = AI_MainActor[(uint32_t)v44 - 1];
                    v43 = v44;
                }
                else
                {
                    v43 = 0;
                }
                v42 = v70;
                if (v70)
                {
                    if ((unsigned int)v70 <= 2)
                        v42 = AI_MainActor[(uint32_t)v70 - 1];
                    v41 = v42;
                }
                else
                {
                    v41 = 0;
                }
                if (v43 != v41)
                    v120 = 1;
            }
            else
            {
                v40 = (tGameObjectMain*)v119->des_o_target;
                if (v40)
                {
                    if ((unsigned int)v40 <= 2)
                        v40 = AI_MainActor[(uint32_t)v40 - 1];
                    v4 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v40, 0x4000);
                }
                else
                {
                    v4 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x4000);
                }
                v123 = v4;
                if (v4)
                {
                    v39 = (tGameObjectMain*)v119->des_o_target;
                    if (v39)
                    {
                        if ((unsigned int)v39 <= 2)
                            v39 = AI_MainActor[(uint32_t)v39 - 1];
                        v5 = AI_EvalFunc_OBJDrawMaskGet_C(v39);
                    }
                    else
                    {
                        v5 = AI_EvalFunc_OBJDrawMaskGet_C(0);
                    }
                    v100 = v5;
                    if ((v5 & 8) == 0)
                        v120 = 1;
                }
            }
            if (v120)
            {
                v38 = (tGameObjectMain*)v119->o_last_target;
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
                if (v37)
                {
                    v119->des_o_target = v119->o_last_target;
                    v119->o_last_target = 0;
                }
                v99 = 0;
            }
            else
            {
                v99 = 1;
            }
        }
        if (v99)
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v118);
            v98 = v87.z * v118.z + v87.y * v118.y + v118.x * v87.x;
            if (v98 < 0.0)
                v14 = 0.0;
            else
                v14 = v98;
            v107 = v14;
            v36 = 1.0f - v14;
            if (v36 > 0.1)
                v13 = 1.0f - v14;
            else
                v13 = 0.1f;
            v85 = v13;
            v35 = (tGameObjectMain*)v119->o_last_target;
            if (v35)
            {
                if ((unsigned int)v35 <= 2)
                    v35 = AI_MainActor[(uint32_t)v35 - 1];
                v34 = v35;
            }
            else
            {
                v34 = 0;
            }
            if (v34)
                v89 = 60.0;
            else
                v89 = 12.0;
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v75);
            v128 = v87.z * v75.z + v87.y * v75.y + v75.x * v87.x;
            if (v128 <= 0.0)
            {
                v79 = TIM_gf_dt;
                v32 = v89 * TIM_gf_dt;
                if (v32 < 0.0)
                    v32 = 0.0;
                if (v32 > 1.0)
                    v32 = 1.0;
                v117 = -v119->des_f_rotation_speed * v85 * v32 + v119->f_sight_rotspeed * (1.0f - v32);
                v119->f_sight_rotspeed = v117;
            }
            else
            {
                v113 = TIM_gf_dt;
                v33 = v89 * TIM_gf_dt;
                if (v33 < 0.0)
                    v33 = 0.0;
                if (v33 > 1.0)
                    v33 = 1.0;
                v68 = v119->des_f_rotation_speed * v85 * v33 + v119->f_sight_rotspeed * (1.0f - v33);
                v119->f_sight_rotspeed = v68;
            }
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v121);
            v101.x = 0.0;
            v101.y = 0.0;
            v101.z = 1.0;
            v94 = TIM_gf_dt;
            v10 = v119->f_sight_rotspeed * TIM_gf_dt;
            AI_EvalFunc_MATHVecRotate_C(&v121, &v101, v10, &a4);
            v81 = a4;
            v77.x = 0.0;
            v77.y = 0.0;
            v77.z = 1.0;
            AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v81, &v77);
        }
        if (!v119->des_seb_mode)
            goto LABEL_176;
        v31 = (tGameObjectMain*)v119->des_o_target;
        if (v31)
        {
            if ((unsigned int)v31 <= 2)
                v31 = AI_MainActor[(uint32_t)v31 - 1];
            v6 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v31, 4);
        }
        else
        {
            v6 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 4);
        }
        v95 = v6;
        if (v6)
        {
            v30 = (tGameObjectMain*)v119->des_o_target;
            if (v30)
            {
                if ((unsigned int)v30 <= 2)
                    v30 = AI_MainActor[(uint32_t)v30 - 1];
                v7 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v30);
            }
            else
            {
                v7 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
            }
            v127 = (float)v7;
            if (v127 <= 400.0)
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v74);
                v27 = (tGameObjectMain*)v119->des_o_target;
                if (v27)
                {
                    if ((unsigned int)v27 <= 2)
                        v27 = AI_MainActor[(uint32_t)v27 - 1];
                    AI_EvalFunc_DYNSpeedGetVector_C(v27, &v93);
                }
                else
                {
                    AI_EvalFunc_DYNSpeedGetVector_C(0, &v93);
                }
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v62);
                v116 = v62.z * v93.z + v62.y * v93.y + v93.x * v62.x;
                if (v116 < 60.0)
                    v11 = 60.0;
                else
                    v11 = v116;
                v63 = v11;
                v106.x = v74.x * v11;
                v106.y = v74.y * v11;
                v106.z = v74.z * v11;
                v103 = v106;
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v115);
                v29 = (tGameObjectMain*)v119->des_o_target;
                if (v29)
                {
                    if ((unsigned int)v29 <= 2)
                        v29 = AI_MainActor[(uint32_t)v29 - 1];
                    AI_EvalFunc_DYNSpeedGetVector_C(v29, &v71);
                }
                else
                {
                    AI_EvalFunc_DYNSpeedGetVector_C(0, &v71);
                }
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v114);
                v130 = v114.z * v71.z + v114.y * v71.y + v71.x * v114.x;
                v28 = v130 + 10.0f;
                if (v28 < (double)v119->des_f_speed)
                    v12 = v119->des_f_speed;
                else
                    v12 = v130 + 10.0f;
                v80 = v12;
                v96.x = v115.x * v12;
                v96.y = v115.y * v12;
                v96.z = v115.z * v12;
                v103 = v96;
            }
        }
        else
        {
        LABEL_176:
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v72);
            v26 = v119->des_f_speed;
            v104.x = v72.x * v26;
            v104.y = v72.y * v26;
            v104.z = v72.z * v26;
            v103 = v104;
        }
        v25 = v119->des_v_air_friction.y;
        v103.x = v103.x * v25;
        v103.y = v103.y * v25;
        v103.z = v103.z * v25;
        AI_EvalFunc_DYNTractionSet_C_CURRENT(&v103);
    }
    v120 = 0;
    v24 = (tGameObjectMain*)v119->des_o_target;
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
    if (!v23)
        goto LABEL_223;
    v22 = (tGameObjectMain*)v119->des_o_target;
    if (v22)
    {
        if ((unsigned int)v22 <= 2)
            v22 = AI_MainActor[(uint32_t)v22 - 1];
        v8 = AI_EvalFunc_OBJ_FlagsControlGet_C(v22);
    }
    else
    {
        v8 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
    }
    v64 = v8;
    if ((v8 & 0x600000) != 0)
    {
        v78 = *AI_MainActor;
        v21 = (tGameObjectMain*)v119->des_o_target;
        if (v21)
        {
            if ((unsigned int)v21 <= 2)
                v21 = AI_MainActor[(uint32_t)v21 - 1];
            v20 = v21;
        }
        else
        {
            v20 = 0;
        }
        v19 = v78;
        if (v78)
        {
            if ((unsigned int)v78 <= 2)
                v19 = AI_MainActor[(uint32_t)v78 - 1];
            v18 = v19;
        }
        else
        {
            v18 = 0;
        }
        if (v20 != v18)
            v120 = 1;
    }
    else
    {
    LABEL_223:
        if (v119->des_f_mine_time != 0.0)
        {
            if (AI_EvalFunc_TIMEElapsed_C(v119->f_time_start, v119->des_f_mine_time))
                v120 = 1;
        }
    }
    if (!v120)
        goto LABEL_218;
    v119->i_flag_mode_air = 0;
    v17 = (tGameObjectMain*)v119->o_me;
    if (v17)
    {
        if ((unsigned int)v17 <= 2)
            v17 = AI_MainActor[(uint32_t)v17 - 1];
        AI_EvalFunc_AICBDel_C(
            AI_gpst_CurrentGameObject,
            v17,
            2,
            (tAIFunction*)fct_torpille_tor_mode_air_torpille_tor_mode_air);
    }
    else
    {
        AI_EvalFunc_AICBDel_C(
            AI_gpst_CurrentGameObject,
            0,
            2,
            (tAIFunction*)fct_torpille_tor_mode_air_torpille_tor_mode_air);
    }
    if (v119->i_flag_mode_explosion)
    {
    LABEL_218:
        v124 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_torpille_tor_mode_air_torpille_tor_exec_test_collision);
        if (v124 == 1000)
            result = 1000;
        else
            result = v124;
    }
    else
    {
        v119->i_flag_mode_explosion = 1;
        v16 = (tGameObjectMain*)v119->o_me;
        if (v16)
        {
            if ((unsigned int)v16 <= 2)
                v16 = AI_MainActor[(uint32_t)v16 - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v16,
                2,
                (tAIFunction*)fct_torpille_tor_mode_air_torpille_tor_mode_explosion);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_torpille_tor_mode_air_torpille_tor_mode_explosion);
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int torpille_tor_exec_trace()
{
    float X_4; // [esp+4h] [ebp-370h]
    float X_4a; // [esp+4h] [ebp-370h]
    float X_4b; // [esp+4h] [ebp-370h]
    float X_4c; // [esp+4h] [ebp-370h]
    float X_4d; // [esp+4h] [ebp-370h]
    float X_4e; // [esp+4h] [ebp-370h]
    float v7; // [esp+8h] [ebp-36Ch]
    float v8; // [esp+Ch] [ebp-368h]
    float v9; // [esp+10h] [ebp-364h]
    tScriptStruct_PNJTorpille* v10; // [esp+14h] [ebp-360h]
    float v11; // [esp+20h] [ebp-354h]
    float v12; // [esp+24h] [ebp-350h]
    float v13; // [esp+28h] [ebp-34Ch]
    float v14; // [esp+2Ch] [ebp-348h]
    float v15; // [esp+30h] [ebp-344h]
    float v16; // [esp+34h] [ebp-340h]
    float v17; // [esp+38h] [ebp-33Ch]
    float v18; // [esp+3Ch] [ebp-338h]
    float v19; // [esp+40h] [ebp-334h]
    float v20; // [esp+44h] [ebp-330h]
    float v21; // [esp+48h] [ebp-32Ch]
    float v22; // [esp+64h] [ebp-310h]
    float v23; // [esp+68h] [ebp-30Ch]
    tGameObjectMain* v24; // [esp+A0h] [ebp-2D4h]
    Vec3 v25; // [esp+A4h] [ebp-2D0h] BYREF
    Vec3 v26; // [esp+B0h] [ebp-2C4h] BYREF
    float v27; // [esp+BCh] [ebp-2B8h]
    float v28; // [esp+C0h] [ebp-2B4h]
    float v29; // [esp+C4h] [ebp-2B0h]
    float v30; // [esp+C8h] [ebp-2ACh]
    float v31; // [esp+CCh] [ebp-2A8h]
    float v32; // [esp+D0h] [ebp-2A4h]
    int v33; // [esp+D4h] [ebp-2A0h]
    float v34; // [esp+D8h] [ebp-29Ch]
    float v35; // [esp+E4h] [ebp-290h]
    float v36; // [esp+ECh] [ebp-288h]
    float v37; // [esp+F4h] [ebp-280h]
    int v38; // [esp+F8h] [ebp-27Ch]
    Vec3 v39; // [esp+FCh] [ebp-278h] BYREF
    Vec3 v40; // [esp+108h] [ebp-26Ch] BYREF
    Vec3 v41; // [esp+114h] [ebp-260h] BYREF
    Vec3 v42; // [esp+120h] [ebp-254h] BYREF
    float v43; // [esp+12Ch] [ebp-248h]
    Vec3 v44; // [esp+130h] [ebp-244h] BYREF
    float v45; // [esp+13Ch] [ebp-238h]
    int v46; // [esp+140h] [ebp-234h]
    float v47; // [esp+144h] [ebp-230h]
    Vec3 v48; // [esp+148h] [ebp-22Ch] BYREF
    float v49; // [esp+154h] [ebp-220h]
    int v50; // [esp+158h] [ebp-21Ch]
    float v51; // [esp+15Ch] [ebp-218h]
    Vec3 v52; // [esp+160h] [ebp-214h] BYREF
    float v53; // [esp+16Ch] [ebp-208h]
    float v54; // [esp+170h] [ebp-204h]
    Vec3 v55; // [esp+174h] [ebp-200h] BYREF
    Vec3 v56; // [esp+180h] [ebp-1F4h] BYREF
    float v57; // [esp+18Ch] [ebp-1E8h]
    int v59; // [esp+19Ch] [ebp-1D8h]
    Vec3 v61; // [esp+1A4h] [ebp-1D0h] BYREF
    float v62; // [esp+1B0h] [ebp-1C4h]
    int v63; // [esp+1B4h] [ebp-1C0h]
    float v64; // [esp+1B8h] [ebp-1BCh]
    Vec3 v65; // [esp+1BCh] [ebp-1B8h] BYREF
    Vec3 v66; // [esp+1C8h] [ebp-1ACh] BYREF
    float v67; // [esp+1D4h] [ebp-1A0h]
    Vec3 v68; // [esp+1DCh] [ebp-198h] BYREF
    Vec3 v69; // [esp+1E8h] [ebp-18Ch] BYREF
    Vec3 v70; // [esp+1F4h] [ebp-180h] BYREF
    Vec3 v71; // [esp+200h] [ebp-174h] BYREF
    float v72; // [esp+20Ch] [ebp-168h]
    Vec3 v73; // [esp+214h] [ebp-160h] BYREF
    Vec3 v74; // [esp+220h] [ebp-154h] BYREF
    float v75; // [esp+22Ch] [ebp-148h]
    int v76; // [esp+230h] [ebp-144h]
    float v77; // [esp+234h] [ebp-140h]
    Vec3 v78; // [esp+238h] [ebp-13Ch] BYREF
    Vec3 v79; // [esp+244h] [ebp-130h]
    float a3; // [esp+250h] [ebp-124h]
    Vec3 v81; // [esp+254h] [ebp-120h] BYREF
    float v82; // [esp+264h] [ebp-110h]
    float v83; // [esp+268h] [ebp-10Ch]
    Vec3 v84; // [esp+26Ch] [ebp-108h] BYREF
    Vec3 v85; // [esp+278h] [ebp-FCh] BYREF
    Vec3 v86; // [esp+284h] [ebp-F0h] BYREF
    Vec3 v87; // [esp+290h] [ebp-E4h] BYREF
    Vec3 a1; // [esp+29Ch] [ebp-D8h] BYREF
    float v89; // [esp+2A8h] [ebp-CCh]
    float v90; // [esp+2ACh] [ebp-C8h]
    float v91; // [esp+2B0h] [ebp-C4h]
    int v92; // [esp+2B4h] [ebp-C0h]
    float v93; // [esp+2B8h] [ebp-BCh]
    Vec3 v94; // [esp+2BCh] [ebp-B8h] BYREF
    float v95; // [esp+2C8h] [ebp-ACh]
    int v96; // [esp+2CCh] [ebp-A8h]
    float v97; // [esp+2D0h] [ebp-A4h]
    float v98; // [esp+2D4h] [ebp-A0h]
    int v99; // [esp+2D8h] [ebp-9Ch]
    float v100; // [esp+2DCh] [ebp-98h]
    Vec3 v101; // [esp+2E0h] [ebp-94h] BYREF
    tScriptStruct_PNJTorpille* v102; // [esp+2ECh] [ebp-88h]
    float v103; // [esp+2F0h] [ebp-84h]
    float v104; // [esp+2F4h] [ebp-80h]
    int v105; // [esp+2FCh] [ebp-78h]
    float v106; // [esp+300h] [ebp-74h]
    Vec3 v107; // [esp+304h] [ebp-70h]
    Vec3 v108; // [esp+310h] [ebp-64h] BYREF
    Vec3 v109; // [esp+31Ch] [ebp-58h] BYREF
    Vec3 a2; // [esp+328h] [ebp-4Ch] BYREF
    float v111; // [esp+338h] [ebp-3Ch]
    float v112; // [esp+33Ch] [ebp-38h]
    Vec3 v113; // [esp+340h] [ebp-34h] BYREF
    Vec3 v114; // [esp+34Ch] [ebp-28h] BYREF
    Vec3 v115; // [esp+358h] [ebp-1Ch] BYREF
    Vec3 v116; // [esp+364h] [ebp-10h] BYREF

    v85.x = 1.0;
    v85.y = 0.0;
    v85.z = 0.0;
    v68.x = 1.0;
    v68.y = 0.0;
    v68.z = 0.0;
    v56.x = 1.0;
    v56.y = 0.0;
    v56.z = 0.0;
    v41.x = 1.0;
    v41.y = 0.0;
    v41.z = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v10 = (tScriptStruct_PNJTorpille*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v10 = 0;
    v102 = v10;
    v27 = 0.0;
    v35 = 0.0;
    v103 = 0.0;
    v89 = 0.0;
    v67 = 0.0;
    v36 = 0.0;
    v57 = 0.0;
    v111 = 0.0;
    v28 = 0.0;
    v59 = 0;
    v72 = 0.0;
    v29 = 0.0;
    v104 = 0.0;
    v37 = 0.0;
    v38 = 0;
    v54 = 0.0;
    v105 = 0;
    v83 = 0.0;
    v30 = 0.0;
    v112 = 0.0;
    v106 = 0.0;
    v43 = 0.0;
    v31 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        torpille_tor_exec_trace_PJ_aircraft_material_gao = AI_ul_RealResolveOneRefObject(0x720006F2u);
    }
    if (v102->GFX_Airtrace == -1)
    {
        v102->GFX_Airtrace = AI_EvalFunc_GFXAdd_C_CURRENT(12);
        v24 = (tGameObjectMain*)torpille_tor_exec_trace_PJ_aircraft_material_gao;
        if (torpille_tor_exec_trace_PJ_aircraft_material_gao)
        {
            if ((unsigned int)torpille_tor_exec_trace_PJ_aircraft_material_gao <= 2)
                v24 = AI_MainActor[torpille_tor_exec_trace_PJ_aircraft_material_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v102->GFX_Airtrace, v24, 0);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v102->GFX_Airtrace, 0, 0);
        }
        GFXSetCurGlobal(v102->GFX_Airtrace);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12100, 6);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12101, 5);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12102, 15);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12103, 1);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12111, 8);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12000, -95.5);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12004, 2.0);
        v101.x = 0.2f;
        v101.y = 0.2f;
        v101.z = 0.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(12202, &v101);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12105, 0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12106, 0);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12107, 1);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12001, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12002, 0.2f);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12105, 1);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12106, 1);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12107, 2);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12001, 0.2f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12002, 0.40000001f);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12105, 2);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12106, 2);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12107, 3);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12001, 0.40000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12002, 0.60000002f);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12105, 3);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12106, 3);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12107, 4);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12001, 0.60000002f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12002, 0.80000001f);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12105, 4);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12106, 4);
        AI_EvalFunc_GFXSeti_C_CURGLOB(12107, 5);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12001, 0.80000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(12002, 1.0);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v102->GFX_Airtrace, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v102->GFX_Airtrace, 2, 1);
        v102->i_flag_trace_on = 0;
    }
    v105 = 0;
    AI_EvalFunc_COLPosGet_C(AI_gpst_CurrentGameObject, 0, &v81);
    v68 = v81;
    v111 = v81.z;
    v54 = AI_EvalFunc_COLSizeGet_C(AI_gpst_CurrentGameObject, 0);
    v57 = v111 - v54;
    if (v102->f_water_Z == -1.0e14f)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v69);
        if (v102->f_last_valid_water_Z > (double)v69.z)
            v105 = 1;
        v31 = v102->f_last_valid_water_Z;
        v36 = v57 - v31;
    }
    else
    {
        v31 = v102->f_water_Z;
        v36 = v57 - v31;
        if (v36 < 0.0)
            v102->f_time_gfx_trace = TIM_gf_MainClock - TIM_gf_LockTime;
    }
    if (v105 && !v102->i_flag_trace_on)
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    v112 = 0.25f;
    v30 = 20.0;
    v72 = 20.0;
    v43 = 60.0;
    v67 = 60.0;
    AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&a2);
    a1.x = a2.x;
    a1.y = a2.y;
    a1.z = 0.0;
    v53 = MATH_VecLen(&a1);
    v106 = v53 / 125.0f;
    v82 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
    if (v82 == 0.0)
    {
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v66);
        v85 = v66;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v86);
        v41 = v86;
        v79.x = 0.0;
        v79.y = 0.0;
        v79.z = 1.0;
        v56.x = 0.0;
        v56.y = 0.0;
        v56.z = 1.0;
        v89 = 0.0;
        v29 = 0.0;
    }
    else
    {
        v107.x = 0.0;
        v107.y = 0.0;
        v107.z = 1.0;
        v56.x = 0.0;
        v56.y = 0.0;
        v56.z = 1.0;
        AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v55);
        a3 = AI_EvalFunc_DYNSpeedGet_C(AI_gpst_CurrentGameObject);
        MATH_VecMultNormScalar(&v42, &v55, a3);
        v41 = v42;
        AI_EvalFunc_MATHVecCrossProduct_C(&v41, &v56, &v116);
        v85 = v116;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v74);
        v35 = v74.z * v41.z + v74.y * v41.y + v41.x * v74.x;
        if (v35 <= 0.1)
        {
            if (v35 >= -0.1)
            {
                v43 = v43 * 0.5f;
                v30 = v30 * 0.5f;
                v89 = 1.0;
                v67 = v67 * 0.5f;
                v72 = v72 * 0.5f;
                v29 = 1.0;
            }
            else
            {
                v22 = -v35 - v35;
                if (v22 > 1.0)
                    v8 = 1.0;
                else
                    v8 = -v35 - v35;
                v104 = v8;
                v43 = v43 * 0.5f;
                v30 = v30 * 0.5f;
                v89 = 0.5f * v35 + 1.0f;
                v67 = v67 * v8;
                v72 = v72 * v8;
                v29 = 1.0;
            }
        }
        else
        {
            v23 = v35 + v35;
            if (v23 > 1.0)
                v9 = 1.0;
            else
                v9 = v35 + v35;
            v104 = v9;
            v43 = v43 * v9;
            v30 = v30 * v9;
            v89 = 1.0;
            v67 = v67 * 0.5f;
            v72 = v72 * 0.5f;
            v29 = 1.0f - 0.5f * v35;
        }
    }
    v21 = v106 * 4.0f;
    if (v21 <= 1.0)
    {
        if (v21 >= 0.2)
            v20 = v106 * 4.0f;
        else
            v20 = 0.2f;
    }
    else
    {
        v20 = 1.0;
    }
    v89 = v89 * v20;
    v19 = v106 * 4.0f;
    if (v19 <= 1.0)
    {
        if (v19 >= 0.2)
            v18 = v106 * 4.0f;
        else
            v18 = 0.2f;
    }
    else
    {
        v18 = 1.0;
    }
    v29 = v29 * v18;
    v43 = v43 * v106 * 0.25f + v43;
    v67 = v67 * v106 * 0.25f + v67;
    v30 = v30 * v106 * 0.5f + v30;
    v72 = v72 * v106 * 0.5f + v72;
    if (v36 >= 0.1)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v102->f_time_gfx_trace, 0.2f))
        {
            v89 = 0.0;
            v29 = 0.0;
            if (AI_EvalFunc_TIMEElapsed_C(v102->f_time_gfx_trace, 0.5f))
            {
            LABEL_48:
                AI_EvalFunc_GFXSeti_C_CURRENT(v102->GFX_Airtrace, 12103, 0);
                v102->i_flag_trace_on = 0;
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                return 1000;
            }
        }
        else
        {
            v90 = TIM_gf_MainClock - TIM_gf_LockTime;
            v83 = 1.0f - (v90 - v102->f_time_gfx_trace) / 0.2f;
            v89 = v89 * v83;
            v29 = v29 * v83;
        }
    }
    if (v105)
        goto LABEL_48;
    if (!v102->i_flag_trace_on)
    {
        v89 = 0.0;
        v29 = 0.0;
        v102->i_flag_trace_on = 1;
    }
    v59 = AI_EvalFunc_COLORBlend_C(0, -1, v89);
    v38 = AI_EvalFunc_COLORBlend_C(0, -1, v29);
    v17 = -v36 / (v54 + v54);
    if (v17 <= 1.0)
    {
        if (v17 >= 0.1)
            v37 = -v36 / (v54 + v54);
        else
            v37 = 0.1f;
    }
    else
    {
        v37 = 1.0;
    }
    v7 = (float)fabs(v37);
    v27 = v7 * 4.0f;
    v28 = v7 * 2.5f;
    v103 = v7 * 1.5f;
    v37 = v7 * 0.5f + 0.5f;
    v72 = v37 * v106 * v72;
    v30 = v37 * v106 * v30;
    v67 = v37 * v106 * v67;
    v43 = v37 * v106 * v43;
    GFXSetCurGlobal(v102->GFX_Airtrace);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12104, 0);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12109, v59);
    v71.x = -v27;
    v71.y = 0.0;
    v71.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12200, &v71);
    v75 = -v43 * 1.5f;
    v76 = 0;
    v77 = v30 * 1.5f;
    v16 = 1.0f - v112 + 1.0f - v112;
    v109.x = v75 * v16;
    v109.y = 0.0f * v16;
    v109.z = v77 * v16;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12201, &v109);
    v45 = -v43 * 1.5f;
    v46 = 0;
    v47 = v30 * 1.5f;
    v15 = 2.0f * v112;
    v113.x = v45 * v15;
    v113.y = 0.0f * v15;
    v113.z = v47 * v15;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12208, &v113);
    X_4 = v102->f_last_valid_water_Z + 0.2f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(12003, X_4);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12104, 5);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12109, v38);
    v84.x = v27;
    v84.y = 0.0;
    v84.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12200, &v84);
    v32 = v67 * 1.5f;
    v33 = 0;
    v34 = v72 * 1.5f;
    v14 = 1.0f - v112 + 1.0f - v112;
    v52.x = v32 * v14;
    v52.y = 0.0f * v14;
    v52.z = v34 * v14;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12201, &v52);
    v91 = v67 * 1.5f;
    v92 = 0;
    v93 = v72 * 1.5f;
    v13 = 2.0f * v112;
    v108.x = v91 * v13;
    v108.y = 0.0f * v13;
    v108.z = v93 * v13;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12208, &v108);
    X_4a = v102->f_last_valid_water_Z + 0.2f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(12003, X_4a);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12104, 1);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12109, v59);
    v73.x = -v28;
    v73.y = 0.0;
    v73.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12200, &v73);
    v62 = -v43;
    v63 = 0;
    v64 = v30;
    v12 = 1.0f - v112;
    v87.x = v62 * v12;
    v87.y = 0.0f * v12;
    v87.z = v30 * v12;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12201, &v87);
    v49 = -v43;
    v50 = 0;
    v51 = v30;
    v61.x = v49 * v112;
    v61.y = 0.0f * v112;
    v61.z = v30 * v112;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12208, &v61);
    X_4b = v102->f_last_valid_water_Z + 0.2f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(12003, X_4b);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12104, 4);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12109, v38);
    v114.x = v28;
    v114.y = 0.0f;
    v114.z = 0.0f;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12200, &v114);
    v98 = v67;
    v99 = 0;
    v100 = v72;
    v11 = 1.0f - v112;
    v39.x = v67 * v11;
    v39.y = 0.0f * v11;
    v39.z = v72 * v11;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12201, &v39);
    v95 = v67;
    v96 = 0;
    v97 = v72;
    v115.x = v67 * v112;
    v115.y = 0.0f * v112;
    v115.z = v72 * v112;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12208, &v115);
    X_4c = v102->f_last_valid_water_Z + 0.2f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(12003, X_4c);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12104, 2);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12109, v59);
    v48.x = -v103;
    v48.y = 0.0;
    v48.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12200, &v48);
    v70.x = -2.0f;
    v70.y = 0.0;
    v70.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12201, &v70);
    v44.x = -0.5f;
    v44.y = 0.0;
    v44.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12208, &v44);
    v65.x = 0.0;
    v65.y = 0.0;
    v65.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12207, &v65);
    X_4d = v102->f_last_valid_water_Z + 0.2f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(12003, X_4d);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12104, 3);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12109, v38);
    v25.x = v103;
    v25.y = 0.0f;
    v25.z = 0.0f;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12200, &v25);
    v94.x = 2.0f;
    v94.y = 0.0f;
    v94.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12201, &v94);
    v26.x = 0.5f;
    v26.y = 0.0;
    v26.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12208, &v26);
    v78.x = 0.0;
    v78.y = 0.0;
    v78.z = 0.0;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12207, &v78);
    X_4e = v102->f_last_valid_water_Z + 0.2f;
    AI_EvalFunc_GFXSetf_C_CURGLOB(12003, X_4e);
    AI_EvalFunc_GFXSetv_C_CURGLOB(12203, &v85);
    AI_EvalFunc_GFXSetv_C_CURGLOB(12204, &v41);
    AI_EvalFunc_GFXSetv_C_CURGLOB(12205, &v56);
    AI_EvalFunc_GFXSeti_C_CURGLOB(12103, 1);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v40);
    v68 = v40;
    v68.z = v102->f_last_valid_water_Z + 0.2f;
    AI_EvalFunc_GFXSetv_C_CURGLOB(12206, &v68);
    return 1000;
}

int torpille_tor_exec_test_collision()
{
    double v0; // st7
    BOOL v1; // eax
    BOOL v3; // eax
    BOOL v4; // eax
    tGameObjectMain* v5; // eax
    BOOL v6; // eax
    BOOL v7; // eax
    tScriptStruct_PNJTorpille* v8; // [esp+0h] [ebp-24Ch]
    tGameObjectMain* v9; // [esp+8h] [ebp-244h]
    tGameObjectMain* v10; // [esp+10h] [ebp-23Ch]
    tGameObjectMain* v11; // [esp+18h] [ebp-234h]
    tGameObjectMain* v12; // [esp+1Ch] [ebp-230h]
    tGameObjectMain* v13; // [esp+20h] [ebp-22Ch]
    tGameObjectMain* v14; // [esp+24h] [ebp-228h]
    tGameObjectMain* v15; // [esp+28h] [ebp-224h]
    tGameObjectMain* v16; // [esp+40h] [ebp-20Ch]
    tGameObjectMain* v17; // [esp+48h] [ebp-204h]
    tGameObjectMain* v18; // [esp+50h] [ebp-1FCh]
    tGameObjectMain* v19; // [esp+54h] [ebp-1F8h]
    tGameObjectMain* v20; // [esp+58h] [ebp-1F4h]
    tGameObjectMain* v21; // [esp+5Ch] [ebp-1F0h]
    tGameObjectMain* v22; // [esp+60h] [ebp-1ECh]
    tGameObjectMain* v23; // [esp+68h] [ebp-1E4h]
    tGameObjectMain* v24; // [esp+70h] [ebp-1DCh]
    tGameObjectMain* v25; // [esp+78h] [ebp-1D4h]
    tGameObjectMain* v26; // [esp+80h] [ebp-1CCh]
    tGameObjectMain* v27; // [esp+88h] [ebp-1C4h]
    tGameObjectMain* v28; // [esp+8Ch] [ebp-1C0h]
    tGameObjectMain* v29; // [esp+90h] [ebp-1BCh]
    tGameObjectMain* v30; // [esp+94h] [ebp-1B8h]
    tGameObjectMain* v31; // [esp+98h] [ebp-1B4h]
    tGameObjectMain* v32; // [esp+9Ch] [ebp-1B0h]
    tGameObjectMain* v33; // [esp+A0h] [ebp-1ACh]
    tGameObjectMain* v34; // [esp+A4h] [ebp-1A8h]
    tGameObjectMain* v35; // [esp+A8h] [ebp-1A4h]
    tGameObjectMain* v36; // [esp+B0h] [ebp-19Ch]
    tGameObjectMain* v37; // [esp+B8h] [ebp-194h]
    tGameObjectMain* v38; // [esp+C0h] [ebp-18Ch]
    tGameObjectMain* v39; // [esp+C4h] [ebp-188h]
    tGameObjectMain* v40; // [esp+C8h] [ebp-184h]
    tGameObjectMain* v41; // [esp+CCh] [ebp-180h]
    tGameObjectMain* v42; // [esp+D0h] [ebp-17Ch]
    tGameObjectMain* v43; // [esp+E8h] [ebp-164h]
    tGameObjectMain* v44; // [esp+F0h] [ebp-15Ch]
    tGameObjectMain* v45; // [esp+F8h] [ebp-154h]
    tGameObjectMain* v46; // [esp+FCh] [ebp-150h]
    tGameObjectMain* v47; // [esp+100h] [ebp-14Ch]
    tGameObjectMain* v48; // [esp+104h] [ebp-148h]
    tGameObjectMain* v49; // [esp+108h] [ebp-144h]
    tGameObjectMain* v50; // [esp+110h] [ebp-13Ch]
    tGameObjectMain* v51; // [esp+118h] [ebp-134h]
    tGameObjectMain* v52; // [esp+120h] [ebp-12Ch]
    tGameObjectMain* v53; // [esp+128h] [ebp-124h]
    tGameObjectMain* v54; // [esp+130h] [ebp-11Ch]
    tGameObjectMain* v55; // [esp+134h] [ebp-118h]
    tGameObjectMain* v56; // [esp+138h] [ebp-114h]
    tGameObjectMain* v57; // [esp+13Ch] [ebp-110h]
    tGameObjectMain* v58; // [esp+140h] [ebp-10Ch]
    tGameObjectMain* v59; // [esp+144h] [ebp-108h]
    tGameObjectMain* v60; // [esp+148h] [ebp-104h]
    tGameObjectMain* v61; // [esp+14Ch] [ebp-100h]
    tGameObjectMain* v62; // [esp+150h] [ebp-FCh]
    tGameObjectMain* v63; // [esp+158h] [ebp-F4h]
    tGameObjectMain* v64; // [esp+160h] [ebp-ECh]
    tGameObjectMain* v65; // [esp+164h] [ebp-E8h]
    tGameObjectMain* v66; // [esp+168h] [ebp-E4h]
    tGameObjectMain* v67; // [esp+16Ch] [ebp-E0h]
    tGameObjectMain* v68; // [esp+170h] [ebp-DCh]
    float v69; // [esp+174h] [ebp-D8h]
    tGameObjectMain* v70; // [esp+17Ch] [ebp-D0h]
    tGameObjectMain* v71; // [esp+180h] [ebp-CCh]
    int v72; // [esp+188h] [ebp-C4h]
    int v73; // [esp+1A0h] [ebp-ACh]
    Vec3 v74; // [esp+1A4h] [ebp-A8h]
    Vec3 v75; // [esp+1A4h] [ebp-A8h]
    float v76; // [esp+1BCh] [ebp-90h]
    Vec3 v77; // [esp+1C0h] [ebp-8Ch] BYREF
    Vec3 v78; // [esp+1CCh] [ebp-80h]
    int v79; // [esp+1D8h] [ebp-74h]
    Vec3 v80; // [esp+1DCh] [ebp-70h] BYREF
    float v81; // [esp+1E8h] [ebp-64h]
    int v82; // [esp+1ECh] [ebp-60h]
    tGameObjectMain* v83; // [esp+1F0h] [ebp-5Ch]
    int v84; // [esp+1F4h] [ebp-58h]
    BOOL v85; // [esp+1F8h] [ebp-54h]
    BOOL v86; // [esp+1FCh] [ebp-50h]
    BOOL v87; // [esp+200h] [ebp-4Ch]
    int v88; // [esp+204h] [ebp-48h]
    int i; // [esp+208h] [ebp-44h]
    tGameObjectMain* v90; // [esp+210h] [ebp-3Ch]
    Vec3 v91; // [esp+218h] [ebp-34h] BYREF
    tScriptStruct_PNJTorpille* v92; // [esp+224h] [ebp-28h]
    int v93; // [esp+228h] [ebp-24h]
    int v94; // [esp+230h] [ebp-1Ch]
    tGameObjectMain* v95; // [esp+234h] [ebp-18h]
    tGameObjectMain* v96; // [esp+238h] [ebp-14h]
    Vec3 a3; // [esp+23Ch] [ebp-10h] BYREF
    tGameObjectMain* v98; // [esp+248h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v8 = (tScriptStruct_PNJTorpille*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v8 = 0;
    v92 = v8;
    i = 0;
    v82 = 0;
    v93 = 0;
    v90 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_torpille_tor_exec_test_collision_torpille_tor_exec_send_paf = (int)AI_ul_RealResolveOneRefFunction(0x72001202u);
        fct_torpille_tor_exec_test_collision_torpille_tor_mode_air = (int)AI_ul_RealResolveOneRefFunction(0x72000722u);
        fct_torpille_tor_exec_test_collision_torpille_tor_mode_mer = (int)AI_ul_RealResolveOneRefFunction(0x72000680u);
        fct_torpille_tor_exec_test_collision_torpille_tor_mode_explosion = (int)AI_ul_RealResolveOneRefFunction(0x7200069Bu);
        fctvar_torpille_tor_exec_test_collision_camera = AI_ul_RealResolveOneRef(42, 0x7100108Bu);
        fctvar_torpille_tor_exec_test_collision_torpille = AI_ul_RealResolveOneRef(42, 0x72000682u);
    }
    v90 = *AI_MainActor;
    v92->i_flag_activation = 1;
    if (v92->des_seb_mode)
    {
        v68 = (tGameObjectMain*)v92->des_o_target;
        if (v68)
        {
            if ((unsigned int)v68 <= 2)
                v68 = AI_MainActor[(uint32_t)v68 - 1];
            v67 = v68;
        }
        else
        {
            v67 = 0;
        }
        v66 = v90;
        if (v90)
        {
            if ((unsigned int)v90 <= 2)
                v66 = AI_MainActor[(uint32_t)v90 - 1];
            v65 = v66;
        }
        else
        {
            v65 = 0;
        }
        if (v67 == v65)
        {
            v64 = (tGameObjectMain*)v92->des_o_target;
            if (v64)
            {
                if ((unsigned int)v64 <= 2)
                    v64 = AI_MainActor[(uint32_t)v64 - 1];
                v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v64);
            }
            else
            {
                v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
            }
            v69 = (float)v0;
            if (v92->f_target_lock_sqr_dist < (double)v69)
            {
                v63 = v90;
                if (v90)
                {
                    if ((unsigned int)v90 <= 2)
                        v63 = AI_MainActor[(uint32_t)v90 - 1];
                    v1 = AI_EvalFunc_OBJ_CapaTest_C(v63, 0x2000);
                }
                else
                {
                    v1 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x2000);
                }
                v86 = v1;
                if (v1)
                {
                    v92->i_flag_activation = 0;
                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                    return 1000;
                }
            }
        }
    }
    v82 = 0;
    if (AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x80))
    {
        v92->o_collide_object = (int)AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 0x80);
        v62 = (tGameObjectMain*)v92->o_collide_object;
        if (v62)
        {
            if ((unsigned int)v62 <= 2)
                v62 = AI_MainActor[(uint32_t)v62 - 1];
            v61 = v62;
        }
        else
        {
            v61 = 0;
        }
        v60 = (tGameObjectMain*)v92->des_o_target;
        if (v60)
        {
            if ((unsigned int)v60 <= 2)
                v60 = AI_MainActor[(uint32_t)v60 - 1];
            v59 = v60;
        }
        else
        {
            v59 = 0;
        }
        if (v61 == v59)
        {
            if (v92->des_mine_infinie)
            {
                v79 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_exec_send_paf);
                if (v79 != 1000)
                    return v79;
            }
            if (!v92->des_mine_infinie)
                goto LABEL_318;
            if (v92->des_mine_infinie == 2)
            {
                v96 = *AI_MainActor;
                v58 = (tGameObjectMain*)v92->o_collide_object;
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
                v56 = v96;
                if (v96)
                {
                    if ((unsigned int)v96 <= 2)
                        v56 = AI_MainActor[(uint32_t)v96 - 1];
                    v55 = v56;
                }
                else
                {
                    v55 = 0;
                }
                if (v57 != v55)
                {
                LABEL_318:
                    if (v92->i_flag_mode_air)
                    {
                        v92->i_flag_mode_air = 0;
                        v54 = (tGameObjectMain*)v92->o_me;
                        if (v54)
                        {
                            if ((unsigned int)v54 <= 2)
                                v54 = AI_MainActor[(uint32_t)v54 - 1];
                            AI_EvalFunc_AICBDel_C(
                                AI_gpst_CurrentGameObject,
                                v54,
                                2,
                                (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_air);
                        }
                        else
                        {
                            AI_EvalFunc_AICBDel_C(
                                AI_gpst_CurrentGameObject,
                                0,
                                2,
                                (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_air);
                        }
                    }
                    if (v92->i_flag_mode_mer)
                    {
                        v92->i_flag_mode_mer = 0;
                        v53 = (tGameObjectMain*)v92->o_me;
                        if (v53)
                        {
                            if ((unsigned int)v53 <= 2)
                                v53 = AI_MainActor[(uint32_t)v53 - 1];
                            AI_EvalFunc_AICBDel_C(
                                AI_gpst_CurrentGameObject,
                                v53,
                                2,
                                (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_mer);
                        }
                        else
                        {
                            AI_EvalFunc_AICBDel_C(
                                AI_gpst_CurrentGameObject,
                                0,
                                2,
                                (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_mer);
                        }
                    }
                    if (!v92->i_flag_mode_explosion)
                    {
                        v92->i_flag_mode_explosion = 1;
                        v52 = (tGameObjectMain*)v92->o_me;
                        if (v52)
                        {
                            if ((unsigned int)v52 <= 2)
                                v52 = AI_MainActor[(uint32_t)v52 - 1];
                            AI_EvalFunc_AICBAdd_C(
                                AI_gpst_CurrentGameObject,
                                v52,
                                2,
                                (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_explosion);
                        }
                        else
                        {
                            AI_EvalFunc_AICBAdd_C(
                                AI_gpst_CurrentGameObject,
                                0,
                                2,
                                (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_explosion);
                        }
                    }
                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                    return 1000;
                }
            }
        }
        else if (v92->des_wall_collision)
        {
            v51 = (tGameObjectMain*)v92->o_collide_object;
            if (v51)
            {
                if ((unsigned int)v51 <= 2)
                    v51 = AI_MainActor[(uint32_t)v51 - 1];
                v3 = AI_EvalFunc_AIIsModel_C(v51, (tAIModel*)fctvar_torpille_tor_exec_test_collision_camera);
            }
            else
            {
                v3 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_torpille_tor_exec_test_collision_camera);
            }
            if (!v3)
            {
                if (v92->des_torpille_collision)
                    goto LABEL_319;
                v50 = (tGameObjectMain*)v92->o_collide_object;
                if (v50)
                {
                    if ((unsigned int)v50 <= 2)
                        v50 = AI_MainActor[(uint32_t)v50 - 1];
                    v4 = AI_EvalFunc_AIIsModel_C(v50, (tAIModel*)fctvar_torpille_tor_exec_test_collision_torpille);
                }
                else
                {
                    v4 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_torpille_tor_exec_test_collision_torpille);
                }
                v85 = v4;
                if (!v4)
                {
                LABEL_319:
                    if (v92->des_i_sound_id_nb)
                    {
                        v93 = AI_EvalFunc_COLGMSoundGet_C(AI_gpst_CurrentGameObject, 0x80);
                        for (i = 0; i < v92->des_i_sound_id_nb; ++i)
                        {
                            if (v92->des_i_sound_id[i] == v93)
                            {
                                if (v92->des_mine_infinie)
                                {
                                    v73 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_exec_send_paf);
                                    if (v73 != 1000)
                                        return v73;
                                }
                                if (!v92->des_mine_infinie)
                                    goto LABEL_320;
                                if (v92->des_mine_infinie == 2)
                                {
                                    v83 = *AI_MainActor;
                                    v49 = (tGameObjectMain*)v92->o_collide_object;
                                    if (v49)
                                    {
                                        if ((unsigned int)v49 <= 2)
                                            v49 = AI_MainActor[(uint32_t)v49 - 1];
                                        v48 = v49;
                                    }
                                    else
                                    {
                                        v48 = 0;
                                    }
                                    v47 = v83;
                                    if (v83)
                                    {
                                        if ((unsigned int)v83 <= 2)
                                            v47 = AI_MainActor[(uint32_t)v83 - 1];
                                        v46 = v47;
                                    }
                                    else
                                    {
                                        v46 = 0;
                                    }
                                    if (v48 != v46)
                                    {
                                    LABEL_320:
                                        if (v92->i_flag_mode_air)
                                        {
                                            v92->i_flag_mode_air = 0;
                                            v45 = (tGameObjectMain*)v92->o_me;
                                            if (v45)
                                            {
                                                if ((unsigned int)v45 <= 2)
                                                    v45 = AI_MainActor[(uint32_t)v45 - 1];
                                                AI_EvalFunc_AICBDel_C(
                                                    AI_gpst_CurrentGameObject,
                                                    v45,
                                                    2,
                                                    (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_air);
                                            }
                                            else
                                            {
                                                AI_EvalFunc_AICBDel_C(
                                                    AI_gpst_CurrentGameObject,
                                                    0,
                                                    2,
                                                    (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_air);
                                            }
                                        }
                                        if (v92->i_flag_mode_mer)
                                        {
                                            v92->i_flag_mode_mer = 0;
                                            v44 = (tGameObjectMain*)v92->o_me;
                                            if (v44)
                                            {
                                                if ((unsigned int)v44 <= 2)
                                                    v44 = AI_MainActor[(uint32_t)v44 - 1];
                                                AI_EvalFunc_AICBDel_C(
                                                    AI_gpst_CurrentGameObject,
                                                    v44,
                                                    2,
                                                    (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_mer);
                                            }
                                            else
                                            {
                                                AI_EvalFunc_AICBDel_C(
                                                    AI_gpst_CurrentGameObject,
                                                    0,
                                                    2,
                                                    (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_mer);
                                            }
                                        }
                                        if (!v92->i_flag_mode_explosion)
                                        {
                                            v92->i_flag_mode_explosion = 1;
                                            v43 = (tGameObjectMain*)v92->o_me;
                                            if (v43)
                                            {
                                                if ((unsigned int)v43 <= 2)
                                                    v43 = AI_MainActor[(uint32_t)v43 - 1];
                                                AI_EvalFunc_AICBAdd_C(
                                                    AI_gpst_CurrentGameObject,
                                                    v43,
                                                    2,
                                                    (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_explosion);
                                            }
                                            else
                                            {
                                                AI_EvalFunc_AICBAdd_C(
                                                    AI_gpst_CurrentGameObject,
                                                    0,
                                                    2,
                                                    (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_explosion);
                                            }
                                        }
                                        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                                        return 1000;
                                    }
                                }
                                break;
                            }
                        }
                    }
                    else
                    {
                        if (v92->des_mine_infinie)
                        {
                            v72 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_exec_send_paf);
                            if (v72 != 1000)
                                return v72;
                        }
                        if (v92->des_seb_mode)
                        {
                            AI_EvalFunc_COLNormalGet_C(AI_gpst_CurrentGameObject, 128, &a3);
                            v74 = a3;
                            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v80);
                            v76 = -v74.z * v80.z
                                + -v74.y * v80.y
                                + v80.x * -v74.x;
                            if (v76 > 0.5)
                                v82 = 1;
                        }
                        else
                        {
                            if (!v92->des_mine_infinie)
                                goto LABEL_152;
                            if (v92->des_mine_infinie == 2)
                            {
                                v70 = *AI_MainActor;
                                v42 = (tGameObjectMain*)v92->o_collide_object;
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
                                v40 = *AI_MainActor;
                                if (v70)
                                {
                                    if ((unsigned int)v70 <= 2)
                                        v40 = AI_MainActor[(uint32_t)v70 - 1];
                                    v39 = v40;
                                }
                                else
                                {
                                    v39 = 0;
                                }
                                if (v41 != v39)
                                    LABEL_152:
                                v82 = 1;
                            }
                        }
                        if (v82)
                        {
                            if (v92->i_flag_mode_air)
                            {
                                v92->i_flag_mode_air = 0;
                                v38 = (tGameObjectMain*)v92->o_me;
                                if (v38)
                                {
                                    if ((unsigned int)v38 <= 2)
                                        v38 = AI_MainActor[(uint32_t)v38 - 1];
                                    AI_EvalFunc_AICBDel_C(
                                        AI_gpst_CurrentGameObject,
                                        v38,
                                        2,
                                        (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_air);
                                }
                                else
                                {
                                    AI_EvalFunc_AICBDel_C(
                                        AI_gpst_CurrentGameObject,
                                        0,
                                        2,
                                        (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_air);
                                }
                            }
                            if (v92->i_flag_mode_mer)
                            {
                                v92->i_flag_mode_mer = 0;
                                v37 = (tGameObjectMain*)v92->o_me;
                                if (v37)
                                {
                                    if ((unsigned int)v37 <= 2)
                                        v37 = AI_MainActor[(uint32_t)v37 - 1];
                                    AI_EvalFunc_AICBDel_C(
                                        AI_gpst_CurrentGameObject,
                                        v37,
                                        2,
                                        (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_mer);
                                }
                                else
                                {
                                    AI_EvalFunc_AICBDel_C(
                                        AI_gpst_CurrentGameObject,
                                        0,
                                        2,
                                        (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_mer);
                                }
                            }
                            if (!v92->i_flag_mode_explosion)
                            {
                                v92->i_flag_mode_explosion = 1;
                                v36 = (tGameObjectMain*)v92->o_me;
                                if (v36)
                                {
                                    if ((unsigned int)v36 <= 2)
                                        v36 = AI_MainActor[(uint32_t)v36 - 1];
                                    AI_EvalFunc_AICBAdd_C(
                                        AI_gpst_CurrentGameObject,
                                        v36,
                                        2,
                                        (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_explosion);
                                }
                                else
                                {
                                    AI_EvalFunc_AICBAdd_C(
                                        AI_gpst_CurrentGameObject,
                                        0,
                                        2,
                                        (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_explosion);
                                }
                            }
                            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                            return 1000;
                        }
                    }
                }
            }
        }
    }
    if (!AI_EvalFunc_COLCollideType_C(AI_gpst_CurrentGameObject, 0x40))
        goto LABEL_315;
    v5 = AI_EvalFunc_COLGaoGet_C(AI_gpst_CurrentGameObject, 0x40);
    v92->o_collide_object = (int)v5;
    v35 = (tGameObjectMain*)v92->o_collide_object;
    if (v35)
    {
        if ((unsigned int)v35 <= 2)
            v35 = AI_MainActor[(uint32_t)v35 - 1];
        v34 = v35;
    }
    else
    {
        v34 = 0;
    }
    v33 = (tGameObjectMain*)v92->des_o_target;
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
    if (v34 == v32)
    {
        if (v92->des_mine_infinie)
        {
            v84 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_exec_send_paf);
            if (v84 != 1000)
                return v84;
        }
        if (!v92->des_mine_infinie)
            goto LABEL_321;
        if (v92->des_mine_infinie == 2)
        {
            v98 = *AI_MainActor;
            v31 = (tGameObjectMain*)v92->o_collide_object;
            if (v31)
            {
                if ((unsigned int)v31 <= 2)
                    v31 = AI_MainActor[(uint32_t)v31 - 1];
                v30 = v31;
            }
            else
            {
                v30 = 0;
            }
            v29 = v98;
            if (v98)
            {
                if ((unsigned int)v98 <= 2)
                    v29 = AI_MainActor[(uint32_t)v98 - 1];
                v28 = v29;
            }
            else
            {
                v28 = 0;
            }
            if (v30 != v28)
            {
            LABEL_321:
                if (v92->i_flag_mode_air)
                {
                    v92->i_flag_mode_air = 0;
                    v27 = (tGameObjectMain*)v92->o_me;
                    if (v27)
                    {
                        if ((unsigned int)v27 <= 2)
                            v27 = AI_MainActor[(uint32_t)v27 - 1];
                        AI_EvalFunc_AICBDel_C(
                            AI_gpst_CurrentGameObject,
                            v27,
                            2,
                            (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_air);
                    }
                    else
                    {
                        AI_EvalFunc_AICBDel_C(
                            AI_gpst_CurrentGameObject,
                            0,
                            2,
                            (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_air);
                    }
                }
                if (v92->i_flag_mode_mer)
                {
                    v92->i_flag_mode_mer = 0;
                    v26 = (tGameObjectMain*)v92->o_me;
                    if (v26)
                    {
                        if ((unsigned int)v26 <= 2)
                            v26 = AI_MainActor[(uint32_t)v26 - 1];
                        AI_EvalFunc_AICBDel_C(
                            AI_gpst_CurrentGameObject,
                            v26,
                            2,
                            (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_mer);
                    }
                    else
                    {
                        AI_EvalFunc_AICBDel_C(
                            AI_gpst_CurrentGameObject,
                            0,
                            2,
                            (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_mer);
                    }
                }
                if (!v92->i_flag_mode_explosion)
                {
                    v92->i_flag_mode_explosion = 1;
                    v25 = (tGameObjectMain*)v92->o_me;
                    if (v25)
                    {
                        if ((unsigned int)v25 <= 2)
                            v25 = AI_MainActor[(uint32_t)v25 - 1];
                        AI_EvalFunc_AICBAdd_C(
                            AI_gpst_CurrentGameObject,
                            v25,
                            2,
                            (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_explosion);
                    }
                    else
                    {
                        AI_EvalFunc_AICBAdd_C(
                            AI_gpst_CurrentGameObject,
                            0,
                            2,
                            (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_explosion);
                    }
                }
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                return 1000;
            }
        }
    LABEL_315:
        v92->o_collide_object = 0;
        return 1000;
    }
    if (!v92->des_ground_collision)
        goto LABEL_315;
    v24 = (tGameObjectMain*)v92->o_collide_object;
    if (v24)
    {
        if ((unsigned int)v24 <= 2)
            v24 = AI_MainActor[(uint32_t)v24 - 1];
        v6 = AI_EvalFunc_AIIsModel_C(v24, (tAIModel*)fctvar_torpille_tor_exec_test_collision_camera);
    }
    else
    {
        v6 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_torpille_tor_exec_test_collision_camera);
    }
    v87 = v6;
    if (v6)
        goto LABEL_315;
    if (!v92->des_torpille_collision)
    {
        v23 = (tGameObjectMain*)v92->o_collide_object;
        if (v23)
        {
            if ((unsigned int)v23 <= 2)
                v23 = AI_MainActor[(uint32_t)v23 - 1];
            v7 = AI_EvalFunc_AIIsModel_C(v23, (tAIModel*)fctvar_torpille_tor_exec_test_collision_torpille);
        }
        else
        {
            v7 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_torpille_tor_exec_test_collision_torpille);
        }
        if (v7)
            goto LABEL_315;
    }
    if (v92->des_i_sound_id_nb)
    {
        v93 = AI_EvalFunc_COLGMSoundGet_C(AI_gpst_CurrentGameObject, 64);
        for (i = 0; i < v92->des_i_sound_id_nb; ++i)
        {
            if (v92->des_i_sound_id[i] == v93)
            {
                if (v92->des_mine_infinie)
                {
                    v88 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_exec_send_paf);
                    if (v88 != 1000)
                        return v88;
                }
                if (!v92->des_mine_infinie)
                    goto LABEL_322;
                if (v92->des_mine_infinie == 2)
                {
                    v71 = *AI_MainActor;
                    v22 = (tGameObjectMain*)v92->o_collide_object;
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
                    v20 = *AI_MainActor;
                    if (v71)
                    {
                        if ((unsigned int)v71 <= 2)
                            v20 = AI_MainActor[(uint32_t)v71 - 1];
                        v19 = v20;
                    }
                    else
                    {
                        v19 = 0;
                    }
                    if (v21 != v19)
                    {
                    LABEL_322:
                        if (v92->i_flag_mode_air)
                        {
                            v92->i_flag_mode_air = 0;
                            v18 = (tGameObjectMain*)v92->o_me;
                            if (v18)
                            {
                                if ((unsigned int)v18 <= 2)
                                    v18 = AI_MainActor[(uint32_t)v18 - 1];
                                AI_EvalFunc_AICBDel_C(
                                    AI_gpst_CurrentGameObject,
                                    v18,
                                    2,
                                    (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_air);
                            }
                            else
                            {
                                AI_EvalFunc_AICBDel_C(
                                    AI_gpst_CurrentGameObject,
                                    0,
                                    2,
                                    (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_air);
                            }
                        }
                        if (v92->i_flag_mode_mer)
                        {
                            v92->i_flag_mode_mer = 0;
                            v17 = (tGameObjectMain*)v92->o_me;
                            if (v17)
                            {
                                if ((unsigned int)v17 <= 2)
                                    v17 = AI_MainActor[(uint32_t)v17 - 1];
                                AI_EvalFunc_AICBDel_C(
                                    AI_gpst_CurrentGameObject,
                                    v17,
                                    2,
                                    (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_mer);
                            }
                            else
                            {
                                AI_EvalFunc_AICBDel_C(
                                    AI_gpst_CurrentGameObject,
                                    0,
                                    2,
                                    (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_mer);
                            }
                        }
                        if (!v92->i_flag_mode_explosion)
                        {
                            v92->i_flag_mode_explosion = 1;
                            v16 = (tGameObjectMain*)v92->o_me;
                            if (v16)
                            {
                                if ((unsigned int)v16 <= 2)
                                    v16 = AI_MainActor[(uint32_t)v16 - 1];
                                AI_EvalFunc_AICBAdd_C(
                                    AI_gpst_CurrentGameObject,
                                    v16,
                                    2,
                                    (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_explosion);
                            }
                            else
                            {
                                AI_EvalFunc_AICBAdd_C(
                                    AI_gpst_CurrentGameObject,
                                    0,
                                    2,
                                    (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_explosion);
                            }
                        }
                        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                        return 1000;
                    }
                }
                goto LABEL_315;
            }
        }
        goto LABEL_315;
    }
    if (v92->des_mine_infinie)
    {
        v94 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_exec_send_paf);
        if (v94 != 1000)
            return v94;
    }
    if (v92->des_seb_mode)
    {
        AI_EvalFunc_COLNormalGet_C(AI_gpst_CurrentGameObject, 64, &v77);
        v75 = v77;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v91);
        v78.x = -v75.x;
        v78.y = -v75.y;
        v78.z = -v75.z;
        v81 = -v75.z * v91.z
            + -v75.y * v91.y
            + v91.x * -v75.x;
        if (v81 > 0.5)
            v82 = 1;
    }
    else
    {
        if (!v92->des_mine_infinie)
            goto LABEL_294;
        if (v92->des_mine_infinie == 2)
        {
            v95 = *AI_MainActor;
            v15 = (tGameObjectMain*)v92->o_collide_object;
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
            v13 = v95;
            if (v95)
            {
                if ((unsigned int)v95 <= 2)
                    v13 = AI_MainActor[(uint32_t)v95 - 1];
                v12 = v13;
            }
            else
            {
                v12 = 0;
            }
            if (v14 != v12)
                LABEL_294:
            v82 = 1;
        }
    }
    if (!v82)
        goto LABEL_315;
    if (v92->i_flag_mode_air)
    {
        v92->i_flag_mode_air = 0;
        v11 = (tGameObjectMain*)v92->o_me;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v11 = AI_MainActor[(uint32_t)v11 - 1];
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                v11,
                2,
                (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_air);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_air);
        }
    }
    if (v92->i_flag_mode_mer)
    {
        v92->i_flag_mode_mer = 0;
        v10 = (tGameObjectMain*)v92->o_me;
        if (v10)
        {
            if ((unsigned int)v10 <= 2)
                v10 = AI_MainActor[(uint32_t)v10 - 1];
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                v10,
                2,
                (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_mer);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_mer);
        }
    }
    if (!v92->i_flag_mode_explosion)
    {
        v92->i_flag_mode_explosion = 1;
        v9 = (tGameObjectMain*)v92->o_me;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v9,
                2,
                (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_explosion);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_torpille_tor_exec_test_collision_torpille_tor_mode_explosion);
        }
    }
    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
    return 1000;
}

int torpille_tor_init()
{
    int result; // eax
    int v1; // eax
    int v2; // eax
    int v3; // eax
    int v4; // eax
    tScriptStruct_PNJTorpille* v5; // [esp+4h] [ebp-190h]
    tGameObjectMain* v6; // [esp+18h] [ebp-17Ch]
    tGameObjectMain* v7; // [esp+1Ch] [ebp-178h]
    tGameObjectMain* v8; // [esp+24h] [ebp-170h]
    tGameObjectMain* v9; // [esp+2Ch] [ebp-168h]
    tGameObjectMain* a3; // [esp+40h] [ebp-154h]
    tGameObjectMain* v11; // [esp+44h] [ebp-150h]
    tGameObjectMain* v12; // [esp+4Ch] [ebp-148h]
    tGameObjectMain* v13; // [esp+54h] [ebp-140h]
    tGameObjectMain* v14; // [esp+5Ch] [ebp-138h]
    tGameObjectMain* v15; // [esp+64h] [ebp-130h]
    tGameObjectMain* v16; // [esp+6Ch] [ebp-128h]
    tGameObjectMain* v17; // [esp+74h] [ebp-120h]
    tGameObjectMain* v18; // [esp+7Ch] [ebp-118h]
    tGameObjectMain* v19; // [esp+84h] [ebp-110h]
    tGameObjectMain* v20; // [esp+8Ch] [ebp-108h]
    tGameObjectMain* v21; // [esp+94h] [ebp-100h]
    Vec3* v22; // [esp+9Ch] [ebp-F8h]
    Vec3* v23; // [esp+A0h] [ebp-F4h]
    tGameObjectMain* v24; // [esp+A8h] [ebp-ECh]
    tGameObjectMain* v25; // [esp+ACh] [ebp-E8h]
    int v26; // [esp+BCh] [ebp-D8h]
    BOOL v27; // [esp+E8h] [ebp-ACh]
    int v28; // [esp+110h] [ebp-84h]
    BOOL v29; // [esp+118h] [ebp-7Ch]
    Vec3 v30; // [esp+128h] [ebp-6Ch] BYREF
    int v31; // [esp+148h] [ebp-4Ch]
    tScriptStruct_PNJTorpille* v32; // [esp+160h] [ebp-34h]
    int v33; // [esp+164h] [ebp-30h]
    BOOL v34; // [esp+168h] [ebp-2Ch]
    void* Src; // [esp+178h] [ebp-1Ch]
    int v36; // [esp+180h] [ebp-14h]
    BOOL v37; // [esp+184h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_PNJTorpille*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    v32 = v5;
    v31 = 0;
    v34 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        torpille_tor_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&torpille_tor_init_univVars_Flag_speciaux_sauve, (void*)torpille_tor_init_univVars, 4488);
        torpille_tor_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &torpille_tor_init_GST_Global_gao_i_map_just_reset,
            (void*)torpille_tor_init_GST_Global_gao,
            3300);
        fct_torpille_tor_init_torpille_tor_exec_visuel = (int)AI_ul_RealResolveOneRefFunction(0x72003A31u);
        fct_torpille_tor_init_torpille_tor_exec_before_display = (int)AI_ul_RealResolveOneRefFunction(0x72003ACAu);
        fct_torpille_tor_init_torpille_tor_exec_after_display = (int)AI_ul_RealResolveOneRefFunction(0x72003ACBu);
        AI2C_ResolveBuffer(&torpille_tor_init_GST_Global_gao_o_targets, (void*)torpille_tor_init_GST_Global_gao, 964);
        AI2C_ResolveBuffer(&torpille_tor_init_GST_Global_gao_i_targets_nb, (void*)torpille_tor_init_GST_Global_gao, 2572);
        AI2C_ResolveBuffer(
            &torpille_tor_init_GST_Global_gao_v_targets_offset,
            (void*)torpille_tor_init_GST_Global_gao,
            1368);
        fct_torpille_tor_init_torpille_tor_mode_mer = (int)AI_ul_RealResolveOneRefFunction(0x72000680u);
        AI2C_ResolveBuffer(&torpille_tor_init_univVars_world_ID, (void*)torpille_tor_init_univVars, 4);
        fct_torpille_tor_init_torpille_tor_reflex = (int)AI_ul_RealResolveOneRefFunction(0x7200071Fu);
    }
    v32->o_me = (int)AI_gpst_CurrentGameObject;
    if (v32->des_seb_mode)
    {
        v32->des_mode = 1;
        v32->des_propulsion = 0;
        v32->des_water_type = 0;
        v32->des_f_gravity = -80.0;
        v32->des_f_archimede = 70.0;
        v32->des_f_speed = 90.0;
        v32->des_f_rotation_speed = 15.0;
        v32->des_v_water_friction.x = 5.0;
        v32->des_v_water_friction.y = 1.5;
        v32->des_v_water_friction.z = 8.0;
        v32->des_v_air_friction.x = 5.0;
        v32->des_v_air_friction.y = 1.5;
        v32->des_v_air_friction.z = 4.0;
        v32->des_wall_collision = 1;
        v32->des_ground_collision = 0;
        v32->des_torpille_collision = 0;
        v32->des_lock_target_mode = 1;
        v32->des_f_target_lock_dist = 80.0;
        v32->des_angle_lock = 80.0;
        v32->des_puissance = 4;
        v32->des_visuel = 1;
    }
    v32->des_i_point_de_vie = 20;
    if (v32->des_SF_ID)
    {
        v34 = v32->des_SF_ID > 0
            && AI_EvalFunc_ARRBitTest_C((int*)(torpille_tor_init_univVars_Flag_speciaux_sauve + 4), v32->des_SF_ID);
        if (v34)
        {
            v26 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v26 != 1000)
                return v26;
        }
    }
    ++v32->des_puissance;
    v32->des_puissance += 0x20000;
    v32->des_puissance += 32;
    v32->f_target_lock_sqr_dist = v32->des_f_target_lock_dist * v32->des_f_target_lock_dist;
    v32->des_angle_lock = v32->des_angle_lock / 2.0f;
    v32->des_angle_lock = v32->des_angle_lock * 0.017453f;
    v32->des_angle_lock = cosf(v32->des_angle_lock);
    AI_EvalFunc_AIPrioritySet_C(AI_gpst_CurrentGameObject, 128);
    AI_EvalFunc_OBJ_FlagsIdentitySet_C(AI_gpst_CurrentGameObject, 0x8000, 0);
    AI_EvalFunc_COLCrossableSet_C(AI_gpst_CurrentGameObject, 1, 0);
    v37 = AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
    AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x1009004, 0);
    v30.x = 0.0;
    v30.y = 0.0;
    v30.z = v32->des_f_gravity;
    AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v30);
    Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v32->m_msg, Src, sizeof(v32->m_msg));
    if (v32->m_msg.msgID)
    {
        v25 = v32->m_msg.gao2;
        if (v25)
        {
            if ((unsigned int)v25 <= 2)
                v25 = AI_MainActor[(uint32_t)v25 - 1];
            v24 = v25;
        }
        else
        {
            v24 = 0;
        }
        if (v24)
            v32->des_o_target = (int)v32->m_msg.gao2;
        if (v32->m_msg.f2 != 0.0)
            v32->des_f_speed = v32->m_msg.f2;
        if (v32->m_msg.f1 != 0.0)
            v32->des_f_rotation_speed = v32->m_msg.f1;
        if (fabs(v32->m_msg.v1.x) != 0.0f)
        {
            v27 = 0;
        }
        else if (fabs(v32->m_msg.v1.y) != 0.0f)
        {
            v27 = 0;
        }
        else
        {
            v27 = (v32->m_msg.v1.z) == 0.0f;
        }
        if (!v27)
        {
            v22 = &v32->v_init_speed;
            v23 = &v32->m_msg.v1;
            v32->v_init_speed.x = v32->m_msg.v1.x;
            v22->y = v23->y;
            v22->z = v23->z;
        }
        if (fabs(v32->m_msg.v2.x) != 0.0f)
        {
            v29 = 0;
        }
        else if (fabs(v32->m_msg.v2.y) != 0.0f)
        {
            v29 = 0;
        }
        else
        {
            v29 = fabs(v32->m_msg.v2.z) == 0.0f;
        }
        if (!v29)
            AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v32->m_msg.v2);
        AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v32->v_init_speed);
        AI_EvalFunc_MSGClear_C(AI_gpst_CurrentGameObject);
    }
    v32->f_water_Z = -1.0e14f;
    if (*(uint32_t*)torpille_tor_init_GST_Global_gao_i_map_just_reset)
        v32->f_time_start = -1.0e14f;
    else
        v32->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
    v32->o_visuel = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0);
    v21 = (tGameObjectMain*)v32->o_me;
    if (v21)
    {
        if ((unsigned int)v21 <= 2)
            v21 = AI_MainActor[(uint32_t)v21 - 1];
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            v21,
            2,
            (tAIFunction*)fct_torpille_tor_init_torpille_tor_exec_visuel);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(
            AI_gpst_CurrentGameObject,
            0,
            2,
            (tAIFunction*)fct_torpille_tor_init_torpille_tor_exec_visuel);
    }
    if (v32->des_visuel == 1)
    {
        v32->f_color_angle = AI_EvalFunc_MATHRandFloat_C(0.0, 6.283185f);
        v20 = (tGameObjectMain*)v32->o_visuel;
        if (v20)
        {
            if ((unsigned int)v20 <= 2)
                v20 = AI_MainActor[(uint32_t)v20 - 1];
            v1 = AI_EvalFunc_MATGetConstant_C(v20, 1u);
        }
        else
        {
            v1 = AI_EvalFunc_MATGetConstant_C(0, 1u);
        }
        v32->c_color_pointe = v1;
        v19 = (tGameObjectMain*)v32->o_me;
        if (v19)
        {
            if ((unsigned int)v19 <= 2)
                v19 = AI_MainActor[(uint32_t)v19 - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v19,
                0,
                (tAIFunction*)fct_torpille_tor_init_torpille_tor_exec_before_display);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                0,
                (tAIFunction*)fct_torpille_tor_init_torpille_tor_exec_before_display);
        }
        v18 = (tGameObjectMain*)v32->o_me;
        if (v18)
        {
            if ((unsigned int)v18 <= 2)
                v18 = AI_MainActor[(uint32_t)v18 - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v18,
                1,
                (tAIFunction*)fct_torpille_tor_init_torpille_tor_exec_after_display);
            goto LABEL_96;
        }
    }
    else if (v32->des_visuel == 2)
    {
        v32->f_color_angle = AI_EvalFunc_MATHRandFloat_C(0.0, 6.283185f);
        v32->o_radar = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(1u);
        v17 = (tGameObjectMain*)v32->o_visuel;
        if (v17)
        {
            if ((unsigned int)v17 <= 2)
                v17 = AI_MainActor[(uint32_t)v17 - 1];
            v2 = AI_EvalFunc_MATGetConstant_C(v17, 2u);
        }
        else
        {
            v2 = AI_EvalFunc_MATGetConstant_C(0, 2u);
        }
        v32->c_color_pointe = v2;
        v16 = (tGameObjectMain*)v32->o_visuel;
        if (v16)
        {
            if ((unsigned int)v16 <= 2)
                v16 = AI_MainActor[(uint32_t)v16 - 1];
            v3 = AI_EvalFunc_MATGetConstant_C(v16, 3u);
        }
        else
        {
            v3 = AI_EvalFunc_MATGetConstant_C(0, 3u);
        }
        v32->c_color_oeil = v3;
        v15 = (tGameObjectMain*)v32->o_me;
        if (v15)
        {
            if ((unsigned int)v15 <= 2)
                v15 = AI_MainActor[(uint32_t)v15 - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v15,
                0,
                (tAIFunction*)fct_torpille_tor_init_torpille_tor_exec_before_display);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                0,
                (tAIFunction*)fct_torpille_tor_init_torpille_tor_exec_before_display);
        }
        v14 = (tGameObjectMain*)v32->o_me;
        if (v14)
        {
            if ((unsigned int)v14 <= 2)
                v14 = AI_MainActor[(uint32_t)v14 - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v14,
                1,
                (tAIFunction*)fct_torpille_tor_init_torpille_tor_exec_after_display);
            goto LABEL_96;
        }
    }
    else
    {
        v32->f_color_angle = AI_EvalFunc_MATHRandFloat_C(0.0, 6.283185f);
        v4 = AI_EvalFunc_MATGetConstant_C(AI_gpst_CurrentGameObject, 2u);
        v32->c_color_pointe = v4;
        v13 = (tGameObjectMain*)v32->o_me;
        if (v13)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v13,
                0,
                (tAIFunction*)fct_torpille_tor_init_torpille_tor_exec_before_display);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                0,
                (tAIFunction*)fct_torpille_tor_init_torpille_tor_exec_before_display);
        }
        v12 = (tGameObjectMain*)v32->o_me;
        if (v12)
        {
            if ((unsigned int)v12 <= 2)
                v12 = AI_MainActor[(uint32_t)v12 - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v12,
                1,
                (tAIFunction*)fct_torpille_tor_init_torpille_tor_exec_after_display);
            goto LABEL_96;
        }
    }
    AI_EvalFunc_AICBAdd_C(
        AI_gpst_CurrentGameObject,
        0,
        1,
        (tAIFunction*)fct_torpille_tor_init_torpille_tor_exec_after_display);
LABEL_96:
    if (v32->des_mode == 2)
    {
        v32->i_flag_mode_mer = 1;
        AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 8, 0);
        v11 = (tGameObjectMain*)v32->o_me;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v11 = AI_MainActor[(uint32_t)v11 - 1];
            a3 = v11;
        }
        else
        {
            a3 = 0;
        }
        v31 = AI_EvalFunc_ARRObjSearch_C(
            (tGameObjectMain**)(torpille_tor_init_GST_Global_gao_o_targets + 4),
            *(uint32_t*)torpille_tor_init_GST_Global_gao_i_targets_nb,
            a3);
        if (v31 == -1)
        {
            v31 = AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)(torpille_tor_init_GST_Global_gao_o_targets + 4), 100, 0);
            if (v31 != -1)
            {
                ++* (uint32_t*)torpille_tor_init_GST_Global_gao_i_targets_nb;
                *(uint32_t*)(torpille_tor_init_GST_Global_gao_o_targets + 4 * v31 + 4) = v32->o_me;
                *(uint32_t*)AI_EvalVarArray_C3((char*)torpille_tor_init_GST_Global_gao_v_targets_offset, v31) = 0;
                *(uint32_t*)(AI_EvalVarArray_C3((char*)torpille_tor_init_GST_Global_gao_v_targets_offset, v31) + 4) = 0;
                *(uint32_t*)(AI_EvalVarArray_C3((char*)torpille_tor_init_GST_Global_gao_v_targets_offset, v31) + 8) = 0;
            }
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v32->v_last_pos);
        AI_EvalFunc_DYNOn_C(AI_gpst_CurrentGameObject);
        v33 = AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 18644992, 0);
        v32->i_flag_mode_mer = 1;
        v9 = (tGameObjectMain*)v32->o_me;
        if (v9)
        {
            if ((unsigned int)v9 <= 2)
                v9 = AI_MainActor[(uint32_t)v9 - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v9,
                2,
                (tAIFunction*)fct_torpille_tor_init_torpille_tor_mode_mer);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(AI_gpst_CurrentGameObject, 0, 2, (tAIFunction*)fct_torpille_tor_init_torpille_tor_mode_mer);
        }
    }
    else if (v32->des_mode == 1)
    {
        v32->m_msg.msgID = 77;
        v32->m_msg.gao = (tGameObjectMain*)v32->o_me;
        v32->m_msg.gao2 = 0;
        v32->m_msg.Int1 = 1;
        v32->m_msg.Int2 = 0;
        v32->m_msg.f2 = 0.0;
        v32->m_msg.f1 = 0.0;
        v8 = (tGameObjectMain*)v32->des_o_target;
        if (v8)
        {
            if ((unsigned int)v8 <= 2)
                v8 = AI_MainActor[(uint32_t)v8 - 1];
            AI_EvalFunc_MSGSend_C(v8, &v32->m_msg);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &v32->m_msg);
        }
        AI_EvalFunc_DYNFlagsSet_C(AI_gpst_CurrentGameObject, 0x3028000, 0);
        AI_EvalFunc_DYNApplyRectFactorSet_C(AI_gpst_CurrentGameObject, 1.0);
        AI_EvalFunc_DYNReboundSet_C(AI_gpst_CurrentGameObject, 0.60000002f);
        v32->f_sound_freq_coef = AI_EvalFunc_MATHRandFloat_C(0.94999999f, 1.0);
        AI_EvalFunc_COLLODSet_C(AI_gpst_CurrentGameObject, 1, 3);
    }
    else
    {
        v36 = AI_EvalFunc_OBJ_FlagsDesignSet_C(AI_gpst_CurrentGameObject, 8, 0);
        v7 = (tGameObjectMain*)v32->o_me;
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
        v31 = AI_EvalFunc_ARRObjSearch_C(
            (tGameObjectMain**)(torpille_tor_init_GST_Global_gao_o_targets + 4),
            *(uint32_t*)torpille_tor_init_GST_Global_gao_i_targets_nb,
            v6);
        if (v31 == -1)
        {
            v31 = AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)(torpille_tor_init_GST_Global_gao_o_targets + 4), 100, 0);
            if (v31 != -1)
            {
                ++* (uint32_t*)torpille_tor_init_GST_Global_gao_i_targets_nb;
                *(uint32_t*)(torpille_tor_init_GST_Global_gao_o_targets + 4 * v31 + 4) = v32->o_me;
                *(uint32_t*)AI_EvalVarArray_C3((char*)torpille_tor_init_GST_Global_gao_v_targets_offset, v31) = 0;
                *(uint32_t*)(AI_EvalVarArray_C3((char*)torpille_tor_init_GST_Global_gao_v_targets_offset, v31) + 4) = 0;
                *(uint32_t*)(AI_EvalVarArray_C3((char*)torpille_tor_init_GST_Global_gao_v_targets_offset, v31) + 8) = 0;
            }
        }
        v32->f_angle = AI_EvalFunc_MATHRandFloat_C(0.0, 6.283185f);
        AI_EvalFunc_OBJRotateGlobalZ_C(AI_gpst_CurrentGameObject, v32->f_angle);
        AI_EvalFunc_COLLODSet_C(AI_gpst_CurrentGameObject, 1, 1);
        v32->f_oscillation_speed = AI_EvalFunc_MATHRandFloat_C(0.78539801f, 3.141593f);
        v32->f_rotation_speed = AI_EvalFunc_MATHRandFloat_C(0.2f, 0.60000002f);
        v32->f_sound_freq_coef = AI_EvalFunc_MATHRandFloat_C(0.89999998f, 1.05f);
        if (*(uint32_t*)torpille_tor_init_univVars_world_ID == 223 && !v32->des_visuel)
            v32->f_rotation_speed = v32->f_rotation_speed * 8.0f;
    }
    v28 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_torpille_tor_init_torpille_tor_reflex);
    if (v28 == 1000)
        result = 1000;
    else
        result = v28;
    return result;
}

int torpille_tor_reflex()
{
    BOOL v0; // eax
    double v1; // st7
    BOOL v2; // eax
    BOOL v3; // eax
    BOOL v4; // eax
    int result; // eax
    int v6; // eax
    double v7; // st7
    int v8; // eax
    int v9; // eax
    float v10; // [esp+8h] [ebp-2E8h]
    float v11; // [esp+Ch] [ebp-2E4h]
    float v12; // [esp+10h] [ebp-2E0h]
    float v13; // [esp+14h] [ebp-2DCh]
    float v14; // [esp+18h] [ebp-2D8h]
    float v15; // [esp+1Ch] [ebp-2D4h]
    float v16; // [esp+20h] [ebp-2D0h]
    tScriptStruct_PNJTorpille* v17; // [esp+28h] [ebp-2C8h]
    tGameObjectMain* v18; // [esp+30h] [ebp-2C0h]
    tGameObjectMain* v19; // [esp+38h] [ebp-2B8h]
    tGameObjectMain* v20; // [esp+40h] [ebp-2B0h]
    tGameObjectMain* v21; // [esp+48h] [ebp-2A8h]
    tGameObjectMain* v22; // [esp+50h] [ebp-2A0h]
    tGameObjectMain* v23; // [esp+58h] [ebp-298h]
    float v24; // [esp+74h] [ebp-27Ch]
    tGameObjectMain* v25; // [esp+7Ch] [ebp-274h]
    float v26; // [esp+80h] [ebp-270h]
    float v27; // [esp+84h] [ebp-26Ch]
    tGameObjectMain* v28; // [esp+94h] [ebp-25Ch]
    tGameObjectMain* v29; // [esp+98h] [ebp-258h]
    tGameObjectMain* v30; // [esp+A0h] [ebp-250h]
    tGameObjectMain* v31; // [esp+A8h] [ebp-248h]
    tGameObjectMain* v32; // [esp+B0h] [ebp-240h]
    tGameObjectMain* v33; // [esp+B8h] [ebp-238h]
    tGameObjectMain* v34; // [esp+BCh] [ebp-234h]
    tGameObjectMain* v35; // [esp+C0h] [ebp-230h]
    tGameObjectMain* v36; // [esp+C4h] [ebp-22Ch]
    tGameObjectMain* v37; // [esp+C8h] [ebp-228h]
    tGameObjectMain* v38; // [esp+D0h] [ebp-220h]
    tGameObjectMain* v39; // [esp+D8h] [ebp-218h]
    tGameObjectMain* v40; // [esp+E0h] [ebp-210h]
    tGameObjectMain* v41; // [esp+F0h] [ebp-200h]
    Vec3 v42; // [esp+F4h] [ebp-1FCh] BYREF
    Vec3 v43; // [esp+100h] [ebp-1F0h] BYREF
    Vec3 v44; // [esp+10Ch] [ebp-1E4h] BYREF
    int v45; // [esp+118h] [ebp-1D8h]
    int v46; // [esp+11Ch] [ebp-1D4h]
    BOOL v47; // [esp+120h] [ebp-1D0h]
    float v48; // [esp+124h] [ebp-1CCh]
    int v49; // [esp+128h] [ebp-1C8h]
    Vec3 a3; // [esp+12Ch] [ebp-1C4h] BYREF
    Vec3 v51; // [esp+138h] [ebp-1B8h]
    tGameObjectMain* v52; // [esp+144h] [ebp-1ACh]
    Vec3 v53; // [esp+148h] [ebp-1A8h] BYREF
    Vec3 v54; // [esp+154h] [ebp-19Ch] BYREF
    float v55; // [esp+160h] [ebp-190h]
    Vec3 v56[2]; // [esp+164h] [ebp-18Ch] BYREF
    unsigned int v57; // [esp+17Ch] [ebp-174h]
    int v58; // [esp+184h] [ebp-16Ch]
    float v59; // [esp+188h] [ebp-168h]
    float v60; // [esp+194h] [ebp-15Ch]
    Vec3 v61; // [esp+1A0h] [ebp-150h]
    float v62; // [esp+1ACh] [ebp-144h]
    int v63; // [esp+1B0h] [ebp-140h]
    Vec3 v64; // [esp+1B4h] [ebp-13Ch] BYREF
    void* Src; // [esp+1C0h] [ebp-130h]
    float v66; // [esp+1C8h] [ebp-128h]
    BOOL v67; // [esp+1CCh] [ebp-124h]
    Vec3 v68; // [esp+1D0h] [ebp-120h] BYREF
    float v69; // [esp+1DCh] [ebp-114h]
    int v70; // [esp+1E4h] [ebp-10Ch]
    Vec3 v71; // [esp+1E8h] [ebp-108h] BYREF
    Vec3 v72; // [esp+1F4h] [ebp-FCh] BYREF
    Vec3 a1; // [esp+200h] [ebp-F0h] BYREF
    Vec3 v74; // [esp+20Ch] [ebp-E4h] BYREF
    float v75; // [esp+21Ch] [ebp-D4h]
    float v76; // [esp+220h] [ebp-D0h]
    int v77; // [esp+224h] [ebp-CCh]
    int v78; // [esp+228h] [ebp-C8h]
    float v79; // [esp+238h] [ebp-B8h]
    void* v80; // [esp+23Ch] [ebp-B4h]
    Vec3 v81; // [esp+240h] [ebp-B0h] BYREF
    Vec3 a2; // [esp+24Ch] [ebp-A4h] BYREF
    BOOL v83; // [esp+25Ch] [ebp-94h]
    Vec3 v84; // [esp+260h] [ebp-90h] BYREF
    Vec3 v85; // [esp+26Ch] [ebp-84h] BYREF
    Vec3 v86; // [esp+278h] [ebp-78h] BYREF
    BOOL v87; // [esp+288h] [ebp-68h]
    tScriptStruct_PNJTorpille* v88; // [esp+28Ch] [ebp-64h]
    unsigned int v90; // [esp+294h] [ebp-5Ch]
    tGameObjectMain* v91; // [esp+298h] [ebp-58h]
    Vec3 dest; // [esp+29Ch] [ebp-54h] BYREF
    Vec3 v93; // [esp+2A8h] [ebp-48h] BYREF
    float v94; // [esp+2C8h] [ebp-28h]
    Vec3 v95; // [esp+2CCh] [ebp-24h] BYREF
    float v96; // [esp+2D8h] [ebp-18h]
    int v97; // [esp+2DCh] [ebp-14h]
    tGameObjectMain* v98; // [esp+2E0h] [ebp-10h]
    float v99; // [esp+2E4h] [ebp-Ch]
    float zoom; // [esp+2E8h] [ebp-8h]
    int v101; // [esp+2ECh] [ebp-4h]

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    v61.x = 1.0;
    v61.y = 0.0;
    v61.z = 0.0;
    v51.x = 1.0;
    v51.y = 0.0;
    v51.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v17 = (tScriptStruct_PNJTorpille*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v17 = 0;
    v88 = v17;
    v96 = 0.0;
    v58 = 0;
    v90 = 0;
    v77 = 0;
    zoom = 0.0;
    v91 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        torpille_tor_reflex_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &torpille_tor_reflex_GST_Global_gao_f_dist_mine,
            (void*)torpille_tor_reflex_GST_Global_gao,
            3612);
        fctvar_torpille_tor_reflex_torpille = AI_ul_RealResolveOneRef(42, 0x72000682u);
        fctvar_torpille_tor_reflex_Hovercraft_shoot = AI_ul_RealResolveOneRef(42, 0x49020714u);
        fct_torpille_tor_reflex_torpille_tor_exec_send_paf = (int)AI_ul_RealResolveOneRefFunction(0x72001202u);
        fct_torpille_tor_reflex_torpille_tor_mode_mer = (int)AI_ul_RealResolveOneRefFunction(0x72000680u);
        fct_torpille_tor_reflex_torpille_tor_mode_air = (int)AI_ul_RealResolveOneRefFunction(0x72000722u);
        fct_torpille_tor_reflex_torpille_tor_mode_explosion = (int)AI_ul_RealResolveOneRefFunction(0x7200069Bu);
        torpille_tor_reflex__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        torpille_tor_reflex_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)22)
    {
        v6 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        v63 = v6;
        if (v6 != 1000)
            return v63;
        goto LABEL_124;
    }
    if (*AI_gppst_CurrentJumpNode == (tAINode*)23)
    {
        v30 = (tGameObjectMain*)torpille_tor_reflex__PJ_Aircraft_gao;
        if (torpille_tor_reflex__PJ_Aircraft_gao)
        {
            if ((unsigned int)torpille_tor_reflex__PJ_Aircraft_gao <= 2)
                v30 = AI_MainActor[torpille_tor_reflex__PJ_Aircraft_gao - 1];
            v7 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v30);
        }
        else
        {
            v7 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
        }
        v62 = (float)v7;
        if (v62 < 225.0)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)23;
            return 0;
        }
        AI_EvalFunc_OBJReinit_C(AI_gpst_CurrentGameObject, 1);
        AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, 0.0);
        goto LABEL_124;
    }
    v91 = *AI_MainActor;
    v0 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 4);
    v87 = v0;
    if (v0)
        AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(&v88->v_last_speed);
    v52 = *AI_MainActor;
    v41 = v52;
    if (v52)
    {
        if ((unsigned int)v52 <= 2)
            v41 = AI_MainActor[(uint32_t)v52 - 1];
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v41);
    }
    else
    {
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v48 = (float)v1;
    v88->f_main_actor_dist = v48;
    if (!v88->des_mode)
    {
        v69 = TIM_gf_MainClock - TIM_gf_LockTime;
        zoom = v69 - v88->f_time_start;
        if (zoom > 1.0)
            v16 = 1.0;
        else
            v16 = zoom;
        zoom = v16;
        AI_EvalFunc_OBJ_ZoomSet_C(AI_gpst_CurrentGameObject, v16);
    }
    v57 = AI_EvalFunc_OBJ_FlagsControlGet_C(AI_gpst_CurrentGameObject);
    if ((v57 & 0x20000) == 0)
    {
        if (v88->des_visuel == 1)
        {
            v79 = TIM_gf_dt;
            v88->f_color_angle = 3.141593f * TIM_gf_dt + v88->f_color_angle;
            if (v88->f_main_actor_dist > 1600.0)
                v15 = 1600.0;
            else
                v15 = v88->f_main_actor_dist;
            v94 = v15;
            v60 = TIM_gf_dt;
            v88->f_color_angle = (1.0f - v15 / 1600.0f) * 6.283185f * TIM_gf_dt + v88->f_color_angle;
            if (v88->f_color_angle >= 6.283185)
                v88->f_color_angle = v88->f_color_angle - 6.283185f;
        }
        else if (v88->des_visuel == 2)
        {
            v75 = TIM_gf_dt;
            v88->f_color_angle = 3.141593f * TIM_gf_dt + v88->f_color_angle;
            if (v88->f_color_angle >= 6.283185)
                v88->f_color_angle = v88->f_color_angle - 6.283185f;
        }
        else
        {
            v55 = TIM_gf_dt;
            v88->f_color_angle = 3.141593f * TIM_gf_dt + v88->f_color_angle;
            if (v88->f_main_actor_dist > 1600.0)
                v14 = 1600.0;
            else
                v14 = v88->f_main_actor_dist;
            v99 = v14;
            v76 = TIM_gf_dt;
            v88->f_color_angle = (1.0f - v14 / 1600.0f) * 18.849556f * TIM_gf_dt + v88->f_color_angle;
            if (v88->f_color_angle >= 6.283185)
                v88->f_color_angle = v88->f_color_angle - 6.283185f;
        }
        v88->f_color_sin_angle = sinf(v88->f_color_angle);
        v88->f_color_sin_angle = v88->f_color_sin_angle + 1.0f;
        v88->f_color_sin_angle = v88->f_color_sin_angle / 2.0f;
    }
    if (v88->i_current_mode == 2)
    {
        if (!v88->i_flag_mode_explosion)
        {
            if (v88->des_mode || v88->des_i_force_suicide)
            {
                AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
                *AI_gppst_CurrentJumpNode = (tAINode*)22;
                TIME_Wait_C(0.5);
                result = 0;
            }
            else
            {
                *AI_gppst_CurrentJumpNode = (tAINode*)23;
                TIME_Wait_C(5.0);
                result = 0;
            }
            return result;
        }
    LABEL_124:
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    if (v88->des_mode == 1 && v88->f_main_actor_dist < (double)*(float*)torpille_tor_reflex_GST_Global_gao_f_dist_mine)
        *(float*)torpille_tor_reflex_GST_Global_gao_f_dist_mine = v88->f_main_actor_dist;
    Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
    memcpy(&v88->m_msg, Src, sizeof(v88->m_msg));
    while (v88->m_msg.msgID)
    {
        if (v88->m_msg.msgID == 37)
        {
            v58 = v88->m_msg.Int1 & 7;
            v90 = (unsigned int)v88->m_msg.gao;
            v40 = (tGameObjectMain*)v90;
            if (v90)
            {
                if (v90 <= 2)
                    v40 = AI_MainActor[v90 - 1];
                v2 = AI_EvalFunc_AIIsModel_C(v40, (tAIModel*)fctvar_torpille_tor_reflex_torpille);
            }
            else
            {
                v2 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_torpille_tor_reflex_torpille);
            }
            v47 = v2;
            if (v2)
            {
                if (v88->des_seb_mode)
                    v58 = -1;
                else
                    v88->i_flag_reaction_en_chaine = 1;
            }
            if (v88->des_visuel == 2)
            {
                v39 = (tGameObjectMain*)v90;
                if (v90)
                {
                    if (v90 <= 2)
                        v39 = AI_MainActor[v90 - 1];
                    v3 = AI_EvalFunc_AIIsModel_C(v39, (tAIModel*)fctvar_torpille_tor_reflex_Hovercraft_shoot);
                }
                else
                {
                    v3 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_torpille_tor_reflex_Hovercraft_shoot);
                }
                v67 = v3;
                if (v3)
                {
                    v38 = (tGameObjectMain*)v90;
                    if (v90)
                    {
                        if (v90 <= 2)
                            v38 = AI_MainActor[v90 - 1];
                        v4 = AI_EvalFunc_OBJ_CapaTest_C(v38, 4);
                    }
                    else
                    {
                        v4 = AI_EvalFunc_OBJ_CapaTest_C(0, 4);
                    }
                    v83 = v4;
                    if (v4)
                        v58 = -1;
                }
            }
            if (v58 > v88->des_paf_resitance)
            {
                if (v88->m_msg.f2 >= 1000.0)
                {
                    v88->i_flag_paf_send = 1;
                    AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
                }
                if (v88->des_mine_infinie)
                {
                    v78 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_torpille_tor_reflex_torpille_tor_exec_send_paf);
                    if (v78 != 1000)
                        return v78;
                }
                if (!v88->des_mine_infinie)
                    goto LABEL_225;
                if (v88->des_mine_infinie == 2)
                {
                    v98 = *AI_MainActor;
                    v37 = (tGameObjectMain*)v90;
                    if (v90)
                    {
                        if (v90 <= 2)
                            v37 = AI_MainActor[v90 - 1];
                        v36 = v37;
                    }
                    else
                    {
                        v36 = 0;
                    }
                    v35 = v98;
                    if (v98)
                    {
                        if ((unsigned int)v98 <= 2)
                            v35 = AI_MainActor[(uint32_t)v98 - 1];
                        v34 = v35;
                    }
                    else
                    {
                        v34 = 0;
                    }
                    if (v36 != v34)
                    {
                    LABEL_225:
                        if (v88->i_flag_mode_mer)
                        {
                            v88->i_flag_mode_mer = 0;
                            v33 = (tGameObjectMain*)v88->o_me;
                            if (v33)
                            {
                                if ((unsigned int)v33 <= 2)
                                    v33 = AI_MainActor[(uint32_t)v33 - 1];
                                AI_EvalFunc_AICBDel_C(
                                    AI_gpst_CurrentGameObject,
                                    v33,
                                    2,
                                    (tAIFunction*)fct_torpille_tor_reflex_torpille_tor_mode_mer);
                            }
                            else
                            {
                                AI_EvalFunc_AICBDel_C(
                                    AI_gpst_CurrentGameObject,
                                    0,
                                    2,
                                    (tAIFunction*)fct_torpille_tor_reflex_torpille_tor_mode_mer);
                            }
                        }
                        if (v88->i_flag_mode_air)
                        {
                            v88->i_flag_mode_air = 0;
                            v32 = (tGameObjectMain*)v88->o_me;
                            if (v32)
                            {
                                if ((unsigned int)v32 <= 2)
                                    v32 = AI_MainActor[(uint32_t)v32 - 1];
                                AI_EvalFunc_AICBDel_C(
                                    AI_gpst_CurrentGameObject,
                                    v32,
                                    2,
                                    (tAIFunction*)fct_torpille_tor_reflex_torpille_tor_mode_air);
                            }
                            else
                            {
                                AI_EvalFunc_AICBDel_C(
                                    AI_gpst_CurrentGameObject,
                                    0,
                                    2,
                                    (tAIFunction*)fct_torpille_tor_reflex_torpille_tor_mode_air);
                            }
                        }
                        if (!v88->i_flag_mode_explosion)
                        {
                            v88->i_flag_mode_explosion = 1;
                            v31 = (tGameObjectMain*)v88->o_me;
                            if (v31)
                            {
                                if ((unsigned int)v31 <= 2)
                                    v31 = AI_MainActor[(uint32_t)v31 - 1];
                                AI_EvalFunc_AICBAdd_C(
                                    AI_gpst_CurrentGameObject,
                                    v31,
                                    2,
                                    (tAIFunction*)fct_torpille_tor_reflex_torpille_tor_mode_explosion);
                            }
                            else
                            {
                                AI_EvalFunc_AICBAdd_C(
                                    AI_gpst_CurrentGameObject,
                                    0,
                                    2,
                                    (tAIFunction*)fct_torpille_tor_reflex_torpille_tor_mode_explosion);
                            }
                            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                            return 4;
                        }
                    }
                }
            }
        }
        v80 = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v88->m_msg, v80, sizeof(v88->m_msg));
    }
    if (v88->des_mode || !v88->des_mine_mode || v88->f_water_Z == -1.0e14)
    {
        if (v88->des_mode == 2)
        {
            v85.x = 0.0;
            v85.y = 0.0;
            v85.z = 0.0;
            v97 = AI_EvalFunc_DYNSpeedSetVector_C(AI_gpst_CurrentGameObject, &v85);
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 4;
        }
        else
        {
            if (v88->f_main_actor_dist >= 22500.0)
            {
                if (v88->i_sound_loop != -1)
                {
                    SND_Release(v88->i_sound_loop);
                    v88->i_sound_loop = -1;
                }
            }
            else if (v88->i_sound_loop == -1)
            {
                v88->i_sound_loop = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000000);
                AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v88->i_sound_loop);
                v70 = SND_l_GetFreq(v88->i_sound_loop);
                v10 = (float)v70;
                SND_SetFreq(v88->i_sound_loop, (unsigned __int64)(v88->f_sound_freq_coef * v10));
            }
            AI_EvalFunc_COLPosGet_C(AI_gpst_CurrentGameObject, 0, v56);
            v61 = v56[0];
            v96 = AI_EvalFunc_COLSizeGet_C(AI_gpst_CurrentGameObject, 0);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v42);
            AI_EvalFunc_WTR_GetDif_C(&v42, &v84);
            v51 = v84;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v44);
            if (v44.z <= (double)v51.z)
            {
                v88->i_flag_real_water = 1;
                v77 = 1;
            }
            else
            {
                v77 = AI_EvalFunc_COLGMatReportGet_C(AI_gpst_CurrentGameObject, 16);
                if (v77 != -1)
                {
                    AI_EvalFunc_COLPointGet_C(AI_gpst_CurrentGameObject, v77 + 0x80000000, &v54);
                    v51 = v54;
                }
            }
            if (v88->f_water_Z == -1.0e14 || v88->des_water_type)
            {
                if (v77 == -1)
                {
                    if (v88->f_water_Z - v96 > v61.z)
                    {
                        v88->i_flag_in_water = 1;
                    }
                    else
                    {
                        v88->i_flag_in_water = 0;
                        v88->f_water_Z = -1.0e14f;
                    }
                }
                else
                {
                    v88->f_water_Z = v51.z;
                    v88->f_last_valid_water_Z = v88->f_water_Z;
                    v88->i_flag_in_water = 1;
                    v88->i_flag_engine_on = 1;
                    if (!v88->des_mode)
                    {
                        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, 1);
                        if (v88->des_mine_mode)
                        {
                            v72.x = 0.0;
                            v72.y = 0.0;
                            v72.z = 0.0;
                            AI_EvalFunc_DYNGravitySet_C(AI_gpst_CurrentGameObject, &v72);
                        }
                        if (v88->i_flag_mode_air)
                        {
                            v88->i_flag_mode_air = 0;
                            v23 = (tGameObjectMain*)v88->o_me;
                            if (v23)
                            {
                                if ((unsigned int)v23 <= 2)
                                    v23 = AI_MainActor[(uint32_t)v23 - 1];
                                AI_EvalFunc_AICBDel_C(
                                    AI_gpst_CurrentGameObject,
                                    v23,
                                    2,
                                    (tAIFunction*)fct_torpille_tor_reflex_torpille_tor_mode_air);
                            }
                            else
                            {
                                AI_EvalFunc_AICBDel_C(
                                    AI_gpst_CurrentGameObject,
                                    0,
                                    2,
                                    (tAIFunction*)fct_torpille_tor_reflex_torpille_tor_mode_air);
                            }
                        }
                        if (!v88->i_flag_mode_mer)
                        {
                            v88->i_flag_mode_mer = 1;
                            v22 = (tGameObjectMain*)v88->o_me;
                            if (v22)
                            {
                                if ((unsigned int)v22 <= 2)
                                    v22 = AI_MainActor[(uint32_t)v22 - 1];
                                AI_EvalFunc_AICBAdd_C(
                                    AI_gpst_CurrentGameObject,
                                    v22,
                                    2,
                                    (tAIFunction*)fct_torpille_tor_reflex_torpille_tor_mode_mer);
                            }
                            else
                            {
                                AI_EvalFunc_AICBAdd_C(
                                    AI_gpst_CurrentGameObject,
                                    0,
                                    2,
                                    (tAIFunction*)fct_torpille_tor_reflex_torpille_tor_mode_mer);
                            }
                        }
                    }
                }
            }
            if (v88->i_flag_in_water)
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v43);
                v88->v_water_force.x = 0.0;
                v88->v_water_force.y = 0.0;
                v88->v_water_force.z = (v88->f_water_Z - v43.z) * v88->des_f_archimede;
                v45 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v88->des_v_water_friction);
                if (!v88->i_flag_mode_explosion)
                {
                    if (v88->i_flag_mode_air)
                    {
                        v88->i_flag_mode_air = 0;
                        v21 = (tGameObjectMain*)v88->o_me;
                        if (v21)
                        {
                            if ((unsigned int)v21 <= 2)
                                v21 = AI_MainActor[(uint32_t)v21 - 1];
                            AI_EvalFunc_AICBDel_C(
                                AI_gpst_CurrentGameObject,
                                v21,
                                2,
                                (tAIFunction*)fct_torpille_tor_reflex_torpille_tor_mode_air);
                        }
                        else
                        {
                            AI_EvalFunc_AICBDel_C(
                                AI_gpst_CurrentGameObject,
                                0,
                                2,
                                (tAIFunction*)fct_torpille_tor_reflex_torpille_tor_mode_air);
                        }
                    }
                    if (!v88->i_flag_mode_mer)
                    {
                        v88->i_flag_mode_mer = 1;
                        v20 = (tGameObjectMain*)v88->o_me;
                        if (v20)
                        {
                            if ((unsigned int)v20 <= 2)
                                v20 = AI_MainActor[(uint32_t)v20 - 1];
                            AI_EvalFunc_AICBAdd_C(
                                AI_gpst_CurrentGameObject,
                                v20,
                                2,
                                (tAIFunction*)fct_torpille_tor_reflex_torpille_tor_mode_mer);
                        }
                        else
                        {
                            AI_EvalFunc_AICBAdd_C(
                                AI_gpst_CurrentGameObject,
                                0,
                                2,
                                (tAIFunction*)fct_torpille_tor_reflex_torpille_tor_mode_mer);
                        }
                    }
                }
            }
            else if (v88->des_mode)
            {
                v88->v_water_force.x = 0.0;
                v88->v_water_force.y = 0.0;
                v88->v_water_force.z = 0.0;
                v49 = AI_EvalFunc_DYNFrictionVectorSet_C(AI_gpst_CurrentGameObject, &v88->des_v_air_friction);
                if (!v88->i_flag_mode_explosion)
                {
                    if (v88->i_flag_mode_mer)
                    {
                        v88->i_flag_mode_mer = 0;
                        v19 = (tGameObjectMain*)v88->o_me;
                        if (v19)
                        {
                            if ((unsigned int)v19 <= 2)
                                v19 = AI_MainActor[(uint32_t)v19 - 1];
                            AI_EvalFunc_AICBDel_C(
                                AI_gpst_CurrentGameObject,
                                v19,
                                2,
                                (tAIFunction*)fct_torpille_tor_reflex_torpille_tor_mode_mer);
                        }
                        else
                        {
                            AI_EvalFunc_AICBDel_C(
                                AI_gpst_CurrentGameObject,
                                0,
                                2,
                                (tAIFunction*)fct_torpille_tor_reflex_torpille_tor_mode_mer);
                        }
                    }
                    if (!v88->i_flag_mode_air)
                    {
                        v88->i_flag_mode_air = 1;
                        v18 = (tGameObjectMain*)v88->o_me;
                        if (v18)
                        {
                            if ((unsigned int)v18 <= 2)
                                v18 = AI_MainActor[(uint32_t)v18 - 1];
                            AI_EvalFunc_AICBAdd_C(
                                AI_gpst_CurrentGameObject,
                                v18,
                                2,
                                (tAIFunction*)fct_torpille_tor_reflex_torpille_tor_mode_air);
                        }
                        else
                        {
                            AI_EvalFunc_AICBAdd_C(
                                AI_gpst_CurrentGameObject,
                                0,
                                2,
                                (tAIFunction*)fct_torpille_tor_reflex_torpille_tor_mode_air);
                        }
                    }
                }
            }
            result = 1000;
        }
    }
    else
    {
        v29 = v91;
        if (v91)
        {
            if ((unsigned int)v91 <= 2)
                v29 = AI_MainActor[(uint32_t)v91 - 1];
            v28 = v29;
        }
        else
        {
            v28 = 0;
        }
        if (v28 == (tGameObjectMain*)torpille_tor_reflex__PJ_Aircraft_gao
            && v88->des_f_min_attraction * v88->des_f_min_attraction > v88->f_main_actor_dist)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
            AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)torpille_tor_reflex__PJ_Aircraft_gao, &a3);
            MATH_SubstractVec(&dest, &a2, &a3);
            a1 = dest;
            zoom = MATH_VecLen(&a1);
            v27 = 1.0f / zoom;
            a1.x = a1.x * v27;
            a1.y = a1.y * v27;
            a1.z = a1.z * v27;
            zoom = 1.0f - zoom / v88->des_f_min_attraction;
            if (AI_EvalFunc_OBJ_FlagsIdentityTest_C((tGameObjectMain*)torpille_tor_reflex__PJ_Aircraft_gao, 4))
            {
                AI_EvalFunc_DYNStreamVectorGet_C((tGameObjectMain*)torpille_tor_reflex__PJ_Aircraft_gao, &v68);
                v26 = v88->des_f_stream_force * zoom;
                v74.x = a1.x * v26;
                v74.y = a1.y * v26;
                v74.z = a1.z * v26;
                MATH_VecAddVec(&v93, &v68, &v74);
                AI_EvalFunc_DYNStreamSet_C((tGameObjectMain*)torpille_tor_reflex__PJ_Aircraft_gao, &v93);
            }
        }
        if (v88->f_main_actor_dist >= 1600.0)
        {
            if (v88->i_sound_loop != -1)
            {
                SND_Release(v88->i_sound_loop);
                v88->i_sound_loop = -1;
            }
        }
        else if (v88->i_sound_loop == -1)
        {
            v8 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000000);
            v88->i_sound_loop = v8;
            AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v88->i_sound_loop);
            v101 = SND_l_GetFreq(v88->i_sound_loop);
            v13 = (float)v101;
            SND_SetFreq(v88->i_sound_loop, (unsigned __int64)(v88->f_sound_freq_coef * v13));
        }
        if (v88->f_main_actor_dist >= 640.0)
        {
            if (v88->i_sound_loop_bis != -1)
            {
                SND_Release(v88->i_sound_loop_bis);
                v88->i_sound_loop_bis = -1;
            }
            if (v88->halo != -1)
            {
                v59 = TIM_gf_dt;
                v24 = 12.0f * TIM_gf_dt;
                if (v24 < 0.0)
                    v24 = 0.0;
                if (v24 > 1.0)
                    v24 = 1.0;
                v66 = 0.0f * v24 + v88->halo_size * (1.0f - v24);
                v88->halo_size = v66;
                GFXSetCurGlobal(v88->halo);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21000, v88->halo_size);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v95);
                v86.x = 0.0;
                v86.y = 0.0;
                v86.z = 2.5;
                MATH_VecAddVec(&v53, &v95, &v86);
                AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v53);
                if (v88->halo_size < 0.001)
                {
                    AI_EvalFunc_GFXDel_C_CURRENT(v88->halo);
                    v88->halo = -1;
                }
            }
        }
        else
        {
            if (v88->i_sound_loop_bis == -1)
            {
                v9 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 1, 0x18000000);
                v88->i_sound_loop_bis = v9;
                AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v88->i_sound_loop_bis);
                v46 = SND_l_GetFreq(v88->i_sound_loop_bis);
                v12 = (float)v46;
                SND_SetFreq(v88->i_sound_loop_bis, (unsigned __int64)(v88->f_sound_freq_coef * v12));
                if (v88->halo == -1)
                {
                    v88->halo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                    AI_EvalFunc_GFXFlagSet_C_CURRENT(v88->halo, 4, 1);
                    v25 = (tGameObjectMain*)torpille_tor_reflex_SFX_light_smoke_gao;
                    if (torpille_tor_reflex_SFX_light_smoke_gao)
                    {
                        if ((unsigned int)torpille_tor_reflex_SFX_light_smoke_gao <= 2)
                            v25 = AI_MainActor[torpille_tor_reflex_SFX_light_smoke_gao - 1];
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v88->halo, v25, 28);
                    }
                    else
                    {
                        AI_EvalFunc_GFXMaterialSet_C_CURRENT(v88->halo, 0, 28);
                    }
                    GFXSetCurGlobal(v88->halo);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 2.0);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
                    AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 255);
                    v88->halo_size = 0.0;
                }
            }
            zoom = AI_EvalFunc_OBJ_ZoomGet_C(AI_gpst_CurrentGameObject);
            v11 = (float)fabs(zoom * 7.5 * v88->f_color_sin_angle);
            v88->halo_size = v11;
            GFXSetCurGlobal(v88->halo);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, v88->halo_size);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v71);
            v64.x = 0.0;
            v64.y = 0.0;
            v64.z = 2.5f * zoom;
            MATH_VecAddVec(&v81, &v71, &v64);
            AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v81);
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 4;
    }
    return result;
}

int torpille_tor_exec_before_display()
{
    tScriptStruct_PNJTorpille* v1; // [esp+4h] [ebp-34h]
    tGameObjectMain* v2; // [esp+Ch] [ebp-2Ch]
    tGameObjectMain* v3; // [esp+14h] [ebp-24h]
    tGameObjectMain* v4; // [esp+1Ch] [ebp-1Ch]
    int a2; // [esp+34h] [ebp-4h]
    int a2a; // [esp+34h] [ebp-4h]
    int a2b; // [esp+34h] [ebp-4h]
    int a2c; // [esp+34h] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJTorpille*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (v1->des_visuel == 1)
    {
        a2 = AI_EvalFunc_COLORBlend_C(0xFF000000, 0xFF0000FF, v1->f_color_sin_angle);
        v4 = (tGameObjectMain*)v1->o_visuel;
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            AI_EvalFunc_MATSetConstant_C(v4, a2, 1);
        }
        else
        {
            AI_EvalFunc_MATSetConstant_C(0, a2, 1);
        }
    }
    else if (v1->des_visuel == 2)
    {
        a2a = AI_EvalFunc_COLORBlend_C(0xFF000080, 0xFF0000FF, v1->f_color_sin_angle);
        v3 = (tGameObjectMain*)v1->o_visuel;
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            AI_EvalFunc_MATSetConstant_C(v3, a2a, 2);
        }
        else
        {
            AI_EvalFunc_MATSetConstant_C(0, a2a, 2);
        }
        a2b = AI_EvalFunc_COLORBlend_C(0xFF0000FF, 0xFF000080, v1->f_color_sin_angle);
        v2 = (tGameObjectMain*)v1->o_visuel;
        if (v2)
        {
            if ((unsigned int)v2 <= 2)
                v2 = AI_MainActor[(uint32_t)v2 - 1];
            AI_EvalFunc_MATSetConstant_C(v2, a2b, 3);
        }
        else
        {
            AI_EvalFunc_MATSetConstant_C(0, a2b, 3);
        }
    }
    else
    {
        a2c = AI_EvalFunc_COLORBlend_C(0xFF000040, 0xFF0000FF, v1->f_color_sin_angle);
        AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, a2c, 1);
    }
    return 1000;
}

int torpille_tor_mode_explosion()
{
    unsigned int v0; // eax
    int v1; // eax
    tScriptStruct_PNJTorpille* v3; // [esp+Ch] [ebp-244h]
    tGameObjectMain* v4; // [esp+14h] [ebp-23Ch]
    uint32_t* v5; // [esp+18h] [ebp-238h]
    uint32_t* v6; // [esp+1Ch] [ebp-234h]
    tGameObjectMain* v7; // [esp+2Ch] [ebp-224h]
    tGameObjectMain* v8; // [esp+30h] [ebp-220h]
    uint32_t* v9; // [esp+34h] [ebp-21Ch]
    uint32_t* v10; // [esp+38h] [ebp-218h]
    tGameObjectMain* v11; // [esp+48h] [ebp-208h]
    tGameObjectMain* v12; // [esp+4Ch] [ebp-204h]
    tGameObjectMain* v13; // [esp+54h] [ebp-1FCh]
    tGameObjectMain* v14; // [esp+5Ch] [ebp-1F4h]
    tGameObjectMain* v15; // [esp+64h] [ebp-1ECh]
    tGameObjectMain* v16; // [esp+68h] [ebp-1E8h]
    tGameObjectMain* v17; // [esp+6Ch] [ebp-1E4h]
    tGameObjectMain* v18; // [esp+7Ch] [ebp-1D4h]
    tGameObjectMain* v19; // [esp+84h] [ebp-1CCh]
    tGameObjectMain* v20; // [esp+8Ch] [ebp-1C4h]
    tGameObjectMain* v21; // [esp+94h] [ebp-1BCh]
    tGameObjectMain* v22; // [esp+9Ch] [ebp-1B4h]
    tGameObjectMain* v23; // [esp+A4h] [ebp-1ACh]
    tGameObjectMain* v24; // [esp+B4h] [ebp-19Ch]
    tGameObjectMain* v25; // [esp+BCh] [ebp-194h]
    tGameObjectMain* v26; // [esp+C0h] [ebp-190h]
    tGameObjectMain* v27; // [esp+C4h] [ebp-18Ch]
    tGameObjectMain* v28; // [esp+CCh] [ebp-184h]
    uint32_t* v29; // [esp+D0h] [ebp-180h]
    uint32_t* v30; // [esp+D4h] [ebp-17Ch]
    tGameObjectMain* v31; // [esp+E4h] [ebp-16Ch]
    tGameObjectMain* v32; // [esp+E8h] [ebp-168h]
    uint32_t* v33; // [esp+ECh] [ebp-164h]
    uint32_t* v34; // [esp+F0h] [ebp-160h]
    tGameObjectMain* a3; // [esp+100h] [ebp-150h]
    tGameObjectMain* v36; // [esp+104h] [ebp-14Ch]
    Vec3 a2; // [esp+120h] [ebp-130h] BYREF
    tMessage Src; // [esp+130h] [ebp-120h] BYREF
    Vec3 v39; // [esp+164h] [ebp-ECh] BYREF
    int v40; // [esp+170h] [ebp-E0h]
    Vec3 v41; // [esp+174h] [ebp-DCh] BYREF
    Vec3 v43; // [esp+190h] [ebp-C0h] BYREF
    tGameObjectMain* v45; // [esp+1A0h] [ebp-B0h]
    Vec3 pos; // [esp+1B0h] [ebp-A0h] BYREF
    Vec3 v47; // [esp+1DCh] [ebp-74h] BYREF
    int v48; // [esp+1E8h] [ebp-68h]
    int v49; // [esp+1F0h] [ebp-60h]
    unsigned int v50; // [esp+200h] [ebp-50h]
    Vec3 v51; // [esp+208h] [ebp-48h] BYREF
    tScriptStruct_PNJTorpille* v52; // [esp+228h] [ebp-28h]
    int v53; // [esp+238h] [ebp-18h]
    tGameObjectMain* v54; // [esp+23Ch] [ebp-14h]
    Vec3 v55; // [esp+240h] [ebp-10h] BYREF

    a2.x = 1.0;
    a2.y = 0.0;
    a2.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_PNJTorpille*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v52 = v3;
    v48 = 0;
    v45 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        torpille_tor_mode_explosion_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &torpille_tor_mode_explosion_univVars_Flag_speciaux_sauve,
            (void*)torpille_tor_mode_explosion_univVars,
            4488);
        torpille_tor_mode_explosion_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &torpille_tor_mode_explosion_GST_Global_gao_o_targets,
            (void*)torpille_tor_mode_explosion_GST_Global_gao,
            964);
        AI2C_ResolveBuffer(
            &torpille_tor_mode_explosion_GST_Global_gao_i_targets_nb,
            (void*)torpille_tor_mode_explosion_GST_Global_gao,
            2572);
        AI2C_ResolveBuffer(
            &torpille_tor_mode_explosion_GST_Global_gao_v_targets_offset,
            (void*)torpille_tor_mode_explosion_GST_Global_gao,
            1368);
        AI2C_ResolveBuffer(
            &torpille_tor_mode_explosion_GST_Global_gao_o_selected_target,
            (void*)torpille_tor_mode_explosion_GST_Global_gao,
            2580);
        AI2C_ResolveBuffer(
            &torpille_tor_mode_explosion_GST_Global_gao_i_selected_target_nb,
            (void*)torpille_tor_mode_explosion_GST_Global_gao,
            2748);
        AI2C_ResolveBuffer(
            &torpille_tor_mode_explosion_GST_Global_gao_v_selected_target_offset,
            (void*)torpille_tor_mode_explosion_GST_Global_gao,
            2624);
        fct_torpille_tor_mode_explosion_torpille_tor_exec_visuel = (int)AI_ul_RealResolveOneRefFunction(0x72003A31u);
        fct_torpille_tor_mode_explosion_torpille_tor_exec_send_paf = (int)AI_ul_RealResolveOneRefFunction(0x72001202u);
        torpille_tor_mode_explosion_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
        fct_torpille_tor_mode_explosion_torpille_tor_mode_explosion = (int)AI_ul_RealResolveOneRefFunction(0x7200069Bu);
    }
    if (v52->i_current_mode != 2)
    {
        v52->i_current_mode = 2;
        AI_EvalFunc_GFXDel_C_CURRENT(v52->GFX_Airtrace);
        if (v52->GFX_Halo != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v52->GFX_Halo);
            v52->GFX_Halo = -1;
        }
        if (v52->halo != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v52->halo);
            v52->halo = -1;
        }
        if (v52->i_snd_rotor != -1)
        {
            SND_Release(v52->i_snd_rotor);
            v52->i_snd_rotor = -1;
        }
        if (v52->i_sound_loop != -1)
        {
            SND_Release(v52->i_sound_loop);
            v52->i_sound_loop = -1;
        }
        if (v52->i_sound_loop_bis != -1)
        {
            SND_Release(v52->i_sound_loop_bis);
            v52->i_sound_loop_bis = -1;
        }
        AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        v41.x = 0.0;
        v41.y = 0.0;
        v41.z = 0.0;
        AI_EvalFunc_DYNTractionSet_C_CURRENT(&v41);
        v52->f_time_change_mode = TIM_gf_MainClock - TIM_gf_LockTime;
        if (v52->des_visuel && v52->des_SF_ID > 0)
            AI_EvalFunc_ARRBitSet_C((int*)(torpille_tor_mode_explosion_univVars_Flag_speciaux_sauve + 4), v52->des_SF_ID);
        v36 = (tGameObjectMain*)v52->o_me;
        if (v36)
        {
            if ((unsigned int)v36 <= 2)
                v36 = AI_MainActor[(uint32_t)v36 - 1];
            a3 = v36;
        }
        else
        {
            a3 = 0;
        }
        v48 = AI_EvalFunc_ARRObjSearch_C(
            (tGameObjectMain**)(torpille_tor_mode_explosion_GST_Global_gao_o_targets + 4),
            100,
            a3);
        if (v48 != -1)
        {
            *(uint32_t*)(torpille_tor_mode_explosion_GST_Global_gao_o_targets + 4 * v48 + 4) = *(uint32_t*)(torpille_tor_mode_explosion_GST_Global_gao_o_targets
                + 4 * *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_targets_nb);
            v34 = (uint32_t*)AI_EvalVarArray_C3(
                (char*)torpille_tor_mode_explosion_GST_Global_gao_v_targets_offset,
                *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_targets_nb - 1);
            v33 = (uint32_t*)AI_EvalVarArray_C3((char*)torpille_tor_mode_explosion_GST_Global_gao_v_targets_offset, v48);
            *v33 = *v34;
            v33[1] = v34[1];
            v33[2] = v34[2];
            *(uint32_t*)(torpille_tor_mode_explosion_GST_Global_gao_o_targets
                + 4 * *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_targets_nb) = 0;
            *(uint32_t*)AI_EvalVarArray_C3(
                (char*)torpille_tor_mode_explosion_GST_Global_gao_v_targets_offset,
                *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_targets_nb - 1) = 0;
            *(uint32_t*)(AI_EvalVarArray_C3(
                (char*)torpille_tor_mode_explosion_GST_Global_gao_v_targets_offset,
                *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_targets_nb - 1)
                + 4) = 0;
            *(uint32_t*)(AI_EvalVarArray_C3(
                (char*)torpille_tor_mode_explosion_GST_Global_gao_v_targets_offset,
                *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_targets_nb - 1)
                + 8) = 0;
            --* (uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_targets_nb;
        }
        v32 = (tGameObjectMain*)v52->o_me;
        if (v32)
        {
            if ((unsigned int)v32 <= 2)
                v32 = AI_MainActor[(uint32_t)v32 - 1];
            v31 = v32;
        }
        else
        {
            v31 = 0;
        }
        v48 = AI_EvalFunc_ARRObjSearch_C(
            (tGameObjectMain**)(torpille_tor_mode_explosion_GST_Global_gao_o_selected_target + 4),
            10,
            v31);
        if (v48 != -1)
        {
            *(uint32_t*)(torpille_tor_mode_explosion_GST_Global_gao_o_selected_target + 4 * v48 + 4) = *(uint32_t*)(torpille_tor_mode_explosion_GST_Global_gao_o_selected_target + 4 * *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_selected_target_nb);
            v30 = (uint32_t*)AI_EvalVarArray_C3(
                (char*)torpille_tor_mode_explosion_GST_Global_gao_v_selected_target_offset,
                *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_selected_target_nb - 1);
            v29 = (uint32_t*)AI_EvalVarArray_C3(
                (char*)torpille_tor_mode_explosion_GST_Global_gao_v_selected_target_offset,
                v48);
            *v29 = *v30;
            v29[1] = v30[1];
            v29[2] = v30[2];
            *(uint32_t*)(torpille_tor_mode_explosion_GST_Global_gao_o_selected_target
                + 4 * *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_selected_target_nb) = 0;
            *(uint32_t*)AI_EvalVarArray_C3(
                (char*)torpille_tor_mode_explosion_GST_Global_gao_v_selected_target_offset,
                *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_selected_target_nb - 1) = 0;
            *(uint32_t*)(AI_EvalVarArray_C3(
                (char*)torpille_tor_mode_explosion_GST_Global_gao_v_selected_target_offset,
                *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_selected_target_nb - 1)
                + 4) = 0;
            *(uint32_t*)(AI_EvalVarArray_C3(
                (char*)torpille_tor_mode_explosion_GST_Global_gao_v_selected_target_offset,
                *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_selected_target_nb - 1)
                + 8) = 0;
            --* (uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_selected_target_nb;
        }
        if (v52->des_visuel)
        {
            v54 = AI_gpst_CurrentGameObject;
            v28 = AI_gpst_CurrentGameObject;
            if (AI_gpst_CurrentGameObject)
            {
                if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                    v28 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v28,
                    2,
                    (tAIFunction*)fct_torpille_tor_mode_explosion_torpille_tor_exec_visuel);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_torpille_tor_mode_explosion_torpille_tor_exec_visuel);
            }
        }
        if (v52->des_mode == 1)
            v40 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(1, -1.0);
    }
    v27 = (tGameObjectMain*)v52->o_collide_object;
    if (v27)
    {
        if ((unsigned int)v27 <= 2)
            v27 = AI_MainActor[(uint32_t)v27 - 1];
        v26 = v27;
    }
    else
    {
        v26 = 0;
    }
    if (v26)
    {
        v25 = (tGameObjectMain*)v52->o_collide_object;
        if (v25)
        {
            if ((unsigned int)v25 <= 2)
                v25 = AI_MainActor[(uint32_t)v25 - 1];
            v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(v25);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
        }
        v50 = v0;
        if ((v0 & 0x200000) != 0)
        {
            v52->o_collide_object = 0;
        }
        else
        {
            v24 = (tGameObjectMain*)v52->o_collide_object;
            if (v24)
            {
                if ((unsigned int)v24 <= 2)
                    v24 = AI_MainActor[(uint32_t)v24 - 1];
                v1 = AI_EvalFunc_OBJDrawMaskGet_C(v24);
            }
            else
            {
                v1 = AI_EvalFunc_OBJDrawMaskGet_C(0);
            }
            v53 = v1;
            if ((v1 & 8) == 0)
                v52->o_collide_object = 0;
        }
    }
    if (!v52->des_mode)
    {
        if (v52->i_flag_reaction_en_chaine)
        {
            if (!AI_EvalFunc_TIMEElapsed_C(v52->f_time_change_mode, 0.5))
            {
                *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                return 1000;
            }
        }
    }
    if (!v52->i_flag_paf_send)
    {
        v49 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_torpille_tor_mode_explosion_torpille_tor_exec_send_paf);
        if (v49 != 1000)
            return v49;
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        v52->i_flag_paf_send = 1;
    }
    if (!v52->des_mode)
    {
        if (v52->des_visuel == 2)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v39);
            a2 = v39;
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v55);
            a2.x = a2.x - v55.x;
            a2.y = a2.y - v55.y;
            a2.z = a2.z - v55.z;
            v45 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)torpille_tor_mode_explosion_SFX_explosion_gao, &a2);
            Src.msgID = 1;
            Src.Int1 = 37;
            Src.f2 = 4.5;
            Src.f1 = 0.0;
            Src.v1.x = 2.0;
            Src.v1.y = 0.0;
            Src.v1.z = 0.0;
            v23 = v45;
            if (v45)
            {
                if ((unsigned int)v45 <= 2)
                    v23 = AI_MainActor[(uint32_t)v45 - 1];
                AI_EvalFunc_MSGSend_C(v23, &Src);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &Src);
            }
            INO_PifSet(4);
            INO_PafSet(155, 10);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
            v45 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)torpille_tor_mode_explosion_SFX_explosion_gao, &pos);
            Src.msgID = 1;
            Src.Int1 = 20;
            Src.f2 = 10.0;
            Src.f1 = 1.0;
            Src.v1.x = 1.0;
            Src.v1.y = 0.0;
            Src.v1.z = 1.0;
            v22 = v45;
            if (v45)
            {
                if ((unsigned int)v45 <= 2)
                    v22 = AI_MainActor[(uint32_t)v45 - 1];
                AI_EvalFunc_MSGSend_C(v22, &Src);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &Src);
            }
        }
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        v52->i_flag_mode_explosion = 0;
        v21 = (tGameObjectMain*)v52->o_me;
        if (v21)
        {
            if ((unsigned int)v21 <= 2)
                v21 = AI_MainActor[(uint32_t)v21 - 1];
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                v21,
                2,
                (tAIFunction*)fct_torpille_tor_mode_explosion_torpille_tor_mode_explosion);
            return 1000;
        }
        goto LABEL_84;
    }
    if (v52->des_mode != 1)
    {
        if (v52->des_mode == 2)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v51);
            v45 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)torpille_tor_mode_explosion_SFX_explosion_gao, &v51);
            Src.msgID = 1;
            Src.Int1 = 22;
            Src.f2 = 20.0;
            Src.f1 = 1.0;
            Src.v1.x = 1.0;
            Src.v1.y = 0.0;
            Src.v1.z = 1.0;
            v13 = v45;
            if (v45)
            {
                if ((unsigned int)v45 <= 2)
                    v13 = AI_MainActor[(uint32_t)v45 - 1];
                AI_EvalFunc_MSGSend_C(v13, &Src);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &Src);
            }
            AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
            AI_EvalFunc_COLMapActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
            v12 = (tGameObjectMain*)v52->o_me;
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
            v48 = AI_EvalFunc_ARRObjSearch_C(
                (tGameObjectMain**)(torpille_tor_mode_explosion_GST_Global_gao_o_targets + 4),
                100,
                v11);
            if (v48 != -1)
            {
                *(uint32_t*)(torpille_tor_mode_explosion_GST_Global_gao_o_targets + 4 * v48 + 4) = *(uint32_t*)(torpille_tor_mode_explosion_GST_Global_gao_o_targets + 4 * *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_targets_nb);
                v10 = (uint32_t*)AI_EvalVarArray_C3(
                    (char*)torpille_tor_mode_explosion_GST_Global_gao_v_targets_offset,
                    *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_targets_nb - 1);
                v9 = (uint32_t*)AI_EvalVarArray_C3((char*)torpille_tor_mode_explosion_GST_Global_gao_v_targets_offset, v48);
                *v9 = *v10;
                v9[1] = v10[1];
                v9[2] = v10[2];
                *(uint32_t*)(torpille_tor_mode_explosion_GST_Global_gao_o_targets
                    + 4 * *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_targets_nb) = 0;
                *(uint32_t*)AI_EvalVarArray_C3(
                    (char*)torpille_tor_mode_explosion_GST_Global_gao_v_targets_offset,
                    *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_targets_nb - 1) = 0;
                *(uint32_t*)(AI_EvalVarArray_C3(
                    (char*)torpille_tor_mode_explosion_GST_Global_gao_v_targets_offset,
                    *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_targets_nb - 1)
                    + 4) = 0;
                *(uint32_t*)(AI_EvalVarArray_C3(
                    (char*)torpille_tor_mode_explosion_GST_Global_gao_v_targets_offset,
                    *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_targets_nb - 1)
                    + 8) = 0;
                --* (uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_targets_nb;
            }
            v8 = (tGameObjectMain*)v52->o_me;
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
            v48 = AI_EvalFunc_ARRObjSearch_C(
                (tGameObjectMain**)(torpille_tor_mode_explosion_GST_Global_gao_o_selected_target + 4),
                10,
                v7);
            if (v48 != -1)
            {
                *(uint32_t*)(torpille_tor_mode_explosion_GST_Global_gao_o_selected_target + 4 * v48 + 4) = *(uint32_t*)(torpille_tor_mode_explosion_GST_Global_gao_o_selected_target + 4 * *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_selected_target_nb);
                v6 = (uint32_t*)AI_EvalVarArray_C3(
                    (char*)torpille_tor_mode_explosion_GST_Global_gao_v_selected_target_offset,
                    *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_selected_target_nb - 1);
                v5 = (uint32_t*)AI_EvalVarArray_C3(
                    (char*)torpille_tor_mode_explosion_GST_Global_gao_v_selected_target_offset,
                    v48);
                *v5 = *v6;
                v5[1] = v6[1];
                v5[2] = v6[2];
                *(uint32_t*)(torpille_tor_mode_explosion_GST_Global_gao_o_selected_target
                    + 4 * *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_selected_target_nb) = 0;
                *(uint32_t*)AI_EvalVarArray_C3(
                    (char*)torpille_tor_mode_explosion_GST_Global_gao_v_selected_target_offset,
                    *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_selected_target_nb - 1) = 0;
                *(uint32_t*)(AI_EvalVarArray_C3(
                    (char*)torpille_tor_mode_explosion_GST_Global_gao_v_selected_target_offset,
                    *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_selected_target_nb - 1)
                    + 4) = 0;
                *(uint32_t*)(AI_EvalVarArray_C3(
                    (char*)torpille_tor_mode_explosion_GST_Global_gao_v_selected_target_offset,
                    *(uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_selected_target_nb - 1)
                    + 8) = 0;
                --* (uint32_t*)torpille_tor_mode_explosion_GST_Global_gao_i_selected_target_nb;
            }
            v52->i_flag_mode_explosion = 0;
            v4 = (tGameObjectMain*)v52->o_me;
            if (v4)
            {
                if ((unsigned int)v4 <= 2)
                    v4 = AI_MainActor[(uint32_t)v4 - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v4,
                    2,
                    (tAIFunction*)fct_torpille_tor_mode_explosion_torpille_tor_mode_explosion);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_torpille_tor_mode_explosion_torpille_tor_mode_explosion);
            }
        }
        return 1000;
    }
    if (v52->f_time_explosion == 0.0)
    {
        v52->f_time_explosion = TIM_gf_MainClock - TIM_gf_LockTime;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v47);
        v45 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)torpille_tor_mode_explosion_SFX_explosion_gao, &v47);
        Src.msgID = 1;
        Src.Int1 = 36;
        Src.f2 = 1.5;
        Src.f1 = 0.0;
        Src.v1.x = 1.0;
        Src.v1.y = 0.0;
        Src.v1.z = 0.0;
        v20 = v45;
        if (v45)
        {
            if ((unsigned int)v45 <= 2)
                v20 = AI_MainActor[(uint32_t)v45 - 1];
            AI_EvalFunc_MSGSend_C(v20, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
        v52->o_sfx_explosion = (int)v45;
        v19 = (tGameObjectMain*)v52->o_sfx_explosion;
        if (v19)
        {
            if ((unsigned int)v19 <= 2)
                v19 = AI_MainActor[(uint32_t)v19 - 1];
            AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, v19, &v52->o_sfx_explosion);
        }
        else
        {
            AI_EvalFunc_OBJ_RegisterKill_C(AI_gpst_CurrentGameObject, 0, &v52->o_sfx_explosion);
        }
        v52->m_msg.msgID = 77;
        v52->m_msg.gao = (tGameObjectMain*)v52->o_me;
        v52->m_msg.gao2 = 0;
        v52->m_msg.Int1 = -1;
        v52->m_msg.Int2 = 0;
        v52->m_msg.f2 = 0.0;
        v52->m_msg.f1 = 0.0;
        v18 = (tGameObjectMain*)v52->des_o_target;
        if (v18)
        {
            if ((unsigned int)v18 <= 2)
                v18 = AI_MainActor[(uint32_t)v18 - 1];
            AI_EvalFunc_MSGSend_C(v18, &v52->m_msg);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &v52->m_msg);
        }
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v43);
    a2 = v43;
    if (v52->f_water_Z + 1.0 > v43.z)
        a2.z = v52->f_water_Z + 1.0f;
    v17 = (tGameObjectMain*)v52->o_sfx_explosion;
    if (v17)
    {
        if ((unsigned int)v17 <= 2)
            v17 = AI_MainActor[(uint32_t)v17 - 1];
        v16 = v17;
    }
    else
    {
        v16 = 0;
    }
    if (v16)
    {
        v15 = (tGameObjectMain*)v52->o_sfx_explosion;
        if (v15)
        {
            if ((unsigned int)v15 <= 2)
                v15 = AI_MainActor[(uint32_t)v15 - 1];
            AI_EvalFunc_OBJSetPos_C(v15, &a2);
        }
        else
        {
            AI_EvalFunc_OBJSetPos_C(0, &a2);
        }
    }
    AI_EvalFunc_OBJSetPos_C_CURRENT(&a2);
    if (AI_EvalFunc_TIMEElapsed_C(v52->f_time_explosion, 1.0))
    {
        AI_EvalFunc_COLSetActivationSet_C(AI_gpst_CurrentGameObject, 0, -1);
        v52->i_flag_mode_explosion = 0;
        v14 = (tGameObjectMain*)v52->o_me;
        if (v14)
        {
            if ((unsigned int)v14 <= 2)
                v14 = AI_MainActor[(uint32_t)v14 - 1];
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                v14,
                2,
                (tAIFunction*)fct_torpille_tor_mode_explosion_torpille_tor_mode_explosion);
            return 1000;
        }
    LABEL_84:
        AI_EvalFunc_AICBDel_C(
            AI_gpst_CurrentGameObject,
            0,
            2,
            (tAIFunction*)fct_torpille_tor_mode_explosion_torpille_tor_mode_explosion);
    }
    return 1000;
}

int torpille_tor_exec_after_display()
{
    tScriptStruct_PNJTorpille* v1; // [esp+0h] [ebp-20h]
    tGameObjectMain* v2; // [esp+8h] [ebp-18h]
    tGameObjectMain* v3; // [esp+10h] [ebp-10h]
    tGameObjectMain* v4; // [esp+18h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJTorpille*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (v1->des_visuel == 1)
    {
        v4 = (tGameObjectMain*)v1->o_visuel;
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            AI_EvalFunc_MATSetConstant_C(v4, v1->c_color_pointe, 1);
        }
        else
        {
            AI_EvalFunc_MATSetConstant_C(0, v1->c_color_pointe, 1);
        }
    }
    else if (v1->des_visuel == 2)
    {
        v3 = (tGameObjectMain*)v1->o_visuel;
        if (v3)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            AI_EvalFunc_MATSetConstant_C(v3, v1->c_color_pointe, 2);
        }
        else
        {
            AI_EvalFunc_MATSetConstant_C(0, v1->c_color_pointe, 2);
        }
        v2 = (tGameObjectMain*)v1->o_visuel;
        if (v2)
        {
            if ((unsigned int)v2 <= 2)
                v2 = AI_MainActor[(uint32_t)v2 - 1];
            AI_EvalFunc_MATSetConstant_C(v2, v1->c_color_oeil, 3);
        }
        else
        {
            AI_EvalFunc_MATSetConstant_C(0, v1->c_color_oeil, 3);
        }
    }
    else
    {
        AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v1->c_color_pointe, 2);
    }
    return 1000;
}