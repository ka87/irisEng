#include "ai_compiledscripts_inc.h"

//Hovercraft_shoot_HoverShoot_init
int fct_Hovercraft_shoot_HoverShoot_init_Hovercraft_shoot_HoverShoot_init;
int fctvar_Hovercraft_shoot_HoverShoot_init_UNKNOWN1;
int fctvar_Hovercraft_shoot_HoverShoot_init_UNKNOWN2;
int fctvar_Hovercraft_shoot_HoverShoot_init_Switch_s1;
int Hovercraft_shoot_HoverShoot_init_univVars;
int Hovercraft_shoot_HoverShoot_init_univVars_Perso_ID_actor;
int Hovercraft_shoot_HoverShoot_init_SFX_light_smoke_gao;
int fct_Hovercraft_shoot_HoverShoot_init_Hovercraft_shoot_HoverShoot_move;
int fct_Hovercraft_shoot_HoverShoot_init_Hovercraft_shoot_HoverShoot_reflex;

//Hovercraft_shoot_HoverShoot_move
int Hovercraft_shoot_HoverShoot_move_univVars;
int Hovercraft_shoot_HoverShoot_move_univVars_world_ID;
int fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_exec_impact;
int fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_move;
int fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_death;
int Hovercraft_shoot_HoverShoot_move_SFX_explosion_gao;

//Hovercraft_shoot_HoverShoot_reflex
int fct_Hovercraft_shoot_HoverShoot_reflex_Hovercraft_shoot_HoverShoot_move;
int fct_Hovercraft_shoot_HoverShoot_reflex_Hovercraft_shoot_HoverShoot_death;

//Hovercraft_shoot_HoverShoot_exec_impact
int fct_Hovercraft_shoot_HoverShoot_exec_impact_Hovercraft_shoot_HoverShoot_exec_GFX_Flash;
int Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao;;

//Hovercraft_shoot_HoverShoot_exec_GFX_Flash
int Hovercraft_shoot_HoverShoot_exec_GFX_Flash_SFX_light_smoke_gao;

int Hovercraft_shoot_HoverShoot_init()
{
    tGameObjectMain* v0; // eax
    int result; // eax
    BOOL v2; // eax
    BOOL v3; // eax
    BOOL v4; // eax
    double v5; // st7
    tGameObjectMain* v6; // eax
    int v7; // eax
    BOOL v8; // eax
    int v9; // eax
    float v10; // [esp+4h] [ebp-468h]
    float X; // [esp+8h] [ebp-464h]
    float X_4; // [esp+Ch] [ebp-460h]
    float v13; // [esp+10h] [ebp-45Ch]
    tScriptStruct_HoverCraftShoot* v14; // [esp+14h] [ebp-458h]
    tGameObjectMain* v15; // [esp+1Ch] [ebp-450h]
    tGameObjectMain* v16; // [esp+24h] [ebp-448h]
    Vec3* v17; // [esp+28h] [ebp-444h]
    Vec3* v18; // [esp+2Ch] [ebp-440h]
    tGameObjectMain* v19; // [esp+34h] [ebp-438h]
    tGameObjectMain* v20; // [esp+3Ch] [ebp-430h]
    tGameObjectMain* v21; // [esp+44h] [ebp-428h]
    tGameObjectMain* v22; // [esp+4Ch] [ebp-420h]
    tGameObjectMain* v23; // [esp+54h] [ebp-418h]
    tGameObjectMain* v24; // [esp+5Ch] [ebp-410h]
    tGameObjectMain* v25; // [esp+60h] [ebp-40Ch]
    tGameObjectMain* v26; // [esp+64h] [ebp-408h]
    tGameObjectMain* v27; // [esp+68h] [ebp-404h]
    tGameObjectMain* v28; // [esp+6Ch] [ebp-400h]
    tGameObjectMain* v29; // [esp+74h] [ebp-3F8h]
    tGameObjectMain* v30; // [esp+78h] [ebp-3F4h]
    tGameObjectMain* v31; // [esp+7Ch] [ebp-3F0h]
    tGameObjectMain* v32; // [esp+80h] [ebp-3ECh]
    tGameObjectMain* v33; // [esp+84h] [ebp-3E8h]
    tGameObjectMain* v34; // [esp+8Ch] [ebp-3E0h]
    tGameObjectMain* v35; // [esp+90h] [ebp-3DCh]
    tGameObjectMain* v36; // [esp+94h] [ebp-3D8h]
    float v37; // [esp+A0h] [ebp-3CCh]
    tGameObjectMain* v38; // [esp+A4h] [ebp-3C8h]
    tGameObjectMain* v39; // [esp+A8h] [ebp-3C4h]
    tGameObjectMain* v40; // [esp+ACh] [ebp-3C0h]
    tGameObjectMain* v41; // [esp+B0h] [ebp-3BCh]
    tGameObjectMain* v42; // [esp+B8h] [ebp-3B4h]
    tGameObjectMain* v43; // [esp+C0h] [ebp-3ACh]
    tGameObjectMain* v44; // [esp+C8h] [ebp-3A4h]
    tGameObjectMain* v45; // [esp+D0h] [ebp-39Ch]
    tGameObjectMain* v46; // [esp+D4h] [ebp-398h]
    tGameObjectMain* v47; // [esp+D8h] [ebp-394h]
    tGameObjectMain* v48; // [esp+DCh] [ebp-390h]
    tGameObjectMain* v49; // [esp+E0h] [ebp-38Ch]
    tGameObjectMain* v50; // [esp+F0h] [ebp-37Ch]
    tGameObjectMain* v51; // [esp+F4h] [ebp-378h]
    tGameObjectMain* v52; // [esp+F8h] [ebp-374h]
    tGameObjectMain* v53; // [esp+FCh] [ebp-370h]
    tGameObjectMain* v54; // [esp+100h] [ebp-36Ch]
    tGameObjectMain* v55; // [esp+110h] [ebp-35Ch]
    tGameObjectMain* v56; // [esp+11Ch] [ebp-350h]
    tGameObjectMain* v57; // [esp+120h] [ebp-34Ch]
    tGameObjectMain* v58; // [esp+130h] [ebp-33Ch]
    tGameObjectMain* v59; // [esp+138h] [ebp-334h]
    tGameObjectMain* v60; // [esp+148h] [ebp-324h]
    tGameObjectMain* v61; // [esp+150h] [ebp-31Ch]
    tGameObjectMain* v62; // [esp+158h] [ebp-314h]
    tGameObjectMain* v63; // [esp+168h] [ebp-304h]
    tGameObjectMain* v64; // [esp+16Ch] [ebp-300h]
    tGameObjectMain* v65; // [esp+170h] [ebp-2FCh]
    tGameObjectMain* v66; // [esp+174h] [ebp-2F8h]
    tGameObjectMain* v67; // [esp+178h] [ebp-2F4h]
    tGameObjectMain* v68; // [esp+184h] [ebp-2E8h]
    tGameObjectMain* v69; // [esp+194h] [ebp-2D8h]
    tGameObjectMain* v70; // [esp+1A4h] [ebp-2C8h]
    tGameObjectMain* v71; // [esp+1ACh] [ebp-2C0h]
    tGameObjectMain* v72; // [esp+1B4h] [ebp-2B8h]
    tGameObjectMain* v73; // [esp+1BCh] [ebp-2B0h]
    tGameObjectMain* v74; // [esp+1C0h] [ebp-2ACh]
    tGameObjectMain* v75; // [esp+1C4h] [ebp-2A8h]
    tGameObjectMain* v76; // [esp+1CCh] [ebp-2A0h]
    tGameObjectMain* v77; // [esp+1D4h] [ebp-298h]
    tGameObjectMain* v78; // [esp+1D8h] [ebp-294h]
    tGameObjectMain* v79; // [esp+1DCh] [ebp-290h]
    tGameObjectMain* v80; // [esp+1E4h] [ebp-288h]
    Vec3 v81; // [esp+1E8h] [ebp-284h] BYREF
    Vec3 v82; // [esp+1F4h] [ebp-278h] BYREF
    Vec3 v83; // [esp+200h] [ebp-26Ch] BYREF
    Vec3 v84; // [esp+210h] [ebp-25Ch] BYREF
    Vec3 v85; // [esp+21Ch] [ebp-250h] BYREF
    BOOL v86; // [esp+228h] [ebp-244h]
    float v87; // [esp+238h] [ebp-234h]
    Vec3 v88; // [esp+23Ch] [ebp-230h] BYREF
    Vec3 v89; // [esp+254h] [ebp-218h]
    float v90; // [esp+260h] [ebp-20Ch]
    Vec3 v91; // [esp+270h] [ebp-1FCh] BYREF
    Vec3 v92; // [esp+27Ch] [ebp-1F0h] BYREF
    Vec3 v93; // [esp+288h] [ebp-1E4h] BYREF
    Vec3 v94; // [esp+294h] [ebp-1D8h] BYREF
    unsigned int v95; // [esp+2B0h] [ebp-1BCh]
    Vec3 v96; // [esp+2B4h] [ebp-1B8h] BYREF
    Vec3 v97; // [esp+2C8h] [ebp-1A4h] BYREF
    Vec3 v98; // [esp+2D4h] [ebp-198h] BYREF
    int v99; // [esp+2E0h] [ebp-18Ch]
    Vec3 v100; // [esp+2E4h] [ebp-188h] BYREF
    Vec3 v101; // [esp+2F0h] [ebp-17Ch] BYREF
    float v102; // [esp+2FCh] [ebp-170h]
    float v103; // [esp+300h] [ebp-16Ch]
    float v104; // [esp+304h] [ebp-168h]
    float v105; // [esp+308h] [ebp-164h]
    float v106; // [esp+30Ch] [ebp-160h]
    float v107; // [esp+310h] [ebp-15Ch]
    tGameObjectMain* v108; // [esp+314h] [ebp-158h]
    Vec3 v109; // [esp+31Ch] [ebp-150h] BYREF
    Vec3 v110; // [esp+328h] [ebp-144h] BYREF
    int v111; // [esp+334h] [ebp-138h]
    BOOL v112; // [esp+338h] [ebp-134h]
    Vec3 v113; // [esp+33Ch] [ebp-130h] BYREF
    Vec3 a3; // [esp+348h] [ebp-124h] BYREF
    Vec3 v115; // [esp+354h] [ebp-118h] BYREF
    Vec3 v116; // [esp+360h] [ebp-10Ch] BYREF
    Vec3 v117; // [esp+36Ch] [ebp-100h] BYREF
    Vec3 v118; // [esp+378h] [ebp-F4h] BYREF
    int v119; // [esp+384h] [ebp-E8h]
    int v120; // [esp+38Ch] [ebp-E0h]
    Vec3 v121; // [esp+390h] [ebp-DCh] BYREF
    int v122; // [esp+3A0h] [ebp-CCh]
    Vec3 v123; // [esp+3A4h] [ebp-C8h] BYREF
    Vec3 v124; // [esp+3B0h] [ebp-BCh] BYREF
    Vec3 a1; // [esp+3BCh] [ebp-B0h] BYREF
    Vec3 v126; // [esp+3CCh] [ebp-A0h] BYREF
    Vec3 dest; // [esp+3D8h] [ebp-94h] BYREF
    float v128; // [esp+3E8h] [ebp-84h]
    float v129; // [esp+3ECh] [ebp-80h]
    Vec3 v130; // [esp+3F0h] [ebp-7Ch] BYREF
    tScriptStruct_HoverCraftShoot* v131; // [esp+3FCh] [ebp-70h]
    BOOL v132; // [esp+400h] [ebp-6Ch]
    float v133; // [esp+404h] [ebp-68h]
    float v134; // [esp+408h] [ebp-64h]
    Vec3 v135; // [esp+40Ch] [ebp-60h]
    Vec3 v136; // [esp+418h] [ebp-54h] BYREF
    Vec3 v137; // [esp+424h] [ebp-48h] BYREF
    Vec3 v138; // [esp+430h] [ebp-3Ch] BYREF
    BOOL v139; // [esp+440h] [ebp-2Ch]
    Vec3 v140; // [esp+444h] [ebp-28h] BYREF
    float v141; // [esp+450h] [ebp-1Ch]
    float v142; // [esp+454h] [ebp-18h]
    float v143; // [esp+458h] [ebp-14h]
    int v144; // [esp+45Ch] [ebp-10h]

    v140.x = 1.0;
    v140.y = 0.0;
    v140.z = 0.0;
    v135.x = 1.0;
    v135.y = 0.0;
    v135.z = 0.0;
    v82.x = 1.0;
    v82.y = 0.0;
    v82.z = 0.0;
    v130.x = 1.0;
    v130.y = 0.0;
    v130.z = 0.0;
    v89.x = 1.0;
    v89.y = 0.0;
    v89.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v14 = (tScriptStruct_HoverCraftShoot*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v14 = 0;
    v131 = v14;
    v99 = 0;
    v108 = 0;
    v119 = 0;
    v120 = 0;
    v133 = 0.0;
    v134 = 0.0;
    v95 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Hovercraft_shoot_HoverShoot_init_Hovercraft_shoot_HoverShoot_init = (int)AI_ul_RealResolveOneRefFunction(0x49020711u);
        fctvar_Hovercraft_shoot_HoverShoot_init_UNKNOWN1 = AI_ul_RealResolveOneRef(42, 0x72002C09u);
        fctvar_Hovercraft_shoot_HoverShoot_init_UNKNOWN2 = AI_ul_RealResolveOneRef(42, 0x720043E2u);
        fctvar_Hovercraft_shoot_HoverShoot_init_Switch_s1 = AI_ul_RealResolveOneRef(42, 0x9E001645);
        Hovercraft_shoot_HoverShoot_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Hovercraft_shoot_HoverShoot_init_univVars_Perso_ID_actor,
            (void*)Hovercraft_shoot_HoverShoot_init_univVars,
            988);
        Hovercraft_shoot_HoverShoot_init_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
        fct_Hovercraft_shoot_HoverShoot_init_Hovercraft_shoot_HoverShoot_move = (int)AI_ul_RealResolveOneRefFunction(0x49020712u);
        fct_Hovercraft_shoot_HoverShoot_init_Hovercraft_shoot_HoverShoot_reflex = (int)AI_ul_RealResolveOneRefFunction(0x7200502Fu);
    }
    if (v131->i_flag_init_done)
    {
        v75 = (tGameObjectMain*)v131->o_Target;
        if (v131->o_Target)
        {
            if ((unsigned int)v75 <= 2)
                v75 = AI_MainActor[(uint32_t)v75 - 1];
            v74 = v75;
        }
        else
        {
            v74 = 0;
        }
        if (v74)
        {
            v73 = (tGameObjectMain*)v131->o_Target;
            if (v131->o_Target)
            {
                if ((unsigned int)v73 <= 2)
                    v73 = AI_MainActor[(uint32_t)v73 - 1];
                v2 = AI_EvalFunc_AIIsModel_C(v73, (tAIModel*)fctvar_Hovercraft_shoot_HoverShoot_init_UNKNOWN1);
            }
            else
            {
                v2 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_Hovercraft_shoot_HoverShoot_init_UNKNOWN1);
            }
            v86 = v2;
            if (v2)
            {
                v131->i_flag_perfect_shoot = 1;
            }
            else
            {
                v72 = (tGameObjectMain*)v131->o_Target;
                if (v131->o_Target)
                {
                    if ((unsigned int)v72 <= 2)
                        v72 = AI_MainActor[(uint32_t)v72 - 1];
                    v3 = AI_EvalFunc_AIIsModel_C(v72, (tAIModel*)fctvar_Hovercraft_shoot_HoverShoot_init_UNKNOWN2);
                }
                else
                {
                    v3 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_Hovercraft_shoot_HoverShoot_init_UNKNOWN2);
                }
                v112 = v3;
                if (v3)
                {
                    v131->i_flag_perfect_shoot = 1;
                }
                else
                {
                    v71 = (tGameObjectMain*)v131->o_Target;
                    if (v131->o_Target)
                    {
                        if ((unsigned int)v71 <= 2)
                            v71 = AI_MainActor[(uint32_t)v71 - 1];
                        v4 = AI_EvalFunc_AIIsModel_C(v71, (tAIModel*)fctvar_Hovercraft_shoot_HoverShoot_init_Switch_s1);
                    }
                    else
                    {
                        v4 = AI_EvalFunc_AIIsModel_C(0, (tAIModel*)fctvar_Hovercraft_shoot_HoverShoot_init_Switch_s1);
                    }
                    v139 = v4;
                    if (v4)
                        v131->i_flag_perfect_shoot = 1;
                }
            }
        }
        v70 = (tGameObjectMain*)v131->o_main_actor;
        if (v70)
        {
            if ((unsigned int)v70 <= 2)
                v70 = AI_MainActor[(uint32_t)v70 - 1];
            AI_EvalFunc_DYNSpeedGetVector_C(v70, &v92);
        }
        else
        {
            AI_EvalFunc_DYNSpeedGetVector_C(0, &v92);
        }
        v135 = v92;
        v69 = (tGameObjectMain*)v131->o_main_actor;
        if (v69)
        {
            if ((unsigned int)v69 <= 2)
                v69 = AI_MainActor[(uint32_t)v69 - 1];
            v5 = AI_EvalFunc_DYNSpeedGet_C(v69);
        }
        else
        {
            v5 = AI_EvalFunc_DYNSpeedGet_C(0);
        }
        v133 = (float)v5;
        v131->v_aircraft_start_speed = v135;
        v131->f_time_start = TIM_gf_MainClock - TIM_gf_LockTime;
        v108 = AI_EvalFunc_VIEWGetObject_C(0);
        v118.x = -200.0;
        v118.y = -200.0;
        v118.z = -200.0;
        AI_EvalFunc_BVMinSet_C(AI_gpst_CurrentGameObject, &v118);
        v88.x = 200.0;
        v88.y = 200.0;
        v88.z = 200.0;
        AI_EvalFunc_BVMaxSet_C(AI_gpst_CurrentGameObject, &v88);
        XMEN_OnOff(AI_gpst_CurrentGameObject, 0, 0);
        v68 = (tGameObjectMain*)v131->o_main_actor;
        if (v68)
        {
            if ((unsigned int)v68 <= 2)
                v68 = AI_MainActor[(uint32_t)v68 - 1];
            v6 = AI_EvalFunc_ANICanalGaoGet_C(v68, 0);
        }
        else
        {
            v6 = AI_EvalFunc_ANICanalGaoGet_C(0, 0);
        }
        v95 = (unsigned int)v6;
        v131->o_aircraft = *(uint32_t*)(Hovercraft_shoot_HoverShoot_init_univVars_Perso_ID_actor + 44);
        v67 = (tGameObjectMain*)v131->o_main_actor;
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
        v65 = (tGameObjectMain*)v131->o_aircraft;
        if (v65)
        {
            if ((unsigned int)v65 <= 2)
                v65 = AI_MainActor[(uint32_t)v65 - 1];
            v64 = v65;
        }
        else
        {
            v64 = 0;
        }
        if (v66 == v64)
        {
            v63 = (tGameObjectMain*)v95;
            if (v95)
            {
                if (v95 <= 2)
                    v63 = AI_MainActor[v95 - 1];
                AI_EvalFunc_OBJGetPos_C(v63, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            v82 = a3;
            v62 = (tGameObjectMain*)v95;
            if (v95)
            {
                if (v95 <= 2)
                    v62 = AI_MainActor[v95 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v62, &v96);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v96);
            }
            v105 = v96.x * 3.5999999f;
            v106 = v96.y * 3.5999999f;
            v107 = v96.z * 3.5999999f;
            v82.x = v82.x + v105;
            v82.y = v82.y + v106;
            v82.z = v82.z + v107;
            v61 = (tGameObjectMain*)v95;
            if (v95)
            {
                if (v95 <= 2)
                    v61 = AI_MainActor[v95 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v61, &v85);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v85);
            }
            v102 = v85.x * 1.25f;
            v103 = v85.y * 1.25f;
            v104 = v85.z * 1.25f;
            v82.x = v82.x + v102;
            v82.y = v82.y + v103;
            v82.z = v82.z + v104;
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v82);
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(4, 0);
        }
        else
        {
            v60 = (tGameObjectMain*)v95;
            if (v95)
            {
                if (v95 <= 2)
                    v60 = AI_MainActor[v95 - 1];
                AI_EvalFunc_OBJGetPos_C(v60, &v124);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v124);
            }
            v82 = v124;
            v59 = (tGameObjectMain*)v95;
            if (v95)
            {
                if (v95 <= 2)
                    v59 = AI_MainActor[v95 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v59, &v117);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v117);
            }
            v141 = v117.x * 5.0f;
            v142 = v117.y * 5.0f;
            v143 = v117.z * 5.0f;
            v82.x = v82.x + v141;
            v82.y = v82.y + v142;
            v82.z = v82.z + v143;
            v58 = (tGameObjectMain*)v95;
            if (v95)
            {
                if (v95 <= 2)
                    v58 = AI_MainActor[v95 - 1];
                AI_EvalFunc_OBJ_BankingGet_C(v58, &v121);
            }
            else
            {
                AI_EvalFunc_OBJ_BankingGet_C(0, &v121);
            }
            v82.x = v82.x + v121.x;
            v82.y = v82.y + v121.y;
            v82.z = v82.z + v121.z;
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v82);
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 4);
        }
        v89 = v82;
        v131->f_Distance = 0.0;
        v119 = -1;
        v99 = 0;
        v57 = (tGameObjectMain*)v131->o_Target;
        if (v131->o_Target)
        {
            if ((unsigned int)v57 <= 2)
                v57 = AI_MainActor[(uint32_t)v57 - 1];
            v56 = v57;
        }
        else
        {
            v56 = 0;
        }
        if (v56)
        {
            v131->f_XMEN_width = 0.0;
            if (v131->GFX_Halo_2 == -1)
            {
                v131->GFX_Halo_2 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v131->GFX_Halo_2, 4, 1);
                v22 = (tGameObjectMain*)Hovercraft_shoot_HoverShoot_init_SFX_light_smoke_gao;
                if (Hovercraft_shoot_HoverShoot_init_SFX_light_smoke_gao)
                {
                    if ((unsigned int)Hovercraft_shoot_HoverShoot_init_SFX_light_smoke_gao <= 2)
                        v22 = AI_MainActor[Hovercraft_shoot_HoverShoot_init_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v131->GFX_Halo_2, v22, 22);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v131->GFX_Halo_2, 0, 22);
                }
                GFXSetCurGlobal(v131->GFX_Halo_2);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 4.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 15.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21004, 5.0);
                AI_EvalFunc_GFXSetf_C_CURGLOB(21006, 3.141593f);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 0xFF9FAF7F);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21104, 8);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v81);
                AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v81);
                v21 = v108;
                if (v108)
                {
                    if ((unsigned int)v108 <= 2)
                        v21 = AI_MainActor[(uint32_t)v108 - 1];
                    AI_EvalFunc_OBJ_BankingGet_C(v21, &v126);
                }
                else
                {
                    AI_EvalFunc_OBJ_BankingGet_C(0, &v126);
                }
                AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v126);
            }
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v83);
            v131->f_speed = v83.z * v135.z + v83.y * v135.y + v135.x * v83.x;
            v131->i_flag_target = 1;
            v20 = (tGameObjectMain*)v131->o_Target;
            if (v131->o_Target)
            {
                if ((unsigned int)v20 <= 2)
                    v20 = AI_MainActor[(uint32_t)v20 - 1];
                AI_EvalFunc_BVRandomPosGet_C(v20, 0, &v131->v_rand_bv_pos);
            }
            else
            {
                AI_EvalFunc_BVRandomPosGet_C(0, 0, &v131->v_rand_bv_pos);
            }
            v19 = (tGameObjectMain*)v131->o_Target;
            if (v131->o_Target)
            {
                if ((unsigned int)v19 <= 2)
                    v19 = AI_MainActor[(uint32_t)v19 - 1];
                AI_EvalFunc_OBJGetPos_C(v19, &v115);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v115);
            }
            v18 = &v131->v_rand_bv_pos;
            v131->v_rand_bv_pos.x = v131->v_rand_bv_pos.x - v115.x;
            v18->y = v18->y - v115.y;
            v18->z = v18->z - v115.z;
            v17 = &v131->v_rand_bv_pos;
            v131->v_rand_bv_pos.x = v131->v_rand_bv_pos.x * 0.5f;
            v17->y = v17->y * 0.5f;
            v17->z = v17->z * 0.5f;
            X = (float)fabs(v131->v_rand_bv_pos.z);
            v131->v_rand_bv_pos.z = X;
            v131->i_flag_1st_trame = 1;
            v16 = (tGameObjectMain*)v131->o_main_actor;
            if (v16)
            {
                if ((unsigned int)v16 <= 2)
                    v16 = AI_MainActor[(uint32_t)v16 - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v16,
                    2,
                    (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_init_Hovercraft_shoot_HoverShoot_move);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_init_Hovercraft_shoot_HoverShoot_move);
            }
            v15 = (tGameObjectMain*)v131->o_main_actor;
            if (v15)
            {
                if ((unsigned int)v15 <= 2)
                    v15 = AI_MainActor[(uint32_t)v15 - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v15,
                    2,
                    (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_init_Hovercraft_shoot_HoverShoot_init);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_init_Hovercraft_shoot_HoverShoot_init);
            }
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v131->v_Sight);
            AI_EvalFunc_AITrackChange_C(
                0,
                (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_init_Hovercraft_shoot_HoverShoot_reflex);
            result = 1000;
        }
        else
        {
            v134 = v133;
            v130 = v135;
            if (v133 == 0.0)
            {
                v131->f_delay_inertie = 0.0;
            }
            else
            {
                v55 = (tGameObjectMain*)v131->o_main_actor;
                if (v55)
                {
                    if ((unsigned int)v55 <= 2)
                        v55 = AI_MainActor[(uint32_t)v55 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v55, &v98);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v98);
                }
                MATH_VecMultNormScalar(&v97, &v130, v134);
                v131->f_delay_inertie = v97.z * v98.z + v97.y * v98.y + v98.x * v97.x;
                if (v131->f_delay_inertie < 0.0f)
                    v13 = 0.0f;
                else
                    v13 = v131->f_delay_inertie;
                v87 = v13;
                v131->f_delay_inertie = 1.0f - v13;
                v54 = (tGameObjectMain*)v131->o_main_actor;
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
                v52 = (tGameObjectMain*)v131->o_aircraft;
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
                if (v53 == v51)
                    v131->f_delay_inertie = v131->f_delay_inertie * 0.5f;
                else
                    v131->f_delay_inertie = v131->f_delay_inertie * 0.1f;
            }
            v50 = (tGameObjectMain*)v95;
            if (v95)
            {
                if (v95 <= 2)
                    v50 = AI_MainActor[v95 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v50, &v136);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v136);
            }
            v140 = v136;
            v49 = (tGameObjectMain*)v131->o_main_actor;
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
            v47 = (tGameObjectMain*)v131->o_aircraft;
            if (v47)
            {
                if ((unsigned int)v47 <= 2)
                    v47 = AI_MainActor[(uint32_t)v47 - 1];
                v46 = v47;
            }
            else
            {
                v46 = 0;
            }
            if (v48 == v46)
            {
                v45 = (tGameObjectMain*)v131->o_aircraft;
                if (v45)
                {
                    if ((unsigned int)v45 <= 2)
                        v45 = AI_MainActor[(uint32_t)v45 - 1];
                    v7 = AI_EvalFunc_OBJ_CapaGet_C(v45);
                }
                else
                {
                    v7 = AI_EvalFunc_OBJ_CapaGet_C(0);
                }
                v111 = v7;
                if ((v7 & 0x2800) == 0)
                    goto LABEL_179;
                v44 = (tGameObjectMain*)v131->o_aircraft;
                if (v44)
                {
                    if ((unsigned int)v44 <= 2)
                        v44 = AI_MainActor[(uint32_t)v44 - 1];
                    v8 = AI_EvalFunc_OBJ_CapaTest_C(v44, 0x1000);
                }
                else
                {
                    v8 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x1000);
                }
                v132 = v8;
                if (v8)
                {
                LABEL_179:
                    v138.x = 0.0;
                    v138.y = 0.0;
                    v138.z = 1.0;
                    AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v140, &v138);
                }
                else
                {
                    v43 = (tGameObjectMain*)v131->o_aircraft;
                    if (v43)
                    {
                        if ((unsigned int)v43 <= 2)
                            v43 = AI_MainActor[(uint32_t)v43 - 1];
                        AI_EvalFunc_OBJ_SightGet_C(v43, &v110);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_SightGet_C(0, &v110);
                    }
                    v116.x = 0.0;
                    v116.y = 0.0;
                    v116.z = 1.0;
                    AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v110, &v116);
                }
            }
            else
            {
                v91.x = 0.0;
                v91.y = 0.0;
                v91.z = 1.0;
                AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v140, &v91);
            }
            if (v131->GFX_Line == -1)
            {
                v131->GFX_Line = AI_EvalFunc_GFXAdd_C_CURRENT(1);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v131->GFX_Line, 0, 1);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v131->GFX_Line, 2, 1);
                v42 = (tGameObjectMain*)Hovercraft_shoot_HoverShoot_init_SFX_light_smoke_gao;
                if (Hovercraft_shoot_HoverShoot_init_SFX_light_smoke_gao)
                {
                    if ((unsigned int)Hovercraft_shoot_HoverShoot_init_SFX_light_smoke_gao <= 2)
                        v42 = AI_MainActor[Hovercraft_shoot_HoverShoot_init_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v131->GFX_Line, v42, 27);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v131->GFX_Line, 0, 27);
                }
                v41 = (tGameObjectMain*)v131->o_main_actor;
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
                v39 = (tGameObjectMain*)v131->o_aircraft;
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
                if (v40 == v38)
                    AI_EvalFunc_GFXSetf_C_CURRENT(v131->GFX_Line, 1000, 0.75);
                else
                    AI_EvalFunc_GFXSetf_C_CURRENT(v131->GFX_Line, 1000, 3.0);
                GFXSetCurGlobal(v131->GFX_Line);
                AI_EvalFunc_GFXSeti_C_CURGLOB(1100, 0xFFF0F0);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v123);
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v84);
                v37 = v131->f_line_length;
                v94.x = v84.x * v37;
                v94.y = v84.y * v37;
                v94.z = v84.z * v37;
                MATH_SubstractVec(&dest, &v123, &v94);
                AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &dest);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v137);
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v113);
                v100.x = v113.x + v113.x;
                v100.y = v113.y + v113.y;
                v100.z = v113.z + v113.z;
                MATH_VecAddVec(&a1, &v137, &v100);
                AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &a1);
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v93);
                v101.x = -v93.x;
                v101.y = -v93.y;
                v101.z = -v93.z;
                AI_EvalFunc_GFXSetv_C_CURGLOB(1202, &v101);
            }
            v36 = (tGameObjectMain*)v131->o_aircraft;
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
            if (v35)
            {
                v34 = (tGameObjectMain*)v131->o_aircraft;
                if (v34)
                {
                    if ((unsigned int)v34 <= 2)
                        v34 = AI_MainActor[(uint32_t)v34 - 1];
                    v9 = AI_EvalFunc_SNDRequest_C(v34, 33, 0x18000080);
                }
                else
                {
                    v9 = AI_EvalFunc_SNDRequest_C(0, 33, 0x18000080);
                }
                v120 = v9;
                AI_EvalFunc_SNDUltraPlaySound_C(AI_gpst_CurrentGameObject, v9);
                v128 = SND_f_GetVol(v120);
                v90 = AI_EvalFunc_MATHRandFloat_C(0.80000001f, 1.0);
                v10 = v131->f_shoot_vol * v90 * v128;
                SND_SetVol(v120, v10);
                v129 = AI_EvalFunc_MATHRandFloat_C(0.94999999f, 1.0);
                v144 = SND_l_GetFreq(v120);
                X_4 = (float)v144;
                SND_SetFreq(v120, (unsigned __int64)(v129 * X_4));
            }
            v33 = (tGameObjectMain*)v131->o_main_actor;
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
            v31 = (tGameObjectMain*)v131->o_aircraft;
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
            if (v32 == v30)
                v131->f_speed = 500.0;
            else
                v131->f_speed = 1000.0;
            if (v131->i_Halo == -1)
            {
                v131->i_Halo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
                AI_EvalFunc_GFXFlagSet_C_CURRENT(v131->i_Halo, 4, 1);
                v29 = (tGameObjectMain*)Hovercraft_shoot_HoverShoot_init_SFX_light_smoke_gao;
                if (Hovercraft_shoot_HoverShoot_init_SFX_light_smoke_gao)
                {
                    if ((unsigned int)Hovercraft_shoot_HoverShoot_init_SFX_light_smoke_gao <= 2)
                        v29 = AI_MainActor[Hovercraft_shoot_HoverShoot_init_SFX_light_smoke_gao - 1];
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v131->i_Halo, v29, 0);
                }
                else
                {
                    AI_EvalFunc_GFXMaterialSet_C_CURRENT(v131->i_Halo, 0, 0);
                }
                v28 = (tGameObjectMain*)v131->o_main_actor;
                if (v28)
                {
                    if ((unsigned int)v28 <= 2)
                        v28 = AI_MainActor[(uint32_t)v28 - 1];
                    v27 = v28;
                }
                else
                {
                    v27 = 0;
                }
                v26 = (tGameObjectMain*)v131->o_aircraft;
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
                if (v27 == v25)
                    AI_EvalFunc_GFXSetf_C_CURRENT(v131->i_Halo, 21000, 5.0);
                else
                    AI_EvalFunc_GFXSetf_C_CURRENT(v131->i_Halo, 21000, 10.0);
                GFXSetCurGlobal(v131->i_Halo);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 0x7E8D49);
                AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v109);
                AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v109);
            }
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v131->v_Sight);
            v131->i_flag_1st_trame = 1;
            v24 = (tGameObjectMain*)v131->o_main_actor;
            if (v24)
            {
                if ((unsigned int)v24 <= 2)
                    v24 = AI_MainActor[(uint32_t)v24 - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v24,
                    2,
                    (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_init_Hovercraft_shoot_HoverShoot_move);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_init_Hovercraft_shoot_HoverShoot_move);
            }
            v23 = (tGameObjectMain*)v131->o_main_actor;
            if (v23)
            {
                if ((unsigned int)v23 <= 2)
                    v23 = AI_MainActor[(uint32_t)v23 - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v23,
                    2,
                    (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_init_Hovercraft_shoot_HoverShoot_init);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_init_Hovercraft_shoot_HoverShoot_init);
            }
            AI_EvalFunc_AITrackChange_C(
                0,
                (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_init_Hovercraft_shoot_HoverShoot_reflex);
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
    }
    else
    {
        v131->o_main_actor = (int)*AI_MainActor;
        v80 = (tGameObjectMain*)v131->o_main_actor;
        if (v80)
        {
            if ((unsigned int)v80 <= 2)
                v80 = AI_MainActor[(uint32_t)v80 - 1];
            v0 = AI_EvalFunc_ANICanalGaoGet_C(v80, 0);
        }
        else
        {
            v0 = AI_EvalFunc_ANICanalGaoGet_C(0, 0);
        }
        v95 = (unsigned int)v0;
        v79 = v0;
        if (v0)
        {
            if ((unsigned int)v0 <= 2)
                v79 = AI_MainActor[(uint32_t)v0 - 1];
            v78 = v79;
        }
        else
        {
            v78 = 0;
        }
        if (v78 || (v122 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject), v122 == 1000))
        {
            v77 = (tGameObjectMain*)v131->o_main_actor;
            if (v77)
            {
                if ((unsigned int)v77 <= 2)
                    v77 = AI_MainActor[(uint32_t)v77 - 1];
                AI_EvalFunc_OBJGetPos_C(v77, &v131->v_last_pos);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v131->v_last_pos);
            }
            v131->i_flag_init_done = 1;
            v76 = (tGameObjectMain*)v131->o_main_actor;
            if (v76)
            {
                if ((unsigned int)v76 <= 2)
                    v76 = AI_MainActor[(uint32_t)v76 - 1];
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    v76,
                    2,
                    (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_init_Hovercraft_shoot_HoverShoot_init);
            }
            else
            {
                AI_EvalFunc_AICBAdd_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_init_Hovercraft_shoot_HoverShoot_init);
            }
            AI_EvalFunc_AITrackCurStop_C();
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 4;
        }
        else
        {
            result = v122;
        }
    }
    return result;
}

int Hovercraft_shoot_HoverShoot_move()
{
    int result; // eax
    unsigned int v1; // eax
    unsigned int v2; // eax
    unsigned int v3; // eax
    int v4; // eax
    double v5; // st7
    double v6; // st7
    unsigned int v7; // eax
    unsigned int v8; // eax
    unsigned int v9; // eax
    double v10; // st7
    float v11; // [esp+8h] [ebp-790h]
    float v12; // [esp+Ch] [ebp-78Ch]
    float v13; // [esp+10h] [ebp-788h]
    float v14; // [esp+14h] [ebp-784h]
    float v15; // [esp+18h] [ebp-780h]
    float v16; // [esp+1Ch] [ebp-77Ch]
    float v17; // [esp+20h] [ebp-778h]
    float v18; // [esp+24h] [ebp-774h]
    float v19; // [esp+28h] [ebp-770h]
    float v20; // [esp+2Ch] [ebp-76Ch]
    float v21; // [esp+30h] [ebp-768h]
    float v22; // [esp+34h] [ebp-764h]
    tScriptStruct_HoverCraftShoot* v23; // [esp+38h] [ebp-760h]
    float v24; // [esp+3Ch] [ebp-75Ch]
    float v25; // [esp+58h] [ebp-740h]
    float v26; // [esp+64h] [ebp-734h]
    tGameObjectMain* v27; // [esp+70h] [ebp-728h]
    tGameObjectMain* v28; // [esp+74h] [ebp-724h]
    tGameObjectMain* v29; // [esp+78h] [ebp-720h]
    tGameObjectMain* v30; // [esp+7Ch] [ebp-71Ch]
    tGameObjectMain* v31; // [esp+84h] [ebp-714h]
    tGameObjectMain* v32; // [esp+8Ch] [ebp-70Ch]
    tGameObjectMain* v33; // [esp+94h] [ebp-704h]
    float v34; // [esp+A4h] [ebp-6F4h]
    tGameObjectMain* v35; // [esp+ACh] [ebp-6ECh]
    tGameObjectMain* v36; // [esp+B4h] [ebp-6E4h]
    float v37; // [esp+BCh] [ebp-6DCh]
    tGameObjectMain* v38; // [esp+CCh] [ebp-6CCh]
    tGameObjectMain* v39; // [esp+D4h] [ebp-6C4h]
    tGameObjectMain* v40; // [esp+DCh] [ebp-6BCh]
    tGameObjectMain* v41; // [esp+E4h] [ebp-6B4h]
    tGameObjectMain* v42; // [esp+F0h] [ebp-6A8h]
    tGameObjectMain* v43; // [esp+F4h] [ebp-6A4h]
    tGameObjectMain* v44; // [esp+FCh] [ebp-69Ch]
    tGameObjectMain* v45; // [esp+104h] [ebp-694h]
    tGameObjectMain* v46; // [esp+10Ch] [ebp-68Ch]
    tGameObjectMain* v47; // [esp+114h] [ebp-684h]
    tGameObjectMain* v48; // [esp+11Ch] [ebp-67Ch]
    float v49; // [esp+12Ch] [ebp-66Ch]
    tGameObjectMain* v50; // [esp+134h] [ebp-664h]
    tGameObjectMain* v51; // [esp+13Ch] [ebp-65Ch]
    float v52; // [esp+144h] [ebp-654h]
    tGameObjectMain* v53; // [esp+154h] [ebp-644h]
    tGameObjectMain* v54; // [esp+15Ch] [ebp-63Ch]
    tGameObjectMain* v55; // [esp+164h] [ebp-634h]
    tGameObjectMain* v56; // [esp+16Ch] [ebp-62Ch]
    tGameObjectMain* v57; // [esp+174h] [ebp-624h]
    tGameObjectMain* v58; // [esp+17Ch] [ebp-61Ch]
    tGameObjectMain* v59; // [esp+180h] [ebp-618h]
    tGameObjectMain* v60; // [esp+184h] [ebp-614h]
    tGameObjectMain* v61; // [esp+18Ch] [ebp-60Ch]
    tGameObjectMain* v62; // [esp+190h] [ebp-608h]
    tGameObjectMain* v63; // [esp+194h] [ebp-604h]
    tGameObjectMain* v64; // [esp+1A0h] [ebp-5F8h]
    tGameObjectMain* v65; // [esp+1A4h] [ebp-5F4h]
    tGameObjectMain* v66; // [esp+1A8h] [ebp-5F0h]
    tAI* v67; // [esp+1B0h] [ebp-5E8h]
    tGameObjectMain* v68; // [esp+1B4h] [ebp-5E4h]
    tGameObjectMain* v69; // [esp+1B8h] [ebp-5E0h]
    tGameObjectMain* v70; // [esp+1C0h] [ebp-5D8h]
    tGameObjectMain* v71; // [esp+1C8h] [ebp-5D0h]
    float v72; // [esp+1CCh] [ebp-5CCh]
    tGameObjectMain* v73; // [esp+1F0h] [ebp-5A8h]
    tGameObjectMain* v74; // [esp+204h] [ebp-594h]
    tGameObjectMain* v75; // [esp+208h] [ebp-590h]
    tGameObjectMain* v76; // [esp+20Ch] [ebp-58Ch]
    tGameObjectMain* v77; // [esp+210h] [ebp-588h]
    tGameObjectMain* v78; // [esp+214h] [ebp-584h]
    tGameObjectMain* v79; // [esp+21Ch] [ebp-57Ch]
    tGameObjectMain* v80; // [esp+224h] [ebp-574h]
    tGameObjectMain* v81; // [esp+22Ch] [ebp-56Ch]
    float v82; // [esp+230h] [ebp-568h]
    float v83; // [esp+234h] [ebp-564h]
    tGameObjectMain* v84; // [esp+238h] [ebp-560h]
    tGameObjectMain* v85; // [esp+23Ch] [ebp-55Ch]
    tGameObjectMain* v86; // [esp+240h] [ebp-558h]
    tGameObjectMain* v87; // [esp+244h] [ebp-554h]
    Vec3 v88; // [esp+24Ch] [ebp-54Ch] BYREF
    Vec3 v89; // [esp+258h] [ebp-540h] BYREF
    Vec3 v90; // [esp+264h] [ebp-534h] BYREF
    Vec3 v91; // [esp+270h] [ebp-528h] BYREF
    Vec3 v92; // [esp+27Ch] [ebp-51Ch] BYREF
    unsigned int v93; // [esp+288h] [ebp-510h]
    float v94; // [esp+28Ch] [ebp-50Ch]
    float v95; // [esp+290h] [ebp-508h]
    float v96; // [esp+294h] [ebp-504h]
    float v97; // [esp+298h] [ebp-500h]
    unsigned int v98; // [esp+29Ch] [ebp-4FCh]
    tGameObjectMain* v99; // [esp+2A0h] [ebp-4F8h]
    float a4; // [esp+2A4h] [ebp-4F4h]
    int v101; // [esp+2ACh] [ebp-4ECh]
    Vec3 v102; // [esp+2B0h] [ebp-4E8h] BYREF
    Vec3 v103; // [esp+2BCh] [ebp-4DCh] BYREF
    Vec3 v104; // [esp+2C8h] [ebp-4D0h] BYREF
    unsigned int v105; // [esp+2DCh] [ebp-4BCh]
    float v106; // [esp+2E0h] [ebp-4B8h]
    float v107; // [esp+2E4h] [ebp-4B4h]
    float v108; // [esp+2ECh] [ebp-4ACh]
    Vec3 v109; // [esp+2F0h] [ebp-4A8h] BYREF
    int v110; // [esp+2FCh] [ebp-49Ch]
    float v111; // [esp+304h] [ebp-494h]
    unsigned int v112; // [esp+308h] [ebp-490h]
    Vec3 v113; // [esp+30Ch] [ebp-48Ch] BYREF
    Vec3 v114; // [esp+318h] [ebp-480h] BYREF
    Vec3 v115; // [esp+324h] [ebp-474h] BYREF
    Vec3 v116; // [esp+330h] [ebp-468h] BYREF
    Vec3 v117; // [esp+33Ch] [ebp-45Ch] BYREF
    Vec3 v118; // [esp+348h] [ebp-450h] BYREF
    float v119; // [esp+354h] [ebp-444h]
    tMessage Src; // [esp+358h] [ebp-440h] BYREF
    Vec3 v121; // [esp+38Ch] [ebp-40Ch] BYREF
    Vec3 v122; // [esp+398h] [ebp-400h] BYREF
    Vec3 v123; // [esp+3A4h] [ebp-3F4h] BYREF
    Vec3 a5; // [esp+3B0h] [ebp-3E8h] BYREF
    Vec3 v125; // [esp+3BCh] [ebp-3DCh] BYREF
    float v126; // [esp+3D4h] [ebp-3C4h]
    float v128; // [esp+3E0h] [ebp-3B8h]
    float v129; // [esp+3E4h] [ebp-3B4h]
    Vec3 v130; // [esp+3E8h] [ebp-3B0h] BYREF
    Vec3 pos; // [esp+3F4h] [ebp-3A4h] BYREF
    Vec3 v132; // [esp+400h] [ebp-398h] BYREF
    Vec3 v133; // [esp+40Ch] [ebp-38Ch] BYREF
    Vec3 v134; // [esp+418h] [ebp-380h] BYREF
    unsigned int v135; // [esp+428h] [ebp-370h]
    float v136; // [esp+42Ch] [ebp-36Ch]
    float v137; // [esp+430h] [ebp-368h]
    float v138; // [esp+434h] [ebp-364h]
    Vec3 dest; // [esp+438h] [ebp-360h] BYREF
    Vec3 v140; // [esp+444h] [ebp-354h] BYREF
    tGameObjectMain* v141; // [esp+450h] [ebp-348h]
    float v142; // [esp+454h] [ebp-344h]
    Vec3 v143; // [esp+458h] [ebp-340h] BYREF
    Vec3 v144; // [esp+464h] [ebp-334h] BYREF
    Vec3 v145; // [esp+470h] [ebp-328h] BYREF
    Vec3 v146; // [esp+47Ch] [ebp-31Ch] BYREF
    Vec3 a3; // [esp+488h] [ebp-310h] BYREF
    Vec3 v148; // [esp+494h] [ebp-304h] BYREF
    Vec3 v149; // [esp+4A0h] [ebp-2F8h] BYREF
    Vec3 v150; // [esp+4ACh] [ebp-2ECh] BYREF
    Vec3 v151; // [esp+4B8h] [ebp-2E0h] BYREF
    Vec3 v152; // [esp+4CCh] [ebp-2CCh] BYREF
    Vec3 v153; // [esp+4D8h] [ebp-2C0h] BYREF
    Vec3 v154; // [esp+4E4h] [ebp-2B4h] BYREF
    Vec3 v155; // [esp+4F0h] [ebp-2A8h]
    Vec3 v156; // [esp+4FCh] [ebp-29Ch] BYREF
    Vec3 v157; // [esp+508h] [ebp-290h] BYREF
    float v158; // [esp+518h] [ebp-280h]
    float v160; // [esp+520h] [ebp-278h]
    Vec3 v161; // [esp+524h] [ebp-274h] BYREF
    Vec3 v162; // [esp+530h] [ebp-268h] BYREF
    Vec3 v163; // [esp+53Ch] [ebp-25Ch] BYREF
    Vec3 v164; // [esp+548h] [ebp-250h] BYREF
    Vec3 v165; // [esp+554h] [ebp-244h] BYREF
    Vec3 v166; // [esp+560h] [ebp-238h] BYREF
    Vec3 v167; // [esp+56Ch] [ebp-22Ch] BYREF
    Vec3 v168; // [esp+578h] [ebp-220h] BYREF
    Vec3 v169; // [esp+584h] [ebp-214h] BYREF
    Vec3 v170; // [esp+590h] [ebp-208h]
    int v171; // [esp+59Ch] [ebp-1FCh]
    int v172; // [esp+5A0h] [ebp-1F8h]
    int v173; // [esp+5A4h] [ebp-1F4h]
    Vec3 v174; // [esp+5A8h] [ebp-1F0h] BYREF
    Vec3 v175; // [esp+5B4h] [ebp-1E4h] BYREF
    Vec3 v176; // [esp+5C0h] [ebp-1D8h] BYREF
    Vec3 v177; // [esp+5CCh] [ebp-1CCh] BYREF
    float v178; // [esp+5D8h] [ebp-1C0h]
    float v179; // [esp+5DCh] [ebp-1BCh]
    Vec3 v180; // [esp+5E0h] [ebp-1B8h] BYREF
    Vec3 v181; // [esp+5ECh] [ebp-1ACh] BYREF
    Vec3 v182; // [esp+5F8h] [ebp-1A0h] BYREF
    float v183; // [esp+604h] [ebp-194h]
    float v184; // [esp+608h] [ebp-190h]
    float v185; // [esp+60Ch] [ebp-18Ch]
    Vec3 v186; // [esp+610h] [ebp-188h] BYREF
    Vec3 v187; // [esp+61Ch] [ebp-17Ch] BYREF
    Vec3 v188; // [esp+628h] [ebp-170h] BYREF
    Vec3 a2; // [esp+634h] [ebp-164h] BYREF
    int v190; // [esp+640h] [ebp-158h]
    float v191; // [esp+644h] [ebp-154h]
    float v192; // [esp+648h] [ebp-150h]
    float v193; // [esp+64Ch] [ebp-14Ch]
    float v194; // [esp+650h] [ebp-148h]
    Vec3 v195; // [esp+654h] [ebp-144h] BYREF
    float v196; // [esp+660h] [ebp-138h]
    float v197; // [esp+664h] [ebp-134h]
    float v198; // [esp+668h] [ebp-130h]
    Vec3 v199; // [esp+66Ch] [ebp-12Ch] BYREF
    Vec3 v200; // [esp+678h] [ebp-120h] BYREF
    Vec3 v201; // [esp+684h] [ebp-114h] BYREF
    Vec3 v202; // [esp+690h] [ebp-108h] BYREF
    Vec3 v203; // [esp+6A8h] [ebp-F0h] BYREF
    tAI* v204; // [esp+6B8h] [ebp-E0h]
    Vec3 v205; // [esp+6BCh] [ebp-DCh] BYREF
    Vec3 v206; // [esp+6C8h] [ebp-D0h] BYREF
    Vec3 v207; // [esp+6D4h] [ebp-C4h] BYREF
    tScriptStruct_HoverCraftShoot* v208; // [esp+6E0h] [ebp-B8h]
    float v209; // [esp+6E4h] [ebp-B4h]
    float v210; // [esp+6E8h] [ebp-B0h]
    float v211; // [esp+6ECh] [ebp-ACh]
    float v212; // [esp+6F0h] [ebp-A8h]
    unsigned int v213; // [esp+6F4h] [ebp-A4h]
    float v214; // [esp+6F8h] [ebp-A0h]
    float v215; // [esp+6FCh] [ebp-9Ch]
    float v216; // [esp+700h] [ebp-98h]
    float v217; // [esp+704h] [ebp-94h]
    Vec3 v218; // [esp+708h] [ebp-90h] BYREF
    Vec3 v219; // [esp+714h] [ebp-84h] BYREF
    Vec3 v220; // [esp+720h] [ebp-78h] BYREF
    unsigned int v221; // [esp+72Ch] [ebp-6Ch]
    float v222; // [esp+734h] [ebp-64h]
    int v223; // [esp+738h] [ebp-60h]
    Vec3 v224; // [esp+73Ch] [ebp-5Ch] BYREF
    Vec3 v225; // [esp+748h] [ebp-50h] BYREF
    Vec3 v226; // [esp+754h] [ebp-44h] BYREF
    Vec3 v227; // [esp+760h] [ebp-38h] BYREF
    Vec3 v228; // [esp+76Ch] [ebp-2Ch] BYREF
    Vec3 v229; // [esp+778h] [ebp-20h] BYREF
    Vec3 v230; // [esp+784h] [ebp-14h] BYREF
    float v231; // [esp+794h] [ebp-4h]

    pos.x = 1.0;
    pos.y = 0.0;
    pos.z = 0.0;
    v186.x = 1.0;
    v186.y = 0.0;
    v186.z = 0.0;
    v187.x = 1.0;
    v187.y = 0.0;
    v187.z = 0.0;
    v157.x = 1.0;
    v157.y = 0.0;
    v157.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v23 = (tScriptStruct_HoverCraftShoot*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v23 = 0;
    v208 = v23;
    v190 = 0;
    v209 = 0.0;
    v171 = 0;
    v141 = 0;
    v99 = 0;
    v172 = 0;
    a4 = 0.0;
    v204 = 0;
    v211 = 0.0;
    v111 = 0.0;
    v213 = 0;
    v214 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Hovercraft_shoot_HoverShoot_move_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Hovercraft_shoot_HoverShoot_move_univVars_world_ID,
            (void*)Hovercraft_shoot_HoverShoot_move_univVars,
            4);
        fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_exec_impact = (int)AI_ul_RealResolveOneRefFunction(0x7200502Cu);
        fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_move = (int)AI_ul_RealResolveOneRefFunction(0x49020712u);
        fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_death = (int)AI_ul_RealResolveOneRefFunction(0x72004FC3u);
        Hovercraft_shoot_HoverShoot_move_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
    }
    if (v208->i_flag_1st_trame)
    {
        v208->i_flag_1st_trame = 0;
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    v190 = 0;
    v172 = 0;
    v141 = AI_EvalFunc_VIEWGetObject_C(0);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v208->v_last_pos);
    if (v208->i_flag_1st_trame)
    {
        v208->i_flag_1st_trame = 0;
        v178 = TIM_gf_dt;
        v109.x = TIM_gf_dt * v208->v_aircraft_start_speed.x;
        v109.y = v208->v_aircraft_start_speed.y * TIM_gf_dt;
        v109.z = v208->v_aircraft_start_speed.z * TIM_gf_dt;
        AI_EvalFunc_OBJMove_C(AI_gpst_CurrentGameObject, &v109);
    }
    v87 = (tGameObjectMain*)v208->o_main_actor;
    if (v87)
    {
        if ((unsigned int)v87 <= 2)
            v87 = AI_MainActor[(uint32_t)v87 - 1];
        v86 = v87;
    }
    else
    {
        v86 = 0;
    }
    v85 = (tGameObjectMain*)v208->o_aircraft;
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
    if (v86 == v84)
    {
        v107 = TIM_gf_dt;
        v83 = 500.0f * TIM_gf_dt + v208->f_speed;
        if (v83 > 500.0)
            v22 = 500.0;
        else
            v22 = 500.0f * TIM_gf_dt + v208->f_speed;
        v158 = v22;
        v208->f_speed = v22;
    }
    else
    {
        v222 = TIM_gf_dt;
        v82 = 1500.0f * TIM_gf_dt + v208->f_speed;
        if (v82 > 1500.0)
            v21 = 1500.0;
        else
            v21 = 1500.0f * TIM_gf_dt + v208->f_speed;
        v128 = v21;
        v208->f_speed = v21;
    }
    v179 = TIM_gf_dt;
    a4 = v208->f_speed * TIM_gf_dt;
    if (v208->i_flag_perfect_shoot)
        goto LABEL_104;
    v81 = (tGameObjectMain*)v208->o_main_actor;
    if (v81)
    {
        if ((unsigned int)v81 <= 2)
            v81 = AI_MainActor[(uint32_t)v81 - 1];
        v1 = AI_EvalFunc_OBJ_FlagsControlGet_C(v81);
    }
    else
    {
        v1 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
    }
    v221 = v1;
    v213 = v1 & 0x1000000;
    v80 = (tGameObjectMain*)v208->o_main_actor;
    if (v80)
    {
        if ((unsigned int)v80 <= 2)
            v80 = AI_MainActor[(uint32_t)v80 - 1];
        v2 = AI_EvalFunc_OBJ_FlagsControlSet_C(v80, 0x1000000, 0);
    }
    else
    {
        v2 = AI_EvalFunc_OBJ_FlagsControlSet_C(0, 0x1000000, 0);
    }
    v135 = v2;
    AI_EvalFunc_COL_SpecificCrossableSet_C(0xFF7Fu);
    if (*(uint32_t*)Hovercraft_shoot_HoverShoot_move_univVars_world_ID == 77)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v169);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v114);
        v99 = (tGameObjectMain*)AI_EvalFunc_COLRayObjectDist_C(
            AI_gpst_CurrentGameObject,
            &v169,
            &v114,
            a4,
            -1,
            0,
            0,
            0x80000001);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v162);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v133);
        v99 = (tGameObjectMain*)AI_EvalFunc_COLRayObjectDist_C(
            AI_gpst_CurrentGameObject,
            &v162,
            &v133,
            a4,
            49154,
            0,
            0,
            0x80000001);
    }
    if (!v213)
    {
        v79 = (tGameObjectMain*)v208->o_main_actor;
        if (v79)
        {
            if ((unsigned int)v79 <= 2)
                v79 = AI_MainActor[(uint32_t)v79 - 1];
            v3 = AI_EvalFunc_OBJ_FlagsControlSet_C(v79, 0, 0x1000000);
        }
        else
        {
            v3 = AI_EvalFunc_OBJ_FlagsControlSet_C(0, 0, 0x1000000);
        }
        v105 = v3;
    }
    v78 = v99;
    if (v99)
    {
        if ((unsigned int)v99 <= 2)
            v78 = AI_MainActor[(uint32_t)v99 - 1];
        v77 = v78;
    }
    else
    {
        v77 = 0;
    }
    if (v77)
    {
        v76 = (tGameObjectMain*)v208->o_aircraft;
        if (v76)
        {
            if ((unsigned int)v76 <= 2)
                v76 = AI_MainActor[(uint32_t)v76 - 1];
            v75 = v76;
        }
        else
        {
            v75 = 0;
        }
        if (v75)
        {
            v74 = (tGameObjectMain*)v208->o_aircraft;
            if (v74)
            {
                if ((unsigned int)v74 <= 2)
                    v74 = AI_MainActor[(uint32_t)v74 - 1];
                v4 = AI_EvalFunc_SNDRequest_C(v74, 0x2F, 0x18000080);
            }
            else
            {
                v4 = AI_EvalFunc_SNDRequest_C(0, 0x2F, 0x18000080);
            }
            v171 = v4;
            AI_EvalFunc_SNDUltraPlaySound_C(AI_gpst_CurrentGameObject, v4);
            v194 = AI_EvalFunc_MATHRandFloat_C(0.94999999f, 1.0);
            v173 = SND_l_GetFreq(v171);
            v20 = (float)v173;
            SND_SetFreq(v171, (unsigned __int64)(v194 * v20));
        }
        v208->o_Target = (int)v99;
        AI_EvalFunc_COLRayPosGet_C(AI_gpst_CurrentGameObject, &v225);
        pos = v225;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v176);
        MATH_SubstractVec(&dest, &pos, &v176);
        v142 = MATH_VecLen(&dest);
        if (v142 > 30.0)
            v19 = 30.0;
        else
            v19 = v142;
        v193 = v19;
        v208->f_line_length = v19;
        AI_EvalFunc_OBJSetPos_C_CURRENT(&pos);
        v110 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_exec_impact);
        if (v110 != 1000)
            return v110;
        v73 = (tGameObjectMain*)v208->o_main_actor;
        if (v73)
        {
            if ((unsigned int)v73 <= 2)
                v73 = AI_MainActor[(uint32_t)v73 - 1];
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                v73,
                2,
                (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_move);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_move);
        }
        AI_EvalFunc_AITrackChange_C(
            0,
            (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_death);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 4;
    }
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v218);
    v155.x = v218.x * a4;
    v155.y = v218.y * a4;
    v155.z = v218.z * a4;
    v186 = v155;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v206);
    pos.x = v206.x + v186.x;
    pos.y = v206.y + v186.y;
    pos.z = v206.z + v186.z;
    if (v186.z >= 0.0f)
    {
        v211 = -1.0e14f;
    }
    else
    {
        AI_EvalFunc_WTR_GetDif_C(&pos, &v156);
        v211 = v156.z;
    }
    if (pos.z >= (double)v211)
    {
    LABEL_104:
        v66 = (tGameObjectMain*)v208->o_Target;
        if (v208->o_Target)
        {
            if ((unsigned int)v66 <= 2)
                v66 = AI_MainActor[(uint32_t)v66 - 1];
            v65 = v66;
        }
        else
        {
            v65 = 0;
        }
        if (v65)
        {
            v64 = (tGameObjectMain*)v208->o_Target;
            if (v208->o_Target)
            {
                if ((unsigned int)v64 <= 2)
                    v64 = AI_MainActor[(uint32_t)v64 - 1];
                AI_EvalFunc_OBJGetPos_C(v64, &v219);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v219);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v161);
            MATH_SubstractVec(&v145, &v219, &v161);
            v191 = MATH_VecLen(&v145);
            if (a4 > (double)v191)
                v16 = v191;
            else
                v16 = a4;
            v126 = v16;
            a4 = v16;
        }
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v151);
        v230.x = v151.x * a4;
        v230.y = v151.y * a4;
        v230.z = v151.z * a4;
        AI_EvalFunc_OBJMove_C(AI_gpst_CurrentGameObject, &v230);
        v208->f_Distance = a4 + v208->f_Distance;
    }
    else
    {
        v190 = 1;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v163);
        pos.x = v163.x;
        pos.y = v163.y;
        pos.z = v211;
        v18 = (float)fabs(v186.z);
        v214 = v18;
        v186.z = 0.0;
        v209 = MATH_VecLen(&v186);
        if (v209 != 0.0)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v229);
            v17 = (float)fabs(v229.z - v211);
            v192 = v17;
            v72 = v17 / v214;
            v186.x = v186.x * v72;
            v186.y = v186.y * v72;
            v186.z = v186.z * v72;
            pos.x = pos.x + v186.x;
            pos.y = pos.y + v186.y;
            pos.z = pos.z + v186.z;
        }
        AI_EvalFunc_OBJSetPos_C_CURRENT(&pos);
        v204 = (tAI*)AI_EvalFunc_OBJDuplicate_C(
            (tGameObjectMain*)Hovercraft_shoot_HoverShoot_move_SFX_explosion_gao,
            &pos);
        Src.msgID = 1;
        Src.Int1 = 7;
        Src.f2 = 50.0;
        Src.f1 = 60.0;
        Src.v1.x = 1.0;
        Src.v1.y = 0.0;
        Src.v1.z = -1.0e14f;
        v71 = (tGameObjectMain*)v204;
        if (v204)
        {
            if ((unsigned int)v204 <= 2)
                v71 = AI_MainActor[(uint32_t)v204 - 1];
            AI_EvalFunc_MSGSend_C(v71, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
        v199.x = 0.0;
        v199.y = 0.0;
        v199.z = 1.0;
        v70 = (tGameObjectMain*)v204;
        if (v204)
        {
            if ((unsigned int)v204 <= 2)
                v70 = AI_MainActor[(uint32_t)v204 - 1];
            AI_EvalFunc_OBJ_SightSet_C(v70, &v199);
        }
        else
        {
            AI_EvalFunc_OBJ_SightSet_C(0, &v199);
        }
        if (v204 == WOR_gst_Universe.univAIInstance)
        {
            v67 = WOR_gst_Universe.univAIInstance;
        }
        else
        {
            v69 = (tGameObjectMain*)v204;
            if (v204)
            {
                if ((unsigned int)v204 <= 2)
                    v69 = AI_MainActor[(uint32_t)v204 - 1];
                v68 = v69;
            }
            else
            {
                v68 = 0;
            }
            v67 = v68->extended->AIInstance;
        }
        ((tScriptStruct_SFXExplosion*)(v67->instanceVarDataCurrent))->table_color[1] = 0xFFFFFF;
    }
    if (v208->i_Halo != -1)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v202);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v113);
        v195.x = v113.x * 10.0f;
        v195.y = v113.y * 10.0f;
        v195.z = v113.z * 10.0f;
        MATH_SubstractVec(&v228, &v202, &v195);
        AI_EvalFunc_GFXSetv_C_CURRENT(v208->i_Halo, 21200, &v228);
    }
    v63 = (tGameObjectMain*)v208->o_Target;
    if (v208->o_Target)
    {
        if ((unsigned int)v63 <= 2)
            v63 = AI_MainActor[(uint32_t)v63 - 1];
        v62 = v63;
    }
    else
    {
        v62 = 0;
    }
    if (v62)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v208->f_time_start, 3.0))
            v190 = 1;
    }
    else if (AI_EvalFunc_TIMEElapsed_C(v208->f_time_start, 1.0))
    {
        v190 = 1;
    }
    if (v190)
    {
        v61 = (tGameObjectMain*)v208->o_main_actor;
        if (v61)
        {
            if ((unsigned int)v61 <= 2)
                v61 = AI_MainActor[(uint32_t)v61 - 1];
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                v61,
                2,
                (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_move);
        }
        else
        {
            AI_EvalFunc_AICBDel_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_move);
        }
        AI_EvalFunc_AITrackChange_C(
            0,
            (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_death);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        return 1000;
    }
    v60 = (tGameObjectMain*)v208->o_Target;
    if (v208->o_Target)
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
        goto LABEL_271;
    if (v208->i_flag_perfect_shoot)
    {
        if (!v208->i_flag_sight_set)
        {
            v58 = (tGameObjectMain*)v208->o_Target;
            if (v208->o_Target)
            {
                if ((unsigned int)v58 <= 2)
                    v58 = AI_MainActor[(uint32_t)v58 - 1];
                AI_EvalFunc_OBJGetPos_C(v58, &v122);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v122);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v150);
            MATH_SubstractVec(&v91, &v122, &v150);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v200);
            v96 = v200.z * v91.z + v200.y * v91.y + v91.x * v200.x;
            if (v96 > 0.0)
            {
                v208->i_flag_sight_set = 1;
            LABEL_174:
                v54 = (tGameObjectMain*)v208->o_Target;
                if (v208->o_Target)
                {
                    if ((unsigned int)v54 <= 2)
                        v54 = AI_MainActor[(uint32_t)v54 - 1];
                    v5 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v54);
                }
                else
                {
                    v5 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
                }
                v209 = (float)v5;
                v94 = sqrtf(a4);
                a4 = v208->f_Distance / v94;
                v53 = (tGameObjectMain*)v208->o_Target;
                if (v208->o_Target)
                {
                    if ((unsigned int)v53 <= 2)
                        v53 = AI_MainActor[(uint32_t)v53 - 1];
                    AI_EvalFunc_OBJGetPos_C(v53, &v144);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v144);
                }
                v187 = v144;
                v52 = v209 / 40000.0f;
                if (v52 > 1.0)
                    v15 = 1.0;
                else
                    v15 = v209 / 40000.0f;
                v95 = v15;
                v183 = v15 * v208->v_rand_bv_pos.x;
                v184 = v208->v_rand_bv_pos.y * v15;
                v185 = v208->v_rand_bv_pos.z * v15;
                v187.x = v187.x + v183;
                v187.y = v187.y + v184;
                v187.z = v187.z + v185;
                v51 = (tGameObjectMain*)v208->o_Target;
                if (v208->o_Target)
                {
                    if ((unsigned int)v51 <= 2)
                        v51 = AI_MainActor[(uint32_t)v51 - 1];
                    AI_EvalFunc_BVMinGet_C(v51, &v148);
                }
                else
                {
                    AI_EvalFunc_BVMinGet_C(0, &v148);
                }
                v50 = (tGameObjectMain*)v208->o_Target;
                if (v208->o_Target)
                {
                    if ((unsigned int)v50 <= 2)
                        v50 = AI_MainActor[(uint32_t)v50 - 1];
                    AI_EvalFunc_BVMaxGet_C(v50, &v177);
                }
                else
                {
                    AI_EvalFunc_BVMaxGet_C(0, &v177);
                }
                MATH_VecAddVec(&v188, &v148, &v177);
                v196 = v188.x * 0.5f;
                v197 = v188.y * 0.5f;
                v198 = v188.z * 0.5f;
                v187.x = v187.x + v196;
                v187.y = v187.y + v197;
                v187.z = v187.z + v198;
                v49 = (1.0f - a4) * 10.0f;
                v103.x = v49 * v208->v_Sight.x;
                v103.y = v208->v_Sight.y * v49;
                v103.z = v208->v_Sight.z * v49;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v175);
                MATH_SubstractVec(&v154, &v187, &v175);
                v203.x = v154.x * a4;
                v203.y = v154.y * a4;
                v203.z = v154.z * a4;
                MATH_VecAddVec(&v165, &v103, &v203);
                v157 = v165;
                AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v157);
            LABEL_271:
                if (v208->GFX_Halo != -1)
                {
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v180);
                    GFXSetCurGlobal(v208->GFX_Halo);
                    AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v180);
                    v33 = v141;
                    if (v141)
                    {
                        if ((unsigned int)v141 <= 2)
                            v33 = AI_MainActor[(uint32_t)v141 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v33, &v149);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &v149);
                    }
                    AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &v149);
                    v119 = AI_EvalFunc_MATHRandFloat_C(10.0, 30.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v119);
                }
                if (v208->GFX_Halo_2 != -1)
                {
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v227);
                    GFXSetCurGlobal(v208->GFX_Halo_2);
                    AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v227);
                    v32 = v141;
                    if (v141)
                    {
                        if ((unsigned int)v141 <= 2)
                            v32 = AI_MainActor[(uint32_t)v141 - 1];
                        AI_EvalFunc_OBJ_BankingGet_C(v32, &a2);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_BankingGet_C(0, &a2);
                    }
                    v31 = v141;
                    if (v141)
                    {
                        if ((unsigned int)v141 <= 2)
                            v31 = AI_MainActor[(uint32_t)v141 - 1];
                        AI_EvalFunc_OBJ_HorizonGet_C(v31, &a3);
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_HorizonGet_C(0, &a3);
                    }
                    AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, 0.33000001f, &a5);
                    AI_EvalFunc_GFXSetv_C_CURGLOB(21201, &a5);
                    v97 = AI_EvalFunc_MATHRandFloat_C(5.0, 10.0);
                    AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v97);
                }
                if (v208->GFX_Line == -1)
                {
                    XMEN_OnOff(AI_gpst_CurrentGameObject, 0, 1);
                    XMEN_SetSize(AI_gpst_CurrentGameObject, 0, -1, v208->f_XMEN_width);
                    v106 = TIM_gf_dt;
                    v25 = 6.0f * TIM_gf_dt;
                    if (v25 < 0.0)
                        v25 = 0.0;
                    if (v25 > 1.0)
                        v25 = 1.0;
                    v210 = 15.0f * v25 + v208->f_XMEN_width * (1.0f - v25);
                    v208->f_XMEN_width = v210;
                }
                else
                {
                    v30 = (tGameObjectMain*)v208->o_main_actor;
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
                    v28 = (tGameObjectMain*)v208->o_aircraft;
                    if (v28)
                    {
                        if ((unsigned int)v28 <= 2)
                            v28 = AI_MainActor[(uint32_t)v28 - 1];
                        v27 = v28;
                    }
                    else
                    {
                        v27 = 0;
                    }
                    if (v29 == v27)
                    {
                        if (v208->f_Distance > 30.0)
                            v13 = 30.0;
                        else
                            v13 = v208->f_Distance;
                        v208->f_line_length = v13;
                    }
                    else
                    {
                        if (v208->f_Distance > 60.0)
                            v12 = 60.0;
                        else
                            v12 = v208->f_Distance;
                        v208->f_line_length = v12;
                    }
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v123);
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v92);
                    v26 = v208->f_line_length;
                    v201.x = v92.x * v26;
                    v201.y = v92.y * v26;
                    v201.z = v92.z * v26;
                    MATH_SubstractVec(&v152, &v123, &v201);
                    GFXSetCurGlobal(v208->GFX_Line);
                    AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &v152);
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v121);
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v90);
                    v102.x = v90.x + v90.x;
                    v102.y = v90.y + v90.y;
                    v102.z = v90.z + v90.z;
                    MATH_VecAddVec(&v207, &v121, &v102);
                    AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &v207);
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v167);
                    v132.x = -v167.x;
                    v132.y = -v167.y;
                    v132.z = -v167.z;
                    AI_EvalFunc_GFXSetv_C_CURGLOB(1202, &v132);
                }
                if (v172)
                {
                    if (v208->GFX_Flash != -1)
                    {
                        AI_EvalFunc_GFXDel_C_CURRENT(v208->GFX_Flash);
                        v208->GFX_Flash = -1;
                    }
                    AI_EvalFunc_COLRayNormalGet_C(AI_gpst_CurrentGameObject, &v166);
                    v186 = v166;
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v134);
                    v108 = v134.z * v186.z + v134.y * v186.y + v186.x * v134.x;
                    v11 = (float)fabs(v108);
                    v212 = v11;
                    v111 = v11;
                    if (v11 <= 0.98480803)
                    {
                        v24 = 2.0f * v111;
                        v186.x = v186.x * v24;
                        v186.y = v186.y * v24;
                        v186.z = v186.z * v24;
                        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v130);
                        v186.x = v186.x + v130.x;
                        v186.y = v186.y + v130.y;
                        v186.z = v186.z + v130.z;
                    }
                    else
                    {
                        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v205);
                        v170.x = -v205.x;
                        v170.y = -v205.y;
                        v170.z = -v205.z;
                        v186.x = -v205.x;
                        v186.y = -v205.y;
                        v186.z = -v205.z;
                    }
                    AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v186);
                }
                return 1000;
            }
        }
        if (!v208->i_flag_sight_set)
            goto LABEL_174;
        v57 = (tGameObjectMain*)v208->o_Target;
        if (v208->o_Target)
        {
            if ((unsigned int)v57 <= 2)
                v57 = AI_MainActor[(uint32_t)v57 - 1];
            AI_EvalFunc_OBJGetPos_C(v57, &v164);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v164);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v118);
        MATH_SubstractVec(&v168, &v164, &v118);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v89);
        v129 = v89.z * v168.z + v89.y * v168.y + v168.x * v89.x;
        if (v129 >= 0.0)
            goto LABEL_174;
        v56 = (tGameObjectMain*)v208->o_Target;
        if (v208->o_Target)
        {
            if ((unsigned int)v56 <= 2)
                v56 = AI_MainActor[(uint32_t)v56 - 1];
            AI_EvalFunc_OBJGetPos_C(v56, &v88);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v88);
        }
        AI_EvalFunc_OBJSetPos_C_CURRENT(&v88);
        v223 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_exec_impact);
        if (v223 == 1000)
        {
            v55 = (tGameObjectMain*)v208->o_main_actor;
            if (v55)
            {
                if ((unsigned int)v55 <= 2)
                    v55 = AI_MainActor[(uint32_t)v55 - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v55,
                    2,
                    (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_move);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_move);
            }
            AI_EvalFunc_AITrackChange_C(
                0,
                (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_death);
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            result = v223;
        }
    }
    else
    {
        v48 = (tGameObjectMain*)v208->o_Target;
        if (v208->o_Target)
        {
            if ((unsigned int)v48 <= 2)
                v48 = AI_MainActor[(uint32_t)v48 - 1];
            v6 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v48);
        }
        else
        {
            v6 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
        }
        a4 = (float)v6;
        if (a4 >= 400.0)
        {
            v39 = (tGameObjectMain*)v208->o_Target;
            if (v208->o_Target)
            {
                if ((unsigned int)v39 <= 2)
                    v39 = AI_MainActor[(uint32_t)v39 - 1];
                v10 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v39);
            }
            else
            {
                v10 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
            }
            v209 = (float)v10;
            v160 = sqrtf(a4);
            a4 = v208->f_Distance / v160;
            v38 = (tGameObjectMain*)v208->o_Target;
            if (v208->o_Target)
            {
                if ((unsigned int)v38 <= 2)
                    v38 = AI_MainActor[(uint32_t)v38 - 1];
                AI_EvalFunc_OBJGetPos_C(v38, &v143);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v143);
            }
            v187 = v143;
            v37 = v209 / 40000.0f;
            if (v37 > 1.0)
                v14 = 1.0;
            else
                v14 = v209 / 40000.0f;
            v231 = v14;
            v215 = v14 * v208->v_rand_bv_pos.x;
            v216 = v208->v_rand_bv_pos.y * v14;
            v217 = v208->v_rand_bv_pos.z * v14;
            v187.x = v187.x + v215;
            v187.y = v187.y + v216;
            v187.z = v187.z + v217;
            v36 = (tGameObjectMain*)v208->o_Target;
            if (v208->o_Target)
            {
                if ((unsigned int)v36 <= 2)
                    v36 = AI_MainActor[(uint32_t)v36 - 1];
                AI_EvalFunc_BVMinGet_C(v36, &v115);
            }
            else
            {
                AI_EvalFunc_BVMinGet_C(0, &v115);
            }
            v35 = (tGameObjectMain*)v208->o_Target;
            if (v208->o_Target)
            {
                if ((unsigned int)v35 <= 2)
                    v35 = AI_MainActor[(uint32_t)v35 - 1];
                AI_EvalFunc_BVMaxGet_C(v35, &v220);
            }
            else
            {
                AI_EvalFunc_BVMaxGet_C(0, &v220);
            }
            MATH_VecAddVec(&v125, &v115, &v220);
            v136 = v125.x * 0.5f;
            v137 = v125.y * 0.5f;
            v138 = v125.z * 0.5f;
            v187.x = v187.x + v136;
            v187.y = v187.y + v137;
            v187.z = v187.z + v138;
            v34 = (1.0f - a4) * 10.0f;
            v117.x = v34 * v208->v_Sight.x;
            v117.y = v208->v_Sight.y * v34;
            v117.z = v208->v_Sight.z * v34;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v182);
            MATH_SubstractVec(&v146, &v187, &v182);
            v116.x = v146.x * a4;
            v116.y = v146.y * a4;
            v116.z = v146.z * a4;
            MATH_VecAddVec(&v224, &v117, &v116);
            v157 = v224;
            AI_EvalFunc_OBJ_SightSet_C(AI_gpst_CurrentGameObject, &v157);
            goto LABEL_271;
        }
        v47 = (tGameObjectMain*)v208->o_main_actor;
        if (v47)
        {
            if ((unsigned int)v47 <= 2)
                v47 = AI_MainActor[(uint32_t)v47 - 1];
            v7 = AI_EvalFunc_OBJ_FlagsControlGet_C(v47);
        }
        else
        {
            v7 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
        }
        v93 = v7;
        v213 = v7 & 0x1000000;
        v46 = (tGameObjectMain*)v208->o_main_actor;
        if (v46)
        {
            if ((unsigned int)v46 <= 2)
                v46 = AI_MainActor[(uint32_t)v46 - 1];
            v8 = AI_EvalFunc_OBJ_FlagsControlSet_C(v46, 0x1000000, 0);
        }
        else
        {
            v8 = AI_EvalFunc_OBJ_FlagsControlSet_C(0, 0x1000000, 0);
        }
        v98 = v8;
        AI_EvalFunc_COL_SpecificCrossableSet_C(0xFFFFu);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v226);
        v45 = (tGameObjectMain*)v208->o_Target;
        if (v208->o_Target)
        {
            if ((unsigned int)v45 <= 2)
                v45 = AI_MainActor[(uint32_t)v45 - 1];
            AI_EvalFunc_OBJGetPos_C(v45, &v104);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v104);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v174);
        MATH_SubstractVec(&v181, &v104, &v174);
        v99 = AI_EvalFunc_COLRayObjectVector_C(AI_gpst_CurrentGameObject, &v226, &v181, -1, 0, 0, 0x80000001);
        if (!v213)
        {
            v44 = (tGameObjectMain*)v208->o_main_actor;
            if (v44)
            {
                if ((unsigned int)v44 <= 2)
                    v44 = AI_MainActor[(uint32_t)v44 - 1];
                v9 = AI_EvalFunc_OBJ_FlagsControlSet_C(v44, 0, 0x1000000);
            }
            else
            {
                v9 = AI_EvalFunc_OBJ_FlagsControlSet_C(0, 0, 0x1000000);
            }
            v112 = v9;
        }
        v43 = v99;
        if (v99)
        {
            if ((unsigned int)v99 <= 2)
                v43 = AI_MainActor[(uint32_t)v99 - 1];
            v42 = v43;
        }
        else
        {
            v42 = 0;
        }
        if (v42)
        {
            v208->o_Target = (int)v99;
            AI_EvalFunc_COLRayPosGet_C(AI_gpst_CurrentGameObject, &v140);
            pos = v140;
            AI_EvalFunc_OBJSetPos_C_CURRENT(&pos);
        }
        else
        {
            v41 = (tGameObjectMain*)v208->o_Target;
            if (v208->o_Target)
            {
                if ((unsigned int)v41 <= 2)
                    v41 = AI_MainActor[(uint32_t)v41 - 1];
                AI_EvalFunc_OBJGetPos_C(v41, &v153);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v153);
            }
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v153);
        }
        v101 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_exec_impact);
        if (v101 == 1000)
        {
            v40 = (tGameObjectMain*)v208->o_main_actor;
            if (v40)
            {
                if ((unsigned int)v40 <= 2)
                    v40 = AI_MainActor[(uint32_t)v40 - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v40,
                    2,
                    (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_move);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_move);
            }
            AI_EvalFunc_AITrackChange_C(
                0,
                (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_move_Hovercraft_shoot_HoverShoot_death);
            *AI_gppst_CurrentJumpNode = (tAINode*)-1;
            result = 1000;
        }
        else
        {
            result = v101;
        }
    }
    return result;
}

int Hovercraft_shoot_HoverShoot_reflex()
{
    unsigned int v0; // eax
    int v1; // eax
    unsigned int v2; // eax
    tScriptStruct_HoverCraftShoot* v4; // [esp+0h] [ebp-5Ch]
    tGameObjectMain* v5; // [esp+8h] [ebp-54h]
    tGameObjectMain* v6; // [esp+Ch] [ebp-50h]
    tGameObjectMain* v7; // [esp+10h] [ebp-4Ch]
    tGameObjectMain* v8; // [esp+18h] [ebp-44h]
    tGameObjectMain* v9; // [esp+1Ch] [ebp-40h]
    tGameObjectMain* v10; // [esp+20h] [ebp-3Ch]
    tGameObjectMain* v11; // [esp+28h] [ebp-34h]
    tGameObjectMain* v12; // [esp+2Ch] [ebp-30h]
    tGameObjectMain* v13; // [esp+30h] [ebp-2Ch]
    tGameObjectMain* v14; // [esp+38h] [ebp-24h]
    tGameObjectMain* v15; // [esp+3Ch] [ebp-20h]
    tGameObjectMain* v16; // [esp+40h] [ebp-1Ch]
    tGameObjectMain* v17; // [esp+44h] [ebp-18h]

    if (AI_gpst_CurrentInstance)
        v4 = (tScriptStruct_HoverCraftShoot*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v4 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Hovercraft_shoot_HoverShoot_reflex_Hovercraft_shoot_HoverShoot_move = (int)AI_ul_RealResolveOneRefFunction(0x49020712u);
        fct_Hovercraft_shoot_HoverShoot_reflex_Hovercraft_shoot_HoverShoot_death = (int)AI_ul_RealResolveOneRefFunction(0x72004FC3u);
    }
    v4->i_flag_1st_trame = 0;
    v17 = AI_EvalFunc_OBJ_HierarchyGet_C(AI_gpst_CurrentGameObject);
    v16 = v17;
    if (v17)
    {
        if ((unsigned int)v17 <= 2)
            v16 = AI_MainActor[(uint32_t)v17 - 1];
        v15 = v16;
    }
    else
    {
        v15 = 0;
    }
    if (v15)
    {
        v14 = v17;
        if (v17)
        {
            if ((unsigned int)v17 <= 2)
                v14 = AI_MainActor[(uint32_t)v17 - 1];
            v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(v14);
        }
        else
        {
            v0 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
        }
        if ((v0 & 0x200000) != 0)
            AI_EvalFunc_OBJ_HierarchyReset_C(AI_gpst_CurrentGameObject);
    }
    v13 = (tGameObjectMain*)v4->o_Target;
    if (v4->o_Target)
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
        v11 = (tGameObjectMain*)v4->o_Target;
        if (v4->o_Target)
        {
            if ((unsigned int)v11 <= 2)
                v11 = AI_MainActor[(uint32_t)v11 - 1];
            v1 = AI_EvalFunc_OBJ_HasBeenKilled_C(v11);
        }
        else
        {
            v1 = AI_EvalFunc_OBJ_HasBeenKilled_C(0);
        }
        if (v1)
            v4->o_Target = 0;
    }
    v10 = (tGameObjectMain*)v4->o_Target;
    if (v4->o_Target)
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
        v8 = (tGameObjectMain*)v4->o_Target;
        if (v4->o_Target)
        {
            if ((unsigned int)v8 <= 2)
                v8 = AI_MainActor[(uint32_t)v8 - 1];
            v2 = AI_EvalFunc_OBJ_FlagsControlGet_C(v8);
        }
        else
        {
            v2 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
        }
        if ((v2 & 0x200000) != 0)
            v4->o_Target = 0;
    }
    if (v4->i_flag_target)
    {
        v7 = (tGameObjectMain*)v4->o_Target;
        if (v4->o_Target)
        {
            if ((unsigned int)v7 <= 2)
                v7 = AI_MainActor[(uint32_t)v7 - 1];
            v6 = v7;
        }
        else
        {
            v6 = 0;
        }
        if (!v6)
        {
            v5 = (tGameObjectMain*)v4->o_main_actor;
            if (v5)
            {
                if ((unsigned int)v5 <= 2)
                    v5 = AI_MainActor[(uint32_t)v5 - 1];
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    v5,
                    2,
                    (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_reflex_Hovercraft_shoot_HoverShoot_move);
            }
            else
            {
                AI_EvalFunc_AICBDel_C(
                    AI_gpst_CurrentGameObject,
                    0,
                    2,
                    (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_reflex_Hovercraft_shoot_HoverShoot_move);
            }
            AI_EvalFunc_AITrackChange_C(
                0,
                (tAIFunction*)fct_Hovercraft_shoot_HoverShoot_reflex_Hovercraft_shoot_HoverShoot_death);
        }
    }
    return 1000;
}

int Hovercraft_shoot_HoverShoot_exec_impact()
{
    int v0; // eax
    tGameObjectMain* v1; // eax
    tGameObjectMain* v2; // eax
    tGameObjectMain* v3; // eax
    BOOL v4; // eax
    int v6; // eax
    tScriptStruct_HoverCraftShoot* v7; // [esp+0h] [ebp-30Ch]
    tGameObjectMain* v8; // [esp+8h] [ebp-304h]
    tGameObjectMain* v9; // [esp+10h] [ebp-2FCh]
    tGameObjectMain* v10; // [esp+18h] [ebp-2F4h]
    tGameObjectMain* v11; // [esp+20h] [ebp-2ECh]
    tGameObjectMain* v12; // [esp+28h] [ebp-2E4h]
    tGameObjectMain* v13; // [esp+38h] [ebp-2D4h]
    tGameObjectMain* v14; // [esp+40h] [ebp-2CCh]
    tGameObjectMain* v15; // [esp+48h] [ebp-2C4h]
    tGameObjectMain* v16; // [esp+58h] [ebp-2B4h]
    tGameObjectMain* v17; // [esp+6Ch] [ebp-2A0h]
    tGameObjectMain* v18; // [esp+70h] [ebp-29Ch]
    tGameObjectMain* v19; // [esp+78h] [ebp-294h]
    tGameObjectMain* v20; // [esp+80h] [ebp-28Ch]
    tGameObjectMain* v21; // [esp+88h] [ebp-284h]
    tGameObjectMain* v22; // [esp+90h] [ebp-27Ch]
    tGameObjectMain* v23; // [esp+94h] [ebp-278h]
    tGameObjectMain* v24; // [esp+98h] [ebp-274h]
    tGameObjectMain* v25; // [esp+A0h] [ebp-26Ch]
    tGameObjectMain* v26; // [esp+BCh] [ebp-250h]
    tGameObjectMain* v27; // [esp+C0h] [ebp-24Ch]
    Vec3 rayV1; // [esp+C4h] [ebp-248h] BYREF
    Vec3 v29; // [esp+D0h] [ebp-23Ch] BYREF
    Vec3 v31; // [esp+E0h] [ebp-22Ch] BYREF
    int v32; // [esp+ECh] [ebp-220h]
    Vec3 v33; // [esp+F0h] [ebp-21Ch] BYREF
    Vec3 v34; // [esp+100h] [ebp-20Ch] BYREF
    Vec3 v35; // [esp+10Ch] [ebp-200h] BYREF
    tMessage Src; // [esp+118h] [ebp-1F4h] BYREF
    int v37; // [esp+158h] [ebp-1B4h]
    Vec3 v38; // [esp+15Ch] [ebp-1B0h] BYREF
    Vec3 a3; // [esp+168h] [ebp-1A4h] BYREF
    Vec3 v40; // [esp+174h] [ebp-198h] BYREF
    Vec3 v41; // [esp+180h] [ebp-18Ch] BYREF
    Vec3 v42; // [esp+18Ch] [ebp-180h] BYREF
    Vec3 v43; // [esp+198h] [ebp-174h] BYREF
    tGameObjectMain* v44; // [esp+1A4h] [ebp-168h]
    Vec3 v45; // [esp+1A8h] [ebp-164h] BYREF
    Vec3 v46; // [esp+1B4h] [ebp-158h] BYREF
    Vec3 rayV2; // [esp+1C0h] [ebp-14Ch] BYREF
    int v48; // [esp+1CCh] [ebp-140h]
    Vec3 v49; // [esp+1D0h] [ebp-13Ch] BYREF
    Vec3 v50; // [esp+1DCh] [ebp-130h] BYREF
    Vec3 v51; // [esp+1E8h] [ebp-124h] BYREF
    Vec3 v52; // [esp+1F4h] [ebp-118h] BYREF
    Vec3 v53; // [esp+200h] [ebp-10Ch] BYREF
    Vec3 v54; // [esp+20Ch] [ebp-100h] BYREF
    Vec3 v55; // [esp+218h] [ebp-F4h] BYREF
    Vec3 v56; // [esp+224h] [ebp-E8h] BYREF
    Vec3 v57; // [esp+238h] [ebp-D4h] BYREF
    Vec3 v58; // [esp+244h] [ebp-C8h]
    Vec3 v59; // [esp+254h] [ebp-B8h] BYREF
    BOOL v60; // [esp+260h] [ebp-ACh]
    int v61; // [esp+264h] [ebp-A8h]
    tGameObjectMain* v62; // [esp+268h] [ebp-A4h]
    tGameObjectMain* v63; // [esp+26Ch] [ebp-A0h]
    Vec3 v64; // [esp+27Ch] [ebp-90h] BYREF
    Vec3 v65; // [esp+294h] [ebp-78h] BYREF
    Vec3 v66; // [esp+2A0h] [ebp-6Ch] BYREF
    tScriptStruct_HoverCraftShoot* v67; // [esp+2ACh] [ebp-60h]
    int gfxId; // [esp+2B0h] [ebp-5Ch]
    Vec3 v69; // [esp+2B4h] [ebp-58h] BYREF
    Vec3 dest; // [esp+2C0h] [ebp-4Ch] BYREF
    Vec3 v71; // [esp+2CCh] [ebp-40h] BYREF
    Vec3 v72; // [esp+2D8h] [ebp-34h] BYREF
    int v73; // [esp+2E4h] [ebp-28h]
    Vec3 v74; // [esp+2E8h] [ebp-24h] BYREF
    float v75; // [esp+2F4h] [ebp-18h]
    float v76; // [esp+2F8h] [ebp-14h]
    float v77; // [esp+2FCh] [ebp-10h]

    v55.x = 1.0;
    v55.y = 0.0;
    v55.z = 0.0;
    v43.x = 1.0;
    v43.y = 0.0;
    v43.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_HoverCraftShoot*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    v67 = v7;
    v61 = 0;
    v44 = 0;
    gfxId = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Hovercraft_shoot_HoverShoot_exec_impact_Hovercraft_shoot_HoverShoot_exec_GFX_Flash = (int)AI_ul_RealResolveOneRefFunction(0x72005273u);
        Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    v27 = (tGameObjectMain*)v67->o_Target;
    if (v67->o_Target)
    {
        if ((unsigned int)v27 <= 2)
            v27 = AI_MainActor[(uint32_t)v27 - 1];
        v26 = v27;
    }
    else
    {
        v26 = 0;
    }
    if (!v26)
        return 1000;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v38);
    v43 = v38;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v65);
    v58.x = -v65.x;
    v58.y = -v65.y;
    v58.z = -v65.z;
    v55.x = -v65.x;
    v55.y = -v65.y;
    v55.z = -v65.z;
    v25 = (tGameObjectMain*)v67->o_Target;
    if (v67->o_Target)
    {
        if ((unsigned int)v25 <= 2)
            v25 = AI_MainActor[(uint32_t)v25 - 1];
        v0 = AI_EvalFunc_OBJ_FlagsIdentityGet_C(v25);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_FlagsIdentityGet_C(0);
    }
    v32 = v0;
    if ((v0 & 0x4002) != 0)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v29);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v49);
        MATH_SubstractVec(&dest, &v29, &v49);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v35);
        v44 = AI_EvalFunc_COLRayObjectDist_C(AI_gpst_CurrentGameObject, &dest, &v35, 30.0, 16640, 0, 0, 0);
        v24 = v44;
        if (v44)
        {
            if ((int)v44 <= 2)
                v24 = AI_MainActor[(int)v44 - 1];
            v23 = v24;
        }
        else
        {
            v23 = 0;
        }
        if (!v23)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v69);
            v22 = (tGameObjectMain*)v67->o_Target;
            if (v67->o_Target)
            {
                if ((unsigned int)v22 <= 2)
                    v22 = AI_MainActor[(uint32_t)v22 - 1];
                AI_EvalFunc_OBJGetPos_C(v22, &a3);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &a3);
            }
            MATH_SubstractVec(&v54, &a3, &v67->v_last_pos);
            AI_EvalFunc_MATHVecNormalize_C(&v54, &v33);
            v44 = AI_EvalFunc_COLRayObjectDist_C(AI_gpst_CurrentGameObject, &v69, &v33, 30.0, 16640, 0, 0, 0);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v53);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v40);
        v46.x = v40.x * 5.0f;
        v46.y = v40.y * 5.0f;
        v46.z = v40.z * 5.0f;
        MATH_SubstractVec(&rayV1, &v53, &v46);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&rayV2);
        v21 = (tGameObjectMain*)v67->o_Target;
        if (v67->o_Target)
        {
            if ((unsigned int)v21 <= 2)
                v21 = AI_MainActor[(uint32_t)v21 - 1];
            v1 = AI_EvalFunc_COLRaySingleObjectDist_C(v21, &rayV1, &rayV2, 30.0, -1, 0, 0, 0);
        }
        else
        {
            v1 = AI_EvalFunc_COLRaySingleObjectDist_C(0, &rayV1, &rayV2, 30.0, -1, 0, 0, 0);
        }
        v63 = v1;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v56);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v74);
        v57.x = v74.x * 5.0f;
        v57.y = v74.y * 5.0f;
        v57.z = v74.z * 5.0f;
        MATH_SubstractVec(&v42, &v56, &v57);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v41);
        v20 = (tGameObjectMain*)v67->o_Target;
        if (v67->o_Target)
        {
            if ((unsigned int)v20 <= 2)
                v20 = AI_MainActor[(uint32_t)v20 - 1];
            v2 = AI_EvalFunc_COLRaySingleObjectDist_C(v20, &v42, &v41, 30.0, -1, 0, 0, 0);
        }
        else
        {
            v2 = AI_EvalFunc_COLRaySingleObjectDist_C(0, &v42, &v41, 30.0, -1, 0, 0, 0);
        }
        v62 = v2;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v31);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v71);
        v50.x = v71.x * 5.0f;
        v50.y = v71.y * 5.0f;
        v50.z = v71.z * 5.0f;
        MATH_SubstractVec(&v66, &v31, &v50);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v52);
        v19 = (tGameObjectMain*)v67->o_Target;
        if (v67->o_Target)
        {
            if ((unsigned int)v19 <= 2)
                v19 = AI_MainActor[(uint32_t)v19 - 1];
            v3 = AI_EvalFunc_COLRaySingleObjectDist_C(v19, &v66, &v52, 30.0, -1, 0, 0, 0);
        }
        else
        {
            v3 = AI_EvalFunc_COLRaySingleObjectDist_C(0, &v66, &v52, 30.0, -1, 0, 0, 0);
        }
        v62 = v3;
        v18 = v44;
        if (v44)
        {
            if ((int)v44 <= 2)
                v18 = AI_MainActor[(int)v44 - 1];
            v17 = v18;
        }
        else
        {
            v17 = 0;
        }
        if (v17)
        {
            AI_EvalFunc_COLRayNormalGet_C(AI_gpst_CurrentGameObject, &v72);
            v55 = v72;
            AI_EvalFunc_COLRayPosGet_C(AI_gpst_CurrentGameObject, &v34);
            v75 = v55.x * 0.5f;
            v76 = v55.y * 0.5f;
            v77 = v55.z * 0.5f;
            v43.x = v34.x + v75;
            v43.y = v34.y + v76;
            v43.z = v34.z + v77;
            AI_EvalFunc_OBJSetPos_C_CURRENT(&v43);
        }
    }
    v16 = (tGameObjectMain*)v67->o_Target;
    if (v67->o_Target)
    {
        if ((unsigned int)v16 <= 2)
            v16 = AI_MainActor[(uint32_t)v16 - 1];
        v4 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v16, 0x8000);
    }
    else
    {
        v4 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x8000);
    }
    v60 = v4;
    if (v4)
    {
        if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
        {
            Src.msgID = 37;
            Src.gao = AI_gpst_CurrentGameObject;
            Src.gao2 = 0;
            Src.Int1 = 37;
            Src.Int2 = 1;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&Src.v1);
            Src.v2 = v43;
            Src.f2 = 20.0;
            Src.f1 = 1000.0;
            v15 = (tGameObjectMain*)v67->o_Target;
            if (v67->o_Target)
            {
                if ((unsigned int)v15 <= 2)
                    v15 = AI_MainActor[(uint32_t)v15 - 1];
                AI_EvalFunc_MSGSend_C(v15, &Src);
            }
            else
            {
                AI_EvalFunc_MSGSend_C(0, &Src);
            }
            v73 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Hovercraft_shoot_HoverShoot_exec_impact_Hovercraft_shoot_HoverShoot_exec_GFX_Flash);
            if (v73 != 1000)
                return v73;
            v61 = AI_EvalFunc_GFXAdd_C_CURRENT(9);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v61, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v61, 2, 1);
            v14 = (tGameObjectMain*)Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao;
            if (Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao)
            {
                if ((unsigned int)Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao <= 2)
                    v14 = AI_MainActor[Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v61, v14, 5);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v61, 0, 5);
            }
            GFXSetCurGlobal(v61);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9000, 1.570796f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9001, 80.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9002, 120.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9003, 0.94999999f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9004, 0.60000002f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9005, 0.40000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9006, 0.80000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9007, 4.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9008, 0.0049999999f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9009, 0.40000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(9010, 0.80000001f);
            AI_EvalFunc_GFXSeti_C_CURGLOB(9100, 40);
            AI_EvalFunc_GFXSeti_C_CURGLOB(9102, 24);
            AI_EvalFunc_GFXSetv_C_CURGLOB(9200, &v43);
            v45.x = 0.0;
            v45.y = 0.0;
            v45.z = 1.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(9201, &v45);
            v59.x = 0.0;
            v59.y = 0.0;
            v59.z = -10.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(9202, &v59);
            AI_EvalFunc_GFXSeti_C_CURGLOB(9101, 0xFFFFB0);
            goto LABEL_105;
        }
        v13 = (tGameObjectMain*)v67->o_Target;
        if (v67->o_Target)
        {
            if ((unsigned int)v13 <= 2)
                v13 = AI_MainActor[(uint32_t)v13 - 1];
            v6 = AI_EvalFunc_OBJ_FlagsDesignGet_C(v13);
        }
        else
        {
            v6 = AI_EvalFunc_OBJ_FlagsDesignGet_C(0);
        }
        v37 = v6;
        if ((v6 & 2) != 0)
        {
            v48 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Hovercraft_shoot_HoverShoot_exec_impact_Hovercraft_shoot_HoverShoot_exec_GFX_Flash);
            if (v48 != 1000)
                return v48;
        }
        Src.msgID = 37;
        Src.gao = AI_gpst_CurrentGameObject;
        Src.gao2 = 0;
        Src.Int1 = 36;
        Src.Int2 = 1;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&Src.v1);
        Src.v2 = v43;
        Src.f2 = 5.0;
        Src.f1 = 1000.0;
        v12 = (tGameObjectMain*)v67->o_Target;
        if (v67->o_Target)
        {
            if ((unsigned int)v12 <= 2)
                v12 = AI_MainActor[(uint32_t)v12 - 1];
            AI_EvalFunc_MSGSend_C(v12, &Src);
        }
        else
        {
            AI_EvalFunc_MSGSend_C(0, &Src);
        }
        v61 = AI_EvalFunc_GFXAdd_C_CURRENT(9);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v61, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v61, 2, 1);
        v11 = (tGameObjectMain*)Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao;
        if (Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao)
        {
            if ((unsigned int)Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao <= 2)
                v11 = AI_MainActor[Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v61, v11, 5);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v61, 0, 5);
        }
        GFXSetCurGlobal(v61);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9000, 1.570796f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9001, 20.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9002, 80.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9003, 0.98000002f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9004, 0.89999998f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9005, 0.1f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9006, 0.2f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9007, 3.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9008, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9009, 0.2f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9010, 0.40000001f);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9100, 20);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9102, 24);
        AI_EvalFunc_GFXSetv_C_CURGLOB(9200, &v43);
        AI_EvalFunc_GFXSetv_C_CURGLOB(9201, &v55);
        v64.x = 0.0;
        v64.y = 0.0;
        v64.z = -20.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(9202, &v64);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9101, 0xFFFFB0);
        gfxId = AI_EvalFunc_GFXAdd_C_CURRENT(21);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(gfxId, 4, 1);
        v10 = (tGameObjectMain*)Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao;
        if (Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao)
        {
            if ((unsigned int)Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao <= 2)
                v10 = AI_MainActor[Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(gfxId, v10, 29);
            goto LABEL_104;
        }
    }
    else
    {
        v61 = AI_EvalFunc_GFXAdd_C_CURRENT(9);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v61, 0, 1);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v61, 2, 1);
        v9 = (tGameObjectMain*)Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao;
        if (Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao)
        {
            if ((unsigned int)Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao <= 2)
                v9 = AI_MainActor[Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v61, v9, 5);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v61, 0, 5);
        }
        GFXSetCurGlobal(v61);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9000, 1.570796f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9001, 20.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9002, 80.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9003, 0.98000002f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9004, 0.89999998f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9005, 0.1f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9006, 0.2f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9007, 3.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9008, 0.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9009, 0.2f);
        AI_EvalFunc_GFXSetf_C_CURGLOB(9010, 0.40000001f);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9100, 20);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9102, 24);
        AI_EvalFunc_GFXSetv_C_CURGLOB(9200, &v43);
        AI_EvalFunc_GFXSetv_C_CURGLOB(9201, &v55);
        v51.x = 0.0;
        v51.y = 0.0;
        v51.z = -20.0;
        AI_EvalFunc_GFXSetv_C_CURGLOB(9202, &v51);
        AI_EvalFunc_GFXSeti_C_CURGLOB(9101, 0xFFFFB0);
        gfxId = AI_EvalFunc_GFXAdd_C_CURRENT(21);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(gfxId, 4, 1);
        v8 = (tGameObjectMain*)Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao;
        if (Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao)
        {
            if ((unsigned int)Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao <= 2)
                v8 = AI_MainActor[Hovercraft_shoot_HoverShoot_exec_impact_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(gfxId, v8, 29);
            goto LABEL_104;
        }
    }
    AI_EvalFunc_GFXMaterialSet_C_CURRENT(gfxId, 0, 29);
LABEL_104:
    GFXSetCurGlobal(gfxId);
    AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 4.0);
    AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
    AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 0xC6C9B8);
    AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 5.0);
    AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v43);
    AI_EvalFunc_GFXSetf_C_CURGLOB(21005, 0.1f);
    AI_EvalFunc_GFXSeti_C_CURGLOB(21100, 16);
    AI_EvalFunc_GFXLifeTimeSet_C(AI_gpst_CurrentGameObject, gfxId, 0.1f);
LABEL_105:
    if (!v67->i_flag_perfect_shoot)
        v67->o_Target = 0;
    return 1000;
}

int Hovercraft_shoot_HoverShoot_exec_GFX_Flash()
{
    tScriptStruct_HoverCraftShoot* v1; // [esp+0h] [ebp-60h]
    tGameObjectMain* v2; // [esp+Ch] [ebp-54h]
    tGameObjectMain* v3; // [esp+14h] [ebp-4Ch]
    Vec3 a1; // [esp+18h] [ebp-48h] BYREF
    Vec3 a3; // [esp+24h] [ebp-3Ch] BYREF
    tGameObjectMain* v6; // [esp+30h] [ebp-30h]
    Vec3 v7; // [esp+34h] [ebp-2Ch] BYREF
    Vec3 dest; // [esp+44h] [ebp-1Ch] BYREF
    BOOL v9; // [esp+54h] [ebp-Ch]
    tScriptStruct_HoverCraftShoot* v10; // [esp+58h] [ebp-8h]
    float v11; // [esp+5Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_HoverCraftShoot*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v10 = v1;
    v6 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Hovercraft_shoot_HoverShoot_exec_GFX_Flash_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    if (v10->GFX_Flash == -1)
    {
        v10->GFX_Flash = AI_EvalFunc_GFXAdd_C_CURRENT(18);
        v3 = (tGameObjectMain*)Hovercraft_shoot_HoverShoot_exec_GFX_Flash_SFX_light_smoke_gao;
        if (Hovercraft_shoot_HoverShoot_exec_GFX_Flash_SFX_light_smoke_gao)
        {
            if ((unsigned int)Hovercraft_shoot_HoverShoot_exec_GFX_Flash_SFX_light_smoke_gao <= 2)
                v3 = AI_MainActor[Hovercraft_shoot_HoverShoot_exec_GFX_Flash_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v10->GFX_Flash, v3, 11);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v10->GFX_Flash, 0, 11);
        }
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v10->GFX_Flash, 2, 1);
        GFXSetCurGlobal(v10->GFX_Flash);
        AI_EvalFunc_GFXSetf_C_CURGLOB(18010, 3.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(18012, 10000.0);
        v9 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100);
        GFXSetCurGlobal(v10->GFX_Flash);
        AI_EvalFunc_GFXSetf_C_CURGLOB(18000, 0.2f);
        if (v9)
        {
            AI_EvalFunc_GFXSetf_C_CURGLOB(18001, 10.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18002, 25.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18003, 6.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18004, 12.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18005, 0.2f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18006, 6.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18007, 12.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18008, 8.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18009, 16.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18011, 0.0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(18101, 16);
            AI_EvalFunc_GFXSeti_C_CURGLOB(18102, -1);
        }
        else
        {
            AI_EvalFunc_GFXSetf_C_CURGLOB(18001, 4.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18002, 5.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18003, 1.5);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18004, 3.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18005, 0.2f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18006, 1.5);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18007, 3.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18008, 2.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18009, 4.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18011, 0.0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(18101, 16);
            AI_EvalFunc_GFXSeti_C_CURGLOB(18102, 0x80FFFFFF);
        }
        AI_EvalFunc_GFXSeti_C_CURGLOB(18103, 4);
        AI_EvalFunc_GFXSeti_C_CURGLOB(18100, 1);
    }
    v6 = AI_EvalFunc_VIEWGetObject_C(0);
    v2 = v6;
    if (v6)
    {
        if ((unsigned int)v6 <= 2)
            v2 = AI_MainActor[(uint32_t)v6 - 1];
        AI_EvalFunc_OBJGetPos_C(v2, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
    MATH_SubstractVec(&dest, &a3, &a1);
    v11 = MATH_VecLen(&dest);
    GFXSetCurGlobal(v10->GFX_Flash);
    AI_EvalFunc_GFXSetf_C_CURGLOB(18013, v11);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v7);
    AI_EvalFunc_GFXSetv_C_CURGLOB(18200, &v7);
    return 1000;
}

int Hovercraft_shoot_HoverShoot_death()
{
    int result; // eax
    float v1; // [esp+8h] [ebp-118h]
    float v2; // [esp+Ch] [ebp-114h]
    tScriptStruct_HoverCraftShoot* v3; // [esp+10h] [ebp-110h]
    tGameObjectMain* v4; // [esp+1Ch] [ebp-104h]
    float v5; // [esp+2Ch] [ebp-F4h]
    tGameObjectMain* v6; // [esp+38h] [ebp-E8h]
    tGameObjectMain* v7; // [esp+3Ch] [ebp-E4h]
    tGameObjectMain* v8; // [esp+40h] [ebp-E0h]
    float v9; // [esp+44h] [ebp-DCh]
    Vec3 v10; // [esp+48h] [ebp-D8h] BYREF
    int v11; // [esp+58h] [ebp-C8h]
    float v12; // [esp+5Ch] [ebp-C4h]
    float v13; // [esp+60h] [ebp-C0h]
    float v14; // [esp+64h] [ebp-BCh]
    Vec3 v15; // [esp+68h] [ebp-B8h] BYREF
    Vec3 v16; // [esp+74h] [ebp-ACh] BYREF
    int v18; // [esp+88h] [ebp-98h]
    int v19; // [esp+8Ch] [ebp-94h]
    Vec3 v20; // [esp+90h] [ebp-90h] BYREF
    float v21; // [esp+9Ch] [ebp-84h]
    Vec3 v22; // [esp+A0h] [ebp-80h] BYREF
    Vec3 v23; // [esp+ACh] [ebp-74h] BYREF
    float v25; // [esp+BCh] [ebp-64h]
    Vec3 a3; // [esp+C0h] [ebp-60h] BYREF
    Vec3 v27; // [esp+CCh] [ebp-54h] BYREF
    tGameObjectMain* v28; // [esp+D8h] [ebp-48h]
    Vec3 a1; // [esp+E0h] [ebp-40h] BYREF
    BOOL v31; // [esp+ECh] [ebp-34h]
    tScriptStruct_HoverCraftShoot* v32; // [esp+F4h] [ebp-2Ch]
    Vec3 dest; // [esp+F8h] [ebp-28h] BYREF
    Vec3 v34; // [esp+104h] [ebp-1Ch] BYREF
    Vec3 v35; // [esp+110h] [ebp-10h] BYREF

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_HoverCraftShoot*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v32 = v3;
    v12 = 0.0;
    v28 = AI_EvalFunc_VIEWGetObject_C(0);
    if (v3->f_time_impact == 0.0)
        v32->f_time_impact = TIM_gf_MainClock - TIM_gf_LockTime;
    v8 = (tGameObjectMain*)v32->o_Target;
    if (v32->o_Target)
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
        v6 = (tGameObjectMain*)v32->o_Target;
        if (v32->o_Target)
        {
            if ((unsigned int)v6 <= 2)
                v6 = AI_MainActor[(uint32_t)v6 - 1];
            AI_EvalFunc_OBJGetPos_C(v6, &a3);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &a3);
        }
        AI_EvalFunc_OBJSetPos_C_CURRENT(&a3);
    }
    if (v32->f_XMEN_width == 0.0)
    {
        XMEN_OnOff(AI_gpst_CurrentGameObject, 0, 0);
    }
    else
    {
        v13 = TIM_gf_dt;
        v32->f_XMEN_width = v32->f_XMEN_width - 16.0f * TIM_gf_dt;
        if (v32->f_XMEN_width < 0.0)
            v2 = 0.0;
        else
            v2 = v32->f_XMEN_width;
        v32->f_XMEN_width = v2;
        XMEN_SetSize(AI_gpst_CurrentGameObject, 0, -1, v32->f_XMEN_width);
    }
    if (v32->i_Halo != -1)
    {
        AI_EvalFunc_GFXDel_C_CURRENT(v32->i_Halo);
        v32->i_Halo = -1;
    }
    if (v32->GFX_Halo != -1)
    {
        AI_EvalFunc_GFXDel_C_CURRENT(v32->GFX_Halo);
        v32->GFX_Halo = -1;
    }
    if (v32->GFX_Halo_2 != -1)
    {
        AI_EvalFunc_GFXDel_C_CURRENT(v32->GFX_Halo_2);
        v32->GFX_Halo_2 = -1;
    }
    if (v32->f_line_length == 0.0)
    {
        if (v32->GFX_Line != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v32->GFX_Line);
            v32->GFX_Line = -1;
        }
    }
    else
    {
        if (v32->GFX_Line != -1)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v22);
            GFXSetCurGlobal(v32->GFX_Line);
            AI_EvalFunc_GFXSetv_C_CURGLOB(1200, &v22);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v34);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v15);
            v5 = v32->f_line_length;
            v27.x = v15.x * v5;
            v27.y = v15.y * v5;
            v27.z = v15.z * v5;
            MATH_SubstractVec(&dest, &v34, &v27);
            AI_EvalFunc_GFXSetv_C_CURGLOB(1201, &dest);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v16);
            v23.x = -v16.x;
            v23.y = -v16.y;
            v23.z = -v16.z;
            AI_EvalFunc_GFXSetv_C_CURGLOB(1202, &v23);
        }
        v14 = TIM_gf_dt;
        v32->f_line_length = v32->f_line_length - v32->f_speed * TIM_gf_dt;
        if (v32->f_line_length < 0.0)
            v1 = 0.0;
        else
            v1 = v32->f_line_length;
        v32->f_line_length = v1;
    }
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
        v12 = 0.5;
    else
        v12 = 0.1f;
    if (AI_EvalFunc_TIMEElapsed_C(v32->f_time_impact, v12))
    {
        if (v32->GFX_Flash != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v32->GFX_Flash);
            v32->GFX_Flash = -1;
        }
    }
    else if (v32->GFX_Flash != -1)
    {
        v21 = TIM_gf_MainClock - TIM_gf_LockTime;
        v9 = (v21 - v32->f_time_impact) / v12;
        v4 = v28;
        if (v28)
        {
            if ((unsigned int)v28 <= 2)
                v4 = AI_MainActor[(uint32_t)v28 - 1];
            AI_EvalFunc_OBJGetPos_C(v4, &v10);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v10);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v20);
        MATH_SubstractVec(&a1, &v10, &v20);
        v25 = MATH_VecLen(&a1);
        GFXSetCurGlobal(v32->GFX_Flash);
        AI_EvalFunc_GFXSetf_C_CURGLOB(18013, v25);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v35);
        AI_EvalFunc_GFXSetv_C_CURGLOB(18200, &v35);
        if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
        {
            v19 = AI_EvalFunc_COLORBlend_C(0xFFC280BE, 0, v9);
            AI_EvalFunc_GFXSeti_C_CURRENT(v32->GFX_Flash, 18102, v19);
        }
        else
        {
            v18 = AI_EvalFunc_COLORBlend_C(0x80C280BE, 0, v9);
            AI_EvalFunc_GFXSeti_C_CURRENT(v32->GFX_Flash, 18102, v18);
        }
    }
    if (v32->GFX_Line == -1
        && v32->GFX_Flash == -1
        && (XMEN_OnOff(AI_gpst_CurrentGameObject, 0, 0), v31 = AI_EvalFunc_TIMEElapsed_C(v32->f_time_start, 1.0))
        && (v11 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject), v11 != 1000))
    {
        result = v11;
    }
    else
    {
        result = 1000;
    }
    return result;
}