#include "ai_netfunctions.h"

int AI_EvalFunc_NetCommand_C(int command)
{
    switch (command)
    {
    case 10:
        // the one command the client side answers for itself
        return NET_b_Client && NET_b_Init && !NET_b_Server;

    case 3:
    case 6:
    case 7:
    case 9:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
        return 0;

    default:
        return 1;
    }
}

tAINode* AI_EvalFunc_NETCommand(tAINode* node)
{
    int command;

    // the second argument is read off the stack and dropped
    AI_GetIntFromStack();
    command = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_NetCommand_C(command));
    return node + 1;
}

int AI_EvalFunc_NETCommandStr_C()
{
    return 1;
}

tAINode* AI_EvalFunc_NETCommandStr(tAINode* node)
{
    // both arguments are read off the stack and dropped
    get_a_string_from_globalstack_sub_4D2040();
    AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_NETCommandStr_C());
    return node + 1;
}

tAINode* AI_EvalFunc_NETServerGetName(tAINode* node)
{
    int empty[3];

    empty[0] = 0;
    AI_PutObjectToStack(empty, AI_gaw_EnumLink[38]);
    return node + 1;
}

tAINode* AI_EvalFunv_NETPushi(tAINode* node)
{
    AI_GetIntFromStack();
    AI_PutIntToStack(0);
    return node + 1;
}

tAINode* AI_EvalFunv_NETPushf(tAINode* node)
{
    AI_PutIntToStack(0);
    return node + 1;
}

tAINode* AI_EvalFunv_NETPopf(tAINode* node)
{
    AI_PutFloatToStack(0.0);
    return node + 1;
}

tAINode* AI_EvalFunv_NETPopv(tAINode* node)
{
    // the vector is pushed uninitialised, as the original has it
    Vec3 popped;

    AI_PutVecToStack(&popped);
    return node + 1;
}

tAINode* AI_EvalFunc_NETServerCreate(tAINode* node)
{
    AI_GetIntFromStack();
    return node + 1;
}