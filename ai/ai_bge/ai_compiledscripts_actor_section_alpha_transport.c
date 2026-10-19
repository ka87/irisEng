#include "ai_compiledscripts_inc.h"

//Section_Alpha_Transport_SAT_exec_GFX_Fire
int Section_Alpha_Transport_SAT_exec_GFX_Fire_SFX_explosion_gao;

//Section_Alpha_Transport_SAT_exec_afterrec
int fct_Section_Alpha_Transport_SAT_exec_afterrec_Section_Alpha_Transport_SAT_exec_GFX_Glow;
int fct_Section_Alpha_Transport_SAT_exec_afterrec_Section_Alpha_Transport_SAT_exec_GFX_smoke;
int fct_Section_Alpha_Transport_SAT_exec_afterrec_Section_Alpha_Transport_SAT_exec_GFX_Fire;

//Section_Alpha_Transport_SAT_exec_GFX_smoke
int Section_Alpha_Transport_SAT_exec_GFX_smoke_SFX_explosion_gao;

//Section_Alpha_Transport_SAT_init
int fct_Section_Alpha_Transport_SAT_init_Section_Alpha_Transport_SAT_exec_afterrec;
int Section_Alpha_Transport_SAT_init_univVars;
int Section_Alpha_Transport_SAT_init_univVars_Flag_speciaux_sauve;

//Section_Alpha_Transport_SAT_exec_GFX_Glow
int Section_Alpha_Transport_SAT_exec_GFX_Glow_SFX_explosion_gao;

int Section_Alpha_Transport_SAT_exec_GFX_Fire()
{
    float v1; // [esp+0h] [ebp-168h]
    float v2; // [esp+0h] [ebp-168h]
    float v3; // [esp+0h] [ebp-168h]
    float v4; // [esp+0h] [ebp-168h]
    float v5; // [esp+4h] [ebp-164h]
    float v6; // [esp+8h] [ebp-160h]
    float v7; // [esp+Ch] [ebp-15Ch]
    float v8; // [esp+10h] [ebp-158h]
    float v9; // [esp+14h] [ebp-154h]
    float v10; // [esp+18h] [ebp-150h]
    float v11; // [esp+1Ch] [ebp-14Ch]
    float v12; // [esp+20h] [ebp-148h]
    tScriptStruct_PNJSectionAlphaTransport* v13; // [esp+24h] [ebp-144h]
    float v14; // [esp+28h] [ebp-140h]
    float v15; // [esp+38h] [ebp-130h]
    tGameObjectMain* v16; // [esp+68h] [ebp-100h]
    tGameObjectMain* v17; // [esp+70h] [ebp-F8h]
    Vec3 v18; // [esp+74h] [ebp-F4h]
    Vec3 v19; // [esp+8Ch] [ebp-DCh] BYREF
    Vec3 v20; // [esp+98h] [ebp-D0h] BYREF
    float v21; // [esp+A4h] [ebp-C4h]
    float v22; // [esp+A8h] [ebp-C0h]
    float v23; // [esp+ACh] [ebp-BCh]
    float v24; // [esp+B0h] [ebp-B8h]
    float v25; // [esp+B4h] [ebp-B4h]
    float v26; // [esp+B8h] [ebp-B0h]
    float v27; // [esp+BCh] [ebp-ACh]
    float v28; // [esp+C0h] [ebp-A8h]
    float v30; // [esp+C8h] [ebp-A0h]
    float v31; // [esp+CCh] [ebp-9Ch]
    float v32; // [esp+D0h] [ebp-98h]
    float v33; // [esp+D4h] [ebp-94h]
    float v34; // [esp+D8h] [ebp-90h]
    float v35; // [esp+DCh] [ebp-8Ch]
    float v36; // [esp+E0h] [ebp-88h]
    float v37; // [esp+E4h] [ebp-84h]
    float v38; // [esp+E8h] [ebp-80h]
    float v39; // [esp+ECh] [ebp-7Ch]
    float v40; // [esp+F0h] [ebp-78h]
    float v41; // [esp+F4h] [ebp-74h]
    float v42; // [esp+F8h] [ebp-70h]
    float v43; // [esp+FCh] [ebp-6Ch]
    float v44; // [esp+100h] [ebp-68h]
    float v45; // [esp+104h] [ebp-64h]
    Vec3 dest; // [esp+108h] [ebp-60h] BYREF
    Vec3 v47; // [esp+114h] [ebp-54h] BYREF
    float v48; // [esp+120h] [ebp-48h]
    Vec3 v49; // [esp+124h] [ebp-44h] BYREF
    Vec3 v50; // [esp+130h] [ebp-38h] BYREF
    float v51; // [esp+13Ch] [ebp-2Ch]
    tScriptStruct_PNJSectionAlphaTransport* v52; // [esp+144h] [ebp-24h]
    float v53; // [esp+148h] [ebp-20h]
    float v54; // [esp+14Ch] [ebp-1Ch]
    float v55; // [esp+150h] [ebp-18h]
    float v56; // [esp+154h] [ebp-14h]
    Vec3 a3; // [esp+158h] [ebp-10h] BYREF
    float v58; // [esp+164h] [ebp-4h]

    v20.x = 1.0;
    v20.y = 0.0;
    v20.z = 0.0;
    v49.x = 1.0;
    v49.y = 0.0;
    v49.z = 0.0;
    v50.x = 1.0;
    v50.y = 0.0;
    v50.z = 0.0;
    v32 = 1.0;
    v33 = 0.0;
    v34 = 0.0;
    v41 = 1.0;
    v42 = 0.0;
    v43 = 0.0;
    if (AI_gpst_CurrentInstance)
        v13 = (tScriptStruct_PNJSectionAlphaTransport*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v13 = 0;
    v52 = v13;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Section_Alpha_Transport_SAT_exec_GFX_Fire_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
    }
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(4))
    {
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 2);
        if (v52->GFX_Fire == -1)
        {
            v52->GFX_Fire = AI_EvalFunc_GFXAdd_C_CURRENT(13);
            GFXSetCurGlobal(v52->GFX_Fire);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 200);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13106, 0xA43B7400);
            v17 = (tGameObjectMain*)Section_Alpha_Transport_SAT_exec_GFX_Fire_SFX_explosion_gao;
            if (Section_Alpha_Transport_SAT_exec_GFX_Fire_SFX_explosion_gao)
            {
                if ((unsigned int)Section_Alpha_Transport_SAT_exec_GFX_Fire_SFX_explosion_gao <= 2)
                    v17 = AI_MainActor[Section_Alpha_Transport_SAT_exec_GFX_Fire_SFX_explosion_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v52->GFX_Fire, v17, -1);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v52->GFX_Fire, 0, -1);
            }
            GFXSetCurGlobal(v52->GFX_Fire);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13101, 8);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13110, 9);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13111, 10);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13112, 8);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13003, 0.15000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13004, 0.44999999f);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13103, 0xFF000000);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13104, 0x80302010);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13105, 0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13107, 0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13000, 0.2f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13001, 0.80000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13002, 0.000099999997f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.0049999999f);
            v47.x = 0.02f;
            v47.y = 0.02f;
            v47.z = 0.02f;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v47);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v52->GFX_Fire, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v52->GFX_Fire, 2, 1);
            AI_EvalFunc_GFXSetf_C_CURRENT(v52->GFX_Fire, 13012, 0.75);
        }
        v16 = (tGameObjectMain*)v52->o_fuselage;
        if (v16)
        {
            if ((unsigned int)v16 <= 2)
                v16 = AI_MainActor[(uint32_t)v16 - 1];
            AI_EvalFunc_OBJ_BankingGet_C(v16, &v19);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGet_C(0, &v19);
        }
        v35 = -v19.x * 20.0f;
        v36 = -v19.y * 20.0f;
        v37 = -v19.z * 20.0f;
        v41 = v35;
        v42 = v36;
        v43 = v37;
        v32 = v35;
        v33 = v36;
        v34 = v37;
        v53 = v52->f_zoom;
        v54 = v52->f_zoom;
        v55 = v52->f_zoom;
        v41 = v35 - v53;
        v42 = v36 - v54;
        v43 = v37 - v55;
        v22 = v52->f_zoom;
        v23 = v52->f_zoom;
        v24 = v52->f_zoom;
        v32 = v35 + v22;
        v33 = v36 + v23;
        v34 = v37 + v24;
        if (v41 > (double)v32)
            v12 = v32;
        else
            v12 = v41;
        v48 = v12;
        if (v42 > (double)v33)
            v11 = v33;
        else
            v11 = v42;
        v56 = v11;
        if (v43 > (double)v34)
            v10 = v34;
        else
            v10 = v43;
        v28 = v10;
        v38 = v48;
        v39 = v56;
        v40 = v10;
        v50.x = v48;
        v50.y = v56;
        v50.z = v10;
        if (v41 < (double)v32)
            v9 = v32;
        else
            v9 = v41;
        v21 = v9;
        if (v42 < (double)v33)
            v8 = v33;
        else
            v8 = v42;
        v58 = v8;
        if (v43 < (double)v34)
            v7 = v34;
        else
            v7 = v43;
        v45 = v7;
        v25 = v21;
        v26 = v58;
        v27 = v7;
        v20.x = v21;
        v20.y = v58;
        v20.z = v7;
        GFXSetCurGlobal(v52->GFX_Fire);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v50);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v20);
        v15 = v52->f_speed / 200.0f;
        if (v15 > 1.0)
            v6 = 1.0;
        else
            v6 = v52->f_speed / 200.0f;
        v30 = v6;
        v18.x = v6 * v52->v_speed.x;
        v18.y = v52->v_speed.y * v6;
        v18.z = v52->v_speed.z * v6;
        v49 = v18;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13204, &v49);
        v31 = TIM_gf_dt;
        v14 = v52->f_fire_coef + TIM_gf_dt;
        if (v14 > 1.0)
            v5 = 1.0;
        else
            v5 = v52->f_fire_coef + TIM_gf_dt;
        v51 = v5;
        v52->f_fire_coef = v5;
        v1 = v52->f_fire_coef * 5.0f * v52->f_zoom;
        AI_EvalFunc_GFXSetf_C_CURGLOB(13009, v1);
        v2 = v52->f_fire_coef * 30.0f * v52->f_zoom;
        AI_EvalFunc_GFXSetf_C_CURGLOB(13010, v2);
        v3 = v52->f_fire_coef * 0.5f * v52->f_zoom;
        AI_EvalFunc_GFXSetf_C_CURGLOB(13005, v3);
        v4 = v52->f_fire_coef * 1.25f * v52->f_zoom;
        AI_EvalFunc_GFXSetf_C_CURGLOB(13006, v4);
        v44 = TIM_gf_dt;
        a3.x = v49.x * TIM_gf_dt;
        a3.y = v49.y * TIM_gf_dt;
        a3.z = v49.z * TIM_gf_dt;
        MATH_SubstractVec(&dest, &v52->v_tuyere_pos, &a3);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13200, &dest);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13011, v52->f_ground_Z);
    }
    else if (v52->GFX_Fire != -1)
    {
        AI_EvalFunc_GFXSeti_C_CURRENT(v52->GFX_Fire, 13106, 5);
        v52->GFX_Fire = -1;
        v52->f_fire_coef = 0.0;
    }
    return 1000;
}

int Section_Alpha_Transport_SAT_exec_afterrec()
{
    int result; // eax
    tScriptStruct_PNJSectionAlphaTransport* v1; // [esp+0h] [ebp-18h]
    int v2; // [esp+4h] [ebp-14h]
    int v3; // [esp+Ch] [ebp-Ch]
    int v4; // [esp+10h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJSectionAlphaTransport*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Section_Alpha_Transport_SAT_exec_afterrec_Section_Alpha_Transport_SAT_exec_GFX_Glow = (int)AI_ul_RealResolveOneRefFunction(0x7200508Au);
        fct_Section_Alpha_Transport_SAT_exec_afterrec_Section_Alpha_Transport_SAT_exec_GFX_smoke = (int)AI_ul_RealResolveOneRefFunction(0x72005069u);
        fct_Section_Alpha_Transport_SAT_exec_afterrec_Section_Alpha_Transport_SAT_exec_GFX_Fire = (int)AI_ul_RealResolveOneRefFunction(0x7200506Fu);
    }
    v1->f_zoom = AI_EvalFunc_OBJ_ZoomGet_C(AI_gpst_CurrentGameObject);
    v4 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Section_Alpha_Transport_SAT_exec_afterrec_Section_Alpha_Transport_SAT_exec_GFX_Glow);
    if (v4 != 1000)
        return v4;
    v3 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Section_Alpha_Transport_SAT_exec_afterrec_Section_Alpha_Transport_SAT_exec_GFX_smoke);
    if (v3 != 1000)
        return v3;
    v2 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Section_Alpha_Transport_SAT_exec_afterrec_Section_Alpha_Transport_SAT_exec_GFX_Fire);
    if (v2 == 1000)
        result = 1000;
    else
        result = v2;
    return result;
}

int Section_Alpha_Transport_SAT_exec_GFX_smoke()
{
    float v1; // [esp+0h] [ebp-D4h]
    float v2; // [esp+0h] [ebp-D4h]
    float v3; // [esp+0h] [ebp-D4h]
    float v4; // [esp+0h] [ebp-D4h]
    float v5; // [esp+4h] [ebp-D0h]
    tScriptStruct_PNJSectionAlphaTransport* v6; // [esp+8h] [ebp-CCh]
    float v7; // [esp+Ch] [ebp-C8h]
    tGameObjectMain* v8; // [esp+18h] [ebp-BCh]
    tGameObjectMain* v9; // [esp+1Ch] [ebp-B8h]
    tGameObjectMain* v10; // [esp+20h] [ebp-B4h]
    tGameObjectMain* v11; // [esp+30h] [ebp-A4h]
    float v12; // [esp+34h] [ebp-A0h]
    float v13; // [esp+34h] [ebp-A0h]
    Vec3 v14; // [esp+50h] [ebp-84h] BYREF
    Vec3 v15; // [esp+5Ch] [ebp-78h] BYREF
    Vec3 a3; // [esp+68h] [ebp-6Ch] BYREF
    Vec3 v17; // [esp+74h] [ebp-60h] BYREF
    float v18; // [esp+80h] [ebp-54h]
    Vec3 v20; // [esp+88h] [ebp-4Ch] BYREF
    Vec3 v21; // [esp+98h] [ebp-3Ch] BYREF
    tGameObjectMain* v22; // [esp+A4h] [ebp-30h]
    Vec3 v23; // [esp+A8h] [ebp-2Ch] BYREF
    tScriptStruct_PNJSectionAlphaTransport* v24; // [esp+B4h] [ebp-20h]
    Vec3 v25; // [esp+B8h] [ebp-1Ch] BYREF
    Vec3 dest; // [esp+C8h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v6 = (tScriptStruct_PNJSectionAlphaTransport*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v6 = 0;
    v24 = v6;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Section_Alpha_Transport_SAT_exec_GFX_smoke_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
    }
    AI_EvalFunc_COLCrossableSet_C(AI_gpst_CurrentGameObject, 0, -1);
    v11 = (tGameObjectMain*)v24->o_fuselage;
    if (v11)
    {
        if ((unsigned int)v11 <= 2)
            v11 = AI_MainActor[(uint32_t)v11 - 1];
        AI_EvalFunc_OBJ_BankingGet_C(v11, &a3);
    }
    else
    {
        AI_EvalFunc_OBJ_BankingGet_C(0, &a3);
    }
    v23.x = -a3.x;
    v23.y = -a3.y;
    v23.z = -a3.z;
    v22 = AI_EvalFunc_COLRayObjectDist_C(
        AI_gpst_CurrentGameObject,
        &v24->v_tuyere_pos,
        &v23,
        200.0,
        0x4000,
        0,
        0,
        0x80000001);
    v10 = v22;
    if (v22)
    {
        if ((unsigned int)v22 <= 2)
            v10 = AI_MainActor[(uint32_t)v22 - 1];
        v9 = v10;
    }
    else
    {
        v9 = 0;
    }
    if (v9)
    {
        AI_EvalFunc_COLRayPosGet_C(AI_gpst_CurrentGameObject, &v24->v_ground_pos);
        v24->f_ground_Z = v24->v_ground_pos.z;
        if (v24->f_fire_coef == 1.0)
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(1, 0);
        else
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 1);
    }
    else
    {
        v24->v_ground_pos.x = 0.0;
        v24->v_ground_pos.y = 0.0;
        v24->v_ground_pos.z = -1.0e14f;
        v24->f_ground_Z = v24->v_ground_pos.z;
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 1);
    }
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(1))
    {
        if (v24->GFX_Smoke == -1)
        {
            v24->GFX_Smoke = AI_EvalFunc_GFXAdd_C_CURRENT(13);
            GFXSetCurGlobal(v24->GFX_Smoke);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 100);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13106, 10000000);
            v8 = (tGameObjectMain*)Section_Alpha_Transport_SAT_exec_GFX_smoke_SFX_explosion_gao;
            if (Section_Alpha_Transport_SAT_exec_GFX_smoke_SFX_explosion_gao)
            {
                if ((unsigned int)Section_Alpha_Transport_SAT_exec_GFX_smoke_SFX_explosion_gao <= 2)
                    v8 = AI_MainActor[Section_Alpha_Transport_SAT_exec_GFX_smoke_SFX_explosion_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v24->GFX_Smoke, v8, -1);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v24->GFX_Smoke, 0, -1);
            }
            GFXSetCurGlobal(v24->GFX_Smoke);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13101, 8);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13110, 9);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13111, 10);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13112, 8);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13000, 0.2f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13001, 0.40000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13002, 0.0099999998f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13003, 0.15000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13004, 0.75);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13007, 5.0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13103, 0xFF000000);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13104, 0x203241);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13105, 0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13107, 0);
            v15.x = -1.0;
            v15.y = -1.0;
            v15.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v15);
            v21.x = 1.0;
            v21.y = 1.0;
            v21.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v21);
            v25.x = 0.050000001f;
            v25.y = 0.050000001f;
            v25.z = 0.050000001f;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v25);
            v17.x = 1.0;
            v17.y = 0.0;
            v17.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13204, &v17);
            v20.x = 0.0;
            v20.y = 0.0;
            v20.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13205, &v20);
            v14.x = 0.0;
            v14.y = 0.0;
            v14.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13206, &v14);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v24->GFX_Smoke, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v24->GFX_Smoke, 2, 1);
            AI_EvalFunc_GFXSetf_C_CURRENT(v24->GFX_Smoke, 13012, 0.75);
        }
        MATH_SubstractVec(&dest, &v24->v_ground_pos, &v24->v_tuyere_pos);
        v12 = MATH_VecLen(&dest);
        v7 = v12 / 200.0f;
        if (v7 > 1.0)
            v5 = 1.0;
        else
            v5 = v12 / 200.0f;
        v18 = v5;
        v13 = 1.0f - v5;
        GFXSetCurGlobal(v24->GFX_Smoke);
        v1 = v13 * 8.0f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(13005, v1);
        v2 = v13 * 16.0f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(13006, v2);
        AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.0099999998f);
        v3 = v13 * 40.0f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(13009, v3);
        v4 = v13 * 100.0f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(13010, v4);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13200, &v24->v_ground_pos);
    }
    else if (v24->GFX_Smoke != -1)
    {
        AI_EvalFunc_GFXSeti_C_CURRENT(v24->GFX_Smoke, 13106, 10);
        v24->GFX_Smoke = -1;
    }
    return 1000;
}

int Section_Alpha_Transport_SAT_init()
{
    tScriptStruct_PNJSectionAlphaTransport* v1; // [esp+0h] [ebp-44h]
    tGameObjectMain* v2; // [esp+8h] [ebp-3Ch]
    tGameObjectMain* v3; // [esp+10h] [ebp-34h]
    int v4; // [esp+20h] [ebp-24h]
    int v5; // [esp+34h] [ebp-10h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJSectionAlphaTransport*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Section_Alpha_Transport_SAT_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Section_Alpha_Transport_SAT_init_univVars_Flag_speciaux_sauve,
            (void*)Section_Alpha_Transport_SAT_init_univVars,
            4488);
        fct_Section_Alpha_Transport_SAT_init_Section_Alpha_Transport_SAT_exec_afterrec = (int)AI_ul_RealResolveOneRefFunction(0xBF003B13);
    }
    if (v1->des_i_sf_jesuisla)
    {
        if (v1->des_i_sf_jesuisla <= 0
            || !AI_EvalFunc_ARRBitTest_C(
                (int*)(Section_Alpha_Transport_SAT_init_univVars_Flag_speciaux_sauve + 4),
                v1->des_i_sf_jesuisla))
        {
            v5 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v5 != 1000)
                return v5;
        }
    }
    if (v1->des_i_sf_jesuisplusla)
    {
        if (v1->des_i_sf_jesuisplusla > 0
            && AI_EvalFunc_ARRBitTest_C(
                (int*)(Section_Alpha_Transport_SAT_init_univVars_Flag_speciaux_sauve + 4),
                v1->des_i_sf_jesuisplusla))
        {
            v4 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
            if (v4 != 1000)
                return v4;
        }
    }
    v1->o_fuselage = (int)AI_EvalFunc_ANICanalGaoGet_C_CURRENT(0);
    if (AI_EvalFunc_OBJ_FlagsIdentityTest_C(AI_gpst_CurrentGameObject, 2))
    {
        v3 = AI_gpst_CurrentGameObject;
        if (AI_gpst_CurrentGameObject)
        {
            if ((unsigned int)AI_gpst_CurrentGameObject <= 2)
                v3 = AI_MainActor[(uint32_t)AI_gpst_CurrentGameObject - 1];
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                v3,
                3,
                (tAIFunction*)fct_Section_Alpha_Transport_SAT_init_Section_Alpha_Transport_SAT_exec_afterrec);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                3,
                (tAIFunction*)fct_Section_Alpha_Transport_SAT_init_Section_Alpha_Transport_SAT_exec_afterrec);
        }
    }
    else
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
                (tAIFunction*)fct_Section_Alpha_Transport_SAT_init_Section_Alpha_Transport_SAT_exec_afterrec);
        }
        else
        {
            AI_EvalFunc_AICBAdd_C(
                AI_gpst_CurrentGameObject,
                0,
                2,
                (tAIFunction*)fct_Section_Alpha_Transport_SAT_init_Section_Alpha_Transport_SAT_exec_afterrec);
        }
    }
    AI_EvalFunc_AITrackCurStop_C();
    return 1000;
}

int Section_Alpha_Transport_SAT_exec_GFX_Glow()
{
    double v0; // st7
    float v2; // [esp+0h] [ebp-274h]
    float v3; // [esp+0h] [ebp-274h]
    float v4; // [esp+4h] [ebp-270h]
    float v5; // [esp+4h] [ebp-270h]
    float v6; // [esp+4h] [ebp-270h]
    float v7; // [esp+4h] [ebp-270h]
    float v8; // [esp+4h] [ebp-270h]
    float v9; // [esp+4h] [ebp-270h]
    float v10; // [esp+8h] [ebp-26Ch]
    float v11; // [esp+Ch] [ebp-268h]
    float v12; // [esp+10h] [ebp-264h]
    float v13; // [esp+14h] [ebp-260h]
    float v14; // [esp+18h] [ebp-25Ch]
    float v15; // [esp+1Ch] [ebp-258h]
    float v16; // [esp+20h] [ebp-254h]
    tScriptStruct_PNJSectionAlphaTransport* v17; // [esp+24h] [ebp-250h]
    float v18; // [esp+3Ch] [ebp-238h]
    tGameObjectMain* v19; // [esp+6Ch] [ebp-208h]
    tGameObjectMain* v20; // [esp+74h] [ebp-200h]
    Vec3* v21; // [esp+78h] [ebp-1FCh]
    Vec3* v22; // [esp+7Ch] [ebp-1F8h]
    tGameObjectMain* v23; // [esp+9Ch] [ebp-1D8h]
    Vec3* v24; // [esp+A0h] [ebp-1D4h]
    float v25; // [esp+A4h] [ebp-1D0h]
    tGameObjectMain* v26; // [esp+ACh] [ebp-1C8h]
    Vec3* v27; // [esp+B0h] [ebp-1C4h]
    float v28; // [esp+B4h] [ebp-1C0h]
    tGameObjectMain* v29; // [esp+BCh] [ebp-1B8h]
    tGameObjectMain* v30; // [esp+CCh] [ebp-1A8h]
    Vec3 v31; // [esp+D0h] [ebp-1A4h]
    float v32; // [esp+DCh] [ebp-198h]
    float v33; // [esp+E0h] [ebp-194h]
    float v34; // [esp+E4h] [ebp-190h]
    Vec3 v35; // [esp+ECh] [ebp-188h] BYREF
    float v36; // [esp+F8h] [ebp-17Ch]
    Vec3 v37; // [esp+FCh] [ebp-178h] BYREF
    Vec3 v39; // [esp+10Ch] [ebp-168h] BYREF
    Vec3 v40; // [esp+118h] [ebp-15Ch]
    float v41; // [esp+124h] [ebp-150h]
    Vec3 a3; // [esp+128h] [ebp-14Ch] BYREF
    Vec3 v43; // [esp+134h] [ebp-140h] BYREF
    Vec3 a1; // [esp+140h] [ebp-134h] BYREF
    float v45; // [esp+14Ch] [ebp-128h]
    float v46; // [esp+150h] [ebp-124h]
    float v47; // [esp+154h] [ebp-120h]
    float v48; // [esp+158h] [ebp-11Ch]
    Vec3 v49; // [esp+15Ch] [ebp-118h]
    float v50; // [esp+168h] [ebp-10Ch]
    float v51; // [esp+16Ch] [ebp-108h]
    float v52; // [esp+170h] [ebp-104h]
    Vec3 v53; // [esp+174h] [ebp-100h] BYREF
    Vec3 v54; // [esp+180h] [ebp-F4h]
    float v55; // [esp+18Ch] [ebp-E8h]
    float v56; // [esp+190h] [ebp-E4h]
    Vec3 v57; // [esp+194h] [ebp-E0h] BYREF
    Vec3 v58; // [esp+1A0h] [ebp-D4h] BYREF
    Vec3 v59; // [esp+1ACh] [ebp-C8h] BYREF
    float v60; // [esp+1B8h] [ebp-BCh]
    Vec3 dest; // [esp+1C0h] [ebp-B4h] BYREF
    float v62; // [esp+1CCh] [ebp-A8h]
    float v63; // [esp+1D0h] [ebp-A4h]
    float v64; // [esp+1D4h] [ebp-A0h]
    float v65; // [esp+1D8h] [ebp-9Ch]
    float v66; // [esp+1DCh] [ebp-98h]
    float v67; // [esp+1E0h] [ebp-94h]
    float v68; // [esp+1E4h] [ebp-90h]
    Vec3 v69; // [esp+1E8h] [ebp-8Ch] BYREF
    Vec3 v70; // [esp+1F4h] [ebp-80h] BYREF
    float v71; // [esp+200h] [ebp-74h]
    float v72; // [esp+204h] [ebp-70h]
    float v73; // [esp+208h] [ebp-6Ch]
    float v74; // [esp+20Ch] [ebp-68h]
    Vec3 v75; // [esp+210h] [ebp-64h] BYREF
    Vec3 v76; // [esp+21Ch] [ebp-58h] BYREF
    Vec3 v77; // [esp+228h] [ebp-4Ch]
    tScriptStruct_PNJSectionAlphaTransport* v78; // [esp+234h] [ebp-40h]
    float v79; // [esp+238h] [ebp-3Ch]
    float v80; // [esp+23Ch] [ebp-38h]
    float v81; // [esp+240h] [ebp-34h]
    Vec3 v82; // [esp+244h] [ebp-30h]
    float v83; // [esp+250h] [ebp-24h]
    float v84; // [esp+254h] [ebp-20h]
    float v85; // [esp+258h] [ebp-1Ch]
    float v86; // [esp+25Ch] [ebp-18h]
    Vec3 v87; // [esp+260h] [ebp-14h] BYREF
    float v88; // [esp+270h] [ebp-4h]

    v37.x = 1.0;
    v37.y = 0.0;
    v37.z = 0.0;
    v69.x = 1.0;
    v69.y = 0.0;
    v69.z = 0.0;
    v70.x = 1.0;
    v70.y = 0.0;
    v70.z = 0.0;
    v54.x = 1.0;
    v54.y = 0.0;
    v54.z = 0.0;
    v59.x = 1.0;
    v59.y = 0.0;
    v59.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v17 = (tScriptStruct_PNJSectionAlphaTransport*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v17 = 0;
    v78 = v17;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Section_Alpha_Transport_SAT_exec_GFX_Glow_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
    }
    v30 = (tGameObjectMain*)v78->o_fuselage;
    if (v30)
    {
        if ((unsigned int)v30 <= 2)
            v30 = AI_MainActor[(uint32_t)v30 - 1];
        AI_EvalFunc_OBJGetPos_C(v30, &a3);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &a3);
    }
    v78->v_tuyere_pos = a3;
    v29 = (tGameObjectMain*)v78->o_fuselage;
    if (v29)
    {
        if ((unsigned int)v29 <= 2)
            v29 = AI_MainActor[(uint32_t)v29 - 1];
        AI_EvalFunc_OBJ_SightGet_C(v29, &v75);
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C(0, &v75);
    }
    v28 = v78->f_zoom;
    v62 = v75.x * v28;
    v63 = v75.y * v28;
    v64 = v75.z * v28;
    v27 = &v78->v_tuyere_pos;
    v78->v_tuyere_pos.x = v78->v_tuyere_pos.x - v62;
    v27->y = v27->y - v63;
    v27->z = v27->z - v64;
    v26 = (tGameObjectMain*)v78->o_fuselage;
    if (v26)
    {
        if ((unsigned int)v26 <= 2)
            v26 = AI_MainActor[(uint32_t)v26 - 1];
        AI_EvalFunc_OBJ_BankingGet_C(v26, &v35);
    }
    else
    {
        AI_EvalFunc_OBJ_BankingGet_C(0, &v35);
    }
    v25 = v78->f_zoom * 2.5f;
    v32 = v35.x * v25;
    v33 = v35.y * v25;
    v34 = v35.z * v25;
    v24 = &v78->v_tuyere_pos;
    v78->v_tuyere_pos.x = v78->v_tuyere_pos.x - v32;
    v24->y = v24->y - v33;
    v24->z = v24->z - v34;
    v23 = (tGameObjectMain*)v78->o_fuselage;
    if (v23)
    {
        if ((unsigned int)v23 <= 2)
            v23 = AI_MainActor[(uint32_t)v23 - 1];
        AI_EvalFunc_OBJ_BankingGet_C(v23, &v53);
    }
    else
    {
        AI_EvalFunc_OBJ_BankingGet_C(0, &v53);
    }
    v82.x = -v53.x;
    v82.y = -v53.y;
    v82.z = -v53.z;
    v40.x = -v53.x * 1000.0f;
    v40.y = -v53.y * 1000.0f;
    v40.z = -v53.z * 1000.0f;
    MATH_SubstractVec(&dest, &v78->v_tuyere_pos, &v78->v_last_pos);
    v86 = TIM_gf_dt;
    MATH_VecMultNormScalar(&a1, &dest, TIM_gf_dt);
    v78->v_speed = a1;
    v0 = MATH_VecLen(&v78->v_speed);
    v78->f_speed = (float)v0;
    v21 = &v78->v_last_pos;
    v22 = &v78->v_tuyere_pos;
    v78->v_last_pos.x = v78->v_tuyere_pos.x;
    v21->y = v22->y;
    v21->z = v22->z;
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(2))
    {
        if (v78->GFX_Glow == -1)
        {
            v78->GFX_Glow = AI_EvalFunc_GFXAdd_C_CURRENT(13);
            GFXSetCurGlobal(v78->GFX_Glow);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 50);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13106, 0xA43B7400);
            v20 = (tGameObjectMain*)Section_Alpha_Transport_SAT_exec_GFX_Glow_SFX_explosion_gao;
            if (Section_Alpha_Transport_SAT_exec_GFX_Glow_SFX_explosion_gao)
            {
                if ((unsigned int)Section_Alpha_Transport_SAT_exec_GFX_Glow_SFX_explosion_gao <= 2)
                    v20 = AI_MainActor[Section_Alpha_Transport_SAT_exec_GFX_Glow_SFX_explosion_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v78->GFX_Glow, v20, -1);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v78->GFX_Glow, 0, -1);
            }
            GFXSetCurGlobal(v78->GFX_Glow);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13101, 8);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13110, 9);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13111, 10);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13112, 8);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13003, 0.15000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13004, 0.44999999f);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13103, 0xFF000000);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13104, 0x80302010);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13105, 0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13107, 0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13000, -0.2f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13001, -0.80000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13002, 0.0099999998f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.02f);
            v57.x = 0.02f;
            v57.y = 0.02f;
            v57.z = 0.02f;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v57);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v78->GFX_Glow, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v78->GFX_Glow, 2, 1);
            AI_EvalFunc_GFXSetf_C_CURRENT(v78->GFX_Glow, 13012, 0.75);
        }
        v19 = (tGameObjectMain*)v78->o_fuselage;
        if (v19)
        {
            if ((unsigned int)v19 <= 2)
                v19 = AI_MainActor[(uint32_t)v19 - 1];
            AI_EvalFunc_OBJ_BankingGet_C(v19, &v43);
        }
        else
        {
            AI_EvalFunc_OBJ_BankingGet_C(0, &v43);
        }
        v49.x = -v43.x;
        v49.y = -v43.y;
        v49.z = -v43.z;
        v31.x = -v43.x * 20.0f;
        v31.y = -v43.y * 20.0f;
        v31.z = -v43.z * 20.0f;
        v59 = v31;
        v54 = v31;
        v50 = v78->f_zoom;
        v51 = v78->f_zoom;
        v52 = v78->f_zoom;
        v59.x = v31.x - v50;
        v59.y = v31.y - v51;
        v59.z = v31.z - v52;
        v66 = v78->f_zoom;
        v67 = v78->f_zoom;
        v68 = v78->f_zoom;
        v54.x = v31.x + v66;
        v54.y = v31.y + v67;
        v54.z = v31.z + v68;
        if (v59.x > (double)v54.x)
            v16 = v54.x;
        else
            v16 = v59.x;
        v60 = v16;
        if (v59.y > (double)v54.y)
            v15 = v54.y;
        else
            v15 = v59.y;
        v88 = v15;
        if (v59.z > (double)v54.z)
            v14 = v54.z;
        else
            v14 = v59.z;
        v65 = v14;
        v45 = v60;
        v46 = v88;
        v47 = v14;
        v70.x = v60;
        v70.y = v88;
        v70.z = v14;
        if (v59.x < (double)v54.x)
            v13 = v54.x;
        else
            v13 = v59.x;
        v48 = v13;
        if (v59.y < (double)v54.y)
            v12 = v54.y;
        else
            v12 = v59.y;
        v74 = v12;
        if (v59.z < (double)v54.z)
            v11 = v54.z;
        else
            v11 = v59.z;
        v36 = v11;
        v83 = v48;
        v84 = v74;
        v85 = v11;
        v37.x = v48;
        v37.y = v74;
        v37.z = v11;
        GFXSetCurGlobal(v78->GFX_Glow);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v70);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v37);
        v18 = v78->f_speed / 100.0f;
        if (v18 > 1.0)
            v10 = 1.0;
        else
            v10 = v78->f_speed / 100.0f;
        v56 = v10;
        v77.x = v10 * v78->v_speed.x;
        v77.y = v78->v_speed.y * v10;
        v77.z = v78->v_speed.z * v10;
        v69 = v77;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13204, &v69);
        v4 = v78->f_zoom * 0.5f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(13009, v4);
        v5 = v78->f_zoom + v78->f_zoom;
        AI_EvalFunc_GFXSetf_C_CURGLOB(13010, v5);
        v6 = v78->f_zoom * 0.25f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(13005, v6);
        v7 = v78->f_zoom * 0.75f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(13006, v7);
        v59 = v78->v_tuyere_pos;
        v8 = v78->f_zoom * 0.25f;
        v2 = v78->f_zoom * -0.25f;
        v55 = AI_EvalFunc_MATHRandFloat_C(v2, v8);
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v58);
        v79 = v58.x * v55;
        v80 = v58.y * v55;
        v81 = v58.z * v55;
        v59.x = v59.x + v79;
        v59.y = v59.y + v80;
        v59.z = v59.z + v81;
        v9 = v78->f_zoom * 0.25f;
        v3 = v78->f_zoom * -0.25f;
        v41 = AI_EvalFunc_MATHRandFloat_C(v3, v9);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v87);
        v71 = v87.x * v41;
        v72 = v87.y * v41;
        v73 = v87.z * v41;
        v59.x = v59.x + v71;
        v59.y = v59.y + v72;
        v59.z = v59.z + v73;
        v39.x = v69.x * TIM_gf_dt;
        v39.y = v69.y * TIM_gf_dt;
        v39.z = v69.z * TIM_gf_dt;
        MATH_SubstractVec(&v76, &v59, &v39);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13200, &v76);
    }
    else if (v78->GFX_Glow != -1)
    {
        AI_EvalFunc_GFXSeti_C_CURRENT(v78->GFX_Glow, 13106, 5);
        v78->GFX_Glow = -1;
    }
    return 1000;
}