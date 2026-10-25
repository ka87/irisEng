#include "basic_array.h"
#include "dbg.h"
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
    int v1;
    int v2;
    int v5;
    int v6;
    BASStruct* v7;
    int v8;
    tActionElem* actionElems;

    v1 = 0;
    if ((*act)->actionElemNum)
    {
        v2 = 0;
        do
        {
            actionElems = (tActionElem*)(((char*)*act) + sizeof(tAction));

            if (actionElems[v2].eventLists)
                EVE_DeleteListTracks(actionElems[v2].eventLists);

            if (actionElems[v2].shape)
            {
                --actionElems[v2].shape->usageCount;
                if (!actionElems[v2].shape->usageCount)
                {
                    MEM_Free(actionElems[v2].shape);
                    LOA_DeleteAddress((int)actionElems[v2].shape);
                    actionElems[v2].shape = NULL;
                }
            }
            ++v1;
            ++v2;
        } while (v1 < (*act)->actionElemNum);
    }
    if (!--(*act)->usageCount)
    {
        v5 = 0;
        if ((*act)->actionElemNum)
        {
            v6 = 0;
            do
            {
                actionElems = (tActionElem*)(((char*)*act) + sizeof(tAction));
                v7 = actionElems[v6].transition;
                if (v7)
                {
                    v8 = 0;
                    if (v7->actElemNum > 0)
                    {
                        do
                            MEM_Free((void *)(actionElems[v6].transition->keyValPtr[v8++].val));
                        while (v8 < actionElems[v6].transition->actElemNum);
                    }
                    BAS_bfree((BASStruct*)actionElems[v6].transition);
                    MEM_Free(actionElems[v6].transition);
                    actionElems[v6].transition = 0;
                }
                ++v5;
                ++v6;
            } while (v5 < (*act)->actionElemNum);
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

struct sTransitionElem* ACT_pst_FindTransition(tActionKit* a1, unsigned __int16 action, unsigned __int8 actionelem, unsigned __int16 id)
{
    tAction* v5;
    int v6;
    tActionElem* actionElems;
    tAction** actions = (tAction**)(((char*)a1) + sizeof(tActionKit));

    if (action == 0xFFFF)
        return 0;
    v5 = actions[action];
    actionElems = (tActionElem*)(((char*)v5) + sizeof(tAction));
    if (!v5 || !actionElems[actionelem].transition)
        return 0;
    v6 = BAS_bsearch(id, (BASStruct*)actionElems[actionelem].transition);
    return (tTransitionElem*)(v6 == -1 ? 0 : v6);
}

void ACT_SetAction(struct sGameObjectMain* a1, int actionId, int forced)
{
    tGameObjectActorObject* gaoao;
    int aactionID;
    tAnim* v5;
    tActionKit* actKit;
    tTransitionElem* v7;
    unsigned __int16 v8;
    tTransitionElem* v9;
    tTransitionElem* v10;
    unsigned __int16 v11;
    tTransitionElem* v12;
    byte v13;
    tAnim* animPtr;
    tEventData* ptrToChain;
    tEventData* v19;
    tAnimTracks* v20;
    tGameObjectEventsListElem* v21;
    unsigned __int16 v22;
    tAction* actionStruct;
    tAnim* v24;
    tGameObjectEventsList* v25;
    tAnim* v26;
    tEventData* v27;
    tGameObjectMain* v28;
    byte v29;
    double v30;
    unsigned __int16 v31;
    tGameObjectMain* v32;
    tEventData* v33;
    tGameObjectEventsList* v34;
    Mtx4x4* v35;
    tGameObjectExtended* v36;
    float v37;
    tTransitionElem* v38;
    tGameObjectEventElem* v39;
    tActionKit* v40;
    tAnimTracks* v41;
    float a3a = 0;
    float a4 = 0;
    float v44;
    float a2b;
    float a2a;
    char forceda;
    tGameObjectEventsListElem* forcedb;
    tAction** actions;
    tActionElem* actionElems;

    gaoao = a1->basic->actorObjectOrVisualObject;
    aactionID = actionId;
    if (actionId >= 0)
    {
        if (!forced && actionId == gaoao->currentAction)
        {
            v5 = gaoao->animPtr[0];
            if (v5)
            {
                if ((~((v5->flags & 0xff) >> 7) & 1) != 0)
                {   /* Temporary: same action, not finished -- correct. */
                    static int n;
                    if (++n % 240 == 1)
                        DBG_OutputDebugString(
                            "ACT: SetAction SAME gao %p action %d flags "
                            "0x%04X\n", (void*)a1, aactionID,
                            (unsigned)(uint16_t)v5->flags);
                    return;
                }
            }
        }
    }
    else
    {
        aactionID = actionId & 0x7FFFFFFF;
    }
    if (ENG_gb_EVERunning)
    {
        {   /* Temporary: deferred, not applied. */
            static int n;
            if (++n % 60 == 1)
                DBG_OutputDebugString(
                    "ACT: SetAction DEFERRED (EVERunning) gao %p want %d "
                    "current %d\n", (void*)a1, aactionID,
                    (int)gaoao->currentAction);
        }
        gaoao->eventRunningFrequency = -1;
        gaoao->elapsedDT = EVE_gf_ElapsedDT;
        gaoao->currentActionTransition = aactionID;
        return;
    }
    actKit = gaoao->actionKit;
    actions = (tAction**)(((char*)actKit) + sizeof(tActionKit));
    v40 = actKit;
    /* Temporary: an action the kit cannot supply is silently ignored
       here, which leaves the object in its previous action for good. */
    if (!(aactionID >= 0 && aactionID < actKit->elemNum
          && (!aactionID || actions[aactionID])))
    {
        static int n;
        if (++n % 60 == 1)
            DBG_OutputDebugString(
                "ACT: SetAction REJECTED gao %p want %d  kit elems %d  "
                "slot %s  current %d\n",
                (void*)a1, aactionID, (int)actKit->elemNum,
                (aactionID >= 0 && aactionID < actKit->elemNum
                 && !actions[aactionID]) ? "null" : "out of range",
                (int)gaoao->currentAction);
    }
    if (aactionID >= 0 && aactionID < actKit->elemNum && (!aactionID || actions[aactionID]))
    {
        v7 = 0;
        forceda = 0;
        if (gaoao->transitionElem)
        {
            v7 = gaoao->transitionElem;
            forceda = 1;
            if (gaoao->currentActionTransition == aactionID)
            {   /* Temporary: already transitioning to it, nothing done. */
                static int n;
                if (++n % 60 == 1)
                    DBG_OutputDebugString(
                        "ACT: SetAction SKIPPED (already transitioning) "
                        "gao %p want %d current %d\n",
                        (void*)a1, aactionID, (int)gaoao->currentAction);
                return;
            }
            gaoao->transitionElem = 0;
        }
        gaoao->flagsEgBlendIw2 &= ~2u;
        v8 = gaoao->currentAction;
        if (v8 == 0xFFFF)
        {
            v38 = 0;
            goto LABEL_48;
        }
        v9 = ACT_pst_FindTransition(actKit, v8, gaoao->actionItem, aactionID);
        v10 = v9;
        v38 = v9;
        if (v9)
        {
            if (gaoao->currentAction != (int16_t)aactionID || (v9->flags & 0x10) == 0)
                goto LABEL_34;
            v38 = 0;
            v10 = 0;
        }
        if (!forceda
            || v7 && ((v7->flags & 0x40) != 0 || (v11 = gaoao->currentActionTransition, aactionID != v11) && v11 != 0xFFFF))
        {
            v12 = ACT_pst_FindTransition(v40, aactionID, 0, aactionID);
            v38 = v12;
            if (!v12)
            {
            LABEL_48:
                v10 = v38;
                goto LABEL_49;
            }
            v13 = v12->flags;
            if ((v13 & 0x10) == 0)
            {
                v38 = 0;
                goto LABEL_48;
            }
            if (gaoao->currentAction == (int16_t)aactionID && (v13 & 0x40) == 0)
            {
                v38 = 0;
                goto LABEL_48;
            }
            v10 = v38;
        }
        if (!v10)
        {
        LABEL_49:
            actionStruct = actions[aactionID];
            gaoao->actionPtr = actionStruct;
            {   /* Temporary: the action really changing. */
                static int n;
                if (++n % 30 == 1)
                    DBG_OutputDebugString(
                        "ACT: SetAction APPLIED gao %p %d -> %d\n",
                        (void*)a1, (int)gaoao->currentAction, aactionID);
            }
            gaoao->currentAction = aactionID;
            if (actionStruct && actionStruct->actionElemNum)
            {
                v24 = gaoao->animPtr[0];
                actionElems = (tActionElem*)(((char*)actionStruct) + sizeof(tAction));
                v25 = actionElems[0].eventLists;
                if (!v24 || v24 == (tAnim*)-1)
                    gaoao->animPtr[0] = ANI_AllocAnim();
                v26 = gaoao->animPtr[0];
                v27 = v26->ptr;
                if (!v27 || v27->eventListStruct != v25)
                    ANI_SetTrackList(v26, v25);
                if (actionElems[0].shape)
                {
                    v28 = a1;
                    ANI_ChangeShape(a1, actionElems[0].shape);
                }
                else
                {
                    ANI_ChangeShape(a1, NULL);
                    v28 = a1;
                }
                gaoao->flagsEgBlendIw2 |= 0x1;
                gaoao->actionItem = 0;
                gaoao->callCountForChangeAction = 0;
                v29 = actionElems[0].b5;
                if ((v28->flags & 4) != 0)
                    v28->basic->dyn->flags ^= (v28->basic->dyn->flags ^ (v29 << 14)) & 0x1C0000;
                gaoao->animPtr[0]->flags = (gaoao->animPtr[0]->flags & 0xff00) | (gaoao->animPtr[0]->flags & 0xF0);
                gaoao->animPtr[0]->flags = (gaoao->animPtr[0]->flags & 0xff00) | (gaoao->animPtr[0]->flags & 0x7F);
                gaoao->animPtr[0]->flags |= v29 & 0xF;
                gaoao->animPtr[0]->frequency = 60 * actionElems[0].actionFrequency / 64;
                if (v10)
                {
                    if ((v10->flags & 3) != 0)
                    {
                        EVE_GetTracksTime(gaoao->animPtr[0]->ptr, 0, 0, &v44);
                        if ((v10->flags & 1) != 0)
                            v30 = v44 / a4 * a3a;
                        else
                            v30 = (1.0 - a3a / a4) * v44;
                        a2b = (float)v30;
                        EVE_SetTracksTime(gaoao->animPtr[0]->ptr, a2b);
                        ANI_UpdateFlash(v28, 1);
                    }
                    if (gaoao->currentActionTransition == gaoao->currentAction)
                    {
                        gaoao->transitionElem = 0;
                        gaoao->currentActionTransition = -1;
                    }
                }
                if (ENG_gb_AIRunning)
                {
                    v31 = gaoao->currTime;
                    if (v31)
                    {
                        v37 = (float)((double)v31 * 0.016666668);
                        EVE_SetTracksTime(gaoao->animPtr[0]->ptr, v37);
                    }
                }
                v32 = EVE_gpst_OwnerGAO;
                v33 = EVE_gpst_CurrentData;
                v34 = EVE_gpst_CurrentListTracks;
                v35 = EVE_gpst_CurrentRefMatrix;
                a2a = TIM_gf_dt;
                forcedb = EVE_gpst_CurrentTrack;
                TIM_gf_dt = 0.0;
                v39 = EVE_gpst_CurrentEvent;
                v36 = a1->extended;
                v41 = EVE_gpst_CurrentParam;
                if (!v36 || (v36->extraFlag & 1) == 0)
                    ANI_OneCall(a1);
                EVE_gpst_CurrentData = v33;
                EVE_gpst_CurrentRefMatrix = v35;
                EVE_gpst_OwnerGAO = v32;
                EVE_gpst_CurrentTrack = forcedb;
                EVE_gpst_CurrentListTracks = v34;
                EVE_gpst_CurrentParam = v41;
                EVE_gpst_CurrentEvent = v39;
                TIM_gf_dt = a2a;
            }
            else
            {
                gaoao->flagsEgBlendIw2 = 0;
            }
            return;
        }
    LABEL_34:
        if ((v10->flags & 3) != 0)
            EVE_GetTracksTime(gaoao->animPtr[0]->ptr, 0, &a3a, &a4);

        if (v10->transitionLength && gaoao->animPtr[0])
        {
            if ((gaoao->flagsEgBlendIw2 & 2) == 0 && !gaoao->animPtr[1])
                gaoao->animPtr[1] = ANI_AllocAnim();
            animPtr = gaoao->animPtr[1];
            gaoao->flagsEgBlendIw2 |= 2u;
            ptrToChain = animPtr->ptr;
            memcpy(animPtr, gaoao->animPtr[0], sizeof(tAnim));
            gaoao->animPtr[0]->ptr = ptrToChain;
            gaoao->animPtr[0]->ptr->eventListStruct = 0;
            MATH_CopyMatrix(&animPtr->flashMtx, OBJ_pst_GetFlashMatrix(a1));
            animPtr->actionLength = v38->transitionLength;
            animPtr->actionForTransition = gaoao->currentAction;
            animPtr->actionElemForTransition = gaoao->actionItem;
            animPtr->transitionFlags = v38->flags;
            animPtr->startTime = 0;
            animPtr->flags2 = 0;
            if ((~((animPtr->flags & 0xff) >> 7) & 1) == 0
                || (v19 = animPtr->ptr,
                    v20 = v19->tracks,
                    v21 = v19->eventListStruct->listElems,
                    v22 = v20->currElemKeyFrame,
                    v22 == v21->elemNum - 1)
                && (double)v21->trackElems[v22].duration * 0.016666668 == v20->currTime)
            {
                animPtr->transitionFlags |= 0x4;
                animPtr->flags2 = 2;
            }
            v10 = v38;
            aactionID = (aactionID & 0xffff0000) | actionId;
        }
        gaoao->currentActionTransition = aactionID;
        gaoao->transitionElem = v10;
        aactionID = v10->actionID;
        goto LABEL_49;
    }
}

// TODO FIXME: kicsit át lett írva, megnézni jó-e
void ACT_SetUsageCounts(struct sActionKit* a1)
{
    int v1;
    tAction** v2;
    tAction* v3;
    byte v4;
    int v5;
    tActionElem* v6;
    tActionElem* actionElems;
    tGameObjectEventsList* v7;
    tAction **actions = (tAction**)(((char*)a1) + sizeof(tActionKit)); // fixed, tAction volt

    ++a1->usageCount;
    v1 = 0;
    if (a1->elemNum)
    {
        v2 = actions;
        do
        {
            v3 = *v2;
            if (*v2)
            {
                v4 = v3->actionElemNum;
                ++v3->usageCount;
                v5 = 0;
                if (v4)
                {
                    actionElems = (tActionElem*)(((char*)v3) + sizeof(tAction));
                    v6 = actionElems;
                    do
                    {
                        v7 = v6->eventLists;
                        if (v7)
                            ++v7->usageCount;
                        if (v6->shape)
                            ++(v6->shape->usageCount);
                        ++v5;
                        v6++;
                    } while (v5 < v3->actionElemNum);
                }
            }
            ++v1;
            ++v2;
        } while (v1 < a1->elemNum);
    }
}

struct sAction* ACT_ul_ActionCallback(uint32_t Offset)
{
    byte v1;
    int v1a;
    int v2;
    tAction* v3;
    tAction* v4;
    tActionElem* v5;
    tActionElem* v6;
    int v7;
    tActionElem* v8;
    int v9;
    tActionElem* v10;
    BASStruct* v11;
    int v12;
    BASKeyValStruct* v13;
    BASKeyValStruct* v14;
    int i;
    int v16;
    BASKeyValStruct* v17;
    tTransitionElem* v18;
    BOOL v19;
    unsigned int v21;
    char* a1;
    tAction* v23;
    tActionElem* v24;
    tActionElem* actionElems;
    int v25;
    int size;

    v1 = 0;
    a1 = BIG_pc_ReadFileTmp(Offset, &size);
    if (size && (v1 = LOA_ReadChar(&a1, 0, 1), v2 = v1, 20 * v1 != 0xFFFFFFFC))
    {
        v4 = (tAction*)MEM_p_Alloc(20 * v1 + 4);
        v23 = v4;
        v4->actionElemNum = v1;
        v4->actionElemIDForTransition = LOA_ReadChar(&a1, 0, 1);
        LOA_ReadShort(&a1, 0, 2);
        actionElems = (tActionElem*)(((char*)v4) + sizeof(tAction));
        v5 = actionElems;
        if (v1)
        {
            v6 = (tActionElem *) actionElems;
            do
            {
                v5->eventLists = (tGameObjectEventsList*)LOA_ReadLong(&a1, 0, 1);
                v5->transition = (BASStruct*)LOA_ReadLong(&a1, 0, 1);
                v5->shape = (tShape*)LOA_ReadLong(&a1, 0, 1);
                v5->changeActionCallAmountToChange = LOA_ReadChar(&a1, 0, 1);
                v5->b2 = LOA_ReadChar(&a1, 0, 1);
                v5->actionFrequency = LOA_ReadChar(&a1, 0, 1);
                v5->customBits = LOA_ReadChar(&a1, 0, 1);
                v5->designFlags = LOA_ReadShort(&a1, 0, 1);
                v5->b5 = LOA_ReadChar(&a1, 0, 1);
                LOA_ReadChar(&a1, 0, 2);
                if (!v6->changeActionCallAmountToChange)
                    v6->changeActionCallAmountToChange = 1;
                ++v6;
                ++v5;
                --v2;
            } while (v2);
        }
        v3 = v23;
    }
    else
    {
        v3 = (tAction*)MEM_p_Alloc(324u);
        v23 = v3;
        v3->actionElemNum = 0;
        v3->actionElemIDForTransition = 0;
    }
    v7 = v1;
    v3->usageCount = 0;
    actionElems = (tActionElem*)(((char*)v3) + sizeof(tAction));
    v8 = actionElems;
    if (v1)
    {
        v1a = v1;
        do
        {
            if (v8->eventLists != (tGameObjectEventsList*)-1)
                LOA_MakeFileRef((int)v8->eventLists, &v8->eventLists, EVE_ul_CallbackLoadListTracks, 1);
            v9 = (int)v8->shape;
            if (v9 == -1)
                v8->shape = 0;
            else
                LOA_MakeFileRef(v9, &v8->shape, ANI_ul_ShapeCallback, 1);
            ++v8;
            --v1a;
        } while (v1a);
        if (v7 > 0)
        {
            v10 = (tActionElem *)actionElems;
            v24 = (tActionElem *)actionElems;
            v25 = v7;
            do
            {
                if (v10->transition)
                {
                    v11 = (BASStruct*)MEM_p_Alloc(16u);
                    LOA_ReadLong(&a1, 0, 2);
                    v11->actElemNum = LOA_ReadLong(&a1, 0, 1);
                    LOA_ReadLong(&a1, 0, 2);
                    v11->maxElemNum = v11->actElemNum;
                    v12 = LOA_ReadLong(&a1, 0, 1);
                    v21 = 8 * v11->maxElemNum;
                    v11->elemNumToAddWhenFull = v12;
                    v13 = (BASKeyValStruct*)MEM_p_Alloc(v21);
                    v11->keyValPtr = v13;
                    v14 = v13;
                    for (i = 0; i < v11->actElemNum; ++v14)
                    {
                        v14->key = LOA_ReadLong(&a1, 0, 1);
                        LOA_ReadLong(&a1, 0, 2);
                        ++i;
                    }
                    v16 = 0;
                    if (v11->actElemNum > 0)
                    {
                        v17 = (BASKeyValStruct *) v11->keyValPtr;
                        do
                        {
                            v18 = (tTransitionElem*)MEM_p_Alloc(sizeof(tTransitionElem));
                            v17->val = (int)v18;
                            v18->actionID = LOA_ReadShort(&a1, 0, 1);
                            v18->flags = LOA_ReadChar(&a1, 0, 1);
                            v18->transitionLength = LOA_ReadChar(&a1, 0, 1);
                            ++v16;
                            ++v17;
                        } while (v16 < v11->actElemNum);
                        v10 = v24;
                    }
                    v3 = v23;
                    v10->transition = v11;
                }
                ++v10;
                v19 = v25 == 1;
                v24 = v10;
                --v25;
            } while (!v19);
        }
    }
    return v3;
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