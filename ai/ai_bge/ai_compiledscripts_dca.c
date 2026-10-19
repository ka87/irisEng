#include "ai_compiledscripts_inc.h"

//dca_dca_loop
int dca_dca_loop_SFX_light_smoke_gao;

int dca_dca_loop()
{
    float X; // [esp+0h] [ebp-6Ch]
    float Xa; // [esp+0h] [ebp-6Ch]
    tScriptStruct_DCA* v3; // [esp+4h] [ebp-68h]
    tGameObjectMain* v4; // [esp+20h] [ebp-4Ch]
    Vec3 a1; // [esp+28h] [ebp-44h] BYREF
    float v6; // [esp+38h] [ebp-34h]
    Vec3 v7; // [esp+40h] [ebp-2Ch]
    Vec3 v8; // [esp+4Ch] [ebp-20h] BYREF
    float v9; // [esp+58h] [ebp-14h]
    unsigned int v10; // [esp+5Ch] [ebp-10h]
    tScriptStruct_DCA* v11; // [esp+60h] [ebp-Ch]
    float v12; // [esp+64h] [ebp-8h]
    float v13; // [esp+68h] [ebp-4h]

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_DCA*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v11 = v3;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        dca_dca_loop_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    if (v11->i_idhalo == -1)
    {
        v11->i_idhalo = AI_EvalFunc_GFXAdd_C_CURRENT(21);
        AI_EvalFunc_GFXFlagSet_C_CURRENT(v11->i_idhalo, 4, 1);
        v4 = (tGameObjectMain*)dca_dca_loop_SFX_light_smoke_gao;
        if (dca_dca_loop_SFX_light_smoke_gao)
        {
            if ((unsigned int)dca_dca_loop_SFX_light_smoke_gao <= 2)
                v4 = AI_MainActor[dca_dca_loop_SFX_light_smoke_gao - 1];
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v11->i_idhalo, v4, 0);
        }
        else
        {
            AI_EvalFunc_GFXMaterialSet_C_CURRENT(v11->i_idhalo, 0, 0);
        }
        GFXSetCurGlobal(v11->i_idhalo);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21000, v11->des_f_Radius);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21002, v11->des_f_Offset);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21003, 5.0);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21004, v11->des_f_Extrude);
        AI_EvalFunc_GFXSetf_C_CURGLOB(21006, v11->des_f_Angle);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v11->des_c_color);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21104, (unsigned int)v11->des_i_Number);
        v10 = AI_EvalFunc_OBJ_FlagsControlSet_C(AI_gpst_CurrentGameObject, 0x10000000, 0);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v8);
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v8);
        v11->f_rand1 = AI_EvalFunc_MATHRandFloat_C(0.40000001f, 0.80000001f);
        v11->f_rand2 = AI_EvalFunc_MATHRandFloat_C(0.40000001f, 0.80000001f);
    }
    v7.x = 0.0;
    v7.y = 0.0;
    v7.z = 1.0;
    a1.x = 0.0;
    a1.y = 0.0;
    a1.z = 1.0;
    v12 = TIM_gf_MainClock - TIM_gf_LockTime;
    X = v11->f_rand1 * v12;
    v6 = sinf(X);
    a1.x = v6;
    v9 = TIM_gf_MainClock - TIM_gf_LockTime;
    Xa = v11->f_rand2 * v9;
    v13 = sinf(Xa);
    a1.x = a1.x * 0.25f;
    a1.y = v13 * 0.25f;
    MATH_NormalizeEqualVector(&a1);
    AI_EvalFunc_GFXSetv_C_CURRENT(v11->i_idhalo, 21201, &a1);
    return 1000;
}