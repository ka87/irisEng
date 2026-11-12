#include "../events.h"
#include "../gameobject.h"
#include "ai_eventtrackfunctions.h"

void AI_EvalFunc_TRAPlayAll_C(struct sGameObjectMain* gao)
{
    EVE_SetRunningTracks(gao->extended->eve, 1);
}

tAINode* AI_EvalFunc_TRAPlayAll(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_TRAPlayAll_C(gaoUltra);
    return node + 1;
}

void AI_EvalFunc_TRAStopAll_C(struct sGameObjectMain* gao)
{
    EVE_SetRunningTracks(gao->extended->eve, 0);
}

tAINode* AI_EvalFunc_TRAStopAll(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_TRAStopAll_C(gaoUltra);
    return node + 1;
}

void AI_EvalFunc_TRAPauseAll_C(tGameObjectMain* gao)
{
    EVE_SetPauseTracks(gao->extended->eve, 1);
}

tAINode* AI_EvalFunc_TRAPauseAll(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_TRAPauseAll_C(gaoUltra);
    return node + 1;
}

void AI_EvalFunc_TRAUnPauseAll_C(tGameObjectMain* gao)
{
    EVE_SetPauseTracks(gao->extended->eve, 0);
}

tAINode* AI_EvalFunc_TRAUnPauseAll(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_TRAUnPauseAll_C(gaoUltra);
    return node + 1;
}

void AI_EvalFunc_TRAReinitAll_C(struct sGameObjectMain* gao)
{
    EVE_ReinitData(gao->extended->eve);
}

tAINode* AI_EvalFunc_TRAReinitAll(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_TRAReinitAll_C(gaoUltra);
    return node + 1;
}

void AI_EvalFunc_TRAPlay_C(struct sGameObjectMain* gao, int trackIdx)
{
    EVE_SetRunningTrack(&gao->extended->eve->tracks[trackIdx], 1);
}

tAINode* AI_EvalFunc_TRAPlay(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int trackIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    trackIdx = AI_GetIntFromStack();
    AI_EvalFunc_TRAPlay_C(gaoUltra, trackIdx);
    return node + 1;
}

void AI_EvalFunc_TRAStop_C(tGameObjectMain* gao, int trackIdx)
{
    EVE_SetRunningTrack(&gao->extended->eve->tracks[trackIdx], 0);
}

tAINode* AI_EvalFunc_TRAStop(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int trackIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    trackIdx = AI_GetIntFromStack();
    AI_EvalFunc_TRAStop_C(gaoUltra, trackIdx);
    return node + 1;
}

tAINode* AI_EvalFunc_TRAPause(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int trackIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    trackIdx = AI_GetIntFromStack();
    EVE_SetPauseTrack(&gaoUltra->extended->eve->tracks[trackIdx], 1);
    return node + 1;
}

tAINode* AI_EvalFunc_TRAUnPause(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int trackIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    trackIdx = AI_GetIntFromStack();
    EVE_SetPauseTrack(&gaoUltra->extended->eve->tracks[trackIdx], 0);
    return node + 1;
}

tAINode* AI_EvalFunc_TRAReinit(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int trackIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    trackIdx = AI_GetIntFromStack();
    EVE_ReinitTrack(gaoUltra->extended->eve, trackIdx);
    return node + 1;
}

BOOL AI_EvalFunc_TRAIsStop_C(struct sGameObjectMain* gao, int trackIdx)
{
    return (gao->extended->eve->tracks[trackIdx].flag & 1) == 0;
}

tAINode* AI_EvalFunc_TRAIsStop(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int trackIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    trackIdx = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_TRAIsStop_C(gaoUltra, trackIdx));
    return node + 1;
}

int AI_EvalFunc_TRAIsStopAll_C(struct sGameObjectMain* gao)
{
    tEventData* eve;
    int i;

    eve = gao->extended->eve;
    if (!eve
        || !eve->eventListStruct
        || !eve->eventListStruct->listElems
        || eve->eventListStruct->listElems == (tGameObjectEventsListElem*)-1)
    {
        return 1;
    }

    for (i = 0; i < eve->eventListStruct->num; ++i)
    {
        if (eve->eventListStruct->listElems[i].trackElems && (eve->tracks[i].flag & 1) != 0)
            return 0;
    }
    return 1;
}

tAINode* AI_EvalFunc_TRAIsStopAll(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_TRAIsStopAll_C(gaoUltra));
    return node + 1;
}

void AI_EvalFunc_TRASetEnd_C(struct sGameObjectMain* gao, int trackIdx)
{
    tEventData* eve;
    int lastKeyFrame;
    float frames;

    eve = gao->extended->eve;
    if (eve->eventListStruct->listElems[trackIdx].elemNum)
    {
        lastKeyFrame = eve->eventListStruct->listElems[trackIdx].elemNum - 1;
        eve->tracks[trackIdx].currElemKeyFrame = lastKeyFrame;
        frames = (float)eve->eventListStruct->listElems[trackIdx].trackElems[lastKeyFrame].duration;
        eve->tracks[trackIdx].currTime = (float)(frames / 60.0);
    }
}

tAINode* AI_EvalFunc_TRASetEnd(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int trackIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    trackIdx = AI_GetIntFromStack();
    AI_EvalFunc_TRASetEnd_C(gaoUltra, trackIdx);
    return node + 1;
}

void AI_EvalFunc_TRASetTime_C(tGameObjectMain* gao, float time)
{
    EVE_SetTracksTime(gao->extended->eve, time);
}

tAINode* AI_EvalFunc_TRASetTime(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float time;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    time = AI_GetFloatFromStack();
    AI_EvalFunc_TRASetTime_C(gaoUltra, time);
    return node + 1;
}

float AI_EvalFunc_TRAGetTotalTime_C(tGameObjectMain* gao)
{
    float totalTime;

    EVE_GetTracksTime(gao->extended->eve, 0, 0, &totalTime);
    return totalTime;
}

tAINode* AI_EvalFunc_TRAGetTotalTime(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_TRAGetTotalTime_C(gaoUltra));
    return node + 1;
}

float AI_EvalFunc_TRAGetCurrentTime_C(tGameObjectMain* gao)
{
    float currentTime;

    EVE_GetTracksTime(gao->extended->eve, 0, &currentTime, 0);
    return currentTime;
}

tAINode* AI_EvalFunc_TRAGetCurrentTime(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_TRAGetCurrentTime_C(gaoUltra));
    return node + 1;
}

void AI_EvalFunc_TRASetTimeToLabel_C(tGameObjectMain* gao, int trackIdx, int labelId)
{
    tEventData* eve;
    float labelTime;

    eve = gao->extended->eve;
    if (EVE_i_SearchLabel(&eve->eventListStruct->listElems[trackIdx], labelId << 12, &labelTime) == -1)
        return;

    EVE_gpst_CurrentListTracks = eve->eventListStruct;
    EVE_gpst_OwnerGAO = gao;
    EVE_SetAllTimesTo(labelTime);
    EVE_gpst_CurrentListTracks = 0;
    EVE_gpst_OwnerGAO = 0;
}

tAINode* AI_EvalFunc_TRASetTimeToLabel(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int trackIdx;
    int labelId;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    labelId = AI_GetIntFromStack();
    trackIdx = AI_GetIntFromStack();
    AI_EvalFunc_TRASetTimeToLabel_C(gaoUltra, trackIdx, labelId);
    return node + 1;
}
