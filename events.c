#include "memory.h"
#include "loading.h"
#include "bigfiles.h"
#include "gameobject.h"
#include "time.h"
#include "ai.h"
#include "animation.h"
#include "modifiers/leglinkmodifier.h"
#include "dbg.h"
#include "events.h"

int EVE_gb_CanFlash = 1;

static double EVE_l_FrameTime(uint32_t frames)
{
    return frames * 1.0 / 60.0;
}

static tGameObjectActorObject* EVE_l_Actor(tGameObjectMain* gao)
{
    return (tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject);
}

static tGameObjectMain* EVE_l_Bone(tGameObjectActorObject* actor, uint16_t boneID)
{
    if (boneID >= actor->skeletonGroup->tablePF->actElemNum)
        return NULL;
    return (tGameObjectMain*)((tableElemStructPF*)(actor->skeletonGroup->tablePF->firstElemPtr))[boneID].objectPtr;
}

static Mtx4x4* EVE_l_CurrentMatrix(tGameObjectMain* gao)
{
    if ((gao->flags & 0x400000) != 0)
        return &OBJ_pst_GetLocalMatrix(gao)->mtx;
    return OBJ_pst_GetAbsoluteMatrix(gao);
}

static void EVE_l_RefRelativeToCurrent(Mtx4x4* dest)
{
    Mtx4x4 inverse;

    MATH_InvertMatrix(&inverse, EVE_l_CurrentMatrix(EVE_gpst_CurrentGAO));
    MATH_MulMatrixMatrix(dest, EVE_gpst_CurrentRefMatrix, &inverse);
}

static tGaoMatrix* EVE_l_TargetMatrix(tGameObjectMain* gao, BOOL* refIsCurrent)
{
    if ((gao->flags & 0x400000) != 0)
    {
        *refIsCurrent = EVE_gpst_CurrentGAO == (gao->basic->mtx ? gao->basic->mtx->refGao2 : NULL);
        return gao->basic->mtx;
    }
    *refIsCurrent = FALSE;
    return (tGaoMatrix*)OBJ_pst_GetAbsoluteMatrix(gao);
}

static Mtx4x4* EVE_l_RefMatrix(tGameObjectMain* gao)
{
    if ((gao->flags & 0x4000000) == 0)
        return NULL;
    if ((gao->flags & 0x10000) != 0)
        return gao->matrices + 2;
    return gao->matrices + 1;
}

static void EVE_l_SelectTrack(tGameObjectEventsList* list, tAnimTracks* states, unsigned int trackIdx)
{
    EVE_gpst_CurrentTrack = &list->listElems[trackIdx];
    EVE_gpst_CurrentParam = &states[trackIdx];
    if (EVE_gpst_CurrentTrack->gao)
    {
        EVE_gpst_CurrentGAO = EVE_gpst_CurrentTrack->gao;
        EVE_gpst_CurrentRefMatrix = EVE_l_RefMatrix(EVE_gpst_CurrentGAO);
    }
    else
    {
        EVE_gpst_CurrentGAO = EVE_gpst_OwnerGAO;
    }
}

static tGameObjectEventElem* EVE_l_NextKey(void)
{
    int next;

    next = EVE_gpst_CurrentParam->currElemKeyFrame + 1;
    if (next == EVE_gpst_CurrentTrack->elemNum)
        next = 0;
    return &EVE_gpst_CurrentTrack->trackElems[next];
}

static void EVE_l_MirrorQuat(Quat4* dest, Quat4* src)
{
    dest->q4 = src->q4;
    dest->q1 = src->q1;
    dest->q2 = -src->q2;
    dest->q3 = -src->q3;
}

static void EVE_l_MirrorPos(Vec3* dest, Vec3* src)
{
    dest->x = -src->x;
    dest->y = src->y;
    dest->z = src->z;
}

void EVE_InitData(tEventData* eve, tGameObjectEventsList* list)
{
    if (eve->eventListStruct && eve->tracks)
    {
        if (eve->eventListStruct->num >= list->num)
        {
            memset(eve->tracks, 0, sizeof(tAnimTracks) * eve->eventListStruct->num);
        }
        else
        {
            eve->tracks = (tAnimTracks*)MEM_p_Realloc(eve->tracks, sizeof(tAnimTracks) * list->num);
            memset(eve->tracks, 0, sizeof(tAnimTracks) * list->num);
        }
    }
    else
    {
        if (eve->tracks)
        {
            MEM_Free(eve->tracks);
            eve->tracks = NULL;
        }
        eve->tracks = (tAnimTracks*)MEM_p_Alloc(sizeof(tAnimTracks) * list->num);
        memset(eve->tracks, 0, sizeof(tAnimTracks) * list->num);
    }
    eve->eventListStruct = list;
}

tEventData* EVE_pst_DuplicateData(tEventData* eve)
{
    int i, j;
    tGameObjectEventsListElem* newListElems;
    tEventData* newEve;
    void* newElemPtr;

    newEve = (tEventData*)MEM_p_Alloc(sizeof(tEventData));
    newEve->tracks = NULL;
    newEve->eventListStruct = NULL;

    if (eve && eve->tracks)
    {
        newEve->tracks = (tAnimTracks*)MEM_p_Alloc(sizeof(tAnimTracks) * eve->eventListStruct->num);
        memcpy(newEve->tracks, eve->tracks, sizeof(tAnimTracks) * eve->eventListStruct->num);
    }

    if (eve && eve->eventListStruct)
    {
        newEve->eventListStruct = (tGameObjectEventsList*)MEM_p_Alloc(sizeof(tGameObjectEventsList));
        memcpy(newEve->eventListStruct, eve->eventListStruct, sizeof(tGameObjectEventsList));
        if (eve->eventListStruct->listElems)
        {
            newEve->eventListStruct->listElems = (tGameObjectEventsListElem*)MEM_p_Alloc(sizeof(tGameObjectEventsListElem) * eve->eventListStruct->num);
            memcpy(newEve->eventListStruct->listElems, eve->eventListStruct->listElems, sizeof(tGameObjectEventsListElem) * eve->eventListStruct->num);

            newListElems = newEve->eventListStruct->listElems;

            for (i = 0; i < newEve->eventListStruct->num; ++i)
            {
                if (newListElems->trackElems)
                {
                    newListElems->trackElems = (tGameObjectEventElem*)MEM_p_Alloc(sizeof(tGameObjectEventElem) * newListElems->elemNum);
                    memcpy(newListElems->trackElems, eve->eventListStruct->listElems[i].trackElems, sizeof(tGameObjectEventElem) * newListElems->elemNum);
                    for (j = 0; j < newListElems->elemNum; ++j)
                    {
                        if (newListElems->trackElems[j].eptrdu.data)
                        {
                            // eptrdu is a union, so this length is the data pointer's own low half.
                            newElemPtr = MEM_p_Alloc(newListElems->trackElems[j].eptrdu.size);
                            memcpy(newElemPtr, newListElems->trackElems[j].eptrdu.data, newListElems->trackElems[j].eptrdu.size);
                            newListElems->trackElems[j].eptrdu.data = newElemPtr;
                        }
                    }
                }
                if (newListElems->dataPtr)
                {
                    newListElems->dataPtr = (uint8_t*)MEM_p_Alloc(newListElems->size);
                    memcpy(newListElems->dataPtr, eve->eventListStruct->listElems[i].dataPtr, newListElems->size);
                }
                ++newListElems;
            }
        }
    }
    return newEve;
}

void EVE_Duplicate(struct sGameObjectMain* srcGao, struct sGameObjectMain* destGao, int wholeEVEWithTrackData)
{
    tEventData* destEVE;

    if (wholeEVEWithTrackData)
    {
        destGao->extended->eve = EVE_pst_DuplicateData(srcGao->extended->eve);
    }
    else
    {
        destEVE = (tEventData*)MEM_p_Alloc(sizeof(tEventData));
        destEVE->tracks = NULL;
        destEVE->eventListStruct = NULL;
        if (srcGao->extended->eve->tracks)
        {
            destEVE->tracks = (tAnimTracks*)MEM_p_Alloc(sizeof(tAnimTracks) * srcGao->extended->eve->eventListStruct->num);
            memcpy(destEVE->tracks, srcGao->extended->eve->tracks, sizeof(tAnimTracks) * srcGao->extended->eve->eventListStruct->num);
        }
        if (srcGao->extended->eve->eventListStruct)
        {
            destEVE->eventListStruct = srcGao->extended->eve->eventListStruct;
            if (OBJ_gb_DuplicateAI)
                ++destEVE->eventListStruct->usageCount;
        }
        destGao->extended->eve = destEVE;
    }
}

BOOL EVE_b_Track_UseFlash(tGameObjectEventsListElem* elt, tGameObjectMain* currentGao, tGameObjectMain* ownerGao, tGameObjectEventsList* currentListTracks)
{
    tGameObjectEventsList* clt;
    tGameObjectMain* owner;
    tGameObjectMain* gao;
    tGameObjectEventsListElem* currListElem;

    if (currentGao)
        gao = currentGao;
    else
        gao = EVE_gpst_CurrentGAO;

    if (ownerGao)
        owner = ownerGao;
    else
        owner = EVE_gpst_OwnerGAO;

    if (currentListTracks)
        clt = currentListTracks;
    else
        clt = EVE_gpst_CurrentListTracks;

    for (currListElem = clt->listElems; currListElem <= elt; ++currListElem)
    {
        if ((currListElem->gao || gao == owner)
            && (!currListElem->gao || currListElem->gao == gao)
            && currListElem->trackElems
            && (currListElem->trackElems->flags2 & 0x7C0) == 0x80
            && (currListElem->trackElems->flags2 & 4) != 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

tGameObjectEventElem* EVE_Event_AIFunction_Play(tGameObjectEventElem* key)
{
    tGameObjectActorObject* actor;
    tAIFunction* savedFunction;
    tGameObjectMain* savedGao;
    tAI* savedInstance;
    tGameObjectMain* bone;

    if (key->eptrdu.data)
    {
        AI_gul_SizeGlobalVarStack = 0;
        AI_gul_SizeGlobalStack = 0;
        savedFunction = AI_gpst_CurrentFunction;
        savedInstance = AI_gpst_CurrentInstance;
        savedGao = AI_gpst_CurrentGameObject;
        AI_gpst_CurrentFunction = 0;
        AI_gpst_CurrentInstanceUltra = 0;
        AI_gpst_CurrentInstance = 0;
        if (EVE_gpst_CurrentTrack->boneID == 0xFFFF)
        {
            if (EVE_gpst_CurrentTrack->gao)
            {
                AI_gpst_CurrentUltra = EVE_gpst_CurrentTrack->gao;
                AI_gpst_CurrentGameObject = AI_gpst_CurrentUltra;
            }
            else
            {
                AI_gpst_CurrentUltra = EVE_gpst_OwnerGAO;
                AI_gpst_CurrentGameObject = EVE_gpst_OwnerGAO;
            }
        }
        else
        {
            // These two returns leave the AI globals zeroed above unrestored.
            if (!EVE_gpst_CurrentTrack->gao
                || (EVE_gpst_CurrentTrack->gao->flags & 2) == 0
                || !EVE_gpst_CurrentTrack->gao->basic
                || !EVE_gpst_CurrentTrack->gao->basic->actorObjectOrVisualObject
                || !EVE_l_Actor(EVE_gpst_CurrentTrack->gao)->skeletonGroup)
            {
                return key;
            }
            actor = EVE_l_Actor(EVE_gpst_CurrentTrack->gao);
            bone = EVE_l_Bone(actor, EVE_gpst_CurrentTrack->boneID);
            if (!bone)
                return key;
            AI_gpst_CurrentUltra = bone;
            AI_gpst_CurrentGameObject = AI_gpst_CurrentUltra;
        }
        AI_RunNode((tAINode*)(((uint8_t*)key->eptrdu.data) + 4));

        if (AI_gul_SizeGlobalStack)
            EVE_gpst_CurrentParam->privateData = *(uint32_t*)AI_gac_GlobalStack;
        AI_gpst_CurrentFunction = savedFunction;
        AI_gpst_CurrentInstanceUltra = savedInstance;
        AI_gpst_CurrentInstance = savedInstance;
        AI_gpst_CurrentUltra = savedGao;
        AI_gpst_CurrentGameObject = savedGao;
    }
    return key;
}

int EVE_i_SearchLabel(struct sGameObjectEventsListElem* track, int label, float* labelTime)
{
    float elapsed;
    int i;

    elapsed = 0.0;
    for (i = 0; i < track->elemNum; ++i)
    {
        if ((track->trackElems[i].flags2 & 0xF000) == label
            && (track->trackElems[i].flags2 & 0x7C0) != 0x200
            && (track->trackElems[i].flags2 & 0x7C0) != 0x400)
        {
            *labelTime = elapsed;
            return i;
        }
        elapsed = track->trackElems[i].duration * 1.0f / 60.0f + elapsed;
    }
    return -1;
}

int EVE_i_SearchTime(tGameObjectEventsListElem* track, float* time)
{
    float keyTime;
    float remaining;
    int i;

    remaining = *time;
    for (i = 0; i < track->elemNum; ++i)
    {
        keyTime = (float)EVE_l_FrameTime(track->trackElems[i].duration);
        if (keyTime >= (double)remaining)
        {
            if (keyTime - remaining >= 0.016656667)
            {
                *time = remaining;
            }
            else
            {
                *time = 0.0;
                ++i;
            }
            return i;
        }
        remaining = (float)(remaining - EVE_l_FrameTime(track->trackElems[i].duration));
    }
    if (track->trackElems)
    {
        *time = (float)EVE_l_FrameTime(track->trackElems[track->elemNum - 1].duration);
        return track->elemNum - 1;
    }
    *time = 0.0;
    return 0;
}

void EVE_SetAllTimesTo(float time)
{
    int keyFrame;
    float trackTime;
    int i;

    for (i = 0; i < EVE_gpst_CurrentListTracks->num; ++i)
    {
        trackTime = time;
        keyFrame = EVE_i_SearchTime(&EVE_gpst_CurrentListTracks->listElems[i], &trackTime);
        if (keyFrame != -1)
        {
            EVE_gpst_OwnerGAO->extended->eve->tracks[i].flag &= ~4u;
            EVE_gpst_OwnerGAO->extended->eve->tracks[i].currElemKeyFrame = keyFrame;
            EVE_gpst_OwnerGAO->extended->eve->tracks[i].currTime = trackTime;
        }
    }
}

void EVE_RunningTracksAtStart(tEventData* eve)
{
    int i;

    if (eve)
    {
        for (i = 0; i < eve->eventListStruct->num; ++i)
        {
            if ((eve->tracks[i].flag & 1) != 0)
            {
                eve->tracks[i].flag &= ~4u;
                eve->tracks[i].currElemKeyFrame = 0;
                eve->tracks[i].currTime = 0.0;
            }
        }
    }
}

void EVE_SetRunningTrack(struct sAnimTracks* state, int running)
{
    if (running)
    {
        if ((state->flag & 1) == 0)
        {
            state->currTime = 0.0;
            state->flag |= 1u;
            state->flag &= ~2u;
            state->flag &= ~4u;
            state->currElemKeyFrame = 0;
        }
    }
    else
    {
        state->flag &= ~1u;
    }
}

void EVE_SetRunningTracks(struct sEventData* eve, int running)
{
    uint16_t i;

    if (eve)
    {
        for (i = 0; i < (int)eve->eventListStruct->num; ++i)
            EVE_SetRunningTrack(&eve->tracks[i], running);
    }
}

void EVE_SetPauseTrack(struct sAnimTracks* state, int paused)
{
    if (paused)
    {
        if ((state->flag & 2) == 0)
            state->flag |= 2u;
    }
    else
    {
        state->flag &= ~2u;
    }
}

void EVE_SetPauseTracks(struct sEventData* eve, int paused)
{
    uint16_t i;

    for (i = 0; i < (int)eve->eventListStruct->num; ++i)
        EVE_SetPauseTrack(&eve->tracks[i], paused);
}

void EVE_FlashMatrix()
{
    Mtx4x4* mtx;

    if (EVE_gb_CanFlash && EVE_gpst_CurrentListTracks && EVE_gpst_CurrentGAO)
    {
        mtx = EVE_l_CurrentMatrix(EVE_gpst_CurrentGAO);
        if (EVE_gpst_CurrentRefMatrix)
        {
            if (EVE_gpst_CurrentRefMatrix != mtx)
                memcpy(EVE_gpst_CurrentRefMatrix, mtx, sizeof(Mtx4x4));
        }
        else
        {
            OBJ_ReinitMatrixStorageByFlags(EVE_gpst_CurrentGAO, mtx);
        }
    }
}

void EVE_GetTracksTime(struct sEventData* eve, int trackIdx, float* currTime, float* fullTime)
{
    tGameObjectEventElem* key;
    tGameObjectEventElem* end;

    if (currTime)
        *currTime = 0.0;
    if (fullTime)
        *fullTime = 0.0;
    if (eve && eve->eventListStruct && eve->eventListStruct->listElems && eve->eventListStruct->listElems[trackIdx].trackElems)
    {
        if (currTime)
        {
            key = eve->eventListStruct->listElems[trackIdx].trackElems;
            end = &key[eve->tracks[trackIdx].currElemKeyFrame];
            while (key < end)
            {
                *currTime = (float)(EVE_l_FrameTime(key->duration) + *currTime);
                ++key;
            }
            if (fullTime)
            {
                *fullTime = *currTime;
                end = &eve->eventListStruct->listElems[trackIdx].trackElems[eve->eventListStruct->listElems[trackIdx].elemNum];
                while (key < end)
                {
                    *fullTime = (float)(EVE_l_FrameTime(key->duration) + *fullTime);
                    ++key;
                }
            }
            *currTime = *currTime + eve->tracks[trackIdx].currTime;
        }
        else if (fullTime)
        {
            key = eve->eventListStruct->listElems[trackIdx].trackElems;
            end = &key[eve->eventListStruct->listElems[trackIdx].elemNum];
            while (key < end)
            {
                *fullTime = (float)(EVE_l_FrameTime(key->duration) + *fullTime);
                ++key;
            }
        }
    }
}

void EVE_SetTracksTime(struct sEventData* eve, float time)
{
    float fraction;
    tGameObjectEventElem* key;
    tGameObjectEventElem* keyEnd;
    tGameObjectEventsListElem* track;
    uint16_t frames;
    tGameObjectEventsListElem* trackEnd;
    tAnimTracks* state;

    track = eve->eventListStruct->listElems;
    trackEnd = &track[eve->eventListStruct->num];
    state = eve->tracks;
    while (track < trackEnd)
    {
        frames = (uint16_t)(time * 60.0);
        key = track->trackElems;
        keyEnd = &key[track->elemNum];
        state->currElemKeyFrame = 0;
        while (key < keyEnd)
        {
            if (frames < (int)key->duration)
            {
                fraction = (float)(time - (uint16_t)(time * 60.0) / 60.0);
                state->currTime = (float)(frames / 60.0 + fraction);
                state->flag |= 4u;
                break;
            }
            frames -= key->duration;
            state->currTime = 0.0;
            state->flag |= 4u;
            ++key;
            ++state->currElemKeyFrame;
        }
        if (state->currElemKeyFrame == track->elemNum)
            state->currElemKeyFrame = track->elemNum - 1;
        ++track;
        ++state;
    }
}

void EVE_SynchronizeTracks(tGameObjectActorObject* actor)
{
    tEventData* eve;
    float currTime;

    eve = actor->animPtr[0]->ptr;
    EVE_GetTracksTime(eve, 0, &currTime, 0);
    EVE_SetTracksTime(eve, currTime);
    if (actor->flagsEgBlendIw2 != 1)
        actor->flagsEgBlendIw2 = 1;
}

void EVE_FreeTrack(tGameObjectEventsListElem* track)
{
    if (track->trackElems)
    {
        if (track->dataPtr)
            MEM_Free(track->dataPtr);
        MEM_Free(track->trackElems);
        track->trackElems = 0;
        track->dataPtr = 0;
    }
}

void EVE_DeleteListTracks(tGameObjectEventsList* list)
{
    unsigned int i;

    if (list)
    {
        if (ENG_gb_ExitApplication && list->usageCount > 1000000u)
            list->usageCount -= 1000000;
        if (!--list->usageCount || list->usageCount == -1)
        {
            for (i = 0; i < list->num; ++i)
                EVE_FreeTrack(&list->listElems[i]);
            if (list->listElems)
                MEM_Free(list->listElems);
            LOA_DeleteAddress((int)list);
            MEM_Free(list);
        }
    }
}

void EVE_FreeMainStruct(tGameObjectMain* gao)
{
    tEventData* eve;

    eve = gao->extended->eve;
    if (eve)
    {
        if (eve->tracks)
            MEM_Free(eve->tracks);
        if (eve->eventListStruct)
            EVE_DeleteListTracks(eve->eventListStruct);
        MEM_Free(eve);
        gao->extended->eve = 0;
    }
}

/*
elemPtr felépítése [keyframe]
0..1 [short] a dataptr pozíciója, ahol az adatunk található, ha páratlan szám, akkor pozicionálnuk kell a dataptr [(short >> 1) - 4 * keyframeID] helyre
2..3 [short] flags
.... a flagnek megfelelõ többi adat, vektorok, quaternionok, mátrixok
....
vagy az elemPtr egy intként van használva ami két részbõl áll: alsó short rész: (2 x hossz | 1) felsõ rész, flagek
*/

uint8_t* EVE_p_GetPdata(tGameObjectEventElem* key)
{
    if ((((uint32_t)key->eptrdu.size) & 0x1) != 0) // &1 != 0: az elemptr nem mutat sehova, hanem intként van használva és ebbõl kell kiszámolni a ptr-t
        return &EVE_gpst_CurrentTrack->dataPtr[((key->eptrdu.size >> 1) - 4) * (key - EVE_gpst_CurrentTrack->trackElems)];
    return ((uint8_t*)key->eptrdu.data) + 4;
}

uint16_t EVE_w_Event_InterpolationKey_GetType(struct sGameObjectEventElem* key)
{
    // ha az elemptr elso ket bajta & 0x1 != 0, akkor csak a felso ket bajt kell nekunk, ami a tipus
    // ha nem, es az adatterulet amire az elemptr mutat nem 0, akkor az egy memoriacim, amit ha 2-vel eltolunk (mivel a felso 2 bajt kell [tipus-ra mutat]), megkapjuk
    if ((key->eptrdu.size & 0x1) != 0)
        return key->eptrdu.type;
    if (key->eptrdu.data)
        return *((uint16_t*)key->eptrdu.data + 1);

    return 0;
}

Quat4* EVE_pst_Event_InterpolationKey_GetQuaternion(tGameObjectEventElem* key)
{
    int type;
    uint8_t* data;

    // type is int, so the 0xFFFF-means-no-key test below can never fire.
    type = EVE_w_Event_InterpolationKey_GetType(key);
    if (type == -1 || (type & 0x10) == 0)
        return 0;
    data = EVE_p_GetPdata(key);
    data += sizeof(Vec3) * (type & 0x3);
    return (Quat4*)data;
}

int EVE_Anim_InterpolationKey_CopyMatrix(Mtx4x4* dest, tGameObjectEventElem* key)
{
    MATH_ConvertQuaternionToMatrix(dest, EVE_pst_Event_InterpolationKey_GetQuaternion(key));
    dest->mtxflag |= 4;
    return dest->mtxflag;
}

uint8_t* EVE_pst_Event_InterpolationKey_GetNextValue(tGameObjectEventElem* key)
{
    int type;
    uint8_t* data;

    // type is int, so the 0xFFFF-means-no-key test below can never fire.
    type = EVE_w_Event_InterpolationKey_GetType(key);
    if (type == -1 || (type & 0x40) == 0)
        return 0;
    data = EVE_p_GetPdata(key);
    if ((type & 3) != 0)
    {
        data += sizeof(Vec3) * (type & 3);
    }
    else if ((type & 4) != 0)
    {
        data += 68; // Mtx4x4, de az utolsó 3 int értelmetlenséget nem tárolja le, nézd a Load metódust, FIXME: meg kéne nézni a loadnál, hogy nem-e írjuk túl a buffert, 
        //mivel a ReadMatrix mind a 80 bájtot beolvassa és leteszi, mi viszont csak kevesebbel növeljük az offsetet így azok az adatok felülíródnak
    }
    else if ((type & 0x10) != 0)
    {
        data += sizeof(Quat4);
    }
    return data;
}

Mtx4x4* EVE_pst_Event_InterpolationKey_GetRotation(tGameObjectEventElem* key)
{
    int type;
    uint8_t* data;

    // type is int, so the 0xFFFF-means-no-key test below can never fire.
    type = EVE_w_Event_InterpolationKey_GetType(key);
    if (type == -1 || (type & 4) == 0)
        return 0;
    data = EVE_p_GetPdata(key);
    data += sizeof(Vec3) * (type & 3);
    return (Mtx4x4*)data;
}

void EVE_Event_InterpolationKey_CopyMatrix(Mtx4x4* dest, tGameObjectEventElem* key, int useNextValue)
{
    Mtx4x4* src;
    Quat4* quat;
    Quat4 mirrored;
    int16_t type;
    int mirror;

    type = EVE_w_Event_InterpolationKey_GetType(key);
    if ((type & 0x14) != 0)
    {
        mirror = key->flags2 & 0x20;
        if ((type & 4) != 0)
        {
            if (useNextValue)
                src = (Mtx4x4*)EVE_pst_Event_InterpolationKey_GetNextValue(key);
            else
                src = (Mtx4x4*)EVE_pst_Event_InterpolationKey_GetRotation(key);
            if (dest != src)
                memcpy(dest, src, sizeof(Mtx4x4));
            if (mirror)
            {
                MATH_ConvertMatrixToQuaternion(&mirrored, dest);
                mirrored.q2 = -mirrored.q2;
                mirrored.q3 = -mirrored.q3;
                MATH_ConvertQuaternionToMatrix(dest, &mirrored);
            }
        }
        else if ((type & 0x10) != 0)
        {
            if (useNextValue)
                quat = (Quat4*)EVE_pst_Event_InterpolationKey_GetNextValue(key);
            else
                quat = EVE_pst_Event_InterpolationKey_GetQuaternion(key);
            if (mirror)
            {
                quat->q2 = -quat->q2;
                quat->q3 = -quat->q3;
            }
            MATH_ConvertQuaternionToMatrix(dest, quat);
            if (mirror)
            {
                quat->q2 = -quat->q2;
                quat->q3 = -quat->q3;
            }
            MATH_SetCorrectType(dest);
        }
    }
}

uint8_t* EVE_pf_Event_InterpolationKey_GetTime(tGameObjectEventElem* key)
{
    int type;
    uint8_t* data;

    // type is int, so the 0xFFFF-means-no-key test below can never fire.
    type = EVE_w_Event_InterpolationKey_GetType(key);
    if (type == -1 || (type & 8) == 0)
        return 0;
    data = EVE_p_GetPdata(key);
    data += sizeof(Vec3) * (type & 3);
    if ((type & 4) != 0)
        data += 68; // Mtx4x4 a felesleges byteok nélkül
    if ((type & 0x10) != 0)
        data += sizeof(Quat4);
    return data;
}

float EVE_f_Event_InterpolationKey_InterpolateTime(tGameObjectEventElem* key, float time)
{
    tEventTimeElem* times;
    tEventTimeElem* cursor;
    tEventTimeElem* last;
    float elapsed;
    float result;
    uint32_t count;

    times = (tEventTimeElem*)EVE_pf_Event_InterpolationKey_GetTime(key);
    cursor = times;
    count = *(uint32_t*)&(times->start);
    times->start = 0.0;
    last = &times[count - 1];
    elapsed = 0.0;
    while (cursor < last && cursor[1].start <= (double)time)
    {
        elapsed = (cursor[1].start - cursor->start) * cursor->multiplicator + elapsed;
        ++cursor;
    }
    result = (time - cursor->start) * cursor->multiplicator + elapsed;
    *(uint32_t*)&(times->start) = count;
    return result;
}

void EVE_Event_AIFunction_Resolve(tGameObjectEventElem* key)
{
    tAINode* node;

    if (key->eptrdu.data)
    {
        node = (tAINode*)(((char*)key->eptrdu.data) + 4);
        if (key->eptrdu.data != (void*)-4)
        {
            while (node->category != 31)
            {
                if (node->category == 1)
                {
                    node->val = AI_ul_ResolveOneRef(node->val2, node->val);
                    AI_b_IsARef(node->val2);
                }
                if (!AI_SpecialWParam(node->category))
                    node->val2 = AI_gaw_EnumLink[node->val2];
                node->category = (char)AI_gaw_EnumLink[node->category];
                ++node;
            }
        }
    }
}

int EVE_Event_AIFunction_Load(tGameObjectEventElem* key, char* src, void* elemPtr)
{
    tAINode* node;
    unsigned int i;
    unsigned int nodeNum;
    char* cursor;

    cursor = src;
    nodeNum = LOA_ReadLong(&cursor, 0, 1);
    if (nodeNum)
    {
        key->eptrdu.data = elemPtr;
        node = (tAINode*)(((char*)(key->eptrdu.data)) + 4);
        for (i = 0; i < nodeNum; ++i)
            LOA_ReadAINode(&cursor, node++, 1);
        *((uint16_t*)(key->eptrdu.data)) = (uint16_t)(cursor - src);
    }
    return cursor - src;
}

void EVE_Anim_ComputeInterpoledMatrix(tGameObjectMain* gao, Mtx4x4* dest, tGameObjectEventElem* key, tGameObjectEventElem* nextKey, float time)
{
    Quat4* quat;
    Quat4* nextQuat;
    float blend;

    if (key->duration)
    {
        if (EVE_l_FrameTime(key->duration) > time)
        {
            quat = EVE_pst_Event_InterpolationKey_GetQuaternion(key);
            nextQuat = EVE_pst_Event_InterpolationKey_GetQuaternion(nextKey);
            blend = (float)(time / EVE_l_FrameTime(key->duration));
            MATH_QuaternionBlend(dest, quat, nextQuat, blend, 1u);
        }
        else
        {
            EVE_Anim_InterpolationKey_CopyMatrix(dest, nextKey);
        }
    }
    else
    {
        EVE_Anim_InterpolationKey_CopyMatrix(dest, key);
    }
}

void EVE_Event_ComputeInterpoledMatrix(tGameObjectMain* gao, Mtx4x4* dest, tGameObjectEventElem* key, tGameObjectEventElem* nextKey, float time)
{
    Mtx4x4* rot;
    float blend;
    int16_t type;
    int16_t nextType;
    Quat4 quat;
    Quat4 nextQuat;
    Quat4 mirrored;
    Quat4 mirroredNext;
    Quat4* from;
    Quat4* to;

    type = EVE_w_Event_InterpolationKey_GetType(key);
    nextType = EVE_w_Event_InterpolationKey_GetType(nextKey);
    if (type != -1 && (type & 0x14) != 0)
    {
        if (!key->duration || nextType == -1 || (nextType & 0x14) == 0 || (key->flags2 & 0x800) != 0)
        {
            EVE_Event_InterpolationKey_CopyMatrix(dest, key, 0);
        }
        else if (EVE_l_FrameTime(key->duration) > time)
        {
            if ((type & 4) != 0)
            {
                rot = (Mtx4x4*)EVE_pst_Event_InterpolationKey_GetRotation(key);
                MATH_ConvertMatrixToQuaternion(&quat, rot);
                from = &quat;
            }
            else                                    // EVE_Event_InterpolationKey_CopyMatrix (,,1) és (,,0) inline 2 db
            {
                from = EVE_pst_Event_InterpolationKey_GetQuaternion(key);
            }
            if ((key->flags2 & 0x20) != 0)
            {
                EVE_l_MirrorQuat(&mirrored, from);
                from = &mirrored;
            }
            if ((type & 0x40) != 0)
            {
                if ((type & 4) != 0)
                {
                    rot = (Mtx4x4*)EVE_pst_Event_InterpolationKey_GetNextValue(key);
                    MATH_ConvertMatrixToQuaternion(&nextQuat, rot);
                    to = &nextQuat;
                }
                else
                {
                    to = (Quat4*)EVE_pst_Event_InterpolationKey_GetNextValue(key);
                }
            }
            else if ((nextType & 4) != 0)
            {
                rot = (Mtx4x4*)EVE_pst_Event_InterpolationKey_GetRotation(nextKey);
                MATH_ConvertMatrixToQuaternion(&nextQuat, rot);
                to = &nextQuat;
            }
            else
            {
                to = EVE_pst_Event_InterpolationKey_GetQuaternion(nextKey);
            }
            if ((nextKey->flags2 & 0x20) != 0)
            {
                EVE_l_MirrorQuat(&mirroredNext, to);
                to = &mirroredNext;
            }
            blend = (float)(time / EVE_l_FrameTime(key->duration));
            MATH_QuaternionBlend(dest, from, to, blend, 1u);
        }
        else if ((type & 0x40) != 0)
        {
            EVE_Event_InterpolationKey_CopyMatrix(dest, key, 1);
        }
        else
        {
            EVE_Event_InterpolationKey_CopyMatrix(dest, nextKey, 0);
        }
    }
}

Vec3* EVE_pst_Event_InterpolationKey_GetPos(tGameObjectEventElem* key)
{
    int type;

    // type is int, so the 0xFFFF-means-no-key test below can never fire.
    type = EVE_w_Event_InterpolationKey_GetType(key);
    if (type != -1 && (type & 3) != 0)
        return (Vec3*)EVE_p_GetPdata(key);
    return 0;
}

void EVE_Anim_TranslationKey_Play(tGameObjectEventElem* key)
{
    Mtx4x4 refRelative;
    Vec3 pos;
    Vec3 scaled;
    Vec3* keyPos;
    Vec3* nextPos;
    float blend;
    BOOL refIsCurrent;
    tGaoMatrix* mtx;
    tGameObjectActorObject* actor;
    tGameObjectMain* target;
    BOOL onBone;
    float keyTime;
    tGameObjectEventElem* nextKey;

    nextKey = EVE_l_NextKey();
    keyTime = EVE_gpst_CurrentParam->currTime;
    onBone = EVE_gpst_CurrentTrack->boneID != 0xFFFF;
    if (onBone)
    {
        actor = EVE_l_Actor(EVE_gpst_CurrentGAO);
        target = EVE_l_Bone(actor, EVE_gpst_CurrentTrack->boneID);
        if (!target)
            return;
        if (target->flags2 < 0
            || target->extended
            && (actor->lod & 8) != 0
            && (target->extended->extraFlag & 0x20) != 0)
        {
            return;
        }
        if (target->extended && (target->extended->extraFlag & 0x200) != 0)
            return;
    }
    else
    {
        target = EVE_gpst_CurrentGAO;
    }
    mtx = EVE_l_TargetMatrix(target, &refIsCurrent);
    keyPos = EVE_pst_Event_InterpolationKey_GetPos(key);
    if (key->duration)
    {
        nextPos = EVE_pst_Event_InterpolationKey_GetPos(nextKey);
        blend = (float)(keyTime / EVE_l_FrameTime(key->duration));
        MATH_VecMultScalar(&pos, keyPos, (float)(1.0 - blend));
        MATH_VecMultScalar(&scaled, nextPos, blend);
        MATH_VecAddVec(&pos, &pos, &scaled);
    }
    else
    {
        pos = *keyPos;
    }
    if (onBone)
    {
        if (refIsCurrent && EVE_gpst_CurrentRefMatrix)
        {
            EVE_l_RefRelativeToCurrent(&refRelative);
            MATH_TransformVertex(&pos, &refRelative, &pos);
            if ((EVE_gpst_CurrentGAO->flags & 2) != 0)
                pos.z = pos.z + EVE_l_Actor(EVE_gpst_CurrentGAO)->aniPelvisOffset;
        }
        mtx->mtx.Pos = pos;
        mtx->mtx.mtxflag |= 2u;
        mtx->mtx.mtxflag |= 2u;
    }
    else
    {
        MATH_TransformVertex(&pos, EVE_gpst_CurrentRefMatrix, &pos);
        mtx->mtx.Pos = pos;
        mtx->mtx.mtxflag |= 2u;
    }
}

void EVE_Anim_QuaternionKey_Play(tGameObjectEventElem* key)
{
    Mtx4x4 refRelative;
    Mtx4x4 rot;
    Mtx4x4 interpolated;
    BOOL refIsCurrent;
    tGaoMatrix* mtx;
    tGameObjectActorObject* actor;
    tGameObjectMain* target;
    BOOL onBone;
    float keyTime;
    tGameObjectEventElem* nextKey;

    nextKey = EVE_l_NextKey();
    keyTime = EVE_gpst_CurrentParam->currTime;
    onBone = EVE_gpst_CurrentTrack->boneID != 0xFFFF;
    if (onBone)
    {
        actor = EVE_l_Actor(EVE_gpst_CurrentGAO);
        target = EVE_l_Bone(actor, EVE_gpst_CurrentTrack->boneID);
        if (!target)
            return;
        if (target->flags2 < 0
            || target->extended
            && (actor->lod & 8) != 0
            && (target->extended->extraFlag & 0x20) != 0)
        {
            return;
        }
        if (target->extended && (target->extended->extraFlag & 0x400) != 0)
            return;
    }
    else
    {
        target = EVE_gpst_CurrentGAO;
    }
    mtx = EVE_l_TargetMatrix(target, &refIsCurrent);
    EVE_Anim_ComputeInterpoledMatrix(target, &interpolated, key, nextKey, keyTime);
    if (onBone)
    {
        if (refIsCurrent && EVE_gpst_CurrentRefMatrix)
        {
            EVE_l_RefRelativeToCurrent(&refRelative);
            MATH_Mul33MatrixMatrix(&rot, &interpolated, &refRelative, 1);
        }
        else
        {
            MATH_Copy33Matrix(&rot, &interpolated);
        }
        MATH_Copy33Matrix(&mtx->mtx, &rot);
    }
    else
    {
        MATH_Mul33MatrixMatrix(&rot, &interpolated, EVE_gpst_CurrentRefMatrix, 1);
        MATH_Copy33Matrix(&mtx->mtx, &rot);
    }
    mtx->mtx.mtxflag |= 4u;
}

int EVE_RunAnimTrack(tGameObjectEventsListElem* track, tAnimTracks* state, unsigned int trackIdx, char lod, float* dtLeft)
{
    float left;
    float keyTime;
    float remaining;
    tGameObjectEventElem* key;

    if (state->currElemKeyFrame == track->elemNum - 1)
    {
        if (track->boneID == 255)
        {
            EVE_RunningTracksAtStart(EVE_gpst_CurrentData);
        }
        else
        {
            state->flag &= ~4u;
            state->currElemKeyFrame = 0;
            state->currTime = 0.0;
        }
    }
    remaining = TIM_gf_dt;
    while (1)
    {
        key = &track->trackElems[state->currElemKeyFrame];
        EVE_gpst_CurrentEvent = key;
        keyTime = key->duration * 1.0f / 60.0f;
        if ((key->flags2 & 4) != 0 && (state->flag & 4) == 0)
            EVE_FlashMatrix();
        if (keyTime - 0.0000099999997f >= remaining + state->currTime)
            break;
        left = remaining - (keyTime - state->currTime);
        if (left <= 0.0f)
            remaining = 0.0f;
        else
            remaining = left;
        if (state->currElemKeyFrame == track->elemNum - 1)
        {
            if ((EVE_w_Event_InterpolationKey_GetType(key) & 0x14) != 0)
                EVE_Anim_QuaternionKey_Play(key);
            else
                EVE_Anim_TranslationKey_Play(key);
            state->currElemKeyFrame = track->elemNum - 1;
            state->currTime = 0.0;
            state->flag &= ~0x4;
            *dtLeft = remaining;
            return 1;
        }
        state->currTime = 0.0;
        ++state->currElemKeyFrame;
        state->flag &= ~4u;
    }
    state->currTime = remaining + state->currTime;
    if (trackIdx > 1 && (lod & 2) != 0 && (lod & 0x10) != 0)
    {
        state->flag |= 4u;
    }
    else
    {
        if ((EVE_w_Event_InterpolationKey_GetType(key) & 0x14) != 0)
            EVE_Anim_QuaternionKey_Play(key);
        else
            EVE_Anim_TranslationKey_Play(key);

        state->flag |= 4u;
    }
    return 0;
}

uint8_t EVE_c_UpdateLOD(tGameObjectMain* gao, int toggleLOD)
{
    uint8_t prevLOD;
    tGameObjectActorObject* actor;

    actor = EVE_l_Actor(gao);
    prevLOD = actor->lod & 0xEF;
    if (toggleLOD)
        actor->lod = (actor->lod & 0x10) != 0 ? 0 : 16;
    if (actor->ANILOD)
    {
        actor->lod |= actor->ANILOD | 0x80;
    }
    else
    {
        actor->lod |= actor->aniBitFieldLOD & 0x80;
        if (gao->distance < 0xAu)
            actor->lod |= actor->aniBitFieldLOD & 1;
        if (gao->distance < 0x19u)
            actor->lod |= actor->aniBitFieldLOD & 2;
        if (gao->distance < 0x64u)
            actor->lod |= actor->aniBitFieldLOD & 0xC;
        if (gao->extended && (gao->extended->extraFlag & 0x80) != 0)
            actor->lod |= 4u;
    }
    if (prevLOD > (actor->lod & 0xEF))
        EVE_SynchronizeTracks(actor);
    return actor->lod;
}

tGameObjectEventElem* EVE_Event_InterpolationKey_Play(tGameObjectEventElem* key)
{
    Mtx4x4 refRelative;
    Mtx4x4 interpolated;
    Mtx4x4 rot;
    Mtx4x4 targetGlobal;
    Mtx4x4 fatherGlobal;
    Mtx4x4 grandFatherGlobal;
    Mtx4x4 refGlobal;
    Mtx4x4 refInverse;
    Mtx4x4* m;
    tGameObjectMain* ref;
    tGameObjectMain* grandFather;
    tGameObjectMain* father = NULL; // TODO ?????
    float fatherLen;
    float targetLen;
    Vec3 pos;
    Vec3 scaled;
    Vec3 mirroredPos;
    Vec3 mirroredNextPos;
    Vec3* keyPos;
    Vec3* nextPos;
    float blend;
    float weight;
    float keyTime;
    int16_t nextType;
    tGameObjectEventElem* nextKey;
    BOOL refIsCurrent;
    tGaoMatrix* mtx;
    int isLocalMtx;
    int mirror;
    tGameObjectMain* newRef;
    tGameObjectMain* target;
    tGameObjectActorObject* actor;
    BOOL onBone;
    int16_t type;

    if (!EVE_gpst_CurrentTrack)
        return key;
    type = EVE_w_Event_InterpolationKey_GetType(key);
    if (type == -1)
        return key;
    onBone = EVE_gpst_CurrentTrack->boneID != 0xFFFF;
    if (onBone)
    {
        if (!EVE_gpst_CurrentGAO->basic
            || !EVE_gpst_CurrentGAO->basic->actorObjectOrVisualObject
            || !EVE_l_Actor(EVE_gpst_CurrentGAO)->skeletonGroup)
        {
            return key;
        }
        actor = EVE_l_Actor(EVE_gpst_CurrentGAO);
        target = EVE_l_Bone(actor, EVE_gpst_CurrentTrack->boneID);
        if (!target)
            return key;
        if (!ENG_gb_EVERunning && target->flags2 < 0)
            return key;
    }
    else
    {
        target = EVE_gpst_CurrentGAO;
    }
    isLocalMtx = target->flags & 0x400000;
    mirror = type & 0x20;
    mtx = EVE_l_TargetMatrix(target, &refIsCurrent);
    nextKey = EVE_l_NextKey();
    if ((nextKey->flags2 & 0x7C0) != 0)
    {
        if ((nextKey->flags2 & 0x7C0) != 128)
            return key;
        nextType = EVE_w_Event_InterpolationKey_GetType(nextKey);
        if (nextType == -1)
            return key;
    }
    else
    {
        nextType = 0;
    }

    if ((type & 8) != 0)
        keyTime = EVE_f_Event_InterpolationKey_InterpolateTime(key, EVE_gpst_CurrentParam->currTime);
    else
        keyTime = EVE_gpst_CurrentParam->currTime;
    if (onBone
        && isLocalMtx
        && (mirror && target->basic->mtx->refGao2 != EVE_gpst_CurrentGAO
            || !mirror && target->basic->mtx->refGao2 != target->basic->mtx->refGao))
    {
        if (mirror)
            newRef = EVE_gpst_CurrentGAO;
        else
            newRef = target->basic->mtx->refGao;
        target->basic->mtx->refGao2 = newRef;
        OBJ_ComputeLocalWhenHie(target);
    }
    if ((type & 3) != 0)
    {
        keyPos = EVE_pst_Event_InterpolationKey_GetPos(key);
        if ((key->flags2 & 0x20) != 0)
        {
            EVE_l_MirrorPos(&mirroredPos, keyPos);
            keyPos = &mirroredPos;
            // keyPos[1] and keyPos[2] below then read past this one-vector local.
        }
        if ((nextType & 3) != 0 && key->duration && (key->flags2 & 0x800) == 0)
        {
            if ((type & 0x40) != 0)
                nextPos = (Vec3*)EVE_pst_Event_InterpolationKey_GetNextValue(key);
            else
                nextPos = (Vec3*)EVE_pst_Event_InterpolationKey_GetPos(nextKey);
            if ((nextKey->flags2 & 0x20) != 0)
            {
                EVE_l_MirrorPos(&mirroredNextPos, nextPos);
                nextPos = &mirroredNextPos;
            }
            if (EVE_l_FrameTime(key->duration) > keyTime)
            {
                blend = (float)(keyTime / EVE_l_FrameTime(key->duration));
                if ((type & 3) == 3)
                {
                    weight = (float)((1.0 - blend) * ((1.0 - blend) * (1.0 - blend)));
                    MATH_VecMultScalar(&pos, keyPos, weight);
                    weight = (float)((1.0 - blend) * ((1.0 - blend) * (3.0 * blend)));
                    MATH_VecMultScalar(&scaled, &keyPos[1], weight);               // addscalevector inlineok
                    MATH_VecAddVec(&pos, &pos, &scaled);
                    weight = (float)((1.0 - blend) * (3.0 * blend * blend));
                    MATH_VecMultScalar(&scaled, &keyPos[2], weight);
                    MATH_VecAddVec(&pos, &pos, &scaled);
                    weight = blend * blend * blend;
                    MATH_VecMultScalar(&scaled, nextPos, weight);
                    MATH_VecAddVec(&pos, &pos, &scaled);
                }
                else if ((type & 3) == 2)
                {
                    weight = (float)((1.0 - blend) * (1.0 - blend));
                    MATH_VecMultScalar(&pos, keyPos, weight);
                    weight = (float)((1.0 - blend) * (2.0 * blend));
                    MATH_VecMultScalar(&scaled, &keyPos[1], weight);
                    MATH_VecAddVec(&pos, &pos, &scaled);
                    weight = blend * blend;
                    MATH_VecMultScalar(&scaled, nextPos, weight);
                    MATH_VecAddVec(&pos, &pos, &scaled);
                }
                else
                {
                    weight = (float)(1.0 - blend);
                    MATH_VecMultScalar(&pos, keyPos, weight);
                    MATH_VecMultScalar(&scaled, nextPos, blend);
                    MATH_VecAddVec(&pos, &pos, &scaled);
                }
            }
            else
            {
                pos = *nextPos;
            }
        }
        else
        {
            pos = *keyPos;
        }
        if (onBone)
        {
            if (target->extended && (target->extended->extraFlag & 0x200) != 0)
                return key;
            if (refIsCurrent && EVE_gpst_CurrentRefMatrix)
            {
                EVE_l_RefRelativeToCurrent(&refRelative);
                MATH_TransformVertex(&pos, &refRelative, &pos);
                if ((EVE_gpst_CurrentGAO->flags & 2) != 0)
                    pos.z = pos.z + EVE_l_Actor(EVE_gpst_CurrentGAO)->aniPelvisOffset;
            }
            mtx->mtx.Pos = pos;
            mtx->mtx.mtxflag |= 2u;
            mtx->mtx.mtxflag |= 2u;
        }
        else
        {
            if (EVE_gpst_CurrentRefMatrix && EVE_b_Track_UseFlash(EVE_gpst_CurrentTrack, 0, 0, 0))
                MATH_TransformVertex(&pos, EVE_gpst_CurrentRefMatrix, &pos);
            mtx->mtx.Pos = pos;
            mtx->mtx.mtxflag |= 2u;
            if ((key->flags2 & 0x800) != 0
                && (target->flags & 0x200) != 0
                && target->extended->coll
                && target->extended->coll->colInstance
                && target->extended->coll->colInstance->mtx)
            {
                target->extended->coll->colInstance->mtx->Pos = pos;
            }
        }
    }
    if ((type & 0x14) != 0)
    {
        EVE_Event_ComputeInterpoledMatrix(target, &interpolated, key, nextKey, keyTime);
        if (onBone)
        {
            if (target->extended && (target->extended->extraFlag & 0x400) != 0)
                return key;
            if (refIsCurrent && EVE_gpst_CurrentRefMatrix)
            {
                EVE_l_RefRelativeToCurrent(&refRelative);
                MATH_Mul33MatrixMatrix(&rot, &interpolated, &refRelative, 1);
            }
            else
            {
                MATH_Copy33Matrix(&rot, &interpolated);
            }
            MATH_Copy33Matrix(&mtx->mtx, &rot);
        }
        else
        {
            if (EVE_gpst_CurrentRefMatrix && EVE_b_Track_UseFlash(EVE_gpst_CurrentTrack, 0, 0, 0))
                MATH_Mul33MatrixMatrix(&rot, &interpolated, EVE_gpst_CurrentRefMatrix, 1);
            else
                MATH_Copy33Matrix(&rot, &interpolated);
            MATH_Copy33Matrix(&mtx->mtx, &rot);
        }
        mtx->mtx.mtxflag |= 4u;
    }
    if (onBone && mirror && (type & 3) != 0)
    {
        if (OBJ_b_IsChild(target))
        {
            if (target->basic && target->basic->mtx)
                father = target->basic->mtx->refGao;
            else
                father = 0;
        }
        if (father && OBJ_b_IsChild(father))
        {
            grandFather = father->basic && father->basic->mtx ? father->basic->mtx->refGao : 0;
            if (grandFather)
            {
                OBJ_ComputeGlobalWithLocal(grandFather, &grandFatherGlobal, 1);
                OBJ_ComputeGlobalWithLocal(father, &fatherGlobal, 1);
                OBJ_ComputeGlobalWithLocal(target, &targetGlobal, 1);
                if ((father->flags & 0x10000) != 0)
                    m = father->matrices + 1;
                else
                    m = father->matrices;
                fatherLen = MATH_VecLen(&m->Pos);
                if ((target->flags & 0x10000) != 0)
                    m = target->matrices + 1;
                else
                    m = target->matrices;
                targetLen = MATH_VecLen(&m->Pos);
                GAO_ModifierLegLink_Compute(&grandFatherGlobal, &fatherGlobal, &targetGlobal, fatherLen, targetLen);
                if (OBJ_b_IsChild(grandFather))
                {
                    ref = grandFather->basic && grandFather->basic->mtx ? grandFather->basic->mtx->refGao2 : 0;
                    if (ref)
                    {
                        OBJ_ComputeGlobalWithLocal(ref, &refGlobal, 1);
                        MATH_InvertMatrix(&refInverse, &refGlobal);
                        MATH_MulMatrixMatrix(&grandFather->basic->mtx->mtx, &grandFatherGlobal, &refInverse);
                        MATH_Orthonormalize(&grandFather->basic->mtx->mtx);
                    }
                }
                MATH_InvertMatrix(&refInverse, &grandFatherGlobal);
                MATH_MulMatrixMatrix(&father->basic->mtx->mtx, &fatherGlobal, &refInverse);
                MATH_Orthonormalize(&father->basic->mtx->mtx);
                if (OBJ_b_IsChild(target))
                {
                    if (target->basic && target->basic->mtx)
                        ref = target->basic->mtx->refGao2;
                    else
                        ref = 0;
                    if (ref)
                    {
                        OBJ_ComputeGlobalWithLocal(ref, &refGlobal, 1);
                        MATH_InvertMatrix(&refInverse, &refGlobal);
                        MATH_MulMatrixMatrix(&target->basic->mtx->mtx, &targetGlobal, &refInverse);
                        MATH_Orthonormalize(&target->basic->mtx->mtx);
                    }
                }
            }
        }
    }
    return key;
}

int EVE_RunOneTrack(tGameObjectEventsListElem* track, tAnimTracks* state, int handleEnd, float* dtLeft)
{
    float remaining;
    float labelTime;
    unsigned int kind;
    int labelIdx;
    tGameObjectEventElem* key;

    if (!track->trackElems)
        return 0;
    EVE_gpst_SavedMatrix = EVE_gpst_OwnerGAO->matrices;
    if ((track->flags & 8) != 0)
        state->privateData = -1;
    if (handleEnd
        && TIM_gf_dt != 0.0
        && state->currElemKeyFrame == track->elemNum - 1
        && EVE_l_FrameTime(track->trackElems[state->currElemKeyFrame].duration) == state->currTime
        && (track->flags & 2) != 0)
    {
        if (track->boneID == 255)
        {
            EVE_RunningTracksAtStart(EVE_gpst_CurrentData);
        }
        else
        {
            state->currElemKeyFrame = 0;
            state->flag &= ~4u;
        }
    }

    remaining = TIM_gf_dt;
    for (;;)
    {
        for (;;)
        {
            key = &track->trackElems[state->currElemKeyFrame];
            EVE_gpst_CurrentEvent = key;
            EVE_gf_ElapsedDT = TIM_gf_dt - remaining;
            if (EVE_l_FrameTime(key->duration) >= remaining + state->currTime)
            {
                EVE_gf_DT = remaining;
                state->currTime = remaining + state->currTime;
                remaining = 0.0;
            }
            else
            {
                EVE_gf_DT = (float)(EVE_l_FrameTime(key->duration) - state->currTime);
                remaining = remaining - EVE_gf_DT;
                state->currTime = (float)EVE_l_FrameTime(key->duration);
            }
            if ((key->flags2 & 4) != 0
                && (state->flag & 4) == 0
                && (EVE_gpst_CurrentListTracks->flags & 1) == 0
                && TIM_gf_dt >= 0.0)
            {
                EVE_FlashMatrix();
            }
            if ((key->flags2 & 1) != 0 && (state->flag & 4) != 0)
                break;
            kind = key->flags2 & 0x7C0;
            if (kind == 512 && !EVE_gb_PlayOnlyAnimTracks)
            {
                labelIdx = EVE_i_SearchLabel(track, key->flags2 & 0xF000, &labelTime);
                if (labelIdx != -1)
                {
                    state->currElemKeyFrame = labelIdx;
                    state->currTime = 0.0;
                    continue;
                }
            }
            else if (kind == 1024 && !EVE_gb_PlayOnlyAnimTracks)
            {
                labelIdx = EVE_i_SearchLabel(track, key->flags2 & 0xF000, &labelTime);
                if (labelIdx != -1)
                {
                    EVE_SetAllTimesTo(labelTime);
                    EVE_gb_NeedToReparse = 1;
                    return 0;
                }
            }
            else if (kind == 64)
            {
                if (!EVE_gb_PlayOnlyAnimTracks)
                {
                    EVE_Event_AIFunction_Play(key);
                    if ((key->flags2 & 8) != 0)
                    {
                        if (AI_GetIntFromStack())
                        {
                            state->currTime = 0.0;
                            return 0;
                        }
                    }
                    else if ((key->flags2 & 0x10) != 0 && !AI_GetIntFromStack())
                    {
                        state->currTime = 0.0;
                        return 0;
                    }
                }
            }
            else if (kind == 128 && (remaining == 0.0 || state->currElemKeyFrame == track->elemNum - 1))
            {
                EVE_Event_InterpolationKey_Play(key);
            }
            break;
        }
        state->flag |= 4u;
        if (remaining == 0.0)
        {
            if (dtLeft)
                *dtLeft = remaining;
            return 0;
        }
        state->currTime = 0.0;
        if (state->currElemKeyFrame != track->elemNum - 1)
        {
            ++state->currElemKeyFrame;
            state->flag &= ~4u;
            continue;
        }
        if ((track->flags & 2) != 0)
        {
            state->currElemKeyFrame = 0;
            state->flag &= ~4u;
        }
        else if ((track->flags & 4) != 0)
        {
            break;
        }
        if (EVE_gpst_CurrentGAO == EVE_gpst_OwnerGAO
            && (EVE_gpst_CurrentGAO->flags & 2) != 0
            && handleEnd
            && (key->flags2 & 0x7C0) == 128)
        {
            if (dtLeft)
                *dtLeft = remaining;
            state->currTime = (float)EVE_l_FrameTime(key->duration);
            state->currElemKeyFrame = track->elemNum - 1;
            return 1;
        }
    }
    state->currTime = (float)EVE_l_FrameTime(key->duration);
    state->flag &= ~1u;
    if (dtLeft)
        *dtLeft = remaining;
    return 0;
}

int EVE_b_RunAnimation(tEventData* eve, float* dtLeft)
{
    tGameObjectEventsList* list;
    tAnimTracks* states;
    Mtx4x4* savedRefMatrix;
    tGroup* skeleton;
    tGameObjectMain* gao;
    tGameObjectMain* bone;
    uint16_t boneID;
    BOOL wholeObject;
    int boneTrack;
    int skip;
    char lod;
    float firstDt;
    int result;
    int ran;
    unsigned int trackIdx;
    unsigned int i;

    if (!eve)
        return 0;
    list = eve->eventListStruct;
    states = eve->tracks;
    if (!eve->eventListStruct || !states)
        return 0;
    EVE_gpst_CurrentListTracks = eve->eventListStruct;
    savedRefMatrix = EVE_gpst_CurrentRefMatrix;
    EVE_gpst_CurrentGAO = EVE_gpst_OwnerGAO;
    skeleton = EVE_l_Actor(EVE_gpst_OwnerGAO)->skeletonGroup;
    skip = !EVE_gb_ForcePlayerOnAllTracks
        && ((EVE_gpst_CurrentGAO->flags2 & 4) != 0 || (EVE_gpst_CurrentGAO->flags2 & 2) == 0);
    if (EVE_gpst_CurrentGAO->if_this_not0_wont_render_gao)
        skip = 1;
    lod = EVE_c_UpdateLOD(EVE_gpst_OwnerGAO, 1);
    firstDt = 0.0;
    result = 0;

    for (trackIdx = 0; trackIdx < list->num; ++trackIdx)
    {
        EVE_gpst_CurrentRefMatrix = savedRefMatrix;
        EVE_l_SelectTrack(list, states, trackIdx);
        wholeObject = EVE_gpst_CurrentTrack->boneID == 0xFFFF;
        if ((EVE_gpst_CurrentParam->flag & 1) == 0)
            continue;
        if ((EVE_gpst_CurrentParam->flag & 2) != 0)
            continue;
        boneTrack = EVE_gpst_CurrentTrack->flags & 0x8000;
        if (boneTrack
            && (lod & 4) != 0
            && EVE_gpst_CurrentTrack->boneID != 0xFFFF
            && EVE_gpst_CurrentTrack->boneID
            && (EVE_w_Event_InterpolationKey_GetType(EVE_gpst_CurrentTrack->trackElems) & 3) != 0)
        {
            continue;
        }
        if (skip && (lod & 0x80) != 0 && !wholeObject)
        {
            EVE_l_Actor(EVE_gpst_OwnerGAO)->initedInANIMainCall = 0;
            break;
        }
        if ((lod & 1) != 0 && !wholeObject)
            break;
        if (boneTrack && (lod & 8) != 0)
        {
            boneID = EVE_gpst_CurrentTrack->boneID;
            if (!wholeObject && skeleton->tablePF->actElemNum > boneID)
            {
                bone = (tGameObjectMain*)((tableElemStructPF*)(skeleton->tablePF->firstElemPtr))[boneID].objectPtr;
                if (bone && bone->extended && (bone->extended->extraFlag & 0x20) != 0)
                    continue;
            }
        }
        if (!boneTrack || EVE_gpst_CurrentTrack->gao)
            ran = EVE_RunOneTrack(&list->listElems[trackIdx], &states[trackIdx], 1, dtLeft);
        else
            ran = EVE_RunAnimTrack(EVE_gpst_CurrentTrack, EVE_gpst_CurrentParam, trackIdx, lod, dtLeft);
        if (!trackIdx)
            result = ran;
        if (!trackIdx && ran)
            firstDt = *dtLeft;
    }

    if (trackIdx < list->num)
    {
        for (i = list->num - 1; i && (list->listElems[i].flags & 0x8000) == 0; --i)
        {
            EVE_gpst_CurrentTrack = &list->listElems[i];
            EVE_gpst_CurrentParam = &states[i];
            if (EVE_gpst_CurrentTrack->gao)
                gao = EVE_gpst_CurrentTrack->gao;
            else
                gao = EVE_gpst_OwnerGAO;
            EVE_gpst_CurrentGAO = gao;
            EVE_gpst_CurrentRefMatrix = EVE_l_RefMatrix(gao);
            EVE_RunOneTrack(&list->listElems[i], &states[i], 1, dtLeft);
        }
    }
    *dtLeft = firstDt;
    return result;
}

int EVE_b_RunAllTracks(tEventData* eve, float* dtLeft)
{
    BOOL hasSkeleton;
    tGameObjectEventsList* list;
    tAnimTracks* states;
    Mtx4x4* savedRefMatrix;
    unsigned int i;
    uint8_t lod;
    float firstDt;
    int result;

    lod = 0;
    if (!eve)
        return 0;
    list = eve->eventListStruct;
    if (!eve->eventListStruct)
        return 0;
    states = eve->tracks;
    if (!states)
        return 0;
    result = 0;
    firstDt = 0.0;
    EVE_gpst_CurrentGAO = EVE_gpst_OwnerGAO;
    hasSkeleton = !ENG_gb_EVERunning
        && EVE_gpst_CurrentGAO->basic
        && EVE_gpst_CurrentGAO->basic->actorObjectOrVisualObject
        && EVE_l_Actor(EVE_gpst_CurrentGAO)->skeletonGroup;
    if (hasSkeleton)
        lod = EVE_c_UpdateLOD(EVE_gpst_CurrentGAO, 0);
    for (i = 0; i < list->num; ++i)
    {
        if (hasSkeleton
            && i > 1
            && !EVE_gb_ForcePlayerOnAllTracks
            && ((EVE_gpst_CurrentGAO->flags2 & 4) != 0 || (EVE_gpst_CurrentGAO->flags2 & 2) == 0)
            && list->listElems[i].trackElems
            && (list->listElems[i].trackElems->flags2 & 0x7C0) == 128)
        {
            if (EVE_gpst_CurrentGAO->basic && EVE_gpst_CurrentGAO->basic->actorObjectOrVisualObject)
                EVE_l_Actor(EVE_gpst_CurrentGAO)->initedInANIMainCall = 0;
        }
        else if ((list->listElems[i].flags & 0x8000) == 0
            || (lod & 4) == 0
            || list->listElems[i].boneID == 0xFFFF
            || !list->listElems[i].boneID
            || (EVE_w_Event_InterpolationKey_GetType(list->listElems[i].trackElems) & 3) == 0)
        {
            savedRefMatrix = EVE_gpst_CurrentRefMatrix;
            if ((states[i].flag & 1) != 0 && (states[i].flag & 2) == 0)
            {
                EVE_l_SelectTrack(list, states, i);
                EVE_gb_NeedToReparse = 0;
                if (EVE_RunOneTrack(&list->listElems[i], &states[i], 1, dtLeft)) // !!!!!!
                {
                    result = 1;
                    if (!i && dtLeft)
                        firstDt = *dtLeft;
                }
                if (EVE_gb_NeedToReparse)
                    i = -1;   // does not terminate if the label resolves to a key at or before the one that set it
                if (ENG_gb_EVERunning)
                {
                    if ((EVE_gpst_CurrentGAO->flags & 0x4000000) != 0)
                    {
                        ENG_gb_EVERunning = 0;
                        ANI_UpdateFlash(EVE_gpst_CurrentGAO, 3);
                        ENG_gb_EVERunning = 1;
                    }
                    if ((EVE_gpst_CurrentGAO->flags & 4) != 0)
                        EVE_gpst_CurrentGAO->basic->dyn->flags |= 0x800000u;
                }
            }
            EVE_gpst_CurrentRefMatrix = savedRefMatrix;
        }
    }
    if (dtLeft)
        *dtLeft = firstDt;
    return result;
}

struct sEventData* EVE_AllocateMainStruct(struct sGameObjectMain* gao)
{
    gao->extended->eve = (tEventData*)MEM_p_Alloc(sizeof(tEventData));
    memset(gao->extended->eve, 0, sizeof(tEventData));
    gao->extended->eve->eventListStruct = (tGameObjectEventsList*)MEM_p_Alloc(sizeof(tGameObjectEventsList));
    memset(gao->extended->eve->eventListStruct, 0, sizeof(tGameObjectEventsList));
    gao->extended->eve->eventListStruct->usageCount = 1;
    gao->extended->eve->tracks = 0;
    return gao->extended->eve;
}

void EVE_ResolveTrackRef(tGameObjectEventsList* list, tGameObjectEventsListElem* track)
{
    tGameObjectEventElem* key;
    uint16_t i;

    if (track->trackElems && (track->flags & 0x10) != 0)
    {
        EVE_gpst_CurrentReolve = list;
        for (i = 0; i < (int)track->elemNum; ++i)
        {
            key = &track->trackElems[i];
            if ((key->flags2 & 0x20) != 0)
            {
                // The track index is the elemPtr value itself, but the key index is read through it.
                key->eptrdu.data = list->listElems[(int16_t)key->eptrdu.data].trackElems[(int16_t)(((*((uint32_t*)key->eptrdu.data)) >> 16) & 0xffff)].eptrdu.data;
            }
            else if ((key->flags2 & 0x7C0) == 64)
            {
                EVE_Event_AIFunction_Resolve(key);
            }
        }
    }
}

void EVE_ResolveListTrackRef(tGameObjectEventsList* list)
{
    int i;

    if (list && (list->flags & 2) == 0)
    {
        list->flags |= 2u;
        for (i = 0; i < list->num; ++i)
            EVE_ResolveTrackRef(list, &list->listElems[i]);
    }
}

void EVE_ReinitTrack(struct sEventData* eve, int trackID)
{
    tGameObjectEventsListElem* track;
    tAnimTracks* state;

    if (!eve || !eve->eventListStruct || !eve->tracks)
        return;
    track = &eve->eventListStruct->listElems[trackID];
    state = &eve->tracks[trackID];
    state->currTime = 0.0;
    state->currElemKeyFrame = 0;
    state->flag &= ~1u;
    if ((track->flags & 1) != 0)
        state->flag |= 1u;
    state->flag &= ~2u;
    state->flag &= ~4u;
}

void EVE_ReinitData(struct sEventData* eve)
{
    uint16_t i;

    if (eve && eve->eventListStruct)
    {
        for (i = 0; i < (int)eve->eventListStruct->num; ++i)
            EVE_ReinitTrack(eve, i);
    }
}

void EVE_ResolveRefs(struct sGameObjectMain* gao, int incrementUsageCount)
{
    if (gao->extended->eve && gao->extended->eve->eventListStruct)
    {
        if (gao->extended->eve->eventListStruct->num)
        {
            gao->extended->eve->tracks = (tAnimTracks*)MEM_p_Alloc(sizeof(tAnimTracks) * gao->extended->eve->eventListStruct->num);
            memset(gao->extended->eve->tracks, 0, sizeof(tAnimTracks) * gao->extended->eve->eventListStruct->num);
        }
        EVE_ResolveListTrackRef(gao->extended->eve->eventListStruct);
        EVE_ReinitData(gao->extended->eve);
        if (incrementUsageCount)
            ++gao->extended->eve->eventListStruct->usageCount;
    }
}

int EVE_Event_InterpolationKey_Load(tGameObjectEventElem* key, tGameObjectEventElem* prevKey, char* src, void* elemPtr, tGameObjectEventsListElem* track, int16_t flags)
{
    int kind;
    unsigned int deltaKind;
    int timeNum;
    int i;
    UltraCompressedQuat ultra;
    Quat4 delta;
    CompressedQuat compressed;
    Quat4* prevQuat;
    CompressedQuat prevCompressed;
    Quat4 decompressed;
    CompressedQuat quat;
    int16_t type;
    uint8_t* dest;
    char* cursor;
    int16_t size;

    cursor = src;
    if ((flags & 0x2000) != 0)
        size = track->trackElems->eptrdu.size >> 1;
    else
        size = LOA_ReadShort(&cursor, 0, 1);
    if ((flags & 0x1000) != 0)
    {
        if ((flags & 0x2000) != 0)
            type = track->trackElems->eptrdu.type;
        else
            type = EVE_w_Event_InterpolationKey_GetType(track->trackElems);
    }
    else
    {
        type = LOA_ReadShort(&cursor, 0, 1);
    }
    key->eptrdu.data = elemPtr;
    if ((type & 0x780) != 0)
    {
        decompressed.q1 = 0.0;
        decompressed.q2 = 0.0;
        decompressed.q3 = 0.0;
        decompressed.q4 = 0.0;
        if ((type & 0x80) != 0)
        {
            LOA_ReadCompressedQuaternion(&cursor, &quat, 1);
        }
        else
        {
            prevQuat = EVE_pst_Event_InterpolationKey_GetQuaternion(prevKey);
            MATH_CompressQuaternion(&prevCompressed, prevQuat);
            LOA_ReadUltraCompressedQuaternion(&cursor, &ultra, 1);
        }
        kind = type & 0x780;
        switch (kind)
        {
        case 0x100:
            quat.s1 = ultra.s1 + prevCompressed.s1;
            quat.s2 = ultra.b2 + prevCompressed.s2;
            quat.s3 = ultra.b3 + prevCompressed.s3;
            break;
        case 0x200:
            quat.s1 = ultra.b2 + prevCompressed.s1;
            quat.s2 = ultra.s1 + prevCompressed.s2;
            quat.s3 = ultra.b3 + prevCompressed.s3;
            break;
        case 0x400:
            quat.s1 = ultra.b2 + prevCompressed.s1;
            quat.s2 = ultra.b3 + prevCompressed.s2;
            quat.s3 = ultra.s1 + prevCompressed.s3;
            break;
        }
        MATH_DecompressQuaternion(&decompressed, &quat);
        size = 20;
        if ((flags & 0x2100) != 0)
        {
            key->eptrdu.size = (size << 1) | 0x1;
            key->eptrdu.type = type;
            memcpy(elemPtr, &decompressed, sizeof(Quat4));
        }
        else
        {
            dest = (uint8_t*)(key->eptrdu.data);
            *(uint16_t*)dest = size;
            dest += 2;
            *(uint16_t*)dest = type;
            dest += 2;
            memcpy(dest, &decompressed, sizeof(Quat4));
        }
        return cursor - src;
    }
    if ((type & 0xF800) != 0)
    {
        delta.q1 = 0.0f;
        delta.q2 = 0.0f;
        delta.q3 = 0.0f;
        deltaKind = type & 0xF800;
        if (deltaKind > 0x2000)
        {
            if (deltaKind == 0x4000)
            {
                LOA_ReadUltraCompressedQuaternion(&cursor, &ultra, 1);
                MATH_calc_curr_quat_from_dt_4000(&delta, &ultra);
            }
            else if (deltaKind == 0x8000)
            {
                LOA_ReadUltraCompressedQuaternion(&cursor, &ultra, 1);
                MATH_calc_curr_quat_from_dt_8000(&delta, &ultra);
            }
        }
        else
        {
            switch (deltaKind)
            {
            case 0x2000u:
                LOA_ReadUltraCompressedQuaternion(&cursor, &ultra, 1);
                MATH_calc_curr_quat_from_dt_2000(&delta, &ultra);
                break;
            case 0x800u:
                LOA_ReadCompressedQuaternion(&cursor, &compressed, 1);
                MATH_calc_curr_quat_from_dt_800(&delta, &compressed);
                break;
            case 0x1000u:
                LOA_ReadCompressedQuaternion(&cursor, &compressed, 1);
                MATH_calc_curr_quat_from_dt_1000(&delta, &compressed);
                break;
            }
        }
        size = 16;
        if ((flags & 0x2100) != 0)
        {
            key->eptrdu.size = (size << 1) | 0x1;
            key->eptrdu.type = type;
            memcpy(elemPtr, &delta, 12u);
        }
        else
        {
            dest = (uint8_t*)(key->eptrdu.data);
            *(uint16_t*)dest = size;
            dest += 2;
            *(uint16_t*)dest = type;
            dest += 2;
            memcpy(dest, &delta, 12u);
        }
        return cursor - src;
    }
    if ((type & 0x17) == 0)
        return cursor - src;
    if ((flags & 0x2100) != 0)
    {
        key->eptrdu.size = (size << 1) | 0x1;
        key->eptrdu.type = type;
        dest = elemPtr;
    }
    else
    {
        dest = (uint8_t*)(key->eptrdu.data);
        *(uint16_t*)dest = size;
        dest += 2;
        *(uint16_t*)dest = type;
        dest += 2;
    }
    if ((flags & 0x2000) != 0)
        size -= 2;
    if ((flags & 0x1000) != 0)
        size -= 2;
    if ((type & 3) == 0)
    {
        if ((type & 0x14) != 0)
        {
            kind = type & 0x14;
            if (kind == 4)
            {
                LOA_ReadMatrix(&cursor, (Mtx4x4*)dest, 1);
                dest += 68;
            }
            else if (kind == 16)
            {
                LOA_ReadQuaternion(&cursor, (Quat4*)dest, 1);
                dest += 16;
            }
        }
    }
    else
    {
        for (i = 0; i < (type & 3); ++i)
        {
            LOA_ReadVector(&cursor, (Vec3*)dest, 1);
            dest += sizeof(Vec3);
        }
    }
    if ((type & 8) != 0 && cursor - src < size)
    {
        timeNum = LOA_ReadLong(&cursor, (uint32_t*)dest, 1);
        dest += 4;
        for (i = 0; i < 2 * timeNum - 1; ++i)
        {
            LOA_ReadFloat(&cursor, (float*)dest, 1);
            dest += 4;
        }
    }
    return cursor - src;
}

void EVE_LoadListEvents(tGameObjectEventsListElem* track, char* src, uint32_t* readSize, int16_t flags)
{
    tGameObjectEventElem* keys;
    tGameObjectEventElem* prevKey;
    uint8_t* dataPtr;
    int dataPtrOffset;
    char* start;
    char* peek;
    int16_t peekType;
    int16_t keySize;
    uint16_t dataSize;
    int16_t keyFlags;
    int kind;
    int isLink;
    int read;
    int link;
    uint16_t i;

    read = 0;
    dataPtrOffset = 0;
    start = src;
    dataPtr = 0;
    keys = 0;
    track->flags &= ~0x10u;
    track->elemNum = LOA_ReadLong(&src, 0, 1);
    track->trackElems = 0;
    if (track->elemNum)
    {
        if (track->size)
        {
            if ((flags & 0x2000) != 0)
            {
                peek = src;
                if ((flags & 0x200) != 0)
                    LOA_ReadChar(&peek, 0, 4);
                else
                    LOA_ReadShort(&peek, 0, 4);
                LOA_ReadShort(&peek, 0, 4);
                LOA_ReadShort(&peek, &keySize, 4);
                LOA_ReadShort(&peek, &peekType, 4);
                if ((peekType & 0x780) != 0)
                {
                    keySize = 16;
                }
                else if ((peekType & 0xF800) != 0)
                {
                    keySize = 12;
                }
                dataSize = keySize * track->elemNum;
                track->dataPtr = (uint8_t*)MEM_p_Alloc(dataSize);
                dataPtr = track->dataPtr;
                track->trackElems = (tGameObjectEventElem*)MEM_p_Alloc(sizeof(tGameObjectEventElem) * track->elemNum);
            }
            else
            {
                track->dataPtr = (uint8_t*)MEM_p_Alloc(track->size);
                dataPtr = track->dataPtr;
                track->trackElems = (tGameObjectEventElem*)MEM_p_Alloc(sizeof(tGameObjectEventElem) * track->elemNum);
            }
            LOA_ul_FileTypeSize_31 += track->size;
        }
        else
        {
            track->trackElems = (tGameObjectEventElem*)MEM_p_Alloc(sizeof(tGameObjectEventElem) * track->elemNum);
        }
        LOA_ul_FileTypeSize_30 += sizeof(tGameObjectEventElem) * track->elemNum;
        keys = track->trackElems;
        for (i = 0; i < (int)track->elemNum; ++i)
        {
            isLink = 0;
            if ((flags & 0x200) != 0)
            {
                // LOA_ReadChar returns a signed char and duration is a
                // uint16_t, so a compact duration of 128 or more has to be
                // taken as a byte: without the cast it sign-extends and a
                // 150-frame key becomes 65430
                keys[i].duration = (uint8_t)LOA_ReadChar(&src, 0, 1);
            }
            else
            {
                keys[i].duration = LOA_ReadShort(&src, 0, 1);
            }
            if ((flags & 0x800) != 0)
            {
                if (i)
                {
                    keys[i].flags2 = keys->flags2;
                    keys[i].flags2 &= ~4u;
                }
                else
                {
                    keys->flags2 = LOA_ReadShort(&src, 0, 1);
                }
            }
            else
            {
                keys[i].flags2 = LOA_ReadShort(&src, 0, 1);
            }
            keys[i].eptrdu.data = NULL;
            keys[i].flags2 &= ~0x2;
            if ((keys[i].flags2 & 0x20) != 0)
            {
                track->flags |= 0x10u;
                isLink = 1;
            }
            kind = keys[i].flags2 & 0x7C0;
            if (kind == 0x40)
            {
                read = EVE_Event_AIFunction_Load(&keys[i], src, &dataPtr[dataPtrOffset]);
                src += read;
                if (read != 4)
                    dataPtrOffset += read;
                track->flags |= 0x10u;
            }
            else if (kind == 0x80)
            {
                if (i)
                    prevKey = &keys[i - 1];
                else
                    prevKey = 0;
                if (isLink)
                {
                    link = LOA_ReadLong(&src, 0, 1);
                    keys[i].eptrdu.data = (void*)link; // link
                }
                else
                {
                    if (i)
                        keyFlags = flags;
                    else
                        keyFlags = 0;
                    if (!i && (flags & 0x2000) != 0)
                        keyFlags |= 0x100u;
                    read = EVE_Event_InterpolationKey_Load(&keys[i], prevKey, src, &dataPtr[dataPtrOffset], track, keyFlags);
                    src += read;
                    if ((flags & 0x2000) != 0)
                        dataPtrOffset = dataPtrOffset + (keys[i].eptrdu.size >> 1) - 4;
                    else
                        dataPtrOffset += *(int16_t*)keys[i].eptrdu.data;
                }
            }
        }
    }
    *readSize = src - start;
}

struct sGameObjectEventsList* EVE_ul_CallbackLoadListTracks(uint32_t Offset)
{
    tGameObjectEventsList* list;
    char* cursor;
    int fileSize;
    int read;
    int16_t trackFlags;
    int noGao;
    BOOL isSpecial;
    int restoreReadMode;
    unsigned int i;

    list = (tGameObjectEventsList*)MEM_p_Alloc(sizeof(tGameObjectEventsList));
    LOA_ul_FileTypeSize_32 += sizeof(tGameObjectEventsList);
    isSpecial = LOA_IsSpecial(LOA_ul_GetCurrentKey());
    if (isSpecial)
        list->usageCount = 1000000;
    else
        list->usageCount = 0;
    restoreReadMode = 0;
    if (BIG_gi_ReadMode == 1 && !BIG_gi_SpecialRead && isSpecial)
    {
        restoreReadMode = 1;
        BIG_gi_ReadMode = 0;
    }
    cursor = BIG_pc_ReadFileTmp(Offset, &fileSize);
    list->num = LOA_ReadShort(&cursor, 0, 1);
    AnimSizeOnDisk += 2;
    list->flags = LOA_ReadShort(&cursor, 0, 1);
    AnimSizeOnDisk += 2;
    list->flags &= ~2u;
    list->flags &= ~0x8000u;
    list->listElems = 0;
    if (list->num)
    {
        list->listElems = (tGameObjectEventsListElem*)MEM_p_Alloc(sizeof(tGameObjectEventsListElem) * list->num);
        LOA_ul_FileTypeSize_33 += sizeof(tGameObjectEventsListElem) * list->num;
        memset(list->listElems, 0, sizeof(tGameObjectEventsListElem) * list->num);
    }
    for (i = 0; i < list->num; ++i)
    {
        ++NumTracks;
        list->listElems[i].flags = LOA_ReadShort(&cursor, 0, 1);
        AnimSizeOnDisk += 2;
        trackFlags = list->listElems[i].flags & 0xFF00;
        trackFlags &= ~0x4000u;
        trackFlags &= ~0x8000u;
        noGao = list->listElems[i].flags & 0x8000;
        list->listElems[i].flags &= ~0x3F00u;
        list->listElems[i].boneID = LOA_ReadShort(&cursor, 0, 1);
        AnimSizeOnDisk += 2;
        list->listElems[i].size = LOA_ReadLong(&cursor, 0, 1);
        AnimSizeOnDisk += 4;
        if (noGao)
        {
            if (!trackFlags)
            {
                list->listElems[i].s3 = LOA_ReadLong(&cursor, 0, 1);
                AnimSizeOnDisk += 4;
            }
        }
        else
        {
            list->listElems[i].gao = (tGameObjectMain*)LOA_ReadLong(&cursor, 0, 1);
            AnimSizeOnDisk += 4;
            if (list->listElems[i].gao)
                LOA_MakeFileRef((int)list->listElems[i].gao, &list->listElems[i].gao, OBJ_ul_GameObjectCallback, 1);
            LOA_ReadString(&cursor, 0, 15, 2);
            LOA_ReadChar(&cursor, 0, 2);
            AnimSizeOnDisk += 16;
        }
        EVE_LoadListEvents(&list->listElems[i], cursor, &read, trackFlags);
        cursor += read;
        AnimSizeOnDisk += read;
    }
    if (restoreReadMode)
        BIG_gi_ReadMode = 1;
    return list;
}

void EVE_MainCall(struct sWorld* world)
{
    tableElemStructPF* cursor;
    tableElemStructPF* last;
    tGameObjectMain* gao;

    cursor = world->eot.eventGameObjectsTablePF.firstElemPtr;
    last = ((tableElemStructPF*)(world->eot.eventGameObjectsTablePF.nextElemPtr)) - 1;
    while (cursor <= last)
    {
        gao = (tGameObjectMain*)cursor->objectPtr;
        if (gao
            && ((uintptr_t)cursor->objectPtr & 3) != 1
            && gao->extended
            && gao->extended->eve
            && ((gao->extended->extraFlag & 4) == 0 || gao->distance)
            && (gao->flags2 & 0x400000) == 0)
        {
            EVE_gpst_OwnerGAO = gao;
            EVE_gpst_CurrentData = gao->extended->eve;
            EVE_gpst_CurrentListTracks = EVE_gpst_CurrentData->eventListStruct;
            EVE_gpst_CurrentRefMatrix = EVE_l_RefMatrix(gao);
            EVE_b_RunAllTracks(EVE_gpst_CurrentData, 0);
        }
        ++cursor;
    }
    EVE_gpst_CurrentTrack = NULL;
}
