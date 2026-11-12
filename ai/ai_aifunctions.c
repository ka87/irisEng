#include "../ai.h"
#include "../memory.h"
#include "../engine.h"
#include "ai_aifunctions.h"

// A numeric argument that may arrive as either type. The value is
// converted through its own type function first, then read as a float,
// which means converting from int for every type but float itself.
// This is not AI_GetFloatFromStack, which skips the type function and
// converts only for the int type.
static float AI_l_PopNumberAsFloat(void)
{
    char converted[52];
    tVarStackElem var;

    AI_GetVarFromStack(converted, &var);
    if (var.type != 2)
        return (float)*(int32_t*)converted;
    return *(float*)converted;
}

// The same the other way round: read as an int, rounded through the
// magic double when it arrived as a float.
static int AI_l_PopNumberAsInt(void)
{
    char converted[52];
    tVarStackElem var;
    double rounded;

    AI_GetVarFromStack(converted, &var);
    if (var.type != 2)
        return *(int32_t*)converted;

    rounded = *(float*)converted + MATH_gd_magicDoubleForRound;
    return *(int32_t*)&rounded;
}

// A behaviour reference arrives as a variable whose converted value is
// the function pointer.
static tAIFunction* AI_l_PopFunction(void)
{
    char converted[52];
    tVarStackElem var;

    AI_GetVarFromStack(converted, &var);
    return *(tAIFunction**)converted;
}

// Where a behaviour runs, a function with no nodes but a C
// implementation gets the -1 sentinel in place of an entry node.
static tAINode* AI_l_EntryNode(tAIFunction* func)
{
    if (func->nodes)
        return func->nodes;
    if (func->funcPtr)
        return (tAINode*)-1;
    return NULL;
}

// A builtin that can end the evaluation returns one of the
// interpreter's control codes in place of a node; anything else means
// carry on with the next node.
static tAINode* AI_l_ControlCode(int code, tAINode* node)
{
    if (code == 1 || code == 3 || code == 4 || code == 5)
        return (tAINode*)code;
    return node + 1;
}

// Clearing a track also narrows the first/last range the interpreter
// walks. The second test deliberately sees the value the first loop
// left behind.
static void AI_l_StopTrack(tAI* instance, unsigned int track)
{
    instance->instanceCallBacks[track] = 0;
    instance->instanceJumpNodes[track] = 0;

    if (track == instance->trackFirst)
    {
        while (track < 5 && !instance->instanceCallBacks[track])
            ++track;
        instance->trackFirst = track;
    }
    if (track == instance->trackLast)
    {
        while (track && !instance->instanceCallBacks[track])
            --track;
        instance->trackLast = track;
    }
}

void AI_EvalFunc_CastVector_C(float x, float y, float z, Vec3* result)
{
    result->x = x;
    result->y = y;
    result->z = z;
}

tAINode* AI_EvalFunc_CastVector(tAINode* node)
{
    Vec3 result;
    float x;
    float y;
    float z;

    z = AI_l_PopNumberAsFloat();
    y = AI_l_PopNumberAsFloat();
    x = AI_l_PopNumberAsFloat();
    AI_EvalFunc_CastVector_C(x, y, z, &result);
    AI_PutVecToStack(&result);
    return node + 1;
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
        jumpNode = AI_l_EntryNode(funcPtr);
        ++AI_gi_RunFctLevel;
        jumpNode = AI_pst_RunFunction(funcPtr, &jumpNode, AI_gpst_CurrentFunction->stackOffsetNum);
        --AI_gi_RunFctLevel;
    }
    AI_gpst_CurrentFunction = currentFunctionBackup;
    AI_gppst_CurrentJumpNode = currentJumpNodeBackup;

    switch ((uintptr_t)jumpNode)
    {
    case 1:
        return 1;
    case 3:
        return 3;
    case 4:
        return 4;
    case 5:
        return 5;
    default:
        return 1000;
    }
}

tAINode* AI_EvalFunc_AIExecute(tAINode* node)
{
    return AI_l_ControlCode(AI_EvalFunc_AIExecute_C(AI_l_PopFunction()), node);
}

// Returns the track range's old upper bound, which no caller reads.
int AI_EvalFunc_AITrackChange_C(int track, struct sAIFunction* func)
{
    tAI* instance;
    int previousLast;

    instance = AI_gpst_CurrentInstanceUltra;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;

    if (instance->instanceCallBacks[track] != func)
    {
        instance->instanceCallBacks[track] = func;
        if (!func->nodes && func->funcPtr)
            instance->instanceCallBackTimeRemaining[track] = 0.0;
        instance->instanceJumpNodes[track] = AI_l_EntryNode(func);
    }

    if (track < instance->trackFirst)
        instance->trackFirst = track;
    previousLast = instance->trackLast;
    if (track > previousLast)
        instance->trackLast = track;
    return previousLast;
}

tAINode* AI_EvalFunc_AITrackChange(tAINode* node)
{
    tAIFunction* func;
    int track;

    func = AI_l_PopFunction();
    track = AI_l_PopNumberAsInt();
    AI_EvalFunc_AITrackChange_C(track, func);
    return node + 1;
}

void AI_EvalFunc_AITrackStop_C(int track)
{
    tAI* instance;

    instance = AI_gpst_CurrentInstanceUltra;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_l_StopTrack(instance, track);
}

tAINode* AI_EvalFunc_AITrackStop(tAINode* node)
{
    int track;

    track = AI_l_PopNumberAsInt();
    AI_EvalFunc_AITrackStop_C(track);
    return node + 1;
}

void AI_EvalFunc_AITrackCurChange_C(struct sAIFunction* func)
{
    tAI* instance;

    instance = AI_gpst_CurrentInstance;
    if (instance->instanceCallBacks[AI_gul_CurrentTrack] == func)
        return;

    instance->instanceCallBacks[AI_gul_CurrentTrack] = func;
    instance->instanceJumpNodes[AI_gul_CurrentTrack] = AI_l_EntryNode(func);
}

tAINode* AI_EvalFunc_AITrackCurChange(tAINode* node)
{
    AI_EvalFunc_AITrackCurChange_C(AI_l_PopFunction());
    return node + 1;
}

void AI_EvalFunc_AITrackCurStop_C()
{
    AI_l_StopTrack(AI_gpst_CurrentInstance, AI_gul_CurrentTrack);
}

tAINode* AI_EvalFunc_AITrackCurStop(tAINode* node)
{
    AI_EvalFunc_AITrackCurStop_C();
    return node + 1;
}

// Switches the current track and ends the evaluation so the new
// behaviour runs in this frame rather than the next.
int AI_EvalFunc_AITrackCurChangeNow_C(struct sAIFunction* func)
{
    tAI* instance;

    if (AI_gpst_CurrentFunction == func)
        return 1000;

    AI_gpst_CurrentFunction = func;
    instance = AI_gpst_CurrentInstance;
    instance->instanceCallBacks[AI_gul_CurrentTrack] = func;
    instance->instanceJumpNodes[AI_gul_CurrentTrack] = AI_l_EntryNode(func);
    return 5;
}

tAINode* AI_EvalFunc_AITrackCurChangeNow(tAINode* node)
{
    return AI_l_ControlCode(AI_EvalFunc_AITrackCurChangeNow_C(AI_l_PopFunction()), node);
}

struct sAIFunction* AI_EvalFunc_AITrackGet_C(int track)
{
    return AI_gpst_CurrentInstance->instanceCallBacks[track];
}

tAINode* AI_EvalFunc_AITrackGet(tAINode* node)
{
    tAIFunction* func;
    int track;

    track = AI_GetIntFromStack();
    func = AI_EvalFunc_AITrackGet_C(track);
    AI_PutObjectToStack(&func, AI_gaw_EnumLink[39]);
    return node + 1;
}

struct sAIFunction* AI_EvalFunc_AITrackCurGet_C()
{
    return AI_gpst_CurrentInstance->instanceCallBacks[AI_gul_CurrentTrack];
}

tAINode* AI_EvalFunc_AITrackCurGet(tAINode* node)
{
    tAIFunction* func;

    func = AI_EvalFunc_AITrackCurGet_C();
    AI_PutObjectToStack(&func, AI_gaw_EnumLink[39]);
    return node + 1;
}

BOOL AI_EvalFunc_AIIsModel_C(struct sGameObjectMain* gao, struct sAIModel* model)
{
    if (!gao->extended || !gao->extended->AIInstance)
        return FALSE;
    return gao->extended->AIInstance->model == model;
}

tAINode* AI_EvalFunc_AIIsModel(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tAIModel* model;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    model = (tAIModel*)AI_GetObjectFromStack();
    AI_PutIntToStack(AI_EvalFunc_AIIsModel_C(gaoUltra, model));
    return node + 1;
}

int AI_EvalFunc_AIPriorityGet_C(struct sGameObjectMain* gao)
{
    return gao->extended->AIPriority;
}

tAINode* AI_EvalFunc_AIPriorityGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_AIPriorityGet_C(gaoUltra));
    return node + 1;
}

void AI_EvalFunc_AIPrioritySet_C(struct sGameObjectMain* gao, char priority)
{
    gao->extended->AIPriority = priority;
}

tAINode* AI_EvalFunc_AIPrioritySet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char priority;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    priority = AI_GetIntFromStack();
    AI_EvalFunc_AIPrioritySet_C(gaoUltra, priority);
    return node + 1;
}

void AI_EvalFunc_AIMainActorSet_C(struct sGameObjectMain* gao, int actorIdx)
{
    AI_MainActor[actorIdx] = gao;
}

tAINode* AI_EvalFunc_AIMainActorSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int actorIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    actorIdx = AI_GetIntFromStack();
    AI_EvalFunc_AIMainActorSet_C(gaoUltra, actorIdx);
    return node + 1;
}

tGameObjectMain* AI_EvalFunc_AIMainActorGet_C(int actorIdx)
{
    return AI_MainActor[actorIdx];
}

tAINode* AI_EvalFunc_AIMainActorGet(tAINode* node)
{
    tGameObjectMain* actor;
    int actorIdx;

    actorIdx = AI_GetIntFromStack();
    actor = AI_EvalFunc_AIMainActorGet_C(actorIdx);
    AI_PutObjectToStack(&actor, AI_gaw_EnumLink[40]);
    return node + 1;
}

// Registers gao to be called back through func when target raises id.
// The free slot is reused where there is one, and the list grows by a
// single entry otherwise.
void AI_EvalFunc_AICBAdd_C(struct sGameObjectMain* gao, struct sGameObjectMain* target, int id, struct sAIFunction* func)
{
    tAI* instance;
    int slot;

    instance = target->extended->AIInstance;
    if (!instance->instanceAICBList)
    {
        instance->instanceAICBList = (tAiCBElemStruct*)MEM_p_Alloc(sizeof(tAiCBElemStruct));
        instance->instanceAICBListNum = 1;
        slot = 0;
    }
    else
    {
        for (slot = 0;
             slot < instance->instanceAICBListNum && instance->instanceAICBList[slot].functionData;
             ++slot)
            ;

        if (slot == instance->instanceAICBListNum)
        {
            instance->instanceAICBList = (tAiCBElemStruct*)MEM_p_Realloc(
                instance->instanceAICBList,
                sizeof(tAiCBElemStruct) * (instance->instanceAICBListNum + 1));
            slot = instance->instanceAICBListNum;
            instance->instanceAICBListNum = slot + 1;
        }
    }

    instance->instanceAICBList[slot].id = id;
    instance->instanceAICBList[slot].functionData = func;
    instance->instanceAICBList[slot].gao = gao;
}

tAINode* AI_EvalFunc_AICBAdd(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* target;
    tAIFunction* func;
    int id;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    func = AI_l_PopFunction();
    id = AI_GetIntFromStack();
    target = (tGameObjectMain*)AI_GetObjectFromStack();
    AI_EvalFunc_AICBAdd_C(gaoUltra, target, id, func);
    return node + 1;
}

// Frees the matching slot by clearing its function; the list itself
// keeps its length.
void AI_EvalFunc_AICBDel_C(struct sGameObjectMain* gao, struct sGameObjectMain* target, int id, struct sAIFunction* func)
{
    tAI* instance;
    int slot;

    instance = target->extended->AIInstance;
    for (slot = 0; slot < instance->instanceAICBListNum; ++slot)
    {
        if (instance->instanceAICBList[slot].id == id
            && instance->instanceAICBList[slot].functionData == func
            && instance->instanceAICBList[slot].gao == gao)
        {
            instance->instanceAICBList[slot].functionData = 0;
            return;
        }
    }
}

tAINode* AI_EvalFunc_AICBDel(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* target;
    tAIFunction* func;
    int id;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    func = AI_l_PopFunction();
    id = AI_GetIntFromStack();
    target = (tGameObjectMain*)AI_GetObjectFromStack();
    AI_EvalFunc_AICBDel_C(gaoUltra, target, id, func);
    return node + 1;
}

int AI_EvalFunc_AITrackCurGetNum_C()
{
    return AI_gul_CurrentTrack;
}

tAINode* AI_EvalFunc_AITrackCurGetNum(tAINode* node)
{
    AI_PutIntToStack(AI_EvalFunc_AITrackCurGetNum_C());
    return node + 1;
}

void AI_EvalFunc_AIMainActorReset_C(int actorIdx)
{
    AI_MainActor[actorIdx] = NULL;
}

tAINode* AI_EvalFunc_AIMainActorReset(tAINode* node)
{
    int actorIdx;

    actorIdx = AI_GetIntFromStack();
    AI_EvalFunc_AIMainActorReset_C(actorIdx);
    return node + 1;
}

tAINode* AI_EvalFunc_AIReinitUniv(tAINode* node)
{
    AI_ReinitUniverse();
    return node + 1;
}

void AI_EvalFunc_AIReinitVars_C(struct sGameObjectMain* gao)
{
    AI_ReinitInstance(gao->extended->AIInstance);
}

tAINode* AI_EvalFunc_AIReinitVars(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_AIReinitVars_C(gaoUltra);
    return node + 1;
}

int AI_EvalFunc_AIGetCurSystem_C()
{
    return ENG_PlatformID;
}

tAINode* AI_EvalFunc_AIGetCurSystem(tAINode* node)
{
    AI_PutIntToStack(AI_EvalFunc_AIGetCurSystem_C());
    return node + 1;
}
