#include "ai_compiledscripts_inc.h"

//Laser_Laser_exec_shot
int Laser_Laser_exec_shot_SFX_light_smoke_gao;
int Laser_Laser_exec_shot_GST_Global_gao;
int Laser_Laser_exec_shot_GST_Global_gao_f_eau_hauteurZ;
int Laser_Laser_exec_shot_SFX_explosion_gao;
int Laser_Laser_exec_shot_DUP_SFX_Sally_Shield_gao;

//Laser_Laser_init
int fct_Laser_Laser_init_Laser_Laser_init;
int fct_Laser_Laser_init_Laser_Laser_exec_shot;
int fct_Laser_Laser_init_Laser_Laser_comportement;

//Laser_Laser_comportement
int fct_Laser_Laser_comportement_Laser_Laser_comportement;
int fct_Laser_Laser_comportement_Laser_Laser_tempo_destroy;
int Laser_Laser_comportement_GST_Global_gao;
int Laser_Laser_comportement_GST_Global_gao_f_eau_hauteurZ;
int Laser_Laser_comportement_univVars;
int Laser_Laser_comportement_univVars_Perso_ID_actor;
int Laser_Laser_comportement_SFX_explosion_gao;
int Laser_Laser_comportement_SFX_light_smoke_gao;
int Laser_Laser_comportement_DUP_SFX_Sally_Shield_gao;

int Laser_Laser_exec_shot()
{
    unsigned int v0; // eax
    unsigned int v1; // eax
    unsigned int v2; // eax
    unsigned int v3; // eax
    int v4; // eax
    unsigned int v5; // eax
    unsigned int v6; // eax
    BOOL v7; // eax
    int v8; // eax
    BOOL v9; // eax
    BOOL v10; // eax
    float X_4; // [esp+10h] [ebp-4B0h]
    float X_4a; // [esp+10h] [ebp-4B0h]
    float X_4b; // [esp+10h] [ebp-4B0h]
    float X_4c; // [esp+10h] [ebp-4B0h]
    float X_4d; // [esp+10h] [ebp-4B0h]
    float X_4e; // [esp+10h] [ebp-4B0h]
    float X_4f; // [esp+10h] [ebp-4B0h]
    float v19; // [esp+14h] [ebp-4ACh]
    float v20; // [esp+18h] [ebp-4A8h]
    float v21; // [esp+1Ch] [ebp-4A4h]
    float v22; // [esp+20h] [ebp-4A0h]
    tScriptStruct_Laser* v23; // [esp+24h] [ebp-49Ch]
    float v24; // [esp+2Ch] [ebp-494h]
    float v25; // [esp+30h] [ebp-490h]
    tGameObjectMain* v26; // [esp+44h] [ebp-47Ch]
    tGameObjectMain* v27; // [esp+48h] [ebp-478h]
    tGameObjectMain* v28; // [esp+4Ch] [ebp-474h]
    tGameObjectMain* v29; // [esp+50h] [ebp-470h]
    tGameObjectMain* v30; // [esp+58h] [ebp-468h]
    float v31; // [esp+64h] [ebp-45Ch]
    tGameObjectMain* v32; // [esp+88h] [ebp-438h]
    tGameObjectMain* v33; // [esp+8Ch] [ebp-434h]
    tGameObjectMain* v34; // [esp+94h] [ebp-42Ch]
    tGameObjectMain* v35; // [esp+9Ch] [ebp-424h]
    tAI* v36; // [esp+A4h] [ebp-41Ch]
    tGameObjectMain* v37; // [esp+A8h] [ebp-418h]
    tGameObjectMain* v38; // [esp+ACh] [ebp-414h]
    tGameObjectMain* v39; // [esp+B4h] [ebp-40Ch]
    tGameObjectMain* v40; // [esp+BCh] [ebp-404h]
    tGameObjectMain* v41; // [esp+C4h] [ebp-3FCh]
    tGameObjectMain* v42; // [esp+CCh] [ebp-3F4h]
    tGameObjectMain* v43; // [esp+DCh] [ebp-3E4h]
    tGameObjectMain* v44; // [esp+FCh] [ebp-3C4h]
    tGameObjectMain* v45; // [esp+104h] [ebp-3BCh]
    Vec3* v46; // [esp+108h] [ebp-3B8h]
    Vec3* v47; // [esp+10Ch] [ebp-3B4h]
    tGameObjectMain* v48; // [esp+114h] [ebp-3ACh]
    Vec3* v49; // [esp+118h] [ebp-3A8h]
    Vec3* v50; // [esp+11Ch] [ebp-3A4h]
    tGameObjectMain* v51; // [esp+124h] [ebp-39Ch]
    tGameObjectMain* v52; // [esp+12Ch] [ebp-394h]
    tGameObjectMain* v53; // [esp+130h] [ebp-390h]
    tGameObjectMain* v54; // [esp+134h] [ebp-38Ch]
    tGameObjectMain* v55; // [esp+138h] [ebp-388h]
    tGameObjectMain* v56; // [esp+13Ch] [ebp-384h]
    float v57; // [esp+140h] [ebp-380h]
    float v58; // [esp+16Ch] [ebp-354h]
    tGameObjectMain* v59; // [esp+170h] [ebp-350h]
    tGameObjectMain* v60; // [esp+174h] [ebp-34Ch]
    tGameObjectMain* v61; // [esp+17Ch] [ebp-344h]
    tGameObjectMain* v62; // [esp+180h] [ebp-340h]
    tGameObjectMain* v63; // [esp+184h] [ebp-33Ch]
    tGameObjectMain* v64; // [esp+18Ch] [ebp-334h]
    tGameObjectMain* v65; // [esp+194h] [ebp-32Ch]
    tGameObjectMain* v66; // [esp+19Ch] [ebp-324h]
    tGameObjectMain* v67; // [esp+1A0h] [ebp-320h]
    tGameObjectMain* v68; // [esp+1A4h] [ebp-31Ch]
    tGameObjectMain* v69; // [esp+1ACh] [ebp-314h]
    tGameObjectMain* v70; // [esp+1B4h] [ebp-30Ch]
    tGameObjectMain* v71; // [esp+1BCh] [ebp-304h]
    tGameObjectMain* v72; // [esp+1C4h] [ebp-2FCh]
    tGameObjectMain* v73; // [esp+1CCh] [ebp-2F4h]
    tGameObjectMain* v74; // [esp+1D8h] [ebp-2E8h]
    tGameObjectMain* v75; // [esp+1E8h] [ebp-2D8h]
    tGameObjectMain* v76; // [esp+1ECh] [ebp-2D4h]
    tGameObjectMain* v77; // [esp+1F0h] [ebp-2D0h]
    Vec3 v78; // [esp+1F4h] [ebp-2CCh] BYREF
    Vec3 v79; // [esp+200h] [ebp-2C0h] BYREF
    Vec3 v80; // [esp+20Ch] [ebp-2B4h] BYREF
    Vec3 v81; // [esp+218h] [ebp-2A8h] BYREF
    Vec3 v82; // [esp+224h] [ebp-29Ch] BYREF
    float v83; // [esp+230h] [ebp-290h]
    tGameObjectMain* v84; // [esp+234h] [ebp-28Ch]
    float v85; // [esp+238h] [ebp-288h]
    Vec3 v86; // [esp+23Ch] [ebp-284h] BYREF
    float v87; // [esp+248h] [ebp-278h]
    float v88; // [esp+24Ch] [ebp-274h]
    float v89; // [esp+250h] [ebp-270h]
    float v90; // [esp+254h] [ebp-26Ch]
    unsigned int v91; // [esp+258h] [ebp-268h]
    tAI* v92; // [esp+25Ch] [ebp-264h]
    Vec3 v93; // [esp+260h] [ebp-260h]
    Vec3 v94; // [esp+26Ch] [ebp-254h] BYREF
    float v95; // [esp+278h] [ebp-248h]
    tMessage Src; // [esp+27Ch] [ebp-244h] BYREF
    int i; // [esp+2D8h] [ebp-1E8h]
    Vec3 v98; // [esp+2DCh] [ebp-1E4h] BYREF
    tGameObjectMain* v99; // [esp+2E8h] [ebp-1D8h]
    Vec3 v100; // [esp+2FCh] [ebp-1C4h] BYREF
    float v101; // [esp+308h] [ebp-1B8h]
    float v102; // [esp+30Ch] [ebp-1B4h]
    float v103; // [esp+310h] [ebp-1B0h]
    float v104; // [esp+314h] [ebp-1ACh]
    int v105; // [esp+31Ch] [ebp-1A4h]
    float v106; // [esp+320h] [ebp-1A0h]
    Vec3 v107; // [esp+340h] [ebp-180h] BYREF
    Vec3 v108; // [esp+34Ch] [ebp-174h]
    Vec3 pos; // [esp+358h] [ebp-168h] BYREF
    float v110; // [esp+364h] [ebp-15Ch]
    float v111; // [esp+368h] [ebp-158h]
    float v112; // [esp+36Ch] [ebp-154h]
    Vec3 v113; // [esp+37Ch] [ebp-144h] BYREF
    float v114; // [esp+388h] [ebp-138h]
    unsigned int v115; // [esp+38Ch] [ebp-134h]
    unsigned int v116; // [esp+390h] [ebp-130h]
    Vec3 v117; // [esp+398h] [ebp-128h] BYREF
    Vec3 v118; // [esp+3A4h] [ebp-11Ch] BYREF
    Vec3 v119; // [esp+3B0h] [ebp-110h] BYREF
    int v120; // [esp+3BCh] [ebp-104h]
    BOOL v121; // [esp+3C4h] [ebp-FCh]
    Vec3 v122; // [esp+3C8h] [ebp-F8h] BYREF
    BOOL v123; // [esp+3D4h] [ebp-ECh]
    float v124; // [esp+3E4h] [ebp-DCh]
    BOOL v125; // [esp+3E8h] [ebp-D8h]
    Vec3 v126; // [esp+3F0h] [ebp-D0h] BYREF
    Vec3 v127; // [esp+3FCh] [ebp-C4h] BYREF
    unsigned int v128; // [esp+408h] [ebp-B8h]
    Vec3 v129; // [esp+40Ch] [ebp-B4h] BYREF
    Vec3 v130; // [esp+418h] [ebp-A8h] BYREF
    int v131; // [esp+424h] [ebp-9Ch]
    int v132; // [esp+428h] [ebp-98h]
    float v133; // [esp+42Ch] [ebp-94h]
    int gfxId; // [esp+430h] [ebp-90h]
    unsigned int v135; // [esp+434h] [ebp-8Ch]
    unsigned int v136; // [esp+43Ch] [ebp-84h]
    Vec3 v137; // [esp+440h] [ebp-80h]
    Vec3 a3; // [esp+44Ch] [ebp-74h] BYREF
    tScriptStruct_Laser* v140; // [esp+464h] [ebp-5Ch]
    float v141; // [esp+468h] [ebp-58h]
    Vec3 dest; // [esp+46Ch] [ebp-54h] BYREF
    Vec3 v143; // [esp+478h] [ebp-48h] BYREF
    float v144; // [esp+490h] [ebp-30h]
    float v145; // [esp+498h] [ebp-28h]
    unsigned int v146; // [esp+4BCh] [ebp-4h]

    v122.x = 1.0;
    v122.y = 0.0;
    v122.z = 0.0;
    pos.x = 1.0;
    pos.y = 0.0;
    pos.z = 0.0;
    v130.x = 1.0;
    v130.y = 0.0;
    v130.z = 0.0;
    v94.x = 1.0;
    v94.y = 0.0;
    v94.z = 0.0;
    v119.x = 1.0;
    v119.y = 0.0;
    v119.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v23 = (tScriptStruct_Laser*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v23 = 0;
    v140 = v23;
    v132 = 0;
    v110 = 0.0;
    v111 = 0.0;
    v92 = 0;
    v120 = 0;
    gfxId = 0;
    v112 = 0.0;
    v83 = 0.0;
    v84 = 0;
    v136 = 0;
    i = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Laser_Laser_exec_shot_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        Laser_Laser_exec_shot_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Laser_Laser_exec_shot_GST_Global_gao_f_eau_hauteurZ,
            (void*)Laser_Laser_exec_shot_GST_Global_gao,
            2772);
        Laser_Laser_exec_shot_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
        Laser_Laser_exec_shot_DUP_SFX_Sally_Shield_gao = AI_ul_RealResolveOneRefObject(0x260020F1u);
    }
    v84 = *AI_MainActor;
    v77 = (tGameObjectMain*)v140->des_target;
    if (v77)
    {
        if ((unsigned int)v77 <= 2)
            v77 = AI_MainActor[(uint32_t)v77 - 1];
        v76 = v77;
    }
    else
    {
        v76 = 0;
    }
    if (v76)
    {
        v75 = (tGameObjectMain*)v140->des_target;
        if (v75)
        {
            if ((unsigned int)v75 <= 2)
                v75 = AI_MainActor[(uint32_t)v75 - 1];
            AI_EvalFunc_OBJGetPos_C(v75, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        v119 = a3;
        v74 = (tGameObjectMain*)v140->des_target;
        if (v74)
        {
            if ((unsigned int)v74 <= 2)
                v74 = AI_MainActor[(uint32_t)v74 - 1];
            AI_EvalFunc_DYNSpeedGetVector_C(v74, &v126);
        }
        else
        {
            AI_EvalFunc_DYNSpeedGetVector_C(0, &v126);
        }
        v95 = TIM_gf_dt;
        v87 = v126.x * TIM_gf_dt;
        v88 = v126.y * TIM_gf_dt;
        v89 = v126.z * TIM_gf_dt;
        v119.x = v119.x + v87;
        v119.y = v119.y + v88;
        v119.z = v119.z + v89;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v117);
        MATH_SubstractVec(&dest, &v119, &v117);
        AI_EvalFunc_MATHVecNormalize_C(&dest, &v140->v_beam_sight);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v127);
        v145 = v140->v_beam_sight.z * v127.z + v140->v_beam_sight.y * v127.y + v127.x * v140->v_beam_sight.x;
        if (v145 > 0.5)
            AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v140->v_beam_sight);
    }
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v140->v_beam_sight);
    if (v140->des_visuel == 1 || v140->des_visuel == 3)
    {
        v140->i_gfx_lightning = AI_EvalFunc_GFXAdd_C_CURRENT(1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v140->i_gfx_lightning, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v140->i_gfx_lightning, 2, 1);
        v73 = (tGameObjectMain*)Laser_Laser_exec_shot_SFX_light_smoke_gao;
        if (Laser_Laser_exec_shot_SFX_light_smoke_gao)
        {
            if ((unsigned int)Laser_Laser_exec_shot_SFX_light_smoke_gao <= 2)
                v73 = AI_MainActor[Laser_Laser_exec_shot_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v140->i_gfx_lightning, v73, v140->des_i_submat_beam);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v140->i_gfx_lightning, 0, v140->des_i_submat_beam);
        }
        GFXSetCurGlobal(v140->i_gfx_lightning);
        AI_EvalFunc_GFXSetf_C_CURGLOB(1000, v140->des_f_laser_width);
        AI_EvalFunc_GFXSeti_C_CURGLOB(1100, v140->des_c_color);
        if (v140->des_i_halo_au_bout)
        {
            v140->i_gfx_idhalo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v140->i_gfx_idhalo, 4, 1);
            v72 = (tGameObjectMain*)Laser_Laser_exec_shot_SFX_light_smoke_gao;
            if (Laser_Laser_exec_shot_SFX_light_smoke_gao)
            {
                if ((unsigned int)Laser_Laser_exec_shot_SFX_light_smoke_gao <= 2)
                    v72 = AI_MainActor[Laser_Laser_exec_shot_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v140->i_gfx_idhalo, v72, v140->des_i_halo_submat);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v140->i_gfx_idhalo, 0, v140->des_i_halo_submat);
            }
            GFXSetCurGlobal(v140->i_gfx_idhalo);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, v140->des_f_realhalo_size);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v140->des_c_color);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
        }
        AI_EvalFunc_LightSetColor_C(AI_gpst_CurrentGameObject, v140->des_c_color);
    }
    if (v140->des_visuel >= 2)
    {
        v140->i_gfx_ripple = AI_EvalFunc_GFXAdd_C_CURRENT(20);
        v71 = (tGameObjectMain*)Laser_Laser_exec_shot_SFX_light_smoke_gao;
        if (Laser_Laser_exec_shot_SFX_light_smoke_gao)
        {
            if ((unsigned int)Laser_Laser_exec_shot_SFX_light_smoke_gao <= 2)
                v71 = AI_MainActor[Laser_Laser_exec_shot_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v140->i_gfx_ripple, v71, v140->des_i_submat_ripple);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v140->i_gfx_ripple, 0, v140->des_i_submat_ripple);
        }
        GFXSetCurGlobal(v140->i_gfx_ripple);
        AI_EvalFunc_GFXSeti_C_CURGLOB(20100, v140->des_i_max_sprites);
        AI_EvalFunc_GFXSeti_C_CURGLOB(20101, v140->des_c_color_ripple);
        AI_EvalFunc_GFXSetf_C_CURGLOB(20000, v140->des_f_life);
        v124 = AI_EvalFunc_MATHRandFloat_C(v140->des_f_sizemin, v140->des_f_sizemax);
        AI_EvalFunc_GFXSetf_C_CURGLOB(20001, v124);
        AI_EvalFunc_GFXSetf_C_CURGLOB(20002, 0.0099999998f);
        X_4 = v140->des_f_laser_speed * 0.15000001f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(20003, X_4);
    }
    v120 = 0;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v140->v_last_beam_pos);
    if (v140->des_puissance == 8)
    {
        v140->objet_touche = 0;
    }
    else
    {
        if (v140->i_flag_factice)
        {
            v70 = v84;
            if (v84)
            {
                if ((unsigned int)v84 <= 2)
                    v70 = AI_MainActor[(uint32_t)v84 - 1];
                v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(v70);
            }
            else
            {
                v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
            }
            v115 = v0;
            v136 = v0 & 0x1000000;
            v69 = v84;
            if (v84)
            {
                if ((unsigned int)v84 <= 2)
                    v69 = AI_MainActor[(uint32_t)v84 - 1];
                v1 = AI_EvalFunc_OBJ_FlagsControlSet_C(v69, 0x1000000, 0);
            }
            else
            {
                v1 = AI_EvalFunc_OBJ_FlagsControlSet_C(0, 0x1000000, 0);
            }
            v91 = v1;
        }
        for (i = 0; i < 10; ++i)
        {
            v68 = (tGameObjectMain*)v140->ao_traverse[i];
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
            if (!v67)
                break;
            v66 = (tGameObjectMain*)v140->ao_traverse[i];
            if (v66)
            {
                if ((unsigned int)v66 <= 2)
                    v66 = AI_MainActor[(uint32_t)v66 - 1];
                v2 = AI_EvalFunc_OBJ_FlagsControlGet_C(v66);
            }
            else
            {
                v2 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
            }
            v116 = v2;
            *((uint32_t*)&Src + i + 13) = v2 & 0x1000000;
            v65 = (tGameObjectMain*)v140->ao_traverse[i];
            if (v65)
            {
                if ((unsigned int)v65 <= 2)
                    v65 = AI_MainActor[(uint32_t)v65 - 1];
                v3 = AI_EvalFunc_OBJ_FlagsControlSet_C(v65, 0x1000000, 0);
            }
            else
            {
                v3 = AI_EvalFunc_OBJ_FlagsControlSet_C(0, 0x1000000, 0);
            }
            v135 = v3;
        }
        AI_EvalFunc_COL_SpecificCrossableSet_C(-1 - (v140->des_test_flag_X & 0xffff));
        v4 = (int)AI_EvalFunc_COLRayObjectDist_C(
            AI_gpst_CurrentGameObject,
            &v140->v_last_beam_pos,
            &v140->v_beam_sight,
            v140->des_f_laser_length,
            v140->i_type_vrai,
            v140->i_type_faux,
            1,
            -2147483647);
        v140->objet_touche = v4;
        if (v140->i_flag_factice && !v136)
        {
            v64 = v84;
            if (v84)
            {
                if ((unsigned int)v84 <= 2)
                    v64 = AI_MainActor[(uint32_t)v84 - 1];
                v5 = AI_EvalFunc_OBJ_FlagsControlSet_C(v64, 0, 0x1000000);
            }
            else
            {
                v5 = AI_EvalFunc_OBJ_FlagsControlSet_C(0, 0, 0x1000000);
            }
            v146 = v5;
        }
        for (i = 0; i < 10; ++i)
        {
            v63 = (tGameObjectMain*)v140->ao_traverse[i];
            if (v63)
            {
                if ((unsigned int)v63 <= 2)
                    v63 = AI_MainActor[(uint32_t)v63 - 1];
                v62 = v63;
            }
            else
            {
                v62 = 0;
            }
            if (!v62)
                break;
            if (!*((uint32_t*)&Src + i + 13))
            {
                v61 = (tGameObjectMain*)v140->ao_traverse[i];
                if (v61)
                {
                    if ((unsigned int)v61 <= 2)
                        v61 = AI_MainActor[(uint32_t)v61 - 1];
                    v6 = AI_EvalFunc_OBJ_FlagsControlSet_C(v61, 0, 0x1000000);
                }
                else
                {
                    v6 = AI_EvalFunc_OBJ_FlagsControlSet_C(0, 0, 0x1000000);
                }
                v128 = v6;
            }
        }
    }
    v60 = (tGameObjectMain*)v140->objet_touche;
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
    if (!v59)
    {
        v58 = v140->des_f_laser_length;
        v108.x = v58 * v140->v_beam_sight.x;
        v108.y = v140->v_beam_sight.y * v58;
        v108.z = v140->v_beam_sight.z * v58;
        v130 = v108;
        v119 = v140->v_last_beam_pos;
        v119.x = v119.x + v108.x;
        v119.y = v119.y + v108.y;
        v119.z = v119.z + v108.z;
        if (v108.z < 0.0 && *(float*)Laser_Laser_exec_shot_GST_Global_gao_f_eau_hauteurZ + 10.0 > v119.z)
        {
            AI_EvalFunc_WTR_GetDif_C(&v119, &v107);
            v112 = v107.z;
            if (v119.z < (double)v107.z)
            {
                v140->objet_touche = (int)AI_gpst_CurrentGameObject;
                v120 = 1;
                v93.x = 0.0;
                v93.y = 0.0;
                v93.z = 1.0;
                v122.x = 0.0;
                v122.y = 0.0;
                v122.z = 1.0;
                pos.x = v140->v_last_beam_pos.x;
                pos.y = v140->v_last_beam_pos.y;
                pos.z = v112;
                v22 = (float)fabs(v130.z);
                v83 = v22;
                v130.z = 0.0;
                v110 = MATH_VecLen(&v130);
                if (v110 != 0.0f)
                {
                    v21 = (float)fabs(v140->v_last_beam_pos.z - v112);
                    v141 = v21;
                    v57 = v21 / v83;
                    v130.x = v130.x * v57;
                    v130.y = v130.y * v57;
                    v130.z = v130.z * v57;
                    pos.x = pos.x + v130.x;
                    pos.y = pos.y + v130.y;
                    pos.z = pos.z + v130.z;
                }
            }
        }
    }
    v56 = (tGameObjectMain*)v140->objet_touche;
    if (v56)
    {
        if ((unsigned int)v56 <= 2)
            v56 = AI_MainActor[(uint32_t)v56 - 1];
        v55 = v56;
    }
    else
    {
        v55 = 0;
    }
    if (!v55)
    {
        GFXSetCurGlobal(v140->i_gfx_lightning);
        AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &v140->v_last_beam_pos);
        v24 = v140->des_f_laser_length;
        v79.x = v24 * v140->v_beam_sight.x;
        v79.y = v140->v_beam_sight.y * v24;
        v79.z = v140->v_beam_sight.z * v24;
        MATH_VecAddVec(&v129, &v140->v_last_beam_pos, &v79);
        AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &v129);
        return 1000;
    }
    v54 = (tGameObjectMain*)v140->o_acteur_a_informer;
    if (v54)
    {
        if ((unsigned int)v54 <= 2)
            v54 = AI_MainActor[(uint32_t)v54 - 1];
        v53 = v54;
    }
    else
    {
        v53 = 0;
    }
    if (v53)
    {
        v140->m_msg.msgID = 77;
        v140->m_msg.gao2 = (tGameObjectMain*)v140->objet_touche;
        v52 = (tGameObjectMain*)v140->o_acteur_a_informer;
        if (v52)
        {
            if ((unsigned int)v52 <= 2)
                v52 = AI_MainActor[(uint32_t)v52 - 1];
            AI_EvalFunc_MSGSend_C(v52, &v140->m_msg);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &v140->m_msg);
        }
    }
    if (v140->des_puissance != 8)
    {
        v51 = (tGameObjectMain*)v140->objet_touche;
        if (v51)
        {
            if ((unsigned int)v51 <= 2)
                v51 = AI_MainActor[(uint32_t)v51 - 1];
            v7 = AI_EvalFunc_OBJ_CapaTest_C(v51, 0x200);
        }
        else
        {
            v7 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x200);
        }
        v121 = v7;
        if (!v7)
        {
            if (v140->des_paf_chaleur)
            {
                v140->m_msg.msgID = 37;
                v140->m_msg.gao = AI_gpst_CurrentGameObject;
                v140->m_msg.gao2 = 0;
                v140->m_msg.Int1 = v140->des_puissance + 40;
                v140->m_msg.Int2 = 0;
                v49 = &v140->m_msg.v1;
                v50 = &v140->v_beam_sight;
                v140->m_msg.v1.x = v140->v_beam_sight.x;
                v49->y = v50->y;
                v49->z = v50->z;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v140->m_msg.v2);
                v140->m_msg.f2 = (float)v140->des_f_point_de_vie;
                v140->m_msg.f1 = 1000.0;
                v48 = (tGameObjectMain*)v140->objet_touche;
                if (v48)
                {
                    if ((unsigned int)v48 <= 2)
                        v48 = AI_MainActor[(uint32_t)v48 - 1];
                    AI_EvalFunc_MSGSend_C(v48, &v140->m_msg);
                    goto LABEL_149;
                }
            }
            else
            {
                v140->m_msg.msgID = 37;
                v140->m_msg.gao = AI_gpst_CurrentGameObject;
                v140->m_msg.gao2 = 0;
                v140->m_msg.Int1 = v140->des_puissance + 8;
                v140->m_msg.Int2 = 0;
                v46 = &v140->m_msg.v1;
                v47 = &v140->v_beam_sight;
                v140->m_msg.v1.x = v140->v_beam_sight.x;
                v46->y = v47->y;
                v46->z = v47->z;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v140->m_msg.v2);
                v140->m_msg.f2 = (float)v140->des_f_point_de_vie;
                v140->m_msg.f1 = 1000.0;
                v45 = (tGameObjectMain*)v140->objet_touche;
                if (v45)
                {
                    if ((unsigned int)v45 <= 2)
                        v45 = AI_MainActor[(uint32_t)v45 - 1];
                    AI_EvalFunc_MSGSend_C(v45, &v140->m_msg);
                    goto LABEL_149;
                }
            }
            AI_EvalFunc_MSGSend_C(0, &v140->m_msg);
        }
    }
LABEL_149:
    if (!v120)
    {
        v44 = (tGameObjectMain*)v140->objet_touche;
        if (v44)
        {
            if ((unsigned int)v44 <= 2)
                v44 = AI_MainActor[(uint32_t)v44 - 1];
            v8 = AI_EvalFunc_COLGMFlagsGet_C(v44, 0xFFFF);
        }
        else
        {
            v8 = AI_EvalFunc_COLGMFlagsGet_C(0, 0xFFFF);
        }
        v131 = v8;
        v120 = v8 & 0x10;
        AI_EvalFunc_COLRayPosGet_C(AI_gpst_CurrentGameObject, &v86);
        pos = v86;
        AI_EvalFunc_COLRayNormalGet_C(AI_gpst_CurrentGameObject, &v100);
        v122 = v100;
        if (AI2C_VecEqNull(&pos))
        {
            v43 = (tGameObjectMain*)v140->objet_touche;
            if (v43)
            {
                if ((unsigned int)v43 <= 2)
                    v43 = AI_MainActor[(uint32_t)v43 - 1];
                AI_EvalFunc_OBJGetPos_C(v43, &v143);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v143);
            }
            pos = v143;
        }
        v42 = (tGameObjectMain*)v140->objet_touche;
        if (v42)
        {
            if ((unsigned int)v42 <= 2)
                v42 = AI_MainActor[(uint32_t)v42 - 1];
            v9 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v42, 4);
        }
        else
        {
            v9 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 4);
        }
        v125 = v9;
        if (v9)
        {
            v41 = (tGameObjectMain*)v140->objet_touche;
            if (v41)
            {
                if ((unsigned int)v41 <= 2)
                    v41 = AI_MainActor[(uint32_t)v41 - 1];
                AI_EvalFunc_DYNSpeedGetVector_C(v41, &v113);
            }
            else
            {
                AI_EvalFunc_DYNSpeedGetVector_C(0, &v113);
            }
            v133 = TIM_gf_dt;
            v101 = v113.x * TIM_gf_dt;
            v102 = v113.y * TIM_gf_dt;
            v103 = v113.z * TIM_gf_dt;
            pos.x = pos.x + v101;
            pos.y = pos.y + v102;
            pos.z = pos.z + v103;
        }
    }
    if (v120)
    {
        v92 = (tAI*)AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)Laser_Laser_exec_shot_SFX_explosion_gao, &pos);
        Src.msgID = 1;
        Src.Int1 = 7;
        Src.f2 = 30.0;
        Src.f1 = 60.0;
        Src.v1.x = 0.25;
        Src.v1.y = 0.0;
        Src.v1.z = -1.0e14f;
        v40 = (tGameObjectMain*)v92;
        if (v92)
        {
            if ((unsigned int)v92 <= 2)
                v40 = AI_MainActor[(uint32_t)v92 - 1];
            AI_EvalFunc_MSGSend_C(v40, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
        v39 = (tGameObjectMain*)v92;
        if (v92)
        {
            if ((unsigned int)v92 <= 2)
                v39 = AI_MainActor[(uint32_t)v92 - 1];
            AI_EvalFunc_OBJ_SightSet_C(v39, &v122);
        }
        else
        {
            AI_EvalFunc_OBJ_SightSet_C(0, &v122);
        }
        if (v92 == WOR_gst_Universe.univAIInstance)
        {
            v36 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v38 = (tGameObjectMain*)v92;
            if (v92)
            {
                if ((unsigned int)v92 <= 2)
                    v38 = AI_MainActor[(uint32_t)v92 - 1];
                v37 = v38;
            }
            else
            {
                v37 = 0;
            }
            v36 = v37->extended->AIInstance;
        }
        *((uint32_t*)v36->instanceVarDataCurrent + 23) = 0xFFFFFF;
    }
    else
    {
        v140->spark = AI_EvalFunc_GFXAdd_C_CURRENT(9);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v140->spark, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v140->spark, 2, 1);
        v35 = (tGameObjectMain*)Laser_Laser_exec_shot_SFX_light_smoke_gao;
        if (Laser_Laser_exec_shot_SFX_light_smoke_gao)
        {
            if ((unsigned int)Laser_Laser_exec_shot_SFX_light_smoke_gao <= 2)
                v35 = AI_MainActor[Laser_Laser_exec_shot_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v140->spark, v35, 5);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v140->spark, 0, 5);
        }
        GFXSetCurGlobal(v140->spark);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9000, 1.570796f);
        X_4a = v140->des_f_laser_speed / 8.0f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(9001, X_4a);
        X_4b = v140->des_f_laser_speed / 4.0f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(9002, X_4b);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9003, 0.80000001f);
        X_4c = v140->des_f_laser_width / 2.0f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(9004, X_4c);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9005, 0.40000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9006, 0.80000001f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9007, 2.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9008, 0.0049999999f);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9100, 10);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9102, 8);
        AI_EvalFunc_GFXSetv_C_CURGLOB(9200, &pos);
        AI_EvalFunc_GFXSetv_C_CURGLOB(9201, &v122);
        v98.x = 0.0;
        v98.y = 0.0;
        v98.z = -50.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(9202, &v98);
    }
    gfxId = AI_EvalFunc_GFXAdd_C_CURRENT(21);
    AI_EvalFunc_GFXFlagSet_C_CURRENT(gfxId, 4, 1);
    v34 = (tGameObjectMain*)Laser_Laser_exec_shot_SFX_light_smoke_gao;
    if (Laser_Laser_exec_shot_SFX_light_smoke_gao)
    {
        if ((unsigned int)Laser_Laser_exec_shot_SFX_light_smoke_gao <= 2)
            v34 = AI_MainActor[Laser_Laser_exec_shot_SFX_light_smoke_gao - 1];
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(gfxId, v34, 0);
    }
    else
    {
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(gfxId, 0, 0);
    }
    GFXSetCurGlobal(gfxId);
    X_4d = v140->des_f_laser_width + v140->des_f_laser_width;
    AI_EvalFunc_GFXSetf_C_CURGLOB(21002, X_4d);
    AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
    AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v140->des_c_color);
    X_4e = v140->des_f_laser_width + v140->des_f_laser_width;
    AI_EvalFunc_GFXSetf_C_CURGLOB(21000, X_4e);
    AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &pos);
    AI_EvalFunc_GFXSetf_C_CURGLOB(21005, 0.1f);
    AI_EvalFunc_GFXSeti_C_CURGLOB(21100, 16);
    AI_EvalFunc_GFXLifeTimeSet_C(AI_gpst_CurrentGameObject, gfxId, 0.1f);
    v140->f_time_spark = TIM_gf_MainClock - TIM_gf_LockTime;
    if (!v140->des_shield_rebound)
        goto LABEL_235;
    v33 = (tGameObjectMain*)v140->objet_touche;
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
    if (v32 == (tGameObjectMain*)Laser_Laser_exec_shot_DUP_SFX_Sally_Shield_gao)
    {
        v105 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(3, -1.0);
        v140->v_last_beam_pos = pos;
        v94 = v122;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v118);
        v104 = v118.z * v94.z + v118.y * v94.y + v94.x * v118.x;
        v20 = (float)fabs(v104);
        v114 = v20;
        v111 = v20;
        if (v20 <= 0.98480803)
        {
            v31 = 2.0f * v111;
            v94.x = v94.x * v31;
            v94.y = v94.y * v31;
            v94.z = v94.z * v31;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v82);
            v94.x = v94.x + v82.x;
            v94.y = v94.y + v82.y;
            v94.z = v94.z + v82.z;
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v78);
            v137.x = -v78.x;
            v137.y = -v78.y;
            v137.z = -v78.z;
            v94.x = -v78.x;
            v94.y = -v78.y;
            v94.z = -v78.z;
        }
        AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v94);
        v140->v_beam_sight = v94;
    }
    else
    {
    LABEL_235:
        if (v140->i_rebound_nb >= v140->des_i_rebound_nb || !v140->des_water_rebound && v120)
            goto LABEL_236;
        v30 = (tGameObjectMain*)v140->objet_touche;
        if (v30)
        {
            if ((unsigned int)v30 <= 2)
                v30 = AI_MainActor[(uint32_t)v30 - 1];
            v10 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v30, 2);
        }
        else
        {
            v10 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 2);
        }
        v123 = v10;
        if (v10)
            goto LABEL_236;
        v99 = *AI_MainActor;
        v29 = (tGameObjectMain*)v140->objet_touche;
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
        v27 = v99;
        if (v99)
        {
            if ((unsigned int)v99 <= 2)
                v27 = AI_MainActor[(uint32_t)v99 - 1];
            v26 = v27;
        }
        else
        {
            v26 = 0;
        }
        if (v28 == v26)
        {
        LABEL_236:
            v140->i_flag_touche = 1;
            v132 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(2, -1.0);
            v85 = SND_f_GetVol(v132);
            v106 = AI_EvalFunc_MATHRandFloat_C(v140->snd_volmin, v140->snd_volmax);
            X_4f = v85 * v106;
            SND_SetVol(v132, X_4f);
            GFXSetCurGlobal(v140->i_gfx_lightning);
            AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &v140->v_last_beam_pos);
            AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &pos);
        }
        else
        {
            v140->v_last_beam_pos = pos;
            v94 = v122;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v80);
            v144 = v80.z * v94.z + v80.y * v94.y + v94.x * v80.x;
            v19 = (float)fabs(v144);
            v90 = v19;
            v111 = v19;
            v25 = 2.0f * v19;
            v94.x = v94.x * v25;
            v94.y = v94.y * v25;
            v94.z = v94.z * v25;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v81);
            v94.x = v94.x + v81.x;
            v94.y = v94.y + v81.y;
            v94.z = v94.z + v81.z;
            AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v94);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v140->v_beam_sight);
        }
    }
    ++v140->i_rebound_nb;
    return 1000;
}

int Laser_Laser_tempo_destroy()
{
    int result; // eax
    tScriptStruct_Laser* v1; // [esp+8h] [ebp-18h]
    float v2; // [esp+Ch] [ebp-14h]
    float v3; // [esp+18h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_Laser*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (*AI_gppst_CurrentJumpNode == (tAINode*)2)
        goto LABEL_11;
    if (*AI_gppst_CurrentJumpNode != (tAINode*)5)
    {
        if (v1->des_mode != 2)
        {
        LABEL_17:
            if (!v1->des_dont_destroy)
            {
                AI_EvalFunc_LightSetNearFar_C(AI_gpst_CurrentGameObject, 0.0, 0.0);
                *AI_gppst_CurrentJumpNode = (tAINode*)5;
                TIME_Wait_C(0.5);
                return 0;
            }
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
            AI_EvalFunc_OBJ_FlagInactiveSet_C(AI_gpst_CurrentGameObject, 1);
            return 1000;
        }
        v1->f_blend_sight = 15.0;
    LABEL_11:
        XMEN_SetSize(AI_gpst_CurrentGameObject, 0, 0, v1->f_blend_sight);
        v2 = 6.0f * TIM_gf_dt;
        if (v2 < 0.0)
            v2 = 0.0;
        if (v2 > 1.0)
            v2 = 1.0;
        v3 = 0.0f * v2 + v1->f_blend_sight * (1.0f - v2);
        v1->f_blend_sight = v3;
        if (v1->f_blend_sight > 1.0)
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)2;
            return 0;
        }
        goto LABEL_17;
    }
    result = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
    if (result == 1000)
        return 1000;
    return result;
}

int Laser_Laser_init()
{
    tGameObjectMain* v0; // eax
    int v2; // eax
    float v3; // [esp+8h] [ebp-1B0h]
    int v4; // [esp+Ch] [ebp-1ACh]
    int v5; // [esp+10h] [ebp-1A8h]
    tScriptStruct_Laser* v6; // [esp+14h] [ebp-1A4h]
    tGameObjectMain* v7; // [esp+1Ch] [ebp-19Ch]
    tGameObjectMain* v8; // [esp+24h] [ebp-194h]
    tGameObjectMain* v9; // [esp+28h] [ebp-190h]
    tGameObjectMain* v10; // [esp+2Ch] [ebp-18Ch]
    double v11; // [esp+30h] [ebp-188h]
    tGameObjectMain* v12; // [esp+44h] [ebp-174h]
    tGameObjectMain* v13; // [esp+48h] [ebp-170h]
    tGameObjectMain* v14; // [esp+50h] [ebp-168h]
    tGameObjectMain* v15; // [esp+58h] [ebp-160h]
    tGameObjectMain* v16; // [esp+68h] [ebp-150h]
    tGameObjectMain* v17; // [esp+70h] [ebp-148h]
    tGameObjectMain* v18; // [esp+80h] [ebp-138h]
    tGameObjectMain* v19; // [esp+88h] [ebp-130h]
    tGameObjectMain* v20; // [esp+98h] [ebp-120h]
    tGameObjectMain* v21; // [esp+A0h] [ebp-118h]
    tGameObjectMain* v22; // [esp+A8h] [ebp-110h]
    tGameObjectMain* v23; // [esp+B8h] [ebp-100h]
    tGameObjectMain* v24; // [esp+C0h] [ebp-F8h]
    tGameObjectMain* v25; // [esp+C4h] [ebp-F4h]
    tGameObjectMain* v26; // [esp+C8h] [ebp-F0h]
    Vec3 v27; // [esp+CCh] [ebp-ECh] BYREF
    void* Src; // [esp+D8h] [ebp-E0h]
    Vec3 a1; // [esp+DCh] [ebp-DCh] BYREF
    float v30; // [esp+E8h] [ebp-D0h]
    int v31; // [esp+ECh] [ebp-CCh]
    Vec3 v32; // [esp+F0h] [ebp-C8h] BYREF
    int v33; // [esp+FCh] [ebp-BCh]
    Vec3 v34[2]; // [esp+100h] [ebp-B8h] BYREF
    unsigned int v35; // [esp+118h] [ebp-A0h]
    Vec3 v37; // [esp+128h] [ebp-90h] BYREF
    tGameObjectMain* v38; // [esp+134h] [ebp-84h]
    Vec3 v39; // [esp+138h] [ebp-80h] BYREF
    Vec3 v40; // [esp+144h] [ebp-74h] BYREF
    BOOL v41; // [esp+150h] [ebp-68h]
    int v42; // [esp+154h] [ebp-64h]
    Vec3 a3; // [esp+158h] [ebp-60h] BYREF
    Vec3 v44; // [esp+164h] [ebp-54h] BYREF
    int v45; // [esp+170h] [ebp-48h]
    int v46; // [esp+174h] [ebp-44h]
    BOOL v47; // [esp+178h] [ebp-40h]
    Vec3 v48; // [esp+17Ch] [ebp-3Ch] BYREF
    tScriptStruct_Laser* v49; // [esp+188h] [ebp-30h]
    Vec3 v50; // [esp+18Ch] [ebp-2Ch] BYREF
    Vec3 v51; // [esp+198h] [ebp-20h] BYREF
    Vec3 v53; // [esp+1A8h] [ebp-10h] BYREF
    int v54; // [esp+1B4h] [ebp-4h]

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_Laser*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v49 = v6;
    v42 = 0;
    v35 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Laser_Laser_init_Laser_Laser_init = (int)AI_ul_RealResolveOneRefFunction(0x720008BBu);
        fct_Laser_Laser_init_Laser_Laser_exec_shot = (int)AI_ul_RealResolveOneRefFunction(0x720008B9u);
        fct_Laser_Laser_init_Laser_Laser_comportement = (int)AI_ul_RealResolveOneRefFunction(0x720008BAu);
    }
    if (v49->des_f_realhalo_size == 0.0)
        v49->des_f_realhalo_size = (float)v49->des_f_halo_size;
    v26 = (tGameObjectMain*)v49->o_start_gao_pos;
    if (v26)
    {
        if ((unsigned int)v26 <= 2)
            v26 = AI_MainActor[(uint32_t)v26 - 1];
        v25 = v26;
    }
    else
    {
        v25 = 0;
    }
    if (!v25)
        goto LABEL_93;
    if (v49->i_flag_special_init)
    {
        if (v49->i_start_bone == -1)
        {
            v35 = v49->o_start_gao_pos;
        }
        else
        {
            v24 = (tGameObjectMain*)v49->o_start_gao_pos;
            if (v24)
            {
                if ((unsigned int)v24 <= 2)
                    v24 = AI_MainActor[(uint32_t)v24 - 1];
                v0 = AI_EvalFunc_ANICanalGaoGet_C(v24, v49->i_start_bone);
            }
            else
            {
                v0 = AI_EvalFunc_ANICanalGaoGet_C(0, v49->i_start_bone);
            }
            v35 = (unsigned int)v0;
        }
        v23 = (tGameObjectMain*)v35;
        if (v35)
        {
            if (v35 <= 2)
                v23 = AI_MainActor[v35 - 1];
            AI_EvalFunc_MATHVecLocalToGlobal_C(v23, &v49->v_start_offset, &v48);
        }
        else
        {
            AI_EvalFunc_MATHVecLocalToGlobal_C(0, &v49->v_start_offset, &v48);
        }
        a1 = v48;
        v22 = (tGameObjectMain*)v35;
        if (v35)
        {
            if (v35 <= 2)
                v22 = AI_MainActor[v35 - 1];
            AI_EvalFunc_OBJGetPos_C(v22, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        a1.x = a1.x + a3.x;
        a1.y = a1.y + a3.y;
        a1.z = a1.z + a3.z;
        if (v49->i_start_sight)
        {
            if (v49->i_start_sight < 0)
                v5 = -v49->i_start_sight;
            else
                v5 = v49->i_start_sight;
            v31 = v5;
            if (v5 == 1)
            {
                if (v49->i_start_sight <= 0)
                {
                    v20 = (tGameObjectMain*)v35;
                    if (v35)
                    {
                        if (v35 <= 2)
                            v20 = AI_MainActor[v35 - 1];
                        AI_EvalFunc_OBJ_HorizonGet_C(v20, &v37);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_HorizonGet_C(0, &v37);
                    }
                    v51.x = -v37.x;
                    v51.y = -v37.y;
                    v51.z = -v37.z;
                    AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v51);
                }
                else
                {
                    v21 = (tGameObjectMain*)v35;
                    if (v35)
                    {
                        if (v35 <= 2)
                            v21 = AI_MainActor[v35 - 1];
                        AI_EvalFunc_OBJ_HorizonGet_C(v21, &v27);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_HorizonGet_C(0, &v27);
                    }
                    AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v27);
                }
            }
            else
            {
                if (v49->i_start_sight < 0)
                    v4 = -v49->i_start_sight;
                else
                    v4 = v49->i_start_sight;
                v33 = v4;
                if (v4 == 2)
                {
                    if (v49->i_start_sight <= 0)
                    {
                        v18 = (tGameObjectMain*)v35;
                        if (v35)
                        {
                            if (v35 <= 2)
                                v18 = AI_MainActor[v35 - 1];
                            AI_EvalFunc_OBJ_SightGet_C(v18, &v50);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_SightGet_C(0, &v50);
                        }
                        v34[0].x = -v50.x;
                        v34[0].y = -v50.y;
                        v34[0].z = -v50.z;
                        AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, v34);
                    }
                    else
                    {
                        v19 = (tGameObjectMain*)v35;
                        if (v35)
                        {
                            if (v35 <= 2)
                                v19 = AI_MainActor[v35 - 1];
                            AI_EvalFunc_OBJ_SightGet_C(v19, &v44);
                        }
                        else
                        {
                            AI_EvalFunc_OBJ_SightGet_C(0, &v44);
                        }
                        AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v44);
                    }
                }
                else if (v49->i_start_sight <= 0)
                {
                    v16 = (tGameObjectMain*)v35;
                    if (v35)
                    {
                        if (v35 <= 2)
                            v16 = AI_MainActor[v35 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v16, &v32);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v32);
                    }
                    v53.x = -v32.x;
                    v53.y = -v32.y;
                    v53.z = -v32.z;
                    AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v53);
                }
                else
                {
                    v17 = (tGameObjectMain*)v35;
                    if (v35)
                    {
                        if (v35 <= 2)
                            v17 = AI_MainActor[v35 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v17, &v40);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v40);
                    }
                    AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v40);
                }
            }
        }
        AI_EvalFunc_OBJSetPos_C_CURRENT(&a1);
        v15 = (tGameObjectMain*)v49->o_start_gao_pos;
        if (v15)
        {
            if ((unsigned int)v15 <= 2)
                v15 = AI_MainActor[(uint32_t)v15 - 1];
            AI_EvalFunc_AICBDel_C(AI_gpst_CurrentGameObject, v15, 2, (tAIFunction*)fct_Laser_Laser_init_Laser_Laser_init);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(AI_gpst_CurrentGameObject, 0, 2, (tAIFunction*)fct_Laser_Laser_init_Laser_Laser_init);
        }
    LABEL_93:
        v41 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 0x8000);
        v49->f_time_last_shoot = TIM_gf_MainClock - TIM_gf_LockTime;
        v49->f_alea_sight = AI_EvalFunc_MATHRandFloat_C(-2.0, 2.0);
        AI_EvalFunc_AIPrioritySet_C(AI_gpst_CurrentGameObject, 255);
        AI_EvalFunc_LightSetNearFar_C(AI_gpst_CurrentGameObject, 3.0, 5.0);
        if (v49->des_test_flag_X)
            v49->des_test_flag_X = 0x200000;
        else
            v49->des_test_flag_X = 0;
        if (v49->des_test_water)
            v49->des_test_flag_X += 16;
        Src = (void*)AI_EvalFunc_MSGGet_C_CURRENT();
        memcpy(&v49->m_msg, Src, sizeof(v49->m_msg));
        if (v49->m_msg.msgID)
        {
            if (v49->m_msg.f2 > 0.0)
                v49->des_f_laser_speed = v49->m_msg.f2;
            if (v49->m_msg.f1 > 0.0)
                v49->des_f_laser_length = v49->m_msg.f1;
            v13 = v49->m_msg.gao2;
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
                v49->des_o_sight = (int)v49->m_msg.gao2;
            if (v49->m_msg.Int1 >= 0)
                v49->des_puissance = v49->m_msg.Int1;
            if (v49->m_msg.Int2)
            {
                v42 = v49->m_msg.Int2;
                v49->des_c_color = AI_EvalFunc_COLORSetRGBA_C(
                    (unsigned __int8)v42,
                    (v42 & 0xFF00) >> 8,
                    (v42 & 0xFF0000) >> 16,
                    (int)(v42 & 0xFF000000) >> 24);
            }
            if (fabs(v49->m_msg.v1.x) != 0.0f)
            {
                v47 = 0;
            }
            else if (fabs(v49->m_msg.v1.y) != 0.0f)
            {
                v47 = 0;
            }
            else
            {
                v47 = fabs(v49->m_msg.v1.z) == 0.0f;
            }
            if (!v47)
            {
                a1 = v49->m_msg.v1;
                if (a1.x != 0.0f)
                {
                    v11 = a1.x + MATH_gd_magicDoubleForRound;
                    v49->des_f_point_de_vie = (*(uint32_t*)&v11);
                }
                if (a1.y != 0.0)
                    v49->des_f_laser_width = a1.y;
                if (a1.z != 0.0)
                    v49->des_f_laser_life_time = a1.z;
            }
            AI_EvalFunc_MSGClear_C(AI_gpst_CurrentGameObject);
        }
        if (!v49->i_type_vrai)
            v49->i_type_vrai = -1;
        v10 = (tGameObjectMain*)v49->des_o_sight;
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
            v8 = (tGameObjectMain*)v49->des_o_sight;
            if (v8)
            {
                if ((unsigned int)v8 <= 2)
                    v8 = AI_MainActor[(uint32_t)v8 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v8, &v39);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v39);
            }
            AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v39);
        }
        v54 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Laser_Laser_init_Laser_Laser_exec_shot);
        if (v54 != 1000)
            return v54;
        v45 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(0, -1.0);
        if (v49->f_SND_play_dist == 0.0)
        {
            if (v49->i_flag_play_loop)
            {
                if (!AI_EvalFunc_OBJ_CapaTest_C_CURRENT(2))
                {
                    v2 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 1, 402653184);
                    v49->i_snd_req = v2;
                    v46 = SND_l_GetFreq(v49->i_snd_req);
                    v30 = AI_EvalFunc_MATHRandFloat_C(v49->f_freq_min, v49->f_freq_max);
                    v3 = (float)v46;
                    SND_SetFreq(v49->i_snd_req, (unsigned __int64)(v3 * v30));
                    AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v49->i_snd_req);
                }
            }
        }
        else
        {
            v49->f_SND_play_dist = v49->f_SND_play_dist * v49->f_SND_play_dist;
        }
        v49->i_flag_1st_trame = 1;
        if (v49->des_f_point_de_vie > 0 && v49->des_f_point_de_vie < 100 && !AI_EvalFunc_OBJ_CapaTest_C_CURRENT(1))
            v49->des_f_point_de_vie = 20;
        v38 = AI_gpst_CurrentGameObject;
        v7 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v7 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v7,
                2,
                (tAIFunction*)fct_Laser_Laser_init_Laser_Laser_comportement);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_Laser_Laser_init_Laser_Laser_comportement);
        }
        if (!v49->i_flag_special_init)
            AI_EvalFunc_AITrackCurStop_C();
        return 1000;
    }
    v49->i_flag_special_init = 1;
    v14 = (tGameObjectMain*)v49->o_start_gao_pos;
    if (v14)
    {
        if ((unsigned int)v14 <= 2)
            v14 = AI_MainActor[(uint32_t)v14 - 1];
        AI_EvalFunc_AICBAdd_C(AI_gpst_CurrentGameObject, v14, 2, (tAIFunction*)fct_Laser_Laser_init_Laser_Laser_init);
    }
    else
    {
        AI_EvalFunc_AICBAdd_C(AI_gpst_CurrentGameObject, 0, 2, (tAIFunction*)fct_Laser_Laser_init_Laser_Laser_init);
    }
    AI_EvalFunc_AITrackCurStop_C();
    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
    return 4;
}

int Laser_Laser_comportement()
{
    int result; // eax
    double v1; // st7
    unsigned int v2; // eax
    unsigned int v3; // eax
    unsigned int v4; // eax
    unsigned int v5; // eax
    int v6; // eax
    unsigned int v7; // eax
    unsigned int v8; // eax
    BOOL v9; // eax
    int v10; // eax
    BOOL v11; // eax
    BOOL v12; // eax
    tGameObjectMain* v13; // eax
    float a4; // [esp+0h] [ebp-730h]
    float X_4; // [esp+10h] [ebp-720h]
    float X_4a; // [esp+10h] [ebp-720h]
    float X_4b; // [esp+10h] [ebp-720h]
    float X_4c; // [esp+10h] [ebp-720h]
    float X_4d; // [esp+10h] [ebp-720h]
    float X_4e; // [esp+10h] [ebp-720h]
    float X_4f; // [esp+10h] [ebp-720h]
    float v22; // [esp+14h] [ebp-71Ch]
    float v23; // [esp+18h] [ebp-718h]
    float v24; // [esp+1Ch] [ebp-714h]
    float v25; // [esp+20h] [ebp-710h]
    float v26; // [esp+24h] [ebp-70Ch]
    tScriptStruct_Laser* v27; // [esp+28h] [ebp-708h]
    float v28; // [esp+30h] [ebp-700h]
    float v29; // [esp+38h] [ebp-6F8h]
    float v30; // [esp+40h] [ebp-6F0h]
    float v31; // [esp+4Ch] [ebp-6E4h]
    float v32; // [esp+6Ch] [ebp-6C4h]
    tGameObjectMain* v33; // [esp+74h] [ebp-6BCh]
    tGameObjectMain* v34; // [esp+78h] [ebp-6B8h]
    tGameObjectMain* v35; // [esp+7Ch] [ebp-6B4h]
    tGameObjectMain* v36; // [esp+80h] [ebp-6B0h]
    tGameObjectMain* v37; // [esp+84h] [ebp-6ACh]
    tGameObjectMain* v38; // [esp+88h] [ebp-6A8h]
    tGameObjectMain* v39; // [esp+8Ch] [ebp-6A4h]
    tGameObjectMain* v40; // [esp+90h] [ebp-6A0h]
    tGameObjectMain* v41; // [esp+94h] [ebp-69Ch]
    tGameObjectMain* v42; // [esp+9Ch] [ebp-694h]
    float v43; // [esp+A8h] [ebp-688h]
    tGameObjectMain* v44; // [esp+CCh] [ebp-664h]
    tGameObjectMain* v45; // [esp+D0h] [ebp-660h]
    tGameObjectMain* v46; // [esp+D8h] [ebp-658h]
    tGameObjectMain* v47; // [esp+E0h] [ebp-650h]
    tAI* v48; // [esp+E8h] [ebp-648h]
    tGameObjectMain* v49; // [esp+ECh] [ebp-644h]
    tGameObjectMain* v50; // [esp+F0h] [ebp-640h]
    tGameObjectMain* v51; // [esp+F8h] [ebp-638h]
    tGameObjectMain* v52; // [esp+100h] [ebp-630h]
    tGameObjectMain* v53; // [esp+108h] [ebp-628h]
    tGameObjectMain* v54; // [esp+110h] [ebp-620h]
    tGameObjectMain* v55; // [esp+120h] [ebp-610h]
    tGameObjectMain* v56; // [esp+140h] [ebp-5F0h]
    tGameObjectMain* v57; // [esp+148h] [ebp-5E8h]
    tGameObjectMain* v58; // [esp+158h] [ebp-5D8h]
    tGameObjectMain* v59; // [esp+178h] [ebp-5B8h]
    tGameObjectMain* v60; // [esp+180h] [ebp-5B0h]
    tGameObjectMain* v61; // [esp+184h] [ebp-5ACh]
    tGameObjectMain* v62; // [esp+188h] [ebp-5A8h]
    tGameObjectMain* v63; // [esp+190h] [ebp-5A0h]
    tAI* v64; // [esp+194h] [ebp-59Ch]
    tAI* v65; // [esp+198h] [ebp-598h]
    tGameObjectMain* v66; // [esp+19Ch] [ebp-594h]
    tGameObjectMain* v67; // [esp+1A0h] [ebp-590h]
    tGameObjectMain* v68; // [esp+1A8h] [ebp-588h]
    tGameObjectMain* v69; // [esp+1ACh] [ebp-584h]
    tGameObjectMain* v70; // [esp+1B4h] [ebp-57Ch]
    tGameObjectMain* v71; // [esp+1B8h] [ebp-578h]
    tGameObjectMain* v72; // [esp+1BCh] [ebp-574h]
    tGameObjectMain* v73; // [esp+1C0h] [ebp-570h]
    tGameObjectMain* v74; // [esp+1C4h] [ebp-56Ch]
    float v75; // [esp+1C8h] [ebp-568h]
    float v76; // [esp+1F4h] [ebp-53Ch]
    tGameObjectMain* v77; // [esp+1F8h] [ebp-538h]
    tGameObjectMain* v78; // [esp+1FCh] [ebp-534h]
    tGameObjectMain* v79; // [esp+204h] [ebp-52Ch]
    tGameObjectMain* v80; // [esp+208h] [ebp-528h]
    tGameObjectMain* v81; // [esp+20Ch] [ebp-524h]
    tGameObjectMain* v82; // [esp+214h] [ebp-51Ch]
    float v83; // [esp+21Ch] [ebp-514h]
    tGameObjectMain* v84; // [esp+220h] [ebp-510h]
    tGameObjectMain* v85; // [esp+224h] [ebp-50Ch]
    tGameObjectMain* v86; // [esp+22Ch] [ebp-504h]
    tGameObjectMain* v87; // [esp+234h] [ebp-4FCh]
    tGameObjectMain* v88; // [esp+238h] [ebp-4F8h]
    tGameObjectMain* v89; // [esp+23Ch] [ebp-4F4h]
    tGameObjectMain* v90; // [esp+244h] [ebp-4ECh]
    tGameObjectMain* v91; // [esp+24Ch] [ebp-4E4h]
    Vec3* v92; // [esp+250h] [ebp-4E0h]
    tGameObjectMain* v93; // [esp+270h] [ebp-4C0h]
    tGameObjectMain* v94; // [esp+27Ch] [ebp-4B4h]
    tGameObjectMain* v95; // [esp+28Ch] [ebp-4A4h]
    tGameObjectMain* v96; // [esp+290h] [ebp-4A0h]
    tGameObjectMain* v97; // [esp+294h] [ebp-49Ch]
    float v98; // [esp+29Ch] [ebp-494h]
    float v99; // [esp+2A4h] [ebp-48Ch]
    float v100; // [esp+2B0h] [ebp-480h]
    tGameObjectMain* v101; // [esp+2B8h] [ebp-478h]
    tGameObjectMain* v102; // [esp+2C0h] [ebp-470h]
    Vec3 v103; // [esp+2C4h] [ebp-46Ch] BYREF
    Vec3 v104; // [esp+2DCh] [ebp-454h] BYREF
    Vec3 v105; // [esp+2E8h] [ebp-448h] BYREF
    int gfxId; // [esp+2F8h] [ebp-438h]
    int v107; // [esp+300h] [ebp-430h]
    int v108; // [esp+304h] [ebp-42Ch]
    tGameObjectMain* v109; // [esp+308h] [ebp-428h]
    BOOL v110; // [esp+30Ch] [ebp-424h]
    Vec3 v112; // [esp+314h] [ebp-41Ch] BYREF
    float v113; // [esp+32Ch] [ebp-404h]
    float v114; // [esp+330h] [ebp-400h]
    float v115; // [esp+334h] [ebp-3FCh]
    float v116; // [esp+338h] [ebp-3F8h]
    float v117; // [esp+33Ch] [ebp-3F4h]
    float v118; // [esp+340h] [ebp-3F0h]
    int v119; // [esp+344h] [ebp-3ECh]
    Vec3 v120; // [esp+348h] [ebp-3E8h] BYREF
    Vec3 v121; // [esp+354h] [ebp-3DCh] BYREF
    Vec3 v122; // [esp+360h] [ebp-3D0h] BYREF
    tMessage Src; // [esp+36Ch] [ebp-3C4h] BYREF
    int v124; // [esp+3A0h] [ebp-390h]
    tAI* v125; // [esp+3A4h] [ebp-38Ch]
    Vec3 v126; // [esp+3A8h] [ebp-388h] BYREF
    Vec3 v127; // [esp+3B4h] [ebp-37Ch] BYREF
    Vec3 v128; // [esp+3C0h] [ebp-370h] BYREF
    Vec3 v129; // [esp+3CCh] [ebp-364h] BYREF
    Vec3 v130; // [esp+3D8h] [ebp-358h] BYREF
    unsigned int v131; // [esp+3E4h] [ebp-34Ch]
    int v132; // [esp+3E8h] [ebp-348h]
    float v133; // [esp+3ECh] [ebp-344h]
    Vec3 v134; // [esp+3F0h] [ebp-340h] BYREF
    Vec3 v135; // [esp+3FCh] [ebp-334h]
    Vec3 v136; // [esp+408h] [ebp-328h] BYREF
    Vec3 v137; // [esp+414h] [ebp-31Ch] BYREF
    unsigned int v138; // [esp+424h] [ebp-30Ch]
    float v139; // [esp+428h] [ebp-308h]
    unsigned int v140; // [esp+430h] [ebp-300h]
    Vec3 v141; // [esp+434h] [ebp-2FCh] BYREF
    Vec3 v142; // [esp+440h] [ebp-2F0h] BYREF
    Vec3 v143; // [esp+44Ch] [ebp-2E4h] BYREF
    float v144; // [esp+45Ch] [ebp-2D4h]
    int v145; // [esp+460h] [ebp-2D0h]
    Vec3 v146; // [esp+464h] [ebp-2CCh]
    Vec3 v147; // [esp+470h] [ebp-2C0h] BYREF
    Vec3 v148; // [esp+47Ch] [ebp-2B4h] BYREF
    float v149; // [esp+488h] [ebp-2A8h]
    float v150; // [esp+48Ch] [ebp-2A4h]
    int v151; // [esp+490h] [ebp-2A0h]
    int v152; // [esp+494h] [ebp-29Ch]
    int v153; // [esp+498h] [ebp-298h]
    Vec3 v154; // [esp+49Ch] [ebp-294h] BYREF
    Vec3 v155; // [esp+4A8h] [ebp-288h]
    Vec3 v156; // [esp+4B4h] [ebp-27Ch] BYREF
    Vec3 v157; // [esp+4C0h] [ebp-270h] BYREF
    Vec3 v158; // [esp+4CCh] [ebp-264h] BYREF
    Vec3 v159; // [esp+4D8h] [ebp-258h] BYREF
    Vec3 v160; // [esp+4E4h] [ebp-24Ch] BYREF
    int i; // [esp+4F4h] [ebp-23Ch]
    float v162; // [esp+4F8h] [ebp-238h]
    unsigned int v163; // [esp+4FCh] [ebp-234h]
    unsigned int v164; // [esp+500h] [ebp-230h]
    Vec3 v165; // [esp+510h] [ebp-220h] BYREF
    Vec3 v166; // [esp+51Ch] [ebp-214h] BYREF
    float v167; // [esp+534h] [ebp-1FCh]
    int v168; // [esp+538h] [ebp-1F8h]
    int v169; // [esp+540h] [ebp-1F0h]
    tGameObjectMain* v170; // [esp+544h] [ebp-1ECh]
    Vec3 v171; // [esp+548h] [ebp-1E8h] BYREF
    Vec3 v172; // [esp+554h] [ebp-1DCh] BYREF
    Vec3 v173; // [esp+560h] [ebp-1D0h] BYREF
    Vec3 pos; // [esp+56Ch] [ebp-1C4h] BYREF
    float v175; // [esp+578h] [ebp-1B8h]
    float v176; // [esp+57Ch] [ebp-1B4h]
    float v177; // [esp+580h] [ebp-1B0h]
    BOOL v178; // [esp+584h] [ebp-1ACh]
    float v179; // [esp+588h] [ebp-1A8h]
    int v180; // [esp+58Ch] [ebp-1A4h]
    int v181; // [esp+590h] [ebp-1A0h]
    int v182[10]; // [esp+594h] [ebp-19Ch]
    Vec3 v183; // [esp+5BCh] [ebp-174h] BYREF
    Vec3 a3; // [esp+5C8h] [ebp-168h] BYREF
    int v185; // [esp+5D4h] [ebp-15Ch]
    BOOL v186; // [esp+5DCh] [ebp-154h]
    float v187; // [esp+5E4h] [ebp-14Ch]
    Vec3 v188; // [esp+5E8h] [ebp-148h] BYREF
    Vec3 v189; // [esp+5F4h] [ebp-13Ch] BYREF
    Vec3 v190; // [esp+600h] [ebp-130h] BYREF
    Vec3 v191; // [esp+60Ch] [ebp-124h] BYREF
    float v192; // [esp+618h] [ebp-118h]
    float v193; // [esp+61Ch] [ebp-114h]
    float v194; // [esp+620h] [ebp-110h]
    Vec3 v195; // [esp+624h] [ebp-10Ch] BYREF
    Vec3 v196; // [esp+630h] [ebp-100h] BYREF
    float v197; // [esp+63Ch] [ebp-F4h]
    float v199; // [esp+644h] [ebp-ECh]
    unsigned int v200; // [esp+648h] [ebp-E8h]
    unsigned int v201; // [esp+64Ch] [ebp-E4h]
    float v202; // [esp+650h] [ebp-E0h]
    float v203; // [esp+654h] [ebp-DCh]
    float v204; // [esp+658h] [ebp-D8h]
    Vec3 v205; // [esp+668h] [ebp-C8h] BYREF
    unsigned int v206; // [esp+674h] [ebp-BCh]
    float v207; // [esp+678h] [ebp-B8h]
    float v208; // [esp+67Ch] [ebp-B4h]
    float v209; // [esp+684h] [ebp-ACh]
    Vec3 v210; // [esp+688h] [ebp-A8h] BYREF
    tScriptStruct_Laser* v211; // [esp+694h] [ebp-9Ch]
    float v212; // [esp+698h] [ebp-98h]
    float v213; // [esp+69Ch] [ebp-94h]
    int v214; // [esp+6A0h] [ebp-90h]
    tGameObjectMain* v215; // [esp+6A4h] [ebp-8Ch]
    Vec3 dest; // [esp+6A8h] [ebp-88h] BYREF
    Vec3 v217; // [esp+6B4h] [ebp-7Ch] BYREF
    Vec3 v218; // [esp+6C0h] [ebp-70h] BYREF
    int v219; // [esp+6CCh] [ebp-64h]
    tGameObjectMain* v220; // [esp+6D0h] [ebp-60h]
    float v221; // [esp+6D8h] [ebp-58h]
    float v222; // [esp+6DCh] [ebp-54h]
    Vec3 v224; // [esp+6E4h] [ebp-4Ch]
    Vec3 v225; // [esp+6FCh] [ebp-34h] BYREF
    Vec3 v226; // [esp+708h] [ebp-28h] BYREF
    Vec3 v227; // [esp+714h] [ebp-1Ch] BYREF
    int v228; // [esp+728h] [ebp-8h]
    float v229; // [esp+72Ch] [ebp-4h]

    v189.x = 1.0;
    v189.y = 0.0;
    v189.z = 0.0;
    v141.x = 1.0;
    v141.y = 0.0;
    v141.z = 0.0;
    v155.x = 1.0;
    v155.y = 0.0;
    v155.z = 0.0;
    v143.x = 1.0;
    v143.y = 0.0;
    v143.z = 0.0;
    pos.x = 1.0;
    pos.y = 0.0;
    pos.z = 0.0;
    v129.x = 1.0;
    v129.y = 0.0;
    v129.z = 0.0;
    v147.x = 1.0;
    v147.y = 0.0;
    v147.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v27 = (tScriptStruct_Laser*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v27 = 0;
    v211 = v27;
    gfxId = 0;
    v168 = 0;
    v212 = 0.0;
    v169 = 0;
    v144 = 0.0;
    i = 0;
    v213 = 0.0;
    v125 = 0;
    v214 = 0;
    v187 = 0.0;
    v222 = 0.0;
    v201 = 0;
    v215 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Laser_Laser_comportement_Laser_Laser_comportement = (int)AI_ul_RealResolveOneRefFunction(0x720008BAu);
        fct_Laser_Laser_comportement_Laser_Laser_tempo_destroy = (int)AI_ul_RealResolveOneRefFunction(0x26001103u);
        Laser_Laser_comportement_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Laser_Laser_comportement_GST_Global_gao_f_eau_hauteurZ,
            (void*)Laser_Laser_comportement_GST_Global_gao,
            2772);
        Laser_Laser_comportement_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Laser_Laser_comportement_univVars_Perso_ID_actor,
            (void*)Laser_Laser_comportement_univVars,
            988);
        Laser_Laser_comportement_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
        Laser_Laser_comportement_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        Laser_Laser_comportement_DUP_SFX_Sally_Shield_gao = AI_ul_RealResolveOneRefObject(0x260020F1u);
    }
    if (v211->i_flag_1st_trame)
    {
        v211->i_flag_1st_trame = 0;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    v215 = *AI_MainActor;
    if (v211->f_SND_play_dist != 0.0f)
    {
        v102 = v215;
        if (v215)
        {
            if ((unsigned int)v215 <= 2)
                v102 = AI_MainActor[(uint32_t)v215 - 1];
            v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v102);
        }
        else
        {
            v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
        }
        v208 = (float)v1;
        if (v211->f_SND_play_dist <= (double)v208)
        {
            if (v211->i_snd_req != -1)
            {
                SND_Release(v211->i_snd_req);
                v211->i_snd_req = -1;
            }
        }
        else if (v211->i_snd_req == -1)
        {
            v211->i_snd_req = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 1, 0x18000000);
            v124 = SND_l_GetFreq(v211->i_snd_req);
            v118 = AI_EvalFunc_MATHRandFloat_C(v211->f_freq_min, v211->f_freq_max);
            v26 = (float)v124;
            SND_SetFreq(v211->i_snd_req, (unsigned __int64)(v26 * v118));
            AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v211->i_snd_req);
        }
    }
    if (v211->i_flag_touche)
    {
        if (v211->i_gfx_ripple != -1)
        {
            AI_EvalFunc_GFXSeti_C_CURRENT(v211->i_gfx_ripple, 20100, -1);
            v211->i_gfx_ripple = -1;
        }
        if (v211->i_gfx_lightning != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v211->i_gfx_lightning);
            v211->i_gfx_lightning = -1;
        }
        if (v211->i_gfx_idhalo != -1)
            AI_EvalFunc_GFXDel_C_CURRENT(v211->i_gfx_idhalo);
        if (v211->i_snd_req != -1)
        {
            SND_Release(v211->i_snd_req);
            v211->i_snd_req = -1;
        }
        v170 = AI_gpst_CurrentGameObject;
        v101 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v101 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                v101,
                2,
                (tAIFunction*)fct_Laser_Laser_comportement_Laser_Laser_comportement);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_Laser_Laser_comportement_Laser_Laser_comportement);
        }
        AI_EvalFunc_AITrackChange_C(2, (tAIFunction*)fct_Laser_Laser_comportement_Laser_Laser_tempo_destroy);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    if (!AI_EvalFunc_TIMEElapsed_C(v211->f_time_last_shoot, v211->des_f_laser_life_time))
    {
        v97 = (tGameObjectMain*)v211->des_target;
        if (v97)
        {
            if ((unsigned int)v97 <= 2)
                v97 = AI_MainActor[(uint32_t)v97 - 1];
            v96 = v97;
        }
        else
        {
            v96 = 0;
        }
        if (v96)
        {
            if (v211->des_mode == 1)
            {
                v95 = (tGameObjectMain*)v211->des_target;
                if (v95)
                {
                    if ((unsigned int)v95 <= 2)
                        v95 = AI_MainActor[(uint32_t)v95 - 1];
                    AI_EvalFunc_OBJGetPos_C(v95, &a3);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &a3);
                }
                v129 = a3;
                v94 = (tGameObjectMain*)v211->des_target;
                if (v94)
                {
                    if ((unsigned int)v94 <= 2)
                        v94 = AI_MainActor[(uint32_t)v94 - 1];
                    AI_EvalFunc_DYNSpeedGetVector_C(v94, &v148);
                }
                else
                {
                    AI_EvalFunc_DYNSpeedGetVector_C(0, &v148);
                }
                v150 = TIM_gf_dt;
                v192 = v148.x * TIM_gf_dt;
                v193 = v148.y * TIM_gf_dt;
                v194 = v148.z * TIM_gf_dt;
                v129.x = v129.x + v192;
                v129.y = v129.y + v193;
                v129.z = v129.z + v194;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v121);
                MATH_SubstractVec(&dest, &v129, &v121);
                AI_EvalFunc_MATHVecNormalize_C(&dest, &v211->v_beam_sight);
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v210);
                v167 = v211->v_beam_sight.z * v210.z + v211->v_beam_sight.y * v210.y + v210.x * v211->v_beam_sight.x;
                if (v167 > 0.5)
                    AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v211->v_beam_sight);
            }
            else
            {
                v93 = (tGameObjectMain*)v211->des_target;
                if (v93)
                {
                    if ((unsigned int)v93 <= 2)
                        v93 = AI_MainActor[(uint32_t)v93 - 1];
                    AI_EvalFunc_OBJGetPos_C(v93, &v173);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v173);
                }
                v129 = v173;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v217);
                MATH_SubstractVec(&v137, &v129, &v217);
                AI_EvalFunc_MATHVecNormalize_C(&v137, &v211->v_beam_sight);
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v188);
                v117 = v211->v_beam_sight.z * v188.z + v211->v_beam_sight.y * v188.y + v188.x * v211->v_beam_sight.x;
                if (v117 > 0.5)
                    AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v211->v_beam_sight);
                v138 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x10000000, 0);
                v211->des_f_laser_speed = v211->des_f_laser_speed + 1.0f;
            }
        }
        v169 = 0;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v211->v_beam_sight);
        v141 = v211->v_last_beam_pos;
        v221 = TIM_gf_dt;
        v213 = v211->des_f_laser_speed * TIM_gf_dt;
        v175 = v213 * v211->v_beam_sight.x;
        v176 = v211->v_beam_sight.y * v213;
        v177 = v211->v_beam_sight.z * v213;
        v92 = &v211->v_last_beam_pos;
        v211->v_last_beam_pos.x = v175 + v211->v_last_beam_pos.x;
        v92->y = v176 + v92->y;
        v92->z = v177 + v92->z;
        if (v211->des_puissance == 8)
        {
            v211->objet_touche = 0;
        }
        else
        {
            if (v211->i_flag_factice)
            {
                v91 = v215;
                if (v215)
                {
                    if ((unsigned int)v215 <= 2)
                        v91 = AI_MainActor[(uint32_t)v215 - 1];
                    v2 = AI_EvalFunc_OBJ_FlagsControlGet_C(v91);
                }
                else
                {
                    v2 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
                }
                v163 = v2;
                v201 = v2 & 0x1000000;
                v90 = v215;
                if (v215)
                {
                    if ((unsigned int)v215 <= 2)
                        v90 = AI_MainActor[(uint32_t)v215 - 1];
                    v3 = AI_EvalFunc_OBJ_FlagsControlSet_C(v90, 0x1000000, 0);
                }
                else
                {
                    v3 = AI_EvalFunc_OBJ_FlagsControlSet_C(0, 0x1000000, 0);
                }
                v200 = v3;
            }
            for (i = 0; i < 10; ++i)
            {
                v89 = (tGameObjectMain*)v211->ao_traverse[i];
                if (v89)
                {
                    if ((unsigned int)v89 <= 2)
                        v89 = AI_MainActor[(uint32_t)v89 - 1];
                    v88 = v89;
                }
                else
                {
                    v88 = 0;
                }
                if (!v88)
                    break;
                v87 = (tGameObjectMain*)v211->ao_traverse[i];
                if (v87)
                {
                    if ((unsigned int)v87 <= 2)
                        v87 = AI_MainActor[(uint32_t)v87 - 1];
                    v4 = AI_EvalFunc_OBJ_FlagsControlGet_C(v87);
                }
                else
                {
                    v4 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
                }
                v140 = v4;
                v182[i] = v4 & 0x1000000;
                v86 = (tGameObjectMain*)v211->ao_traverse[i];
                if (v86)
                {
                    if ((unsigned int)v86 <= 2)
                        v86 = AI_MainActor[(uint32_t)v86 - 1];
                    v5 = AI_EvalFunc_OBJ_FlagsControlSet_C(v86, 0x1000000, 0);
                }
                else
                {
                    v5 = AI_EvalFunc_OBJ_FlagsControlSet_C(0, 0x1000000, 0);
                }
                v164 = v5;
            }
            AI_EvalFunc_COL_SpecificCrossableSet_C(-1 - (v211->des_test_flag_X & 0xffff));
            a4 = v211->des_f_laser_length + v213;
            v6 = (int)AI_EvalFunc_COLRayObjectDist_C(
                AI_gpst_CurrentGameObject,
                &v141,
                &v211->v_beam_sight,
                a4,
                v211->i_type_vrai,
                v211->i_type_faux,
                1,
                0x80000001);
            v211->objet_touche = v6;
            v85 = (tGameObjectMain*)v211->objet_touche;
            if (v85)
            {
                if ((unsigned int)v85 <= 2)
                    v85 = AI_MainActor[(uint32_t)v85 - 1];
                v84 = v85;
            }
            else
            {
                v84 = 0;
            }
            if (v84)
                v211->objet_touche = v211->objet_touche;
            v83 = v211->des_f_laser_length + v213;
            v202 = v83 * v211->v_beam_sight.x;
            v203 = v211->v_beam_sight.y * v83;
            v204 = v211->v_beam_sight.z * v83;
            if (v211->i_flag_factice && !v201)
            {
                v82 = v215;
                if (v215)
                {
                    if ((unsigned int)v215 <= 2)
                        v82 = AI_MainActor[(uint32_t)v215 - 1];
                    v7 = AI_EvalFunc_OBJ_FlagsControlSet_C(v82, 0, 0x1000000);
                }
                else
                {
                    v7 = AI_EvalFunc_OBJ_FlagsControlSet_C(0, 0, 0x1000000);
                }
                v131 = v7;
            }
            for (i = 0; i < 10; ++i)
            {
                v81 = (tGameObjectMain*)v211->ao_traverse[i];
                if (v81)
                {
                    if ((unsigned int)v81 <= 2)
                        v81 = AI_MainActor[(uint32_t)v81 - 1];
                    v80 = v81;
                }
                else
                {
                    v80 = 0;
                }
                if (!v80)
                    break;
                if (!v182[i])
                {
                    v79 = (tGameObjectMain*)v211->ao_traverse[i];
                    if (v79)
                    {
                        if ((unsigned int)v79 <= 2)
                            v79 = AI_MainActor[(uint32_t)v79 - 1];
                        v8 = AI_EvalFunc_OBJ_FlagsControlSet_C(v79, 0, 0x1000000);
                    }
                    else
                    {
                        v8 = AI_EvalFunc_OBJ_FlagsControlSet_C(0, 0, 0x1000000);
                    }
                    v206 = v8;
                }
            }
        }
        v78 = (tGameObjectMain*)v211->objet_touche;
        if (v78)
        {
            if ((unsigned int)v78 <= 2)
                v78 = AI_MainActor[(uint32_t)v78 - 1];
            v77 = v78;
        }
        else
        {
            v77 = 0;
        }
        if (!v77)
        {
            v76 = v211->des_f_laser_length + v213;
            v224.x = v76 * v211->v_beam_sight.x;
            v224.y = v211->v_beam_sight.y * v76;
            v224.z = v211->v_beam_sight.z * v76;
            v143 = v224;
            v129.x = v141.x + v224.x;
            v129.y = v141.y + v224.y;
            v129.z = v141.z + v224.z;
            if (v224.z < 0.0 && *(float*)Laser_Laser_comportement_GST_Global_gao_f_eau_hauteurZ + 10.0 > v129.z)
            {
                AI_EvalFunc_WTR_GetDif_C(&v129, &v136);
                v187 = v136.z;
                if (v129.z < (double)v136.z)
                {
                    v211->objet_touche = (int)AI_gpst_CurrentGameObject;
                    v169 = 1;
                    v135.x = 0.0;
                    v135.y = 0.0;
                    v135.z = 1.0;
                    v189.x = 0.0;
                    v189.y = 0.0;
                    v189.z = 1.0;
                    pos.x = v141.x;
                    pos.y = v141.y;
                    pos.z = v187;
                    v25 = (float)fabs(v143.z);
                    v144 = v25;
                    v143.z = 0.0;
                    v222 = MATH_VecLen(&v143);
                    if (v222 != 0.0f)
                    {
                        v24 = (float)fabs(v141.z - v187);
                        v207 = v24;
                        v75 = v24 / v144;
                        v143.x = v143.x * v75;
                        v143.y = v143.y * v75;
                        v143.z = v143.z * v75;
                        pos.x = pos.x + v143.x;
                        pos.y = pos.y + v143.y;
                        pos.z = pos.z + v143.z;
                    }
                }
            }
        }
        v74 = (tGameObjectMain*)v211->objet_touche;
        if (v74)
        {
            if ((unsigned int)v74 <= 2)
                v74 = AI_MainActor[(uint32_t)v74 - 1];
            v73 = v74;
        }
        else
        {
            v73 = 0;
        }
        if (!v73)
        {
            GFXSetCurGlobal(v211->i_gfx_lightning);
            AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &v211->v_last_beam_pos);
            v29 = v211->des_f_laser_length;
            v160.x = v29 * v211->v_beam_sight.x;
            v160.y = v211->v_beam_sight.y * v29;
            v160.z = v211->v_beam_sight.z * v29;
            MATH_VecAddVec(&v134, &v211->v_last_beam_pos, &v160);
            AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &v134);
            v28 = v211->des_f_laser_length * 0.89999998f;
            v104.x = v28 * v211->v_beam_sight.x;
            v104.y = v211->v_beam_sight.y * v28;
            v104.z = v211->v_beam_sight.z * v28;
            MATH_VecAddVec(&v205, &v211->v_last_beam_pos, &v104);
            AI_EvalFunc_GFXSetv_C_CURRENT(v211->i_gfx_idhalo, 21200, &v205);
        LABEL_342:
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v211->v_last_beam_pos);
            return 1000;
        }
        v72 = (tGameObjectMain*)v211->o_acteur_a_informer;
        if (v72)
        {
            if ((unsigned int)v72 <= 2)
                v72 = AI_MainActor[(uint32_t)v72 - 1];
            v71 = v72;
        }
        else
        {
            v71 = 0;
        }
        if (v71)
        {
            v211->m_msg.msgID = 77;
            v211->m_msg.gao2 = (tGameObjectMain*)v211->objet_touche;
            v70 = (tGameObjectMain*)v211->o_acteur_a_informer;
            if (v70)
            {
                if ((unsigned int)v70 <= 2)
                    v70 = AI_MainActor[(uint32_t)v70 - 1];
                AI_EvalFunc_MSGSend_C(v70, &v211->m_msg);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &v211->m_msg);
            }
        }
        v69 = (tGameObjectMain*)v211->objet_touche;
        if (v69)
        {
            if ((unsigned int)v69 <= 2)
                v69 = AI_MainActor[(uint32_t)v69 - 1];
            v68 = v69;
        }
        else
        {
            v68 = 0;
        }
        if (v68 != *(tGameObjectMain**)(Laser_Laser_comportement_univVars_Perso_ID_actor + 40))
            goto LABEL_192;
        v65 = (tAI*)v211->objet_touche;
        if (v65 == WOR_gst_Universe.univAIInstance)
        {
            v64 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v67 = (tGameObjectMain*)v211->objet_touche;
            if (v65)
            {
                if ((unsigned int)v65 <= 2)
                    v67 = AI_MainActor[(uint32_t)v65 - 1];
                v66 = v67;
            }
            else
            {
                v66 = 0;
            }
            v64 = v66->extended->AIInstance;
        }
        if (AI_EvalFunc_TIMEElapsed_C(((tScriptStruct_PJAircraft*)(v64->instanceVarDataCurrent))->f_time_start_mode, 0.5))
            LABEL_192:
        v214 = 1;
        else
            v214 = 0;
        if (v211->des_puissance != 8)
        {
            v63 = (tGameObjectMain*)v211->objet_touche;
            if (v63)
            {
                if ((unsigned int)v63 <= 2)
                    v63 = AI_MainActor[(uint32_t)v63 - 1];
                v9 = AI_EvalFunc_OBJ_CapaTest_C(v63, 0x200);
            }
            else
            {
                v9 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x200);
            }
            v178 = v9;
            if (!v9)
            {
                if (v214)
                {
                    v62 = (tGameObjectMain*)v211->o_gao_axis_paf;
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
                    if (v61)
                    {
                        v60 = (tGameObjectMain*)v211->o_gao_axis_paf;
                        if (v60)
                        {
                            if ((unsigned int)v60 <= 2)
                                v60 = AI_MainActor[(uint32_t)v60 - 1];
                            AI_EvalFunc_OBJGetPos_C(v60, &v128);
                        }
                        else
                        {
                            AI_EvalFunc_OBJGetPos_C(0, &v128);
                        }
                        v59 = (tGameObjectMain*)v211->objet_touche;
                        if (v59)
                        {
                            if ((unsigned int)v59 <= 2)
                                v59 = AI_MainActor[(uint32_t)v59 - 1];
                            AI_EvalFunc_OBJGetPos_C(v59, &v171);
                        }
                        else
                        {
                            AI_EvalFunc_OBJGetPos_C(0, &v171);
                        }
                        MATH_SubstractVec(&v103, &v128, &v171);
                        v155 = v103;
                    }
                    else
                    {
                        v155 = v211->v_beam_sight;
                    }
                    if (v211->des_paf_chaleur)
                    {
                        v211->m_msg.msgID = 37;
                        v211->m_msg.gao = AI_gpst_CurrentGameObject;
                        v211->m_msg.gao2 = 0;
                        v211->m_msg.Int1 = v211->des_puissance + 40;
                        v211->m_msg.Int2 = 0;
                        v211->m_msg.v1 = v155;
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&v211->m_msg.v2);
                        v211->m_msg.f2 = (float)v211->des_f_point_de_vie;
                        v211->m_msg.f1 = 1000.0;
                        v58 = (tGameObjectMain*)v211->objet_touche;
                        if (v58)
                        {
                            if ((unsigned int)v58 <= 2)
                                v58 = AI_MainActor[(uint32_t)v58 - 1];
                            AI_EvalFunc_MSGSend_C(v58, &v211->m_msg);
                            goto LABEL_229;
                        }
                    }
                    else
                    {
                        v211->m_msg.msgID = 37;
                        v211->m_msg.gao = AI_gpst_CurrentGameObject;
                        v211->m_msg.gao2 = 0;
                        v211->m_msg.Int1 = v211->des_puissance + 8;
                        v211->m_msg.Int2 = 0;
                        v211->m_msg.v1 = v155;
                        AI_EvalFunc_OBJGetPos_C_CURRENT(&v211->m_msg.v2);
                        v211->m_msg.f2 = (float)v211->des_f_point_de_vie;
                        v211->m_msg.f1 = 1000.0;
                        v57 = (tGameObjectMain*)v211->objet_touche;
                        if (v57)
                        {
                            if ((unsigned int)v57 <= 2)
                                v57 = AI_MainActor[(uint32_t)v57 - 1];
                            AI_EvalFunc_MSGSend_C(v57, &v211->m_msg);
                            goto LABEL_229;
                        }
                    }
                    AI_EvalFunc_MSGSend_C(0, &v211->m_msg);
                }
            }
        }
    LABEL_229:
        if (!v169)
        {
            v56 = (tGameObjectMain*)v211->objet_touche;
            if (v56)
            {
                if ((unsigned int)v56 <= 2)
                    v56 = AI_MainActor[(uint32_t)v56 - 1];
                v10 = AI_EvalFunc_COLGMFlagsGet_C(v56, 0xFFFF);
            }
            else
            {
                v10 = AI_EvalFunc_COLGMFlagsGet_C(0, 0xFFFF);
            }
            v108 = v10;
            v169 = v10 & 0x10;
            AI_EvalFunc_COLRayPosGet_C(AI_gpst_CurrentGameObject, &v154);
            pos = v154;
            AI_EvalFunc_COLRayNormalGet_C(AI_gpst_CurrentGameObject, &v105);
            v189 = v105;
            if (AI2C_VecEqNull(&pos))
            {
                v55 = (tGameObjectMain*)v211->objet_touche;
                if (v55)
                {
                    if ((unsigned int)v55 <= 2)
                        v55 = AI_MainActor[(uint32_t)v55 - 1];
                    AI_EvalFunc_OBJGetPos_C(v55, &v157);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v157);
                }
                pos = v157;
            }
            v54 = (tGameObjectMain*)v211->objet_touche;
            if (v54)
            {
                if ((unsigned int)v54 <= 2)
                    v54 = AI_MainActor[(uint32_t)v54 - 1];
                v11 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v54, 4);
            }
            else
            {
                v11 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 4);
            }
            v186 = v11;
            if (v11)
            {
                v53 = (tGameObjectMain*)v211->objet_touche;
                if (v53)
                {
                    if ((unsigned int)v53 <= 2)
                        v53 = AI_MainActor[(uint32_t)v53 - 1];
                    AI_EvalFunc_DYNSpeedGetVector_C(v53, &v190);
                }
                else
                {
                    AI_EvalFunc_DYNSpeedGetVector_C(0, &v190);
                }
                v209 = TIM_gf_dt;
                v113 = v190.x * TIM_gf_dt;
                v114 = v190.y * TIM_gf_dt;
                v115 = v190.z * TIM_gf_dt;
                pos.x = pos.x + v113;
                pos.y = pos.y + v114;
                pos.z = pos.z + v115;
            }
        }
        if (v169)
        {
            v125 = (tAI*)AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)Laser_Laser_comportement_SFX_explosion_gao, &pos);
            Src.msgID = 1;
            Src.Int1 = 7;
            Src.f2 = 30.0;
            Src.f1 = 60.0;
            Src.v1.x = 0.25;
            Src.v1.y = 0.0;
            Src.v1.z = -1.0e14f;
            v52 = (tGameObjectMain*)v125;
            if (v125)
            {
                if ((unsigned int)v125 <= 2)
                    v52 = AI_MainActor[(uint32_t)v125 - 1];
                AI_EvalFunc_MSGSend_C(v52, &Src);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &Src);
            }
            v51 = (tGameObjectMain*)v125;
            if (v125)
            {
                if ((unsigned int)v125 <= 2)
                    v51 = AI_MainActor[(uint32_t)v125 - 1];
                AI_EvalFunc_OBJ_SightSet_C(v51, &v189);
            }
            else
            {
                AI_EvalFunc_OBJ_SightSet_C(0, &v189);
            }
            if (v125 == WOR_gst_Universe.univAIInstance)
            {
                v48 = WOR_gst_Universe.univAIInstance;
            }
            else
            {
                v50 = (tGameObjectMain*)v125;
                if (v125)
                {
                    if ((unsigned int)v125 <= 2)
                        v50 = AI_MainActor[(uint32_t)v125 - 1];
                    v49 = v50;
                }
                else
                {
                    v49 = 0;
                }
                v48 = v49->extended->AIInstance;
            }
            ((tScriptStruct_PJAircraft*)(v48->instanceVarDataCurrent))->des_f_shoot_delay_min = 2.3509886e-38f;
        }
        else
        {
            v211->spark = AI_EvalFunc_GFXAdd_C_CURRENT(9);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v211->spark, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v211->spark, 2, 1);
            v47 = (tGameObjectMain*)Laser_Laser_comportement_SFX_light_smoke_gao;
            if (Laser_Laser_comportement_SFX_light_smoke_gao)
            {
                if ((unsigned int)Laser_Laser_comportement_SFX_light_smoke_gao <= 2)
                    v47 = AI_MainActor[Laser_Laser_comportement_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v211->spark, v47, 5);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v211->spark, 0, 5);
            }
            GFXSetCurGlobal(v211->spark);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9000, 1.570796f);
            X_4a = v211->des_f_laser_speed / 12.0f;
            AI_EvalFunc_GFXSetf_C_CURGLOB(9001, X_4a);
            X_4b = v211->des_f_laser_speed / 8.0f;
            AI_EvalFunc_GFXSetf_C_CURGLOB(9002, X_4b);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9003, 0.98000002f);
            X_4c = v211->des_f_laser_width / 2.0f;
            AI_EvalFunc_GFXSetf_C_CURGLOB(9004, X_4c);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9005, 0.2f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9006, 0.40000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9007, 2.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9008, 0.0049999999f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9009, 0.2f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9010, 0.40000001f);
            AI_EvalFunc_GFXSeti_C_CURGLOB(9100, 10);
            v180 = AI_EvalFunc_COLORBlend_C(v211->des_c_color, -1, 0.5);
            AI_EvalFunc_GFXSeti_C_CURGLOB(9101, v180);
            AI_EvalFunc_GFXSeti_C_CURGLOB(9102, 24);
            AI_EvalFunc_GFXSetv_C_CURGLOB(9200, &pos);
            AI_EvalFunc_GFXSetv_C_CURGLOB(9201, &v189);
            v120.x = 0.0;
            v120.y = 0.0;
            v120.z = -2.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(9202, &v120);
        }
        gfxId = AI_EvalFunc_GFXAdd_C_CURRENT(21);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(gfxId, 4, 1);
        v46 = (tGameObjectMain*)Laser_Laser_comportement_SFX_light_smoke_gao;
        if (Laser_Laser_comportement_SFX_light_smoke_gao)
        {
            if ((unsigned int)Laser_Laser_comportement_SFX_light_smoke_gao <= 2)
                v46 = AI_MainActor[Laser_Laser_comportement_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(gfxId, v46, 0);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(gfxId, 0, 0);
        }
        GFXSetCurGlobal(gfxId);
        X_4d = v211->des_f_laser_width + v211->des_f_laser_width;
        AI_EvalFunc_GFXSetf_C_CURGLOB(21002, X_4d);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
        v107 = AI_EvalFunc_COLORBlend_C(v211->des_c_color, -1, 0.5);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v107);
        X_4e = v211->des_f_laser_width * 4.0f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(21000, X_4e);
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &pos);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21005, 0.1f);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21100, 16);
        AI_EvalFunc_GFXLifeTimeSet_C(AI_gpst_CurrentGameObject, gfxId, 0.1f);
        v211->f_time_spark = TIM_gf_MainClock - TIM_gf_LockTime;
        if (!v211->des_shield_rebound)
            goto LABEL_315;
        v45 = (tGameObjectMain*)v211->objet_touche;
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
        if (v44 == (tGameObjectMain*)Laser_Laser_comportement_DUP_SFX_Sally_Shield_gao)
        {
            v119 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(3, -1.0);
            v211->v_last_beam_pos = pos;
            v147 = v189;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v183);
            v197 = v183.z * v147.z + v183.y * v147.y + v147.x * v183.x;
            v23 = (float)fabs(v197);
            v116 = v23;
            v212 = v23;
            if (v23 <= 0.98480803f)
            {
                v43 = 2.0f * v212;
                v147.x = v147.x * v43;
                v147.y = v147.y * v43;
                v147.z = v147.z * v43;
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v165);
                v147.x = v147.x + v165.x;
                v147.y = v147.y + v165.y;
                v147.z = v147.z + v165.z;
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v122);
                v146.x = -v122.x;
                v146.y = -v122.y;
                v146.z = -v122.z;
                v147.x = -v122.x;
                v147.y = -v122.y;
                v147.z = -v122.z;
            }
            AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v147);
            v211->v_beam_sight = v147;
        }
        else
        {
        LABEL_315:
            if (v211->i_rebound_nb >= v211->des_i_rebound_nb || !v211->des_water_rebound && v169)
                goto LABEL_316;
            v42 = (tGameObjectMain*)v211->objet_touche;
            if (v42)
            {
                if ((unsigned int)v42 <= 2)
                    v42 = AI_MainActor[(uint32_t)v42 - 1];
                v12 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v42, 2);
            }
            else
            {
                v12 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 2);
            }
            v110 = v12;
            if (v12)
                goto LABEL_316;
            v109 = *AI_MainActor;
            v41 = (tGameObjectMain*)v211->objet_touche;
            if (v41)
            {
                if ((unsigned int)v41 <= 2)
                    v41 = AI_MainActor[(uint32_t)v41 - 1];
                v40 = v41;
            }
            else
            {
                v40 = 0;
            }
            v39 = v109;
            if (v109)
            {
                if ((unsigned int)v109 <= 2)
                    v39 = AI_MainActor[(uint32_t)v109 - 1];
                v38 = v39;
            }
            else
            {
                v38 = 0;
            }
            if (v40 == v38)
            {
            LABEL_316:
                v211->i_flag_touche = 1;
                v37 = (tGameObjectMain*)v211->des_residu;
                if (v37)
                {
                    if ((unsigned int)v37 <= 2)
                        v37 = AI_MainActor[(uint32_t)v37 - 1];
                    v36 = v37;
                }
                else
                {
                    v36 = 0;
                }
                if (v36)
                {
                    if (!v211->des_residu_mode)
                        goto LABEL_329;
                    v35 = (tGameObjectMain*)v211->objet_touche;
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
                    if (v34 == (tGameObjectMain*)Laser_Laser_comportement_DUP_SFX_Sally_Shield_gao)
                    {
                    LABEL_329:
                        AI_EvalFunc_COLNormalGet_C(AI_gpst_CurrentGameObject, 0xFFFF, &v166);
                        v151 = 0;
                        v152 = 0;
                        v153 = 1065353216;
                        v229 = 1.0f * v166.z + 0.0f * v166.y + v166.x * 0.0f;
                        if (v229 < 0.5)
                        {
                            v218.x = 0.0;
                            v218.y = 0.0;
                            v218.z = 0.2f;
                            MATH_VecAddVec(&v130, &pos, &v218);
                            v33 = (tGameObjectMain*)v211->des_residu;
                            if (v33)
                            {
                                if ((unsigned int)v33 <= 2)
                                    v33 = AI_MainActor[(uint32_t)v33 - 1];
                                v13 = AI_EvalFunc_OBJDuplicate_C(v33, &v130);
                            }
                            else
                            {
                                v13 = AI_EvalFunc_OBJDuplicate_C(0, &v130);
                            }
                            v220 = v13;
                        }
                    }
                }
                if (v211->i_snd_req != -1)
                    SND_Release(v211->i_snd_req);
                v211->i_snd_req = -1;
                v168 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(2, -1.0);
                v149 = SND_f_GetVol(v168);
                v133 = AI_EvalFunc_MATHRandFloat_C(v211->snd_volmin, v211->snd_volmax);
                X_4f = v149 * v133;
                SND_SetVol(v168, X_4f);
                v32 = v211->des_f_laser_length;
                v195.x = v32 * v211->v_beam_sight.x;
                v195.y = v211->v_beam_sight.y * v32;
                v195.z = v211->v_beam_sight.z * v32;
                MATH_SubstractVec(&v156, &pos, &v195);
                GFXSetCurGlobal(v211->i_gfx_lightning);
                AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &v156);
                AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &pos);
                v211->v_last_beam_pos = pos;
            }
            else
            {
                v132 = AI_EvalFunc_SNDRequestPlay_C_CURRENT(3, -1.0);
                v211->v_last_beam_pos = pos;
                v147 = v189;
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v227);
                v199 = v227.z * v147.z + v227.y * v147.y + v147.x * v227.x;
                v22 = (float)fabs(v199);
                v162 = v22;
                v212 = v22;
                v31 = 2.0f * v22;
                v147.x = v147.x * v31;
                v147.y = v147.y * v31;
                v147.z = v147.z * v31;
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v126);
                v147.x = v147.x + v126.x;
                v147.y = v147.y + v126.y;
                v147.z = v147.z + v126.z;
                AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v147);
                v211->v_beam_sight = v147;
                GFXSetCurGlobal(v211->i_gfx_lightning);
                AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &pos);
                v30 = v211->des_f_laser_length;
                v226.x = v30 * v211->v_beam_sight.x;
                v226.y = v211->v_beam_sight.y * v30;
                v226.z = v211->v_beam_sight.z * v30;
                MATH_VecAddVec(&v191, &pos, &v226);
                AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &v191);
            }
        }
        ++v211->i_rebound_nb;
        goto LABEL_342;
    }
    if (v211->i_snd_req != -1)
        SND_Release(v211->i_snd_req);
    v211->i_snd_req = -1;
    v139 = TIM_gf_dt;
    X_4 = 5.0f * TIM_gf_dt;
    v185 = AI_EvalFunc_COLORBlend_C(v211->des_c_color, 0, X_4);
    v211->des_c_color = v185;
    v219 = AI_EvalFunc_COLORGetRed_C(v211->des_c_color);
    if (v219 <= 10
        && (v145 = AI_EvalFunc_COLORGetGreen_C(v211->des_c_color), v145 <= 10)
        && (v181 = AI_EvalFunc_COLORGetBlue_C(v211->des_c_color), v181 <= 10))
    {
        if (v211->i_gfx_lightning != -1)
            AI_EvalFunc_GFXDel_C_CURRENT(v211->i_gfx_lightning);
        if (v211->i_gfx_idhalo != -1)
            AI_EvalFunc_GFXDel_C_CURRENT(v211->i_gfx_idhalo);
        if (v211->i_gfx_ripple != -1)
            AI_EvalFunc_GFXSeti_C_CURRENT(v211->i_gfx_ripple, 20100, -1);
        if (v211->des_dont_destroy)
        {
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
            AI_EvalFunc_OBJ_FlagInactiveSet_C(AI_gpst_CurrentGameObject, 1);
        }
        else
        {
            v228 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v228 != 1000)
                return v228;
        }
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    else
    {
        if (v211->i_gfx_lightning != -1)
            AI_EvalFunc_GFXSeti_C_CURRENT(v211->i_gfx_lightning, 1100, v211->des_c_color);
        if (v211->i_gfx_idhalo != -1)
            AI_EvalFunc_GFXSeti_C_CURRENT(v211->i_gfx_idhalo, 21101, v211->des_c_color);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v127);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v225);
        v179 = TIM_gf_dt;
        v100 = v211->des_f_laser_speed * TIM_gf_dt;
        v142.x = v225.x * v100;
        v142.y = v225.y * v100;
        v142.z = v225.z * v100;
        MATH_VecAddVec(&v159, &v127, &v142);
        v211->v_last_beam_pos = v159;
        AI_EvalFunc_OBJSetPos_C_CURRENT(&v211->v_last_beam_pos);
        GFXSetCurGlobal(v211->i_gfx_lightning);
        AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &v211->v_last_beam_pos);
        v99 = v211->des_f_laser_length;
        v112.x = v99 * v211->v_beam_sight.x;
        v112.y = v211->v_beam_sight.y * v99;
        v112.z = v211->v_beam_sight.z * v99;
        MATH_VecAddVec(&v158, &v211->v_last_beam_pos, &v112);
        AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &v158);
        v98 = v211->des_f_laser_length * 0.89999998f;
        v196.x = v98 * v211->v_beam_sight.x;
        v196.y = v211->v_beam_sight.y * v98;
        v196.z = v211->v_beam_sight.z * v98;
        MATH_VecAddVec(&v172, &v211->v_last_beam_pos, &v196);
        AI_EvalFunc_GFXSetv_C_CURRENT(v211->i_gfx_idhalo, 21200, &v172);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}