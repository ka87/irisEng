#include "memory.h"
#include "bigfiles.h"
#include "loading.h"
#include "time.h"
#include "events.h"
#include "ai.h"
#include "sectors.h"
#include "profiler.h"
#include "textget.h"
#include "dbg.h"
#include "ai/ai_compiledscripts.h"
#include "sideload.h"

int AI_CompiledAIEnabled = 1;
int ENG_AIProfilerEnabled = 0; // game doesn't set this explicitly
int AIUnivIsRunning = 1;

int AI_GetIntFromStack()
{
    double v1;

    AI_gul_SizeGlobalStack -= 32;
    if (AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type != 2)
        return *(int32_t*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v1 = *(float*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack] + MATH_gd_magicDoubleForRound;
    return (int32_t)v1;
}

int AI_PutIntToStack(int a1)
{
    tVarStackElem* v2;
    char* v3;

    v3 = &AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    *(int32_t*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack] = a1;
    AI_gul_SizeGlobalStack += 32;
    v2 = &AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1];
    v2->type = 1;
    v2->ptr = v3;
    return ++AI_gul_SizeGlobalVarStack;
}

int AI_PutFloatToStack(float a1)
{
    tVarStackElem* v2;
    char* v3;

    v3 = &AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    *(float*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack] = a1;
    AI_gul_SizeGlobalStack += 32;
    v2 = &AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1];
    v2->type = 2;
    v2->ptr = v3;
    return ++AI_gul_SizeGlobalVarStack;
}

float AI_GetFloatFromStack()
{
    AI_gul_SizeGlobalStack -= 32;
    if (AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type != 1)
        return *(float*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    return (float)*(int*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
}

void AI_PutVecToStack(Vec3* v)
{
    tVarStackElem* v1;
    char* v2;

    v2 = &AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    *(Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack] = *v;
    AI_gul_SizeGlobalStack += 32;
    v1 = &AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1];
    v1->type = 3;
    v1->ptr = v2;
    ++AI_gul_SizeGlobalVarStack;
}

Vec3* AI_GetVecFromStack(Vec3* v)
{
    Vec3* v2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v2 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    *v = *(Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    return v2;
}

void AI_GetVarFromStack(void* dest, tVarStackElem* vse)
{
    vse->type = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    vse->ptr = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    AI_gast_Types[vse->type].funcptr(vse->ptr, dest);
    AI_gul_SizeGlobalStack -= 32;
}

void AI_PutObjectToStack(void* a1, int16_t type)
{
    tVarStackElem* v2;
    char* a2;

    a2 = &AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gast_Types[type].funcptr(a1, &AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    AI_gul_SizeGlobalStack += 32;
    v2 = &AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1];
    v2->type = type;
    v2->ptr = a2;
    ++AI_gul_SizeGlobalVarStack;
}

void AI_PopUserStack(void* a1, tVarStackElem* a2)
{
    memset(a1, 0, sizeof(uint32_t) * 3);

    a2->type = AI_gast_UserVarStack[--AI_gul_SizeUserVarStack].type;
    a2->ptr = AI_gast_UserVarStack[AI_gul_SizeUserVarStack].ptr;
    AI_gast_Types[a2->type].funcptr(a2->ptr, a1);
    AI_gul_SizeUserStack -= AI_gast_Types[a2->type].dataTypeSize;
}

char* AI_PushUserStack(void *a1, int16_t a2)
{
    char* result;
    tVarStackElem* v3;
    char* v4;

    v4 = &AI_gac_UserStack[AI_gul_SizeUserStack];
    AI_gast_Types[a2].funcptr(a1, &AI_gac_UserStack[AI_gul_SizeUserStack]);
    AI_gul_SizeUserStack += AI_gast_Types[a2].dataTypeSize;
    v3 = &AI_gast_UserVarStack[AI_gul_SizeUserVarStack];
    v3->type = a2;
    result = v4;
    v3->ptr = v4;
    ++AI_gul_SizeUserVarStack;
    return result;
}

int AI2C_PopInt()
{
    int v0;
    int a1[13];
    tVarStackElem a2;

    AI_PopUserStack(a1, &a2);
    if (a2.type != 2)
        return a1[0];
    v0 = (int32_t)*(float*)a1;
    return v0;
}

float AI2C_PopFloat()
{
    float v2[13];
    tVarStackElem a2;

    AI_PopUserStack(v2, &a2);
    if (a2.type != 1)
        return v2[0];
    return (float)*((int32_t*)(&v2[0]));
}

struct sGameObjectMain* AI2C_PopGameObject()
{
    tGameObjectMain* a1[13];
    tVarStackElem a2;

    AI_PopUserStack(a1, &a2);
    return a1[0];
}

Vec3* AI2C_PopVector(Vec3* v)
{
    char* v2;
    int v3[13];
    tVarStackElem vse;

    AI_PopUserStack(v3, &vse);
    v2 = vse.ptr;
    *v = *(Vec3*)vse.ptr;
    return (Vec3*)v2;
}

void AI2C_PushInt(int i)
{
    AI_PushUserStack(&i, 1);
}

char* AI2C_PushFloat(float f)
{
    return AI_PushUserStack(&f, 2);
}

char* AI2C_PushVector(Vec3* v)
{
    return AI_PushUserStack(v, 3);
}

char* AI2C_PushGameObject(struct sGameObjectMain* a1)
{
    return AI_PushUserStack(&a1, AI_gaw_EnumLink[40]);// ai_evalType_GetGao
}

void AI2C_AddVector(Vec3* dest, Vec3* v1, Vec3* v2)
{
    MATH_VecAddVec(dest, v1, v2);
}

void AI2C_SubstractVector(Vec3* dest, Vec3* v1, Vec3* v2)
{
    MATH_SubstractVec(dest, v1, v2);
}

void AI2C_AssignVector(Vec3* dest, Vec3* src)
{
    *dest = *src;
}

void AI2C_NegateVector(Vec3* dest, Vec3* src)
{
    dest->x = -src->x;
    dest->y = -src->y;
    dest->z = -src->z;
}

void AI2C_MultiplyVectorWithScalar(Vec3* dest, Vec3* src, float scalar)
{
    dest->x = src->x * scalar;
    dest->y = src->y * scalar;
    dest->z = src->z * scalar;
}

float AI2C_DotProductVector(Vec3* v1, Vec3* v2)
{
    return v2->z * v1->z + v2->y * v1->y + v1->x * v2->x;
}

float AI2C_VectorLen(Vec3* v)
{
    return MATH_VecLen(v);
}

void AI2C_NormalizeEqVector(Vec3* v)
{
    MATH_NormalizeEqualVector(v);
}

struct sGameObjectMain* AI2C_GetMainActor(void* actorIDOrGao)
{
    if (!actorIDOrGao)
        return NULL;
    if ((uint32_t)actorIDOrGao <= 2)
        actorIDOrGao = AI_MainActor[(uint32_t)actorIDOrGao - 1];
    return actorIDOrGao;
}

char* AI_EvalVarArray_C(int a1, char* a2, int num)
{
    return &a2[AI_gast_Types[a1].dataTypeSize * num + 4];
}

char* AI_EvalVarArray2_C(int a1, char* a2, int a3, int a4)
{
    return &a2[(a3 + *((int32_t*)a2 + 1) * a4) * AI_gast_Types[a1].dataTypeSize + 8];
}

char* AI_EvalVarArray_C2(char* a1, int a2)
{
    return &a1[AI_gast_Types[2].dataTypeSize * a2 + 4];
}

char* AI_EvalVarArray3_C(int a1, char* a2, int a3, int a4, int a5)
{
    return &a2[(*((int32_t*)a2 + 2) * *((int32_t*)a2 + 1) * a5 + a3 + *((int32_t*)a2 + 2) * a4)
        * AI_gast_Types[a1].dataTypeSize + 12];
}

void AI_EvalVar(tVarStackElem* vse)
{
    tVarStackElem* vse2;

    AI_gast_Types[vse->type].funcptr(vse->ptr, &AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    AI_gul_SizeGlobalStack += 32;
    vse2 = &AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1];
    vse2->type = vse->type;
    vse2->ptr = vse->ptr;
    ++AI_gul_SizeGlobalVarStack;
}

void AI_EvalVarArray(tAINode* aiNode, char* a2)
{
    int dest[13];
    tVarStackElem a2a;

    AI_GetVarFromStack(dest, &a2a);
    a2a.type = aiNode->val2;
    a2a.ptr = AI_EvalVarArray_C(a2a.type, a2, dest[0]);
    AI_EvalVar(&a2a);
}

void AI_EvalVarArray2(tAINode* aiNode, char* a2)
{
    int dest[13];
    tVarStackElem v3;
    int v4[13];
    tVarStackElem a2a;

    AI_GetVarFromStack(dest, &a2a);
    AI_GetVarFromStack(v4, &v3);
    a2a.type = aiNode->val2;
    a2a.ptr = AI_EvalVarArray2_C(a2a.type, a2, dest[0], v4[0]);
    AI_EvalVar(&a2a);
}

void AI_EvalVarArray3(tAINode* aiNode, char* a2)
{
    tVarStackElem v2;
    int dest[13];
    tVarStackElem v4;
    int v5[13];
    int v6[13];
    tVarStackElem a2a;

    AI_GetVarFromStack(dest, &a2a);
    AI_GetVarFromStack(v5, &v4);
    AI_GetVarFromStack(v6, &v2);
    a2a.type = aiNode->val2;
    a2a.ptr = AI_EvalVarArray3_C(a2a.type, a2, dest[0], v5[0], v6[0]);
    AI_EvalVar(&a2a);
}

void AI_InitVarArray(tAINode* aiNode, char* a2)
{
    int dest[13];
    tVarStackElem a2a;
    int i;
    int v5;
    char* v6;

    AI_GetVarFromStack(dest, &a2a);
    v5 = dest[0];
    v6 = a2;
    for (i = 0; i < v5; ++i)
    {
        AI_GetVarFromStack(dest, &a2a);
        *(int32_t*)&v6[4 * i] = dest[0];
    }
}

char* AI_EvalVarArray_C3(char* a1, int a2)
{
    return &a1[AI_gast_Types[3].dataTypeSize * a2 + 4];
}

char* AI_EvalVarArray_C7(char* a1, int a2)
{
    return &a1[AI_gast_Types[7].dataTypeSize * a2 + 4];
}

void AI_Reset()
{
    AI_gul_SizeLocalStack = 0;
    AI_gul_SizeGlobalStack = 0;
    AI_gul_SizeGlobalVarStack = 0;
    AI_gul_SizeUserStack = 0;
    AI_gul_SizeUserVarStack = 0;
    AI_gpst_CurrentFunction = NULL;
    AI_gpst_CurrentInstance = NULL;
    AI_gpst_CurrentInstanceUltra = NULL;
    AI_gpst_CurrentGameObject = NULL;
    AI_gpst_CurrentUltra = NULL;
    AI_gi_RunFctLevel = 0;
    AI_gul_CurrentTrack = 0;
}

void AI_ReinitInstance(tAI* ai)
{
    unsigned int i;
    unsigned int arrayDimensionNum;
    unsigned int variableArrayCount;
    int varOffset;
    byte* ivData;

    if (!ai || !ai->model) return;

    if (WOR_gst_Universe.univAIInstance != ai)
    {
        ai->start = 0;
        ai->end = 5;
        for (i = 0; i < 5; i++)
        {
            if (ai->model->vars->callBacks[i] && i < ai->end)
                ai->end = i;
            if (ai->model->vars->callBacks[i] && i > ai->start)
                ai->start = i;
            if (ai->model->vars)
                ai->instanceCallBacks[i] = ai->model->vars->callBacks[i];
            else
                ai->instanceCallBacks[i] = NULL;
            if (ai->instanceCallBacks[i])
            {
                ai->instanceJumpNodes[i] = ai->instanceCallBacks[i]->nodes;
                if (!ai->instanceJumpNodes[i])
                {
                    if (ai->instanceCallBacks[i]->funcPtr)
                        ai->instanceJumpNodes[i] = (tAINode*)-1;
                }
            }
            else
            {
                ai->instanceJumpNodes[i] = NULL;
            }
            ai->instanceCallBackTimeRemaining[i] = 0.0f;
        }
        if (ai->instanceAICBList)
        {
            MEM_Free(ai->instanceAICBList);
            ai->instanceAICBList = NULL;
            ai->instanceAICBListNum = 0;
        }
    }
    if (ai->model->vars)
    {
        for (i = 0; i < ai->model->vars->varNum; i++)
        {
            if (WOR_gst_Universe.univAIInstance != ai || (ai->model->vars->variables[i].flags & 0x20) != 0)
            {
                varOffset = ai->model->vars->variables[i].varOffset;
                ivData = ai->instanceVarDataInitial;
                if (!ivData)
                    ivData = ai->model->vars->varData;
                if (ai->instanceVarDataCurrent)
                {
                    variableArrayCount = ai->model->vars->variables[i].variableArrayCount;
                    arrayDimensionNum = variableArrayCount >> 30;
                    if (arrayDimensionNum)
                        memcpy(&ai->instanceVarDataCurrent[varOffset], &ivData[varOffset], 4 * arrayDimensionNum);
                    memcpy(
                        &ai->instanceVarDataCurrent[4 * arrayDimensionNum + varOffset], &ivData[4 * arrayDimensionNum + varOffset],
                        AI_gast_Types[ai->model->vars->variables[i].variableType].dataTypeSize * (variableArrayCount & 0x3fffffff));
                }
            }
        }
    }
}

void AI_ReinitUniverse()
{
    AI_ReinitInstance(WOR_gst_Universe.univAIInstance);
}

BOOL AI_SpecialWParam(char a1)
{
    return a1 == 24;
}

unsigned int AI_ul_RealResolveOneRef(int type, unsigned int key)
{
    unsigned int pos;

    if (!key)
        return 0;

    if (type == 40 && key <= 2)
        return key;

    pos = BIG_ul_SearchKeyToPos(key);
    
    if (pos == -1)
        return key;
    else
        return LOA_ul_SearchAddress(pos);
}

BOOL AI_b_IsARef(int type)
{
    switch (type)
    {
    case 39:
    case 40:
    case 42:
    case 43:
        return TRUE;
        break;
    default:
        return FALSE;
        break;
    }
}

int AI_ul_ResolveOneRef(int type, int key)
{
    if (AI_b_IsARef(type))
        return AI_ul_RealResolveOneRef(type, key);
    else
        return key;
}

void AI_ResolveAllFctRef(tAIModel* aiModel)
{
    unsigned int i;
    tAINode* currNode;

    for (i = 0; i < aiModel->funcNum; ++i)
    {
        if (aiModel->funcs[i])
        {
            currNode = aiModel->funcs[i]->nodes;
            if (currNode)
            {
                if ((currNode->flags & 0x4) == 0)
                {
                    currNode->flags |= 0x4;
                    while (currNode->category != 31)
                    {
                        if (currNode->category == 1)
                        {
                            if (currNode->val2 == 38)
                                currNode->val += (int)aiModel->funcs[i]->localStringConstsData;
                            else
                                currNode->val = AI_ul_ResolveOneRef(currNode->val2, currNode->val);
                        }
                        if (!AI_SpecialWParam(currNode->category))
                            currNode->val2 = AI_gaw_EnumLink[currNode->val2];
                        currNode->category = (char)AI_gaw_EnumLink[currNode->category]; // ?
                        ++currNode;
                    }
                }
            }
        }
    }
}

void AI_ResolveVarRef(void* a1, byte* allVarData, AIVarNameElem* a3, AIVarElem* varElems, unsigned int num, int a6)
{
    unsigned int i;
    signed int j;
    int16_t varType;
    byte* varData;
    byte* actVarData;

    if (allVarData)
    {
        for (i = 0; i < num; i++)
        {
            varData = &allVarData[varElems[i].varOffset];
            varType = varElems[i].variableType;
            if (!a6)
                varType = AI_gast_Types[varType].funcid;
            if (AI_b_IsARef(varType))
            {
                actVarData = &varData[4 * (varElems[i].variableArrayCount >> 30)];
                for (j = 0; j < (signed int)(varElems[i].variableArrayCount & 0x3fffffff); j++)
                    *(int32_t*)&actVarData[4 * j] = AI_ul_ResolveOneRef(varType, *(int32_t*)&actVarData[4 * j]);
            }
            if (a6)
                varElems[i].variableType = AI_gaw_EnumLink[varElems[i].variableType];
        }
    }
}

void AI_ResolveModelRef(tAIModel* aiModel)
{
    if (aiModel && !aiModel->isResolved)
    {
        AI_ResolveAllFctRef(aiModel);
        if (aiModel->vars)
            AI_ResolveVarRef(aiModel, aiModel->vars->varData, aiModel->vars->varNames, aiModel->vars->variables, aiModel->vars->varNum, 1);
        aiModel->isResolved = TRUE;
    }
}

void AI_ResolveCarDecHiHiEditor(tAI* ai)
{
    unsigned int i, j;
    unsigned int arrayDimensionNum;
    unsigned int arrayDimensionNum2;
    unsigned int arrayElemNum;
    int processedVarNum;
    int variableType;
    unsigned int instVarOffset, modelVarOffset;

    if (ai->model)
    {
        memcpy(ai->instanceVarDataInitial, ai->model->vars->varData, ai->model->vars->varFullSizeToAlloc);
        processedVarNum = 0;
        AI_ResolveVarRef(ai, ai->varsInstance->varData, ai->varsInstance->varNames, ai->varsInstance->variables, ai->varsInstance->varNum, 1);

        for (i = 0; i < ai->model->vars->varNum; ++i)
        {

            for (j = 0; j < ai->varsInstance->varNum; j++) {
                if (!_stricmp(ai->model->vars->varNames[i].varName, ai->varsInstance->varNames[j].varName)) break;
            }

            if (j != ai->varsInstance->varNum)
            {
                variableType = ai->model->vars->variables[i].variableType;
                if (!ai->model->isResolved)
                    variableType = AI_gaw_EnumLink[variableType];
                modelVarOffset = ai->model->vars->variables[i].varOffset;
                instVarOffset = ai->varsInstance->variables[j].varOffset;
                if (instVarOffset > ai->varsInstance->varFullSizeToAlloc)
                    instVarOffset = 0;

                arrayDimensionNum = ai->varsInstance->variables[j].variableArrayCount >> 30;
                if (ai->varsInstance->variables[j].variableArrayCount == ai->model->vars->variables[i].variableArrayCount)
                {
                    if (arrayDimensionNum)
                        memcpy(&ai->instanceVarDataInitial[modelVarOffset], &ai->model->vars->varData[modelVarOffset], 4 * arrayDimensionNum);
                    memcpy(&ai->instanceVarDataInitial[4 * arrayDimensionNum + modelVarOffset], &ai->varsInstance->varData[4 * arrayDimensionNum + instVarOffset], 
                        (ai->varsInstance->variables[j].variableArrayCount & 0x3fffffff) * AI_gast_Types[variableType].dataTypeSize);
                }
                else
                {
                    arrayElemNum = ai->varsInstance->variables[j].variableArrayCount & 0x3fffffff;
                    if ((int)(ai->varsInstance->variables[j].variableArrayCount & 0x3fffffff) > (signed int)(ai->model->vars->variables[i].variableArrayCount & 0x3fffffff))
                        arrayElemNum = ai->model->vars->variables[i].variableArrayCount & 0x3fffffff;
                    arrayDimensionNum2 = ai->model->vars->variables[i].variableArrayCount >> 30;
                    memcpy(&ai->instanceVarDataInitial[4 * arrayDimensionNum2 + modelVarOffset], &ai->varsInstance->varData[4 * arrayDimensionNum2 + instVarOffset], 
                        (arrayElemNum & 0x3fffffff) * AI_gast_Types[variableType].dataTypeSize);
                }
                ++processedVarNum;
            }
            if (processedVarNum == ai->varsInstance->varNum)
                break;
        }
    }
}

void AI_FreeVarDes(tAIVars* aiVars)
{
    if (aiVars)
    {
        LOA_DeleteAddress((int)aiVars);
        if (aiVars->varData)
            MEM_Free(aiVars->varData);
        if (aiVars->variables)
            MEM_Free(aiVars->variables);
        if (aiVars->varNames)
            MEM_Free(aiVars->varNames);
        if (aiVars->instanceFuncs)
            MEM_Free(aiVars->instanceFuncs);
        MEM_Free(aiVars);
    }
}

void AI_InstanceAfterLoading(tAI* ai)
{
    if (!ai->instanceVarDataCurrent)
    {
        ++ai->model->instanceCnt;
        ai->instanceVarDataCurrent = NULL;
        ai->instanceVarDataInitial = NULL;
        if (ai->model->vars)
        {
            if (ai->model->vars->varFullSizeToAlloc)
                ai->instanceVarDataCurrent = (byte*)MEM_p_Alloc(ai->model->vars->varFullSizeToAlloc);
            if (ai->model->vars->varFullSizeToAlloc)
                ai->instanceVarDataInitial = (byte*)MEM_p_Alloc(ai->model->vars->varFullSizeToAlloc);
            if (ai->varsInstance)
            {
                AI_ResolveCarDecHiHiEditor(ai);
                AI_FreeVarDes(ai->varsInstance);
                ai->varsInstance = NULL;
            }
            else
            {
                memcpy(ai->instanceVarDataInitial, ai->model->vars->varData, ai->model->vars->varFullSizeToAlloc);
            }
        }
    }
}

void AI_ResolveInstanceRef(tAI* ai)
{
    if (ai)
    {
        AI_ResolveModelRef(ai->model);
        AI_InstanceAfterLoading(ai);
    }
}

tAINode* AI_dum(tAINode* aiNode)
{
    return aiNode + 1;
}

/* The last few nodes run, for reporting a stack fault in context. */
tAINodeTrace AI_gast_Trace[AI_TRACE_LEN];
int AI_gi_TraceAt;

tAINode* AI_pst_RunFunction(tAIFunction* funcPtr, tAINode** jumpNode, int stackDecrease)
{
    tAINode* i;

    AI_gppst_CurrentJumpNode = jumpNode;
    AI_gpst_CurrentFunction = funcPtr;
    if (AI_CompiledAIEnabled && funcPtr->funcPtr)
        return (tAINode*)funcPtr->funcPtr();
    AI_gul_SizeLocalStack += stackDecrease;
    /* Temporary: the cutscene camera's inputs, as it starts a pass.
       Offsets are the camera model's o_cine_objet_dest,
       o_cine_objet_cible, v_cine_pos_dest and v_cine_offset_cible. */
    if (funcPtr && funcPtr->key == 0x35009614 && AI_gpst_CurrentInstance
        && AI_gpst_CurrentInstance->instanceVarDataCurrent)
    {
        byte* v = AI_gpst_CurrentInstance->instanceVarDataCurrent;
        DBG_OutputDebugString(
            "AI: cine dest=%p cible=%p pos_dest(%f %f %f) "
            "offset_cible(%f %f %f) must_cut=%d delai_trans=%f mode=%d\n",
            *(void**)(v + 180), *(void**)(v + 184),
            *(float*)(v + 188), *(float*)(v + 192), *(float*)(v + 196),
            *(float*)(v + 200), *(float*)(v + 204), *(float*)(v + 208),
            *(int*)(v + 144), *(float*)(v + 232), *(int*)(v + 224));
    }
    AI_gi_TraceAt = 0;
    for (i = *jumpNode; (unsigned int)i >= 7 && i->category != 31; i = AI_gast_Categs[i->category].funcptr(i))
    {
        AI_gppst_CurrentJumpNode = jumpNode;
        /* Keep the last few nodes and the stack depth each one left
           behind. Where the depth first drifts is the fault; where it
           goes negative is only where the drift finally shows. */
        AI_gast_Trace[AI_gi_TraceAt & (AI_TRACE_LEN - 1)].node =
            funcPtr && funcPtr->nodes ? (int)(i - funcPtr->nodes) : -1;
        AI_gast_Trace[AI_gi_TraceAt & (AI_TRACE_LEN - 1)].category = i->category;
        AI_gast_Trace[AI_gi_TraceAt & (AI_TRACE_LEN - 1)].val = i->val;
        AI_gast_Trace[AI_gi_TraceAt & (AI_TRACE_LEN - 1)].val2 = i->val2;
        AI_gast_Trace[AI_gi_TraceAt & (AI_TRACE_LEN - 1)].depth = AI_gul_SizeGlobalVarStack;
        ++AI_gi_TraceAt;

        /* The expression stack went negative while this node ran. Name
           the behaviour and the node rather than letting the next
           keyword read element -1. */
        if (AI_gul_SizeGlobalVarStack < 0)
        {
            int t, first;
            DBG_OutputDebugString(
                "AI: var stack underflow in behaviour 0x%08x at node %d "
                "(category %d val %d val2 %d)\n",
                funcPtr ? funcPtr->key : 0,
                funcPtr && funcPtr->nodes ? (int)(i - funcPtr->nodes) : -1,
                i->category, i->val, i->val2);
            first = AI_gi_TraceAt > AI_TRACE_LEN ? AI_gi_TraceAt - AI_TRACE_LEN : 0;
            for (t = first; t < AI_gi_TraceAt; ++t)
            {
                DBG_OutputDebugString(
                    "AI:   node %5d  categ %2d  val %10d  val2 %5d  depth %d\n",
                    AI_gast_Trace[t & (AI_TRACE_LEN - 1)].node,
                    AI_gast_Trace[t & (AI_TRACE_LEN - 1)].category,
                    AI_gast_Trace[t & (AI_TRACE_LEN - 1)].val,
                    AI_gast_Trace[t & (AI_TRACE_LEN - 1)].val2,
                    AI_gast_Trace[t & (AI_TRACE_LEN - 1)].depth);
            }
            AI_gul_SizeGlobalVarStack = 0;
            break;
        }
    }
    AI_gul_SizeLocalStack -= stackDecrease;
    return i;
}

void AI_RunNode(tAINode* aiNode)
{
    if (!aiNode->val && AI_gast_Categs[aiNode[1].category].funcid == 2 /*AI_EvalCateg_Keyword*/ && AI_gast_Keywords[aiNode[1].val2].funcid == 2054 /*AI_EvalKeyword_Ultra*/)
        aiNode += 2; // skip ultra set
    while ((unsigned int)aiNode > 7 && aiNode->category != 31)
        aiNode = AI_gast_Categs[aiNode->category].funcptr(aiNode);
}

int AI_RunInstance(struct sAI* ai)
{
    int i;
    tAINode* ret;

    if (!ai)
        return 1;

    if (!ai->model)
        return 1;

    if (ai->model->isDisabled)
        return 1;

    AI_gpst_CurrentInstanceUltra = ai;
    AI_gpst_CurrentInstance = ai;
    AI_gi_RunFctLevel = 0;
    
    for (i = ai->end; i < ai->start + 1; i++)
    {
        AI_gul_SizeGlobalStack = 0;
        AI_gul_SizeGlobalVarStack = 0;
        AI_gul_SizeUserStack = 0;
        AI_gul_SizeUserVarStack = 0;
        AI_gul_CurrentTrack = i;
        if (ai->instanceCallBackTimeRemaining[i] > 0.0f)
        {
            ai->instanceCallBackTimeRemaining[i] = ai->instanceCallBackTimeRemaining[i] - TIM_gf_dt;
            if (ai->instanceCallBackTimeRemaining[i] < 0.0f)
                ai->instanceCallBackTimeRemaining[i] = 0.0f;
        }
        if (ai->instanceCallBackTimeRemaining[i] <= 0.0f && ai->instanceCallBacks[i])
        {
            ret = AI_pst_RunFunction(ai->instanceCallBacks[i], &ai->instanceJumpNodes[i], 0);
            if (ret != (tAINode*)6 && ret && ai->instanceCallBacks[i])
            {
                if (ai->instanceCallBacks[i]->funcPtr)
                    *AI_gppst_CurrentJumpNode = (tAINode*)-1;
                else
                    *AI_gppst_CurrentJumpNode = ai->instanceCallBacks[i]->nodes;
            }
            if (ret == (tAINode*)6)
                return 1;
            if (ret == (tAINode*)1)
                return 1;
            if (ret == (tAINode*)3)
                return 0;
            if (ret == (tAINode*)5)
                --i;
        }
    }
    return 1;
}

void AI_MainCall(struct sWorld* world)
{
    BOOL reCheckCurrElem;
    tGameObjectMain* currGao;
    tableElemStructPF* currElem, *lastElem;

    EVE_gpst_CurrentTrack = NULL;

    currElem = world->eot.AICallbacksTablePF.firstElemPtr;
    lastElem = ((tableElemStructPF*)(world->eot.AICallbacksTablePF.nextElemPtr)) - 1;

    while (currElem <= lastElem)
    {
        currGao = (tGameObjectMain*)currElem->objectPtr;
        if (!TAB_TableElemIsInvalid(currElem->objectPtr) && !OBJ_b_HasBeenProcess(currGao, world->aiCallCount, 0x20) && 
            ((currGao->extended->extraFlag & 0x4) == 0 || currGao->distance))
        {
            reCheckCurrElem = FALSE;
            while (currGao->basic && currGao->basic->mtx && currGao->basic->mtx->refGao2 && !OBJ_b_HasBeenProcess(currGao->basic->mtx->refGao2, world->aiCallCount, 0x20))
            {
                currGao = currGao->basic->mtx->refGao2;
                reCheckCurrElem = TRUE;
            }
            if (reCheckCurrElem)
                --currElem;

            OBJ_SetProcess(currGao, world->aiCallCount, 0x20);

            if (currGao->extended && currGao->extended->AIInstance)
            {
                if ((currGao->flags2 & 0x200000) == 0 && ((currGao->flags & 0x4000) != 0 || (currGao->flags & 0x2) != 0) && (currGao->flags2 & 0x10000) == 0)
                {
                    AI_EvalFunc_GRPSectUpdateMe_C(currGao);
                }

                AI_gpst_CurrentUltra = currGao;
                AI_gpst_CurrentGameObject = currGao;

                if ((currGao->flags2 & 0x400000) == 0 && !AI_RunInstance(currGao->extended->AIInstance))
                    break;
            }
        }
        ++currElem;
    }
    ++world->aiCallCount;
}

void AI_ExecCallback(tGameObjectMain* gao, int id)
{
    int i;
    tGameObjectMain* gaoBackup;
    tAIFunction* aiFuncBackup;
    int globalStackSizeBackup;
    int localStackSizeBackup;
    tAINode** jumpNodeBackup;
    tAINode* jumpNode;
    tAI* aiBackup;
    int varStackSizeBackup;

    if (gao->extended)
    {
        if (gao->extended->AIInstance)
        {
            if ((gao->flags2 & 0x400000) == 0 && (gao->flags2 & 0x4000000) == 0)
            {
                if (gao->extended->AIInstance->model)
                {
                    if (gao->extended->AIInstance->model->vars->instanceFuncNum || gao->extended->AIInstance->instanceAICBListNum)
                    {
                        gaoBackup = AI_gpst_CurrentGameObject;
                        aiBackup = AI_gpst_CurrentInstance;
                        aiFuncBackup = AI_gpst_CurrentFunction;
                        jumpNodeBackup = AI_gppst_CurrentJumpNode;
                        localStackSizeBackup = AI_gul_SizeLocalStack;
                        globalStackSizeBackup = AI_gul_SizeGlobalStack;
                        varStackSizeBackup = AI_gul_SizeGlobalVarStack;
                        ENG_gb_AIRunning = TRUE;
                        if (ENG_AIProfilerEnabled)
                        {
                            if (!PRO_AI.insideCounter)
                                PRO_AI.startTick = TIM_GetTicksAsInt32();
                            ++PRO_AI.insideCounter;
                        }
                        QueryPerformanceCounter(&PerfCounterStruct[14]);
                        if (id < gao->extended->AIInstance->model->vars->instanceFuncNum && gao->extended->AIInstance->model->vars->instanceFuncs[id])
                        {
                            jumpNode = gao->extended->AIInstance->model->vars->instanceFuncs[id]->nodes;
                            AI_gul_SizeGlobalStack = 0;
                            AI_gul_SizeGlobalVarStack = 0;
                            AI_gul_SizeLocalStack = 0;
                            AI_gpst_CurrentUltra = gao;
                            AI_gpst_CurrentGameObject = gao;
                            AI_gpst_CurrentInstanceUltra = gao->extended->AIInstance;
                            AI_gpst_CurrentInstance = gao->extended->AIInstance;
                            AI_pst_RunFunction(gao->extended->AIInstance->model->vars->instanceFuncs[id], &jumpNode, 0);// itt meg van nézve ohgy a függvény bele van-e fordítva a binárisba, és ha igen csak átadja neki a futást; egyébként meg lefuttatja a kódot ha van
                        }
                        for (i = 0; i < gao->extended->AIInstance->instanceAICBListNum; i++)
                        {
                            if (gao->extended->AIInstance->instanceAICBList[i].functionData)
                            {
                                if (gao->extended->AIInstance->instanceAICBList[i].id == id)
                                {
                                    jumpNode = gao->extended->AIInstance->instanceAICBList[i].functionData->nodes;
                                    AI_gul_SizeGlobalStack = 0;
                                    AI_gul_SizeGlobalVarStack = 0;
                                    AI_gul_SizeLocalStack = 0;
                                    AI_gpst_CurrentUltra = gao->extended->AIInstance->instanceAICBList[i].gao;
                                    AI_gpst_CurrentGameObject = AI_gpst_CurrentUltra;
                                    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentUltra->extended->AIInstance;
                                    AI_gpst_CurrentInstance = AI_gpst_CurrentInstanceUltra;
                                    AI_pst_RunFunction(gao->extended->AIInstance->instanceAICBList[i].functionData, &jumpNode, 0);
                                }
                            }
                        }
                        ENG_gb_AIRunning = FALSE;
                        AI_gpst_CurrentUltra = gaoBackup;
                        AI_gpst_CurrentGameObject = gaoBackup;
                        AI_gpst_CurrentInstanceUltra = aiBackup;
                        AI_gpst_CurrentInstance = aiBackup;
                        AI_gpst_CurrentFunction = aiFuncBackup;
                        AI_gppst_CurrentJumpNode = jumpNodeBackup;
                        AI_gul_SizeLocalStack = localStackSizeBackup;
                        AI_gul_SizeGlobalStack = globalStackSizeBackup;
                        AI_gul_SizeGlobalVarStack = varStackSizeBackup;
                        PRO_QPC_RecordTimeDecRef(14);
                        if (ENG_AIProfilerEnabled)
                            PRO_RecordTimeDecRef(&PRO_AI);
                    }
                }
            }
        }
    }
}

void AI_ExecCallbackAll(struct sWorld* world, int callbackID)
{
    tableElemStructPF* currElem, *lastElem;

    if (ENG_gb_EngineRunning)
    {
        currElem = world->eot.AICallbacksTablePF.firstElemPtr;
        lastElem = ((tableElemStructPF*)(world->eot.AICallbacksTablePF.nextElemPtr)) - 1;

        while (currElem <= lastElem)
        {
            if (!TAB_TableElemIsInvalid(currElem->objectPtr))
                AI_ExecCallback((tGameObjectMain*)currElem->objectPtr, callbackID);
            ++currElem;
        }
    }
}

void AI_FreeModel(tAIModel* aiModel)
{
    unsigned int i;

    for (i = 0; i < aiModel->funcNum; i++)
        LOA_DeleteAddress((int)aiModel->funcs[i]);

    LOA_DeleteAddress((int)aiModel);
    AI_FreeVarDes(aiModel->vars);

    for (i = 0; i < aiModel->funcNum; i++)
    {
        if (aiModel->funcs[i])
        {
            if (!--aiModel->funcs[i]->usageCount)
            {
                if (aiModel->funcs[i]->nodes)
                    MEM_FreeAlign(aiModel->funcs[i]->nodes);
                if (aiModel->funcs[i]->localStringConstsData)
                    MEM_Free(aiModel->funcs[i]->localStringConstsData);
                if (aiModel->funcs[i])
                    MEM_Free(aiModel->funcs[i]);
            }
        }
    }
    if (aiModel->funcs)
        MEM_Free(aiModel->funcs);
    MEM_Free(aiModel);
}

void AI_FreeInstance(tAI* ai)
{
    if (ai && ai->model)
    {
        if (!--ai->model->instanceCnt)
            AI_FreeModel(ai->model);
        LOA_DeleteAddress((int)ai);
        if (ai->instanceAICBList)
            MEM_Free(ai->instanceAICBList);
        if (ai->varsInstance)
            AI_FreeVarDes(ai->varsInstance);
        if (ai->instanceVarDataInitial)
            MEM_Free(ai->instanceVarDataInitial);
        if (ai->instanceVarDataCurrent)
            MEM_Free(ai->instanceVarDataCurrent);
        MEM_Free(ai);
    }
}

void AI_DuplicateInstance(struct sGameObjectMain* srcGao, struct sGameObjectMain* destGao)
{
    tAI* srcAI, *newAI;
    tAIVars* newAIVars;

    if (srcGao->extended && srcGao->extended->AIInstance)
    {
        srcAI = srcGao->extended->AIInstance;
        newAI = (tAI*)MEM_p_Alloc(sizeof(tAI));
        memcpy(newAI, srcAI, sizeof(tAI));
        newAI->instanceAICBListNum = 0;
        newAI->instanceAICBList = NULL;
        destGao->extended->AIInstance = newAI;
        
        if (newAI->model->vars->varFullSizeToAlloc)
        {
            newAI->instanceVarDataCurrent = (byte*)MEM_p_Alloc(newAI->model->vars->varFullSizeToAlloc);
            newAI->instanceVarDataInitial = (byte*)MEM_p_Alloc(newAI->model->vars->varFullSizeToAlloc);
            if (srcAI->instanceVarDataInitial)
                memcpy(newAI->instanceVarDataInitial, srcAI->instanceVarDataInitial, newAI->model->vars->varFullSizeToAlloc);
        }

        if (srcAI->varsInstance)
        {
            newAIVars = (tAIVars*)MEM_p_Alloc(sizeof(tAIVars));
            memcpy(newAIVars, srcAI->varsInstance, sizeof(tAIVars));
            newAI->varsInstance = newAIVars;

            if (newAIVars->varFullSizeToAlloc)
            {
                newAIVars->varData = (byte*)MEM_p_Alloc(newAIVars->varFullSizeToAlloc);
                memcpy(newAIVars->varData, srcAI->varsInstance->varData, newAIVars->varFullSizeToAlloc);
            }
            if (newAIVars->varNum)
            {
                newAIVars->variables = (AIVarElem*)MEM_p_Alloc(sizeof(AIVarElem) * newAIVars->varNum);
                memcpy(newAIVars->variables, srcAI->varsInstance->variables, sizeof(AIVarElem) * newAIVars->varNum);
                newAIVars->varNames = (AIVarNameElem*)MEM_p_Alloc(sizeof(AIVarNameElem) * newAIVars->varNum);
                memcpy(newAIVars->varNames, srcAI->varsInstance->varNames, sizeof(AIVarNameElem) * newAIVars->varNum);
            }
        }

        ++newAI->model->instanceCnt;
        AI_ReinitInstance(newAI);
    }
}

unsigned int AI_ul_RealResolveOneRefObject(unsigned int a1)
{
    return AI_ul_RealResolveOneRef(40, a1);
}

unsigned int AI_ul_RealResolveOneRefFunction(unsigned int key)
{
    return AI_ul_RealResolveOneRef(39, key);
}

/* Undo what a sideloaded parse borrowed, whichever way the loader
   leaves. `speedMode` is what LOA_gb_SpeedMode held before; nothing
   is touched unless a file was actually substituted. */
static void AI_SideloadDone(char* sideData, int speedMode)
{
    if (sideData)
    {
        SIDE_Free(sideData);
        LOA_gb_SpeedMode = speedMode;
    }
}

tAIFunction* AI_ul_CallbackLoadFunction(uint32_t offset)
{
    tAINode* currNode;
    int currKey, size;
    unsigned int i;
    tAIFunction *aiFunc, *newAiFunc;
    unsigned int nodeSizeSum;
    unsigned int extraAiNodeSizeSum;
    int binarizedLocalDataSize; 
    unsigned int localDataNodeNum;
    char* rawData, *currOffset;
    char* sideData = NULL;
    int sideSize = 0, sideSpeedMode = 0;

    aiFunc = (tAIFunction*)LOA_ul_SearchAddress(offset);
    if (aiFunc == (tAIFunction*)-1)
    {
        newAiFunc = (tAIFunction*)MEM_p_Alloc(sizeof(tAIFunction));

        LOA_ul_FileTypeSize_36 += sizeof(tAIFunction);

        memset(newAiFunc, 0, sizeof(tAIFunction));
        newAiFunc->usageCount = 1;
        newAiFunc->key = LOA_ul_GetCurrentKey();
        
        /* Read from the .bf even when a sideloaded copy is going to
           replace it. In a binarized world BIG_pc_ReadFileTmp is what
           advances the stream, by the length the archive recorded, so
           skipping it would desynchronise everything loaded after. */
        rawData = BIG_pc_ReadFileTmp(offset, &size);
        currKey = LOA_ul_GetCurrentKey();

        sideData = SIDE_pc_ReadByKey(currKey, &sideSize);
        if (sideData)
        {
            rawData = sideData;
            size = sideSize;
            /* What was dropped in the folder is a standalone .ofc, not
               a binarized member, so every field is present. Speed
               mode skips the ones read with mode 2, which would be
               read past the end here; turn it off for this parse. */
            sideSpeedMode = LOA_gb_SpeedMode;
            LOA_gb_SpeedMode = 0;
        }

        currOffset = rawData;
        
        if (size)
        {
            newAiFunc->stackOffsetNum = LOA_ReadLong(&currOffset, 0, 1);
            DBG_OutputDebugString("Trying to load function with key: 0x%x\n", currKey);
            /* A key the sideload folder names runs from its nodes,
               whatever the transcription has for it. That is how a
               behaviour already written back to .ofc gets tested
               without editing AI2C_gat_fctdefs. */
            if (!SIDE_b_Overrides(currKey))
            {
                for (i = 0; i < AI2C_i_SizeofArray(); i++)
                {
                    if (currKey == AI2C_gat_fctdefs[i].key)
                    {
                        newAiFunc->funcPtr = AI2C_gat_fctdefs[i].func;
                        break;
                    }
                }
            }
            if (!newAiFunc->funcPtr) {
                DBG_OutputDebugString("...not compiled in\n", currKey);
            }
            else {
                DBG_OutputDebugString("...ok\n", currKey);
            }
            nodeSizeSum = LOA_ReadLong(&currOffset, 0, 1);
            newAiFunc->nodes = NULL;
            if (nodeSizeSum)
            {
                if (!newAiFunc->funcPtr || (!LOA_gb_SpeedMode || BIG_gi_ReadMode != 2))
                {
                    if (!newAiFunc->funcPtr)
                        newAiFunc->nodes = (tAINode*)MEM_p_AllocAligned(nodeSizeSum, 64);

                    currNode = newAiFunc->nodes;
                    for (i = 0; i < (nodeSizeSum / sizeof(tAINode)); i++) // an AINode is 8 bytes long
                    {
                        if (newAiFunc->funcPtr)
                            LOA_ReadAINode(&currOffset, 0, 2); 
                        else
                            LOA_ReadAINode(&currOffset, currNode++, 1);
                    }
                }
            }
            extraAiNodeSizeSum = LOA_ReadLong(&currOffset, 0, 2);
            if (extraAiNodeSizeSum)
            {
                for (i = 0; i < (extraAiNodeSizeSum / sizeof(tAINode)); i++)
                {
                    LOA_ReadAINode(&currOffset, 0, 2);
                }
            }
            binarizedLocalDataSize = LOA_ReadLong(&currOffset, 0, 1);
            newAiFunc->localStringConstsData = NULL;
            if (binarizedLocalDataSize)
            {
                newAiFunc->localStringConstsData = (char*)MEM_p_Alloc(binarizedLocalDataSize);
                LOA_ReadString(&currOffset, (char*)newAiFunc->localStringConstsData, binarizedLocalDataSize, 1);
            }
            if (!LOA_gb_SpeedMode && currOffset != &rawData[size])
            {
                localDataNodeNum = LOA_ReadLong(&currOffset, 0, 2);
                if (localDataNodeNum)
                {
                    for (i = 0; i < localDataNodeNum / sizeof(tAILocal); i++)
                        LOA_ReadAILocal(&currOffset, 0, 2);
                }
            }
            AI_SideloadDone(sideData, sideSpeedMode);
            return newAiFunc;
        }
        else
        {
            AI_SideloadDone(sideData, sideSpeedMode);
            return newAiFunc;
        }
    }
    else
    {
        ++aiFunc->usageCount;
        return aiFunc;
    }
}

tAIVars* AI_ul_CallbackLoadVars(uint32_t offset)
{
    unsigned int i, j;
    unsigned int len, size, arrayDimensionNum, arrayElemNum;
    int funcKey;
    char* currVarDataPtr, *currReadPtr, *varDataPtr;
    tAIVarUnk* varUnkData;
    unsigned int instanceFuncNum, variableDataSizeSum, varNamesSizeSum, varUnkSizeSum, variableDefaultValueSizeSum;
    tAIVars* aiVars;
    tMessage* msg;
    tTextGet* tg;
    char* sideData = NULL;
    int sideSize = 0, sideSpeedMode = 0;

    aiVars = (tAIVars*)MEM_p_Alloc(sizeof(tAIVars));

    LOA_ul_FileTypeSize_35 += sizeof(tAIVars);

    memset(aiVars, 0, sizeof(tAIVars));
    varDataPtr = BIG_pc_ReadFileTmp(offset, &size);

    sideData = SIDE_pc_ReadByKey(LOA_ul_GetCurrentKey(), &sideSize);
    if (sideData)
    {
        varDataPtr = sideData;
        size = sideSize;
        sideSpeedMode = LOA_gb_SpeedMode;
        LOA_gb_SpeedMode = 0;
    }

    if (!size)
    {
        AI_SideloadDone(sideData, sideSpeedMode);
        return aiVars;
    }

    currReadPtr = varDataPtr;

    variableDataSizeSum = LOA_ReadLong(&currReadPtr, 0, 1);
    aiVars->varNum = variableDataSizeSum / sizeof(AIVarElem);
    aiVars->variables = NULL;

    if (variableDataSizeSum)
    {
        aiVars->variables = (AIVarElem*)MEM_p_Alloc(variableDataSizeSum);;
        LOA_ul_FileTypeSize_35 += variableDataSizeSum;

        for (i = 0; i < aiVars->varNum; i++)
        {
            aiVars->variables[i].varOffset = LOA_ReadLong(&currReadPtr, 0, 1);
            aiVars->variables[i].variableArrayCount = LOA_ReadLong(&currReadPtr, 0, 1);
            aiVars->variables[i].variableType = LOA_ReadShort(&currReadPtr, 0, 1);
            aiVars->variables[i].flags = LOA_ReadShort(&currReadPtr, 0, 1);
        }
    }

    varNamesSizeSum = LOA_ReadLong(&currReadPtr, 0, 1);
    aiVars->varNames = NULL;

    if (varNamesSizeSum)
    {
        aiVars->varNames = (AIVarNameElem*)MEM_p_Alloc(varNamesSizeSum);
        LOA_ul_FileTypeSize_35 += varNamesSizeSum;

        for (i = 0; i < aiVars->varNum; i++)
        {
            LOA_ReadString(&currReadPtr, aiVars->varNames[i].varName, struct_member_size(AIVarNameElem, varName), 1);
        }
    }

    varUnkSizeSum = LOA_ReadLong(&currReadPtr, 0, 2);
    LOA_ReadLong(&currReadPtr, 0, 2);
    varUnkData = NULL;

    if (varUnkSizeSum)
    {
        if (!LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
        {
            varUnkData = (tAIVarUnk*)MEM_p_AllocFromEnd(varUnkSizeSum);

            for (i = 0; i < (varUnkSizeSum / sizeof(tAIVarUnk)); i++)
            {
                LOA_ReadLong(&currReadPtr, 0, 2);
                varUnkData[i].i1 = LOA_ReadLong(&currReadPtr, 0, 2);
                varUnkData[i].i2 = LOA_ReadLong(&currReadPtr, 0, 2);
                LOA_ReadShort(&currReadPtr, 0, 2);
                LOA_ReadShort(&currReadPtr, 0, 2);
                LOA_ReadLong(&currReadPtr, 0, 2);
            }

            for (i = 0; i < (varUnkSizeSum / sizeof(tAIVarUnk)); i++)
            {
                if (varUnkData[i].i1)
                {
                    len = strlen(currReadPtr) + 1;
                    LOA_ReadString(&currReadPtr, 0, len, 2);
                }
                if (varUnkData[i].i2)
                {
                    len = strlen(currReadPtr) + 1;
                    LOA_ReadString(&currReadPtr, 0, len, 2);
                }
            }
        }
    }
    if (varUnkData)
        MEM_FreeFromEnd(varUnkData);

    variableDefaultValueSizeSum = LOA_ReadLong(&currReadPtr, 0, 1);
    aiVars->varFullSizeToAlloc = variableDefaultValueSizeSum;
    aiVars->varData = NULL;
    
    if (variableDefaultValueSizeSum)
    {
        currVarDataPtr = (char*)MEM_p_Alloc(variableDefaultValueSizeSum);
        aiVars->varData = (byte*)currVarDataPtr;
        
        LOA_ul_FileTypeSize_35 += variableDefaultValueSizeSum;

        for (i = 0; i < aiVars->varNum; ++i)
        {
            if (aiVars->variables[i].variableArrayCount != 1)
            {
                arrayDimensionNum = (aiVars->variables[i].variableArrayCount >> 30) & 0xff;
                for (j = 0; j < arrayDimensionNum; j++)
                {
                    *(int32_t*)currVarDataPtr = LOA_ReadLong(&currReadPtr, 0, 1);
                    currVarDataPtr += 4;
                }
            }

            arrayElemNum = aiVars->variables[i].variableArrayCount & 0x3fffffff;
            
            if (AI_gast_Types[AI_gaw_EnumLink[aiVars->variables[i].variableType]].dataTypeSize == 4)
            {
                for (j = 0; j < arrayElemNum; j++)
                {
                    LOA_ReadLong(&currReadPtr, (uint32_t*)currVarDataPtr, 1);
                    currVarDataPtr += 4;
                }
            }
            else
            {
                switch ((int)aiVars->variables[i].variableType)
                {
                case 37:
                    for (j = 0; j < arrayElemNum; j++)
                    {
                        LOA_ReadVector(&currReadPtr, (Vec3*)currVarDataPtr, 1);
                        currVarDataPtr += sizeof(Vec3);
                    }
                    break;
                case 41:
                    for (j = 0; j < arrayElemNum; j++)
                    {
                        msg = (tMessage*)currVarDataPtr;
                        msg->gao = (tGameObjectMain*)LOA_ReadLong(&currReadPtr, 0, 1);
                        msg->gao2 = (tGameObjectMain*)LOA_ReadLong(&currReadPtr, 0, 1);
                        LOA_ReadVector(&currReadPtr, &msg->v1, 1);
                        LOA_ReadVector(&currReadPtr, &msg->v2, 1);
                        msg->f2 = LOA_ReadFloat(&currReadPtr, 0, 1);
                        msg->f1 = LOA_ReadFloat(&currReadPtr, 0, 1);
                        msg->Int1 = LOA_ReadLong(&currReadPtr, 0, 1);
                        msg->Int2 = LOA_ReadLong(&currReadPtr, 0, 1);
                        msg->msgID = LOA_ReadLong(&currReadPtr, 0, 1);
                        currVarDataPtr += sizeof(tMessage);
                    }
                    break;
                case 44:
                    for (j = 0; j < arrayElemNum; j++)
                    {
                        tg = (tTextGet*)currVarDataPtr;
                        tg->key = LOA_ReadLong(&currReadPtr, 0, 1);
                        tg->idx = LOA_ReadLong(&currReadPtr, 0, 1);
                        currVarDataPtr += sizeof(tTextGet);
                    }
                    break;
                }
            }
        }
    }

    for (i = 0; i < 5; i++)
    {
        funcKey = LOA_ReadLong(&currReadPtr, 0, 1);
        if (funcKey)
            LOA_MakeFileRef(funcKey, &aiVars->callBacks[i], AI_ul_CallbackLoadFunction, 1);
    }

    aiVars->instanceFuncNum = 0;
    aiVars->instanceFuncs = NULL;

    if (currReadPtr != &varDataPtr[size])
    {
        instanceFuncNum = LOA_ReadLong(&currReadPtr, 0, 1);
        if (instanceFuncNum)
        {
            aiVars->instanceFuncNum = instanceFuncNum;
            aiVars->instanceFuncs = (tAIFunction**)MEM_p_Alloc(sizeof(tAIFunction**) * instanceFuncNum);

            LOA_ul_FileTypeSize_35 += sizeof(tAIFunction**) * instanceFuncNum;

            for (i = 0; i < instanceFuncNum; i++)
            {
                funcKey = LOA_ReadLong(&currReadPtr, 0, 1);
                if (funcKey)
                    LOA_MakeFileRef(funcKey, &aiVars->instanceFuncs[i], AI_ul_CallbackLoadFunction, 1);
                else
                    aiVars->instanceFuncs[i] = NULL;
            }
        }
    }
    AI_SideloadDone(sideData, sideSpeedMode);
    return aiVars;
}

tAIModel* AI_ul_CallbackLoadModel(uint32_t offset)
{
    char typeIdentifier[68];
    int idx;
    int funcNum;
    char* readPtr; 
    tAIModel* aiModel; 
    int size;
    unsigned int i;
    int key;
    char* currReadPtr;
    char* sideData = NULL;
    int sideSize = 0, sideSpeedMode = 0;

    aiModel = (tAIModel*)MEM_p_Alloc(sizeof(tAIModel));
    LOA_ul_FileTypeSize_36 += sizeof(tAIModel);
    
    memset(aiModel, 0, sizeof(tAIModel));
    readPtr = BIG_pc_ReadFileTmp(offset, &size);

    sideData = SIDE_pc_ReadByKey(LOA_ul_GetCurrentKey(), &sideSize);
    if (sideData)
    {
        readPtr = sideData;
        size = sideSize;
        sideSpeedMode = LOA_gb_SpeedMode;
        LOA_gb_SpeedMode = 0;
    }
    
    currReadPtr = readPtr;
    funcNum = 0;

    for (i = 0; i < (unsigned int)size >> 3; ++i)
    {
        LOA_ReadLong(&currReadPtr, 0, 4);
        LOA_ReadCharArray(&currReadPtr, typeIdentifier, 4, 4);
        typeIdentifier[4] = '\0'; // to be able to use stricmp
        if (!_stricmp(typeIdentifier, ".ofc"))        
            ++funcNum;
    }
    
    currReadPtr = readPtr;
    
    if (funcNum)
    {
        aiModel->funcs = (tAIFunction**)MEM_p_Alloc(4 * funcNum);
        LOA_ul_FileTypeSize_36 += 4 * funcNum;
    }
    
    aiModel->funcNum = funcNum;
    idx = 0;

    for (i = 0; i < (unsigned int)size / 8; i++) // ([key][typeid]) = 8 bytes
    {
        key = LOA_ReadLong(&currReadPtr, 0, 1);
        LOA_ReadCharArray(&currReadPtr, typeIdentifier, 4, 1);

        if (key)
        {
            typeIdentifier[4] = '\0';
            if (!_stricmp(typeIdentifier, ".ova"))
            {
                LOA_MakeFileRef(key, &(aiModel->vars), AI_ul_CallbackLoadVars, 1);
            }
            else if (!_stricmp(typeIdentifier, ".ofc"))
            {
                LOA_MakeFileRef(key, &(aiModel->funcs[idx++]), AI_ul_CallbackLoadFunction, 1);
            }
        }
    }
    AI_SideloadDone(sideData, sideSpeedMode);
    return aiModel;
}

struct sAI* AI_ul_CallbackLoadInstance(uint32_t offset)
{
    char* readPtr; 
    tAI* aiPtr;
    int size; 
    int key; 
    char* sideData = NULL;
    int sideSize = 0, sideSpeedMode = 0;

    aiPtr = (tAI*)MEM_p_Alloc(sizeof(tAI));
    LOA_ul_FileTypeSize_36 += sizeof(tAI);

    memset(aiPtr, 0, sizeof(tAI));

    readPtr = BIG_pc_ReadFileTmp(offset, &size);

    sideData = SIDE_pc_ReadByKey(LOA_ul_GetCurrentKey(), &sideSize);
    if (sideData)
    {
        readPtr = sideData;
        size = sideSize;
        sideSpeedMode = LOA_gb_SpeedMode;
        LOA_gb_SpeedMode = 0;
    }

    key = LOA_ReadLong(&readPtr, 0, 1);
    if (key && key != -1)
        LOA_MakeFileRef(key, &(aiPtr->model), AI_ul_CallbackLoadModel, 1);
    key = LOA_ReadLong(&readPtr, 0, 1);
    if (key && key != -1)
        LOA_MakeFileRef(key, &(aiPtr->varsInstance), AI_ul_CallbackLoadVars, 9);

    AI_SideloadDone(sideData, sideSpeedMode);
    return aiPtr;
}

void AI_AllocateSaveBuffer(tAI* aiPtr)
{
    unsigned int i, sizeToAlloc;

    if (aiPtr->model && aiPtr->model->vars)
    {
        sizeToAlloc = 0;
        for (i = 0; i < aiPtr->model->vars->varNum; i++)
        {
            if ((aiPtr->model->vars->variables[i].flags & 0x10) != 0)
            {
                sizeToAlloc += AI_gast_Types[aiPtr->model->vars->variables[i].variableType].dataTypeSize * 
                    (aiPtr->model->vars->variables[i].variableArrayCount & 0x3fffffff) +
                    4 * (aiPtr->model->vars->variables[i].variableArrayCount >> 30);
            }
        }
        if (AI_gpc_SaveBuffer)
        {
            MEM_Free(AI_gpc_SaveBuffer);
            AI_gpc_SaveBuffer = NULL;
            AI_gi_SaveBufferSize = 0;
        }
        if (sizeToAlloc)
        {
            AI_gpc_SaveBuffer = (byte*)MEM_p_Alloc(sizeToAlloc);
            AI_gi_SaveBufferSize = sizeToAlloc;
        }
    }
}

struct sAI* AI_pst_LoadInstance(int key)
{
    tAI* aiPtr;

    aiPtr = NULL;
    MEM_gb_EnableTmp = TRUE;
    LOA_MakeFileRef(key, &aiPtr, AI_ul_CallbackLoadInstance, 1);
    LOA_Resolve();
    MEM_gb_EnableTmp = FALSE;
    if (!aiPtr)
        return NULL;
    AI_ResolveInstanceRef(aiPtr);
    AI_ReinitInstance(aiPtr);
    if (key == BIG_gst.hdr.startKey)
        AI_AllocateSaveBuffer(aiPtr);
    return aiPtr;
}

void AI_ResolveEnums()
{
    int i;      

    for (i = 0; i < (sizeof(AI_gast_Categs) / sizeof(AI_gast_Categs[0])); ++i)
        AI_gaw_EnumLink[AI_gast_Categs[i].funcid] = i;
    for (i = 0; i < (sizeof(AI_gast_Types) / sizeof(AI_gast_Types[0])); ++i)
        AI_gaw_EnumLink[AI_gast_Types[i].funcid] = i;
    for (i = 0; i < (sizeof(AI_gast_Keywords) / sizeof(AI_gast_Keywords[0])); ++i)
        AI_gaw_EnumLink[AI_gast_Keywords[i].funcid] = i;
    for (i = 0; i < (sizeof(AI_gast_Functions) / sizeof(AI_gast_Functions[0])); ++i)
        AI_gaw_EnumLink[AI_gast_Functions[i].funcid] = i;
    for (i = 0; i < (sizeof(AI_gast_Fields) / sizeof(AI_gast_Fields[0])); ++i)
        AI_gaw_EnumLink[AI_gast_Fields[i].funcid] = i;
}

void AI_InitModule()
{
    AI_ResolveEnums();
    memset(AI_MainActor, 0, sizeof(tGameObjectMain*));
}

void AI2C_ResolveBuffer(uint32_t* varPtr, void* gaoOrGaoIdxOrAIPtr, int varOffset)
{
    tAI* aiPtr;
    tGameObjectMain* gao;

    if (gaoOrGaoIdxOrAIPtr == WOR_gst_Universe.univAIInstance)
    {
        aiPtr = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        gao = (tGameObjectMain*)gaoOrGaoIdxOrAIPtr;
        if (gaoOrGaoIdxOrAIPtr)
        {
            if ((unsigned int)gaoOrGaoIdxOrAIPtr <= 2)
                gao = AI_MainActor[(uint32_t)gaoOrGaoIdxOrAIPtr - 1];
        }
        else
        {
            gao = NULL;
        }
        aiPtr = gao->extended->AIInstance;
    }
    *varPtr = (uint32_t)&aiPtr->instanceVarDataCurrent[varOffset];
}

void* AI2C_ResolveBufferPtr(void* gaoOrGaoIdxOrAIPtr)
{
    tAI* aiPtr;
    tGameObjectMain* gao;

    if (gaoOrGaoIdxOrAIPtr == WOR_gst_Universe.univAIInstance)
    {
        aiPtr = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        gao = (tGameObjectMain*)gaoOrGaoIdxOrAIPtr;
        if (gaoOrGaoIdxOrAIPtr)
        {
            if ((unsigned int)gaoOrGaoIdxOrAIPtr <= 2)
                gao = AI_MainActor[(uint32_t)gaoOrGaoIdxOrAIPtr - 1];
        }
        else
        {
            gao = NULL;
        }
        aiPtr = gao->extended->AIInstance;
    }
    
    return aiPtr->instanceVarDataCurrent;
}

void* AI2C_VarGetFromAI(struct sAI* aiPtr, char* name) {
    unsigned int i;
    if (!aiPtr->model || !aiPtr->model->vars || !aiPtr->model->vars->varNames || !aiPtr->model->vars->variables)
        return NULL;

    for (i = 0; i < aiPtr->model->vars->varNum; i++) {
        if (!_strcmpi(aiPtr->model->vars->varNames[i].varName, name)) {
            return (void*)&aiPtr->instanceVarDataCurrent[aiPtr->model->vars->variables[i].varOffset];
        }
    }

    DBG_OutputDebugString("AI: Warning: Global variable used in script function (\"%s\") is not found.\n", name);
    return NULL;
}

void* AI2C_VarArrayGetFromAI(struct sAI* aiPtr, char* name, int idx) {
    unsigned int i;
    if (!aiPtr->model || !aiPtr->model->vars || !aiPtr->model->vars->varNames || !aiPtr->model->vars->variables)
        return NULL;

    for (i = 0; i < aiPtr->model->vars->varNum; i++) {
        if (!_strcmpi(aiPtr->model->vars->varNames[i].varName, name)) {
            return (void*)AI_EvalVarArray_C(aiPtr->model->vars->variables[i].variableType, &aiPtr->instanceVarDataCurrent[aiPtr->model->vars->variables[i].varOffset], idx);
        }
    }

    DBG_OutputDebugString("AI: Warning: Global variable used in script function (\"%s\") is not found.\n", name);
    return NULL;
}

void* AI2C_VarArrayC3GetFromAI(struct sAI* aiPtr, char* name, int idx) {
    unsigned int i;
    if (!aiPtr->model || !aiPtr->model->vars || !aiPtr->model->vars->varNames || !aiPtr->model->vars->variables)
        return NULL;

    for (i = 0; i < aiPtr->model->vars->varNum; i++) {
        if (!_strcmpi(aiPtr->model->vars->varNames[i].varName, name)) {
            return (void*)AI_EvalVarArray_C3(&aiPtr->instanceVarDataCurrent[aiPtr->model->vars->variables[i].varOffset], idx);
        }
    }

    DBG_OutputDebugString("AI: Warning: Global variable used in script function (\"%s\") is not found.\n", name);
    return NULL;
}

struct sGameObjectMain* AI2C_CheckActorGao(struct sGameObjectMain* gao) { // TODO:FIXME UA mint AI_EvalType_GetGAO
    if (gao)
    {
        if ((unsigned int)gao <= 2)
            return AI_MainActor[(uint32_t)gao - 1];
    }
    return gao;
}

struct sGameObjectMain* AI2C_GaoGetFromAI(struct sAI* aiPtr, char* name) {
    tGameObjectMain* ret = *(tGameObjectMain**)AI2C_VarGetFromAI(aiPtr, name);

    if (ret)
    {
        if ((unsigned int)ret <= 2)
            return AI_MainActor[(uint32_t)ret - 1];
    }
    return ret;
}

void AI2C_IntSetFromAI(struct sAI* aiPtr, char* name, int val) {
    *(int*)AI2C_VarGetFromAI(aiPtr, name) = val;
}

void AI2C_FloatSetFromAI(struct sAI* aiPtr, char* name, float val) {    
    *(float*)AI2C_VarGetFromAI(aiPtr, name) = val;    
}

void AI2C_VecSetFromAI(struct sAI* aiPtr, char* name, Vec3* val) {
    *(Vec3*)AI2C_VarGetFromAI(aiPtr, name) = *val;
}

void AI2C_GaoSetFromAI(struct sAI* aiPtr, char* name, struct sGameObjectMain* val) {
    *(tGameObjectMain**)AI2C_VarGetFromAI(aiPtr, name) = val;
}

void AI2C_TextGetSetFromAI(struct sAI* aiPtr, char* name, struct sTextGet* val) {
    *(tTextGet**)AI2C_VarGetFromAI(aiPtr, name) = val;
}

void AI2C_IntArraySetFromAI(struct sAI* aiPtr, char* name, int idx, int val) {
    *(int*)AI2C_VarArrayGetFromAI(aiPtr, name, idx) = val;
}

void AI2C_FloatArraySetFromAI(struct sAI* aiPtr, char* name, int idx, float val) {
    *(float*)AI2C_VarArrayGetFromAI(aiPtr, name, idx) = val;
}

struct sAI* AI2C_GetAIModelFromGao(void* gaoOrGaoIdxOrAIPtr) {
    tGameObjectMain* gao;

    if (gaoOrGaoIdxOrAIPtr == WOR_gst_Universe.univAIInstance)
    {
        return  WOR_gst_Universe.univAIInstance;
    }
    else
    {
        gao = (tGameObjectMain*)gaoOrGaoIdxOrAIPtr;
        if (gaoOrGaoIdxOrAIPtr)
        {
            if ((unsigned int)gaoOrGaoIdxOrAIPtr <= 2)
                gao = AI_MainActor[(uint32_t)gaoOrGaoIdxOrAIPtr - 1];
        }
        else
        {
            gao = NULL;
        }
        return gao->extended->AIInstance;
    }
}

// not in retail nor demo version
void AI2C_ResolveBufferByName(uint32_t* varPtr, void* gaoOrGaoIdxOrAIPtr, char* name) // osszevonni ezzel: AI2C_GetAIModelFromGao
{
    tAI* aiPtr;
    tGameObjectMain* gao;
    unsigned int i;

    if (gaoOrGaoIdxOrAIPtr == WOR_gst_Universe.univAIInstance)
    {
        aiPtr = WOR_gst_Universe.univAIInstance;
    }
    else
    {
        gao = (tGameObjectMain*)gaoOrGaoIdxOrAIPtr;
        if (gaoOrGaoIdxOrAIPtr)
        {
            if ((unsigned int)gaoOrGaoIdxOrAIPtr <= 2)
                gao = AI_MainActor[(uint32_t)gaoOrGaoIdxOrAIPtr - 1];
        }
        else
        {
            gao = NULL;
        }
        aiPtr = gao->extended->AIInstance;
    }

    if (!aiPtr->model || !aiPtr->model->vars || !aiPtr->model->vars->varNames || !aiPtr->model->vars->variables)
        return;

    for (i = 0; i < aiPtr->model->vars->varNum; i++) {
        if (!_strcmpi(aiPtr->model->vars->varNames[i].varName, name)) {
            *varPtr = (uint32_t)&aiPtr->instanceVarDataCurrent[aiPtr->model->vars->variables[i].varOffset];
            return;
        }
    }

    DBG_OutputDebugString("AI: Warning: Global variable used in script function (\"%s\") is not found.\n", name);
}

BOOL AI2C_VecEqNull(Vec3* v)
{
    if (fabs(v->x) != 0.0f)
        return FALSE;
    if (fabs(v->y) != 0.0f)
        return FALSE;
    return fabs(v->z) == 0.0f;
}

BOOL AI2C_DistOfTwoPointsIsZero(Vec3* v1, Vec3* v2)
{
    Vec3 dest;

    MATH_SubstractVec(&dest, v1, v2);

    if (fabs(dest.x) != 0.0f)
        return FALSE;
    if (fabs(dest.y) != 0.0f)
        return FALSE;
    return fabs(dest.z) == 0.0f;
}
