#include "ai_netfunctions.h"

int AI_EvalFunc_NetCommand_C(int a1)
{
    int result;
    BOOL v2;

    switch (a1)
    {
    case 3:
        result = 0;
        break;
    case 6:
        result = 0;
        break;
    case 7:
        result = 0;
        break;
    case 9:
        result = 0;
        break;
    case 10:
        v2 = NET_b_Client && NET_b_Init && !NET_b_Server;
        result = v2;
        break;
    case 13:
        result = 0;
        break;
    case 14:
        result = 0;
        break;
    case 15:
        result = 0;
        break;
    case 16:
        result = 0;
        break;
    case 17:
        result = 0;
        break;
    case 18:
        result = 0;
        break;
    default:
        result = 1;
        break;
    }
    return result;
}

tAINode* AI_EvalFunc_NETCommand(tAINode* a1)
{
    int v1; 
    int v3;

    AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_NetCommand_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_NETCommandStr_C()
{
    return 1;
}

tAINode* AI_EvalFunc_NETCommandStr(tAINode* a1)
{
    int v1;

    get_a_string_from_globalstack_sub_4D2040();
    AI_GetIntFromStack();
    v1 = AI_EvalFunc_NETCommandStr_C();
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_NETServerGetName(tAINode* a1)
{
    int v2[3];

    v2[0] = 0;
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[38]);
    return a1 + 1;
}

tAINode* AI_EvalFunv_NETPushi(tAINode* a1)
{
    AI_GetIntFromStack();
    AI_PutIntToStack(0);
    return a1 + 1;
}

tAINode* AI_EvalFunv_NETPushf(tAINode* a1)
{
    AI_PutIntToStack(0);
    return a1 + 1;
}

tAINode* AI_EvalFunv_NETPopf(tAINode* a1)
{
    AI_PutFloatToStack(0.0);
    return a1 + 1;
}

tAINode* AI_EvalFunv_NETPopv(tAINode* arg0)
{
    Vec3 a1;

    AI_PutVecToStack(&a1);
    return arg0 + 1;
}

tAINode* AI_EvalFunc_NETServerCreate(tAINode* a1)
{
    AI_GetIntFromStack();
    return a1 + 1;
}