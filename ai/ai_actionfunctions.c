#include "../gameobject.h"
#include "ai_actionfunctions.h"

BOOL AI_EvalFunc_ACTHasKit_C(struct sGameObjectMain* gao)
{
    return gao
        && (gao->flags & 2) != 0
        && gao->basic
        && gao->basic->actorObjectOrVisualObject
        && ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->actionKit;
}

tAINode* AI_EvalFunc_ACTHasKit(tAINode* node)
{
    tGameObjectMain* target;

    target = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_ACTHasKit_C(target));
    return node + 1;
}

int AI_EvalFunc_ACTActionGet_C(struct sGameObjectMain* gao)
{
    return ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->currentAction;
}

int AI_EvalFunc_ACTActionGet_C_CURRENT()
{
    return ((tGameObjectActorObject*)(AI_gpst_CurrentGameObject->basic->actorObjectOrVisualObject))->currentAction;
}

tAINode* AI_EvalFunc_ACTActionGet(tAINode* node)
{
    tGameObjectMain* target;

    target = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_ACTActionGet_C(target));
    return node + 1;
}

int AI_EvalFunc_ACTActionItemGet_C(struct sGameObjectMain* gao)
{
    return ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->actionItem;
}

tAINode* AI_EvalFunc_ACTActionItemGet(tAINode* node)
{
    tGameObjectMain* target;

    target = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_ACTActionItemGet_C(target));
    return node + 1;
}

void AI_EvalFunc_ACTActionSet_C(struct sGameObjectMain* gao, int actionId)
{
    ACT_SetAction(gao, actionId, 0);
    ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->currTime = 0;
}

void AI_EvalFunc_ACTActionSet_C_CURRENT(int actionId)
{
    ACT_SetAction(AI_gpst_CurrentGameObject, actionId, 0);
    ((tGameObjectActorObject*)(AI_gpst_CurrentGameObject->basic->actorObjectOrVisualObject))->currTime = 0;
}

tAINode* AI_EvalFunc_ACTActionSet(tAINode* node)
{
    tGameObjectMain* target;
    int actionId;

    target = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    actionId = AI_GetIntFromStack();

    AI_EvalFunc_ACTActionSet_C(target, actionId);
    return node + 1;
}

void AI_EvalFunc_ACTActionFrequencySet_C(tGameObjectMain* gao, int actionId, int framesPerSecond)
{
    tGameObjectActorObject* actor;
    unsigned int frequency;

    ACT_SetAction(gao, actionId, 1);
    actor = (tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject);
    frequency = (unsigned int)((float)framesPerSecond * 60.0 / 64.0 + 0.5);
    if (ENG_gb_EVERunning)
        actor->eventRunningFrequency = frequency;
    else
        actor->animPtr[0]->frequency = frequency;
}

tAINode* AI_EvalFunc_ACTActionFrequencySet(tAINode* node)
{
    tGameObjectMain* target;
    int actionId;
    int framesPerSecond;

    framesPerSecond = AI_GetIntFromStack();
    actionId = AI_GetIntFromStack();

    target = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_ACTActionFrequencySet_C(target, actionId, framesPerSecond);
    return node + 1;
}

int AI_EvalFunc_ACTFrequencyGet_C(struct sGameObjectMain* gao, int actionId, int itemId)
{
    tActionKit* kit = ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->actionKit;
    tAction** actions = (tAction**)(((char*)kit) + sizeof(tActionKit));
    tActionElem* actionElems;

    if (!actions[actionId])
        return 0;

    actionElems = (tActionElem*)(((char*)actions[actionId]) + sizeof(tAction));
    return actionElems[itemId].actionFrequency;
}

tAINode* AI_EvalFunc_ACTFrequencyGet(tAINode* node)
{
    tGameObjectMain* target;
    int actionId;
    int itemId;

    itemId = AI_GetIntFromStack();
    actionId = AI_GetIntFromStack();

    target = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_ACTFrequencyGet_C(target, actionId, itemId));
    return node + 1;
}

void AI_EvalFunc_ACTActionFrameSet_C(tGameObjectMain* gao, int actionId, int16_t frame)
{
    // the frame is set on both sides of the action change, because
    // setting the action is what reads it
    ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->currTime = frame;
    ACT_SetAction(gao, actionId, 1);
    ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->currTime = frame;
}

tAINode* AI_EvalFunc_ACTActionFrameSet(tAINode* node)
{
    tGameObjectMain* target;
    int actionId;
    int16_t frame;

    frame = AI_GetIntFromStack();
    actionId = AI_GetIntFromStack();

    target = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_ACTActionFrameSet_C(target, actionId, frame);
    return node + 1;
}

int AI_EvalFunc_ACTCustomBitGet_C(struct sGameObjectMain* gao)
{
    tGameObjectActorObject* actor = (tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject);
    tActionElem* actionElems = (tActionElem*)(((char*)actor->actionPtr) + sizeof(tAction));

    return actionElems[actor->actionItem].customBits;
}

tAINode* AI_EvalFunc_ACTCustomBitGet(tAINode* node)
{
    tGameObjectMain* target;

    target = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_ACTCustomBitGet_C(target));
    return node + 1;
}

int AI_EvalFunc_ACTDesignFlagsGet_C(struct sGameObjectMain* gao)
{
    tGameObjectActorObject* actor = (tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject);
    tActionElem* actionElems = (tActionElem*)(((char*)actor->actionPtr) + sizeof(tAction));

    return (uint16_t)actionElems[actor->actionItem].designFlags;
}

tAINode* AI_EvalFunc_ACTDesignFlagsGet(tAINode* node)
{
    tGameObjectMain* target;

    target = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_ACTDesignFlagsGet_C(target));
    return node + 1;
}

int AI_EvalFunc_ACTCustomBitTest_C(struct sGameObjectMain* gao, uint8_t mask)
{
    tGameObjectActorObject* actor = (tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject);
    tActionElem* actionElems = (tActionElem*)(((char*)actor->actionPtr) + sizeof(tAction));

    return mask & actionElems[actor->actionItem].customBits;
}

tAINode* AI_EvalFunc_ACTCustomBitTest(tAINode* node)
{
    tGameObjectMain* target;
    uint8_t mask;

    target = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    mask = (uint8_t)AI_GetIntFromStack();

    AI_PutIntToStack(AI_EvalFunc_ACTCustomBitTest_C(target, mask));
    return node + 1;
}

int AI_EvalFunc_ACTDesignFlagsTest_C(struct sGameObjectMain* gao, uint16_t mask)
{
    tGameObjectActorObject* actor = (tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject);
    tActionElem* actionElems = (tActionElem*)(((char*)actor->actionPtr) + sizeof(tAction));

    return mask & actionElems[actor->actionItem].designFlags;
}

tAINode* AI_EvalFunc_ACTDesignFlagsTest(tAINode* node)
{
    tGameObjectMain* target;
    uint16_t mask;

    target = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    mask = AI_GetIntFromStack();

    AI_PutIntToStack(AI_EvalFunc_ACTDesignFlagsTest_C(target, mask));
    return node + 1;
}

BOOL AI_EvalFunc_ACTActionFinished_C(struct sGameObjectMain* gao)
{
    return (((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->animPtr[0]->flags & 0x80) != 0;
}

BOOL AI_EvalFunc_ACTActionFinished_C_CURRENT()
{
    return AI_EvalFunc_ACTActionFinished_C(AI_gpst_CurrentGameObject);
}

tAINode* AI_EvalFunc_ACTActionFinished(tAINode* node)
{
    tGameObjectMain* target;

    target = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_ACTActionFinished_C(target));
    return node + 1;
}

BOOL AI_EvalFunc_ACTActionIsTransition_C(struct sGameObjectMain* gao)
{
    return ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->transitionElem != 0;
}

tAINode* AI_EvalFunc_ACTActionIsTransition(tAINode* node)
{
    tGameObjectMain* target;

    target = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_ACTActionIsTransition_C(target));
    return node + 1;
}

int AI_EvalFunc_ACTActionGetTransition_C(struct sGameObjectMain* gao)
{
    return ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->currentActionTransition;
}

tAINode* AI_EvalFunc_ACTActionGetTransition(tAINode* node)
{
    tGameObjectMain* target;

    target = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_ACTActionGetTransition_C(target));
    return node + 1;
}

tAINode* AI_EvalFunc_ACTKitMerge(tAINode* node)
{
    tGameObjectMain* target;
    tGameObjectMain* source;
    int from;
    int to;

    target = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    to = AI_GetIntFromStack();
    from = AI_GetIntFromStack();
    source = (tGameObjectMain*)AI_GetObjectFromStack();

    ACTKitMergeNoOver(target, source, from, to);
    return node + 1;
}

tAINode* AI_EvalFunc_ACTKitMergeOver(tAINode* node)
{
    tGameObjectMain* target;
    tGameObjectMain* source;
    int from;
    int to;

    target = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    to = AI_GetIntFromStack();
    from = AI_GetIntFromStack();
    source = (tGameObjectMain*)AI_GetObjectFromStack();

    ACTKitMergeOver(target, source, from, to);
    return node + 1;
}

void AI_EvalFunc_ACTKitRestore_C(tGameObjectMain* gao)
{
    ACT_ActionRestoreInit(((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->actionKit);
}

tAINode* AI_EvalFunc_ACTKitRestore(tAINode* node)
{
    tGameObjectMain* target;

    target = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_ACTKitRestore_C(target);
    return node + 1;
}