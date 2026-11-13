#include "basic_array.h"
#include "memory.h"
#include "loading.h"
#include "bigfiles.h"
#include "events.h"
#include "time.h"
#include "animation.h"
#include "events.h"
#include "gameobject.h" // there is no actions.h for now

void ACT_FreeAction(tAction** act)
{
    tActionElem* actionElems;
    BASStruct* transition;
    int i;
    int k;

    actionElems = (tActionElem*)(((char*)*act) + sizeof(tAction));

    for (i = 0; i < (*act)->actionElemNum; i++)
    {
        if (actionElems[i].eventLists)
            EVE_DeleteListTracks(actionElems[i].eventLists);

        if (actionElems[i].shape)
        {
            --actionElems[i].shape->usageCount;
            if (!actionElems[i].shape->usageCount)
            {
                MEM_Free(actionElems[i].shape);
                LOA_DeleteAddress((int)actionElems[i].shape);
                actionElems[i].shape = NULL;
            }
        }
    }

    if (!--(*act)->usageCount)
    {
        for (i = 0; i < (*act)->actionElemNum; i++)
        {
            transition = actionElems[i].transition;
            if (transition)
            {
                for (k = 0; k < transition->actElemNum; k++)
                    MEM_Free((void *)(transition->keyValPtr[k].val));

                BAS_bfree(transition);
                MEM_Free(transition);
                actionElems[i].transition = 0;
            }
        }
        MEM_Free(*act);
        LOA_DeleteAddress((int)*act);
        *act = 0;
    }
}

void ACT_ActionRestoreInit(struct sActionKit* ak)
{
    int i;
    tAction** actions = (tAction**)(((char*)ak) + sizeof(tActionKit));
    
    if (ak->savedActions)
    {

        for (i = 0; i < ak->elemNum; i++) {
            if (actions[i] == ak->savedActions[i]) {
                if (actions[i]) {
                    ACT_FreeAction(&(actions[i]));
                }
            }
            else {
                if (actions[i]) {
                    ACT_FreeAction(&(actions[i]));
                }
                actions[i] = ak->savedActions[i];
            }
        }        
        MEM_Free(ak->savedActions);
        ak->savedActions = NULL;
    }
}

void ACT_FreeActionKit(tActionKit** ak)
{
    int i;

    tAction** actions = (tAction**)(((char*)*ak) + sizeof(tActionKit));

    ACT_ActionRestoreInit(*ak);

    if ((*ak)->elemNum)
    {
        for (i = 0; i < (*ak)->elemNum; i++) {
            if (actions[i]) {
                ACT_FreeAction(&(actions[i]));
            }
        }
        if (!--(*ak)->usageCount)
        {
            MEM_Free(*ak);
            LOA_DeleteAddress((int)*ak);
            *ak = 0;
        }
    }
}

struct sTransitionElem* ACT_pst_FindTransition(tActionKit* ak, uint16_t action, uint8_t actionElemIdx, uint16_t id)
{
    tAction* actionStruct;
    tActionElem* actionElems;
    int val;
    tAction** actions = (tAction**)(((char*)ak) + sizeof(tActionKit));

    if (action == 0xFFFF)
        return 0;
    actionStruct = actions[action];
    actionElems = (tActionElem*)(((char*)actionStruct) + sizeof(tAction));
    if (!actionStruct || !actionElems[actionElemIdx].transition)
        return 0;
    val = BAS_bsearch(id, (BASStruct*)actionElems[actionElemIdx].transition);
    return (tTransitionElem*)(val == -1 ? 0 : val);
}

void ACT_SetAction(struct sGameObjectMain* gao, int actionId, int forced)
{
    tGameObjectActorObject* actor;
    tActionKit* actKit;
    tAction** actions;
    tAction* actionStruct;
    tActionElem* actionElems;
    tTransitionElem* prevTransition;
    tTransitionElem* fromTransition;
    tTransitionElem* selfTransition;
    tTransitionElem* transition;
    tGameObjectEventsList* eventList;
    tGameObjectEventsListElem* track;
    tGameObjectExtended* extended;
    tAnimTracks* tracks;
    tEventData* animData;
    tEventData* recycledData;
    tAnim* anim;
    tAnim* blendAnim;
    Mtx4x4* savedRefMatrix;
    tGameObjectMain* savedOwnerGao;
    tEventData* savedData;
    tGameObjectEventsList* savedListTracks;
    tGameObjectEventsListElem* savedTrack;
    tGameObjectEventElem* savedEvent;
    tAnimTracks* savedParam;
    float savedDt;
    double blendTime;
    float newFullTime;
    float oldCurrTime = 0;
    float oldFullTime = 0;
    uint16_t keyFrame;
    uint16_t startTime;
    uint8_t actionFlags;
    uint8_t selfFlags;
    char hadTransition;
    int actionIdx;

    actor = gao->basic->actorObjectOrVisualObject;
    actionIdx = actionId;
    if (actionId >= 0)
    {
        if (!forced && actionId == actor->currentAction)
        {
            anim = actor->animPtr[0];
            if (anim && (anim->flags & 0x80) == 0)
                return;
        }
    }
    else
    {
        actionIdx = actionId & 0x7FFFFFFF;
    }

    if (ENG_gb_EVERunning)
    {
        actor->eventRunningFrequency = -1;
        actor->elapsedDT = EVE_gf_ElapsedDT;
        actor->currentActionTransition = actionIdx;
        return;
    }

    actKit = actor->actionKit;
    actions = (tAction**)(((char*)actKit) + sizeof(tActionKit));
    if (actionIdx < 0 || actionIdx >= actKit->elemNum || (actionIdx && !actions[actionIdx]))
        return;

    prevTransition = 0;
    hadTransition = 0;
    if (actor->transitionElem)
    {
        prevTransition = actor->transitionElem;
        hadTransition = 1;
        if (actor->currentActionTransition == actionIdx)
            return;
        actor->transitionElem = 0;
    }
    actor->flagsEgBlendIw2 &= ~2u;

    transition = 0;
    if (actor->currentAction != 0xFFFF)
    {
        fromTransition = ACT_pst_FindTransition(actKit, actor->currentAction, actor->actionItem, actionIdx);
        if (fromTransition
            && (actor->currentAction != (int16_t)actionIdx || (fromTransition->flags & 0x10) == 0))
        {
            transition = fromTransition;
        }
        else if (!hadTransition
            || prevTransition && ((prevTransition->flags & 0x40) != 0
                || actionIdx != actor->currentActionTransition && actor->currentActionTransition != 0xFFFF))
        {
            selfTransition = ACT_pst_FindTransition(actKit, actionIdx, 0, actionIdx);
            if (selfTransition)
            {
                selfFlags = selfTransition->flags;
                if ((selfFlags & 0x10) != 0
                    && (actor->currentAction != (int16_t)actionIdx || (selfFlags & 0x40) != 0))
                {
                    transition = selfTransition;
                }
            }
        }
    }

    if (transition)
    {
        // preserved: animPtr[0] is dereferenced here and null-tested on the next statement
        if ((transition->flags & 3) != 0)
            EVE_GetTracksTime(actor->animPtr[0]->ptr, 0, &oldCurrTime, &oldFullTime);

        if (transition->transitionLength && actor->animPtr[0])
        {
            if ((actor->flagsEgBlendIw2 & 2) == 0 && !actor->animPtr[1])
                actor->animPtr[1] = ANI_AllocAnim();
            blendAnim = actor->animPtr[1];
            actor->flagsEgBlendIw2 |= 2u;
            recycledData = blendAnim->ptr;
            memcpy(blendAnim, actor->animPtr[0], sizeof(tAnim));
            actor->animPtr[0]->ptr = recycledData;
            actor->animPtr[0]->ptr->eventListStruct = 0;
            MATH_CopyMatrix(&blendAnim->flashMtx, OBJ_pst_GetFlashMatrix(gao));
            blendAnim->actionLength = transition->transitionLength;
            blendAnim->actionForTransition = actor->currentAction;
            blendAnim->actionElemForTransition = actor->actionItem;
            blendAnim->transitionFlags = transition->flags;
            blendAnim->startTime = 0;
            blendAnim->flags2 = 0;
            animData = blendAnim->ptr;
            tracks = animData->tracks;
            track = animData->eventListStruct->listElems;
            keyFrame = tracks->currElemKeyFrame;
            if ((blendAnim->flags & 0x80) != 0
                || keyFrame == track->elemNum - 1
                && (double)track->trackElems[keyFrame].duration * 0.016666668 == tracks->currTime)
            {
                blendAnim->transitionFlags |= 0x4;
                blendAnim->flags2 = 2;
            }
            actionIdx = (actionIdx & 0xffff0000) | actionId;
        }
        actor->currentActionTransition = actionIdx;
        actor->transitionElem = transition;
        actionIdx = transition->actionID;
    }

    actionStruct = actions[actionIdx];
    actor->actionPtr = actionStruct;
    actor->currentAction = actionIdx;
    if (!actionStruct || !actionStruct->actionElemNum)
    {
        actor->flagsEgBlendIw2 = 0;
        return;
    }

    actionElems = (tActionElem*)(((char*)actionStruct) + sizeof(tAction));
    eventList = actionElems[0].eventLists;
    anim = actor->animPtr[0];
    if (!anim || anim == (tAnim*)-1)
        actor->animPtr[0] = ANI_AllocAnim();
    anim = actor->animPtr[0];
    animData = anim->ptr;
    if (!animData || animData->eventListStruct != eventList)
        ANI_SetTrackList(anim, eventList);

    if (actionElems[0].shape)
        ANI_ChangeShape(gao, actionElems[0].shape);
    else
        ANI_ChangeShape(gao, NULL);

    actor->flagsEgBlendIw2 |= 0x1;
    actor->actionItem = 0;
    actor->callCountForChangeAction = 0;
    actionFlags = actionElems[0].b5;
    if ((gao->flags & 4) != 0)
        gao->basic->dyn->flags = (gao->basic->dyn->flags & ~0x1C0000) | ((actionFlags << 14) & 0x1C0000);

    actor->animPtr[0]->flags = (actor->animPtr[0]->flags & 0xFF70) | (actionFlags & 0xF);
    actor->animPtr[0]->frequency = 60 * actionElems[0].actionFrequency / 64;

    if (transition)
    {
        if ((transition->flags & 3) != 0)
        {
            EVE_GetTracksTime(actor->animPtr[0]->ptr, 0, 0, &newFullTime);
            if ((transition->flags & 1) != 0)
                blendTime = newFullTime / oldFullTime * oldCurrTime;
            else
                blendTime = (1.0 - oldCurrTime / oldFullTime) * newFullTime;
            EVE_SetTracksTime(actor->animPtr[0]->ptr, (float)blendTime);
            ANI_UpdateFlash(gao, 1);
        }
        if (actor->currentActionTransition == actor->currentAction)
        {
            actor->transitionElem = 0;
            actor->currentActionTransition = -1;
        }
    }

    if (ENG_gb_AIRunning)
    {
        startTime = actor->currTime;
        if (startTime)
            EVE_SetTracksTime(actor->animPtr[0]->ptr, (float)((double)startTime * 0.016666668));
    }

    savedOwnerGao = EVE_gpst_OwnerGAO;
    savedData = EVE_gpst_CurrentData;
    savedListTracks = EVE_gpst_CurrentListTracks;
    savedRefMatrix = EVE_gpst_CurrentRefMatrix;
    savedTrack = EVE_gpst_CurrentTrack;
    savedEvent = EVE_gpst_CurrentEvent;
    savedParam = EVE_gpst_CurrentParam;
    savedDt = TIM_gf_dt;
    TIM_gf_dt = 0.0;

    extended = gao->extended;
    if (!extended || (extended->extraFlag & 1) == 0)
        ANI_OneCall(gao);

    EVE_gpst_CurrentData = savedData;
    EVE_gpst_CurrentRefMatrix = savedRefMatrix;
    EVE_gpst_OwnerGAO = savedOwnerGao;
    EVE_gpst_CurrentTrack = savedTrack;
    EVE_gpst_CurrentListTracks = savedListTracks;
    EVE_gpst_CurrentParam = savedParam;
    EVE_gpst_CurrentEvent = savedEvent;
    TIM_gf_dt = savedDt;
}

// TODO FIXME: kicsit át lett írva, megnézni jó-e
void ACT_SetUsageCounts(struct sActionKit* ak)
{
    tAction* actionStruct;
    tActionElem* actionElems;
    tGameObjectEventsList* eventList;
    int i;
    int k;
    tAction **actions = (tAction**)(((char*)ak) + sizeof(tActionKit)); // fixed, tAction volt

    ++ak->usageCount;

    for (i = 0; i < ak->elemNum; i++)
    {
        actionStruct = actions[i];
        if (!actionStruct)
            continue;

        ++actionStruct->usageCount;
        actionElems = (tActionElem*)(((char*)actionStruct) + sizeof(tAction));
        for (k = 0; k < actionStruct->actionElemNum; k++)
        {
            eventList = actionElems[k].eventLists;
            if (eventList)
                ++eventList->usageCount;
            if (actionElems[k].shape)
                ++(actionElems[k].shape->usageCount);
        }
    }
}

struct sAction* ACT_ul_ActionCallback(uint32_t Offset)
{
    tAction* act;
    tActionElem* actionElems;
    BASStruct* transition;
    BASKeyValStruct* keyVal;
    tTransitionElem* transitionElem;
    char* readPtr;
    uint8_t elemNum;
    int shapeKey;
    int i;
    int k;
    int size;

    elemNum = 0;
    readPtr = BIG_pc_ReadFileTmp(Offset, &size);
    if (size)
        elemNum = LOA_ReadChar(&readPtr, 0, 1);

    if (size && 20 * elemNum != 0xFFFFFFFC)
    {
        act = (tAction*)MEM_p_Alloc(20 * elemNum + 4);
        act->actionElemNum = elemNum;
        act->actionElemIDForTransition = LOA_ReadChar(&readPtr, 0, 1);
        LOA_ReadShort(&readPtr, 0, 2);
        actionElems = (tActionElem*)(((char*)act) + sizeof(tAction));

        for (i = 0; i < elemNum; i++)
        {
            actionElems[i].eventLists = (tGameObjectEventsList*)LOA_ReadLong(&readPtr, 0, 1);
            actionElems[i].transition = (BASStruct*)LOA_ReadLong(&readPtr, 0, 1);
            actionElems[i].shape = (tShape*)LOA_ReadLong(&readPtr, 0, 1);
            actionElems[i].changeActionCallAmountToChange = LOA_ReadChar(&readPtr, 0, 1);
            actionElems[i].b2 = LOA_ReadChar(&readPtr, 0, 1);
            actionElems[i].actionFrequency = LOA_ReadChar(&readPtr, 0, 1);
            actionElems[i].customBits = LOA_ReadChar(&readPtr, 0, 1);
            actionElems[i].designFlags = LOA_ReadShort(&readPtr, 0, 1);
            actionElems[i].b5 = LOA_ReadChar(&readPtr, 0, 1);
            LOA_ReadChar(&readPtr, 0, 2);
            if (!actionElems[i].changeActionCallAmountToChange)
                actionElems[i].changeActionCallAmountToChange = 1;
        }
    }
    else
    {
        act = (tAction*)MEM_p_Alloc(324u);
        act->actionElemNum = 0;
        act->actionElemIDForTransition = 0;
    }

    act->usageCount = 0;
    actionElems = (tActionElem*)(((char*)act) + sizeof(tAction));

    if (!elemNum)
        return act;

    for (i = 0; i < elemNum; i++)
    {
        if (actionElems[i].eventLists != (tGameObjectEventsList*)-1)
            LOA_MakeFileRef((int)actionElems[i].eventLists, &actionElems[i].eventLists,
                EVE_ul_CallbackLoadListTracks, 1);

        shapeKey = (int)actionElems[i].shape;
        if (shapeKey == -1)
            actionElems[i].shape = 0;
        else
            LOA_MakeFileRef(shapeKey, &actionElems[i].shape, ANI_ul_ShapeCallback, 1);
    }

    for (i = 0; i < elemNum; i++)
    {
        if (!actionElems[i].transition)
            continue;

        transition = (BASStruct*)MEM_p_Alloc(16u);
        LOA_ReadLong(&readPtr, 0, 2);
        transition->actElemNum = LOA_ReadLong(&readPtr, 0, 1);
        LOA_ReadLong(&readPtr, 0, 2);
        transition->maxElemNum = transition->actElemNum;
        transition->elemNumToAddWhenFull = LOA_ReadLong(&readPtr, 0, 1);
        transition->keyValPtr = (BASKeyValStruct*)MEM_p_Alloc(8 * transition->maxElemNum);

        keyVal = transition->keyValPtr;
        for (k = 0; k < transition->actElemNum; k++, ++keyVal)
        {
            keyVal->key = LOA_ReadLong(&readPtr, 0, 1);
            LOA_ReadLong(&readPtr, 0, 2);
        }

        keyVal = transition->keyValPtr;
        for (k = 0; k < transition->actElemNum; k++, ++keyVal)
        {
            transitionElem = (tTransitionElem*)MEM_p_Alloc(sizeof(tTransitionElem));
            keyVal->val = (int)transitionElem;
            transitionElem->actionID = LOA_ReadShort(&readPtr, 0, 1);
            transitionElem->flags = LOA_ReadChar(&readPtr, 0, 1);
            transitionElem->transitionLength = LOA_ReadChar(&readPtr, 0, 1);
        }

        actionElems[i].transition = transition;
    }

    return act;
}

struct sActionKit* ACT_ul_ActionKitCallback(uint32_t Offset)
{
    tActionKit* ak;
    int i, size;
    int num;
    int actionKey;
    char* currReadPtr;
    tAction** actions;

    currReadPtr = BIG_pc_ReadFileTmp(Offset, &size);
    if (size)
    {
        if (LOA_gb_SpeedMode && BIG_gi_ReadMode == 2)
            num = size / 4;
        else
            num = size / 8;
        
        ak = (tActionKit*)MEM_p_Alloc(sizeof(tActionKit) + sizeof(tAction**) * num);
        memset(ak, 0, sizeof(tActionKit) + sizeof(tAction**) * num);
        
        ak->usageCount = 0;
        ak->flags = 0;
        ak->savedActions = NULL;
        ak->elemNum = num;
        
        actions = (tAction**)(((char*)ak) + sizeof(tActionKit));

        for (i = 0; i < num; ++i)
        {
            actionKey = LOA_ReadLong(&currReadPtr, 0, 1);
            if (actionKey == 1) // ilyen mindig csak a végén van, szóval ha lecsökkentjük az elemszámot nem okoz gondot
            {
                ak->elemNum--;
                ak->flags |= 1;
                num--;
                LOA_ReadLong(&currReadPtr, 0, 2);               
            }
            else
            {
                LOA_ReadLong(&currReadPtr, 0, 2);
                if (actionKey == -1 || !actionKey)
                    actions[i] = NULL;
                else
                    LOA_MakeFileRef(actionKey, &actions[i], ACT_ul_ActionCallback, 1);
            }
        }
    }
    else
    {
        ak = (tActionKit*)MEM_p_Alloc(sizeof(tActionKit) + sizeof(tAction**) * 512);
        memset(ak, 0, sizeof(tActionKit) + sizeof(tAction**) * 512);
    }
    return ak;
}

void ACT_UseAction(tAction* act)
{
    tActionElem* actionElems = (tActionElem*)(((char*)act) + sizeof(tAction));
    int i;

    act->usageCount++;

    for (i = 0; i < act->actionElemNum; i++) {
        if (actionElems[i].eventLists)
            actionElems[i].eventLists->usageCount++;
        if (actionElems[i].shape)
            actionElems[i].shape->usageCount++;
    }
}

void ACT_ActionSaveInit(tActionKit* ak)
{
    int i;

    if (!ak->savedActions)
    {
        ak->savedActions = (tAction**)MEM_p_Alloc(sizeof(tAction**) * ak->elemNum);
        memcpy(ak->savedActions, (((char*)ak) + sizeof(tActionKit)), sizeof(tAction**) * ak->elemNum);

        for (i = 0; i < ak->elemNum; i++) {
            if (ak->savedActions[i])
                ACT_UseAction(ak->savedActions[i]);
        }        
    }
}

void ACTKitMerge(struct sGameObjectMain* gaoDest, struct sGameObjectMain* gaoSrc, int firstActionID, int lastActionID, BOOL overWrite)
{
    int i;
    tGameObjectActorObject* gaodestao;
    tGameObjectActorObject* gaosrcao;
    tAction** gaodestactions;
    tAction** gaosrcactions;

    gaodestao = gaoDest->basic->actorObjectOrVisualObject;
    gaosrcao = gaoSrc->basic->actorObjectOrVisualObject;
    ACT_ActionSaveInit(gaodestao->actionKit);

    gaodestactions = (tAction**)(((char*)(gaodestao->actionKit)) + sizeof(tActionKit));
    gaosrcactions = (tAction**)(((char*)(gaosrcao->actionKit)) + sizeof(tActionKit));

    for (i = firstActionID; i <= lastActionID; i++)
    {
        if (gaosrcactions[i] || overWrite)
        {
            if (gaodestactions[i])
                ACT_FreeAction(&gaodestactions[i]);
            gaodestactions[i] = gaosrcactions[i];
            if (gaodestactions[i])
                ACT_UseAction(gaodestactions[i]);
        }
    }
}

void ACTKitMergeNoOver(struct sGameObjectMain* gaoDest, struct sGameObjectMain* gaoSrc, int firstActionID, int lastActionID)
{
    ACTKitMerge(gaoDest, gaoSrc, firstActionID, lastActionID, FALSE);
}

void ACTKitMergeOver(struct sGameObjectMain* gaoDest, struct sGameObjectMain* gaoSrc, int firstActionID, int lastActionID)
{
    ACTKitMerge(gaoDest, gaoSrc, firstActionID, lastActionID, TRUE);
}
