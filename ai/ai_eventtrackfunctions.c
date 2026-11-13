#include "../events.h"
#include "../gameobject.h"
#include "ai_eventtrackfunctions.h"

void AI_EvalFunc_TRAPlayAll_C(struct sGameObjectMain* a1)
{
    EVE_SetRunningTracks(a1->extended->eve, 1);
}

tAINode* AI_EvalFunc_TRAPlayAll(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_TRAPlayAll_C(v2);
    return a1 + 1;
}

void AI_EvalFunc_TRAStopAll_C(struct sGameObjectMain* a1)
{
    EVE_SetRunningTracks(a1->extended->eve, 0);
}

tAINode* AI_EvalFunc_TRAStopAll(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_TRAStopAll_C(v2);
    return a1 + 1;
}

void AI_EvalFunc_TRAPauseAll_C(tGameObjectMain* a1)
{
    EVE_SetPauseTracks(a1->extended->eve, 1);
}

tAINode* AI_EvalFunc_TRAPauseAll(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_TRAPauseAll_C(v2);
    return a1 + 1;
}

void AI_EvalFunc_TRAUnPauseAll_C(tGameObjectMain* a1)
{
    EVE_SetPauseTracks(a1->extended->eve, 0);
}

tAINode* AI_EvalFunc_TRAUnPauseAll(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_TRAUnPauseAll_C(v2);
    return a1 + 1;
}

void AI_EvalFunc_TRAReinitAll_C(struct sGameObjectMain* a1)
{
    EVE_ReinitData(a1->extended->eve);
}

tAINode* AI_EvalFunc_TRAReinitAll(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_TRAReinitAll_C(v2);
    return a1 + 1;
}

void AI_EvalFunc_TRAPlay_C(struct sGameObjectMain* a1, int a2)
{
    EVE_SetRunningTrack(&a1->extended->eve->tracks[a2], 1);
}

tAINode* AI_EvalFunc_TRAPlay(tAINode* a1)
{
    tGameObjectMain* v2;
    int a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_TRAPlay_C(v2, a2);
    return a1 + 1;
}

void AI_EvalFunc_TRAStop_C(tGameObjectMain* a1, int a2)
{
    EVE_SetRunningTrack(&a1->extended->eve->tracks[a2], 0);
}

tAINode* AI_EvalFunc_TRAStop(tAINode* a1)
{
    tGameObjectMain* v2;
    int a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_TRAStop_C(v2, a2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_TRAPause(tAINode* a1)
{
    tGameObjectMain* v2;
    int v3;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v3 = AI_GetIntFromStack();
    EVE_SetPauseTrack(&v2->extended->eve->tracks[v3], 1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_TRAUnPause(tAINode* a1)
{
    tGameObjectMain* v2;
    int v3;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v3 = AI_GetIntFromStack();
    EVE_SetPauseTrack(&v2->extended->eve->tracks[v3], 0);
    return a1 + 1;
}

tAINode* AI_EvalFunc_TRAReinit(tAINode* a1)
{
    tGameObjectMain* v2;
    int a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetIntFromStack();
    EVE_ReinitTrack(v2->extended->eve, a2);
    return a1 + 1;
}

BOOL AI_EvalFunc_TRAIsStop_C(struct sGameObjectMain* a1, int a2)
{
    return (a1->extended->eve->tracks[a2].flag & 1) == 0;
}

tAINode* AI_EvalFunc_TRAIsStop(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int a2;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_TRAIsStop_C(v3, a2);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_TRAIsStopAll_C(struct sGameObjectMain* a1)
{
    int i;
    tEventData* v3;

    v3 = a1->extended->eve;
    if (!v3
        || !v3->eventListStruct
        || !v3->eventListStruct->listElems
        || v3->eventListStruct->listElems == (tGameObjectEventsListElem*)-1)
    {
        return 1;
    }
    for (i = 0; i < v3->eventListStruct->num; ++i)
    {
        if (v3->eventListStruct->listElems[i].trackElems && (v3->tracks[i].flag & 1) != 0)
            return 0;
    }
    return 1;
}

tAINode* AI_EvalFunc_TRAIsStopAll(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_TRAIsStopAll_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_TRASetEnd_C(struct sGameObjectMain* a1, int a2)
{
    float v2;
    tEventData* v3;
    int v4;

    v3 = a1->extended->eve;
    if (v3->eventListStruct->listElems[a2].elemNum)
    {
        v4 = v3->eventListStruct->listElems[a2].elemNum - 1;
        v3->tracks[a2].currElemKeyFrame = v4;
        v2 = (float)v3->eventListStruct->listElems[a2].trackElems[v4].duration;
        v3->tracks[a2].currTime = (float)(v2 * 1.0 / 60.0);
    }
}

tAINode* AI_EvalFunc_TRASetEnd(tAINode* a1)
{
    tGameObjectMain* v2;
    int a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_TRASetEnd_C(v2, a2);
    return a1 + 1;
}

void AI_EvalFunc_TRASetTime_C(tGameObjectMain* a1, float a2)
{
    EVE_SetTracksTime(a1->extended->eve, a2);
}

tAINode* AI_EvalFunc_TRASetTime(tAINode* a1)
{
    tGameObjectMain* v2;
    float a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_TRASetTime_C(v2, a2);
    return a1 + 1;
}

float AI_EvalFunc_TRAGetTotalTime_C(tGameObjectMain* a1)
{
    float a4;

    EVE_GetTracksTime(a1->extended->eve, 0, 0, &a4);
    return a4;
}

tAINode* AI_EvalFunc_TRAGetTotalTime(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_TRAGetTotalTime_C(v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

float AI_EvalFunc_TRAGetCurrentTime_C(tGameObjectMain* a1)
{
    float a3;

    EVE_GetTracksTime(a1->extended->eve, 0, &a3, 0);
    return a3;
}

tAINode* AI_EvalFunc_TRAGetCurrentTime(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_TRAGetCurrentTime_C(v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

void AI_EvalFunc_TRASetTimeToLabel_C(tGameObjectMain* a1, int a2, int a3)
{
    float a3a;
    tEventData* v4;
    int v5;

    v4 = a1->extended->eve;
    v5 = EVE_i_SearchLabel(&v4->eventListStruct->listElems[a2], a3 << 12, &a3a);
    if (v5 != -1)
    {
        EVE_gpst_CurrentListTracks = v4->eventListStruct;
        EVE_gpst_OwnerGAO = a1;
        EVE_SetAllTimesTo(a3a);
        EVE_gpst_CurrentListTracks = 0;
        EVE_gpst_OwnerGAO = 0;
    }
}

tAINode* AI_EvalFunc_TRASetTimeToLabel(tAINode* a1)
{
    tGameObjectMain* v2;
    int a3;
    int a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_TRASetTimeToLabel_C(v2, a2, a3);
    return a1 + 1;
}