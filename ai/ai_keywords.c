#include "../time.h"
#include "../ai.h"
#include "../dbg.h"

tAINode* AI_EvalKeyword_Ultra(tAINode* aiNode)
{
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_gpst_CurrentUltra = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];

    if (AI_gpst_CurrentUltra == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
    {
        AI_gpst_CurrentInstanceUltra = WOR_gst_Universe.univAIInstance;
        AI_gpst_CurrentUltra = NULL;
    }
    else if (AI_gpst_CurrentUltra->extended)
    {
        AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentUltra->extended->AIInstance;
    }
    else
    {
        AI_gpst_CurrentInstanceUltra = NULL;
    }
    
    return aiNode + 1;
}

tAINode* AI_EvalKeyword_ObjEqual(tAINode* aiNode)
{
    tGameObjectMain* gao1, *gao2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    gao2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    gao1 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    
    AI_EvalType_GetGAO(&gao2, &gao2);
    AI_EvalType_GetGAO(&gao1, &gao1);
    
    if (gao2 == gao1)
        AI_PutIntToStack(1);
    else
        AI_PutIntToStack(0);
    
    return aiNode + 1;
}

tAINode* AI_EvalKeyword_ObjDifferent(tAINode* aiNode)
{
    tGameObjectMain* gao1, *gao2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    gao2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    gao1 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];

    AI_EvalType_GetGAO(&gao2, &gao2);
    AI_EvalType_GetGAO(&gao1, &gao1);
    
    if (gao2 == gao1)
        AI_PutIntToStack(0);
    else
        AI_PutIntToStack(1);
    
    return aiNode + 1;
}

tAINode* AI_EvalKeyword_ObjNot(tAINode* aiNode)
{
    tGameObjectMain* gao;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    gao = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];

    AI_EvalType_GetGAO(&gao, &gao);
    
    if (gao)
        AI_PutIntToStack(0);
    else
        AI_PutIntToStack(1);
    
    return aiNode + 1;
}

tAINode* AI_EvalKeyword_Return(tAINode* aiNode)
{
    *AI_gppst_CurrentJumpNode = NULL;
    return (tAINode*)2;
}

tAINode* AI_EvalKeyword_ReturnTrack(tAINode* aiNode)
{
    *AI_gppst_CurrentJumpNode = NULL;
    return (tAINode*)4;
}

tAINode* AI_EvalKeyword_Stop(tAINode* aiNode)
{
    *AI_gppst_CurrentJumpNode = NULL;
    return (tAINode*)1;
}

tAINode* AI_EvalKeyword_Push(tAINode* aiNode)
{
    int dest[13];
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    AI_PushUserStack(dest, a2.type);
    return aiNode + 1;
}

tAINode* AI_EvalKeyword_Pop(tAINode* a1)
{
    int a1a[13];
    tVarStackElem a2;

    AI_PopUserStack(a1a, &a2);
    AI_PutObjectToStack(a1a, a2.type);
    return a1 + 1;
}

tAINode* AI_EvalKeyword_PopSP(tAINode* a1)
{
    tVarStackElem* v2;
    int dest[13];
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    v2 = &AI_gast_UserVarStack[AI_gul_SizeUserVarStack - dest[0] - 1];
    a2.type = v2->type;
    a2.ptr = v2->ptr;
    AI_gast_Types[a2.type].funcptr(a2.ptr, dest);
    AI_PutObjectToStack(dest, a2.type);
    return a1 + 1;
}

tAINode* AI_EvalKeyword_PopNow(tAINode* a1)
{
    int dest[13];
    int i;
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    for (i = dest[0]; i > 0; --i)
        AI_PopUserStack(dest, &a2);
    return a1 + 1;
}

tAINode* AI_EvalKeyword_Meta(tAINode* a1)
{
    *AI_gppst_CurrentJumpNode = &a1[a1->val];
    return (tAINode*)0;
}

int AI_EvalKeyword_During_C()
{
    tAI* v1;

    v1 = AI_gpst_CurrentInstanceUltra;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    v1->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = TIM_gf_dt + v1->instanceCallBackTimeRemaining[AI_gul_CurrentTrack];
    if (v1->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] < 0.0)
        return 1;
    v1->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = 0.0;
    return 0;
}

tAINode* AI_EvalKeyword_During(tAINode* a1)
{
    int v1;

    v1 = AI_EvalKeyword_During_C();
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalKeyword_DuringInit(tAINode* a1)
{
    tAI* v2;
    float v3;

    v2 = AI_gpst_CurrentInstanceUltra;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    v3 = AI_GetFloatFromStack();
    v2->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -v3;
    return a1 + 1;
}

tAINode* AI_EvalKeyword_JumpFalse(tAINode* a1)
{
    tAINode* result;
    int v2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v2 = *(int32_t*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_PutIntToStack(v2);
    if (v2)
        result = a1 + 1;
    else
        result = &a1[a1->val];
    return result;
}

tAINode* AI_EvalKeyword_JumpFF(tAINode* a1)
{
    int v2[3];

    v2[0] = 0;
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[32]);
    return &a1[a1->val];
}

tAINode* AI_EvalKeyword_JumpTrue(tAINode* a1)
{
    tAINode* result;
    int v2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v2 = *(int32_t*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_PutIntToStack(v2);
    if (v2)
        result = &a1[a1->val];
    else
        result = a1 + 1;
    return result;
}

tAINode* AI_EvalKeyword_JumpFalseSP(tAINode* a1)
{
    tAINode* result;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    if (*(int32_t*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack])
        result = a1 + 1;
    else
        result = &a1[a1->val];
    return result;
}

tAINode* AI_EvalKeyword_JumpTrueSP(tAINode* a1)
{
    tAINode* result;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    if (*(int32_t*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack])
        result = &a1[a1->val];
    else
        result = a1 + 1;
    return result;
}

tAINode* AI_EvalKeyword_Jump(tAINode* a1)
{
    return &a1[a1->val];
}

tAINode* AI_EvalKeyword_Different(tAINode* arg0)
{
    float v2;
    int v3[3];
    int a1[3];
    BOOL isFloat;
    double v6;
    int lvalData[13];
    tVarStackElem rval;
    int rvalData[13];
    tVarStackElem lval;

    AI_GetVarFromStack(lvalData, &lval);
    AI_GetVarFromStack(rvalData, &rval);

    if (rval.type == lval.type) {
        isFloat = rval.type == 2;
    }
    else {
        if (rval.type == 2)
        {
            v2 = (float)lvalData[0];
            *(float*)lvalData = v2;
            isFloat = 1;
        } 
        else {
            if (rval.type != 1)
            {
                isFloat = rval.type == 2;
            }
            else
            {
                v6 = *(float*)lvalData + MATH_gd_magicDoubleForRound;
                lvalData[0] = *((uint32_t *)&v6);
                isFloat = 0;
            }
        }
    }
    if (isFloat)
    {
        a1[0] = *(float*)rvalData != *(float*)lvalData;
        AI_PutObjectToStack(a1, AI_gaw_EnumLink[32]);
    }
    else
    {
        v3[0] = rvalData[0] != lvalData[0];
        AI_PutObjectToStack(v3, AI_gaw_EnumLink[32]);
    }
    return arg0 + 1;
}

tAINode* AI_EvalKeyword_Equal(tAINode* arg0)
{
    float v2;
    int v3[3];
    int a1[3];
    BOOL isFloat;
    double v6;
    int lvalData[13];
    tVarStackElem rval;
    int rvalData[13];
    tVarStackElem lval;

    AI_GetVarFromStack(lvalData, &lval);
    AI_GetVarFromStack(rvalData, &rval);

    if (rval.type == lval.type) {
        isFloat = rval.type == 2;
    }
    else {
        if (rval.type == 2)
        {
            v2 = (float)lvalData[0];
            *(float*)lvalData = v2;
            isFloat = 1;
        }
        else {
            if (rval.type != 1)
            {
                isFloat = rval.type == 2;
            }
            else
            {
                v6 = *(float*)lvalData + MATH_gd_magicDoubleForRound;
                lvalData[0] = *((uint32_t*)&v6);
                isFloat = 0;
            }
        }
    }
    if (isFloat)
    {
        a1[0] = *(float*)rvalData == *(float*)lvalData;
        AI_PutObjectToStack(a1, AI_gaw_EnumLink[32]);
    }
    else
    {
        v3[0] = rvalData[0] == lvalData[0];
        AI_PutObjectToStack(v3, AI_gaw_EnumLink[32]);
    }
    return arg0 + 1;
}

tAINode* AI_EvalKeyword_GreatEq(tAINode* arg0)
{
    float v2;
    int v3[3];
    int a1[3];
    BOOL isFloat;
    double v6;
    int lvalData[13];
    tVarStackElem rval;
    int rvalData[13];
    tVarStackElem lval;

    AI_GetVarFromStack(lvalData, &lval);
    AI_GetVarFromStack(rvalData, &rval);

    if (rval.type == lval.type) {
        isFloat = rval.type == 2;
    }
    else {
        if (rval.type == 2)
        {
            v2 = (float)lvalData[0];
            *(float*)lvalData = v2;
            isFloat = 1;
        }
        else {
            if (rval.type != 1)
            {
                isFloat = rval.type == 2;
            }
            else
            {
                v6 = *(float*)lvalData + MATH_gd_magicDoubleForRound;
                lvalData[0] = *((uint32_t*)&v6);
                isFloat = 0;
            }
        }
    }
    if (isFloat)
    {
        a1[0] = *(float*)rvalData >= *(float*)lvalData;
        AI_PutObjectToStack(a1, AI_gaw_EnumLink[32]);
    }
    else
    {
        v3[0] = rvalData[0] >= lvalData[0];
        AI_PutObjectToStack(v3, AI_gaw_EnumLink[32]);
    }
    return arg0 + 1;
}

tAINode* AI_EvalKeyword_Great(tAINode* arg0)
{
    float v2;
    int v3[3];
    int a1[3];
    BOOL isFloat;
    double v6;
    int lvalData[13];
    tVarStackElem rval;
    int rvalData[13];
    tVarStackElem lval;

    AI_GetVarFromStack(lvalData, &lval);
    AI_GetVarFromStack(rvalData, &rval);

    if (rval.type == lval.type) {
        isFloat = rval.type == 2;
    }
    else {
        if (rval.type == 2)
        {
            v2 = (float)lvalData[0];
            *(float*)lvalData = v2;
            isFloat = 1;
        }
        else {
            if (rval.type != 1)
            {
                isFloat = rval.type == 2;
            }
            else
            {
                v6 = *(float*)lvalData + MATH_gd_magicDoubleForRound;
                lvalData[0] = *((uint32_t*)&v6);
                isFloat = 0;
            }
        }
    }
    if (isFloat)
    {
        a1[0] = *(float*)rvalData > *(float*)lvalData;
        AI_PutObjectToStack(a1, AI_gaw_EnumLink[32]);
    }
    else
    {
        v3[0] = rvalData[0] > lvalData[0];
        AI_PutObjectToStack(v3, AI_gaw_EnumLink[32]);
    }
    return arg0 + 1;
}

tAINode* AI_EvalKeyword_LessEq(tAINode* arg0)
{
    float v2;
    int v3[3];
    int a1[3];
    BOOL isFloat;
    double v6;
    int lvalData[13];
    tVarStackElem rval;
    int rvalData[13];
    tVarStackElem lval;

    AI_GetVarFromStack(lvalData, &lval);
    AI_GetVarFromStack(rvalData, &rval);

    if (rval.type == lval.type) {
        isFloat = rval.type == 2;
    }
    else {
        if (rval.type == 2)
        {
            v2 = (float)lvalData[0];
            *(float*)lvalData = v2;
            isFloat = 1;
        }
        else {
            if (rval.type != 1)
            {
                isFloat = rval.type == 2;
            }
            else
            {
                v6 = *(float*)lvalData + MATH_gd_magicDoubleForRound;
                lvalData[0] = *((uint32_t*)&v6);
                isFloat = 0;
            }
        }
    }
    if (isFloat)
    {
        a1[0] = *(float*)rvalData <= *(float*)lvalData;
        AI_PutObjectToStack(a1, AI_gaw_EnumLink[32]);
    }
    else
    {
        v3[0] = rvalData[0] <= lvalData[0];
        AI_PutObjectToStack(v3, AI_gaw_EnumLink[32]);
    }
    return arg0 + 1;
}

tAINode* AI_EvalKeyword_Less(tAINode* arg0)
{
    float v2;
    int v3[3];
    int a1[3];
    BOOL isFloat;
    double v6;
    int lvalData[13];
    tVarStackElem rval;
    int rvalData[13];
    tVarStackElem lval;

    AI_GetVarFromStack(lvalData, &lval);
    AI_GetVarFromStack(rvalData, &rval);

    if (rval.type == lval.type) {
        isFloat = rval.type == 2;
    }
    else {
        if (rval.type == 2)
        {
            v2 = (float)lvalData[0];
            *(float*)lvalData = v2;
            isFloat = 1;
        }
        else {
            if (rval.type != 1)
            {
                isFloat = rval.type == 2;
            }
            else
            {
                v6 = *(float*)lvalData + MATH_gd_magicDoubleForRound;
                lvalData[0] = *((uint32_t*)&v6);
                isFloat = 0;
            }
        }
    }
    if (isFloat)
    {
        a1[0] = *(float*)rvalData < *(float*)lvalData;
        AI_PutObjectToStack(a1, AI_gaw_EnumLink[32]);
    }
    else
    {
        v3[0] = rvalData[0] < lvalData[0];
        AI_PutObjectToStack(v3, AI_gaw_EnumLink[32]);
    }
    return arg0 + 1;
}

tAINode* AI_EvalKeyword_Not(tAINode* a1)
{
    int v2;
    int dest[13];
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    if (a2.type == 2)
    {
        if (*(float*)dest == 0.0f)
            v2 = 0x3F800000;
        else
            v2 = 0;
        dest[0] = v2;
    }
    else
    {
        dest[0] = (uint8_t)((dest[0] != 0) - 1);
    }
    AI_PutObjectToStack(dest, a2.type);
    return a1 + 1;
}

tAINode* AI_EvalKeyword_Neg(tAINode* a1)
{
    int dest[13];
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    if (a2.type == 2)
        *(float*)dest = -*(float*)dest;
    else
        dest[0] = -dest[0];
    AI_PutObjectToStack(dest, a2.type);
    return a1 + 1;
}

tAINode* AI_EvalKeyword_VecNeg(tAINode* a1)
{
    Vec3 dest;
    tVarStackElem a2;

    AI_GetVarFromStack(&dest, &a2);
    dest.x = -dest.x;
    dest.y = -dest.y;
    dest.z = -dest.z;
    AI_PutVecToStack(&dest);
    return a1 + 1;
}

tAINode* AI_EvalKeyword_VecEqual(tAINode* a1)
{
    Vec3* v2;
    Vec3* v3;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v3 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v2 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    if (v3->x == *(float*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack] && v3->y == v2->y && v3->z == v2->z)
        AI_PutIntToStack(1);
    else
        AI_PutIntToStack(0);
    return a1 + 1;
}

tAINode* AI_EvalKeyword_VecDifferent(tAINode* a1)
{
    Vec3* v2;
    Vec3* v3;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v3 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v2 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    if (v3->x == *(float*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack] && v3->y == v2->y && v3->z == v2->z)
        AI_PutIntToStack(0);
    else
        AI_PutIntToStack(1);
    return a1 + 1;
}

tAINode* AI_EvalKeyword_IncAffect(tAINode* a1)
{
    int dest[13];
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    if (a2.type == 2)
        *(float*)dest = *(float*)dest + 1.0f;
    else
        ++dest[0];
    AI_gast_Types[a2.type].funcptr(dest, a2.ptr);
    return a1 + 1;
}

tAINode* AI_EvalKeyword_DecAffect(tAINode* a1)
{
    int dest[13];
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    if (a2.type == 2)
        *(float*)dest = *(float*)dest - 1.0f;
    else
        --dest[0];
    AI_gast_Types[a2.type].funcptr(dest, a2.ptr);
    return a1 + 1;
}

tAINode* AI_EvalKeyword_Minus(tAINode* a1)
{
    int v2;
    int v3;
    BOOL isFloat;
    int a1a;
    float a1b;
    int16_t v7;
    int v8;
    int16_t v9;

    v9 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    a1a = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    v7 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v8 = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    if (v7 == v9) {
        isFloat = v7 == 2;
    }
    else {
        if (v7 == 2)
        {
            *(float*)&v3 = (float)a1a;
            a1a = v3;
            isFloat = 1;
        }
        else {
            if (v9 != 2)
            {
                isFloat = v7 == 2;
            }
            else
            {
                *(float*)&v2 = (float)v8;
                v8 = v2;
                isFloat = 1;
            }
        }
    }
    if (isFloat)
    {
        a1b = *(float*)&v8 - *(float*)&a1a;
        AI_PutFloatToStack(a1b);
    }
    else
    {
        AI_PutIntToStack(v8 - a1a);
    }
    return a1 + 1;
}

tAINode* AI_EvalKeyword_Plus(tAINode* a1)
{
    int v2;
    int v3;
    BOOL isFloat;
    int a1a;
    float a1b;
    int16_t v7;
    int v8;
    int16_t v9;

    v9 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    a1a = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    v7 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v8 = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    if (v7 == v9) {
        isFloat = v7 == 2;
    }
    else {
        if (v7 == 2)
        {
            *(float*)&v3 = (float)a1a;
            a1a = v3;
            isFloat = 1;
        }
        else {
            if (v9 != 2)
            {
                isFloat = v7 == 2;
            }
            else
            {
                *(float*)&v2 = (float)v8;
                v8 = v2;
                isFloat = 1;
            }
        }
    }
    if (isFloat)
    {
        a1b = *(float*)&v8 + *(float*)&a1a;
        AI_PutFloatToStack(a1b);
    }
    else
    {
        AI_PutIntToStack(v8 + a1a);
    }
    return a1 + 1;
}

tAINode* AI_EvalKeyword_Mul(tAINode* a1)
{
    int v2;
    int v3;
    BOOL isFloat;
    int a1a;
    float a1b;
    int16_t v7;
    int v8;
    int16_t v9;

    v9 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    a1a = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    v7 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v8 = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    if (v7 == v9) {
        isFloat = v7 == 2;
    }
    else {
        if (v7 == 2)
        {
            *(float*)&v3 = (float)a1a;
            a1a = v3;
            isFloat = 1;
        }
        else {
            if (v9 != 2)
            {
                isFloat = v7 == 2;
            }
            else
            {
                *(float*)&v2 = (float)v8;
                v8 = v2;
                isFloat = 1;
            }
        }
    }
    if (isFloat)
    {
        a1b = *(float*)&v8 * *(float*)&a1a;

        /* Temporary: a product that is NaN or far out of range turns
           into 0x80000000 the moment anything reads it as an int, and
           by then both operands are gone. Name them here. */
        if (!(a1b > -1.0e9f && a1b < 1.0e9f))
        {
            static int n;
            if (++n <= 40 || n % 200 == 0)
                DBG_OutputDebugString(
                    "AI: Mul non-finite in behaviour 0x%08x  "
                    "lhs type %d bits 0x%08X (%f)  rhs type %d bits 0x%08X (%f)\n",
                    AI_gpst_CurrentFunction ? AI_gpst_CurrentFunction->key : 0,
                    (int)v7, (unsigned)v8, *(float*)&v8,
                    (int)v9, (unsigned)a1a, *(float*)&a1a);
        }

        AI_PutFloatToStack(a1b);
    }
    else
    {
        AI_PutIntToStack(v8 * a1a);
    }
    return a1 + 1;
}

tAINode* AI_EvalKeyword_Div(tAINode* a1)
{
    int v2;
    int v3;
    BOOL isFloat;
    int a1a;
    float a1b;
    int16_t v7;
    int v8;
    int16_t v9;

    v9 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    a1a = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    v7 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v8 = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    if (v7 == v9) {
        isFloat = v7 == 2;
    }
    else {
        if (v7 == 2)
        {
            *(float*)&v3 = (float)a1a;
            a1a = v3;
            isFloat = 1;
        }
        else {
            if (v9 != 2)
            {
                isFloat = v7 == 2;
            }
            else
            {
                *(float*)&v2 = (float)v8;
                v8 = v2;
                isFloat = 1;
            }
        }
    }
    if (isFloat)
    {
        a1b = *(float*)&v8 / *(float*)&a1a;
        AI_PutFloatToStack(a1b);
    }
    else
    {
        AI_PutIntToStack(v8 / a1a);
    }
    return a1 + 1;
}

tAINode* AI_EvalKeyword_OrOp(tAINode* a1)
{
    int v2;
    int v3;
    BOOL isFloat;
    int a1a;
    int16_t v7;
    int v8;
    int16_t v9;
    double v4;
    double v5;
    float v8a;

    v9 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    a1a = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    v7 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v8 = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    if (v7 == v9) {
        isFloat = v7 == 2;
    }
    else {
        if (v7 == 2)
        {
            *(float*)&v3 = (float)a1a;
            a1a = v3;
            isFloat = 1;
        }
        else {
            if (v9 != 2)
            {
                isFloat = v7 == 2;
            }
            else
            {
                *(float*)&v2 = (float)v8;
                v8 = v2;
                isFloat = 1;
            }
        }
    }
    if (isFloat)
    {
        v5 = *(float*)&v8 + MATH_gd_magicDoubleForRound;
        v4 = *(float*)&a1a + MATH_gd_magicDoubleForRound;
        v8a = (float)(*((int32_t*)&v4) | *((int32_t*)&v5));
        AI_PutFloatToStack(v8a);
    }
    else
    {
        AI_PutIntToStack(a1a | v8);
    }
    return a1 + 1;
}

tAINode* AI_EvalKeyword_AndOp(tAINode* a1)
{
    int v2;
    int v3;
    BOOL isFloat;
    int a1a;
    int16_t v7;
    int v8;
    int16_t v9;
    double v4;
    double v5;
    float v8a;

    v9 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    a1a = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    v7 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v8 = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    if (v7 == v9) {
        isFloat = v7 == 2;
    }
    else {
        if (v7 == 2)
        {
            *(float*)&v3 = (float)a1a;
            a1a = v3;
            isFloat = 1;
        }
        else {
            if (v9 != 2)
            {
                isFloat = v7 == 2;
            }
            else
            {
                *(float*)&v2 = (float)v8;
                v8 = v2;
                isFloat = 1;
            }
        }
    }
    if (isFloat)
    {
        v5 = *(float*)&v8 + MATH_gd_magicDoubleForRound;
        v4 = *(float*)&a1a + MATH_gd_magicDoubleForRound;
        v8a = (float)(*((int32_t*)&v4) & *((int32_t*)&v5));
        AI_PutFloatToStack(v8a);
    }
    else
    {
        AI_PutIntToStack(a1a & v8);
    }
    return a1 + 1;
}

tAINode* AI_EvalKeyword_XorOp(tAINode* a1)
{
    int v2;
    int v3;
    BOOL isFloat;
    int a1a;
    int16_t v7;
    int v8;
    int16_t v9;
    double v4;
    double v5;
    float v8a;

    v9 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    a1a = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    v7 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v8 = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    if (v7 == v9) {
        isFloat = v7 == 2;
    }
    else {
        if (v7 == 2)
        {
            *(float*)&v3 = (float)a1a;
            a1a = v3;
            isFloat = 1;
        }
        else {
            if (v9 != 2)
            {
                isFloat = v7 == 2;
            }
            else
            {
                *(float*)&v2 = (float)v8;
                v8 = v2;
                isFloat = 1;
            }
        }
    }
    if (isFloat)
    {
        v5 = *(float*)&v8 + MATH_gd_magicDoubleForRound;
        v4 = *(float*)&a1a + MATH_gd_magicDoubleForRound;
        v8a = (float)(*((int32_t*)&v4) ^ *((int32_t*)&v5));
        AI_PutFloatToStack(v8a);
    }
    else
    {
        AI_PutIntToStack(a1a ^ v8);
    }
    return a1 + 1;
}

tAINode* AI_EvalKeyword_SLeft(tAINode* a1)
{
    int v2;
    int v3;
    BOOL isFloat;
    int a1a;
    int16_t v7;
    int v8;
    int16_t v9;
    double v4;
    double v5;
    float v8a;

    v9 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    a1a = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    v7 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v8 = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    if (v7 == v9) {
        isFloat = v7 == 2;
    }
    else {
        if (v7 == 2)
        {
            *(float*)&v3 = (float)a1a;
            a1a = v3;
            isFloat = 1;
        }
        else {
            if (v9 != 2)
            {
                isFloat = v7 == 2;
            }
            else
            {
                *(float*)&v2 = (float)v8;
                v8 = v2;
                isFloat = 1;
            }
        }
    }
    if (isFloat)
    {
        v5 = *(float*)&v8 + MATH_gd_magicDoubleForRound;
        v4 = *(float*)&a1a + MATH_gd_magicDoubleForRound;
        v8a = (float)(*((int32_t*)&v4) << *((int8_t*)&v5));
        AI_PutFloatToStack(v8a);
    }
    else
    {
        AI_PutIntToStack(v8 << a1a);
    }
    return a1 + 1;
}

tAINode* AI_EvalKeyword_SRight(tAINode* a1)
{
    int v2;
    int v3;
    BOOL isFloat;
    int a1a;
    int16_t v7;
    int v8;
    int16_t v9;
    double v4;
    double v5;
    float v8a;

    v9 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    a1a = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    v7 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v8 = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    if (v7 == v9) {
        isFloat = v7 == 2;
    }
    else {
        if (v7 == 2)
        {
            *(float*)&v3 = (float)a1a;
            a1a = v3;
            isFloat = 1;
        }
        else {
            if (v9 != 2)
            {
                isFloat = v7 == 2;
            }
            else
            {
                *(float*)&v2 = (float)v8;
                v8 = v2;
                isFloat = 1;
            }
        }
    }
    if (isFloat)
    {
        v5 = *(float*)&v8 + MATH_gd_magicDoubleForRound;
        v4 = *(float*)&a1a + MATH_gd_magicDoubleForRound;
        v8a = (float)(*((int32_t*)&v4) >> *((int8_t*)&v5));
        AI_PutFloatToStack(v8a);
    }
    else
    {
        AI_PutIntToStack(v8 >> a1a);
    }
    return a1 + 1;
}

/* An lvalue that evaluated to NULL. Reports which behaviour and which
   keyword hit it and skips the write, so a bad tree names itself
   instead of taking the process down. */
static BOOL AI_b_NullLValue(const void* dst, const char* keyword)
{
    if (dst)
        return FALSE;
    DBG_OutputDebugString(
        "AI: %s: destination is NULL in behaviour 0x%08x; write skipped\n",
        keyword,
        AI_gpst_CurrentFunction ? AI_gpst_CurrentFunction->key : 0);
    return TRUE;
}

tAINode* AI_EvalKeyword_Affect(tAINode* a1)
{
    int v2;
    double v3;
    int v4;
    int16_t v5;
    int* v6;
    int16_t v7;

    v7 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v4 = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    v5 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v6 = (int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    if (v5 != v7)
    {
        if (v5 == 2)
        {
            *(float*)&v2 = (float)v4;
            v4 = v2;
        }
        else if (v5 == 1)
        {
            v3 = *(float*)&v4 + MATH_gd_magicDoubleForRound;
            v4 = *((int32_t *)&v3);
        }
    }
    if (AI_b_NullLValue(v6, "Affect"))
        return a1 + 1;
    *v6 = v4;
    return a1 + 1;
}

tAINode* AI_EvalKeyword_MinusAffect(tAINode* a1)
{
    int v2;
    BOOL isFloat;
    double v4;
    int v5;
    int16_t v6;
    char* v7;
    int v8;
    int v9;
    int16_t v10;

    v10 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v5 = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    v6 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v7 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    v8 = *(int32_t*)v7;
    AI_gul_SizeGlobalStack -= 32;
    if (v6 == v10) {
        isFloat = v6 == 2;
    } 
    else {
        if (v6 == 2)
        {
            *(float*)&v2 = (float)v5;
            v5 = v2;
            isFloat = 1;
        } 
        else {
            if (v6 != 1)
            {
                isFloat = v6 == 2;
            }
            else
            {
                v4 = *(float*)&v5 + MATH_gd_magicDoubleForRound;
                v5 = *((int32_t*)&v4);
                isFloat = 0;
            }
        }
    }
    if (isFloat)
        *(float*)&v9 = *(float*)&v8 - *(float*)&v5;
    else
        v9 = v8 - v5;
    *(float*)v7 = *(float*)&v9;
    return a1 + 1;
}

tAINode* AI_EvalKeyword_PlusAffect(tAINode* a1)
{
    int v2;
    BOOL isFloat;
    double v4;
    int v5;
    int16_t v6;
    char* v7;
    int v8;
    int v9;
    int16_t v10;

    v10 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v5 = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    v6 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v7 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    v8 = *(int32_t*)v7;
    AI_gul_SizeGlobalStack -= 32;
    if (v6 == v10) {
        isFloat = v6 == 2;
    }
    else {
        if (v6 == 2)
        {
            *(float*)&v2 = (float)v5;
            v5 = v2;
            isFloat = 1;
        }
        else {
            if (v6 != 1)
            {
                isFloat = v6 == 2;
            }
            else
            {
                v4 = *(float*)&v5 + MATH_gd_magicDoubleForRound;
                v5 = *((int32_t*)&v4);
                isFloat = 0;
            }
        }
    }
    if (isFloat)
        *(float*)&v9 = *(float*)&v8 + *(float*)&v5;
    else
        v9 = v8 + v5;
    *(float*)v7 = *(float*)&v9;
    return a1 + 1;
}

tAINode* AI_EvalKeyword_MulAffect(tAINode* a1)
{
    int v2;
    BOOL isFloat;
    double v4;
    int v5;
    int16_t v6;
    char *v7;
    int v8;
    int v9;
    int16_t v10;

    v10 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v5 = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    v6 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v7 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    v8 = *(int32_t*)v7;
    AI_gul_SizeGlobalStack -= 32;
    if (v6 == v10) {
        isFloat = v6 == 2;
    }
    else {
        if (v6 == 2)
        {
            *(float*)&v2 = (float)v5;
            v5 = v2;
            isFloat = 1;
        }
        else {
            if (v6 != 1)
            {
                isFloat = v6 == 2;
            }
            else
            {
                v4 = *(float*)&v5 + MATH_gd_magicDoubleForRound;
                v5 = *((int32_t*)&v4);
                isFloat = 0;
            }
        }
    }
    if (isFloat)
        *(float*)&v9 = *(float*)&v8 * *(float*)&v5;
    else
        v9 = v8 * v5;
    *(float*)v7 = *(float*)&v9;
    return a1 + 1;
}

tAINode* AI_EvalKeyword_DivAffect(tAINode* a1)
{
    int v2;
    BOOL isFloat;
    double v4;
    int v5;
    int16_t v6;
    char *v7;
    int v8;
    int v9;
    int16_t v10;

    v10 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v5 = *(int32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    v6 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v7 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    v8 = *(int32_t*)v7;
    AI_gul_SizeGlobalStack -= 32;
    if (v6 == v10) {
        isFloat = v6 == 2;
    }
    else {
        if (v6 == 2)
        {
            *(float*)&v2 = (float)v5;
            v5 = v2;
            isFloat = 1;
        }
        else {
            if (v6 != 1)
            {
                isFloat = v6 == 2;
            }
            else
            {
                v4 = *(float*)&v5 + MATH_gd_magicDoubleForRound;
                v5 = *((int32_t*)&v4);
                isFloat = 0;
            }
        }
    }
    if (isFloat)
        *(float*)&v9 = *(float*)&v8 / *(float*)&v5;
    else
        v9 = v8 / v5;
    *(float*)v7 = *(float*)&v9;
    return a1 + 1;
}

tAINode* AI_EvalKeyword_OrAffect(tAINode* a1)
{
    int v2;
    double v3;
    double v4;
    BOOL floats;
    double v6;
    int v7;
    __int16 v8;
    char* v9;
    int v10;
    int v11;
    __int16 v12;

    v12 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v7 = *(int*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    v8 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v9 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    v10 = *(int*)v9;
    AI_gul_SizeGlobalStack -= 32;
    if (v8 == v12)
        goto LABEL_6;
    if (v8 == 2)
    {
        *(float*)&v2 = (float)v7;
        v7 = v2;
        floats = 1;
        goto LABEL_7;
    }
    if (v8 != 1)
    {
    LABEL_6:
        floats = v8 == 2;
    }
    else
    {
        v6 = *(float*)&v7 + MATH_gd_magicDoubleForRound;
        v7 = (*(int32_t *)&v6);
        floats = 0;
    }
LABEL_7:
    if (floats)
    {
        v4 = *(float*)&v10 + MATH_gd_magicDoubleForRound;
        v3 = *(float*)&v7 + MATH_gd_magicDoubleForRound;
        *(float*)&v11 = (float)((*(uint32_t *)&v3) | (*(uint32_t *)&v4));
    }
    else
    {
        v11 = v7 | v10;
    }
    *(uint32_t*)v9 = v11;
    return a1 + 1;
}

tAINode* AI_EvalKeyword_AndAffect(tAINode* a1)
{
    int v2;
    double v3;
    double v4;
    BOOL v5; 
    double v6;
    int v7; 
    __int16 v8; 
    char* v9; 
    int v10; 
    int v11; 
    __int16 v12;

    v12 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v7 = *(uint32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    v8 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v9 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    v10 = *(uint32_t*)v9;
    AI_gul_SizeGlobalStack -= 32;
    if (v8 == v12)
        goto LABEL_6;
    if (v8 == 2)
    {
        *(float*)&v2 = (float)v7;
        v7 = v2;
        v5 = 1;
        goto LABEL_7;
    }
    if (v8 != 1)
    {
    LABEL_6:
        v5 = v8 == 2;
    }
    else
    {
        v6 = *(float*)&v7 + MATH_gd_magicDoubleForRound;
        v7 = (*(uint32_t *)&v6);
        v5 = 0;
    }
LABEL_7:
    if (v5)
    {
        v4 = *(float*)&v10 + MATH_gd_magicDoubleForRound;
        v3 = *(float*)&v7 + MATH_gd_magicDoubleForRound;
        *(float*)&v11 = (float)((*(uint32_t *)&v3) & (*(uint32_t*)&v4));
    }
    else
    {
        v11 = v7 & v10;
    }
    *(uint32_t*)v9 = v11;
    return a1 + 1;
}

tAINode* AI_EvalKeyword_XorAffect(tAINode* a1)
{
    int v2;
    double v3;
    double v4;
    BOOL v5; 
    double v6;
    int v7; 
    __int16 v8; 
    int v9;
    int v10;
    int v11;
    __int16 v12;

    v12 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v7 = *(int*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    v8 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v9 = (int)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    v10 = *(int*)v9;
    AI_gul_SizeGlobalStack -= 32;
    if (v8 == v12)
        goto LABEL_6;
    if (v8 == 2)
    {
        *(float*)&v2 = (float)v7;
        v7 = v2;
        v5 = 1;
        goto LABEL_7;
    }
    if (v8 != 1)
    {
    LABEL_6:
        v5 = v8 == 2;
    }
    else
    {
        v6 = *(float*)&v7 + MATH_gd_magicDoubleForRound;
        v7 = (*(int32_t *)&v6);
        v5 = 0;
    }
LABEL_7:
    if (v5)
    {
        v4 = *(float*)&v10 + MATH_gd_magicDoubleForRound;
        v3 = *(float*)&v7 + MATH_gd_magicDoubleForRound;
        *(float*)&v11 = (float)((*(uint32_t *)&v3) ^ (*(uint32_t *)&v4));
    }
    else
    {
        v11 = v7 ^ v10;
    }
    *(uint32_t*)v9 = v11;
    return a1 + 1;
}

tAINode* AI_EvalKeyword_SLeftAffect(tAINode* a1)
{
    int v2;
    double v3;
    double v4;
    BOOL v5; 
    double v6;
    int v7; 
    __int16 v8; 
    int* v9;
    int v10;
    int v11;
    __int16 v12;

    v12 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v7 = *(uint32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    v8 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v9 = (int*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    v10 = *v9;
    AI_gul_SizeGlobalStack -= 32;
    if (v8 == v12)
        goto LABEL_6;
    if (v8 == 2)
    {
        *(float*)&v2 = (float)v7;
        v7 = v2;
        v5 = 1;
        goto LABEL_7;
    }
    if (v8 != 1)
    {
    LABEL_6:
        v5 = v8 == 2;
    }
    else
    {
        v6 = *(float*)&v7 + MATH_gd_magicDoubleForRound;
        v7 = (*(uint32_t *)&v6);
        v5 = 0;
    }
LABEL_7:
    if (v5)
    {
        v4 = *(float*)&v10 + MATH_gd_magicDoubleForRound;
        v3 = *(float*)&v7 + MATH_gd_magicDoubleForRound;
        *(float*)&v11 = (float)(int)((*(uint32_t *)&v4) << (*(int8_t *)&v3));
    }
    else
    {
        v11 = v10 << (v7 & 0xff);
    }
    *v9 = v11;
    return a1 + 1;
}

tAINode* AI_EvalKeyword_SRightAffect(tAINode* a1)
{
    int v2;
    double v3;
    double v4;
    BOOL v5;
    double v6;
    int v7; 
    __int16 v8;
    int* v9; 
    int v10; 
    int v11; 
    __int16 v12;

    v12 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v7 = *(uint32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gul_SizeGlobalStack -= 32;
    v8 = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    v9 = (int*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    v10 = *v9;
    AI_gul_SizeGlobalStack -= 32;
    if (v8 == v12)
        goto LABEL_6;
    if (v8 == 2)
    {
        *(float*)&v2 = (float)v7;
        v7 = v2;
        v5 = 1;
        goto LABEL_7;
    }
    if (v8 != 1)
    {
    LABEL_6:
        v5 = v8 == 2;
    }
    else
    {
        v6 = *(float*)&v7 + MATH_gd_magicDoubleForRound;
        v7 = (*(uint32_t *)&v6);
        v5 = 0;
    }
LABEL_7:
    if (v5)
    {
        v4 = *(float*)&v10 + MATH_gd_magicDoubleForRound;
        v3 = *(float*)&v7 + MATH_gd_magicDoubleForRound;
        *(float*)&v11 = (float)((*(int32_t *)&v4) >> (*(int8_t *)&v3));
    }
    else
    {
        v11 = v10 >> (v7 & 0xff);
    }
    *v9 = v11;
    return a1 + 1;
}

tAINode* AI_EvalKeyword_Invert(tAINode* a1)
{
    int v1;

    v1 = AI_GetIntFromStack();
    AI_PutIntToStack(~v1);
    return a1 + 1;
}

tAINode* AI_EvalKeyword_VecAffect(tAINode* aiNode)
{
    Vec3* sv, *dv;
    tVarStackElem vse;
    int dest[13];

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    sv = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_GetVarFromStack(dest, &vse);

    dv = (Vec3*)vse.ptr;
    dv->x = sv->x;
    dv->y = sv->y;
    dv->z = sv->z;

    return aiNode + 1;
}

tAINode* AI_EvalKeyword_VecPlusAffect(tAINode* a1)
{
    tVarStackElem v2;
    Vec3* a3;
    Vec3 a2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_GetVarFromStack(&a2, &v2);
    MATH_VecAddVec((Vec3*)v2.ptr, &a2, a3);
    return a1 + 1;
}

tAINode* AI_EvalKeyword_VecMinusAffect(tAINode* a1)
{
    tVarStackElem v2;
    Vec3* a3;
    Vec3 a2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_GetVarFromStack(&a2, &v2);
    MATH_SubstractVec((Vec3*)v2.ptr, &a2, a3);
    return a1 + 1;
}

tAINode* AI_EvalKeyword_VecMulAffect(tAINode* a1)
{
    Vec3* v2;
    float v3; 
    tVarStackElem a2;
    Vec3 dest[2]; // TODO 1 is elég cska a biztonság kedvéért 2 (idában votl rossz)

    v3 = AI_GetFloatFromStack();
    AI_GetVarFromStack(dest, &a2);
    v2 = (Vec3*)a2.ptr;
    *(float*)a2.ptr = dest[0].x * v3;
    v2->y = dest[0].y * v3;
    v2->z = dest[0].z * v3;
    return a1 + 1;
}

tAINode* AI_EvalKeyword_VecDivAffect(tAINode* a1)
{
    Vec3* v2;
    float v3;
    float v4;
    tVarStackElem a2;
    Vec3 dest[2];

    v4 = AI_GetFloatFromStack();
    AI_GetVarFromStack(dest, &a2);
    v3 = (float)(1.0 / v4);
    v2 = (Vec3*)a2.ptr;
    *(float*)a2.ptr = dest[0].x * v3;
    v2->y = dest[0].y * v3;
    v2->z = dest[0].z * v3;
    return a1 + 1;
}

tAINode* AI_EvalKeyword_VecMinus(tAINode* a1)
{
    Vec3* a3;
    Vec3* dest;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    dest = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    MATH_SubstractVec(
        (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack],
        (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack],
        a3);
    AI_PutVecToStack(dest);
    return a1 + 1;
}

tAINode* AI_EvalKeyword_VecPlus(tAINode* arg0)
{
    Vec3* a3;
    Vec3* a1;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a1 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    MATH_VecAddVec(
        (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack],
        (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack],
        a3);
    AI_PutVecToStack(a1);
    return arg0 + 1;
}

tAINode* AI_EvalKeyword_VecMul(tAINode* arg0)
{
    float v2;
    float v3;
    Vec3 dest[4];
    tVarStackElem v5;
    Vec3 a1;
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    AI_GetVarFromStack(&a1, &v5);
    if (v5.type == 3)
    {
        if (a2.type != 2)
        {
            v3 = (float)(*(int32_t *)&(dest[0].x));
            dest[0].x = v3;
        }
        a1.x = a1.x * dest[0].x;
        a1.y = a1.y * dest[0].x;
        a1.z = a1.z * dest[0].x;
        AI_PutVecToStack(&a1);
    }
    else
    {
        if (v5.type != 2)
        {
            v2 = (float)(*(int32_t *)&(a1.x));
            a1.x = v2;
        }
        dest[0].x = dest[0].x * a1.x;
        dest[0].y = dest[0].y * a1.x;
        dest[0].z = dest[0].z * a1.x;
        AI_PutVecToStack(dest);
    }
    return arg0 + 1;
}

tAINode* AI_EvalKeyword_VecDiv(tAINode* arg0)
{
    float v2;
    float v3;
    float v4;
    float v5;
    Vec3 dest[4];
    tVarStackElem v7; 
    Vec3 a1;
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    AI_GetVarFromStack(&a1, &v7);
    if (v7.type == 3)
    {
        if (a2.type != 2)
        {
            v3 = (float)(*(int32_t *)&(dest[0].x));
            dest[0].x = v3;
        }
        v5 = (float)(1.0 / dest[0].x);
        a1.x = a1.x * v5;
        a1.y = a1.y * v5;
        a1.z = a1.z * v5;
        AI_PutVecToStack(&a1);
    }
    else
    {
        if (v7.type != 2)
        {
            v2 = (float)(*(int32_t *)&(a1.x));
            a1.x = v2;
        }
        v4 = (float)(1.0 / a1.x);
        dest[0].x = dest[0].x * v4;
        dest[0].y = dest[0].y * v4;
        dest[0].z = dest[0].z * v4;
        AI_PutVecToStack(dest);
    }
    return arg0 + 1;
}

tAINode* AI_EvalKeyword_VecMul2(tAINode* a1)
{
    float v2;
    Vec3 dest[4];
    tVarStackElem v4;
    float v5[13]; 
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    AI_GetVarFromStack(v5, &v4);
    if (v4.type != 2)
    {
        v2 = (float)(*(int32_t *)&(v5[0]));
        v5[0] = v2;
    }
    dest[0].x = dest[0].x * v5[0];
    dest[0].y = dest[0].y * v5[0];
    dest[0].z = dest[0].z * v5[0];
    AI_PutVecToStack(dest);
    return a1 + 1;
}

tAINode* AI_EvalKeyword_MsgAffect(tAINode* a1)
{
    tMessage Src;
    tVarStackElem v3;
    tMessage dest;
    tVarStackElem a2;

    AI_GetVarFromStack(&Src, &a2);
    AI_GetVarFromStack(&dest, &v3);
    memcpy(&dest, &Src, sizeof(dest));
    AI_gast_Types[v3.type].funcptr((void *)&dest, (void*)v3.ptr);
    return a1 + 1;
}

tAINode* AI_EvalKeyword_TextAffect(tAINode* a1)
{
    char Src[52];
    tVarStackElem v3;
    char dest[52];
    tVarStackElem a2;

    AI_GetVarFromStack(Src, &a2);
    AI_GetVarFromStack(dest, &v3);
    memcpy(dest, Src, 8u);
    AI_gast_Types[v3.type].funcptr((void *)dest, (void*)v3.ptr);
    return a1 + 1;
}

tAINode* AI_EvalKeyword_TextPlus(tAINode* a1)
{
    int a1a[13];
    tVarStackElem a2;
    int dest[13];
    tVarStackElem v5;

    AI_GetVarFromStack(dest, &a2);
    AI_GetVarFromStack(a1a, &v5);
    a1a[1] += dest[0];                            // tTextGet.idx += ..
    AI_PutObjectToStack(a1a, v5.type);
    return a1 + 1;
}

tOfc_scriptengine_functions AI_gast_Keywords[74] = {

                                { 0x800,      0, AI_dum },
                                { 0x801,      0, AI_dum },
                                { 0x802,      0, AI_dum },
                                { 0x803,      0, AI_dum },
                                { 0x804,      0, AI_dum },
                                { 0x805,      0, AI_EvalKeyword_Meta },
                                { 0x806,      0, AI_EvalKeyword_Ultra },
                                { 0x807,      0, AI_EvalKeyword_During },
                                { 0x808,      0, AI_EvalKeyword_DuringInit },
                                { 0x809,      0, AI_dum },
                                { 0x834,      0, AI_EvalKeyword_Return },
                                { 0x835,      0, AI_dum },
                                { 0x836,      0, AI_EvalKeyword_Push },
                                { 0x837,      0, AI_EvalKeyword_Pop },
                                { 0x838,      0, AI_EvalKeyword_PopSP },
                                { 0x839,      0, AI_EvalKeyword_PopNow },
                                { 0x83A,      0, AI_EvalKeyword_Stop },
                                { 0x83B,      0, AI_EvalKeyword_ReturnTrack },
                                { 0x898,      0, AI_EvalKeyword_Affect },
                                { 0x899,      0, AI_EvalKeyword_PlusAffect },
                                { 0x89A,      0, AI_EvalKeyword_MinusAffect },
                                { 0x89B,      0, AI_EvalKeyword_IncAffect },
                                { 0x89C,      0, AI_EvalKeyword_DecAffect },
                                { 0x89D,      0, AI_EvalKeyword_MulAffect },
                                { 0x89E,      0, AI_EvalKeyword_DivAffect },
                                { 0x89F,      0, AI_EvalKeyword_Not },
                                { 0x8A0,      0, AI_EvalKeyword_Neg },
                                { 0x8A1,      0, AI_EvalKeyword_Equal },
                                { 0x8A2,      0, AI_EvalKeyword_GreatEq },
                                { 0x8A3,      0, AI_EvalKeyword_LessEq },
                                { 0x8A4,      0, AI_EvalKeyword_Great },
                                { 0x8A5,      0, AI_EvalKeyword_Less },
                                { 0x8A6,      0, AI_EvalKeyword_Different },
                                { 0x8A7,      0, AI_EvalKeyword_Minus },
                                { 0x8A8,      0, AI_EvalKeyword_Plus },
                                { 0x8A9,      0, AI_EvalKeyword_Mul },
                                { 0x8AA,      0, AI_EvalKeyword_Div },
                                { 0x8AB,      0, AI_EvalKeyword_OrAffect },
                                { 0x8AC,      0, AI_EvalKeyword_AndAffect },
                                { 0x8AD,      0, AI_EvalKeyword_XorAffect },
                                { 0x8AE,      0, AI_EvalKeyword_OrOp },
                                { 0x8AF,      0, AI_EvalKeyword_AndOp },
                                { 0x8B0,      0, AI_EvalKeyword_XorOp },
                                { 0x8B1,      0, AI_EvalKeyword_SLeft },
                                { 0x8B2,      0, AI_EvalKeyword_SRight },
                                { 0x8B3,      0, AI_EvalKeyword_SLeftAffect },
                                { 0x8B4,      0, AI_EvalKeyword_SRightAffect },
                                { 0x8B5,      0, AI_EvalKeyword_Invert },
                                { 0x8B6,      0, AI_EvalKeyword_ObjEqual },
                                { 0x8B7,      0, AI_EvalKeyword_ObjDifferent },
                                { 0x8B8,      0, AI_EvalKeyword_ObjNot },
                                { 0x8FC,      0, AI_EvalKeyword_Jump },
                                { 0x8FD,      0, AI_EvalKeyword_JumpFalse },
                                { 0x8FE,      0, AI_EvalKeyword_JumpTrue },
                                { 0x900,      0, AI_EvalKeyword_JumpFalseSP },
                                { 0x901,      0, AI_EvalKeyword_JumpTrueSP },
                                { 0x902,      0, AI_EvalKeyword_JumpFF },
                                { 0x903,      0, AI_dum },
                                { 0x960,      0, AI_EvalKeyword_VecAffect },
                                { 0x961,      0, AI_EvalKeyword_VecPlusAffect },
                                { 0x962,      0, AI_EvalKeyword_VecMinusAffect },
                                { 0x963,      0, AI_EvalKeyword_VecMulAffect },
                                { 0x964,      0, AI_EvalKeyword_VecDivAffect },
                                { 0x965,      0, AI_EvalKeyword_VecMinus },
                                { 0x966,      0, AI_EvalKeyword_VecPlus },
                                { 0x967,      0, AI_EvalKeyword_VecMul },
                                { 0x968,      0, AI_EvalKeyword_VecDiv },
                                { 0x969,      0, AI_EvalKeyword_VecNeg },
                                { 0x96A,      0, AI_EvalKeyword_VecEqual },
                                { 0x96B,      0, AI_EvalKeyword_VecDifferent },
                                { 0x96C,      0, AI_EvalKeyword_VecMul2 },
                                { 0x9C4,      0, AI_EvalKeyword_MsgAffect },
                                { 0x9F6,      0, AI_EvalKeyword_TextAffect },
                                { 0x9F7,      0, AI_EvalKeyword_TextPlus }
};
