#include "../ai.h"
#include "../memory.h"
#include "../engine.h"
#include "ai_aifunctions.h"

void AI_EvalFunc_CastVector_C(float a1, float a2, float a3, Vec3* a4)
{
    a4->x = a1;
    a4->y = a2;
    a4->z = a3;
}

tAINode* AI_EvalFunc_CastVector(tAINode* a1)
{
    float v2;
    float v3;
    float v4;
    Vec3 a4; 
    tVarStackElem v6;
    float dest[13];
    tVarStackElem v8;
    float v9[13];
    float v10[13];
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    AI_GetVarFromStack(v9, &v8);
    AI_GetVarFromStack(v10, &v6);
    if (a2.type != 2)
    {
        v4 = (float)(*(int32_t*)&dest[0]);
        dest[0] = v4;
    }
    if (v8.type != 2)
    {
        v3 = (float)(*(int32_t*)&v9[0]);
        v9[0] = v3;
    }
    if (v6.type != 2)
    {
        v2 = (float)(*(int32_t*)&v10[0]);
        v10[0] = v2;
    }
    AI_EvalFunc_CastVector_C(v10[0], v9[0], dest[0], &a4);
    AI_PutVecToStack(&a4);
    return a1 + 1;
}

int AI_EvalFunc_AIExecute_C(struct sAIFunction* funcPtr)
{
    tAIFunction* currentFunctionBackup;
    tAINode** currentJumpNodeBackup;
    tAINode* jumpNode = NULL; // FIXME: game doesn't initialize it, should we?

    currentFunctionBackup = AI_gpst_CurrentFunction;
    currentJumpNodeBackup = AI_gppst_CurrentJumpNode;

    if (funcPtr && funcPtr != (tAIFunction*)-1)
    {
        jumpNode = funcPtr->nodes;
        if (!jumpNode)
        {
            if (funcPtr->funcPtr)
                jumpNode = (tAINode*)-1;
        }
        ++AI_gi_RunFctLevel;
        jumpNode = AI_pst_RunFunction(funcPtr, &jumpNode, AI_gpst_CurrentFunction->stackOffsetNum);
        --AI_gi_RunFctLevel;
    }
    AI_gpst_CurrentFunction = currentFunctionBackup;
    AI_gppst_CurrentJumpNode = currentJumpNodeBackup;
    if ((unsigned int)jumpNode >= 7)
        return 1000;
    if (jumpNode == (tAINode*)1)
        return 1;
    if (jumpNode == (tAINode*)3)
        return 3;
    if (jumpNode == (tAINode*)4)
        return 4;
    if (jumpNode == (tAINode*)5)
        return 5;
    else
        return 1000;
}

tAINode* AI_EvalFunc_AIExecute(tAINode* a1)
{
    int dest[13];
    unsigned int ret;
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);

    ret = AI_EvalFunc_AIExecute_C((tAIFunction*)dest[0]);
    if (ret >= 7)
        return a1 + 1;

    switch (ret)
    {
    case 1:
        return (tAINode*)1;
    case 3: // reinit
        return (tAINode*)3;
    case 4:
        return (tAINode*)4;
    case 5:
        return (tAINode*)5;
    default:
        return a1 + 1;
    }
}

int AI_EvalFunc_AITrackChange_C(int a1, struct sAIFunction* a2)
{
    int result;
    tAI* v3;

    v3 = AI_gpst_CurrentInstanceUltra;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    if (v3->instanceCallBacks[a1] != a2)
    {
        v3->instanceCallBacks[a1] = a2;
        v3->instanceJumpNodes[a1] = a2->nodes;
        if (!v3->instanceJumpNodes[a1])
        {
            if (v3->instanceCallBacks[a1]->funcPtr)
            {
                v3->instanceCallBackTimeRemaining[a1] = 0.0;
                v3->instanceJumpNodes[a1] = (tAINode*)-1;
            }
        }
    }
    if (a1 < v3->end)
        v3->end = a1;
    result = v3->start;
    if (a1 > result)
        v3->start = a1;
    return result;
}

tAINode* AI_EvalFunc_AITrackChange(tAINode* a1)
{
    double v2;
    int dest[13];
    tVarStackElem v4;
    float v5[13];
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    AI_GetVarFromStack(v5, &v4);
    if (v4.type == 2)
    {
        v2 = v5[0] + MATH_gd_magicDoubleForRound;
        v5[0] = *(float*)&v2;
    }
    AI_EvalFunc_AITrackChange_C((*(int32_t *)&v5[0]), (tAIFunction*)dest[0]);
    return a1 + 1;
}

void AI_EvalFunc_AITrackStop_C(int a1)
{
    tAI* v1;

    v1 = AI_gpst_CurrentInstanceUltra;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    v1->instanceCallBacks[a1] = 0;
    v1->instanceJumpNodes[a1] = 0;
    if (a1 == v1->end)
    {
        while (a1 < 5 && !v1->instanceCallBacks[a1])
            ++a1;
        v1->end = a1;
    }
    if (a1 == v1->start)
    {
        while (a1 && !v1->instanceCallBacks[a1])
            --a1;
        v1->start = a1;
    }
}

tAINode* AI_EvalFunc_AITrackStop(tAINode* a1)
{
    double v2;
    float dest[13];
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    if (a2.type == 2)
    {
        v2 = dest[0] + MATH_gd_magicDoubleForRound;
        dest[0] = *(float*)&v2;
    }
    AI_EvalFunc_AITrackStop_C(*(int32_t *)(&dest[0]));
    return a1 + 1;
}

void AI_EvalFunc_AITrackCurChange_C(struct sAIFunction* a1)
{
    if (AI_gpst_CurrentInstance->instanceCallBacks[AI_gul_CurrentTrack] != a1)
    {
        AI_gpst_CurrentInstance->instanceCallBacks[AI_gul_CurrentTrack] = a1;
        AI_gpst_CurrentInstance->instanceJumpNodes[AI_gul_CurrentTrack] = a1->nodes;
        if (!AI_gpst_CurrentInstance->instanceJumpNodes[AI_gul_CurrentTrack])
        {
            if (AI_gpst_CurrentInstance->instanceCallBacks[AI_gul_CurrentTrack]->funcPtr)
                AI_gpst_CurrentInstance->instanceJumpNodes[AI_gul_CurrentTrack] = (tAINode*)-1;
        }
    }
}

tAINode* AI_EvalFunc_AITrackCurChange(tAINode* a1)
{
    int dest[14];
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    dest[13] = dest[0];
    AI_EvalFunc_AITrackCurChange_C((tAIFunction*)dest[0]);
    return a1 + 1;
}

void AI_EvalFunc_AITrackCurStop_C()
{
    unsigned int v0;

    AI_gpst_CurrentInstance->instanceCallBacks[AI_gul_CurrentTrack] = 0;
    AI_gpst_CurrentInstance->instanceJumpNodes[AI_gul_CurrentTrack] = 0;
    v0 = AI_gul_CurrentTrack;
    if (AI_gul_CurrentTrack == AI_gpst_CurrentInstance->end)
    {
        while (v0 < 5 && !AI_gpst_CurrentInstance->instanceCallBacks[v0])
            ++v0;
        AI_gpst_CurrentInstance->end = v0;
    }
    if (v0 == AI_gpst_CurrentInstance->start)
    {
        while (v0 && !AI_gpst_CurrentInstance->instanceCallBacks[v0])
            --v0;
        AI_gpst_CurrentInstance->start = v0;
    }
}

tAINode* AI_EvalFunc_AITrackCurStop(tAINode* a1)
{
    AI_EvalFunc_AITrackCurStop_C();
    return a1 + 1;
}

int AI_EvalFunc_AITrackCurChangeNow_C(struct sAIFunction* a1)
{
    if (AI_gpst_CurrentFunction == a1)
        return 1000;
    AI_gpst_CurrentFunction = a1;
    AI_gpst_CurrentInstance->instanceCallBacks[AI_gul_CurrentTrack] = a1;
    AI_gpst_CurrentInstance->instanceJumpNodes[AI_gul_CurrentTrack] = AI_gpst_CurrentFunction->nodes;
    if (!AI_gpst_CurrentInstance->instanceJumpNodes[AI_gul_CurrentTrack])
    {
        if (AI_gpst_CurrentInstance->instanceCallBacks[AI_gul_CurrentTrack]->funcPtr)
            AI_gpst_CurrentInstance->instanceJumpNodes[AI_gul_CurrentTrack] = (tAINode*)-1;
    }
    return 5;
}

tAINode* AI_EvalFunc_AITrackCurChangeNow(tAINode* a1)
{
    tAINode* result;
    int dest[13];
    tAINode* v3;
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    v3 = (tAINode*)AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)dest[0]);
    if (v3 == (tAINode*)1000)
        result = a1 + 1;
    else
        result = v3;
    return result;
}

struct sAIFunction* AI_EvalFunc_AITrackGet_C(int a1)
{
    return AI_gpst_CurrentInstance->instanceCallBacks[a1];
}

tAINode* AI_EvalFunc_AITrackGet(tAINode* a1)
{
    int v1;
    int a1a[5];

    v1 = AI_GetIntFromStack();
    a1a[0] = (int)AI_EvalFunc_AITrackGet_C(v1);
    AI_PutObjectToStack(a1a, AI_gaw_EnumLink[39]);
    return a1 + 1;
}

struct sAIFunction* AI_EvalFunc_AITrackCurGet_C()
{
    return AI_gpst_CurrentInstance->instanceCallBacks[AI_gul_CurrentTrack];
}

tAINode* AI_EvalFunc_AITrackCurGet(tAINode* a1)
{
    int a1a[3];

    a1a[0] = (int)AI_EvalFunc_AITrackCurGet_C();
    AI_PutObjectToStack(a1a, AI_gaw_EnumLink[39]);
    return a1 + 1;
}

BOOL AI_EvalFunc_AIIsModel_C(struct sGameObjectMain* a1, struct sAIModel* a2)
{
    BOOL v3;

    if (a1->extended && a1->extended->AIInstance)
        v3 = a1->extended->AIInstance->model == a2;
    else
        v3 = 0;
    return v3;
}

tAINode* AI_EvalFunc_AIIsModel(tAINode* a1)
{
    BOOL v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v1 = AI_EvalFunc_AIIsModel_C(v3, *(tAIModel**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_AIPriorityGet_C(struct sGameObjectMain* a1)
{
    return a1->extended->AIPriority;
}

tAINode* AI_EvalFunc_AIPriorityGet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_AIPriorityGet_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_AIPrioritySet_C(struct sGameObjectMain* a1, char a2)
{
    a1->extended->AIPriority = a2;
}

tAINode* AI_EvalFunc_AIPrioritySet(tAINode* a1)
{
    char v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_AIPrioritySet_C(v3, v1);
    return a1 + 1;
}

void AI_EvalFunc_AIMainActorSet_C(struct sGameObjectMain* a1, int a2)
{
    AI_MainActor[a2] = a1;
}

tAINode* AI_EvalFunc_AIMainActorSet(tAINode* a1)
{
    tGameObjectMain* v2;
    int a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_AIMainActorSet_C(v2, a2);
    return a1 + 1;
}

tGameObjectMain* AI_EvalFunc_AIMainActorGet_C(int a1)
{
    return AI_MainActor[a1];
}

tAINode* AI_EvalFunc_AIMainActorGet(tAINode* a1)
{
    int v2[6];
    int v3; 

    v3 = AI_GetIntFromStack();
    v2[0] = (int)AI_EvalFunc_AIMainActorGet_C(v3);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return a1 + 1;
}

void AI_EvalFunc_AICBAdd_C(struct sGameObjectMain* gao, struct sGameObjectMain* a2, int id, struct sAIFunction* ptr)
{
    tAI* v4;
    int i;

    v4 = a2->extended->AIInstance;
    if (v4->instanceAICBList)
    {
        for (i = 0; i < v4->instanceAICBListNum && v4->instanceAICBList[i].functionData; ++i)
            ;
        if (i == v4->instanceAICBListNum)
        {
            v4->instanceAICBList = (tAiCBElemStruct*)MEM_p_Realloc(v4->instanceAICBList, sizeof(tAiCBElemStruct) * (v4->instanceAICBListNum + 1));
            i = v4->instanceAICBListNum;
            v4->instanceAICBListNum = i + 1;
        }
    }
    else
    {
        v4->instanceAICBList = (tAiCBElemStruct*)MEM_p_Alloc(sizeof(tAiCBElemStruct));
        v4->instanceAICBListNum = 1;
        i = 0;
    }
    v4->instanceAICBList[i].id = id;
    v4->instanceAICBList[i].functionData = ptr;
    v4->instanceAICBList[i].gao = gao;
}

tAINode* AI_EvalFunc_AICBAdd(tAINode* a1)
{
    tGameObjectMain* v2;
    int dest[13];
    tGameObjectMain* gao;
    int ptr;
    int id;
    tVarStackElem a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    gao = v2;
    AI_GetVarFromStack(dest, &a2);
    ptr = dest[0];
    id = AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_AICBAdd_C(gao, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], id, (tAIFunction*)ptr);
    return a1 + 1;
}

void AI_EvalFunc_AICBDel_C(struct sGameObjectMain* a1, struct sGameObjectMain* a2, int a3, struct sAIFunction* a4)
{
    tAI* v4;
    int i;

    v4 = a2->extended->AIInstance;
    for (i = 0; i < v4->instanceAICBListNum; ++i)
    {
        if (v4->instanceAICBList[i].id == a3
            && v4->instanceAICBList[i].functionData == a4
            && v4->instanceAICBList[i].gao == a1)
        {
            v4->instanceAICBList[i].functionData = 0;
            return;
        }
    }
}

tAINode* AI_EvalFunc_AICBDel(tAINode* a1)
{
    tGameObjectMain* v2; 
    int dest[13];
    tGameObjectMain* a1a;
    tAIFunction* a4;
    int a3;
    tVarStackElem a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1a = v2;
    AI_GetVarFromStack(dest, &a2);
    a4 = (tAIFunction*)dest[0];
    a3 = AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_AICBDel_C(a1a, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a3, a4);
    return a1 + 1;
}

int AI_EvalFunc_AITrackCurGetNum_C()
{
    return AI_gul_CurrentTrack;
}

tAINode* AI_EvalFunc_AITrackCurGetNum(tAINode* a1)
{
    int v1;

    v1 = AI_EvalFunc_AITrackCurGetNum_C();
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_AIMainActorReset_C(int a1)
{
    AI_MainActor[a1] = NULL;
}

tAINode* AI_EvalFunc_AIMainActorReset(tAINode* a1)
{
    int v1;

    v1 = AI_GetIntFromStack();
    AI_EvalFunc_AIMainActorReset_C(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_AIReinitUniv(tAINode* a1)
{
    AI_ReinitUniverse();
    return a1 + 1;
}

void AI_EvalFunc_AIReinitVars_C(struct sGameObjectMain* gao)
{
    AI_ReinitInstance(gao->extended->AIInstance);
}

tAINode* AI_EvalFunc_AIReinitVars(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_AIReinitVars_C(v2);
    return a1 + 1;
}

int AI_EvalFunc_AIGetCurSystem_C()
{
    return ENG_PlatformID;
}

tAINode* AI_EvalFunc_AIGetCurSystem(tAINode* a1)
{
    int v1;

    v1 = AI_EvalFunc_AIGetCurSystem_C();
    AI_PutIntToStack(v1);
    return a1 + 1;
}