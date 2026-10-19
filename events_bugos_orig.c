#include "memory.h"
#include "loading.h"
#include "bigfiles.h"
#include "gameobject.h"
#include "time.h"
#include "ai.h"
#include "animation.h"
#include "modifiers/leglinkmodifier.h"
#include "events.h"

int EVE_gb_CanFlash = 1;

void EVE_InitData(tAnimInner* a1, tGameObjectEventsList* a2)
{
    if (a1->eventListStruct && a1->tracks)
    {
        if (a1->eventListStruct->num >= (int)a2->num)
        {
            memset(a1->tracks, 0, sizeof(tAnimTracks) * a1->eventListStruct->num);
        }
        else
        {
            a1->tracks = (tAnimTracks*)MEM_p_Realloc(a1->tracks, sizeof(tAnimTracks) * a2->num);
            memset(a1->tracks, 0, sizeof(tAnimTracks) * a2->num);
        }
    }
    else
    {
        if (a1->tracks)
        {
            MEM_Free(a1->tracks);
            a1->tracks = 0;
        }
        a1->tracks = (tAnimTracks*)MEM_p_Alloc(sizeof(tAnimTracks) * a2->num);
        memset(a1->tracks, 0, sizeof(tAnimTracks) * a2->num);
    }
    a1->eventListStruct = a2;
}

tAnimInner* EVE_pst_DuplicateData(tAnimInner* a1)
{
    int j;
    tGameObjectEventsListElem* v3;
    int i;
    tAnimInner* v5;
    void* v6;

    v5 = (tAnimInner*)MEM_p_Alloc(sizeof(tAnimInner));
    v5->tracks = 0;
    v5->eventListStruct = 0;
    if (a1 && a1->tracks)
    {
        v5->tracks = (tAnimTracks*)MEM_p_Alloc(sizeof(tAnimTracks) * a1->eventListStruct->num);
        memcpy(v5->tracks, a1->tracks, sizeof(tAnimTracks) * a1->eventListStruct->num);
    }
    if (a1)
    {
        if (a1->eventListStruct)
        {
            v5->eventListStruct = (tGameObjectEventsList*)MEM_p_Alloc(sizeof(tGameObjectEventsList));
            memcpy(v5->eventListStruct, a1->eventListStruct, sizeof(tGameObjectEventsList));
            if (a1->eventListStruct->listElems)
            {
                v5->eventListStruct->listElems = (tGameObjectEventsListElem*)MEM_p_Alloc(sizeof(tGameObjectEventsListElem) * a1->eventListStruct->num);
                memcpy(v5->eventListStruct->listElems, a1->eventListStruct->listElems, sizeof(tGameObjectEventsListElem) * a1->eventListStruct->num);
                v3 = v5->eventListStruct->listElems;
                for (i = 0; i < v5->eventListStruct->num; ++i)
                {
                    if (v3->trackElems)
                    {
                        v3->trackElems = (tGameObjectEventElem*)MEM_p_Alloc(sizeof(tGameObjectEventElem) * v3->elemNum);
                        memcpy(v3->trackElems, a1->eventListStruct->listElems[i].trackElems, sizeof(tGameObjectEventElem) * v3->elemNum);
                        for (j = 0; j < v3->elemNum; ++j)
                        {
                            if (v3->trackElems[j].elemPtr)
                            {
                                v6 = MEM_p_Alloc(*(int16_t*)v3->trackElems[j].elemPtr);
                                memcpy(v6, v3->trackElems[j].elemPtr, *(int16_t*)v3->trackElems[j].elemPtr);
                                v3->trackElems[j].elemPtr = (byte*)v6;
                            }
                        }
                    }
                    if (v3->dataPtr)
                    {
                        v3->dataPtr = (byte*)MEM_p_Alloc(v3->size);
                        memcpy(v3->dataPtr, a1->eventListStruct->listElems[i].dataPtr, v3->size);
                    }
                    ++v3;
                }
            }
        }
    }
    return v5;
}

void EVE_Duplicate(struct sGameObjectMain* a1, struct sGameObjectMain* a2, int a3)
{
    tAnimInner* v3;
    tAnimInner* v4;

    if (a3)
    {
        a2->extended->eve = EVE_pst_DuplicateData(a1->extended->eve);
    }
    else
    {
        v3 = a1->extended->eve;
        v4 = (tAnimInner*)MEM_p_Alloc(sizeof(tAnimInner));
        v4->tracks = 0;
        v4->eventListStruct = 0;
        if (v3->tracks)
        {
            v4->tracks = (tAnimTracks*)MEM_p_Alloc(sizeof(tAnimTracks) * v3->eventListStruct->num);
            memcpy(v4->tracks, v3->tracks, sizeof(tAnimTracks) * v3->eventListStruct->num);
        }
        if (v3->eventListStruct)
        {
            v4->eventListStruct = v3->eventListStruct;
            if (OBJ_gb_DuplicateAI)
                ++v4->eventListStruct->usageCount;
        }
        a2->extended->eve = v4;
    }
}

int EVE_b_Track_UseFlash(tGameObjectEventsListElem* a1, tGameObjectMain* a2, tGameObjectMain* a3, tGameObjectEventsList* a4)
{
    tGameObjectEventsList* v5;
    tGameObjectMain* v6;
    tGameObjectMain* v7;
    tGameObjectEventsListElem* v8;

    if (a2)
        v7 = a2;
    else
        v7 = EVE_gpst_CurrentGAO;
    if (a3)
        v6 = a3;
    else
        v6 = EVE_gpst_OwnerGAO;
    if (a4)
        v5 = a4;
    else
        v5 = EVE_gpst_CurrentListTracks;
    v8 = v5->listElems;
    while (v8 <= a1)
    {
        if ((v8->gao || v7 == v6)
            && (!v8->gao || v8->gao == v7)
            && v8->trackElems
            && (v8->trackElems->flags2 & 0x7C0) == 0x80)
        {
            if ((v8->trackElems->flags2 & 4) != 0)
                return 1;
            ++v8;
        }
        else
        {
            ++v8;
        }
    }
    return 0;
}

tGameObjectEventElem* EVE_Event_AIFunction_Play(tGameObjectEventElem* a1)
{
    tGameObjectActorObject* v2;
    tAIFunction* v3;
    tGameObjectMain* v4;
    tAI* v5;

    if (a1->elemPtr)
    {
        AI_gul_SizeGlobalVarStack = 0;
        AI_gul_SizeGlobalStack = 0;
        v3 = AI_gpst_CurrentFunction;
        v5 = AI_gpst_CurrentInstance;
        v4 = AI_gpst_CurrentGameObject;
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
            if (!EVE_gpst_CurrentTrack->gao
                || (EVE_gpst_CurrentTrack->gao->flags & 2) == 0
                || !EVE_gpst_CurrentTrack->gao->basic
                || !EVE_gpst_CurrentTrack->gao->basic->actorObjectOrVisualObject
                || !((tGameObjectActorObject*)(EVE_gpst_CurrentTrack->gao->basic->actorObjectOrVisualObject))->skeletonGroup)
            {
                return a1;
            }
            v2 = EVE_gpst_CurrentTrack->gao->basic->actorObjectOrVisualObject;
            if (EVE_gpst_CurrentTrack->boneID >= v2->skeletonGroup->tablePF->actElemNum)
                return a1;
            AI_gpst_CurrentUltra = (tGameObjectMain*)(((tableElemStructPF*)(v2->skeletonGroup->tablePF->firstElemPtr))[EVE_gpst_CurrentTrack->boneID].objectPtr);
            AI_gpst_CurrentGameObject = AI_gpst_CurrentUltra;
        }
        AI_RunNode((tAINode*)(a1->elemPtr + 4));
        if (AI_gul_SizeGlobalStack)
            EVE_gpst_CurrentParam->blendFactor = *(uint32_t*)AI_gac_GlobalStack;
        AI_gpst_CurrentFunction = v3;
        AI_gpst_CurrentInstanceUltra = v5;
        AI_gpst_CurrentInstance = v5;
        AI_gpst_CurrentUltra = v4;
        AI_gpst_CurrentGameObject = v4;
    }
    return a1;
}

int EVE_i_SearchLabel(struct sGameObjectEventsListElem* a1, int a2, float* a3)
{
    float v4;
    float v5;
    int i;

    v5 = 0.0;
    for (i = 0; i < a1->elemNum; ++i)
    {
        if ((a1->trackElems[i].flags2 & 0xF000) == a2
            && (a1->trackElems[i].flags2 & 0x7C0) != 0x200
            && (a1->trackElems[i].flags2 & 0x7C0) != 0x400)
        {
            *a3 = v5;
            return i;
        }
        v4 = (float)a1->trackElems[i].duration;
        v5 = (float)(v4 * 1.0 / 60.0 + v5);
    }
    return -1;
}

int EVE_i_SearchTime(tGameObjectEventsListElem* a1, float* a2)
{
    int result;
    float v3;
    float v4;
    float v5;
    float v6;
    float v7;
    int i;

    v6 = *a2;
    for (i = 0; ; ++i)
    {
        if (i >= a1->elemNum)
        {
            if (a1->trackElems)
            {
                v3 = (float)a1->trackElems[a1->elemNum - 1].duration;
                *a2 = (float)(v3 * 1.0 / 60.0);
                result = a1->elemNum - 1;
            }
            else
            {
                *a2 = 0.0;
                result = 0;
            }
            return result;
        }
        v5 = (float)a1->trackElems[i].duration;
        v7 = (float)(v5 * 1.0 / 60.0);
        if (v7 >= (double)v6)
            break;
        v4 = (float)a1->trackElems[i].duration;
        v6 = (float)(v6 - v4 * 1.0 / 60.0);
    }
    if (v7 - v6 >= 0.016656667)
    {
        *a2 = v6;
    }
    else
    {
        *a2 = 0.0;
        ++i;
    }
    return i;
}

void EVE_SetAllTimesTo(float a1)
{
    int v1;
    float v2;
    int i;

    for (i = 0; i < EVE_gpst_CurrentListTracks->num; ++i)
    {
        v2 = a1;
        v1 = EVE_i_SearchTime(&EVE_gpst_CurrentListTracks->listElems[i], &v2);
        if (v1 != -1)
        {
            EVE_gpst_OwnerGAO->extended->eve->tracks[i].flag &= ~4u;
            EVE_gpst_OwnerGAO->extended->eve->tracks[i].currElemKeyFrame = v1;
            EVE_gpst_OwnerGAO->extended->eve->tracks[i].currTime = v2;
        }
    }
}

void EVE_RunningTracksAtStart(tAnimInner* a1)
{
    int i;

    if (a1)
    {
        for (i = 0; i < a1->eventListStruct->num; ++i)
        {
            if ((a1->tracks[i].flag & 1) != 0)
            {
                a1->tracks[i].flag &= ~4u;
                a1->tracks[i].currElemKeyFrame = 0;
                a1->tracks[i].currTime = 0.0;
            }
        }
    }
}

void EVE_SetRunningTrack(struct sAnimTracks* a1, int a2)
{
    if (a2)
    {
        if ((a1->flag & 1) == 0)
        {
            a1->currTime = 0.0;
            a1->flag |= 1u;
            a1->flag &= ~2u;
            a1->flag &= ~4u;
            a1->currElemKeyFrame = 0;
        }
    }
    else
    {
        a1->flag &= ~1u;
    }
}

void EVE_SetRunningTracks(struct sAnimInner* a1, int a2)
{
    uint16_t i;

    if (a1)
    {
        for (i = 0; i < (int)a1->eventListStruct->num; ++i)
            EVE_SetRunningTrack(&a1->tracks[i], a2);
    }
}

void EVE_SetPauseTrack(struct sAnimTracks* a1, int a2)
{
    if (a2)
    {
        if ((a1->flag & 2) == 0)
            a1->flag |= 2u;
    }
    else
    {
        a1->flag &= ~2u;
    }
}

void EVE_SetPauseTracks(struct sAnimInner* a1, int a2)
{
    uint16_t i;

    for (i = 0; i < (int)a1->eventListStruct->num; ++i)
        EVE_SetPauseTrack(&a1->tracks[i], a2);
}

void EVE_FlashMatrix()
{
    tGaoMatrix* v0;

    if (EVE_gb_CanFlash && EVE_gpst_CurrentListTracks && EVE_gpst_CurrentGAO)
    {
        if ((EVE_gpst_CurrentGAO->flags & 0x400000) != 0)
            v0 = OBJ_pst_GetLocalMatrix(EVE_gpst_CurrentGAO);
        else
            v0 = (tGaoMatrix*)OBJ_pst_GetAbsoluteMatrix(EVE_gpst_CurrentGAO);
        if (EVE_gpst_CurrentRefMatrix)
        {
            if (EVE_gpst_CurrentRefMatrix != (Mtx4x4*)v0)
                memcpy(EVE_gpst_CurrentRefMatrix, v0, sizeof(Mtx4x4));
        }
        else
        {
            OBJ_ReinitMatrixStorageByFlags(EVE_gpst_CurrentGAO, &v0->mtx);
        }
    }
}

void EVE_GetTracksTime(struct sAnimInner* a1, int a2, float* currTime, float* fullTime)
{
    float v4;
    float v5;
    float v6;
    tGameObjectEventElem* v7;
    tGameObjectEventElem* v8;
    tGameObjectEventElem* v9; 
    tGameObjectEventElem* v10;
    tGameObjectEventElem* v11;

    if (currTime)
        *currTime = 0.0;
    if (fullTime)
        *fullTime = 0.0;
    if (a1 && a1->eventListStruct && a1->eventListStruct->listElems && a1->eventListStruct->listElems[a2].trackElems)
    {
        if (currTime)
        {
            v8 = a1->eventListStruct->listElems[a2].trackElems;
            v10 = &v8[a1->tracks[a2].currElemKeyFrame];
            while (v8 < v10)
            {
                v5 = (float)v8->duration;
                *currTime = (float)(v5 * 1.0 / 60.0 + *currTime);
                ++v8;
            }
            if (fullTime)
            {
                *fullTime = *currTime;
                v11 = &a1->eventListStruct->listElems[a2].trackElems[a1->eventListStruct->listElems[a2].elemNum];
                while (v8 < v11)
                {
                    v4 = (float)v8->duration;
                    *fullTime = (float)(v4 * 1.0 / 60.0 + *fullTime);
                    ++v8;
                }
            }
            *currTime = *currTime + a1->tracks[a2].currTime;
        }
        else if (fullTime)
        {
            v7 = a1->eventListStruct->listElems[a2].trackElems;
            v9 = &v7[a1->eventListStruct->listElems[a2].elemNum];
            while (v7 < v9)
            {
                v6 = (float)v7->duration;
                *fullTime = (float)(v6 * 1.0 / 60.0 + *fullTime);
                ++v7;
            }
        }
    }
}

void EVE_SetTracksTime(struct sAnimInner* a1, float time)
{
    float v2;
    float v3;
    tGameObjectEventElem* v4;
    tGameObjectEventElem* v5;
    float v6;
    tGameObjectEventsListElem* v7;
    uint16_t v8;
    tGameObjectEventsListElem* v9;
    tAnimTracks* v10;

    v7 = a1->eventListStruct->listElems;
    v9 = &v7[a1->eventListStruct->num];
    v10 = a1->tracks;
    while (v7 < v9)
    {
        v8 = (uint16_t)(time * 60.0);
        v4 = v7->trackElems;
        v5 = &v4[v7->elemNum];
        v10->currElemKeyFrame = 0;
        while (v4 < v5)
        {
            if (v8 < (int)v4->duration)
            {
                v2 = (float)v8;
                v3 = (float)(unsigned __int16)(time * 60.0);
                v6 = (float)(time - v3 / 60.0);
                v10->currTime = (float)(v2 / 60.0 + v6);
                v10->flag |= 4u;
                break;
            }
            v8 -= v4->duration;
            v10->currTime = 0.0;
            v10->flag |= 4u;
            ++v4;
            ++v10->currElemKeyFrame;
        }
        if (v10->currElemKeyFrame == v7->elemNum)
            v10->currElemKeyFrame = v7->elemNum - 1;
        ++v7;
        ++v10;
    }
}

void EVE_SynchronizeTracks(tGameObjectActorObject* a1)
{
    tAnimInner* a1a;
    float a3;

    a1a = a1->animPtr[0]->ptr;
    EVE_GetTracksTime(a1a, 0, &a3, 0);
    EVE_SetTracksTime(a1a, a3);
    if (a1->flagsEgBlendIw2 != 1)
        a1->flagsEgBlendIw2 = 1;
}

void EVE_FreeTrack(tGameObjectEventsListElem* a1)
{
    if (a1->trackElems)
    {
        if (a1->dataPtr)
            MEM_Free(a1->dataPtr);
        MEM_Free(a1->trackElems);
        a1->trackElems = 0;
        a1->dataPtr = 0;
    }
}

void EVE_DeleteListTracks(tGameObjectEventsList* a1)
{
    unsigned int i;

    if (a1)
    {
        if (ENG_gb_ExitApplication && a1->usageCount > 1000000u)
            a1->usageCount -= 1000000;
        if (!--a1->usageCount || a1->usageCount == -1)
        {
            for (i = 0; i < a1->num; ++i)
                EVE_FreeTrack(&a1->listElems[i]);
            if (a1->listElems)
                MEM_Free(a1->listElems);
            LOA_DeleteAddress((int)a1);
            MEM_Free(a1);
        }
    }
}

void EVE_FreeMainStruct(tGameObjectMain* a1)
{
    tAnimInner* v1;

    v1 = a1->extended->eve;
    if (v1)
    {
        if (v1->tracks)
            MEM_Free(v1->tracks);
        if (v1->eventListStruct)
            EVE_DeleteListTracks(v1->eventListStruct);
        MEM_Free(v1);
        a1->extended->eve = 0;
    }
}

/*
elemPtr felépítése [keyframe]
0..1 [short] a dataptr pozíciója, ahol az adatunk található, ha páratlan szám, akkor pozicionálnuk kell a dataptr [(short >> 1) - 4 * keyframeID] helyre
2..3 [short] flags
.... a flagnek megfelelõ többi adat, vektorok, quaternionok, mátrixok
....
*/

byte* EVE_p_GetPdata(tGameObjectEventElem* a1)
{
    byte* result;

    // (*((uint16_t *)(a1->elemPtr)))

    if (((*((uint16_t *)(a1->elemPtr))) & 1) != 0) // &1 != 0: pozíció van bent, át kell rá ugrani
        result = &EVE_gpst_CurrentTrack->dataPtr[(((*((uint16_t*)(a1->elemPtr))) >> 1) - 4) * (a1 - EVE_gpst_CurrentTrack->trackElems)];
    else
        result = a1->elemPtr + 4;
    return result;
}

// TODO FIXME ezt leelenõrizni, mert lehet nem jó
uint16_t EVE_w_Event_InterpolationKey_GetType(struct sGameObjectEventElem* a1)
{
    // ha az elemptr elso ket bajta & 0x1 != 0, akkor csak a felso ket bajt kell nekunk, ami a tipus
    // ha nem, es az adatterulet amire az elemptr mutat nem 0, akkor az egy memoriacim, amit ha 2-vel eltolunk (mivel a felso 2 bajt kell [tipus-ra mutat]), megkapjuk
    if (((*((uint16_t *)(a1->elemPtr))) & 1) != 0)
        return ((*((uint32_t *)(a1->elemPtr))) >> 16) & 0xffff;
    if ((*((uint32_t*)(a1->elemPtr))))
        return *(uint16_t*)((*((uint32_t*)(a1->elemPtr))) + 2);

    return 0;
}

Quat4* EVE_pst_Event_InterpolationKey_GetQuaternion(tGameObjectEventElem* a1)
{
    int v2;
    byte* v3;

    v2 = EVE_w_Event_InterpolationKey_GetType(a1);
    if (v2 == -1 || (v2 & 0x10) == 0)
        return 0;
    v3 = EVE_p_GetPdata(a1);
    if ((v2 & 3) != 0)
    {
        v3 = ((char*)v3 + 12);
        if ((v2 & 3u) > 1)
        {
            v3 = ((char*)v3 + 12);
            if ((v2 & 3u) > 2)
                v3 = ((char*)v3 + 12);
        }
    }
    return (Quat4 *)v3;
}

int EVE_Anim_InterpolationKey_CopyMatrix(Mtx4x4* a1, tGameObjectEventElem* a2)
{
    Quat4* v2;
    int result;

    v2 = EVE_pst_Event_InterpolationKey_GetQuaternion(a2);
    MATH_ConvertQuaternionToMatrix(a1, v2);
    result = a1->mtxflag | 4;
    a1->mtxflag = result;
    return result;
}

byte* EVE_pst_Event_InterpolationKey_GetNextValue(tGameObjectEventElem* a1)
{
    int v2;
    byte* v3;

    v2 = EVE_w_Event_InterpolationKey_GetType(a1);
    if (v2 == -1 || (v2 & 0x40) == 0)
        return 0;
    v3 = EVE_p_GetPdata(a1);
    if ((v2 & 3) != 0)
    {
        v3 += sizeof(Vec3);
        if ((v2 & 3u) > 1)
        {
            v3 += sizeof(Vec3);
            if ((v2 & 3u) > 2)
                v3 += sizeof(Vec3);
        }
    }
    else if ((v2 & 4) != 0)
    {
        v3 += 68; // Mtx4x4, de az utolsó 3 int értelmetlenséget nem tárolja le, nézd a Load metódust, FIXME: meg kéne nézni a loadnál, hogy nem-e írjuk túl a buffert, 
        //mivel a ReadMatrix mind a 80 bájtot beolvassa és leteszi, mi viszont csak kevesebbel növeljük az offsetet így azok az adatok felülíródnak
    }
    else if ((v2 & 0x10) != 0)
    {
        v3 += sizeof(Quat4);
    }
    return v3;
}

Mtx4x4* EVE_pst_Event_InterpolationKey_GetRotation(tGameObjectEventElem* a1)
{
    int v2;
    byte* v3;

    v2 = EVE_w_Event_InterpolationKey_GetType(a1);
    if (v2 == -1 || (v2 & 4) == 0)
        return 0;
    v3 = EVE_p_GetPdata(a1);
    if ((v2 & 3) != 0)
    {
        v3 += sizeof(Vec3);
        if ((v2 & 3u) > 1)
        {
            v3 += sizeof(Vec3);
            if ((v2 & 3u) > 2)
                v3 += sizeof(Vec3);
        }
    }
    return (Mtx4x4*)v3;
}

void EVE_Event_InterpolationKey_CopyMatrix(Mtx4x4* a1, tGameObjectEventElem* a2, int a3)
{
    Mtx4x4* v3;
    Mtx4x4* v4;
    Quat4* v5;
    Quat4 a2a;
    __int16 v7;
    int v8;

    v7 = EVE_w_Event_InterpolationKey_GetType(a2);
    if ((v7 & 0x14) != 0)
    {
        v8 = a2->flags2 & 0x20;
        if ((v7 & 4) != 0)
        {
            if (a3)
            {
                v4 = (Mtx4x4*)EVE_pst_Event_InterpolationKey_GetNextValue(a2);
                if (a1 != v4)
                    memcpy(a1, v4, sizeof(Mtx4x4));
            }
            else
            {
                v3 = (Mtx4x4*)EVE_pst_Event_InterpolationKey_GetRotation(a2);
                if (a1 != v3)
                    memcpy(a1, v3, sizeof(Mtx4x4));
            }
            if (v8)
            {
                MATH_ConvertMatrixToQuaternion(&a2a, a1);
                a2a.q2 = -a2a.q2;
                a2a.q3 = -a2a.q3;
                MATH_ConvertQuaternionToMatrix(a1, &a2a);
            }
        }
        else if ((v7 & 0x10) != 0)
        {
            if (a3)
                v5 = (Quat4*)EVE_pst_Event_InterpolationKey_GetNextValue(a2);
            else
                v5 = EVE_pst_Event_InterpolationKey_GetQuaternion(a2);
            if (v8)
            {
                v5->q2 = -v5->q2;
                v5->q3 = -v5->q3;
            }
            MATH_ConvertQuaternionToMatrix(a1, v5);
            if (v8)
            {
                v5->q2 = -v5->q2;
                v5->q3 = -v5->q3;
            }
            MATH_SetCorrectType(a1);
        }
    }
}

byte* EVE_pf_Event_InterpolationKey_GetTime(tGameObjectEventElem* a1)
{
    int v2;
    byte* v3;

    v2 = EVE_w_Event_InterpolationKey_GetType(a1);
    if (v2 == -1 || (v2 & 8) == 0)
        return 0;
    v3 = EVE_p_GetPdata(a1);
    if ((v2 & 3) != 0)
    {
        v3 += sizeof(Vec3);
        if ((v2 & 3u) > 1)
        {
            v3 += sizeof(Vec3);
            if ((v2 & 3u) > 2)
                v3 += sizeof(Vec3);
        }
    }
    if ((v2 & 4) != 0)
        v3 += 68; // Mtx4x4 a felesleges byteok nélkül
    if ((v2 & 0x10) != 0)
        v3 += sizeof(Quat4);
    return v3;
}

float  EVE_f_Event_InterpolationKey_InterpolateTime(tGameObjectEventElem* a1, float a2)
{
    tEventTimeElem* v3;
    float v4;
    float v5;
    uint32_t v6;
    tEventTimeElem* v7;
    uint32_t* v8;

    v7 = (tEventTimeElem*)EVE_pf_Event_InterpolationKey_GetTime(a1);
    v8 = (uint32_t*)v7;
    v6 = *(uint32_t*)&(v7->start);
    v7->start = 0.0;
    v3 = &v7[v6 - 1];
    v4 = 0.0;
    while (v7 < v3 && v7[1].start <= (double)a2)
    {
        v4 = (v7[1].start - v7->start) * v7->multiplicator + v4;
        ++v7;
    }
    v5 = (a2 - v7->start) * v7->multiplicator + v4;
    *v8 = v6;
    return v5;
}

void EVE_Event_AIFunction_Resolve(tGameObjectEventElem* a1)
{
    tAINode* v1;

    if (a1->elemPtr)
    {
        v1 = (tAINode*)(a1->elemPtr + 4); /// TODO FIXME ezt megnézni, jó????
        if (a1->elemPtr != (byte*)-4)
        {
            while (v1->category != 31)
            {
                if (v1->category == 1)
                {
                    v1->val = AI_ul_ResolveOneRef(v1->s1, v1->val);
                    AI_b_IsARef(v1->s1);
                }
                if (!AI_SpecialWParam(v1->category))
                    v1->s1 = AI_gaw_EnumLink[v1->s1];
                v1->category = (char)AI_gaw_EnumLink[v1->category];
                ++v1;
            }
        }
    }
}

int EVE_Event_AIFunction_Load(tGameObjectEventElem* a1, char* a2, byte* a3)
{
    tAINode* a2a;
    unsigned int i;
    unsigned int v6;
    char* a1a;

    a1a = a2;
    v6 = LOA_ReadLong(&a1a, 0, 1);
    if (v6)
    {
        a1->elemPtr = a3;
        a2a = (tAINode*)(a1->elemPtr + 4);
        for (i = 0; i < v6; ++i)
            LOA_ReadAINode(&a1a, a2a++, 1);
        *((uint16_t*)(a1->elemPtr)) = (uint16_t)((uint32_t)a1a - (uint32_t)a2);
    }
    return a1a - a2;
}

void EVE_Anim_ComputeInterpoledMatrix(tGameObjectMain* a1, Mtx4x4* a2, tGameObjectEventElem* a3, tGameObjectEventElem* a4, float a5)
{
    float v5;
    float v6;
    float a4a;
    Quat4* v8;
    Quat4* a3a;

    if (a3->duration)
    {
        v6 = (float)a3->duration;
        if (v6 * 1.0 / 60.0 > a5)
        {
            v8 = EVE_pst_Event_InterpolationKey_GetQuaternion(a3);
            a3a = EVE_pst_Event_InterpolationKey_GetQuaternion(a4);
            v5 = (float)a3->duration;
            a4a = (float)(a5 / (v5 * 1.0 / 60.0));
            MATH_QuaternionBlend(a2, v8, a3a, a4a, 1u);
        }
        else
        {
            EVE_Anim_InterpolationKey_CopyMatrix(a2, a4);
        }
    }
    else
    {
        EVE_Anim_InterpolationKey_CopyMatrix(a2, a3);
    }
}

void EVE_Event_ComputeInterpoledMatrix(tGameObjectMain* a1, Mtx4x4* a2, tGameObjectEventElem* a3, tGameObjectEventElem* a4, float a5)
{
    Mtx4x4* v5;
    Mtx4x4* v6;
    Mtx4x4* v7;
    float v8;
    float v9;
    __int16 v10;
    Quat4 v11;
    float a4a;
    Quat4 v13;
    Quat4 v14;
    Quat4* a2a;
    Quat4* a3a;
    __int16 v17;
    Quat4 v18;

    v10 = EVE_w_Event_InterpolationKey_GetType(a3);
    v17 = EVE_w_Event_InterpolationKey_GetType(a4);
    if (v10 != -1 && (v10 & 0x14) != 0)
    {
        if (!a3->duration || v17 == -1 || (v17 & 0x14) == 0 || (a3->flags2 & 0x800) != 0)
        {
            EVE_Event_InterpolationKey_CopyMatrix(a2, a3, 0);
        }
        else
        {
            v9 = (float)a3->duration;
            if (v9 * 1.0 / 60.0 > a5)
            {
                if ((v10 & 4) != 0)
                {
                    v5 = (Mtx4x4*)EVE_pst_Event_InterpolationKey_GetRotation(a3);
                    MATH_ConvertMatrixToQuaternion(&v18, v5);
                    a2a = &v18;
                }
                else                                    // EVE_Event_InterpolationKey_CopyMatrix (,,1) és (,,0) inline 2 db
                {
                    a2a = EVE_pst_Event_InterpolationKey_GetQuaternion(a3);
                }
                if ((a3->flags2 & 0x20) != 0)
                {
                    v13.q4 = a2a->q4;
                    v13.q1 = a2a->q1;
                    v13.q2 = -a2a->q2;
                    v13.q3 = -a2a->q3;
                    a2a = &v13;
                }
                if ((v10 & 0x40) != 0)
                {
                    if ((v10 & 4) != 0)
                    {
                        v6 = (Mtx4x4*)EVE_pst_Event_InterpolationKey_GetNextValue(a3);
                        MATH_ConvertMatrixToQuaternion(&v11, v6);
                        a3a = &v11;
                    }
                    else
                    {
                        a3a = (Quat4*)EVE_pst_Event_InterpolationKey_GetNextValue(a3);
                    }
                }
                else if ((v17 & 4) != 0)
                {
                    v7 = (Mtx4x4*)EVE_pst_Event_InterpolationKey_GetRotation(a4);
                    MATH_ConvertMatrixToQuaternion(&v11, v7);
                    a3a = &v11;
                }
                else
                {
                    a3a = EVE_pst_Event_InterpolationKey_GetQuaternion(a4);
                }
                if ((a4->flags2 & 0x20) != 0)
                {
                    v14.q4 = a3a->q4;
                    v14.q1 = a3a->q1;
                    v14.q2 = -a3a->q2;
                    v14.q3 = -a3a->q3;
                    a3a = &v14;
                }
                v8 = (float)a3->duration;
                a4a = (float)(a5 / (v8 * 1.0 / 60.0));
                MATH_QuaternionBlend(a2, a2a, a3a, a4a, 1u);
            }
            else if ((v10 & 0x40) != 0)
            {
                EVE_Event_InterpolationKey_CopyMatrix(a2, a3, 1);
            }
            else
            {
                EVE_Event_InterpolationKey_CopyMatrix(a2, a4, 0);
            }
        }
    }
}

Vec3* EVE_pst_Event_InterpolationKey_GetPos(tGameObjectEventElem* a1)
{
    Vec3* result;
    int v2;

    v2 = EVE_w_Event_InterpolationKey_GetType(a1);
    if (v2 != -1 && (v2 & 3) != 0)
        result = (Vec3*)EVE_p_GetPdata(a1);
    else
        result = 0;
    return result;
}

void EVE_Anim_TranslationKey_Play(tGameObjectEventElem* a2)
{
    tGaoMatrix* v2;
    Mtx4x4 v3;
    Mtx4x4 v4;
    Vec3* v5;
    Vec3* v6;
    Vec3* v7;
    Vec3* v8;
    Vec3 v9;
    Vec3 v10;
    float v11;
    float v12;
    float v13;
    int v14;
    Vec3* v15;
    Vec3* v16;
    BOOL v17;
    tGameObjectMain* v18;
    tGaoMatrix* v19;
    tGameObjectMain* v20;
    BOOL v21;
    float v22;
    tGameObjectEventElem* v23;
    int v24;

    v24 = EVE_gpst_CurrentParam->currElemKeyFrame + 1;
    if (v24 == EVE_gpst_CurrentTrack->elemNum)
        v24 = 0;
    v23 = &EVE_gpst_CurrentTrack->trackElems[v24];
    v22 = EVE_gpst_CurrentParam->currTime;
    v21 = EVE_gpst_CurrentTrack->boneID != 0xFFFF;
    if (v21)
    {
        if (EVE_gpst_CurrentTrack->boneID >= ((tGameObjectActorObject*)(EVE_gpst_CurrentGAO->basic->actorObjectOrVisualObject))->skeletonGroup->tablePF->actElemNum)
            return;
        v20 = (tGameObjectMain*)((tableElemStructPF*)(((tGameObjectActorObject*)(EVE_gpst_CurrentGAO->basic->actorObjectOrVisualObject))->skeletonGroup->tablePF->firstElemPtr))[EVE_gpst_CurrentTrack->boneID].objectPtr;        
        if (v20->flags2 < 0
            || v20->extended
            && (((tGameObjectActorObject*)(EVE_gpst_CurrentGAO->basic->actorObjectOrVisualObject))->lod & 8) != 0
            && (v20->extended->extraFlag & 0x20) != 0)
        {
            return;
        }
        if (v20->extended && (v20->extended->extraFlag & 0x200) != 0)
            return;
    }
    else
    {
        v20 = EVE_gpst_CurrentGAO;
    }
    if ((v20->flags & 0x400000) != 0)
    {
        v19 = v20->basic->mtx;
        if (v20 && v20->basic && v20->basic->mtx)
            v18 = v20->basic->mtx->refGao2;
        else
            v18 = 0;
        v17 = EVE_gpst_CurrentGAO == v18;
    }
    else
    {
        v19 = (tGaoMatrix*)OBJ_pst_GetAbsoluteMatrix(v20);
        v17 = 0;
    }
    v16 = EVE_pst_Event_InterpolationKey_GetPos(a2);
    if (a2->duration)
    {
        v15 = EVE_pst_Event_InterpolationKey_GetPos(v23);
        v14 = a2->duration;
        v13 = (float)v14;
        v12 = (float)(v22 / (v13 * 1.0 / 60.0));
        v11 = (float)(1.0 - v12);
        v10.x = v11 * v16->x;
        v10.y = v16->y * v11;
        v10.z = v16->z * v11;
        v9.x = v12 * v15->x;
        v9.y = v15->y * v12;
        v9.z = v15->z * v12;
        MATH_VecAddVec(&v10, &v10, &v9);
    }
    else
    {
        v8 = &v10;
        v7 = v16;
        v10 = *v16;
    }
    if (v21)
    {
        if (v17 && EVE_gpst_CurrentRefMatrix)
        {
            if ((EVE_gpst_CurrentGAO->flags & 0x400000) != 0)
                v2 = OBJ_pst_GetLocalMatrix(EVE_gpst_CurrentGAO);
            else
                v2 = (tGaoMatrix*)OBJ_pst_GetAbsoluteMatrix(EVE_gpst_CurrentGAO);
            MATH_InvertMatrix(&v4, &v2->mtx);
            MATH_MulMatrixMatrix(&v3, EVE_gpst_CurrentRefMatrix, &v4);
            MATH_TransformVertex(&v10, &v3, &v10);
            if ((EVE_gpst_CurrentGAO->flags & 2) != 0)
                v10.z = v10.z + ((tGameObjectActorObject*)(EVE_gpst_CurrentGAO->basic->actorObjectOrVisualObject))->aniPelvisOffset;
        }
        v19->mtx.Pos = v10;
        v19->mtx.mtxflag |= 2u;
        v19->mtx.mtxflag |= 2u;
    }
    else
    {
        MATH_TransformVertex(&v10, EVE_gpst_CurrentRefMatrix, &v10);
        v6 = &v19->mtx.Pos;
        v5 = &v10;
        v19->mtx.Pos.x = v10.x;
        v6->y = v5->y;
        v6->z = v5->z;
        v19->mtx.mtxflag |= 2u;
    }
}

void EVE_Anim_QuaternionKey_Play(tGameObjectEventElem* a2)
{
    tGaoMatrix* v2;
    Mtx4x4 v3;
    Mtx4x4 v4;
    Mtx4x4 v5;
    Mtx4x4 v6;
    BOOL v7;
    tGameObjectMain* v8;
    tGaoMatrix* v9;
    tGameObjectMain* v10;
    BOOL v11;
    float v12;
    tGameObjectEventElem* v13;
    int v14;

    
    v14 = EVE_gpst_CurrentParam->currElemKeyFrame + 1;
    if (v14 == EVE_gpst_CurrentTrack->elemNum)
        v14 = 0;
    v13 = &EVE_gpst_CurrentTrack->trackElems[v14];
    v12 = EVE_gpst_CurrentParam->currTime;
    v11 = EVE_gpst_CurrentTrack->boneID != 0xFFFF;
    if (v11)
    {
        if (EVE_gpst_CurrentTrack->boneID >= ((tGameObjectActorObject*)(EVE_gpst_CurrentGAO->basic->actorObjectOrVisualObject))->skeletonGroup->tablePF->actElemNum)
            return;
        v10 = (tGameObjectMain*)((tableElemStructPF*)(((tGameObjectActorObject*)(EVE_gpst_CurrentGAO->basic->actorObjectOrVisualObject))->skeletonGroup->tablePF->firstElemPtr))[EVE_gpst_CurrentTrack->boneID].objectPtr;
        if (v10->flags2 < 0
            || v10->extended
            && (((tGameObjectActorObject*)(EVE_gpst_CurrentGAO->basic->actorObjectOrVisualObject))->lod & 8) != 0
            && (v10->extended->extraFlag & 0x20) != 0)
        {
            return;
        }
        if (v10->extended && (v10->extended->extraFlag & 0x400) != 0)
            return;
    }
    else
    {
        v10 = EVE_gpst_CurrentGAO;
    }
    if ((v10->flags & 0x400000) != 0)
    {
        v9 = v10->basic->mtx;
        if (v10 && v10->basic && v10->basic->mtx)
            v8 = v10->basic->mtx->refGao2;
        else
            v8 = 0;
        v7 = EVE_gpst_CurrentGAO == v8;
    }
    else
    {
        v9 = (tGaoMatrix*)OBJ_pst_GetAbsoluteMatrix(v10);
        v7 = 0;
    }
    EVE_Anim_ComputeInterpoledMatrix(v10, &v6, a2, v13, v12);
    if (v11)
    {
        if (v7 && EVE_gpst_CurrentRefMatrix)
        {
            if ((EVE_gpst_CurrentGAO->flags & 0x400000) != 0)
                v2 = OBJ_pst_GetLocalMatrix(EVE_gpst_CurrentGAO);
            else
                v2 = (tGaoMatrix*)OBJ_pst_GetAbsoluteMatrix(EVE_gpst_CurrentGAO);
            MATH_InvertMatrix(&v4, &v2->mtx);
            MATH_MulMatrixMatrix(&v3, EVE_gpst_CurrentRefMatrix, &v4);
            MATH_Mul33MatrixMatrix(&v5, &v6, &v3, 1);
        }
        else
        {
            MATH_Copy33Matrix(&v5, &v6);
        }
        MATH_Copy33Matrix(&v9->mtx, &v5);
    }
    else
    {
        MATH_Mul33MatrixMatrix(&v5, &v6, EVE_gpst_CurrentRefMatrix, 1);
        MATH_Copy33Matrix(&v9->mtx, &v5);
    }
    v9->mtx.mtxflag |= 4u;
}

int EVE_RunAnimTrack(tGameObjectEventsListElem* a1, tAnimTracks* a2, unsigned int a3, char a4, float* a5)
{
    int result;
    float v6;
    float v7;
    float v8;
    float v9;
    tGameObjectEventElem* a1a;
    float v11;

    if (a2->currElemKeyFrame == a1->elemNum - 1)
    {
        if (a1->boneID == 255)
        {
            EVE_RunningTracksAtStart(EVE_gpst_CurrentData);
        }
        else
        {
            a2->flag &= ~4u;
            a2->currElemKeyFrame = 0;
            a2->currTime = 0.0;
        }
    }
    v8 = TIM_gf_dt;
    while (1)
    {
        a1a = &a1->trackElems[a2->currElemKeyFrame];
        EVE_gpst_CurrentEvent = a1a;
        v7 = (float)a1a->duration;
        v11 = (float)(v7 * 1.0 / 60.0);
        if ((a1a->flags2 & 4) != 0 && (a2->flag & 4) == 0)
            EVE_FlashMatrix();
        if (v11 - 0.0000099999997 >= v8 + a2->currTime)
            break;
        v9 = v8 - (v11 - a2->currTime);
        if (v9 <= 0.0)
            v6 = 0.0;
        else
            v6 = v9;
        v8 = v6;
        if (a2->currElemKeyFrame == a1->elemNum - 1)
        {
            if ((EVE_w_Event_InterpolationKey_GetType(a1a) & 0x14) != 0)
                EVE_Anim_QuaternionKey_Play(a1a);
            else
                EVE_Anim_TranslationKey_Play(a1a);
            a2->currElemKeyFrame = a1->elemNum - 1;
            a2->currTime = 0.0;
            a2->flag &= ~4u;
            *a5 = v6;
            return 1;
        }
        a2->currTime = 0.0;
        ++a2->currElemKeyFrame;
        a2->flag &= ~4u;
    }
    a2->currTime = v8 + a2->currTime;
    if (a3 > 1 && (a4 & 2) != 0 && (a4 & 0x10) != 0)
    {
        a2->flag |= 4u;
        result = 0;
    }
    else
    {
        if ((EVE_w_Event_InterpolationKey_GetType(a1a) & 0x14) != 0)
            EVE_Anim_QuaternionKey_Play(a1a);
        else
            EVE_Anim_TranslationKey_Play(a1a);
        a2->flag |= 4u;
        result = 0;
    }
    return result;
}

byte EVE_c_UpdateLOD(tGameObjectMain* a1, int a2)
{
    byte result; // al
    unsigned __int8 v3; // [esp+3h] [ebp-5h]
    tGameObjectActorObject* v4; // [esp+4h] [ebp-4h]

    v4 = (tGameObjectActorObject*)a1->basic->actorObjectOrVisualObject;
    v3 = v4->lod & 0xEF;
    if (a2)
        v4->lod = (v4->lod & 0x10) != 0 ? 0 : 16;
    if (v4->ANILOD)
    {
        v4->lod |= v4->ANILOD | 0x80;
        if (v3 > (v4->lod & 0xEF))
            EVE_SynchronizeTracks(v4);
        result = v4->lod;
    }
    else
    {
        v4->lod |= v4->aniBitFieldLOD & 0x80;
        if (a1->distance < 0xAu)
            v4->lod |= v4->aniBitFieldLOD & 1;
        if (a1->distance < 0x19u)
            v4->lod |= v4->aniBitFieldLOD & 2;
        if (a1->distance < 0x64u)
            v4->lod |= v4->aniBitFieldLOD & 0xC;
        if (a1->extended && (a1->extended->extraFlag & 0x80) != 0)
            v4->lod |= 4u;
        if (v3 > (v4->lod & 0xEF))
            EVE_SynchronizeTracks(v4);
        result = v4->lod;
    }
    return result;
}

tGameObjectEventElem* EVE_Event_InterpolationKey_Play(tGameObjectEventElem* a3)
{
    Mtx4x4* v3;
    tGaoMatrix* v4;
    Mtx4x4 v5;
    Mtx4x4 v6;
    tGameObjectMain* v7; 
    float v8;
    Mtx4x4* v9;
    float v10;
    Mtx4x4* v11;
    Mtx4x4 v12; 
    Mtx4x4 v13; 
    Mtx4x4 v14; 
    tGameObjectMain* v15; 
    tGameObjectMain* v16 = NULL; // TODO ?????
    Mtx4x4 v17;
    Mtx4x4 v18;
    Mtx4x4 v19;
    Mtx4x4 v20;
    Vec3* v21; 
    Vec3* v22;
    Mtx4x4 v23; 
    Mtx4x4 v24; 
    Vec3* v25;
    Vec3* v26;
    Vec3* v27;
    Vec3* v28;
    Vec3* v29;
    Vec3* v30;
    Vec3* v31;
    Vec3* v32;
    Vec3* v33;
    Vec3* v34;
    Vec3 v35; 
    float v36;
    Vec3 v37; 
    float v38;
    Vec3 v39; 
    Vec3* v40;
    float v41;
    float v42;
    Vec3 v43; 
    float v44;
    Vec3 v45; 
    Vec3* v46;
    float v47;
    Vec3 v48; 
    Vec3* v49;
    float v50;
    float v51;
    float v52;
    float v53;
    int v54; 
    float* v55;
    Vec3* v56; 
    Vec3 v57; 
    float v58;
    int v59; 
    float v60[3];
    float* v61;
    Vec3 v62; 
    Vec3* v63;
    tGameObjectMain* v64;
    float v65;
    __int16 v66;
    tGameObjectEventElem* v67;
    int v68;
    BOOL v69;
    tGameObjectMain* v70;
    tGaoMatrix* v71;
    int v72; 
    int v73; 
    tGameObjectMain* v74; 
    tableElemStructPF* v75;
    tGameObjectActorObject* v76;
    BOOL v77;
    __int16 v78;

    if (!EVE_gpst_CurrentTrack)
        return a3;
    v78 = EVE_w_Event_InterpolationKey_GetType(a3);
    if (v78 == -1)
        return a3;
    v77 = EVE_gpst_CurrentTrack->boneID != 0xFFFF;
    if (v77)
    {
        if (!EVE_gpst_CurrentGAO->basic
            || !EVE_gpst_CurrentGAO->basic->actorObjectOrVisualObject
            || !((tGameObjectActorObject*)(EVE_gpst_CurrentGAO->basic->actorObjectOrVisualObject))->skeletonGroup)
        {
            return a3;
        }
        v76 = EVE_gpst_CurrentGAO->basic->actorObjectOrVisualObject;
        if (EVE_gpst_CurrentTrack->boneID >= v76->skeletonGroup->tablePF->actElemNum)
            return a3;
        v75 = &((tableElemStructPF*)(v76->skeletonGroup->tablePF->firstElemPtr))[EVE_gpst_CurrentTrack->boneID];
        v74 = (tGameObjectMain*)v75->objectPtr;
        if (!ENG_gb_EVERunning && v74->flags2 < 0)
            return a3;
    }
    else
    {
        v74 = EVE_gpst_CurrentGAO;
    }
    v73 = v74->flags & 0x400000;
    v72 = v78 & 0x20;
    if (v73)
    {
        v71 = v74->basic->mtx;
        if (v74 && v74->basic && v74->basic->mtx)
            v70 = v74->basic->mtx->refGao2;
        else
            v70 = 0;
        v69 = EVE_gpst_CurrentGAO == v70;
    }
    else
    {
        v71 = (tGaoMatrix*)OBJ_pst_GetAbsoluteMatrix(v74);
        v69 = 0;
    }
    v68 = EVE_gpst_CurrentParam->currElemKeyFrame + 1;
    if (v68 == EVE_gpst_CurrentTrack->elemNum)
        v68 = 0;
    v67 = &EVE_gpst_CurrentTrack->trackElems[v68];
    if ((v67->flags2 & 0x7C0) != 0)
    {
        if ((v67->flags2 & 0x7C0) != 128)
            return a3;
        v66 = EVE_w_Event_InterpolationKey_GetType(v67);
        if (v66 == -1)
            return a3;
    }
    else
    {
        v66 = 0;
    }
    if ((v78 & 8) != 0)
        v65 = EVE_f_Event_InterpolationKey_InterpolateTime(a3, EVE_gpst_CurrentParam->currTime);
    else
        v65 = EVE_gpst_CurrentParam->currTime;
    if (v77
        && v73
        && (v72 && v74->basic->mtx->refGao2 != EVE_gpst_CurrentGAO
            || !v72 && v74->basic->mtx->refGao2 != v74->basic->mtx->refGao))
    {
        if (v72)
            v64 = EVE_gpst_CurrentGAO;
        else
            v64 = v74->basic->mtx->refGao;
        v74->basic->mtx->refGao2 = v64;
        OBJ_ComputeLocalWhenHie(v74);
    }
    if ((v78 & 3) != 0)
    {
        v63 = EVE_pst_Event_InterpolationKey_GetPos(a3);
        if ((a3->flags2 & 0x20) != 0)
        {
            v62.x = -v63->x;
            v62.y = v63->y;
            v62.z = v63->z;
            v63 = &v62;
        }
        if ((v66 & 3) != 0 && a3->duration && (a3->flags2 & 0x800) == 0)
        {
            if ((v78 & 0x40) != 0)
                v61 = (float*)EVE_pst_Event_InterpolationKey_GetNextValue(a3);
            else
                v61 = (float*)EVE_pst_Event_InterpolationKey_GetPos(v67);
            if ((v67->flags2 & 0x20) != 0)
            {
                v60[0] = -*v61;
                v60[1] = v61[1];
                v60[2] = v61[2];
                v61 = v60;
            }
            v59 = a3->duration;
            v58 = (float)v59;
            if (v58 * 1.0 / 60.0 > v65)
            {
                v54 = a3->duration;
                v53 = (float)v54;
                v52 = (float)(v65 / (v53 * 1.0 / 60.0));
                if ((v78 & 3) == 3)
                {
                    v51 = (float)((1.0 - v52) * ((1.0 - v52) * (1.0 - v52)));
                    v57.x = v51 * v63->x;
                    v57.y = v63->y * v51;
                    v57.z = v63->z * v51;
                    v50 = (float)((1.0 - v52) * ((1.0 - v52) * (3.0 * v52)));
                    v49 = v63 + 1;
                    v48.x = v50 * v63[1].x;               // addscalevector inlineok
                    v48.y = v63[1].y * v50;
                    v48.z = v63[1].z * v50;
                    MATH_VecAddVec(&v57, &v57, &v48);
                    v47 = (float)((1.0 - v52) * (3.0 * v52 * v52));
                    v46 = v63 + 2;
                    v45.x = v47 * v63[2].x;
                    v45.y = v63[2].y * v47;
                    v45.z = v63[2].z * v47;
                    MATH_VecAddVec(&v57, &v57, &v45);
                    v44 = v52 * v52 * v52;
                    v43.x = v44 * *v61;
                    v43.y = v61[1] * v44;
                    v43.z = v61[2] * v44;
                    MATH_VecAddVec(&v57, &v57, &v43);
                }
                else if ((v78 & 3) == 2)
                {
                    v42 = (float)((1.0 - v52) * (1.0 - v52));
                    v57.x = v42 * v63->x;
                    v57.y = v63->y * v42;
                    v57.z = v63->z * v42;
                    v41 = (float)((1.0 - v52) * (2.0 * v52));
                    v40 = v63 + 1;
                    v39.x = v41 * v63[1].x;
                    v39.y = v63[1].y * v41;
                    v39.z = v63[1].z * v41;
                    MATH_VecAddVec(&v57, &v57, &v39);
                    v38 = v52 * v52;
                    v37.x = v38 * *v61;
                    v37.y = v61[1] * v38;
                    v37.z = v61[2] * v38;
                    MATH_VecAddVec(&v57, &v57, &v37);
                }
                else
                {
                    v36 = (float)(1.0 - v52);
                    v57.x = v36 * v63->x;
                    v57.y = v63->y * v36;
                    v57.z = v63->z * v36;
                    v35.x = v52 * *v61;
                    v35.y = v61[1] * v52;
                    v35.z = v61[2] * v52;
                    MATH_VecAddVec(&v57, &v57, &v35);
                }
            }
            else
            {
                v56 = &v57;
                v55 = v61;
                v57 = *(Vec3*)v61;
            }
        }
        else
        {
            v34 = &v57;
            v33 = v63;
            v57 = *v63;
        }
        if (v77)
        {
            if (v74->extended && (v74->extended->extraFlag & 0x200) != 0)
                return a3;
            if (v69 && EVE_gpst_CurrentRefMatrix)
            {
                v3 = (EVE_gpst_CurrentGAO->flags & 0x400000) != 0 ? (Mtx4x4*)OBJ_pst_GetLocalMatrix(EVE_gpst_CurrentGAO) : OBJ_pst_GetAbsoluteMatrix(EVE_gpst_CurrentGAO);
                MATH_InvertMatrix(&v24, v3);
                MATH_MulMatrixMatrix(&v23, EVE_gpst_CurrentRefMatrix, &v24);
                MATH_TransformVertex(&v57, &v23, &v57);
                if ((EVE_gpst_CurrentGAO->flags & 2) != 0)
                    v57.z = v57.z + ((tGameObjectActorObject*)(EVE_gpst_CurrentGAO->basic->actorObjectOrVisualObject))->aniPelvisOffset;
            }
            v22 = &v71->mtx.Pos;
            v21 = &v57;
            v71->mtx.Pos.x = v57.x;
            v22->y = v21->y;
            v22->z = v21->z;
            v71->mtx.mtxflag |= 2u;
            v71->mtx.mtxflag |= 2u;
        }
        else
        {
            if (EVE_gpst_CurrentRefMatrix && EVE_b_Track_UseFlash(EVE_gpst_CurrentTrack, 0, 0, 0))
                MATH_TransformVertex(&v57, EVE_gpst_CurrentRefMatrix, &v57);
            v32 = &v71->mtx.Pos;
            v31 = &v57;
            v71->mtx.Pos.x = v57.x;
            v32->y = v31->y;
            v32->z = v31->z;
            v71->mtx.mtxflag |= 2u;
            if ((a3->flags2 & 0x800) != 0
                && (v74->flags & 0x200) != 0
                && v74->extended->coll
                && v74->extended->coll->colInstance
                && v74->extended->coll->colInstance->mtx)
            {
                if (v73)
                {
                    v30 = &v74->extended->coll->colInstance->mtx->Pos;
                    v29 = v30;
                    v28 = &v57;
                    v30->x = v57.x;
                    v29->y = v28->y;
                    v29->z = v28->z;
                }
                else
                {
                    v27 = &v74->extended->coll->colInstance->mtx->Pos;
                    v26 = v27;
                    v25 = &v57;
                    v27->x = v57.x;
                    v26->y = v25->y;
                    v26->z = v25->z;
                }
            }
        }
    }
    if ((v78 & 0x14) != 0)
    {
        EVE_Event_ComputeInterpoledMatrix(v74, &v20, a3, v67, v65);
        if (v77)
        {
            if (v74->extended && (v74->extended->extraFlag & 0x400) != 0)
                return a3;
            if (v69 && EVE_gpst_CurrentRefMatrix)
            {
                if ((EVE_gpst_CurrentGAO->flags & 0x400000) != 0)
                    v4 = OBJ_pst_GetLocalMatrix(EVE_gpst_CurrentGAO);
                else
                    v4 = (tGaoMatrix*)OBJ_pst_GetAbsoluteMatrix(EVE_gpst_CurrentGAO);
                MATH_InvertMatrix(&v18, &v4->mtx);
                MATH_MulMatrixMatrix(&v17, EVE_gpst_CurrentRefMatrix, &v18);
                MATH_Mul33MatrixMatrix(&v19, &v20, &v17, 1);
            }
            else
            {
                MATH_Copy33Matrix(&v19, &v20);
            }
            MATH_Copy33Matrix(&v71->mtx, &v19);
        }
        else
        {
            if (EVE_gpst_CurrentRefMatrix && EVE_b_Track_UseFlash(EVE_gpst_CurrentTrack, 0, 0, 0))
                MATH_Mul33MatrixMatrix(&v19, &v20, EVE_gpst_CurrentRefMatrix, 1);
            else
                MATH_Copy33Matrix(&v19, &v20);
            MATH_Copy33Matrix(&v71->mtx, &v19);
        }
        v71->mtx.mtxflag |= 4u;
    }
    if (v77 && v72 && (v78 & 3) != 0)
    {
        if (OBJ_b_IsChild(v74))
        {
            if (v74 && v74->basic && v74->basic->mtx)
                v16 = v74->basic->mtx->refGao;
            else
                v16 = 0;
        }
        if (v16 && OBJ_b_IsChild(v16))
        {
            v15 = v16->basic && v16->basic->mtx ? v16->basic->mtx->refGao : 0;
            if (v15)
            {
                OBJ_ComputeGlobalWithLocal(v15, &v14, 1);
                OBJ_ComputeGlobalWithLocal(v16, &v13, 1);
                OBJ_ComputeGlobalWithLocal(v74, &v12, 1);
                if ((v16->flags & 0x10000) != 0)
                    v11 = v16->matrices + 1;
                else
                    v11 = v16->matrices;
                v10 = MATH_VecLen(&v11->Pos);
                if ((v74->flags & 0x10000) != 0)
                    v9 = v74->matrices + 1;
                else
                    v9 = v74->matrices;
                v8 = MATH_VecLen(&v9->Pos);
                GAO_ModifierLegLink_Compute(&v14, &v13, &v12, v10, v8);
                if (OBJ_b_IsChild(v15))
                {
                    v7 = v15 && v15->basic && v15->basic->mtx ? v15->basic->mtx->refGao2 : 0;
                    if (v7)
                    {
                        OBJ_ComputeGlobalWithLocal(v7, &v6, 1);
                        MATH_InvertMatrix(&v5, &v6);
                        MATH_MulMatrixMatrix(&v15->basic->mtx->mtx, &v14, &v5);
                        MATH_Orthonormalize(&v15->basic->mtx->mtx);
                    }
                }
                MATH_InvertMatrix(&v5, &v14);
                MATH_MulMatrixMatrix(&v16->basic->mtx->mtx, &v13, &v5);
                MATH_Orthonormalize(&v16->basic->mtx->mtx);
                if (OBJ_b_IsChild(v74))
                {
                    if (v74 && v74->basic && v74->basic->mtx)
                        v7 = v74->basic->mtx->refGao2;
                    else
                        v7 = 0;
                    if (v7)
                    {
                        OBJ_ComputeGlobalWithLocal(v7, &v6, 1);
                        MATH_InvertMatrix(&v5, &v6);
                        MATH_MulMatrixMatrix(&v74->basic->mtx->mtx, &v12, &v5);
                        MATH_Orthonormalize(&v74->basic->mtx->mtx);
                    }
                }
            }
        }
    }
    return a3;
}

int EVE_RunOneTrack(tGameObjectEventsListElem* a1, tAnimTracks* a2, int a3, float* a4)
{
    float v5;
    float v6;
    unsigned int v7;
    float v8;
    float v9;
    float v10;
    float v11;
    float a3a;
    float v13;
    tGameObjectEventElem* v14;
    int v15;
    tGameObjectEventElem* v16;

    if (!a1->trackElems)
        return 0;
    EVE_gpst_SavedMatrix = EVE_gpst_OwnerGAO->matrices;
    if ((a1->flags & 8) != 0)
        a2->blendFactor = -1;
    if (a3)
    {
        if (TIM_gf_dt != 0.0 && a2->currElemKeyFrame == a1->elemNum - 1)
        {
            v11 = (float)a1->trackElems[a2->currElemKeyFrame].duration;
            if (v11 * 1.0 / 60.0 == a2->currTime && (a1->flags & 2) != 0)
            {
                if (a1->boneID == 255)
                {
                    EVE_RunningTracksAtStart(EVE_gpst_CurrentData);
                }
                else
                {
                    a2->currElemKeyFrame = 0;
                    a2->flag &= ~4u;
                }
            }
        }
    }
    v13 = TIM_gf_dt;
    while (1)
    {
        while (1)
        {
            while (1)
            {
                v14 = &a1->trackElems[a2->currElemKeyFrame];
                EVE_gpst_CurrentEvent = v14;
                EVE_gf_ElapsedDT = TIM_gf_dt - v13;
                v10 = (float)v14->duration;
                if (v10 * 1.0 / 60.0 >= v13 + a2->currTime)
                {
                    EVE_gf_DT = v13;
                    a2->currTime = v13 + a2->currTime;
                    v13 = 0.0;
                }
                else
                {
                    v9 = (float)v14->duration;
                    EVE_gf_DT = (float)(v9 * 1.0 / 60.0 - a2->currTime);
                    v13 = v13 - EVE_gf_DT;
                    v8 = (float)v14->duration;
                    a2->currTime = (float)(v8 * 1.0 / 60.0);
                }
                if ((v14->flags2 & 4) != 0
                    && (a2->flag & 4) == 0
                    && (EVE_gpst_CurrentListTracks->flags & 1) == 0
                    && TIM_gf_dt >= 0.0)
                {
                    EVE_FlashMatrix();
                }
                if ((v14->flags2 & 1) != 0 && (a2->flag & 4) != 0)
                    goto LABEL_51;
                v7 = v14->flags2 & 0x7C0;
                if (v7 > 0x200)
                    break;
                if (v7 != 512)
                {
                    if (v7 == 64)
                    {
                        if (!EVE_gb_PlayOnlyAnimTracks)
                        {
                            v16 = EVE_Event_AIFunction_Play(v14);
                            if ((v14->flags2 & 8) != 0)
                            {
                                if (AI_GetIntFromStack())
                                {
                                    a2->currTime = 0.0;
                                    return 0;
                                }
                            }
                            else if ((v14->flags2 & 0x10) != 0 && !AI_GetIntFromStack())
                            {
                                a2->currTime = 0.0;
                                return 0;
                            }
                        }
                    }
                    else if (v7 == 128 && (v13 == 0.0 || a2->currElemKeyFrame == a1->elemNum - 1))
                    {
                        v16 = EVE_Event_InterpolationKey_Play(v14);
                    }
                    goto LABEL_51;
                }
                if (EVE_gb_PlayOnlyAnimTracks)
                    goto LABEL_51;
                v15 = EVE_i_SearchLabel(a1, v14->flags2 & 0xF000, &a3a);
                if (v15 == -1)
                    goto LABEL_51;
                a2->currElemKeyFrame = v15;
                a2->currTime = 0.0;
            }
            if (v7 == 1024 && !EVE_gb_PlayOnlyAnimTracks)
            {
                v15 = EVE_i_SearchLabel(a1, v14->flags2 & 0xF000, &a3a);
                if (v15 != -1)
                {
                    EVE_SetAllTimesTo(a3a);
                    EVE_gb_NeedToReparse = 1;
                    return 0;
                }
            }
        LABEL_51:
            a2->flag |= 4u;
            if (v13 == 0.0)
                goto LABEL_66;
            a2->currTime = 0.0;
            if (a2->currElemKeyFrame == a1->elemNum - 1)
                break;
            ++a2->currElemKeyFrame;
            a2->flag &= ~4u;
        }
        if ((a1->flags & 2) != 0)
        {
            a2->currElemKeyFrame = 0;
            a2->flag &= ~4u;
            goto LABEL_57;
        }
        if ((a1->flags & 4) != 0)
            break;
    LABEL_57:
        if (EVE_gpst_CurrentGAO == EVE_gpst_OwnerGAO
            && (EVE_gpst_CurrentGAO->flags & 2) != 0
            && a3
            && (v14->flags2 & 0x7C0) == 128)
        {
            if (a4)
                *a4 = v13;
            v5 = (float)v14->duration;
            a2->currTime = (float)(v5 * 1.0 / 60.0);
            a2->currElemKeyFrame = a1->elemNum - 1;
            return 1;
        }
    }
    v6 = (float)v14->duration;
    a2->currTime = (float)(v6 * 1.0 / 60.0);
    a2->flag &= ~1u;
LABEL_66:
    if (a4)
        *a4 = v13;
    return 0;
}

int EVE_b_RunAnimation(tAnimInner* a1, float* a4)
{
    tGameObjectMain* v3;
    BOOL v4;
    Mtx4x4* v5;
    Mtx4x4* v6;
    int v7;
    int v8;
    float v9;
    int v10;
    BOOL v11;
    tGameObjectEventsList* v12;
    Mtx4x4* v13;
    unsigned __int16 v14;
    char v15;
    unsigned int a3;
    unsigned int a3a;
    tAnimTracks* v18;
    tGroup* v19;
    tGameObjectMain* v20;
    int v21;

    if (!a1)
        return 0;
    v12 = a1->eventListStruct;
    v18 = a1->tracks;
    if (!a1->eventListStruct || !v18)
        return 0;
    EVE_gpst_CurrentListTracks = a1->eventListStruct;
    v13 = EVE_gpst_CurrentRefMatrix;
    EVE_gpst_CurrentGAO = EVE_gpst_OwnerGAO;
    v19 = ((tGameObjectActorObject*)(EVE_gpst_OwnerGAO->basic->actorObjectOrVisualObject))->skeletonGroup;
    v4 = !EVE_gb_ForcePlayerOnAllTracks
        && ((EVE_gpst_CurrentGAO->flags2 & 4) != 0 || (EVE_gpst_CurrentGAO->flags2 & 2) == 0);
    v8 = v4;
    if (EVE_gpst_CurrentGAO->if_this_not0_wont_render_gao)
        v8 = 1;
    v15 = EVE_c_UpdateLOD(EVE_gpst_OwnerGAO, 1);
    v9 = 0.0;
    v7 = 0;
    for (a3 = 0; ; ++a3)
    {
        if (a3 >= v12->num)
        {
            *a4 = v9;
            return v7;
        }
        EVE_gpst_CurrentRefMatrix = v13;
        EVE_gpst_CurrentTrack = &v12->listElems[a3];
        EVE_gpst_CurrentParam = &v18[a3];
        if (EVE_gpst_CurrentTrack->gao)
        {
            EVE_gpst_CurrentGAO = EVE_gpst_CurrentTrack->gao;
            if ((EVE_gpst_CurrentGAO->flags & 0x4000000) != 0)
            {
                if ((EVE_gpst_CurrentGAO->flags & 0x10000) != 0)
                    v6 = EVE_gpst_CurrentGAO->matrices + 2;
                else
                    v6 = EVE_gpst_CurrentGAO->matrices + 1;
            }
            else
            {
                v6 = 0;
            }
            EVE_gpst_CurrentRefMatrix = v6;
        }
        else
        {
            EVE_gpst_CurrentGAO = EVE_gpst_OwnerGAO;
        }
        v11 = EVE_gpst_CurrentTrack->boneID == 0xFFFF;
        if ((EVE_gpst_CurrentParam->flag & 1) == 0)
            continue;
        if ((EVE_gpst_CurrentParam->flag & 2) != 0)
            continue;
        v21 = EVE_gpst_CurrentTrack->flags & 0x8000;
        if ((EVE_gpst_CurrentTrack->flags & 0x8000) != 0
            && (v15 & 4) != 0
            && EVE_gpst_CurrentTrack->boneID != 0xFFFF
            && EVE_gpst_CurrentTrack->boneID
            && (EVE_w_Event_InterpolationKey_GetType(EVE_gpst_CurrentTrack->trackElems) & 3) != 0)
        {
            continue;
        }
        if (v8 && (v15 & 0x80) != 0 && !v11)
            break;
        if ((v15 & 1) != 0 && !v11)
            goto LABEL_56;
        if (v21)
        {
            if ((v15 & 8) != 0)
            {
                v14 = EVE_gpst_CurrentTrack->boneID;
                if (!v11 && v19->tablePF->actElemNum > v14)
                {
                    v20 = (tGameObjectMain*)((tableElemStructPF*)(v19->tablePF->firstElemPtr))[v14].objectPtr;
                    if (v20)
                    {
                        if (v20->extended && (v20->extended->extraFlag & 0x20) != 0)
                            continue;
                    }
                }
            }
        }
        if (!v21 || EVE_gpst_CurrentTrack->gao)
            v10 = EVE_RunOneTrack(&v12->listElems[a3], &v18[a3], 1, a4);
        else
            v10 = EVE_RunAnimTrack(EVE_gpst_CurrentTrack, EVE_gpst_CurrentParam, a3, v15, a4);
        if (!a3)
            v7 = v10;
        if (!a3 && v10)
            v9 = *a4;
    }
    ((tGameObjectActorObject*)(EVE_gpst_OwnerGAO->basic->actorObjectOrVisualObject))->initedInANIMainCall = 0;
LABEL_56:
    for (a3a = v12->num - 1; a3a && (v12->listElems[a3a].flags & 0x8000) == 0; --a3a)
    {
        EVE_gpst_CurrentTrack = &v12->listElems[a3a];
        EVE_gpst_CurrentParam = &v18[a3a];
        if (EVE_gpst_CurrentTrack->gao)
            v3 = EVE_gpst_CurrentTrack->gao;
        else
            v3 = EVE_gpst_OwnerGAO;
        EVE_gpst_CurrentGAO = v3;
        if ((v3->flags & 0x4000000) != 0)
        {
            if ((EVE_gpst_CurrentGAO->flags & 0x10000) != 0)
                v5 = EVE_gpst_CurrentGAO->matrices + 2;
            else
                v5 = EVE_gpst_CurrentGAO->matrices + 1;
        }
        else
        {
            v5 = 0;
        }
        EVE_gpst_CurrentRefMatrix = v5;
        EVE_RunOneTrack(&v12->listElems[a3a], &v18[a3a], 1, a4);
    }
    *a4 = v9;
    return v7;
}

int EVE_b_RunAllTracks(tAnimInner* a1, float* a2)
{
    BOOL v3;
    Mtx4x4* v4;
    tGameObjectEventsList* v5;
    Mtx4x4* v6;
    unsigned int i;
    byte v8;
    tAnimTracks* v9;
    float v10;
    int v11;

    v8 = 0;
    if (!a1)
        return 0;
    v5 = a1->eventListStruct;
    if (!a1->eventListStruct)
        return 0;
    v9 = a1->tracks;
    if (!v9)
        return 0;
    v11 = 0;
    v10 = 0.0;
    EVE_gpst_CurrentGAO = EVE_gpst_OwnerGAO;
    v3 = !ENG_gb_EVERunning
        && EVE_gpst_CurrentGAO->basic
        && EVE_gpst_CurrentGAO->basic->actorObjectOrVisualObject
        && ((tGameObjectActorObject*)(EVE_gpst_CurrentGAO->basic->actorObjectOrVisualObject))->skeletonGroup;
    if (v3)
        v8 = EVE_c_UpdateLOD(EVE_gpst_CurrentGAO, 0);
    for (i = 0; i < v5->num; ++i)
    {
        if (v3
            && i > 1
            && !EVE_gb_ForcePlayerOnAllTracks
            && ((EVE_gpst_CurrentGAO->flags2 & 4) != 0 || (EVE_gpst_CurrentGAO->flags2 & 2) == 0)
            && v5->listElems[i].trackElems
            && (v5->listElems[i].trackElems->flags2 & 0x7C0) == 128)
        {
            if (EVE_gpst_CurrentGAO->basic)
            {
                if (EVE_gpst_CurrentGAO->basic->actorObjectOrVisualObject)
                    ((tGameObjectActorObject*)(EVE_gpst_CurrentGAO->basic->actorObjectOrVisualObject))->initedInANIMainCall = 0;
            }
        }
        else if ((v5->listElems[i].flags & 0x8000) == 0
            || (v8 & 4) == 0
            || v5->listElems[i].boneID == 0xFFFF
            || !v5->listElems[i].boneID
            || (EVE_w_Event_InterpolationKey_GetType(v5->listElems[i].trackElems) & 3) == 0)
        {
            v6 = EVE_gpst_CurrentRefMatrix;
            if ((v9[i].flag & 1) != 0 && (v9[i].flag & 2) == 0)
            {
                EVE_gpst_CurrentTrack = &v5->listElems[i];
                EVE_gpst_CurrentParam = &v9[i];
                if (EVE_gpst_CurrentTrack->gao)
                {
                    EVE_gpst_CurrentGAO = EVE_gpst_CurrentTrack->gao;
                    if ((EVE_gpst_CurrentGAO->flags & 0x4000000) != 0)
                    {
                        if ((EVE_gpst_CurrentGAO->flags & 0x10000) != 0)
                            v4 = EVE_gpst_CurrentGAO->matrices + 2;
                        else
                            v4 = EVE_gpst_CurrentGAO->matrices + 1;
                    }
                    else
                    {
                        v4 = 0;
                    }
                    EVE_gpst_CurrentRefMatrix = v4;
                }
                else
                {
                    EVE_gpst_CurrentGAO = EVE_gpst_OwnerGAO;
                }
                EVE_gb_NeedToReparse = 0;
                if (EVE_RunOneTrack(&v5->listElems[i], &v9[i], 1, a2))
                {
                    v11 = 1;
                    if (!i)
                    {
                        if (a2)
                            v10 = *a2;
                    }
                }
                if (EVE_gb_NeedToReparse)
                    i = -1;
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
            EVE_gpst_CurrentRefMatrix = v6;
        }
    }
    if (a2)
        *a2 = v10;
    return v11;
}

struct sAnimInner* EVE_AllocateMainStruct(struct sGameObjectMain* a1)
{
    tAnimInner* result;

    a1->extended->eve = (tAnimInner*)MEM_p_Alloc(sizeof(tAnimInner));
    memset(a1->extended->eve, 0, sizeof(tAnimInner));
    a1->extended->eve->eventListStruct = (tGameObjectEventsList*)MEM_p_Alloc(sizeof(tGameObjectEventsList));
    memset(a1->extended->eve->eventListStruct, 0, sizeof(tGameObjectEventsList));
    a1->extended->eve->eventListStruct->usageCount = 1;
    result = a1->extended->eve;
    result->tracks = 0;
    return result;
}

void EVE_ResolveTrackRef(tGameObjectEventsList* a1, tGameObjectEventsListElem* a2)
{
    tGameObjectEventElem* a1a;
    uint16_t i;

    if (a2->trackElems && (a2->flags & 0x10) != 0)
    {
        EVE_gpst_CurrentReolve = a1;
        for (i = 0; i < (int)a2->elemNum; ++i)
        {
            a1a = &a2->trackElems[i];
            if ((a1a->flags2 & 0x20) != 0)
            {
                a1a->elemPtr = a1->listElems[(__int16)a1a->elemPtr].trackElems[(int16_t) (((*((uint32_t *)a1a->elemPtr)) >> 16) & 0xffff)].elemPtr;
            }
            else if ((a1a->flags2 & 0x7C0) == 64)
            {
                EVE_Event_AIFunction_Resolve(a1a);
            }
        }
    }
}

void EVE_ResolveListTrackRef(tGameObjectEventsList* a1)
{
    int i;

    if (a1 && (a1->flags & 2) == 0)
    {
        a1->flags |= 2u;
        for (i = 0; i < a1->num; ++i)
            EVE_ResolveTrackRef(a1, &a1->listElems[i]);
    }
}

void EVE_ReinitTrack(struct sAnimInner* a1, int trackID)
{
    tGameObjectEventsListElem* v2;
    tAnimTracks* v3;

    if (a1)
    {
        if (a1->eventListStruct)
        {
            if (a1->tracks)
            {
                v2 = &a1->eventListStruct->listElems[trackID];
                v3 = &a1->tracks[trackID];
                if (v3)
                {
                    v3->currTime = 0.0;
                    v3->currElemKeyFrame = 0;
                    v3->flag &= ~1u;
                    if ((v2->flags & 1) != 0)
                        v3->flag |= 1u;
                    v3->flag &= ~2u;
                    v3->flag &= ~4u;
                }
            }
        }
    }
}

void EVE_ReinitData(struct sAnimInner* a1)
{
    uint16_t i;

    if (a1 && a1->eventListStruct)
    {
        for (i = 0; i < (int)a1->eventListStruct->num; ++i)
            EVE_ReinitTrack(a1, i);
    }
}

void EVE_ResolveRefs(struct sGameObjectMain* a1, int a2)
{
    if (a1->extended->eve && a1->extended->eve->eventListStruct)
    {
        if (a1->extended->eve->eventListStruct->num)
        {
            a1->extended->eve->tracks = (tAnimTracks*)MEM_p_Alloc(12 * a1->extended->eve->eventListStruct->num);
            memset(a1->extended->eve->tracks, 0, 12 * a1->extended->eve->eventListStruct->num);
        }
        EVE_ResolveListTrackRef(a1->extended->eve->eventListStruct);
        EVE_ReinitData(a1->extended->eve);
        if (a2)
            ++a1->extended->eve->eventListStruct->usageCount;
    }
}

int EVE_Event_InterpolationKey_Load(tGameObjectEventElem* arg0, tGameObjectEventElem* a2, char* a3, byte* elemPtr, tGameObjectEventsListElem* a5, __int16 a6)
{
    int v7;
    int v8;
    unsigned int v9;
    int v10;
    int v11;
    int i;
    UltraCompressedQuat v13;
    Quat4 v14;
    CompressedQuat v14b;
    Quat4* v15;
    CompressedQuat v16;
    Quat4 Src;
    CompressedQuat v18;
    UltraCompressedQuat v19;
    __int16 v20;
    byte* retVal;
    char* a1;
    __int16 v23;

    a1 = a3;
    if ((a6 & 0x2000) != 0)
        v23 = (int)(*(uint16_t*)a5->trackElems->elemPtr) >> 1;
    else
        v23 = LOA_ReadShort(&a1, 0, 1);
    if ((a6 & 0x1000) != 0)
    {
        if ((a6 & 0x2000) != 0)
            v20 = (((*(uint32_t*)a5->trackElems->elemPtr) >> 16) & 0xffff);
        else
            v20 = EVE_w_Event_InterpolationKey_GetType(a5->trackElems);
    }
    else
    {
        v20 = LOA_ReadShort(&a1, 0, 1);
    }
    arg0->elemPtr = elemPtr;
    if ((v20 & 0x780) != 0)
    {
        Src.q1 = 0.0;
        Src.q2 = 0.0;
        Src.q3 = 0.0;
        Src.q4 = 0.0;
        if ((v20 & 0x80) != 0)
        {
            LOA_ReadCompressedQuaternion(&a1, &v18, 1);
        }
        else
        {
            v15 = EVE_pst_Event_InterpolationKey_GetQuaternion(a2);
            MATH_CompressQuaternion(&v16, v15);
            LOA_ReadUltraCompressedQuaternion(&a1, &v19, 1);
        }
        v10 = v20 & 0x780;
        switch (v10)
        {
        case 0x100:
            v18.s1 = v19.s1 + v16.s1;
            v18.s2 = v19.b2 + v16.s2;
            v18.s3 = v19.b3 + v16.s3;
            break;
        case 0x200:
            v18.s1 = v19.b2 + v16.s1;
            v18.s2 = v19.s1 + v16.s2;
            v18.s3 = v19.b3 + v16.s3;
            break;
        case 0x400:
            v18.s1 = v19.b2 + v16.s1;
            v18.s2 = v19.b3 + v16.s2;
            v18.s3 = v19.s1 + v16.s3;
            break;
        }
        MATH_DecompressQuaternion(&Src, &v18);
        v23 = 20;
        if ((a6 & 0x2100) != 0)
        {
            *(uint16_t*)(arg0->elemPtr) = (2 * v23) | 1; // LOWORD(arg0->elemPtr) = (2 * v23) | 1;
            *(uint16_t*)(arg0->elemPtr + 2) = v20;        // HIWORD(arg0->elemPtr) = v20;
            memcpy(elemPtr, &Src, sizeof(Quat4));
        }
        else
        {
            retVal = arg0->elemPtr;
            *(uint16_t*)retVal = v23;
            retVal += 2;
            *(uint16_t*)retVal = v20;
            retVal += 2;
            memcpy(retVal, &Src, sizeof(Quat4));
        }
        return a1 - a3;
    }
    if ((v20 & 0xF800) != 0)
    {
        v14.q1 = 0.0f;
        v14.q2 = 0.0f;
        v14.q3 = 0.0f;
        v9 = v20 & 0xF800;
        if (v9 > 0x2000)
        {
            if (v9 == 0x4000)
            {
                LOA_ReadUltraCompressedQuaternion(&a1, &v13, 1);
                MATH_calc_curr_quat_from_dt_4000(&v14, &v13);
            }
            else if (v9 == 0x8000)
            {
                LOA_ReadUltraCompressedQuaternion(&a1, &v13, 1);
                MATH_calc_curr_quat_from_dt_8000(&v14, &v13);
            }
        }
        else
        {
            switch (v9)
            {
            case 0x2000u:
                LOA_ReadUltraCompressedQuaternion(&a1, &v13, 1);
                MATH_calc_curr_quat_from_dt_2000(&v14, &v13);
                break;
            case 0x800u:
                LOA_ReadCompressedQuaternion(&a1, &v14b, 1);
                MATH_calc_curr_quat_from_dt_800(&v14, &v14b);
                break;
            case 0x1000u:
                LOA_ReadCompressedQuaternion(&a1, &v14b, 1);
                MATH_calc_curr_quat_from_dt_1000(&v14, &v14b);
                break;
            }
        }
        v23 = 16;
        if ((a6 & 0x2100) != 0)
        {
            *(uint16_t*)(arg0->elemPtr) = (2 * v23) | 1; // LOWORD(arg0->elemPtr) = (2 * v23) | 1;
            *(uint16_t*)(arg0->elemPtr + 2) = v20;       // HIWORD(arg0->elemPtr) = v20;
            memcpy(elemPtr, &v14, 12u);
        }
        else
        {
            retVal = arg0->elemPtr;
            *(uint16_t*)retVal = v23;
            retVal += 2;
            *(uint16_t*)retVal = v20;
            retVal += 2;
            memcpy(retVal, &v14, 12u);
        }
        return a1 - a3;
    }
    if ((v20 & 0x17) == 0)
        return a1 - a3;
    if ((a6 & 0x2100) != 0)
    {
        *(uint16_t*)(arg0->elemPtr) = (2 * v23) | 1; // LOWORD(arg0->elemPtr) = (2 * v23) | 1;
        *(uint16_t*)(arg0->elemPtr + 2) = v20;       // HIWORD(arg0->elemPtr) = v20;
        retVal = elemPtr;
    }
    else
    {
        retVal = arg0->elemPtr;
        *(uint16_t*)retVal = v23;
        retVal += 2;
        *(uint16_t*)retVal = v20;
        retVal += 2;
    }
    if ((a6 & 0x2000) != 0)
        v23 -= 2;
    if ((a6 & 0x1000) != 0)
        v23 -= 2;
    if ((v20 & 3) == 0)
    {
        if ((v20 & 0x14) != 0)
        {
            v7 = v20 & 0x14;
            if (v7 == 4)
            {
                LOA_ReadMatrix(&a1, (Mtx4x4*)retVal, 1);
                retVal += 68;
            }
            else if (v7 == 16)
            {
                LOA_ReadQuaternion(&a1, (Quat4*)retVal, 1);
                retVal += 16;
            }
        }
        goto LABEL_64;
    }
    v8 = v20 & 3;
    if (v8 != 1)
    {
        if (v8 != 2)
        {
            if (v8 != 3)
                goto LABEL_64;
            LOA_ReadVector(&a1, (Vec3*)retVal, 1);
            retVal += 12;
        }
        LOA_ReadVector(&a1, (Vec3*)retVal, 1);
        retVal += 12;
    }
    LOA_ReadVector(&a1, (Vec3*)retVal, 1);
    retVal += 12;
LABEL_64:
    if ((v20 & 8) != 0 && a1 - a3 < v23)
    {
        v11 = LOA_ReadLong(&a1, (uint32_t*)retVal, 1);
        retVal += 4;
        for (i = 0; i < 2 * v11 - 1; ++i)
        {
            LOA_ReadFloat(&a1, (float*)retVal, 1);
            retVal += 4;
        }
    }
    return a1 - a3;
}

void EVE_LoadListEvents(tGameObjectEventsListElem* a5, char* a2, uint32_t* a3, __int16 a4)
{
    unsigned __int8 v4;
    int v5;
    __int16 v6;
    __int16 v7;
    int v8;
    __int16 v9;
    int v10;
    __int16 v11;
    tGameObjectEventElem* v12;
    int v13;
    int v14;
    byte* dataPtr;
    int dataPtrOffset;
    __int16 v17;
    char* v18;
    char* v19;
    int v20;
    __int16 a6;
    unsigned __int16 v22;
    unsigned __int16 i;
    tGameObjectEventElem* v24;
    int v25;
    tGameObjectEventElem* v26;
    __int16 a2a;

    v20 = 0;
    dataPtrOffset = 0;
    v19 = a2;
    dataPtr = 0;
    v26 = 0;
    a5->flags &= ~0x10u;
    a5->elemNum = LOA_ReadLong(&a2, 0, 1);
    a5->trackElems = 0;
    if (a5->elemNum)
    {
        if (a5->size)
        {
            if ((a4 & 0x2000) != 0)
            {
                v18 = a2;
                if ((a4 & 0x200) != 0)
                    LOA_ReadChar(&v18, 0, 4);
                else
                    LOA_ReadShort(&v18, 0, 4);
                LOA_ReadShort(&v18, 0, 4);
                LOA_ReadShort(&v18, &a2a, 4);
                LOA_ReadShort(&v18, &v17, 4);
                if ((v17 & 0x780) != 0)
                {
                    a2a = 16;
                }
                else if ((v17 & 0xF800) != 0)
                {
                    a2a = 12;
                }
                v22 = a2a * a5->elemNum;
                a5->dataPtr = (byte*)MEM_p_Alloc(v22);
                dataPtr = a5->dataPtr;
                a5->trackElems = (tGameObjectEventElem*)MEM_p_Alloc(sizeof(tGameObjectEventElem) * a5->elemNum);
                v26 = a5->trackElems;
            }
            else
            {
                a5->dataPtr = (byte*)MEM_p_Alloc(a5->size);
                dataPtr = a5->dataPtr;
                a5->trackElems = (tGameObjectEventElem*)MEM_p_Alloc(sizeof(tGameObjectEventElem) * a5->elemNum);
                v26 = a5->trackElems;
            }
            v25 = 1;
            LOA_ul_FileTypeSize_31 += a5->size;
        }
        else
        {
            v25 = 0;
            a5->trackElems = (tGameObjectEventElem*)MEM_p_Alloc(sizeof(tGameObjectEventElem) * a5->elemNum);
            v26 = a5->trackElems;
        }
        LOA_ul_FileTypeSize_30 += sizeof(tGameObjectEventElem) * a5->elemNum;
        v24 = v26;
        for (i = 0; i < (int)a5->elemNum; ++i)
        {
            v14 = 0;
            if ((a4 & 0x200) != 0)
            {
                v4 = LOA_ReadChar(&a2, 0, 1);
                v24[i].duration = v4;
            }
            else
            {
                v5 = i;
                v6 = LOA_ReadShort(&a2, 0, 1);
                v24[v5].duration = v6;
            }
            if ((a4 & 0x800) != 0)
            {
                if (i)
                {
                    v24[i].flags2 = v24->flags2;
                    v24[i].flags2 &= ~4u;
                }
                else
                {
                    v7 = LOA_ReadShort(&a2, 0, 1);
                    v24->flags2 = v7;
                }
            }
            else
            {
                v8 = i;
                v9 = LOA_ReadShort(&a2, 0, 1);
                v24[v8].flags2 = v9;
            }
            v24[i].elemPtr = 0;
            v24[i].flags2 &= ~2u;
            if ((v24[i].flags2 & 0x20) != 0)
            {
                a5->flags |= 0x10u;
                v14 = 1;
            }
            v13 = v24[i].flags2 & 0x7C0;
            if (v13 == 0x40)
            {
                v20 = EVE_Event_AIFunction_Load(&v24[i], a2, &dataPtr[dataPtrOffset]);
                a2 += v20;
                if (v20 != 4)
                    dataPtrOffset += v20;
                a5->flags |= 0x10u;
            }
            else if (v13 == 0x80)
            {
                if (i)
                    v12 = &v24[i - 1];
                else
                    v12 = 0;
                if (v14)
                {
                    v10 = LOA_ReadLong(&a2, 0, 1);
                    v24[i].elemPtr = (byte*)v10; // link
                }
                else
                {
                    if (i)
                        v11 = a4;
                    else
                        v11 = 0;
                    a6 = v11;
                    if (!i && (a4 & 0x2000) != 0)
                        a6 |= 0x100u;
                    v20 = EVE_Event_InterpolationKey_Load(&v24[i], v12, a2, &dataPtr[dataPtrOffset], a5, a6);
                    a2 += v20;
                    if ((a4 & 0x2000) != 0)
                        dataPtrOffset = dataPtrOffset + ((int)((*(uint16_t*)(v24[i].elemPtr)) & 0xffff) >> 1) - 4;
                    else
                        dataPtrOffset += *(__int16*)v24[i].elemPtr;
                }
            }
        }
    }
    *a3 = a2 - v19;
}

struct sGameObjectEventsList* EVE_ul_CallbackLoadListTracks(uint32_t Offset)
{
    int v1;
    unsigned __int16 v2;
    __int16 v3;
    __int16 v4;
    unsigned __int16 v5;
    int v6;
    tGameObjectMain* v7;
    __int16 v8;
    int a3;
    char* a1;
    BOOL v12;
    tGameObjectEventsList* v13;
    int v14;
    __int16 a4;
    int a2;
    unsigned int i;
    int v18;

    v13 = (tGameObjectEventsList*)MEM_p_Alloc(sizeof(tGameObjectEventsList));
    LOA_ul_FileTypeSize_32 += sizeof(tGameObjectEventsList);
    v1 = LOA_ul_GetCurrentKey();
    v12 = LOA_IsSpecial(v1);
    if (v12)
        v13->usageCount = 1000000;
    else
        v13->usageCount = 0;
    v14 = 0;
    if (BIG_gi_ReadMode == 1 && !BIG_gi_SpecialRead && v12)
    {
        v14 = 1;
        BIG_gi_ReadMode = 0;
    }
    a1 = BIG_pc_ReadFileTmp(Offset, &a2);
    v2 = LOA_ReadShort(&a1, 0, 1);
    v13->num = v2;
    if (!v13->num)
        v13->num = 0;
    AnimSizeOnDisk += 2;
    v3 = LOA_ReadShort(&a1, 0, 1);
    v13->flags = v3;
    AnimSizeOnDisk += 2;
    v13->flags &= ~2u;
    v13->flags &= ~0x8000u;
    v13->listElems = 0;
    if (v13->num)
    {
        v13->listElems = (tGameObjectEventsListElem*)MEM_p_Alloc(sizeof(tGameObjectEventsListElem) * v13->num);
        LOA_ul_FileTypeSize_33 += sizeof(tGameObjectEventsListElem) * v13->num;
        memset(v13->listElems, 0, sizeof(tGameObjectEventsListElem) * v13->num);
    }
    for (i = 0; i < v13->num; ++i)
    {
        ++NumTracks;
        v4 = LOA_ReadShort(&a1, 0, 1);
        v13->listElems[i].flags = v4;
        AnimSizeOnDisk += 2;
        a4 = v13->listElems[i].flags & 0xFF00;
        a4 &= ~0x4000u;
        a4 &= ~0x8000u;
        v18 = v13->listElems[i].flags & 0x8000;
        v13->listElems[i].flags &= ~0x3F00u;
        v5 = LOA_ReadShort(&a1, 0, 1);
        v13->listElems[i].boneID = v5;
        AnimSizeOnDisk += 2;
        v6 = LOA_ReadLong(&a1, 0, 1);
        v13->listElems[i].size = v6;
        AnimSizeOnDisk += 4;
        if (v18)
        {
            if (!a4)
            {
                v8 = LOA_ReadLong(&a1, 0, 1);
                v13->listElems[i].s3 = v8;
                AnimSizeOnDisk += 4;
            }
        }
        else
        {
            v7 = (tGameObjectMain*)LOA_ReadLong(&a1, 0, 1);
            v13->listElems[i].gao = v7;
            AnimSizeOnDisk += 4;
            if (v13->listElems[i].gao)
                LOA_MakeFileRef((int)v13->listElems[i].gao, &v13->listElems[i].gao, OBJ_ul_GameObjectCallback, 1);
            LOA_ReadString(&a1, 0, 15, 2);
            LOA_ReadChar(&a1, 0, 2);
            AnimSizeOnDisk += 16;
        }
        EVE_LoadListEvents(&v13->listElems[i], a1, &a3, a4);
        a1 += a3;
        AnimSizeOnDisk += a3;
    }
    if (v14)
        BIG_gi_ReadMode = 1;
    return v13;
}

void EVE_MainCall(struct sWorld* a1)
{
    BOOL v1;
    Mtx4x4* v2;
    tGameObjectMain* v3;
    tableElemStructPF* v4;
    tableElemStructPF* v5;

    v4 = a1->eot.unkEOTTable7PF.firstElemPtr;
    v5 = ((tableElemStructPF*)(a1->eot.unkEOTTable7PF.nextElemPtr)) - 1;
    while (v4 <= v5)
    {
        v3 = (tGameObjectMain*)v4->objectPtr;
        v1 = !v4->objectPtr || ((int)v4->objectPtr & 3) == 1;
        if (!v1
            && v3->extended
            && v3->extended->eve
            && ((v3->extended->extraFlag & 4) == 0 || v3->distance)
            && (v3->flags2 & 0x400000) == 0)
        {
            EVE_gpst_OwnerGAO = (tGameObjectMain*)v4->objectPtr;
            EVE_gpst_CurrentData = v3->extended->eve;
            EVE_gpst_CurrentListTracks = EVE_gpst_CurrentData->eventListStruct;
            if ((v3->flags & 0x4000000) != 0)
            {
                if ((v3->flags & 0x10000) != 0)
                    v2 = v3->matrices + 2;
                else
                    v2 = v3->matrices + 1;
            }
            else
            {
                v2 = 0;
            }
            EVE_gpst_CurrentRefMatrix = v2;
            EVE_b_RunAllTracks(EVE_gpst_CurrentData, 0);
        }
        ++v4;
    }
    EVE_gpst_CurrentTrack = 0;
}
