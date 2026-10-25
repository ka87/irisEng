#include "../gameobject.h"
#include "../dbg.h"
#include "ai_actionfunctions.h"

BOOL AI_EvalFunc_ACTHasKit_C(struct sGameObjectMain* a1)
{
    return a1
        && (a1->flags & 2) != 0
        && a1->basic
        && a1->basic->actorObjectOrVisualObject
        && ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->actionKit;
}

tAINode* AI_EvalFunc_ACTHasKit(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_ACTHasKit_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_ACTActionGet_C(struct sGameObjectMain* a1)
{
    return ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->currentAction;
}

int AI_EvalFunc_ACTActionGet_C_CURRENT()
{
    return ((tGameObjectActorObject*)(AI_gpst_CurrentGameObject->basic->actorObjectOrVisualObject))->currentAction;
}

tAINode* AI_EvalFunc_ACTActionGet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_ACTActionGet_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_ACTActionItemGet_C(struct sGameObjectMain* a1)
{
    return ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->actionItem;
}

tAINode* AI_EvalFunc_ACTActionItemGet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_ACTActionItemGet_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_ACTActionSet_C(struct sGameObjectMain* a1, int actionId)
{
    DBG_OutputDebugString("ACT: Set gao %p action %d\n",
        (void*)a1, actionId);
    ACT_SetAction(a1, actionId, 0);
    ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->currTime = 0;
}

void AI_EvalFunc_ACTActionSet_C_CURRENT(int a1)
{
    ACT_SetAction(AI_gpst_CurrentGameObject, a1, 0);
    ((tGameObjectActorObject*)(AI_gpst_CurrentGameObject->basic->actorObjectOrVisualObject))->currTime = 0;
}

tAINode* AI_EvalFunc_ACTActionSet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_ACTActionSet_C(v3, v1);
    return a1 + 1;
}

void AI_EvalFunc_ACTActionFrequencySet_C(tGameObjectMain* a1, int actionId, int a3)
{
    float v3;
    float v4;

    ACT_SetAction(a1, actionId, 1);
    if (ENG_gb_EVERunning)
    {
        v4 = (float)a3;
        ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->eventRunningFrequency = (unsigned int)(v4 * 60.0 / 64.0 + 0.5);
    }
    else
    {
        v3 = (float)a3;
        ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->animPtr[0]->frequency = (unsigned int)(v3 * 60.0 / 64.0 + 0.5);
    }
}

tAINode* AI_EvalFunc_ACTActionFrequencySet(tAINode* a1)
{
    tGameObjectMain* v2;
    int a3;
    int v4;

    a3 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_ACTActionFrequencySet_C(v2, v4, a3);
    return a1 + 1;
}

int AI_EvalFunc_ACTFrequencyGet_C(struct sGameObjectMain* a1, int a2, int a3)
{
    int result;
    tActionKit* kit = ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->actionKit;
    tAction** actions = (tAction**)(((char*)kit) + sizeof(tActionKit));
    tActionElem* actionElems;

    if (actions[a2]) {
        actionElems = (tActionElem*)(((char*)actions[a2]) + sizeof(tAction));
        result = actionElems[a3].actionFrequency;
    }
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_ACTFrequencyGet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int a2;
    int a3;

    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_ACTFrequencyGet_C(v3, a2, a3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_ACTActionFrameSet_C(tGameObjectMain* a1, int actionId, int16_t a3)
{
    ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->currTime = a3;
    ACT_SetAction(a1, actionId, 1);
    ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->currTime = a3;
}

tAINode* AI_EvalFunc_ACTActionFrameSet(tAINode* a1)
{
    tGameObjectMain* v2;
    int v3;
    int16_t a3;

    a3 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_ACTActionFrameSet_C(v2, v3, a3);
    return a1 + 1;
}

int AI_EvalFunc_ACTCustomBitGet_C(struct sGameObjectMain* a1)
{
    tAction* action = ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->actionPtr;
    tActionElem* actionElems = (tActionElem*)(((char*)action) + sizeof(tAction));

    return actionElems[((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->actionItem].customBits;
}

tAINode* AI_EvalFunc_ACTCustomBitGet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_ACTCustomBitGet_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_ACTDesignFlagsGet_C(struct sGameObjectMain* a1)
{
    tAction* action = ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->actionPtr;
    tActionElem* actionElems = (tActionElem*)(((char*)action) + sizeof(tAction));

    return (uint16_t)actionElems[((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->actionItem].designFlags;
}

tAINode* AI_EvalFunc_ACTDesignFlagsGet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_ACTDesignFlagsGet_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_ACTCustomBitTest_C(struct sGameObjectMain* a1, uint8_t a2)
{
    tAction* action = ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->actionPtr;
    tActionElem* actionElems = (tActionElem*)(((char*)action) + sizeof(tAction));

    return a2 & actionElems[((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->actionItem].customBits;
}

tAINode* AI_EvalFunc_ACTCustomBitTest(tAINode* a1)
{
    uint8_t v1;
    int v2;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = (uint8_t)AI_GetIntFromStack();
    v2 = AI_EvalFunc_ACTCustomBitTest_C(v4, v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_ACTDesignFlagsTest_C(struct sGameObjectMain* a1, uint16_t a2)
{
    tAction* action = ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->actionPtr;
    tActionElem* actionElems = (tActionElem*)(((char*)action) + sizeof(tAction));

    return a2 & actionElems[((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->actionItem].designFlags;
}

tAINode* AI_EvalFunc_ACTDesignFlagsTest(tAINode* a1)
{
    uint16_t v1;
    int v2;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_ACTDesignFlagsTest_C(v4, v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

BOOL AI_EvalFunc_ACTActionFinished_C(struct sGameObjectMain* a1)
{
    tGameObjectActorObject* ao =
        (tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject);
    int done = (ao->animPtr[0]->flags & 0x80) != 0;
    {   /* Temporary: both the compiled and the interpreted path reach
           here, so the two builds can be compared line for line. */
        static tGameObjectMain* last; static int lastv = -1; static int n;
        if (a1 != last || done != lastv || ++n % 120 == 0)
        {
            DBG_OutputDebugString(
                "ACT: Finished gao %p action %d -> %d  flags 0x%04X\n",
                (void*)a1, (int)ao->currentAction, done,
                (unsigned)(uint16_t)ao->animPtr[0]->flags);
            last = a1; lastv = done;
        }
    }
    return done;
}

BOOL AI_EvalFunc_ACTActionFinished_C_CURRENT()
{
    return AI_EvalFunc_ACTActionFinished_C(AI_gpst_CurrentGameObject);
}

tAINode* AI_EvalFunc_ACTActionFinished(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_ACTActionFinished_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

BOOL AI_EvalFunc_ACTActionIsTransition_C(struct sGameObjectMain* a1)
{
    return ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->transitionElem != 0;
}

tAINode* AI_EvalFunc_ACTActionIsTransition(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_ACTActionIsTransition_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_ACTActionGetTransition_C(struct sGameObjectMain* a1)
{
    return ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->currentActionTransition;
}

tAINode* AI_EvalFunc_ACTActionGetTransition(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_ACTActionGetTransition_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_ACTKitMerge(tAINode* a1)
{
    tGameObjectMain* v2;
    int a3;
    int a4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    ACTKitMergeNoOver(v2, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a3, a4);
    return a1 + 1;
}

tAINode* AI_EvalFunc_ACTKitMergeOver(tAINode* a1)
{
    tGameObjectMain* v2;
    int a3;
    int a4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    ACTKitMergeOver(v2, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a3, a4);
    return a1 + 1;
}

void AI_EvalFunc_ACTKitRestore_C(tGameObjectMain* a1)
{
    ACT_ActionRestoreInit(((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->actionKit);
}

tAINode* AI_EvalFunc_ACTKitRestore(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_ACTKitRestore_C(v2);
    return a1 + 1;
}