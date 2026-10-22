#include "../math.h"
#include "../gameobject.h"
#include "../time.h"
#include "ai_debugfunctions.h"
#include "../dbg.h"

void AI_EvalFunc_DbgTraceInt_C() {
}

void AI_EvalFunc_DbgLastRayGet_Dummy_C(struct sGameObjectMain* a1, Vec3* a2)
{
    *a2 = MATH_gst_NulVector;
}

tAINode* AI_EvalFunc_DbgTraceInt(tAINode* a1)
{
    int v = AI_GetIntFromStack();
    DBG_OutputDebugString("AI: %d (0x%x)\n", v, v);
    return a1 + 1;
}

tAINode* AI_EvalFunc_DbgTraceFloat(tAINode* a1)
{
    float v = AI_GetFloatFromStack();
    DBG_OutputDebugString("AI: %f\n", v);
    return a1 + 1;
}

tAINode* AI_EvalFunc_DbgTraceVector(tAINode* arg0)
{
    Vec3 a1;

    AI_GetVecFromStack(&a1);
    DBG_OutputDebugString("AI: (%f, %f, %f)\n", a1.x, a1.y, a1.z);
    return arg0 + 1;
}

tAINode* AI_EvalFunc_DbgTraceString(tAINode* a1)
{
    char* s = get_a_string_from_globalstack_sub_4D2040();
    DBG_OutputDebugString("AI: %s\n", s ? s : "(null)");
    return a1 + 1;
}

tAINode* AI_EvalFunc_DbgBreakPoint(tAINode* a1)
{
    a1[1].flags |= 2u;
    return a1 + 1;
}

tAINode* AI_EvalFunc_DbgTraceChar(tAINode* a1)
{
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_DbgTraceInt_C();
    return a1 + 1;
}

tAINode* AI_EvalFunc_DbgTraceObject(tAINode* a1)
{
    tGameObjectMain* o;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    o = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    DBG_OutputDebugString("AI: object %p\n", (void*)o);
    return a1 + 1;
}

tAINode* AI_EvalFunc_DbgRenderVector(tAINode* arg0)
{
    tGameObjectMain* v2;
    Vec3 a1;
    Vec3 v4;
    int v5;
    tGameObjectMain* x;

    v5 = AI_GetIntFromStack();
    AI_GetVecFromStack(&a1);
    AI_GetVecFromStack(&v4);
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    x = v2;
    AI_EvalFunc_DbgTraceInt_C();
    return arg0 + 1;
}

tAINode* AI_EvalFunc_DbgEraseAllVectors(tAINode* a1)
{
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_DbgTraceInt_C();
    return a1 + 1;
}

tAINode* AI_EvalFunc_DbgLastRayGet(tAINode* arg0)
{
    tGameObjectMain* v2;
    Vec3 a2;
    tGameObjectMain* a1;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1 = v2;
    AI_EvalFunc_DbgLastRayGet_Dummy_C(v2, &a2);
    AI_PutVecToStack(&a2);
    return arg0 + 1;
}

tAINode* AI_EvalFunc_DbgClearLog(tAINode* a1)
{
    AI_EvalFunc_DbgTraceInt_C();
    return a1 + 1;
}

tAINode* AI_EvalFunc_DbgStartRaster(tAINode* a1)
{
    profilerRasterStructElem* v2;
    unsigned int v3;
    char* v4;

    v4 = get_a_string_from_globalstack_sub_4D2040();
    v3 = AI_GetIntFromStack();
    if (v3 < 10)
    {
        PRO_RasterGroup.PRO_UserProfilerRasters[v3].insideCounter = 0;
        PRO_RasterGroup.PRO_UserProfilerRasters[v3].module = v4;
        v2 = &PRO_RasterGroup.PRO_UserProfilerRasters[v3];
        if (!v2->insideCounter)
            v2->startTick = TIM_GetTicksAsInt32();
        ++v2->insideCounter;
    }
    return a1 + 1;
}

tAINode* AI_EvalFunc_DbgStopRaster(tAINode* a1)
{
    unsigned int v2;

    v2 = AI_GetIntFromStack();
    if (v2 < 10)
        PRO_RecordTimeDecRef(&PRO_RasterGroup.PRO_UserProfilerRasters[v2]);
    return a1 + 1;
}

tAINode* AI_EvalFunc_DbgRenderCircle(tAINode* arg0)
{
    tGameObjectMain* v2;
    Vec3 v3;
    Vec3 a1;
    tGameObjectMain* x;

    AI_GetIntFromStack();
    AI_GetVecFromStack(&a1);
    AI_GetFloatFromStack();
    AI_GetVecFromStack(&v3);
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    x = v2;
    AI_EvalFunc_DbgTraceInt_C();
    return arg0 + 1;
}

tAINode* AI_EvalFunc_DbgVarsCurToInit(tAINode* aiNode)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    if (!gaoUltra->extended)
        return aiNode + 1;
    if (!gaoUltra->extended->AIInstance)
        return aiNode + 1;
    if (!gaoUltra->extended->AIInstance->instanceVarDataInitial)
        return aiNode + 1;
    if (!gaoUltra->extended->AIInstance->model)
        return aiNode + 1;
    if (!gaoUltra->extended->AIInstance->model->vars)
        return aiNode + 1;
    
    if (gaoUltra->extended->AIInstance->model->vars->varFullSizeToAlloc)
        memcpy(gaoUltra->extended->AIInstance->instanceVarDataInitial, gaoUltra->extended->AIInstance->instanceVarDataCurrent, gaoUltra->extended->AIInstance->model->vars->varFullSizeToAlloc);
    
    return aiNode + 1;
}
