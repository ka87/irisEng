#include "../math.h"
#include "../gameobject.h"
#include "../time.h"
#include "ai_debugfunctions.h"
#include "../dbg.h"

void AI_EvalFunc_DbgTraceInt_C() {
}

void AI_EvalFunc_DbgLastRayGet_Dummy_C(struct sGameObjectMain* gao, Vec3* ray)
{
    *ray = MATH_gst_NulVector;
}

tAINode* AI_EvalFunc_DbgTraceInt(tAINode* node)
{
    int v = AI_GetIntFromStack();
    DBG_OutputDebugString("AI: %d (0x%x)\n", v, v);
    return node + 1;
}

tAINode* AI_EvalFunc_DbgTraceFloat(tAINode* node)
{
    float v = AI_GetFloatFromStack();
    DBG_OutputDebugString("AI: %f\n", v);
    return node + 1;
}

tAINode* AI_EvalFunc_DbgTraceVector(tAINode* node)
{
    Vec3 traced;

    AI_GetVecFromStack(&traced);
    DBG_OutputDebugString("AI: (%f, %f, %f)\n", traced.x, traced.y, traced.z);
    return node + 1;
}

tAINode* AI_EvalFunc_DbgTraceString(tAINode* node)
{
    char* s = get_a_string_from_globalstack_sub_4D2040();
    DBG_OutputDebugString("AI: %s\n", s ? s : "(null)");
    return node + 1;
}

tAINode* AI_EvalFunc_DbgBreakPoint(tAINode* node)
{
    node[1].flags |= 2u;
    return node + 1;
}

tAINode* AI_EvalFunc_DbgTraceChar(tAINode* node)
{
    AI_GetObjectFromStack();
    AI_EvalFunc_DbgTraceInt_C();
    return node + 1;
}

tAINode* AI_EvalFunc_DbgTraceObject(tAINode* node)
{
    tGameObjectMain* traced;

    traced = (tGameObjectMain*)AI_GetObjectFromStack();
    DBG_OutputDebugString("AI: object %p\n", (void*)traced);
    return node + 1;
}

// the debug renderer is a stub, so the arguments are read off the stack
// and dropped; the stack has to be unwound either way
tAINode* AI_EvalFunc_DbgRenderVector(tAINode* node)
{
    Vec3 to;
    Vec3 from;

    AI_GetIntFromStack();
    AI_GetVecFromStack(&to);
    AI_GetVecFromStack(&from);

    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    AI_EvalFunc_DbgTraceInt_C();
    return node + 1;
}

tAINode* AI_EvalFunc_DbgEraseAllVectors(tAINode* node)
{
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_DbgTraceInt_C();
    return node + 1;
}

tAINode* AI_EvalFunc_DbgLastRayGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 ray;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    AI_EvalFunc_DbgLastRayGet_Dummy_C(gaoUltra, &ray);
    AI_PutVecToStack(&ray);
    return node + 1;
}

tAINode* AI_EvalFunc_DbgClearLog(tAINode* node)
{
    AI_EvalFunc_DbgTraceInt_C();
    return node + 1;
}

tAINode* AI_EvalFunc_DbgStartRaster(tAINode* node)
{
    profilerRasterStructElem* raster;
    unsigned int index;
    char* name;

    name = get_a_string_from_globalstack_sub_4D2040();
    index = AI_GetIntFromStack();
    if (index >= 10)
        return node + 1;

    raster = &PRO_RasterGroup.PRO_UserProfilerRasters[index];
    // the counter is cleared before it is tested, so a raster the
    // scripts start twice over restarts rather than nesting
    raster->insideCounter = 0;
    raster->module = name;
    if (!raster->insideCounter)
        raster->startTick = TIM_GetTicksAsInt32();
    ++raster->insideCounter;
    return node + 1;
}

tAINode* AI_EvalFunc_DbgStopRaster(tAINode* node)
{
    unsigned int index;

    index = AI_GetIntFromStack();
    if (index < 10)
        PRO_RecordTimeDecRef(&PRO_RasterGroup.PRO_UserProfilerRasters[index]);
    return node + 1;
}

// stubbed like the vector renderer above
tAINode* AI_EvalFunc_DbgRenderCircle(tAINode* node)
{
    Vec3 axis;
    Vec3 centre;

    AI_GetIntFromStack();
    AI_GetVecFromStack(&axis);
    AI_GetFloatFromStack();
    AI_GetVecFromStack(&centre);

    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    AI_EvalFunc_DbgTraceInt_C();
    return node + 1;
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
