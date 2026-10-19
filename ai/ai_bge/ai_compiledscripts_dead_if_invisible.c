#include "ai_compiledscripts_inc.h"

int dead_if_invisible_dii_go()
{
    void* v1; // [esp+0h] [ebp-50h]
    Vec3 a2; // [esp+4h] [ebp-4Ch] BYREF
    Vec3 v3; // [esp+10h] [ebp-40h] BYREF
    Vec3 a1; // [esp+1Ch] [ebp-34h] BYREF
    int v5; // [esp+28h] [ebp-28h]
    Vec3 v6; // [esp+2Ch] [ebp-24h] BYREF
    float v7; // [esp+38h] [ebp-18h]
    int v8; // [esp+3Ch] [ebp-14h]
    void* v9; // [esp+40h] [ebp-10h]
    Vec3 a3; // [esp+44h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v1 = AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v9 = v1;
    v5 = (unsigned __int16)AI_gpst_CurrentGameObject->flags2;
    if ((v5 & 2) == 0)
    {
        v8 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v8 != 1000)
            return v8;
    }
    v7 = AI_EvalFunc_MATHRandFloat_C(0.050000001f, 0.2f);
    v3.x = 0.1f;
    v3.y = v7;
    v3.z = 0.2f;
    AI_EvalFunc_OBJRotateLocal_C(AI_gpst_CurrentGameObject, &v3);
    a2.x = 2.0;
    a2.y = 2.0;
    a2.z = 2.0;
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v6);
    a3.x = v6.x * 1.5f;
    a3.y = v6.y * 1.5f;
    a3.z = v6.z * 1.5f;
    MATH_VecAddVec(&a1, &a2, &a3);
    OBJ_ScaleSet(AI_gpst_CurrentGameObject, &a1);
    return 1000;
}